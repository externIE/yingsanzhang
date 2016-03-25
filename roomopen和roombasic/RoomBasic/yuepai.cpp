#include "roombasic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL NotifyQuanPlayers(CBaseRoomData* lpRoomData, int nQuanID,int playeridExcept, UINT nRequest, void* pData, int nLen)
{
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.bNeedEcho = FALSE;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));

	request.head.nRequest = nRequest;
	request.pDataPtr = pData;
	request.nDataLen = nLen;
	
    CQuanData* pQuanData=NULL;
	lpRoomData->m_mapQuanData.Lookup(nQuanID,pQuanData);
	if(pQuanData!=NULL)	
	{
		int userid=0;
		int token = 0;
		POSITION pos = pQuanData->m_mapQuanUser.GetStartPosition();
		while (pos)	
		{
			pQuanData->m_mapQuanUser.GetNextAssoc(pos, userid, token);
			if(playeridExcept==userid)
				continue;

			USER_SOCK us;
			ZeroMemory(&us,sizeof(USER_SOCK));
			if(lpRoomData->m_mapTokenUserSock.Lookup(token,us))
			{
  				context.hSocket = us.sock;
				context.lTokenID = us.token;
				bSendOK = g_pSockServer->SendRequest(context.hSocket, &context, &request);
			}
		}
	}
	return TRUE;
}
void   CQuanData::AddQuanUser(int nUserID,int nTokenID)
{
	m_mapQuanUser.SetAt(nUserID,nTokenID);

}



int CBaseRoomData::GetFreeChairNO(int nTableNO)
{
 	LPTABLE pTable = NULL;
	if(!m_mapTable.Lookup(nTableNO, pTable))
	{
		pTable = new TABLE;
		ZeroMemory(pTable, sizeof(TABLE));
		pTable->th.nTableNO =nTableNO;
		pTable->th.nMinScore=SCORE_MIN;
		pTable->th.nMinDeposit=0;
		m_mapTable.SetAt(nTableNO, pTable);
	}
	return GetFreeChairNO(pTable);
}

int CBaseRoomData::GetFreeTableNOEx( int nFromTableNO)
{	
	int nBeginRandomTableNO=GetBeginRandomTableNO();

 	for(int i=nFromTableNO ;i<nBeginRandomTableNO;i++)
	{	
		LPTABLE lpTable = NULL;
 		if(m_mapTable.Lookup(i,lpTable))
		{
			if(lpTable->th.nStatus==TABLE_STATUS_STATIC && lpTable->th.nPlayerCount==0)
				return  i;
		}
		else
		{
			lpTable = new TABLE;
			ZeroMemory(lpTable, sizeof(TABLE));
			lpTable->th.nTableNO = i;
			lpTable->th.nMinDeposit=0;
			lpTable->th.nMinScore =SCORE_MIN;
			m_mapTable.SetAt(i, lpTable);
			return i;
		}
	}
	return -1;
}

int  CBaseRoomData::GetUserQuanTableNO(int nUserID,int nQuanID,int& nChairNO)
{
	int nTableNO=-1;
	int nOtherID=0;
	LPPLAYER lpPlayer = NULL;
	POSITION pos = m_mapPlayer.GetStartPosition();
	while(pos)
	{
		m_mapPlayer.GetNextAssoc(pos, nOtherID, lpPlayer);
		if(lpPlayer==NULL)
			continue;
		if(nOtherID==nUserID)
			continue;
		if(nQuanID==lpPlayer->pe.nQuanID && lpPlayer->nStatus != PLAYER_STATUS_WALKAROUND &&  lpPlayer->nStatus != PLAYER_STATUS_LOOKON)
		{
			nTableNO=lpPlayer->nTableNO;
			break;
		}
	}
	
	if(nTableNO==-1)
		nTableNO=GetFreeTableNOEx(0);

	if(nTableNO!=-1)
 		nChairNO=GetFreeChairNO(nTableNO);
 
	return nTableNO;
}

int  CBaseRoomData::GetUserQuanTableNOEx(int nUserID,int nQuanID,int& nChairNO)
{
	int nTableNO=-1;
	int nOtherID=0;
	LPPLAYER lpPlayer = NULL;
	POSITION pos = m_mapPlayer.GetStartPosition();
	while(pos)
	{
		m_mapPlayer.GetNextAssoc(pos, nOtherID, lpPlayer);
		if(lpPlayer==NULL)
			continue;
		if(nOtherID==nUserID)
			continue;
		if(nQuanID==lpPlayer->pe.nQuanID && lpPlayer->nStatus != PLAYER_STATUS_WALKAROUND &&  lpPlayer->nStatus != PLAYER_STATUS_LOOKON)
		{
			nTableNO=lpPlayer->nTableNO;
			break;
		}
	}
	
	if(nTableNO==-1)
		nTableNO=GetFreeTableNOEx(0);
	
	if(nTableNO!=-1)
	{
		if (nChairNO>=0 && nChairNO<MAX_CHAIR_COUNT)
		{
			LPTABLE pTable = NULL;
			if(!m_mapTable.Lookup(nTableNO, pTable))
			{
				pTable = new TABLE;
				ZeroMemory(pTable, sizeof(TABLE));
				pTable->th.nTableNO =nTableNO;
				pTable->th.nMinScore=SCORE_MIN;
				pTable->th.nMinDeposit=0;
				m_mapTable.SetAt(nTableNO, pTable);
			}
			if(pTable->nPlayerAry[nChairNO]!=0)	//出了问题，这个椅子已经有人了，则随便找个空位
			{
				nChairNO = GetFreeChairNO(pTable);
			}
		}
		else
		{
			nChairNO=GetFreeChairNO(nTableNO);
		}
	}

	
	return nTableNO;
}

int CBaseRoomData::CopyQuanPlayer(PBYTE pData, int nQuanID)
{
	int  nRet=0;

 	int nSizeofPlayer=sizeof(PLAYER)-sizeof(PLAYER_EXTEND);

	int userid = 0;
	LPPLAYER lpPlayer = NULL;
	LPPLAYER lpPlayerData = LPPLAYER(pData);
	POSITION pos;
	pos  = m_mapPlayer.GetStartPosition();
	while (pos)	
	{
		m_mapPlayer.GetNextAssoc(pos, userid, lpPlayer);
		if(lpPlayer)
		{
			if(lpPlayer->pe.nQuanID ==nQuanID)
			{
				memcpy(lpPlayerData, lpPlayer,nSizeofPlayer);
 				lpPlayerData = (LPPLAYER)((PBYTE)(lpPlayerData)+nSizeofPlayer) ;
 			
				nRet++;
  
			}
		}
	}
	return nRet;
}


int CBaseRoomData::GetQuanPlayerCount(int nQuanID,int& nTableNO)
{
	int  nRet=0;

 	int userid = 0;
	LPPLAYER lpPlayer = NULL;
 	POSITION pos = m_mapPlayer.GetStartPosition();
	while (pos)	
	{
		m_mapPlayer.GetNextAssoc(pos, userid, lpPlayer);
		if(lpPlayer)
		{
			if(lpPlayer->pe.nQuanID ==nQuanID)
			{
  			 	nRet++;

 				if(lpPlayer->nStatus != PLAYER_STATUS_WALKAROUND &&  lpPlayer->nStatus != PLAYER_STATUS_LOOKON )
					nTableNO=lpPlayer->nTableNO;
			}
		}
	}
	return nRet;
}
void CBaseRoomData::CopyQuanTable(PBYTE pData,TABLE* pTable )
{
	PBYTE lpDataPtr = pData;

	if(pTable && pTable->th.nPlayerCount)
	{
 		LPTABLE_HEAD pTableHead = LPTABLE_HEAD(lpDataPtr);
		memcpy(pTableHead, pTable, sizeof(TABLE_HEAD));
		lpDataPtr += sizeof(TABLE_HEAD);
			
		int playercount = 0;
		LPPLAYER_POS pPlayerPos = LPPLAYER_POS(lpDataPtr);
		for(int i = 0; i < MAX_CHAIR_COUNT; i++)
		{
			if(pTable->nPlayerAry[i])
			{
				pPlayerPos->nChairNO = i;
				pPlayerPos->nUserID = pTable->nPlayerAry[i];
				pPlayerPos++;
			
				playercount++;
			}
		}
	 
		lpDataPtr += sizeof(PLAYER_POS) * pTable->th.nPlayerCount;
	}
}

void CBaseRoomData::ClearQuanDataMap()
{
	POSITION  pos= m_mapQuanData.GetStartPosition();
	while (pos != NULL)
	{
		int  nQuanID; 
		CQuanData *   pQuanData;
		m_mapQuanData.GetNextAssoc(pos, nQuanID, pQuanData); 
 		SAFE_DELETE(pQuanData);
	}
	m_mapQuanData.RemoveAll();
}

BOOL CBaseRoomData::IsYuePaiRoom()
{
    return IS_BIT_SET( roomdata.dwManages,ROOM_MNG_SUPPORTYUEPAI);

}


BOOL CBaseSockClient::YP_OnGetQuanTableEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{ 
	LPGET_QUANTABLEEX lpGetQuanTable = LPGET_QUANTABLEEX(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));
	
	if(UwlGetCurrentSeconds()-lpGetQuanTable->nGetTime>GETQUANTABLEEX_TIMEOUT_SECONDS)		
		lpGetQuanTable->nGetReturn=UR_RESPONSE_TIMEOUT;
	
	if(lpGetQuanTable->nGetReturn==UR_FETCH_SUCCEEDED)//通过认证 
	{
		LPGET_QUANTABLEEX lpQt = new GET_QUANTABLEEX;
		ZeroMemory(lpQt, sizeof(GET_QUANTABLEEX));
		memcpy(lpQt,lpGetQuanTable,sizeof(GET_QUANTABLEEX));
		
		g_pSockServer->PutToServer(lpContext,YR_GET_QUANTABLEEX_FETCHED , lpQt, sizeof(GET_QUANTABLEEX));
	}
	else
	{
		lpReqToClient->head.nRequest = lpGetQuanTable->nGetReturn;
		
		BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient);
	}
	return TRUE;
}


BOOL CBaseSockClient::YP_OnSetQuanGameSetOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{ 
	LPYP_SET_QUAN_GAMESET lpGetQuanGameSet = LPYP_SET_QUAN_GAMESET(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));
	
	LPYP_SET_QUAN_GAMESET lpQg = new YP_SET_QUAN_GAMESET;
	ZeroMemory(lpQg, sizeof(YP_SET_QUAN_GAMESET));
	memcpy(lpQg,lpGetQuanGameSet,sizeof(YP_SET_QUAN_GAMESET));
	g_pSockServer->PutToServer(lpContext,YR_SET_QUAN_GAMESET_OK,lpQg, sizeof(YP_SET_QUAN_GAMESET));

	return TRUE;
}


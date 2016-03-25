#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSockClient::CSockClient(const BYTE key[] , const ULONG key_len , DWORD flagEncrypt , DWORD flagCompress) 
	: CSockOpenClient(key, key_len, flagEncrypt, flagCompress)
{

}

CSockClient::CSockClient(int nKeyType, DWORD flagEncrypt, DWORD flagCompress)
	: CSockOpenClient(nKeyType, flagEncrypt, flagCompress)
{
}

CSockClient::~CSockClient()
{

}

BOOL CSockClient::OnEnterRoomOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	LPPLAYER lpPlayer = LPPLAYER(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));
	if(UwlGetCurrentSeconds()-lpPlayer->pe.nEnterTime>ENTERROOM_TIMEOUT_SECONDS)
		return TRUE;

	LPROOMDATA lprd = LPROOMDATA((PBYTE)lpPlayer + sizeof(PLAYER));
	int nRoomID=lprd->nRoomID;

	ROOM_USERS ru;
	ZeroMemory(&ru, sizeof(ru));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		
		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData)){
			lpRoomData = CreateRoomData();
			g_mapRoomDataEx.SetAt(nRoomID, lpRoomData);
		}
	}
	
 	lpRoomData->PostHardAndToken(lpPlayer->nUserID,lpPlayer->pe.szHardID,lpContext->lTokenID);
	if(IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))//�ڲ���
		lpRoomData->PostSecurityToHold(lpPlayer->nUserID,lpPlayer->pe.szVolumeID,lpContext->lTokenID,lpPlayer->nUserType);
		
	LPPLAYER pFind = NULL;
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));

		memcpy(&(lpRoomData->roomdata), lprd, sizeof(ROOMDATA));

		if(!lpRoomData->m_mapPlayer.Lookup(lpPlayer->nUserID, pFind))
		{
			pFind = GetPlayerBlock();
			ZeroMemory(pFind, sizeof(PLAYER));
			lpRoomData->m_mapPlayer.SetAt(lpPlayer->nUserID, pFind);
		}
		memcpy(pFind, lpPlayer, sizeof(PLAYER));

		USER_SOCK us;
		ZeroMemory(&us,sizeof(us));
		us.sock=lpContext->hSocket;
		us.token=lpContext->lTokenID;
		us.nUserID=lpPlayer->nUserID;
		us.dwEnterFlags=lpPlayer->pe.dwEnterFlags;
		lpRoomData->m_mapUserSock.SetAt(us.nUserID,us);
 		lpRoomData->m_mapTokenUserSock.SetAt( us.token,us);		
 		pFind->pe.dwPulseTime = GetTickCount();
		pFind->pe.dwLatestTime = GetTickCount();
		pFind->pe.lTokenID=lpContext->lTokenID;
		lpPlayer->pe.lTokenID = pFind->pe.lTokenID;
 
   		BOOL bIsMember=IS_BIT_SET(lpPlayer->nUserType,USER_TYPE_MEMBER);
		BOOL bLookDark=IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_LOOKDARK);
		BOOL bIsMobile=IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_HANDPHONE);

		if(!bIsMobile)//���ֻ��û����»�ȡIP��ַ
		{
			pFind->pe.dwIPAddr = g_pSockServer->GetClientAddress(lpContext->hSocket, lpContext->lTokenID);
 	 		lpPlayer->pe.dwIPAddr = pFind->pe.dwIPAddr;
		}
	
		if(lpRoomData->IsYuePaiRoom() && lpPlayer->pe.nQuanID > 0 )
		{
			CQuanData* pQuanData=NULL;
			lpRoomData->m_mapQuanData.Lookup( lpPlayer->pe.nQuanID,pQuanData);
			if(pQuanData==NULL)
			{
 				pQuanData=new CQuanData;
				lpRoomData->m_mapQuanData.SetAt(lpPlayer->pe.nQuanID,pQuanData);
			}
			pQuanData->AddQuanUser(lpPlayer->nUserID,lpPlayer->pe.lTokenID);
		}

		ENTER_ROOM_OK enterroomOK;
		ZeroMemory(&enterroomOK,sizeof(enterroomOK));
		enterroomOK.nPlayerCount = lpRoomData->m_mapPlayer.GetCount();//�������������
		enterroomOK.nTableCount=lpRoomData->roomdata.nTableCount;
		enterroomOK.nRoomTokenID=lpPlayer->pe.lTokenID;
		enterroomOK.nRoomPulseInterval=60;
		enterroomOK.dwClientIP=lpPlayer->pe.dwIPAddr;
		enterroomOK.nGiftDeposit=lpPlayer->pe.nGiftDeposit;			//���뷿���õ�����
	
		ru.nUserCount=enterroomOK.nPlayerCount;//�������������
 		int nPlayerMinSize=(sizeof(PLAYER)-sizeof(PLAYER_EXTEND));

		if(bIsMobile && lpRoomData->IsSupportMobile() )//������֧���ֻ�����
		{ 	
			if(IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_YUEPAI) && lpRoomData->IsYuePaiRoom() )//����Լ�Ʒ���
			{
 				int nTableNO=-1;
				TABLE* pTable=NULL;
 				enterroomOK.nPlayerCount=lpRoomData->GetQuanPlayerCount(lpPlayer->pe.nQuanID,nTableNO);//��Ȧ����
				enterroomOK.nActiveTableCount=(nTableNO==-1 ? 0 : 1);
 				int nLen = sizeof(enterroomOK) + enterroomOK.nPlayerCount * nPlayerMinSize  ;
 
				PBYTE pData = new BYTE[nLen];
				ZeroMemory(pData, nLen);

				memcpy(pData, &enterroomOK, sizeof(enterroomOK));
				lpRoomData->CopyQuanPlayer(pData + sizeof(enterroomOK), lpPlayer->pe.nQuanID);
	 			
				lpReqToClient->pDataPtr = pData;
				lpReqToClient->nDataLen = nLen;

				BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient, MSG_RESPONSE, 0, COMPRESS_ZIP);
 			 
 				NotifyQuanPlayers(lpRoomData,lpPlayer->pe.nQuanID,lpPlayer->nUserID,GR_PLAYER_ENTERED, lpPlayer, sizeof(PLAYER)-sizeof(PLAYER_EXTEND));
				NotifyDarkLooker(lpRoomData,0,GR_PLAYER_ENTERED,lpPlayer, sizeof(PLAYER));
			}
			else
			{
  				pFind->nStatus = PLAYER_STATUS_WALKAROUND;
				pFind->nTableNO = -1;
				pFind->nChairNO = -1;
 				enterroomOK.dwEnterOKFlag = FLAG_ENTERROOMOK_MOBILE;
				
				int nLen  = sizeof(enterroomOK) + nPlayerMinSize ;
				PBYTE pData = new BYTE[nLen];
				ZeroMemory(pData, nLen);
 				memcpy(pData, &enterroomOK, sizeof(enterroomOK));
				memcpy(pData+sizeof(enterroomOK),pFind,nPlayerMinSize);

  				lpReqToClient->pDataPtr = pData;
				lpReqToClient->nDataLen = nLen;
 				BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient,MSG_RESPONSE, 0, COMPRESS_ZIP);			
			
				NotifyRoomPlayers(lpRoomData, GR_PLAYER_ENTERED, pFind, sizeof(PLAYER),	lpContext->hSocket, lpContext->lTokenID);
			}
		}
 		else
		{
			int nBeginRandomTableNO=lpRoomData->GetBeginRandomTableNO();

			int nLen=0;
			int tablecount = 0;
			int tableno = 0;
			LPTABLE pTable = NULL;
			POSITION pos  = lpRoomData->m_mapTable.GetStartPosition();
			while (pos)	{
				lpRoomData->m_mapTable.GetNextAssoc(pos, tableno, pTable);
				if(tableno>=nBeginRandomTableNO)
					continue;

				//�жϰ�����Һ��Թ���
				if(pTable && (pTable->th.nPlayerCount || pTable->th.nVisitorCount)){
					nLen += (pTable->th.nPlayerCount * sizeof(PLAYER_POS));
					nLen += (pTable->th.nVisitorCount * sizeof(VISITOR_POS));
					tablecount++;
				}
			}

	 	 	enterroomOK.dwEnterOKFlag|=(lpRoomData->IsSoloRoom()?FLAG_ENTERROOMOK_SOLO:0);
	 	 	enterroomOK.dwEnterOKFlag|=(lpRoomData->IsVariableChair()?FLAG_ENTERROOMOK_VARI:0);
  			enterroomOK.dwEnterOKFlag|=(lpRoomData->IsRandomRoom()?FLAG_ENTERROOMOK_RANDOM:0);
			if(lpPlayer->nTimeCost / 60 >= lpRoomData->GetUnwelcomeExperience())
 				enterroomOK.dwEnterOKFlag|=(lpRoomData->IsGetUnwelcome()?FLAG_ENTERROOMOK_UNWELCOME:0);
 			enterroomOK.dwEnterOKFlag|=(lpRoomData->IsGetClientInfo()?FLAG_ENTERROOMOK_CLIENTINFO:0);

 			enterroomOK.nActiveTableCount = tablecount;


			nLen += sizeof(enterroomOK) + enterroomOK.nPlayerCount * nPlayerMinSize + tablecount* sizeof(TABLE_HEAD);
			PBYTE pData = new BYTE[nLen];
			ZeroMemory(pData, nLen);

			memcpy(pData, &enterroomOK, sizeof(enterroomOK));

			ru.nUserCount=lpRoomData->CopyPlayerMap(pData + sizeof(enterroomOK),  bLookDark);
			lpRoomData->CopyTableMapEx(pData + sizeof(enterroomOK) + enterroomOK.nPlayerCount * nPlayerMinSize, lpPlayer);
 
			lpReqToClient->pDataPtr = pData;
			lpReqToClient->nDataLen = nLen;

			BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient, MSG_RESPONSE, 0, COMPRESS_ZIP);
 		 
			if(!IS_BIT_SET(lpPlayer->nUserType,USER_TYPE_ENTERHIDING))
				NotifyRoomPlayers(lpRoomData, GR_PLAYER_ENTERED, lpPlayer, sizeof(PLAYER),	lpContext->hSocket, lpContext->lTokenID);
 
		}
 		if(lpRoomData->IsRandomRoom() && bLookDark)//������� ������darkLooker;
		{
			lpRoomData->m_mapDarkLooker.SetAt(lpPlayer->nUserID,lpPlayer->pe.lTokenID);
		}
	 	if(lpRoomData->IsYuePaiRoom() && bLookDark)//Լ�Ʒ��� ������darkLooker;
		{
			lpRoomData->m_mapDarkLooker.SetAt(lpPlayer->nUserID,lpPlayer->pe.lTokenID);
		}

		if(bIsMember || bIsMobile)	//�ƶ�����ҽ����������䣬Ҳ�ߵ�һ���ǻ�Ա��� 20140808
		{
			int nLimitUsers=lpRoomData->roomdata.nMaxUsers * 1.1 ;
			if(enterroomOK.nPlayerCount>nLimitUsers)
 				KickOffOneNotMember(lpRoomData, lpPlayer->nUserID,TRUE/*�����ֻ��û� 20140821*/);
		}
    
		////////////////////////////////////////////////////////////////////////////////////////////
		ru.nClientID = g_nClientID;
 		ru.nGameID = lpRoomData->GetGameID();
		ru.nAreaID = lpRoomData->GetAreaID();
		ru.nRoomID = lpRoomData->GetRoomID();
		ru.nUserID = lpPlayer->nUserID;

	}

 	TransmitRoomUsers(lpContext, &ru);

	return TRUE;
}

CBaseRoomData* CSockClient::CreateRoomData()
{
	return new CRoomData;
}
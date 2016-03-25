//#include "stdafx.h"
#include "RoomBasic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGameSvrInfoMap	     g_mapGameSvrInfo;
CCritSec			 g_csGameSvrInfo;

CIntIntMap			 g_mapProxyIP;
CCritSec			 g_csProxyIP;

CFilterWordMap      g_mapFilterWord;
CCritSec			g_csFilterWord;

CChatForbidMap      g_mapChatForbid;
CChatForbidIDMap    g_mapChatForbidID;
CCritSec			g_csChatForbid;

CRoomDataExMap	    g_mapRoomDataEx;
CCritSec			g_csRoomData;

SERVERPULSE_INFO	g_ServerPulseInfo;
CCritSec			g_csServerPulseInfo;

CTokenRoomSockMap	g_mapTokenRoomSock;
CCritSec		g_csTokenRoomSock;

CAskerActionMap     g_mapAddFriendAction;
CCritSec        g_csAddFriendAction;

CKickoffRecordMap		g_mapKickoffRecord;
CCritSec				g_csKickoffRecord;
CPlayerKickoffListMap	g_mapPlayerKickList;
CCritSec				g_csPlayerKickList;
CCritSec		g_csPlayerList;
CPtrList		g_PoolOfPlayer;

CCritSec        g_csCallAction;
CCritSec        g_csChatAction;
CCritSec        g_csTablePwdAction;

CCritSec        g_csInviteAction;
CCritSec		g_csTokenSock;
CCallActionListMap  g_mapCallAction;
CChatActionListMap  g_mapChatAction;
CAskerActionMap     g_mapTablePwdAction;

CAskerActionMap     g_mapInviteAction;
CCritSec	    g_csGameSvrToken;
CClientTokenMap	g_mapGameSvrToken;
CCritSec	    g_csMutexHard;

CMutexHardSmallMap    g_mapMutexHardSmall;
CStringIntMap         g_mapMutexUserID;

CTokenSockMap	g_mapTokenSock;//游戏服务端连接
CBaseRoomServer*	g_pRoomServer = NULL;
CBaseSockClient*	g_pSockClient = NULL;

CIDMap          g_mapLocalIP;
CBaseSockServer*	g_pSockServer = NULL;
HWND            g_hMessageWnd=NULL;

int				g_nOverSeeUserId = 0;
int				g_bOpenOverSeeUser=0;

volatile LONG   g_lNoBufferCount = 0; 
volatile LONG   g_lNoBufferLatest= 0;
int				g_nHostID = 0;
int				g_nClientID = 0;
LONG			g_lClientForbid = 0;
int             g_nRoomPort=PORT_ROOM_SERVER;
int             g_nSysFindCD=0;
int				g_nSysFindScale=0;   
int             g_nModeMoneyDiff=0;   
int             g_nModeBountDiff=0;   
int             g_fModeWinsDiff=0;   
HANDLE			g_hExitServer = NULL;
HANDLE			g_hThrdSend= NULL;
HANDLE			g_hThrdRecv= NULL;
HANDLE			g_hThrdTimer= NULL;
HANDLE           g_hThrdPulse= NULL;
HANDLE			g_hThrdConnect= NULL;
HANDLE           g_hThrdMessage= NULL;
HANDLE           g_hThrdMatch= NULL;
HANDLE           g_hThrdStat= NULL;
HANDLE			g_hThrdSvrPulse= NULL;
UINT			g_uiThrdSend = 0;
UINT			g_uiThrdRecv = 0;
UINT			g_uiThrdTimer = 0;
UINT			g_uiThrdPulse = 0;
UINT			g_uiThrdMessage = 0;
UINT			g_uiThrdMatch = 0;
UINT			g_uiThrdStat = 0;
UINT			g_uiThrdSvrPulse = 0;

UINT			g_uiThrdConnect = 0;
TCHAR			g_szIniFile[MAX_PATH];
TCHAR			g_szIPManFile[MAX_PATH];
TCHAR			g_szKickManFile[MAX_PATH];
TCHAR			g_szCheckServer[MAX_PATH];
TCHAR			g_szLicFile[MAX_PATH];
CClientInfo::~CClientInfo()
{
	
	
}

void CClientInfo::ZeroClientInfo()
{
	ZeroMemory(&cci, sizeof(cci));
	ZeroMemory(tipp,sizeof(tipp));
	ZeroMemory(tras,sizeof(tras));
	mapIP.RemoveAll();
	mapARP.RemoveAll();
	mapARPComb.RemoveAll();
}
void CClientInfo::FillClientInfo(COMMIT_CLIENTINFO* pcc)
{
	memcpy(&cci,pcc,sizeof(cci));
	
	if(cci.nIPCount>0)
	{
		LPTCY_IPINFO lptii=(LPTCY_IPINFO) (PBYTE(pcc)+sizeof(cci));
		for(int i=0;i<cci.nIPCount;i++)
		{
			mapIP.SetAt(lptii->dwIPAddr,*lptii);
			lptii++;
		}
	}
	if(cci.nARPCount>0)
	{	
		CString sKey;
		
		LPTCY_IPINFO lptii=(LPTCY_IPINFO) (PBYTE(pcc)+sizeof(cci)+sizeof(TCY_IPINFO)*cci.nIPCount);
		for(int i=0;i<cci.nARPCount;i++)
		{
			mapARP.SetAt(lptii->dwIPAddr,*lptii);
			
			sKey.Format(_T("%d|%s"),lptii->dwIPAddr,lptii->szPhysAddr);
			mapARPComb.SetAt(sKey,1);
			
			lptii++;
		}
	}
	if(cci.nIPPortCount>0)
	{
		LPTCY_IPPORTINFO lptipp=(LPTCY_IPPORTINFO) (PBYTE(pcc)+sizeof(cci)+sizeof(TCY_IPINFO)*(cci.nIPCount+cci.nARPCount));
		memcpy(tipp,lptipp,sizeof(TCY_IPPORTINFO)*cci.nIPPortCount);
	}
	if(cci.nRASCount>0)
	{
		LPTCY_RASINFO lptras=(LPTCY_RASINFO) (PBYTE(pcc)+sizeof(cci)+sizeof(TCY_IPINFO)*(cci.nIPCount+cci.nARPCount)+sizeof(TCY_IPPORTINFO)*cci.nIPPortCount);
		memcpy(tras,lptras,sizeof(TCY_RASINFO)*cci.nRASCount);
	}
}

int   CClientInfo::GetClientInfoSize()
{
	return sizeof(cci)+sizeof(TCY_IPINFO)*(cci.nIPCount+cci.nARPCount)+sizeof(TCY_IPPORTINFO)*cci.nIPPortCount+sizeof(TCY_RASINFO)*cci.nRASCount;
	
}
void  CClientInfo::CombClientInfo(BYTE* pData)
{
	memcpy(pData,&cci,sizeof(cci));
	
	int i=0;
	DWORD dwIP=0;
	TCY_IPINFO tii;
	ZeroMemory(&tii,sizeof(tii));
	POSITION pos=mapIP.GetStartPosition();
	while(pos)
	{
		mapIP.GetNextAssoc(pos,dwIP,tii);
		memcpy(pData+sizeof(cci)+i*sizeof(tii),&tii,sizeof(tii));
		i++;
	}
	
	pos=mapARP.GetStartPosition();
	while(pos)
	{
		mapARP.GetNextAssoc(pos,dwIP,tii);
		memcpy(pData+sizeof(cci)+i*sizeof(tii),&tii,sizeof(tii));
		i++;
	}
	if(cci.nIPPortCount>0)
		memcpy(pData+sizeof(cci)+sizeof(TCY_IPINFO)* (cci.nIPCount+cci.nARPCount),tipp,sizeof(TCY_IPPORTINFO)* (cci.nIPPortCount));
	if(cci.nRASCount>0)
		memcpy(pData+sizeof(cci)+sizeof(TCY_IPINFO)* (cci.nIPCount+cci.nARPCount)+sizeof(TCY_IPPORTINFO)* (cci.nIPPortCount),tras,sizeof(TCY_RASINFO)* (cci.nRASCount));
}


int ClearAddFriendAction()
{
	CAutoLock lock(&g_csAddFriendAction);
	int nRet=g_mapAddFriendAction.GetCount();
	g_mapAddFriendAction.RemoveAll();
	return nRet;		
}

BOOL SaveAddFriendAction(int nSender,int nPlayer)
{
	ASKER_ACTION aa;
	memset(&aa,0,sizeof(aa));
	CString sKey;
	sKey.Format(_T("%d|%d"),nSender,nPlayer);
    int nNow=UwlGetCurrentSeconds();
	
	CAutoLock lock(&g_csAddFriendAction);
	if(g_mapAddFriendAction.Lookup(sKey,aa))
	{
		if(abs(nNow - aa.nTime) <  DEF_ACTION_INTERVAL )  //间隔小于15秒
			return FALSE;
		else
		{
			if(aa.nCount >=2 ) 
			{
				if(abs(nNow-aa.nTime) > 60 * 60 ) //超过1小时才能继续加好友
				{
					aa.nTime =nNow;
					aa.nCount = 1;
					g_mapAddFriendAction.SetAt(sKey,aa);					
				}
				else
				{
					return FALSE;   
				}
				
			}
			else
			{
				aa.nTime =nNow;
				aa.nCount ++;
				g_mapAddFriendAction.SetAt(sKey,aa);
			}
		}
	}
	else
	{
		aa.nTime =nNow;
		aa.nCount ++;
		g_mapAddFriendAction.SetAt(sKey,aa);
	}
	
	return TRUE;
}

BOOL NotifyOnePlayer(CBaseRoomData* lpRoomData, int playerid,  UINT nRequest, void* pData, int nLen)
{
	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.bNeedEcho = FALSE;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));
	
	request.head.nRequest = nRequest;
	request.pDataPtr = pData;
	request.nDataLen = nLen;
	
	USER_SOCK us;
	ZeroMemory(&us,sizeof(us));
	if(lpRoomData->m_mapUserSock.Lookup(playerid, us))
	{
		context.hSocket = us.sock;
		context.lTokenID = us.token;
		g_pSockServer->SendRequest(context.hSocket, &context, &request);
	}
	return TRUE;
}

BOOL NotifyRoomPlayers(CBaseRoomData* lpRoomData, UINT nRequest, void* pData, int nLen,   SOCKET socExcept, LONG tokenExcept)
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
	
 	USER_SOCK us;
	ZeroMemory(&us,sizeof(USER_SOCK));
	LONG token = 0;
	POSITION pos = lpRoomData->m_mapTokenUserSock.GetStartPosition();
	while (pos)
	{
		lpRoomData->m_mapTokenUserSock.GetNextAssoc(pos, token, us);
 
		if(us.sock == socExcept && us.token == tokenExcept) 
			continue;

		if(IS_BIT_SET(us.dwEnterFlags ,FLAG_ENTERROOM_HANDPHONE)) //不发给手机用户
		{
			switch(nRequest)
			{
			case GR_PLAYER_ENTERED:
			case GR_PLAYER_SEATED:
			case GR_PLAYER_STARTED:
			case GR_PLAYER_UNSEATED:
			case GR_PLAYER_PLAYING:
			case GR_PLAYER_LEFT:
			case GR_PLAYER_FINISHED:
			case GR_PLAYER_LOOKON:
			case GR_PLAYER_UNLOOKON:
			case GR_PLAYER_RESULT:
			case GR_PLAYER_DEPOSIT:
			case GR_PLAYER_SCORE:
			case GR_PLAYER_GAMESTARTUP:
			case GR_PLAYER_GAMEBOUTEND:
			case GR_PLAYER_BOUTEND:
			case GR_PLAYER_LEAVETABLE:
			case GR_PLAYER_CLOTHING:
			case GR_PLAYER_NEWTABLE:
			case GR_CHAT_FROM_ROOM:
			case GR_PLAYER_MEMBER:
			case GR_USER_LEVELUP:
			case GR_SOLORANDOM_PLAYING:
			case GR_MEMBER_LEVELUP:
			case GR_MEMBER_UPDATE:
 				continue;
			default:
				;
			}
		}

	    /////////////////通知长度额外处理//////////////////////////////////////////////////
		if(GR_PLAYER_ENTERED==nRequest)
		{

 			if(IS_BIT_SET(us.dwEnterFlags,FLAG_ENTERROOM_INTER))
				request.nDataLen=sizeof(PLAYER);
			else
				request.nDataLen=sizeof(PLAYER)-sizeof(PLAYER_EXTEND);
		}
		else if( GR_SOLORANDOM_PLAYING ==nRequest || GR_RANDOM_PLAYING ==nRequest)
		{
			if(IS_BIT_SET(us.dwEnterFlags,FLAG_ENTERROOM_INTER))
				request.nDataLen=sizeof(RANDOM_PLAYING);
			else
				request.nDataLen=sizeof(RANDOM_PLAYING)-sizeof(int)*MAX_CHAIR_COUNT;
		}
		context.hSocket = us.sock;
		context.lTokenID = us.token;
		bSendOK = g_pSockServer->SendRequest(context.hSocket, &context, &request);
	}
	return TRUE;
}


BOOL NotifyDarkLooker(CBaseRoomData* lpRoomData,  int playeridExcept,UINT nRequest, void* pData, int nLen)
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
	
    int userid=0;
 	int token = 0;
	POSITION pos = lpRoomData->m_mapDarkLooker.GetStartPosition();
	while (pos)	
	{
		lpRoomData->m_mapDarkLooker.GetNextAssoc(pos, userid, token);
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
	return TRUE;
}


HWND FindLoftSvrWindow()
{
	TCHAR szClsName[64];
	TCHAR szWndName[64];
	sprintf(szClsName,_T("%s%d"),LOFTSVR_CLSNAME, PORT_LOFT_SERVER);
	sprintf(szWndName,_T("%s%d"),LOFTSVR_WNDNAME, PORT_LOFT_SERVER);
	
	return  FindWindow(szClsName,szWndName);
}

BOOL IsSomeValueInFile(LPCTSTR lpszSection,LPCTSTR lpszKey,CString& sValue ,LPCTSTR lpszFileName)
{
	TCHAR szValue[256];
	ZeroMemory(szValue,sizeof(szValue));
	GetPrivateProfileString(lpszSection,lpszKey,_T(""),szValue,sizeof(szValue) ,lpszFileName);
	xyAllTrim(szValue);
	sValue.Format(_T("%s"),szValue);
	return !IS_EMPTY_STR(szValue);
}

BOOL GetSocketTokenIDByUserID(CBaseRoomData* lpRoomData, int nUserID, 
							  SOCKET& hSocket, LONG& lTokenID)
{
	BOOL bFound = FALSE;
	
	USER_SOCK us;
	ZeroMemory(&us,sizeof(us));
	bFound=lpRoomData->m_mapUserSock.Lookup(nUserID, us);
	if (bFound)
	{
		hSocket = us.sock;
		lTokenID =us.token;
	}
	return bFound;
}

int RemoveTokenRoomSock(LONG token)
{
	CAutoLock lock(&g_csTokenRoomSock);
	
	ROOM_SOCK rs;
	ZeroMemory(&rs,sizeof(ROOM_SOCK));
	if(g_mapTokenRoomSock.Lookup(token,rs))
		g_mapTokenRoomSock.RemoveKey(token);
	
	return rs.nRoomID ;
}

BOOL ExchangeChair(CBaseRoomData* lpRoomData, LPTABLE pTable, int chair1, int chair2, int user1, int user2)
{
	LPPLAYER lpPlayer1 = NULL;
	LPPLAYER lpPlayer2 = NULL;
	lpRoomData->m_mapPlayer.Lookup(user1, lpPlayer1);
	lpRoomData->m_mapPlayer.Lookup(user2, lpPlayer2);
	if(lpPlayer1 && lpPlayer2){
		lpPlayer1->nChairNO = chair2;
		lpPlayer2->nChairNO = chair1;
		
		pTable->nPlayerAry[chair1] = user2;
		pTable->nPlayerAry[chair2] = user1;
		
		DWORD ipaddr = pTable->dwIPAddrs[chair1];
		pTable->dwIPAddrs[chair1] = pTable->dwIPAddrs[chair2];
		pTable->dwIPAddrs[chair2] = ipaddr;
		
		int ipconfig = pTable->nIPConfigs[chair1];
		pTable->nIPConfigs[chair1] = pTable->nIPConfigs[chair2];
		pTable->nIPConfigs[chair2] = ipconfig;
		
		float breakoff = pTable->fBreakOffs[chair1];
		pTable->fBreakOffs[chair1] = pTable->fBreakOffs[chair2];
		pTable->fBreakOffs[chair2] = breakoff;
		
		int breakreq = pTable->nBreakReqs[chair1];
		pTable->nBreakReqs[chair1] = pTable->nBreakReqs[chair2];
		pTable->nBreakReqs[chair2] = breakreq;
		
		int netspeed = pTable->nNetSpeeds[chair1];
		pTable->nNetSpeeds[chair1] = pTable->nNetSpeeds[chair2];
		pTable->nNetSpeeds[chair2] = netspeed;
		
		int speedreq = pTable->nSpeedReqs[chair1];
		pTable->nSpeedReqs[chair1] = pTable->nSpeedReqs[chair2];
		pTable->nSpeedReqs[chair2] = speedreq;
	}
	return TRUE;
}

void SetTableAfterFinished(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer)
{
	LPTABLE lpTable = NULL;
	if(lpRoomData->m_mapTable.Lookup(lpPlayer->nTableNO, lpTable) && lpTable)
	{
		if(TABLE_STATUS_PLAYING == lpTable->th.nStatus)
		{
			for(int i = 0; i < MAX_CHAIR_COUNT; i++)
			{
				if(0 == lpTable->nPlayerAry[i]) 
					continue;
				LPPLAYER lpTablePlayer = NULL;
				lpRoomData->m_mapPlayer.Lookup(lpTable->nPlayerAry[i], lpTablePlayer);
				if(lpTablePlayer)
				{
					if(lpTablePlayer->nTableNO == lpTable->th.nTableNO && lpTablePlayer->nChairNO == i)
					{
						if(lpTablePlayer->nUserID ==lpPlayer->nUserID)
						{
							lpTablePlayer->nStatus = PLAYER_STATUS_WALKAROUND;
							lpTablePlayer->nChairNO = -1;
							lpTablePlayer->nTableNO = -1;
							if(lpTable->th.nPlayerCount>0)
								lpTable->th.nPlayerCount--;
							lpTable->nPlayerAry[i] = 0;
						}
						else
						{
							lpTablePlayer->nStatus = PLAYER_STATUS_SEATED;
						}
					}
				}
			}
			for(i = 0; i < MAX_CHAIR_COUNT; i++)
			{
				for(int j = 0; j < MAX_VISITOR_COUNT; j++)
				{ // !!! i 与 j不能搞错
					if(0 == lpTable->nVisitorAry[i][j])
						continue;
					LPPLAYER lpTableVisitor = NULL;
					lpRoomData->m_mapPlayer.Lookup(lpTable->nVisitorAry[i][j], lpTableVisitor);
					if(lpTableVisitor)
					{						
						if(lpTableVisitor->nTableNO == lpTable->th.nTableNO && lpTableVisitor->nChairNO == i)
						{
							lpTableVisitor->nStatus = PLAYER_STATUS_WALKAROUND;
							lpTableVisitor->nChairNO = -1;
							lpTableVisitor->nTableNO = -1;
						}
					}
				}
			}
			lpTable->th.nVisitorCount = 0;
			memset(lpTable->nVisitorAry, 0, sizeof(lpTable->nVisitorAry));
			
			lpTable->dwBreakCount = 0;
			lpTable->th.nStatus = TABLE_STATUS_STATIC;
		}
	}
	
}

void SetChairLeave(int nUserID,TABLE* pTable,int nChairNO)
{
	if(pTable && nChairNO>=0 && nChairNO < MAX_CHAIR_COUNT )
	{
		if(pTable->nPlayerAry[nChairNO]==nUserID && (nUserID!=0))
		{
			pTable->nPlayerAry[nChairNO] = 0;
			if(pTable->th.nPlayerCount>0) 
				pTable->th.nPlayerCount--;
		}
	}
}
void SetRandomTableLeave(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer)
{
	if(lpRoomData->IsRandomRoom())
	{
		int nBeginRandomTableNO=lpRoomData->GetBeginRandomTableNO();
		if(lpPlayer->pe.nRandomTableNO>=nBeginRandomTableNO  && (lpPlayer->nStatus == PLAYER_STATUS_WAITING))
		{
			LPTABLE pRandTable = NULL;
			lpRoomData->m_mapTable.Lookup(lpPlayer->pe.nRandomTableNO, pRandTable);
			SetChairLeave(lpPlayer->nUserID,pRandTable,lpPlayer->pe.nRandomChairNO);
		}
	}
}
void SetTableAsEmptyOrLeave(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer)
{
	SetRandomTableLeave(lpRoomData,lpPlayer);
	
	LPTABLE lpTable = NULL;
	if(lpRoomData->m_mapTable.Lookup(lpPlayer->nTableNO, lpTable) && lpTable)
	{
		if(TABLE_STATUS_PLAYING == lpTable->th.nStatus && lpPlayer->nStatus == PLAYER_STATUS_PLAYING)
		{
			for(int i = 0; i < MAX_CHAIR_COUNT; i++)
			{
				if(0 == lpTable->nPlayerAry[i]) continue;
				LPPLAYER lpTablePlayer = NULL;
				lpRoomData->m_mapPlayer.Lookup(lpTable->nPlayerAry[i], lpTablePlayer);
				if(lpTablePlayer)
				{
					if(lpTablePlayer->nTableNO == lpTable->th.nTableNO 
						&& lpTablePlayer->nChairNO == i)
					{
						lpTablePlayer->nStatus = PLAYER_STATUS_WALKAROUND;
						lpTablePlayer->nChairNO = -1;
						lpTablePlayer->nTableNO = -1;
					}
				}
			}
			lpTable->th.nPlayerCount = 0;
			memset(lpTable->nPlayerAry, 0, sizeof(lpTable->nPlayerAry));
			
			for(i = 0; i < MAX_CHAIR_COUNT; i++){
				for(int j = 0; j < MAX_VISITOR_COUNT; j++){ // !!! i 与 j不能搞错
					if(0 == lpTable->nVisitorAry[i][j]) continue;
					LPPLAYER lpTableVisitor = NULL;
					lpRoomData->m_mapPlayer.Lookup(lpTable->nVisitorAry[i][j], lpTableVisitor);
					if(lpTableVisitor){						
						if(lpTableVisitor->nTableNO == lpTable->th.nTableNO 
							&& lpTableVisitor->nChairNO == i){
							lpTableVisitor->nStatus = PLAYER_STATUS_WALKAROUND;
							lpTableVisitor->nChairNO = -1;
							lpTableVisitor->nTableNO = -1;
						}
					}
				}
			}
			lpTable->th.nVisitorCount = 0;
			memset(lpTable->nVisitorAry, 0, sizeof(lpTable->nVisitorAry));
			
			lpTable->dwBreakCount = 0;
			lpTable->th.nStatus = TABLE_STATUS_STATIC;
		}
		else
		{
			SetChairLeave(lpPlayer->nUserID ,lpTable,lpPlayer->nChairNO);
		}
		
		if(0 == lpTable->th.nPlayerCount)
		{
			lpTable->th.bHavePassword = FALSE;
			memset(lpTable->szPassword, 0, sizeof(lpTable->szPassword));
			
			lpTable->th.nMinScore=SCORE_MIN;
			lpTable->th.nMinDeposit=0;
			
			lpTable->dwBreakCount = 0;
			lpTable->th.nStatus = TABLE_STATUS_STATIC;
		}
	}
	lpPlayer->nStatus = PLAYER_STATUS_WALKAROUND;
	lpPlayer->nChairNO = -1;
	lpPlayer->nTableNO = -1;
}


BOOL NotifyServer(UINT nRequest, void* pData, int nLen)
{
	LPREQUEST pRequest = new REQUEST;
	memset(pRequest, 0, sizeof(REQUEST));
	
	pRequest->head.nRequest = nRequest;
	
	pRequest->pDataPtr = new BYTE[nLen];
	pRequest->nDataLen = nLen;
	memcpy(pRequest->pDataPtr, pData, nLen);
	
	if(!PostThreadMessage(g_uiThrdSend, UM_DATA_TOSEND, 0, (LPARAM)pRequest)){
		UwlClearRequest(pRequest);
		SAFE_DELETE(pRequest);
		return FALSE;
	}else{
		return TRUE;
	}
}
 
BOOL FilterPlayAryByMinDeposit(CBaseRoomData* lpRoomData, LPTABLE lpTable, int aryPlayer[])
{
	int nMinDeposit = 0;
	int nAffectedNO = 0;
	
	for(int i = 0; i < MAX_CHAIR_COUNT; i++){
		if(lpTable->nPlayerAry[i]){
			nMinDeposit = lpTable->nMinDeposits[i];
			nAffectedNO = i;
			break;
		}
	}
	for(i = 0; i < MAX_CHAIR_COUNT; i++){
		if(0 == lpTable->nPlayerAry[i]) continue;
		if(lpTable->nMinDeposits[i] > nMinDeposit){
			nMinDeposit = lpTable->nMinDeposits[i];
			nAffectedNO = i;
		}
	}
	for(i = 0; i < MAX_CHAIR_COUNT; i++){
		if(0 == lpTable->nPlayerAry[i]) continue;
		if(i == nAffectedNO){ // 最强的自己除外
			continue;
		}
		LPPLAYER lpTablePlayer = NULL;
		lpRoomData->m_mapPlayer.Lookup(lpTable->nPlayerAry[i], lpTablePlayer);
		if(lpTablePlayer){
			if(lpTablePlayer->nTableNO == lpTable->th.nTableNO 
				&& lpTablePlayer->nChairNO == i){
				if(lpTablePlayer->nDeposit < nMinDeposit){
					aryPlayer[i] = 0;
				}
			}
		}
	}
	for(i = 0; i < MAX_CHAIR_COUNT; i++){
		if(0 == lpTable->nPlayerAry[i]) continue;
		LPPLAYER lpTablePlayer = NULL;
		lpRoomData->m_mapPlayer.Lookup(lpTable->nPlayerAry[i], lpTablePlayer);
		if(lpTablePlayer){
			if(lpTablePlayer->nDeposit < lpRoomData->roomdata.nMinDeposit)
			{
				aryPlayer[i] = 0;
			}
		}
	}
	return TRUE;
}

BOOL FilterPlayAryByMinScore(CBaseRoomData* lpRoomData, LPTABLE lpTable, int aryPlayer[])
{
	int nMinScore = 0;
	int nAffectedNO = 0;
	
	for(int i = 0; i < MAX_CHAIR_COUNT; i++){
		if(lpTable->nPlayerAry[i]){
			nMinScore = lpTable->nMinScores[i];
			nAffectedNO = i;
			break;
		}
	}
	for(i = 0; i < MAX_CHAIR_COUNT; i++){
		if(0 == lpTable->nPlayerAry[i]) continue;
		if(lpTable->nMinScores[i] > nMinScore){
			nMinScore = lpTable->nMinScores[i];
			nAffectedNO = i;
		}
	}
	for(i = 0; i < MAX_CHAIR_COUNT; i++){
		if(0 == lpTable->nPlayerAry[i]) continue;
		if(i == nAffectedNO){ // 最强的自己除外
			continue;
		}
		LPPLAYER lpTablePlayer = NULL;
		lpRoomData->m_mapPlayer.Lookup(lpTable->nPlayerAry[i], lpTablePlayer);
		if(lpTablePlayer){
			if(lpTablePlayer->nTableNO == lpTable->th.nTableNO 
				&& lpTablePlayer->nChairNO == i){
				if(lpTablePlayer->nScore < nMinScore){
					aryPlayer[i] = 0;
				}
			}
		}
	}
	return TRUE;
}

BOOL SetTableAsCompareScoreDeposit(CBaseRoomData* lpRoomData, LPTABLE lpTable, int aryPlayer[])
{
	for(int i = 0; i < MAX_CHAIR_COUNT; i++){
		if(0 == lpTable->nPlayerAry[i]) continue;
		if(aryPlayer[i]) continue;
		LPPLAYER lpTablePlayer = NULL;
		if(lpRoomData->m_mapPlayer.Lookup(lpTable->nPlayerAry[i], lpTablePlayer)){
			if(lpTablePlayer->nTableNO == lpTable->th.nTableNO 
				&& lpTablePlayer->nChairNO == i){
				lpTablePlayer->nStatus = PLAYER_STATUS_WALKAROUND;
				lpTablePlayer->nChairNO = -1;
				lpTablePlayer->nTableNO = -1;
				if(lpTable->th.nPlayerCount>0)
					lpTable->th.nPlayerCount--;
				lpTable->nPlayerAry[i] = 0;
			}
		}
	}
	if(0 == lpTable->th.nPlayerCount){
		lpTable->th.bHavePassword = FALSE;
		memset(lpTable->szPassword, 0, sizeof(lpTable->szPassword));

		lpTable->th.nMinScore=SCORE_MIN;
		lpTable->th.nMinDeposit=0;

	}
	return TRUE;
}
 
BOOL SaveChatAction( int nSender,int nPlayer)
{
	CHAT_ACTION ca;
	ZeroMemory(&ca,sizeof(ca));
	int nIN = GetPrivateProfileInt(_T("ChatAction"),_T("Interval"),3600, g_szIniFile);
	int nCT = GetPrivateProfileInt(_T("ChatAction"),_T("Players"),10, g_szIniFile);  //1小时内，只能跟10个不同的人聊天。

 	BOOL bOld=FALSE;
	POSITION pos=NULL;
 	int nTime=UwlGetCurrentSeconds();

	CAutoLock lock(&g_csChatAction);
	CChatActionList * pList=NULL;
	g_mapChatAction.Lookup(nSender,pList);
	if(pList==NULL)
	{
		pList=new CChatActionList;
 		g_mapChatAction.SetAt(nSender,pList);
		goto SAVECHATATION;   
	}		
	if(pList->GetCount()==0)
		goto SAVECHATATION;   

    pos= pList->GetHeadPosition();
	while (pos)
	{
 		ca=pList->GetNext(pos);
		if(ca.nPlayer==nPlayer)
		{
			bOld=TRUE;
			break; 
		}
	}

	if(bOld)//聊过的直接添加
	{

	}
	else
	{
		POSITION findPos,statPos;
		BOOL bFind=FALSE;
		BOOL bStat=FALSE;
		pos= pList->GetHeadPosition();
 		while (pos)
		{
			findPos=pos;
 			ca=pList->GetNext(pos);
			if( nTime-ca.nTime > nIN)
			{
				bFind=TRUE;
				break;
			}
		}
		if(bFind)//找到间隔点
		{
			POSITION pos=findPos;
  			pList->GetPrev(pos);
			if(pos==NULL) //间隔点findPos是head
			{
			}
			else
			{
				statPos=pos;
				bStat=TRUE;
			}
		}
		else //找不到间隔点，
		{
 			statPos=pList->GetTailPosition();
			bStat=TRUE;
		}

		if(bStat)
		{
			CIDMap statmap;
			while (statPos)
			{
 				ca=pList->GetPrev(statPos);
				statmap.SetAt(ca.nPlayer,ca.nPlayer);
			}
			if(statmap.GetCount()>=nCT)
				return FALSE;
		}
	}

SAVECHATATION:
 	ca.nTime=nTime;
	ca.nPlayer=nPlayer;
	pList->AddHead(ca);

	return TRUE;
}

int ClearCallAction()
{
	CAutoLock lock(&g_csCallAction);
	int nRet=0;
	int nUserID=0;
	CCallActionList* pList=NULL;
	POSITION pos = g_mapCallAction.GetStartPosition();
	while(pos)
	{
		g_mapCallAction.GetNextAssoc(pos, nUserID, pList);
		nRet+=pList->GetCount();
		pList->RemoveAll();
		SAFE_DELETE(pList);
	}
	g_mapCallAction.RemoveAll();
	return nRet;		
}

BOOL SaveCallAction( int nSender,int nPlayer)
{
	CALL_ACTION ca;
	ZeroMemory(&ca,sizeof(ca));
	int nIN = GetPrivateProfileInt(_T("CallAction"),_T("Interval"),1800, g_szIniFile);
	int nCT = GetPrivateProfileInt(_T("CallAction"),_T("Players"),10, g_szIniFile);  //30分钟，只能呼叫10个不同的人。
	
	BOOL bOld=FALSE;
	POSITION pos=NULL;
	int nTime=UwlGetCurrentSeconds();
	
	CAutoLock lock(&g_csCallAction);
	CCallActionList * pList=NULL;
	g_mapCallAction.Lookup(nSender,pList);
	if(pList==NULL)
	{
		pList=new CCallActionList;
		g_mapCallAction.SetAt(nSender,pList);
		goto SAVECALLATION;   
	}		
	if(pList->GetCount()==0)
		goto SAVECALLATION;   
	
    pos= pList->GetHeadPosition();
	while (pos)
	{
		ca=pList->GetNext(pos);
		if(ca.nPlayer==nPlayer)
		{
			bOld=TRUE;
			break; 
		}
	}
	
	if(bOld)//聊过的直接添加
	{
		
	}
	else
	{
		POSITION findPos,statPos;
		BOOL bFind=FALSE;
		BOOL bStat=FALSE;
		pos= pList->GetHeadPosition();
		while (pos)
		{
			findPos=pos;
			ca=pList->GetNext(pos);
			if( nTime-ca.nTime > nIN)
			{
				bFind=TRUE;
				break;
			}
		}
		if(bFind)//找到间隔点
		{
			POSITION pos=findPos;
			pList->GetPrev(pos);
			if(pos==NULL) //间隔点findPos是head
			{
			}
			else
			{
				statPos=pos;
				bStat=TRUE;
			}
		}
		else //找不到间隔点，
		{
			statPos=pList->GetTailPosition();
			bStat=TRUE;
		}
		
		if(bStat)
		{
			CIDMap statmap;
			while (statPos)
			{
				ca=pList->GetPrev(statPos);
				statmap.SetAt(ca.nPlayer,ca.nPlayer);
			}
			if(statmap.GetCount()>=nCT)
				return FALSE;
		}
	}
	
SAVECALLATION:
	ca.nTime=nTime;
	ca.nPlayer=nPlayer;
	pList->AddHead(ca);
	
	return TRUE;
}

int ClearChatAction()
{
	CAutoLock lock(&g_csChatAction);
	int nRet=0;
	int nUserID=0;
	CChatActionList* pList=NULL;
	POSITION pos = g_mapChatAction.GetStartPosition();
	while(pos)
	{
		g_mapChatAction.GetNextAssoc(pos, nUserID, pList);
		nRet+=pList->GetCount();
		pList->RemoveAll();
		SAFE_DELETE(pList);
	}
 	g_mapChatAction.RemoveAll();
	return nRet;		
}


BOOL SaveTablePwdAction(int nSender,int nPlayer)
{
	ASKER_ACTION aa;
	memset(&aa,0,sizeof(aa));
	CString sKey;
	sKey.Format(_T("%d|%d"),nSender,nPlayer);
    int nNow=UwlGetCurrentSeconds();

  	CAutoLock lock(&g_csTablePwdAction);
  	if(g_mapTablePwdAction.Lookup(sKey,aa))
	{
		if(abs(nNow - aa.nTime) <  DEF_ACTION_INTERVAL )  //间隔小于15秒
  			 return FALSE;
	    else
		{
			if(aa.nCount >=2 ) 
			{
				if(abs(nNow-aa.nTime) > 5 * 60 ) //超过5分钟才能继续问密码
				{
					aa.nTime =nNow;
					aa.nCount = 1;
					g_mapTablePwdAction.SetAt(sKey,aa);					
				}
				else
				{
					return FALSE;   
				}

			}
			else
			{
				aa.nTime =nNow;
				aa.nCount ++;
				g_mapTablePwdAction.SetAt(sKey,aa);
			}
		}
	}
	else
	{
		aa.nTime =nNow;
		aa.nCount ++;
		g_mapTablePwdAction.SetAt(sKey,aa);
	}

  	return TRUE;
}



int ClearTablePwdAction()
{
	CAutoLock lock(&g_csTablePwdAction);
	int nRet=g_mapTablePwdAction.GetCount();
  	g_mapTablePwdAction.RemoveAll();
	return nRet;		
}

BOOL SaveInviteAction(int nSender,int nPlayer)
{
	ASKER_ACTION aa;
	memset(&aa,0,sizeof(aa));
	CString sKey;
	sKey.Format(_T("%d|%d"),nSender,nPlayer);
    int nNow=UwlGetCurrentSeconds();

  	CAutoLock lock(&g_csInviteAction);
  	if(g_mapInviteAction.Lookup(sKey,aa))
	{
		if(abs(nNow - aa.nTime) <  DEF_ACTION_INTERVAL )  //间隔小于15秒
  			 return FALSE;
	    else
		{
			if(aa.nCount >=2 ) 
			{
				if(abs(nNow-aa.nTime) > 5 * 60 ) //超过5分钟才能继续邀请
				{
					aa.nTime =nNow;
					aa.nCount = 1;
					g_mapInviteAction.SetAt(sKey,aa);					
				}
				else
				{
					return FALSE;   
				}

			}
			else
			{
				aa.nTime =nNow;
				aa.nCount ++;
				g_mapInviteAction.SetAt(sKey,aa);
			}
		}
	}
	else
	{
		aa.nTime =nNow;
		aa.nCount ++;
		g_mapInviteAction.SetAt(sKey,aa);
	}

  	return TRUE;
}

int ClearInviteAction()
{
	CAutoLock lock(&g_csInviteAction);
	int nRet=g_mapInviteAction.GetCount();
  	g_mapInviteAction.RemoveAll();
	return nRet;		
}

void RemoveOnePlayer(CBaseRoomData* lpRoomData,LPPLAYER lpPlayer, SOCKET socExcept, LONG tokenExcept)
{
	if(lpPlayer==NULL)
		return ;
	
	lpRoomData->m_mapPlayer.RemoveKey(lpPlayer->nUserID);
	lpRoomData->DeletePrivateTableCache(lpPlayer->nUserID);
	lpRoomData->RemoveClientInfo(lpPlayer->nUserID);	
	
	NotifyPlayerLeft(lpRoomData,lpPlayer,socExcept,tokenExcept);
	
	if(lpPlayer->nStatus == PLAYER_STATUS_LOOKON)
	{
		SetTableAsLookonLeave(lpRoomData, lpPlayer);
	}
	else if(lpPlayer->nStatus == PLAYER_STATUS_WALKAROUND)
	{
		
	}
	else
	{
		SetTableAsEmptyOrLeave(lpRoomData, lpPlayer);
	}
	
	FreePlayerBlock(lpPlayer);
	
}

BOOL NotifyPlayerLeft(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer, SOCKET socExcept, LONG tokenExcept)
{
	PLAYER_POSITION pp;
	memset(&pp,0,sizeof(PLAYER_POSITION));
	pp.nUserID=lpPlayer->nUserID;
	pp.nTableNO=lpPlayer->nTableNO;
	pp.nChairNO=lpPlayer->nChairNO;
	if(lpRoomData->IsDarkRoom() || lpRoomData->IsWebGameRoom() || lpRoomData->IsMatchRoom() || lpRoomData->IsCloakingRoom() )
		NotifyDarkLooker(lpRoomData,pp.nUserID,GR_PLAYER_LEFT, &pp, sizeof(PLAYER_POSITION));
	else if (lpRoomData->IsYuePaiRoom())
	{
		int nQuanID = lpPlayer->pe.nQuanID;
		NotifyQuanPlayers(lpRoomData,nQuanID,pp.nUserID,GR_PLAYER_LEFT, &pp, sizeof(PLAYER_POSITION));
	}
	else
		NotifyRoomPlayers(lpRoomData, GR_PLAYER_LEFT, &pp, sizeof(PLAYER_POSITION),	socExcept, tokenExcept);
	
	return TRUE;
}

void SetCommonTableLeave(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer)
{
	LPTABLE pOldTable = NULL;
	lpRoomData->m_mapTable.Lookup(lpPlayer->nTableNO, pOldTable);
	SetChairLeave(lpPlayer->nUserID ,pOldTable,lpPlayer->nChairNO);
	
}


void SetTableAsLookonLeave(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer)
{
	LPTABLE lpTable = NULL;
	if(lpRoomData->m_mapTable.Lookup(lpPlayer->nTableNO, lpTable) && lpTable){
		for(int i = 0; i < MAX_VISITOR_COUNT; i++){
			if(lpPlayer->nUserID == lpTable->nVisitorAry[lpPlayer->nChairNO][i]){
				lpTable->nVisitorAry[lpPlayer->nChairNO][i] = 0;
				lpTable->th.nVisitorCount--;
				break;
			}
		}
	}
	lpPlayer->nStatus = PLAYER_STATUS_WALKAROUND;
	lpPlayer->nChairNO = -1;
	lpPlayer->nTableNO = -1;
}
BOOL KickOffDeadPlayersInRoom(CBaseRoomData* lpRoomData, int deadtime, BOOL bStatic,BOOL bAtMorning)
{	
	int roomid = lpRoomData->roomdata.nRoomID;
	int gameid=  lpRoomData->roomdata.nGameID;
    if(IS_BIT_SET(lpRoomData->roomdata.dwManages,ROOM_MNG_FORBID_KICKOFF_ROOM))
		return FALSE;
	int isClearInterHall= GetPrivateProfileInt(
										_T("KickOff"),			// section name
										_T("isClearInterHall"),			// key name
										1,		// default int
										g_szIniFile				// initialization file name
										);
	int userid = 0;
	LPPLAYER lpPlayer = NULL;
	POSITION pos = lpRoomData->m_mapPlayer.GetStartPosition();
	while (pos)	{
		lpRoomData->m_mapPlayer.GetNextAssoc(pos, userid, lpPlayer);
		
		if(IS_BIT_SET( lpPlayer->nUserType,USER_TYPE_ADMIN ))// 管理员
			continue;
		if(isClearInterHall==0 && IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))//内部大厅不踢
			continue;
		if(lpPlayer->nStatus == PLAYER_STATUS_PLAYING || lpPlayer->nStatus == PLAYER_STATUS_LOOKON)
			continue;
 
 		if(lpPlayer->nStatus == PLAYER_STATUS_READY || lpPlayer->nStatus == PLAYER_STATUS_STAND)
			continue;

		if(lpRoomData->IsSoloRoom() && lpPlayer->nStatus == PLAYER_STATUS_WAITING  && !bAtMorning) //非早上清理时间,solo房间(针对开了客户端的)不踢"不活动玩家"。
 		   bStatic=FALSE;
 
		DWORD pulse = lpPlayer->pe.dwPulseTime;
		DWORD latest = lpPlayer->pe.dwLatestTime;
		
		if(( pulse && (GetTickCount() - pulse >= deadtime * 60 * 1000)) // 僵尸玩家
			|| ( bStatic && latest && (GetTickCount() - latest >= deadtime * 60 * 1000) ))// 不活动玩家
		{ 
			{			 
				if(lpRoomData->IsSoloRoom())
				{
					if(lpPlayer->nStatus == PLAYER_STATUS_WAITING)
					{
						lpRoomData->PostKickoffPlayer(lpPlayer->nUserID,lpPlayer->nTableNO,lpPlayer->nChairNO);
						OverSeeUserData(lpPlayer->nUserID,"KickOffDeadPlayersInRoom::CommitTPS_SOLO");
						lpRoomData->CommitTPS_SOLO(lpPlayer->nUserID,lpPlayer->nTableNO,PS_WALKAROUND);
					}
				}

				SOCKET sock = 0;
				LONG token = 0;
				if(GetSocketTokenIDByUserID(lpRoomData, userid, sock, token))
				{
				 	lpRoomData->m_mapUserSock.RemoveKey(userid);
  					lpRoomData->m_mapTokenUserSock.RemoveKey(token);
					g_pSockServer->CloseClient(sock, token);
					RemoveTokenRoomSock(token);
				}
				RemoveOnePlayer(lpRoomData,lpPlayer,sock,token);

			}
		}				
	}

	int nBeginRandomTableNO=lpRoomData->GetBeginRandomTableNO();

	int tableno = 0;
	LPTABLE lpTable = NULL;
	pos = lpRoomData->m_mapTable.GetStartPosition();
	while (pos)	{
		lpRoomData->m_mapTable.GetNextAssoc(pos, tableno, lpTable);
		if(tableno>=nBeginRandomTableNO)
			continue;
		if(lpTable){					
			for(int i = 0; i < MAX_CHAIR_COUNT; i++){
				userid = lpTable->nPlayerAry[i];
				if(0 == userid) continue;
				LPPLAYER lpTablePlayer = NULL;
				BOOL bToClear = FALSE;
				if(!lpRoomData->m_mapPlayer.Lookup(userid, lpTablePlayer)){
					bToClear = TRUE;
				}else if(lpTablePlayer){
					if(lpTablePlayer->nTableNO != lpTable->th.nTableNO
					   || lpTablePlayer->nChairNO != i){
						bToClear = TRUE;
					}
				}
				if(bToClear){
					SetTableAsEmptyOrLeaveEx(lpRoomData, lpTable, i, userid);
				}
			}
			for(i = 0; i < MAX_CHAIR_COUNT; i++){
				for(int j = 0; j < MAX_VISITOR_COUNT; j++){ // !!! i 与 j不能搞错
					userid = lpTable->nVisitorAry[i][j];
					if(0 == userid) continue;
					LPPLAYER lpTableVisitor = NULL;
					BOOL bToClear = FALSE;
					if(!lpRoomData->m_mapPlayer.Lookup(userid, lpTableVisitor)){
						bToClear = TRUE;
					}else if(lpTableVisitor){
						if(lpTableVisitor->nTableNO != lpTable->th.nTableNO
						   || lpTableVisitor->nChairNO != i){
							bToClear = TRUE;
						}
					}
					if(bToClear){
						SetTableAsLookonLeaveEx(lpRoomData, lpTable, i, userid);
					}
				}
			}
		}
	}
	return TRUE;
}

void SetTableAsEmptyOrLeaveEx(CBaseRoomData* lpRoomData, LPTABLE lpTable, int chairno, int userid)
{
	if(TABLE_STATUS_PLAYING == lpTable->th.nStatus){
		for(int i = 0; i < MAX_CHAIR_COUNT; i++){
			if(0 == lpTable->nPlayerAry[i]) continue;
			LPPLAYER lpTablePlayer = NULL;
			lpRoomData->m_mapPlayer.Lookup(lpTable->nPlayerAry[i], lpTablePlayer);
			if(lpTablePlayer){
				if(lpTablePlayer->nTableNO == lpTable->th.nTableNO 
					&& lpTablePlayer->nChairNO == i){
					lpTablePlayer->nStatus = PLAYER_STATUS_WALKAROUND;
					lpTablePlayer->nChairNO = -1;
					lpTablePlayer->nTableNO = -1;
				}
			}
		}
		lpTable->th.nPlayerCount = 0;
		memset(lpTable->nPlayerAry, 0, sizeof(lpTable->nPlayerAry));
		
		for(i = 0; i < MAX_CHAIR_COUNT; i++){
			for(int j = 0; j < MAX_VISITOR_COUNT; j++){ // !!! i 与 j不能搞错
				if(0 == lpTable->nVisitorAry[i][j]) continue;
				LPPLAYER lpTableVisitor = NULL;
				lpRoomData->m_mapPlayer.Lookup(lpTable->nVisitorAry[i][j], lpTableVisitor);
				if(lpTableVisitor){					
					if(lpTableVisitor->nTableNO == lpTable->th.nTableNO 
						&& lpTableVisitor->nChairNO == i){
						lpTableVisitor->nStatus = PLAYER_STATUS_WALKAROUND;
						lpTableVisitor->nChairNO = -1;
						lpTableVisitor->nTableNO = -1;
					}
				}
			}
		}
		lpTable->th.nVisitorCount = 0;
		memset(lpTable->nVisitorAry, 0, sizeof(lpTable->nVisitorAry));
		
		lpTable->dwBreakCount = 0;
		lpTable->th.nStatus = TABLE_STATUS_STATIC;
	}
	else
	{
		SetChairLeave(userid,lpTable,chairno);
	}
	if(0 == lpTable->th.nPlayerCount)
	{
		lpTable->th.bHavePassword = FALSE;
		memset(lpTable->szPassword, 0, sizeof(lpTable->szPassword));
		
		lpTable->th.nMinScore=SCORE_MIN;
		lpTable->th.nMinDeposit=0;
		
		lpTable->dwBreakCount = 0;
		lpTable->th.nStatus = TABLE_STATUS_STATIC;
	}
	
}

void SetTableAsLookonLeaveEx(CBaseRoomData* lpRoomData, LPTABLE lpTable, int chairno, int userid)
{
	for(int i = 0; i < MAX_VISITOR_COUNT; i++){
		if(userid == lpTable->nVisitorAry[chairno][i]){
			lpTable->nVisitorAry[chairno][i] = 0;
			lpTable->th.nVisitorCount--;
			break;
		}
	}
	
}
 
BOOL KickOffOneWalkAround(CBaseRoomData* lpRoomData,LPPLAYER lpPlayer )
{	
	SOCKET sock = 0;
	LONG token = 0;
	if(GetSocketTokenIDByUserID(lpRoomData, lpPlayer->nUserID, sock, token))
	{
		lpRoomData->m_mapUserSock.RemoveKey(lpPlayer->nUserID);
		lpRoomData->m_mapTokenUserSock.RemoveKey(token);
		g_pSockServer->CloseClient(sock, token);
		RemoveTokenRoomSock(token);
	}
	
	RemoveOnePlayer(lpRoomData,lpPlayer,sock,token);
	
	return TRUE;
}

BOOL KickOffRoomPlayer(LPCONTEXT_HEAD lpContext, CBaseRoomData* lpRoomData, int nUserID,BOOL bKickAfterFinished)
{
	LPPLAYER lpPlayer = NULL;
	if(lpRoomData->m_mapPlayer.Lookup(nUserID, lpPlayer) && lpPlayer)
	{
		if(lpPlayer->nStatus != PLAYER_STATUS_PLAYING)
		{
			if(lpRoomData->IsSoloRoom())
			{
				if(lpPlayer->nStatus == PLAYER_STATUS_WAITING || lpPlayer->nStatus == PLAYER_STATUS_LOOKON)
				{
					lpRoomData->PostKickoffPlayer(lpPlayer->nUserID,lpPlayer->nTableNO,lpPlayer->nChairNO);
					
					if(lpPlayer->nStatus== PLAYER_STATUS_WAITING)//已经进入游戏
					{
						OverSeeUserData(lpPlayer->nUserID,"KickOffRoomPlayer::CommitTPS_SOLO");
						lpRoomData->CommitTPS_SOLO(lpPlayer->nUserID,lpPlayer->nTableNO,PS_WALKAROUND);
					}
				}
			}
			
			SOCKET sock = 0;
			LONG token = 0;
			if(GetSocketTokenIDByUserID(lpRoomData, nUserID, sock, token))
			{
				lpRoomData->m_mapUserSock.RemoveKey(nUserID);
				lpRoomData->m_mapTokenUserSock.RemoveKey(token);
				g_pSockServer->CloseClient(sock, token);
				RemoveTokenRoomSock(token);
			}
			RemoveOnePlayer(lpRoomData,lpPlayer,sock,token);
			
		}
		else if(lpPlayer->nStatus == PLAYER_STATUS_PLAYING)//用户正在玩
		{ 
			if(lpRoomData->IsDarkRoom())
			{
				LPDARK_FINISHED lpDarkFinished=new DARK_FINISHED;
				ZeroMemory(lpDarkFinished, sizeof(DARK_FINISHED));
				if(bKickAfterFinished)
					lpDarkFinished->dwDarkFlag |=FLAG_GETFINISHED_KICKOFF;
				
				lpDarkFinished->nGameID = lpRoomData->GetGameID();
				lpDarkFinished->nAreaID = lpRoomData->GetAreaID();
				lpDarkFinished->nRoomID = lpRoomData->GetRoomID();
				
				lpDarkFinished->nChairNO = lpPlayer->nChairNO;
				lpDarkFinished->nTableNO = lpPlayer->nTableNO;
				lpDarkFinished->nUserID = lpPlayer->nUserID;
				lstrcpyn(lpDarkFinished->szHardID ,lpPlayer->pe.szHardID,MAX_HARDID_LEN);
				lpContext->lTokenID=lpPlayer->pe.lTokenID;
				g_pSockServer->PutToServer(lpContext, GR_DARK_FINISHED, lpDarkFinished, sizeof(DARK_FINISHED));
				
			}
			else
			{
				LPGET_FINISHED lpGetFinished = new GET_FINISHED;
				ZeroMemory(lpGetFinished, sizeof(GET_FINISHED));
				if(bKickAfterFinished)
					lpGetFinished->dwGetFlag |=FLAG_GETFINISHED_KICKOFF;
				
				lpGetFinished->nGameID = lpRoomData->GetGameID();
				lpGetFinished->nAreaID = lpRoomData->GetAreaID();
				lpGetFinished->nRoomID = lpRoomData->GetRoomID();
				
				lpGetFinished->nUserID = lpPlayer->nUserID;
				lpGetFinished->nTableNO = lpPlayer->nTableNO;
				lpGetFinished->nChairNO = lpPlayer->nChairNO;
				lstrcpyn(lpGetFinished->szHardID ,lpPlayer->pe.szHardID,MAX_HARDID_LEN);
				lpContext->lTokenID=lpPlayer->pe.lTokenID;
				g_pSockServer->PutToServer(lpContext, GR_GET_FINISHED, lpGetFinished, sizeof(GET_FINISHED));
			}
			//////////////////////////////////////////////////////////////////
			
		}
	}
	return TRUE;
}

void SaveVipKickoffRecord(int nKicker, int nRoomID)
{
	{
		int nCurrentDate = 0;
		int nCurrentTime = 0;
		UwlGetCurrentDateTime(nCurrentDate, nCurrentTime);
		
		KICKOFF_RECORD kr;
		ZeroMemory(&kr, sizeof(kr));
		CString sKey;
		sKey.Format(_T("%d|%d"), nKicker, nRoomID);
		CAutoLock lock(&g_csKickoffRecord);
		if (g_mapKickoffRecord.Lookup(sKey, kr))
		{
			if (kr.nCurrentDate != nCurrentDate)
				kr.nKickCount = 1;
			else
				kr.nKickCount++;
		}
		else
		{
			kr.nKickCount = 1;	
		}
		
		kr.nUserID = nKicker;
		kr.nCurrentDate = nCurrentDate;
		
		g_mapKickoffRecord.SetAt(sKey, kr);
	}
}


void SaveVipKickoffDetail(int nKicker, int nKickedPlayer)
{
	int nUpdateTime = UwlGetCurrentSeconds();
	
	CAutoLock lock(&g_csPlayerKickList);
	
	//踢人者map
	{
		CKickoffDetailMap* pDetailMap = NULL;
		g_mapPlayerKickList.Lookup(nKicker, pDetailMap);
		if(NULL == pDetailMap)
		{
			pDetailMap =new CKickoffDetailMap;
			g_mapPlayerKickList.SetAt(nKicker, pDetailMap); 
		}
		
		KICKOFF_DETAIL kd_Kicker;
		kd_Kicker.nUserID = nKicker;
		kd_Kicker.nOther = nKickedPlayer;
		kd_Kicker.nRole = ROLE_KICKER;
		kd_Kicker.nKickTime = nUpdateTime;
		pDetailMap->SetAt(kd_Kicker.nOther, kd_Kicker);
	}
	
	//被踢者map
	{
		CKickoffDetailMap* pDetailMap2 = NULL;
		g_mapPlayerKickList.Lookup(nKickedPlayer, pDetailMap2);
		if(NULL == pDetailMap2)
		{
			pDetailMap2 =new CKickoffDetailMap;
			g_mapPlayerKickList.SetAt(nKickedPlayer, pDetailMap2); 
		}
		
		KICKOFF_DETAIL kd_Kicked;
		kd_Kicked.nUserID = nKickedPlayer;
		kd_Kicked.nOther = nKicker;
		kd_Kicked.nRole = ROLE_BEENKICKED;
		kd_Kicked.nKickTime = nUpdateTime;
		pDetailMap2->SetAt(kd_Kicked.nOther, kd_Kicked);
	}
}

CString FormatDeposit(int nDeposit)
{
	CString sRet;
	if(nDeposit % 10000 ==0 && nDeposit>0 )
		sRet.Format(_T("%d万"),nDeposit/10000);
	else
		sRet.Format(_T("%d"),nDeposit);
	return sRet;
}


SOCKET FindSocket(LONG token)
{
	SOCKET sock = 0;
	
	CAutoLock lock(&g_csTokenSock);
	
	g_mapTokenSock.Lookup(token, sock);
	
	return sock;
}

LONG FindGameSvrToken(int clientid)
{
	LONG token = 0;
	CAutoLock lock(&g_csGameSvrToken);
	g_mapGameSvrToken.Lookup(clientid, token);
	
	return token;
}
 
BOOL NotifyOneGameSvr(UINT nRequest, void* pData, int nLen, int clientid, BOOL toCompress)
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
	
	LONG token = FindGameSvrToken(clientid);
	if(token){
		SOCKET sock = FindSocket(token);
		if(sock){
			context.hSocket = sock;
			context.lTokenID = token;
			DWORD dwEncrypt = g_pRoomServer->m_flagEncrypt;
			DWORD dwCompress = toCompress ? COMPRESS_ZIP : 0;
			bSendOK = g_pRoomServer->SendRequest(context.hSocket, &context, &request,
				MSG_REQUEST, dwEncrypt, dwCompress);
		}
	}
	return TRUE;
}


BOOL NotifyRoomPlayer(int roomid,int playerid, LPREQUEST lpRequest)
{
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(roomid, lpRoomData))
 			return FALSE;
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		/*
 		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(playerid, pPlayer)) 
			return TRUE;
			*/
		USER_SOCK us;
		ZeroMemory(&us,sizeof(us));
		if(lpRoomData->m_mapUserSock.Lookup(playerid, us))
		{
			CONTEXT_HEAD context;
			ZeroMemory(&context, sizeof(context));
  			context.hSocket = us.sock;
			context.lTokenID = us.token;
			g_pSockServer->SendRequest(context.hSocket, &context, lpRequest);
		}
 	 	
	}
	return TRUE;
}

BOOL ValidateClientInfo(int nClientID)
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.hSocket = g_pSockClient->GetSocket();
	
	VALIDATE_CLIENT vc;
	ZeroMemory(&vc,sizeof(vc));
	vc.nClientID = nClientID; // 
	vc.nClientType = CLIENT_TYPE_ROOM; // 
	
	request.head.nRequest = GR_VALIDATE_CLIENT;
	request.nDataLen = sizeof(vc);
	request.pDataPtr = &vc;
	
	BOOL bTimeout = FALSE;
	BOOL bSendOK = g_pSockClient->SendRequest(&context, &request, &response,bTimeout);
	return bSendOK;
}

void CommitGameUsers(int nStatTime)
{
	int roomid = 0;
	CBaseRoomData* lpRoomData = NULL;
	CRoomDataExMap mapRoomDataEx;
	{
		CAutoLock lock(&g_csRoomData);	
		POSITION pos = g_mapRoomDataEx.GetStartPosition();
		while (pos)	
		{
			g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			mapRoomDataEx.SetAt(roomid, lpRoomData);
		}
	}
	
	CIntIntMap  mapGame;
	POSITION pos = mapRoomDataEx.GetStartPosition();
	while (pos)	
	{
		mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
		if(lpRoomData)
		{
			CAutoLock lock(&(lpRoomData->m_csRoomData));
			
			int nCount=0;
			mapGame.Lookup(lpRoomData->roomdata.nGameID,nCount);
			mapGame.SetAt(lpRoomData->roomdata.nGameID,nCount+lpRoomData->m_mapPlayer.GetCount());
			
		}
	}
	/////////////////////////////////////////////////////////////////////////
	ROOMSVR_USERS ru;
	ZeroMemory(&ru,sizeof(ru));
	ru.dwFlags=FLAG_ROOMSVR_GAMEUSERS;
	ru.nStatTime=nStatTime;
	ru.nRoomSvrID =g_nClientID;
	ru.nItemCount =mapGame.GetCount();
	
	int nLen=sizeof(ROOMSVR_USERS)+ ru.nItemCount * sizeof(ITEM_USERS);
	BYTE* pData=new BYTE[nLen];
	memset(pData,0,nLen);
    memcpy(pData,&ru,sizeof(ru));
	
	int nCopyed= sizeof(ru);
	ITEM_USERS  iu;
	ZeroMemory(&iu,sizeof(iu));
	int nGameID=0;
	int nCount=0;
    pos  = mapGame.GetStartPosition();
	while (pos)	
	{
		mapGame.GetNextAssoc(pos, nGameID, nCount);
		
		iu.nItemID=nGameID;
		iu.nUsers=nCount;
		memcpy(pData +nCopyed,&iu,sizeof(iu));
		nCopyed+=sizeof(iu);
	} 
	
	NotifyServer(GR_ROOMSVR_USERS,pData,nLen);
	SAFE_DELETE_ARRAY(pData);
}

void CommitGroupUsersOnGame(int nStatTime)
{
	CIntIntIntMap mapGame;

	int roomid = 0;
	CBaseRoomData* lpRoomData = NULL;
	CRoomDataExMap mapRoomDataEx;
	{
		CAutoLock lock(&g_csRoomData);	
		POSITION pos = g_mapRoomDataEx.GetStartPosition();
		while (pos)	
		{
			g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			mapRoomDataEx.SetAt(roomid, lpRoomData);
		}
	}

	POSITION pos = mapRoomDataEx.GetStartPosition();
	while (pos)	
	{
		mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
		if(lpRoomData)
		{
			CAutoLock lock(&(lpRoomData->m_csRoomData));
			
			CIntIntMap*  pmapGroup=NULL;
 			mapGame.Lookup(lpRoomData->roomdata.nGameID, pmapGroup);
			if(pmapGroup==NULL)
			{
 				pmapGroup =new CIntIntMap;
				mapGame.SetAt(lpRoomData->roomdata.nGameID, pmapGroup); 
			}
 			int userid = 0;
  			LPPLAYER lpPlayer = NULL;
			POSITION pos = lpRoomData->m_mapPlayer.GetStartPosition();
			while (pos)	
			{
				lpRoomData->m_mapPlayer.GetNextAssoc(pos, userid, lpPlayer);
				int nUsers=0;
				pmapGroup->Lookup(lpPlayer->pe.nAgentGroupID,nUsers);
 				pmapGroup->SetAt(lpPlayer->pe.nAgentGroupID,nUsers+1);
			}
		
		}
	}
	/////////////////////////////////////////////////////////////////////////
	int nSubCount=0;
	int gameid=0;
	CIntIntMap* pmapGroup=NULL;
 	pos = mapGame.GetStartPosition();
	while (pos)	
	{
 		mapGame.GetNextAssoc(pos, gameid, pmapGroup);
		nSubCount+=pmapGroup->GetCount();
	}

	///////////////////////////////////////////////////////////////////////
	/*存放格式：按照游戏顺序来存放，先放一个ITEM_USERS,里面2个字段表示游戏ID和这个游戏上的“组用户数”组合个数N。
    接着依次存放N个“组用户数”组合。然后继续放一个ITEM_USERS开始下一个游戏*/
	///////////////////////////////////////////////////////////////////////
	ROOMSVR_USERS ru;
	ZeroMemory(&ru,sizeof(ru));
	ru.dwFlags=FLAG_ROOMSVR_GROUPUSERS_ONGAME;
 	ru.nStatTime=nStatTime;
	ru.nRoomSvrID =g_nClientID;
	ru.nItemCount =mapGame.GetCount()+nSubCount;
	ru.nSubCount=nSubCount;

	int nLen=sizeof(ROOMSVR_USERS)+ ru.nItemCount* sizeof(ITEM_USERS);
	BYTE* pData=new BYTE[nLen];
	memset(pData,0,nLen);
    memcpy(pData,&ru,sizeof(ru));

	int nCopyed= sizeof(ru);
    pos  = mapGame.GetStartPosition();
	ITEM_USERS  iu;
	ZeroMemory(&iu,sizeof(iu));
	int groupid=0;
	int users=0;
	while (pos)	
	{
  		mapGame.GetNextAssoc(pos, gameid, pmapGroup);

		iu.nItemID=gameid;
		iu.nUsers=pmapGroup->GetCount();
		memcpy(pData +nCopyed,&iu,sizeof(iu));
		nCopyed+=sizeof(iu);


		POSITION subpos=pmapGroup->GetStartPosition();
		while(subpos)
		{
			pmapGroup->GetNextAssoc(subpos, groupid,users);
			iu.nItemID=groupid;
			iu.nUsers=users;
			memcpy(pData +nCopyed,&iu,sizeof(iu));
			nCopyed+=sizeof(iu);
		}
	} 

	NotifyServer(GR_ROOMSVR_USERS,pData,nLen);
	SAFE_DELETE_ARRAY(pData);


	
 	pos = mapGame.GetStartPosition();
	while (pos)	{
		mapGame.GetNextAssoc(pos, gameid, pmapGroup);
		SAFE_DELETE(pmapGroup);
	}
}





void CommitIPUsersOnGame(int nStatTime)
{
	CIntIntIntMap mapGame;

	int roomid = 0;
	CBaseRoomData* lpRoomData = NULL;
	CRoomDataExMap mapRoomDataEx;
	{
		CAutoLock lock(&g_csRoomData);	
		POSITION pos = g_mapRoomDataEx.GetStartPosition();
		while (pos)	
		{
			g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			mapRoomDataEx.SetAt(roomid, lpRoomData);
		}
	}

	POSITION pos = mapRoomDataEx.GetStartPosition();
	while (pos)	
	{
		mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
		if(lpRoomData)
		{
			CAutoLock lock(&(lpRoomData->m_csRoomData));
			
			CIntIntMap*  pmapIP=NULL;
 			mapGame.Lookup(lpRoomData->roomdata.nGameID, pmapIP);
			if(pmapIP==NULL)
			{
 				pmapIP =new CIntIntMap;
				mapGame.SetAt(lpRoomData->roomdata.nGameID, pmapIP); 
			}
 			int userid = 0;
  			LPPLAYER lpPlayer = NULL;
			POSITION pos = lpRoomData->m_mapPlayer.GetStartPosition();
			while (pos)	
			{
				lpRoomData->m_mapPlayer.GetNextAssoc(pos, userid, lpPlayer);
				if(lpPlayer->pe.nAgentGroupID==DEF_AGENT_GROUP ||
				   lpPlayer->pe.nAgentGroupID==GROUPID_MOBILE  ||
				   lpPlayer->pe.nAgentGroupID==GROUPID_IPHONE ) 
				{
					int nCount=0;
					pmapIP->Lookup(lpPlayer->pe.dwIPAddr,nCount);
					nCount++;
					pmapIP->SetAt(lpPlayer->pe.dwIPAddr,nCount);
				}
			}
		
		}
	}
	/////////////////////////////////////////////////////////////////////////
	int nSubCount=0;
	int gameid=0;
	CIntIntMap* pmapIP=NULL;
 	pos = mapGame.GetStartPosition();
	while (pos)	
	{
 		mapGame.GetNextAssoc(pos, gameid, pmapIP);
		nSubCount+=pmapIP->GetCount();
	}

	///////////////////////////////////////////////////////////////////////
	/*存放格式：按照游戏顺序来存放，先放一个ITEM_USERS,里面2个字段表示游戏ID和这个游戏上的“组用户数”组合个数N。
    接着依次存放N个“组用户数”组合。然后继续放一个ITEM_USERS开始下一个游戏*/
	///////////////////////////////////////////////////////////////////////
	ROOMSVR_USERS ru;
	ZeroMemory(&ru,sizeof(ru));
	ru.dwFlags=FLAG_ROOMSVR_IPPUSERS_ONGAME;
 	ru.nStatTime=nStatTime;
	ru.nRoomSvrID =g_nClientID;
	ru.nItemCount =mapGame.GetCount()+nSubCount;
	ru.nSubCount=nSubCount;

	int nLen=sizeof(ROOMSVR_USERS)+ ru.nItemCount* sizeof(ITEM_USERS);
	BYTE* pData=new BYTE[nLen];
	memset(pData,0,nLen);
    memcpy(pData,&ru,sizeof(ru));

	int nCopyed= sizeof(ru);
    pos  = mapGame.GetStartPosition();
	ITEM_USERS  iu;
	ZeroMemory(&iu,sizeof(iu));
	int ip=0;
	int users=0;
	while (pos)	
	{
  		mapGame.GetNextAssoc(pos, gameid, pmapIP);

		iu.nItemID=gameid;
		iu.nUsers=pmapIP->GetCount();
		memcpy(pData +nCopyed,&iu,sizeof(iu));
		nCopyed+=sizeof(iu);


		POSITION subpos=pmapIP->GetStartPosition();
		while(subpos)
		{
			pmapIP->GetNextAssoc(subpos, ip,users);
			iu.nItemID=ip;
			iu.nUsers=users;
			memcpy(pData +nCopyed,&iu,sizeof(iu));
			nCopyed+=sizeof(iu);
		}
	} 

	NotifyServer(GR_ROOMSVR_USERS,pData,nLen);
	SAFE_DELETE_ARRAY(pData);


	
 	pos = mapGame.GetStartPosition();
	while (pos)	{
		mapGame.GetNextAssoc(pos, gameid, pmapIP);
		SAFE_DELETE(pmapIP);
	}
}

void CommitGroupUsers(int nStatTime)
{
	int roomid = 0;
	CBaseRoomData* lpRoomData = NULL;
	CRoomDataExMap mapRoomDataEx;
	{
		CAutoLock lock(&g_csRoomData);	
		POSITION pos = g_mapRoomDataEx.GetStartPosition();
		while (pos)	
		{
			g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			mapRoomDataEx.SetAt(roomid, lpRoomData);
		}
	}

	CIntIntMap  mapGroup;
	POSITION pos = mapRoomDataEx.GetStartPosition();
	while (pos)	
	{
		mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
		if(lpRoomData)
		{
			CAutoLock lock(&(lpRoomData->m_csRoomData));
			
 			int userid = 0;
  			LPPLAYER lpPlayer = NULL;
			POSITION pos = lpRoomData->m_mapPlayer.GetStartPosition();
			while (pos)	
			{
				lpRoomData->m_mapPlayer.GetNextAssoc(pos, userid, lpPlayer);
				int nCount=0;
				mapGroup.Lookup(lpPlayer->pe.nAgentGroupID,nCount);
				nCount++;
				mapGroup.SetAt(lpPlayer->pe.nAgentGroupID,nCount);
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////
	ROOMSVR_USERS ru;
	ZeroMemory(&ru,sizeof(ru));
	ru.dwFlags=FLAG_ROOMSVR_GROUPUSERS;
 	ru.nStatTime=nStatTime;
	ru.nRoomSvrID =g_nClientID;
	ru.nItemCount =mapGroup.GetCount();

	int nLen=sizeof(ROOMSVR_USERS)+ ru.nItemCount * sizeof(ITEM_USERS);
	BYTE* pData=new BYTE[nLen];
	memset(pData,0,nLen);
    memcpy(pData,&ru,sizeof(ru));

	int nCopyed= sizeof(ru);
	ITEM_USERS  iu;
	ZeroMemory(&iu,sizeof(iu));
	int nGroupID=0;
	int nCount=0;
    pos  = mapGroup.GetStartPosition();
 	while (pos)	
	{
  		mapGroup.GetNextAssoc(pos, nGroupID, nCount);

		iu.nItemID=nGroupID;
		iu.nUsers=nCount;
		memcpy(pData +nCopyed,&iu,sizeof(iu));
		nCopyed+=sizeof(iu);
	} 

	NotifyServer(GR_ROOMSVR_USERS,pData,nLen);
	SAFE_DELETE_ARRAY(pData);
}
 


void CommitIPUsers(int nStatTime)
{
	int roomid = 0;
	CBaseRoomData* lpRoomData = NULL;
	CRoomDataExMap mapRoomDataEx;
	{
		CAutoLock lock(&g_csRoomData);	
		POSITION pos = g_mapRoomDataEx.GetStartPosition();
		while (pos)	
		{
			g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			mapRoomDataEx.SetAt(roomid, lpRoomData);
		}
	}

	CIntIntMap  mapIP;
	POSITION pos = mapRoomDataEx.GetStartPosition();
	while (pos)	
	{
		mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
		if(lpRoomData)
		{
			CAutoLock lock(&(lpRoomData->m_csRoomData));
			
 			int userid = 0;
  			LPPLAYER lpPlayer = NULL;
			POSITION pos = lpRoomData->m_mapPlayer.GetStartPosition();
			while (pos)	
			{
				lpRoomData->m_mapPlayer.GetNextAssoc(pos, userid, lpPlayer);
				if(lpPlayer->pe.nAgentGroupID==DEF_AGENT_GROUP ||
				   lpPlayer->pe.nAgentGroupID==GROUPID_MOBILE  ||
				   lpPlayer->pe.nAgentGroupID==GROUPID_IPHONE ) 
				{
					int nCount=0;
					mapIP.Lookup(lpPlayer->pe.dwIPAddr,nCount);
					nCount++;
					mapIP.SetAt(lpPlayer->pe.dwIPAddr,nCount);
				}
			}
		}
	}
   ////////////////////////////////////////////////////////////////////////////
	int nIP=0;
	int nCount=0;
 	CIntIntMap  mapOne,mapTwo;
  	pos  = mapIP.GetStartPosition();
	while (pos)	
	{
  		mapIP.GetNextAssoc(pos, nIP, nCount);
		if(nCount==1)
			mapOne.SetAt(nIP,nCount);
		else
			mapTwo.SetAt(nIP,nCount);
	} 
 	/////////////////////////////////////////////////////////////////////////
	ROOMSVR_IPUSERS ri;
	ZeroMemory(&ri,sizeof(ri));
 	ri.nStatTime=nStatTime;
	ri.nRoomSvrID =g_nClientID;
	ri.nOneCount =mapOne.GetCount();
    ri.nTwoCount =mapTwo.GetCount();

	int nLen=sizeof(ROOMSVR_IPUSERS)+ ri.nTwoCount * sizeof(ITEM_USERS)+ ri.nOneCount* sizeof(int);
	BYTE* pData=new BYTE[nLen];
	memset(pData,0,nLen);
    memcpy(pData,&ri,sizeof(ri));

	int nCopyed= sizeof(ri);
 	pos  = mapOne.GetStartPosition();
	while (pos)	
	{
  		mapOne.GetNextAssoc(pos, nIP, nCount);
 		memcpy(pData +nCopyed,&nIP,sizeof(int));
		nCopyed+=sizeof(int);
	} 
  	ITEM_USERS  iu;
	ZeroMemory(&iu,sizeof(iu));
    pos  = mapTwo.GetStartPosition();
	while (pos)	
	{
  		mapTwo.GetNextAssoc(pos, nIP, nCount);
	
 		iu.nItemID=nIP;
		iu.nUsers=nCount;
		memcpy(pData +nCopyed,&iu,sizeof(iu));
		nCopyed+=sizeof(iu);
	} 

	NotifyServer(GR_ROOMSVR_IPUSERS,pData,nLen);
	SAFE_DELETE_ARRAY(pData);
}

PLAYER* GetPlayerBlock()
{
    PLAYER* pPlayer = NULL;
	
	CAutoLock lock(&g_csPlayerList);
	
	if (g_PoolOfPlayer.GetCount() == 0){
		pPlayer = (PLAYER *)new (PLAYER);
		if (pPlayer == NULL){
			DWORD dwError = GetLastError();
			UwlTrace(_T("new() PLAYER failed. errcode: %ld, errstr: %s.\n"), 
				dwError, UwlGetAPIErrStr(dwError));
		}else{
			
		}
	}else{
		pPlayer	= (PLAYER*)(g_PoolOfPlayer.RemoveHead());
	}	
    return pPlayer;
}


BOOL GetFilterWord()
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	CONTEXT_HEAD context;
	context.hSocket = g_pSockClient->GetSocket();
	context.lSession = 0;
	context.bNeedEcho = TRUE;
	
	request.head.nRequest = GR_GET_FILTERWORD;
	request.nDataLen = 0;
	request.pDataPtr =0;
	
	BOOL bTimeout = FALSE;
	BOOL bSendOK = g_pSockClient->SendRequest(&context, &request, &response, 
		bTimeout, 30*1000);
	if(!bSendOK || UR_FETCH_SUCCEEDED != response.head.nRequest){
		UwlLogFile(_T("GetFilterWord() failed!"));
		UwlClearRequest(&response);
		return FALSE;
	}
	LPFILTER_WORD lpFilterWord = LPFILTER_WORD(response.pDataPtr);
	{
		CAutoLock lock(&g_csFilterWord);
		g_mapFilterWord.RemoveAll();
		int nCount=response.nDataLen / sizeof(FILTER_WORD);
        for(int i = 0; i < nCount; i++)
		{
			FILTER_WORD FilterWord;
			memcpy(&FilterWord,lpFilterWord,sizeof(FILTER_WORD));
			g_mapFilterWord.SetAt(FilterWord.nWordID, FilterWord);
			lpFilterWord++;
		}
	}
	UwlClearRequest(&response);
	UwlLogFile(_T("GetFilterWord() OK!"));
	return TRUE;
}
BOOL GetMutexHard()
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	CONTEXT_HEAD context;
	context.hSocket = g_pSockClient->GetSocket();
	context.lSession = 0;
	context.bNeedEcho = TRUE;
	
	request.head.nRequest = GR_GET_MUTEXHARD;
	request.nDataLen = 0;
	request.pDataPtr =0;
	
	BOOL bTimeout = FALSE;
	BOOL bSendOK = g_pSockClient->SendRequest(&context, &request, &response, 
		bTimeout, 5*60*1000);
	if(!bSendOK || UR_FETCH_SUCCEEDED != response.head.nRequest){
		UwlLogFile(_T("GetMutexHard() failed!"));
		UwlClearRequest(&response);
		return FALSE;
	}
	LPMUTEX_HARD_SMALL lpMutexHardSmall = LPMUTEX_HARD_SMALL(response.pDataPtr);
	{ 
		CString sKey;
		CAutoLock lock(&g_csMutexHard);
		g_mapMutexHardSmall.RemoveAll();
		
		int nCount=response.nDataLen / sizeof(MUTEX_HARD_SMALL);
        for(int i = 0; i < nCount; i++)
		{
			sKey.Format(_T("%s|%s"),lpMutexHardSmall->szMachineID1, lpMutexHardSmall->szMachineID2);
			g_mapMutexHardSmall.SetAt(sKey,*lpMutexHardSmall);
			sKey.Format(_T("%s|%s"),lpMutexHardSmall->szMachineID2, lpMutexHardSmall->szMachineID1);
			g_mapMutexHardSmall.SetAt(sKey,*lpMutexHardSmall);
			///////////////////////////////////////////////
			lpMutexHardSmall++;
		}
	}
	UwlClearRequest(&response);
	UwlLogFile(_T("GetMutexHard() OK!"));
	return TRUE;
}

BOOL KickOffOneNotMember(CBaseRoomData* lpRoomData,int playeridExcept, BOOL bUnKickOffMobile/* = FALSE*/)
{	
	int roomid = lpRoomData->roomdata.nRoomID;
	int gameid=  lpRoomData->roomdata.nGameID;
    if(IS_BIT_SET(lpRoomData->roomdata.dwManages,ROOM_MNG_FORBID_KICKOFF_ROOM))
		return FALSE;
	
	int nKickoffLatest=GetTickCount();
	LPPLAYER lpKickoffPlayer=NULL;
	
	int userid = 0;
	LPPLAYER lpPlayer = NULL;
	POSITION pos = lpRoomData->m_mapPlayer.GetStartPosition();
	while (pos)	{
		lpRoomData->m_mapPlayer.GetNextAssoc(pos, userid, lpPlayer);
		
		if(IS_BIT_SET( lpPlayer->nUserType,USER_TYPE_ADMIN ))// 管理员
			continue;
		
		if(IS_BIT_SET( lpPlayer->nUserType,USER_TYPE_MEMBER ))//会员
			continue;
		
		if(lpPlayer->nUserID == playeridExcept)
			continue;
		
		if (bUnKickOffMobile 
			&& IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_HANDPHONE) 
			&& lpRoomData->IsSupportMobile())	//不踢手机用户 20140821
			continue;
		
		if(lpPlayer->nStatus != PLAYER_STATUS_WALKAROUND)
			continue;
		
		DWORD pulse = lpPlayer->pe.dwPulseTime;
		DWORD latest = lpPlayer->pe.dwLatestTime;
		
		if(pulse && (GetTickCount() - pulse >= 5 * 60 * 1000) )// 5分钟僵尸玩家
		{
			lpKickoffPlayer=lpPlayer;
			break;
		}
		
		if(  latest <= nKickoffLatest )  // 不活动玩家
		{
			nKickoffLatest=latest;
			lpKickoffPlayer=lpPlayer;
		}
	}
	if(lpKickoffPlayer!=NULL)
		KickOffOneWalkAround(lpRoomData,lpKickoffPlayer);
	
	return TRUE;
}

BOOL TransmitRoomUsers(LPCONTEXT_HEAD lpContext, LPROOM_USERS lpRoomUsers)
{
	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	REQUEST request;
	ZeroMemory(&request, sizeof(request));
	
	request.head.nRequest = GR_ROOM_USERS;
	request.pDataPtr = lpRoomUsers;
	request.nDataLen = sizeof(ROOM_USERS);
	return g_pSockServer->TransmitRequest(&context, &request);
}
  
BOOL ReadRangeRandomConfig(CBaseRoomData* lpRoomData,  RANDOM_CONFIG* lpRandomConfig)
{
	if (!lpRoomData || !lpRandomConfig)
		return FALSE;

	TCHAR szSection[16];
	sprintf(szSection, _T("Room%d"), lpRoomData->roomdata.nRoomID);

	if (GetPrivateProfileInt(szSection,_T("UseRange"),0, g_szIniFile)==0)
		return FALSE;

	//默认配置
	int nPlayerCount = lpRoomData->m_mapPlayer.GetCount();
	/*
	if (GetPrivateProfileInt(szSection,_T("Default"),0, g_szIniFile)==0)
	{
		if (nPlayerCount>=0 && nPlayerCount<40)
		{
			lpRandomConfig->nFixTable = 1;
		}
		else if (nPlayerCount>=40 && nPlayerCount<100)
		{
			lpRandomConfig->nMaxPreTables = 2;
			lpRandomConfig->nRandomDen = 4;
		}
		else if (nPlayerCount>=100 && nPlayerCount<200)
		{
			lpRandomConfig->nMaxPreTables = 3;
			lpRandomConfig->nRandomDen = 4;
		}
		else
		{
			lpRandomConfig->nMaxPreTables = 4;
			lpRandomConfig->nRandomDen = 4;		
		}
		return TRUE; 
	}
	*/

	int anRange[3];
	TCHAR szValue[64];	
	memset(szValue, 0, sizeof(szValue));
	GetPrivateProfileString(szSection,_T("range"),_T(""),szValue, sizeof(szValue),g_szIniFile);
	if (lstrlen(szValue)==0)
	{
		anRange[0] = 40;
		anRange[1] = 100;
		anRange[2] = 200;
	}
	else
	{
		TCHAR *p1 = szValue, *p2 = NULL;
		TCHAR *fields[8];
		memset(fields, 0, sizeof(fields));
		int nCount = xyRetrieveFields( p1, fields, 8, &p2 ) ;
		if (nCount<3)
			return FALSE;
		
		//人数区间范围配置为3个点 "range=40|100|200"
		
		for (int i=0; i<3; i++)
		{
			if (fields[i])
				anRange[i] = atoi(fields[i]);
			else
				return FALSE;
		}
	}

	
	//判断当前人数在哪个区间段 （4个区间段）
	int nIndex = 0;
	if (nPlayerCount>=0 && nPlayerCount<anRange[0])
		nIndex = 0;
	else if (nPlayerCount>=anRange[0] && nPlayerCount<anRange[1])
		nIndex = 1;
	else if (nPlayerCount>=anRange[1] && nPlayerCount<anRange[2])
		nIndex = 2;
	else
		nIndex = 3;

	TCHAR szKey[16];
	int anFixTable[4] = {1,0,0,0};
	sprintf(szKey, _T("fixtable%d"), nIndex);
	lpRandomConfig->nFixTable = GetPrivateProfileInt(szSection, szKey, anFixTable[nIndex], g_szIniFile);
	if (1==lpRandomConfig->nFixTable)
		return TRUE;

	int anDefaultPreTables[4] = {4,2,3,4};
	sprintf(szKey, _T("PreTables%d"), nIndex);
	lpRandomConfig->nMaxPreTables = GetPrivateProfileInt(szSection, szKey, anDefaultPreTables[nIndex], g_szIniFile);
	sprintf(szKey, _T("MinRandomPlayer%d"), nIndex);
	lpRandomConfig->nMinRandomPlayer = GetPrivateProfileInt(szSection, szKey, 1, g_szIniFile);
	sprintf(szKey, _T("RandomDen%d"), nIndex);
	lpRandomConfig->nRandomDen = GetPrivateProfileInt(szSection, szKey, 3, g_szIniFile);

	return TRUE;
}
HWND FindHoldSvrWindow()
{
	TCHAR szClsName[64];
	TCHAR szWndName[64];
	sprintf(szClsName,_T("%s%d"),HOLDSVR_CLSNAME, PORT_HOLD_SERVER);
	sprintf(szWndName,_T("%s%d"),HOLDSVR_WNDNAME, PORT_HOLD_SERVER);
	
	return  FindWindow(szClsName,szWndName);
}

CString GetYxpdsvrIniFile()
{
	CString sRet;
	TCHAR szFile[MAX_PATH];
	memset(szFile,0,sizeof(szFile));
	GetSystemDirectory(szFile,MAX_PATH);
	lstrcat(szFile,_T("\\yxpdsvr.ini"));
	sRet.Format(_T("%s"),szFile);
	return sRet;
}

int GetIsolateSecond(int nRoomID)
{
	TCHAR szKey[32];
	sprintf(szKey, _T("Isolate_Room%d"), nRoomID);
	
	int nIsolate = GetPrivateProfileInt(_T("VipKickoff"), szKey, 600, g_szIniFile);
	
	return nIsolate;
}

DWORD ComboRoomTableChair(int nRoomID,int nTableNO,int nChairNO)
{
	/************************************************************************/
	/*                                                                      */
	/*			roomid			  tableno	chairno							*/
	/*			(16位)            (8位)		(8位)							*/
	/*			(0-65535)		  (0-255)	(0-255)							*/
	/*			----------------  --------  --------						*/
	/*			XXXXXXXXXXXXXXXX  XXXXXXXX  XXXXXXXX						*/
	/************************************************************************/
	/*
	WORD wRoom = nRoomID;
	WORD wTableChair = MAKEWORD((BYTE)nChairNO, (BYTE)nTableNO);
	DWORD dwRoomTableChair = MAKEWPARAM(wTableChair, wRoom);

	return dwRoomTableChair;
	*/

	//2012.11.7  TableNo站位从256扩大到1024
	/************************************************************************/
	/*                                                                      */
	/*			roomid			  tableno	  chairno						*/
	/*			(16位)            (10位)	  (6位)							*/
	/*			(0-65535)		  (0-1023)	  (0-63)						*/
	/*			----------------  ----------  ------						*/
	/*			XXXXXXXXXXXXXXXX  ABCDEFGHIJ  XXXXXX						*/
	/*						实际tableno为 IJABCDEFGH，IJ为高位				*/
	/************************************************************************/
	/*
	DWORD	dwRoomTableChair = 0;
	WORD	wRoom=nRoomID;
	WORD	wTableNo = nTableNO;
	WORD	wTableChair=MAKEWORD(((HIBYTE(wTableNo))<<6)|((BYTE)nChairNO), LOBYTE((WORD)nTableNO));
			dwRoomTableChair=MAKEWPARAM(wTableChair,wRoom);

	return dwRoomTableChair;
	*/
	
	//2014.3.18  TableNo站位从1024扩大到4096
	/************************************************************************/
	/*                                                                      */
	/*			roomid			  tableno	    chairno						*/
	/*			(16位)            (12位)	    (4位)						*/
	/*			(0-65535)		  (0-4095)	    (0-15)						*/
	/*			----------------  ------------  ----						*/
	/*			XXXXXXXXXXXXXXXX  ABCDEFGHIJKL  XXXX						*/
	/*			实际tableno为 KLIJABCDEFGH，KLIJ为高位 						*/
	/*(注意：为了兼容1024桌子的解析方式，IJKL情况必须是，KL为高位，IJ为低位)*/
	/************************************************************************/
	DWORD	dwRoomTableChair = 0;
	WORD	wRoom=nRoomID;
	WORD	wTableNo = nTableNO;
	WORD	wTableChair=MAKEWORD(
						(((HIBYTE(wTableNo))<<6)|(((HIBYTE(wTableNo))>>2)<<4))|((BYTE)nChairNO),
						LOBYTE((WORD)nTableNO)
						);
	dwRoomTableChair=MAKEWPARAM(wTableChair,wRoom);
	
	return dwRoomTableChair;
}

BOOL ClearRoomChat(CBaseRoomData* lpRoomData)
{
	if (lpRoomData)
	{
		UwlLogFile(_T("===========Clear User ROOM_CHAT,system,mapCount=%d "),lpRoomData->m_mapRoomChat.GetCount());
		lpRoomData->m_mapRoomChat.RemoveAll();
	}
	
	return TRUE;
}

BOOL OnUserAddDeposit(DWORD nRoomDeposit,int nUserID)
{
	int nRoomID = HIWORD(nRoomDeposit);
	int nDeposit  = LOWORD(nRoomDeposit);
	
	if (nRoomID<=0 || nRoomID>65535
		|| nDeposit<0 || nDeposit>65535)
	{
		return FALSE;
	}
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
		
		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData))
			return FALSE;
	}
	
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		//		if(!lpRoomData->IsOldMatchRoom())	//所有房间均可使用
		{
			PLAYER_DEPOSIT pd;
			ZeroMemory(&pd, sizeof(pd));
			
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(nUserID, lpPlayer) && lpPlayer)
			{
				lpPlayer->nDeposit += nDeposit;
				pd.nUserID = nUserID;
				pd.nDeposit = lpPlayer->nDeposit;
				if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
				{
					NotifyOnePlayer(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
					NotifyDarkLooker(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
				}
				else
					NotifyRoomPlayers(lpRoomData, GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
			}
		}
	}
	return TRUE;
}

BOOL OnUserMinusDeposit(DWORD nRoomDeposit,int nUserID)
{
	int nRoomID = HIWORD(nRoomDeposit);
	int nDeposit  = LOWORD(nRoomDeposit);
	
	if (nRoomID<=0 || nRoomID>65535
		|| nDeposit<0 || nDeposit>65535)
	{
		return FALSE;
	}
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
		
		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData))
			return FALSE;
	}
	
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		//		if(!lpRoomData->IsOldMatchRoom())	//所有房间均可使用
		{
			PLAYER_DEPOSIT pd;
			ZeroMemory(&pd, sizeof(pd));
			
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(nUserID, lpPlayer) && lpPlayer)
			{
				if (lpPlayer->nDeposit<nDeposit)
					return TRUE;
				
				lpPlayer->nDeposit -= nDeposit;
				pd.nUserID = nUserID;
				pd.nDeposit = lpPlayer->nDeposit;
				if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
				{
					NotifyOnePlayer(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
					NotifyDarkLooker(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
				}
				else
					NotifyRoomPlayers(lpRoomData, GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
			}
		}
	}
	return TRUE;
}

// 监视玩家收集日志
void OverSeeUserData(int nUserId, LPCTSTR  callFunName)
{
	if (g_bOpenOverSeeUser)
	{
		if (nUserId>0 && nUserId == g_nOverSeeUserId)
		{
			UwlLogFile(_T("userId:%d, OverSeeUserData:call in fun:,%s"),nUserId,callFunName);
		}
	}
}
BOOL OnUserStatusPlaying(DWORD nRoomStatus,int nUserID)
{
	int nRoomID = HIWORD(nRoomStatus);
	int nPlayingStatus  = LOWORD(nRoomStatus);
	
	if (nRoomID<=0 || nRoomID>65535)
	{
		return FALSE;
	}
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
		
		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData))
			return FALSE;
	}
	
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		PLAYER_DEPOSIT pd;
		ZeroMemory(&pd, sizeof(pd));
		
		LPPLAYER lpPlayer = NULL;
		if(lpRoomData->m_mapPlayer.Lookup(nUserID, lpPlayer) && lpPlayer)
		{
			TABLE* pTable;
			lpRoomData->m_mapTable.Lookup(lpPlayer->nTableNO,pTable);
			if (!pTable)
			{
				return FALSE;
			}
			if (pTable->th.nStatus != TABLE_STATUS_PLAYING )
			{
				UwlLogFile("UserStatusPlaying:pTable->th.nStatus != TABLE_STATUS_PLAYING");
				return FALSE;
			}
// 			NTF_GET_STARTED  ngs;
// 			memset(&ngs, 0, sizeof(NTF_GET_STARTED));
// 			ngs.pp.nUserID=lpPlayer->nUserID;
// 			ngs.pp.nTableNO=lpPlayer->nTableNO;
// 			ngs.pp.nChairNO=lpPlayer->nChairNO;
// 			ngs.pp.nNetDelay=0;
// 			
// 			lpPlayer->pe.nStartupTime=UwlGetCurrentSeconds();
// 			lpPlayer->nStatus = PLAYER_STATUS_PLAYING;
// 			lpPlayer->pe.bRealPlaying=TRUE;
// 			NotifyRoomPlayers(lpRoomData, GR_PLAYER_PLAYING, &ngs, sizeof(ngs));
// 			UwlLogFile("UserStatusPlaying:UID=%d nTableNo=%d nChairNo=%d",lpPlayer->nUserID,lpPlayer->nTableNO,lpPlayer->nChairNO);
// 		
			NTF_GAMESTARTUP  ng;
			memset(&ng, 0, sizeof(ng));
			ng.nRoomID =nRoomID;
			ng.nTableNO=lpPlayer->nTableNO;
			ng.dwTableChairStatus = 0/* (lpRoomData->IsVariableChair() ? dwTableChairStatus:0 )*/;//非可变椅子房间,dwTableChairStatus必须=0
			memcpy(ng.nPlayerAry, pTable->nPlayerAry, sizeof(ng.nPlayerAry));
			
			if(lpRoomData->IsCloakingRoom())
			{
				NotifyDarkLooker(lpRoomData,0, GR_PLAYER_GAMESTARTUP, &ng, sizeof(ng));
			}
			else
			{
				NotifyRoomPlayers(lpRoomData, GR_PLAYER_GAMESTARTUP, &ng, sizeof(ng));
			}	
		}
	}
	return TRUE;
}
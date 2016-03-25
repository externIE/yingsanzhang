#include "roombasic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEF_GAMESVR_DEADTIME  5 //分钟
BOOL CBaseSockServer::OnAddFriend(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
  	LPADD_FRIEND lpAddFriend = LPADD_FRIEND(PBYTE(lpRequest->pDataPtr));
 	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpAddFriend->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		///////////////////////////////////////////////czk add
		if(IS_BIT_SET(lpRoomData->roomdata.dwManages , ROOM_MNG_FORBIDPRIVCHAT))
			return TRUE; 
		if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom() )
			return TRUE;
		LPPLAYER pSender = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpAddFriend->nUserID, pSender)) 
			return TRUE;
 		if(lstrcmp(pSender->pe.szHardID,lpAddFriend->szHardID))
			return TRUE;
		if(pSender->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
        pSender->pe.dwLatestTime = GetTickCount();
		int nRemains=0;
		if(IsForbidChatMachine(pSender->pe.szHardID,nRemains))
			return TRUE;
 		if(IsForbidChatUserID(lpAddFriend->nUserID,nRemains))
			return TRUE;
		////////////////////////////////////////////////////////////////////////////////
		LPPLAYER pFriend = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpAddFriend->nFriendID, pFriend)) 
			return TRUE;
 
		if(IS_BIT_SET(lpRoomData->roomdata.dwManages , ROOM_MNG_FORBIDDISTURB))
		{
			if(pFriend->nStatus==PLAYER_STATUS_LOOKON || pFriend->nStatus==PLAYER_STATUS_PLAYING )
				return TRUE; 
		}
 	    if(!SaveAddFriendAction(lpAddFriend->nUserID,lpAddFriend->nFriendID))
			return FALSE;

		memset(lpAddFriend->szHardID,0,sizeof(lpAddFriend->szHardID));
 		NotifyOnePlayer(lpRoomData, lpAddFriend->nFriendID,
						GR_ADD_FRIEND, lpAddFriend, lpRequest->nDataLen);

		
		CIDMap* pIDMap=NULL;
		lpRoomData->m_mapUnwelcomes.Lookup( lpAddFriend->nUserID,pIDMap);
		if(pIDMap )
 			pIDMap->RemoveKey(lpAddFriend->nFriendID);
 
		return TRUE;
	}
}

BOOL CBaseSockServer::OnAgreeFriend(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{

	LPAGREE_FRIEND lpAgreeFriend = LPAGREE_FRIEND(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpAgreeFriend->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 		if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom() )
			return TRUE;

		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpAgreeFriend->nUserID, pPlayer)) 
			return TRUE;
 		if(lstrcmp(pPlayer->pe.szHardID,lpAgreeFriend->szHardID))
			return TRUE;
		if(pPlayer->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		pPlayer->pe.dwLatestTime = GetTickCount();

		LPPLAYER pFriend = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpAgreeFriend->nFriendID, pFriend))
			return TRUE;
		memset(lpAgreeFriend->szHardID,0,sizeof(lpAgreeFriend->szHardID));
 		NotifyOnePlayer(lpRoomData, lpAgreeFriend->nFriendID,
						GR_AGREE_FRIEND, lpAgreeFriend, lpRequest->nDataLen);
		return TRUE;
	}
}

BOOL CBaseSockServer::OnDisagreeFriend(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{

	LPDISAGREE_FRIEND lpDisagreeFriend = LPDISAGREE_FRIEND(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpDisagreeFriend->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpDisagreeFriend->nUserID, pPlayer)) 
			return TRUE;
 		if(lstrcmp(pPlayer->pe.szHardID,lpDisagreeFriend->szHardID))
			return TRUE;

		LPPLAYER pFriend = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpDisagreeFriend->nFriendID, pFriend))
			return TRUE;
		memset(lpDisagreeFriend->szHardID,0,sizeof(lpDisagreeFriend->szHardID));
 		NotifyOnePlayer(lpRoomData, lpDisagreeFriend->nFriendID,
						GR_DISAGREE_FRIEND, lpDisagreeFriend, lpRequest->nDataLen);
		return TRUE;
	}
}

BOOL CBaseSockServer::OnCommitOneUnwelcome(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPCOMMIT_ONE_UNWELCOME  lpcon = LPCOMMIT_ONE_UNWELCOME(PBYTE(lpRequest->pDataPtr));
 	int nUserID=lpcon->nUserID;

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpcon->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
 	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup( nUserID, pPlayer)) 
			return TRUE;
 		if(lstrcmp(pPlayer->pe.szHardID,lpcon->szHardID))
			return TRUE;
 		if(pPlayer->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		if(pPlayer->nTimeCost / 60 <lpRoomData->GetUnwelcomeExperience())
			return TRUE;

		CIDMap* pIDMap=NULL;
		lpRoomData->m_mapUnwelcomes.Lookup( nUserID,pIDMap);
		if(pIDMap==NULL)
		{
 			pIDMap=new CIDMap;
			lpRoomData->m_mapUnwelcomes.SetAt(nUserID,pIDMap);
		}
 		if(lpcon->bUnwelcome)
 			pIDMap->SetAt(lpcon->nPlayer,lpcon->nPlayer);
		else
			pIDMap->RemoveKey(lpcon->nPlayer);
		 
 		return TRUE;
	}
}
BOOL CBaseSockServer::OnCommitUnwelcomes(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	LPCOMMIT_UNWELCOMES lpCommitUnwelcomes = LPCOMMIT_UNWELCOMES(PBYTE(lpRequest->pDataPtr));
	int nUserID=lpCommitUnwelcomes->nUserID;

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpCommitUnwelcomes->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup( nUserID, pPlayer)) 
			return TRUE;
 		if(lstrcmp(pPlayer->pe.szHardID,lpCommitUnwelcomes->szHardID))
			return TRUE;
 		if(pPlayer->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		if(pPlayer->nTimeCost / 60 <lpRoomData->GetUnwelcomeExperience())
			return TRUE;

		CIDMap* pIDMap=NULL;
		lpRoomData->m_mapUnwelcomes.Lookup( nUserID,pIDMap);
		if(pIDMap==NULL)
		{
 			pIDMap=new CIDMap;
			lpRoomData->m_mapUnwelcomes.SetAt(nUserID,pIDMap);
		}
		pIDMap->RemoveAll();
		for(int i=0;i<lpCommitUnwelcomes->nPlayerCount;i++)
		{
			int nPlayerID=lpCommitUnwelcomes->nPlayerID[i];
			pIDMap->SetAt(nPlayerID,nPlayerID);
		}
 		return TRUE;
	}

}



BOOL CBaseSockServer::IsForbidIP4(LPTSTR lpszIP,CString& sCause)
{  
	return IsSomeValueInFile(_T("ForbidIP4"),lpszIP,sCause,g_szIPManFile);

}
BOOL CBaseSockServer::IsForbidIP3(LPTSTR lpszIP,CString& sCause)
{
 	CString  sIP3 ;
 	sIP3.Format(_T("%s"),lpszIP);
 	int nFind=sIP3.ReverseFind('.');
	if(nFind!=-1)
		sIP3=sIP3.Left(nFind);

	return IsSomeValueInFile(_T("ForbidIP3"),sIP3,sCause,g_szIPManFile);
 
 
}
BOOL CBaseSockServer::IsForbidIP2(LPTSTR lpszIP,CString& sCause)
{
 	CString sIP2;
	sIP2.Format(_T("%s"),lpszIP);
 	int nFind=sIP2.ReverseFind('.');
	if(nFind!=-1)
	{
		sIP2=sIP2.Left(nFind);
		int nFind=sIP2.ReverseFind('.');
		if(nFind!=-1)
			sIP2=sIP2.Left(nFind);
	}
	return IsSomeValueInFile(_T("ForbidIP2"),sIP2,sCause,g_szIPManFile);

}
BOOL CBaseSockServer::IsInsideIP(DWORD dwIPAddr)
{
 	TCHAR szIP[MAX_SERVERIP_LEN];
 	ZeroMemory(szIP,sizeof(szIP));
 	UwlAddrToName(dwIPAddr,szIP);

	TCHAR szKey[MAX_SERVERIP_LEN];
 	ZeroMemory(szKey,sizeof(szKey));

 	TCHAR *fields[8];
	ZeroMemory(fields, sizeof(fields));
 	TCHAR  *p ;
    xyRetrieveFields2 (szIP, fields, 8, &p,'.' ) ;
    sprintf(szKey,_T("%s.%s.0.0"),fields[0],fields[1]);

	CString sValue;
	return IsSomeValueInFile(_T("InsideIP"),szKey,sValue,g_szIPManFile);
}
BOOL CBaseSockServer::OnEnterRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{   
	CBaseRoomData* lpRoomData = NULL;
	LPPLAYER pPlayer = NULL;
	BOOL  bForbidProxyIP=FALSE;
	BOOL  bForbidVPNPort=FALSE;
	BOOL  bForbidVirtual=FALSE;
	BOOL  bForbidVPN10=FALSE;
	
 	BOOL  bCheckTrustIP=FALSE;
	CString sGameIniFile,sCause;

	REQUEST response;
	memset(&response, 0, sizeof(response));
 
	TCHAR szError[512];
	memset(szError,0,sizeof(szError));
 	
	int nClientPort=0;
 	int nServerPort=0;
   	LPENTER_ROOM lpEnterRoom = LPENTER_ROOM(PBYTE(lpRequest->pDataPtr));


	if(lpRequest->nDataLen != sizeof(ENTER_ROOM))
	{
		response.head.nRequest = UR_INVALID_PARAM;
		goto SendResponse;
	}

    if(lstrlen(lpEnterRoom->szMachineID)==0 || lstrlen(lpEnterRoom->szHardID)==0 )
	{
		response.head.nRequest = UR_INVALID_PARAM;
		goto SendResponse;
	}

	lpEnterRoom->dwEnterFlags &=~FLAG_ENTERROOM_HANDPHONE;
 	lpEnterRoom->nEnterTime=UwlGetCurrentSeconds();
	lpEnterRoom->nRoomSvrID=g_nClientID;
	lpEnterRoom->nHostID=g_nHostID;
    lpEnterRoom->dwIPAddr = GetClientAddress(lpContext->hSocket, lpContext->lTokenID);	
		
 	UwlGetPeerPort(lpContext->hSocket,nClientPort);
	lpEnterRoom->dwClientPort=MAKELONG(lpEnterRoom->dwClientPort,nClientPort );

 	UwlGetSockPort(lpContext->hSocket,nServerPort);
	lpEnterRoom->dwServerPort=MAKELONG(lpEnterRoom->dwServerPort,nServerPort );

	TCHAR szIP[MAX_SERVERIP_LEN];
 	ZeroMemory(szIP,sizeof(szIP));
  	UwlAddrToName(lpEnterRoom->dwIPAddr,szIP);

 	/////////////////////////////////////////
  	if(IsForbidIP4(szIP,sCause))
	{
		sprintf(szError,_T("您的IP(%s)被禁止，原因：\r\n\r\n%s\r\n\r\n详情请与管理员联系。"),szIP,sCause);
		response.head.nRequest = GR_ROOM_FORBID_IP;
		goto SendResponse;
	}
 
 	if(IsForbidIP3(szIP,sCause))
	{
		sprintf(szError,_T("您的IP(%s)前3位被禁止，原因：\r\n\r\n%s\r\n\r\n详情请与管理员联系。"),szIP,sCause);
 		response.head.nRequest = GR_ROOM_FORBID_IP;
		goto SendResponse;
	}
	if(IsForbidIP2(szIP,sCause))
	{
		sprintf(szError,_T("您的IP(%s)前2位被禁止，原因：\r\n\r\n%s\r\n\r\n详情请与管理员联系。"),szIP,sCause);
 		response.head.nRequest = GR_ROOM_FORBID_IP;
		goto SendResponse;
	}
	
	{
		int nLatestNobuffer=InterlockedExchangeAdd(&g_lNoBufferLatest, 0);
		int nBusyWaitSecond=GetBusyWaitSecond() ;
		int nInterval=UwlGetCurrentSeconds()-nLatestNobuffer;
		if(nInterval>=0 && nInterval<= nBusyWaitSecond)//2分钟之内
		{
			response.head.nRequest = GR_SERVICE_BUSY;
			goto SendResponse;
		}
	}
	/////////////////////////////////////////////////////////////////
	{
		CAutoLock lock(&g_csRoomData);
 		g_mapRoomDataEx.Lookup(lpEnterRoom->nRoomID, lpRoomData);
	}
	if(lpRoomData)
	{
 		CAutoLock lock(&(lpRoomData->m_csRoomData));

  		if(lpRoomData->m_mapPlayer.Lookup(lpEnterRoom->nUserID, pPlayer))
		{
			if(pPlayer->nStatus==PLAYER_STATUS_PLAYING)
				lpEnterRoom->dwEnterFlags|=FLAG_ENTERROOM_DXXW;
		}
		bForbidProxyIP=IS_BIT_SET(lpRoomData->roomdata.dwOptions,ROOM_OPT_FORBIDPROXYIP);
		bForbidVPNPort=IS_BIT_SET(lpRoomData->roomdata.dwOptions,ROOM_OPT_FORBIDVPNPORT);
    	bForbidVirtual=IS_BIT_SET(lpRoomData->roomdata.dwOptions,ROOM_OPT_FORBIDVIRTUAL);
 		bForbidVPN10=  IS_BIT_SET(lpRoomData->roomdata.dwOptions,ROOM_OPT_FORBIDVPN10);

		bCheckTrustIP=(lpRoomData->IsOldMatchRoom() && !IS_BIT_SET(lpEnterRoom->dwEnterFlags,FLAG_ENTERROOM_INTER));//排行赛房间需要检查比赛是否限定IP（网吧比赛）
		if(bCheckTrustIP)
			sGameIniFile=lpRoomData->GetGameSvrIniFile();
	}

 	if(bForbidProxyIP)
	{	
		WORD w3=LOWORD(lpEnterRoom->dwServerPort);
		WORD w4=HIWORD(lpEnterRoom->dwServerPort);

		BOOL b1=(w3!=w4);//socket代理
 		BOOL b2=IS_BIT_SET(lpEnterRoom->dwClientFlags,FLAG_CLIENTINFO_FINDVPN);
 		BOOL b3=IS_BIT_SET(lpEnterRoom->dwClientFlags,FLAG_CLIENTINFO_CLTISRMT);
 		BOOL b4=IS_BIT_SET(lpEnterRoom->dwClientFlags,FLAG_CLIENTINFO_LOOPER);
		
		if( b1||b2||(b3 && b4) )
		{
			response.head.nRequest = GR_FORBID_PROXYIP;
			goto SendResponse;
		}
 
	}
	if(bForbidVirtual)
	{
  		BOOL b1=IS_BIT_SET(lpEnterRoom->dwClientFlags,FLAG_CLIENTINFO_VMWARE);
 		BOOL b2=IS_BIT_SET(lpEnterRoom->dwClientFlags,FLAG_CLIENTINFO_VPC);
 		BOOL b3=IS_BIT_SET(lpEnterRoom->dwClientFlags,FLAG_CLIENTINFO_VBOX);
		
		if( b1||b2||b3)
		{
			response.head.nRequest = GR_FORBID_VIRTUAL;
			goto SendResponse;
		}

	}
	if(bForbidVPNPort)
	{
		BOOL b5=IsProxyIP(lpEnterRoom->dwIPAddr);
		if( b5 )
		{
			response.head.nRequest = GR_FORBID_PROXYIP;
			goto SendResponse;
		}
	}

	if(bForbidVPN10 )
	{
 		BOOL bClientIPDiff=(lpEnterRoom->dwIPAddr!=lpEnterRoom->dwClientSockIP);
 		BOOL bClientIPIsGateway=(lpEnterRoom->dwClientSockIP==lpEnterRoom->dwClientGateway);
 		BOOL bIsOutsideIP=xyIsOutsideIP(lpEnterRoom->dwClientSockIP);

		if(bIsOutsideIP)
			bIsOutsideIP=!IsInsideIP(lpEnterRoom->dwClientSockIP);//xyIsOutsideIP函数判断有遗漏，在这里继续靠配置判断

        BOOL b6=(bClientIPDiff && bClientIPIsGateway  && !bIsOutsideIP);//clientsockip是内网地址(10)
 
		if( b6 )
		{
			response.head.nRequest = GR_FORBID_PROXYIP;
			goto SendResponse;
		}
	}

	if(bCheckTrustIP)
	{
		if(IsSomeValueInFile(_T("IPLimited"),xyConvertIntToStr(lpEnterRoom->nRoomID),sCause,sGameIniFile))
		{	
			CString sWhere;
			if(!IsSomeValueInFile(_T("TrustIP"),szIP,sWhere,sGameIniFile))
			{
				lstrcpyn(szError,sCause,256);
 				response.head.nRequest = GR_ERROR_INFOMATION;
				goto SendResponse;
			}
		}
	}


	if(bForbidVPNPort)
	{
		BOOL bClientIPDiff=(lpEnterRoom->dwIPAddr!=lpEnterRoom->dwClientSockIP);
		if(bClientIPDiff)
		{
			HWND hWnd=FindLoftSvrWindow();
			if(IsWindow(hWnd))
		 		PostMessage(hWnd,WM_RTG_SCANVPN,lpEnterRoom->dwIPAddr, (LPARAM)g_hMessageWnd);
 
		}
	}
	////////////////////////////////////////////////////////////////
	TransmitRequest(lpContext, lpRequest);
 	return TRUE;

SendResponse:
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	if(GR_FORBID_PROXYIP == response.head.nRequest)
	{
	 	response.pDataPtr=&lpEnterRoom->dwIPAddr;
		response.nDataLen=sizeof(int);
	}
	else if(GR_ERROR_INFOMATION==response.head.nRequest ||
		     GR_ROOM_FORBID_IP ==response.head.nRequest )
	{
	 	response.pDataPtr=szError;
	 	response.nDataLen=lstrlen(szError)+1;
	}
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	if(UR_OPERATE_SUCCEEDED == response.head.nRequest)
	{
	}
	else
	{
	}
 	return TRUE;
}

 

BOOL CBaseSockServer::OnLeaveRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{ 
 	LPLEAVE_ROOM lpLeaveRoom = LPLEAVE_ROOM(PBYTE(lpRequest->pDataPtr));
	LONG token =  lpContext->lTokenID;

	RemoveTokenRoomSock(token);   

 	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpLeaveRoom->nRoomID, lpRoomData)){
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));

		USER_SOCK us;
		ZeroMemory(&us,sizeof(USER_SOCK));

		LPPLAYER pPlayer = NULL;
		lpRoomData->m_mapTokenUserSock.Lookup(token, us);

  		if( lpLeaveRoom->nUserID == us.nUserID  )
		{		
			lpRoomData->m_mapUserSock.RemoveKey(us.nUserID);
  			lpRoomData->m_mapTokenUserSock.RemoveKey(us.token);

			if(lpRoomData->m_mapPlayer.Lookup(us.nUserID, pPlayer) && pPlayer)
			{
 				if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
				{	
					UwlLogFile(_T("===========LeaveRoom tokenid failed.userid=%d"), lpLeaveRoom->nUserID);
 					return TRUE;
				}
 				
				if(lstrcmp(pPlayer->pe.szHardID,lpLeaveRoom->szHardID))//硬件匹配
				{
					UwlLogFile(_T("===========LeaveRoom hardid failed.userid=%d,hardid=%s"), lpLeaveRoom->nUserID,lpLeaveRoom->szHardID);
 					return TRUE;
				}
	        	if(pPlayer->nStatus == PLAYER_STATUS_PLAYING ) //游戏开始之后,不允许离开房间
				{
   					int nInterval=UwlGetCurrentSeconds()-pPlayer->pe.nStartupTime;
 					UwlLogFile(_T("===========LeaveRoom failed.GameID=%d,RoomID=%d,UserID=%d,interval seconds=%d"),lpLeaveRoom->nGameID,lpLeaveRoom->nRoomID,lpLeaveRoom->nUserID,nInterval);
					return TRUE;
				}
 			 
				RemoveOnePlayer(lpRoomData,pPlayer,lpContext->hSocket, lpContext->lTokenID);
			

				PLAYER_OUTROOM po;
				ZeroMemory(&po,sizeof(po));
				po.nUserID=lpLeaveRoom->nUserID;
 				po.nAreaID=lpLeaveRoom->nAreaID; 
				po.nGameID=lpLeaveRoom->nGameID; 
				po.nRoomID=lpLeaveRoom->nRoomID; 
				po.nRoomSvrID=g_nClientID;
				po.nHostID=g_nHostID;
				po.nGamePort=lpRoomData->roomdata.nGamePort;

				NotifyServer(GR_PLAYER_OUTROOM,&po,sizeof(PLAYER_OUTROOM));
			
				//////通知gamesvr
			  //lpRoomData->PostLeaveRoom(lpLeaveRoom->nUserID); czk于2011-01-11取消这个通知

			}

		}
	}
  	return TRUE;
 
}




BOOL CBaseSockServer::OnSetRandomTable(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPSET_RANDOM_TABLE lpSetRandomTable = LPSET_RANDOM_TABLE(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpSetRandomTable->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));
    	if(!lpRoomData->IsRandomRoom())
 			return TRUE;

		LPPLAYER pAdmin= NULL;
		LPPLAYER pPlayer = NULL;

 		if(!lpRoomData->m_mapPlayer.Lookup(lpSetRandomTable->nAdminID, pAdmin)) 
			return TRUE;
		if(!IS_BIT_SET(pAdmin->nUserType ,USER_TYPE_ADMIN))//不是网管
			return TRUE;
 		if(lstrcmp(pAdmin->pe.szHardID,lpSetRandomTable->szHardID))
			return TRUE;
 		if(pAdmin->pe.lTokenID!= lpContext->lTokenID)
			return TRUE;
		if(!lpRoomData->m_mapPlayer.Lookup(lpSetRandomTable->nPlayer, pPlayer)) 
			return TRUE;
	 	if( pPlayer->nStatus != PLAYER_STATUS_WAITING  ) 	// 这个用户还没开始
	 		return TRUE;

		SetRandomTableLeave(lpRoomData, pPlayer);
    
		TABLE* pTable=NULL;
		if(!lpRoomData->m_mapTable.Lookup(lpSetRandomTable->nTableNO,pTable))
		{
			pTable = new TABLE;
			ZeroMemory(pTable, sizeof(TABLE));
			pTable->th.nTableNO = lpSetRandomTable->nTableNO;
			lpRoomData->m_mapTable.SetAt(pTable->th.nTableNO, pTable);
		}

		PLAYER_POSITION PP;
		ZeroMemory(&PP,sizeof(PP));
		PP.nUserID=lpSetRandomTable->nPlayer;
		PP.nTableNO=lpSetRandomTable->nTableNO;
 		PP.nChairNO=lpRoomData->GetFreeChairNO(pTable);
 		lpRoomData->SetRandomPosition(pPlayer,pTable,&PP);
   	
		NotifyDarkLooker(lpRoomData,0, GR_RANDOM_POSITION, &PP, sizeof(PP));
  		if(lpRoomData->IsFullTable(pTable)) // 坐满了
		{
			TABLE* pNewTable=NULL;
			lpRoomData->SendRandomPlaying(pTable,&PP,pNewTable);
			OverSeeUserData(pPlayer->nUserID,"OnSetRandomTable::CommitTPS");
			lpRoomData->CommitTPS(pNewTable,PS_PLAYING);
			if( lpRoomData->IsSoloRoom())
  				lpRoomData->PostStartSoloTable(pNewTable);
		}
	}
	return TRUE;


}


BOOL CBaseSockServer::OnGetFinished(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	GET_FINISHED_OK gfo;
	memset(&gfo, 0, sizeof(gfo));

	LPGET_FINISHED lpGetFinished = LPGET_FINISHED(PBYTE(lpRequest->pDataPtr));


	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpGetFinished->nRoomID, lpRoomData)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
	 	if(lpRoomData->IsDarkRoom())
		{
			response.head.nRequest =GR_NO_PERMISSION;	 
			goto SendResponse;
		}
		if(lpGetFinished->nChairNO<0 || lpGetFinished->nChairNO>=MAX_CHAIR_COUNT)
		{
			response.head.nRequest = UR_INVALID_PARAM;
			goto SendResponse;
		}
 		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpGetFinished->nUserID, pPlayer)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}

		if(lstrcmp(pPlayer->pe.szHardID,lpGetFinished->szHardID))
		{
			response.head.nRequest = GR_HARDID_MISMATCH;
			goto SendResponse;
		}
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}
		pPlayer->pe.dwLatestTime = GetTickCount();

		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(lpGetFinished->nTableNO, pTable)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有就座
			goto SendResponse;
		}
		if(lpGetFinished->nUserID != pTable->nPlayerAry[lpGetFinished->nChairNO]){	
			response.head.nRequest = UR_OPERATE_FAILED;		// 这个位置不是这个人
			goto SendResponse;
		}
		if(pPlayer->nStatus != PLAYER_STATUS_PLAYING){	// 这个用户没有在玩(埋头)
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		if(pTable->th.nStatus != TABLE_STATUS_PLAYING){	// 游戏没有进行中
			response.head.nRequest = UR_OPERATE_FAILED;	
			goto SendResponse;
		}
		////////////////////////////////////////////
		if(IS_BIT_SET(lpRoomData->roomdata.dwConfigs,ROOM_CFG_VERIFYGETFINISHED)
			&& !IS_BIT_SET(lpContext->dwFlags, CH_FLAG_SYSTEM_EJECT))
		{
			HWND hWnd=lpRoomData->FindGameSvrWindow();
			if(IsWindow(hWnd))//当前游戏服务端正常
			{
				if(hWnd==pTable->hGameSvrWnd) //gamesvr没重启过
				{
					if(pPlayer->pe.bRealPlaying)//玩家实际在玩
					{
						CString sIniFile=lpRoomData->GetGameSvrIniFile();
						int nInterval=GetPrivateProfileInt(_T("Timer"),_T("Interval"),DEF_GAMESVR_DEADTIME, sIniFile);//配置里是分钟
						if(!IsDeadGameSvr(lpRoomData->roomdata.nGamePort,nInterval*60 + 30 ))//gamesvr心跳正常(额外加30秒)
						{
							response.head.nRequest = GR_PLAYING_GAME;		
							goto SendResponse;
						}
					}
				}
			}
		}
		////////////////////////////////////////////////////////////////
		OverSeeUserData(pPlayer->nUserID,"OnGetFinished::CommitTPS");
        lpRoomData->CommitTPS(pTable,PS_WALKAROUND);

		///////////////////////////////////////////////czk end
		if(IS_BIT_SET(lpRoomData->roomdata.dwOptions , ROOM_OPT_SEATEDCONTINUE))
		{
			SetTableAfterFinished(lpRoomData, pPlayer);
		}
		else
		{
			SetTableAsEmptyOrLeave(lpRoomData, pPlayer);//整桌清空
		}
		gfo.nUserID = lpGetFinished->nUserID;
		gfo.nTableNO = lpGetFinished->nTableNO;
		gfo.nChairNO = lpGetFinished->nChairNO;
		memcpy(gfo.nPlayerAry, pTable->nPlayerAry, sizeof(gfo.nPlayerAry));

		FilterPlayAryByMinDeposit(lpRoomData, pTable, gfo.nPlayerAry);
		FilterPlayAryByMinScore(lpRoomData, pTable, gfo.nPlayerAry);

		SetTableAsCompareScoreDeposit(lpRoomData, pTable, gfo.nPlayerAry);
		
		gfo.nPlayerCount = pTable->th.nPlayerCount;

		NotifyRoomPlayers(lpRoomData, GR_PLAYER_FINISHED, &gfo, sizeof(gfo),
							lpContext->hSocket, lpContext->lTokenID);
		//////////////////////////////////////////////////////////////////////////////////////////
  	    lpRoomData->PostGetFinished(lpGetFinished->nUserID,lpGetFinished->nTableNO,lpGetFinished->nChairNO);
	}
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	response.pDataPtr = &gfo;
	response.nDataLen = sizeof(gfo);


SendResponse:
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	if(UR_OPERATE_SUCCEEDED == response.head.nRequest)
	{

	}
	else
	{
	
	}

	if(IS_BIT_SET(lpGetFinished->dwGetFlag,FLAG_GETFINISHED_KICKOFF))
	{
		LPKICKOFF_WALKAROUND   lpKickoff=new KICKOFF_WALKAROUND;
		ZeroMemory(lpKickoff,sizeof(KICKOFF_WALKAROUND));
		lpKickoff->nAreaID = lpGetFinished->nAreaID;
		lpKickoff->nGameID = lpGetFinished->nGameID;
		lpKickoff->nRoomID = lpGetFinished->nRoomID;
		lpKickoff->nPlayer = lpGetFinished->nUserID;
		PutToServer(lpContext, GR_KICKOFF_WALKAROUND, lpKickoff, sizeof(KICKOFF_WALKAROUND));
	}

//	UwlClearRequest(&response);
	return TRUE;
}

BOOL CBaseSockServer::OnGetLookon(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	LPGET_LOOKON lpGetLookon = LPGET_LOOKON(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpGetLookon->nRoomID, lpRoomData)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));

		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpGetLookon->nUserID, pPlayer)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}
		if(lpGetLookon->nChairNO<0 || lpGetLookon->nChairNO>=MAX_CHAIR_COUNT)
		{
			response.head.nRequest = UR_INVALID_PARAM;
			goto SendResponse;
		}
		if(lstrcmp(pPlayer->pe.szHardID,lpGetLookon->szHardID))
		{
			response.head.nRequest = GR_HARDID_MISMATCH;
			goto SendResponse;
		}
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}
		if(lpRoomData->IsDarkRoom())
		{
		   if(!IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_LOOKDARK))
		   {
				response.head.nRequest = GR_NO_PERMISSION;
				goto SendResponse;
		   }
 
		}

		if(lpRoomData->IsCloakingRoom())
		{
			//Add on 20140107 旁观玩家无法进入游戏，hard和token验证不过
			lpRoomData->PostHardAndToken(pPlayer->nUserID,pPlayer->pe.szHardID,lpContext->lTokenID);
 
			if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ADMIN))	//网管才能旁观隐身房
			{
				response.head.nRequest = GR_NO_PERMISSION;
				goto SendResponse;
			}
		}

		pPlayer->pe.dwLatestTime = GetTickCount();

		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(lpGetLookon->nTableNO, pTable)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
		if(pTable->th.nStatus != TABLE_STATUS_PLAYING){ // 游戏没开始，不能旁观
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		if(pPlayer->nStatus != PLAYER_STATUS_WALKAROUND){	// 
			response.head.nRequest = GR_MUST_STANDUP;		
			goto SendResponse;
		}
		//////////////////////////////////////////////////////////////////////////////
 		LPPLAYER lpSeated = NULL;
		if(pTable->nPlayerAry[lpGetLookon->nChairNO]==0 || !lpRoomData->m_mapPlayer.Lookup(pTable->nPlayerAry[lpGetLookon->nChairNO], lpSeated))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	
 			goto SendResponse;
		}
		if(lpSeated->nStatus != PLAYER_STATUS_PLAYING){ // 没有在玩，不能旁观
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
	   ///////////////////////////////////////////////////////////////////////////////////////
		if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ADMIN))//不是网管
		{
			if(IS_BIT_SET(lpRoomData->roomdata.dwConfigs,ROOM_CFG_FORBIDLOOKON))
			{
 				response.head.nRequest = GR_FORBID_LOOKON;		
				goto SendResponse;
			}

			if(lstrcmp(pTable->szPassword, _T("")))
			{
				if(lstrcmp(pTable->szPassword, lpGetLookon->szPassword))
				{
					response.head.nRequest = UR_PASSWORD_WRONG;			// 旁观者需要输入密码
					goto SendResponse;
				}
			}
		}
		BOOL bNoPosition;
		bNoPosition = TRUE;
		for(int i = 0; i < MAX_VISITOR_COUNT; i++){
			if(0 == pTable->nVisitorAry[lpGetLookon->nChairNO][i]){
				pTable->nVisitorAry[lpGetLookon->nChairNO][i] = lpGetLookon->nUserID;
				pTable->th.nVisitorCount++;
				bNoPosition = FALSE;
				break;
			}
		}
		if(bNoPosition){
			response.head.nRequest = GR_NO_CHAIRS;		
			goto SendResponse;
		}
		///////////////////////////////////////////////////
		lpRoomData->PostExperience(pPlayer->nUserID,pPlayer->nTimeCost / 60 );
 
		pPlayer->nStatus = PLAYER_STATUS_LOOKON;
		pPlayer->nTableNO = lpGetLookon->nTableNO;
		pPlayer->nChairNO = lpGetLookon->nChairNO;
		
		PLAYER_POSITION pp;
		memset(&pp,0,sizeof(PLAYER_POSITION));
		pp.nUserID=lpGetLookon->nUserID;
		pp.nTableNO=lpGetLookon->nTableNO;
		pp.nChairNO=lpGetLookon->nChairNO;
		if(lpRoomData->IsDarkRoom())
		{
 			NotifyDarkLooker(lpRoomData,  pp.nUserID,GR_PLAYER_LOOKON, &pp, sizeof(PLAYER_POSITION));

		}
		else
		{
		 	if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ADMIN))//
 				NotifyRoomPlayers(lpRoomData, GR_PLAYER_LOOKON, &pp, sizeof(pp),lpContext->hSocket, lpContext->lTokenID);
		}
	}
	response.head.nRequest = UR_OPERATE_SUCCEEDED;

SendResponse:
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	if(UR_OPERATE_SUCCEEDED == response.head.nRequest){
	}else{
	
	}
	UwlClearRequest(&response);
	return TRUE;
}

BOOL CBaseSockServer::OnGetUnlookon(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	LPGET_UNLOOKON lpGetUnlookon = LPGET_UNLOOKON(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpGetUnlookon->nRoomID, lpRoomData)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 
 		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpGetUnlookon->nUserID, pPlayer)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}
 		if(lstrcmp(pPlayer->pe.szHardID,lpGetUnlookon->szHardID))
		{
			response.head.nRequest = GR_HARDID_MISMATCH;
			goto SendResponse;
		}
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}
		pPlayer->pe.dwLatestTime = GetTickCount();

		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(lpGetUnlookon->nTableNO, pTable)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
		if(pPlayer->nStatus != PLAYER_STATUS_LOOKON){	// 
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		SetTableAsLookonLeave(lpRoomData, pPlayer);
	
		PLAYER_POSITION pp;
		memset(&pp,0,sizeof(PLAYER_POSITION));
		pp.nUserID=lpGetUnlookon->nUserID;
		pp.nTableNO=lpGetUnlookon->nTableNO;
		pp.nChairNO=lpGetUnlookon->nChairNO;
		if(lpRoomData->IsDarkRoom())
		{
 			NotifyDarkLooker(lpRoomData,  pp.nUserID,GR_PLAYER_UNLOOKON, &pp, sizeof(PLAYER_POSITION));

		}
		else
		{
			NotifyRoomPlayers(lpRoomData, GR_PLAYER_UNLOOKON, &pp, sizeof(PLAYER_POSITION),	lpContext->hSocket, lpContext->lTokenID);
		}


	}
	response.head.nRequest = UR_OPERATE_SUCCEEDED;

SendResponse:
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	if(UR_OPERATE_SUCCEEDED == response.head.nRequest){
	}else{
	
	}
	UwlClearRequest(&response);
	return TRUE;
}


BOOL CBaseSockServer::OnQueryTablePwd(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 
	LPQUERY_TABLEPWD lpQueryTablePwd = LPQUERY_TABLEPWD(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpQueryTablePwd->nRoomID, lpRoomData)) 
			return TRUE;
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 		if(lpRoomData->IsDarkRoom())
			return TRUE;

  		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpQueryTablePwd->nUserID, pPlayer)) 
			return TRUE;
 		if(lstrcmp(pPlayer->pe.szHardID,lpQueryTablePwd->szHardID))
			return TRUE;
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
            return TRUE;
		pPlayer->pe.dwLatestTime = GetTickCount();

		if(!lpRoomData->m_mapPlayer.Lookup(lpQueryTablePwd->nPlayer, pPlayer)) 
			return TRUE;


		///////////////////////////////////////////////////////////////////////////////
		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(lpQueryTablePwd->nTableNO, pTable)) 
			return TRUE;
		if(pTable->th.nPlayerCount==0)
			return FALSE;
 		if(pTable->th.bHavePassword==FALSE)
			return FALSE;
		if(pTable->th.nStatus==TABLE_STATUS_PLAYING)
			return FALSE;
	
		//////////////////////////////////////////////////////////////////////////////
 	    if(!SaveTablePwdAction(lpQueryTablePwd->nUserID,lpQueryTablePwd->nPlayer))
			return FALSE;
   		////////////////////////////////////////
		memset(lpQueryTablePwd->szHardID,0,sizeof(lpQueryTablePwd->szHardID));
 		NotifyOnePlayer(lpRoomData, lpQueryTablePwd->nPlayer,
						GR_QUERY_TABLEPWD, lpQueryTablePwd, lpRequest->nDataLen);
		return TRUE;
	}
}

BOOL CBaseSockServer::OnAnswerTablePwd(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{

	LPANSWER_TABLEPWD lpAnswerTablePwd = LPANSWER_TABLEPWD(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpAnswerTablePwd->nRoomID, lpRoomData)) 
			return TRUE;
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 		if(lpRoomData->IsDarkRoom())
			return TRUE;

		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpAnswerTablePwd->nUserID, pPlayer)) 
			return TRUE;
 		if(lstrcmp(pPlayer->pe.szHardID,lpAnswerTablePwd->szHardID))
			return TRUE;
		if(pPlayer->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		pPlayer->pe.dwLatestTime = GetTickCount();

 		if(!lpRoomData->m_mapPlayer.Lookup(lpAnswerTablePwd->nPlayer, pPlayer)) 
			return TRUE;
  		////////////////////////////////////////
		memset(lpAnswerTablePwd->szHardID,0,sizeof(lpAnswerTablePwd->szHardID));
 		NotifyOnePlayer(lpRoomData, lpAnswerTablePwd->nPlayer,
						GR_ANSWER_TABLEPWD, lpAnswerTablePwd, lpRequest->nDataLen);
		return TRUE;
	}
}
BOOL CBaseSockServer::OnForbidChat(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	LPFORBID_CHAT lpForbidChat = LPFORBID_CHAT(PBYTE(lpRequest->pDataPtr));
 
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpForbidChat->nRoomID, lpRoomData)){
			return TRUE;
		}
	}

	CString sMachineID;
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		///////////////////////////////////////////////////////////////////
		LPPLAYER pAdmin = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpForbidChat->nAdminID, pAdmin)) 
			return TRUE;
 		if(lstrcmp(pAdmin->pe.szHardID,lpForbidChat->szAdminHardID))
			return TRUE;
		if(pAdmin->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		pAdmin->pe.dwLatestTime = GetTickCount();

		if(!IS_BIT_SET(pAdmin->nUserType ,USER_TYPE_ADMIN))//不是网管
			return TRUE;
		/////////////////////////////////////////////////////////////////////
 		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpForbidChat->nPlayerID, pPlayer)){
			return TRUE;
		}
 	    sMachineID.Format(_T("%s"),pPlayer->pe.szHardID);//这里用网卡号做key,硬盘号不够唯一
	}
	if(sMachineID.IsEmpty())
		return TRUE;
	CHAT_FORBID  cf;
	ZeroMemory(&cf,sizeof(CHAT_FORBID));
	cf.nRoomSvrID=g_nClientID;
	cf.nRoomID=lpForbidChat->nRoomID;
	lstrcpyn(cf.szHardID,sMachineID,MAX_HARDID_LEN);
	cf.nAdminID=lpForbidChat->nAdminID;
	cf.nPlayerID=lpForbidChat->nPlayerID;

	if(lpForbidChat->bForbid==FALSE) //删除
	{  	
		CAutoLock lock(&g_csChatForbid);

		cf.bForbid=FALSE;

		CHAT_FORBID  cfold;
		ZeroMemory(&cfold,sizeof(cfold));
		if(g_mapChatForbid.Lookup(sMachineID,cfold))
		{
			g_mapChatForbid.RemoveKey(sMachineID);
			g_mapChatForbidID.RemoveKey(cfold.nPlayerID);
		}
  
		g_mapChatForbidID.RemoveKey(cf.nPlayerID);
    	NotifyServer(GR_FORBID_CHAT,&cf,sizeof(CHAT_FORBID));
 		return FALSE;
	}
	{
  		CAutoLock lock(&g_csChatForbid);
		cf.nCreateTime=UwlGetCurrentSeconds();
		cf.nExpiration=cf.nCreateTime+lpForbidChat->nDuration*60;
 		cf.bForbid=TRUE;
  		g_mapChatForbid.SetAt(sMachineID,cf);
  		g_mapChatForbidID.SetAt(cf.nPlayerID,cf);
   		NotifyServer(GR_FORBID_CHAT,&cf,sizeof(CHAT_FORBID));
	}
	return TRUE;
}


BOOL CBaseSockServer::OnForbidRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	LPFORBID_ROOM lpForbidRoom = LPFORBID_ROOM(PBYTE(lpRequest->pDataPtr));
 	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpForbidRoom->nRoomID, lpRoomData)){
			return TRUE;
		}
	}

	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 		LPPLAYER pAdmin = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpForbidRoom->nAdminID, pAdmin)) 
			return TRUE;
 		if(lstrcmp(pAdmin->pe.szHardID,lpForbidRoom->szAdminHardID))
			return TRUE;
		if(pAdmin->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		pAdmin->pe.dwLatestTime = GetTickCount();
 		if(!IS_BIT_SET(pAdmin->nUserType ,USER_TYPE_ADMIN))//不是网管
			return TRUE;
 		if(!IS_BIT_SET(pAdmin->nUserType ,USER_TYPE_FORBIDROOM))//不是禁止房间网管
			return TRUE;
	}
 	CString sMachineID;
	if(lpForbidRoom->bForbid) //禁止
	{
 		CAutoLock lock(&(lpRoomData->m_csRoomData));
  		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpForbidRoom->nPlayerID, pPlayer))
 			return TRUE;
   
 		ROOM_FORBID  rf;
		ZeroMemory(&rf,sizeof(ROOM_FORBID));
 		lstrcpyn(rf.szHardID,pPlayer->pe.szHardID,MAX_HARDID_LEN);
		rf.nRoomSvrID=g_nClientID;
		rf.nRoomID=lpForbidRoom->nRoomID;
		rf.nAdminID=lpForbidRoom->nAdminID;
		rf.nRoomID=lpForbidRoom->nRoomID;
		rf.nPlayerID=lpForbidRoom->nPlayerID;
		rf.nCreateTime=UwlGetCurrentSeconds();
		rf.nExpiration=rf.nCreateTime+lpForbidRoom->nDuration*60;
		rf.bForbid=TRUE;
        lpRoomData->m_mapRoomForbid.SetAt(rf.nPlayerID,rf);
	}
	else
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 	 	lpRoomData->m_mapRoomForbid.RemoveKey(lpForbidRoom->nPlayerID);
		 
	 
	}
	return TRUE;
}


BOOL CBaseSockServer::OnDelMutexHard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPDEL_MUTEX_HARD lpDelMutexHard = LPDEL_MUTEX_HARD(PBYTE(lpRequest->pDataPtr));
  	REQUEST response;
	memset(&response, 0, sizeof(response));
  	CBaseRoomData* lpRoomData = NULL;
  
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpDelMutexHard->nRoomID, lpRoomData))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		/////////////////
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpDelMutexHard->nAdminID, pPlayer)) 
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}
		if(lstrcmp(pPlayer->pe.szHardID,lpDelMutexHard->szAdminHardID))
		{
			response.head.nRequest = GR_HARDID_MISMATCH;
			goto SendResponse;
		}
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}
		if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ADMIN))//不是网管
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
		if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_MUTEX))//不是隔离网管
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
 			
		TransmitRequest(lpContext, lpRequest);
	    return TRUE;
	 
	}
 	
SendResponse:
	BOOL bSendOK = FALSE;
	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
	UwlClearRequest(&response);
	return TRUE;
}
BOOL CBaseSockServer::OnAddMutexHard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPADD_MUTEX_HARD lpAddMutexHard = LPADD_MUTEX_HARD(PBYTE(lpRequest->pDataPtr));
  	REQUEST response;
	memset(&response, 0, sizeof(response));
  	CBaseRoomData* lpRoomData = NULL;
  
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpAddMutexHard->mh.nRoomID, lpRoomData))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		/////////////////
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpAddMutexHard->nAdminID, pPlayer)) 
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}
		if(lstrcmp(pPlayer->pe.szHardID,lpAddMutexHard->szAdminHardID))
		{
			response.head.nRequest = GR_HARDID_MISMATCH;
			goto SendResponse;
		}
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}
		if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ADMIN))//不是网管
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
		if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_MUTEX))//不是隔离网管
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
		////////////////////////////////////////

		LPPLAYER pPlayer1 = NULL;
		LPPLAYER pPlayer2 = NULL;
		lpRoomData->m_mapPlayer.Lookup(lpAddMutexHard->mh.nUserID1, pPlayer1);
		lpRoomData->m_mapPlayer.Lookup(lpAddMutexHard->mh.nUserID2, pPlayer2);

		if(pPlayer1==NULL || pPlayer2==NULL)
		{
 			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
		else
		{	
 			lstrcpyn(lpAddMutexHard->mh.szMachineID1,pPlayer1->pe.szMachineID,MAX_HARDID_LEN);
 			lstrcpyn(lpAddMutexHard->mh.szMachineID2,pPlayer2->pe.szMachineID,MAX_HARDID_LEN);

 			UwlAddrToName( pPlayer1->pe.dwIPAddr,lpAddMutexHard->mh.szFromIP1);
 			UwlAddrToName( pPlayer2->pe.dwIPAddr,lpAddMutexHard->mh.szFromIP2);
		
 			lpAddMutexHard->mh.dwSysVer1=pPlayer1->pe.dwSysVer;
 			lpAddMutexHard->mh.dwSysVer2=pPlayer2->pe.dwSysVer;	
			lpAddMutexHard->mh.nDeposit1=pPlayer1->nDeposit;
 			lpAddMutexHard->mh.nDeposit2=pPlayer2->nDeposit;	
			
		  	TransmitRequest(lpContext, lpRequest);
		    return TRUE;
		}
	}
 	
SendResponse:
	BOOL bSendOK = FALSE;
	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
	UwlClearRequest(&response);
	return TRUE;
}

BOOL CBaseSockServer::OnInvitePlayer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	LPINVITE_PLAYER lpInvitePlayer = LPINVITE_PLAYER(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpInvitePlayer->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		if(lpRoomData->IsDarkRoom())
			return TRUE;
		if(lpRoomData->IsRandomRoom())
			return TRUE;
		if (lpRoomData->IsForbidInvite())
			return TRUE;
		LPPLAYER pSender = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpInvitePlayer->nUserID, pSender)) 
			return TRUE;
		if(lstrcmp(pSender->pe.szHardID,lpInvitePlayer->szHardID))
			return TRUE;
		if(pSender->pe.lTokenID!= lpContext->lTokenID)
			return TRUE;
		pSender->pe.dwLatestTime = GetTickCount();

		///////////////////////////////////////////////////
		LPPLAYER pPlayer = NULL;
 		if(!lpRoomData->m_mapPlayer.Lookup(lpInvitePlayer->nPlayer, pPlayer)) 
			return TRUE;
	    if(pPlayer->nStatus==PLAYER_STATUS_PLAYING)
			return TRUE;
  	    if(lpRoomData->IsSoloRoom() && pPlayer->nStatus ==PLAYER_STATUS_WAITING)//Solo房间玩家已进入游戏
			return TRUE;
		if(pSender->nStatus!=PLAYER_STATUS_SEATED &&  pSender->nStatus!=PLAYER_STATUS_WAITING) 
			return TRUE;
 	    if(pSender->nTableNO==pPlayer->nTableNO)
			return TRUE;
 		//////////////////////////////////////////////////////////////////////////////
 	    if(!SaveInviteAction(lpInvitePlayer->nUserID,lpInvitePlayer->nPlayer))
			return FALSE;

		memset(lpInvitePlayer->szHardID,0,sizeof(lpInvitePlayer->szHardID));
  		NotifyOnePlayer(lpRoomData, lpInvitePlayer->nPlayer,
						GR_PLAYER_INVITED, lpInvitePlayer, lpRequest->nDataLen);
		return TRUE;
	}
}

BOOL CBaseSockServer::OnCallPlayer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPCALL_PLAYER lpCallPlayer = LPCALL_PLAYER(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpCallPlayer->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
 			return TRUE;

		LPPLAYER pSender = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpCallPlayer->nUserID, pSender)) 
			return TRUE;
 		if(lstrcmp(pSender->pe.szHardID,lpCallPlayer->szHardID))
			return TRUE;
		if(pSender->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		pSender->pe.dwLatestTime = GetTickCount();

		if(IS_BIT_SET(pSender->nUserType ,USER_TYPE_ENTERHIDING))
			return TRUE;

		LPPLAYER pPlayer = NULL;
  		if(!lpRoomData->m_mapPlayer.Lookup(lpCallPlayer->nPlayer, pPlayer))
			return TRUE;
		////////////////////////////////////////
 	 	if(!IS_BIT_SET(pSender->nUserType,USER_TYPE_ADMIN) && !IS_BIT_SET(pPlayer->nUserType,USER_TYPE_ADMIN))  
		{
 			///////////////同桌玩家禁止呼叫
			if(pSender->nStatus==PLAYER_STATUS_PLAYING  &&  pPlayer->nStatus==PLAYER_STATUS_PLAYING && pSender->nTableNO == pPlayer->nTableNO)
 				 return TRUE;
 			if(!SaveCallAction(lpCallPlayer->nUserID,lpCallPlayer->nPlayer))
				return TRUE;
		}
		memset(lpCallPlayer->szHardID,0,sizeof(lpCallPlayer->szHardID));
 		NotifyOnePlayer(lpRoomData, lpCallPlayer->nPlayer,
						GR_PLAYER_CALLED, lpCallPlayer, lpRequest->nDataLen);
		return TRUE;
	}
}


BOOL CBaseSockServer::OnResetCheckSvrIP(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
   	LPWRITE_PROFILE lpWriteProfile = LPWRITE_PROFILE(PBYTE(lpRequest->pDataPtr));
	WritePrivateProfileString(lpWriteProfile->pi.szSection,	lpWriteProfile->pi.szKeyName,lpWriteProfile->pi.szString,g_szIniFile);

	//////////////////////////////////////
 	TCHAR szPath[MAX_PATH],szTemp[MAX_PATH];
	memset(szPath,0,sizeof(szPath));
 	memset(szTemp,0,sizeof(szTemp));

	GetModuleFileName(GetModuleHandle(NULL), szPath, sizeof(szPath));
	UwlSplitPath(szPath, SPLIT_DRIVE_DIR, szPath);

	lstrcpy(szTemp,szPath);
 	lstrcat(szTemp,_T("*.*"));	//在当前目录下查找*.*
   
	WIN32_FIND_DATA          FindFileData;  
	HANDLE hFind=FindFirstFile(szTemp,&FindFileData);
	if(hFind==INVALID_HANDLE_VALUE)//目录文件失败
		return FALSE;
	
 	BOOL  bDone=TRUE; 
	int nNum=0;//找到的文件夹数
 	while(bDone)
	{
 		if(IS_BIT_SET(FindFileData.dwFileAttributes , FILE_ATTRIBUTE_DIRECTORY)) 
		{
			if(lstrcmp(FindFileData.cFileName ,_T("."))==0||lstrcmp(FindFileData.cFileName ,_T(".."))==0)
			{
				bDone=FindNextFile(hFind,&FindFileData);
				continue;///如果是.和..子目录,跳到下一个
			}
			//找到子目录
	 		CString sFolder,sConfigFile ;
	 		sFolder.Format(_T("%s%s\\"),szPath,FindFileData.cFileName);
 			sConfigFile=GetConfigFileUnderFolder(sFolder);
            if(UwlPathExists(sConfigFile,FALSE))
			{
				TCHAR szText[32];
				memset(szText,0,sizeof(szText));
				GetPrivateProfileString(lpWriteProfile->pi.szSection,lpWriteProfile->pi.szKeyName,	_T(""),	szText,sizeof(szText) ,sConfigFile);
                if(lstrlen(szText)>0)
					WritePrivateProfileString(lpWriteProfile->pi.szSection,	lpWriteProfile->pi.szKeyName,lpWriteProfile->pi.szString,sConfigFile);
			}
  			nNum++;
		}
		else
		{
		   
		}
		bDone=FindNextFile(hFind,&FindFileData);
		// 找下一个
	}
	FindClose(hFind);

	return TRUE;
}
 

BOOL CBaseSockServer::OnReconnectSvr(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	int nResult=UR_OPERATE_SUCCEEDED;
 
	LPRECONNECT_SVR lpReconnectSvr = LPRECONNECT_SVR(PBYTE(lpRequest->pDataPtr));
  
  	REQUEST response;
	memset(&response, 0, sizeof(response));

	response.head.nRequest = nResult;
	response.pDataPtr =0;
	response.nDataLen =0;

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

 	if(nResult==UR_OPERATE_SUCCEEDED)
	{	
		if(lstrlen(lpReconnectSvr->szReconnectIP)>0)
			WritePrivateProfileString(_T("CheckServer"),_T("Name"),lpReconnectSvr->szReconnectIP,g_szIniFile);
		PostThreadMessage(g_uiThrdConnect, UM_DATA_TOSEND, UR_RECONNECT_SVR, GetTickCount());
	}
 	UwlLogFile(_T("===========ReconnectSvr,AdminID=%d"),lpReconnectSvr->nAdminID);

	return TRUE;
}

BOOL CBaseSockServer::OnRoomUserPulse(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPROOMUSER_PULSE lpRoomUserPulse = LPROOMUSER_PULSE(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpRoomUserPulse->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpRoomUserPulse->nUserID, pPlayer)) 
			return TRUE;
 		if(pPlayer->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		pPlayer->pe.dwPulseTime = GetTickCount();
	}	
	return TRUE;
}
 

BOOL CBaseSockServer::OnWriteProfile(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
   	LPWRITE_PROFILE lpWriteProfile = LPWRITE_PROFILE(PBYTE(lpRequest->pDataPtr));
 	if(IS_BIT_SET(lpWriteProfile->dwFlags,FLAG_WRITEPROFILE_DELKEY))
		WritePrivateProfileString(lpWriteProfile->pi.szSection,	lpWriteProfile->pi.szKeyName,NULL,g_szIniFile);
	else if(IS_BIT_SET(lpWriteProfile->dwFlags,FLAG_WRITEPROFILE_DELSEC))
		WritePrivateProfileString(lpWriteProfile->pi.szSection,	NULL,NULL,g_szIniFile);
	else
		WritePrivateProfileString(lpWriteProfile->pi.szSection,	lpWriteProfile->pi.szKeyName,lpWriteProfile->pi.szString,g_szIniFile);
 	return TRUE;
}

BOOL CBaseSockServer::OnKickoffRoomDead(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	////定时器踢僵死(KickoffRoom.exe)
	LPKICKOFF_ROOMDEAD lpKickoffRoomDead = LPKICKOFF_ROOMDEAD(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpKickoffRoomDead->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));

		// gamehall to modify
		int nDeadTime = lpKickoffRoomDead->nDeadTime;
		int nStatic = lpKickoffRoomDead->nStatic;

		KickOffDeadPlayersInRoom(lpRoomData, nDeadTime, nStatic,FALSE);
	}
	return TRUE;
}

BOOL CBaseSockServer::OnKickoffWalkAround(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
  	LPKICKOFF_WALKAROUND lpKickoff = LPKICKOFF_WALKAROUND(PBYTE(lpRequest->pDataPtr));
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpKickoff->nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpKickoff->nPlayer, pPlayer)) 
			return FALSE;
 		KickOffOneWalkAround(lpRoomData,pPlayer);
  	}
 
	return TRUE;
}
BOOL CBaseSockServer::OnKickoffRoomPlayer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
   	LPKICKOFF_ROOM_PLAYER lpKickOff = LPKICKOFF_ROOM_PLAYER(PBYTE(lpRequest->pDataPtr));
 
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpKickOff->nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		///////////////////////////////////////////////
		if(IS_BIT_SET(lpRoomData->roomdata.dwManages , ROOM_MNG_FORBID_KICKOFF_PLAYER))
			return FALSE;
		/////////////////
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpKickOff->nAdminID, pPlayer)) 
			return FALSE;
		if(lstrcmp(pPlayer->pe.szHardID,lpKickOff->szAdminHardID))
			return FALSE;
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
			return FALSE;
		if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ADMIN))//不是网管
			return FALSE;
		if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_KICKOFF))//不是踢人网管
			return FALSE;
		////////////////////////////////////////
		memset(lpKickOff->szAdminHardID,0,sizeof(lpKickOff->szAdminHardID));
		NotifyOnePlayer(lpRoomData, lpKickOff->nPlayer,	GR_KICKOFF_ROOM_PLAYER, lpKickOff, lpRequest->nDataLen);

		CONTEXT_HEAD context;
		ZeroMemory(&context ,sizeof(context));
		context.dwFlags|=CH_FLAG_SYSTEM_EJECT;		
		KickOffRoomPlayer(&context, lpRoomData, lpKickOff->nPlayer,FALSE);
		NotifyServer(GR_KICKOFF_ROOM_PLAYER,lpKickOff,lpRequest->nDataLen);

 
	}
	return TRUE;
} 

BOOL CBaseSockServer::OnClearTable(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	LPCLEAR_TABLE lpClearTable = LPCLEAR_TABLE(PBYTE(lpRequest->pDataPtr));
  
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpClearTable->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		/////////////////
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpClearTable->nAdminID, pPlayer)) 
			return FALSE;
		if(lstrcmp(pPlayer->pe.szHardID,lpClearTable->szAdminHardID))
			return FALSE;
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
			return FALSE;
		if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ADMIN))//不是网管
			return FALSE;
		if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_KICKOFF))//不是踢人网管
			return FALSE;
		////////////////////////////////////////


		LPTABLE lpTable = NULL;
		if(lpRoomData->m_mapTable.Lookup(lpClearTable->nTableNO, lpTable) && lpTable)
		{
			if(lpTable->th.nStatus == TABLE_STATUS_PLAYING)
			{
				lpRoomData->PostClearTable(lpClearTable->nTableNO);
		          
				///////////////////////////////////////////////////////////////////////////////////////////////
				memset(lpClearTable->szAdminHardID,0,sizeof(lpClearTable->szAdminHardID));
				for(int i = 0; i < lpTable->th.nPlayerCount; i++)
				{
					LPPLAYER lpSeated = NULL;
					if(lpRoomData->m_mapPlayer.Lookup(lpTable->nPlayerAry[i], lpSeated))
						NotifyOnePlayer(lpRoomData, lpSeated->nUserID,GR_CLEAR_TABLE, lpClearTable, lpRequest->nDataLen);
				}
				for(i = 0; i < MAX_CHAIR_COUNT; i++)
				{
					for(int j = 0; j < MAX_VISITOR_COUNT; j++)
					{  
						if(0 == lpTable->nVisitorAry[i][j])
							continue;
						LPPLAYER lpVisitor = NULL;
						if(lpRoomData->m_mapPlayer.Lookup(lpTable->nVisitorAry[i][j], lpVisitor))
 						    NotifyOnePlayer(lpRoomData, lpVisitor->nUserID,GR_CLEAR_TABLE, lpClearTable, lpRequest->nDataLen);
					}
				}
			}
		}
		return TRUE;
	}
}

BOOL CBaseSockServer::OnSysMsgToRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	LPSYSTEM_MSG lpSystemMsg = LPSYSTEM_MSG(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpSystemMsg->nRoomID, lpRoomData))
 			return TRUE;
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 
 		NotifyRoomPlayers(lpRoomData, GR_SYSMSG_TO_ROOM, lpSystemMsg, lpRequest->nDataLen);
		return TRUE;
	}
}

BOOL CBaseSockServer::OnAdminMsgToRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	LPSYSTEM_MSG lpSystemMsg = LPSYSTEM_MSG(PBYTE(lpRequest->pDataPtr));
  
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpSystemMsg->nRoomID, lpRoomData))
 			return FALSE;
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 	/////////////////
		LPPLAYER pAdmin = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpSystemMsg->nMsgFrom, pAdmin)) 
			return TRUE;
		if(pAdmin->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		pAdmin->pe.dwLatestTime = GetTickCount();

		if(!IS_BIT_SET(pAdmin->nUserType ,USER_TYPE_ADMIN))//不是网管
			return TRUE;

	////////////////////////////////////////
 		NotifyRoomPlayers(lpRoomData, GR_ADMINMSG_TO_ROOM, lpSystemMsg, lpRequest->nDataLen);
		return TRUE;
	}
}

BOOL CBaseSockServer::OnChatToPlayer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPCHAT_TO_PLAYER lpChatToPlayer = LPCHAT_TO_PLAYER(PBYTE(lpRequest->pDataPtr));
    if(lstrlen(lpChatToPlayer->szChatMsg) > (MAX_ROOMCHAT_LEN-2) )
		return FALSE;
 
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpChatToPlayer->nRoomID, lpRoomData)){
			return TRUE;
		}
	}

	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));

		if(lpRoomData->IsDarkRoom())
			return TRUE;

  		//////////////////////////////////////////// 
  		LPPLAYER pSender = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpChatToPlayer->nUserID, pSender)) 
			return TRUE;
 		LPPLAYER pPlayer = NULL;
  		if(!lpRoomData->m_mapPlayer.Lookup(lpChatToPlayer->nPlayer, pPlayer))
			return TRUE;

	   	if(lstrcmp(pSender->pe.szHardID,lpChatToPlayer->szHardID))
			return TRUE;
		if(pSender->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		pSender->pe.dwLatestTime = GetTickCount();

		if(IS_BIT_SET(pSender->nUserType ,USER_TYPE_ENTERHIDING))
			return TRUE;

		CHAT_FROM_PLAYER cfp;
		memset(&cfp,0,sizeof(cfp));
	 	cfp.nRoomID=lpChatToPlayer->nRoomID;
		cfp.nUserID=lpChatToPlayer->nUserID;
		cfp.nPlayer=lpChatToPlayer->nPlayer;

		CHAT_FROM_PLAYER cfpself;
		memcpy(&cfpself,&cfp,sizeof(cfpself));

		////////////////////////////////////////
		BOOL bIsAdminSender=IS_BIT_SET(pSender->nUserType,USER_TYPE_ADMIN);
		BOOL bIsAdminTaker=IS_BIT_SET(pPlayer->nUserType,USER_TYPE_ADMIN);

		BOOL bMember = IS_BIT_SET(pSender->nUserType,USER_TYPE_MEMBER);//会员
		////////////////////////////////////
		// 聊天消息长度越界导致大厅崩溃   //
		// 暂时这么处理 带大厅更新后可解除//
		if(lstrlen(lpChatToPlayer->szChatMsg) > 192 )
			lpChatToPlayer->szChatMsg[192]=0;
		////////////////////////////////////
 		if(bIsAdminSender || bIsAdminTaker )  //其中有个是网管
		{
			if(bIsAdminSender)//发送消息的是网管
			{

				CString sHeader;
				if (bMember)
					sHeader.Format(_T("<z=%d,e=lb,l=%s//%s%d>%s"),GetVipFaceByLevel(pSender->nMemberLevel),AHREF_APP,APP_FINDUSER,pSender->nUserID,pSender->szUsername);
				else
					sHeader.Format(_T("<e=lb,l=%s//%s%d>%s"),AHREF_APP,APP_FINDUSER,pSender->nUserID,pSender->szUsername);
			
				cfp.nHeadLen=lstrlen(sHeader);
				sprintf(cfp.szChatMsg,_T("%s：<c=255>%s<>\r\n"),sHeader,lpChatToPlayer->szChatMsg);
			 	cfp.nMsgLen=lstrlen(cfp.szChatMsg)+1;
			 	cfp.dwFlags=FLAG_CHAT_ADMINMSG;

				memcpy(&cfpself,&cfp,sizeof(cfpself));
			}
			else
			{
 
 				CString sHeader;
				if (bMember)
					sHeader.Format(_T("<z=%d,e=l,l=%s//%s%d>%s"),GetVipFaceByLevel(pSender->nMemberLevel),AHREF_APP,APP_FINDUSER,pSender->nUserID,pSender->szUsername);
				else
					sHeader.Format(_T("<e=l,l=%s//%s%d>%s"),AHREF_APP,APP_FINDUSER,pSender->nUserID,pSender->szUsername);
			
				cfp.nHeadLen=lstrlen(sHeader);
				sprintf(cfp.szChatMsg,_T("%s：<c=0>%s<>\r\n"),sHeader,lpChatToPlayer->szChatMsg);
		 		cfp.nMsgLen=lstrlen(cfp.szChatMsg)+1;
				cfp.dwFlags=FLAG_CHAT_ADMINMSG;

				cfpself.nHeadLen=lstrlen(sHeader);
				sprintf(cfpself.szChatMsg,_T("%s：<c=32768>%s<>\r\n"),sHeader,lpChatToPlayer->szChatMsg);
 		 		cfpself.nMsgLen=lstrlen(cfpself.szChatMsg)+1;
				cfpself.dwFlags=FLAG_CHAT_ADMINMSG;
			}

		}
		else
		{	
 			int nRemains=0;
 			int nMinExp=0;
 			BOOL bAllowed=TRUE;
			CString sForbid,sTag;
			sTag.Format(_T("<c=%d,s=220>"),RGB(255,0,0));
		    if(!IS_BIT_SET(pSender->nUserType,USER_TYPE_MEMBER))//非会员
			{
 				if(lstrlen(lpChatToPlayer->szChatMsg) >PROPER_CHATMSG_LEN)
					lpChatToPlayer->szChatMsg[PROPER_CHATMSG_LEN]=0;
			}
  
 			if(pPlayer->nStatus==PLAYER_STATUS_PLAYING && pSender->nTableNO == pPlayer->nTableNO)
			{
				bAllowed=FALSE;
 		 		sForbid.Format(_T("%s同桌玩家禁止聊天\r\n"),sTag);
			}
 			else if(IS_BIT_SET(lpRoomData->roomdata.dwManages , ROOM_MNG_FORBIDPRIVCHAT))
			{
				bAllowed=FALSE;
 				sForbid.Format(_T("%s本房间禁止聊天\r\n"),sTag);
			}
 			else if(IsForbidChatUserID(lpChatToPlayer->nUserID,nRemains))
			{
				bAllowed=FALSE;
 				sForbid.Format(_T("%s您被禁止在本房间聊天(%s后解禁)\r\n"),sTag,GetRemainsBySeconds(nRemains));
			}
 			else if(IsForbidChatMachine(pSender->pe.szHardID,nRemains) )
			{
				bAllowed=FALSE;
 				sForbid.Format(_T("%s您被禁止在本房间聊天(%s后解禁)\r\n"),sTag,GetRemainsBySeconds(nRemains));
			}
		 
 			else if(IS_BIT_SET(lpRoomData->roomdata.dwManages , ROOM_MNG_FORBIDDISTURB) && pPlayer->nStatus==PLAYER_STATUS_PLAYING)
			{
	 			bAllowed=FALSE;
 				sForbid.Format(_T("%s对方正在玩，禁止打扰\r\n"),sTag);
			}
 			else if(IS_BIT_SET(lpRoomData->roomdata.dwManages , ROOM_MNG_FORBIDDISTURB) && pPlayer->nStatus==PLAYER_STATUS_LOOKON)
			{
	 			bAllowed=FALSE;
 				sForbid.Format(_T("%s对方正在旁观，禁止打扰\r\n"),sTag);
			}
			else if(lpRoomData->IsForbidPrivChatByExp(pSender->nTimeCost /60 ,nMinExp ))
			{
 				bAllowed=FALSE;
 				sForbid.Format(_T("%s新手(经验值＜%d分钟)禁止在本房间聊天\r\n"),sTag,nMinExp);
			}
 			else if(IsIncludeFilterWord(lpChatToPlayer->szChatMsg))
			{
				bAllowed=FALSE;
 				sForbid.Format(_T("%s禁止发送违禁单词或符号\r\n"),sTag);
			}
			else if(!SaveChatAction(lpChatToPlayer->nUserID,lpChatToPlayer->nPlayer))
			{
 				bAllowed=FALSE;
 				sForbid.Format(_T("%s禁止在房间里打扰多人\r\n"),sTag);
			}
			if(bAllowed==FALSE )
			{
   				lstrcpy(cfp.szChatMsg,sForbid);
				cfp.nHeadLen=0;
				cfp.nMsgLen=lstrlen(cfp.szChatMsg)+1;
				cfp.dwFlags=FLAG_CHAT_SYSNOTIFY;

				int nSendLen=sizeof(CHAT_FROM_PLAYER)-sizeof(cfp.szChatMsg)+cfp.nMsgLen;
				NotifyOnePlayer(lpRoomData,lpChatToPlayer->nUserID,GR_CHAT_FROM_PLAYER, &cfp,nSendLen);
				return TRUE;
			}

 			CString sHeader;
			if (bMember)
				sHeader.Format(_T("<z=%d,e=l,l=%s//%s%d>%s"),GetVipFaceByLevel(pSender->nMemberLevel),AHREF_APP,APP_FINDUSER,pSender->nUserID,pSender->szUsername);
			else
				sHeader.Format(_T("<e=l,l=%s//%s%d>%s"),AHREF_APP,APP_FINDUSER,pSender->nUserID,pSender->szUsername);
			
			cfp.nHeadLen=lstrlen(sHeader);
			sprintf(cfp.szChatMsg,_T("%s：<c=%d>%s<>\r\n"),sHeader, GetMsgTextColor(bMember),lpChatToPlayer->szChatMsg);
		 	cfp.nMsgLen=lstrlen(cfp.szChatMsg)+1;
		 	cfp.dwFlags=FLAG_CHAT_PLAYERMSG;

			cfpself.nHeadLen=lstrlen(sHeader);
			sprintf(cfpself.szChatMsg,_T("%s：<c=32768>%s<>\r\n"),sHeader,lpChatToPlayer->szChatMsg);
 		 	cfpself.nMsgLen=lstrlen(cfpself.szChatMsg)+1;
		 	cfpself.dwFlags=FLAG_CHAT_PLAYERMSG;

		}
 		
		int nSendLen=sizeof(CHAT_FROM_PLAYER)-sizeof(cfpself.szChatMsg)+cfpself.nMsgLen;
  		NotifyOnePlayer(lpRoomData, lpChatToPlayer->nUserID,GR_CHAT_FROM_PLAYER, &cfpself,nSendLen);//发给自己

		if(lpChatToPlayer->nPlayer!=lpChatToPlayer->nUserID)//接收者不是自己
		{
			nSendLen=sizeof(CHAT_FROM_PLAYER)-sizeof(cfp.szChatMsg)+cfp.nMsgLen;
 			NotifyOnePlayer(lpRoomData, lpChatToPlayer->nPlayer,GR_CHAT_FROM_PLAYER,  &cfp, nSendLen);
		}

		return TRUE;
	}
}

BOOL CBaseSockServer::OnChatToRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	LPCHAT_TO_ROOM lpChatToRoom = LPCHAT_TO_ROOM(PBYTE(lpRequest->pDataPtr));
    if(lstrlen(lpChatToRoom->szChatMsg) > (MAX_ROOMCHAT_LEN-2) )
		return FALSE;

	lpChatToRoom->dwIPAddr = GetClientAddress(lpContext->hSocket, lpContext->lTokenID);

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpChatToRoom->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	/*
		测试 充值到大厅room
	
	LPPAY_RESULT lpPay = new PAY_RESULT();
	lpPay->nUserID = lpChatToRoom->nUserID;
	lpPay->nGameID = lpChatToRoom->nGameID;
	lpPay->nPayTo = TCY_PAY_TO(PAY_TO_COFFER+rand()%3);
	lpPay->nPayFor = TCY_PAY_FOR(PAY_FOR_DEPOSIT+rand()%3);
	//lpPay->llOperationID = userid;
	lpPay->llBalance = 1500;
	lpPay->nOperateAmount = 1000;
	lpPay->nRoomID = lpChatToRoom->nRoomID;
	REQUEST req(GR_USER_PAY_RESULT,lpPay,sizeof(PAY_RESULT));
	g_pSockClient->OnUserPayResult(&req);
	*/
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));

		lpChatToRoom->nGameID = lpRoomData->roomdata.nGameID;
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpChatToRoom->nUserID, pPlayer)) 
			return TRUE;
 	   	if(lstrcmp(pPlayer->pe.szHardID,lpChatToRoom->szHardID))
			return TRUE;
		if(pPlayer->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		pPlayer->pe.dwLatestTime = GetTickCount();

		CHAT_FROM_ROOM cfr;
		memset(&cfr,0,sizeof(cfr));
	 	cfr.nRoomID=lpChatToRoom->nRoomID;
		cfr.nUserID=lpChatToRoom->nUserID;

		BOOL bMember = IS_BIT_SET(pPlayer->nUserType,USER_TYPE_MEMBER );//会员
		////////////////////////////////////
		// 聊天消息长度越界导致大厅崩溃   //
		// 暂时这么处理 带大厅更新后可解除//
		if(lstrlen(lpChatToRoom->szChatMsg) > 192 )
			lpChatToRoom->szChatMsg[192]=0;
		////////////////////////////////////
  		////////////////////////////////////////////////////////////////
  		if(IS_BIT_SET(pPlayer->nUserType,USER_TYPE_ADMIN)) //是网管
		{ 	
 			CString sHeader;
			if (bMember)
				sHeader.Format(_T("<z=%d,e=lb,l=%s//%s%d>%s"),GetVipFaceByLevel(pPlayer->nMemberLevel),AHREF_APP,APP_FINDUSER,pPlayer->nUserID,pPlayer->szUsername);
			else
				sHeader.Format(_T("<e=lb,l=%s//%s%d>%s"),AHREF_APP,APP_FINDUSER,pPlayer->nUserID,pPlayer->szUsername);
		
			cfr.nHeadLen=lstrlen(sHeader);
			sprintf(cfr.szChatMsg,_T("%s：<c=255,e=b,s=240>%s<>\r\n"),sHeader,lpChatToRoom->szChatMsg);
		 	cfr.nMsgLen=lstrlen(cfr.szChatMsg)+1;
			cfr.dwFlags=FLAG_CHAT_ADMINMSG;

		}
		else
		{	
 			int nRemains=0;
			int nMinExp=0;
 			BOOL bAllowed=TRUE;
 			CString sForbid,sTag;
			sTag.Format(_T("<c=%d,s=220>"),RGB(255,0,0));
	    	if(!IS_BIT_SET(pPlayer->nUserType,USER_TYPE_MEMBER))//非会员
			{
    			if(lstrlen(lpChatToRoom->szChatMsg) > PROPER_CHATMSG_LEN)
					lpChatToRoom->szChatMsg[PROPER_CHATMSG_LEN]=0;
			}
		
			//房间说话，时间间隔2秒以上
			ROOM_CHAT rc;
			ZeroMemory(&rc, sizeof(ROOM_CHAT));
			DWORD dwLastestTime = GetTickCount();
			if (lpRoomData->m_mapRoomChat.Lookup(lpChatToRoom->nUserID, rc))
			{
				if (abs(dwLastestTime-rc.dwLatestTime)<ROOM_CHAT_INTERVAL)
				{
					bAllowed=FALSE;
 		 			sForbid.Format(_T("%s您说话太快了，请休息一下\r\n"),sTag);
				}
			}
			rc.nUserID = lpChatToRoom->nUserID;
			rc.dwLatestTime = dwLastestTime;
			lpRoomData->m_mapRoomChat.SetAt(lpChatToRoom->nUserID, rc);

   			if(pPlayer->nStatus==PLAYER_STATUS_PLAYING )
			{
				bAllowed=FALSE;
 		 		sForbid.Format(_T("%s正在玩游戏时不允许发消息\r\n"),sTag);
			}
  			else if(pPlayer->nStatus ==PLAYER_STATUS_LOOKON )
			{
				bAllowed=FALSE;
 				sForbid.Format(_T("%s正在旁观时不允许发消息\r\n"),sTag);

			}
 			else if(lpRoomData->IsDarkRoom() || IS_BIT_SET(lpRoomData->roomdata.dwManages , ROOM_MNG_FORBIDROOMCHAT) )
			{
				bAllowed=FALSE;
 				sForbid.Format(_T("%s本房间禁止发消息\r\n"),sTag);
			}
 		  	else if(IsForbidChatUserID(lpChatToRoom->nUserID,nRemains) )
	 		{
				bAllowed=FALSE;
  				sForbid.Format(_T("%s管理员禁止你在本房间发消息(%s后解禁)\r\n"),sTag,GetRemainsBySeconds(nRemains));
			}
 		  	else if(IsForbidChatMachine(pPlayer->pe.szHardID,nRemains))
	 		{
				bAllowed=FALSE;
  				sForbid.Format(_T("%s管理员禁止你在本房间发消息(%s后解禁)。\r\n"),sTag,GetRemainsBySeconds(nRemains));
			}
  			else if(lpRoomData->IsForbidRoomChatByExp(pPlayer->nTimeCost /60 ,nMinExp ))
			{
 				bAllowed=FALSE;
 				sForbid.Format(_T("%s新手(经验值＜%d分钟)禁止在本房间发消息\r\n"),sTag,nMinExp);
			}
			else if(IsIncludeFilterWord(lpChatToRoom->szChatMsg))
			{
 				bAllowed=FALSE;
 				sForbid.Format(_T("%s禁止发送违禁单词或符号\r\n"),sTag);
			}
			else 
			{

			}
  
			if(bAllowed==FALSE )
			{
  				lstrcpy(cfr.szChatMsg,sForbid);	
				cfr.nHeadLen=0;
 				cfr.nMsgLen=lstrlen(cfr.szChatMsg)+1;
  				cfr.dwFlags=FLAG_CHAT_SYSNOTIFY;
 				int nSendLen=sizeof(CHAT_FROM_ROOM)-sizeof(cfr.szChatMsg)+cfr.nMsgLen;
				NotifyOnePlayer(lpRoomData,lpChatToRoom->nUserID,GR_CHAT_FROM_ROOM, &cfr,nSendLen);
				return TRUE;
			}

			CString sHeader;
			if (bMember)
				sHeader.Format(_T("<z=%d,e=l,l=%s//%s%d>%s"),GetVipFaceByLevel(pPlayer->nMemberLevel),AHREF_APP,APP_FINDUSER,pPlayer->nUserID,pPlayer->szUsername);
			else
				sHeader.Format(_T("<e=l,l=%s//%s%d>%s"),AHREF_APP,APP_FINDUSER,pPlayer->nUserID,pPlayer->szUsername);
 		
			cfr.nHeadLen=lstrlen(sHeader);
			sprintf(cfr.szChatMsg,_T("%s：<c=%d>%s<>\r\n"),sHeader, GetMsgTextColor(bMember),lpChatToRoom->szChatMsg);
  		 	cfr.nMsgLen=lstrlen(cfr.szChatMsg)+1;
 			cfr.dwFlags=FLAG_CHAT_PLAYERMSG;

		}

 		/////////////////////////////////////////////////////////////////////////
		int nSendLen=sizeof(CHAT_FROM_ROOM)-sizeof(cfr.szChatMsg)+cfr.nMsgLen;
    	NotifyRoomPlayers(lpRoomData, GR_CHAT_FROM_ROOM, &cfr, nSendLen);

		//记录聊天日志
		if (IS_BIT_SET(lpRoomData->roomdata.dwConfigs , ROOM_CFG_CHATLOG))
		{
			NotifyServer(GR_CHAT_TO_ROOM,lpChatToRoom,lpRequest->nDataLen);
		}
 	 					
		return TRUE;
	}
}

BOOL CBaseSockServer::OnClearRoomChat(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	LPCLEAR_ROOM_CHAT lpClearRoomChat = LPCLEAR_ROOM_CHAT(PBYTE(lpRequest->pDataPtr));
 
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpClearRoomChat->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
	/////////////////
		LPPLAYER pAdmin = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpClearRoomChat->nUserID, pAdmin)) 
			return TRUE;
 		if(lstrcmp(pAdmin->pe.szHardID,lpClearRoomChat->szHardID))
			return TRUE;
		if(pAdmin->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		pAdmin->pe.dwLatestTime = GetTickCount();

		if(!IS_BIT_SET(pAdmin->nUserType ,USER_TYPE_ADMIN))//不是网管
			return TRUE;

		NotifyRoomPlayers(lpRoomData, GR_CLEAR_ROOM_CHAT, NULL, 0,lpContext->hSocket, lpContext->lTokenID);
		return TRUE;
	}
}

BOOL CBaseSockServer::OnCloseSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
	LONG token = lpContext->lTokenID;

	// 连接异常断开
	int roomid = RemoveTokenRoomSock(token);
	if(roomid==0)
		return TRUE;

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(roomid, lpRoomData)){
		}
	}
	if(lpRoomData){ // break of connection for room
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		USER_SOCK us;
		ZeroMemory(&us,sizeof(USER_SOCK));
		LPPLAYER pPlayer = NULL;
		lpRoomData->m_mapTokenUserSock.Lookup(token, us);
 		if(  us.nUserID && token ){ // 突然断线
			
			lpRoomData->m_mapUserSock.RemoveKey(us.nUserID);
  			lpRoomData->m_mapTokenUserSock.RemoveKey(token);
 			if(lpRoomData->m_mapPlayer.Lookup(us.nUserID, pPlayer) && pPlayer)
			{
 				if(lpRoomData->IsMatchRoom())
				{
					lpRoomData->AT_OnSocketClose(pPlayer);
			        return TRUE;
				}
 
				if(pPlayer->nStatus == PLAYER_STATUS_PLAYING)
				{
					LPTABLE lpTable = NULL;
					if(lpRoomData->m_mapTable.Lookup(pPlayer->nTableNO, lpTable) && lpTable)
					{
						if(pPlayer->nChairNO>=0 &&  pPlayer->nChairNO < MAX_CHAIR_COUNT)
 							lpTable->dwBreakCount |=  (1 <<  pPlayer->nChairNO );//x or (1 shl (k-1)) 把右数第k位变成1
					}
			
  			 		if( lpTable->dwBreakCount != lpRoomData->GetTotalPlayingBreak(lpTable))
 						return TRUE;

					//Add on 20150623 
					//判断 “断线不清桌”配置
					if (IS_BIT_SET(lpRoomData->roomdata.dwConfigs, ROOM_CFG_ALLBREAKNOCLEAR))
						return TRUE;

					//////////该桌所有玩家全部断线，清理桌子，该桌玩家离开房间
					lpRoomData->PostClearTable(pPlayer->nTableNO);

					int nPlayerAry[MAX_CHAIR_COUNT];
					memcpy(nPlayerAry, lpTable->nPlayerAry, sizeof(nPlayerAry));
 				
					SetTableAsEmptyOrLeave(lpRoomData, pPlayer);
					
					for(int i = 0; i < MAX_CHAIR_COUNT; i++)
					{
						if(nPlayerAry[i])
						{
							LPPLAYER pFind = NULL;
							lpRoomData->m_mapPlayer.Lookup(nPlayerAry[i], pFind);
							RemoveOnePlayer(lpRoomData,pFind,0, 0);
						}	
					}

				}
				else
				{
				    if(lpRoomData->IsSoloRoom() && pPlayer->nStatus ==PLAYER_STATUS_WAITING)//Solo房间玩家已进入游戏
					{


					}
					else
					{
						RemoveOnePlayer(lpRoomData,pPlayer,lpContext->hSocket, lpContext->lTokenID);
					}
				}
			}
		}
	}
	return TRUE;
}



BOOL CBaseSockServer::OnUnWelcome(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
  	LPUNWELCOME lpUnWelcome = LPUNWELCOME(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpUnWelcome->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		if(lpRoomData->IsDarkRoom())
			return TRUE;

		LPPLAYER pSender = NULL;

	 	if(!lpRoomData->m_mapPlayer.Lookup(lpUnWelcome->nUserID, pSender)) 
			return TRUE;
 		if(lstrcmp(pSender->pe.szHardID,lpUnWelcome->szHardID))
			return TRUE;
		if(pSender->pe.lTokenID!= lpContext->lTokenID)
			return TRUE;
		pSender->pe.dwLatestTime = GetTickCount();

		LPPLAYER pPlayer = NULL;
 		if(!lpRoomData->m_mapPlayer.Lookup(lpUnWelcome->nPlayer, pPlayer)) 
			return TRUE;

		if(pSender->nStatus!=PLAYER_STATUS_SEATED &&  pSender->nStatus!=PLAYER_STATUS_WAITING && pSender->nStatus!=PLAYER_STATUS_PLAYING)	//自己在游戏中，也不让不受欢迎的玩家坐下
			return TRUE;

		/*
		if(pPlayer->nStatus!=PLAYER_STATUS_SEATED &&  pPlayer->nStatus!=PLAYER_STATUS_WAITING) 
			return TRUE;
		*/
		//Modify on 20150927 
		//solo房如果已经进入游戏则不再踢人
		if (lpRoomData->IsSoloRoom())
		{
			if(pPlayer->nStatus!=PLAYER_STATUS_SEATED) 
			return TRUE;
		}
		else 
		{
			if(pPlayer->nStatus!=PLAYER_STATUS_SEATED &&  pPlayer->nStatus!=PLAYER_STATUS_WAITING) 
			return TRUE;
		}

		if(pSender->nTableNO!=pPlayer->nTableNO)
			return TRUE;
		int nChairNO=pPlayer->nChairNO;

		if(nChairNO <0 || nChairNO >= lpRoomData->GetChairCount())
			return TRUE;

		/////////////////////////////////////////////////////////////
		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(pSender->nTableNO, pTable))
			return TRUE;
  		if(pPlayer->nUserID != pTable->nPlayerAry[nChairNO]) 	
			return TRUE;	 
  
		SetCommonTableLeave(lpRoomData, pPlayer);
 		SetRandomTableLeave(lpRoomData, pPlayer);
 	 
		pPlayer->nStatus = PLAYER_STATUS_WALKAROUND;
		pPlayer->nTableNO = -1;
		pPlayer->nChairNO = -1;

		PLAYER_POSITION pp;
		memset(&pp,0,sizeof(PLAYER_POSITION));
		pp.nUserID=pPlayer->nUserID;
		pp.nTableNO=pSender->nTableNO;
		pp.nChairNO=nChairNO;
		pp.nNetDelay=pPlayer->nNetSpeed;
		NotifyRoomPlayers(lpRoomData, GR_PLAYER_UNSEATED, &pp, sizeof(PLAYER_POSITION));

	}

	return TRUE;
}
 



BOOL CBaseSockServer::OnDarkSeated(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	REQUEST response;
	memset(&response, 0, sizeof(response));

	LPDARK_SEATED lpDarkSeated = LPDARK_SEATED(PBYTE(lpRequest->pDataPtr));
	LPPLAYER pPlayer = NULL;
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpDarkSeated->nRoomID, lpRoomData)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));


		if(!lpRoomData->m_mapPlayer.Lookup(lpDarkSeated->nUserID, pPlayer)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}
		if(lstrcmp(pPlayer->pe.szHardID,lpDarkSeated->szHardID))
		{
			response.head.nRequest = GR_HARDID_MISMATCH;
			goto SendResponse;
		}
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}
		pPlayer->pe.dwLatestTime = GetTickCount();
		pPlayer->nNetSpeed=lpDarkSeated->nNetDelay;

		if(IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ENTERHIDING))
		{
			response.head.nRequest = GR_NO_PERMISSION; //隐身网管禁止入座
			goto SendResponse;
		}
		if(pPlayer->nDeposit < lpRoomData->roomdata.nMinDeposit){  // 
			response.head.nRequest = GR_DEPOSIT_NOTENOUGH;	// 银子数不够，不能就座
			goto SendResponse;
		}	
		if(pPlayer->nStatus == PLAYER_STATUS_PLAYING 
			|| pPlayer->nStatus == PLAYER_STATUS_LOOKON ){	// 这个用户已进入游戏
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		//solo房waiting状态不能入座
		if(lpRoomData->IsSoloRoom() && pPlayer->nStatus == PLAYER_STATUS_WAITING ){
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		if(pPlayer->nStatus == PLAYER_STATUS_WAITING)// 这个用户已经开始
		{	
 			SetCommonTableLeave(lpRoomData,pPlayer);
			PLAYER_POSITION pp;
			ZeroMemory(&pp,sizeof(pp));
			pp.nUserID=pPlayer->nUserID;
			pp.nTableNO=pPlayer->nTableNO;
			pp.nChairNO=pPlayer->nChairNO;
			pp.nNetDelay=lpDarkSeated->nNetDelay;
			NotifyDarkLooker(lpRoomData, pp.nUserID,GR_PLAYER_UNSEATED, &pp, sizeof(PLAYER_POSITION));
		}
 
		pPlayer->nStatus=PLAYER_STATUS_WALKAROUND;
		pPlayer->nTableNO = -1;
		pPlayer->nChairNO = -1;
	}
	response.head.nRequest = UR_OPERATE_SUCCEEDED;

SendResponse:
	BOOL bSendOK = FALSE;
	int nData[2];
	ZeroMemory(nData,sizeof(nData));

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	if(GR_DEPOSIT_NOTENOUGH==response.head.nRequest)
	{
 		nData[0]=pPlayer->nDeposit;
		nData[1]=lpRoomData->roomdata.nMinDeposit;
		response.pDataPtr=&nData;
		response.nDataLen=sizeof(int)*2;
	}
 	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
 
	if(UR_OPERATE_SUCCEEDED == response.head.nRequest)
	{
	}
	else
	{
	
	}
 
	return TRUE;
}




BOOL CBaseSockServer::OnDarkUnseated(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	
	LPDARK_UNSEATED lpDarkUnseated = LPDARK_UNSEATED(PBYTE(lpRequest->pDataPtr));
	
	
 	REQUEST response;
	memset(&response, 0, sizeof(response));
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpDarkUnseated->nRoomID, lpRoomData)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));

		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpDarkUnseated->nUserID, pPlayer)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}
 		if(lstrcmp(pPlayer->pe.szHardID,lpDarkUnseated->szHardID))
		{
			response.head.nRequest = GR_HARDID_MISMATCH;
			goto SendResponse;
		}
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}
		pPlayer->pe.dwLatestTime = GetTickCount();

 	 
		if(pPlayer->nStatus == PLAYER_STATUS_PLAYING 
			|| pPlayer->nStatus == PLAYER_STATUS_LOOKON ){	// 这个用户已进入游戏
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		//solo房waiting状态不能换桌
		if(lpRoomData->IsSoloRoom() && pPlayer->nStatus == PLAYER_STATUS_WAITING ){
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
 
		if(pPlayer->nStatus == PLAYER_STATUS_WAITING)// 这个用户已经开始
		{	
			SetCommonTableLeave(lpRoomData,pPlayer);
			PLAYER_POSITION pp;
			ZeroMemory(&pp,sizeof(pp));
			pp.nUserID=pPlayer->nUserID;
			pp.nTableNO=pPlayer->nTableNO;
			pp.nChairNO=pPlayer->nChairNO;
			pp.nNetDelay=lpDarkUnseated->nNetDelay;
			NotifyDarkLooker(lpRoomData, pp.nUserID,GR_PLAYER_UNSEATED, &pp, sizeof(PLAYER_POSITION));
		
//			UwlLogFile(_T("===========UNSEATED_WAITING    UserID=% 8d,TableNO=% 2d,ChairNO=%d"), pp.nUserID,pp.nTableNO,pp.nChairNO );

		}
		else
		{
//			UwlLogFile(_T("===========UNSEATED_NOWAITING  UserID=% 8d"), pPlayer->nUserID);

		}
		pPlayer->nStatus = PLAYER_STATUS_WALKAROUND;
		pPlayer->nTableNO = -1;
		pPlayer->nChairNO = -1;
	}
	response.head.nRequest = UR_OPERATE_SUCCEEDED;

SendResponse:
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	if(UR_OPERATE_SUCCEEDED == response.head.nRequest){
		
	}else{
	
	}
	UwlClearRequest(&response);
	return TRUE;
}


BOOL CBaseSockServer::OnDarkFinished(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	DARK_FINISHED_OK dfo;
	memset(&dfo, 0, sizeof(dfo));
	
	LPDARK_FINISHED lpDarkFinished = LPDARK_FINISHED(PBYTE(lpRequest->pDataPtr));
	dfo.pp.nUserID=lpDarkFinished->nUserID;
	dfo.pp.nTableNO=lpDarkFinished->nTableNO;
	dfo.pp.nChairNO=lpDarkFinished->nChairNO;

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpDarkFinished->nRoomID, lpRoomData)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
	 
 		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpDarkFinished->nUserID, pPlayer)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}
		if(lpDarkFinished->nChairNO<0 || lpDarkFinished->nChairNO>=MAX_CHAIR_COUNT)
		{
			response.head.nRequest = UR_INVALID_PARAM;
			goto SendResponse;
		}
		if(lstrcmp(pPlayer->pe.szHardID,lpDarkFinished->szHardID))
		{
			response.head.nRequest = GR_HARDID_MISMATCH;
			goto SendResponse;
		}
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}
		pPlayer->pe.dwLatestTime = GetTickCount();

		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(lpDarkFinished->nTableNO, pTable)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有就座
			goto SendResponse;
		}
		if(lpDarkFinished->nUserID != pTable->nPlayerAry[lpDarkFinished->nChairNO]){	
			response.head.nRequest = UR_OPERATE_FAILED;		// 这个位置不是这个人
			goto SendResponse;
		}
		if(pPlayer->nStatus != PLAYER_STATUS_PLAYING){	// 这个用户没有进入游戏
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		if(pTable->th.nStatus != TABLE_STATUS_PLAYING){	// 游戏没有进行中
			response.head.nRequest = UR_OPERATE_FAILED;	
			goto SendResponse;
		}
		for(int i = 0; i < pTable->th.nPlayerCount; i++)
		{
 			if(lpDarkFinished->nUserID!=pTable->nPlayerAry[i])
				NotifyOnePlayer(lpRoomData, pTable->nPlayerAry[i],GR_DARK_FINISHED, &dfo, sizeof(dfo));

		}
		OverSeeUserData(pPlayer->nUserID,"OnDarkFinished::CommitTPS");
        lpRoomData->CommitTPS(pTable,PS_WALKAROUND);
 
		SetTableAsEmptyOrLeave(lpRoomData, pPlayer);
  
		GET_FINISHED_OK gfo;
		memset(&gfo, 0, sizeof(gfo));
		gfo.nUserID = lpDarkFinished->nUserID;
		gfo.nTableNO = lpDarkFinished->nTableNO;
		gfo.nChairNO = lpDarkFinished->nChairNO;
 		NotifyDarkLooker(lpRoomData,gfo.nUserID, GR_PLAYER_FINISHED, &gfo, sizeof(gfo));
		
		//////////////////////////////////////////////////////////////////////////////////////////
 	    lpRoomData->PostGetFinished(lpDarkFinished->nUserID,lpDarkFinished->nTableNO,lpDarkFinished->nChairNO);
	}
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	response.pDataPtr = &dfo;
	response.nDataLen = sizeof(dfo);

SendResponse:
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	if(UR_OPERATE_SUCCEEDED == response.head.nRequest)
	{

	}
	else
	{
	
	}
	if(IS_BIT_SET(lpDarkFinished->dwDarkFlag,FLAG_GETFINISHED_KICKOFF))
	{
		LPKICKOFF_WALKAROUND   lpKickoff=new KICKOFF_WALKAROUND;
		ZeroMemory(lpKickoff,sizeof(KICKOFF_WALKAROUND));
		lpKickoff->nAreaID = lpDarkFinished->nAreaID;
		lpKickoff->nGameID = lpDarkFinished->nGameID;
		lpKickoff->nRoomID = lpDarkFinished->nRoomID;
		lpKickoff->nPlayer = lpDarkFinished->nUserID;
		PutToServer(lpContext, GR_KICKOFF_WALKAROUND, lpKickoff, sizeof(KICKOFF_WALKAROUND));
	}
//	UwlClearRequest(&response);
	return TRUE;
}



BOOL CBaseSockServer::OnRefreshClothing(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPREFRESH_CLOTHING lpRefreshClothing = LPREFRESH_CLOTHING(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpRefreshClothing->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));

		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpRefreshClothing->nUserID, pPlayer)) 
			return TRUE;
 		if(lstrcmp(pPlayer->pe.szHardID,lpRefreshClothing->szHardID))
			return TRUE;
		if(pPlayer->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;

		if(lpRefreshClothing->nClothingID==pPlayer->nClothingID)
			return TRUE;

		pPlayer->nClothingID=lpRefreshClothing->nClothingID;

		if(lpRoomData->IsDarkRoom() || lpRoomData->IsMatchRoom())
			return TRUE;

		POINT pp;
		pp.x=lpRefreshClothing->nUserID;
		pp.y=lpRefreshClothing->nClothingID;
		NotifyRoomPlayers(lpRoomData, GR_PLAYER_CLOTHING, &pp, sizeof(pp),
 							lpContext->hSocket, lpContext->lTokenID);

		//已经开启客户端的，通知到gamesvr更新玩家会员
		if (pPlayer->nStatus == PLAYER_STATUS_PLAYING
			|| (lpRoomData->IsSoloRoom() && pPlayer->nStatus == PLAYER_STATUS_WAITING))
		{
			lpRoomData->PostUserClothing(pPlayer->nUserID, pPlayer->nClothingID);
		}
		

   		return TRUE;
	}
} 



BOOL CBaseSockServer::OnAskEnterMatch(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
  	REQUEST response;
	memset(&response, 0, sizeof(response));
 	TCHAR szError[256];
	ZeroMemory(szError,sizeof(szError));

 
 	LPASK_ENTERMATCH lpAskEnterMatch = LPASK_ENTERMATCH(PBYTE(lpRequest->pDataPtr));
	LPPLAYER pPlayer = NULL;
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpAskEnterMatch->nRoomID, lpRoomData)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		if(! lpRoomData->IsMatchRoom())
		{
			response.head.nRequest =GR_NO_PERMISSION;	 
			goto SendResponse;
		}

		if(!lpRoomData->m_mapPlayer.Lookup(lpAskEnterMatch->nUserID, pPlayer)){
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}
		if(lstrcmp(pPlayer->pe.szHardID,lpAskEnterMatch->szHardID))
		{
			response.head.nRequest = GR_HARDID_MISMATCH;
			goto SendResponse;
		}
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}

		if(lpRoomData->m_pTcMatch==NULL)
		{
			response.head.nRequest = GR_MATCH_NOTREADY;
			goto SendResponse;
		}

		pPlayer->pe.dwLatestTime = GetTickCount();
		pPlayer->nNetSpeed=lpAskEnterMatch->nNetDelay;

 //		lpRoomData->PostHardAndToken(lpAskEnterMatch->nUserID,lpAskEnterMatch->szHardID,pPlayer->pe.lTokenID);
		////////////////////////////////////////////////////////////////////////////
		/*
		if(pPlayer->nDeposit < lpRoomData->roomdata.nMinDeposit){  // 
			response.head.nRequest = GR_DEPOSIT_NOTENOUGH;	// 银子数不够，不能就座
			goto SendResponse;
		}	

		if(pPlayer->nScore < lpRoomData->roomdata.nMinPlayScore){  // 积分不够
			response.head.nRequest = GR_PLAYSCORE_NOTENOUGH;	//  
 			goto SendResponse;
		}
		if(pPlayer->nScore > lpRoomData->roomdata.nMaxPlayScore){  // 积分超出
			response.head.nRequest = GR_PLAYSCORE_OVERFLOW;	//  
 			goto SendResponse;
		}
  
		if(pPlayer->nStatus == PLAYER_STATUS_PLAYING 
			|| pPlayer->nStatus == PLAYER_STATUS_LOOKON ){	// 这个用户已进入游戏
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		*/
		///////////////////////////////////////////////////////////////////////////
 
	}
	response.head.nRequest = UR_OPERATE_SUCCEEDED;

SendResponse:
	BOOL bSendOK = FALSE;
	int nData[2];
	ZeroMemory(nData,sizeof(nData));

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
 
	if(GR_DEPOSIT_NOTENOUGH==response.head.nRequest)
	{
		nData[0]=pPlayer->nDeposit;
		nData[1]=lpRoomData->roomdata.nMinDeposit;
		response.pDataPtr=&nData;
		response.nDataLen=sizeof(int)*2;
	}
 	if(GR_ERROR_INFOMATION==response.head.nRequest)
	{
	 	response.pDataPtr=szError;
	 	response.nDataLen=lstrlen(szError)+1;
	}
	if(GR_PLAYSCORE_NOTENOUGH==response.head.nRequest)
	{   
		nData[0]=pPlayer->nScore;
		nData[1]=lpRoomData->roomdata.nMinPlayScore;
 	 	response.pDataPtr=&nData;
	 	response.nDataLen=sizeof(int)*2;
	}
	if(GR_PLAYSCORE_OVERFLOW==response.head.nRequest)
	{   
		nData[0]=pPlayer->nScore;
		nData[1]=lpRoomData->roomdata.nMaxPlayScore;
 	 	response.pDataPtr=&nData;
	 	response.nDataLen=sizeof(int)*2;
	}
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	if(UR_OPERATE_SUCCEEDED == response.head.nRequest){
	}else{
	
	}
//	UwlClearRequest(&response);
	return TRUE;
}

BOOL CBaseSockServer::OnMatchLeaveRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPLEAVE_ROOM lpLeaveRoom = LPLEAVE_ROOM(PBYTE(lpRequest->pDataPtr));
	LONG token =  lpContext->lTokenID;
	
	
	RemoveTokenRoomSock(token);   
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpLeaveRoom->nRoomID, lpRoomData)){
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		USER_SOCK us;
		ZeroMemory(&us,sizeof(USER_SOCK));
		
		LPPLAYER pPlayer = NULL;
		lpRoomData->m_mapTokenUserSock.Lookup(token, us);
		
		if( lpLeaveRoom->nUserID == us.nUserID  )
		{		
			lpRoomData->m_mapUserSock.RemoveKey(us.nUserID);
			lpRoomData->m_mapTokenUserSock.RemoveKey(us.token);
			
			if(lpRoomData->m_mapPlayer.Lookup(us.nUserID, pPlayer) && pPlayer)
			{
				BOOL bIsDarkRoom=lpRoomData->IsDarkRoom();
				BOOL bLookDark=IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_LOOKDARK);
                if(bIsDarkRoom && !bLookDark)
					return TRUE;
				////////////////////
				if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
				{	
					UwlLogFile(_T("===========LeaveRoom tokenid failed.userid=%d"), lpLeaveRoom->nUserID);
					return TRUE;
				}
				
				if(lstrcmp(pPlayer->pe.szHardID,lpLeaveRoom->szHardID))//硬件匹配
				{
					UwlLogFile(_T("===========LeaveRoom hardid failed.userid=%d,hardid=%s"), lpLeaveRoom->nUserID,lpLeaveRoom->szHardID);
					return TRUE;
				}

				if(pPlayer->nStatus == PLAYER_STATUS_PLAYING ) //游戏开始之后,不允许离开房间
				{
					int nInterval=UwlGetCurrentSeconds()-pPlayer->pe.nStartupTime;
					UwlLogFile(_T("===========LeaveRoom failed.GameID=%d,RoomID=%d,UserID=%d,interval seconds=%d"),lpLeaveRoom->nGameID,lpLeaveRoom->nRoomID,lpLeaveRoom->nUserID,nInterval);
					return TRUE;
				}

				if (lpRoomData->m_pTcMatch)
				{
					CTcMatchPlayer* pTcPlayer=lpRoomData->m_pTcMatch->GetPlayer(pPlayer->nUserID);
					if (pTcPlayer
						&&lpRoomData->m_pTcMatch->IsMatchStart()
						&&!lpRoomData->m_pTcMatch->IsMatchEnd()
						&&!pTcPlayer->IsQuit())
					{
						UwlLogFile(_T("===========LeaveRoom failed,match is not finished.GameID=%d,RoomID=%d,UserID=%d"),lpLeaveRoom->nGameID,lpLeaveRoom->nRoomID,lpLeaveRoom->nUserID);
						return TRUE;
					}
				}

				RemoveOnePlayer(lpRoomData,pPlayer,0,0);
		

				PLAYER_OUTROOM po;
				ZeroMemory(&po,sizeof(po));
				po.nUserID=lpLeaveRoom->nUserID;
				po.nAreaID=lpLeaveRoom->nAreaID; 
				po.nGameID=lpLeaveRoom->nGameID; 
				po.nRoomID=lpLeaveRoom->nRoomID; 
				po.nRoomSvrID=g_nClientID;
				po.nHostID=g_nHostID;
 				po.nGamePort=lpRoomData->roomdata.nGamePort;

				NotifyServer(GR_PLAYER_OUTROOM,&po,sizeof(PLAYER_OUTROOM));
				
	
				
			}
			
		}
	}
  	return TRUE;
}



BOOL CBaseSockServer::OnKickoffByExeVer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	LPKICKOFF_BYEXEVER lpKickoffByExeVer = LPKICKOFF_BYEXEVER(PBYTE(lpRequest->pDataPtr));
  
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpKickoffByExeVer->nRoomID, lpRoomData))
 			return FALSE;
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 	/////////////////
		LPPLAYER pAdmin = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpKickoffByExeVer->nAdminID, pAdmin)) 
			return TRUE;
 		if(lstrcmp(pAdmin->pe.szHardID,lpKickoffByExeVer->szHardID))
			return TRUE;
		if(pAdmin->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		pAdmin->pe.dwLatestTime = GetTickCount();

		if(!IS_BIT_SET(pAdmin->nUserType ,USER_TYPE_ADMIN))//不是网管
			return TRUE;
		if(!IS_BIT_SET(pAdmin->nUserType ,USER_TYPE_KICKOFF))//不是踢人网管
			return FALSE;
	////////////////////////////////////////
	    memset(lpKickoffByExeVer->szHardID,0,sizeof(lpKickoffByExeVer->szHardID));
  		NotifyRoomPlayers(lpRoomData, GR_KICKOFF_BYEXEVER, lpKickoffByExeVer, lpRequest->nDataLen);
		return TRUE;
	}
}

BOOL CBaseSockServer::OnMatchAdmin(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	CBaseRoomData* lpRoomData = NULL;

	REQUEST response;
	memset(&response, 0, sizeof(response));
 
   	LPMATCH_ADMIN lpMatchAttach = LPMATCH_ADMIN(PBYTE(lpRequest->pDataPtr));
    lpMatchAttach->dwIPAddr = GetClientAddress(lpContext->hSocket, lpContext->lTokenID);	
		
 	/////////////////////////////////////////////////////////////////

	{
		CAutoLock lock(&g_csRoomData);
 		g_mapRoomDataEx.Lookup(lpMatchAttach->nRoomID, lpRoomData);
	}
	if(lpRoomData)
	{
 		CAutoLock lock(&(lpRoomData->m_csRoomData));
		/////////////////
		if (!lpRoomData->IsMatchRoom())
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}

		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpMatchAttach->nAdminID, pPlayer)) 
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}
 
		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}
		if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ADMIN))//不是网管
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
		if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_MATCHADMIN))//不是比赛管理员
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
		////////////////////////////////////////
	}
  	TransmitRequest(lpContext, lpRequest);
	return TRUE;
SendResponse:
	BOOL bSendOK = FALSE;
	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
	UwlClearRequest(&response);
	return TRUE;
 
}

BOOL CBaseSockServer::OnCommitClientInfo(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	LPCOMMIT_CLIENTINFO lpcc = LPCOMMIT_CLIENTINFO(PBYTE(lpRequest->pDataPtr));
	int nUserID=lpcc->nUserID;

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpcc->nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	if(lpRoomData)
	{
 		CAutoLock lock(&(lpRoomData->m_csRoomData));
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup( nUserID, pPlayer)) 
			return TRUE;
 		if(lstrcmp(pPlayer->pe.szHardID,lpcc->szHardID))
			return TRUE;
 		if(pPlayer->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
 	
		CClientInfo* pClientInfo=NULL;
		lpRoomData->m_mapClientInfo.Lookup( nUserID,pClientInfo);
		if(pClientInfo==NULL)
		{
 			pClientInfo=new CClientInfo;
			lpRoomData->m_mapClientInfo.SetAt(nUserID,pClientInfo);
		}
		pClientInfo->ZeroClientInfo();
		pClientInfo->FillClientInfo(lpcc);
	
		return TRUE;
	}
	return FALSE;
}

BOOL CBaseSockServer::OnClearProxyIPs(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	LPCLEAR_PROXYIPS lpcp = LPCLEAR_PROXYIPS(PBYTE(lpRequest->pDataPtr));

	int nCount=0;
	{
		CAutoLock lock(&g_csProxyIP);
	    nCount=g_mapProxyIP.GetCount();
		g_mapProxyIP.RemoveAll();
	}
   	UwlLogFile(_T("===========Clear ProxyIPs,AdminID=%d,mapCount=%d "),lpcp->nAdminID,nCount);     

	response.head.nRequest = UR_OPERATE_SUCCEEDED;

	BOOL bSendOK = FALSE;
 	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

 	return TRUE;
}


BOOL CBaseSockServer::OnSaveProxyIPs(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	LPSAVE_PROXYIPS lpsp = LPSAVE_PROXYIPS(PBYTE(lpRequest->pDataPtr));

   	TCHAR szIP[MAX_SERVERIP_LEN];
	memset(szIP,0,sizeof(szIP));
	CString sTmp,sContent;
	int nCount=0;
 	int nKey,nValue;
	{
		CAutoLock lock(&g_csProxyIP);
  		POSITION pos=g_mapProxyIP.GetStartPosition();
 		while (pos)	
		{
  	 		g_mapProxyIP.GetNextAssoc(pos, nKey, nValue);
			UwlAddrToName(nKey,szIP);
			sTmp.Format(_T("%s=%d\r\n"),szIP,nValue);
			sContent+= sTmp ;
			nCount++;
		}
	}
  
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
 	BOOL bSendOK = FALSE;
 	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
 	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
 	UwlLogFile(_T("===========Save ProxyIPs,AdminID=%d,mapCount=%d "),lpsp->nAdminID,nCount);     
 
   	TCHAR szFullName[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), szFullName, sizeof(szFullName));
 	UwlSplitPath(szFullName, SPLIT_DRIVE_DIR, szFullName);
 	CString sFile;
    sFile.Format(_T("%sproxyips.log"), szFullName);
  
  	HANDLE hFile=CreateFile(sFile, 
					  GENERIC_WRITE|GENERIC_READ,
		              FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
					  OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
 		DWORD dwWrote;
		WriteFile(hFile,(LPCVOID)(LPCTSTR)sContent,sContent.GetLength(),&dwWrote,NULL);
 	    SetEndOfFile(hFile);
  		CloseHandle(hFile);
	}
  
 	return TRUE;
}

BOOL CBaseSockServer::MB_OnSendPulse(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{   
 	if(lpRequest->nDataLen>0)
	{
		return OnRoomUserPulse(lpContext,lpRequest,pThreadCxt);
	}
 	return TRUE;
}

BOOL CBaseSockServer::MB_OnEnterRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{   
	CBaseRoomData* lpRoomData = NULL;
	LPPLAYER pPlayer = NULL;
 	
 	BOOL  bCheckTrustIP=FALSE;
	CString sGameIniFile,sCause;

	REQUEST response;
	memset(&response, 0, sizeof(response));
 
	TCHAR szError[512];
	memset(szError,0,sizeof(szError));
 	
	int nClientPort=0;
 	int nServerPort=0;
   	LPENTER_ROOM lpEnterRoom = LPENTER_ROOM(PBYTE(lpRequest->pDataPtr));

 
	int nValue=0;
  	DWORD dwProxyIP = GetClientAddress(lpContext->hSocket, lpContext->lTokenID);

	if(!g_mapLocalIP.Lookup(dwProxyIP,nValue))
	{
  		response.head.nRequest = GR_NO_PERMISSION;//RoomMP必须和RoomSvr同一台服务器
		goto SendResponse;
	}
 
 	if(lpRequest->nDataLen != sizeof(ENTER_ROOM))
	{
		response.head.nRequest = UR_INVALID_PARAM;
		goto SendResponse;
	}

    if(lstrlen(lpEnterRoom->szMachineID)==0 || lstrlen(lpEnterRoom->szHardID)==0  || lstrlen(lpEnterRoom->szVolumeID)==0)
	{
		response.head.nRequest = UR_INVALID_PARAM;
		goto SendResponse;
	}

	lpEnterRoom->dwEnterFlags |=FLAG_ENTERROOM_HANDPHONE;//手机进入标志
 	lpEnterRoom->nEnterTime=UwlGetCurrentSeconds();
	lpEnterRoom->nRoomSvrID=g_nClientID;
	lpEnterRoom->nHostID=g_nHostID;
//  lpEnterRoom->dwIPAddr  手机端IP由RoomMP代理获取;

 	UwlGetPeerPort(lpContext->hSocket,nClientPort);
	lpEnterRoom->dwClientPort=MAKELONG(lpEnterRoom->dwClientPort,nClientPort );

 	UwlGetSockPort(lpContext->hSocket,nServerPort);
	lpEnterRoom->dwServerPort=MAKELONG(lpEnterRoom->dwServerPort,nServerPort );

	TCHAR szIP[MAX_SERVERIP_LEN];
 	ZeroMemory(szIP,sizeof(szIP));
  	UwlAddrToName(lpEnterRoom->dwIPAddr,szIP);

 	/////////////////////////////////////////
  	if(IsForbidIP4(szIP,sCause))
	{
		sprintf(szError,_T("您的IP(%s)被禁止，原因：\r\n\r\n%s\r\n\r\n详情请与管理员联系。"),szIP,sCause);
		response.head.nRequest = GR_ROOM_FORBID_IP;
		goto SendResponse;
	}
 
 	if(IsForbidIP3(szIP,sCause))
	{
		sprintf(szError,_T("您的IP(%s)前3位被禁止，原因：\r\n\r\n%s\r\n\r\n详情请与管理员联系。"),szIP,sCause);
 		response.head.nRequest = GR_ROOM_FORBID_IP;
		goto SendResponse;
	}
	if(IsForbidIP2(szIP,sCause))
	{
		sprintf(szError,_T("您的IP(%s)前2位被禁止，原因：\r\n\r\n%s\r\n\r\n详情请与管理员联系。"),szIP,sCause);
 		response.head.nRequest = GR_ROOM_FORBID_IP;
		goto SendResponse;
	}
	
	{
		int nLatestNobuffer=InterlockedExchangeAdd(&g_lNoBufferLatest, 0);
		int nBusyWaitSecond=GetBusyWaitSecond() ;
		int nInterval=UwlGetCurrentSeconds()-nLatestNobuffer;
		if(nInterval>=0 && nInterval<= nBusyWaitSecond)//2分钟之内
		{
			response.head.nRequest = GR_SERVICE_BUSY;
			goto SendResponse;
		}
	}

 
	/////////////////////////////////////////////////////////////////
	{
		CAutoLock lock(&g_csRoomData);
 		g_mapRoomDataEx.Lookup(lpEnterRoom->nRoomID, lpRoomData);
	}
	if(lpRoomData)
	{
 		CAutoLock lock(&(lpRoomData->m_csRoomData));

  		if(lpRoomData->m_mapPlayer.Lookup(lpEnterRoom->nUserID, pPlayer))
		{
			if(pPlayer->nStatus==PLAYER_STATUS_PLAYING)
				lpEnterRoom->dwEnterFlags|=FLAG_ENTERROOM_DXXW;
		}
	 
		bCheckTrustIP=(lpRoomData->IsOldMatchRoom() && !IS_BIT_SET(lpEnterRoom->dwEnterFlags,FLAG_ENTERROOM_INTER));//排行赛房间需要检查比赛是否限定IP（网吧比赛）
		if(bCheckTrustIP)
			sGameIniFile=lpRoomData->GetGameSvrIniFile();
	}


	if(bCheckTrustIP)
	{
		if(IsSomeValueInFile(_T("IPLimited"),xyConvertIntToStr(lpEnterRoom->nRoomID),sCause,sGameIniFile))
		{	
			CString sWhere;
			if(!IsSomeValueInFile(_T("TrustIP"),szIP,sWhere,sGameIniFile))
			{
				lstrcpyn(szError,sCause,256);
 				response.head.nRequest = GR_ERROR_INFOMATION;
				goto SendResponse;
			}
		}
	}

  
	////////////////////////////////////////////////////////////////
	TransmitRequest(lpContext, lpRequest);
 	return TRUE;

SendResponse:
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
    if(GR_ERROR_INFOMATION==response.head.nRequest || GR_ROOM_FORBID_IP ==response.head.nRequest )
	{
	 	response.pDataPtr=szError;
	 	response.nDataLen=lstrlen(szError)+1;
	}
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	if(UR_OPERATE_SUCCEEDED == response.head.nRequest)
	{
	}
	else
	{
	}
 	return TRUE;

}



BOOL CBaseSockServer::MB_OnLeaveRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
    return OnLeaveRoom(lpContext,lpRequest,pThreadCxt);
 
}
BOOL CBaseSockServer::MB_OnGetFinished(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	return OnGetFinished(lpContext, lpRequest, pThreadCxt);
}



BOOL CBaseSockServer::OnAskEnterGame(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	TCHAR szError[256];
	ZeroMemory(szError,sizeof(szError));
	int nPlayerScore=0;
	int nPlayerDeposit=0;
	int nTableNO = 0;
	int nChairNO = 0;	

 	DWORD dwIPAddr = GetClientAddress(lpContext->hSocket, lpContext->lTokenID);	
	LPASK_ENTERGAME lpAskEnterGame = LPASK_ENTERGAME(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
 		if(!g_mapRoomDataEx.Lookup(lpAskEnterGame->nRoomID, lpRoomData))
		{	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
 			goto SendResponse;
		}
	}

	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 
		if (!lpRoomData->IsSoloRoom() || !lpRoomData->IsCloakingRoom() )
		{
			response.head.nRequest = GR_NO_PERMISSION;
 			goto SendResponse;
		}
 
		LPPLAYER pPlayer = NULL;
 		if(!lpRoomData->m_mapPlayer.Lookup(lpAskEnterGame->nUserID, pPlayer))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}

		nPlayerScore=pPlayer->nScore ;
		nPlayerDeposit=pPlayer->nDeposit ;

		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}
  		pPlayer->pe.dwLatestTime = GetTickCount();
		pPlayer->nNetSpeed=lpAskEnterGame->nNetDelay;

		if(pPlayer->nStatus != PLAYER_STATUS_WALKAROUND  )
		{	 
			response.head.nRequest = UR_OBJECT_EXIST;		
			goto SendResponse;
		}

     	if(IS_BIT_SET(pPlayer->pe.dwEnterFlags ,FLAG_ENTERROOM_LOOKDARK))
		{
			response.head.nRequest = GR_NO_PERMISSION; //网管禁止进入
			goto SendResponse;
		}
		if(IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ENTERHIDING))
		{
			response.head.nRequest = GR_NO_PERMISSION; //隐身网管禁止进入
			goto SendResponse;
		}

 		if(pPlayer->nDeposit < lpRoomData->roomdata.nMinDeposit){  // 
			response.head.nRequest = GR_DEPOSIT_NOTENOUGH;	// 银子数不够，不能进入
			goto SendResponse;
		}	
		if(pPlayer->nScore < lpRoomData->roomdata.nMinPlayScore){  // 积分不够
			response.head.nRequest = GR_PLAYSCORE_NOTENOUGH;	//  
 			goto SendResponse;
		}
		if(pPlayer->nScore > lpRoomData->roomdata.nMaxPlayScore){  // 积分超出
			response.head.nRequest = GR_PLAYSCORE_OVERFLOW;	//  
 			goto SendResponse;
		}
		if(IS_BIT_SET(lpRoomData->roomdata.dwOptions,ROOM_OPT_FORBIDPROXYIP))//禁止代理
		{
 			CClientInfo* pci=NULL;
			lpRoomData->m_mapClientInfo.Lookup(lpAskEnterGame->nUserID,pci);
			if(pci!=NULL)
			{
   				BOOL b1=IS_BIT_SET(pci->cci.dwFlags,FLAG_CLIENTINFO_CLTISRMT);
				if(b1)
				{
					response.head.nRequest = GR_FORBID_PROXYIP;	//  
 					goto SendResponse;
				}
			}
		}
 		if(IS_BIT_SET(lpRoomData->roomdata.dwOptions,ROOM_OPT_FORBIDVPNPORT))//禁止vpn端口
		{
  			if(IsProxyIP(dwIPAddr))
			{
				pPlayer->pe.dwClientFlags |=FLAG_CLIENTINFO_CLTISVPN;
				response.head.nRequest = GR_FORBID_PROXYIP;	//  
 				goto SendResponse;
			}
		}
 		///////////2012-04-13/////////////////////
		if(lpRoomData->IsOldMatchRoom())//排行赛房间需要检查比赛时间和局数限制,从游戏配置ini文件里读取数据
		{ 	
			CString sIniFile=lpRoomData->GetGameSvrIniFile();
		
			int nMaxUserBout=GetPrivateProfileInt(_T("MaxUserBout"),xyConvertIntToStr(lpAskEnterGame->nRoomID ),0, sIniFile);
			if(nMaxUserBout>0 && pPlayer->nBout >= nMaxUserBout) // 局数超出
			{ 
				response.head.nRequest = GR_ERROR_INFOMATION;	//  
				sprintf(szError,_T("对不起，您已经打满%d局,不能继续。"),nMaxUserBout);
 				goto SendResponse;
			}


 			OPENTIME_SECTION ots;
			memset(&ots,0,sizeof(ots));

			if(!lpRoomData->GetOpenTimeSection(sIniFile,&ots))
			{
			 	CString sCause=lpRoomData->GetOutOfOpenTimeCause(sIniFile,&ots);
				response.head.nRequest = GR_ERROR_INFOMATION;	//  
				lstrcpyn(szError,sCause,256);
 				goto SendResponse;
			} 
 
		}
 
 	    if(!lpRoomData->GetFreeChairNO2 (nTableNO, nChairNO ))
		{
 			response.head.nRequest = GR_NO_CHAIRS;	//连空座位都没有
 			goto SendResponse;
		}
		////////////////////////////////////////////////////////////////////////////////////

		lpRoomData->PostRoomData();
 		lpRoomData->PostHardAndToken(lpAskEnterGame->nUserID,pPlayer->pe.szHardID,pPlayer->pe.lTokenID);
		lpRoomData->PostRoomTableChair(lpAskEnterGame->nUserID,nTableNO,nChairNO);

 		LPTABLE lpTable = NULL;
		lpRoomData->m_mapTable.Lookup(nTableNO, lpTable);
  		lpTable->nPlayerAry[nChairNO] = lpAskEnterGame->nUserID;
		lpTable->th.nPlayerCount++;
	
    	pPlayer->nStatus = PLAYER_STATUS_SEATED;
		pPlayer->nTableNO = nTableNO;
		pPlayer->nChairNO = nChairNO;
 		pPlayer->pe.nStartupTime=UwlGetCurrentSeconds();

		PLAYER_POSITION PP;
		ZeroMemory(&PP,sizeof(PP));
		PP.nUserID=lpAskEnterGame->nUserID;
		PP.nNetDelay=lpAskEnterGame->nNetDelay;
		PP.nTableNO=nTableNO;
		PP.nChairNO=nChairNO;
 		NotifyDarkLooker(lpRoomData,PP.nUserID, GR_PLAYER_ASKENTERGAME, &PP, sizeof(PLAYER_POSITION));
 
		response.pDataPtr = &PP;
		response.nDataLen = sizeof(PP);
		response.head.nRequest = UR_OPERATE_SUCCEEDED;
	}
 
SendResponse:
	BOOL bSendOK = FALSE;
	int nData[3];
	ZeroMemory(nData,sizeof(nData));

 	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
 
	if(GR_ERROR_INFOMATION==response.head.nRequest)
	{
	 	response.pDataPtr=szError;
	 	response.nDataLen=lstrlen(szError)+1;
	}
	if(GR_DEPOSIT_NOTENOUGH==response.head.nRequest)
	{
		nData[0]=nPlayerDeposit;
		nData[1]=lpRoomData->roomdata.nMinDeposit;
		response.pDataPtr=&nData;
		response.nDataLen=sizeof(int)*2;
	}
	if(GR_PLAYSCORE_NOTENOUGH==response.head.nRequest)
	{   
		nData[0]=nPlayerScore;
		nData[1]=lpRoomData->roomdata.nMinPlayScore;
 	 	response.pDataPtr=&nData;
	 	response.nDataLen=sizeof(int)*2;
	}
	if(GR_PLAYSCORE_OVERFLOW==response.head.nRequest)
	{   
		nData[0]=nPlayerScore;
		nData[1]=lpRoomData->roomdata.nMaxPlayScore;
 	 	response.pDataPtr=&nData;
	 	response.nDataLen=sizeof(int)*2;
	}
	if(GR_FORBID_PROXYIP==response.head.nRequest)
	{   
   	 	response.pDataPtr=&dwIPAddr;
	 	response.nDataLen=sizeof(int);
	}
    bSendOK = SendResponse(context.hSocket, &context, &response);
 	return TRUE;

}

BOOL CBaseSockServer::MB_OnAskEnterGame(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
    return OnAskEnterGame(lpContext,lpRequest,pThreadCxt);
	
}

BOOL CBaseSockServer::MB_OnRefreshMember(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
    return OnRefreshMember(lpContext,lpRequest,pThreadCxt);
}

BOOL CBaseSockServer::OnRefreshMember(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPREFRESH_MEMBER lpRefreshMember = LPREFRESH_MEMBER(PBYTE(lpRequest->pDataPtr));
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		
		if(!g_mapRoomDataEx.Lookup(lpRefreshMember->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpRefreshMember->nUserID, pPlayer)) 
			return TRUE;
		if(lstrcmp(pPlayer->pe.szHardID,lpRefreshMember->szHardID))
			return TRUE;
		if(pPlayer->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		if(lpRoomData->IsDarkRoom() || lpRoomData->IsMatchRoom())
			return TRUE;
		
		BOOL bMember = IS_BIT_SET(lpRefreshMember->nMemberType, USER_TYPE_MEMBER);
		if(bMember == IS_BIT_SET( pPlayer->nUserType,USER_TYPE_MEMBER ))
			return TRUE;
		
		if (bMember)
			pPlayer->nUserType |= USER_TYPE_MEMBER;
		else
			pPlayer->nUserType &=~USER_TYPE_MEMBER;
		
		POINT pp;
		pp.x=lpRefreshMember->nUserID;
		pp.y=lpRefreshMember->nMemberType;
		NotifyRoomPlayers(lpRoomData, GR_PLAYER_MEMBER, &pp, sizeof(pp),
			lpContext->hSocket, lpContext->lTokenID);

		//已经开启客户端的，通知到gamesvr更新玩家会员
		if (bMember && (pPlayer->nStatus == PLAYER_STATUS_PLAYING
			|| (lpRoomData->IsSoloRoom() && pPlayer->nStatus == PLAYER_STATUS_WAITING)))
		{
			lpRoomData->PostBecomeMember(pPlayer->nUserID, pPlayer->nUserType);
		}
		
		return TRUE;
	}
} 

BOOL CBaseSockServer::OnAdminMsgToRoomSvr(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPSYSTEM_MSG lpSystemMsg = LPSYSTEM_MSG(PBYTE(lpRequest->pDataPtr));
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		
		if(!g_mapRoomDataEx.Lookup(lpSystemMsg->nRoomID, lpRoomData))
			return FALSE;
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		/////////////////
		LPPLAYER pAdmin = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpSystemMsg->nMsgFrom, pAdmin)) 
			return TRUE;
		if(pAdmin->pe.lTokenID!=lpContext->lTokenID)
			return TRUE;
		pAdmin->pe.dwLatestTime = GetTickCount();
		
		if(!IS_BIT_SET(pAdmin->nUserType ,USER_TYPE_ADMIN))//不是网管
			return TRUE;

		if(!IS_BIT_SET(pAdmin->nUserType ,USER_TYPE_SYSADMIN))//不是系统管理员
			return TRUE;
	}


	int roomid = 0;
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
			NotifyRoomPlayers(lpRoomData, GR_ADMINMSG_TO_ROOM, lpSystemMsg, lpRequest->nDataLen);
		}
	}
	
	////////////////////////////////////////
	
	
	return TRUE;
}

BOOL CBaseSockServer::MB_OnGetGameVersion(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPGET_GAMEVERISON lpGetGameVer = LPGET_GAMEVERISON(PBYTE(lpRequest->pDataPtr));

	if (!lpGetGameVer || (lpRequest->nDataLen!=sizeof(GET_GAMEVERISON)))
		return TRUE;
	
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	GET_GAMEVERISON_OK ggo;
	ZeroMemory(&ggo, sizeof(ggo));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		
		if(!g_mapRoomDataEx.Lookup(lpGetGameVer->nRoomID, lpRoomData)){
			return TRUE;
		}
	}

	if (lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 
		if (lpRoomData->roomdata.nGameID != lpGetGameVer->nGameID)
		{
			response.head.nRequest = UR_INVALID_PARAM;
 			goto SendResponse;
		}

		CString sIniFile=lpRoomData->GetGameSvrIniFile();
		TCHAR szVersion[MAX_VERSION_LEN];
		ZeroMemory(szVersion,sizeof(szVersion));
		if (IS_BIT_SET(lpGetGameVer->dwGetFlags, FLAG_CLIENTINFO_IOSPHONE))
		{
			GetPrivateProfileString (_T("clientversion"),_T("IPHONE"), _T(""),szVersion, MAX_VERSION_LEN,sIniFile) ; 
		}
		else if (IS_BIT_SET(lpGetGameVer->dwGetFlags, FLAG_CLIENTINFO_IOSPAD))
		{
			GetPrivateProfileString (_T("clientversion"),_T("IOSPAD"), _T(""),szVersion, MAX_VERSION_LEN,sIniFile) ; 
		}
		else
		{
			GetPrivateProfileString (_T("clientversion"),_T("AND"), _T(""),szVersion, MAX_VERSION_LEN,sIniFile) ; 
		}

		
		if (lstrlen(szVersion))
		{	
			ggo.nGameID = lpGetGameVer->nGameID;

			TCHAR *fields[4];
			memset(fields,0,sizeof(fields));
			TCHAR *p1, *p2;
			p1=szVersion;
			if (xyRetrieveFields2 ( p1, fields, 4, &p2,'.' )>=3)
			{
				ggo.nMajorVer= atoi(fields[0]);
				ggo.nMinorVer= atoi(fields[1]);
				ggo.nBuildNO = atoi(fields[2]);
			}

			response.pDataPtr = &ggo;
			response.nDataLen = sizeof(ggo);
			response.head.nRequest = UR_OPERATE_SUCCEEDED;
		}
		else
		{
			response.head.nRequest = UR_OPERATE_FAILED;
 			goto SendResponse;
		}
	}
 
SendResponse:

	BOOL bSendOK = FALSE;
 	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
    bSendOK = SendResponse(context.hSocket, &context, &response);
 	return TRUE;
} 
BOOL CBaseSockServer::MB_OnNewPrivateRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	DWORD dwIPAddr = 0;
	LPNEW_PRIVATEROOM lpNewPrivateRoom= LPNEW_PRIVATEROOM(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
 		if(!g_mapRoomDataEx.Lookup(lpNewPrivateRoom->nRoomID, lpRoomData))
		{	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
 			goto SendResponse;
		}
	}

	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		// solo房 私有包房
		if (!lpRoomData->IsSoloRoom() || !lpRoomData->IsPrivateRoom())
		{
			response.head.nRequest = GR_NO_PERMISSION;
 			goto SendResponse;
		}
 
		LPPLAYER pPlayer = NULL;
 		if(!lpRoomData->m_mapPlayer.Lookup(lpNewPrivateRoom->nUserID, pPlayer))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}

		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}

		if(!IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_HANDPHONE) || !lpRoomData->IsSupportMobile() ) //不支持手机
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
		if(pPlayer->nStatus != PLAYER_STATUS_WALKAROUND)
		{	 
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}

		dwIPAddr = pPlayer->pe.dwIPAddr;

		pPlayer->pe.dwLatestTime = GetTickCount();
		pPlayer->nNetSpeed=lpNewPrivateRoom->nNetDelay;

		if(IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ENTERHIDING))
		{
			response.head.nRequest = GR_NO_PERMISSION; //隐身网管禁止入座
			goto SendResponse;
		}
 		if(pPlayer->nDeposit < lpRoomData->roomdata.nMinDeposit){  // 
			response.head.nRequest = GR_DEPOSIT_NOTENOUGH;	// 银子数不够，不能就座
			goto SendResponse;
		}	
		if(pPlayer->nScore < lpRoomData->roomdata.nMinPlayScore){  // 积分不够
			response.head.nRequest = GR_PLAYSCORE_NOTENOUGH;	//  
 			goto SendResponse;
		}
		if(pPlayer->nScore > lpRoomData->roomdata.nMaxPlayScore){  // 积分超出
			response.head.nRequest = GR_PLAYSCORE_OVERFLOW;	//  
 			goto SendResponse;
		}

		lpRoomData->PostRoomData();
 		lpRoomData->PostHardAndToken(lpNewPrivateRoom->nUserID,pPlayer->pe.szHardID,pPlayer->pe.lTokenID);

		float fBreakOff = 0;
		int bout = pPlayer->nWin + pPlayer->nLoss + pPlayer->nStandOff;
		if(bout)
			fBreakOff = (float)pPlayer->nBreakOff * 100 / bout;
		int nNetSpeed = pPlayer->nNetSpeed;	

		////////////////////////////////////////////////////////////////////////////////////
 		LPTABLE lpTable = NULL;
		int nTableNo = 0;
		int nChairNo = 0;
	
		nTableNo = lpRoomData->GetFreeTableNOEx(0);

		if (nTableNo<0 || !lpRoomData->m_mapTable.Lookup(nTableNo, lpTable))
		{
 			response.head.nRequest = GR_NO_EXTRA_TABLE;	//连空座位都没有
 			goto SendResponse;
		}

		lpTable->dwIPAddrs[nChairNo] = dwIPAddr;
		lpTable->fBreakOffs[nChairNo] = fBreakOff;
		lpTable->nNetSpeeds[nChairNo] = nNetSpeed;

		lpTable->nIPConfigs[nChairNo] =   lpNewPrivateRoom->nIPConfig;
		lpTable->nBreakReqs[nChairNo] =   lpNewPrivateRoom->nBreakReq;
		lpTable->nSpeedReqs[nChairNo] =   lpNewPrivateRoom->nSpeedReq;
		lpTable->nMinScores[nChairNo] =   lpNewPrivateRoom->nMinScore;
		lpTable->nMinDeposits[nChairNo] = lpNewPrivateRoom->nMinDeposit;

		lpTable->nPlayerAry[nChairNo] = lpNewPrivateRoom->nUserID;
		lpTable->th.nPlayerCount++;
	
		//Room table chair
		lpRoomData->PostRoomTableChair(lpNewPrivateRoom->nUserID,nTableNo,nChairNo);

		if(lpTable->th.nPlayerCount==1) //第一个入座
		{
			lpTable->th.nFirstSeatedPlayer=lpNewPrivateRoom->nUserID ;
			lpTable->nHomeUserID = lpNewPrivateRoom->nUserID;
//			lpRoomData->PostHomeUserID(nTableNo,lpTable->nHomeUserID);
		}

		if (pPlayer->nStatus == PLAYER_STATUS_WALKAROUND)
			pPlayer->nStatus = PLAYER_STATUS_SEATED;

		pPlayer->nTableNO = nTableNo;
		pPlayer->nChairNO = nChairNo;
		pPlayer->nNetSpeed= lpNewPrivateRoom->nNetDelay;
		pPlayer->pe.nStartupTime=UwlGetCurrentSeconds();

		NTF_GET_NEWTABLE ngn;
		memset(&ngn,0,sizeof(ngn));
		ngn.pp.nUserID=lpNewPrivateRoom->nUserID;
		ngn.pp.nTableNO=nTableNo;
		ngn.pp.nChairNO=nChairNo;
		ngn.pp.nNetDelay=lpNewPrivateRoom->nNetDelay;
		ngn.nMinScore =lpTable->th.nMinScore ;// 
		ngn.nMinDeposit =lpTable->th.nMinDeposit ;//
		ngn.nFirstSeatedPlayer=lpTable->th.nFirstSeatedPlayer;
 		NotifyRoomPlayers(lpRoomData, GR_PLAYER_NEWTABLE, &ngn,  sizeof(ngn),lpContext->hSocket, lpContext->lTokenID);
		
   		response.pDataPtr = &ngn;
		response.nDataLen = sizeof(ngn);
		response.head.nRequest = UR_OPERATE_SUCCEEDED;
	}
 
SendResponse:
	BOOL bSendOK = FALSE;
 	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
    bSendOK = SendResponse(context.hSocket, &context, &response);
 	return TRUE;
}

BOOL CBaseSockServer::MB_OnTryGotoOtherPrivateRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response; 
	memset(&response, 0, sizeof(response));
	
	DWORD dwIPAddr = 0;

	LPTRYGOTO_OTHERROOM lpGotoPR= LPTRYGOTO_OTHERROOM(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
 		if(!g_mapRoomDataEx.Lookup(lpGotoPR->nRoomID, lpRoomData))
		{	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
 			goto SendResponse;
		}
	}

	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 
		if (!lpRoomData->IsSoloRoom())
		{
			response.head.nRequest = GR_NO_PERMISSION;
 			goto SendResponse;
		}
 
		LPPLAYER pPlayer = NULL;
 		if(!lpRoomData->m_mapPlayer.Lookup(lpGotoPR->nUserID, pPlayer))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}

		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}

		if(!IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_HANDPHONE) || !lpRoomData->IsSupportMobile() ) //不支持手机
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
		if(pPlayer->nStatus != PLAYER_STATUS_WALKAROUND)
		{	 
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}

		dwIPAddr = pPlayer->pe.dwIPAddr;

		pPlayer->pe.dwLatestTime = GetTickCount();
		pPlayer->nNetSpeed=lpGotoPR->nNetDelay;

		if(IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ENTERHIDING))
		{
			response.head.nRequest = GR_NO_PERMISSION; //隐身网管禁止入座
			goto SendResponse;
		}
 		if(pPlayer->nDeposit < lpRoomData->roomdata.nMinDeposit){  // 
			response.head.nRequest = GR_DEPOSIT_NOTENOUGH;	// 银子数不够，不能就座
			goto SendResponse;
		}	
		if(pPlayer->nScore < lpRoomData->roomdata.nMinPlayScore){  // 积分不够
			response.head.nRequest = GR_PLAYSCORE_NOTENOUGH;	//  
 			goto SendResponse;
		}
		if(pPlayer->nScore > lpRoomData->roomdata.nMaxPlayScore){  // 积分超出
			response.head.nRequest = GR_PLAYSCORE_OVERFLOW;	//  
 			goto SendResponse;
		}

		//lpRoomData->PostRoomData();
 		//lpRoomData->PostHardAndToken(lpGotoPR->nUserID,pPlayer->pe.szHardID,pPlayer->pe.lTokenID);

		float fBreakOff = 0;
		int bout = pPlayer->nWin + pPlayer->nLoss + pPlayer->nStandOff;
		if(bout)
			fBreakOff = (float)pPlayer->nBreakOff * 100 / bout;
		int nNetSpeed = pPlayer->nNetSpeed;	

		////////////////////////////////////////////////////////////////////////////////////
		TRYGOTORESULT tgtr;
		ZeroMemory(&tgtr,sizeof(TRYGOTORESULT));
		tgtr.nGameID = lpRoomData->GetGameID();
		tgtr.nRoomID = lpRoomData->GetRoomID();
 		LPTABLE lpTable = NULL;
		int nTableNo = 0;
		int nChairNo = 0;
		BOOL bFindSeat = FALSE;
		// cloud 2016 换特定业务的函数 
		bFindSeat = lpRoomData->GetPreferPrivateTableChair(pPlayer, nTableNo, nChairNo, dwIPAddr, lpGotoPR->nIPConfig, 
			lpGotoPR->nMinScore, lpGotoPR->nMinDeposit,lpGotoPR->nExcludedHomeID);
		if (bFindSeat)// 找到了 但是是空桌 返回没有合适的桌子
		{
			if (!lpRoomData->m_mapTable.Lookup(nTableNo, lpTable))
			{
				// 没找到最合适的位置
				response.head.nRequest = UR_OPERATE_FAILED;	//连空座位都没有
 				goto SendResponse;
			}
			if (lpTable->th.nPlayerCount==0)
			{
				// 赋值
				tgtr.nResultType = eTRYGOTO_NeedCreate;
			}
			else
			{
				tgtr.nResultType = eTRYGOTO_Yes;
				tgtr.nHomeUserID =lpTable->nHomeUserID;
				tgtr.nTableNO = nTableNo;
				tgtr.nChairNO = nChairNo;
			}
		}
		else{
			// 全失败 赋值
			tgtr.nResultType = eTRYGOTO_Error;
		}
   		response.pDataPtr = &tgtr;
		response.nDataLen = sizeof(tgtr);
		response.head.nRequest = UR_OPERATE_SUCCEEDED;
	}
 
SendResponse:
	BOOL bSendOK = FALSE;
 	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
    bSendOK = SendResponse(context.hSocket, &context, &response);
 	return TRUE;
}
BOOL CBaseSockServer::MB_OnSetGameIsActived(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPSETGAMEISACTIVED lpIsActived = LPSETGAMEISACTIVED(PBYTE(lpRequest->pDataPtr));
	
	if (!lpIsActived || (lpRequest->nDataLen!=sizeof(SETGAMEISACTIVED)))
		return TRUE;
	// 查找room
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpIsActived->nRoomID, lpRoomData))
		{	
			return FALSE;
		}
	}
	//
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		if(!lpRoomData->IsSoloRoom() || !lpRoomData->IsPrivateRoom())
			return FALSE;
		
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpIsActived->nUserID, pPlayer))
			return FALSE;
		if (pPlayer->nChairNO!=lpIsActived->nChairNO)
			return FALSE;
		if (pPlayer->nTableNO!=lpIsActived->nTableNO)
			return FALSE;
		if (lpIsActived->nIsActived)
			pPlayer->pe.dwClientFlags &= ~FLAG_CLIENTINFO_INACTIVATED;
		else
			pPlayer->pe.dwClientFlags |= FLAG_CLIENTINFO_INACTIVATED;
	}
	return TRUE;
}
BOOL CBaseSockServer::MB_OnFoundPrivateRooms(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPFOUND_NEW_GROUP_TABLEROOMS lpFind = LPFOUND_NEW_GROUP_TABLEROOMS(PBYTE(lpRequest->pDataPtr));
	
	if (!lpFind || (lpRequest->nDataLen!=sizeof(FOUND_NEW_GROUP_TABLEROOMS)))
		return TRUE;
	
	REQUEST response;
	memset(&response, 0, sizeof(response));

	ONE_TABLEROOM  Result[ONE_GROUP_TABLE_NUM];
	ZeroMemory(Result,sizeof(Result));
	int nCount =0;
	int nLen1 = 0;
	int nlen2 = 0;
	// 查找room
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpFind->nRoomID, lpRoomData))
		{	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	//
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		if(!lpRoomData->IsSoloRoom() || !lpRoomData->IsPrivateRoom())
		{
			response.head.nRequest =GR_NO_PERMISSION;	 
			goto SendResponse;
		}
	
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpFind->nUserID, pPlayer))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}
		if(pPlayer->nStatus != PLAYER_STATUS_WALKAROUND){	// 这个用户应该是什么都不干的状态
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		pPlayer->pe.dwClientFlags &= ~FLAG_CLIENTINFO_INACTIVATED;

		int nOldCacheNum = lpRoomData->GetPrivateTableCacheNum(pPlayer->nUserID);
		nCount = lpRoomData->FindPrivateTables(pPlayer,Result,lpFind->nGroupNum);

		if((nCount == 0 && lpRoomData->GetPrivateTableCacheNum(pPlayer->nUserID) > 0) // 尾部刷新
			||(nOldCacheNum >0 && nOldCacheNum < lpFind->nGroupNum) )// 第一批动态刷新
		{
			lpRoomData->DeletePrivateTableCache(pPlayer->nUserID);
			nCount = lpRoomData->FindPrivateTables(pPlayer,Result,lpFind->nGroupNum);
		}
	}

	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	nLen1 = sizeof(REPLY_NEW_GROUP_TABLEROOMS);
	nlen2 = sizeof(ONE_TABLEROOM) * nCount;
	response.pDataPtr = new BYTE[nLen1+nlen2];
	REPLY_NEW_GROUP_TABLEROOMS rngt;
	ZeroMemory(&rngt,nLen1);
	rngt.nResultCount = nCount;
	memcpy(response.pDataPtr, &rngt, nLen1);
	memcpy((BYTE*)response.pDataPtr+nLen1, Result, nlen2);
	response.nDataLen = nLen1+nlen2;


SendResponse:
	BOOL bSendOK = FALSE;
	
	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
	UwlClearRequest(&response);
	return TRUE;
}
BOOL CBaseSockServer::MB_OnAskDetailTableRooms(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPASK_DETAIL_TABLEROOMS lpDetail = LPASK_DETAIL_TABLEROOMS(PBYTE(lpRequest->pDataPtr));
	
	if (!lpDetail || (lpRequest->nDataLen!=sizeof(ASK_DETAIL_TABLEROOMS)))
		return TRUE;
	
	REQUEST response;
	memset(&response, 0, sizeof(response));
	REPLY_DETAIL_TABLEROOMS reply;
	memset(&reply, 0, sizeof(reply));
	ONE_TRPLAYER allPlayers[MAX_CHAIR_COUNT];
	memset(allPlayers, 0, sizeof(allPlayers));

	int nCount =0;
	int nLen1 = 0;
	int nlen2 = 0;
	// 查找room
	CBaseRoomData* lpRoomData = NULL;
	TABLE* pTable=NULL;
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpDetail->nRoomID, lpRoomData))
		{	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		if(lpRoomData->IsDarkRoom() || !lpRoomData->IsPrivateRoom())
		{
			response.head.nRequest =GR_NO_PERMISSION;	 
			goto SendResponse;
		}
		
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpDetail->nUserID, pPlayer))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}
		if(pPlayer->nStatus != PLAYER_STATUS_WALKAROUND){	// 这个用户应该是什么都不干的状态
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		
		if(!lpRoomData->m_mapTable.Lookup(lpDetail->nTableID,pTable))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 桌子不存在
			goto SendResponse;
		}
		reply.nHomeUserID = pTable->nHomeUserID;
		nCount = lpRoomData->PrivateTableDetail(pTable,allPlayers);
	}

	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	
	nLen1 = sizeof(REPLY_DETAIL_TABLEROOMS);
	nlen2 = sizeof(ONE_TRPLAYER) * nCount;
	response.pDataPtr = new BYTE[nLen1+nlen2];
	reply.nPlayerCount = nCount;
	memcpy(response.pDataPtr, &reply, nLen1);
	memcpy((BYTE*)response.pDataPtr+nLen1, allPlayers, nlen2);
	response.nDataLen = nLen1+nlen2;
	
SendResponse:
	BOOL bSendOK = FALSE;
	
	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;

	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
	UwlClearRequest(&response);
	return TRUE;


}
BOOL CBaseSockServer::MB_OnAskEnterPrivateRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	DWORD dwIPAddr = 0;

	LPENTER_PRIVATEROOM lpEnterPR= LPENTER_PRIVATEROOM(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
 		if(!g_mapRoomDataEx.Lookup(lpEnterPR->nRoomID, lpRoomData))
		{	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
 			goto SendResponse;
		}
	}

	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 
		if (!lpRoomData->IsSoloRoom() ||!lpRoomData->IsPrivateRoom())
		{
			response.head.nRequest = GR_NO_PERMISSION;
 			goto SendResponse;
		}
 
		LPPLAYER pPlayer = NULL;
 		if(!lpRoomData->m_mapPlayer.Lookup(lpEnterPR->nUserID, pPlayer))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}

		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}

		if(!IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_HANDPHONE) || !lpRoomData->IsSupportMobile() ) //不支持手机
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
		if(pPlayer->nStatus != PLAYER_STATUS_WALKAROUND )
		{	 
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}

		dwIPAddr = pPlayer->pe.dwIPAddr;
		int nMutexExpiration = 0;
		pPlayer->pe.dwLatestTime = GetTickCount();
		pPlayer->nNetSpeed=lpEnterPR->nNetDelay;

		float fBreakOff = 0;
		int bout = pPlayer->nWin + pPlayer->nLoss + pPlayer->nStandOff;
		if(bout)
			fBreakOff = (float)pPlayer->nBreakOff * 100 / bout;
		int nNetSpeed = pPlayer->nNetSpeed;	

		if(IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ENTERHIDING))
		{
			response.head.nRequest = GR_NO_PERMISSION; //隐身网管禁止入座
			goto SendResponse;
		}
 		if(pPlayer->nDeposit < lpRoomData->roomdata.nMinDeposit){  // 
			response.head.nRequest = GR_DEPOSIT_NOTENOUGH;	// 银子数不够，不能就座
			goto SendResponse;
		}	
		if(pPlayer->nScore < lpRoomData->roomdata.nMinPlayScore){  // 积分不够
			response.head.nRequest = GR_PLAYSCORE_NOTENOUGH;	//  
 			goto SendResponse;
		}
		if(pPlayer->nScore > lpRoomData->roomdata.nMaxPlayScore){  // 积分超出
			response.head.nRequest = GR_PLAYSCORE_OVERFLOW;	//  
 			goto SendResponse;
		}
		LPTABLE lpTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(lpEnterPR->nTableNO, lpTable)) // 没有找到这个桌子
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
		// 检查IP
		if (lpRoomData->IsForbidIPOnTable(pPlayer->nUserID,lpTable,dwIPAddr,lpEnterPR->nIPConfig)){
			response.head.nRequest = GR_IP_FORBIDDEN;	//  
 			goto SendResponse;
		}
		//检查IP互斥
		if (lpRoomData->IsMutexIPOnTable(pPlayer->nUserID,lpTable,dwIPAddr)){
			response.head.nRequest = GR_IP_FORBIDDEN;	//  
			goto SendResponse;
		}
		//检查IP组互斥
		if (lpRoomData->IsMutexIPGroupOnTable(pPlayer->nUserID,lpTable,dwIPAddr)){
			response.head.nRequest = GR_IP_FORBIDDEN;	//  
			goto SendResponse;
		}
		//检查局域网同桌
		if(lpRoomData->IsSameLANOnTable(pPlayer->nUserID,lpTable)){
			response.head.nRequest = GR_IP_FORBIDDEN;	//  
			goto SendResponse;
		}
		// 检查断线率
		if (lpRoomData->IsForbidBreakOnTable(pPlayer->nUserID,lpTable,fBreakOff)){
			response.head.nRequest = GR_BREAKOFF_FORBIDDEN;	//  
			goto SendResponse;
		}
		// 检查网络延迟
		if (lpRoomData->IsForbidSpeedOnTable(pPlayer->nUserID,lpTable,nNetSpeed)){
			response.head.nRequest = GR_NETSPEED_FORBIDDEN;	//  
			goto SendResponse;
		}
		// 检查桌银限制
		if(lpRoomData->IsTableDeposit())
		{
			int nMinDeposit=0;
			int nMaxDeposit=0;
			CString sRemark;
			lpRoomData->GetTableDeposit(lpEnterPR->nTableNO,nMinDeposit,nMaxDeposit,sRemark);
			if( pPlayer->nDeposit < nMinDeposit || pPlayer->nDeposit > nMaxDeposit){
				response.head.nRequest = GR_MINDEPOSIT_FORBIDDEN;	//  
				goto SendResponse;
			}
		}
		// 检查桌子密码
		if(lpTable->th.bHavePassword){
			response.head.nRequest = UR_PASSWORD_WRONG;	//  
			goto SendResponse;
		}
		// 检查硬件互斥
		if(lpRoomData->IsMutexHardOnTable(pPlayer->nUserID,pPlayer->pe.szMachineID,lpTable,nMutexExpiration)){
			response.head.nRequest = GR_MUTEXHARD_FORBIDDEN;	//  
			goto SendResponse;
		}

		
		if (lpEnterPR->nHomeUserID != lpTable->nHomeUserID) // 房间过期 已经散了
		{
			response.head.nRequest = GR_NOBODY_PRIVATE_TABLE;
			goto SendResponse;
		}
		if (lpEnterPR->nEnterGameFlag == eENTERFLAG_FROM_SEARCH && !lpRoomData->IsOpenPrivateTable(lpTable) )
		{
			response.head.nRequest = UR_PRIVATE_TABLE_LOCKED;
			goto SendResponse;
		}
		//
		lpRoomData->PostRoomData();
 		lpRoomData->PostHardAndToken(lpEnterPR->nUserID,pPlayer->pe.szHardID,pPlayer->pe.lTokenID);

		

		////////////////////////////////////////////////////////////////////////////////////
 	
		int nTableNo = lpEnterPR->nTableNO;
		int nChairNo = 0;
		BOOL bFindSeat = FALSE;
		bFindSeat = lpRoomData->GetAppointTableChair(pPlayer, nTableNo, nChairNo);
		
		if (!bFindSeat )
		{
			if (lpTable->th.nPlayerCount == lpRoomData->GetChairCount())
			{
				response.head.nRequest = GR_FULL_PRIVATE_TABLE;	//人满
			}
			else 
			{
				response.head.nRequest = GR_NOBODY_PRIVATE_TABLE;	//已经散了
			}
 			goto SendResponse;
		}
		

		lpTable->dwIPAddrs[nChairNo] = dwIPAddr;
		lpTable->fBreakOffs[nChairNo] = fBreakOff;
		lpTable->nNetSpeeds[nChairNo] = nNetSpeed;

		lpTable->nIPConfigs[nChairNo] =   lpEnterPR->nIPConfig;
		lpTable->nBreakReqs[nChairNo] =   lpEnterPR->nBreakReq;
		lpTable->nSpeedReqs[nChairNo] =   lpEnterPR->nSpeedReq;
		lpTable->nMinScores[nChairNo] =   lpEnterPR->nMinScore;
		lpTable->nMinDeposits[nChairNo] = lpEnterPR->nMinDeposit;


		lpTable->nPlayerAry[nChairNo] = lpEnterPR->nUserID;
		lpTable->th.nPlayerCount++;
	
		//Room table chair
		lpRoomData->PostRoomTableChair(lpEnterPR->nUserID,nTableNo,nChairNo);
		if (pPlayer->nStatus == PLAYER_STATUS_WALKAROUND)
			pPlayer->nStatus = PLAYER_STATUS_SEATED;
		// 进入了包房清理缓存
		lpRoomData->DeletePrivateTableCache(lpEnterPR->nUserID);
	
		pPlayer->nTableNO = nTableNo;
		pPlayer->nChairNO = nChairNo;
		pPlayer->nNetSpeed= lpEnterPR->nNetDelay;
		pPlayer->pe.nStartupTime=UwlGetCurrentSeconds();

		NTF_GET_NEWTABLE ngn;
		memset(&ngn,0,sizeof(ngn));
		ngn.pp.nUserID=lpEnterPR->nUserID;
		ngn.pp.nTableNO=nTableNo;
		ngn.pp.nChairNO=nChairNo;
		ngn.pp.nNetDelay=lpEnterPR->nNetDelay;
		ngn.nMinScore =lpTable->th.nMinScore ;//
		ngn.nMinDeposit =lpTable->th.nMinDeposit ;//
		ngn.nFirstSeatedPlayer=lpTable->th.nFirstSeatedPlayer;
		ngn.nHomeUserID = lpTable->nHomeUserID;
 		NotifyRoomPlayers(lpRoomData, GR_PLAYER_NEWTABLE, &ngn,  sizeof(ngn),lpContext->hSocket, lpContext->lTokenID);
		
   		response.pDataPtr = &ngn;
		response.nDataLen = sizeof(ngn);
		response.head.nRequest = UR_OPERATE_SUCCEEDED;
	}
 
SendResponse:
	BOOL bSendOK = FALSE;
 	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
    bSendOK = SendResponse(context.hSocket, &context, &response);
 	return TRUE;

}
BOOL CBaseSockServer::MB_OnSetOpeningPrivateRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	DWORD dwIPAddr = 0;
	
	LPSET_OPENING_PRIVATEROOM lpSOPR= LPSET_OPENING_PRIVATEROOM(PBYTE(lpRequest->pDataPtr));
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpSOPR->nRoomID, lpRoomData))
		{	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		LPTABLE pTable = NULL;
		if (!lpRoomData->IsSoloRoom() || !lpRoomData->IsPrivateRoom())
		{
			response.head.nRequest = UR_INVALID_PARAM;
			goto SendResponse;
		}
		if(!lpRoomData->m_mapTable.Lookup(lpSOPR->nTableNO, pTable))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
		if(lpSOPR->nChairNO<0 || lpSOPR->nChairNO>=MAX_CHAIR_COUNT)
		{
			response.head.nRequest = UR_INVALID_PARAM;
			goto SendResponse;
		}
		if(pTable->nPlayerAry[lpSOPR->nChairNO] != lpSOPR->nUserID){	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;		// 用户不匹配
			goto SendResponse;
		}
		if (pTable->th.nStatus==TABLE_STATUS_PLAYING)
		{
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		if (pTable->nHomeUserID!=lpSOPR->nUserID)
		{
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		lpRoomData->SetOpenPrivateTable(pTable,lpSOPR->nIsOpening);
		response.pDataPtr = &dwIPAddr;
		response.nDataLen = sizeof(int);
		response.head.nRequest = UR_OPERATE_SUCCEEDED;
	}
SendResponse:

	BOOL bSendOK = FALSE;
	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
    bSendOK = SendResponse(context.hSocket, &context, &response);
 	return TRUE;
}
BOOL CBaseSockServer::MB_OnAskSystemFindPlayers(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	int nNeedCount = 0;
	BE_FOUND_BY_SYSTEM bfbs;

	LPASK_SYSFINDPLAYERS lpSfps= LPASK_SYSFINDPLAYERS(PBYTE(lpRequest->pDataPtr));
	if (!lpSfps)
		return FALSE;

	TABLEAVGVALUE avgValue;
	ZeroMemory(&avgValue,sizeof(avgValue));

	LPPLAYER pSender = NULL;
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpSfps->nRoomID, lpRoomData))
			return FALSE;
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		LPTABLE pTable = NULL;
		if (!lpRoomData->IsSoloRoom() || !lpRoomData->IsPrivateRoom())
		{
			response.head.nRequest = UR_INVALID_PARAM;
			goto SendResponse;
		}
		if(!lpRoomData->m_mapTable.Lookup(lpSfps->nTableNO, pTable))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
		if(lpSfps->nChairNO<0 || lpSfps->nChairNO>=MAX_CHAIR_COUNT)
		{
			response.head.nRequest = UR_INVALID_PARAM;
			goto SendResponse;
		}
		if(pTable->nPlayerAry[lpSfps->nChairNO] != lpSfps->nUserID){	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;		// 用户不匹配
			goto SendResponse;
		}
		if (pTable->th.nStatus==TABLE_STATUS_PLAYING)
		{
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		if (pTable->nHomeUserID!=lpSfps->nUserID)
		{
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		if(!lpRoomData->m_mapPlayer.Lookup(lpSfps->nUserID, pSender)) 
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}
		if (pSender->nStatus != PLAYER_STATUS_WAITING)
		{
			response.head.nRequest = UR_OPERATE_FAILED;
			goto SendResponse;
		}
		if(!lpRoomData->CheckSystemFindPlayersCD(lpSfps->nTableNO))
		{
			response.head.nRequest = UR_SYSTEMFINDPLAYERSCD;		
			goto SendResponse;
		}
		nNeedCount = (lpRoomData->GetChairCount() - pTable->th.nPlayerCount) * g_nSysFindScale;
		if (nNeedCount == 0)//人满了
		{
			response.head.nRequest = UR_OPERATE_FAILED;		
			goto SendResponse;
		}
		
		lpRoomData->CalculateAvgValue(pTable,avgValue.nAvgBounts,avgValue.nAvgWins,avgValue.nAvgScore,avgValue.nAvgDeposit);
		bfbs.nHomeUserID = lpSfps->nUserID;
		bfbs.nTableNO = lpSfps->nTableNO;
		StrCpyN(bfbs.szHUserName,pSender->szUsername,sizeof(pSender->szUsername));
		lpRoomData->SetSystemFindPlayersTime(lpSfps->nTableNO);
	}
	if (nNeedCount>0)
	{
		BeginSystemFindPlayers(lpRoomData,nNeedCount,&bfbs,avgValue);
		
	}
	response.pDataPtr = &lpSfps->nTableNO;
	response.nDataLen = sizeof(int);
	response.head.nRequest = UR_OPERATE_SUCCEEDED;

SendResponse:
	BOOL bSendOK = FALSE;
	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
    bSendOK = SendResponse(context.hSocket, &context, &response);
 	return TRUE;
}
BOOL CBaseSockServer::OnVipKickOffPlayer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	LPVIP_KICKOFFPLAYER lpKickOff = LPVIP_KICKOFFPLAYER(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;

	TCHAR szError[256];
	memset(szError,0,sizeof(szError));

	lpKickOff->dwIP = GetClientAddress(lpContext->hSocket, lpContext->lTokenID);	

	if(!lpKickOff || lpKickOff->nUserID<=0 || lpKickOff->nTableNO<0 || lpKickOff->nRoomID<=0 
		|| lpKickOff->nKickedPlayer<=0 || lpKickOff->nKickedChairNO<0 
		|| lpKickOff->nKickedChairNO>=MAX_CHAIR_COUNT
		|| lpKickOff->nUserID == lpKickOff->nKickedPlayer)
	{
		response.head.nRequest = UR_INVALID_PARAM;
		goto SendResponse;
	}

	{
		//踢人总开关，应对版本升级期间，暂时不能使用踢人功能
		int nClose = GetPrivateProfileInt(_T("VipKickoff"), _T("close"), 0, g_szIniFile);
		if (nClose)
		{
			if (GetPrivateProfileString(_T("VipKickoff"), _T("CloseReason"), _T(""), szError, 256, g_szIniFile)==0)
				sprintf(szError, _T("尊贵的蓝钻会员，本房间禁止请TA离开！"));
			response.head.nRequest = GR_ERROR_INFOMATION; 
			goto SendResponse;
		}
	}

	{
		CAutoLock lock(&g_csRoomData);
		
		if(!g_mapRoomDataEx.Lookup(lpKickOff->nRoomID, lpRoomData))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		if(lpRoomData->IsDarkRoom() || lpRoomData->IsRandomRoom() || lpRoomData->IsWebGameRoom())
		{
			sprintf(szError,_T("尊贵的蓝钻会员，本房间禁止请TA离开！"));
			response.head.nRequest = GR_ERROR_INFOMATION; 
			goto SendResponse;
		}

		if(IS_BIT_SET(lpRoomData->roomdata.dwManages , ROOM_MNG_FORBID_VIPKICK))
		{
			sprintf(szError,_T("尊贵的蓝钻会员，本房间禁止请TA离开！"));
			response.head.nRequest = GR_ERROR_INFOMATION;	 
			goto SendResponse;
		}
		
		LPPLAYER pSender = NULL;
		
		if(!lpRoomData->m_mapPlayer.Lookup(lpKickOff->nUserID, pSender)) 
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}

		if(lstrcmp(pSender->pe.szHardID,lpKickOff->szHardID))
		{
			response.head.nRequest = GR_HARDID_MISMATCH;
			goto SendResponse;
		}
		if(pSender->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}

		if(!IS_BIT_SET(pSender->nUserType,USER_TYPE_MEMBER))
		{
			sprintf(szError,_T("您还不是会员，成为会员畅享会员特权！"));
			response.head.nRequest = GR_ERROR_INFOMATION;
			goto SendResponse;
		}

		//今日踢人次数判断
		if(!IS_BIT_SET(lpRoomData->roomdata.dwManages , ROOM_MNG_NOLIMIT_VIPKICK))
		{
			int nLimitCount = 0;
			if (lpRoomData->IsKickoffToomany(pSender->nUserID, pSender->nMemberLevel, nLimitCount))
			{
				sprintf(szError,_T("非常抱歉，您在本房间的踢人次数超过限制！\r\n\r\n本房间Vip%d级的玩家，每日允许踢人%d次。")
					, ConvertMemberLevelToDisplay(pSender->nMemberLevel), nLimitCount);
				response.head.nRequest = GR_ERROR_INFOMATION;
				goto SendResponse;	
			}
		}

		CString sCause;
		if (IsForbidVipKick(lpKickOff->nUserID, sCause))
		{
			sprintf(szError,_T("非常抱歉，您被管理员禁止请TA离开，原因：\r\n\r\n%s\r\n\r\n详情请与管理员联系。"), sCause);
			response.head.nRequest = GR_ERROR_INFOMATION;
			goto SendResponse;	
		}

		LPPLAYER pKickedPlayer = NULL;
		
		if(!lpRoomData->m_mapPlayer.Lookup(lpKickOff->nKickedPlayer, pKickedPlayer)) 
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}

		lpKickOff->nMemberLevel = pSender->nMemberLevel;			//赋值踢人者等级
		lpKickOff->nKickedUType = pKickedPlayer->nUserType;			//赋值踢人者类型
		lpKickOff->nKickedUMLevel = pKickedPlayer->nMemberLevel;	//赋值踢人者等级

		//同桌判断
		if (lpKickOff->nTableNO != pSender->nTableNO || pSender->nTableNO != pKickedPlayer->nTableNO)
		{
			sprintf(szError,_T("尊贵的蓝钻会员，您只能请和你同桌的玩家离开！"));
			response.head.nRequest = GR_ERROR_INFOMATION;
			goto SendResponse;
		}
		
		if (lpKickOff->nKickedChairNO != pKickedPlayer->nChairNO)
		{
			response.head.nRequest = UR_INVALID_PARAM;
			goto SendResponse;
		}

		//不踢移动端
		if(!IS_BIT_SET(lpRoomData->roomdata.dwManages , ROOM_MNG_VIPKICK_MOBILE)
			&& IS_BIT_SET(pKickedPlayer->pe.dwEnterFlags, FLAG_ENTERROOM_HANDPHONE))
		{
			sprintf(szError,_T("尊贵的蓝钻会员，本房间禁止请移动端玩家离开！"));
			response.head.nRequest = GR_ERROR_INFOMATION;
			goto SendResponse;
		}

		//vip会员等级不够
		if(IS_BIT_SET(pKickedPlayer->nUserType,USER_TYPE_MEMBER) 
			&& pKickedPlayer->nMemberLevel >= pSender->nMemberLevel)
		{
			sprintf(szError,_T("尊贵的蓝钻会员，只有您的蓝钻等级高于对方才可以请TA离开！"));
			response.head.nRequest = GR_ERROR_INFOMATION;
			goto SendResponse;
		}

		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(lpKickOff->nTableNO, pTable))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有就座
			goto SendResponse;
		}
		
		if(lpKickOff->nUserID != pTable->nPlayerAry[pSender->nChairNO]
			|| lpKickOff->nKickedPlayer != pTable->nPlayerAry[pKickedPlayer->nChairNO])
		{	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;		// 这个位置没有人
			goto SendResponse;
		}

		if(pTable->th.nStatus == TABLE_STATUS_PLAYING)// 游戏进行中
		{
			sprintf(szError,_T("尊贵的蓝钻会员，游戏开始后不能请TA离开！"));
			response.head.nRequest = GR_ERROR_INFOMATION; 
			goto SendResponse;
		}

		//入座和等待状态，才能参与踢人
		if (pSender->nStatus != PLAYER_STATUS_SEATED && pSender->nStatus != PLAYER_STATUS_WAITING)
		{
			sprintf(szError,_T("尊贵的蓝钻会员，您入座后才能请TA离开！"));
			response.head.nRequest = GR_ERROR_INFOMATION; 
			goto SendResponse;
		}

		if(pKickedPlayer->nStatus != PLAYER_STATUS_SEATED && pKickedPlayer->nStatus != PLAYER_STATUS_WAITING )
		{	
			sprintf(szError,_T("尊贵的蓝钻会员，现在不能请TA离开！"));
			response.head.nRequest = GR_ERROR_INFOMATION; 
			goto SendResponse;
		}
		
		pSender->pe.dwLatestTime = GetTickCount();
		if (lpRoomData->IsSoloRoom() &&  PLAYER_STATUS_WAITING == pKickedPlayer->nStatus)
		{
			lpRoomData->PostKickoffPlayer(pKickedPlayer->nUserID,pKickedPlayer->nTableNO,pKickedPlayer->nChairNO);
			OverSeeUserData(pKickedPlayer->nUserID,"OnVipKickOffPlayer::CommitTPS_SOLO");
			lpRoomData->CommitTPS_SOLO(pKickedPlayer->nUserID,pKickedPlayer->nTableNO,PS_WALKAROUND);
		}

		SetCommonTableLeave(lpRoomData, pKickedPlayer);
		SetRandomTableLeave(lpRoomData, pKickedPlayer);

		if(pTable->th.nFirstSeatedPlayer==pKickedPlayer->nUserID) //第一个入座的人离开
		{
			pTable->th.nMinDeposit=0;
			pTable->th.nMinScore =SCORE_MIN;
		}
		
		pKickedPlayer->nStatus = PLAYER_STATUS_WALKAROUND;
		pKickedPlayer->nTableNO = -1;
		pKickedPlayer->nChairNO = -1;
		
		PLAYER_KICKED pk;
		memset(&pk, 0, sizeof(PLAYER_KICKED));
		pk.nUserID = lpKickOff->nKickedPlayer;
		pk.nTableNO = lpKickOff->nTableNO;
		pk.nChairNO = lpKickOff->nKickedChairNO;
		pk.nKickerID = lpKickOff->nUserID;
		pk.nKickerMemberLevel = pSender->nMemberLevel;
		lstrcpyn(pk.szKickerName, pSender->szUsername, MAX_USERNAME_LEN);
		NotifyRoomPlayers(lpRoomData, GR_PLAYER_KICKED, &pk, sizeof(PLAYER_KICKED));

//		if(!IS_BIT_SET(lpRoomData->roomdata.dwManages , ROOM_MNG_NOLIMIT_VIPKICK))
		{
			//记录踢人次数
			SaveVipKickoffRecord(lpKickOff->nUserID, lpKickOff->nRoomID);
		}

//		if(IS_BIT_SET(lpRoomData->roomdata.dwManages , ROOM_MNG_ISOLATE_KICKEACHOTHER))
		{
			//记录踢人明细
			SaveVipKickoffDetail(lpKickOff->nUserID, lpKickOff->nKickedPlayer);
		}
	}
	
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	
SendResponse:
	BOOL bSendOK = FALSE;
	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	if(GR_ERROR_INFOMATION==response.head.nRequest)
	{
		response.pDataPtr=szError;
		response.nDataLen=lstrlen(szError)+1;
	}	
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
	
	if(UR_OPERATE_SUCCEEDED == response.head.nRequest)
	{
		NotifyServer(GR_VIP_KICKOFFPLAYER, lpKickOff, lpRequest->nDataLen);
	}
	else
	{
		
	}
	return TRUE;
}

BOOL CBaseSockServer::IsForbidVipKick(int nUserID, CString& sCause)
{
	return IsSomeValueInFile(_T("ForbidUser"), xyConvertIntToStr(nUserID), sCause, g_szKickManFile);
}

void  CBaseSockServer::BeginSystemFindPlayers(CBaseRoomData* pCurRoom,int nNeedCount,LPBE_FOUND_BY_SYSTEM pBfbs,const TABLEAVGVALUE& avgValue)
{
	// 准备数据
	if (!pCurRoom)
		return;
                                                                                                                                      
	pBfbs->nRoomID = pCurRoom->GetRoomID();	
	pBfbs->nAreaID = pCurRoom->GetAreaID();
	pBfbs->nGameID = pCurRoom->GetGameID();
	int nCurRoomId = pCurRoom->GetRoomID();
	// 随机选择算法
	int nMode = rand()%ESFP_MODE_MAX;
	//nMode = GetPrivateProfileInt (_T("PrivateRoom"),_T("ModeType"),ESFP_MODE_MAX-1,g_szIniFile);
	//	条件一：银两/积分等级一致
	//	条件二：局数相差不到N局（N可配）
	//	条件三：胜率相差不到x（x可配）

	// 先通知本房间
	int nNotifiedCount = pCurRoom->BeginSystemFindPlayers(nNeedCount,pBfbs,nMode,avgValue);
	if (nNotifiedCount>=nNeedCount)
		return;

	// 再通知其他房间
	int roomid = 0;
	CBaseRoomData* lpRoomData=0;
	CRoomDataExMap mapRoomDataEx;
	{
		CAutoLock lock(&g_csRoomData);	
		POSITION pos = g_mapRoomDataEx.GetStartPosition();
		while (pos)	
		{
			g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			if (nCurRoomId == roomid)
				continue;
			if (lpRoomData->GetGameID()!= pCurRoom->GetGameID())
				continue;
			mapRoomDataEx.SetAt(roomid, lpRoomData);
		}
	}
	
	POSITION pos = mapRoomDataEx.GetStartPosition();
	while (pos)	
	{
		mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
		if(lpRoomData)
		{
			nNotifiedCount += lpRoomData->BeginSystemFindPlayers(nNeedCount-nNotifiedCount,pBfbs,nMode,avgValue);
			if (nNotifiedCount>=nNeedCount)
				return;
		}
	}
}
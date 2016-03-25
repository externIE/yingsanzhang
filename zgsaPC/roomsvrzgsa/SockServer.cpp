#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CSockServer::CSockServer(const BYTE key[] , const ULONG key_len , DWORD flagEncrypt , DWORD flagCompress)
	: CSockOpenServer(key, key_len, flagEncrypt, flagCompress)
{

}
CSockServer::CSockServer(int nKeyType, DWORD flagEncrypt, DWORD flagCompress)
	: CSockOpenServer(nKeyType, flagEncrypt, flagCompress)
{
   	
}

CSockServer::~CSockServer()
{

}

BOOL CSockServer::OnRequest(void* lpParam1, void* lpParam2)
{//try{
	LPCONTEXT_HEAD	lpContext = LPCONTEXT_HEAD	(lpParam1);
	LPREQUEST		lpRequest = LPREQUEST		(lpParam2);

	UwlTrace(_T("----------------------start of request process-------------------"));
	
#if defined(_UWL_TRACE) | defined(UWL_TRACE)
	DWORD dwTimeStart = GetTickCount();
#else
	DWORD dwTimeStart = 0;
#endif
	CWorkerContext* pThreadCxt = reinterpret_cast<CWorkerContext*>(GetWorkerContext());

	assert(lpContext && lpRequest);
	UwlTrace(_T("socket = %ld requesting..."), lpContext->hSocket);
	switch(lpRequest->head.nRequest){
	// request from system connection
	case UR_SOCKET_CONNECT:
		UwlTrace(_T("UR_SOCKET_CONNECT requesting..."));
		OnConnectSocket(lpContext, lpRequest, pThreadCxt);
		break;
	case UR_SOCKET_CLOSE:
		UwlTrace(_T("UR_SOCKET_CLOSE requesting..."));
		OnCloseSocket(lpContext, lpRequest, pThreadCxt);
		break;
	case UR_SOCKET_ERROR:
		UwlTrace(_T("UR_SOCKET_ERROR requesting..."));
		//OnCloseSocket(lpContext, lpRequest, pThreadCxt);
		break;
	// user request
	case GR_ROOMUSER_PULSE:
		UwlTrace(_T("GR_ROOMUSER_PULSE requesting..."));
		OnRoomUserPulse(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_RECONNECT_SVR:
		UwlTrace(_T("GR_RECONNECT_SVR requesting..."));
		OnReconnectSvr(lpContext, lpRequest, pThreadCxt);
		break;
	case IR_RESET_CHECKSVRIP:
		UwlTrace(_T("IR_RESET_CHECKSVRIP requesting..."));
		OnResetCheckSvrIP(lpContext, lpRequest, pThreadCxt);
 		break;
	case GR_CHECK_NETWORK:
		UwlTrace(_T("GR_CHECK_NETWORK requesting..."));
		OnCheckNetwork(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_SEND_PULSE:
		UwlTrace(_T("GR_SEND_PULSE requesting..."));
		OnSendPulse(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_ADD_FRIEND:
		UwlTrace(_T("GR_ADD_FRIEND requesting..."));
		OnAddFriend(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_AGREE_FRIEND:
		UwlTrace(_T("GR_AGREE_FRIEND requesting..."));
		OnAgreeFriend(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_DISAGREE_FRIEND:
		UwlTrace(_T("GR_DISAGREE_FRIEND requesting..."));
		OnDisagreeFriend(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_COMMIT_UNWELCOMES:
		UwlTrace(_T("GR_COMMIT_UNWELCOMES requesting..."));
		OnCommitUnwelcomes(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_COMMIT_ONE_UNWELCOME:
		UwlTrace(_T("GR_COMMIT_ONE_UNWELCOME requesting..."));
		OnCommitOneUnwelcome(lpContext, lpRequest, pThreadCxt);
 		break;
	case GR_COMMIT_CLIENTINFO:
 		OnCommitClientInfo(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_CALL_PLAYER:
		UwlTrace(_T("GR_CALL_PLAYER requesting..."));
		OnCallPlayer(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_CHAT_TO_PLAYER:
		UwlTrace(_T("GR_CHAT_TO_PLAYER requesting..."));
		OnChatToPlayer(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_QUERY_TABLEPWD:
		UwlTrace(_T("GR_QUERY_TABLEPWD requesting..."));
		OnQueryTablePwd(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_ANSWER_TABLEPWD:
		UwlTrace(_T("GR_ANSWER_TABLEPWD requesting..."));
		OnAnswerTablePwd(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_FORBID_CHAT:
		UwlTrace(_T("GR_FORBID_CHAT requesting..."));
		OnForbidChat(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_FORBID_ROOM:
		UwlTrace(_T("GR_FORBID_ROOM requesting..."));
		OnForbidRoom(lpContext, lpRequest, pThreadCxt);
		break;
 	case GR_CHAT_TO_ROOM:
		UwlTrace(_T("GR_CHAT_TO_ROOM requesting..."));
		OnChatToRoom(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_ENTER_ROOM:
		UwlTrace(_T("GR_ENTER_ROOM requesting..."));
		OnEnterRoom(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_ENTER_ROOM_VERIFIED:
		UwlTrace(_T("GR_ENTER_ROOM_VERIFIED requesting..."));
		OnEnterRoomVerified(lpContext, lpRequest, pThreadCxt);
		break;
 	case GR_GET_FINISHED:
		UwlTrace(_T("GR_GET_FINISHED requesting..."));
		OnGetFinished(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_GET_LOOKON:
		UwlTrace(_T("GR_GET_LOOKON requesting..."));
		OnGetLookon(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_LEAVE_ROOM:
		UwlTrace(_T("GR_LEAVE_ROOM requesting..."));
		OnLeaveRoom(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_GET_SEATED:
		UwlTrace(_T("GR_GET_SEATED requesting..."));
		OnGetSeated(lpContext, lpRequest, pThreadCxt);
		break;

	case GR_DARK_SEATED:
		UwlTrace(_T("GR_DARK_SEATED requesting..."));
		OnDarkSeated(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_DARK_UNSEATED:
		UwlTrace(_T("GR_DARK_UNSEATED requesting..."));
		OnDarkUnseated(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_DARK_STARTED:
		UwlTrace(_T("GR_DARK_STARTED requesting..."));
		OnDarkStarted(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_DARK_FINISHED:
		UwlTrace(_T("GR_DARK_FINISHED requesting..."));
		OnDarkFinished(lpContext, lpRequest, pThreadCxt);
		break;
 	case GR_GET_UNSEATED:
		UwlTrace(_T("GR_GET_UNSEATED requesting..."));
		OnGetUnseated(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_GET_STARTED:
		UwlTrace(_T("GR_GET_STARTED requesting..."));
		OnGetStarted(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_GET_UNLOOKON:
		UwlTrace(_T("GR_GET_UNLOOKON requesting..."));
		OnGetUnlookon(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_INVITE_PLAYER:
		UwlTrace(_T("GR_INVITE_PLAYER requesting..."));
		OnInvitePlayer(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_UNWELCOME_PLAYER:
		UwlTrace(_T("GR_UNWELCOME_PLAYER requesting..."));
		OnUnWelcome(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_REFRESH_CLOTHING:
		UwlTrace(_T("GR_REFRESH_CLOTHING requesting..."));
		OnRefreshClothing(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_ASK_ENTERGAME:
		OnAskEnterGame(lpContext, lpRequest, pThreadCxt);
 		break;
	case GR_REFRESH_MEMBER:
		OnRefreshMember(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_VIP_KICKOFFPLAYER:
		OnVipKickOffPlayer(lpContext, lpRequest, pThreadCxt);
		break;

		////移动端请求
	case MR_SEND_PULSE://手机发上来的心跳
		MB_OnSendPulse(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_ENTER_ROOM:
 		MB_OnEnterRoom(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_GET_NEWTABLE:
 		MB_OnGetNewTable(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_LEAVE_ROOM:
 		MB_OnLeaveRoom(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_GET_FINISHED:
		MB_OnGetFinished(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_ASK_ENTERGAME:
		MB_OnAskEnterGame(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_REFRESH_MEMBER:
		MB_OnRefreshMember(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_GET_GAMEVERISON:
		MB_OnGetGameVersion(lpContext, lpRequest, pThreadCxt);
		break;
	///////////////////////////////////////////////////////////////////////
	///////        约牌系统      //////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	case YR_GET_QUANTABLE:
		YP_OnGetQuanTable(lpContext, lpRequest, pThreadCxt);
		break;
	case YR_CHAT_TO_QUAN:
		YP_OnChatToQuan(lpContext, lpRequest, pThreadCxt);
 		break;
	case YR_ENTER_ROOM:
		YP_OnEnterRoom(lpContext, lpRequest, pThreadCxt);
 		break;
	case YR_GET_QUANTABLEEX:
		YP_OnGetQuanTableEx(lpContext, lpRequest, pThreadCxt);
		break;
	case YR_GET_QUANTABLEEX_FETCHED:
		YP_OnGetQuanTableExFetched(lpContext, lpRequest, pThreadCxt);
		break;
	case YR_GET_QUAN_GAMESET:
		YP_OnGetQuanGameSet(lpContext, lpRequest, pThreadCxt);
		break;
	case YR_SET_QUAN_GAMESET:
		YP_OnSetQuanGameSet(lpContext, lpRequest, pThreadCxt);
		break;
	case YR_SET_QUAN_GAMESET_OK:
		YP_OnSetQuanGameSetOK(lpContext, lpRequest, pThreadCxt);
		break;
//	case YR_GET_STARTED:
//		YP_OnGetStarted(lpContext, lpRequest, pThreadCxt);
// 		break;
	// admin request
	case GR_CLEAR_TABLE:
		UwlTrace(_T("GR_CLEAR_TABLE requesting..."));
		OnClearTable(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_CLEAR_PROXYIPS:
		OnClearProxyIPs(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_SAVE_PROXYIPS:
		OnSaveProxyIPs(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_KICKOFF_ROOMDEAD://定时器踢僵死(KickoffRoom.exe)
		UwlTrace(_T("GR_KICKOFF_ROOMDEAD requesting..."));
		OnKickoffRoomDead(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_SET_RANDOM_TABLE:
		UwlTrace(_T("GR_SET_RANDOM_TABLE requesting..."));
		OnSetRandomTable(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_KICKOFF_ROOM_PLAYER:
		UwlTrace(_T("GR_KICKOFF_ROOM_PLAYER requesting..."));
		OnKickoffRoomPlayer(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_KICKOFF_WALKAROUND:
		UwlTrace(_T("GR_KICKOFF_WALKAROUND requesting..."));
		OnKickoffWalkAround(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_WRITE_PROFILE:
		UwlTrace(_T("GR_WRITE_PROFILE requesting..."));
		OnWriteProfile(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_GET_PROFILE:
		UwlTrace(_T("GR_GET_PROFILE requesting..."));
		OnGetProfile(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_GET_LOGFILE_SIZE:
		UwlTrace(_T("GR_GET_LOGFILE_SIZE requesting..."));
		OnGetLogfileSize(lpContext, lpRequest, pThreadCxt);
		break;
 	case GR_SYSMSG_TO_ROOM:
		UwlTrace(_T("GR_SYSMSG_TO_ROOM requesting..."));
		OnSysMsgToRoom(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_ADMINMSG_TO_ROOM:
		UwlTrace(_T("GR_ADMINMSG_TO_ROOM requesting..."));
		OnAdminMsgToRoom(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_KICKOFF_BYEXEVER:
		UwlTrace(_T("GR_KICKOFF_BYEXEVER requesting..."));
		OnKickoffByExeVer(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_CLEAR_ROOM_CHAT:
		UwlTrace(_T("GR_CLEAR_ROOM_CHAT requesting..."));
		OnClearRoomChat(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_GET_CLIENT_COUNT:
		UwlTrace(_T("GR_GET_CLIENT_COUNT requesting..."));
		OnGetClientCount(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_GET_PLAYER_COUNT:
		UwlTrace(_T("GR_GET_PLAYER_COUNT requesting..."));
		OnGetPlayerCount(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_GET_TOKEN_COUNT:
		OnGetTokenCount(lpContext, lpRequest, pThreadCxt);
 		break;
	case GR_GET_CLIENT_ID:
		UwlTrace(_T("GR_GET_CLIENT_ID requesting..."));
		OnGetClientID(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_GET_ROOM_CAPACITY:
		OnGetRoomCapacity(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_GET_ROOMSVR_CONFIG:
		UwlTrace(_T("GR_GET_ROOMSVR_CONFIG requesting..."));
		OnGetRoomSvrConfig(lpContext, lpRequest, pThreadCxt);
 		break;
	case GR_GET_MEMORYSTATUS:
		UwlTrace(_T("GR_GET_MEMORYSTATUS requesting..."));
		OnGetMemoryStatus(lpContext, lpRequest, pThreadCxt);
 		break;
	case GR_GET_NOBUFFER_COUNT:
		UwlTrace(_T("GR_GET_NOBUFFER_COUNT requesting..."));
		OnGetNoBufferCount(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_CLEAN_STALES:
		OnCleanStales(lpContext, lpRequest, pThreadCxt);
 		break;
	case GR_GET_PLAYERS:
		OnGetPlayers(lpContext, lpRequest, pThreadCxt);
 		break;
	case GR_GET_ONEPLAYER:
		OnGetOnePlayer(lpContext, lpRequest, pThreadCxt);
 		break;
	case GR_ADD_MUTEX_HARD:
		UwlTrace(_T("GR_ADD_MUTEX_HARD requesting..."));
		OnAddMutexHard(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_DEL_MUTEX_HARD:
		UwlTrace(_T("GR_DEL_MUTEX_HARD requesting..."));
		OnDelMutexHard(lpContext, lpRequest, pThreadCxt);
		break;
  	case IR_TRANSMIT_NOTIFY:
		UwlTrace(_T("IR_TRANSMIT_NOTIFY requesting..."));
		OnTransmitNotify(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_MATCH_ADMIN:
		OnMatchAdmin(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_MATCH_LEAVEROOM:
		UwlTrace(_T("GR_MATCH_LEAVEROOM requesting..."));
		OnMatchLeaveRoom(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_ASK_ENTERMATCH:
		OnAskEnterMatch(lpContext, lpRequest, pThreadCxt);
 		break;
	case GR_GET_CLIENTINFO:
		OnGetClientInfo(lpContext, lpRequest, pThreadCxt);
 		break;
	case GR_ADMINMSG_TO_ROOMSVR:
		OnAdminMsgToRoomSvr(lpContext, lpRequest, pThreadCxt);
		break;
 	default:
		UwlTrace(_T("unsupport requesting..."));
		OnUnsupported(lpContext, lpRequest, pThreadCxt);
		break;
	}

	UwlClearRequest(lpRequest);

#if defined(_UWL_TRACE) | defined(UWL_TRACE)
	DWORD dwTimeEnd = GetTickCount();
#else
	DWORD dwTimeEnd = 0;
#endif
	UwlTrace(_T("request process time costs: %d ms"), dwTimeEnd - dwTimeStart);
	UwlTrace(_T("----------------------end of request process---------------------\r\n"));
// }
// catch(...)
// {
// 	LPREQUEST		lpReq  = LPREQUEST(lpParam2);
//  	UwlLogFile("catch error!!! reqid=%ld,DataLen=%ld",lpReq->head.nRequest,lpReq->nDataLen );
//  
// }
	return TRUE;
}

BOOL CSockServer::OnEnterRoomVerified(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{   
  	REQUEST response;
	memset(&response, 0, sizeof(response));

	LPENTER_ROOM_EX lpEnterRoomEx = LPENTER_ROOM_EX(PBYTE(lpRequest->pDataPtr));
	int nRoomID=lpEnterRoomEx->er.nRoomID;
	int nUserID=lpEnterRoomEx->er.nUserID;
	int nGameID=lpEnterRoomEx->er.nGameID;

	int nForbidExpiration=0; //禁止进入房间到期时间
	CBaseRoomData* lpRoomData = NULL;

	{
		CAutoLock lock(&g_csTokenRoomSock);
		ROOM_SOCK rs;
		ZeroMemory(&rs,sizeof(ROOM_SOCK));
		rs.sock=lpContext->hSocket;
		rs.token=lpContext->lTokenID;
		rs.nRoomID=nRoomID;
		g_mapTokenRoomSock.SetAt(rs.token,  rs);
	}
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup( nRoomID, lpRoomData)){
	
		}
	}

	if(lpRoomData){

		CAutoLock lock(&(lpRoomData->m_csRoomData));

		if(lpRoomData->roomdata.nGameID!=nGameID)
		{
			response.head.nRequest = UR_INVALID_PARAM;
			goto SendResponse;
		}


		LPPLAYER pPlayer = NULL;
		lpRoomData->m_mapPlayer.Lookup( nUserID, pPlayer);
  
		if(pPlayer==NULL)//新进入
		{
			if(!IS_BIT_SET(lpEnterRoomEx->nUserType ,USER_TYPE_ADMIN))//不是网管
			{
				if(!IS_BIT_SET(lpEnterRoomEx->nUserType,USER_TYPE_MEMBER) && !IS_BIT_SET(lpEnterRoomEx->nUserType,USER_TYPE_HANDPHONE))
				{
					if(lpRoomData->m_mapPlayer.GetCount() >= lpRoomData->roomdata.nMaxUsers)
					{
						response.head.nRequest = GR_ROOM_FULL;
						goto SendResponse;
					}
				}
				nForbidExpiration =lpRoomData->IsForbidPlayer(lpEnterRoomEx->er.nUserID);
				if(nForbidExpiration)
				{
					response.head.nRequest = GR_FORBID_ENTERROOM;
					goto SendResponse;

				}

				if(IS_BIT_SET(lpRoomData->roomdata.dwOptions ,ROOM_OPT_FORBIDSAMEHARDID))
				{
					if(lpRoomData->FindPlayerHardID(lpEnterRoomEx->er.szHardID))
					{
 						response.head.nRequest = GR_FORBID_SAMEHARDID;
						goto SendResponse;

					}
				}
				if(IS_BIT_SET(lpRoomData->roomdata.dwOptions ,ROOM_OPT_FORBIDSAMEIPINROOM))
				{
					if(lpRoomData->FindPlayerIP(lpEnterRoomEx->er.dwIPAddr))
					{
 						response.head.nRequest = GR_FORBID_SAMEIPINROOM;
						goto SendResponse;

					}
				}
			}
		}
		else //已存在
		{			
			SOCKET sock = 0;
			LONG token = 0;
			if(GetSocketTokenIDByUserID(lpRoomData,  nUserID, sock, token))
			{
				lpRoomData->m_mapUserSock.RemoveKey(nUserID);
 				lpRoomData->m_mapTokenUserSock.RemoveKey(token);
 				g_pSockServer->CloseClient(sock, token);
				RemoveTokenRoomSock(token);
			}
			if(pPlayer->nStatus != PLAYER_STATUS_PLAYING)
			{			
				//Add on 20151028 on xi'an 
				if (lpRoomData->IsSoloRoom() && (PLAYER_STATUS_WAITING==pPlayer->nStatus || PLAYER_STATUS_LOOKON==pPlayer->nStatus))
				{					
					//玩家如果已经进入了游戏，是waiting状态，补发一个离开消息，让游戏服务端踢掉该玩家
					lpRoomData->PostLeaveRoom(nUserID);	//由于没有合适的消息，就发送这个消息达到踢客户端的目的
					UwlLogFile(_T("Player EnterRoom again but his status is waiting in room(%d). Take him(%d) away from game."), nRoomID, nUserID);
				}
				//Add end
				
				RemoveOnePlayer(lpRoomData,pPlayer,lpContext->hSocket, lpContext->lTokenID);				
			}
			else// 断线续玩 
			{
				DWORD dwClientIP= lpEnterRoomEx->er.dwIPAddr;
				if(!IS_BIT_SET(lpEnterRoomEx->er.dwEnterFlags,FLAG_ENTERROOM_HANDPHONE))//非手机用户重新获取IP
					dwClientIP=GetClientAddress(lpContext->hSocket, lpContext->lTokenID);

				// 该房间禁止IP相同的人一桌
				int nIPConfig = 0;
				if(IS_BIT_SET(lpRoomData->roomdata.dwOptions ,ROOM_OPT_FORBIDSAMEIP))
 					nIPConfig = 4; 
 				if(IS_BIT_SET(lpRoomData->roomdata.dwOptions ,ROOM_OPT_FORBIDSAME3IP))
 					nIPConfig = 3; 
 				if(IS_BIT_SET(lpRoomData->roomdata.dwOptions ,ROOM_OPT_FORBIDSAME2IP))
 					nIPConfig = 2; 
 
				if(nIPConfig){
					LPTABLE lpTb = NULL;
					if(lpRoomData->m_mapTable.Lookup(pPlayer->nTableNO, lpTb) && lpTb){
						for(int i = 0; i < MAX_CHAIR_COUNT; i++){
							if(0 == lpTb->nPlayerAry[i] || lpTb->nPlayerAry[i] ==  nUserID) continue;
							if(UwlIsSameIPAddr(lpTb->dwIPAddrs[i], dwClientIP, nIPConfig)){
								response.head.nRequest = GR_IP_FORBIDDEN;		
								goto SendResponse;
							}
							for(int j = 0; j < MAX_CHAIR_COUNT; j++){
								if(j == i || 0 == lpTb->nPlayerAry[j] || lpTb->nPlayerAry[j] ==  nUserID) continue;
								if(UwlIsSameIPAddr(lpTb->dwIPAddrs[i], lpTb->dwIPAddrs[j], nIPConfig)){
									response.head.nRequest = GR_IP_FORBIDDEN;		
									goto SendResponse;
								}
							}
						}
					}
				}


				//////////////////////////////////////////////////////////////////////////
				USER_SOCK us;
				ZeroMemory(&us,sizeof(USER_SOCK));
				us.sock=lpContext->hSocket;
				us.token=lpContext->lTokenID;
				us.nUserID= nUserID;
				us.dwEnterFlags=lpEnterRoomEx->er.dwEnterFlags;
			 	lpRoomData->m_mapUserSock.SetAt(us.nUserID,us);
 				lpRoomData->m_mapTokenUserSock.SetAt(us.token,us );
 
				pPlayer->pe.dwPulseTime = GetTickCount();
				pPlayer->pe.dwLatestTime = GetTickCount();
				pPlayer->pe.dwIPAddr =dwClientIP;
 				pPlayer->pe.lTokenID=lpContext->lTokenID;
				pPlayer->pe.dwEnterFlags=lpEnterRoomEx->er.dwEnterFlags;
 				pPlayer->nUserType=lpEnterRoomEx->nUserType;

				if(lstrcmpi(pPlayer->pe.szHardID,lpEnterRoomEx->er.szHardID))
				{
					UwlLogFile(_T("===========DXXW hardid differ.userid=%d,hardid1=%s,hardid2=%s"), pPlayer->nUserID,pPlayer->pe.szHardID,lpEnterRoomEx->er.szHardID);
 
				}
 			 	lstrcpyn(pPlayer->pe.szHardID,lpEnterRoomEx->er.szHardID,MAX_HARDID_LEN);
				
				lpRoomData->PostHardAndToken(nUserID,pPlayer->pe.szHardID,lpContext->lTokenID);


				LPTABLE lpTable = NULL;
				if(lpRoomData->m_mapTable.Lookup(pPlayer->nTableNO, lpTable) && lpTable)
				{
					if(pPlayer->nChairNO>=0 &&  pPlayer->nChairNO < MAX_CHAIR_COUNT)
 						lpTable->dwBreakCount &=  ~(1 << pPlayer->nChairNO );	//	 x and not (1 shl (k-1)) 把右数第k位变成0

				}
				int nPlayerMinSize=(sizeof(PLAYER)-sizeof(PLAYER_EXTEND));

				BOOL bLookDark=IS_BIT_SET(lpEnterRoomEx->er.dwEnterFlags,FLAG_ENTERROOM_LOOKDARK);

				ENTER_ROOM_OK enterroomOK;
				ZeroMemory(&enterroomOK,sizeof(enterroomOK));
				enterroomOK.nPlayerCount = lpRoomData->m_mapPlayer.GetCount();
				enterroomOK.nTableCount=lpRoomData->roomdata.nTableCount;
		    	enterroomOK.nRoomTokenID=pPlayer->pe.lTokenID;
			   	enterroomOK.nRoomPulseInterval=60;
				enterroomOK.dwClientIP=dwClientIP;

				if(lpRoomData->IsMatchRoom())//玩家
				{
// 					enterroomOK.dwEnterOKFlag = FLAG_ENTERROOMOK_MATCH;
// 					enterroomOK.nTableCount=1;//比赛系统只处理1张桌子
// 					int nLen  = sizeof(enterroomOK) +  nSizeofPlayer;
// 					PBYTE pData = new BYTE[nLen];
// 					ZeroMemory(pData, nLen);
//  				memcpy(pData, &enterroomOK, sizeof(enterroomOK));
// 					memcpy(pData+sizeof(enterroomOK),pPlayer,nSizeofPlayer);
// 				
// 					response.pDataPtr = pData;
// 					response.nDataLen = nLen;
// 					response.head.nRequest = GR_ENTER_MATCHROOM_OK;

// 					goto SendResponse;
 
				}
				else if(lpRoomData->IsCloakingRoom())
				{
 					CString sRoomID=xyConvertIntToStr(nRoomID);
					CLOAKING_DETAIL cloakingdetail;
					ZeroMemory(&cloakingdetail,sizeof(cloakingdetail));
				
					cloakingdetail.nPlayerCount=enterroomOK.nPlayerCount;
 				    lpRoomData->FillCloakingDetail(&cloakingdetail);

					if(IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_HANDPHONE) && lpRoomData->IsSupportMobile() )//本房间支持手机进入
					{
						enterroomOK.dwEnterOKFlag = FLAG_ENTERROOMOK_MOBILE;
						enterroomOK.dwEnterOKFlag |= FLAG_ENTERROOMOK_CLOAKING;
					}
					else
					{
						enterroomOK.dwEnterOKFlag |= FLAG_ENTERROOMOK_CLOAKING;
						
						enterroomOK.dwEnterOKFlag|=(lpRoomData->IsSoloRoom()?FLAG_ENTERROOMOK_SOLO:0);
						enterroomOK.dwEnterOKFlag|=(lpRoomData->IsVariableChair()?FLAG_ENTERROOMOK_VARI:0);
						enterroomOK.dwEnterOKFlag|=(lpRoomData->IsRandomRoom()?FLAG_ENTERROOMOK_RANDOM:0);
						if(pPlayer->nTimeCost / 60  >= lpRoomData->GetUnwelcomeExperience())
							enterroomOK.dwEnterOKFlag|=(lpRoomData->IsGetUnwelcome()?FLAG_ENTERROOMOK_UNWELCOME:0);
						enterroomOK.dwEnterOKFlag|=(lpRoomData->IsGetClientInfo()?FLAG_ENTERROOMOK_CLIENTINFO:0);
					}

					int nLen  = sizeof(enterroomOK) + nPlayerMinSize + sizeof(cloakingdetail);
					PBYTE pData = new BYTE[nLen];
					ZeroMemory(pData, nLen);
					memcpy(pData, &enterroomOK, sizeof(enterroomOK));
					memcpy(pData+sizeof(enterroomOK),pPlayer,nPlayerMinSize);
					memcpy(pData+sizeof(enterroomOK)+nPlayerMinSize,&cloakingdetail,sizeof(cloakingdetail));
					response.pDataPtr = pData;
					response.nDataLen = nLen;	
					response.head.nRequest = GR_ENTER_CLOAKINGROOM_OK;
					goto SendResponse;
				}
				else if(lpRoomData->IsDarkRoom())//玩家
				{
			    	enterroomOK.dwEnterOKFlag = FLAG_ENTERROOMOK_DARK;

					int nLen  = sizeof(enterroomOK) +  nPlayerMinSize;
					PBYTE pData = new BYTE[nLen];
					ZeroMemory(pData, nLen);
 					memcpy(pData, &enterroomOK, sizeof(enterroomOK));
					memcpy(pData+sizeof(enterroomOK),pPlayer,nPlayerMinSize);
				
					response.pDataPtr = pData;
					response.nDataLen = nLen;
					response.head.nRequest = GR_ENTER_DARKROOM_OK;

					goto SendResponse;
				}

				else//玩家
				{
					if(IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_HANDPHONE) && lpRoomData->IsSupportMobile() )//本房间支持手机进入
					{
						if(IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_YUEPAI) && lpRoomData->IsYuePaiRoom() )//进入约牌房间
						{
							int nTableNO=-1;
							enterroomOK.nPlayerCount=lpRoomData->GetQuanPlayerCount(pPlayer->pe.nQuanID,nTableNO);//本圈人数
							enterroomOK.nActiveTableCount=(nTableNO==-1 ? 0 : 1);
							int nLen = sizeof(enterroomOK) + enterroomOK.nPlayerCount * nPlayerMinSize  ;
							
							PBYTE pData = new BYTE[nLen];
							ZeroMemory(pData, nLen);
							
							memcpy(pData, &enterroomOK, sizeof(enterroomOK));
							lpRoomData->CopyQuanPlayer(pData + sizeof(enterroomOK), pPlayer->pe.nQuanID);
							
							response.pDataPtr = pData;
							response.nDataLen = nLen;
							response.head.nRequest = GR_ENTER_ROOM_OK;
							
							goto SendResponse;
						}
						else
						{
 						enterroomOK.dwEnterOKFlag = FLAG_ENTERROOMOK_MOBILE;

						int nLen  = sizeof(enterroomOK) +  nPlayerMinSize;
						PBYTE pData = new BYTE[nLen];
						ZeroMemory(pData, nLen);
 						memcpy(pData, &enterroomOK, sizeof(enterroomOK));
						memcpy(pData+sizeof(enterroomOK),pPlayer,nPlayerMinSize);
					
						response.pDataPtr = pData;
						response.nDataLen = nLen;
						response.head.nRequest = GR_ENTER_ROOM_OK;
					
						goto SendResponse;
						}
					}
					else
					{
						int nBeginRandomTableNO=lpRoomData->GetBeginRandomTableNO();
					
						int nLen = 0;
 						int tablecount = 0;
						int tableno = 0;
						LPTABLE pTable = NULL;
						POSITION pos  = lpRoomData->m_mapTable.GetStartPosition();
						while (pos)	{
							lpRoomData->m_mapTable.GetNextAssoc(pos, tableno, pTable);
							if(tableno>=nBeginRandomTableNO)
								continue;
							if(pTable && pTable->th.nPlayerCount){
								nLen += (pTable->th.nPlayerCount * sizeof(PLAYER_POS));
								nLen += (pTable->th.nVisitorCount * sizeof(VISITOR_POS));
								tablecount++;
							}
						}
						
				 		enterroomOK.dwEnterOKFlag|=(lpRoomData->IsSoloRoom()?FLAG_ENTERROOMOK_SOLO:0);
				 		enterroomOK.dwEnterOKFlag|=(lpRoomData->IsVariableChair()?FLAG_ENTERROOMOK_VARI:0);
 						enterroomOK.dwEnterOKFlag|=(lpRoomData->IsRandomRoom()?FLAG_ENTERROOMOK_RANDOM:0);
						if(pPlayer->nTimeCost / 60  >= lpRoomData->GetUnwelcomeExperience())
 							enterroomOK.dwEnterOKFlag|=(lpRoomData->IsGetUnwelcome()?FLAG_ENTERROOMOK_UNWELCOME:0);
 						enterroomOK.dwEnterOKFlag|=(lpRoomData->IsGetClientInfo()?FLAG_ENTERROOMOK_CLIENTINFO:0);

 						enterroomOK.nActiveTableCount = tablecount;
 
						nLen += sizeof(enterroomOK) + enterroomOK.nPlayerCount * nPlayerMinSize	+ tablecount * sizeof(TABLE_HEAD);
						PBYTE pData = new BYTE[nLen];
						ZeroMemory(pData, nLen);

						memcpy(pData, &enterroomOK, sizeof(enterroomOK));

						lpRoomData->CopyPlayerMap(pData + sizeof(enterroomOK),  bLookDark);
						lpRoomData->CopyTableMapEx(pData + sizeof(enterroomOK) + enterroomOK.nPlayerCount * nPlayerMinSize,pPlayer);
 					
						response.pDataPtr = pData;
						response.nDataLen = nLen;
						response.head.nRequest = GR_ENTER_ROOM_OK;
					
						goto SendResponse;
					}
				}
			}
		}
		//移动端版本检查
		if (IS_BIT_SET(lpEnterRoomEx->er.dwEnterFlags,FLAG_ENTERROOM_HANDPHONE) && lpRoomData->IsSupportMobile() )
		{
			CString sIniFile=lpRoomData->GetGameSvrIniFile();
			TCHAR szVersion[MAX_VERSION_LEN];
			ZeroMemory(szVersion,sizeof(szVersion));
			int nHallBuildNO = 0;
			bool bEnableVersionRange = GetPrivateProfileInt (_T("clientversion"),_T("enableVersionRange"),0,sIniFile) == 1;
			
			if(!bEnableVersionRange)
			{
				if (IS_BIT_SET(lpEnterRoomEx->er.dwClientFlags, FLAG_CLIENTINFO_IOSPHONE))
				{
					GetPrivateProfileString (_T("clientversion"),_T("IPHONE"), _T(""),szVersion, MAX_VERSION_LEN,sIniFile) ; 
					nHallBuildNO = GetPrivateProfileInt (_T("clientversion"),_T("IPHONEHallBuildNO"),0,sIniFile);
				}
				else if (IS_BIT_SET(lpEnterRoomEx->er.dwClientFlags, FLAG_CLIENTINFO_IOSPAD))
				{
					GetPrivateProfileString (_T("clientversion"),_T("IOSPAD"), _T(""),szVersion, MAX_VERSION_LEN,sIniFile) ; 
					nHallBuildNO = GetPrivateProfileInt (_T("clientversion"),_T("IOSPADHallBuildNO"),0,sIniFile);
				}
				else
				{
					GetPrivateProfileString (_T("clientversion"),_T("AND"), _T(""),szVersion, MAX_VERSION_LEN,sIniFile) ; 
					nHallBuildNO = GetPrivateProfileInt (_T("clientversion"),_T("ANDHallBuildNO"),0,sIniFile);
				}
				if(lstrlen(szVersion) > 0 )
				{
					TCHAR *fields[4];
					memset(fields,0,sizeof(fields));
					TCHAR *p1, *p2;
					p1=szVersion;
					xyRetrieveFields2 ( p1, fields, 4, &p2,'.' ) ;
					int nMajorVer= atoi(fields[0]);
					int nMinorVer= atoi(fields[1]);
					int nBuildNO = atoi(fields[2]);

					if(lpEnterRoomEx->nHallBuildNO<nHallBuildNO)
					{
						response.head.nRequest = GR_OLD_HALLBUILDNO;
						int nLen  = sizeof(int);
						PBYTE pData = new BYTE[nLen];
						ZeroMemory(pData, nLen);
						memcpy(pData, &nHallBuildNO, sizeof(int));
						response.pDataPtr = pData;
						response.nDataLen = nLen;
						goto SendResponse;
					}
					if(lpEnterRoomEx->er.nExeMajorVer<nMajorVer)
					{
						response.head.nRequest = GR_OLD_EXEMAJORVER;
						int nLen  = sizeof(int)*2;
						PBYTE pData = new BYTE[nLen];
						ZeroMemory(pData, nLen);
						memcpy(pData, &nMajorVer, sizeof(int));
						memcpy(pData+sizeof(int), &nMinorVer, sizeof(int));
						response.pDataPtr = pData;
						response.nDataLen = nLen;
						goto SendResponse;
					}
					if(lpEnterRoomEx->er.nExeMajorVer==nMajorVer)
					{
						if(lpEnterRoomEx->er.nExeMinorVer<nMinorVer)
						{
							response.head.nRequest = GR_OLD_EXEMINORVER;
							int nLen  = sizeof(int);
							PBYTE pData = new BYTE[nLen];
							ZeroMemory(pData, nLen);
							memcpy(pData, &nMinorVer, sizeof(int));
							response.pDataPtr = pData;
							response.nDataLen = nLen;
							goto SendResponse;
						}
					}
					if(IS_BIT_SET(lpEnterRoomEx->er.dwEnterFlags, FLAG_ENTERROOM_EXEBUILDNO) 
						&& lpEnterRoomEx->er.nExeMajorVer==nMajorVer
						&& lpEnterRoomEx->er.nExeMinorVer==nMinorVer)
					{
						if(lpEnterRoomEx->er.nExeBuildno<nBuildNO)
						{
							response.head.nRequest = GR_OLD_EXEBUILDNO;
							int nLen  = sizeof(int);
							PBYTE pData = new BYTE[nLen];
							ZeroMemory(pData, nLen);
							memcpy(pData, &nBuildNO, sizeof(int));
							response.pDataPtr = pData;
							response.nDataLen = nLen;
							goto SendResponse;
						}
					}
				}
			}
			else
			{
				TCHAR szLVersion[MAX_VERSION_LEN];
				TCHAR szHVersion[MAX_VERSION_LEN];
				ZeroMemory(szLVersion,sizeof(szLVersion));
				ZeroMemory(szHVersion,sizeof(szHVersion));
				if (IS_BIT_SET(lpEnterRoomEx->er.dwClientFlags, FLAG_CLIENTINFO_IOSPHONE))
				{
					GetPrivateProfileString (_T("clientversion"),_T("IPHONE_Low"), _T(""),szLVersion, MAX_VERSION_LEN,sIniFile) ; 
					GetPrivateProfileString (_T("clientversion"),_T("IPHONE_High"), _T(""),szHVersion, MAX_VERSION_LEN,sIniFile) ;
				
					nHallBuildNO = GetPrivateProfileInt (_T("clientversion"),_T("IPHONEHallBuildNO"),0,sIniFile);
				}
				else if (IS_BIT_SET(lpEnterRoomEx->er.dwClientFlags, FLAG_CLIENTINFO_IOSPAD))
				{
					GetPrivateProfileString (_T("clientversion"),_T("IOSPAD_Low"), _T(""),szLVersion, MAX_VERSION_LEN,sIniFile) ; 
					GetPrivateProfileString (_T("clientversion"),_T("IOSPAD_High"), _T(""),szHVersion, MAX_VERSION_LEN,sIniFile) ;
				
					nHallBuildNO = GetPrivateProfileInt (_T("clientversion"),_T("IOSPADHallBuildNO"),0,sIniFile);
				}
				else
				{
					GetPrivateProfileString (_T("clientversion"),_T("AND_Low"), _T(""),szLVersion, MAX_VERSION_LEN,sIniFile) ; 
					GetPrivateProfileString (_T("clientversion"),_T("AND_High"), _T(""),szHVersion, MAX_VERSION_LEN,sIniFile) ;
				
					nHallBuildNO = GetPrivateProfileInt (_T("clientversion"),_T("ANDHallBuildNO"),0,sIniFile);
				}

				if(lstrlen(szLVersion) > 0 && lstrlen(szHVersion) > 0)
				{
					TCHAR *LowFields[4];
					memset(LowFields,0,sizeof(LowFields));
					TCHAR *p1, *p2;
					p1=szLVersion;
					xyRetrieveFields2 ( p1, LowFields, 4, &p2,'.' ) ;
					int nLMajorVer= atoi(LowFields[0]);
					int nLMinorVer= atoi(LowFields[1]);
					int nLBuildNO = atoi(LowFields[2]);

					TCHAR *HighFields[4];
					memset(HighFields,0,sizeof(HighFields));
					p1=szHVersion;
					xyRetrieveFields2 ( p1, HighFields, 4, &p2,'.' ) ;
					int nHMajorVer= atoi(HighFields[0]);
					int nHMinorVer= atoi(HighFields[1]);
					int nHBuildNO = atoi(HighFields[2]);

					if(lpEnterRoomEx->nHallBuildNO<nHallBuildNO)
					{
						response.head.nRequest = GR_OLD_HALLBUILDNO;
						int nLen  = sizeof(int);
						PBYTE pData = new BYTE[nLen];
						ZeroMemory(pData, nLen);
						memcpy(pData, &nHallBuildNO, sizeof(int));
						response.pDataPtr = pData;
						response.nDataLen = nLen;
						goto SendResponse;
					}
					if(lpEnterRoomEx->er.nExeMajorVer<nLMajorVer)
					{
						response.head.nRequest = GR_OLD_EXEMAJORVER;
						int nLen  = sizeof(int)*2;
						PBYTE pData = new BYTE[nLen];
						ZeroMemory(pData, nLen);
						memcpy(pData, &nLMajorVer, sizeof(int));
						memcpy(pData+sizeof(int), &nLMajorVer, sizeof(int));
						response.pDataPtr = pData;
						response.nDataLen = nLen;
						goto SendResponse;
					}
					if(lpEnterRoomEx->er.nExeMajorVer==nLMajorVer)
					{
						if(lpEnterRoomEx->er.nExeMinorVer<nLMinorVer)
						{
							response.head.nRequest = GR_OLD_EXEMINORVER;
							int nLen  = sizeof(int);
							PBYTE pData = new BYTE[nLen];
							ZeroMemory(pData, nLen);
							memcpy(pData, &nLMinorVer, sizeof(int));
							response.pDataPtr = pData;
							response.nDataLen = nLen;
							goto SendResponse;
						}
					}
					if(IS_BIT_SET(lpEnterRoomEx->er.dwEnterFlags, FLAG_ENTERROOM_EXEBUILDNO) 
						&& lpEnterRoomEx->er.nExeMajorVer==nLMajorVer
						&& lpEnterRoomEx->er.nExeMinorVer==nLMinorVer)
					{
						if(lpEnterRoomEx->er.nExeBuildno<nLBuildNO)
						{
							response.head.nRequest = GR_OLD_EXEBUILDNO;
							int nLen  = sizeof(int);
							PBYTE pData = new BYTE[nLen];
							ZeroMemory(pData, nLen);
							memcpy(pData, &nLBuildNO, sizeof(int));
							response.pDataPtr = pData;
							response.nDataLen = nLen;
							goto SendResponse;
						}
					}
					// 版本号过高
// 					if(lpEnterRoomEx->nHallBuildNO>nHallBuildNO)
// 					{
// 						response.head.nRequest = GR_NEW_HALLBUILDNO;
// 						int nLen  = sizeof(int);
// 						PBYTE pData = new BYTE[nLen];
// 						ZeroMemory(pData, nLen);
// 						memcpy(pData, &nHallBuildNO, sizeof(int));
// 						response.pDataPtr = pData;
// 						response.nDataLen = nLen;
// 						goto SendResponse;
// 					}
					if(lpEnterRoomEx->er.nExeMajorVer>nHMajorVer)
					{
						response.head.nRequest = GR_NEW_EXEMAJORVER;
						int nLen  = sizeof(int)*2;
						PBYTE pData = new BYTE[nLen];
						ZeroMemory(pData, nLen);
						memcpy(pData, &nHMajorVer, sizeof(int));
						memcpy(pData+sizeof(int), &nHMajorVer, sizeof(int));
						response.pDataPtr = pData;
						response.nDataLen = nLen;
						goto SendResponse;
					}
					if(lpEnterRoomEx->er.nExeMajorVer==nHMajorVer)
					{
						if(lpEnterRoomEx->er.nExeMinorVer>nHMinorVer)
						{
							response.head.nRequest = GR_NEW_EXEMINORVER;
							int nLen  = sizeof(int);
							PBYTE pData = new BYTE[nLen];
							ZeroMemory(pData, nLen);
							memcpy(pData, &nHMinorVer, sizeof(int));
							response.pDataPtr = pData;
							response.nDataLen = nLen;
							goto SendResponse;
						}
					}
					if(IS_BIT_SET(lpEnterRoomEx->er.dwEnterFlags, FLAG_ENTERROOM_EXEBUILDNO) 
						&& lpEnterRoomEx->er.nExeMajorVer==nHMajorVer
						&& lpEnterRoomEx->er.nExeMinorVer==nHMinorVer)
					{
						if(lpEnterRoomEx->er.nExeBuildno>nHBuildNO)
						{
							response.head.nRequest = GR_NEW_EXEBUILDNO;
							int nLen  = sizeof(int);
							PBYTE pData = new BYTE[nLen];
							ZeroMemory(pData, nLen);
							memcpy(pData, &nHBuildNO, sizeof(int));
							response.pDataPtr = pData;
							response.nDataLen = nLen;
							goto SendResponse;
						}
					}
				}
				
			}
		}
	}
	TransmitRequest(lpContext, lpRequest);
	return TRUE;
	
SendResponse:
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	if(GR_FORBID_ENTERROOM==response.head.nRequest)
	{
	 	response.pDataPtr=&nForbidExpiration;
	 	response.nDataLen=sizeof(int);
	}
	bSendOK = SendResponse(lpContext->hSocket, &context, &response, MSG_RESPONSE, 0, COMPRESS_ZIP);
 
	if (response.head.nRequest!=GR_FORBID_ENTERROOM)
	{
		UwlClearRequest(&response);
	}
 
	return TRUE;
}
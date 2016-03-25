#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL CGameServer::OnRequest(void* lpParam1, void* lpParam2)
{
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
	case GR_SENDMSG_TO_SERVER:
		UwlTrace(_T("发送通知给服务器 requesting..."));
		OnSendSysMsgToServer(lpContext, lpRequest, pThreadCxt);
		break; 
	case GR_SEATED_TO_LOOK_ON:
		UwlTrace(_T("GR_SEATED_TO_LOOK_ON requesting..."));
		OnChangeSeatedToLookOn(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_LOOK_ON_TO_SEATED:
		UwlTrace(_T("GR_LOOK_ON_TO_SEATED requesting..."));
		OnChangeLookOnToSeated(lpContext, lpRequest, pThreadCxt);
		break;
	default:
		UwlTrace(_T("goto default proceeding..."));
		CMainServer::OnRequest(lpParam1, lpParam2);
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

	return TRUE;
}

unsigned CGameServer::MessageThreadProc()
{
	UwlTrace(_T("message thread started. id = %d"), GetCurrentThreadId());
	
	xygRegMessageWnd(m_nListenPort, GAMESVR_CLSNAME_EX, DefWindowProc);
	m_hGameSvrWnd = xygCreateMessageWnd(m_nListenPort, GAMESVR_CLSNAME_EX, GAMESVR_WNDNAME_EX, m_szIniFile);
	if(!m_hGameSvrWnd){
		UwlLogFile(_T("can not create message windows. we exit."));
		return 0;
	}
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)){
		if(m_hGameSvrWnd != msg.hwnd) continue; //
		if (WM_RTG_RECONNECTSVR_EX==msg.message)
		{
			int adminid=int(msg.wParam);
			OnReconnectSvr(adminid);
		}
		else if(WM_RTG_GETFINISHED_EX == msg.message){
			int roomid  = 0;
			int tableno = 0;
			int chairno = 0;
			int userid  = int(msg.lParam);
			ParseRoomTableChair(msg.wParam,roomid,tableno,chairno);
			OnGetFinished(roomid, tableno, chairno, userid);
		}else if(WM_RTG_KICKOFFPLAYER_EX == msg.message)
		{
			int roomid  = 0;
			int tableno = 0;
			int chairno = 0;
			int userid  = int(msg.lParam);
			ParseRoomTableChair(msg.wParam,roomid,tableno,chairno);
			OnKickOffPlayer(roomid,tableno,chairno,userid);
		}
		else if(WM_RTG_HARDCODE_EX == msg.message){
			DWORD hardcode = DWORD(msg.wParam);
			int userid = int(msg.lParam);
			OnHardCode(userid, hardcode);
		}else if(WM_RTG_TOKENID_EX == msg.message){
			int tokenid = int(msg.wParam);
			int userid = int(msg.lParam);
			OnRoomTokenID(userid, tokenid);
		}else if(WM_RTG_EXPERIENCE_EX == msg.message){
			int experience = int(msg.wParam);
			int userid = int(msg.lParam);
			OnExperience(userid, experience);
		}else if(WM_RTG_ROOMSVRHWND_EX == msg.message){
			int roomid = int(msg.wParam);
			HWND hwnd = HWND(msg.lParam);
			OnRoomSvrHWnd(roomid, hwnd);
		}else if(WM_RTG_ROOMOPTIONS_EX == msg.message){
			int roomid = int(msg.wParam);
			int option = int(msg.lParam);
			OnRoomOption(roomid, option);
		}else if(WM_RTG_ROOMCONFIGS_EX == msg.message){
			int roomid = int(msg.wParam);
			int config = int(msg.lParam);
			OnRoomConfig(roomid, config);
		}else if(WM_RTG_ROOMMANAGES_EX == msg.message){
			int roomid = int(msg.wParam);
			int config = int(msg.lParam);
			OnRoomManage(roomid, config);
		}else if(WM_RTG_GAMEOPTIONS_EX == msg.message){
			int roomid = int(msg.wParam);
			int config = int(msg.lParam);
			OnGameOption(roomid, config);
		}else if(WM_RTG_GAMEPARAM_EX == msg.message){
			int roomid = int(msg.wParam);
			int config = int(msg.lParam);
			OnGameParam(roomid, config);
		}else if(WM_RTG_GAMEDATA_EX == msg.message){
			int roomid = int(msg.wParam);
			int config = int(msg.lParam);
			OnGameData(roomid, config);
		}else if(WM_RTG_MINPLAYSCORE_EX == msg.message){
			int roomid = int(msg.wParam);
			int min_playscore = int(msg.lParam);
			OnMinPlayScore(roomid, min_playscore);
		}else if(WM_RTG_MAXPLAYSCORE_EX == msg.message){
			int roomid = int(msg.wParam);
			int max_playscore = int(msg.lParam);
			OnMaxPlayScore(roomid, max_playscore);
		}else if (WM_RTG_MINDEPOSIT_EX == msg.message){
			int roomid = int(msg.wParam);
			int min_deposit = int(msg.lParam);
			OnMinDeposit(roomid,min_deposit);
		}
		else if (WM_RTG_MAXDEPOSIT_EX == msg.message){
			int roomid = int(msg.wParam);
			int max_deposit = int(msg.lParam);
			OnMaxDeposit(roomid,max_deposit);
		}else if(WM_RTG_CLEARTABLE_EX == msg.message){
			int roomid = int(msg.wParam);
			int tableno = int(msg.lParam);
			OnClearTable(roomid, tableno);
		}else if(WM_RTG_ROOMTABLECHAIR_EX == msg.message){
			DWORD roomtablechair = DWORD(msg.wParam);
			int userid = int(msg.lParam);
			OnRoomTableChair(roomtablechair, userid);
		}else if(WM_RTG_LEAVEROOM_EX == msg.message){
			int roomid = int(msg.wParam);
			int userid = int(msg.lParam);
			OnLeaveRoom(roomid, userid);
		}else if(WM_RTG_STARTVALID_EX==msg.message)
		{
			int roomid  = 0;
			int tableno = 0;
			int chairno = 0;
			int userid  = int(msg.lParam);
			ParseRoomTableChair(msg.wParam,roomid,tableno,chairno);
			OnVerifyStart(roomid,tableno,chairno,userid);
		}else if(WM_RTG_STARTINVALID_EX==msg.message)
		{
			int roomid  = 0;
			int tableno = 0;
			int chairno = 0;
			int userid  = int(msg.lParam);
			ParseRoomTableChair(msg.wParam,roomid,tableno,chairno);
			OnVerifyStartFaild(roomid,tableno,chairno,userid);
		}else if (WM_RTG_STARTSOLOTABLE_EX==msg.message)
		{
			int roomid = int(msg.wParam);
			int tableno = int(msg.lParam);
			OnStartSoloTable(roomid, tableno);
		}else if (WM_RTG_NEWTABLECHAIR_EX==msg.message)
		{
			int roomid  = 0;
			int tableno = 0;
			int chairno = 0;
			int userid  = int(msg.lParam);
			ParseRoomTableChair(msg.wParam,roomid,tableno,chairno);
			OnNewTableChair(roomid, tableno, chairno, userid);
		}else if (WM_RTG_USERCLOTHING_EX==msg.message)
		{			
			int clothid = int(msg.wParam);
			int userid = int(msg.lParam);
			OnUserClothing(clothid, userid);			
		}else if (WM_RTG_BECOMEMEMBER_EX==msg.message)
		{
			BOOL bMember = BOOL(msg.wParam);
			int userid = int(msg.lParam);
			OnBecomeMember(bMember, userid);
		}else if (WM_RTG_VERIFY_ROOMTABLECHAIR==msg.message)
		{
			int roomid  = 0;
			int tableno = 0;
			int chairno = 0;
			int userid  = int(msg.lParam);
			ParseRoomTableChair(msg.wParam,roomid,tableno,chairno);
			OnVerifyRoomTableChair(roomid,tableno,chairno,userid);
		}else if (WM_GTR_CHANGE_SEATED_TO_LOOK_ON_RESULT_EX==msg.message)
		{
			int roomid  = 0;
			int tableno = 0;
			int chairno = 0;
			int userid  = int(msg.lParam);
			ParseRoomTableChair(msg.wParam,roomid,tableno,chairno);
			OnChangeSeatedToLookOnResult(roomid, tableno, chairno, userid);
		}else if (WM_GTR_CHANGE_LOOK_ON_TO_SEATED_RESULT_EX==msg.message)
		{
			int roomid  = 0;
			int tableno = 0;
			int chairno = 0;
			int userid  = int(msg.lParam);
			ParseRoomTableChair(msg.wParam,roomid,tableno,chairno);
			OnChangeLookOnToSeatedResult(roomid, tableno, chairno, userid);
		}else{
			DispatchMessage(&msg);
		}
	}
	if(m_hGameSvrWnd){
		DestroyWindow(m_hGameSvrWnd);
		m_hGameSvrWnd = NULL;
	}
	xygUnregMessageWnd(m_nListenPort, GAMESVR_CLSNAME_EX);
	UwlTrace(_T("message thread exiting. id = %d"), GetCurrentThreadId());
   	return 0;
} 

CTable* CGameServer::OnNewTable(int roomid, int tableno, int score_mult)
{
	CGameTable* pTable = new CGameTable(roomid, tableno, score_mult);
	return (CTable*)pTable;
}

int CGameServer::OnUserStart(CTable* pTable,int chairno)
{
	if (!pTable||chairno<0||chairno>pTable->m_nTotalChairs)
		return -1;
	
	CPlayer* pPlayer=pTable->m_ptrPlayers[chairno];
	if (!pPlayer)
		return -1;
	
	pTable->OnUserStart(chairno);
	int startcount=XygGetStartCount(pTable->m_dwUserStatus, pTable->m_nTotalChairs);
	
	if (startcount<pTable->m_nTotalChairs)
	{
		if (IsVariableChairRoom(pTable->m_nRoomID)
			&& IsAllowStartGame(pTable, startcount))
		{
			// 目前桌上所有人都按下了开始
			if (pTable->IsNeedCountdown() && !pTable->IsInCountDown())
			{
				START_COUNTDOWN  sc;
				memset(&sc,0,sizeof(START_COUNTDOWN));
				sc.nRoomID = pTable->m_nRoomID;
				sc.nTableNO = pTable->m_nTableNO;
				sc.bStartorStop = TRUE;
				sc.nCountDown = GetCountdown(pTable,pTable->m_nRoomID);
				pTable->SetCountDown(TRUE, sc.nCountDown);
				NotifyTablePlayers(pTable,GR_START_COUNTDOWN,&sc,sizeof(START_COUNTDOWN));
				NotifyTableVisitors(pTable,GR_START_COUNTDOWN,&sc,sizeof(START_COUNTDOWN));
			}
		}
		
		START_GAME sg;
		memset(&sg,0,sizeof(START_GAME));
		sg.nRoomID=pTable->m_nRoomID;
		sg.nTableNO=pTable->m_nTableNO;
		sg.nChairNO=chairno;
		sg.nUserID=pPlayer->m_nUserID;	
		NotifyTablePlayers(pTable, GR_START_GAME, &sg, sizeof(START_GAME));
		NotifyTableVisitors(pTable, GR_START_GAME, &sg, sizeof(START_GAME));
	}
	else
	{
		OnGameStarted(pTable,0);
	}
	
	
	return 1;
}

BOOL CGameServer::NotifyPlayerMsgAndResponse(LPCONTEXT_HEAD lpContext, CTable* pTable,int nDest,DWORD dwFlags,DWORD datalen,void* data)
{
	int size=datalen+sizeof(GAME_MSG);
	BYTE* pGameMsg=new BYTE[size];
	memset(pGameMsg,0,size);
	GAME_MSG* pHead=(GAME_MSG*)pGameMsg;
	BYTE* pData=pGameMsg+sizeof(GAME_MSG);
	pHead->nMsgID=dwFlags;
	pHead->nUserID=-1;
	pHead->nVerifyKey=pTable->m_nTableNO;
	pHead->nDatalen=datalen;
	if (datalen)
		memcpy(pData,data,datalen);
	
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	response.pDataPtr=pGameMsg;
	response.nDataLen=size;
	
	if (pTable->ValidateChair(nDest))
	{
		CPlayer* pPlayer = pTable->m_ptrPlayers[nDest];
		if (lpContext->lTokenID==pPlayer->m_lTokenID
			&&lpContext->bNeedEcho)
		{
			//是自己，那么回应
			lpContext->bNeedEcho=FALSE;//已经回应过不再回应
			SendUserResponse(lpContext,&response);
		}
		else
		{
			//不是发送家，那么发送
			NotifyOneUser(pPlayer->m_hSocket,pPlayer->m_lTokenID,GR_SENDMSG_TO_PLAYER,pGameMsg,size);
		}
	}
	else 
	{
		if (nDest==GAME_MSG_SEND_OTHER)
		{
			NotifyTablePlayers(pTable,  GR_SENDMSG_TO_PLAYER, pGameMsg,size,lpContext->lTokenID);
			NotifyTableVisitors(pTable, GR_SENDMSG_TO_PLAYER, pGameMsg,size,lpContext->lTokenID);
		}
		else if(nDest==GAME_MSG_SEND_OTHER_PLAYER)
		{
            NotifyTablePlayers(pTable,  GR_SENDMSG_TO_PLAYER, pGameMsg,size,lpContext->lTokenID);
		}
		else if (nDest==GAME_MSG_SEND_VISITOR)
		{
            NotifyTableVisitors(pTable, GR_SENDMSG_TO_PLAYER, pGameMsg,size);
		}
		else if (nDest==GAME_MSG_SEND_EVERY_PLAYER)
		{
			if (lpContext->bNeedEcho)
			{
				lpContext->bNeedEcho=FALSE;//已经回应过不再回应
				SendUserResponse(lpContext,&response);
                NotifyTablePlayers(pTable,  GR_SENDMSG_TO_PLAYER, pGameMsg,size,lpContext->lTokenID);
			}
			else
			{
                NotifyTablePlayers(pTable,  GR_SENDMSG_TO_PLAYER, pGameMsg,size,0);
			}
		}
		else
		{
			if (lpContext->bNeedEcho)
			{
				lpContext->bNeedEcho=FALSE;//已经回应过不再回应
				SendUserResponse(lpContext,&response);
				NotifyTablePlayers(pTable,  GR_SENDMSG_TO_PLAYER, pGameMsg,size,lpContext->lTokenID);
				NotifyTableVisitors(pTable, GR_SENDMSG_TO_PLAYER, pGameMsg,size,lpContext->lTokenID);
			}
			else
			{
				NotifyTablePlayers(pTable,  GR_SENDMSG_TO_PLAYER, pGameMsg,size,0);
				NotifyTableVisitors(pTable, GR_SENDMSG_TO_PLAYER, pGameMsg,size,0);
			}
		}
	}
	
	delete []pGameMsg;
	return TRUE;
}

BOOL CGameServer::NotifyTableMsg(CTable* pTable,int nDest,int nMsgID,int datalen,void* data,LONG tokenExcept)
{
	int size=datalen+sizeof(GAME_MSG);
	BYTE* pGameMsg=new BYTE[size];
	memset(pGameMsg,0,size);
	GAME_MSG* pHead=(GAME_MSG*)pGameMsg;
	BYTE* pData=pGameMsg+sizeof(GAME_MSG);
	pHead->nMsgID=nMsgID;
	pHead->nUserID=-1;
	pHead->nVerifyKey=pTable->m_nTableNO;
	pHead->nDatalen=datalen;
	if (datalen)
		memcpy(pData,data,datalen);
	
	if (pTable->ValidateChair(nDest))
	{
		//发送给个体
		CPlayer* pPlayer = pTable->m_ptrPlayers[nDest];
		NotifyOneUser(pPlayer->m_hSocket,pPlayer->m_lTokenID,GR_SENDMSG_TO_PLAYER,pGameMsg,size);
	}
	else
	{
		if (nDest==GAME_MSG_SEND_OTHER)
		{
			CPlayer* pPlayer = pTable->m_ptrPlayers[nDest];
			NotifyTablePlayers(pTable,  GR_SENDMSG_TO_PLAYER, pGameMsg, size,tokenExcept);
			NotifyTableVisitors(pTable, GR_SENDMSG_TO_PLAYER, pGameMsg, size,tokenExcept);
		}
		else if(nDest==GAME_MSG_SEND_OTHER_PLAYER)
		{
			CPlayer* pPlayer = pTable->m_ptrPlayers[nDest];
            NotifyTablePlayers(pTable,  GR_SENDMSG_TO_PLAYER, pGameMsg, size,tokenExcept);
		}
		else if (nDest==GAME_MSG_SEND_EVERY_PLAYER)
		{
            NotifyTablePlayers(pTable,  GR_SENDMSG_TO_PLAYER, pGameMsg, size);
		}
		else
		{
			NotifyTablePlayers(pTable,  GR_SENDMSG_TO_PLAYER, pGameMsg, size);
			NotifyTableVisitors(pTable, GR_SENDMSG_TO_PLAYER, pGameMsg, size);
		}
	}
	
	delete []pGameMsg;
	return TRUE;
}

BOOL CGameServer::NotifyResponseFaild(LPCONTEXT_HEAD lpContext)
{
	if (lpContext->bNeedEcho)
	{
		lpContext->bNeedEcho=FALSE;//已经回应过不再回应
		REQUEST response;
		memset(&response, 0, sizeof(response));
		response.head.nRequest = UR_OPERATE_FAILED;
		return SendUserResponse(lpContext, &response);
	}
	else
	{
        return TRUE;
	}
}

BOOL CGameServer::NotifyResponseSucceesd(LPCONTEXT_HEAD lpContext)
{
	if (lpContext->bNeedEcho)
	{
		lpContext->bNeedEcho=FALSE;//已经回应过不再回应
		REQUEST response;
		memset(&response, 0, sizeof(response));
		response.head.nRequest = UR_OPERATE_SUCCEEDED;
		return SendUserResponse(lpContext, &response);
	}
	else
	{
		return TRUE;
	}
}

BOOL CGameServer::OnSendSysMsgToServer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	SOCKET sock = INVALID_SOCKET;
	LONG token = 0;
	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	int cardchair = INVALID_OBJECT_ID;
	int r_id = 0;
	int t_no = INVALID_OBJECT_ID;
	int u_id = 0;
	int c_no = INVALID_OBJECT_ID;
	BOOL lookon = FALSE;
	BOOL bPassive=IS_BIT_SET(lpContext->dwFlags,CH_FLAG_SYSTEM_EJECT);//是否系统自己生成的消息
	
	CRoom* pRoom = NULL;
	CGameTable* pTable = NULL;
	CPlayer* pPlayer = NULL;
	
	sock = lpContext->hSocket;
	token = lpContext->lTokenID;

	//数据长度验证
	if (lpRequest->nDataLen<sizeof(GAME_MSG))
	{
		UwlLogFile(_T("%ld发送了不合法的消息结构，结构长度:%ld"),lpContext->lTokenID,lpRequest->nDataLen);
		return NotifyResponseFaild(lpContext);
	}
	GAME_MSG* pMsg = LPGAME_MSG(PBYTE(lpRequest->pDataPtr));
	BYTE* pData=PBYTE(lpRequest->pDataPtr)+sizeof(GAME_MSG);

	UwlTrace("OnSendSysMsgToServer SubID:%ld ...",pMsg->nMsgID);

	roomid = pMsg->nRoomID;
	userid = pMsg->nUserID;

	{
		CAutoLock lock(&m_csRoomMap);
		if(!m_mapRoom.Lookup(roomid, pRoom))
		{
			return FALSE;
		}
	}

	USER_DATA user_data;
	memset(&user_data,0,sizeof(user_data));
	
	if(pRoom)
	{
		CAutoLock lock(&(pRoom->m_csRoom));
		
		if (!m_mapUserData.Lookup(userid,user_data)
			&&bPassive==FALSE)
		{
			UwlLogFile(_T("user:%ld未在服务器注册，试图发送消息号为:%ld"),userid,pMsg->nMsgID);
			return NotifyResponseFaild(lpContext);
		}
		else
		{
			roomid=user_data.nRoomID;
			tableno=user_data.nTableNO;
			chairno=user_data.nChairNO;
		}

		if(!pRoom->m_mapTable.Lookup(tableno, (CTable*&)pTable))
		{
			return NotifyResponseFaild(lpContext);
		}
	}

	if(pTable) 
	{
		LOCK_TABLE(pTable, chairno, bPassive, userid, token);

		//消息集验证
		if (!pTable->IsGameMsg(pMsg->nMsgID))
		{
		 	UwlLogFile(_T("%ld发送了不合法的消息结构，消息号为:%ld"),token,pMsg->nMsgID);
		    return NotifyResponseFaild(lpContext);	
		}
		
		if(!pTable->IsPlayer(userid))
		{ // 不是玩家
			UwlLogFile(_T("user not player. user %ld SendMsgToPlayer Failed,dwFlags:%ld"), userid,pMsg->nMsgID);
			return NotifyResponseFaild(lpContext);
		}

		if (NULL==pTable->m_ptrPlayers[chairno] || pTable->m_ptrPlayers[chairno]->m_nUserID!=userid)
		{
			UwlLogFile("chairno is dismatch, UserID=%d", userid);
			return NotifyResponseFaild(lpContext); 
		}

	//	 try{

		switch(pMsg->nMsgID) 
		{
			case SYSMSG_GAME_CLOCK_STOP:
				{
					if (IS_BIT_SET(pTable->m_dwStatus,TS_WAITING_OPERATION)) 
					{
						if (pTable->IsOperateTimeOver()&&GetTickCount()-pTable->m_dwLastClockStop>3000)//操作操时
						{
							pTable->m_dwLastClockStop=GetTickCount();
							int offline=pTable->GetCurrentChair();
							if (pTable->CheckOffline(offline))
							{
								//OnPlayerOffline(pTable,offline);	//掉线直接弃牌，不通知客户端
								OnSeverAutoPlay(pRoom,pTable,offline);
							}
						} 
					}
				}
				break;
			case SYSMSG_PLAYER_ONLINE:
				{
					pPlayer=pTable->m_ptrPlayers[chairno];
					if (pTable->IsOffline(chairno))
					{
						//断线续完
						pTable->m_dwUserStatus[chairno]&=~US_USER_OFFLINE;
						/////////////////////////////////////////////////////////////////////////
						//掉线回来不通知客户端
						//NotifyTableMsg(pTable,GAME_MSG_SEND_OTHER,SYSMSG_RETURN_GAME,4,&pPlayer->m_nChairNO,pPlayer->m_lTokenID);
						/////////////////////////////////////////////////////////////////////////
					}
				}
				break;
			case SYSMSG_GAME_ON_AUTOPLAY:
				{
					if (!pTable->IsAutoPlay(chairno))
					{
						pTable->m_dwUserStatus[chairno]|=US_USER_AUTOPLAY;
						NotifyPlayerMsgAndResponse(lpContext,pTable,GAME_MSG_SEND_EVERYONE,SYSMSG_GAME_ON_AUTOPLAY,sizeof(int),&chairno);
					}
				}
				break;
			case SYSMSG_GAME_CANCEL_AUTOPLAY:
				{
 					if (pTable->IsAutoPlay(chairno))
					{
						pTable->m_dwUserStatus[chairno]&=~US_USER_AUTOPLAY;
						NotifyPlayerMsgAndResponse(lpContext,pTable,GAME_MSG_SEND_EVERYONE,SYSMSG_GAME_CANCEL_AUTOPLAY,sizeof(int),&chairno);
					}
				}
				break;

			//掉线玩家服务端自动出牌，过牌，请重载实现
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case GAME_MSG_SHOW_CARD:
				{
					if (UR_OPERATE_FAILED == OnShowCard(lpContext, lpRequest, chairno, pTable))
					{	
						UwlLogFile("OnLookCard failed. roomid:%ld, tableno:%ld, userid:%ld",roomid, tableno, userid);
						REQUEST response;
						memset(&response, 0, sizeof(response));
						response.head.nRequest = UR_OPERATE_FAILED;
						return SendUserResponse(lpContext, &response);
					}
				}
				break;
			case GAME_MSG_LOOK_CARD:
				{
					if (UR_OPERATE_FAILED == OnLookCard(lpContext, lpRequest, chairno, pTable))
					{	
						UwlLogFile("OnLookCard failed. roomid:%ld, tableno:%ld, userid:%ld",roomid, tableno, userid);
						REQUEST response;
						memset(&response, 0, sizeof(response));
						response.head.nRequest = UR_OPERATE_FAILED;
						return SendUserResponse(lpContext, &response);
					}
				}
				break;
			case GAME_MSG_BET:
				{
					if (UR_OPERATE_FAILED == OnBet(lpContext, lpRequest, chairno, pTable))
					{	
						UwlLogFile("OnBet failed. roomid:%ld, tableno:%ld, userid:%ld",roomid, tableno, userid);
						REQUEST response;
						memset(&response, 0, sizeof(response));
						response.head.nRequest = UR_OPERATE_FAILED;
						return SendUserResponse(lpContext, &response);
					}
				}
				break;
			case GAME_MSG_RISE:
				{
					if (UR_OPERATE_FAILED == OnRise(lpContext, lpRequest, chairno, pTable))
					{	
						UwlLogFile("OnRise failed. roomid:%ld, tableno:%ld, userid:%ld",roomid, tableno, userid);
						REQUEST response;
						memset(&response, 0, sizeof(response));
						response.head.nRequest = UR_OPERATE_FAILED;
						return SendUserResponse(lpContext, &response);
					}
				}
				break;
			case GAME_MSG_FOLLOW:
				{
					if (UR_OPERATE_FAILED == OnFollowBet(lpContext, lpRequest, chairno, pTable))
					{	
						UwlLogFile("OnFollowBet failed. roomid:%ld, tableno:%ld, userid:%ld",roomid, tableno, userid);
						REQUEST response;
						memset(&response, 0, sizeof(response));
						response.head.nRequest = UR_OPERATE_FAILED;
						return SendUserResponse(lpContext, &response);
					}
				}
				break;
			case GAME_MSG_COMPARE:
				{
					int nLoseChair = -1;
					if (UR_OPERATE_FAILED == OnCompare(lpContext, lpRequest, chairno, pTable, nLoseChair))
					{	
						UwlLogFile("OnCompare failed. roomid:%ld, tableno:%ld, userid:%ld",roomid, tableno, userid);
						REQUEST response;
						memset(&response, 0, sizeof(response));
						response.head.nRequest = UR_OPERATE_FAILED;
						return SendUserResponse(lpContext, &response);
					}

					if(TRUE == IsGameWin(pTable))
						return OnGameWin(lpContext,pRoom,pTable,chairno,FALSE,roomid);
					else if (pTable->ValidateChair(nLoseChair))
						OnUserBoutEnd(lpContext, pRoom, pTable, nLoseChair, FALSE, roomid);
				}
				break;
			case GAME_MSG_ALL_IN:
				{
					ALL_IN_RESPONSE allInResponse;
					memset(&allInResponse, 0, sizeof(ALL_IN_RESPONSE));
					allInResponse.nNextChair = -1;
					allInResponse.nChairNo = chairno;
					if (UR_OPERATE_FAILED == OnAllIn(lpContext, lpRequest, chairno, pTable, allInResponse))
					{	
						UwlLogFile("OnAllIn failed. roomid:%ld, tableno:%ld, userid:%ld",roomid, tableno, userid);
						REQUEST response;
						memset(&response, 0, sizeof(response));
						response.head.nRequest = UR_OPERATE_FAILED;
						return SendUserResponse(lpContext, &response);
					}

					if(TRUE == IsGameWin(pTable))
					{
						return OnGameWin(lpContext,pRoom,pTable,chairno,FALSE,roomid);
					}
					else 
					{
						int nTempChair = -1;
						for (int i=0; i<TOTAL_CHAIRS; i++)
						{
							nTempChair = (chairno+TOTAL_CHAIRS-i-1) % TOTAL_CHAIRS;
							if (TRUE == allInResponse.bNeedCompare[nTempChair])
							{
								if (TRUE == allInResponse.bWinOrLose[nTempChair])
								{
									OnUserBoutEnd(lpContext, pRoom, pTable, nTempChair, FALSE, roomid);
								}
								else
								{
									OnUserBoutEnd(lpContext, pRoom, pTable, chairno, FALSE, roomid);
									break;
								}
							}
						}
					}
				}
				break;
			case GAME_MSG_BET_FULL:
				{
					if (UR_OPERATE_FAILED == OnBetFull(lpContext, lpRequest, chairno, pTable))
					{	
						UwlLogFile("OnBetFull failed. roomid:%ld, tableno:%ld, userid:%ld",roomid, tableno, userid);
						REQUEST response;
						memset(&response, 0, sizeof(response));
						response.head.nRequest = UR_OPERATE_FAILED;
						return SendUserResponse(lpContext, &response);
					}
					
					if(TRUE == IsGameWin(pTable))
						return OnGameWin(lpContext,pRoom,pTable,chairno,FALSE,roomid);
				}
				break;
			case GAME_MSG_GIVE_UP:
				{
					if (GetAlivePlayerCount(pTable) < 2)
						return NotifyResponseSucceesd(lpContext);

					if (UR_OPERATE_FAILED == OnGiveUp(lpContext, lpRequest, chairno, pTable))
					{	
						UwlLogFile("OnGiveUp failed. roomid:%ld, tableno:%ld, userid:%ld",roomid, tableno, userid);
						REQUEST response;
						memset(&response, 0, sizeof(response));
						response.head.nRequest = UR_OPERATE_FAILED;
						return SendUserResponse(lpContext, &response);
					}

					if(TRUE == IsGameWin(pTable))
						return OnGameWin(lpContext,pRoom,pTable,chairno,FALSE,roomid);
					else
						OnUserBoutEnd(lpContext, pRoom, pTable, chairno, FALSE, roomid);
				}
				break;
			default:
				return NotifyResponseSucceesd(lpContext);
		}
	}
	return TRUE;
}
 
void CGameServer::OnSeverAutoPlay(CRoom* pRoom,CTable* ptr,int chairno)
{
	int nRoomID=pRoom->m_nRoomID;
	
	CGameTable* pTable=(CGameTable*)ptr;

	if (!IS_BIT_SET(pTable->m_dwStatus,TS_PLAYING_GAME))
	{
		return; 
	}
	if (!pTable->ValidateChair(chairno)) 
		return;
	CPlayer* pPlayer = pTable->m_ptrPlayers[chairno];
	if (!pPlayer) 
		return;
	
	if (chairno != pTable->GetCurrentChair())
		return;
 
	GIVE_UP_REQUEST giveUpRequest;
	memset(&giveUpRequest, -1, sizeof(GIVE_UP_REQUEST));
	giveUpRequest.nChairNo = chairno;
	giveUpRequest.dwGameOpe = GAME_OPE_GIVE_UP;

	DWORD dwTickWait=0; 
	SimulateGameMsgFromUser(nRoomID,pPlayer,GAME_MSG_GIVE_UP,sizeof(GIVE_UP_REQUEST),&giveUpRequest,dwTickWait);
}

BOOL CGameServer::OnEnterMatchDXXW(LPCONTEXT_HEAD lpContext, CTable* pTable,CPlayer* pPlayer)
{
	//断线续完
	/////////////////////////////////////////////////////////////////////////
	NotifyTableMsg(pTable,GAME_MSG_SEND_OTHER,SYSMSG_RETURN_GAME,4,&pPlayer->m_nChairNO,pPlayer->m_lTokenID);
	/////////////////////////////////////////////////////////////////////////
	return TRUE;
}

BOOL CGameServer::SimulateGameMsgFromUser(int nRoomID,CPlayer* player,int nMsgID,int nDatalen,void* data,DWORD dwSpace)
{
	if (!player)
		return FALSE;

	int size=nDatalen+sizeof(GAME_MSG);
	BYTE* pGameMsg=new BYTE[size];
	memset(pGameMsg,0,size);
	GAME_MSG* pHead=(GAME_MSG*)pGameMsg;
	BYTE* pData=pGameMsg+sizeof(GAME_MSG);
	
	pHead->nRoomID=nRoomID;
	pHead->nMsgID=nMsgID;
	pHead->nUserID=player->m_nUserID;
	pHead->nVerifyKey=-1;
	pHead->nDatalen=nDatalen;
	
	if (nDatalen)
		memcpy(pData,data,nDatalen);
	
	BOOL bn=TRUE;
	if (dwSpace==0)
	{
		bn=SimulateReqFromUser(player->m_hSocket,player->m_lTokenID,GR_SENDMSG_TO_SERVER,size,pGameMsg);
	}
	else
	{
		CreateSimulateRequst(dwSpace,GR_SENDMSG_TO_SERVER,size,pGameMsg,player->m_hSocket,player->m_lTokenID);
		delete []pGameMsg;
	}
	
	return bn;
}

void CGameServer::OnPlayerLeave(LPCONTEXT_HEAD lpContext,CTable* pTable,int chairno)
{
	NotifyTableMsg(pTable,GAME_MSG_SEND_OTHER,SYSMSG_PLAYER_OFFLINE,4,&chairno,lpContext->lTokenID);
}

//计算进入游戏时，返回客户端后附带用户信息长度
int CGameServer::CalcEnterUserDatelen(CRoom* pRoom,CTable* pTable,CPlayer* pPlayer)
{
	if (!pRoom || !pTable || !pPlayer)
		return 0;
	
	if (IS_BIT_SET(pPlayer->m_nUserType, UT_HANDPHONE) 
		&& IsSoloRoom(pRoom->m_nRoomID))
	{
		int nPlayer = pTable->m_mapUser.GetCount();					
		return sizeof(SOLOPLAYER_HEAD) + nPlayer*sizeof(SOLO_PLAYER);	
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
BOOL CGameServer::IsTableDepositRoom(int roomid)
{
	return IS_BIT_SET(GetRoomManage(roomid),RM_TABLEDEPOSIT);
}

//..../gamesvr/xxx.exe
//..../roomsvr/roomsvr.ini
//..../roomsvr2/roomsvr.ini
//			...
//..../roomsvr16/roomsvr.ini
BOOL CGameServer::GetRoomSvrIniFile(int roomid, CString& sRoomSvrIniFile)
{
	if (roomid<=0)
		return FALSE;

	HWND hRoomSvrWnd = GetRoomSvrWnd(roomid);

	if (!hRoomSvrWnd) 
		return FALSE;

	TCHAR szTemp[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), szTemp, sizeof(szTemp));

	//获取游戏目录
	CString sFolder;
	sFolder.Format(_T("%s"), szTemp);
	int nIndex = sFolder.ReverseFind('\\');
	if (nIndex<0)
		return FALSE;
	sFolder = sFolder.Left(nIndex);
	nIndex = sFolder.ReverseFind('\\');
	if (nIndex<0)
		return FALSE;
	sFolder = sFolder.Left(nIndex+1);

	int hWnd = 0;
	
	for (int i=0;i<16;i++)
	{
		if (i==0)
			sRoomSvrIniFile.Format(_T("%sroomsvr\\roomsvr.ini"), sFolder);
		else
			sRoomSvrIniFile.Format(_T("%sroomsvr%d\\roomsvr.ini"), sFolder, i+1);

		memset(szTemp, 0, sizeof(szTemp));
		GetPrivateProfileString(_T("listen"), _T("hwnd"), _T(""),szTemp,sizeof(szTemp) ,sRoomSvrIniFile);
		if (strlen(szTemp)>0)
		{	
			hWnd = atoi(szTemp);
			if (hWnd == (int)hRoomSvrWnd)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

int	 CGameServer::GetTableDepositMin(int roomid, int tableno)
{
	CString sRoomSvrIniFile;
	if (!GetRoomSvrIniFile(roomid, sRoomSvrIniFile))
		return -1;
	
	TCHAR szSection[32];
	memset(szSection,0,sizeof(szSection));
	sprintf(szSection,"TableDeposit%d",roomid);

	int nCount = GetPrivateProfileInt(szSection,_T("Count"),0, sRoomSvrIniFile);
	if(nCount<=0 || nCount>32)
		return -1;

	CString sTmp;
	for(int i=0;i<nCount;i++)
	{
		TCHAR szValue[64];
		memset(szValue,0,sizeof(szValue));
		sTmp.Format(_T("%d"),i);
		GetPrivateProfileString(szSection,sTmp,_T(""),szValue,sizeof(szValue) ,sRoomSvrIniFile);	
		if(szValue[0]==0)
			return -1;
		TCHAR *fields[8];
		memset(fields,0,sizeof(fields));
		TCHAR *p1, *p2;
		p1=szValue;
		int nFields=Svr_RetrieveFields ( p1, fields, 8, &p2 ) ;
		if(nFields<3)
			return FALSE;
		
		int nTableNO1=atoi(fields[0]);
		int nTableNO2=atoi(fields[1]);
		int nMinDeposit=atoi(fields[2]);
		if(tableno>=nTableNO1 && tableno<=nTableNO2)
			return nMinDeposit;
	}

	return 0;
}

int	 CGameServer::GetTableDepositMax(int roomid, int tableno)
{
	return 0;
}

BOOL CGameServer::OnSaveSafeDeposit(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	SOCKET sock = lpContext->hSocket;;
	LONG token = lpContext->lTokenID;;
	int gameid = 0;
	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	
	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;
	
	response.head.nRequest = UR_OPERATE_FAILED;
	
	if (lpRequest->nDataLen!=sizeof(SAVE_SAFE_DEPOSIT)){
		return SendUserResponse(lpContext, &response);
	}
	
	LPSAVE_SAFE_DEPOSIT pSaveSafeDeposit = (LPSAVE_SAFE_DEPOSIT)lpRequest->pDataPtr;
	gameid = pSaveSafeDeposit->nGameID;
	roomid = pSaveSafeDeposit->nRoomID;
	tableno = pSaveSafeDeposit->nTableNO;
	userid = pSaveSafeDeposit->nUserID;
	chairno = pSaveSafeDeposit->nChairNO;
	LPCTSTR hardid = LPCTSTR(pSaveSafeDeposit->szHardID);
	
	if(roomid <= 0 || tableno < 0 || userid <= 0 || chairno < 0 || gameid != m_nGameID
		|| chairno >=MAX_CHAIR_COUNT){		
		return SendUserResponse(lpContext, &response);
	}

	if (!IsNeedDepositRoom(roomid))
	{
		response.head.nRequest = GR_NO_THIS_FUNCTION_EX;
		return SendUserResponse(lpContext, &response);
	}

	if (!IsTakeDepositRoom(roomid))
	{
		response.head.nRequest = GR_NO_THIS_FUNCTION_EX;
		return SendUserResponse(lpContext, &response);
	}

	//校验hardid reserved
	{
		DWORD dwCode = GetHardCode(userid);
		if(!dwCode){
			response.head.nRequest = GR_HARDID_MISMATCH;
			return SendUserResponse(lpContext, &response);
		}
		DWORD hardcode = 0;
		xygMakeHardID2Code(hardid, lstrlen(hardid), hardcode); 
		if(hardcode != dwCode){
			response.head.nRequest = GR_HARDID_MISMATCH;
			return SendUserResponse(lpContext, &response);
		}
	}
	
	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			return SendUserResponse(lpContext, &response);
		}
	}
	
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		if(!pRoom->m_mapTable.Lookup(tableno, (CTable*&)pTable)){
			return SendUserResponse(lpContext, &response);
		}
	}
	
	if(pTable) {
		LOCK_TABLE(pTable, chairno, FALSE, userid, token);
		
		if (FALSE == pTable->m_mapUser.Lookup(userid, pPlayer))
		{
			UwlLogFile(_T("user not player. user %ld take deposit failed."), userid);
			return SendUserResponse(lpContext, &response);
		}

		if (!pPlayer || pPlayer->m_nUserID!=userid)
		{
			response.head.nRequest=UR_OPERATE_FAILED;
			return SendUserResponse(lpContext, &response);
		}

		if (IS_BIT_SET(pTable->m_dwStatus, TS_PLAYING_GAME))
		{
			if (!pPlayer->m_bIdlePlayer)
			{
				response.head.nRequest=GR_SAFEBOX_GAME_PLAYING;
				return SendUserResponse(lpContext, &response);
			}
		}
		else
		{
			if (NULL!=pTable->m_ptrPlayers[chairno] && pPlayer->m_nUserID==pTable->m_ptrPlayers[chairno]->m_nUserID && IS_BIT_SET(pTable->m_dwUserStatus[chairno],US_GAME_STARTED)){
				response.head.nRequest=GR_SAFEBOX_GAME_READY;
				return SendUserResponse(lpContext, &response);
			}
		}

		//等待分桌状态
		if (!pPlayer || IS_BIT_SET(pTable->m_dwUserStatus[chairno],US_USER_WAITNEWTABLE))
		{
			response.head.nRequest=GR_SAFEBOX_GAME_READY;
			return SendUserResponse(lpContext, &response);
		}


		//与玩家手中银子不等
		if (pPlayer->m_nDeposit != pSaveSafeDeposit->nGameDeposit){
			response.head.nRequest=GR_SAFEBOX_DEPOSIT_DIFFER;
			return SendUserResponse(lpContext, &response);
		}

		//划出银子后剩余银子不能少于房间最少限制
		int nMinDeposit = 0;
		if (IsTableDepositRoom(roomid))
		{
			nMinDeposit = GetTableDepositMin(roomid, tableno);	//取桌子银两
		}

		if (nMinDeposit<=0)
		{
			nMinDeposit = GetMinDeposit(roomid);	//取房间银两
		}

		if ((pPlayer->m_nDeposit-pSaveSafeDeposit->nDeposit)<nMinDeposit){
			response.head.nRequest = GR_SAFEBOX_DEPOSIT_MIN;
			response.pDataPtr = &nMinDeposit;
			response.nDataLen = sizeof(int);
			return SendUserResponse(lpContext, &response);
		}
		
		pSaveSafeDeposit->dwIPAddr = GetClientAddress(lpContext->hSocket, lpContext->lTokenID);
		
		CONTEXT_HEAD context;
		memcpy(&context, lpContext, sizeof(context));
		context.bNeedEcho = TRUE;
		
		REQUEST request;
		ZeroMemory(&request, sizeof(request));
		request.head.nRequest = GR_SAVE_SAFE_DEPOSIT;
		request.nDataLen = sizeof(SAVE_SAFE_DEPOSIT);
		request.pDataPtr = pSaveSafeDeposit;
		
		TransmitRequest(&context, &request);
	}
	
	return TRUE;
}

BOOL  CGameServer::OnSaveSafeDepositOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	SOCKET sock = INVALID_SOCKET;
	LONG token = 0;
	int gameid = 0;
	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	int r_id = 0;
	int t_no = INVALID_OBJECT_ID;
	int u_id = 0;
	int c_no = INVALID_OBJECT_ID;
	LONG room_tokenid = 0;
	
	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;
	
	sock = lpContext->hSocket;
	token = lpContext->lTokenID;
	
	lpReqToClient->head.nRequest = UR_OPERATE_SUCCEEDED;
	SendUserResponse(lpContext, lpReqToClient, FALSE, TRUE);
	LPSAVE_SAFE_DEPOSIT_OK lpSaveDepositOK = (LPSAVE_SAFE_DEPOSIT_OK)(PBYTE(lpReqFromServer->pDataPtr) + sizeof(CONTEXT_HEAD));
	gameid = lpSaveDepositOK->nGameID;
	roomid = lpSaveDepositOK->nRoomID;
	tableno = lpSaveDepositOK->nTableNO;
	userid = lpSaveDepositOK->nUserID;
	chairno = lpSaveDepositOK->nChairNO;
	
	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			return TRUE;
		}
	}
	
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		if(!pRoom->m_mapTable.Lookup(tableno, (CTable*&)pTable)){
			return TRUE;
		}
	}
	
	if(pTable) {
		LOCK_TABLE(pTable, chairno, FALSE, userid, token);
		
		if (FALSE == pTable->m_mapUser.Lookup(userid, pPlayer))
		{
			UwlLogFile(_T("user not player. user %ld take deposit failed."), userid);
			return TRUE;
		}

		if (pPlayer)
		{
			pPlayer->m_nDeposit = lpSaveDepositOK->nGameDeposit;
			
			//通知其它玩家
			USER_DEPOSITEVENT ude;
			memset(&ude, 0, sizeof(ude));
			ude.nUserID = pPlayer->m_nUserID;
			ude.nChairNO = pPlayer->m_nChairNO;
			ude.nEvent = USER_SAVE_SAFE_DEPOSIT;
			ude.nDepositDiff = lpSaveDepositOK->nDeposit;
			ude.nDeposit = pPlayer->m_nDeposit;
			if (NeedChangeBaseDeposit(pTable,pPlayer))
			{
				int nBaseDepos=ChangeBaseDeposit(pTable);
				if (nBaseDepos>0)
					ude.nBaseDeposit = nBaseDepos;
			}
			NotifyTablePlayers(pTable, GR_USER_DEPOSIT_EVENT, &ude, sizeof(ude), 0);
			NotifyTableVisitors(pTable, GR_USER_DEPOSIT_EVENT, &ude, sizeof(ude), 0);
		}
		
	}
	
	return TRUE;
}

int CGameServer::GetMinPlayingDeposit(CTable *pTable, int roomid)
{
	TCHAR szRoomID[16];
	memset(szRoomID, 0, sizeof(szRoomID));
	_stprintf(szRoomID, _T("%ld"), pTable->m_nRoomID);
	
	int deposit_min = GetPrivateProfileInt(
		_T("deposit"),			// section name
		_T("min"),				// key name
		MIN_DEPOSIT_NEED,		// default int
		m_szIniFile				// initialization file name
		);
	int fee_mode = GetPrivateProfileInt(
		_T("feemode"),	// section name
		szRoomID,		// key name
		FEE_MODE_TEA,	// default int
		m_szIniFile		// initialization file name
		);
	int fee_value = GetPrivateProfileInt(
		_T("feevalue"),	// section name
		szRoomID,		// key name
		1,				// default int
		m_szIniFile		// initialization file name
		);
	
	//如果是服务费模式，银子的最低要求为deposit_min+服务费
	if (FEE_MODE_SERVICE_FIXED == fee_mode
		&& IsNeedDepositRoom(roomid))
	{
		deposit_min += fee_value;
	}

	//判断桌子银两最低要求
	int nMinDeposit = 0;
	if (IsTableDepositRoom(roomid))
	{
		nMinDeposit = GetTableDepositMin(roomid, pTable->m_nTableNO);	//取桌子银两
	}
	if (nMinDeposit>deposit_min)
	{
		deposit_min = nMinDeposit;
	}
	
	return deposit_min;
}

BOOL CGameServer::OnTakeSafeDeposit(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	SOCKET sock = INVALID_SOCKET;
	LONG token = 0;
	int gameid = 0;
	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	int r_id = 0;
	int t_no = INVALID_OBJECT_ID;
	int u_id = 0;
	int c_no = INVALID_OBJECT_ID;
	LONG room_tokenid = 0;

	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;
	
	sock = lpContext->hSocket;
	token = lpContext->lTokenID;

	response.head.nRequest = UR_OPERATE_FAILED;

	if (lpRequest->nDataLen!=sizeof(TAKE_SAFE_DEPOSIT)){
		return SendUserResponse(lpContext, &response);
	}

 	LPTAKE_SAFE_DEPOSIT pTakeSafeDeposit = (LPTAKE_SAFE_DEPOSIT)lpRequest->pDataPtr;
	gameid = pTakeSafeDeposit->nGameID;
	roomid = pTakeSafeDeposit->nRoomID;
	tableno = pTakeSafeDeposit->nTableNO;
	userid = pTakeSafeDeposit->nUserID;
	chairno = pTakeSafeDeposit->nChairNO;
	LPCTSTR hardid = LPCTSTR(pTakeSafeDeposit->szHardID);

	if(roomid <= 0 || tableno < 0 || userid <= 0 || chairno < 0 || gameid != m_nGameID
		|| chairno >=MAX_CHAIR_COUNT){		
		return SendUserResponse(lpContext, &response);
	}

	if (!IsNeedDepositRoom(roomid))
	{
		response.head.nRequest = GR_NO_THIS_FUNCTION_EX;
		return SendUserResponse(lpContext, &response);
	}

	if (!IsTakeDepositRoom(roomid))
	{
		response.head.nRequest = GR_NO_THIS_FUNCTION_EX;
		return SendUserResponse(lpContext, &response);
	}

	//校验hardid reserved
	{
		DWORD dwCode = GetHardCode(userid);
		if(!dwCode){
			response.head.nRequest = GR_HARDID_MISMATCH_EX;
			return SendUserResponse(lpContext, &response);
		}
		DWORD hardcode = 0;
		xygMakeHardID2Code(hardid, lstrlen(hardid), hardcode); 
		if(hardcode != dwCode){
			response.head.nRequest = GR_HARDID_MISMATCH_EX;
			return SendUserResponse(lpContext, &response);
		}
	}

	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			return SendUserResponse(lpContext, &response);
		}
	}

	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		if(!pRoom->m_mapTable.Lookup(tableno, (CTable*&)pTable)){
			return SendUserResponse(lpContext, &response);
		}
	}

	if(pTable) {
		LOCK_TABLE(pTable, chairno, FALSE, userid, token);
		
		if (FALSE == pTable->m_mapUser.Lookup(userid, pPlayer))
		{
			UwlLogFile(_T("user not player. user %ld take deposit failed."), userid);
			return SendUserResponse(lpContext, &response);
		}
		
		if (!pPlayer || pPlayer->m_nUserID!=userid)
		{
			response.head.nRequest=UR_OPERATE_FAILED;
			return SendUserResponse(lpContext, &response);
		}

		if (IS_BIT_SET(pTable->m_dwStatus, TS_PLAYING_GAME))
		{
			if (!pPlayer->m_bIdlePlayer)
			{
				response.head.nRequest=GR_SAFEBOX_GAME_PLAYING;
				return SendUserResponse(lpContext, &response);
			}
		}
		else
		{
			if (NULL!=pTable->m_ptrPlayers[chairno] && pPlayer->m_nUserID==pTable->m_ptrPlayers[chairno]->m_nUserID && IS_BIT_SET(pTable->m_dwUserStatus[chairno],US_GAME_STARTED)){
				response.head.nRequest=GR_SAFEBOX_GAME_READY;
				return SendUserResponse(lpContext, &response);
			}
		}
		
		//等待分桌状态
		if (!pPlayer || IS_BIT_SET(pTable->m_dwUserStatus[chairno],US_USER_WAITNEWTABLE))
		{
			response.head.nRequest=GR_SAFEBOX_GAME_READY;
			return SendUserResponse(lpContext, &response);
		}

		//与玩家手中银子不等
		if (pPlayer->m_nDeposit != pTakeSafeDeposit->nGameDeposit){
			response.head.nRequest=GR_SAFEBOX_DEPOSIT_DIFFER;
			return SendUserResponse(lpContext, &response);
		}

		//划入银子后剩余银子不能高于房间最高限制
		int nMaxDeposit = GetMaxDeposit(roomid);
		if ((pPlayer->m_nDeposit+pTakeSafeDeposit->nDeposit)>nMaxDeposit){
			response.head.nRequest = GR_SAFEBOX_DEPOSIT_MAX;
			response.pDataPtr = &nMaxDeposit;
			response.nDataLen = sizeof(int);
			return SendUserResponse(lpContext, &response);
		}

		pTakeSafeDeposit->dwIPAddr = GetClientAddress(lpContext->hSocket, lpContext->lTokenID);
		
		CONTEXT_HEAD context;
		memcpy(&context, lpContext, sizeof(context));
		context.bNeedEcho = TRUE;
		
		REQUEST request;
		ZeroMemory(&request, sizeof(request));
		request.head.nRequest = GR_TAKE_SAFE_DEPOSIT;
		request.nDataLen = sizeof(TAKE_SAFE_DEPOSIT);
		request.pDataPtr = pTakeSafeDeposit;
		
		TransmitRequest(&context, &request);
	}

	return TRUE;
}

BOOL  CGameServer::OnTakeSafeDepositOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	SOCKET sock = lpContext->hSocket;
	LONG token = lpContext->lTokenID;
	int gameid = 0;
	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	
	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;
	
	LPTAKE_SAFE_DEPOSIT_OK lpTakeDepositOK = (LPTAKE_SAFE_DEPOSIT_OK)(PBYTE(lpReqFromServer->pDataPtr)+sizeof(CONTEXT_HEAD));
	gameid = lpTakeDepositOK->nGameID;
	roomid = lpTakeDepositOK->nRoomID;
	tableno = lpTakeDepositOK->nTableNO;
	userid = lpTakeDepositOK->nUserID;
	chairno = lpTakeDepositOK->nChairNO;
	
	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			return TRUE;
		}
	}
	
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		if(!pRoom->m_mapTable.Lookup(tableno, (CTable*&)pTable)){
			return TRUE;
		}
	}
	
	if(pTable) {
		LOCK_TABLE(pTable, chairno, FALSE, userid, token);
		
		if (FALSE == pTable->m_mapUser.Lookup(userid, pPlayer))
		{
			UwlLogFile(_T("user not player. user %ld take deposit failed."), userid);
			return TRUE;
		}

		lpReqToClient->head.nRequest = UR_OPERATE_SUCCEEDED;
		lpReqToClient->nDataLen = 0;
		lpReqToClient->pDataPtr = NULL;
		SendUserResponse(lpContext, lpReqToClient, FALSE, TRUE);

		if (pPlayer)
		{
			pPlayer->m_nDeposit = lpTakeDepositOK->nGameDeposit;
			
			//通知其它玩家
			USER_DEPOSITEVENT ude;
			memset(&ude, 0, sizeof(ude));
			ude.nUserID = pPlayer->m_nUserID;
			ude.nChairNO = pPlayer->m_nChairNO;
			ude.nEvent = USER_TAKE_SAFE_DEPOSIT;
			ude.nDepositDiff = lpTakeDepositOK->nDeposit;
			ude.nDeposit = pPlayer->m_nDeposit;
			if (NeedChangeBaseDeposit(pTable,pPlayer))
			{
				int nBaseDepos=ChangeBaseDeposit(pTable);
				if (nBaseDepos>0)
					ude.nBaseDeposit = nBaseDepos;
			}
			
			NotifyTablePlayers(pTable, GR_USER_DEPOSIT_EVENT, &ude, sizeof(ude), 0);
			NotifyTableVisitors(pTable, GR_USER_DEPOSIT_EVENT, &ude, sizeof(ude), 0);
		}
		
	}
	
	return TRUE;
}

BOOL CGameServer::OnAskNewTableChair(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	SOCKET sock = INVALID_SOCKET;
	LONG token = 0;
	int gameid = 0;
	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	int r_id = 0;
	int t_no = INVALID_OBJECT_ID;
	int u_id = 0;
	int c_no = INVALID_OBJECT_ID;
	LONG room_tokenid = 0;
	
	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;
	BOOL bPassive = IS_BIT_SET(lpContext->dwFlags,CH_FLAG_SYSTEM_EJECT);//是否系统自己生成的消息
	
	sock = lpContext->hSocket;
	token = lpContext->lTokenID;
	
	response.head.nRequest = UR_OPERATE_FAILED;
	
	if (lpRequest->nDataLen!=sizeof(ASK_NEWTABLECHAIR)){
		return SendUserResponse(lpContext, &response);
	}
	
	LPASK_NEWTABLECHAIR lpAskNewTableChair = (LPASK_NEWTABLECHAIR)lpRequest->pDataPtr;
	gameid = m_nGameID;
	roomid = lpAskNewTableChair->nRoomID;
	tableno = lpAskNewTableChair->nTableNO;
	userid = lpAskNewTableChair->nUserID;
	chairno = lpAskNewTableChair->nChairNO;

	if(roomid <= 0 || tableno < 0 || userid <= 0 || chairno < 0 || gameid != m_nGameID
		|| chairno >=MAX_CHAIR_COUNT){		
		return SendUserResponse(lpContext, &response);
	}

	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			return SendUserResponse(lpContext, &response);
		}
	}
	
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		if(!pRoom->m_mapTable.Lookup(tableno, (CTable*&)pTable)){
			return SendUserResponse(lpContext, &response);
		}

		USER_DATA user_data;
		memset(&user_data,0,sizeof(user_data));
		
		if (!m_mapUserData.Lookup(userid,user_data)
			&&bPassive==FALSE)
		{
			UwlLogFile(_T("user:%ld未在服务器注册，试图发送消息号为:GR_ASK_NEW_TABLECHAIR"),userid);
			return NotifyResponseFaild(lpContext);
		}
		else
		{
			roomid=user_data.nRoomID;
			tableno=user_data.nTableNO;
			chairno=user_data.nChairNO;
			
			if (user_data.nChairNO != lpAskNewTableChair->nChairNO)
			{
				UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GR_ASK_NEW_TABLECHAIR"), user_data.nRoomID, user_data.nTableNO, user_data.nUserID);
				return NotifyResponseFaild(lpContext);
			}	
		}
	}
	
	if(pTable) {
		LOCK_TABLE(pTable, chairno, FALSE, userid, token);
		
		if (FALSE == pTable->m_mapUser.Lookup(userid, pPlayer))
		{
			UwlLogFile(_T("user not player. user %ld take deposit failed."), userid);
			return SendUserResponse(lpContext, &response);
		}

		if (!pPlayer || pPlayer->m_nUserID!=userid){
			return SendUserResponse(lpContext, &response);
		}

		if (pPlayer->m_nDeposit < GetMinDeposit(roomid))
		{//银子不够
			response.head.nRequest = UR_ASK_NEW_TABLE_DEPOSIT_NOT_ENOUGH;
			return SendUserResponse(lpContext, &response);
		}

		if(IS_BIT_SET(pTable->m_dwStatus, TS_PLAYING_GAME)){
			if (!CanChangeTableInGame(roomid, pPlayer))
			{
				return SendUserResponse(lpContext, &response);
			}
		}

		if (!IsSoloRoom(roomid)
			|| !IsLeaveAlone(roomid)){
			return SendUserResponse(lpContext, &response);
		}

		// Add on 20130806
		if (IsRandomRoom(roomid)){ //随机房不得主动换桌
			return SendUserResponse(lpContext, &response);
		}

		if (!IS_BIT_SET(pPlayer->m_nUserType, UT_HANDPHONE)) //移动端不做验证
		{
			if (!VerifyAskNewTableChair(userid)) //时间间隔验证
			{
				response.head.nRequest = GR_RESPONE_ASKNEWTABLE_SPANSHORT;
				return SendUserResponse(lpContext, &response);		
			}		
		}
		// Add End	

		PostChangeTable(userid,roomid,tableno,chairno);
	}
	
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	SendUserResponse(lpContext, &response);
	return TRUE;
}

int CGameServer::OnNewTableChair(int roomid, int tableno, int chairno, int userid)
{
	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CTable* pOldTable = NULL;
	CPlayer* pPlayer = NULL;

	DWORD dwRoomOption = 0;
	DWORD dwRoomConfig = 0;

	USER_DATA user_data;
	memset(&user_data, 0, sizeof(user_data));
	if (!LookupUserData(userid, user_data))
		return -1;
	if (roomid != user_data.nRoomID)
		return -1;

	if (!IsSoloRoom(roomid)
		|| IsRandomRoom(roomid))
		return -1;
	
	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			return -1;
		}
	}
	
	SOLO_PLAYER pSoloPlayer;
	memset(&pSoloPlayer,0,sizeof(pSoloPlayer));
	if (!LookupSoloPlayer(userid,pSoloPlayer))
		return -1;

	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		//清理桌子，以RoomSvr为准
		if(!pRoom->m_mapTable.Lookup(tableno, pTable))
		{
			//桌子找不到,新建
			pTable = OnNewTable(roomid, tableno, m_nScoreMult);
			pRoom->m_mapTable.SetAt(tableno, pTable);
		}

		if (!pRoom->m_mapTable.Lookup(pSoloPlayer.nTableNO, pOldTable))
		{
			UwlLogFile(_T("OnNewTableChair failed. Can not find old table."));
		}

		dwRoomOption=GetRoomOption(roomid);
		dwRoomConfig=GetRoomConfig(roomid);
	}

	if (pTable)
	{
		if (!MoveUserToChair(userid,tableno,chairno,&pSoloPlayer))
		{
			UwlLogFile("Change Table Failed,roomid:%ld,userid:%d,tableno:%ld,chairno:%d",roomid,userid,tableno,chairno);
			//退出换座位玩家
			CloseClientInSolo(userid);
			//通知房间
			OnGameLeft(userid, roomid, tableno, chairno);

			return -1;
		}

		if (pOldTable)
		{
			int startcount=XygGetStartCount(pOldTable->m_dwUserStatus, pOldTable->m_nTotalChairs);
			if (pOldTable->IsNeedCountdown() && TRUE==pOldTable->IsInCountDown() && startcount<2)
			{
				START_COUNTDOWN  sc;
				memset(&sc,0,sizeof(START_COUNTDOWN));
				sc.nRoomID = pOldTable->m_nRoomID;
				sc.nTableNO = pOldTable->m_nTableNO;
				sc.bStartorStop = FALSE;
				sc.nCountDown = GetCountdown(pOldTable,pOldTable->m_nRoomID);
				pOldTable->SetCountDown(FALSE);
				NotifyTablePlayers(pOldTable,GR_START_COUNTDOWN,&sc,sizeof(START_COUNTDOWN));
				NotifyTableVisitors(pOldTable,GR_START_COUNTDOWN,&sc,sizeof(START_COUNTDOWN));
			}
		}

		{
			CAutoLock lock(&(pTable->m_csTable));
			pPlayer = pTable->m_ptrPlayers[chairno];
			pPlayer->m_bLookOn = FALSE;
			pTable->m_dwUserStatus[chairno] &= ~US_GAME_STARTED;
			pTable->m_dwRoomConfig[chairno] = dwRoomConfig;
			pTable->m_dwRoomOption[chairno] = dwRoomOption;
			//Solo模式中通知其他客户端
			if (pPlayer)
			{
				//Solo模式中通知其他客户端
				SOLO_PLAYER sp;
				memset(&sp,0,sizeof(sp));
				if (LookupSoloPlayer(userid,sp))
				{
					//modify 2014.1.27 by pgl
					NotifyPlayersSomeoneEnter(pTable,&sp,pPlayer->m_lTokenID);
					NotifyVistorsSomeoneEnter(pTable,&sp,pPlayer->m_lTokenID);
				}
				
				//通知换桌子的本人
				USER_POSITION userPos;
				memset(&userPos, 0, sizeof(userPos));
				userPos.nUserID  = userid;
				userPos.nRoomID  = roomid;
				userPos.nTableNO = tableno;
				userPos.nChairNO = chairno;
				userPos.nPlayerCount = pTable->m_mapUser.GetCount();
				memcpy(userPos.dwUserStatus,pTable->m_dwUserStatus,sizeof(userPos.dwUserStatus));
				userPos.dwTableStatus = pTable->m_dwStatus;
				if (IsVariableChairRoom(roomid)
					&& pTable->IsGameOver()
					&& pTable->IsInCountDown())
				{
					userPos.nCountdown = pTable->CalcRemainCountdown(GetTickCount());
				}			
				int nLen = sizeof(USER_POSITION) + userPos.nPlayerCount*sizeof(SOLO_PLAYER);
				int nGameTableLen = 0;
				if(IsVariableChairRoom(roomid)
					&& IS_BIT_SET(userPos.dwTableStatus, TS_PLAYING_GAME))
				{
					nGameTableLen = pTable->GetGameTableInfoSize();
				}
				nLen += nGameTableLen; // 可变桌椅也传GameTableInfo
				
				BYTE* pData = new BYTE[nLen];
				memset(pData,0,nLen);
				memcpy(pData,((PBYTE)&userPos),sizeof(USER_POSITION));
				int nPlayerCount = 0;

				int nTempUserID = 0;
				CPlayer* pTempPlayer = NULL;
				POSITION pos = pTable->m_mapUser.GetStartPosition();
				while(NULL != pos)
				{
					pTable->m_mapUser.GetNextAssoc(pos, nTempUserID, pTempPlayer);
					if (NULL != pTempPlayer && pTempPlayer->m_nUserID>0)
					{
						memset(&sp, 0, sizeof(sp));
						if (LookupSoloPlayer(pTempPlayer->m_nUserID, sp))
						{
							//更新soloplayer
							{
								sp.nTableNO = pTable->m_nTableNO;
								sp.nChairNO = pTempPlayer->m_nChairNO;
								sp.nScore = pTempPlayer->m_nScore;
								sp.nDeposit = pTempPlayer->m_nDeposit;
								sp.nPlayerLevel = pTempPlayer->m_nLevelID;
								sp.nBout = pTempPlayer->m_nBout;
								sp.nTimeCost = pTempPlayer->m_nExperience * 60;	

								if (pTempPlayer->m_nUserID == userid)
									sp.nStatus = ROOM_PLAYER_STATUS_WAITING;
								
								SetSoloPlayer(pTempPlayer->m_nUserID, sp);
							}
							
							memcpy(pData+sizeof(USER_POSITION)+nPlayerCount*sizeof(SOLO_PLAYER), &sp, sizeof(SOLO_PLAYER));
							nPlayerCount++;
						}
					}
				}

				if(IsVariableChairRoom(roomid)
					&& IS_BIT_SET(userPos.dwTableStatus, TS_PLAYING_GAME))
				{//如果新桌子已经开始游戏，则拷贝对局信息
					pTable->FillupGameTableInfo(pData+sizeof(USER_POSITION)+userPos.nPlayerCount*sizeof(SOLO_PLAYER), nGameTableLen, chairno, FALSE);	
				} 

				NotifyOneUser(pPlayer->m_hSocket, pPlayer->m_lTokenID, GR_USER_POSITION, pData, nLen, TRUE);
				SAFE_DELETE_ARRAY(pData);
			}
		}
	}

	return 1;
}

BOOL CGameServer::OnLeaveGame(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	GAME_ABORT GameAbort;
	ZeroMemory(&GameAbort, sizeof(GameAbort));

	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	int r_id = 0;
	int t_no = INVALID_OBJECT_ID;
	int u_id = 0;
	int c_no = INVALID_OBJECT_ID;

	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;

	LPLEAVE_GAME pLeaveGame = LPLEAVE_GAME(PBYTE(lpRequest->pDataPtr));

	roomid = pLeaveGame->nRoomID;
	tableno = pLeaveGame->nTableNO;
	userid = pLeaveGame->nUserID;
	chairno = pLeaveGame->nChairNO;

	//BOOL bPassive = pLeaveGame->bPassive;
	LPSENDER_INFO pSenderInfo = LPSENDER_INFO(&(pLeaveGame->sender_info));

	BOOL bPassive = (chairno != pSenderInfo->nSendChair) ? TRUE : FALSE;

	int sendtable = pSenderInfo->nSendTable;
	int sendchair = pSenderInfo->nSendChair;
	int senduser = pSenderInfo->nSendUser;
	LONG token = lpContext->lTokenID;
	
	//在结算 不能散桌
	if (NeedLeaveWaitGameWin(roomid,tableno,chairno,userid,token)) 
		return TRUE;

	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			return TRUE;
		}
	}
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		if(!pRoom->m_mapTable.Lookup(tableno, pTable)){
			return TRUE;
		}
	}
	if(pTable) {
		LOCK_TABLE(pTable, chairno, bPassive, senduser, token);

		if(pTable->m_nRoomID != roomid){
			UwlLogFile(_T("table.roomid %ld != leave.roomid %ld, tableno = %ld, chairno = %ld. leavegame failed."), 
					pTable->m_nRoomID, roomid, tableno, chairno);
			return TRUE;
		}
		pTable->m_mapUser.Lookup(userid, pPlayer);
		if(!pPlayer || !pPlayer->m_nUserID){
			return TRUE;
		}
		if(!VerifySenderInfo(pTable, pSenderInfo)){
			UwlLogFile(_T("verify sender_info failed! roomid = %ld, tableno = %ld, chairno = %ld, userid = %ld, sendtable = %ld, sendchair = %ld, senduser = %ld"),
				roomid, tableno, chairno, userid, sendtable, sendchair, senduser);
			return TRUE;
		}
		if(pTable->IsPlayer(userid)){ // 玩家离开
			CPlayer* ptrP = pTable->m_ptrPlayers[chairno];
			if(!ptrP) return TRUE;
			if(userid != ptrP->m_nUserID){
				UwlLogFile(_T("leave userid not matched! roomid = %ld, tableno = %ld, chairno = %ld, userid = %ld, actualid = %ld"),
					roomid, tableno, chairno, userid, ptrP->m_nUserID);
				return TRUE;
			}
			GameAbort.nUserID = userid;
			GameAbort.nChairNO = chairno;
			GameAbort.nOldScore = pPlayer->m_nScore;
			GameAbort.nOldDeposit = pPlayer->m_nDeposit;
			GameAbort.nTableNO = pTable->m_nTableNO;
			
			// 判断是否要逃跑扣分
			int leastbout = GetBoutLeast();
			int leastround = GetPrivateProfileInt(
						_T("round"),		// section name
						_T("least"),	// key name
						0,				// default int
						m_szIniFile		// initialization file name
						);
			int breakwait = GetPrivateProfileInt(
						_T("break"),		// section name
						_T("wait"),			// key name
						DEF_BREAK_WAIT,		// default int
						m_szIniFile			// initialization file name
						);
			int defdouble = GetPrivateProfileInt(
						_T("break"),		// section name
						_T("double"),		// key name
						DEF_BREAK_DOUBLE,	// default int
						m_szIniFile			// initialization file name
						);
			int max_bouttime = GetBoutTimeMax();

			BOOL bAsBreak = pTable->LeaveAsBreak(leastbout, leastround);

			//可变桌椅空闲玩家退出不算强退	20151016
			if (IsVariableChairRoom(roomid)
				&& pTable->m_ptrPlayers[chairno]
				&& pTable->m_ptrPlayers[chairno]->m_bIdlePlayer)
			{
				bAsBreak = FALSE;
			}

			if (bAsBreak)
			{
				int breakchair = -1;
				breakchair = pTable->TellBreakChair(chairno, breakwait);
				if(-1 != breakchair
					&& pTable->m_ptrPlayers[breakchair])
				{ // 增加m_ptrPlayers[breakchair]检查，如果为空，直接散桌，不扣分	//Add on 20121217 by chenyang
					if (!IsVariableChairRoom(roomid))
					{
						TransmitBreakResult(lpContext,pTable,ptrP,GameAbort,breakchair,max_bouttime,defdouble);
					}
					else
					{
						TransmitBreakResultEx(lpContext,pTable,ptrP,GameAbort,breakchair,max_bouttime,defdouble);
					}
				}
				RemoveClients(pTable, 0, FALSE);
				
				NotifyTablePlayers(pTable, GR_GAME_ABORT, &GameAbort, sizeof(GameAbort));
				NotifyTableVisitors(pTable, GR_GAME_ABORT, &GameAbort, sizeof(GameAbort));
				
				pTable->Reset(); // 清空桌子
			}
			else if (IsSoloRoom(roomid) && IsLeaveAlone(roomid))	//非强退，让他自己离开 20151016
			{
				RemoveOneClients(pTable,userid,TRUE);
				
				if (!IsCloakingRoom(roomid))
				{
					NotifyTablePlayers(pTable, GR_PLAYER_ABORT, &GameAbort, sizeof(GameAbort));
				}
				
				NotifyTableVisitors(pTable, GR_PLAYER_ABORT, &GameAbort, sizeof(GameAbort));
								
				pTable->PlayerLeave(userid);
				
				OnGameLeft(userid,roomid,tableno,chairno);
			}
			
		}else if(pTable->IsVisitor(userid)){ // 旁观者离开
			RemoveOneClients(pTable,userid,TRUE);

			LOOKON_ABORT LookOnAbort;
			ZeroMemory(&LookOnAbort, sizeof(LookOnAbort));		
			LookOnAbort.nUserID = userid;
			LookOnAbort.nChairNO = chairno;

			OnGameLeft(userid,roomid,tableno,chairno);

			NotifyTablePlayers(pTable, GR_LOOKON_ABORT, &LookOnAbort, sizeof(LookOnAbort));
			NotifyTableVisitors(pTable, GR_LOOKON_ABORT, &LookOnAbort, sizeof(LookOnAbort));

			pTable->VisitorLeave(userid, chairno);
		}
	}
	return TRUE;
}

BOOL CGameServer::OnLeaveGameEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	GAME_ABORT GameAbort;
	ZeroMemory(&GameAbort, sizeof(GameAbort));

	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	int r_id = 0;
	int t_no = INVALID_OBJECT_ID;
	int u_id = 0;
	int c_no = INVALID_OBJECT_ID;

	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;

	LPLEAVE_GAME pLeaveGame = LPLEAVE_GAME(PBYTE(lpRequest->pDataPtr));

	roomid  = pLeaveGame->nRoomID;
	tableno = pLeaveGame->nTableNO;
	userid  = pLeaveGame->nUserID;
	chairno = pLeaveGame->nChairNO;

//	BOOL bPassive = pLeaveGame->bPassive;
	LPSENDER_INFO pSenderInfo = LPSENDER_INFO(&(pLeaveGame->sender_info));

	BOOL bPassive = IS_BIT_SET(lpContext->dwFlags,CH_FLAG_SYSTEM_EJECT);

	int sendtable = pSenderInfo->nSendTable;
	int sendchair = pSenderInfo->nSendChair;
	int senduser = pSenderInfo->nSendUser;
	LONG token = lpContext->lTokenID;
	
	//在结算 不能散桌
	if (NeedLeaveWaitGameWin(roomid,tableno,chairno,userid,token)) 	
	{
		response.head.nRequest = GR_WAIT_CHECKRESULT;
		return SendUserResponse(lpContext, &response, bPassive);
	}

	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			response.head.nRequest = UR_OPERATE_FAILED;
			return SendUserResponse(lpContext, &response,bPassive);
		}
	}
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		if(!pRoom->m_mapTable.Lookup(tableno, pTable)){
			response.head.nRequest = UR_OPERATE_FAILED;
			return SendUserResponse(lpContext, &response,bPassive);
		}
	}
	if(pTable) {
		LOCK_TABLE(pTable, chairno, bPassive, senduser, token);

		if(pTable->m_nRoomID != roomid){
			UwlLogFile(_T("table.roomid %ld != leave.roomid %ld, tableno = %ld, chairno = %ld. leavegame failed."), 
					pTable->m_nRoomID, roomid, tableno, chairno);
			response.head.nRequest = UR_OPERATE_FAILED;
			return SendUserResponse(lpContext, &response,bPassive);
		}
		pTable->m_mapUser.Lookup(userid, pPlayer);
		if(!pPlayer || !pPlayer->m_nUserID){
			response.head.nRequest = UR_OPERATE_FAILED;
			return SendUserResponse(lpContext, &response,bPassive);
		}
		if(!VerifySenderInfo(pTable, pSenderInfo)){
			UwlLogFile(_T("verify sender_info failed! roomid = %ld, tableno = %ld, chairno = %ld, userid = %ld, sendtable = %ld, sendchair = %ld, senduser = %ld"),
				roomid, tableno, chairno, userid, sendtable, sendchair, senduser);
			response.head.nRequest = UR_OPERATE_FAILED;
			return SendUserResponse(lpContext, &response,bPassive);
		}
		if(pTable->IsPlayer(userid)){ // 玩家离开
			CPlayer* ptrP = pTable->m_ptrPlayers[chairno];
			if(!ptrP) 
			{
				response.head.nRequest = UR_OPERATE_FAILED;
		    	return SendUserResponse(lpContext, &response,bPassive);
			}
			if(userid != ptrP->m_nUserID){
				UwlLogFile(_T("leave userid not matched! roomid = %ld, tableno = %ld, chairno = %ld, userid = %ld, actualid = %ld"),
					roomid, tableno, chairno, userid, ptrP->m_nUserID);
				response.head.nRequest = UR_OPERATE_FAILED;
			    return SendUserResponse(lpContext, &response,bPassive);
			}
			GameAbort.nUserID = userid;
			GameAbort.nChairNO = chairno;
			GameAbort.nOldScore = pPlayer->m_nScore;
			GameAbort.nOldDeposit = pPlayer->m_nDeposit;
			GameAbort.nTableNO = pTable->m_nTableNO;


			
			// 判断是否要逃跑扣分
			int leastbout = GetBoutLeast();
			int leastround = GetPrivateProfileInt(
						_T("round"),		// section name
						_T("least"),	// key name
						0,				// default int
						m_szIniFile		// initialization file name
						);
			int breakwait = GetPrivateProfileInt(
						_T("break"),		// section name
						_T("wait"),			// key name
						DEF_BREAK_WAIT,		// default int
						m_szIniFile			// initialization file name
						);
			int defdouble = GetPrivateProfileInt(
						_T("break"),		// section name
						_T("double"),		// key name
						DEF_BREAK_DOUBLE,	// default int
						m_szIniFile			// initialization file name
						);
			int max_bouttime = GetBoutTimeMax();

			TCHAR szRoomID[16];
			memset(szRoomID, 0, sizeof(szRoomID));
			_stprintf(szRoomID, _T("%ld"), pTable->m_nRoomID);

			int least_entertable = GetPrivateProfileInt(
				_T("KeepStartedSecond"),		// section name
						szRoomID,		// key name
								0,	// default int
							m_szIniFile		// initialization file name
									);

			//判断是不是强退
			/////////////////////////////////////////////////////////////////////////
			BOOL bAsBreak = pTable->LeaveAsBreak(leastbout, leastround);

			//可变桌椅空闲玩家退出不算强退
			if (IsVariableChairRoom(roomid)
				&& pTable->m_ptrPlayers[chairno]
				&& pTable->m_ptrPlayers[chairno]->m_bIdlePlayer)
			{
				bAsBreak = FALSE;
			}

			int breakchair = -1;
			if(bAsBreak) breakchair = pTable->TellBreakChair(chairno, breakwait);
			////////////////////////////////////////////////////////////////////////////

			if (!bPassive
				&&IS_BIT_SET(pTable->m_dwStatus,TS_PLAYING_GAME)
				&&pTable->m_bForbidDesert
				&&bAsBreak
				&&breakchair==chairno)//随机Solo房间禁止主动强退，其他人超时后可以退出
			{
				//游戏中不能退出
				response.head.nRequest = GR_LEAVEGAME_PLAYING;
				return SendUserResponse(lpContext, &response,bPassive);
			}
			
			DWORD dwEnterTime=GetTickCount()-pTable->m_dwFirstEnter[chairno];
			if (IsSoloRoom(roomid)				//增加solo房间判断，因为普通房间游戏也要会发送LeaveGameEx
				&&!bPassive
				&&dwEnterTime<=least_entertable*1000
				&&pTable->IsFirstBout())//玩过一局以后总是可以退出了。
			{
				//太快退出
				response.head.nRequest = GR_LEAVEGAME_TOOFAST;
				response.pDataPtr=new int;
				*(int*)response.pDataPtr=(least_entertable*1000-dwEnterTime)/1000+1;
				response.nDataLen=sizeof(int);
				
				SendUserResponse(lpContext, &response,bPassive);
				UwlClearRequest(&response);
				return TRUE;
			}

			if(bAsBreak && -1 != breakchair
				&& pTable->m_ptrPlayers[breakchair])
			{ //强退 // 增加m_ptrPlayers[breakchair]检查，如果为空，直接散桌，不扣分	//Add on 20121217 by chenyang
				if (!IsVariableChairRoom(roomid))
				{
					TransmitBreakResult(lpContext,pTable,ptrP,GameAbort,breakchair,max_bouttime,defdouble);
				}
				else
				{
					TransmitBreakResultEx(lpContext,pTable,ptrP,GameAbort,breakchair,max_bouttime,defdouble);
				}
			}

			if (!bAsBreak
				&&IsSoloRoom(roomid))//Solo模式
			{
				BOOL bLeaveAlone=IsLeaveAlone(roomid);
				BOOL bRandomRoom=IsRandomRoom(roomid);
				BOOL bFirstBount=pTable->IsFirstBout();	//第一局，还没有正式开始游戏
				
				if (bFirstBount||bLeaveAlone)
				{
					RemoveOneClients(pTable,userid,FALSE);
					
					if (!IsCloakingRoom(roomid))
					{
						NotifyTablePlayers(pTable, GR_PLAYER_ABORT, &GameAbort, sizeof(GameAbort));
					}

					NotifyTableVisitors(pTable, GR_PLAYER_ABORT, &GameAbort, sizeof(GameAbort));
					
					
					if (IsVariableChairRoom(roomid)
						&& pTable->m_ptrPlayers[chairno]
						&& pTable->m_ptrPlayers[chairno]->m_bIdlePlayer
						&& !pTable->IsGameOver())
					{
						//空闲玩家离开不清空桌子
						pTable->PlayerLeave(userid);
					}
					else
					{
						pTable->PlayerLeave(userid);
						pTable->ResetTable();
					}
					
					OnGameLeft(userid,roomid,tableno,chairno);

					if (bRandomRoom&&bLeaveAlone&&!bFirstBount)
					{
						//随机分桌模式中，如果至少玩过一局后
						//有玩家退出,需要把其他已经按下开始键的玩家重新加入等待分桌序列。
						for(int i = 0; i < pTable->m_nTotalChairs; i++)	
						{
							CPlayer* pStartedPlayer=pTable->m_ptrPlayers[i];
							if (pStartedPlayer
								&&IS_BIT_SET(pTable->m_dwUserStatus[i],US_GAME_STARTED))
							{
								pTable->m_dwUserStatus[i]&=~US_GAME_STARTED;//去掉准备状态
								pTable->m_dwUserStatus[i]|=US_USER_WAITNEWTABLE;//pengsy
								PostAskNewTable(pStartedPlayer->m_nUserID,roomid,tableno,i);
								NotifyOneUser(pStartedPlayer->m_hSocket,pStartedPlayer->m_lTokenID,GR_WAIT_NEWTABLE,NULL,0);
							}
						}
					}

					//自由人数模式，游戏未开始时退出，必要时终止倒计时
					if (IsVariableChairRoom(roomid)
						&& pTable->IsGameOver()
						&& pTable->IsNeedCountdown())
					{
						int  startcount = XygGetStartCount(pTable->m_dwUserStatus, pTable->m_nTotalChairs);
						BOOL bAllowStartGame = IsAllowStartGame(pTable, startcount);
						BOOL bInCountdown = pTable->IsInCountDown();
						if (!bAllowStartGame && bInCountdown)
						{
							//结束倒计时
							START_COUNTDOWN  sc;
							memset(&sc,0,sizeof(START_COUNTDOWN));
							sc.nUserID = userid;
							sc.nRoomID = roomid;
							sc.nTableNO = tableno;
							sc.nChairNO = chairno;
							sc.bStartorStop = FALSE;
							pTable->SetCountDown(FALSE);
							NotifyTablePlayers(pTable,GR_START_COUNTDOWN,&sc,sizeof(START_COUNTDOWN));
							NotifyTableVisitors(pTable, GR_START_COUNTDOWN, &sc, sizeof(START_COUNTDOWN));
						}
					}
				}
				else
				{
					RemoveClients(pTable, 0, FALSE);
					
					NotifyTablePlayers(pTable, GR_GAME_ABORT, &GameAbort, sizeof(GameAbort));
					NotifyTableVisitors(pTable, GR_GAME_ABORT, &GameAbort, sizeof(GameAbort));
					
					pTable->Reset(); // 清空桌子
				}
			}
			else//非Solo模式或强退
			{
				RemoveClients(pTable, 0, FALSE);
				
				NotifyTablePlayers(pTable, GR_GAME_ABORT, &GameAbort, sizeof(GameAbort));
				NotifyTableVisitors(pTable, GR_GAME_ABORT, &GameAbort, sizeof(GameAbort));
				
				pTable->Reset(); // 清空桌子
			}
		}else if(pTable->IsVisitor(userid)){ // 旁观者离开
			LOOKON_ABORT LookOnAbort;
			ZeroMemory(&LookOnAbort, sizeof(LookOnAbort));		
			LookOnAbort.nUserID = userid;
			LookOnAbort.nChairNO = chairno;

			OnGameLeft(userid,roomid,tableno,chairno);

			NotifyTablePlayers(pTable, GR_LOOKON_ABORT, &LookOnAbort, sizeof(LookOnAbort));
			NotifyTableVisitors(pTable, GR_LOOKON_ABORT, &LookOnAbort, sizeof(LookOnAbort));

			pTable->VisitorLeave(userid, chairno);
		}
	}

	response.head.nRequest = UR_OPERATE_SUCCEEDED;
    return SendUserResponse(lpContext, &response,bPassive);
}

void CGameServer::CopyEnterUserData(CRoom* pRoom,CTable* pTable,CPlayer* pPlayer,BYTE* pSendClient,int nInfoLen)
{
	if (!pRoom || !pTable || !pPlayer || !pSendClient)
		return;
	
	int roomid  = pRoom->m_nRoomID;
	int tableno = pTable->m_nTableNO;
	int userid  = pPlayer->m_nUserID;
	
	if (IsSoloRoom(roomid))
	{
		if (IS_BIT_SET(pPlayer->m_nUserType, UT_HANDPHONE))
		{																	
			SOLOPLAYER_HEAD sph;
			memset(&sph, 0, sizeof(SOLOPLAYER_HEAD));
			sph.nRoomID      = roomid;
			sph.nTableNO     = tableno;
			sph.nPlayerCount = pTable->m_mapUser.GetCount();
			memcpy(sph.dwUserStatus,pTable->m_dwUserStatus,sizeof(sph.dwUserStatus));				
			
			//SOLOPLAYER_HEAD
			memcpy(pSendClient+nInfoLen, &sph, sizeof(SOLOPLAYER_HEAD));
			
			SOLO_PLAYER sp;
			int nPlayerCount = 0;
			int nTempUserID = 0;
			CPlayer* pTempPlayer = NULL;
			POSITION pos = pTable->m_mapUser.GetStartPosition();
			while(NULL != pos)
			{
				pTable->m_mapUser.GetNextAssoc(pos, nTempUserID, pTempPlayer);
				if (NULL != pTempPlayer && pTempPlayer->m_nUserID>0)
				{
					memset(&sp, 0, sizeof(sp));
					if (LookupSoloPlayer(pTempPlayer->m_nUserID, sp))
					{
						//更新soloplayer
						{
							sp.nTableNO = pTable->m_nTableNO;
							sp.nChairNO = pTempPlayer->m_nChairNO;
							sp.nScore = pTempPlayer->m_nScore;
							sp.nDeposit = pTempPlayer->m_nDeposit;
							sp.nPlayerLevel = pTempPlayer->m_nLevelID;
							sp.nBout = pTempPlayer->m_nBout;
							sp.nTimeCost = pTempPlayer->m_nExperience * 60;	
							
							SetSoloPlayer(pTempPlayer->m_nUserID, sp);
						}

						if (IsCloakingRoom(roomid)) //手机隐身房重置其他玩家信息
						{
							if(sp.nUserID!=pPlayer->m_nUserID)
								memset(&sp, 0, sizeof(sp));
						}
						
						memcpy(pSendClient+nInfoLen+sizeof(SOLOPLAYER_HEAD)+nPlayerCount*sizeof(SOLO_PLAYER), &sp, sizeof(SOLO_PLAYER));
						nPlayerCount++;
					}
				}
			}
		}
		else
		{			
			SOLO_PLAYER sp;
			memset(&sp, 0, sizeof(sp));
			if (LookupSoloPlayer(userid, sp))
			{
				{	//用服务端数据更新soloplayer
					sp.nTableNO     = tableno;
					sp.nChairNO     = pPlayer->m_nChairNO;
					sp.nScore       = pPlayer->m_nScore;
					sp.nDeposit     = pPlayer->m_nDeposit;
					sp.nPlayerLevel = pPlayer->m_nLevelID;
					sp.nBout        = pPlayer->m_nBout;
					sp.nTimeCost    = pPlayer->m_nExperience * 60;					
					
					SetSoloPlayer(userid, sp);
				}
			}
		}
	}
}

BOOL CGameServer::IsGameWin(CGameTable* pTable)
{
	if (1 == GetAlivePlayerCount(pTable))
		return TRUE;
	else
		return FALSE;
}

int CGameServer::GetAlivePlayerCount(CGameTable* pTable)
{
	int nAliveCount = 0;
	for (int i = 0; i < TOTAL_CHAIRS; i++)
	{
		if (pTable->GetPlayerInfo(i)->bInGame && pTable->GetPlayerInfo(i)->nPlayerStatus != PLAYER_STATUS_GIVE_UP)
			nAliveCount++;
	}

	return nAliveCount;
}

UINT CGameServer::OnLookCard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable)
{
	GAME_MSG* pMsg = LPGAME_MSG(PBYTE(lpRequest->pDataPtr));
	BYTE* pData = PBYTE(lpRequest->pDataPtr)+sizeof(GAME_MSG);
	
	int nShouldDataLen=sizeof(GAME_MSG)+sizeof(SHOW_CARD_REQUEST);
	if (lpRequest->nDataLen!=nShouldDataLen)
	{
		UwlLogFile(_T("SHOW_CARD_REQUEST结构长度不对, UserID=%ld"), pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}

	SHOW_CARD_REQUEST *pLookCard=(SHOW_CARD_REQUEST*)pData;

	if (nChairNo != pLookCard->nChairNo)
	{
		UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GAME_MSG_LOOK_CARD"), pMsg->nRoomID, pTable->m_nTableNO, pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}

	if (!IS_BIT_SET(pTable->m_dwStatus,TS_PLAYING_GAME))
	{//未在游戏中，不允许看牌
		return UR_OPERATE_FAILED;
	}

	if (pTable->m_GameTalbeInfo->m_Public.bnLookCard[nChairNo]==TRUE)
	{//已经看过牌了，不能再看
		return UR_OPERATE_FAILED;
	}

	if (PLAYER_STATUS_GIVE_UP == pTable->GetChairStatus(nChairNo))
	{
		UwlLogFile("OnLookCard failed, player has Given up, line=%d", __LINE__);
		return FALSE;
	}
	
	pTable->m_GameTalbeInfo->m_Public.bnLookCard[nChairNo]=TRUE;
	int *chair_cards=pTable->m_GameTalbeInfo->m_Player[nChairNo].nChairCards;
	
	SHOW_CARD_RESPONSE look_card;
	ZeroMemory(&look_card,sizeof(SHOW_CARD_RESPONSE));
	
	//通知其他家，该玩家看牌了
	look_card.nChairNo=nChairNo;
	NotifyTableMsg(pTable,GAME_MSG_SEND_OTHER,GAME_MSG_LOOK_CARD, sizeof(SHOW_CARD_RESPONSE), &look_card, lpContext->lTokenID);
	//把自己的牌放进结构，并发送
	memcpy(look_card.nChairCards,chair_cards,sizeof(int)*CHAIR_CARDS);
	if (pTable->GetCurrentChair() == nChairNo)
		look_card.dwNextCouldOpe = pTable->CalcPlayerCouldOpe(nChairNo);//看牌之后如果轮到对应玩家需要更新可执行的操作

	NotifyPlayerMsgAndResponse(lpContext,pTable,nChairNo,GAME_MSG_LOOK_CARD,sizeof(SHOW_CARD_RESPONSE),&look_card);
	
	return UR_OPERATE_SUCCEEDED;
}

UINT CGameServer::OnShowCard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable)
{
	GAME_MSG* pMsg = LPGAME_MSG(PBYTE(lpRequest->pDataPtr));
	BYTE* pData = PBYTE(lpRequest->pDataPtr)+sizeof(GAME_MSG);

	int nShouldDataLen=sizeof(GAME_MSG)+sizeof(SHOW_CARD_REQUEST);
	if (lpRequest->nDataLen!=nShouldDataLen)
	{
		UwlLogFile(_T("SHOW_CARD_REQUEST结构长度不对, UserID=%ld"), pMsg->nUserID);
		return UR_OPERATE_FAILED;	
	}

	SHOW_CARD_REQUEST *pShowCard=(SHOW_CARD_REQUEST*)pData;

	if (nChairNo != pShowCard->nChairNo)
	{
		UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GAME_MSG_SHOW_CARD"), pMsg->nRoomID, pTable->m_nTableNO, pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}

	SHOW_CARD_RESPONSE showCardResponse;
	memset(&showCardResponse, -1, sizeof(SHOW_CARD_RESPONSE));
	showCardResponse.nChairNo = pShowCard->nChairNo;

	if (!pTable->OnShowCard(pShowCard, &showCardResponse))
		return UR_OPERATE_FAILED;
	
	NotifyPlayerMsgAndResponse(lpContext, pTable, GAME_MSG_SEND_EVERYONE, GAME_MSG_SHOW_CARD, sizeof(SHOW_CARD_RESPONSE), &showCardResponse);
	return UR_OPERATE_SUCCEEDED;
}

UINT CGameServer::OnGiveUp(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable)
{
	GAME_MSG* pMsg = LPGAME_MSG(PBYTE(lpRequest->pDataPtr));
	BYTE* pData = PBYTE(lpRequest->pDataPtr)+sizeof(GAME_MSG);
	
	int nShouldDataLen=sizeof(GAME_MSG)+sizeof(GIVE_UP_REQUEST);
	if (lpRequest->nDataLen!=nShouldDataLen)
	{
		UwlLogFile(_T("GIVE_UP_REQUEST结构长度不对, UserID=%ld"), pMsg->nUserID);
		return UR_OPERATE_FAILED;	
	}
	
	GIVE_UP_REQUEST *pGiveUp=(GIVE_UP_REQUEST*)pData;

	if (nChairNo != pGiveUp->nChairNo)
	{
		UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GAME_MSG_GIVE_UP"), pMsg->nRoomID, pTable->m_nTableNO, pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}

	if (m_nOpen >= 1 && pTable->GetCurrentChair() != pGiveUp->nChairNo)
	{
		UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GAME_MSG_GIVE_UP"), pMsg->nRoomID, pTable->m_nTableNO, pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}

	GIVE_UP_RESPONSE giveUpResponse;
	memset(&giveUpResponse, -1, sizeof(GIVE_UP_RESPONSE));
	giveUpResponse.nChairNo = pGiveUp->nChairNo;

	if (!pTable->OnGiveUp(pGiveUp, &giveUpResponse))
		return UR_OPERATE_FAILED;
	
	NotifyPlayerMsgAndResponse(lpContext, pTable, GAME_MSG_SEND_EVERYONE, GAME_MSG_GIVE_UP, sizeof(GIVE_UP_RESPONSE), &giveUpResponse);

	return UR_OPERATE_SUCCEEDED;
}

UINT CGameServer::OnBet(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable)
{
	GAME_MSG* pMsg = LPGAME_MSG(PBYTE(lpRequest->pDataPtr));
	BYTE* pData = PBYTE(lpRequest->pDataPtr)+sizeof(GAME_MSG);

	int nShouldDataLen=sizeof(GAME_MSG)+sizeof(BET_REQUEST);
	if (lpRequest->nDataLen!=nShouldDataLen)
	{
		UwlLogFile(_T("BET_REQUEST结构长度不对, UserID=%ld"), pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}

	BET_REQUEST *pBet=(BET_REQUEST*)pData;

	if (nChairNo != pBet->nChairNo)
	{
		UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GAME_MSG_BET"), pMsg->nRoomID, pTable->m_nTableNO, pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}

	BET_RESPONSE betResponse;
	memset(&betResponse, -1, sizeof(BET_RESPONSE));
	betResponse.nChairNo = pBet->nChairNo;

	if (!pTable->OnBet(pBet, &betResponse))
		return UR_OPERATE_FAILED;
	
	NotifyPlayerMsgAndResponse(lpContext, pTable, GAME_MSG_SEND_EVERYONE, GAME_MSG_BET, sizeof(BET_RESPONSE), &betResponse);

	return UR_OPERATE_SUCCEEDED;
}

UINT CGameServer::OnRise(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable)
{
	GAME_MSG* pMsg = LPGAME_MSG(PBYTE(lpRequest->pDataPtr));
	BYTE* pData = PBYTE(lpRequest->pDataPtr)+sizeof(GAME_MSG);
	
	int nShouldDataLen=sizeof(GAME_MSG)+sizeof(BET_REQUEST);
	if (lpRequest->nDataLen!=nShouldDataLen)
	{
		UwlLogFile(_T("BET_REQUEST结构长度不对, UserID=%ld"), pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}
	
	BET_REQUEST *pBet=(BET_REQUEST*)pData;

	if (nChairNo != pBet->nChairNo)
	{
		UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GAME_MSG_RISE"), pMsg->nRoomID, pTable->m_nTableNO, pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}

	BET_RESPONSE betResponse;
	memset(&betResponse, -1, sizeof(BET_RESPONSE));
	betResponse.nChairNo = pBet->nChairNo;
	
	if (!pTable->OnRise(pBet, &betResponse))
		return UR_OPERATE_FAILED;
	
	NotifyPlayerMsgAndResponse(lpContext, pTable, GAME_MSG_SEND_EVERYONE, GAME_MSG_RISE, sizeof(BET_RESPONSE), &betResponse);
	
	return UR_OPERATE_SUCCEEDED;
}

UINT CGameServer::OnFollowBet(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable)
{
	GAME_MSG* pMsg = LPGAME_MSG(PBYTE(lpRequest->pDataPtr));
	BYTE* pData = PBYTE(lpRequest->pDataPtr)+sizeof(GAME_MSG);
	
	int nShouldDataLen=sizeof(GAME_MSG)+sizeof(FOLLOW_BET_REQUEST);
	if (lpRequest->nDataLen!=nShouldDataLen)
	{
		UwlLogFile(_T("BET_REQUEST结构长度不对, UserID=%ld"), pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}
	
	FOLLOW_BET_REQUEST *pFollowBet=(FOLLOW_BET_REQUEST*)pData;

	if (nChairNo != pFollowBet->nChairNo)
	{
		UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GAME_MSG_FOLLOW"), pMsg->nRoomID, pTable->m_nTableNO, pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}

	FOLLOW_BET_RESPONSE followBetResponse;
	memset(&followBetResponse, -1, sizeof(FOLLOW_BET_RESPONSE));
	followBetResponse.nChairNo = pFollowBet->nChairNo;

	if (!pTable->OnFollowBet(pFollowBet, &followBetResponse))
		return UR_OPERATE_FAILED;
	
	NotifyPlayerMsgAndResponse(lpContext, pTable, GAME_MSG_SEND_EVERYONE, GAME_MSG_FOLLOW, sizeof(FOLLOW_BET_RESPONSE), &followBetResponse);

	return UR_OPERATE_SUCCEEDED;
}

UINT CGameServer::OnCompare(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable, int& nLoseChair)
{
	GAME_MSG* pMsg = LPGAME_MSG(PBYTE(lpRequest->pDataPtr));
	BYTE* pData = PBYTE(lpRequest->pDataPtr)+sizeof(GAME_MSG);
	
	int nShouldDataLen=sizeof(GAME_MSG)+sizeof(COMPARE_CARD_REQUEST);
	if (lpRequest->nDataLen!=nShouldDataLen)
	{
		UwlLogFile(_T("BET_REQUEST结构长度不对, UserID=%ld"), pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}
	
	COMPARE_CARD_REQUEST *pCompare = (COMPARE_CARD_REQUEST*)pData;

	if (nChairNo != pCompare->nChairNo)
	{
		UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GAME_MSG_COMPARE"), pMsg->nRoomID, pTable->m_nTableNO, pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}

	COMPARE_CARD_RESPONSE compareCardResponse;
	memset(&compareCardResponse, -1, sizeof(COMPARE_CARD_RESPONSE));
	compareCardResponse.nChairNo = pCompare->nChairNo;
	compareCardResponse.nGoalChairNo = pCompare->nGoalChairNo;

	if (!pTable->OnCompare(pCompare, &compareCardResponse))
		return UR_OPERATE_FAILED;
	
	nLoseChair = compareCardResponse.nLostChairNo;
	NotifyPlayerMsgAndResponse(lpContext, pTable, GAME_MSG_SEND_EVERYONE, GAME_MSG_COMPARE, sizeof(COMPARE_CARD_RESPONSE), &compareCardResponse);

	return UR_OPERATE_SUCCEEDED;
}

UINT CGameServer::OnAllIn(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable, ALL_IN_RESPONSE& allInResponse)
{
	GAME_MSG* pMsg = LPGAME_MSG(PBYTE(lpRequest->pDataPtr));
	BYTE* pData = PBYTE(lpRequest->pDataPtr)+sizeof(GAME_MSG);
	
	int nShouldDataLen=sizeof(GAME_MSG)+sizeof(ALL_IN_REQUEST);
	if (lpRequest->nDataLen!=nShouldDataLen)
	{
		UwlLogFile(_T("ALL_IN_REQUEST结构长度不对, UserID=%ld"), pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}
	
	ALL_IN_REQUEST *pAllIn = (ALL_IN_REQUEST*)pData;

	if (nChairNo != pAllIn->nChairNo)
	{
		UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GAME_MSG_ALL_IN"), pMsg->nRoomID, pTable->m_nTableNO, pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}

	if (!pTable->OnAllIn(pAllIn, &allInResponse))
		return UR_OPERATE_FAILED;
	
	NotifyPlayerMsgAndResponse(lpContext, pTable, GAME_MSG_SEND_EVERYONE, GAME_MSG_ALL_IN, sizeof(ALL_IN_RESPONSE), &allInResponse);

	return UR_OPERATE_SUCCEEDED;
}

UINT CGameServer::OnBetFull(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable)
{
	GAME_MSG* pMsg = LPGAME_MSG(PBYTE(lpRequest->pDataPtr));
	BYTE* pData = PBYTE(lpRequest->pDataPtr)+sizeof(GAME_MSG);
	
	int nShouldDataLen=sizeof(GAME_MSG)+sizeof(BET_FULL_REQUEST);
	if (lpRequest->nDataLen!=nShouldDataLen)
	{
		UwlLogFile(_T("BET_FULL_REQUEST结构长度不对, UserID=%ld"), pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}

	if (FALSE == pTable->GetPublicInfo()->bOpenBetFull)
	{
		UwlLogFile(_T("OnBetFull failed, BetFull do not open, UserID=%ld"), pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}
	
	BET_FULL_REQUEST *pBetFull = (BET_FULL_REQUEST*)pData;
	
	if (nChairNo != pBetFull->nChairNo)
	{
		UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GAME_MSG_ALL_IN"), pMsg->nRoomID, pTable->m_nTableNO, pMsg->nUserID);
		return UR_OPERATE_FAILED;
	}
	
	BET_FULL_RESPONSE betFullResponse;
	memset(&betFullResponse, 0, sizeof(BET_FULL_RESPONSE));
	betFullResponse.nChairNo = pBetFull->nChairNo;
	betFullResponse.bNextChair = -1;
	betFullResponse.nGoalCompareChair = -1;

	if (!pTable->OnBetFull(pBetFull, &betFullResponse))
		return UR_OPERATE_FAILED;
	
	NotifyPlayerMsgAndResponse(lpContext, pTable, GAME_MSG_SEND_EVERYONE, GAME_MSG_BET_FULL, sizeof(BET_FULL_RESPONSE), &betFullResponse);
	
	return UR_OPERATE_SUCCEEDED;
}

BOOL CGameServer::OnChangeSeatedToLookOn(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	SOCKET sock = INVALID_SOCKET;
	LONG token = 0;
	int gameid = 0;
	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	int r_id = 0;
	int t_no = INVALID_OBJECT_ID;
	int u_id = 0;
	int c_no = INVALID_OBJECT_ID;
	LONG room_tokenid = 0;
	
	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;
	BOOL bPassive = IS_BIT_SET(lpContext->dwFlags,CH_FLAG_SYSTEM_EJECT);//是否系统自己生成的消息
	
	sock = lpContext->hSocket;
	token = lpContext->lTokenID;
	
	response.head.nRequest = UR_OPERATE_FAILED;
	
	if (lpRequest->nDataLen!=sizeof(LOOK_ON_AND_SEATED_CHANGE)){
		UwlLogFile(_T("OnChangeSeatedToLookOn failed. lpRequest->nDataLen is wrong."));
		return SendUserResponse(lpContext, &response);
	}
	
	LPLOOK_ON_AND_SEATED_CHANGE lpLookOnAndSeatedChange = (LPLOOK_ON_AND_SEATED_CHANGE)lpRequest->pDataPtr;
	gameid = m_nGameID;
	roomid	= lpLookOnAndSeatedChange->nRoomID;
	tableno = lpLookOnAndSeatedChange->nTableNO;
	userid	= lpLookOnAndSeatedChange->nUserID;
	chairno = lpLookOnAndSeatedChange->nChairNO;

	if(roomid <= 0 || tableno < 0 || userid <= 0 || chairno < 0 || gameid != m_nGameID
		|| chairno >=MAX_CHAIR_COUNT){	
		UwlLogFile(_T("OnChangeSeatedToLookOn failed. RoomID:%ld, TableNo:%ld, UserID:%ld, ChairNo:%ld, GameID:%ld"), roomid, tableno, userid, chairno, m_nGameID);
		return SendUserResponse(lpContext, &response);
	}

	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			UwlLogFile(_T("OnChangeSeatedToLookOn failed. RoomID(%ld) is not exist."), roomid);
			return SendUserResponse(lpContext, &response);
		}
	}
	
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		if(!pRoom->m_mapTable.Lookup(tableno, (CTable*&)pTable)){
			UwlLogFile(_T("OnChangeSeatedToLookOn failed. tableno(%ld) is not exist."), tableno);
			return SendUserResponse(lpContext, &response);
		}

		USER_DATA user_data;
		memset(&user_data,0,sizeof(user_data));
		
		if (!m_mapUserData.Lookup(userid,user_data)
			&&bPassive==FALSE)
		{
			UwlLogFile(_T("user:%ld未在服务器注册，试图发送消息号为:GR_SEATED_TO_LOOK_ON"),userid);
			return NotifyResponseFaild(lpContext);
		}
		else
		{
			roomid=user_data.nRoomID;
			tableno=user_data.nTableNO;
			chairno=user_data.nChairNO;
			
			if (user_data.nChairNO != lpLookOnAndSeatedChange->nChairNO)
			{
				UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GR_SEATED_TO_LOOK_ON"), user_data.nRoomID, user_data.nTableNO, user_data.nUserID);
				return NotifyResponseFaild(lpContext);
			}	
		}
	}
	
	if(pTable) {
		LOCK_TABLE(pTable, chairno, FALSE, userid, token);
		
		pTable->m_mapUser.Lookup(userid, pPlayer);
		if (!pPlayer || pPlayer->m_nUserID!=userid){
			UwlLogFile(_T("OnChangeSeatedToLookOn failed. UserID(%ld) is not same."), userid);
			return SendUserResponse(lpContext, &response);
		}

		if (TRUE == pPlayer->m_bLookOn)
		{
			UwlLogFile(_T("OnChangeSeatedToLookOn failed. Player already look on. UserID:%ld"), userid);
			response.head.nRequest = UR_CHANGE_LOOKON_OR_SEATED_SAME_STATUS;
			return SendUserResponse(lpContext, &response);
		}

		if(IS_BIT_SET(pTable->m_dwStatus, TS_PLAYING_GAME)){
			if (!CanChangeTableInGame(roomid, pPlayer))
			{
				UwlLogFile(_T("OnChangeLookOnToSeated failed. Player can not look on now. RoomID:%ld, TableNo:%ld, UserID:%ld"), roomid, tableno, userid);
				return SendUserResponse(lpContext, &response);
			}
		}

		if (!IsSoloRoom(roomid) || !IsLeaveAlone(roomid)){
			UwlLogFile(_T("OnChangeLookOnToSeated failed. Room is not Solo or not LeaveAlone. RoomID:%ld"), roomid);
			return SendUserResponse(lpContext, &response);
		}

		if (IsRandomRoom(roomid)){ //随机房不得主动旁观
			UwlLogFile(_T("OnChangeLookOnToSeated failed. Random room can not look on by user. RoomID:%ld, UserID:%ld"), roomid, userid);
			return SendUserResponse(lpContext, &response);
		}

		if (pTable->m_mapVisitors[chairno].GetCount() >= MAX_VISITOR_COUNT)
		{
			UwlLogFile(_T("OnChangeSeatedToLookOn failed. Visitor count >= %ld, can not look on."), MAX_VISITOR_COUNT);
			response.head.nRequest = UR_CHANGE_LOOKON_OR_SEATED_FULL_LOOK_ON;
			return SendUserResponse(lpContext, &response);
		}

		if (!IS_BIT_SET(pPlayer->m_nUserType, UT_HANDPHONE)) //移动端不做验证
		{
			if (!VerifyAskNewTableChair(userid)) //时间间隔验证，换桌、旁观和坐下的时间验证用的是一个时间
			{
				response.head.nRequest = UR_CHANGE_LOOKON_OR_SEATED_SPANSHORT;
				return SendUserResponse(lpContext, &response);		
			}		
		}

		PostChangeSeatedToLookOn(roomid, tableno, chairno, userid);
	}
	
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	SendUserResponse(lpContext, &response);
	return TRUE;
}
			
BOOL CGameServer::OnChangeLookOnToSeated(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	SOCKET sock = INVALID_SOCKET;
	LONG token = 0;
	int gameid = 0;
	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	int r_id = 0;
	int t_no = INVALID_OBJECT_ID;
	int u_id = 0;
	int c_no = INVALID_OBJECT_ID;
	LONG room_tokenid = 0;
	
	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;
	BOOL bPassive = IS_BIT_SET(lpContext->dwFlags,CH_FLAG_SYSTEM_EJECT);//是否系统自己生成的消息
	
	sock = lpContext->hSocket;
	token = lpContext->lTokenID;
	
	response.head.nRequest = UR_OPERATE_FAILED;
	
	if (lpRequest->nDataLen!=sizeof(LOOK_ON_AND_SEATED_CHANGE)){
		UwlLogFile(_T("OnChangeLookOnToSeated failed. lpRequest->nDataLen is wrong."));
		return SendUserResponse(lpContext, &response);
	}
	
	LPLOOK_ON_AND_SEATED_CHANGE lpLookOnAndSeatedChange = (LPLOOK_ON_AND_SEATED_CHANGE)lpRequest->pDataPtr;
	gameid = m_nGameID;
	roomid = lpLookOnAndSeatedChange->nRoomID;
	tableno = lpLookOnAndSeatedChange->nTableNO;
	userid = lpLookOnAndSeatedChange->nUserID;
	chairno = lpLookOnAndSeatedChange->nChairNO;

	if(roomid <= 0 || tableno < 0 || userid <= 0 || chairno < 0 || gameid != m_nGameID
		|| chairno >=MAX_CHAIR_COUNT){	
		UwlLogFile(_T("OnChangeLookOnToSeated failed. RoomID:%ld, TableNo:%ld, UserID:%ld, ChairNo:%ld, GameID:%ld"), roomid, tableno, userid, chairno, m_nGameID);
		return SendUserResponse(lpContext, &response);
	}

	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			UwlLogFile(_T("OnChangeLookOnToSeated failed. RoomID(%ld) is not exist."), roomid);
			return SendUserResponse(lpContext, &response);
		}
	}
	
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		if(!pRoom->m_mapTable.Lookup(tableno, (CTable*&)pTable)){
			UwlLogFile(_T("OnChangeLookOnToSeated failed. TableNo(%ld) is not exist."), tableno);
			return SendUserResponse(lpContext, &response);
		}

		USER_DATA user_data;
		memset(&user_data,0,sizeof(user_data));
		
		if (!m_mapUserData.Lookup(userid,user_data)
			&&bPassive==FALSE)
		{
			UwlLogFile(_T("user:%ld未在服务器注册，试图发送消息号为:GR_LOOK_ON_TO_SEATED"),userid);
			return NotifyResponseFaild(lpContext);
		}
		else
		{
			roomid=user_data.nRoomID;
			tableno=user_data.nTableNO;
			chairno=user_data.nChairNO;
			
			if (user_data.nChairNO != lpLookOnAndSeatedChange->nChairNO)
			{
				UwlLogFile(_T("RoomID:%ld, TableNO:%ld, UserID:%ld, 试图发送伪造消息号为:GR_LOOK_ON_TO_SEATED"), user_data.nRoomID, user_data.nTableNO, user_data.nUserID);
				return NotifyResponseFaild(lpContext);
			}	
		}
	}
	
	if(pTable) {
		LOCK_TABLE(pTable, chairno, FALSE, userid, token);
		
		pTable->m_mapUser.Lookup(userid, pPlayer);
		if (!pPlayer || pPlayer->m_nUserID!=userid){
			UwlLogFile(_T("OnChangeLookOnToSeated failed. UserID(%ld) is not same."), userid);
			return SendUserResponse(lpContext, &response);
		}

		if (FALSE == pPlayer->m_bLookOn)
		{//相同状态
			UwlLogFile(_T("OnChangeLookOnToSeated failed. Player already seated. UserID:%ld"), userid);
			response.head.nRequest = UR_CHANGE_LOOKON_OR_SEATED_SAME_STATUS;
			return SendUserResponse(lpContext, &response);
		}
		
		if (pPlayer->m_nDeposit < GetMinDeposit(roomid))
		{//银子不够
			response.head.nRequest = UR_CHANGE_LOOKON_OR_SEATED_DEPOSIT_NOT_ENOUGH;
			return SendUserResponse(lpContext, &response);
		}
	
		if(IS_BIT_SET(pTable->m_dwStatus, TS_PLAYING_GAME)){
			if (!CanChangeTableInGame(roomid, pPlayer))
			{
				UwlLogFile(_T("OnChangeLookOnToSeated failed. Player can not seat now. RoomID:%ld, TableNo:%ld, UserID:%ld"), roomid, tableno, userid);
				return SendUserResponse(lpContext, &response);
			}
		}

		int nEmptyChairCount = 0;
		for (int i=0; i<TOTAL_CHAIRS; i++)
		{
			if (NULL == pTable->m_ptrPlayers[i])
				nEmptyChairCount++;
		}
		
		if (0 == nEmptyChairCount)
		{//没有空的座位了
			UwlLogFile(_T("OnChangeLookOnToSeated failed. table has no empty chair."));
			response.head.nRequest = UR_CHANGE_LOOKON_OR_SEATED_NO_EMPTY_CHAIR;
			return SendUserResponse(lpContext, &response);
		}

		if (!IsSoloRoom(roomid) || !IsLeaveAlone(roomid)){
			UwlLogFile(_T("OnChangeLookOnToSeated failed. Room is not Solo or not LeaveAlone. RoomID:%ld"), roomid);
			return SendUserResponse(lpContext, &response);
		}

		if (IsRandomRoom(roomid)){ //随机房不得主动坐下
			UwlLogFile(_T("OnChangeLookOnToSeated failed. Random room can not seat by user. RoomID:%ld, UserID:%ld"), roomid, userid);
			return SendUserResponse(lpContext, &response);
		}

		if (!IS_BIT_SET(pPlayer->m_nUserType, UT_HANDPHONE)) //移动端不做验证
		{
			if (!VerifyAskNewTableChair(userid)) //时间间隔验证，换桌、旁观和坐下的时间验证用的是一个时间
			{
				response.head.nRequest = UR_CHANGE_LOOKON_OR_SEATED_SPANSHORT;
				return SendUserResponse(lpContext, &response);		
			}		
		}

		PostChangeLookOnToSeated(roomid, tableno, chairno, userid);
	}
	
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	SendUserResponse(lpContext, &response);
	return TRUE;
}

void CGameServer::OnChangeSeatedToLookOnResult(int roomid, int tableno, int chairno, int userid)
{
	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;

	LOOK_ON_AND_SEATED_CHANGE_RESULT lookOnAndSeatedChangeResult;
	memset(&lookOnAndSeatedChangeResult, 0, sizeof(LOOK_ON_AND_SEATED_CHANGE_RESULT));
	lookOnAndSeatedChangeResult.nTableNO = tableno;
	lookOnAndSeatedChangeResult.nUserID = userid;
	lookOnAndSeatedChangeResult.nChairNO = chairno;

	USER_DATA user_data;
	memset(&user_data, 0, sizeof(user_data));
	if (!LookupUserData(userid, user_data))
		return;

	if (roomid != user_data.nRoomID)
		return;

	if (!IsSoloRoom(roomid) || IsRandomRoom(roomid))
		return;
	
	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			return;
		}
	}
	
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		//清理桌子，以RoomSvr为准
		if(!pRoom->m_mapTable.Lookup(tableno, pTable))
		{
			//桌子找不到,新建
			pTable = OnNewTable(roomid, tableno, m_nScoreMult);
			pRoom->m_mapTable.SetAt(tableno, pTable);
		}
	}

	if (pTable)
	{
		CAutoLock lock(&(pTable->m_csTable));
		pTable->m_mapUser.Lookup(userid, pPlayer);

		if (NULL != pPlayer)
		{
			if (NULL == pTable->m_ptrPlayers[chairno])
			{
				UwlLogFile(_T("OnChangeSeatedToLookOnResult failed. pTable->m_ptrPlayers[chairno] is NULL."));
				lookOnAndSeatedChangeResult.bSucceed = FALSE;
				NotifyOneUser(pPlayer->m_hSocket, pPlayer->m_lTokenID, GR_CHANGE_SEATED_TO_LOOK_ON_RESULT, &lookOnAndSeatedChangeResult, sizeof(lookOnAndSeatedChangeResult));
				return;
			}

			pTable->m_ptrPlayers[chairno] = NULL;
			pTable->m_dwUserStatus[chairno] &= ~US_GAME_STARTED;
			pTable->m_mapVisitors[chairno].SetAt(userid, pPlayer);
			pPlayer->m_bLookOn = TRUE;
			pPlayer->m_bIdlePlayer = TRUE;

			int startcount=XygGetStartCount(pTable->m_dwUserStatus, pTable->m_nTotalChairs);
			if (pTable->IsNeedCountdown() && TRUE==pTable->IsInCountDown() && startcount<2)
			{
				START_COUNTDOWN  sc;
				memset(&sc,0,sizeof(START_COUNTDOWN));
				sc.nRoomID = pTable->m_nRoomID;
				sc.nTableNO = pTable->m_nTableNO;
				sc.bStartorStop = FALSE;
				sc.nCountDown = GetCountdown(pTable,pTable->m_nRoomID);
				pTable->SetCountDown(FALSE);
				NotifyTablePlayers(pTable,GR_START_COUNTDOWN,&sc,sizeof(START_COUNTDOWN));
				NotifyTableVisitors(pTable,GR_START_COUNTDOWN,&sc,sizeof(START_COUNTDOWN));
			}

			//Solo模式中通知其他客户端
			SOLO_PLAYER sp;
			memset(&sp,0,sizeof(sp));
			if (FALSE == LookupSoloPlayer(userid,sp))
			{
				lookOnAndSeatedChangeResult.bSucceed = FALSE;
				NotifyOneUser(pPlayer->m_hSocket, pPlayer->m_lTokenID, GR_CHANGE_SEATED_TO_LOOK_ON_RESULT, &lookOnAndSeatedChangeResult, sizeof(lookOnAndSeatedChangeResult));
				return;
			}

			sp.nStatus = ROOM_PLAYER_STATUS_LOOKON;
			SetSoloPlayer(userid, sp);
			
			lookOnAndSeatedChangeResult.bSucceed = TRUE;

			NotifyTablePlayers(pTable, GR_CHANGE_SEATED_TO_LOOK_ON_RESULT, &lookOnAndSeatedChangeResult, sizeof(lookOnAndSeatedChangeResult));
			NotifyTableVisitors(pTable, GR_CHANGE_SEATED_TO_LOOK_ON_RESULT, &lookOnAndSeatedChangeResult, sizeof(lookOnAndSeatedChangeResult));
		}
	}
}

void CGameServer::OnChangeLookOnToSeatedResult(int roomid, int tableno, int chairno, int userid)
{
	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;

	DWORD dwRoomOption=0;
	DWORD dwRoomConfig=0;
	
	LOOK_ON_AND_SEATED_CHANGE_RESULT lookOnAndSeatedChangeResult;
	memset(&lookOnAndSeatedChangeResult, 0, sizeof(LOOK_ON_AND_SEATED_CHANGE_RESULT));
	lookOnAndSeatedChangeResult.nTableNO = tableno;
	lookOnAndSeatedChangeResult.nUserID = userid;
	lookOnAndSeatedChangeResult.nChairNO = chairno;
	
	USER_DATA user_data;
	memset(&user_data, 0, sizeof(user_data));
	if (!LookupUserData(userid, user_data))
		return;
	
	if (roomid != user_data.nRoomID)
		return;
	
	if (!IsSoloRoom(roomid) || IsRandomRoom(roomid))
		return;
	
	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			return;
		}
	}
	
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		//清理桌子，以RoomSvr为准
		if(!pRoom->m_mapTable.Lookup(tableno, pTable))
		{
			//桌子找不到,新建
			pTable = OnNewTable(roomid, tableno, m_nScoreMult);
			pRoom->m_mapTable.SetAt(tableno, pTable);
		}

		dwRoomOption=GetRoomOption(roomid);
		dwRoomConfig=GetRoomConfig(roomid);
	}
	
	if (pTable)
	{
		CAutoLock lock(&(pTable->m_csTable));
		pTable->m_mapUser.Lookup(userid, pPlayer);
		
		if (INVALID_CHAIR_NO_EX == chairno)
		{
			lookOnAndSeatedChangeResult.bSucceed = FALSE;
			NotifyOneUser(pPlayer->m_hSocket, pPlayer->m_lTokenID, GR_CHANGE_LOOK_ON_TO_SEATED_RESULT, &lookOnAndSeatedChangeResult, sizeof(lookOnAndSeatedChangeResult));
			return;
		}

		if (NULL != pPlayer)
		{	
			pTable->RemoveVisitor(userid, FALSE);
			pTable->m_ptrPlayers[chairno] = pPlayer;
			pTable->m_dwUserStatus[chairno] &= ~US_GAME_STARTED;
			pTable->m_dwRoomConfig[chairno] = dwRoomConfig;
			pTable->m_dwRoomOption[chairno] = dwRoomOption;
			pPlayer->m_bLookOn = FALSE;
			pPlayer->m_nChairNO = chairno;

			USER_DATA user_data;
			memset(&user_data,0,sizeof(user_data));
			
			CAutoLock lock(&(pRoom->m_csRoom));
			if (m_mapUserData.Lookup(userid,user_data) && user_data.nChairNO!=chairno)
			{
				user_data.nChairNO = chairno;
				m_mapUserData.SetAt(userid, user_data);
			}
			
			//Solo模式中通知其他客户端
			SOLO_PLAYER sp;
			memset(&sp,0,sizeof(sp));
			if (FALSE == LookupSoloPlayer(userid,sp))
			{
				lookOnAndSeatedChangeResult.bSucceed = FALSE;
				NotifyOneUser(pPlayer->m_hSocket, pPlayer->m_lTokenID, GR_CHANGE_LOOK_ON_TO_SEATED_RESULT, &lookOnAndSeatedChangeResult, sizeof(lookOnAndSeatedChangeResult));
				return;
			}
			
			sp.nStatus = ROOM_PLAYER_STATUS_WAITING;
			SetSoloPlayer(userid, sp);
			
			lookOnAndSeatedChangeResult.bSucceed = TRUE;
			
			NotifyTablePlayers(pTable, GR_CHANGE_LOOK_ON_TO_SEATED_RESULT, &lookOnAndSeatedChangeResult, sizeof(lookOnAndSeatedChangeResult));
			NotifyTableVisitors(pTable, GR_CHANGE_LOOK_ON_TO_SEATED_RESULT, &lookOnAndSeatedChangeResult, sizeof(lookOnAndSeatedChangeResult));
		}
	}
}

void CGameServer::PostChangeSeatedToLookOn(int nRoomID, int nTableNO, int nChairNO, int nUserID)
{
	HWND hWnd=GetRoomSvrWnd(nRoomID);
	if (IsWindow(hWnd))
	{	
		DWORD dwRoomTableChair=MakeRoomTableChair(nRoomID,nTableNO,nChairNO);
		PostMessage(hWnd,WM_GTR_CHANGE_SEATED_TO_LOOK_ON_EX, (WPARAM)dwRoomTableChair, (LPARAM)nUserID); 
	}
}

void CGameServer::PostChangeLookOnToSeated(int nRoomID, int nTableNO, int nChairNO, int nUserID)
{
	HWND hWnd=GetRoomSvrWnd(nRoomID);
	if (IsWindow(hWnd))
	{	
		DWORD dwRoomTableChair=MakeRoomTableChair(nRoomID,nTableNO,nChairNO);
		PostMessage(hWnd,WM_GTR_CHANGE_LOOK_ON_TO_SEATED_EX, (WPARAM)dwRoomTableChair, (LPARAM)nUserID); 
	}
}

BOOL CGameServer::OnChatToTableEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	SOCKET sock = INVALID_SOCKET;
	LONG token = 0;
	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	int r_id = 0;
	int t_no = INVALID_OBJECT_ID;
	int u_id = 0;
	int c_no = INVALID_OBJECT_ID;
	BOOL lookon = FALSE;
	
	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;
	
	token = lpContext->lTokenID;
	sock = lpContext->hSocket;
	
	LPCHAT_TO_TABLE pChatToTable = LPCHAT_TO_TABLE(PBYTE(lpRequest->pDataPtr));
	roomid = pChatToTable->nRoomID;
	tableno = pChatToTable->nTableNO;
	userid = pChatToTable->nUserID;
	chairno = pChatToTable->nChairNO;
	{
		CAutoLock lock(&m_csRoomMap);
		
		if(!m_mapRoom.Lookup(roomid, pRoom)){
			return TRUE;
		}
	}
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));
		
		if(!pRoom->m_mapTable.Lookup(tableno, pTable)){
			return TRUE;
		}
	}
	if(pTable) 
	{
		LOCK_TABLE(pTable, chairno, FALSE, userid, token);

		if(lstrlen(pChatToTable->szChatMsg) > (MAX_CHATMSG_LEN-2) )
			return FALSE;
		
		int nMinExp=0;
		BOOL bAllowed=TRUE;
  		CString sForbid;

		CHAT_FROM_TABLE cft;
		memset(&cft,0,sizeof(cft));
		cft.nRoomID = roomid;
		cft.nUserID = userid;

		DWORD dwRoomOption=GetRoomOption(roomid);

		if(IS_BIT_SET(dwRoomOption,RO_FORBID_CHAT))// 禁止聊天
		{ 
			bAllowed=FALSE;
 			sForbid.Format(_T(" 本游戏禁止发消息"));
		}
		else if(pTable->IsForbidLookChat() && pTable->IsVisitor(userid))// 禁止旁观者聊天
		{ 
	 		bAllowed=FALSE;
 			sForbid.Format(_T(" 本游戏禁止旁观者发消息"));
		}
		else if(pTable->IsDarkRoom())// 隐名房间
		{ 
 			bAllowed=FALSE;
 			sForbid.Format(_T(" 本游戏禁止发消息"));
		}			
		else if(IsForbidChatByExp(pTable,userid,nMinExp))
		{
 			bAllowed=FALSE;
 			sForbid.Format(_T(" 新手(经验值＜%d分钟)禁止发消息"),nMinExp);
		}
		else if (pTable->IsVisitor(userid)/*&&((nRet=VerifyVistorSpeak(userid,roomid,tableno))<0)*/)
		{
			//验证旁观发言
			int nRet = VerifyVistorSpeak(userid,roomid,tableno,pChatToTable->szChatMsg);
			if (-1==nRet)
			{
				bAllowed=FALSE;
				sForbid.Format(_T(" 您说话太快了，请休息一会儿"));
			}
			else if (-2==nRet)
			{
				bAllowed=FALSE;
				sForbid.Format(_T(" 进入房间10分钟后，才可以旁观时发言"));
			}
			else if (-3==nRet)
			{
				bAllowed=FALSE;
				sForbid.Format(_T(" 禁止发送违禁单词或符号"));
			}
		}
 	 	else if(IsIncludeFilterWord(pChatToTable->szChatMsg))
		{
 			bAllowed=FALSE;
 			sForbid.Format(_T(" 禁止发送违禁单词或符号"));
		}
		else 
		{
		}
		 
		if(bAllowed==FALSE )
		{
 			sprintf(cft.szChatMsg,_T("%s%s\r\n"),GetChatTags(FLAG_CHAT_SYSNOTIFY),sForbid);
 			cft.nMsgLen = lstrlen(cft.szChatMsg)+1;  
 			cft.dwFlags	= FLAG_CHAT_SYSNOTIFY;
  			int nSendLen=sizeof(CHAT_FROM_TABLE)-sizeof(cft.szChatMsg)+cft.nMsgLen;
 			NotifyOneUser(sock,token,GR_CHAT_FROM_TALBE,&cft,nSendLen);
			return TRUE;
		}

		//Add on 20130122
		//记录旁观发言时间地点人物
		if (pTable->IsVisitor(userid))
		{
			SetVisitorLastSpeakInfo(userid,roomid,tableno);
		}
		//Add end
		
		sprintf(cft.szChatMsg,_T("%s%s\r\n"),GetChatTags(FLAG_CHAT_PLAYERMSG),pChatToTable->szChatMsg);
  		cft.nMsgLen=lstrlen(cft.szChatMsg)+1;
  		cft.dwFlags=FLAG_CHAT_PLAYERMSG;
		int nSendLen=sizeof(CHAT_FROM_TABLE)-sizeof(cft.szChatMsg)+cft.nMsgLen;
		cft.nReserved[0] = pChatToTable->nReserved[0];
		cft.nReserved[1] = pChatToTable->nReserved[1];
		cft.nReserved[2] = pChatToTable->nReserved[2];

  		NotifyTablePlayers(pTable, GR_CHAT_FROM_TALBE, &cft,nSendLen);
		NotifyTableVisitors(pTable, GR_CHAT_FROM_TALBE,&cft, nSendLen);
	}
	return TRUE;
}

BOOL CGameServer::OnGameWin(LPCONTEXT_HEAD lpContext, CRoom* pRoom,CTable* pTable, int chairno, BOOL bout_invalid, int roomid)
{
    if (pRoom && pRoom->IsOnMatch()){ //比赛
        return OnMatchGameWin(lpContext,pRoom,pTable,chairno,bout_invalid,roomid);
    }
    
    //等待标记 置1
    pTable->SetCheckingResult(TRUE); 
    
    //结算延迟
    if (NeedGameWinDelay(lpContext, pTable, chairno, bout_invalid, roomid)) 
    {			
        GAME_WIN_DELAY gameWin;	
        memset(&gameWin,0,sizeof(gameWin));
        if (ConstructDelayGameWin(lpContext,pTable,chairno,bout_invalid,roomid,gameWin,DEF_DELAY_GAMEWIN)){
            return DelayGameWin(lpContext,pTable,&gameWin,gameWin.dwDelay);
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    BOOL bWaitCheckRult=IsWaitCheckResult(roomid);
    pTable->SetStatusOnEnd();
    
    int nLen = pTable->GetGameWinSize();
    void* pData = new_byte_array(nLen);
    
    pTable->BuildUpGameWin(pData, nLen, chairno, bout_invalid, m_mapPlayerLevel);
    CalcResultWinOrLoss(pData, nLen, pTable);

    //////////////////////////////////////记录茶水费，玩家局数等数据
    LPGAME_WIN_RESULT pGameWin = (GAME_WIN_RESULT*)pData;
    RecordData(((CGameTable*)pTable), chairno, pGameWin->gamewin.nWinFees, pGameWin->gamewin.nDepositDiffs);
    //////////////////////////////////////记录茶水费，玩家局数等数据


    if(!bout_invalid && pTable->CheckInDB()) // 本局结果有效并且需要提交到数据库
    { 
        CheckInGameResult(lpContext, pTable, pData, nLen, roomid);
        
        //抽奖数据保存
        Lottery_SaveData(lpContext, pTable, roomid, chairno);
    }
    
    if (!bWaitCheckRult)
    {
        OpeAfterCheckResult(pTable, pData, nLen, roomid, FALSE);
    }
    else
    {
        //等待checksvr验证
        StoreUpGameWin(pTable,pData,nLen,chairno);
    }
    
    return TRUE;
}


BOOL CGameServer::OnUserBoutEnd(LPCONTEXT_HEAD lpContext, CRoom* pRoom,CTable* pTable, int chairno, BOOL bout_invalid, int roomid)
{
    if (!IsVariableChairRoom(roomid))
        return FALSE;
    
    if (!pTable)
        return FALSE;
    
    CPlayer* ptrPlayer	= pTable->m_ptrPlayers[chairno];
    
    if (!ptrPlayer)
        return FALSE;
    
    if (ptrPlayer->m_bIdlePlayer)
        return FALSE;
    
    USER_BOUTEND userEnd;
    memset(&userEnd,0,sizeof(userEnd));
    pTable->BuildUpUserBoutEnd(&userEnd, sizeof(userEnd), chairno,bout_invalid, m_mapPlayerLevel);
    
    RecordDataOne(((CGameTable*)pTable), chairno, userEnd.nWinFee, userEnd.nDepositDiff);


    if(!bout_invalid && pTable->CheckInDB()){ // 本局结果有效并且需要提交到数据库
        REFRESH_RESULT_EX RefreshResult;
        ZeroMemory(&RefreshResult, sizeof(RefreshResult));
        GAME_RESULT_EX GameResults;
        ZeroMemory(&GameResults, sizeof(GameResults));
        RefreshResult.nTableNO	=	pTable->m_nTableNO;
        pTable->ConstructPreSaveResult(&userEnd, sizeof(userEnd), roomid, chairno, m_nGameID, &RefreshResult, &GameResults);
        pTable->RefreshOnePlayerData(&GameResults);
        pTable->m_bResultPreSaved = TRUE;
        TransmitGameResultEx(pTable, lpContext, &RefreshResult, &GameResults, sizeof(GAME_RESULT_EX));
        //		TransmitGameResult(lpContext, &RefreshResult, &GameResults, sizeof(GAME_RESULT_EX));
        
        NotifyUserBoutEnd(pTable, &userEnd, 0);
    }
    
    // 	if (!bWaitCheckRult)
    // 	{
    // 		//不等待checksvr验证
    // 		NotifyTablePlayers(pTable, GR_GAME_WIN, pData, nLen, 0, TRUE);
    // 		NotifyTableVisitors(pTable, GR_GAME_WIN, pData, nLen, 0, TRUE);
    // 		
    // 		pTable->PrepareNextBout(pData, nLen);
    // 		
    // 		SAFE_DELETE(pData);
    // 		
    // 		if (IsSoloRoom(roomid)
    // 			&&pTable->IsGameOver())
    // 		{
    // 			if (IsVariableChairRoom(pTable->m_nRoomID))
    // 			{
    // 				//通知房间游戏结束
    // 				int nChairStatus[MAX_CHAIRS_PER_TABLE];
    // 				memset(&nChairStatus,0,sizeof(nChairStatus));
    // 				for (int i=0; i<MAX_CHAIRS_PER_TABLE; i++)
    // 				{	
    // 					if (pTable->m_ptrPlayers[i]
    // 						&& !(pTable->m_ptrPlayers[i]->m_bIdlePlayer))
    // 						nChairStatus[i] = 1;
    // 				}
    // 				DWORD dwTableChairStatus = MakeTableChairStatus(pTable->m_nTableNO, nChairStatus, MAX_CHAIRS_PER_TABLE);
    // 				PostSoloGameWin(pTable->m_nRoomID,dwTableChairStatus);
    // 			}
    // 			else
    // 				PostSoloGameWin(pTable->m_nRoomID,pTable->m_nTableNO);	//通知房间游戏结束
    // 		}
    // 	}
    // 	else
    // 	{
    // 		//等待checksvr验证
    // 		StoreUpGameWin(pTable,pData,nLen,chairno);
    // 	}
    
    ptrPlayer->m_bIdlePlayer=TRUE;
    PostSoloUserBoutEnd(ptrPlayer->m_nUserID,roomid);
    
    if (IsSoloRoom(roomid)
        && IsLeaveAlone(roomid)
        && !IsRandomRoom(roomid))
    {
        //可变桌椅的房间，主动退掉已经掉线的结算玩家
        if (pTable->m_ptrPlayers[chairno]
            && pTable->m_dwBreakTime[chairno]>0)
        {
            RemoveOneClients(pTable,pTable->m_ptrPlayers[chairno]->m_nUserID,TRUE);
            EjectLeaveGameEx(pTable->m_ptrPlayers[chairno]->m_nUserID, roomid, pTable->m_nTableNO, 
                chairno, pTable->m_ptrPlayers[chairno]->m_hSocket, pTable->m_ptrPlayers[chairno]->m_lTokenID);
        }
    }
    
    return TRUE;
}

BOOL CGameServer::OnStartGame(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LONG token = 0;
	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	int r_id = 0;
	int t_no = INVALID_OBJECT_ID;
	int u_id = 0;
	int c_no = INVALID_OBJECT_ID;
	
	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;
	
	token = lpContext->lTokenID;

	LPSTART_GAME pStartGame = LPSTART_GAME(PBYTE(lpRequest->pDataPtr));
	roomid = pStartGame->nRoomID;
	tableno = pStartGame->nTableNO;
	userid = pStartGame->nUserID;
	chairno = pStartGame->nChairNO;

	{
		CAutoLock lock(&m_csRoomMap);

		if(!m_mapRoom.Lookup(roomid, pRoom)){
			return TRUE;
		}
	}
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));

		if(!pRoom->m_mapTable.Lookup(tableno, pTable)){
			return TRUE;
		}
	}
	if(pTable) {
		LOCK_TABLE(pTable, chairno, FALSE, userid, token);
		
		if(!pTable->IsPlayer(userid)){ // 不是玩家
			return TRUE;
		}

		if(IS_BIT_SET(pTable->m_dwStatus, TS_PLAYING_GAME)){ // 游戏进行中
			return TRUE;
		}

		if(IS_BIT_SET(pTable->m_dwUserStatus[chairno], US_GAME_STARTED)){ // 已经开始
			return TRUE;
		}

		BOOL bSoloRoom=IsSoloRoom(roomid);
		BOOL bRandomRoom=IsRandomRoom(roomid);
		
		// 		//随机Solo房间，第一局不能自动开始
		// 		if (pTable->IsFirstBout()
		// 			&&bSoloRoom
		// 			&&bRandomRoom)
		// 		{
		// 			UwlLogFile("Can't StartGameSelf In Random_SoloRoom First Bout,userid:%ld,tableno:%ld,chairno:%ld",userid,tableno,chairno);
		// 			return TRUE;
		// 		}
		
		if(!VerifyRoomTableChair(roomid, tableno, chairno, userid)){ // 该位置上用户不匹配
			return TRUE;
		}
		//
		if (bSoloRoom)
		{
			//Modify on 20130106
			//独离模式下，按下开始时，判断下银子是否足够达到房间下限，不够清退
			if (IsLeaveAlone(roomid))
			{
				pPlayer=pTable->m_ptrPlayers[chairno];
				if (pPlayer	&& IsNeedDepositRoom(roomid))
				{
					int nMinDeposit=GetMinPlayingDeposit(pTable, roomid);
					
					if (nMinDeposit > pPlayer->m_nDeposit)
					{
						DEPOSIT_NOT_ENOUGH depositNotEnough;
						ZeroMemory(&depositNotEnough, sizeof(depositNotEnough));
						
						depositNotEnough.nUserID = pPlayer->m_nUserID;
						depositNotEnough.nChairNO = chairno;
						depositNotEnough.nDeposit = pPlayer->m_nDeposit;
						depositNotEnough.nMinDeposit = nMinDeposit;	
						
						NotifyOneUser(pPlayer->m_hSocket,pPlayer->m_lTokenID,GR_DEPOSIT_NOT_ENOUGH,&depositNotEnough, sizeof(depositNotEnough));
						return FALSE;
					}
				}
			}
			
			if (bRandomRoom
				&& (pTable->IsFirstBout()
				||(IsLeaveAlone(roomid)
				&&IsNeedWaitArrageTable(pTable,roomid,userid))))//已经达到桌局数上限，那么重新向RoomSvr请求分桌
			{
				pTable->m_dwUserStatus[chairno]|=US_USER_WAITNEWTABLE; //pengsy
				PostAskNewTable(userid,roomid,tableno,chairno);
				
				pPlayer=pTable->m_ptrPlayers[chairno];
				if (pPlayer)
				{
					NotifyOneUser(pPlayer->m_hSocket,pPlayer->m_lTokenID,GR_WAIT_NEWTABLE,NULL,0);
				}
				
				return TRUE;
			}
			//Modify end
		}
		//
		if (IS_BIT_SET(GetGameOption(roomid),GO_NOT_VERIFYSTART))
		{
			OnUserStart(pTable,chairno);
		}
		else
		{
			PostVerifyStart(userid,roomid,tableno,chairno);
		}
	}
	return TRUE;
}

BOOL CGameServer::OnStartGameEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	LONG token = 0;
	int roomid = 0;
	int tableno = INVALID_OBJECT_ID;
	int userid = 0;
	int chairno = INVALID_OBJECT_ID;
	int r_id = 0;
	int t_no = INVALID_OBJECT_ID;
	int u_id = 0;
	int c_no = INVALID_OBJECT_ID;
	
	CRoom* pRoom = NULL;
	CTable* pTable = NULL;
	CPlayer* pPlayer = NULL;
	
	token = lpContext->lTokenID;

	LPSTART_GAME pStartGame = LPSTART_GAME(PBYTE(lpRequest->pDataPtr));
	roomid = pStartGame->nRoomID;
	tableno = pStartGame->nTableNO;
	userid = pStartGame->nUserID;
	chairno = pStartGame->nChairNO;

	BOOL bPassive = IS_BIT_SET(lpContext->dwFlags,CH_FLAG_SYSTEM_EJECT);
	response.head.nRequest = UR_OPERATE_FAILED;

	{
		CAutoLock lock(&m_csRoomMap);

		if(!m_mapRoom.Lookup(roomid, pRoom)){
			return SendUserResponse(lpContext, &response,bPassive);
		}
	}
	if(pRoom){
		CAutoLock lock(&(pRoom->m_csRoom));

		if(!pRoom->m_mapTable.Lookup(tableno, pTable)){
			return SendUserResponse(lpContext, &response,bPassive);
		}
	}
	if(pTable) {
		LOCK_TABLE(pTable, chairno, FALSE, userid, token);
		
		if(!pTable->IsPlayer(userid)){ // 不是玩家
			return SendUserResponse(lpContext, &response,bPassive);
		}

		if(IS_BIT_SET(pTable->m_dwStatus, TS_PLAYING_GAME)){ // 游戏进行中
			return SendUserResponse(lpContext, &response,bPassive);
		}

		if(IS_BIT_SET(pTable->m_dwUserStatus[chairno], US_GAME_STARTED)){ // 已经开始
			return SendUserResponse(lpContext, &response,bPassive);
		}

		if(IS_BIT_SET(pTable->m_dwUserStatus[chairno], US_USER_WAITNEWTABLE)){ // 已经进入分桌队列
			return SendUserResponse(lpContext, &response,bPassive);
		}

		BOOL bSoloRoom=IsSoloRoom(roomid);
		BOOL bRandomRoom=IsRandomRoom(roomid);
		
		if(!VerifyRoomTableChair(roomid, tableno, chairno, userid)){ // 该位置上用户不匹配
			return SendUserResponse(lpContext, &response,bPassive);
		}
		//
		if (bSoloRoom) 
		{			
			if (IsLeaveAlone(roomid))
			{
				//solo独离房, 按下开始时, 判断银子是否在房间限定范围内,否则返回超出银两值				
				//add by thg, 20130802
				pPlayer=pTable->m_ptrPlayers[chairno];
				if (pPlayer && IsNeedDepositRoom(roomid))
				{
					//检查银两最低要求
					BOOL bDepositNotEnough = FALSE;
					int nMinDeposit = 0;
					if (IsCheckDepositMin(roomid)) 
					{
						nMinDeposit=GetMinDeposit(roomid); 
						if (pPlayer->m_nDeposit < nMinDeposit) //银子不够
						{
							bDepositNotEnough = TRUE;
						}
					}
					else
					{
						nMinDeposit = GetMinPlayingDeposit(pTable, roomid);
						if (pPlayer->m_nDeposit < nMinDeposit) //银子不够
						{
							bDepositNotEnough = TRUE;
						}
					}
					if (bDepositNotEnough)
					{
						DEPOSIT_NOT_ENOUGH dne;
						memset(&dne, 0, sizeof(dne));
						dne.nUserID = userid;
						dne.nChairNO = chairno;
						dne.nDeposit = pPlayer->m_nDeposit;
						dne.nMinDeposit = nMinDeposit;
						
						response.head.nRequest = GR_RESPONE_DEPOSIT_NOTENOUGH;
						response.nDataLen = sizeof(dne);
						response.pDataPtr = &dne;
						return SendUserResponse(lpContext,&response);
					}
					
					//检查银两是否超出
					if (IsCheckDepositMax(roomid))
					{
						int nMaxDeposit=GetMaxDeposit(roomid); //银子超出						
						if (nMaxDeposit < pPlayer->m_nDeposit)
						{
							DEPOSIT_TOO_HIGH dth;
							memset(&dth, 0, sizeof(dth));
							dth.nUserID = userid;
							dth.nChairNO = chairno;
							dth.nDeposit = pPlayer->m_nDeposit;
							dth.nMaxDeposit = nMaxDeposit;
							
							response.head.nRequest = GR_RESPONE_DEPOSIT_TOOHIGH;
							response.nDataLen = sizeof(dth);
							response.pDataPtr = &dth;
							return SendUserResponse(lpContext,&response,bPassive);
						}
					}
				}
			}
			
			if (bRandomRoom
				&& (pTable->IsFirstBout()
				||(IsLeaveAlone(roomid)
				&&IsNeedWaitArrageTable(pTable,roomid,userid))))//已经达到桌局数上限，那么重新向RoomSvr请求分桌
			{
				pTable->m_dwUserStatus[chairno]|=US_USER_WAITNEWTABLE; 
				PostAskNewTable(userid,roomid,tableno,chairno);
				
				pPlayer=pTable->m_ptrPlayers[chairno];
				if (pPlayer)
				{
					NotifyOneUser(pPlayer->m_hSocket,pPlayer->m_lTokenID,GR_WAIT_NEWTABLE,NULL,0);
				}
				
				response.head.nRequest = UR_OPERATE_SUCCEEDED;
				return SendUserResponse(lpContext, &response,bPassive);
			}
		}

		if (IS_BIT_SET(GetGameOption(roomid),GO_NOT_VERIFYSTART))
		{
			OnUserStart(pTable,chairno);
		}
		else
		{
			PostVerifyStart(userid,roomid,tableno,chairno);
		}
	}

	response.head.nRequest = UR_OPERATE_SUCCEEDED;
    return SendUserResponse(lpContext, &response,bPassive);
}

void CGameServer::InitRecord()
{
    m_dwTimeLastSave = GetTickCount();
    m_dwTodayPlayerWinCount = 0;   //今日收集银子流通 -- 日流通
    m_dwTodayPlayerCount = 0;	   //今日收集的总人数 -- 日活跃
    m_dwTodayTotalFee = 0;	       //今日收集的总茶水费
    m_dwTodayTotalBount = 0;      //今日收集的总局数
    m_strRecordFile = GetRecordFile();
    m_dwTodayPlayerDepositCount = 0;//所有玩家身上所有的银子
    {
        CAutoLock lock(&m_cslistPlayerRecord);
        PLAYERRECORDLIST::iterator it = m_listPlayerRecord.begin();
        PLAYERRECORDLIST::iterator itEnd = m_listPlayerRecord.end();
        while (it != itEnd)
        {
            m_dwTodayPlayerDepositCount += it->dwPlayerDeposit;
            it++;
        }
    }


    {
        CAutoLock lock(&m_cslistPlayerRecord);
        m_listPlayerRecord.clear();
    }
    
    {
        CAutoLock lock(&m_csmapRoomRecord);
        m_mapRoomRecord.clear();
    }
}

CString CGameServer::GetRecordFile()
{
    CTime NowTime = CTime::GetCurrentTime();
    TCHAR szFilePath[MAX_PATH];
    GetModuleFileName(NULL,szFilePath,MAX_PATH);
    *strrchr(szFilePath,'\\')=0;
    CString strRecordFile;
    strRecordFile.Format(_T("%s\\Record%04d%02d%02d.log"), szFilePath, NowTime.GetYear(), NowTime.GetMonth(), NowTime.GetDay());
    return strRecordFile;
}

//读取日志文件
void CGameServer::ReadRecordDataFromFile()
{
	try
	{
		InitRecord();
		CFileFind fFind;								//文件查找
		if (!fFind.FindFile(m_strRecordFile))
		{//文件不存在，则不用读取
			return;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//读取文件
		CStdioFile file;								//文件操作
		file.Open(m_strRecordFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::shareDenyNone);

		const int ncRow = 20;							//每个数据所占位
		CString strTempTTTTTTTTTT;						//临时数据
		CString strText;								//读取当前行的文本
		
		while(file.ReadString(strText))
		{
			//////////////////////////////////////////////////////////
			//个人信息
			int nPos = strText.Find(" ");
			if (nPos>=0)
			{
				PLAYER_RECORD_ST stPlayerRecord;
				memset(&stPlayerRecord, 0, sizeof(stPlayerRecord));

				strTempTTTTTTTTTT = strText.Left(nPos);
				if (strTempTTTTTTTTTT == "PlayerID") 
				{
					continue;
				}
				stPlayerRecord.nPlayerID = atoi(strTempTTTTTTTTTT);
				strText.Delete(0,strTempTTTTTTTTTT.GetLength()+m_strSpace.GetLength());

				nPos = strText.Find(" ");
				if (nPos>=0)
				{
					strTempTTTTTTTTTT = strText.Left(nPos);
					stPlayerRecord.dwPlayerWin = atoi(strTempTTTTTTTTTT);
					strText.Delete(0,strTempTTTTTTTTTT.GetLength()+m_strSpace.GetLength());

					nPos = strText.Find(" ");
					if (nPos>=0)
					{
						strTempTTTTTTTTTT = strText.Left(nPos);
						stPlayerRecord.dwPlayerBout = atoi(strTempTTTTTTTTTT);
						strText.Delete(0,strTempTTTTTTTTTT.GetLength()+m_strSpace.GetLength());

						nPos = strText.Find(" ");
						if (nPos>=0)
						{
							strTempTTTTTTTTTT = strText.Left(nPos);
							stPlayerRecord.dwPlayerFee = atoi(strTempTTTTTTTTTT);
							strText.Delete(0,strTempTTTTTTTTTT.GetLength()+m_strSpace.GetLength());

							nPos = strText.Find("\r");
							if (nPos<0)
							{
								strText.Find("\n");
							}
							if (nPos>=0)
							{
								strTempTTTTTTTTTT = strText.Left(nPos);
                                stPlayerRecord.dwPlayerDeposit = atoi(strTempTTTTTTTTTT);
								m_listPlayerRecord.push_back(stPlayerRecord);
								continue;
							}
						}
					}
				}
			}

			/////////////////////////////////////////////////////////
			//统计信息
			strTempTTTTTTTTTT = strText.Left(strText.Find("="));
            if (strTempTTTTTTTTTT == "TodayPlayerCount")
            {
                //今日玩家数
                strText.Delete(0,strlen("TodayPlayerCount="));
                int nPos = strText.Find(" ");
                if (nPos<0)
                {
                    nPos = strText.Find("\r");
                }
                if (nPos<0)
                {
                    nPos = strText.Find("\n");
                }
                if (nPos<0)
                {
                    continue;
                }
                m_dwTodayPlayerCount = atoi(strText.Left(nPos));
                continue;
			}
            else if (strTempTTTTTTTTTT == "TodayTotalBount")
            {//对局数
                strText.Delete(0,strlen("TodayTotalBount="));
                int nPos = strText.Find(" ");
                if (nPos<0)
                {
                    nPos = strText.Find("\r");
                }
                if (nPos<0)
                {
                    nPos = strText.Find("\n");
                }
                if (nPos<0)
                {
                    continue;
                }
                m_dwTodayTotalBount = atoi(strText.Left(nPos));
                continue;
			}
            else if (strTempTTTTTTTTTT == "TodayPlayerWinCount")
            {
                //流通量
                strText.Delete(0,strlen("TodayPlayerWinCount="));
                int nPos = strText.Find(" ");
                if (nPos<0)
                {
                    nPos = strText.Find("\r");
                }
                if (nPos<0)
                {
                    nPos = strText.Find("\n");
                }
                if (nPos<0)
                {
                    continue;
                }
                m_dwTodayPlayerWinCount = atoi(strText.Left(nPos));
				continue;
            }
			else if (strTempTTTTTTTTTT == "TodayTotalFee")
			{//茶水费未记录时，读取茶水费
				strText.Delete(0,strlen("TodayTotalFee="));
				int nPos = strText.Find(" ");
				if (nPos<0)
				{
					nPos = strText.Find("\r");
				}
				if (nPos<0)
				{
					nPos = strText.Find("\n");
				}
				if (nPos<0)
				{
					continue;
				}
				m_dwTodayTotalFee = atoi(strText.Left(nPos));
				continue;
			}
            else if (strTempTTTTTTTTTT == "TodayPlayerDepositCount")
            {//茶水费未记录时，读取茶水费
                strText.Delete(0,strlen("TodayPlayerDepositCount="));
                int nPos = strText.Find(" ");
                if (nPos<0)
                {
                    nPos = strText.Find("\r");
                }
                if (nPos<0)
                {
                    nPos = strText.Find("\n");
                }
                if (nPos<0)
                {
                    continue;
                }
                m_dwTodayPlayerDepositCount = atoi(strText.Left(nPos));
                continue;
			}
			else
			{//
				strTempTTTTTTTTTT = strText.Left(strText.Find("["));
				if (strTempTTTTTTTTTT == "RoomFee")
				{
					ROOM_RECORD_ST stRoomRecord;
					memset(&stRoomRecord, 0, sizeof(stRoomRecord));
					strText.Delete(0,strlen("RoomFee["));
					int nPos = strText.Find("]");
					if (nPos<0) 
					{
						continue;
					}
					stRoomRecord.nRoomID = atoi(strText.Left(nPos));
					strText.Delete(0,strlen(strText.Left(nPos))+strlen("]="));

					nPos = strText.Find("\r");
					if (nPos<0)
					{
						nPos = strText.Find("\n");
					}
					if (nPos<0)
					{
						continue;
					}
					stRoomRecord.dwRoomFee = atoi(strText.Left(nPos));
					ROOMRECORDMAP::iterator it = m_mapRoomRecord.find(stRoomRecord.nRoomID);
					if (it != m_mapRoomRecord.end())
					{
						it->second.dwRoomFee = stRoomRecord.dwRoomFee;
					}
					else
					{
						m_mapRoomRecord.insert(std::pair<int, ROOM_RECORD_ST>(stRoomRecord.nRoomID, stRoomRecord));
					}
					continue;
				}
			}
		}

		file.Close();
		if (m_listPlayerRecord.size()<=0)
		{
			InitRecord();
		}
	}
	catch (CException* e)
	{
		e->Delete();
	}
}

void CGameServer::RecordDataOne(CGameTable* pTable, int chairno, int nFees, int nDepositDiff) 
{
	if (chairno<0 && chairno>=TOTAL_CHAIRS)
	{
		return;
	}

	DWORD dwThisFee = 0; //本局茶水费
	{
		CAutoLock lock(&m_cslistPlayerRecord);
	
		if (!pTable->m_ptrPlayers[chairno]) return ;//为空
		if(pTable->m_ptrPlayers[chairno]->m_bIdlePlayer) return;//空闲玩家
        PLAYERRECORDLIST::iterator it = m_listPlayerRecord.begin();
        PLAYERRECORDLIST::iterator itEnd = m_listPlayerRecord.end();
        while (it != itEnd)
        {
            if (pTable->m_ptrPlayers[chairno]->m_nUserID == it->nPlayerID)
            {
                break;
            }
            it++;
        }

		if (it != itEnd)
		{
			
			if (nDepositDiff>0)
			{
				it->dwPlayerWin += (nDepositDiff+nFees);
                m_dwTodayPlayerWinCount += (nDepositDiff+nFees);
			}
			
			it->dwPlayerBout++;
			it->dwPlayerFee+=nFees;
            it->dwPlayerDeposit = pTable->m_ptrPlayers[chairno]->m_nDeposit;
		}
		else
		{//新玩家
            PLAYER_RECORD_ST stPlayerRecord = {0};
			stPlayerRecord.nPlayerID = pTable->m_ptrPlayers[chairno]->m_nUserID;
			if (nDepositDiff>0)
			{
                stPlayerRecord.dwPlayerWin += (nDepositDiff+nFees);
                m_dwTodayPlayerWinCount += (nDepositDiff+nFees);
			}
			else
			{
				stPlayerRecord.dwPlayerWin = 0;
			}
			
			stPlayerRecord.dwPlayerBout = 1;
			stPlayerRecord.dwPlayerFee = nFees;
            stPlayerRecord.dwPlayerDeposit = pTable->m_ptrPlayers[chairno]->m_nDeposit;
			m_listPlayerRecord.push_back(stPlayerRecord);
			
			m_dwTodayPlayerCount++;
		}
		dwThisFee += nFees;
		m_dwTodayTotalFee += nFees;
	}

		
	{//房间信息填充
        CAutoLock lock(&m_csmapRoomRecord); 
		ROOMRECORDMAP::iterator it = m_mapRoomRecord.find(pTable->m_nRoomID);
		if (it != m_mapRoomRecord.end())
		{
			 it->second.dwRoomFee+=dwThisFee;
		}
		else
		{
			ROOM_RECORD_ST stRoom;
			stRoom.nRoomID = pTable->m_nRoomID;
			stRoom.dwRoomFee = dwThisFee;
			
			m_mapRoomRecord.insert(std::pair<int,ROOM_RECORD_ST>(stRoom.nRoomID, stRoom));
		}
	}

	SaveRecordDataToFile();
}


//写日志文件
void CGameServer::RecordData( CGameTable* pTable, int chairno, int nFees[], int nDepositDiff[] )
{
    DWORD dwThisFee = 0; //本局茶水费
    {
        CAutoLock lock(&m_cslistPlayerRecord);
        for (int i=0;i<pTable->m_nTotalChairs;i++)
        {
            if (!pTable->m_ptrPlayers[i])continue;//为空
            if(pTable->m_ptrPlayers[i]->m_bIdlePlayer) continue;//空闲玩家
            
            PLAYERRECORDLIST::iterator it = m_listPlayerRecord.begin();
            PLAYERRECORDLIST::iterator itEnd = m_listPlayerRecord.end();
            while (it != itEnd)
            {
                if (pTable->m_ptrPlayers[i]->m_nUserID == it->nPlayerID)
                {
                    break;
                }
                it++;
            }
            
            
            if (it != m_listPlayerRecord.end())
            {
                if (nDepositDiff[i]>0)
                {
                    it->dwPlayerWin += nDepositDiff[i]+nFees[i];
                    m_dwTodayPlayerWinCount += nDepositDiff[i]+nFees[i];
                }
                
                it->dwPlayerBout++;
                it->dwPlayerFee+=nFees[i];
                it->dwPlayerDeposit = pTable->m_ptrPlayers[i]->m_nDeposit;
            }
            else
            {//新玩家
                PLAYER_RECORD_ST stPlayerRecord;
                stPlayerRecord.nPlayerID = pTable->m_ptrPlayers[i]->m_nUserID;
                if (nDepositDiff[i]>0)
                {
                    stPlayerRecord.dwPlayerWin = nDepositDiff[i]+nFees[i];
                    m_dwTodayPlayerWinCount += nDepositDiff[i]+nFees[i];
                }
                else
                {
                    stPlayerRecord.dwPlayerWin = 0;
                }
                             
                
                stPlayerRecord.dwPlayerBout = 1;
                stPlayerRecord.dwPlayerFee = nFees[i];
                stPlayerRecord.dwPlayerDeposit = pTable->m_ptrPlayers[i]->m_nDeposit;
                m_listPlayerRecord.push_back(stPlayerRecord);
                
                m_dwTodayPlayerCount++;
            }
            dwThisFee += nFees[i];
            m_dwTodayTotalFee += nFees[i];
        }
        m_dwTodayTotalBount++;
	}

    {//房间信息填充
        CAutoLock lock(&m_csmapRoomRecord);
        ROOMRECORDMAP::iterator it = m_mapRoomRecord.find(pTable->m_nRoomID);
        if (it != m_mapRoomRecord.end())
        {
            it->second.dwRoomFee+=dwThisFee;
        }
        else
        {
            ROOM_RECORD_ST stRoom;
            stRoom.nRoomID = pTable->m_nRoomID;
            stRoom.dwRoomFee = dwThisFee;        
            m_mapRoomRecord.insert(std::pair<int,ROOM_RECORD_ST>(stRoom.nRoomID, stRoom));
        }
    }
    
	SaveRecordDataToFile();
}

//保存日志文件
void CGameServer::SaveRecordDataToFile( BOOL bImmediate /*= FALSE*/ )
{
try
	{	
		if (!bImmediate)//马上写
		{
			if (GetTickCount() - m_dwTimeLastSave < 180000 && GetTickCount() > m_dwTimeLastSave) 
			{
				return;
			}
		}
		m_dwTimeLastSave = GetTickCount();

		CString strRecordFile = GetRecordFile();
		if (m_strRecordFile!=strRecordFile)
		{//保存的文件与原来不符合，则新创建文件，清空数据
			InitRecord();
		}
		
		/////////////////////////////////////////////////////////////////////////////////////
		CStdioFile file;												//文件操作
		file.Open(strRecordFile, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);

		const int ncRow = 20;										//每个数据所占位
		
		CString strWriteWWWWWWWWWW;									//待写入的数据
		CString strTempTTTTTTTTTT;									//临时数据

		//////////////////////写入项目名
		strWriteWWWWWWWWWW.Format("PlayerID");
		strWriteWWWWWWWWWW += m_strSpace;
		strTempTTTTTTTTTT.Format("PlayerWin");
		strWriteWWWWWWWWWW += strTempTTTTTTTTTT;
		strWriteWWWWWWWWWW += m_strSpace;
		strTempTTTTTTTTTT.Format("PlayerBout");
		strWriteWWWWWWWWWW += strTempTTTTTTTTTT;
		strWriteWWWWWWWWWW += m_strSpace;
		strTempTTTTTTTTTT.Format("PlayerFee");
		strWriteWWWWWWWWWW += strTempTTTTTTTTTT;
		strWriteWWWWWWWWWW += m_strSpace;
		strTempTTTTTTTTTT.Format("PlayerDeposit\r\n");
		strWriteWWWWWWWWWW += strTempTTTTTTTTTT;
		file.WriteString(strWriteWWWWWWWWWW);

		//////////////////////写入数据
		{
			CAutoLock lock(&m_cslistPlayerRecord);
			PLAYERRECORDLIST::iterator it = m_listPlayerRecord.begin();
			while (it != m_listPlayerRecord.end())
			{
				strTempTTTTTTTTTT.Format("%u",it->nPlayerID);
				strWriteWWWWWWWWWW = strTempTTTTTTTTTT;		//加入玩家ID
				strWriteWWWWWWWWWW += m_strSpace;
				
				//////////////////////////////////////////
				strTempTTTTTTTTTT.Format("%u",it->dwPlayerWin);
				strWriteWWWWWWWWWW += strTempTTTTTTTTTT;	//加入日流通
				strWriteWWWWWWWWWW += m_strSpace;

				//////////////////////////////////////////
				strTempTTTTTTTTTT.Format("%u",it->dwPlayerBout);
				strWriteWWWWWWWWWW += strTempTTTTTTTTTT;	//加入今天玩家玩的局数
				strWriteWWWWWWWWWW += m_strSpace;
				
				//////////////////////////////////////////
				strTempTTTTTTTTTT.Format("%u",it->dwPlayerFee);
				strWriteWWWWWWWWWW += strTempTTTTTTTTTT;	//加入今天玩家茶水费
				strWriteWWWWWWWWWW += m_strSpace;

				///////////////////////////////////////////
				strTempTTTTTTTTTT.Format("%u\r\n",it->dwPlayerDeposit);
				strWriteWWWWWWWWWW += strTempTTTTTTTTTT;	//加入今天玩家奖励额
				
				file.WriteString(strWriteWWWWWWWWWW);
				
				it++;
			}
		}

		//////////////////////写入总结
		strWriteWWWWWWWWWW.Format("\r\n[Summary]\r\n");

		strTempTTTTTTTTTT.Format("TodayPlayerCount=%u\r\n",m_dwTodayPlayerCount);
		strWriteWWWWWWWWWW += strTempTTTTTTTTTT;

        strTempTTTTTTTTTT.Format("TodayTotalBount=%u\r\n",m_dwTodayTotalBount);
        strWriteWWWWWWWWWW += strTempTTTTTTTTTT;

        strTempTTTTTTTTTT.Format("TodayPlayerWinCount=%u\r\n",m_dwTodayPlayerWinCount);
		strWriteWWWWWWWWWW += strTempTTTTTTTTTT;

		strTempTTTTTTTTTT.Format("TodayTotalFee=%u\r\n",m_dwTodayTotalFee);
		strWriteWWWWWWWWWW += strTempTTTTTTTTTT;

		strTempTTTTTTTTTT.Format("TodayPlayerDepositCount=%d\r\n",m_dwTodayPlayerDepositCount);
		strWriteWWWWWWWWWW += strTempTTTTTTTTTT;

		file.WriteString(strWriteWWWWWWWWWW);

		//////////////////////写入房间信息总结
        {
            CAutoLock lock(&m_csmapRoomRecord);
            ROOMRECORDMAP::iterator it2 = m_mapRoomRecord.begin();
            while (it2 != m_mapRoomRecord.end())
            {
                strWriteWWWWWWWWWW.Format("RoomFee[%d]=%u\r\n", it2->second.nRoomID, it2->second.dwRoomFee);
                file.WriteString(strWriteWWWWWWWWWW);
                it2++;
            }
        }	
		file.Close();
	}
	catch (CException* e)
	{
		e->Delete();
	}    
}

BOOL CGameServer::OnEnterGameDXXW(LPCONTEXT_HEAD lpContext, LPENTER_GAME_EX pEnterGame)
{
	m_nOpen = GetPrivateProfileInt(
		_T("HideGiveUpBtn"),	// section name
		_T("open"),				// key name
		0,						// default int
		m_szIniFile				// initialization file name
									);
	NotifyOneUser(lpContext->hSocket,lpContext->lTokenID, GR_HIDE_GIVE_UP_BUTTON, &m_nOpen, sizeof(int));
	return TRUE;
}

BOOL CGameServer::OnGameStarted(CTable* pTable, DWORD dwFlags)
{
	for(int i = 0; i < pTable->m_nTotalChairs; i++){
		CPlayer* ptrP = pTable->m_ptrPlayers[i];
		if(!ptrP) continue;
		int roomid = pTable->m_nRoomID;
		int tableno = pTable->m_nTableNO;
		int userid = ptrP->m_nUserID;
		int chairno = ptrP->m_nChairNO;
		
		if(!VerifyRoomTableChair(roomid, tableno, chairno, userid)){ // 该位置上用户不匹配
			return FALSE;
		}
	}
	
	if (!CheckBeforeGameStart(pTable))
		return FALSE;

	
	if (IS_BIT_SET(GetRoomManage(pTable->m_nRoomID),RM_MATCHONGAME))
	{
		if (!CheckOpenTime(pTable))
			return FALSE;
	}
	
	TCHAR szRoomID[16];
	memset(szRoomID, 0, sizeof(szRoomID));
	_stprintf(szRoomID, _T("%ld"), pTable->m_nRoomID);

	/*
	int deposit_mult = GetPrivateProfileInt(
							_T("deposit"),			// section name
							_T("multiple"),			// key name
							1,						// default int
							m_szIniFile				// initialization file name
							);
	*/
	int deposit_mult = GetPrivateProfileInt(				
							_T("multisilver"),		// section name
							szRoomID,				// key name
							1,						// default int
							m_szIniFile				// initialization file name
							);
	int fee_ratio = GetPrivateProfileInt(			// 胜方每人收取n%的手续费
							_T("fee"),				// section name
							_T("ratio"),			// key name
							1,						// default int
							m_szIniFile				// initialization file name
							);
	int max_trans = GetPrivateProfileInt(			// 
							_T("deposit"),			// section name
							_T("maxtrans"),			// key name
							0,						// default int
							m_szIniFile				// initialization file name
							);
	int cut_ratio = GetPrivateProfileInt(			// 胜方每人收取n%的手续费
							_T("cut"),				// section name
							_T("ratio"),			// key name
							100,					// default int
							m_szIniFile				// initialization file name
							);
	int deposit_logdb = GetPrivateProfileInt(		// 
							_T("deposit"),			// section name
							_T("logdb"),			// key name
							0,						// default int
							m_szIniFile				// initialization file name
							);			
	int fee_mode = GetPrivateProfileInt(
							_T("feemode"),			// section name
							szRoomID,				// key name
							FEE_MODE_TEA,			// default int
							m_szIniFile				// initialization file name
							);
	int fee_value = GetPrivateProfileInt(
							_T("feevalue"),			// section name
							szRoomID,				// key name
							1,						// default int
							m_szIniFile				// initialization file name
							);

	int fee_tenthousandth =	GetPrivateProfileInt(
		_T("feeTTratio"),	// section name
		szRoomID,			// key name
		10,					// default int
		m_szIniFile			// initialization file name
		);
	int fee_minimum	=	GetPrivateProfileInt(
		_T("feeminimum"),	// section name
		szRoomID,			// key name
		1000,				// default int
		m_szIniFile			// initialization file name
		);

// 	int deposit_min = GetPrivateProfileInt(
// 		_T("deposit"),			// section name
// 		_T("min"),				// key name
// 		MIN_DEPOSIT_NEED,		// default int
// 		m_szIniFile				// initialization file name
// 							);

	////玩游戏所需的最小银两下限, 修改为接口模式，可以重载
	int deposit_min = GetMinPlayingDeposit(pTable, pTable->m_nRoomID);	

	//Modify on 20130225
	int nBaseSliverDefault = 1;
	if (FEE_MODE_SERVICE_FIXED == fee_mode	//服务费模式下，有没有配置基础银，决定了是使用配置的基础银，还是计算出的基础银
		|| FEE_MODE_SERVICE_MINDEPOSIT == fee_mode
		|| FEE_MODE_SERVICE_SELFDEPOSIT == fee_mode)
		nBaseSliverDefault = 0;
	
	int base_silver = GetPrivateProfileInt(
							_T("basesilver"),	// section name
							szRoomID,			// key name
							nBaseSliverDefault,	// default int
							m_szIniFile			// initialization file name
							);
	int base_score = GetPrivateProfileInt(
							_T("basescore"),	// section name
							szRoomID,			// key name
							0,					// default int
							m_szIniFile			// initialization file name
							);
	int max_bouttime = GetBoutTimeMax();

	int max_user_bout	=	GetPrivateProfileInt(
							_T("maxuserbout"),	// section name
							szRoomID,			// key name
							MAX_USER_BOUT,		// default int
							m_szIniFile			// initialization file name
							);
	
	int max_table_bout	=	GetPrivateProfileInt(
							_T("maxtablebout"),	// section name
							szRoomID,			// key name
							MAX_TABLE_BOUT,		// default int
							m_szIniFile			// initialization file name
							);

	int score_min = GetMinPlayScore(pTable->m_nRoomID);
	int score_max = GetMaxPlayScore(pTable->m_nRoomID);

	//可变玩家人数游戏模式
	int min_player_count = GetMinPlayerCount(pTable,pTable->m_nRoomID);

	int errchair = INVALID_OBJECT_ID;
	int error = pTable->Restart(errchair, deposit_mult, deposit_min, 
								fee_ratio, max_trans, cut_ratio, deposit_logdb,
								fee_mode, fee_value, base_silver, max_bouttime, 
								base_score, score_min, score_max,
								max_user_bout,max_table_bout,min_player_count,
								fee_tenthousandth,fee_minimum);
	if(error < 0){
		if(TE_DEPOSIT_NOT_ENOUGH == error){ // 有人银子不够
			DEPOSIT_NOT_ENOUGH depositNotEnough;
			ZeroMemory(&depositNotEnough, sizeof(depositNotEnough));
			
			depositNotEnough.nUserID = pTable->m_ptrPlayers[errchair]->m_nUserID;
			depositNotEnough.nChairNO = errchair;
			depositNotEnough.nDeposit = pTable->m_ptrPlayers[errchair]->m_nDeposit;
			depositNotEnough.nMinDeposit = pTable->m_nDepositMin;			
			
			NotifyTablePlayers(pTable, GR_DEPOSIT_NOT_ENOUGH, &depositNotEnough, sizeof(depositNotEnough));
			NotifyTableVisitors(pTable, GR_DEPOSIT_NOT_ENOUGH, &depositNotEnough, sizeof(depositNotEnough));
		}else if(TE_PLAYER_NOT_SEATED == error){ // 有人离开游戏			
			PLAYER_NOT_SEATED playNotSeated;
			ZeroMemory(&playNotSeated, sizeof(playNotSeated));

			//playNotSeated.nUserID = pTable->m_ptrPlayers[errchair]->m_nUserID; // NULL == pTable->m_ptrPlayers[errchair]
			playNotSeated.nChairNO = errchair;

			NotifyTablePlayers(pTable, GR_PLAYER_NOT_SEATED, &playNotSeated, sizeof(playNotSeated));
			NotifyTableVisitors(pTable, GR_PLAYER_NOT_SEATED, &playNotSeated, sizeof(playNotSeated));
		}else if(TE_SCORE_NOT_ENOUGH == error){ // 有人积分不够
			SCORE_NOT_ENOUGH scoreNotEnough;
			ZeroMemory(&scoreNotEnough, sizeof(scoreNotEnough));
			
			scoreNotEnough.nUserID = pTable->m_ptrPlayers[errchair]->m_nUserID;
			scoreNotEnough.nChairNO = errchair;
			scoreNotEnough.nScore = pTable->m_ptrPlayers[errchair]->m_nScore;
			scoreNotEnough.nMinScore = pTable->m_nScoreMin;			
			
			NotifyTablePlayers(pTable, GR_SCORE_NOT_ENOUGH, &scoreNotEnough, sizeof(scoreNotEnough));
			NotifyTableVisitors(pTable, GR_SCORE_NOT_ENOUGH, &scoreNotEnough, sizeof(scoreNotEnough));	
		}else if(TE_SCORE_TOO_HIGH == error){ // 有人积分超出
			SCORE_TOO_HIGH scoreTooHigh;
			ZeroMemory(&scoreTooHigh, sizeof(scoreTooHigh));
			
			scoreTooHigh.nUserID = pTable->m_ptrPlayers[errchair]->m_nUserID;
			scoreTooHigh.nChairNO = errchair;
			scoreTooHigh.nScore = pTable->m_ptrPlayers[errchair]->m_nScore;
			scoreTooHigh.nMaxScore = pTable->m_nScoreMax;			
			
			NotifyTablePlayers(pTable, GR_SCORE_TOO_HIGH, &scoreTooHigh, sizeof(scoreTooHigh));
			NotifyTableVisitors(pTable, GR_SCORE_TOO_HIGH, &scoreTooHigh, sizeof(scoreTooHigh));			
		}else if(TE_USER_BOUT_TOO_HIGH == error){ // 有玩家游戏局数超过上限
			USER_BOUT_TOO_HIGH userBoutTooHigh;
			ZeroMemory(&userBoutTooHigh, sizeof(userBoutTooHigh));
			
			userBoutTooHigh.nUserID = pTable->m_ptrPlayers[errchair]->m_nUserID;
			userBoutTooHigh.nChairNO = errchair;
			userBoutTooHigh.nBout = pTable->m_ptrPlayers[errchair]->m_nBout;
			userBoutTooHigh.nMaxBout = max_user_bout;		
			
			NotifyTablePlayers(pTable, GR_USER_BOUT_TOO_HIGH, &userBoutTooHigh, sizeof(userBoutTooHigh));
			NotifyTableVisitors(pTable, GR_USER_BOUT_TOO_HIGH, &userBoutTooHigh, sizeof(userBoutTooHigh));	
		}else if(TE_TABLE_BOUT_TOO_HIGH == error){ // 本桌游戏局数超过上限
			TABLE_BOUT_TOO_HIGH tableBoutTooHigh;
			ZeroMemory(&tableBoutTooHigh, sizeof(tableBoutTooHigh));
			
			tableBoutTooHigh.nTableNO= pTable->m_nTableNO;
			tableBoutTooHigh.nBout = pTable->m_nBoutCount;
			tableBoutTooHigh.nMaxBout = max_table_bout;			
			
			NotifyTablePlayers(pTable, GR_TABLE_BOUT_TOO_HIGH, &tableBoutTooHigh, sizeof(tableBoutTooHigh));
			NotifyTableVisitors(pTable, GR_TABLE_BOUT_TOO_HIGH, &tableBoutTooHigh, sizeof(tableBoutTooHigh));			
		}else{
		}

		//Add on 20121210 by chenyang
		//防止另外几人重新被分桌，确保散桌
		for (int i=0;i<pTable->m_nTotalChairs;i++)
		{
			pTable->m_dwUserStatus[i]&=~US_GAME_STARTED;//去掉准备状态
		}
		//Add end

		return FALSE;
	}

	RecordUserGameBout(pTable);

	//Del by chenyang on 20130507
	//所有房间都发生游戏开始消息到roomsvr
//	if (IsSoloRoom(pTable->m_nRoomID))
	{
		if (IsVariableChairRoom(pTable->m_nRoomID))
		{
			int nChairStatus[MAX_CHAIRS_PER_TABLE];
			memset(&nChairStatus,0,sizeof(nChairStatus));
			for (int i=0; i<MAX_CHAIRS_PER_TABLE; i++)
			{	
				if (pTable->m_ptrPlayers[i]
					&& !(pTable->m_ptrPlayers[i]->m_bIdlePlayer))
					nChairStatus[i] = 1;
			}

			DWORD dwTableChairStatus = MakeTableChairStatus(pTable->m_nTableNO, nChairStatus, MAX_CHAIRS_PER_TABLE);
			
			PostGameStart(pTable->m_nRoomID,dwTableChairStatus);

			//踢掉连续多局不参与的玩家
			KickOffTooManyIdlePlayer(pTable, pTable->m_nRoomID);
		}
		else
			PostGameStart(pTable->m_nRoomID,pTable->m_nTableNO);
	}

	int nLen = 0;
	nLen = pTable->GetGameStartSize();
	void* pData = new BYTE[nLen];
	
	m_nOpen = GetPrivateProfileInt(
		_T("HideGiveUpBtn"),	// section name
		_T("open"),				// key name
		0,						// default int
		m_szIniFile				// initialization file name
									);

	for(i = 0; i < pTable->m_nTotalChairs; i++){
	
		pTable->FillupGameStart(pData, nLen, i, FALSE);
		CPlayer* ptrP = pTable->m_ptrPlayers[i];
		if(ptrP){
			NotifyOneUser(ptrP->m_hSocket, ptrP->m_lTokenID, GR_GAME_START, pData, nLen, TRUE);
			NotifyOneUser(ptrP->m_hSocket, ptrP->m_lTokenID, GR_HIDE_GIVE_UP_BUTTON, &m_nOpen, sizeof(int));
		}
	}
	for(i = 0; i < pTable->m_nTotalChairs; i++){
		pTable->FillupGameStart(pData, nLen, i, TRUE);	
		int userid = 0;
		CPlayer* ptrV = NULL;
		POSITION pos = pTable->m_mapVisitors[i].GetStartPosition();
		while (pos)	{
			pTable->m_mapVisitors[i].GetNextAssoc(pos, userid, ptrV);
			if(ptrV){
				NotifyOneUser(ptrV->m_hSocket, ptrV->m_lTokenID, GR_GAME_START, pData, nLen, TRUE);
			}
		}
	}
	SAFE_DELETE(pData);

	//服务费的通知
	NotifyServiceFee(pTable);
	/*{
		int nNotifyFee = GetPrivateProfileInt(
			_T("feenotify"),	// section name
			szRoomID,			// key name
			1,					// default int
			m_szIniFile			// initialization file name
			);
		if (nNotifyFee &&
			(FEE_MODE_SERVICE_FIXED == pTable->m_nFeeMode
			|| FEE_MODE_SERVICE_MINDEPOSIT == pTable->m_nFeeMode
			|| FEE_MODE_SERVICE_SELFDEPOSIT == pTable->m_nFeeMode))
		{
			GAME_WIN gameWin;
			memset(&gameWin, 0, sizeof(GAME_WIN));
			pTable->FillupOldScoreDeposit(&gameWin, sizeof(GAME_WIN));
			int	nOldDeposits[MAX_CHAIRS_PER_TABLE];						// 旧银子
			memset(nOldDeposits, 0, sizeof(nOldDeposits));
			pTable->CalcWinFeesEx(gameWin.nOldDeposits, nOldDeposits, gameWin.nDepositDiffs, gameWin.nWinFees);

			CHAT_FROM_TABLE cft;
			memset(&cft,0,sizeof(cft));
			cft.nRoomID = pTable->m_nRoomID;
			for(i = 0; i < pTable->m_nTotalChairs; i++)
			{
				CPlayer* ptrP = pTable->m_ptrPlayers[i];
				if(ptrP && !ptrP->m_bIdlePlayer && gameWin.nWinFees[i]>0)
				{
					cft.nUserID = ptrP->m_nUserID;
					sprintf(cft.szChatMsg,_T(" 本局游戏将收取%d两服务费。\r\n"), gameWin.nWinFees[i]);
					cft.nMsgLen = lstrlen(cft.szChatMsg)+1;  
					cft.dwFlags	= FLAG_CHAT_SYSNOTIFY;
					int nSendLen=sizeof(CHAT_FROM_TABLE)-sizeof(cft.szChatMsg)+cft.nMsgLen;
 					NotifyOneUser(ptrP->m_hSocket, ptrP->m_lTokenID,GR_CHAT_FROM_TALBE,&cft,nSendLen);
				}
			}
		}
	}*/

	return TRUE;
}
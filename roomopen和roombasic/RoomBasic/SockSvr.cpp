#include "roombasic.h"
#include <Iphlpapi.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
  
void* CBaseSockServer::OnWorkerStart()
{
	_ConnectionPtr pDBConnect = NULL;
	HRESULT hr = 0;

	hr = ::CoInitialize(NULL);
	if(FAILED(hr)) return NULL;

 
	CWorkerContext* pThreadCxt = new CWorkerContext();
	pThreadCxt->pDBConnect = pDBConnect;
	return pThreadCxt;
}

void CBaseSockServer::OnWorkerExit(void* pContext)
{
	CWorkerContext* pThreadCxt = (CWorkerContext*)(pContext);
	if(pThreadCxt){
		/*
		_ConnectionPtr pDBConnect = pThreadCxt->pDBConnect;
		if(pDBConnect){
			pDBConnect->Close();
			pDBConnect = NULL;
		}
		*/
		SAFE_DELETE(pThreadCxt);
	}
	::CoUninitialize();
}

BOOL CBaseSockServer::OnRequest(void* lpParam1, void* lpParam2)
{try{
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
	case MR_NEW_PRIVATEROOM:
		MB_OnNewPrivateRoom(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_TRYGOTO_OTHERROOM:
		MB_OnTryGotoOtherPrivateRoom(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_SET_GAMEISACTIVED:
		MB_OnSetGameIsActived(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_FOUND_NEW_GROUP_TABLEROOMS:
		MB_OnFoundPrivateRooms(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_ASK_DETAIL_TABLEROOMS:
		MB_OnAskDetailTableRooms(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_ASK_ENTER_PRIVATEROOM:
		MB_OnAskEnterPrivateRoom(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_SET_OPENING_PRIVATEROOM:
		MB_OnSetOpeningPrivateRoom(lpContext, lpRequest, pThreadCxt);
		break;
	case MR_ASK_SYSTEM_FIND_PLAYERS:
		MB_OnAskSystemFindPlayers(lpContext, lpRequest, pThreadCxt);
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
}
catch(...)
{
	LPREQUEST		lpReq  = LPREQUEST(lpParam2);
 	UwlLogFile("catch error!!! reqid=%ld,DataLen=%ld",lpReq->head.nRequest,lpReq->nDataLen );
 
}
	return TRUE;
}

BOOL CBaseSockServer::OnUnsupported(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest,
							CWorkerContext* pThreadCxt)
{
	UwlTrace(_T("unsupported request from socket = %d"), lpContext->hSocket);

	return TRUE;
}

BOOL CBaseSockServer::TransmitRequestEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest,void* pData,int nDataLen)
{
 	LPREQUEST pRequest = new REQUEST;
	memcpy(pRequest, lpRequest, sizeof(REQUEST));

	if(nDataLen && pData)
	{
		pRequest->pDataPtr = new BYTE[nDataLen];
		memcpy(pRequest->pDataPtr, pData, nDataLen);
		pRequest->nDataLen = nDataLen;
	}
	else
	{
		pRequest->pDataPtr = NULL;
		pRequest->nDataLen = 0;
	}
	LPCONTEXT_HEAD pContext = new CONTEXT_HEAD;
	memcpy(pContext, lpContext, sizeof(CONTEXT_HEAD));
	
	if(!PostThreadMessage(g_uiThrdSend, UM_DATA_TOSEND, (WPARAM)pContext, (LPARAM)pRequest)){
		UwlClearRequest(pRequest);
		SAFE_DELETE(pRequest);
		SAFE_DELETE(pContext);
		return FALSE;
	}else{
		return TRUE;
	}
}

BOOL CBaseSockServer::TransmitRequest(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest)
{
	LPREQUEST pRequest = new REQUEST;
	memcpy(pRequest, lpRequest, sizeof(REQUEST));

	if(lpRequest->nDataLen && lpRequest->pDataPtr){
		pRequest->pDataPtr = new BYTE[lpRequest->nDataLen];
		memcpy(pRequest->pDataPtr, lpRequest->pDataPtr, lpRequest->nDataLen);
		pRequest->nDataLen = lpRequest->nDataLen;
	}else{
		pRequest->pDataPtr = NULL;
		pRequest->nDataLen = 0;
	}
	LPCONTEXT_HEAD pContext = new CONTEXT_HEAD;
	memcpy(pContext, lpContext, sizeof(CONTEXT_HEAD));
	
	if(!PostThreadMessage(g_uiThrdSend, UM_DATA_TOSEND, (WPARAM)pContext, (LPARAM)pRequest)){
		UwlClearRequest(pRequest);
		SAFE_DELETE(pRequest);
		SAFE_DELETE(pContext);
		return FALSE;
	}else{
		return TRUE;
	}
}

BOOL CBaseSockServer::PutToServer(LPCONTEXT_HEAD lpContext, UINT nRequest, void* pData, int nLen)
{
	LPREQUEST pRequest = new REQUEST;
	ZeroMemory(pRequest, sizeof(REQUEST));

	pRequest->head.nRequest = nRequest;
	pRequest->pDataPtr = pData;
	pRequest->nDataLen = nLen;

	LPCONTEXT_HEAD pContext = new CONTEXT_HEAD;
	ZeroMemory(pContext, sizeof(CONTEXT_HEAD));
	
	memcpy(pContext, lpContext, sizeof(CONTEXT_HEAD));

	return PutRequestToWorker(pRequest->nDataLen, DWORD(pContext->hSocket), 
						pContext, pRequest, pRequest->pDataPtr);
}
BOOL CBaseSockServer::VerifyUserHardID(int nUserID,LPCTSTR lpszHardID)
{
	return TRUE;
}



CString CBaseSockServer::GetConfigFileUnderFolder(LPCTSTR lpszFolder)
{
	CString sRet;
  	/////////////////////////////////////////////////////////////
  	TCHAR szTemp[MAX_PATH];
	memset(szTemp,0,sizeof(szTemp));
	lstrcpy(szTemp,lpszFolder);
 	lstrcat(szTemp,_T("*.ini"));	//在当前目录下查找*.ini
   
	WIN32_FIND_DATA          FindFileData;  
	HANDLE hFind=FindFirstFile(szTemp,&FindFileData);
	if(hFind==INVALID_HANDLE_VALUE)//目录文件失败
		return sRet;
	
 	BOOL  bDone=TRUE; 
  	while(bDone)
	{
 		if(IS_BIT_SET(FindFileData.dwFileAttributes , FILE_ATTRIBUTE_DIRECTORY)) 
		{
		 
 	
		}
		else
		{ 	
			sRet.Format(_T("%s%s"),lpszFolder,FindFileData.cFileName );
 		    FindClose(hFind);
			return sRet;
		}
		bDone=FindNextFile(hFind,&FindFileData);
		// 找下一个
	}
	FindClose(hFind);

	return sRet;
}
int CBaseSockServer::ConvertMemberLevelToDisplay(int nMemberLevel)
{
	return (nMemberLevel+1);
}

int CBaseSockServer::GetMsgTextColor(BOOL bMember)
{
	if (bMember)
		return RGB(0,0,255);
	else
		return RGB(0,0,0);
}

int CBaseSockServer::GetVipFaceByLevel(int nMemberLevel)
{
	int nFace = 101;
	switch (nMemberLevel)
	{
	case 0:
		nFace = 101;
		break;
	case 1:
		nFace = 102;
		break;
	case 2:
		nFace = 103;
		break;
	case 3:
		nFace = 104;
		break;
	case 4:
		nFace = 105;
		break;
		
	default:
		break;
	}
	
	return nFace;
}

int  CBaseSockServer::GetBusyWaitSecond()
{
	return  GetPrivateProfileInt(_T("listen"),_T("busywait"),120,	g_szIniFile		); //缺省120秒

} 

CString CBaseSockServer::GetRemainsBySeconds(int nSeconds)
{
	CString sRet,sTime;

 	int day=nSeconds/86400;
	int hour=(nSeconds-day*86400)/3600;
	int min=(nSeconds-day*86400-hour*3600)/60;
	nSeconds=nSeconds%60;
 
	if (day>0)
	{
		sTime.Format("%d天",day);
		sRet+=sTime;
	}
	
	if (hour>0||day>0)
	{
		sTime.Format("%d小时",hour);
		sRet+=sTime;
	}
	
	if (min>0||hour>0||day>0)
	{
		sTime.Format("%d分",min);
		sRet+=sTime;
	}
	
	sTime.Format("%d秒",nSeconds);
	sRet+=sTime;
 
	return  sRet;
}

BOOL CBaseSockServer::IsForbidChatUserID(int nUserID,int& nRemains)
{
  	CHAT_FORBID cf;
	memset(&cf,0,sizeof(cf));
	{
  		CAutoLock lock(&g_csChatForbid);
  		if(!g_mapChatForbidID.Lookup(nUserID,cf))
			return FALSE;
	}
	nRemains=cf.nExpiration-UwlGetCurrentSeconds();
    if(nRemains > 0 ) //还在禁用期内
 		return TRUE;
 	return FALSE;
}
BOOL CBaseSockServer::IsForbidChatMachine(LPCTSTR lpszMachineID,int& nRemains)
{
 	CString sMachineID;
	sMachineID.Format(_T("%s"),lpszMachineID);
 	CHAT_FORBID cf;
	memset(&cf,0,sizeof(CHAT_FORBID));
	{
  		CAutoLock lock(&g_csChatForbid);
  		if(!g_mapChatForbid.Lookup(sMachineID,cf))
			return FALSE;
	}
	nRemains=cf.nExpiration-UwlGetCurrentSeconds();
    if(nRemains > 0 ) //还在禁用期内
  		return TRUE;
 	return FALSE;
} 

BOOL CBaseSockServer::IsIncludeFilterWord(LPCTSTR lpszMsg)
{
	CAutoLock lock(&g_csFilterWord);

	TCHAR  szContent[MAX_ROOMCHAT_LEN];
	memset(szContent,0,sizeof(szContent));
	lstrcpyn(szContent,lpszMsg,MAX_ROOMCHAT_LEN-2);

	POSITION pos;
	int wordid=0;
	FILTER_WORD FilterWord;
	pos  = g_mapFilterWord.GetStartPosition();
	while (pos)	
	{
		g_mapFilterWord.GetNextAssoc(pos, wordid, FilterWord);
		char * p=xyStrstr(szContent,FilterWord.szWord);
		if(p)//找到匹配字符串
			return TRUE;
	}
	return FALSE;
}

 
BOOL  CBaseSockServer::IsProxyIP(DWORD dwIPAddr)
{
   	int nValue=0;
 	CAutoLock lock(&g_csProxyIP);
    g_mapProxyIP.Lookup(dwIPAddr,nValue );
	return nValue;
}
BOOL  CBaseSockServer::IsDeadGameSvr(int nGamePort,int nInterval)
{
   	GAMESVR_INFO gi;
	memset(&gi,0,sizeof(gi));
 	CAutoLock lock(&g_csGameSvrInfo);
    g_mapGameSvrInfo.Lookup(nGamePort,gi);
 
	if(UwlGetCurrentSeconds()-gi.nLatestTime > nInterval)
		return TRUE;
	return FALSE;
}

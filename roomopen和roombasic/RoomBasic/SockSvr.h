#pragma once

class CWorkerContext{
public:
	_ConnectionPtr	pDBConnect;
};

class CBaseSockServer : public CDefIocpServer{
public:
	CBaseSockServer(const BYTE key[] = 0, const ULONG key_len = 0, DWORD flagEncrypt = 0, DWORD flagCompress = 0)
		: CDefIocpServer(key, key_len, flagEncrypt, flagCompress){}
	CBaseSockServer(int nKeyType, DWORD flagEncrypt = 0, DWORD flagCompress = 0) 
		: CDefIocpServer(nKeyType, flagEncrypt, flagCompress){}
	virtual ~CBaseSockServer(){}

	virtual BOOL TransmitRequest(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest);

protected:
	virtual void* OnWorkerStart();
	virtual void OnWorkerExit(void* pContext);	

	virtual BOOL OnRequest(void* lpParam1, void* lpParam2);

	virtual BOOL OnUnsupported(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

			BOOL OnConnectSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnCloseSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnSendPulse(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnCheckNetwork(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnEnterRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual	BOOL OnEnterRoomVerified(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
			
			BOOL OnDarkSeated(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnDarkUnseated(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnDarkStarted(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
			BOOL OnDarkFinished(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

 			BOOL OnLeaveRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual	BOOL OnGetSeated(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
	virtual	BOOL OnGetUnseated(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
	virtual BOOL OnGetStarted(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
			BOOL OnGetFinished(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetLookon(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetUnlookon(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnAddFriend(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnAgreeFriend(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnDisagreeFriend(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnChatToPlayer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnInvitePlayer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnCallPlayer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnChatToRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnUnWelcome(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnForbidChat(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnForbidRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnRoomUserPulse(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnReconnectSvr(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

			BOOL OnQueryTablePwd(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnAnswerTablePwd(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnCommitUnwelcomes(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnCommitOneUnwelcome(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnRefreshClothing(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnCommitClientInfo(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnAskEnterGame(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnRefreshMember(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

			BOOL OnVipKickOffPlayer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

			// admin request 
			BOOL OnClearTable(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
 			BOOL OnClearProxyIPs(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
 			BOOL OnSaveProxyIPs(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

			BOOL OnKickoffRoomDead(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnKickoffRoomPlayer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
 			BOOL OnKickoffWalkAround(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

			BOOL OnSysMsgToRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnAdminMsgToRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnAdminMsgToRoomSvr(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);	// 发生网管消息给该roomsvr下所有房间的玩家
 			BOOL OnSetRandomTable(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnKickoffByExeVer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

			BOOL OnClearRoomChat(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetClientID(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetClientCount(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetPlayerCount(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetTokenCount(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

			BOOL OnTransmitNotify(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
 			BOOL OnGetRoomCapacity(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
  			BOOL OnGetRoomSvrConfig(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetMemoryStatus(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnWriteProfile(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetProfile(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnAddMutexHard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
 			BOOL OnDelMutexHard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetLogfileSize(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
 			BOOL OnResetCheckSvrIP(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetNoBufferCount(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnCleanStales(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetPlayers(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetOnePlayer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnGetClientInfo(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
public:
			BOOL OnMatchAdmin(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnAskEnterMatch(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnMatchLeaveRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
public:
			BOOL PutToServer(LPCONTEXT_HEAD lpContext, UINT nRequest, void* pData, int nLen);
			BOOL VerifyUserHardID(int nUserID,LPCTSTR lpszHardID);
			BOOL TransmitRequestEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest,void* pData,int nDataLen);
			CString GetConfigFileUnderFolder(LPCTSTR lpszFolder);
			int		ConvertMemberLevelToDisplay(int nMemberLevel);
			int		GetMsgTextColor(BOOL bMember);
			int     GetVipFaceByLevel(int nMemberLevel);
			CString GetRemainsBySeconds(int nSeconds);
			BOOL  IsForbidChatUserID(int nUserID,int& nRemains);
			BOOL  IsForbidChatMachine(LPCTSTR lpszMachineID,int& nRemains);
			BOOL  IsIncludeFilterWord(LPCTSTR lpszMsg);

			BOOL  IsForbidIP4(LPTSTR lpszIP,CString& sCause);
			BOOL  IsForbidIP3(LPTSTR lpszIP,CString& sCause);
			BOOL  IsForbidIP2(LPTSTR lpszIP,CString& sCause);

			BOOL  IsInsideIP(DWORD dwIPAddr);
			BOOL  IsProxyIP(DWORD dwIPAddr);
			BOOL  IsDeadGameSvr(int nGamePort,int nInterval);
			int   GetBusyWaitSecond();

			BOOL  IsForbidVipKick(int nUserID, CString& sCause);
			void  BeginSystemFindPlayers(CBaseRoomData* pCurRoom,int nNeedCount,LPBE_FOUND_BY_SYSTEM pBfbs,const TABLEAVGVALUE & avgValue);
public:
			BOOL MB_OnSendPulse(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
 			BOOL MB_OnEnterRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL MB_OnGetNewTable(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
			BOOL MB_OnLeaveRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL MB_OnGetFinished(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL MB_OnAskEnterGame(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL MB_OnRefreshMember(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL MB_OnGetGameVersion(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL MB_OnNewPrivateRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL MB_OnTryGotoOtherPrivateRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL MB_OnFoundPrivateRooms(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL MB_OnAskDetailTableRooms(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL MB_OnAskEnterPrivateRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL MB_OnSetOpeningPrivateRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL MB_OnAskSystemFindPlayers(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL MB_OnSetGameIsActived(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
public:
	///////////////////////////////////////////////////////////////////////
	///////        约牌系统      //////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////

	virtual BOOL YP_OnGetQuanTable(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
	virtual BOOL YP_OnChatToQuan(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
	virtual BOOL YP_OnEnterRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
	virtual BOOL YP_OnGetStarted(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
	virtual BOOL YP_OnGetQuanTableEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
	virtual BOOL YP_OnGetQuanTableExFetched(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
	virtual BOOL YP_OnGetQuanGameSet(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
	virtual BOOL YP_OnSetQuanGameSet(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
	virtual BOOL YP_OnSetQuanGameSetOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt){return TRUE;}
	virtual BOOL YP_GetClientAddress(LPCONTEXT_HEAD lpContext, DWORD& dwIPAddr){return TRUE;}	//约牌系统获取ip，统一用该接口，目前都用代理服务器ip
};

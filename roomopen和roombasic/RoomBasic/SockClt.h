#pragma once

class CBaseSockClient : public CDefIocpClient{
public:
	CBaseSockClient(const BYTE key[] = 0, const ULONG key_len = 0, DWORD flagEncrypt = 0, DWORD flagCompress = 0) 
		: CDefIocpClient(key, key_len, flagEncrypt, flagCompress){}
	CBaseSockClient(int nKeyType, DWORD flagEncrypt = 0, DWORD flagCompress = 0) 
		: CDefIocpClient(nKeyType, flagEncrypt, flagCompress){}
	virtual ~CBaseSockClient(){}
protected:
	virtual CBaseRoomData* CreateRoomData();
	virtual BOOL OnRequest(void* lpParam1, void* lpParam2);
	virtual BOOL OnNotifyPlayer(LPREQUEST lpRequest);
 	virtual BOOL OnNotifyClient(LPREQUEST lpRequest);
			BOOL OnSendPulse(LPREQUEST lpRequest);

 			BOOL OnClientForbid(LPREQUEST lpRequest);
 			BOOL OnRefreshResult(LPREQUEST lpRequest);
			BOOL OnRoomedKickoff(LPREQUEST lpRequest);

			BOOL OnTransferDeposit(LPREQUEST lpRequest);
			BOOL OnMoveSafeDeposit(LPREQUEST lpRequest);
			BOOL OnClearNegaScore(LPREQUEST lpRequest);
			BOOL OnJumpLevelScore(LPREQUEST lpRequest);
			BOOL OnTakeBackDeposit(LPREQUEST lpRequest);
			BOOL OnSaveBackDeposit(LPREQUEST lpRequest);

			BOOL OnSearchPlayerEx(LPREQUEST lpRequest);
			BOOL OnValidateAsk(LPREQUEST lpRequest);

			BOOL OnAddMutexHard(LPREQUEST lpRequest);
			BOOL OnDelMutexHard(LPREQUEST lpRequest);
			BOOL OnForbidChat(LPREQUEST lpRequest);

			BOOL OnResponse(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);

			BOOL OnDefault(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
	virtual	BOOL OnEnterRoomOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer){return TRUE;}
			BOOL OnEnterDarkRoomOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
			BOOL OnEnterWebGameRoomOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
			BOOL OnEnterCloakingRoomOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);

			BOOL OnEnterMatchRoomOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);

			BOOL OnEnterRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
			BOOL OnRoomNeedSignUp(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);

			BOOL OnUserLevelUp(LPREQUEST lpRequest);
			BOOL OnMemberLevelUp(LPREQUEST lpRequest);
			BOOL OnMemberUpdate(LPREQUEST lpRequest);
			////////////////////////////////////////////////////////////////
			BOOL OnUpdateRoom(LPREQUEST lpRequest);
			BOOL OnUpdateRoomFields(LPREQUEST lpRequest);
			BOOL OnUpdateRoomLevel(LPREQUEST lpRequest);
			BOOL OnUpdateRoomOCMG(LPREQUEST lpRequest);
			BOOL OnUpdateRoomIntField(LPREQUEST lpRequest);
			BOOL OnUpdateRoomCharField(LPREQUEST lpRequest);

			BOOL OnUpdateRoomSameIP(LPREQUEST lpRequest);
			BOOL OnAddFilterWord(LPREQUEST lpRequest);
			BOOL OnDeleteFilterWord(LPREQUEST lpRequest);
			BOOL OnUpdateUserStatus(LPREQUEST lpRequest);
			BOOL OnKickoffHallPlayer(LPREQUEST lpRequest);
			BOOL OnWriteRoomSvrProfile(LPREQUEST lpRequest);
			BOOL OnWriteIPManProfile(LPREQUEST lpRequest);
			BOOL OnRefreshFilterWord(LPREQUEST lpRequest);
			BOOL OnRefreshMutexHard(LPREQUEST lpRequest);
			BOOL OnWriteKickManProfile(LPREQUEST lpRequest);

			////////////////////////人数统计通知/////////////
			BOOL OnStatGameUsers(LPREQUEST lpRequest);
 			BOOL OnStatGroupUsers(LPREQUEST lpRequest);
			BOOL OnStatGroupUsersOnGame(LPREQUEST lpRequest);
			BOOL OnStatIPUsers(LPREQUEST lpRequest);
			BOOL OnStatIPUsersOnGame(LPREQUEST lpRequest);
	
			
public:
	////////////////////////充值到账///////////// 要放到procted里面去
			BOOL OnUserPayResult(LPREQUEST lpRequest);
			BOOL OnUserCurrencyExchange(LPREQUEST lpRequest);

			BOOL AT_OnMatchStart(LPMATCH_STARTTOROOM lpmsr);
			BOOL AT_OnEnterMatchReturn(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
			BOOL AT_OnMatchGain(LPREQUEST lpRequest);
 			BOOL AT_OnMatchCanceled(LPREQUEST lpRequest);
			BOOL AT_OnMatchSignUped(LPREQUEST lpRequest);
			BOOL AT_OnMatchSignUpOK(LPREQUEST lpRequest);//报名成功
			BOOL AT_OnMatchGiveUpOK(LPREQUEST lpRequest);//退赛成功
			void AT_OnMatchSignUpFail(LPMATCH_SIGNUPFAIL lpms);//报名失败
			void AT_OnMatchGiveUpFail(LPMATCH_GIVEUPFAIL lpmg);//退赛失败

			BOOL AT_OnAdminMatchAttach(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
			BOOL AT_OnAdminMatchStart(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);//管理员强制启动比赛
			BOOL AT_OnAdminMatchRestTable(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);//管理员要求一桌玩家重新分桌
			BOOL AT_OnAdminMatchTeminate(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer); //管理员强制终止比赛
			BOOL AT_OnAdminMatchRefresh(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);  //管理员刷新比赛数据
			BOOL AT_OnAdminMatchKickOff(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);//管理员踢出比赛玩家
			BOOL AT_OnAdminMatchResetAllTable(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);//管理员要求所有玩家重新分桌
public:

			BOOL YP_OnGetQuanTableEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
			BOOL YP_OnSetQuanGameSetOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);

};

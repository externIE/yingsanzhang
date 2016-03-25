#pragma once
 
class CBaseRoomServer : public CDefIocpServer{
public:
	CBaseRoomServer(const BYTE key[] = 0, const ULONG key_len = 0, DWORD flagEncrypt = 0, DWORD flagCompress = 0) 
		: CDefIocpServer(key, key_len, flagEncrypt, flagCompress){}
	CBaseRoomServer(int nKeyType, DWORD flagEncrypt = 0, DWORD flagCompress = 0) 
		: CDefIocpServer(nKeyType, flagEncrypt, flagCompress){}
    virtual ~CBaseRoomServer(){}
protected:
	virtual void* OnWorkerStart();
	virtual void OnWorkerExit(void* pContext);	

	virtual BOOL OnRequest(void* lpParam1, void* lpParam2);

	virtual BOOL OnUnsupported(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

	virtual BOOL OnConnectSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnCloseSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnSendPulse(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnRegisterGameSvr(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnMatchUserReady(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
 	virtual BOOL OnMatchUserLeave(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

	virtual BOOL OnCommitDiff(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
 	
	virtual BOOL OnMatchPlayerAskSignUp(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnMatchPlayerAskGiveUp(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnMatchPlayerAskRankList(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnMatchPlayerOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);//G2R_MATCH_PLAYER_OK
	virtual BOOL OnMatchArrageFaild(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

 	virtual BOOL OnGame2Room(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
 	virtual BOOL OnEnterMatchToCheck(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

public:
	virtual BOOL PutToServer(LPCONTEXT_HEAD lpContext, UINT nRequest, void* pData, int nLen);
public:

};

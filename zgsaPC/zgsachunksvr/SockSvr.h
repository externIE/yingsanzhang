#pragma once

#define MAX_DBCONNECT_COUNT		10
#define DEF_DBCONNECT_COUNT		2

#define	DEF_REGISTER_INTERVAL	60 // minutes

class CWorkerContext
{

public:
	BOOL            m_bReConnectMain;
	_ConnectionPtr  m_pDBConnectMain;

     BOOL           m_bReConnectGame;
	_ConnectionPtr  m_pDBConnectGame;

  
public:

	CWorkerContext()
	{
		m_bReConnectMain = FALSE;
		m_pDBConnectMain = NULL;

		m_bReConnectGame = FALSE;
		m_pDBConnectGame = NULL;
	}


};


class CSockServer : public CDefIocpServer{
public:
	CSockServer(const BYTE key[] = 0, const ULONG key_len = 0, DWORD flagEncrypt = 0, DWORD flagCompress = 0) 
		: CDefIocpServer(key, key_len, flagEncrypt, flagCompress){}

	CSockServer(int nKeyType, DWORD flagEncrypt = 0, DWORD flagCompress = 0) 
		: CDefIocpServer(nKeyType, flagEncrypt, flagCompress){}

	virtual BOOL TransmitRequest(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int svrindex);
	virtual BOOL TransmitRequestEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest,int svrindex,void* pData,int nDataLen);

protected:
	virtual BOOL ReConnectMainDB(CWorkerContext* pThreadCxt);
	virtual BOOL ReConnectGameDB(CWorkerContext* pThreadCxt);


	virtual void* OnWorkerStart();
	virtual void OnWorkerExit(void* pContext);

	virtual BOOL OnRequest(void* lpParam1, void* lpParam2);

	virtual BOOL OnUnsupported(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

	virtual BOOL OnCloseSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnConnectSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnSendPulse(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnValidateClient(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnValidateClientEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

	virtual BOOL OnValidateGameSvr(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
 
	virtual BOOL SendOpeResponse(LPCONTEXT_HEAD lpContext, BOOL bNeedEcho, REQUEST& response);//有回应的请求
	virtual BOOL SendOpeRequest(LPCONTEXT_HEAD lpContext, REQUEST& response);				  //无回应的请求
	virtual BOOL SendOpeRequest(LPCONTEXT_HEAD lpContext, void* pData, int nLen, REQUEST& response);  //无回应的请求
	virtual BOOL SendOpeReqOnlyCxt(LPCONTEXT_HEAD lpContext, UINT nRepeatHead, void* pData, REQUEST& response); //无回应的请求,只回报文头
	

	///////////////////////////////
 	virtual BOOL OnGetClientCount(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnQueryPlayerLogon(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

	//example
	virtual BOOL OnGetPropInfo(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnUseLuckCard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

public:
	virtual BOOL    PutToServer(LPCONTEXT_HEAD lpContext, UINT nRequest, void* pData, int nLen);

	virtual CString GetProfileContent();

};


#pragma once

class CSockClient : public CDefIocpClient{
public:
	CSockClient(const BYTE key[] = 0, const ULONG key_len = 0, DWORD flagEncrypt = 0, DWORD flagCompress = 0,
		int svrindex = 0) 
		: CDefIocpClient(key, key_len, flagEncrypt, flagCompress)
	{
		m_nSvrIndex = svrindex;
	}
	
	CSockClient(int nKeyType,  DWORD flagEncrypt = 0, DWORD flagCompress = 0,
		int svrindex = 0) 
		: CDefIocpClient(nKeyType, flagEncrypt, flagCompress)
	{
		m_nSvrIndex = svrindex;
	}
	
public:
	int	m_nSvrIndex;
protected:
	virtual BOOL OnRequest(void* lpParam1, void* lpParam2);
	
	virtual BOOL OnNotifyClient(LPREQUEST lpRequest);
	
	virtual BOOL OnResponse(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
	
	virtual BOOL OnDefault(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
	
	virtual BOOL PutToServer(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);

	virtual BOOL GoToServer(LPREQUEST lpRequest);
	
	virtual BOOL OnNTFPlayerLogon(LPREQUEST lpRequest);

	virtual BOOL OnNTFPlayerLogoff(LPREQUEST lpRequest);
	
	virtual BOOL OnQueryPlayerLogon(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer); 

	virtual BOOL OnUserPayResult(LPREQUEST lpRequest);
};

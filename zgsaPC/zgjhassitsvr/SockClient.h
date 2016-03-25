#pragma once

class CAssitServer;

class CSockClient : public CDefIocpClient{
public:
	CSockClient(CAssitServer* pMainServer, 
				const BYTE key[] = 0, const ULONG key_len = 0, DWORD flagEncrypt = 0, DWORD flagCompress = 0) 
		: CDefIocpClient(key, key_len, flagEncrypt, flagCompress)
	{
		m_pMainServer = pMainServer;
	}
	
	CSockClient(CAssitServer* pMainServer, int nKeyType,  DWORD flagEncrypt = 0, DWORD flagCompress = 0) 
		: CDefIocpClient(nKeyType, flagEncrypt, flagCompress)
	{
		m_pMainServer = pMainServer;
	}

protected:
	virtual BOOL OnRequest(void* lpParam1, void* lpParam2);		
	virtual BOOL OnNotifyClient(LPREQUEST lpRequest);
	virtual BOOL OnSendPulse(LPREQUEST lpRequest);
	virtual BOOL OnNTFPlayerLogon(LPREQUEST lpRequest);
	virtual BOOL OnNTFPlayerLogoff(LPREQUEST lpRequest);

	virtual BOOL OnResponse(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromSvr);

	virtual BOOL OnDefault(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromSvr);

protected:
	CAssitServer* m_pMainServer;
};

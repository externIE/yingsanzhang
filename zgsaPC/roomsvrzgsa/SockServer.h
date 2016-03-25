#pragma once

class CSockServer : public CSockOpenServer{
public:
	CSockServer(const BYTE key[] = 0, const ULONG key_len = 0, DWORD flagEncrypt = 0, DWORD flagCompress = 0);

	CSockServer(int nKeyType, DWORD flagEncrypt = 0, DWORD flagCompress = 0) ;
		
	virtual ~CSockServer();

	virtual BOOL OnRequest(void* lpParam1, void* lpParam2);
	virtual	BOOL OnEnterRoomVerified(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

};

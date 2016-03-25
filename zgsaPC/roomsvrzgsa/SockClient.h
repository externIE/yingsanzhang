#pragma once

class CSockClient : public CSockOpenClient{
public:
	CSockClient(const BYTE key[] = 0, const ULONG key_len = 0, DWORD flagEncrypt = 0, DWORD flagCompress = 0);
	CSockClient(int nKeyType, DWORD flagEncrypt = 0, DWORD flagCompress = 0) ;
	virtual ~CSockClient();
	virtual	BOOL OnEnterRoomOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);

protected:
	virtual CBaseRoomData* CreateRoomData();
};

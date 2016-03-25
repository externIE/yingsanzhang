#pragma once

#include "StdAfx.h"

typedef struct _tagTOKEN_INFO{
	LONG		lTokenID;
	SOCKET		hSocket;
	int			nPulseTime;    //2038√Î
	int         nRoomID;
	int         nUserID;
	int			nReserved[4];
}TOKEN_INFO, *LPTOKEN_INFO;

typedef CMap<LONG, LONG, TOKEN_INFO, TOKEN_INFO&>				CTokenInfoMap;

typedef CMap<LONG, LONG, int, int> CTokenRoomMap;
typedef CMap<LONG, LONG, int, int> CTokenPlayerMap;
typedef CMap<int, int, int, int>   CIntIntMap;


class CWorkerContext{
public:
	_ConnectionPtr	pDBConnect;
};

class CSockServer : public CDefIocpServer{
public:
	CSockServer(const BYTE key[]=0, const ULONG key_len = 0, DWORD flagEncrypt = 0, DWORD flagCompress = 0) 
		: CDefIocpServer(key, key_len, flagEncrypt, flagCompress){}

	CSockServer(int nKeyType, DWORD flagEncrypt = 0, DWORD flagCompress = 0) 
		: CDefIocpServer(nKeyType, flagEncrypt, flagCompress){}

	virtual ~CSockServer(){}
	virtual void Shutdown();
	virtual BOOL SendUserResponse(LPCONTEXT_HEAD lpContext, LPREQUEST lpResponse, BOOL passive = FALSE, BOOL compressed = FALSE);
	virtual BOOL SendResponseData(LPCONTEXT_HEAD lpContext, REQUEST& response, void* pData, int nLen);
	virtual BOOL SendErrorInfoResponse(LPCONTEXT_HEAD lpContext, LPCTSTR strInfo);
	virtual BOOL SendFailedResponse(LPCONTEXT_HEAD lpContext, BOOL passive = FALSE, BOOL compressed = FALSE);
	virtual BOOL NotifyOneUser(SOCKET sock, LONG token, UINT nRequest, void* pData, int nLen, BOOL compressed = FALSE);
protected:
	virtual void* OnWorkerStart();
	virtual void OnWorkerExit(void* pContext);

	virtual BOOL OnRequest(void* lpParam1, void* lpParam2);

	virtual BOOL OnUnsupported(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	
	virtual void ClearRoomMap();
};

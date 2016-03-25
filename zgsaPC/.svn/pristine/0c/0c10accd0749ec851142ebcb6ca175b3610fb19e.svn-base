#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



int GetDBServerIndexByType(int nType)
{
	for(int i=0;i<MAX_TOTALDB_COUNT;i++)
	{
		if(  g_ChunkDB[i].nType==nType)
		{
			return i;
		}
	}
	return -1;
}

BOOL CSockServer::ReConnectMainDB(CWorkerContext* pThreadCxt)
{
	DB_SafeClose(pThreadCxt->m_pDBConnectMain);

 	int nIndex=GetDBServerIndexByType(TYPE_CHUNKDB_MAIN);
	if(nIndex!=-1)
	{
		CString sConnect=DB_GetConnectStr(&g_ChunkDB[nIndex]);
		if(DB_SafeOpen(pThreadCxt->m_pDBConnectMain, sConnect))
		{
			UwlLogFile(_T("《《《《《《《《ReConnectMainDB() succeed! threadid = %ld"), GetCurrentThreadId());

			return TRUE;
		}
	}
	return FALSE;
}

BOOL CSockServer::ReConnectGameDB(CWorkerContext* pThreadCxt)
{	
	DB_SafeClose(pThreadCxt->m_pDBConnectGame);

	int nIndex=GetDBServerIndexByType(TYPE_CHUNKDB_GAME);
	if(nIndex!=-1)
	{
		CString sConnect=DB_GetConnectStr(&g_ChunkDB[nIndex]);
		if(DB_SafeOpen(pThreadCxt->m_pDBConnectGame, sConnect))
		{
			UwlLogFile(_T("ReConnectGameDB() succeed! threadid = %ld"), GetCurrentThreadId());
			
			return TRUE;
		}
	}
	return FALSE;

}


/*
 

 

BOOL CSockServer::ReConnectMatchDB(CWorkerContext* pThreadCxt)
{
	DB_SafeClose(pThreadCxt->m_pDBConnectMatch);

 	int nIndex=GetDBServerIndexByType(TYPE_MATCH_DBS);
	if(nIndex!=-1)
	{
		CString sConnect=DB_GetConnectStr(&g_DBServer[nIndex]);
		if(DB_SafeOpen(pThreadCxt->m_pDBConnectMatch, sConnect))
		{
			UwlLogFile(_T("ReConnectMatchDB() succeed! threadid = %ld"), GetCurrentThreadId());
		}
	}
	return (pThreadCxt->m_pDBConnectMatch != NULL);
}

*/

void* CSockServer::OnWorkerStart()
{
	HRESULT hr = ::CoInitialize(NULL);
	if(FAILED(hr)) return NULL;

	CWorkerContext* pThreadCxt = new CWorkerContext();
	
	ReConnectMainDB(pThreadCxt);	
	ReConnectGameDB(pThreadCxt);

	return pThreadCxt;
}

void CSockServer::OnWorkerExit(void* pContext)
{
	CWorkerContext* pThreadCxt = (CWorkerContext*)(pContext);
	if(pThreadCxt)
	{
		DB_SafeClose(pThreadCxt->m_pDBConnectMain);
		DB_SafeClose(pThreadCxt->m_pDBConnectGame);
	}

	SAFE_DELETE(pThreadCxt);
	::CoUninitialize();
}

BOOL CSockServer::OnRequest(void* lpParam1, void* lpParam2)
{try{
	LPCONTEXT_HEAD	lpContext = LPCONTEXT_HEAD	(lpParam1);
	LPREQUEST		lpRequest = LPREQUEST		(lpParam2);
	
	UwlTrace(_T("----------------------start of request process-------------------"));
	
#if defined(_UWL_TRACE) | defined(UWL_TRACE)
	DWORD dwTimeStart = GetTickCount();
#else
	DWORD dwTimeStart = 0;
#endif
	CWorkerContext* pThreadCxt = (CWorkerContext*)(GetWorkerContext());

	assert(lpContext && lpRequest);
	UwlTrace(_T("socket = %ld requesting..."), lpContext->hSocket);
 
	switch(lpRequest->head.nRequest){
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
	case GR_VALIDATE_CLIENT:
		UwlTrace(_T("GR_VALIDATE_CLIENT requesting..."));
		OnValidateClient(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_VALIDATE_CLIENT_EX:
		UwlTrace(_T("GR_VALIDATE_CLIENT_EX requesting..."));
		OnValidateClientEx(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_VALIDATE_GAMESVR_EX:
		UwlTrace(_T("GR_VALIDATE_GAMESVR_EX requesting..."));
		OnValidateGameSvr(lpContext, lpRequest, pThreadCxt);
		break;

	case GR_SEND_PULSE_EX:
		UwlTrace(_T("GR_SEND_PULSE_EX requesting..."));
		OnSendPulse(lpContext, lpRequest, pThreadCxt);
		break;
		/*
	case GR_REFRESH_RESULT_EX:
		UwlTrace(_T("GR_REFRESH_RESULT_EX requesting..."));
		OnRefreshResultEx(lpContext, lpRequest, pThreadCxt);
		break;
 	case GR_ENTER_GAME_EX:
		UwlTrace(_T("GR_ENTER_GAME_EX requesting..."));
		OnEnterGameEx(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_REFRESH_RESULT_EX_FROM_KUB:
		UwlTrace(_T("GR_REFRESH_RESULT_EX_FROM_KUB requesting..."));
		OnRefreshResultExFromKub(lpContext, lpRequest, pThreadCxt);
 		break;
		*/
 	case GR_GET_CLIENT_COUNT:
		UwlTrace(_T("GR_GET_CLIENT_COUNT requesting..."));
		OnGetClientCount(lpContext, lpRequest, pThreadCxt);
		break;
	//example
	case GR_GET_PROPINFO:
		UwlTrace(_T("GR_GET_PROPINFO requesting..."));
		OnGetPropInfo(lpContext, lpRequest, pThreadCxt);
		break;
	case GR_USE_LUCKCARD:
		UwlTrace(_T("GR_USE_LUCKCARD requesting..."));
		OnUseLuckCard(lpContext, lpRequest, pThreadCxt);
		break;
	//example end
	case GR_QUERY_PLAYERLOGON:
		OnQueryPlayerLogon(lpContext, lpRequest, pThreadCxt);
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

	//数据库重连机制
	// maindb reconnection
	if(pThreadCxt->m_bReConnectMain)
	{
		//Modify by YeWen on 20150918 for reconnecting db every time when connect failed!
	    pThreadCxt->m_bReConnectMain = !ReConnectMainDB(pThreadCxt);
 		UwlLogFile(_T("maindb reconnection! threadid = %ld"), GetCurrentThreadId());
	}
 
	// gamedb reconnection
	if(pThreadCxt->m_bReConnectGame)
	{
		//Modify by YeWen on 20150918 for reconnecting db every time when connect failed!
	    pThreadCxt->m_bReConnectGame = !ReConnectGameDB(pThreadCxt);
 		UwlLogFile(_T("gamedb reconnection! threadid = %ld"), GetCurrentThreadId());
	}

}
catch(...)
{
	LPREQUEST		lpReq  = LPREQUEST(lpParam2);
   	UwlLogFile("catch error!!! reqid=%ld,subreqid=%ld,DataLen=%ld",lpReq->head.nRequest,lpReq->head.nSubReq,lpReq->nDataLen );

}
	return TRUE;
}

BOOL CSockServer::OnUnsupported(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest,
							CWorkerContext* pThreadCxt)
{
	UwlTrace(_T("unsupported request from socket = %d"), lpContext->hSocket);

	return TRUE;
}

BOOL CSockServer::TransmitRequest(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int svrindex)
{
	LPREQUEST pRequest = new REQUEST;
	memcpy(pRequest, lpRequest, sizeof(REQUEST));

	pRequest->pDataPtr = new BYTE[lpRequest->nDataLen];
	pRequest->nDataLen = lpRequest->nDataLen;
	memcpy(pRequest->pDataPtr, lpRequest->pDataPtr, lpRequest->nDataLen);

	LPCONTEXT_HEAD pContext = new CONTEXT_HEAD;
	memcpy(pContext, lpContext, sizeof(CONTEXT_HEAD));
	
	if(!PostThreadMessage(g_uiThrdSends[svrindex], UM_DATA_TOSEND, (WPARAM)pContext, (LPARAM)pRequest)){
		UwlClearRequest(pRequest);
		SAFE_DELETE(pRequest);
		SAFE_DELETE(pContext);
		return FALSE;
	}else{
		return TRUE;
	}
}
 

BOOL CSockServer::TransmitRequestEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int svrindex,void* pData,int nDataLen)
{
	LPREQUEST pRequest = new REQUEST;
	memcpy(pRequest, lpRequest, sizeof(REQUEST));

	pRequest->pDataPtr = new BYTE[nDataLen];
	pRequest->nDataLen = nDataLen;
	memcpy(pRequest->pDataPtr,  pData, nDataLen);

	LPCONTEXT_HEAD pContext = new CONTEXT_HEAD;
	memcpy(pContext, lpContext, sizeof(CONTEXT_HEAD));
	
	if(!PostThreadMessage(g_uiThrdSends[svrindex], UM_DATA_TOSEND, (WPARAM)pContext, (LPARAM)pRequest)){
		UwlClearRequest(pRequest);
		SAFE_DELETE(pRequest);
		SAFE_DELETE(pContext);
		return FALSE;
	}else{
		return TRUE;
	}
}
BOOL CSockServer::PutToServer(LPCONTEXT_HEAD lpContext, UINT nRequest, void* pData, int nLen)
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

BOOL CSockServer::SendOpeResponse(LPCONTEXT_HEAD lpContext, BOOL bNeedEcho, REQUEST& response)
{
	BOOL bSendOK = FALSE;
	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = bNeedEcho;
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
	return bSendOK;
}

BOOL CSockServer::SendOpeRequest(LPCONTEXT_HEAD lpContext, REQUEST& response)
{
	CONTEXT_HEAD context;
	memset(&context,0,sizeof(context));
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE; 
	
	BOOL bSendOK=SendRequest(lpContext->hSocket, &context, &response);
	return bSendOK;
}

BOOL CSockServer::SendOpeRequest(LPCONTEXT_HEAD lpContext, void* pData, int nLen, REQUEST& response)
{
	CONTEXT_HEAD context;
	memset(&context,0,sizeof(context));
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE; 
	
	PBYTE pNewData = NULL;
	pNewData = new BYTE[nLen];
	memset(pNewData,0,nLen);
	memcpy(pNewData, pData, nLen);
	response.pDataPtr = pNewData;//!
	response.nDataLen = nLen;
	
	BOOL bSendOK=SendRequest(lpContext->hSocket, &context, &response);
	UwlClearRequest(&response);
	return bSendOK;
}

BOOL CSockServer::SendOpeReqOnlyCxt(LPCONTEXT_HEAD lpContext, UINT nRepeatHead, void* pData, REQUEST& response)
{
	if (0==nRepeatHead || !pData)
		return FALSE;

	int nLen = nRepeatHead*sizeof(CONTEXT_HEAD);
	return SendOpeRequest(lpContext, pData, nLen, response);
}

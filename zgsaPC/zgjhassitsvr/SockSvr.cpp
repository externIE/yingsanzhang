#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif  

void CSockServer::Shutdown()
{
	CDefIocpServer::Shutdown();
		
	ClearRoomMap();
}

void* CSockServer::OnWorkerStart()
{
	_ConnectionPtr pDBConnect = NULL;
	HRESULT hr = 0;

	hr = ::CoInitialize(NULL);
	if(FAILED(hr)) return NULL;

	CWorkerContext* pThreadCxt = new CWorkerContext();
	pThreadCxt->pDBConnect = pDBConnect;
	return pThreadCxt;
}

void CSockServer::OnWorkerExit(void* pContext)
{
	CWorkerContext* pThreadCxt = (CWorkerContext*)(pContext);
	if(pThreadCxt){
		SAFE_DELETE(pThreadCxt);
	}
	::CoUninitialize();
}

BOOL CSockServer::OnRequest(void* lpParam1, void* lpParam2)
{
	LPCONTEXT_HEAD	lpContext = LPCONTEXT_HEAD	(lpParam1);
	LPREQUEST		lpRequest = LPREQUEST		(lpParam2);

	UwlTrace(_T("----------------------start of request process (default)-------------------"));
	
#if defined(_UWL_TRACE) | defined(UWL_TRACE)
	DWORD dwTimeStart = GetTickCount();
#else
	DWORD dwTimeStart = 0;
#endif
	CWorkerContext* pThreadCxt = reinterpret_cast<CWorkerContext*>(GetWorkerContext());

	assert(lpContext && lpRequest);
	UwlTrace(_T("socket = %ld requesting..."), lpContext->hSocket);
	switch(lpRequest->head.nRequest){
	case UR_SOCKET_CONNECT:
		UwlTrace(_T("UR_SOCKET_CONNECT requesting..."));
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
	UwlTrace(_T("----------------------end of request process (default)---------------------\r\n"));

	return TRUE;
}

BOOL CSockServer::OnUnsupported(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest,
							CWorkerContext* pThreadCxt)
{
	UwlTrace(_T("unsupported request from socket = %d"), lpContext->hSocket);

	return TRUE;
}

BOOL CSockServer::SendUserResponse(LPCONTEXT_HEAD lpContext, LPREQUEST lpResponse, BOOL passive, BOOL compressed)
{
	if(passive) return TRUE;

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;

	
	if(compressed){
		bSendOK = SendResponse(lpContext->hSocket, &context, lpResponse, MSG_RESPONSE, 0, COMPRESS_ZIP);
	}else{
		bSendOK = SendResponse(lpContext->hSocket, &context, lpResponse);
	}
	return bSendOK;
}

BOOL CSockServer::SendResponseData(LPCONTEXT_HEAD lpContext, REQUEST& response, 
										void* pData, int nLen)
{
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	response.pDataPtr = pData;
	response.nDataLen = nLen;
	BOOL bSendOK = SendUserResponse(lpContext, &response);
	return bSendOK;
}

BOOL CSockServer::SendErrorInfoResponse(LPCONTEXT_HEAD lpContext, LPCTSTR strInfo)
{
	if (!lpContext || !strInfo)
	{
		return SendFailedResponse(lpContext);
	}
	
	int nLen = lstrlen(strInfo) + 1;
	PBYTE pData = new BYTE[nLen];
	memcpy(pData, strInfo, nLen-1);
	pData[nLen-1]='\0';
	
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	response.head.nRequest = GR_ERROR_INFOMATION_EX;
	response.pDataPtr = pData;
	response.nDataLen = nLen;
	
	BOOL bRet=SendUserResponse(lpContext, &response);
	SAFE_DELETE_ARRAY(pData);
	
	return bRet;
}

BOOL CSockServer::SendFailedResponse(LPCONTEXT_HEAD lpContext, BOOL passive /*= FALSE*/, BOOL compressed /*= FALSE*/)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRequest = UR_OPERATE_FAILED;
	
	return SendUserResponse(lpContext, &response, passive, compressed);
}

BOOL CSockServer::NotifyOneUser(SOCKET sock, LONG token, UINT nRequest, void* pData, int nLen, BOOL compressed)
{
	if (sock==0||token==0) return FALSE;

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));

	context.bNeedEcho = FALSE;
	context.hSocket = sock;
	context.lTokenID = token;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));

	request.head.nRequest = nRequest;
	request.pDataPtr = pData;
	request.nDataLen = nLen;

	return bSendOK = SendRequest(context.hSocket, &context, &request, MSG_REQUEST, 0, COMPRESS_ZIP);
}
   
void CSockServer::ClearRoomMap()
{
// 	return;
// 	CAutoLock lock(&m_csRoomMap);
// 	int roomid = 0;
// 	CRoom* ptrR = NULL;
// 	POSITION pos = m_mapRoom.GetStartPosition();
// 	while (pos)	{
// 		m_mapRoom.GetNextAssoc(pos, roomid, ptrR);
// 		if(ptrR){
// 			m_mapRoom.RemoveKey(roomid);
// 			SAFE_DELETE(ptrR);
// 		}
// 	}
}
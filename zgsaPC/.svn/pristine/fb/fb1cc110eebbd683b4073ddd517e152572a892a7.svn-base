#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL CSockClient::OnRequest(void* lpParam1, void* lpParam2)
{
	LPCONTEXT_HEAD pContext = LPCONTEXT_HEAD(lpParam1);
	LPREQUEST pRequest = LPREQUEST(lpParam2);
	
	// if received msg of socket close. we do reconnect
	if(UR_SOCKET_CLOSE == pRequest->head.nRequest){

		UwlTrace(_T("Disconnected from chunk server."));
		UwlLogFile(_T("Disconnected from chunk server."));

		PostThreadMessage(m_pMainServer->m_uiThrdConnect, UM_DATA_TOSEND, UR_SOCKET_CLOSE, 0);
		return TRUE;
	}

	if(0 == pRequest->head.nRepeated)
	{ 
		OnNotifyClient(pRequest);
	}
	else
	{	
		CONTEXT_HEAD context;
		memcpy(&context, pRequest->pDataPtr, sizeof(context));
		context.bNeedEcho = FALSE;

		REQUEST request;
		memset(&request, 0, sizeof(request));
		request.head.nRequest = pRequest->head.nRequest;
		request.head.nSubReq  = pRequest->head.nSubReq;

		OnResponse(&context, &request, pRequest);
	
		UwlClearRequest(&request);
	}
	UwlClearRequest(pRequest);
	return TRUE;
}

BOOL CSockClient::OnNotifyClient(LPREQUEST lpRequest)
{
	switch(lpRequest->head.nRequest){
	
	case GR_SEND_PULSE:
		OnSendPulse(lpRequest);
		break;

	case GR_NTF_PLAYERLOGON:
		OnNTFPlayerLogon(lpRequest);
		break;
		
	case GR_NTF_PLAYERLOGOFF:
		OnNTFPlayerLogoff(lpRequest);
		break;
		
	default://
		break;
	}
	return TRUE;
}  

BOOL CSockClient::OnSendPulse(LPREQUEST lpRequest)
{
	if (!m_pMainServer)
		return FALSE;

	return m_pMainServer->OnChunkSendPulse(lpRequest);
} 

BOOL CSockClient::OnNTFPlayerLogon(LPREQUEST lpRequest)
{
	LPNTF_PLAYERLOGON lpLogon = (LPNTF_PLAYERLOGON)lpRequest->pDataPtr;
	
	if (lpLogon)
	{
		m_pMainServer->OnUserLogon(lpLogon);
	}
	return TRUE;
}

BOOL CSockClient::OnNTFPlayerLogoff(LPREQUEST lpRequest)
{
	LPNTF_PLAYERLOGOFF lpLogoff = (LPNTF_PLAYERLOGOFF)lpRequest->pDataPtr;
	
	if (lpLogoff)
	{
		m_pMainServer->OnUserLogoff(lpLogoff);
	}
	return TRUE;
}
 
BOOL CSockClient::OnResponse(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromSvr)
{
	switch(lpReqToClient->head.nRequest){

	case GR_GET_PROPINFO:
		{
			m_pMainServer->OnGetPropInfoRet(lpContext, lpReqToClient, lpReqFromSvr);
		}
		break;
	case GR_USE_LUCKCARD:
		{
			m_pMainServer->OnUseLuckCardRet(lpContext, lpReqToClient, lpReqFromSvr);
		}
		break;

 	default:
		OnDefault(lpContext, lpReqToClient, lpReqFromSvr);
		break;
	}
	return TRUE;
}

BOOL CSockClient::OnDefault(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	int nLen = lpReqFromServer->nDataLen - sizeof(CONTEXT_HEAD);
	PBYTE pData = new BYTE[nLen];
	lpReqToClient->pDataPtr = pData;
	lpReqToClient->nDataLen = nLen;
	memcpy(lpReqToClient->pDataPtr, PBYTE(lpReqFromServer->pDataPtr) + sizeof(CONTEXT_HEAD), nLen);

	BOOL bSendOK = m_pMainServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient);
	return TRUE;
}

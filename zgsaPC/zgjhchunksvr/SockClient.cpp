#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <TaskReq.h>

BOOL CSockClient::OnRequest(void* lpParam1, void* lpParam2)
{
	LPCONTEXT_HEAD pContext = LPCONTEXT_HEAD(lpParam1);
	LPREQUEST pRequest = LPREQUEST(lpParam2);
	
	// begin socket close msg received. we do reconnect
	if(UR_SOCKET_CLOSE == pRequest->head.nRequest){

		UwlTrace(_T("Disconnected from server. svrindex = %ld"), m_nSvrIndex);
		UwlLogFile(_T("Disconnected from server. svrindex = %ld"), m_nSvrIndex);
		
		PostThreadMessage(g_uiThrdConnects[m_nSvrIndex], UM_DATA_TOSEND, UR_SOCKET_CLOSE, 0);
		return TRUE;
	}
	// end
	if(0 == pRequest->head.nRepeated){
		OnNotifyClient(pRequest);
	}else{
		CONTEXT_HEAD context;
		memcpy(&context, pRequest->pDataPtr, sizeof(context));
		context.bNeedEcho = FALSE;

		REQUEST request;
		memset(&request, 0, sizeof(request));
		request.head.nRequest = pRequest->head.nRequest;
		request.head.nRepeated = 1;

		OnResponse(&context, &request, pRequest);
	
		UwlClearRequest(&request);
	}
	UwlClearRequest(pRequest);
	return TRUE;
}

BOOL CSockClient::OnNotifyClient(LPREQUEST lpRequest)
{
	switch(lpRequest->head.nRequest){
  

 	case GR_NTF_PLAYERLOGON:
		OnNTFPlayerLogon(lpRequest);
		break;
		
	case GR_NTF_PLAYERLOGOFF:
		OnNTFPlayerLogoff(lpRequest);
		break;

	case GR_USER_PAY_RESULT:
		OnUserPayResult(lpRequest);
		break;


	default:
		break;
	}
	return TRUE;
}
	
BOOL CSockClient::OnResponse(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	
	switch(lpReqToClient->head.nRequest)
	{
 	case GR_QUERY_PLAYERLOGON:
  		OnQueryPlayerLogon(lpContext, lpReqToClient, lpReqFromServer);
		break;
 	default:
		OnDefault(lpContext, lpReqToClient, lpReqFromServer);
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

	BOOL bSendOK = g_pSockServer->SendRequest(lpContext->hSocket, lpContext, lpReqToClient);
	return TRUE;
}

BOOL CSockClient::PutToServer(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{ 
	LPCONTEXT_HEAD pContext = new CONTEXT_HEAD;
	memcpy(pContext, lpContext, sizeof(CONTEXT_HEAD));

	LPREQUEST pRequest = new REQUEST;
	ZeroMemory(pRequest, sizeof(REQUEST));
	
	pRequest->head.nRequest = lpReqFromServer->head.nRequest;
	pRequest->head.nRepeated = 1;

	int nLen = lpReqFromServer->nDataLen - sizeof(CONTEXT_HEAD);
	PBYTE pData = new BYTE[nLen];
	pRequest->pDataPtr = pData;
	pRequest->nDataLen = nLen;
	memcpy(pRequest->pDataPtr, PBYTE(lpReqFromServer->pDataPtr) + sizeof(CONTEXT_HEAD), nLen);

	g_pSockServer->PutRequestToWorker(pRequest->nDataLen, DWORD(pContext->hSocket),
					pContext, pRequest, pRequest->pDataPtr);
	return TRUE;
}
 
BOOL CSockClient::OnNTFPlayerLogon(LPREQUEST lpRequest)
{
	LPNTF_PLAYERLOGON lpNtfPlayerLogon = (LPNTF_PLAYERLOGON)lpRequest->pDataPtr;

	if (lpNtfPlayerLogon)
	{
		NotifyAllAssitSvr(GR_NTF_PLAYERLOGON, lpRequest->pDataPtr, lpRequest->nDataLen);
	}
	return TRUE;
}

BOOL CSockClient::OnNTFPlayerLogoff(LPREQUEST lpRequest)
{
	LPNTF_PLAYERLOGOFF lpNtfPlayerLogoff = (LPNTF_PLAYERLOGOFF)lpRequest->pDataPtr;
	
	if (lpNtfPlayerLogoff)
	{
		NotifyAllAssitSvr(GR_NTF_PLAYERLOGOFF, lpRequest->pDataPtr, lpRequest->nDataLen);;
	}

	return TRUE;
}


BOOL CSockClient::OnQueryPlayerLogon(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	//subsvr������ 2����ͷCONTEXT_HEAD + NTF_PLAYERLOGON�ṹ
	//��һ����ͷ�� assitsvr��chunksvr���ӵİ�ͷ
	//�ڶ�����ͷ�� �ͻ��ˣ��������������assitsvr�İ�ͷ
	//��¼֪ͨ���跢���ͻ��ˣ���������ֱ��ȥ����������ͷ��ת���� NTF_PLAYERLOGON�ṹ �� GR_NTF_PLAYERLOGON��¼֪ͨ
	int nLen = lpReqFromServer->nDataLen - lpReqFromServer->head.nRepeated * sizeof(CONTEXT_HEAD);	

	if (nLen != sizeof(NTF_PLAYERLOGON))
		return FALSE;

	PBYTE pData = new BYTE[nLen];
	lpReqToClient->pDataPtr = pData;
	lpReqToClient->nDataLen = nLen;
	memcpy(lpReqToClient->pDataPtr, PBYTE(lpReqFromServer->pDataPtr) + lpReqFromServer->head.nRepeated * sizeof(CONTEXT_HEAD), nLen);

	//��Ϣ�����õ���½֪ͨ
	lpReqToClient->head.nRequest = GR_NTF_PLAYERLOGON;		//������¼֪ͨ
	lpReqToClient->head.nRepeated = 0;
	
	BOOL bSendOK = g_pSockServer->SendRequest(lpContext->hSocket, lpContext, lpReqToClient);

	return TRUE;
}

BOOL CSockClient::GoToServer(LPREQUEST lpRequest)
{
	CONTEXT_HEAD context;
	ZeroMemory(&context ,sizeof(context));
	
	int nLen=lpRequest->nDataLen;
	
	PBYTE pData = new BYTE[nLen];
	ZeroMemory(pData, nLen);
	memcpy(pData,lpRequest->pDataPtr,nLen);
    g_pSockServer->PutToServer(&context, lpRequest->head.nRequest, pData, nLen);
	
	return TRUE;
	
}

BOOL CSockClient::OnUserPayResult(LPREQUEST lpRequest)
{
	/*
	//ת����SockServer����
	if (lpRequest)
	{
		GoToServer(lpRequest);
		return TRUE;
	}
	*/
	
	/*
	//ֱ����CSockClient����
	LPPAY_RESULT lpPayResult = LPPAY_RESULT(PBYTE(lpRequest->pDataPtr));
	*/

	return TRUE;
}
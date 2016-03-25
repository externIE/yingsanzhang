#include "roombasic.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 

BOOL CBaseRoomServer::OnConnectSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	CAutoLock lock(&g_csTokenSock);
	g_mapTokenSock.SetAt(lpContext->lTokenID, lpContext->hSocket);

	UwlLogFile(_T("gamesvr connect"));
	return TRUE;
}

BOOL CBaseRoomServer::OnCloseSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
 	CAutoLock lock(&g_csTokenSock);
	SOCKET sock = 0;
	if(g_mapTokenSock.Lookup(lpContext->lTokenID, sock)){
		if(sock == lpContext->hSocket){
			g_mapTokenSock.RemoveKey(lpContext->lTokenID);
		}
	}	
 	UwlLogFile(_T("gamesvr close"));



	return TRUE;
}


BOOL CBaseRoomServer::OnRequest(void* lpParam1, void* lpParam2)
{try{
	LPCONTEXT_HEAD	lpContext = LPCONTEXT_HEAD	(lpParam1);
	LPREQUEST		lpRequest = LPREQUEST		(lpParam2);

	UwlTrace(_T("----------------------start of request process-------------------"));
	
#if defined(_UWL_TRACE) | defined(UWL_TRACE)
	DWORD dwTimeStart = GetTickCount();
#else
	DWORD dwTimeStart = 0;
#endif
	CWorkerContext* pThreadCxt = reinterpret_cast<CWorkerContext*>(GetWorkerContext());

	assert(lpContext && lpRequest);
	UwlTrace(_T("socket = %ld requesting..."), lpContext->hSocket);
	switch(lpRequest->head.nRequest){
	// request from system connection
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
	// user request
 	case GR_SEND_PULSE:
		UwlTrace(_T("GR_SEND_PULSE requesting..."));
		OnSendPulse(lpContext, lpRequest, pThreadCxt);
		break;
	case G2R_REGISTER_GAMESVR:
		OnRegisterGameSvr(lpContext, lpRequest, pThreadCxt);
		break;
	case G2R_USER_READY:
		OnMatchUserReady(lpContext, lpRequest, pThreadCxt);
		break;
	case G2R_USER_LEAVE:
		OnMatchUserLeave(lpContext, lpRequest, pThreadCxt);
		break;
	case G2R_COMMIT_DIFF:
		OnCommitDiff(lpContext, lpRequest, pThreadCxt);
		break;
	case G2R_PLAYER_OK:
		OnMatchPlayerOK(lpContext, lpRequest, pThreadCxt);
		break;
	case G2R_ASK_RANKLIST:
		OnMatchPlayerAskRankList(lpContext, lpRequest, pThreadCxt);
		break;
	case G2R_ARRAGE_FAID:
		OnMatchArrageFaild(lpContext, lpRequest, pThreadCxt);
		break;
	case G2R_ASK_SIGNUP:
		OnMatchPlayerAskSignUp(lpContext, lpRequest, pThreadCxt);
		break;
	case G2R_ASK_GIVEUP:
		OnMatchPlayerAskGiveUp(lpContext, lpRequest, pThreadCxt);
		break;
 	case GR_ENTER_MATCH_TOCHECK:
 		OnEnterMatchToCheck(lpContext, lpRequest, pThreadCxt);
		break;
  	default:
// 		if(lpRequest->head.nRequest>=GR_GAME2ROOM_BEGIN &&	lpRequest->head.nRequest<=GR_GAME2ROOM_END)
// 			OnGame2Room(lpContext, lpRequest, pThreadCxt);
// 		else
		{
			UwlTrace(_T("unsupport requesting..."));
			OnUnsupported(lpContext, lpRequest, pThreadCxt);
		}
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
}
catch(...)
{
	LPREQUEST		lpReq  = LPREQUEST(lpParam2);
 	UwlLogFile("catch error!!! reqid=%ld,DataLen=%ld",lpReq->head.nRequest,lpReq->nDataLen );
 
}
	return TRUE;
}
BOOL CBaseRoomServer::OnSendPulse(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	return TRUE;
}
BOOL CBaseRoomServer::OnUnsupported(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest,
							CWorkerContext* pThreadCxt)
{
	UwlTrace(_T("unsupported request from socket = %d"), lpContext->hSocket);

	return TRUE;
}

BOOL CBaseRoomServer::PutToServer(LPCONTEXT_HEAD lpContext, UINT nRequest, void* pData, int nLen)
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



void* CBaseRoomServer::OnWorkerStart()
{
	_ConnectionPtr pDBConnect = NULL;
	HRESULT hr = 0;

	hr = ::CoInitialize(NULL);
	if(FAILED(hr)) return NULL;

 
	CWorkerContext* pThreadCxt = new CWorkerContext();
	pThreadCxt->pDBConnect = pDBConnect;
	return pThreadCxt;
}

void CBaseRoomServer::OnWorkerExit(void* pContext)
{
	CWorkerContext* pThreadCxt = (CWorkerContext*)(pContext);
	if(pThreadCxt)
	{
	 
		SAFE_DELETE(pThreadCxt);
	}
	::CoUninitialize();
}

BOOL CBaseRoomServer::OnRegisterGameSvr(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	int nRepeated = lpRequest->head.nRepeated;

  	LPREGISTER_GAMESVR lpRegisterGameSvr = LPREGISTER_GAMESVR(PBYTE(lpRequest->pDataPtr)
									+ nRepeated * sizeof(CONTEXT_HEAD));								 
	{
		long token=0;
	 	CAutoLock lock(&g_csGameSvrToken);
		UwlLogFile(_T("OnRegisterGameSvr gameid=%d,gameport=%d"),lpRegisterGameSvr->nGameID,lpRegisterGameSvr->nGamePort);
    	g_mapGameSvrToken.SetAt(lpRegisterGameSvr->nGamePort, lpContext->lTokenID);
	}

	int userID=0;
	CBaseRoomData* pData=NULL;
	POSITION pos=g_mapRoomDataEx.GetStartPosition();
	while(pos)
	{
		g_mapRoomDataEx.GetNextAssoc(pos,userID,pData);
		if (pData
			&&pData->GetGameID()==lpRegisterGameSvr->nGameID)
		{
			pData->AT_BreakOffMatch();
			pData->AT_ContinueMatch();
		}
	}
	return TRUE;
}

BOOL CBaseRoomServer::OnMatchUserReady(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
 	int nRepeated = lpRequest->head.nRepeated;
  	LPMATCHUSER_READY lpMatchUserReady = LPMATCHUSER_READY(PBYTE(lpRequest->pDataPtr)
									+ nRepeated * sizeof(CONTEXT_HEAD));
								 
 	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpMatchUserReady->nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 	
		lpRoomData->AT_OnMatchUserReady(lpMatchUserReady);
	}
	return TRUE;
}



BOOL CBaseRoomServer::OnMatchUserLeave(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
 	int nRepeated = lpRequest->head.nRepeated;
  	LPMATCHUSER_LEAVE lpMatchUserLeave = LPMATCHUSER_LEAVE(PBYTE(lpRequest->pDataPtr)
									+ nRepeated * sizeof(CONTEXT_HEAD));
								 
 	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpMatchUserLeave->nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 		lpRoomData->AT_OnMatchUserLeave(lpMatchUserLeave);
	}
	return TRUE;
}

BOOL CBaseRoomServer::OnMatchArrageFaild(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	int nRepeated = lpRequest->head.nRepeated;
	LPARRANGE_FAILD paf = LPARRANGE_FAILD(PBYTE(lpRequest->pDataPtr)
		+ nRepeated * sizeof(CONTEXT_HEAD));
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		
		if(!g_mapRoomDataEx.Lookup(paf->nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));

		lpRoomData->AT_OnArrageFaild(paf);
	}
	return TRUE;

}

BOOL CBaseRoomServer::OnMatchPlayerAskRankList(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	int nRepeated = lpRequest->head.nRepeated;
  	LPMATCH_PLAYER_OK pPlayerOk = LPMATCH_PLAYER_OK(PBYTE(lpRequest->pDataPtr)
									+ nRepeated * sizeof(CONTEXT_HEAD));
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		
		if(!g_mapRoomDataEx.Lookup(pPlayerOk->nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		lpRoomData->AT_SendUserRankList(pPlayerOk->nUserID);
	}
	return TRUE;
}

BOOL CBaseRoomServer::OnMatchPlayerAskSignUp(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	int nRepeated = lpRequest->head.nRepeated;
	LPMATCH_ASKSIGNUP pAskSignUp = LPMATCH_ASKSIGNUP(PBYTE(lpRequest->pDataPtr)
		+ nRepeated * sizeof(CONTEXT_HEAD));
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		
		if(!g_mapRoomDataEx.Lookup(pAskSignUp->nRoomID, lpRoomData)){
			return FALSE;
		}
	}


	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));


		lpRoomData->AT_SignUp(pAskSignUp);

	}

	return TRUE;
}

BOOL CBaseRoomServer::OnMatchPlayerAskGiveUp(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	int nRepeated = lpRequest->head.nRepeated;
	LPMATCH_ASKGIVEUP pAskGiveUp = LPMATCH_ASKGIVEUP(PBYTE(lpRequest->pDataPtr)
		+ nRepeated * sizeof(CONTEXT_HEAD));
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		
		if(!g_mapRoomDataEx.Lookup(pAskGiveUp->nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	{
		lpRoomData->AT_GiveUp(pAskGiveUp);
	}
	return TRUE;
}

BOOL  CBaseRoomServer::OnMatchPlayerOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	int nRepeated = lpRequest->head.nRepeated;
  	LPMATCH_PLAYER_OK pPlayerOk = LPMATCH_PLAYER_OK(PBYTE(lpRequest->pDataPtr)
									+ nRepeated * sizeof(CONTEXT_HEAD));
			 		 
 	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(pPlayerOk->nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 	 	lpRoomData->AT_OnMatchPlayerOK(pPlayerOk);
 
	}
	return TRUE;
}


BOOL CBaseRoomServer::OnCommitDiff(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
 	int nRepeated = lpRequest->head.nRepeated;
 
	LPCOMMIT_DIFF lpCommitDiff = LPCOMMIT_DIFF(PBYTE(lpRequest->pDataPtr) 
									+ nRepeated * sizeof(CONTEXT_HEAD));

 								 
 	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpCommitDiff->nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		lpRoomData->AT_OnCommitDiff(lpCommitDiff);
	
	}
	return TRUE;
}


BOOL CBaseRoomServer::OnEnterMatchToCheck(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	 
	int nRepeated = lpRequest->head.nRepeated;
	LPENTER_MATCH_TOCHECK  lpEnterMatchToCheck = LPENTER_MATCH_TOCHECK(PBYTE(lpRequest->pDataPtr)
									+ nRepeated * sizeof(CONTEXT_HEAD));

	LPCONTEXT_HEAD  lpch1=LPCONTEXT_HEAD(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		
		if(!g_mapRoomDataEx.Lookup(lpEnterMatchToCheck->nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
        /////////////////////×À×Ó/////////////////////////
		
		LPPLAYER pPlayer = NULL;
		lpRoomData->m_mapPlayer.Lookup(lpEnterMatchToCheck->nUserID, pPlayer);
		if(!pPlayer)
			return FALSE;

		if (strcmp(pPlayer->pe.szHardID,lpEnterMatchToCheck->szHardID)
			||pPlayer->pe.lTokenID!=lpEnterMatchToCheck->nRoomTokenID)
		{		
			int nLen =  sizeof(CONTEXT_HEAD)+sizeof(int);
			BYTE* pData = new BYTE[nLen];
			ZeroMemory(pData, nLen);
			memcpy(pData,lpch1,sizeof(CONTEXT_HEAD));
			memcpy(pData+sizeof(CONTEXT_HEAD),&(lpEnterMatchToCheck->nUserID),sizeof(int));
			
			REQUEST response;
			memset(&response, 0, sizeof(response));
			response.head.nRepeated = 1;
			response.pDataPtr = pData;
			response.nDataLen = nLen;
			response.head.nRequest =  GR_ENTER_MATCH_HARDIDMISMATCH;
			
			CONTEXT_HEAD context;
			memcpy(&context, lpch1, sizeof(context));
			context.bNeedEcho = FALSE;
			g_pRoomServer->SendRequest(lpContext->hSocket, lpContext, &response);
			UwlClearRequest(&response); 
			return TRUE;
		}
	}

	return g_pSockServer->TransmitRequest(lpContext,lpRequest);
}
 

BOOL CBaseRoomServer::OnGame2Room(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
	int nRepeated = lpRequest->head.nRepeated;
  	LPBYTE lpData = LPBYTE(PBYTE(lpRequest->pDataPtr)
									+ nRepeated * sizeof(CONTEXT_HEAD));
							
    int nRoomID= *((int*)lpData);

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
   	 	 
 		NotifyOneGameSvr(lpRequest->head.nRequest,lpData,lpRequest->nDataLen,lpRoomData->roomdata.nGamePort);
   		return TRUE;
	}
 
}
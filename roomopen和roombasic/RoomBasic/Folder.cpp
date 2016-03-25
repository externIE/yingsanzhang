#include "roombasic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL CBaseSockServer::OnConnectSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	return TRUE;
}

BOOL CBaseSockServer::OnSendPulse(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	return TRUE;
}

BOOL CBaseSockServer::OnCheckNetwork(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	response.head.nRequest = GR_CHECK_NETWORK;

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	return TRUE;
}

BOOL CBaseSockServer::OnGetClientCount(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	LONG lClientCount = m_lClientCount;
	response.head.nRequest = UR_FETCH_SUCCEEDED;
	response.pDataPtr = &lClientCount;
	response.nDataLen = sizeof(lClientCount);

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	return TRUE;
}


BOOL CBaseSockServer::OnGetPlayerCount(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	int nCount=0;
	int roomid = 0;
	CBaseRoomData* lpRoomData = NULL;
	CRoomDataExMap mapRoomDataEx;
	{
		CAutoLock lock(&g_csRoomData);	
		POSITION pos = g_mapRoomDataEx.GetStartPosition();
		while (pos)	{
			g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			mapRoomDataEx.SetAt(roomid, lpRoomData);
		}
	}
	POSITION pos = mapRoomDataEx.GetStartPosition();
	while (pos)	{
		mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
		if(lpRoomData){
			CAutoLock lock(&(lpRoomData->m_csRoomData));
			nCount+=lpRoomData->m_mapPlayer.GetCount();
		}
	}
 
	REQUEST response;
	memset(&response, 0, sizeof(response));
  	response.head.nRequest = UR_FETCH_SUCCEEDED;
	response.pDataPtr = &nCount;
	response.nDataLen = sizeof(nCount);

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	return TRUE;
}

BOOL CBaseSockServer::OnGetTokenCount(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	int nCount=0;
	int roomid = 0;
	CBaseRoomData* lpRoomData = NULL;
	CRoomDataExMap mapRoomDataEx;
	{
		CAutoLock lock(&g_csRoomData);	
		POSITION pos = g_mapRoomDataEx.GetStartPosition();
		while (pos)	{
			g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			mapRoomDataEx.SetAt(roomid, lpRoomData);
		}
	}
	POSITION pos = mapRoomDataEx.GetStartPosition();
	while (pos)	{
		mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
		if(lpRoomData){
			CAutoLock lock(&(lpRoomData->m_csRoomData));
			nCount+=lpRoomData->m_mapTokenUserSock.GetCount();
		}
	}
 
	REQUEST response;
	memset(&response, 0, sizeof(response));
  	response.head.nRequest = UR_FETCH_SUCCEEDED;
	response.pDataPtr = &nCount;
	response.nDataLen = sizeof(nCount);

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	return TRUE;
}
BOOL CBaseSockServer::OnGetClientID(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	response.head.nRequest = UR_FETCH_SUCCEEDED;
	response.pDataPtr = &g_nClientID;
	response.nDataLen = sizeof(g_nClientID);

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	return TRUE;
}

BOOL CBaseSockServer::OnGetRoomSvrConfig(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	ROOMSVR_CONFIG roomsvrconfig;
	memset(&roomsvrconfig,0,sizeof(ROOMSVR_CONFIG));
 
	GetPrivateProfileString(_T("CheckServer"),_T("Name"),_T(""),roomsvrconfig.szCheckSvrName,sizeof(roomsvrconfig.szCheckSvrName) ,	g_szIniFile);

	{
		CAutoLock lock(&g_csServerPulseInfo);
		roomsvrconfig.nReconnectCount=g_ServerPulseInfo.nReconnectCount;
		roomsvrconfig.nSvrPulseLatest=g_ServerPulseInfo.nLatestTime;

	}
	response.head.nRequest = UR_FETCH_SUCCEEDED;
	response.pDataPtr = &roomsvrconfig;
	response.nDataLen = sizeof(roomsvrconfig);

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	return TRUE;
}
BOOL CBaseSockServer::OnGetMemoryStatus(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));

	MEMORYSTATUS ms;
	memset(&ms,0,sizeof(MEMORYSTATUS));
	ms.dwLength =sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&ms);
  
	response.head.nRequest = GR_GET_MEMORYSTATUS;
	response.pDataPtr = &ms;
	response.nDataLen = sizeof(MEMORYSTATUS);

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
	return TRUE;
}

BOOL CBaseSockServer::OnGetNoBufferCount(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
   	LPGET_NOBUFFER_COUNT lpgnbc = LPGET_NOBUFFER_COUNT(PBYTE(lpRequest->pDataPtr));
    if(lpgnbc->bSetZero)
		lpgnbc->nNoBufferCount = InterlockedExchange(&g_lNoBufferCount, 0);
	else
		lpgnbc->nNoBufferCount = InterlockedExchangeAdd(&g_lNoBufferCount, 0);
	
 
	lpgnbc->nNoBufferLatest=InterlockedExchangeAdd(&g_lNoBufferLatest, 0);

	int nData[2];
    nData[0]=lpgnbc->nNoBufferCount;
	nData[1]=lpgnbc->nNoBufferLatest;

	REQUEST response;
	memset(&response, 0, sizeof(response));
 	response.head.nRequest = GR_GET_NOBUFFER_COUNT;
	response.pDataPtr =  nData ;
	response.nDataLen = sizeof(int)*2;

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
	return TRUE;
}
BOOL CBaseSockServer::OnGetLogfileSize(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{ 
	DWORD dwFileSize=0;
   	LPGET_REMOTE_FILESIZE lpgrfs = LPGET_REMOTE_FILESIZE(PBYTE(lpRequest->pDataPtr));
   	TCHAR szFullName[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), szFullName, sizeof(szFullName));
 	UwlSplitPath(szFullName, SPLIT_DRIVE_DIR, szFullName);
	lstrcat(szFullName,lpgrfs->szFileName);
   	HANDLE hFile = CreateFile(szFullName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		dwFileSize=GetFileSize(hFile,NULL);
 		CloseHandle(hFile);
	}
 	
 	REQUEST response;
	memset(&response, 0, sizeof(response));

	response.head.nRequest = UR_FETCH_SUCCEEDED;
	response.pDataPtr = &dwFileSize;
	response.nDataLen = sizeof(DWORD);

	BOOL bSendOK = FALSE;
 	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
 	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
  	return TRUE;
}
BOOL CBaseSockServer::OnGetProfile(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	CString  sContent;
 	HANDLE hFile=CreateFile( g_szIniFile, 
					  GENERIC_WRITE|GENERIC_READ,
		              FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
					  OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile!=INVALID_HANDLE_VALUE)
    {
		DWORD dwRead;
		DWORD dwFileSize=GetFileSize(hFile,NULL);
 		SetFilePointer(hFile,0,NULL,FILE_BEGIN);
		ReadFile(hFile,(LPVOID)sContent.GetBuffer(dwFileSize),dwFileSize,&dwRead,NULL);
		sContent.ReleaseBuffer();
 		CloseHandle(hFile);
	}

	REQUEST response;
	memset(&response, 0, sizeof(response));

   
	response.head.nRequest = UR_FETCH_SUCCEEDED;
	response.pDataPtr = (LPVOID)(LPCTSTR)sContent;
	response.nDataLen = sContent.GetLength();

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	return TRUE;
}
 

BOOL CBaseSockServer::OnTransmitNotify(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
  	TransmitRequest(lpContext, lpRequest);
 	return TRUE;
}


BOOL CBaseSockServer::OnGetRoomCapacity(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
 	LPROOM_CAPACITY lprc = LPROOM_CAPACITY(PBYTE(lpRequest->pDataPtr));
 
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
 	    g_mapRoomDataEx.Lookup(lprc->nRoomID, lpRoomData);
	}
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));

		lprc->nPlayerCount =lpRoomData->m_mapPlayer.GetCount();
		lprc->nTokenUserCount =lpRoomData->m_mapTokenUserSock.GetCount();
		lprc->nUserSockCount =lpRoomData->m_mapUserSock.GetCount();
	}

 
	response.head.nRequest = UR_FETCH_SUCCEEDED;
	response.pDataPtr = lprc;
	response.nDataLen = sizeof(ROOM_CAPACITY);

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	return TRUE;
}



BOOL CBaseSockServer::OnCleanStales(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	LPCLEAN_STALES lpcs = LPCLEAN_STALES(PBYTE(lpRequest->pDataPtr));

 	CDWordArray aryToken;
  	int stales = g_pSockServer->CleanStales(lpcs->nDeadSeconds , aryToken); 
 	UwlLogFile(_T("===========CleanStales,AdminID=%d,DeadSeconds=%d,Count=%d"),lpcs->nAdminID,lpcs->nDeadSeconds,stales);
	 
	response.head.nRequest = UR_OPERATE_SUCCEEDED;

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);

	return TRUE;

}


BOOL CBaseSockServer::OnGetPlayers(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	REQUEST response;
	memset(&response, 0, sizeof(response));

	LPGET_PLAYERS lpgp= LPGET_PLAYERS(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
 		if(!g_mapRoomDataEx.Lookup(lpgp->nRoomID, lpRoomData))
		{	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
 			goto SendResponse;
		}
	}


	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 
		LPPLAYER pPlayer = NULL;
 		if(!lpRoomData->m_mapPlayer.Lookup(lpgp->nUserID, pPlayer))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}

		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}

		if(!IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
		if(IS_BIT_SET(lpgp->dwGetFlag ,FLAG_GETPLAYER_EXTEND))
		{
			if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ADMIN))//不是网管
			{
				response.head.nRequest = GR_NO_PERMISSION;
				goto SendResponse;
			}
			if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_SYSADMIN))//不是系统管理员
			{
				response.head.nRequest = GR_NO_PERMISSION;
				goto SendResponse;
			}
		}
		PBYTE pData=NULL;
		int nLen=0;

		if(IS_BIT_SET(lpgp->dwGetFlag ,FLAG_GETPLAYER_PRIVINFO))
		{
			nLen= lpRoomData->m_mapPlayer.GetCount() * sizeof(PLAYER_PRIVINFO);
			pData= new BYTE[nLen];
			PLAYER_PRIVINFO* ppp=(PLAYER_PRIVINFO*)pData;

			int nUserID;
			PLAYER* lpFind;
 			POSITION pos;
			pos  =  lpRoomData->m_mapPlayer.GetStartPosition();
			while (pos)	
			{
				lpRoomData->m_mapPlayer.GetNextAssoc(pos, nUserID, lpFind);
				lpRoomData->CopyPlayerPrivInfo(ppp,lpFind);
				ppp++;
			}
		}
		else if(IS_BIT_SET(lpgp->dwGetFlag ,FLAG_GETPLAYER_EXTEND))
		{

			nLen= lpRoomData->m_mapPlayer.GetCount() * sizeof(PLAYER_EXTEND);
			pData= new BYTE[nLen];
			PLAYER_EXTEND* ppe=(PLAYER_EXTEND*)pData;

			int nUserID;
			PLAYER* lpFind;
 			POSITION pos;
			pos  =  lpRoomData->m_mapPlayer.GetStartPosition();
			while (pos)	
			{
				lpRoomData->m_mapPlayer.GetNextAssoc(pos, nUserID, lpFind);
				memcpy(ppe,&lpFind->pe,sizeof(PLAYER_EXTEND));
				ppe->nUserID=nUserID;
				ppe++;
			}

		}
 		response.pDataPtr = pData;
		response.nDataLen = nLen;
		response.head.nRequest = UR_FETCH_SUCCEEDED;
	}
 
SendResponse:
	BOOL bSendOK = FALSE;
 	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
    bSendOK = SendResponse(context.hSocket, &context, &response,MSG_RESPONSE,0,COMPRESS_ZIP);
 	UwlClearRequest(&response);
	return TRUE;
}




BOOL CBaseSockServer::OnGetOnePlayer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	REQUEST response;
	memset(&response, 0, sizeof(response));

	LPGET_ONEPLAYER lpgop= LPGET_ONEPLAYER(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
 		if(!g_mapRoomDataEx.Lookup(lpgop->nRoomID, lpRoomData))
		{	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
 			goto SendResponse;
		}
	}


	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 
		LPPLAYER pPlayer = NULL;
 		if(!lpRoomData->m_mapPlayer.Lookup(lpgop->nUserID, pPlayer))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;	// 用户没有进入房间
			goto SendResponse;
		}

		if(pPlayer->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}

		if(!IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
		if(IS_BIT_SET(lpgop->dwGetFlag ,FLAG_GETPLAYER_EXTEND))
		{
			if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_ADMIN))//不是网管
			{
				response.head.nRequest = GR_NO_PERMISSION;
				goto SendResponse;
			}
			if(!IS_BIT_SET(pPlayer->nUserType ,USER_TYPE_SYSADMIN))//不是系统管理员
			{
				response.head.nRequest = GR_NO_PERMISSION;
				goto SendResponse;
			}
		}

		LPPLAYER lpFind = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpgop->nPlayer, lpFind))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}

 

		PBYTE pData=NULL;
		int nLen=0;

		if(IS_BIT_SET(lpgop->dwGetFlag ,FLAG_GETPLAYER_PRIVINFO))
		{
			nLen= sizeof(PLAYER_PRIVINFO);
			pData= new BYTE[nLen];
	    	lpRoomData->CopyPlayerPrivInfo((PLAYER_PRIVINFO*)pData,lpFind);
		}
		else if(IS_BIT_SET(lpgop->dwGetFlag ,FLAG_GETPLAYER_EXTEND))
		{
			nLen=  sizeof(PLAYER_EXTEND);
			pData= new BYTE[nLen];
	 		memcpy(pData,&lpFind->pe,sizeof(PLAYER_EXTEND));
		}
 		response.pDataPtr = pData;
		response.nDataLen = nLen;
		response.head.nRequest = UR_FETCH_SUCCEEDED;
	}
 
SendResponse:
	BOOL bSendOK = FALSE;
 	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
    bSendOK = SendResponse(context.hSocket, &context, &response);
 	UwlClearRequest(&response);
	return TRUE;
}
BOOL CBaseSockServer::OnGetClientInfo(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	REQUEST response;
	memset(&response, 0, sizeof(response));

	LPGET_CLIENTINFO lpgci= LPGET_CLIENTINFO(PBYTE(lpRequest->pDataPtr));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
 		if(!g_mapRoomDataEx.Lookup(lpgci->nRoomID, lpRoomData))
		{	
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
 			goto SendResponse;
		}
	}
 
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 
		LPPLAYER pAdmin = NULL;
 		if(!lpRoomData->m_mapPlayer.Lookup(lpgci->nAdminID, pAdmin))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}

		if(pAdmin->pe.lTokenID!= lpContext->lTokenID)
		{
			response.head.nRequest = GR_TOKENID_MISMATCH;
			goto SendResponse;
		}

		if(!IS_BIT_SET(pAdmin->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
	 
		if(!IS_BIT_SET(pAdmin->nUserType ,USER_TYPE_ADMIN))//不是网管
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
		if(!IS_BIT_SET(pAdmin->nUserType ,USER_TYPE_SYSADMIN))//不是系统管理员
		{
			response.head.nRequest = GR_NO_PERMISSION;
			goto SendResponse;
		}
	 

		LPPLAYER lpFind = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(lpgci->nPlayer, lpFind))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}

		CClientInfo* pClientInfo=NULL;
		lpRoomData->m_mapClientInfo.Lookup( lpgci->nPlayer,pClientInfo);
		if(pClientInfo==NULL)
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			goto SendResponse;
		}

		int nLen=pClientInfo->GetClientInfoSize();
		BYTE * pData=new BYTE[nLen];
		memset(pData,0,nLen);
		
		pClientInfo->CombClientInfo(pData);
		 
 		response.pDataPtr = pData;
		response.nDataLen = nLen;
		response.head.nRequest = UR_FETCH_SUCCEEDED;
	}
 
SendResponse:
	BOOL bSendOK = FALSE;
 	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
    bSendOK = SendResponse(context.hSocket, &context, &response);
 	UwlClearRequest(&response);
	return TRUE;
}
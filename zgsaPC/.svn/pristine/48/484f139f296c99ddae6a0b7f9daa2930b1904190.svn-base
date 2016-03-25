#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CString CSockServer::GetProfileContent() 
{
	CString  sRet;
	HANDLE hFile=CreateFile( g_szIniFile, 
		GENERIC_WRITE|GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile!=INVALID_HANDLE_VALUE)
    {
		DWORD dwRead;
		DWORD dwFileSize=GetFileSize(hFile,NULL);
		SetFilePointer(hFile,0,NULL,FILE_BEGIN);
		ReadFile(hFile,(LPVOID)sRet.GetBuffer(dwFileSize),dwFileSize,&dwRead,NULL);
		sRet.ReleaseBuffer();
		CloseHandle(hFile);
	}
	return sRet;
}

/*
void CSockServer::FillPlayerByEnterRoomEx( PLAYER* lpPlayer,ENTER_ROOM_EX * lpEnterRoomEx)
{
	lpPlayer->nUserType = lpEnterRoomEx->nUserType;
	lpPlayer->nClothingID = lpEnterRoomEx->nClothingID;
	lpPlayer->nPortrait = lpEnterRoomEx->nPortrait;
	lpPlayer->nNickSex = lpEnterRoomEx->nNickSex;
	
	lstrcpy(lpPlayer->szUsername, lpEnterRoomEx->szUsername);
	lstrcpy(lpPlayer->szNickName, lpEnterRoomEx->szNickName);
	
	lpPlayer->pe.nUserID =lpPlayer->nUserID ;
	lpPlayer->pe.nExpiration = lpEnterRoomEx->nExpiration;
	lpPlayer->pe.nPlayRoom = lpEnterRoomEx->nPlayRoom;
	lpPlayer->pe.nRegFrom = lpEnterRoomEx->nRegFrom;
	lpPlayer->pe.nHaveBind=lpEnterRoomEx->nHaveBind;
	lpPlayer->pe.nOnRegMachine=lpEnterRoomEx->nOnRegMachine;
	lpPlayer->pe.nLogonSvrID=lpEnterRoomEx->nLogonSvrID;
	lpPlayer->pe.nHallBuildNO=lpEnterRoomEx->nHallBuildNO;
	lpPlayer->pe.nHallRunCount=lpEnterRoomEx->nHallRunCount;
	lpPlayer->pe.nAgentGroupID = lpEnterRoomEx->nAgentGroupID;
	lpPlayer->pe.nDownloadGroup = lpEnterRoomEx->nDownloadGroup;
	lpPlayer->pe.nHallNetDelay = lpEnterRoomEx->nHallNetDelay;
	lpPlayer->pe.nEnterTime=lpEnterRoomEx->er.nEnterTime;
	
	lpPlayer->pe.dwSysVer=lpEnterRoomEx->dwSysVer;
	lpPlayer->pe.dwScreenXY=lpEnterRoomEx->er.dwScreenXY;
	lpPlayer->pe.dwEnterFlags=lpEnterRoomEx->er.dwEnterFlags;
	lpPlayer->pe.dwClientPort=lpEnterRoomEx->er.dwClientPort;
	lpPlayer->pe.dwServerPort=lpEnterRoomEx->er.dwServerPort;
	
	lpPlayer->pe.dwIPAddr =lpEnterRoomEx->er.dwIPAddr;
	lpPlayer->pe.dwClientSockIP=lpEnterRoomEx->er.dwClientSockIP;
	lpPlayer->pe.dwRemoteSockIP=lpEnterRoomEx->er.dwRemoteSockIP;
	lpPlayer->pe.dwClientLANIP =lpEnterRoomEx->er.dwClientLANIP;
	lpPlayer->pe.dwClientMask=lpEnterRoomEx->er.dwClientMask;
	lpPlayer->pe.dwClientGateway=lpEnterRoomEx->er.dwClientGateway;
	lpPlayer->pe.dwClientDNS=lpEnterRoomEx->er.dwClientDNS;
	lpPlayer->pe.dwPixelsXY =lpEnterRoomEx->er.dwPixelsXY;
	lpPlayer->pe.dwClientFlags =lpEnterRoomEx->er.dwClientFlags;
	
	lpPlayer->pe.dwPwdCode=lpEnterRoomEx->dwPwdCode;
	lpPlayer->pe.nBirthday=lpEnterRoomEx->nBirthday;
	
	memcpy(lpPlayer->pe.szUniqueID,lpEnterRoomEx->er.szUniqueID,sizeof(lpPlayer->pe.szUniqueID));
	memcpy(lpPlayer->pe.szHardID,lpEnterRoomEx->er.szHardID,sizeof(lpPlayer->pe.szHardID));
	memcpy(lpPlayer->pe.szVolumeID,lpEnterRoomEx->er.szVolumeID,sizeof(lpPlayer->pe.szVolumeID));
	memcpy(lpPlayer->pe.szMachineID,lpEnterRoomEx->er.szMachineID,sizeof(lpPlayer->pe.szMachineID));
	memcpy(lpPlayer->pe.szPhysAddr,lpEnterRoomEx->er.szPhysAddr,sizeof(lpPlayer->pe.szPhysAddr));
	
}
*/




BOOL CSockServer::OnConnectSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	CAutoLock lock(&g_csTokenSock);
	g_mapTokenSock.SetAt(lpContext->lTokenID, lpContext->hSocket);

	try{
		DWORD dwIP = GetClientAddress(lpContext->hSocket, lpContext->lTokenID);
		TCHAR szIP[MAX_SERVERIP_LEN];
		ZeroMemory(szIP, sizeof(szIP));
		UwlAddrToName(dwIP, szIP);
		UwlLogFile(_T("client connected. ip: %s"), szIP);
	}catch(...){
		UwlLogFile(_T("try to get connected ip failed!"));
	}
	return TRUE;
}

BOOL CSockServer::OnCloseSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	CAutoLock lock(&g_csTokenSock);
	SOCKET sock = 0;
	if(g_mapTokenSock.Lookup(lpContext->lTokenID, sock)){
		if(sock == lpContext->hSocket){
			g_mapTokenSock.RemoveKey(lpContext->lTokenID);
		}
	}	
	return TRUE;
}

BOOL CSockServer::OnSendPulse(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	return TRUE;
}

BOOL CSockServer::OnValidateClientEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	_ConnectionPtr pDBConnect = NULL;
	if(pThreadCxt){
		pDBConnect = pThreadCxt->m_pDBConnectMain;
	}
	if(NULL == pDBConnect){
		return TRUE;
	}


   	LPVALIDATE_CLIENT_EX lpvce = LPVALIDATE_CLIENT_EX(PBYTE(lpRequest->pDataPtr));
    lpvce->dwIPAddr = GetClientAddress(lpContext->hSocket, lpContext->lTokenID);	

	TCHAR szIP[MAX_SERVERIP_LEN];
 	ZeroMemory(szIP,sizeof(szIP));
  	UwlAddrToName(lpvce->dwIPAddr,szIP);

 	REQUEST response;
	memset(&response, 0, sizeof(response));
	LONG errcode = 0;
   	int nResult = DB_ValidateClientEx(errcode, pDBConnect, response.head.nRequest, lpvce);

#ifdef _RS125
	response.head.nRequest=UR_FETCH_SUCCEEDED;
#endif
 
	BOOL bSendOK = FALSE;
 	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
 	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
 	return TRUE;	
}


BOOL CSockServer::OnValidateGameSvr(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	DWORD dwIP = GetClientAddress(lpContext->hSocket, lpContext->lTokenID);

	int nRepeated = lpRequest->head.nRepeated;

	LPVALIDATE_GAMESVR lpvg = LPVALIDATE_GAMESVR(PBYTE(lpRequest->pDataPtr) 
									+ nRepeated * sizeof(CONTEXT_HEAD));
	{
		if(CLIENT_TYPE_GAME == lpvg->nClientType)
		{
			CAutoLock lock(&g_csClientTokenG);


			CClientTokenMap* pClientTokenMap = NULL;
			if(!g_mapClientTokenG.Lookup(lpvg->nGameID, pClientTokenMap))
			{
				pClientTokenMap = new CClientTokenMap;
				g_mapClientTokenG.SetAt(lpvg->nGameID, pClientTokenMap);
			}
			pClientTokenMap->SetAt(dwIP, lpContext->lTokenID);
			////////////////////////////////////////////////////

			if(lpvg->nClientID>0 && lpvg->nGamePort >0 &&  lpvg->nGamePort< 65535 )
			{
				CString sKey;
				sKey.Format(_T("%d|%d"),lpvg->nClientID,lpvg->nGamePort);
				g_mapGameSvrToken.SetAt(sKey,lpContext->lTokenID);
			}
			///////////////////////////////////////////////////
		}
	}
	return TRUE;
}


BOOL CSockServer::OnGetClientCount(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
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

BOOL CSockServer::OnGetPropInfo(LPCONTEXT_HEAD lpContext, LPREQUEST lpReq, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRequest = lpReq->head.nRequest;
	response.head.nSubReq  = UR_OPERATE_FAILED;
	response.head.nRepeated = 1;
 
	int  nResult = 0;
	LONG errcode = 0;
	UINT nResponse = 0;
	int nTransStarted = 0;

	int nRepeated = lpReq->head.nRepeated;
	LPGET_PROP_INFO lpProp = LPGET_PROP_INFO(PBYTE(lpReq->pDataPtr) + nRepeated * sizeof(CONTEXT_HEAD));
	int nUserID  = lpProp->nUserID;		
	int nGameID  = lpProp->nGameID;	

	GET_PROPINFO_OK gpi;
	memset(&gpi,0,sizeof(gpi));
	gpi.nUserID = nUserID;
		
	if(nUserID <= 0 || nGameID<=0 || nGameID>=MAX_GAME_NUM ){			
		return SendOpeReqOnlyCxt(lpContext, nRepeated, lpReq->pDataPtr, response);
	}

	_ConnectionPtr pDBConnect = NULL;	
	if(pThreadCxt){		
		pDBConnect = pThreadCxt->m_pDBConnectGame;
	}
	if(NULL == pDBConnect){
		pThreadCxt->m_bReConnectGame=TRUE; 
		return SendOpeReqOnlyCxt(lpContext, nRepeated, lpReq->pDataPtr, response);
	}

	if(IsInPeriodOfProfile(_T("dblock"), DEF_DBLOCK_BEGIN, DEF_DBLOCK_END)){
		response.head.nSubReq = GR_SYSTEM_LOCKED;
		return SendOpeReqOnlyCxt(lpContext, nRepeated, lpReq->pDataPtr, response);
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	nResult = DB_TestConnect(errcode, pDBConnect, nResponse);
	if(nResult < 0 && DB_CONNECT_ERROR == errcode && UR_DATABASE_ERROR == nResponse){
		response.head.nSubReq=UR_DATABASE_ERROR;
		pThreadCxt->m_bReConnectGame = TRUE; 
		return SendOpeReqOnlyCxt(lpContext, nRepeated, lpReq->pDataPtr, response);
	}
	
	nResult = DB_GetPropInfo(errcode, pDBConnect, nResponse, nUserID, gpi);
	if (nResult <= 0){ 
		response.head.nSubReq=nResponse;
		return SendOpeReqOnlyCxt(lpContext, nRepeated, lpReq->pDataPtr, response);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// ope succeed

	response.head.nRequest = lpReq->head.nRequest; 
	response.head.nSubReq  = UR_FETCH_SUCCEEDED;

	int nLen = nRepeated*sizeof(CONTEXT_HEAD) + sizeof(gpi);
	PBYTE pData=NULL;
	pData = new BYTE[nLen];
	memset(pData,0,nLen);
	memcpy(pData, lpReq->pDataPtr, nRepeated*sizeof(CONTEXT_HEAD));
	memcpy(pData + nRepeated*sizeof(CONTEXT_HEAD), &gpi, sizeof(gpi));
	response.pDataPtr = pData;
	response.nDataLen = nLen;
	
	SendOpeRequest(lpContext, response);
	UwlClearRequest(&response);

 	return TRUE; 
}

BOOL CSockServer::OnUseLuckCard(LPCONTEXT_HEAD lpContext, LPREQUEST lpReq, CWorkerContext* pThreadCxt)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRequest = lpReq->head.nRequest;
	response.head.nSubReq  = UR_OPERATE_FAILED;
	response.head.nRepeated = 1;
	
	int  nResult = 0;
	LONG errcode = 0;
	int nTransStarted = 0;
	UINT nResponse = 0;
	
	int nRepeated = lpReq->head.nRepeated;
	LPUSE_LUCKCARD lpLuck = LPUSE_LUCKCARD(PBYTE(lpReq->pDataPtr) + nRepeated * sizeof(CONTEXT_HEAD));
	int nUserID  = lpLuck->nUserID;		
	int nGameID  = lpLuck->nGameID;	
	
	USE_LUCKCARD_OK ul;
	ZeroMemory(&ul, sizeof(ul));
	ul.nUserID = nUserID;
	
	if(nUserID <= 0 || nGameID<=0 || nGameID>=MAX_GAME_NUM ){			 
		return SendOpeReqOnlyCxt(lpContext, nRepeated, lpReq->pDataPtr, response);
	}
	
	_ConnectionPtr pDBConnect = NULL;	
	if(pThreadCxt){		
		pDBConnect = pThreadCxt->m_pDBConnectGame;
	}
	if(NULL == pDBConnect){
		pThreadCxt->m_bReConnectGame=TRUE; 
		return SendOpeReqOnlyCxt(lpContext, nRepeated, lpReq->pDataPtr, response);
	}

	if(IsInPeriodOfProfile(_T("dblock"), DEF_DBLOCK_BEGIN, DEF_DBLOCK_END)){
		response.head.nSubReq = GR_SYSTEM_LOCKED;
		return SendOpeReqOnlyCxt(lpContext, nRepeated, lpReq->pDataPtr, response);
	}
	
	//////////////////////////////////////////////////////////////////////////
	nResult = DB_TestConnect(errcode, pDBConnect, nResponse);
	if(nResult < 0 && DB_CONNECT_ERROR == errcode && UR_DATABASE_ERROR == nResponse)
	{
		pThreadCxt->m_bReConnectGame = TRUE;

		response.head.nSubReq = nResponse;
		return SendOpeReqOnlyCxt(lpContext, nRepeated, lpReq->pDataPtr, response);
	}

	//////////////////////////////////////////////////////////////////////////
	nTransStarted = DB_BeginTransEx(errcode, pDBConnect, nResponse);
	if (nTransStarted <= 0)
	{
		response.head.nSubReq = nResponse;
		return SendOpeReqOnlyCxt(lpContext, nRepeated, lpReq->pDataPtr, response);
	}

	nResult = DB_UseLuckyCard(errcode, pDBConnect, nResponse, nUserID, ul);
	if (nResult <= 0)
	{ 
		response.head.nSubReq = nResponse;
		SendOpeReqOnlyCxt(lpContext, nRepeated, lpReq->pDataPtr, response);

		DB_RollbackEx(errcode, pDBConnect, response.head.nRequest); //回滚db操作
		return FALSE;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// ope succeed

	response.head.nRequest = lpReq->head.nRequest; 
	response.head.nSubReq  = UR_FETCH_SUCCEEDED;
	
	int nDataLen = nRepeated*sizeof(CONTEXT_HEAD) + sizeof(ul);
	PBYTE pData=NULL;
	pData = new BYTE[nDataLen];
	ZeroMemory(pData,nDataLen);
	memcpy(pData, lpReq->pDataPtr, nRepeated*sizeof(CONTEXT_HEAD));
	memcpy(pData+nRepeated*sizeof(CONTEXT_HEAD), &ul, sizeof(ul));
	
	response.pDataPtr = pData;
	response.nDataLen = nDataLen;
	
	SendOpeRequest(lpContext, response);

	DB_CommitEx(errcode, pDBConnect, response.head.nRequest); //提交db操作
	
	UwlClearRequest(&response);

	NotifyLogServer(GR_USE_LUCKCARD, &ul, sizeof(ul));
	
 	return TRUE; 
}

#define CLIENT_TYPE_CHUNKLOG 18
BOOL CSockServer::OnValidateClient(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	int nRepeated = lpRequest->head.nRepeated;
	
	LPVALIDATE_CLIENT lpValidateClient = LPVALIDATE_CLIENT(PBYTE(lpRequest->pDataPtr) 
		+ nRepeated * sizeof(CONTEXT_HEAD));
	
	
	if(CLIENT_TYPE_ROOM == lpValidateClient->nClientType)
	{
		CAutoLock lock(&g_csRoomSvrToken);
		g_mapRoomSvrToken.SetAt(lpValidateClient->nClientID, lpContext->lTokenID);
	}
	else if (CLIENT_TYPE_ASSIT == lpValidateClient->nClientType)
	{
		CAutoLock lock(&g_csAssitSvrToken);
		g_mapAssitSvrToken.SetAt(lpValidateClient->nClientID, lpContext->lTokenID);
	}
	else if (CLIENT_TYPE_CHUNKLOG == lpValidateClient->nClientType)
	{
		CAutoLock lock(&g_csChunkLogToken);
		g_mapChunkLogToken.RemoveAll();
		g_mapChunkLogToken.SetAt(lpValidateClient->nClientID, lpContext->lTokenID);
	}
	
	return TRUE;
}

BOOL CSockServer::OnQueryPlayerLogon(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPQUERY_PLAYERLOGON lpQuery = LPQUERY_PLAYERLOGON(PBYTE(lpRequest->pDataPtr) + lpRequest->head.nRepeated * sizeof(CONTEXT_HEAD));
	
	TransmitRequest(lpContext, lpRequest, CalcSubIndexByUserID(lpQuery->nUserID));
 	return TRUE;
}

/* 
保险箱操作请求示例
BOOL CSockServer::OnTakeSafeDeposit(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPTAKE_SAFE_DEPOSIT lpTakeSafeDeposit = LPTAKE_SAFE_DEPOSIT(PBYTE(lpRequest->pDataPtr) 	+   sizeof(CONTEXT_HEAD));

  	TransmitRequest(lpContext, lpRequest, CalcKubIndexByGameID(lpTakeSafeDeposit->nGameID));
 	return TRUE;
}

BOOL CSockServer::OnTakeSafeDepositFromKub(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	int gamedbindex=0;
	BOOL bPureRepeat = FALSE;
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRepeated = 1;
	int nRepeated = lpRequest->head.nRepeated;
	int nResult = 0;
	LONG errcode = 0;
	int nTransStarted = 0;
	int nTransStartedEx = 0;
    __int64 totaltransfer =0;
	__int64 monthTotalTransfer = 0;
	TCHAR szError[256];
	ZeroMemory(szError, sizeof(szError));

	LPTAKE_SAFE_DEPOSIT lpTakeSafeDeposit = LPTAKE_SAFE_DEPOSIT(PBYTE(lpRequest->pDataPtr) + nRepeated * sizeof(CONTEXT_HEAD));
	
	int nGameID=lpTakeSafeDeposit->nGameID;
	int nRoomID=lpTakeSafeDeposit->nRoomID;
	int nUserID=lpTakeSafeDeposit->nUserID;

	_ConnectionPtr pDBConnect = NULL;
	_ConnectionPtr pDBConnectEx = NULL;
  
	if(nGameID<=0 || nGameID>=MAX_GAME_NUM || lpTakeSafeDeposit->nDeposit < 0)
	{
		response.head.nRequest = UR_OPERATE_FAILED;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	gamedbindex=CalcGameDBIndexByGameID(nGameID);

	//维护期间停止请求
	if(IsGameDBLock(gamedbindex) || IsMainDBLock())
	{
		response.head.nRequest = GR_SYSTEM_LOCKED;
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	if(pThreadCxt){
		pDBConnect = pThreadCxt->m_pDBConnectMain;		
		pDBConnectEx = pThreadCxt->m_pDBConnectGame[gamedbindex];
	}
	if(NULL == pDBConnect){
		pThreadCxt->m_bReConnectMain = TRUE; // !!! dont forget !!!
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(NULL == pDBConnectEx){
		pThreadCxt->m_bReConnectGame[gamedbindex]=TRUE; // !!! dont forget !!!
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(IsInPeriodOfProfile(_T("dblock"), DEF_DBLOCK_BEGIN, DEF_DBLOCK_END)){
		response.head.nRequest = GR_SYSTEM_LOCKED;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(lpTakeSafeDeposit->nPlayingGameID!=nGameID)//没有在玩
	{
		response.head.nRequest = GR_NEED_PLAYING;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(IsGameForbidInputDeposit(nGameID))
	{
		response.head.nRequest = GR_NODEPOSIT_GAME;
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	ROOMDATA roomdata;
	ZeroMemory(&roomdata, sizeof(roomdata));
	{
		CAutoLock lock(&g_csBaseData);
 		FindRoomDataByID(nRoomID,g_mapRoomData,&roomdata);
	}
   	if(roomdata.nRoomID==0)
	{
		response.head.nRequest = GR_ROOM_NOT_EXIST;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(!IS_BIT_SET(roomdata.dwConfigs, ROOM_CFG_TAKEDEPOSITINGAME))
	{
		response.head.nRequest = GR_NO_THIS_FUNCTION;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	//每日划银限制
	totaltransfer=(__int64)(lpTakeSafeDeposit->nTransferTotal) + (__int64)(lpTakeSafeDeposit->nDeposit);
 	if(totaltransfer > (__int64)(lpTakeSafeDeposit->nTransferLimit))
	{
		response.head.nRequest = GR_INPUTLIMIT_DAILY;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	//每月划银限制
	monthTotalTransfer=lpTakeSafeDeposit->llMonthTransferTotal + (__int64)(lpTakeSafeDeposit->nDeposit);
	if(monthTotalTransfer > lpTakeSafeDeposit->llMonthTransferLimit)
	{
		if (IS_BIT_SET(lpTakeSafeDeposit->dwFlags, FLAG_SUPPORT_MONTHLY_LIMIT))
		{
			response.head.nRequest = GR_INPUTLIMIT_MONTHLY;
		}
		else
		{
			response.head.nRequest = GR_ERROR_INFOMATION;
			LONGLONG llRemain = lpTakeSafeDeposit->llMonthTransferLimit-lpTakeSafeDeposit->llMonthTransferTotal;
			if (llRemain>0)
			{
				sprintf(szError,_T("系统限定每月划入游戏总额不能超过%I64d两，\r\n本月您最多还可以划入%I64d两。"),
					lpTakeSafeDeposit->llMonthTransferLimit,llRemain);
			}
			else
			{
				sprintf(szError,_T("系统限定每月划入游戏不能超过%I64d两，\r\n本月您已经超限，到下个月才能划银。"),
					lpTakeSafeDeposit->llMonthTransferLimit);
			}
		}

		bPureRepeat = TRUE;
		goto SendResponse;
	}
	////////////////////////////////////////////////////////////
 	nResult = DB_TestConnect(errcode, pDBConnectEx, response.head.nRequest);
	if(nResult < 0 && DB_CONNECT_ERROR == errcode && UR_DATABASE_ERROR == response.head.nRequest)
	{
		pThreadCxt->m_bReConnectGame[gamedbindex]= TRUE;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TCHAR szSecurePwd[MAX_PASSWORD_LEN];
	memset(szSecurePwd,0,sizeof(szSecurePwd));
	nResult = DB_GetSecurePwd(errcode, pDBConnect, response.head.nRequest, nUserID, szSecurePwd);
	if(nResult <= 0){
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(lstrcmpi(szSecurePwd, _T("")))// 保护密码不为空
	{ 
 		if (!IsSecurePwdVerified(nUserID, lpTakeSafeDeposit->dwIPAddr, lpTakeSafeDeposit->szHardID))//满足条件，免计算保护密码
		{
			///////////////////禁止连续验证///////////////////////
			{
				CAutoLock lock(&g_csSecurePwdError);
 				PASSWORD_WRONG pw;
				memset(&pw,0,sizeof(pw));
 				BOOL bFind=g_mapSecurePwdError.Lookup(nUserID,pw);
				if(bFind)
				{
 					int nNow=UwlGetCurrentSeconds();
		 			if(nNow-pw.nLatestTime>3600*4) //与上次验证间隔超过4小时　
					{
						g_mapSecurePwdError.RemoveKey(nUserID);
					}
					else
					{
						if(pw.nWrongCount>=10)
						{
							response.head.nRequest = GR_CONTINUE_PWDWRONG;
							bPureRepeat = TRUE;
							goto SendResponse;
						}
					}
				}
			}
 			nResult = DB_VerifyKeyResult(errcode, pDBConnect, response.head.nRequest, nUserID, lpTakeSafeDeposit->nKeyResult);
 		
			if( UR_INVALID_PARAM==response.head.nRequest)//保护密码错误的额外处理//
			{
				CAutoLock lock(&g_csSecurePwdError);
 				PASSWORD_WRONG pw;
				memset(&pw,0,sizeof(pw));
 				BOOL bFind=g_mapSecurePwdError.Lookup(nUserID,pw);
 				int nNow=UwlGetCurrentSeconds();
				if(bFind)
				{
  					if(nNow-pw.nLatestTime<300) //与上次验证间隔不到5分钟
					{
						pw.nWrongCount++;
						pw.nLatestTime=nNow;
					}
					else
					{
						pw.nWrongCount=1;
						pw.nLatestTime=nNow;
					}
				}
				else
				{
					pw.nLatestTime=nNow;
					pw.nWrongCount=1;
				}
				g_mapSecurePwdError.SetAt(nUserID,pw);
			}

			if (nResult==1)//记录成功验证
			{
 				CAutoLock lock(&g_csSecurePwdVerified);
				PASSWORD_VERIFIED pv;
				memset(&pv,0,sizeof(pv));
				int nCreateDate = 0;
				int nCreateTime = 0;
				UwlGetCurrentDateTime(nCreateDate, nCreateTime);
				pv.nCurrentDate = nCreateDate;
				pv.dwIPAddr = lpTakeSafeDeposit->dwIPAddr;
				lstrcpyn(pv.szHardID, lpTakeSafeDeposit->szHardID,MAX_HARDID_LEN_EX);
				g_mapSecurePwdVerified.SetAt(nUserID,pv);
			}
			else
			{
				//删除保护密码成功验证的记录
				CAutoLock lock(&g_csSecurePwdVerified);
				g_mapSecurePwdVerified.RemoveKey(nUserID);
			}
			
			if(nResult <= 0){
				bPureRepeat = TRUE;
				goto SendResponse;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	nTransStarted = DB_BeginTransEx(errcode, pDBConnect, response.head.nRequest);
	if(nTransStarted <= 0){
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	nTransStartedEx = DB_BeginTransEx(errcode, pDBConnectEx, response.head.nRequest);
	if(nTransStartedEx <= 0){
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	nResult = DB_MoveDepositFromSafeEx(errcode, pDBConnect, pDBConnectEx, response.head.nRequest, nUserID,nGameID,lpTakeSafeDeposit->nDeposit);
	if(nResult <= 0){
		bPureRepeat = TRUE;
		goto SendResponse;
	}

 	TAKE_SAFE_DEPOSIT_OK tsdo;
	ZeroMemory(&tsdo,sizeof(tsdo));
	tsdo.nUserID=nUserID;
	tsdo.nGameID=nGameID;
	tsdo.nRoomID=nRoomID;
	tsdo.nTableNO=lpTakeSafeDeposit->nTableNO;
	tsdo.nChairNO=lpTakeSafeDeposit->nChairNO;
	tsdo.nDeposit=lpTakeSafeDeposit->nDeposit;

	nResult=DB_FetchUserGameDeposit(errcode,pDBConnectEx,response.head.nRequest,tsdo.nUserID,tsdo.nGameID,tsdo.nGameDeposit);
	if(nResult <= 0){
		bPureRepeat = TRUE;
		goto SendResponse;
	}

  
	int nLen;
	nLen = nRepeated * sizeof(CONTEXT_HEAD) + sizeof(tsdo);
	PBYTE pData;
	pData = NULL;
	pData = new BYTE[nLen];

	memcpy(pData, lpRequest->pDataPtr, nRepeated * sizeof(CONTEXT_HEAD));
	memcpy(pData + nRepeated * sizeof(CONTEXT_HEAD), &tsdo, sizeof(tsdo));

	response.pDataPtr = pData;
	response.nDataLen = nLen;
	response.head.nRequest = GR_TAKE_SAFE_DEPOSIT_OK;

SendResponse:
	if(bPureRepeat){
		int nLen;
		nLen = nRepeated * sizeof(CONTEXT_HEAD);
		if(GR_INPUTLIMIT_DAILY==response.head.nRequest)
			nLen+=( sizeof(int)*2 );
		if (GR_INPUTLIMIT_MONTHLY==response.head.nRequest)
			nLen+=( sizeof(LONGLONG)*2 );
		if(GR_ERROR_INFOMATION==response.head.nRequest)
			nLen+=lstrlen(szError)+1;

		PBYTE pData;
		pData = NULL;
		if(nLen) 
		{
			pData = new BYTE[nLen];
			memcpy(pData, lpRequest->pDataPtr, nRepeated * sizeof(CONTEXT_HEAD));
			if(GR_INPUTLIMIT_DAILY==response.head.nRequest)
			{
 				memcpy(pData+nRepeated * sizeof(CONTEXT_HEAD),&lpTakeSafeDeposit->nTransferTotal ,sizeof(int));
				memcpy(pData+nRepeated * sizeof(CONTEXT_HEAD)+sizeof(int),&lpTakeSafeDeposit->nTransferLimit ,sizeof(int));
			}
			else if (GR_INPUTLIMIT_MONTHLY==response.head.nRequest)
			{
				memcpy(pData+nRepeated * sizeof(CONTEXT_HEAD),&lpTakeSafeDeposit->llMonthTransferTotal ,sizeof(LONGLONG));
				memcpy(pData+nRepeated * sizeof(CONTEXT_HEAD)+sizeof(LONGLONG),&lpTakeSafeDeposit->llMonthTransferLimit ,sizeof(LONGLONG));
			}
			else if(GR_ERROR_INFOMATION == response.head.nRequest)
				memcpy(pData+nRepeated * sizeof(CONTEXT_HEAD),szError,lstrlen(szError)+1);
		}
		response.pDataPtr = pData;
		response.nDataLen = nLen;
	}
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendRequest(lpContext->hSocket, &context, &response);

	if(GR_TAKE_SAFE_DEPOSIT_OK == response.head.nRequest){
		if(nTransStarted > 0){
			DB_CommitEx(errcode, pDBConnect, response.head.nRequest);
		}
		if(nTransStartedEx > 0){
			DB_CommitEx(errcode, pDBConnectEx, response.head.nRequest);
		}
	}else{
		if(nTransStarted > 0){
			DB_RollbackEx(errcode, pDBConnect, response.head.nRequest);
		}
		if(nTransStartedEx > 0){
			DB_RollbackEx(errcode, pDBConnectEx, response.head.nRequest);
		}
	}
	if(GR_TAKE_SAFE_DEPOSIT_OK == response.head.nRequest)
	{
		MOVE_SAFE_DEPOSIT msd;
		memset(&msd,0,sizeof(msd));
		msd.nUserID=nUserID;
		msd.nGameID=nGameID;
		msd.nRoomID=-1;//不通知roomsvr
		msd.nDeposit=lpTakeSafeDeposit->nDeposit;
		msd.nKeyResult=lpTakeSafeDeposit->nKeyResult;
		msd.nPlayingGameID=lpTakeSafeDeposit->nPlayingGameID;
		msd.dwIPAddr=lpTakeSafeDeposit->dwIPAddr;
		msd.nGameVID=lpTakeSafeDeposit->nGameVID;
 		msd.nTransferTotal=lpTakeSafeDeposit->nTransferTotal;
		msd.nTransferLimit=lpTakeSafeDeposit->nTransferLimit;
		lstrcpyn(msd.szHardID,lpTakeSafeDeposit->szHardID,MAX_HARDID_LEN);
  
 		NotifyServer(GR_MOVE_SAFE_DEPOSIT, &msd, sizeof(msd), CalcSubIndexByUserID(msd.nUserID));


		GAME_DEPOSIT_TRANSFER gdt;
		memset(&gdt,0,sizeof(gdt));
		gdt.nRequestID=GR_TAKE_SAFE_DEPOSIT;
		gdt.nUserID=nUserID;
		gdt.nFromGame=0;//保险箱
		gdt.nToGame=nGameID;
		gdt.nDeposit=lpTakeSafeDeposit->nDeposit;
 		NotifyServer(GR_GAME_DEPOSIT_TRANSFER, &gdt, sizeof(gdt), CalcKubIndexByGameID(gdt.nToGame));


		msd.nRoomID=nRoomID;
	 	int clientid = FindClientID(msd.nRoomID);
 		if(clientid) 
			NotifyOneRoomSvr(GR_MOVE_SAFE_DEPOSIT, &msd, sizeof(msd), clientid);


 
	}
	UwlClearRequest(&response);

	return TRUE;
}



BOOL CSockServer::OnSaveSafeDeposit(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPSAVE_SAFE_DEPOSIT lpSaveSafeDeposit = LPSAVE_SAFE_DEPOSIT(PBYTE(lpRequest->pDataPtr) 
									+   sizeof(CONTEXT_HEAD));

 	TransmitRequest(lpContext, lpRequest, CalcKubIndexByGameID(lpSaveSafeDeposit->nGameID));
	
	return TRUE;
}



BOOL CSockServer::OnSaveSafeDepositFromKub(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	int gamedbindex=0;
  
	TCHAR szError[256];
	ZeroMemory(szError, sizeof(szError));
	int nData[2];//存放返回数据
	ZeroMemory(nData,sizeof(nData));
	int nKeepDeposit=0;//游戏必须保留的银子数

 	BOOL bPureRepeat = FALSE;
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRepeated = 1;
	int nRepeated = lpRequest->head.nRepeated;
	int nResult = 0;
	LONG errcode = 0;
	int nTransStarted = 0;
 	int nTransStartedEx = 0;
	LPSAVE_SAFE_DEPOSIT lpSaveSafeDeposit = LPSAVE_SAFE_DEPOSIT(PBYTE(lpRequest->pDataPtr) + nRepeated * sizeof(CONTEXT_HEAD));
 	int nGameID=lpSaveSafeDeposit->nGameID;
	int nRoomID=lpSaveSafeDeposit->nRoomID;
	int nUserID=lpSaveSafeDeposit->nUserID;

	
	_ConnectionPtr pDBConnect = NULL;
	_ConnectionPtr pDBConnectEx = NULL;
  
	if(nGameID<=0 || nGameID>=MAX_GAME_NUM || lpSaveSafeDeposit->nDeposit < 0 )
	{
		response.head.nRequest = UR_OPERATE_FAILED;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	gamedbindex=CalcGameDBIndexByGameID(nGameID);
 
	//维护期间停止请求
	if(IsGameDBLock(gamedbindex) || IsMainDBLock())
	{
		response.head.nRequest = GR_SYSTEM_LOCKED;
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	if(pThreadCxt){
		pDBConnect = pThreadCxt->m_pDBConnectMain;
		pDBConnectEx = pThreadCxt->m_pDBConnectGame[gamedbindex];
	}
	if(NULL == pDBConnect){
		pThreadCxt->m_bReConnectMain = TRUE; // !!! dont forget !!!
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(NULL == pDBConnectEx){
		pThreadCxt->m_bReConnectGame[gamedbindex]=TRUE; // !!! dont forget !!!
		bPureRepeat = TRUE;
		goto SendResponse;
	}
 
	if(IsInPeriodOfProfile(_T("dblock"), DEF_DBLOCK_BEGIN, DEF_DBLOCK_END)){
		response.head.nRequest = GR_SYSTEM_LOCKED;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
  	if(lpSaveSafeDeposit->nPlayingGameID!=nGameID)//没有在玩
	{
		response.head.nRequest = GR_NEED_PLAYING;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
 
	if(IsGameLimitOutputDeposit(nGameID,nKeepDeposit))
	{
 		USER_GAME_INFO  usergameinfo;
		ZeroMemory(&usergameinfo,sizeof(usergameinfo));
		usergameinfo.nGameID=nGameID;
		usergameinfo.nUserID=nUserID;
		nResult = DB_FetchUserGameInfo(errcode, pDBConnectEx, response.head.nRequest, &usergameinfo);
		if(nResult!=1)
		{
			bPureRepeat = TRUE;
			goto SendResponse;
		}
		if(usergameinfo.nDeposit < lpSaveSafeDeposit->nDeposit )
		{
			bPureRepeat = TRUE;
			response.head.nRequest=GR_DEPOSIT_NOTENOUGH;
			goto SendResponse;
		}
		if(usergameinfo.nDeposit - lpSaveSafeDeposit->nDeposit < nKeepDeposit)//剩余银子 < 保留阀值
		{
			int nMinTimeCost= GetPrivateProfileInt(_T("output_mintimecost"),xyConvertIntToStr(lpSaveSafeDeposit->nGameID),3600,g_szIniFile);
			if(usergameinfo.nTimeCost<nMinTimeCost)
			{
				if(usergameinfo.nDeposit <= nKeepDeposit)
				{
 					response.head.nRequest=GR_TIMECOST_NOTENOUGH;
					nData[0]=nMinTimeCost/60 ;
				}
				else
				{
					if(IS_BIT_SET(lpSaveSafeDeposit->dwFlags,FLAG_SUPPORT_KEEPDEPOSIT))
					{
 						response.head.nRequest=GR_KEEPDEPOSIT_LIMIT;
						nData[0]= usergameinfo.nDeposit;
						nData[1]= nKeepDeposit;
					}
					else
					{
 						response.head.nRequest=GR_ERROR_INFOMATION;
						sprintf(szError,_T("很抱歉，由于您在初次进入游戏房间时获赠了银两,系统只允许你转出%d两。(至少保留%d两)"),usergameinfo.nDeposit-nKeepDeposit,nKeepDeposit);
					}
				}
		 			
				bPureRepeat = TRUE;
				goto SendResponse;
			}

			int nMinBout= GetPrivateProfileInt(_T("output_minbout"),xyConvertIntToStr(lpSaveSafeDeposit->nGameID),20,g_szIniFile);
			if(usergameinfo.nBout<nMinBout)
			{
				if(usergameinfo.nDeposit <= nKeepDeposit)
				{
					if (nMinBout>=1000000)
					{
						//设置1000000以上，表示根本不想玩家存到保险箱，此时不再提示局数不够
						response.head.nRequest=GR_ERROR_INFOMATION;
						sprintf(szError,_T("很抱歉，由于您在初次进入游戏房间时获赠了银两，这些银子无法转出。(至少保留%d两)"),nKeepDeposit);
					}
					else
					{
						response.head.nRequest=GR_BOUT_NOTENOUGH;
						nData[0]=nMinBout;
					}
				}
				else
				{

 					if(IS_BIT_SET(lpSaveSafeDeposit->dwFlags,FLAG_SUPPORT_KEEPDEPOSIT))
					{
 						response.head.nRequest=GR_KEEPDEPOSIT_LIMIT;
						nData[0]= usergameinfo.nDeposit;
						nData[1]= nKeepDeposit;
					}
					else
					{
 						response.head.nRequest=GR_ERROR_INFOMATION;
						sprintf(szError,_T("很抱歉，由于您在初次进入游戏房间时获赠了银两,系统只允许你转出%d两。(至少保留%d两)"),usergameinfo.nDeposit-nKeepDeposit,nKeepDeposit);
					}

				}
				bPureRepeat = TRUE;
				goto SendResponse;

			}
		}
	}

	ROOMDATA roomdata;
	ZeroMemory(&roomdata, sizeof(roomdata));
	{
		CAutoLock lock(&g_csBaseData);
 		FindRoomDataByID(nRoomID,g_mapRoomData,&roomdata);
	}
   	if(roomdata.nRoomID==0)
	{
		response.head.nRequest = GR_ROOM_NOT_EXIST;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(!IS_BIT_SET(roomdata.dwConfigs, ROOM_CFG_TAKEDEPOSITINGAME))
	{
		response.head.nRequest = GR_NO_THIS_FUNCTION;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
 	////////////////////////////////////////////////////////////
 	nResult = DB_TestConnect(errcode, pDBConnectEx, response.head.nRequest);
	if(nResult < 0 && DB_CONNECT_ERROR == errcode && UR_DATABASE_ERROR == response.head.nRequest)
	{
		pThreadCxt->m_bReConnectGame[gamedbindex]= TRUE;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
  
	///////////////////////////////////////////////////////////
	nTransStarted = DB_BeginTransEx(errcode, pDBConnect, response.head.nRequest);
	if(nTransStarted <= 0)	
	{
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	nTransStartedEx = DB_BeginTransEx(errcode, pDBConnectEx, response.head.nRequest);
	if(nTransStartedEx <= 0)
	{
		bPureRepeat = TRUE;
		goto SendResponse;
	}
 	nResult = DB_MoveDepositToSafeEx(errcode, pDBConnectEx, pDBConnect, response.head.nRequest, nUserID,nGameID,lpSaveSafeDeposit->nDeposit);
	if(nResult <= 0){
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	SAVE_SAFE_DEPOSIT_OK ssdo;
	ZeroMemory(&ssdo,sizeof(ssdo));
	ssdo.nUserID=nUserID;
	ssdo.nGameID=nGameID;
	ssdo.nRoomID=nRoomID;
	ssdo.nTableNO=lpSaveSafeDeposit->nTableNO;
	ssdo.nChairNO=lpSaveSafeDeposit->nChairNO;
	ssdo.nDeposit=lpSaveSafeDeposit->nDeposit;

	nResult=DB_FetchUserGameDeposit(errcode,pDBConnectEx,response.head.nRequest,ssdo.nUserID,ssdo.nGameID,ssdo.nGameDeposit);
	if(nResult <= 0){
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	int nLen;
	nLen = nRepeated * sizeof(CONTEXT_HEAD) + sizeof(ssdo);;
	PBYTE pData;
	pData = NULL;
	pData = new BYTE[nLen];

	memcpy(pData, lpRequest->pDataPtr, nRepeated * sizeof(CONTEXT_HEAD));
	memcpy(pData + nRepeated * sizeof(CONTEXT_HEAD), &ssdo, sizeof(ssdo));

	response.pDataPtr = pData;
	response.nDataLen = nLen;
	response.head.nRequest = GR_SAVE_SAFE_DEPOSIT_OK;

SendResponse:
	if(bPureRepeat){
		int nLen = nRepeated * sizeof(CONTEXT_HEAD);
	    if(GR_ERROR_INFOMATION ==response.head.nRequest  )
			nLen+=lstrlen(szError)+1;
		if(GR_BOUT_NOTENOUGH==response.head.nRequest || GR_TIMECOST_NOTENOUGH==response.head.nRequest)
			nLen+=sizeof(int);
		if(GR_KEEPDEPOSIT_LIMIT==response.head.nRequest)
			nLen+=( sizeof(int)*2 );
		PBYTE pData;
		pData = NULL;
		if(nLen)
		{
			pData = new BYTE[nLen];
			memcpy(pData, lpRequest->pDataPtr, nRepeated * sizeof(CONTEXT_HEAD));
			if(GR_ERROR_INFOMATION ==response.head.nRequest )
				memcpy(pData+nRepeated * sizeof(CONTEXT_HEAD),szError,lstrlen(szError)+1);
			if(GR_BOUT_NOTENOUGH==response.head.nRequest || GR_TIMECOST_NOTENOUGH==response.head.nRequest)
				memcpy(pData+nRepeated * sizeof(CONTEXT_HEAD),nData,sizeof(int));
			if(GR_KEEPDEPOSIT_LIMIT==response.head.nRequest )
				memcpy(pData+nRepeated * sizeof(CONTEXT_HEAD),nData,sizeof(int)*2);
		}
		response.pDataPtr = pData;
		response.nDataLen = nLen;
	}
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendRequest(lpContext->hSocket, &context, &response);

	if(GR_SAVE_SAFE_DEPOSIT_OK == response.head.nRequest){
		if(nTransStarted > 0){
			DB_CommitEx(errcode, pDBConnect, response.head.nRequest);
		}
		if(nTransStartedEx > 0){
			DB_CommitEx(errcode, pDBConnectEx, response.head.nRequest);
		}
	 
	}else{
		if(nTransStarted > 0){
			DB_RollbackEx(errcode, pDBConnect, response.head.nRequest);
		}
		if(nTransStartedEx > 0){
			DB_RollbackEx(errcode, pDBConnectEx, response.head.nRequest);
		}
	 
	}
	if(GR_SAVE_SAFE_DEPOSIT_OK == response.head.nRequest)
	{
		TRANSFER_DEPOSIT td;
		memset(&td,0,sizeof(td));

 		td.nUserID=nUserID;
		td.nFromGame=nGameID;
		td.nToGame=0;
 		td.nFromRoom=-1;							
		td.nToRoom=0;
		td.nDeposit=lpSaveSafeDeposit->nDeposit; 
		td.nPlayingGameID=lpSaveSafeDeposit->nPlayingGameID;
		td.nVerifyGame=0;
 		td.dwIPAddr=lpSaveSafeDeposit->dwIPAddr;
		td.nGameVID=lpSaveSafeDeposit->nGameVID;
		td.nTransferTotal=lpSaveSafeDeposit->nTransferTotal;
		td.nTransferLimit=lpSaveSafeDeposit->nTransferTotal;
  		lstrcpyn(td.szHardID,lpSaveSafeDeposit->szHardID,MAX_HARDID_LEN);

 		NotifyServer(GR_TRANSFER_DEPOSIT, &td, sizeof(td), CalcSubIndexByUserID(td.nUserID));

		GAME_DEPOSIT_TRANSFER gdt;
		memset(&gdt,0,sizeof(gdt));
		gdt.nRequestID=GR_SAVE_SAFE_DEPOSIT;
		gdt.nUserID=nUserID;
		gdt.nFromGame=nGameID;
		gdt.nToGame=0;
		gdt.nDeposit=lpSaveSafeDeposit->nDeposit;
 		NotifyServer(GR_GAME_DEPOSIT_TRANSFER, &gdt, sizeof(gdt), CalcKubIndexByGameID(gdt.nFromGame));


		td.nFromRoom=nRoomID;
	 	int clientid = FindClientID(td.nFromRoom);
 		if(clientid) 
			NotifyOneRoomSvr(GR_TRANSFER_DEPOSIT, &td, sizeof(td), clientid);
 
	}
	UwlClearRequest(&response);

	return TRUE;
}


BOOL CSockServer::OnLookSafeDeposit(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRepeated = 1;

	BOOL bPureRepeat = FALSE;
	int nRepeated = lpRequest->head.nRepeated;
	int nResult = 0;
	LONG errcode = 0;
	_ConnectionPtr pDBConnect = NULL;

	LPLOOK_SAFE_DEPOSIT lpLookSafeDeposit = LPLOOK_SAFE_DEPOSIT(PBYTE(lpRequest->pDataPtr) + nRepeated * sizeof(CONTEXT_HEAD));	

	SAFE_DEPOSIT_EX sde;
	ZeroMemory(&sde, sizeof(sde));
	sde.nUserID=lpLookSafeDeposit->nUserID;
	sde.nRemindDeposit=1000000;

	//维护期间停止请求
	if(IsMainDBLock())
	{
		response.head.nRequest = GR_SYSTEM_LOCKED;
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	
	if(pThreadCxt){
		pDBConnect = pThreadCxt->m_pDBConnectMain;
	}
	if(NULL == pDBConnect){
		pThreadCxt->m_bReConnectMain = TRUE; // !!! dont forget !!!
		response.head.nRequest= UR_DATABASE_ERROR;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
 	if(IsInPeriodOfProfile(_T("dblock"), DEF_DBLOCK_BEGIN, DEF_DBLOCK_END)){
		response.head.nRequest = GR_SYSTEM_LOCKED;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	nResult = DB_LookSafeDeposit(errcode, pDBConnect, response.head.nRequest, sde.nUserID,sde.nDeposit,sde.bHaveSecurePwd);
	if(nResult <= 0){
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	//满足条件，免计算保护密码
	if (sde.bHaveSecurePwd && IsSecurePwdVerified(sde.nUserID, lpLookSafeDeposit->dwIPAddr, lpLookSafeDeposit->szHardID))
	{
		sde.bHaveSecurePwd = FALSE;
	}

	int nLen;
	nLen = nRepeated * sizeof(CONTEXT_HEAD) + sizeof(SAFE_DEPOSIT_EX);
	PBYTE pData;
	pData = NULL;
	pData = new BYTE[nLen];

	memcpy(pData, lpRequest->pDataPtr, nRepeated * sizeof(CONTEXT_HEAD));
	memcpy(pData + nRepeated * sizeof(CONTEXT_HEAD), &sde, sizeof(sde));

	response.pDataPtr = pData;
	response.nDataLen = nLen;
	response.head.nRequest = GR_LOOK_SAFE_DEPOSIT_OK;

SendResponse:
	if(bPureRepeat){
		int nLen;
		nLen = nRepeated * sizeof(CONTEXT_HEAD);
		PBYTE pData;
		pData = NULL;
		if(nLen) pData = new BYTE[nLen];
		memcpy(pData, lpRequest->pDataPtr, nRepeated * sizeof(CONTEXT_HEAD));
		response.pDataPtr = pData;
		response.nDataLen = nLen;
	}
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendRequest(lpContext->hSocket, &context, &response);

	if(GR_LOOK_SAFE_DEPOSIT_OK == response.head.nRequest)
	{
	}
	else
	{
	}
	UwlClearRequest(&response);

	return TRUE;
}



BOOL CSockServer::OnTakeSafeRndKey(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRepeated = 1;

	BOOL bPureRepeat = FALSE;
	int nRepeated = lpRequest->head.nRepeated;
	int nResult = 0;
 	LONG errcode = 0;
	int nTransStarted = 0;

 	TCHAR szSecurePwd[MAX_PASSWORD_LEN];
	ZeroMemory(szSecurePwd,sizeof(szSecurePwd));

	SAFE_RNDKEY sr;
	ZeroMemory(&sr, sizeof(sr));
	
 	LPTAKE_SAFE_RNDKEY lpTakeSafeRndKey = LPTAKE_SAFE_RNDKEY(PBYTE(lpRequest->pDataPtr) + nRepeated * sizeof(CONTEXT_HEAD));	

	_ConnectionPtr pDBConnect = NULL;
	//维护期间停止请求
	if(IsMainDBLock())
	{
		response.head.nRequest = GR_SYSTEM_LOCKED;
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	if(pThreadCxt){
		pDBConnect = pThreadCxt->m_pDBConnectMain;
	}
	if(NULL == pDBConnect){
		pThreadCxt->m_bReConnectMain = TRUE; // !!! dont forget !!!
		response.head.nRequest= UR_DATABASE_ERROR;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
 	if(IsInPeriodOfProfile(_T("dblock"), DEF_DBLOCK_BEGIN, DEF_DBLOCK_END)){
		response.head.nRequest = GR_SYSTEM_LOCKED;
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	//满足条件，免计算保护密码
	if (IsSecurePwdVerified(lpTakeSafeRndKey->nUserID, lpTakeSafeRndKey->dwIPAddr, lpTakeSafeRndKey->szHardID))
	{
		response.head.nRequest = GR_SECUREPWD_EMPTY;
		bPureRepeat = TRUE;
		goto SendResponse;
	}

 	nResult = DB_GetSecurePwd(errcode, pDBConnect, response.head.nRequest, lpTakeSafeRndKey->nUserID, szSecurePwd);
	if(nResult <= 0){
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(lstrcmpi(szSecurePwd, _T(""))==0)// 保护密码为空
	{ 
 		response.head.nRequest = GR_SECUREPWD_EMPTY;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
    
	nTransStarted=DB_BeginTrans(errcode, pDBConnect, response.head.nRequest);
	if(nTransStarted <= 0){
		bPureRepeat = TRUE;
		goto SendResponse;
	}


	sr.nUserID=lpTakeSafeRndKey->nUserID;
 	UwlCreateRandomNumbers(sr.nRndKey, MAX_RND_DIGIT_NUM);
 	nResult = DB_SetRndKey(errcode, pDBConnect, response.head.nRequest, sr.nUserID,sr.nRndKey);
	if(nResult <= 0){
		bPureRepeat = TRUE;
		goto SendResponse;
	}


	int nLen;
	nLen = nRepeated * sizeof(CONTEXT_HEAD) + sizeof(SAFE_RNDKEY);
	PBYTE pData;
	pData = NULL;
	pData = new BYTE[nLen];

	memcpy(pData, lpRequest->pDataPtr, nRepeated * sizeof(CONTEXT_HEAD));
	memcpy(pData + nRepeated * sizeof(CONTEXT_HEAD), &sr, sizeof(sr));

	response.pDataPtr = pData;
	response.nDataLen = nLen;
	response.head.nRequest = GR_TAKE_SAFE_RNDKEY_OK;

SendResponse:
	if(bPureRepeat){
		int nLen;
		nLen = nRepeated * sizeof(CONTEXT_HEAD);
		PBYTE pData;
		pData = NULL;
		if(nLen) pData = new BYTE[nLen];
		memcpy(pData, lpRequest->pDataPtr, nRepeated * sizeof(CONTEXT_HEAD));
		response.pDataPtr = pData;
		response.nDataLen = nLen;
	}
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendRequest(lpContext->hSocket, &context, &response);

	if(GR_TAKE_SAFE_RNDKEY_OK == response.head.nRequest){
		if(nTransStarted > 0){
			DB_Commit(errcode, pDBConnect, response.head.nRequest);
		}
	}else{
		if(nTransStarted > 0){
			DB_Rollback(errcode, pDBConnect, response.head.nRequest);
		}
	}
	UwlClearRequest(&response);

	return TRUE;
}


BOOL CSockServer::IsSecurePwdVerified(int nUserID, DWORD dwIPAddr, LPCTSTR szHardID)
{
	CAutoLock lock(&g_csSecurePwdVerified);
	PASSWORD_VERIFIED pv;
	memset(&pv,0,sizeof(pv));
	if (g_mapSecurePwdVerified.Lookup(nUserID,pv))
	{
		int nCreateDate = 0;
		int nCreateTime = 0;
		UwlGetCurrentDateTime(nCreateDate, nCreateTime);
		if (pv.nCurrentDate == nCreateDate
			&& pv.dwIPAddr == dwIPAddr
			&& lstrcmp(pv.szHardID, szHardID)==0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

*/

/*
赛趣斗地主进入游戏，提交结果示例
BOOL CSockServer::OnExtEnterGame_SQ(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
	int gamedbindex=0;

	BOOL bPureRepeat = FALSE;
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRepeated = 1;
	int nRepeated = lpRequest->head.nRepeated;
	int nResult = 0;
	LONG errcode = 0;
	BOOL bFindRoom=FALSE;

	LPENTER_GAME_EX lpEnterGame = LPENTER_GAME_EX(PBYTE(lpRequest->pDataPtr)
									+ nRepeated * sizeof(CONTEXT_HEAD));

	int nGameID=lpEnterGame->nGameID;
	int nUserID=lpEnterGame->nUserID;
	
	_ConnectionPtr pDBConnectEx = NULL;
	if( nGameID<=0 || nGameID >=MAX_GAME_NUM )
	{
		response.head.nRequest = UR_OBJECT_NOT_EXIST;
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	gamedbindex=CalcGameDBIndexByGameID(nGameID);
	
	//维护期间停止请求
	if(IsGameDBLock(gamedbindex))
	{
		response.head.nRequest = GR_SYSTEM_LOCKED;
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	ROOM room;
	ZeroMemory(&room, sizeof(room));
	{
		CAutoLock lock(&g_csBaseData);
		bFindRoom=FindRoomByID(lpEnterGame->nRoomID,g_mapRoom,&room);
	}
   	if(!bFindRoom )
	{
		response.head.nRequest = GR_ROOM_NOT_EXIST;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(lpEnterGame->nTableNO >= room.nTableCount || lpEnterGame->nChairNO >= room.nChairCount)
	{
		response.head.nRequest = UR_OBJECT_NOT_EXIST;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(!IS_BIT_SET(room.dwConfigs,ROOM_CFG_SOLO))
	{
		if(!VerifyEnterGame(room.nGameVID,lpEnterGame->nUserID,lpEnterGame->nRoomID,lpEnterGame->nTableNO,lpEnterGame->nChairNO))
		{
			response.head.nRequest = UR_OBJECT_NOT_EXIST;
			bPureRepeat = TRUE;
			goto SendResponse;
		}
	}
	if (!IS_BIT_SET(room.dwManages,ROOM_MNG_MATCHONGAME))	//只能用于使用赛趣游戏表的游戏
	{
		response.head.nRequest = GR_NO_THIS_FUNCTION;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////

	if(pThreadCxt){		
		pDBConnectEx = pThreadCxt->m_pDBConnectGame[gamedbindex];
	}
	if(NULL == pDBConnectEx)
	{
		pThreadCxt->m_bReConnectGame[gamedbindex] = TRUE; // !!! dont forget !!!
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	GAME_RESULT_SQ GameResult;
	ZeroMemory(&GameResult, sizeof(GameResult));
	GameResult.nRoomID  = lpEnterGame->nRoomID;
	GameResult.nTableNO = lpEnterGame->nTableNO;
	GameResult.nChairNO = lpEnterGame->nChairNO;
	GameResult.nUserID = nUserID;

	nResult = DB_GetMatchGameResult_SQ(errcode, pDBConnectEx, response.head.nRequest, room.nGameDBID,nUserID, &GameResult);
	if(nResult <= 0){
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	
	if(!IS_BIT_SET(room.dwManages,ROOM_CFG_NOTCHECKDEPOSIT))
	{
 		if(GameResult.nDeposit > room.nMaxDeposit || GameResult.nDeposit < room.nMinDeposit)
		{
			response.head.nRequest = UR_OPERATE_FAILED;
			bPureRepeat = TRUE;
			goto SendResponse;

		}
	}

	EXT_ENTER_SQ_OK eeso;
	ZeroMemory(&eeso, sizeof(eeso));

	eeso.egoe.nUserID		= GameResult.nUserID;				// 用户ID
	eeso.egoe.nRoomID		= GameResult.nRoomID;				// 房间ID
	eeso.egoe.nTableNO		= GameResult.nTableNO;				// 桌号
	eeso.egoe.nChairNO		= GameResult.nChairNO;				// 位置
	eeso.egoe.nScore		= GameResult.nScore;				// 积分
	eeso.egoe.nDeposit		= GameResult.nDeposit;				// 银子
	eeso.egoe.nLevelID		= GameResult.nPlayerLevel;			// 级别ID
	eeso.egoe.nExperience   = GameResult.nTimeCost / 60; 
 	eeso.egoe.nBout			= GameResult.nBout;
	eeso.egoe.nUserType		= GetUserType(nGameID,nUserID);     //UserType 

	{
		CAutoLock lock(&g_csBaseData);			//级别名称
		
		LPPLAYERLEVEL pPlayerLevel = MemoGetPlayerLevel(g_mapAllPlayerLevel, nGameID, GameResult.nPlayerLevel);
		if(pPlayerLevel){
			memcpy(eeso.egoe.szLevelName, pPlayerLevel->szLevelName, sizeof(eeso.egoe.szLevelName));
		}
	}

	eeso.nGameID			= nGameID;
	eeso.nMatchID			= GameResult.nMatchID;
	eeso.nClubID			= GameResult.nClubID;
	eeso.nMemberLevel		= GameResult.nMemberLevel;
	eeso.nTaskGain			= GameResult.nTaskGain;
	eeso.nBombCount			= GameResult.nBombCount;
	eeso.nMaxBoutScore		= GameResult.nMaxBoutScore;

	eeso.nContestentLevel	= GameResult.nContestentLevel;
	eeso.nCoachLevel		= GameResult.nCoachLevel;
	eeso.nLuckBoxCount		= GameResult.nLuckBoxCount;
	eeso.nLuckBoxTaken		= GameResult.nLuckBoxTaken;
	eeso.nWinPoint			= GameResult.nWinPoint;
	eeso.nPointPerWin		= GameResult.nPointPerWin;
	eeso.nNextBoxNeedPoint	= GameResult.nNextBoxNeedPoint;
	eeso.dwLuckBuff			= GameResult.dwLuckBuff;
	eeso.nCardRob			= GameResult.nCardRob;
	eeso.nCardProtect		= GameResult.nCardProtect;
	eeso.nComboWin			= GameResult.nComboWin;
	eeso.nAuthority			= GameResult.nAuthority;
	eeso.nCardGuidance		= GameResult.nCardGuidance;

	/////////////////////////////////////////////////////////////////
	int nLen;
	nLen = nRepeated * sizeof(CONTEXT_HEAD) + sizeof(eeso);
	PBYTE pData;
	pData = NULL;
	pData = new BYTE[nLen];

	memcpy(pData, lpRequest->pDataPtr, nRepeated * sizeof(CONTEXT_HEAD));
	memcpy(pData + nRepeated * sizeof(CONTEXT_HEAD), &eeso, sizeof(eeso));

	response.pDataPtr = pData;
	response.nDataLen = nLen;
	response.head.nRequest = EGR_ENTER_GAME_OK;

SendResponse:
	if(bPureRepeat){
		int nLen;
		nLen = nRepeated * sizeof(CONTEXT_HEAD);
		PBYTE pData;
		pData = NULL;
		if(nLen) pData = new BYTE[nLen];
		memcpy(pData, lpRequest->pDataPtr, nRepeated * sizeof(CONTEXT_HEAD));
		response.pDataPtr = pData;
		response.nDataLen = nLen;
	}
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendRequest(lpContext->hSocket, &context, &response);

	UwlClearRequest(&response);
	
	if(EGR_ENTER_GAME_OK == response.head.nRequest)
	{
		if(IS_BIT_SET(room.dwConfigs,ROOM_CFG_ENTERGAMEOKTOKUB))
		{
			ENTER_GAME_OK  ego;
			ZeroMemory(&ego,sizeof(ego));
			ego.nGameID=nGameID;
			ego.nGameVID=room.nGameVID;
			ego.nRoomID=lpEnterGame->nRoomID;
			ego.nUserID=lpEnterGame->nUserID;
			ego.nTableNO=lpEnterGame->nTableNO;
			ego.nChairNO=lpEnterGame->nChairNO;
 			NotifyServer(GR_ENTER_GAME_OK, &ego, sizeof(ego),  CalcKubIndexByGameID(nGameID));
		}	
	}
	else
	{
	}
	return TRUE;
}


BOOL CSockServer::OnExtRefreshResult_SQ(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
	//本请求暂不支持kubsvr验证

//  	int nRoomID=lpRefreshResultEx->nRoomID;
// 
//   	ROOM room;
//  	ZeroMemory(&room,sizeof(room));
// 	{
// 		CAutoLock lock(&g_csBaseData);
// 		FindRoomByID(nRoomID,g_mapRoom,&room);
// 	}
//  	if(room.nRoomID==0)
// 		return FALSE;
// 
// 	lpRefreshResultEx->nClientID=room.nGameVID;//借用nClinetID存放nGameVID;

//   if(IS_BIT_SET(room.dwConfigs,ROOM_CFG_VERIFYRESULT))
// 		TransmitRequest(lpContext, lpRequest,CalcKubIndexByGameID(room.nGameID));//发送到kubsvr验证
// 	 else
//		OnExtRefreshResultFromKub_SQ(lpContext,lpRequest,pThreadCxt);

	int nTime1 = 0;
	int nTime2 = 0;
	int gamedbindex=0;
	BOOL bPureRepeat = FALSE;
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRepeated = 1;
	int nRepeated = lpRequest->head.nRepeated;
	int nResult = 0;
	LONG errcode = 0;
	int nTransStarted = 0;
	int i = 0;	
  	BOOL bFindRoom=FALSE;
	BOOL bFindGame=FALSE;
	GAME_DEPOSIT_TAKEOUT gdt;
	ZeroMemory(&gdt,sizeof(gdt));

	NTF_RESULT_UNSAVED  nru;
	memset(&nru,0,sizeof(nru));

	ROOM room;
	GAME game;
	ZeroMemory(&room,sizeof(room));
	ZeroMemory(&game,sizeof(game));

	_ConnectionPtr pDBConnectEx = NULL;

	LPREFRESH_RESULT_EX lpRefreshResultEx = LPREFRESH_RESULT_EX(PBYTE(lpRequest->pDataPtr) 
									+ nRepeated * sizeof(CONTEXT_HEAD));
	LPEXT_GAME_RESULT_SQ lpGameResultEx1 = LPEXT_GAME_RESULT_SQ((PBYTE)lpRefreshResultEx + sizeof(REFRESH_RESULT_EX) );

	int nGameID=lpRefreshResultEx->nGameID;
	int nRoomID=lpRefreshResultEx->nRoomID;
	int nTableNO=lpRefreshResultEx->nTableNO;

	gdt.nGameID=nGameID;
 	gdt.nRoomID=nRoomID;
	gdt.nCount=lpRefreshResultEx->nResultCount;
	
	nru.nGameID=nGameID;
	nru.nRoomID=nRoomID;
	nru.nTableNO=nTableNO;
	nru.dwResetTime=lpRefreshResultEx->dwResetTime;

	if( nGameID <= 0 || nGameID >=MAX_GAME_NUM)
	{
	    nru.nCauseID=RUCID_INVALIDPARAM;
 		bPureRepeat = TRUE;
		goto SendResponse;
	}

	{
		CAutoLock lock(&g_csBaseData);
		bFindRoom=FindRoomByID(nRoomID,g_mapRoom,&room);
		bFindGame=FindGameByID(nGameID,&game);

		gdt.dwDBIP=game.dwDBIP;
		gdt.nGameVID=room.nGameVID;
		gdt.dwRoomManages = room.dwManages;		//记录带上房间manage, 用于比赛房间不记录茶水费和大银两交易	
	}
 	if(!bFindRoom || !bFindGame)
	{
	    nru.nCauseID=RUCID_OBJECTNOTFIND;
 		bPureRepeat = TRUE;
		goto SendResponse;
	}
 
	if( room.nGameID != nGameID )
	{
	    nru.nCauseID=RUCID_INVALIDPARAM;
 		bPureRepeat = TRUE;
		goto SendResponse;
	}

	gamedbindex=CalcGameDBIndexByGameID(nGameID);
	
	//维护期间停止请求
	if(IsGameDBLock(gamedbindex))
	{
		nru.nCauseID=RUCID_DATABASEERROR;
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	if (!IS_BIT_SET(room.dwManages,ROOM_MNG_MATCHONGAME))	//只能用于使用赛趣游戏表的游戏
	{
		response.head.nRequest = RUCID_OBJECTNOTFIND;
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	if(ROOM_TYPE_TRAIN == room.nRoomType){
		bPureRepeat = TRUE;
		goto SendResponse;
	}
  
	__int64 total_deposit;
	total_deposit = 0;
	__int64 total_fee;
	total_fee = 0;
	__int64 total_cut;
	total_cut = 0;
	__int64 total_extra;//目前只有jsbgsvr支持这个。
	total_extra = 0;

	for(i = 0; i < lpRefreshResultEx->nResultCount; i++){
		LPEXT_GAME_RESULT_SQ lpGameResultEx = LPEXT_GAME_RESULT_SQ((PBYTE)lpRefreshResultEx 
						+ sizeof(REFRESH_RESULT_EX) + i * sizeof(EXT_GAME_RESULT_SQ));
  
		if(0 == lpGameResultEx->nUserID ){
			nru.nCauseID=RUCID_INVALIDPARAM;
 			bPureRepeat = TRUE;
			goto SendResponse;
		}
		if( lpGameResultEx->nGameID <= 0 || lpGameResultEx->nGameID >=MAX_GAME_NUM){
		    nru.nCauseID=RUCID_INVALIDPARAM;
			bPureRepeat = TRUE;
			goto SendResponse;
		}
		if(IsForbidUser(lpGameResultEx->nUserID)){
			nru.nCauseID=RUCID_FORBIDDENUSER;
			nru.nUserID=lpGameResultEx->nUserID;
			bPureRepeat = TRUE;
			goto SendResponse;
		}

		total_deposit += (__int64)(lpGameResultEx->nDepositDiff);
		total_fee += (__int64)(lpGameResultEx->nFee);
		total_cut += (__int64)(lpGameResultEx->nCut);
		total_extra += (__int64)(lpGameResultEx->nExtra);
		//////////////////////////////////////////////////
		///////////////////////////////////////////////////
		//////////////////////////////////////////////////
		gdt.udd[i].nUserID=lpGameResultEx->nUserID;
		gdt.udd[i].nOldDeposit=lpGameResultEx->nOldDeposit;
		gdt.udd[i].nDepositDiff=lpGameResultEx->nDepositDiff;
	
	}

	if(pThreadCxt){
		pDBConnectEx = pThreadCxt->m_pDBConnectGame[gamedbindex];
	}
	if(NULL == pDBConnectEx )
	{
		pThreadCxt->m_bReConnectGame[gamedbindex] = TRUE; // !!! dont forget !!!
	    nru.nCauseID=RUCID_DATABASEERROR;
 		bPureRepeat = TRUE;
		goto SendResponse;
	}

	//
	LPEXT_GAME_RESULT_SQ lpgrex;
	lpgrex = LPEXT_GAME_RESULT_SQ((PBYTE)lpRefreshResultEx + sizeof(REFRESH_RESULT_EX));

	if(total_cut < 0 || total_fee < 0 || total_extra < 0){
		ExtLogDepositError_SQ(lpRefreshResultEx, lpgrex);
		nru.nCauseID=RUCID_INVALIDPARAM;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(total_cut > INT_MAX || total_fee > INT_MAX || total_extra > INT_MAX){
		ExtLogDepositError_SQ(lpRefreshResultEx, lpgrex);
		nru.nCauseID=RUCID_INVALIDPARAM;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(IS_BIT_SET(lpRefreshResultEx->dwFlags, RF_IGNORE_DIFFSUM)){ // 忽略差值总和校验
		// 不支持此项FLAG
	}

	if(IS_BIT_SET(lpRefreshResultEx->dwFlags, RF_PRESAVE_DEPOSIT) && IS_BIT_SET(room.dwConfigs,ROOM_CFG_VARIABLECHAIR))
	{
		if(lpRefreshResultEx->nResultCount != 1 || lpGameResultEx1->nDepositDiff > 0 )
		{
			nru.nCauseID=RUCID_INVALIDPARAM;
			bPureRepeat = TRUE;
			goto SendResponse;
		}
	}

	if(IS_BIT_SET(lpRefreshResultEx->dwFlags, RF_ENDSAVE_DEPOSIT) && IS_BIT_SET(room.dwConfigs,ROOM_CFG_VARIABLECHAIR))
	{
//		if(lpRefreshResultEx->nResultCount < 1 || total_deposit + total_cut  < 0  || total_deposit + total_fee  < 0)
 		if(lpRefreshResultEx->nResultCount < 1 )
		{
			nru.nCauseID=RUCID_INVALIDPARAM;
			bPureRepeat = TRUE;
			goto SendResponse;
		}
	}


	if(total_deposit + total_cut - total_extra > 0  || total_deposit + total_fee - total_extra > 0)
	{
		if(IS_BIT_SET(lpRefreshResultEx->dwFlags, RF_ENDSAVE_DEPOSIT) && IS_BIT_SET(room.dwConfigs,ROOM_CFG_VARIABLECHAIR))
		{
			LONG nPreDeposit=0;
			{
				CAutoLock lock(&g_csPreSaveDeposit);
 				CString sKey;
				sKey.Format(_T("%d|%d"),nRoomID,nTableNO);
 				g_mapPreSaveDeposit.Lookup(sKey,nPreDeposit);
			}

			int nDiff1=total_deposit + total_cut;
			int nDiff2=total_deposit + total_fee;

			if(nDiff1 > nPreDeposit || nDiff2 > nPreDeposit)
			{
				ExtLogDepositError_SQ2(lpRefreshResultEx, lpgrex);
				nru.nCauseID=RUCID_INVALIDPARAM;
				bPureRepeat = TRUE;
				goto SendResponse;

			}
		}
		else
		{
			ExtLogDepositError_SQ(lpRefreshResultEx, lpgrex);
			nru.nCauseID=RUCID_INVALIDPARAM;
			bPureRepeat = TRUE;
			goto SendResponse;
		}
	}

	////////////////////////////////////////////////////////
	DWORD dwIP;
	dwIP= GetClientAddress(lpContext->hSocket, lpContext->lTokenID);
	SALARY_CONTEXT  sc;
	ZeroMemory(&sc,sizeof(sc));
	sc.nMinSalarySecond=room.nMinSalarySecond;
 	sc.nMaxSalarySecond=room.nMaxSalarySecond;
	sc.nUnitSalary=room.nUnitSalary;
	sc.dwRoomManages=room.dwManages;
	sc.dwGameOptions=game.dwOptions;
	sc.bIsGameSvrIP=IsGameSvrIP(dwIP);
	sc.bIsValidTimeCost=IsValidTimeCost(room.nGameVID,lpGameResultEx1->nUserID,lpGameResultEx1->nTimeCost,sc.nValidInterval);
	if(!sc.bIsGameSvrIP)
	{
 		TCHAR szIP[MAX_SERVERIP_LEN];
		ZeroMemory(szIP, sizeof(szIP));
		UwlAddrToName(dwIP, szIP);
#ifndef _RS125
		UwlLogFile(_T("OnExtRefreshResult_SQ Find UnKnowable IP. GameID=%d, RoomID=%d, GameSvrIP=%s"), nGameID,nRoomID,szIP);
#endif
	}
	if(!sc.bIsValidTimeCost)
	{
		UwlLogFile(_T("OnExtRefreshResult_SQ Find InValidTimeCost. GameID=%d, RoomID=%d, UserID= %d, TimeCost=%d, Interval=%d"), nGameID,nRoomID,lpGameResultEx1->nUserID,lpGameResultEx1->nTimeCost,sc.nValidInterval);
	}

  	DWORD dwFlags;
	dwFlags = ExtGetVerifyFlags_SQ(lpRefreshResultEx );
	
	////////////////////////////////////////////////////////
	nTime1 = GetTickCount();
	nTransStarted = DB_BeginTransEx(errcode, pDBConnectEx, response.head.nRequest);
	if(nTransStarted <= 0){
		nru.nCauseID=RUCID_DATABASEERROR;
 		bPureRepeat = TRUE;
		goto SendResponse;
	}

	for(i = 0; i < lpRefreshResultEx->nResultCount; i++)
	{
		LPEXT_GAME_RESULT_SQ lpGameResultEx = LPEXT_GAME_RESULT_SQ((PBYTE)lpRefreshResultEx + sizeof(REFRESH_RESULT_EX) + i * sizeof(EXT_GAME_RESULT_SQ));
	
		if(lpGameResultEx->nTimeCost<0)
			lpGameResultEx->nTimeCost=0;
		if(lpGameResultEx->nTimeCost > sc.nMaxSalarySecond)
			lpGameResultEx->nTimeCost = sc.nMaxSalarySecond;
 		if(IS_BIT_SET(room.dwManages,ROOM_MNG_NOEXPIFSTANDOFF))
		{
			if(	lpGameResultEx->nStandOff > 0 ) //和局
			{
				lpGameResultEx->nTimeCost=0;
			}
		}
		if(!sc.bIsValidTimeCost)
		{
			lpGameResultEx->nTimeCost=0;
		}

		//////////////////////////////////////////////////////////////////////////////////
		nResult = DB_RefreshMatchResult_SQ(errcode, pDBConnectEx, response.head.nRequest, room.nGameDBID, lpGameResultEx,dwFlags);

		if(nResult <= 0){
				
			if(nResult==0)
			{
 				nru.nCauseID=RUCID_DEPOSITDIFFER;
				nru.nUserID=lpGameResultEx->nUserID;
			}
			else
				nru.nCauseID=RUCID_DATABASEERROR;
			bPureRepeat = TRUE;
			goto SendResponse;
		}
	}
	response.head.nRequest = UR_UPDATE_SUCCEEDED;

SendResponse:
 
	if(bPureRepeat){
	}
	if(UR_UPDATE_SUCCEEDED == response.head.nRequest){
		if(nTransStarted > 0){
			DB_CommitEx(errcode, pDBConnectEx, response.head.nRequest);
		}
	}else{
		if(nTransStarted > 0){
			DB_RollbackEx(errcode, pDBConnectEx, response.head.nRequest);
		}
	}

	if (UR_UPDATE_SUCCEEDED==response.head.nRequest)
	{
		nTime2 = GetTickCount();
		CAutoLock lock(&g_csDBMonitor);
		g_dbMonitor.dGameResult[gamedbindex] = CalcAverage(g_dbMonitor.dGameResult[gamedbindex], g_dbMonitor.nGRCount[gamedbindex], nTime2-nTime1, 1);
		g_dbMonitor.nGRCount[gamedbindex] += 1;
	}


	if(UR_UPDATE_SUCCEEDED == response.head.nRequest)
	{
 		if(IS_BIT_SET(lpRefreshResultEx->dwFlags, RF_PRESAVE_DEPOSIT) && IS_BIT_SET(room.dwConfigs,ROOM_CFG_VARIABLECHAIR))
		{
			LONG nPreDeposit=0;
			{
				CAutoLock lock(&g_csPreSaveDeposit);
 				CString sKey;
				sKey.Format(_T("%d|%d"),nRoomID,nTableNO);
 				g_mapPreSaveDeposit.Lookup(sKey,nPreDeposit);
				nPreDeposit+=(-lpGameResultEx1->nDepositDiff);
 				g_mapPreSaveDeposit.SetAt(sKey,nPreDeposit);
			}
		}

 		if(IS_BIT_SET(lpRefreshResultEx->dwFlags, RF_ENDSAVE_DEPOSIT) && IS_BIT_SET(room.dwConfigs,ROOM_CFG_VARIABLECHAIR))
		{
			{
				CAutoLock lock(&g_csPreSaveDeposit);
 				CString sKey;
				sKey.Format(_T("%d|%d"),nRoomID,nTableNO);
  				g_mapPreSaveDeposit.RemoveKey(sKey);
			}
		}

 		if(total_fee){
			LPADD_FEE pAddFee = new ADD_FEE;
			ZeroMemory(pAddFee, sizeof(ADD_FEE));
			pAddFee->nFee = total_fee;
			PutToServer(lpContext, GR_ADD_FEE, pAddFee, sizeof(ADD_FEE));

			gdt.nTotalFee=total_fee;///
		}
		if(total_cut){
			LPADD_CUT pAddCut = new ADD_CUT;
			ZeroMemory(pAddCut, sizeof(ADD_CUT));
			pAddCut->nCut = total_cut;
			PutToServer(lpContext, GR_ADD_CUT, pAddCut, sizeof(ADD_CUT));

			gdt.nTotalCut=total_cut; ////
		}
		if(total_extra)
			gdt.nTotalExtra=total_extra;
 
		lpRefreshResultEx->nClientID = g_nClientID;
		int clientid = FindClientID(lpRefreshResultEx->nRoomID);
		if(clientid)
		{
			int nLen = sizeof(REFRESH_RESULT) + lpRefreshResultEx->nResultCount * sizeof(GAME_RESULT);
			PBYTE pData = new BYTE[nLen];
			ZeroMemory(pData, nLen);
			LPREFRESH_RESULT lpRefreshResult = (LPREFRESH_RESULT)pData;
			ExtCompactRefreshResult_SQ(lpRefreshResultEx, lpRefreshResult);
			NotifyOneRoomSvr(GR_REFRESH_RESULT, pData, nLen, clientid);
			SAFE_DELETE_ARRAY(pData);
		} 
		///////////////////////////////////////////////////////////////////////
		///////////////////////是否记录游戏结果日志////////////////////////////
 		if(IS_BIT_SET(dwFlags, RF_VERIFY_DEPOSIT))
		{
 			int deposit_logdb = GetPrivateProfileInt(_T("LogGameResult"), xyConvertIntToStr(gdt.nGameID),50000, g_szIniFile);
			if(deposit_logdb)
			{
				if(total_fee >= deposit_logdb || total_cut >= deposit_logdb || total_extra >= deposit_logdb || NeedLogGameResultEx_SQ(lpRefreshResultEx, deposit_logdb))
				{
	//				if(lpRefreshResultEx->nResultCount <= 4)
					if(lpRefreshResultEx->nResultCount <= 6)	//扩展到6人 2014.11.26
					{
						gdt.nLogResult=1;
					}
				}
			}
		}		
		///////////////////////////////////////////////////////////////////////
 		//////////////发现系统抽取银子,或者需要记录日志,则通知到kubsvr////////// 
		if(!IS_BIT_SET(room.dwManages,ROOM_MNG_MATCHONGAME))
		{
 			if(gdt.nLogResult>0 ||  gdt.nTotalFee>0 || gdt.nTotalCut>0 )
				NotifyServer(GR_GAME_DEPOSIT_TAKEOUT, &gdt, sizeof(gdt)-sizeof(USER_DEPOSIT_DIFF)*MAX_CHAIR_COUNT+gdt.nCount*sizeof(USER_DEPOSIT_DIFF),  CalcKubIndexByGameID( gdt.nGameID));
		}
	//	SaveTimeCostCheck(&gdt); 模板游戏暂时不检查时间

		//////////////////////////////////////////////////////////////////////////
		/////////////////////通知subsvr游戏结果，完成任务/////////////////////////
		{
			CAutoLock lock(&g_csTaskUserStatus);

			TASK_USERSTATUS tus;
			for (int i=0; i<lpRefreshResultEx->nResultCount; i++)
			{
				LPEXT_GAME_RESULT_SQ lpGameResultEx = LPEXT_GAME_RESULT_SQ((PBYTE)lpRefreshResultEx 
						+ sizeof(REFRESH_RESULT_EX) + i * sizeof(EXT_GAME_RESULT_SQ));

				if (g_mapTaskUserStatus.Lookup(lpGameResultEx->nUserID, tus))
				{
					if (IS_BIT_SET(tus.dwUserStatus, TASK_USERSTATUS_SUBMIT_GAMERESULT))
					{
						TASK_USERGAMERESULT tu;
						ZeroMemory(&tu, sizeof(tu));
						tu.nUserID = lpGameResultEx->nUserID;
						tu.nGameID = lpRefreshResultEx->nGameID;
						tu.nRoomID = lpRefreshResultEx->nRoomID;
						tu.nDepositDiff = lpGameResultEx->nDepositDiff;
						tu.nScoreDiff = lpGameResultEx->nScoreDiff;
						tu.nTimeCost = lpGameResultEx->nTimeCost;
						NotifyServer(GR_TASK_USERGAMERESULT, &tu, sizeof(tu),  CalcSubIndexByUserID(tu.nUserID));
					}
				}
			}
		}
	}
	else
	{
		if(nru.nCauseID>0)
		{
 			memset(&response, 0, sizeof(response));
			response.pDataPtr = &nru;
			response.nDataLen = sizeof(nru);
			response.head.nRequest = GR_RESULT_UNSAVED;

			CONTEXT_HEAD context;
			memcpy(&context, lpContext, sizeof(context));
			context.bNeedEcho = FALSE;
 			SendRequest(lpContext->hSocket, &context, &response,MSG_REQUEST);

			UwlLogFile(_T("GR_RESULT_UNSAVED,GameID=%d, RoomID=%d, CauseID=%d, UserID=%d"),nru.nGameID,nru.nRoomID,nru.nCauseID,nru.nUserID);
		}

		if (RUCID_DATABASEERROR == nru.nCauseID || RUCID_DEPOSITDIFFER == nru.nCauseID)
		{
			CAutoLock lock(&g_csDBMonitor);
			if (RUCID_DATABASEERROR == nru.nCauseID)
				g_dbMonitor.nGRDBErrCount += 1;
			else
				g_dbMonitor.nGRDepositDiffCount += 1;
		}

		if (nru.nCauseID>0)
		{
			int nNoLog = GetPrivateProfileInt(_T("LogGameResultError"), _T("disable"),0, g_szIniFile);
			if (0==nNoLog)
			{
				GAMERESULT_ERROR_LOG gel;
				ZeroMemory(&gel, sizeof(gel));
				gel.nGameID = nGameID;
				gel.nRoomID = nRoomID;
				gel.nClientID = g_nClientID;
				gel.dwDBIP = gdt.dwDBIP;
				gel.nCauseID = nru.nCauseID;
				gel.nCount = lpRefreshResultEx->nResultCount;
				for (int i=0; i<gel.nCount; i++)
				{
					memcpy(&gel.udd[i], &gdt.udd[i], sizeof(USER_DEPOSIT_DIFF));
				}
				
				NotifyServer(GR_GAMERESULT_ERROR_LOG, &gel, sizeof(gel)-sizeof(USER_DEPOSIT_DIFF)*MAX_CHAIR_COUNT+gel.nCount*sizeof(USER_DEPOSIT_DIFF),  CalcKubIndexByGameID( gel.nGameID));
			}
		}
	}
 
 	return TRUE;
 
}
*/
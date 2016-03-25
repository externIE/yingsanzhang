#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CopyPlayerLevelExMap(PBYTE pData, CPlayerLevelExMap& mapPlayerLevel, BOOL toremove)
{
	int levelid = 0;
	LPPLAYERLEVEL_EX pPlayerLevel = NULL;
	LPPLAYERLEVEL_EX lpPlayerLevelData = LPPLAYERLEVEL_EX(pData);
	POSITION pos;
	pos  = mapPlayerLevel.GetStartPosition();
	while (pos)	{
		mapPlayerLevel.GetNextAssoc(pos, levelid, pPlayerLevel);
		if(pPlayerLevel){
			memcpy(lpPlayerLevelData, pPlayerLevel, sizeof(PLAYERLEVEL_EX));
	
			lpPlayerLevelData++;

			if(toremove){
				mapPlayerLevel.RemoveKey(levelid);
				SAFE_DELETE(pPlayerLevel);
			}
		}
	}
}

void LogDepositErrorEx(LPREFRESH_RESULT_EX lpRefreshResultEx, LPGAME_RESULT_EX lpGameResultEx)
{
	TCHAR szLogStr[512];
	TCHAR szBufStr[128];
	_stprintf(szLogStr, _T("deposit error! gameid = %ld."), lpRefreshResultEx->nGameID);
	for(int i = 0; i < lpRefreshResultEx->nResultCount; i++){
		LPGAME_RESULT_EX lpgrex = LPGAME_RESULT_EX((PBYTE)lpRefreshResultEx 
						+ sizeof(REFRESH_RESULT_EX) + i * sizeof(GAME_RESULT_EX));
		_stprintf(szBufStr, _T("user%ld = %ld, "), i, lpgrex->nUserID);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("diff%ld = %ld, "), i, lpgrex->nDepositDiff);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("cut%ld = %ld, "), i, lpgrex->nCut);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("fee%ld = %ld, "), i, lpgrex->nFee);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("extra%ld = %ld\r\n"), i, lpgrex->nExtra);
		lstrcat(szLogStr, szBufStr);
	}
	UwlLogFile(szLogStr);
}


void LogDepositErrorEx2(LPREFRESH_RESULT_EX lpRefreshResultEx, LPGAME_RESULT_EX lpGameResultEx)
{
	TCHAR szLogStr[512];
	TCHAR szBufStr[128];
	_stprintf(szLogStr, _T("end save deposit error! gameid = %ld."), lpRefreshResultEx->nGameID);
	for(int i = 0; i < lpRefreshResultEx->nResultCount; i++){
		LPGAME_RESULT_EX lpgrex = LPGAME_RESULT_EX((PBYTE)lpRefreshResultEx 
						+ sizeof(REFRESH_RESULT_EX) + i * sizeof(GAME_RESULT_EX));
		_stprintf(szBufStr, _T("user%ld = %ld, "), i, lpgrex->nUserID);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("diff%ld = %ld, "), i, lpgrex->nDepositDiff);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("cut%ld = %ld, "), i, lpgrex->nCut);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("fee%ld = %ld, "), i, lpgrex->nFee);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("extra%ld = %ld\r\n"), i, lpgrex->nExtra);
		lstrcat(szLogStr, szBufStr);
	}
	UwlLogFile(szLogStr);
}

void ExtLogDepositError_SQ(LPREFRESH_RESULT_EX lpRefreshResultEx, LPEXT_GAME_RESULT_SQ lpGameResultEx)
{
	TCHAR szLogStr[512];
	TCHAR szBufStr[128];
	_stprintf(szLogStr, _T("Ext SQ deposit error! gameid = %ld."), lpRefreshResultEx->nGameID);
	for(int i = 0; i < lpRefreshResultEx->nResultCount; i++){
		LPEXT_GAME_RESULT_SQ lpgrex = LPEXT_GAME_RESULT_SQ((PBYTE)lpRefreshResultEx 
			+ sizeof(REFRESH_RESULT_EX) + i * sizeof(EXT_GAME_RESULT_SQ));
		_stprintf(szBufStr, _T("user%ld = %ld, "), i, lpgrex->nUserID);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("diff%ld = %ld, "), i, lpgrex->nDepositDiff);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("cut%ld = %ld, "), i, lpgrex->nCut);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("fee%ld = %ld, "), i, lpgrex->nFee);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("extra%ld = %ld\r\n"), i, lpgrex->nExtra);
		lstrcat(szLogStr, szBufStr);
	}
	UwlLogFile(szLogStr);
}

void ExtLogDepositError_SQ2(LPREFRESH_RESULT_EX lpRefreshResultEx, LPEXT_GAME_RESULT_SQ lpGameResultE)
{
	TCHAR szLogStr[512];
	TCHAR szBufStr[128];
	_stprintf(szLogStr, _T("Ext SQ end save deposit error! gameid = %ld."), lpRefreshResultEx->nGameID);
	for(int i = 0; i < lpRefreshResultEx->nResultCount; i++){
		LPEXT_GAME_RESULT_SQ lpgrex = LPEXT_GAME_RESULT_SQ((PBYTE)lpRefreshResultEx 
			+ sizeof(REFRESH_RESULT_EX) + i * sizeof(EXT_GAME_RESULT_SQ));
		_stprintf(szBufStr, _T("user%ld = %ld, "), i, lpgrex->nUserID);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("diff%ld = %ld, "), i, lpgrex->nDepositDiff);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("cut%ld = %ld, "), i, lpgrex->nCut);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("fee%ld = %ld, "), i, lpgrex->nFee);
		lstrcat(szLogStr, szBufStr);
		_stprintf(szBufStr, _T("extra%ld = %ld\r\n"), i, lpgrex->nExtra);
		lstrcat(szLogStr, szBufStr);
	}
	UwlLogFile(szLogStr);
}

CString GetUserIDListEx(LPREFRESH_RESULT_EX lpRefreshResultEx )
{
	CString sRet,sTmp;
 	for(int i = 0; i < lpRefreshResultEx->nResultCount; i++)
	{
		LPGAME_RESULT_EX lpgrex = LPGAME_RESULT_EX((PBYTE)lpRefreshResultEx + sizeof(REFRESH_RESULT_EX) + i * sizeof(GAME_RESULT_EX));
		sTmp.Format(_T("%d,"),lpgrex->nUserID);
		sRet+=sTmp;
	}
	return sRet;
}
DWORD GetVerifyFlagsEx(LPREFRESH_RESULT_EX lpRefreshResultEx )
{
	DWORD dwVerifyFlags;
	dwVerifyFlags = 0;
	for(int i = 0; i < lpRefreshResultEx->nResultCount; i++){
		LPGAME_RESULT_EX lpgrex = LPGAME_RESULT_EX((PBYTE)lpRefreshResultEx 
						+ sizeof(REFRESH_RESULT_EX) + i * sizeof(GAME_RESULT_EX));
		if(lpgrex->nDepositDiff){
			dwVerifyFlags |= RF_VERIFY_DEPOSIT;
			break;
		}
	}

	for(i = 0; i < lpRefreshResultEx->nResultCount; i++){
		LPGAME_RESULT_EX lpgrex = LPGAME_RESULT_EX((PBYTE)lpRefreshResultEx 
						+ sizeof(REFRESH_RESULT_EX) + i * sizeof(GAME_RESULT_EX));
		if(lpgrex->nScoreDiff){
			dwVerifyFlags |= RF_VERIFY_SCORE;
			break;
		}
	}

	return dwVerifyFlags;
}

DWORD ExtGetVerifyFlags_SQ(LPREFRESH_RESULT_EX lpRefreshResultEx )
{
	DWORD dwVerifyFlags;
	dwVerifyFlags = 0;
	for(int i = 0; i < lpRefreshResultEx->nResultCount; i++){
		LPEXT_GAME_RESULT_SQ lpgrex = LPEXT_GAME_RESULT_SQ((PBYTE)lpRefreshResultEx 
			+ sizeof(REFRESH_RESULT_EX) + i * sizeof(EXT_GAME_RESULT_SQ));
		if(lpgrex->nDepositDiff){
			dwVerifyFlags |= RF_VERIFY_DEPOSIT;
			break;
		}
	}
	
	for(i = 0; i < lpRefreshResultEx->nResultCount; i++){
		LPEXT_GAME_RESULT_SQ lpgrex = LPEXT_GAME_RESULT_SQ((PBYTE)lpRefreshResultEx 
			+ sizeof(REFRESH_RESULT_EX) + i * sizeof(EXT_GAME_RESULT_SQ));
		if(lpgrex->nScoreDiff){
			dwVerifyFlags |= RF_VERIFY_SCORE;
			break;
		}
	}
	return dwVerifyFlags;
}

BOOL NeedLogGameResultEx(LPREFRESH_RESULT_EX lpRefreshResultEx,   int deposit_logdb)
{
	for(int i = 0; i < lpRefreshResultEx->nResultCount; i++){
		LPGAME_RESULT_EX lpgrex = LPGAME_RESULT_EX((PBYTE)lpRefreshResultEx 
						+ sizeof(REFRESH_RESULT_EX) + i * sizeof(GAME_RESULT_EX));
		if(lpgrex->nDepositDiff >= deposit_logdb){
			return TRUE;
		}
	}
	return FALSE;
}

/*
BOOL CSockServer::OnRefreshResultEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
  	int nRepeated = lpRequest->head.nRepeated;
  
  	LPREFRESH_RESULT_EX lpRefreshResultEx = LPREFRESH_RESULT_EX(PBYTE(lpRequest->pDataPtr) 
									+ nRepeated * sizeof(CONTEXT_HEAD));

 	int nRoomID=lpRefreshResultEx->nRoomID;

  	ROOM room;
 	ZeroMemory(&room,sizeof(room));
	{
		CAutoLock lock(&g_csBaseData);
		FindRoomByID(nRoomID,g_mapRoom,&room);
	}
 	if(room.nRoomID==0)
		return FALSE;

	lpRefreshResultEx->nClientID=room.nGameVID;//借用nClinetID存放nGameVID;


    if(IS_BIT_SET(room.dwConfigs,ROOM_CFG_VERIFYRESULT))
		TransmitRequest(lpContext, lpRequest,CalcKubIndexByGameID(room.nGameID));//发送到kubsvr验证
	else
		OnRefreshResultExFromKub(lpContext,lpRequest,pThreadCxt);
    return TRUE;
 
}
*/

/*
BOOL CSockServer::OnRefreshResultExFromKub(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
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
	LPGAME_RESULT_EX lpGameResultEx1 = LPGAME_RESULT_EX((PBYTE)lpRefreshResultEx + sizeof(REFRESH_RESULT_EX) );

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
		LPGAME_RESULT_EX lpGameResultEx = LPGAME_RESULT_EX((PBYTE)lpRefreshResultEx 
						+ sizeof(REFRESH_RESULT_EX) + i * sizeof(GAME_RESULT_EX));
  
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
	gamedbindex=CalcGameDBIndexByGameID(nGameID);

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
	LPGAME_RESULT_EX lpgrex;
	lpgrex = LPGAME_RESULT_EX((PBYTE)lpRefreshResultEx + sizeof(REFRESH_RESULT_EX));

	if(total_cut < 0 || total_fee < 0 || total_extra < 0){
		LogDepositErrorEx(lpRefreshResultEx, lpgrex);
		nru.nCauseID=RUCID_INVALIDPARAM;
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(total_cut > INT_MAX || total_fee > INT_MAX || total_extra > INT_MAX){
		LogDepositErrorEx(lpRefreshResultEx, lpgrex);
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
				LogDepositErrorEx2(lpRefreshResultEx, lpgrex);
				nru.nCauseID=RUCID_INVALIDPARAM;
				bPureRepeat = TRUE;
				goto SendResponse;

			}
		}
		else
		{
			LogDepositErrorEx(lpRefreshResultEx, lpgrex);
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
		UwlLogFile(_T("OnRefreshResultEx Find UnKnowable IP. GameID=%d, RoomID=%d, GameSvrIP=%s"), nGameID,nRoomID,szIP);
#endif
	}
	if(!sc.bIsValidTimeCost)
	{
		UwlLogFile(_T("OnRefreshResultEx Find InValidTimeCost. GameID=%d, RoomID=%d, UserID= %d, TimeCost=%d, Interval=%d"), nGameID,nRoomID,lpGameResultEx1->nUserID,lpGameResultEx1->nTimeCost,sc.nValidInterval);
	}

  	DWORD dwFlags;
	dwFlags = GetVerifyFlagsEx(lpRefreshResultEx );
 
	
	////////////////////////////////////////////////////////

	nTransStarted = DB_BeginTransEx(errcode, pDBConnectEx, response.head.nRequest);
	if(nTransStarted <= 0){
		nru.nCauseID=RUCID_DATABASEERROR;
 		bPureRepeat = TRUE;
		goto SendResponse;
	}

	for(i = 0; i < lpRefreshResultEx->nResultCount; i++)
	{
		LPGAME_RESULT_EX lpGameResultEx = LPGAME_RESULT_EX((PBYTE)lpRefreshResultEx + sizeof(REFRESH_RESULT_EX) + i * sizeof(GAME_RESULT_EX));
	
		if(lpGameResultEx->nTimeCost<0)
			lpGameResultEx->nTimeCost=0;
		if(lpGameResultEx->nTimeCost > sc.nMaxSalarySecond)
			lpGameResultEx->nTimeCost = sc.nMaxSalarySecond;
 		if(IS_BIT_SET(room.dwManages,ROOM_MNG_NOEXPIFSTANDOFF))
		{
			if(	lpGameResultEx->nStandOff > 0 ) //和局
			{
				lpGameResultEx->nTimeCost=0;
				lpGameResultEx->nExperience=0;
			}
		}
		if(!sc.bIsValidTimeCost)
		{
			lpGameResultEx->nTimeCost=0;
			lpGameResultEx->nExperience=0;
		}

		//////////////////////////////////////////////////////////////////////////////////
		if(IS_BIT_SET(room.dwManages,ROOM_MNG_MATCHONGAME))
		{
			nResult = DB_RefreshMatchResultEx2(errcode, pDBConnectEx, response.head.nRequest, room.nGameDBID, lpGameResultEx,dwFlags);
		}
		else
		{
			if(IS_BIT_SET(sc.dwGameOptions,GAME_OPT_SUPPORTSALARY) && sc.bIsGameSvrIP )
			{
   				sc.nUserType= GetUserType(nGameID,lpGameResultEx->nUserID); 
				if(IS_BIT_SET(sc.dwRoomManages,ROOM_MNG_SALARYBYTIME))
  					sc.nOldSalaryTime=DB_GetSalaryTime (errcode,pDBConnectEx,response.head.nRequest,lpGameResultEx->nUserID,nGameID);
				sc.nBreakOffDiff=lpGameResultEx->nBreakOff;
                sc.nTimeCostDiff=lpGameResultEx->nTimeCost;
 				DB_CalcGameSalary(&sc);
				 
				USERONGAME_DIFF ud;
				memset(&ud,0,sizeof(ud));
				GetUserOnGameDiffEx(&ud,lpGameResultEx,&sc,dwFlags);
				nResult=DB_SetUserOnGameDiff(errcode,pDBConnectEx,response.head.nRequest,&ud);
				if(nResult==0)
				{
					if(ud.nDepositDiff==0)
						UwlLogFile(_T("result save error! GameID=%d,RoomID=%d,UserID= %d ,OldDeposit=%d,DepositDiff=%d,UserIDList=%s"), nGameID, nRoomID,ud.nUserID,ud.nOldDeposit,ud.nDepositDiff,GetUserIDListEx(lpRefreshResultEx));
					else
					{
						int nActualDeposit=DB_GetUserGameDeposit(pDBConnectEx,nGameID,ud.nUserID);
						UwlLogFile(_T("result save error! GameID=%d,RoomID=%d,UserID= %d ,OldDeposit=%d,DepositDiff=%d,ActualDeposit=%d,UserIDList=%s"), nGameID, nRoomID,ud.nUserID,ud.nOldDeposit,ud.nDepositDiff,nActualDeposit,GetUserIDListEx(lpRefreshResultEx));
					}

				}
			}
			else
			{
				nResult = DB_SetGameResultEx(errcode, pDBConnectEx, response.head.nRequest, lpGameResultEx, dwFlags);
				if(nResult==0)
				{
					if(lpGameResultEx->nDepositDiff==0)
						UwlLogFile(_T("result save error! GameID=%d,RoomID=%d,UserID= %d ,OldDeposit=%d,DepositDiff=%d,UserIDList=%s"), nGameID, nRoomID,lpGameResultEx->nUserID,lpGameResultEx->nOldDeposit,lpGameResultEx->nDepositDiff,GetUserIDListEx(lpRefreshResultEx));
					else
					{
						int nActualDeposit=DB_GetUserGameDeposit(pDBConnectEx,nGameID,lpGameResultEx->nUserID);
						UwlLogFile(_T("result save error! GameID=%d,RoomID=%d,UserID= %d ,OldDeposit=%d,DepositDiff=%d,ActualDeposit=%d,UserIDList=%s"), nGameID, nRoomID,lpGameResultEx->nUserID,lpGameResultEx->nOldDeposit,lpGameResultEx->nDepositDiff,nActualDeposit,GetUserIDListEx(lpRefreshResultEx));
					}
				}
			}
		}
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
			CompactRefreshResultEx(lpRefreshResultEx, lpRefreshResult);
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
				if(total_fee >= deposit_logdb || total_cut >= deposit_logdb || total_extra >= deposit_logdb || NeedLogGameResultEx(lpRefreshResultEx, deposit_logdb))
				{
					if(lpRefreshResultEx->nResultCount <= 4)
					{
						gdt.nLogResult=1;
					}
				}
			}
		}		

		///////////////////////////////////////////////////////////////////////
		///////////////////////是否记录游戏结果日志////////////////////////////
		if (IS_BIT_SET(game.dwOptions, GAME_OPT_RESULTLOG))
		{
			//记录游戏结果，目前是为了记录积分
			GAME_RESULT_LOG grl;
			ZeroMemory(&grl,sizeof(GAME_RESULT_LOG));
			memcpy(&grl, &gdt, sizeof(GAME_RESULT_LOG));
			grl.nLogResult = 1;
			NotifyServer(GR_GAME_RESULT_LOG, &grl, sizeof(grl)-sizeof(USER_RESULT_DIFF)*MAX_CHAIR_COUNT+grl.nCount*sizeof(USER_RESULT_DIFF),  CalcKubIndexByGameID( grl.nGameID));
		}

		///////////////////////////////////////////////////////////////////////
 		//////////////发现系统抽取银子,或者需要记录日志,则通知到kubsvr////////// 
		if(!IS_BIT_SET(room.dwManages,ROOM_MNG_MATCHONGAME))
		{
 			if(gdt.nLogResult>0 ||  gdt.nTotalFee>0 || gdt.nTotalCut>0 )
				NotifyServer(GR_GAME_DEPOSIT_TAKEOUT, &gdt, sizeof(gdt)-sizeof(USER_DEPOSIT_DIFF)*MAX_CHAIR_COUNT+gdt.nCount*sizeof(USER_DEPOSIT_DIFF),  CalcKubIndexByGameID( gdt.nGameID));
		}
	//	SaveTimeCostCheck(&gdt); 模板游戏暂时不检查时间

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
	}
 
 	return TRUE;
}
*/

/*
BOOL CSockServer::OnEnterGameEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
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
	
	_ConnectionPtr pDBConnect = NULL;
	_ConnectionPtr pDBConnectEx = NULL;
	if( nGameID<=0 || nGameID >=MAX_GAME_NUM )
	{
		response.head.nRequest = UR_OBJECT_NOT_EXIST;
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
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////
 	gamedbindex=CalcGameDBIndexByGameID(nGameID);

	if(pThreadCxt){
		pDBConnect = pThreadCxt->m_pDBConnectMain;		
		pDBConnectEx = pThreadCxt->m_pDBConnectGame[gamedbindex];
	}
	if(NULL == pDBConnect){
		pThreadCxt->m_bReConnectMain = TRUE; // !!! dont forget !!!
		bPureRepeat = TRUE;
		goto SendResponse;
	}
	if(NULL == pDBConnectEx)
	{
		pThreadCxt->m_bReConnectGame[gamedbindex] = TRUE; // !!! dont forget !!!
		bPureRepeat = TRUE;
		goto SendResponse;
	}

	GAME_RESULT GameResult;
	ZeroMemory(&GameResult, sizeof(GameResult));
	GameResult.nRoomID  = lpEnterGame->nRoomID;
	GameResult.nTableNO = lpEnterGame->nTableNO;
	GameResult.nChairNO = lpEnterGame->nChairNO;
	GameResult.nUserID = nUserID;

	if(IS_BIT_SET(room.dwManages,ROOM_MNG_MATCHONGAME))
	{
		nResult = DB_GetMatchGameResult(errcode, pDBConnectEx, response.head.nRequest, room.nGameDBID,nUserID,   &GameResult);

	}
	else
	{
		nResult = DB_GetGameResult(errcode, pDBConnectEx, response.head.nRequest,  nUserID, nGameID, &GameResult);
	}
	
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

	{
		CAutoLock lock(&g_csBaseData);
		
		LPPLAYERLEVEL pPlayerLevel = MemoGetPlayerLevel(g_mapAllPlayerLevel, nGameID, GameResult.nPlayerLevel);
		if(pPlayerLevel){
			memcpy(GameResult.szLevelName, pPlayerLevel->szLevelName, sizeof(GameResult.szLevelName));
		}
	}
	ENTER_GAME_OK_EX egoe;
	ZeroMemory(&egoe, sizeof(egoe));

	egoe.nUserID		= GameResult.nUserID;				// 用户ID
	egoe.nRoomID		= GameResult.nRoomID;				// 房间ID
	egoe.nTableNO		= GameResult.nTableNO;				// 桌号
	egoe.nChairNO		= GameResult.nChairNO;				// 位置
	egoe.nScore			= GameResult.nScore;				// 积分
	egoe.nDeposit		= GameResult.nDeposit;				// 银子
	egoe.nLevelID		= GameResult.nPlayerLevel;			// 级别ID
	egoe.nExperience    = GameResult.nExperience;
 	egoe.nBout			= GameResult.nBout;
	egoe.nUserType		= GetUserType(nGameID,nUserID);     //UserType
	lstrcpy(egoe.szLevelName, GameResult.szLevelName);      //级别名称

	/////////////////////////////////////////////////////////////////
	int nLen;
	nLen = nRepeated * sizeof(CONTEXT_HEAD) + sizeof(egoe);
	PBYTE pData;
	pData = NULL;
	pData = new BYTE[nLen];

	memcpy(pData, lpRequest->pDataPtr, nRepeated * sizeof(CONTEXT_HEAD));
	memcpy(pData + nRepeated * sizeof(CONTEXT_HEAD), &egoe, sizeof(egoe));

	response.pDataPtr = pData;
	response.nDataLen = nLen;
	response.head.nRequest = GR_ENTER_GAME_OK_EX;

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
	
	if(GR_ENTER_GAME_OK_EX == response.head.nRequest)
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

*/
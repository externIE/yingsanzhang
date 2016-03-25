#include "stdafx.h"

/************************************************************************/
/* 本文件为数据库操作例子示范							                */
/************************************************************************/

/*  添加一条数据
int DB_AddQuan(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPQUAN lpQuan)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	if(lpQuan->nGameID <0 || lpQuan->nGameID >= MAX_GAME_NUM)
		return nResult;
	
	_stprintf(szSql, _T("select top 1 * from tblQuan  ") );
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		pRecordset->CursorLocation = adUseClient;	// so we can obtain autocreated id soon.
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		
		TESTHR(pRecordset->AddNew());
        DBSetQuan(lpQuan,pRecordset);
		TESTHR(pRecordset->Update());
		
		lpQuan->nQuanID = pRecordset->Fields->Item[_T("ID")]->Value.lVal;
		
		nResult = 1;
		
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

void DBSetQuan(LPQUAN lpQuan, _RecordsetPtr& pRecordset)
{
	TCHAR szFieldName[32];
	ZeroMemory(szFieldName,sizeof(szFieldName));
	
	_variant_t varField;
	varField.Clear();
	varField.ChangeType(VT_I4);
	
	varField.lVal = lpQuan->nQuanType;
	pRecordset->Fields->Item[_T("Type")]->Value = varField;
	
	varField.lVal = lpQuan->nSubType;
	pRecordset->Fields->Item[_T("SubType")]->Value = varField;
	
	varField.lVal = lpQuan->nStatus;
	pRecordset->Fields->Item[_T("Status")]->Value = varField;
	
	varField.lVal = lpQuan->dwOptions;
	pRecordset->Fields->Item[_T("Options")]->Value = varField;
	
	varField.lVal = lpQuan->nGameID;
	pRecordset->Fields->Item[_T("GameID")]->Value = varField;
	
	varField.lVal = lpQuan->nRoomID;
	pRecordset->Fields->Item[_T("RoomID")]->Value = varField;
	
	varField.lVal = lpQuan->nTableNO;
	pRecordset->Fields->Item[_T("TableNO")]->Value = varField;
	
	varField.lVal = lpQuan->nCreatorID;
	pRecordset->Fields->Item[_T("CreatorID")]->Value = varField;
	
	varField.lVal = lpQuan->nCreateDate;
	pRecordset->Fields->Item[_T("CreateDate")]->Value = varField;
	
	varField.lVal = lpQuan->nCreateTime;
	pRecordset->Fields->Item[_T("CreateTime")]->Value = varField;
	
	varField.lVal = lpQuan->nUserCount;
	pRecordset->Fields->Item[_T("UserCount")]->Value = varField;
	
	
	for(int i=0;i<lpQuan->nUserCount;i++)
	{
		sprintf(szFieldName,_T("UserID%d"),i);
		varField.lVal = lpQuan->nUserIDs[i];
		pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value = varField;
		
		sprintf(szFieldName,_T("HandNO%d"),i);
		pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value = (_bstr_t)(lpQuan->szHandNOs[i]);
		
	}
	
	pRecordset->Fields->Item[_T("EMGroupID")]->Value = (_bstr_t)(lpQuan->szEMGroupID);
	
	pRecordset->Fields->Item[_T("QuanName")]->Value = (_bstr_t)(lpQuan->szQuanName);
}
*/

/* 查询数据
int DB_FetchOneQuan(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPQUAN lpQuan)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select * from tblQuan where id = %d"),lpQuan->nQuanID);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			DBCopyQuan(lpQuan, pRecordset);
			nResult=1;
		}	
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
		ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

void DBCopyQuan(LPQUAN lpQuan, _RecordsetPtr& pRecordset)
{
	lpQuan->nQuanID   = pRecordset->Fields->Item[_T("ID")]->Value.lVal;
	lpQuan->nQuanType = pRecordset->Fields->Item[_T("Type")]->Value.lVal;
	lpQuan->nSubType  = pRecordset->Fields->Item[_T("SubType")]->Value.lVal;
	lpQuan->nStatus   = pRecordset->Fields->Item[_T("Status")]->Value.lVal;
	lpQuan->dwOptions = pRecordset->Fields->Item[_T("Options")]->Value.lVal;
	lpQuan->nGameID   = pRecordset->Fields->Item[_T("GameID")]->Value.lVal;
   	lpQuan->nRoomID   = pRecordset->Fields->Item[_T("RoomID")]->Value.lVal;
	lpQuan->nTableNO  = pRecordset->Fields->Item[_T("TableNO")]->Value.lVal;
	lpQuan->nCreatorID  = pRecordset->Fields->Item[_T("CreatorID")]->Value.lVal;
	lpQuan->nCreateDate  = pRecordset->Fields->Item[_T("CreateDate")]->Value.lVal;
	lpQuan->nCreateTime  = pRecordset->Fields->Item[_T("CreateTime")]->Value.lVal;
	lpQuan->nUserCount  = pRecordset->Fields->Item[_T("UserCount")]->Value.lVal;
	lpQuan->nLastRoundID = pRecordset->Fields->Item[_T("LastRoundID")]->Value.lVal;
	
	TCHAR szFieldName[32];
	ZeroMemory(szFieldName,sizeof(szFieldName));
	_bstr_t bstrValue ;
	
	for(int i=0;i<lpQuan->nUserCount&&i<MAX_QUANCHAIR_COUNT;i++)
	{
		sprintf(szFieldName,_T("UserID%d"),i);
		lpQuan->nUserIDs[i]  = pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value.lVal;
		
		sprintf(szFieldName,_T("HandNO%d"),i);
		
		bstrValue  = pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value;
		lstrcpy(lpQuan->szHandNOs[i], (LPCTSTR)bstrValue);
		
	}
	
	bstrValue = pRecordset->Fields->Item[_T("EMGroupID")]->Value;
	lstrcpy(lpQuan->szEMGroupID, (LPCTSTR)bstrValue);
	
	bstrValue = pRecordset->Fields->Item[_T("QuanName")]->Value;
	lstrcpy(lpQuan->szQuanName, (LPCTSTR)bstrValue);
}

int DB_FetchUser3(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPLOGON_USER_OK_YP lpLogonUserOK)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select * from tblUser3 where id = %d"),lpLogonUserOK->nUserID);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			_bstr_t bstrValue ;
			
			bstrValue  = pRecordset->Fields->Item[_T("HandNO")]->Value;
			lstrcpy(lpLogonUserOK->szHandNO, (LPCTSTR)bstrValue);
			
			bstrValue  = pRecordset->Fields->Item[_T("ShowName")]->Value;
			lstrcpy(lpLogonUserOK->szShowName, (LPCTSTR)bstrValue);
			
			nResult=1;
		}	
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}
*/

/* 更新数据
int DB_SetClothing(LONG& errcode, _ConnectionPtr& connect, UINT& response, int userid, int clothingid)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	TCHAR szBuf[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("select ClothingID from tblUser "));
	_stprintf(szBuf, _T("where  ID = %d "), userid);
	lstrcat(szSql, szBuf);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile){
			
			_variant_t varField;
			varField.Clear();
			varField.ChangeType(VT_I4);
			
			varField.lVal = clothingid;
			pRecordset->Fields->Item[_T("ClothingID")]->Value = varField;
			
			TESTHR(pRecordset->Update());
			
			nResult = 1;
		}
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); 
		ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_SetUserOnGameDiff(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPUSERONGAME_DIFF lpUserOnGameDiff)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	TCHAR szBuf[MAX_SQL_LENGTH];
	memset(szSql,0,sizeof(szSql));
	memset(szBuf,0,sizeof(szBuf));
	
	_stprintf(szSql, _T("update tblUserOnGame%d "),				 lpUserOnGameDiff->nGameID);
	///////////////////////////////////////////////////////////////////////////////////////////
	_stprintf(szBuf, _T("set PlayerLevel = %d, "),				 lpUserOnGameDiff->nPlayerLevel);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Deposit = Deposit + %d, "),			 lpUserOnGameDiff->nDepositDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Score = Score + %d, "),				 lpUserOnGameDiff->nScoreDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Experience = Experience + %d, "),		 lpUserOnGameDiff->nExperienceDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("BreakOff = BreakOff + %d, "),			 lpUserOnGameDiff->nBreakOffDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Win = Win + %d, "),					 lpUserOnGameDiff->nWinDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Loss = Loss + %d, "),					 lpUserOnGameDiff->nLossDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("StandOff = StandOff + %d, "),			 lpUserOnGameDiff->nStandOffDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Bout = Bout + %d, "),					 lpUserOnGameDiff->nBoutDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("TimeCost = TimeCost + %d, "),			 lpUserOnGameDiff->nTimeCostDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("SalaryTime = SalaryTime + %d, "),		 lpUserOnGameDiff->nSalaryTimeDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("SalaryDeposit = SalaryDeposit + %d, "), lpUserOnGameDiff->nSalaryDepositDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("TotalSalary = TotalSalary + %d "),      lpUserOnGameDiff->nSalaryDepositDiff);
	lstrcat(szSql, szBuf);
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	if(lpUserOnGameDiff->nGameID==421)//针对13水这个游戏特别处理
	{
		int nDivTimeCost=lpUserOnGameDiff->nReserved[0];
        if(nDivTimeCost >0 && nDivTimeCost<=lpUserOnGameDiff->nTimeCostDiff)
		{
			_stprintf(szBuf, _T(", DivBout = DivBout + %d, DivTimeCost = DivTimeCost + %d "), lpUserOnGameDiff->nBoutDiff,nDivTimeCost);
			lstrcat(szSql, szBuf);
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	_stprintf(szBuf, _T("where UserID = %d "),                   lpUserOnGameDiff->nUserID);
	lstrcat(szSql, szBuf);
	
	
	if(IS_BIT_SET(lpUserOnGameDiff->dwVerifyFlag, RESULT_FLAG_DB_DEPOSIT))
	{
		_stprintf(szBuf, _T("and Deposit = %d "),                lpUserOnGameDiff->nOldDeposit);
		lstrcat(szSql, szBuf);
		_stprintf(szBuf, _T("and Deposit + %d >= 0 "),           lpUserOnGameDiff->nDepositDiff);
		lstrcat(szSql, szBuf);
	}
	
	UwlTraceSql(szSql);
	try{
		_variant_t varRows;
		TESTHR(connect->Execute(szSql, &varRows, 
			adCmdText | adExecuteNoRecords));
		nResult = varRows.lVal;
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
	
}
*/


/* 调用存储过程
int DBSP_CheckUserOnGame(LONG& errcode, _ConnectionPtr& connect, UINT& response,  int userid, int gameid,int score,int deposit,int& effected)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	
	//VARIANT var;
    SAFEARRAY *psa = NULL;
	
    SAFEARRAYBOUND rgsabound[1];
    rgsabound[0].lLbound = 0;
    rgsabound[0].cElements = 4;
    psa = SafeArrayCreate(VT_VARIANT, 1, rgsabound);
	
	_variant_t vtParamValue;
	long iIndex[1] ;
	// userid
	iIndex[0] = 0;
	vtParamValue.vt = VT_INT;
	vtParamValue.lVal = userid;
	SafeArrayPutElement(psa, iIndex, &vtParamValue);
	// gameid
	iIndex[0] = 1;
	vtParamValue.vt = VT_INT;
	vtParamValue.lVal = gameid;
	SafeArrayPutElement(psa, iIndex, &vtParamValue);
	// score
	iIndex[0] = 2;
	vtParamValue.vt = VT_INT;
	vtParamValue.lVal = score;
	SafeArrayPutElement(psa, iIndex, &vtParamValue);
	
	// deposit
	iIndex[0] = 3;
	vtParamValue.vt = VT_INT;
	vtParamValue.lVal = deposit;
	SafeArrayPutElement(psa, iIndex, &vtParamValue);
	
	if(DBSP_Execute2(errcode, connect, response, _T("uspCheckUserOnGame"), psa,effected)){
		response = UR_OBJECT_EXIST;
		nResult = 1;
	}
	SafeArrayDestroy(psa);
	return nResult;
}
*/

///* 乱七八糟的例子
/*
int DB_SetGameResultEx(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPGAME_RESULT_EX lpGameResult, DWORD dwFlags)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	TCHAR szBuf[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("update tblUserOnGame%d "), lpGameResult->nGameID);
	_stprintf(szBuf, _T("set PlayerLevel = %d, "), lpGameResult->nLevelID);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Deposit = Deposit + %d, "), lpGameResult->nDepositDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Score = Score + %d, "), lpGameResult->nScoreDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Experience = Experience + %d, "), lpGameResult->nExperience);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("BreakOff = BreakOff + %d, "), lpGameResult->nBreakOff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Win = Win + %d, "), lpGameResult->nWin);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Loss = Loss + %d, "), lpGameResult->nLoss);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("StandOff = StandOff + %d, "), lpGameResult->nStandOff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Bout = Bout + %d, "), lpGameResult->nBout);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("TimeCost = TimeCost + %d "), lpGameResult->nTimeCost);
	lstrcat(szSql, szBuf);
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	if(lpGameResult->nGameID==421)//针对13水这个游戏特别处理
	{
		int nDivTimeCost=lpGameResult->nReserved[0];
        if(nDivTimeCost >0 && nDivTimeCost<=lpGameResult->nTimeCost)
		{
			_stprintf(szBuf, _T(", DivBout = DivBout + %d, DivTimeCost = DivTimeCost + %d "), lpGameResult->nBout,nDivTimeCost);
			lstrcat(szSql, szBuf);
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
 	_stprintf(szBuf, _T("where UserID = %d "), lpGameResult->nUserID);
	lstrcat(szSql, szBuf);

	if(IS_BIT_SET(dwFlags, RF_VERIFY_DEPOSIT)){
		_stprintf(szBuf, _T("and Deposit = %d "), lpGameResult->nOldDeposit);
		lstrcat(szSql, szBuf);
		_stprintf(szBuf, _T("and Deposit + %d >= 0 "), lpGameResult->nDepositDiff);
		lstrcat(szSql, szBuf);
	}

	UwlTraceSql(szSql);
	try{
		_variant_t varRows;
		TESTHR(connect->Execute(szSql, &varRows, 
							adCmdText | adExecuteNoRecords));
		nResult = varRows.lVal;
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
}


int DB_RefreshMatchResultEx2(LONG& errcode, _ConnectionPtr& connect, UINT& response, int nGameDBID,LPGAME_RESULT_EX lpGameResult, DWORD dwFlags)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	TCHAR szBuf[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("update tblMatchOnGame%d "), nGameDBID);
	_stprintf(szBuf, _T("set PlayerLevel = %d, "), lpGameResult->nLevelID);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Deposit = Deposit + %d, "), lpGameResult->nDepositDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Score = Score + %d, "), lpGameResult->nScoreDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Experience = Experience + %d, "), lpGameResult->nExperience);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("BreakOff = BreakOff + %d, "), lpGameResult->nBreakOff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Win = Win + %d, "), lpGameResult->nWin);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Loss = Loss + %d, "), lpGameResult->nLoss);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("StandOff = StandOff + %d, "), lpGameResult->nStandOff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Bout = Bout + %d, "), lpGameResult->nBout);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("TimeCost = TimeCost + %d "), lpGameResult->nTimeCost);
	lstrcat(szSql, szBuf);
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	if(lpGameResult->nGameID==421)//针对13水这个游戏特别处理
	{
		int nDivTimeCost=lpGameResult->nReserved[0];
        if(nDivTimeCost >0 && nDivTimeCost<=lpGameResult->nTimeCost)
		{
			_stprintf(szBuf, _T(", DivBout = DivBout + %d, DivTimeCost = DivTimeCost + %d "), lpGameResult->nBout,nDivTimeCost);
			lstrcat(szSql, szBuf);
		}
	} 
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
  	_stprintf(szBuf, _T("where UserID = %d "), lpGameResult->nUserID);
	lstrcat(szSql, szBuf);
	if(IS_BIT_SET(dwFlags, RF_VERIFY_DEPOSIT)){
		_stprintf(szBuf, _T("and Deposit = %d "), lpGameResult->nOldDeposit);
		lstrcat(szSql, szBuf);
		_stprintf(szBuf, _T("and Deposit + %d >= 0 "), lpGameResult->nDepositDiff);
		lstrcat(szSql, szBuf);
	}

	UwlTraceSql(szSql);
	try{
		_variant_t varRows;
		TESTHR(connect->Execute(szSql, &varRows, 
							adCmdText | adExecuteNoRecords));
		nResult = varRows.lVal;
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
}


int DB_GetSalaryTime(LONG& errcode, _ConnectionPtr& connect, UINT& response,  int userid, int gameid)
{
  	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
 	_stprintf(szSql, _T("select SalaryTime from tblUserOnGame%d where UserID = %d "), gameid,userid);
 	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile){
			
			nResult = pRecordset->Fields->Item[_T("SalaryTime")]->Value.lVal;
			if(nResult<0)
				nResult=0;
			if(nResult>59)
				nResult=59;
		}
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = 0;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
}

int DB_FetchUserGameDeposit(LONG& errcode,_ConnectionPtr& connect, UINT& response,int nUserID,int nGameID,int& nDeposit)
{
   	int nResult = 0;
  	
	response = UR_OBJECT_NOT_EXIST;
 	TCHAR szSql[MAX_SQL_LENGTH];
 	_stprintf(szSql, _T("select Deposit from tblUserOnGame%d where UserID=%d "), nGameID, nUserID);
  	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
  			nDeposit= pRecordset->Fields->Item[_T("Deposit")]->Value.lVal;
			nResult=1;
		}
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	}
 	return nResult;
}

int DB_GetSecurePwd(LONG& errcode, _ConnectionPtr& connect, UINT& response, int userid, LPTSTR securepwd)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	TCHAR szBuf[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("select SecurePwd from tblUser "));
	_stprintf(szBuf, _T("where ID = %d "), userid);
	lstrcat(szSql, szBuf);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile){
			_bstr_t bstrSecurePwd = pRecordset->Fields->Item[_T("SecurePwd")]->Value;
			lstrcpy(securepwd, LPCTSTR(bstrSecurePwd));
			nResult = 1;
		}
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
}


int DB_MoveDepositFromSafeEx(LONG& errcode, _ConnectionPtr& connectFrom, _ConnectionPtr& connectTo, UINT& response, int userid, int gameid, int deposit)
{
	int nResult = 0;
	nResult = DB_MinusSafeDeposit(errcode, connectFrom, response, userid,deposit);
 
	if(0 < nResult){
		return DB_AddGameDeposit(errcode, connectTo, response, userid,gameid,deposit);
			 
	}else if(0 == nResult){
		response = GR_DEPOSIT_NOTENOUGH;
	}
	return nResult;
}


int DB_MoveDepositToSafeEx(LONG& errcode, _ConnectionPtr& connectFrom, _ConnectionPtr& connectTo, UINT& response, int userid, int gameid, int deposit)
{
	int nResult = 0;
	nResult = DB_MinusGameDeposit(errcode, connectFrom, response, userid,gameid,deposit);
						 
	if(0 < nResult){
		return DB_AddSafeDeposit(errcode, connectTo, response, userid,deposit);
			 
	}else if(0 == nResult){
		response = GR_DEPOSIT_NOTENOUGH;
	}
	return nResult;
}
int DB_MinusSafeDeposit(LONG& errcode, _ConnectionPtr& connect, UINT& response, int userid, int deposit)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	if(deposit<0)
		return nResult;

	TCHAR szSql[MAX_SQL_LENGTH];
	TCHAR szBuf[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("update tblUser "));
	_stprintf(szBuf, _T("set Deposit = Deposit - %d "), deposit);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("where ID = %d "), userid);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("and Deposit >= %d "), deposit);
	lstrcat(szSql, szBuf);
	UwlTraceSql(szSql);
	try{
		_variant_t varRows;
		TESTHR(connect->Execute(szSql, &varRows, 
							adCmdText | adExecuteNoRecords));
		nResult = varRows.lVal;
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
}


int DB_MinusGameDeposit(LONG& errcode, _ConnectionPtr& connect, UINT& response, int userid, int gameid, int deposit)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;

	if(deposit<0)
		return nResult;

	TCHAR szSql[MAX_SQL_LENGTH];
	TCHAR szBuf[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("update tblUserOnGame%d "), gameid);
	_stprintf(szBuf, _T("set Deposit = Deposit - %d "), deposit);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("where UserID = %d "), userid);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("and Deposit >= %d "), deposit);
	lstrcat(szSql, szBuf);
	UwlTraceSql(szSql);
	try{
		_variant_t varRows;
		TESTHR(connect->Execute(szSql, &varRows, 
							adCmdText | adExecuteNoRecords));
		nResult = varRows.lVal;
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
}
int DB_AddGameDeposit(LONG& errcode, _ConnectionPtr& connect, UINT& response, int userid, int gameid, int deposit)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;

	if(deposit<0)
		return nResult;

	TCHAR szSql[MAX_SQL_LENGTH];
	TCHAR szBuf[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("update tblUserOnGame%d "), gameid);
	_stprintf(szBuf, _T("set Deposit = Deposit + %d "), deposit);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("where UserID = %d "), userid);
	lstrcat(szSql, szBuf);
	UwlTraceSql(szSql);
	try{
		_variant_t varRows;
		TESTHR(connect->Execute(szSql, &varRows, 
							adCmdText | adExecuteNoRecords));
		nResult = varRows.lVal;
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
}

int DB_AddSafeDeposit(LONG& errcode, _ConnectionPtr& connect, UINT& response, int userid, int deposit)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;

	if(deposit<0)
		return nResult;

	TCHAR szSql[MAX_SQL_LENGTH];
	TCHAR szBuf[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("update tblUser "));
	_stprintf(szBuf, _T("set Deposit = Deposit + %d "), deposit);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("where ID = %d "), userid);
	lstrcat(szSql, szBuf);
	UwlTraceSql(szSql);
	try{
		_variant_t varRows;
		TESTHR(connect->Execute(szSql, &varRows, 
							adCmdText | adExecuteNoRecords));
		nResult = varRows.lVal;
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
}

int DB_LookSafeDeposit(LONG& errcode, _ConnectionPtr& connect, UINT& response, int nUserID,int& nDeposit,BOOL& bHaveSecurePwd)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
 	_stprintf(szSql, _T("select Deposit,SecurePwd from tblUser where ID = %d "),nUserID);
 	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile){
			nDeposit = pRecordset->Fields->Item[_T("Deposit")]->Value.lVal;
			_bstr_t bstrSecurePwd = pRecordset->Fields->Item[_T("SecurePwd")]->Value;
			bHaveSecurePwd=lstrlen(LPCTSTR(bstrSecurePwd));
			nResult = 1;
		}
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
}

void SetAuanFromAddQuan(LPQUAN lpQuan, LPADD_QUAN lpAddQuan)
{
	lpQuan->nGameID=lpAddQuan->nGameID;
	lpQuan->nCreatorID=lpAddQuan->nUserID;
	lpQuan->nUserCount=lpAddQuan->nUserCount;
 	UwlGetCurrentDateTime(lpQuan->nCreateDate, lpQuan->nCreateTime);

	for(int i=0;i<lpQuan->nUserCount;i++)
	{
		lstrcpyn(lpQuan->szHandNOs[i],lpAddQuan->szHandNOs[i],MAX_HANDNO_LEN);

	}
	if(IS_BIT_SET(lpAddQuan->dwFlags,FLAG_ADDQUAN_INCLUDESELF))
		lpQuan->nUserIDs[0]=lpAddQuan->nUserID;

	lstrcpyn(lpQuan->szEMGroupID, lpAddQuan->szEMGroupID, MAX_EMGROUP_LEN);
	lpQuan->szEMGroupID[MAX_EMGROUP_LEN-1]=0;

	lstrcpyn(lpQuan->szQuanName, lpAddQuan->szQuanName, MAX_QUANNAME_LEN);
	lpQuan->szQuanName[MAX_QUANNAME_LEN-1]=0;
}
void SetAuanUserFromAddQuan(LPQUAN_USER lpQuanUser, LPADD_QUAN lpAddQuan)
{
	lpQuanUser->nUserID=lpAddQuan->nUserID;
	lpQuanUser->nQuanID=lpAddQuan->nQuanID;
	lpQuanUser->nUserCount=lpAddQuan->nUserCount;
 
	lstrcpyn(lpQuanUser->szQuanName,lpAddQuan->szQuanName,MAX_QUANNAME_LEN);

	for(int i=0;i<lpQuanUser->nUserCount;i++)
	{
		lstrcpyn(lpQuanUser->szShowNames[i],lpAddQuan->szShowNames[i],MAX_USERNAME_LEN);

	}
 
}

 
void SetAuanUserFromAddQuanUser(LPQUAN_USER lpQuanUser, LPADD_QUANUSER lpAddQuanUser)
{
	lpQuanUser->nUserID=lpAddQuanUser->nUserID;
	lpQuanUser->nQuanID=lpAddQuanUser->nQuanID;
	lpQuanUser->nUserCount=lpAddQuanUser->nUserCount;
 
	lstrcpyn(lpQuanUser->szQuanName,lpAddQuanUser->szQuanName,MAX_QUANNAME_LEN);

	for(int i=0;i<lpQuanUser->nUserCount;i++)
	{
		lstrcpyn(lpQuanUser->szShowNames[i],lpAddQuanUser->szShowNames[i],MAX_USERNAME_LEN);

	}
}

void SetQuanRoomFromRoom(LPQUANROOM lpQuanRoom,LPROOM lpRoom)
{
	lpQuanRoom->nAreaID =lpRoom->nAreaID;
	lpQuanRoom->nRoomPort=lpRoom->nPort;
	lpQuanRoom->nGamePort=lpRoom->nGamePort;
	lpQuanRoom->dwRoomOptions=lpRoom->dwOptions;
	lpQuanRoom->dwRoomConfigs=lpRoom->dwConfigs;
	lpQuanRoom->dwRoomManages=lpRoom->dwManages;
	lpQuanRoom->dwGameOptions=lpRoom->dwGameOptions;
	lpQuanRoom->nHallBuildNO=lpRoom->nHallBuildNO; 
	lpQuanRoom->nExeMajorVer=lpRoom->nExeMajorVer;
	lpQuanRoom->nExeMinorVer=lpRoom->nExeMinorVer;
	lstrcpy(lpQuanRoom->szGameIP,lpRoom->szGameIP);
	lstrcpy(lpQuanRoom->szExeName,lpRoom->szExeName);
	lpQuanRoom->nGameVID =lpRoom->nGameVID;
	
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//DB operation
void DBSetRegUserYP(_RecordsetPtr& recordset, LPREG_USER_YP lpRegUser)
{
	//////////////////注册预处理//////////////////////////
 	lpRegUser->nPortrait=(lpRegUser->nNickSex==0 ? 0:105);
	lpRegUser->nClothingID=(lpRegUser->nNickSex==0 ? DEF_MALE_CLOTHING:DEF_FEMALE_CLOTHING);
 	lpRegUser->nUserType=USER_TYPE_COMMON;	 
	lpRegUser->nSubType=0;			 
  	/////////////////////////////////////////////////////

	_variant_t varField;
	varField.Clear();
	varField.ChangeType(VT_I4);
 
	varField.lVal = lpRegUser->nGiftDeposit;
	recordset->Fields->Item[_T("Deposit")]->Value = varField;

	varField.lVal = lpRegUser->nNickSex;
	recordset->Fields->Item[_T("NickSex")]->Value = varField;

	varField.lVal = lpRegUser->nPortrait;
	recordset->Fields->Item[_T("Portrait")]->Value = varField;

 	varField.lVal = lpRegUser->nClothingID;
	recordset->Fields->Item[_T("ClothingID")]->Value = varField;
 
 
	varField.lVal = lpRegUser->nUserType;
	recordset->Fields->Item[_T("Type")]->Value = varField;

	varField.lVal = lpRegUser->nSubType;
	recordset->Fields->Item[_T("SubType")]->Value = varField;

	varField.lVal = lpRegUser->dwIPAddr;
	recordset->Fields->Item[_T("RegIP")]->Value = varField;
 	 
	varField.lVal = lpRegUser->nAgentGroupID;
	recordset->Fields->Item[_T("DownloadGroup")]->Value = varField;

 	varField.lVal =PLATFORMID_YUEPAI;
 	recordset->Fields->Item[_T("RegFrom")]->Value = varField;
 	
	recordset->Fields->Item[_T("Name")]->Value = (_bstr_t)(lpRegUser->szUsername);
	recordset->Fields->Item[_T("RegMachineID")]->Value = (_bstr_t)(lpRegUser->szImeiID);

	TCHAR szHandPhone[MAX_TELNO_LEN];
	ZeroMemory(szHandPhone,sizeof(szHandPhone));
	lstrcpyn(szHandPhone,lpRegUser->szHandNO,MAX_TELNO_LEN);
 	recordset->Fields->Item[_T("HandPhone")]->Value = (_bstr_t)(szHandPhone);

	TCHAR szNickName[MAX_NICKNAME_LEN];
 	ZeroMemory(szNickName,sizeof(szNickName));
	lstrcpyn(szNickName,lpRegUser->szShowName,MAX_NICKNAME_LEN);
 	recordset->Fields->Item[_T("NickName")]->Value = (_bstr_t)(szNickName);

	int createday,createtime;
 	UwlGetCurrentDateTime(createday, createtime);
	varField.lVal = createday;
	recordset->Fields->Item[_T("CreateDay")]->Value = varField;
	varField.lVal = createtime;
	recordset->Fields->Item[_T("CreateTime")]->Value = varField;

	//密码改为加密存放
 
	CString sPwd=MD5Pwd2(lpRegUser->szPassword );
	recordset->Fields->Item[_T("Password")]->Value = (_bstr_t)sPwd;
	recordset->Fields->Item[_T("RegPwd")]->Value = (_bstr_t)sPwd;

}

int DB_SaveUser3(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPREG_USER_YP lpRegUser )
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
  
	TCHAR szSql[MAX_SQL_LENGTH];
 	_stprintf(szSql, _T("select * from tblUser3 where ID = %d  "),lpRegUser->nUserID);
 	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		pRecordset->CursorLocation = adUseClient;	// so we can obtain autocreated id soon.
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		if(pRecordset->EndOfFile){
			TESTHR(pRecordset->AddNew());
	
		
			_variant_t varField;
			varField.Clear();
			varField.ChangeType(VT_I4);
 
			varField.lVal = lpRegUser->nUserID;
			pRecordset->Fields->Item[_T("ID")]->Value = varField;
 
			pRecordset->Fields->Item[_T("HandNO")]->Value = (_bstr_t)(lpRegUser->szHandNO);
			pRecordset->Fields->Item[_T("ShowName")]->Value = (_bstr_t)(lpRegUser->szShowName);

 			TESTHR(pRecordset->Update());
    			 
			nResult = 1;
		}else{
			response = UR_OBJECT_EXIST;
		}
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_CreateUserYP(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPREG_USER_YP lpRegUser )
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;

 	TCHAR szSql[MAX_SQL_LENGTH];
 	_stprintf(szSql, _T("select * from tblUser where Name = '%s' and RegisterGroup = %d "),lpRegUser->szUsername,DEF_REGISTER_GROUP);
 	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		pRecordset->CursorLocation = adUseClient;	// so we can obtain autocreated id soon.
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		if(pRecordset->EndOfFile){
			TESTHR(pRecordset->AddNew());

			DBSetRegUserYP(pRecordset, lpRegUser);
		
			TESTHR(pRecordset->Update());
		 
			lpRegUser->nUserID = pRecordset->Fields->Item[_T("ID")]->Value.lVal;
			 
			nResult = 1;
		}else{
			response = UR_OBJECT_EXIST;
		}
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_FetchUser3(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPLOGON_USER_OK_YP lpLogonUserOK)
{
	int nResult = 0;
 	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select * from tblUser3 where id = %d"),lpLogonUserOK->nUserID);
  	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			_bstr_t bstrValue ;

 			bstrValue  = pRecordset->Fields->Item[_T("HandNO")]->Value;
 			lstrcpy(lpLogonUserOK->szHandNO, (LPCTSTR)bstrValue);
		
			bstrValue  = pRecordset->Fields->Item[_T("ShowName")]->Value;
 			lstrcpy(lpLogonUserOK->szShowName, (LPCTSTR)bstrValue);

			nResult=1;
		}	
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_VerifyUserYP(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPLOGON_USER lpLogonUser, LPLOGON_USER_OK lpLogonUserOK)
{
 	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;

 	TCHAR szSql[MAX_SQL_LENGTH];
 	_stprintf(szSql, _T("select * from tblUser where Name = '%s' and RegisterGroup = 1 "),lpLogonUser->szUsername);
 	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile){
			TCHAR szPassword[MAX_PASSWORD_LEN];
			memset(szPassword,0,sizeof(szPassword));
			_bstr_t bstrPassword = pRecordset->Fields->Item[_T("Password")]->Value;
			lstrcpy(szPassword, (LPCTSTR)bstrPassword);
			BOOL bPwdRight = FALSE;

			int nUsertype = pRecordset->Fields->Item[_T("Type")]->Value.lVal;
			if(IsMD5Pwd(szPassword))
			{
				//第三方帐号登录
				if (IS_BIT_SET(nUsertype, USER_TYPE_THIRDUSER))
				{
					CString sPwd2;
					if (IsMD5Pwd(lpLogonUser->szPassword))
						sPwd2.Format(_T("%s"), lpLogonUser->szPassword);	//密文登录
					else
						sPwd2=MD5Pwd2(lpLogonUser->szPassword);				//明文登录

					if(!lstrcmp(szPassword, sPwd2))
						bPwdRight = TRUE;
				}
				else
				{
					CString sPwd2;
					if(lpLogonUser->szPassword[0]==0 && lpLogonUser->szHashPwd[0]!=0)// 键盘保护
						sPwd2=MD5MD5Pwd(lpLogonUser->szHashPwd);
					else//客户端明码发送
						sPwd2=MD5Pwd2(lpLogonUser->szPassword);
					
					if(!lstrcmp(szPassword, sPwd2))
						bPwdRight = TRUE;
				}
			}
			else
			{
 				if(lpLogonUser->szPassword[0]==0 && lpLogonUser->szHashPwd[0]!=0)// 键盘保护
				{ 
			        CString sPwd1=MD5Pwd1(szPassword);
 					if(!lstrcmpi(sPwd1, lpLogonUser->szHashPwd))
					{
						bPwdRight = TRUE;
					}
				}
				else//客户端明码发送
				{ 
					if(!lstrcmp(szPassword, lpLogonUser->szPassword))
					{ 
						bPwdRight = TRUE;
					}
				}
			}

			if(!bPwdRight)// password mismatch
			{ 
				response = UR_PASSWORD_WRONG;
			}
			else
			{
				lstrcpy(lpLogonUser->szPassword,szPassword); //
			
				lpLogonUserOK->nUserID = pRecordset->Fields->Item[_T("ID")]->Value.lVal;
 				lpLogonUser->nUserID = lpLogonUserOK->nUserID;//保存userid

 				int nStatus = pRecordset->Fields->Item[_T("Status")]->Value.lVal;
				if(IS_BIT_SET(nStatus,USER_STATUS_FORBIDDEN))// already forbidden
				{ 
					response = GR_USER_FORBIDDEN;
  					int nForbidExpiration = pRecordset->Fields->Item[_T("ForbidExpiration")]->Value.lVal;
					if(nForbidExpiration>0)//==0表示永久禁用,>0表示有日期
					{
  						if(UwlGetCurrentSeconds()>nForbidExpiration) //到期了
						{
							_variant_t varField;
							varField.Clear();
							varField.ChangeType(VT_I4);
  							varField.lVal = nStatus & ~USER_STATUS_FORBIDDEN ;
							pRecordset->Fields->Item[_T("Status")]->Value = varField;
   							TESTHR(pRecordset->Update());
							response = GR_FORBID_EXPIRATION;
						}
						else //未到期
						{
							response=GR_FORBID_UNEXPIRATION;
							lpLogonUserOK->nForbidExpiration =nForbidExpiration;

						}
					}
				}
				else
				{
					if(IS_BIT_SET( nStatus,USER_STATUS_LOCKED) )// already locked
					{ 
						response = GR_USER_LOCKED;
					}
					else
					{

						TCHAR szHardID[MAX_HARDID_LEN];
						ZeroMemory(szHardID,sizeof(szHardID));
 						_bstr_t bstrValue = pRecordset->Fields->Item[_T("HardID")]->Value;
						lstrcpy(szHardID, (LPCTSTR)bstrValue);
				
						TCHAR szRegMachineID[MAX_HARDID_LEN];
						ZeroMemory(szRegMachineID,sizeof(szRegMachineID));
						bstrValue = pRecordset->Fields->Item[_T("RegMachineID")]->Value;
						lstrcpy(szRegMachineID, (LPCTSTR)bstrValue);

						lpLogonUserOK->nOnRegMachine =(lstrcmp(szRegMachineID,lpLogonUser->szMachineID)==0);
 						lpLogonUserOK->nHaveBind=lstrlen(szHardID);
				 
				 
  						lpLogonUserOK->nNickSex = pRecordset->Fields->Item[_T("NickSex")]->Value.lVal;
						lpLogonUserOK->nPortrait = pRecordset->Fields->Item[_T("Portrait")]->Value.lVal;
						if(lpLogonUserOK->nPortrait==PORTRAITID_NOTHING || lpLogonUserOK->nPortrait==PORTRAITID_VIP)
						  	lpLogonUserOK->nPortrait =0;

						lpLogonUserOK->nUserType = pRecordset->Fields->Item[_T("Type")]->Value.lVal;
						lpLogonUserOK->nSubType = pRecordset->Fields->Item[_T("SubType")]->Value.lVal;
 						lpLogonUserOK->nClothingID = pRecordset->Fields->Item[_T("ClothingID")]->Value.lVal;
						lpLogonUserOK->nRegisterGroup = pRecordset->Fields->Item[_T("RegisterGroup")]->Value.lVal;
						lpLogonUserOK->nDownloadGroup = pRecordset->Fields->Item[_T("DownloadGroup")]->Value.lVal;
						lpLogonUserOK->nExpiration = pRecordset->Fields->Item[_T("Expiration")]->Value.lVal;
						lpLogonUserOK->nPlayRoom = pRecordset->Fields->Item[_T("PlayRoom")]->Value.lVal;
				 		lpLogonUserOK->nRegFrom = pRecordset->Fields->Item[_T("RegFrom")]->Value.lVal;

						_bstr_t bstrText = pRecordset->Fields->Item[_T("NickName")]->Value;
						lstrcpy(lpLogonUserOK->szNickName, (LPCTSTR)bstrText);

 						nResult = 1;
						
					}
				}
			}
		}
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_AddQuan(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPQUAN lpQuan)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
 	if(lpQuan->nGameID <0 || lpQuan->nGameID >= MAX_GAME_NUM)
		return nResult;

	_stprintf(szSql, _T("select top 1 * from tblQuan  ") );
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		pRecordset->CursorLocation = adUseClient;	// so we can obtain autocreated id soon.
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		
		TESTHR(pRecordset->AddNew());
        DBSetQuan(lpQuan,pRecordset);
		TESTHR(pRecordset->Update());
			
		lpQuan->nQuanID = pRecordset->Fields->Item[_T("ID")]->Value.lVal;

		nResult = 1;
		
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_AddQuanUser(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPQUAN_USER lpQuanUser)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
 
	_stprintf(szSql, _T("select top 1 * from tblQuanUser  ") );
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		pRecordset->CursorLocation = adUseClient;	// so we can obtain autocreated id soon.
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		
		TESTHR(pRecordset->AddNew());
        DBSetQuanUser(lpQuanUser,pRecordset);
		TESTHR(pRecordset->Update());
			
		lpQuanUser->nID = pRecordset->Fields->Item[_T("ID")]->Value.lVal;

		nResult = 1;
		
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_AddQuanHand(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPQUAN lpQuan)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
 
	_stprintf(szSql, _T("select top 1 * from tblQuanHand  ") );
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		pRecordset->CursorLocation = adUseClient;	// so we can obtain autocreated id soon.
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		
		for(int i=0;i<lpQuan->nUserCount;i++)
		{
			TESTHR(pRecordset->AddNew());
			DBSetQuanHand(lpQuan->nQuanID ,lpQuan->szHandNOs[i],pRecordset);
		}
 		TESTHR(pRecordset->Update());

		nResult = 1;
		
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;

}

int DB_FetchQuansID(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPCTSTR lpszHandNO, CIDMap& mapQuanID)
{	
	int nResult = 0;
	
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
 	
	sprintf(szSql, _T("select  QuanID from  tblQuanHand where HandNO='%s' "),lpszHandNO);
 	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		while(FALSE == pRecordset->EndOfFile){

			int quanid = pRecordset->Fields->Item[_T("QuanID")]->Value.lVal;
			mapQuanID.SetAt(quanid,quanid);
					
			TESTHR(pRecordset->MoveNext());
		}
		TESTHR(pRecordset->Close());
		nResult = mapQuanID.GetCount();
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
} 

int DB_FillUserIDToQuan(LONG& errcode, _ConnectionPtr& connect, UINT& response,int nQuanID, LPCTSTR lpszHandNO,int nUserID)
{
	int nResult = 0;
 	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select * from tblQuan where ID = %d "),nQuanID);
  	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			TCHAR szFieldName[32];
			ZeroMemory(szFieldName,sizeof(szFieldName));
			_bstr_t bstrValue ;
			int nUserCount   = pRecordset->Fields->Item[_T("UserCount")]->Value.lVal;

			for(int i=0;i<nUserCount;i++)
			{
 				sprintf(szFieldName,_T("HandNO%d"),i);
 
				bstrValue  = pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value;
 				if(lstrcmp(lpszHandNO, (LPCTSTR)bstrValue)==0)//手机号相同
				{
					sprintf(szFieldName,_T("UserID%d"),i);
					int nOldUserID=	pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value.lVal;	
					if(nOldUserID==0)
					{
						_variant_t varField;
						varField.Clear();
						varField.ChangeType(VT_I4);
  
						varField.lVal = nUserID;
						pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value = varField;

						TESTHR(pRecordset->Update());
 						nResult = 1;
						break;
					}
				}
			}
		}	
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_YPSetPortrait(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPYP_USER lpYPUser)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("select * from tblUser3 where ID = %d "), lpYPUser->nID);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		pRecordset->CursorLocation = adUseClient;	// so we can obtain autocreated id soon.
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			pRecordset->Fields->Item[_T("ProtraitFile")]->Value = (_bstr_t)(lpYPUser->szPortrait);
			TESTHR(pRecordset->Update());
			nResult=1;
		}	
		TESTHR(pRecordset->Close());

	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_FetchPortrait(LONG& errcode, _ConnectionPtr& connect, UINT& response, int nUserID, YP_PORTRAIT& portrait)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	_bstr_t bstrValue ;

	_stprintf(szSql, _T("select * from tblUser3 where ID = %d "), nUserID);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			bstrValue  = pRecordset->Fields->Item[_T("ProtraitFile")]->Value;
 			lstrcpy(portrait.szPortrait, (LPCTSTR)bstrValue);
			nResult=1;
		}	
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}


int DB_FetchRep(LONG& errcode, _ConnectionPtr& connect, UINT& response, YP_REP& rep, LPVOID* ppRep)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	_bstr_t bstrValue ;
	
	_stprintf(szSql, _T("select * from tblYuePaiRep where ID = %d "), rep.nID);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			bstrValue  = pRecordset->Fields->Item[_T("Name")]->Value;
 			lstrcpy(rep.szName, (LPCTSTR)bstrValue);

			nResult = DBGetFieldChunk(pRecordset, _T("rep"), ppRep, rep.nRepLen);
		}	
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_FetchUserIDByHand(LONG& errcode, _ConnectionPtr& connect, UINT& response, int& nUserID, LPCTSTR lpszHandNO)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select * from tblUser3 where HandNO = '%s' "), lpszHandNO);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			nUserID = pRecordset->Fields->Item[_T("ID")]->Value.lVal;
			nResult=1;
		}	
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); ReportDBErrorLog(e, szSql);
	}
	return nResult;
}


void DBCopyQuan(LPQUAN lpQuan, _RecordsetPtr& pRecordset)
{
	lpQuan->nQuanID   = pRecordset->Fields->Item[_T("ID")]->Value.lVal;
	lpQuan->nQuanType = pRecordset->Fields->Item[_T("Type")]->Value.lVal;
	lpQuan->nSubType  = pRecordset->Fields->Item[_T("SubType")]->Value.lVal;
	lpQuan->nStatus   = pRecordset->Fields->Item[_T("Status")]->Value.lVal;
	lpQuan->dwOptions = pRecordset->Fields->Item[_T("Options")]->Value.lVal;
	lpQuan->nGameID   = pRecordset->Fields->Item[_T("GameID")]->Value.lVal;
   	lpQuan->nRoomID   = pRecordset->Fields->Item[_T("RoomID")]->Value.lVal;
	lpQuan->nTableNO  = pRecordset->Fields->Item[_T("TableNO")]->Value.lVal;
	lpQuan->nCreatorID  = pRecordset->Fields->Item[_T("CreatorID")]->Value.lVal;
	lpQuan->nCreateDate  = pRecordset->Fields->Item[_T("CreateDate")]->Value.lVal;
	lpQuan->nCreateTime  = pRecordset->Fields->Item[_T("CreateTime")]->Value.lVal;
	lpQuan->nUserCount  = pRecordset->Fields->Item[_T("UserCount")]->Value.lVal;
	lpQuan->nLastRoundID = pRecordset->Fields->Item[_T("LastRoundID")]->Value.lVal;
	
	TCHAR szFieldName[32];
	ZeroMemory(szFieldName,sizeof(szFieldName));
	_bstr_t bstrValue ;
	
	for(int i=0;i<lpQuan->nUserCount&&i<MAX_QUANCHAIR_COUNT;i++)
	{
		sprintf(szFieldName,_T("UserID%d"),i);
		lpQuan->nUserIDs[i]  = pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value.lVal;
		
		sprintf(szFieldName,_T("HandNO%d"),i);
		
		bstrValue  = pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value;
		lstrcpy(lpQuan->szHandNOs[i], (LPCTSTR)bstrValue);
		
	}
	
	bstrValue = pRecordset->Fields->Item[_T("EMGroupID")]->Value;
	lstrcpy(lpQuan->szEMGroupID, (LPCTSTR)bstrValue);

	bstrValue = pRecordset->Fields->Item[_T("QuanName")]->Value;
	lstrcpy(lpQuan->szQuanName, (LPCTSTR)bstrValue);
}

void DBCopyQuanRound(LPQUAN_ROUND lpQuanRound, _RecordsetPtr& pRecordset)
{
	lpQuanRound->nID   = pRecordset->Fields->Item[_T("ID")]->Value.lVal;
	lpQuanRound->nRoundID   = pRecordset->Fields->Item[_T("RoundID")]->Value.lVal;
	lpQuanRound->nQuanID = pRecordset->Fields->Item[_T("QuanID")]->Value.lVal;
	lpQuanRound->nInitPian  = pRecordset->Fields->Item[_T("InitPian")]->Value.lVal;
	lpQuanRound->nStatus   = pRecordset->Fields->Item[_T("Status")]->Value.lVal;
	lpQuanRound->nWinUser = pRecordset->Fields->Item[_T("WinUser")]->Value.lVal;
	lpQuanRound->nStartDate = pRecordset->Fields->Item[_T("StartDate")]->Value.lVal;
	lpQuanRound->nStartTime   = pRecordset->Fields->Item[_T("StartTime")]->Value.lVal;
   	lpQuanRound->nLastDate   = pRecordset->Fields->Item[_T("LastDate")]->Value.lVal;
	lpQuanRound->nLastTime  = pRecordset->Fields->Item[_T("LastTime")]->Value.lVal;
	lpQuanRound->nBoutID  = pRecordset->Fields->Item[_T("BoutID")]->Value.lVal;
	lpQuanRound->nUserCount = pRecordset->Fields->Item[_T("UserCount")]->Value.lVal;
	lpQuanRound->nTotalFee = pRecordset->Fields->Item[_T("TotalFee")]->Value.lVal;
	
	TCHAR szFieldName[32];
	ZeroMemory(szFieldName,sizeof(szFieldName));
	for(int i=0;i<lpQuanRound->nUserCount&&i<MAX_QUANCHAIR_COUNT;i++)
	{
		sprintf(szFieldName,_T("ChairNO%d"),i);
		lpQuanRound->nChairNO[i]  = pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value.lVal;
		
		sprintf(szFieldName,_T("Dice%d"),i);
		lpQuanRound->nDice[i]  = pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value.lVal;
		
		sprintf(szFieldName,_T("Pian%d"),i);
		lpQuanRound->nPian[i]  = pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value.lVal;
	}
}

void DBCopyQuanUser(LPQUAN_USER lpQuanUser, _RecordsetPtr& pRecordset)
{
	TCHAR szFieldName[32];
	ZeroMemory(szFieldName,sizeof(szFieldName));
	_bstr_t bstrValue ;
	
	
	lpQuanUser->nID		  = pRecordset->Fields->Item[_T("ID")]->Value.lVal;
	lpQuanUser->nUserID   = pRecordset->Fields->Item[_T("UserID")]->Value.lVal;
	lpQuanUser->nQuanID   = pRecordset->Fields->Item[_T("QuanID")]->Value.lVal;
	lpQuanUser->dwOptions = pRecordset->Fields->Item[_T("Options")]->Value.lVal;
	lpQuanUser->nLayOrder = pRecordset->Fields->Item[_T("LayOrder")]->Value.lVal;
	lpQuanUser->nFontColor = pRecordset->Fields->Item[_T("FontColor")]->Value.lVal;
	lpQuanUser->nUserCount   = pRecordset->Fields->Item[_T("UserCount")]->Value.lVal;
	
	bstrValue  = pRecordset->Fields->Item[_T("QuanName")]->Value;
	lstrcpy(lpQuanUser->szQuanName, (LPCTSTR)bstrValue);
	
	for(int i=0;i<lpQuanUser->nUserCount&&i<MAX_QUANCHAIR_COUNT;i++)
	{
		sprintf(szFieldName,_T("ShowName%d"),i);
		
		bstrValue  = pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value;
		lstrcpy(lpQuanUser->szShowNames[i], (LPCTSTR)bstrValue);
		
	}
	
}

void DBCopyQuanBout(LPQUAN_BOUT lpQuanBout, _RecordsetPtr& pRecordset)
{
	TCHAR szFieldName[32];
	ZeroMemory(szFieldName,sizeof(szFieldName));
	_bstr_t bstrValue ;
	
	lpQuanBout->nID		  = pRecordset->Fields->Item[_T("ID")]->Value.lVal;
	lpQuanBout->nQuanID   = pRecordset->Fields->Item[_T("QuanID")]->Value.lVal;
	lpQuanBout->nRoundID   = pRecordset->Fields->Item[_T("RoundID")]->Value.lVal;
	lpQuanBout->nBoutID = pRecordset->Fields->Item[_T("BoutID")]->Value.lVal;
	lpQuanBout->nBanker = pRecordset->Fields->Item[_T("Banker")]->Value.lVal;
	lpQuanBout->nUserCount = pRecordset->Fields->Item[_T("UserCount")]->Value.lVal;
	lpQuanBout->nCreateDate = pRecordset->Fields->Item[_T("CreateDate")]->Value.lVal;
	lpQuanBout->nCreateTime   = pRecordset->Fields->Item[_T("CreateTime")]->Value.lVal;
	lpQuanBout->nRepID = pRecordset->Fields->Item[_T("RepID")]->Value.lVal;
	lpQuanBout->nTotalFee = pRecordset->Fields->Item[_T("TotalFee")]->Value.lVal;
	
	for(int i=0;i<lpQuanBout->nUserCount&&i<MAX_QUANCHAIR_COUNT;i++)
	{
		sprintf(szFieldName,_T("UserDiff%d"),i);
		lpQuanBout->nUserDiff[i] = pRecordset->Fields->Item[szFieldName]->Value.lVal;
	}
}


int DB_FetchQuanRound(LONG& errcode, _ConnectionPtr& connect, UINT& response,int nGameID, int nQuanID, int nRoundID, LPQUAN_ROUND lpQuanRound)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select * from tblRoundOnGame%d where QuanID = %d and RoundID = %d"), nGameID, nQuanID, nRoundID);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			DBCopyQuanRound(lpQuanRound, pRecordset);
			nResult=1;
		}	
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
		ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_FetchQuanUser(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPQUAN_USER lpQuanUser)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select * from tblQuanUser where UserID = %d and QuanID = %d "),lpQuanUser->nUserID,lpQuanUser->nQuanID);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			DBCopyQuanUser(lpQuanUser, pRecordset);
			nResult=1;
		}	
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
		ReportDBErrorLog(e, szSql);
	}
	return nResult;
}


int DB_FetchQuanBout(LONG& errcode, _ConnectionPtr& connect, UINT& response,int nGameID, LPQUAN_BOUT lpQuanBout)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select * from tblBoutOnGame%d where QuanID = %d and RoundID = %d and BoutID = %d "), 
		nGameID, lpQuanBout->nQuanID, lpQuanBout->nRoundID, lpQuanBout->nBoutID);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			DBCopyQuanBout(lpQuanBout, pRecordset);
			nResult=1;
		}	
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
		ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_FetchOneQuan(LONG& errcode, _ConnectionPtr& connect, UINT& response, LPQUAN lpQuan)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select * from tblQuan where id = %d"),lpQuan->nQuanID);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			DBCopyQuan(lpQuan, pRecordset);
			nResult=1;
		}	
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
		ReportDBErrorLog(e, szSql);
	}
	return nResult;
}


void DBSetQuan(LPQUAN lpQuan, _RecordsetPtr& pRecordset)
{
	TCHAR szFieldName[32];
	ZeroMemory(szFieldName,sizeof(szFieldName));

 	_variant_t varField;
	varField.Clear();
	varField.ChangeType(VT_I4);

	varField.lVal = lpQuan->nQuanType;
	pRecordset->Fields->Item[_T("Type")]->Value = varField;

	varField.lVal = lpQuan->nSubType;
	pRecordset->Fields->Item[_T("SubType")]->Value = varField;

	varField.lVal = lpQuan->nStatus;
	pRecordset->Fields->Item[_T("Status")]->Value = varField;
 
	varField.lVal = lpQuan->dwOptions;
	pRecordset->Fields->Item[_T("Options")]->Value = varField;

 	varField.lVal = lpQuan->nGameID;
	pRecordset->Fields->Item[_T("GameID")]->Value = varField;

 	varField.lVal = lpQuan->nRoomID;
	pRecordset->Fields->Item[_T("RoomID")]->Value = varField;

 	varField.lVal = lpQuan->nTableNO;
	pRecordset->Fields->Item[_T("TableNO")]->Value = varField;

 	varField.lVal = lpQuan->nCreatorID;
	pRecordset->Fields->Item[_T("CreatorID")]->Value = varField;

 	varField.lVal = lpQuan->nCreateDate;
	pRecordset->Fields->Item[_T("CreateDate")]->Value = varField;

 	varField.lVal = lpQuan->nCreateTime;
	pRecordset->Fields->Item[_T("CreateTime")]->Value = varField;

 	varField.lVal = lpQuan->nUserCount;
	pRecordset->Fields->Item[_T("UserCount")]->Value = varField;
	

	for(int i=0;i<lpQuan->nUserCount;i++)
	{
		sprintf(szFieldName,_T("UserID%d"),i);
 		varField.lVal = lpQuan->nUserIDs[i];
		pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value = varField;

		sprintf(szFieldName,_T("HandNO%d"),i);
		pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value = (_bstr_t)(lpQuan->szHandNOs[i]);

	}

	pRecordset->Fields->Item[_T("EMGroupID")]->Value = (_bstr_t)(lpQuan->szEMGroupID);

	pRecordset->Fields->Item[_T("QuanName")]->Value = (_bstr_t)(lpQuan->szQuanName);
}

void DBSetQuanUser(LPQUAN_USER lpQuanUser, _RecordsetPtr& pRecordset)
{
 	_variant_t varField;
	varField.Clear();
	varField.ChangeType(VT_I4);

	varField.lVal = lpQuanUser->nUserID;
	pRecordset->Fields->Item[_T("UserID")]->Value = varField;

	varField.lVal = lpQuanUser->nQuanID;
	pRecordset->Fields->Item[_T("QuanID")]->Value = varField;

 	varField.lVal = lpQuanUser->dwOptions;
	pRecordset->Fields->Item[_T("Options")]->Value = varField;

 	varField.lVal = lpQuanUser->nLayOrder;
	pRecordset->Fields->Item[_T("LayOrder")]->Value = varField;

 	varField.lVal = lpQuanUser->nFontColor;
	pRecordset->Fields->Item[_T("FontColor")]->Value = varField;

	pRecordset->Fields->Item[_T("QuanName")]->Value = (_bstr_t)(lpQuanUser->szQuanName);

 	varField.lVal = lpQuanUser->nUserCount;
	pRecordset->Fields->Item[_T("UserCount")]->Value = varField;

  	
	for(int i=0;i<lpQuanUser->nUserCount;i++)
	{
		TCHAR szFieldName[32];
	 
		sprintf(szFieldName,_T("ShowName%d"),i);
		pRecordset->Fields->Item[(LPCTSTR)szFieldName]->Value = (_bstr_t)(lpQuanUser->szShowNames[i]);

	}
}

void DBSetQuanHand(int nQuanID,LPCTSTR lpszHandNO, _RecordsetPtr& pRecordset)
{
 	_variant_t varField;
	varField.Clear();
	varField.ChangeType(VT_I4);
  
	varField.lVal = nQuanID;
	pRecordset->Fields->Item[_T("QuanID")]->Value = varField;
	
	pRecordset->Fields->Item[_T("HandNO")]->Value = (_bstr_t)lpszHandNO;
   
}

void DBSetFieldChunk(_RecordsetPtr pRecordset, LPCTSTR lpszField, LPVOID pChunk, int nChunkSize)
{
	SAFEARRAY* pSafeArray = NULL;
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = nChunkSize;
	
	pSafeArray = SafeArrayCreate(VT_UI1, 1, rgsabound);
	
	for (int i = 0; i < nChunkSize; i++)
	{
		BYTE* pData = &(((BYTE*)pChunk)[i]);
		SafeArrayPutElement(pSafeArray, (long*)&i, pData);
	}
	
	_variant_t vtChunk;
	vtChunk.vt = VT_ARRAY | VT_UI1;
	vtChunk.parray = pSafeArray;
	
	pRecordset->Fields->GetItem(_variant_t(lpszField))->AppendChunk(vtChunk);
	
}


int DBGetFieldChunk(_RecordsetPtr pRecordset, LPCTSTR lpszField, LPVOID* ppChunk, int& nChunkSize)
{
	int nResult = 0;
	char* pbuf = NULL;
	nChunkSize = pRecordset->GetFields()->GetItem(lpszField)->ActualSize;
	if (nChunkSize>0)
	{
		*ppChunk = new BYTE[nChunkSize];
		_variant_t varBLOB;
		VariantInit(&varBLOB);
		varBLOB = pRecordset->GetFields()->GetItem(lpszField)->GetChunk(nChunkSize);
		if(varBLOB.vt == (VT_ARRAY | VT_UI1)) 
		{
			char *pBuf = NULL;
			SafeArrayAccessData(varBLOB.parray,(void **)&pBuf);
			memcpy(*ppChunk, pBuf, nChunkSize);
			SafeArrayUnaccessData(varBLOB.parray);		
			nResult = 1;
		}
	}
	
	return nResult;
}

int DB_SetQuanName(LONG& errcode, _ConnectionPtr& connect, UINT& response,int nQuanID, LPCTSTR lpszQuanName)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("select * from tblQuan where ID = %d "), nQuanID);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		pRecordset->CursorLocation = adUseClient;	// so we can obtain autocreated id soon.
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{
			pRecordset->Fields->Item[_T("QuanName")]->Value = (_bstr_t)lpszQuanName;
			TESTHR(pRecordset->Update());
			nResult=1;
		}	
		TESTHR(pRecordset->Close());
		
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e); 
		ReportDBErrorLog(e, szSql);
	}
	return nResult;	
}


int DB_GetMatchGameResult_SQ(LONG& errcode, _ConnectionPtr& connect, UINT& response, int nGameDBID,int userid,  LPGAME_RESULT_SQ lpGameResult)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select * from tblMatchOnGame%d where UserID = %d "), nGameDBID,userid);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect, true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile){

			lpGameResult->nBout = pRecordset->Fields->Item[_T("Bout")]->Value.lVal;
			lpGameResult->nBreakOff = pRecordset->Fields->Item[_T("BreakOff")]->Value.lVal;
			lpGameResult->nDeposit = pRecordset->Fields->Item[_T("Deposit")]->Value.lVal;
			lpGameResult->nLoss = pRecordset->Fields->Item[_T("Loss")]->Value.lVal;
			lpGameResult->nPlayerLevel = pRecordset->Fields->Item[_T("PlayerLevel")]->Value.lVal;
			lpGameResult->nScore = pRecordset->Fields->Item[_T("Score")]->Value.lVal;
			lpGameResult->nStandOff = pRecordset->Fields->Item[_T("StandOff")]->Value.lVal;
			lpGameResult->nTimeCost = pRecordset->Fields->Item[_T("TimeCost")]->Value.lVal;
			lpGameResult->nWin = pRecordset->Fields->Item[_T("Win")]->Value.lVal;

			lpGameResult->nMatchID = pRecordset->Fields->Item[_T("MatchID")]->Value.lVal;
			lpGameResult->nClubID = pRecordset->Fields->Item[_T("ClubID")]->Value.lVal;
			lpGameResult->nMemberLevel = pRecordset->Fields->Item[_T("MemberLevel")]->Value.lVal;
			lpGameResult->nTaskGain = pRecordset->Fields->Item[_T("TaskGain")]->Value.lVal;
			lpGameResult->nBombCount = pRecordset->Fields->Item[_T("BombCount")]->Value.lVal;
			lpGameResult->nMaxBoutScore = pRecordset->Fields->Item[_T("MaxBoutScore")]->Value.lVal;

			lpGameResult->nContestentLevel = pRecordset->Fields->Item[_T("ContestentLevel")]->Value.lVal;
			lpGameResult->nCoachLevel = pRecordset->Fields->Item[_T("CoachLevel")]->Value.lVal;
			lpGameResult->nLuckBoxCount = pRecordset->Fields->Item[_T("LuckBoxCount")]->Value.lVal;
			lpGameResult->nLuckBoxTaken = pRecordset->Fields->Item[_T("LuckBoxTaken")]->Value.lVal;
			lpGameResult->nWinPoint = pRecordset->Fields->Item[_T("WinPoint")]->Value.lVal;
			lpGameResult->nPointPerWin = pRecordset->Fields->Item[_T("PointPerWin")]->Value.lVal;
			lpGameResult->nNextBoxNeedPoint = pRecordset->Fields->Item[_T("NextBoxNeedPoint")]->Value.lVal;
			lpGameResult->dwLuckBuff = pRecordset->Fields->Item[_T("LuckBuff")]->Value.lVal;
			lpGameResult->nCardRob = pRecordset->Fields->Item[_T("CardRob")]->Value.lVal;
			lpGameResult->nCardProtect = pRecordset->Fields->Item[_T("CardProtect")]->Value.lVal;
			lpGameResult->nComboWin = pRecordset->Fields->Item[_T("ComboWin")]->Value.lVal;
			lpGameResult->nAuthority = pRecordset->Fields->Item[_T("Authority")]->Value.lVal;
			lpGameResult->nCardGuidance = pRecordset->Fields->Item[_T("CardGuidance")]->Value.lVal;

			nResult = 1;
		}
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
		ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

int DB_RefreshMatchResult_SQ(LONG& errcode, _ConnectionPtr& connect, UINT& response, int nGameDBID,LPEXT_GAME_RESULT_SQ lpGameResult, DWORD dwFlags)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	TCHAR szBuf[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("update tblMatchOnGame%d "), nGameDBID);
	_stprintf(szBuf, _T("set PlayerLevel = %d, "), lpGameResult->nLevelID);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Deposit = Deposit + %d, "), lpGameResult->nDepositDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Score = Score + %d, "), lpGameResult->nScoreDiff);
	lstrcat(szSql, szBuf);
// 	_stprintf(szBuf, _T("Experience = Experience + %d, "), lpGameResult->nExperience);
// 	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("BreakOff = BreakOff + %d, "), lpGameResult->nBreakOff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Win = Win + %d, "), lpGameResult->nWin);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Loss = Loss + %d, "), lpGameResult->nLoss);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("StandOff = StandOff + %d, "), lpGameResult->nStandOff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("Bout = Bout + %d, "), lpGameResult->nBout);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("TimeCost = TimeCost + %d ,"), lpGameResult->nTimeCost);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("BombCount = BombCount + %d ,"), lpGameResult->nBombCountDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("TaskGain = TaskGain + %d ,"), lpGameResult->nTaskGainDiff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("MaxBoutScore = %d ,"), lpGameResult->nMaxBoutScore);
	lstrcat(szSql, szBuf);

	_stprintf(szBuf, _T("LuckBoxCount = %d ,"), lpGameResult->nLuckBoxCount);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("LuckBoxTaken = %d ,"), lpGameResult->nLuckBoxTaken);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("WinPoint = %d ,"), lpGameResult->nWinPoint);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("NextBoxNeedPoint = %d ,"), lpGameResult->nNextBoxNeedPoint);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("CardRob = %d ,"), lpGameResult->nCardRob);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("LuckBuff = %d ,"), lpGameResult->dwLuckBuff);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("ComboWin = %d ,"), lpGameResult->nComboWin);
	lstrcat(szSql, szBuf);
	_stprintf(szBuf, _T("CardGuidance = %d "), lpGameResult->nCardGuidance);
	lstrcat(szSql, szBuf);
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	//赛趣表里，无DivBout、DivTimeCost字段
// 	if(lpGameResult->nGameID==421)//针对13水这个游戏特别处理
// 	{
// 		int nDivTimeCost=lpGameResult->nReserved[0];
//         if(nDivTimeCost >0 && nDivTimeCost<=lpGameResult->nTimeCost)
// 		{
// 			_stprintf(szBuf, _T(", DivBout = DivBout + %d, DivTimeCost = DivTimeCost + %d "), lpGameResult->nBout,nDivTimeCost);
// 			lstrcat(szSql, szBuf);
// 		}
// 	} 
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
  	_stprintf(szBuf, _T("where UserID = %d "), lpGameResult->nUserID);
	lstrcat(szSql, szBuf);
	if(IS_BIT_SET(dwFlags, RF_VERIFY_DEPOSIT)){
		_stprintf(szBuf, _T("and Deposit = %d "), lpGameResult->nOldDeposit);
		lstrcat(szSql, szBuf);
		_stprintf(szBuf, _T("and Deposit + %d >= 0 "), lpGameResult->nDepositDiff);
		lstrcat(szSql, szBuf);
	}

	UwlTraceSql(szSql);
	try{
		_variant_t varRows;
		TESTHR(connect->Execute(szSql, &varRows, 
							adCmdText | adExecuteNoRecords));
		nResult = varRows.lVal;
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
		ReportDBErrorLog(e, szSql);
	}
	return nResult;
}

*/
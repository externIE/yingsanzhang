#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


int DB_ValidateClientEx(LONG& errcode, _ConnectionPtr& connect, UINT& response,  LPVALIDATE_CLIENT_EX lpValidateClientEx)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	if(DB_FindInValidWord(lpValidateClientEx->szVolumeID ))
	{
		UwlLogFile(_T("InValidWord->DB_ValidateClientEx():%s"),lpValidateClientEx->szVolumeID );
		return 0;
	}
	
	if(lstrlen(lpValidateClientEx->szVolumeID)>= MAX_HARDID_LEN )
		return nResult;
	
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select * from tblHost where VolumeID='%s' and status = 0 ") ,lpValidateClientEx->szVolumeID);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile){
			nResult = 1;
			response=UR_FETCH_SUCCEEDED;
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

BOOL DB_FindInValidWord(LPCTSTR lpszName)
{
	CString sMask=_T("¡¡%	<>()[]{}*';\"/\\| ¡¡ ");

	CString sName;
	sName=lpszName;

	if(sName.FindOneOf(sMask)!=-1  )
	    return TRUE;
	return FALSE;
}

int DB_GetPropInfo(LONG& errcode, _ConnectionPtr& connect, UINT& response, int userid, GET_PROPINFO_OK& prop)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select * from tblProp where UserID = %d "), userid);
	
	UwlTraceSql(szSql);
	
	try{
		_RecordsetPtr pRecordSet = NULL;
		TESTHR(pRecordSet.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordSet->Open(szSql, _variant_t((IDispatch*)connect, true), 
				adOpenDynamic, adLockOptimistic, adCmdText));
		if (FALSE == pRecordSet->EndOfFile)
		{
			prop.nUserID	= pRecordSet->Fields->Item[_T("UserID")]->Value.lVal;
			prop.nLuckyCard = pRecordSet->Fields->Item[_T("LuckCard")]->Value.lVal;
			prop.nScoreMult = pRecordSet->Fields->Item[_T("ScoreMult")]->Value.lVal;
			prop.nScoreMultStartDate = pRecordSet->Fields->Item[_T("ScoreMultStartTime")]->Value.lVal;
			prop.nScoreMultEndDate   = pRecordSet->Fields->Item[_T("ScoreMultEndTime")]->Value.lVal;
			prop.nScoreSafe = pRecordSet->Fields->Item[_T("ScoreSafe")]->Value.lVal;
			prop.nScoreSafeStartDate = pRecordSet->Fields->Item[_T("ScoreSafeStartTime")]->Value.lVal;
			prop.nScoreSafeEndDate = pRecordSet->Fields->Item[_T("ScoreSafeEndTime")]->Value.lVal;
			prop.nUpdateDate = pRecordSet->Fields->Item[_T("UpdateTime")]->Value.lVal;

			nResult  = 1;
		}
		TESTHR(pRecordSet->Close());
	}catch (_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		errcode = e.Error();
		UwlTrace(_T("database access error!"));
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	}
	
	return nResult;
}

int DB_UseLuckyCard(LONG& errcode, _ConnectionPtr& connect, UINT& response, int userid,  USE_LUCKCARD_OK& luckCard)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	
	// Query lucky-card info, Use one lucky-card, and Update the db table.
	TCHAR szSql[MAX_SQL_LENGTH];
	_stprintf(szSql, _T("select LuckCard, UpdateTime from tblProp where UserID = %d  "), userid);
 	UwlTraceSql(szSql);	
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
				adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile)
		{	
			luckCard.nLuckyCardLeft = pRecordset->Fields->Item[_T("LuckCard")]->Value.lVal;
			if (luckCard.nLuckyCardLeft > 0)
			{
				luckCard.nLuckyCardLeft--;
				luckCard.nUpdateTime = UwlGetCurrentSeconds();

				_variant_t varField;
				varField.Clear();
				varField.ChangeType(VT_I4);
				varField.lVal = luckCard.nLuckyCardLeft;
				pRecordset->Fields->Item[_T("LuckCard")]->Value = varField;

				varField.lVal = luckCard.nUpdateTime;
				pRecordset->Fields->Item[_T("UpdateTime")]->Value = varField;

				TESTHR(pRecordset->Update());
				nResult = 1;
			}
			else
			{
				response = UR_LUCKCARD_ZERO; 
			}
		}
		TESTHR(pRecordset->Close());
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		errcode = e.Error();
		UwlTrace(_T("database access error!"));
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(connect);
		LogComError(e);
	};
	
	return nResult; 
}

COleDateTime ReadDateTime(_RecordsetPtr& pRecordset, LPCTSTR lpszField)
{
	DATE date =  pRecordset->Fields->Item[lpszField]->Value.date;
	
	COleDateTime dateTime = COleDateTime(date);
	
	return dateTime;
}

void WriteDateTimeForNow(_RecordsetPtr& pRecordset, LPCTSTR lpszField)
{
	COleDateTime dateTime = COleDateTime::GetCurrentTime();
	_variant_t temp;
	temp.vt = VT_DATE;
	temp = dateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	pRecordset->Fields->Item[lpszField]->PutValue(temp);
}

void WriteDateTime(_RecordsetPtr& pRecordset, LPCTSTR lpszField, CTime& time)
{
	COleDateTime dateTime = COleDateTime(time.GetTime());
	_variant_t temp;
	temp.vt = VT_DATE;
	temp = dateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	pRecordset->Fields->Item[lpszField]->PutValue(temp);
}

void WriteDateTime(_RecordsetPtr& pRecordset, LPCTSTR lpszField, int nUnixTime)
{
	CTime time(nUnixTime);
	COleDateTime dateTime = COleDateTime(time.GetTime()); 
	_variant_t temp;
	temp.vt = VT_DATE;
	temp = dateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	pRecordset->Fields->Item[lpszField]->PutValue(temp);
}

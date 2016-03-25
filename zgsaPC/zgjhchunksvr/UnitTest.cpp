#include "StdAfx.h"
#include "UnitTest.h"



BOOL TestAliDB()
{
	_ConnectionPtr pConnect = NULL;

	CString sConnect;
	sConnect.Format(_T("Provider=sqloledb;Data Source=%s;Initial Catalog=%s;User Id=%s;Password=%s;"), 
		_T("ctchunkdb.sqlserver.rds.aliyuncs.com,3433"), _T("chunkdb"),  _T("chunksvr221"), _T("pwnyrk89"));
// 	sConnect.Format(_T("Provider=SQLNCLI;Server=%s;Database=%s;uid=%s;pwd=%s;"), 
// 		_T("rdsmeuryrqvjzaq.sqlserver.rds.aliyuncs.com,3433"), _T("chunkdb"),  _T("chunksvr221"), _T("pwnyrk89"));

	if(!DB_SafeOpen(pConnect, sConnect))
		return FALSE;

	LONG errcode = 0;
	int nResult = 0;
	UINT response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];

	lstrcpy(szSql, _T("select * from tblSignIn221 where UserID = 45 "));
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)pConnect, true), 
			adOpenDynamic, adLockOptimistic, adCmdText));
		if (FALSE == pRecordset->EndOfFile)
		{
			COleDateTime dateTime = ReadDateTime(pRecordset, _T("UpdateTime"));
			int nYear = dateTime.GetYear();
			int nMonth = dateTime.GetMonth();
			CString   strData = dateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			
			WriteDateTimeForNow(pRecordset, _T("UpdateTime"));
			
			WriteDateTime(pRecordset, _T("UpdateTime"), UwlGetCurrentSeconds());
			
			CTime time = CTime::GetCurrentTime();
			WriteDateTime(pRecordset, _T("UpdateTime"), time);

			TESTHR(pRecordset->Update());
		}
		TESTHR(pRecordset->Close());
		
	}catch(_com_error e){
		TraceProviderError(pConnect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database access error!"));
		errcode = e.Error();
		UwlLogFile(_T("database access error!"));
		UwlLogFile(_T("sql: %s"), szSql);
		LogProviderError(pConnect);
		LogComError(e);
		ReportDBErrorLog(e, szSql);
	}
 
	DB_SafeClose(pConnect);
	
	return TRUE;
}

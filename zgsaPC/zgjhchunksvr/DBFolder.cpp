#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 


int DB_TestConnect(LONG& errcode, _ConnectionPtr& connect, UINT& response)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("select dummy0 from tblDummy "));
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile){
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
		TCHAR szSqlSate[16];
		memset(szSqlSate, 0, sizeof(szSqlSate));
		if(DB_GetSqlState(connect, szSqlSate)){
			if(!lstrcmpi(szSqlSate, _T("01002")) // 断开连接错误
				|| !lstrcmpi(szSqlSate, _T("08S0 "))  // 连接失败
				|| !lstrcmpi(szSqlSate, _T("08S01"))  // 通讯链接失败
				|| !lstrcmpi(szSqlSate, _T("08001"))  // 无法连接到数据源
				|| !lstrcmpi(szSqlSate, _T("08004"))  // 数据源拒绝建立连接
				|| !lstrcmpi(szSqlSate, _T("08007"))){// 在执行事务的过程中连接失败
				errcode = DB_CONNECT_ERROR;
			}
		}
	}
	return nResult;
}

// function to test connect for game database
int DB_TestConnectEx(LONG& errcode, _ConnectionPtr& connect, UINT& response, int gameid)
{
	int nResult = 0;
	response = UR_OBJECT_NOT_EXIST;
	TCHAR szSql[MAX_SQL_LENGTH];
	
	_stprintf(szSql, _T("select dummy%ld from tblDummy "), gameid % 32 + 1);
	UwlTraceSql(szSql);
	try{
		_RecordsetPtr pRecordset = NULL;
		TESTHR(pRecordset.CreateInstance(__uuidof(Recordset)));
		TESTHR(pRecordset->Open(szSql, _variant_t((IDispatch *)connect,true), 
					adOpenDynamic, adLockOptimistic, adCmdText));
		if(FALSE == pRecordset->EndOfFile){
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
		TCHAR szSqlSate[16];
		memset(szSqlSate, 0, sizeof(szSqlSate));
		if(DB_GetSqlState(connect, szSqlSate)){
			if(!lstrcmpi(szSqlSate, _T("01002")) // 断开连接错误
				|| !lstrcmpi(szSqlSate, _T("08S0 "))  // 连接失败
				|| !lstrcmpi(szSqlSate, _T("08S01"))  // 通讯链接失败
				|| !lstrcmpi(szSqlSate, _T("08001"))  // 无法连接到数据源
				|| !lstrcmpi(szSqlSate, _T("08004"))  // 数据源拒绝建立连接
				|| !lstrcmpi(szSqlSate, _T("08007"))){// 在执行事务的过程中连接失败
				errcode = DB_CONNECT_ERROR;
			}
		}
	}
	return nResult;
}



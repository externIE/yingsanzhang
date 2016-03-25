#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

void _TraceProviderError(_ConnectionPtr pConnection)
{
    // Print Provider Errors from Connection object.
    // pErr is a record object in the Connection's Error collection.
    ErrorPtr  pErr = NULL;

    if( (pConnection->Errors->Count) > 0) {
        long nCount = pConnection->Errors->Count;
        // Collection ranges from 0 to nCount -1.
        for(long i = 0; i < nCount; i++) {
            pErr = pConnection->Errors->GetItem(i);
            UwlTrace(_T("\t Error number: %x\t%s"), pErr->Number,
						(LPCTSTR)pErr->Description);
            UwlTrace(_T("\t Error native: %d\t%s"), pErr->NativeError,
						(LPCTSTR)pErr->SQLState);
        }
    }
}

void _TraceComError(_com_error &e)
{
    _bstr_t bstrSource(e.Source());
    _bstr_t bstrDescription(e.Description());
    
    // Print Com errors.
    UwlTrace(_T("Error\n"));
    UwlTrace(_T("\tCode = %08lx\n"), e.Error());
    UwlTrace(_T("\tCode meaning = %s\n"), e.ErrorMessage());
    UwlTrace(_T("\tSource = %s\n"), (LPCTSTR) bstrSource);
    UwlTrace(_T("\tDescription = %s\n"), (LPCTSTR) bstrDescription);
}

void LogProviderError(_ConnectionPtr pConnection)
{
    // Print Provider Errors from Connection object.
    // pErr is a record object in the Connection's Error collection.
    ErrorPtr  pErr = NULL;

    if( (pConnection->Errors->Count) > 0) {
        long nCount = pConnection->Errors->Count;
        // Collection ranges from 0 to nCount -1.
        for(long i = 0; i < nCount; i++) {
            pErr = pConnection->Errors->GetItem(i);
            UwlLogFile(_T("\t Error number: %x\t%s"), pErr->Number,
						(LPCTSTR)pErr->Description);
            UwlLogFile(_T("\t Error native: %d\t%s"), pErr->NativeError,
						(LPCTSTR)pErr->SQLState);
        }
    }
}

void LogComError(_com_error &e)
{
    _bstr_t bstrSource(e.Source());
    _bstr_t bstrDescription(e.Description());
    
    // Print Com errors.
    UwlLogFile(_T("Error\n"));
    UwlLogFile(_T("\tCode = %08lx\n"), e.Error());
    UwlLogFile(_T("\tCode meaning = %s\n"), e.ErrorMessage());
    UwlLogFile(_T("\tSource = %s\n"), (LPCTSTR) bstrSource);
    UwlLogFile(_T("\tDescription = %s\n"), (LPCTSTR) bstrDescription);
}

BOOL DB_GetSqlState(_ConnectionPtr pConnection, LPTSTR lpszSqlSate)
{
    // Print Provider Errors from Connection object.
    // pErr is a record object in the Connection's Error collection.
    ErrorPtr  pErr = NULL;

    if( (pConnection->Errors->Count) > 0) {
        long nCount = pConnection->Errors->Count;
        // Collection ranges from 0 to nCount -1.
        for(long i = 0; i < nCount; i++) {
            pErr = pConnection->Errors->GetItem(i);
			lstrcpy(lpszSqlSate, (LPCTSTR)pErr->SQLState);
        }
		return TRUE;
    }else{
		return FALSE;
	}
}

int DB_Connect(_ConnectionPtr& connect, LPCTSTR lpszConnect)
{
	HRESULT hr = ::CoInitialize(NULL);
	if(FAILED(hr)) return 0;

	return DB_SafeOpen(connect, lpszConnect);
}

int DB_Disconnect(_ConnectionPtr& connect)
{
	DB_SafeClose(connect);

	::CoUninitialize();
	return 1;
}

BOOL DB_SafeOpen(_ConnectionPtr& connect, LPCTSTR lpszConnect)
{
	HRESULT hr = connect.CreateInstance(__uuidof(Connection));
	if(FAILED(hr)) return 0;

	try{
		hr = 1;
		connect->Open(lpszConnect, _T(""), _T(""), adConnectUnspecified);
	}catch(_com_error e){
		hr = 0;
		_TraceProviderError(connect);
		_TraceComError(e);
		LogProviderError(connect);
		LogComError(e);
	}
	return (hr != 0);	
}

BOOL DB_SafeClose(_ConnectionPtr& connect)
{
	try{
		if(connect){
			connect->Close();
			connect = NULL;
		}
	}catch(_com_error e){
		_TraceProviderError(connect);
		_TraceComError(e);
		LogProviderError(connect);
		LogComError(e);
		return FALSE;
	}
	return TRUE;
}

int DB_BeginTrans(LONG& errcode, _ConnectionPtr& connect, UINT& response)
{
#ifndef UWL_TRANSDB
	return 1;
#else
	return DB_BeginTransEx(errorcode, connect, resposne);
#endif
}

int DB_Commit(LONG& errcode, _ConnectionPtr& connect, UINT& response)
{
#ifndef UWL_TRANSDB
	return 1;
#else
	return DB_CommitEx(errorcode, connect, resposne);
#endif
}

int DB_Rollback(LONG& errcode, _ConnectionPtr& connect, UINT& response)
{
#ifndef UWL_TRANSDB
	return 1;
#else
	return DB_RollbackEx(errorcode, connect, resposne);
#endif
}

int DB_BeginTransEx(LONG& errcode, _ConnectionPtr& connect, UINT& response)
{
	int nResult = 0;

	try{
		TESTHR(connect->BeginTrans());
		nResult = 1;
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database begintrans error!"));
		errcode = e.Error();
		UwlLogFile(_T("database begintrans error!"));
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
}

int DB_CommitEx(LONG& errcode, _ConnectionPtr& connect, UINT& response)
{
	int nResult = 0;

	try{
		TESTHR(connect->CommitTrans());
		nResult = 1;
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database commit error!"));
		errcode = e.Error();
		UwlLogFile(_T("database commit error!"));
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
}

int DB_RollbackEx(LONG& errcode, _ConnectionPtr& connect, UINT& response)
{
	int nResult = 0;

	try{
		TESTHR(connect->RollbackTrans());
		nResult = 1;
	}catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		nResult = -1;
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database rollback error!"));
		errcode = e.Error();
		UwlLogFile(_T("database rollback error!"));
		LogProviderError(connect);
		LogComError(e);
	}
	return nResult;
}
 

long DBSP_Execute3(LONG& errcode, _ConnectionPtr& connect, UINT& response,  TCHAR* lpszName, SAFEARRAY *psaFields,int& nOutput)
{	
    try{
        _CommandPtr   pCmdPtr = NULL;
        _RecordsetPtr pRecordset = NULL;
		TESTHR(pCmdPtr.CreateInstance(__uuidof(Command)));

		pCmdPtr->ActiveConnection = connect;
        pCmdPtr->CommandType = adCmdStoredProc;
		pCmdPtr->CommandText = lpszName ;
		pCmdPtr->Parameters->Refresh();

		long lBound, uBound = 0;
		HRESULT hr = 0;
		// Getting Safe Array's Lower and Upper Bounds
		hr = SafeArrayGetLBound(psaFields, 1, &lBound);
   		hr = SafeArrayGetUBound(psaFields, 1, &uBound);

		variant_t vtParamVal;
		_variant_t Index;
		Index.vt = VT_I2;
		Index.iVal = 1 ;
		for(long i = lBound; i <= uBound; i++){
			hr = SafeArrayGetElement(psaFields, &i, &vtParamVal);
			pCmdPtr->GetParameters()->GetItem(Index)->PutValue(vtParamVal);

			Index.iVal++;
		}
 		pCmdPtr->Execute(NULL, NULL, adCmdStoredProc);


		Index.iVal = uBound-lBound + 1 ;
		nOutput=pCmdPtr->Parameters->GetItem(Index)->Value.lVal;
		
		pCmdPtr.Release();
   		return 1;
    }catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database stored procedure error!"));
		errcode = e.Error();
		UwlLogFile(_T("database stored procedure error!"));
		UwlLogFile(_T("DBSP name: %s"), lpszName);
		LogProviderError(connect);
		LogComError(e);
		return 0;
    }
}

long DBSP_Execute2(LONG& errcode, _ConnectionPtr& connect, UINT& response, 
				  TCHAR* lpszName, SAFEARRAY *psaFields,int& effected)
{	
    try{
        _CommandPtr   pCmdPtr = NULL;
        _RecordsetPtr pRecordset = NULL;
		TESTHR(pCmdPtr.CreateInstance(__uuidof(Command)));

		pCmdPtr->ActiveConnection = connect;
        pCmdPtr->CommandType = adCmdStoredProc;
		pCmdPtr->CommandText = lpszName ;
		pCmdPtr->Parameters->Refresh();

		long lBound, uBound = 0;
		HRESULT hr = 0;
		// Getting Safe Array's Lower and Upper Bounds
		hr = SafeArrayGetLBound(psaFields, 1, &lBound);
   		hr = SafeArrayGetUBound(psaFields, 1, &uBound);

		variant_t vtParamVal;
		_variant_t Index;
		Index.vt = VT_I2;
		Index.iVal = 1 ;
		for(long i = lBound; i <= uBound; i++){
			hr = SafeArrayGetElement(psaFields, &i, &vtParamVal);
			pCmdPtr->GetParameters()->GetItem(Index)->PutValue(vtParamVal);
			Index.iVal++;
		}
        _variant_t vEffected;
 		pCmdPtr->Execute(&vEffected, NULL, NULL);
		effected=vEffected.lVal;

		pCmdPtr.Release();
 		return 1;
    }catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database stored procedure error!"));
		errcode = e.Error();
		UwlLogFile(_T("database stored procedure error!"));
		UwlLogFile(_T("DBSP name: %s"), lpszName);
		LogProviderError(connect);
		LogComError(e);
		return 0;
    }
}
long DBSP_Execute(LONG& errcode, _ConnectionPtr& connect, UINT& response, 
				  TCHAR* lpszName, SAFEARRAY *psaFields)
{	
    try{
        _CommandPtr   pCmdPtr = NULL;
        _RecordsetPtr pRecordset = NULL;
		TESTHR(pCmdPtr.CreateInstance(__uuidof(Command)));

		pCmdPtr->ActiveConnection = connect;
        pCmdPtr->CommandType = adCmdStoredProc;
		pCmdPtr->CommandText = lpszName ;
		pCmdPtr->Parameters->Refresh();

		long lBound, uBound = 0;
		HRESULT hr = 0;
		// Getting Safe Array's Lower and Upper Bounds
		hr = SafeArrayGetLBound(psaFields, 1, &lBound);
   		hr = SafeArrayGetUBound(psaFields, 1, &uBound);

		variant_t vtParamVal;
		_variant_t Index;
		Index.vt = VT_I2;
		Index.iVal = 1 ;
		for(long i = lBound; i <= uBound; i++){
			hr = SafeArrayGetElement(psaFields, &i, &vtParamVal);
			pCmdPtr->GetParameters()->GetItem(Index)->PutValue(vtParamVal);
			Index.iVal++;
		}
        _variant_t vEffected;
  		pCmdPtr->Execute(&vEffected, NULL, NULL);

		pCmdPtr.Release();
  		return 1;
    }catch(_com_error e){
		TraceProviderError(connect);
		TraceComError(e);
		response = UR_DATABASE_ERROR;
		UwlTrace(_T("database stored procedure error!"));
		errcode = e.Error();
		UwlLogFile(_T("database stored procedure error!"));
		UwlLogFile(_T("DBSP name: %s"), lpszName);
		LogProviderError(connect);
		LogComError(e);
		return 0;
    }
}
int  RetrieveDBFields ( TCHAR *buf, TCHAR **fields, int maxfields, TCHAR**buf2 )
{
	if ( buf == NULL )
		return 0;

	TCHAR *p;
	p = buf;
	int count = 0;
  
	try{
	while ( 1 ) {
		fields [ count ++ ] = p;
		while ( *p != '|' && *p != '\0' ) p++;
		if ( *p == '\0' || count >= maxfields ) 
			break;
		*p = '\0';
		p++;
		}
	}catch(...)
	{
	    buf2=NULL;
		return 0;
	}

	if ( *p == '\0' ) 
		*buf2 = NULL;
	else
		*buf2 = p+1;
	*p = '\0';

	return count;
}


CString DB_GetConnectStr(CHUNK_DB* pDBS)
{ 
	CString sRet;
	if(pDBS->nSecurityMode==1)
	{
 		sRet.Format(_T("Provider=sqloledb;Integrated Security=SSPI;Persist Security Info=True;Data Source=%s;Initial Catalog=%s;"),	pDBS->szSource,	pDBS->szCatalog);
	}
	else
	{
 		sRet.Format(_T("Provider=sqloledb;Data Source=%s;Initial Catalog=%s;User Id=%s;Password=%s;"),pDBS->szSource,pDBS->szCatalog, pDBS->szUserName, pDBS->szPassword);

	}
  	return sRet;
}



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


/*
BOOL  ConnectMainDB(_ConnectionPtr& connect)
{
 	CString sConnect=DB_GetConnectStr(&g_DBServer[0]);
	if(!DB_SafeOpen(connect, sConnect))
		return FALSE;
	return TRUE;
} 


int GetDBServerIndex(int nType,LPCTSTR lpszDBIP)
{
	for(int i=0;i<MAX_TOTALDB_COUNT;i++)
	{
		if(lstrcmpi(g_DBServer[i].szSource ,lpszDBIP)==0 && g_DBServer[i].nType==nType)
		{
			return i;
		}
	}
	return -1;
}


int GetGameDBServerIndex( LPCTSTR lpszDBIP)
{
	for(int i=0;i<MAX_GAMEDB_COUNT;i++)
	{
		CString sDBIP;
		sDBIP.Format(_T("%s"),g_GameDBServer[i].szSource);
		int nFind=sDBIP.Find(',');
		if(nFind!=-1)
			sDBIP=sDBIP.Left(nFind);

		if(lstrcmpi(sDBIP ,lpszDBIP)==0 )
		{
			return i;
		}
	}
	return -1;
}

*/

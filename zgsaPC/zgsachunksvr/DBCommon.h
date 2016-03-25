#pragma once

#define MAX_SQL_LENGTH	1024

#define DB_CONNECT_ERROR  -1

#if defined(UWL_TRACE_SQL)
#define	UwlTraceSql(sql) UwlTrace(sql)
#else
#define	UwlTraceSql(sql) (0L)
#endif

inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};

#if defined(_UWL_TRACE) | defined(UWL_TRACE)
#define	TraceProviderError(c) _TraceProviderError(c)
#define	TraceComError(e) _TraceComError(e)
#else
#define	TraceProviderError(c) (0L)
#define	TraceComError(e) (0L)
#endif

void _TraceProviderError(_ConnectionPtr pConnection);
void _TraceComError(_com_error &e);

void LogProviderError(_ConnectionPtr pConnection);
void LogComError(_com_error &e);

BOOL DB_GetSqlState(_ConnectionPtr pConnection, LPTSTR lpszSqlSate);

int DB_Connect(_ConnectionPtr& connect, LPCTSTR lpszConnect);
int DB_Disconnect(_ConnectionPtr& connect);
CString  DB_GetConnectStr(CHUNK_DB* pDBS);
BOOL DB_SafeOpen(_ConnectionPtr& connect, LPCTSTR lpszConnect);
BOOL DB_SafeClose(_ConnectionPtr& connect);

int DB_BeginTrans(LONG& errcode, _ConnectionPtr& connect, UINT& response);
int DB_Commit(LONG& errcode, _ConnectionPtr& connect, UINT& response);
int DB_Rollback(LONG& errcode, _ConnectionPtr& connect, UINT& response);

int DB_BeginTransEx(LONG& errcode, _ConnectionPtr& connect, UINT& response);
int DB_CommitEx(LONG& errcode, _ConnectionPtr& connect, UINT& response);
int DB_RollbackEx(LONG& errcode, _ConnectionPtr& connect, UINT& response);

long DBSP_Execute(LONG& errcode, _ConnectionPtr& connect, UINT& response, 
				  TCHAR* lpszName, SAFEARRAY *psaFields);

long DBSP_Execute2(LONG& errcode, _ConnectionPtr& connect, UINT& response, 
				  TCHAR* lpszName, SAFEARRAY *psaFields,int& effected);

long DBSP_Execute3(LONG& errcode, _ConnectionPtr& connect, UINT& response, 
				  TCHAR* lpszName, SAFEARRAY *psaFields,int& nOutput);//约定最后一个变量是输出

int  RetrieveDBFields ( TCHAR *buf, TCHAR **fields, int maxfields, TCHAR**buf2 );


int DB_TestConnect(LONG& errcode, _ConnectionPtr& connect, UINT& response);
int DB_TestConnectEx(LONG& errcode, _ConnectionPtr& connect, UINT& response, int gameid);

/*
BOOL ConnectMainDB(_ConnectionPtr& connect);
int GetDBServerIndex(int nType,LPCTSTR lpszDBIP);
int GetDBServerIndexByType(int nType);
int GetGameDBServerIndex( LPCTSTR lpszDBIP);
*/

#pragma once
	 
int DB_ValidateClientEx(LONG& errcode, _ConnectionPtr& connect, UINT& response,  LPVALIDATE_CLIENT_EX lpValidateClientEx);
BOOL DB_FindInValidWord(LPCTSTR lpszName);

int DB_GetPropInfo(LONG& errcode, _ConnectionPtr& connect, UINT& response, int userid, GET_PROPINFO_OK& prop);
int DB_UseLuckyCard(LONG& errcode, _ConnectionPtr& connect, UINT& response, int userid, USE_LUCKCARD_OK& luckcard);

//读写datetime类型数据
COleDateTime ReadDateTime(_RecordsetPtr& pRecordset, LPCTSTR lpszField);
void WriteDateTimeForNow(_RecordsetPtr& pRecordset, LPCTSTR lpszField);
void WriteDateTime(_RecordsetPtr& pRecordset, LPCTSTR lpszField, CTime& time);
void WriteDateTime(_RecordsetPtr& pRecordset, LPCTSTR lpszField, int nUnixTime);

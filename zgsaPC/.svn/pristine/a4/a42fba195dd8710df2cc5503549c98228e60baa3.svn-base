#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRoomServer::CRoomServer(const BYTE key[] , const ULONG key_len , DWORD flagEncrypt , DWORD flagCompress ) 
		: CRoomOpenServer(key, key_len, flagEncrypt, flagCompress)
{

}

CRoomServer::CRoomServer(int nKeyType, DWORD flagEncrypt, DWORD flagCompress)
	: CRoomOpenServer(nKeyType, flagEncrypt, flagCompress)
{
   	
}

CRoomServer::~CRoomServer()
{

}
// Main.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TCHAR			g_szConnect[MAX_PATH];

// 唯一的应用程序对象
CWinApp theApp;

HINSTANCE		g_hResDll = NULL;

using namespace std;

void WriteMiniDMP(struct _EXCEPTION_POINTERS *pExp)
{
	CString   strDumpFile;
	TCHAR szFilePath[MAX_PATH];
	GetModuleFileName(NULL,szFilePath,MAX_PATH);
	*strrchr(szFilePath,'\\')=0;
    strDumpFile.Format("%s\\%d.dmp",szFilePath,CTime::GetCurrentTime().GetTickCount());
    HANDLE   hFile   =   CreateFile(strDumpFile,   GENERIC_WRITE,   FILE_SHARE_WRITE,   NULL,   CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,   NULL   ); 
    if   (hFile!=INVALID_HANDLE_VALUE) 
    { 
        MINIDUMP_EXCEPTION_INFORMATION   ExInfo; 
        ExInfo.ThreadId   =   ::GetCurrentThreadId(); 
        ExInfo.ExceptionPointers   =   pExp; 
        ExInfo.ClientPointers   =   NULL; 
        //   write   the   dump 
        BOOL   bOK   =   MiniDumpWriteDump(GetCurrentProcess(),   GetCurrentProcessId(),   hFile,   MiniDumpNormal,   &ExInfo,   NULL,   NULL   ); 
        CloseHandle(hFile); 
    } 
}

LONG WINAPI ExpFilter(struct _EXCEPTION_POINTERS *pExp)  
{  
	WriteMiniDMP(pExp);
    return EXCEPTION_EXECUTE_HANDLER;  
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	::SetUnhandledExceptionFilter(ExpFilter);

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)){
		// TODO: 更改错误代码以符合您的需要
		MessageBox(NULL, _T("Fatal error: MFC initialization failed!\n"), APPLICATION_TITLE, MB_ICONSTOP);
		nRetCode = -1;
		return nRetCode;
	}

	if (!UwlInit()){
		// TODO: 更改错误代码以符合您的需要
		MessageBox(NULL, _T("Fatal error: UWL initialization failed!\n"), APPLICATION_TITLE, MB_ICONSTOP);
		nRetCode = -1;
		return nRetCode;
	}

	DWORD dwTraceMode = UWL_TRACE_DATETIME | UWL_TRACE_FILELINE | UWL_TRACE_NOTFULLPATH
						| UWL_TRACE_FORCERETURN | UWL_TRACE_CONSOLE;
					//	| UWL_TRACE_FORCERETURN | UWL_TRACE_DUMPFILE | UWL_TRACE_CONSOLE;

	UwlBeginTrace((TCHAR*)AfxGetAppName(), dwTraceMode);
	UwlBeginLog((TCHAR*)AfxGetAppName());

	if(!AfxSocketInit()){
		MessageBox(NULL, _T("Fatal error: Failed to initialize sockets!\n"), APPLICATION_TITLE, MB_ICONSTOP);
		nRetCode = -1;
		return nRetCode;
	}

#ifdef UWL_SERVICE

	CMainService MainService(STR_SERVICE_NAME, STR_DISPLAY_NAME, 0, 0);
    
    if (!MainService.ParseStandardArgs(argc, argv)) {
        // Didn't find any standard args so start the service
        // Uncomment the DebugBreak line below to enter the debugger when the service is started.
        //DebugBreak();
        MainService.StartService();
    }
	// When we get here, the service has been stopped
    nRetCode = MainService.m_Status.dwWin32ExitCode;
#else
	
	CMainServer MainServer;
		
	if(FALSE == MainServer.Initialize()){
		UwlTrace(_T("server initialize failed!"));
	}

	UwlTrace( "Type 'q' when you want to exit. " );
	TCHAR ch;
	do{
	  ch = _getch();
	  ch = toupper( ch );
	} while( ch != 'Q' );

	MainServer.Shutdown();

	nRetCode = 1;
#endif
	

	UwlEndLog();
 
	UwlEndTrace();

	UwlTerm();
	return nRetCode;
}

void CopyMapClientToken(CClientTokenMap& mapClientToken)
{
 	CAutoLock lock(&g_csGameSvrToken);

	int client = 0;
	LONG token = 0;
	POSITION pos = g_mapGameSvrToken.GetStartPosition();
	while (pos)	
	{
		g_mapGameSvrToken.GetNextAssoc(pos, client, token);
		mapClientToken.SetAt(client, token);
	}
 
}

int CreateRandomIDEx(CIDArray& aryID, int nMaxNum, int seed)
{
	CIDArray aryTemp;

	srand( (unsigned)time( NULL ) + seed * 1000);
	
	int count = 0;
	while(aryID.GetSize() < nMaxNum){
		int result = rand();
		
		count++;
		
		BOOL found = FALSE;
		for(int i = 0; i < aryID.GetSize(); i++){
			if(aryID[i] == result){
				found = TRUE;
				break;
			}
		}
		if(!found){
			aryID.Add(result);
			for(int j = 0; j < aryTemp.GetSize(); j++){
				if(aryTemp[j] > result){
					break;
				}
			}
			aryTemp.InsertAt(j, result);
		}
	}
	for(int i = 0; i < aryID.GetSize(); i++){
		for(int j = 0; j < aryTemp.GetSize(); j++){
			if(aryID[i] == aryTemp[j]){
				aryID[i] = j;
			}
		}
	}
	return 1;
}

DWORD ComboTableChairStatus(int nTalbeNO, int nChairStatus[], int nChairCount)
{
	ASSERT(nChairCount<=16);
	DWORD dwTableChairStatus = ((DWORD)nTalbeNO)<<16;
	for (int i=0;i<nChairCount;i++)
	{
		dwTableChairStatus|=(nChairStatus[i]<<i);
	}

	return dwTableChairStatus;
}


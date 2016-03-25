// AssitSvr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{	
	DWORD dwTraceMode = UWL_TRACE_DATETIME | UWL_TRACE_FILELINE | UWL_TRACE_NOTFULLPATH
						| UWL_TRACE_FORCERETURN | UWL_TRACE_CONSOLE;
	//UwlBeginTrace(PRODUCT_NAME, dwTraceMode);
	if( !XygInitNoRes(PRODUCT_NAME, dwTraceMode))
		return 0; 
#ifdef UWL_SERVICE
	CString sDisplayName;
	BOOL bChinese=(GetUserDefaultLangID()==0x804);
	sDisplayName=(bChinese?STR_DISPLAY_NAME:STR_DISPLAY_NAME_ENU);

	CAssitService MainService(STR_SERVICE_NAME, sDisplayName, 2, 0,
							PRODUCT_LICENSE, PRODUCT_NAME, PRODUCT_VERSION, 
							PORT_OF_ASSITSVR, GAME_ID, ENCRYPT_AES, 0);

    if (!MainService.ParseStandardArgs(argc, argv)) {
        // Didn't find any standard args so start the service
        // Uncomment the DebugBreak line below to enter the debugger when the service is started.
        //DebugBreak();
        MainService.StartService();
    }
	// When we get here, the service has been stopped
    int nRetCode = MainService.m_Status.dwWin32ExitCode;
#else
	
	CAssitServer MainServer(PRODUCT_LICENSE, PRODUCT_NAME, PRODUCT_VERSION, 
					PORT_OF_ASSITSVR, GAME_ID, ENCRYPT_AES, 0);
	
	if(FALSE == MainServer.Initialize()){
		UwlTrace(_T("server initialize failed!"));
	}

	UwlTrace( "Type 'q' when you want to exit. " );
	TCHAR ch;
	do{
		ch = _getch();
		ch = toupper( ch );
		if (ch =='T')
		{
			LPCONTEXT_HEAD pContext = new CONTEXT_HEAD();
			MainServer.QueryUserLogon(pContext,72374);
		}
	} while( ch != 'Q' );
	
	MainServer.Shutdown();
	
#endif

	
	XygTermNoRes();
	//UwlEndTrace();
	return 1;
}

HINSTANCE XygInit(LPCTSTR lpszAppTitle, DWORD dwTraceMode, BOOL bNoResDll)
{
	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)){
		// TODO: ���Ĵ�������Է���������Ҫ
		MessageBox(NULL, _T("Fatal error: MFC initialization failed!\n"), lpszAppTitle, MB_ICONSTOP);
		return NULL;
	}
	if (!UwlInit()){
		// TODO: ���Ĵ�������Է���������Ҫ
		MessageBox(NULL, _T("Fatal error: UWL initialization failed!\n"), lpszAppTitle, MB_ICONSTOP);
		return NULL;
	}
	UwlBeginTrace((TCHAR*)AfxGetAppName(), dwTraceMode);
	UwlBeginLog((TCHAR*)AfxGetAppName());
	
	if(!AfxSocketInit()){
		MessageBox(NULL, _T("Fatal error: Failed to initialize sockets!\n"), lpszAppTitle, MB_ICONSTOP);
		return NULL;
	}
	if(!bNoResDll){
		TCHAR szResDllName[MAX_PATH];
		lstrcpy(szResDllName, lpszAppTitle);
		lstrcat(szResDllName, RESOURCE_DLL_EXT);
		
		HINSTANCE hResDll = AfxLoadLibrary(szResDllName);
		if(!hResDll){
			MessageBox(NULL, _T("Fatal error: Can not load resource dll!\n"), lpszAppTitle, MB_ICONSTOP);
			return NULL;
		}
		AfxSetResourceHandle(hResDll);
		return hResDll;
	}else{
		return (HINSTANCE)1;
	}
}

void XygTerm(HINSTANCE hResDll, BOOL bNoResDll)
{
	if(!bNoResDll){
		if(hResDll){
			AfxFreeLibrary(hResDll);
		}
	}
	UwlEndLog();
	UwlEndTrace();
	UwlTerm();
}

int XygInitNoRes(LPCTSTR lpszAppTitle, DWORD dwTraceMode)
{
	return (int)XygInit(lpszAppTitle, dwTraceMode, TRUE);
}

void XygTermNoRes()
{
	XygTerm(NULL, TRUE);
}

const DWORD dwWaitFinished = 5000; // time to wait for threads to finish up

CAssitService::CAssitService(const TCHAR* szServiceName, const TCHAR* szDisplayName,
						   const int iMajorVersion, const int iMinorVersion,
						   const TCHAR* szLicenseFile,
						   const TCHAR* szProductName,
						   const TCHAR* szProductVer,
						   const int nListenPort, const int nGameID,
						   DWORD flagEncrypt, DWORD flagCompress)
	:CNTService(szServiceName, szDisplayName, iMajorVersion, iMinorVersion)
{
	m_iStartParam = 0;
	m_iIncParam = 1;
	m_iState = m_iStartParam;

	lstrcpy(m_szLicenseFile, szLicenseFile);
	lstrcpy(m_szProductName, szProductName);
	lstrcpy(m_szProductVer, szProductVer);
	m_nListenPort = nListenPort;
	m_nGameID = nGameID;
	m_flagEncrypt = flagEncrypt;
	m_flagCompress = flagCompress;
}

CAssitService::~CAssitService()
{
	
}

BOOL CAssitService::OnInit()
{
	// Read the registry parameters
    // Try opening the registry key:
    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\Parameters
    HKEY hkey;
	TCHAR szKey[1024];
	_tcscpy(szKey, _T("SYSTEM\\CurrentControlSet\\Services\\"));
	_tcscat(szKey, m_szServiceName);
	_tcscat(szKey, _T("\\Parameters"));
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     szKey,
                     0,
                     KEY_QUERY_VALUE,
                     &hkey) == ERROR_SUCCESS) {
        // Yes we are installed
        DWORD dwType = 0;
        DWORD dwSize = sizeof(m_iStartParam);
        RegQueryValueEx(hkey,
                        _T("Start"),
                        NULL,
                        &dwType,
                        (BYTE*)&m_iStartParam,
                        &dwSize);
        dwSize = sizeof(m_iIncParam);
        RegQueryValueEx(hkey,
                        _T("Inc"),
                        NULL,
                        &dwType,
                        (BYTE*)&m_iIncParam,
                        &dwSize);
        RegCloseKey(hkey);
    }

	// Set the initial state
	m_iState = m_iStartParam;

	return TRUE;
}

void CAssitService::Run()
{
	m_dwThreadId = GetCurrentThreadId();
	
	m_pMainServer = OnNewServer();
	if(FALSE == m_pMainServer->Initialize()){
		UwlTrace(_T("server initialize failed!"));
		PostQuitMessage(0);
	}

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)){
		DispatchMessage(&msg);
	}

	m_pMainServer->Shutdown();

	SAFE_DELETE(m_pMainServer);
	
	// Sleep for a while
	UwlTrace(_T("service is sleeping to finish(%lu)..."), m_iState);
	Sleep(dwWaitFinished); //wait for any threads to finish
	
	// Update the current state
	m_iState += m_iIncParam;
    
}

// Called when the service control manager wants to stop the service
void CAssitService::OnStop()
{
    UwlTrace(_T("CAssitService::OnStop()"));

	PostThreadMessage(m_dwThreadId, WM_QUIT, 0, 0);
}

// Process user control requests
BOOL CAssitService::OnUserControl(DWORD dwOpcode)
{
    switch (dwOpcode) {
    case SERVICE_CONTROL_USER + 0:

        // Save the current status in the registry
        SaveStatus();
        return TRUE;

    default:
        break;
    }
    return FALSE; // say not handled
}

// Save the current status in the registry
void CAssitService::SaveStatus()
{
    UwlTrace(_T("Saving current status"));
    // Try opening the registry key:
    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\...
    HKEY hkey = NULL;
	TCHAR szKey[1024];
	_tcscpy(szKey, _T("SYSTEM\\CurrentControlSet\\Services\\"));
	_tcscat(szKey, m_szServiceName);
	_tcscat(szKey, _T("\\Status"));
    DWORD dwDisp;
	DWORD dwErr;
    UwlTrace(_T("Creating key: %s"), szKey);
    dwErr = RegCreateKeyEx(	HKEY_LOCAL_MACHINE,
                           	szKey,
                   			0,
                   			_T(""),
                   			REG_OPTION_NON_VOLATILE,
                   			KEY_WRITE,
                   			NULL,
                   			&hkey,
                   			&dwDisp);
	if (dwErr != ERROR_SUCCESS) {
		UwlTrace(_T("Failed to create Status key (%lu)"), dwErr);
		return;
	}	

    // Set the registry values
	UwlTrace(_T("Saving 'Current' as %ld"), m_iState); 
    RegSetValueEx(hkey,
                  _T("Current"),
                  0,
                  REG_DWORD,
                  (BYTE*)&m_iState,
                  sizeof(m_iState));


    // Finished with key
    RegCloseKey(hkey);
}

CAssitServer* CAssitService::OnNewServer()
{
	CAssitServer* pMainServer = new CAssitServer(m_szLicenseFile, m_szProductName, m_szProductVer,	
											m_nListenPort, m_nGameID, 
											m_flagEncrypt, m_flagCompress);
	return pMainServer;
}



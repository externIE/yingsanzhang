// MainFrame.cpp: implementation of the CMainFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainFrame.h"
#include "Game.h"

extern BOOL				m_bThreadStart;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMainFrame, CGameFrameWnd)

CMainFrame::CMainFrame()
{

}

CMainFrame::~CMainFrame()
{

}

BEGIN_MESSAGE_MAP(CMainFrame, CGameFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{ 
	if (CGameFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	  	////////  m_wndDialogBar        //////////////////
	if(!CreateDialogBar())
	{
		TRACE0("Failed to create dialog bar\n");
		return -1;
	}

	return 0;
}

BOOL CMainFrame::CreateDialogBar()
{
   	if (!m_wndDlgBar.Create(this, IDD_DLGBAR,
		CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_DLGBAR))
		return FALSE;
	return TRUE;
	
} 

CDialogBar* CMainFrame::GetGameDlgBar()
{
	return &m_wndDlgBar;
}


void CMainFrame::OnClose() 
{ 
	m_bThreadStart = FALSE;
	//Modify by chenyang
	CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
	
	if (pGameApp->IsMatch())
	{
		if (pGameApp->IsMatchStart()
			&&!pGameApp->IsMatchEnd())
		{
			pGameApp->GetGameFrame()->OnQuitMatch();
			return;
		}
	}
	else
	{
		if(!pGameApp->IsLookOn())
		{
			if(pGameApp->GetGameConnect()->GC_QueryBreak())//�˳���Ҫ�۷ֻ��߿���
			{
				
// 				DWORD dwLastTick=pGameApp->GetGameInfo()->m_dwLastAskExit;
// 				if (pGameApp->IsGameRunning()//��Ϸ��
// 					&&dwLastTick>0
// 					&&GetTickCount()-dwLastTick<DEF_ASKEXIT_WAIT*1000)
// 				{
// 					DWORD rest=(GetTickCount()-dwLastTick)/1000;
// 					
// 					CString cs;
// 					cs.Format((" <c=%d>�Ѿ������˳��������ĵȺ�������һ�Ӧ(%d��)��\r\n"),RGB(88,88,44),DEF_ASKEXIT_WAIT-rest);
// 					pGameApp->ShowMsgInChatView(cs);
// 					return;
// 				}
// 				
// 				if (pGameApp->IsAllowAskExit())
// 				{
// 					pGameApp->GetGameFrame()->ShowAskExitDlg();
// 					return;
// 				}
 				
				int nLifeTime=5;
				if(pGameApp->IsForbidDesert())//��ֹǿ��
 				{
 					pGameApp->ShowTimerMsgBox(_T("�������Ʋ����뿪!������ע����������!"),NULL,NULL,nLifeTime,TIMERDLG_FLAGS_ICON|TIMERDLG_FLAGS_DANYMIC,"xyf����");
 					return ;
 				}
			}
		}
	}
	
	if(pGameApp->IsSoloRoom())
	{
		int nLifeTime=5;
		CString sRet;
		if(pGameApp->GetGameConnect()->GC_LeaveGameEx(sRet,pGameApp->GetUserID(),pGameApp->GetChairNO()))
		{
			pGameApp->ShowTimerMsgBox(sRet,&(SNG_MSG)CreateMsg(),&(SNG_MSG)CreateMsg(),nLifeTime);
			return ;
		}
	}
	else
	{
		pGameApp->GetGameConnect()->GC_LeaveGame(pGameApp->GetUserID(),pGameApp->GetChairNO());
	}
	
	pGameApp->NotifyRoomWhenExit(AfxGetMainWnd()->m_hWnd);
	pGameApp->TerminateCommThread();
	//Modify end
	
	CShinyNovaWnd::OnClose();
}

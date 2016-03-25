#include "StdAfx.h"


CMyGameFrame::CMyGameFrame()
{
	
}

CMyGameFrame::~CMyGameFrame()
{
	
}

void	CMyGameFrame::LoadRes()
{
	Res_LoadPack(GetGamePath("skins\\common.xyr"));
	Res_LoadPack(GetGamePath("skins\\frame.xyr")); 
	Res_LoadPack(GetGamePath("skins\\聊天框资源包.xyr"));
	Res_LoadPack(GetGamePath("skins\\xyface_tangtang.xyr"));
	Res_LoadPack(GetGamePath("skins\\xyfaces_tangtang.xyr"));
}

void CMyGameFrame::AdjustFrame()
{
	SetSubSystem(new CMySubSystem);
	SetSubWidth(GetSubSystemWidth());
	SetSubDisToRight(GetSubSystemWidth());
	SetMainGame(new CMyGame);
}


int CMyGameFrame::GetSubSystemWidth()
{
	int nSubSystemWidth=0;

	if (m_bMaxClient)
	{
		RECT rc={0,0,0,0};
		SystemParametersInfo(SPI_GETWORKAREA, NULL,&rc,NULL);
		
		int nWidth = rc.right-rc.left;
		if (nWidth>=1280)
		{
			nSubSystemWidth = FRAME_ADVERT_WIDTH+2;
		}
		else
		{
			nSubSystemWidth=nWidth*2/9;
		}
		
		if (nSubSystemWidth<FRAME_SUBSYSTEM_WIDTH)
			nSubSystemWidth = FRAME_SUBSYSTEM_WIDTH;
	}
	else
	{
		//缩成小窗口时，右侧固定250
		nSubSystemWidth = FRAME_SUBSYSTEM_WIDTH+10;
	}
	
	
	return nSubSystemWidth;
}

void    CMyGameFrame::CreateFrame()
{ 
	CXyGameFrame::CreateFrame();
	//如果屏幕宽度过窄，自动隐藏SubSystem
	RECT WinRect={0,0,0,0};
	SystemParametersInfo(SPI_GETWORKAREA, NULL,&WinRect,NULL);
	if (WinRect.right-WinRect.left<=1024)
	{
		if (m_SplitterWnd
			&&m_SplitterWnd->IsShowSubSystem())
		{
			m_SplitterWnd->SetSplitterBarStutus(SPLITTER_HIDE_SUBYSYTEM);
			SetSubDisToRight(0);
		}
	}
}

void    CMyGameFrame::OnFitClient()
{
	CGameFrameWnd* pFrameWnd=(CGameFrameWnd*)AfxGetMainWnd();
	
	//改变尺寸时，如果屏幕宽度过窄，自动隐藏SubSystem
	if (IsMaxClient())
	{
		m_bMaxClient = FALSE;
		//缩成小窗口时，右侧固定250
		{
			SetSubWidth(FRAME_SUBSYSTEM_WIDTH+10);
			SetSubDisToRight(FRAME_SUBSYSTEM_WIDTH+10);
		}
		
		int nSmallWidth = GetSmallClientWidth();
		if (nSmallWidth<=800&&m_SplitterWnd->IsShowSubSystem())
		{
			m_SplitterWnd->SetSplitterBarStutus(SPLITTER_HIDE_SUBYSYTEM);
			SetSubDisToRight(0);
		}
		SetWindowPos(AfxGetMainWnd()->GetSafeHwnd(),HWND_NOTOPMOST,m_ptWindowRect.x,m_ptWindowRect.y,nSmallWidth,GetSmallClientHeight(),0);
		m_bnMaxClient=FALSE;
		m_btnFrameFit->LoadButtonPic(Res_GetPICStruct("btnFrameMax"));
	}
	else
	{
		m_bMaxClient = TRUE;
		//最大化时，右侧自适应
		{
			SetSubWidth(GetSubSystemWidth());
			SetSubDisToRight(GetSubSystemWidth());
		}
		
		RECT rc={0,0,0,0};
		SystemParametersInfo(SPI_GETWORKAREA, NULL,&rc,NULL);
		if (rc.right-rc.left<=800&&m_SplitterWnd->IsShowSubSystem())
		{
			m_SplitterWnd->SetSplitterBarStutus(SPLITTER_HIDE_SUBYSYTEM);
			SetSubDisToRight(0);
		}
		SetWindowPos(AfxGetMainWnd()->GetSafeHwnd(),HWND_NOTOPMOST,0,0,rc.right-rc.left,rc.bottom-rc.top,0);
		m_bnMaxClient=TRUE;
		m_btnFrameFit->LoadButtonPic(Res_GetPICStruct("btnFrameFit"));
	}
}

void CMyGameFrame::IniGameStatus()
{
	CXyGameFrame::IniGameStatus();
	
	//...
}



void CMyGameFrame::GameInstance()
{
	CXyGameFrame::GameInstance();
}

void CMyGameFrame::OnTest()
{
	CString str;
	str.Format("定局淘汰第%d轮",1);
	m_boardWashTurn->BulidString(str);
	m_boardWashTurn->Show();
	m_boardWashTurn->Updata();
}


void    CMyGameFrame::DrawBackGround()
{
	CXyGameFrame::DrawBackGround();

	if (GetMainGame())
	{
		CMyGame*pMyGame =(CMyGame*)GetMainGame();
		RECT rect=pMyGame->RUL_GetZgSaTableRect();
		int x=rect.left;
		int y=rect.top;
		int w=rect.right-rect.left;
		int h=rect.bottom-rect.top;
        int nTableSize=Sys_GetIntStatus(GAME_STATUS_TABLE_SIZE);
		
		SNG_PIC TableCloth;
		if (nTableSize==1) 
			TableCloth=Res_GetPICStruct("desk_750"); 
		else 
			TableCloth=Res_GetPICStruct("desk_600");
		
		int nTableW=TableCloth.width;
		int nTableH=TableCloth.height;
		int nTableX=x+w/2-nTableW/2;
		int nTableY=y+60+30*Sys_GetIntStatus(GAME_STATUS_TABLE_SIZE);
		SCR_SetBackGround(TableCloth,nTableX,nTableY,nTableW,nTableH);		
	}
}

int     CMyGameFrame::GetSmallClientWidth()
{
	RECT rc={0,0,0,0};
	SystemParametersInfo(SPI_GETWORKAREA, NULL,&rc,NULL);
	int nWidth = rc.right-rc.left;
	
	if (nWidth>=1280)
		return 880+FRAME_SUBSYSTEM_WIDTH;
	else
		return 880;
}

int        CMyGameFrame::GetSmallClientHeight()
{
	return 648;
}



void CMyGameFrame::NTF_LookOnAndSeatedChange()
{
	CGameWinApp* pApp=(CGameWinApp*)AfxGetApp();
	CGameBase* pSystemFrame=GetSubSystem();
	
	if (pSystemFrame)
	{
		pSystemFrame->OnMsg(CreateMsg(0,GAME_MSG_GET_NEW_POSITION,NULL));
		pSystemFrame->OnMsg(CreateMsg(0,GAME_MSG_FRESH_USER_LIST));
	}
}
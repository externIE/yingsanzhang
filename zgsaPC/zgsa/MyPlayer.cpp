#include "StdAfx.h"

CMyPlayer::CMyPlayer()
{
	m_bitBanker			= NULL;
	m_bMask				= FALSE;
	m_plMask			= NULL;
	m_nChairNO			= -1;
	m_bTakeDeposit		= FALSE;
	m_bitReady			= NULL;
	m_bitNameBoard		= NULL;
	m_bitEmptySeat		= NULL;
	m_bitBrightOrDark	= NULL;
	m_btnCompareCard	= NULL;
	m_bitLost			= NULL;
	m_bitWinAni			= NULL;
	m_bitPlayerFire		= NULL;
	m_bitTimeAni		= NULL;
	m_bitTimeAniBk		= NULL;
	m_bitOpeStatus		= NULL;
	m_bitRetStatus		= NULL;
	m_bitRetStatusBk	= NULL;
	m_pNewDigit			= NULL;
	m_nWinType			= 0;
	m_bLost				= FALSE;
}

CMyPlayer::~CMyPlayer()
{
	
}

void CMyPlayer::FitScrren()//刷新位置
{
	POINT pt = GetFrameCenter(); 
    MoveTo(pt.x,pt.y);
	
    pt = GetFaceCenter();
	m_bPlayerBoxBk->MoveTo(pt.x, pt.y);
	m_bitTimeAni->MoveTo(pt.x - 1, pt.y - 2);
	m_bitTimeAniBk->MoveTo(pt.x - 1, pt.y - 2);
	m_plPlayerFace->MoveTo(pt.x+1, pt.y-3);
	m_bitEmptySeat->MoveTo(pt.x, pt.y);
	m_digitUsedBet->MoveTo(pt.x - 7, pt.y + 40);
	
	pt = GetOpeStatusPostion();
	m_bitOpeStatus->MoveTo(pt.x,pt.y);
	
	pt = GetPlayerNameCenter();
	m_plPlayerName->MoveTo(pt.x,pt.y); 

	pt = GetNameBoardPosition();
	m_bitNameBoard->MoveTo(pt.x,pt.y);
	
    pt = GetRestBetPostion();
	m_digitRestBet->MoveTo(pt.x,pt.y); 
	
	pt = GetRetStatusPostion();
	m_bitRetStatus->MoveTo(pt.x,pt.y-3);
	m_bitRetStatusBk->MoveTo(pt.x, pt.y);

	//对家的坐标需要单独处理
	if(MYDRAWINDEX+3 == m_nDrawIndex)
		m_bitRetStatus->MoveTo(pt.x,pt.y+7);

	if (m_bitReady&&m_bitReady->IsShow())
	{
		pt = GetReadyPosition();
		m_bitReady->MoveTo(pt.x,pt.y);
		m_bitReady->Updata();
	}
	if (NULL != m_bitBanker && m_bitBanker->IsShow())
	{
		pt = GetBankerPositon();
        m_bitBanker->MoveTo(pt.x,pt.y);
	}

	if (NULL != m_bitBrightOrDark)
	{
		StopObject(m_bitBrightOrDark->GetHandle());
		pt = GetCardPositon(FALSE, 0, 0);
		m_bitBrightOrDark->MoveTo(pt.x + CARD_WIDTH_SMALL/2, pt.y);
	}

	if (NULL != m_bitLost && m_bLost)
	{
		StopObject(m_bitLost->GetHandle());
		if (m_pMainGame->m_dwGameStatus == STATUS_PLAYING_GAME)
			ShowLostLightningMask();
		pt = GetCardPositon(FALSE, 0, 0);
		m_bitLost->MoveTo(pt.x + CARD_WIDTH_SMALL/2, pt.y);
	}

	if (NULL != m_btnCompareCard && m_btnCompareCard->IsShow())
	{
		ShowCompareBtn();
	}

	if (NULL != m_bitWinAni && m_bitWinAni->IsShow())
	{
		pt = GetCardPositon(FALSE, 1, 0);
		switch (m_nWinType)
		{
		case HANDCARD_TYPE_BAOZI:
			m_bitWinAni->MoveTo(pt.x+8, pt.y+20);
			break;
		case HANDCARD_TYPE_SHUNJIN:
			m_bitWinAni->MoveTo(pt.x, pt.y+20);
			break;
		}
	}

	if (NULL != m_bitPlayerFire && m_bitPlayerFire->IsShow())
	{
		pt = GetFaceCenter();
		m_bitPlayerFire->MoveTo(pt.x, pt.y-7);
	}
    
	Updata();
}


void  CMyPlayer::Restart()
{ 
    /*ShowMask(FALSE); */
	DrawPlayerName();

    if (m_nChairNO==-1)
    {
		m_digitRestBet->Hide();
		m_digitRestBet->Updata();
    }
	HideReady();
    SetOpeStatus(PLAYER_STATUS_NULL);
	ShowBanker(FALSE);
	SetCurrentChair(FALSE);
	HideBrightOrDarkBoard();
	HideLostLightningMask();
	HideWinAni();
	HideCompareBtn();
	HidePlayerFire();
	HideTimeAni();
	
	m_bTakeDeposit=FALSE;
	m_bLost = FALSE;
}

void CMyPlayer::CreateGUI(int nDrawIndex) 
{
	CGameApp* pApp=(CGameApp*)AfxGetApp();
	m_pMainGame=(CMyGame*)GetObject(OBJECTNAME_MAIN_GAME);
	int nTableSize=GetTableSize();
	m_nDrawIndex=nDrawIndex;

	//主框体 
	m_bitBoard=CreateBitMap(Res_GetPICStruct("透明"));
	
	m_bitBoard->TransGUI();
	m_bitBoard->SetFloor(FLOOR_PLAYER_NAME);
	
    SetFrame(m_bitBoard);

	m_bitEmptySeat = CreateBitMap(Res_GetPICStruct("空位"));
	m_bitEmptySeat->SetFloor(m_bitBoard->GetZ()-0.2);
	m_bitBoard->AddChild(m_bitEmptySeat,OBJECT_LOCK_UPDATA);
	
    m_bPlayerBoxBk=CreateBitMap(Res_GetPICStruct("avdbg"));
	m_bPlayerBoxBk->SetFloor(m_bitBoard->GetZ()-0.1);
    m_bitBoard->AddChild(m_bPlayerBoxBk,OBJECT_LOCK_UPDATA);
	
	m_bitTimeAni=CreateBitMap(Res_GetPICStruct("timeAni"));
	m_bitTimeAni->SetFloor(m_bitBoard->GetZ()-0.1);
    m_bitBoard->AddChild(m_bitTimeAni,OBJECT_LOCK_UPDATA);

	m_bitTimeAniBk=CreateBitMap(Res_GetPICStruct("bk_timeAni"));
	m_bitTimeAniBk->SetFloor(m_bitBoard->GetZ()-0.1);
    m_bitBoard->AddChild(m_bitTimeAniBk,OBJECT_LOCK_UPDATA);

	//人物头像
	{
		m_plPlayerFace=CreatePlane(ZGSA_PLAYER_FACE_WIDTH,ZGSA_PLAYER_FACE_HEIGHT); 
		m_plPlayerFace->SetFloor(m_bitBoard->GetZ()+0.1);
		m_bitBoard->AddChild(m_plPlayerFace,OBJECT_LOCK_SHOW|OBJECT_LOCK_UPDATA);
	}
	
	//玩家操作状态
	{
		m_bitOpeStatus=CreateBitMap(Res_GetPICStruct("text_action_1"));
		m_bitOpeStatus->SetFloor(m_bitBoard->GetZ()+0.1);
        m_bitBoard->AddChild(m_bitOpeStatus,OBJECT_LOCK_UPDATA);

		m_bitRetStatus=CreateBitMap(Res_GetPICStruct("text_action_2"));
		m_bitRetStatus->SetFloor(m_bitBoard->GetZ()+0.2);
        m_bitBoard->AddChild(m_bitRetStatus,OBJECT_LOCK_UPDATA);

		if(m_nDrawIndex == MYDRAWINDEX+4  || m_nDrawIndex == MYDRAWINDEX+5)
			m_bitRetStatusBk=CreateBitMap(Res_GetPICStruct("text_action_bk2"));
		else
			m_bitRetStatusBk=CreateBitMap(Res_GetPICStruct("text_action_bk1"));
	
		m_bitRetStatusBk->SetFloor(m_bitBoard->GetZ()+0.1);
        m_bitBoard->AddChild(m_bitRetStatusBk,OBJECT_LOCK_UPDATA);
	}
	
	if (theApp.GetChairNO() != m_nChairNO)
	{
		PGL_MSG_CLASS(m_btnCompareCard,CXyStandButton);
		SNG_MSG msg = CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_COMPARE_INDEX1 + m_nDrawIndex-1);
		m_btnCompareCard->CreateGUI(Res_GetPICStruct("btn_bipai"),&msg);
		m_btnCompareCard->SetButtonSound(BUTTON_STATUS_TOUCH,"SndTouchButton");
		m_btnCompareCard->SetButtonSound(BUTTON_STATUS_LEFT_DOWN,"SndHitButton");
		m_btnCompareCard->SetFloor(FLOOR_BTN);
		
	}
	
	{//玩家用户栏 剩下注 已压注
		
		m_bitNameBoard=CreateBitMap(Res_GetPICStruct("namemoneybg"));
		m_bitNameBoard->SetFloor(m_bitBoard->GetZ()+0.1);
		m_bitBoard->AddChild(m_bitNameBoard,OBJECT_LOCK_UPDATA);
		
		m_plPlayerName = CreatePlane(ZGSA_PLAYER_NAME_WIDTH,ZGSA_PLAYER_NAME_HEIGHT);
		m_plPlayerName->SetTextSize(12); 
		m_plPlayerName->SetTextColor(RGB_YELLOW); 
		m_plPlayerName->SetFloor(m_bitBoard->GetZ()+0.1);
		m_bitBoard->AddChild(m_plPlayerName, OBJECT_LOCK_SHOW|OBJECT_LOCK_UPDATA);  
		
		//剩下注
		PGL_MSG_CLASS(m_digitRestBet,CMyDigit); 
		m_digitRestBet->CreateDigit("text_bet_numb",DIGIT_FLAGS_LEFT_JUSITIFY|DIGIT_FLAGS_USE_PIC_SIZE); 
		m_digitRestBet->LoadWord("两","namemoneycoin");  
		m_digitRestBet->ShowUnit("两"); 
		m_digitRestBet->SetFloor(m_bitBoard->GetZ()+0.1); 
		m_bitBoard->AddChild(m_digitRestBet, OBJECT_LOCK_UPDATA);
		
		//已压注
		PGL_MSG_CLASS(m_digitUsedBet,CMyDigit); 
		m_digitUsedBet->CreateDigit("text_bet_numb",DIGIT_FLAGS_MID_JUSITIFY|DIGIT_FLAGS_USE_PIC_SIZE); 
		m_digitUsedBet->LoadWord("两","icon_blueRet");  
		m_digitUsedBet->ShowUnit("两"); 
		m_digitUsedBet->SetWordSpace(-2);
		m_digitUsedBet->SetFloor(m_bitBoard->GetZ()+0.1); 
		m_bitBoard->AddChild(m_digitUsedBet, OBJECT_LOCK_UPDATA);
	}
	
	{
		//明、暗牌遮罩资源
		if (MYDRAWINDEX != m_nDrawIndex || theApp.IsLookOn())
		{
			m_bitBrightOrDark = CreateBitMap(Res_GetPICStruct("暗_big"));
			m_bitBrightOrDark->SetFloor(CARD_FLOOR_MASK);
			m_bitBoard->AddChild(m_bitBrightOrDark,OBJECT_LOCK_UPDATA);
		}
		else
		{
			m_bitBrightOrDark = CreateBitMap(Res_GetPICStruct("透明"));
			m_bitBrightOrDark->SetFloor(CARD_FLOOR_MASK);
			m_bitBoard->AddChild(m_bitBrightOrDark,OBJECT_LOCK_UPDATA);
		}
	}

	{
		m_bitLost = CreateBitMap(Res_GetPICStruct("被劈2"));
		m_bitLost->SetFloor(CARD_FLOOR_MASK+1);
		m_bitBoard->AddChild(m_bitLost,OBJECT_LOCK_UPDATA);
	}

	{
		m_bitWinAni = CreateBitMap(Res_GetPICStruct("豹子"));
		m_bitWinAni->SetFloor(CARD_FLOOR_MASK+1);
		m_bitBoard->AddChild(m_bitWinAni,OBJECT_LOCK_UPDATA);
	}

	{
		m_bitPlayerFire = CreateBitMap(Res_GetPICStruct("FIRE1"));
		m_bitPlayerFire->SetFloor(FLOOR_PLAYER_NAME);
		m_bitBoard->AddChild(m_bitPlayerFire, OBJECT_LOCK_UPDATA);
	}

	CString szName;
	szName.Format("玩家姓名板%d",m_nDrawIndex);
	ReName(szName);
}

BOOL CMyPlayer::OnMsg(SNG_MSG& msg)
{
	switch (msg.msgID)
	{
	case MSG_HIDE_OPE_STATUS: 
		APPLY_ShowObject(m_bitOpeStatus,-0.1,0);
		break;
	case MSG_THROW_BET_END:
		{
			PlaneObject*pl=(PlaneObject*)msg.param1;
			DeleteObject(pl);
		}
		break;
	default:
		break;
	}
	
	return TRUE;
}

void CMyPlayer::LoadPlayer(int nChairNO)
{
	if (nChairNO==-1)
		return;
	
	CGameWinApp* pApp=(CGameWinApp*)AfxGetApp(); 
	m_nChairNO=nChairNO;  
	
	//统一用一种方法打开,点击
	m_bitBoard->StopAutoMsg(MSG_INPUT_TOUCH);
	m_bitBoard->StopAutoMsg(MSG_INPUT_UNTOUCH);
	m_bitBoard->StopAutoMsg(MSG_INPUT_MOUSE_LEFT_DOWN);
	m_bitBoard->StopAutoMsg(MSG_INPUT_MOUSE_RIGHT_DOWN);
	
	//初始化数据
	DrawPlayerFace();
	DrawPlayerName();
	DrawNameBoard();
	SetCurrentChair(FALSE);
	FreshPlayerBetInfo();

#ifdef  _OFFLINE_
	FreshPlayerBetInfo();
	ShowBanker(TRUE);
	ShowReady();
#endif
	
	if (m_pMainGame->GetGameInfoEx()->IsChairReady(m_nChairNO) && !m_pMainGame->GetGameInfoEx()->IsGamePlaying())
		ShowReady();

	Show();
	Updata();
}

void CMyPlayer::UnLoadPlayer()
{
	m_bitBoard->StopAutoMsg(MSG_INPUT_TOUCH); 
	
    HideReady();
	SetOpeStatus(PLAYER_STATUS_NULL);
	ShowBanker(FALSE);
	
	PGL_HIDE_OBJECT(m_bitNameBoard);
	PGL_HIDE_OBJECT(m_digitRestBet);
	PGL_HIDE_OBJECT(m_digitUsedBet);

	if (NULL != m_bitBrightOrDark)
	{
		m_bitBrightOrDark->Hide();
		m_bitBrightOrDark->Updata();
	}

	if (NULL != m_bitLost)
	{
		m_bitLost->Hide();
		m_bitLost->Updata();
	}
	
	m_nChairNO=-1;
	m_bCurrentChair=FALSE;
	m_bMask= FALSE;
	
	m_plPlayerFace->Erasure();
	m_plPlayerFace->Updata();
	
	m_bPlayerBoxBk->Hide();
	m_bPlayerBoxBk->Updata();
	
	m_bitBoard->Hide();
	m_bitBoard->Updata();

	m_bitPlayerFire->Hide();
	m_bitPlayerFire->Updata();
	
	if (m_plMask)
	{
		m_plMask->Hide();
		m_plMask->Updata();
	}
	Hide();
	Updata();
}

POINT CMyPlayer::GetClockPosition()
{
	return GetReadyPosition();
}

POINT     CMyPlayer::GetReadyPosition()
{
	RECT rc =m_pMainGame->GetGameRect();
	int nTableSize=GetTableSize();
	int x=rc.left;
	int y=rc.top;
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;

	POINT pt; 
	if (0 == nTableSize)
	{
		switch (m_nDrawIndex)
		{
		case MYDRAWINDEX:
			pt.x=nWidth/2;
			pt.y=nHeight/2+80;
			break;
		case MYDRAWINDEX+1:
			pt.x=nWidth/2-185;
			pt.y=nHeight/2+45;
			break;
		case MYDRAWINDEX+2:
			pt.x=nWidth/2-185;
			pt.y=nHeight/2-105;
			break;
		case MYDRAWINDEX+3:
			pt.x=nWidth/2;
			pt.y=nHeight/2-145;
			break;
		case MYDRAWINDEX+4:
			pt.x=nWidth/2+195;
			pt.y=nHeight/2-105;
			break;
		case MYDRAWINDEX+5:
			pt.x=nWidth/2+195;
			pt.y=nHeight/2+45;
			break;
		}
	}
	else
	{
		switch (m_nDrawIndex)
		{
		case MYDRAWINDEX:
			pt.x=nWidth/2;
			pt.y=nHeight/2+120;
			break;
		case MYDRAWINDEX+1:
			pt.x=nWidth/2-250;
			pt.y=nHeight/2+60;
			break;
		case MYDRAWINDEX+2:
			pt.x=nWidth/2-250;
			pt.y=nHeight/2-100;
			break;
		case MYDRAWINDEX+3:
			pt.x=nWidth/2;
			pt.y=nHeight/2-165;
			break;
		case MYDRAWINDEX+4:
			pt.x=nWidth/2+265;
			pt.y=nHeight/2-100;
			break;
		case MYDRAWINDEX+5:
			pt.x=nWidth/2+265;
			pt.y=nHeight/2+60;
			break;
		}
	}
	
	return pt;
}

void  CMyPlayer::ShowReady()
{
	// 	  ShowMask(FALSE);
	if (m_nChairNO==-1)
	{
		return;
	}
	if (!m_bitReady)
	{
		m_bitReady=CreateBitMap(Res_GetPICStruct("准备"));
	}
	
	POINT pt=GetReadyPosition();
	m_bitReady->MoveTo(pt.x,pt.y);
	m_bitReady->Show();
	m_bitReady->SetAlpha(1);
	m_bitReady->Updata(); 
}

void  CMyPlayer::HideReady()
{
	
	if (m_bitReady)
	{
		m_bitReady->Hide();
		m_bitReady->Updata();
	}
}

int        CMyPlayer::GetTableSize()
{
	int nSize=Sys_GetIntStatus(GAME_STATUS_TABLE_SIZE);
	
	return nSize;
}

POINT CMyPlayer::GetFaceCenter()
{ 
	RECT rc =m_pMainGame->GetGameRect();
	int nTableSize=GetTableSize();
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;
    if (nTableSize==0)
    {
		POINT pt; 
		switch (m_nDrawIndex)
		{
		case MYDRAWINDEX:
			pt.x=nWidth/2-200;
			pt.y=nHeight/2+180;
			break;
		case MYDRAWINDEX+1:
			pt.x=nWidth/2-385;
			pt.y=nHeight/2+50;
			break;
		case MYDRAWINDEX+2:
			pt.x=nWidth/2-385;
			pt.y=nHeight/2-110;
			break;
		case MYDRAWINDEX+3:
			pt.x=nWidth/2;
			pt.y=nHeight/2-235;
			break;
		case MYDRAWINDEX+4:
			pt.x=nWidth/2+390;
			pt.y=nHeight/2-110;
			break;
		case MYDRAWINDEX+5:
			pt.x=nWidth/2+390;
			pt.y=nHeight/2+50;
			break;
		}

		return pt;
    }
	else  //大桌面
	{
		POINT pt; 
		switch (m_nDrawIndex)
		{
		case MYDRAWINDEX:
			pt.x=nWidth/2-220;
			pt.y=nHeight/2+225;
			break;
		case MYDRAWINDEX+1:
			pt.x=nWidth/2-450;
			pt.y=nHeight/2+70;
			break;
		case MYDRAWINDEX+2:
			pt.x=nWidth/2-450;
			pt.y=nHeight/2-110;
			break;
		case MYDRAWINDEX+3:
			pt.x=nWidth/2;
			pt.y=nHeight/2-265;
			break;
		case MYDRAWINDEX+4:
			pt.x=nWidth/2+460;
			pt.y=nHeight/2-110;
			break;
		case MYDRAWINDEX+5:
			pt.x=nWidth/2+460;
			pt.y=nHeight/2+70;
			break;
		}

		return pt;
	}
}

POINT      CMyPlayer::GetFrameCenter()
{
	POINT ptBase=GetFaceCenter();
	return ptBase;
}

POINT  CMyPlayer::GetPlayerNameCenter()
{
	POINT ptBase=GetFaceCenter();  
	int x=ptBase.x;
	int y=ptBase.y;
	
	POINT pt; 
	if (m_nDrawIndex==MYDRAWINDEX || m_nDrawIndex==MYDRAWINDEX+1 || m_nDrawIndex==MYDRAWINDEX+2)
	{  
		pt.x=x+105;
		pt.y=y-8;
	}
	else  
	{
		pt.x=x-75;
		pt.y=y-8;
	}
	
	return pt;
}

POINT    CMyPlayer::GetRestBetPostion()
{
	POINT pt=GetFaceCenter();
	
	if (m_nDrawIndex==MYDRAWINDEX || m_nDrawIndex==MYDRAWINDEX+1 || m_nDrawIndex==MYDRAWINDEX+2)
	{  
		pt.x=pt.x+40;
		pt.y=pt.y+9;
	}
	else  
	{
		pt.x=pt.x-140;
		pt.y=pt.y+9;
	}
	
	return pt;
}

POINT  CMyPlayer::GetThrowBetEndPositon()
{
	POINT pt = GetFaceCenter();

	pt.x -= 250;
	pt.y += 10;

	return pt;
}

POINT  CMyPlayer::GetCardPositon(BOOL bnFace,int nUnitIndex,int nCardStatus)
{
	RECT rc =m_pMainGame->GetGameRect();
	int nTableSize=GetTableSize();
	int x=rc.left;
	int y=rc.top;
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;

	POINT pt;
	
	if (nTableSize==0)
	{ 
		switch (m_nDrawIndex)
		{
		case MYDRAWINDEX:
			pt.x=nWidth/2+CARD_WIDTH_SMALL/2*(nUnitIndex-1);
			pt.y=nHeight/2+180;
			break;
		case MYDRAWINDEX+1:
			pt.x=nWidth/2-320+(CARD_WIDTH_SMALL/2)*nUnitIndex;
			pt.y=nHeight/2+110;
			break;
		case MYDRAWINDEX+2:
			pt.x=nWidth/2-320+(CARD_WIDTH_SMALL/2)*nUnitIndex;
			pt.y=nHeight/2-172;
			break;
		case MYDRAWINDEX+3:
			pt.x=nWidth/2+90+(CARD_WIDTH_SMALL/2)*nUnitIndex;
			pt.y=nHeight/2-235;
			break;
		case MYDRAWINDEX+4:
			pt.x=nWidth/2+275+(CARD_WIDTH_SMALL/2)*nUnitIndex;
			pt.y=nHeight/2-172;
			break;
		case MYDRAWINDEX+5:
			pt.x=nWidth/2+275+(CARD_WIDTH_SMALL/2)*nUnitIndex;
			pt.y=nHeight/2+110;
			break;
		}
	}
	else
	{ 
		switch (m_nDrawIndex)
		{
		case MYDRAWINDEX:
			pt.x=nWidth/2+CARD_WIDTH_SMALL/2*(nUnitIndex-1);
			pt.y=nHeight/2+230;
			break;
		case MYDRAWINDEX+1:
			pt.x=nWidth/2-385+(CARD_WIDTH_SMALL/2)*nUnitIndex;
			pt.y=nHeight/2+130;
			break;
		case MYDRAWINDEX+2:
			pt.x=nWidth/2-385+(CARD_WIDTH_SMALL/2)*nUnitIndex;
			pt.y=nHeight/2-172;
			break;
		case MYDRAWINDEX+3:
			pt.x=nWidth/2+90+(CARD_WIDTH_SMALL/2)*nUnitIndex;
			pt.y=nHeight/2-265;
			break;
		case MYDRAWINDEX+4:
			pt.x=nWidth/2+345+(CARD_WIDTH_SMALL/2)*nUnitIndex;
			pt.y=nHeight/2-172;
			break;
		case MYDRAWINDEX+5:
			pt.x=nWidth/2+345+(CARD_WIDTH_SMALL/2)*nUnitIndex;
			pt.y=nHeight/2+130;
			break;
		}
	}
	
	return pt;
 }
 
 
POINT     CMyPlayer::GetOpeStatusPostion()
{
	POINT ptBase=GetFaceCenter(); 

	POINT pt; 
	pt.x=ptBase.x+15;
	pt.y=ptBase.y+40;
	
	return pt;
}

POINT CMyPlayer::GetRetStatusPostion()
{
	int nTableSize=GetTableSize();
	POINT ptBase=GetNameBoardPosition();
	int x = ptBase.x;
	int y = ptBase.y;
	POINT pt;

	switch (m_nDrawIndex)
	{
	case MYDRAWINDEX:
		pt.x = x - 20;
		pt.y = y - 55;
		break;
	case MYDRAWINDEX+1:
	case MYDRAWINDEX+2:
		pt.x = x + 90;
		pt.y = y - 20;
		break;
	case MYDRAWINDEX+3:
		m_bitRetStatusBk->RotateToAngle(180);
		pt.x = x - 10;
		pt.y = y + 50;
		break;
	case MYDRAWINDEX+4:
	case MYDRAWINDEX+5:
		pt.x = x - 90;
		pt.y = y - 20;
		break;
	}
	return pt;
}
 
POINT  CMyPlayer::GetBankerPositon()
{
	POINT ptBase=GetNameBoardPosition();
	int x=ptBase.x;
	int y=ptBase.y;

	POINT pt; 

	switch (m_nDrawIndex)
	{
	case MYDRAWINDEX:
		pt.x=x + 45;
		pt.y=y - 40; 
		break;
	case MYDRAWINDEX+1:
	case MYDRAWINDEX+5:
		pt.x=x;
		pt.y=y-40; 
		break;
	case MYDRAWINDEX+2:
	case MYDRAWINDEX+4:
		pt.x=x;
		pt.y=y+30; 
		break;
	case MYDRAWINDEX+3:
		pt.x=x-80;
		pt.y=y-5; 
		break;
	}
	
	return pt;
}
 
 struct APPLYINFO_PLAYBIT1
 {
	 int         startframe;
	 int			 mode;
	 int			param;
	 DWORD       total;
 };
 BOOL FP_PlayBitObject1(PObject* ob, BYTE* data, GameCore* m_core, DWORD time)
 {
	 BITObject* bit=(BITObject*)ob;
	 if (!bit->GetRes()->IsValidate())//资源不符合,挂起
		 return TRUE;
	 
	 APPLYINFO_PLAYBIT1* applyinfo=(APPLYINFO_PLAYBIT1*)data;
	 
	 if (applyinfo->mode==MODE_PLAYBIT_BY_CIRCLE)
	 {
		 int nFrame=bit->GetCurrentFrame();
		 PaintModule* module=(PaintModule*)m_core->GetModule(PAINT_MODULE);
		 bit->GoToFrameByLifeTime(time,applyinfo->startframe);
		 
		 if (bit->GetCurrentFrame()!=nFrame)
			 bit->Updata();
		 
		 if (applyinfo->total!=0
			 &&time>=applyinfo->total)
		 {
			 int nLastFrame=(applyinfo->startframe+bit->GetTotalFrame()-1)%bit->GetTotalFrame();
			 bit->GoToFrame(nLastFrame);//跳到最后一帧结束
			 bit->Updata();
			 return TRUE;
		 }
	 }
	 else if (applyinfo->mode==MODE_PLAYBIT_BY_FRAMECOUNT)
	 {
		 int nFrame=bit->GetCurrentFrame();
		 bit->GoToFrameByLifeTime(time,applyinfo->startframe);
		 if (bit->GetCurrentFrame()!=nFrame)
			 bit->Updata();
		 
		 if (applyinfo->total!=0
			 &&time>=applyinfo->total)
		 {
			 return TRUE;
		 }
	 }
	 else if (applyinfo->mode==MODE_PLAYBIT_BY_TIME)
	 {
		 int nFrame=bit->GetCurrentFrame(); 
		 int nElapseBeFore=applyinfo->startframe;
		 int nTotalFrame=bit->GetTotalFrame();
		 int nFrameNext=nTotalFrame*(time+nElapseBeFore)/applyinfo->total;
		 if (nFrameNext>=nTotalFrame)
		 {
			 nFrameNext=nTotalFrame-1;
		 }
		 //bit->GoToFrameByLifeTime(,applyinfo->startframe);
		 bit->GoToFrame(nFrameNext);
		 
		 
		 if (bit->GetCurrentFrame()!=nFrame)
			 bit->Updata();
		 if (applyinfo->total!=0
			 &&(time+nElapseBeFore)>=applyinfo->total)
		 {
			 bit->GoToFrame(bit->GetTotalFrame()-1);
			 bit->Show();
			 bit->Updata();
			 return TRUE;
		 }
	 }
	 else if (applyinfo->mode==MODE_PLAYBIT_TO_FRAME)
	 {
		 bit->GoToFrame(applyinfo->param);
		 bit->Updata();
		 return TRUE;
	 }
	 else if (applyinfo->mode==MODE_PLAYBIT_STEP_BY_STEP)
	 {
		 bit->GoToNextFrame();
		 bit->Updata();
		 applyinfo->total--;
		 if (applyinfo->total<=0)
			 return TRUE;
	 }
	 
	 return FALSE;
 }
 
EVENTHANDLE      SNGAPI  CMyPlayer::APPLY_PlayBitObjectEx(BITObject* ob,
 int  mode,
 int  param,
 int  nStartFrame,
 DWORD nTimeSpace,
 SNG_MSG* CallBackMsg)
{
	if (!ob) return 0;
	APPLYINFO_PLAYBIT1 temp;
	memset(&temp,0,sizeof(APPLYINFO_PLAYBIT1));
	temp.startframe=nStartFrame;
	temp.mode=mode;
	temp.param=param; 
	temp.total=0;//={mode,param,0};
	if (mode==MODE_PLAYBIT_BY_CIRCLE)
	{
		if (param==-1)
			temp.total=0;
		else
			temp.total=ob->GetTotalDelayTime()*param;
 
		nTimeSpace=GetMinApplySpace();
	}
	else if (mode==MODE_PLAYBIT_BY_FRAMECOUNT)
	{
		temp.total=ob->GetCostDelayTime(ob->GetCurrentFrame(),ob->GetCurrentFrame()+param);
		nTimeSpace=GetMinApplySpace();
	}
	else if (mode==MODE_PLAYBIT_BY_TIME)
	{
		temp.total=param;
		temp.startframe=nStartFrame;
	}
	else if (mode=MODE_PLAYBIT_STEP_BY_STEP)
	{
		temp.total=param;
	}
	else 
	{
		temp.total=0;
	}
	APPLY* apply=GetObjectModule()->CreateApply();
	if (!apply) return 0;
	apply->ObjectPtr=ob;
	if (CallBackMsg)
		memcpy(&apply->CallBackMsg,CallBackMsg,sizeof(SNG_MSG));
	apply->fp_applyfuction=FP_PlayBitObject1;
	apply->dwSpaceCount=nTimeSpace;
	memcpy(apply->param,&temp,sizeof(APPLYINFO_PLAYBIT1));
	APPLYINFO_PLAYBIT1* applyinfo=(APPLYINFO_PLAYBIT1*)apply->param;
	GetCore()->PostQuest(OBJECT_MODULE,SNG_QUEST_CRAETE_APPLY,apply,NULL);
	return apply->EventHandle;
}
 
void CMyPlayer::SetCurrentChair(BOOL bn,int nTimeRest)
{
	 m_bCurrentChair=bn;

	 if (m_nChairNO==-1)return ;
	 int nBetWait=m_pMainGame->GetGameInfoEx()->GetBetWait();
 
	 if (nTimeRest==-1)
	 {
		 nTimeRest=nBetWait;
	 }  

	 {
		 StopObject(m_bPlayerBoxBk->GetHandle());
		 SNG_PIC pic = Res_GetPICStruct("avdbg");
		 m_bPlayerBoxBk->LoadPic(pic);
		 m_bPlayerBoxBk->Show();
		 m_bPlayerBoxBk->Updata(); 
	 }

	int nTimeSpace=50;
	if (bn)
	{
		StopObject(m_bitTimeAni->GetHandle());
		SNG_PIC  pic;

		pic=Res_GetPICStruct("timeAni");

		SetOpeStatus(PLAYER_STATUS_NULL);
		m_bitTimeAni->LoadPic(pic);
		int nTimeElapse=nBetWait-nTimeRest;
		int nTotalFrame= m_bitTimeAni->GetTotalFrame();
		int nFrameNext=nTotalFrame*nTimeElapse/m_pMainGame->GetGameInfoEx()->GetBetWait();
		if (nFrameNext>=nTotalFrame)
		{
			nFrameNext=nTotalFrame-1;
		}
		if (nFrameNext<=0)
		{
			nFrameNext=0;
		}

		m_bitTimeAni->GoToFrame(nFrameNext); 
		m_bitTimeAni->Updata(); 
		APPLY_PlayBitObjectEx(m_bitTimeAni,MODE_PLAYBIT_BY_TIME,nBetWait*1000,nTimeElapse*1000,nTimeSpace,NULL);

		PGL_SHOW_OBJECT(m_bitTimeAni);
		PGL_SHOW_OBJECT(m_bitTimeAniBk);
	}
	else
	{
		PGL_HIDE_OBJECT(m_bitTimeAni);
		PGL_HIDE_OBJECT(m_bitTimeAniBk);
	}
}
	 
 
void   CMyPlayer::DrawPlayerName()
{
	m_plPlayerName->Erasure();
	if (m_nChairNO==-1)return;
	
	UINT rgb=RGB_GREEN;
	
	if (m_bMask==TRUE)
	{
		rgb=RGB(155,155,155);
	}
	
	CString szName=theApp.m_GamePlayer[m_nChairNO].szUsername;
	m_plPlayerName->SetTextColor(rgb);
	int nTableSize=GetTableSize();
	if (nTableSize==0)
	{
		m_plPlayerName->TextOut(0,0,szName);
	}
	else
	{
		m_plPlayerName->TextOut(0,0,szName); 
	}
	
	m_plPlayerName->Updata();  
}
 
 void    CMyPlayer::DrawPlayerFace()
 {
	 m_plPlayerFace->Erasure();
	 int nClothID=theApp.m_GamePlayer[m_nChairNO].nClothingID;
	 CString szCharacterName=GetUserCharacter(nClothID,CHARACTER_TYPE_MIDDLE);
	 SNG_PIC cloth_pic=Res_GetPICStruct(szCharacterName);
	 
	 double alpha=1.0;
	 m_plPlayerFace->ScalePlane(cloth_pic.width-3,cloth_pic.height-3);
	 m_plPlayerFace->ScaleTo(cloth_pic.width-3,cloth_pic.height-3);
	 m_plPlayerFace->DrawPic(cloth_pic); 
	 m_plPlayerFace->SetAlpha(alpha);
	 m_plPlayerFace->Show();
 }
 
 void   CMyPlayer::ShowBanker(BOOL bShow)  //显示庄家标记
 {
	 if (!m_bitBanker)
	 {
         m_bitBanker = CreateBitMap(Res_GetPICStruct("icon_dealer"));
		 m_bitBanker->SetFloor(m_bitBoard->GetZ()+0.1);
		 m_bitBoard->AddChild(m_bitBanker,OBJECT_LOCK_UPDATA);
	 }
	 POINT pt=GetBankerPositon();
	 m_bitBanker->MoveTo(pt.x,pt.y);
	 if (bShow) 
		 m_bitBanker->Show(); 
	 else
		 m_bitBanker->Hide();
	 m_bitBanker->Updata();
 }
 
 void  CMyPlayer::FreshPlayerBetInfo() 
 { 
	 int nRestBet =	m_pMainGame->GetGameInfoEx()->GetChairRestDeposit(m_nChairNO); 
	 int nUsedBet = m_pMainGame->GetGameInfoEx()->GetChairUsedDeposit(m_nChairNO);

	 if (nRestBet >= 0)
	 {
		 m_digitRestBet->SetDigit(nRestBet); 
		 m_digitRestBet->Show();
		 m_digitRestBet->Updata();
	 }
	 
	 if (nUsedBet > 0)
	 {
		 m_digitUsedBet->SetDigit(nUsedBet); 
		 m_digitUsedBet->Show();
		 m_digitUsedBet->Updata();
	 }
	 else if (nUsedBet == 0)
	 {
		 PGL_HIDE_OBJECT(m_digitUsedBet);
	 }
 }
 
 void  CMyPlayer::MOVIE_FreshRestBet(int nAddBet, int nWinChair, int nWinType)
 { 
	 int nCurBet=m_digitRestBet->GetDigit();
	 int nRestBet=m_pMainGame->GetGameInfoEx()->GetChairRestDeposit(m_nChairNO);
	 //int nRestBet=theApp.m_GamePlayer[m_nChairNO].nDeposit;//
	 
#ifdef _OFFLINE_
	 if (nCurBet<=0)
	 {
		 nCurBet=100;
	 }
	 
	 nRestBet=nCurBet+100;
#endif
	 
	 {  
		 PlaySound("snd_GetBet");
		 
		 POINT ptStart=GetWinOrLoseNumPosition();
		 ptStart.y+=40;
		 POINT ptEnd;
		 ptEnd.x=ptStart.x;
		 ptEnd.y=GetWinOrLoseNumPosition().y-50;

		 //CMyDigit_Move*pNewDigit = NULL;
		 m_pNewDigit = NULL;
		 PGL_MSG_CLASS(m_pNewDigit,CMyDigit_Move);
		 
		 if (nAddBet > 0)
		 {
			 m_pNewDigit->CreateDigit("winnum",DIGIT_FLAGS_LEFT_JUSITIFY|DIGIT_FLAGS_USE_PIC_SIZE); 
			 m_pNewDigit->LoadWord("加","win+");  
			 m_pNewDigit->ShowUnit("加");
		 }
		 else
		 {
			 m_pNewDigit->CreateDigit("losenum",DIGIT_FLAGS_LEFT_JUSITIFY|DIGIT_FLAGS_USE_PIC_SIZE); 
			 m_pNewDigit->LoadWord("减","lose-");  
			 m_pNewDigit->ShowUnit("减");
		 }
		 
		 m_pNewDigit->SetFloor(FLOOR_ANIMATION+1);
		 m_pNewDigit->TransGUI();
		 m_pNewDigit->SetDigit(abs(nAddBet));
		 m_pNewDigit->Show(); 
		 m_pNewDigit->MoveTo(ptStart.x,ptStart.y);
		 m_pNewDigit->Updata();
		 
		 double speed=2;

		 BeginGroup();
		 APPLY_MoveObject(m_pNewDigit,ptEnd,speed,0,speed);
		 NextGroup();
		 APPLY_DirectShow(m_pNewDigit,FALSE);
		 EndGroup();
	 } 
	 
	 if (nRestBet>nCurBet)
	 {  
		 int nOffSet=nRestBet/20+1;
		 m_digitRestBet->ScrollDigitTo(nRestBet,nOffSet); 
		 m_digitRestBet->Updata();
	 }
	 else  if (nRestBet<nCurBet)
	 {
		 m_digitRestBet->SetDigit(nRestBet); 
		 m_digitRestBet->Updata();
	 }

	 PGL_HIDE_OBJECT(m_digitUsedBet);
 }
 
 void   CMyPlayer::MOVIE_ThrowCard()  //放弃时扔牌
 {
	 if (!m_pMainGame)return;
	 if (m_nChairNO==-1)return;

	 if (MYDRAWINDEX != m_nDrawIndex)
		HideBrightOrDarkBoard();
	 
	 CMyCard *card0,*card1,*card2;
	 card0=NULL;
	 card1=NULL;
	 card2=NULL;
	 
	 for (int i=0;i<TOTAL_CHAIRS*CHAIR_CARDS;i++)
	 { 
		 if (m_pMainGame->m_Card[i]->m_nChairNO==m_nChairNO )
		 { 
			 if (card0==NULL)
			 {
				 card0=m_pMainGame->m_Card[i];
			 }
			 else if (card1==NULL)
			 {  
				 card1=m_pMainGame->m_Card[i];
			 }
			 else if (card2 == NULL)
			 {
				 card2=m_pMainGame->m_Card[i];
				 break;
			 }
		 }
	 }
	 card0->m_bnFace=FALSE;
	 card1->m_bnFace=FALSE;
	 card2->m_bnFace=FALSE;
	 card0->Fresh();
	 card1->Fresh();
	 card2->Fresh();
	 BeginGroup();
	 POINT ptEnd=m_pMainGame->GetPokerDealPosition();
	 double speed=50;
	 APPLY_MoveObject(card0,ptEnd,speed,3,speed+20);
	 APPLY_MoveObject(card1,ptEnd,speed,3,speed+20);
	 APPLY_MoveObject(card2,ptEnd,speed,3,speed+20);
	 NextGroup();
	 APPLY_DirectShow(card0,FALSE);
	 APPLY_DirectShow(card1,FALSE);
	 APPLY_DirectShow(card2,FALSE);
	 EndGroup();
 }

void  CMyPlayer::SetOpeStatus(int nStatus)
{
	if (m_nChairNO==-1)return;
	
	int nFrame = -1;
	switch(nStatus)
	{
	case PLAYER_STATUS_NULL:
	case PLAYER_STATUS_BET_BIG:
	case PLAYER_STATUS_BET_SMALL:
	case PLAYER_STATUS_WAIT_BET:
		nFrame = -1;
		break;
	case PLAYER_STATUS_WAIT_START:
		nFrame = 1;
		break;
	case PLAYER_STATUS_LOOK_CARD:
		nFrame = 3;
		break;
	case PLAYER_STATUS_READY:
		nFrame = 5;
		break;
	case PLAYER_STATUS_FOLLOW:
		nFrame = 6;
		break;
	case PLAYER_STATUS_RISE:
		nFrame = 7;
		break;
	case PLAYER_STATUS_GIVE_UP:
		nFrame = 8;
		break;
	case PLAYER_STATUS_ALLIN:
		nFrame = 9;
		break;
	case PLAYER_STATUS_BET:
		nFrame = 10;
		break;
	case PLAYER_STATUS_BET_FULL:
		nFrame = 11;
		break;
	}
	
	if (nFrame == -1)
	{
		PGL_HIDE_OBJECT(m_bitOpeStatus);
		PGL_HIDE_OBJECT(m_bitRetStatus);
		PGL_HIDE_OBJECT(m_bitRetStatusBk);
	}
	else if(nFrame >= 1 && nFrame <= 5)
	{	
		m_bitOpeStatus->GoToFrame(nFrame);
		POINT pt=GetOpeStatusPostion();
		m_bitOpeStatus->MoveTo(pt.x,pt.y);
		m_bitOpeStatus->SetAlpha(1);
		PGL_SHOW_OBJECT(m_bitOpeStatus);
	}
	else if (nFrame >= 6 && nFrame <= 11)
	{
		PGL_SHOW_OBJECT(m_bitRetStatusBk);

		m_bitRetStatus->GoToFrame(nFrame);;
		PGL_SHOW_OBJECT(m_bitRetStatus);	
	}
}
 
 
//  void  CMyPlayer::ShowMask(BOOL bShow)
//  {
// 	    m_bMask=bShow;
// 		DrawPlayerName();
// 		//	DrawPlayerFace();
// 		
// 		if (m_nChairNO==-1)return;
// 		
// 		if (!m_plMask)
// 		{
// 			int nClothID=theApp.m_GamePlayer[m_nChairNO].nClothingID;
// 			CString szCharacterName=GetUserCharacter(nClothID,CHARACTER_TYPE_MIDDLE);
// 			SNG_PIC cloth_pic=Res_GetPICStruct(szCharacterName);
// 			
// 			m_plMask=CreatePlane(cloth_pic.width+12,cloth_pic.height+12); 
// 			m_plMask->SetFloor(m_bitBoard->GetZ()+1);
// 			
//             m_bitBoard->AddChild(m_plMask,OBJECT_LOCK_POSITION|OBJECT_LOCK_UPDATA);
// 		}
// 		
// 		if (bShow)
// 		{
// 			POINT pt;
// 			pt=GetFaceCenter();
// 			m_plMask->MoveTo(pt.x,pt.y-2); 
// 			m_plMask->SetAlpha(0.5);
// 			m_plMask->SetGroundColor(RGB(139,139,139)); 
// 			//m_plMask->SetGroundColor(RGB_BLACK);
// 			m_plMask->Erasure();
// 			//	m_plMask->ShowBorderLine(RGB_WHITE);
// 			m_plMask->Show();
// 			m_plMask->Updata();
// 		}
// 		else
// 		{
// 			m_plMask->Hide();
// 			m_plMask->Updata();
// 		}
//  }

void CMyPlayer::DrawNameBoard()
{
	if (NULL != m_bitNameBoard)
	{
		POINT pt=GetNameBoardPosition();
		m_bitNameBoard->MoveTo(pt.x,pt.y); 
		m_bitNameBoard->Show();
		m_bitNameBoard->Updata();
	}
}

POINT CMyPlayer::GetNameBoardPosition()
{
	POINT ptBase=GetFaceCenter();  
	int x=ptBase.x;
	int y=ptBase.y;
	
	POINT pt; 
	
	if (m_nDrawIndex==MYDRAWINDEX || m_nDrawIndex==MYDRAWINDEX+1 || m_nDrawIndex==MYDRAWINDEX+2)   //左 或 右
	{  
		pt.x=x+90;
		pt.y=y+5;
	}
	else  
	{
		pt.x=x-90;
		pt.y=y+5;
	}
	
	return pt;
}

void CMyPlayer::DrawEmptySeat()
{
	if (NULL != m_bitEmptySeat)
	{
		POINT pt=GetFaceCenter();
		m_bitEmptySeat->MoveTo(pt.x,pt.y); 
		m_bitEmptySeat->Show();
		m_bitEmptySeat->Updata();
	}
}

void CMyPlayer::ShowBrightOrDarkBoard(BOOL bBright, BOOL bLost)
{
	if (NULL != m_bitBrightOrDark)
	{
		if (bLost)
		{
			StopObject(m_bitBrightOrDark->GetHandle());
			m_bitBrightOrDark->LoadPic(Res_GetPICStruct("弃牌"));
		}
		else
		{
			if (MYDRAWINDEX != m_nDrawIndex || theApp.IsLookOn())
			{
				StopObject(m_bitBrightOrDark->GetHandle());
				if (bBright)
					m_bitBrightOrDark->LoadPic(Res_GetPICStruct("明_big"));
				else
					m_bitBrightOrDark->LoadPic(Res_GetPICStruct("暗_big"));
			}
		}

		POINT pt = GetCardPositon(FALSE, 0, 0);
		m_bitBrightOrDark->MoveTo(pt.x+CARD_WIDTH_SMALL/2, pt.y);
		m_bitBrightOrDark->Show();
		m_bitBrightOrDark->Updata();
		m_bitLost->MoveTo(pt.x+CARD_WIDTH_SMALL/2, pt.y);
		m_bitLost->Updata();
	}
}

void CMyPlayer::HideBrightOrDarkBoard()
{
	if (NULL != m_bitBrightOrDark)
	{
		m_bitBrightOrDark->Hide();
		m_bitBrightOrDark->Updata();
	}
}

void CMyPlayer::ShowLostLightningMask()
{
	if (NULL != m_bitLost)
	{
		m_bitLost->GoToFrame(2);
		m_bitLost->Show();
		m_bitLost->Updata();
	}
}

void CMyPlayer::HideLostLightningMask()
{
	if (NULL != m_bitLost)
	{
		m_bitLost->Hide();
		m_bitLost->Updata();
	}
}

void CMyPlayer::ShowCompareBtn()
{
	if (NULL != m_btnCompareCard)
	{
		POINT pt = GetCardPositon(FALSE, 0, 0);
		m_btnCompareCard->MoveTo(pt.x+CARD_WIDTH_SMALL/2, pt.y+CARD_HEIGHT_SMALL/2 - 10);
		m_btnCompareCard->Show();
		m_btnCompareCard->Updata();
	}
}

void CMyPlayer::HideCompareBtn()
{
	if (NULL != m_btnCompareCard)
	{
		m_btnCompareCard->Hide();
		m_btnCompareCard->Updata();
	}
}

BOOL CMyPlayer::IsShowCompareBtn()
{
	if(m_btnCompareCard->IsShow())
		return TRUE;
	else
		return FALSE;
}

POINT CMyPlayer::GetWinOrLoseNumPosition()
{
	RECT rc =m_pMainGame->GetGameRect();
	int nTableSize=GetTableSize();
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;
	
	POINT pt; 
	if (0 == nTableSize)
	{
		switch (m_nDrawIndex)
		{
		case MYDRAWINDEX:
			pt.x=nWidth/2;
			pt.y=nHeight/2+80;
			break;
		case MYDRAWINDEX+1:
			pt.x=nWidth/2-300;
			pt.y=nHeight/2+20;
			break;
		case MYDRAWINDEX+2:
			pt.x=nWidth/2-300;
			pt.y=nHeight/2-140;
			break;
		case MYDRAWINDEX+3:
			pt.x=nWidth/2;
			pt.y=nHeight/2-205;
			break;
		case MYDRAWINDEX+4:
			pt.x=nWidth/2+300;
			pt.y=nHeight/2-140;
			break;
		case MYDRAWINDEX+5:
			pt.x=nWidth/2+300;
			pt.y=nHeight/2+20;
			break;
		}
	}
	else
	{
		switch (m_nDrawIndex)
		{
		case MYDRAWINDEX:
			pt.x=nWidth/2;
			pt.y=nHeight/2+130;
			break;
		case MYDRAWINDEX+1:
			pt.x=nWidth/2-360;
			pt.y=nHeight/2+40;
			break;
		case MYDRAWINDEX+2:
			pt.x=nWidth/2-360;
			pt.y=nHeight/2-140;
			break;
		case MYDRAWINDEX+3:
			pt.x=nWidth/2;
			pt.y=nHeight/2-230;
			break;
		case MYDRAWINDEX+4:
			pt.x=nWidth/2+370;
			pt.y=nHeight/2-140;
			break;
		case MYDRAWINDEX+5:
			pt.x=nWidth/2+370;
			pt.y=nHeight/2+40;
			break;
		}
	}
	
	return pt;
}

void CMyPlayer::ShowWinAni(int nWinType)
{
	m_nWinType = nWinType;
	POINT pt = GetCardPositon(FALSE, 1, 0);
	BeginGroup();
	switch (nWinType)
	{
	case HANDCARD_TYPE_BAOZI:
		m_bitWinAni->LoadPic(Res_GetPICStruct("豹子"));
		m_bitWinAni->MoveTo(pt.x+8, pt.y+20);
		m_bitWinAni->GoToFrame(0);
		APPLY_DirectShow(m_bitWinAni, TRUE);
		APPLY_PlayBitObject(m_bitWinAni, MODE_PLAYBIT_BY_CIRCLE, 1);
		PlaySound("豹子音乐");
		NextGroup();
		break;
	case HANDCARD_TYPE_SHUNJIN:
		m_bitWinAni->LoadPic(Res_GetPICStruct("顺金"));
		m_bitWinAni->MoveTo(pt.x, pt.y+20);
		m_bitWinAni->GoToFrame(0);
		APPLY_DirectShow(m_bitWinAni, TRUE);
		APPLY_PlayBitObject(m_bitWinAni, MODE_PLAYBIT_BY_CIRCLE, 1);
		NextGroup();
		break;
	}
	EndGroup();
}

void CMyPlayer::HideWinAni()
{
	m_bitWinAni->Hide();
	m_bitWinAni->Updata();
}

void CMyPlayer::ShowPlayerFire()
{
	POINT pt = GetFaceCenter();
	m_bitPlayerFire->MoveTo(pt.x, pt.y-7);
	m_bitPlayerFire->GoToFrame(0);
	APPLY_DirectShow(m_bitPlayerFire, TRUE);
	APPLY_PlayBitObject(m_bitPlayerFire, MODE_PLAYBIT_BY_CIRCLE);
}

void CMyPlayer::HidePlayerFire()
{
	m_bitPlayerFire->Hide();
	m_bitPlayerFire->Updata();
}

void CMyPlayer::HideTimeAni()
{
	PGL_HIDE_OBJECT(m_bitTimeAni);
	PGL_HIDE_OBJECT(m_bitTimeAniBk);
}

void CMyPlayer::HideNewDigitAni()
{
	PGL_HIDE_OBJECT(m_pNewDigit);
}
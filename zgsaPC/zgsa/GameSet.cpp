#include "stdafx.h"
using namespace ShinyNova;

CGameSetBoard::CGameSetBoard()
{

}

CGameSetBoard::~CGameSetBoard()
{

}

void CGameSetBoard::OnIniData()
{
	BITObject* bit=CreateBitMap(Res_GetPICStruct("bit_gamesetboard"));
	bit->TransGUI();
	bit->SetMoveAble(TRUE);
	bit->Updata();
	SetFrame(bit);

	SNG_PIC pic = Res_GetPICStruct("复选框");
	int nX = bit->GetLeft()+40;
	int nY = bit->GetTop()+60;
	int nOffX = 40;
	int nOffY = 35;

	PGL_MSG_CLASS(m_btnMusic,CXyCheckButton);
	m_btnMusic->CreateGUI(pic,theApp.m_GameSet.nMusic);
	m_btnMusic->MoveTo(nX,nY);
	m_btnMusic->SetFloor(bit->GetZ()+0.01);
	m_btnMusic->Updata();
	PlaneObject* pl=CreatePlane(170,pic.height);
	pl->SetTextColor(0);
	pl->SetTextSize(12);
	pl->MoveTo(bit->GetX(),nY);
	pl->SetAutoMsg(MSG_INPUT_MOUSE_LEFT_DOWN,GetHandle(),SET_CALLBACK_CHECK_MUSIC);
	pl->TextOut(15,4,_T("打开背景音乐"));
	bit->AddChild(m_btnMusic,OBJECT_LOCK_ALL);
	bit->AddChild(pl,OBJECT_LOCK_ALL);
	nY+=nOffY;

	PGL_MSG_CLASS(m_btnSound,CXyCheckButton);
	m_btnSound->CreateGUI(pic,theApp.m_GameSet.nSoundEffect);
	m_btnSound->MoveTo(nX,nY);
	m_btnSound->SetFloor(bit->GetZ()+0.01);
	m_btnSound->Updata();
	pl=CreatePlane(170,pic.height);
	pl->SetTextColor(0);
	pl->SetTextSize(12);
	pl->MoveTo(bit->GetX(),nY);
	pl->SetAutoMsg(MSG_INPUT_MOUSE_LEFT_DOWN,GetHandle(),SET_CALLBACK_CHECK_SOUND);
	pl->TextOut(15,4,_T("打开游戏音效"));
	bit->AddChild(m_btnSound,OBJECT_LOCK_ALL);
	bit->AddChild(pl,OBJECT_LOCK_ALL);
	nY+=nOffY;


	PGL_MSG_CLASS(m_btnPokerType,CXyCheckButton);
	m_btnPokerType->CreateGUI(pic,theApp.m_GameSet.nPokerType);
	m_btnPokerType->MoveTo(nX,nY);
	m_btnPokerType->SetFloor(bit->GetZ()+0.01);
	m_btnPokerType->Updata();
	pl=CreatePlane(170,pic.height);
	pl->SetTextColor(0);
	pl->SetTextSize(12);
	pl->MoveTo(bit->GetX(),nY);
	pl->SetAutoMsg(MSG_INPUT_MOUSE_LEFT_DOWN,GetHandle(),SET_CALLBACK_CHECK_POKERTYPE);
	pl->TextOut(15,4,_T("选择双数字扑克牌面"));
	bit->AddChild(m_btnPokerType,OBJECT_LOCK_ALL);
	bit->AddChild(pl,OBJECT_LOCK_ALL);
	nY+=nOffY;

	CXyStandButton* btnOK;
	PGL_MSG_CLASS(btnOK,CXyStandButton);
	btnOK->CreateGUI(Res_GetPICStruct("btn_gameset_ok"),&(SNG_MSG)CreateMsg(GetHandle(),MSG_PUBLIC_OK));
	btnOK->SetButtonSound(BUTTON_STATUS_TOUCH,"SndTouchButton");
	btnOK->SetButtonSound(BUTTON_STATUS_LEFT_DOWN,"SndHitButton");
	btnOK->MoveTo(bit->GetLeft()+84,bit->GetTop()+177);
	btnOK->SetFloor(bit->GetZ()+0.1);
	btnOK->Updata();
	bit->AddChild(btnOK,OBJECT_LOCK_ALL);

	CXyStandButton* btnCancel;
	PGL_MSG_CLASS(btnCancel,CXyStandButton);
	btnCancel->CreateGUI(Res_GetPICStruct("btn_gameset_cancel"),&(SNG_MSG)CreateMsg(GetHandle(),MSG_PUBLIC_CANCEL));
	btnCancel->SetButtonSound(BUTTON_STATUS_TOUCH,"SndTouchButton");
	btnCancel->SetButtonSound(BUTTON_STATUS_LEFT_DOWN,"SndHitButton");
	btnCancel->MoveTo(bit->GetLeft()+176,bit->GetTop()+177);
	btnCancel->SetFloor(bit->GetZ()+0.1);
	btnCancel->Updata();
	bit->AddChild(btnCancel,OBJECT_LOCK_ALL);

	bit->SetFloor(FLOOR_GAMSET);
	bit->Updata();
}

void CGameSetBoard::CheckSet()
{
	theApp.m_GameSet.nSoundEffect=m_btnSound->CheckButton();
	theApp.m_GameSet.nMusic=m_btnMusic->CheckButton();

	theApp.m_GameSet.nPokerType= m_btnPokerType->CheckButton();
	PObject* pMainGame=GetObject(OBJECTNAME_MAIN_GAME);
	if (pMainGame)
		pMainGame->OnMsg(CreateMsg(0,GAME_MSG_FRESH_GAMESET));

}

BOOL CGameSetBoard::OnMsg(SNG_MSG& Msg)
{
	switch(Msg.msgID)
	{
	case MSG_PUBLIC_OK:
		{
			CheckSet();
			Hide();
			Updata();
		}
		break;
	case MSG_PUBLIC_CANCEL:
		{
			m_btnSound->SetCheckStatus(theApp.m_GameSet.nSoundEffect);
			m_btnMusic->SetCheckStatus(theApp.m_GameSet.nMusic);
			m_btnPokerType->SetCheckStatus(theApp.m_GameSet.nPokerType);
			Hide();
			Updata();
		}
		break;

	case SET_CALLBACK_CHECK_SOUND:
		{
			m_btnSound->SetCheckStatus(!m_btnSound->CheckButton());
		}
		break;
	case SET_CALLBACK_CHECK_MUSIC:
		{
			m_btnMusic->SetCheckStatus(!m_btnMusic->CheckButton());
		}
		break;
	case SET_CALLBACK_CHECK_POKERTYPE:
		{
	 		m_btnPokerType->SetCheckStatus(!m_btnPokerType->CheckButton());
		}
		break;
	default:
		return CShinyNovaGui::OnMsg(Msg);
	}


	return TRUE;
}
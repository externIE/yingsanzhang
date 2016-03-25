#include "StdAfx.h"
#include "AutoSetDepositDlg.h"
using namespace ShinyNova;
////////////////////////////////�Զ�����////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
CAutoSetDepositDlg::CAutoSetDepositDlg()
{
	m_nFixDeposit = 0;
	m_pInputDeposit = NULL;
	m_nCheckAuto = 0;
	m_pCheckAuto = NULL;
}

void CAutoSetDepositDlg::CreateSetDepositDlg()
{//�����Զ������Ի���
	ReName(SAFEBOX_AUTOSETDEPOSIT_DLG);
	CreateGUI(Res_GetPICStruct(RES_SYSTEMBOX_FRAME,0),Res_GetPICStruct(RES_SYSTEMBOX_FRAME,1),250,160);
	
	SetTitle(_T("�Զ�����"));
	CreateSpirit();
	CreatInputBox();
	CreateBtn();

	CString strKeyName;
	TCHAR szValue[32];
	strKeyName.Format(_T("AutoFix"));
	CGameWinApp* pGameApp = (CGameWinApp*)AfxGetApp();
	GetPrivateProfileString(_T("FIXDEPOSIT"), strKeyName, _T(""), szValue, 32, pGameApp->GetDepositSetFile());
	FillDeposit(atoi(szValue));
}

void  CAutoSetDepositDlg::RePaintTitle()	//�ػ��ϱ���
{//
	if(!m_plTitle) 
		return;
	
	m_plTitle->Erasure();
	m_plTitle->SetTextSize(12);
	m_plTitle->TextOut(5,6,m_szTitle);
}

void CAutoSetDepositDlg::CreateSpirit()
{//������ͼ
	PlaneObject* pSpirit=CreatePlane(10,10);
	pSpirit->SetTextColor(0);
	CRect rc=GetWorkRect();
	int nWidth = rc.Width()-15*2;
	int nHeight = rc.Height()-15*2;


	pSpirit->ScalePlane(nWidth,nHeight);
	pSpirit->ScaleTo(nWidth,nHeight);
	pSpirit->MoveTo(GetLeft()+rc.left+15+nWidth/2,GetTop()+rc.top+15+nHeight/2);
	
	SetSpirit(pSpirit);
	SpiritContext();
}


void CAutoSetDepositDlg::SpiritContext()
{//�������
	PlaneObject* pl=GetSpirit();
	
	pl->Erasure();
	pl->SetTextSize(12,400);
	pl->SetTextColor(0);
	pl->SetSpace(0, 4);
	pl->TextOut(0, 0, _T("��Ϸ����ʱ��ϵͳ��Ϊ���ӱ�������ȡ�����㵽�·���ֵ�����ܸ��ڷ������ޣ���"));
	pl->SetTextColor(RGB(140,160,100));
	int y=pl->GetHeight()*5/6+2;
 	pl->DrawLine(0,y,pl->GetWidth(),y);
}

void CAutoSetDepositDlg::CreatInputBox()
{//���������
	PlaneObject* pl=GetSpirit();
	pl->SetTextColor(0);
	
	PGL_MSG_CLASS(m_pInputDeposit,CGameInputBox);
	m_pInputDeposit->SetFloor(GetZ()+0.02);
	m_pInputDeposit->SetMaxInputLength(8);
	m_pInputDeposit->AdjustWidth(80);
	m_pInputDeposit->MoveTo(pl->GetLeft()+m_pInputDeposit->GetWidth()/2+40, pl->GetTop()+m_pInputDeposit->GetHeight()/2+80);
	m_pInputDeposit->Show();
	m_pInputDeposit->Updata();
	AddChild(m_pInputDeposit,OBJECT_LOCK_ALL);
	pl->TextOut(125,85, _T("��"));
}

void CAutoSetDepositDlg::CreateBtn()
{//������ť
	PlaneObject* pl=GetSpirit();
	SNG_PIC picBtn = Res_GetPICStruct(RES_SYSTEMBOX2_BTN);

	CRect rc=GetWorkRect();
	int nY = GetTop()+rc.top+rc.Height()*9/10;
	int nSpace=24;														// ��ť���
	int x1 = pl->GetLeft()+pl->GetWidth()/2-nSpace/2-picBtn.width/2;
	int x2 = pl->GetLeft()+pl->GetWidth()/2+nSpace/2+picBtn.width/2;  
	
	//ȷ��
	CXyStandButton* btnSure=NULL;
	PGL_MSG_CLASS(btnSure,CXyStandButton);
	btnSure->CreateGUI(picBtn,&(SNG_MSG)CreateMsg(GetHandle(),ZGSA_SAFEBOX_SETDEPOSIT_BTN_SURE));
	btnSure->SetButtonSound(BUTTON_STATUS_TOUCH,"SndTouchButton");
	btnSure->SetButtonSound(BUTTON_STATUS_LEFT_DOWN,"SndHitButton");
	btnSure->MoveTo(x1, nY);
	btnSure->SetFloor(GetZ()+0.02);
	btnSure->Updata();
	pl->AddChild(btnSure,OBJECT_LOCK_ALL);
	
	PlaneObject* plSpirit = btnSure->GetSpirit();
	plSpirit->SetTextSize(12);
	plSpirit->SetTextColor(RGB(35,46,0));
	plSpirit->TextOutEx(plSpirit->GetWidth()/2,plSpirit->GetHeight()/2,"ȷ��");	

	//ȡ��
	CXyStandButton* btnCancel=NULL;
	PGL_MSG_CLASS(btnCancel,CXyStandButton);
	btnCancel->CreateGUI(picBtn,&(SNG_MSG)CreateMsg(GetHandle(),ZGSA_SAFEBOX_SETDEPOSIT_BTN_CLOSE));
	btnCancel->SetButtonSound(BUTTON_STATUS_TOUCH,"SndTouchButton");
	btnCancel->SetButtonSound(BUTTON_STATUS_LEFT_DOWN,"SndHitButton");
	btnCancel->MoveTo(x2, nY);
	btnCancel->SetFloor(GetZ()+0.02);
	btnCancel->Updata();
	pl->AddChild(btnCancel,OBJECT_LOCK_ALL);
	
	plSpirit = btnCancel->GetSpirit();
	plSpirit->SetTextSize(12);
	plSpirit->SetTextColor(RGB(35,46,0));
	plSpirit->TextOutEx(plSpirit->GetWidth()/2,plSpirit->GetHeight()/2,"ȡ��");
	
	//�ر�
	CXyStandButton* btnCross=NULL;
	PGL_MSG_CLASS(btnCross,CXyStandButton);
	btnCross->CreateGUI(Res_GetPICStruct(RES_SYSTEMBOX_CLOSE),&(SNG_MSG)CreateMsg(GetHandle(),ZGSA_SAFEBOX_SETDEPOSIT_BTN_CLOSE));
	btnCross->SetButtonSound(BUTTON_STATUS_TOUCH,"SndTouchButton");
	btnCross->SetButtonSound(BUTTON_STATUS_LEFT_DOWN,"SndHitButton");
	btnCross->MoveTo(GetRight()-7-btnCross->GetWidth()/2,GetTop()+btnCross->GetHeight()/2+1);
	btnCross->SetFloor(GetZ()+0.02);
	btnCross->Updata();
	AddChild(btnCross,OBJECT_LOCK_ALL);

	//��ѡ��ť
	SNG_PIC pic = Res_GetPICStruct("��ѡ��");
	PGL_MSG_CLASS(m_pCheckAuto,CXyCheckButton);
	m_pCheckAuto->CreateGUI(pic, m_nCheckAuto);
	m_pCheckAuto->MoveTo(GetLeft()+btnCross->GetWidth()/2+55,GetTop()+btnCross->GetHeight()/2+85);
	m_pCheckAuto->SetFloor(GetZ()+0.02);
	m_pCheckAuto->Updata();
	AddChild(m_pCheckAuto,OBJECT_LOCK_ALL);

	CXyChatBox*	m_chatText;
	PGL_MSG_CLASS(m_chatText, CXyChatBox);
	m_chatText->CreateGUI(72, 20, 0);
	m_chatText->SetBkgndColor(COLOR_TRANSPARENT);
	m_chatText->SetMaxFontSize(14);
	m_chatText->SetTextSize(12);
	m_chatText->SetTextColor(0);
	m_chatText->SetRowSpacePixel(5);
	m_chatText->SetFloor(GetZ());
	m_chatText->InSertString(_T("���Զ�����"));
	m_chatText->MoveTo(m_pCheckAuto->GetX()+m_chatText->GetWidth()/2 + 7, m_pCheckAuto->GetY() + 4);
	AddChild(m_chatText, OBJECT_LOCK_ALL);
}

void CAutoSetDepositDlg::FillDeposit(int nFixDeposit)
{
	m_nFixDeposit = nFixDeposit;
	CString strDeposit=xyConvertIntToStr(m_nFixDeposit);
	m_pInputDeposit->ClearChat();
	m_pInputDeposit->InSertInput(strDeposit);
}

BOOL CAutoSetDepositDlg::OnMsg(SNG_MSG& Msg)
{
	switch(Msg.msgID)
	{
	case ZGSA_SAFEBOX_SETDEPOSIT_BTN_SURE:
		OnOK();
		break;
	case ZGSA_SAFEBOX_SETDEPOSIT_BTN_CLOSE:
		OnCancel();
		break;
	case ZGSA_SAFEBOX_SETDEPOSIT_CHECK_SET:
		OnCheck();
		break;
 	}

	return CXyStandDlg::OnMsg(Msg);
}

void CAutoSetDepositDlg::OnCheck()
{
	m_pCheckAuto->SetCheckStatus(!m_pCheckAuto->CheckButton());
}

int CAutoSetDepositDlg::IsAutoSetDeposit()
{
	return m_pCheckAuto->CheckButton();
}

void CAutoSetDepositDlg::OnOK()
{
	int nDeposit;
	LPCTSTR szText;
	CString strKeyName;
	CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();

	if (m_pInputDeposit)
	{
		szText=m_pInputDeposit->GetTextContext();
		nDeposit=atoi(szText);
		m_nFixDeposit=nDeposit;

		strKeyName.Format(_T("AutoFix"));
		WritePrivateProfileString( _T("FIXDEPOSIT"),strKeyName,xyConvertIntToStr(nDeposit),pGameApp->GetDepositSetFile()); 
	}

	CMyGame* pMainGame=(CMyGame*)GetObject(OBJECTNAME_MAIN_GAME);
	pMainGame->CheckKeyResult();
	OnCancel();
}

void CAutoSetDepositDlg::OnCancel()
{
 	Hide();
 	Updata();
}
#pragma once
//”Œœ∑…Ë÷√√Ê∞Â
using namespace ShinyNova;

enum{
	SET_CALLBACK_CHECK_SOUND = MSG_CUSTOMER_BEGIN+1,
	SET_CALLBACK_CHECK_MUSIC,
	SET_CALLBACK_CHECK_POKERTYPE
};

class CGameSetBoard:public CShinyNovaGui
{
public:
	CGameSetBoard();
	virtual ~CGameSetBoard();
	virtual void OnIniData();
	virtual BOOL OnMsg(SNG_MSG& Msg);
	virtual void CheckSet();

public:
	CXyCheckButton* m_btnSound;
	CXyCheckButton* m_btnMusic;
	CXyCheckButton* m_btnPokerType;
};
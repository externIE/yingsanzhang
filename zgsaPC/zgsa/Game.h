// Game.h : main header file for the GAME application
//

#pragma once


#include "resource.h"       // main symbols
#include "GameInfo.h"
#include "GameRequest.h" 
#include "GameNotifyEx.h" 
using namespace ShinyNova;


class CGameApp : public CGameWinApp
{
public:
	CGameApp();
	~CGameApp();

	virtual  BOOL				CreateGameFrame();         //����������Cwnd
	virtual  CXyGameFrame*		CreateLogicFrame();

	
    virtual  void               UpdateTableInfo();
    virtual  void		    	PlayGameSound(UINT nSoundID,UINT nFlag);
public:
	virtual  VOID*				GetTableInfo();
	virtual  VOID*				GetEnterInfo();
	virtual  CGameInfo*			GetGameInfo();
	virtual  DWORD				GetUserConfig();
	virtual  CGameConnect*		GetGameConnect();
	virtual  CGameNotify*		GetGameNotify();
	virtual  BOOL				IsSilence();

	virtual  BOOL				ParseCommond(LPCTSTR str);
	virtual  void				CreateArrageTable();//�齨�µ�����
	virtual  void				DisBandTable();

	virtual  int				GetTableChairCount();
  
	virtual	 int				GetChairCardsCount();
public:
	void ReadGameSet();
	void SaveGameSet();
	void FreshGameSet();
	void EraseBkgnd();
	void FreshScores();

	void SetVisitorPlayerBySoloPlayer(GAMEPLAYER* pgp, SOLO_PLAYER* psp);
	GAMEPLAYER* GetEmptyVisitorPlayer();
	GAMEPLAYER* GetVisitorPlayerByUserID(int nUserID);
	BOOL RemoveVisitorPlayerByUserID(int nUserID);
	void RemoveAllVisitorPlayer();

	virtual BOOL IsRejectKanPaiAsk();
	virtual BOOL IsAllowCardMaster();
	virtual BOOL IsRejectPassSound();
	virtual int  IsSaveReplay();
	virtual	BOOL IsTakeDepositInGame();//֧�ֿͻ��˻���
	virtual void ResetDataByNewPos(USER_POSITION* pUserPos);

	virtual void NotifyUserBoutEnd(LPUSER_BOUTEND pUser_BoutEnd);
	virtual	void NotifyUserDepositEvent(LPUSER_DEPOSITEVENT pUser_Deposit);
 
	//log����Dbgview���Խ���
	inline void OutputDebugStringEx(CString format,...)
	{
		CString s;
		va_list args;
		va_start(args,format);
		s.FormatV(format,args);
		va_end(args); 
		::OutputDebugString(s);
	}
 
public:
	GAME_SET           m_GameSet;    //��Ϸ��������
    CGameRequest       m_GameRequest;//����������������Ӳ���������
	CGameNotifyEx      m_GameNotifyEx;//����ӷ���������֪ͨ

	CGameInfoEx		   m_GameInfoEx;//��׿��Ϸ��Ϣ,
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameApp)
	public:
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGameApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CGameTable m_MyGameTable;
extern CGameApp theApp;
extern HHOOK   hMsgBoxHook;   
LRESULT   CALLBACK   CBTProc(int   nCode,   
							 WPARAM   wParam,   
							 LPARAM   lParam)   ;

extern int   MsgBoxEx(HWND   hwnd,   TCHAR   *szText,   TCHAR   *szCaption,   UINT   uType)     ;

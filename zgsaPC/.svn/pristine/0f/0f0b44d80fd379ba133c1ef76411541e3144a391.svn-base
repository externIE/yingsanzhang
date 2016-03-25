#pragma once
using namespace ShinyNova;

enum{
	MSG_HIDE_OPE_STATUS=MSG_CUSTOMER_BEGIN+1, 
 	MSG_THROW_BET_END
};
#include "COperationDlg.h"
class CMyGame;
class CMyPlayer:public CShinyNovaGui
{
public:
	CMyPlayer();
	virtual ~CMyPlayer();
	virtual BOOL  OnMsg(SNG_MSG& Msg);
	
	virtual void  CreateGUI(int nDrawIndex);  //创建玩家，从CGameWinApp数据
	virtual void  LoadPlayer(int nChairNO);
	virtual void  UnLoadPlayer();  

	virtual void  FitScrren();//刷新位置 
	virtual void  Restart();
	EVENTHANDLE      SNGAPI  APPLY_PlayBitObjectEx(BITObject* ob,
		int  mode,
		int  param,
		int  nStartFrame,  //用作用去的时间
		DWORD nTimeSpace,
		SNG_MSG* CallBackMsg);
 	virtual void  SetCurrentChair(BOOL bn,int nTimeRest=-1); 
	virtual POINT GetClockPosition();   

    int			GetTableSize();
	POINT		GetFaceCenter();  //相框中心为基准点 
 	POINT		GetFrameCenter();
	POINT		GetPlayerNameCenter();
    POINT		GetRestBetPostion();
 
	POINT		GetReadyPosition(); 
	POINT		GetThrowBetEndPositon();
	POINT		GetBankerPositon();
	POINT		GetCardPositon(BOOL bnFace,int nUnitIndex,int nCardStatus=0); 
    POINT		GetOpeStatusPostion();
	POINT		GetRetStatusPostion();
	POINT		GetNameBoardPosition();
	POINT		GetWinOrLoseNumPosition();

 	void		DrawPlayerFace(); 
    void		DrawPlayerName(); 
	void		DrawNameBoard();
	void		DrawEmptySeat();

	void		ShowBanker(BOOL bShow); //显示庄家标记 
    void		FreshPlayerBetInfo();		//刷新剩余和已用银子信息

    void		MOVIE_FreshRestBet(int nAddBet, int nWinChair, int nWinType);

	void		MOVIE_ThrowCard(); //放弃时扔牌

	void		SetOpeStatus(int nStatus);

    //void		ShowMask(BOOL bShow);

	void		SetTakeDeposit(BOOL bTake){m_bTakeDeposit=bTake;} //是否划银
	BOOL		IsTakeDeposit(){return m_bTakeDeposit;}
	
	//准备相关
	void  		ShowReady();
	void  		HideReady(); 

	//明、暗、被劈资源相关
	void		ShowBrightOrDarkBoard(BOOL bBright = TRUE, BOOL bLost = FALSE);
	void		HideBrightOrDarkBoard();

	//比牌失败相关
	void		ShowLostLightningMask();
	void		HideLostLightningMask();

	//被比牌按钮相关
	void		ShowCompareBtn();
	void		HideCompareBtn();
	BOOL		IsShowCompareBtn();

	//赢牌动画相关
	void		ShowWinAni(int nWinType);
	void		HideWinAni();

	//火资源相关
	void		ShowPlayerFire();
	void		HidePlayerFire();

	void		HideTimeAni();
	void		HideNewDigitAni();
	
public:
    CMyDigit*		m_digitRestBet;		//剩余银两
	CMyDigit*		m_digitUsedBet;		//玩家已用银两
	BITObject*		m_bitBrightOrDark;	//明、暗、被劈资源
	BITObject*		m_bitLost;			//弃牌资源
	BOOL			m_bLost;			//比牌失败
	BITObject*		m_bPlayerBoxBk;		//人物头像框背景
	BITObject*		m_bitTimeAni;		//时间动画
	BITObject*		m_bitTimeAniBk;		//时间动画背景
	
protected:
	int				m_nChairNO;			//座位号
	int				m_nDrawIndex;		
	int				m_nWinType;			//赢牌类型
	
	BITObject*		m_bitBoard;			//载体
	BITObject*		m_bitBanker;		//庄家
	BITObject*		m_bitOpeStatus;		//游戏未开始时的状态
	BITObject*		m_bitRetStatus;		//游戏中的状态
	BITObject*		m_bitRetStatusBk;	//状态背景框
    BITObject*		m_bitReady;			//准备
	BITObject*		m_bitNameBoard;		//名字面板
	BITObject*		m_bitEmptySeat;		//空位
	BITObject*		m_bitPlayerFire;	//火资源
	BITObject*		m_bitWinAni;		//输赢动画
   
	PlaneObject*	m_plPlayerFace;		//头像
	PlaneObject*	m_plPlayerName;		//名字	
	PlaneObject*	m_plMask;			//输赢蒙版

	BOOL			m_bCurrentChair;
	BOOL			m_bMask;			
	BOOL			m_bTakeDeposit;		//划银
	
	CMyDigit_Move*	m_pNewDigit;		//
	CXyStandButton* m_btnCompareCard;	//被比牌按钮

	CMyGame*		m_pMainGame;
};

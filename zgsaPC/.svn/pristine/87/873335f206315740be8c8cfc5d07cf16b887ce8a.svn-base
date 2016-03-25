#pragma once
using namespace ShinyNova;
#include "Game.h"
#include "Replay.h"
#include "COperationDlg.h"
#include "AutoSetDepositDlg.h"
#include <vector>
//extern CGameTable g_GameTable;

class CMyKeyResult;

class CMyGame:public CMainGame
{
public:
		 	 CMyGame();
	virtual ~CMyGame(); 
	virtual GAME_START_DATA*    GetStartData();
    virtual GAME_PUBLIC_INFO*	GetPublicInfoEx();
	virtual	GAME_PLAYER_INFO*	GetPlayerInfoEx(int chairno);
	virtual CGameInfoEx*		GetGameInfoEx();


	virtual void				OnGameSize();
	virtual void     			OnIniData();//初始化
	virtual void				GameInstance();//初始化
	virtual void                OnEnterMatchOK();
	virtual void                OnEnterGameOK();
	virtual void                OnEnterDXXW();
	virtual void                OnEnterIdle();
	virtual void                OnPlayerEnter(GAMEPLAYER* pPlayer);//其他玩家进入
	virtual void                OnPlayerAbort(GAMEPLAYER* pPlayer);//其他玩家离开
	virtual void                OnVisitorEnter(GAMEPLAYER* pPlayer);//旁观者进入
	virtual void                OnVisitorAbort(GAMEPLAYER* pPlayer);//旁观者退出
	virtual void			    OnStartSoloTable();


	//构建游戏
	virtual void				CreatePlayers(int nTotalChairs); 
	virtual void				CreateCard(); 
	virtual void				CreateBtn();
	virtual void				CreateClock();
	virtual void				CreateRetInfo();
	virtual CXyStandButton*		FastCreateStandButton(LPCTSTR picName,SNG_MSG& msg,LPCTSTR szNote=NULL);
	virtual void                SetObjectStayNote(PObject* ob,LPCTSTR note);
//消息处理
	virtual BOOL			 	OnMsg(SNG_MSG& msg);
	virtual BOOL                OM_DealBTN(SNG_MSG& msg);
	virtual BOOL                OM_DealOPE(SNG_MSG& msg);


//操作按钮
	virtual void				OnStart(); 
    virtual void				OnFreshGameSet();
	virtual void				OnGetNewPosition();
	        BOOL                OPE_GiveUp();
			BOOL                OPE_Bet(int nAddBet);
        	BOOL                OPE_Rise(int nAddBet);
			BOOL                OPE_Follow();
			BOOL                OPE_LookCard(); 
			BOOL                OPE_Allin();
			BOOL				OPE_BetFull();
			BOOL				OPE_CompareCard();
			BOOL				OPE_CompareCardByIndex(int nDrawIndex);
			void                OnShowCard();
			void				OPE_ChangeLookOnToSeated();
			void				OPE_ChangeSeatedToLookOn();
			void				OPE_ChangeTable();

	virtual void				OnTimer(); 
    virtual BOOL                OPE_GameClockZero();

public:
//通讯 

	virtual void                NTF_StartGame(int chairno);
	virtual void                NTF_GameStart();
	virtual void                NTF_PlayerOffline(int nUserID);  
	virtual void			    NTF_GameMsg(GAME_MSG* pGameMsg);
	virtual void			    NTF_SystemMsg(int nMsgID,void* data,DWORD datalen);
	virtual void			    NTF_LocalMsg(int nMsgID,void* data,DWORD datalen);
	        void 	            NTF_ZgsaShowCard(SHOW_CARD_RESPONSE *pShowCard);   
 	virtual void				NTF_GameWin(); 
			void				GameWin();
	virtual void		    	NTF_StartCountdown(START_COUNTDOWN* pCountdown);
	virtual	void		    	NTF_SyncStartCountdown(SYNC_COUNTDOWN* pSyncCountdown);
	virtual void				NTF_UserDepositEvent(LPUSER_DEPOSITEVENT pUser_Deposit);
 	virtual void                NTF_UserBoutEnd(LPUSER_BOUTEND pUser_BoutEnd);
			void				NTF_LookCard(SHOW_CARD_RESPONSE* look_card);
			void				NTF_ChangeSeatedToLookOn(LOOK_ON_AND_SEATED_CHANGE_RESULT* ploascr);
			void				NTF_ChangeLookOnToSeated(LOOK_ON_AND_SEATED_CHANGE_RESULT* ploascr);
			void				NTF_GetGiveUpBtnConfig(int* bOpen);

 	virtual UINT			    REQ_SendMsgToServer(UINT msg,int datalen=0,void* data=NULL,BOOL bnNeedEcho=FALSE);
public:
//流程
 
	virtual	void                OPE_InitData();  //初始化
	virtual	void                OPE_ClearGame(); //游戏清理
	virtual	void		    	OPE_DealCard();  //发牌
	virtual	void                MOVIE_DealCard();
	virtual void                MOVIE_DealCardEnd(SNG_MSG& msg);
	virtual	void                OPE_GameStart(); //发牌结束，游戏开始
	virtual	void		    	OPE_DXXW();
	virtual	void                OPE_CloseResult();  
	virtual void                OPE_ShowGameInfo();//左上角比赛信息框
	virtual void                OPE_StartTimer();
	virtual void				OPE_StartIdleTimer();
	virtual void                OPE_EndTimer(); 

	    	void				OPE_ShowDigitAni(int nDigit);

         	CString				GetCardDescStr(int nCardID);
	virtual void				OPE_ShowResult();
	        void				OPE_ShowResultAni();
			void				OPE_ShowResultAni2();
			void				OPE_AfterShowResultAni(); 
            
        	void				ShowWaitArrageTable();
        	void				HideWaitArrageTable();

            POINT				GetWaitCountDownPostion();
	        POINT				GetWaitCountDownDigitPostion();
			void				ShowWaitCountDown();
        	void				HideWaitCountDown();
			void				EnableNextChairOpe(int nChairNo,int nNextChair,DWORD dwOpe);//下一家操作
 	      
	        void				MOVIE_ShowWinCards(PLAYER_WIN_INFO*pPlayerWinInfo,int chairno);
public:
           
	        void				OPE_HideChairCard(int chairno);
			void				OPE_ShowChairCard(int chairno,int nUnitIndex,int nCardID,int nCardStatus);
			void				OPE_MaskAllShowCard();
			void				OPE_UnMaskAllShowCard();
			void				OPE_ThrowChips(int chairno,int bet);
			void				OPE_ThrowOneChips(int chairno,int times,BOOL bnFast=FALSE);
			void				OPE_TakeBet();

			EVENTHANDLE			APPLY_Turn(PObject* ob,int speed,   BITObject * bit1,SNG_MSG* CallBackMsg =NULL);
            POINT				GetPokerDealPosition(); 
			POINT				GetPotsNumPosition(); 
			POINT				GetBaseDepositBoardPosition();
			POINT				GetComparePosition();
			POINT				GetVSPosition();
			POINT				GetLightningPosition(BOOL bLeft);

		    void				FreshBetInfo(int nChairNo = -1);
			void				FreshTotalDepositInfo();

			void				DrawPlaneChip(PlaneObject*pl,int maxCluster,int nTotalBet); 
			void				DrawPlaneChip2(PlaneObject*pl,int maxCluster,int nTotalBet);  //数字为图片

			BOOL				GetGiveUpBtnConfig();
public:
//控件
	 virtual    void			GUI_MoveClock(int chairno,int second);
	            void			GUI_MoveClockIdle(int chairno,int second);
	 //规则
public:
	       
	virtual void				RUL_CalcGameStatus(int w,int h); 
	        RECT				RUL_GetZgSaTableRect();

	virtual	int			 		RUL_GetChairNOByDrawIndex(int nDrawIndex);
	virtual	int					RUL_GetDrawIndexByChairNO(int nChairNO);
	virtual	int					RUL_GetNextIndex(int nDrawIndex);
	virtual	int					RUL_GetPrevIndex(int nDrawIndex);
	virtual	int					RUL_GetNextChairNO(int nChairNO);
	virtual	int					RUL_GetPrevChairNO(int nChairNO);

			void				HidePlayersCompareBtn();
			void				PlayCompareCardAni(COMPARE_CARD_RESPONSE* pCompare);
			void				StopCompareCardAni();
			void				FitBet(int total, int& nBet4, int& nBet2, int& nBet1);
			POINT				GetWorldPoint(POINT pt);

			void				SetAutoFollow(BOOL bAutoFollow);
			BOOL				IsAutoFollow();//获得m_bAutoFollow值
			void				SetAutoStart(BOOL bAutoStart);
			BOOL				IsAutoStart();//获得m_bAutoStart值
			void				ShowLookOn_Seat_ChangeTableBtn(BOOL bShowLookOn, BOOL bShowSeatBtn, BOOL bShowChangeTableBtn);
			void				ResetAllCards();
			void				UpdateAllCards();
			void				UpdateAllInPlayersStatus(ALL_IN_RESPONSE* allInReponse);
			BOOL				IsPlayerGiveUp(int nChairNO);
			BOOL				CheckRoomPlayerStatus(int nChairNO);
   
	static  UINT				ThreadShowWinResult(LPVOID lpParameter);//游戏结束赢家线程函数

	//auto set deposit beginSS
			void				createAutoSetDepositDlg();							//创建自动设银框
			void				CheckKeyResult();
	virtual	void				AutoSetDeposit();									//自动设银
	virtual	int					GetAutoSetDeposit();								//获取本地保存的设置银子数值
	virtual	void				ShowKeyResult(BOOL bShow,int nRandKey, int nFlag);	//显示密码提示框
	virtual	void				TakeDepositToGame(int nDestDeposit,int nKeyResult);	//保险箱取钱
			int					GetPlayerDeposit();
	//auto set deposit end

			void				SetIsOpenBetFull();
			BOOL				GetIsOpenBetFull();
			int					GetBetDeposit();
			int					GetBetFullDeposit();
			int					GetBetFullCount();
public: 
//设置框
	 CGameSetBoard*     m_GameSetBoard;  
	 //{
	 HCURSOR			m_hCursorHand;
	 HCURSOR			m_hCursorArrow;
	 //}
public:
	//变量 
	PlaneObject*		m_plGameInfo;					//左上角游戏框
	CXyDigit*			m_digitBase;					//基础分/基础银 
 
	///////////////////////////////////////////////////////////////////////////////
 	CMyCard*			m_Card[TOTAL_CHAIRS*CHAIR_CARDS];
	int			        m_nTotalPlayer;					//玩家总数
	CMyPlayer*	     	m_ptrPlayer[MAX_CHAIR_COUNT];	//按DrawIndex序号 

	//单注、本局可压银两、总注信息
    BITObject*          m_betInfoBoard;				//面板
	BITObject*          m_bitIconMaxDeposit;		//最大可压银两图标
	CXyDigit*           m_digitPotsNum;				//总注
	CXyChatBox*			m_chatSingleBet;			//单注
	CXyChatBox*			m_chatBetMaxDepositNum;		//本局可压银两

	//开始按钮相关
	CXyDynamicButton*   m_btnStart;

	//按钮操作面板相关
	COperationDlg*     m_dlgOperation;
	
	//计时器相关
	EVENTHANDLE			m_hEventTimer;                 //主计时器
  	CXyDigit*           m_ClockCountDown;
    CClockDigit*        m_Clock;
	int                 m_nGameZeroCount; 
	int					m_nKeepGiveUpTimes;//持续超时弃牌次数;跟注，加注，全下，弃牌，压满 返原初始值
	BITObject*          m_bitCountDown; //倒计时
    int                 m_nZeroCountDown;
	BITObject*			m_bitTimeAni;		//时间动画
	BITObject*			m_bitTimeAniBk;		//时间动画背景
	
	//发牌相关
	int				    m_nDealCount;                 //发牌轮数，一人一张，为一轮
	BOOL				m_bCardDealing;
	
	//分桌相关
	void*				m_ShowWaitArrage;
	
	//游戏状态相关
	DWORD               m_dwGameStatus;
    EVENTHANDLE         m_handleNewRound;

	//比牌资源
	BITObject*			m_bitVS;
	BITObject*			m_bitLightning;
	BOOL				m_bLightningLeft;

	//筹码资源
	BITObject*			m_bitThrownBet[MAX_BET_ALL_COUNT];            //投资到桌子中的筹码
	POINT				m_TrownPoint[MAX_BET_ALL_COUNT];			  //砝码目标点
	int					m_nBetCount;
private:
	BOOL				m_bSitOrStand;//1 sit,0 standby
	BOOL				m_bAutoStart;	//是否自动重新开始开关，if ture is Yes, false is No
	BOOL				m_bAutoFollow;	//是否自动跟注，if ture is Yes, false is No
	BOOL				m_bOpeDuringCompareCard;
	int					m_nAutoFollowTime;//留给用户操作时间，暂定2秒
	int					m_nLeftCompareNum;//剩余比牌次数
	BOOL				m_bGameWin;//是否接收到赢家信息
	int					m_nComNextPlayer;//记录比牌到第几个玩家
	COMPARE_CARD_RESPONSE* m_pCompareCardResponse;
	ALL_IN_RESPONSE*	m_pAllInResponse;
	std::vector<ShinyNova::GROUP*>		m_vecCompareAni;

	//auto set deposit begin
	CAutoSetDepositDlg*	m_dlgAutoSetDeposit;	//自动设银对话框
	CMyKeyResult*		m_pMyKeyResult;			//保护密码验证框
	//auto set deposit end

	BOOL				m_bOpenBetFull;			//是否支持压满
	int					m_nBetFullDeposit;		//保存第一家压满的银子

	BOOL				m_bShowGiveUpBtn;		//未轮到玩家操作时是否显示弃牌按钮
};

//保护密码验证
class CMyKeyResult:public CKeyResult
{
public:
	CMyKeyResult(){};
	virtual ~CMyKeyResult(){}
	virtual void OnOK();
};
// GameInfo.h: interface for the CGameInfoEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEINFO_H__17F16CEC_D35E_47E0_A4D6_4A88F209871B__INCLUDED_)
#define AFX_GAMEINFO_H__17F16CEC_D35E_47E0_A4D6_4A88F209871B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGameInfoEx:public CGameInfo
{
public:
	CGameInfoEx();
	virtual ~CGameInfoEx();
public:
public:  
	GAME_ENTER_INFO			       m_GameEnterInfo;
	GAME_TABLE_INFO				   m_GameTableInfo;//改结构保存DXXW时从服务器返回的游戏信息
    GAME_WIN_RESULT                m_GameWinResult;
	BOOL						   m_bnAlowLook;//是否允许看牌

	virtual  GAME_WIN_RESULT*      GetGameWin();
	virtual  GAME_START_DATA*      GetGameStartData();
	virtual  GAME_PUBLIC_INFO*     GetPubInfo();
	virtual  GAME_TABLE_INFO*      GetGameTableInfo();   

	virtual BOOL  IsGamePlaying();
	virtual BOOL  IsForbidDesert();
	virtual BOOL  IsChairReady(int chairno);
	virtual DWORD GetStatus();
	virtual void  SetStatus(DWORD dwStatus);
	virtual int   GetBoutCount();
	virtual int   GetBaseDeposit();
	virtual int   GetBaseScore();
	virtual	void  SetCurrentChairNO(int chairno);
	virtual	int   GetCurrentChairNO();

	virtual int	  GetBanker(); 

	virtual void  NTF_StartGame(START_GAME* pStartGame);
 	virtual void  NTF_PlayerAbort(int chairno);
	virtual void  NTF_GameStart(GAME_START_INFO *pStartData);
	virtual void  NTF_GameWin(GAME_WIN_RESULT* pGameWinMJ);
	virtual void  ClearTableInfo();//游戏结束后清理数据
	virtual void  ResetStatusWhenStart();//游戏开始时清理数据

//zgsa fuction

	
	        int   GetBetWait();
        	int   GetOpeRestTime();
			void  SetOpeRestTime(int nRestTime);	

        	void  SetChairUsedDeposit(int chairno,int nDeposit);
	        int   GetChairUsedDeposit(int chairno);
	
          	int   GetChairTotalDeposit(int chairno);
			void  SetChairTotalDeposit(int chairno,int nDeposit);

 	        int   GetChairRestDeposit(int chairno);

			int	  GetChairPlayerDeposit(int chairno);
			void  SetChairPlayerDepoist(int chairno,int nDeposit);
			
            void  SetChairRoundDeposit(int chairno,int nDeposit);
			int   GetChairRoundDeposit(int chairno);

			int   GetChairStatus(int chairno);
			void  SetChairStatus(int chairno,int status);

		    DWORD  GetCouldOpe();
			void  SetCouldOpe(DWORD dwOpe);
			
			int   GetRoundMinDeposit();
			int   GetRoundCurDeposit();
			void  SetRoundCurDeposit(int nDeposit);
			
            int   GetTotalDesposit();
			void  SetTotalDesposit(int nDesposit);

			BOOL   IsNeedAssistOpe();
			BOOL  IsGameEndAhead();

			BOOL   IsChairFold(int chairno);
			BOOL   IsIdlePlayer(int chairno);
            BOOL    IsUserIDEqual(int chairno);

			double  GetWinRatio(int chairno);
		    int     GetFinalDeposit(int chairno);
};

#endif // !defined(AFX_GAMEINFO_H__17F16CEC_D35E_47E0_A4D6_4A88F209871B__INCLUDED_)

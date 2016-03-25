#pragma once


#include         <map>
#include         <list>
#include		 <algorithm> 
#include		 <functional>

#define TCMATCH_WASHOUT_COEFFICIENT      1.5  //��̭ϵ�����������ֵ�ʱ��ʣ���������ھ���������1.5��ʱ����ת�׶�
#define TCMATCH_MAX_WASHTURN             7     //����7�ֶ�����̭

#define TCMATCH_WAIT_NEXT_BOUT_SECOND    5     //5������·���
#define TCMATCH_MAX_GAME_PLAY_SECOND     900   //һ����Ϸ���15����
#define TCMATCH_FRESHRANKLIST_SECOND     60    //�60��ˢ��һ�λ����б�

//���״̬
#define TCMATCH_STATUS_WIN				 -4       //�ѽ���
#define TCMATCH_STATUS_OFFLINE           -3       //���ߣ�ע����״̬��Ȼ�ڵȴ�������,����һ��ʱ��ᱻת��ΪGIVEUP  
#define TCMATCH_STATUS_GIVEUP			 -2       //����
#define TCMATCH_STATUS_WASHOUT			 -1       //��̭
#define TCMATCH_STATUS_NOT_READY		 0       //��δ����
#define TCMATCH_STATUS_WAIT				 1       //�ȴ�ȷ��
#define TCMATCH_STATUS_WAIT_NEXT_PHASE   2       //�ȴ���һ�׶�
#define TCMATCH_STATUS_READY_NEXT_BOUT   3       //׼���ý�����һ��
#define TCMATCH_STATUS_PLAYING			 4       //������
#define TCMATCH_STATUS_WAIT_RANK         6       //�����ѽ������ȴ�������Ҽ�������
//����״̬
#define TCMATCH_TABLE_EMPTY              0      //δʹ��
#define TCMATCH_TABLE_PLAYING            1      //�ڴ�
#define TCMATCH_TABLE_WAIT_NEXT_BOUT     2      //�ȴ���һ����



#define  TCMATCH_ANIMATCION_WAIT_FINAL   "animation_WaitFinal"
#define  TCMATCH_ANIMATCION_ARRAGETABLE  "animation_WaitArrangeTable"
#define  TCMATCH_ANIMATCION_WATI_RANK    "animation_WaitCaclRank"
#define  TCMATCH_ANIMATCION_WAITTALBE    "animation_WaitTable"   //�ֿ�״̬

#define  TCMATCH_MIN_SCORE              -100000000

#define	 ONE_GROUP_TABLE_NUM			5		//���5�� һ��̽������
enum SYSTEMFINDPLAYERS_MODE{
	eSFP_MODE_MONEY,
	ESFP_MODE_BOUNT,
	ESFP_MODE_WINS,
	ESFP_MODE_MAX,//ϵͳ�����㷨ģʽ����
};
typedef struct _tagTABLEAVGVALUE{
	int nAvgBounts;
	int nAvgWins;
	int nAvgScore;
	int nAvgDeposit;
}TABLEAVGVALUE, *LPTABLEAVGVALUE;
// typedef struct _tagMATCHDATA{      //�˽ṹ��ű��������������
// 	int nMatchID;
//  	int nMatchType;
// 	int nSubType;
// 	int nStatus;
//    	DWORD dwOptions;
// 	DWORD dwConfigs;
//  
// 	int nGameID;
// 	int nAreaID;
// 	int nRoomID;
// 
// 	int nBigMatchID;
// 	int nPreviousID;
// 	int nLaunchDate;
// 	int nLaunchTime;
// 	int nMaxRunTime;
//  	int nMatchLevel;
// 
// 	int nInitScore;
// 	int nInitBaseScore; 
// 	int nInitOutScore;
// 	int nInitDeposit;
// 	int nInitBaseDeposit;
// 	int nInitOutDeposit;
// 	int nFixRings;
// 	int nFixBouts;
// 
//   	int nSignUpCount; //�ѱ�������
// 	int nMinRunners; //������������
// 	int nMaxRunners; //������������
// 	int nEndRunners;//������������
// 
// 	int nBaseScore; // ������,�ɱ�
// 	int nWashScore; // ��̭��,�ɱ�
// 
// 	DUAN_NEEDSCORE dns;
//     int nReserved[16];
// }MATCHDATA, *LPMATCHDATA;

typedef struct _tagMATCHDATA{      //�˽ṹ��ű��������������
	MATCH					match;

	int						nBaseScore;					// ������,�ɱ�
	int						nWashScore;					// ��̭��,�ɱ�

	DUAN_NEEDSCORE			dns;

	int                     nSignUpCount;               // ��������

    int						nReserved[16];
}MATCHDATA, *LPMATCHDATA;

typedef class CTcMatchPlayer
{
public:
	CTcMatchPlayer(LPMATCH_PLAYER lmp,LPUSERONMGAME lug,LPUSERONMATCH lum,LPMATCH_HINT lmh);
	~CTcMatchPlayer();
	
	unsigned long GetWaitSecond();    
			 BOOL IsPlaying();
			 BOOL IsQuit();
			 BOOL IsWaitNextPhase();   //�ȴ���һ�׶�
			 BOOL IsOffline();         //����״̬
			 BOOL IsReadyMatch();
			 void SetRank(int nRank);
			 int  GetRank();
			 void SetScore(int nScore);
			 int  GetScore();
			 void SetDeposit(int nDeposit);
			 int  GetDeposit();
			 int  GetUserID();
public:
	int			  m_nStatus;
	int           m_nRoomID;
	int           m_nTableNO;
	int           m_nChairNO;
	int           m_nMode;              //ģ��
	int           m_nParseBout;			//�ڱ��׶εľ���
	int			  m_nTotalBout;         //�ܾ���,��������
	BOOL          m_bReady;             //�Ƿ�׼���ñ���
	BOOL          m_bActive;            //��Ծ�û�
	BOOL          m_bOffline;           //�Ƿ����
	BOOL          m_bQuit;              //�û�����QuitGame
	int			  m_nLastPlaySecond;    //���һ�ν�����Ϸ��ʱ�䣬���Լ���ȴ�ʱ��
	int           m_nLastSendRankSecond;//���һ�η��������б��ʱ��
	int			  m_nTotalWait;         //�ܵȴ�ʱ��
public:
//������
	MATCH_PLAYER  match_player;
	USERONMGAME   userongame;
	USERONMATCH   useronmatch;
	MATCH_HINT    match_hint;
}*LPCTcMatchPlayer;

class  CTcMatchBase;

typedef class CTcMatchTable
{
public: 
	CTcMatchTable(CTcMatchBase* pTcMatchBase,int nTableNO);
	CTcMatchTable(int nRoomID,int nTableNO,DWORD dwMatchOptions=MATCH_CFG_TURN_BANKER);
	~CTcMatchTable();
	
	BOOL		  PlayerEnter(CTcMatchPlayer* player);
	void          ResetTable(int nStatus=TCMATCH_STATUS_WAIT);
	BOOL          GetNextBout(LPARRANGE_TABLE pArrangeTable,int nBaseScore,int nLevelScore,int nMaxPlaySecond=60);
	int			  GetNextChair(int chairno);
	int			  GetPrevChair(int chairno);
	void		  RandomSortPlayer(LPCTcMatchPlayer array[],int length);
	DWORD         GetPlaySecond();//��Ϸ���е�����
	DWORD         GetWaitSecond();//��ȡ���ӵȴ���ʼ������
	BOOL          IsOverTime();//��Ϸ��ʱ
public://�̶�
 	int                 m_nTableNO;
	int					m_nPlayerCount;
public://�ɱ�
    int                 m_nStatus;
  	int                 m_nLastPlayStartTime;//
	int                 m_nLastPlayEndTime;

	int                 m_nBaseScore;
	int                 m_nWashScore;

 	int                 m_nBanker;              //ׯ��,userID
	int					m_nBout;				//���˼���
	int					m_nPlanBout;			//Ԥ���漸��
	int                 m_nMaxPlaySecond;
	LPCTcMatchPlayer    m_playerPtr[MAX_MATCHCHAIR_COUNT];
public:
	CTcMatchBase*       m_pTcMatchBase;

}*LPCTcTable;


#define  MATCH_STEP_WASHPLAYER    1
#define  MATCH_STEP_ARREAGETABLE  2
#define  MATCH_STEP_RANKLIST      3
#define  MATCH_STEP_FINIEDMATCH   4       //�����Ѿ������ȴ���Ϣ
#define  MATCH_STEP_MATCHEND      5       //��������
#define  MATCH_STEP_WAITFINAL     6       //�ȴ�����
#define  MATCH_STEP_USERSTATUS    7       //�����û�״̬
#define  MATCH_STEP_OVERTIME      8       //��Ϸ��ʱ
#define  MATCH_STEP_DISBANDTABLE  9       //��������
#define  MATCH_STEP_RF_USERSCORE  10	  //ˢ���û�����
#define  MATCH_STEP_TERMINATE_TABLE  11   //ǿ�Ʋ���
#define  MATCH_STEP_BCWARSHTURN      12   //�㲥��ǰ�ڼ���

typedef struct _tagMATCH_STEP
{
	int		nStepID;
	void*   pStepData;
	int     nDataLen;
	UINT    nReqID;
}MATCH_STEP,*LPMATCH_STEP;

typedef CMap<int, int, CTcMatchTable*, CTcMatchTable*> CTcMatchTableMap;
typedef CMap<int, int, CTcMatchPlayer*, CTcMatchPlayer*> CTcMatchPlayerMap;
typedef CList<MATCH_STEP*,MATCH_STEP*&> CTcStepList;

class CBaseRoomData;
class CTcMatchBase
{
public:
 	CTcMatchBase(CBaseRoomData* pRoomData,LPMATCH lpMatch);
	virtual ~CTcMatchBase();

	void					         ResetParameter();
	void                             SetOptions(DWORD dwOptions);
	LPCTcTable						 GetFreeTable();
	CTcMatchPlayer*                  GetPlayer(int nUserID);
	CTcMatchTable*                   GetTable(int nTableNO);
	
	void                             PushStep(int nStepID,void* pStepData);
	MATCH_STEP*                      PopStep();
	void                             ClearStep(MATCH_STEP* step);
	virtual  void					 NotifyMatchFinish(CTcMatchPlayer* pPlayer);
	virtual  void                    NotifyUserRankList(int nUserID);
	virtual  void                    NotifyUserStatus(int nUserID,LPCTSTR szStatus,BOOL bnAnimation,int nTablePlaying=0,int nPromotionCount=0);
	virtual  void                    NotifyWaitUserTableCount(int nTablePlayingCount,int nPromotionCount);//ˢ�µ�ǰ�ȴ���һ�׶ε��û�����ǰ���ж���������
	virtual  void                    NotifyUserWaitTalbe();
	virtual  void                    PushWaitRankUser(int nUserID);//Ԥ������һ����ҵ�������Ϣ
	virtual  void                    SendWaitRankUser();//�����б�������������Ϣ

	virtual  void                    SortPlayerRank(MATCH_RANK rank[],int nCount);
	virtual  void                    CalcPlayersRank();
	virtual  int                     CalcRestUserCount();		//����ʣ��ɲ����������
	virtual  void                    CreateTableRankList(int nTalbeNO);
	virtual  void                    CreateTableRankList(int nUserIDs[],int nUserCount);
	virtual  void                    CreateUserRankList(int userid);
	virtual  void                    ReFreshUserScore();//ˢ�������û�����
	virtual  void                    BreakOffMatch();//���ñ�������GameSvr�жϵ����
	virtual  void                    ContinueMatch();//������������GameSvr�������ӵ����
	virtual  int                     CalcWinPointByRank(int nUserID,int nRank);//�������μ��㽱��������
	virtual  int                     CalcBaseDuanScore();//���ݱ�����֯��������ض�λ����������

	virtual  ARRANGE_TABLE*          ArrageOneTable(CTcMatchTable* pTable,int nUserID[]);
	virtual  int                     GetWashOutRank();//ע���������ȵ��ô˺���������Σ����޸Ľ�Ҫ��̭��ҵ�״̬
	virtual  int                     GetTotalUserCount();//���㻹�ж�����Ҵ��
	virtual  int                     GetPlayingTableCount();//��ȡ������Ϸ�е�������
			 void                    CopyUserIDs(int* pUserIDs);
			 BOOL                    IsPlayerSeated(int nUserID);
protected:
	int						         CalcTableSort();
	void                             ClearMatch();
#ifdef _DEBUG
//��¼�����û����ֵ�ͬ���Ĵ���
 	std::map<CString,int>              m_mapSametable;
#endif
	virtual  void                    LogMatchInfo();//��¼��Ϸ��Ϣ
public:
   	int                              m_nChairPerTable;  //ÿ���������
 	MATCHDATA                        m_MatchData;
public: //�ɱ�
 
	int                              m_nUserMaxScore;     //��ǰ������������ɱ�
	int                              m_nPhaseTurn; //���������׶ε�ǰ������(CurrentMinUserTurn)��ÿ���׶��¿�ʼ��0��
//	int                              m_nRestUserCount;//ʣ��δ����̭�����
	int                              m_nMatchStatus;//����״̬

	CList<int,int>                   m_WaitWaitRankList;
	MATCH_RANK*                      m_PlayerRank;
 	int                              m_nRankListCount;//��¼ָ��ʵ�ʳ��ȣ����ó������������������ͬʱ��ָ�뽫ɾ����������ռ�
public:
 	CTcStepList			   		     m_listMatch;
	CTcMatchPlayerMap				 m_mapPlayer;
	CTcMatchTableMap                 m_mapTable;
	int                              m_nTableSortCount;//����ţ���¼һ���������ٴ�
public://ͳ��
	int                              m_nTotalPlaySeconds;//������Ϸ�ķѵ�����Ϸʱ��
	int                              m_nMaxGamePlaySeconds;//������Ϸ��ķ�ʱ��
 	int                              m_nTotalUserWaitSeconds; //�û����ڵȴ�����ʱ�䳤
	int                              m_nMaxUserWaitSeconds; //�û��ȴ������ʱ�䳤��
	int                              m_nMaxUserWaitArrageTable;//�û����εȴ����������ʱ�䳤��
	int                              m_nMaxUserPlayBout;//�û�����������
	int                              m_nMatchStartSecond; //������ʼʱ��

	int                              m_nFinalBout;//ʣ��Ķ�����̭����
	int                              m_nWaitNextBoutSecond;//�ȴ���һ�ֿ�ʼ������
	int                              m_nMaxPlaySecond;//һ�����Ϸʱ��
	int                              m_nWashOutIndex;//��̭˳��

	int                              m_nCurrentUserWaitArrage;//��ǰ�û��ȴ�������time
	BOOL                             m_bGameSvrConnnet;//GameSvr�Ƿ�����״̬
public:
	CBaseRoomData *                      m_pRoomData;
	void							 CopyMatchDetail(MATCH_DETAIL* lpMD,MATCH_DYNA* lpMD2);
    void                             RefreshDataFromStartToRoom(LPMATCH_STARTTOROOM lpMS);
	void                             RefreshDataFromMatchDynamic(LPMATCH_DYNA lpmatch_dynamic);
	int                              GetRoomID();
	int                              GetMatchID();
	int								 GetGameID();
	DWORD                            GetMatchConfig();
	int                              GetInitScore();
	int                              GetInitDeposit();

	int                              GetBaseScore();
	int                              GetWashScore();
	void                             SetBaseScore(int nBaseScore);
 	void                             SetWashScore(int nWashScore);
};



//�����׶�
class CTCMatchPhase
{
public:
	 CTCMatchPhase(int nBoutPerTurn);
	 ~CTCMatchPhase();
public:
	virtual    void      TransPhase(); //����ǰһ���׶�
	virtual    void      BeginPhase(); //��ʼ�׶�
	virtual    int       CalcBaseScore();
	virtual    int       CalcLevelScore();
	virtual    BOOL      ValidatePlayerEnter(int nUserID);//�ж�����Ƿ���Լ������
	virtual    void      InitialPlayerWhenEnter(CTcMatchPlayer* pPlayer);
	virtual    BOOL      IsPrepareEnterPhase();//�Ƿ�������������ת�׶�
	virtual    BOOL      IsReadyEnterPhase();//�Ƿ������������׶�
	virtual    void      FillMatchDetail(LPARRANGE_TABLE pArrageTable);
	virtual    void      ArrageTable();
	virtual    void      ArrageOneTable(int nUserIDs[]);
	virtual    void      InitialMatchTable(CTcMatchTable* pTable);
	virtual    BOOL      DisBandTable(LPCOMMIT_DIFF pDisBandTable,CTcMatchTable* pTable);
	virtual    void      SetMatchPtr(CTcMatchBase* Ptr);
	virtual	   BOOL      IsPhaseEnd();
	virtual    BOOL      AutoKickOffliner();        //�Ƿ��Զ��߳��������
	virtual    int       GetStartUserCount();//���ر��׶�����ʱ�����������Ʒ���-1
	virtual    BOOL      IsGameInvalid(){return TRUE;}//�����Ƿ���Ч
	virtual    void      SetPlayerStatusWhenLeave(CTcMatchPlayer* pPlayer);
			   BOOL      IsFirstPhase();
	int        m_nBoutPerTurn; //ÿ�ִ���پ�
	int        m_nArrageTableTimes;//��������
	class      CTcMatchBase*  m_MatchPtr;
	class      CTCMatchPhase* m_NextPhase;
};

//�����������ͣ���������
//��Ҳ������С������߻����֣�ֱ����̭��ʣ��ָ����������
class CTcMatch:public CTcMatchBase
{
public:
	CTcMatch(CBaseRoomData* pRoomData,LPMATCH lpMatch);
	virtual ~CTcMatch();
	void                             CreateMatch(LPMATCH_STARTTOROOM pStartMatch);
	void                             CreatePlayer(LPMATCH_PLAYER lmp,LPUSERONMGAME lug,LPUSERONMATCH lum,LPMATCH_HINT lmh);//��ҽ���
	//��Ϣ����
	void                             PushBackPhase(CTCMatchPhase* phase);
	void                             BeginMatch();//������Ϸ��ʼ
	BOOL                             CommitResult(LPCOMMIT_DIFF pResult);//ˢ�½��
	void                             FreshUserResult(LPUSER_DIFF pResult);
	void                             DisBandTable(LPCOMMIT_DIFF pDisBandTable);//����
	virtual  BOOL				     TerminateTable(int nTableNO);
	virtual  BOOL					 ForceKickPlayer(int nUserID);
	virtual  void                    OnPlayerReady(LPMATCHUSER_READY pPlayerReady);
	virtual  void                    OnPlayerOK(LPMATCH_PLAYER_OK pPlayerOk); //��һ�ȡ����״̬
	virtual  void                    OnPlayerLeave(LPMATCHUSER_LEAVE pPlayerLeave);//����뿪
//	virtual  void                    OnContinueMatch(LPCONTINUE_MATCH pContinueMatch);
	BOOL                             FreshMatch(); //��ʱˢ�±���״̬

	virtual  BOOL                    IsMatchStart();            //��������Ƿ��Ѿ���ʼ
	virtual  BOOL                    IsMatchEnd();				//��������Ƿ��Ѿ�����
	virtual  void                    OnMatchEnd();				//������������
	virtual  void                    ClearMatch();              //��������

	virtual  void                    ArrageTable();				//����

	virtual  BOOL                    TransMatchPhase();         //ת�������׶�
	virtual  void                    CheckTableStatus();        //���ÿ������Ϸ״���Ƿ�����
	virtual  void                    CheckPlayerStatus();       //���ÿ����ҵ���Ϸ״��

	virtual  void                    CalcBaseScore();
	virtual  void                    CalcLevelScore();

	CTCMatchPhase*                   m_CurrentPhase;
	CTCMatchPhase*                   m_FirstPhase;
};

//������̭
class CTCMatchPhase2:public CTCMatchPhase
{
public:
	  CTCMatchPhase2(int nBoutPerTurn,int nChairPerTable,int nTotalPlayerCount);
	 ~CTCMatchPhase2();
public:
	virtual    void      TransPhase(); //����ǰһ���׶�
	virtual    void      BeginPhase(); //��ʼ�׶�

	virtual    int       CalcBaseScore();
	virtual    int       CalcLevelScore();
    virtual    BOOL      IsPrepareEnterPhase();
	virtual    BOOL      IsReadyEnterPhase();//�Ƿ������������׶�
	virtual    void      FillMatchDetail(LPARRANGE_TABLE pArrageTable);
	virtual    void      ArrageTable();
	virtual    void      InitialMatchTable(CTcMatchTable* pTable);
	virtual    BOOL      DisBandTable(LPCOMMIT_DIFF pDisBandTable,CTcMatchTable* pTable);
	virtual    int       GetStartUserCount();//���ر��׶�����ʱ�����������Ʒ���-1
	virtual    BOOL      AutoKickOffliner(); //�Ƿ��Զ��߳��������
	virtual    void      SetPlayerStatusWhenLeave(CTcMatchPlayer* pPlayer);

	int m_nWashTurn;       //��̭�ִ�
	int m_nTransUserCount; //ת�׶ν�ֹ������ʣ��������������������ֱ����̭���ȴ��������ξ�����������
	int m_nTurnIndex;      //��̭��n��
};

//ѭ��������
class CTCMatchPhase3:public CTCMatchPhase
{
public:
	CTCMatchPhase3(int nBoutPerTurn,int nMatchSeconds,int nWinBout);
	~CTCMatchPhase3();
public:
	virtual    void      TransPhase(); //����ǰһ���׶�
	virtual    void      BeginPhase(); //��ʼ�׶�
	virtual    int       CalcBaseScore();
	virtual    int       CalcLevelScore();
	virtual    BOOL      ValidatePlayerEnter(int nUserID);//�ж�����Ƿ���Լ������
	virtual    void      InitialPlayerWhenEnter(CTcMatchPlayer* pPlayer);
	virtual    void      ArrageTable();
	virtual    void      InitialMatchTable(CTcMatchTable* pTable);
	virtual    BOOL      DisBandTable(LPCOMMIT_DIFF pDisBandTable,CTcMatchTable* pTable);
	virtual    BOOL      IsGameInvalid();//�����Ƿ���Ч
	virtual    void      FillMatchDetail(LPARRANGE_TABLE pArrageTable);
public:
			   int       m_nMatchSeconds;
			   int       m_nWinBout;
};

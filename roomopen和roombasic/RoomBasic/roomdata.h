#pragma once

class CBaseRoomData{
public:
	CBaseRoomData(){
		ZeroMemory(&roomdata, sizeof(roomdata));
 		m_pTcMatch=NULL;
	}
	virtual ~CBaseRoomData();

	CCritSec                m_csRoomData;


	ROOMDATA                roomdata;

	CTableMap				m_mapTable;
	CPlayerMap				m_mapPlayer;
	CUserSockMap			m_mapUserSock;
	CTokenUserSockMap		m_mapTokenUserSock;
 	CRoomForbidMap			m_mapRoomForbid;
	CIDMap					m_mapDarkLooker; //保存进入dark room的 具有lookdark权限的玩家ID
	CUnwelcomesMap			m_mapUnwelcomes;

	CClientInfoMap          m_mapClientInfo;

 	CTcMatch*               m_pTcMatch;  //初始化为NULL

	CRoomChatMap			m_mapRoomChat;
	CIDMap					m_mapPrivateTableInfo;
	CIntVecIntMap			m_mapFindPrivateTableCache;
public://比赛处理函数
	void AT_CreateMatch(LPMATCH lpMatch);
 	int  AT_GetReadyPlayerCount();
	void AT_OnMatchStart(LPMATCH_STARTTOROOM lpms);
	void AT_OnMatchCanceled(LPMATCH_CANCELED lpmc);
	void AT_OnMatchSignUped(LPMATCH_SIGNUPED lpms);
	void AT_OnMatchSignUpOK(LPMATCH_SIGNUPOK lpms);
	void AT_OnMatchGiveUpOK(LPMATCH_GIVEUPOK lpms);
	void AT_OnMatchTrigger();//定时每秒调用此函数
	void AT_OnMatchStep(LPMATCH_STEP pStep);
	void AT_OnMatchFinished(int nUserID,int nRank,int nMatchID,int nRoomID,int nMatchStatus);//用户的比赛结束
	void AT_OnMatchEnd();//整场比赛End
 	void AT_OnMatchPlayerOK(LPMATCH_PLAYER_OK pPlayerOk);
	void AT_OnMatchUserReady(LPMATCHUSER_READY lpMatchUserReady);
	void AT_OnMatchUserLeave(LPMATCHUSER_LEAVE lpMatchUserLeave);
	void AT_OnCommitDiff(LPCOMMIT_DIFF lpCommitDiff);
	void AT_OnArrangeTable(LPARRANGE_TABLE lpArrangeTable);//分桌之后更新房间里玩家和桌子
	void AT_OnArrageFaild(LPARRANGE_FAILD lpArrangeTable);//分桌失败
	void AT_OnDisbandTable(LPDISBAND_TABLE lpDisbandTable);//分桌之后更新房间里玩家和桌子
	void AT_OnTerminateTable(LPTERMINATE_TABLE lpTerminateTable);//强制清桌
	void AT_OnMatchTableOverTime(int nTalbeNO);
	void AT_OnMatchResetAllTable();
	void AT_OnKickPlayer(int nUserID);
	void AT_OnEnterMatchOK(LPENTER_MATCH_RETURN lpEnterMatchReturn);
	int  AT_GetMatchUserStatus(LPENTER_MATCH_RETURN lpEnterMatchReturn);//返回玩家当前状态对应的通知号
	void AT_SendUserRankList(int nUserID);
	void AT_OnReFreshUserScore(LPMATCH_REFRESH_USERSCORE lpFreshUserScore);
	void AT_BreakOffMatch();//GameSvr关闭，比赛桌子全部打散重分
	void AT_ContinueMatch();//GameSvr重新连接
	void AT_SignUp(LPMATCH_ASKSIGNUP lpAskSignUp);
	void AT_GiveUp(LPMATCH_ASKGIVEUP lpAskGiveUp);
	void AT_OnSocketClose(LPPLAYER lpPlayer);
public:
	virtual void CopyTableMapEx(PBYTE pData, PLAYER* lpPlayer){}
			int  CopyPlayerMap(PBYTE pData, BOOL bLookDark);
			HWND FindGameSvrWindow();
			void CommitTPS(TABLE* pTable,int nStatus);
			void CommitTPS_SOLO(int nUserID,int nTableNO,int nStatus);
			void CommitTPS_VARI(TABLE* pTable,int nStatus,int nChairStatus[]);
			void CopyMatchPlayerMap(PBYTE pData);
			void CopyMatchPlayerSmall(MATCHPLAYER_SMALL* pms,PLAYER* pPlayer);
			void CopyPlayerPrivInfo(PLAYER_PRIVINFO* ppp,PLAYER* pPlayer);
			void CopySoloPlayer(SOLO_PLAYER* psp,PLAYER* pPlayer);
public:
			int  GetAreaID();
			int  GetGameID();
			int  GetRoomID();
			int  GetMatchID();
			int  GetChairCount();
			int  GetTableCount();
			int  GetShowPlayerCount();
			int  GetPlayingCount(TABLE* pTable);//这桌上正在玩的玩家个数
			DWORD GetTotalPlayingBreak(TABLE* pTable);
			BOOL IsDarkRoom();
			BOOL IsWebGameRoom();
			BOOL IsMicroEndRoom();
			BOOL IsCloakingRoom();
			BOOL IsRandomRoom();
			BOOL IsSoloRoom();
			BOOL IsVariableChair();
			BOOL IsSupportMobile();
			BOOL IsLeaveAlone();
			BOOL IsEarlyLeave();
			BOOL IsPrivateRoom();	// 是否是私人包房
			BOOL IsMatchRoom();
			BOOL IsOldMatchRoom();
			BOOL IsTableDeposit();
			BOOL IsMutexHardRoom();
			BOOL IsExchangeSeat();
			BOOL IsGetUnwelcome();
			BOOL IsGetClientInfo();
			BOOL IsLeagueMachRoom();
			BOOL IsSupportDeposit();
 			BOOL FindPlayerIP(DWORD dwIP) ;
			BOOL FindPlayerOnTable(TABLE* pTable,int nPlayerID);
			BOOL IsPlayingOnTable(TABLE* pTable);//桌上是否有玩家正在玩
			
	virtual BOOL IsFullTable(TABLE* pTable){return FALSE;}
 			BOOL FindPlayerHardID(LPCTSTR lpszHardID);
			BOOL IsMustKeepStarted(PLAYER* pPlayer, int& nWaitSeconds);//必须保持开始状态
			BOOL IsForbidPlayer(int nPlayerID);
			BOOL IsUnwelcomePlayer(int nUserID,int nPlayerID);
			BOOL IsMutexIP(DWORD dwIPAddr1,DWORD dwIPAddr2,int nSameSubs);
			BOOL IsMutexIPGroup(DWORD dwIPAddr1,DWORD dwIPAddr2,int nSameSubs);
			BOOL IsMutexHard(LPCTSTR lpszMachineID1,LPCTSTR lpszMachineID2,int& nExpiration);
			BOOL IsForbidInvite();
			BOOL IsForbidPrivChatByExp(int nExperience,int& nMinExp);
			BOOL IsForbidRoomChatByExp(int nExperience,int& nMinExp);
			BOOL IsForbidMinScoreOnTable(PLAYER* pPlayer,TABLE* pTable,int nMinScore);
			BOOL IsForbidMinDepositOnTable(PLAYER* pPlayer,TABLE* pTable,int nMinDeposit);
			BOOL IsTablePasswordWrong(TABLE* pTable,LPCTSTR lpszPassword);
			BOOL IsSameLANOnTable(int nUserID,TABLE* pTable);
			BOOL IsForbidSpeedOnTable(int nUserID,TABLE* pTable,int nNetSpeed);
			BOOL IsForbidBreakOnTable(int nUserID,TABLE* pTable,float fBreakOff);
			BOOL IsForbidIPOnTable(int nUserID,TABLE* pTable,DWORD dwIPAddr,int nIPConfig);
			BOOL IsMutexHardOnTable(int nUserID,LPCTSTR lpszMachineID,TABLE* pTable,int& nExpiration);
			BOOL IsMutexIPOnTable(int nUserID,TABLE* pTable,DWORD dwIPAddr);
			BOOL IsMutexIPGroupOnTable(int nUserID,TABLE* pTable,DWORD dwIPAddr);
			BOOL IsSameClubOnTable( int nClub,TABLE* pTable);
			BOOL IsSameIPOnTable( DWORD dwIPAddr,int nTableNO);
			BOOL IsSameTimeOnTable( int nMillisecond,int nTableNO);// 
			BOOL IsUnwelcomesOnTable(int nUserID,int nTableNO);				//互相不受欢迎的个数
			BOOL IsAnyOneUnwelcomeYouOnTable(int nUserID,int nTableNO);		//对nUserID不受欢迎的个数
			BOOL IsMutexHardOnTable(LPCTSTR lpszMachineID,int nTableNO);
			BOOL IsSameSeatedOnTable(int nSeatedTableNO,int nTableNO);
			BOOL IsLessExperienceOnTable(int nExperience,int nTableNO);
			BOOL IsSameVolumeIDOnTable(LPCTSTR lpszVolumeID,int nTableNO);
			BOOL IsSamePwdOnTable( DWORD dwPwdCode,int nTableNO);
			BOOL IsSameLAN(int nUserID1,int nUserID2);
			BOOL IsKickoffToomany(int nUserID, int nMemberLevel, int& nLimitCount);
			BOOL IsIsolateOnTable(int nUserID, TABLE* pTable, int nNowSeconds, KICKOFF_DETAIL& kickDetail, int& nRestSeconds);
 			BOOL GetOpenTimeSection(LPCTSTR lpszIniFile,OPENTIME_SECTION* pots);
			CString GetOutOfOpenTimeCause(LPCTSTR lpszIniFile,OPENTIME_SECTION* pots);
			CString FormatDateAndTime(int nDate,int nTime);
			BOOL GetTableDeposit(int nTableNO,int& nMinDeposit,int& nMaxDeposit,CString& sRemark);
			BOOL GetTableDepositSection(int nIndex, int& nTableBegin, int& nTableEnd, int& nMinDeposit,int& nMaxDeposit);
public:
			void SetFixedPosition(PLAYER_POSITION* pp,int nFromTableNO);
			BOOL JoinMin( int nPreTables[],PLAYER_POSITION* pp ,int nMaxPreTables,int nFromTableNO=0 );//尽量分散
			BOOL JoinMax( int nPreTables[],PLAYER_POSITION* pp ,int nFromTableNO=0 );// 发现其他桌缺人时加入
   			void ForbidSameIP( int nPreTables[], DWORD dwIPAddr, int nFromTableNO=0);//nPreTables[]置1表示禁止
 			void ForbidMutexIP( int nPreTables[], DWORD dwIPAddr, int nFromTableNO=0);//nPreTables[]置1表示禁止
 			void ForbidMutexIPGroup( int nPreTables[], DWORD dwIPAddr, int nFromTableNO=0);//nPreTables[]置1表示禁止
			void ForbidSameTime( int nPreTables[],int nFromTableNO=0);//禁止同时进入同一桌
			void ForbidUnwelcomes( int nPreTables[],int nUserID,int nFromTableNO=0);//禁止不受欢迎同一桌
			void ForbidMutexHard( int nPreTables[],LPCTSTR lpszMachineID,int nFromTableNO=0);//禁止硬件互斥同一桌
 			void ForbidVolumeID( int nPreTables[],LPCTSTR lpszVolumeID,int nFromTableNO=0);//禁止硬盘号相同同一桌
			void ForbidSameLAN(int nPreTables[],int nUserID, int nFromTableNO);
			void ForbidSamePwd(int nPreTables[],DWORD dwPwdCode, int nFromTableNO);//禁止密码相同玩家一桌
			void ForbidSameSeated(int nPreTables[],int nSeatedTableNO,int nFromTableNO=0) ;//禁止相同的入坐桌号
			void ForbidMicroExperience(int nPreTables[],int  nExperience, int nFromTableNO);//禁止经验值小的玩家同桌
			void ForbidSmallExperience(int nPreTables[],int  nExperience, int nFromTableNO);
			void ForbidSameClub(int nPreTables[],int  nClubID,int nFromTableNO);//禁止相同俱乐部的玩家进入同一桌
 			int  GetUnwelcomeExperience();//能够提交不受欢迎的玩家的至少经验值
			int  GetKeepStartedSecond();
			int  GetFreeTableNO( int nFromTableNO);
			int  GetBeginRandomTableNO();
			int  GetFreeChairNO(TABLE* pTable);
			BOOL GetFreeChairNO2(int& nTableNO,int& nChairNO);
 			int  GetPreTablesCount(int nPreTables[]);
			void MoveDarkTable(TABLE* pTable,TABLE* pNewTable);
	virtual void MoveRandomTable(TABLE* pTable,TABLE* pNewTable){};
			void SendDarkPlaying(TABLE* pTable,PLAYER_POSITION* pp,LPTABLE&  pNewTable);
	virtual	void SendRandomPlaying(TABLE* pTable,PLAYER_POSITION* pp,LPTABLE& pNewTable){};
			void SetRandomPosition(PLAYER* pPlayer,TABLE* pTable,PLAYER_POSITION* pp ); //把玩家设置到随机桌
			void SetDarkPosition(PLAYER* pPlayer,TABLE* pTable,PLAYER_POSITION* pp ); //把玩家设置到随机桌
			void ClearUnwelcomesMap();
			void ClearClientInfoMap();
			void RemoveClientInfo(int nUserID);
			CString GetGameSvrFolder();
			CString GetGameSvrIniFile();
			void GetMutexGroupIP(DWORD dwIPAddr,LPTSTR lpszIP);
public:
			void PostSecurityToHold( int nUserID,LPCTSTR lpszVolumeID,int nTokenID,int nUserType);
			void PostHardAndToken(int nUserID,LPCTSTR lpszHardID,int nTokenID) ;
  			void PostGetFinished(int nUserID,int nTableNO,int nChairNO);
			void PostRoomTableChair(int nUserID,int nTableNO,int nChairNO);
			void PostNewChair(int nUserID,int nTableNO,int nChairNO);
			void PostHomeUserID(int nTableID,int nHomeUerID);
			void PostNewTableChair(int nUserID, int nTableNO, int nChairNO);
			void PostPlayerPosition( TABLE* pTable);
			void PostLeaveRoom(int nUserID);
			void PostExperience(int nUserID,int nExperience);
			void PostClearTable(int nTableNO);
			void PostRoomData();
	virtual void PostStartSoloTable(TABLE* pTable){};
			void PostKickoffPlayer(int nUserID,int nTableNO,int nChairNO);
			void PostUserClothing(int nUserID, int nClothingID);
			void PostBecomeMember(int nUserID, int nUserType);
			void PostVerifyRoomTableChair(int nUserID,int nTableNO,int nChairNO);
public:
	void  TransmitPlayerInRoom(PLAYER* lpPlayer,SEARCH_PLAYER* lpSearchPlayer);
	void  TransmitPlayerInRoomEx(PLAYER* lpPlayer,SEARCH_PLAYER_EX* lpSearchPlayerEx);

	void  NotifyMatchSvr(int nNotifyID,void * pData,int nLen,int nUserID=0);
public:
	int   GetFreeChairOnTable(LPTABLE pTable);
	BOOL  IsAllStartedOnTable(LPTABLE pTable);
 
	virtual void  GetRandomPosition( PLAYER* pPlayer,PLAYER_POSITION* pp){}
	virtual BOOL  GetPreferPrivateTableChair(LPPLAYER pPlayer,int& nTableNo,int& nChairNo, DWORD dwIPAddr, int nIPConfig=0, int nMinScore=0, int nMinDeposit=0,int ExcludedHomeID=0);
	virtual BOOL  GetPreferTableChair(LPPLAYER pPlayer,int& nTableNo,int& nChairNo, DWORD dwIPAddr, int nIPConfig=0, int nMinScore=0, int nMinDeposit=0){return TRUE;}	//找到能尽快开始的桌椅
	virtual BOOL  GetNextTableChair(LPPLAYER pPlayer,int& nTableNo,int& nChairNo, DWORD dwIPAddr, int nIPConfig=0, int nMinScore=0, int nMinDeposit=0) {return TRUE;}		//从当前桌椅开始，找到下一张符合要求的桌椅
	virtual BOOL  GetSepicifyTableChair(LPPLAYER pPlayer,int& nTableNo,int& nChairNo, int nTableBegin, int nTableEnd, DWORD dwIPAddr, int nIPConfig=0, int nMinScore=0, int nMinDeposit=0) {return TRUE;}		//从当前桌椅开始，找到下一张符合要求的桌椅
	virtual BOOL  GetAppointTableChair(LPPLAYER pPlayer,int nTableNo,int& nChairNo);
	void FillCloakingDetail(CLOAKING_DETAIL* pcd);

	int	  GetStartedPlayerCountOnTable(LPTABLE pTable);
public:
	///////////////////////////////////////////////////////////////////////
	///////        约牌系统      //////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	CQuanDataMap			m_mapQuanData;
	int  GetQuanPlayerCount(int nQuanID,int& nTableNO);
	int  CopyQuanPlayer(PBYTE pData, int nQuanID);
	void CopyQuanTable(PBYTE pData, TABLE* pTable);
	BOOL IsYuePaiRoom();
	int  GetUserQuanTableNO(int nUserID,int nQuanID,int& nChairNO);
 	int  GetFreeTableNOEx( int nFromTableNO);
	int  GetFreeChairNO(int nTableNO);
	void ClearQuanDataMap();
	int  GetUserQuanTableNOEx(int nUserID,int nQuanID,int& nChairNO);	//nChairNO是该轮指定了的椅子号

public:
	//////////////包房//////////////////
	BOOL IsOpenPrivateTable(TABLE* pTable);//是否开放包间
	void SetOpenPrivateTable(TABLE* pTable,BOOL bOpen);
	void CalculateAvgValue(TABLE* pTable,int & nAvgBounts,int & nAvgWins,int & nAvgScore,int & nAvgDeposit);
	int  FindPrivateTables(LPPLAYER pPlayer,ONE_TABLEROOM  Result[],int nNeedNum);//返回找到的数量
	void DeletePrivateTableCache(int nPlayerID);
	int  GetPrivateTableCacheNum(int nPlayerID);
	int  CalculatePrivateTablesCount(LPPLAYER pPlayer,ONE_TABLEROOM  Result[],int nWantCount,int nNowResultCount,int nNeedNum);
	int  PrivateTableDetail(TABLE* pTable,ONE_TRPLAYER  Result[]);//包房详情 返回找到的数量
	BOOL CheckSystemFindPlayersCD(int nTableNO); //包房检查 系统找人时间 true - 说明可以触发找人
	void SetSystemFindPlayersTime(int nTableNO); //设置本次系统找人时间点
	int BeginSystemFindPlayers(int  nNeedCount,LPBE_FOUND_BY_SYSTEM pMsg,int nFindMode ,const TABLEAVGVALUE& avgValue); // 返回通知的人的数量
	
};

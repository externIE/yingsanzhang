#pragma once

#define MYSTRSPACEC  "       "

class CGameServer : public CMainServer{
public:
	CGameServer( const TCHAR* szLicenseFile,
			 const TCHAR* szProductName,
			 const TCHAR* szProductVer,
			 const int nListenPort, const int nGameID,
			 DWORD flagEncrypt = 0, DWORD flagCompress = 0)
			 : CMainServer(szLicenseFile, szProductName, szProductVer,
				nListenPort, nGameID, flagEncrypt, flagCompress)
    {
        m_strSpace = MYSTRSPACEC;
        ReadRecordDataFromFile();
    }
		//
protected:	 
    //	
	virtual BOOL OnRequest(void* lpParam1, void* lpParam2);
	virtual unsigned MessageThreadProc();
	
	// for sub request
// event proceed
	virtual CTable* OnNewTable(int roomid = INVALID_OBJECT_ID, int tableno = INVALID_OBJECT_ID, int score_mult = 1);
	virtual int	 OnUserStart(CTable* pTable,int chairno);//返回当前开始的用户数

	//一些notify方法 from zgsa
	virtual BOOL NotifyPlayerMsgAndResponse(LPCONTEXT_HEAD lpContext, CTable* pTable,int nDest,DWORD dwFlags,DWORD datalen=0,void* data=NULL);
	virtual BOOL NotifyTableMsg(CTable* pTable,int nDest,int nMsgID,int datalen=0,void* data=NULL,LONG tokenExcept=0);
	virtual BOOL NotifyResponseFaild(LPCONTEXT_HEAD lpContext);
	virtual BOOL NotifyResponseSucceesd(LPCONTEXT_HEAD lpContext); 

	virtual BOOL OnSendSysMsgToServer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);//消息处理

	//2.0新增，match， 掉线托管
	virtual void OnSeverAutoPlay(CRoom* pRoom,CTable* pTable,int chairno);
	virtual BOOL OnEnterMatchDXXW(LPCONTEXT_HEAD lpContext, CTable* pTable,CPlayer* pPlayer);
	virtual BOOL SimulateGameMsgFromUser(int nRoomID,CPlayer* player,int nMsgID,int nDatalen,void* data,DWORD dwSpace=0);
    virtual void OnPlayerLeave(LPCONTEXT_HEAD lpContext,CTable* pTable,int chairno);
	virtual int  CalcEnterUserDatelen(CRoom* pRoom,CTable* pTable,CPlayer* pPlayer);//计算进入游戏时，返回的用户信息长度
 
	virtual BOOL OnChatToTableEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

	//桌银限制
	//////////////////////////////////////////////////////////////////////////
#define RM_TABLEDEPOSIT 0x00000200	
	virtual BOOL IsTableDepositRoom(int roomid);
	virtual BOOL GetRoomSvrIniFile(int roomid, CString& sRoomSvrIniFile);
	virtual int	 GetTableDepositMin(int roomid, int tableno);
	virtual int	 GetTableDepositMax(int roomid, int tableno);
	virtual BOOL OnSaveSafeDeposit(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnSaveSafeDepositOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
	virtual int  GetMinPlayingDeposit(CTable *pTable, int roomid);		//玩游戏所需的最小银两下限
	virtual BOOL OnTakeSafeDeposit(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnTakeSafeDepositOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
	virtual BOOL OnAskNewTableChair(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual int  OnNewTableChair(int roomid, int tableno, int chairno, int userid);
	virtual BOOL OnLeaveGame(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnLeaveGameEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);//等待回应
	virtual void CopyEnterUserData(CRoom* pRoom,CTable* pTable,CPlayer* pPlayer,BYTE* pSendClient,int nInfoLen);
	
    virtual BOOL OnGameWin(LPCONTEXT_HEAD lpContext, CRoom* pRoom,CTable* pTable, int chairno, BOOL bout_invalid, int roomid);
    virtual BOOL OnUserBoutEnd(LPCONTEXT_HEAD lpContext, CRoom* pRoom,CTable* pTable, int chairno, BOOL bout_invalid, int roomid);

	virtual BOOL OnStartGame(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnStartGameEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);//等待回应
	virtual BOOL OnEnterGameDXXW(LPCONTEXT_HEAD lpContext, LPENTER_GAME_EX pEnterGame);
	virtual BOOL OnGameStarted(CTable* pTable, DWORD dwFlags);

    //////////////////////////////////////////////////////////////////////////

			BOOL IsGameWin(CGameTable* pTable);
			int  GetAlivePlayerCount(CGameTable* pTable);

			//牌局操作相关
			UINT OnLookCard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);
			UINT OnShowCard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);
			UINT OnGiveUp(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);
			UINT OnBet(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);
			UINT OnRise(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);
			UINT OnFollowBet(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);
			UINT OnCompare(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable, int& nLoseChair);
			UINT OnAllIn(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable, ALL_IN_RESPONSE& allInResponse);
			UINT OnBetFull(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);

			//坐下和旁观状态转换相关
			BOOL OnChangeSeatedToLookOn(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnChangeLookOnToSeated(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			void OnChangeSeatedToLookOnResult(int roomid, int tableno, int chairno, int userid);
			void OnChangeLookOnToSeatedResult(int roomid, int tableno, int chairno, int userid);
			void PostChangeSeatedToLookOn(int nRoomID, int nTableNO, int nChairNO, int nUserID);
			void PostChangeLookOnToSeated(int nRoomID, int nTableNO, int nChairNO, int nUserID);
private:
            //统计日志相关
            void InitRecord();  //初始化统计日志信息数据
            CString GetRecordFile(); //获取统计日志路径
            void ReadRecordDataFromFile(); //读取日志文件
            void RecordData(CGameTable* pTable, int chairno, int nFees[], int nDepositDiff[]) ;//写日志文件
            void RecordDataOne(CGameTable* pTable, int chairno, int nFees, int nDepositDiff); 
            void SaveRecordDataToFile(BOOL bImmediate = FALSE);	//保存日志文件
private:
    DWORD m_dwTimeLastSave;			//上次玩家信息的保存时间
    CString	m_strSpace;					//各字段中间空格
    CString m_strRecordFile; //统计信息保存路径
    DWORD m_dwTodayPlayerWinCount;	//今日收集银子流通 -- 日流通
    DWORD m_dwTodayPlayerCount;		//今日收集的总人数 -- 日活跃
	DWORD m_dwTodayTotalFee;	    //今日收集的总茶水费
    DWORD m_dwTodayTotalBount;      //今日收集的总局数
    DWORD m_dwTodayPlayerDepositCount;  //所有玩家身上所有的银子

    CCritSec m_cslistPlayerRecord;       //对m_listPlayerRecord加锁 注：此锁经常和m_csmapRoomRecord一起用，注意顺序一致，先此锁再m_csmapRoomRecord
    CCritSec m_csmapRoomRecord;          //对m_mapRoomRecord加锁
    PLAYERRECORDLIST m_listPlayerRecord; //今日玩家数据链表
    ROOMRECORDMAP m_mapRoomRecord;		 //今日房间信息记录链表

	int		m_nOpen;					//是否显示弃牌按钮

};

inline int Svr_RetrieveFields ( TCHAR *buf, TCHAR **fields, int maxfields, TCHAR**buf2 )
{
	if ( buf == NULL )
		return 0;
	
	TCHAR *p;
	p = buf;
	int count = 0;
	
	try{
		while ( 1 ) {
			fields [ count ++ ] = p;
			while ( *p != '|' && *p != '\0' ) p++;
			if ( *p == '\0' || count >= maxfields ) 
				break;
			*p = '\0';
			p++;
		}
	}catch(...)
	{
		buf2=NULL;
		return 0;
	}
	
	if ( *p == '\0' ) 
		*buf2 = NULL;
	else
		*buf2 = p+1;
	*p = '\0';
	
	return count;
}

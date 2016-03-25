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
	virtual int	 OnUserStart(CTable* pTable,int chairno);//���ص�ǰ��ʼ���û���

	//һЩnotify���� from zgsa
	virtual BOOL NotifyPlayerMsgAndResponse(LPCONTEXT_HEAD lpContext, CTable* pTable,int nDest,DWORD dwFlags,DWORD datalen=0,void* data=NULL);
	virtual BOOL NotifyTableMsg(CTable* pTable,int nDest,int nMsgID,int datalen=0,void* data=NULL,LONG tokenExcept=0);
	virtual BOOL NotifyResponseFaild(LPCONTEXT_HEAD lpContext);
	virtual BOOL NotifyResponseSucceesd(LPCONTEXT_HEAD lpContext); 

	virtual BOOL OnSendSysMsgToServer(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);//��Ϣ����

	//2.0������match�� �����й�
	virtual void OnSeverAutoPlay(CRoom* pRoom,CTable* pTable,int chairno);
	virtual BOOL OnEnterMatchDXXW(LPCONTEXT_HEAD lpContext, CTable* pTable,CPlayer* pPlayer);
	virtual BOOL SimulateGameMsgFromUser(int nRoomID,CPlayer* player,int nMsgID,int nDatalen,void* data,DWORD dwSpace=0);
    virtual void OnPlayerLeave(LPCONTEXT_HEAD lpContext,CTable* pTable,int chairno);
	virtual int  CalcEnterUserDatelen(CRoom* pRoom,CTable* pTable,CPlayer* pPlayer);//���������Ϸʱ�����ص��û���Ϣ����
 
	virtual BOOL OnChatToTableEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

	//��������
	//////////////////////////////////////////////////////////////////////////
#define RM_TABLEDEPOSIT 0x00000200	
	virtual BOOL IsTableDepositRoom(int roomid);
	virtual BOOL GetRoomSvrIniFile(int roomid, CString& sRoomSvrIniFile);
	virtual int	 GetTableDepositMin(int roomid, int tableno);
	virtual int	 GetTableDepositMax(int roomid, int tableno);
	virtual BOOL OnSaveSafeDeposit(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnSaveSafeDepositOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
	virtual int  GetMinPlayingDeposit(CTable *pTable, int roomid);		//����Ϸ�������С��������
	virtual BOOL OnTakeSafeDeposit(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnTakeSafeDepositOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer);
	virtual BOOL OnAskNewTableChair(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual int  OnNewTableChair(int roomid, int tableno, int chairno, int userid);
	virtual BOOL OnLeaveGame(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnLeaveGameEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);//�ȴ���Ӧ
	virtual void CopyEnterUserData(CRoom* pRoom,CTable* pTable,CPlayer* pPlayer,BYTE* pSendClient,int nInfoLen);
	
    virtual BOOL OnGameWin(LPCONTEXT_HEAD lpContext, CRoom* pRoom,CTable* pTable, int chairno, BOOL bout_invalid, int roomid);
    virtual BOOL OnUserBoutEnd(LPCONTEXT_HEAD lpContext, CRoom* pRoom,CTable* pTable, int chairno, BOOL bout_invalid, int roomid);

	virtual BOOL OnStartGame(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnStartGameEx(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);//�ȴ���Ӧ
	virtual BOOL OnEnterGameDXXW(LPCONTEXT_HEAD lpContext, LPENTER_GAME_EX pEnterGame);
	virtual BOOL OnGameStarted(CTable* pTable, DWORD dwFlags);

    //////////////////////////////////////////////////////////////////////////

			BOOL IsGameWin(CGameTable* pTable);
			int  GetAlivePlayerCount(CGameTable* pTable);

			//�ƾֲ������
			UINT OnLookCard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);
			UINT OnShowCard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);
			UINT OnGiveUp(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);
			UINT OnBet(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);
			UINT OnRise(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);
			UINT OnFollowBet(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);
			UINT OnCompare(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable, int& nLoseChair);
			UINT OnAllIn(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable, ALL_IN_RESPONSE& allInResponse);
			UINT OnBetFull(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, int nChairNo, CGameTable* pTable);

			//���º��Թ�״̬ת�����
			BOOL OnChangeSeatedToLookOn(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			BOOL OnChangeLookOnToSeated(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
			void OnChangeSeatedToLookOnResult(int roomid, int tableno, int chairno, int userid);
			void OnChangeLookOnToSeatedResult(int roomid, int tableno, int chairno, int userid);
			void PostChangeSeatedToLookOn(int nRoomID, int nTableNO, int nChairNO, int nUserID);
			void PostChangeLookOnToSeated(int nRoomID, int nTableNO, int nChairNO, int nUserID);
private:
            //ͳ����־���
            void InitRecord();  //��ʼ��ͳ����־��Ϣ����
            CString GetRecordFile(); //��ȡͳ����־·��
            void ReadRecordDataFromFile(); //��ȡ��־�ļ�
            void RecordData(CGameTable* pTable, int chairno, int nFees[], int nDepositDiff[]) ;//д��־�ļ�
            void RecordDataOne(CGameTable* pTable, int chairno, int nFees, int nDepositDiff); 
            void SaveRecordDataToFile(BOOL bImmediate = FALSE);	//������־�ļ�
private:
    DWORD m_dwTimeLastSave;			//�ϴ������Ϣ�ı���ʱ��
    CString	m_strSpace;					//���ֶ��м�ո�
    CString m_strRecordFile; //ͳ����Ϣ����·��
    DWORD m_dwTodayPlayerWinCount;	//�����ռ�������ͨ -- ����ͨ
    DWORD m_dwTodayPlayerCount;		//�����ռ��������� -- �ջ�Ծ
	DWORD m_dwTodayTotalFee;	    //�����ռ����ܲ�ˮ��
    DWORD m_dwTodayTotalBount;      //�����ռ����ܾ���
    DWORD m_dwTodayPlayerDepositCount;  //��������������е�����

    CCritSec m_cslistPlayerRecord;       //��m_listPlayerRecord���� ע������������m_csmapRoomRecordһ���ã�ע��˳��һ�£��ȴ�����m_csmapRoomRecord
    CCritSec m_csmapRoomRecord;          //��m_mapRoomRecord����
    PLAYERRECORDLIST m_listPlayerRecord; //���������������
    ROOMRECORDMAP m_mapRoomRecord;		 //���շ�����Ϣ��¼����

	int		m_nOpen;					//�Ƿ���ʾ���ư�ť

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

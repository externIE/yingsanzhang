#ifndef _XYGEXT2_H_
#define _XYGEXT2_H_

/*
#define		WM_USER							0x0400

#define     GAMESVR_CLSNAME_EX              _T("TCYX_GAMESVR_CLSNAME")
#define     GAMESVR_WNDNAME_EX              _T("TCYX_GAMESVR_WNDNAME")
#define     WM_RTG_RECONNECTSVR_EX          (WM_USER+999)
#define     WM_RTG_GETFINISHED_EX           (WM_USER+1000)
#define     WM_RTG_CLEARTABLE_EX            (WM_USER+1001)
#define     WM_RTG_HARDCODE_EX              (WM_USER+1002)
#define     WM_RTG_TOKENID_EX               (WM_USER+1003)
#define		WM_RTG_ROOMTABLECHAIR_EX		(WM_USER+1004)
#define     WM_RTG_EXPERIENCE_EX			(WM_USER+1005)
#define     WM_RTG_LEAVEROOM_EX             (WM_USER+1006)
#define		WM_RTG_ROOMSVRHWND_EX			(WM_USER+1009)
#define     WM_RTG_ROOMOPTIONS_EX           (WM_USER+1010)  
#define     WM_RTG_ROOMCONFIGS_EX           (WM_USER+1011)
#define     WM_RTG_ROOMMANAGES_EX           (WM_USER+1012)  
#define     WM_RTG_GAMEOPTIONS_EX           (WM_USER+1013)  
#define     WM_RTG_GAMEPARAM_EX             (WM_USER+1014)  
#define     WM_RTG_GAMEDATA_EX              (WM_USER+1015)  
#define     WM_RTG_MINPLAYSCORE_EX          (WM_USER+1025)  
#define     WM_RTG_MAXPLAYSCORE_EX          (WM_USER+1026)  
#define     WM_RTG_MINDEPOSIT_EX			(WM_USER+1027)  
#define     WM_RTG_MAXDEPOSIT_EX			(WM_USER+1028) 
#define     WM_RTG_STARTVALID_EX            (WM_USER+1030)  
#define     WM_RTG_STARTINVALID_EX          (WM_USER+1031)

#define     WM_RTG_STARTSOLOTABLE_EX		(WM_USER+1040)  
#define     WM_RTG_KICKOFFPLAYER_EX			(WM_USER+1050)

#define		WM_GTR_ENTERGAMEOK_EX			(WM_USER+2000)
#define		WM_GTR_VERIFYSTART_EX           (WM_USER+2001)
#define     WM_GTR_LEAVEGAMEOK_EX			(WM_USER+2002)
#define		WM_GTR_GAMESTARTUP_EX			(WM_USER+2003)
#define		WM_GTR_GAMEBOUTEND_EX			(WM_USER+2004)
#define     WM_GTR_ASKNEWTABLE_EX 		    (WM_USER+2005)
#define     WM_GTR_CLOSESOLOTABLE           (WM_USER+2010)
*/


#define		FEE_MODE_FREE			0			// ����
#define		FEE_MODE_FIXED			1			// �̶�
#define		FEE_MODE_TEA			6			// ��ˮ
#define		FEE_MODE_SERVICE		7			// �����ģʽ����ȡÿλ��ҹ̶���������ѣ�ÿ�ָ��Ѻ��ٽ�����Ӯ

#define		MAX_FILTERWORD_LEN		32			// ���˴���󳤶�
#define		MAX_LEVELNAME_LEN_EX	16			// �������Ƴ���
#define		MAX_HARDID_LEN_EX		32			// Ӳ����ʶID��󳤶�
#define		MAX_SERVERIP_LEN_EX		32			// ������IP�����(��NULL)
#define		CLIENT_TYPE_GAME_EX		4			// ��Ϸ������
// 
#define		RF_VERIFY_SCORE			0x00000001			// �����ݿ�У����ֽ��
#define		RF_LOG_TO_DB			0x00000002			// ���ݿ��¼��Ϸ���
#define		RF_VERIFY_DEPOSIT		0x00000004			// �����ݿ�У�����ӽ��
#define		RF_IGNORE_DIFFSUM		0x00000008			// У�����ӽ��ʱ���Բ�ֵ�ܺ�У��(ˮ��,�Ծֽ�)-��֧�ִ���FLAG

/////////////////////////////////////////////////////////////////////////////
// req id from 20000 to 30000 (compact for commreq.h of version 6.0)
// 

#define		COMMON_REQ_BASE_EX		20000

#define     GR_RECONNECT_SVR     (COMMON_REQ_BASE_EX + 12) 

/////////////////////////////////////////////////////////////////////////////

#define		GAME_REQ_BASE_EX		200000
#define		GAME_REQ_INDIVIDUAL		400000

#define		ROOM_REQ_BASE			50000         //ʹ����RoomSvrһ��������
#define     IR_REQ_BASE             90000		  //��insidereq.hһ��

// request (to check server)
#define		GR_GET_PLAYERLEVELS_EX	(GAME_REQ_BASE_EX + 10)		// ��ȡ������Ϣ
#define		GR_SEND_PULSE_EX		(GAME_REQ_BASE_EX + 20)		// ���������ź�
#define		GR_REFRESH_RESULT_EX	(GAME_REQ_BASE_EX + 30)		// ������Ϸ���

#define		GR_ENTER_GAME_EX		(GAME_REQ_BASE_EX + 110)	// �û�������Ϸ
#define		GR_VALIDATE_GAMESVR_EX	(GAME_REQ_BASE_EX + 200)	// �����ʻ�����

#define     GR_SEND_GAMERUNLOG_EX  	(GAME_REQ_BASE_EX + 210)	// ���ͷ�����������־

#define		GR_GET_FILTERWORD_EX	(ROOM_REQ_BASE + 22)		// ��ȡ���˹ؼ���

// response (from check server)
#define		GR_ENTER_GAME_OK_EX		(GAME_REQ_BASE_EX + 500)	// ������Ϸ�ɹ�

#define		GR_ASK_NEW_TABLECHAIR	 (GAME_REQ_BASE_EX+1300)		// ���������λ
#define		GR_LOOK_SAFE_DEPOSIT	(GAME_REQ_BASE_EX + 40)		// �鿴����������
#define		GR_TAKE_SAFE_DEPOSIT    (GAME_REQ_BASE_EX + 41)		// ȡ����������(����Ϸ)
#define		GR_SAVE_SAFE_DEPOSIT	(GAME_REQ_BASE_EX + 42)		// �浽������
#define		GR_TAKE_SAFE_RNDKEY    	(GAME_REQ_BASE_EX + 43)		// ��ȡ�����

// notification  (from check server)
#define		GR_ENTER_ROOM_NOTIFY	(GAME_REQ_BASE_EX + 600)
#define		GR_RESULT_UNSAVED	    (GAME_REQ_BASE_EX + 610)

#define     GR_ADD_FILER_WORD_EX        (IR_REQ_BASE + 110)		 //��ӹ��˹ؼ���
#define     GR_DELETE_FILTER_WORD_EX    (IR_REQ_BASE + 209)	     //ɾ�����˹ؼ���

#define		GR_INPUTLIMIT_DAILY		(ROOM_REQ_BASE + 10086)		// ����ÿ������

// compacted to existed check server
typedef struct _tagGET_PLAYERLEVELS_EX{
	int nGameID;								// ��ϷID
	int nLevelID;								// ����ID
	int nReserved[4];
}GET_PLAYERLEVELS_EX, *LPGET_PLAYERLEVELS_EX;

typedef struct _tagPLAYERLEVELS_EX{
	int nLevelCount;							// �������
	int nGameID;								// ��ϷID
	int nScoreMult;								// ��Ա���
	int nReserved[6];
}PLAYERLEVELS_EX, *LPPLAYERLEVELS_EX;

typedef struct _tagPLAYERLEVEL_EX{
	int nLevelID;								// ����ID
	char szLevelName[MAX_LEVELNAME_LEN_EX];	// ��������
	int nReserved[4];
}PLAYERLEVEL_EX, *LPPLAYERLEVEL_EX;

typedef struct _tagREFRESH_RESULT_EX{
	int nResultCount;							// �������
	int nRoomID;								// ����ID
	int	nGameID;								// ��ϷID
	int nClientID;								// ���Ͷ�ID
	T_DWORD dwFlags;	
	int nTableNO;								// ����
	T_DWORD dwResetTime;							// �ϴ�����ʱ��
	int nReserved[2];
}REFRESH_RESULT_EX, *LPREFRESH_RESULT_EX;

typedef struct _tagGAME_RESULT_EX{
	int	nUserID;								// �û�ID
	int nRoomID;								// ����ID
	int nTableNO;								// ����
	int nChairNO;								// λ��
	int	nGameID;								// ��ϷID
	int	nBaseScore;								// ��������
	int	nBaseDeposit;							// ��������
	int	nOldScore;								// �ɻ���
	int	nOldDeposit;							// ������
	int	nExperience;							// ����ֵ(����)
	int	nTimeCost;								// ��ʱ(��)
	int	nBout;									// �ܻغ�
	int	nBreakOff;								// ���ߴ���
	int	nWin;									// Ӯ(����)
	int	nLoss;									// ��(����)
	int	nStandOff;								// ��(����)
	int	nScoreDiff;								// ��������	
	int	nDepositDiff;							// ������Ӯ
	int	nLevelID;								// ����ID
	char szLevelName[MAX_LEVELNAME_LEN_EX];	// ��������
	int nFee;									// ������
	int nCut;									// ���ܿ���
	int nExtra;									// ��������(ˮ��,�Ծַѵ�)
	int nReserved[3];
}GAME_RESULT_EX, *LPGAME_RESULT_EX;

typedef struct _tagENTER_GAME_EX{
	int nUserID;								// �û�ID
	int nUserType;								// �û�����
	int	nGameID;								// ��ϷID
	int nRoomID;								// ����ID
	int nTableNO;								// ����
	int nChairNO;								// λ��
	char szHardID[MAX_HARDID_LEN_EX];			// Ӳ����ʶ
	T_BOOL bLookOn;								// �Թ�
	int nReserved[3];
	T_DWORD dwUserConfigs;						// ��Ϸ����
	int nReserved2[3];
	int nRoomTokenID;							// ��������ID
	int nReserved3[4];
}ENTER_GAME_EX, *LPENTER_GAME_EX;

typedef struct _tagENTER_GAME_OK_EX{
	int nUserID;								// �û�ID
	int nRoomID;								// ����ID
	int nTableNO;								// ����
	int nChairNO;								// λ��
	int nScore;									// ����
	int	nDeposit;								// ����
	int nLevelID;								// ����ID
	char szLevelName[MAX_LEVELNAME_LEN_EX];	// ��������
	int nUserType;								// �û�����
	int nExperience;							// ����ֵ
	int nBout;		                            // �����Ϸ�ܾ���
	int nReserved[1];
}ENTER_GAME_OK_EX, *LPENTER_GAME_OK_EX;

typedef struct _tagVALIDATE_GAMESVR{
	int nClientID;
	int nClientType;
	int nGameID;
	int nGamePort;
	T_DWORD dwIPAddr;
	int nReserved[6];
}VALIDATE_GAMESVR, *LPVALIDATE_GAMESVR;

typedef struct _tagENTER_ROOM_NOTIFY{
	int nUserID;								// �û�ID
	int nGameID;								// ��ϷID
	int nRoomID;								// ����ID
    int nReserved[4];
}ENTER_ROOM_NOTIFY, *LPENTER_ROOM_NOTIFY;

typedef struct _tagRECONNECT_SVR{	
 	int   nAdminID;
	char szHardID[MAX_HARDID_LEN_EX];	 
  	char szReconnectIP[MAX_SERVERIP_LEN_EX];
	int   nReconnectPort;
   	int nReserved[4];
}RECONNECT_SVR, *LPRECONNECT_SVR;

//typedef CMap<int, int, PLAYERLEVEL_EX*, PLAYERLEVEL_EX*> CPlayerLevelExMap;


#define     RUCID_DATABASEERROR      1
#define     RUCID_INVALIDPARAM		 2
#define     RUCID_OBJECTNOTFIND      3
#define     RUCID_SCOREDIFFER       10
#define     RUCID_DEPOSITDIFFER     11
#define     RUCID_FORBIDDENUSER     20
typedef struct _tagNTF_RESULT_UNSAVED{
	int nCauseID; 
	int nGameID;
	int nRoomID;
	int nUserID;
	int nTableNO;
	T_DWORD dwFlags;
	T_DWORD dwResetTime;
	int nReserved[2];
}NTF_RESULT_UNSAVED, *LPNTF_RESULT_UNSAVED;



typedef struct _tagNTF_RESULT_CHECKED{
	int nResultCount;							// �������
	int nPropsCount;							// ���߷���
	int nClientID;								// ���Ͷ�ID
	
	int nRoomID;								// ����ID
	int	nGameID;								// ��ϷID
	int nTableNO;								// ����
	T_DWORD dwFlags;	
	T_DWORD dwResetTime;							// �ϴ�����ʱ��
	
	int nReserved[7];
}NTF_RESULT_CHECKED, *LPNTF_RESULT_CHECKED;

typedef struct _tagNTF_CHAIR_RESULT{
	int nChairNO;								// λ��
	int	nUserID;								// �û�ID
	int	nTimeCost;								// ��ʱ(��)
	int	nOldScore;								// �ɻ���
	int	nOldDeposit;							// ������
	int	nScoreDiff;								// ��������	
	int	nDepositDiff;							// ������Ӯ
	int	nLevelID;								// ����ID
	int nFee;									// ������
	int nCut;									// ���ܿ���
	int nExtra;									// ��������(ˮ��,�Ծַѵ�)
	int nReserved[3];
}NTF_CHAIR_RESULT, *LPNTF_CHAIR_RESULT;

typedef struct _tagFILTER_WORD_EX{
	int	nWordID;
	int nType;
    char szWord[MAX_FILTERWORD_LEN];
	int nReserved;
}FILTER_WORD_EX, *LPFILTER_WORD_EX;

typedef struct _tagADD_FILTER_WORD_EX{
	int nClientID;
	int nAdminID;
	char szHardID[MAX_HARDID_LEN_EX];				 
    FILTER_WORD_EX  fw; 
   	int nReserved[4];
}ADD_FILTER_WORD_EX, *LPADD_FILTER_WORD_EX;


typedef struct _tagDEL_FILTER_WORD_EX{
	int nClientID;
	int nAdminID;
	char szHardID[MAX_HARDID_LEN_EX];			 
    int nWordID;
   	int nReserved[4];
}DEL_FILTER_WORD_EX, *LPDEL_FILTER_WORD_EX;

typedef struct _tagAsk_NewTableChair{
	int nUserID;								// �û�ID
	int nRoomID;								// ����ID
	int nTableNO;								// ����
	int nChairNO;								// λ��
	int nReserved[4];
}ASK_NEWTABLECHAIR, *LPASK_NEWTABLECHAIR;

typedef struct _tagTAKE_SAFE_DEPOSIT{
	int nUserID;								// �û�ID
	int nGameID;								// ��ϷID
	int nRoomID;
	int nTableNO;
	int nChairNO;	
	int nDeposit;								// ����
	int nKeyResult;								// ������ = func(��������, �����)
	int nPlayingGameID;                         
	int nGameVID;
	int nTransferTotal;//���������Ϸ���ܺ�(�۳�������)��ע�����Ϊ��ֵ��
	int nTransferLimit;//���������Ϸ���޶�����(��,>0)��
	T_DWORD dwIPAddr;                             //IP��ַ
	char szHardID[MAX_HARDID_LEN_EX];			// Ӳ����ʶ�����������кţ�	
	int nGameDeposit;
	int nReserved[7];
}TAKE_SAFE_DEPOSIT, *LPTAKE_SAFE_DEPOSIT;

typedef TAKE_SAFE_DEPOSIT    SAVE_SAFE_DEPOSIT;
typedef LPTAKE_SAFE_DEPOSIT  LPSAVE_SAFE_DEPOSIT;

typedef struct _tagLOOK_SAFE_DEPOSIT{
	int nUserID;								// �û�ID
	int nGameID;
	T_DWORD dwIPAddr;                             //IP��ַ
	char szHardID[MAX_HARDID_LEN_EX];			// Ӳ����ʶ�����������кţ�	
	int nReserved[8];
}LOOK_SAFE_DEPOSIT, *LPLOOK_SAFE_DEPOSIT;

typedef struct _tagSAFE_DEPOSIT_EX{
	int  nUserID;
	int  nDeposit;								// ����
	long bHaveSecurePwd;                        //�Ƿ��б�������
	int  nRemindDeposit;                        //����������ӣ�������Ҫ���ñ�������
	int  nReserved[4];
}SAFE_DEPOSIT_EX, *LPSAFE_DEPOSIT_EX;

typedef struct _tagTAKE_SAFE_DEPOSIT_OK{
	int nUserID;								// �û�ID
	int nGameID;								// ��ϷID
	int nRoomID;
	int nTableNO;
	int nChairNO;
	int nDeposit;								// ����
	int nGameDeposit;                           //��Ϸ����(����)
	int nReserved[7];
}TAKE_SAFE_DEPOSIT_OK, *LPTAKE_SAFE_DEPOSIT_OK;

typedef TAKE_SAFE_DEPOSIT_OK    SAVE_SAFE_DEPOSIT_OK;
typedef LPTAKE_SAFE_DEPOSIT_OK  LPSAVE_SAFE_DEPOSIT_OK;

typedef LOOK_SAFE_DEPOSIT    TAKE_SAFE_RNDKEY;
typedef LPLOOK_SAFE_DEPOSIT  LPTAKE_SAFE_RNDKEY;

typedef struct _tagSAFE_RNDKEY{
	int nUserID;
	int nRndKey;
	int nReserved[4];
}SAFE_RNDKEY, *LPSAFE_RNDKEY;
#endif
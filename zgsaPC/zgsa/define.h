#pragma once

///////////////////////////����汾////////////////////////
#define APP_NAME       _T("zgsa")
#define VER_MAJOR       2
#define VER_MINOR       1
#define VER_BUILDNO     20160324

typedef struct _tagGAME_SET{
	int        nSoundEffect;    //�Ƿ����Ч
	int        nMusic;          //�Ƿ������
	int        nRejectKanPaiAsk;//������
	int		   nRejectPassSound;	//����������
    int        nPokerType;  // 
}GAME_SET, *LPGAME_SET;


enum{
    	GAME_CALLBACKMSG_BEGIN =GAME_MSG_END+1,
		GAME_CALLBACKMSG_DEALCARD,         //����
		GAME_CALLBACKMSG_DEALCARD_END,
	    GAME_CALLBACKMSG_OPE_START,
		GAME_CALLBACKMSG_ONTIMER,
		GAME_CALLBACKMSG_MOVERESULT,
		GAME_CALLBACKMSG_RESTARTGAME,      //restart the game
		GAME_CALLBACKMSG_HITSCREEN,//�����Ļ 
		GAME_CALLBACKMSG_COUNTDOWNZERO,

		ZGSA_SAFEBOX_SETDEPOSIT_BTN_SURE,
		ZGSA_SAFEBOX_SETDEPOSIT_BTN_CLOSE,
		ZGSA_SAFEBOX_SETDEPOSIT_CHECK_SET,

	 	GAME_MSG_USER_BOUT_END,	 	//����м��˳�
		
		GAME_CALLBACKMSG_END,
		 
		MSG_BUTTON_BEGIN, 
        MSG_BUTTON_BET,
		MSG_BUTTON_RISE,
		MSG_BUTTON_FOLLOW,
		MSG_BUTTON_LOOK_CARD,
		MSG_BUTTON_ALLIN,
		MSG_BUTTON_FULL_CHARGE,
		MSG_BUTTON_COMPARE,
		MSG_BUTTON_COMPARE_INDEX1,
		MSG_BUTTON_COMPARE_INDEX2,
		MSG_BUTTON_COMPARE_INDEX3,
		MSG_BUTTON_COMPARE_INDEX4,
		MSG_BUTTON_COMPARE_INDEX5,
		MSG_BUTTON_BET_TWO,
		MSG_BUTTON_BET_FOUR,
		MSG_BUTTON_BET_EIGHT,
		MSG_BUTTON_GIVE_UP, 
		MSG_BUTTON_AUTO_START_FOCUS,
		MSG_BUTTON_AUTO_START,
		MSG_BUTTON_AUTO_FOLLOW_FOCUS,
		MSG_BUTTON_AUTO_FOLLOW,
		MSG_BUTTON_WIN_SHOW,
		MSG_BUTTON_SIT_ON,
		MSG_BUTTON_STAND_BY,
		MSG_BUTTON_CHANGE_TABLE,
		MSG_BUTTON_BET_FULL,	//ѹ��
		MSG_BUTTON_AUTO_SET_DEPOSIT, //�Զ�����
		MSG_BUTTON_END,
		
		OPE_CALLBACKMSG_BEGIN, 
		MSG_AFTER_ROUND_ANIMATION, 
		MSG_SHOW_RESULT_END,  //�����������
		MSG_HIDE_POT_WINCARD,   //�����ص�Ӯ�� ����
        MSG_REQ_NEW_ROUND,
		MSG_COMPARE_RESULT,
		OPE_CALLBACKMSG_END,
};

 


enum GAME_STATUS_CARD
{
    GAME_STATUS_TABLE_SIZE=GAME_STATUS_CUSTOMER_BEGIN+1, 
};

#define	   MYDRAWINDEX						0
#define    ZGSA_PLAYER_NAME_WIDTH			128
#define    ZGSA_PLAYER_NAME_HEIGHT			18
#define    ZGSA_PLAYER_FACE_WIDTH			48
#define    ZGSA_PLAYER_FACE_HEIGHT			48

#define    CARD_HEIGHT_SMALL				68
#define    CARD_WIDTH_SMALL					50 
 
#define    ZGSA_CLOCK_WIDTH           40
#define    ZGSA_CLOCK_HEIGHT          20    

#define	   COMPARE_MOVE_SPEED         20
#define	   TAKE_BET_SPEED			  10
#define	   AUTO_FOLLOW_TIME			  1

//������Ϣ
#define    CHIPS_COUNT              10
struct CHIP_DESC
{
	int nUnit; //1  5 10 20 50  100 500 1000 5000 10000
 	TCHAR szResName[30]; 
};    

static CHIP_DESC  g_chipsDesc[CHIPS_COUNT]=
{
	{	1,"icon_chip_1"},
	{	5,"icon_chip_5"},
	{	10,"icon_chip_10"},
	{	20,"icon_chip_20"},
	{	50,"icon_chip_50"},
	{	100,"icon_chip_100"},
	{	500,"icon_chip_500"},
	{	1000,"icon_chip_1000"},
	{	5000,"icon_chip_5000"},
	{	10000,"icon_chip_10000"},
};

struct CHIP_POS 
{
	int nChipDescIndex;		//������Ϣ���
	int nClusterPos;		//λ�ڳ�����һ��
	int nBetIndex;			//�������
};

enum{
		STATUS_WAITING_START = 0,  //���˿�ʼ ��ť ��Ϸû��ʼ
		STATUS_START_COUNTDOWN,
		STATUS_PLAYING_GAME,
		STATUS_SHOW_RESULT
};
//////////////////////////////////////////////////////////////////////////
#define CUT_OUT_TWO_BYTE 0x0000ffff
#define CUT_OUT_ONE_BYTE 0x000000ff

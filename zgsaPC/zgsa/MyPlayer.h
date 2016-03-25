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
	
	virtual void  CreateGUI(int nDrawIndex);  //������ң���CGameWinApp����
	virtual void  LoadPlayer(int nChairNO);
	virtual void  UnLoadPlayer();  

	virtual void  FitScrren();//ˢ��λ�� 
	virtual void  Restart();
	EVENTHANDLE      SNGAPI  APPLY_PlayBitObjectEx(BITObject* ob,
		int  mode,
		int  param,
		int  nStartFrame,  //������ȥ��ʱ��
		DWORD nTimeSpace,
		SNG_MSG* CallBackMsg);
 	virtual void  SetCurrentChair(BOOL bn,int nTimeRest=-1); 
	virtual POINT GetClockPosition();   

    int			GetTableSize();
	POINT		GetFaceCenter();  //�������Ϊ��׼�� 
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

	void		ShowBanker(BOOL bShow); //��ʾׯ�ұ�� 
    void		FreshPlayerBetInfo();		//ˢ��ʣ�������������Ϣ

    void		MOVIE_FreshRestBet(int nAddBet, int nWinChair, int nWinType);

	void		MOVIE_ThrowCard(); //����ʱ����

	void		SetOpeStatus(int nStatus);

    //void		ShowMask(BOOL bShow);

	void		SetTakeDeposit(BOOL bTake){m_bTakeDeposit=bTake;} //�Ƿ���
	BOOL		IsTakeDeposit(){return m_bTakeDeposit;}
	
	//׼�����
	void  		ShowReady();
	void  		HideReady(); 

	//��������������Դ���
	void		ShowBrightOrDarkBoard(BOOL bBright = TRUE, BOOL bLost = FALSE);
	void		HideBrightOrDarkBoard();

	//����ʧ�����
	void		ShowLostLightningMask();
	void		HideLostLightningMask();

	//�����ư�ť���
	void		ShowCompareBtn();
	void		HideCompareBtn();
	BOOL		IsShowCompareBtn();

	//Ӯ�ƶ������
	void		ShowWinAni(int nWinType);
	void		HideWinAni();

	//����Դ���
	void		ShowPlayerFire();
	void		HidePlayerFire();

	void		HideTimeAni();
	void		HideNewDigitAni();
	
public:
    CMyDigit*		m_digitRestBet;		//ʣ������
	CMyDigit*		m_digitUsedBet;		//�����������
	BITObject*		m_bitBrightOrDark;	//��������������Դ
	BITObject*		m_bitLost;			//������Դ
	BOOL			m_bLost;			//����ʧ��
	BITObject*		m_bPlayerBoxBk;		//����ͷ��򱳾�
	BITObject*		m_bitTimeAni;		//ʱ�䶯��
	BITObject*		m_bitTimeAniBk;		//ʱ�䶯������
	
protected:
	int				m_nChairNO;			//��λ��
	int				m_nDrawIndex;		
	int				m_nWinType;			//Ӯ������
	
	BITObject*		m_bitBoard;			//����
	BITObject*		m_bitBanker;		//ׯ��
	BITObject*		m_bitOpeStatus;		//��Ϸδ��ʼʱ��״̬
	BITObject*		m_bitRetStatus;		//��Ϸ�е�״̬
	BITObject*		m_bitRetStatusBk;	//״̬������
    BITObject*		m_bitReady;			//׼��
	BITObject*		m_bitNameBoard;		//�������
	BITObject*		m_bitEmptySeat;		//��λ
	BITObject*		m_bitPlayerFire;	//����Դ
	BITObject*		m_bitWinAni;		//��Ӯ����
   
	PlaneObject*	m_plPlayerFace;		//ͷ��
	PlaneObject*	m_plPlayerName;		//����	
	PlaneObject*	m_plMask;			//��Ӯ�ɰ�

	BOOL			m_bCurrentChair;
	BOOL			m_bMask;			
	BOOL			m_bTakeDeposit;		//����
	
	CMyDigit_Move*	m_pNewDigit;		//
	CXyStandButton* m_btnCompareCard;	//�����ư�ť

	CMyGame*		m_pMainGame;
};

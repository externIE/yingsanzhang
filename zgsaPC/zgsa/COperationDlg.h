#pragma  once
using namespace  ShinyNova;
class CMyGame;

class  CMyDigit : public CXyDigit
{
public:
	void 	 FreshDigit()
	{
		CString str;
		if (IS_BIT_SET(m_dwFlags,DIGIT_FLAGS_SHOW_UNIT))
		{
			str.Format("%s%d",m_szUnit,m_nDigit);
		}
		else
		{
			str.Format("%d",m_nDigit);
		}
		BulidString(str);
	};
	
	int  GetDigitNum(){return m_nDigit;}
};


class  CClockDigit : public CXyDigit
{
public:
	void FreshDigit()
	{
		CString str;
		if (IS_BIT_SET(m_dwFlags,DIGIT_FLAGS_SHOW_UNIT))
		{
			str.Format("%s%2d",m_szUnit,m_nDigit);
		}
		else
		{
			str.Format("%02d",m_nDigit);
		}
		BulidString(str);
	}; 
};


class CMyDigit_Move : public CMyDigit
{
public: 
	void MoveTo(double x,double y)
	{
		DWORD dwRelation=0;
		if (GetFrame()!=this)
			dwRelation=CClassSenior::GetChildRelation(GetFrame()->GetHandle());
		
		if (GetFrame()!=this
			&&IS_BIT_SET(dwRelation,OBJECT_LOCK_COORDINATE))
			GetFrame()->MoveTo(x,y);
		else
			CClassSenior::MoveTo(x,y);
	}
	
};

class COperationDlg : public CShinyNovaGui
{
public:
	COperationDlg();
	virtual ~COperationDlg();
public:
	virtual void 	OnIniData(); 

	//按钮创建相关
	CXyStandButton*		FastCreateStandButton(LPCTSTR picName,SNG_MSG& msg,POINT pt,LPCTSTR szNote=NULL);
	CXyDynamicButton*	FastCreateDynamicButton(LPCTSTR picUntouch,LPCTSTR picTouch,LPCTSTR picDown,LPCTSTR picDisable,SNG_MSG& msg,POINT pt,LPCTSTR szNote=NULL);
	
	//文本创建相关
	CMyDigit*			FastCreateDigit(LPCTSTR picName, DWORD dwRelation = NULL);

	//位置自适应相关
	void			FitScreen();
	
	//当前可进行操作相关
    void            ShowPlayerCouldOpe(DWORD dwCouldOpe);
	void			ShowPlayerCouldOpeNotCurrentChair(DWORD dwCouldOpe);
    void            HidePlayerOpe(int ndwOpe);//ndwOPe:是否隐藏弃牌、看牌、自动开始、跟注
	
	//亮牌按钮相关
	void            ShowShowCardBtn();
	void            HideShowCardBtn();
	
	//按钮面板背景相关
	POINT			GetRightBottomBoardPosition();

	//下注或加注面板相关
	POINT			GetBetBoardPosition();
	void			ShowBetBoard();
	void			HideBetBoard();
	void			EnableBetBoard(int nCouldOpe);

	void			ShowNextAutoRestAndFollowBtn();
	void			ShowNextAutoRestBtn(BOOL bShowAutoStart, BOOL bShowAutoStartFocus);
	void			ShowAutoFollowBtn(BOOL bShowAutoFollow, BOOL bShowAutoFollowFocus);
	void			EnableNextAutoRestBtn(BOOL bEnableAutoStart, BOOL bEnableAutoStartFocus);
	void			EnableAutoFollowBtn(BOOL bEnableAutoFollow, BOOL bEnableAutoFollowFocus);
	void			SetFullBetDeposit(int nDeposit);
	void			SetBetDeposit(int nDeposit);
	void			HideDigitNumber();
	void			ShowGiveUpBtn(DWORD dwCouldOpe);
public:
	PlaneObject*		m_plBoard;
    PlaneObject*		m_plNotes;				//技七说明信息

	CXyStandButton*	m_btnBet;					//下注 
	CXyStandButton*	m_btnBetTwo;				//2倍注
	CXyStandButton*	m_btnBetFour;				//4倍注
	CXyStandButton*	m_btnBetEight;				//8倍注
	CXyStandButton*	m_btnAllin;					//全下
	CXyStandButton*	m_btnFollow;				//跟注
	CXyStandButton*	m_btnLookCard;				//看牌
	CXyStandButton*	m_btnGiveUp;				//弃牌
	CXyStandButton*	m_btnNextAutoRestFocus;		//下一局休息未选中状态
	CXyStandButton*	m_btnNextAutoRest;			//下一局休息选中状态
	CXyStandButton*	m_btnAutoFollowFocus;		//自动跟注选中状态
	CXyStandButton*	m_btnAutoFollow;			//自动跟注未选中状态
    CXyStandButton*	m_btnWinShow;				//亮牌
	CXyStandButton* m_btnStandBy;				//旁观
	CXyStandButton* m_btnSitDown;				//坐下
	CXyStandButton* m_btnChangeTable;			//换桌
	CXyStandButton*	m_btnBetFull;				//压满
	CXyStandButton*	m_btnAutoSetDeposit;		//自动设银按钮

	CMyDigit*		m_digitBet;					//下注、跟注的银两
	CMyDigit*		m_digitAllInBet;			//全下的银两
	CMyDigit*		m_digitFullBet;				//压满银两

    CMyGame*		m_pMainGame;
};


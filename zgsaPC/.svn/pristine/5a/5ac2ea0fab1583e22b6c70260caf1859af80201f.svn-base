#pragma once
extern CGameTable g_GameTable;
namespace ShinyNova{

//游戏的逻辑图层在0～10000之间
 
	 
#define  CARD_FLOOR_MASK			3000
#define  CARD_FLOOR_INHAND			2000
#define  CARD_FLOOR_UNUSED			500
 
	class CMyCard : public CShinyNovaGui
	{
	public:
		CMyCard();
		virtual ~CMyCard();
	public:
		virtual void		OnIniData();
		virtual void		Fresh();
		virtual int			GetCardPic(int nCardID);
        virtual  void       FitScreen();
	 
		int        m_nUnitIndex;	  //单位序号  
		int        m_nCardID; 
		int		   m_nChairNO;
		int		   m_nDrawIndex;  
		BOOL       m_bnFace;			  //是否面朝上
	 
        int        m_nStatus;  //0 正常 1 高亮 2 掩码中
		BOOL       bnTouch;           
		 
		BITObject* m_bitCard;
	};
	


}
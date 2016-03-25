#pragma once


typedef struct _tagQUANDATA{
   	int nUserIDs[MAX_QUANCHAIR_COUNT];
}QUANDATA, *LPQUANDATA;

class CQuanData{
public:
	CQuanData()
	{
//		ZeroMemory(&quandata, sizeof(quandata));
	}
	~CQuanData(){};

 //	QUANDATA                quandata;
	CIntIntMap              m_mapQuanUser;
public:
	void  AddQuanUser(int nUserID,int nTokenID);
};

typedef CMap<int, int&, CQuanData*, CQuanData*&> CQuanDataMap;


extern BOOL NotifyQuanPlayers(CBaseRoomData* lpRoomData, int nQuanID,int playeridExcept, UINT nRequest, void* pData, int nLen);

#pragma once

#include "resource.h"

#define APPLICATION_TITLE	_T("RoomSvrZgSa")
#define STR_SERVICE_NAME     _T("RoomSvrZgSa")
#define STR_DISPLAY_NAME     _T("ͬ������Ӯ���ŷ������")
#define STR_DISPLAY_NAME_ENU _T("TCY RoomSvrZgSa Service") 

extern TCHAR			g_szConnect[MAX_PATH];

extern void CopyMapClientToken(CClientTokenMap& mapClientToken);

extern int  CreateRandomIDEx(CIDArray& aryID, int nMaxNum, int seed);

DWORD ComboTableChairStatus(int nTalbeNO, int nChairStatus[], int nChairCount  );

//��16λ�����Ӻţ���16λ������״̬��1��ʾ���������ˣ����Ҳ�������Ϸ��0��ʾ������û�ˣ��������˵�û������Ϸ


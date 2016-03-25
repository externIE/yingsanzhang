#pragma once

#include "resource.h"

#define APPLICATION_TITLE	_T("RoomSvrZgSa")
#define STR_SERVICE_NAME     _T("RoomSvrZgSa")
#define STR_DISPLAY_NAME     _T("同城游新赢三张房间服务")
#define STR_DISPLAY_NAME_ENU _T("TCY RoomSvrZgSa Service") 

extern TCHAR			g_szConnect[MAX_PATH];

extern void CopyMapClientToken(CClientTokenMap& mapClientToken);

extern int  CreateRandomIDEx(CIDArray& aryID, int nMaxNum, int seed);

DWORD ComboTableChairStatus(int nTalbeNO, int nChairStatus[], int nChairCount  );

//高16位放桌子号，低16位放桌子状态：1表示椅子上有人，并且参与了游戏。0表示椅子上没人，或者有人但没参与游戏


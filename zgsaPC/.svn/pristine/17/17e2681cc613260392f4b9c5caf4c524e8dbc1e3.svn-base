#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRoomData::CRoomData()
{

}

CRoomData::~CRoomData()
{

}

void CRoomData::CopyTableMapEx(PBYTE pData, PLAYER* lpPlayer )
{
	if (!pData||!lpPlayer)
		return;
	
	int nBeginRandomTableNO=GetBeginRandomTableNO();
	
	BOOL bInterHall=IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER);
	
	int tableno = 0;
	LPTABLE pTable = NULL;
	PBYTE lpDataPtr = pData;
	POSITION pos;
	pos  = m_mapTable.GetStartPosition();
	while (pos)	{
		m_mapTable.GetNextAssoc(pos, tableno, pTable);
		if(tableno>=nBeginRandomTableNO)
			continue;

		//判断包括玩家和旁观者
		if(pTable && (pTable->th.nPlayerCount || pTable->th.nVisitorCount)){
			// !!! _tagTABLE_HEAD must be the head part of the _tagTABLE !!!
			LPTABLE_HEAD pTableHead = LPTABLE_HEAD(lpDataPtr);
			memcpy(pTableHead, pTable, sizeof(TABLE_HEAD));
			lpDataPtr += sizeof(TABLE_HEAD);
			
			int playercount = 0;
			LPPLAYER_POS pPlayerPos = LPPLAYER_POS(lpDataPtr);
			for(int i = 0; i < MAX_CHAIR_COUNT; i++){
				if(pTable->nPlayerAry[i]){
					pPlayerPos->nChairNO = i;
					pPlayerPos->nUserID = pTable->nPlayerAry[i];
					pPlayerPos++;
					playercount++;
				}
			}
			//assert(playercount == pTable->nPlayerCount);
			if(playercount != pTable->th.nPlayerCount){
				UwlTrace(_T("playercount = %ld, pTable->nPlayerCount = %ld"),
					playercount, pTable->th.nPlayerCount);
			}
			lpDataPtr += sizeof(PLAYER_POS) * pTable->th.nPlayerCount;
			
			int visitorcount = 0;
			LPVISITOR_POS pVisitorPos = LPVISITOR_POS(lpDataPtr);
			for(i = 0; i < MAX_CHAIR_COUNT; i++){
				for(int j = 0; j < MAX_VISITOR_COUNT; j++){
					if(pTable->nVisitorAry[i][j]){
						pVisitorPos->nChairNO = i;
						pVisitorPos->nUserID = pTable->nVisitorAry[i][j];
						if (!bInterHall) //外部大厅
						{
							LPPLAYER pPlayer=NULL;						
							if (m_mapPlayer.Lookup(pVisitorPos->nUserID, pPlayer)							
								&& IS_BIT_SET(pPlayer->nUserType,USER_TYPE_ENTERHIDING))  //此旁观者为隐身管理员 
							{
								pVisitorPos->nUserID=0; //大厅客户端对UserID==0的用户 不做操作
							}
						}
						pVisitorPos++;
						visitorcount++;
					}
				}
			}
			//assert(visitorcount == pTable->nVisitorCount);
			if(visitorcount != pTable->th.nVisitorCount){
				UwlTrace(_T("visitorcount = %ld, pTable->th.nVisitorCount = %ld"),
					visitorcount, pTable->th.nVisitorCount);
			}
			lpDataPtr += sizeof(VISITOR_POS) * pTable->th.nVisitorCount;
			
		}
	}
}

void CRoomData::PostChangeSeatedToLookOnResult(int nUserID,int nTableNO,int nChairNO)
{
	HWND hWnd=FindGameSvrWindow();
	if(IsWindow(hWnd))
	{
		DWORD dwRoomTableChair=ComboRoomTableChair(roomdata.nRoomID,nTableNO,nChairNO);
		PostMessage(hWnd,WM_GTR_CHANGE_SEATED_TO_LOOK_ON_RESULT,dwRoomTableChair,nUserID);
	}
}

void CRoomData::PostChangeLookOnToSeatedResult(int nUserID,int nTableNO,int nChairNO)
{
	HWND hWnd=FindGameSvrWindow();
	if(IsWindow(hWnd))
	{
		DWORD dwRoomTableChair=ComboRoomTableChair(roomdata.nRoomID,nTableNO,nChairNO);
		PostMessage(hWnd,WM_GTR_CHANGE_LOOK_ON_TO_SEATED_RESULT,dwRoomTableChair,nUserID);
	}
}	
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE		g_hThrdTimer2 = NULL;
UINT		g_uiThrdTimer2 = 0;

void SetCommonTableLeaveEx(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer)
{
	LPTABLE pOldTable = NULL;
	lpRoomData->m_mapTable.Lookup(lpPlayer->nTableNO, pOldTable);
	
	int nChairNO = lpPlayer->nChairNO;
	int nUserID = lpPlayer->nUserID;
	
	if(pOldTable && nChairNO>=0 && nChairNO < MAX_CHAIR_COUNT )
	{
		if (PLAYER_STATUS_WAITING == lpPlayer->nStatus)
		{
			if(pOldTable->nPlayerAry[nChairNO]==nUserID && (nUserID!=0))
			{
				pOldTable->nPlayerAry[nChairNO] = 0;
				if(pOldTable->th.nPlayerCount>0) 
					pOldTable->th.nPlayerCount--;
			}
		}
		else if (PLAYER_STATUS_LOOKON == lpPlayer->nStatus)
		{
			for(int i = 0; i < MAX_VISITOR_COUNT; i++){
				if(lpPlayer->nUserID == pOldTable->nVisitorAry[lpPlayer->nChairNO][i]){
					pOldTable->nVisitorAry[lpPlayer->nChairNO][i] = 0;
					if(pOldTable->th.nVisitorCount > 0)
						pOldTable->th.nVisitorCount--;

					break;
				}
			}

			PLAYER_POSITION pp;
			memset(&pp,0,sizeof(PLAYER_POSITION));
			pp.nUserID=nUserID;
			pp.nTableNO=lpPlayer->nTableNO;
			pp.nChairNO=lpPlayer->nChairNO;
			
			NotifyRoomPlayers(lpRoomData, GR_PLAYER_UNLOOKON, &pp, sizeof(PLAYER_POSITION));
		}
	}
}

CMainServer::CMainServer()
{
}

CMainServer::~CMainServer()
{

}

CBaseSockServer* CMainServer::CreateCSockServer()
{
	return new CSockServer(KEY_HALL, ENCRYPT_AES, 0);
}
CBaseSockClient* CMainServer::CreateCSockClient()
{
	return new CSockClient(KEY_HALL, ENCRYPT_AES, 0);
}
CBaseRoomServer* CMainServer::CreateCRoomServer()
{
	return new CRoomServer(KEY_HALL, ENCRYPT_AES, 0);
}

BOOL CMainServer::Initialize()
{
	BOOL bResult = CBaseServer::Initialize();
	
	if (FALSE == CreateTimer2Thread())
	{//开启定时器2线程
		UwlLogFile(_T("CreateTimer2Thread failed."));
		bResult = FALSE;
	}
	
	return bResult;
}
void CMainServer::Shutdown()
{
	ClearSeatedToLookOnPlayerMap();

	//关闭定时器2线程
	PostThreadMessage(g_uiThrdTimer2, WM_QUIT, 0, 0);
	WaitForSingleObject(g_hThrdTimer2, WAITTIME_EXIT);	
	if(g_hThrdTimer2){
		CloseHandle(g_hThrdTimer2);
		g_hThrdTimer2 = NULL;
	}
	
	CBaseServer::Shutdown();
}

CString CMainServer::GetProductName()
{
	return _T("roomsvrzgsa");
}

BOOL CMainServer::OnChangeTable(DWORD dwRoomTableChair,int nUserID)
{
 	int nRoomID =  0;
	int nOldTableNO = 0;
	int nOldChairNO = 0;
	ParseRoomTableChair(dwRoomTableChair,nRoomID,nOldTableNO,nOldChairNO);

	if(nRoomID <= 0 || nOldTableNO < 0 || nUserID <= 0 || nOldChairNO < 0 || nOldChairNO >=MAX_CHAIR_COUNT)	
		return FALSE;
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);

		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData))
			return FALSE;
	}
	
	if(lpRoomData)
	{	 
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if (!lpRoomData->IsSoloRoom() || lpRoomData->IsRandomRoom())
			return FALSE;

		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(nUserID, pPlayer)) 
		{
			//玩家如果已经不在房间里，又进入了游戏，就补发一个离开房间消息，让游戏服务端踢掉该玩家
			lpRoomData->PostLeaveRoom(nUserID);
			UwlLogFile(_T("OnChangeTable failed. Can not find user which nUserID is %ld."), nUserID);
			return FALSE;
		}

		//校验玩家的桌椅号
		if (pPlayer->nTableNO!=nOldTableNO || pPlayer->nChairNO!=nOldChairNO)
		{
			UwlLogFile(_T("OnChangeTable Failed. table,chair is changed in room(%d). userid(%d), Room table,chair is (%d,%d), gamesvr is (%d,%d)."),
				nRoomID, nUserID, pPlayer->nTableNO, pPlayer->nChairNO, nOldTableNO, nOldChairNO);
			lpRoomData->PostVerifyRoomTableChair(nUserID, pPlayer->nTableNO, pPlayer->nChairNO);
			return FALSE;
		}

		if(pPlayer->nStatus!=PLAYER_STATUS_WAITING && pPlayer->nStatus!=PLAYER_STATUS_LOOKON)
			return FALSE;

 		pPlayer->pe.dwLatestTime = GetTickCount();

		float fBreakOff = 0;
		int bout = pPlayer->nWin + pPlayer->nLoss + pPlayer->nStandOff;
		if(bout)
			fBreakOff = (float)pPlayer->nBreakOff * 100 / bout;
		int nNetSpeed = pPlayer->nNetSpeed;	

		LPTABLE lpTable = NULL;
		int nTableNo = 0;
		int nChairNo = 0;
		BOOL bFindSeat = FALSE;

		lpRoomData->m_mapTable.Lookup(nOldTableNO,lpTable);

		if (!lpTable)
			return FALSE;

		DWORD dwIPAddr = lpTable->dwIPAddrs[nOldChairNO];
		int nIpConfig = lpTable->nIPConfigs[nOldChairNO];
		int nMinScore = lpTable->nMinScores[nOldChairNO];
		int nMinDeposit = lpTable->nMinDeposits[nOldChairNO];
		int nBreakReq = lpTable->nBreakReqs[nOldChairNO];
		int nSpeedReq = lpTable->nSpeedReqs[nOldChairNO];

		if (PLAYER_STATUS_LOOKON == pPlayer->nStatus)
		{//旁观玩家的信息需要从坐下变旁观玩家队列中取出来
			LPSEATEDTOLOOKONPLAYER pSeatedToLookOnPlayer = NULL;
			if (m_mapSeatedToLookOnPlayers.Lookup(nUserID, pSeatedToLookOnPlayer))
			{
				dwIPAddr	= pPlayer->pe.dwIPAddr;
				nIpConfig	= pSeatedToLookOnPlayer->nIPConfig;
				nMinScore	= pSeatedToLookOnPlayer->nMinScore;
				nMinDeposit = pSeatedToLookOnPlayer->nMinDeposit;
				nBreakReq	= pSeatedToLookOnPlayer->nBreakReq;
				nSpeedReq	= pSeatedToLookOnPlayer->nSpeedReq;
				RemoveSeatedToLookOnPlayer(nUserID);
			}
			else
			{
				dwIPAddr	= pPlayer->pe.dwIPAddr;
				nIpConfig	= 0;
				nMinScore	= SCORE_MIN;
				nMinDeposit = 0;
				nBreakReq	= 0;
				nSpeedReq	= 0;
				UwlLogFile(_T("Can not find SeatedToLookOnPlayer who UserID is %ld."), nUserID);
			}
		}

		nTableNo = nOldTableNO;		//从当前位置开始找桌位
		bFindSeat = lpRoomData->GetNextTableChair(pPlayer, nTableNo, nChairNo, dwIPAddr, nIpConfig, nMinScore, nMinDeposit);

		if (!bFindSeat || !lpRoomData->m_mapTable.Lookup(nTableNo, lpTable))
			return FALSE;

		lpTable->dwIPAddrs[nChairNo] = dwIPAddr;
		lpTable->fBreakOffs[nChairNo] = fBreakOff;
		lpTable->nNetSpeeds[nChairNo] = nNetSpeed;
		lpTable->nIPConfigs[nChairNo] =   nIpConfig;
		lpTable->nBreakReqs[nChairNo] =   nBreakReq;
		lpTable->nSpeedReqs[nChairNo] =   nSpeedReq;
		lpTable->nMinScores[nChairNo] =   nMinScore;
		lpTable->nMinDeposits[nChairNo] = nMinDeposit;

		if(pPlayer->nStatus != PLAYER_STATUS_WALKAROUND)	// 这个用户已坐下
		{
			SetCommonTableLeaveEx(lpRoomData, pPlayer);
			SetRandomTableLeave(lpRoomData, pPlayer);
			if(pPlayer->nTableNO!=nTableNo) //先前是另外一桌
			{
				LPTABLE pOldTable = NULL;
				if(lpRoomData->m_mapTable.Lookup(pPlayer->nTableNO, pOldTable))
				{
					if(pOldTable->th.nFirstSeatedPlayer==nUserID)
					{
						pOldTable->th.nMinDeposit=0;
						pOldTable->th.nMinScore =SCORE_MIN;
					}
				}
			}
		}

		lpTable->nPlayerAry[nChairNo] = nUserID;
		lpTable->th.nPlayerCount++;
		if(lpTable->th.nPlayerCount==1) //第一个入座
			lpTable->th.nFirstSeatedPlayer=nUserID ;

		pPlayer->nStatus = PLAYER_STATUS_WAITING;
		pPlayer->nTableNO = nTableNo;
		pPlayer->nChairNO = nChairNo;

		//Add on 20130723
		//更新kubsvr玩家位置
		lpRoomData->CommitTPS_SOLO(nUserID,pPlayer->nTableNO,PS_WAITING);
		//Add end

		NTF_GET_NEWTABLE ngn;
		memset(&ngn,0,sizeof(ngn));
		ngn.pp.nUserID=nUserID;
		ngn.pp.nTableNO=nTableNo;
		ngn.pp.nChairNO=nChairNo;
		ngn.pp.nNetDelay=pPlayer->nNetSpeed;
		ngn.nMinScore =lpTable->th.nMinScore ;//
		ngn.nMinDeposit =lpTable->th.nMinDeposit ;//
		ngn.nFirstSeatedPlayer=lpTable->th.nFirstSeatedPlayer;
 
		NotifyRoomPlayers(lpRoomData, GR_PLAYER_NEWTABLE, &ngn,  sizeof(ngn));

		PLAYER_POSITION pp;
		memset(&pp,0,sizeof(PLAYER_POSITION));
		pp.nUserID=nUserID;
		pp.nTableNO=nTableNo;
		pp.nChairNO=nChairNo;

		NotifyRoomPlayers(lpRoomData, GR_PLAYER_STARTED, &pp, sizeof(pp));

		//Room table chair
		lpRoomData->PostRoomTableChair(nUserID,nTableNo,nChairNo);
		lpRoomData->PostNewTableChair(nUserID,nTableNo,nChairNo);
	}

 	return TRUE;
}

BOOL CMainServer::OnUserLeaveGameOKVerified(int nUserID,DWORD dwRoomTableChair)
{
	int nRoomID =  0;
	int nTableNO = 0;
	int nChairNO = 0;
	ParseRoomTableChair(dwRoomTableChair,nRoomID,nTableNO,nChairNO);

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
	}
	
	if(lpRoomData)
	{	 
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		if(lpRoomData->IsSoloRoom())
		{
			LPPLAYER pPlayer = NULL;
			if(!lpRoomData->m_mapPlayer.Lookup( nUserID, pPlayer)) 
				return FALSE;
			pPlayer->pe.dwLatestTime = GetTickCount();
			
			if(pPlayer->nStatus!=PLAYER_STATUS_WAITING && pPlayer->nStatus!=PLAYER_STATUS_LOOKON)
				return FALSE;

			//校验玩家的桌椅号
			if ((pPlayer->nTableNO != nTableNO) || (pPlayer->nChairNO != nChairNO))
			{
				UwlLogFile(_T("LeaveGame Failed. table,chair is changed in room(%d). userid(%d), Room table,chair is (%d,%d), gamesvr is (%d,%d)."),
					nRoomID, nUserID, pPlayer->nTableNO, pPlayer->nChairNO, nTableNO, nChairNO);
				return FALSE;
			}
			
			//pengsy
			if (lpRoomData->IsYuePaiRoom())
			{
				pPlayer->nStatus = PLAYER_STATUS_SEATED;
				lpRoomData->CommitTPS_SOLO(pPlayer->nUserID,pPlayer->nTableNO,PS_SEATED);
				
				PLAYER_POSITION pp;
				memset(&pp,0,sizeof(pp));
				pp.nUserID=pPlayer->nUserID;
				pp.nTableNO=pPlayer->nTableNO;
				pp.nChairNO=pPlayer->nChairNO;
				pp.nNetDelay=pPlayer->nNetSpeed;
				
				int nQuanID = pPlayer->pe.nQuanID;
				NotifyQuanPlayers(lpRoomData,nQuanID,pPlayer->nUserID,GR_PLAYER_LEAVEGAME,&pp,sizeof(pp));
				
				return TRUE;
			}
			
			LPTABLE pTable = NULL;
			if(!lpRoomData->m_mapTable.Lookup(pPlayer->nTableNO, pTable))
				return FALSE;
			
			SetCommonTableLeaveEx(lpRoomData, pPlayer);
			SetRandomTableLeave(lpRoomData, pPlayer);
			
			if(pTable->th.nFirstSeatedPlayer==pPlayer->nUserID) //第一个入座的人离开
			{
				pTable->th.nMinDeposit=0;
				pTable->th.nMinScore =SCORE_MIN;
			}
			OverSeeUserData(pPlayer->nUserID,"OnUserLeaveGameOKVerified2::CommitTPS_SOLO");
			lpRoomData->CommitTPS_SOLO(pPlayer->nUserID,pPlayer->nTableNO,PS_WALKAROUND);
			
			PLAYER_POSITION pp;
			memset(&pp,0,sizeof(pp));
			pp.nUserID=pPlayer->nUserID;
			pp.nTableNO=pPlayer->nTableNO;
			pp.nChairNO=pPlayer->nChairNO;
			pp.nNetDelay=pPlayer->nNetSpeed;
			
			if(lpRoomData->IsCloakingRoom())
			{	
				NotifyOnePlayer(lpRoomData, pp.nUserID, GR_PLAYER_LEAVETABLE, &pp, sizeof(pp));
				NotifyDarkLooker(lpRoomData,pp.nUserID, GR_PLAYER_LEAVETABLE, &pp, sizeof(pp));
			}
			else if (pPlayer->nStatus==PLAYER_STATUS_WAITING || pPlayer->nStatus==PLAYER_STATUS_LOOKON)
			{
				NotifyRoomPlayers(lpRoomData, GR_PLAYER_LEAVETABLE, &pp, sizeof(pp)); 
			}
			
			pPlayer->nStatus = PLAYER_STATUS_WALKAROUND;
			pPlayer->nTableNO = -1;
			pPlayer->nChairNO = -1;
			
			if(IS_BIT_SET(pPlayer->nUserType,USER_TYPE_HANDPHONE))//手机用户离开游戏时，同时也会离开房间
			{
				//判断房间连接是否断开
				SOCKET sock = 0;
				LONG token = 0;
				if(!GetSocketTokenIDByUserID(lpRoomData, pPlayer->nUserID, sock, token))//没找到用户连接
				{
					RemoveOnePlayer(lpRoomData,pPlayer,0,0);
				}
				
			}

			//将旁观变坐下的玩家的信息从队列中删除
			RemoveSeatedToLookOnPlayer(nUserID);
		}
		
	}
	return TRUE;
}

void CMainServer::OnChangeSeatedToLookOnFromGameSvr(DWORD dwRoomTableChair,int nUserID)
{
	int nRoomID =  0;
	int nTableNO = 0;
	int nChairNO = 0;
	
	ParseRoomTableChair(dwRoomTableChair,nRoomID,nTableNO,nChairNO);
	
	UINT nMsgID=WM_RTG_STARTINVALID;
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
	}
	
	if(lpRoomData)
	{	 
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if(nChairNO<0 || nChairNO >=lpRoomData->GetChairCount())
		{
			UwlLogFile(_T("OnChangeSeatedToLookOnFromGameSvr failed. nChairNO(%ld) is wrong."), nChairNO);
			return;
		}
		
		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(nTableNO, pTable)) 
		{
			UwlLogFile(_T("OnChangeSeatedToLookOnFromGameSvr failed. nTableNO(%ld) is wrong."), nTableNO);
			return;
		}
		
		if(pTable->nPlayerAry[nChairNO]!=nUserID)
		{
			UwlLogFile(_T("OnChangeSeatedToLookOnFromGameSvr failed. nUserID(%ld) mismatch."), pTable->nPlayerAry[nChairNO]);
			return;
		}
		
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(nUserID, pPlayer)) 
		{
			//玩家如果已经不在房间里，又进入了游戏，就补发一个离开房间消息，让游戏服务端踢掉该玩家
			lpRoomData->PostLeaveRoom(nUserID);
			UwlLogFile(_T("OnChangeSeatedToLookOnFromGameSvr failed. Can not find user which nUserID is %ld."), nUserID);
			return;
		}

		//校验玩家的桌椅号
		if (pPlayer->nTableNO!=nTableNO || pPlayer->nChairNO!=nChairNO)
		{
			UwlLogFile(_T("OnChangeSeatedToLookOnFromGameSvr Failed. table,chair is changed in room(%d). userid(%d), Room table,chair is (%d,%d), gamesvr is (%d,%d)."),
				nRoomID, nUserID, pPlayer->nTableNO, pPlayer->nChairNO, nTableNO, nChairNO);
			lpRoomData->PostVerifyRoomTableChair(nUserID, pPlayer->nTableNO, pPlayer->nChairNO);
			return;
		}

		if(pPlayer->nStatus == PLAYER_STATUS_LOOKON){	//这个用户已经是旁观状态了
			UwlLogFile(_T("OnChangeSeatedToLookOnFromGameSvr failed. User(%ld) is already look on."), nUserID);
			return;
		}

		if(pPlayer->nStatus == PLAYER_STATUS_PLAYING){	// 这个用户已进入游戏
			UwlLogFile(_T("OnChangeSeatedToLookOnFromGameSvr failed. User(%ld) is playing, can not change status."), nUserID);
			return;
		}
		
		//将坐下变成旁观的玩家的一些基本信息存起来
		InsertToSeatedToLookOnPlayerMap(nUserID, nRoomID, nTableNO, nChairNO, pTable->nIPConfigs[nChairNO], pTable->nBreakReqs[nChairNO],
			pTable->nSpeedReqs[nChairNO], pTable->nMinScores[nChairNO], pTable->nMinDeposits[nChairNO]);

		pPlayer->pe.dwLatestTime = GetTickCount();
		
		if(pPlayer->nStatus != PLAYER_STATUS_WALKAROUND)	// 这个用户已坐下
		{
			SetCommonTableLeave(lpRoomData, pPlayer);
			SetRandomTableLeave(lpRoomData, pPlayer);	
		}
		
		for(int i = 0; i < MAX_VISITOR_COUNT; i++){
			if(0 == pTable->nVisitorAry[nChairNO][i]){
				pTable->nVisitorAry[nChairNO][i] = nUserID;
				pTable->th.nVisitorCount++;
				break;
			}
		}

		pPlayer->nStatus = PLAYER_STATUS_WALKAROUND;
		pPlayer->nTableNO = -1;
		pPlayer->nChairNO = -1;

		lpRoomData->CommitTPS_SOLO(pPlayer->nUserID,pPlayer->nTableNO,PS_WALKAROUND);
		
		PLAYER_POSITION pp;
		memset(&pp,0,sizeof(PLAYER_POSITION));
		pp.nUserID=nUserID;
		pp.nTableNO=nTableNO;
		pp.nChairNO=nChairNO;

		NotifyRoomPlayers(lpRoomData, GR_PLAYER_LEAVETABLE, &pp, sizeof(PLAYER_POSITION));
		
		pPlayer->nStatus = PLAYER_STATUS_LOOKON;
		pPlayer->nTableNO = nTableNO;
		pPlayer->nChairNO = nChairNO;

 		NotifyRoomPlayers(lpRoomData, GR_PLAYER_LOOKON, &pp, sizeof(pp));

		lpRoomData->PostExperience(pPlayer->nUserID,pPlayer->nTimeCost / 60);
		((CRoomData*)lpRoomData)->PostChangeSeatedToLookOnResult(nUserID, nTableNO, nChairNO);
	}
}

void CMainServer::OnChangeLookOnToSeatedFromGameSvr(DWORD dwRoomTableChair,int nUserID)
{
	int nRoomID =  0;
	int nTableNO = 0;
	int nChairNO = 0;
	
	ParseRoomTableChair(dwRoomTableChair,nRoomID,nTableNO,nChairNO);
	
	UINT nMsgID=WM_RTG_STARTINVALID;
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
	}
	
	if(lpRoomData)
	{	 
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if(nChairNO<0 || nChairNO >=lpRoomData->GetChairCount())
		{
			UwlLogFile(_T("OnChangeLookOnToSeatedFromGameSvr failed. nChairNO(%ld) is wrong."), nChairNO);
			return;
		}
		
		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(nTableNO, pTable)) 
		{
			UwlLogFile(_T("OnChangeLookOnToSeatedFromGameSvr failed. nTableNO(%ld) is wrong."), nTableNO);
			return;
		}
		
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup(nUserID, pPlayer)) 
		{
			//玩家如果已经不在房间里，又进入了游戏，就补发一个离开房间消息，让游戏服务端踢掉该玩家
			lpRoomData->PostLeaveRoom(nUserID);
			UwlLogFile(_T("OnChangeLookOnToSeatedFromGameSvr failed. Can not find user which nUserID is %ld."), nUserID);
			return;
		}
		
		//校验玩家的桌椅号
		if (pPlayer->nTableNO!=nTableNO || pPlayer->nChairNO!=nChairNO)
		{
			UwlLogFile(_T("OnChangeLookOnToSeatedFromGameSvr Failed. table,chair is changed in room(%d). userid(%d), Room table,chair is (%d,%d), gamesvr is (%d,%d)."),
				nRoomID, nUserID, pPlayer->nTableNO, pPlayer->nChairNO, nTableNO, nChairNO);
			lpRoomData->PostVerifyRoomTableChair(nUserID, pPlayer->nTableNO, pPlayer->nChairNO);
			return;
		}

		if(pPlayer->nStatus != PLAYER_STATUS_LOOKON){	// 
			UwlLogFile(_T("OnChangeLookOnToSeatedFromGameSvr failed. nStatus is %ld."), pPlayer->nStatus);
			return;
		}

		if (pTable->nPlayerAry[nChairNO])
		{
			int nOldChairNo = nChairNO;
			for (int i=0; i<lpRoomData->GetChairCount(); i++)
			{
				if (0 == pTable->nPlayerAry[i])
				{
					nChairNO = i;
					break;
				}
			}

			if (nOldChairNo == nChairNO)
			{
				((CRoomData*)lpRoomData)->PostChangeLookOnToSeatedResult(nUserID, nTableNO, INVALID_CHAIR_NO);
				UwlLogFile(_T("OnChangeLookOnToSeatedFromGameSvr failed. No empty chair. RoomID:%ld, TableNO:%ld, UserID:%ld"), nRoomID, nTableNO, nUserID);
				return;
			}
		}

		//将旁观变坐下的玩家的信息从队列中删除
		RemoveSeatedToLookOnPlayer(nUserID);

		pPlayer->pe.dwLatestTime = GetTickCount();

		PLAYER_POSITION pp;
		memset(&pp,0,sizeof(PLAYER_POSITION));
		pp.nUserID=nUserID;
		pp.nTableNO=nTableNO;
		pp.nChairNO=pPlayer->nChairNO;

		SetTableAsLookonLeave(lpRoomData, pPlayer);
		NotifyRoomPlayers(lpRoomData, GR_PLAYER_UNLOOKON, &pp, sizeof(PLAYER_POSITION));

		pTable->nPlayerAry[nChairNO] = nUserID;
		pTable->th.nPlayerCount++;
		
		if(pTable->th.nPlayerCount==1) //第一个入座
			pTable->th.nFirstSeatedPlayer=nUserID ;
		
		pPlayer->nStatus = PLAYER_STATUS_WAITING;
		pPlayer->nTableNO = nTableNO;
		pPlayer->nChairNO = nChairNO;

		lpRoomData->CommitTPS_SOLO(pPlayer->nUserID,pPlayer->nTableNO,PS_WAITING);

		NTF_GET_SEATED ngs;
		memset(&ngs,0,sizeof(NTF_GET_SEATED));
		ngs.pp.nUserID=nUserID;
		ngs.pp.nTableNO=nTableNO;
		ngs.pp.nChairNO=nChairNO;
		ngs.nMinScore =pTable->th.nMinScore ;//
		ngs.nMinDeposit =pTable->th.nMinDeposit ;//
		ngs.nFirstSeatedPlayer=pTable->th.nFirstSeatedPlayer;

		NotifyRoomPlayers(lpRoomData, GR_PLAYER_SEATED, &ngs,  sizeof(NTF_GET_SEATED)-MAX_PASSWORD_LEN+lstrlen(ngs.szPassword)+1);
		NotifyRoomPlayers(lpRoomData, GR_PLAYER_STARTED, &pp, sizeof(pp));

		lpRoomData->PostRoomTableChair(nUserID,nTableNO,nChairNO);
		((CRoomData*)lpRoomData)->PostChangeLookOnToSeatedResult(nUserID, nTableNO, nChairNO);
	}
}

BOOL CMainServer::InsertToSeatedToLookOnPlayerMap(int nUserID, int nRoomID, int nTableNo, int nChairNo, int nIPConfig, int nBreakReq, int nSpeedReq, int nMinScore, int nMinDeposit)
{
	if (nUserID<=0 || nRoomID<=0 || nTableNo<0 || nChairNo<0)
	{
		UwlLogFile(_T("InsertToSeatedToLookOnPlayerMap failed. UserID(%d) or RoomID(%d) or TableNo(%ld) or ChairNo(%ld) is wrong."), nUserID, nRoomID, nTableNo, nChairNo);
		return FALSE;
	}
	
	CAutoLock lock(&m_csSeatedToLookOnPlayers);
	LPSEATEDTOLOOKONPLAYER pSeatedToLookOnPlayer = NULL;
	if (FALSE == m_mapSeatedToLookOnPlayers.Lookup(nUserID, pSeatedToLookOnPlayer))
	{
		pSeatedToLookOnPlayer = new SEATEDTOLOOKONPLAYER;
		memset(pSeatedToLookOnPlayer, 0, sizeof(SEATEDTOLOOKONPLAYER));
	}
	
	pSeatedToLookOnPlayer->nUserID = nUserID;
	pSeatedToLookOnPlayer->nRoomID = nRoomID;
	pSeatedToLookOnPlayer->nTableNo = nTableNo;
	pSeatedToLookOnPlayer->nChairNo = nChairNo;
	pSeatedToLookOnPlayer->nIPConfig = nIPConfig;
	pSeatedToLookOnPlayer->nBreakReq = nBreakReq;
	pSeatedToLookOnPlayer->nSpeedReq = nSpeedReq;
	pSeatedToLookOnPlayer->nMinScore = nMinScore;
	pSeatedToLookOnPlayer->nMinDeposit = nMinDeposit;
	pSeatedToLookOnPlayer->nStartLookOnTime = GetTickCount();
	
	m_mapSeatedToLookOnPlayers.SetAt(nUserID, pSeatedToLookOnPlayer);
	
	return TRUE;
}

void CMainServer::DestorySeatedToLookOnPlayer(LPSEATEDTOLOOKONPLAYER pSeatedToLookOnPlayer)
{
	SAFE_DELETE(pSeatedToLookOnPlayer);
}

void CMainServer::RemoveSeatedToLookOnPlayer(int nUserID)
{
	CAutoLock lock(&m_csSeatedToLookOnPlayers);
	LPSEATEDTOLOOKONPLAYER pSeatedToLookOnPlayer = NULL;
	if (m_mapSeatedToLookOnPlayers.Lookup(nUserID, pSeatedToLookOnPlayer))
	{
		m_mapSeatedToLookOnPlayers.RemoveKey(nUserID);
		DestorySeatedToLookOnPlayer(pSeatedToLookOnPlayer);
	}
}

void CMainServer::RemoveTimeOutSeatedToLookOnPlayers()
{
	CAutoLock lock(&m_csSeatedToLookOnPlayers);
	POSITION pos = m_mapSeatedToLookOnPlayers.GetStartPosition();
	while (pos != NULL)
	{
		int nUserID = 0; 
		int nCurrentTime = GetTickCount();
		LPSEATEDTOLOOKONPLAYER pSeatedToLookOnPlayer = NULL;
		m_mapSeatedToLookOnPlayers.GetNextAssoc(pos, nUserID, pSeatedToLookOnPlayer); 
		if (NULL!=pSeatedToLookOnPlayer && nCurrentTime-pSeatedToLookOnPlayer->nStartLookOnTime>=MAX_SEATED_TO_LOOKON_TIME)
		{
			UwlLogFile(_T("RemoveTimeOutSeatedToLookOnPlayers executed. Time:%ld minutes."), (nCurrentTime-pSeatedToLookOnPlayer->nStartLookOnTime)/60000);
			m_mapSeatedToLookOnPlayers.RemoveKey(nUserID);
			DestorySeatedToLookOnPlayer(pSeatedToLookOnPlayer);
		}
	}
}

void CMainServer::ClearSeatedToLookOnPlayerMap()
{
	CAutoLock lock(&m_csSeatedToLookOnPlayers);
	POSITION pos = m_mapSeatedToLookOnPlayers.GetStartPosition();
	while (pos != NULL)
	{
		int nUserID = 0; 
		LPSEATEDTOLOOKONPLAYER pSeatedToLookOnPlayer = NULL;
		m_mapSeatedToLookOnPlayers.GetNextAssoc(pos, nUserID, pSeatedToLookOnPlayer); 
		DestorySeatedToLookOnPlayer(pSeatedToLookOnPlayer);
	}
	m_mapSeatedToLookOnPlayers.RemoveAll();
}

unsigned CMainServer::MessageThreadProc()
{
	UwlTrace(_T("message thread started. id = %d"), GetCurrentThreadId());
  	RegMessageWnd(g_nRoomPort);
    g_hMessageWnd = CreateMessageWnd(g_nRoomPort);

 	WritePrivateProfileString(_T("listen"),	 _T("hwnd"), xyConvertIntToStr((int)g_hMessageWnd),g_szIniFile	);
  	
	if(!g_hMessageWnd){
		UwlLogFile(_T("can not create message windows. we exit."));
		return 0;
	}

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)){
		if(g_hMessageWnd != msg.hwnd) continue; //
		if(UM_SOCKET_NOBUFFER == msg.message)
		{
			InterlockedIncrement(&g_lNoBufferCount);
			InterlockedExchange(&g_lNoBufferLatest, UwlGetCurrentSeconds());
		}
		else if(WM_GTR_ENTERGAMEOK == msg.message)
		{
			OnUserEnterGameOK(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_ENTERGAMEOK_VERIFIED == msg.message)
		{
			OnUserEnterGameOKVerified(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_LEAVEGAMEOK == msg.message)
		{
			OnUserLeaveGameOK(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_LEAVEGAMEOK_VERIFIED == msg.message)
		{
			OnUserLeaveGameOKVerified(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_VERIFYSTART == msg.message)
		{
			OnUserVerifyStart(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_GAMESTARTUP == msg.message)
		{
			OnGameStartUp(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_GAMEBOUTEND == msg.message)
		{
			OnGameBoutEnd(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_CLOSESOLOTABLE == msg.message)
		{
			OnCloseSoloTable(msg.wParam ,msg.lParam);
		}
	 	else if(WM_GTR_ASKNEWTABLE == msg.message)
		{
			OnAskNewTable(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_USERBOUTEND == msg.message)
		{
			OnUserBoutEnd(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_FINDVPN == msg.message)
		{
 			OnFindVPN(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_REMOVEVPN == msg.message)
		{
			OnRemoveVPN(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_CHANGETABLE == msg.message)
		{
			OnChangeTable(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_GAMESVRPULSE == msg.message)
		{
			OnGameSvrPulse(msg.wParam ,(HWND)msg.lParam);
		}
		else if(WM_GTR_USERMATCHSCORE == msg.message)
		{
			OnUserMatchScore(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_WELFAREDEPOSIT == msg.message)
		{
			OnUserWelfareDeposit(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_ADD_SOCRE == msg.message)
		{
			OnUserAddScore(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_MINUS_SOCRE == msg.message)
		{
			OnUserMinusScore(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_CHANGE_SPECIFYTABLE == msg.message)
		{
			OnChangeSpecifyTable(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_PLAYING_CHANGECHAIR == msg.message)
		{
			OnPlayingChangeChair(msg.wParam,msg.lParam);
		}
		else if(WM_GTR_ADD_DEPOSIT == msg.message)
		{
			OnUserAddDeposit(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_MINUS_DEPOSIT == msg.message)
		{
			OnUserMinusDeposit(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_CHANGE_SEATED_TO_LOOK_ON == msg.message)
		{
			OnChangeSeatedToLookOnFromGameSvr(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_CHANGE_LOOK_ON_TO_SEATED == msg.message)
		{
			OnChangeLookOnToSeatedFromGameSvr(msg.wParam ,msg.lParam);
		}
		else
		{
			if(!OnCustomWndMsg(msg.message,msg.wParam ,msg.lParam))
			{
				DispatchMessage(&msg);
			}
		}
	}
	if(g_hMessageWnd){
		DestroyWindow(g_hMessageWnd);
		g_hMessageWnd = NULL;
	}
	UnregMessageWnd(g_nRoomPort);

	UwlTrace(_T("message thread exiting. id = %d"), GetCurrentThreadId());
	UwlLogFile(_T("message thread exiting. id = %d"), GetCurrentThreadId());
   	
	return 0;
} 

unsigned CMainServer::Timer2ThreadFunc(LPVOID lpVoid)
{
	CMainServer * pThread = (CMainServer *) lpVoid;
	
    return pThread->Timer2ThreadProc();
}

BOOL CMainServer::CreateTimer2Thread()
{
	g_hThrdTimer2 = (HANDLE)_beginthreadex(NULL,			// Security
		0,						// Stack size - use default
		Timer2ThreadFunc,		// Thread fn entry point
		(void*) this,			// Param for thread
		0,						// Init flag
		&g_uiThrdTimer2);		// Thread address;
	return TRUE;
}

unsigned CMainServer::Timer2ThreadProc()
{
	UwlTrace(_T("Timer2 thread started. id = %d"), GetCurrentThreadId());

	int lastHour = -1;
	
	while(TRUE){
		int interval = GetPrivateProfileInt(_T("Timer"), _T("Interval"), 15, g_szIniFile);
		
		DWORD dwRet = WaitForSingleObject(g_hExitServer, interval * 60 * 1000);
		if(WAIT_OBJECT_0 == dwRet){ // exit event
			break;
		}
		if(WAIT_TIMEOUT == dwRet){ // timeout
			UwlTrace(_T("Timer2 thread triggered. do something. interval = %ld minutes."), interval);
			
			{
				SYSTEMTIME time;
				GetLocalTime(&time);
				
				if (lastHour < 0 
					|| time.wHour != lastHour)
				{
					//每小时记录一次..
					//添加在这里..
					RemoveTimeOutSeatedToLookOnPlayers();
				}
				
				lastHour = time.wHour;
			}
		}
	}
	UwlTrace(_T("Timer2 thread exiting. id = %d"), GetCurrentThreadId());
	UwlLogFile(_T("Timer2 thread exiting. id = %d"), GetCurrentThreadId());
	
   	return 0;
}

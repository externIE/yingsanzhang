#include "roombasic.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 
CBaseRoomData* CBaseSockClient::CreateRoomData()
{
	return new CBaseRoomData;
}
BOOL CBaseSockClient::OnRequest(void* lpParam1, void* lpParam2)
{
	LPCONTEXT_HEAD pContext = LPCONTEXT_HEAD(lpParam1);
	LPREQUEST pRequest = LPREQUEST(lpParam2);
	
	// begin socket close msg received. we do reconnect
	if(UR_SOCKET_CLOSE == pRequest->head.nRequest){

		UwlTrace(_T("Disconnected from check server."));
		UwlLogFile(_T("Disconnected from check server."));

		PostThreadMessage(g_uiThrdConnect, UM_DATA_TOSEND, UR_SOCKET_CLOSE, 0);
		return TRUE;
	}
	// end
	if(0 == pRequest->head.nRepeated)
	{
		if(pRequest->head.nValue)// notify to player
		{ 
			OnNotifyPlayer(pRequest);

		}
		else
		{
			OnNotifyClient(pRequest);
		}
	}else{
		CONTEXT_HEAD context;
		memcpy(&context, pRequest->pDataPtr, sizeof(context));
		context.bNeedEcho = FALSE;

		REQUEST request;
		memset(&request, 0, sizeof(request));
		request.head.nRequest = pRequest->head.nRequest;
		
		OnResponse(&context, &request, pRequest);
	
		UwlClearRequest(&request);
	}
	UwlClearRequest(pRequest);
	return TRUE;
}
BOOL CBaseSockClient::OnNotifyPlayer(LPREQUEST lpRequest)
{
	int playerid = lpRequest->head.nValue;
	int roomid=lpRequest->head.wParam;

 
	return NotifyRoomPlayer(roomid,playerid, lpRequest);
}

BOOL CBaseSockClient::OnNotifyClient(LPREQUEST lpRequest)
{
	switch(lpRequest->head.nRequest){
	case GR_SEND_PULSE:
		OnSendPulse(lpRequest);
		break;
 	case GR_REFRESH_RESULT:
		OnRefreshResult(lpRequest);
		break;
	case GR_ROOMED_KICKOFF:
		OnRoomedKickoff(lpRequest);
		break;
	case GR_TRANSFER_DEPOSIT:
		OnTransferDeposit(lpRequest);
		break;
	case GR_MOVE_SAFE_DEPOSIT:
		OnMoveSafeDeposit(lpRequest);
		break;
	case GR_TAKE_BACKDEPOSIT:
		OnTakeBackDeposit(lpRequest);
		break;
	case GR_SAVE_BACKDEPOSIT:
		OnSaveBackDeposit(lpRequest);
		break;
	case GR_CLEAR_NEGA_SCORE:
		OnClearNegaScore(lpRequest);
		break;
	case GR_JUMP_LEVEL_SCORE:
		OnJumpLevelScore(lpRequest);
		break;
	case GR_CLIENT_FORBID:
		OnClientForbid(lpRequest);
		break;
 	case GR_VALIDATE_ASK:
		OnValidateAsk(lpRequest);
		break;
	case GR_SEARCH_PLAYER_EX:
		OnSearchPlayerEx(lpRequest);
		break;
	case GR_ADD_MUTEX_HARD:
		OnAddMutexHard(lpRequest);
		break;
	case GR_DEL_MUTEX_HARD:
		OnDelMutexHard(lpRequest);
		break;
	case GR_FORBID_CHAT:
		OnForbidChat(lpRequest);
		break;
	case GR_USER_LEVELUP:
		OnUserLevelUp(lpRequest);
		break;
	case GR_MEMBER_LEVELUP:
		OnMemberLevelUp(lpRequest);
		break;
	case GR_MEMBER_UPDATE:
		OnMemberUpdate(lpRequest);
		break;

	case IR_UPDATE_ROOM_LEVEL:
		OnUpdateRoomLevel(lpRequest);
		break;
	case IR_UPDATE_ROOM_OCMG:
		OnUpdateRoomOCMG(lpRequest);
		break;
	case IR_UPDATE_ROOM_INTFIELD:
		OnUpdateRoomIntField(lpRequest);
		break;
	case IR_UPDATE_ROOM_CHARFIELD:
		OnUpdateRoomCharField(lpRequest);
		break;
	case IR_UPDATE_ROOM_SAMEIP:
		OnUpdateRoomSameIP(lpRequest);
		break;
	case IR_UPDATE_ROOM:
		OnUpdateRoom(lpRequest);
		break;
	case IR_UPDATE_ROOM_FIELDS:
		OnUpdateRoomFields(lpRequest);
		break;
	case IR_WRITE_ROOMSVR_PROFILE:
		OnWriteRoomSvrProfile(lpRequest);
		break;
	case IR_WRITE_IPMAN_PROFILE:
		OnWriteIPManProfile(lpRequest);
		break;
	case IR_REFRESH_FILTER_WORD:
		OnRefreshFilterWord(lpRequest);
		break;
	case IR_REFRESH_MUTEX_HARD:
		OnRefreshMutexHard(lpRequest);
		break;
	case IR_ADD_FILTER_WORD:
		OnAddFilterWord(lpRequest);
		break;
	case IR_DELETE_FILTER_WORD:
		OnDeleteFilterWord(lpRequest);
		break;

	case GR_KICKOFF_HALL_PLAYER:
		OnKickoffHallPlayer(lpRequest);
 		break;
	case GR_MATCH_START:
		{
			LPMATCH_STARTTOROOM lpms = LPMATCH_STARTTOROOM(lpRequest->pDataPtr);
			AT_OnMatchStart(lpms);
		}
		break;
	case GR_MATCH_GAIN:
		AT_OnMatchGain(lpRequest);
		break;
	case GR_MATCH_CANCELED:
		AT_OnMatchCanceled(lpRequest);
		break;
	case GR_MATCH_SIGNUPED:
		AT_OnMatchSignUped(lpRequest);
		break;
	case GR_MATCH_SIGNUPOK:
		AT_OnMatchSignUpOK(lpRequest);
		break;
	case GR_MATCH_GIVEUPOK:
		AT_OnMatchGiveUpOK(lpRequest);
		break;
	case GR_MATCH_SIGNUPFAIL:
		AT_OnMatchSignUpFail((LPMATCH_SIGNUPFAIL)lpRequest->pDataPtr);
		break;
	case GR_MATCH_GIVEUPFAIL:
		AT_OnMatchGiveUpFail((LPMATCH_GIVEUPFAIL)lpRequest->pDataPtr);
		break;
	case GR_STAT_GAMEUSERS:
		OnStatGameUsers(lpRequest);
 		break;
	case GR_STAT_GROUPUSERS:
		OnStatGroupUsers(lpRequest);
		break;
	case GR_STAT_GROUPUSERS_ONGAME:
		OnStatGroupUsersOnGame(lpRequest);
		break;
	case GR_STAT_IPUSERS:
		OnStatIPUsers(lpRequest);
		break;
	case GR_STAT_IPUSERS_ONGAME:
		OnStatIPUsersOnGame(lpRequest);
		break;
	case IR_WRITE_KICKMAN_PROFILE:
		OnWriteKickManProfile(lpRequest);
		break;
	case GR_USER_PAY_RESULT:
		OnUserPayResult(lpRequest);
		break;
	case GR_USER_CURRENCY_EXCHANGE:
		OnUserCurrencyExchange(lpRequest);
		break;
	default://
		break;
	}
	return TRUE;
}

BOOL CBaseSockClient::OnClientForbid(LPREQUEST lpRequest)
{
	UwlLogFile(_T("client is to be forbidded."));

	InterlockedExchange(&g_lClientForbid, TRUE);

	return TRUE;
}
 

BOOL CBaseSockClient::OnRoomedKickoff(LPREQUEST lpRequest)
{
	LPROOMED_KICKOFF lpRoomedKickoff = LPROOMED_KICKOFF(lpRequest->pDataPtr);

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(lpRoomedKickoff->nRoomID, lpRoomData)){
			return FALSE;
		}
	}
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 
		CONTEXT_HEAD context;
		ZeroMemory(&context ,sizeof(context));
		context.dwFlags|=CH_FLAG_SYSTEM_EJECT;		
 		KickOffRoomPlayer(&context, lpRoomData,lpRoomedKickoff->nUserID,TRUE);
	}
	return TRUE;
}
BOOL CBaseSockClient::OnRefreshResult(LPREQUEST lpRequest)
{
	LPREFRESH_RESULT lpRefreshResult = LPREFRESH_RESULT(lpRequest->pDataPtr);
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpRefreshResult->nRoomID, lpRoomData)){
			return TRUE;
		}
	}
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		NTF_REFRESH_RESULT   nrr;
		memset(&nrr,0,sizeof(nrr));
		nrr.nCount=lpRefreshResult->nResultCount;
 
		for(int i = 0; i < lpRefreshResult->nResultCount; i++)
		{
			LPGAME_RESULT lpGameResult = LPGAME_RESULT((PBYTE)lpRefreshResult + sizeof(REFRESH_RESULT) + i * sizeof(GAME_RESULT));
		
			nrr.nr[i].nUserID=lpGameResult->nUserID;
		
			nrr.nr[i].nDeposit=lpGameResult->nDeposit;
			nrr.nr[i].nScore=lpGameResult->nScore;
			nrr.nr[i].nExperience=lpGameResult->nExperience;
			nrr.nr[i].nBreakOff=lpGameResult->nBreakOff;
			nrr.nr[i].nWin=lpGameResult->nWin;
			nrr.nr[i].nLoss=lpGameResult->nLoss;
			nrr.nr[i].nStandOff=lpGameResult->nStandOff;
			nrr.nr[i].nBout=lpGameResult->nBout;
			nrr.nr[i].nTimeCost=lpGameResult->nTimeCost;
			nrr.nr[i].nPlayerLevel=lpGameResult->nPlayerLevel;

 			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpGameResult->nUserID, lpPlayer) && lpPlayer)
			{
				if(lpRoomData->GetGameID() == lpGameResult->nGameID)
				{
					lpPlayer->nPlayerLevel = lpGameResult->nPlayerLevel;
					lpPlayer->nBout += lpGameResult->nBout;
					lpPlayer->nBreakOff += lpGameResult->nBreakOff;
					lpPlayer->nDeposit += lpGameResult->nDeposit;
 					lpPlayer->nLoss += lpGameResult->nLoss;
					lpPlayer->nScore += lpGameResult->nScore;
					lpPlayer->nStandOff += lpGameResult->nStandOff;
					lpPlayer->nTimeCost += lpGameResult->nTimeCost;
					lpPlayer->nWin += lpGameResult->nWin;
				}
			}
		}
 
		if( lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom() )
		{
			NotifyDarkLooker(lpRoomData, 0,GR_PLAYER_RESULT, &nrr,  sizeof(int)+ nrr.nCount * sizeof(NTF_RESULT));

 			int nCount=nrr.nCount;
			nrr.nCount=1;
			for(int i=0;i<nCount;i++)
			{	
				memcpy(&nrr.nr[0],&nrr.nr[i],sizeof(NTF_RESULT));
   				NotifyOnePlayer(lpRoomData,nrr.nr[i].nUserID,GR_PLAYER_RESULT, &nrr, sizeof(int)+  sizeof(NTF_RESULT));
			}
		}
		else 
			NotifyRoomPlayers(lpRoomData, GR_PLAYER_RESULT, &nrr,  sizeof(int)+ nrr.nCount * sizeof(NTF_RESULT));
	}
	return TRUE;
}
 
 

BOOL CBaseSockClient::OnValidateAsk(LPREQUEST lpRequest)
{
	ValidateClientInfo(g_nClientID);
	return TRUE;
}
 

BOOL CBaseSockClient::OnSearchPlayerEx(LPREQUEST lpRequest)
{
	LPSEARCH_PLAYER_EX lpSearchPlayerEx = LPSEARCH_PLAYER_EX(lpRequest->pDataPtr);

	int roomid = 0;
	CBaseRoomData* lpRoomData = NULL;
	CRoomDataExMap mapRoomDataEx;
	{ // 检查每个房间
		CAutoLock lock(&g_csRoomData);
		POSITION pos = g_mapRoomDataEx.GetStartPosition();
		while (pos)	{
			g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			mapRoomDataEx.SetAt(roomid, lpRoomData);
		}
	}
	if(IS_BIT_SET(lpSearchPlayerEx->dwSearchFlag,SEARCH_PLAYER_BYUSERID))
	{
		POSITION pos = mapRoomDataEx.GetStartPosition();
		while (pos)	{
			mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			if(lpRoomData){
				CAutoLock lock(&(lpRoomData->m_csRoomData));
				LPPLAYER lpPlayer = NULL;
				if(lpRoomData->m_mapPlayer.Lookup(lpSearchPlayerEx->nPlayer, lpPlayer) && lpPlayer){
					lpRoomData->TransmitPlayerInRoomEx(lpPlayer,lpSearchPlayerEx);
				}
			}
		}
	}
	if(IS_BIT_SET(lpSearchPlayerEx->dwSearchFlag,SEARCH_PLAYER_BYIP))
	{
		TCHAR szText[128];
 		POSITION pos = mapRoomDataEx.GetStartPosition();
		while (pos)	
		{
			mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			if(lpRoomData)
			{
				CAutoLock lock(&(lpRoomData->m_csRoomData));
				int userid = 0;
				LPPLAYER lpPlayer = NULL;
				POSITION posPlayer = lpRoomData->m_mapPlayer.GetStartPosition();
				while(posPlayer)
				{
					lpRoomData->m_mapPlayer.GetNextAssoc(posPlayer, userid, lpPlayer);
					if(lpPlayer==NULL)
						continue;

					if(IS_BIT_SET(lpSearchPlayerEx->dwSearchFlag,SEARCH_PLAYER_IPMATCH))
					{
						ZeroMemory(szText,sizeof(szText));
						UwlAddrToName(lpPlayer->pe.dwIPAddr,szText);
 	                 	lstrcpyn(szText,szText,lstrlen(lpSearchPlayerEx->szMachineID)+1);
						if(!lstrcmp(lpSearchPlayerEx->szMachineID,szText))
						{
							lpRoomData->TransmitPlayerInRoomEx(lpPlayer,lpSearchPlayerEx);
 							break;
						}
					}
					else 
					{
						if(lpPlayer->pe.dwIPAddr==lpSearchPlayerEx->dwSearchIPAddr)
						{
							lpRoomData->TransmitPlayerInRoomEx(lpPlayer,lpSearchPlayerEx);
 							break;
						}
					}
				}
 
			}
		}

	}
	if(IS_BIT_SET(lpSearchPlayerEx->dwSearchFlag,SEARCH_PLAYER_BYMACHINEID))
	{
 		POSITION pos = mapRoomDataEx.GetStartPosition();
		while (pos)	
		{
			mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			if(lpRoomData)
			{
				CAutoLock lock(&(lpRoomData->m_csRoomData));
				int userid = 0;
				LPPLAYER lpPlayer = NULL;
				POSITION posPlayer = lpRoomData->m_mapPlayer.GetStartPosition();
				while(posPlayer)
				{
					lpRoomData->m_mapPlayer.GetNextAssoc(posPlayer, userid, lpPlayer);
					if(lpPlayer && !lstrcmp(lpPlayer->pe.szMachineID,lpSearchPlayerEx->szMachineID))
					{
						lpRoomData->TransmitPlayerInRoomEx(lpPlayer,lpSearchPlayerEx);
						break;
					}
				}
 
			}
		}
	}
	return TRUE;
}


BOOL CBaseSockClient::OnAddMutexHard(LPREQUEST lpRequest)
{
	LPADD_MUTEX_HARD  lpAddMutexHard  = LPADD_MUTEX_HARD(lpRequest->pDataPtr);
 
	CString sKey;
	CAutoLock lock(&g_csMutexHard);
	MUTEX_HARD_SMALL mhs;
	memset(&mhs,0,sizeof(mhs));

	mhs.nMutexID =lpAddMutexHard->mh.nMutexID;
	mhs.nExpiration =lpAddMutexHard->mh.nExpiration ;
	memcpy(mhs.szMachineID1,lpAddMutexHard->mh.szMachineID1,sizeof(mhs.szMachineID1));
	memcpy(mhs.szMachineID2,lpAddMutexHard->mh.szMachineID2,sizeof(mhs.szMachineID2));
 	
 	sKey.Format(_T("%s|%s"),mhs.szMachineID1, mhs.szMachineID2);
	g_mapMutexHardSmall.SetAt(sKey,mhs);

	sKey.Format(_T("%s|%s"),mhs.szMachineID2, mhs.szMachineID1);
	g_mapMutexHardSmall.SetAt(sKey,mhs);

	return TRUE;
}

BOOL CBaseSockClient::OnDelMutexHard(LPREQUEST lpRequest)
{
	LPDEL_MUTEX_HARD  lpDelMutexHard  = LPDEL_MUTEX_HARD(lpRequest->pDataPtr);
 
	CString sKey;
	MUTEX_HARD_SMALL mhs;
	memset(&mhs,0,sizeof(mhs));
 	CAutoLock lock(&g_csMutexHard);
	POSITION pos=g_mapMutexHardSmall.GetStartPosition();
 	while (pos)	
	{


		g_mapMutexHardSmall.GetNextAssoc(pos, sKey, mhs);
        if(mhs.nMutexID==lpDelMutexHard->nMutexID)
			g_mapMutexHardSmall.RemoveKey(sKey);
	}
 	return TRUE;
}
BOOL CBaseSockClient::OnStatGameUsers(LPREQUEST lpRequest)
{
	LPSTAT_USERS lpsu = LPSTAT_USERS(lpRequest->pDataPtr);
	CommitGameUsers(lpsu->nStatTime);
 	return TRUE;
}
BOOL CBaseSockClient::OnStatGroupUsers(LPREQUEST lpRequest)
{
	LPSTAT_USERS lpsu = LPSTAT_USERS(lpRequest->pDataPtr);
	CommitGroupUsers(lpsu->nStatTime);
 	return TRUE;
}

BOOL CBaseSockClient::OnStatGroupUsersOnGame(LPREQUEST lpRequest)
{
	LPSTAT_USERS lpsu = LPSTAT_USERS(lpRequest->pDataPtr);
	CommitGroupUsersOnGame(lpsu->nStatTime);
 	return TRUE;
}

BOOL CBaseSockClient::OnStatIPUsers(LPREQUEST lpRequest)
{
	LPSTAT_USERS lpsu = LPSTAT_USERS(lpRequest->pDataPtr);
	CommitIPUsers(lpsu->nStatTime);
 	return TRUE;
}
BOOL CBaseSockClient::OnStatIPUsersOnGame(LPREQUEST lpRequest)
{
	LPSTAT_USERS lpsu = LPSTAT_USERS(lpRequest->pDataPtr);
	CommitIPUsersOnGame(lpsu->nStatTime);
 	return TRUE;
}
BOOL CBaseSockClient::OnForbidChat(LPREQUEST lpRequest)
{
  	LPCHAT_FORBID  lpChatForbid  = LPCHAT_FORBID(lpRequest->pDataPtr);
	{
		CString sMachineID;
		sMachineID.Format(_T("%s"),lpChatForbid->szHardID);
		CAutoLock lock(&g_csChatForbid);
		if(lpChatForbid->bForbid) //添加
		{
			g_mapChatForbid.SetAt(sMachineID,*lpChatForbid);
			g_mapChatForbidID.SetAt(lpChatForbid->nPlayerID,*lpChatForbid);
		}
		else
		{
			CHAT_FORBID cf;
			memset(&cf,0,sizeof(CHAT_FORBID));
			if(g_mapChatForbid.Lookup(sMachineID,cf))
			{
				g_mapChatForbid.RemoveKey(sMachineID);
				g_mapChatForbidID.RemoveKey(cf.nPlayerID);
			}
 			
			g_mapChatForbidID.RemoveKey(lpChatForbid->nPlayerID);

		}
	}
   	return TRUE;
}

BOOL CBaseSockClient::OnRefreshFilterWord(LPREQUEST lpRequest)
{
	GetFilterWord();
	return TRUE;
}
BOOL CBaseSockClient::OnRefreshMutexHard(LPREQUEST lpRequest)
{
	GetMutexHard();
	return TRUE;
}

BOOL CBaseSockClient::OnWriteRoomSvrProfile(LPREQUEST lpRequest)
{
  	LPWRITE_PROFILE lpWriteProfile = LPWRITE_PROFILE(PBYTE(lpRequest->pDataPtr));
	if(IS_BIT_SET(lpWriteProfile->dwFlags,FLAG_WRITEPROFILE_DELKEY))
		WritePrivateProfileString(lpWriteProfile->pi.szSection,	lpWriteProfile->pi.szKeyName,NULL,g_szIniFile);
	else if(IS_BIT_SET(lpWriteProfile->dwFlags,FLAG_WRITEPROFILE_DELSEC))
		WritePrivateProfileString(lpWriteProfile->pi.szSection,	NULL,NULL,g_szIniFile);
	else
		WritePrivateProfileString(lpWriteProfile->pi.szSection,	lpWriteProfile->pi.szKeyName,lpWriteProfile->pi.szString,g_szIniFile);

	return TRUE;
}

BOOL CBaseSockClient::OnWriteIPManProfile(LPREQUEST lpRequest)
{
  	LPWRITE_PROFILE lpWriteProfile = LPWRITE_PROFILE(PBYTE(lpRequest->pDataPtr));
	if(IS_BIT_SET(lpWriteProfile->dwFlags,FLAG_WRITEPROFILE_DELKEY))
		WritePrivateProfileString(lpWriteProfile->pi.szSection,	lpWriteProfile->pi.szKeyName,NULL,g_szIPManFile);
	else if(IS_BIT_SET(lpWriteProfile->dwFlags,FLAG_WRITEPROFILE_DELSEC))
		WritePrivateProfileString(lpWriteProfile->pi.szSection,	NULL,NULL,g_szIPManFile);
	else
		WritePrivateProfileString(lpWriteProfile->pi.szSection,	lpWriteProfile->pi.szKeyName,lpWriteProfile->pi.szString,g_szIPManFile);

	return TRUE;
}
BOOL CBaseSockClient::OnUpdateRoomLevel(LPREQUEST lpRequest)
{
	LPUPDATE_ROOM_LEVEL lpUpdateroomlevel=LPUPDATE_ROOM_LEVEL(PBYTE(lpRequest->pDataPtr));

    CBaseRoomData* lpRoomData = NULL;
	{
	 	CAutoLock lock(&g_csRoomData);
		if(g_mapRoomDataEx.Lookup(lpUpdateroomlevel->nRoomID, lpRoomData))
		{
			lpRoomData->roomdata.nMinDeposit=lpUpdateroomlevel->nMinDeposit;
			lpRoomData->roomdata.nMaxDeposit=lpUpdateroomlevel->nMaxDeposit;
 
			lpRoomData->roomdata.nMinPlayScore=lpUpdateroomlevel->nMinPlayScore;
			lpRoomData->roomdata.nMaxPlayScore=lpUpdateroomlevel->nMaxPlayScore;
 
			HWND hWnd=lpRoomData->FindGameSvrWindow();
			if(IsWindow(hWnd))
			{
   				PostMessage(hWnd,WM_RTG_MINPLAYSCORE,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.nMinPlayScore);
				PostMessage(hWnd,WM_RTG_MAXPLAYSCORE,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.nMaxPlayScore);
	
				PostMessage(hWnd,WM_RTG_MINDEPOSIT,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.nMinDeposit);
  				PostMessage(hWnd,WM_RTG_MAXDEPOSIT,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.nMaxDeposit);


			}
		}
	}
 	return TRUE;
}


BOOL CBaseSockClient::OnUpdateRoomOCMG(LPREQUEST lpRequest)
{
	LPUPDATE_ROOM_OCMG lpUpdateroomocmg=LPUPDATE_ROOM_OCMG(PBYTE(lpRequest->pDataPtr));

    CBaseRoomData* lpRoomData = NULL;
	{
	 	CAutoLock lock(&g_csRoomData);
		if(g_mapRoomDataEx.Lookup(lpUpdateroomocmg->nRoomID, lpRoomData))
		{
 			lpRoomData->roomdata.dwOptions=lpUpdateroomocmg->dwOptions;
			lpRoomData->roomdata.dwConfigs=lpUpdateroomocmg->dwConfigs;
			lpRoomData->roomdata.dwManages=lpUpdateroomocmg->dwManages;
			lpRoomData->roomdata.dwGameOptions=lpUpdateroomocmg->dwGameOptions;

			HWND hWnd=lpRoomData->FindGameSvrWindow();
			if(IsWindow(hWnd))
			{
				PostMessage(hWnd,WM_RTG_ROOMOPTIONS,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.dwOptions);
				PostMessage(hWnd,WM_RTG_ROOMCONFIGS,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.dwConfigs);
				PostMessage(hWnd,WM_RTG_ROOMMANAGES,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.dwManages);
 				PostMessage(hWnd,WM_RTG_GAMEOPTIONS,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.dwGameOptions);
			}
		}
	}
 	return TRUE;
}


BOOL CBaseSockClient::OnUpdateRoomIntField(LPREQUEST lpRequest)
{
	LPUPDATE_ROOM_INTFIELD lpUpdateroomintfield=LPUPDATE_ROOM_INTFIELD(PBYTE(lpRequest->pDataPtr));

    CBaseRoomData* lpRoomData = NULL;
	{
	 	CAutoLock lock(&g_csRoomData);
		if(g_mapRoomDataEx.Lookup(lpUpdateroomintfield->nRoomID, lpRoomData))
		{
			if(ROOM_FIELD_OPTIONS == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.dwOptions=lpUpdateroomintfield->nFieldValue;
				HWND hWnd=lpRoomData->FindGameSvrWindow();
				if(IsWindow(hWnd))
   					PostMessage(hWnd,WM_RTG_ROOMOPTIONS,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.dwOptions);
			}
			else  if(ROOM_FIELD_CONFIGS == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.dwConfigs=lpUpdateroomintfield->nFieldValue;
				HWND hWnd=lpRoomData->FindGameSvrWindow();
				if(IsWindow(hWnd))
   					PostMessage(hWnd,WM_RTG_ROOMCONFIGS,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.dwConfigs);

			}
			else  if(ROOM_FIELD_MANAGES == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.dwManages=lpUpdateroomintfield->nFieldValue;
				HWND hWnd=lpRoomData->FindGameSvrWindow();
				if(IsWindow(hWnd))
   					PostMessage(hWnd,WM_RTG_ROOMMANAGES,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.dwManages);

			}
			else  if(ROOM_FIELD_GAMEOPTIONS == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.dwGameOptions=lpUpdateroomintfield->nFieldValue;
				HWND hWnd=lpRoomData->FindGameSvrWindow();
				if(IsWindow(hWnd))
   					PostMessage(hWnd,WM_RTG_GAMEOPTIONS,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.dwGameOptions);
			}
			else  if(ROOM_FIELD_GAMEPARAM == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.nGameParam=lpUpdateroomintfield->nFieldValue;
				HWND hWnd=lpRoomData->FindGameSvrWindow();
				if(IsWindow(hWnd))
   					PostMessage(hWnd,WM_RTG_GAMEPARAM,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.nGameParam);
 
			}
			else  if(ROOM_FIELD_GAMEDATA == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.nGameData=lpUpdateroomintfield->nFieldValue;
				HWND hWnd=lpRoomData->FindGameSvrWindow();
				if(IsWindow(hWnd))
   					PostMessage(hWnd,WM_RTG_GAMEDATA,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.nGameData);
			}
			else  if(ROOM_FIELD_PORT == lpUpdateroomintfield->nFieldName)
			{
			}
			else  if(ROOM_FIELD_AREAID == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.nAreaID=lpUpdateroomintfield->nFieldValue;
			}
  			else  if(ROOM_FIELD_MINPLAYSCORE ==lpUpdateroomintfield->nFieldName )
			{
 				lpRoomData->roomdata.nMinPlayScore=lpUpdateroomintfield->nFieldValue;

				HWND hWnd=lpRoomData->FindGameSvrWindow();
				if(IsWindow(hWnd))
   					PostMessage(hWnd,WM_RTG_MINPLAYSCORE,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.nMinPlayScore);

			}
 			else  if(ROOM_FIELD_MAXPLAYSCORE ==lpUpdateroomintfield->nFieldName )
			{
 				lpRoomData->roomdata.nMaxPlayScore=lpUpdateroomintfield->nFieldValue;

				HWND hWnd=lpRoomData->FindGameSvrWindow();
				if(IsWindow(hWnd))
   					PostMessage(hWnd,WM_RTG_MAXPLAYSCORE,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.nMaxPlayScore);
			}
  			else  if(ROOM_FIELD_TABLECOUNT ==lpUpdateroomintfield->nFieldName )
			{
 				lpRoomData->roomdata.nTableCount=lpUpdateroomintfield->nFieldValue;
			}
			else if(ROOM_FIELD_MATCHID == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.nMatchID=lpUpdateroomintfield->nFieldValue;
			}
  			else  if(ROOM_FIELD_MAXUSERS ==lpUpdateroomintfield->nFieldName )
			{
 				lpRoomData->roomdata.nMaxUsers=lpUpdateroomintfield->nFieldValue;
			}

 			else if(ROOM_FIELD_MINBOUTSECOND == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.nMinBoutSecond=lpUpdateroomintfield->nFieldValue;
			}
			else if(ROOM_FIELD_MAXBOUTSECOND == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.nMaxBoutSecond=lpUpdateroomintfield->nFieldValue;
			}
 			else if(ROOM_FIELD_MINEXPERIENCE == lpUpdateroomintfield->nFieldName)
			{
// 				lpRoomData->roomdata.nMinExperience=lpUpdateroomintfield->nFieldValue;
			}
 			else if(ROOM_FIELD_MINSCORE == lpUpdateroomintfield->nFieldName)
			{
// 				lpRoomData->roomdata.nMinScore=lpUpdateroomintfield->nFieldValue;
			}
 			else if(ROOM_FIELD_MAXSCORE == lpUpdateroomintfield->nFieldName)
			{
// 				lpRoomData->roomdata.nMaxScore=lpUpdateroomintfield->nFieldValue;
			}
			else if(ROOM_FIELD_MINLEVEL == lpUpdateroomintfield->nFieldName)
			{
// 				lpRoomData->roomdata.nMinLevel=lpUpdateroomintfield->nFieldValue;
			}
			else if(ROOM_FIELD_MINDEPOSIT == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.nMinDeposit=lpUpdateroomintfield->nFieldValue;
			
				HWND hWnd=lpRoomData->FindGameSvrWindow();
				if(IsWindow(hWnd))
   					PostMessage(hWnd,WM_RTG_MINDEPOSIT,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.nMinDeposit);

			}
			else if(ROOM_FIELD_MAXDEPOSIT == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.nMaxDeposit=lpUpdateroomintfield->nFieldValue;

				HWND hWnd=lpRoomData->FindGameSvrWindow();
				if(IsWindow(hWnd))
   					PostMessage(hWnd,WM_RTG_MAXDEPOSIT,lpRoomData->roomdata.nRoomID, lpRoomData->roomdata.nMaxDeposit);

			}
			else if(ROOM_FIELD_GAMEPORT == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.nGamePort =lpUpdateroomintfield->nFieldValue;
			}
			else if(ROOM_FIELD_TYPE == lpUpdateroomintfield->nFieldName)
			{
 				lpRoomData->roomdata.nRoomType =lpUpdateroomintfield->nFieldValue;
			}
  			else  if(ROOM_FIELD_CHAIRCOUNT ==lpUpdateroomintfield->nFieldName )
			{
 				lpRoomData->roomdata.nChairCount=lpUpdateroomintfield->nFieldValue;
			}
			else if (ROOM_FIELD_ACTIVITYCLOTHINGS==lpUpdateroomintfield->nFieldName )
			{
				
			}
			else if (ROOM_FIELD_CLIENTID==lpUpdateroomintfield->nFieldName )
			{
				
			}
		}
	}
 	return TRUE;
}


BOOL CBaseSockClient::OnUpdateRoomCharField(LPREQUEST lpRequest)
{
	LPUPDATE_ROOM_CHARFIELD lpUpdateroomcharfield=LPUPDATE_ROOM_CHARFIELD(PBYTE(lpRequest->pDataPtr));

    CBaseRoomData* lpRoomData = NULL;
	{
	 	CAutoLock lock(&g_csRoomData);
		if(g_mapRoomDataEx.Lookup(lpUpdateroomcharfield->nRoomID, lpRoomData))
		{
			if(ROOM_FIELD_GAMEIP == lpUpdateroomcharfield->nFieldName)
			{
			}
			else if(ROOM_FIELD_NAME ==lpUpdateroomcharfield->nFieldName)
			{
			}
			else if(ROOM_FIELD_EXENAME ==lpUpdateroomcharfield->nFieldName)
			{
			}
			else if(ROOM_FIELD_WWW ==lpUpdateroomcharfield->nFieldName)
			{
			}
			else if(ROOM_FIELD_PASSWORD ==lpUpdateroomcharfield->nFieldName)
			{
			}

		}
	}
 	return TRUE;
}
BOOL CBaseSockClient::OnUpdateRoomSameIP(LPREQUEST lpRequest)
{
	LPUPDATE_ROOM_SAMEIP lpUpdateroomsameip=LPUPDATE_ROOM_SAMEIP(PBYTE(lpRequest->pDataPtr));
	int nRoomID=lpUpdateroomsameip->nRoomID;
   
    CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		if(g_mapRoomDataEx.Lookup(nRoomID,lpRoomData))
		{
			if(lpUpdateroomsameip->bAllowSameIP)
				lpRoomData->roomdata.dwOptions &=~ROOM_OPT_FORBIDSAMEIP;
			else
				lpRoomData->roomdata.dwOptions |=ROOM_OPT_FORBIDSAMEIP;
		}
 
	}
    return TRUE;

}

BOOL CBaseSockClient::OnUpdateRoom(LPREQUEST lpRequest)
{
	LPUPDATE_ROOM  lpUpdateRoom=LPUPDATE_ROOM(PBYTE(lpRequest->pDataPtr));

    CBaseRoomData* lpRoomData = NULL;
	{
	 	CAutoLock lock(&g_csRoomData);

		if(g_mapRoomDataEx.Lookup(lpUpdateRoom->Room.nRoomID, lpRoomData))
		{
 			lpRoomData->roomdata.nRoomType=lpUpdateRoom->Room.nRoomType;
 			lpRoomData->roomdata.nGameID=lpUpdateRoom->Room.nGameID;
			lpRoomData->roomdata.nAreaID=lpUpdateRoom->Room.nAreaID;
			lpRoomData->roomdata.nMaxUsers=lpUpdateRoom->Room.nMaxUsers;

			lpRoomData->roomdata.dwOptions=lpUpdateRoom->Room.dwOptions;
			lpRoomData->roomdata.dwConfigs=lpUpdateRoom->Room.dwConfigs;
			lpRoomData->roomdata.dwManages=lpUpdateRoom->Room.dwManages;
			lpRoomData->roomdata.dwGameOptions=lpUpdateRoom->Room.dwGameOptions;
	
			lpRoomData->roomdata.nTableCount=lpUpdateRoom->Room.nTableCount;
 			lpRoomData->roomdata.nChairCount=lpUpdateRoom->Room.nChairCount;
			lpRoomData->roomdata.nGamePort=  lpUpdateRoom->Room.nGamePort;
			lpRoomData->roomdata.nMinDeposit=lpUpdateRoom->Room.nMinDeposit;
			lpRoomData->roomdata.nMaxDeposit=lpUpdateRoom->Room.nMaxDeposit;
	
			lpRoomData->roomdata.nGameParam=lpUpdateRoom->Room.nGameParam;
			lpRoomData->roomdata.nGameData= lpUpdateRoom->Room.nGameData;

			lpRoomData->roomdata.nMinPlayScore=lpUpdateRoom->Room.nMinPlayScore;
			lpRoomData->roomdata.nMaxPlayScore=lpUpdateRoom->Room.nMaxPlayScore;
 			lpRoomData->roomdata.nMatchID=lpUpdateRoom->Room.nMatchID;
		
			lpRoomData->roomdata.nMinBoutSecond=lpUpdateRoom->Room.nMinBoutSecond;
			lpRoomData->roomdata.nMaxBoutSecond=lpUpdateRoom->Room.nMaxBoutSecond;

		 	lpRoomData->PostRoomData();

		}
	}
 	return TRUE;
}

BOOL CBaseSockClient::OnUpdateRoomFields(LPREQUEST lpRequest)
{
	LPUPDATE_ROOM_FIELDS lpUpdateroomfields=LPUPDATE_ROOM_FIELDS(PBYTE(lpRequest->pDataPtr));

    CBaseRoomData* lpRoomData = NULL;
	{
	 	CAutoLock lock(&g_csRoomData);

		if(g_mapRoomDataEx.Lookup(lpUpdateroomfields->nRoomID, lpRoomData))
		{
			lpRoomData->roomdata.nRoomType=lpUpdateroomfields->nRoomType;
  			lpRoomData->roomdata.nMaxUsers=lpUpdateroomfields->nMaxUsers;
		}
	}
 	return TRUE;
}
BOOL CBaseSockClient::OnDeleteFilterWord(LPREQUEST lpRequest)
{
	LPDEL_FILTER_WORD lpDelFilterWord=LPDEL_FILTER_WORD(PBYTE(lpRequest->pDataPtr));
 	CAutoLock lock(&g_csFilterWord);
    g_mapFilterWord.RemoveKey (lpDelFilterWord->nWordID);
  	return TRUE;
}
BOOL CBaseSockClient::OnAddFilterWord(LPREQUEST lpRequest)
{
	LPADD_FILTER_WORD lpAddFilterWord=LPADD_FILTER_WORD(PBYTE(lpRequest->pDataPtr));

 	CAutoLock lock(&g_csFilterWord);
  	FILTER_WORD FilterWord;
    memcpy(&FilterWord,&lpAddFilterWord->fw,sizeof(FILTER_WORD));
    g_mapFilterWord.SetAt(FilterWord.nWordID, FilterWord);
 	return TRUE;
}
BOOL CBaseSockClient::OnTransferDeposit(LPREQUEST lpRequest)
{
	LPTRANSFER_DEPOSIT lpTransferDeposit = LPTRANSFER_DEPOSIT(lpRequest->pDataPtr);

	CBaseRoomData* lpRoomData = NULL;
	if(lpTransferDeposit->nFromRoom > 0 ){
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpTransferDeposit->nFromRoom, lpRoomData)){
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if(!lpRoomData->IsOldMatchRoom())
		{
			PLAYER_DEPOSIT pd;
			ZeroMemory(&pd, sizeof(pd));

			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpTransferDeposit->nUserID, lpPlayer) && lpPlayer){
				if(lpRoomData->GetGameID() == lpTransferDeposit->nFromGame){
					lpPlayer->nDeposit -= lpTransferDeposit->nDeposit;
					pd.nUserID = lpTransferDeposit->nUserID;
					pd.nDeposit = lpPlayer->nDeposit;
					/////////////////////////////////////////////////////
 
					if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom() )
					{
  						NotifyOnePlayer(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
  						NotifyDarkLooker(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
					}
					else
						NotifyRoomPlayers(lpRoomData, GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
				}
			}
		}
	}

	lpRoomData = NULL;
	if(lpTransferDeposit->nToRoom > 0 ){
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpTransferDeposit->nToRoom, lpRoomData)){
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));

		if(!lpRoomData->IsOldMatchRoom())
		{
			PLAYER_DEPOSIT pd;
			ZeroMemory(&pd, sizeof(pd));

			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpTransferDeposit->nUserID, lpPlayer) && lpPlayer){
				if(lpRoomData->GetGameID() == lpTransferDeposit->nToGame){
					lpPlayer->nDeposit += lpTransferDeposit->nDeposit;
					pd.nUserID = lpTransferDeposit->nUserID;
					pd.nDeposit = lpPlayer->nDeposit;
					if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
					{
  						NotifyOnePlayer(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
						NotifyDarkLooker(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
					}
					else
						NotifyRoomPlayers(lpRoomData, GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
				}
			}
		}
	}
	return TRUE;
}

BOOL CBaseSockClient::OnMoveSafeDeposit(LPREQUEST lpRequest)
{
	LPMOVE_SAFE_DEPOSIT lpMoveSafeDeposit = LPMOVE_SAFE_DEPOSIT(lpRequest->pDataPtr);

	CBaseRoomData* lpRoomData = NULL;
	if(lpMoveSafeDeposit->nRoomID){
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpMoveSafeDeposit->nRoomID, lpRoomData)){
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));
	
		if(!lpRoomData->IsOldMatchRoom())
		{
			PLAYER_DEPOSIT pd;
			ZeroMemory(&pd, sizeof(pd));

			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpMoveSafeDeposit->nUserID, lpPlayer) && lpPlayer){
				if(lpRoomData->GetGameID() == lpMoveSafeDeposit->nGameID){
					lpPlayer->nDeposit += lpMoveSafeDeposit->nDeposit;
					pd.nUserID = lpMoveSafeDeposit->nUserID;
					pd.nDeposit = lpPlayer->nDeposit;
					if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
					{
  						NotifyOnePlayer(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
						NotifyDarkLooker(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
					}
					else
						NotifyRoomPlayers(lpRoomData, GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
				}
			}
		}
	}
	return TRUE;
}


BOOL CBaseSockClient::OnClearNegaScore(LPREQUEST lpRequest)
{
	LPCLEAR_NEGA_SCORE lpClearNegaScore = LPCLEAR_NEGA_SCORE(lpRequest->pDataPtr);

	CBaseRoomData* lpRoomData = NULL;
	if(lpClearNegaScore->nRoomID){
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpClearNegaScore->nRoomID, lpRoomData)){
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 	
		if(!lpRoomData->IsOldMatchRoom())
		{
			PLAYER_SCORE ps;
			ZeroMemory(&ps, sizeof(PLAYER_SCORE));

			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpClearNegaScore->nUserID, lpPlayer) && lpPlayer){
				if(lpRoomData->GetGameID() == lpClearNegaScore->nGameID){
					lpPlayer->nScore  = lpClearNegaScore->nNewScore;
					ps.nUserID = lpClearNegaScore->nUserID;
					ps.nScore = lpClearNegaScore->nNewScore;
					
					if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
					{
  						NotifyOnePlayer(lpRoomData,ps.nUserID,GR_PLAYER_SCORE, &ps, sizeof(ps));
						NotifyDarkLooker(lpRoomData,ps.nUserID,GR_PLAYER_SCORE, &ps, sizeof(ps));
					}
					else 
						NotifyRoomPlayers(lpRoomData, GR_PLAYER_SCORE, &ps, sizeof(ps));
				}
			}
		}
	}
	return TRUE;
}



BOOL CBaseSockClient::OnJumpLevelScore(LPREQUEST lpRequest)
{
	LPJUMP_LEVEL_SCORE lpJumpLevelScore = LPJUMP_LEVEL_SCORE(lpRequest->pDataPtr);

	CBaseRoomData* lpRoomData = NULL;
	if(lpJumpLevelScore->nRoomID){
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpJumpLevelScore->nRoomID, lpRoomData)){
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));
	
		if(!lpRoomData->IsOldMatchRoom())
		{
			PLAYER_SCORE ps;
			ZeroMemory(&ps, sizeof(PLAYER_SCORE));

			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpJumpLevelScore->nUserID, lpPlayer) && lpPlayer){
				if(lpRoomData->GetGameID() == lpJumpLevelScore->nGameID){
					lpPlayer->nScore  = lpJumpLevelScore->nNewScore;
					lpPlayer->nPlayerLevel=lpJumpLevelScore->nLevel;

					ps.nUserID = lpJumpLevelScore->nUserID;
					ps.nScore = lpJumpLevelScore->nNewScore;
					ps.nPlayerLevel=lpJumpLevelScore->nLevel;

					if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
					{
  						NotifyOnePlayer(lpRoomData,ps.nUserID,GR_PLAYER_SCORE, &ps, sizeof(ps));
						NotifyDarkLooker(lpRoomData,ps.nUserID,GR_PLAYER_SCORE, &ps, sizeof(ps));
					}
					else 
 						NotifyRoomPlayers(lpRoomData, GR_PLAYER_SCORE, &ps, sizeof(ps));
				}
			}

		}
	}
	return TRUE;
}


BOOL CBaseSockClient::OnResponse(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	switch(lpReqToClient->head.nRequest){
	case GR_ENTER_ROOM_OK:
		OnEnterRoomOK(lpContext, lpReqToClient, lpReqFromServer);
		break;
	case GR_ENTER_DARKROOM_OK:
		OnEnterDarkRoomOK(lpContext, lpReqToClient, lpReqFromServer);
		break;
	case GR_ENTER_WEBGAMEROOM_OK:
		OnEnterWebGameRoomOK(lpContext, lpReqToClient, lpReqFromServer);
 		break;
	case GR_ENTER_CLOAKINGROOM_OK:
		OnEnterCloakingRoomOK(lpContext, lpReqToClient, lpReqFromServer);
 		break;
 	case GR_ENTER_MATCHROOM_OK:
		OnEnterMatchRoomOK(lpContext, lpReqToClient, lpReqFromServer);
 		break;
	case GR_ENTER_ROOM:
		OnEnterRoom(lpContext, lpReqToClient, lpReqFromServer);
		break;
	case GR_ENTER_MATCH_RETURN:
		AT_OnEnterMatchReturn(lpContext, lpReqToClient, lpReqFromServer);
 		break;
	case GR_MATCH_ADMIN_ATTACH:
		AT_OnAdminMatchAttach(lpContext, lpReqToClient, lpReqFromServer);
		break;
	case GR_MATCH_ADMIN_START:
		AT_OnAdminMatchStart(lpContext, lpReqToClient, lpReqFromServer);
		break;
	case GR_MATCH_ADMIN_TERMINATE:
		AT_OnAdminMatchTeminate(lpContext, lpReqToClient, lpReqFromServer);
		break;
	case GR_MATCH_ADMIN_RESETTABLE:
		AT_OnAdminMatchRestTable(lpContext, lpReqToClient, lpReqFromServer);
		break;
	case GR_MATCH_ADMIN_REFRESH:
		AT_OnAdminMatchRefresh(lpContext, lpReqToClient, lpReqFromServer);
		break;
	case GR_MATCH_ADMIN_KICKOFF:
		AT_OnAdminMatchKickOff(lpContext, lpReqToClient, lpReqFromServer);
		break;
	case GR_MATCH_ADMIN_RESETALLTABLE:
		AT_OnAdminMatchResetAllTable(lpContext, lpReqToClient, lpReqFromServer);
		break;
	case GR_ROOM_NEEDSIGNUP:
		OnRoomNeedSignUp(lpContext, lpReqToClient, lpReqFromServer);
 		break;

	///////////////////////////////////////////////////////////////////////
	///////        约牌系统      //////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	case YR_GET_QUANTABLEEX:
		YP_OnGetQuanTableEx(lpContext, lpReqToClient, lpReqFromServer);
		break;
	case YR_SET_QUAN_GAMESET_OK:
		YP_OnSetQuanGameSetOK(lpContext, lpReqToClient, lpReqFromServer);
		break;
	default:
		OnDefault(lpContext, lpReqToClient, lpReqFromServer);
		break;
	}
	return TRUE;
}

BOOL CBaseSockClient::OnDefault(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	int nLen = lpReqFromServer->nDataLen - sizeof(CONTEXT_HEAD);
	PBYTE pData = new BYTE[nLen];
	lpReqToClient->pDataPtr = pData;
	lpReqToClient->nDataLen = nLen;
	memcpy(lpReqToClient->pDataPtr, PBYTE(lpReqFromServer->pDataPtr) + sizeof(CONTEXT_HEAD), nLen);

	BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient);
	
	return TRUE;
}
 

BOOL CBaseSockClient::OnEnterRoom(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{ 
	LPENTER_ROOM_EX lpEnterRoomEx = LPENTER_ROOM_EX(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));

	if(UwlGetCurrentSeconds()-lpEnterRoomEx->er.nEnterTime>ENTERROOM_TIMEOUT_SECONDS)
		lpEnterRoomEx->nVerifyReturn=UR_RESPONSE_TIMEOUT;

	if(lpEnterRoomEx->nVerifyReturn==UR_OPERATE_SUCCEEDED)//通过认证 
	{
 		LPENTER_ROOM_EX lpere=new ENTER_ROOM_EX;
		ZeroMemory(lpere, sizeof(ENTER_ROOM_EX));
		memcpy(lpere,lpEnterRoomEx,sizeof(ENTER_ROOM_EX));
		lpere->nRoomPort = g_nRoomPort;		//进入房间校验roomsvr端口
 		g_pSockServer->PutToServer(lpContext,GR_ENTER_ROOM_VERIFIED , lpere, sizeof(ENTER_ROOM_EX));
	}
	else
	{
		lpReqToClient->head.nRequest = lpEnterRoomEx->nVerifyReturn;
		if(lpReqToClient->head.nRequest==GR_ROOM_NEED_DXXW)
		{
			PBYTE pData = new BYTE[sizeof(int)];
			memcpy(pData,&lpEnterRoomEx->nDXXWRoomID,sizeof(int));
			lpReqToClient->pDataPtr = pData;
			lpReqToClient->nDataLen = sizeof(int);
		}
 		BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient);
	}
	return TRUE;
}



BOOL CBaseSockClient::OnEnterDarkRoomOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	LPPLAYER lpPlayer = LPPLAYER(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));

	if(UwlGetCurrentSeconds()-lpPlayer->pe.nEnterTime>ENTERROOM_TIMEOUT_SECONDS)
		return TRUE;

	LPROOMDATA lprd = LPROOMDATA((PBYTE)lpPlayer + sizeof(PLAYER));
	int nRoomID=lprd->nRoomID;

	ROOM_USERS ru;
	ZeroMemory(&ru, sizeof(ru));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData)){
			lpRoomData = CreateRoomData();
			g_mapRoomDataEx.SetAt(nRoomID, lpRoomData);
		}
	}
  	lpRoomData->PostHardAndToken(lpPlayer->nUserID,lpPlayer->pe.szHardID,lpContext->lTokenID);
	if(IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))//内部版
		lpRoomData->PostSecurityToHold(lpPlayer->nUserID,lpPlayer->pe.szVolumeID,lpContext->lTokenID,lpPlayer->nUserType);

	LPPLAYER pFind = NULL;
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 		memcpy(&(lpRoomData->roomdata), lprd, sizeof(ROOMDATA));
 		if(!lpRoomData->m_mapPlayer.Lookup(lpPlayer->nUserID, pFind))
		{
			pFind = GetPlayerBlock();
			ZeroMemory(pFind, sizeof(PLAYER));
			lpRoomData->m_mapPlayer.SetAt(lpPlayer->nUserID, pFind);
		}
		memcpy(pFind, lpPlayer, sizeof(PLAYER));
 		USER_SOCK us;
		ZeroMemory(&us,sizeof(us));
		us.sock=lpContext->hSocket;
		us.token=lpContext->lTokenID;
		us.nUserID=lpPlayer->nUserID;
		us.dwEnterFlags=lpPlayer->pe.dwEnterFlags;
		lpRoomData->m_mapUserSock.SetAt(us.nUserID,us);
 		lpRoomData->m_mapTokenUserSock.SetAt( us.token,us);		
 		pFind->pe.dwPulseTime = GetTickCount();
		pFind->pe.dwLatestTime = GetTickCount();
		pFind->pe.dwIPAddr = g_pSockServer->GetClientAddress(lpContext->hSocket, lpContext->lTokenID);
		pFind->pe.lTokenID=lpContext->lTokenID;
 		lpPlayer->pe.dwIPAddr = pFind->pe.dwIPAddr;
		lpPlayer->pe.lTokenID = pFind->pe.lTokenID;
	
 	    int nPlayerMinSize=(sizeof(PLAYER)-sizeof(PLAYER_EXTEND));
		
 		BOOL bLookDark=IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_LOOKDARK);
 
		ENTER_ROOM_OK enterroomOK;
		ZeroMemory(&enterroomOK,sizeof(enterroomOK));
		enterroomOK.nPlayerCount = lpRoomData->m_mapPlayer.GetCount();
		enterroomOK.nTableCount=lpRoomData->roomdata.nTableCount;
		enterroomOK.nRoomTokenID=lpPlayer->pe.lTokenID;
	  	enterroomOK.nRoomPulseInterval=60;
	  	enterroomOK.dwClientIP=lpPlayer->pe.dwIPAddr;

		if( bLookDark==FALSE) //玩家进入
		{
 			enterroomOK.dwEnterOKFlag = FLAG_ENTERROOMOK_DARK;

			int nLen  = sizeof(enterroomOK) + nPlayerMinSize;
			PBYTE pData = new BYTE[nLen];
			ZeroMemory(pData, nLen);
 			memcpy(pData, &enterroomOK, sizeof(enterroomOK));
			memcpy(pData+sizeof(enterroomOK),pFind,nPlayerMinSize);
 			lpReqToClient->pDataPtr = pData;
			lpReqToClient->nDataLen = nLen;
 			BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient);			
			NotifyDarkLooker(lpRoomData,0,GR_PLAYER_ENTERED,lpPlayer, sizeof(PLAYER));
		}
		else  //管理员进入
		{
			int nBeginRandomTableNO=lpRoomData->GetBeginRandomTableNO();
		
			int nLen=0;
			int tablecount = 0;
			int tableno = 0;
			LPTABLE pTable = NULL;
			POSITION pos  = lpRoomData->m_mapTable.GetStartPosition();
			while (pos)	{
				lpRoomData->m_mapTable.GetNextAssoc(pos, tableno, pTable);
				if(tableno>=nBeginRandomTableNO)
					continue;
				if(pTable && pTable->th.nPlayerCount){
					nLen += (pTable->th.nPlayerCount * sizeof(PLAYER_POS));
					nLen += (pTable->th.nVisitorCount * sizeof(VISITOR_POS));
					tablecount++;
				}
			}
  			enterroomOK.nActiveTableCount = tablecount;

			nLen += sizeof(enterroomOK) + enterroomOK.nPlayerCount * nPlayerMinSize + tablecount* sizeof(TABLE_HEAD);
			PBYTE pData = new BYTE[nLen];
			ZeroMemory(pData, nLen);

			memcpy(pData, &enterroomOK, sizeof(enterroomOK));

			lpRoomData->CopyPlayerMap(pData + sizeof(enterroomOK),  bLookDark);
			lpRoomData->CopyTableMapEx(pData + sizeof(enterroomOK) + enterroomOK.nPlayerCount * nPlayerMinSize, lpPlayer); 
 
			lpReqToClient->pDataPtr = pData;
			lpReqToClient->nDataLen = nLen;

			BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient, MSG_RESPONSE, 0, COMPRESS_ZIP);
  		 
			NotifyDarkLooker(lpRoomData,lpPlayer->nUserID,GR_PLAYER_ENTERED,lpPlayer, sizeof(PLAYER));
	 
			lpRoomData->m_mapDarkLooker.SetAt(lpPlayer->nUserID,lpPlayer->pe.lTokenID);
		}
  	    ////////////////////////////////////////////////////////////////////////////////////////////
		ru.nClientID = g_nClientID;
 		ru.nGameID = lpRoomData->GetGameID();
		ru.nAreaID = lpRoomData->GetAreaID();
		ru.nRoomID = lpRoomData->GetRoomID();
		ru.nUserCount = enterroomOK.nPlayerCount;
		ru.nUserID = lpPlayer->nUserID;
	}

 	TransmitRoomUsers(lpContext, &ru);

	return TRUE;
}


BOOL CBaseSockClient::OnEnterMatchRoomOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	LPPLAYER lpPlayer = LPPLAYER(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));

	if(UwlGetCurrentSeconds()-lpPlayer->pe.nEnterTime>ENTERROOM_TIMEOUT_SECONDS)
		return TRUE;

	LPROOMDATA lprd = LPROOMDATA((PBYTE)lpPlayer + sizeof(PLAYER));
	LPMATCH_DYNA lpmd = LPMATCH_DYNA((PBYTE)lpPlayer + sizeof(PLAYER)+sizeof(ROOMDATA));

	int nRoomID=lprd->nRoomID;

	ROOM_USERS ru;
	ZeroMemory(&ru, sizeof(ru));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData)){
			lpRoomData = CreateRoomData();
			g_mapRoomDataEx.SetAt(nRoomID, lpRoomData);
		}
	}
	if(IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))//内部版
		lpRoomData->PostSecurityToHold(lpPlayer->nUserID,lpPlayer->pe.szVolumeID,lpContext->lTokenID,lpPlayer->nUserType);

 	LPPLAYER pFind = NULL;
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 		memcpy(&(lpRoomData->roomdata), lprd, sizeof(ROOMDATA));

		//刷新比赛状态
		if (lpRoomData->m_pTcMatch)
	   		lpRoomData->m_pTcMatch->RefreshDataFromMatchDynamic(lpmd);

 		if(!lpRoomData->m_mapPlayer.Lookup(lpPlayer->nUserID, pFind))
		{
			pFind = GetPlayerBlock();
			ZeroMemory(pFind, sizeof(PLAYER));
			lpRoomData->m_mapPlayer.SetAt(lpPlayer->nUserID, pFind);
		}
		memcpy(pFind, lpPlayer, sizeof(PLAYER));
 		USER_SOCK us;
		ZeroMemory(&us,sizeof(us));
		us.sock=lpContext->hSocket;
		us.token=lpContext->lTokenID;
		us.nUserID=lpPlayer->nUserID;
		us.dwEnterFlags=lpPlayer->pe.dwEnterFlags;
 		lpRoomData->m_mapUserSock.SetAt(us.nUserID,us);
 		lpRoomData->m_mapTokenUserSock.SetAt( us.token,us);		
 		pFind->pe.dwPulseTime = GetTickCount();
		pFind->pe.dwLatestTime = GetTickCount();
		pFind->pe.dwIPAddr = g_pSockServer->GetClientAddress(lpContext->hSocket, lpContext->lTokenID);
		pFind->pe.lTokenID=lpContext->lTokenID;
 		lpPlayer->pe.dwIPAddr = pFind->pe.dwIPAddr;
		lpPlayer->pe.lTokenID = pFind->pe.lTokenID;
	
 		ENTER_ROOM_OK enterroomOK;
		ZeroMemory(&enterroomOK,sizeof(enterroomOK));
		enterroomOK.nPlayerCount = lpRoomData->m_mapPlayer.GetCount();
		enterroomOK.nTableCount=lpRoomData->roomdata.nTableCount;
		enterroomOK.nRoomTokenID=lpPlayer->pe.lTokenID;
 		enterroomOK.dwEnterOKFlag = FLAG_ENTERROOMOK_MATCH;
 	 	enterroomOK.nRoomPulseInterval=60;
 	 	enterroomOK.dwClientIP=lpPlayer->pe.dwIPAddr;

		BOOL bLookDark=IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_LOOKDARK);
 		int nPlayerMinSize=(sizeof(PLAYER)-sizeof(PLAYER_EXTEND));

		if( bLookDark==FALSE) //比赛用户进入
		{
			enterroomOK.nTableCount=1;//比赛系统只处理1张桌子

			int nLen  = sizeof(enterroomOK) + nPlayerMinSize + sizeof(MATCH_DETAIL);
			PBYTE pData = new BYTE[nLen];
			ZeroMemory(pData, nLen);

 			memcpy(pData, &enterroomOK, sizeof(enterroomOK));
			memcpy(pData+sizeof(enterroomOK),pFind,nPlayerMinSize);

			LPMATCH_DETAIL lpMatch_Detail=(LPMATCH_DETAIL)(pData+sizeof(enterroomOK)+nPlayerMinSize);

			if (lpRoomData->m_pTcMatch)
				lpRoomData->m_pTcMatch->CopyMatchDetail(lpMatch_Detail,lpmd);

 			lpReqToClient->pDataPtr = pData;
			lpReqToClient->nDataLen = nLen;
 			BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient, MSG_RESPONSE, 0, COMPRESS_ZIP);			
 			NotifyDarkLooker(lpRoomData,0,GR_PLAYER_ENTERED,lpPlayer, sizeof(PLAYER));
		}
	    else //管理员进入
		{
			int nLen=0;
 			nLen += sizeof(enterroomOK) + enterroomOK.nPlayerCount *  sizeof(MATCHPLAYER_SMALL);
			PBYTE pData = new BYTE[nLen];
			ZeroMemory(pData, nLen);
 			memcpy(pData, &enterroomOK, sizeof(enterroomOK));
 			lpRoomData->CopyMatchPlayerMap(pData + sizeof(enterroomOK));
  
			lpReqToClient->pDataPtr = pData;
			lpReqToClient->nDataLen = nLen;

			BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient, MSG_RESPONSE, 0, COMPRESS_ZIP);
 		
			NotifyDarkLooker(lpRoomData,lpPlayer->nUserID,GR_PLAYER_ENTERED,lpPlayer, sizeof(PLAYER));
 		
			lpRoomData->m_mapDarkLooker.SetAt(lpPlayer->nUserID,lpPlayer->pe.lTokenID);
		}
 	    ////////////////////////////////////////////////////////////////////////////////////////////
		ru.nClientID = g_nClientID;
 		ru.nGameID = lpRoomData->GetGameID();
		ru.nAreaID = lpRoomData->GetAreaID();
		ru.nRoomID = lpRoomData->GetRoomID();
		ru.nUserCount = enterroomOK.nPlayerCount;
		ru.nUserID = lpPlayer->nUserID;
	}

 	TransmitRoomUsers(lpContext, &ru);
	return TRUE;
}

BOOL CBaseSockClient::OnEnterWebGameRoomOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	LPPLAYER lpPlayer = LPPLAYER(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));

	if(UwlGetCurrentSeconds()-lpPlayer->pe.nEnterTime>ENTERROOM_TIMEOUT_SECONDS)
		return TRUE;

	LPROOMDATA lprd = LPROOMDATA((PBYTE)lpPlayer + sizeof(PLAYER));
	int nRoomID=lprd->nRoomID;
	int nGameID=lprd->nGameID;

	ROOM_USERS ru;
	ZeroMemory(&ru, sizeof(ru));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData)){
			lpRoomData = CreateRoomData();
			g_mapRoomDataEx.SetAt(nRoomID, lpRoomData);
		}
	}
	if(IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))//内部版
		lpRoomData->PostSecurityToHold(lpPlayer->nUserID,lpPlayer->pe.szVolumeID,lpContext->lTokenID,lpPlayer->nUserType);

	LPPLAYER pFind = NULL;
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 		memcpy(&(lpRoomData->roomdata), lprd, sizeof(ROOMDATA));
 		if(!lpRoomData->m_mapPlayer.Lookup(lpPlayer->nUserID, pFind))
		{
			pFind = GetPlayerBlock();
			ZeroMemory(pFind, sizeof(PLAYER));
			lpRoomData->m_mapPlayer.SetAt(lpPlayer->nUserID, pFind);
		}
		memcpy(pFind, lpPlayer, sizeof(PLAYER));
 		USER_SOCK us;
		ZeroMemory(&us,sizeof(us));
		us.sock=lpContext->hSocket;
		us.token=lpContext->lTokenID;
		us.nUserID=lpPlayer->nUserID;
		us.dwEnterFlags=lpPlayer->pe.dwEnterFlags;
		lpRoomData->m_mapUserSock.SetAt(us.nUserID,us);
 		lpRoomData->m_mapTokenUserSock.SetAt( us.token,us);		
 		pFind->pe.dwPulseTime = GetTickCount();
		pFind->pe.dwLatestTime = GetTickCount();
		pFind->pe.dwIPAddr = g_pSockServer->GetClientAddress(lpContext->hSocket, lpContext->lTokenID);
		pFind->pe.lTokenID=lpContext->lTokenID;
 		lpPlayer->pe.dwIPAddr = pFind->pe.dwIPAddr;
		lpPlayer->pe.lTokenID = pFind->pe.lTokenID;
	
 	    int nPlayerMinSize=(sizeof(PLAYER)-sizeof(PLAYER_EXTEND));
		
 		BOOL bLookDark=IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_LOOKDARK);
 
		ENTER_ROOM_OK enterroomOK;
		ZeroMemory(&enterroomOK,sizeof(enterroomOK));
		enterroomOK.nPlayerCount = lpRoomData->m_mapPlayer.GetCount();
		enterroomOK.nTableCount=lpRoomData->roomdata.nTableCount;
		enterroomOK.nRoomTokenID=lpPlayer->pe.lTokenID;
	  	enterroomOK.nRoomPulseInterval=60;
	  	enterroomOK.dwClientIP=lpPlayer->pe.dwIPAddr;
		enterroomOK.nGiftDeposit=lpPlayer->pe.nGiftDeposit;			//进入房间获得的赠银

		CString sRoomID=xyConvertIntToStr(nRoomID);
		WEBGAME_DETAIL webgamedetail;
		ZeroMemory(&webgamedetail,sizeof(webgamedetail));
		webgamedetail.nRoomID=nRoomID;
		webgamedetail.nGameID=nGameID;
		webgamedetail.nPlayerCount=enterroomOK.nPlayerCount;
		webgamedetail.nHandWidth=GetPrivateProfileInt(_T("WebHandWidth"), sRoomID,200, g_szIniFile );
		webgamedetail.nWebShowTime=GetPrivateProfileInt(_T("WebShowTime"), sRoomID,3, g_szIniFile );
		webgamedetail.nTimeStamp=GetPrivateProfileInt(_T("WebTimeStamp"), sRoomID,0, g_szIniFile );
		webgamedetail.nUnicodeGame=GetPrivateProfileInt(_T("WebUnicode"), sRoomID,0, g_szIniFile );
		webgamedetail.nSilentMode=GetPrivateProfileInt(_T("SilentMode"), sRoomID,0, g_szIniFile );
		webgamedetail.nAutoContract=GetPrivateProfileInt(_T("AutoContract"), sRoomID,0, g_szIniFile );

		GetPrivateProfileString(_T("WebHomeURL"),sRoomID,_T(""),webgamedetail.szHomeURL,sizeof(webgamedetail.szHomeURL) ,g_szIniFile);
		GetPrivateProfileString(_T("WebPayURL"),sRoomID,_T(""), webgamedetail.szPayURL, sizeof(webgamedetail.szPayURL) , g_szIniFile);
		GetPrivateProfileString(_T("WebHandURL"),sRoomID,_T(""),webgamedetail.szHandURL,sizeof(webgamedetail.szHandURL) ,g_szIniFile);
		GetPrivateProfileString(_T("WebGameURL"),sRoomID,_T(""),webgamedetail.szGameURL,sizeof(webgamedetail.szGameURL) ,g_szIniFile);

		if( bLookDark==FALSE) //玩家进入
		{
 			enterroomOK.dwEnterOKFlag = FLAG_ENTERROOMOK_WEBGAME;
			if(lpRoomData->IsMicroEndRoom())
				enterroomOK.dwEnterOKFlag|=FLAG_ENTERROOMOK_MICROEND;

			int nLen  = sizeof(enterroomOK) + nPlayerMinSize + sizeof(webgamedetail);
			PBYTE pData = new BYTE[nLen];
			ZeroMemory(pData, nLen);
 			memcpy(pData, &enterroomOK, sizeof(enterroomOK));
			memcpy(pData+sizeof(enterroomOK),pFind,nPlayerMinSize);
			memcpy(pData+sizeof(enterroomOK)+nPlayerMinSize,&webgamedetail,sizeof(webgamedetail));

 			lpReqToClient->pDataPtr = pData;
			lpReqToClient->nDataLen = nLen;
 			BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient,MSG_RESPONSE, 0, COMPRESS_ZIP);			
			NotifyDarkLooker(lpRoomData,0,GR_PLAYER_ENTERED,lpPlayer, sizeof(PLAYER));
		}
		else  //管理员进入
		{
			int nBeginRandomTableNO=lpRoomData->GetBeginRandomTableNO();
		
			int nLen=0;
			int tablecount = 0;
			int tableno = 0;
			LPTABLE pTable = NULL;
			POSITION pos  = lpRoomData->m_mapTable.GetStartPosition();
			while (pos)	{
				lpRoomData->m_mapTable.GetNextAssoc(pos, tableno, pTable);
				if(tableno>=nBeginRandomTableNO)
					continue;
				if(pTable && pTable->th.nPlayerCount){
					nLen += (pTable->th.nPlayerCount * sizeof(PLAYER_POS));
					nLen += (pTable->th.nVisitorCount * sizeof(VISITOR_POS));
					tablecount++;
				}
			}
  			enterroomOK.nActiveTableCount = tablecount;

			nLen += sizeof(enterroomOK) + enterroomOK.nPlayerCount * nPlayerMinSize + tablecount* sizeof(TABLE_HEAD);
			PBYTE pData = new BYTE[nLen];
			ZeroMemory(pData, nLen);

			memcpy(pData, &enterroomOK, sizeof(enterroomOK));

			lpRoomData->CopyPlayerMap(pData + sizeof(enterroomOK),  bLookDark);
			lpRoomData->CopyTableMapEx(pData + sizeof(enterroomOK) + enterroomOK.nPlayerCount * nPlayerMinSize, lpPlayer); 
 
			lpReqToClient->pDataPtr = pData;
			lpReqToClient->nDataLen = nLen;

			BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient, MSG_RESPONSE, 0, COMPRESS_ZIP);
  		 
			NotifyDarkLooker(lpRoomData,lpPlayer->nUserID,GR_PLAYER_ENTERED,lpPlayer, sizeof(PLAYER));
	 
			lpRoomData->m_mapDarkLooker.SetAt(lpPlayer->nUserID,lpPlayer->pe.lTokenID);
		}
  	    ////////////////////////////////////////////////////////////////////////////////////////////
		ru.nClientID = g_nClientID;
 		ru.nGameID = lpRoomData->GetGameID();
		ru.nAreaID = lpRoomData->GetAreaID();
		ru.nRoomID = lpRoomData->GetRoomID();
		ru.nUserCount = enterroomOK.nPlayerCount;
		ru.nUserID = lpPlayer->nUserID;
	}

 	TransmitRoomUsers(lpContext, &ru);

	return TRUE;
}



BOOL CBaseSockClient::OnSendPulse(LPREQUEST lpRequest)
{
	CAutoLock lock(&g_csServerPulseInfo);
	g_ServerPulseInfo.nLatestTime =UwlGetCurrentSeconds();

	return TRUE;
} 
 

BOOL CBaseSockClient::AT_OnMatchStart(LPMATCH_STARTTOROOM lpms)
{	
	CBaseRoomData* lpRoomData = NULL;
	if(lpms->nRoomID)
	{
		CAutoLock lock(&g_csRoomData);
		g_mapRoomDataEx.Lookup(lpms->nRoomID, lpRoomData);
	 
	}
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
	 
    	lpRoomData->AT_OnMatchStart(lpms);
 
	}
 

 	return TRUE;
}

BOOL CBaseSockClient::AT_OnMatchCanceled(LPREQUEST lpRequest)
{
	LPMATCH_CANCELED lpmc = LPMATCH_CANCELED(lpRequest->pDataPtr);
	
	CBaseRoomData* lpRoomData = NULL;
	if(lpmc->nRoomID)
	{
		CAutoLock lock(&g_csRoomData);
		g_mapRoomDataEx.Lookup(lpmc->nRoomID, lpRoomData);
	}

	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		lpRoomData->AT_OnMatchCanceled(lpmc);
		
	}

	return TRUE;
}

BOOL CBaseSockClient::AT_OnMatchSignUped(LPREQUEST lpRequest)
{
	LPMATCH_SIGNUPED lpms = LPMATCH_SIGNUPED(lpRequest->pDataPtr);
	
	CBaseRoomData* lpRoomData = NULL;
	if(lpms->nRoomID)
	{
		CAutoLock lock(&g_csRoomData);
		g_mapRoomDataEx.Lookup(lpms->nRoomID, lpRoomData);
	}
	
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		lpRoomData->AT_OnMatchSignUped(lpms);
		
	}


	return TRUE;
}

BOOL CBaseSockClient::AT_OnMatchSignUpOK(LPREQUEST lpRequest)//报名成功
{
	LPMATCH_SIGNUPOK lpms = LPMATCH_SIGNUPOK(lpRequest->pDataPtr);
	
	CBaseRoomData* lpRoomData = NULL;
	if(lpms->nRoomID)
	{
		CAutoLock lock(&g_csRoomData);
		g_mapRoomDataEx.Lookup(lpms->nRoomID, lpRoomData);
	}
	
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		lpRoomData->AT_OnMatchSignUpOK(lpms);
		
	}
	
	
	return TRUE;
}

BOOL CBaseSockClient::AT_OnMatchGiveUpOK(LPREQUEST lpRequest)//退赛成功
{
	LPMATCH_GIVEUPOK lpms = LPMATCH_GIVEUPOK(lpRequest->pDataPtr);
	
	CBaseRoomData* lpRoomData = NULL;
	if(lpms->nRoomID)
	{
		CAutoLock lock(&g_csRoomData);
		g_mapRoomDataEx.Lookup(lpms->nRoomID, lpRoomData);
	}
	
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		lpRoomData->AT_OnMatchGiveUpOK(lpms);
		
	}
	
	
	return TRUE;
}

void CBaseSockClient::AT_OnMatchSignUpFail(LPMATCH_SIGNUPFAIL lpms)
{
	CBaseRoomData* lpRoomData = NULL;
	if(lpms->nRoomID)
	{
		CAutoLock lock(&g_csRoomData);
		g_mapRoomDataEx.Lookup(lpms->nRoomID, lpRoomData);
	}
	
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		NotifyOneGameSvr(R2G_MATCH_SIGNUPFAIL,lpms,sizeof(MATCH_SIGNUPFAIL),lpRoomData->roomdata.nGamePort);	
	}
}

void CBaseSockClient::AT_OnMatchGiveUpFail(LPMATCH_GIVEUPFAIL lpmg)
{
	CBaseRoomData* lpRoomData = NULL;
	if(lpmg->nRoomID)
	{
		CAutoLock lock(&g_csRoomData);
		g_mapRoomDataEx.Lookup(lpmg->nRoomID, lpRoomData);
	}
	
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		NotifyOneGameSvr(R2G_MATCH_GIVEUPFAIL,lpmg,sizeof(MATCH_GIVEUPFAIL),lpRoomData->roomdata.nGamePort);	
	}
}

BOOL CBaseSockClient::AT_OnEnterMatchReturn(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{	
	LPCONTEXT_HEAD  lpch1=LPCONTEXT_HEAD(PBYTE(lpReqFromServer->pDataPtr));
	LPCONTEXT_HEAD  lpch2=LPCONTEXT_HEAD(PBYTE(lpReqFromServer->pDataPtr)+sizeof(CONTEXT_HEAD));
	LPENTER_MATCH_RETURN lpemr = LPENTER_MATCH_RETURN(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD)*2);

	int	nLen=0;
	PBYTE pData=NULL;
	int nRequestID=lpemr->nRequestID;

	if(nRequestID==GR_ENTER_MATCH_OK)
	{
		//判断在当前比赛中的状态
		//判断在当前比赛中的状态
		LPMATCH_PLAYER lpMatchPlayer = LPMATCH_PLAYER(PBYTE(lpemr)+ sizeof(ENTER_MATCH_RETURN) );
		LPUSERONMGAME  lpUserOnMGame = LPUSERONMGAME(PBYTE(lpMatchPlayer)+ sizeof(MATCH_PLAYER) );
		LPUSERONMATCH  lpUserOnMatch = LPUSERONMATCH(PBYTE(lpUserOnMGame)+ sizeof(USERONMGAME) );
	  	LPMATCH_HINT   lpMatchHint =   LPMATCH_HINT(PBYTE(lpUserOnMatch)+  sizeof(USERONMATCH) );

		////////////////////////////////////////////////////////////////////////////
		CBaseRoomData* lpRoomData = NULL;
		{
			CAutoLock lock(&g_csRoomData);
 		    g_mapRoomDataEx.Lookup(lpemr->nRoomID, lpRoomData) ; 
		}
		if(lpRoomData)
		{
			CAutoLock lock(&(lpRoomData->m_csRoomData));
			//刷新玩家状态
			lpRoomData->AT_OnEnterMatchOK(lpemr);
			nRequestID=lpRoomData->AT_GetMatchUserStatus(lpemr);
		}
 		/////////////////////////////////////////////////////////////////////////////

		nLen =  sizeof(CONTEXT_HEAD)+sizeof(MATCH_PLAYER)+sizeof(USERONMGAME)+sizeof(USERONMATCH)+sizeof(MATCH_HINT);
		pData = new BYTE[nLen];
		ZeroMemory(pData, nLen);
		memcpy(pData,lpch2,sizeof(CONTEXT_HEAD));
   		memcpy(pData+sizeof(CONTEXT_HEAD),lpMatchPlayer, nLen-sizeof(CONTEXT_HEAD));
	}
	else if (nRequestID==GR_ENTER_MATCH_NOSIGNUP
		||nRequestID==GR_ENTER_MATCH_SELFCANCEL)
	{
		CBaseRoomData* lpRoomData = NULL;
		{
			CAutoLock lock(&g_csRoomData);
			g_mapRoomDataEx.Lookup(lpemr->nRoomID, lpRoomData) ; 
		}
		if(lpRoomData)
		{
			CAutoLock lock(&(lpRoomData->m_csRoomData));

			if (lpRoomData->m_pTcMatch->IsMatchStart())
			{
				nRequestID=GR_ENTER_MATCH_STARTED;
			}
		}

		nLen =  sizeof(CONTEXT_HEAD);
		pData = new BYTE[nLen];
		ZeroMemory(pData, nLen);
		memcpy(pData,lpch2,sizeof(CONTEXT_HEAD));
	}
	else
	{
 		nLen =  sizeof(CONTEXT_HEAD);
   		pData = new BYTE[nLen];
 		ZeroMemory(pData, nLen);
		memcpy(pData,lpch2,sizeof(CONTEXT_HEAD));
	}

	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRepeated = 1;
	response.pDataPtr = pData;
	response.nDataLen = nLen;
	response.head.nRequest = nRequestID;//lpemr->nRequestID;

	CONTEXT_HEAD context;
	memcpy(&context, lpch1, sizeof(context));
	context.bNeedEcho = FALSE;
	g_pRoomServer->SendRequest(context.hSocket, &context, &response);
	UwlClearRequest(&response); 

 	return TRUE;
 
}

BOOL CBaseSockClient::AT_OnMatchGain(LPREQUEST lpRequest)
{
	LPMATCH_USERGAIN lpmg = LPMATCH_USERGAIN(lpRequest->pDataPtr);
	LPUSER_GAME_DUAN lpgd=(LPUSER_GAME_DUAN)((PBYTE)lpmg+sizeof(MATCH_USERGAIN));

	CBaseRoomData* lpRoomData = NULL;
	if(lpmg->nRoomID)
	{
		CAutoLock lock(&g_csRoomData);
		g_mapRoomDataEx.Lookup(lpmg->nRoomID, lpRoomData);
	}

	if (lpRoomData)
	{
		NotifyOneGameSvr(R2G_MATCH_GAIN,lpRequest->pDataPtr,lpRequest->nDataLen,lpRoomData->roomdata.nGamePort);

		NotifyOnePlayer(lpRoomData,lpmg->nUserID,GR_MATCH_GAMEDUAN,lpgd,sizeof(USER_GAME_DUAN));
		NotifyDarkLooker(lpRoomData,0,GR_MATCH_GAMEDUAN,lpgd,sizeof(USER_GAME_DUAN));
	}
	return TRUE;
}
//////////////////////////////////////////////
////////////////////////////////////////////////
//////////////////////////////////////////////////

BOOL CBaseSockClient::AT_OnAdminMatchAttach(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{ 
 	LPROOMDATA lpRoomData = LPROOMDATA(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));
 	LPMATCH lpMatch = LPMATCH(PBYTE(lpRoomData)+ sizeof(ROOMDATA));

 	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
 	BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, &response);
 

	CBaseRoomData* lpRD = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		
		if(!g_mapRoomDataEx.Lookup(lpRoomData->nRoomID, lpRD)){
			lpRD = CreateRoomData();
			g_mapRoomDataEx.SetAt(lpRoomData->nRoomID, lpRD);
		}
	}
	
	LPPLAYER pFind = NULL;
	{
		CAutoLock lock(&(lpRD->m_csRoomData));
 		memcpy(&(lpRD->roomdata), lpRoomData, sizeof(ROOMDATA));
		lpRD->AT_CreateMatch(lpMatch);
	}

	return TRUE;
}

BOOL CBaseSockClient::AT_OnAdminMatchStart(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, &response);

	LPMATCH_STARTTOROOM lpms = LPMATCH_STARTTOROOM(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));

	AT_OnMatchStart(lpms);

	return TRUE;
}

BOOL CBaseSockClient::AT_OnAdminMatchRestTable(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)//管理员要求比赛重新分桌
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, &response);
 
	LPMATCH_RUNNER lpmr = LPMATCH_RUNNER(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));
	CBaseRoomData* lpRoomData = NULL;
	if(lpmr->nRoomID)
	{
		CAutoLock lock(&g_csRoomData);
		g_mapRoomDataEx.Lookup(lpmr->nRoomID, lpRoomData);
	}
	
	if (lpRoomData)
	{
		UwlLogFile("Admin Disband Table,matchid:%ld,roomid:%ld,tableno:%ld",lpmr->nMatchID,lpmr->nRoomID,lpmr->nTableNO);
		lpRoomData->AT_OnMatchTableOverTime(lpmr->nTableNO);
	}

	return TRUE;
}

BOOL CBaseSockClient::AT_OnAdminMatchKickOff(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, &response);
	
	
	LPMATCH_RUNNER lpmr = LPMATCH_RUNNER(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));
	CBaseRoomData* lpRoomData = NULL;
	if(lpmr->nRoomID)
	{
		CAutoLock lock(&g_csRoomData);
		g_mapRoomDataEx.Lookup(lpmr->nRoomID, lpRoomData);
	}
	
	if (lpRoomData)
	{
		UwlLogFile("Admin Kickoff Player,matchid:%ld,roomid:%ld,tableno:%ld,userid:%ld",lpmr->nMatchID,lpmr->nRoomID,lpmr->nTableNO,lpmr->nUserID);
		lpRoomData->AT_OnKickPlayer(lpmr->nUserID);
	}
	
	return TRUE;
}

BOOL CBaseSockClient::AT_OnAdminMatchTeminate(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer) //管理员强制终止比赛
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, &response);
 

	return TRUE;
}

BOOL CBaseSockClient::AT_OnAdminMatchRefresh(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer) //管理员刷新比赛数据
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, &response);
 

	return TRUE;
}


BOOL	CBaseSockClient::AT_OnAdminMatchResetAllTable(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	REQUEST response;
	memset(&response, 0, sizeof(response));
	response.head.nRequest = UR_OPERATE_SUCCEEDED;
	BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, &response);
	
	LPMATCH_RUNNER lpmr = LPMATCH_RUNNER(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));
	CBaseRoomData* lpRoomData = NULL;
	if(lpmr->nRoomID)
	{
		CAutoLock lock(&g_csRoomData);
		g_mapRoomDataEx.Lookup(lpmr->nRoomID, lpRoomData);
	}
	
	if (lpRoomData)
	{
		UwlLogFile("Admin Kickoff Player,matchid:%ld,roomid:%ld",lpmr->nMatchID,lpmr->nRoomID);
		lpRoomData->AT_OnMatchResetAllTable();
	}

	return TRUE;
}

BOOL CBaseSockClient::OnRoomNeedSignUp(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	ROOM_NEEDSIGNUP  roomns;
	ZeroMemory(&roomns,sizeof(roomns));

	int nRoomID =*((int*)  (PBYTE(lpReqFromServer->pDataPtr) + sizeof(CONTEXT_HEAD )));
	roomns.nRoomID=nRoomID;

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
	}
	
	if (lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 		CString sIniFile=lpRoomData->GetGameSvrIniFile();

		OPENTIME_SECTION ots;
		memset(&ots,0,sizeof(ots));
		lpRoomData->GetOpenTimeSection(sIniFile,&ots);

		if (ots.nMatchStatus!=RANKMATCH_STATUS_FINISHED)//比赛尚未结束,发送报名地址
		{
			lpReqToClient->head.nRequest=GR_ROOM_NEEDSIGNUP;
 			GetPrivateProfileString(_T("SignUpURL"),xyConvertIntToStr(nRoomID),_T(""),roomns.szSignUpURL,sizeof(roomns.szSignUpURL) ,sIniFile);
		}
 		else 
		{
			lpReqToClient->head.nRequest=GR_MATCH_FINISHED;
  			GetPrivateProfileString(_T("MatchResultURL"),xyConvertIntToStr(nRoomID),_T(""),roomns.szSignUpURL,sizeof(roomns.szSignUpURL) ,sIniFile);
		}
 
		roomns.nURLLen=lstrlen(roomns.szSignUpURL)+1;
		int nLen = sizeof(ROOM_NEEDSIGNUP)-MAX_URL_LEN+roomns.nURLLen;
			
		PBYTE pData = new BYTE[nLen];
		lpReqToClient->pDataPtr = pData;
		lpReqToClient->nDataLen = nLen;
		memcpy(pData,&roomns,nLen);
	}
	else
	{
		lpReqToClient->head.nRequest=GR_ROOM_NOT_OPENED;
	}

	BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient );
	return TRUE;
} 



BOOL CBaseSockClient::OnEnterCloakingRoomOK(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClient, LPREQUEST lpReqFromServer)
{
	LPPLAYER lpPlayer = LPPLAYER(PBYTE(lpReqFromServer->pDataPtr)+ sizeof(CONTEXT_HEAD));

	if(UwlGetCurrentSeconds()-lpPlayer->pe.nEnterTime>ENTERROOM_TIMEOUT_SECONDS)
		return TRUE;

	LPROOMDATA lprd = LPROOMDATA((PBYTE)lpPlayer + sizeof(PLAYER));
	int nRoomID=lprd->nRoomID;
	int nGameID=lprd->nGameID;

	ROOM_USERS ru;
	ZeroMemory(&ru, sizeof(ru));

	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);

		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData)){
			lpRoomData = CreateRoomData();
			g_mapRoomDataEx.SetAt(nRoomID, lpRoomData);
		}
	}
	if(IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))//内部版
		lpRoomData->PostSecurityToHold(lpPlayer->nUserID,lpPlayer->pe.szVolumeID,lpContext->lTokenID,lpPlayer->nUserType);

	LPPLAYER pFind = NULL;
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 		memcpy(&(lpRoomData->roomdata), lprd, sizeof(ROOMDATA));
 		if(!lpRoomData->m_mapPlayer.Lookup(lpPlayer->nUserID, pFind))
		{
			pFind = GetPlayerBlock();
			ZeroMemory(pFind, sizeof(PLAYER));
			lpRoomData->m_mapPlayer.SetAt(lpPlayer->nUserID, pFind);
		}
		memcpy(pFind, lpPlayer, sizeof(PLAYER));
 		USER_SOCK us;
		ZeroMemory(&us,sizeof(us));
		us.sock=lpContext->hSocket;
		us.token=lpContext->lTokenID;
		us.nUserID=lpPlayer->nUserID;
		us.dwEnterFlags=lpPlayer->pe.dwEnterFlags;
		lpRoomData->m_mapUserSock.SetAt(us.nUserID,us);
 		lpRoomData->m_mapTokenUserSock.SetAt( us.token,us);		
 		pFind->pe.dwPulseTime = GetTickCount();
		pFind->pe.dwLatestTime = GetTickCount();
		pFind->pe.lTokenID=lpContext->lTokenID;
		lpPlayer->pe.lTokenID = pFind->pe.lTokenID;
  	
		
 		BOOL bLookDark=IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_LOOKDARK);
		BOOL bIsMobile=IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_HANDPHONE);
	
		if(!bIsMobile)//非手机用户重新获取IP地址
		{
			pFind->pe.dwIPAddr = g_pSockServer->GetClientAddress(lpContext->hSocket, lpContext->lTokenID);
			lpPlayer->pe.dwIPAddr = pFind->pe.dwIPAddr;
		}
 
		ENTER_ROOM_OK enterroomOK;
		ZeroMemory(&enterroomOK,sizeof(enterroomOK));
		enterroomOK.nPlayerCount = lpRoomData->m_mapPlayer.GetCount();
		enterroomOK.nTableCount=lpRoomData->roomdata.nTableCount;
		enterroomOK.nRoomTokenID=lpPlayer->pe.lTokenID;
	  	enterroomOK.nRoomPulseInterval=60;
	  	enterroomOK.dwClientIP=lpPlayer->pe.dwIPAddr;
		enterroomOK.nGiftDeposit=lpPlayer->pe.nGiftDeposit;			//进入房间获得的赠银
 	 
		int nPlayerMinSize=(sizeof(PLAYER)-sizeof(PLAYER_EXTEND));

		CString sRoomID=xyConvertIntToStr(nRoomID);
		CLOAKING_DETAIL cloakingdetail;
		ZeroMemory(&cloakingdetail,sizeof(cloakingdetail));
 		cloakingdetail.nPlayerCount=enterroomOK.nPlayerCount;
		lpRoomData->FillCloakingDetail(&cloakingdetail);
		
		if(bIsMobile && lpRoomData->IsSupportMobile() )//本房间支持手机进入
		{
			pFind->nStatus = PLAYER_STATUS_WALKAROUND;
			pFind->nTableNO = -1;
			pFind->nChairNO = -1;
			enterroomOK.dwEnterOKFlag = FLAG_ENTERROOMOK_MOBILE;
			enterroomOK.dwEnterOKFlag |= FLAG_ENTERROOMOK_CLOAKING;
			
			int nLen  = sizeof(enterroomOK) + nPlayerMinSize + sizeof(cloakingdetail);
			PBYTE pData = new BYTE[nLen];
			ZeroMemory(pData, nLen);
			memcpy(pData, &enterroomOK, sizeof(enterroomOK));
			memcpy(pData+sizeof(enterroomOK),pFind,nPlayerMinSize);
			memcpy(pData+sizeof(enterroomOK)+nPlayerMinSize,&cloakingdetail,sizeof(cloakingdetail));
			
			lpReqToClient->pDataPtr = pData;
			lpReqToClient->nDataLen = nLen;
			BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient,MSG_RESPONSE, 0, COMPRESS_ZIP);			
			NotifyDarkLooker(lpRoomData,0,GR_PLAYER_ENTERED,lpPlayer, sizeof(PLAYER));
		}
		else
		{
			if( bLookDark==FALSE) //玩家进入
			{
				enterroomOK.dwEnterOKFlag |= FLAG_ENTERROOMOK_CLOAKING;
				enterroomOK.dwEnterOKFlag|=(lpRoomData->IsSoloRoom()?FLAG_ENTERROOMOK_SOLO:0);
				enterroomOK.dwEnterOKFlag|=(lpRoomData->IsVariableChair()?FLAG_ENTERROOMOK_VARI:0);
				enterroomOK.dwEnterOKFlag|=(lpRoomData->IsRandomRoom()?FLAG_ENTERROOMOK_RANDOM:0);
				if(pFind->nTimeCost / 60  >= lpRoomData->GetUnwelcomeExperience())
					enterroomOK.dwEnterOKFlag|=(lpRoomData->IsGetUnwelcome()?FLAG_ENTERROOMOK_UNWELCOME:0);
				enterroomOK.dwEnterOKFlag|=(lpRoomData->IsGetClientInfo()?FLAG_ENTERROOMOK_CLIENTINFO:0);
				
				int nLen  = sizeof(enterroomOK) + nPlayerMinSize + sizeof(cloakingdetail);
				PBYTE pData = new BYTE[nLen];
				ZeroMemory(pData, nLen);
				memcpy(pData, &enterroomOK, sizeof(enterroomOK));
				memcpy(pData+sizeof(enterroomOK),pFind,nPlayerMinSize);
				memcpy(pData+sizeof(enterroomOK)+nPlayerMinSize,&cloakingdetail,sizeof(cloakingdetail));
				
				lpReqToClient->pDataPtr = pData;
				lpReqToClient->nDataLen = nLen;
				BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient,MSG_RESPONSE, 0, COMPRESS_ZIP);			
				NotifyDarkLooker(lpRoomData,0,GR_PLAYER_ENTERED,lpPlayer, sizeof(PLAYER));
			}
			else  //管理员进入
			{
				int nBeginRandomTableNO=lpRoomData->GetBeginRandomTableNO();
				
				int nLen=0;
				int tablecount = 0;
				int tableno = 0;
				LPTABLE pTable = NULL;
				POSITION pos  = lpRoomData->m_mapTable.GetStartPosition();
				while (pos)	{
					lpRoomData->m_mapTable.GetNextAssoc(pos, tableno, pTable);
					if(tableno>=nBeginRandomTableNO)
						continue;
					if(pTable && pTable->th.nPlayerCount){
						nLen += (pTable->th.nPlayerCount * sizeof(PLAYER_POS));
						nLen += (pTable->th.nVisitorCount * sizeof(VISITOR_POS));
						tablecount++;
					}
				}
				enterroomOK.nActiveTableCount = tablecount;
				
				nLen += sizeof(enterroomOK) + enterroomOK.nPlayerCount * nPlayerMinSize + tablecount* sizeof(TABLE_HEAD);
				PBYTE pData = new BYTE[nLen];
				ZeroMemory(pData, nLen);
				
				memcpy(pData, &enterroomOK, sizeof(enterroomOK));
				
				lpRoomData->CopyPlayerMap(pData + sizeof(enterroomOK),  bLookDark);
				lpRoomData->CopyTableMapEx(pData + sizeof(enterroomOK) + enterroomOK.nPlayerCount * nPlayerMinSize, lpPlayer); 
				
				lpReqToClient->pDataPtr = pData;
				lpReqToClient->nDataLen = nLen;
				
				BOOL bSendOK = g_pSockServer->SendResponse(lpContext->hSocket, lpContext, lpReqToClient, MSG_RESPONSE, 0, COMPRESS_ZIP);
				
				NotifyDarkLooker(lpRoomData,lpPlayer->nUserID,GR_PLAYER_ENTERED,lpPlayer, sizeof(PLAYER));
				
				lpRoomData->m_mapDarkLooker.SetAt(lpPlayer->nUserID,lpPlayer->pe.lTokenID);
			}
		}

  	    ////////////////////////////////////////////////////////////////////////////////////////////
		ru.nClientID = g_nClientID;
 		ru.nGameID = lpRoomData->GetGameID();
		ru.nAreaID = lpRoomData->GetAreaID();
		ru.nRoomID = lpRoomData->GetRoomID();
		ru.nUserCount = enterroomOK.nPlayerCount;
		ru.nUserID = lpPlayer->nUserID;
	}

 	TransmitRoomUsers(lpContext, &ru);

	return TRUE;
}

BOOL CBaseSockClient::OnUpdateUserStatus(LPREQUEST lpRequest)
{
	LPUPDATE_USER_STATUS lpuus=LPUPDATE_USER_STATUS(PBYTE(lpRequest->pDataPtr));
	if(IS_BIT_SET(lpuus->nStatus,USER_STATUS_FORBIDDEN))//帐号被禁用
	{

	}
  	return TRUE;
}
BOOL CBaseSockClient::OnKickoffHallPlayer(LPREQUEST lpRequest)
{
    LPKICKOFF_HALL_PLAYER lpkhp =LPKICKOFF_HALL_PLAYER(PBYTE(lpRequest->pDataPtr));
 
	int roomid = 0;
	CBaseRoomData* lpRoomData = NULL;
	CRoomDataExMap mapRoomDataEx;
	{
		CAutoLock lock(&g_csRoomData);	
		POSITION pos = g_mapRoomDataEx.GetStartPosition();
		while (pos)	{
			g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			mapRoomDataEx.SetAt(roomid, lpRoomData);
		}
	}
	POSITION pos = mapRoomDataEx.GetStartPosition();
	while (pos)	{
		mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
		if(lpRoomData){
			CAutoLock lock(&(lpRoomData->m_csRoomData));
			
  			CONTEXT_HEAD context;
			ZeroMemory(&context ,sizeof(context));
			context.dwFlags|=CH_FLAG_SYSTEM_EJECT;		
 	   		KickOffRoomPlayer(&context, lpRoomData,lpkhp->nPlayer,TRUE); //踢出

		}
	}
  	return TRUE;
}

BOOL CBaseSockClient::OnTakeBackDeposit(LPREQUEST lpRequest)
{
	LPTAKE_BACKDEPOSIT lpTakeBackDeposit = LPTAKE_BACKDEPOSIT(PBYTE(lpRequest->pDataPtr));
	
	CBaseRoomData* lpRoomData = NULL;
	if(lpTakeBackDeposit->nRoomID){
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpTakeBackDeposit->nRoomID, lpRoomData)){
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if(!lpRoomData->IsOldMatchRoom())
		{
			PLAYER_DEPOSIT pd;
			ZeroMemory(&pd, sizeof(pd));
			
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpTakeBackDeposit->nUserID, lpPlayer) && lpPlayer){
				if(lpRoomData->GetGameID() == lpTakeBackDeposit->nGameID){
					lpPlayer->nDeposit += lpTakeBackDeposit->nDeposit;
					pd.nUserID = lpTakeBackDeposit->nUserID;
					pd.nDeposit = lpPlayer->nDeposit;
					if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
					{
						NotifyOnePlayer(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
						NotifyDarkLooker(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
					}
					else
						NotifyRoomPlayers(lpRoomData, GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
				}
			}
		}
	}
	return TRUE;
}

BOOL CBaseSockClient::OnSaveBackDeposit(LPREQUEST lpRequest)
{
	LPTRANSFER_BACKDEPOSIT lpTransferBackDeposit = LPTRANSFER_BACKDEPOSIT(lpRequest->pDataPtr);
	
	CBaseRoomData* lpRoomData = NULL;
	if(lpTransferBackDeposit->nFromRoom > 0 ){
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpTransferBackDeposit->nFromRoom, lpRoomData)){
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if(!lpRoomData->IsOldMatchRoom())
		{
			PLAYER_DEPOSIT pd;
			ZeroMemory(&pd, sizeof(pd));
			
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpTransferBackDeposit->nUserID, lpPlayer) && lpPlayer){
				if(lpRoomData->GetGameID() == lpTransferBackDeposit->nFromGame){
					lpPlayer->nDeposit -= lpTransferBackDeposit->nDeposit;
					pd.nUserID = lpTransferBackDeposit->nUserID;
					pd.nDeposit = lpPlayer->nDeposit;
					/////////////////////////////////////////////////////
					
					if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom() )
					{
						NotifyOnePlayer(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
						NotifyDarkLooker(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
					}
					else
						NotifyRoomPlayers(lpRoomData, GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
				}
			}
		}
	}

	return TRUE;
}

BOOL CBaseSockClient::OnUserLevelUp(LPREQUEST lpRequest)
{
	LPUSER_LEVELUP lpLevelUp = LPUSER_LEVELUP(lpRequest->pDataPtr);
	
	CBaseRoomData* lpRoomData = NULL;
	if(lpLevelUp->nRoomID > 0 ){
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpLevelUp->nRoomID, lpRoomData)){
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if(!lpRoomData->IsOldMatchRoom())
		{
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpLevelUp->nUserID, lpPlayer) && lpPlayer)
			{
				lpPlayer->nGrowthLevel = lpLevelUp->nGrowthLevel;

				if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom() )
				{
					NotifyOnePlayer(lpRoomData,lpLevelUp->nUserID,GR_USER_LEVELUP, lpLevelUp, sizeof(USER_LEVELUP));
					NotifyDarkLooker(lpRoomData,lpLevelUp->nUserID,GR_USER_LEVELUP, lpLevelUp, sizeof(USER_LEVELUP));
				}
				else
					NotifyRoomPlayers(lpRoomData, GR_USER_LEVELUP, lpLevelUp, sizeof(USER_LEVELUP));
			}
		}
	}
	
	return TRUE;
}


BOOL CBaseSockClient::OnMemberLevelUp(LPREQUEST lpRequest)
{
	LPMEMBER_LEVELUP lpLevelUp = LPMEMBER_LEVELUP(PBYTE(lpRequest->pDataPtr));

	if (!lpLevelUp)
		return FALSE;

	CBaseRoomData* lpRoomData = NULL;
	if(lpLevelUp->nRoomID > 0 ){
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpLevelUp->nRoomID, lpRoomData)){
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if(!lpRoomData->IsOldMatchRoom())
		{
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpLevelUp->nUserID, lpPlayer) && lpPlayer)
			{
				lpPlayer->nUserType = lpLevelUp->nUserType;
				lpPlayer->nMemberLevel = lpLevelUp->nMemberLevel;

				if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom() )
				{
					NotifyOnePlayer(lpRoomData,lpLevelUp->nUserID,GR_MEMBER_LEVELUP, lpLevelUp, sizeof(MEMBER_LEVELUP));
					NotifyDarkLooker(lpRoomData,lpLevelUp->nUserID,GR_MEMBER_LEVELUP, lpLevelUp, sizeof(MEMBER_LEVELUP));
				}
				else
					NotifyRoomPlayers(lpRoomData, GR_MEMBER_LEVELUP, lpLevelUp, sizeof(MEMBER_LEVELUP));
			}
		}
	}
	
	return TRUE;
}


BOOL CBaseSockClient::OnMemberUpdate(LPREQUEST lpRequest)
{
	LPMEMBER_UPDATE lpMemberUpdate = LPMEMBER_UPDATE(PBYTE(lpRequest->pDataPtr));
	
	if (!lpMemberUpdate)
		return FALSE;
	
	CBaseRoomData* lpRoomData = NULL;
	if(lpMemberUpdate->nRoomID > 0 ){
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpMemberUpdate->nRoomID, lpRoomData)){
		}
	}
	if(lpRoomData){
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if(!lpRoomData->IsOldMatchRoom())
		{
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpMemberUpdate->nUserID, lpPlayer) && lpPlayer)
			{
				lpPlayer->nUserType = lpMemberUpdate->nUserType;
				lpPlayer->nMemberLevel = lpMemberUpdate->nMemberLevel;
				
				if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom() )
				{
					NotifyOnePlayer(lpRoomData,lpMemberUpdate->nUserID,GR_MEMBER_UPDATE, lpMemberUpdate, sizeof(MEMBER_UPDATE));
					NotifyDarkLooker(lpRoomData,lpMemberUpdate->nUserID,GR_MEMBER_UPDATE, lpMemberUpdate, sizeof(MEMBER_UPDATE));
				}
				else
					NotifyRoomPlayers(lpRoomData, GR_MEMBER_UPDATE, lpMemberUpdate, sizeof(MEMBER_LEVELUP));
			}
		}
	}
	
	return TRUE;
}


BOOL CBaseSockClient::OnWriteKickManProfile(LPREQUEST lpRequest)
{
	LPWRITE_PROFILE lpWriteProfile = LPWRITE_PROFILE(PBYTE(lpRequest->pDataPtr));
	if(IS_BIT_SET(lpWriteProfile->dwFlags,FLAG_WRITEPROFILE_DELKEY))
		WritePrivateProfileString(lpWriteProfile->pi.szSection,	lpWriteProfile->pi.szKeyName,NULL,g_szKickManFile);
	else if(IS_BIT_SET(lpWriteProfile->dwFlags,FLAG_WRITEPROFILE_DELSEC))
		WritePrivateProfileString(lpWriteProfile->pi.szSection,	NULL,NULL,g_szKickManFile);
	else
		WritePrivateProfileString(lpWriteProfile->pi.szSection,	lpWriteProfile->pi.szKeyName,lpWriteProfile->pi.szString,g_szKickManFile);
	
	return TRUE;
}
BOOL CBaseSockClient::OnUserPayResult(LPREQUEST lpRequest)
{
	LPPAY_RESULT lpPay = LPPAY_RESULT(PBYTE(lpRequest->pDataPtr));
	
	if (!lpPay)
		return FALSE;
	
	if (lpRequest->nDataLen != sizeof(PAY_RESULT))
		return FALSE;
	
	CBaseRoomData* lpRoomData = NULL;
	if(lpPay->nRoomID > 0 )
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpPay->nRoomID, lpRoomData))
		{
			return FALSE;
		}
	}
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if(!lpRoomData->IsOldMatchRoom())
		{
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpPay->nUserID, lpPlayer) && lpPlayer)
			{
				if(lpPay->nPayTo == PAY_TO_GAME && lpRoomData->GetGameID() == lpPay->nGameID)// 充值到游戏
				{
					if(lpPay->nPayFor == PAY_FOR_DEPOSIT ) // for 银子
					{
						lpPlayer->nDeposit = lpPay->llBalance;
						
						PLAYER_DEPOSIT pd;
						ZeroMemory(&pd, sizeof(pd));
						pd.nUserID = lpPay->nUserID;
						pd.nDeposit = lpPlayer->nDeposit;
						if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
						{
							NotifyOnePlayer(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
							NotifyDarkLooker(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
						}
						else
							NotifyRoomPlayers(lpRoomData, GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
					}
					else if(lpPay->nPayFor == PAY_FOR_SCORE)// for 积分
					{
						lpPlayer->nScore = lpPay->llBalance;
						
						PLAYER_SCORE pd;
						ZeroMemory(&pd, sizeof(pd));
						pd.nUserID = lpPay->nUserID;
						pd.nScore = lpPlayer->nScore;
						if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
						{
							NotifyOnePlayer(lpRoomData,pd.nUserID,GR_PLAYER_SCORE, &pd, sizeof(pd));
							NotifyDarkLooker(lpRoomData,pd.nUserID,GR_PLAYER_SCORE, &pd, sizeof(pd));
						}
						else
							NotifyRoomPlayers(lpRoomData, GR_PLAYER_SCORE, &pd, sizeof(pd));
					}
					// 暂时注释
					// 					if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom() )
					// 					{
					// 						NotifyOnePlayer(lpRoomData,lpPay->nUserID,GR_USER_PAY_RESULT, lpPay, sizeof(PAY_RESULT));
					// 						NotifyDarkLooker(lpRoomData,lpPay->nUserID,GR_USER_PAY_RESULT, lpPay, sizeof(PAY_RESULT));
					// 					}
					// 					else
					// 						NotifyRoomPlayers(lpRoomData, GR_USER_PAY_RESULT, lpPay, sizeof(PAY_RESULT));
					
				}
			}
		}
	}
	
	return TRUE;
}
BOOL CBaseSockClient::OnUserCurrencyExchange(LPREQUEST lpRequest)
{
	LPCURRENCY_EXCHANGE_EX lpExchange = LPCURRENCY_EXCHANGE_EX(PBYTE(lpRequest->pDataPtr));
	if (!lpExchange)
		return FALSE;
	
	if (lpRequest->nDataLen != sizeof(CURRENCY_EXCHANGE_EX))
		return FALSE;
	
	CBaseRoomData* lpRoomData = NULL;
	if(lpExchange->nEnterRoomID > 0 )
	{
		CAutoLock lock(&g_csRoomData);
		if(!g_mapRoomDataEx.Lookup(lpExchange->nEnterRoomID, lpRoomData))
		{
			return FALSE;
		}
	}
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if(!lpRoomData->IsOldMatchRoom())
		{
			BOOL bExchangeToGame	 = (TCY_CURRENCY_CONTAINER_GAME == lpExchange->currencyExchange.nContainer);// 交易到游戏
			BOOL bExchangeForDeposit = (TCY_CURRENCY_DEPOSIT == lpExchange->currencyExchange.nCurrency); // 交易成银子
			BOOL bExchangeForScore   = (TCY_CURRENCY_SCORE == lpExchange->currencyExchange.nCurrency);	 // 交易成积分
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(lpExchange->currencyExchange.nUserID, lpPlayer) && lpPlayer)
			{
				if(bExchangeToGame && lpRoomData->GetGameID() == lpExchange->currencyExchange.nExchangeGameID)
				{
					if(bExchangeForDeposit ) // for 银子
					{
						lpPlayer->nDeposit = lpExchange->currencyExchange.llBalance;
						
						PLAYER_DEPOSIT pd;
						ZeroMemory(&pd, sizeof(pd));
						pd.nUserID = lpExchange->currencyExchange.nUserID;
						pd.nDeposit = lpPlayer->nDeposit;
						if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
						{
							NotifyOnePlayer(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
							NotifyDarkLooker(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
						}
						else
							NotifyRoomPlayers(lpRoomData, GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
					}
					else if(bExchangeForScore)// for 积分
					{
						lpPlayer->nScore = lpExchange->currencyExchange.llBalance;
						
						PLAYER_SCORE pd;
						ZeroMemory(&pd, sizeof(pd));
						pd.nUserID = lpExchange->currencyExchange.nUserID;
						pd.nScore = lpPlayer->nScore;
						if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
						{
							NotifyOnePlayer(lpRoomData,pd.nUserID,GR_PLAYER_SCORE, &pd, sizeof(pd));
							NotifyDarkLooker(lpRoomData,pd.nUserID,GR_PLAYER_SCORE, &pd, sizeof(pd));
						}
						else
							NotifyRoomPlayers(lpRoomData, GR_PLAYER_SCORE, &pd, sizeof(pd));
					}
				
				}
			}
		}
	}
	return TRUE;
}
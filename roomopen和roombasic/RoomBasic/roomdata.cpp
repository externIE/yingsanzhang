#include "roombasic.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 
#define MAX_TABLEDEPOSIT_COUNT	32

CBaseRoomData::~CBaseRoomData()
{
    ClearUnwelcomesMap();
	ClearClientInfoMap();
	ClearQuanDataMap();
 	SAFE_DELETE(m_pTcMatch);
 
}
int  CBaseRoomData::GetAreaID()
{
	return roomdata.nAreaID;
}

int  CBaseRoomData::GetGameID()
{
	return roomdata.nGameID;

}
int  CBaseRoomData::GetRoomID()
{
	return roomdata.nRoomID;

}
int  CBaseRoomData::GetMatchID()
{
	return roomdata.nMatchID;
}
int  CBaseRoomData::GetChairCount()
{
	return roomdata.nChairCount;
}
int  CBaseRoomData::GetTableCount()
{
	return roomdata.nTableCount;
}

int  CBaseRoomData::GetPlayingCount(TABLE* pTable)
{
	int nRet=0;

	if(pTable && pTable->th.nPlayerCount>0)
	{
		for(int i = 0; i < MAX_CHAIR_COUNT; i++)
		{
			if(pTable->nPlayerAry[i])
			{
				LPPLAYER pPlayer = NULL;
				if(m_mapPlayer.Lookup(pTable->nPlayerAry[i], pPlayer))
				{
					if(pPlayer)
					{
 						if(pPlayer->nStatus ==PLAYER_STATUS_PLAYING)
							nRet++;
					}
				}
			}
		}
	}

	return nRet;
}

DWORD CBaseRoomData::GetTotalPlayingBreak(TABLE* pTable)
{
	DWORD dwRet=0;

	if(pTable)
	{
		for(int i = 0; i < MAX_CHAIR_COUNT; i++)
		{
			if(pTable->nPlayerAry[i])
			{
				LPPLAYER pPlayer = NULL;
				if(m_mapPlayer.Lookup(pTable->nPlayerAry[i], pPlayer))
				{
					if(pPlayer)
					{
 						if(pPlayer->nStatus ==PLAYER_STATUS_PLAYING)
							dwRet|= (1 <<  pPlayer->nChairNO ) ;
					}
				}
			}
		}
	}

	return dwRet;

}
int CBaseRoomData::GetShowPlayerCount()
{
	int nRet=0;
 	int userid = 0;
	LPPLAYER pPlayer = NULL;
 	POSITION pos = m_mapPlayer.GetStartPosition();
	while (pos)
	{
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if(pPlayer)
		{
			if(!IS_BIT_SET(pPlayer->nUserType,USER_TYPE_ENTERHIDING))
				nRet++;
		}
	}
	return nRet;
}
void CBaseRoomData::ClearUnwelcomesMap()
{
	POSITION  pos= m_mapUnwelcomes.GetStartPosition();
	while (pos != NULL)
	{
		int  nUserID; 
		CIDMap *   pIDMap;
		m_mapUnwelcomes.GetNextAssoc(pos, nUserID, pIDMap); 
		pIDMap->RemoveAll();
		SAFE_DELETE(pIDMap);
	}
	m_mapUnwelcomes.RemoveAll();
}

void CBaseRoomData::ClearClientInfoMap()
{
	POSITION  pos= m_mapClientInfo.GetStartPosition();
	while (pos != NULL)
	{
		int  nUserID; 
		CClientInfo *   pInfo;
		m_mapClientInfo.GetNextAssoc(pos, nUserID, pInfo); 
 		SAFE_DELETE(pInfo);
	}
	m_mapClientInfo.RemoveAll();
}

int CBaseRoomData::CopyPlayerMap(PBYTE pData,  BOOL bLookDark)
{
	int  nRet=0;

	BOOL bIsRandom=IsRandomRoom();
	int nSizeofPlayer=sizeof(PLAYER)-sizeof(PLAYER_EXTEND);

	int userid = 0;
	LPPLAYER pPlayer = NULL;
	LPPLAYER lpPlayerData = LPPLAYER(pData);
	POSITION pos;
	pos  = m_mapPlayer.GetStartPosition();
	while (pos)	{
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if(pPlayer){
			memcpy(lpPlayerData, pPlayer,nSizeofPlayer);
			if(!bLookDark && bIsRandom && (lpPlayerData->nStatus ==PLAYER_STATUS_WAITING))
				lpPlayerData->nStatus=PLAYER_STATUS_SEATED;
			lpPlayerData = (LPPLAYER)((PBYTE)(lpPlayerData)+nSizeofPlayer) ;

			if(!IS_BIT_SET(pPlayer->nUserType,USER_TYPE_ENTERHIDING))
				nRet++;
		}
	}
	return nRet;
}

void CBaseRoomData::CopyMatchPlayerSmall(MATCHPLAYER_SMALL* pms,PLAYER* pPlayer)
{
	pms->nUserID =pPlayer->nUserID ;
	pms->nStatus =pPlayer->nStatus ;
	pms->nTableNO =pPlayer->nTableNO ;
	pms->nChairNO =pPlayer->nChairNO ;
	pms->nDeposit =pPlayer->nDeposit ;

	pms->nScore =pPlayer->nScore ;
	pms->nBreakOff =pPlayer->nBreakOff ;
	pms->nWin =pPlayer->nWin ;

	pms->nLoss =pPlayer->nLoss ;
	pms->nStandOff =pPlayer->nStandOff ;
	pms->nBout =pPlayer->nBout ;
	pms->nTimeCost =pPlayer->nTimeCost ;

	pms->nDuan=pPlayer->pe.nDuan;
	pms->nDuanScore=pPlayer->pe.nDuanScore;
	pms->nRank=pPlayer->pe.nRank;

	pms->dwIPAddr =pPlayer->pe.dwIPAddr;
	lstrcpy(pms->szUsername,pPlayer->szUsername);
	pms->dwSysVer=pPlayer->pe.dwSysVer;
	pms->dwScreenXY=pPlayer->pe.dwScreenXY;

}



void CBaseRoomData::CopyPlayerPrivInfo(PLAYER_PRIVINFO* ppp,PLAYER* pPlayer)
{
	ppp->nUserID =pPlayer->nUserID ;
 	ppp->nAgentGroupID =pPlayer->pe.nAgentGroupID;
	ppp->nLogonSvrID =pPlayer->pe.nLogonSvrID;
	ppp->dwIPAddr =pPlayer->pe.dwIPAddr;
	ppp->dwSysVer =pPlayer->pe.dwSysVer;
	ppp->dwScreenXY =pPlayer->pe.dwScreenXY;
	ppp->dwPixelsXY =pPlayer->pe.dwPixelsXY;
	ppp->nWaitTime =pPlayer->pe.nEnterTime;

}


void CBaseRoomData::CopySoloPlayer(SOLO_PLAYER* psp,PLAYER* pPlayer)
{
	psp->nUserID=	pPlayer->nUserID;
	psp->nUserType=pPlayer->nUserType;
	psp->nStatus=	pPlayer->nStatus;
	psp->nTableNO=pPlayer->nTableNO;
	psp->nChairNO=pPlayer->nChairNO;
	psp->nNickSex=pPlayer->nNickSex;
	psp->nPortrait=pPlayer->nPortrait;
	psp->nNetSpeed=pPlayer->nNetSpeed;
	psp->nClothingID=pPlayer->nClothingID;
	psp->nDeposit=pPlayer->nDeposit;
	psp->nPlayerLevel=pPlayer->nPlayerLevel;
	psp->nScore=pPlayer->nScore;
	psp->nBreakOff=pPlayer->nBreakOff;
	psp->nWin=pPlayer->nWin;
	psp->nLoss=pPlayer->nLoss;
	psp->nStandOff=pPlayer->nStandOff;
	psp->nBout=pPlayer->nBout;
	psp->nTimeCost=pPlayer->nTimeCost;

	lstrcpy(psp->szUsername,pPlayer->szUsername);
	lstrcpy(psp->szNickName,pPlayer->szNickName);

}

void CBaseRoomData::CopyMatchPlayerMap(PBYTE pData)
{
   	int userid = 0;
	LPPLAYER pPlayer = NULL;
	MATCHPLAYER_SMALL * pms = (MATCHPLAYER_SMALL*)pData;
	POSITION pos;
	pos  = m_mapPlayer.GetStartPosition();
	while (pos)	
	{
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if(pPlayer)
		{ 
 			CopyMatchPlayerSmall(pms,pPlayer);
   			pms ++ ;
		}
	}
}

int CBaseRoomData::GetBeginRandomTableNO()
{
	return BEGIN_RANDOM_TABLENO_EX;
}

int CBaseRoomData::GetFreeChairNO(TABLE* pTable)
{
	int nMode= xyGetRandomDigit(1);
	if(nMode==0)
	{
  		for( int i=0;i<MAX_CHAIR_COUNT;i++)
		{
			if(pTable->nPlayerAry[i]==0)
				return i;
		}
	}
	else
	{
  		for( int i=roomdata.nChairCount-1;i>=0;i--)
		{
			if(pTable->nPlayerAry[i]==0)
				return i;
		}
	}
	return 0;
}

int CBaseRoomData::GetFreeTableNO( int nFromTableNO)
{	
	int nBeginRandomTableNO=GetBeginRandomTableNO();

 	for(int i=nFromTableNO ;i<nBeginRandomTableNO;i++)
	{	
		LPTABLE lpTable = NULL;
 		if(m_mapTable.Lookup(i,lpTable))
		{
			if(lpTable->th.nStatus==TABLE_STATUS_STATIC && lpTable->th.nPlayerCount==0)
				return  i;
		}
		else
		{
			lpTable = new TABLE;
			ZeroMemory(lpTable, sizeof(TABLE));
			lpTable->th.nTableNO = i;
			lpTable->th.nMinDeposit=0;
			lpTable->th.nMinScore =SCORE_MIN;
			m_mapTable.SetAt(i, lpTable);
			return i;
		}
	}
	return nFromTableNO;
}
BOOL CBaseRoomData::GetFreeChairNO2(int& nTableNO,int& nChairNO)
{
 	for(int i=0 ;i<roomdata.nTableCount;i++)
	{	
		LPTABLE lpTable = NULL;
 		if(m_mapTable.Lookup(i,lpTable))
		{
			if(lpTable->th.nStatus==TABLE_STATUS_STATIC)
			{
 				for( int j=0;j<roomdata.nChairCount;j++)
				{
					if(lpTable->nPlayerAry[j]==0)
					{
						nTableNO=i;
						nChairNO=j;
						return TRUE;
					}
				}
			}
		}
		else
		{
			lpTable = new TABLE;
			ZeroMemory(lpTable, sizeof(TABLE));
			lpTable->th.nTableNO = i;
			lpTable->th.nMinDeposit=0;
			lpTable->th.nMinScore =SCORE_MIN;
			m_mapTable.SetAt(i, lpTable);
				
			nTableNO=i;
			nChairNO=0;
  			return TRUE;
		}
	}
	return FALSE;
}

int CBaseRoomData::IsLessExperienceOnTable(int nExperience,int nTableNO)
{
	int nRet=0;
	TABLE* pTable=NULL;
	if(!m_mapTable.Lookup(nTableNO,pTable))
		return nRet;
	if(pTable->th.nPlayerCount==0)
		return nRet;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
  		if(0 == pTable->nPlayerAry[i]  )
			 continue;
         PLAYER* pPlayer=NULL;
		 m_mapPlayer.Lookup(pTable->nPlayerAry[i],pPlayer);
		 if(pPlayer==NULL)
			 continue;
		 if( (pPlayer->nTimeCost / 60 ) < nExperience)
			 nRet++;
	}
	return nRet;
}

BOOL CBaseRoomData::IsUnwelcomesOnTable(int nUserID,int nTableNO)
{
	int nRet=0;
	TABLE* pTable=NULL;
	if(!m_mapTable.Lookup(nTableNO,pTable))
		return nRet;
	if(pTable->th.nPlayerCount==0)
		return nRet;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
 		if(0 == pTable->nPlayerAry[i] || pTable->nPlayerAry[i]==nUserID)
			continue;
		if(IsUnwelcomePlayer(nUserID,pTable->nPlayerAry[i]))
		    nRet++;
		if(IsUnwelcomePlayer(pTable->nPlayerAry[i],nUserID))
		    nRet++;
	}
	return nRet;
}

BOOL CBaseRoomData::IsAnyOneUnwelcomeYouOnTable(int nUserID,int nTableNO)
{
	int nRet=0;
	TABLE* pTable=NULL;
	if(!m_mapTable.Lookup(nTableNO,pTable))
		return nRet;
	if(pTable->th.nPlayerCount==0)
		return nRet;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
		if(0 == pTable->nPlayerAry[i] || pTable->nPlayerAry[i]==nUserID)
			continue;
		if(IsUnwelcomePlayer(pTable->nPlayerAry[i],nUserID))
			nRet++;
	}
	return nRet;
}

BOOL CBaseRoomData::IsSameTimeOnTable( int nMillisecond,int nTableNO ) 
{   
 	TABLE* pTable=NULL;
	if(!m_mapTable.Lookup(nTableNO,pTable))
		return FALSE;
	if(pTable->th.nPlayerCount==0)
		return FALSE;
	if(GetTickCount()-pTable->nLatestStartTime >nMillisecond)
		return FALSE;
    return TRUE;
}
int CBaseRoomData::IsSamePwdOnTable( DWORD dwPwdCode,int nTableNO)
{
	int nRet=0;
	TABLE* pTable=NULL;
	if(!m_mapTable.Lookup(nTableNO,pTable))
		return nRet;
	if(pTable->th.nPlayerCount==0)
		return nRet;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
 		if(0 == pTable->nPlayerAry[i]  )
			 continue;
         PLAYER* pPlayer=NULL;
		 m_mapPlayer.Lookup(pTable->nPlayerAry[i],pPlayer);
		 if(pPlayer==NULL)
			 continue;
		 if( pPlayer->pe.dwPwdCode==dwPwdCode)
			 nRet++;
	}
	return nRet;
}
int CBaseRoomData::IsSameSeatedOnTable(int nSeatedTableNO,int nTableNO)
{
	int nRet=0;
	TABLE* pTable=NULL;
	if(!m_mapTable.Lookup(nTableNO,pTable))
		return nRet;
	if(pTable->th.nPlayerCount==0)
		return nRet;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
 		if(0 == pTable->nPlayerAry[i]  )
			 continue;
         PLAYER* pPlayer=NULL;
		 m_mapPlayer.Lookup(pTable->nPlayerAry[i],pPlayer);
		 if(pPlayer==NULL)
			 continue;
		 if( pPlayer->nTableNO==nSeatedTableNO)
			 nRet++;
	}
	return nRet;
}
BOOL CBaseRoomData::IsMutexHardOnTable(int nUserID,LPCTSTR lpszMachineID,TABLE* pTable,int& nExpiration)
{
	if(!IsMutexHardRoom())
		return FALSE;
	if(pTable->th.nPlayerCount==0)
		return FALSE;
	
	CAutoLock lock(&g_csMutexHard);			
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
		if(0 == pTable->nPlayerAry[i] || pTable->nPlayerAry[i] ==  nUserID)
			continue;
		LPPLAYER pPlayer = NULL;
		m_mapPlayer.Lookup(pTable->nPlayerAry[i], pPlayer);
		if(pPlayer==NULL)
			continue;
		
		if(IsMutexHard(lpszMachineID,pPlayer->pe.szMachineID,nExpiration))
			return TRUE;
		
	}
	return FALSE;
}
BOOL CBaseRoomData::IsSameVolumeIDOnTable(LPCTSTR lpszVolumeID,int nTableNO)
{
 	TABLE* pTable=NULL;
	if(!m_mapTable.Lookup(nTableNO,pTable))
		return FALSE;
	if(pTable->th.nPlayerCount==0)
		return FALSE;
	TCHAR szVolumeID1[MAX_HARDID_LEN];
	ZeroMemory(szVolumeID1,sizeof(szVolumeID1));
	lstrcpyn(szVolumeID1,lpszVolumeID,8);
		
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
 		if(0 == pTable->nPlayerAry[i]  )
			 continue;
         PLAYER* pPlayer=NULL;
		 m_mapPlayer.Lookup(pTable->nPlayerAry[i],pPlayer);
		 if(pPlayer==NULL)
			 continue;
       	TCHAR szVolumeID2[MAX_HARDID_LEN];
		ZeroMemory(szVolumeID2,sizeof(szVolumeID2));
		lstrcpyn(szVolumeID2,pPlayer->pe.szVolumeID,8);

		if(lstrcmp(szVolumeID1,szVolumeID2)==0)
			return TRUE;

	}
	return FALSE;
}
BOOL CBaseRoomData::IsMutexHardOnTable(LPCTSTR lpszMachineID,int nTableNO)
{
	TABLE* pTable=NULL;
	if(!m_mapTable.Lookup(nTableNO,pTable))
		return FALSE;
	if(pTable->th.nPlayerCount==0)
		return FALSE;
	int nExpiration=0;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
 		if(0 == pTable->nPlayerAry[i]  )
			continue;
         PLAYER* pPlayer=NULL;
		 m_mapPlayer.Lookup(pTable->nPlayerAry[i],pPlayer);
		 if(pPlayer==NULL)
			continue;
		 if(IsMutexHard(lpszMachineID,pPlayer->pe.szMachineID,nExpiration))
			 return TRUE;
	}
	return FALSE;
}
BOOL CBaseRoomData::IsSameIPOnTable( DWORD dwIPAddr,int nTableNO)
{
	int nIPForbid=0;
	int nIPConfig=0;
 	if(IS_BIT_SET(roomdata.dwOptions,ROOM_OPT_FORBIDSAMEIP)) 
 		nIPConfig = 4; 
  	if(IS_BIT_SET(roomdata.dwOptions,ROOM_OPT_FORBIDSAME3IP)) 
  		nIPConfig = 3; 
  	if(IS_BIT_SET(roomdata.dwOptions,ROOM_OPT_FORBIDSAME2IP)) 
  		nIPConfig = 2; 
	if(nIPConfig)
	{
		TABLE* pTable=NULL;
		if(!m_mapTable.Lookup(nTableNO,pTable))
			return nIPForbid;
		if(pTable->th.nPlayerCount==0)
	    	return nIPForbid;

		for(int i = 0; i < MAX_CHAIR_COUNT; i++)
		{
			if(nIPForbid>0)
				break;
			if(0 == pTable->nPlayerAry[i]  )
				continue;
			if(UwlIsSameIPAddr(pTable->dwIPAddrs[i], dwIPAddr, nIPConfig))
			{
				nIPForbid=1;
				break;
			}
			for(int j = 0; j < MAX_CHAIR_COUNT; j++)
			{
				if(j == i || 0 == pTable->nPlayerAry[j]  ) 
					continue;
				if(UwlIsSameIPAddr(pTable->dwIPAddrs[i], pTable->dwIPAddrs[j], nIPConfig))
				{
					nIPForbid=2;		
					break;
				}
			}
		}
	}
  
	return nIPForbid;

}
BOOL CBaseRoomData::IsForbidMinDepositOnTable(PLAYER* pPlayer,TABLE* pTable,int nMinDeposit)
{
  	 if(0 == pTable->th.nPlayerCount)	// 第一个入座用户可设
	 {	
 		pTable->th.nMinDeposit = nMinDeposit;
	 }
	 else
	 {	

		if(1 == pTable->th.nPlayerCount)
		{
			if(FindPlayerOnTable(pTable,pPlayer->nUserID) &&  (pTable->th.nFirstSeatedPlayer == pPlayer->nUserID))
			{
 				pTable->th.nMinDeposit = nMinDeposit;
			}

		}

 		if(pPlayer->nUserID ==pTable->th.nFirstSeatedPlayer)
			return FALSE;

    	if( pPlayer->nDeposit < pTable->th.nMinDeposit)
          return TRUE;	
	 }
	return FALSE;
}



BOOL CBaseRoomData::IsForbidMinScoreOnTable(PLAYER* pPlayer,TABLE* pTable,int nMinScore)
{
	if(0 == pTable->th.nPlayerCount)	// 第一个入座用户可设
	{	
  		pTable->th.nMinScore = nMinScore;
	}
	else
	{	
		if(1 == pTable->th.nPlayerCount)//桌子上只有自己，并且自己是第一个入座的，允许重设置
		{
 			if(FindPlayerOnTable(pTable,pPlayer->nUserID) &&  (pTable->th.nFirstSeatedPlayer == pPlayer->nUserID))
			{	
 				pTable->th.nMinScore = nMinScore;
			}

		}
  			
 		if(pPlayer->nUserID ==pTable->th.nFirstSeatedPlayer)
			return FALSE;

    	if( pPlayer->nScore < pTable->th.nMinScore)
           return TRUE;	
	}
	return FALSE;
}

BOOL CBaseRoomData::IsForbidPrivChatByExp(int nExperience,int& nMinExp)
{
	if(IS_BIT_SET(roomdata.dwManages, ROOM_MNG_FORBIDPRIVCHAT_BYEXP))
	{
		nMinExp = GetPrivateProfileInt(_T("ChatAction"),_T("MinExperience"),MIN_CHAT_EXPERIENCE, g_szIniFile);  
	 	if(nExperience < nMinExp)
	 		return TRUE;
	}
	return FALSE;
}



BOOL CBaseRoomData::IsForbidRoomChatByExp(int nExperience,int& nMinExp)
{
	if(IS_BIT_SET(roomdata.dwManages, ROOM_MNG_FORBIDROOMCHAT_BYEXP))
	{
		nMinExp = GetPrivateProfileInt(_T("ChatAction"),_T("MinExperience"),MIN_CHAT_EXPERIENCE, g_szIniFile);  
		if(nExperience  < nMinExp)
			return TRUE;
	}
	return FALSE;
}

BOOL CBaseRoomData::IsForbidBreakOnTable(int nUserID,TABLE* pTable,float fBreakOff)
{
	int nBreakReq = 0;
 	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
		if(0 == pTable->nPlayerAry[i] || pTable->nPlayerAry[i] == nUserID)
			continue;
		if(pTable->nBreakReqs[i] >0)
		{
			if(0 == nBreakReq || pTable->nBreakReqs[i] < nBreakReq)
 				nBreakReq = pTable->nBreakReqs[i];
		}
	}
  	if( (nBreakReq>0) && (fBreakOff > nBreakReq)) //设置了断线率
 		return nBreakReq;

	return FALSE;
}
BOOL CBaseRoomData::IsForbidSpeedOnTable(int nUserID,TABLE* pTable,int nNetSpeed)
{
	int nSpeedReq = 0;
 	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
		if(0 == pTable->nPlayerAry[i] || pTable->nPlayerAry[i] == nUserID)
			continue;
		if(pTable->nSpeedReqs[i]>0)
		{
			if(0 == nSpeedReq || pTable->nSpeedReqs[i] < nSpeedReq)
 				nSpeedReq = pTable->nSpeedReqs[i];
		}
	}
	if((nSpeedReq >0) &&  (nNetSpeed > nSpeedReq))//设置了网速
 		return nSpeedReq;

    return FALSE; 
}
BOOL CBaseRoomData::IsForbidIPOnTable(int nUserID,TABLE* pTable,DWORD dwIPAddr,int nIPConfig)
{
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)	// 检查IP配置是否允许
	{
		if(0 == pTable->nPlayerAry[i] || pTable->nPlayerAry[i] ==  nUserID) 
			continue;
		if(pTable->nIPConfigs[i])
		{
			if(0 == nIPConfig || pTable->nIPConfigs[i] < nIPConfig)
			{
				nIPConfig = pTable->nIPConfigs[i];
			}
		}
	}
			
	if(IS_BIT_SET(roomdata.dwOptions,ROOM_OPT_FORBIDSAMEIP)) // 该房间禁止IP相同的人一桌
	{
		if(0 == nIPConfig)
			nIPConfig = 4; 
	}	
	if(IS_BIT_SET(roomdata.dwOptions,ROOM_OPT_FORBIDSAME3IP))  	// 该房间禁止IP前3位相同的人一桌
   		nIPConfig = 3; 
 	if(IS_BIT_SET(roomdata.dwOptions,ROOM_OPT_FORBIDSAME2IP)) 
  		nIPConfig = 2; 
 	///////////////////////////////////////////////////////////////////////////////////////
	if(nIPConfig)
	{
		for(int i = 0; i < MAX_CHAIR_COUNT; i++)
		{
			if(0 == pTable->nPlayerAry[i] || pTable->nPlayerAry[i] == nUserID)
				continue;
			if(UwlIsSameIPAddr(pTable->dwIPAddrs[i], dwIPAddr, nIPConfig))
				return TRUE;
			for(int j = 0; j < MAX_CHAIR_COUNT; j++)
			{
				if(j == i || 0 == pTable->nPlayerAry[j] || pTable->nPlayerAry[j] ==nUserID) 
					continue;
				if(UwlIsSameIPAddr(pTable->dwIPAddrs[i], pTable->dwIPAddrs[j], nIPConfig))
					return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CBaseRoomData::IsMutexIPOnTable(int nUserID,TABLE* pTable,DWORD dwIPAddr)
{
	if(pTable->th.nPlayerCount==0)
		return FALSE;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
 		if(0 == pTable->nPlayerAry[i] || pTable->nPlayerAry[i] == nUserID)
			continue;
		if(IsMutexIP(pTable->dwIPAddrs[i], dwIPAddr,2))
            return TRUE;	 
	}
  
	return FALSE;

}

BOOL CBaseRoomData::IsSameClubOnTable( int nClub,TABLE* pTable)
{
	if(pTable->th.nPlayerCount==0)
		return FALSE;

	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
 		if(0 == pTable->nPlayerAry[i])
			continue;

         PLAYER* pPlayer=NULL;
		 m_mapPlayer.Lookup(pTable->nPlayerAry[i],pPlayer);

		 if(pPlayer==NULL)
			 continue;

		 if(pPlayer->pe.nRank == nClub) 
			 return TRUE;
	}
	
	return FALSE;
}


BOOL CBaseRoomData::IsMutexIPGroupOnTable(int nUserID,TABLE* pTable,DWORD dwIPAddr)
{
	if(pTable->th.nPlayerCount==0)
		return FALSE;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
 		if(0 == pTable->nPlayerAry[i] || pTable->nPlayerAry[i] == nUserID)
			continue;
		if(IsMutexIPGroup(pTable->dwIPAddrs[i], dwIPAddr,2))
            return TRUE;	 
	}
  
	return FALSE;

}

BOOL CBaseRoomData::IsSameLANOnTable(int nUserID,TABLE* pTable)
{	
	if(!IS_BIT_SET( roomdata.dwOptions,ROOM_OPT_FORBIDSAMELAN))
		return FALSE;
	if(pTable->th.nPlayerCount==0)
		return FALSE;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
 		if(0 == pTable->nPlayerAry[i] || pTable->nPlayerAry[i] == nUserID )
			continue;
		if(IsSameLAN(nUserID,pTable->nPlayerAry[i]))
            return TRUE;	 
	}
	return FALSE;

}
int CBaseRoomData::GetPreTablesCount(int nPreTables[])
{
	int nRet=0;
	for(int i=0;i<MAX_PRETABLES;i++)
	{
		if(nPreTables[i]==0)
			nRet++;
	}
	return nRet;
}

void CBaseRoomData::ForbidSameIP(int nPreTables[],DWORD dwIPAddr, int nFromTableNO)
{
	for(int i=0;i<MAX_PRETABLES;i++)
	{
		if(nPreTables[i]==1)
			continue;
		
		if(IsSameIPOnTable( dwIPAddr, nFromTableNO+i))
			nPreTables[i]=1;
		
	}
  
}

void CBaseRoomData::ForbidSamePwd(int nPreTables[],DWORD dwPwdCode, int nFromTableNO)
{
	if(!IsMutexHardRoom())  
		return ;
	for(int i=0;i<MAX_PRETABLES;i++)
	{
		if(nPreTables[i]==1)
			continue;
		
		if(IsSamePwdOnTable( dwPwdCode, nFromTableNO+i))
			nPreTables[i]=1;
		
	}
  
}
void CBaseRoomData::ForbidSmallExperience(int nPreTables[],int  nExperience, int nFromTableNO)
{
	if(roomdata.nChairCount< 4 )
		return ;

	int nSmallExperience = GetPrivateProfileInt(_T("SmallExperience"),xyConvertIntToStr(roomdata.nRoomID ),200, g_szIniFile);
	if(nExperience >= nSmallExperience)
		return ;

	for(int i=0;i<MAX_PRETABLES;i++)
	{
		if(nPreTables[i]==1)
			continue;
		
		if(IsLessExperienceOnTable( nSmallExperience, nFromTableNO+i) >=2 )
			nPreTables[i]=1;
	}
  
}
void CBaseRoomData::ForbidMicroExperience(int nPreTables[],int  nExperience, int nFromTableNO)
{
	int nMicroExperience = GetPrivateProfileInt(_T("MicroExperience"),xyConvertIntToStr(roomdata.nRoomID ),100, g_szIniFile);
	if(nExperience >= nMicroExperience)
		return ;

	for(int i=0;i<MAX_PRETABLES;i++)
	{
		if(nPreTables[i]==1)
			continue;
		
		if(IsLessExperienceOnTable( nMicroExperience, nFromTableNO+i))
			nPreTables[i]=1;
	}
  
}
void CBaseRoomData::ForbidMutexIP(int nPreTables[],DWORD dwIPAddr, int nFromTableNO)
{
	for(int i=0;i<MAX_PRETABLES;i++)
	{
		if(nPreTables[i]==1)
			continue;
		 
		TABLE* pTable=NULL;
		if(!m_mapTable.Lookup(nFromTableNO+i,pTable))
			continue;

		if(IsMutexIPOnTable(0,pTable,dwIPAddr))
			nPreTables[i]=1;
		
	}
}

void CBaseRoomData::ForbidMutexIPGroup(int nPreTables[],DWORD dwIPAddr, int nFromTableNO)
{
	for(int i=0;i<MAX_PRETABLES;i++)
	{
		if(nPreTables[i]==1)
			continue;
		 
		TABLE* pTable=NULL;
		if(!m_mapTable.Lookup(nFromTableNO+i,pTable))
			continue;

		if(IsMutexIPGroupOnTable(0,pTable,dwIPAddr))
			nPreTables[i]=1;
		
	}
}
void CBaseRoomData::ForbidSameTime(int nPreTables[],int nFromTableNO) 
{   
	int nMillisecond = GetPrivateProfileInt(_T("SameMillisecond"),xyConvertIntToStr(roomdata.nRoomID ),1000, g_szIniFile);
	for(int i=0;i<MAX_PRETABLES;i++)
	{
		if(nPreTables[i]==1)
			continue;
 		if(IsSameTimeOnTable( nMillisecond, nFromTableNO+i))
			nPreTables[i]=1;
	}
}

void CBaseRoomData::ForbidSameClub(int nPreTables[],int  nClubID,int nFromTableNO)
{
	if (nClubID == 0)
		return;

	for(int i=0;i<MAX_PRETABLES;i++)
	{
		if(nPreTables[i]==1)
			continue;

		TABLE* pTable=NULL;
		if(!m_mapTable.Lookup(nFromTableNO+i,pTable))
			continue;

		if(IsSameClubOnTable(nClubID,pTable))
			nPreTables[i]=1;
	}
}

void CBaseRoomData::ForbidUnwelcomes( int nPreTables[],int nUserID,int nFromTableNO ) 
{
	for(int i=0;i<MAX_PRETABLES;i++)
	{
 	   if(nPreTables[i]==1) 
		   continue;
 	   if(IsUnwelcomesOnTable(nUserID,nFromTableNO+i))
	  	  nPreTables[i]=1;
	}
}
void CBaseRoomData::ForbidSameSeated( int nPreTables[],int nSeatedTableNO,int nFromTableNO ) 
{ 	
    int nMaxSame=roomdata.nChairCount-2;
	if(nMaxSame==0)
		return ;

  	for(int i=0;i<MAX_PRETABLES;i++)
	{
 		if(nPreTables[i]==1) 
		   continue;
 	    if(IsSameSeatedOnTable(nSeatedTableNO,nFromTableNO+i)>=nMaxSame)
	  	   nPreTables[i]=1;
	}
 
}
void CBaseRoomData::ForbidMutexHard( int nPreTables[],LPCTSTR lpszMachineID,int nFromTableNO ) 
{ 	
	if(!IsMutexHardRoom())  
		return ;
	{
  		CAutoLock lock(&g_csMutexHard);
 		for(int i=0;i<MAX_PRETABLES;i++)
		{
 		   if(nPreTables[i]==1) 
			   continue;
 		   if(IsMutexHardOnTable(lpszMachineID,nFromTableNO+i))
	  		  nPreTables[i]=1;
		}
	}
}
void CBaseRoomData::ForbidVolumeID( int nPreTables[],LPCTSTR lpszVolumeID,int nFromTableNO) 
{
	if(!IsMutexHardRoom())  
		return ;
	for(int i=0;i<MAX_PRETABLES;i++)
	{
		if(nPreTables[i]==1)
			continue;
		
		if(IsSameVolumeIDOnTable(lpszVolumeID, nFromTableNO+i))
			nPreTables[i]=1;
		
	}
  

}
void CBaseRoomData::ForbidSameLAN(int nPreTables[],int nUserID, int nFromTableNO)
{
	for(int i=0;i<MAX_PRETABLES;i++)
	{
		if(nPreTables[i]==1)
			continue;
		 
		TABLE* pTable=NULL;
		if(!m_mapTable.Lookup(nFromTableNO+i,pTable))
			continue;

		if(IsSameLANOnTable(nUserID,pTable))
			nPreTables[i]=1;
		
	}
}
BOOL CBaseRoomData::JoinMax( int nPreTables[],PLAYER_POSITION* pp ,int nFromTableNO)
{
 	TABLE* lpPreTable[MAX_PRETABLES];
 	ZeroMemory(lpPreTable,sizeof(lpPreTable));
 	int nMaxTableNO=0;
 	int nMaxPlayerCount=0;

    for(int i=0;i<MAX_PRETABLES;i++)
	{ 
 		 if(nPreTables[i]==1) 
			 continue;
  		 m_mapTable.Lookup(nFromTableNO+i,lpPreTable[i]);
	}
    for( i=0;i<MAX_PRETABLES;i++)//赋初始值
	{ 
 		 if(nPreTables[i]==0) //允许桌子
		 {
 			  nMaxTableNO=i+nFromTableNO;
			  nMaxPlayerCount=lpPreTable[i]->th.nPlayerCount;
			  break;
		 }
	}
 
	for(i=0;i<MAX_PRETABLES;i++)
	{
 	   if(nPreTables[i]==1) 
			 continue;
 		if(lpPreTable[i]->th.nPlayerCount>nMaxPlayerCount)
		{
			nMaxTableNO=nFromTableNO+i;
			nMaxPlayerCount=lpPreTable[i]->th.nPlayerCount;
		}
	}
	if(nMaxPlayerCount==0)
	{
	 	pp->nTableNO=nMaxTableNO;
		pp->nChairNO=0;
	}
	else
	{
		int nFirstTableNO=nMaxTableNO;
		int nFirstStartTime=lpPreTable[nFirstTableNO-nFromTableNO]->nFirstStartTime;
		for(int i=0;i<MAX_PRETABLES;i++)
		{ 	 
			if(nPreTables[i]==1) 
			     continue;
			if(lpPreTable[i]->th.nPlayerCount==nMaxPlayerCount)
			{
				 if(lpPreTable[i]->nFirstStartTime < nFirstStartTime)
				 {
					 nFirstTableNO=nFromTableNO+i;
					 nFirstStartTime=lpPreTable[i]->nFirstStartTime;
				 }
			}
		}
		pp->nTableNO =nFirstTableNO;
		pp->nChairNO=GetFreeChairNO(lpPreTable[pp->nTableNO-nFromTableNO]);
	
	}
	return TRUE;
}

BOOL CBaseRoomData::JoinMin( int nPreTables[],PLAYER_POSITION* pp ,int nMaxPreTables,int nFromTableNO )
{
 	TABLE* lpPreTable[MAX_PRETABLES];
 	ZeroMemory(lpPreTable,sizeof(lpPreTable));
	int nMinTableNO=-1;
	int nMinPlayerCount=0;
 
    for(int i=0;i<MAX_PRETABLES;i++)
	{ 
		if(nPreTables[i]==1) 
			 continue;
 		m_mapTable.Lookup(nFromTableNO+i,lpPreTable[i]);
 
	}
    for( i=0;i<nMaxPreTables;i++)//赋初始值
	{ 
 		 if(nPreTables[i]==0) //允许桌子
		 {
 			  nMinTableNO=i+nFromTableNO;
			  nMinPlayerCount=lpPreTable[i]->th.nPlayerCount;
			  break;
		 }
	}
	if(nMinTableNO==-1)//在限定的nMaxPreTables里没有允许的桌子
	{
		for(int i=0;i<MAX_PRETABLES;i++)
		{ 
			if(nPreTables[i]==0) //允许桌子
			{
				pp->nTableNO=nFromTableNO+i;
				pp->nChairNO=GetFreeChairNO(lpPreTable[i]);
 				return TRUE;
			}
		}
		return FALSE;
	}
	for( i=0;i<nMaxPreTables;i++)
	{	
		if(nPreTables[i]==1) 
			 continue;
		if(lpPreTable[i]->th.nPlayerCount<nMinPlayerCount)
		{
			nMinTableNO=nFromTableNO+i;
			nMinPlayerCount=lpPreTable[i]->th.nPlayerCount;
		}
	}
	if(nMinPlayerCount==0)
	{
		pp->nTableNO=nMinTableNO;
		pp->nChairNO=0;
	}
	else
	{
		int nFirstTableNO=nMinTableNO;
		int nFirstStartTime=lpPreTable[nFirstTableNO-nFromTableNO]->nFirstStartTime;
		for(int i=0;i<nMaxPreTables;i++)
		{
			if(nPreTables[i]==1) 
				 continue;
			if(lpPreTable[i]->th.nPlayerCount==nMinPlayerCount)
			{
				 if(lpPreTable[i]->nFirstStartTime < nFirstStartTime)
				 {
					 nFirstTableNO=nFromTableNO+i;
					 nFirstStartTime=lpPreTable[i]->nFirstStartTime;
				 }
			}
		}
		pp->nTableNO =nFirstTableNO;
		pp->nChairNO=GetFreeChairNO(lpPreTable[pp->nTableNO-nFromTableNO]);
	 
	}

	return TRUE;
}	
void CBaseRoomData::SetDarkPosition(PLAYER* pPlayer,TABLE* pTable,PLAYER_POSITION* pp ) 
{
	pPlayer->nTableNO=pp->nTableNO;
	pPlayer->nChairNO=pp->nChairNO;
   	if(pTable->th.nPlayerCount==0) //这一桌第一个入座
 		pTable->nFirstStartTime=GetTickCount();
 	pTable->th.nPlayerCount++;
	pTable->nPlayerAry[pp->nChairNO]=pp->nUserID;
	pTable->dwIPAddrs[pp->nChairNO] = pPlayer->pe.dwIPAddr;
	pTable->nLatestStartTime=GetTickCount();
}

void CBaseRoomData::SetRandomPosition(PLAYER* pPlayer,TABLE* pTable,PLAYER_POSITION* pp)
{
 	pPlayer->pe.nRandomTableNO=pp->nTableNO;
	pPlayer->pe.nRandomChairNO=pp->nChairNO;
	pPlayer->pe.nEnterTime =GetTickCount();//进入分桌队列时间

  	if(pTable->th.nPlayerCount==0) //这一桌第一个入座
 	 	pTable->nFirstStartTime=GetTickCount();
 	pTable->th.nPlayerCount++;

	if(pTable->nPlayerAry[pp->nChairNO]!=0)
	{
		UwlLogFile(_T("=========== SetRandom Position,PlayerAry[%d]=%d============="),pp->nChairNO,pTable->nPlayerAry[pp->nChairNO]);
		pp->nChairNO =GetFreeChairNO(pTable);
	}

	pTable->nPlayerAry[pp->nChairNO]=pp->nUserID;
  	pTable->dwIPAddrs[pp->nChairNO] = pPlayer->pe.dwIPAddr;
	pTable->nLatestStartTime=GetTickCount();
 

}

int  CBaseRoomData::GetKeepStartedSecond()
{
	int nRet = GetPrivateProfileInt(_T("KeepStartedSecond"),xyConvertIntToStr(roomdata.nRoomID ),10, g_szIniFile);
	return nRet;
}
int  CBaseRoomData::GetUnwelcomeExperience()
{
	int nRet = GetPrivateProfileInt(_T("UnwelcomeExperience"),xyConvertIntToStr(roomdata.nRoomID ),2000, g_szIniFile);
	return nRet;
}

void CBaseRoomData::SetFixedPosition(PLAYER_POSITION* pp,int nFromTableNO )
{
	pp->nTableNO =nFromTableNO+MAX_PRETABLES ; 
 
	TABLE* pTable=NULL;
	if(!m_mapTable.Lookup(pp->nTableNO,pTable))
	{
		pTable = new TABLE;
		ZeroMemory(pTable, sizeof(TABLE));
		pTable->th.nTableNO = pp->nTableNO;
		m_mapTable.SetAt(pTable->th.nTableNO, pTable);
	}
 	pp->nChairNO =GetFreeChairNO(pTable);

}

BOOL CBaseRoomData::FindPlayerOnTable(TABLE* pTable,int nPlayerID)
{
 	if(pTable->th.nPlayerCount==0)
		return FALSE;

	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
	 	if(nPlayerID== pTable->nPlayerAry[i])
		 	return TRUE;
	}
	return FALSE;

}

BOOL CBaseRoomData::FindPlayerIP(DWORD dwIP) 
{
  	int userid = 0;
	LPPLAYER lpPlayer = NULL;
	POSITION pos =  m_mapPlayer.GetStartPosition();
	while(pos)
	{
		m_mapPlayer.GetNextAssoc(pos, userid, lpPlayer);
		if(lpPlayer)
		{
			if(lpPlayer->pe.dwIPAddr==dwIP)
				return TRUE;
		}
	}
	return FALSE;

}
HWND CBaseRoomData::FindGameSvrWindow()
{
  	TCHAR szClsName[64];
	TCHAR szWndName[64];
 	sprintf(szClsName,_T("%s%d"),GAMESVR_CLSNAME, roomdata.nGamePort);
	sprintf(szWndName,_T("%s%d"),GAMESVR_WNDNAME, roomdata.nGamePort);

  	return  FindWindow(szClsName,szWndName);
}
 
void  CBaseRoomData::PostSecurityToHold(int nUserID,LPCTSTR lpszVolumeID,int nTokenID,int nUserType) 
{
  	HWND hWnd=FindHoldSvrWindow();
 	if(IsWindow(hWnd))
	{
		DWORD dwCode = 0;
		UwlMakeOnewayCodeDWord(lpszVolumeID, lstrlen(lpszVolumeID), dwCode, GetUWLSeedLicense()); 
 		PostMessage(hWnd,WM_RTG_HARDCODE,dwCode, nUserID);
 		PostMessage(hWnd,WM_RTG_TOKENID,nTokenID, nUserID);
 		PostMessage(hWnd,WM_RTG_USERTYPE,nUserType, nUserID);
	}
}
void  CBaseRoomData::PostHardAndToken(  int nUserID,LPCTSTR lpszHardID,int nTokenID) 
{
  	HWND hWnd=FindGameSvrWindow();
 	if(IsWindow(hWnd))
	{
		DWORD dwCode = 0;
		UwlMakeOnewayCodeDWord(lpszHardID, lstrlen(lpszHardID), dwCode, GetUWLSeedLicense()); 
 		PostMessage(hWnd,WM_RTG_HARDCODE,dwCode, nUserID);
 		PostMessage(hWnd,WM_RTG_TOKENID,nTokenID, nUserID);
	}
}


void CBaseRoomData::PostGetFinished(int nUserID,int nTableNO,int nChairNO)
{
   	HWND hWnd=FindGameSvrWindow();
 	if(IsWindow(hWnd))
	{
 		DWORD dwRoomTableChair=ComboRoomTableChair(roomdata.nRoomID,nTableNO,nChairNO);
 		PostMessage(hWnd,WM_RTG_GETFINISHED,dwRoomTableChair,nUserID);
	}

}

void CBaseRoomData::PostLeaveRoom(int nUserID)
{
  	HWND hWnd=FindGameSvrWindow();
 	if(IsWindow(hWnd))
 		PostMessage(hWnd,WM_RTG_LEAVEROOM,roomdata.nRoomID,nUserID);
}


void CBaseRoomData::PostRoomTableChair(int nUserID,int nTableNO,int nChairNO)
{
  	HWND hWnd=FindGameSvrWindow();
 	if(IsWindow(hWnd))
	{
 		DWORD dwRoomTableChair=ComboRoomTableChair(roomdata.nRoomID,nTableNO,nChairNO);
 		PostMessage(hWnd,WM_RTG_ROOMTABLECHAIR,dwRoomTableChair,nUserID);
	}
	
}
void CBaseRoomData::PostNewChair(int nUserID,int nTableNO,int nChairNO)
{
	HWND hWnd=FindGameSvrWindow();
	if(IsWindow(hWnd))
	{
		DWORD dwRoomTableChair=ComboRoomTableChair(roomdata.nRoomID,nTableNO,nChairNO);
		PostMessage(hWnd,WM_RTG_NEWCHAIR,dwRoomTableChair,nUserID);
	}
}
void CBaseRoomData::PostHomeUserID(int nTableID,int nHomeUerID)
{
	HWND hWnd=FindGameSvrWindow();
	if(IsWindow(hWnd))
	{
		//DWORD dwRoomTableChair=ComboRoomTableChair(roomdata.nRoomID,nTableNO,nChairNO);
		PostMessage(hWnd,WM_RTG_HOMEUSERID,nTableID,nHomeUerID);
	}
}
void CBaseRoomData::PostVerifyRoomTableChair(int nUserID,int nTableNO,int nChairNO)
{
	HWND hWnd=FindGameSvrWindow();
	if(IsWindow(hWnd))
	{
		DWORD dwRoomTableChair=ComboRoomTableChair(roomdata.nRoomID,nTableNO,nChairNO);
		PostMessage(hWnd,WM_RTG_VERIFY_ROOMTABLECHAIR,dwRoomTableChair,nUserID);
	}
}
  
void CBaseRoomData::PostNewTableChair(int nUserID, int nTableNO, int nChairNO)
{
	HWND hWnd=FindGameSvrWindow();
	if(IsWindow(hWnd))
	{
		DWORD dwRoomTableChair=ComboRoomTableChair(roomdata.nRoomID,nTableNO,nChairNO);
		PostMessage(hWnd,WM_RTG_NEWTABLECHAIR,dwRoomTableChair,nUserID);
	}
}

void CBaseRoomData::PostPlayerPosition( TABLE* pTable )
{
  	HWND hWnd=FindGameSvrWindow();
 	if(IsWindow(hWnd))
	{
		int   nTableNO=pTable->th.nTableNO;
 		for(int i=0;i<pTable->th.nPlayerCount;i++)
		{
  			DWORD dwRoomTableChair=ComboRoomTableChair(roomdata.nRoomID,nTableNO,i);
  			PostMessage(hWnd,WM_RTG_ROOMTABLECHAIR,dwRoomTableChair,pTable->nPlayerAry[i]);
		}
	}
}
void CBaseRoomData::PostClearTable(int nTableNO)
{
  	HWND hWnd=FindGameSvrWindow();
 	if(IsWindow(hWnd))
  		PostMessage(hWnd,WM_RTG_CLEARTABLE, roomdata.nRoomID, nTableNO);    
}

void CBaseRoomData::PostExperience(int nUserID,int nExperience)
{
  	HWND hWnd=FindGameSvrWindow();
 	if(IsWindow(hWnd))
 		PostMessage(hWnd,WM_RTG_EXPERIENCE,nExperience,nUserID);
}


void CBaseRoomData::PostRoomData()
{
  	HWND hWnd=FindGameSvrWindow();
 	if(IsWindow(hWnd))
	{
		PostMessage(hWnd,WM_RTG_ROOMSVRHWND,roomdata.nRoomID,(LPARAM)g_hMessageWnd);

  		PostMessage(hWnd,WM_RTG_ROOMOPTIONS,roomdata.nRoomID, roomdata.dwOptions);
  		PostMessage(hWnd,WM_RTG_ROOMCONFIGS,roomdata.nRoomID, roomdata.dwConfigs);
  		PostMessage(hWnd,WM_RTG_ROOMMANAGES,roomdata.nRoomID, roomdata.dwManages);
  		PostMessage(hWnd,WM_RTG_GAMEOPTIONS,roomdata.nRoomID, roomdata.dwGameOptions);
  		PostMessage(hWnd,WM_RTG_GAMEPARAM,roomdata.nRoomID, roomdata.nGameParam);
  		PostMessage(hWnd,WM_RTG_GAMEDATA,roomdata.nRoomID, roomdata.nGameData);

 		PostMessage(hWnd,WM_RTG_MINPLAYSCORE,roomdata.nRoomID, roomdata.nMinPlayScore);
  		PostMessage(hWnd,WM_RTG_MAXPLAYSCORE,roomdata.nRoomID, roomdata.nMaxPlayScore);

 		PostMessage(hWnd,WM_RTG_MINDEPOSIT,roomdata.nRoomID, roomdata.nMinDeposit);
  		PostMessage(hWnd,WM_RTG_MAXDEPOSIT,roomdata.nRoomID, roomdata.nMaxDeposit);

	}
}
void CBaseRoomData::PostKickoffPlayer(int nUserID,int nTableNO,int nChairNO)
{
   	HWND hWnd=FindGameSvrWindow();
 	if(IsWindow(hWnd))
	{
   		DWORD dwRoomTableChair=ComboRoomTableChair(roomdata.nRoomID,nTableNO,nChairNO);
  		PostMessage(hWnd,WM_RTG_KICKOFFPLAYER,dwRoomTableChair,nUserID);
	}

}

CString CBaseRoomData::GetGameSvrFolder()
{
	CString sRet;
   
	TCHAR szExeFile[MAX_PATH];
 	memset(szExeFile,0,sizeof(szExeFile));
 	GetPrivateProfileString(_T("SVRPATH"), xyConvertIntToStr(roomdata.nGamePort),_T(""),szExeFile,sizeof(szExeFile) ,GetYxpdsvrIniFile());
	UwlSplitPath(szExeFile, SPLIT_DRIVE_DIR, szExeFile);

	sRet.Format(_T("%s"),szExeFile);

	return sRet;
}
CString CBaseRoomData::GetGameSvrIniFile()
{
	CString sRet;

 	TCHAR szExeFile[MAX_PATH];
 	memset(szExeFile,0,sizeof(szExeFile));
 	GetPrivateProfileString(_T("SVRPATH"), xyConvertIntToStr(roomdata.nGamePort),_T(""),szExeFile,sizeof(szExeFile) ,GetYxpdsvrIniFile());
	sRet.Format(_T("%s"),szExeFile);
	
 	int nFind=sRet.ReverseFind('\\');
	if(nFind!=-1)
	{
		sRet=sRet.Left(nFind+1+7);
		sRet+=_T(".ini");
	}
	else
		sRet.Empty();

 	return sRet;
}


BOOL CBaseRoomData::FindPlayerHardID(LPCTSTR lpszHardID)
{
  	int userid = 0;
	LPPLAYER lpPlayer = NULL;
	POSITION pos =  m_mapPlayer.GetStartPosition();
	while(pos)
	{
		m_mapPlayer.GetNextAssoc(pos, userid, lpPlayer);
		if(lpPlayer)
		{
			if(lstrcmpi(lpPlayer->pe.szHardID,lpszHardID)==0)
				return TRUE;
		}
	}
	return FALSE;

}
BOOL CBaseRoomData::IsMustKeepStarted(PLAYER* pPlayer ,int& nWaitSeconds)
{
	if(pPlayer->nStatus == PLAYER_STATUS_WAITING)
	{
		int nKeep=GetKeepStartedSecond();
 		int nInterval=pPlayer->pe.dwLatestTime-pPlayer->pe.dwLatestStartTime;
		if(nInterval<nKeep*1000) //不到N秒钟
		{
		    nWaitSeconds=nKeep-nInterval/1000;
 			return TRUE;
		}
	}
	return FALSE;
}
BOOL CBaseRoomData::IsExchangeSeat()
{
	return IS_BIT_SET(roomdata.dwConfigs , ROOM_CFG_EXCHANGESEAT);
}

BOOL CBaseRoomData::IsMutexHardRoom()
{
   return IS_BIT_SET( roomdata.dwOptions,ROOM_OPT_USEDMUTEXHARD);

}
BOOL CBaseRoomData::IsRandomRoom()
{
    return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_RANDOM);

}
BOOL CBaseRoomData::IsSoloRoom()
{
    return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_SOLO);

}
BOOL CBaseRoomData::IsVariableChair()
{
    return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_VARIABLECHAIR);

}
BOOL CBaseRoomData::IsSupportMobile()
{
    return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_SUPPORTMOBILE);

}
BOOL CBaseRoomData::IsLeaveAlone()
{
    return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_LEAVEALONE);
}
BOOL CBaseRoomData::IsEarlyLeave()
{
	return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_EARLYLEAVE);
}
BOOL CBaseRoomData::IsPrivateRoom()
{
	return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_PRIVATEROOM);
}
BOOL CBaseRoomData::IsMatchRoom()
{
    return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_MATCH);

}
BOOL CBaseRoomData::IsOldMatchRoom()
{
    return IS_BIT_SET( roomdata.dwManages,ROOM_MNG_MATCHONGAME);

}

BOOL CBaseRoomData::IsLeagueMachRoom()
{
	return IS_BIT_SET( roomdata.dwManages,ROOM_MNG_LEAGUEMATCH);
}
BOOL CBaseRoomData::IsSupportDeposit()
{
 	return IS_BIT_SET(roomdata.dwOptions,ROOM_OPT_NEEDDEPOSIT);
}
BOOL CBaseRoomData::IsTableDeposit()
{
    return IS_BIT_SET( roomdata.dwManages,ROOM_MNG_TABLEDEPOSIT);

}
BOOL CBaseRoomData::IsTablePasswordWrong(TABLE* pTable,LPCTSTR lpszPassword)
{
 	 if(0 == pTable->th.nPlayerCount)	// 第一个入座用户可设密码
	 {	
		lstrcpyn(pTable->szPassword,lpszPassword,MAX_PASSWORD_LEN);
		if(lstrcmp(pTable->szPassword, _T("")))
			pTable->th.bHavePassword = TRUE;
		else
			pTable->th.bHavePassword = FALSE;
	 }
	 else
	 {								
	 	if(lstrcmp(pTable->szPassword, _T("")))// 后来者需要输入密码
		{
		 	if(lstrcmp(pTable->szPassword, lpszPassword))//密码不同
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CBaseRoomData::IsDarkRoom()
{
    return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_DARK);
		 
}
BOOL CBaseRoomData::IsWebGameRoom()
{
    return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_WEBGAME);
		 
}


BOOL CBaseRoomData::IsCloakingRoom()
{
    return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_CLOAKING);
		 
}
BOOL CBaseRoomData::IsMicroEndRoom()
{
    return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_MICROEND);
		 
}
BOOL CBaseRoomData::IsGetUnwelcome()
{
    return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_UNWELCOME);
		 
}
BOOL CBaseRoomData::IsGetClientInfo()
{
    return IS_BIT_SET( roomdata.dwConfigs,ROOM_CFG_CLIENTINFO);
		 
}
BOOL CBaseRoomData::IsUnwelcomePlayer(int nUserID,int nPlayerID)
{
	int nRet=0;
	CIDMap* pIDMap=NULL;
	m_mapUnwelcomes.Lookup(nUserID,pIDMap);
	if(pIDMap)
  		pIDMap->Lookup(nPlayerID,nRet);
 	return  nRet;
}
BOOL CBaseRoomData::IsMutexIP(DWORD dwIPAddr1,DWORD dwIPAddr2,int nSameSubs)
{
	CString sSubIP1,sSubIP2;
	TCHAR szIP[MAX_SERVERIP_LEN];
 	ZeroMemory(szIP,sizeof(szIP));
  	UwlAddrToName(dwIPAddr1,szIP);
  	sSubIP1=xyGetIPSubFields(szIP,nSameSubs);
    if(sSubIP1.IsEmpty() )
	    return FALSE;
	
	ZeroMemory(szIP,sizeof(szIP));
  	UwlAddrToName(dwIPAddr2,szIP);
   	sSubIP2=xyGetIPSubFields(szIP,nSameSubs);
    if( sSubIP2.IsEmpty())
	    return FALSE;

 	CString sValue;
  	sprintf(szIP,_T("%s.%s"),sSubIP1,sSubIP2);
 	if(IsSomeValueInFile(_T("MutexIP"),szIP,sValue,g_szIPManFile))
		return TRUE;
  
  	sprintf(szIP,_T("%s.%s"),sSubIP2,sSubIP1);
	if(IsSomeValueInFile(_T("MutexIP"),szIP,sValue,g_szIPManFile))
		return TRUE;
  
	return FALSE;
}
void CBaseRoomData::GetMutexGroupIP(DWORD dwIPAddr,LPTSTR lpszIP)
{
 	TCHAR szIP[MAX_SERVERIP_LEN];
 	ZeroMemory(szIP,sizeof(szIP));
 
	UwlAddrToName(dwIPAddr,szIP);

 	TCHAR *fields[8];
	ZeroMemory(fields, sizeof(fields));
 	TCHAR  *p ;
    xyRetrieveFields2 (szIP, fields, 8, &p,'.' ) ;
    sprintf(lpszIP,_T("%s.%s.0.0"),fields[0],fields[1]);
}

BOOL CBaseRoomData::IsMutexIPGroup(DWORD dwIPAddr1,DWORD dwIPAddr2,int nSameSubs)
{
 	int nCount=GetPrivateProfileInt(_T("MutexIPGroup"),_T("Count"),0,g_szIPManFile);
	if(nCount<=0)
		return FALSE;

 	TCHAR szIP1[MAX_SERVERIP_LEN];
 	ZeroMemory(szIP1,sizeof(szIP1));
 
  	TCHAR szIP2[MAX_SERVERIP_LEN];
 	ZeroMemory(szIP2,sizeof(szIP2));

	GetMutexGroupIP(dwIPAddr1,szIP1);
	GetMutexGroupIP(dwIPAddr2,szIP2);
	
	TCHAR szSection[32];
  	ZeroMemory(szSection,sizeof(szSection));
 
 	CString sValue;
	for(int i=0;i<nCount;i++)
	{
		sprintf(szSection,_T("MIPGroup%d"),i);
		BOOL b1= IsSomeValueInFile(szSection,szIP1,sValue,g_szIPManFile);
		BOOL b2= IsSomeValueInFile(szSection,szIP2,sValue,g_szIPManFile);
 
		if(b1 && b2 )
			return TRUE;
	}
	return FALSE;
}
BOOL CBaseRoomData::IsMutexHard(LPCTSTR lpszMachineID1,LPCTSTR lpszMachineID2,int& nExpiration)
{
 	CString sFind;
  	sFind.Format(_T("%s|%s"),lpszMachineID1,lpszMachineID2);
	MUTEX_HARD_SMALL mhs;
	memset(&mhs,0,sizeof(mhs));
	if(g_mapMutexHardSmall.Lookup(sFind,mhs))
	{
		if(mhs.nExpiration==0 || UwlGetCurrentSeconds()<mhs.nExpiration)
		{
			nExpiration=mhs.nExpiration;
			return TRUE;
		}
 		else
			g_mapMutexHardSmall.RemoveKey(sFind);
	}
	return FALSE;

}

BOOL CBaseRoomData::IsForbidPlayer(int nPlayerID)
{
  	ROOM_FORBID rf;
	memset(&rf,0,sizeof(ROOM_FORBID));
 
   	if(!m_mapRoomForbid.Lookup(nPlayerID,rf))
		return FALSE;

    if(UwlGetCurrentSeconds()<rf.nExpiration) //还在禁用期内
		return rf.nExpiration;
	return FALSE;
}
void CBaseRoomData::SendDarkPlaying(TABLE* pTable,PLAYER_POSITION* pp,LPTABLE& pNewTable)
{
	PostRoomData();

 	int nNewTableNO=GetFreeTableNO( MAX_PRETABLES+1);

	m_mapTable.Lookup(nNewTableNO,pNewTable);
    MoveDarkTable(pTable,pNewTable);

	//////////////////////////////////////////////////////////
	DARK_PLAYING  dp;
	memset(&dp, 0, sizeof(dp));
	memcpy(dp.nPlayerAry, pNewTable->nPlayerAry, sizeof(dp.nPlayerAry));

	PostPlayerPosition(pNewTable);

 	/////////////////////////////////////////////////////////
	for(int i = 0; i < pNewTable->th.nPlayerCount; i++)
	{
		LPPLAYER lpSeated = NULL;
		if(m_mapPlayer.Lookup(pNewTable->nPlayerAry[i], lpSeated))
		{
			dp.pp.nUserID=lpSeated->nUserID;
			dp.pp.nTableNO=lpSeated->nTableNO;
			dp.pp.nChairNO=lpSeated->nChairNO;
			NotifyOnePlayer(this, lpSeated->nUserID,GR_DARK_PLAYING, &dp, sizeof(dp));
		}
	}
     ///////////////////////////////////////////////////////////////////////////
	NTF_GET_STARTED  ngs;
	memset(&ngs, 0, sizeof(NTF_GET_STARTED));
	ngs.pp.nUserID=pp->nUserID;
	ngs.pp.nTableNO=nNewTableNO;
	ngs.pp.nChairNO=pp->nChairNO;
 	memcpy(ngs.nPlayerAry, pNewTable->nPlayerAry, sizeof(ngs.nPlayerAry));
 	NotifyDarkLooker(this, ngs.pp.nUserID,GR_PLAYER_PLAYING, &ngs, sizeof(NTF_GET_STARTED));		  

}

 
void CBaseRoomData::MoveDarkTable(TABLE* pTable,TABLE* pNewTable)
{
	int nOldTableNO=pTable->th.nTableNO;
 	int nNewTableNO=pNewTable->th.nTableNO ;

	for(int i = 0; i < pTable->th.nPlayerCount; i++)
	{
		LPPLAYER lpSeated = NULL;
		if(m_mapPlayer.Lookup(pTable->nPlayerAry[i], lpSeated))
		{
		 	lpSeated->nTableNO = nNewTableNO;
 			lpSeated->pe.nStartupTime=UwlGetCurrentSeconds();
			lpSeated->nStatus = PLAYER_STATUS_PLAYING;
		}
	}
	 
	pTable->th.nStatus = TABLE_STATUS_PLAYING;
  	pTable->nStartupTime = UwlGetCurrentSeconds();

	memcpy(pNewTable,pTable,sizeof(TABLE));
 	pNewTable->th.nTableNO=nNewTableNO;
	ZeroMemory(pTable,sizeof(TABLE));
	pTable->th.nTableNO=nOldTableNO;
}




void CBaseRoomData::CommitTPS(TABLE* pTable,int nStatus)
{
	TABLE_PLAYER_STATUS tps;
   	ZeroMemory(&tps,sizeof(tps));
 	tps.nRoomSvrID=g_nClientID;
   	tps.nGameID= roomdata.nGameID;
	tps.nRoomID= roomdata.nRoomID;
    tps.nAreaID= roomdata.nAreaID;
  	tps.nStatus=nStatus;
	tps.nTableNO=pTable->th.nTableNO;
	tps.nChairCount=roomdata.nChairCount;//提交这桌上所有玩家
 
 	for(int i = 0; i < tps.nChairCount; i++)
 	{
	 	 tps.nPlayerID[i]=pTable->nPlayerAry[i] ;
	}
	NotifyServer(GR_TABLE_PLAYER_STATUS,&tps,sizeof(TABLE_PLAYER_STATUS));
 
}


void CBaseRoomData::CommitTPS_VARI(TABLE* pTable,int nStatus,int nChairStatus[])
{
	TABLE_PLAYER_STATUS tps;
   	ZeroMemory(&tps,sizeof(tps));
 	tps.nRoomSvrID=g_nClientID;
   	tps.nGameID= roomdata.nGameID;
	tps.nRoomID= roomdata.nRoomID;
    tps.nAreaID= roomdata.nAreaID;
  	tps.nStatus=nStatus;
	tps.nTableNO=pTable->th.nTableNO;
	tps.nChairCount=roomdata.nChairCount;//针对可变椅子的房间，用最大椅子数
 
 	for(int i = 0; i < tps.nChairCount; i++)
 	{
		if(nChairStatus[i]==1)
	 		tps.nPlayerID[i]=pTable->nPlayerAry[i] ;
	}
	NotifyServer(GR_TABLE_PLAYER_STATUS,&tps,sizeof(TABLE_PLAYER_STATUS));
 
}

void CBaseRoomData::CommitTPS_SOLO(int nUserID,int nTableNO,int nStatus)
{
	TABLE_PLAYER_STATUS tps;
   	ZeroMemory(&tps,sizeof(tps));
 	tps.nRoomSvrID=g_nClientID;
   	tps.nGameID= roomdata.nGameID;
	tps.nRoomID= roomdata.nRoomID;
    tps.nAreaID= roomdata.nAreaID;
  	tps.nStatus=nStatus;
	tps.nTableNO=nTableNO;
	tps.nChairCount=1;
 	tps.nPlayerID[0]=nUserID;
 
	NotifyServer(GR_TABLE_PLAYER_STATUS,&tps,sizeof(TABLE_PLAYER_STATUS));
 
}

void CBaseRoomData::TransmitPlayerInRoom(PLAYER* lpPlayer,SEARCH_PLAYER* lpSearchPlayer)
{
	PLAYER_IN_ROOM pir;
	ZeroMemory(&pir, sizeof(pir));
	pir.nGameID = roomdata.nGameID;
	pir.nAreaID = roomdata.nAreaID;
	pir.nRoomID = roomdata.nRoomID;

	pir.nUserID = lpPlayer->nUserID;
	pir.dwIPAddr=lpPlayer->pe.dwIPAddr;
	pir.dwSearchFlag=lpSearchPlayer->dwSearchFlag;
	pir.nHallSvrID = lpSearchPlayer->nHallSvrID;
	pir.nBlockSvrID = lpSearchPlayer->nBlockSvrID;
	pir.nAskerID = lpSearchPlayer->nAskerID;
	pir.nAgentGroupID =lpSearchPlayer->nAgentGroupID ;
	NotifyServer(GR_PLAYER_IN_ROOM, &pir, sizeof(pir));

}
void CBaseRoomData::TransmitPlayerInRoomEx(PLAYER* lpPlayer,SEARCH_PLAYER_EX* lpSearchPlayerEx)
{
	PLAYER_IN_ROOM pir;
	ZeroMemory(&pir, sizeof(pir));
	pir.nGameID = roomdata.nGameID;
	pir.nAreaID = roomdata.nAreaID;
	pir.nRoomID = roomdata.nRoomID;

	pir.nUserID = lpPlayer->nUserID;
	pir.dwIPAddr=lpPlayer->pe.dwIPAddr;
	pir.dwSearchFlag=lpSearchPlayerEx->dwSearchFlag;
	pir.nHallSvrID = lpSearchPlayerEx->nHallSvrID;
	pir.nBlockSvrID = lpSearchPlayerEx->nBlockSvrID;
	pir.nAskerID = lpSearchPlayerEx->nAskerID;
	pir.nAgentGroupID=lpSearchPlayerEx->nAgentGroupID ;
	pir.nRoomSvrID = g_nClientID;
	NotifyServer(GR_PLAYER_IN_ROOM, &pir, sizeof(pir));

}


int  CBaseRoomData::AT_GetReadyPlayerCount()
{
	int nRet=0;
	int userid = 0;
 	LPPLAYER lpPlayer = NULL;
 	POSITION pos  = m_mapPlayer.GetStartPosition();
 	while(pos)
	{
		m_mapPlayer.GetNextAssoc(pos, userid, lpPlayer);
		if(lpPlayer==NULL)
			continue;
     	if(IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_LOOKDARK))
			continue;
     	if(IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))
			continue;
		if(lpPlayer->nStatus==PLAYER_STATUS_READY)
		{
 			nRet++;
		}
	}
 	return nRet;
}

void CBaseRoomData::AT_CreateMatch(LPMATCH lpMatch)
{
	if (m_pTcMatch)
	{
		
		NotifyOneGameSvr(R2G_MATCH_CLEAR,&(roomdata.nRoomID),sizeof(int),roomdata.nGamePort);
	}

	SAFE_DELETE(m_pTcMatch);
	m_pTcMatch=new CTcMatch(this,lpMatch);
}

void CBaseRoomData::AT_OnMatchStart(LPMATCH_STARTTOROOM lpms)
{
	if(m_pTcMatch==NULL)
		return;	//未创建
	
	if (m_pTcMatch->IsMatchStart())
		return;	//已经开始

	int nMatchID=lpms->nMatchID;
 	NotifyRoomPlayers(this, GR_MATCH_STARTUP,&nMatchID,sizeof(int));
	
	MATCH_START   ms;
	ZeroMemory(&ms,sizeof(ms));
	ms.nRoomID=GetRoomID();
	ms.nMatchID=GetMatchID();
	ms.nMatchPlayerCount=AT_GetReadyPlayerCount();
 	NotifyOneGameSvr(R2G_MATCH_START,&ms,sizeof(ms),roomdata.nGamePort);
  	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
 	m_pTcMatch->RefreshDataFromStartToRoom(lpms);

	m_pTcMatch->CreateMatch(lpms);

	m_pTcMatch->BeginMatch();
	while(MATCH_STEP* step=m_pTcMatch->PopStep())
	{
		AT_OnMatchStep(step);
		m_pTcMatch->ClearStep(step);
	}

	int nLen=sizeof(MATCH_START_OK)+sizeof(int)*m_pTcMatch->GetTotalUserCount();
	BYTE* pData=new BYTE[nLen];
	memset(pData,0,nLen);
	MATCH_START_OK* pStartOK=(MATCH_START_OK*) pData;
	pStartOK->nGameID=GetGameID();
	pStartOK->nRoomID=GetRoomID();
	pStartOK->nMatchID=GetMatchID();
	pStartOK->nCount=m_pTcMatch->GetTotalUserCount();

	int* pUserID=(int*)(pData+sizeof(MATCH_START_OK));
	m_pTcMatch->CopyUserIDs(pUserID);

	NotifyMatchSvr(GR_MATCH_START_OK,pData,nLen);
	SAFE_DELETE_ARRAY(pData);
}

void CBaseRoomData::AT_OnMatchCanceled(LPMATCH_CANCELED lpmc)
{
	if(m_pTcMatch==NULL)
		return;	//未创建

	int nMatchID=lpmc->nMatchID;
	NotifyRoomPlayers(this, GR_MATCH_CANCELED,&nMatchID,sizeof(int));

	NotifyOneGameSvr(R2G_MATCH_CANCELED,lpmc,sizeof(MATCH_CANCELED),roomdata.nGamePort);
}

void CBaseRoomData::AT_OnMatchSignUped(LPMATCH_SIGNUPED lpms)
{
	if(m_pTcMatch==NULL)
		return;	//未创建

	int nMatchID=lpms->nMatchID;
	NotifyRoomPlayers(this, GR_MATCH_SIGNUPED,&nMatchID,sizeof(int));

	NotifyOneGameSvr(R2G_MATCH_SIGNUPED,lpms,sizeof(MATCH_SIGNUPED),roomdata.nGamePort);
}

void CBaseRoomData::AT_OnMatchSignUpOK(LPMATCH_SIGNUPOK lpms)
{
	if(m_pTcMatch==NULL)
		return;	//未创建

	m_pTcMatch->m_MatchData.nSignUpCount=lpms->nSignUpCount;//刷新报名人数
	NotifyOnePlayer(this,lpms->nUserID,GR_MATCH_SIGNUPOK,lpms,sizeof(MATCH_SIGNUPOK));
	NotifyDarkLooker(this,0,GR_MATCH_SIGNUPOK,lpms,sizeof(MATCH_SIGNUPOK));
	NotifyOneGameSvr(R2G_MATCH_SIGNUPOK,lpms,sizeof(MATCH_SIGNUPOK),roomdata.nGamePort);
}



void CBaseRoomData::AT_OnMatchGiveUpOK(LPMATCH_GIVEUPOK lpms)
{
	if(m_pTcMatch==NULL)
		return;	//未创建

	m_pTcMatch->m_MatchData.nSignUpCount=lpms->nSignUpCount;
	m_pTcMatch->m_mapPlayer.RemoveKey(lpms->nUserID);//去掉已经退赛的人

	NotifyOnePlayer(this,lpms->nUserID,GR_MATCH_GIVEUPOK,lpms,sizeof(MATCH_GIVEUPOK));
	NotifyDarkLooker(this,0,GR_MATCH_SIGNUPOK,lpms,sizeof(MATCH_GIVEUPOK));
	NotifyOneGameSvr(R2G_MATCH_GIVEUPOK,lpms,sizeof(MATCH_GIVEUPOK),roomdata.nGamePort);
}

void CBaseRoomData::AT_OnArrangeTable(LPARRANGE_TABLE  lpArrangeTable)
{
 	ARRANGE_TABLE at;
	ZeroMemory(&at,sizeof(at));
	memcpy(&at,lpArrangeTable,sizeof(at));
 
    /////////////////////桌子/////////////////////////
	LPTABLE pTable = NULL;
	if(!m_mapTable.Lookup(at.nTableNO, pTable))
	{
		pTable = new TABLE;
		ZeroMemory(pTable, sizeof(TABLE));
		pTable->th.nTableNO = at.nTableNO;
		m_mapTable.SetAt(at.nTableNO, pTable);
	}
	pTable->th.nPlayerCount=at.nChairCount;
	pTable->th.nStatus=TABLE_STATUS_PLAYING;
  	pTable->nStartupTime = UwlGetCurrentSeconds();

	memcpy(pTable->nPlayerAry,at.nUserIDs,sizeof(int)*at.nChairCount);

    ///////////////////玩家////////////////////////////
	//强制踢出用户
	BOOL bnAllPlayerOK=TRUE;
	for(int i=0;i<at.nChairCount;i++)
	{
 		LPPLAYER pPlayer = NULL;
		BOOL bFind=m_mapPlayer.Lookup(at.nUserIDs[i], pPlayer);
		if(bFind==FALSE)
		{
			UwlLogFile(_T("OnArrangeTable Failed! Not Find UserID=%d "),at.nUserIDs[i]);
// 			bnAllPlayerOK=FALSE;
// 			m_pTcMatch->ForceKickPlayer(at.nUserIDs[i]);
			continue;
		}
		pPlayer->nTableNO=at.nTableNO;
		pPlayer->nChairNO=i;
		pPlayer->nStatus=PLAYER_STATUS_PLAYING;
	}

// 	if (bnAllPlayerOK==FALSE)
// 	{
// 		m_pTcMatch->ForceDisBandTable(at.nTableNO);
// 		return;
// 	}
         
	for(int j=0;j<at.nChairCount;j++)
	{
		PLAYER_POSITION pp;
		memset(&pp,0,sizeof(pp));
		pp.nUserID=at.nUserIDs[j];
		pp.nTableNO=at.nTableNO;
		pp.nChairNO=j;
		NotifyOnePlayer(this,pp.nUserID,GR_MATCH_ARRANGE,&pp,sizeof(pp));
	}


 	NotifyOneGameSvr(R2G_ARRANGE_TABLE,&at,sizeof(at), roomdata.nGamePort);

  	NotifyDarkLooker(this,0,GR_MATCH_ARRANGE,&at,sizeof(at) );

}

void CBaseRoomData::AT_OnArrageFaild(LPARRANGE_FAILD lpArrangeTable)
{
	if (!m_pTcMatch) return;
	m_pTcMatch->TerminateTable(lpArrangeTable->nTableNO);

	UwlLogFile("AT_OnArrageFaild nTableNO:%ld",lpArrangeTable->nTableNO);

	for(int i=0;i<lpArrangeTable->nFaildCount;i++)
	{
		int nUserID=lpArrangeTable->nFaildUserID[i];
		CTcMatchPlayer* pPlayer=m_pTcMatch->GetPlayer(nUserID);
		if (pPlayer)
		{
			UwlLogFile("AT_OnArrageFaild nUserID:%ld",lpArrangeTable->nFaildUserID[i]);
			if (!pPlayer->IsQuit()
				&&!pPlayer->IsOffline())
			{
				pPlayer->m_nStatus=TCMATCH_STATUS_OFFLINE;
			}
		}
	}
	
}

void CBaseRoomData::AT_OnTerminateTable(LPTERMINATE_TABLE ptt)
{
	TERMINATE_TABLE tt;
	memcpy(&tt,ptt,sizeof(TERMINATE_TABLE));

	NotifyOneGameSvr(R2G_MATCH_TERMINATE_TABLE,&tt,sizeof(TERMINATE_TABLE),roomdata.nGamePort);
}

void CBaseRoomData::AT_OnDisbandTable(LPDISBAND_TABLE lpDisbandTable)
{ 
 	DISBAND_TABLE dt;
	ZeroMemory(&dt,sizeof(dt));
	memcpy(&dt,lpDisbandTable,sizeof(dt));

    /////////////////////桌子/////////////////////////
	LPTABLE pTable = NULL;
	if(!m_mapTable.Lookup(dt.nTableNO, pTable))
		return ;
    if(pTable->th.nStatus!=TABLE_STATUS_PLAYING)
		return ;
 	for(int i = 0; i < pTable->th.nPlayerCount; i++)
	{
 		LPPLAYER pPlayer = NULL;
	    m_mapPlayer.Lookup(pTable->nPlayerAry[i], pPlayer);
		if(pPlayer)
		{
			pPlayer->nTableNO = -1;
			pPlayer->nChairNO = -1;
			pPlayer->nStatus = PLAYER_STATUS_STAND;
			PLAYER_POSITION pp;
			memset(&pp,0,sizeof(pp));
			pp.nUserID=pTable->nPlayerAry[i];
	 	   	NotifyOnePlayer(this,pp.nUserID,GR_MATCH_DISBAND,&pp,sizeof(pp));
		}
	}	
 	pTable->dwBreakCount = 0;
   	pTable->th.nPlayerCount = 0;
  	pTable->th.nStatus = TABLE_STATUS_STATIC;
 	memset(pTable->nPlayerAry, 0, sizeof(pTable->nPlayerAry));
	 
	NotifyDarkLooker(this,0, GR_MATCH_DISBAND, &dt, sizeof(dt));
 
}

void CBaseRoomData::AT_BreakOffMatch()
{
	if (!m_pTcMatch)  return;
	if (!m_pTcMatch->IsMatchStart()) return;
	if (m_pTcMatch->IsMatchEnd()) return;
	//比赛进行中
	LPTABLE pTable = NULL;
	POSITION pos=m_mapTable.GetStartPosition();
	int nTableNO=0;
	while(pos)
	{
		m_mapTable.GetNextAssoc(pos,nTableNO,pTable);
		if (pTable
			&&pTable->th.nStatus==TABLE_STATUS_PLAYING)
		{
			DISBAND_TABLE dt;
			ZeroMemory(&dt,sizeof(dt));
			memset(&dt,0,sizeof(DISBAND_TABLE));
			dt.nRoomID=GetRoomID();
			dt.nMatchID=GetMatchID();
			dt.nTableNO=pTable->th.nTableNO;
			dt.nPlayerCount=pTable->th.nPlayerCount;
			memcpy(dt.nPlayerIDs,pTable->nPlayerAry,sizeof(int)*dt.nPlayerCount);

			for(int i = 0; i < pTable->th.nPlayerCount; i++)
			{
				LPPLAYER pPlayer = NULL;
				m_mapPlayer.Lookup(pTable->nPlayerAry[i], pPlayer);
				if(pPlayer)
				{
					pPlayer->nTableNO = -1;
					pPlayer->nChairNO = -1;
					pPlayer->nStatus = PLAYER_STATUS_STAND;
					PLAYER_POSITION pp;
					memset(&pp,0,sizeof(pp));
					pp.nUserID=pTable->nPlayerAry[i];
					NotifyOnePlayer(this,pp.nUserID,GR_MATCH_DISBAND,&pp,sizeof(pp));
					
				}
			}	
			pTable->dwBreakCount = 0;
			pTable->th.nPlayerCount = 0;
			pTable->th.nStatus = TABLE_STATUS_STATIC;
			memset(pTable->nPlayerAry, 0, sizeof(pTable->nPlayerAry));
			
			NotifyDarkLooker(this,0, GR_MATCH_DISBAND, &dt, sizeof(dt));
		}
	}
	m_pTcMatch->BreakOffMatch();
}

void CBaseRoomData::AT_ContinueMatch()
{
	if (!m_pTcMatch)  return;
	if (!m_pTcMatch->IsMatchStart()) return;
	if (m_pTcMatch->IsMatchEnd()) return;

	m_pTcMatch->ContinueMatch();

	int count=m_pTcMatch->GetTotalUserCount();
	int nStuctSize=sizeof(MATCH_PLAYER)+sizeof(USERONMGAME)+sizeof(USERONMATCH)+sizeof(MATCH_HINT);
	int nTotalLen=sizeof(MATCH_RECOVER)+nStuctSize*count;

	BYTE* pData=new BYTE[nTotalLen];
	memset(pData,0,nTotalLen);
	POSITION pos=m_pTcMatch->m_mapPlayer.GetStartPosition();
	int nUserID=0;
	CTcMatchPlayer* pPlayer=NULL;

	LPMATCH_RECOVER pReCover=(LPMATCH_RECOVER)pData;

	pReCover->nPlayerCount=0;
	pReCover->nGameID=GetGameID();
	pReCover->nMatchID=GetMatchID();
	pReCover->nRoomID=GetRoomID();

	while(pos)
	{
		m_pTcMatch->m_mapPlayer.GetNextAssoc(pos,nUserID,pPlayer);
		if (pPlayer)
		{
			LPMATCH_PLAYER pMatchPlayer=(LPMATCH_PLAYER)(pData+nStuctSize*pReCover->nPlayerCount+sizeof(MATCH_RECOVER));
			LPUSERONMGAME  pUserOnGame=(LPUSERONMGAME)(PBYTE(pMatchPlayer)+sizeof(MATCH_PLAYER));
			LPUSERONMATCH  pUserOnMatch=(LPUSERONMATCH)(PBYTE(pMatchPlayer)+sizeof(MATCH_PLAYER)+sizeof(USERONMGAME));
			LPMATCH_HINT   pMatchHint=(LPMATCH_HINT)(PBYTE(pMatchPlayer)+sizeof(MATCH_PLAYER)+sizeof(USERONMGAME)+sizeof(USERONMATCH));

			memcpy(pMatchPlayer,&pPlayer->match_player,sizeof(MATCH_PLAYER));
			memcpy(pUserOnGame,&pPlayer->userongame,sizeof(USERONMGAME));
			memcpy(pUserOnMatch,&pPlayer->useronmatch,sizeof(USERONMATCH));
			memcpy(pMatchHint,&pPlayer->match_hint,sizeof(MATCH_HINT));
			//填充
			pReCover->nPlayerCount++;
		}
	}

	NotifyOneGameSvr(R2G_MATCH_PLAYER_LIST,pData,nTotalLen, roomdata.nGamePort);
}

void CBaseRoomData::AT_OnMatchTrigger()
{
	if (m_pTcMatch==NULL)
		return ;

 	m_pTcMatch->FreshMatch();
	while(MATCH_STEP* step=m_pTcMatch->PopStep())
	{
		AT_OnMatchStep(step);
		m_pTcMatch->ClearStep(step);
	}
}


void CBaseRoomData::AT_OnMatchStep(LPMATCH_STEP pStep)
{
	switch(pStep->nStepID)
	{
	case MATCH_STEP_WASHPLAYER:
	case MATCH_STEP_USERSTATUS:
	case MATCH_STEP_OVERTIME:
	case MATCH_STEP_WAITFINAL:
    	NotifyOneGameSvr(pStep->nReqID,(void*)pStep->pStepData,pStep->nDataLen, roomdata.nGamePort);
		break;
	case MATCH_STEP_RANKLIST:
		NotifyOneGameSvr(pStep->nReqID,(void*)pStep->pStepData,pStep->nDataLen, roomdata.nGamePort);
		break;
 	case MATCH_STEP_ARREAGETABLE:
		AT_OnArrangeTable((ARRANGE_TABLE*)pStep->pStepData);
 		break;
 	case MATCH_STEP_DISBANDTABLE:
		AT_OnDisbandTable((DISBAND_TABLE*)pStep->pStepData);
 		break;
	case MATCH_STEP_TERMINATE_TABLE:
		AT_OnTerminateTable((TERMINATE_TABLE*)pStep->pStepData);
		break;
	case MATCH_STEP_RF_USERSCORE:
		AT_OnReFreshUserScore((LPMATCH_REFRESH_USERSCORE)pStep->pStepData);
		break;
	case MATCH_STEP_MATCHEND:
		{
			NotifyOneGameSvr(pStep->nReqID,(void*)pStep->pStepData,pStep->nDataLen, roomdata.nGamePort);
			AT_OnMatchEnd();
		}
		break;
	}
 
	if (pStep->nStepID==MATCH_STEP_FINIEDMATCH)
	{
		//从数据库中读出奖励信息
		MATCH_USERGAIN* pGain=(MATCH_USERGAIN*)pStep->pStepData;
		int nUserID=pGain->nUserID;
		int nRank=pGain->nRank;
		int nMatchID=pGain->nMatchID;
		int nRoomID=pGain->nRoomID;
		int nMatchStatus=pGain->nStatus;
		AT_OnMatchFinished(nUserID,nRank,nMatchID,nRoomID,nMatchStatus);
	} 
   
}

void CBaseRoomData::AT_OnMatchTableOverTime(int nTalbeNO)
{
	MATCH_OVERTIME mo;
	memset(&mo,0,sizeof(MATCH_OVERTIME));
	mo.nRoomID=roomdata.nRoomID;
	mo.nMatchID=roomdata.nMatchID;
	mo.nTableNO=nTalbeNO;

	NotifyOneGameSvr(R2G_MATCH_OVER_TIME,&mo,sizeof(MATCH_OVERTIME),roomdata.nGamePort);
}

void CBaseRoomData::AT_OnMatchResetAllTable()
{
	if(m_pTcMatch==NULL)
		return ;
	POSITION pos=m_pTcMatch->m_mapTable.GetStartPosition();
	int nTableNO=0;
	CTcMatchTable* pTable=NULL;
	while(pos)
	{
		m_pTcMatch->m_mapTable.GetNextAssoc(pos,nTableNO,pTable);
		if (pTable
			&&pTable->m_nStatus==TCMATCH_TABLE_PLAYING)
		{
			AT_OnMatchTableOverTime(nTableNO);
		}
	}
}

void CBaseRoomData::AT_OnReFreshUserScore(LPMATCH_REFRESH_USERSCORE lpReFreshUserScore)
{	
	for(int i = 0; i < lpReFreshUserScore->nCount; i++)
	{
		LPMATCH_USERSCORE lpUserScore = LPMATCH_USERSCORE((PBYTE)lpReFreshUserScore + sizeof(MATCH_REFRESH_USERSCORE) + i * sizeof(MATCH_USERSCORE));
		LPPLAYER lpPlayer = NULL;
		m_mapPlayer.Lookup(lpUserScore->nUserID, lpPlayer);
		if( lpPlayer)
		{
			lpPlayer->nScore      = lpUserScore->nScore;
			lpPlayer->nDeposit    = lpUserScore->nDeposit;
			lpPlayer->pe.nRank    = lpUserScore->nRank;
			//通知玩家大厅
			PLAYER_READY pr;
			ZeroMemory(&pr,sizeof(pr));
			pr.nUserID =lpUserScore->nUserID;
			pr.nRank   =lpUserScore->nRank ;
			pr.nScore  =lpUserScore->nScore;
			pr.nDeposit=lpUserScore->nDeposit;

			NotifyOnePlayer(this,lpUserScore->nUserID,GR_MATCH_NEWSCORE, &pr, sizeof(pr));
		}
	}
	
	NotifyMatchSvr(GR_MATCH_USERSCORE,lpReFreshUserScore,sizeof(MATCH_REFRESH_USERSCORE)+sizeof(MATCH_USERSCORE)*lpReFreshUserScore->nCount);
	//通知管理员
    NotifyDarkLooker(this,0,GR_MATCH_NEWSCORE,lpReFreshUserScore,sizeof(MATCH_REFRESH_USERSCORE)+sizeof(MATCH_USERSCORE)*lpReFreshUserScore->nCount);
}

void CBaseRoomData::AT_SignUp(LPMATCH_ASKSIGNUP lpAskSignUp)
{
	int nUserID=lpAskSignUp->nUserID;

	LPPLAYER lpPlayer = NULL;
	if(m_mapPlayer.Lookup(nUserID, lpPlayer))
	{
		lpAskSignUp->nGameID=roomdata.nGameID;
		lpAskSignUp->nMatchID=roomdata.nMatchID;
		lpAskSignUp->dwIPAddr=lpPlayer->pe.dwIPAddr;
		NotifyMatchSvr(GR_MATCH_ASKSIGNUP,lpAskSignUp,sizeof(MATCH_ASKSIGNUP));
	}

	
}


void CBaseRoomData::AT_GiveUp(LPMATCH_ASKGIVEUP lpAsGiveUp)
{

	LPPLAYER lpPlayer = NULL;
	if(m_mapPlayer.Lookup(lpAsGiveUp->nUserID, lpPlayer))
	{
		lpAsGiveUp->nGameID=roomdata.nGameID;
		lpAsGiveUp->nMatchID=roomdata.nMatchID;
		lpAsGiveUp->dwIPAddr=lpPlayer->pe.dwIPAddr;
		NotifyMatchSvr(GR_MATCH_ASKGIVEUP,lpAsGiveUp,sizeof(MATCH_ASKGIVEUP));
	}
	
}

void CBaseRoomData::AT_OnSocketClose(LPPLAYER pPlayer)
{
	LPTABLE lpTable = NULL;
	if(pPlayer->nStatus == PLAYER_STATUS_PLAYING)
	{
		return;
	}
			
	if (m_pTcMatch)
	{
		CTcMatchPlayer* pTcPlayer=m_pTcMatch->GetPlayer(pPlayer->nUserID);
		if (pTcPlayer
			&&m_pTcMatch->IsMatchStart()
			&&!m_pTcMatch->IsMatchEnd()
			&&!pTcPlayer->IsQuit())
		{
			return;
		}
	}

	// 不是正在玩的状态，清除信息，离开房间
	m_mapPlayer.RemoveKey(pPlayer->nUserID);
	DeletePrivateTableCache(pPlayer->nUserID);
	RemoveClientInfo(pPlayer->nUserID);		

	PLAYER_POSITION pp;
	memset(&pp,0,sizeof(PLAYER_POSITION));
	pp.nUserID=pPlayer->nUserID;
	pp.nTableNO=pPlayer->nTableNO;
	pp.nChairNO=pPlayer->nChairNO;
	NotifyDarkLooker(this,pp.nUserID,GR_PLAYER_LEFT, &pp, sizeof(PLAYER_POSITION));		

	FreePlayerBlock(pPlayer);
	

}

void CBaseRoomData::AT_OnMatchEnd()
{ 
	int nMatchID=roomdata.nMatchID;
	NotifyRoomPlayers(this, GR_MATCH_FINISHED,&nMatchID,sizeof(int));
 
	MATCH_END me;
	memset(&me,0,sizeof(me));
	me.nGameID=GetGameID();
	me.nRoomID=GetRoomID();
	me.nMatchID=GetMatchID();
	me.nRunTime=UwlGetCurrentSeconds()-m_pTcMatch->m_nMatchStartSecond;
 
	NotifyMatchSvr(GR_MATCH_END,&me,sizeof(me));
}
 

void CBaseRoomData::AT_OnMatchFinished(int nUserID,int nRank,int nMatchID,int nRoomID,int nMatchStatus)
{
	//根据名次计算奖励

	MATCH_ASKGAIN ask_gain;
	memset(&ask_gain,0,sizeof(MATCH_ASKGAIN));
	ask_gain.nGameID=GetGameID();
	ask_gain.nUserID=nUserID;
	ask_gain.nRank=nRank;
	ask_gain.nMatchID=nMatchID;
	ask_gain.nRoomID=nRoomID;
	ask_gain.nStatus=nMatchStatus;
	ask_gain.nScoreGain=m_pTcMatch->CalcWinPointByRank(nUserID,nRank)*m_pTcMatch->CalcBaseDuanScore();
	
	NotifyMatchSvr(GR_MATCH_ASKGAIN,&ask_gain,sizeof(MATCH_ASKGAIN));//向CheckSvr请求获取奖励
	NotifyOneGameSvr(R2G_MATCH_FINISHED,&ask_gain,sizeof(MATCH_ASKGAIN), roomdata.nGamePort);//通知游戏客户端用户的比赛已结束
 	//填写奖励
}


void CBaseRoomData::AT_OnMatchPlayerOK(LPMATCH_PLAYER_OK pPlayerOk)
{
	
	int userid=pPlayerOk->nUserID;
	PLAYER* lpPlayer=NULL;
	if (m_mapPlayer.Lookup(userid,lpPlayer))
	{
		lpPlayer->nStatus=PLAYER_STATUS_STAND;

		PLAYER_POSITION pp;
		ZeroMemory(&pp,sizeof(pp));
		pp.nUserID=lpPlayer->nUserID;

		NotifyDarkLooker(this, 0,GR_PLAYER_STAND, &pp, sizeof(PLAYER_POSITION));
	}

	if (!m_pTcMatch )
		return;

	m_pTcMatch->OnPlayerOK(pPlayerOk);
	while(MATCH_STEP* step=m_pTcMatch->PopStep())
	{
		AT_OnMatchStep(step);
		m_pTcMatch->ClearStep(step);
	}
}

void CBaseRoomData::AT_OnMatchUserReady(LPMATCHUSER_READY lpMatchUserReady)
{
 	LPPLAYER pPlayer = NULL;
	if(!m_mapPlayer.Lookup(lpMatchUserReady->nUserID, pPlayer)) 
		return ;
// 	if(pPlayer->pe.lTokenID!=lpMatchUserReady->nRoomTokenID)
// 		return ;
	if(IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_LOOKDARK))
		return ;
   	if(IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))
		return ;

	pPlayer->nTableNO=-1;
	pPlayer->nChairNO=-1;
	pPlayer->nStatus=PLAYER_STATUS_READY;

	PLAYER_POSITION  pp;
	memset(&pp,0,sizeof(pp));
	pp.nUserID=pPlayer->nUserID;
	NotifyOnePlayer(this,pp.nUserID,GR_PLAYER_READY, &pp, sizeof(pp));

	{
		PLAYER_READY pr;
		memset(&pr,0,sizeof(PLAYER_READY));
		pr.nUserID   =  pPlayer->nUserID;
		pr.nStatus   =  PLAYER_STATUS_READY;
		pr.nDeposit  =  pPlayer->nDeposit;
		pr.nScore    =  pPlayer->nScore;
		NotifyDarkLooker(this,0,GR_PLAYER_READY,&pr,sizeof(pr));
	}
 	//////////////////////////////////////////////////////////////////////
 	//////////////////////////////////////////////////////////////////////

	if (!m_pTcMatch) return;
	m_pTcMatch->OnPlayerReady(lpMatchUserReady);
}

void CBaseRoomData::AT_OnKickPlayer(int nUserID)
{
	LPPLAYER pPlayer = NULL;
	if(!m_mapPlayer.Lookup(nUserID, pPlayer)) 
		return;

	if(IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_LOOKDARK))
		return ;


	MATCH_KICKPLAYER mk;
	memset(&mk,0,sizeof(MATCH_KICKPLAYER));
	mk.nMatchID=roomdata.nMatchID;
	mk.nRoomID=roomdata.nRoomID;
	mk.nTableNO=pPlayer->nTableNO;
	mk.nChairNO=pPlayer->nChairNO;
	mk.nUserID=nUserID;
	strcpy(mk.szMsg,"您已经被管理员踢出比赛!");

	if(!m_pTcMatch)
		return ;
	
	MATCHUSER_LEAVE ml;
	memset(&ml,0,sizeof(MATCHUSER_LEAVE));
	
	ml.nMatchID=roomdata.nMatchID;
	ml.nRoomID=roomdata.nRoomID;
	ml.nUserID=nUserID;

	m_pTcMatch->OnPlayerLeave(&ml);

	pPlayer->nTableNO=-1;
	pPlayer->nChairNO=-1;
	pPlayer->nStatus=PLAYER_STATUS_WALKAROUND;
	
	PLAYER_POSITION  pp;
	memset(&pp,0,sizeof(pp));
	pp.nUserID=pPlayer->nUserID;
   	NotifyOnePlayer(this,pp.nUserID,GR_PLAYER_UNREADY, &pp, sizeof(pp));
	NotifyDarkLooker(this,0,GR_PLAYER_UNREADY,&pp,sizeof(pp));
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	NotifyOneGameSvr(R2G_MATCH_KICKPLAYER,&mk,sizeof(MATCH_KICKPLAYER),roomdata.nGamePort);
}

void CBaseRoomData::AT_OnMatchUserLeave(LPMATCHUSER_LEAVE lpMatchUserLeave)
{
 	LPPLAYER pPlayer = NULL;
	if(!m_mapPlayer.Lookup(lpMatchUserLeave->nUserID, pPlayer)) 
		return ;
// 	if(pPlayer->pe.lTokenID!=lpMatchUserLeave->nRoomTokenID)
// 		return ;
	if(IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_LOOKDARK))
		return ;
   	if(IS_BIT_SET(pPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))
		return ;
	pPlayer->nTableNO=-1;
	pPlayer->nChairNO=-1;
	pPlayer->nStatus=PLAYER_STATUS_WALKAROUND;

	PLAYER_POSITION  pp;
	memset(&pp,0,sizeof(pp));
	pp.nUserID=pPlayer->nUserID;
   	NotifyOnePlayer(this,pp.nUserID,GR_PLAYER_UNREADY, &pp, sizeof(pp));
	NotifyDarkLooker(this,0,GR_PLAYER_UNREADY,&pp,sizeof(pp));
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
 	if(!m_pTcMatch)
		return ;
 	m_pTcMatch->OnPlayerLeave(lpMatchUserLeave);
}


void CBaseRoomData::AT_OnCommitDiff(LPCOMMIT_DIFF lpCommitDiff)
{
	if(!m_pTcMatch)
		return ;

	if (m_pTcMatch->CommitResult(lpCommitDiff))
	{
		lpCommitDiff->nMatchID=GetMatchID();
		for(int i = 0; i < lpCommitDiff->nDiffCount; i++)
		{
			LPUSER_DIFF lpUserDiff = LPUSER_DIFF((PBYTE)lpCommitDiff + sizeof(COMMIT_DIFF) + i * sizeof(USER_DIFF));
			LPPLAYER lpPlayer = NULL;
			m_mapPlayer.Lookup(lpUserDiff->nUserID, lpPlayer);
			if( lpPlayer)
			{
				if (m_pTcMatch->IsPlayerSeated(lpPlayer->nUserID))
					lpPlayer->nStatus		=     PLAYER_STATUS_SEATED; 
				else
					lpPlayer->nStatus		=     PLAYER_STATUS_STAND;

				lpPlayer->nBout       += lpUserDiff->nBoutDiff;
				lpPlayer->nBreakOff   += lpUserDiff->nBreakOffDiff;
				lpPlayer->nDeposit    += lpUserDiff->nDepositDiff;
				lpPlayer->nScore      += lpUserDiff->nScoreDiff;
				lpPlayer->nLoss       += lpUserDiff->nLossDiff;
				lpPlayer->nStandOff   += lpUserDiff->nStandOffDiff;
				lpPlayer->nTimeCost   += lpUserDiff->nTimeCostDiff;
				lpPlayer->nWin        += lpUserDiff->nWinDiff;

				lpPlayer->pe.nRank     = lpUserDiff->nRank;
				
				PLAYER_READY pr;
				ZeroMemory(&pr,sizeof(pr));
				pr.nUserID =lpUserDiff->nUserID;
				pr.nRank   =lpUserDiff->nRank ;
				pr.nScore  =lpPlayer->nScore;
				pr.nDeposit=lpPlayer->nDeposit;

				NotifyOnePlayer(this,pr.nUserID,GR_MATCH_BOUTEND, &pr, sizeof(pr));
			}
			
		}
		
		int nDataLen=sizeof(COMMIT_DIFF)+sizeof(USER_DIFF)*lpCommitDiff->nDiffCount;
		NotifyDarkLooker(this,0,GR_MATCH_BOUTEND,lpCommitDiff,nDataLen  );

		NotifyMatchSvr(GR_MATCH_COMMITDIFF,lpCommitDiff,nDataLen);
	}
	
	while(MATCH_STEP* step=m_pTcMatch->PopStep())
	{
		AT_OnMatchStep(step);
		m_pTcMatch->ClearStep(step);
	}

}


void CBaseRoomData::AT_OnEnterMatchOK(LPENTER_MATCH_RETURN lpEnterMatchReturn)
{
	LPMATCH_PLAYER lpMatchPlayer = LPMATCH_PLAYER(PBYTE(lpEnterMatchReturn)+ sizeof(ENTER_MATCH_RETURN) );
	LPUSERONMGAME  lpUserOnMGame = LPUSERONMGAME(PBYTE(lpMatchPlayer)+ sizeof(MATCH_PLAYER) );
	LPUSERONMATCH  lpUserOnMatch = LPUSERONMATCH(PBYTE(lpUserOnMGame)+ sizeof(USERONMGAME) );
	LPMATCH_HINT   lpMatchHint =   LPMATCH_HINT(PBYTE(lpUserOnMatch)+  sizeof(USERONMATCH) );

	LPPLAYER pPlayer = NULL;
	if(!m_mapPlayer.Lookup(lpUserOnMatch->nUserID, pPlayer)) 
		return;

	if (!m_pTcMatch)
		return;

	//在数据库没有更新的情况下，暂时用TCMATCH的状态替代
	/////////////////////////////////////////////////////
	lpMatchHint->nStatus=m_pTcMatch->m_nMatchStatus;
	/////////////////////////////////////////////////////

	strcpy(pPlayer->szUsername,lpMatchPlayer->szUsername);
	strcpy(pPlayer->szNickName,lpMatchPlayer->szNickName);

	pPlayer->nScore=lpUserOnMatch->nScore;
	pPlayer->nDeposit=lpUserOnMatch->nDeposit;
	pPlayer->nWin=lpUserOnMatch->nWin;
 	pPlayer->nLoss=lpUserOnMatch->nLoss;
	pPlayer->nStandOff=lpUserOnMatch->nStandOff;
	pPlayer->nBout=lpUserOnMatch->nBout;
 	pPlayer->nBreakOff=lpUserOnMatch->nBreakOff;
	pPlayer->nTimeCost=lpUserOnMatch->nTimeCost;

	pPlayer->pe.nDuan=lpUserOnMGame->nDuan;
	pPlayer->pe.nDuanScore=lpUserOnMGame->nDuanScore;
	pPlayer->pe.nRank=lpUserOnMatch->nRank;
	
 	m_pTcMatch->CreatePlayer(lpMatchPlayer,lpUserOnMGame,lpUserOnMatch,lpMatchHint);
}

int  CBaseRoomData::AT_GetMatchUserStatus(LPENTER_MATCH_RETURN lpEnterMatchReturn)
{
	LPMATCH_PLAYER lpMatchPlayer = LPMATCH_PLAYER(PBYTE(lpEnterMatchReturn)+ sizeof(ENTER_MATCH_RETURN) );
	int nUserID=lpMatchPlayer->nUserID;
	if (!m_pTcMatch)
		return GR_ENTER_MATCH_NOTFOUND;

	if (!m_pTcMatch->IsMatchStart())
		return GR_ENTER_MATCH_OK;//初次加入,比赛还未开始

	CTcMatchPlayer* pPlayer=m_pTcMatch->GetPlayer(nUserID);
	if (!pPlayer)
		return GR_ENTER_MATCH_NOTFOUND;

	if (pPlayer->IsQuit())
		return GR_ENTER_MATCH_WASHOUT;

	if (pPlayer->IsPlaying())
 		return GR_ENTER_MATCH_DXXW;


	return GR_ENTER_MATCH_WAITARRANGETABLE;
}

void CBaseRoomData::AT_SendUserRankList(int nUserID)
{
	if(!m_pTcMatch)
		return ;
	//
	m_pTcMatch->CreateUserRankList(nUserID);
	while(MATCH_STEP* step=m_pTcMatch->PopStep())
	{
		AT_OnMatchStep(step);
		m_pTcMatch->ClearStep(step);
	}
}

void  CBaseRoomData::NotifyMatchSvr(int nNotifyID,void* pData, int nLen,int nUserID)
{
	MATCH_ROOMTOMATCH  mr;
	ZeroMemory(&mr,sizeof(mr));
	
	mr.nNotifyID=nNotifyID;
	mr.nGameID=GetGameID();
	mr.nRoomID=GetRoomID();
	mr.nUserID=nUserID;
	mr.nMatchID=GetMatchID();
	mr.nDataLen=nLen;
	
	mr.nRoomSvrID=g_nClientID;
	
	int nSendLen = sizeof(mr)  + mr.nDataLen;
	PBYTE pSendData = new BYTE[nSendLen];
	memcpy(pSendData,&mr, sizeof(mr));
	memcpy(pSendData + sizeof(mr),pData, nLen);
	
	NotifyServer(GR_MATCH_ROOMTOMATCH, pSendData, nSendLen);
	SAFE_DELETE(pSendData);
}

BOOL CBaseRoomData::GetOpenTimeSection(LPCTSTR lpszIniFile,OPENTIME_SECTION* pots)
{
	pots->nRoomID=GetRoomID();
	pots->nSectionCount=0;

	char szSection[32];
	char szBegin[16];
	char szEnd[16];
	memset(szSection,0,sizeof(szSection));
	memset(szBegin,0,sizeof(szBegin));
	memset(szEnd,0,sizeof(szEnd));

	sprintf(szSection,"OpenTime%d",pots->nRoomID);
	
	int curDate = 0;
	int curTime = 0;
	int result  = UwlGetCurrentDateTime(curDate, curTime);
	curTime=curTime/100;//分钟
	
 	int nBeginDate=GetPrivateProfileInt(szSection,"begindate",-1,lpszIniFile);
	int nBeginTime=GetPrivateProfileInt(szSection,"begintime",-1,lpszIniFile);
	int nEndDate=GetPrivateProfileInt(szSection,"enddate",-1,lpszIniFile);
	int nEndTime=GetPrivateProfileInt(szSection,"endtime",-1,lpszIniFile);
	pots->nBeginDate=nBeginDate;
	pots->nBeginTime=nBeginTime;
	pots->nEndDate=nEndDate;
	pots->nEndTime=nEndTime;

	if (nBeginDate==-1 ||nBeginTime==-1	||nEndDate==-1 ||nEndTime==-1)
		return TRUE;

	if (curDate<nBeginDate || (curDate==nBeginDate && curTime<nBeginTime))
	{
		pots->nMatchStatus=RANKMATCH_STATUS_UNSTART;

		pots->nRestTime=xyGetSecondsByDateTime(nBeginDate,nBeginTime*100)-UwlGetCurrentSeconds();//距离比赛开始的时间,单位秒
	}
	else if (curDate>nEndDate ||(curDate==nEndDate 	&& curTime>nEndTime))
	{
		pots->nMatchStatus=RANKMATCH_STATUS_FINISHED;
	}
	else 
	{
		pots->nMatchStatus=RANKMATCH_STATUS_STARTED;
	}


	int nMaxOpenSection=8;//最多支持8个时间段
	int nBegin,nEnd;
	
	for(int i=0;i<nMaxOpenSection;i++)
	{
		sprintf(szBegin,"begin%d",i);
		sprintf(szEnd,"end%d",i);
		
		nBegin = GetPrivateProfileInt(szSection,szBegin,-1,lpszIniFile);
		nEnd = GetPrivateProfileInt(szSection,szEnd,-1,lpszIniFile);
		if (nBegin==-1||nEnd==-1)//检索时间段失败
			break;

		pots->nBeginSection[i]=nBegin;
		pots->nEndSection[i]=nEnd;
		pots->nSectionCount++;
	}

	
	if (pots->nMatchStatus==RANKMATCH_STATUS_FINISHED
		||pots->nMatchStatus==RANKMATCH_STATUS_UNSTART)
		return FALSE;


	int curSeconds=UwlGetCurrentSeconds();
	pots->nRestTime=-1;

	for(i=0;i<pots->nSectionCount;i++)
	{
		if (pots->nBeginSection[i]<=curTime
			&&curTime<=pots->nEndSection[i])
		{
			return TRUE;
		}
		else 
		{
			int nSectionRest=xyGetSecondsByDateTime(curDate,pots->nBeginSection[i]*100)-curSeconds;
		
			if (nSectionRest<0)
				nSectionRest+=24*60*60;

			if (pots->nRestTime==-1
				||nSectionRest<pots->nRestTime)
			{
				pots->nRestTime = nSectionRest;
			}
		}
	}

	if (i==0)
	{
		return TRUE; //没有对应配置文件,则为所有时段全开放
	}
	else
	{
		return FALSE;//有对应配置文件,但未处于开放时段中
	}
}


CString CBaseRoomData::GetOutOfOpenTimeCause(LPCTSTR lpszIniFile,OPENTIME_SECTION* pots)
{
  	TCHAR szMatchName[256];
	memset(szMatchName,0,sizeof(szMatchName));
	GetPrivateProfileString("MatchName",xyConvertIntToStr(pots->nRoomID), "",szMatchName,sizeof(szMatchName) ,lpszIniFile);

	CString sMsg,sHead,sOpenTime,sTmp,sRestTime;
 
	sHead.Format("%s\r\n(%s ～ %s)",szMatchName,FormatDateAndTime(pots->nBeginDate,pots->nBeginTime*100),FormatDateAndTime(pots->nEndDate,pots->nEndTime*100));

	for(int i=0;i<pots->nSectionCount;i++)
	{
		int nBegin=pots->nBeginSection[i];
		int nEnd=pots->nEndSection[i];
		sTmp.Format(_T("%02d:%02d ～ %02d:%02d\r\n"),nBegin/100,nBegin%100,nEnd/100,nEnd%100);
		sOpenTime+=sTmp;
	}
	if(!sOpenTime.IsEmpty())
		sOpenTime=_T("开放时段如下：\r\n\r\n")+sOpenTime;

	if(pots->nMatchStatus==RANKMATCH_STATUS_UNSTART)
	{
		sMsg=sHead+_T("\r\n\r\n比赛还没开始。")+sOpenTime;

		sRestTime.Format("\r\n距离比赛开始：%s",xyFormatTime_Rest(pots->nRestTime));

		sMsg+=sRestTime;
	}
	else if (pots->nMatchStatus==RANKMATCH_STATUS_STARTED)
	{
		sMsg=sHead+_T("\r\n\r\n比赛还没开始。")+sOpenTime;

		sRestTime.Format("\r\n距离下一场比赛开始：%s",xyFormatTime_Rest(pots->nRestTime));
		
		sMsg+=sRestTime;
	}
	else if (pots->nMatchStatus==RANKMATCH_STATUS_FINISHED)
	{
		sMsg=sHead+_T("\r\n\r\n比赛已经结束。");
	}
	return sMsg;
}


CString CBaseRoomData::FormatDateAndTime(int nDate,int nTime)
{
	CString sReturn;
	if(nDate==0 )
		return sReturn ;
	
	CString sDate,sTime;
	sDate.Format(_T("%08d"),nDate);
	sTime.Format(_T("%06d"),nTime);
	
	int nYear,nMonth,nDay;
	nYear=_ttoi(sDate.Left(4));
	nMonth=_ttoi(sDate.Mid(4,2));
	nDay=_ttoi(sDate.Right(2));
	
	int nHour,nMinute;
	nHour=_ttoi(sTime.Left(2));
	nMinute=_ttoi(sTime.Mid(2,2));
    sReturn.Format(_T("%d月%d日,%02d:%02d"),nMonth,nDay,nHour,nMinute);
	return sReturn;
}

BOOL CBaseRoomData::GetTableDeposit(int nTableNO,int& nMinDeposit,int& nMaxDeposit,CString& sRemark)
{
	TCHAR szSection[32];
	memset(szSection,0,sizeof(szSection));
	sprintf(szSection,"TableDeposit%d",roomdata.nRoomID);
 	 
 	int nCount = GetPrivateProfileInt(szSection,_T("Count"),0, g_szIniFile);
	if(nCount<=0)
		return FALSE;
	if(nCount>MAX_TABLEDEPOSIT_COUNT)
		return FALSE;
	
	BOOL bSupportMinMax = FALSE;
	nMaxDeposit = 2147483646;
	CString sTmp;
	for(int i=0;i<nCount;i++)
	{
		TCHAR szValue[64];
		memset(szValue,0,sizeof(szValue));
	 	GetPrivateProfileString(szSection,xyConvertIntToStr(i),_T(""),szValue,sizeof(szValue) ,g_szIniFile);	
		if(szValue[0]==0)
			return FALSE;
		TCHAR *fields[8];
		memset(fields,0,sizeof(fields));
 		TCHAR *p1, *p2;
		p1=szValue;
		int nFields=xyRetrieveFields ( p1, fields, 8, &p2 ) ;
		if(nFields<3)
			return FALSE;

		int nTableNO1=atoi(fields[0]);
		int nTableNO2=atoi(fields[1]);
		int nTheMinDeposit=atoi(fields[2]);
 		if(nTableNO>=nTableNO1 && nTableNO<=nTableNO2)
		{
			nMinDeposit=nTheMinDeposit;
			if (nFields>3)// 如果设置了上限
			{
				nMaxDeposit=atoi(fields[3]);
			}
		}

		if (nFields>3)// 如果设置了上限
		{
			bSupportMinMax = TRUE;
			int nTheMaxDeposit=atoi(fields[3]);
			sTmp.Format(_T("%d ～ %d 桌，需要银两在%s两 ～ %s两之间\r\n"),nTableNO1+1,nTableNO2+1,
				FormatDeposit(nTheMinDeposit),FormatDeposit(nTheMaxDeposit));
		}
		else//否则兼容以前的格式
		{
			//  该提示不能更改，否则大厅认不出来
			sTmp.Format(_T("%d ～ %d 桌，需要%s两以上\r\n"),nTableNO1+1,nTableNO2+1,FormatDeposit(nTheMinDeposit));
		}
		

		sRemark+=sTmp;
	}
	return bSupportMinMax ? 2 : TRUE;
}
void CBaseRoomData::RemoveClientInfo(int nUserID)
{
	CClientInfo* pClientInfo=NULL;
	if(m_mapClientInfo.Lookup(nUserID,pClientInfo))
	{
		m_mapClientInfo.RemoveKey(nUserID);
		SAFE_DELETE(pClientInfo);
	}
}

BOOL CBaseRoomData::IsSameLAN(int nUserID1,int nUserID2)
{
 	CClientInfo* pci1=NULL;
 	CClientInfo* pci2=NULL;
	
	m_mapClientInfo.Lookup(nUserID1,pci1);
 	m_mapClientInfo.Lookup(nUserID2,pci2);

	if(pci1==NULL || pci2==NULL)
		return FALSE;
 
	
    POSITION pos= pci1->mapARPComb.GetStartPosition();
    POSITION pos2=pci2->mapARPComb.GetStartPosition();
	if(pos==NULL || pos2==NULL)
		return FALSE;

	while (pos)	
	{
		CString sKey;
		int nValue;
		pci1->mapARPComb.GetNextAssoc(pos, sKey, nValue);

		if(pci2->mapARPComb.Lookup(sKey,nValue))
			return TRUE;
	}
 
	return FALSE;
}



int CBaseRoomData::GetFreeChairOnTable(LPTABLE pTable)
{
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
		if(0 == pTable->nPlayerAry[i])
			return i;
	}
	
	return -1;
}

BOOL CBaseRoomData::IsAllStartedOnTable(LPTABLE pTable)
{
	LPPLAYER pPlayer = NULL;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
		if(pTable->nPlayerAry[i]>0)
		{
			if(!m_mapPlayer.Lookup( pTable->nPlayerAry[i], pPlayer)) 
				return FALSE;
			if (pPlayer->nStatus != PLAYER_STATUS_WAITING)
				return FALSE;
		}
	}
	
	return TRUE;
}
BOOL CBaseRoomData::GetPreferPrivateTableChair(LPPLAYER pPlayer,int& nTableNo,int& nChairNo, DWORD dwIPAddr, int nIPConfig, int nMinScore, int nMinDeposit,int ExcludedHomeID)
{
	//第一次进房间，尽量安排尽快开始的桌位
	//A：差1人，且都已点击开始 waiting状态
	//B：差2人，且都已点击开始 waiting状态
	//C：差3人，且都已点击开始 waiting状态
	//D：差1人
	//E：差2人
	//F：差3人
	//G：至少有1个人的位置

	int nTableNoA = -1;
	int nTableNoB = -1;
	int nTableNoC = -1;
	int	nTableNoD = -1;
	int nTableNoE = -1;
	int nTableNoF = -1;
	int	nTableNoG = -1;

	float fBreakOff = 0;
	int bout = pPlayer->nWin + pPlayer->nLoss + pPlayer->nStandOff;
	if(bout)
		fBreakOff = (float)pPlayer->nBreakOff * 100 / bout;
	int nNetSpeed = pPlayer->nNetSpeed;	
	int nMaxTableCount = GetTableCount();
	int nTotalChair = GetChairCount();
	int nMutexExpiration = 0;
	LPTABLE lpTable = NULL;
	BOOL bFindSeat = FALSE;
	int nStartedPlayer = 0;
	int nNowSeconds = UwlGetCurrentSeconds();

	for (int i=0;i<nMaxTableCount;i++)
	{
		//从当前座位往后查找到第一个符合的位置
		nTableNo = i;

		if (m_mapTable.Lookup(nTableNo, lpTable))
		{
			if (lpTable->th.nPlayerCount>0
				&& lpTable->th.nPlayerCount<nTotalChair)
				//&& (nTotalChair-lpTable->th.nPlayerCount<3)	//只差一个人的桌位
			{
				// 检查IP
				if (IsForbidIPOnTable(pPlayer->nUserID,lpTable,dwIPAddr,nIPConfig))
					continue;
				//检查IP互斥
				if (IsMutexIPOnTable(pPlayer->nUserID,lpTable,dwIPAddr))
					continue;
				//检查IP组互斥
				if (IsMutexIPGroupOnTable(pPlayer->nUserID,lpTable,dwIPAddr))
					continue;
				//检查局域网同桌
				if(IsSameLANOnTable(pPlayer->nUserID,lpTable))
					continue;
				// 检查断线率
				if (IsForbidBreakOnTable(pPlayer->nUserID,lpTable,fBreakOff))
					continue;
				// 检查网络延迟
				if (IsForbidSpeedOnTable(pPlayer->nUserID,lpTable,nNetSpeed))
					continue;
				if(lpTable->th.nStatus==TABLE_STATUS_PLAYING && IsEarlyLeave())
					continue;
				if(!IsOldMatchRoom())
				{
					// 检查积分限制是否允许 
					if(IsForbidMinScoreOnTable(pPlayer,lpTable,nMinScore))
						continue;
					// 检查银子限制是否允许
					if(IsForbidMinDepositOnTable( pPlayer,lpTable,nMinDeposit))
						continue;
					// 检查桌银限制
					if(IsTableDeposit())
					{
						int nMinDeposit=0;
						int nMaxDeposit=0;
						CString sRemark;
						GetTableDeposit(nTableNo,nMinDeposit,nMaxDeposit,sRemark);
						if( pPlayer->nDeposit < nMinDeposit || pPlayer->nDeposit > nMaxDeposit)
						continue;
					}
				}
				// 检查桌子密码
				if(lpTable->th.bHavePassword)
					continue;

				if (!IsOpenPrivateTable(lpTable))
					continue;

				if ( ExcludedHomeID>0 && lpTable->nHomeUserID == ExcludedHomeID)
					continue;
				
				// 检查硬件互斥
				if(IsMutexHardOnTable(pPlayer->nUserID,pPlayer->pe.szMachineID,lpTable,nMutexExpiration))
					continue;
				// 检查踢人关系
				KICKOFF_DETAIL kickDetail;
				ZeroMemory(&kickDetail, sizeof(kickDetail));
				int nRestSeconds = 0;
				if(IsIsolateOnTable(pPlayer->nUserID,lpTable,nNowSeconds,kickDetail,nRestSeconds))
					continue;

				//检查不受欢迎
				if(IsGetUnwelcome() && 
					IsUnwelcomesOnTable(pPlayer->nUserID, nTableNo))
					continue;

				//A：差1人，且都已点击开始 waiting状态
				int nStartedPlayer = GetStartedPlayerCountOnTable(lpTable);
				if (nTotalChair-nStartedPlayer == 1)
				{
					nTableNoA = nTableNo;
					break;
				}
				//B：差2人，且都已点击开始 waiting状态
				else if (nTotalChair-nStartedPlayer == 2)
				{
					if (nTableNoB<0)
						nTableNoB = nTableNo;
				}
				//C：差3人，且都已点击开始 waiting状态
				else if (nTotalChair-nStartedPlayer == 3)
				{
					if (nTableNoC<0)
						nTableNoC = nTableNo;
				}
				//D：差1人
				else if (nTotalChair-lpTable->th.nPlayerCount==1)
				{
					if (nTableNoD<0)
						nTableNoD = nTableNo;
				}
				//E：差2人
				else if (nTotalChair-lpTable->th.nPlayerCount==2)
				{
					if (nTableNoE<0)
						nTableNoE = nTableNo;
				}
				//F：差3人
				else if (nTotalChair-lpTable->th.nPlayerCount==3)
				{
					if (nTableNoF<0)
						nTableNoF = nTableNo;
				}
				//G：有人就行
				else if (lpTable->th.nPlayerCount>0)
				{
					if (nTableNoG<0)
						nTableNoG = nTableNo;
				}
			}
		}
	}
	
	
	if (nTableNoA>=0)
	{
		if (m_mapTable.Lookup(nTableNoA, lpTable))
		{
			bFindSeat = TRUE;
			nTableNo = nTableNoA;
			nChairNo = GetFreeChairOnTable(lpTable);
		}
	}
	else if (nTableNoB>=0)
	{
		if (m_mapTable.Lookup(nTableNoB, lpTable))
		{
			bFindSeat = TRUE;
			nTableNo = nTableNoB;
			nChairNo = GetFreeChairOnTable(lpTable);
		}
		
	}
	else if (nTableNoC>=0)
	{
		if (m_mapTable.Lookup(nTableNoC, lpTable))
		{
			bFindSeat = TRUE;
			nTableNo = nTableNoC;
			nChairNo = GetFreeChairOnTable(lpTable);
		}
	}
	else if (nTableNoD>=0)
	{
		if (m_mapTable.Lookup(nTableNoD, lpTable))
		{
			bFindSeat = TRUE;
			nTableNo = nTableNoD;
			nChairNo = GetFreeChairOnTable(lpTable);
		}
	}
	else if (nTableNoE>=0)
	{
		if (m_mapTable.Lookup(nTableNoE, lpTable))
		{
			bFindSeat = TRUE;
			nTableNo = nTableNoE;
			nChairNo = GetFreeChairOnTable(lpTable);
		}
	}
	else if (nTableNoF>=0)
	{
		if (m_mapTable.Lookup(nTableNoF, lpTable))
		{
			bFindSeat = TRUE;
			nTableNo = nTableNoF;
			nChairNo = GetFreeChairOnTable(lpTable);
		}
	}
	else if (nTableNoG>=0)
	{
		if (m_mapTable.Lookup(nTableNoG, lpTable))
		{
			bFindSeat = TRUE;
			nTableNo = nTableNoG;
			nChairNo = GetFreeChairOnTable(lpTable);
		}
	}

	//找符合桌银的空桌位
	if (!bFindSeat)
	{
		for (int i=0;!bFindSeat&&i<nMaxTableCount;i++)
		{
			//从当前座位往后查找到第一个符合的位置
			nTableNo = i;
			if(m_mapTable.Lookup(nTableNo,lpTable))
			{
				if(lpTable->th.nStatus==TABLE_STATUS_STATIC && lpTable->th.nPlayerCount==0)
				{
					// 检查桌银限制
					if(IsTableDeposit())
					{
						int nMinDeposit=0;
						int nMaxDeposit=0;
						CString sRemark;
						GetTableDeposit(nTableNo,nMinDeposit,nMaxDeposit,sRemark);
						if( pPlayer->nDeposit < nMinDeposit || pPlayer->nDeposit > nMaxDeposit)
						continue;
					}

					bFindSeat = TRUE;
					nChairNo = 0;
				}
			}
			else
			{
				// 检查桌银限制
				if(IsTableDeposit())
				{
					int nMinDeposit=0;
					int nMaxDeposit=0;
					CString sRemark;
					GetTableDeposit(nTableNo,nMinDeposit,nMaxDeposit,sRemark);
					if( pPlayer->nDeposit < nMinDeposit || pPlayer->nDeposit > nMaxDeposit)
						continue;
				}

				lpTable = new TABLE;
				ZeroMemory(lpTable, sizeof(TABLE));
				lpTable->th.nTableNO = nTableNo;
				lpTable->th.nMinDeposit=0;
				lpTable->th.nMinScore =SCORE_MIN;
				m_mapTable.SetAt(nTableNo, lpTable);
				
				bFindSeat = TRUE;
				nChairNo = 0;
			}
		}

	}

	//不挑了，找空桌位
	if (!bFindSeat)
	{
		for (int i=0;!bFindSeat&&i<nMaxTableCount;i++)
		{
			//从当前座位往后查找到第一个符合的位置
			nTableNo = i;
			if(m_mapTable.Lookup(nTableNo,lpTable))
			{
				if(lpTable->th.nStatus==TABLE_STATUS_STATIC && lpTable->th.nPlayerCount==0)
				{
					bFindSeat = TRUE;
					nChairNo = 0;
				}
			}
			else
			{
				lpTable = new TABLE;
				ZeroMemory(lpTable, sizeof(TABLE));
				lpTable->th.nTableNO = nTableNo;
				lpTable->th.nMinDeposit=0;
				lpTable->th.nMinScore =SCORE_MIN;
				m_mapTable.SetAt(nTableNo, lpTable);
				
				bFindSeat = TRUE;
				nChairNo = 0;
			}
		}
		
	}
	
	return bFindSeat;
}

BOOL CBaseRoomData::GetAppointTableChair(LPPLAYER pPlayer,int nTableNo,int& nChairNo)
{

	LPTABLE lpTable = NULL;
	int nTotalChair = GetChairCount();
	if (!m_mapTable.Lookup(nTableNo, lpTable))
		return FALSE;
	
	if (lpTable->th.nPlayerCount<=0 || lpTable->th.nPlayerCount>=nTotalChair)
		return FALSE;
	
		
	nChairNo = GetFreeChairOnTable(lpTable);
	if (nChairNo<0 || nChairNo>=nTotalChair)
		return FALSE;
			
	return TRUE;
}
void CBaseRoomData::FillCloakingDetail(CLOAKING_DETAIL* pcd)
{
	// x=LOWORD(lpPlayer->pe.dwScreenXY)
	// y=HIWORD(lpPlayer->pe.dwScreenXY)
	TCHAR szRoomID[32];
	memset(szRoomID,0,sizeof(szRoomID));
	sprintf(szRoomID,_T("%d"),roomdata.nRoomID);

	pcd->nRightWidth=GetPrivateProfileInt(_T("CloakingRightWidth"), szRoomID,320, g_szIniFile );
	pcd->nRightHeight=GetPrivateProfileInt(_T("CloakingRightHeight"), szRoomID,400, g_szIniFile );

	GetPrivateProfileString(_T("CloakingLeftURL"),szRoomID,_T(""),pcd->szLeftURL,sizeof(pcd->szLeftURL) ,g_szIniFile);
	GetPrivateProfileString(_T("CloakingRightURL"),szRoomID,_T(""), pcd->szRightURL, sizeof(pcd->szRightURL) , g_szIniFile);
 

	pcd->nRoomID=roomdata.nRoomID;
	pcd->nGameID=roomdata.nGameID;
	pcd->nCurrentSeconds = UwlGetCurrentSeconds();
}

BOOL CBaseRoomData::IsPlayingOnTable(TABLE* pTable)
{
 	return GetPlayingCount(pTable) > 0;
}


void CBaseRoomData::PostUserClothing(int nUserID, int nClothingID)
{
   	HWND hWnd=FindGameSvrWindow();
	if(IsWindow(hWnd))
	{
		PostMessage(hWnd,WM_RTG_USERCLOTHING,nClothingID,nUserID);
	}
}

void CBaseRoomData::PostBecomeMember(int nUserID, int nUserType)
{
   	HWND hWnd=FindGameSvrWindow();
	if(IsWindow(hWnd))
	{
		PostMessage(hWnd,WM_RTG_BECOMEMEMBER,nUserType,nUserID);
	}
	
}

int	CBaseRoomData::GetStartedPlayerCountOnTable(LPTABLE pTable)
{
	int nCount = 0;
	LPPLAYER pPlayer = NULL;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
		if(pTable->nPlayerAry[i]>0)
		{
			if(!m_mapPlayer.Lookup( pTable->nPlayerAry[i], pPlayer)) 
				continue;
			if (PLAYER_STATUS_WAITING == pPlayer->nStatus)
				nCount++;
		}
	}
	
	return nCount;
}

BOOL CBaseRoomData::GetTableDepositSection(int nIndex, int& nTableBegin, int& nTableEnd, int& nMinDeposit,int& nMaxDeposit)
{
	TCHAR szSection[32];
	memset(szSection,0,sizeof(szSection));
	sprintf(szSection,"TableDeposit%d",roomdata.nRoomID);
	
	int nCount = GetPrivateProfileInt(szSection,_T("Count"),0, g_szIniFile);
	if(nCount<=0)
		return FALSE;
	if(nCount>MAX_TABLEDEPOSIT_COUNT)
		return FALSE;

	TCHAR szValue[64];
	memset(szValue,0,sizeof(szValue));
	GetPrivateProfileString(szSection,xyConvertIntToStr(nIndex),_T(""),szValue,sizeof(szValue) ,g_szIniFile);	
	if(szValue[0]==0)
		return FALSE;
	TCHAR *fields[8];
	memset(fields,0,sizeof(fields));
	TCHAR *p1, *p2;
	p1=szValue;
	int nFields=xyRetrieveFields ( p1, fields, 8, &p2 ) ;
	if(nFields<3)
		return FALSE;
	nTableBegin=atoi(fields[0]);
	nTableEnd=atoi(fields[1]);
	nMinDeposit=atoi(fields[2]);

	nMaxDeposit = 2147483646;
	if (nFields>3)
	{
		nMaxDeposit = atoi(fields[3]);
	}

	return TRUE;
}

BOOL CBaseRoomData::IsKickoffToomany(int nUserID, int nMemberLevel, int& nLimitCount)
{
	KICKOFF_RECORD kr;
	ZeroMemory(&kr, sizeof(kr));
	
	CString sKey;
	sKey.Format(_T("%d|%d"), nUserID, roomdata.nRoomID);
	{
		CAutoLock lock(&g_csKickoffRecord);
		if (!g_mapKickoffRecord.Lookup(sKey, kr))
			return FALSE;
	}

	if (nMemberLevel<0 || nMemberLevel>=VIP_MAX)
		return FALSE;

	//各等级限制的默认次数不同
	int anLimit[VIP_MAX] = {3,6,9,12,15};
	TCHAR szValue[128];	
	memset(szValue, 0, sizeof(szValue));
	TCHAR szKey[32];
	sprintf(szKey, _T("Limit_Room%d"), GetRoomID());
	GetPrivateProfileString(_T("VipKickoff"),szKey,_T(""),szValue, sizeof(szValue),g_szIniFile);
	if (lstrlen(szValue)>0)
	{
		TCHAR *p1 = szValue, *p2 = NULL;
		TCHAR *fields[VIP_MAX];
		memset(fields, 0, sizeof(fields));
		int nCount = xyRetrieveFields( p1, fields, VIP_MAX, &p2 ) ;
		for (int i=0; i<nCount&&i<VIP_MAX; i++)
		{
			if (fields[i])
				anLimit[i] = atoi(fields[i]);
		}
	}

	nLimitCount = anLimit[nMemberLevel];

	return (kr.nKickCount >= nLimitCount);
}

//是否有相互隔离的用户
BOOL CBaseRoomData::IsIsolateOnTable(int nUserID, TABLE* pTable, int nNowSeconds, KICKOFF_DETAIL& kickDetail, int& nRestSeconds)
{
	if(!IS_BIT_SET(roomdata.dwManages , ROOM_MNG_ISOLATE_KICKEACHOTHER))
		return FALSE;

	if(pTable->th.nPlayerCount==0)
		return FALSE;
	
	CAutoLock lock(&g_csPlayerKickList);	
	CKickoffDetailMap* pDetailMap = NULL;
	g_mapPlayerKickList.Lookup(nUserID, pDetailMap);
	if (NULL == pDetailMap)
		return FALSE;

	int nIsolaterSecond = GetIsolateSecond(roomdata.nRoomID);
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
		if(0 == pTable->nPlayerAry[i] || pTable->nPlayerAry[i] ==  nUserID)
			continue;

		//没有踢人记录
		if (!pDetailMap->Lookup(pTable->nPlayerAry[i], kickDetail))
			continue;

		//踢人记录已经超时
		if (nNowSeconds - kickDetail.nKickTime >= nIsolaterSecond)
			continue;

		nRestSeconds = nIsolaterSecond - (nNowSeconds - kickDetail.nKickTime);
		return TRUE;
	}

	return FALSE;
}

BOOL CBaseRoomData::IsForbidInvite()
{
    return IS_BIT_SET( roomdata.dwManages,ROOM_MNG_FORBIDINVITE);
	
}

void FreePlayerBlock(PLAYER* pPlayer)
{
	CAutoLock lock(&g_csPlayerList);
	
	if(!g_PoolOfPlayer.Find(pPlayer)){
		g_PoolOfPlayer.AddTail(pPlayer);
	}
}
BOOL CBaseRoomData::IsOpenPrivateTable(TABLE* pTable)
{
	if (!pTable)
		return FALSE;
	
	return pTable->bOpenPrivateTable == 0;
}
void CBaseRoomData::SetOpenPrivateTable(TABLE* pTable,BOOL bOpen)
{
	if (!pTable)
		return;
	pTable->bOpenPrivateTable = !bOpen;
}
void CBaseRoomData::CalculateAvgValue(TABLE* pTable,int & nAvgBounts,int & nAvgWins,int & nAvgScore,int & nAvgDeposit)
{
	int nCount = 0 , nTotalBounts = 0;
	nAvgBounts = nAvgWins = nAvgScore = nAvgDeposit = 0;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
		if(0 == pTable->nPlayerAry[i]  )
			continue;
		PLAYER* pPlayer=NULL;
		m_mapPlayer.Lookup(pTable->nPlayerAry[i],pPlayer);
		if(pPlayer==NULL)
			continue;
		nAvgBounts += pPlayer->nBout;
		nTotalBounts = pPlayer->nWin+pPlayer->nStandOff+pPlayer->nLoss;
		if(nTotalBounts>0)
			nAvgWins   += pPlayer->nWin/(nTotalBounts*1.f)*100;
		nAvgScore  += pPlayer->nScore;
		nAvgDeposit+= pPlayer->nDeposit;
		nCount++;
	}
	if (nCount == 0)
		return;
	nAvgBounts = nAvgBounts/nCount;
	nAvgWins   = nAvgWins/nCount;
	nAvgScore  = nAvgScore/nCount;
	nAvgDeposit= nAvgDeposit/nCount;
}
int  CBaseRoomData::GetPrivateTableCacheNum(int nPlayerID)
{
	CIntVecIntMap::iterator it = m_mapFindPrivateTableCache.find(nPlayerID);
	if (it != m_mapFindPrivateTableCache.end())
	{
		return it->second.size();
	}
	return 0;
}
void  CBaseRoomData::DeletePrivateTableCache(int nPlayerID)
{
	m_mapFindPrivateTableCache.erase(nPlayerID);
}
int  CBaseRoomData::FindPrivateTables(LPPLAYER pPlayer,ONE_TABLEROOM  Result[],int nNeedNum)
{
	int nTotalChair = GetChairCount();
	int nNowResultCount = 0;
	for (int nWantCount = nTotalChair-1;nWantCount>=1;nWantCount--)
	{
		nNowResultCount += CalculatePrivateTablesCount(pPlayer,Result,nWantCount,nNowResultCount,nNeedNum);
		if (nNowResultCount>=nNeedNum)
			break;
	}
	return nNowResultCount;
}
int CBaseRoomData::CalculatePrivateTablesCount(LPPLAYER pPlayer,ONE_TABLEROOM  Result[],int nWantCount,int nNowResultCount,int nNeedNum)
{
	if (nNowResultCount>=nNeedNum)
		return 0;

	int nTotalChair = GetChairCount();
	int nMaxTableCount = GetTableCount();
	LPTABLE lpTable = NULL;
	PLAYER* pHomePlayer=NULL;
	int nIndex = 0;
	int i=0;
	for (;i<nMaxTableCount;i++)
	{
		if (m_mapTable.Lookup(i, lpTable))
		{
			if (lpTable->th.nStatus == TABLE_STATUS_PLAYING)
				continue;
			if (lpTable->th.nPlayerCount!=nWantCount)
				continue;	
			if (!IsOpenPrivateTable(lpTable))
				continue;
			if(!m_mapPlayer.Lookup(lpTable->nHomeUserID,pHomePlayer))
				continue;
			if (pHomePlayer->nStatus != PLAYER_STATUS_WAITING){
				continue;
			}
			if (pHomePlayer->nTableNO!=lpTable->th.nTableNO){
				continue;
			}
			CIntVecIntMap::iterator it = m_mapFindPrivateTableCache.find(pPlayer->nUserID);
			if (it != m_mapFindPrivateTableCache.end())
			{
				if(std::find(it->second.begin(), it->second.end(), lpTable->th.nTableNO) != it->second.end()){
					continue;
				}
			}
			StrCpyN(Result[nNowResultCount+nIndex].szUserName,pHomePlayer->szUsername,sizeof(pHomePlayer->szUsername));
			Result[nNowResultCount+nIndex].nNickSex = pHomePlayer->nNickSex;
			Result[nNowResultCount+nIndex].nTableId = lpTable->th.nTableNO;
			Result[nNowResultCount+nIndex].nHomeUserID = pHomePlayer->nUserID;
			CalculateAvgValue(lpTable,Result[nNowResultCount+nIndex].nAvgBounts,Result[nNowResultCount+nIndex].nAvgWins,
				Result[nNowResultCount+nIndex].nAvgScore,Result[nNowResultCount+nIndex].nAvgDeposit);
			memcpy(Result[nNowResultCount+nIndex].nExistPlayerIDs,lpTable->nPlayerAry,sizeof(lpTable->nPlayerAry));

			//找过的记录存起来
			m_mapFindPrivateTableCache[pPlayer->nUserID].push_back(lpTable->th.nTableNO);
			nIndex++;
			if (nNowResultCount+nIndex>=nNeedNum)
				break;

		}
	}
// 	if (i == nMaxTableCount && nWantCount == 1)
// 	{
// 		DeletePrivateTableCache(pPlayer->nUserID);
// 	}
	return nIndex;
}
int  CBaseRoomData::PrivateTableDetail(TABLE* pTable,ONE_TRPLAYER  Result[])
{
	if(pTable->th.nPlayerCount==0)
		return 0;
	int nIndex = 0;
	for(int i = 0; i < MAX_CHAIR_COUNT; i++)
	{
		if(0 == pTable->nPlayerAry[i]  )
			continue;
		PLAYER* pPlayer=NULL;
		m_mapPlayer.Lookup(pTable->nPlayerAry[i],pPlayer);
		if(pPlayer==NULL)
			continue;
		Result[nIndex].nUserID = pPlayer->nUserID;
		Result[nIndex].nNickSex = pPlayer->nNickSex;
		StrCpyN(Result[nIndex].szUserName,pPlayer->szUsername,sizeof(pPlayer->szUsername));
		Result[nIndex].nWins= pPlayer->nWin;
		Result[nIndex].nTotalBount= pPlayer->nBout;
		Result[nIndex].nScore= pPlayer->nScore;
		Result[nIndex].nDeposit= pPlayer->nDeposit;
		nIndex++;
	}
	return nIndex;
}
BOOL CBaseRoomData::CheckSystemFindPlayersCD(int nTableNO)
{
	int nTime = 0;
	if(!m_mapPrivateTableInfo.Lookup(nTableNO,nTime))
		return TRUE;
	if (UwlGetCurrentSeconds()-nTime > g_nSysFindCD)
		return TRUE;
	return FALSE;
}
void CBaseRoomData::SetSystemFindPlayersTime(int nTableNO)
{
	m_mapPrivateTableInfo.SetAt(nTableNO,UwlGetCurrentSeconds());
}
//	在私人包间中，未上桌的玩家。优先级更高。
//	在随机房中，未进入分桌队列的玩家。优先级偏低。

int CBaseRoomData::BeginSystemFindPlayers(int  nNeedCount,LPBE_FOUND_BY_SYSTEM pMsg,int nFindMode ,const TABLEAVGVALUE& avgValue)
{
	//	条件一：银两/积分等级一致
	//	条件二：局数相差不到N局（N可配）
	//	条件三：胜率相差不到x（x可配）

	int userid = 0;
	int nCount = 0;
	int nTempMoney = 0;
	LPPLAYER lpPlayer = NULL;
	CAutoLock lock(&m_csRoomData);
	POSITION pos = m_mapPlayer.GetStartPosition();
	while (pos)	{
		m_mapPlayer.GetNextAssoc(pos, userid, lpPlayer);
		
		if(IS_BIT_SET( lpPlayer->nStatus,PLAYER_STATUS_PLAYING ))//在游戏
			continue;
		
// 		if(IS_BIT_SET( lpPlayer->nStatus,PLAYER_STATUS_WAITING ))//等待
// 			continue;

		if(!IS_BIT_SET( lpPlayer->nUserType,USER_TYPE_HANDPHONE )) //移动
			continue;

		if(IS_BIT_SET( lpPlayer->pe.dwClientFlags,FLAG_CLIENTINFO_INACTIVATED )) //切后台了
			continue;

		if (lpPlayer->nTableNO == pMsg->nTableNO && GetRoomID() == pMsg->nRoomID)
			continue;
		if (lpPlayer->nUserID == pMsg->nHomeUserID)
			continue;

		switch (nFindMode)
		{
		case eSFP_MODE_MONEY:
			if (IsSupportDeposit())
				nTempMoney = abs(lpPlayer->nDeposit - avgValue.nAvgDeposit);
			else
				nTempMoney = abs(lpPlayer->nScore - avgValue.nAvgScore);

			if (nTempMoney>g_nModeMoneyDiff)
				continue;
			break;
		case ESFP_MODE_BOUNT:
			if (abs(lpPlayer->nBout - avgValue.nAvgBounts)>g_nModeBountDiff)
			{
				continue;
			}
			break;
		case ESFP_MODE_WINS:
			int nTargetWin =  lpPlayer->nBout <=0 ? 50 : lpPlayer->nWin/(lpPlayer->nBout*1.0f)*100.f;
			int nSourceWin =  avgValue.nAvgBounts <=0 ? 50 : avgValue.nAvgWins/(avgValue.nAvgBounts*1.0f)*100.f;
			if (abs(nTargetWin - nSourceWin)>g_fModeWinsDiff)
			{
				continue;
			}
			break;
		}

		NotifyOnePlayer(this,lpPlayer->nUserID,MR_BE_FOUND_BY_SYSTEM, pMsg, sizeof(BE_FOUND_BY_SYSTEM));
		nCount++;
		if (nCount>=nNeedCount)
			break;
	}
	return nCount;
}
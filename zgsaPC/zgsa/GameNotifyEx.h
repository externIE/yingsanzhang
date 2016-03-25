// GameNotify.h: interface for the CGameNotify class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMENOTIFY_H__A27FBBB7_FF64_4448_989B_EDF0F101A2C1__INCLUDED_)
#define AFX_GAMENOTIFY_H__A27FBBB7_FF64_4448_989B_EDF0F101A2C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CGameNotifyEx : public CGameNotify
{
public:
	CGameNotifyEx();
	virtual ~CGameNotifyEx();

public:
	virtual void OnNotifyReceived( int nRequestID, VOID* pDataPtr,int nSize);
	virtual void NTF_GameAbort(GAME_ABORT* pGameAbort);//ÓÐÍæ¼ÒÀë¿ª
	virtual void NTF_PlayerAbort(GAME_ABORT * pPlayerAbort);
 	virtual void NTF_StartGame(START_GAME* pStartGame);
 
 	virtual void NTF_GameStart(GAME_START_INFO* pStartData);
 	virtual	void NTF_GameMsg(GAME_MSG* pGameMsg);
 			void NTF_GameWin(GAME_WIN_RESULT* pGameWinMJ);
			void NTF_GameTableInfo(GAME_TABLE_INFO* pGameTableInfo);
	virtual void NTF_PlayerOffline(LPPLAYER_OFFLINE lpOffline); 
	virtual void NTF_GameStartSolo(LPVOID pStartInfo);
	virtual void NTF_ChatFromTable(CHAT_FROM_TABLE* pChatFromTable);

    virtual	void NTF_WaitNewTable(); 

	virtual void NTF_StartCountdown(START_COUNTDOWN* pCountdown);
	virtual void NTF_SyncStartCountdown(SYNC_COUNTDOWN* pSyncCountdown);

	virtual void NTF_DepositNotEnough(DEPOSIT_NOT_ENOUGH*  pDepositNotEnough);

	virtual void SaveLastNotify(int nRequestID,VOID* pDataPtr,int nSize);
			void NTF_ChangeSeatedToLookOn(LOOK_ON_AND_SEATED_CHANGE_RESULT* ploascr);
			void NTF_ChangeLookOnToSeated(LOOK_ON_AND_SEATED_CHANGE_RESULT* ploascr);
			void NTF_GetGiveUpBtnConfig(int* nOpen);
};

#endif // !defined(AFX_GAMENOTIFY_H__A27FBBB7_FF64_4448_989B_EDF0F101A2C1__INCLUDED_)

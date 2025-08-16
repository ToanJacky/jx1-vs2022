//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KPlayerTrade.h
// Date:	2003.02.17
// Code:	边城浪子
// Desc:	Trade Class
//---------------------------------------------------------------------------

#ifndef KPLAYERTRADE_H
#define KPLAYERTRADE_H

#include	"KPlayerDef.h"



class KTrade
{
	friend class KPlayer;
public:
	int			m_nIsTrading;						// 是否正在交易 0 没有 1 正在交易
	int			m_nTradeDest;						// 服务器端记的是 player index 客户端记的是 npc id
	int			m_nTradeState;						// 是否已经点了ok 0 没有 1 点了
	int			m_nTradeLock;						// 是否锁定 0 没有 1 已经锁定
	BOOL		m_bTradeFolkGame;						// 是否锁定 0 没有 1 已经锁定

#ifdef _SERVER
	int			m_nApplyIdx;
	unsigned int m_uTradeOkTime;
	int			m_nTradeFolkId;
#endif

#ifndef _SERVER
	int			m_nBackEquipMoney;					// 交易开始时备份物品栏money
	int			m_nBackRepositoryMoney;				// 交易开始时备份储物箱money
	int			m_nTradeDestState;					// 客户端记录对方是否ok
	int			m_nTradeDestLock;					// 客户端记录对方是否锁定
	char		m_szDestName[32];					// 对方名字
#endif

public:
	KTrade();
	void		Release();
	BOOL		StartTrade(int nDest, BOOL bFolkGame = FALSE, int nEquipMoney = 0, int nRepositoryMoney = 0);
	void		Lock(BOOL bFlag);

#ifndef _SERVER
	void		SetTradeState(TRADE_STATE_SYNC	*pPress);
	static void		ReplyInvite(int nDestIdx, int nReply, BOOL bFolkGame = FALSE);
#endif

};
#endif

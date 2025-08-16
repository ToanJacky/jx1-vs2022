/*******************Editer	: duccom0123 EditTime:	2024/06/12 11:48:44*********************
//	剑侠应用界面系统的公共接口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-17
------------------------------------------------------------------------------------------
	窗口面板的基类，此窗口面板及其派生类，每个类都只能创建一个实例。
*****************************************************************************************/
#pragma once


void		UiPostQuitMsg();						//发送退出消息
bool		UiIsAlreadyQuit();						//是否已经选择退出程序
int			UiInit();								//界面系统初始化
int			UiStart();								//�_始界面控制流程
void		UiExit();								//界面系统退出
void		UiPaint(int nGameLoop);					//绘制界面
void		UiSetScreenSize(int nWidth, int nHeight);//设置屏幕范围大小
int			UiHeartBeat();							//界面活动
void		UiProcessInput(unsigned int uMsg, unsigned int uParam, int nParam);//处理输入
void		UiSetGraphicDevice(void* pDevice);		//设置绘图设备

void		UiStartGame();							//进入游戏运行时
void		UiOnGameServerConnected();				//与一个新的GameServer建立连接完毕
void		UiOnGameServerStartSyncEnd();			//与一个新的GameServer建立连接，启动同步完毕
void		UiResumeGame();							//游戏（断线后）继续
void		UiEndGame();							//离开游戏运行时
void		UiSetSwitchSceneStatus(int bSwitching);//切换加载地图状态


#include "Elem/ComWindow.h"
#include "Elem/WndButton.h"

//生命
class Player_Life : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Life)
	void UpdateData();
	void OnButtonClick();
	static bool m_bText;
};

//魔法
class Player_Mana : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Mana)
	void UpdateData();
	void OnButtonClick();
	static bool m_bText;
};

//体力
class Player_Stamina : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Stamina)
	void UpdateData();
	void OnButtonClick();
	static bool m_bText;
};

//经验
class Player_Exp : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Exp)
	void UpdateData();
	void OnButtonClick();
	int  GetToolTipInfo(char* szTip, int nMax);
	static bool m_bText;
};

//等级
class Player_Level : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_Level)
	void UpdateData();
};

class Player_SectSort : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_SectSort)
	void OnButtonClick();
	void UpdateData();
};

//世界排名
class Player_WorldSort : public KWndImageTextButton
{
public:
	DECLARE_COMCLASS(Player_WorldSort)
	void OnButtonClick();
	void UpdateData();
};

//状态
class Player_Status : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Status)
	void OnButtonClick();
	const char*	GetShortKey();
};

//物品
class Player_Items : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Items)
	void OnButtonClick();
	const char*	GetShortKey();
};

class Player_ItemEx : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_ItemEx)
	void OnButtonClick();
	const char*	GetShortKey();
};

//武功
class Player_Skills : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Skills)
	void OnButtonClick();
	const char*	GetShortKey();
};

//队伍
class Player_Team : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Team)
	void OnButtonClick();
	const char*	GetShortKey();
};

//门派
class Player_Faction : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Faction)
	void OnButtonClick();
	const char*	GetShortKey();
};


//打坐
class Player_Sit : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Sit)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};

//跑步
class Player_Run : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Run)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};

//上马
class Player_Horse : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Horse)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};

//交易
class Player_Exchange : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Exchange)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};


//PK开关
class Player_PK : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_PK)
	void OnButtonClick();
	void UpdateData();
	const char*	GetShortKey();
};

class Player_ChatRoom : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_ChatRoom)
	void OnButtonClick();
	const char*	GetShortKey();
};

class Player_Recorder : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Recorder)
	void OnButtonClick();
	const char*	GetShortKey();
};


class Player_Auto : public KWndButton
{
public:
	DECLARE_COMCLASS(Player_Auto)
	void OnButtonClick();
	const char*	GetShortKey();
};

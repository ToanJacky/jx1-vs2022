/*******************Editer	: duccom0123 EditTime:	2024/06/12 11:48:44*********************
//	剑侠界面系统
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-17
------------------------------------------------------------------------------------------
		剑侠界面系统以剑侠界面窗口元素体系为基础，为剑侠的实际应用界面的实现。每个界面窗口
    都是遵从iKUiWindow接口。界面系统可以支持让窗口有不同的外表(skin)，在系统中称为方案(Scheme)
	整个界面系统的实现程序文件都以Ui作为文件名前缀。
	    界面系统将针对每个（玩家）名称id保存一份窗口自定义应用配置文件。

        界面系统的实现类为KUiBase。界面系统的应用：
		Init --> 初始化界面系统与各界面窗口，执行此操作后，界面窗口可以执行窗口操作功能了。
		LoadConfig --> 载入窗口的可定制的配置信息，如窗口的位置。
		LoadScheme --> 载入窗口系统的界面方案，在执行这个操作后，界面窗口才知道用什么图来
						装扮自己。

	注：
		现在全部窗口界面方案是一次一起载入，但是每个界面中用到的图都在第一次被显示的时候才
	载入，需要时可以进一步改造优化为各个窗口的界面方案在第一次显示窗口时候被载入。
*****************************************************************************************/
#pragma once

enum UISYS_STATUS
{
	UIS_S_IDLE,
	UIS_S_TRADE_PLAYER,		//（与Player）交易
	UIS_S_TRADE_NPC,		//（与npc）交易
	UIS_S_TRADE_SALE,		//（与npc）交易 卖
	UIS_S_TRADE_BUY,		//（与npc）交易 买
	UIS_S_TRADE_REPAIR,		//（与npc）交易 修理
	UIS_S_TRADE_SETPRICE,
	UIS_S_LOCK_ITEM,
	UIS_S_UNLOCK_ITEM,
	UIS_S_INPUT,
};

enum UISYS_OPERATION
{
	UIS_O_MOVE_ITEM,
	UIS_O_TRADE_ITEM,
	UIS_O_USE_ITEM,
	UIS_O_LOCK_ITEM,
	UIS_O_UNLOCK_ITEM
};

#define APP_START	"Start()"
#define APP_EXIT	"Exit()"
#define APP_STARTGAME	"StartGame(\"%s\")"
#define APP_EXITGAME	"ExitGame()"
#define APP_SAY		"Say(\"%s\", \"%s\")"
#define APP_CHAT	"Chat(\"%s\", \"%s\")"
#define APP_CREATEUNIT	"CreateUnit(\"%s\")"
#define APP_DELETEUNIT	"DeleteUnit(\"%s\")"
#define APP_CREATEUNITGROUP	"CreateUnitGroup(\"%s\", \"%s\")"
#define APP_RENAMEUNITROUP	"RenameUnitGroup(\"%s\", \"%s\", \"%s\")"
#define APP_DELETEUNITROUP	"DeleteUnitGroup(\"%s\", \"%s\")"
#define APP_MOVEUNITROUP	"MoveUnitGroup(\"%s\", \"%s\", \"%s\")"
#define APP_CREATEUNITMEMBER	"CreateUnitMember(\"%s\", \"%s\", \"%s\")"
#define APP_DELETEUNITMEMBER	"DeleteUnitMember(\"%s\", \"%s\")"
#define APP_MOVEUNITMEMBER	"MoveUnitMember(\"%s\", \"%s\", \"%s\")"
#define APP_SETUNITMEMBERSTATUS	"SetUnitMemberStatus(\"%s\", \"%s\", \"%s\")"

class KIniFile;

extern const char*	$Main;

#pragma warning(disable:4786)
#include "map"
#include "string"

class KUiBase
{
public:
	KUiBase();
	~KUiBase();
	int		Init();											//初始化
	void	Exit();											//退出

	void	SetUserAccount(char* pszId, char* pszRole);		//设置用户账号
	int		GetUserPrivateDataFolder(char* pszFilePath, int nSize);	//获得当前玩家私有数据的存储目录
	const char*	GetUserTempDataFolder();					//获得玩家数据的临时存盘目录位置
	void	CleanTempDataFolder();							//清除玩家数据的临时存盘目录内的全部文件
	void	CleanPrivateDataFolder();						//清除当前玩家私有数据的存储目录
	void	DeletePrivateDataFile(const char* pszFileName);	//清除当前玩家私有数据的某个文件

	int		LoadScheme(const char* pSchemeName);			//按照某种方案载入界面(Skin)
	int		SchemeCount();									//获得界面方案的数目
	int		GetScheme(int nIndex, char* pName, char* pPath);//获得某个界面方案的名称与路径
	int		GetCurSchemePath(char* pBuffer, int nSize);		//获得当前界面方案的路径
	
	//操作配置、设定文件
	KIniFile*	GetCommSettingFile();						//打开保存界面公共设置的文件
	void		CloseCommSettingFile(bool bSave);			//关闭保存界面公共设置的文件
	KIniFile*	GetPrivateSettingFile();					//打开打开当前账号的设置文件
	void		ClosePrivateSettingFile(bool bSave);		//关闭打开当前账号的设置文件
	KIniFile*	GetCommConfigFile();
	void		CloseCommConfigFile();

	//设置获取界面操作状态
	UISYS_STATUS GetStatus() { return m_Status; }
	void		 SetStatus(UISYS_STATUS eStatus);
	void		 RestoreStatus();
	void		 SetCurrentCursor();						//根据当前状态设置鼠标指针图形
	int			 IsOperationEnable(UISYS_OPERATION uOper);

	void		LoadPrivateConfig();						//载入配置纪录，如定制的窗口位置
	int			SavePrivateConfig();						//存储配置纪录

	void		RegisterEvent(const char* szName, HANDLE hWnd);
	void		UnregisterEvent(const char* szName, HANDLE hWnd);
	int			NotifyEvent(const char* szEvent);		//0为已经处理, 不要再处理了, 非0为未处理或错误
	int			NotifyOneEvent(const char* szName, const char* szEvent);		//0为已经处理, 不要再处理了, 非0为未处理或错误

private:
	void	LoadSchemeForEachWnd();
	int		GetSchemePath(const char* pScheme);				//获得当前指定界面方案的目录名
	void	ClearFolder(const char* pszFolder, bool bDeleteFolder);	//清除当前玩家私有数据的存储目录

public:
	char		m_UserAccountId[32];		//当前玩家账号在界面系统中的id
private:
	UISYS_STATUS	m_Status;				//界面操作状态
	UISYS_STATUS	m_PrevStatus;				//界面操作状态

	char		m_CurScheme[32];			//当前界面方案的名称
	char		m_CurSchemePath[40];		//当前界面方案的路径

	KIniFile*	m_pUiCommSettingFile;		//界面公共设置的文件
	KIniFile*	m_pUiPrivateSettingFile;	//当前账号的设置文件
	KIniFile*	m_pUiCommConfigFile;

	typedef std::map<std::string, HANDLE> EVENTWNDLIST;
	EVENTWNDLIST m_EventWndList;
};

extern KUiBase	g_UiBase;
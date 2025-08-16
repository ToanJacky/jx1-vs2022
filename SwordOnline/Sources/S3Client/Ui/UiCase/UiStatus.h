/*******************Editer	: duccom0123 EditTime:	2024/06/12 11:48:45*********************
//	界面--状态界面
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-2
------------------------------------------------------------------------------------------
	共有11个装备摆放位置，武器2×4，衣服2×3，头部2×2，腰带2×1，
	手镯1×1，鞋子2×2，戒指1×1，腰坠1×2，项链1×1，马2×3。
*****************************************************************************************/
#pragma once

#include "../Elem/WndImage.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../../../Core/Src/GameDatadef.h"
#include "../Elem/WndShowAnimate.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndPureTextBtn.h"	

enum UI_PLAYER_ATTRIBUTE;
struct KUiPlayerRuntimeInfo;
struct KUiObjAtRegion;
struct KUiPlayerAttribute;
struct KPopupMenuData;
#define _ITEM_COUNT 17

class KUiStatus : public KWndShowAnimate
{
public:
	//----界面面板统一的接口函数----
	static KUiStatus*	OpenWindow(bool bShow = true);					//打开窗口，返回唯一的一个类对象实例
	static KUiStatus*	GetIfVisible();					//如果窗口正被显示，则返回实例指针
	static void			CloseWindow(bool bDestroy);		//关闭窗口，同时可以选则是否删除对象实例
//	static void			LoadConfig(KIniFile* pIni);		//载入自定义配置
	static void			LoadScheme(const char* pScheme);//载入界面方案
	
	void	UpdateBaseData();							//更新基本数据（人名等不易变数据）
	void	UpdateData();
	void	Breathe();
	void	UpdateRuntimeInfo(KUiPlayerRuntimeInfo* pInfo);
	void	UpdateAllEquips();
	void	UpdateRuntimeAttribute(KUiPlayerAttribute* pInfo);
	void	UpdateEquip(KUiObjAtRegion* pEquip, int bAdd);	//装备变化更新
	void	SwitchExpand(BOOL bShow);

private:
	KUiStatus() {}
	~KUiStatus() {}
	void	Initialize();								//初始化	
	void	UseRemainPoint(int ntype, int nPoint);	//升级某项属性
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	//窗口函数
	void	LoadScheme(class KIniFile* pIni);			//载入界面方案
	void	OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);//响应界面操作引起装备的改变

private:
	static KUiStatus*	m_pSelf;

private:
	KWndText32	m_Name;
	KWndText32	m_Title;
	
	KWndText32	m_Life;
	KWndText32	m_Mana;
	KWndText32	m_Experience;
	KWndText32	m_Stamina;

	int			m_nRemainPoint;
	KWndText32	m_RemainPoint;

	KWndText32	m_Strength, m_Dexterity, m_Vitality, m_Energy;
	KWndButton	m_AddStrength, m_AddDexterity, m_AddVitality, m_AddEnergy;

	KWndText32	m_LeftDamage, m_RightDamage, m_Attack, m_Defence, m_MoveSpeed, m_AttackSpeed;
	KWndText32	m_PhyDef, m_CoolDef, m_LightDef, m_FireDef, m_PoisonDef;
	KWndText32	m_PhyDefPlus, m_CoolDefPlus, m_LightDefPlus, m_FireDefPlus, m_PoisonDefPlus;
	KWndText32	m_Level, m_StatusDesc;
	KWndText32	m_PKValue;
	KWndText32	m_Repute;
	KWndText32	m_FuYuan;
	KWndText32	m_TransLife;
	KWndText32	m_WorldRank;
	KWndImage	m_Avt;

	KWndButton	m_OpenItemPad;
	KWndButton	m_Close;

	KWndButton	m_EquipExpandBtn;
	KWndImage	m_EquipExpandImg;
	KWndButton	m_MaskFeature;
	KWndButton	m_UnlockBtn;
	KWndButton	m_BtnBind;
	KWndButton	m_BtnUnBind;	
//	KWndButton  m_Avatar;
//	KWndButton	m_ClickHere;
//	KWndText80	m_ClickHere1;
	KWndText80	m_chooseavatar;
	KWndButton  m_Avatar;
//	int 		m_nNumicon;
	int			nNumIcon;
	//装备物品
	KWndObjectBox	m_EquipBox[_ITEM_COUNT];
};

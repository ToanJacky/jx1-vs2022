/* 
 * File:     UiWorldMap.h
 * Desc:     世界地图
 * Author:   flying
 * Creation: 2003/7/22
 */
//-----------------------------------------------------------------------------
#pragma once

#include "../Elem/WndImage.h"
#include "../Elem/UiImage.h"
#include "../Elem/WndShadow.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndText.h"

class KUiWorldmap : protected KWndImage
{
public:
	static KUiWorldmap* OpenWindow();		//打开窗口，返回唯一的一个类对象实例
	static void			CloseWindow();		//关闭窗口
	static KUiWorldmap*	GetIfVisible();
	void				LoadScheme(const char* pScheme);	//载入界面方案

private:
	KUiWorldmap() {}
	~KUiWorldmap() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	PaintWindow();				//活动函数
	void	UpdateData();
private:
	static KUiWorldmap* m_pSelf;
	KWndImage			m_Sign;
	KWndImage			m_City;
	KWndImage			m_Capital;
	KWndImage			m_Cave;
	KWndImage			m_Battlefield;
	KWndImage			m_Field;
	KWndImage			m_Others;
	KWndImage			m_Country;
	KWndImage			m_Tong;
};
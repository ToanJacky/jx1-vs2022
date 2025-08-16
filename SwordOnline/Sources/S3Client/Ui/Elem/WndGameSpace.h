/*******************Editer	: duccom0123 EditTime:	2024/06/12 11:48:45*********************
//	�[�虼翱诘慕涌�
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-18
------------------------------------------------------------------------------------------
	根据当前游戏设定，暂不处理游世界非满窗口绘制的情况。
*****************************************************************************************/
#pragma once

struct iKWndGameSpaceX
{
	//输入处理消息以及一些特定窗口消息的响应
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) = 0;
	//绘制游戏世界
	virtual void	Paint() = 0;
};
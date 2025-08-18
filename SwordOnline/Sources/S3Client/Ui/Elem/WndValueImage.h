/*******************Editer	: duccom0123 EditTime:	2024/06/12 11:48:45*********************
//	���洰����ϵ�ṹ--ֵ���ƻ�֡����ʾͼ�εĴ���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-10
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "WndImage.h"

class KWndValueImage : public KWndImage
{
public:
	virtual int		Init(KIniFile* pIniFile, const char* pSection);//��ʼ��
	void			SetValue(int nCurrentValue, int nFullValue);
	KWndValueImage();
	void			Clone(KWndValueImage* pCopy);
private:
	int		m_nFrameCount;
	int		m_nCurrentValue;
	int		m_nFullValue;
};
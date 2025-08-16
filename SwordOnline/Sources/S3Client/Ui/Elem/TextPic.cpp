//////////////////////////////////////////////////////////////////////////////////////
//	文件名			:	TextPic.cpp
//	创建人			:	王西贝
//	创建时间		:	2003-6-22 23:13:12
//	文件说明		:	文字嵌入式图片
//////////////////////////////////////////////////////////////////////////////////////

#include "KWin32.h"
#include "TextPic.h"
#include "KIniFile.h"
#include "CrtDbg.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"

KInlinePicSink::KInlinePicSink()
{
	m_pRepShell = NULL;
}
	
BOOL KInlinePicSink::Init(iRepresentShell* pShell)
{
	m_pRepShell = pShell;
	if (!m_pRepShell)
		return FALSE;
	KIniFile ini;
	if(!ini.Load("\\Ui\\表情大全.ini"))
		return FALSE;
	char szBuffer[MAX_PATH];
	int i,n;
	KUiImageRef CurrentImg;
	KImageParam	Param;

	ini.GetInteger("List", "Total", 0, &n);
	if(n > MAX_SYSTEM_INLINE_PICTURES)
		n = MAX_SYSTEM_INLINE_PICTURES;

	for (i = 0; i < n; i++)
	{
		sprintf(szBuffer, "\\spr\\Ui3\\表情\\%02d.spr", i+1);
		IR_InitUiImageRef(CurrentImg);
		CurrentImg.nFlipTime = 0;
		CurrentImg.nInterval = 0;
		CurrentImg.nNumFrames = 0;
		strncpy(CurrentImg.szImage, szBuffer, sizeof(CurrentImg.szImage));
		CurrentImg.szImage[sizeof(CurrentImg.szImage) - 1] = 0;
		CurrentImg.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
		CurrentImg.nType = ISI_T_SPR;
		memset(&Param, 0, sizeof(Param));
		pShell->GetImageParam(CurrentImg.szImage, &Param, CurrentImg.nType);
		m_Images.push_back(CurrentImg);
		m_ImageParams.push_back(Param);
	}

	if (FAILED(m_pRepShell->AdviseRepresent(this)))
		return FALSE;
	if (FAILED(AdviseEngine(this)))
		return FALSE;
	return TRUE;
}

BOOL KInlinePicSink::UnInit()
{
	if (!m_pRepShell)
		return FALSE;
	if (FAILED(m_pRepShell->UnAdviseRepresent(this)))
		return FALSE;
	if (FAILED(UnAdviseEngine(this)))
		return FALSE;
	return TRUE;
}

long KInlinePicSink::GetPicSize(unsigned short wIndex, int& cx, int& cy)
{
	_ASSERT(m_Images.size() == m_ImageParams.size() &&
			m_CustomImages.size() == m_CustomImageParams.size());
	if (!m_pRepShell ||
		wIndex < MAX_SYSTEM_INLINE_PICTURES && wIndex >= m_Images.size() ||
		wIndex >= MAX_SYSTEM_INLINE_PICTURES &&
			(wIndex - MAX_SYSTEM_INLINE_PICTURES >= m_CustomImages.size() ||
			!m_CustomImages[wIndex - MAX_SYSTEM_INLINE_PICTURES].szImage[0]))
		return E_FAIL;

	if (wIndex < MAX_SYSTEM_INLINE_PICTURES)
	{
		cx = m_ImageParams[wIndex].nWidth;
		cy = m_ImageParams[wIndex].nHeight;
	}
	else
	{
		cx = m_CustomImageParams[wIndex - MAX_SYSTEM_INLINE_PICTURES].nWidth;
		cy = m_CustomImageParams[wIndex - MAX_SYSTEM_INLINE_PICTURES].nHeight;
	}

	return S_OK;
}

long KInlinePicSink::DrawPic(unsigned short wIndex, int x, int y)
{
	_ASSERT(m_Images.size() == m_ImageParams.size() &&
			m_CustomImages.size() == m_CustomImageParams.size());
	if (!m_pRepShell ||
		wIndex < MAX_SYSTEM_INLINE_PICTURES && wIndex >= m_Images.size() ||
		wIndex >= MAX_SYSTEM_INLINE_PICTURES &&
			(wIndex - MAX_SYSTEM_INLINE_PICTURES >= m_CustomImages.size() ||
			!m_CustomImages[wIndex - MAX_SYSTEM_INLINE_PICTURES].szImage[0]))
		return E_FAIL;

	KUiImageRef* pPic = NULL;
	if (wIndex < MAX_SYSTEM_INLINE_PICTURES)
		pPic = &(m_Images[wIndex]);
	else
		pPic = &(m_CustomImages[wIndex - MAX_SYSTEM_INLINE_PICTURES]);
	pPic->oPosition.nX = x;
	pPic->oPosition.nY = y;

	m_pRepShell->DrawPrimitives(1, pPic, RU_T_IMAGE, true);
	IR_NextFrame(*pPic);
	return S_OK;
}


//动态加载图片,获取一个WORD,即图片的索引
long KInlinePicSink::AddCustomInlinePic(unsigned short& rwIndex, const char* szSprPathName)
{
	KUiImageRef CurrentImg;
	KImageParam	Param;
	int i = 0, nSize = 0;

	rwIndex = -1;
	if (!szSprPathName || !szSprPathName[0])
		return E_FAIL;

	IR_InitUiImageRef(CurrentImg);
	CurrentImg.nFlipTime = 0;
	CurrentImg.nInterval = 0;
	CurrentImg.nNumFrames = 0;
	strncpy(CurrentImg.szImage, szSprPathName, sizeof(CurrentImg.szImage));
	CurrentImg.szImage[sizeof(CurrentImg.szImage) - 1] = 0;
	CurrentImg.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
	CurrentImg.nType = ISI_T_SPR;
	memset(&Param, 0, sizeof(Param));
	if (!m_pRepShell->GetImageParam(CurrentImg.szImage, &Param, CurrentImg.nType))
		return E_FAIL;
	for (i = 0, nSize = (int)m_CustomImages.size(); i < nSize; i++)
	{
		if (0 == m_CustomImages[i].szImage[0])
		{
			m_CustomImages[i] = CurrentImg;
			m_CustomImageParams[i] = Param;
			break;
		}
	}
	if (i >= nSize)
	{
		m_CustomImages.push_back(CurrentImg);
		m_CustomImageParams.push_back(Param);
	}
	rwIndex = i + MAX_SYSTEM_INLINE_PICTURES;
	//return S_OK;
	return rwIndex;
}

//动态卸载图片
long KInlinePicSink::RemoveCustomInlinePic(unsigned short wIndex)
{
	if (wIndex < MAX_SYSTEM_INLINE_PICTURES ||
		wIndex >= MAX_SYSTEM_INLINE_PICTURES &&
			(wIndex - MAX_SYSTEM_INLINE_PICTURES >= m_CustomImages.size() ||
			!m_CustomImages[wIndex - MAX_SYSTEM_INLINE_PICTURES].szImage[0]))
		return E_FAIL;
	m_CustomImages[wIndex - MAX_SYSTEM_INLINE_PICTURES].szImage[0] = 0;
	return S_OK;
}



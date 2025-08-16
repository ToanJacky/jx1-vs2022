/*******************Editer	: duccom0123 EditTime:	2024/06/12 11:48:42*********************
//	界面--聊天快捷输入短语
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-3-23
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "Uichatphrase.h"
#include "UiBase.h"
#include "elem/wndwindow.h"
#include "ShortcutKey.h"
#include "../../Engine/Src/Text.h"
#include "../../Engine/Src/KDebug.h"

#include <crtdbg.h>

#define EMOTE_INI_FILE "\\Ui\\Emote.ini"

//构造在调用ShortcutKey::LuaRegisterFunctionAlias注册SayEmote函数别名时所需参数的定义
#define SAY_EMOTE_FUNCTION "SayEmote"
#define SAY_EMOTE_PARAM_DEST_PLAYER "GetRecentPlayerName()"
#define SAY_EMOTE_PARAM_DEST_CHANNEL "GetCurrentChannelName()"
//////////////////////////////////////////////////////////////////////////////

KUiChatPhrase	g_UiChatPhrase;

KUiChatPhrase::KUiChatPhrase()
{
	m_pEmotePhrase = NULL;
	m_pMenuEmote   = NULL;
	m_nEmotePhraseCount = 0;
	m_nMenuEmoteCount   = 0;
	ClearAllPhrase();
}

KUiChatPhrase::~KUiChatPhrase()
{
	if(m_pEmotePhrase)
	{
		delete(m_pEmotePhrase);
		m_pEmotePhrase = NULL;
	}
	if(m_pMenuEmote)
	{
		delete(m_pMenuEmote);
		m_pMenuEmote = NULL;
	}
}

void KUiChatPhrase::ClearAllPhrase()
{
	memset(&m_PhraseList, 0, sizeof(m_PhraseList));
}

int KUiChatPhrase::GetPhrase(int nIndex, char* pszPhrase)
{
	if (nIndex >= 0 && nIndex < MAX_PHRASE_COUNT && pszPhrase)
	{
		pszPhrase[m_PhraseList[nIndex].nStringLen] = 0;
		memcpy(pszPhrase, m_PhraseList[nIndex].szString,
			m_PhraseList[nIndex].nStringLen);
		return m_PhraseList[nIndex].nStringLen;
	}
	return 0;
}

int KUiChatPhrase::SetPhrase(int nIndex, char* pszPhrase, int nLen)
{
	if (nIndex >= 0 && nIndex < MAX_PHRASE_COUNT)
	{
		m_PhraseList[nIndex].szString[0] = 0;
		m_PhraseList[nIndex].nStringLen = 0;
		if (pszPhrase && nLen && nLen <= 64)
		{
			memcpy(m_PhraseList[nIndex].szString, pszPhrase, nLen);
			m_PhraseList[nIndex].nStringLen = nLen;
			m_PhraseList[nIndex].szString[nLen] = 0;
		}
		return nLen;
	}
	return 0;
}

int	KUiChatPhrase::GetPhraseCount()
{
	return MAX_PHRASE_COUNT;
}

int	KUiChatPhrase::SavePrivateSetting(KIniFile* pFile, LPCSTR lpSection, int nStart)
{
	int i = nStart;
	char szKey[10];
	char szLine[256];
	for (; i - nStart < MAX_PHRASE_COUNT; i++)
	{
		sprintf(szKey, "%d", i);
		sprintf(szLine, "SetPhrase(%d, \"%s\")", i - nStart, m_PhraseList[i - nStart].szString);
		pFile->WriteString(lpSection, szKey, szLine);
	}
	return i;
}


/************************************************************************
* 载入所有的聊天表情动作文件
*************************************************************************/
int KUiChatPhrase::LoadEntireEmote()
{
	KIniFile Ini;

	if(Ini.Load(EMOTE_INI_FILE))
	{
		char szBuf[32];
		Ini.GetString ("Main", "MyNameColor", "0,0,0", szBuf, sizeof(szBuf));
		m_uMyNameColor = GetColor(szBuf);
		Ini.GetString ("Main", "TargetColor", "0,0,0", szBuf, sizeof(szBuf));
		m_uTargetColor = GetColor(szBuf);
	    LoadEmotePhrase(&Ini);
	    LoadMenuEmote(&Ini);
	}
	return 1;
}


/************************************************************************
* 功能：设置聊天动作，有给出index的就改index的，没有就根据Cmd寻找要替换的
*************************************************************************/
int KUiChatPhrase::SetEmote(char *szCmd, char *szName, char *szStringTarget, char *szStringSelf, int nIndex)
{
	if(szCmd && szName && szStringTarget && szStringSelf && szCmd[0])
	{
    	int nIdx;
    	if(nIndex == -1)
    	{
		    if((nIdx = FindEmote(EMOTE_T_MENU, szCmd)) == -1)
		    {
    			if((nIdx = FindEmote(EMOTE_T_PHRASE, szCmd)) == -1)
				    return -1;
		    }
	    }
	    else
	    {
    		if(nIndex < 0 || nIndex > (m_nMenuEmoteCount + m_nEmotePhraseCount))
			    return -1;
		    nIdx = nIndex;
	    }

	    EMOTE_PHRASE *pData;
	    if(nIdx >= m_nMenuEmoteCount)
	    {
    		nIdx -= m_nMenuEmoteCount;
		    pData = m_pEmotePhrase;
	    }
	    else
	    {
    		pData = m_pMenuEmote;
    	}

	    memcpy(pData[nIdx].szCmd, szCmd, strlen(szCmd) + 1);
	    memcpy(pData[nIdx].szName, szName, strlen(szName) + 1);
	    memcpy(pData[nIdx].szString, szStringTarget, strlen(szStringTarget) + 1);
	    memcpy(pData[nIdx].szStringMe, szStringSelf, strlen(szStringSelf) + 1);
	    pData[nIdx].nStringLen = strlen(szStringTarget);
	    pData[nIdx].nStringMeLen = strlen(szStringSelf);
	    pData[nIdx].nNameLen = strlen(szName);

	    return 1;
	}
	return -1;
}


/************************************************************************
* 功能：载入一般聊天表情动作文件
*************************************************************************/
int KUiChatPhrase::LoadEmotePhrase(KIniFile *pIni)
{
	int  nCount;

	pIni->GetInteger("Phrase", "Count", 0, &nCount);
	if(m_pEmotePhrase)
	{
        delete(m_pEmotePhrase);
		m_pEmotePhrase = NULL;
	}
	m_pEmotePhrase = new EMOTE_PHRASE[nCount];
	if(m_pEmotePhrase)
	{
		m_nEmotePhraseCount = nCount;
        HandleLoad(pIni, m_pEmotePhrase, nCount, "Phrase");
	}
	return 1;
}


/************************************************************************
* 功能：载入ALT右键菜单的聊天表情动作文件
*************************************************************************/
int KUiChatPhrase::LoadMenuEmote(KIniFile *pIni)
{
	int  nCount;

	pIni->GetInteger("Menu", "Count", 0, &nCount);
	if(m_pMenuEmote)
	{
        delete(m_pMenuEmote);
		m_pMenuEmote = NULL;
	}
	m_pMenuEmote = new EMOTE_PHRASE[nCount];
	if(m_pMenuEmote)
	{
		m_nMenuEmoteCount = nCount;
	    HandleLoad(pIni, m_pMenuEmote, nCount, "Menu");
	}
	return 1;
}


/************************************************************************
* 功能：载入数据的文件
*************************************************************************/
int KUiChatPhrase::HandleLoad(KIniFile *pIni, EMOTE_PHRASE *pData, int nCount, const char *szSection)
{
	int i;
	char szKey[32];

	if(pData)
	{
	    for(i = 0;i < nCount;i++)
	    {
			sprintf(szKey, "%d_Name", i);
		    pIni->GetString(szSection, szKey, "", pData[i].szName, 
				             sizeof(pData[i].szName));

			pData[i].nNameLen = strlen(pData[i].szName);

    		sprintf(szKey, "%d_String", i);
		    pIni->GetString(szSection, szKey, "", pData[i].szString, 
				             sizeof(pData[i].szString));

			pData[i].nStringLen = TEncodeText(pData[i].szString, 
	                         strlen(pData[i].szString));

			sprintf(szKey, "%d_StringOnlyMe", i);
		    pIni->GetString(szSection, szKey, "", pData[i].szStringMe, 
				             sizeof(pData[i].szStringMe));

			pData[i].nStringMeLen = TEncodeText(pData[i].szStringMe, 
	                         strlen(pData[i].szStringMe));

			sprintf(szKey, "%d_Command", i);
			pIni->GetString(szSection, szKey, "", pData[i].szCmd, 
				             sizeof(pData[i].szCmd));
	    }
	}
	return 1;
}


/************************************************************************
* 功能：把读取到的Emote信息构造进去Shortcut系统中的函数别名表
*************************************************************************/
int KUiChatPhrase::ConstructFunctionAlias()
{
	int i;
	char Buf[8];
	PARAMLIST PmList;

	for(i = 0;i<m_nMenuEmoteCount;i++)
	{
		PmList.clear();
		PmList.push_back(SAY_EMOTE_PARAM_DEST_PLAYER);
		PmList.push_back(SAY_EMOTE_PARAM_DEST_CHANNEL);
		PmList.push_back(itoa(i, Buf, 10));

		KShortcutKeyCentre::RegisterFunctionAlias(m_pMenuEmote[i].szCmd, "SayEmote", 3, PmList);
	}
	for(i = 0;i<m_nEmotePhraseCount;i++)
	{
		PmList.clear();
		PmList.push_back(SAY_EMOTE_PARAM_DEST_PLAYER);
		PmList.push_back(SAY_EMOTE_PARAM_DEST_CHANNEL);
		PmList.push_back(itoa(i + m_nMenuEmoteCount, Buf, 10));

		KShortcutKeyCentre::RegisterFunctionAlias(m_pEmotePhrase[i].szCmd, "SayEmote", 3, PmList);
	}

	return 1;
}


/************************************************************************
* 功能：取得某种表情动作的个数
*************************************************************************/
int KUiChatPhrase::GetEmoteCount(EMOTE_TYPE eType)
{
	if(eType == EMOTE_T_PHRASE)
	{
		return m_nEmotePhraseCount;
	}
	else
	{
		return m_nMenuEmoteCount;
	}
}


/************************************************************************
* 功能：根据命令获取一个动作描述(pBuff)，返回描述长度，
*       这里是包括了MENU和PHRASE的
*************************************************************************/
int KUiChatPhrase::GetEmote(const char *szCmd, char *pBuff, int nBuffLen, int nType)
{
	int nIndex;

	if((nIndex = GetMenuEmote(szCmd, pBuff, nBuffLen, nType)) > 0)
		return nIndex;

	nIndex = FindEmote(EMOTE_T_PHRASE, szCmd);

	return GetEmote(nIndex, pBuff, nBuffLen, nType);
}
//上面的多态
/************************************************************************
* 功能：根据索引获取一个动作描述(pBuff)，返回描述长度，
*       这里是包括了MENU和PHRASE的
*************************************************************************/
int KUiChatPhrase::GetEmote(int nIndex, char *pBuff, int nBuffLen, int nType)
{
	if(nIndex >= 0 && nIndex < (m_nEmotePhraseCount + m_nMenuEmoteCount) &&
		m_pEmotePhrase && nBuffLen >= m_pEmotePhrase[nIndex].nStringLen)
	{
		if(nIndex < m_nMenuEmoteCount)
		{
			return GetMenuEmote(nIndex, pBuff, nBuffLen, nType);
		}
		else
		{
			nIndex -= m_nMenuEmoteCount;
    		if(nType == 0)
		    {
    			memcpy(pBuff, m_pEmotePhrase[nIndex].szString, m_pEmotePhrase[nIndex].nStringLen);
			    return m_pEmotePhrase[nIndex].nStringLen;
		    }
		    else
		    {
    			memcpy(pBuff, m_pEmotePhrase[nIndex].szStringMe, m_pEmotePhrase[nIndex].nStringMeLen);
			    return m_pEmotePhrase[nIndex].nStringMeLen;
		    }
		}
	}
	else
	{
		pBuff[0] = 0;
		return 0;
	}
}


/**************************************************************************
* 功能：根据命令获取一个用于菜单上的动作描述(pBuff)，返回描述长度，仅仅MENU
***************************************************************************/
int KUiChatPhrase::GetMenuEmote(const char *szCmd, char *pBuff, int nBuffLen, int nType)
{
	int nIndex = FindEmote(EMOTE_T_MENU, szCmd);

	return GetMenuEmote(nIndex, pBuff, nBuffLen, nType);
}
//上面的多态
/**************************************************************************
* 功能：根据索引获取一个用于菜单上的动作描述(pBuff)，返回描述长度，仅仅MENU
***************************************************************************/
int KUiChatPhrase::GetMenuEmote(int nIndex, char *pBuff, int nBuffLen, int nType)
{
	if(nIndex >= 0 && nIndex < m_nMenuEmoteCount && m_pMenuEmote &&
	   nBuffLen >= m_pMenuEmote[nIndex].nStringLen)
	{
        if(nType == 0)
		{
    		memcpy(pBuff, m_pMenuEmote[nIndex].szString, m_pMenuEmote[nIndex].nStringLen);
	    	return m_pMenuEmote[nIndex].nStringLen;
		}
		else
		{
			memcpy(pBuff, m_pMenuEmote[nIndex].szStringMe, m_pMenuEmote[nIndex].nStringMeLen);
	    	return m_pMenuEmote[nIndex].nStringMeLen;
		}
	}
	else
	{
		pBuff[0] = 0;
		return 0;
	}
}


/************************************************************************
* 功能：取出指定索引的动作名字
*************************************************************************/
int KUiChatPhrase::GetMenuEmoteName(int nIndex, char *pBuff, int nBuffLen)
{
	if(nIndex >= 0 && nIndex < m_nMenuEmoteCount && m_pMenuEmote &&
	   nBuffLen >= m_pMenuEmote[nIndex].nNameLen)
	{
		memcpy(pBuff, m_pMenuEmote[nIndex].szName, m_pMenuEmote[nIndex].nNameLen);
		return m_pMenuEmote[nIndex].nNameLen;
	}
	else
	{
		pBuff[0] = 0;
		return 0;
	}
}


/************************************************************************
* 功能：寻找一个命令所对应的表情数据的索引
*************************************************************************/
int KUiChatPhrase::FindEmote(EMOTE_TYPE eType, const char *szCmd)
{
	int i = -1, nCount, nBaseIndex;
	EMOTE_PHRASE *pData;

	if(eType == EMOTE_T_PHRASE)
	{
		pData = m_pEmotePhrase;
		nCount = m_nEmotePhraseCount;
		nBaseIndex = m_nMenuEmoteCount;
	}
	else
	{
		nBaseIndex = 0;
		pData = m_pMenuEmote;
		nCount = m_nMenuEmoteCount;
	}
	if(pData)
	{
		for(i = 0;i < nCount;i++)
		{
			if(strcmp(pData[i].szCmd, szCmd) == 0)
				break;
		}
		if(i >= nCount)
			i = -1;
	}
	return nBaseIndex + i;
}


/************************************************************************
* 功能：制作字符串
*************************************************************************/
int KUiChatPhrase::ConvertEmoteString(char *szString, int nStringLen, const char *szMyName, const char *szTarName)
{
	int nLen, i = 0, nNewMyNameLen, nNewTarNameLen;
	char szNewMyName[32], szNewTarName[32];

	szNewMyName[0] = KTC_COLOR;
	*(int *)(szNewMyName + 1) = m_uMyNameColor;
	nNewMyNameLen = strlen(szMyName);
	memcpy(szNewMyName + 4, szMyName, nNewMyNameLen);
	szNewMyName[4 + nNewMyNameLen] = KTC_COLOR_RESTORE;
	nNewMyNameLen += 5;

	szNewTarName[0] = KTC_COLOR;
	*(int *)(szNewTarName + 1) = m_uTargetColor;
	nNewTarNameLen = strlen(szTarName);
	memcpy(szNewTarName + 4, szTarName, nNewTarNameLen);
	szNewTarName[4 + nNewTarNameLen] = KTC_COLOR_RESTORE;
	nNewTarNameLen += 5;

	memmove(szString + 1, szString, nStringLen);
	szString[0] = '\t';
	nStringLen ++;
	nLen = nStringLen;
	while(1)
	{
		i = TFindSpecialCtrlInEncodedText(szString, nLen, i, '$');
		if(i != -1 && szString[i + 1] == 'N')
		{
			memmove(szString + nNewMyNameLen + i, szString + i + 2, nLen - i - 2);
			memcpy(szString + i, szNewMyName, nNewMyNameLen);
			nLen += (nNewMyNameLen - 2);
			i += (nNewMyNameLen - 2);
		}
		else if(i == -1)
		{
			break;
		}
		else
		{
			i++;
		}
	};

	i = 0;
    while(1)
	{
		i = TFindSpecialCtrlInEncodedText(szString, nLen, i, '$');
		if(i != -1 && szString[i + 1] == 'n')
		{
			memmove(szString + nNewTarNameLen + i, szString + i + 2, nLen - 2);
			memcpy(szString + i, szNewTarName, nNewTarNameLen);
			nLen += (nNewTarNameLen - 2);
			i += (nNewTarNameLen - 2);
		}
		else if(i == -1)
		{
			break;
		}
		else
		{
			i++;
		}
	};

	return nLen;
}

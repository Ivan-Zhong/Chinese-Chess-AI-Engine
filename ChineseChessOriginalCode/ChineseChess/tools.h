#ifndef _TOOLS_H_
#define _TOOLS_H_
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdint.h>
#include <cstring>
#include <string.h>
using namespace std;

//空闲状态的维持
inline void Idle(void)
{
	Sleep(1);
}

//比较字符串，若成功即后移
inline bool StrncmpSkip(char *&str1, const char *str2)
{
	if (_strnicmp(str1, str2, strlen(str2)) == 0)
	{
		str1 += strlen(str2);
		return true;
	}
	else
	{
		return false;
	}
}

//字符串转数字
inline int StrToInt(const char *str, int MIN, int MAX)
{
	int num = 0;
	if (sscanf_s(str, "%d", &num) > 0)
		return min(max(MIN, num), MAX);
	return MIN;
}

//判断字符串str2是否是str1的子串。如果是，则该函数返回 str1字符串从 str2第一次出现的位置开始到 str1结尾的字符串；否则，返回NULL
//不分大小写
inline char *strstri(char *str1, const char *str2)
{
	char *currBuffPointer = str1;

	while (*currBuffPointer != 0x00)
	{
		char *compareOne = currBuffPointer;
		const char *compareTwo = str2;
		//统一转换为小写字符
		while (tolower(*compareOne) == tolower(*compareTwo))
		{
			compareOne++;
			compareTwo++;
			if (*compareTwo == 0x00)
			{
				return (char *)currBuffPointer;
			}
		}
		currBuffPointer++;
	}
	return NULL;
}

//寻找字符串
inline bool findstr(char *&str1, const char *str2)
{
	char *lp;
	lp = strstri(str1, str2);
	if (lp == NULL)
		return false;
	else
	{
		str1 = lp + strlen(str2);
		return true;
	}
}

//返回ucci响应
inline void UcciResponse(const char *str)
{
	cout << str << endl;
	fflush(stdout);
}

//从字母获得棋的类型,0-6表示将士相马车炮兵
inline int ChToPiece(char ch)
{
	int type = -1;
	switch (ch)
	{
	case 'k':
		type = 0;
		break;
	case 'a':
		type = 1;
		break;
	case 'b':
		type = 2;
		break;
	case 'n':
		type = 3;
		break;
	case 'r':
		type = 4;
		break;
	case 'c':
		type = 5;
		break;
	case 'p':
		type = 6;
		break;
	default:
		break;
	}
	return type;
}

//从数字获得棋的类型,0-6表示红色将士相马车炮兵，7-13表示黑色将士相马车炮兵
inline int NumToPiece(int num)
{
	int type = -1;
	if (num == 0)
		return type;

	if (num == 16)
		type = 0;
	else if (num <= 18)
		type = 1;
	else if (num <= 20)
		type = 2;
	else if (num <= 22)
		type = 3;
	else if (num <= 24)
		type = 4;
	else if (num <= 26)
		type = 5;
	else if (num <= 31)
		type = 6;
	else if (num <= 32)
		type = 7;
	else if (num <= 33)
		type = 8;
	else if (num <= 35)
		type = 9;
	else if (num <= 37)
		type = 10;
	else if (num <= 39)
		type = 11;
	else if (num <= 41)
		type = 12;
	else if (num <= 43)
		type = 13;
	return type;
}

#endif
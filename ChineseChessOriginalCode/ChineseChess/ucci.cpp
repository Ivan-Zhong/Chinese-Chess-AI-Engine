#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "ucci.h"
using namespace std;

char FEN[MAX_UCCICOMM_LEN];
static uint32_t MovesList[MAX_MOVENUM];

#define TEST 1

//分析position指令的后缀信息，取得fen串与move列表
bool StrToPos(UcciComStruct& UcciComm, char* lp)
{
	if (StrncmpSkip(lp, "fen "))
	{
		//将fen中的字符串全部复制到UcciComm.FenStr中，等待后续以moves为边界进行截断
		strcpy_s(FEN, lp);
		UcciComm.FenStr = FEN;
	}

	//处理startpos的情况，在fen串中加入默认字符串
	else if (StrncmpSkip(lp, "startpos"))
	{
		strcpy_s(FEN, "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1");
		strcpy(FEN + strlen(FEN), lp);
		UcciComm.FenStr = FEN;
	}
	else
		return false;

	UcciComm.Num_Moves = 0;
	char* lp_fen = FEN;
	if (findstr(lp_fen, " moves "))
	{
		//将UcciComm.FenStr在moves之前截断
		*(lp_fen - strlen(" moves ")) = '\0';

		//由字符串长度得到moves的数目
		UcciComm.Num_Moves = min((int)(strlen(lp_fen) + 1) / 5, MAX_MOVENUM);

		//逐个获得moves
		for (int i = 0; i < UcciComm.Num_Moves; i++)
		{
			MovesList[i] = *(uint32_t*)lp_fen;
			lp_fen += sizeof(uint32_t) + 1;
		}
		UcciComm.List_Moves = MovesList;
	}
	else
		return false;
	return true;
}

//处理并解释引导状态下的信息
UcciCommEnum GuideState()
{
	char UcciCommStr[MAX_UCCICOMM_LEN];

	//等待指令传入
	while (!std::cin.getline(UcciCommStr, MAX_UCCICOMM_LEN))
	{
		Idle();
	}

	//分析指令
	if (_strnicmp(UcciCommStr, "ucci", strlen("ucci")) == 0)
	{
		return UCCI_COMM_UCCI;
	}
	else
	{
		return UCCI_COMM_UNKNOWN;
	}
}

//处理并解释空闲状态下的信息
UcciCommEnum IdleState(UcciComStruct& UcciCom, bool debug)
{
	char UcciCommStr[MAX_UCCICOMM_LEN];
	char* lp;

	//等待指令传入
	while (!std::cin.getline(UcciCommStr, MAX_UCCICOMM_LEN))
	{
		Idle();
	}
	lp = UcciCommStr;

	//分析指令

	//1.判断isready指令
	if (_strnicmp(lp, "isready", strlen("isready")) == 0)
	{
		return UCCI_COMM_ISREADY;
	}

	//2.判断position指令，若是，则对附带信息进行处理
	else if (StrncmpSkip(lp, "position "))
	{
		StrToPos(UcciCom, lp);

		//DEBUG模式下输出fen串与moves列表，以供检测
		if (debug)
		{
			cout << UcciCom.FenStr << endl;
			for (int i = 0; i < UcciCom.Num_Moves; i++)
				cout << UcciCom.List_Moves[i] << " ";
			cout << endl;
		}

		return UCCI_COMM_POSITION;
	}

#if TEST
	else if (StrncmpSkip(lp, "go "))
	{
		//将字符串转化为数字
		if (StrncmpSkip(lp, "time "))
		{
			//将字符串转化为数字
			UcciCom.nTimes = StrToInt(lp, MIN_GoTime, MAX_GoTime);
		}
		else
			UcciCom.nTimes = 60000;
		return UCCI_COMM_GO;
	}
#endif
	//3.判断go time指令，若是，则对附带信息进行处理
	else if (StrncmpSkip(lp, "go time "))
	{
		//将字符串转化为数字
		UcciCom.nTimes = StrToInt(lp, MIN_GoTime, MAX_GoTime);

		//DEBUG模式下输出时间，以供检测
		if (debug)
		{
			cout << UcciCom.nTimes << endl;
		}

		return UCCI_COMM_GO;
	}

	//4.判断quit指令
	else if (_strnicmp(lp, "quit", strlen("quit")) == 0)
	{
		return UCCI_COMM_QUIT;
	}
	else
	{
		return UCCI_COMM_UNKNOWN;
	}
}

//处理并解释思考状态下的信息
UcciCommEnum ThinkState(UcciComStruct& UcciCom, bool debug)
{
	char UcciCommStr[MAX_UCCICOMM_LEN];
	if (std::cin.getline(UcciCommStr, MAX_UCCICOMM_LEN))
	{
		//1.判断isready指令
		if (_strnicmp(UcciCommStr, "isready", strlen("isready")) == 0)
		{
			return UCCI_COMM_ISREADY;
		}
		//2.判断quit指令
		else if (_strnicmp(UcciCommStr, "quit", strlen("quit")) == 0)
		{
			return UCCI_COMM_QUIT;
		}
		else
			return UCCI_COMM_UNKNOWN;
	}
	else
		return UCCI_COMM_UNKNOWN;
}


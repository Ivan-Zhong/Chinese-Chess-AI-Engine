#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Search.h"
#include "ucci.h"
using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	UcciComStruct UcciComm;

	//处理引导状态指令
	if (GuideState() != UCCI_COMM_UCCI)
		return 0;
	Search search;
	UcciResponse("ucciok");
	UcciCommEnum UcciState = UCCI_COMM_UNKNOWN;
	while (UcciState != UCCI_COMM_QUIT)
	{
		switch (UcciState = IdleState(UcciComm, 0))			//第二个变量：1输出过程中结果以供DEBUG，0不输出过程中结果
		{
		case UCCI_COMM_ISREADY:
			UcciResponse("readyok");
			break;
		case UCCI_COMM_POSITION:
			search.setBoard(UcciComm);
			break;
		case UCCI_COMM_GO:
			search.searchMove(UcciComm.nTimes);
			break;
		case UCCI_COMM_QUIT:
			break;
		case UCCI_COMM_UNKNOWN:
			break;
		default:
			break;
		}
	}
	UcciResponse("bye");
	return 0;
}
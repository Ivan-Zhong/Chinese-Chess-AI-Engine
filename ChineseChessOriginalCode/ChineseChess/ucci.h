#pragma once
#include "tools.h"
using namespace std;

const int MAX_UCCICOMM_LEN = 8192;
const int MIN_GoTime = 0;
const int MAX_GoTime = 20000000;
const int MAX_MOVENUM = 1024;
const int SizeOfMove = 4;


//接受的UCCI指令的类型
enum UcciCommEnum
{
	UCCI_COMM_UNKNOWN,
	UCCI_COMM_UCCI,
	UCCI_COMM_ISREADY,
	UCCI_COMM_POSITION,
	UCCI_COMM_GO,
	UCCI_COMM_QUIT
};

//储存指令附带的信息
union UcciComStruct
{
	//position指令的附带信息
	struct
	{
		const char *FenStr;	  //储存fen串
		int Num_Moves;		  //储存后续着法的数目
		uint32_t *List_Moves; //储存后续着法的列表
	};

	//go time指令的附带信息
	struct
	{
		int nTimes; //储存限定的时间
	};
};

//分别在有引导状态、空闲状态、思考状态下接收并处理ucci指令的函数
UcciCommEnum GuideState();									 //引导状态
UcciCommEnum IdleState(UcciComStruct &UcciCom, bool debug);	 //空闲状态
UcciCommEnum ThinkState(UcciComStruct &UcciCom, bool debug); //思考状态
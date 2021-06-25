#include "Search.h"
#include "hash.h"
#include "mybuffer.h"
using namespace std;

#define USE_XQBASE_EVAL 1 // 1，使用象棋巫师的估值函数；0，使用自己的估值函数
//边界常量
const int EDGE_LEFT = 3;
const int EDGE_RIGHT = 11;
const int EDGE_TOP = 3;
const int EDGE_BOTTOM = 12;

//每种棋子序号的起始数字
const int black_start[7] = { 17, 18, 20, 22, 24, 26, 28 };
const int red_start[7] = { 1, 2, 4, 6, 8, 10, 12 };

const int blackPalace[9] = { 54, 55, 56, 70, 71, 72, 86, 87, 88 };
const int redPalace[9] = {166, 167, 168, 182, 183, 184, 198, 199, 200};

inline void printPiece(piece const& p) {
	switch (p) {
	case Empty: cout << "Empty"; break;
	case rKing: cout << "rKing"; break;
	case rShi1: cout << "rShi1"; break;
	case rShi2: cout << "rShi2"; break;
	case rXiang1: cout << "rXiang1"; break;
	case rXiang2: cout << "rXiang2"; break;
	case rMa1: cout << "rMa1"; break;
	case rMa2: cout << "rMa2"; break;
	case rChe1: cout << "rChe1"; break;
	case rChe2: cout << "rChe2"; break;
	case rPao1: cout << "rPao1"; break;
	case rPao2: cout << "rPao2"; break;
	case rZu1: cout << "rZu1"; break;
	case rZu2: cout << "rZu2"; break;
	case rZu3: cout << "rZu3"; break;
	case rZu4: cout << "rZu4"; break;
	case rZu5: cout << "rZu5"; break;
	case bKing: cout << "bKing"; break;
	case bShi1: cout << "bShi1"; break;
	case bShi2: cout << "bShi2"; break;
	case bXiang1: cout << "bXiang1"; break;
	case bXiang2: cout << "bXiang2"; break;
	case bMa1: cout << "bMa1"; break;
	case bMa2: cout << "bMa2"; break;
	case bChe1: cout << "bChe1"; break;
	case bChe2: cout << "bChe2"; break;
	case bPao1: cout << "bPao1"; break;
	case bPao2: cout << "bPao2"; break;
	case bZu1: cout << "bZu1"; break;
	case bZu2: cout << "bZu2"; break;
	case bZu3: cout << "bZu3"; break;
	case bZu4: cout << "bZu4"; break;
	case bZu5: cout << "bZu5"; break;
	}
}

Search::Search() :useBook(true), bookOpt(false), searchDepth(SEARCHDEPTH){
	longKillMove.src = 0;
	longKillMove.dst = 0;
	memset(cBoard, 0, sizeof(board));
	memset(history, 0, sizeof(history));
	initZobrist_table();
	initZobrist(*this); //??????需要吗
}

void Search::setBoard(UcciComStruct const& UcciComm)
{
	//每个棋子目前有几个
	int black_num[7] = { 0 };
	int red_num[7] = { 0 };
	int pos_pre = -1, pos_now = -1;

	/*按照fen串改cBoard*/
	//由fen串直接获得当前棋盘

	int i = EDGE_TOP;
	int j = EDGE_LEFT - 1;


	memset(cBoard, 0, sizeof(cBoard));
	const char* lp = UcciComm.FenStr;

	//对fen字符串进行解析
	while (*lp != ' ')
	{
		if (*lp == '/')
			//棋盘换行
		{
			i++;
			j = EDGE_LEFT - 1;
			if (i > EDGE_BOTTOM)
				break;
		}
		else if (*lp >= '1' && *lp <= '9')
			//棋盘跳过空位
		{
			j += *lp - '0';
			if (j > EDGE_RIGHT)
				break;
		}
		else if (*lp >= 'a' && *lp <= 'z')
			//处理黑方棋子
		{
			j++;

			//由字符得到当前棋子类型
			int k = ChToPiece(*lp);

			//给该棋子取号
			cBoard[i * 16 + j] = (piece)(black_start[k] + black_num[k]);
			black_num[k]++;

			if (j > EDGE_RIGHT)
				break;
		}
		else if (*lp >= 'A' && *lp <= 'Z')
			//处理红方棋子
		{
			j++;

			//由字符得到当前棋子类型
			int k = ChToPiece(*lp - 'A' + 'a');

			//给该棋子取号
			cBoard[i * 16 + j] = (piece)(red_start[k] + red_num[k]);
			red_num[k]++;

			if (j > EDGE_RIGHT)
				break;
		}
		lp++;
	}


	for (int i = 0; i < UcciComm.Num_Moves; i++)
	{
		//取更新的moves
		uint32_t move = UcciComm.List_Moves[i];
		//转化成字符串，如h2e2
		char* str = (char*)(&move);
		//由字符串得到两个位置
		pos_pre = (9 - (str[1] - '0') + EDGE_TOP) * 16 + str[0] - 'a' + EDGE_LEFT;
		pos_now = (9 - (str[3] - '0') + EDGE_TOP) * 16 + str[2] - 'a' + EDGE_LEFT;
		cBoard[pos_now] = cBoard[pos_pre];
		cBoard[pos_pre] = Empty;
	}

	//得到fen串的局面中谁先走
	if (UcciComm.FenStr[strlen(UcciComm.FenStr) - 9] == 'b')
		machineMax = false;
	else
		machineMax = true;//红方先走

	//从moves数量得到经过moves后谁先走
	if (UcciComm.Num_Moves % 2 == 1)
		machineMax = 1 ^ machineMax;

	//待添加：更新zobrist, zobrist_mir
	initZobrist(*this);

	cBoard_value = evaluate();
	killerSet.clear();

	//长打判断
	longKillMove.dst = 0;
	longKillMove.src = 0;
	if (UcciComm.Num_Moves >= 8 &&
		UcciComm.List_Moves[UcciComm.Num_Moves - 1] == UcciComm.List_Moves[UcciComm.Num_Moves - 5] &&
		UcciComm.List_Moves[UcciComm.Num_Moves - 2] == UcciComm.List_Moves[UcciComm.Num_Moves - 6] &&
		UcciComm.List_Moves[UcciComm.Num_Moves - 3] == UcciComm.List_Moves[UcciComm.Num_Moves - 7] &&
		UcciComm.List_Moves[UcciComm.Num_Moves - 4] == UcciComm.List_Moves[UcciComm.Num_Moves - 8])
	{
		// 取moves
		uint32_t move = UcciComm.List_Moves[UcciComm.Num_Moves - 4];
		//转化成字符串，如h2e2
		char* str = (char*)(&move);
		//由字符串得到两个位置
		longKillMove.src = (9 - (str[1] - '0') + EDGE_TOP) * 16 + str[0] - 'a' + EDGE_LEFT;
		longKillMove.dst = (9 - (str[3] - '0') + EDGE_TOP) * 16 + str[2] - 'a' + EDGE_LEFT;
	}

	//确定是否使用残局库,NUM_FINAL标注进入残局库的棋子数
	int chess_sum = 0;
	for (int i = 0; i < 256; i++)
		if (cBoard[i])
			chess_sum++;
	if (chess_sum <= NUM_FINAL)
	{
		useBook = true;
		bookOpt = true;
	}
}

state Search::judge(bool maximize) const {
	int blackKingPos = 0, redKingPos = 0;
	for (int i = 0; i < 9; ++i) {
		if (cBoard[(blackPalace[i])] == bKing)
		{
			blackKingPos = blackPalace[i];
			if (blackKingPos && redKingPos)
				break;
		}
		if (cBoard[(redPalace[i])] == rKing)
		{
			redKingPos = redPalace[i];
			if (blackKingPos && redKingPos)
				break;
		}
	}
	if (blackKingPos == 0)
		return redWin;
	if (redKingPos == 0)
		return blackWin;
	if ((blackKingPos & 15) == (redKingPos & 15))
	{
		for (int pos = redKingPos - 16; pos > blackKingPos; pos -= 16)
		{
			if (cBoard[pos] != Empty)
				return unfinished;
		}
		if (maximize)
			return redWin;
		else if (!maximize)
			return blackWin;
	}
	return unfinished;
}

int Search::minimax(int depth, int alpha, int beta, bool maximize)
{
	state curState = judge(maximize); // 先判断当前状态
	if (curState == redWin)
	{
		if (depth >= searchDepth - 3)
			return +inf * 2;
		return +inf;
	}
	else if (curState == blackWin)
	{
		if (depth >= searchDepth - 3)
			return -inf * 2;
		return -inf;
	}
	else if (curState == draw)
		return 0;
	if (depth == 0)   // 深度为零直接返回当前的估值
		return cBoard_value; // return value *****************
	vector<Move> vecMove;
	getMoves(vecMove, depth, maximize);
	int searchResult;
	int minEval = +evalInf;
	int maxEval = -evalInf;
	piece originalPiece;
	int originalValue;
	Move curBestMove = {0, 0};
	if (depth == searchDepth)
		bestMove = vecMove[0];
	for (int i = 0; i < vecMove.size(); ++i)
	{
		if (vecMove[i].value.isLongKill)
			continue;
		if ((clock() - startTime) >= searchTime) // 超时则返回
			break;
		//按照着法走子
		updateBoard(vecMove[i], originalPiece, originalValue); // 走子、value
		bool found = false;
		if (lookupTable.find(zobrist.dWlock1) != lookupTable.end())
		{
			lookupStruct ls = lookupTable[zobrist.dWlock1];
			if ((ls.depth >= (depth - 1)) && (((ls.depth - depth + 1) & 1) == 0))
			{
				searchResult = ls.eval;
				found = true;
			}
		}
		if (maximize)
		{
			if(!found)
			{
				searchResult = minimax(depth - 1, alpha, beta, false);
				lookupTable.insert(pair<int, lookupStruct>(zobrist.dWlock1, { depth - 1, searchResult }));
			}
			if (searchResult > maxEval)
			{
				maxEval = searchResult;
				curBestMove = vecMove[i];
				if (depth == searchDepth)
					bestMove = vecMove[i];
			}
			if (searchResult > alpha)
			{
				alpha = searchResult;
			}
			if (alpha >= beta)
			{
				killerSet.insert((depth << 16) + (vecMove[i].src << 8) + vecMove[i].dst);
				recoverBoard(vecMove[i], originalPiece, originalValue);
				// 更新历史表
				updateHistory(curBestMove, depth);
				return maxEval;
			}
		}
		else
		{
			if (!found)
			{
				searchResult = minimax(depth - 1, alpha, beta, true);
				lookupTable.insert(pair<int, lookupStruct>(zobrist.dWlock1, { depth - 1, searchResult }));
			}
			if (searchResult < minEval)
			{
				minEval = searchResult;
				curBestMove = vecMove[i];
				if (depth == searchDepth)
					bestMove = vecMove[i];
			}
			if (searchResult < beta)
			{
				beta = searchResult;
			}
			if (alpha >= beta)
			{
				killerSet.insert((depth << 16) + (vecMove[i].src << 8) + vecMove[i].dst);
				recoverBoard(vecMove[i], originalPiece, originalValue);
				// 更新历史表
				updateHistory(curBestMove, depth);
				return minEval;
			}
		}
		//恢复走子前的状态
		recoverBoard(vecMove[i], originalPiece, originalValue);
	}
	// 更新历史表
	updateHistory(curBestMove, depth);
	if (maximize)
		return maxEval;
	else
		return minEval;
}

void Search::searchMove(int const& time) // 搜索最优着法
{
	searchTime = (int)time * 0.9;
	if (!(useBook && searchBook(zobrist.dWlock1, zobrist_mir.dWlock1, bestMove, machineMax, bookOpt)))
	{
		useBook = false;
	    int alpha = -inf;
	    int beta = +inf;
	    startTime = clock();
	    int searchResult = minimax(searchDepth, alpha, beta, machineMax);
		if (lookupTable.find(zobrist.dWlock1) != lookupTable.end())
		{
			if (lookupTable[zobrist.dWlock1].depth < searchDepth)
			{
				lookupTable[zobrist.dWlock1].depth = searchDepth;
				lookupTable[zobrist.dWlock1].eval = searchResult;
			}
		}
		else
		{
			lookupTable.insert(pair<int, lookupStruct>(zobrist.dWlock1, { searchDepth, searchResult }));
		}
	}
    printBestMove();
}

void Search::getMoves(vector<Move>& vecMove, int depth, bool const& gmMaximize)
{
	int pos;
	if (gmMaximize) // 生成红方走法，红方为大写
	{
		for (int r = 3; r <= 12; ++r)
		{
			for (int c = 3; c <= 11; ++c)
			{
				pos = (r << 4) + c;
				switch (cBoard[pos])
				{
					case rKing:
						moveKing(vecMove, pos, depth, true);
						break;
					case rShi1:
					case rShi2:
						moveShi(vecMove, pos, depth, true);
						break;
					case rXiang1:
					case rXiang2:
						moveXiang(vecMove, pos, depth, true);
						break;
					case rMa1:
					case rMa2:
						moveMa(vecMove, pos, depth, true);
						break;
					case rChe1:
					case rChe2:
						moveChe(vecMove, pos, depth, true);
						break;
					case rPao1:
					case rPao2:
						movePao(vecMove, pos, depth, true);
						break;
					case rZu1:
					case rZu2:
					case rZu3:
					case rZu4:
					case rZu5:
						moveZu(vecMove, pos, depth, true);
						break;
					default:
						break;
				}
			}
		}
		sort(vecMove.begin(), vecMove.end(), RULEMAX());
	}
	else // 生成黑方走法，黑方为小写
	{
		for (int r = 3; r <= 12; ++r)
		{
			for (int c = 3; c <= 11; ++c)
			{
				pos = (r << 4) + c;
				switch (cBoard[pos])
				{
					case bKing:
						moveKing(vecMove, pos, depth, false);
						break;
					case bShi1:
					case bShi2:
						moveShi(vecMove, pos, depth, false);
						break;
					case bXiang1:
					case bXiang2:
						moveXiang(vecMove, pos, depth, false);
						break;
					case bMa1:
					case bMa2:
						moveMa(vecMove, pos, depth, false);
						break;
					case bChe1:
					case bChe2:
						moveChe(vecMove, pos, depth, false);
						break;
					case bPao1:
					case bPao2:
						movePao(vecMove, pos, depth, false);
						break;
					case bZu1:
					case bZu2:
					case bZu3:
					case bZu4:
					case bZu5:
						moveZu(vecMove, pos, depth, false);
						break;
					default:
						break;
				}
			}
		}
		sort(vecMove.begin(), vecMove.end(), RULEMIN());
	}
}
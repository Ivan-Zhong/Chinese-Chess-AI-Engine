#pragma once
#include "mybuffer.h"
#include "Search.h"
using namespace std;
//比较二者之间的Zobrist码
int book_zobrist_com(int currentZo, int ptr, const singlebookdata* bookbuffer)
{
    if ((unsigned int)currentZo < bookbuffer[ptr].Zobristcode)
        return -1;
    else if ((unsigned int)currentZo == bookbuffer[ptr].Zobristcode)
        return 0;
    else
        return 1;
}

bool searchBook(int currentZo, int currentZo_mir, Move& bestmove, bool red, bool final)
{
    const singlebookdata* bookbuffer = NULL;
    int BOOK_LENGTH = 0;
    if (red && final) {
        BOOK_LENGTH = RED_FINAL_BOOK_LENGTH;
        bookbuffer = finalredbook;
    }
    if (red && !final) {
        BOOK_LENGTH = RED_BOOK_LENGTH;
        bookbuffer = redbook;
    }
    if (!red && final) {
        BOOK_LENGTH = BLACK_FINAL_BOOK_LENGTH;
        bookbuffer = finalblackbook;
    }
    if (!red && !final) {
        BOOK_LENGTH = BLACK_BOOK_LENGTH;
        bookbuffer = blackbook;
    }


    int ptr, low, high;
    int i, min_value = 0xffffffff;
    int ret_mv = 0;
    //扫描开局库中是否有
    for (i = 1; i < 3; i++)
    {
        low = ptr = 0;
        high = BOOK_LENGTH - 1;
        bool found = false;
        while (low <= high)
        { //二分查找
            ptr = (high + low) / 2;
            int comp_ret;
            if (i == 1)
                comp_ret = book_zobrist_com(currentZo, ptr, bookbuffer);
            else
                comp_ret = book_zobrist_com(currentZo_mir, ptr, bookbuffer);
            if (comp_ret < 0)
                high = ptr - 1;
            else if (comp_ret > 0)
                low = ptr + 1;
            else
            {
                found = true;
                break;
            }
        }
        if (found)
            break;
    }
    if (i == 3) //没找到
        return false;
    if (i == 2)
        currentZo = currentZo_mir;

    while (ptr >= 0 && bookbuffer[ptr].Zobristcode == currentZo) //找到最前面的相同情况
        ptr--;
    ptr++; //多减了一次

    int j = ptr;
    ret_mv = ptr;
    while (j < BOOK_LENGTH && bookbuffer[j].Zobristcode == currentZo)
    {
        if (bookbuffer[j].value > min_value)
        {
            min_value = bookbuffer[j].value;
            ret_mv = j;
        }
        j++;
    }

    short mv = bookbuffer[ret_mv].move;
    int mvsrc = mv & 255;
    int srccol = mvsrc & 15, srcrow = mvsrc >> 4; //'C4'=>(11,5)

    int mvdst = (mv >> 8) & 255;
    int dstcol = mvdst & 15, dstrow = mvdst >> 4;

    if (i == 2)
    { //镜像搜索到的
        srccol = 14 - srccol;
        dstcol = 14 - dstcol;
    }
    bestmove.src = srcrow * 16 + srccol;
    bestmove.dst = dstrow * 16 + dstcol;

    return true;
}

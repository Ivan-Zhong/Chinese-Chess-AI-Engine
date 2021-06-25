#pragma once
#include <stdlib.h>
using namespace std;

struct rc4item
{
    unsigned char s[256];
    int x, y;

    void initzero(void){
        int i, j;
        unsigned char uc;

        x = y = j = 0;
        for (i = 0; i < 256; i++)
        {
            s[i] = i;
        }
        for (i = 0; i < 256; i++)
        {
            j = (j + s[i]) & 255;
            uc = s[i];
            s[i] = s[j];
            s[j] = uc;
        }
    }// 用空密钥初始化密码流生成器
    char nextchar(void)
    { // 生成密码流的下一个字节
        unsigned char uc;
        x = (x + 1) & 255;
        y = (y + s[x]) & 255;
        uc = s[x];
        s[x] = s[y];
        s[y] = uc;
        return s[(s[x] + s[y]) & 255];
    }
    int nextlong(void)
    { // 生成密码流的下四个字节
        unsigned char uc0, uc1, uc2, uc3;
        uc0 = nextchar();
        uc1 = nextchar();
        uc2 = nextchar();
        uc3 = nextchar();
        return uc0 + (uc1 << 8) + (uc2 << 16) + (uc3 << 24);
    }
};

// 用空密钥初始化密码流生成器
class ZobristItem
{
public:
    int dWkey, dWlock0, dWlock1;
    void setzero()
    {
        dWkey = dWlock0 = dWlock1 = 0;
    }
    void initrc4(rc4item &rc4)
    {
        dWkey = rc4.nextlong();
        dWlock0 = rc4.nextlong();
        dWlock1 = rc4.nextlong();
    }
    void Xor(const ZobristItem &zobr)
    {
        dWkey ^= zobr.dWkey;
        dWlock0 ^= zobr.dWlock0;
        dWlock1 ^= zobr.dWlock1;
    }
    void Xor(const ZobristItem &zobr1, const ZobristItem &zobr2)
    {
        dWkey ^= zobr1.dWkey ^ zobr2.dWkey;
        dWlock0 ^= zobr1.dWlock0 ^ zobr2.dWlock0;
        dWlock1 ^= zobr1.dWlock1 ^ zobr2.dWlock1;
    }
    void moveonepiece(int src, int pc);
};

struct ZobristTable
{
    ZobristItem Player;
    ZobristItem Table[14][256];
};

const int startchessboard[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const int chessforhash[33] = {-1, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 13, 13, 13}; //zobrist寻找table时转换的类
int getmirrorpos(int pos);
void initZobrist_table();
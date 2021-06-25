#include "hash.h"
#include "Search.h"
using namespace std;
ZobristTable Zobrist;
void ZobristItem::moveonepiece(int src, int pc)
{
    Xor(Zobrist.Table[chessforhash[pc]][src]);
}

void initZobrist_table()
{
    rc4item rc4;
    rc4.initzero();
    Zobrist.Player.initrc4(rc4);
    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 256; j++)
            Zobrist.Table[i][j].initrc4(rc4);
    }
}

void initZobrist(Search& s)
{
    s.zobrist.setzero();
    s.zobrist_mir.setzero();
    for (int i = 0; i <= 255; i++)
        if (s.cBoard[i])
        {
            s.zobrist.Xor(Zobrist.Table[chessforhash[s.cBoard[i]]][i]);
            s.zobrist_mir.Xor(Zobrist.Table[chessforhash[s.cBoard[i]]][getmirrorpos(i)]);
        }
}

int getmirrorpos(int pos)
{
    int line = pos / 16;
    int col = pos % 16;
    col = 14 - col;
    return line * 16 + col;
}

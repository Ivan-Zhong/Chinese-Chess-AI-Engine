#pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <ctime>
#include "hash.h"
#include "ucci.h"
#include "tools.h"
using namespace std;

#define DEBUG 0

const int origin_value[32] = { 10000, 250, 250, 250, 250, 300, 300, 500, 500, 300,
                              300, 80, 80, 80, 80, 80,
                              10000, 250, 250, 250, 250, 300, 300, 500, 500, 300, 300, 80, 80, 80, 80, 80 };

const int origin_possibility[32] = { 1, 1, 1, 2, 2, 2, 2, 1,
                                    1, 4, 4, 1, 1, 1, 1, 1,
                                    1, 1, 1, 2, 2, 2, 2, 1,
                                    1, 4, 4, 1, 1, 1, 1, 1 };

//示例的子力价值，参考
// 子力位置价值表
//顺序：
const int pieceIndex[33] = { 0,1,2,2,3,3,4,4,5,5,6,6,7,7,7,7,7,8,9,9,10,10,11,11,12,12,13,13,14,14,14,14,14 };
static const int pieceEatenValue[33] = { 0, -7, -2, -2, -3,-3, -4,-4, -6,-6, -5,-5, -1,-1,-1,-1,-1, 7, 2, 2, 3, 3, 4, 4, 6, 6, 5, 5, 1, 1, 1,1,1 };
static const uint8_t piece_value[15][256] = {
  { // 空
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  },
  { // 红帅(将)
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 11, 15, 11,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 红仕(士)
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 23,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 红相(象)
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, 18,  0,  0,  0, 23,  0,  0,  0, 18,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 红马
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, 90, 90, 90, 96, 90, 96, 90, 90, 90,  0,  0,  0,  0,
    0,  0,  0, 90, 96,103, 97, 94, 97,103, 96, 90,  0,  0,  0,  0,
    0,  0,  0, 92, 98, 99,103, 99,103, 99, 98, 92,  0,  0,  0,  0,
    0,  0,  0, 93,108,100,107,100,107,100,108, 93,  0,  0,  0,  0,
    0,  0,  0, 90,100, 99,103,104,103, 99,100, 90,  0,  0,  0,  0,
    0,  0,  0, 90, 98,101,102,103,102,101, 98, 90,  0,  0,  0,  0,
    0,  0,  0, 92, 94, 98, 95, 98, 95, 98, 94, 92,  0,  0,  0,  0,
    0,  0,  0, 93, 92, 94, 95, 92, 95, 94, 92, 93,  0,  0,  0,  0,
    0,  0,  0, 85, 90, 92, 93, 78, 93, 92, 90, 85,  0,  0,  0,  0,
    0,  0,  0, 88, 85, 90, 88, 90, 88, 90, 85, 88,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 红车
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,206,208,207,213,214,213,207,208,206,  0,  0,  0,  0,
    0,  0,  0,206,212,209,216,233,216,209,212,206,  0,  0,  0,  0,
    0,  0,  0,206,208,207,214,216,214,207,208,206,  0,  0,  0,  0,
    0,  0,  0,206,213,213,216,216,216,213,213,206,  0,  0,  0,  0,
    0,  0,  0,208,211,211,214,215,214,211,211,208,  0,  0,  0,  0,
    0,  0,  0,208,212,212,214,215,214,212,212,208,  0,  0,  0,  0,
    0,  0,  0,204,209,204,212,214,212,204,209,204,  0,  0,  0,  0,
    0,  0,  0,198,208,204,212,212,212,204,208,198,  0,  0,  0,  0,
    0,  0,  0,200,208,206,212,200,212,206,208,200,  0,  0,  0,  0,
    0,  0,  0,194,206,204,212,200,212,204,206,194,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 红炮
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,100,100, 96, 91, 90, 91, 96,100,100,  0,  0,  0,  0,
    0,  0,  0, 98, 98, 96, 92, 89, 92, 96, 98, 98,  0,  0,  0,  0,
    0,  0,  0, 97, 97, 96, 91, 92, 91, 96, 97, 97,  0,  0,  0,  0,
    0,  0,  0, 96, 99, 99, 98,100, 98, 99, 99, 96,  0,  0,  0,  0,
    0,  0,  0, 96, 96, 96, 96,100, 96, 96, 96, 96,  0,  0,  0,  0,
    0,  0,  0, 95, 96, 99, 96,100, 96, 99, 96, 95,  0,  0,  0,  0,
    0,  0,  0, 96, 96, 96, 96, 96, 96, 96, 96, 96,  0,  0,  0,  0,
    0,  0,  0, 97, 96,100, 99,101, 99,100, 96, 97,  0,  0,  0,  0,
    0,  0,  0, 96, 97, 98, 98, 98, 98, 98, 97, 96,  0,  0,  0,  0,
    0,  0,  0, 96, 96, 97, 99, 99, 99, 97, 96, 96,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 红兵(卒)
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  9,  9,  9, 11, 13, 11,  9,  9,  9,  0,  0,  0,  0,
    0,  0,  0, 19, 24, 34, 42, 44, 42, 34, 24, 19,  0,  0,  0,  0,
    0,  0,  0, 19, 24, 32, 37, 37, 37, 32, 24, 19,  0,  0,  0,  0,
    0,  0,  0, 19, 23, 27, 29, 30, 29, 27, 23, 19,  0,  0,  0,  0,
    0,  0,  0, 14, 18, 20, 27, 29, 27, 20, 18, 14,  0,  0,  0,  0,
    0,  0,  0,  7,  0, 13,  0, 16,  0, 13,  0,  7,  0,  0,  0,  0,
    0,  0,  0,  7,  0,  7,  0, 15,  0,  7,  0,  7,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  },
  { // 黑帅(将)
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0, 11, 15, 11,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 黑仕(士)
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, 23,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 黑相(象)
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0, 18,  0,  0,  0, 23,  0,  0,  0, 18,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 黑马
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0, 88, 85, 90, 88, 90, 88, 90, 85, 88,  0,  0,  0,  0,
  0,  0,  0, 85, 90, 92, 93, 78, 93, 92, 90, 85,  0,  0,  0,  0,
  0,  0,  0, 93, 92, 94, 95, 92, 95, 94, 92, 93,  0,  0,  0,  0,
  0,  0,  0, 92, 94, 98, 95, 98, 95, 98, 94, 92,  0,  0,  0,  0,
  0,  0,  0, 90, 98,101,102,103,102,101, 98, 90,  0,  0,  0,  0,
  0,  0,  0, 90,100, 99,103,104,103, 99,100, 90,  0,  0,  0,  0,
  0,  0,  0, 93,108,100,107,100,107,100,108, 93,  0,  0,  0,  0,
  0,  0,  0, 92, 98, 99,103, 99,103, 99, 98, 92,  0,  0,  0,  0,
  0,  0,  0, 90, 96,103, 97, 94, 97,103, 96, 90,  0,  0,  0,  0,
  0,  0,  0, 90, 90, 90, 96, 90, 96, 90, 90, 90,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 黑车
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,194,206,204,212,200,212,204,206,194,  0,  0,  0,  0,
  0,  0,  0,200,208,206,212,200,212,206,208,200,  0,  0,  0,  0,
  0,  0,  0,198,208,204,212,212,212,204,208,198,  0,  0,  0,  0,
  0,  0,  0,204,209,204,212,214,212,204,209,204,  0,  0,  0,  0,
  0,  0,  0,208,212,212,214,215,214,212,212,208,  0,  0,  0,  0,
  0,  0,  0,208,211,211,214,215,214,211,211,208,  0,  0,  0,  0,
  0,  0,  0,206,213,213,216,216,216,213,213,206,  0,  0,  0,  0,
  0,  0,  0,206,208,207,214,216,214,207,208,206,  0,  0,  0,  0,
  0,  0,  0,206,212,209,216,233,216,209,212,206,  0,  0,  0,  0,
  0,  0,  0,206,208,207,213,214,213,207,208,206,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 黑炮
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0, 96, 96, 97, 99, 99, 99, 97, 96, 96,  0,  0,  0,  0,
  0,  0,  0, 96, 97, 98, 98, 98, 98, 98, 97, 96,  0,  0,  0,  0,
  0,  0,  0, 97, 96,100, 99,101, 99,100, 96, 97,  0,  0,  0,  0,
  0,  0,  0, 96, 96, 96, 96, 96, 96, 96, 96, 96,  0,  0,  0,  0,
  0,  0,  0, 95, 96, 99, 96,100, 96, 99, 96, 95,  0,  0,  0,  0,
  0,  0,  0, 96, 96, 96, 96,100, 96, 96, 96, 96,  0,  0,  0,  0,
  0,  0,  0, 96, 99, 99, 98,100, 98, 99, 99, 96,  0,  0,  0,  0,
  0,  0,  0, 97, 97, 96, 91, 92, 91, 96, 97, 97,  0,  0,  0,  0,
  0,  0,  0, 98, 98, 96, 92, 89, 92, 96, 98, 98,  0,  0,  0,  0,
  0,  0,  0,100,100, 96, 91, 90, 91, 96,100,100,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 黑兵(卒)
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  7,  0,  7,  0, 15,  0,  7,  0,  7,  0,  0,  0,  0,
  0,  0,  0,  7,  0, 13,  0, 16,  0, 13,  0,  7,  0,  0,  0,  0,
  0,  0,  0, 14, 18, 20, 27, 29, 27, 20, 18, 14,  0,  0,  0,  0,
  0,  0,  0, 19, 23, 27, 29, 30, 29, 27, 23, 19,  0,  0,  0,  0,
  0,  0,  0, 19, 24, 32, 37, 37, 37, 32, 24, 19,  0,  0,  0,  0,
  0,  0,  0, 19, 24, 34, 42, 44, 42, 34, 24, 19,  0,  0,  0,  0,
  0,  0,  0,  9,  9,  9, 11, 13, 11,  9,  9,  9,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }
};

int const SEARCHDEPTH = 5;
int const NUM_FINAL = 12;

enum piece
{
    Empty,
    rKing,
    rShi1,
    rShi2,
    rXiang1,
    rXiang2,
    rMa1,
    rMa2,
    rChe1,
    rChe2,
    rPao1,
    rPao2,
    rZu1,
    rZu2,
    rZu3,
    rZu4,
    rZu5,
    bKing,
    bShi1,
    bShi2,
    bXiang1,
    bXiang2,
    bMa1,
    bMa2,
    bChe1,
    bChe2,
    bPao1,
    bPao2,
    bZu1,
    bZu2,
    bZu3,
    bZu4,
    bZu5
};

char const pieceFenMapping[] = { ' ', 'K', 'A', 'A', 'B', 'B', 'N', 'N', 'R', 'R', 'C', 'C', 'P', 'P', 'P', 'P', 'P', 'k', 'a', 'a', 'b', 'b', 'n', 'n', 'r', 'r', 'c', 'c', 'p', 'p', 'p', 'p', 'p' };

int const RedMargin = 8;
int const BlackMargin = 7;

// 需要考察估值函数的情况重新确定，现在是暂时的值
int const inf = 200000;
int const evalInf = 2000000;

struct stepEval {
    bool isLongKill;
    bool isKiller;
    int historicValue;
    int killEval;
    int staticEval;
};

// 着法结构体
struct Move
{
    int src, dst; // 着法
    stepEval value;
};
typedef piece board[256]; // 棋盘类型

enum state // 当前棋局状态
{
    blackWin,
    redWin,
    draw,
    unfinished
};

struct RULEMAX {
    bool operator()(Move const& m1, Move const& m2) const {
        if (!m1.value.isLongKill && m2.value.isLongKill)
            return true;
        else if ((m1.value.isLongKill && m2.value.isLongKill)
            || (!m1.value.isLongKill && !m2.value.isLongKill))
        {
            if (m1.value.isKiller && !m2.value.isKiller)
                return true;
            else if ((m1.value.isKiller && m2.value.isKiller)
                || (!m1.value.isKiller && !m2.value.isKiller))
            {
                if (m1.value.historicValue > m2.value.historicValue)
                    return true;
                else if (m1.value.historicValue == m2.value.historicValue 
                    && m1.value.killEval > m2.value.killEval)
                    return true;
                else if (m1.value.historicValue == m2.value.historicValue
                    && m1.value.killEval == m2.value.killEval
                    && m1.value.staticEval > m2.value.staticEval)
                    return true;
            }
            return false;
        }
        return false;
    }
};

struct RULEMIN {
    bool operator()(Move const& m1, Move const& m2) const {
        if (!m1.value.isLongKill && m2.value.isLongKill)
            return true;
        else if ((m1.value.isLongKill && m2.value.isLongKill)
            || (!m1.value.isLongKill && !m2.value.isLongKill))
        {
            if (m1.value.isKiller && !m2.value.isKiller)
                return true;
            else if ((m1.value.isKiller && m2.value.isKiller)
                || (!m1.value.isKiller && !m2.value.isKiller))
            {
                if (m1.value.historicValue > m2.value.historicValue)
                    return true;
                else if (m1.value.historicValue == m2.value.historicValue
                    && m1.value.killEval < m2.value.killEval)
                    return true;
                else if (m1.value.historicValue == m2.value.historicValue
                    && m1.value.killEval == m2.value.killEval
                    && m1.value.staticEval < m2.value.staticEval)
                    return true;
            }
            return false;
        }
        return false;
    }
};

struct Chess
{
    int possibility; // 棋子灵活度
    int alive;       // 活着1，死了是0
    int importance;  // 活着是子力数，死了是0
    int security;    // 安全程度：under protection+=保护子力和 under attack-=攻击子力和
    int position;    // 在棋盘中的索引
};
typedef Chess chessArray[33];

struct lookupStruct
{
    int depth, eval;
};

class Search
{
public:
    Search();
    void setBoard(UcciComStruct const &UcciComm);                                                           // 构造函数
    void searchMove(int const &time); // 然后调用这个（陈）
private:
    inline void printBestMove();
    void getMoves(vector<Move> &vecMove, int depth, bool const &gmMaximize);
    int minimax(int depth, int alpha, int beta, bool maximize);
    inline void updateBoard(Move const& m, piece& originalPiece, int& originalValue);
    inline void recoverBoard(Move const& m, piece const& originalPiece, int const& originalValue);
    inline int evaluate() const;
    inline stepEval evaluate_step(int depth, int src, int dst) const;
    inline void moveKing(vector<Move> &vecMove, int const &pos, int depth, bool red); ///adsjfklasdjklfjasdlkf
    inline void moveShi(vector<Move> &vecMove, int const &pos, int depth, bool red);
    inline void moveXiang(vector<Move> &vecMove, int const &pos, int depth, bool red);
    inline void moveMa(vector<Move> &vecMove, int const &pos, int depth, bool red);
    inline void moveChe(vector<Move> &vecMove, int const &pos, int depth, bool red);
    inline void movePao(vector<Move> &vecMove, int const &pos, int depth, bool red);
    inline void moveZu(vector<Move> &vecMove, int const &pos, int depth, bool red);
    inline bool isBlack(piece const &piece) const;
    inline bool isRed(piece const &piece) const;
    inline void drawBoard() const;
    inline void updateHistory(Move const& curBestMove, int depth);
    state judge(bool maximize) const;
    friend void initZobrist(Search &s);
    bool machineMax; // AI走法是否要价值最大化
    board cBoard;
    int searchTime; // (unit: millisecond)
    int searchDepth;
    clock_t startTime;
    Move bestMove;
    ZobristItem zobrist;
    ZobristItem zobrist_mir;
    int cBoard_value;
   set<int> killerSet;
    long long history[256][256];
    Move longKillMove;
    bool useBook; // true为搜索库
    bool bookOpt; // false为搜索开局库，true为搜索残局库
    map<int, lookupStruct> lookupTable;

};

const int kingMove[4] = {-16, -1, 16, 1};
const int shiMove[4] = {17, -17, 15, -15};
const int shiMove1[8] = { -16, 17, 1, -17, 16, 15, -1, -15 };
const int xiangMove[4] = {34, -34, 30, -30};
const int maMove[8] = {-14, -18, 33, 31, 14, 18, -33, -31};
const int maMove_illegal[8] = {1, -1, 16, 16, -1, 1, -16, -16};
const int maMove_illegal_reversal[8] = {-15, -17, 17, 15, 15, 17, -17, -15};

int const PawnDirections[2] = {1, -1};
int const directions[4] = {-16, -1, 16, 1};

// 判断棋子是否在棋盘子上
bool const inChessboard[256] = {
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

bool const inPalace[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

char const stepDict[256][3] =
    {
        "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "a9", "b9", "c9", "d9", "e9", "f9", "g9", "h9", "i9", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "i8", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "i7", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "i6", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "i5", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "i4", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "i3", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "i2", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "i1", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "a0", "b0", "c0", "d0", "e0", "f0", "g0", "h0", "i0", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ",
        "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "};
//
//void Search::drawBoard() const {
//    cout << "###########################" << endl;
//    cout << "#    a b c d e f g h i    #" << endl;
//    cout << "#                         #" << endl;
//    for (int r = 3; r <= 12; ++r)
//    {
//        cout << "# " << 12 - r << " ";
//        for (int c = 3; c <= 11; ++c)
//        {
//            cout << " " << pieceFenMapping[(cBoard[r * 16 + c])];
//        }
//        cout << "  " << 12 - r << " #";
//        cout << endl;
//    }
//    cout << "#                         #" << endl;
//    cout << "#    a b c d e f g h i    #" << endl;
//    cout << "###########################" << endl;
//}

void Search::printBestMove()
{
    cout << "bestmove " << stepDict[bestMove.src] << stepDict[bestMove.dst] << endl;
    fflush(stdout);
}

int Search::evaluate() const { 
    int redValue = 0;
    int blackValue = 0;
    int pos;
    for (int r = 3; r <= 12; ++r)
    {
        for (int c = 3; c <= 11; ++c)
        {
            pos = r * 16 + c;
            if (isBlack(cBoard[pos]))
                blackValue += piece_value[(pieceIndex[(cBoard[pos])])][pos];
            if (isRed(cBoard[pos]))
                redValue += piece_value[(pieceIndex[(cBoard[pos])])][pos];
        }
    }
    return redValue - blackValue;
}

void Search::updateBoard(Move const& m, piece& originalPiece, int& originalValue)
{
    zobrist.moveonepiece(m.src, cBoard[m.src]);
    zobrist.moveonepiece(m.dst, cBoard[m.src]);
    if(cBoard[m.dst])
        zobrist.moveonepiece(m.dst, cBoard[m.dst]);
    originalPiece = cBoard[m.dst];
    cBoard[m.dst] = cBoard[m.src];
    cBoard[m.src] = Empty;
    originalValue = cBoard_value;
    cBoard_value = m.value.staticEval;
}

void Search::recoverBoard(Move const& m, piece const& originalPiece, int const&originalValue)
{
    zobrist.moveonepiece(m.dst, cBoard[m.dst]);
    zobrist.moveonepiece(m.src, cBoard[m.dst]);
    if (originalPiece)
        zobrist.moveonepiece(m.dst, originalPiece);
    cBoard[m.src] = cBoard[m.dst];
    cBoard[m.dst] = originalPiece;
    cBoard_value = originalValue;
}

bool Search::isBlack(piece const &piece) const
{
    return piece >= 17;
}

bool Search::isRed(piece const &piece) const
{
    return piece <= 16 && piece >= 1;
}

stepEval Search::evaluate_step(int depth, int src, int dst) const {
    stepEval se;
    se.historicValue = history[src][dst];
    se.isKiller = killerSet.find((depth << 16)+ (src << 8) + dst) != killerSet.end();
    se.staticEval = cBoard_value;
    se.killEval = pieceEatenValue[cBoard[dst]];
    if (isBlack(cBoard[src]))
    {
        se.staticEval = se.staticEval + piece_value[(pieceIndex[(cBoard[src])])][src]
            - piece_value[(pieceIndex[(cBoard[src])])][dst];
    }
    else if (isRed(cBoard[src]))
    {
        se.staticEval = se.staticEval - piece_value[(pieceIndex[(cBoard[src])])][src]
            + piece_value[(pieceIndex[(cBoard[src])])][dst];
    }
    else
        cerr << "FATAL ERROR!!! Source is Empty!" << endl;
    if (isBlack(cBoard[dst]))
        se.staticEval += piece_value[(pieceIndex[(cBoard[dst])])][dst] * 4;
    else if(isRed(cBoard[dst]))
        se.staticEval -= piece_value[(pieceIndex[(cBoard[dst])])][dst] * 4;
    if (src == longKillMove.src && dst == longKillMove.dst)
        se.isLongKill = true;
    else
        se.isLongKill = false;
    return se;
}

void Search::moveKing(vector<Move> &vecMove, int const &pos, int depth, bool red) // red为true表示当前是红方在走King
{
    int newpos;
    for (int i = 0; i < 4; ++i)
    {
        newpos = pos + kingMove[i];
        if (inChessboard[newpos] && inPalace[newpos])
        {
            if (cBoard[newpos] == Empty)
            {
                vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) }); // 获得估值
            }
            else
            {
                if (isBlack(cBoard[newpos]))
                {
                    if (red)
                        vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                }
                else if(isRed(cBoard[newpos]))
                {
                    if (!red)
                        vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                }
            }
        }
    }
}

void Search::moveShi(vector<Move> &vecMove, int const &pos, int depth, bool red)
{
    int newpos;
    for (int i = 0; i < 4; ++i)
    {
        newpos = pos + shiMove[i];
        if (inChessboard[newpos] && inPalace[newpos])
        {
            if (cBoard[newpos] == Empty)
            {
                vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
            }
            else
            {
                if (isBlack(cBoard[newpos]))
                {
                    if (red)
                        vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                }
                else if(isRed(cBoard[newpos]))
                {
                    if (!red)
                        vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                }
            }
        }
    }
}

void Search::moveXiang(vector<Move> &vecMove, int const &pos, int depth, bool red)
{
    int newpos;
    for (int i = 0; i < 4; ++i)
    {
        newpos = pos + xiangMove[i];
        if (inChessboard[newpos])
        {
            if (cBoard[pos + shiMove[i]] != Empty) // 塞象眼
                continue;
            if (red && ((newpos >> 4) <= BlackMargin))
                continue;
            if (!red && ((newpos >> 4) >= RedMargin))
                continue;
            if (cBoard[newpos] == Empty)
            {
                vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
            }
            else
            {
                if (isBlack(cBoard[newpos]))
                {
                    if (red)
                        vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                }
                else if(isRed(cBoard[newpos]))
                {
                    if (!red)
                        vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                }
            }
        }
    }
}

void Search::moveMa(vector<Move> &vecMove, int const &pos, int depth, bool red)
{
    int newpos;
    for (int i = 0; i < 8; ++i)
    {
        newpos = pos + maMove[i];
        if (inChessboard[newpos])
        {
            if (cBoard[pos + maMove_illegal[i]] != 0) // 蹩马腿
                continue;
            if (cBoard[newpos] == 0)
            {
                vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
            }
            else
            {
                if (isBlack(cBoard[newpos]))
                {
                    if (red)
                        vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                }
                else if(isRed(cBoard[newpos]))
                {
                    if (!red)
                        vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                }
            }
        }
    }
}

void Search::moveZu(vector<Move> &vecMove, int const &pos, int depth, bool red)
{
    int newpos;
    if (red)
    {
        newpos = pos - 16;
        if (cBoard[newpos] == Empty || isBlack(cBoard[newpos]))
            vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
        if ((pos >> 4) <= BlackMargin)
        {
            for (int i = 0; i < 2; ++i)
            {
                newpos = pos + PawnDirections[i];
                if (inChessboard[newpos])
                {
                    if (cBoard[newpos] == Empty || isBlack(cBoard[newpos]))
                        vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                }
            }
        }
    }
    else
    {
        newpos = pos + 16;
        if (cBoard[newpos] == 0 || isRed(cBoard[newpos]))
            vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
        if ((pos >> 4) >= RedMargin)
        {
            for (int i = 0; i < 2; ++i)
            {
                newpos = pos + PawnDirections[i];
                if (inChessboard[newpos])
                {
                    if ((cBoard[newpos] == Empty) || isRed(cBoard[newpos]))
                        vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                }
            }
        }
    }
}

void Search::moveChe(vector<Move> &vecMove, int const &pos, int depth, bool red)
{
    int newpos;
    for (int i = 0; i < 4; ++i)
    {
        newpos = pos + directions[i];
        while (true)
        {
            if (!inChessboard[newpos])
                break;
            if (cBoard[newpos] == Empty)
                vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
            else
            {
                if (red && isBlack(cBoard[newpos]))
                    vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                else if (!red && isRed(cBoard[newpos]))
                    vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                break;
            }
            newpos += directions[i];
        }
    }
}

void Search::movePao(vector<Move> &vecMove, int const &pos, int depth, bool red)
{
    int newpos;
    int midpos;
    bool haveMid;
    bool haveTarget;
    for (int i = 0; i < 4; ++i)
    {
        haveMid = haveTarget = true;
        // 找本方向上第一个子
        midpos = pos + directions[i];
        while (true)
        {
            if (!inChessboard[midpos])
            {
                haveMid = false;
                break;
            }
            if (cBoard[midpos] != Empty)
            {
                newpos = midpos + directions[i];
                break;
            }
            else
                vecMove.push_back({ pos, midpos, evaluate_step(depth, pos, midpos)});
            midpos += directions[i];
        }
        if (!haveMid)
            continue;
        while (true)
        {
            if (!inChessboard[newpos])
                break;
            if (cBoard[newpos] != Empty)
            {
                if (red && isBlack(cBoard[newpos]))
                    vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                else if (!red && isRed(cBoard[newpos]))
                    vecMove.push_back({ pos, newpos, evaluate_step(depth, pos, newpos) });
                break;
            }
            newpos += directions[i];
        }
    }
}

void Search::updateHistory(Move const& curBestMove, int depth)
{
    history[curBestMove.src][curBestMove.dst] += depth * depth;
}
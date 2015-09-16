#ifndef __BAB_LIST___
#define __BAB_LIST___
//#define PAS_SRCFILE    3101
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include "utoall.h"
#include "ncsdef.h"
#include "pasutl.h"
#include "pasdb.h"
#include "pasmessage.h"
#include "ncapi.h"
#include "ncmac.h"


#include <time.h>
#include <malloc.h>
#include <pthread.h>
#include "string.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

struct s_maccount
{
    char mac[20];
	ulong count;
};



struct s_macinfo
{
    char mac[20];
    char servicecode[17];
    uint8 starttime;
    uint8 endtime;
    uint4 cid;
};

typedef struct eachpoint
{
    uint4 cid;
    uint8 starttime;
    uint8 endtime;
    char servicecode[17];
    char mac[20];
    struct eachpoint* pNext;
} s_eachpoint;



struct s_babtask
{
    uint4 cid;
    uint4 groupid;
    uint8 starttime;
    uint8 endtime;
    char dids[255];
    char mac[20];
};

s_eachpoint* createPointList();

/**
* 释放列表占用的所有内存
*/
int destroyPointList(s_eachpoint* pFirstNode);

/**
* 相当于拷贝构造函数
*/
s_eachpoint* createNodeByData(s_eachpoint* pPoint);


/**
* 按starttime从小到大的顺序插入到链表
*/
int addToPointList(s_eachpoint* pFirstNode, s_eachpoint* pPoint);

/**
*根据定制规则，合并相邻starttime在1小时内，servicecode相同的结点
*/
int reOrganizePointList(s_eachpoint* pFirstNode);


ulong getPointListLength(s_eachpoint* pFirstNode);




#endif

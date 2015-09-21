/*

*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bab_list.h"

/**
* 创建一个列表返回头结点地址，头结点的数据不使用
*/
s_eachpoint* createPointList()
{
    s_eachpoint* pFirstNode = (s_eachpoint*)malloc(sizeof(s_eachpoint));
    memset(pFirstNode, 0, sizeof(s_eachpoint));
    pFirstNode->pNext = NULL;
    return pFirstNode;
}

/**
* 释放列表占用的所有内存
*/
int destroyPointList(s_eachpoint* pFirstNode)
{
    s_eachpoint* pNextNode = NULL;
    if(!pFirstNode)
        return -1;
    free(pFirstNode);

    s_eachpoint* pNode = pFirstNode->pNext;
    while(pNode)
    {
        pNextNode = pNode->pNext;
        free(pNode);
        pNode = pNextNode;
    }
    return 0;
}

/**
* 相当于拷贝构造函数
*/
s_eachpoint* createNodeByData(s_eachpoint* pPoint)
{
    s_eachpoint* insertNode = (s_eachpoint*)malloc(sizeof(s_eachpoint));
    memset(insertNode, 0, sizeof(s_eachpoint));
    insertNode->cid = pPoint->cid;
    memcpy(insertNode->mac, pPoint->mac, sizeof(insertNode->mac));
    insertNode->starttime = pPoint->starttime;
    insertNode->endtime = pPoint->endtime;
    memcpy(insertNode->servicecode, pPoint->servicecode, sizeof(insertNode->servicecode));
    return insertNode;
}


/**
* 按starttime从小到大的顺序插入到链表
*/
int addToPointList(s_eachpoint* pFirstNode, s_eachpoint* pPoint)
{
    s_eachpoint* insertNode = NULL;
    s_eachpoint* pNextNode = NULL;
    s_eachpoint* pBeforeNode = pFirstNode;
    if(!pFirstNode)
        return -1;
    s_eachpoint* pNode = pFirstNode->pNext;//第一个结点
    // DO:要判断下是否取代第一个结点
    if(pNode)
    {
        //printf("next stime=%llu, first stime=%llu", pPoint->starttime, pNode->starttime);
        if(pPoint->starttime < pNode->starttime)
        {
          //  printf(",插入第一个以前\n");
            insertNode = createNodeByData(pPoint);
            pFirstNode->pNext = insertNode;
            insertNode->pNext = pNode;
            return 0;
        }
    }

    while(pNode)
    {
        //如果已经是最后一个结点特殊处理,直接插入到最后即可
        if(!pNode->pNext)
        {
            //printf("已经是最后一个，插入最后\n");
            insertNode = createNodeByData(pPoint);
            pNode->pNext = insertNode;
            insertNode->pNext = NULL;
            return 0;
        }
        else
        {
           // printf("bStime=%llu, pStime=%llu nStime=%llu", pNode->starttime , pPoint->starttime, pNode->pNext->starttime);
            if((pNode->starttime < pPoint->starttime) && (pNode->pNext->starttime > pPoint->starttime))
            {
                pNextNode = pNode ->pNext;
               // printf("入到pNode之后\n");
                insertNode = createNodeByData(pPoint);
                pNode->pNext = insertNode;
                insertNode->pNext = pNextNode;
                return 0;
            }
        }
        pBeforeNode = pNode;
        pNode = pNode->pNext;
    }
  //  printf("是最后一个，插入最后\n");
    insertNode = createNodeByData(pPoint);
    pBeforeNode->pNext = insertNode;
    insertNode->pNext = NULL;
    return 0;
}

/**
*根据定制规则，合并相邻starttime在1小时内，servicecode相同的结点
*/
int reOrganizePointList(s_eachpoint* pFirstNode)
{
    if(!pFirstNode)
        return -1;
    s_eachpoint* pNode = pFirstNode->pNext;
    s_eachpoint* pNextNode = NULL;
    while(pNode)
    {
        pNextNode = pNode->pNext;
        if(!pNextNode)
            return 0;
        //printf("next stime=%llu, current stime=%llu", pNextNode->starttime, pNode->starttime);
        if(pNextNode->starttime - pNode->starttime < 3600)
        {
            if(strcmp(pNextNode->servicecode, pNode->servicecode) == 0)
            {
            	//printf(",合并\n");
				if(pNextNode->starttime>pNode->endtime)
                	pNode->endtime = pNextNode->starttime;
                pNode->pNext = pNextNode->pNext;
                free(pNextNode);
                continue;
            }else{
				//printf(",场所切换不合并\n");
				//printf("my etime=%llu, next stime=%llu\n", pNode->endtime, pNextNode->starttime);
				pNode->endtime = pNextNode->starttime;
				
			}
        }
        else
        {
            //printf(",不合并\n");
			if(pNode->endtime>pNextNode->starttime)
			{
				//printf("but etime=%llu, next stime=%llu\n", pNode->endtime, pNextNode->starttime);
				pNode->endtime = pNextNode->starttime;				
			}
        }
        pNode = pNode->pNext;
    }
}

/**
* 展示列表
*/


/**
* 取列表长度
*/
ulong getPointListLength(s_eachpoint* pFirstNode)
{
	if(!pFirstNode)
       return -1;
	s_eachpoint* pNode = pFirstNode->pNext;
	ulong lLen = 0;
	while(pNode){
		lLen++;
		pNode = pNode->pNext;
	}
	return lLen;
}




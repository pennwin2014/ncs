#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utoall.h"
#include "pasdef.h"
#include "ncmac.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "utoall.h"
#include "pasdb.h"
#include "ncsdef.h"
#include "pasutl.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <utoall.h>
#include "ncsdef.h"
#include "pasdb.h"



struct s_nctermtask
{
    uint4 cid;
    uint4 groupid;
    uint8 starttime;
    uint8 endtime;
    char dids[255];
    char mac[20];
    uint4 maccount;
    uint4 servcount;
};

typedef struct infonode
{
    char mac[19];
    char servicecode[17];
    char servicename[128];
    char address[128];//     char(128)
    uint8 stime;//       bigint
    int vtype;//       int(1)
    char vname[64];//       char(64)
    char ssid[255];//        char(255)
    char apname[21];//      char(21)
    char apmac[17];//       char(17)
    char longitude[11];//   char(11)
    char latitude[11];//    char(11)
    struct infonode* pNext;
} s_infonode;

typedef struct timenode
{
    char timestr[17];
    long count;
    s_infonode* pInfoList;
    struct timenode* pNext;
} s_timenode;
typedef struct macnode
{
    char mac[19];
    long count;
    struct macnode* pNext;
} s_macnode;

struct s_uniformtime
{
    char servicecode[14];
    s_timenode* pTimeList;//时间列表的头指针
    s_macnode* pMacList;//mac列表的头指针
};

typedef struct
{
    long appearCount;
    s_infonode* pInfoList;
} s_serviceappear;
static int getTimeListByNodeList(s_timenode* pnode, char* timelist)
{
    s_timenode* ptmp = pnode;
    if(ptmp)
    {
        sprintf(timelist, "'%s'", ptmp->timestr);
    }
    while(ptmp->pNext)
    {
        ptmp = ptmp->pNext;
        sprintf(timelist + strlen(timelist), ",'%s'", ptmp->timestr);
    }
    printf("timelist=[%s]\n", timelist);
    return 0;
}

static int isMacInList(s_macnode* pnode, char* mac)
{
    s_macnode* ptmp = pnode;
    while(ptmp)
    {
        if(strcmp(ptmp->mac, mac) == 0)
        {
            return 1;
        }
        ptmp = ptmp->pNext;
    }
    return 0;
}


static int findAndIncMac(s_macnode* pnode, char* mac)
{
    s_macnode* ptmp = pnode;
    while(ptmp)
    {
        if(strcmp(ptmp->mac, mac) == 0)
        {
            ptmp->count++;
            //printf("find mac=%s,count=%d\n", mac, ptmp->count);
            return 1;
        }
        ptmp = ptmp->pNext;
    }
    return 0;
}

//新增一个mac
//所以macList的头指针不能用
static int insertMac(s_macnode* pnode, char* mac)
{
    s_macnode* ptmp = pnode;
    s_macnode* pLast = pnode;
    while(ptmp)
    {
        pLast = ptmp;
        ptmp = ptmp->pNext;
    }
    ptmp = (s_macnode*)malloc(sizeof(s_macnode));
    ptmp->count = 1;
    strcpy(ptmp->mac, mac);
    //printf("recieve a mac[%s] from outside\n", mac);
    ptmp->pNext = NULL;
    pLast->pNext = ptmp;
    return 0;
}

static s_infonode* getDetailByMac(char* mac, s_timenode* ptimenode)
{
    s_timenode* pTimeNode = NULL;
    s_infonode* pInfoNode = NULL;
    if(ptimenode)
    {
        pTimeNode = ptimenode->pNext;
        pInfoNode = ptimenode->pInfoList;
    }

    while(pTimeNode)
    {
        pInfoNode = pTimeNode->pInfoList->pNext;
        while(pInfoNode)
        {
            if(strcmp(mac, pInfoNode->mac) == 0)
            {
                printf("get matched infonode ,mac=[%s]->[%s]\n", mac, pInfoNode->mac);
                return pInfoNode;
            }
            pInfoNode = pInfoNode->pNext;
        }
        pTimeNode = pTimeNode->pNext;
    }
    printf("did not get matched infonode ,mac=%s\n", mac);
    return NULL;
}

static int isAlreadyInAppearList(s_serviceappear* pAppearCount, long sumCount, char* mac)
{
	long i = 0;
	s_infonode* ptmp =NULL;
	for(i=0;i<sumCount;i++)
	{
		ptmp = pAppearCount[i].pInfoList;
	    while(ptmp)
	    {
	        if(strcmp(mac, ptmp->mac) == 0)
	        {
	            return 1;
	        }
	        ptmp = ptmp->pNext;
	    }
	}
    return 0;
}


static int insertMacToInfoList(s_infonode* pnode, char* mac, s_infonode* pInfo)
{
    s_infonode* ptmp = pnode;
    s_infonode* pLast = pnode;

    while(ptmp)
    {
        pLast = ptmp;
        ptmp = ptmp->pNext;
    }
    ptmp = (s_infonode*)malloc(sizeof(s_infonode));
    strcpy(ptmp->mac, mac);
    if(pInfo)
    {
        strcpy(ptmp->servicecode, pInfo->servicecode);
        strcpy(ptmp->servicename, pInfo->servicename);
        strcpy(ptmp->address, pInfo->address);
        ptmp->stime = pInfo->stime;
        ptmp->vtype = pInfo->vtype;
        strcpy(ptmp->vname, pInfo->vname);
        strcpy(ptmp->ssid, pInfo->ssid);
        strcpy(ptmp->apname, pInfo->apname);
        strcpy(ptmp->apmac, pInfo->apmac);
        strcpy(ptmp->longitude, pInfo->longitude);
        strcpy(ptmp->latitude, pInfo->latitude);
    }
    ptmp->pNext = NULL;
    pLast->pNext = ptmp;
    return 0;
}


static int insertMacToList(s_macnode* pnode, char* mac)
{
    /*如果没有就新建一个，如果有就count+1*/
    //printf("insert mac[%s] to maclist\n", mac);
    if(!findAndIncMac(pnode, mac))
    {
        //printf("did not find mac=%s\n",mac);
        insertMac(pnode, mac);
    }

    return 0;
}

static int deleteAllMacInfoByMac(s_infonode* totalList, char* mac)
{
	s_infonode* pnode = totalList->pNext;
	s_infonode* pBeforeNode = totalList;
	while(pnode)
	{		
		if(strcmp(pnode->mac, mac) == 0)
		{
			pBeforeNode->pNext = pnode->pNext;
			free(pnode);
			pnode = pBeforeNode->pNext;
		}	
		pBeforeNode = pnode;
		if(pnode)
			pnode=pnode->pNext;
	}
	return 0;
}

static int deleteMacUnmatch(s_macnode* pMacNode, struct s_nctermtask* psTask, s_infonode* totalMacList)
{
    //首指针为0不算
    s_macnode* pbefore = pMacNode;
    s_macnode* ptmp = NULL;
    if(pMacNode)
    {
        ptmp = pMacNode->pNext;
    }

    while(ptmp)
    {
        if(ptmp->count < psTask->maccount)
        {
            //删除这个节点
            pbefore->pNext = ptmp->pNext;
            printf("delete a mac=[%s],because count=%lu, and maccount=%lu \n", ptmp->mac, ptmp->count, psTask->maccount);
            free(ptmp);
            ptmp = pbefore;
			//删除掉totalMacList中所有mac等于这个mac的数据
			deleteAllMacInfoByMac(totalMacList, ptmp->mac);
        }
        else
        {
            printf("keep a mac=[%s],because count=%lu, and maccount=%lu \n", ptmp->mac, ptmp->count, psTask->maccount);
        }
        pbefore = ptmp;
        ptmp = ptmp->pNext;
    }
    return 0;
}

static int insertToDB(s_infonode* pInfoNode, struct s_nctermtask* psTask)
{
    char sql[2048] = "";
    if(((strlen(pInfoNode->mac) > 0) && (strlen(pInfoNode->servicecode) > 0)))
    {
    	memset(sql, 0, sizeof(sql));
		printf("%lu,'%s','%s','%s','%s',%llu,%d,'%s','%s','%s','%s','%s','%s'\n", psTask->cid, pInfoNode->mac, pInfoNode->servicecode, pInfoNode->servicename,
                pInfoNode->address, pInfoNode->stime, pInfoNode->vtype, pInfoNode->vname, pInfoNode->ssid,
                pInfoNode->apname, pInfoNode->apmac, pInfoNode->longitude, pInfoNode->latitude);
        sprintf(sql, "insert into nctermtasklog(cid,mac,servicecode,servicename,address,stime,vtype,vname,ssid,apname,apmac,longitude,latitude) \
        	 values(%lu,'%s','%s','%s','%s',%llu,%d,'%s','%s','%s','%s','%s','%s') ",
                psTask->cid, pInfoNode->mac, pInfoNode->servicecode, pInfoNode->servicename,
                pInfoNode->address, pInfoNode->stime, pInfoNode->vtype, pInfoNode->vname, pInfoNode->ssid,
                pInfoNode->apname, pInfoNode->apmac, pInfoNode->longitude, pInfoNode->latitude);
        int iReturn = pasDbExecSqlF(sql);
        printf("insert db sql = %s\n", sql);
    }
    return 0;
}

static int deleteSameRule(uint4 cid)
{
    char sql[1024] = "";
    sprintf(sql, "delete from nctermtasklog where cid=%lu", cid);
    printf("delete db sql = %s\n", sql);
    return pasDbExecSqlF(sql);
}

static int updateReportTime(uint4 cid)
{
    char sql[1024] = "";
    sprintf(sql, "update nctermtask set reoporttime=%lu where cid=%lu", time(0), cid);
    printf("update db sql = %s\n", sql);
    return pasDbExecSqlF(sql);
}


static char *getServicecodeByDids(utShmHead *psShmHead, char *caGroupids)
{
    static char caReturn[8024];
    char caAllGroupIds[8024];
    char sql[8024];
    char caServicecode[32];
    int iReturn, lId, iNum;
    char caGroupId[12] = {0};
    pasDbCursor *psCur;
    memset(caReturn, 0, sizeof(caReturn));
    memset(caAllGroupIds, 0, sizeof(caAllGroupIds));
    strcpy(caAllGroupIds, caGroupids);
    int isBreak = 0;

    for(iNum = 0; iNum < 3; iNum++)
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select groupid from ncsgroup where groupid in (%s) or pid in (%s)", caAllGroupIds, caAllGroupIds);
        printf("iNum=%d,sql=[%s],groupids=[%s]\n", iNum, sql, caAllGroupIds);
        /*
        //放到sql以后就可以清空
        memset(caAllGroupIds, 0, sizeof(caAllGroupIds));
        */
        psCur = pasDbOpenSqlF(sql);
        if(psCur)
        {
            iReturn = 0;
            memset(caGroupId, 0, sizeof(caGroupId));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 11, caGroupId);
            while(iReturn == 0)
            {
                //不断壮大的groupid列表
                if(strlen(caAllGroupIds) == 0)
                {
                    sprintf(caAllGroupIds, "%s", caGroupId);
                }
                else
                {
                    if(!strstr(caAllGroupIds, caGroupId))
                        sprintf(caAllGroupIds + strlen(caAllGroupIds), ",%s", caGroupId);
                }
                memset(caGroupId, 0, sizeof(caGroupId));
                iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 11, caGroupId);
            }
        }
        pasDbCloseCursor(psCur);
    }

    iNum = 0;
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select username from ncsuser a,ncsgroup b where a.groupid=b.groupid and (b.groupid in (%s) or b.pid in(%s)) limit 0,300 ", caAllGroupIds, caAllGroupIds);
    printf("get servicecode list sql=[%s]\n", sql);
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        iReturn = 0;
        memset(caServicecode, 0, sizeof(caServicecode));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 16, caServicecode);
        while(iReturn == 0)
        {
            //最终结果
            if(iNum == 0)
            {
                sprintf(caReturn, "\'%s\'", caServicecode);
            }
            else
            {
                sprintf(caReturn + strlen(caReturn), ",\'%s\'", caServicecode);
            }
            memset(caServicecode, 0, sizeof(caServicecode));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 16, caServicecode);
            iNum++;
        }
    }
    pasDbCloseCursor(psCur);

    return &caReturn[0];
}

static int copyNode(s_infonode* toNode, s_infonode* fromNode)
{
	memcpy(toNode, fromNode, sizeof(s_infonode));
	return 0;
}

static int insertMacTotalList(s_infonode* pnode, s_infonode* pInfo)
{
	s_infonode* ptmp = pnode;
    s_infonode* pLast = pnode;
    while(ptmp)
    {
        pLast = ptmp;
        ptmp = ptmp->pNext;
    }
    ptmp = (s_infonode*)malloc(sizeof(s_infonode));
    memset(ptmp, 0, sizeof(s_infonode));
	copyNode(ptmp, pInfo);
    ptmp->pNext = NULL;
    pLast->pNext = ptmp;
}

static int insertTimeToTotalTimeList(s_timenode* pnode, char* timestr)
{
	//如果重复的就不放
	s_timenode* pLast = pnode;
	s_timenode* pTmp = pnode;
	while(pTmp)
	{
		if(strcmp(pTmp->timestr, timestr)==0)
		{
			pTmp->count++;
			return 1;
		}
		pLast = pTmp;
		pTmp=pTmp->pNext;
	}
	s_timenode* newTime = (s_timenode*)malloc(sizeof(s_timenode));
	pLast->pNext = newTime;
	memset(newTime, 0, sizeof(s_timenode));
	strcpy(newTime->timestr, timestr);
	newTime->pNext = NULL;
	return 0;
}

static int isInSameTime(uint8 lTime, char* sTime)
{
	char tempTime[11] = {0};
	sprintf(tempTime, "%s", utTimFormat("%Y%m%d%H", lTime));
	if(strcmp(tempTime, sTime) == 0)
		return 1;
	else
		return 0;
}


static long getMacCountByTime(s_infonode* pTotalMacList, char* timestr, s_infonode* ptempNode)
{
	long macCount = 0;
	s_infonode* ptemp = pTotalMacList->pNext;
	s_infonode* pLastTemp = ptempNode;
	s_infonode* pTempTemp = ptempNode;
	while(ptemp)
	{
		if(isInSameTime(ptemp->stime, timestr))
		{
			//顺便将该node添加到临时列表以方便最后的插入数据库
			while(pTempTemp)
			{
				pLastTemp = pTempTemp;
				pTempTemp=pTempTemp->pNext;
			}
			s_infonode* newTemp = (s_infonode*)malloc(sizeof(s_infonode));
			pLastTemp->pNext = newTemp;
			memset(newTemp, 0, sizeof(s_infonode));
			copyNode(newTemp, ptemp);
			newTemp->pNext = NULL;
			macCount++;
		}
		ptemp = ptemp->pNext;
	}
	return macCount;
}

static int deleteAllMacByTime(s_infonode* pnode, char* timestr)
{
	s_infonode* pBefore = pnode;
	s_infonode* pTemp = pnode->pNext;
	while(pTemp)
	{
		pBefore = pTemp;
		if(isInSameTime(pTemp->stime, timestr))
		{
			//删掉该结点
			pBefore->pNext = pTemp->pNext;
			free(pTemp);
			pTemp = pBefore;
		}
		pTemp = pTemp->pNext;
	}
	return 0;
}

static int insertAllTempMacToDB(s_infonode* pnode, struct s_nctermtask* psTask)
{
	s_infonode* pTemp = pnode->pNext;
	while(pTemp)
	{
		insertToDB(pTemp, psTask);
		pTemp = pTemp->pNext;
	}
	return 0;
}




static int ncsStatMacBabEx(utShmHead *psShmHead)
{
    pasDbCursor *psCur;
    char sql[2048] = "";
    long lSumTask = 0, lSumService = 0, lMacCount = 0, lAppearCount = 0;
    unsigned char* pHash_task = NULL;
    unsigned char* pHash_utime = NULL; //归一化后的时间
    char** pServicecode = NULL;
    struct s_nctermtask tmpTask;
    struct s_nctermtask* psTask = NULL;
    struct s_uniformtime* psUtime = NULL;
    struct s_uniformtime* psUtime2 = NULL;
    s_timenode* pTimeNode = NULL;
    s_timenode* pTimeNodeBefore = NULL;
    s_macnode* pMacNode = NULL;
    s_macnode* pMacNodeBefore = NULL;
    s_infonode* pInfoNode = NULL;
    s_infonode* pInfoNodeBefore = NULL;
    pasLHashInfo sHashInfo;
    char caServerlist[8024] = "";
    char caTimeList[2048] = "";
    char caMac[19] = "";
    char caUniformTime[17] = "";
    long iNum = 0, i = 0, j = 0;
    long totalAppearTimes = 0;
    /*以下为一些结构体赋值用的临时变量*/
    char caServicecode[17] = "";
    char caServicename[128] = "";
    char caAddress[128] = "";
    char caVname[64] = "";
    long lStime = 0, lVtype = 0;
    long taskCount = 0;
    char caSsid[255] = "";
    char caApname[21] = "";
    char caApmac[17] = "";
    char caLongitude[11] = "";
    char caLatitude[11] = "";
    int iReturn = pasConnect(psShmHead);
    printf("iReturn=%d\n", iReturn);
    if(iReturn < 0)
    {
        sleep(60);
        return 0;
    }
    while(1)
    {
        //1、统计总共有多少条规则
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select count(cid) from nctermtask where length(mac)>0");
        printf("sql=%s\n", sql);
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lSumTask);
        printf("iReturn=%d,lSumTask=%d\n", iReturn, lSumTask);
        if(iReturn != 0 || lSumTask == 0)
        {
            return 0;
        }

        //2、取出所有规则的明细信息装入内存
        pHash_task = (unsigned char *)pasLHashInit(5000, 5000, sizeof(struct s_nctermtask), 0, 4);
        if(pHash_task == NULL)
        {
            printf("分配内存出错\n");
            return 0;
        }
        else
        {
            printf("pHash_task is not null\n");
        }
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select cid,groupid,starttime,endtime,dids,mac,maccount,servcount from nctermtask");
        psCur = pasDbOpenSql(sql, 0);
        if(psCur)
        {
            memset(&tmpTask, 0, sizeof(tmpTask));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG, 4, &tmpTask.cid,
                                     UT_TYPE_LONG, 4, &tmpTask.groupid,
                                     UT_TYPE_LONG8, 8, &tmpTask.starttime,
                                     UT_TYPE_LONG8, 8, &tmpTask.endtime,
                                     UT_TYPE_STRING, 250, tmpTask.dids,
                                     UT_TYPE_STRING, 18, tmpTask.mac,
                                     UT_TYPE_LONG, 4, &tmpTask.maccount,
                                     UT_TYPE_LONG, 4, &tmpTask.servcount);
            printf("first sql=[%s],fetchinto ret=%d\n", sql, iReturn);
            while((iReturn == 0) || (iReturn == 1405))
            {
                printf("fetch into ret=%d,cid=%d,mac=%s\n", iReturn, tmpTask.cid, tmpTask.mac);
                psTask = (struct s_nctermtask *)pasLHashLookA(pHash_task, (char*)&tmpTask.cid);
                if(psTask)
                {
                    psTask->cid = tmpTask.cid;
                    psTask->groupid = tmpTask.groupid;
                    psTask->starttime = tmpTask.starttime;
                    psTask->endtime = tmpTask.endtime;
                    strcpy(psTask->dids, tmpTask.dids);
                    strcpy(psTask->mac, tmpTask.mac);
                    psTask->maccount = tmpTask.maccount;
                    psTask->servcount = tmpTask.servcount;
                }
                memset(&tmpTask, 0, sizeof(tmpTask));
                iReturn = pasDbFetchInto(psCur,
                                         UT_TYPE_LONG, 4, &tmpTask.cid,
                                         UT_TYPE_LONG, 4, &tmpTask.groupid,
                                         UT_TYPE_LONG8, 8, &tmpTask.starttime,
                                         UT_TYPE_LONG8, 8, &tmpTask.endtime,
                                         UT_TYPE_STRING, 250, tmpTask.dids,
                                         UT_TYPE_STRING, 18, tmpTask.mac,
                                         UT_TYPE_LONG, 4, &tmpTask.maccount,
                                         UT_TYPE_LONG, 4, &tmpTask.servcount);
            }
            pasDbCloseCursor(psCur);
        }
        else
        {
            printf("sql=[%s],cur is null\n", sql);
        }
        psTask = (struct s_nctermtask*)pasLHashFirst(pHash_task, &sHashInfo);
        taskCount = 0;
        while(psTask)
        {
            //2.5、以下所有查询都基于dids,starttime和endtime为前提条件
            memset(caServerlist, 0, sizeof(caServerlist));
            // DO:根据dids获取servicecode的列表
            strcpy(caServerlist, getServicecodeByDids(psShmHead, psTask->dids));
            if(strlen(caServerlist) == 0)
            {
                psTask = (struct s_nctermtask*)pasLHashNext(&sHashInfo);
                printf("===================dids didn't get servicecode list ,pass %d task, total %d======= \n", (++taskCount), lSumTask);
                sleep(10);
                continue;
            }
            printf("finish get servicecode list %s\n", caServerlist);
            //3、获取该mac在该时间段所经历的场所列表(N个)
            lSumService = 0;
            memset(sql, 0, sizeof(sql));
            //sprintf(sql, "select count(*) from (select distinct servicecode from  ncmactermatt_if_201508 where mac='%s' and stime>%llu and stime<%llu and servicecode in (%s)) as aa", psTask->mac, psTask->starttime, psTask->endtime, caServerlist);
            sprintf(sql, "select count(*) from (select distinct servicecode from  ncmactermatt_if_%s where mac='%s' and stime>%llu and stime<%llu) as aa", utTimFormat("%Y%m", time(0)), psTask->mac, psTask->starttime, psTask->endtime);
            printf("do sql=[%s]\n", sql);
            iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lSumService);
            printf("ret=%d,lSumService=%d\n", iReturn, lSumService);
            if(iReturn != 0 || lSumTask == 0)
            {
                return 0;
            }
            pServicecode = (char **)malloc(sizeof(char *) * lSumService);
            memset(sql, 0, sizeof(sql));
            //sprintf(sql, "select distinct servicecode from  ncmactermatt_if_201508 where mac='%s' and stime>%llu and stime<%llu and servicecode in (%s)", psTask->mac, psTask->starttime, psTask->endtime, caServerlist);
            sprintf(sql, "select distinct servicecode from  ncmactermatt_if_%s where mac='%s' and stime>%llu and stime<%llu", utTimFormat("%Y%m", time(0)), psTask->mac, psTask->starttime, psTask->endtime);
            printf("do get servicecodes sql=%s\n", sql);
            psCur = pasDbOpenSql(sql, 0);
            if(psCur)
            {
                iNum = 0;
                pServicecode[iNum] = (char *)malloc(20);
                memset(pServicecode[iNum], 0, 20);
                iReturn = pasDbFetchInto(psCur,
                                         UT_TYPE_STRING, 14, pServicecode[iNum]);
                printf("first ret=%d,pServicecode=%s,iNum=%d,lSumService=%d\n", iReturn, pServicecode[iNum], iNum, lSumService);
                while((iReturn == 0) || (iReturn == 1405))
                {
                    iNum++;
                    if(iNum >= lSumService)
                        break;
                    pServicecode[iNum] = (char *)malloc(20);
                    memset(pServicecode[iNum], 0, 20);
                    iReturn = pasDbFetchInto(psCur,
                                             UT_TYPE_STRING, 14, pServicecode[iNum]);
                    printf("fetch ret=%d,pServicecode=%s,iNum=%d,lSumService=%d\n", iReturn, pServicecode[iNum], iNum, lSumService);
                }
                pasDbCloseCursor(psCur);
            }
            else
            {
                printf("sql=%s,cur is null\n", sql);
            }
            printf("finish step 3\n\n");
            //4、归一化该mac在该些场所所经历的各个时间段并统计其次数(次数可不用)
            pHash_utime = (unsigned char *)pasLHashInit(5000, 5000, sizeof(struct s_uniformtime), 0, 14);
            if(pHash_utime == NULL)
            {
                printf("分配内存出错\n");
                return 0;
            }
            for(i = 0; i < lSumService; i++)
            {
                psUtime = (struct s_uniformtime*)pasLHashLookA(pHash_utime, pServicecode[i]);
                if(psUtime)
                {
                    strcpy(psUtime->servicecode, pServicecode[i]);
                    pTimeNode = (s_timenode*)malloc(sizeof(s_timenode));
                    memset(pTimeNode, 0, sizeof(s_timenode));
                    //为下一个mac简单列表的指针分配内存
                    pTimeNode->pInfoList = (s_infonode*)malloc(sizeof(s_infonode));
                    memset(pTimeNode->pInfoList, 0, sizeof(s_infonode));
                    pTimeNode->pInfoList->pNext = NULL;
                    //下一节点的指针置空
                    pTimeNode->pNext = NULL;
                    psUtime->pTimeList = pTimeNode;
                    //同时也创建一个空的macList
                    pMacNode = (s_macnode*)malloc(sizeof(s_macnode));
                    memset(pMacNode, 0, sizeof(s_macnode));
                    psUtime->pMacList = pMacNode;
                    memset(sql, 0, sizeof(sql));
                    sprintf(sql, "select date_format(from_unixtime(stime),'%%Y%%m%%d%%H'),count(mac) from ncmactermatt_if_%s where servicecode='%s' and mac='%s' and stime>%llu and stime<%llu group by date_format(from_unixtime(stime),'%%Y%%m%%d%%H')",
                            utTimFormat("%Y%m", time(0)), pServicecode[i], psTask->mac, psTask->starttime, psTask->endtime);
                    printf("first get timestr sql=[%s]\n", sql);
                    psCur = pasDbOpenSql(sql, 0);
                    if(psCur)
                    {
                        memset(caUniformTime, 0, sizeof(caUniformTime));
                        lMacCount = 0;
                        iReturn = pasDbFetchInto(psCur,
                                                 UT_TYPE_STRING, 16, caUniformTime,
                                                 UT_TYPE_LONG, 4, &lMacCount);
                        while((iReturn == 0) || (iReturn == 1405))
                        {
                            printf("fetch ret=%d,timestr=%s\n", iReturn, caUniformTime);
                            pTimeNode->count = lMacCount;
                            strcpy(pTimeNode->timestr, caUniformTime);
                            pTimeNodeBefore = pTimeNode;
                            pTimeNode = (s_timenode*)malloc(sizeof(s_timenode));
                            pTimeNodeBefore->pNext = pTimeNode;
                            memset(pTimeNode, 0, sizeof(s_timenode));
                            //为下一个明细列表的指针分配内存
                            pTimeNode->pInfoList = (s_infonode*)malloc(sizeof(s_infonode));
                            memset(pTimeNode->pInfoList, 0, sizeof(s_infonode));
                            pTimeNode->pInfoList->pNext = NULL;
                            //下一节点的指针置空
                            pTimeNode->pNext = NULL;
                            memset(caUniformTime, 0, sizeof(caUniformTime));
                            lMacCount = 0;
                            iReturn = pasDbFetchInto(psCur,
                                                     UT_TYPE_STRING, 16, caUniformTime,
                                                     UT_TYPE_LONG, 4, &lMacCount);
                        }
                        pasDbCloseCursor(psCur);
                    }
                    else
                    {
                        printf("sql=[%s],cur is null\n", sql);
                    }
                }
            }
            printf("finish step 4 \n\n");
            //5、为每个servicecode下的每个时间点的maclist填上所有出现过的mac
            //5.5、统计5里面的所有mac
            //总mac列表的头指针
            s_infonode* totalMacList = (s_infonode*)malloc(sizeof(s_infonode));
			memset(totalMacList, 0, sizeof(s_infonode));
			s_infonode* pTotalMacList = totalMacList;
			//总的时间的列表
			s_timenode* totalTimeList = (s_timenode*)malloc(sizeof(s_timenode));
			memset(totalTimeList, 0, sizeof(s_timenode));
			s_timenode* pTotalTimeList = totalTimeList;
			if(lSumService>0)
			{
				psUtime = (struct s_uniformtime*)pasLHashLookA(pHash_utime, pServicecode[0]);
				pMacNode = psUtime->pMacList;//永远用这个作为存放mac详细信息的列表
			}			
            for(i = 0; i < lSumService; i++)
            {
                psUtime = (struct s_uniformtime*)pasLHashLookA(pHash_utime, pServicecode[i]);
                if(psUtime)
                {
                    pTimeNode = psUtime->pTimeList;                    
                    while(pTimeNode)
                    {
                        if(strlen(pTimeNode->timestr) > 0)
                        {
                        	insertTimeToTotalTimeList(totalTimeList, pTimeNode->timestr);
                            memset(sql, 0, sizeof(sql));
                            sprintf(sql, "select mac,servicecode,servicename,address,vname,stime,vtype,ssid,apname,apmac,longitude,latitude from ncmactermatt_if_%s where servicecode='%s' and mac!='%s' and stime>%llu and stime<%llu and date_format(from_unixtime(stime),'%%Y%%m%%d%%H')='%s' group by mac",
                                    utTimFormat("%Y%m", time(0)), pServicecode[i], psTask->mac, psTask->starttime, psTask->endtime, pTimeNode->timestr);
                            printf("Count mac appear in time=[%s]\n sql=%s\n", pTimeNode->timestr, sql);
                            psCur = pasDbOpenSql(sql, 0);
                            if(psCur)
                            {
                                memset(caMac, 0, sizeof(caMac));
                                memset(caServicecode, 0, sizeof(caServicecode));
                                memset(caServicename, 0, sizeof(caServicename));
                                memset(caAddress, 0, sizeof(caAddress));
                                memset(caVname, 0, sizeof(caVname));
                                memset(caSsid, 0, sizeof(caSsid));
                                memset(caApname, 0, sizeof(caApname));
                                memset(caApmac, 0, sizeof(caApmac));
                                memset(caLongitude, 0, sizeof(caLongitude));
                                memset(caLatitude, 0, sizeof(caLatitude));
                                lStime = 0;
                                lVtype = 0;

                                iReturn = pasDbFetchInto(psCur,
                                                         UT_TYPE_STRING, 18, caMac,
                                                         UT_TYPE_STRING, 14, caServicecode,
                                                         UT_TYPE_STRING, 125, caServicename,
                                                         UT_TYPE_STRING, 125, caAddress,
                                                         UT_TYPE_STRING, 63, caVname,
                                                         UT_TYPE_LONG8, 8, &lStime,
                                                         UT_TYPE_LONG, 4, &lVtype,
                                                         UT_TYPE_STRING, 250, caSsid,
                                                         UT_TYPE_STRING, 20, caApname,
                                                         UT_TYPE_STRING, 16, caApmac,
                                                         UT_TYPE_STRING, 10, caLongitude,
                                                         UT_TYPE_STRING, 10, caLatitude);
                                pInfoNode = pTimeNode->pInfoList;
                                while((iReturn == 0) || (iReturn == 1405))
                                {
                                    strcpy(pInfoNode->mac, caMac);
                                    // DO: 将该mac对应的其他信息保存到内存
                                    strcpy(pInfoNode->servicecode, caServicecode);
                                    strcpy(pInfoNode->servicename, caServicename);
                                    strcpy(pInfoNode->address, caAddress);
                                    pInfoNode->stime = lStime;
                                    pInfoNode->vtype = lVtype;
                                    strcpy(pInfoNode->vname, caVname);
                                    strcpy(pInfoNode->ssid, caSsid);
                                    strcpy(pInfoNode->apname, caApname);
                                    strcpy(pInfoNode->apmac, caApmac);
                                    strcpy(pInfoNode->longitude, caLongitude);
                                    strcpy(pInfoNode->latitude, caLatitude);
                                    //随时记录并统计该mac
                                    insertMacToList(pMacNode, caMac);
									insertMacTotalList(totalMacList, pInfoNode);									
									//创建下一个节点
                                    pInfoNodeBefore = pInfoNode;
                                    pInfoNode = (s_infonode*)malloc(sizeof(s_infonode));
                                    pInfoNodeBefore->pNext = pInfoNode;
                                    memset(pInfoNode, 0, sizeof(s_infonode));
                                    memset(caMac, 0, sizeof(caMac));
                                    memset(caServicecode, 0, sizeof(caServicecode));
                                    memset(caServicename, 0, sizeof(caServicename));
                                    memset(caAddress, 0, sizeof(caAddress));
                                    memset(caVname, 0, sizeof(caVname));
                                    memset(caSsid, 0, sizeof(caSsid));
                                    memset(caApname, 0, sizeof(caApname));
                                    memset(caApmac, 0, sizeof(caApmac));
                                    memset(caLongitude, 0, sizeof(caLongitude));
                                    memset(caLatitude, 0, sizeof(caLatitude));
                                    lStime = 0;
                                    lVtype = 0;
                                    iReturn = pasDbFetchInto(psCur,
                                                             UT_TYPE_STRING, 18, caMac,
                                                             UT_TYPE_STRING, 14, caServicecode,
                                                             UT_TYPE_STRING, 125, caServicename,
                                                             UT_TYPE_STRING, 125, caAddress,
                                                             UT_TYPE_STRING, 63, caVname,
                                                             UT_TYPE_LONG8, 8, &lStime,
                                                             UT_TYPE_LONG, 4, &lVtype,
                                                             UT_TYPE_STRING, 250, caSsid,
                                                             UT_TYPE_STRING, 20, caApname,
                                                             UT_TYPE_STRING, 16, caApmac,
                                                             UT_TYPE_STRING, 10, caLongitude,
                                                             UT_TYPE_STRING, 10, caLatitude);
                                }
                                pasDbCloseCursor(psCur);
                            }
                        }
                        pTimeNode = pTimeNode->pNext;
                    }
                    //6、剔除掉列表里出现时间点个数小于maccount的数据
                    //deleteMacUnmatch(pMacNode, psTask);
                }
            }
			deleteMacUnmatch(pMacNode, psTask, totalMacList);
            printf("finish step 5~6\n\n");
            if(lSumService > 0)
            {
				//7、遍历时间总列表，统计mac总列表中，
				//在该时间点出现的所有不同mac的种数,把种数小于svrcount的剔除
				//7.5、遍历mac总列表，把所有在该时间出现的mac都放进临时列表
				//如果满足条件就把该临时列表的东西存到数据，否则释放临时列表
				pTotalTimeList = totalTimeList->pNext;
				long lDiffMacCount = 0;
				s_infonode* tempInfoList = (s_infonode*)malloc(sizeof(s_infonode));
				memset(tempInfoList, 0, sizeof(s_infonode));
				s_infonode* pTempInfoList = tempInfoList;
				s_infonode* pTempBeforeInfoList = NULL;
				deleteSameRule(psTask->cid);
				while(pTotalTimeList)
				{
					lDiffMacCount = getMacCountByTime(totalMacList, pTotalTimeList->timestr, tempInfoList);
					if(lDiffMacCount<psTask->servcount)
						deleteAllMacByTime(totalMacList, pTotalTimeList->timestr);
					else
						insertAllTempMacToDB(tempInfoList, psTask);
				    //目的是头指针不释放
					pTempInfoList = tempInfoList->pNext;
					//tempInfoList->pNext = NULL;
					while(pTempInfoList)
					{
						free(pTempInfoList);
						pTempInfoList = pTempInfoList->pNext;						
					}
					tempInfoList->pNext=NULL;
					pTotalTimeList=pTotalTimeList->pNext;
				}
				updateReportTime(psTask->cid);
				//最后再释放头指针
				free(tempInfoList);
            }
            else
            {
                printf("lSumService=0, don't need step 7 above\n");
            }

            //10、释放内存
            for(i = 0; i < lSumService; i++)
            {
                psUtime = (struct s_uniformtime*)pasLHashLookA(pHash_utime, pServicecode[i]);
                if(psUtime)
                {
                    pMacNode = psUtime->pMacList;
                    while(pMacNode)
                    {
                        pMacNodeBefore = pMacNode;
                        pMacNode = pMacNode->pNext;
                        free(pMacNodeBefore);
                    }
                    pTimeNode = psUtime->pTimeList;
                    while(pTimeNode)
                    {
                        pInfoNode = pTimeNode->pInfoList;
                        while(pInfoNode)
                        {
                            pInfoNodeBefore = pInfoNode;
                            pInfoNode = pInfoNode->pNext;
                            free(pInfoNodeBefore);
                        }
                        pTimeNodeBefore = pTimeNode;
                        pTimeNode = pTimeNode->pNext;
                        free(pTimeNodeBefore);
                    }
                }
                free(pServicecode[i]);
            }
            if(pServicecode)
                free(pServicecode);
            if(pHash_utime)
                free(pHash_utime);
			// DO: 释放totalMacList和totalTimeList
			//把头指针也一起释放
			pTotalMacList = totalMacList;
			s_infonode* pTotalBeforeMacList = pTotalMacList;
			while(pTotalMacList)
			{
				pTotalBeforeMacList = pTotalMacList;
				pTotalMacList = pTotalMacList->pNext;
				free(pTotalBeforeMacList);
			}
			pTotalTimeList = totalTimeList;
			s_timenode* pTotalBeforeTimeList = pTotalTimeList;
			while(pTotalTimeList)
			{
				pTotalBeforeTimeList = pTotalTimeList;
				pTotalTimeList = pTotalTimeList->pNext;
				free(pTotalBeforeTimeList);
			}
            psTask = (struct s_nctermtask*)pasLHashNext(&sHashInfo);
            printf("====================finish %d task, total %d=================== \n", (++taskCount), lSumTask);
            sleep(10);
        }
        if(pHash_task)
            free(pHash_task);
        printf("\n================finish one round, will start in 60s...===========\n\n");
        sleep(60);
    }
}


int ncsStatMacDevWarn(utShmHead *psShmHead);


int main(int argc, char **argv)
{
    utShmHead  *psShmHead;
    char caTime[32], caThisTime[32];
    char argval;
    char caConfFile[64];
    int i, iReturn;
    unsigned long lTime;
    int iSum;
    printf("in function main");
    strcpy(caConfFile, "pqconf_ncs.cnf");
    /*
    argv++;
    argc--;
    while(argc) {
        if(**argv != '-') {
            printf("ncsshowmaclog  [-d configfile] \n");
            exit(0);
        }
        argval = *(*argv + 1);
        argc--;
        argv++;
        switch(argval) {
            case 'd':
                strcpy(caConfFile,(*argv));
                argc--;
                argv++;
                break;
            default:
                printf("ncicauth [-d configfile] \n");
                exit(0);
        }
    }*/
    psShmHead = utComShmOpen(caConfFile);
    if(psShmHead == NULL)
    {
        printf(" Share Memory not Init \n");
        exit(0);
    }
    else
    {
        printf("\nnot null\n");
    }
	//ncsStatMacDevWarn(psShmHead);
    ncsStatMacBabEx(psShmHead);
    utShmClose(psShmHead);
    exit(0);
}


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
#include "bab_list.h"

static int macPrint(long isDebug, const char* fmt, ...)
{
    int retCnt = 0;
    if(isDebug)
    {
        va_list ap;
        va_start(ap, fmt);
        retCnt = vprintf(fmt, ap);
        va_end(ap);
    }
    return retCnt;
}

static char *getServicecodesByDids(utShmHead *psShmHead, char *caGroupids)
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
    if(strlen(caAllGroupIds) == 0)
        strcpy(caAllGroupIds, "0");
    int isBreak = 0;

    for(iNum = 0; iNum < 3; iNum++)
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select groupid from ncsgroup where groupid in (%s) or pid in (%s)", caAllGroupIds, caAllGroupIds);
        //printf("iNum=%d,sql=[%s],groupids=[%s]\n", iNum, sql, caAllGroupIds);
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
#ifdef SHOWBAB_DEBUG
    printf("get servicecode list sql=[%s]\n", sql);
#endif
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


static int insertToDB(long lDebug, struct s_macinfo* pInfoNode)
{
    char sql[2048] = "";
    if(((strlen(pInfoNode->mac) > 0) && (strlen(pInfoNode->servicecode) > 0)))
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "insert into ncbabplacetimemac(cid,mac,servicecode,starttime,endtime) \
        	 values(%lu,'%s','%s',%llu,%llu) ", pInfoNode->cid,
                pInfoNode->mac, pInfoNode->servicecode, pInfoNode->starttime, pInfoNode->endtime);
        macPrint(lDebug, "insert db sql=%s", sql);
        int iReturn = pasDbExecSqlF(sql);
        macPrint(lDebug, "insert db sql = %s\n", sql);
    }
    return 0;
}

static int deleteSameRule(long lDebug, uint4 cid)
{
    char sql[1024] = "";
    sprintf(sql, "delete from ncbabplacetimemac where cid=%lu", cid);
    macPrint(lDebug, "delete db sql = %s\n", sql);
    return pasDbExecSqlF(sql);
}

static int updateReportTime(long lDebug, uint4 cid)
{
    char sql[1024] = "";
    sprintf(sql, "update ncbabtask set reoporttime=%lu where cid=%lu", time(0), cid);

    macPrint(lDebug, "update db sql = %s\n", sql);

    return pasDbExecSqlF(sql);
}

static void doDbSearchMacs(long lDebug, unsigned char* pHash_allMacs, s_eachpoint* pTmpNode)
{
    struct s_macinfo* psMacinfo = NULL;
    char sql[8023] = "";
    int iReturn = 0;
    pasDbCursor *psCur;
    char caMac[20] = "";
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select distinct(mac) from ncmactermatt_if_%s where servicecode='%s' and mac!='%s' and stime>%llu and stime<%llu",
            utTimFormat("%Y%m", time(0)), pTmpNode->servicecode, pTmpNode->mac, pTmpNode->starttime, pTmpNode->endtime);
    macPrint(lDebug, "get diff mac sql = %s\n", sql);
    psCur = pasDbOpenSql(sql, 0);
    if(psCur)
    {
        memset(caMac, 0, sizeof(caMac));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 17, caMac);
        while((iReturn == 0) || (iReturn == 1405))
        {

            psMacinfo = (struct s_macinfo*)pasLHashLookA(pHash_allMacs, (char*)&caMac);
            if(psMacinfo)
            {
                // DO: 拷贝数据
                memcpy(psMacinfo->mac, caMac, sizeof(psMacinfo->mac));
                memcpy(psMacinfo->servicecode, pTmpNode->servicecode, sizeof(psMacinfo->servicecode));
                psMacinfo->starttime = pTmpNode->starttime;
                psMacinfo->endtime =  pTmpNode->endtime;
                psMacinfo->cid = pTmpNode->cid;
            }
            memset(caMac, 0, sizeof(caMac));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 17, caMac);
        }
        pasDbCloseCursor(psCur);
    }
    else
    {
        macPrint(lDebug, "sql=[%s],cur is null\n", sql);
    }
}

static int doDbInsertMacs(long lDebug, unsigned char* pHash_allMacs)
{
    struct s_macinfo* psMacs = NULL;
    pasLHashInfo sHashInfo;
    psMacs = (struct s_macinfo*)pasLHashFirst(pHash_allMacs, &sHashInfo);
    while(psMacs)
    {
        insertToDB(lDebug, psMacs);
        psMacs = (struct s_macinfo*)pasLHashNext(&sHashInfo);
    }
    return 0;
}

static int ncsStatMacBab(utShmHead *psShmHead)
{
    pasDbCursor *psCur;
    char sql[2048] = "";
    long lSumTask = 0, taskCount = 0;
    unsigned char* pHash_task = NULL;
    unsigned char* pHash_allMacs = NULL;
    struct s_babtask tmpTask;
    s_eachpoint tmpPoint;
    struct s_babtask* psTask = NULL;
    pasLHashInfo sHashInfo;
    char caServerlist[8024] = "";
    long babDebug = utComGetVar_ld(psShmHead, "BabDebug", 0);
    int iReturn = pasConnect(psShmHead);
    if(iReturn < 0)
    {
        printf("conn psShmHead fail iReturn=%d\n", iReturn);
        sleep(60);
        return 0;
    }
    while(1)
    {
        //1、统计总共有多少条规则
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select count(cid) from ncbabtask where length(mac)>0");
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lSumTask);
        macPrint(babDebug, "iReturn=%d,lSumTask=%d\n", iReturn, lSumTask);
        if(iReturn != 0 || lSumTask == 0)
        {
            macPrint(babDebug, "不进入后面过程，休眠一段时间后继续\n\n");
            sleep(3 * 60);
            continue;
            //return 0;
        }
        //2、取出所有规则的明细信息装入哈希
        pHash_task = (unsigned char *)pasLHashInit(5000, 5000, sizeof(struct s_babtask), 0, 4);
        if(pHash_task == NULL)
        {
            printf("分配内存出错\n");
            return 0;
        }
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select cid,groupid,starttime,endtime,dids,mac from ncbabtask");
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
                                     UT_TYPE_STRING, 18, tmpTask.mac);

            macPrint(babDebug, "first sql=[%s],fetchinto ret=%d\n", sql, iReturn);

            while((iReturn == 0) || (iReturn == 1405))
            {
                macPrint(babDebug, "fetch into ret=%d,cid=%d,mac=%s\n", iReturn, tmpTask.cid, tmpTask.mac);
                if(strlen(tmpTask.mac) == 17)
                {
                    psTask = (struct s_babtask *)pasLHashLookA(pHash_task, (char*)&tmpTask.cid);
                    if(psTask)
                    {
                        psTask->cid = tmpTask.cid;
                        psTask->groupid = tmpTask.groupid;
                        psTask->starttime = tmpTask.starttime;
                        psTask->endtime = tmpTask.endtime;
                        strcpy(psTask->dids, tmpTask.dids);
                        strcpy(psTask->mac, tmpTask.mac);
						macPrint(babDebug, "insert to hash cid=%lu, starttime=%llu, endtime=%llu\n", 
							psTask->cid, psTask->starttime, psTask->endtime);
                    }
                }
                else
                {
                    macPrint(babDebug, "主mac不为17位, mac=[%s]\n", tmpTask.mac);
                }

                memset(&tmpTask, 0, sizeof(tmpTask));
                iReturn = pasDbFetchInto(psCur,
                                         UT_TYPE_LONG, 4, &tmpTask.cid,
                                         UT_TYPE_LONG, 4, &tmpTask.groupid,
                                         UT_TYPE_LONG8, 8, &tmpTask.starttime,
                                         UT_TYPE_LONG8, 8, &tmpTask.endtime,
                                         UT_TYPE_STRING, 250, tmpTask.dids,
                                         UT_TYPE_STRING, 18, tmpTask.mac);
            }
            pasDbCloseCursor(psCur);
        }
        else
        {
            printf("error sql=[%s],cur is null\n", sql);
        }
        //3、遍历哈希表做任务对应的统计
        psTask = (struct s_babtask*)pasLHashFirst(pHash_task, &sHashInfo);
        taskCount = 0;
        while(psTask)
        {
            memset(caServerlist, 0, sizeof(caServerlist));
            // DO:根据dids获取servicecode的列表
            strcpy(caServerlist, getServicecodesByDids(psShmHead, psTask->dids));
            macPrint(babDebug, "finish get servicecode list %s\n", caServerlist);
            //创建一个单链表用于存放时间段+servicecode组合
            s_eachpoint* pPointList = createPointList();  //用于存放时间点+地点的单链表
            memset(sql, 0, sizeof(sql));
            //e.g. select mac,servicecode,from_unixtime(stime) from ncmactermatt_if_201508 where mac='20-02-AF-C5-43-AC' group by stime
            if(strlen(caServerlist) > 0)
            {
                sprintf(sql, "select servicecode,stime from ncmactermatt_if_%s where mac='%s' and stime>%llu and stime<%llu and servicecode in (%s) group by stime", utTimFormat("%Y%m", time(0)), psTask->mac, psTask->starttime, psTask->endtime, caServerlist);
            }
            else
            {
                sprintf(sql, "select servicecode,stime from ncmactermatt_if_%s where mac='%s' and stime>%llu and stime<%llu group by stime", utTimFormat("%Y%m", time(0)), psTask->mac, psTask->starttime, psTask->endtime);
            }
			macPrint(babDebug, "get time list sql=%s\n", sql);
            psCur = pasDbOpenSql(sql, 0);
            if(psCur)
            {
                memset(&tmpPoint, 0, sizeof(tmpPoint));
                tmpPoint.cid = psTask->cid;
				memcpy(tmpPoint.mac, psTask->mac, sizeof(tmpPoint.mac));
                iReturn = pasDbFetchInto(psCur, 
                                         UT_TYPE_STRING, 16, tmpPoint.servicecode,
                                         UT_TYPE_LONG8, 8, &tmpPoint.starttime);
                tmpPoint.endtime = tmpPoint.starttime + 3600;//离开时间默认为起始时间+1小时
                while((iReturn == 0) || (iReturn == 1405))
                {
                	macPrint(babDebug, "========add pair<starttime=%llu, servicecode=%s>=======\n\n", tmpPoint.starttime,tmpPoint.servicecode);
                    addToPointList(pPointList, &tmpPoint);
                    memset(&tmpPoint, 0, sizeof(tmpPoint));
                    tmpPoint.cid = psTask->cid;
					memcpy(tmpPoint.mac, psTask->mac, sizeof(tmpPoint.mac));
                    iReturn = pasDbFetchInto(psCur,
                                             UT_TYPE_STRING, 16, tmpPoint.servicecode,
                                             UT_TYPE_LONG8, 8, &tmpPoint.starttime);
                    tmpPoint.endtime = tmpPoint.starttime + 3600;					
                }
                pasDbCloseCursor(psCur);
            }

            //整理列表里相隔时间过近的点,合并为一个点
            reOrganizePointList(pPointList);
			
            s_eachpoint* pTmpNode = pPointList->pNext;
            //统计所有的mac地址
            while(pTmpNode)
            {
                pHash_allMacs = (unsigned char *)pasLHashInit(5000, 5000, sizeof(struct s_macinfo), 0, 17);
                if(pHash_allMacs == NULL)
                {
                    printf("分配内存出错\n");
                    return 0;
                }
				macPrint(babDebug,"before search pair<starttime=%llu, servicecode=%s>\n", pTmpNode->starttime,pTmpNode->servicecode);
                doDbSearchMacs(babDebug, pHash_allMacs, pTmpNode);
                //遍历pHash_allPoints，将所有的点存入数据库
                doDbInsertMacs(babDebug, pHash_allMacs);
                //释放存放所有点的哈希表
                if(pHash_allMacs)
                    free(pHash_allMacs);
                pTmpNode = pTmpNode->pNext;
            }

            //释放内存
            destroyPointList(pPointList);
            psTask = (struct s_babtask*)pasLHashNext(&sHashInfo);
            macPrint(babDebug, "====================finish %d task, total %d=================== \n", (++taskCount), lSumTask);
            sleep(2);
        }
        if(pHash_task)
            free(pHash_task);
        macPrint(babDebug, "\n================finish one round, will start in 3*60s...===========\n\n");
        sleep(3 * 60);
    }
}



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
    ncsStatMacBab(psShmHead);
    utShmClose(psShmHead);
    exit(0);
}


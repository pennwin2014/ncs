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



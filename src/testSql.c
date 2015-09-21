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

int ncsDoSql(utShmHead *psShmHead)
{
    char sql[2048] = "";
    int iReturn = pasConnect(psShmHead);
    if(iReturn < 0)
    {
        printf("conn psShmHead fail iReturn=%d\n", iReturn);
        sleep(60);
        return 0;
    }

    long sqlDebug = utComGetVar_ld(psShmHead, "SqlDebug", 0);
    utPltDbHead *psDbHead;
    char caSql[1024] = "";
    char fileName[128] = "";
    psDbHead = utPltInitDb();  
	snprintf(caSql, sizeof(caSql)-1, "select * from ncmactermatt_if_201509 where 1=1 order by mac limit 5");
	macPrint(sqlDebug, "[caSql=%s\n", caSql);

    snprintf(fileName, sizeof(fileName) - 1, "/home/ncmysql/ncs/download/doUserSqlResult_%llu.dat", time(0));
    snprintf(sql, sizeof(sql) - 1, "%s into outfile '%s'", caSql, fileName);
    macPrint(sqlDebug, "sql=%s", sql);
    //iReturn = pasDbExecSqlF(caSql);

	
   // sprintf(sql, "%s into outfile 'testDoSql.txt' ");

    //macPrint(1, "do sql = %s\n", sql);

    return pasDbExecSqlF(sql);
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
    ncsDoSql(psShmHead);
    utShmClose(psShmHead);
    exit(0);
}



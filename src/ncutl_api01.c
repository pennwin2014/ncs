// Mac嗅探
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <zlib.h>
#include "utoall.h"
#include "pasutl.h"
#include "pasdb.h"
#include "pasdef.h"
#include "pasutl.h"
#include "ncapi.h"
#include "ncmac.h"
#include "ncdef.h"
#define NC_LNK_APSRVONLINE     161   /* AP的在线信息              		 */
#define NC_LNK_MACLOCATION     169   /* Mac采集信息                    */   
#define NC_LNK_APMACONLINE     170   /* AP MAC地址对照表               */
#define NC_LNK_DEPTINFO        171   /* 场所信息                       */
#define NC_LNK_SECFACTORY      172   /* 安全厂商信息                   */
#define   PAS_SRCFILE    2891


ncApSrvOnline *ncMacLogGetOnlineApByMac(utShmHead *psShmHead,char *mac)
{
    ncApMacOnline *psMac;
    psMac = (ncApMacOnline *)utShmHashLook(psShmHead,NC_LNK_APMACONLINE,mac);
    if(psMac) {
        ncApSrvOnline *psAp;
        psAp = (ncApSrvOnline *)utShmHashLook(psShmHead,NC_LNK_APSRVONLINE,psMac->apname);
        return psAp;
    }
    else {
        return NULL;
    }
}

ncBcpSec *ncUtlGetScodeFromName(utShmHead *psShmHead,uchar *caScode)
{
    int i,iSumRecord;
    ncBcpSec *psSec;
    iSumRecord = utShmArrayRecord(psShmHead,NC_LNK_SECFACTORY);
    psSec = (ncBcpSec *)utShmArray(psShmHead,NC_LNK_SECFACTORY);
    if(psSec) {
        for(i=0;i<iSumRecord;i++) {
            if(strcasecmp(psSec[i].scode,caScode)==0) {
                return &psSec[i];
            }
        }
        return NULL;
    }
    else {
        return NULL;
    }
}


ncDeptinfo *ncUtlGetDeptByServiceCode(utShmHead *psShmHead,char *pName)
{
    int i,iSumRecord;
    ncDeptinfo *psDept;
    iSumRecord = utShmArrayRecord(psShmHead,NC_LNK_DEPTINFO);
    psDept = (ncDeptinfo *)utShmArray(psShmHead,NC_LNK_DEPTINFO);
    if(psDept) {
        for(i=0;i<iSumRecord;i++) {
            if(strcasecmp(psDept[i].name,pName)==0) {
                return &psDept[i];
            }
        }
        return NULL;
    }
    else {
        return NULL;
    }
}

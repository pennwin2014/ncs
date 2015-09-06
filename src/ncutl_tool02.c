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
#define   PAS_SRCFILE    2891
// iFlags == 1  Ap   2--Group  3--GroupDept

int ncApSrvShowOnlineAp(utShmHead *psShmHead,int iFlags)
{
    uchar caServiceName[128];
    ncApSrvOnline *psOnline;
    pasHashInfo     sHashInfo;
    ncDeptinfo      *psDept;
    int iReturn;
    uint4 lBase;
    uchar *pHash;
    int iCount = 0;
    lBase = utShmGetBaseAddr(psShmHead);
    if(iFlags == 0 || iFlags == 1) {
        pHash = utShmHashHead(psShmHead,NC_LNK_APSRVONLINE);
        if(pHash == NULL) {
            printf(" Memory Error \n");
            return (-1);
        }
        printf(" Num   ApName    ServiceName   Gid   dbmark \n");
        psOnline = (ncApSrvOnline *)pasHashFirst(pHash,&sHashInfo);
        while(psOnline) {
            psDept = (ncDeptinfo *)ncUtlGetDeptByServiceCode(psShmHead,psOnline->servicecode);
            if(psDept) {
               strcpy(caServiceName,psDept->disp);
            }
            else {
                strcpy(caServiceName,"\0");
            }
            printf("[% 4d] %-15s  %-15s %-15s  %-31s  %10u  %-15s %-17s %-15s  %-31s  %s %10s %10s %d %d  Mtype=%d\n",
                                    iCount,
                                    psOnline->apname,
                                    utComHostIp(htonl(psOnline->lSip)),
                                    psOnline->servicecode,
                                    caServiceName,
                                    psOnline->lGid,
                                    utComHostIp(htonl(psOnline->lIp)),
                                    psOnline->apmac,
                                    psOnline->ssid,
                                    psOnline->address,
                                    utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime),
                                    psOnline->longitude,
                                    psOnline->latitude,
                                    psOnline->dbmark,
                                    psOnline->cmode,
                                    psOnline->mtype);
            iCount ++;
            psOnline = (ncApSrvOnline *)pasHashNextS(&sHashInfo);
        }
    }
    if(iFlags == 0 || iFlags == 3) {
        ncDeptinfo *psDept;
        int i,iSum;
        psDept = (ncDeptinfo *)utShmArray(psShmHead,NC_LNK_DEPTINFO);
        if(psDept) {
            printf(" Dept Info \n");
            iSum = utShmArrayRecord(psShmHead,NC_LNK_DEPTINFO);
            for(i=0;i<iSum;i++) {
                printf("   Did:%10u  Gid:%10u Ip:%-15s  DName:%-15s  Disp:%-31s Address:%-31s %s %s Gtype=%c  Scode:%s\n",
                        psDept[i].lDid,
                        psDept[i].lGid,
                        utComHostIp(htonl(psDept[i].lSip)),
                        psDept[i].name,
                        psDept[i].disp,
                        psDept[i].address,
                        psDept[i].caLongitude,
                        psDept[i].caLatitude,
                        psDept[i].gtype,
                        psDept[i].caScode);
            }
        }
    }
    if(iFlags == 0 || iFlags == 2) {
        ncBcpSec *psSec;
        int i,iSum;
        psSec = (ncBcpSec *)utShmArray(psShmHead,NC_LNK_SECFACTORY);
        if(psSec) {
            printf(" SecFactory Info \n");
            iSum = utShmArrayRecord(psShmHead,NC_LNK_SECFACTORY);
            for(i=0;i<iSum;i++) {
                printf("   sCode:%-9s  sName:%-31s    sLink:%-15s  sMail:%s  Tel:%s  Modi:%u \n",
                        psSec[i].scode,
                        psSec[i].sname,
                        psSec[i].slink,
                        psSec[i].saddress,
                        psSec[i].smail,
                        psSec[i].stel,
                        psSec[i].moditime);
            }
        }
    }
    
}

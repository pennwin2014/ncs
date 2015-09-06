#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utoall.h"
#include "pasdef.h"
#include "ncmac.h"
int main(int argc,char **argv)
{
    utShmHead  *psShmHead;
    char caTime[32],caThisTime[32];
    char argval;
    char caConfFile[64];
    int i,iReturn;
    unsigned long lTime;
    int iSum;
    strcpy(caConfFile,"pqconf_ncs.cnf");    
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
    }
    psShmHead=utComShmOpen(caConfFile);
    if(psShmHead == NULL) {
        printf(" Share Memory not Init \n");
        exit(0);
    }
    ncSrvShowAllMac(psShmHead);
    utShmClose(psShmHead);
    exit(0);
}


int ncSrvShowAllMac(utShmHead *psShmHead)
{
    
    pasHashInfo sHashInfo;
    uchar *pHash;
    ncApSrvOnline *psAp;
    long lBase;
    char caSdate[32],caMark[64];
    char caMacStr[20],caMacStr1[20],caServiceCode[16],caMacStr2[20];
    int iReturn;
    ncLbsMacLocation *psMacLoc;
    pHash =  (unsigned char *)utShmHashHead(psShmHead,NC_LNK_MACLOCATION);
    if(pHash) {
        psMacLoc = (ncLbsMacLocation *)pasHashFirst(pHash,&sHashInfo);
        printf("TermMac    ApMac  lRssi  URssi  Noise  Channel StartTime  LastTime SSID Encrypt Mutype Longitude Latitude ServiceCode\n");
        while(psMacLoc) {
            psAp = (ncApSrvOnline *)ncMacLogGetOnlineApByMac(psShmHead,psMacLoc->apmac);
            if(psAp) {
				printf("ss apmac=%s ---> dispname=%s\n",  pasCvtMacStr(psMacLoc->apmac,caMacStr1), psAp->dispname);
                strcpy(caServiceCode,psAp->servicecode);
            }
            else {
                strcpy(caServiceCode,"\0");
            }
            strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",psMacLoc->lasttime));
            printf("%s  %s   %d  %s %5d  %5d  %5d %3u %s %s Ssid:%-18s Crypt:%d MuType:%d %-10s %-10s S:%s\n",
                    pasCvtMacStr(psMacLoc->mac,caMacStr),
                    pasCvtMacStr(psMacLoc->apmac,caMacStr1),
                    psMacLoc->cAssocicated,
                    pasCvtMacStr(psMacLoc->caBssid,caMacStr2),
                    psMacLoc->lrssi,
                    psMacLoc->urssi,
                    psMacLoc->noise,
                    psMacLoc->channel,
                    utTimFormat("%Y-%m-%d %H:%M:%S",psMacLoc->starttime),
                    caSdate,
                    psMacLoc->ssid,
                    psMacLoc->encrypt,
                    psMacLoc->mutype,
                    psMacLoc->longitude,
                    psMacLoc->latitude,
                    caServiceCode);
            psMacLoc = (ncLbsMacLocation *)pasHashNextS(&sHashInfo);
        }
    }
    return 0;
}

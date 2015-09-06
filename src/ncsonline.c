#include <unistd.h>
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
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include "config.h"
#include <pcap.h>
#include "utoall.h"
#include "pasdef.h"
#include "ncsdef.h"
main()
{
    utShmHead *psShmHead;
    char *pHash;
    pasHashInfo sHashInfo;
    int  iCount;
    char caTemp[64];
    uint4 lTime=time(0);
    unsigned long lBase;
    ncsOnline *psOnline;
    ncsClient *psClient;
    int i,iReturn;
    psShmHead = utComShmOpen("/home/ncmysql/ncs/bin/pqconf_ncs.cnf");
 	if(psShmHead == NULL) {
   	    printf(" Share Memory Open Error i==%d\n",i);
   	    return (-1);
    }
    iCount = 0;
    pHash = (unsigned char *)utShmHashHead(psShmHead,NCS_LNK_ONLINE);
    if(pHash == NULL) {
        printf(" NCS_LNK_ONLINE Error");
        return (-1);
    }
    lBase = utShmGetBaseAddr(psShmHead);
    lTime = time(0);
    psOnline  = (ncsOnline *)pasHashFirst(pHash,&sHashInfo);
    printf("Online Client, NcsName:%-15s  Print at %s \n",utComGetVar_sd(psShmHead,"NcsName","\0"),utTimFormat("%Y-%m-%d %H:%M:%S",lTime));
    printf("%-4s %-10s %-14s %-40s %-15s %-10s %-10s %-8s %-4s %-6s %-5s\n",
                        "Num","Did","ServiceCode","DispName","ClientIp","Bytes","TransBytes","Tcp","Users","Start","Last");
    while(psOnline) {
        iCount ++;
        psClient = (ncsClient *)ncsUtlGetClientById(psShmHead,psOnline->userid); 
        printf("%4lu %10lu %10u %14s %-40s %-15s %10llu %10llu %8lu %4lu %6lu %5lu\n",
                        iCount, psOnline->userid,
                        psClient->groupid,
                        utStrncpy(caTemp,psClient->username,39),psClient->dispname,utComHostIp(htonl(psOnline->lSip)),
                        psOnline->lBytes[0]+psOnline->lBytes[1],
                        psOnline->lSendBytes+psOnline->lRecvBytes,psOnline->lTcp,psOnline->lUser,
                        lTime-psOnline->lStartTime,lTime-psOnline->lLastTime);
        psOnline = (ncsOnline *)pasHashNextS(&sHashInfo);
    }
    return 0;
}
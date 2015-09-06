#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utoall.h"
#include "pasdef.h"
#include "ncdef.h"
int main(int argc,char **argv)
{
    utShmHead  *psShmHead;
    char caOutFile[32];
    char argval;
    int iFlags = 0;
    argv++;
    argc--;
    while(argc) {
        if(**argv != '-') {
            printf("ncsshowbase [-o ap | sec | Dept] \n");
            exit(0);
        }
        argval = *(*argv + 1);
        argc--;
        argv++;
        switch(argval) {
            case 'o':
                strcpy(caOutFile,(*argv));
                if(strcasecmp(caOutFile,"Ap")==0) {
                    iFlags = 1;
                }
                else if(strcasecmp(caOutFile,"Sec")==0) {
                    iFlags = 2;
                }
                else if(strcasecmp(caOutFile,"Dept")==0) {
                    iFlags = 3;
                }
                argc--;
                argv++;
                break;
            default:
                printf("ncsshowbase -o ap | sec | dept \n");
                exit(0);
        }
    }
    psShmHead=utComShmOpen("pqconf_ncs.cnf");
    ncApSrvShowOnlineAp(psShmHead,iFlags);
}


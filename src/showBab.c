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




/*
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
*/


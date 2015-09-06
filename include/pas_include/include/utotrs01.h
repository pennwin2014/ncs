/*
   
   FileName:  uttrs01.h
   ModiFiy :  04/15/2000   By Liyunming
   Date    :  11/10/1999   By Josen
*/


#ifndef _UT_TRS_H01_H 
#define _UT_TRS_H01_H
#define  UT_ERR_MESSAGE      "系统发生故障，请与系统管理员联系"
/* define public function return value */
#define  UT_TRS_SUCCESS      00000L           /* success */
#define  UT_TRS_ERROR        -1               /* Error   */
#define  UT_TRS_ERR_INIT     400001L          /* initial error */
#define  UT_TRS_ERR_WRIT     400002L          /* initial error */
#define  UT_TRS_ERR_MEM      400003L           /* mem malloc error */
#define  UT_TRS_ERR_OPEN     400004L           /* translate action error */



/*define the information link struct */
/* note: mark flag length must be 2 */
#define UT_TRS_MAX_MARK_NUM     8
#define UT_TRS_MARK_HEAD        "[#"
#define UT_TRS_MARK_TAIL        "#]"

#define UT_TRS_SINGLELOOP_NUM       7   /* [#SLPB999#] single loop begin*/

typedef struct utTrsDataLink_s
{
    char   caMark[UT_TRS_MAX_MARK_NUM+1];
    char  *cpInfo;
    struct utTrsDataLink_s *spNext;
} 
utTrsPlateDbLink;


/* Function define */
long utTrsInitial(char *pConfFile );
long utTrsInitialDbLink( utTrsPlateDbLink **spDbHead );
long utTrsWriteBufToFile( char *cpOutFileBuf, int iBufSize, char *caFilePath );
long utTrsFreeDbLink( utTrsPlateDbLink *spDbHead );
long utTrsShowDbLink( utTrsPlateDbLink *spDbHead );
long utTrsPlateToStateHtml(char *caFilePath, utTrsPlateDbLink *spDbHead, char *cpOutBuf, 
                                            int iBufSize, long *lBufLen );
long utTrsAppendToDbLink( utTrsPlateDbLink *spDbHead, utTrsPlateDbLink *spDbIn );                                            
int utTrsGetDbLinkNum(int *in);
#endif




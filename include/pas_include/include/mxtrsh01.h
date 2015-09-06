/*
   FileName:  mxtrsh01.h
   Date    :  11/10/1999   By Josen
*/

#ifndef _MX_TRS_H01_H 
#define _MX_TRS_H01_H

/* define public function return value */
#define  MX_TRS_SUCCESS    400000L           /* success */
#define  MX_TRS_ERR_INI    400001L           /* initial error */
#define  MX_TRS_ERR_MEM    400002L           /* mem malloc error */
#define  MX_TRS_ERR_TRS    400003L           /* translate action error */
#define  MX_TRS_ERR_ENV    400004L           /* read env from config file error */
#define  MX_TRS_ERR_NOR    400005L           /* error mark for internal normal */


#define  MX_TRS_CONFIG_FILE  "pqconf.cnf"
#define  MX_TRS_PLATE_DIR    "plate"
#define  MX_TRS_HTML_DIR     "html"

/* when read infor from file, some times, I will read it to one buffer first */
#define  MX_TRS_MAX_FILEBUF_LENGTH      (65535L)
#define  MX_TRS_MAX_PLATEFILE_LENGTH    (32767L)

/*define debug level */
#define DEBUG_LEVEL_0	0L        /* No Debug */ 
#define DEBUG_LEVEL_1	1L        /* A: It will cause Down */ 
#define DEBUG_LEVEL_2	2L        /* B: It will cause UI error or action error */ 
#define DEBUG_LEVEL_3	3L        /* C: The action has some problem or include some data error */ 
#define DEBUG_LEVEL_4	4L        /* D: DataBase error occur */ 
#define DEBUG_LEVEL_5	5L        /* E: Other light problem */ 
#define DEBUG_LEVEL_6	6L        /*    Normal message only for debug use */ 
#define DEBUG_LEVEL_9	9L        /*    Show all message */ 


/*define the information link struct */
/* note: mark flag length must be 2 */
#define MX_TRS_MAX_MARK_NUM     8
#define MX_TRS_MARK_HEAD        "[#"
#define MX_TRS_MARK_TAIL        "#]"

#define MX_TRS_SINGLELOOP_NUM       7   /* [#SLPB999#] single loop begin*/

typedef struct MxDataLink
{
    char caMark[MX_TRS_MAX_MARK_NUM+1];
    char *cpInfo;
    struct MxDataLink *spNext;
} 
MXTRSPLATEDBLINK;


/* Function define */

extern long mxTrsInitial( void );                                                             /* 001 */
/* Entrance for extern calling, note: you need call mxTrsInitial at first */
extern long mxTrsPlateToStateHtml( char *pcPlateName, MXTRSPLATEDBLINK *spDbHead, 
                                        char *pcOutBuf, int iBufSize, long *lBufLen );        /* 002 */
                                        
extern long mxTrsReadEnvString( char *psEnvBuf, char *psTitle, long *lValue, 
                         long lMax, char *psOutStr, int iStrSize );                           /* 003 */
extern long mxTrsWriteBufToFile( char *cpOutFileBuf, int iBufSize, char *caFilePath );        /* 004 */
extern long mxTrsStrncmpi( char *psSource, char *psTarget, int iLen );
extern void mxTrsPutMessage( long iBugLevel, char *pcModeNo, char *pcMessageStr );
extern long mxTrsTransMain( char *cpPlateName, MXTRSPLATEDBLINK *spDbHead );                  /* 005 */
extern long mxTrsInitialDbLink( MXTRSPLATEDBLINK **spDbHead );                                /* 006 */
extern long mxTrsFreeDbLink( MXTRSPLATEDBLINK *spDbHead );                                    /* 007 */
extern long mxTrsAppendToDBLink( MXTRSPLATEDBLINK *spDbHead, MXTRSPLATEDBLINK *psDbIn );      /* 008 */
extern long mxTrsGetDBLinkNode( MXTRSPLATEDBLINK **psDbNew );                                 /* 009 */
extern long mxTrsShowDbLink( MXTRSPLATEDBLINK *spDbHead );                                    /* 010 */
extern long mxTrsGetMarkFromPlateBuf( char *cpCurIn, int *ipForwardLen, char *caMark );       /* 011 */
extern long mxTrsSearchMarkFromLink( char *caMark, MXTRSPLATEDBLINK *spDbHead,
                                                   char **cppInfo, int *ipInfoLen );          /* 012 */
extern int mxTrsAnalysisPlateHtml( char *cpIn, char *cpOut,  
                                       MXTRSPLATEDBLINK *spDbHead, long *lBufLen);            /* 013 */

/* only for test use now */
char *MxTrsGetEnvWebPath( void );


#endif




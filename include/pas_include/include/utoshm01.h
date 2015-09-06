#ifndef  __UTSHMDEF
#define  __UTSHMDEF            1
#define UT_SHMID               199812201
#define UT_SHMVER	          1001
// #define UT_SHMHEADSIZE         64

#define UT_SHMNOUSED            0

#define UT_SHMDOUBLELNK         1
#define UT_SHMBINTREE           2
#define UT_SHMARRAY             3
#define UT_SHMHASH              4
#define UT_SHMLHASH             5
#define UT_SHMNOTFOUND         0
#define UT_SHMISEXIST          1
#define UT_SHMADDTOEND         0
#define UT_SHMADDTOBEGIN       1

#define UT_SHMMAXPAGES      20
#define UT_SHMBASESIZE      1024000
#define UT_SHMPERSIZE       1024000
#define UT_SHMMAXLNK        20
#include "utotype.h"
typedef struct {
        short   nNodeType;   /* Node type           */
        short   nPageNo;     /* Page Number  */
        short   nKeyLen;     /* The Length of Key   */
        short   reverse1;
        long    lOffset;     /* Offset        */
        long    lKeyStart;   /* The Position of Key (Based on current record) */
        long    reverse2;
        long    lLockKey;         /*  Lock Id, If the value is  -1, no lock is  set  */
} utShmBinRoot;


typedef struct {
        short   nNodeType;        /* Node type           */
        short   nStartPageNo;     /* Page Number  */
        short   nEndPageNo;
        short   nCurPageNo;
        long    lStartOffset;     /* Offset        */
        long    lEndOffset;
        long    lCurOffset;
        long    lLockKey;         /*  Lock Id, If the value is  -1, no lock is  set  */
} utShmNodeRoot;

/* The Struct of utShmNodeRoot is different in different Data
     Double Link    Bin Tree
     nNodeType        nNodeType
     nStartPageNo     nPageNo
     lStartOffset     lOffset
     lEndOffset       lKeyStart
     nEndPageNo       nKeyLen
     nCurPageNo       reverse1
     lCurOffset       reverse2
     lLockKey         lLockKey(Not Used)
*/

typedef struct {
        long  lPageId;
        long  lOffset;
        long  lTotalBytes;
        long  lUsedBytes;
} utShmPageInfo;


typedef struct {
        long  lBytes;
        long  lPrevOffset;
        long  lNextOffset;
        short nPrevPageNo;
        short nNextPageNo;
#ifdef PAS_X64
        int4  iRev;
#endif        
} utShmNodeInfo;

#define UT_SHMNODEHEADSIZE     sizeof(utShmNodeInfo)

typedef struct {
        long  lShmId;
        long  lBaseSize;
        long  lPerSize;
        long  lLockKey;
        short nShmVer;
        short nMaxPages;
        short nNumPages;
        short nMaxLnk;
        unsigned long  lCount;
        long  lShmKey;
        long  lLockNum;   /* Lock Number   */
        long  lAllocSize; /* Alloc Size    */
        short nNumLnk;
        unsigned char  cMark[6];           /*  2009/03/16  修改为18个标志位    */
        unsigned long  lThisTime;           /* 当前时间               */   //2012/02/09 修改
        unsigned long  lThisDateTime;       /* hhmmss     */
        unsigned char  *pBase;              /* 基准地址                         */
        unsigned long  lCreateTime;
} utShmHeadInfo;

typedef struct {
        utShmHeadInfo  *psHead;
        utShmNodeRoot  *psFreeLnk;
        utShmNodeRoot  *psUsedLnk;
        utShmPageInfo  *psPages;
        char           **pIndex;
        char           *user;
        unsigned long  lKey;                 // Add:2011/09/20
        unsigned long  lCreateTime;          // 创建时间
        unsigned long  lShmId;
} utShmHead;

typedef struct {
        utShmHead*psShmHead;
        utShmNodeInfo  *psShmLnkInfo;
        long  lOffset;
        short nPageNo;
        short nReverse;
#ifdef PAS_X64        
        short nReverse1[2];
#endif
} utShmHand;


typedef struct {
        long  lOffset;
        short nPageNo;
        short nReserve;
#ifdef PAS_X64        
        short nReverse1[2];
#endif
} utShmMem;

typedef struct {
    unsigned long lNext;
    unsigned long lContent;
} pasLHashIndex;


typedef struct {
        unsigned  long  lMaxHash;      /* Hash表的最大值      */
        unsigned  long  lMaxRec;       /* 最大记录数          */
        unsigned  long  lRecLen;       /* 记录长度            */
        unsigned  long  lFreeIndex;     /* 空闲Hash表的地址    */
        unsigned  long  lFreeText;      /* 空闲内容            */
        unsigned  long  lSumRecord;     /* 最大记录数          */ 
        unsigned  long  lCurRecord;     /* 当前记录            */
        unsigned  short nKeyStart;      /* Key的起始位置       */
        unsigned  short nKeyLen;        /* Key的长度           */
        unsigned  char  lLock;          /* 锁                  */
        unsigned  char  cKeyType;       /* key类型 0-字符串 1-短整型  2- long 3-long long   */
        unsigned  char  cRev[2];        /* 保留                */
        unsigned  long  lPid;           /* Pid                 */
        unsigned  long  lLastTime;      /* Hash表最后修改时间  */
} pasLHashHead;


typedef struct {
        unsigned long lNext;          /* lNext为0表示记录有内容  */
        char          caText[128];
} pasLHashText;

typedef struct {
        char *pHash;
        unsigned long lIndex;      /* 当前索引位置  */
        pasLHashIndex *psIndex;
        unsigned long lIndexNum;   /* 序号          */
} pasLHashInfo;

typedef struct pasHashHead_s {
    unsigned long  lMaxHash;           // Hash表最大
    unsigned long  lMaxRec;            // 最大记录数
    unsigned long  lSumRec;            // 记录总数
    unsigned short iRecLen;            // 记录长度
    unsigned short iTxtLen;            // 每一条Hash记录的长度
    unsigned short iKeyLen;            // Key的长度
    unsigned short iKeyStart;          // Key起始位置
    unsigned  char  iLock;             /* 0--能操作  1--不能操作              */
    unsigned  char  cKeyType;          /* key类型             */
    unsigned  char  cRev[2];           /* 保留                */
    int            iPid;               // 进程号
    unsigned long  lLastLock;          // 最后一次上锁时间
    unsigned long  lCtime;             // 创建时间
} pasHashHead;
    
typedef struct pasHashText_s {
    char        cMark;           // 0-无数据   1-有数据
    char        cRev;            // 保留
    unsigned    short  nCount;   // 记录数量
#ifdef PAS_X64
    uint4       lRev;
#endif
    long        lIndex;          // 当前记录的Index
    char        a[0];            // 内容   
} pasHashText;

typedef struct {
        char *pHash;
        long lIndex;              /* 当前索引位置  */
} pasHashInfo;
        

/* 内存记录   */
typedef struct {
    unsigned char **ps;
    unsigned long lSum;
    unsigned long lMax;
} pasMemRecordHead;


/* utoshm01.c */
utShmHead *utShmCreate(
                        long lKey,     
                        short nMaxPages, 
                        short nMaxLnk,
                        long  lBaseSize,
                        long  lPerSize);
utShmHead *utShmCreate777(
                        long lKey,     
                        short nMaxPages, 
                        short nMaxLnk,
                        long  lBaseSize,
                        long  lPerSize);
int utShmCreateFromOld(utShmHead *psShmHead);
utShmHead *utShmOpen(long lKey);
utShmHead *utShmCurHead();
void utShmClose(utShmHead *psShmHead);
void utShmRemove(utShmHead *psShmHead);
int utShmAlloc(utShmHead *psShmHead, utShmHand *psShm,long lSize);
void *utShmAddr(utShmHand *psShm);
void *utShmMapAddr(utShmHead *psShmHead,short nPageNo,long lOffset);
int utShmAllocPages(utShmHead *psShmHead);
int utShmLookTree(utShmHead *psShmHead,utShmHand *psShm,
                             long lSize);
void utShmFree(utShmHand *psShm);
long utShmAllocSize(utShmHand *psShm);
void utShmFreeShm0(utShmHead *psShmHead, short nPageNo, long lOffset);
int utShmAddToFreeLnk(utShmHead *psShmHead,
                        utShmHand *psShm);
int utShmDelFreeLnk(utShmHand *psShm);
void utShmLock(utShmHead *psShmHead);
void utShmUnLock(utShmHead *psShmHead);
int utShmAllocMem(utShmHead *psShmHead, utShmMem *psMem,long lSize);
void utShmFreeMem(utShmHead *psShmHead, utShmMem *psMem);
void *utShmMemAddr(utShmHead *psShmHead,utShmMem *psMem);
void utShmHandToMem(utShmHand *psShm,utShmMem *psMem);
void utShmMemToHand(utShmHead *psShmHead,utShmMem *psMem,utShmHand *psShm);
int utShmReAlloc(utShmHead *psShmHead, utShmHand *psShm,long lSize);
int utShmSetMark(utShmHead *psShmHead,int iNum,int iValue);
int utShmGetMark(utShmHead *psShmHead,int iNum);
long utShmGetBaseAddr(utShmHead *psShmHead);
int utShmTestShmHead(utShmHead *psShmHead);
int utShmSetShmChange(utShmHead *psShmHead);
int utShmSetTime(utShmHead *psShmHead);
unsigned long utShmTime(utShmHead *psShmHead);
unsigned long  utShmGetThatDayTime(utShmHead *psShmHead);
int utShmSetThisTime(utShmHead *psShmHead,unsigned long lTime);
/* utoshm02.c */
int utShmInitDLink(utShmHead *psShmHead,short nLnkNo);
int utShmFreeDLink(utShmHead *psShmHead,short nLnkNo);
int utShmAddDLink(utShmHand *psShm,short nLnkNo,short nFlags);
int utShmGetFirstDlink(utShmHead *psShmHead,
                  short nLnkNo,
                  utShmHand *psShm);
int utShmGetLastDlink(utShmHead *psShmHead,
                       short nLnkNo,
                       utShmHand *psShm);
int utShmGetCurDlink(utShmHead *psShmHead,
                       short nLnkNo,
                       utShmHand *psShm);
int utShmGetNextDlink(utShmHead *psShmHead,
                       short nLnkNo,
                       utShmHand *psShm);
int utShmGetPrevDlink(utShmHead *psShmHead,
                       short nLnkNo,
                       utShmHand *psShm);
int utShmAllDlink(utShmHead *psShmHead, char *pHead,
                short nLnkNo,long lStart,long lNum,
                int utShmFun(char *pHead,char *p));
int utShmRemoveDlink(utShmHead *psShmHead,
                       short nLnkNo,
                       utShmHand *psShm);
int utShmUnlinkDlink(utShmHead *psShmHead,
                       short nLnkNo,
                       utShmHand *psShm);
int utShmInitArray(utShmHead *psShmHead,short nLnkNo,long lSize,
                   long lRecord);
int utShmFreeArray(utShmHead *psShmHead,short nLnkNo);
char *utShmArray(utShmHead *psShmHead,short nLnkNo);
long utShmArrayRecord(utShmHead *psShmHead,short nLnkNo);
long utShmArraySize(utShmHead *psShmHead,short nLnkNo);
unsigned long utShmCount(utShmHead *psShmHead);
int utShmArrayResize(utShmHead *psShmHead,short nLnkNo,long lSize,
                   long lRecord);
/* utoshm03.c */
int utShmInitBinTree(utShmHead *psShmHead,short nLnkNo);
int utShmFreeBinTree(utShmHead *psShmHead,short nLnkNo);
int utShmFreeBinTree0(utShmHead *psShmHead,
                      short nPageNo,long lOffset);
int utShmAddBinTree(utShmHand *psShm,
                      short nLnkNo,
                      int utShmBinCompare(char *pSource,char *pKey));
int utShmAddBinTree0(utShmHand *psShm,
                      short nLnkNo,
                      int utShmBinCompare(char *pSource,char *pKey),
                      int iFlags);
int utShmLookBinTree(utShmHead *psShmHead,
                      short nLnkNo,
                      void *pKey,
                      utShmHand *psShm,
                      int utShmBinCompare(char *pSource,char *pKey));
long utShmAllBinTree(utShmHead *psShmHead,char *pHead,
                    short nLnkNo,long lStartRecord,
                    long lNumRecord,
                    int utShmFun(char *pHead,char *p));
long utShmAllBinTree0(utShmHead *psShmHead,
                      char *pHead,
                      short nPageNo,long lOffset,
                      long lStartRecord,
                      long lNumRecord,
                      int  utShmFun(char *pHead,char *p),
                      long lCount);
int utShmDelBinTree(utShmHead *psShmHead,short nLnkNo,char *pKey,
        int utShmBinCompare(char *pSource,char *pKey));
int utShmUnlinkBinTree(utShmHead *psShmHead,short nLnkNo,
        char *pKey,utShmHand *psShm,
        int utShmBinCompare(char *pSource,char *pKey));
int utShmBinComp_l(char *pSource,char *pKey);
int utShmBinComp_s(char *pSource,char *pKey);
int utShmSumBinTree(utShmHead *psShmHead,short nLnkNo);
/* utoshm04.c */
long utShmBinTreeGet(utShmHead *psShmHead,
                    short nLnkNo,
                    int utShmBinCompare(char *pSource,char *pKey),
                    char *pKey1,char *pKey2,
                    int utShmFun(char *pHead,char *p),
                    char *pHead);
int utShmAddBinTreeKey(utShmHand *psShm,
                      short nLnkNo,
                      int utShmBinCompare(char *pSource,char *pKey),
                      char *pKey);
int utShmAddBinTreeKey0(utShmHand *psShm,
                      short nLnkNo,
                      int utShmBinCompare(char *pSource,char *pKey),
                      char *pKey,
                      int iFlags);
/* utoshm05.c */
unsigned long pasLHashBufSize(unsigned long lMaxHash,
                              unsigned long lMaxRec,
                              unsigned long lRecLen);
char *pasLHashInit(unsigned long lMaxHash,unsigned long lMaxRec,unsigned long lRecLen,
                    unsigned short nKeyStart,unsigned short nKeyLen);
int pasLHashInit0(char *pBase,unsigned long lMaxHash,unsigned long lMaxRec,
            unsigned long lRecLen,unsigned short nKeyStart,unsigned short nKeyLen);
int pasLHashAdd(char *pHash,char *pBuffer,int iFlags);
char *pasLHashLook(char *pHash,char *pKey);
char *pasLHashLookA(char *pHash,char *pKey);
char *pasLHashLookA_L(char *pHash,char *pKey,unsigned long *lRec);
int pasLHashDel(char *pHash,char *pKey);
int pasLHashPrint(char *pHash);
int pasLHashAll(char *pHash,char *pHead,unsigned long lStart,unsigned long lNum,
                           int pasLHashFun(char *pHead,char *p));
unsigned long pasCompLHashIndex(char *pKey,unsigned long lLen,unsigned long lMaxHash);
unsigned long pasLHashRecord(char *pHash);
unsigned long pasLHashMaxRecord(char *pHash);
void *pasLHashFirst(char *pHash,pasLHashInfo *psHashInfo);
void *pasLHashNext(pasLHashInfo *psHashInfo);
void *pasLHashNextS(pasLHashInfo *psHashInfo);
int pasLHashLock(char *pHead);
int pasLHashUnlock(char *pHead);
int pasDddddd(char *pMsg,char *pHash);
int pasLHashRepair(char *pHash);
int pasLHashAddRecord(char *pHash,char *pBuffer);
char *pasLHashLookFirst(char *pHash,char *pKey);
pasMemRecordHead *pasLHashGetAllRecord(char *pHash,char *pHead,
                  int pasMemCompareFun(char *pHead,char *ps),
                  int pasMemSort(char **ps1,char **ps2));
pasMemRecordHead *pasHashGetAllRecord(char *pHash,char *pHead,
                  int pasMemCompareFun(char *pHead,char *ps),
                  int pasMemSort(char **ps1,char **ps2));
void pasFreeAllMemRecord(pasMemRecordHead *psHead);
/* utoshm06.c */
int utShmHashInit(utShmHead *psShmHead,short nLnkNo,
           unsigned long lMaxHash,unsigned long lMaxRec,unsigned long lRecLen,
           unsigned short nKeyStart,unsigned short nKeyLen);
int utShmEmptyHash(utShmHead *psShmHead,short nLnkNo);
int utShmFreeHash(utShmHead *psShmHead,short nLnkNo);
char *utShmHashHead(utShmHead *psShmHead,short nLnkNo);
int utShmHashAdd(utShmHead *psShmHead,short nLnkNo,char *pBuffer,int iFlags);
char *utShmHashLook(utShmHead *psShmHead,short nLnkNo,char *pKey);
char *utShmHashLookA(utShmHead *psShmHead,short nLnkNo,char *pKey);
char *utShmHashLookA0(utShmHead *psShmHead,short nLnkNo,char *pKey,int *iStatus);
int utShmHashDel(utShmHead *psShmHead,short nLnkNo,char *pKey);
int utShmHashAll(utShmHead *psShmHead,short nLnkNo,char *pHead,unsigned long lStart,unsigned long lNum,
                           int pasHashFun(char *pHead,char *p));
unsigned long utShmHashRecord(utShmHead *psShmHead,short nLnkNo);
unsigned long utShmHashMaxRecord(utShmHead *psShmHead,short nLnkNo);
/* utoshm07.c */
int utShmLHashInit(utShmHead *psShmHead,short nLnkNo,
           unsigned long lMaxHash,unsigned long lMaxRec,unsigned long lRecLen,
           unsigned short nKeyStart,unsigned short nKeyLen);
int utShmFreeLHash(utShmHead *psShmHead,short nLnkNo);
char *utShmLHashHead(utShmHead *psShmHead,short nLnkNo);
int utShmLHashAdd(utShmHead *psShmHead,short nLnkNo,char *pBuffer,int iFlags);
char *utShmLHashLook(utShmHead *psShmHead,short nLnkNo,char *pKey);
char *utShmLHashLookA(utShmHead *psShmHead,short nLnkNo,char *pKey);
int utShmLHashDel(utShmHead *psShmHead,short nLnkNo,char *pKey);
int utShmLHashAll(utShmHead *psShmHead,short nLnkNo,char *pHead,unsigned long lStart,unsigned long lNum,
                           int pasLHashFun(char *pHead,char *p));
unsigned long utShmLHashRecord(utShmHead *psShmHead,short nLnkNo);
unsigned long utShmLHashMaxRecord(utShmHead *psShmHead,short nLnkNo);
pasMemRecordHead *utShmLHashGetAllRecord(utShmHead *psShmHead,short nLnkNo,
                  char *pHead,
                  int pasMemCompareFun(char *pHead,char *ps),
                  int pasMemSort(char **ps1,char **ps2));
pasMemRecordHead *utShmHashGetAllRecord(utShmHead *psShmHead,short nLnkNo,
                  char *pHead,
                  int pasMemCompareFun(char *pHead,char *ps),
                  int pasMemSort(char **ps1,char **ps2));
#endif


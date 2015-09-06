#ifndef __UTOSTRHEAD_
#define __UTOSTRHEAD_
#define UT_STR_LEFTHANZI   0
#define UT_STR_RIGHTHANZI  1
#define UT_STR_ISASCII     2

#define PAS_SYS_SHUTDOWN   0
#define PAS_SYS_RESTART    6
/* ´íÎó´úÂë  */
#define UT_STR_ERRMSG01    02100
#define UT_STR_ERRMSG02    02200
#define UT_STR_ERRMSG03    02300
#define UT_STR_ERRMSG04    02400       
#define utStrBitIs(x,y) ( (x>>(y-1))&1 )
#include <stdio.h>
#include <string.h>
#include "pasfile.h"
#include "pasutl.h"

typedef struct {
    long lRand;
    long lNumber;
} utStrRandNumber;

struct utStrCnfList_S {
    char caVar1[32];
    char caVar2[32];
    struct utStrCnfList_S *psNext;
};
    
typedef struct utStrCnfList_S utStrCnfList;

struct utStrCnfList3_S {
    char caVar1[32];
    char caVar2[32];
    char caVar3[32];
    struct utStrCnfList3_S *psNext;
};
    
typedef struct utStrCnfList3_S utStrCnfList3;

struct utStrCnfList4_S {
    char caVar1[32];
    char caVar2[32];
    char caVar3[32];
    char caVar4[32];
    struct utStrCnfList4_S *psNext;
};
    
typedef struct utStrCnfList4_S utStrCnfList4;

/* utostr01.c */
int pasSetSystemNode(int iMaxNode,int iNode);
unsigned long pasbBGetNextSid(unsigned long lSid);
unsigned long long pasbBGetNextSid8(unsigned long long lSid);
char *utStrDelSpaces(char *pInstr);
int utStrToUpper(char *pInstr);
int utStrToLower(char *pInstr);
int utStrCvtLower(char *pInstr,char *pOut,int iMax);
char *utStrUpper(char *pInstr);
char *utStrLower(char *pInstr);
char *utStrGetWord(char *p,char *pOut,
                   short nMaxLen,char *pTerm);
char *utStrGotoTerm(char *p,char *pTerm);
char *utStrGetWordByC(char *p,char *pOut,
                   short nMaxLen,char c);
char *utStrGetWord1(char *p,char *pOut,
                   short nMaxLen,char *pTerm);
char *utStrSkipSpaces1(char *pIn);
int utStrIsSpaces1(char *pIn);
int utStrCharIsIn(char *pTerm,char c);
int utStrIsSpaces(char *pIn);
int utStrIsSpacesl(char *pIn,int l);
char *utStrSkipSpaces(char *pIn);
char *utStrSkipSpaces2(char *pIn);
int utStrHanZi(char *pInstr,int iPos);
char *utStrGetHanZi(char *pIn,int iPos);
int utStrRand(long iNumber,utStrRandNumber *psStrRand);
int utStrCompare_001(const void *p1,const void *p2);
char *utStrAddOneStr(char *pOutStr,char *pInstr,int iLen);
char *utStrncpy(char *pOutstr,char *pInstr,int iLen);
char *utStrncpy0(char *pOutstr,char *pInstr,int iLen);
char *utStrGetCharHan(char *pInstr,int iLen);
long utStrGetId();
int utStrIsDigital(char *InStr);
long utStrCvtHtol(char *In);
int utStrIsHex(char *InStr);
int utStrIsPrint(char *InStr);
int utStrIsMailAddr(char *InStr);
int utStrGet_l(char *pPrompt,long *lReturn);
int utStrGet_s(char *pPrompt,char *pOut);
int utStrGetd_l(char *pPrompt, long *lReturn);
int utStrGetd_s(char *pPrompt,char *pOut);
int utStrGetd_d(char *pPrompt,char *pFormat,long *lOut);
int utStrCountChar(char *pIn,char c);
int utStrIsNull(char *pIn,int iLen);
char *utStrLtoF8(long8 lValue,char *pValue);
char *utStrLtoF(long lValue,char *pValue);
int pasStrStrIs(char *pSrc,char *pStr,int cond);
int utStrCvtEsc(char *pIn,char *caOut);
unsigned long utStrCvt8(unsigned long l);
int utStr2IpPort(char *pIp,unsigned long *lIp,unsigned short *nPort);
int pasStrOrStr(char *pCont,char *pSub);
int pasStrAndStr(char *pCont,char *pSub);
char *pasStrGoToStr(char *pCont,char *pSub);
void utStrXString(char *p1,char *p2,int l);
void utStrXLong(unsigned long *l1,unsigned long *l2);
void utStrXShort(unsigned short *l1,unsigned short *l2);
char *pasCvtMac(unsigned char *caMac);
int pasCvtMacI(unsigned char *caMacStr,unsigned char *caMac);
/* utostr02.c */
unsigned long long utLongTime();
unsigned long long pasTimeM();
unsigned long long pasTimeU();
long utTimTime();
char *utTimFormat1(char *pFormat,long lTime);
char *utTimFormat(char *pFormat,long lTime);
char *utTimLongToStr(char *pFormat,long lTime,char *pStr);
unsigned long utTimStrToLong(char *fmt,char *instr);
int utTimStr10To8(char *pInTime,char *pFormat);
int utTimNextDay(char *caInDate,char *caOutDate);
int utTimPrevDay(char *caInDate,char *caOutDate);
unsigned long utTimNextDayl(unsigned long lTime0);
unsigned long utTimThisDayStart(unsigned long lTime0);
unsigned long utTimPrevDayl(unsigned long lTime0);
int utTimIsValidDate(int iYear,int iMonth,int iDay);
int utTarTxtFile(char *pTarFile,char *pFlst);
int utTxtDos2Unix(char *pDosFile,char *pUnixFile);
int utTxtFileFormatIsDos(char *pFile);
int utTxtUnix2Dos(char *pUnixFile,char *pDosFile);
int utTarDirFile(char *pTarFile,char *pPath,char *pFlst);
int utStrGetUploadName(char *pIn,char *pPath,char *pFile);
int pasUtlUnzipFile(char *pZipFile,char *pPath);
int pasUtlZipFileP(char *pZipFile,char *pPath,char *pFileList,char *pPass);
int pasUtlUnzipFileP(char *pZipFile,char *pPath,char *pPass);
int pasUtlMountSmbFs(char *pIp,char *pHost,
             char *pShare,char *pUsername,
             char *pPassword,char *pMount);
int pasUtlMountCifFs(char *pIp,char *pHost,
             char *pShare,char *pUsername,
             char *pPassword,char *pMount);
int pasUtlUmountSmbFs(char *pMount);
int pasUtlCheckMount(char *pMount);
int pasGetCpuRate(char *cmd,double *cpu,double *mem);
int pasGetMemInfo(unsigned long  *mem, unsigned long *freemem,
            unsigned long *swap,unsigned long *freeswap);
unsigned long  ncStrGetThatDays(unsigned long lTime);
unsigned long  utStrGetThatDayTime(unsigned long lTime);
/* utostr03.c */
int utStrSetNull(int iSum,...);
int utStrAddF(char *caOut,char *pFormat,...);
char *utStrCompose(char *caOut,char *pFormat,...);
int utStrnAddF(char *caOut,int iMax,char *pFormat,...);
int utStrBitIs1(unsigned char cChar,int iNum);
unsigned char utStrSetBit(unsigned char cChar,int iNum,int iValue);
int utStrSetStringBit(unsigned char *pIn,int iNum,int iValue);
int utStrStringBitIs(unsigned char *pIn,int iNum);
int utStrBitAnd(unsigned char *p1,unsigned char *p2,int iLen);
int utStrBitOr(unsigned char *p1,unsigned char *p2,int iLen);
char *pasDbRecordValue(char *caRecord,int iRow,int iSumField,...);
int utStrSetRecordValue(char *caRecord,int iSumField,...);
int utStrIsExistRecord(char *caRecord,int iStart,int iSumF,char *pValue);
int utStrIsValidIp(char *pIp);
char *utStrReplace(char *pBuf,char *pStart,char *pEnd,char *pDst);
int utStrReplace0(char *pBuf,char *pStart,char *pEnd,char *pDst);
int utStrReplaceWith(char *pBuf,char *s,char *d);
char *ncUtlGetWordBetween(char *pBuf,char *pStart,char *pEnd,char *caOut,int iMax);
char *utStrGetWordBetween(char *pBuf,char *pStart,char *pEnd,char *caOut,int iMax);
int ncUtlGetWordStartWith(char *pBuf,char *pStart,char *caOut,int iMax);
char *ncUtlGetWordNum(char *pBuf,int n,char *caOut,int iMax);
int utStrGetSomeWordBy(char *caRecord,char *pTerm,int iSum,...);
int utStrGetSomeWordBySp(char *caRecord,int iSum,...);
int utStrGetSomeNameValueBy(char *caRecord,char *pTerm,int iSum,...);
int pasUtlGetInt4(int lLen);
char *utStrCaseStr(char *pCont,char *pSub);
char *utStrCaseMemMem(char *pCont,int l1,char *pSub,int l2);
char *utStrCaseMemStr(char *pCont,int l,char *pSub);
char *utStrIsInCaseStrs(char *pCont,char *pSub);
char *utStrIsInStrs(char *pCont,char *pSub);
int utStrIsDoMain(char *caUrl);
int utStrIsMacAddress(char *pMac);
int utStrGetHttpHead(char *pStr,int iSum,...);
int utStrGetVarValue(char *pIn,char *pVar,char *pValue,char *pMid);
int utStrStrList(char *pCont,char *pSub);
int utStrIsValidMacAddress(char *p);
/* utostr04.c */
void utFileSetCurLine(int iLine);
int utFileGetCurLine();
utStrCnfList *utStrReadCnfFile(char *pConfFile);
void utStrFreeCnfList(utStrCnfList *psStrCnfList);
utStrCnfList4 *utStrReadCnfFile4(char *pConfFile);
void utStrFreeCnfList4(utStrCnfList4 *psStrCnfList);
utStrCnfList3 *utStrReadCnfFile3(char *pConfFile);
void utStrFreeCnfList3(utStrCnfList3 *psStrCnfList);
int utFileSkipSpaces(FILE *fp);
int utStrFreadUntilC(FILE *fp,char *caBuffer,int iMaxLen,char *pTerm);
int utSysShutdown(int iFlags);
/* utostr05.c */
char *wcrypt(char  *input,char  *key,int bIsEncrypt);
/* utostr06.c */
int utStrSplit(char *pIn,char *pTerm,char iNum,...);
/* utostr07.c */
unsigned long utFileSkipUntilStr(FILE *fp,char *pTerm,int lTerm);
int utFileReadWord(FILE *fp,
                char *caVar,int iVlen,
                char *caWord,int iMaxLen,int *iRetLen,
                char *pTerm,int iLen);
char *utFileReadWordUntil(FILE *fp,int iMaxLen,int *iRetLen,
                char *pTerm,int iLen);
int utFileReadWordTermByChar(FILE *fp,
                char *caVar,int iVlen,
                char *caWord,int iMaxLen,int *iRetLen,
                char *pTerm);
char *utFileReadAllFile(char *pFile,unsigned long *lSize);
int utStrGetThisWordTermByStr(char *pBuffer,int iBytes,char *caVar,int iVlen,
                char *caWord,char iMaxLen,int *iRetLen,
                char *pTerm,int iLen);
char *utStrGetThisWord(char *pBuffer,int iBytes,char *caVar,int iVlen,
                char *caWord,int  iMaxLen,int *iRetLen,
                char *pTerm,int iLen);
int utStrGetThisWordTermByChar(char *pBuffer,int iBytes,char *caVar,int iVlen,
                char *caWord,int iMaxLen,int *iRetLen,
                char *pTerm);
/* utostr08.c */
char *utStrDup(char *pIn);
char *utStrnDup(char *pIn,int iLen);
void utStrFree(char *pIn);
int pasExeSystem(char *pCommand);
/* utostr09.c */
void pasSetFileBuf(long lBufSize);
pasFile *pasOpenFile(char *pFile);
void pasCloseFile(pasFile *ps);
int pasReadFile(pasFile *ps,char *buf,int iLen);
int pasFseek(pasFile *ps,unsigned long lOffSet,int iFlags);
int pasReadLine(pasFile *ps,char *buf,int iMax);
int pasFeof(pasFile *ps);
long pasFilePos(pasFile *ps);
long pasFskipUntilStr(pasFile *ps,char *pTerm,int iTerm);
long pasFskipUntilChar(pasFile *ps,char *pTerm,int iTerm);
long pasReadFileUntilChar(pasFile *ps,char *buf,int iMax,char *pTerm,int iTerm);
long pasReadFileUntilStr(pasFile *ps,char *buf,int iMax,char *pTerm,int iTerm);
long pasFReadUntilStrW(pasFile *ps,FILE *fp,char *pTerm,int iTerm);
/* utostr10.c */
pasMemInfo *pasMemInit(int iMax,int iStep);
int pasMemFree(pasMemInfo *psMem);
char *pasMemAdd(pasMemInfo *psMem,char *pIn,int iLen);
char *pasMemDup(char *pIn,int iLen);
/* utostr11.c */
int pasHtmHtmlCvtCont(char *pCont,char *pOut);
/* utostr12.c */
unsigned char *pasStrCvtBin2Hex(unsigned char *pIn,int iLen,unsigned char *pOut);
unsigned char *pasStrCvtBin2Txt(unsigned char *pIn,int iLen,unsigned char *pOut);
unsigned char *pasStrCvtBin2Hex4(unsigned char *pIn,int iLen,unsigned char *pOut);
int pasStrCvtHex2Bin(unsigned char *pIn,unsigned char *pOut);
unsigned char  pasStrCvtX2C(char *what);
char *pasStrUrlUnEsc(char *url,char *out0);
char *pasStrUrlUnEsc0(char *url,char *out0);
unsigned char *pasStrUrlEsc(unsigned char *url,unsigned char  *out);
int pasStrIsHexDigital(char *pIn,int n);
int pasTimeIsDoTime(unsigned long *lTime,unsigned int lTimeStep,int lPos);
/* utostr015.c */
int utStrStrListCode(char *pCont,char *pSub,int iCode);
int utStrStrListAll(char *pCont,char *pSub);


#endif

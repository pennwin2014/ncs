/* 手机运营商
   
*/
#ifndef __PASMOBILEDEF__
#define __PASMOBILEDEF__



// 手机运营商定义
#define PAS_MOBILE_INVALID             0       /* 非国内手机              */
#define PAS_MOBILE_CHINAMOBILE         1       /* 中国移动  CHINAMOBILE   */
#define PAS_MOBILE_CHINAUNICOM         2       /* 联通      CHINAUNICOM   */
#define PAS_MOBILE_CHINATELECOM        3       /* 中国电信  CHINATELECOM  */
#define PAS_MOBILE_UNKNOW              8       /* 其它        */
#define PAS_MOBILE_INTERNATIONAL       9       /* 国际      INTERNATIONAL */



#define PAS_MOBILE_2G                  100     /* 2G        */
#define PAS_MOBILE_3G                  110     
#define PAS_MOBILE_G3                  120 
#define PAS_MOBILE_NOTEBOOK            130
#define PAS_MOBILESP_HEADLEN           24

#define PAS_MOBILE_UNKNOWAREA          999
// 运营商和制式
typedef struct pasMobileType_s {
    uint2 cType;
    uchar caName[14];
} pasMobileType;



// 手机号段对照表
typedef struct pasMobileSeg_s {
    int2   nNum;
    uchar  cType;
    uchar  cSp;
} pasMobileSeg;


typedef struct pasMobileSpHead_s {
    uint2           nId;                 // 文件ID
    uint2           nVersion;            // 版本
    uint4           lModiTime;           // 发行时间
    uint4           nSumNumber;          
    uint4           lSize;               // 内容长度
    uint4           lCheckSum;
    uint2           nSumType;            //
    uint2           nRev;
    uchar           *pBuf;
    pasMobileType   *psType;
    pasMobileSeg    *psSeg;
} pasMobileSpHead;

typedef struct pasMobileNum_s {
    uint4  number;             // 号码7位
    uint2  area;               // 区域2位
    uchar  cType;              // 移动类别 3G  2G
    uchar  cSp;                // 运营商
} pasMobileNum;


typedef struct pasMobileNumHead_s {
    uint2           nId;                 // 文件ID
    uint2           nVersion;            // 版本
    uint4           lModiTime;           // 发行时间
    uint4           nSumNumber;          // 号码总数
    uint4           lSize;               // 内容长度
    uint4           lCheckSum;
    uint2           nSumType;            // 区域总数
    uint2           nRev;
    uchar           *pBuf;
    pasMobileType   *psType;
    pasMobileNum    *psNum;
} pasMobileNumHead;

// 国外手机
typedef struct pasMobileFor_s {
    uint2  areanum;            // 区号
    uchar  lenmin;
    uchar  lenmax;
    uchar  prenum[12];         // 前缀
    uchar  cname[32];
    uchar  ename[32];
} pasMobileFor;


int pasCheckMobileNumber(char *pStr,char *pSpname,char *pType);
int pasUtlFreeMobileSp();
int pasUtlFreeMobileArea();
int pasUtlLoadMobileArea(char *pFile);
int pasCheckMobileArea(char *pStr,char *pSpname,char *pType,char *pArea);
int pasUtlLoadMobileAreaFromTxtFile(char *pFile,char *pOutFile,uint4 lMaxType,uint4 lMaxNumber);
int utStrIsMobileNumber(char *pStr);
int pasUtlLoadMobileAreaFromTxtFile(char *pFile,char *pOutFile,uint4 lMaxType,uint4 lMaxNumber);
#endif

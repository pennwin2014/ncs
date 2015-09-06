// 压缩文件系统
#ifndef  ___PASZLDFILE_DEF___
#define  ___PASZLDFILE_DEF___

#define PAS_ZLDFILEID     1269558547L

typedef struct pasZldHead_s {
    uint4  lId;                    // 文件开始标志
    uint4  lSlen;                  // 压缩前字节数
    uint4  lDlen;                  // 压缩后字节数
} pasZldHead;


typedef struct pasZldRecord_s {
    uint4  lSlen;                  // 压缩前字节数
    uint4  lDlen;                  // 压缩后字节数
} pasZldRecord;


#define PAS_ZLDMODE_READ       1
#define PAS_ZLDMODE_WRITE      2
#define PAS_ZLDMODE_APPEND     3

typedef struct pasZldFile_s {
    pasZldHead    sHead;
    FILE          *fp;             // 文件标识
    int           mode;            // 
    int           isNew;           // 0-- 旧文件  1--新文件
} pasZldFile;


#endif
/* PAS中文件操作部份  */
#ifndef __PASFILE_DEF_
#define __PASFILE_DEF_
typedef struct pasFile_s {
    int            iFd;               /* 文件句炳                          */
    char           *pBuf;             /* 文件缓冲区                        */
    unsigned long  lBufLen;           /* 文件缓冲区的大小，缺省为 8192     */
    unsigned long  lOffSet;           /* 文件当前位置在缓冲区中的偏移量    */
    unsigned long  lBufStart;         /* 缓冲区第一个指针对应文件的位置    */
    unsigned long  lLen;              /* 缓冲区内容的大小                  */
    unsigned long  lSize;             /* 文件大小                          */
} pasFile;

#endif

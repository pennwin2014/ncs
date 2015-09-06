/* 创建于  2013/06/01
   一种基于 LZO压缩算法的专用压缩文件操作

   文件结构:
       Fid(4)+Slen(4)+Dlen(4)+Wlen+Buf, 以4字节为单位对齐

*/
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <zlib.h>
#include <dirent.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "minilzo.h"
#include "paszldfile.h"


#define IN_LEN      (128*1024ul)
#define OUT_LEN     (IN_LEN + IN_LEN / 16 + 64 + 3)

static unsigned char __LZO_MMODEL in  [ IN_LEN ];
static unsigned char __LZO_MMODEL out [ OUT_LEN ];
static int     iLZOInitFlags=0;
#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);


// pMode --  'r'   'a'
pasZldFile *ncZldOpen(char *pInfile,char *pMode)
{
    pasZldFile *zp;
    FILE        *fp;
    int4        iRead,iWrite;
    zp = (pasZldFile *)malloc(sizeof(pasZldFile));
    if(zp == NULL) {
        return NULL;
    }
    if(*pMode == 'r') {
        fp = fopen(pInfile,"r");
        if(fp == NULL) {
            free(zp);
            return NULL;
        }
        else {
            iRead = fread(&zp->sHead,1,sizeof(pasZldHead),fp);
            if(iRead != sizeof(pasZldHead) && zp->sHead.lId != PAS_ZLDFILEID) {
                fclose(fp);
                free(zp);
                return NULL;
            }
            else {
                zp->fp = fp;
                zp->mode = PAS_ZLDMODE_READ;
                return zp;
            }
        }
    }
    else if(*pMode == 'a') {
        fp = fopen(pInfile,"a+");
        if(fp == NULL) {
            free(zp);
            return NULL;
        }
        else {
            lSize = utFilFileSize(fp);
            if(lSize == 0) {
                memset(zp,0,pasZldFile);
                zp->sHead.lId = PAS_ZLDFILEID;
                iWrite = fwrite(&zp->sHead,1,sizeof(pasZldHead),fp);
                if(iWrite != sizeof(pasZldHead)) {
                    fclose(fp);
                    free(zp);
                    return NULL;
                }
                else {
                   zp->fp = fp;
                   zp->mode = PAS_ZLDMODE_APPEND;
                   zp->isNew = 1;
                   return zp;
                } 
            }
            else {
                memset(zp,0,pasZldFile);
                zp->sHead.lId = PAS_ZLDFILEID;
                zp->fp = fp;
                zp->mode = PAS_ZLDMODE_APPEND;
                zp->isNew = 0;
                return zp;
            }
        }
    }
    else if(*pMode == 'w') {
        fp = fopen(pInfile,"w+");
        if(fp == NULL) {
            free(zp);
            return NULL;
        }
        else {
            memset(zp,0,pasZldFile);
            zp->sHead.lId = PAS_ZLDFILEID;
            iWrite = fwrite(&zp->sHead,1,sizeof(pasZldHead),fp);
            if(iWrite != sizeof(pasZldHead)) {
                fclose(fp);
                free(zp);
                return NULL;
            }
            else {
               zp->fp = fp;
               zp->mode = PAS_ZLDMODE_WRITE;
               zp->isNew = 1;
               return zp;
            } 
        }
    }
    else {
        free(zp);
        return NULL;
    }
}

    
int4 ncZldClose(pasZldFile *zp)
{
    if(zp->mode == PAS_ZLDMODE_READ) {
        if(zp->fp) fclose(zp->fp);
        free(zp);
    }
    else if(zp->mode == PAS_ZLDMODE_WRITE) {
        fseek(zp->fp,0,SEEK_SET);
        fwrite(&zp->sHead,1,sizeof(pasZldHead),zp->fp);
        if(zp->fp) fclose(zp->fp);
        free(zp);
    }
    else if(zp->mode == PAS_ZLDMODE_APPEND) {
        fseek(zp->fp,0,SEEK_SET);
        if(zp->isNew == 0) {
            pasZldHead sHead;
            memset(&sHead,0,sizeof(sHead));
            fread(&sHead,1,sizeof(sHead),zp->fp);
            if(sHead->lId != PAS_ZLDFILEID) {
                if(zp->fp) fclose(zp->fp);
                free(zp);
                return (-1);
            }
            else {
                zp->sHead.lSlen += sHead.lSlen;
                zp->sHead.lDlen += sHead.lDlen;
                fseek(zp->fp,0,SEEK_SET);
                fwrite(&zp->sHead,1,sizeof(pasZldHead),zp->fp);
                if(zp->fp) fclose(zp->fp);
                free(zp);
            }
        }
        else {
            fwrite(&zp->sHead,1,sizeof(pasZldHead),zp->fp);
            if(zp->fp) fclose(zp->fp);
            free(zp);
        }
    }
    return 0;
}


int ncZldWrite(pasZldFile *zp,char *buf,int4 iLen)
{
    int4 r,iStart;
    p = buf;
    while(iLen > 0) {
        if(iLen > ZLD_MAX_LEN) {
            lIn = ZLD_MAX_LEN;
            iLen -= ZLD_MAX_LEN;
        }
        else {
            lIn = iLen;
            iLen = 0;
        }
        r = lzo1x_1_compress(p,lIn,out,&lOut,wrkmem);
        if (r == LZO_E_OK) {
            sRecord.lSlen = lIn;
            sRecord.lDlen = lOut;
            fwrite(&sRecord,1,sizeof(sRecord),zp->fp);
            iWrite += fwrite(out,1,lOut,zp->fp);
            zp->sHead.lSlen += lIn;
            zp->sHead.lDlen += lOut;
        }
        else
        {
            return (-1);
        }
        p += lIn;
    }
    return iWrite;
}


// 解压到指定的文件
int ncZldUncompress2Sld(pasZldFile *zp,FILE *fp)
{
    
    
    

}


//
int ncZldAdd2Zld(pasZldFile *sZp,pasZldFile *dZp)
{


}


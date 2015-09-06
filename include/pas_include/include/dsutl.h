#ifndef  ___DSUTL_DEF___
#define  ___DSUTL_DEF___

#define UT_TYPE_FLIST       30
#define _DS_MD5_CODEKEY      "pronetway"



/* 属性链表  */
typedef struct dsUtlAttrList_s {
    unsigned short nType;             /* 类型,                  */
    unsigned short nBytes;            /* 内容的字节数           */    
    unsigned long lAttr;        /* 属性名称 md5code(upper(name))  */
    char *pValue;               /* 属性值                 */
    struct dsUtlAttrList_s *psNext;
} dsUtlAttrList;

/* 属性   */
typedef struct dsUtlAttr_s {
    unsigned long  lCode;                      /* 代码用Md5计算获得   */
    char           caName[32];                 /* 功能名称            */
    char           caDispname[32];             /* 显示名称            */
    dsUtlAttrList  *psAttr;                    /* 属性的描述          */
    struct dsUtlAttr_s    *psNext;
} dsUtlAttr;


typedef struct dsUtlConf_s {
    dsUtlAttr *psFun;
    dsUtlAttr *psRight;
    dsUtlAttr *psApp;
    unsigned long lModitime;   /* 记录修改时间  */
} dsUtlConf;


/* dsutl001.c */
int dsUtlLoadConfig(char *pConfFile,dsUtlConf *psConf);
int dsUtlReadOneAttr(FILE *fp,dsUtlConf *psConf);
dsUtlAttr *dsUtlReadFunList(FILE *fp,dsUtlAttr *psAttr,char *pName);
int dsUtlFreeAttr(dsUtlAttr *psAttr);
dsUtlAttrList *dsUtlAttrAdd(dsUtlAttrList *psFun,char *pName,int iType,int iBytes,char *pValue);
dsUtlAttrList *dsUtlAttrAppend(dsUtlAttrList *psFun,char *pName,int iType,int iBytes,char *pValue);
dsUtlAttrList *dsUtlGetAttrByName(dsUtlAttrList *psFun,char *caName);
dsUtlAttrList *dsUtlGetAttrByCode(dsUtlAttrList *psFun,unsigned long lCode);
dsUtlAttr *dsUtlReadRightList(FILE *fp,dsUtlAttr *psAttr,char *pName);
dsUtlAttr *dsUtlReadRoleList(FILE *fp,dsUtlAttr *psAttr,char *pName);
dsUtlAttr *dsUtlReadApp(FILE *fp,dsUtlAttr *psAttr,char *pName);
int dsUtlPrintAttr(dsUtlAttr *psAttr);

/* dsutl002.c */
int _dsUtlSetFun(utShmHead *psShmHead,dsUtlConf *psConf);
int _dsUtlGetAttrList_l(dsUtlAttrList *psFun,char *pName);
char *_dsUtlGetAttrList_s(dsUtlAttrList *psFun,char *pName);
char *_dsUtlGetAttrList_v(dsUtlAttrList *psFun,long lCodee,int *iNum);
char *dsUtlNewRightValue(utShmHead *psShmHead,int *iSize);
int _dsUtlSetRightValueByRight(dsUtlConf *psConf,
            unsigned char *pRightValue,
            unsigned long lRightId);
int _dsUtlSetRightValueByRole(dsUtlConf *psConf,
            unsigned char *pRightValue,
            unsigned long lRoleId);
unsigned long *_dsUtlGetFunlistByRight(dsUtlConf *psConf, 
                           unsigned long lRightId,
                           int *iSum);
unsigned long *_dsUtlGetRightlistByRole(dsUtlConf *psConf, 
                           unsigned long lRoleId,
                           int *iSum);
long  dsUtlGetAppAttr_l(dsUtlConf *psConf, char *pAttrName);
char *dsUtlGetAppAttr_s(dsUtlConf *psConf, char *pAttrName);                           

#endif

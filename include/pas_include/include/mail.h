#ifndef _X_MAIL_HEAD_
#define _X_MAIL_HEAD_

/*
 * Name:	mail_header.h
		解析邮件的头信息
*/

typedef struct xContent{
	char *pContent;
	unsigned long len;
	unsigned long lSize;
}mailContent;


typedef struct
{
        int             isnew;                  /* new is 1 or not new is 0     */
        char            *addressF;              /* address */
        char            *personF;
        char            *addressT;              /* address   */
        char            *personT;
        char            *addressCc;             /* address   */
        char            *personCc;
        char            *addressBcc;             /* address   */
        char            *personBcc;
        char            *subject;               /* subject item   */
        char            *date;                  /* date item      */
        unsigned        long size;              /* message size   */
        int             important;              /*msg important 0 1 2  */
}mail_header_info;

struct  mattach {
	char displayname[256];     /*要显示的附件名*/
        char filename[256];        /* 保存的附件名  */
        unsigned long filesize;         /* 附件大小    */
        char content_type[128];         /* 附件类型*/
        char cid[128];               /* 附件CID  */
        struct  mattach *next;       /* 下一个附件  */
} ;
typedef struct mattach  mail_attach;

typedef struct {
	    int  contype;
        char *textcon;
        char *htmlcon;
        char *charset;  
        mail_attach *attach;                      
} mail_body;


int conAdd_v(mailContent *hContent,char *pValue,unsigned long lLen);
mailContent *initContent(unsigned long  lMailSize);
int conCopy(mailContent *fContent,mailContent *tContent);


mail_header_info *mail_header(char *mailfn, int *errFlag);
int mailHeadFree(mail_header_info *mailHead);

/*参数说明：
mailfn： 邮件url
dir:     邮件解析后存放地址
prefile： 邮件解析后附件存放的前缀
precid：  cid替换 precid+邮件解析后附件存放的名称
errFlag=-1  文件不存在
errFlag=-2  解析信笺出错
-3	    rfc2047_decode_simple error
-4          lseek error
-5          生成文件名出错
-6          打开文件出错 
-7          写文件出错
-8       
-10        cid 替换   

*/

mail_body *mail_getbody(char *mailfn, char *dir, char *prefile, char *precid, int *errFlag);
void freeBody(mail_body *mBody);


/*打包与归档*/
/*
mimebuf:
	file1\nfile2\nfile3

fn:     要保存的路径
conflag: 0--Text  1--HTML
*/

int mail_digest(char *mimebuf,char *fn,char *from,char *to,char *subject,char *content,int conflag);

/*

编译：
gcc -o test test.c -I../include ../lib/maillib.a 

*/

/*headercache*/
/*消息头的缓存与处理*/
int newMailCache(char *maildir,char *mailfn);



#endif






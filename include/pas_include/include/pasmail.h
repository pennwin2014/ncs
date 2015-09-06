#ifndef  ___PAS_MAILDEF___
#define  ___PAS_MAILDEF___  
#define  PM_LOG_EVENT           0                 /* 事件                 */
#define  PM_LOG_ERROR           1                 /* 错误信息             */
#define  PM_LOG_MAILSEND        200               /* 邮件发送成功           */
#define  PM_LOG_MAILPUT         201               /* 将邮件放到队列成功     */
#define  PM_ERR_SMTPTCP         101               /* SMTP 网络连接有问题  */
#define  PM_ERR_INVALIDTO       102               /* 无效的收件人         */
#define  PM_ERR_NOMX            103               /* 找不到MX或域名解析问题  */
#define  PM_ERR_SMTPSRV         104               /* 邮件服务器连接出错     */
#define  PM_ERR_MSGFILE         105               /* 邮件内容不存在         */

#define  PAS_MAILQ_ID           20061129L
typedef struct pasMailQFile_s {
    long id;                  /* 文件ID     */
    long count;               /* 重发次数   */
    long lasttime;            /* 最后一次发送日期  */
    char to[128];
    char from[128];
} pasMailQFile;

#include "pasdef.h"    
/* pasutl_mail.c */
int pasMailSetESmtpAuth(int flags);
int pasMailSetSmtpAuth(char *pUser,char *pPass,char *pDomain);
char *get_mail_server( const char *remote, const char *local );
int pasSendMailBySmtp(char *msg,char *pTo,char *pFrom);
int pasSmtpSendSession(int iFd,char *pComm,...);
int pasCvtDosStr2Unix(char *pBuf,char *pBuf1,int l);
int pasMailDebug(int iFlags);
int pasMailSendStart();
int pasMailSendStart1();
int pasMailSendStart0();
int pasMailServerIsStart();
int pasMailLog(int status,char *pMsg,...);
int pasMailInitMailDir(char *pHome);
int pasSendMail(char *pMsg,char *pFrom,char *pTo);
int pasSendMailCont(char *pMsg,char *pFrom,char *pTo);

    
#endif

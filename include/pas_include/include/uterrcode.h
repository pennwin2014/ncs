#ifndef UTERRCODE_DEF
#define UTERRCODE_DEF

/* 错误代码  */
/* 错误代码由10位数字组成
   AA BBB C DDDD
   其中:
   AA  ---- 项目编号
   BBB ---- 项目中函数编号
   C   ---- 错误类别
   DDDD---- 具体的错误内容
*/

/* 项目编号  */
#define PROCODE_PICQSERVER    1      /* PICQ Server */
#define PROCODE_PICQONLINE    2      /* 联机服务器  */
#define PROCODE_UTILSHM       3      /* 实用函数    */
#define PROCODE_UTILSTR       4      
#define PROCODE_UTILCOM       5
#define PROCODE_UTILMSG       6
#define PROCODE_MEETSRV       7      /* 会议服务器  */


/* 函数代码  */
#define FUN_UTSHM_utStr01           1
#define FUN_pqcUtlReqNumber         10

#define FUN_utComTcpMsgSend         401
#define FUN_utComTcpMsgRead         402
#define FUN_utComTcpMsgRead000      403

/* 错误类型  */

#define ERR_TYPE_CODE         0    /* 本系统规定 */
#define ERR_TYPE_DBMS         1    /* SQLCODE    */
#define ERR_TYPE_SYSTEM       2    /* errorno    */
#define ERR_TYPE_USER         9    /* 自定义     */

/* 本系统内部使用   */
#define ERR_CODE_PACKAGE      1    /* 数据包出错     */
#define ERR_CODE_VARNUM       2    /* 变量个数不对   */
#define ERR_CODE_INVMSGCODE   3    /* 无效的数据代码 */
#define ERR_CODE_PICQOFFLINE  4    /* PICQ已离线     */
#define ERR_CODE_CONFIRMREAD  5    /* 确认信息出错   */
#define ERR_CODE_WRITEERR     6    /* 写出错         */


#endif

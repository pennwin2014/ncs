#ifndef __PUBLIC_H
#define __PUBLIC_H

#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include "utoall.h"
#include "utopc01.h"
#include "utotrs01.h"
#include "utoplt01.h"

/*---------------------------------------------------------------
  函数名称：pubExecSQL()
  功能说明：执行SQL语句，并自动做相应的处理。
            传入参数放在psMsgHead中(不需传入、传出参数时可使用NULL):
                CurPg -- 表示表格需要显示第CurPg页
                Row   -- 表示表格最大行数为RowPg
                Col   -- 表示显示二维列表，列数为Col
                以及用来绑定placeholder的变量
            传出参数追加在psMsgHead中，同时加入到psDbHead中:
                CurPg -- 表示表格需要显示第CurPg页
                TotPg  -- 总页数
                TotRec -- 总记录数
                Row   -- 表示表格最大行数为RowPg
                Col   -- 表示显示二维列表，列数为Col
                以及用来绑定placeholder的变量
                各select-list的值                
  参数说明：[in]pConn   -- DB_CONNECT结构指针
            [in]sql     -- SQL语句字符串，以'\0'结尾
            [in][out]psMsgHead -- utMsgHead结构指针
            [in][out]psDbHead  -- utPltDbHead结构指针
  返回值：  SQL语句错误号
---------------------------------------------------------------*/
int pubExecSQL(/*[in]*/DB_CONNECT* pConn, 
               /*[in]*/char* sql, 
               /*[in][out]*/utMsgHead** psMsgHead,
               /*[in][out]*/utPltDbHead* psDbHead);
               

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include "dsutl.h"
#include "dsclt.h"
#include "ncsdef.h"
#include"dlfcn.h"
extern char *pHashNettype;
extern char *pHashWebclass;
extern char *pHashService;
#include <iconv.h>
int getCharset(char *, int);


void ncsReplaceStr(char *pInput, char *pOutput, char *pSrc, char *pDst)
{
    char *pi, *po, *p;
    int nSrcLen, nDstLen, nLen;

    // 指向输入字符串的游动指针.
    pi = pInput;
    // 指向输出字符串的游动指针.
    po = pOutput;
    // 计算被替换串和替换串的长度.
    nSrcLen = strlen(pSrc);
    nDstLen = strlen(pDst);

    // 查找pi指向字符串中第一次出现替换串的位置,并返回指针(找不到则返回null).
    p = strstr(pi, pSrc);
    if(p) {
        // 找到.
        while(p) {
              // 计算被替换串前边字符串的长度.
            nLen = (int)(p - pi);
              // 复制到输出字符串.
            memcpy(po, pi, nLen);
            memcpy(po + nLen, pDst, nDstLen);
              // 跳过被替换串.
            pi = p + nSrcLen;
              // 调整指向输出串的指针位置.
            po = po + nLen + nDstLen;
              // 继续查找.
            p = strstr(pi, pSrc);
        }
        // 复制剩余字符串.
       strcpy(po, pi);
    }
    else
    {
        // 没有找到则原样复制.
       strcpy(po, pi);
    }
} 

static char *convert(const char *codefrom, const char *codeto, const char *src)
{
        char    to[32] = "";
        char    *out = NULL;
        char    *dst = NULL;

        iconv_t c;
        size_t  n_in  =  strlen(src);
        size_t  n_out =  2*n_in;

        snprintf(to, sizeof(to), "%s//IGNORE", codeto);
        if( (out=(char *)calloc(n_out+1, sizeof(char))) == NULL)
        {
                printf("allocate memory for %d bytes failed\n", n_out);
                return NULL;
        }
        dst=out;

        if((c = iconv_open(to, codefrom))==(iconv_t)(-1))
        {
                printf("codefrom = %s, codeto = %s, failed\n", codefrom, codeto);
                return NULL;
        }
        iconv(c, (char **)&src, &n_in, &dst, &n_out);
        iconv_close(c);
        return out;
}


char *convert_v4(const char *codefrom, const char *codeto, const char *src)
{
        char    to[32] = "";
        char    *out = NULL;
        char    *dst = NULL;

        iconv_t c;
        size_t  n_in  =  strlen(src);
        size_t  n_out =  2*n_in;

        snprintf(to, sizeof(to), "%s//IGNORE", codeto);
        if( (out=(char *)calloc(n_out+1, sizeof(char))) == NULL)
        {
                printf("allocate memory for %d bytes failed\n", n_out);
                return NULL;
        }
        dst=out;

        if((c = iconv_open(to, codefrom))==(iconv_t)(-1))
        {
                printf("codefrom = %s, codeto = %s, failed\n", codefrom, codeto);
                return NULL;
        }
        iconv(c, (char **)&src, &n_in, &dst, &n_out);
        iconv_close(c);
        return out;
}


char *my_convert(char *buf)
{
        if(buf == NULL)
                return NULL;
        int charset = getCharset(buf, strlen(buf));
        switch(charset)
        {
                case 2:
                        return convert_v4("UTF-8", "GBK", buf);
                default:
                        return strdup(buf);
        }
}


char **ncsUtlGetTable(unsigned long lTime, int days, char *prefix, unsigned long *lStartTime, int *lCount)
{
   	char sdate[33] = "";
   	strcpy(sdate, utTimFormat("%Y-%m-%d", lTime));//当前日期
   	strcat(sdate, " 00:00:00");
   	*lStartTime = utTimStrToLong("%Y/%m/%d %H:%M:%S", sdate) - (days-1) * 86400;
   	
   	unsigned long lStartTime1 = *lStartTime;
   	
   	int i = 0;
   	
   	char minYear[5] = "";
   	char maxYear[5] = "";
   	char minMon[3] = "";
   	char maxMon[3] = "";
   	
   	strcpy(minYear, utTimFormat("%Y", lStartTime1));
   	strcpy(maxYear, utTimFormat("%Y", lTime));
   	
   	strcpy(minMon, utTimFormat("%m", lStartTime1));
   	strcpy(maxMon, utTimFormat("%m", lTime));
   	
   	int iMin = atoi(minYear) * 12 + atoi(minMon);
   	int iMax = atoi(maxYear) * 12 + atoi(maxMon);
   	int strLen = strlen(prefix) + 7;
   	
   	*lCount = (iMax - iMin) + 1;
   	
   	 int a;
   	 int b;
   	 char **tmp = (char **)malloc(sizeof(char *)*((iMax - iMin) + 1));
   	 if(tmp != NULL)
   	 {
   	 	int j = 0;
   	 	for(i = iMin; i <= iMax; i++)
   	 	{
   	 		tmp[j] = (char *)malloc(strLen);
   	 		if(tmp[j] != NULL)
   	 		{
   	 			a = i/12;
   	 			b = i%12;
   	 			if(b == 0)
   	 			{
   	 				a--;
   	 				b = 12;
   	 			}
   	 			snprintf(tmp[j], strLen, "%s%04d%02d", prefix, a, b);
   	 		}
   	 		j++;
   	 	}	
   	 }
   	 return tmp;
}

char **ncsUtlGetTable2(char *sdate, char *edate, char *prefix, unsigned long *lStartTime, unsigned long *lEndTime, int *lCount)
{
   	char stime[15] = "";
   	char etime[15] = "";
   	
   	snprintf(stime, sizeof(stime), "%c%c%c%c%c%c%c%c000000", sdate[0], sdate[1], sdate[2], sdate[3], sdate[5], sdate[6], sdate[8], sdate[9]);
   	snprintf(etime, sizeof(stime), "%c%c%c%c%c%c%c%c235959", edate[0], edate[1], edate[2], edate[3], edate[5], edate[6], edate[8], edate[9]);
   	*lStartTime = utTimStrToLong("%Y%m%d%H%M%S", stime);
   	
   	unsigned long lTime = utTimStrToLong("%Y%m%d%H%M%S", etime);
   	*lEndTime = lTime;
   	
   	unsigned long lStartTime1 = *lStartTime;
   	
   	int i = 0;
   	
   	char minYear[5] = "";
   	char maxYear[5] = "";
   	char minMon[3] = "";
   	char maxMon[3] = "";
   	
   	strcpy(minYear, utTimFormat("%Y", lStartTime1));
   	strcpy(maxYear, utTimFormat("%Y", lTime));
   	
   	strcpy(minMon, utTimFormat("%m", lStartTime1));
   	strcpy(maxMon, utTimFormat("%m", lTime));
   	
   	int iMin = atoi(minYear) * 12 + atoi(minMon);
   	int iMax = atoi(maxYear) * 12 + atoi(maxMon);
   	int strLen = strlen(prefix) + 7;
   	
   	*lCount = (iMax - iMin) + 1;
   	
   	 int a;
   	 int b;
   	 char **tmp = (char **)malloc(sizeof(char *)*((iMax - iMin) + 1));
   	 if(tmp != NULL)
   	 {
   	 	int j = 0;
   	 	for(i = iMin; i <= iMax; i++)
   	 	{
   	 		tmp[j] = (char *)malloc(strLen);
   	 		if(tmp[j] != NULL)
   	 		{
   	 			a = i/12;
   	 			b = i%12;
   	 			if(b == 0)
   	 			{
   	 				a--;
   	 				b = 12;
   	 			}
   	 			snprintf(tmp[j], strLen, "%s%04d%02d", prefix, a, b);
   	 		}
   	 		j++;
   	 	}	
   	 }
   	 return tmp;
}



char **ncsUtlGetTable3(char *sdate, char *edate,char *stime1,char *etime1,char *prefix, unsigned long *lStartTime, unsigned long *lEndTime, int *lCount)
{
   	char stime[15] = "";
   	char etime[15] = "";
   	
   	snprintf(stime, sizeof(stime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", sdate[0], sdate[1], sdate[2], sdate[3], sdate[5], sdate[6], sdate[8], sdate[9],stime1[0],stime1[1],stime1[3],stime1[4],stime1[6],stime1[7]);
   	snprintf(etime, sizeof(stime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", edate[0], edate[1], edate[2], edate[3], edate[5], edate[6], edate[8], edate[9],etime1[0],etime1[1],etime1[3],etime1[4],etime1[6],etime1[7]);
   	*lStartTime = utTimStrToLong("%Y%m%d%H%M%S", stime);
   	
   	unsigned long lTime = utTimStrToLong("%Y%m%d%H%M%S", etime);
   	*lEndTime = lTime;
   	
   	unsigned long lStartTime1 = *lStartTime;
   	
   	int i = 0;
   	
   	char minYear[5] = "";
   	char maxYear[5] = "";
   	char minMon[3] = "";
   	char maxMon[3] = "";
   	
   	strcpy(minYear, utTimFormat("%Y", lStartTime1));
   	strcpy(maxYear, utTimFormat("%Y", lTime));
   	
   	strcpy(minMon, utTimFormat("%m", lStartTime1));
   	strcpy(maxMon, utTimFormat("%m", lTime));
   	
   	int iMin = atoi(minYear) * 12 + atoi(minMon);
   	int iMax = atoi(maxYear) * 12 + atoi(maxMon);
   	int strLen = strlen(prefix) + 7;
   	
   	*lCount = (iMax - iMin) + 1;
   	
   	 int a;
   	 int b;
   	 char **tmp = (char **)malloc(sizeof(char *)*((iMax - iMin) + 1));
   	 if(tmp != NULL)
   	 {
   	 	int j = 0;
   	 	for(i = iMin; i <= iMax; i++)
   	 	{
   	 		tmp[j] = (char *)malloc(strLen);
   	 		if(tmp[j] != NULL)
   	 		{
   	 			a = i/12;
   	 			b = i%12;
   	 			if(b == 0)
   	 			{
   	 				a--;
   	 				b = 12;
   	 			}
   	 			snprintf(tmp[j], strLen, "%s%04d%02d", prefix, a, b);
   	 		}
   	 		j++;
   	 	}	
   	 }
   	 return tmp;
}

static char *squeeze(char *s, int c)
{
        int i, j;
        for(i = j = 0; s[i]!= '\0'; i++)                                                                                                                       
        {                                                                                                                                                      
                if(s[i] != c)                                                                                                                                  
                {                                                                                                                                              
                        s[j++] = s[i];                                                                                                                         
                }                                                                                                                                              
        }                                                                                                                                                      
        s[j] = '\0';                                                                                                                                           
        return s;                                                                                                                                              
}
int ncsUserlogSearch2_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	int iTime_flag = 0;
	int days = 0;   
	int iret = 0;
	int _break = 0;
	int i = 0;
	int lCount = 0;
	int num = 0;
	int iNum = 0;
	
	unsigned long did = 0;
	unsigned long iStart = 0;
	unsigned long iLimit = 0;
	unsigned long lCount1 = 0;
	unsigned long lStartTime = 0;
   	unsigned long lTime = time(0);
   	unsigned long lTotRec = 0;
	
   	char sdate[11] = "";
   	char edate[11] = "";
   	char caStime[16];
   	char caEtime[16];
   	char time_flag[2] = "";
   	char start[17] = "";
   	char limit[17] = "";
   	char sort[17] = "";
   	char dir[9] = "";
   	char sql[4096] = "";
   	char _sip[16] = "";
   	char _idno[19] = "";
   	char _dispname[33] = "";
   	char corpname[129] = "";
   	char _groupid[33] = "";
   	char _did[33] = "";
   	char _uid[17] = "";
   	char instr[8192] = "";	
   	char sql2[256] = "";
   	char frmfile[256] = "";
   	char _position[33] = "";
   	char _mac[19] = "";
   	long lFlags;
   	char *tmpgbk = NULL;
	char **ptmp = NULL;	
	char _keyword[33] = "";
	long iReturn;
	char flag_desc[32];
	char caExport[32];
	
	
					  FILE *fp;
		    	char caFilename[32];
		    	char caFile[128];
		    	char caPath[128];
	
	
	struct sttable
	{
		char tname[32];
		unsigned long lCount;
	};
	struct sttable tableinfo[120];//10年？
   	
  char caGroups[1024];
  char caFacs[1024];
	 strcpy(caFacs,getDsFacDid());
   strcpy(caGroups,getDsGroupid());
   	
   	pasDbCursor *psCur = NULL;
   	utPltDbHead *psDbHead = utPltInitDb();
   	
	utMsgPrintMsg(psMsgHead);
   	utMsgGetSomeNVar(psMsgHead, 20,
   				     "keyword",  UT_TYPE_STRING,  sizeof(_keyword)-1, _keyword,
   				     "uid",	 UT_TYPE_STRING,  sizeof(_uid)-1,     _uid,
   				     "position", UT_TYPE_STRING,  sizeof(_position)-1, _position,
   				     "mac",      UT_TYPE_STRING,  sizeof(_mac)-1, _mac,
   				     "groupid",  UT_TYPE_STRING,  sizeof(_groupid)-1, _groupid,
   				     "did",      UT_TYPE_STRING,  sizeof(_did)-1,     _did,
   				     "corpname", UT_TYPE_STRING,  sizeof(corpname)-1, corpname,
   				     "sip",      UT_TYPE_STRING,  sizeof(_sip)-1,     _sip,   
   				     "idno",     UT_TYPE_STRING,  sizeof(_idno)-1,    _idno,
   				     "dispname", UT_TYPE_STRING,  sizeof(_dispname)-1, _dispname,
   				     "sdate", 	 UT_TYPE_STRING,  sizeof(sdate)-1,     sdate,
   				     "edate", 	 UT_TYPE_STRING,  sizeof(edate)-1,     edate,
   				     "time_flag",UT_TYPE_STRING,  sizeof(time_flag)-1, time_flag,
   				     "start",    UT_TYPE_STRING,  sizeof(start)-1,     start,
   				     "limit",    UT_TYPE_STRING,  sizeof(limit)-1,     limit,
   				     "sort",     UT_TYPE_STRING,  sizeof(sort)-1,      sort,
   				     "dir",      UT_TYPE_STRING,  sizeof(dir)-1,       dir,
   				     "exp",      UT_TYPE_STRING,  10,caExport,
   				     "stime",    UT_TYPE_STRING, 10,caStime,
   				     "etime",    UT_TYPE_STRING, 10,caEtime);
   				     
   				     if(strlen(caStime)==0){
   				     	strcpy(caStime,"00:00:00");
   				    }
   				    if(strlen(caEtime)==0){
   				    	strcpy(caEtime,"23:59:59");
   				    }
 if(strlen(sort)==0){
 	strcpy(sort,"stime");
 }
   	char *_dispname_gbk = NULL;
	if(!utStrIsSpaces(_dispname) )
	{
		_dispname_gbk = convert("UTF-8", "GBK", _dispname);
	}
	
   	iTime_flag = atoi(time_flag);
   	switch(iTime_flag)
   	{
   		case 1://今天
   			days = 1;
   			break;
   		case 2://最近三天
   			days = 3;
   			break;
   		case 3://最近一周
   			days = 7;
   			break;
   		case 4://最近一月
   			days = 30;
   			break;
   		case 5://最近三月
   			days = 90;
   			break;
   		case 6://最近半年
   			days = 180;
   			break;
   		default:
   			days = 1;
   			break;
   	}	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncsuserlog_if_", &lStartTime, &lTime, &lCount);
		
	}
	else 
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncsuserlog_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}
	if(!utStrIsSpaces(corpname))//指定了单位名称查询
	{
		tmpgbk = convert("UTF-8", "GBK", corpname);
		snprintf(sql2, sizeof(sql2), "select userid from ncsuser where (dispname like '%c%s%c' or username like '%c%s%c' )", '%',tmpgbk,'%', '%',tmpgbk,'%');
		free(tmpgbk);
		iNum=0;
		memset(instr,0,sizeof(instr));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			memset(instr,0,sizeof(instr));
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), ",");
				}
				snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
		
//		pasDbOneRecord(sql2, 0, UT_TYPE_ULONG, 4, &did);
	}
	if(!utStrIsSpaces(_did))
	{
		did = atol(_did);
	}
	if(!utStrIsSpaces(_groupid))
	{
		strcpy(instr,getGroupid(atol(_groupid)));
		/*
		snprintf(sql2, sizeof(sql2), "select userid from ncsuser where groupid in (select groupid from ncsgroup where groupid = %lu or pid=%lu) ", atol(_groupid),atol(_groupid));
		printf("sql2=%s\n",sql2);
		iNum=0;
		memset(instr,0,sizeof(instr));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), ",");
				}
				snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
		*/
	}
	char *_keyword_gbk = convert("UTF-8", "GBK", _keyword);
	char _sdate[11] = "";
	char _edate[11] = "";
	strcpy(_sdate, utTimFormat("%Y/%m/%d", lStartTime));
	strcpy(_edate, utTimFormat("%Y/%m/%d", lTime));
	
	for(i = 0; i < lCount; i++)
	{
		snprintf(frmfile, sizeof(frmfile), "/usr/local/infobright/data/ncs/%s.frm", ptmp[i]);
//		if(access(frmfile, F_OK) == 0)
//		{
			lCount1 = 0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu and (flags=1 or flags=2)",  ptmp[i], lStartTime, lTime);
			
			
			if(!utStrIsSpaces(_keyword))//0
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (position like '%%%s%%' or mac like '%%%s%%' or sip like '%%%s%%'  or dispname like '%%%s%%' or idno like '%%%s%%' )", _keyword_gbk, _keyword_gbk, _keyword_gbk, _keyword_gbk, _keyword_gbk);
			}
			if(!utStrIsSpaces(_position) )//1
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and position like '%%%s%%'", _position);
			}
			if(!utStrIsSpaces(_mac) )//2
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", _mac);
			}
			if(!utStrIsSpaces(_sip) )//3
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
			}
			if(!utStrIsSpaces(_idno) )//4
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
			}
			if(!utStrIsSpaces(_dispname) )//依据显示名查询
			{
				if(_dispname_gbk == NULL)
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
				}
				else 
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
				}
			}
			if(!utStrIsSpaces(corpname))//6
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
			}
			if(!utStrIsSpaces(_did))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
			}
			if(!utStrIsSpaces(_uid))//增加了按照uid查询
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
			}
			if(!utStrIsSpaces(_groupid))//8
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr);
			}
			if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
			
			printf("sql = %s\n", sql);
			lCount1=0;
			iReturn=pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
			if(iReturn!=1146&&lCount1 > 0)
			{
				strcpy(tableinfo[num].tname, ptmp[i]);
				tableinfo[num].lCount = lCount1;
				num++;
			}
//		}
		free(ptmp[i]);
	}
	free(ptmp);	
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	
	//
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	}
	//
	
	iNum = 0;
	if(strlen(caExport)>0){
		iStart=0;
		iLimit=99999;
				

		        sprintf(caFilename,"ncsuserlog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"用户上下线日志\n");
		        ncUtlPrintCsv(fp,7,UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"房间号/名称",
		                           UT_TYPE_STRING,"上网时间",
	//	                           UT_TYPE_STRING,"结束时间",
	                             UT_TYPE_STRING,"动作",
		                           UT_TYPE_STRING,"IP地址",
		                           UT_TYPE_STRING,"MAC地址");
				
			  
		
		
	}
	else{
	iStart = atol(start);
	iLimit = atol(limit);
  }
	for(i = num-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		snprintf(sql, sizeof(sql), "select uid, did, dispname, stime, etime, position, mac, sip, sumflow,flags from %s where stime >= %lu and stime <= %lu and (flags=1 or flags=2) ", tableinfo[i].tname, lStartTime, lTime);
		if(!utStrIsSpaces(_keyword))//0
		{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (position like '%%%s%%' or mac like '%%%s%%' or sip like '%%%s%%'  or dispname like '%%%s%%' or idno like '%%%s%%' )", _keyword_gbk, _keyword_gbk, _keyword_gbk, _keyword_gbk, _keyword_gbk);
		}
		if(!utStrIsSpaces(_position) )//1
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and position like '%%%s%%'", _position);
		}
		if(!utStrIsSpaces(_mac) )//2
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%'", _mac);
		}
		if(!utStrIsSpaces(_sip) )//3
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
		}
		if(!utStrIsSpaces(_idno) )//4
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
		}
		
		if(!utStrIsSpaces(_dispname) )//依据显示名查询
		{
			if(_dispname_gbk == NULL)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
			}
			else 
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
			}
		}
		if(!utStrIsSpaces(corpname))//6
		{
		  snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
		}
		if(!utStrIsSpaces(_uid))//增加了按照uid查询
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
		}
		if(!utStrIsSpaces(_did))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
		}
		if(!utStrIsSpaces(_groupid))//8
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr);
		}
		
		
		  if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
		
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
		printf("__line = %d, sql = %s\n", __LINE__, sql);
		
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			_break = 1;
		}
		psCur = pasDbOpenSql(sql, 0);
		if(psCur != NULL)
		{
			unsigned long uid = 0;
			unsigned long did = 0;
			char dispname[33] ="";
			unsigned long stime = 0;
			unsigned long etime = 0;
			char position[33] = "";
			char mac[19] = "";
			char sip[16] = "";
			unsigned long sumflow = 0;
			char corpname[256] = "";
			char s_time[21] = "";
			char e_time[21] = "";
      char caApmac[64],caSsid[36];
      char *p;
      long lCount_t;
      
			
			
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_ULONG,  4,              	&etime,
					 UT_TYPE_STRING, sizeof(position)-1,  	position,
					 UT_TYPE_STRING, sizeof(mac)-1, 	mac,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_ULONG,  4,              	&sumflow,
					 UT_TYPE_LONG,4,&lFlags);
			while((iret == 0) || (iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
				strcpy(corpname, ncsUtlGetClientNameById(psShmHead,did,"未知"));
				strcpy(s_time, utTimFormat("%Y/%m/%d %H:%M:%S", stime));
				strcpy(e_time, utTimFormat("%Y/%m/%d %H:%M:%S", etime));
				squeeze(dispname, '-');
				utPltPutLoopVarF(psDbHead,"corpname",iNum,"%s_%lu", corpname, did);
				
				utPltPutLoopVarF(psDbHead,"othlog_param",iNum,"%s_%s_%lu_%lu", _sdate, _edate, did, uid);
				utPltPutLoopVar(psDbHead,"sip",iNum,sip);
				utPltSetCvtHtml(1);
				utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s_%lu_%lu", dispname, did, uid);
				utPltPutLoopVar(psDbHead,"position",iNum,position);
				if(p=strstr(position,"-")){
		//			printf("ccc\n");
					memset(caApmac,0,sizeof(caApmac));
					memset(caSsid,0,sizeof(caSsid));
					strncpy(caApmac,position,strlen(position)-strlen(p));
					strcpy(caSsid,p+1);
		//			printf("caApmac=%s,caSsid=%s\n",caApmac,caSsid);
					  if(strlen(caApmac)>0){
						utPltPutLoopVar(psDbHead,"apmac",iNum,caApmac);
					 }
					 else{
					 	utPltPutLoopVar(psDbHead,"apmac",iNum,position);
					 }
						utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
					}
					else{
						utPltPutLoopVar(psDbHead,"apmac",iNum,position);
					}
			
				utPltSetCvtHtml(0);
				utPltPutLoopVar(psDbHead,"mac",iNum,mac);
				utPltPutLoopVar(psDbHead,"stime",iNum,s_time);
				if(lFlags==2){
				  utPltPutLoopVar(psDbHead,"etime",iNum,e_time);
			  }
				utPltPutLoopVarF(psDbHead,"sumflow",iNum, "%lu", sumflow);
				if(lFlags==1){
					strcpy(flag_desc,"上线");
					utPltPutLoopVar(psDbHead,"flags_desc",iNum,"上线");
				}
				else if(lFlags==2){
					strcpy(flag_desc,"下线");
					utPltPutLoopVar(psDbHead,"flags_desc",iNum,"下线");
				}
				
				if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,7,UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,dispname,
                          UT_TYPE_STRING,position,
                          UT_TYPE_STRING,s_time,
                          UT_TYPE_STRING,flag_desc,
                          UT_TYPE_STRING,sip,
                          UT_TYPE_STRING,mac);
				 }
				
				
				
				
				uid = did = stime = etime = sumflow = 0;
				memset(dispname, 0, sizeof(dispname));
				memset(sip, 0, sizeof(sip));
				memset(position, 0, sizeof(position));
				memset(mac, 0, sizeof(mac));
				iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_ULONG,  4,              	&etime,
					 UT_TYPE_STRING, sizeof(position)-1,  	position,
					 UT_TYPE_STRING, sizeof(mac)-1, 	mac,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_ULONG,  4,              	&sumflow,
					 UT_TYPE_LONG,4,&lFlags);
			}			 
			pasDbCloseCursor(psCur);
		} 
		if(_break == 1)
			break; 
	}
	free(_keyword_gbk);
	free(_dispname_gbk);
	
	
	 if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }
	
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/search/ncs_userlog_list.html");
	return iret;
}
int ncsServiceIndex_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	char groupid[33] = "";
	utMsgGetSomeNVar(psMsgHead, 1, "groupid", UT_TYPE_STRING, sizeof(groupid)-1, groupid);
	char sql[256] = "";
	snprintf(sql, sizeof(sql), "select count(*) from ncipindex where groupid = %lu", atol(groupid));
	printf("sql = %s\n", sql);
	unsigned long lCount = 0;
	pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount);
	utPltPutVarF(psDbHead, "TotRec", "%lu", lCount);
	
	snprintf(sql, sizeof(sql), "select  ncipindex.name, ncipindex.sid from ncipindex where groupid = %lu", atol(groupid));
	printf("sql = %s\n", sql);
	
	pasDbCursor *psCur = pasDbOpenSql(sql, 0);
	if(psCur != NULL)
	{
		int iNum = 0;
		char service[65] = "";
		unsigned long sid = 0;
		int iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(service)-1, service, UT_TYPE_ULONG, 4, &sid);
		while((iret == 0) || (iret == 1405))
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			utPltPutLoopVar(psDbHead, "name", iNum, service);
			utPltPutLoopVarF(psDbHead, "groupid", iNum, "%lu", sid);
			memset(service, 0, sizeof(service));
			sid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(service)-1, service, UT_TYPE_ULONG, 4, &sid);
		}
		pasDbCloseCursor(psCur);
	}
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_servicegroup_list.html");
	return 0;
}

int ncsServiceGroup_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	unsigned long lCount = 0;
	pasDbOneRecord("select count(distinct ncservicecgroup.groupid) from ncipindex,ncservicecgroup where ncipindex.groupid=ncservicecgroup.groupid", 0, UT_TYPE_ULONG, 4, &lCount);
	utPltPutVarF(psDbHead, "TotRec", "%lu", lCount);
	pasDbCursor *psCur = pasDbOpenSql("select ncservicecgroup.name, ncservicecgroup.groupid from ncipindex,ncservicecgroup where ncipindex.groupid=ncservicecgroup.groupid group by ncipindex.groupid", 0);
	if(psCur != NULL)
	{
		char name[33] = "";
		unsigned long groupid = 0;
		int iNum = 0;
		int iret = pasDbFetchInto(psCur,
						UT_TYPE_STRING, sizeof(name)-1, name,
						UT_TYPE_ULONG,  4,              &groupid);
		while((iret == 0) || (iret == 1405))
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			utPltPutLoopVar(psDbHead, "name", iNum, name);
			utPltPutLoopVarF(psDbHead, "groupid", iNum, "%lu", groupid);
			memset(name, 0, sizeof(name));
			groupid = 0;
			iret = pasDbFetchInto(psCur,
						UT_TYPE_STRING, sizeof(name)-1, name,
						UT_TYPE_ULONG,  4,              &groupid);
		}
		pasDbCloseCursor(psCur);
	}
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_servicegroup_list.html");
	return 0;
}
int ncsSearchCorp_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	char query[33] = "";
	utMsgGetSomeNVar(psMsgHead, 1, "query", UT_TYPE_STRING, sizeof(query)-1, query);
	char sql[128] = "";
	char *tmp_gbk = convert("UTF-8", "GBK", query);
	snprintf(sql, sizeof(sql), "select count(*) from ncsuser where dispname like '%%%s%%'", tmp_gbk);
	unsigned long lCount = 0;
	pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount);
	utPltPutVarF(psDbHead, "TotRec", "%lu", lCount);
	snprintf(sql, sizeof(sql), "select username, dispname from ncsuser where dispname like '%%%s%%'", tmp_gbk);
	
	free(tmp_gbk);
	pasDbCursor *psCur = pasDbOpenSql(sql, 0);
	if(psCur != NULL)
	{
		char username[15] = "";
		char dispname[129] = "";
		int iNum = 0;
		int iret = pasDbFetchInto(psCur,
						UT_TYPE_STRING, sizeof(username)-1, username,
						UT_TYPE_STRING, sizeof(dispname)-1, dispname);
		while((iret == 0) || (iret == 1405))
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			utPltPutLoopVar(psDbHead, "username", iNum, username);
			utPltPutLoopVar(psDbHead, "dispname", iNum, dispname);
			memset(username, 0, sizeof(username));
			memset(dispname, 0, sizeof(dispname));
			iret = pasDbFetchInto(psCur,
						UT_TYPE_STRING, sizeof(username)-1, username,
						UT_TYPE_STRING, sizeof(dispname)-1, dispname);
			
		}				
		pasDbCloseCursor(psCur);
	}
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_corp_list.html");
	return 0;
}


int ncsNetidSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  ncsNettype *psData;
  char caType[128];
  char caExport[32];
  FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];
  			unsigned long lIp;
			long iReturn;
			char caTemp[1024];
  	char caArea[256],caName2[256];
	int iret = 0;
	utMsgPrintMsg(psMsgHead);

   	utPltDbHead *psDbHead = utPltInitDb();
   	
   	char nname[33] = "";
   	char sdate[11] = "";
   	char edate[11] = "";
   	char time_flag[2] = "";
   	char start[17] = "";
   	char limit[17] = "";
   	char sort[17] = "";
   	char dir[9] = "";
   	char nettype[12] = "";
   	char sql[4096] = "";
   	
   	char _sip[16] = "";
   	char _dip[16] = "";
   	char _dispname[33] = "";
   	char _groupid[33] = "";
   	char _did[33] = "";
   	char _uid[17] = "";
   	char corpname[129] = "";
   	char _dispname_temp[36];
   	char _keyword[33] = "";
   	char _idno[33] = "";
   	char caStime[16];
   	char caEtime[16];
   	utMsgGetSomeNVar(psMsgHead, 21, 
   				     "idno",     UT_TYPE_STRING,  sizeof(_idno)-1,    _idno,
   				     "keyword",  UT_TYPE_STRING,  sizeof(_keyword)-1, _keyword,
   				     "uid",      UT_TYPE_STRING,  sizeof(_uid)-1,     _uid,
   				     "groupid",  UT_TYPE_STRING,  sizeof(_groupid)-1, _groupid,
   				     "did",      UT_TYPE_STRING,  sizeof(_did)-1,     _did,
   				     "corpname", UT_TYPE_STRING,  sizeof(corpname)-1, corpname,
   				     "sip",      UT_TYPE_STRING,  sizeof(_sip)-1,     _sip,  
   				     "dip",      UT_TYPE_STRING,  sizeof(_sip)-1,     _dip,  
   				     "dispname", UT_TYPE_STRING,  sizeof(_dispname)-1, _dispname,
   				     
   				     "nettype",  UT_TYPE_STRING,  sizeof(nettype)-1, nettype,
   				     "nname", 	 UT_TYPE_STRING,  sizeof(nname)-1, nname,
   				     "sdate", 	 UT_TYPE_STRING,  sizeof(sdate)-1, sdate,
   				     "edate", 	 UT_TYPE_STRING,  sizeof(edate)-1, edate,
   				     "time_flag",UT_TYPE_STRING,  sizeof(time_flag)-1, time_flag,
   				     "start",    UT_TYPE_STRING,  sizeof(start)-1, start,
   				     "limit",    UT_TYPE_STRING,  sizeof(limit)-1, limit,
   				     "sort",     UT_TYPE_STRING,  sizeof(sort)-1, sort,
   				     "dir",      UT_TYPE_STRING,  sizeof(dir)-1,  dir,
   				     "exp",      UT_TYPE_STRING,  10,caExport,
   				     "stime",    UT_TYPE_STRING, 10,caStime,
   				     "etime",    UT_TYPE_STRING, 10,caEtime);
   		
   	   				if(strlen(caStime)==0){
   				     	strcpy(caStime,"00:00:00");
   				    }
   				    if(strlen(caEtime)==0){
   				    	strcpy(caEtime,"23:59:59");
   				    }		     
   if(strlen(sort)==0){
 	   strcpy(sort,"stime");
   }			     
   	char *_dispname_gbk = NULL;
	if(!utStrIsSpaces(_dispname) )
	{
		_dispname_gbk = convert("UTF-8", "GBK", _dispname);
	}
   	if(utStrIsSpaces(nettype) )
   	{
   		strcpy(nettype, "0");
   	}			     
   	int iTime_flag = atoi(time_flag);
	int days = 0;   	
   	switch(iTime_flag)
   	{
   		case 1://今天
   			days = 1;
   			break;
   		case 2://最近三天
   			days = 3;
   			break;
   		case 3://最近一周
   			days = 7;
   			break;
   		case 4://最近一月
   			days = 30;
   			break;
   		case 5://最近三月
   			days = 90;
   			break;
   		case 6://最近半年
   			days = 180;
   			break;
   		default:
   			days = 1;
   			break;
   	}	
   	unsigned long lStartTime = 0;
   	unsigned long lEndTime = 0;
   	int i = 0;
	int lCount = 0;
	char **ptmp = NULL;	
	unsigned long lTime = time(0);
	char frmfile[256] = "";
	//int iNum = 0;
	pasDbCursor *psCur = NULL;
	int iNum = 0;
	int _break = 0;
	unsigned long did = 0;
	char instr[8192] = "";
	char instr_t[8192] = "";
	char *tmpgbk = NULL;
	char sql2[256] = "";
	
	struct sttable
	{
		char tname[32];
		unsigned long lCount;
	};
	struct sttable tableinfo[120];//10年？
	
	char caGroups[1024];
  char caFacs[1024];
  strcpy(caFacs,getDsFacDid());
  strcpy(caGroups,getDsGroupid());
	
	
	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncsnetlog_if_", &lStartTime, &lEndTime, &lCount);
		lTime = lEndTime;
	}
	else 
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncsnetlog_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}
	if(!utStrIsSpaces(corpname))//指定了单位名称查询
	{
		tmpgbk = convert("UTF-8", "GBK", corpname);
		snprintf(sql2, sizeof(sql2), "select userid from ncsuser where (dispname like '%c%s%c' or username like '%c%s%c' )", '%',tmpgbk,'%', '%',tmpgbk,'%');
		free(tmpgbk);
		iNum=0;
		memset(instr,0,sizeof(instr));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			memset(instr,0,sizeof(instr));
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), ",");
				}
				snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	if(!utStrIsSpaces(_did))
	{
		did = atol(_did);
	}
	if(!utStrIsSpaces(_groupid))
	{
		snprintf(sql2, sizeof(sql2), "select groupid from ncsgroup where  groupid = %lu or pid=%lu ", atol(_groupid),atol(_groupid));
		printf("sql2=%s\n",sql2);
		iNum=0;
		memset(instr_t,0,sizeof(instr_t));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), ",");
				}
				snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	
	char _sdate[11] = "";
	char _edate[11] = "";
	strcpy(_sdate, utTimFormat("%Y/%m/%d", lStartTime));
	strcpy(_edate, utTimFormat("%Y/%m/%d", lTime));
	
	int num = 0;
	unsigned long lCount1 = 0;
	char *_keyword_gbk = NULL;
	_keyword_gbk = convert("UTF-8", "GBK", _keyword);
	for(i = 0; i < lCount; i++)
	{
		snprintf(frmfile, sizeof(frmfile), "/usr/local/infobright/data/ncs/%s.frm", ptmp[i]);
//		if(access(frmfile, F_OK) == 0)
	//	{
			lCount1 = 0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu ",  ptmp[i], lStartTime, lTime);
			if(!utStrIsSpaces(_keyword))
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), "and (idno like '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or dispname like '%%%s%%' or name like '%%%s%%' or mac like '%%%s%%')", _keyword, _keyword, _keyword, _keyword, _keyword, _keyword);
			}
			if(atol(nettype)>0 )
			{
				if(strcmp(nettype, "1") == 0)//QQ账号
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ntype>=1001 and ntype<=1999 ");
				}
				else if(strcmp(nettype, "2") == 0)//game
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ntype >= 2000 and ntype <= 2999 ");
				}
				else if(strcmp(nettype, "13") == 0)//邮件账号
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ntype > 13000 and ntype < 13999 ");
				}
				else if(atol(nettype)>=20&&atol(nettype)<=25)//论坛
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ntype in (%s) ",getNetidByGid(atol(nettype)));
				}
				else if(strcmp(nettype, "5") == 0)//FTP账号
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ntype = 4002 ");
				}
				else 
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ntype = %s ",nettype);
				}
			}
			if(!utStrIsSpaces(_idno) )
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
			}
			if(!utStrIsSpaces(_sip) )
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
			}
			if(!utStrIsSpaces(_dip) )
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
			}
			if(!utStrIsSpaces(_dispname) )//依据显示名查询
			{
				if(_dispname_gbk == NULL)
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
				}
				else 
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%')", _dispname);
				}
			}	
			if(!utStrIsSpaces(nname) )
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and name like '%%%s%%'", nname);
			}
			if(!utStrIsSpaces(_did))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
			}
			if(!utStrIsSpaces(_uid))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
			}
			if(!utStrIsSpaces(_groupid))//8
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
			}
			
			if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
			
			
			if(!utStrIsSpaces(corpname))//6
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
			}
			
			tmpgbk = convert("UTF-8", "GBK", sql);
			if(tmpgbk != NULL)
			{
				printf("sql = %s\n", tmpgbk);
				pasDbOneRecord(tmpgbk, 0, UT_TYPE_ULONG, 4, &lCount1);
				free(tmpgbk);
			}
			if(lCount1 > 0)
			{
				strcpy(tableinfo[num].tname, ptmp[i]);
				tableinfo[num].lCount = lCount1;
				num++;
			}
	//	}
		free(ptmp[i]);
	}
	free(ptmp);	
	unsigned long lTotRec = 0;
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	
	
	//
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	}
	//
	

	
	
	
	memset(sql, 0, sizeof(sql));
	unsigned long iStart = atol(start);
	unsigned long iLimit = atol(limit);
	
	
		if(strlen(caExport)>0){
		
		iStart=0;
		iLimit=99999;
				

		        sprintf(caFilename,"ncsnetlog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"用户使用虚拟身份日志\n");
		        ncUtlPrintCsv(fp,9,UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"动作类型",
		                           UT_TYPE_STRING,"开始时间",
		                           UT_TYPE_STRING,"结束时间",
		                           UT_TYPE_STRING,"IP地址",
		                           UT_TYPE_STRING,"虚拟帐号",
		                           UT_TYPE_STRING,"虚拟帐号类别",
		                           UT_TYPE_STRING,"目标IP");
				
			  
		
		
	}
	
	
	
	for(i = num-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		snprintf(sql, sizeof(sql), "select uid, dispname, etime, name, ntype, stime, sip, dip, funcode, flag, did from %s where stime >= %lu and stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);
		if(!utStrIsSpaces(_keyword))
		{
snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), "and (idno like '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or dispname like '%%%s%%' or name like '%%%s%%' or mac like '%%%s%%')", _keyword, _keyword, _keyword, _keyword, _keyword, _keyword);
		}
			if(atol(nettype)>0 )
			{
				if(strcmp(nettype, "1") == 0)//QQ账号
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ntype>=1001 and ntype<=1999 ");
				}
				else if(strcmp(nettype, "2") == 0)//game
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ntype >= 2000 and ntype <= 2999 ");
				}
				else if(strcmp(nettype, "13") == 0)//邮件账号
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ntype > 13000 and ntype < 13999 ");
				}
				else if(atol(nettype)>=20&&atol(nettype)<=25)//论坛
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ntype in (%s) ",getNetidByGid(atol(nettype)));
				}
				else if(strcmp(nettype, "5") == 0)//FTP账号
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ntype = 4002 ");
				}
				else 
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and ntype = %s ",nettype);
				}
			}
			
		if(!utStrIsSpaces(_idno) )
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
		}
		if(!utStrIsSpaces(_sip) )
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
		}
		if(!utStrIsSpaces(_dip) )
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
		}
		if(!utStrIsSpaces(_dispname) )//依据显示名查询
		{
			if(_dispname_gbk == NULL)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
			}
			else 
			{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%')", _dispname);
			}
		}
		if(!utStrIsSpaces(nname) )
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and name like '%%%s%%'", nname);
		}
		if(!utStrIsSpaces(_did))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
		}
		if(!utStrIsSpaces(_uid))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
		}
		if(!utStrIsSpaces(_groupid))//8
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
		}
		
		 if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			
			}
		
		if(!utStrIsSpaces(corpname))//6
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
			}
		
		
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			_break = 1;
		}
		tmpgbk = convert("UTF-8", "GBK", sql);
		if(tmpgbk ==  NULL)
			continue;
		printf("__line = %d, sql = %s\n", __LINE__, tmpgbk);
		psCur = pasDbOpenSql(tmpgbk, 0);
		iNum=0;
		if(psCur != NULL)
		{
			char name[256] = "";
			unsigned long ntype = 0;
			unsigned long stime = 0;
			char sip[16] = "";
			char dip[16] = "";
			unsigned long funcode = 0;
			unsigned long flag = 0;
			unsigned long did = 0;
			char corpname[256] = "";
			char s_time[21] = "";
			char s_time2[21] = "";
			char dispname[33] = "";
			unsigned long etime = 0;
			unsigned long uid = 0;
			char caFun[16];
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,                  &uid,
					 UT_TYPE_STRING, sizeof(dispname)-1, dispname,
					 UT_TYPE_ULONG,  4,                  &etime,
					 UT_TYPE_STRING, sizeof(name)-1, name,
					 UT_TYPE_ULONG,  4,              &ntype,
					 UT_TYPE_ULONG,  4,              &stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  sip,
					 UT_TYPE_STRING, sizeof(dip)-1,  dip,
					 UT_TYPE_ULONG,  4,              &funcode,
					 UT_TYPE_ULONG,  4,              &flag,
					 UT_TYPE_ULONG,  4,              &did);
			while((iret == 0) || (iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
				strcpy(corpname, ncsUtlGetClientNameById(psShmHead,did,"未知"));
				strcpy(s_time, utTimFormat("%Y/%m/%d %H:%M:%S", stime));
				strcpy(s_time2, utTimFormat("%Y/%m/%d %H:%M:%S", etime));
				squeeze(name, '\r');
//		printf("name=%s\n",name);
				utPltPutLoopVar(psDbHead,"nname",iNum,name);
				utPltPutLoopVar(psDbHead,"stime",iNum, s_time);
				utPltPutLoopVar(psDbHead,"etime",iNum, s_time2);
				utPltPutLoopVar(psDbHead,"sip",iNum,sip);
				utPltPutLoopVar(psDbHead,"dip",iNum,dip);
				 lIp=ntohl(utComHostAddress(dip));
          iReturn = pasUtlGetIpLocation(lIp,caArea,caName2);
  //      printf("iReturn=%d,lIp=%lu,caArea=%s\n",iReturn,lIp,caArea);
          if(iReturn == 1) {
            sprintf(caTemp,"%s,%s",caArea,caName2);
    //        printf("caTemp=%s\n",caTemp);
          }
          else{
                strcpy(caTemp,"");
          }
//printf("caTemp=%s\n",caTemp);
        utPltPutLoopVar(psDbHead,"iplocal",iNum,caTemp);
				
				
				psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&ntype); 
	      if(psData){
	          strcpy(caType,psData->caName);
	      }
	      else{
	      	strcpy(caType,"");
	      }
	      utPltPutLoopVar(psDbHead,"ntype",iNum, caType);
			
//				utPltPutLoopVar(psDbHead,"ntype",iNum, ncsUtlGetServiceNameByType(ntype));
				utPltPutLoopVarF(psDbHead,"corpname",iNum,"%s_%lu", corpname, did);
				utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s_%lu_%lu", dispname, did, uid);
				utPltPutLoopVarF(psDbHead,"othlog_param",iNum,"%s_%s_%lu_%lu", _sdate, _edate, did, uid);
				strcpy(caFun,"");
				if(funcode == 1)
				{
					strcpy(caFun,"登录");
					utPltPutLoopVar(psDbHead,"action",iNum,"登录");
				}
				else if(funcode == 2)
				{
					strcpy(caFun,"退出");
					utPltPutLoopVar(psDbHead,"action",iNum,"退出");
				}
				
				if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,9,UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,dispname,
                          UT_TYPE_STRING,caFun,
                          UT_TYPE_STRING,s_time,
                          UT_TYPE_STRING,s_time2,
                          UT_TYPE_STRING,sip,
                          UT_TYPE_STRING,name,
                          UT_TYPE_STRING,caType,
                          UT_TYPE_STRING,dip);
				 }
				
				
				
				
				memset(sip, 0, sizeof(sip));
				memset(dip, 0, sizeof(dip));
				memset(name,0, sizeof(name));
				memset(dispname, 0, sizeof(dispname));
				ntype = stime = etime = funcode = flag = did = uid = 0;
				iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,                  &uid,
					 UT_TYPE_STRING, sizeof(dispname)-1, dispname,
					 UT_TYPE_ULONG,  4,              &etime,
					 UT_TYPE_STRING, sizeof(name)-1, name,
					 UT_TYPE_ULONG,  4,              &ntype,
					 UT_TYPE_ULONG,  4,              &stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  sip,
					 UT_TYPE_STRING, sizeof(dip)-1,  dip,
					 UT_TYPE_ULONG,  4,              &funcode,
					 UT_TYPE_ULONG,  4,              &flag,
					 UT_TYPE_ULONG,  4,              &did);
			}			 
			pasDbCloseCursor(psCur);
		} 
		free(tmpgbk);
		if(_break == 1)
			break; 
	}
	free(_keyword_gbk);
	free(_dispname_gbk);
	
	
		if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }
printf("aaaaaaaaaaaaaa\n");	
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/search/ncs_netid_list.html");
	return iret;
}

int ncsImlogSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  ncsNettype *psData;
  char caType[32];
  char caExport[32];
  FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];
	char _uid[17] = "";
	char _sdate[11] = "";
	char _edate[11] = "";
	int iTime_flag = 0;
	int days = 0;   
	int iret = 0;
	int _break = 0;
	int i = 0;
	int lCount = 0;
	int num = 0;
	int iNum = 0;
	
	unsigned long did = 0;
	unsigned long iStart = 0;
	unsigned long iLimit = 0;
	unsigned long lCount1 = 0;
	unsigned long lStartTime = 0;
   	unsigned long lTime = time(0);
   	unsigned long lTotRec = 0;
	
   	char sdate[11] = "";
   	char edate[11] = "";
   	char time_flag[2] = "";
   	char start[17] = "";
   	char limit[17] = "";
   	char sort[17] = "";
   	char dir[9] = "";
   	char sql[4096] = "";
   	char _sip[16] = "";
   	char _idno[19] = "";
   	char _dispname[33] = "";
   	char corpname[129] = "";
   	char _groupid[33] = "";
   	char _did[33] = "";
   	char instr[8192] = "";	
   	char instr_t[8192] = "";	
   	char sql2[256] = "";
   	char frmfile[256] = "";
   	char *tmpgbk = NULL;
	char **ptmp = NULL;	
	char _mesg[129] = "";
	char _service[32] = "";
	//char _dip[16] = "";
	//char *_service = NULL;
	char _from[33] = "";
	char _to[33] = "";
	char _action[2] = "";
	char _keyword[33] = "";
	  char caStime[16];
   	char caEtime[16];
	struct sttable
	{
		char tname[32];
		unsigned long lCount;
	};
	struct sttable tableinfo[120];//10年？
   	pasDbCursor *psCur = NULL;
  char caGroups[1024];
  char caFacs[1024];
	 strcpy(caFacs,getDsFacDid());
   strcpy(caGroups,getDsGroupid());
   	
   	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
   	utMsgGetSomeNVar(psMsgHead, 23,
   				     "keyword",  UT_TYPE_STRING,  sizeof(_keyword)-1,_keyword,
   				     "uid",      UT_TYPE_STRING,  sizeof(_uid)-1,    _uid,
   				     "mesg",     UT_TYPE_STRING,  sizeof(_mesg)-1,   _mesg,
   				     "action",   UT_TYPE_STRING,  sizeof(_action)-1, _action,
   				     "service",  UT_TYPE_STRING,  sizeof(_service)-1, _service,
   				     "from",     UT_TYPE_STRING,  sizeof(_from)-1,    _from,
   				     "to",       UT_TYPE_STRING,  sizeof(_to)-1,      _to,
   				     
   				     "groupid",  UT_TYPE_STRING,  sizeof(_groupid)-1, _groupid,
   				     "did",      UT_TYPE_STRING,  sizeof(_did)-1,     _did,
   				     "corpname", UT_TYPE_STRING,  sizeof(corpname)-1, corpname,
   				     "sip",      UT_TYPE_STRING,  sizeof(_sip)-1,     _sip,  
   				     "idno",     UT_TYPE_STRING,  sizeof(_idno)-1,    _idno,
   				     
   				     "dispname", UT_TYPE_STRING,  sizeof(_dispname)-1, _dispname,
   				     "sdate", 	 UT_TYPE_STRING,  sizeof(sdate)-1,     sdate,
   				     "edate", 	 UT_TYPE_STRING,  sizeof(edate)-1,     edate,
   				     "time_flag",UT_TYPE_STRING,  sizeof(time_flag)-1, time_flag,
   				     "start",    UT_TYPE_STRING,  sizeof(start)-1,     start,
   				     
   				     "limit",    UT_TYPE_STRING,  sizeof(limit)-1,     limit,
   				     "sort",     UT_TYPE_STRING,  sizeof(sort)-1,      sort,
   				     "dir",      UT_TYPE_STRING,  sizeof(dir)-1,       dir,
   				     "exp",      UT_TYPE_STRING,  10,caExport,
   				     "stime",    UT_TYPE_STRING, 10,caStime,
   				     "etime",    UT_TYPE_STRING, 10,caEtime);
   	char *_dispname_gbk = NULL;
   if(strlen(sort)==0){
 	   strcpy(sort,"stime");
   }	
   	   	   	   if(strlen(caStime)==0){
   				     	strcpy(caStime,"00:00:00");
   				  }
   				  if(strlen(caEtime)==0){
   				    	strcpy(caEtime,"23:59:59");
   				  }	
   	
	if(!utStrIsSpaces(_dispname) )
	{
		_dispname_gbk = convert("UTF-8", "GBK", _dispname);
	}			     
   	iTime_flag = atoi(time_flag);
   	switch(iTime_flag)
   	{
   		case 1://今天
   			days = 1;
   			break;
   		case 2://最近三天
   			days = 3;
   			break;
   		case 3://最近一周
   			days = 7;
   			break;
   		case 4://最近一月
   			days = 30;
   			break;
   		case 5://最近三月
   			days = 90;
   			break;
   		case 6://最近半年
   			days = 180;
   			break;
   		default:
   			days = 1;
   			break;
   	}	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncsimlog_if_", &lStartTime, &lTime, &lCount);
	}
	else 
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncsimlog_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}
	if(!utStrIsSpaces(corpname))//指定了单位名称查询
	{
		tmpgbk = convert("UTF-8", "GBK", corpname);
		snprintf(sql2, sizeof(sql2), "select userid from ncsuser where (dispname like '%c%s%c' or username like '%c%s%c' )", '%',tmpgbk,'%', '%',tmpgbk,'%');
		free(tmpgbk);
		iNum=0;
		memset(instr,0,sizeof(instr));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			memset(instr,0,sizeof(instr));
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), ",");
				}
				snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	if(!utStrIsSpaces(_did))
	{
		did = atol(_did);
	}
	if(!utStrIsSpaces(_groupid))
	{
		snprintf(sql2, sizeof(sql2), "select groupid from ncsgroup where  groupid = %lu or pid=%lu ", atol(_groupid),atol(_groupid));
		printf("sql2=%s\n",sql2);
		iNum=0;
		memset(instr_t,0,sizeof(instr_t));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), ",");
				}
				snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	char *_keyword_gbk = convert("UTF-8", "GBK", _keyword);
	strcpy(_sdate, utTimFormat("%Y/%m/%d", lStartTime));
	strcpy(_edate, utTimFormat("%Y/%m/%d", lTime));
	for(i = 0; i < lCount; i++)
	{
		snprintf(frmfile, sizeof(frmfile), "/usr/local/infobright/data/ncs/%s.frm", ptmp[i]);
//		if(access(frmfile, F_OK) == 0)
//		{
			lCount1 = 0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu ",  ptmp[i], lStartTime, lTime);
			
			if(!utStrIsSpaces(_keyword))
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (mfrom like '%%%s%%' or mto like '%%%s%%' or sip like '%%%s%%' or idno like '%%%s%%' or dispname like '%%%s%%' or mesg like '%%%s%%')", _keyword, _keyword, _keyword, _keyword, _keyword, _keyword);
			}
			if(!utStrIsSpaces(_service) && atol(_service) > 1000 )//1
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and service = %lu", atol(_service));
			}
			if(!utStrIsSpaces(_action) && atol(_action) > 0 )//1
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and funcode = %lu", atol(_action));
			}
			if(!utStrIsSpaces(_from) )
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mfrom like '%%%s%%'", _from);
			}
			if(!utStrIsSpaces(_to) )
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mto like '%%%s%%'", _to);
			}
			if(!utStrIsSpaces(_mesg) )//2
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mesg like '%%%s%%'", _mesg);
			}
			if(!utStrIsSpaces(_sip) )//源IP地址
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
			}
			if(!utStrIsSpaces(_idno) )//4
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
			}
			if(!utStrIsSpaces(_dispname) )//依据显示名查询
			{
				if(_dispname_gbk == NULL)
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
				}
				else 
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' )", _dispname);
				}
			}
			if(!utStrIsSpaces(corpname))//6
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
			}
			if(!utStrIsSpaces(_did))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
			}
			if(!utStrIsSpaces(_uid))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
			}
			if(!utStrIsSpaces(_groupid))//8
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
			}
			
		  if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			
			}
			
			
			tmpgbk = convert("UTF-8", "GBK", sql);
			if(tmpgbk != NULL)
			{
				printf("sql = %s\n", tmpgbk);
				pasDbOneRecord(tmpgbk, 0, UT_TYPE_ULONG, 4, &lCount1);
				free(tmpgbk);
			}
			if(lCount1 > 0)
			{
				strcpy(tableinfo[num].tname, ptmp[i]);
				tableinfo[num].lCount = lCount1;
				num++;
			}
//		}
		free(ptmp[i]);
	}
	free(ptmp);	
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	
	//
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	}
	//
	
	iNum = 0;
	iStart = atol(start);
	iLimit = atol(limit);
	
	
	if(strlen(caExport)>0){
		
		iStart=0;
		iLimit=99999;
				

		        sprintf(caFilename,"ncsimlog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"聊天日志\n");
		        ncUtlPrintCsv(fp,8,UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"IM类型",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"源IP",
		                           UT_TYPE_STRING,"发送人",
		                           UT_TYPE_STRING,"接收人",
		                           UT_TYPE_STRING,"内容");
		                         
				
			  
		
		
	}
	
	
	for(i = num-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		snprintf(sql, sizeof(sql), "select uid, did, dispname, stime, sip, service, mesg, mfrom, mto from %s where stime >= %lu and stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);
		if(!utStrIsSpaces(_keyword))
		{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (mfrom like '%%%s%%' or mto like '%%%s%%' or sip like '%%%s%%' or idno like '%%%s%%' or dispname like '%%%s%%' or mesg like '%%%s%%')", _keyword, _keyword, _keyword, _keyword, _keyword, _keyword);
		}
		if(!utStrIsSpaces(_service) && atol(_service) > 1000 )//1
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and service = %lu", atol(_service));
		}
		if(!utStrIsSpaces(_action) && atol(_action) > 0 )//1
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and funcode = %lu", atol(_action));
		}
		if(!utStrIsSpaces(_from) )
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mfrom like '%%%s%%'", _from);
		}
		if(!utStrIsSpaces(_to) )
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mto like '%%%s%%'", _to);
		}
		if(!utStrIsSpaces(_mesg) )//2
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mesg like '%%%s%%'", _mesg);
		}
		if(!utStrIsSpaces(_sip) )//源IP地址
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
		}
		if(!utStrIsSpaces(_idno) )//4
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
		}
		if(!utStrIsSpaces(_dispname) )//依据显示名查询
		{
			if(_dispname_gbk == NULL)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
			}
			else 
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
			}
		}
		if(!utStrIsSpaces(corpname))//6
		{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
		}
		if(!utStrIsSpaces(_did))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
		}
		if(!utStrIsSpaces(_uid))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
		}
		if(!utStrIsSpaces(_groupid))//8
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
		}
		 if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
		
		
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
		
		
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			_break = 1;
		}
		tmpgbk = convert("UTF-8", "GBK", sql);
		if(tmpgbk == NULL)
			continue;
		printf("__line = %d, sql = %s\n", __LINE__, tmpgbk);
		psCur = pasDbOpenSql(tmpgbk, 0);
		if(psCur != NULL)
		{
			//select uid, did, dispname, stime, sip, service, mesg, mfrom, mto from
			unsigned long uid = 0;
			unsigned long did = 0;
			char dispname[33] ="";
			unsigned long stime = 0;
			char sip[16] = "";
			unsigned long service = 0;
			char mesg[256] = "";
			char mfrom[49] = "";
			char mto[49] = "";
			
			char corpname[256] = "";
			char s_time[21] = "";
			char _service[33] = "";
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_ULONG,  4,              	&service,
					 UT_TYPE_STRING, sizeof(mesg)-1,        mesg,
					 UT_TYPE_STRING, sizeof(mfrom)-1,       mfrom,
					 UT_TYPE_STRING, sizeof(mto)-1,         mto);
			while((iret == 0) || (iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
				strcpy(corpname, ncsUtlGetClientNameById(psShmHead,did,"未知"));
				strcpy(s_time, utTimFormat("%Y/%m/%d %H:%M:%S", stime));
				squeeze(dispname, '-');
				utPltPutLoopVarF(psDbHead,"corpname",iNum,"%s_%lu", corpname, did);
				utPltPutLoopVar(psDbHead,"stime",iNum,s_time);
				utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s_%lu_%lu", dispname, did, uid);
				utPltPutLoopVarF(psDbHead,"othlog_param",iNum,"%s_%s_%lu_%lu", _sdate, _edate, did, uid);
				utPltPutLoopVar(psDbHead,"sip",iNum,sip);
				
				 utPltSetCvtHtml(1);
				utPltPutLoopVar(psDbHead,"from",iNum,mfrom);
				utPltPutLoopVar(psDbHead,"to",iNum,mto);
				utPltPutLoopVar(psDbHead, "mesg", iNum,  mesg);	
				 utPltSetCvtHtml(0);
				psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&service); 
	      if(psData){
	          strcpy(_service,psData->caName);
	      }
	      else{
	      	strcpy(_service,"");
	      }
				utPltPutLoopVarF(psDbHead,"did",iNum,"%d",did);
				utPltPutLoopVarF(psDbHead,"uid",iNum,"%d",uid);
				utPltPutLoopVarF(psDbHead,"service",iNum,"%d",service);
//				strcpy(_service, ncsUtlGetServiceNameByType(service));
				utPltPutLoopVar(psDbHead, "imtype", iNum,  _service);	
				
				
				if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,8,UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,dispname,
                          UT_TYPE_STRING,_service,
                          UT_TYPE_STRING,s_time,
                          UT_TYPE_STRING,sip,
                          UT_TYPE_STRING,mfrom,
                          UT_TYPE_STRING,mto,
                          UT_TYPE_STRING,mesg);
                         
				 }
				
				uid = did = stime = service = 0;
				memset(dispname, 0, sizeof(dispname));
				memset(sip, 0, sizeof(sip));
				memset(mesg, 0, sizeof(mesg));
				memset(mfrom, 0, sizeof(mfrom));
				memset(mto, 0, sizeof(mto));
				iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_ULONG,  4,              	&service,
					 UT_TYPE_STRING, sizeof(mesg)-1,        mesg,
					 UT_TYPE_STRING, sizeof(mfrom)-1,       mfrom,
					 UT_TYPE_STRING, sizeof(mto)-1,         mto);
			}			 
			pasDbCloseCursor(psCur);
		} 
		free(tmpgbk);
		if(_break == 1)
			break; 
	}
	free(_keyword_gbk);
	free(_dispname_gbk);
	
			if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }
	
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/search/ncs_imlog_list.html");
	return iret;
}

int ncsMailSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  char caType[32];
  char caExport[32];
  FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];
	char _uid[17] = "";
	char _sdate[11] = "";
	char _edate[11] = "";
	
	int iTime_flag = 0;
	int days = 0;   
	int iret = 0;
	int _break = 0;
	int i = 0;
	int lCount = 0;
	int num = 0;
	int iNum = 0;
	
	unsigned long did = 0;
	unsigned long iStart = 0;
	unsigned long iLimit = 0;
	unsigned long lCount1 = 0;
	unsigned long lStartTime = 0;
   	unsigned long lTime = time(0);
   	unsigned long lTotRec = 0;
	
   	char sdate[11] = "";
   	char edate[11] = "";
   	char time_flag[2] = "";
   	char start[17] = "";
   	char limit[17] = "";
   	char sort[17] = "";
   	char dir[9] = "";
   	char sql[4096] = "";
   	char _sip[16] = "";
   	char _idno[19] = "";
   	char _dispname[33] = "";
   	char corpname[129] = "";
   	char _groupid[33] = "";
   	char _did[33] = "";
   	char instr[8192] = "";	
   	char instr_t[8192] = "";
   	char sql2[256] = "";
   	char frmfile[256] = "";
   	char *tmpgbk = NULL;
	char **ptmp = NULL;	
	char _mfrom[49] = "";
	char _mto[49] = "";
	char _mailsub[256] = "";
	char _mailasize[8] = "";
	char _dip[16] = "";
	char _keyword[33] = "";
	long long lSid;
	long lCcode;
	struct sttable
	{
		char tname[32];
		unsigned long lCount;
	};
	struct sttable tableinfo[120];//10年？
   	pasDbCursor *psCur = NULL;
    char caGroups[1024];
   char caFacs[1024];
      	char caStime[16];
   	char caEtime[16];
	 strcpy(caFacs,getDsFacDid());
   strcpy(caGroups,getDsGroupid());	
   	
   	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
   	utMsgGetSomeNVar(psMsgHead, 23,
   				     "keyword",    UT_TYPE_STRING,  sizeof(_keyword)-1,    _keyword,
   				     "uid",        UT_TYPE_STRING,  sizeof(_uid)-1,        _uid,
   				     "mfrom",      UT_TYPE_STRING,  sizeof(_mfrom)-1,      _mfrom,
   				     "mto",        UT_TYPE_STRING,  sizeof(_mto)-1,        _mto,
   				     "mailsub",    UT_TYPE_STRING,  sizeof(_mailsub)-1,    _mailsub,
   				     "mailasize",  UT_TYPE_STRING,  sizeof(_mailasize)-1,  _mailasize,
   				     "dip",        UT_TYPE_STRING,  sizeof(_dip)-1,        _dip,  
   				     "groupid",    UT_TYPE_STRING,  sizeof(_groupid)-1,    _groupid,
   				     "did",        UT_TYPE_STRING,  sizeof(_did)-1,        _did,
   				     "corpname",   UT_TYPE_STRING,  sizeof(corpname)-1,    corpname,
   				     "sip",        UT_TYPE_STRING,  sizeof(_sip)-1,        _sip,  
   				     "idno",       UT_TYPE_STRING,  sizeof(_idno)-1,       _idno,
   				     "dispname",   UT_TYPE_STRING,  sizeof(_dispname)-1,   _dispname,
   				     "sdate", 	   UT_TYPE_STRING,  sizeof(sdate)-1,       sdate,
   				     "edate", 	   UT_TYPE_STRING,  sizeof(edate)-1,       edate,
   				     "time_flag",  UT_TYPE_STRING,  sizeof(time_flag)-1,   time_flag,
   				     "start",      UT_TYPE_STRING,  sizeof(start)-1,       start,
   				     "limit",      UT_TYPE_STRING,  sizeof(limit)-1,       limit,
   				     "sort",       UT_TYPE_STRING,  sizeof(sort)-1,        sort,
   				     "dir",        UT_TYPE_STRING,  sizeof(dir)-1,         dir,
   				     "exp",      UT_TYPE_STRING,  10,caExport,
   				      "stime",    UT_TYPE_STRING, 10,caStime,
   				     "etime",    UT_TYPE_STRING, 10,caEtime);
   	iTime_flag = atoi(time_flag);
   	
   	   	   	if(strlen(caStime)==0){
   				     	strcpy(caStime,"00:00:00");
   				  }
   				  if(strlen(caEtime)==0){
   				    	strcpy(caEtime,"23:59:59");
   				  }	
   if(strlen(sort)==0){
 	   strcpy(sort,"stime");
   }
   	switch(iTime_flag)
   	{
   		case 1://今天
   			days = 1;
   			break;
   		case 2://最近三天
   			days = 3;
   			break;
   		case 3://最近一周
   			days = 7;
   			break;
   		case 4://最近一月
   			days = 30;
   			break;
   		case 5://最近三月
   			days = 90;
   			break;
   		case 6://最近半年
   			days = 180;
   			break;
   		default:
   			days = 1;
   			break;
   	}	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncsmailmon_if_", &lStartTime, &lTime, &lCount);
	}
	else 
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncsmailmon_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}
	if(!utStrIsSpaces(corpname))//指定了单位名称查询
	{
		tmpgbk = convert("UTF-8", "GBK", corpname);
		snprintf(sql2, sizeof(sql2), "select userid from ncsuser where (dispname like '%c%s%c' or username like '%c%s%c' )", '%',tmpgbk,'%', '%',tmpgbk,'%');
		free(tmpgbk);
		iNum=0;
		memset(instr,0,sizeof(instr));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			memset(instr,0,sizeof(instr));
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), ",");
				}
				snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	if(!utStrIsSpaces(_did))
	{
		did = atol(_did);
	}
	if(!utStrIsSpaces(_groupid))
	{
		snprintf(sql2, sizeof(sql2), "select groupid from ncsgroup where  groupid = %lu or pid=%lu ", atol(_groupid),atol(_groupid));
		printf("sql2=%s\n",sql2);
		iNum=0;
		memset(instr_t,0,sizeof(instr_t));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), ",");
				}
				snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	
	
	char *_mailsub_gbk = NULL;
	char *_dispname_gbk = NULL;
	if(!utStrIsSpaces(_mailsub) )
	{
		_mailsub_gbk = convert("UTF-8", "GBK", _mailsub);
	}
	if(!utStrIsSpaces(_dispname) )
	{
		_dispname_gbk = convert("UTF-8", "GBK", _dispname);
	}
	
	strcpy(_sdate, utTimFormat("%Y/%m/%d", lStartTime));
	strcpy(_edate, utTimFormat("%Y/%m/%d", lTime));
	char *_keyword_gbk = NULL;
	_keyword_gbk = convert("UTF-8", "GBK", _keyword);
	for(i = 0; i < lCount; i++)
	{
		snprintf(frmfile, sizeof(frmfile), "/usr/local/infobright/data/ncs/%s.frm", ptmp[i]);
//		if(access(frmfile, F_OK) == 0)
//		{
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu ",  ptmp[i], lStartTime, lTime);
			
			if(!utStrIsSpaces(_keyword))
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (mailfrom like '%%%s%%' or mailto like '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or idno like '%%%s%%' or mailsub like '%%%s%%' or mailsub like '%%%s%%' or dispname like '%%%s%%')", _keyword, _keyword, _keyword, _keyword, _keyword, _keyword, _keyword_gbk, _keyword_gbk);
			}
			if(!utStrIsSpaces(_mfrom) )//依据发件人查询
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mailfrom like '%%%s%%'", _mfrom);
			}
			if(!utStrIsSpaces(_mto) )//依据收件人查询
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mailto like '%%%s%%'", _mto);
			}
			if(!utStrIsSpaces(_mailsub) )//依据邮件主题查询
			{
				if(_mailsub_gbk == NULL)
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mailsub like '%%%s%%'", _mailsub);
				}
				else 
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (mailsub like '%%%s%%' or mailsub like '%%%s%%')", _mailsub, _mailsub_gbk);
				}
			}
			if(!utStrIsSpaces(_dispname) )//依据显示名查询
			{
				if(_dispname_gbk == NULL)
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
				}
				else 
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
				}
			}
			if(atol(_mailasize) == 2)//有附件
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mailasize > 0");
			}
			else if(atol(_mailasize) == 1)//无附件
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mailasize = 0");
			}
			else 
			{
			}
			if(!utStrIsSpaces(_dip) )//目的IP地址
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
			}
			if(!utStrIsSpaces(_sip) )//源IP地址
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
			}
			if(!utStrIsSpaces(_idno) )//证件号码
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
			}
			if(!utStrIsSpaces(corpname))//6
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
			}
			if(!utStrIsSpaces(_did))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
			}
			if(!utStrIsSpaces(_uid))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
			}
			if(!utStrIsSpaces(_groupid))//8
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
			}
			if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
			
			lCount1 = 0;
			pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
			printf("sql = %s\n", sql);
			if(lCount1 > 0)
			{
				strcpy(tableinfo[num].tname, ptmp[i]);
				tableinfo[num].lCount = lCount1;
				num++;
			}
//		}
		free(ptmp[i]);
	}
	free(ptmp);	
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	
	//
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	}
	//
	
	iNum = 0;
	iStart = atol(start);
	iLimit = atol(limit);
	
if(strlen(caExport)>0){
		
		iStart=0;
		iLimit=99999;
				

		        sprintf(caFilename,"ncsmaillog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"邮件日志\n");
		        ncUtlPrintCsv(fp,8,UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"邮件类型",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"发件人",
		                           UT_TYPE_STRING,"收件人",
		                           UT_TYPE_STRING,"主题",
		                           UT_TYPE_STRING,"邮件大小");
		                         
				
			  
		
		
	}
	
	for(i = num-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		snprintf(sql, sizeof(sql), "select uid, did, dispname, stime, sip, dip, flags, mailfrom, mailto, fileid, mailsub, mailsize, mailasize,sid,ccode from %s where stime >= %lu and stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);
		if(!utStrIsSpaces(_keyword))
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (mailfrom like '%%%s%%' or mailto like '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or idno like '%%%s%%' or mailsub like '%%%s%%' or mailsub like '%%%s%%' or dispname like '%%%s%%')", _keyword, _keyword, _keyword, _keyword, _keyword, _keyword, _keyword_gbk, _keyword_gbk);
		}
		if(!utStrIsSpaces(_mfrom) )//依据发件人查询
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mailfrom like '%%%s%%'", _mfrom);
		}
		if(!utStrIsSpaces(_mto) )//依据收件人查询
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mailto like '%%%s%%'", _mto);
		}
		if(!utStrIsSpaces(_mailsub) )//依据邮件主题查询
		{
			if(_mailsub_gbk == NULL)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mailsub like '%%%s%%'", _mailsub);
			}
			else 
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (mailsub like '%%%s%%' or mailsub like '%%%s%%')", _mailsub, _mailsub_gbk);
			}
		}
		if(!utStrIsSpaces(_dispname) )//依据显示名查询
		{
			if(_dispname_gbk == NULL)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
			}
			else 
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
			}
		}
		if(atol(_mailasize) == 2)//有附件
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mailasize > 0");
		}
		else if(atol(_mailasize) == 1)//无附件
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mailasize = 0");
		}
		else 
		{
		}
		if(!utStrIsSpaces(_dip) )//目的IP地址
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
		}
		if(!utStrIsSpaces(_sip) )//源IP地址
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
		}
		if(!utStrIsSpaces(_idno) )//证件号码
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
		}
		if(!utStrIsSpaces(corpname))//6
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
		}
		if(!utStrIsSpaces(_did))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
		}
		if(!utStrIsSpaces(_uid))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
		}
		if(!utStrIsSpaces(_groupid))//8
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
		}
		if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
		
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			_break = 1;
		}
		printf("__line = %d, sql = %s\n", __LINE__, sql);
		psCur = pasDbOpenSql(sql, 0);
		if(psCur != NULL)
		{
			unsigned long uid = 0;
			unsigned long did = 0;
			char dispname[33] ="";
			unsigned long stime = 0;
			char sip[16] = "";
			char dip[16] = "";
			unsigned long flag = 0;
			char caTemp[1024];
			char mailfrom[256] = "";
			char mailto[256] = "";
			char mailfile[65] = "";
			char mailsub[256] = "";
			unsigned long mailsize = 0;
			unsigned long mailasize = 0;
			char corpname[256] = "";
			long long lFileid;
			char s_time[21] = "";
			long lCcode=0;
			lSid=0;
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_STRING, sizeof(dip)-1,  	dip,
					 UT_TYPE_ULONG,  4,                     &flag,
					 UT_TYPE_STRING, sizeof(mailfrom)-1,    mailfrom,
					 UT_TYPE_STRING, sizeof(mailto)-1,      mailto,
					 UT_TYPE_LONG8, 8,    &lFileid,
					 UT_TYPE_STRING, sizeof(mailsub)-1,     mailsub,
					 UT_TYPE_ULONG,  4,                     &mailsize,
					 UT_TYPE_ULONG,  4,                     &mailasize,
					 UT_TYPE_LONG8,8,&lSid,
					 UT_TYPE_LONG,4,&lCcode);
			while((iret == 0) || (iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
				strcpy(corpname, ncsUtlGetClientNameById(psShmHead,did,"未知"));
				strcpy(s_time, utTimFormat("%Y/%m/%d %H:%M:%S", stime));
				squeeze(dispname, '-');
				utPltPutLoopVarF(psDbHead,"corpname",iNum,"%s_%lu", corpname, did);
				utPltPutLoopVar(psDbHead,"stime",iNum,s_time);
				utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s_%lu_%lu", pasStrCvtUcode(dispname,caTemp), did, uid);
				utPltPutLoopVar(psDbHead,"sip",iNum,sip);
				utPltPutLoopVar(psDbHead,"dip",iNum,dip);

				utPltPutLoopVarF(psDbHead,"othlog_param",iNum,"%s_%s_%lu_%lu", _sdate, _edate, did, uid);
					utPltSetCvtHtml(1);
				utPltPutLoopVar(psDbHead,"mfrom",iNum,mailfrom);
				utPltPutLoopVar(psDbHead,"mto",iNum,mailto);
					
				
//printf("mailsub=%s\n",mailsub);				
			if(lCcode==1){
		//			utPltPutLoopVar(psDbHead, "mailsub", iNum,pasStrCvtUcode(pasBig5Togb2312(mailsub),caTemp));   
					     utPltPutLoopVar(psDbHead, "mailsub", iNum,pasBig5Togb2312(mailsub));        
			 }
			 else{
			 	tmpgbk = my_convert(mailsub);
			
			
	if(tmpgbk){
		printf("tmpgbk=%s\n",tmpgbk);
	}		
//				utPltPutLoopVar(psDbHead, "mailsub", iNum, pasStrCvtUcode(tmpgbk ? tmpgbk : " ",caTemp));/////////
		utPltPutLoopVar(psDbHead, "mailsub", iNum, tmpgbk ? tmpgbk : " ");/////////		
				free(tmpgbk);	
			}
	utPltSetCvtHtml(0);			
				
				if(flag == 1)
				{
					strcpy(caType,"SMTP");
					utPltPutLoopVar(psDbHead, "mtype", iNum,  "SMTP");	
				}
				else if(flag == 2)
				{
					strcpy(caType,"POP3");
					utPltPutLoopVar(psDbHead, "mtype", iNum,  "POP3");	
				}
				else if(flag == 3)
				{
					strcpy(caType,"WEBMAIL");
					utPltPutLoopVar(psDbHead, "mtype", iNum,  "WEBMAIL");	
				}
				else{
					strcpy(caType,"");
				}
				utPltPutLoopVarF(psDbHead,"did",iNum,"%d",did);
				utPltPutLoopVarF(psDbHead,"uid",iNum,"%d",uid);
				utPltPutLoopVarF(psDbHead, "mailsize", iNum, "%lu", mailsize);//邮件大小
				utPltPutLoopVar(psDbHead, "mailasize", iNum, mailasize ? "有" : "无");//有无附件
				utPltPutLoopVarF(psDbHead, "showmail", iNum, "%s;%lu;%llu;%llu", tableinfo[i].tname, stime,lFileid,lSid);
				
				if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,8,UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,dispname,
                          UT_TYPE_STRING,caType,
                          UT_TYPE_STRING,s_time,
                          UT_TYPE_STRING,mailfrom,
                          UT_TYPE_STRING,mailto,
                          UT_TYPE_STRING,mailsub,
                          UT_TYPE_LONG,mailsize);
                         
				 }
				
				uid = did = stime = flag = mailsize = mailasize = 0;
				
				memset(dispname, 0, sizeof(dispname));
				memset(sip, 0, sizeof(sip));
				memset(dip, 0, sizeof(dip));
				memset(mailfrom, 0, sizeof(mailfrom));
				memset(mailto, 0, sizeof(mailto));
				memset(mailsub, 0, sizeof(mailsub));
				memset(mailfile, 0, sizeof(mailfile));
				lFileid=0;
			lSid=0;
			lCcode=0;
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_STRING, sizeof(dip)-1,  	dip,
					 UT_TYPE_ULONG,  4,                     &flag,
					 UT_TYPE_STRING, sizeof(mailfrom)-1,    mailfrom,
					 UT_TYPE_STRING, sizeof(mailto)-1,      mailto,
					 UT_TYPE_LONG8, 8,    &lFileid,
					 UT_TYPE_STRING, sizeof(mailsub)-1,     mailsub,
					 UT_TYPE_ULONG,  4,                     &mailsize,
					 UT_TYPE_ULONG,  4,                     &mailasize,
					 UT_TYPE_LONG8,8,&lSid,
					 UT_TYPE_LONG,4,&lCcode);
			}			 
			pasDbCloseCursor(psCur);
		} 
		if(_break == 1)
			break; 
	}
	free(_keyword_gbk);
	free(_mailsub_gbk);
	free(_dispname_gbk);
	
	
	if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }
	
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/search/ncs_mail_list.html");
	return iret;
}


int ncsSearchlogSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	char caType[32];
  char caExport[32];
  FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];
	char instr_t[8192] = "";
	char _uid[17] = "";
	char _sdate[11] = "";
	char _edate[11] = "";
	int iTime_flag = 0;
	int days = 0;   
	int iret = 0;
	int _break = 0;
	int i = 0;
	int lCount = 0;
	int num = 0;
	int iNum = 0;
	
	unsigned long did = 0;
	unsigned long iStart = 0;
	unsigned long iLimit = 0;
	unsigned long lCount1 = 0;
	unsigned long lStartTime = 0;
   	unsigned long lTime = time(0);
   	unsigned long lTotRec = 0;
	
   	char sdate[11] = "";
   	char edate[11] = "";
   	char time_flag[2] = "";
   	char start[17] = "";
   	char limit[17] = "";
   	char sort[17] = "";
   	char dir[9] = "";
   	char sql[4096] = "";
   	char _sip[16] = "";
   	char _idno[19] = "";
   	char _dispname[33] = "";
   	char corpname[129] = "";
   	char _groupid[33] = "";
   	char _did[33] = "";
   	char instr[8192] = "";	
   	char sql2[256] = "";
   	char frmfile[256] = "";
   	char *tmpgbk = NULL;
	char **ptmp = NULL;	
	char _sname[16] = "";
	char _svalue[65] = "";
	char _keyword[33] = "";
	struct sttable
	{
		char tname[32];
		unsigned long lCount;
	};
	struct sttable tableinfo[120];//10年？
   	pasDbCursor *psCur = NULL;
   	char caGroups[1024];
    char caFacs[1024];
    char caStime[16];
   	char caEtime[16];
	 strcpy(caFacs,getDsFacDid());
   strcpy(caGroups,getDsGroupid());
   	
   	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
   	utMsgGetSomeNVar(psMsgHead, 20,
   				     "keyword",  UT_TYPE_STRING,  sizeof(_keyword)-1, _keyword,
   				     "uid",      UT_TYPE_STRING,  sizeof(_uid)-1,     _uid,
   				     "sname",    UT_TYPE_STRING,  sizeof(_sname)-1,   _sname,
   				     "svalue",   UT_TYPE_STRING,  sizeof(_svalue)-1,   _svalue,
   				     "groupid",  UT_TYPE_STRING,  sizeof(_groupid)-1, _groupid,
   				     "did",      UT_TYPE_STRING,  sizeof(_did)-1,     _did,
   				     "corpname", UT_TYPE_STRING,  sizeof(corpname)-1, corpname,
   				     
   				     "sip",      UT_TYPE_STRING,  sizeof(_sip)-1,     _sip,  
   				     "idno",     UT_TYPE_STRING,  sizeof(_idno)-1,    _idno,
   				     "dispname", UT_TYPE_STRING,  sizeof(_dispname)-1, _dispname,
   				     "sdate", 	 UT_TYPE_STRING,  sizeof(sdate)-1,     sdate,
   				     "edate", 	 UT_TYPE_STRING,  sizeof(edate)-1,     edate,
   				     
   				     "time_flag",UT_TYPE_STRING,  sizeof(time_flag)-1, time_flag,
   				     "start",    UT_TYPE_STRING,  sizeof(start)-1,     start,
   				     "limit",    UT_TYPE_STRING,  sizeof(limit)-1,     limit,
   				     "sort",     UT_TYPE_STRING,  sizeof(sort)-1,      sort,
   				     "dir",      UT_TYPE_STRING,  sizeof(dir)-1,       dir,
   				      "exp",      UT_TYPE_STRING,  10,caExport,
   				     "stime",    UT_TYPE_STRING, 10,caStime,
   				     "etime",    UT_TYPE_STRING, 10,caEtime);
   if(strcmp(_sname,"undefined")==0){
   	strcpy(_sname,"");
   }
   		   	   	   	if(strlen(caStime)==0){
   				     	strcpy(caStime,"00:00:00");
   				  }
   				  if(strlen(caEtime)==0){
   				    	strcpy(caEtime,"23:59:59");
   				  }			      
   	if(strlen(sort)==0){
 	   strcpy(sort,"stime");
   }			      
   	char *_dispname_gbk = NULL;
	if(!utStrIsSpaces(_dispname) )
	{
		_dispname_gbk = convert("UTF-8", "GBK", _dispname);
	}
   	iTime_flag = atoi(time_flag);
   	switch(iTime_flag)
   	{
   		case 1://今天
   			days = 1;
   			break;
   		case 2://最近三天
   			days = 3;
   			break;
   		case 3://最近一周
   			days = 7;
   			break;
   		case 4://最近一月
   			days = 30;
   			break;
   		case 5://最近三月
   			days = 90;
   			break;
   		case 6://最近半年
   			days = 180;
   			break;
   		default:
   			days = 1;
   			break;
   	}	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncssearchlog_if_", &lStartTime, &lTime, &lCount);
	}
	else 
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncssearchlog_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}
	if(!utStrIsSpaces(corpname))//指定了单位名称查询
	{
		tmpgbk = convert("UTF-8", "GBK", corpname);
		snprintf(sql2, sizeof(sql2), "select userid from ncsuser where (dispname like '%c%s%c' or username like '%c%s%c' )", '%',tmpgbk,'%', '%',tmpgbk,'%');
		free(tmpgbk);
		iNum=0;
		memset(instr,0,sizeof(instr));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			memset(instr,0,sizeof(instr));
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), ",");
				}
				snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	if(!utStrIsSpaces(_did))
	{
		did = atol(_did);
	}
	if(!utStrIsSpaces(_groupid))
	{
		snprintf(sql2, sizeof(sql2), "select groupid from ncsgroup where  groupid = %lu or pid=%lu ", atol(_groupid),atol(_groupid));
		printf("sql2=%s\n",sql2);
		iNum=0;
		memset(instr_t,0,sizeof(instr_t));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), ",");
				}
				snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	char *_keyword_gbk = NULL;
	_keyword_gbk = convert("UTF-8", "GBK", _keyword);
	strcpy(_sdate, utTimFormat("%Y/%m/%d", lStartTime));
	strcpy(_edate, utTimFormat("%Y/%m/%d", lTime));
	for(i = 0; i < lCount; i++)
	{
		snprintf(frmfile, sizeof(frmfile), "/usr/local/infobright/data/ncs/%s.frm", ptmp[i]);
//		if(access(frmfile, F_OK) == 0)
//		{
			lCount1 = 0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu ",  ptmp[i], lStartTime, lTime);
			
			if(!utStrIsSpaces(_keyword))
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (svalue like '%%%s%%' or sip like '%%%s%%' or idno like '%%%s%%' or dispname like '%%%s%%' or username like '%%%s%%')", _keyword,  _keyword,  _keyword, _keyword,_keyword);
			}
			if(!utStrIsSpaces(_sname) && strcmp(_sname, "0"))//1
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sname = '%s'", _sname);
			}
			if(!utStrIsSpaces(_svalue) )
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and svalue like '%%%s%%'", _svalue);
			}
			
			if(!utStrIsSpaces(_sip) )//源IP地址
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
			}
			if(!utStrIsSpaces(_idno) )//4
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
			}
			if(!utStrIsSpaces(_dispname) )//依据显示名查询
			{

					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or username like '%%%s%%')", _dispname,_dispname);
			
			}
			if(!utStrIsSpaces(corpname))//6
			{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
			}
			if(!utStrIsSpaces(_did))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
			}
			if(!utStrIsSpaces(_uid))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
			}
			if(!utStrIsSpaces(_groupid))//8
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
			}
			if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
			
			tmpgbk = convert("UTF-8", "GBK", sql);
			if(tmpgbk != NULL)
			{
				printf("sql = %s\n", tmpgbk);
				pasDbOneRecord(tmpgbk, 0, UT_TYPE_ULONG, 4, &lCount1);
				free(tmpgbk);
			}
			if(lCount1 > 0)
			{
				strcpy(tableinfo[num].tname, ptmp[i]);
				tableinfo[num].lCount = lCount1;
				num++;
			}
//		}
		free(ptmp[i]);
	}
	free(ptmp);	
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	
	//
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	}
	//
	
	iNum = 0;
	iStart = atol(start);
	iLimit = atol(limit);
	
	if(strlen(caExport)>0){
		
		iStart=0;
		iLimit=99999;
				

		        sprintf(caFilename,"ncssearchlog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"搜索日志\n");
		        ncUtlPrintCsv(fp,6,UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"源IP",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"搜索工具",
		                           UT_TYPE_STRING,"搜索关键字");
		                           
				
			  
		
		
	}
	
	
	for(i = num-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		snprintf(sql, sizeof(sql), "select uid, did, dispname, stime, sip, sname, svalue,username from %s where stime >= %lu and stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);
		if(!utStrIsSpaces(_keyword))
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (svalue like '%%%s%%' or sip like '%%%s%%' or idno like '%%%s%%' or dispname like '%%%s%%' or username like '%%%s%%')", _keyword,  _keyword,  _keyword, _keyword,_keyword);
		}
		if(!utStrIsSpaces(_sname) && strcmp(_sname, "0"))//1
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sname = '%s'", _sname);
		}
		if(!utStrIsSpaces(_svalue) )
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and svalue like '%%%s%%'", _svalue);
		}
		if(!utStrIsSpaces(_sip) )//源IP地址
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
		}
		if(!utStrIsSpaces(_idno) )//4
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
		}
		if(!utStrIsSpaces(_dispname) )//依据显示名查询
		{
	     snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or username like '%%%s%%')", _dispname,_dispname);
		}
		if(!utStrIsSpaces(corpname))//6
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
		}
		if(!utStrIsSpaces(_did))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
		}
		if(!utStrIsSpaces(_uid))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
		}
		if(!utStrIsSpaces(_groupid))//8
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
		}
		 if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
		
		
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
		
		
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			_break = 1;
		}
		tmpgbk = convert("UTF-8", "GBK", sql);
		if(tmpgbk == NULL)
			continue;
		printf("__line = %d, sql = %s\n", __LINE__, tmpgbk);
		psCur = pasDbOpenSql(tmpgbk, 0);
		if(psCur != NULL)
		{
			//select uid, did, dispname, stime, sip, service, mesg, mfrom, mto from
			unsigned long uid = 0;
			unsigned long did = 0;
			char dispname[33] ="";
			unsigned long stime = 0;
			char sip[16] = "";
			char *svalue_gbk = NULL;
			/*
			unsigned long service = 0;
			char mesg[256] = "";
			char mfrom[49] = "";
			char mto[49] = "";
			*/
			char sname[17] = "";
			char svalue[129] = "";
			char caUsername[32];
			char corpname[256] = "";
			char s_time[21] = "";
			memset(caUsername,0,sizeof(caUsername));
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_STRING, sizeof(sname)-1,       sname,
					 UT_TYPE_STRING, sizeof(svalue)-1,      svalue,
					 UT_TYPE_STRING,sizeof(caUsername)-1,caUsername);
			while((iret == 0) || (iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
				if(strlen(caUsername)==11){
					strcpy(dispname,caUsername);
				}
				strcpy(corpname, ncsUtlGetClientNameById(psShmHead,did,"未知"));
				strcpy(s_time, utTimFormat("%Y/%m/%d %H:%M:%S", stime));
				squeeze(dispname, '-');
				utPltPutLoopVarF(psDbHead,"corpname",iNum,"%s_%lu", corpname, did);
				utPltPutLoopVar(psDbHead,"stime",iNum,s_time);
				utPltPutLoopVarF(psDbHead,"did",iNum,"%d",did);
				utPltPutLoopVarF(psDbHead,"uid",iNum,"%d",uid);
				
				utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s_%lu_%lu", dispname, did, uid);
				utPltPutLoopVarF(psDbHead,"othlog_param",iNum,"%s_%s_%lu_%lu", _sdate, _edate, did, uid);
				utPltPutLoopVar(psDbHead,"sname",iNum,sname);
				svalue_gbk = my_convert(svalue);
				utPltSetCvtHtml(1);
				utPltPutLoopVar(psDbHead,"svalue",iNum,svalue_gbk ? svalue_gbk : "");
				utPltSetCvtHtml(0);
				free(svalue_gbk);
				utPltPutLoopVar(psDbHead,"sip",iNum,sip);
				
				if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,6,UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,dispname,
                          UT_TYPE_STRING,sip,
                          UT_TYPE_STRING,s_time,
                          UT_TYPE_STRING,sname,
                          UT_TYPE_STRING,svalue);
                         
                         
				 }
				
				uid = did = stime =0;
				memset(dispname, 0, sizeof(dispname));
				memset(sip, 0, sizeof(sip));
				memset(sname, 0, sizeof(sname));
				memset(svalue, 0, sizeof(svalue));
			memset(caUsername,0,sizeof(caUsername));
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_STRING, sizeof(sname)-1,       sname,
					 UT_TYPE_STRING, sizeof(svalue)-1,      svalue,
					 UT_TYPE_STRING,sizeof(caUsername)-1,caUsername);
			}			 
			pasDbCloseCursor(psCur);
		} 
		free(tmpgbk);
		if(_break == 1)
			break; 
	}
	free(_keyword_gbk);
	free(_dispname_gbk);
	
	if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }
	
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/search/ncs_searchlog_list.html");
	return iret;
}


int ncsFormSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	char caType[32];
  char caExport[32];
  FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];
	char instr_t[8192] = "";
	char _uid[17] = "";
	char _sdate[11] = "";
	char _edate[11] = "";
	
	int iTime_flag = 0;
	int days = 0;   
	int iret = 0;
	int _break = 0;
	int i = 0;
	int lCount = 0;
	int num = 0;
	int iNum = 0;
	
	unsigned long did = 0;
	unsigned long iStart = 0;
	unsigned long iLimit = 0;
	unsigned long lCount1 = 0;
	unsigned long lStartTime = 0;
   	unsigned long lTime = time(0);
   	unsigned long lTotRec = 0;
	
   	char sdate[11] = "";
   	char edate[11] = "";
   	char time_flag[2] = "";
   	char start[17] = "";
   	char limit[17] = "";
   	char sort[17] = "";
   	char dir[9] = "";
   	char sql[4096] = "";
   	char _sip[16] = "";
   	char _idno[19] = "";
   	char _dispname[33] = "";
   	char corpname[129] = "";
   	char _groupid[33] = "";
   	char _did[33] = "";
   	char instr[8192] = "";	
   	char sql2[256] = "";
   	char frmfile[256] = "";
   	char *tmpgbk = NULL;
	char **ptmp = NULL;	
	char _keyword[33] = "";
	char _host[33] = "";
	char _url[65] = "";
	char _formasize[8] = "";
	char _dip[16] = "";
	struct sttable
	{
		char tname[32];
		unsigned long lCount;
	};
	struct sttable tableinfo[120];//10年？
   	pasDbCursor *psCur = NULL;
  char caGroups[1024];
  char caFacs[1024];
   char caStime[16];
   char caEtime[16];
  
	 strcpy(caFacs,getDsFacDid());
   strcpy(caGroups,getDsGroupid());
   	
   	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
   	utMsgGetSomeNVar(psMsgHead, 22,
   				     "keyword",    UT_TYPE_STRING,  sizeof(_keyword)-1,   _keyword,
   				     "uid",        UT_TYPE_STRING,  sizeof(_uid)-1,       _uid,
   				     "host",       UT_TYPE_STRING,  sizeof(_host)-1,     _host,
   				     "url",        UT_TYPE_STRING,  sizeof(_url)-1,       _url,
   				     "formasize",  UT_TYPE_STRING,  sizeof(_formasize)-1, _formasize,
   				     "dip",        UT_TYPE_STRING,  sizeof(_dip)-1,       _dip,  
   				     "groupid",    UT_TYPE_STRING,  sizeof(_groupid)-1,   _groupid,
   				     
   				     "did",        UT_TYPE_STRING,  sizeof(_did)-1,        _did,
   				     "corpname",   UT_TYPE_STRING,  sizeof(corpname)-1,    corpname,
   				     "sip",        UT_TYPE_STRING,  sizeof(_sip)-1,        _sip,  
   				     "idno",       UT_TYPE_STRING,  sizeof(_idno)-1,       _idno,
   				     "dispname",   UT_TYPE_STRING,  sizeof(_dispname)-1,   _dispname,
   				     
   				     "sdate", 	   UT_TYPE_STRING,  sizeof(sdate)-1,       sdate,
   				     "edate", 	   UT_TYPE_STRING,  sizeof(edate)-1,       edate,
   				     "time_flag",  UT_TYPE_STRING,  sizeof(time_flag)-1,   time_flag,
   				     "start",      UT_TYPE_STRING,  sizeof(start)-1,       start,
   				     "limit",      UT_TYPE_STRING,  sizeof(limit)-1,       limit,
   				     "sort",       UT_TYPE_STRING,  sizeof(sort)-1,        sort,
   				     "dir",        UT_TYPE_STRING,  sizeof(dir)-1,         dir,
   				      "exp",      UT_TYPE_STRING,  10,caExport,
   				      "stime",    UT_TYPE_STRING, 10,caStime,
   				     "etime",    UT_TYPE_STRING, 10,caEtime);
  	//char *_dispname_gbk = NULL;
	//if(!utStrIsSpaces(_dispname) )
	//{
	//	_dispname_gbk = convert("UTF-8", "GBK", _dispname);
	//}
   	iTime_flag = atoi(time_flag);
   	
   	   	   	 if(strlen(caStime)==0){
   				     	strcpy(caStime,"00:00:00");
   				  }
   				  if(strlen(caEtime)==0){
   				    	strcpy(caEtime,"23:59:59");
   				  }	
   if(strlen(sort)==0){
 	   strcpy(sort,"stime");
   }	
   	
   	switch(iTime_flag)
   	{
   		case 1://今天
   			days = 1;
   			break;
   		case 2://最近三天
   			days = 3;
   			break;
   		case 3://最近一周
   			days = 7;
   			break;
   		case 4://最近一月
   			days = 30;
   			break;
   		case 5://最近三月
   			days = 90;
   			break;
   		case 6://最近半年
   			days = 180;
   			break;
   		default:
   			days = 1;
   			break;
   	}	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime,"ncsformmon_if_", &lStartTime, &lTime, &lCount);
	}
	else 
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncsformmon_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}
	if(!utStrIsSpaces(corpname))//指定了单位名称查询
	{
		tmpgbk = convert("UTF-8", "GBK", corpname);
		snprintf(sql2, sizeof(sql2), "select userid from ncsuser where (dispname like '%c%s%c' or username like '%c%s%c' )", '%',tmpgbk,'%', '%',tmpgbk,'%');
		free(tmpgbk);
		iNum=0;
		memset(instr,0,sizeof(instr));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			memset(instr,0,sizeof(instr));
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), ",");
				}
				snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	if(!utStrIsSpaces(_did))
	{
		did = atol(_did);
	}
	if(!utStrIsSpaces(_groupid))
	{
		snprintf(sql2, sizeof(sql2), "select groupid from ncsgroup where  groupid = %lu or pid=%lu ", atol(_groupid),atol(_groupid));
		printf("sql2=%s\n",sql2);
		iNum=0;
		memset(instr_t,0,sizeof(instr_t));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), ",");
				}
				snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	char *_dispname_gbk = NULL;
	if(!utStrIsSpaces(_dispname) )
	{
		_dispname_gbk = convert("UTF-8", "GBK", _dispname);
	}
	char *_keyword_gbk = convert("UTF-8", "GBK", _keyword);
	strcpy(_sdate, utTimFormat("%Y/%m/%d", lStartTime));
	strcpy(_edate, utTimFormat("%Y/%m/%d", lTime));
	for(i = 0; i < lCount; i++)
	{
		snprintf(frmfile, sizeof(frmfile), "/usr/local/infobright/data/ncs/%s.frm", ptmp[i]);
//		if(access(frmfile, F_OK) == 0)
//		{
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu ",  ptmp[i], lStartTime, lTime);
			if(!utStrIsSpaces(_keyword))
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (host like '%%%s%%' or url like '%%%s%%' or dispname like '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or idno like '%%%s%%')", _keyword, _keyword, _keyword_gbk, _keyword, _keyword, _keyword);
			}
			if(!utStrIsSpaces(_host) )
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and host like '%%%s%%'", _host);
			}
			if(!utStrIsSpaces(_url) )
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and url like '%%%s%%'", _url);
			}
			if(!utStrIsSpaces(_dispname) )//依据显示名查询
			{
				if(_dispname_gbk == NULL)
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
				}
				else 
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
				}
			}
			if(atol(_formasize) == 2)//有附件
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and asize > 0");
			}
			else if(atol(_formasize) == 1)//无附件
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and asize = 0");
			}
			else 
			{
			}
			if(!utStrIsSpaces(_dip) )//目的IP地址
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
			}
			if(!utStrIsSpaces(_sip) )//源IP地址
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
			}
			if(!utStrIsSpaces(_idno) )//证件号码
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
			}
			if(!utStrIsSpaces(corpname))//6
			{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
			}
			if(!utStrIsSpaces(_did))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
			}
			if(!utStrIsSpaces(_uid))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
			}
			if(!utStrIsSpaces(_groupid))//8
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
			}
			
			if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
			
			
			lCount1 = 0;
			pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
			printf("sql = %s\n", sql);
			if(lCount1 > 0)
			{
				strcpy(tableinfo[num].tname, ptmp[i]);
				tableinfo[num].lCount = lCount1;
				num++;
			}
//		}
		free(ptmp[i]);
	}
	free(ptmp);	
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	
	//
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	}
	//
	
	iNum = 0;
	iStart = atol(start);
	iLimit = atol(limit);
	
		if(strlen(caExport)>0){
		
		iStart=0;
		iLimit=99999;
				

		        sprintf(caFilename,"ncsformlog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"表单日志\n");
		        ncUtlPrintCsv(fp,8,UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"源IP",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"网址",
		                           UT_TYPE_STRING,"目标IP",
		                           UT_TYPE_STRING,"表单大小",
		                           UT_TYPE_STRING,"附件");
		                           
				
			  
		
		
	}
	
	
	
	for(i = num-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		snprintf(sql, sizeof(sql), "select uid, did, dispname, stime, sip, dip, host, url, fileid, msize, asize from %s where stime >= %lu and stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);
		if(!utStrIsSpaces(_keyword))
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (host like '%%%s%%' or url like '%%%s%%' or dispname like '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or idno like '%%%s%%')", _keyword, _keyword, _keyword_gbk, _keyword, _keyword, _keyword);
		}
		if(!utStrIsSpaces(_host) )
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and host like '%%%s%%'", _host);
		}
		if(!utStrIsSpaces(_url) )
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and url like '%%%s%%'", _url);
		}
		if(!utStrIsSpaces(_dispname) )//依据显示名查询
		{
			if(_dispname_gbk == NULL)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
			}
			else 
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
			}
		}
		if(atol(_formasize) == 2)//有附件
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and asize > 0");
		}
		else if(atol(_formasize) == 1)//无附件
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and asize = 0");
		}
		else 
		{
		}
		if(!utStrIsSpaces(_dip) )//目的IP地址
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
		}
		if(!utStrIsSpaces(_sip) )//源IP地址
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
		}
		if(!utStrIsSpaces(_idno) )//证件号码
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
		}
		if(!utStrIsSpaces(corpname))//6
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
		}
		if(!utStrIsSpaces(_did))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
		}
		if(!utStrIsSpaces(_uid))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
		}
		
		 if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
		
		if(!utStrIsSpaces(_groupid))//8
		{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
		}
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			_break = 1;
		}
		printf("__line = %d, sql = %s\n", __LINE__, sql);
		psCur = pasDbOpenSql(sql, 0);
		if(psCur != NULL)
		{
			unsigned long uid = 0;
			unsigned long did = 0;
			char dispname[33] ="";
			unsigned long stime = 0;
			char sip[16] = "";
			char dip[16] = "";
			char host[65] = "";
			char url[65] = "";
			char upfile[65] = "";
			unsigned long msize = 0;
			unsigned long asize = 0;
			char corpname[256] = "";
			char caUrl[256];
			char s_time[21] = "";
			long long lUpfile;
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_STRING, sizeof(dip)-1,  	dip,
					 UT_TYPE_STRING, sizeof(host)-1,    	host,
					 UT_TYPE_STRING,63,      	url,
					 UT_TYPE_LONG8, 8,    	&lUpfile,
					 UT_TYPE_ULONG,  4,                     &msize,
					 UT_TYPE_ULONG,  4,                     &asize);
			while((iret == 0) || (iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
				strcpy(corpname, ncsUtlGetClientNameById(psShmHead,did,"未知"));
				strcpy(s_time, utTimFormat("%Y/%m/%d %H:%M:%S", stime));
				squeeze(dispname, '-');
				utPltPutLoopVarF(psDbHead,"corpname",iNum,"%s_%lu", corpname, did);
				utPltPutLoopVar(psDbHead,"stime",iNum,s_time);
				
				utPltPutLoopVarF(psDbHead,"othlog_param",iNum,"%s_%s_%lu_%lu", _sdate, _edate, did, uid);
				utPltPutLoopVar(psDbHead,"sip",iNum,sip);
				utPltPutLoopVar(psDbHead,"dip",iNum,dip);
				utPltPutLoopVarF(psDbHead, "formsize", iNum, "%lu", msize);//表单大小
				utPltPutLoopVar(psDbHead, "formasize", iNum, asize ? "有" : "无");//有无附件
				utPltPutLoopVarF(psDbHead, "showform", iNum, "%s;%lu;%llu", tableinfo[i].tname, stime, lUpfile);
				utPltSetCvtHtml(1);
	//			printf("url=%s\n",url);
				utPltPutLoopVarF(psDbHead, "url", iNum, "http://%s%s", host, url);
				utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s_%lu_%lu", dispname, did, uid);
				utPltSetCvtHtml(0);
				utPltPutLoopVarF(psDbHead,"did",iNum,"%d",did);
				utPltPutLoopVarF(psDbHead,"uid",iNum,"%d",uid);
				utPltPutLoopVarF(psDbHead,"upfile",iNum,"%llu",lUpfile);
				
			if(strlen(caExport)>0){
				sprintf(caUrl,"%s%s",host,url);
					 ncUtlPrintCsv(fp,8,UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,dispname,
                          UT_TYPE_STRING,sip,
                          UT_TYPE_STRING,s_time,
                          UT_TYPE_STRING,caUrl,
                          UT_TYPE_STRING,dip,
                          UT_TYPE_LONG,msize,
                          UT_TYPE_STRING,asize ? "有" : "无");
                         
                         
				 }
				
				
				uid = did = stime =  msize = asize = 0;
				
				memset(dispname, 0, sizeof(dispname));
				memset(sip, 0, sizeof(sip));
				memset(dip, 0, sizeof(dip));
				memset(host, 0, sizeof(host));
				memset(url, 0, sizeof(url));
				memset(upfile, 0, sizeof(upfile));
				iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_STRING, sizeof(dip)-1,  	dip,
					 UT_TYPE_STRING, sizeof(host)-1,    	host,
					 UT_TYPE_STRING, 63,      	url,
					 UT_TYPE_LONG8, 8,    	&lUpfile,
					 UT_TYPE_ULONG,  4,                     &msize,
					 UT_TYPE_ULONG,  4,                     &asize);
			}			 
			pasDbCloseCursor(psCur);
		} 
		if(_break == 1)
			break; 
	}
	free(_keyword_gbk);
	free(_dispname_gbk);
	
	if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }
	
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/search/ncs_form_list.html");
	return iret;
}

int ncsHttpSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	ncsNettype *psData;
  char caType[32];
  char caExport[32];
  FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];
	char instr_t[8192] = "";	
	
	int iTime_flag = 0;
	int days = 0;   
	int iret = 0;
	int _break = 0;
	int i = 0;
	int lCount = 0;
	int num = 0;
	int iNum = 0;
	
	unsigned long did = 0;
	unsigned long iStart = 0;
	unsigned long iLimit = 0;
	unsigned long lCount1 = 0;
	unsigned long lStartTime = 0;
   	unsigned long lTime = time(0);
   	unsigned long lTotRec = 0;
	
   	char sdate[11] = "";
   	char edate[11] = "";
   	char time_flag[2] = "";
   	char start[17] = "";
   	char limit[17] = "";
   	char sort[17] = "";
   	char dir[9] = "";
   	char _host[65] = "";
   	char sql[4096] = "";
   	char _uri[129] = "";
   	char _sip[16] = "";
   	char _dip[16] = "";
   	char _idno[19] = "";
   	char _title[129] = "";
   	char _dispname[33] = "";
   	char corpname[129] = "";
   	char _groupid[33] = "";
   	char _did[33] = "";
   	char _uid[17] = "";
   	char instr[8192] = "";	
   	char sql2[256] = "";
   	char frmfile[256] = "";
   	char _keyword[33] = "";
   	char *tmpgbk = NULL;
   	char caStime[16];
   	char caEtime[16];
   	
	char **ptmp = NULL;	
	
	struct sttable
	{
		char tname[32];
		unsigned long lCount;
	};
	struct sttable tableinfo[120];//10年？
   	
   pasDbCursor *psCur = NULL;
   char caGroups[1024];
   char caFacs[1024];
   unsigned long lIp;
	 long iReturn;
	 char caTemp[1024];
   char caArea[256],caName2[256];
   
	 strcpy(caFacs,getDsFacDid());
   strcpy(caGroups,getDsGroupid());
   
   	utPltDbHead *psDbHead = utPltInitDb();
   	

	utMsgPrintMsg(psMsgHead);
   	utMsgGetSomeNVar(psMsgHead, 22,
   				     "keyword",  UT_TYPE_STRING,  sizeof(_keyword)-1, _keyword,
   				     "uid",      UT_TYPE_STRING,  sizeof(_uid)-1,     _uid,
   				     "groupid",  UT_TYPE_STRING,  sizeof(_groupid)-1, _groupid,
   				     "did",      UT_TYPE_STRING,  sizeof(_did)-1,     _did,
   				     "corpname", UT_TYPE_STRING,  sizeof(corpname)-1, corpname,
   				     "host",     UT_TYPE_STRING,  sizeof(_host)-1,    _host,
   				     "uri",      UT_TYPE_STRING,  sizeof(_uri)-1,     _uri,
   				     "sip",      UT_TYPE_STRING,  sizeof(_sip)-1,     _sip,   
   				     "dip",      UT_TYPE_STRING,  sizeof(_dip)-1,     _dip,   
   				     "idno",     UT_TYPE_STRING,  sizeof(_idno)-1,    _idno,
   				     "title",    UT_TYPE_STRING,  sizeof(_title)-1,   _title,   
   				     "dispname", UT_TYPE_STRING,  sizeof(_dispname)-1, _dispname,
   				     "sdate", 	 UT_TYPE_STRING,  sizeof(sdate)-1,     sdate,
   				     "edate", 	 UT_TYPE_STRING,  sizeof(edate)-1,     edate,
   				     "time_flag",UT_TYPE_STRING,  sizeof(time_flag)-1, time_flag,
   				     "start",    UT_TYPE_STRING,  sizeof(start)-1,     start,
   				     "limit",    UT_TYPE_STRING,  sizeof(limit)-1,     limit,
   				     "sort",     UT_TYPE_STRING,  sizeof(sort)-1,      sort,
   				     "dir",      UT_TYPE_STRING,  sizeof(dir)-1,       dir,
   				      "exp",      UT_TYPE_STRING,  10,caExport,
   				     "stime",    UT_TYPE_STRING, 10,caStime,
   				     "etime",    UT_TYPE_STRING, 10,caEtime);
   	char *_dispname_gbk = NULL;
   if(strlen(sort)==0){
 	   strcpy(sort,"stime");
   }
      	    if(strlen(caStime)==0){
   				     	strcpy(caStime,"00:00:00");
   				  }
   				  if(strlen(caEtime)==0){
   				    	strcpy(caEtime,"23:59:59");
   				  }		
   	
	if(!utStrIsSpaces(_dispname) )
	{
		_dispname_gbk = convert("UTF-8", "GBK", _dispname);
	}
   	iTime_flag = atoi(time_flag);
   	switch(iTime_flag)
   	{
   		case 1://今天
   			days = 1;
   			break;
   		case 2://最近三天
   			days = 3;
   			break;
   		case 3://最近一周
   			days = 7;
   			break;
   		case 4://最近一月
   			days = 30;
   			break;
   		case 5://最近三月
   			days = 90;
   			break;
   		case 6://最近半年
   			days = 180;
   			break;
   		default:
   			days = 1;
   			break;
   	}	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncshttplog_if_", &lStartTime, &lTime, &lCount);
	}
	else 
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncshttplog_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}
	if(!utStrIsSpaces(corpname))//指定了单位名称查询
	{
		tmpgbk = convert("UTF-8", "GBK", corpname);
		snprintf(sql2, sizeof(sql2), "select userid from ncsuser where (dispname like '%c%s%c' or username like '%c%s%c' )", '%',tmpgbk,'%', '%',tmpgbk,'%');
		free(tmpgbk);
		iNum=0;
		memset(instr,0,sizeof(instr));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			memset(instr,0,sizeof(instr));
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), ",");
				}
				snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	if(!utStrIsSpaces(_did))
	{
		did = atol(_did);
	}
	if(!utStrIsSpaces(_groupid))
	{
		snprintf(sql2, sizeof(sql2), "select groupid from ncsgroup where  groupid = %lu or pid=%lu ", atol(_groupid),atol(_groupid));
		printf("sql2=%s\n",sql2);
		iNum=0;
		memset(instr_t,0,sizeof(instr_t));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), ",");
				}
				snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	char *_keyword_gbk = convert("UTF-8", "GBK", _keyword);
	char _sdate[11] = "";
	char _edate[11] = "";
	strcpy(_sdate, utTimFormat("%Y/%m/%d", lStartTime));
	strcpy(_edate, utTimFormat("%Y/%m/%d", lTime));
	for(i = 0; i < lCount; i++)
	{
		snprintf(frmfile, sizeof(frmfile), "/usr/local/infobright/data/ncs/%s.frm", ptmp[i]);
//		if(access(frmfile, F_OK) == 0)
//		{
			lCount1 = 0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu ",  ptmp[i], lStartTime, lTime);
			
			if(!utStrIsSpaces(_keyword))
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), "and (host like '%%%s%%' or uri like '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or idno like '%%%s%%' or title like '%%%s%%' or dispname like '%%%s%%')", _keyword_gbk,  _keyword_gbk,  _keyword_gbk,  _keyword_gbk,  _keyword_gbk,  _keyword_gbk,  _keyword_gbk);
			}
			if(!utStrIsSpaces(_host) )//1
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and host like '%%%s%%'", _host);
			}
			if(!utStrIsSpaces(_uri) )//2
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uri like '%%%s%%'", _uri);
			}
			if(!utStrIsSpaces(_sip) )//3
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
			}
			if(!utStrIsSpaces(_dip) )//4
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
			}
			if(!utStrIsSpaces(_idno) )//5
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
			}
			if(!utStrIsSpaces(_title) )//6
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and title like '%%%s%%'", _title);
			}
			if(!utStrIsSpaces(_dispname) )//依据显示名查询
			{
				if(_dispname_gbk == NULL)
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
				}
				else 
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
				}
			}
			if(!utStrIsSpaces(corpname))//8
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
			}
			if(!utStrIsSpaces(_did))//9
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
			}
			if(!utStrIsSpaces(_uid))//9
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
			}
			if(!utStrIsSpaces(_groupid))//10
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
			}
			if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
			
			
			printf("__line = %d, sql = %s\n", __LINE__, sql);
			pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
			if(lCount1 > 0)
			{
				strcpy(tableinfo[num].tname, ptmp[i]);
				tableinfo[num].lCount = lCount1;
				num++;
			}
//		}
		free(ptmp[i]);
	}
	free(ptmp);	
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	
	//
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	}
	//
	
	iNum = 0;
	iStart = atol(start);
	iLimit = atol(limit);
	
	if(strlen(caExport)>0){
		
		iStart=0;
		iLimit=99999;
				

		        sprintf(caFilename,"ncshttplog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"网站访问日志\n");
		        ncUtlPrintCsv(fp,9,UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"网址类别",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"源IP",
		                           UT_TYPE_STRING,"目标IP",
		                           UT_TYPE_STRING,"访问网址",
		                           UT_TYPE_STRING,"标题",
		                           UT_TYPE_STRING,"流量");
		                         
				
			  
		
		
	}
	
	for(i = num-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		snprintf(sql, sizeof(sql), "select uid, did, dispname, stime, sip, host, dport, uri, dip, title, bytes, urlid from %s where stime >= %lu and stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);
		if(!utStrIsSpaces(_keyword))
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), "and (host like '%%%s%%' or uri like '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or idno like '%%%s%%' or title like '%%%s%%' or dispname like '%%%s%%')", _keyword_gbk,  _keyword_gbk,  _keyword_gbk,  _keyword_gbk,  _keyword_gbk,  _keyword_gbk,  _keyword_gbk);
		}
		if(!utStrIsSpaces(_host) )//1
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and host like '%%%s%%'", _host);
		}
		if(!utStrIsSpaces(_uri) )//2
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uri like '%%%s%%'", _uri);
		}
		if(!utStrIsSpaces(_sip) )//3
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
		}
		if(!utStrIsSpaces(_dip) )//4
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
		}
		if(!utStrIsSpaces(_idno) )//5
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
		}
		if(!utStrIsSpaces(_title) )//6
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and title like '%%%s%%'", _title);
		}
		if(!utStrIsSpaces(_dispname) )//依据显示名查询
		{
			if(_dispname_gbk == NULL)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
			}
			else 
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
			}
		}
		if(!utStrIsSpaces(corpname))//8
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
		}
		if(!utStrIsSpaces(_did))//9
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
		}
		if(!utStrIsSpaces(_uid))//9
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
		}
		if(!utStrIsSpaces(_groupid))//10
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
		}
		if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
		}
		if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
		}
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
		printf("__line = %d, sql = %s\n", __LINE__, sql);
		
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			_break = 1;
		}
		psCur = pasDbOpenSql(sql, 0);
		if(psCur != NULL)
		{
			unsigned long uid = 0;
			unsigned long did = 0;
			char dispname[33] ="";
			unsigned long stime = 0;
			char sip[16] = "";
			char host[65] = "";
			unsigned long dport = 0;
			char uri[129] = "";
			char dip[16];
			char title[129] = "";
			unsigned long bytes = 0;
			unsigned long urlid = 0;
			char corpname[256] = "";
			char s_time[21] = "";
			char classname[33] = "";
			char caUrl[256];
			memset(title,0,sizeof(title));
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,              &uid,
					 UT_TYPE_ULONG,  4,              &did,
					 UT_TYPE_STRING, sizeof(dispname)-1, dispname,
					 UT_TYPE_ULONG,  4,              &stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  sip,
					 UT_TYPE_STRING, sizeof(host)-1, host,
					 UT_TYPE_ULONG,  4,              &dport,
					 UT_TYPE_STRING, sizeof(uri)-1,  uri,
					 UT_TYPE_STRING, sizeof(dip)-1,  dip,
					 UT_TYPE_STRING,sizeof(title)-1,title,
					 UT_TYPE_ULONG,  4,              &bytes,
					 UT_TYPE_ULONG,  4,              &urlid);
			while((iret == 0) || (iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
				strcpy(corpname, ncsUtlGetClientNameById(psShmHead,did,"未知"));
				strcpy(s_time, utTimFormat("%Y/%m/%d %H:%M:%S", stime));
					utPltSetCvtHtml(1);
				utPltPutLoopVarF(psDbHead,"corpname",iNum,"%s_%lu", corpname, did);
				
				utPltPutLoopVar(psDbHead,"stime",iNum, s_time);
				squeeze(dispname, '-');
				
				utPltPutLoopVar(psDbHead,"sip",iNum,sip);
				utPltPutLoopVar(psDbHead,"dip",iNum,dip);
           lIp=ntohl(utComHostAddress(dip));
          iReturn = pasUtlGetIpLocation(lIp,caArea,caName2);
      //    printf("iReturn=%d,lIp=%lu,caArea=%s\n",iReturn,lIp,caArea);
          if(iReturn == 1) {
            sprintf(caTemp,"%s,%s",caArea,caName2);
    //        printf("caTemp=%s\n",caTemp);
          }
          else{
                strcpy(caTemp,"");
          }

        utPltPutLoopVar(psDbHead,"iplocal",iNum,caTemp);
				if(dport>0){
				sprintf(caUrl,"http://%s:%lu",host, dport);
				strcat(caUrl,uri);
				}
				else{
					sprintf(caUrl,"http://%s",host);
					strcat(caUrl,uri);
				}
				utPltPutLoopVar(psDbHead,"url",iNum,caUrl);//url
				utPltPutLoopVar(psDbHead,"title",iNum,title);//标题
				utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s_%lu_%lu", dispname, did, uid);
				
			 psData = (struct ncsNettype_s *)pasLHashLook(pHashWebclass,&urlid); 
	      if(psData){
	          strcpy(classname,psData->caName);
	      }
	      else{
	      	strcpy(classname,"");
	      }
	//			printf("urlid=%d,name=%s\n",urlid,classname);
				utPltPutLoopVar(psDbHead, "urltype", iNum, classname);//网址库类型					   
				utPltPutLoopVarF(psDbHead,"bytes",iNum,"%lu", bytes);					     
				utPltPutLoopVarF(psDbHead, "othlog_param",iNum,"%s_%s_%lu_%lu", _sdate, _edate, did, uid);	
				
				utPltSetCvtHtml(0);
				utPltPutLoopVarF(psDbHead,"did",iNum,"%d",did);
				utPltPutLoopVarF(psDbHead,"uid",iNum,"%d",uid);
				
				if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,9,UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,dispname,
                          UT_TYPE_STRING,classname,
                          UT_TYPE_STRING,s_time,
                          UT_TYPE_STRING,sip,
                          UT_TYPE_STRING,dip,
                          UT_TYPE_STRING,caUrl,
                          UT_TYPE_STRING,title,
                          UT_TYPE_LONG,bytes);
				 }
				
				
				urlid = did = stime = dport = bytes = 0;
				memset(dispname, 0, sizeof(dispname));
				memset(sip, 0, sizeof(sip));
				memset(host, 0, sizeof(host));
				memset(uri, 0, sizeof(uri));
				memset(dip, 0, sizeof(dip));
			memset(title,0,sizeof(title));
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,              &uid,
					 UT_TYPE_ULONG,  4,              &did,
					 UT_TYPE_STRING, sizeof(dispname)-1, dispname,
					 UT_TYPE_ULONG,  4,              &stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  sip,
					 UT_TYPE_STRING, sizeof(host)-1, host,
					 UT_TYPE_ULONG,  4,              &dport,
					 UT_TYPE_STRING, sizeof(uri)-1,  uri,
					 UT_TYPE_STRING, sizeof(dip)-1,  dip,
					 UT_TYPE_STRING,sizeof(title)-1,title,
					 UT_TYPE_ULONG,  4,              &bytes,
					 UT_TYPE_ULONG,  4,              &urlid);
			}			 
			pasDbCloseCursor(psCur);
		} 
		if(_break == 1)
			break; 
	}
	free(_keyword_gbk);
	free(_dispname_gbk);
	
	if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }
	 printf("lTot=%d\n",lTotRec);
//utPltShowDb(psDbHead);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/search/ncs_http_list.html");
	printf("aaaaaaa\n");
	return iret;
}
//网络会话日志
int ncsNetsesSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	ncsNettype *psData;
  char caType[32];
  char caExport[32];
  FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];
	char instr_t[8192] = "";	
	int iTime_flag = 0;
	int days = 0;   
	int iret = 0;
	int _break = 0;
	int i = 0;
	int lCount = 0;
	int num = 0;
	int iNum = 0;
	
	unsigned long did = 0;
	unsigned long iStart = 0;
	unsigned long iLimit = 0;
	unsigned long lCount1 = 0;
	unsigned long lStartTime = 0;
   	unsigned long lTime = time(0);
   	unsigned long lTotRec = 0;
	
   	char sdate[11] = "";
   	char edate[11] = "";
   	char time_flag[2] = "";
   	char start[17] = "";
   	char limit[17] = "";
   	char sort[17] = "";
   	char dir[9] = "";
   	char sql[4096] = "";
   	char _sip[16] = "";
   	char _idno[19] = "";
   	char _dispname[33] = "";
   	char corpname[129] = "";
   	char _groupid[33] = "";
   	char _did[33] = "";
   	char _uid[17] = "";
   	char instr[8192] = "";	
   	char sql2[256] = "";
   	char frmfile[256] = "";
   	char *tmpgbk = NULL;
	char **ptmp = NULL;	
	char _mesg[129] = "";
	char _netsestype[9] = "";
	char _dip[16] = "";
	char _service[32];
	char _keyword[33] = "";
	struct sttable
	{
		char tname[32];
		unsigned long lCount;
	};
	struct sttable tableinfo[120];//10年？
   	pasDbCursor *psCur = NULL;
  char caGroups[1024];
  char caFacs[1024];
   char caArea[256],caName2[256];
     char caTemp[1024];
   long iReturn;
  unsigned long lIp;
      char caStime[16];
   	char caEtime[16];
	 strcpy(caFacs,getDsFacDid());
   strcpy(caGroups,getDsGroupid());
   
   	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
   	utMsgGetSomeNVar(psMsgHead, 21,
   				     "keyword",  UT_TYPE_STRING,  sizeof(_keyword)-1,_keyword,
   				     "uid",      UT_TYPE_STRING,  sizeof(_uid)-1,    _uid,
   				     "mesg",     UT_TYPE_STRING,  sizeof(_mesg)-1,   _mesg,
   				     "netsestype",UT_TYPE_STRING, sizeof(_netsestype)-1, _netsestype,
   				     "dip",      UT_TYPE_STRING,  sizeof(_dip)-1,     _dip,
   				     "groupid",  UT_TYPE_STRING,  sizeof(_groupid)-1, _groupid,
   				     "did",      UT_TYPE_STRING,  sizeof(_did)-1,     _did,
   				     "corpname", UT_TYPE_STRING,  sizeof(corpname)-1, corpname,
   				     "sip",      UT_TYPE_STRING,  sizeof(_sip)-1,     _sip,  
   				     "idno",     UT_TYPE_STRING,  sizeof(_idno)-1,    _idno,
   				     "dispname", UT_TYPE_STRING,  sizeof(_dispname)-1, _dispname,
   				     "sdate", 	 UT_TYPE_STRING,  sizeof(sdate)-1,     sdate,
   				     "edate", 	 UT_TYPE_STRING,  sizeof(edate)-1,     edate,
   				     "time_flag",UT_TYPE_STRING,  sizeof(time_flag)-1, time_flag,
   				     "start",    UT_TYPE_STRING,  sizeof(start)-1,     start,
   				     "limit",    UT_TYPE_STRING,  sizeof(limit)-1,     limit,
   				     "sort",     UT_TYPE_STRING,  sizeof(sort)-1,      sort,
   				     "dir",      UT_TYPE_STRING,  sizeof(dir)-1,       dir,
   				     "exp",      UT_TYPE_STRING,  10,caExport,
   				      "stime",    UT_TYPE_STRING, 10,caStime,
   				     "etime",    UT_TYPE_STRING, 10,caEtime);
   	char *_dispname_gbk = NULL;
   	   	   	if(strlen(caStime)==0){
   				     	strcpy(caStime,"00:00:00");
   				  }
   				  if(strlen(caEtime)==0){
   				    	strcpy(caEtime,"23:59:59");
   				  }	
   	
  if(strlen(sort)==0){
 	   strcpy(sort,"stime");
   }	
	if(!utStrIsSpaces(_dispname) )
	{
		_dispname_gbk = convert("UTF-8", "GBK", _dispname);
	}
   	iTime_flag = atoi(time_flag);
   	switch(iTime_flag)
   	{
   		case 1://今天
   			days = 1;
   			break;
   		case 2://最近三天
   			days = 3;
   			break;
   		case 3://最近一周
   			days = 7;
   			break;
   		case 4://最近一月
   			days = 30;
   			break;
   		case 5://最近三月
   			days = 90;
   			break;
   		case 6://最近半年
   			days = 180;
   			break;
   		default:
   			days = 1;
   			break;
   	}	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncsnetseslog_if_", &lStartTime, &lTime, &lCount);
	}
	else 
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncsnetseslog_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}
	if(!utStrIsSpaces(corpname))//指定了单位名称查询
	{
		tmpgbk = convert("UTF-8", "GBK", corpname);
		snprintf(sql2, sizeof(sql2), "select userid from ncsuser where (dispname like '%c%s%c' or username like '%c%s%c' )", '%',tmpgbk,'%', '%',tmpgbk,'%');
		free(tmpgbk);
		iNum=0;
		memset(instr,0,sizeof(instr));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			memset(instr,0,sizeof(instr));
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), ",");
				}
				snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	if(!utStrIsSpaces(_did))
	{
		did = atol(_did);
	}
	if(!utStrIsSpaces(_groupid))
	{
		snprintf(sql2, sizeof(sql2), "select groupid from ncsgroup where  groupid = %lu or pid=%lu ", atol(_groupid),atol(_groupid));
		printf("sql2=%s\n",sql2);
		iNum=0;
		memset(instr_t,0,sizeof(instr_t));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), ",");
				}
				snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	char _sdate[11] = "";
	char _edate[11] = "";
	char *_keyword_gbk = convert("UTF-8", "GBK", _keyword); 
	strcpy(_sdate, utTimFormat("%Y/%m/%d", lStartTime));
	strcpy(_edate, utTimFormat("%Y/%m/%d", lTime));
	for(i = 0; i < lCount; i++)
	{
		snprintf(frmfile, sizeof(frmfile), "/usr/local/infobright/data/ncs/%s.frm", ptmp[i]);
//		if(access(frmfile, F_OK) == 0)
//		{
			lCount1 = 0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu ",  ptmp[i], lStartTime, lTime);
			
			if(!utStrIsSpaces(_keyword))
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (mesg like '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or idno like '%%%s%%' or dispname like '%%%s%%')", _keyword, _keyword, _keyword, _keyword, _keyword);
			}
			if(!utStrIsSpaces(_netsestype) && atol(_netsestype) > 0 )//1(协议类型FTP,TELNET)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and service = %lu", atol(_netsestype));
			}
			if(!utStrIsSpaces(_mesg) )//2
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mesg like '%%%s%%'", _mesg);
			}
			if(!utStrIsSpaces(_sip) )//源IP地址
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
			}
			if(!utStrIsSpaces(_dip) )//目标IP地址
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
			}
			if(!utStrIsSpaces(_idno) )//4
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
			}
			if(!utStrIsSpaces(_dispname) )//依据显示名查询
			{
				if(_dispname_gbk == NULL)
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
				}
				else 
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
				}
			}
			if(!utStrIsSpaces(corpname))//6
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
			}
			if(!utStrIsSpaces(_did))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
			}
			if(!utStrIsSpaces(_uid))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
			}
			if(!utStrIsSpaces(_groupid))//8
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
			}
			if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
			
			
			tmpgbk = convert("UTF-8", "GBK", sql);
			if(tmpgbk != NULL)
			{
				printf("sql = %s\n", tmpgbk);
				pasDbOneRecord(tmpgbk, 0, UT_TYPE_ULONG, 4, &lCount1);
				free(tmpgbk);
			}
			if(lCount1 > 0)
			{
				strcpy(tableinfo[num].tname, ptmp[i]);
				tableinfo[num].lCount = lCount1;
				num++;
			}
//		}
		free(ptmp[i]);
	}
	free(ptmp);	
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	
	//
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	}
	//
	
	iNum = 0;
	iStart = atol(start);
	iLimit = atol(limit);
	if(strlen(caExport)>0){
		
		iStart=0;
		iLimit=99999;
		        sprintf(caFilename,"ncsseslog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"网络会话日志\n");
		        ncUtlPrintCsv(fp,7,UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"类别",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"源IP",
		                           UT_TYPE_STRING,"目标IP",
		                           UT_TYPE_STRING,"操作内容");
		                          
		                         
				
			  
		
		
	}
	
	for(i = num-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		snprintf(sql, sizeof(sql), "select uid, did, dispname, stime, sip, dip, service, mesg from %s where stime >= %lu and stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);
		
		if(!utStrIsSpaces(_keyword))
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (mesg like '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or idno like '%%%s%%' or dispname like '%%%s%%')", _keyword, _keyword, _keyword, _keyword, _keyword);
		}
		if(!utStrIsSpaces(_netsestype) && atol(_netsestype) > 0 )//1(协议类型FTP,TELNET)
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and service = %lu", atol(_netsestype));
		}
		if(!utStrIsSpaces(_mesg) )//2
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mesg like '%%%s%%'", _mesg);
		}
		if(!utStrIsSpaces(_sip) )//源IP地址
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
		}
		if(!utStrIsSpaces(_dip) )//目标IP地址
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
		}
		if(!utStrIsSpaces(_idno) )//4
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
		}
		if(!utStrIsSpaces(_dispname) )//依据显示名查询
		{
			if(_dispname_gbk == NULL)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
			}
			else 
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
			}
		}
		if(!utStrIsSpaces(corpname))//6
		{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
		}
		if(!utStrIsSpaces(_did))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
		}
		if(!utStrIsSpaces(_uid))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
		}
		if(!utStrIsSpaces(_groupid))//8
		{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
		}
		 if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
		
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
		
		
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			_break = 1;
		}
		tmpgbk = convert("UTF-8", "GBK", sql);
		if(tmpgbk == NULL)
			continue;
		printf("__line = %d, sql = %s\n", __LINE__, tmpgbk);
		psCur = pasDbOpenSql(tmpgbk, 0);
		if(psCur != NULL)
		{
			unsigned long uid = 0;
			unsigned long did = 0;
			char dispname[33] ="";
			unsigned long stime = 0;
			char sip[16] = "";
			char dip[16] = "";
			unsigned long service = 0;
			char mesg[256] = "";
			char corpname[256] = "";
			char s_time[21] = "";
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_STRING, sizeof(dip)-1,  	dip,
					 UT_TYPE_ULONG,  4,              	&service,
					 UT_TYPE_STRING, sizeof(mesg)-1,        mesg);
			while((iret == 0) || (iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
				strcpy(corpname, ncsUtlGetClientNameById(psShmHead,did,"未知"));
				strcpy(s_time, utTimFormat("%Y/%m/%d %H:%M:%S", stime));
				squeeze(dispname, '-');
				utPltPutLoopVarF(psDbHead,"corpname",iNum,"%s_%lu", corpname, did);
				utPltPutLoopVar(psDbHead,"stime",iNum,s_time);
			
				utPltPutLoopVar(psDbHead,"sip",iNum,sip);
				utPltPutLoopVar(psDbHead,"dip",iNum,dip);
				lIp=ntohl(utComHostAddress(dip));
          iReturn = pasUtlGetIpLocation(lIp,caArea,caName2);
      //    printf("iReturn=%d,lIp=%lu,caArea=%s\n",iReturn,lIp,caArea);
          if(iReturn == 1) {
            sprintf(caTemp,"%s,%s",caArea,caName2);
    //        printf("caTemp=%s\n",caTemp);
          }
          else{
                strcpy(caTemp,"");
          }
        utPltPutLoopVar(psDbHead,"iplocal",iNum,caTemp);
				
				utPltPutLoopVarF(psDbHead,"othlog_param",iNum,"%s_%s_%lu_%lu", _sdate, _edate, did, uid);
				psData = (struct ncsNettype_s *)pasLHashLook(pHashService,&service); 
	      if(psData){
	          strcpy(_service,psData->caName);
	      }
	      else{
	      	strcpy(_service,"");
	      }
utPltSetCvtHtml(1);	
	
				utPltPutLoopVar(psDbHead, "netsestype", iNum,  _service);	
				utPltPutLoopVar(psDbHead, "mesg", iNum,  mesg);	
				utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s_%lu_%lu", dispname, did, uid);
utPltSetCvtHtml(0);
				if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,7,UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,dispname,
                          UT_TYPE_STRING,_service,
                          UT_TYPE_STRING,s_time,
                          UT_TYPE_STRING,sip,
                          UT_TYPE_STRING,dip,
                          UT_TYPE_STRING,mesg);
                                                  
				 }
				
				uid = did = stime = service = 0;
				memset(dispname, 0, sizeof(dispname));
				memset(sip, 0, sizeof(sip));
				memset(dip, 0, sizeof(dip));
				memset(mesg, 0, sizeof(mesg));
				iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_STRING, sizeof(dip)-1,  	dip,
					 UT_TYPE_ULONG,  4,              	&service,
					 UT_TYPE_STRING, sizeof(mesg)-1,        mesg);
			}			 
			pasDbCloseCursor(psCur);
		} 
		free(tmpgbk);
		if(_break == 1)
			break; 
	}
	free(_keyword_gbk);
	free(_dispname_gbk);
	if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }

	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/search/ncs_netses_list.html");

	return iret;
}

// iplog记录
int ncsIplogSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	ncsNettype *psData;
  char caType[32];
  char caExport[32];
  FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];
	char instr_t[8192] = "";
	int iTime_flag = 0;
	int days = 0;   
	int iret = 0;
	int _break = 0;
	int i = 0;
	int lCount = 0;
	int num = 0;
	int iNum = 0;
	
	unsigned long did = 0;
	unsigned long iStart = 0;
	unsigned long iLimit = 0;
	unsigned long lCount1 = 0;
	unsigned long lStartTime = 0;
   	unsigned long lTime = time(0);
   	unsigned long lTotRec = 0;
	
   	char sdate[11] = "";
   	char edate[11] = "";
   	char time_flag[2] = "";
   	char start[17] = "";
   	char limit[17] = "";
   	char sort[17] = "";
   	char dir[9] = "";
   	char sql[4096] = "";
   	char _sip[16] = "";
   	char _idno[19] = "";
   	char _dispname[33] = "";
   	char corpname[129] = "";
   	char _groupid[33] = "";
   	char _did[33] = "";
   	char _uid[17] = "";
   	char instr[8192] = "";	
   	char sql2[256] = "";
   	char frmfile[256] = "";
   	//char _position[33] = "";
   	//char _mac[19] = "";
   	char *tmpgbk = NULL;
	char **ptmp = NULL;	
	char _proto[3] = "";
	char _dport[17] = "";
	char _service[17] = "";
	char _dip[16] = "";
	char _keyword[33] = "";
	struct sttable
	{
		char tname[32];
		unsigned long lCount;
	};
	char caArea[256],caName2[256];
	struct sttable tableinfo[120];//10年？
   	pasDbCursor *psCur = NULL;
   	char caGroups[1024];
   char caFacs[1024];
    char caStime[16];
   	char caEtime[16];
	 strcpy(caFacs,getDsFacDid());
   strcpy(caGroups,getDsGroupid());
   	
   	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
   	utMsgGetSomeNVar(psMsgHead, 22,
   				     "keyword",  UT_TYPE_STRING,  sizeof(_keyword)-1, _keyword,
   				     "uid",      UT_TYPE_STRING,  sizeof(_uid)-1,     _uid,
   				     "proto",    UT_TYPE_STRING,  sizeof(_proto)-1,   _proto,
   				     "dport",    UT_TYPE_STRING,  sizeof(_dport)-1,   _dport,
   				     "service",  UT_TYPE_STRING,  sizeof(_service)-1, _service,
   				     "dip",      UT_TYPE_STRING,  sizeof(_dip)-1,     _dip,
   				     "groupid",  UT_TYPE_STRING,  sizeof(_groupid)-1, _groupid,
   				     "did",      UT_TYPE_STRING,  sizeof(_did)-1,     _did,
   				     "corpname", UT_TYPE_STRING,  sizeof(corpname)-1, corpname,
   				     "sip",      UT_TYPE_STRING,  sizeof(_sip)-1,     _sip,  
   				     "idno",     UT_TYPE_STRING,  sizeof(_idno)-1,    _idno,
   				     "dispname", UT_TYPE_STRING,  sizeof(_dispname)-1, _dispname,
   				     "sdate", 	 UT_TYPE_STRING,  sizeof(sdate)-1,     sdate,
   				     "edate", 	 UT_TYPE_STRING,  sizeof(edate)-1,     edate,
   				     "time_flag",UT_TYPE_STRING,  sizeof(time_flag)-1, time_flag,
   				     "start",    UT_TYPE_STRING,  sizeof(start)-1,     start,
   				     "limit",    UT_TYPE_STRING,  sizeof(limit)-1,     limit,
   				     "sort",     UT_TYPE_STRING,  sizeof(sort)-1,      sort,
   				     "dir",      UT_TYPE_STRING,  sizeof(dir)-1,       dir,
   				      "exp",      UT_TYPE_STRING,  10,caExport,
   				     "stime",    UT_TYPE_STRING, 10,caStime,
   				     "etime",    UT_TYPE_STRING, 10,caEtime);
   	char *_dispname_gbk = NULL;
   	   	   	   if(strlen(caStime)==0){
   				     	strcpy(caStime,"00:00:00");
   				  }
   				  if(strlen(caEtime)==0){
   				    	strcpy(caEtime,"23:59:59");
   				  }	
   	
   if(strlen(sort)==0){
 	   strcpy(sort,"stime");
   }
	if(!utStrIsSpaces(_dispname) )
	{
		_dispname_gbk = convert("UTF-8", "GBK", _dispname);
	}
   	iTime_flag = atoi(time_flag);
   	switch(iTime_flag)
   	{
   		case 1://今天
   			days = 1;
   			break;
   		case 2://最近三天
   			days = 3;
   			break;
   		case 3://最近一周
   			days = 7;
   			break;
   		case 4://最近一月
   			days = 30;
   			break;
   		case 5://最近三月
   			days = 90;
   			break;
   		case 6://最近半年
   			days = 180;
   			break;
   		default:
   			days = 1;
   			break;
   	}	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncsiplog_if_", &lStartTime, &lTime, &lCount);
	}
	else 
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncsiplog_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}
	if(!utStrIsSpaces(corpname))//指定了单位名称查询
	{
		tmpgbk = convert("UTF-8", "GBK", corpname);
		snprintf(sql2, sizeof(sql2), "select userid from ncsuser where (dispname like '%c%s%c' or username like '%c%s%c' )", '%',tmpgbk,'%', '%',tmpgbk,'%');
		free(tmpgbk);
		iNum=0;
		memset(instr,0,sizeof(instr));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			memset(instr,0,sizeof(instr));
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), ",");
				}
				snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	if(!utStrIsSpaces(_did))
	{
		did = atol(_did);
	}
	if(!utStrIsSpaces(_groupid))
	{
		snprintf(sql2, sizeof(sql2), "select groupid from ncsgroup where  groupid = %lu or pid=%lu ", atol(_groupid),atol(_groupid));
		printf("sql2=%s\n",sql2);
		iNum=0;
		memset(instr_t,0,sizeof(instr_t));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), ",");
				}
				snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	char *_keyword_gbk = convert("UTF-8", "GBK", _keyword);
	char _sdate[11] = "";
	char _edate[11] = "";
	strcpy(_sdate, utTimFormat("%Y/%m/%d", lStartTime));
	strcpy(_edate, utTimFormat("%Y/%m/%d", lTime));
	for(i = 0; i < lCount; i++)
	{
		snprintf(frmfile, sizeof(frmfile), "/usr/local/infobright/data/ncs/%s.frm", ptmp[i]);
//		if(access(frmfile, F_OK) == 0)
//		{
			lCount1 = 0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu ",  ptmp[i], lStartTime, lTime);
			
			if(!utStrIsSpaces(_keyword))
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), "and (dport = '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or idno like '%%%s%%' or dispname like '%%%s%%')", _keyword, _keyword, _keyword, _keyword, _keyword);
			}
			if(!utStrIsSpaces(_proto) && atol(_proto) > 0 )//1(协议类型TCP,UDP)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and trantype = %lu", atol(_proto));
			}
			if(!utStrIsSpaces(_dport) )//2(目标端口查询)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dport = %lu", atol(_dport));
			}
			if(!utStrIsSpaces(_service) )//服务查询
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and service = %lu", atol(_service));
			}
			if(!utStrIsSpaces(_sip) )//源IP地址
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
			}
			if(!utStrIsSpaces(_dip) )//目标IP地址
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
			}
			if(!utStrIsSpaces(_idno) )//4
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
			}
			if(!utStrIsSpaces(_dispname) )//依据显示名查询
			{
				if(_dispname_gbk == NULL)
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
				}
				else 
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%')", _dispname);
				}
			}
			if(!utStrIsSpaces(corpname))//6
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
			}
			if(!utStrIsSpaces(_did))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
			}
			if(!utStrIsSpaces(_uid))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
			}
			if(!utStrIsSpaces(_groupid))//8
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
			}
			if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
			
			tmpgbk = convert("UTF-8", "GBK", sql);
			if(tmpgbk != NULL)
			{
				printf("sql = %s\n", tmpgbk);
				pasDbOneRecord(tmpgbk, 0, UT_TYPE_ULONG, 4, &lCount1);
				free(tmpgbk);
			}
			if(lCount1 > 0)
			{
				strcpy(tableinfo[num].tname, ptmp[i]);
				tableinfo[num].lCount = lCount1;
				num++;
			}
//		}
		free(ptmp[i]);
	}
	free(ptmp);	
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	
	//
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	}
	//
	
	iNum = 0;
	iStart = atol(start);
	iLimit = atol(limit);
	
	if(strlen(caExport)>0){
		
		iStart=0;
		iLimit=99999;
		        sprintf(caFilename,"ncsiplog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"IPLOG日志\n");
		        ncUtlPrintCsv(fp,10,UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"服务名称",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"源IP",
		                           UT_TYPE_STRING,"目标IP",
		                           UT_TYPE_STRING,"目标端口",
		                           UT_TYPE_STRING,"协议",
		                           UT_TYPE_STRING,"上行流量(字节)",
		                           UT_TYPE_STRING,"下行流量(字节)");
		                          
		                         
				
			  
		
		
	}
	
	
	
	for(i = num-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		snprintf(sql, sizeof(sql), "select uid, did, dispname, stime, sip, dip, dport, trantype, service, ubytes, dbytes from %s where stime >= %lu and stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);
		if(!utStrIsSpaces(_keyword))
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), "and (dport = '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or idno like '%%%s%%' or dispname like '%%%s%%')", _keyword, _keyword, _keyword, _keyword, _keyword);
		}
		if(!utStrIsSpaces(_proto) && atol(_proto) > 0 )//1(协议类型TCP,UDP)
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and trantype = %lu", atol(_proto));
		}
		if(!utStrIsSpaces(_dport) )//2(目标端口查询)
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dport = %lu", atol(_dport));
		}
		if(!utStrIsSpaces(_service) )//服务查询
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and service = %lu", atol(_service));
		}
		if(!utStrIsSpaces(_sip) )//源IP地址
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
		}
		if(!utStrIsSpaces(_dip) )//目标IP地址
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
		}
		if(!utStrIsSpaces(_idno) )//4
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
		}
		if(!utStrIsSpaces(_dispname) )//依据显示名查询
		{
			if(_dispname_gbk == NULL)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
			}
			else 
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
			}
		}
		if(!utStrIsSpaces(corpname))//6
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
		}
		if(!utStrIsSpaces(_did))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
		}
		if(!utStrIsSpaces(_uid))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
		}
		if(!utStrIsSpaces(_groupid))//8
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
		}
		if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
		
		
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			_break = 1;
		}
		tmpgbk = convert("UTF-8", "GBK", sql);
		if(tmpgbk == NULL)
			continue;
		printf("__line = %d, sql = %s\n", __LINE__, tmpgbk);
		psCur = pasDbOpenSql(tmpgbk, 0);
		if(psCur != NULL)
		{
			unsigned long uid = 0;
			unsigned long did = 0;
			char dispname[33] ="";
			unsigned long stime = 0;
			char sip[16] = "";
			char dip[16] = "";
			unsigned long dport = 0;
			unsigned long trantype = 0;
			unsigned long service = 0;
			unsigned long ubytes = 0;
			unsigned long dbytes = 0;
			char corpname[256] = "";
			char s_time[21] = "";
			unsigned long lIp;
			long iReturn;
			char caTemp[1024];
			char _service[33] = "";
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_STRING, sizeof(dip)-1,  	dip,
					 UT_TYPE_ULONG,  4,              	&dport,
					 UT_TYPE_ULONG,  4,              	&trantype,
					 UT_TYPE_ULONG,  4,              	&service,
					 UT_TYPE_ULONG,  4,              	&ubytes,
					 UT_TYPE_ULONG,  4,              	&dbytes);
					 
			while((iret == 0) || (iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
				strcpy(corpname, ncsUtlGetClientNameById(psShmHead,did,"未知"));
				strcpy(s_time, utTimFormat("%Y/%m/%d %H:%M:%S", stime));
				squeeze(dispname, '-');
				utPltPutLoopVarF(psDbHead,"corpname",iNum,"%s_%lu", corpname, did);
				utPltPutLoopVar(psDbHead,"stime",iNum,s_time);
				utPltSetCvtHtml(1);
				utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s_%lu_%lu", dispname, did, uid);
				utPltSetCvtHtml(0);
				utPltPutLoopVar(psDbHead,"sip",iNum,sip);
				utPltPutLoopVar(psDbHead,"dip",iNum,dip);

           lIp=ntohl(utComHostAddress(dip));
          iReturn = pasUtlGetIpLocation(lIp,caArea,caName2);
      //    printf("iReturn=%d,lIp=%lu,caArea=%s\n",iReturn,lIp,caArea);
          if(iReturn == 1) {
            sprintf(caTemp,"%s,%s",caArea,caName2);
    //        printf("caTemp=%s\n",caTemp);
          }
          else{
                strcpy(caTemp,"");
          }

        utPltPutLoopVar(psDbHead,"iplocal",iNum,caTemp);
				
				utPltPutLoopVarF(psDbHead, "ubytes", iNum, "%lu", ubytes);
				utPltPutLoopVarF(psDbHead, "dbytes", iNum, "%lu", dbytes);
				utPltPutLoopVarF(psDbHead, "dport", iNum, "%lu",  dport);
				utPltPutLoopVar(psDbHead, "proto", iNum, trantype == 6 ? "TCP" : "UDP");
				utPltPutLoopVarF(psDbHead,"did",iNum,"%d",did);
				utPltPutLoopVarF(psDbHead,"uid",iNum,"%d",uid);
				psData = (struct ncsNettype_s *)pasLHashLook(pHashService,&service); 
			  if(psData){
	        strcpy(_service,psData->caName);
	      }
	      else{
	      	strcpy(_service,"");
	      }
				utPltPutLoopVar(psDbHead, "service", iNum,  _service);	
				utPltPutLoopVarF(psDbHead,"othlog_param",iNum,"%s_%s_%lu_%lu", _sdate, _edate, did, uid);
				
				if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,10,UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,dispname,
                          UT_TYPE_STRING,_service,
                          UT_TYPE_STRING,s_time,
                          UT_TYPE_STRING,sip,
                          UT_TYPE_STRING,dip,
                          UT_TYPE_LONG,dport,
                          UT_TYPE_STRING,trantype == 6 ? "TCP" : "UDP",
                          UT_TYPE_LONG,ubytes,
                          UT_TYPE_LONG,dbytes);
                         
                                                  
				 }
				
				uid = did = stime = dport = trantype = service = ubytes = dbytes = 0;
				memset(dispname, 0, sizeof(dispname));
				memset(sip, 0, sizeof(sip));
				memset(dip, 0, sizeof(dip));
				iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_STRING, sizeof(dip)-1,  	dip,
					 UT_TYPE_ULONG,  4,              	&dport,
					 UT_TYPE_ULONG,  4,              	&trantype,
					 UT_TYPE_ULONG,  4,              	&service,
					 UT_TYPE_ULONG,  4,              	&ubytes,
					 UT_TYPE_ULONG,  4,              	&dbytes);
			}			 
			pasDbCloseCursor(psCur);
		} 
		free(tmpgbk);
		if(_break == 1)
			break; 
	}
	free(_keyword_gbk);
	free(_dispname_gbk);
	
	if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/search/ncs_iplog_list.html");
	return iret;
}


//bbs论坛
int ncsBbsSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	ncsNettype *psData;
  char caType[32];
  char caExport[32];
  FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];
	char instr_t[8192] = "";
	int iTime_flag = 0;
	int days = 0;   
	int iret = 0;
	int _break = 0;
	int i = 0;
	int lCount = 0;
	int num = 0;
	int iNum = 0;
	
	unsigned long did = 0;
	unsigned long iStart = 0;
	unsigned long iLimit = 0;
	unsigned long lCount1 = 0;
	unsigned long lStartTime = 0;
   	unsigned long lTime = time(0);
   	unsigned long lTotRec = 0;
	
   	char sdate[11] = "";
   	char edate[11] = "";
   	char time_flag[2] = "";
   	char start[17] = "";
   	char limit[17] = "";
   	char sort[17] = "";
   	char dir[9] = "";
   	char sql[4096] = "";
   	char _sip[16] = "";
   	char _idno[19] = "";
   	char _dispname[33] = "";
   	char corpname[129] = "";
   	char _groupid[33] = "";
   	char _did[33] = "";
   	char _uid[17] = "";
   	char instr[8192] = "";	
   	char sql2[256] = "";
   	char frmfile[256] = "";
   	//char _position[33] = "";
   	//char _mac[19] = "";
   	char *tmpgbk = NULL;
	char **ptmp = NULL;	
	char _proto[3] = "";
	char _dport[17] = "";
	char _service[17] = "";
	char _dip[16] = "";
	char _keyword[64] = "";
	char _keyword_red[64]="";
	char _bname[52],_upfile[52],_author[52],_content[2048],_subject[256],_url[256],_host[128],_mac[20];
	char _author_gbk[52],_bname_gbk[52],_subject_gbk[256],_content_gbk[2048];
	char caTemp[2048];
	struct sttable
	{
		char tname[32];
		unsigned long lCount;
	};
	struct sttable tableinfo[120];//10年？
   	pasDbCursor *psCur = NULL;
  char caGroups[1024];
  char caFacs[1024];
  char _dispname_temp[36];
  char caArea[256],caName2[256];
   	char caStime[16];
   	char caEtime[16];
   long iReturn;
   unsigned long lIp;
	 strcpy(caFacs,getDsFacDid());
   strcpy(caGroups,getDsGroupid());
   	
   	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
   	utMsgGetSomeNVar(psMsgHead, 25,
   				     "keyword",  UT_TYPE_STRING,  sizeof(_keyword)-1, _keyword,
   				     "uid",      UT_TYPE_STRING,  sizeof(_uid)-1,     _uid,
   				     "mac",    UT_TYPE_STRING,  sizeof(_mac)-1,   _mac,
   				     "host",    UT_TYPE_STRING,  sizeof(_host)-1,   _host,
   				     "subject",  UT_TYPE_STRING,  sizeof(_subject)-1, _subject,
   				     "content",    UT_TYPE_STRING,  sizeof(_content)-1,   _content,
   				     "bname",    UT_TYPE_STRING,  sizeof(_bname)-1,   _bname,
   				     "dip",      UT_TYPE_STRING,  sizeof(_dip)-1,     _dip,
   				     "groupid",  UT_TYPE_STRING,  sizeof(_groupid)-1, _groupid,
   				     "did",      UT_TYPE_STRING,  sizeof(_did)-1,     _did,
   				     "corpname", UT_TYPE_STRING,  sizeof(corpname)-1, corpname,
   				     "sip",      UT_TYPE_STRING,  sizeof(_sip)-1,     _sip,  
   				     "idno",     UT_TYPE_STRING,  sizeof(_idno)-1,    _idno,
   				     "dispname", UT_TYPE_STRING,  sizeof(_dispname)-1, _dispname,
   				     "sdate", 	 UT_TYPE_STRING,  sizeof(sdate)-1,     sdate,
   				     "edate", 	 UT_TYPE_STRING,  sizeof(edate)-1,     edate,
   				     "time_flag",UT_TYPE_STRING,  sizeof(time_flag)-1, time_flag,
   				     "author",   UT_TYPE_STRING,  sizeof(_author)-1,  _author,
   				     "start",    UT_TYPE_STRING,  sizeof(start)-1,     start,
   				     "limit",    UT_TYPE_STRING,  sizeof(limit)-1,     limit,
   				     "sort",     UT_TYPE_STRING,  sizeof(sort)-1,      sort,
   				     "dir",      UT_TYPE_STRING,  sizeof(dir)-1,       dir,
   				     "exp",      UT_TYPE_STRING,  10,caExport,
   				     "stime",    UT_TYPE_STRING, 10,caStime,
   				     "etime",    UT_TYPE_STRING, 10,caEtime);
   				     
   	   	   if(strlen(caStime)==0){
   				     	strcpy(caStime,"00:00:00");
   				  }
   				  if(strlen(caEtime)==0){
   				    	strcpy(caEtime,"23:59:59");
   				  }				     
   	if(strlen(_keyword)>0){
	  	sprintf(_keyword_red,"<em>%s</em>",_keyword);
	  }
   	char *_dispname_gbk = NULL;
   if(strlen(sort)==0){
 	   strcpy(sort,"stime");
   }
	if(!utStrIsSpaces(_dispname) )
	{
		_dispname_gbk = convert("UTF-8", "GBK", _dispname);
	}
   	iTime_flag = atoi(time_flag);
   	switch(iTime_flag)
   	{
   		case 1://今天
   			days = 1;
   			break;
   		case 2://最近三天
   			days = 3;
   			break;
   		case 3://最近一周
   			days = 7;
   			break;
   		case 4://最近一月
   			days = 30;
   			break;
   		case 5://最近三月
   			days = 90;
   			break;
   		case 6://最近半年
   			days = 180;
   			break;
   		default:
   			days = 1;
   			break;
   	}	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncsbbsmon_if_", &lStartTime, &lTime, &lCount);
	}
	else 
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncsbbsmon_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}
	if(!utStrIsSpaces(corpname))//指定了单位名称查询
	{
		tmpgbk = convert("UTF-8", "GBK", corpname);
		snprintf(sql2, sizeof(sql2), "select userid from ncsuser where (dispname like '%c%s%c' or username like '%c%s%c' )", '%',tmpgbk,'%', '%',tmpgbk,'%');
		free(tmpgbk);
		iNum=0;
		memset(instr,0,sizeof(instr));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			memset(instr,0,sizeof(instr));
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), ",");
				}
				snprintf(instr+strlen(instr), sizeof(instr)-strlen(instr), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	if(!utStrIsSpaces(_did))
	{
		did = atol(_did);
	}
	if(!utStrIsSpaces(_groupid))
	{
		snprintf(sql2, sizeof(sql2), "select groupid from ncsgroup where  groupid = %lu or pid=%lu ", atol(_groupid),atol(_groupid));
		printf("sql2=%s\n",sql2);
		iNum=0;
		memset(instr_t,0,sizeof(instr_t));
		psCur = pasDbOpenSql(sql2, 0);
		if(psCur != NULL)
		{
			unsigned long userid = 0;
			iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			
			while((iret == 0)||(iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), ",");
				}
				snprintf(instr_t+strlen(instr_t), sizeof(instr_t)-strlen(instr_t), "%lu", userid);
				userid = 0;
				iret = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &userid);
			}
			pasDbCloseCursor(psCur);
		}
	}
	char *_keyword_gbk = convert("UTF-8", "GBK", _keyword);
	char _sdate[11] = "";
	char _edate[11] = "";
	strcpy(_sdate, utTimFormat("%Y/%m/%d", lStartTime));
	strcpy(_edate, utTimFormat("%Y/%m/%d", lTime));
	for(i = 0; i < lCount; i++)
	{
		snprintf(frmfile, sizeof(frmfile), "/usr/local/infobright/data/ncs/%s.frm", ptmp[i]);
//		if(access(frmfile, F_OK) == 0)
//		{
			lCount1 = 0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu ",  ptmp[i], lStartTime, lTime);
			
			if(!utStrIsSpaces(_keyword))
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), "and (bbsname = '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or idno like '%%%s%%' or dispname like '%%%s%%' or url like '%%%s%%' or subject like '%%%s%%' or content like '%%%s%%' or author like '%%%s%%' )", _keyword, _keyword, _keyword, _keyword, _keyword, _keyword, _keyword, _keyword, _keyword);
			}
			
			if(!utStrIsSpaces(_mac) )//2(MAC查询)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%' ",_mac);
			}
			if(!utStrIsSpaces(_host) )//url查询
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and url like '%%%s%%' ",_host);
			}
			
			if(!utStrIsSpaces(_subject) )//2(subjectC查询)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and subject like '%%%s%%' ",_subject);
			}
			if(!utStrIsSpaces(_content) )//content查询
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and content like '%%%s%%' ",_content);
			}
			
			if(!utStrIsSpaces(_bname) )//bbsname查询
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and bbsname like '%%%s%%' ",_bname);
			}
			if(!utStrIsSpaces(_author) )//auth查询
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and author like '%%%s%%' ",_author);
			}
			
			if(!utStrIsSpaces(_sip) )//源IP地址
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
			}
			if(!utStrIsSpaces(_dip) )//目标IP地址
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
			}
			if(!utStrIsSpaces(_idno) )//4
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
			}
			if(!utStrIsSpaces(_dispname) )//依据显示名查询
			{
				if(_dispname_gbk == NULL)
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
				}
				else 
				{
					snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
				}
			}
			if(!utStrIsSpaces(corpname))//6
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
			}
			if(!utStrIsSpaces(_did))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
			}
			if(!utStrIsSpaces(_uid))//7
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
			}
			if(!utStrIsSpaces(_groupid))//8
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
			}
			
			if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
			}
	printf("sql=%s\n",sql);		
			pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
//			tmpgbk = convert("UTF-8", "GBK", sql);
//			if(tmpgbk != NULL)
//			{
//				printf("sql = %s\n", tmpgbk);
//				pasDbOneRecord(tmpgbk, 0, UT_TYPE_ULONG, 4, &lCount1);
//				free(tmpgbk);
//			}
			if(lCount1 > 0)
			{
				strcpy(tableinfo[num].tname, ptmp[i]);
				tableinfo[num].lCount = lCount1;
				num++;
			}
//		}
		free(ptmp[i]);
	}
	free(ptmp);	
	
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	
	//
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	}
	//
	
	iNum = 0;
	iStart = atol(start);
	iLimit = atol(limit);
	
	if(strlen(caExport)>0){
		
		iStart=0;
		iLimit=99999;
		        sprintf(caFilename,"ncsbbslog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"bbs日志\n");
		        ncUtlPrintCsv(fp,10,UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"论坛名称",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"源IP",
		                           UT_TYPE_STRING,"目标IP",
		                           UT_TYPE_STRING,"作者",
		                           UT_TYPE_STRING,"主题",
		                           UT_TYPE_STRING,"内容",
		                           UT_TYPE_STRING,"URL");
		                          
		                         
				
			  
		
		
	}
	
	
	
	for(i = num-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		snprintf(sql, sizeof(sql), "select uid, did, dispname, stime, sip,dip,bbsname,url,upfile,subject,author,content,mac,position from %s where stime >= %lu and stime <= %lu ",tableinfo[i].tname, lStartTime, lTime);
			if(!utStrIsSpaces(_keyword))
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), "and (bbsname = '%%%s%%' or sip like '%%%s%%' or dip like '%%%s%%' or idno like '%%%s%%' or dispname like '%%%s%%' or url like '%%%s%%' or subject like '%%%s%%' or content like '%%%s%%' or author like '%%%s%%' )", _keyword, _keyword, _keyword, _keyword, _keyword, _keyword, _keyword, _keyword, _keyword);
			}
			
			if(!utStrIsSpaces(_mac) )//2(MAC查询)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac like '%%%s%%' ",_mac);
			}
			if(!utStrIsSpaces(_host) )//url查询
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and url like '%%%s%%' ",_host);
			}
			
			if(!utStrIsSpaces(_subject) )//2(subjectC查询)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and subject like '%%%s%%' ",_subject);
			}
			if(!utStrIsSpaces(_content) )//content查询
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and content like '%%%s%%' ",_content);
			}
			
			if(!utStrIsSpaces(_bname) )//bbsname查询
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and bbsname like '%%%s%%' ",_bname);
			}
			if(!utStrIsSpaces(_author) )//auth查询
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and author like '%%%s%%' ",_author);
			}
		if(!utStrIsSpaces(_sip) )//源IP地址
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and sip like '%%%s%%'", _sip);
		}
		if(!utStrIsSpaces(_dip) )//目标IP地址
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dip like '%%%s%%'", _dip);
		}
		if(!utStrIsSpaces(_idno) )//4
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and idno like '%%%s%%'", _idno);
		}
		if(!utStrIsSpaces(_dispname) )//依据显示名查询
		{
			if(_dispname_gbk == NULL)
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and dispname like '%%%s%%'", _dispname);
			}
			else 
			{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (dispname like '%%%s%%' or dispname like '%%%s%%')", _dispname, _dispname_gbk);
			}
		}
		if(!utStrIsSpaces(corpname))//6
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did in (%s)", instr);
		}
		if(!utStrIsSpaces(_did))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and did = %lu", did);
		}
		if(!utStrIsSpaces(_uid))//7
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and uid = %lu", atol(_uid));
		}
		if(!utStrIsSpaces(_groupid))//8
		{
			snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and tid in (%s)", instr_t);
		}
		if(strlen(caGroups)>0){
				sprintf(sql+strlen(sql)," and tid in (%s) ",caGroups);
		}
		if(strlen(caFacs)>0){
				if(utStrIsSpaces(_did)){		
				  sprintf(sql+strlen(sql)," and did in (%s) ",caFacs);
			  }
		}
		
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by %s %s limit %lu, %lu", sort, dir, iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
		
		
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			_break = 1;
		}
//		tmpgbk = convert("UTF-8", "GBK", sql);
//		if(tmpgbk == NULL)
//			continue;
//		printf("__line = %d, sql = %s\n", __LINE__, tmpgbk);
printf("sql=%s\n",sql);		
			psCur = pasDbOpenSql(sql, 0);
//		psCur = pasDbOpenSql(tmpgbk, 0);
		if(psCur != NULL)
		{
			unsigned long uid = 0;
			unsigned long did = 0;
			char dispname[33] ="";
			char _position[32];
			unsigned long stime = 0;
			char sip[16] = "";
			char dip[16] = "";
      
			char corpname[256] = "";
			char s_time[21] = "";
			
			char _service[33] = "";
			
			memset(_bname,0,sizeof(_bname));
			memset(_url,0,sizeof(_url));
			memset(_subject,0,sizeof(_subject));
			memset(_upfile,0,sizeof(_upfile));
			memset(_author,0,sizeof(_author));
			memset(_content,0,sizeof(_content));
			memset(_mac,0,sizeof(_mac));
			memset(_position,0,sizeof(_position));
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_STRING, sizeof(dip)-1,  	dip,
					 UT_TYPE_STRING,sizeof(_bname)-1,_bname,
					 UT_TYPE_STRING,sizeof(_url)-1,_url,
					 UT_TYPE_STRING,sizeof(_upfile)-1,_upfile,
					 UT_TYPE_STRING,sizeof(_subject)-1,_subject,
					 UT_TYPE_STRING,sizeof(_author)-1,_author,
					 UT_TYPE_STRING,sizeof(_content)-1,_content,
					 UT_TYPE_STRING,sizeof(_mac)-1,_mac,
					 UT_TYPE_STRING,sizeof(_position)-1,_position);
	
			while((iret == 0) || (iret == 1405))
			{
				iNum++;
				if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
				strcpy(corpname, ncsUtlGetClientNameById(psShmHead,did,"未知"));
				strcpy(s_time, utTimFormat("%Y/%m/%d %H:%M:%S", stime));
				strcpy(_author_gbk,pasCvtGBK(2,_author,sql,47));
				strcpy(_bname_gbk,pasCvtGBK(2,_bname,sql,47));
				strcpy(_subject_gbk,pasCvtGBK(2,_subject,sql,255));
				strcpy(_content_gbk,pasCvtGBK(2,_content,sql,2047));
				strcpy(_dispname_temp,pasCvtGBK(2,dispname,sql,31));
				
				utPltPutLoopVarF(psDbHead,"corpname",iNum,"%s", corpname, did);
				utPltPutLoopVar(psDbHead,"stime",iNum,s_time);
				utPltSetCvtHtml(1);
				utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s", _dispname_temp);
				utPltPutLoopVar(psDbHead,"author",iNum,_author_gbk);
				utPltPutLoopVar(psDbHead,"bname",iNum,_bname_gbk);
//				printf("len=%d\n",strlen(_keyword_gbk));
	if(strlen(_keyword)>0){	
			sprintf(_keyword_red,"<em>%s</em>",_keyword_gbk);
//		printf("red=%s\n",_keyword_red);
			ncsReplaceStr(_subject_gbk, _subject, _keyword_gbk, _keyword_red);
			strcpy(_subject_gbk,_subject);
			
			ncsReplaceStr(_content_gbk, _content, _keyword_gbk, _keyword_red);
			strcpy(_content_gbk,_content);
			
			ncsReplaceStr( _url,caTemp,_keyword_gbk, _keyword_red);
			strcpy(_url,caTemp);
			
			ncsReplaceStr(sip,caTemp,_keyword_gbk, _keyword_red);
			strcpy(sip,caTemp);
			ncsReplaceStr(dip,caTemp,_keyword_gbk, _keyword_red);
			strcpy(dip,caTemp);
			
//			utStrReplaceWith(_subject_gbk,_keyword_gbk,_keyword_red);
//			utStrReplaceWith(_content_gbk,_keyword_gbk,_keyword_red);
//			utStrReplaceWith(_url,_keyword_gbk,_keyword_red);
//			utStrReplaceWith(sip,_keyword_gbk,_keyword_red);
//			utStrReplaceWith(dip,_keyword_gbk,_keyword_red);
			
			}
//printf("_keyword_gbk=%s,keyword_red=%s,subject_gbk=%s\n",_keyword_gbk,_keyword_red,_subject_gbk);
				utPltPutLoopVar(psDbHead,"subject",iNum,_subject_gbk);
				utPltPutLoopVar(psDbHead,"content",iNum,_content_gbk);
				utPltPutLoopVar(psDbHead,"url",iNum,_url);
				utPltPutLoopVar(psDbHead,"sip",iNum,sip);
				utPltPutLoopVar(psDbHead,"dip",iNum,dip);
	
	        lIp=ntohl(utComHostAddress(dip));
          iReturn = pasUtlGetIpLocation(lIp,caArea,caName2);
      //    printf("iReturn=%d,lIp=%lu,caArea=%s\n",iReturn,lIp,caArea);
          if(iReturn == 1) {
            sprintf(caTemp,"%s,%s",caArea,caName2);
    //        printf("caTemp=%s\n",caTemp);
          }
          else{
                strcpy(caTemp,"");
          }
        utPltPutLoopVar(psDbHead,"iplocal",iNum,caTemp);
	
        utPltPutLoopVar(psDbHead,"mac",iNum,_mac);
        utPltPutLoopVar(psDbHead,"position",iNum,_position);
				
				utPltSetCvtHtml(0);

			 
				utPltPutLoopVarF(psDbHead,"did",iNum,"%d",did);
				utPltPutLoopVarF(psDbHead,"uid",iNum,"%d",uid);
			
				if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,10,UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,_dispname_temp,
                          UT_TYPE_STRING,_bname_gbk,
                          UT_TYPE_STRING,s_time,
                          UT_TYPE_STRING,sip,
                          UT_TYPE_STRING,dip,
                          UT_TYPE_STRING,_author_gbk,
                          UT_TYPE_STRING,_subject_gbk,
                          UT_TYPE_STRING,_content_gbk,
                          UT_TYPE_STRING,_url);
                        
                         
                                                  
				 }
				
				uid = did = stime = 0;
				memset(dispname, 0, sizeof(dispname));
				memset(sip, 0, sizeof(sip));
				memset(dip, 0, sizeof(dip));
				memset(_bname,0,sizeof(_bname));
				memset(_url,0,sizeof(_url));
				memset(_subject,0,sizeof(_subject));
				memset(_upfile,0,sizeof(_upfile));
				memset(_author,0,sizeof(_author));
				memset(_content,0,sizeof(_content));
			memset(_mac,0,sizeof(_mac));
			memset(_position,0,sizeof(_position));
			iret = pasDbFetchInto(psCur,
					 UT_TYPE_ULONG,  4,               	&uid,
					 UT_TYPE_ULONG,  4,              	&did,
					 UT_TYPE_STRING, sizeof(dispname)-1, 	dispname,
					 UT_TYPE_ULONG,  4,              	&stime,
					 UT_TYPE_STRING, sizeof(sip)-1,  	sip,
					 UT_TYPE_STRING, sizeof(dip)-1,  	dip,
					 UT_TYPE_STRING,sizeof(_bname)-1,_bname,
					 UT_TYPE_STRING,sizeof(_url)-1,_url,
					 UT_TYPE_STRING,sizeof(_upfile)-1,_upfile,
					 UT_TYPE_STRING,sizeof(_subject)-1,_subject,
					 UT_TYPE_STRING,sizeof(_author)-1,_author,
					 UT_TYPE_STRING,sizeof(_content)-1,_content,
					 UT_TYPE_STRING,sizeof(_mac)-1,_mac,
					 UT_TYPE_STRING,sizeof(_position)-1,_position);
			}			 
			pasDbCloseCursor(psCur);
		} 
		free(tmpgbk);
		if(_break == 1)
			break; 
	}
//	printf("ccccc\n");
	free(_keyword_gbk);
//	printf("qqqqqqqq\n");
	free(_dispname_gbk);
	
	if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/search/ncs_bbs_list.html");
	return iret;
}



void ncsInitncsFun_v4(void)
{
  pasSetTcpFunName("ncsSearchCorp_v4", ncsSearchCorp_v4, 0);
	pasSetTcpFunName("ncsServiceGroup_v4", ncsServiceGroup_v4, 0);
	pasSetTcpFunName("ncsServiceIndex_v4", ncsServiceIndex_v4, 0);
	pasSetTcpFunName("ncsUserlogSearch2_v4", ncsUserlogSearch2_v4, 0);//1(上下线日志)
	pasSetTcpFunName("ncsNetidSearch_v4", ncsNetidSearch_v4, 0);//1(虚拟身份日志)
	pasSetTcpFunName("ncsImlogSearch_v4", ncsImlogSearch_v4, 0);//1(即时通讯日志日志)
	pasSetTcpFunName("ncsMailSearch_v4", ncsMailSearch_v4, 0);//1(邮件日志)
	
	pasSetTcpFunName("ncsSearchlogSearch_v4", ncsSearchlogSearch_v4, 0);//1(搜索引擎日志)
	pasSetTcpFunName("ncsFormSearch_v4", ncsFormSearch_v4, 0);//1(搜索引擎日志)
	pasSetTcpFunName("ncsHttpSearch_v4",ncsHttpSearch_v4, 0);//1(网站日志)
	pasSetTcpFunName("ncsNetsesSearch_v4",ncsNetsesSearch_v4, 0);//1(网站日志)
	pasSetTcpFunName("ncsIplogSearch_v4",ncsIplogSearch_v4, 0);//1(IPLOG日志)
	pasSetTcpFunName("ncsBbsSearch_v4",ncsBbsSearch_v4, 0);//1(IPLOG日志)
		
}

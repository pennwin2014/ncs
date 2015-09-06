/* 监控软件---有关Web的应用
     Modify 2002/9/9   by Liyunming*/
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
#include "pasutl.h"
#include "mail.h"
#include "pasfile.h"
#include <fcntl.h>
#include "ncpost.h"
//#include "ncpost.h"
//#include "pasutl.h"

extern char *pHashNettype;
static char ncPkgCgiX2C(char *what)
{
  register char digit;
  digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
  digit *= 16;
  digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
  return(digit);
}
static char *ncPkgUnEscapeUrl(char *url)
{
  register int x,y;
  for(x = 0,y = 0; url[y]; x++, y++)
  {
    if ( (url[x] = url[y]) == '%')
    {
      url[x] = ncPkgCgiX2C(&url[y+1]);
      y+=2;
    }
    else if(url[x] == '+') {
        url[x] = ' ';
    }
  }
  url[x] = '\0';
 
 
  return url;
}







/* 将文本字段保存在文件中 

        pTables 表名，如 ncsformmon_if_201303

           系统只取该表名中的时间，转换为要查找内容的表 ncsfiles_201303

        pFile --- 文本字段取出来存放的文件名

        llFileId --- 文件ID

 

*/

 

int ncsUtlCvtNcsText2File(char *pTable,char *pFile,int8  llFileId)

{

    int l,iLen,iReturn;

    char *pText;

    char caSmt[128],caTable[32];

    l = strlen(pTable);

    sprintf(caTable,"ncsfiles_%s",pTable + l - 6);

    sprintf(caSmt,"select content from %s where fileid = %llu",caTable,llFileId);

    iReturn = pasDbOneRecord(caSmt,1,"fieldid",UT_TYPE_LONG8,llFileId,

                                     UT_TYPE_TEXT,&iLen,&pText);

    if(iReturn == 0) {

        FILE *fp;

        fp = fopen(pFile,"w");

        if(fp) {

            fwrite(pText,1,iLen,fp);

            fclose(fp);

            free(pText);

            return 1;

        }

        else {

            return 0;

        }

    }

    else {

        return (-1);

    }

}  




//统计个人网站访问次数
int ncsUserHttpStat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[20480];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;

  
   	float lRat;
   	long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;

	char sqlbuf[20480]="";
	unsigned long lStatrow,lSum;
   long lStime,lEtime;
 	pasDbCursor *psCur;
 
 	char caTable_hz[36];
 	char caSdate_d[16],caEdate_d[16];
 	char caTable[20],caUdisp[36];
 	long starttime;
  char caKeyword[64],caTime_flag[16];
  char caDid[16],caPage[12],caLimit[16],caSort[16],caDir[16];

  char caUid[16];
  char caExport[16];
  long mCount,lCmonth;

  char caHost[64];
  char caTemp_d[68];
  lCurPg=1;
 	strcpy(caWhere,"\0");

 	

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,11,
			"did",UT_TYPE_STRING,12,caDid,
			"uid",UT_TYPE_STRING,10,caUid,
			"sdate",UT_TYPE_STRING,19,caSdate,
			"edate",UT_TYPE_STRING,19,caEdate,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "export1",     UT_TYPE_STRING,10,caExport,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag);
   	  
    
      if(strlen(caKeyword)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
      }
      
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 

    if(!utStrIsSpaces(caExport)){
    lRowNum = 99999;
    lStartRec=0;
    }
    else{
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
    }
    
   	
  
  
   	                
   
   lTime=time(0);
   
   if(strlen(caSdate)>0||strlen(caEdate)>0){
   	  if(strlen(caSdate)==0){
   	  	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
		     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
		     sprintf(caEtemptime,"%s 23:59",caEdate,caEtime);
		     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
   	  }
   	  else{
   	  	sprintf(caStemptime,"%s 00:00",caSdate);
   	  	lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  	letime=lTime;
   	  }
   	 }
   	  else{
   	  	if(atol(caTime_flag)==1){
   	  		sprintf(caStemptime,"%s 00:00",utTimFormat("%Y/%m/%d",lTime));
 //  	  		printf("caStemptime=%s,lTime=%d\n",caStemptime,lTime);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		letime=lTime;
   	  	}
        else if(atol(caTime_flag)==2){
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==3){
        	lstime=lTime-7*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==4){
        	lstime=lTime-30*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==5){
        	lstime=lTime-90*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==6){
        	lstime=lTime-180*3600*24;
        	letime=lTime;
        }
        else{
        	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
	   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
			     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
			     sprintf(caEtemptime,"%s 23:59",caEdate_d);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	  }
             
   utStrDelSpaces(caKeyword);      
	 memset(caWhere,0,sizeof(caWhere));
	 
	    lCmonth=0;   
nn:
	 
	 
	 sprintf(caWhere,"where stime>=%d and stime<=%d ",lstime,letime); 
   if(atol(caDid)>0){
	  	  utStrAddF(caWhere," and did=%lu ",atol(caDid));
	  	}
    if(atol(caUid)>0){
    	utStrAddF(caWhere," and uid=%lu ",atol(caUid));
    }
   
     if(strlen(caKeyword)>0){
        utStrAddF(caWhere," and host like '%c%s%c' ", '%',caKeyword,'%');
     }
    	
    
    	
    psDbHead = utPltInitDb();
    
    
// printf("caWhere=%s\n",caWhere);  
    
    	
    	
   lSum=0;
   iNum=0;

   char caPre_table[64],caEtable[16];
   strcpy(caEtable,utTimFormat("%Y/%m",letime));
   long lEtabe=utTimStrToLong("%Y/%m",caEtable)+86400*30;
   memset(caPre_table,0,sizeof(caPre_table));
   for(starttime=lstime;starttime<lEtabe;starttime+=2419200){
   	sprintf(caTable,"ncshttplog_if_%s",utTimFormat("%Y%m",starttime));
   	if(strcmp(caPre_table,caTable)==0){
   		continue;
   	}
   	strcpy(caPre_table,caTable);
   	
    sprintf(caTemp,"select count(distinct host) from %s ",caTable);
    strcat(caTemp,caWhere);
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
 //  	printf("table=%s,iReturn=%d,lCount=%d\n",caTable,iReturn,lCount);
   	if(iReturn==1146||lCount==0){
 		   continue;
   	}
   	lSum=lSum+lCount;
   	iNum++;
   	if(iNum==1){
   			sprintf(sqlbuf,"(select host,count(*) aa from %s ",caTable);
        strcat(sqlbuf,caWhere);
        sprintf(sqlbuf+strlen(sqlbuf)," group by host ");
        strcat(sqlbuf," )");
    }
    else{
    	  sprintf(sqlbuf+strlen(sqlbuf)," union all (select host,count(*) aa from %s ",caTable);
    	   strcat(sqlbuf,caWhere);
    	    sprintf(sqlbuf+strlen(sqlbuf)," group by host "); 
        strcat(sqlbuf," )");
    }
  }
 
   
    lStartRec = (lCurPg - 1) *lRowNum;  
 
  if(iNum>0){
  	sprintf(caTemp,"select tb.host,sum(aa) as bb from (%s) as tb group by tb.host ",sqlbuf);
  	sprintf(caTemp+strlen(caTemp)," order by bb desc limit %d,%d ",lStartRec,lRowNum);  
   
  }    
  else{
  	 utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
     utPltPutVarF(psDbHead,"TotPg", "%lu",0);
     utPltPutVarF(psDbHead,"CurPg", "%lu", 1);

     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
     return 0;
  }	              

    
   	              
 
 //   strcpy(caDispname,ncUtlBufGetUsername(psShmHead,atol(caUserid)));  	              
                 	              

	
printf("caTemp=%s\n",caTemp);	 
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
    
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncshttp%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
         
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
        
        fprintf(fp,"访问网址排行\n");
      ncUtlPrintCsv(fp,2,UT_TYPE_STRING,"网址",
                       UT_TYPE_STRING,"连接次数");
                       
                                      
           memset(caHost,0,sizeof(caHost));
          lCount=0;
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,63,caHost,
			                                   UT_TYPE_LONG,4,&lCount);
			    															 
  	                              
        while((iReturn == 0)||(iReturn==1405)) { 
       

	      
        ncUtlPrintCsv(fp,2,UT_TYPE_STRING,caHost,
                          UT_TYPE_LONG,lCount);
                      
                       
	         memset(caHost,0,sizeof(caHost));
          lCount=0;
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,63,caHost,
			                                   UT_TYPE_LONG,4,&lCount);
        }
        pasDbCloseCursor(psCur);
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);

        return 0;
    }
    
    

  

     
    iReturn = 0;
    memset(caHost,0,sizeof(caHost));
    lCount=0;
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,63,caHost,
			                                   UT_TYPE_LONG,4,&lCount);
			                                
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
	 //       utPltSetCvtHtml(1);

	  //      utPltSetCvtHtml(0);
	      utPltPutLoopVar(psDbHead,"host",iNum,caHost);
	      utPltPutLoopVarF(psDbHead,"lcount",iNum,"%d",lCount);
         
 
 

     
       memset(caHost,0,sizeof(caHost));
       lCount=0;
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,63,caHost,
			                                   UT_TYPE_LONG,4,&lCount);
    }



    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
    return 0;
} 

//显示某个用户URl明细
int ncsUserHttpDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[20480];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;

  
   	float lRat;
   	long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;

	char sqlbuf[20480]="";
	unsigned long lStatrow,lSum;
   long lStime,lEtime;
 	pasDbCursor *psCur;
 
 	char caTable_hz[36];
 	char caSdate_d[16],caEdate_d[16];
 	char caTable[20],caUdisp[36];
 	long starttime;
  char caKeyword[64],caTime_flag[16];
  char caDid[16],caPage[12],caLimit[16],caSort[16],caDir[16];

  char caUid[16];
  char caExport[16];
  long mCount,lCmonth;
  char caUrlPath[256],caTitle[128],caUri[128],caPath[128];
  char caHost[64];
  char caTemp_d[68];
  lCurPg=1;
 	strcpy(caWhere,"\0");

 	

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,12,
			"did",UT_TYPE_STRING,12,caDid,
			"uid",UT_TYPE_STRING,10,caUid,
			"h1", UT_TYPE_STRING,63,caHost,
			"sdate",UT_TYPE_STRING,19,caSdate,
			"edate",UT_TYPE_STRING,19,caEdate,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "export1",     UT_TYPE_STRING,10,caExport,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag);
   	  
    
      if(strlen(caKeyword)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
      }
      
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 

    if(!utStrIsSpaces(caExport)){
    lRowNum = 99999;
    lStartRec=0;
    }
    else{
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
    }
    
   	
  
  
   	                
   
   lTime=time(0);
   
   if(strlen(caSdate)>0||strlen(caEdate)>0){
   	  if(strlen(caSdate)==0){
   	  	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
		     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
		     sprintf(caEtemptime,"%s 23:59",caEdate,caEtime);
		     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
   	  }
   	  else{
   	  	sprintf(caStemptime,"%s 00:00",caSdate);
   	  	lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  	letime=lTime;
   	  }
   	 }
   	  else{
   	  	if(atol(caTime_flag)==1){
   	  		sprintf(caStemptime,"%s 00:00",utTimFormat("%Y/%m/%d",lTime));
 //  	  		printf("caStemptime=%s,lTime=%d\n",caStemptime,lTime);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		letime=lTime;
   	  	}
        else if(atol(caTime_flag)==2){
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==3){
        	lstime=lTime-7*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==4){
        	lstime=lTime-30*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==5){
        	lstime=lTime-90*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==6){
        	lstime=lTime-180*3600*24;
        	letime=lTime;
        }
        else{
        	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
	   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
			     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
			     sprintf(caEtemptime,"%s 23:59",caEdate_d);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	  }
             
   utStrDelSpaces(caKeyword);      
	 memset(caWhere,0,sizeof(caWhere));
	 
	    lCmonth=0;   
nn:
	 
	 
	 sprintf(caWhere,"where stime>=%d and stime<=%d ",lstime,letime); 
   if(atol(caDid)>0){
	  	  utStrAddF(caWhere," and did=%lu ",atol(caDid));
	  	}
    if(atol(caUid)>0){
    	utStrAddF(caWhere," and uid=%lu ",atol(caUid));
    }
   
     if(strlen(caHost)>0){
        utStrAddF(caWhere," and host='%s' ",caHost);
     }
    	
    
    	
    psDbHead = utPltInitDb();
    
    
// printf("caWhere=%s\n",caWhere);  
    
    	
    	
   lSum=0;
   iNum=0;

   char caPre_table[64],caEtable[16];
   strcpy(caEtable,utTimFormat("%Y/%m",letime));
   long lEtabe=utTimStrToLong("%Y/%m",caEtable)+86400*30;
   memset(caPre_table,0,sizeof(caPre_table));
   for(starttime=lstime;starttime<lEtabe;starttime+=2419200){
   	sprintf(caTable,"ncshttplog_if_%s",utTimFormat("%Y%m",starttime));
   	if(strcmp(caPre_table,caTable)==0){
   		continue;
   	}
   	strcpy(caPre_table,caTable);
   	
    sprintf(caTemp,"select count(*) from %s ",caTable);
    strcat(caTemp,caWhere);
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
 //  	printf("table=%s,iReturn=%d,lCount=%d\n",caTable,iReturn,lCount);
   	if(iReturn==1146||lCount==0){
 		   continue;
   	}
   	lSum=lSum+lCount;
   	iNum++;
   	if(iNum==1){
   			sprintf(sqlbuf,"(select host,stime,title,uri from %s ",caTable);
        strcat(sqlbuf,caWhere);
       
        strcat(sqlbuf," )");
    }
    else{
    	  sprintf(sqlbuf+strlen(sqlbuf)," union all (select host,stime,title,uri from %s ",caTable);
    	   strcat(sqlbuf,caWhere);
    	   
        strcat(sqlbuf," )");
    }
  }
 
   
    lStartRec = (lCurPg - 1) *lRowNum;  
 
  if(iNum>0){
  	sprintf(caTemp,"select tb.host,tb.stime,tb.title,tb.uri from (%s) as tb ",sqlbuf);
  	sprintf(caTemp+strlen(caTemp)," order by tb.stime desc limit %d,%d ",lStartRec,lRowNum);  
   
  }    
  else{
  	 utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
     utPltPutVarF(psDbHead,"TotPg", "%lu",0);
     utPltPutVarF(psDbHead,"CurPg", "%lu", 1);

     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
     return 0;
  }	              

    
   	              
 
 //   strcpy(caDispname,ncUtlBufGetUsername(psShmHead,atol(caUserid)));  	              
                 	              

	
printf("caTemp=%s\n",caTemp);	 
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
    
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncshttp%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
         
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
        
        fprintf(fp,"用户访问网址明细\n");
      ncUtlPrintCsv(fp,3,UT_TYPE_STRING,"网址",
                         UT_TYPE_STRING,"访问时间",
                         UT_TYPE_STRING,"标题");
                       
                                      
    memset(caHost,0,sizeof(caHost));
    lStime=0;
    memset(caTitle,0,sizeof(caTitle));
    memset(caUri,0,sizeof(caUri));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,63,caHost,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_STRING,127,caTitle,
			                                   UT_TYPE_STRING,127,caUri);
			    															 
  	                              
        while((iReturn == 0)||(iReturn==1405)) { 
       
        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));
	      sprintf(caUrlPath,"%s%s",caHost,caPath);
        ncUtlPrintCsv(fp,3,UT_TYPE_STRING,caUrlPath,
                           UT_TYPE_STRING,caSdate,
                           UT_TYPE_STRING,caTitle);
                      
                       
			    memset(caHost,0,sizeof(caHost));
			    lStime=0;
			    memset(caTitle,0,sizeof(caTitle));
          memset(caUri,0,sizeof(caUri));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,63,caHost,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_STRING,127,caTitle,
			                                   UT_TYPE_STRING,127,caUri);
        }
        pasDbCloseCursor(psCur);
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);

        return 0;
    }
    
    

  

     
    iReturn = 0;
    memset(caHost,0,sizeof(caHost));
    lStime=0;
    memset(caTitle,0,sizeof(caTitle));
    memset(caUri,0,sizeof(caUri));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,63,caHost,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_STRING,127,caTitle,
			                                   UT_TYPE_STRING,127,caUri);
			                                  
			                                
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
	        utPltSetCvtHtml(1);

        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));
	      sprintf(caUrlPath,"%s%s",caHost,caUri);
        utPltPutLoopVar(psDbHead,"host",iNum,caUrlPath);
        utPltPutLoopVar(psDbHead,"title",iNum,caTitle);

	        utPltSetCvtHtml(0);
	      
	      utPltPutLoopVar(psDbHead,"stime",iNum,caSdate);
         
 
 

     
    memset(caHost,0,sizeof(caHost));
    lStime=0;
    memset(caTitle,0,sizeof(caTitle));
    memset(caUri,0,sizeof(caUri));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,63,caHost,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_STRING,127,caTitle,
			                                   UT_TYPE_STRING,127,caUri);
    }



    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
    return 0;
} 



//统计个人搜索引擎使用次数
int ncsUserSearchStat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[20480];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
    char caSvalue[128],caSname[128];
  
   	float lRat;
   	long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;

	char sqlbuf[20480]="";
	unsigned long lStatrow,lSum;
   long lStime,lEtime;
 	pasDbCursor *psCur;
 
 	char caTable_hz[36];
 	char caSdate_d[16],caEdate_d[16];
 	char caTable[20],caUdisp[36];
 	long starttime;
  char caKeyword[64],caTime_flag[16];
  char caDid[16],caPage[12],caLimit[16],caSort[16],caDir[16];

  char caUid[16];
  char caExport[16];
  long mCount,lCmonth;

  char caHost[64];
  char caTemp_d[68];
  lCurPg=1;
 	strcpy(caWhere,"\0");

 	

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,11,
			"did",UT_TYPE_STRING,12,caDid,
			"uid",UT_TYPE_STRING,10,caUid,
			"sdate",UT_TYPE_STRING,19,caSdate,
			"edate",UT_TYPE_STRING,19,caEdate,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "export1",     UT_TYPE_STRING,10,caExport,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag);
   	  
    
      if(strlen(caKeyword)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
      }
      
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 

    if(!utStrIsSpaces(caExport)){
    lRowNum = 99999;
    lStartRec=0;
    }
    else{
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
    }
    
   	
  
  
   	                
   
   lTime=time(0);
   
   if(strlen(caSdate)>0||strlen(caEdate)>0){
   	  if(strlen(caSdate)==0){
   	  	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
		     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
		     sprintf(caEtemptime,"%s 23:59",caEdate,caEtime);
		     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
   	  }
   	  else{
   	  	sprintf(caStemptime,"%s 00:00",caSdate);
   	  	lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  	letime=lTime;
   	  }
   	 }
   	  else{
   	  	if(atol(caTime_flag)==1){
   	  		sprintf(caStemptime,"%s 00:00",utTimFormat("%Y/%m/%d",lTime));
 //  	  		printf("caStemptime=%s,lTime=%d\n",caStemptime,lTime);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		letime=lTime;
   	  	}
        else if(atol(caTime_flag)==2){
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==3){
        	lstime=lTime-7*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==4){
        	lstime=lTime-30*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==5){
        	lstime=lTime-90*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==6){
        	lstime=lTime-180*3600*24;
        	letime=lTime;
        }
        else{
        	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
	   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
			     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
			     sprintf(caEtemptime,"%s 23:59",caEdate_d);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	  }
             
   utStrDelSpaces(caKeyword);      
	 memset(caWhere,0,sizeof(caWhere));
	 
	    lCmonth=0;   
nn:
	 
	 
	 sprintf(caWhere,"where stime>=%d and stime<=%d ",lstime,letime); 
   if(atol(caDid)>0){
	  	  utStrAddF(caWhere," and did=%lu ",atol(caDid));
	  	}
    if(atol(caUid)>0){
    	utStrAddF(caWhere," and uid=%lu ",atol(caUid));
    }
   
     if(strlen(caKeyword)>0){
        utStrAddF(caWhere," and svalue like '%c%s%c' ", '%',caKeyword,'%');
     }
    	
    
    	
    psDbHead = utPltInitDb();
    
    
// printf("caWhere=%s\n",caWhere);  
    
    	
    	
   lSum=0;
   iNum=0;

   char caPre_table[64],caEtable[16];
   strcpy(caEtable,utTimFormat("%Y/%m",letime));
   long lEtabe=utTimStrToLong("%Y/%m",caEtable)+86400*30;
   memset(caPre_table,0,sizeof(caPre_table));
   for(starttime=lstime;starttime<lEtabe;starttime+=2419200){
   	sprintf(caTable,"ncssearchlog_if_%s",utTimFormat("%Y%m",starttime));
   	if(strcmp(caPre_table,caTable)==0){
   		continue;
   	}
   	strcpy(caPre_table,caTable);
   	
    sprintf(caTemp,"select count(*) from (select svalue,sname from %s  ",caTable);
    strcat(caTemp,caWhere);
    strcat(caTemp," group by svalue,sname) as tb ");
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
 //  	printf("table=%s,iReturn=%d,lCount=%d\n",caTable,iReturn,lCount);
   	if(iReturn==1146||lCount==0){
 		   continue;
   	}
   	lSum=lSum+lCount;
   	iNum++;
   	if(iNum==1){
   			sprintf(sqlbuf,"(select svalue,sname,count(*) aa from %s ",caTable);
        strcat(sqlbuf,caWhere);
        sprintf(sqlbuf+strlen(sqlbuf)," group by svalue,sname ");
        strcat(sqlbuf," )");
    }
    else{
    	  sprintf(sqlbuf+strlen(sqlbuf)," union all (select svalue,sname,count(*) aa from %s ",caTable);
    	   strcat(sqlbuf,caWhere);
    	    sprintf(sqlbuf+strlen(sqlbuf)," group by svalue,sname "); 
        strcat(sqlbuf," )");
    }
  }
 
   
    lStartRec = (lCurPg - 1) *lRowNum;  
 
  if(iNum>0){
  	sprintf(caTemp,"select tb.svalue,tb.sname,sum(aa) as bb from (%s) as tb group by tb.svalue,tb.sname ",sqlbuf);
  	sprintf(caTemp+strlen(caTemp)," order by bb desc limit %d,%d ",lStartRec,lRowNum);  
   
  }    
  else{
  	 utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
     utPltPutVarF(psDbHead,"TotPg", "%lu",0);
     utPltPutVarF(psDbHead,"CurPg", "%lu", 1);

     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
     return 0;
  }
  
  	
	
printf("caTemp=%s\n",caTemp);	 
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
    
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncssearch%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
         
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
        
        fprintf(fp,"用户搜索排行\n");
      ncUtlPrintCsv(fp,3,
                         UT_TYPE_STRING,"搜索工具",
                         UT_TYPE_STRING,"搜索关键字",
                         UT_TYPE_STRING,"次数");
                       
                                      
    memset(caSname,0,sizeof(caSname));
    memset(caSvalue,0,sizeof(caSvalue));
    lCount=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,127,caSvalue,
		                               UT_TYPE_STRING,15,caSname,
		                               UT_TYPE_LONG,4,&lCount);
			    															 
  	                              
        while((iReturn == 0)||(iReturn==1405)) { 
       
        ncUtlPrintCsv(fp,3,UT_TYPE_STRING,caSvalue,
                           UT_TYPE_STRING,caSname,
                           UT_TYPE_LONG,lCount);
                      
                       
    memset(caSname,0,sizeof(caSname));
    memset(caSvalue,0,sizeof(caSvalue));
    lCount=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,127,caSvalue,
		                               UT_TYPE_STRING,15,caSname,
		                               UT_TYPE_LONG,4,&lCount);
        }
        pasDbCloseCursor(psCur);
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);

        return 0;
    }
    
    

  

     
    iReturn = 0;
    memset(caSname,0,sizeof(caSname));
    memset(caSvalue,0,sizeof(caSvalue));
    lCount=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,127,caSvalue,
		                               UT_TYPE_STRING,15,caSname,
		                               UT_TYPE_LONG,4,&lCount);
			                                  
			                                
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
	        utPltSetCvtHtml(1);

        
        utPltPutLoopVar(psDbHead,"sname",iNum,caSname);
        utPltPutLoopVar(psDbHead,"svalue",iNum,caSvalue);
	        utPltSetCvtHtml(0);
	     
	      utPltPutLoopVarF(psDbHead,"lcount",iNum,"%d",lCount);
         
 
 

     
    memset(caSname,0,sizeof(caSname));
    memset(caSvalue,0,sizeof(caSvalue));
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,127,caSvalue,
		                               UT_TYPE_STRING,15,caSname,
		                               UT_TYPE_LONG,4,&lCount);
			                                  
    }



    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
  //          utPltShowDb(psDbHead);
        
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
    return 0;
}  



     //显示某个用户搜索引擎明细
int ncsUserSearchDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[20480];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
    char caSvalue[128],caSip[16];
  
   	float lRat;
   	long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;

	char sqlbuf[20480]="";
	unsigned long lStatrow,lSum;
   long lStime,lEtime;
 	pasDbCursor *psCur;
 
 	char caTable_hz[36];
 	char caSdate_d[16],caEdate_d[16];
 	char caTable[20],caUdisp[36];
 	long starttime;
  char caKeyword[64],caTime_flag[16];
  char caDid[16],caPage[12],caLimit[16],caSort[16],caDir[16];

  char caUid[16];
  char caExport[16];
  long mCount,lCmonth;
  char caUrlPath[256],caTitle[128],caUri[128],caPath[128];
  char caHost[64],caSname[20];
  char caTemp_d[68];
  lCurPg=1;
 	strcpy(caWhere,"\0");

 	

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,12,
			"did",UT_TYPE_STRING,12,caDid,
			"uid",UT_TYPE_STRING,10,caUid,
			"h1", UT_TYPE_STRING,63,caHost,
			"sdate",UT_TYPE_STRING,19,caSdate,
			"edate",UT_TYPE_STRING,19,caEdate,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "export1",     UT_TYPE_STRING,10,caExport,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag);
   	  memset(caTemp,0,sizeof(caTemp));
   	  utStrReplaceWith(caHost,"^^"," ");
    
      if(strlen(caKeyword)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
      }
      
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 

    if(!utStrIsSpaces(caExport)){
    lRowNum = 99999;
    lStartRec=0;
    }
    else{
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
    }
    
   	
  
  
   	                
   
   lTime=time(0);
   
   if(strlen(caSdate)>0||strlen(caEdate)>0){
   	  if(strlen(caSdate)==0){
   	  	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
		     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
		     sprintf(caEtemptime,"%s 23:59",caEdate,caEtime);
		     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
   	  }
   	  else{
   	  	sprintf(caStemptime,"%s 00:00",caSdate);
   	  	lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  	letime=lTime;
   	  }
   	 }
   	  else{
   	  	if(atol(caTime_flag)==1){
   	  		sprintf(caStemptime,"%s 00:00",utTimFormat("%Y/%m/%d",lTime));
 //  	  		printf("caStemptime=%s,lTime=%d\n",caStemptime,lTime);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		letime=lTime;
   	  	}
        else if(atol(caTime_flag)==2){
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==3){
        	lstime=lTime-7*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==4){
        	lstime=lTime-30*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==5){
        	lstime=lTime-90*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==6){
        	lstime=lTime-180*3600*24;
        	letime=lTime;
        }
        else{
        	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
	   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
			     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
			     sprintf(caEtemptime,"%s 23:59",caEdate_d);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	  }
             
   utStrDelSpaces(caKeyword);      
	 memset(caWhere,0,sizeof(caWhere));
	 
	    lCmonth=0;   
nn:
	 
	 
	 sprintf(caWhere,"where stime>=%d and stime<=%d ",lstime,letime); 
   if(atol(caDid)>0){
	  	  utStrAddF(caWhere," and did=%lu ",atol(caDid));
	  	}
    if(atol(caUid)>0){
    	utStrAddF(caWhere," and uid=%lu ",atol(caUid));
    }
   
     if(strlen(caHost)>0){
        utStrAddF(caWhere," and svalue='%s' ",caHost);
     }
    	
    
    	
    psDbHead = utPltInitDb();
    
    
// printf("caWhere=%s\n",caWhere);  
    
    	
    	
   lSum=0;
   iNum=0;

   char caPre_table[64],caEtable[16];
   strcpy(caEtable,utTimFormat("%Y/%m",letime));
   long lEtabe=utTimStrToLong("%Y/%m",caEtable)+86400*30;
   memset(caPre_table,0,sizeof(caPre_table));
   for(starttime=lstime;starttime<lEtabe;starttime+=2419200){
   	sprintf(caTable,"ncssearchlog_if_%s",utTimFormat("%Y%m",starttime));
   	if(strcmp(caPre_table,caTable)==0){
   		continue;
   	}
   	strcpy(caPre_table,caTable);
   	
    sprintf(caTemp,"select count(*) from %s ",caTable);
    strcat(caTemp,caWhere);
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
 //  	printf("table=%s,iReturn=%d,lCount=%d\n",caTable,iReturn,lCount);
   	if(iReturn==1146||lCount==0){
 		   continue;
   	}
   	lSum=lSum+lCount;
   	iNum++;
   	if(iNum==1){
   			sprintf(sqlbuf,"(select svalue,stime,sname,sip from %s ",caTable);
        strcat(sqlbuf,caWhere);
       
        strcat(sqlbuf," )");
    }
    else{
    	  sprintf(sqlbuf+strlen(sqlbuf)," union all (select svalue,stime,sname,sip from %s ",caTable);
    	   strcat(sqlbuf,caWhere);
    	   
        strcat(sqlbuf," )");
    }
  }
 
   
    lStartRec = (lCurPg - 1) *lRowNum;  
 
  if(iNum>0){
  	sprintf(caTemp,"select tb.svalue,tb.stime,tb.sname,tb.sip from (%s) as tb ",sqlbuf);
  	sprintf(caTemp+strlen(caTemp)," order by tb.stime desc limit %d,%d ",lStartRec,lRowNum);  
   
  }    
  else{
  	 utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
     utPltPutVarF(psDbHead,"TotPg", "%lu",0);
     utPltPutVarF(psDbHead,"CurPg", "%lu", 1);

     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
     return 0;
  }	              

    
   	              
 
 //   strcpy(caDispname,ncUtlBufGetUsername(psShmHead,atol(caUserid)));  	              
                 	              

	
printf("caTemp=%s\n",caTemp);	 
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
    
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncssearch%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
         
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
        
        fprintf(fp,"用户使用搜索明细\n");
      ncUtlPrintCsv(fp,4,UT_TYPE_STRING,"搜索关键字",
                         UT_TYPE_STRING,"搜索工具",
                         UT_TYPE_STRING,"使用时间",
                         UT_TYPE_STRING,"源IP");
                       
                                      
    memset(caSvalue,0,sizeof(caSvalue));
    lStime=0;
    memset(caSname,0,sizeof(caSname));
    memset(caSip,0,sizeof(caSip));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caSvalue,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_STRING,15,caSname,
			                                   UT_TYPE_STRING,15,caSip);
			    															 
  	                              
        while((iReturn == 0)||(iReturn==1405)) { 
       
        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));	     
        ncUtlPrintCsv(fp,4,UT_TYPE_STRING,caSvalue,
                           UT_TYPE_STRING,caSname,
                           UT_TYPE_STRING,caSdate,
                           UT_TYPE_STRING,caSip);
                      
                       
    memset(caSvalue,0,sizeof(caSvalue));
    lStime=0;
    memset(caSname,0,sizeof(caSname));
    memset(caSip,0,sizeof(caSip));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caSvalue,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_STRING,15,caSname,
			                                   UT_TYPE_STRING,15,caSip);
        }
        pasDbCloseCursor(psCur);
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);

        return 0;
    }
    
    

  

     
    iReturn = 0;
    memset(caSvalue,0,sizeof(caSvalue));
    lStime=0;
    memset(caSname,0,sizeof(caSname));
    memset(caSip,0,sizeof(caSip));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caSvalue,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_STRING,15,caSname,
			                                   UT_TYPE_STRING,15,caSip);
			                                  
			                                
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
	        utPltSetCvtHtml(1);

        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));
        utPltPutLoopVar(psDbHead,"sip",iNum,caSip);
        utPltPutLoopVar(psDbHead,"svalue",iNum,caSvalue);
        utPltPutLoopVar(psDbHead,"sname",iNum,caSname);

	        utPltSetCvtHtml(0);
	      
	      utPltPutLoopVar(psDbHead,"stime",iNum,caSdate);
         
 
 
    memset(caSvalue,0,sizeof(caSvalue));
    lStime=0;
    memset(caSname,0,sizeof(caSname));
    memset(caSip,0,sizeof(caSip));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caSvalue,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_STRING,15,caSname,
			                                   UT_TYPE_STRING,15,caSip);
    }



    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
    return 0;
} 
  
  
//统计个人BBS发帖次数
int ncsUserBbsStat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[20480];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
    char caSvalue[128],caSname[128];
  
   	float lRat;
   	long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;

	char sqlbuf[20480]="";
	unsigned long lStatrow,lSum;
   long lStime,lEtime;
 	pasDbCursor *psCur;
 
 	char caTable_hz[36];
 	char caSdate_d[16],caEdate_d[16];
 	char caTable[20],caUdisp[36];
 	long starttime;
  char caKeyword[64],caTime_flag[16];
  char caDid[16],caPage[12],caLimit[16],caSort[16],caDir[16];
  char caBbsname[48],caUrl[128];
  char caUid[16];
  char caExport[16];
  long mCount,lCmonth;

  char caHost[64];
  char caTemp_d[68];
  lCurPg=1;
 	strcpy(caWhere,"\0");

 	

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,11,
			"did",UT_TYPE_STRING,12,caDid,
			"uid",UT_TYPE_STRING,10,caUid,
			"sdate",UT_TYPE_STRING,19,caSdate,
			"edate",UT_TYPE_STRING,19,caEdate,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "export1",     UT_TYPE_STRING,10,caExport,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag);
   	  
    
  //    if(strlen(caKeyword)>0){
  //    	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
  //    	strcpy(caKeyword,caTemp_d);
 //     }
      
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 

    if(!utStrIsSpaces(caExport)){
    lRowNum = 99999;
    lStartRec=0;
    }
    else{
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
    }
    
   	
  
  
   	                
   
   lTime=time(0);
   
   if(strlen(caSdate)>0||strlen(caEdate)>0){
   	  if(strlen(caSdate)==0){
   	  	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
		     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
		     sprintf(caEtemptime,"%s 23:59",caEdate,caEtime);
		     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
   	  }
   	  else{
   	  	sprintf(caStemptime,"%s 00:00",caSdate);
   	  	lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  	letime=lTime;
   	  }
   	 }
   	  else{
   	  	if(atol(caTime_flag)==1){
   	  		sprintf(caStemptime,"%s 00:00",utTimFormat("%Y/%m/%d",lTime));
 //  	  		printf("caStemptime=%s,lTime=%d\n",caStemptime,lTime);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		letime=lTime;
   	  	}
        else if(atol(caTime_flag)==2){
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==3){
        	lstime=lTime-7*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==4){
        	lstime=lTime-30*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==5){
        	lstime=lTime-90*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==6){
        	lstime=lTime-180*3600*24;
        	letime=lTime;
        }
        else{
        	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
	   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
			     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
			     sprintf(caEtemptime,"%s 23:59",caEdate_d);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	  }
             
   utStrDelSpaces(caKeyword);      
	 memset(caWhere,0,sizeof(caWhere));
	 
	    lCmonth=0;   
nn:
	 
	 
	 sprintf(caWhere,"where stime>=%d and stime<=%d ",lstime,letime); 
   if(atol(caDid)>0){
	  	  utStrAddF(caWhere," and did=%lu ",atol(caDid));
	  	}
    if(atol(caUid)>0){
    	utStrAddF(caWhere," and uid=%lu ",atol(caUid));
    }
   
     if(strlen(caKeyword)>0){
        utStrAddF(caWhere," and (bbsname like '%c%s%c' or url like '%c%s%c' ) ", '%',caKeyword,'%', '%',caKeyword,'%');
     }
    	
    
    	
    psDbHead = utPltInitDb();
    
    
// printf("caWhere=%s\n",caWhere);  
    
    	
    	
   lSum=0;
   iNum=0;

   char caPre_table[64],caEtable[16];
   strcpy(caEtable,utTimFormat("%Y/%m",letime));
   long lEtabe=utTimStrToLong("%Y/%m",caEtable)+86400*30;
   memset(caPre_table,0,sizeof(caPre_table));
   for(starttime=lstime;starttime<lEtabe;starttime+=2419200){
   	sprintf(caTable,"ncsbbsmon_if_%s",utTimFormat("%Y%m",starttime));
   	if(strcmp(caPre_table,caTable)==0){
   		continue;
   	}
   	strcpy(caPre_table,caTable);
   	
    sprintf(caTemp,"select count(*) from (select  bbsname,url from %s  ",caTable);
    strcat(caTemp,caWhere);
    strcat(caTemp," group by bbsname,url) as tb ");
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
 //  	printf("table=%s,iReturn=%d,lCount=%d\n",caTable,iReturn,lCount);
   	if(iReturn==1146||lCount==0){
 		   continue;
   	}
   	lSum=lSum+lCount;
   	iNum++;
   	if(iNum==1){
   			sprintf(sqlbuf,"(select bbsname,url,count(*) aa from %s ",caTable);
        strcat(sqlbuf,caWhere);
        sprintf(sqlbuf+strlen(sqlbuf)," group by bbsname,url ");
        strcat(sqlbuf," )");
    }
    else{
    	  sprintf(sqlbuf+strlen(sqlbuf)," union all (select bbsname,url,count(*) aa from %s ",caTable);
    	   strcat(sqlbuf,caWhere);
    	    sprintf(sqlbuf+strlen(sqlbuf)," group by bbsname,url "); 
        strcat(sqlbuf," )");
    }
  }
 
   
    lStartRec = (lCurPg - 1) *lRowNum;  
 
  if(iNum>0){
  	sprintf(caTemp,"select tb.bbsname,tb.url,sum(aa) as bb from (%s) as tb group by tb.bbsname,tb.url ",sqlbuf);
  	sprintf(caTemp+strlen(caTemp)," order by bb desc limit %d,%d ",lStartRec,lRowNum);  
   
  }    
  else{
  	 utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
     utPltPutVarF(psDbHead,"TotPg", "%lu",0);
     utPltPutVarF(psDbHead,"CurPg", "%lu", 1);

     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
     return 0;
  }
  
  	
	
printf("caTemp=%s\n",caTemp);	 
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
    
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncsbbs%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
         
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
        
        fprintf(fp,"用户BBS发帖统计\n");
      ncUtlPrintCsv(fp,3,
                         UT_TYPE_STRING,"论坛名称",
                         UT_TYPE_STRING,"论坛URL",
                         UT_TYPE_STRING,"记录数");
                       
                                      
    memset(caBbsname,0,sizeof(caBbsname));
    memset(caUrl,0,sizeof(caUrl));
    lCount=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,47,caBbsname,
		                               UT_TYPE_STRING,47,caUrl,
		                               UT_TYPE_LONG,4,&lCount);
			    															 
  	                              
        while((iReturn == 0)||(iReturn==1405)) { 
       
       if(strlen(caBbsname)>0){
       	  strcpy(caTemp_d,pasCvtGBK(2,caBbsname,caTemp,47));
        	strcpy(caBbsname,caTemp_d);
         }
       
       
        ncUtlPrintCsv(fp,3,UT_TYPE_STRING,caBbsname,
                           UT_TYPE_STRING,caUrl,
                           UT_TYPE_LONG,lCount);
                      
                       
    memset(caBbsname,0,sizeof(caBbsname));
    memset(caUrl,0,sizeof(caUrl));
    lCount=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,47,caBbsname,
		                               UT_TYPE_STRING,47,caUrl,
		                               UT_TYPE_LONG,4,&lCount);
        }
        pasDbCloseCursor(psCur);
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);

        return 0;
    }
    
    

  

     
    iReturn = 0;
    memset(caBbsname,0,sizeof(caBbsname));
    memset(caUrl,0,sizeof(caUrl));
    lCount=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,47,caBbsname,
		                               UT_TYPE_STRING,47,caUrl,
		                               UT_TYPE_LONG,4,&lCount);
			                                  
			                                
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
	        if(strlen(caBbsname)>0){
       	  strcpy(caTemp_d,pasCvtGBK(2,caBbsname,caTemp,47));
        	strcpy(caBbsname,caTemp_d);
         }
	        
	        utPltSetCvtHtml(1);

        utPltPutLoopVar(psDbHead,"svalue",iNum,caBbsname);
        utPltPutLoopVar(psDbHead,"sname",iNum,caUrl);

	        utPltSetCvtHtml(0);
	      
	      utPltPutLoopVarF(psDbHead,"lcount",iNum,"%d",lCount);
         
 
 

    memset(caBbsname,0,sizeof(caBbsname));
    memset(caUrl,0,sizeof(caUrl));
    lCount=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,47,caBbsname,
		                               UT_TYPE_STRING,47,caUrl,
		                               UT_TYPE_LONG,4,&lCount);
			                                  
    }



    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
    return 0;
}    



//显示某个用户BBS发帖明细
int ncsUserBbsDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[20480];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
    char caSvalue[128],caSip[16];
  
   	float lRat;
   	long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;

	char sqlbuf[20480]="";
	unsigned long lStatrow,lSum;
   long lStime,lEtime;
 	pasDbCursor *psCur;
 
 	char caTable_hz[36];
 	char caSdate_d[16],caEdate_d[16];
 	char caTable[20],caUdisp[36];
 	long starttime;
  char caKeyword[64],caTime_flag[16];
  char caDid[16],caPage[12],caLimit[16],caSort[16],caDir[16];

  char caUid[16];
  char caExport[16];
  long mCount,lCmonth;
  char caUrlPath[256],caTitle[128],caUri[128],caPath[128];
  char caHost[64],caSname[20];
  char caTemp_d[1024];
  char caSubject[256],caContent[1024],caAuthor[48],caUpfile[48];
  lCurPg=1;
 	strcpy(caWhere,"\0");

 	

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,12,
			"did",UT_TYPE_STRING,12,caDid,
			"uid",UT_TYPE_STRING,10,caUid,
			"h1", UT_TYPE_STRING,63,caHost,
			"sdate",UT_TYPE_STRING,19,caSdate,
			"edate",UT_TYPE_STRING,19,caEdate,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "export1",     UT_TYPE_STRING,10,caExport,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag);
   	  
   	  
    
      if(strlen(caKeyword)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
      }
      
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 

    if(!utStrIsSpaces(caExport)){
    lRowNum = 99999;
    lStartRec=0;
    }
    else{
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
    }
    
   	
  
  
   	                
   
   lTime=time(0);
   
   if(strlen(caSdate)>0||strlen(caEdate)>0){
   	  if(strlen(caSdate)==0){
   	  	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
		     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
		     sprintf(caEtemptime,"%s 23:59",caEdate,caEtime);
		     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
   	  }
   	  else{
   	  	sprintf(caStemptime,"%s 00:00",caSdate);
   	  	lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  	letime=lTime;
   	  }
   	 }
   	  else{
   	  	if(atol(caTime_flag)==1){
   	  		sprintf(caStemptime,"%s 00:00",utTimFormat("%Y/%m/%d",lTime));
 //  	  		printf("caStemptime=%s,lTime=%d\n",caStemptime,lTime);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		letime=lTime;
   	  	}
        else if(atol(caTime_flag)==2){
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==3){
        	lstime=lTime-7*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==4){
        	lstime=lTime-30*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==5){
        	lstime=lTime-90*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==6){
        	lstime=lTime-180*3600*24;
        	letime=lTime;
        }
        else{
        	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
	   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
			     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
			     sprintf(caEtemptime,"%s 23:59",caEdate_d);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	  }
             
   utStrDelSpaces(caKeyword);      
	 memset(caWhere,0,sizeof(caWhere));
	 
	    lCmonth=0;   
nn:
	 
	 
	 sprintf(caWhere,"where stime>=%d and stime<=%d ",lstime,letime); 
   if(atol(caDid)>0){
	  	  utStrAddF(caWhere," and did=%lu ",atol(caDid));
	  	}
    if(atol(caUid)>0){
    	utStrAddF(caWhere," and uid=%lu ",atol(caUid));
    }
   
     if(strlen(caHost)>0){
    //    utStrAddF(caWhere," and bbsname='%s' ",caHost);
    utStrAddF(caWhere," and bbsname='%s' ",pasCharsetCvtP(PAS_CCODE_GB,PAS_CCODE_UTF8,caHost));
     }
    	
    
    	
    psDbHead = utPltInitDb();
    
    
// printf("caWhere=%s\n",caWhere);  
    
    	
    	
   lSum=0;
   iNum=0;

   char caPre_table[64],caEtable[16];
   strcpy(caEtable,utTimFormat("%Y/%m",letime));
   long lEtabe=utTimStrToLong("%Y/%m",caEtable)+86400*30;
   memset(caPre_table,0,sizeof(caPre_table));
   for(starttime=lstime;starttime<lEtabe;starttime+=2419200){
   	sprintf(caTable,"ncsbbsmon_if_%s",utTimFormat("%Y%m",starttime));
   	if(strcmp(caPre_table,caTable)==0){
   		continue;
   	}
   	strcpy(caPre_table,caTable);
   	
    sprintf(caTemp,"select count(*) from %s ",caTable);
    strcat(caTemp,caWhere);
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
 //  	printf("table=%s,iReturn=%d,lCount=%d\n",caTable,iReturn,lCount);
   	if(iReturn==1146||lCount==0){
 		   continue;
   	}
   	lSum=lSum+lCount;
   	iNum++;
   	if(iNum==1){
   			sprintf(sqlbuf,"(select bbsname,stime,subject,content,author,upfile from %s ",caTable);
        strcat(sqlbuf,caWhere);
       
        strcat(sqlbuf," )");
    }
    else{
    	  sprintf(sqlbuf+strlen(sqlbuf)," union all (select bbsname,stime,subject,content,author,upfile from %s ",caTable);
    	   strcat(sqlbuf,caWhere);
    	   
        strcat(sqlbuf," )");
    }
  }
 
   
    lStartRec = (lCurPg - 1) *lRowNum;  
 
  if(iNum>0){
  	sprintf(caTemp,"select tb.bbsname,tb.stime,tb.subject,tb.content,tb.author,tb.upfile from (%s) as tb ",sqlbuf);
  	sprintf(caTemp+strlen(caTemp)," order by tb.stime desc limit %d,%d ",lStartRec,lRowNum);  
   
  }    
  else{
  	 utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
     utPltPutVarF(psDbHead,"TotPg", "%lu",0);
     utPltPutVarF(psDbHead,"CurPg", "%lu", 1);

     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
     return 0;
  }	              

    
   	              
 
 //   strcpy(caDispname,ncUtlBufGetUsername(psShmHead,atol(caUserid)));  	              
                 	              

	
printf("caTemp=%s\n",caTemp);	 
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
    
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncsbbs%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
         
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
        
        fprintf(fp,"用户BBS发帖明细\n");
      ncUtlPrintCsv(fp,5,UT_TYPE_STRING,"论坛名称",
                         UT_TYPE_STRING,"帐号",
                         UT_TYPE_STRING,"使用时间",
                         UT_TYPE_STRING,"主题",
                         UT_TYPE_STRING,"内容");
                       
                                      
    memset(caSvalue,0,sizeof(caSvalue));
    lStime=0;
    memset(caSubject,0,sizeof(caSubject));
    memset(caContent,0,sizeof(caContent));
    memset(caAuthor,0,sizeof(caAuthor));
    memset(caUpfile,0,sizeof(caUpfile));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caSvalue,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_STRING,254,caSubject,
			                                   UT_TYPE_STRING,1000,caContent,
			                                   UT_TYPE_STRING,47,caAuthor,
			                                   UT_TYPE_STRING,47,caUpfile);
			    															 
  	                              
        while((iReturn == 0)||(iReturn==1405)) { 
       if(strlen(caSubject)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caSubject,caTemp,254));
      	strcpy(caSubject,caTemp_d);
       }
       if(strlen(caContent)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caContent,caTemp,1000));
      	strcpy(caContent,caTemp_d);
       }
       if(strlen(caSvalue)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caSvalue,caTemp,47));
      	strcpy(caSvalue,caTemp_d);
       }
       if(strlen(caAuthor)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caAuthor,caTemp,47));
      	strcpy(caAuthor,caTemp_d);
       }
        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));	     
        ncUtlPrintCsv(fp,5,UT_TYPE_STRING,caSvalue,
                           UT_TYPE_STRING,caAuthor,
                           UT_TYPE_STRING,caSdate,
                           UT_TYPE_STRING,caSubject,
                           UT_TYPE_STRING,caContent);
                      
                       
    memset(caSvalue,0,sizeof(caSvalue));
    lStime=0;
    memset(caSubject,0,sizeof(caSubject));
    memset(caContent,0,sizeof(caContent));
    memset(caAuthor,0,sizeof(caAuthor));
    memset(caUpfile,0,sizeof(caUpfile));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caSvalue,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_STRING,254,caSubject,
			                                   UT_TYPE_STRING,1000,caContent,
			                                   UT_TYPE_STRING,47,caAuthor,
			                                   UT_TYPE_STRING,47,caUpfile);
        }
        pasDbCloseCursor(psCur);
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);

        return 0;
    }
    
    

  

     
    iReturn = 0;
    memset(caSvalue,0,sizeof(caSvalue));
    lStime=0;
    memset(caSubject,0,sizeof(caSubject));
    memset(caContent,0,sizeof(caContent));
    memset(caAuthor,0,sizeof(caAuthor));
    memset(caUpfile,0,sizeof(caUpfile));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caSvalue,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_STRING,254,caSubject,
			                                   UT_TYPE_STRING,1000,caContent,
			                                   UT_TYPE_STRING,47,caAuthor,
			                                   UT_TYPE_STRING,47,caUpfile);
			                                  
			                                
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
	        utPltSetCvtHtml(1);
       if(strlen(caSubject)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caSubject,caTemp,254));
      	strcpy(caSubject,caTemp_d);
       }
       if(strlen(caContent)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caContent,caTemp,1000));
      	strcpy(caContent,caTemp_d);
       }
       if(strlen(caSvalue)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caSvalue,caTemp,47));
      	strcpy(caSvalue,caTemp_d);
       }
       if(strlen(caAuthor)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caAuthor,caTemp,47));
      	strcpy(caAuthor,caTemp_d);
       }
        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));
        utPltPutLoopVar(psDbHead,"subject",iNum,caSubject);
        utPltPutLoopVar(psDbHead,"svalue",iNum,caSvalue);
        utPltPutLoopVar(psDbHead,"content",iNum,caContent);
        utPltPutLoopVar(psDbHead,"author",iNum,caAuthor);

	        utPltSetCvtHtml(0);
	      
	      utPltPutLoopVar(psDbHead,"stime",iNum,caSdate);
         
 
 
    memset(caSvalue,0,sizeof(caSvalue));
    lStime=0;
    memset(caSubject,0,sizeof(caSubject));
    memset(caContent,0,sizeof(caContent));
    memset(caAuthor,0,sizeof(caAuthor));
    memset(caUpfile,0,sizeof(caUpfile));
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caSvalue,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_STRING,254,caSubject,
			                                   UT_TYPE_STRING,1000,caContent,
			                                   UT_TYPE_STRING,47,caAuthor,
			                                   UT_TYPE_STRING,47,caUpfile);
    }



    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
    return 0;
} 




   //统计个人聊天次数
int ncsUserImStat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[20480];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
    char caSvalue[128],caSname[128];
    ncsNettype *psData;
   	float lRat;
   	long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;

	char sqlbuf[20480]="";
	unsigned long lStatrow,lSum;
   long lStime,lEtime;
 	pasDbCursor *psCur;
 
 	char caTable_hz[36];
 	char caSdate_d[16],caEdate_d[16];
 	char caTable[20],caUdisp[36];
 	long starttime;
  char caKeyword[64],caTime_flag[16];
  char caDid[16],caPage[12],caLimit[16],caSort[16],caDir[16];
  char caBbsname[48],caUrl[128];
  char caUid[16];
  char caExport[16];
  long mCount,lCmonth;
  char caMfrom[48],caMto[48],caType[48];
  long lService;
  char caHost[64];
  char caTemp_d[68];
  lCurPg=1;
 	strcpy(caWhere,"\0");

 	

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,11,
			"did",UT_TYPE_STRING,12,caDid,
			"uid",UT_TYPE_STRING,10,caUid,
			"sdate",UT_TYPE_STRING,19,caSdate,
			"edate",UT_TYPE_STRING,19,caEdate,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "export1",     UT_TYPE_STRING,10,caExport,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag);
   	  
    
  //    if(strlen(caKeyword)>0){
  //    	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
  //    	strcpy(caKeyword,caTemp_d);
 //     }
      
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 

    if(!utStrIsSpaces(caExport)){
    lRowNum = 99999;
    lStartRec=0;
    }
    else{
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
    }
    
   	
  
  
   	                
   
   lTime=time(0);
   
   if(strlen(caSdate)>0||strlen(caEdate)>0){
   	  if(strlen(caSdate)==0){
   	  	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
		     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
		     sprintf(caEtemptime,"%s 23:59",caEdate,caEtime);
		     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
   	  }
   	  else{
   	  	sprintf(caStemptime,"%s 00:00",caSdate);
   	  	lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  	letime=lTime;
   	  }
   	 }
   	  else{
   	  	if(atol(caTime_flag)==1){
   	  		sprintf(caStemptime,"%s 00:00",utTimFormat("%Y/%m/%d",lTime));
 //  	  		printf("caStemptime=%s,lTime=%d\n",caStemptime,lTime);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		letime=lTime;
   	  	}
        else if(atol(caTime_flag)==2){
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==3){
        	lstime=lTime-7*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==4){
        	lstime=lTime-30*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==5){
        	lstime=lTime-90*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==6){
        	lstime=lTime-180*3600*24;
        	letime=lTime;
        }
        else{
        	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
	   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
			     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
			     sprintf(caEtemptime,"%s 23:59",caEdate_d);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	  }
             
   utStrDelSpaces(caKeyword);      
	 memset(caWhere,0,sizeof(caWhere));
	 
	    lCmonth=0;   
nn:
	 
	 
	 sprintf(caWhere,"where stime>=%d and stime<=%d ",lstime,letime); 
   if(atol(caDid)>0){
	  	  utStrAddF(caWhere," and did=%lu ",atol(caDid));
	  	}
    if(atol(caUid)>0){
    	utStrAddF(caWhere," and uid=%lu ",atol(caUid));
    }
   
     if(strlen(caKeyword)>0){
        utStrAddF(caWhere," and (mfrom like '%c%s%c' or mto like '%c%s%c' ) ", '%',caKeyword,'%', '%',caKeyword,'%');
     }
    	
    
    	
    psDbHead = utPltInitDb();
    
    
// printf("caWhere=%s\n",caWhere);  
    
    	
    	
   lSum=0;
   iNum=0;

   char caPre_table[64],caEtable[16];
   strcpy(caEtable,utTimFormat("%Y/%m",letime));
   long lEtabe=utTimStrToLong("%Y/%m",caEtable)+86400*30;
   memset(caPre_table,0,sizeof(caPre_table));
   for(starttime=lstime;starttime<lEtabe;starttime+=2419200){
   	sprintf(caTable,"ncsimlog_if_%s",utTimFormat("%Y%m",starttime));
   	if(strcmp(caPre_table,caTable)==0){
   		continue;
   	}
   	strcpy(caPre_table,caTable);
   	
    sprintf(caTemp,"select count(*) from (select mfrom,service from %s ",caTable);
    strcat(caTemp,caWhere);
    strcat(caTemp," group by mfrom,service) as tb ");
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
 //  	printf("table=%s,iReturn=%d,lCount=%d\n",caTable,iReturn,lCount);
   	if(iReturn==1146||lCount==0){
 		   continue;
   	}
   	lSum=lSum+lCount;
   	iNum++;
   	if(iNum==1){
   			sprintf(sqlbuf,"(select mfrom,mto,service,count(*) aa from %s ",caTable);
        strcat(sqlbuf,caWhere);
        sprintf(sqlbuf+strlen(sqlbuf)," group by mfrom,mto,service ");
        strcat(sqlbuf," )");
    }
    else{
    	  sprintf(sqlbuf+strlen(sqlbuf)," union all (select mfrom,mto,service,count(*) aa from %s ",caTable);
    	   strcat(sqlbuf,caWhere);
    	    sprintf(sqlbuf+strlen(sqlbuf)," group by mfrom,mto,service "); 
        strcat(sqlbuf," )");
    }
  }
 
   
    lStartRec = (lCurPg - 1) *lRowNum;  
 
  if(iNum>0){
  	sprintf(caTemp,"select tb.mfrom,tb.mto,tb.service,sum(aa) as bb from (%s) as tb group by tb.mfrom,tb.mto,tb.service ",sqlbuf);
  	sprintf(caTemp+strlen(caTemp)," order by bb desc limit %d,%d ",lStartRec,lRowNum);  
   
  }    
  else{
  	 utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
     utPltPutVarF(psDbHead,"TotPg", "%lu",0);
     utPltPutVarF(psDbHead,"CurPg", "%lu", 1);

     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
     return 0;
  }
  
  	
	
printf("caTemp=%s\n",caTemp);	 
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
    
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncsim%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
         
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }

      fprintf(fp,"用户聊天记录统计\n");
      ncUtlPrintCsv(fp,4,
                         UT_TYPE_STRING,"聊天工具",
                         UT_TYPE_STRING,"发送人",
                         UT_TYPE_STRING,"接收人",
                         UT_TYPE_STRING,"记录数");
                       
                                      
    memset(caMfrom,0,sizeof(caMfrom));
    memset(caMto,0,sizeof(caMto));
    lService=0;
    lCount=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,47,caMfrom,
		                               UT_TYPE_STRING,47,caMto,
		                               UT_TYPE_LONG,4,&lService,
		                               UT_TYPE_LONG,4,&lCount);
			    															 
  	                              
        while((iReturn == 0)||(iReturn==1405)) { 
       
      psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&lService); 
      if(psData){
          strcpy(caType,psData->caName);
      }
      else{
      	strcpy(caType,"");
      }        
      ncUtlPrintCsv(fp,4,UT_TYPE_STRING,caType,
                           UT_TYPE_STRING,caMfrom,
                           UT_TYPE_STRING,caMto,
                           UT_TYPE_LONG,lCount);
                      
                       
    memset(caMfrom,0,sizeof(caMfrom));
    memset(caMto,0,sizeof(caMto));
    lService=0;
    lCount=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,47,caMfrom,
		                               UT_TYPE_STRING,47,caMto,
		                               UT_TYPE_LONG,4,&lService,
		                               UT_TYPE_LONG,4,&lCount);
        }
        pasDbCloseCursor(psCur);
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);

        return 0;
    }
    
    

  

     
    iReturn = 0;
    memset(caMfrom,0,sizeof(caMfrom));
    memset(caMto,0,sizeof(caMto));
    lService=0;
    lCount=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,47,caMfrom,
		                               UT_TYPE_STRING,47,caMto,
		                               UT_TYPE_LONG,4,&lService,
		                               UT_TYPE_LONG,4,&lCount);
			                                  
			                                
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
      psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&lService); 
      if(psData){
          strcpy(caType,psData->caName);
      }
      else{
      	strcpy(caType,"");
      } 
	//printf("lservice=%d,caType=%s\n",lService,caType);        
	        utPltSetCvtHtml(1);

        utPltPutLoopVar(psDbHead,"mfrom",iNum,caMfrom);
        utPltPutLoopVar(psDbHead,"mto",iNum,caMto);
        utPltPutLoopVar(psDbHead,"service_desc",iNum,caType);

	        utPltSetCvtHtml(0);
	      
	      utPltPutLoopVarF(psDbHead,"lcount",iNum,"%d",lCount);
        utPltPutLoopVarF(psDbHead,"service",iNum,"%d",lService);
 
 

    memset(caMfrom,0,sizeof(caMfrom));
    memset(caMto,0,sizeof(caMto));
    lService=0;
    lCount=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,47,caMfrom,
		                               UT_TYPE_STRING,47,caMto,
		                               UT_TYPE_LONG,4,&lService,
		                               UT_TYPE_LONG,4,&lCount);
			                                  
    }



    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
    return 0;
}



//显示某个帐号聊天明细
int ncsImdetcontlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[20480];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
    char caSvalue[128],caSip[16];
  
   	float lRat;
   	long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;

	char sqlbuf[20480]="";
	unsigned long lStatrow,lSum;
   long lStime,lEtime;
 	pasDbCursor *psCur;
 
 	char caTable_hz[36];
 	char caSdate_d[16],caEdate_d[16];
 	char caTable[20],caUdisp[36];
 	long starttime;
  char caKeyword[64],caTime_flag[16];
  char caDid[16],caPage[12],caLimit[16],caSort[16],caDir[16];

  char caUid[16];
  char caExport[16];
  long mCount,lCmonth;
  char caUrlPath[256],caTitle[128],caUri[128],caPath[128];
  char caHost[64],caSname[20];
  char caTemp_d[1024];
  char caMesg[256],caMfrom[48],caMto[48],caMfrom2[48],caService[16];
  lCurPg=1;
 	strcpy(caWhere,"\0");

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,9,
			"did",UT_TYPE_STRING,12,caDid,
			"uid",UT_TYPE_STRING,10,caUid,
			"mfrom",UT_TYPE_STRING,47,caMfrom,
			"mto",UT_TYPE_STRING,47,caMto,
			"service", UT_TYPE_STRING,10,caService,
   	  "export1",     UT_TYPE_STRING,10,caExport,
      "TotPg",      UT_TYPE_STRING, 10, caTotPg,
      "CurPg",      UT_TYPE_STRING, 10, caCurPg,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag);
/*   	  
   	 if(strlen(caMfrom)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caMfrom,caTemp,46));
      	strcpy(caMfrom,caTemp_d);
      } 
      
   	 if(strlen(caMto)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caMto,caTemp,46));
      	strcpy(caMto,caTemp_d);
      } 
  */ 	  
    strcpy(caMfrom2,caMfrom);

      
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caCurPg);
    if(lCurPg <= 0) lCurPg = 1;

 

    if(!utStrIsSpaces(caExport)){
    lRowNum = 99999;
    lStartRec=0;
    }
    else{

         lRowNum =40;//每一页面行数
      
      lStartRec = (lCurPg - 1) * lRowNum;    	
    }
    
   	
  
  
   	                
   
   lTime=time(0);
   

   	  	if(atol(caTime_flag)==1){
   	  		sprintf(caStemptime,"%s 00:00",utTimFormat("%Y/%m/%d",lTime));
 //  	  		printf("caStemptime=%s,lTime=%d\n",caStemptime,lTime);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		letime=lTime;
   	  	}
        else if(atol(caTime_flag)==2){
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==3){
        	lstime=lTime-7*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==4){
        	lstime=lTime-30*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==5){
        	lstime=lTime-90*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==6){
        	lstime=lTime-180*3600*24;
        	letime=lTime;
        }
        else{
        	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
	   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
			     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
			     sprintf(caEtemptime,"%s 23:59",caEdate_d);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
     
	 memset(caWhere,0,sizeof(caWhere));
	 
	    lCmonth=0;   
nn:
	 
	 
	 sprintf(caWhere,"where stime>=%d and stime<=%d ",lstime,letime); 
   if(atol(caDid)>0){
	  	  utStrAddF(caWhere," and did=%lu ",atol(caDid));
	  	}
    if(atol(caUid)>0){
    	utStrAddF(caWhere," and uid=%lu ",atol(caUid));
    }
   
     if(strlen(caMfrom)>0){
        utStrAddF(caWhere," and (mfrom like '%c%s%c' or mto like '%c%s%c' ) ", '%',caMfrom,'%', '%',caMfrom,'%');
     }
    if(strlen(caMto)>0){
        utStrAddF(caWhere," and (mfrom like '%c%s%c' or mto like '%c%s%c' ) ", '%',caMto,'%', '%',caMto,'%');
     }
     if(strlen(caService)>0){
     	utStrAddF(caWhere," and service=%s ",caService);
    }
    psDbHead = utPltInitDb();
    
     utPltPutVar(psDbHead,"mfrom",caMfrom);
  	 utPltPutVar(psDbHead,"mto",caMto);
  	 utPltPutVar(psDbHead,"did",caDid);
  	 utPltPutVar(psDbHead,"uid",caUid);
  	 utPltPutVar(psDbHead,"service",caService);
  	 utPltPutVar(psDbHead,"time_flag",caTime_flag);
    	
    	
   lSum=0;
   iNum=0;

   char caPre_table[64],caEtable[16];
   strcpy(caEtable,utTimFormat("%Y/%m",letime));
   long lEtabe=utTimStrToLong("%Y/%m",caEtable)+86400*30;
   memset(caPre_table,0,sizeof(caPre_table));
   for(starttime=lstime;starttime<lEtabe;starttime+=2419200){
   	sprintf(caTable,"ncsimlog_if_%s",utTimFormat("%Y%m",starttime));
   	if(strcmp(caPre_table,caTable)==0){
   		continue;
   	}
   	strcpy(caPre_table,caTable);
   	
    sprintf(caTemp,"select count(*) from %s ",caTable);
    strcat(caTemp,caWhere);
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
 //  	printf("table=%s,iReturn=%d,lCount=%d\n",caTable,iReturn,lCount);
   	if(iReturn==1146||lCount==0){
 		   continue;
   	}
   	lSum=lSum+lCount;
   	iNum++;
   	if(iNum==1){
   			sprintf(sqlbuf,"(select stime,mfrom,mto,mesg from %s ",caTable);
        strcat(sqlbuf,caWhere);
       
        strcat(sqlbuf," )");
    }
    else{
    	  sprintf(sqlbuf+strlen(sqlbuf)," union all (select stime,mfrom,mto,mesg from %s ",caTable);
    	   strcat(sqlbuf,caWhere);
    	   
        strcat(sqlbuf," )");
    }
  }
 
   
 //   lStartRec = (lCurPg - 1) *lRowNum;  
 
  if(iNum>0){
  	sprintf(caTemp,"select tb.stime,tb.mfrom,tb.mto,tb.mesg from (%s) as tb ",sqlbuf);
  	sprintf(caTemp+strlen(caTemp)," order by tb.stime desc limit %d,%d ",lStartRec,lRowNum);  
   
  }    
  else{

  	 utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
     utPltPutVarF(psDbHead,"TotPg", "%lu",0);
     utPltPutVarF(psDbHead,"CurPg", "%lu", 1);

     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/userbh/ncsimdet_list.htm");
     return 0;
  }	              

    
   	              
 
 //   strcpy(caDispname,ncUtlBufGetUsername(psShmHead,atol(caUserid)));  	              
                 	              

	
printf("caTemp=%s\n",caTemp);	 
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
    
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncsim%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
         
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
        
        fprintf(fp,"用户聊天明细\n");
        fprintf(fp,"%s<===>%s聊天内容\n",caMfrom,caMto);
        
      ncUtlPrintCsv(fp,4,UT_TYPE_STRING,"时间",
                         UT_TYPE_STRING,"发送者",
                         UT_TYPE_STRING,"接收者",
                         UT_TYPE_STRING,"内容");
                       
                                      
     lStime=0;
    memset(caMesg,0,sizeof(caMesg));
    memset(caMto,0,sizeof(caMto));
    memset(caMfrom,0,sizeof(caMfrom));

    iReturn = pasDbFetchInto(psCur,
                                   UT_TYPE_ULONG,4,&lStime,
                                       UT_TYPE_STRING,47,caMfrom,
                                       UT_TYPE_STRING,47,caMto,
                                       UT_TYPE_STRING,255,caMesg);
			    															 
  	                              
        while((iReturn == 0)||(iReturn==1405)) { 
      
      
        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));	     
        ncUtlPrintCsv(fp,4,UT_TYPE_STRING,caSdate,
                           UT_TYPE_STRING,caMfrom,
                           UT_TYPE_STRING,caMto,
                           UT_TYPE_STRING,caMesg);
                           
                      
                       
     lStime=0;
    memset(caMesg,0,sizeof(caMesg));
    memset(caMto,0,sizeof(caMto));
    memset(caMfrom,0,sizeof(caMfrom));

    iReturn = pasDbFetchInto(psCur,
                                   UT_TYPE_ULONG,4,&lStime,
                                       UT_TYPE_STRING,47,caMfrom,
                                       UT_TYPE_STRING,47,caMto,
                                       UT_TYPE_STRING,255,caMesg);
        }
        pasDbCloseCursor(psCur);
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);

        return 0;
    }
    
    

  

     
    iReturn = 0;
     lStime=0;
    memset(caMesg,0,sizeof(caMesg));
    memset(caMto,0,sizeof(caMto));
    memset(caMfrom,0,sizeof(caMfrom));

    iReturn = pasDbFetchInto(psCur,
                                   UT_TYPE_ULONG,4,&lStime,
                                       UT_TYPE_STRING,47,caMfrom,
                                       UT_TYPE_STRING,47,caMto,
                                       UT_TYPE_STRING,255,caMesg);
			                                
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	  
	        iNum++;
	        
	        utPltSetCvtHtml(1);
    
        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));
            utPltPutLoopVar(psDbHead,"from",iNum,caMfrom);
            utPltPutLoopVar(psDbHead,"to",iNum,caMto);
            utPltPutLoopVar(psDbHead,"mesg",iNum,caMesg);

            if(strcmp(caMfrom,caMfrom2)==0){
                utPltPutLoopVar(psDbHead,"color",iNum,"blue");
            }
            else{
                utPltPutLoopVar(psDbHead,"color",iNum,"green");
            }

	        utPltSetCvtHtml(0);
	      
	      utPltPutLoopVar(psDbHead,"stime",iNum,caSdate);
         
 
 
     lStime=0;
    memset(caMesg,0,sizeof(caMesg));
    memset(caMto,0,sizeof(caMto));
    memset(caMfrom,0,sizeof(caMfrom));

    iReturn = pasDbFetchInto(psCur,
                                   UT_TYPE_ULONG,4,&lStime,
                                       UT_TYPE_STRING,47,caMfrom,
                                       UT_TYPE_STRING,47,caMto,
                                       UT_TYPE_STRING,255,caMesg);
    }



    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/userbh/ncsimdet_list.htm");
    return 0;
} 
   
   
   
//显示某个用户邮件发送明细
int ncsUserMailDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[20480];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn,lMailsize,lFlag,lCcode;
    char caMailfrom[128],caMailto[128],caMailsub[256],caMailfile[128],caType[32];
  
   	float lRat;
   	long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;

	char sqlbuf[20480]="";
	unsigned long lStatrow,lSum;
   long lStime,lEtime;
 	pasDbCursor *psCur;
 
 	char caTable_hz[36];
 	char caSdate_d[16],caEdate_d[16];
 	char caTable[20],caUdisp[36];
 	long starttime;
  char caKeyword[64],caTime_flag[16];
  char caDid[16],caPage[12],caLimit[16],caSort[16],caDir[16];

  char caUid[16];
  char caExport[16];
  long mCount,lCmonth;
  char caUrlPath[256],caTitle[128],caUri[128],caPath[128];
  char caHost[64],caSname[20];
  char caTemp_d[1024];
  char caSubject[256],caContent[1024],caAuthor[48],caUpfile[48];
  lCurPg=1;
 	strcpy(caWhere,"\0");

 	

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,9,
			"did",UT_TYPE_STRING,12,caDid,
			"uid",UT_TYPE_STRING,10,caUid,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "export1",     UT_TYPE_STRING,10,caExport,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag);
   	  
    
      if(strlen(caKeyword)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
      }
      
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 

    if(!utStrIsSpaces(caExport)){
    lRowNum = 99999;
    lStartRec=0;
    }
    else{
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
    }
   	                
   
   lTime=time(0);
   

   	  	if(atol(caTime_flag)==1){
   	  		sprintf(caStemptime,"%s 00:00",utTimFormat("%Y/%m/%d",lTime));
 //  	  		printf("caStemptime=%s,lTime=%d\n",caStemptime,lTime);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		letime=lTime;
   	  	}
        else if(atol(caTime_flag)==2){
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==3){
        	lstime=lTime-7*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==4){
        	lstime=lTime-30*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==5){
        	lstime=lTime-90*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==6){
        	lstime=lTime-180*3600*24;
        	letime=lTime;
        }
        else{
        	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
	   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
			     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
			     sprintf(caEtemptime,"%s 23:59",caEdate_d);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	 
             
   utStrDelSpaces(caKeyword);      
	 memset(caWhere,0,sizeof(caWhere));
	 
	    lCmonth=0;   
nn:
	 
	 
	 sprintf(caWhere,"where stime>=%d and stime<=%d ",lstime,letime); 
   if(atol(caDid)>0){
	  	  utStrAddF(caWhere," and did=%lu ",atol(caDid));
	  	}
    if(atol(caUid)>0){
    	utStrAddF(caWhere," and uid=%lu ",atol(caUid));
    }
   
     if(strlen(caKeyword)>0){
        utStrAddF(caWhere," and (mailfrom like '%c%s%c' or mailto like '%c%s%c' or mailsub like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
     }
    	
    
    	
    psDbHead = utPltInitDb();
    
    
// printf("caWhere=%s\n",caWhere);  
    
    	
    	
   lSum=0;
   iNum=0;

   char caPre_table[64],caEtable[16];
   strcpy(caEtable,utTimFormat("%Y/%m",letime));
   long lEtabe=utTimStrToLong("%Y/%m",caEtable)+86400*30;
   memset(caPre_table,0,sizeof(caPre_table));
   for(starttime=lstime;starttime<lEtabe;starttime+=2419200){
   	sprintf(caTable,"ncsmailmon_if_%s",utTimFormat("%Y%m",starttime));
   	if(strcmp(caPre_table,caTable)==0){
   		continue;
   	}
   	strcpy(caPre_table,caTable);
   	
    sprintf(caTemp,"select count(*) from %s ",caTable);
    strcat(caTemp,caWhere);
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
 //  	printf("table=%s,iReturn=%d,lCount=%d\n",caTable,iReturn,lCount);
   	if(iReturn==1146||lCount==0){
 		   continue;
   	}
   	lSum=lSum+lCount;
   	iNum++;
   	if(iNum==1){
   			sprintf(sqlbuf,"(select mailfrom,mailto,mailsub,stime,mailsize,mailfile,flag,ccode from %s ",caTable);
        strcat(sqlbuf,caWhere);
       
        strcat(sqlbuf," )");
    }
    else{
    	  sprintf(sqlbuf+strlen(sqlbuf)," union all (select mailfrom,mailto,mailsub,stime,mailsize,mailfile,flag,ccode from %s ",caTable);
    	   strcat(sqlbuf,caWhere);
    	   
        strcat(sqlbuf," )");
    }
  }
 
   
    lStartRec = (lCurPg - 1) *lRowNum;  
 
     if(!utStrIsSpaces(caExport)){
     	lStartRec=0;
    	lRowNum=9999;
    }
 
  if(iNum>0){
  	sprintf(caTemp,"select tb.mailfrom,tb.mailto,tb.mailsub,tb.stime,tb.mailsize,tb.mailfile,tb.flag,tb.ccode from (%s) as tb ",sqlbuf);
  	sprintf(caTemp+strlen(caTemp)," order by tb.stime desc limit %d,%d ",lStartRec,lRowNum);  
   
  }    
  else{
  	 utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
     utPltPutVarF(psDbHead,"TotPg", "%lu",0);
     utPltPutVarF(psDbHead,"CurPg", "%lu", 1);

     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
     return 0;
  }	              

    
   	              
 
 //   strcpy(caDispname,ncUtlBufGetUsername(psShmHead,atol(caUserid)));  	              
                 	              

	
printf("caTemp=%s\n",caTemp);	 
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
    
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncsmail%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
         
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
        
        fprintf(fp,"用户邮件发送明细\n");
      ncUtlPrintCsv(fp,6,UT_TYPE_STRING,"类型",
                         UT_TYPE_STRING,"发件人",
                         UT_TYPE_STRING,"收件人",
                         UT_TYPE_STRING,"主题",
                         UT_TYPE_STRING,"时间",
                         UT_TYPE_STRING,"邮件大小");
                       
    memset(caMailfrom,0,sizeof(caMailfrom));
    memset(caMailto,0,sizeof(caMailto));
    memset(caMailsub,0,sizeof(caMailsub));
    lStime=0;
    lMailsize=0;
    memset(caMailfile,0,sizeof(caMailfile));
    lFlag=0;
    lCcode=0;                                
   
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caMailfrom,
		                               UT_TYPE_STRING,127,caMailto,
		                               UT_TYPE_STRING,255,caMailsub,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_LONG,4,&lMailsize,
			                                   UT_TYPE_STRING,127,caMailfile,
			                                   UT_TYPE_LONG,4,&lFlag,
			                                   UT_TYPE_LONG,4,&lCcode);
			                                 
			    															 
  	                              
        while((iReturn == 0)||(iReturn==1405)) { 
       if(strlen(caMailsub)>0){
      	strcpy(caTemp_d,pasCvtGBK(lCcode,caMailsub,caTemp,254));
      	strcpy(caMailsub,caTemp_d);
       }
       if(strlen(caMailto)>0){
      	strcpy(caTemp_d,pasCvtGBK(lCcode,caMailto,caTemp,127));
      	strcpy(caMailto,caTemp_d);
       }
       if(strlen(caMailfrom)>0){
      	strcpy(caTemp_d,pasCvtGBK(lCcode,caMailfrom,caTemp,127));
      	strcpy(caMailfrom,caTemp_d);
       }
     
      if(lFlag==1){
      	strcpy(caType,"SMTP");
      }
      else if(lFlag==2){
      	strcpy(caType,"POP3");
      }
      else{
      	strcpy(caType,"Webmail");
      }
     
        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));	     
        ncUtlPrintCsv(fp,6,UT_TYPE_STRING,caType,
                           UT_TYPE_STRING,caMailfrom,
                           UT_TYPE_STRING,caMailto,
                           UT_TYPE_STRING,caMailsub,
                           UT_TYPE_STRING,caSdate,
                           UT_TYPE_LONG,lMailsize);
                      
                       
    memset(caMailfrom,0,sizeof(caMailfrom));
    memset(caMailto,0,sizeof(caMailto));
    memset(caMailsub,0,sizeof(caMailsub));
    lStime=0;
    lMailsize=0;
    memset(caMailfile,0,sizeof(caMailfile));
    lFlag=0;
    lCcode=0;                                
   
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caMailfrom,
		                               UT_TYPE_STRING,127,caMailto,
		                               UT_TYPE_STRING,255,caMailsub,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_LONG,4,&lMailsize,
			                                   UT_TYPE_STRING,127,caMailfile,
			                                   UT_TYPE_LONG,4,&lFlag,
			                                   UT_TYPE_LONG,4,&lCcode);
        }
        pasDbCloseCursor(psCur);
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);

        return 0;
    }
    
    

  

     
    iReturn = 0;
    memset(caMailfrom,0,sizeof(caMailfrom));
    memset(caMailto,0,sizeof(caMailto));
    memset(caMailsub,0,sizeof(caMailsub));
    lStime=0;
    lMailsize=0;
    memset(caMailfile,0,sizeof(caMailfile));
    lFlag=0;
    lCcode=0;                                
   
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caMailfrom,
		                               UT_TYPE_STRING,127,caMailto,
		                               UT_TYPE_STRING,255,caMailsub,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_LONG,4,&lMailsize,
			                                   UT_TYPE_STRING,127,caMailfile,
			                                   UT_TYPE_LONG,4,&lFlag,
			                                   UT_TYPE_LONG,4,&lCcode);
			                                  
			                                
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
	        utPltSetCvtHtml(1);
       if(strlen(caMailsub)>0){
      	strcpy(caTemp_d,pasCvtGBK(lCcode,caMailsub,caTemp,254));
      	strcpy(caMailsub,caTemp_d);
       }
       if(strlen(caMailto)>0){
      	strcpy(caTemp_d,pasCvtGBK(lCcode,caMailto,caTemp,127));
      	strcpy(caMailto,caTemp_d);
       }
       if(strlen(caMailfrom)>0){
      	strcpy(caTemp_d,pasCvtGBK(lCcode,caMailfrom,caTemp,127));
      	strcpy(caMailfrom,caTemp_d);
       }
     
      if(lFlag==1){
      	strcpy(caType,"SMTP");
      }
      else if(lFlag==2){
      	strcpy(caType,"POP3");
      }
      else{
      	strcpy(caType,"Webmail");
      }
        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));
        utPltPutLoopVar(psDbHead,"subject",iNum,caMailsub);
        utPltPutLoopVar(psDbHead,"mfrom",iNum,caMailfrom);
        utPltPutLoopVar(psDbHead,"mto",iNum,caMailto);
        utPltPutLoopVar(psDbHead,"service_desc",iNum,caType);
        
        utPltPutLoopVar(psDbHead,"upfile",iNum,caMailfile);
       

	        utPltSetCvtHtml(0);
	      utPltPutLoopVarF(psDbHead,"lcount",iNum,"%d",lMailsize);
	      utPltPutLoopVar(psDbHead,"stime",iNum,caSdate);
        utPltPutLoopVarF(psDbHead,"ccode",iNum,"%d",lCcode);
 
 
    memset(caMailfrom,0,sizeof(caMailfrom));
    memset(caMailto,0,sizeof(caMailto));
    memset(caMailsub,0,sizeof(caMailsub));
    lStime=0;
    lMailsize=0;
    memset(caMailfile,0,sizeof(caMailfile));
    lFlag=0;
    lCcode=0;                                
   
		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,127,caMailfrom,
		                               UT_TYPE_STRING,127,caMailto,
		                               UT_TYPE_STRING,255,caMailsub,
			                                   UT_TYPE_LONG,4,&lStime,
			                                   UT_TYPE_LONG,4,&lMailsize,
			                                   UT_TYPE_STRING,127,caMailfile,
			                                   UT_TYPE_LONG,4,&lFlag,
			                                   UT_TYPE_LONG,4,&lCcode);
    }



    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
    return 0;
}   



    //统计个人虚拟身份次数
int ncsUserNetidStat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[20480];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
    char caSvalue[128],caSname[128];
    ncsNettype *psData;
   	float lRat;
   	long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;
  char caName[256];
	char sqlbuf[20480]="";
	unsigned long lStatrow,lSum;
   long lStime,lEtime;
 	pasDbCursor *psCur;
 
 	char caTable_hz[36];
 	char caSdate_d[16],caEdate_d[16];
 	char caTable[20],caUdisp[36];
 	long starttime;
  char caKeyword[64],caTime_flag[16];
  char caDid[16],caPage[12],caLimit[16],caSort[16],caDir[16];
  char caBbsname[48],caUrl[128];
  char caUid[16];
  char caExport[16];
  long mCount,lCmonth;
  char caMfrom[48],caMto[48],caType[48];
  long lService;
  char caHost[64];
  char caTemp_d[68];
  lCurPg=1;
 	strcpy(caWhere,"\0");

 	

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,11,
			"did",UT_TYPE_STRING,12,caDid,
			"uid",UT_TYPE_STRING,10,caUid,
			"sdate",UT_TYPE_STRING,19,caSdate,
			"edate",UT_TYPE_STRING,19,caEdate,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "export1",     UT_TYPE_STRING,10,caExport,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag);
   	  
    
  //    if(strlen(caKeyword)>0){
  //    	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
  //    	strcpy(caKeyword,caTemp_d);
 //     }
      
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 

    if(!utStrIsSpaces(caExport)){
    lRowNum = 99999;
    lStartRec=0;
    }
    else{
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
    }
    
   	
  
  
   	                
   
   lTime=time(0);
   
   if(strlen(caSdate)>0||strlen(caEdate)>0){
   	  if(strlen(caSdate)==0){
   	  	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
		     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
		     sprintf(caEtemptime,"%s 23:59",caEdate,caEtime);
		     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
   	  }
   	  else{
   	  	sprintf(caStemptime,"%s 00:00",caSdate);
   	  	lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  	letime=lTime;
   	  }
   	 }
   	  else{
   	  	if(atol(caTime_flag)==1){
   	  		sprintf(caStemptime,"%s 00:00",utTimFormat("%Y/%m/%d",lTime));
 //  	  		printf("caStemptime=%s,lTime=%d\n",caStemptime,lTime);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		letime=lTime;
   	  	}
        else if(atol(caTime_flag)==2){
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==3){
        	lstime=lTime-7*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==4){
        	lstime=lTime-30*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==5){
        	lstime=lTime-90*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==6){
        	lstime=lTime-180*3600*24;
        	letime=lTime;
        }
        else{
        	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
	   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
			     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
			     sprintf(caEtemptime,"%s 23:59",caEdate_d);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	  }
             
   utStrDelSpaces(caKeyword);      
	 memset(caWhere,0,sizeof(caWhere));
	 
	    lCmonth=0;   
nn:
	 
	 
	 sprintf(caWhere,"where stime>=%d and stime<=%d ",lstime,letime); 
   if(atol(caDid)>0){
	  	  utStrAddF(caWhere," and did=%lu ",atol(caDid));
	  	}
    if(atol(caUid)>0){
    	utStrAddF(caWhere," and uid=%lu ",atol(caUid));
    }
   
     if(strlen(caKeyword)>0){
        utStrAddF(caWhere," and (name like '%c%s%c' ) ", '%',caKeyword,'%');
     }
    	
    
    	
    psDbHead = utPltInitDb();
    
    
// printf("caWhere=%s\n",caWhere);  
    
    	
    	
   lSum=0;
   iNum=0;

   char caPre_table[64],caEtable[16];
   strcpy(caEtable,utTimFormat("%Y/%m",letime));
   long lEtabe=utTimStrToLong("%Y/%m",caEtable)+86400*30;
   memset(caPre_table,0,sizeof(caPre_table));
   for(starttime=lstime;starttime<lEtabe;starttime+=2419200){
   	sprintf(caTable,"ncsnetlog_if_%s",utTimFormat("%Y%m",starttime));
   	if(strcmp(caPre_table,caTable)==0){
   		continue;
   	}
   	strcpy(caPre_table,caTable);
   	
    sprintf(caTemp,"select count(*) from (select  name,ntype from %s ",caTable);
    strcat(caTemp,caWhere);
    strcat(caTemp," group by name,ntype ) as tb ");
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
 //  	printf("table=%s,iReturn=%d,lCount=%d\n",caTable,iReturn,lCount);
   	if(iReturn==1146||lCount==0){
 		   continue;
   	}
   	lSum=lSum+lCount;
   	iNum++;
   	if(iNum==1){
   			sprintf(sqlbuf,"(select name,ntype,count(*) aa,max(stime) stime from %s ",caTable);
        strcat(sqlbuf,caWhere);
        sprintf(sqlbuf+strlen(sqlbuf)," group by name,ntype ");
        strcat(sqlbuf," )");
    }
    else{
    	  sprintf(sqlbuf+strlen(sqlbuf)," union all (select name,ntype,count(*) aa,max(stime) stime from %s ",caTable);
    	   strcat(sqlbuf,caWhere);
    	    sprintf(sqlbuf+strlen(sqlbuf)," group by name,ntype "); 
        strcat(sqlbuf," )");
    }
  }
 
   
    lStartRec = (lCurPg - 1) *lRowNum;  
 
  if(iNum>0){
  	sprintf(caTemp,"select tb.name,tb.ntype,sum(aa) as bb,max(tb.stime) from (%s) as tb group by tb.name,tb.ntype ",sqlbuf);
  	sprintf(caTemp+strlen(caTemp)," order by bb desc limit %d,%d ",lStartRec,lRowNum);  
   
  }    
  else{
  	 utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
     utPltPutVarF(psDbHead,"TotPg", "%lu",0);
     utPltPutVarF(psDbHead,"CurPg", "%lu", 1);

     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
     return 0;
  }
  
  	
	
printf("caTemp=%s\n",caTemp);	 
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
    
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncsnetid%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
         
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }

      fprintf(fp,"用户虚拟身份统计\n");
      ncUtlPrintCsv(fp,4,
                         UT_TYPE_STRING,"虚拟身份帐号",
                         UT_TYPE_STRING,"虚拟身份类别",
                         UT_TYPE_STRING,"记录数",
                         UT_TYPE_STRING,"最近访问时间");
                       
                                      
    memset(caName,0,sizeof(caName));
    lService=0;
    lCount=0;
    lStime=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,254,caName,
		                               UT_TYPE_LONG,4,&lService,
		                               UT_TYPE_LONG,4,&lCount,
		                               UT_TYPE_LONG,4,&lStime);
			    															 
  	                              
        while((iReturn == 0)||(iReturn==1405)) { 
  
      psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&lService); 
      if(psData){
          strcpy(caType,psData->caName);
      }
      else{
      	strcpy(caType,"");
      }    
      
      strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));
          
      ncUtlPrintCsv(fp,4,  UT_TYPE_STRING,caName,
                           UT_TYPE_STRING,caType,
                           UT_TYPE_LONG,lCount,
                           UT_TYPE_STRING,caSdate);
                      
                   
    memset(caName,0,sizeof(caName));
    lService=0;
    lCount=0;
    lStime=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,254,caName,
		                               UT_TYPE_LONG,4,&lService,
		                               UT_TYPE_LONG,4,&lCount,
		                               UT_TYPE_LONG,4,&lStime);
        }
        pasDbCloseCursor(psCur);
        fclose(fp);

        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);

        return 0;
    }
    
    

  

     
    iReturn = 0;
    memset(caName,0,sizeof(caName));
    lService=0;
    lCount=0;
    lStime=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,254,caName,
		                               UT_TYPE_LONG,4,&lService,
		                               UT_TYPE_LONG,4,&lCount,
		                               UT_TYPE_LONG,4,&lStime);
			                                  
			                                
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
      psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&lService); 
      if(psData){
          strcpy(caType,psData->caName);
      }
      else{
      	strcpy(caType,"");
      } 
	//printf("lservice=%d,caType=%s\n",lService,caType);        
	        utPltSetCvtHtml(1);

        utPltPutLoopVar(psDbHead,"dispname",iNum,caName);
        utPltPutLoopVar(psDbHead,"service_desc",iNum,caType);

	        utPltSetCvtHtml(0);
	       strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));
	       utPltPutLoopVar(psDbHead,"stime",iNum,caSdate);
	      utPltPutLoopVarF(psDbHead,"lcount",iNum,"%d",lCount);
        utPltPutLoopVarF(psDbHead,"service",iNum,"%d",lService);
 
 
    memset(caName,0,sizeof(caName));
    lService=0;
    lCount=0;
    lStime=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,254,caName,
		                               UT_TYPE_LONG,4,&lService,
		                               UT_TYPE_LONG,4,&lCount,
		                               UT_TYPE_LONG,4,&lStime);
			                                  
    }



    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
    return 0;
} 



    //个人使用虚拟身份明细
int ncsUserNetidDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[20480];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
    char caSvalue[128],caSname[128];
    ncsNettype *psData;
   	float lRat;
   	long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;
  char caName[256];
	char sqlbuf[20480]="";
	unsigned long lStatrow,lSum;
   long lStime,lEtime;
 	pasDbCursor *psCur;
 
 	char caTable_hz[36];
 	char caSdate_d[16],caEdate_d[16];
 	char caTable[20],caUdisp[36];
 	long starttime;
  char caKeyword[64],caTime_flag[16];
  char caDid[16],caPage[12],caLimit[16],caSort[16],caDir[16];
  char caBbsname[48],caUrl[128];
  char caUid[16];
  char caExport[16];
  long mCount,lCmonth;
  char caMfrom[48],caMto[48],caType[48];
  long lService;
  char caHost[64];
  char caTemp_d[68];
  char caService[16],caSip[16];
  lCurPg=1;
 	strcpy(caWhere,"\0");

 	

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,12,
			"did",UT_TYPE_STRING,12,caDid,
			"uid",UT_TYPE_STRING,10,caUid,
			"sdate",UT_TYPE_STRING,19,caSdate,
			"edate",UT_TYPE_STRING,19,caEdate,
			"service",UT_TYPE_STRING,10,caService,
			"dispname", UT_TYPE_STRING,60,caKeyword,
   	  "export1",     UT_TYPE_STRING,10,caExport,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag);
   	  
    
    if(strlen(caKeyword)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
    	strcpy(caKeyword,caTemp_d);
     }
      
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 

    if(!utStrIsSpaces(caExport)){
    lRowNum = 99999;
    lStartRec=0;
    }
    else{
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
    }
    
   	
  
  
   	                
   
   lTime=time(0);
   
   if(strlen(caSdate)>0||strlen(caEdate)>0){
   	  if(strlen(caSdate)==0){
   	  	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
		     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
		     sprintf(caEtemptime,"%s 23:59",caEdate,caEtime);
		     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
   	  }
   	  else{
   	  	sprintf(caStemptime,"%s 00:00",caSdate);
   	  	lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  	letime=lTime;
   	  }
   	 }
   	  else{
   	  	if(atol(caTime_flag)==1){
   	  		sprintf(caStemptime,"%s 00:00",utTimFormat("%Y/%m/%d",lTime));
 //  	  		printf("caStemptime=%s,lTime=%d\n",caStemptime,lTime);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		letime=lTime;
   	  	}
        else if(atol(caTime_flag)==2){
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==3){
        	lstime=lTime-7*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==4){
        	lstime=lTime-30*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==5){
        	lstime=lTime-90*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==6){
        	lstime=lTime-180*3600*24;
        	letime=lTime;
        }
        else{
        	 ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
	   	  	 sprintf(caStemptime,"%s 00:00",caSdate_d);
			     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
			     sprintf(caEtemptime,"%s 23:59",caEdate_d);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	  }
             
   utStrDelSpaces(caKeyword);      
	 memset(caWhere,0,sizeof(caWhere));
	 
	    lCmonth=0;   
nn:
	 
	 
	 sprintf(caWhere,"where stime>=%d and stime<=%d ",lstime,letime); 
   if(atol(caDid)>0){
	  	  utStrAddF(caWhere," and did=%lu ",atol(caDid));
	  	}
    if(atol(caUid)>0){
    	utStrAddF(caWhere," and uid=%lu ",atol(caUid));
    }
   
     if(strlen(caKeyword)>0){
        utStrAddF(caWhere," and name='%s' ", caKeyword);
     }
    if(strlen(caService)>0){
    	utStrAddF(caWhere," and ntype=%s ",caService);
    }
    
    	
    psDbHead = utPltInitDb();
    
    
// printf("caWhere=%s\n",caWhere);  
    
    	
    	
   lSum=0;
   iNum=0;

   char caPre_table[64],caEtable[16];
   strcpy(caEtable,utTimFormat("%Y/%m",letime));
   long lEtabe=utTimStrToLong("%Y/%m",caEtable)+86400*30;
   memset(caPre_table,0,sizeof(caPre_table));
   for(starttime=lstime;starttime<lEtabe;starttime+=2419200){
   	sprintf(caTable,"ncsnetlog_if_%s",utTimFormat("%Y%m",starttime));
   	if(strcmp(caPre_table,caTable)==0){
   		continue;
   	}
   	strcpy(caPre_table,caTable);
   	
    sprintf(caTemp,"select count(*) from %s ",caTable);
    strcat(caTemp,caWhere);
    lCount=0;
    printf("caTemp=%s\n",caTemp);
    iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
 //  	printf("table=%s,iReturn=%d,lCount=%d\n",caTable,iReturn,lCount);
   	if(iReturn==1146||lCount==0){
 		   continue;
   	}
   	lSum=lSum+lCount;
   	iNum++;
   	if(iNum==1){
   			sprintf(sqlbuf,"(select name,ntype,stime,sip  from %s ",caTable);
        strcat(sqlbuf,caWhere);
  
        strcat(sqlbuf," )");
    }
    else{
    	  sprintf(sqlbuf+strlen(sqlbuf)," union all (select name,ntype,stime,sip from %s ",caTable);
    	   strcat(sqlbuf,caWhere);
    	   
        strcat(sqlbuf," )");
    }
  }
 
   
    lStartRec = (lCurPg - 1) *lRowNum;  
 
  if(iNum>0){
  	sprintf(caTemp,"select tb.name,tb.ntype,tb.stime,tb.sip from (%s) as tb  ",sqlbuf);
  	sprintf(caTemp+strlen(caTemp)," order by tb.stime desc limit %d,%d ",lStartRec,lRowNum);  
   
  }    
  else{
  	 utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
     utPltPutVarF(psDbHead,"TotPg", "%lu",0);
     utPltPutVarF(psDbHead,"CurPg", "%lu", 1);

     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
     return 0;
  }
  
  	
	
printf("caTemp=%s\n",caTemp);	 
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
    
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncsnetid%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
         
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }

      fprintf(fp,"用户虚拟身份使用情况\n");
      ncUtlPrintCsv(fp,4,
                         UT_TYPE_STRING,"虚拟身份帐号",
                         UT_TYPE_STRING,"虚拟身份类别",
                         UT_TYPE_STRING,"上网时间",
                         UT_TYPE_STRING,"源IP");
                       
                                      
    memset(caName,0,sizeof(caName));
    lService=0;
    memset(caSip,0,sizeof(caSip));
    lStime=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,254,caName,
		                               UT_TYPE_LONG,4,&lService,
		                               UT_TYPE_LONG,4,&lStime,
		                               UT_TYPE_STRING,15,caSip);
			    															 
  	                              
        while((iReturn == 0)||(iReturn==1405)) { 
  
      psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&lService); 
      if(psData){
          strcpy(caType,psData->caName);
      }
      else{
      	strcpy(caType,"");
      }    
      
      strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));
          
      ncUtlPrintCsv(fp,4,  UT_TYPE_STRING,caName,
                           UT_TYPE_STRING,caType,
                           UT_TYPE_STRING,caSdate,
                           UT_TYPE_STRING,caSip);
                      
                   
    memset(caName,0,sizeof(caName));
    lService=0;
    memset(caSip,0,sizeof(caSip));
    lStime=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,254,caName,
		                               UT_TYPE_LONG,4,&lService,
		                               UT_TYPE_LONG,4,&lStime,
		                               UT_TYPE_STRING,15,caSip);
        }
        pasDbCloseCursor(psCur);
        fclose(fp);

        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);

        return 0;
    }
    
    

  

     
    iReturn = 0;
    memset(caName,0,sizeof(caName));
    lService=0;
    memset(caSip,0,sizeof(caSip));
    lStime=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,254,caName,
		                               UT_TYPE_LONG,4,&lService,
		                               UT_TYPE_LONG,4,&lStime,
		                               UT_TYPE_STRING,15,caSip);
			                                  
			                                
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
      psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&lService); 
      if(psData){
          strcpy(caType,psData->caName);
      }
      else{
      	strcpy(caType,"");
      } 
	//printf("lservice=%d,caType=%s\n",lService,caType);        
	        utPltSetCvtHtml(1);

        utPltPutLoopVar(psDbHead,"dispname",iNum,caName);
        utPltPutLoopVar(psDbHead,"service_desc",iNum,caType);

	        utPltSetCvtHtml(0);
	       strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lStime));
	       utPltPutLoopVar(psDbHead,"stime",iNum,caSdate);
         utPltPutLoopVarF(psDbHead,"service",iNum,"%d",lService);
         utPltPutLoopVar(psDbHead,"sip",iNum,caSip);
 
    memset(caName,0,sizeof(caName));
    lService=0;
    memset(caSip,0,sizeof(caSip));
    lStime=0;
		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_STRING,254,caName,
		                               UT_TYPE_LONG,4,&lService,
		                               UT_TYPE_LONG,4,&lStime,
		                               UT_TYPE_STRING,15,caSip);
			                                  
    }



    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
 //   utPltShowDb(psDbHead);
        
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
    return 0;
}


//显示物流线索
int ncsLogistlog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		char caExport[32];
	  FILE *fp;
		char caFilename[32];
		char caFile[128];
		char caPath[128];
		 char caKflag[16],caSid[16];
	 long lFlag;
	 char caFlag[16],caSet[16];
	 	  char caTname[36];
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec;
	  char caTemp_d[128];
	  long lSid,lTid;
	  long iNum;
    ncsClient * psClient;
    char caDispname[32],caSip[20],caMac[20],caIdno[24],caKeycond[64];
    char caSdate[20],caEdate[20],caExp[16],caTime_flag[12],caField_flag[12];
    long lTime,lstime,letime,lDid,lUid,lVtime,lKid,lType;
    char caStemptime[24],caIdcard[24],caIdtype[16],caPosition[32],caSubject[256],caContent[1024],caDescr[256],caUrl[128];
	 char caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caKid[16],caCname[64];
	 char caHost[64],caTitle[128];
	 long lUrlid;
	 char corpname[64];
	 char caDid[16],caGroupid[16],caDdno[32],caMfrom[64],caFromadress[128],caFromtel[20],caEmail[64];
	 char caMto[64],caToadress[128],caTotel[20];
	  utMsgPrintMsg(psMsgHead);

	  iReturn = utMsgGetSomeNVar(psMsgHead,17,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  		          
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,			          
			             "dispname",UT_TYPE_STRING,31,caDispname,
			             "cname",UT_TYPE_STRING,63,caCname,
			             "sip",UT_TYPE_STRING,15,caSip,
			             "mac",UT_TYPE_STRING,20,caMac,
			             "idno",UT_TYPE_STRING,20,caIdno,
			             "position",UT_TYPE_STRING,30,caPosition,
			             "sdate",UT_TYPE_STRING,10,caSdate,
			             "edate",UT_TYPE_STRING,10,caEdate,
			             "start",UT_TYPE_STRING,10,caStart,
			             "exp",UT_TYPE_STRING,10,caExport,
			            "time_flag",    UT_TYPE_STRING,8,caTime_flag,
			            "did", UT_TYPE_STRING,10,caDid,
			            "groupid",UT_TYPE_STRING,10,caGroupid);

			    
	  if(strlen(caDispname)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caDispname,caTemp,31)); 
	  	strcpy(caDispname,caTemp_d);
	  }
	  if(strlen(caCname)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caCname,caTemp,31)); 
	  	strcpy(caCname,caTemp_d);
	  }
	  if(strlen(caKeyword)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,63));
	  	strcpy(caKeyword,caTemp_d);
	  }          
	               
	
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  if(strlen(caExport)>0){		
		lStartRec=0;
		lRowNum=99999;
		        sprintf(caFilename,"ncslogistlog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"物流线索日志\n");
		        ncUtlPrintCsv(fp,14,
		                           UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"源IP",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"MAC地址",
		                           UT_TYPE_STRING,"url",
		                           UT_TYPE_STRING,"货单号",
		                           UT_TYPE_STRING,"发货人",
		                           UT_TYPE_STRING,"发货地址",
		                           UT_TYPE_STRING,"发货人电话",
		                           UT_TYPE_STRING,"收货人",
		                           UT_TYPE_STRING,"收货人地址",
		                           UT_TYPE_STRING,"收货人电话",
		                           UT_TYPE_STRING,"Email");
		                          
		                          
		                          
			
			  
		
		
	 }
	  
	  
	      lTime=time(0);
    
    if(atol(caTime_flag)==1){
   	  		sprintf(caStemptime,"%s 00:00",utTimFormat("%Y/%m/%d",lTime));
 //  	  		printf("caStemptime=%s,lTime=%d\n",caStemptime,lTime);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		letime=lTime;
   	  	}
        else if(atol(caTime_flag)==2){
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==3){
        	lstime=lTime-7*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==4){
        	lstime=lTime-30*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==5){
        	lstime=lTime-90*3600*24;
        	letime=lTime;
        }
        else if(atol(caTime_flag)==6){
        	lstime=lTime-180*3600*24;
        	letime=lTime;
        }
        else{
        	lstime=lTime-3*3600*24;
        	letime=lTime;
        }
	  
  
	  
	  psDbHead = utPltInitDb(); 
	  
	
	  
	  sprintf(caTemp,"select count(*) from ncslogistlog where 1=1 ");
  	if(strlen(caDid)>0){
	    	sprintf(caTemp+strlen(caTemp)," and did=%s ",caDid);
	  }
    if(strlen(caGroupid)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid=%s ",caGroupid);
    }
    
	  if(strlen(caSdate)==0&&atol(caTime_flag)>0){
    		sprintf(caTemp+strlen(caTemp)," and vtime>=%lu and vtime<=%lu ",lstime,letime);
    }

    if(strlen(caSdate)>0){
    	   	 sprintf(caStemptime,"%s 00:00",caSdate);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		sprintf(caTemp+strlen(caTemp)," and vtime>=%d ",lstime);
   	  		if(strlen(caEdate)>0){
   	  		  sprintf(caStemptime,"%s 23:59",caEdate);
   	  		  letime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		  sprintf(caTemp+strlen(caTemp)," and vtime<=%d ",letime);
   	  		}
    }
    if(strlen(caDispname)>0){
    	sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caDispname,'%');
    }
    if(strlen(caMac)>0){
    	sprintf(caTemp+strlen(caTemp)," and mac like '%c%s%c' ",'%',caMac,'%');
    }
    if(strlen(caSip)>0){
    	sprintf(caTemp+strlen(caTemp)," and sip like '%c%s%c' ",'%',caSip,'%');
    }
    if(strlen(caIdno)>0){
    	sprintf(caTemp+strlen(caTemp)," and idcard like '%c%s%c' ",'%',caIdno,'%');
    }
    if(strlen(caPosition)>0){
    	sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caPosition,'%');
    }
    if(strlen(caCname)>0){
    	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caCname,'%','%',caCname,'%');
    	
    }
    if(strlen(caKeyword)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (url like '%c%s%c' or ddno like '%c%s%c' or mfrom like '%c%s%c' or fromadress like '%c%s%c' or fromtel like '%c%s%c' or mto like '%c%s%c' or toaddress like '%c%s%c' or totel like '%c%s%c' or email like '%c%s%c'  ) ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    }

    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select sid,did,uid,dispname,idcard,mac,idtype,position,sip,vtime,url,ddno,mfrom,fromadress,fromtel,mto,toaddress,totel,email from ncslogistlog  where 1=1");
  	if(strlen(caDid)>0){
	    	sprintf(caTemp+strlen(caTemp)," and did=%s ",caDid);
	  }
    if(strlen(caGroupid)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid=%s ",caGroupid);
    }
    
	  if(strlen(caSdate)==0&&atol(caTime_flag)>0){
    		sprintf(caTemp+strlen(caTemp)," and vtime>=%lu and vtime<=%lu ",lstime,letime);
    }

    if(strlen(caSdate)>0){
    	   	 sprintf(caStemptime,"%s 00:00",caSdate);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		sprintf(caTemp+strlen(caTemp)," and vtime>=%d ",lstime);
   	  		if(strlen(caEdate)>0){
   	  		  sprintf(caStemptime,"%s 23:59",caEdate);
   	  		  letime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		  sprintf(caTemp+strlen(caTemp)," and vtime<=%d ",letime);
   	  		}
    }
    if(strlen(caDispname)>0){
    	sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caDispname,'%');
    }
    if(strlen(caMac)>0){
    	sprintf(caTemp+strlen(caTemp)," and mac like '%c%s%c' ",'%',caMac,'%');
    }
    if(strlen(caSip)>0){
    	sprintf(caTemp+strlen(caTemp)," and sip like '%c%s%c' ",'%',caSip,'%');
    }
    if(strlen(caIdno)>0){
    	sprintf(caTemp+strlen(caTemp)," and idcard like '%c%s%c' ",'%',caIdno,'%');
    }
    if(strlen(caPosition)>0){
    	sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caPosition,'%');
    }
    if(strlen(caCname)>0){
    	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caCname,'%','%',caCname,'%');
    	
    }
    if(strlen(caKeyword)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (url like '%c%s%c' or ddno like '%c%s%c' or mfrom like '%c%s%c' or fromadress like '%c%s%c' or fromtel like '%c%s%c' or mto like '%c%s%c' or toaddress like '%c%s%c' or totel like '%c%s%c' or email like '%c%s%c'  ) ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    }
    	sprintf(caTemp+strlen(caTemp)," order by sid desc  limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {

    lSid=0;
    lDid=0;
    lUid=0;
    memset(caDispname,0,sizeof(caDispname));
    memset(caIdcard,0,sizeof(caIdcard));
    memset(caMac,0,sizeof(caMac));
    memset(caIdtype,0,sizeof(caIdtype));
    memset(caPosition,0,sizeof(caPosition));
    memset(caSip,0,sizeof(caSip));
    lVtime=0;
    memset(caUrl,0,sizeof(caUrl));
    memset(caDdno,0,sizeof(caDdno));
    memset(caMfrom,0,sizeof(caMfrom));
    memset(caFromadress,0,sizeof(caFromadress));
    memset(caFromtel,0,sizeof(caFromtel));
    memset(caMto,0,sizeof(caMto));
    memset(caToadress,0,sizeof(caToadress));
    memset(caTotel,0,sizeof(caTotel));
    memset(caEmail,0,sizeof(caEmail));
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
                                       UT_TYPE_LONG,4,&lDid,
                                       UT_TYPE_LONG,4,&lUid,
                                       UT_TYPE_STRING,31,caDispname,
                                       UT_TYPE_STRING,19,caIdcard,
                                       UT_TYPE_STRING,19,caMac,
                                       UT_TYPE_STRING,4,caIdtype,
                                       UT_TYPE_STRING,30,caPosition,
                                       UT_TYPE_STRING,15,caSip,
                                       UT_TYPE_LONG,4,&lVtime,
                                       UT_TYPE_STRING,127,caUrl,
                                       UT_TYPE_STRING,20,caDdno,
                                       UT_TYPE_STRING,63,caMfrom,
                                       UT_TYPE_STRING,127,caFromadress,
                                       UT_TYPE_STRING,19,caFromtel,
                                       UT_TYPE_STRING,63,caMto,
                                       UT_TYPE_STRING,127,caToadress,
                                       UT_TYPE_STRING,19,caTotel,
                                       UT_TYPE_STRING,63,caEmail);
                                     
                                    
      	                             
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",lDid);
        utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);

        
         utPltSetCvtHtml(1);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
        utPltPutLoopVar(psDbHead,"idcard",iNum,caIdcard);
        utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
        utPltPutLoopVar(psDbHead,"sip",iNum,caSip);
        utPltPutLoopVar(psDbHead,"mac",iNum,caMac);

        utPltPutLoopVar(psDbHead,"ddno",iNum,caDdno);
        utPltPutLoopVar(psDbHead,"mfrom",iNum,caMfrom);
        utPltPutLoopVar(psDbHead,"fromadress",iNum,caFromadress);
        utPltPutLoopVar(psDbHead,"fromtel",iNum,caFromtel);
        utPltPutLoopVar(psDbHead,"mto",iNum,caMto);
      
        utPltPutLoopVar(psDbHead,"url",iNum,caUrl);
        utPltSetCvtHtml(0);
         psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lDid);           
         if(psClient){
         	utPltPutLoopVar(psDbHead,"servicecode",iNum,psClient->username);
         	utPltPutLoopVar(psDbHead,"cname",iNum,psClient->dispname);
         	strcpy(corpname,psClient->dispname);
         }
        else{
         	strcpy(corpname,"");
        }
        utPltPutLoopVar(psDbHead,"idtype",iNum,ncsGetIdnoNameByType(caIdtype));
        
        utPltPutLoopVar(psDbHead,"vtime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lVtime));
        utPltPutLoopVar(psDbHead,"toaddress",iNum,caToadress);
        
        utPltPutLoopVar(psDbHead,"totel",iNum,caTotel);
        utPltPutLoopVar(psDbHead,"email",iNum,caEmail);
        
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        
           if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,14,					                
					                UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,caDispname,
                          UT_TYPE_STRING,caSip,
                          UT_TYPE_STRING,utTimFormat("%Y-%m-%d %H:%M:%S",lVtime),
                          UT_TYPE_STRING,caMac,
                          UT_TYPE_STRING,caUrl,
                          UT_TYPE_STRING,caDdno,
                          UT_TYPE_STRING,caMfrom,
                          UT_TYPE_STRING,caFromadress,
                          UT_TYPE_STRING,caFromtel,
                          UT_TYPE_STRING,caMto,
                          UT_TYPE_STRING,caToadress,
                          UT_TYPE_STRING,caTotel,
                          UT_TYPE_STRING,caEmail);

                         
                         
                         
                                                  
				 }

			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
 
	  
	  if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }
//	utPltShowDb(psDbHead);  
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/search/ncs_logistlog_list.htm");
	  return 0;
}




//显示邮件
int ncsShowOneMail_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    mail_header_info *psMailHead;
    char *tmpgbk=NULL;
    mail_body *psMailBody;
    mail_attach   *psAtt;
    char      caTemp[6000],caContent[6000];
    char      *pTempDir,*pMsgFile;
    char       *pSelect;
    char      caTempDir[128],caFile[128],caFile2[128],caFile1[128];
    char      caMsgFile[128],caUserid[16];
    char      caCode[16],caCvt[16];
    char      caLid[16];
    char      caMsid[16],caMailfrom[64],caMailto[64],caMailsub[128],caSdate[20];
    int i,iNum,iFlags,iAttFlags,fd;
    char caSid[24];
    utPltDbHead *psDbHead;
    char caTable[32],caStime[16];
   	int iReturn,lCode;
   	char caOut[10204];
    int iRet;
    char caMailfile[128];
    long long lMfile;
    lCode=0;
utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead,5,
                    "table",       UT_TYPE_STRING, 30, caTable,
                    "stime",      UT_TYPE_STRING, 12, caStime,
	                 "mailfile",     UT_TYPE_STRING, 31, caMsgFile,
	                  "ccode",       UT_TYPE_STRING, 12, caCode,
	                  "sid",         UT_TYPE_STRING,20,caSid);
	    lCode = atol(caCode);       
   sprintf(caMailfile,"/home/ncmysql/ncs/log/tmp");
   if(!utFileIsExist(caMailfile)){
   	system("mkdir /home/ncmysql/ncs/log/tmp ");
  }
  sscanf(caMsgFile,"%llu",&lMfile);
  sprintf(caMailfile,"/home/ncmysql/ncs/log/tmp/%llu",lMfile);
  iReturn=ncsUtlCvtNcsText2File(caTable,caMailfile,lMfile);
//printf("iReturn=%d\n",iReturn);
				//		printf("11pid=%d\n",getpid());
				//			sleep(20);

  if(iReturn==1){
  		psMailHead = mail_header(caMailfile,&iFlags);
  		sprintf(caTempDir,"/home/ncmysql/ncs/log/tmp/%lu",utStrGetId());
  		 if(utFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
		    mailHeadFree(psMailHead);
            utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_close.htm",ncsLang("0214读邮件"),ncsLang("0515 生成用户目录错误"));
            return 0;
        }
      }
        sprintf(caFile1,"%s/%llu",caTempDir,lMfile);
        strcpy(caFile2,caMailfile);

        psMailBody = mail_getbody(caFile2,caTempDir,caFile1,"/pronline/Msg?FunName@ncsWebMailCtlDownload&fname@",&iFlags);
		    if(psMailBody == NULL) {
		        mailHeadFree(psMailHead);
		        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_close.htm",ncsLang("0214读邮件"),ncsLang("0808邮件头不正确"));
		        return 0;
		    }
		    
		  
           	/* 生成邮件头  */
		    psDbHead=utPltInitDbHead();
    utPltPutVar(psDbHead,"mailfile",caMsgFile);
    utPltPutVar(psDbHead,"table",caTable);
    
		    if(psMailHead->personF) {
		        utPltPutLoopVar(psDbHead,"fromp", iNum,pasCvtGBK(lCode,psMailHead->personF,caTemp,128));
		    }
		    if(psMailHead->addressF) {
		        utPltPutVar(psDbHead,"from",psMailHead->addressF);
		    }
		
		    if(psMailHead->addressT) {
		        utPltPutVar(psDbHead,"to",psMailHead->addressT);
		    }
		    if(psMailHead->personT) {
		        utPltPutVar(psDbHead,"top",psMailHead->personT);
		    }
		    if(psMailHead->addressCc) {
		        utPltPutVar(psDbHead,"cc",psMailHead->addressCc);
		    }
		    if(psMailHead->personCc) {
		        utPltPutVar(psDbHead,"ccp",psMailHead->personCc);
		    }
		    if(psMailHead->addressBcc) {
		        utPltPutVar(psDbHead,"bcc",psMailHead->addressBcc);
		    }
		    if(psMailHead->subject) {
		    	if(lCode==1){
		    		utPltPutVar(psDbHead,"sub",(char *)pasCvtGBK(lCode,psMailHead->subject,caTemp,255));
		    	}
		    	else{
		         tmpgbk = my_convert(psMailHead->subject);
			       utPltPutVar(psDbHead, "sub", tmpgbk ? tmpgbk : " ");/////////
				    free(tmpgbk);
			    }
		      
		    }
        utPltPutVar(psDbHead,"msgfile",caMsgFile);
		    if(psMailHead->date) {
		        utPltPutVar(psDbHead,"date",psMailHead->date);
		    }
		    /* 正文  */
	printf("type=%d\n",psMailBody->contype);	  
		
		    if(psMailBody->contype == 0) { /* 文本  */
		        sprintf(caFile,"%s/%s.txt",caTempDir,caMsgFile);
		        fd = open(caFile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
					if(fd == (-1)) {
			            utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_close.htm",ncsLang("0214读邮件"),ncsLang("0648文件生成错"));
					    return 0;
					}
					if(psMailBody->textcon) {
						long lSize;
printf("textcon=%s\n",psMailBody->textcon);					
		    	if(lCode==1){
		    		ncMilAddBr(fd,pasCvtGBK(lCode,psMailBody->textcon,caTemp,1000));
		    	
		    	}
		    	else{
					  tmpgbk = my_convert(psMailBody->textcon);		
		//			  printf("tmpgbk=%s\n",tmpgbk);
					 if(tmpgbk){
					 	lSize=strlen(tmpgbk)+100;
					 	ncMilAddBr(fd,tmpgbk);
					 	free(tmpgbk);
					}
					else{
	//					ncMilAddBr(fd,"");
					}
				}
		/*				
			            long lSize;
			            char *pTemp;
					    lSize = strlen(psMailBody->textcon)+100;
					    pTemp = malloc(lSize);
					    ncMilAddBr(fd,pasCvtGBK(lCode,psMailBody->textcon,pTemp,lSize-1));
			
					    free(pTemp);
			       	
		*/
		      }
			       	close(fd);
			        utPltPutVar(psDbHead,"type","text/plain");
			        utPltPutVar(psDbHead,"dfile","file.txt");
			    }
			    else {  /* html */
			        sprintf(caFile,"%s/%s.htm",caTempDir,caMsgFile);
			        fd = open(caFile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
					if(fd == (-1)) {
			            utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_close.htm",ncsLang("0214读邮件"),ncsLang("0648文件生成错"));
					    return 0;
					}
					if(psMailBody->htmlcon) {
			            long lSize;
			            char *pTemp;
//printf("html=%s\n",psMailBody->htmlcon);
					
		       tmpgbk = my_convert(psMailBody->htmlcon);	
		//       printf("sssstmp=%s\n",tmpgbk);	
					 if(tmpgbk){
	//				 	lSize=strlen(tmpgbk)+100;
					 	write(fd, tmpgbk,strlen(tmpgbk));
					 	free(tmpgbk);
						}
						else{
			//				ncMilAddBr(fd,"");
						}
		            
		            
			  /*          
					    lSize = strlen(psMailBody->htmlcon)+100;
					    pTemp = malloc(lSize);
			
					    pasCvtGBK(lCode,psMailBody->htmlcon,pTemp,lSize-1);
			       	    write(fd, pTemp,strlen(pTemp));
			       	    free(pTemp);
			   */    	    
			       	}
			   
			       	close(fd);
			        utPltPutVar(psDbHead,"type","text/html");
			        utPltPutVar(psDbHead,"dfile","file.htm");
	        
	        
	
	        
	    }
	      utPltPutVar(psDbHead,"fname",caFile);
	        if(psMailBody->attach) {
	        psAtt = psMailBody->attach;
	        iNum = 0;
		        while(psAtt) {
		            if(strcasecmp(psAtt->content_type,"application/ms-tnef") == 0) {
		                /* Following is add by Liyunming 2004/07/01 
		                   Read the winmail.dat格式 邮件                     */
		                struct WinMailLst_s {
		                    char                *name;
		                    char                *dispname;
		                    unsigned            long lSize;
		                    struct WinMailLst_s   *next;
		                } *ps,*ps0;
		                iReturn = ncMilExpWinmail(psAtt->filename,caTempDir,&ps);
		                if(iReturn < 0) {
		                    iNum++;
		                    
		                    iRet=pasCharIsUtf8(psAtt->displayname, strlen(psAtt->displayname));
		                    if(iRet){
		                    	pasUtf8ToGBK(psAtt->displayname,caOut,10000);
		                    	utPltPutLoopVar(psDbHead,"disp",iNum,caOut);
		                    }
		                    else{
		                    	utPltPutLoopVar(psDbHead,"disp",iNum,psAtt->displayname);
		 //                   utPltPutLoopVar(psDbHead,"disp",iNum,pasCvtGBK(lCode,psAtt->displayname,caTemp,255));
		                  }
		                    
		                    
		                    
		 //                  utPltPutLoopVar(psDbHead,"disp",iNum,pasCvtGBK(lCode,psAtt->displayname,caTemp,255));
		                    utPltPutLoopVar(psDbHead,"name",iNum,psAtt->filename);
		                    utPltPutLoopVarF(psDbHead,"fsize",iNum,"%lu",psAtt->filesize);
		                    utPltPutLoopVar(psDbHead,"type",iNum,psAtt->content_type);
		                }
		                else {
		                    ps0 = ps;
		                    while(ps) {
		                        iNum++;
		                     iRet=pasCharIsUtf8(ps->name, strlen(ps->name));
		                     if(iRet){
		                    	pasUtf8ToGBK(ps->name,caOut,10000);
		                    	utPltPutLoopVar(psDbHead,"disp",iNum,caOut);
		                     }
		                    else{
		                    	utPltPutLoopVar(psDbHead,"disp",iNum,ps->name);
		 //                   utPltPutLoopVar(psDbHead,"disp",iNum,pasCvtGBK(lCode,psAtt->displayname,caTemp,255));
		                    }
		                        
		        //                utPltPutLoopVar(psDbHead,"disp",iNum,pasCvtGBK(lCode,ps->dispname,caTemp,255));
		                        utPltPutLoopVar(psDbHead,"name",iNum,ps->name);
		                        utPltPutLoopVarF(psDbHead,"fsize",iNum,"%lu",ps->lSize);
		                        utPltPutLoopVar(psDbHead,"type",iNum,"\0");
		                        ps = ps->next;
		                    }        
		                    freewinmaillist(ps0);
		                }
		            }
		            else {
		                iNum++;
		                
		                 iRet=pasCharIsUtf8(psAtt->displayname, strlen(psAtt->displayname));
		                    if(iRet){
		                    	pasUtf8ToGBK(psAtt->displayname,caOut,10000);
		                    	utPltPutLoopVar(psDbHead,"disp",iNum,caOut);
		                    }
		                    else{
		                    	utPltPutLoopVar(psDbHead,"disp",iNum,psAtt->displayname);
		 //                   utPltPutLoopVar(psDbHead,"disp",iNum,pasCvtGBK(lCode,psAtt->displayname,caTemp,255));
		                  }
		                
		//                utPltPutLoopVar(psDbHead,"disp",iNum,pasCvtGBK(lCode,psAtt->displayname,caTemp,255));
		                utPltPutLoopVar(psDbHead,"name",iNum,psAtt->filename);
		                utPltPutLoopVarF(psDbHead,"fsize",iNum,"%lu",psAtt->filesize);
		                utPltPutLoopVar(psDbHead,"type",iNum,psAtt->content_type);
		            }
		            psAtt = psAtt->next;
		        }
		    }
	      pSelect= (char *)pasUtlCvtInputSelect("ccode",caCode,(char *)ncsLang("0926简体,0;繁体,1;UTF8,2;日文,3;"));
		    utPltPutVar(psDbHead,"ccode", pSelect);
		    free(pSelect);
	      mailHeadFree(psMailHead);
        freeBody(psMailBody);
	      utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/ncs_mailmonread_form.htm");
        return 0;
	    
	    
  }
  else{
  	 psDbHead=utPltInitDbHead();
  	 long lStime;
  	 if(strlen(caSid)>0){
  	 sprintf(caTemp,"select mailfrom,mailto,mailsub,stime,ccode from %s where sid=%s ",caTable,caSid);
  	}
  	else{
  		  	 sprintf(caTemp,"select mailfrom,mailto,mailsub,stime,ccode from %s where fileid=%llu ",caTable,lMfile);
  	}
  	 printf("caTemp=%s\n",caTemp);
  	 pasDbOneRecord(caTemp,0,UT_TYPE_STRING,200,caMailfrom,
  	                         UT_TYPE_STRING,200,caMailto,
  	                         UT_TYPE_STRING,200,caMailsub,
  	                         UT_TYPE_LONG,4,&lStime,
  	                         UT_TYPE_LONG,4,&lCode);
  	 utPltPutVar(psDbHead,"from",caMailfrom);
  	 utPltPutVar(psDbHead,"to",caMailto);
  	 
  	  tmpgbk = my_convert(caMailsub);
  	 
			utPltPutVar(psDbHead, "sub",tmpgbk ? tmpgbk : " ");/////////
				free(tmpgbk);	
  	 
 // 	 utPltPutVar(psDbHead,"sub",pasCvtGBK(lCode,caMailsub,caTemp,255));
  	 utPltPutVar(psDbHead,"date",utTimFormat("%Y-%m-%d %H:%M:%S",lStime));
  	utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/ncs_mailmonread_form_db.htm");
  }






   	

}



int ncsWebMailCtlOutHtm_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    
    char caFile[128];
    int i,iReturn;
    char caTemp[256];
    iReturn = utMsgGetSomeNVar(psMsgHead,1,
                    "fname",      UT_TYPE_STRING, 127, caFile);
       printf("caFile=%s\n",caFile);
    sprintf(caTemp,"/home/ncmysql/ncs/log/tmp");
    if(strstr(caFile,caTemp) == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_close.htm",ncsLang("0214读邮件"),ncsLang("0645文件不正确"));
        return 0;
    }       
    utPltHtmlFileOut(iFd,psMsgHead,caFile);
    return 0;
}



/* 读一个表单文件  */
int ncsWebPostRead_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    ncsClient * psClient;
    ncPostHead  *psHead;
    pasFile     *ps;
    ncPostCont *psCont;
    char       *pMailDir;
    char      caTempDir[128],caFile[128],caDfile[128],caSfile[128];
    char      caMsgFile[128],caUserid[16],caDid[16],caSid[16];
    char      caTime[32],caDispname[32],caGroupname[32];
    int i,iNum,i1,i2;
    unsigned long lUserid,lTime;
    char caTemp[1024];
    utPltDbHead *psDbHead;
    int iReturn;
    char      caLid[16];
    char    caStartTime[16];
    char caOut[10204];
    char caTable[32];
    long long lMfile;
    long lDid;
    int iRet;
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif   	
    iReturn = utMsgGetSomeNVar(psMsgHead,3,
                    "table",      UT_TYPE_STRING, 31, caTable,
	                  "formfile",     UT_TYPE_STRING, 31, caMsgFile,
	                  "stime",       UT_TYPE_STRING,10,caStartTime);
	printf("caTable=%s,caMsgFile=%s\n",caTable,caMsgFile);                

 
  	strcpy(caTime,utTimFormat("%Y-%m-%d %H:%M:%S",atol(caStartTime)));
  	
  
  
 	/* 读邮件头信息  */
       sprintf(caFile,"/home/ncmysql/ncs/log/tmp/%s",caMsgFile);

       sprintf(caTempDir,"/home/ncmysql/ncs/log/tmp/%08d",utStrGetId());
    

    if(utFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            utSysLog(" Dir Mkerror %s",caTempDir);
            return 0;
        }
    }
    
    sscanf(caMsgFile,"%llu",&lMfile);
  iReturn=ncsUtlCvtNcsText2File(caTable,caFile,lMfile);
  printf("iReturn=%d\n",iReturn);
  if(iReturn==1){
    
    
    ps = pasOpenFile(caFile);
    if(ps == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_close.htm",ncsLang("0127表单"),ncsLang("0131表单文件打开出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
    
    sprintf(caTemp,"select dispname,did from %s where fileid=%llu ",caTable,lMfile);

    memset(caDispname,0,sizeof(caDispname));
   lDid=0;
    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,30,caDispname,
    			                  UT_TYPE_LONG,4,&lDid);

    psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lDid);           
    if(psClient){
         	utPltPutVar(psDbHead,"groupname",psClient->dispname);
        
     }



    utPltPutVar(psDbHead,"username",caDispname);
    utPltPutVar(psDbHead,"groupname",caGroupname);
    utPltPutVar(psDbHead,"date",caTime);
    psHead = ncPostGetPostData(ps);
    iNum = 0;
    while(psHead) {
        iNum++;
        utPltPutLoopVar(psDbHead,"host",iNum,psHead->pHost?psHead->pHost:"\0");
        utPltPutLoopVar(psDbHead,"url",iNum,psHead->pUrl?psHead->pUrl:"\0");
        psCont = psHead->psCont;
        i1 = 0;
        i2 = 0;
        while(psCont) {
            if(psCont->lFileId > 0) {  /* 附件  */
                i1++;
                sprintf(caDfile,"%s/%lu",caTempDir,psCont->lFileId);
                link(ncPostFileName(psCont->lFileId,caSfile),caDfile);
                utPltPutLoopVar2(psDbHead,"fvar",iNum,i1,psCont->pVar?psCont->pVar:"\0");
 //               utPltPutLoopVar2(psDbHead,"fvalue",iNum,i1,psCont->pValue?psCont->pValue:"\0");
                   if(psCont->pValue){
                    	  iRet=pasCharIsUtf8(psCont->pValue, strlen(psCont->pValue));
                    	      if(iRet){
			                    	pasUtf8ToGBK(psCont->pValue,caOut,10000);
			                    	utPltPutLoopVar2(psDbHead,"fvalue",iNum,i1,caOut);
			                    }
                    	     else{
                    	     	utPltPutLoopVar2(psDbHead,"fvalue",iNum,i1,psCont->pValue);
                    	    }
                    } 
 
 
                if(psCont->pValue) {
                    for(i=strlen(psCont->pValue)-1;
                       psCont->pValue[i]!=92 && psCont->pValue[i]!='/' && i>=0;i--);
   //                 utPltPutLoopVar2(psDbHead,"disp",iNum,i1,psCont->pValue + i + 1);
                           iRet=pasCharIsUtf8(psCont->pValue+ i + 1, strlen(psCont->pValue+ i + 1));
                        if(iRet){
                        	pasUtf8ToGBK(psCont->pValue+ i + 1,caOut,10000);
			                    	utPltPutLoopVar2(psDbHead,"disp",iNum,i1,caOut);
                        }
                        else{
                         utPltPutLoopVar2(psDbHead,"disp",iNum,i1,psCont->pValue + i + 1);
                        }
   
   
                }
                utPltPutLoopVar2(psDbHead,"ftype", iNum,i1,psCont->pContentType?psCont->pContentType:"\0");
                utPltPutLoopVar2(psDbHead,"fname", iNum,i1,caDfile);
            }
            else {
                if(psCont->pValue && !utStrIsSpaces(psCont->pValue)) {
                    i2++;
                    utPltPutLoopVar2(psDbHead,"vvar",iNum,i2,psCont->pVar?psCont->pVar:"\0");
 //                   utPltPutLoopVar2(psDbHead,"vvalue",iNum,i2,psCont->pValue?psCont->pValue:"\0");
                   	  if(psCont->pValue){
                    	    iRet=pasCharIsUtf8(psCont->pValue, strlen(psCont->pValue));
                    	      if(iRet){
			                    	pasUtf8ToGBK(psCont->pValue,caOut,10000);
			                    	utPltPutLoopVar2(psDbHead,"vvalue",iNum,i2,caOut);
			                    }
                    	     else{
                    	     	utPltPutLoopVar2(psDbHead,"vvalue",iNum,i2,psCont->pValue);
                    	    }
                    }
 
                }                
            }
            psCont = psCont->next;
        }
        ncPostFreeHttpHead(psHead);
        psHead = ncPostGetPostData(ps);
    }
    pasCloseFile(ps);
  }
   	utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/ncs_postcontent.htm");
   	return 0;
}

int ncsMailCtlDownload_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    
    char caFile[128],caDispname[64],caType[32];
    char caTemp[256];
    int i,iReturn;
    char *pFile;
utMsgPrintMsg(psMsgHead);    
    iReturn = utMsgGetSomeNVar(psMsgHead,3,
                    "fname",      UT_TYPE_STRING, 127, caFile,
	                "disp",       UT_TYPE_STRING, 63,  caDispname,
	                "type",       UT_TYPE_STRING, 63,  caType);    

    sprintf(caTemp,"%s/tmp","/home/ncmysql/ncs/log");
    if(strstr(caFile,caTemp) == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_close.htm",ncsLang("0214读邮件"),ncsLang("0645文件不正确"));
        return 0;
    }       
    for(i=strlen(caFile)-1;caFile[i]!='/' && i > 0;i--);
    if(i>0) {
        pFile = caFile + i + 1;
        caFile[i] = 0;
    }
    else {
        pFile = caFile;
    }
    if(utStrIsSpaces(caDispname)) {
        strcpy(caDispname,pFile);
    }
    return utPltFileDownload(iFd,caType,caFile,pFile,caDispname);
}

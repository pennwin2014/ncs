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
#include "ncpost.h"
extern char *pHashNettype;

//根据厂商权限,获取厂商代码
char *getDsFacs(){
        static char caReturn[1024];
        char caIds[1024],caTempid[1024];
        char caOpt[256];
        int iReturn,lId,iNum;
         pasDbCursor *psCur;
         char caObj[100][50];
         long i;
   long num=0;
         memset(caReturn,0,sizeof(caReturn));


    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    if(iReturn!=0||lId<=0){
         return &caReturn[0];
    }

        memset(caOpt,0,sizeof(caOpt));
            pasDbOneRecord("select fac from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
            if(strstr(caOpt,"all")||strlen(caOpt)==0){
                return &caReturn[0];
    }

    num=ncs_SepChar0(caOpt,',',caObj);
    for(i=0;i<num;i++){
    if(i==0)
        sprintf(caReturn,"\'%s\'",caObj[0]);
        else{
           sprintf(caReturn+strlen(caReturn),",\'%s\'",caObj[i]);
        }
     }

                 return &caReturn[0];
 }


int ncsSysloglist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caStemptime[17];
    char caEtemptime[17];
    char caUserid[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caSdate[32];
    char caTemp[1024];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime,l,lTime1,lSip,lDip;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
   	long lRowNum,lflags,lStartRec,lCount,lSid;
	int     lSumUser;
	char sqlbuf[2048]="";

 	pasDbCursor *psCur;
 

  char caKeyword[64],caTime_flag[16];
  char caPage[12],caLimit[16],caSort[16],caDir[16];

  char caName[36],caSdate_d[20],caEdate_d[20],caEtime[16];

 char caIpaddr[20],caAction[12],caOpname[24],caResult[12],caDescr[1024];
  char caTemp_d[68];
  lCurPg=1;
 	strcpy(caWhere,"\0");
	
	iReturn = utMsgGetSomeNVar(psMsgHead,15,
	    "TotPg",      UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      UT_TYPE_STRING, 12, caCurPg,
			"ipaddr",UT_TYPE_STRING,15,caIpaddr,
			"sdate",UT_TYPE_STRING,19,caSdate,
			"edate",UT_TYPE_STRING,19,caEdate,
			"action",UT_TYPE_STRING,6,caAction,
			"opname",     UT_TYPE_STRING,20,caOpname,
			"keyword", UT_TYPE_STRING,60,caKeyword,
			"result",UT_TYPE_STRING,8,caResult,
   	  "descr",     UT_TYPE_STRING,60,caDescr,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag);
   	
    
      if(strlen(caDescr)>0){
      	 strcpy(caTemp_d,pasCvtGBK(2,caDescr,caTemp,60));
    	   strcpy(caDescr,caTemp_d);
      }
      if(strlen(caKeyword)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
      }
      if(strlen(caOpname)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caOpname,caTemp,20));
      	strcpy(caOpname,caTemp_d);
      }
      if(strlen(caResult)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caResult,caTemp,20));
      	strcpy(caResult,caTemp_d);
      }
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
   
   	
  
  
   	                
   
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
			     sprintf(caEtemptime,"%s 23:59",caEdate_d,caEtime);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	  }
   

 
   utStrDelSpaces(caKeyword);           
   utStrDelSpaces(caResult);  
   utStrDelSpaces(caDescr);           
   utStrDelSpaces(caIpaddr);  
  utStrDelSpaces(caOpname);  
   
   
	 memset(caWhere,0,sizeof(caWhere));
	 
	 strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S",lstime));
	 strcpy(caEdate,utTimFormat("%Y-%m-%d %H:%M:%S",letime));
	 	
	 sprintf(caWhere+strlen(caWhere)," and sdate>='%s' ",caSdate);
	 sprintf(caWhere+strlen(caWhere)," and sdate<='%s' ",caEdate);
	 		if(strlen(caOpname)>0){
		sprintf(caWhere+strlen(caWhere)," and name like '%c%s%c' ",'%',caOpname,'%');
	}
	if(strlen(caIpaddr)>0){
		sprintf(caWhere+strlen(caWhere)," and ipaddr like '%c%s%c' ",'%',caIpaddr,'%');
	}
	if(strlen(caAction)>0){
	  sprintf(caWhere+strlen(caWhere)," and action='%s' ",caAction);
	}
	if(strlen(caResult)>0){
		sprintf(caWhere+strlen(caWhere)," and result='%s' ",caResult);
	}
  if(strlen(caDescr)>0){
		sprintf(caWhere+strlen(caWhere)," and descr like '%c%s%c' ",'%',caDescr,'%');
	} 
	if(strlen(caKeyword)>0){
		sprintf(caWhere+strlen(caWhere)," and (descr like '%c%s%c' or ipaddr like '%c%s%c' or result like '%c%s%c' or name like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
	}
	
	sprintf(sqlbuf,"select count(*) from ncsadminlog where 1=1 ");
	sprintf(sqlbuf+strlen(sqlbuf)," %s ",caWhere);
	lCount=0;
	pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lCount);
  
	sprintf(sqlbuf,"select sid,sdate,name,ipaddr,descr,action,result from ncsadminlog where 1=1 ");
	strcat(sqlbuf,caWhere);
	sprintf(sqlbuf+strlen(sqlbuf)," order by %s %s limit %lu,%lu",caSort,caDir,lStartRec,lRowNum);

 printf("sqlbuf=%s\n",sqlbuf);
 psCur = pasDbOpenSql(sqlbuf,0);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
	 	
  
  psDbHead = utPltInitDb();
    
        iReturn = 0;
			   lSid=0;
			    memset(caSdate,0,sizeof(caSdate));
			    memset(caName,0,sizeof(caName));
			    memset(caIpaddr,0,sizeof(caIpaddr));
			    memset(caDescr,0,sizeof(caDescr));
			    memset(caAction,0,sizeof(caAction));
			    memset(caResult,0,sizeof(caResult));
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lSid,
			                                   UT_TYPE_STRING,19,caSdate,
			                                   UT_TYPE_STRING,15, caName,
			                                   UT_TYPE_STRING,15,caIpaddr,
			                                   UT_TYPE_STRING,1000,caDescr,
			                                   UT_TYPE_STRING,4,caAction,
			                                   UT_TYPE_STRING,10,caResult);
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
    	
    	    if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
	        iNum++;
	        
	        utPltPutLoopVarF(psDbHead,"num", iNum,"%lu",iNum);
          utPltPutLoopVarF(psDbHead,"sid", iNum,"%lu",lSid);
          utPltPutLoopVar(psDbHead,"sdate", iNum,caSdate);
	        utPltSetCvtHtml(1);
          utPltPutLoopVar(psDbHead,"desc", iNum,caDescr);
          utPltPutLoopVar(psDbHead,"action",iNum,ncsGetActionNameByCode(caAction));
          utPltPutLoopVar(psDbHead,"name",  iNum,caName);
          utPltPutLoopVar(psDbHead,"ipaddr", iNum,caIpaddr);
	       utPltPutLoopVar(psDbHead,"result",iNum,caResult);  
	        utPltSetCvtHtml(0);
	      
         
      
     
			   lSid=0;
			    memset(caSdate,0,sizeof(caSdate));
			    memset(caName,0,sizeof(caName));
			    memset(caIpaddr,0,sizeof(caIpaddr));
			    memset(caDescr,0,sizeof(caDescr));
			    memset(caAction,0,sizeof(caAction));
			    memset(caResult,0,sizeof(caResult));
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lSid,
			                                   UT_TYPE_STRING,19,caSdate,
			                                   UT_TYPE_STRING,15, caName,
			                                   UT_TYPE_STRING,15,caIpaddr,
			                                   UT_TYPE_STRING,1000,caDescr,
			                                   UT_TYPE_STRING,4,caAction,
			                                   UT_TYPE_STRING,10,caResult);
    }



    lTotPg = (lCount -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lCount);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        
//     utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/system/ncs_adminlog_list.htm");
    return 0;
}

extern char*pHashEndgroup;
//上网用户
int ncsEndUserlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caStemptime[17];
    char caEtemptime[17];
    char caUserid[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caSdate[32];
    char caTemp[4024];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime,l,lTime1,lSip,lDip;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
   	long lRowNum,lflags,lStartRec,lCount,lSid;
	int     lSumUser;
	 char sqlbuf[2048]="";
 	  struct userdate{
 	  	unsigned long uid;
 	  	unsigned long did;
 	  	char dispname[32];
 	  	char username[16];
 	  	char ip[16];
 	  	char mac[20];
 	  	char position[32];
 	  	char idno[20];
 	  	char idfyunit[32];
 	  	long lasttime;
 	  	long flag;
 	  	long gid;
 	  	long lIdtype;
 	  };
 	  struct ncsGid_s {
            unsigned long did;
            unsigned long gid;
    };
    struct ncsGid_s  stGid; 
    struct ncsEndGroup_s *psData; 	  
 	  long lCount_n;
 	  struct userdate *pUserdate;
    long lUid,lDid,lFlag,lGid,lLasttime,lIdfytype;
    char caCorpname[64],caUsername[32],caDepname[64],caEmail[128],caGname[64];
 	pasDbCursor *psCur;
  char caKeyword[64],caTime_flag[16];
  char caPage[12],caLimit[16],caSort[16],caDir[16];
  char caName[36],caSdate_d[20],caEdate_d[20],caEtime[16];
  char caIpaddr[20],caAction[12],caOpname[24],caResult[12],caDescr[1024];
  char caDispname[32],caCname[64],caMac[20],caIdcard[20],caIp[20],caAddr[128],caPosition[32],caExport[16],caField_flag[16];
  char caTemp_d[68];
  char caServicecode[16],caGroupid[16];
  char caGroups[1024];
  char caFacs[1024];
	strcpy(caFacs,getDsFacDid());
  strcpy(caGroups,getDsGroupid());
  lCurPg=1;
 	strcpy(caWhere,"\0");
	
	iReturn = utMsgGetSomeNVar(psMsgHead,19,
	    "TotPg",      UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      UT_TYPE_STRING, 12, caCurPg,
			"uname",UT_TYPE_STRING,31,caDispname,
			"cname",UT_TYPE_STRING,31,caCname,
			"mac", UT_TYPE_STRING,20,caMac,
			"idno",UT_TYPE_STRING,19,caIdcard,
			"ip",UT_TYPE_STRING,15,caIp,
			"idfyunit",UT_TYPE_STRING,30,caAddr,
			"position",UT_TYPE_STRING,30,caPosition,
			"keyword",UT_TYPE_STRING,60,caKeyword,
			"did",UT_TYPE_STRING,10,caUserid,
			"groupid",UT_TYPE_STRING,10,caGroupid,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "exp",     UT_TYPE_STRING,10,caExport,
   	   "field_flag",   UT_TYPE_STRING,8,caField_flag,
   	   "servicecode",  UT_TYPE_STRING,14,caServicecode);
   	  if(strlen(caServicecode)>0){
   	  	strcpy(caTemp_d,pasCvtGBK(2,caServicecode,caTemp,14));
   	  	strcpy(caServicecode,caTemp_d);
   	  }
    
      if(strlen(caDispname)>0){
      	 strcpy(caTemp_d,pasCvtGBK(2,caDispname,caTemp,30));
    	   strcpy(caDispname,caTemp_d);
      }
      if(strlen(caKeyword)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
      }
      if(strlen(caCname)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caCname,caTemp,60));
      	strcpy(caCname,caTemp_d);
      }
      if(strlen(caAddr)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caAddr,caTemp,30));
      	strcpy(caAddr,caTemp_d);
      }
      if(strlen(caPosition)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caPosition,caTemp,30));
      	strcpy(caPosition,caTemp_d);
      }
      
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
   
    if(!utStrIsSpaces(caExport)){
    	lRowNum=9999;
    }
    pUserdate=(struct userdate*)malloc((lRowNum+1)*sizeof(struct userdate));
    if(pUserdate==NULL){
  	    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;
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
			     sprintf(caEtemptime,"%s 23:59",caEdate_d,caEtime);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	  }
   

 
   utStrDelSpaces(caKeyword);           
   utStrDelSpaces(caDispname);  
   utStrDelSpaces(caCname);           
   utStrDelSpaces(caAddr);  
  utStrDelSpaces(caPosition);  
   
   
	 memset(caWhere,0,sizeof(caWhere));
	 

	if(strlen(caDispname)!=0)
	sprintf(caWhere+strlen(caWhere)," and (a.username like '%c%s%c' or a.dispname like '%c%s%c') ",'%',caDispname,'%','%',caDispname,'%');
	
	if((strlen(caCname)>0)&&(strlen(caUserid)==0)){
		sprintf(caWhere+strlen(caWhere)," and (c.username like '%c%s%c' or c.dispname like '%c%s%c') ",'%',caCname,'%','%',caCname,'%');
	}
	if((strlen(caServicecode)>0)&&(strlen(caUserid)==0)){
		sprintf(caWhere+strlen(caWhere)," and (c.username like '%c%s%c' ) ",'%',caServicecode,'%');
	}
	
	
	if(strlen(caIdcard)!=0)
	sprintf(caWhere+strlen(caWhere)," and a.idcard like '%c%s%c' ",'%',caIdcard,'%');
	
	if(strlen(caUserid)!=0)
	sprintf(caWhere+strlen(caWhere)," and a.did=%s ",caUserid);
	
	if(strlen(caIp)!=0)
	sprintf(caWhere+strlen(caWhere)," and inet_ntoa(a.ip)='%s' ",caIp);

  if(strlen(caMac)!=0)
  sprintf(caWhere+strlen(caWhere)," and a.mac='%s' ",caMac);
  
  if(strlen(caPosition)!=0)
  sprintf(caWhere+strlen(caWhere)," and a.position='%s' ",caPosition);
  
  if(strlen(caAddr)>0){
  	sprintf(caWhere+strlen(caWhere)," and a.idfyunit like '%c%s%c' ",'%',caAddr,'%');
  }
	if(strlen(caGroupid)>0){
		sprintf(caWhere+strlen(caWhere)," and a.did in (select userid from ncsuser where groupid in (%s) ) ",getGroupid(atol(caGroupid )));
	}
	if(strlen(caGroups)>0){
		sprintf(caWhere+strlen(caWhere)," and a.did in (select userid from ncsuser where groupid in (%s) ) ",caGroups);
	}
	if(strlen(caFacs)>0){
			sprintf(caWhere+strlen(caWhere)," and a.did in (%s) ",caFacs);
	}
	
	
	if(strlen(caKeyword)>0){
		if(atol(caField_flag)==1){
			sprintf(caWhere+strlen(caWhere)," and (a.username like '%c%s%c' or a.dispname like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
		}
		else if(atol(caField_flag)==2){
			sprintf(caWhere+strlen(caWhere)," and a.idcard like '%c%s%c' ",'%',caKeyword,'%');
		}
		else if(atol(caField_flag)==3){
			sprintf(caWhere+strlen(caWhere)," and a.mac like '%c%s%c' ",'%',caKeyword,'%');
		}
		else if(atol(caField_flag)==4){
			sprintf(caWhere+strlen(caWhere)," and a.position like '%c%s%c' ",'%',caKeyword,'%');
		}
		else if(atol(caField_flag)==5){
			sprintf(caWhere+strlen(caWhere)," and inet_ntoa(a.ip) like '%c%s%c' ",'%',caKeyword,'%');
		}
		else if(atol(caField_flag)==6){
			sprintf(caWhere+strlen(caWhere)," and a.idfyunit like '%c%s%c' ",'%',caKeyword,'%');
		}
		else if(atol(caField_flag)==7){
			sprintf(caWhere+strlen(caWhere)," and (c.dispname like '%c%s%c' or c.username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
	  }
	 }
//	 if(strlen(caWhere)==0){
//	 	sprintf(sqlbuf,"select count(*) from ncsenduser ");
//	}
//	else{
		sprintf(sqlbuf,"select count(*) from ncsenduser a,ncsuser c where a.did=c.userid  ");
		strcat(sqlbuf, caWhere);
//	}
	 
printf("sqlbuf=%s\n",sqlbuf);
	lCount=0;
	pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lCount);
  
  sprintf(sqlbuf,"select a.sid,a.did,a.uid,a.username,a.dispname,a.email,a.idcard,inet_ntoa(a.ip),a.lasttime,c.dispname,a.position,a.mac,a.idfyunit,a.flag,a.gid,a.idfytype from ncsenduser a,ncsuser c  where a.did=c.userid ");
	strcat(sqlbuf,caWhere);
	if(strlen(caSort)==0){
		strcpy(caSort,"a.did");
	}

	sprintf(sqlbuf+strlen(sqlbuf)," order by %s %s limit %lu,%lu",caSort,caDir,lStartRec,lRowNum);

 printf("sqlbuf=%s\n",sqlbuf);
 psCur = pasDbOpenSql(sqlbuf,0);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
	 	   memset(caDispname,0,sizeof(caCorpname));
       memset(caGname,0,sizeof(caUsername));
       memset(caCorpname,0,sizeof(caDispname));
       memset(caDepname,0,sizeof(caDepname));
       memset(caIp,0,sizeof(caIp));
       memset(caEmail,0,sizeof(caEmail));
       memset(caIdcard,0,sizeof(caIdcard));
       memset(caPosition,0,sizeof(caPosition));
       memset(caMac,0,sizeof(caMac));
       memset(caUsername,0,sizeof(caUsername));
        lLasttime=0;
        memset(caAddr,0,sizeof(caAddr));
        lGid=0;
       lFlag=0;
       lIdfytype=0;
   	iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lSid,
   				       UT_TYPE_LONG,4,&lDid,
   				       UT_TYPE_LONG,4,&lUid,
   				       UT_TYPE_STRING,15,caUsername,
   				       UT_TYPE_STRING,31,caDispname,
   				       UT_TYPE_STRING,63,caEmail,
   				       UT_TYPE_STRING,19,caIdcard,
   				       UT_TYPE_STRING,15,caIp,
  	             UT_TYPE_LONG,4,&lLasttime,
  	             UT_TYPE_STRING,63,caCorpname,
  	             UT_TYPE_STRING,31,caPosition,
  	             UT_TYPE_STRING,18,caMac,
  	             UT_TYPE_STRING,30,caAddr,
  	             UT_TYPE_LONG,4,&lFlag,
  	             UT_TYPE_LONG,4,&lGid,
  	             UT_TYPE_LONG,4,&lIdfytype);
	 	
	 	
	 	
	 	
  
  psDbHead = utPltInitDb();
    iNum=0;
  	while(iReturn == 0||iReturn==1405 && iNum < lRowNum) {
       
        pUserdate[iNum].uid=lUid;
        pUserdate[iNum].did=lDid;
        strcpy(pUserdate[iNum].dispname,caDispname);
        strcpy(pUserdate[iNum].ip,caIp);
        strcpy(pUserdate[iNum].mac,caMac);
        strcpy(pUserdate[iNum].position,caPosition);
        pUserdate[iNum].lasttime=lLasttime;
        strcpy(pUserdate[iNum].idno,caIdcard);
        strcpy(pUserdate[iNum].idfyunit,caAddr);
        strcpy(pUserdate[iNum].username,caUsername);
        pUserdate[iNum].flag=lFlag;
        pUserdate[iNum].gid=lGid;
        pUserdate[iNum].lIdtype=lIdfytype;
         iNum++;
   
        memset(caDispname,0,sizeof(caCorpname));
       memset(caGname,0,sizeof(caUsername));
       memset(caCorpname,0,sizeof(caDispname));
       memset(caDepname,0,sizeof(caDepname));
       memset(caIp,0,sizeof(caIp));
       memset(caEmail,0,sizeof(caEmail));
       memset(caIdcard,0,sizeof(caIdcard));
       memset(caPosition,0,sizeof(caPosition));
       memset(caMac,0,sizeof(caMac));
       memset(caUsername,0,sizeof(caUsername));
        lLasttime=0;
        memset(caAddr,0,sizeof(caAddr));
        lGid=0;
       lFlag=0;
       lIdfytype=0;
   	iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lSid,
   				       UT_TYPE_LONG,4,&lDid,
   				       UT_TYPE_LONG,4,&lUid,
   				       UT_TYPE_STRING,15,caUsername,
   				       UT_TYPE_STRING,31,caDispname,
   				       UT_TYPE_STRING,63,caEmail,
   				       UT_TYPE_STRING,19,caIdcard,
   				       UT_TYPE_STRING,15,caIp,
  	             UT_TYPE_LONG,4,&lLasttime,
  	             UT_TYPE_STRING,63,caCorpname,
  	             UT_TYPE_STRING,31,caPosition,
  	             UT_TYPE_STRING,18,caMac,
  	             UT_TYPE_STRING,30,caAddr,
  	             UT_TYPE_LONG,4,&lFlag,
  	             UT_TYPE_LONG,4,&lGid,
  	             UT_TYPE_LONG,4,&lIdfytype);
    }
    pasDbCloseCursor(psCur);
//printf("cccccccccc\n");

    if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"user%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
        
        fprintf(fp,ncsLang("0771用户信息\n"));
      ncUtlPrintCsv(fp,6,UT_TYPE_STRING,ncsLang("0906单位名称"),
                       UT_TYPE_STRING,ncsLang("0762用户名"),
                       UT_TYPE_STRING,ncsLang("0054IP地址"),
                       UT_TYPE_STRING,"MAC地址",
                       UT_TYPE_STRING,ncsLang("0918身份证号"),
                       UT_TYPE_STRING,"房间号位置");
                                      
       for(i=0;i<iNum;i++){
       	stGid.did=pUserdate[i].did;
     	  stGid.gid=pUserdate[i].gid;
     	  strcpy(caCname,ncsUtlGetClientNameById(psShmHead,pUserdate[i].did,"未知"));
        psData = (struct ncsEndGroup_s *)pasLHashLook(pHashEndgroup,&stGid);   
       	if(psData){ 

       		if(strcmp(psData->name,"未知部门")!=0){
       			sprintf(caCname,"%s/%s",ncsUtlGetClientNameById(psShmHead,pUserdate[i].did,"未知"),psData->name);
       	  }
        }
       	
       	
        ncUtlPrintCsv(fp,6,UT_TYPE_STRING,caCname,
                       UT_TYPE_STRING,pUserdate[i].dispname,
                       UT_TYPE_STRING,pUserdate[i].ip,
                       UT_TYPE_STRING,pUserdate[i].mac,
                       UT_TYPE_STRING,pUserdate[i].idno,
                       UT_TYPE_STRING,pUserdate[i].position);
        }
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        free(pUserdate);
        return 0;
    }
    
        for(i=0;i<iNum;i++){
// printf("i=%d\n",i);
        	if(i>0){
        		utPltPutLoopVar(psDbHead,"dh",i+1,",");
        	}
        utPltPutLoopVarF(psDbHead,"num",i+1,"%d",i+1);
//printf("username=%s\n",pUserdate[i].username);
        utPltPutLoopVarF(psDbHead,"did",i+1,"%lu",pUserdate[i].did);
        utPltPutLoopVarF(psDbHead,"uid",i+1,"%lu",pUserdate[i].uid);
         utPltSetCvtHtml(1);  		
        utPltPutLoopVar(psDbHead,"username",i+1,pUserdate[i].username);
        stGid.did=pUserdate[i].did;
     	  stGid.gid=pUserdate[i].gid;

     	  strcpy(caCname,ncsUtlGetClientNameById(psShmHead,pUserdate[i].did,"未知"));

        psData = (struct ncsEndGroup_s *)pasLHashLook(pHashEndgroup,&stGid);   

       	if(psData){ 
           utPltPutLoopVar(psDbHead,"depname",i+1,psData->name);
 //      		if(strcmp(psData->name,"未知部门")!=0){
 //      			sprintf(caCname,"%s/%s",ncsUtlGetClientNameById(psShmHead,pUserdate[i].did,"未知"),psData->name);
 //      	  }
        }
  
        utPltPutLoopVar(psDbHead,"cname",i+1,caCname);
        utPltPutLoopVar(psDbHead,"dispname",i+1,pUserdate[i].dispname);
        utPltPutLoopVar(psDbHead,"idcard",i+1,pUserdate[i].idno);
        utPltPutLoopVar(psDbHead,"ip",i+1,pUserdate[i].ip);
      
        utPltPutLoopVar(psDbHead,"position",i+1,pUserdate[i].position);
        utPltPutLoopVar(psDbHead,"mac",i+1,pUserdate[i].mac);
        utPltPutLoopVar(psDbHead,"addr",i+1,utStrGetCharHan(pUserdate[i].idfyunit,28));
        utPltPutLoopVar(psDbHead,"sdate",i+1,utTimFormat("%Y-%m-%d %H:%M:%S",pUserdate[i].lasttime));
        utPltPutLoopVar(psDbHead,"idtype",i+1,ncsGetICNameById(pUserdate[i].lIdtype));
        
        if(pUserdate[i].flag==1){
        	utPltPutLoopVar(psDbHead,"state",i+1,"<font color=red>重点</font>");
        }
        else{
        	utPltPutLoopVar(psDbHead,"state",i+1,"<font color=green>普通</font>");
        }
        utPltSetCvtHtml(0);  		 
        sprintf(caTemp,"select count(distinct name) from ncsnetid where did=%lu and nid=%lu ",pUserdate[i].did,pUserdate[i].uid);
        lCount_n=0;
        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount_n);
        utPltPutLoopVarF(psDbHead,"countuid",i+1,"%d",lCount_n);
      }



    lTotPg = (lCount -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lCount);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       

    
        
//    utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_enduser_list.htm");
    return 0;
}


//重点监控虚拟身份组树
int ncsTreeSnetid_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[1024];
   	int  iNum;
    char *pHash;
    char sqlbuf[1024];
   	long lFlag,lKid;
    typedef struct ncsKeywordStat_s {
            long id;
            long sum;                //总记录数
            long csum;               //已处理记录数
   } ncsKeywordStat;
   ncsKeywordStat *psData;
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64];
    long lId,lSid,lStatus;
    char caStatus[16];
    char caPlate[128];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,8,
			             "del",           UT_TYPE_STRING,8,caDel,
			             "keyname",       UT_TYPE_STRING,30,caKeyname,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,8,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "del",UT_TYPE_STRING,10,caDel,
			             "plate",UT_TYPE_STRING,127,caPlate);

    if(strlen(caPlate)>0){
    	pHash=(unsigned char *)pasLHashInit(1000,1000,sizeof(struct ncsKeywordStat_s),0,4); 
    	if(pHash==NULL){
    		return -1;
    	}
    	sprintf(sqlbuf,"select count(*),flag,kid from ncssennetidlog group by kid,flag ");

    	psCur=pasDbOpenSql(sqlbuf,0);
    	if(psCur==NULL){
    		free(pHash);
    		return 0;
    	}
    	lCount=0;
    	lFlag=0;
    	lKid=0;
    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lFlag,
                                 UT_TYPE_LONG,4,&lKid);
                       
     while(iReturn==0||iReturn==1405)
     {

     	  psData = (struct ncsKeywordStat_s *)pasLHashLookA(pHash,&lKid);  
     	  if(psData){
     	  	if(lFlag>0){
     	  		psData->csum=psData->csum+lCount;
     	  	}
     	  	psData->sum=psData->sum+lCount;
     	  }
     	  
     	  
    	lCount=0;
    	lFlag=0;
    	lKid=0;
    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lFlag,
                                 UT_TYPE_LONG,4,&lKid);
     }
     pasDbCloseCursor(psCur); 
    	
    	
    }


	  if(strlen(caKeyname)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyname,caTemp,63));
	  	strcpy(caKeyname,caTemp_d);
	  } 
 
 
 	  if(strcmp(caUpdate,"update")==0){
	  	if(strlen(caSid)==0){
	  		sprintf(caTemp,"select count(*) from ncuseridclass where name=Binary('%s') ",caKeyname);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
	  		   lSid = ncsUtlGenId(psShmHead);
	  			 pasDbExecSqlF("insert into ncuseridclass(id,name,modtime,status,flags) values (%lu,'%s',%lu,%lu,8)",
                                             lSid,caKeyname,time(0),atol(caStatus));
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncuseridclass where name=Binary('%s') and id!=%s ",caKeyname,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
	  					  			pasDbExecSqlF("update ncuseridclass set name='%s',modtime=%d,status=%d where id=%s ",caKeyname,time(0),atol(caStatus),caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	   	       iReturn=pasDbExecSqlF("delete from ncuseridlist where id in (%s)",caSelsid);
   	             if(iReturn==0){
                iReturn = pasDbExecSqlF("delete from ncuseridclass where id  in(%s)",caSelsid);
              }

	  	
	  }
 
 
 
 
 
    iNum = 0;
    
     psDbHead = utPltInitDb();

		  	 psCur = pasDbOpenSqlF("select name,id,status from ncuseridclass  order by name limit 0,1000");
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
	     memset(caName,0,sizeof(caName));
	     lId=0;
	     lStatus=0;
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lId,
		                                   UT_TYPE_LONG,4,&lStatus);
		                                
		
		 
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(0); 
		         if(strlen(caPlate)>0){
		         	psData = (struct ncsKeywordStat_s *)pasLHashLook(pHash,&lId);  
			     	  if(psData){
			     	  	if(lStatus==0){
			     	  	    sprintf(caTemp,"<font color=#bbbbbb>%s_(%d/%d)</font>",caName,psData->csum,psData->sum);
			     	  	  }
			     	  	  else{
			     	  	  	 sprintf(caTemp,"%s_(%d/%d)",caName,psData->csum,psData->sum);
			     	  	  }
					    }
					    else{
					    	    sprintf(caTemp,"%s_(%d/%d)",caName,0,0);
					     }
					   }
		         else{
		          
				         if(lStatus==0){
				         	sprintf(caTemp,"<font color=#bbbbbb>%s(%s)</font>",caName,"停用");
				         }
				         else{
				         	
				         	sprintf(caTemp,"%s",caName);
				         }
		         
		        }
		         
		         
		         
		         
		         utPltPutLoopVar(psDbHead,"name", iNum,caName);
		         utPltPutLoopVar(psDbHead,"groupname", iNum,caTemp);
		         
	
		         
		         
		         
		         
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%ld",lId);
		         utPltPutLoopVarF(psDbHead,"status",iNum,"%d",lStatus);
		         utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
		         utPltPutLoopVar(psDbHead,"cls", iNum,"file");
		         	
				     memset(caName,0,sizeof(caName));
				     lId=0;
				     lStatus=0;
					   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
					                                   UT_TYPE_LONG,4,&lId,
					                                   UT_TYPE_LONG,4,&lStatus);
		    }
		    pasDbCloseCursor(psCur);
		
    
    	
    	
 //utPltShowDb(psDbHead);
        if(strlen(caPlate)>0){
        	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
        	free(pHash);
        }
        else{
    	   utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_keyword_checktree.htm");
        }

    return 0;
}



//显示虚拟身份组明细
int ncsSentive_NetidDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  char caContent[64],caKtype[8],caCaseflags[8];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  char caTid[16],caKid[16];
	  long lSid,lTid;
	  char caPre[36];
	  long lId,lModtime,lFlags,lNtype;
	  char caUname[64],caDescr[64],caSname[36];
	  long iNum;
	   long lktype,lcaseflags,ltid;
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  memset(caPre,0,sizeof(caPre));
	  iReturn = utMsgGetSomeNVar(psMsgHead,14,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,4,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "ntype",UT_TYPE_STRING,10,caNtype,
			             "tid",UT_TYPE_STRING,10,caTid,
			             "content",UT_TYPE_STRING,31,caContent,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel);  
	  if(strlen(caContent)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caContent,caTemp,63)); 
	  	strcpy(caContent,caTemp_d);
	  }
	  if(strlen(caKeyword)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,63));
	  	strcpy(caKeyword,caTemp_d);
	  }          
			              

	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caUpdate,"update")==0){
	  	if(strlen(caSid)==0){
	  		sprintf(caTemp,"select count(*) from ncuseridlist where id=%lu  and uname=Binary('%s') and ntype=%lu ",atol(caTid),caContent,atol(caNtype));
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
	  			 pasDbExecSqlF("insert into ncuseridlist(id,uname,ntype,descr,modtime,flags) \
	       		values(%lu,'%s',%lu,'%s',%lu,8)",atol(caTid),caContent,atol(caNtype),caContent,time(0));
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncuseridlist where id=%lu and uname='%s' and ntype=%lu and sid!=%s ",atol(caTid),caContent,atol(caNtype),caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
 		
	  				pasDbExecSqlF("update ncuseridlist set ntype=%lu,uname='%s',modtime=%d where sid=%s ",atol(caNtype),caContent,time(0),caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("delete from  ncuseridlist  where sid in(%s)",caSelsid);
	  }
	  
	  
	  
	  
	  sprintf(caTemp,"select count(*) from ncuseridlist where 1=1 ");
    if(strlen(caTid)>0)
    sprintf(caTemp+strlen(caTemp)," and id=%s  ",caTid);
    
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and uname like '%c%s%c' ",'%',caKeyword,'%');
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select a.sid,a.id,a.uname,a.descr,a.modtime,a.flags,b.name,a.ntype,c.name from ncuseridlist a left join ncuseridclass b on (a.id=b.id) left join ncsnettype c on  (a.ntype=c.ntype) where 1=1 ");
    	if(strlen(caTid)>0)
    	sprintf(caTemp+strlen(caTemp)," and a.id=%s ",caTid);
    	
     if(strlen(caKeyword)>0){
    	 sprintf(caTemp+strlen(caTemp)," and a.uname like '%c%s%c' ",'%',caKeyword,'%');
     }
    	sprintf(caTemp+strlen(caTemp)," order by a.id,a.uname limit %d,%d",lStartRec,lRowNum);
    	
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
    lId=0;
    memset(caUname,0,sizeof(caUname));
    memset(caDescr,0,sizeof(caDescr));
    lModtime=0;
    lFlags=0;
    memset(caName,0,sizeof(caName));
    memset(caSname,0,sizeof(caSname));
    lNtype=0;
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&lId,
      	                               UT_TYPE_STRING,63,caUname,
      	                               UT_TYPE_STRING,63,caDescr,
      	                               UT_TYPE_LONG,4,&lModtime,
      	                               UT_TYPE_LONG,4,&lFlags,
      	                               UT_TYPE_STRING,31,caName,
      	                               UT_TYPE_LONG,4,&lNtype,
      	                               UT_TYPE_STRING,31,caSname);
      	                             
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"tid",iNum,"%lu",lId);
        utPltPutLoopVar(psDbHead,"content",iNum,caUname);
        utPltPutLoopVarF(psDbHead,"service",iNum,"%lu",lNtype);
        utPltPutLoopVar(psDbHead,"sname",iNum,caSname);
      
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
         if(strcmp(caPre,caName)!=0){
        	utPltPutLoopVar(psDbHead,"type_desc",iNum,caName);
        }
         strcpy(caPre,caName);

       
			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_keword_list.htm");
	  return 0;
}


/* 显示重点虚拟身份导入next界面  */
int ncSenVidDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
    char caTemp[400];
    FILE *fp;
    int i,iNum,iReturn;
   
    long lSid,lCount,lFlags;
    utPltDbHead *psDbHead;

 	char caFile[256];
 	char imp_file[129]="";
 	char temp_file[129]="";
 	char sqlbuf[1024]="";
 	int  m=0;
 	char caLocal[128],caRemote[128],caType[128];
 	char caText[256],caBuf[16002];
  unsigned id=0;
  char name[36];
 	char *p;
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif
psDbHead = utPltInitDb();
   if(!utFileIsExist("/home/ncmysql/ncs/upload")) {
        if(mkdir("/home/ncmysql/ncs/upload",777)!=0){
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0194导入"),ncsLang("0183打开文件出错"));
        return 0;

        }
}
system("chmod -Rf 777 /home/ncmysql/ncs/upload");
//chmod("/home/ncmysql/ncs/upload",777);

	/*取重点虚拟身份类别*/ 
  sprintf(sqlbuf,"select id,name from ncuseridclass order by name ");
	psCur=pasDbOpenSql(sqlbuf,0);
	if(psCur==NULL)
	{
		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0194导入"),ncsLang("0183打开文件出错"));
		return 0;
	}
	iReturn=0;
	m=0;
	while((iReturn==0)||(iReturn==1405))
	{
		memset(name,0,sizeof(name));
		id=0;
		iReturn=pasDbFetchInto(psCur,
		UT_TYPE_LONG,4,&id,
		UT_TYPE_STRING,16,name);
			
		if((iReturn==0)||(iReturn==1405))
		{
			m++;
			utPltPutLoopVar(psDbHead,"name",m,name);
	    utPltPutLoopVarF(psDbHead,"id",m,"%lu",id);
		}
	}
	pasDbCloseCursor(psCur);


	/*取身份类别*/ 
  sprintf(sqlbuf,"select ntype,name from ncsnettype order by name ");
	psCur=pasDbOpenSql(sqlbuf,0);
	if(psCur==NULL)
	{
		utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0194导入"),ncsLang("0183打开文件出错"));
		return 0;
	}
	iReturn=0;
	m=0;
	while((iReturn==0)||(iReturn==1405))
	{
		memset(name,0,sizeof(name));
		id=0;
		iReturn=pasDbFetchInto(psCur,
		UT_TYPE_LONG,4,&id,
		UT_TYPE_STRING,16,name);
			
		if((iReturn==0)||(iReturn==1405))
		{
			m++;
			utPltPutLoopVar(psDbHead,"sname",m,name);
	    utPltPutLoopVarF(psDbHead,"ntype",m,"%lu",id);
		}
	}
	pasDbCloseCursor(psCur);



	utMsgGetSomeNVar(psMsgHead,1,"fname",UT_TYPE_STRING,  255,imp_file);
	                           
     printf("imp_file=%s\n",imp_file);                 
  if(strlen(imp_file)!=0)
  {
  	p=utStrSkipSpaces(imp_file);
  	p=utStrGetWord(p,temp_file,200,";\n");
  	if((*p)==';')
  	p=utStrGetWord(p+1,temp_file,200,";\n");

  	fp=fopen(temp_file,"r");
  	printf("temp_file=%s\n",temp_file);
  	if(fp == NULL) 
    {
    	utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0194导入"),ncsLang("0183打开文件出错"));
     	return 0;
    } 
     	

    p = fgets(caBuf,16000,fp);
    iNum = 0;
    while(p && *p) {
        p = utStrGetWord(p,caTemp,300,",\r\n");
        if(!utStrIsSpaces(caTemp)) {
            iNum++;
            utPltPutLoopVar(psDbHead,"caTemp",iNum,caTemp);
            utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum-1);
            
            if(iNum==1)
            {
            utPltPutVar(psDbHead,"webclass",caTemp);
            utPltPutVarF(psDbHead,"webclass_num","%lu",iNum-1);
            }
            
            
 
  
        }
        if(*p != ',') {
            break;
        }
        p++;
    }
    fclose(fp);
    
   
}
   utPltPutVar(psDbHead,"fname",temp_file);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/senvid_import_next.htm");
    return 0;
}


//导入敏感虚拟身份
int ncsSenVid_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	FILE *fp;
	int iReturn;
	char caFile[128]="";
	
	char *p;
	long sid,id;
	char caUrl[68],caName[68],caUrlClass[36];
  
  
    long lcount=0;
    char caFirst[12]="";
    char caUrlNum[12]="";
    char caNameNum[12]="";
    char caIdNum[12]="";
    char caDefid[16];
    long lUrl,lName,lId;
    long lTime=0;
    char caTime[16]="";
  int tReturn=0;
  int lIdn=0;
	char  caBuf[16000]="";
	char sqlbuf[2048]="";
	char caKeyword[64];
	char *APName;
  char caNtype[16];
	long sum=0;
	utPltDbHead *psDbHead;
//	psDbHead = utPltInitDb();
//utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,5,
					"keyword",UT_TYPE_STRING,11,caIdNum,
					"defclass",UT_TYPE_STRING,10,caDefid,
					"ntype",UT_TYPE_STRING,10,caNtype,
		      "first",     UT_TYPE_STRING, 11,  caFirst,
		      "fname",   UT_TYPE_STRING, 127, caFile);
		      
	lTime=time(0);
	
	if(utStrIsSpaces(caIdNum)) {
        lId = 999;
    }
    else {
        lId = atol(caIdNum);
    }
    
   
    
  
    fp = fopen(caFile,"r");
    if(fp == NULL) {
    	 utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0194导入"),ncsLang("0183打开文件出错"));
        return 0;
    }
    p = fgets(caBuf,10000,fp);
    if(!utStrIsSpaces(caFirst)) {  /* 忽略第一行  */
        p = fgets(caBuf,10000,fp);
    }
    while(p) {
        pmUtlGetCsvWord(caBuf,lId,caKeyword);
  
      	id=atol(caDefid);
      
        
      if(!utStrIsSpaces(caKeyword)) {
			lcount=0;
			sprintf(sqlbuf,"select count(*) from ncuseridlist where uname='%s' and id=%lu and ntype=%lu   ",caKeyword,id,atol(caNtype));
//	printf("sqlbuf=%s\n",sqlbuf);		
			pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lcount);

			if(lcount==0)
			{
				
				sprintf(sqlbuf,"insert into ncuseridlist (sid,id,uname,ntype,descr,modtime,flags) \
					values(0,%lu,'%s',%lu,'%s',%lu,8) ",\
					id,caKeyword,atol(caNtype),caKeyword,time(0));
printf("sqlbuf=%s\n",sqlbuf);																																									                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          					                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
				pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
				pasDbCommit(NULL);
			}                               
        }
        p = fgets(caBuf,10000,fp);
    }
    fclose(fp);
    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入完成!","导入完成!");   	
	return 0;

}


//敏感网址库树
int ncsTreeUrl_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[1024];
   	int  iNum;
   	char sqlbuf[1024];
   	long lFlag,lKid;
    char *pHash;
    typedef struct ncsKeywordStat_s {
            long id;
            long sum;                //总记录数
            long csum;               //已处理记录数
   } ncsKeywordStat;
   ncsKeywordStat *psData;
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64];
    long lId,lSid,lStatus;
    char caStatus[16];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
    char caPlate[128];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,8,
			             "del",           UT_TYPE_STRING,8,caDel,
			             "keyname",       UT_TYPE_STRING,30,caKeyname,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,8,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "del",UT_TYPE_STRING,10,caDel,
			             "plate",UT_TYPE_STRING,100,caPlate);

    if(strlen(caPlate)>0){
    	pHash=(unsigned char *)pasLHashInit(1000,1000,sizeof(struct ncsKeywordStat_s),0,4); 
    	if(pHash==NULL){
    		return -1;
    	}
    	sprintf(sqlbuf,"select count(*),flag,kid from ncssenurllog group by kid,flag ");

    	psCur=pasDbOpenSql(sqlbuf,0);
    	if(psCur==NULL){
    		free(pHash);
    		return 0;
    	}
    	lCount=0;
    	lFlag=0;
    	lKid=0;
    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lFlag,
                                 UT_TYPE_LONG,4,&lKid);
                       
     while(iReturn==0||iReturn==1405)
     {

     	  psData = (struct ncsKeywordStat_s *)pasLHashLookA(pHash,&lKid);  
     	  if(psData){
     	  	if(lFlag>0){
     	  		psData->csum=psData->csum+lCount;
     	  	}
     	  	psData->sum=psData->sum+lCount;
     	  }
     	  
     	  
    	lCount=0;
    	lFlag=0;
    	lKid=0;
    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lFlag,
                                 UT_TYPE_LONG,4,&lKid);
     }
     pasDbCloseCursor(psCur); 
    	
    	
    }



	  if(strlen(caKeyname)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyname,caTemp,63));
	  	strcpy(caKeyname,caTemp_d);
	  } 
 
 
 	  if(strcmp(caUpdate,"update")==0){
	  	if(strlen(caSid)==0){
	  		sprintf(caTemp,"select count(*) from ncwebclass where name=Binary('%s') and flags=1 ",caKeyname);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
	 // 		   lSid = ncsUtlGenId(psShmHead);
	          lSid=0;
            iReturn = pasDbOneRecord("select max(id) from ncwebclass;",0,UT_TYPE_LONG,4,&lSid);
           
            	if(lSid<20000000)
                lSid = 20000000;
                else
                lSid=lSid+1;
	 

	 
	  			 pasDbExecSqlF("insert into ncwebclass(id,name,lcount,modtime,status,flags) values (%lu,'%s',%lu,%lu,%lu,1)",
                                             lSid,caKeyname,0,time(0),atol(caStatus));
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncwebclass where name=Binary('%s') and id!=%s ",caKeyname,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
	  			pasDbExecSqlF("update ncwebclass set name='%s',modtime=%d,status=%d where id=%s ",caKeyname,time(0),atol(caStatus),caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	   	       iReturn=pasDbExecSqlF("delete from ncweblist where id in (%s)",caSelsid);
   	             if(iReturn==0){
                iReturn = pasDbExecSqlF("delete from ncwebclass where id  in(%s)",caSelsid);
              }

	  	
	  }
 
 
 
 
 
    iNum = 0;
    
     psDbHead = utPltInitDb();

		  	 psCur = pasDbOpenSqlF("select name,id,status from ncwebclass where flags=1 order by name limit 0,1000");
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
	     memset(caName,0,sizeof(caName));
	     lId=0;
	     lStatus=0;
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lId,
		                                   UT_TYPE_LONG,4,&lStatus);
		                                
		
		 
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(0);  
		         if(lStatus==0){
		         	sprintf(caTemp,"<font color=#bbbbbb>%s(%s)</font>",caName,"停用");
		         }
		         else{
		         	sprintf(caTemp,"%s",caName);
		         }
		         utPltPutLoopVar(psDbHead,"name", iNum,caName);
		         
		        if(strlen(caPlate)>0){
		         	psData = (struct ncsKeywordStat_s *)pasLHashLook(pHash,&lId);  
			     	  if(psData){
			     	  	 utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s_(%d/%d)",caName,psData->csum,psData->sum);
					    }
					    else{
					    	utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s_(%d/%d)",caName,0,0);
					    }
					  }
		        else{
		         utPltPutLoopVar(psDbHead,"groupname", iNum,caTemp);
		        }
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%ld",lId);
		         utPltPutLoopVarF(psDbHead,"status",iNum,"%d",lStatus);
		         utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
		         utPltPutLoopVar(psDbHead,"cls", iNum,"file");
		         	
				     memset(caName,0,sizeof(caName));
				     lId=0;
				     lStatus=0;
					   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
					                                   UT_TYPE_LONG,4,&lId,
					                                   UT_TYPE_LONG,4,&lStatus);
		    }
		    pasDbCloseCursor(psCur);
		
    
    	
    	
 //utPltShowDb(psDbHead);
        if(strlen(caPlate)>0){
        	free(pHash);
        	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
        }
        else{
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_keyword_checktree.htm");
    	  }
    

    return 0;
}


//显示敏感URL明细
int ncsSen_UrlDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  char caContent[64],caKtype[8],caCaseflags[8];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  char caTid[16],caKid[16];
	  long lSid,lTid;
	  char caPre[36];
	  long iNum;
	   long lktype,lcaseflags,ltid;
	   char caDescr[64];
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  memset(caPre,0,sizeof(caPre));
	  iReturn = utMsgGetSomeNVar(psMsgHead,15,
	                 "kid",            UT_TYPE_STRING, 10, caKid,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,4,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "descr",UT_TYPE_STRING,63,caDescr,
			             "tid",UT_TYPE_STRING,10,caTid,
			             "content",UT_TYPE_STRING,31,caContent,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel);  
	  if(strlen(caContent)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caContent,caTemp,63)); 
	  	strcpy(caContent,caTemp_d);
	  }
	  if(strlen(caKeyword)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,63));
	  	strcpy(caKeyword,caTemp_d);
	  }          
	  if(strlen(caDescr)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caDescr,caTemp,63));
	  	strcpy(caDescr,caTemp_d);
	  }                

	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caUpdate,"update")==0){
	  	if(strlen(caSid)==0){
	  		sprintf(caTemp,"select count(*) from ncweblist where id=%lu  and url='%s' ",atol(caTid),caContent);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("update ncweblist set name='%s' where id=%lu and url='%s' ",caDescr,atol(caTid),caContent);
	  		}
	  		else{
	  			 pasDbExecSqlF("insert into ncweblist(id,url,name,lcount,modtime,flags) \
	       		values(%lu,'%s','%s',0,%lu,1)",atol(caTid),caContent,caDescr,time(0));
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncweblist where id=%lu and url='%s' and sid!=%s ",atol(caTid),caContent,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
 		
	  				pasDbExecSqlF("update ncweblist set url='%s',name='%s',modtime=%lu where sid=%s ",caContent,caDescr,time(0),caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("delete from  ncweblist  where sid in(%s)",caSelsid);
	  }
	  
	  
	  
	  
	  sprintf(caTemp,"select count(*) from ncweblist where 1=1 ");
    if(strlen(caTid)>0)
    sprintf(caTemp+strlen(caTemp)," and id=%s  ",caTid);
    
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and (url like '%c%s%c' or b.name like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select a.sid,a.id,a.url,a.name,b.name from ncweblist a left join ncwebclass b on (a.id=b.id) where 1=1");
    	if(strlen(caTid)>0)
    	sprintf(caTemp+strlen(caTemp)," and a.id=%s ",caTid);
    	
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and (url like '%c%s%c' or b.name like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    }
    	sprintf(caTemp+strlen(caTemp)," order by a.id,a.url  limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
    memset(caContent,0,sizeof(caContent));
    lSid=0;
    lTid=0;
    memset(caDescr,0,sizeof(caDescr));
    memset(caName,0,sizeof(caName));
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&lTid,
      	                               UT_TYPE_STRING,63,caContent,
      	                               UT_TYPE_STRING,63,caDescr,
      	                               UT_TYPE_STRING,31,caName);
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"tid",iNum,"%lu",lTid);
        utPltPutLoopVar(psDbHead,"content",iNum,caContent);
        utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
         if(strcmp(caPre,caName)!=0){
        	utPltPutLoopVar(psDbHead,"type_desc",iNum,caName);
        }
         strcpy(caPre,caName);


			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_keword_list.htm");
	  return 0;
}



//显示CCIC库明细
int ncsSen_CCICDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  char caContent[64],caKtype[8],caCaseflags[8];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  char caTid[16],caKid[16];
	  long lSid,lTid;
	  char caPre[36];
	  long iNum;
	   long lktype,lcaseflags,ltid;
	   char caDescr[64],caBh[32],caBm[32],caCh[32],caIdno[32];
	   char caDelall[16],caJoin[16];
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  memset(caPre,0,sizeof(caPre));
	  iReturn = utMsgGetSomeNVar(psMsgHead,18,
	                 "bh",            UT_TYPE_STRING, 31, caBh,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "name",UT_TYPE_STRING,31,caName,
			             "bm",UT_TYPE_STRING,31,caBm,
			             "ch",UT_TYPE_STRING,31,caCh,
			             "idno",UT_TYPE_STRING,31,caIdno,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "descr",UT_TYPE_STRING,63,caDescr,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel,
			             "delall",UT_TYPE_STRING,10,caDelall,
			             "join",UT_TYPE_STRING,10,caJoin);  
	  if(strlen(caName)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caName,caTemp,31)); 
	  	strcpy(caName,caTemp_d);
	  }
	  if(strlen(caKeyword)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,63));
	  	strcpy(caKeyword,caTemp_d);
	  }          
	  if(strlen(caDescr)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caDescr,caTemp,63));
	  	strcpy(caDescr,caTemp_d);
	  }                
	  if(strlen(caBm)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caBm,caTemp,31));
	  	strcpy(caBm,caTemp_d);
	  }
	  if(strlen(caCh)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caCh,caTemp,31));
	  	strcpy(caCh,caTemp_d);
	  }

	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caUpdate,"update")==0){
	  	if(strlen(caSid)==0){
	  		sprintf(caTemp,"select count(*) from ncsccic where name='%s'  and idno='%s' ",caName,caIdno);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
        if(lCount==0){
	  			 pasDbExecSqlF("insert into ncsccic(name,idno,descr,bm,ch,bh) \
	       		values('%s','%s','%s','%s','%s','%s')",caName,caIdno,caDescr,caBm,caCh,caBh);
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncsccic where name='%s' and idno='%s' and id!=%s ",caName,caIdno,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
 
	  				pasDbExecSqlF("update ncsccic set name='%s',idno='%s',descr='%s',bm='%s',ch='%s',bh='%s' where id=%s ",caName,caIdno,caDescr,caBm,caCh,caBh,caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("delete from  ncsccic  where id in(%s)",caSelsid);
	  }
	  if(strcmp(caDelall,"delall")==0){
	  	pasDbExecSqlF("delete from ncsccic ");
	  }
	  if(strcmp(caJoin,"join")==0){
	  	//将CCIC库用户加入到重点关注用户中
	  	 sprintf(caTemp,"insert into ncszduser(dispname,idcard,mac,status,modtime) select name,idno,'',1,%lu from ncsccic where id in(%s) ",time(0),caSelsid);
	//  	 printf("caTemp=%s\n",caTemp);
	  	 pasDbExecSql(caTemp,0);
	  }
	  
	  
	  
	  sprintf(caTemp,"select count(*) from ncsccic where 1=1 ");
  
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and (name like '%c%s%c' or idno like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select id,name,idno,descr,bm,ch,bh from ncsccic where 1=1");
     if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and (name like '%c%s%c' or idno like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    }
    	sprintf(caTemp+strlen(caTemp)," order by id desc  limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {

    memset(caIdno,0,sizeof(caIdno));
    memset(caBm,0,sizeof(caBm));
    memset(caCh,0,sizeof(caCh));
    memset(caBh,0,sizeof(caBh));
    memset(caDescr,0,sizeof(caDescr));
    memset(caName,0,sizeof(caName));
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_STRING,31,caName,
      	                               UT_TYPE_STRING,31,caIdno,
      	                               UT_TYPE_STRING,63,caDescr,
      	                               UT_TYPE_STRING,31,caBm,
      	                               UT_TYPE_STRING,31,caCh,
      	                               UT_TYPE_STRING,31,caBh);
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVar(psDbHead,"name",iNum,caName);
        utPltPutLoopVar(psDbHead,"idno",iNum,caIdno);
        utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
        utPltPutLoopVar(psDbHead,"bm",iNum,caBm);
        utPltPutLoopVar(psDbHead,"ch",iNum,caCh);
        utPltPutLoopVar(psDbHead,"bh",iNum,caBh);
       
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        

			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_ccic_list.htm");
	  return 0;
}



//显示重点监控用户明细
int ncsSen_UserDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  char caContent[64],caKtype[8],caCaseflags[8];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  char caTid[16],caKid[16];
	  long lSid,lTid;
	  char caPre[36];
	  char caDispname[36],caMac[24];
	  long lStatus;
	  long iNum;
	   long lktype,lcaseflags,ltid;
	   char caDescr[64],caBh[32],caBm[32],caCh[32],caIdno[32];
	   char caDelall[16],caJoin[16];
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  memset(caPre,0,sizeof(caPre));
	  iReturn = utMsgGetSomeNVar(psMsgHead,13,
	                 "dispname",     UT_TYPE_STRING, 31, caDispname,
			             "limit",        UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "idcard",UT_TYPE_STRING,31,caIdno,
			             "mac",UT_TYPE_STRING,20,caMac,
			             "status",UT_TYPE_STRING,8,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart);
			            
	  if(strlen(caDispname)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caDispname,caTemp,31)); 
	  	strcpy(caDispname,caTemp_d);
	  }
	  if(strlen(caKeyword)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,63));
	  	strcpy(caKeyword,caTemp_d);
	  }          
	
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caUpdate,"update")==0){
	  	if(strlen(caSid)==0){
	  		sprintf(caTemp,"select count(*) from ncszduser where dispname='%s'  and idcard='%s' ",caDispname,caIdno);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
        if(lCount==0){
	  			 pasDbExecSqlF("insert into ncszduser(dispname,idcard,mac,status) \
	       		values('%s','%s','%s',%d)",caDispname,caIdno,caMac,atol(caStatus));
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncszduser where dispname='%s' and idcard='%s' and sid!=%s ",caDispname,caIdno,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
 
	  				pasDbExecSqlF("update ncszduser set dispname='%s',idcard='%s',status=%d where sid=%s ",caDispname,caIdno,atol(caStatus),caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("delete from  ncszduser  where sid in(%s)",caSelsid);
	  }
	 
	  
	  
	  sprintf(caTemp,"select count(*) from ncszduser where 1=1 ");
  
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and (dispname like '%c%s%c' or idcard like '%c%s%c' or mac like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select sid,dispname,idcard,mac,status from ncszduser where 1=1");
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and (dispname like '%c%s%c' or idcard like '%c%s%c' or mac like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
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

    memset(caIdno,0,sizeof(caIdno));
    memset(caDispname,0,sizeof(caDispname));
    memset(caMac,0,sizeof(caMac));
    lStatus=0;

    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_STRING,31,caDispname,
      	                               UT_TYPE_STRING,31,caIdno,
      	                               UT_TYPE_STRING,20,caMac,
      	                               UT_TYPE_LONG,4,&lStatus);
      	                                  	                              
      	                                                                 	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
        utPltPutLoopVar(psDbHead,"idno",iNum,caIdno);
        utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
        utPltPutLoopVarF(psDbHead,"status",iNum,"%d",lStatus);
        if(lStatus==1){
        	utPltPutLoopVar(psDbHead,"status_desc",iNum,"启用");
        }
        else{
        	utPltPutLoopVar(psDbHead,"status_desc",iNum,"停用");
        }
       
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        

			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_zduser_list.htm");
	  return 0;
}


/* 显示导入重点用户next界面  */
int ncSentiveUserDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
    char caTemp[400];
    FILE *fp;
    int i,iNum,iReturn;
   
    long lSid,lCount,lFlags;
    utPltDbHead *psDbHead;
  char caCid[16];
 	char caFile[256];
 	char imp_file[129]="";
 	char temp_file[129]="";
 	char sqlbuf[1024]="";
 	int  m=0;
 	char caLocal[128],caRemote[128],caType[128];
 	char caText[256],caBuf[16002];
  unsigned id=0;
  char name[36];
  char caName[64];
 	char *p;
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif
psDbHead = utPltInitDb();
   if(!utFileIsExist("/home/ncmysql/ncs/upload")) {
        if(mkdir("/home/ncmysql/ncs/upload",555)!=0){
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0194导入"),ncsLang("0183打开文件出错"));
        return 0;

        }
   }
   system("chmod -Rf 777 /home/ncmysql/ncs/upload");

	utMsgGetSomeNVar(psMsgHead,1,"fname",UT_TYPE_STRING,  255,imp_file);
	                            
	                           
                      
  if(strlen(imp_file)!=0)
  {
  	p=utStrSkipSpaces(imp_file);
  	p=utStrGetWord(p,temp_file,200,";\n");
  	if((*p)==';')
  	p=utStrGetWord(p+1,temp_file,200,";\n");

  	fp=fopen(temp_file,"r");
  	if(fp == NULL) 
    {
    	utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0194导入"),ncsLang("0183打开文件出错"));
     	return 0;
    } 
     	
     	
 
    p = fgets(caBuf,16000,fp);
    iNum = 0;
    while(p && *p) {
        p = utStrGetWord(p,caTemp,300,",\r\n");
//        if(!utStrIsSpaces(caTemp)) {
            iNum++;
            utPltPutLoopVar(psDbHead,"caTemp",iNum,caTemp);
            utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum-1);
                       
            if(iNum==1)
            {
            utPltPutVar(psDbHead,"name_name",caTemp);
            utPltPutVarF(psDbHead,"name_num","%lu",iNum-1);
            }
      
            if(iNum==2)
            {
            utPltPutVar(psDbHead,"idno_name",caTemp);
            utPltPutVarF(psDbHead,"idno_num","%lu",iNum-1);
            }
            if(iNum==3)
            {
            utPltPutVar(psDbHead,"mac_name",caTemp);
            utPltPutVarF(psDbHead,"mac_num","%lu",iNum-1);
            }
 //       }
        if(*p != ',') {
            break;
        }
        p++;
    }
    fclose(fp);
    
   
}
  
   utPltPutVar(psDbHead,"fname",temp_file);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/sentive_userimport_next.htm");
    return 0;
}


//导入重点用户库
int ncs_sentiveuser_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	FILE *fp;
	int iReturn;
	char caFile[128]="";
	
	char *p;
	long sid,id;
	char caUsername[128],caDispname[128],caCompname[128],caMac[128],caIdtype[128],caIdcard[128]; 
  char caCid[16],caMacNum[20];
  long lMac;
  unsigned long lNettype;
    long lcount=0;
    char caFirst[12]="";
    
    char caDispnameNum[12]="";
    char caIdno[36],caDescr[64];
    char caFlags[16];
    long lUsernameNum,lDispnameNum,lIdno,lDescr;
    long lNettypeNum,lNameNum;
    long lTime=0;
    char caTime[16]="";
    char caTname[36];
  int tReturn=0;
  int lIdn=0;
  char caCtype[16];
	char  caBuf[16000]="";
	char sqlbuf[2048]="";
	char *APName;
  char caBh[16],caBm[16],caCh[16];
  long lBh,lBm,lCh;
  char caBh_d[32],caBm_d[32],caCh_d[32];
	long sum=0;
	utPltDbHead *psDbHead;
//	psDbHead = utPltInitDb();
//utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,5,
					"idno",UT_TYPE_STRING,8,caIdno,
					"dispname",UT_TYPE_STRING,8,caDispnameNum,
					"mac",UT_TYPE_STRING,8,caMacNum,
		      "first",     UT_TYPE_STRING, 11,  caFirst,
		      "fname",   UT_TYPE_STRING, 127, caFile);
		     

	      
	lTime=time(0);
	
	
	
    if(utStrIsSpaces(caDispnameNum)) {
        lDispnameNum = 999;
    }
    else {
        lDispnameNum = atol(caDispnameNum);
    }
    if(utStrIsSpaces(caIdno)) {
        lIdno = 999;
    }
    else {
        lIdno = atol(caIdno);
    }
    
    if(utStrIsSpaces(caMacNum)) {
        lMac = 999;
    }
    else {
        lMac = atol(caMacNum);
    }
    
    fp = fopen(caFile,"r");
    if(fp == NULL) {
    	 utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0194导入"),ncsLang("0183打开文件出错"));
        return 0;
    }
    p = fgets(caBuf,10000,fp);
    if(!utStrIsSpaces(caFirst)) {  /* 忽略第一行  */
        p = fgets(caBuf,10000,fp);
    }
    while(p) {
       
        pmUtlGetCsvWord(caBuf,lDispnameNum,caDispname);
        pmUtlGetCsvWord(caBuf,lIdno,caIdno);
        pmUtlGetCsvWord(caBuf,lMac,caMac);
    

     
			lcount=0;
			if(strlen(caDispname)>0){
			sprintf(sqlbuf,"select count(*) from ncszduser where dispname='%s' and idcard='%s' ",caDispname,caIdno);
//	printf("sqlbuf=%s\n",sqlbuf);		
			pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lcount);

			if(lcount==0)
			{
				
				sprintf(sqlbuf,"insert into ncszduser (dispname,idcard,mac,status,modtime) \
					values('%s','%s','%s',1,%d) ",\
					caDispname,caIdno,caMac,lTime);
				pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
				pasDbCommit(NULL);  																																	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          					                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

			}   
		}
//			 printf("sqlbuf=%s\n",sqlbuf);		
                  
        
        p = fgets(caBuf,10000,fp);
    }
    fclose(fp);
		
    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","Sucess!","Sucess!");   	
	return 0;

}



//显示关键字库归档日志
int ncsKewordlog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	char caExport[32];
  FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];
	ncsNettype *psData;
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  char caType[32],corpname[64];
	  long iReturn,lCount;
	  char caStart[16],caTemp[4048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec;
	  char caTemp_d[128];
	  long lSid,lTid;
	  long iNum;
	  char caTname[36];
    ncsClient * psClient;
    char caDispname[32],caSip[20],caMac[20],caIdno[24],caKeycond[64];
    char caSdate[20],caEdate[20],caExp[16],caTime_flag[12],caField_flag[12];
    long lTime,lstime,letime,lDid,lUid,lVtime,lKid,lType;
    char caStemptime[24],caIdcard[24],caIdtype[16],caPosition[32],caSubject[256],caContent[10240],caDescr[256],caUrl[128];
	 char caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caKid[16],caCname[64];
	 char caKflag[16],caSid[1024];
	 char *p;
	 long rCount;
	 long lFlag;
	 char caFlag[16],caSet[16],caCond[64];
	 char caGroups[2048];
	 char caUsername2[32];
	 char caUid[16];
	 	 memset(caUsername2,0,sizeof(caUsername2)); 
	 	 strcpy(caGroups,getDsGroupid());       
   dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
	  utMsgPrintMsg(psMsgHead);

	  iReturn = utMsgGetSomeNVar(psMsgHead,25,
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
			             "keycond",UT_TYPE_STRING,10,caKeycond,
			             "sdate",UT_TYPE_STRING,10,caSdate,
			             "edate",UT_TYPE_STRING,10,caEdate,
			             "start",UT_TYPE_STRING,10,caStart,
			             "exp",UT_TYPE_STRING,10,caExp,
			            "time_flag",    UT_TYPE_STRING,8,caTime_flag,
			            "field_flag",   UT_TYPE_STRING,8,caField_flag,
			            "kid", UT_TYPE_STRING,10,caKid,
			            "kflag",UT_TYPE_STRING,8,caKflag,
			            "sid",UT_TYPE_STRING,1000,caSid,
			            "set",UT_TYPE_STRING,10,caSet,
			            "flag",UT_TYPE_STRING,10,caFlag,
			            "exp",UT_TYPE_STRING,10,caExport,
			            "did",UT_TYPE_STRING,63,caCond,
			            "uid",UT_TYPE_STRING,10,caUid);
	 if(strlen(caCond)>0){
	  	strcpy(caTemp_d, pasCvtGBK(2,caCond,caTemp,63)); 
	  	strcpy(caCond,caTemp_d);
	  }           
			           
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
	  if(strlen(caKeycond)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeycond,caTemp,63));
	  	strcpy(caKeycond,caTemp_d);
	  }                
	
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  	if(strlen(caExport)>0){
		
		lStartRec=0;
		lRowNum=99999;
		        sprintf(caFilename,"ncskeywordlog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"关键字日志\n");
		        ncUtlPrintCsv(fp,10,UT_TYPE_STRING,"关键字组",
		                           UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"源IP",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"命中关键字",
		                           UT_TYPE_STRING,"主题",
		                           UT_TYPE_STRING,"内容",
		                           UT_TYPE_STRING,"备注",
		                           UT_TYPE_STRING,"日志类型");
		                          
		                          
		                         
				
			  
		
		
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
	  if(strcmp(caSet,"set")==0&&strlen(caSid)>0){
	  	sprintf(caTemp,"update ncskeywordlog set flag=%d,opname='%s',optime=%d where sid in (%s)",atol(caFlag),caUsername2,time(0),caSid);
	  	pasDbExecSql(caTemp,0);
	  }

	  
	  sprintf(caTemp,"select count(*) from ncskeywordlog,ncskeyindex where id=kid and flag!=9 ");
  	if(strlen(caKid)>0){
	    	sprintf(caTemp+strlen(caTemp)," and kid=%s ",caKid);
	  }
    if((strlen(caCond)>0)&&strlen(caUid)==0){
    	sprintf(caTemp+strlen(caTemp)," and keyword='%s' ",caCond);
    }
    if(strlen(caCond)>0&&strlen(caUid)>0){
    	sprintf(caTemp+strlen(caTemp)," and did=%s and uid=%s ",caCond,caUid);
    }
    if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
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
    if(strlen(caKflag)>0){
    	sprintf(caTemp+strlen(caTemp)," and flag=%s ",caKflag);
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
    if(strlen(caKeycond)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (content like '%c%s%c' or subject like '%c%s%c' or descr like '%c%s%c' ) ",'%',caKeycond,'%','%',caKeycond,'%','%',caKeycond,'%');
    }
    if(strlen(caKeyword)>0){
    	if(atol(caField_flag)==8){
    	  sprintf(caTemp+strlen(caTemp)," and (content like '%c%s%c' or subject like '%c%s%c' or descr like '%c%s%c' ) ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==1){
    		sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==2){
    		sprintf(caTemp+strlen(caTemp)," and sip like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==3){
    		sprintf(caTemp+strlen(caTemp)," and mac like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==4){
    		sprintf(caTemp+strlen(caTemp)," and idcard like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==6){
    		sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==7){
    		sprintf(caTemp+strlen(caTemp)," and keyword like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==5){
    		sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select sid,did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,subject,content,url,descr,type,name,flag from ncskeywordlog,ncskeyindex where id=kid and flag!=9 ");
	    if(strlen(caKid)>0){
	    	sprintf(caTemp+strlen(caTemp)," and kid=%s ",caKid);
	    }
    if((strlen(caCond)>0)&&strlen(caUid)==0){
    	sprintf(caTemp+strlen(caTemp)," and keyword='%s' ",caCond);
    }
    if(strlen(caCond)>0&&strlen(caUid)>0){
    	sprintf(caTemp+strlen(caTemp)," and did=%s and uid=%s ",caCond,caUid);
    }
    
    if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
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
    if(strlen(caKflag)>0){
    	sprintf(caTemp+strlen(caTemp)," and flag=%s ",caKflag);
    }
    
    if(strlen(caCname)>0){
    	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caCname,'%','%',caCname,'%');
    	
    }
    if(strlen(caKeycond)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (content like '%c%s%c' or subject like '%c%s%c' or descr like '%c%s%c' ) ",'%',caKeycond,'%','%',caKeycond,'%','%',caKeycond,'%');
    }  
	    
	    
    if(strlen(caKeyword)>0){
    	if(atol(caField_flag)==8){
    	  sprintf(caTemp+strlen(caTemp)," and (content like '%c%s%c' or subject like '%c%s%c' or descr like '%c%s%c' ) ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==1){
    		sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==2){
    		sprintf(caTemp+strlen(caTemp)," and sip like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==3){
    		sprintf(caTemp+strlen(caTemp)," and mac like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==4){
    		sprintf(caTemp+strlen(caTemp)," and idcard like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==6){
    		sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==7){
    		sprintf(caTemp+strlen(caTemp)," and keyword like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==5){
    		sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
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
    lKid=0;
    memset(caKeyword,0,sizeof(caKeyword));
    memset(caSubject,0,sizeof(caSubject));
    memset(caContent,0,sizeof(caContent));
    memset(caDescr,0,sizeof(caDescr));
    memset(caUrl,0,sizeof(caUrl));
    memset(caTname,0,sizeof(caTname));
    lType=0;
    lFlag=0;
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
                                       UT_TYPE_LONG,4,&lKid,
                                       UT_TYPE_STRING,64,caKeyword,
                                       UT_TYPE_STRING,255,caSubject,
                                       UT_TYPE_STRING,10000,caContent,
                                       UT_TYPE_STRING,127,caUrl,
                                       UT_TYPE_STRING,255,caDescr,
                                       UT_TYPE_LONG,4,&lType,
                                       UT_TYPE_STRING,31,caTname,
                                       UT_TYPE_LONG,4,&lFlag);
      	                             
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",lDid);
        utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
        utPltPutLoopVarF(psDbHead,"kid",iNum,"%lu",lKid);
        utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",lType);
        
         utPltSetCvtHtml(1);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
        utPltPutLoopVar(psDbHead,"idcard",iNum,caIdcard);
        utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
        utPltPutLoopVar(psDbHead,"sip",iNum,caSip);
        utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
        utPltPutLoopVar(psDbHead,"url",iNum,caUrl);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%d",iNum);
        utPltPutLoopVarF(psDbHead,"no",iNum,"%d",lStartRec+iNum);
         html2text(caContent);
         html2text(caSubject);
         sprintf(caTemp,"<em>%s</em>",caKeyword);
        p=pasStrReplaceWith(caContent,caKeyword,caTemp,&rCount);
        if(p){
        	strcpy(caContent,p);
        	free(p);
        }
        
        p=pasStrReplaceWith(caSubject,caKeyword,caTemp,&rCount);
        if(p){
        	strcpy(caSubject,p);
        	free(p);
        }
        
        utPltPutLoopVar(psDbHead,"subject",iNum,caSubject);

       
        utPltPutLoopVar(psDbHead,"content",iNum,caContent);
        utPltPutLoopVar(psDbHead,"keyword",iNum,caKeyword);
        utPltPutLoopVar(psDbHead,"tname",iNum,caTname);
         utPltSetCvtHtml(0);
         utPltPutLoopVarF(psDbHead,"flag",iNum,"%d",lFlag);
        if(lType==1){
        	strcpy(caType,"邮件");
        }
        else if(lType==2){

        	strcpy(caType,"BBS表单");
        }
        else if(lType==3){

        	strcpy(caType,"聊天内容");
        }
        else if(lType==4){
        	strcpy(caType,"表单");
        }
        else if(lType==5){
        	strcpy(caType,"搜索日志");
        }
        else if(lType>10){
        	 psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&lType); 
		      if(psData){
		          strcpy(caType,psData->caName);
		      }
		      else{
		      	strcpy(caType,"");
		      }
        }
        else{
        	strcpy(caType,"");
        }
        utPltPutLoopVar(psDbHead,"type_desc",iNum,caType);
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
       
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,10,
					                UT_TYPE_STRING,caTname,
					                UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,caDispname,
                          UT_TYPE_STRING,caSip,
                          UT_TYPE_STRING,utTimFormat("%Y-%m-%d %H:%M:%S",lVtime),
                          UT_TYPE_STRING,caKeyword,
                          UT_TYPE_STRING,caSubject,
                          UT_TYPE_STRING,caContent,
                          UT_TYPE_STRING,caDescr,
                          UT_TYPE_STRING,caType);
                         
                        
                         
                                                  
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
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_keywordlog_list.htm");
	  return 0;
}
int isCtlChar(char*in){
	int i;
	char *p;
	p=in;
	int len=strlen(in);
	for(i=0;i<len;i++){
		
   if(iscntrl(*p)){
      return 1;
    }
    p++;
  }
  return 0;
}

//显示敏感网址归档日志
int ncsSenUrllog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		char caExport[32];
	  FILE *fp;
		char caFilename[32];
		char caFile[128];
		char caPath[128];
		 char caKflag[16],caSid[1024];
	 long lFlag;
	 char caFlag[16],caSet[16];
	 	  char caTname[36];
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[4048],caCurPg[16];
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
	 char caCond[128];
	 	char caUsername2[32];
	 	char caGroups[2048];
	  memset(caUsername2,0,sizeof(caUsername2));         
    dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
	 	strcpy(caGroups,getDsGroupid());
	  utMsgPrintMsg(psMsgHead);

	  iReturn = utMsgGetSomeNVar(psMsgHead,23,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  		          
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,			          
			             "dispname",UT_TYPE_STRING,31,caDispname,
			             "cname",UT_TYPE_STRING,63,caCname,
			             "sip",UT_TYPE_STRING,15,caSip,
			             "mac",UT_TYPE_STRING,20,caMac,
			             "idno",UT_TYPE_STRING,20,caIdno,
			             "keycond",UT_TYPE_STRING,10,caKeycond,
			             "position",UT_TYPE_STRING,30,caPosition,
			             "sdate",UT_TYPE_STRING,10,caSdate,
			             "edate",UT_TYPE_STRING,10,caEdate,
			             "start",UT_TYPE_STRING,10,caStart,
			             "exp",UT_TYPE_STRING,10,caExport,
			            "time_flag",    UT_TYPE_STRING,8,caTime_flag,
			            "field_flag",   UT_TYPE_STRING,8,caField_flag,
			            "kid", UT_TYPE_STRING,10,caKid,
			            "kflag",UT_TYPE_STRING,8,caKflag,
			            "sid",UT_TYPE_STRING,1000,caSid,
			            "set",UT_TYPE_STRING,10,caSet,
			            "flag",UT_TYPE_STRING,10,caFlag,
			             "did",UT_TYPE_STRING,63,caCond);
			          
			 if(strlen(caCond)>0){
	  	strcpy(caTemp_d, pasCvtGBK(2,caCond,caTemp,63)); 
	  	strcpy(caCond,caTemp_d);
	  } 	            
			            
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
	  if(strlen(caKeycond)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeycond,caTemp,63));
	  	strcpy(caKeycond,caTemp_d);
	  }                
	
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  if(strlen(caExport)>0){		
		lStartRec=0;
		lRowNum=99999;
		        sprintf(caFilename,"ncssenurllog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"敏感网址日志\n");
		        ncUtlPrintCsv(fp,8,UT_TYPE_STRING,"敏感网址库名",
		                           UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"源IP",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"命中关键字",
		                           UT_TYPE_STRING,"url",
		                           UT_TYPE_STRING,"标题");
		                          
		                          
		                          
		                         
				
			  
		
		
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
	   lTime=time(0);
	  if(strcmp(caSet,"set")==0&&strlen(caSid)>0){
	  	sprintf(caTemp,"update ncssenurllog set flag=%d,opname='%s',optime=%d where sid in(%s)",atol(caFlag),caUsername2,time(0),caSid);
	  	pasDbExecSql(caTemp,0);
	  }
	  
	  sprintf(caTemp,"select count(*) from ncssenurllog,ncwebclass where kid=id and flag!=9 ");
	  if(strlen(caCond)>0){
    	sprintf(caTemp+strlen(caTemp)," and keyword='%s' ",caCond);
    }
  	if(strlen(caKid)>0){
	    	sprintf(caTemp+strlen(caTemp)," and kid=%s ",caKid);
	  }
    if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
    }
	  if(strlen(caSdate)==0&&atol(caTime_flag)>0){
    		sprintf(caTemp+strlen(caTemp)," and vtime>=%lu and vtime<=%lu ",lstime,letime);
    }
    if(strlen(caKflag)>0){
    	sprintf(caTemp+strlen(caTemp)," and flag=%s ",caKflag);
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
    if(strlen(caKeycond)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (uri like '%c%s%c' or host like '%c%s%c' or title like '%c%s%c' ) ",'%',caKeycond,'%','%',caKeycond,'%','%',caKeycond,'%');
    }
    if(strlen(caKeyword)>0){
    	if(atol(caField_flag)==8){
    	  sprintf(caTemp+strlen(caTemp)," and (uri like '%c%s%c' or title like '%c%s%c' or keyword like '%c%s%c' ) ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==1){
    		sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==2){
    		sprintf(caTemp+strlen(caTemp)," and sip like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==3){
    		sprintf(caTemp+strlen(caTemp)," and mac like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==4){
    		sprintf(caTemp+strlen(caTemp)," and idcard like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==6){
    		sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==7){
    		sprintf(caTemp+strlen(caTemp)," and keyword like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==5){
    		sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
    }
    lCount=0;
    
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select sid,did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,host,uri,urlid,title,name,flag from ncssenurllog,ncwebclass where kid=id and flag!=9");
	   if(strlen(caCond)>0){
    	sprintf(caTemp+strlen(caTemp)," and keyword='%s' ",caCond);
    }
	    if(strlen(caKid)>0){
	    	sprintf(caTemp+strlen(caTemp)," and kid=%s ",caKid);
	    }
	   if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
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
    if(strlen(caKeycond)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (uri like '%c%s%c' or host like '%c%s%c' or title like '%c%s%c' ) ",'%',caKeycond,'%','%',caKeycond,'%','%',caKeycond,'%');
    }  
	  if(strlen(caKflag)>0){
    	sprintf(caTemp+strlen(caTemp)," and flag=%s ",caKflag);
    }  
	    
    if(strlen(caKeyword)>0){
    	if(atol(caField_flag)==8){
    	  sprintf(caTemp+strlen(caTemp)," and (uri like '%c%s%c' or title like '%c%s%c' or keyword like '%c%s%c' ) ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==1){
    		sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==2){
    		sprintf(caTemp+strlen(caTemp)," and sip like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==3){
    		sprintf(caTemp+strlen(caTemp)," and mac like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==4){
    		sprintf(caTemp+strlen(caTemp)," and idcard like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==6){
    		sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==7){
    		sprintf(caTemp+strlen(caTemp)," and keyword like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==5){
    		sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
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
    lKid=0;
    memset(caKeyword,0,sizeof(caKeyword));
    memset(caHost,0,sizeof(caHost));
    lUrlid=0;
    memset(caTitle,0,sizeof(caTitle));
    memset(caUrl,0,sizeof(caUrl));
    lType=0;
    memset(caTname,0,sizeof(caTname));
    lFlag=0;
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
                                       UT_TYPE_LONG,4,&lKid,
                                       UT_TYPE_STRING,64,caKeyword,
                                       UT_TYPE_STRING,63,caHost,
                                       UT_TYPE_STRING,127,caUrl,
                                       UT_TYPE_LONG,4,&lUrlid,
                                       UT_TYPE_STRING,127,caTitle,
                                       UT_TYPE_STRING,31,caTname,
                                       UT_TYPE_LONG,4,&lFlag);
                                    
      	                             
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",lDid);
        utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
        utPltPutLoopVarF(psDbHead,"kid",iNum,"%lu",lKid);
        utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",lType);
    
         utPltSetCvtHtml(1);
         if(!isCtlChar(caDispname)){
  //       	printf("caDispname=%s\n",caDispname);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
      }
        utPltPutLoopVar(psDbHead,"idcard",iNum,caIdcard);
  //      utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
        utPltPutLoopVar(psDbHead,"sip",iNum,caSip);
        utPltPutLoopVar(psDbHead,"mac",iNum,caMac);

        utPltPutLoopVar(psDbHead,"subject",iNum,caSubject);
        utPltPutLoopVar(psDbHead,"content",iNum,caContent);
        utPltPutLoopVar(psDbHead,"keyword",iNum,caKeyword);
        utPltPutLoopVar(psDbHead,"title",iNum,caTitle);
        utPltPutLoopVar(psDbHead,"tname",iNum,caTname);

        strcpy(caTemp,caHost);
         strcat(caTemp,caUrl);

        utPltPutLoopVar(psDbHead,"url",iNum,caTemp);
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
        utPltPutLoopVarF(psDbHead,"flag",iNum,"%d",lFlag);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        
           if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,8,
					                UT_TYPE_STRING,caTname,
					                UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,caDispname,
                          UT_TYPE_STRING,caSip,
                          UT_TYPE_STRING,utTimFormat("%Y-%m-%d %H:%M:%S",lVtime),
                          UT_TYPE_STRING,caKeyword,
                          UT_TYPE_STRING,caTemp,
                          UT_TYPE_STRING,caTitle);
                         
                         
                        
                         
                                                  
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
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_keywordlog_list.htm");
	  return 0;
}



//显示虚拟身份归档日志
int ncsSenNetidlog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		char caExport[32];
  FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];
	char caTname[36];
	char caKflag[16],caSid[1024],caType[16],corpname[36];
	 long lFlag;
	 char caFlag[16],caSet[16];
	
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[4048],caCurPg[16];
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
	 char caNetid[64],caDip[16],caNtype[32],caCond[64];
	 long lFun;
	 char caGroups[2048];
	 	 char caUsername2[32];
	 	 memset(caUsername2,0,sizeof(caUsername2)); 
	 strcpy(caGroups,getDsGroupid());  
   dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
	  utMsgPrintMsg(psMsgHead);

	  iReturn = utMsgGetSomeNVar(psMsgHead,24,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  		          
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,			          
			             "dispname",UT_TYPE_STRING,31,caDispname,
			             "cname",UT_TYPE_STRING,63,caCname,
			             "sip",UT_TYPE_STRING,15,caSip,
			             "mac",UT_TYPE_STRING,20,caMac,
			             "idno",UT_TYPE_STRING,20,caIdno,
			             "keycond",UT_TYPE_STRING,10,caKeycond,
			             "position",UT_TYPE_STRING,30,caPosition,
			             "sdate",UT_TYPE_STRING,10,caSdate,
			             "edate",UT_TYPE_STRING,10,caEdate,
			             "start",UT_TYPE_STRING,10,caStart,
			             "exp",UT_TYPE_STRING,10,caExp,
			            "time_flag",    UT_TYPE_STRING,8,caTime_flag,
			            "field_flag",   UT_TYPE_STRING,8,caField_flag,
			            "kid", UT_TYPE_STRING,10,caKid,
			            "kflag",UT_TYPE_STRING,8,caKflag,
			            "sid",UT_TYPE_STRING,1000,caSid,
			            "set",UT_TYPE_STRING,10,caSet,
			            "flag",UT_TYPE_STRING,10,caFlag,
			            "exp",UT_TYPE_STRING,10,caExport,
			             "did",UT_TYPE_STRING,63,caCond);
			
		if(strlen(caCond)>0){
	  	strcpy(caTemp_d, pasCvtGBK(2,caCond,caTemp,63)); 
	  	strcpy(caCond,caTemp_d);
	  } 	            
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
	  if(strlen(caKeycond)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeycond,caTemp,63));
	  	strcpy(caKeycond,caTemp_d);
	  }                
	
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  	  	if(strlen(caExport)>0){
		
		lStartRec=0;
		lRowNum=99999;
		        sprintf(caFilename,"ncsnetidlog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"敏感虚拟身份日志\n");
		        ncUtlPrintCsv(fp,9,UT_TYPE_STRING,"虚拟身份组名",
		                           UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"源IP",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"命中关键字",
		                           UT_TYPE_STRING,"虚拟身份",
		                           UT_TYPE_STRING,"虚拟身份类别",
		                           UT_TYPE_STRING,"动作");
		                          
		                          
		                          
		                         
				
			  
		
		
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
	  	  if(strcmp(caSet,"set")==0&&strlen(caSid)>0){
	  	sprintf(caTemp,"update ncssennetidlog set flag=%d,opname='%s',optime=%d where sid in(%s)",atol(caFlag),caUsername2,time(0),caSid);
	  	pasDbExecSql(caTemp,0);
	  }

	  
	  sprintf(caTemp,"select count(*) from ncssennetidlog,ncuseridclass where kid=id and flag!=9 ");
  	if(strlen(caKid)>0){
	    	sprintf(caTemp+strlen(caTemp)," and kid=%s ",caKid);
	  }
	  if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
    }
    if(strlen(caCond)>0){
    	sprintf(caTemp+strlen(caTemp)," and keyword='%s' ",caCond);
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
    if(strlen(caKflag)>0){
    	sprintf(caTemp+strlen(caTemp)," and flag=%s ",caKflag);
    }
    
    if(strlen(caKeycond)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (keyword like '%c%s%c' or netid like '%c%s%c'  ) ",'%',caKeycond,'%','%',caKeycond,'%');
    }
    if(strlen(caKeyword)>0){
    	if(atol(caField_flag)==8){
    	  sprintf(caTemp+strlen(caTemp)," and (keyword like '%c%s%c' or netid like '%c%s%c'  ) ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==1){
    		sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==2){
    		sprintf(caTemp+strlen(caTemp)," and sip like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==3){
    		sprintf(caTemp+strlen(caTemp)," and mac like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==4){
    		sprintf(caTemp+strlen(caTemp)," and idcard like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==6){
    		sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==7){
    		sprintf(caTemp+strlen(caTemp)," and keyword like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==5){
    		sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select sid,did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,netid,dip,fun,b.name,a.flag,c.name from (ncssennetidlog a ,ncuseridclass c) left join ncsnettype b on (a.ntype=b.ntype) where a.kid=c.id and flag!=9");
	    if(strlen(caKid)>0){
	    	sprintf(caTemp+strlen(caTemp)," and kid=%s ",caKid);
	    }
	   if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
     }
	   if(strlen(caCond)>0){
    		sprintf(caTemp+strlen(caTemp)," and keyword='%s' ",caCond);
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
    if(strlen(caKeycond)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (keyword like '%c%s%c' or netid like '%c%s%c'  ) ",'%',caKeycond,'%','%',caKeycond,'%');
    }  
	  if(strlen(caKflag)>0){
    	sprintf(caTemp+strlen(caTemp)," and flag=%s ",caKflag);
    }  
	    
    if(strlen(caKeyword)>0){
    	if(atol(caField_flag)==8){
    	 sprintf(caTemp+strlen(caTemp)," and (keyword like '%c%s%c' or netid like '%c%s%c'  ) ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==1){
    		sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==2){
    		sprintf(caTemp+strlen(caTemp)," and sip like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==3){
    		sprintf(caTemp+strlen(caTemp)," and mac like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==4){
    		sprintf(caTemp+strlen(caTemp)," and idcard like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==6){
    		sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==7){
    		sprintf(caTemp+strlen(caTemp)," and keyword like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==5){
    		sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
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
    lKid=0;
    memset(caKeyword,0,sizeof(caKeyword));
    memset(caNetid,0,sizeof(caNetid));
    memset(caDip,0,sizeof(caDip));
    lFun=0;
    memset(caNtype,0,sizeof(caNtype));
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
                                       UT_TYPE_LONG,4,&lKid,
                                       UT_TYPE_STRING,64,caKeyword,
                                       UT_TYPE_STRING,63,caNetid,
                                       UT_TYPE_STRING,15,caDip,
                                       UT_TYPE_LONG,4,&lFun,
                                       UT_TYPE_STRING,31,caNtype,
                                       UT_TYPE_LONG,4,&lFlag,
                                       UT_TYPE_STRING,31,caTname);
                                     
      	                             
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",lDid);
        utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
        utPltPutLoopVarF(psDbHead,"kid",iNum,"%lu",lKid);
        utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",lType);
        
         utPltSetCvtHtml(1);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
        utPltPutLoopVar(psDbHead,"idcard",iNum,caIdcard);
 //       utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
        utPltPutLoopVar(psDbHead,"sip",iNum,caSip);
        utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
        utPltPutLoopVar(psDbHead,"netid",iNum,caNetid);
        utPltPutLoopVar(psDbHead,"type_desc",iNum,caNtype);        
        utPltPutLoopVar(psDbHead,"keyword",iNum,caKeyword);
        if(lFun==1){
        	strcpy(caType,"登录");
        	utPltPutLoopVar(psDbHead,"fun",iNum,"登录");
        }
        else if(lFun==2){
        	strcpy(caType,"退出");
        	utPltPutLoopVar(psDbHead,"fun",iNum,"退出");
        }
        else{
        	strcpy(caType,"");
        }
        utPltPutLoopVar(psDbHead,"dip",iNum,caDip);
        utPltPutLoopVar(psDbHead,"tname",iNum,caTname);
        utPltSetCvtHtml(0);
        utPltPutLoopVarF(psDbHead,"flag",iNum,"%d",lFlag);
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
       
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        
        if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,9,
					                UT_TYPE_STRING,caTname,
					                UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,caDispname,
                          UT_TYPE_STRING,caSip,
                          UT_TYPE_STRING,utTimFormat("%Y-%m-%d %H:%M:%S",lVtime),
                          UT_TYPE_STRING,caKeyword,
                          UT_TYPE_STRING,caNetid,
                          UT_TYPE_STRING,caNtype,
                          UT_TYPE_STRING,caType);
                         
                        
                         
                                                  
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
	  
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_keywordlog_list.htm");
	  return 0;
}


//显示重点用户归档日志
int ncsSenUserlog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
		char caExport[32];
	  FILE *fp;
		char caFilename[32];
		char caFile[128];
		char caPath[128];
		char caType[32],corpname[64];
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[4048],caCurPg[16];
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
	 char caNetid[64],caDip[16],caNtype[32];
	 char caKflag[16],caSid[1024],caSet[16],caFlag[16];
	 long lFun,lFlag;
	 char caCond[128];
	 char caUsername2[32];
	 char caGroups[2048];
	 memset(caUsername2,0,sizeof(caUsername2));  
	 strcpy(caGroups,getDsGroupid());    
   dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
	 
	  utMsgPrintMsg(psMsgHead);

	  iReturn = utMsgGetSomeNVar(psMsgHead,24,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  		          
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,			          
			             "dispname",UT_TYPE_STRING,31,caDispname,
			             "cname",UT_TYPE_STRING,63,caCname,
			             "sip",UT_TYPE_STRING,15,caSip,
			             "mac",UT_TYPE_STRING,20,caMac,
			             "idno",UT_TYPE_STRING,20,caIdno,
			             "keycond",UT_TYPE_STRING,10,caKeycond,
			             "position",UT_TYPE_STRING,30,caPosition,
			             "sdate",UT_TYPE_STRING,10,caSdate,
			             "edate",UT_TYPE_STRING,10,caEdate,
			             "start",UT_TYPE_STRING,10,caStart,
			             "exp",UT_TYPE_STRING,10,caExp,
			            "time_flag",    UT_TYPE_STRING,8,caTime_flag,
			            "field_flag",   UT_TYPE_STRING,8,caField_flag,
			            "kid", UT_TYPE_STRING,10,caKid,
			            "kflag",UT_TYPE_STRING,8,caKflag,
			            "sid",UT_TYPE_STRING,1000,caSid,
			            "set",UT_TYPE_STRING,10,caSet,
			            "flag",UT_TYPE_STRING,10,caFlag,
			            "exp",UT_TYPE_STRING,10,caExport,
			            "did",UT_TYPE_STRING,63,caCond);
		if(strlen(caCond)>0){
	  	strcpy(caTemp_d, pasCvtGBK(2,caCond,caTemp,63)); 
	  	strcpy(caCond,caTemp_d);
	  }            			            
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
	  if(strlen(caKeycond)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeycond,caTemp,63));
	  	strcpy(caKeycond,caTemp_d);
	  }                
	
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  	  	if(strlen(caExport)>0){
		
		lStartRec=0;
		lRowNum=99999;
		        sprintf(caFilename,"ncssenuserlog%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"重点关注用户日志\n");
		        ncUtlPrintCsv(fp,9,
		                           UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"源IP",
		                           UT_TYPE_STRING,"MAC地址",
		                           UT_TYPE_STRING,"证件类型",
		                           UT_TYPE_STRING,"证件号码",
		                           UT_TYPE_STRING,"上网时间",
		                           UT_TYPE_STRING,"命中关键字",
		                           UT_TYPE_STRING,"动作");
		
		                          
		                          
		                         
				
			  
		
		
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
	  if(strcmp(caSet,"set")==0&&strlen(caSid)>0){
	  	sprintf(caTemp,"update ncssenuserlog set flag=%d,opname='%s',optime=%d where sid in(%s)",atol(caFlag),caUsername2,time(0),caSid);
	  	pasDbExecSql(caTemp,0);
	  }

	  
	  sprintf(caTemp,"select count(*) from ncssenuserlog where flag!=9 ");
    if(strlen(caCond)>0){
    	sprintf(caTemp+strlen(caTemp)," and keyword='%s' ",caCond);
    }
    if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
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
    if(strlen(caKeycond)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (keyword like '%c%s%c' or netid like '%c%s%c'  ) ",'%',caKeycond,'%','%',caKeycond,'%');
    }
    if(strlen(caKeyword)>0){
    	if(atol(caField_flag)==8){
    	  sprintf(caTemp+strlen(caTemp)," and (keyword like '%c%s%c') ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==1){
    		sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==2){
    		sprintf(caTemp+strlen(caTemp)," and sip like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==3){
    		sprintf(caTemp+strlen(caTemp)," and mac like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==4){
    		sprintf(caTemp+strlen(caTemp)," and idcard like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==6){
    		sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==7){
    		sprintf(caTemp+strlen(caTemp)," and keyword like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==5){
    		sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
    }
    if(strlen(caKflag)>0){
    	sprintf(caTemp+strlen(caTemp)," and flag=%s ",caKflag);
    }
    
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	  sprintf(caTemp,"select sid,did,uid,dispname,idcard,mac,idtype,position,sip,vtime,keyword,flag,fun from ncssenuserlog  where flag!=9");
	  if(strlen(caCond)>0){
    	sprintf(caTemp+strlen(caTemp)," and keyword='%s' ",caCond);
    }
    if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
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
    if(strlen(caKeycond)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (keyword like '%c%s%c') ",'%',caKeycond,'%');
    }  
	  if(strlen(caKflag)>0){
    	sprintf(caTemp+strlen(caTemp)," and flag=%s ",caKflag);
    } 
	    
    if(strlen(caKeyword)>0){
    	if(atol(caField_flag)==8){
    	 sprintf(caTemp+strlen(caTemp)," and (keyword like '%c%s%c') ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==1){
    		sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==2){
    		sprintf(caTemp+strlen(caTemp)," and sip like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==3){
    		sprintf(caTemp+strlen(caTemp)," and mac like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==4){
    		sprintf(caTemp+strlen(caTemp)," and idcard like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==6){
    		sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==7){
    		sprintf(caTemp+strlen(caTemp)," and keyword like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==5){
    		sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
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
    lKid=0;
    memset(caKeyword,0,sizeof(caKeyword));
    memset(caNetid,0,sizeof(caNetid));
    memset(caDip,0,sizeof(caDip));
    lFun=0;
    lFlag=0;
    memset(caNtype,0,sizeof(caNtype));
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
                                       UT_TYPE_STRING,64,caKeyword,
                                       UT_TYPE_LONG,4,&lFlag,
                                       UT_TYPE_LONG,4,&lFun);
                                      
                                     
      	                             
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",lDid);
        utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
        utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",lType);
        
         utPltSetCvtHtml(1);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
        utPltPutLoopVar(psDbHead,"idcard",iNum,caIdcard);
        utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
        utPltPutLoopVar(psDbHead,"sip",iNum,caSip);
        utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
        utPltPutLoopVar(psDbHead,"type_desc",iNum,caNtype);        
        utPltPutLoopVar(psDbHead,"keyword",iNum,caKeyword);
        if(lFun==1){
        	utPltPutLoopVar(psDbHead,"fun",iNum,"上线");
        	strcpy(caType,"上线");
        }
        else if(lFun==2){
        	utPltPutLoopVar(psDbHead,"fun",iNum,"下线");
        	strcpy(caType,"下线");
        }
        else{
        	strcpy(caType,"");
        }
       utPltPutLoopVarF(psDbHead,"flag",iNum,"%d",lFlag);
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
       
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        
        if(strlen(caExport)>0){
					 ncUtlPrintCsv(fp,9,					               
					                UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,caDispname,
                          UT_TYPE_STRING,caSip,
                          UT_TYPE_STRING,caMac,
                          UT_TYPE_STRING,ncsGetIdnoNameByType(caIdtype),
                          UT_TYPE_STRING,caIdcard,
                          UT_TYPE_STRING,utTimFormat("%Y-%m-%d %H:%M:%S",lVtime),
                          UT_TYPE_STRING,caKeyword,
                          UT_TYPE_STRING,caType);
                        
                        
                         
                                                  
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
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_keywordlog_list.htm");
	  return 0;
}



//显示虚拟身份库
int ncsVid_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[4048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec;
	  char caTemp_d[128];
	  long lSid,lTid;
	  long iNum;
    ncsClient * psClient;
    ncsNettype *psData;
    char caNtype[32];
    char caDispname[32],caSip[20],caMac[20],caIdno[24],caKeycond[64];
    char caSdate[20],caEdate[20],caExport[16],caTime_flag[12],caField_flag[12];
    long lTime,lstime,letime,lDid,lUid,lVtime,lKid,lType;
    char caStemptime[24],caIdcard[24],caIdtype[16],caPosition[32],caSubject[256],caContent[1024],caDescr[256],caUrl[128];
	 char caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caKid[16],caCname[64];
	 char caNetid[64],caDip[16];
	 long lFun,lNtype,i;
	 char caPass[64];
	 struct ntId_s{
	 	long ntype;
	 	char caName[64];
	};
	char caDid[16],caGroupid[16];
	char caType_flag[16];
	long lSvtime;
	struct ntId_s stData[48];
	long lSum;
	 FILE *fp;
	 char caFilename[32];
	 char caFile[128];
	 char caPath[128];
	 char caServicecode[20];
	 char caNid[64],corpname[100],caStime[24],caEtime[24];
	//   pasLHashInfo sHashInfo; 
	char caGroups[1024];
  char caFacs[1024];
	 strcpy(caFacs,getDsFacDid());
   strcpy(caGroups,getDsGroupid());
   
	  utMsgPrintMsg(psMsgHead);

  //       psData = (struct ncsNettype_s *)pasLHashFirst(pHashNettype,&sHashInfo);
  //    	 while(psData){
 //        printf("ID=%d,name=%s\n",psData->id,psData->caName);
 //     	 	psData = (struct ncsNettype_s *)pasLHashNext(&sHashInfo);
 //     	}
      


	  iReturn = utMsgGetSomeNVar(psMsgHead,21,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  		          
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,			          
			             "dispname",UT_TYPE_STRING,31,caDispname,
			             "cname",UT_TYPE_STRING,63,caCname,
			             "sip",UT_TYPE_STRING,15,caSip,
			             "mac",UT_TYPE_STRING,20,caMac,
			             "idno",UT_TYPE_STRING,20,caIdno,
			             "keycond",UT_TYPE_STRING,10,caKeycond,
			             "position",UT_TYPE_STRING,30,caPosition,
			             "sdate",UT_TYPE_STRING,10,caSdate,
			             "edate",UT_TYPE_STRING,10,caEdate,
			             "start",UT_TYPE_STRING,10,caStart,
			             "export2",UT_TYPE_STRING,10,caExport,
			            "time_flag",    UT_TYPE_STRING,8,caTime_flag,
			            "field_flag",   UT_TYPE_STRING,8,caField_flag,
			            "ntype", UT_TYPE_STRING,10,caNtype,
			            "type_flag",UT_TYPE_STRING,10,caType_flag,
			            "did",UT_TYPE_STRING,10,caDid,
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
	  if(strlen(caKeycond)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeycond,caTemp,63));
	  	strcpy(caKeycond,caTemp_d);
	  }                
	
	
		if(strlen(caExport)>0){
		lStartRec=0;
		lRowNum=99999;
				

		        sprintf(caFilename,"ncsnetid%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"虚拟身份\n");
		        ncUtlPrintCsv(fp,10,UT_TYPE_STRING,"单位名称",
		                           UT_TYPE_STRING,"用户名",
		                           UT_TYPE_STRING,"MAC地址",
		                           UT_TYPE_STRING,"证件类型",
		                           UT_TYPE_STRING,"证件号码",
		                           UT_TYPE_STRING,"房间号/位置",
		                           UT_TYPE_STRING,"虚拟身份类别",
	                             UT_TYPE_STRING,"虚拟身份帐号",
		                           UT_TYPE_STRING,"最早使用时间",
		                           UT_TYPE_STRING,"最后使用时间");
				
			  
		
		
	}
	else{
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
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
	  

	  
	  sprintf(caTemp,"select count(*) from ncsnetid_if where 1=1 ");
  
	  if(strlen(caSdate)==0&&atol(caTime_flag)>0){
    		sprintf(caTemp+strlen(caTemp)," and lasttime>=%lu and lasttime<=%lu ",lstime,letime);
    }
    
    if(strlen(caSdate)>0){
    	   	 sprintf(caStemptime,"%s 00:00",caSdate);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		sprintf(caTemp+strlen(caTemp)," and lasttime>=%d ",lstime);
   	  		if(strlen(caEdate)>0){
   	  		  sprintf(caStemptime,"%s 23:59",caEdate);
   	  		  letime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		  sprintf(caTemp+strlen(caTemp)," and lasttime<=%d ",letime);
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
    	sprintf(caTemp+strlen(caTemp)," and idno like '%c%s%c' ",'%',caIdno,'%');
    }
    if(strlen(caPosition)>0){
    	sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caPosition,'%');
    }
    if(strlen(caCname)>0){
    	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caCname,'%','%',caCname,'%');
    	
    }
    if(strlen(caNtype)>0){
    	sprintf(caTemp+strlen(caTemp)," and ntype=%s ",caNtype);
    }
    if(strlen(caKeycond)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (name like '%c%s%c'   ) ",'%',caKeycond,'%');
    }
    if(atol(caType_flag)==1001){
    	sprintf(caTemp+strlen(caTemp)," and ntype=1001 ");
    }
    else if(atol(caType_flag)==1000){
    	sprintf(caTemp+strlen(caTemp)," and ntype>=1002 and ntype<=1999 ");
    }
    else if(atol(caType_flag)==20000){
  //  	sprintf(caTemp+strlen(caTemp)," and ntype>=20000 and ntype<=29999 ");
        sprintf(caTemp+strlen(caTemp)," and ntype in (select ntype from ncsnettype where gid=20 ) ");
    }
    else if(atol(caType_flag)==21000){
        sprintf(caTemp+strlen(caTemp)," and ntype in (select ntype from ncsnettype where gid=21 ) ");
    }
    else if(atol(caType_flag)==24000){
        sprintf(caTemp+strlen(caTemp)," and ntype in (select ntype from ncsnettype where gid=24 ) ");
    }
    else if(atol(caType_flag)==22000){
        sprintf(caTemp+strlen(caTemp)," and ntype in (select ntype from ncsnettype where gid=22 ) ");
    }
    else if(atol(caType_flag)==23000){
        sprintf(caTemp+strlen(caTemp)," and ntype in (select ntype from ncsnettype where gid=23 ) ");
    }
    else if(atol(caType_flag)==25000){
        sprintf(caTemp+strlen(caTemp)," and ntype in (select ntype from ncsnettype where gid=25 ) ");
    }
    else if(atol(caType_flag)==2000){
    	sprintf(caTemp+strlen(caTemp)," and ntype>=2000 and ntype<=2999 ");
    }
    else if(atol(caType_flag)==13000){
    	sprintf(caTemp+strlen(caTemp)," and ((ntype>=13000 and ntype<=13999) or ntype in (select ntype from ncsnettype where gid=13)) ");
    }
    else if(atol(caType_flag)==4002){
    	sprintf(caTemp+strlen(caTemp)," and ntype=4002 ");
    }
    if(strlen(caKeyword)>0){
    	if(atol(caField_flag)==8){
    	  sprintf(caTemp+strlen(caTemp)," and (name like '%c%s%c') ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==1){
    		sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==2){
    		sprintf(caTemp+strlen(caTemp)," and sip like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==3){
    		sprintf(caTemp+strlen(caTemp)," and mac like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==4){
    		sprintf(caTemp+strlen(caTemp)," and idno like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==6){
    		sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==7){
    		sprintf(caTemp+strlen(caTemp)," and name like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==5){
    		sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
    }
    if(strlen(caDid)>0){
    	sprintf(caTemp+strlen(caTemp)," and did=%s ",caDid);
    }
    if(strlen(caGroupid)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",getGroupid(atol(caGroupid )));
    }
    if(strlen(caGroups)>0){
				sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				sprintf(caTemp+strlen(caTemp)," and did in (%s) ",caFacs);
			}
    
    
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	  sprintf(caTemp,"select did,uid,dispname,idno,mac,idtype,position,sip,lasttime,name,ntype,starttime from ncsnetid_if  where 1=1");
	 
	  if(strlen(caSdate)==0&&atol(caTime_flag)>0){
    		sprintf(caTemp+strlen(caTemp)," and lasttime>=%lu and lasttime<=%lu ",lstime,letime);
    }
	    
	      if(strlen(caSdate)>0){
    	   	 sprintf(caStemptime,"%s 00:00",caSdate);
   	  		lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		sprintf(caTemp+strlen(caTemp)," and lasttime>=%d ",lstime);
   	  		if(strlen(caEdate)>0){
   	  		  sprintf(caStemptime,"%s 23:59",caEdate);
   	  		  letime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
   	  		  sprintf(caTemp+strlen(caTemp)," and lasttime<=%d ",letime);
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
    	sprintf(caTemp+strlen(caTemp)," and idno like '%c%s%c' ",'%',caIdno,'%');
    }
    if(strlen(caPosition)>0){
    	sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caPosition,'%');
    }
    if(strlen(caCname)>0){
    	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caCname,'%','%',caCname,'%');
    	
    }
    if(strlen(caNtype)>0){
    	sprintf(caTemp+strlen(caTemp)," and ntype=%s ",caNtype);
    }
    if(strlen(caDid)>0){
    	sprintf(caTemp+strlen(caTemp)," and did=%s ",caDid);
    }
    if(strlen(caGroupid)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",getGroupid(atol(caGroupid )));
    }
     if(strlen(caGroups)>0){
				sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
			}
			if(strlen(caFacs)>0){
				sprintf(caTemp+strlen(caTemp)," and did in (%s) ",caFacs);
			}
    
    
    if(atol(caType_flag)==1001){
    	sprintf(caTemp+strlen(caTemp)," and ntype=1001 ");
    }
    else if(atol(caType_flag)==1000){
    	sprintf(caTemp+strlen(caTemp)," and ntype>=1002 and ntype<=1999 ");
    }
    else if(atol(caType_flag)==20000){
  //  	sprintf(caTemp+strlen(caTemp)," and ntype>=20000 and ntype<=29999 ");
        sprintf(caTemp+strlen(caTemp)," and ntype in (select ntype from ncsnettype where gid=20 ) ");
    }
    else if(atol(caType_flag)==21000){
        sprintf(caTemp+strlen(caTemp)," and ntype in (select ntype from ncsnettype where gid=21 ) ");
    }
    else if(atol(caType_flag)==24000){
        sprintf(caTemp+strlen(caTemp)," and ntype in (select ntype from ncsnettype where gid=24 ) ");
    }
    else if(atol(caType_flag)==22000){
        sprintf(caTemp+strlen(caTemp)," and ntype in (select ntype from ncsnettype where gid=22 ) ");
    }
    else if(atol(caType_flag)==23000){
        sprintf(caTemp+strlen(caTemp)," and ntype in (select ntype from ncsnettype where gid=23 ) ");
    }
    else if(atol(caType_flag)==25000){
        sprintf(caTemp+strlen(caTemp)," and ntype in (select ntype from ncsnettype where gid=25 ) ");
    }
    else if(atol(caType_flag)==2000){
    	sprintf(caTemp+strlen(caTemp)," and ntype>=2000 and ntype<=2999 ");

    }
    else if(atol(caType_flag)==13000){
    	sprintf(caTemp+strlen(caTemp)," and ((ntype>=13000 and ntype<=13999) or ntype in (select ntype from ncsnettype where gid=13)) ");
    }
    else if(atol(caType_flag)==4002){
    	sprintf(caTemp+strlen(caTemp)," and ntype=4002 ");
    }
    
    
    if(strlen(caKeycond)>0){
    	 sprintf(caTemp+strlen(caTemp)," and (name like '%c%s%c') ",'%',caKeycond,'%');
    }  
	    
	    
    if(strlen(caKeyword)>0){
    	if(atol(caField_flag)==8){
    	 sprintf(caTemp+strlen(caTemp)," and (name like '%c%s%c') ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==1){
    		sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==2){
    		sprintf(caTemp+strlen(caTemp)," and sip like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==3){
    		sprintf(caTemp+strlen(caTemp)," and mac like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==4){
    		sprintf(caTemp+strlen(caTemp)," and idno like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==6){
    		sprintf(caTemp+strlen(caTemp)," and position like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==7){
    		sprintf(caTemp+strlen(caTemp)," and name like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==5){
    		sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
    }
    	sprintf(caTemp+strlen(caTemp)," limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	 lSum=0;      
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
    lKid=0;
    memset(caKeyword,0,sizeof(caKeyword));
    memset(caNetid,0,sizeof(caNetid));
    memset(caDip,0,sizeof(caDip));
    lFun=0;
    memset(caNtype,0,sizeof(caNtype));
    lNtype=0;
    lSvtime=0;
    iReturn = pasDbFetchInto(psCur,    
                                       UT_TYPE_LONG,4,&lDid,
                                       UT_TYPE_LONG,4,&lUid,
                                       UT_TYPE_STRING,31,caDispname,
                                       UT_TYPE_STRING,19,caIdcard,
                                       UT_TYPE_STRING,19,caMac,
                                       UT_TYPE_STRING,4,caIdtype,
                                       UT_TYPE_STRING,30,caPosition,
                                       UT_TYPE_STRING,15,caSip,
                                       UT_TYPE_LONG,4,&lVtime,
                                       UT_TYPE_STRING,63,caKeyword,
                                       UT_TYPE_LONG,4,&lNtype,
                                       UT_TYPE_LONG,4,&lSvtime);
                                      
                                     
      	                             
      	                              
      	                                                             
                        
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
        memset(caNid,0,sizeof(caNid));
        memset(corpname,0,sizeof(corpname));
        memset(caEtime,0,sizeof(caEtime));
        memset(caStime,0,sizeof(caStime));
        

      memset(caNtype,0,sizeof(caNtype));
      psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&lNtype); 
      if(psData){
         strcpy(caNtype,psData->caName);      	
      }; 
         psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lDid);           
         if(psClient){
         	strcpy(corpname,psClient->dispname);
         	strcpy(caServicecode,psClient->username);
         }
   if(strlen(caExport)==0)
   	{     
   		 if(lSum<40){ 
        stData[lSum].ntype=lNtype;
        strcpy(stData[lSum].caName,caKeyword);
      }
        lSum++;
   		
   		
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
  
        utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",lDid);
        utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
        utPltPutLoopVarF(psDbHead,"type",iNum,"%lu",lType);
        
         utPltSetCvtHtml(1);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
        utPltPutLoopVar(psDbHead,"idcard",iNum,caIdcard);
        utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
        utPltPutLoopVar(psDbHead,"sip",iNum,caSip);
        utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
        utPltPutLoopVar(psDbHead,"type_desc",iNum,caNtype);        
        utPltPutLoopVar(psDbHead,"name",iNum,caKeyword);
        utPltPutLoopVar(psDbHead,"ntype_desc",iNum,caNtype);
        utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
         utPltPutLoopVar(psDbHead,"cname",iNum,corpname);
         	
        utPltSetCvtHtml(0);
        memset(corpname,0,sizeof(corpname));
         utPltPutLoopVarF(psDbHead,"ntype",iNum,"%d",lNtype);

        utPltPutLoopVar(psDbHead,"idtype",iNum,ncsGetIdnoNameByType(caIdtype));
        
        if(lVtime>1000000000&&lVtime<2000000000){
        	strcpy(caEtime,utTimFormat("%Y-%m-%d %H:%M:%S",lVtime));
        			utPltPutLoopVar(psDbHead,"vtime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lVtime));
         }
        if(lSvtime>1000000000&&lSvtime<2000000000){
        	 strcpy(caStime,utTimFormat("%Y-%m-%d %H:%M:%S",lSvtime));
         	utPltPutLoopVar(psDbHead,"svtime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lSvtime));
        }
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
     }
				if(strlen(caExport)>0){
	//				printf("aaaaa=%s\n",ncsGetIdnoNameByType(caIdtype));
					 ncUtlPrintCsv(fp,10,UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,caDispname,
                          UT_TYPE_STRING,caMac,
                          UT_TYPE_STRING,ncsGetIdnoNameByType(caIdtype),
                          UT_TYPE_STRING,caIdcard,
                          UT_TYPE_STRING,caPosition,
                          UT_TYPE_STRING,caNtype,
                          UT_TYPE_STRING,caKeyword,
                          UT_TYPE_STRING,caStime,
                          UT_TYPE_STRING,caEtime);
				 }
			
    }
    pasDbCloseCursor(psCur);
	//printf("lSum=%d\n",lSum); 
	  if(strlen(caExport)==0)
   	{
	 
    for(i=0;i<lSum;i++){
 //   	printf("st=%d\n",stData[i].ntype);
    	if(stData[i].ntype==1001){
    		sprintf(caTemp,"select qqkey from ncsqqnumber where qq='%s' ",stData[i].caName);
    		memset(caPass,0,sizeof(caPass));
 //   		printf("caTemp=%s\n",caTemp);
    		pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caPass);
    		if(strlen(caPass)>4){
    			utPltPutLoopVar(psDbHead,"passwd",i+1,caPass);
    		}
    	}
    	else if(stData[i].ntype>=20001&&stData[i].ntype<=29999){
    		sprintf(caTemp,"select passwd from nchttpacclog where ntype=%d and username='%s' ",stData[i].ntype,stData[i].caName);
    		memset(caPass,0,sizeof(caPass));
    		pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caPass);
    		if(strlen(caPass)>0){
    			utPltPutLoopVar(psDbHead,"passwd",i+1,caPass);
    		}
    	}
    }

}

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  
	  	 if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_allvidlog_list.htm");
	  return 0;
}




//显示MAC地址库
int ncsMacVid_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec;
	  char caTemp_d[128];
	  long lSid,lTid;
	  long iNum;
    ncsClient * psClient;
    ncsNettype *psData;
    char caNtype[32];
    char caDispname[32],caSip[20],caMac[20],caIdno[24],caKeycond[64];
    char caSdate[20],caEdate[20],caExp[16],caTime_flag[12],caField_flag[12];
    long lTime,lstime,letime,lDid,lUid,lVtime,lKid,lType;
    char caStemptime[24],caIdcard[24],caIdtype[16],caPosition[32],caSubject[256],caContent[1024],caDescr[256],caUrl[128];
	 char caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caKid[16],caCname[64];
	 char caNetid[64],caDip[16];
	 long lFun,lNtype,i;
	 char caPass[64];
	 struct ntId_s{
	 	long ntype;
	 	char caName[64];
	};
	char caType_flag[16];
	struct ntId_s stData[48];
	long lSum;
	char caGroups[1024];
  char caFacs[1024];
  char caDid[20],caGid[20];
  long lGid;
  char caExport[36];
  FILE *fp;
	 char caFilename[32];
	 char caFile[128];
	 char caPath[128];
	 char corpname[128];
	strcpy(caFacs,getDsFacDid());
  strcpy(caGroups,getDsGroupid());
	//   pasLHashInfo sHashInfo; 
	  utMsgPrintMsg(psMsgHead);

  //       psData = (struct ncsNettype_s *)pasLHashFirst(pHashNettype,&sHashInfo);
  //    	 while(psData){
 //        printf("ID=%d,name=%s\n",psData->id,psData->caName);
 //     	 	psData = (struct ncsNettype_s *)pasLHashNext(&sHashInfo);
 //     	}
      


	  iReturn = utMsgGetSomeNVar(psMsgHead,21,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  		          
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,			          
			             "dispname",UT_TYPE_STRING,31,caDispname,
			             "cname",UT_TYPE_STRING,63,caCname,
			             "sip",UT_TYPE_STRING,15,caSip,
			             "mac",UT_TYPE_STRING,20,caMac,
			             "idno",UT_TYPE_STRING,20,caIdno,
			             "keycond",UT_TYPE_STRING,10,caKeycond,
			             "position",UT_TYPE_STRING,30,caPosition,
			             "sdate",UT_TYPE_STRING,10,caSdate,
			             "edate",UT_TYPE_STRING,10,caEdate,
			             "start",UT_TYPE_STRING,10,caStart,
			             "export2",UT_TYPE_STRING,10,caExport,
			            "time_flag",    UT_TYPE_STRING,8,caTime_flag,
			            "field_flag",   UT_TYPE_STRING,8,caField_flag,
			            "ntype", UT_TYPE_STRING,10,caNtype,
			            "type_flag",UT_TYPE_STRING,10,caType_flag,
			            "did",UT_TYPE_STRING,10,caDid,
			            "gid",UT_TYPE_STRING,10,caGid);
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
	  if(strlen(caKeycond)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeycond,caTemp,63));
	  	strcpy(caKeycond,caTemp_d);
	  }                
	
			if(strlen(caExport)>0){
					lStartRec=0;
					lRowNum=99999;
							
			
					        sprintf(caFilename,"ncsmac%d.csv",utStrGetId());
					        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
					        sprintf(caFile,"%s/%s",caPath,caFilename);
					    	fp = fopen(caFile,"w");
					        if(fp == NULL) {
					            pasDbCloseCursor(psCur);
					         
					            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
					            return 0;
					        }
					        
					        fprintf(fp,"MAC地址\n");
					        ncUtlPrintCsv(fp,3,UT_TYPE_STRING,"单位名称",
					                           UT_TYPE_STRING,"用户名",
					                           UT_TYPE_STRING,"MAC地址",
					                           UT_TYPE_STRING,"最后使用时间");
				
			  
		
		
	}
	else{
	
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	}
	  
	  
	  
	      lTime=time(0);
    

	  
  
	  
	  psDbHead = utPltInitDb(); 
	  
    lGid=atol(caGid);
	  
	  sprintf(caTemp,"select count(*) from ncsmac where 1=1 ");
    if(strlen(caDid)>0){
    	sprintf(caTemp+strlen(caTemp)," and did=%s ",caDid);
    }
    if(strlen(caGid)>0){
    	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where groupid in (%s)) ",getGroupid(lGid));
    }
  
    if(strlen(caKeyword)>0){
    
     if(atol(caField_flag)==1){
    		sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==3){
    		sprintf(caTemp+strlen(caTemp)," and mac='%s' ",caKeyword);
    	}
    	else if(atol(caField_flag)==5){
    		sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
    }
    if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where groupid in (%s) )",caGroups);
    }
    if(strlen(caFacs)>0){
    	sprintf(caTemp+strlen(caTemp)," and did in (%s) ",caFacs);
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
//	printf("caTemp=%s\n",caTemp);  
	  sprintf(caTemp,"select did,uid,dispname,mac,lasttime from ncsmac where 1=1");
	 
    if(strlen(caKeyword)>0){
    
     if(atol(caField_flag)==1){
    		sprintf(caTemp+strlen(caTemp)," and dispname like '%c%s%c' ",'%',caKeyword,'%');
    	}
    	else if(atol(caField_flag)==3){
    		sprintf(caTemp+strlen(caTemp)," and mac='%s' ",caKeyword);
    	}
    	else if(atol(caField_flag)==5){
    		sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where dispname like '%c%s%c' or username like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
    	}
    }
    if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where groupid in (%s) )",caGroups);
    }
    if(strlen(caFacs)>0){
    	sprintf(caTemp+strlen(caTemp)," and did in (%s) ",caFacs);
    }
      if(strlen(caDid)>0){
    	sprintf(caTemp+strlen(caTemp)," and did=%s ",caDid);
    }
    if(strlen(caGid)>0){
    	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where groupid in (%s)) ",getGroupid(lGid));
    }
    	sprintf(caTemp+strlen(caTemp)," order by sid desc limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

     psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	 lSum=0;      
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {

    lSid=0;
    lDid=0;
    lUid=0;
    memset(caDispname,0,sizeof(caDispname));
    memset(caMac,0,sizeof(caMac));
    lVtime=0;
  
    iReturn = pasDbFetchInto(psCur,    
                                       UT_TYPE_LONG,4,&lDid,
                                       UT_TYPE_LONG,4,&lUid,
                                       UT_TYPE_STRING,31,caDispname,
                                       UT_TYPE_STRING,19,caMac,
                                       UT_TYPE_LONG,4,&lVtime);
                                     
                                      
                                     
      	                             
      	                              
      	                                                             
                        
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
   		
   	 if(strlen(caExport)==0)
   	{ 
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
  
        utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",lDid);
        utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
        
         utPltSetCvtHtml(1);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);


        utPltPutLoopVar(psDbHead,"mac",iNum,caMac);

        
       
      
//      psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&lNtype); 
//      if(psData){
      
//      	utPltPutLoopVar(psDbHead,"ntype_desc",iNum,psData->caName);
 //     }; 
        utPltSetCvtHtml(0);
//         utPltPutLoopVarF(psDbHead,"ntype",iNum,"%d",lNtype);
         psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lDid);           
         if(psClient){
         	utPltPutLoopVar(psDbHead,"servicecode",iNum,psClient->username);
         	utPltPutLoopVar(psDbHead,"cname",iNum,psClient->dispname);
         }
        utPltPutLoopVar(psDbHead,"idtype",iNum,ncsGetIdnoNameByType(caIdtype));
        
        utPltPutLoopVar(psDbHead,"vtime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lVtime));
       
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
      }
      else{
      	 psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lDid);           
         if(psClient){
         	strcpy(corpname,psClient->dispname);
         }
         else{
         	strcpy(corpname,"");
        }
      						 ncUtlPrintCsv(fp,3,UT_TYPE_STRING,corpname,
                          UT_TYPE_STRING,caDispname,
                          UT_TYPE_STRING,caMac,
                          UT_TYPE_STRING,utTimFormat("%Y-%m-%d %H:%M:%S",lVtime));
                        
      	
      }

			
    }
    pasDbCloseCursor(psCur);
	//printf("lSum=%d\n",lSum);  

	  	 if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 	
	 	
	 }

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_allvidlog_list.htm");
	  return 0;
}

//显示用户的上网行为
int ncsWebShowUserBh(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 utPltDbHead *psDbHead;
	 char caUid[16],caDid[16];
	 ncsClient * psClient;
	 char caTemp[1024],caUsername[16],caDispname[32],caIp[16];
	 char caMac[20],caIdcard[20],caIdfytype[12],caPosition[32],caTime_flag[10];
	 utMsgGetSomeNVar(psMsgHead,3,"uid",UT_TYPE_STRING,10,caUid,
	                              "did",UT_TYPE_STRING,10,caDid,
	                              "time_flag",UT_TYPE_STRING,10,caTime_flag);
	 psDbHead = utPltInitDb(); 
	 
	 utPltPutVar(psDbHead,"did",caDid);
	 utPltPutVar(psDbHead,"uid",caUid);
	 utPltPutVar(psDbHead,"time_flag",caTime_flag);
	 sprintf(caTemp,"select username,dispname,inet_ntoa(ip),mac,idcard,idfytype,position from ncsenduser where did=%s and uid=%s ",caDid,caUid);
	 printf("caTemp=%s\n",caTemp);
	 memset(caUsername,0,sizeof(caUsername));
	 memset(caDispname,0,sizeof(caDispname));
	 memset(caIp,0,sizeof(caIp));
	 memset(caMac,0,sizeof(caMac));
	 memset(caIdcard,0,sizeof(caIdcard));
	 memset(caIdfytype,0,sizeof(caIdfytype));
	 memset(caPosition,0,sizeof(caPosition));
	 pasDbOneRecord(caTemp,0,UT_TYPE_STRING,15,caUsername,
	                          UT_TYPE_STRING,31,caDispname,
	                          UT_TYPE_STRING,15,caIp,
	                          UT_TYPE_STRING,19,caMac,
	                          UT_TYPE_STRING,18,caIdcard,
	                          UT_TYPE_STRING,3,caIdfytype,
	                          UT_TYPE_STRING,31,caPosition);
	
	    utPltPutVar(psDbHead,"dispname",caDispname);
	    utPltPutVar(psDbHead,"ip",caIp);
	    utPltPutVar(psDbHead,"mac",caMac);
	    utPltPutVar(psDbHead,"idno",caIdcard);
	    utPltPutVar(psDbHead,"position",caPosition);
	      
	      
	      
	      
	    psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,atol(caDid));           
      if(psClient){
         	utPltPutVar(psDbHead,"cname",psClient->dispname);
         }
        utPltPutVar(psDbHead,"idtype",ncsGetIdnoNameByType(caIdfytype));
	
		utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userbehv.htm");
	  return 0;
}


//有害网址库树
int ncsTreeUrl_yh_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[1024];
   	char sqlbuf[1024];
   	long lFlag,lKid;
   	int  iNum;
  
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64];
    long lId,lSid;
    char caPlate[128];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,7,
			             "del",           UT_TYPE_STRING,8,caDel,
			             "keyname",       UT_TYPE_STRING,30,caKeyname,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "del",UT_TYPE_STRING,10,caDel,
			             "plate",UT_TYPE_STRING,100,caPlate);

 
    iNum = 0;
    
     psDbHead = utPltInitDb();

    


		  	 psCur = pasDbOpenSqlF("select name,id,count(*) from ncssenurllog,ncwebclass where kid=id and flag=1 group by id  limit 0,1000");
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
	     memset(caName,0,sizeof(caName));
	     lId=0;
	     lCount=0;
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lId,
		                                   UT_TYPE_LONG,4,&lCount);
		                                
		
		 
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  		        
					   utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s_(%d)",caName,lCount);					    
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%ld",lId);
		         utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
		         utPltPutLoopVar(psDbHead,"cls", iNum,"file");
		         	
	    		 memset(caName,0,sizeof(caName));
	     		 lId=0;
		   		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lId);
		    }
		    pasDbCloseCursor(psCur);
		

    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");

    

    return 0;
}


//有害虚拟身份类别树
int ncsTreeNetid_yh_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[1024];
   	char sqlbuf[1024];
   	long lFlag,lKid;
   	int  iNum;
  
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64];
    long lId,lSid;
    char caPlate[128];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,7,
			             "del",           UT_TYPE_STRING,8,caDel,
			             "keyname",       UT_TYPE_STRING,30,caKeyname,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "del",UT_TYPE_STRING,10,caDel,
			             "plate",UT_TYPE_STRING,100,caPlate);

 
    iNum = 0;
    
     psDbHead = utPltInitDb();

    


		  	 psCur = pasDbOpenSqlF("select name,id,count(*) from ncssennetidlog,ncuseridclass where kid=id and flag=1 group by id  limit 0,1000");
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
	     memset(caName,0,sizeof(caName));
	     lId=0;
	     lCount=0;
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lId,
		                                   UT_TYPE_LONG,4,&lCount);
		                                
		
		 
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  		        
					   utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s_(%d)",caName,lCount);					    
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%ld",lId);
		         utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
		         utPltPutLoopVar(psDbHead,"cls", iNum,"file");
		         	
	    		 memset(caName,0,sizeof(caName));
	     		 lId=0;
		   		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lId);
		    }
		    pasDbCloseCursor(psCur);
		

    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");

    

    return 0;
}




//显示接入厂商代码维护
int ncsFacCode_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caTemp_d[256];
	  long lSid,lTid;
	  char caPre[36];
	  char caDispname[68],caMac[24];
	  long lStatus;
	  long iNum;
	   long lktype,lcaseflags,ltid;
	   char caFacs[1024];
	   char caDescr[64],caBh[32],caBm[32],caCh[32],caIdno[32];
	   char caDelall[16],caJoin[16];
	   char caSlink[36],caSaddress[256],caStel[68],caSmail[64];
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caCode[16],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	char caUsername2[36];
  memset(caUsername2,0,sizeof(caUsername2));
  dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);

   strcpy(caFacs,getDsFacs());
	  iReturn = utMsgGetSomeNVar(psMsgHead,14,
	                 "dispname",     UT_TYPE_STRING, 63, caDispname,
			             "limit",        UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "code",UT_TYPE_STRING,10,caCode,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "slink",UT_TYPE_STRING,31,caSlink,
			             "saddress",UT_TYPE_STRING,254,caSaddress,
			             "stel",UT_TYPE_STRING,63,caStel,
			             "smail",UT_TYPE_STRING,31,caSmail,
			             "start",UT_TYPE_STRING,10,caStart);
			printf("caUpdate=[%s],caSmail=[%s]\n",caUpdate,caSmail);
			  if(strlen(caDispname)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caDispname,caTemp,63)); 
	  	strcpy(caDispname,caTemp_d);
	  }	            
	  if(strlen(caSlink)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caSlink,caTemp,31)); 
	  	strcpy(caSlink,caTemp_d);
	  }
     if(strlen(caSaddress)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caSaddress,caTemp,127)); 
	  	strcpy(caSaddress,caTemp_d);
	  }
	  if(strlen(caSmail)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caSmail,caTemp,31)); 
	  	strcpy(caSmail,caTemp_d);
	  }
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caUpdate,"update")==0){
	  	if(strlen(caSid)==0){
	  		sprintf(caTemp,"select count(*) from ncsfactorycod where name='%s'  and code='%s' ",caDispname,caCode);
	  		lCount=0;
	  		printf("caTemp=%s\n",caTemp);
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
        if(lCount==0)
		{
        	printf("insert into ncsfactorycod(name,code,slink,saddress,stel,smail) values('%s','%s','%s','%s','%s','%s')\n",caDispname,caCode,caSlink,caSaddress,caStel,caSmail);
        
	  			 pasDbExecSqlF("insert into ncsfactorycod(name,code,slink,saddress,stel,smail) \
	       		values('%s','%s','%s','%s','%s','%s')",caDispname,caCode,caSlink,caSaddress,caStel,caSmail);
	       		pasDbExecSqlF("update ncsfactorycode set moditime=%d where code='%s' ",time(0),caCode);
	       		sprintf(caTemp,"添加接入厂商:%s",caDispname);
	       		ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caTemp);
	  		}
	  	}
	  	else
		{
	  		sprintf(caTemp,"select count(*) from ncsfactorycod where name='%s' and code='%s' and sid!=%s ",caDispname,caCode,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
 
	  				pasDbExecSqlF("update ncsfactorycod set name='%s',code='%s',slink='%s',saddress='%s',stel='%s',smail='%s' where sid=%s ",caDispname,caCode,caSlink,caSaddress,caStel,caSmail,caSid);
	  				pasDbExecSqlF("update ncsfactorycode set moditime=%d where sid=%s ",time(0),caSid);
	  				sprintf(caTemp,"修改接入厂商:%s",caDispname);
	       		ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caTemp);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("delete from  ncsfactorycod  where sid in(%s)",caSelsid);
	  	 sprintf(caTemp,"select dispname from ncsfactorycode where sid in (%s)",caSelsid);
	  	 memset(caDispname,0,sizeof(caDispname));
	  	 pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caDispname);
	  	
	  	sprintf(caTemp,"删除接入厂商:%s",caDispname);
	    ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caTemp);
	  }
	 
	  
	  
	  sprintf(caTemp,"select count(*) from ncsfactorycod where 1=1 ");
    if(strlen(caFacs)>0){
    	sprintf(caTemp+strlen(caTemp)," and code in (%s) ",caFacs);
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select sid,name,code,slink,saddress,stel,smail from ncsfactorycod where 1=1");
    if(strlen(caFacs)>0){
    	sprintf(caTemp+strlen(caTemp)," and code in (%s) ",caFacs);
    }
    	sprintf(caTemp+strlen(caTemp)," order by sid desc  limit %d,%d",lStartRec,lRowNum);
    	
  	printf("5537======caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {

    lSid=0;
    memset(caDispname,0,sizeof(caDispname));
    memset(caCode,0,sizeof(caCode));
    memset(caSlink,0,sizeof(caSlink));
    memset(caSaddress,0,sizeof(caSaddress));
    memset(caStel,0,sizeof(caStel));
    memset(caSmail,0,sizeof(caSmail));
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_STRING,63,caDispname,
      	                               UT_TYPE_STRING,10,caCode,
      	                               UT_TYPE_STRING,31,caSlink,
      	                               UT_TYPE_STRING,127,caSaddress,
      	                               UT_TYPE_STRING,31,caStel,
      	                               UT_TYPE_STRING,31,caSmail);
      	                    
      	                                  	                              
      	                                                                 	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
        utPltPutLoopVar(psDbHead,"code",iNum,caCode);
        utPltPutLoopVar(psDbHead,"slink",iNum,caSlink);
        utPltPutLoopVar(psDbHead,"saddress",iNum,caSaddress);
        utPltPutLoopVar(psDbHead,"stel",iNum,caStel);
        utPltPutLoopVar(psDbHead,"smail",iNum,caSmail);
       
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        

			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_faccode_list.htm");
	  return 0;
}



//测试是否登录，用作首页检测是否需要重新登录
int ncsTestRight_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  utPltDbHead *psDbHead;
	  unsigned long iLicense,lExpire,lUsers,lStart;
	  char *pProdSn;
	  psDbHead = utPltInitDb(); 
	  
	   pProdSn = utComGetVar_sd(psShmHead,"ProductSn","\0");
	
    iLicense = pasInstGetLicense(pProdSn,&lExpire,&lUsers,&lStart);

	  if(iLicense!=1){
	  	utPltPutVar(psDbHead,"mesg","true");
	  	utPltPutVarF(psDbHead,"userid","许可证已于%s到期,请及时更新许可证,否则会造成某些功能不正常。",utTimFormat("%Y-%m-%d %H:%M:%S",lExpire));
	  	utPltPutVar(psDbHead,"reason","license");
	  }
	  else{
	  utPltPutVar(psDbHead,"mesg","true");
	  utPltPutVar(psDbHead,"userid","ok");
	}
	  
	  
	  
	  	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_msg.htm");
	  return 0;
}



//当前布控信息树
int ncsTreeCase_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[4024];
   	int  iNum;
   	char sqlbuf[4024];
   	long lFlag,lKid;
    char *pHash;
    typedef struct ncsKeywordStat_s {
            long id;
            long sum;                //总记录数
            long csum;               //已处理记录数
   } ncsKeywordStat;
   ncsKeywordStat *psData;
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64];
    long lId,lSid,lStatus;
    char caStatus[16];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
    char caPlate[128];
    char caGroups[2048];
    strcpy(caGroups,getDsGroupid());
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,8,
			             "del",           UT_TYPE_STRING,8,caDel,
			             "keyname",       UT_TYPE_STRING,30,caKeyname,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,8,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "del",UT_TYPE_STRING,10,caDel,
			             "plate",UT_TYPE_STRING,100,caPlate);

    if(strlen(caPlate)>0){
    	pHash=(unsigned char *)pasLHashInit(1000,1000,sizeof(struct ncsKeywordStat_s),0,4); 
    	if(pHash==NULL){
    		return -1;
    	}
    	if(strlen(caGroups)>0){
    		 sprintf(sqlbuf,"select count(*),a.flags,a.cid from ncscasemesg a,ncscaseinfo b,ncsuser c where a.cid=b.sid and b.flags=1 and a.flags!=9 and a.did=c.userid and c.groupid in (%s) group by a.cid,a.flags ",caGroups);
    	}
    	else{
        sprintf(sqlbuf,"select count(*),a.flags,a.cid from ncscasemesg a,ncscaseinfo b where a.cid=b.sid and b.flags=1 and a.flags!=9 group by a.cid,a.flags ");
      }
      printf("sqlbuf=%s\n",sqlbuf);
    	psCur=pasDbOpenSql(sqlbuf,0);
    	if(psCur==NULL){
    		free(pHash);
    		return 0;
    	}
    	lCount=0;
    	lFlag=0;
    	lKid=0;
    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lFlag,
                                 UT_TYPE_LONG,4,&lKid);
                       
     while(iReturn==0||iReturn==1405)
     {

     	  psData = (struct ncsKeywordStat_s *)pasLHashLookA(pHash,&lKid);  
     	  if(psData){
     	  	if(lFlag>0){
     	  		psData->csum=psData->csum+lCount;
     	  	}
     	  	psData->sum=psData->sum+lCount;
     	  }
     	  
     	  
    	lCount=0;
    	lFlag=0;
    	lKid=0;
    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lFlag,
                                 UT_TYPE_LONG,4,&lKid);
     }
     pasDbCloseCursor(psCur); 
    	
    	
    }



 
 
 
    iNum = 0;
    
     psDbHead = utPltInitDb();

		  	 psCur = pasDbOpenSqlF("select casename,sid from ncscaseinfo where flags=1 order by casename limit 0,1000");
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
	     memset(caName,0,sizeof(caName));
	     lId=0;
	    
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lId);
		                                 
		                                
		
		 
		    while(iReturn == 0||iReturn==1405) {
		       psData = (struct ncsKeywordStat_s *)pasLHashLook(pHash,&lId);  
			     if(psData){
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		        
             utPltSetCvtHtml(1);  
			     	 utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s_(%d/%d)",caName,psData->csum,psData->sum);
					   	 
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%ld",lId);
		         utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
		         utPltPutLoopVar(psDbHead,"cls", iNum,"file");
		        }
	
				     memset(caName,0,sizeof(caName));
				     lId=0;
				    
					   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
					                                   UT_TYPE_LONG,4,&lId);
		    }
		    pasDbCloseCursor(psCur);
		
    
    	
    	
 //utPltShowDb(psDbHead);

        free(pHash);

    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");
    	 

    return 0;
}


//入库布控告警信息
int ncsTreeCase_yh_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[1024];
   	char sqlbuf[4024];
   	long lFlag,lKid;
   	int  iNum;
  
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64];
    long lId,lSid;
    char caPlate[128];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
    char caGroups[2048];
    strcpy(caGroups,getDsGroupid());
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,7,
			             "del",           UT_TYPE_STRING,8,caDel,
			             "keyname",       UT_TYPE_STRING,30,caKeyname,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "del",UT_TYPE_STRING,10,caDel,
			             "plate",UT_TYPE_STRING,100,caPlate);

 
    iNum = 0;
    
     psDbHead = utPltInitDb();

    if(strlen(caGroups)>0){
    	sprintf(sqlbuf,"select a.casename,a.sid,count(*) from ncscasemesg b,ncscaseinfo a,ncsuser c where b.cid=a.sid and b.did=c.userid and b.flags=1 and a.flags=1 and c.groupid in (%s) group by a.sid  limit 0,1000",caGroups);
    }
    else{
    	sprintf(sqlbuf,"select a.casename,a.sid,count(*) from ncscasemesg b,ncscaseinfo a where b.cid=a.sid and b.flags=1 and a.flags=1  group by a.sid  limit 0,1000");
    }

printf("sqlbuf=%s\n",sqlbuf);
		  	 psCur = pasDbOpenSqlF(sqlbuf);
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
	     memset(caName,0,sizeof(caName));
	     lId=0;
	     lCount=0;
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lId,
		                                   UT_TYPE_LONG,4,&lCount);
		                                
		
		 
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  		        
					   utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s_(%d)",caName,lCount);					    
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%ld",lId);
		         utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
		         utPltPutLoopVar(psDbHead,"cls", iNum,"file");
		         	
	    		 memset(caName,0,sizeof(caName));
	     		 lId=0;
		   		iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lId);
		    }
		    pasDbCloseCursor(psCur);
		

    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");

    

    return 0;
}

//显示场所类别代码维护
int ncsLbCode_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caTemp_d[128];
	  long lSid,lTid;
	  char caPre[36];
	  char caDispname[68],caMac[24];
	  long lStatus;
	  long iNum;
	   long lktype,lcaseflags,ltid;
	   char caDescr[64],caBh[32],caBm[32],caCh[32],caIdno[32];
	   char caDelall[16],caJoin[16];
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caCode[16],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);

	  iReturn = utMsgGetSomeNVar(psMsgHead,10,
	                 "dispname",     UT_TYPE_STRING, 31, caDispname,
			             "limit",        UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "code",UT_TYPE_STRING,10,caCode,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart);
			            
	  if(strlen(caDispname)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caDispname,caTemp,31)); 
	  	strcpy(caDispname,caTemp_d);
	  }
     
	
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caUpdate,"update")==0){
	  	if(strlen(caSid)==0){
	  		sprintf(caTemp,"select count(*) from ncsuser_lb where name='%s'  and code='%s' ",caDispname,caCode);
	  		lCount=0;
	  		printf("caTemp=%s\n",caTemp);
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
        if(lCount==0){
        	printf("insert into ncsuser_lb(name,code) values('%s','%s')\n",caDispname,caCode);
        
	  			 pasDbExecSqlF("insert into ncsuser_lb(name,code) \
	       		values('%s','%s')",caDispname,caCode);
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncsuser_lb where name='%s' and code='%s' and sid!=%s ",caDispname,caCode,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
 
	  				pasDbExecSqlF("update ncsuser_lb set name='%s',code='%s' where sid=%s ",caDispname,caCode,caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("delete from  ncsuser_lb  where sid in(%s)",caSelsid);
	  }
	 
	  
	  
	  sprintf(caTemp,"select count(*) from ncsuser_lb where 1=1 ");

    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select sid,name,code from ncsuser_lb where 1=1");

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
    memset(caDispname,0,sizeof(caDispname));
    memset(caCode,0,sizeof(caCode));
    
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_STRING,63,caDispname,
      	                               UT_TYPE_STRING,10,caCode);
      	                    
      	                                  	                              
      	                                                                 	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
        utPltPutLoopVar(psDbHead,"code",iNum,caCode);

       
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        

			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_faccode_list.htm");
	  return 0;
}
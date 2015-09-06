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
#include "utotype.h"
#include "utoshm01.h"
#include "pasutl.h"
#include "mail.h"
#include "pasfile.h"
#include <fcntl.h>
#include "pas_acbm.h"
#include "ncpost.h"
static  pattern_data  *psAcbmPat_keyword = NULL;      // 关键字模式内容
static  pattern_tree  *psAcbmTree_keyword = NULL;     // 关键字Acbm模式树

static  pattern_data  *psAcbmPat_url = NULL;      // URL模式内容
static  pattern_tree  *psAcbmTree_url = NULL;     // URLAcbm模式树

static  pattern_data  *psAcbmPat_netid = NULL;      // 虚拟身份模式内容
static  pattern_tree  *psAcbmTree_netid = NULL;     // 虚拟身份Acbm模式树

static  pattern_data  *psAcbmPat_user = NULL;      // 重点用户模式内容
static  pattern_tree  *psAcbmTree_user= NULL;     // 重点用户Acbm模式树
int ncLoadAcbm_keyword()
{
	pasDbCursor  *psCur;
	int iSumAcbmPat=0,i;
	int lSumRecord;
  char  caWord[128];
  char caTemp[1024];
  int iReturn,lTid;
  sprintf(caTemp,"select count(*) from ncskeycont,ncskeyindex where tid=id and flags!=0 ");
  iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSumRecord);
  if(iReturn!=0||lSumRecord==0){
  	return 0;
  }
  psAcbmPat_keyword=(pattern_data*)malloc(lSumRecord*sizeof(pattern_data));
  if(psAcbmPat_keyword==NULL){
  	return (-1);
  }
  memset(psAcbmPat_keyword,0,lSumRecord*sizeof(pattern_data));
  psCur=pasDbOpenSqlF("select content,tid from ncskeycont,ncskeyindex where tid=id and flags!=0 ");
  if(psCur==NULL){
  	free(psAcbmPat_keyword);
  	return (-1);
  }
  i=0;
  memset(caWord,0,sizeof(caWord));
  lTid=0;
  iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,64,caWord,
                               UT_TYPE_LONG,4,&lTid);
  while(iReturn==0||iReturn==1405){
  	if(strlen(caWord)>0){
  	   strcpy(psAcbmPat_keyword[i].data,caWord);
  	   psAcbmPat_keyword[i].len=strlen(caWord);
  	   psAcbmPat_keyword[i].flags=lTid;
  	   i++;
  	   }
  	  memset(caWord,0,sizeof(caWord));
  	  
		  lTid=0;
		  iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,64,caWord,
		                               UT_TYPE_LONG,4,&lTid);
  }
     iSumAcbmPat = i;


  pasDbCloseCursor(psCur);
  psAcbmTree_keyword = (pattern_tree *)pasAcbmInit(psAcbmPat_keyword,iSumAcbmPat);

    return iSumAcbmPat;

  
  
}

//加载URL库
int ncLoadAcbm_url()
{
	pasDbCursor  *psCur;
	int iSumAcbmPat=0,i;
	int lSumRecord;
  char  caWord[128];
  char caTemp[1024];
  int iReturn,lTid;
  sprintf(caTemp,"select count(*) from ncweblist a,ncwebclass b where a.id=b.id and a.flags=1 and status=1 ");
  iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSumRecord);
  if(iReturn!=0||lSumRecord==0){
  	return 0;
  }
  psAcbmPat_url=(pattern_data*)malloc(lSumRecord*sizeof(pattern_data));
  if(psAcbmPat_url==NULL){
  	return (-1);
  }
  memset(psAcbmPat_url,0,lSumRecord*sizeof(pattern_data));
  psCur=pasDbOpenSqlF("select a.url,a.id from ncweblist a,ncwebclass b where a.id=b.id and a.flags=1 and status=1 ");
  if(psCur==NULL){
  	free(psAcbmPat_url);
  	return (-1);
  }
  i=0;
  memset(caWord,0,sizeof(caWord));
  lTid=0;
  iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,64,caWord,
                               UT_TYPE_LONG,4,&lTid);
  while(iReturn==0||iReturn==1405){
  	if(strlen(caWord)>3){
  	   strcpy(psAcbmPat_url[i].data,caWord);
  	   psAcbmPat_url[i].len=strlen(caWord);
  	   psAcbmPat_url[i].flags=lTid;
  	   i++;
  	   }
  	  memset(caWord,0,sizeof(caWord));
  	  
		  lTid=0;
		  iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,64,caWord,
		                               UT_TYPE_LONG,4,&lTid);
  }
     iSumAcbmPat = i;


  pasDbCloseCursor(psCur);
  psAcbmTree_url = (pattern_tree *)pasAcbmInit(psAcbmPat_url,iSumAcbmPat);

    return iSumAcbmPat; 
}
//加载重点用户
int ncLoadAcbm_user()
{
	pasDbCursor  *psCur;
	int iSumAcbmPat=0,i;
	int lSumRecord,lSid;
  char  caDispname[32],caIdcard[20],caMac[20];
  char caTemp[1024];
  int iReturn,lTid;
  sprintf(caTemp,"select count(*) from ncszduser where status=1 ");
  iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSumRecord);
  if(iReturn!=0||lSumRecord==0){
  	return 0;
  }
  psAcbmPat_user=(pattern_data*)malloc(3*lSumRecord*sizeof(pattern_data));
  if(psAcbmPat_user==NULL){
  	return (-1);
  }
  memset(psAcbmPat_user,0,lSumRecord*sizeof(pattern_data));
  psCur=pasDbOpenSqlF("select sid,dispname,idcard,mac from ncszduser where status=1 ");
  if(psCur==NULL){
  	free(psAcbmPat_user);
  	return (-1);
  }
  i=0;
   lSid=0;
   memset(caDispname,0,sizeof(caDispname));
   memset(caIdcard,0,sizeof(caIdcard));
   memset(caMac,0,sizeof(caMac));
  iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lSid,
                               UT_TYPE_STRING,31,caDispname,
                               UT_TYPE_STRING,18,caIdcard,
                               UT_TYPE_STRING,19,caMac);
                              
  while(iReturn==0||iReturn==1405){
  	if(strlen(caDispname)>0){
  	   strcpy(psAcbmPat_user[i].data,caDispname);
  	   psAcbmPat_user[i].len=strlen(caDispname);
  	   psAcbmPat_user[i].flags=lSid;
  	 
  	   i++;
  	   }
  	   if(strlen(caIdcard)==15||strlen(caIdcard)==18){
  	   strcpy(psAcbmPat_user[i].data,caIdcard);
  	   psAcbmPat_user[i].len=strlen(caIdcard);
  	   psAcbmPat_user[i].flags=lSid;
  	
  	   i++;
  	   }
  	    if(strlen(caMac)==17){
  	   strcpy(psAcbmPat_user[i].data,caMac);
  	   psAcbmPat_user[i].len=strlen(caMac);
  	   psAcbmPat_user[i].flags=lSid;
  	 
  	   i++;
  	   }
  	   
	   lSid=0;
	   memset(caDispname,0,sizeof(caDispname));
	   memset(caIdcard,0,sizeof(caIdcard));
	   memset(caMac,0,sizeof(caMac));
	  iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lSid,
	                               UT_TYPE_STRING,31,caDispname,
	                               UT_TYPE_STRING,18,caIdcard,
	                               UT_TYPE_STRING,19,caMac);
  }
     iSumAcbmPat = i;


  pasDbCloseCursor(psCur);
  psAcbmTree_user = (pattern_tree *)pasAcbmInit(psAcbmPat_user,iSumAcbmPat);

    return iSumAcbmPat;

  
  
}


//加载虚拟身份库
int ncLoadAcbm_netid()
{
	pasDbCursor  *psCur;
	int iSumAcbmPat=0,i;
	int lSumRecord;
  char  caWord[128];
  char caTemp[1024];
  int iReturn,lTid,lNtype;
  sprintf(caTemp,"select count(*) from ncuseridlist a,ncuseridclass b where a.id=b.id  and status=1 ");
  iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSumRecord);
  if(iReturn!=0||lSumRecord==0){
  	return 0;
  }
  psAcbmPat_netid=(pattern_data*)malloc(lSumRecord*sizeof(pattern_data));
  if(psAcbmPat_url==NULL){
  	return (-1);
  }
  memset(psAcbmPat_netid,0,lSumRecord*sizeof(pattern_data));
  psCur=pasDbOpenSqlF("select a.uname,a.id,a.ntype from ncuseridlist a,ncuseridclass b where a.id=b.id  and status=1 ");
  if(psCur==NULL){
  	free(psAcbmPat_netid);
  	return (-1);
  }
  i=0;
  memset(caWord,0,sizeof(caWord));
  lTid=0;
  lNtype=0;
  iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,64,caWord,
                               UT_TYPE_LONG,4,&lTid,
                               UT_TYPE_LONG,4,&lNtype);
  while(iReturn==0||iReturn==1405){
  	if(strlen(caWord)>1){
  		memset(caTemp,0,sizeof(caTemp));
  		if(lNtype>0){
  			sprintf(caTemp,"%d%s",lNtype,caWord);
  		}
  		else{
  			strcpy(caTemp,caWord);
  	   
  	  }
  //	  printf("caTemp=%s\n",caTemp);
  	  strcpy(psAcbmPat_netid[i].data,caTemp);
  	   psAcbmPat_netid[i].len=strlen(caTemp);
  	   psAcbmPat_netid[i].flags=lTid;
  	   i++;
  	   }
  	  memset(caWord,0,sizeof(caWord));
  	  
		  lTid=0;
		  lNtype=0;
		  iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,64,caWord,
		                               UT_TYPE_LONG,4,&lTid,
		                               UT_TYPE_LONG,4,&lNtype);
  }
     iSumAcbmPat = i;


  pasDbCloseCursor(psCur);
  psAcbmTree_netid = (pattern_tree *)pasAcbmInit(psAcbmPat_netid,iSumAcbmPat);

    return iSumAcbmPat; 
}

char *ncsHtmlToText(char *in){
	char *pOut;
	char *p;
	int i;
	long iLen,iCount,index,preflag,flag,precount;
	if(strlen(in)==0)
	   return NULL;
	

	

	i=0;
	iLen=strlen(in);
	
	pOut=malloc(iLen+4000);
	if(pOut==NULL){
		return NULL;
	}
	memset(pOut,0,sizeof(iLen+4000));
	index=0;
	preflag=0;
	flag=0;
	p=utStrSkipSpaces(in);
	iCount=0;
	precount=0;
	while(*p){
		if(i>iLen) break;
		if(*p=='<'&&preflag!=1){
			iCount=1;
			
			flag=1;
			precount=0;
		}
		else if(*p=='<'&&preflag==1){
			memcpy(pOut+index,p-precount,precount);
			index=index+precount;
			iCount=1;
			flag=1;
			precount=0;
		}
		else if(*p=='>'){
			iCount=0;
			flag=2;
			precount=0;
		}

		
		precount++;

		if(iCount%2==0){
			if(*p!='>'&&*p!='<'){
			   pOut[index]=*p;
			   index++;
			   precount=0;
		   }
		  }
		  preflag=flag;
	    p++;
	    i++;
//	 p = utStrSkipSpaces(p);
	}
	pOut[index]='\0';
	//if(*(p-1)!='>'&&preflag==1){
//		memcpy(pOut+index,p-precount-1,precount);
//	}
	
	
	return pOut;
}

int ncsCheckKeyword(utShmHead *psShmHead)
{
	pasDbCursor *psCur;
	long iSumkey;
	long iReturn,lTime;
	char caTemp[20000];
	char caTableday[20];
	 struct data_s{
	 	char caDispname[32];
	 	char caMac[20];
	 	char caPosition[32];
	 	char caIdcard[20];
	 	char caIdtype[4];
	 	long did;
	 	long uid;
	 	long tid;
	 	char caSip[16];
	 	long vtime;
	 	char caKeyword[64];
	 	char caSubject[256];
	 	long service;
	 	char caContent[10000];
	 	char caUrl[200];
	 	long long fileid;
	 	char caDesc[256];
	};
  struct data_s *pData;
	long long lLastid;
	long lCount;
	long lSum_keylog;
	char caDispname[32],caMac[20],caPosition[32],caIdno[20],caIdtype[8];
	long lDid,lUid,lStime,lRep;
	char caSip[16],caUrl[128],caSubject[512],caContent[10000],caTemp_d[10000];
	long long lSid;
	long int psMatch[8];
  int iFind,i;
  long lService,lTid;
  long iFlags;
  char *p;
  int iSize;
  char caMfrom[48],caMto[48],caTempDir[128],caFile2[128],caFile1[128];
  char caMailfile[256],caTable[64];
  mail_body *psMailBody;
    ncPostHead  *psHead;
    pasFile     *ps;
    ncPostCont *psCont;
  mail_header_info *psMailHead;
  char *tmpgbk;
  long long lFileid,len;
  char caHost[72];
	iReturn = pasConnect(psShmHead);
	if(iReturn<0){
		sleep(60);
  	return 0;
  }
  iSumkey=ncLoadAcbm_keyword();
  

  sprintf(caMailfile,"/home/ncmysql/ncs/log/tmp");
   if(!utFileIsExist(caMailfile)){
   	system("mkdir /home/ncmysql/ncs/log/tmp ");
  }
  
  while(1){


  	if(iSumkey>0){    		                                                 //对关键字进行过滤
  		  lTime=time(0);
			  strcpy(caTableday,utTimFormat("%Y%m",lTime));
			  pData=(struct data_s*)malloc((1002)*sizeof(struct data_s));
			  if(pData==NULL){
			  	free(psAcbmPat_keyword);
			  	sleep(60);
			  	return (-1);
			  }
			  
			  
			  ncsUtlGetLastInfo_v4("bbsidSearch",&lLastid);
			  sprintf(caTemp,"select dispname,mac,position,idno,idtype,did,uid,stime,sip,url,subject,content,sid,tid from ncsbbsmon_if_%s where sid>%llu order by sid limit 0,1000 ", caTableday,lLastid);
	//		  printf("caTemp=%s\n",caTemp);
			  

		
			  psCur = pasDbOpenSql(caTemp,0);
        if(psCur){
        	memset(caDispname,0,sizeof(caDispname));
        	memset(caMac,0,sizeof(caMac));
        	memset(caPosition,0,sizeof(caPosition));
        	memset(caIdno,0,sizeof(caIdno));
        	memset(caIdtype,0,sizeof(caIdtype));
        	lDid=0;
        	lUid=0;
        	lStime=0;
        	memset(caSip,0,sizeof(caSip));
        	memset(caUrl,0,sizeof(caUrl));
        	memset(caSubject,0,sizeof(caSubject));
        	memset(caContent,0,sizeof(caContent));
        	lSid=0;
        	lLastid=0;
        	lTid=0;
        	iReturn = pasDbFetchInto(psCur,  
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_STRING,127,caUrl,
                                     UT_TYPE_STRING,255,caSubject,
                                     UT_TYPE_STRING,8999,caContent,
                                     UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_LONG,4,&lTid);
            lSum_keylog=0; 
            lRep=0;                     
            while((iReturn==0)||(iReturn==1405)){
            	          
            if(lSum_keylog==0){
            	lRep=1;
            }
            else{
            	if(lDid==pData[lSum_keylog-1].did&&lUid==pData[lSum_keylog-1].uid&&strcmp(caSubject,pData[lSum_keylog-1].caSubject)==0&&strcmp(caContent,pData[lSum_keylog-1].caContent)==0){
            		lRep=0;
            	}
            	else{
            		lRep=1;
            	}
            }
            	if(lRep==1){
            		strcpy(pData[lSum_keylog].caDispname,caDispname);
            		strcpy(pData[lSum_keylog].caMac,caMac);
            		strcpy(pData[lSum_keylog].caPosition,caPosition);
            		strcpy(pData[lSum_keylog].caIdcard,caIdno);
            		strcpy(pData[lSum_keylog].caIdtype,caIdtype);
            		pData[lSum_keylog].did=lDid;
            		pData[lSum_keylog].uid=lUid;
            		pData[lSum_keylog].tid=lTid;
            		strcpy(pData[lSum_keylog].caSip,caSip);
            		pData[lSum_keylog].vtime=lStime;
            		strcpy(pData[lSum_keylog].caSubject,caSubject);
            		strcpy(pData[lSum_keylog].caContent,caContent);
            		strcpy(pData[lSum_keylog].caUrl,caUrl);
            		lSum_keylog++;
            	}
            	
            	          lLastid=lSid;
            	        	memset(caDispname,0,sizeof(caDispname));
							        	memset(caMac,0,sizeof(caMac));
							        	memset(caPosition,0,sizeof(caPosition));
							        	memset(caIdno,0,sizeof(caIdno));
							        	memset(caIdtype,0,sizeof(caIdtype));
							        	lDid=0;
							        	lUid=0;
							        	lStime=0;
							        	memset(caSip,0,sizeof(caSip));
							        	memset(caUrl,0,sizeof(caUrl));
							        	memset(caSubject,0,sizeof(caSubject));
							        	memset(caContent,0,sizeof(caContent));
							        	lSid=0;
							        	lTid=0;
							        	iReturn = pasDbFetchInto(psCur,  
							                                     UT_TYPE_STRING,31,caDispname,
							                                     UT_TYPE_STRING,19,caMac,
							                                     UT_TYPE_STRING,31,caPosition,
							                                     UT_TYPE_STRING,18,caIdno,
							                                     UT_TYPE_STRING,3,caIdtype,
							                                     UT_TYPE_LONG,4,&lDid,
							                                     UT_TYPE_LONG,4,&lUid,
							                                     UT_TYPE_LONG,4,&lStime,
							                                     UT_TYPE_STRING,15,caSip,
							                                     UT_TYPE_STRING,127,caUrl,
							                                     UT_TYPE_STRING,255,caSubject,
							                                     UT_TYPE_STRING,8999,caContent,
							                                     UT_TYPE_LONG8,8,&lSid,
							                                     UT_TYPE_LONG,4,&lTid);
            	
            }
            pasDbCloseCursor(psCur);
            if(lLastid>0&&lSum_keylog>0){
            	ncsUtlSetLastInfo_v4("bbsidSearch",lLastid); 
            }
        	
        	for(i=0;i<lSum_keylog;i++){
        		iFind=0;
        		if(strlen(pData[i].caSubject)>0){
    	         strcpy(caTemp_d,pasCvtGBK(2,pData[i].caSubject,caSubject,255));
    	         strcpy(caSubject,pasDbReplaceQuote1(caTemp_d));
               iFind = pasAcbmSearch(psAcbmTree_keyword,caTemp_d,strlen(caTemp_d),psMatch,1);
              }
            else{
            	strcpy(caSubject,"");
            }
              if(strlen(pData[i].caContent)>0){
               	    strcpy(caTemp_d,pasCvtGBK(2,pData[i].caContent,caContent,8999));
               	    strcpy(caContent,pasDbReplaceQuote1(caTemp_d));
               	    if(iFind<=0){
               	    iFind = pasAcbmSearch(psAcbmTree_keyword,caTemp_d,strlen(caTemp_d),psMatch,1);
               	  }
              }
              else{
              	strcpy(caContent,"");
              }
              
               if(iFind>0){

               	    sprintf(caTemp,"select count(*) from ncskeywordlog where did=%d and uid=%d and subject ='%s' and content='%s' and vtime>%d and vtime<%d ",
               	        pData[i].did,pData[i].uid,caSubject,caContent,pData[i].vtime-1800,pData[i].vtime+1800);
          
               	        lCount=0;
               	        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount); 
               	        if(lCount==0){
               //	        	printf("insert into ncskeywordlog (did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,subject,content,url,type) \
               //	        	 values(%d,%d,'%s','%s','%s','%s','%s','%s',%d,%d,'%s','%s','%s','%s',2)\n", \
               	//        	 pData[i].did,pData[i].uid,pData[i].caDispname,pData[i].caIdcard,pData[i].caMac,pData[i].caIdtype,pData[i].caPosition,pData[i].caSip,pData[i].vtime,psAcbmPat_keyword[psMatch[0]].flags,psAcbmPat_keyword[psMatch[0]].data,caSubject,caContent,pData[i].caUrl);
             	            sprintf(caTemp,"insert into ncskeywordlog (did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,subject,content,url,type,tid) \
               	        	 values(%d,%d,'%s','%s','%s','%s','%s','%s',%d,%d,'%s','%s','%s','%s',2,%d)", \
               	        	 pData[i].did,pData[i].uid,pData[i].caDispname,pData[i].caIdcard,pData[i].caMac,pData[i].caIdtype,pData[i].caPosition,pData[i].caSip,pData[i].vtime,psAcbmPat_keyword[psMatch[0]].flags,psAcbmPat_keyword[psMatch[0]].data,caSubject,caContent,pData[i].caUrl,pData[i].tid); 	 
               	       
                  	      iSize=strlen(caTemp)+256;       	               	        	             	        	 
               	        	pasDbExecSqlFn(caTemp,iSize);         	        	 
               	        } 
               	    
               	 
               
               }

             
        		}
        		
        	}
//bbs搜索结束  



//搜索即时通讯
 			  ncsUtlGetLastInfo_v4("ImidSearch",&lLastid);
			  sprintf(caTemp,"select dispname,mac,position,idno,idtype,did,uid,stime,sip,service,mfrom,mto,mesg,sid,tid from ncsimlog_if_%s where sid>%llu order by sid limit 0,1000 ", caTableday,lLastid);
//		  printf("caTemp=%s\n",caTemp);
			  		
			  psCur = pasDbOpenSql(caTemp,0);
        if(psCur){
        	memset(caDispname,0,sizeof(caDispname));
        	memset(caMac,0,sizeof(caMac));
        	memset(caPosition,0,sizeof(caPosition));
        	memset(caIdno,0,sizeof(caIdno));
        	memset(caIdtype,0,sizeof(caIdtype));
        	lDid=0;
        	lUid=0;
        	lStime=0;
        	memset(caSip,0,sizeof(caSip));
        	lService=0;
        	memset(caMfrom,0,sizeof(caMfrom));
        	memset(caMto,0,sizeof(caMto));
        	memset(caContent,0,sizeof(caContent));
        	lSid=0;
        	lLastid=0;
        	lTid=0;
        	iReturn = pasDbFetchInto(psCur,  
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_LONG,4,&lService,
                                     UT_TYPE_STRING,47,caMfrom,
                                     UT_TYPE_STRING,47,caMto,
                                     UT_TYPE_STRING,8999,caContent,
                                     UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_LONG,4,&lTid);
            lSum_keylog=0; 
            lRep=0;                     
            while((iReturn==0)||(iReturn==1405)){
            	          
            if(lSum_keylog==0){
            	lRep=1;
            }
            else{
            	if(lDid==pData[lSum_keylog-1].did&&lUid==pData[lSum_keylog-1].uid&&strcmp(caContent,pData[lSum_keylog-1].caContent)==0){
            		lRep=0;
            	}
            	else{
            		lRep=1;
            	}
            }
            	if(lRep==1){
            		strcpy(pData[lSum_keylog].caDispname,caDispname);
            		strcpy(pData[lSum_keylog].caMac,caMac);
            		strcpy(pData[lSum_keylog].caPosition,caPosition);
            		strcpy(pData[lSum_keylog].caIdcard,caIdno);
            		strcpy(pData[lSum_keylog].caIdtype,caIdtype);
            		pData[lSum_keylog].did=lDid;
            		pData[lSum_keylog].uid=lUid;
            		pData[lSum_keylog].tid=lTid;
            		strcpy(pData[lSum_keylog].caSip,caSip);
            		pData[lSum_keylog].service=lService;
            		pData[lSum_keylog].vtime=lStime;
            		sprintf(pData[lSum_keylog].caSubject,"发送者:%s<==>接收者:%s",caMfrom,caMto);
            		strcpy(pData[lSum_keylog].caContent,caContent);
            		sprintf(pData[lSum_keylog].caUrl,"/pronline/Msg?FunName@ncsWebReplace&plate@v4/userbh/im_det_frm.htm&mfrom@%s&mto@%s&service@%d",caMfrom,caMto,lService);
            		lSum_keylog++;
            	}
            	
            	    lLastid=lSid;
				        	memset(caDispname,0,sizeof(caDispname));
				        	memset(caMac,0,sizeof(caMac));
				        	memset(caPosition,0,sizeof(caPosition));
				        	memset(caIdno,0,sizeof(caIdno));
				        	memset(caIdtype,0,sizeof(caIdtype));
				        	lDid=0;
				        	lUid=0;
				        	lStime=0;
				        	memset(caSip,0,sizeof(caSip));
				        	lService=0;
				        	memset(caMfrom,0,sizeof(caMfrom));
				        	memset(caMto,0,sizeof(caMto));
				        	memset(caContent,0,sizeof(caContent));
				        	lSid=0;
				        	lTid=0;
            	        	iReturn = pasDbFetchInto(psCur,  
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_LONG,4,&lService,
                                     UT_TYPE_STRING,47,caMfrom,
                                     UT_TYPE_STRING,47,caMto,
                                     UT_TYPE_STRING,8999,caContent,
                                     UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_LONG,4,&lTid);
            }
            pasDbCloseCursor(psCur);
            if(lLastid>0&&lSum_keylog>0){
            	ncsUtlSetLastInfo_v4("ImidSearch",lLastid); 
            }
        	
        	for(i=0;i<lSum_keylog;i++){
        		iFind=0;
             strcpy(caSubject,pData[i].caSubject);
              if(strlen(pData[i].caContent)>0){
                  strcpy(caContent,pData[i].caContent);
               	  iFind = pasAcbmSearch(psAcbmTree_keyword,caContent,strlen(caContent),psMatch,1);
               	 strcpy(caContent,pasDbReplaceQuote1(pData[i].caContent));
              }
              else{
              	strcpy(caContent,"");
              }
              
               if(iFind>0){

               	    sprintf(caTemp,"select count(*) from ncskeywordlog where did=%d and uid=%d and subject ='%s' and content='%s' and vtime>%d and vtime<%d ",
               	        pData[i].did,pData[i].uid,caSubject,caContent,pData[i].vtime-1800,pData[i].vtime+1800);
          
               	        lCount=0;
               	        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount); 
               	        if(lCount==0){
               //	        	printf("insert into ncskeywordlog (did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,subject,content,url,type) \
               //	        	 values(%d,%d,'%s','%s','%s','%s','%s','%s',%d,%d,'%s','%s','%s','%s',2)\n", \
               	//        	 pData[i].did,pData[i].uid,pData[i].caDispname,pData[i].caIdcard,pData[i].caMac,pData[i].caIdtype,pData[i].caPosition,pData[i].caSip,pData[i].vtime,psAcbmPat_keyword[psMatch[0]].flags,psAcbmPat_keyword[psMatch[0]].data,caSubject,caContent,pData[i].caUrl);
               	        	 
               	        	pasDbExecSqlF("insert into ncskeywordlog (did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,subject,content,url,type,tid) \
               	        	 values(%d,%d,'%s','%s','%s','%s','%s','%s',%d,%d,'%s','%s','%s','%s',%d,%d)", \
               	        	 pData[i].did,pData[i].uid,pData[i].caDispname,pData[i].caIdcard,pData[i].caMac,pData[i].caIdtype,pData[i].caPosition,pData[i].caSip,pData[i].vtime,psAcbmPat_keyword[psMatch[0]].flags,psAcbmPat_keyword[psMatch[0]].data,caSubject,caContent,pData[i].caUrl,pData[i].service,pData[i].tid);
               	        	 
               	        } 
               	    
               	 
               
               }

             
        		}
        		
        	}
//即时通讯搜索结束 


 
 //搜索邮件
  			ncsUtlGetLastInfo_v4("MailidSearch",&lLastid);
  			sprintf(caTable,"ncsmailmon_if_%s",caTableday);
			  sprintf(caTemp,"select dispname,mac,position,idno,idtype,did,uid,stime,sip,mailfrom,mailto,mailsub,sid,tid,fileid from ncsmailmon_if_%s where sid>%llu order by sid limit 0,1000 ", caTableday,lLastid);
	//	  printf("caTemp=%s\n",caTemp);
			  

		
			  psCur = pasDbOpenSql(caTemp,0);
        if(psCur){
        	memset(caDispname,0,sizeof(caDispname));
        	memset(caMac,0,sizeof(caMac));
        	memset(caPosition,0,sizeof(caPosition));
        	memset(caIdno,0,sizeof(caIdno));
        	memset(caIdtype,0,sizeof(caIdtype));
        	lDid=0;
        	lUid=0;
        	lStime=0;
        	memset(caSip,0,sizeof(caSip));
        	lService=0;
        	memset(caMfrom,0,sizeof(caMfrom));
        	memset(caMto,0,sizeof(caMto));
        	memset(caSubject,0,sizeof(caSubject));
        	lSid=0;
        	lLastid=0;
        	lTid=0;
        	lFileid=0;
        	iReturn = pasDbFetchInto(psCur,  
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_STRING,47,caMfrom,
                                     UT_TYPE_STRING,47,caMto,
                                     UT_TYPE_STRING,255,caSubject,
                                     UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_LONG,4,&lTid,
                                     UT_TYPE_LONG8,8,&lFileid);
            lSum_keylog=0; 
            lRep=0;                     
            while((iReturn==0)||(iReturn==1405)){
            	          
            if(lSum_keylog==0){
            	lRep=1;
            }
            else{
            	if(lDid==pData[lSum_keylog-1].did&&lUid==pData[lSum_keylog-1].uid&&strcmp(caSubject,pData[lSum_keylog-1].caSubject)==0){
            		lRep=0;
            	}
            	else{
            		lRep=1;
            	}
            }
            	if(lRep==1){
            		strcpy(pData[lSum_keylog].caDispname,caDispname);
            		strcpy(pData[lSum_keylog].caMac,caMac);
            		strcpy(pData[lSum_keylog].caPosition,caPosition);
            		strcpy(pData[lSum_keylog].caIdcard,caIdno);
            		strcpy(pData[lSum_keylog].caIdtype,caIdtype);
            		pData[lSum_keylog].did=lDid;
            		pData[lSum_keylog].uid=lUid;
            		pData[lSum_keylog].tid=lTid;
            		strcpy(pData[lSum_keylog].caSip,caSip);
            		pData[lSum_keylog].vtime=lStime;
            		strcpy(pData[lSum_keylog].caSubject,caSubject);
            		pData[lSum_keylog].fileid=lFileid;
            		sprintf(pData[lSum_keylog].caDesc,"发件人:%s<==>收件人:%s",caMfrom,caMto);
        //    		strcpy(pData[lSum_keylog].caContent,caContent);
            		sprintf(pData[lSum_keylog].caUrl,"/pronline/Msg?FunName@ncsShowOneMail_v4&table@%s&mailfile@%llu",caTableday,lFileid);
            		lSum_keylog++;
            	}
            	
            	    lLastid=lSid;
				        	memset(caDispname,0,sizeof(caDispname));
				        	memset(caMac,0,sizeof(caMac));
				        	memset(caPosition,0,sizeof(caPosition));
				        	memset(caIdno,0,sizeof(caIdno));
				        	memset(caIdtype,0,sizeof(caIdtype));
				        	lDid=0;
				        	lUid=0;
				        	lStime=0;
				        	memset(caSip,0,sizeof(caSip));
				        	lService=0;
				        	memset(caMfrom,0,sizeof(caMfrom));
				        	memset(caMto,0,sizeof(caMto));
				        	memset(caContent,0,sizeof(caContent));
				        	lSid=0;
				        	lTid=0;
        	lFileid=0;
        	iReturn = pasDbFetchInto(psCur,  
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_STRING,47,caMfrom,
                                     UT_TYPE_STRING,47,caMto,
                                     UT_TYPE_STRING,255,caSubject,
                                     UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_LONG,4,&lTid,
                                     UT_TYPE_LONG8,8,&lFileid);
                                     
                            //         printf("lFileid=%llu",lFileid);
            }
            pasDbCloseCursor(psCur);
        
            if(lLastid>0&&lSum_keylog>0){
            	ncsUtlSetLastInfo_v4("MailidSearch",lLastid); 
            }
        	
        	for(i=0;i<lSum_keylog;i++){
 //       		sleep(20);
        		iFind=0;
        		   sprintf(caMailfile,"/home/ncmysql/ncs/log/tmp/%llu",pData[i].fileid);
        		 
        		   iReturn=ncsUtlCvtNcsText2File(caTable,caMailfile,pData[i].fileid);

        		   memset(caContent,0,sizeof(caContent));
        		   memset(caSubject,0,sizeof(caSubject));
        		   if(iReturn==1){
        		   	psMailBody == NULL;
        		//   	  	psMailHead = mail_header(caMailfile,&iFlags);
  		              sprintf(caTempDir,"/home/ncmysql/ncs/log/tmp/1111");
  		               if(utFileIsExist(caTempDir)!=1) {
								        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
									//	       mailHeadFree(psMailHead);
								            break;
								        }
						        }
						        sprintf(caFile1,"%s/%llu",caTempDir,pData[i].fileid);
                    strcpy(caFile2,caMailfile);

						        psMailBody = mail_getbody(caFile2,caTempDir,caFile1,NULL,&iFlags);

								    if(psMailBody){
								     tmpgbk=NULL;

						        	if(psMailBody->textcon){
//						        	  html2text(psMailBody->textcon);

						        	  tmpgbk = my_convert(psMailBody->textcon);

						        	  if(tmpgbk){
		//				        	   html2text(tmpgbk);		
	                         p=ncsHtmlToText(tmpgbk);
	                         if(p){
	                         	 len=strlen(p);
	                         	 if(len>8000){
								               strncpy(caContent,p,8000);
								              }
								              else{
								              	strcpy(caContent,p);
								              }

								             free(p);

						               }
							        			free(tmpgbk);	

							        			}
						        	 }
						        else{

						        	if(psMailBody->htmlcon) {
 		 
						        		 tmpgbk=my_convert(psMailBody->htmlcon);
						        		 if(tmpgbk){
	//					        		 html2text(tmpgbk);

	                         p=ncsHtmlToText(tmpgbk);
	                         if(p){
	                         	 len=strlen(p);
	                         	 if(len>8000){
								               strncpy(caContent,p,8000);
								              }
								              else{
								              	strcpy(caContent,p);
								              }

								             free(p);

						               }

							        	 free(tmpgbk);	

						        	}
						        }
						        

                   }
						  //      	 mailHeadFree(psMailHead);

                    freeBody(psMailBody);

               }
						        remove(caFile1);
						        remove(caFile2);
						        
						      }

						      memset(caSubject,0,sizeof(caSubject));
						      tmpgbk=my_convert(pData[i].caSubject);
						      if(tmpgbk){
						     	 	strcpy(caSubject,tmpgbk);
						      	free(tmpgbk);
						      }

						      sprintf(caTemp_d,"%s%s",caContent,caSubject);
	//			printf("caCont=%s\n",caContent);		      
						      iFind = pasAcbmSearch(psAcbmTree_keyword,caTemp_d,strlen(caTemp_d),psMatch,1);

               if(iFind>0){
                  strcpy(caTemp_d,pasDbReplaceQuote1(caContent));
                  strcpy(caContent,caTemp_d);
                  strcpy(caTemp_d,pasDbReplaceQuote1(caSubject));
                  strcpy(caSubject,caTemp_d);


               	    sprintf(caTemp,"select count(*) from ncskeywordlog where did=%d and uid=%d and subject ='%s'  and vtime>%d and vtime<%d ",
               	        pData[i].did,pData[i].uid,caSubject,pData[i].vtime-1800,pData[i].vtime+1800);

               	        lCount=0;
               	        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount); 
               	        if(lCount==0){
     //       printf("sid=%llu\n",pData[i].sid);
  	 
              	sprintf(caTemp,"insert into ncskeywordlog (did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,subject,content,url,type,tid,descr) \
               	        	 values(%d,%d,'%s','%s','%s','%s','%s','%s',%d,%d,'%s','%s','%s','%s',%d,%d,'%s')", \
               	        	 pData[i].did,pData[i].uid,pData[i].caDispname,pData[i].caIdcard,pData[i].caMac,pData[i].caIdtype,pData[i].caPosition,pData[i].caSip,pData[i].vtime,psAcbmPat_keyword[psMatch[0]].flags,psAcbmPat_keyword[psMatch[0]].data,caSubject,caContent,pData[i].caUrl,1,pData[i].tid,pData[i].caDesc);  	 
              	           iSize=strlen(caTemp)+256;       	 
              	        	             	        	 
               	        	pasDbExecSqlFn(caTemp,iSize);
               	        } 
       	    
               	 
               
               }

             }
             if(lSum_keylog>0){
             	  system("rm /home/ncmysql/ncs/log/tmp/1111/*");
             }
        		}
        		
        		
//邮件搜索结束
 
 
 
  //搜索表单
  			ncsUtlGetLastInfo_v4("FormidSearch",&lLastid);
  			sprintf(caTable,"ncsformmon_if_%s",caTableday);
			  sprintf(caTemp,"select dispname,mac,position,idno,idtype,did,uid,stime,sip,host,url,sid,tid,fileid from ncsformmon_if_%s where sid>%llu order by sid limit 0,1000 ", caTableday,lLastid);
//		  printf("caTemp=%s\n",caTemp);
			  

		
			  psCur = pasDbOpenSql(caTemp,0);
        if(psCur){
        	memset(caDispname,0,sizeof(caDispname));
        	memset(caMac,0,sizeof(caMac));
        	memset(caPosition,0,sizeof(caPosition));
        	memset(caIdno,0,sizeof(caIdno));
        	memset(caIdtype,0,sizeof(caIdtype));
        	lDid=0;
        	lUid=0;
        	lStime=0;
        	memset(caSip,0,sizeof(caSip));
        	memset(caHost,0,sizeof(caHost));
        	memset(caUrl,0,sizeof(caUrl));
        	lSid=0;
        	lLastid=0;
        	lTid=0;
        	lFileid=0;
        	iReturn = pasDbFetchInto(psCur,  
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_STRING,63,caHost,
                                     UT_TYPE_STRING,63,caUrl,
                                     UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_LONG,4,&lTid,
                                     UT_TYPE_LONG8,8,&lFileid);
            lSum_keylog=0; 
            lRep=0;                     
            while((iReturn==0)||(iReturn==1405)){
            	          
            if(lSum_keylog==0){
            	lRep=1;
            }
            else{
            	if(lDid==pData[lSum_keylog-1].did&&lUid==pData[lSum_keylog-1].uid&&pData[lSum_keylog-1].fileid==lFileid){
            		lRep=0;
            	}
            	else{
            		lRep=1;
            	}
            }
            	if(lRep==1){
            		strcpy(pData[lSum_keylog].caDispname,caDispname);
            		strcpy(pData[lSum_keylog].caMac,caMac);
            		strcpy(pData[lSum_keylog].caPosition,caPosition);
            		strcpy(pData[lSum_keylog].caIdcard,caIdno);
            		strcpy(pData[lSum_keylog].caIdtype,caIdtype);
            		pData[lSum_keylog].did=lDid;
            		pData[lSum_keylog].uid=lUid;
            		pData[lSum_keylog].tid=lTid;
            		strcpy(pData[lSum_keylog].caSip,caSip);
            		pData[lSum_keylog].vtime=lStime;
  //          		strcpy(pData[lSum_keylog].caSubject,caSubject);
            		pData[lSum_keylog].fileid=lFileid;
                strcpy(pData[lSum_keylog].caDesc,caHost);
        //    		strcpy(pData[lSum_keylog].caContent,caContent);
            		sprintf(pData[lSum_keylog].caUrl,"http://%s%s",caHost,caUrl);
            		lSum_keylog++;
            	}
            	
            	    lLastid=lSid;
				        	memset(caDispname,0,sizeof(caDispname));
				        	memset(caMac,0,sizeof(caMac));
				        	memset(caPosition,0,sizeof(caPosition));
				        	memset(caIdno,0,sizeof(caIdno));
				        	memset(caIdtype,0,sizeof(caIdtype));
				        	lDid=0;
				        	lUid=0;
				        	lStime=0;
				        	memset(caSip,0,sizeof(caSip));
				        	lService=0;
				          memset(caHost,0,sizeof(caHost));
				          memset(caUrl,0,sizeof(caUrl));
				        	lSid=0;
				        	lTid=0;
        	lFileid=0;
        	iReturn = pasDbFetchInto(psCur,  
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_STRING,63,caHost,
                                     UT_TYPE_STRING,63,caUrl,
                                     UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_LONG,4,&lTid,
                                     UT_TYPE_LONG8,8,&lFileid);
                                     
            }
            pasDbCloseCursor(psCur);
        
            if(lLastid>0&&lSum_keylog>0){
            	ncsUtlSetLastInfo_v4("FormidSearch",lLastid); 
            }
        	
        	for(i=0;i<lSum_keylog;i++){
 //       		sleep(20);
        		iFind=0;
        		   sprintf(caMailfile,"/home/ncmysql/ncs/log/tmp/%llu",pData[i].fileid);
        		 
        		   iReturn=ncsUtlCvtNcsText2File(caTable,caMailfile,pData[i].fileid);

        		   memset(caContent,0,sizeof(caContent));
        		   memset(caSubject,0,sizeof(caSubject));
        		   if(iReturn==1){
        		   	 ps = pasOpenFile(caMailfile);
        		   	 if(ps){
        		   	 	psHead = ncPostGetPostData(ps);
        		   	 	 while(psHead) {
        		   	 	 	  psCont = psHead->psCont;
        		   	 	 	  while(psCont) {
        		   	 	 	  	if(psCont->pValue && !utStrIsSpaces(psCont->pValue)) {
        		   	 	 	  //		printf("value=%s\n",psCont->pValue);
        		   	 	 	  		iReturn=pasCharIsUtf8(psCont->pValue, strlen(psCont->pValue));
        		   	 	 	  		if(iReturn){
        		   	 	 	  			pasUtf8ToGBK(psCont->pValue,caTemp,8000);
        		   	 	 	  		}
        		   	 	 	  		else{
        		   	 	 	  			memset(caTemp,0,sizeof(caTemp));
        		   	 	 	  			strncpy(caTemp,psCont->pValue,8000);
        		   	 	 	  		}
        		   	 	 	  		iFind=0;
        		   	 	 	  		 iFind = pasAcbmSearch(psAcbmTree_keyword,caTemp,strlen(caTemp),psMatch,1);

                           if(iFind>0){
                           	   if(strlen(pData[i].caContent)+strlen(caTemp)<8000){
        		   	 	 	  			    sprintf(pData[i].caContent+strlen(pData[i].caContent),"%s\n",caTemp);
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
        		  
					  remove(caMailfile);
						     						        
				

				 memset(caSubject,0,sizeof(caSubject));
						      
					sprintf(caTemp_d,"%s",pData[i].caContent);
			    if(strlen(caTemp_d)>0){
						 iFind = pasAcbmSearch(psAcbmTree_keyword,caTemp_d,strlen(caTemp_d),psMatch,1);

               if(iFind>0){
                  strcpy(caTemp_d,pasDbReplaceQuote1(pData[i].caContent));
                  strcpy(caContent,caTemp_d);
                 
                  strcpy(caSubject,psAcbmPat_keyword[psMatch[0]].data);


               	    sprintf(caTemp,"select count(*) from ncskeywordlog where did=%d and uid=%d and keyword='%s'  and vtime>%d and vtime<%d ",
               	        pData[i].did,pData[i].uid,psAcbmPat_keyword[psMatch[0]].data,pData[i].vtime-1800,pData[i].vtime+1800);
       printf("caTemp=%s\n",caTemp);
               	        lCount=0;
               	        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount); 
               	        if(lCount==0){
     //       printf("sid=%llu\n",pData[i].sid);
  	 
              	sprintf(caTemp,"insert into ncskeywordlog (did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,subject,content,url,type,tid) \
               	        	 values(%d,%d,'%s','%s','%s','%s','%s','%s',%d,%d,'%s','%s','%s','%s',%d,%d)", \
               	        	 pData[i].did,pData[i].uid,pData[i].caDispname,pData[i].caIdcard,pData[i].caMac,pData[i].caIdtype,pData[i].caPosition,pData[i].caSip,pData[i].vtime,psAcbmPat_keyword[psMatch[0]].flags,psAcbmPat_keyword[psMatch[0]].data,caSubject,caContent,pData[i].caUrl,4,pData[i].tid);  	 
              	           iSize=strlen(caTemp)+256;       	 
       	  printf("caTemp=%s\n",caTemp);      	             	        	 
               	        	pasDbExecSqlFn(caTemp,iSize);
               	        } 
       	    
               	 
               
               }
              }
             }
            }
        	}
        		
        		
//表单搜索结束

//搜索搜索日志
 			  ncsUtlGetLastInfo_v4("SearchidSearch",&lLastid);

			  sprintf(caTemp,"select dispname,mac,position,idno,idtype,did,uid,stime,sip,sname,svalue,sid,tid from ncssearchlog_if_%s where sid>%llu order by sid limit 0,1000 ", caTableday,lLastid);
//		  printf("caTemp=%s\n",caTemp);
			  		
			  psCur = pasDbOpenSql(caTemp,0);
        if(psCur){
        	memset(caDispname,0,sizeof(caDispname));
        	memset(caMac,0,sizeof(caMac));
        	memset(caPosition,0,sizeof(caPosition));
        	memset(caIdno,0,sizeof(caIdno));
        	memset(caIdtype,0,sizeof(caIdtype));
        	lDid=0;
        	lUid=0;
        	lStime=0;
        	memset(caSip,0,sizeof(caSip));
        	lService=0;
        	memset(caMfrom,0,sizeof(caMfrom));
        	memset(caMto,0,sizeof(caMto));
        	memset(caContent,0,sizeof(caContent));
        	lSid=0;
        	lLastid=0;
        	lTid=0;
        	iReturn = pasDbFetchInto(psCur,  
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_STRING,47,caMfrom,
                                     UT_TYPE_STRING,254,caContent,
                                     UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_LONG,4,&lTid);
            lSum_keylog=0; 
            lRep=0;                     
            while((iReturn==0)||(iReturn==1405)){
            	          
            if(lSum_keylog==0){
            	lRep=1;
            }
            else{
            	if(lDid==pData[lSum_keylog-1].did&&lUid==pData[lSum_keylog-1].uid&&strcmp(caContent,pData[lSum_keylog-1].caContent)==0){
            		lRep=0;
            	}
            	else{
            		lRep=1;
            	}
            }
            	if(lRep==1){
            		strcpy(pData[lSum_keylog].caDispname,caDispname);
            		strcpy(pData[lSum_keylog].caMac,caMac);
            		strcpy(pData[lSum_keylog].caPosition,caPosition);
            		strcpy(pData[lSum_keylog].caIdcard,caIdno);
            		strcpy(pData[lSum_keylog].caIdtype,caIdtype);
            		pData[lSum_keylog].did=lDid;
            		pData[lSum_keylog].uid=lUid;
            		pData[lSum_keylog].tid=lTid;
            		strcpy(pData[lSum_keylog].caSip,caSip);

            		pData[lSum_keylog].vtime=lStime;
            		sprintf(pData[lSum_keylog].caSubject,"使用%s搜索",caMfrom);
            		strcpy(pData[lSum_keylog].caContent,caContent);
            		sprintf(pData[lSum_keylog].caUrl,"");
            		lSum_keylog++;
            	}
            	
            	    lLastid=lSid;
				        	memset(caDispname,0,sizeof(caDispname));
				        	memset(caMac,0,sizeof(caMac));
				        	memset(caPosition,0,sizeof(caPosition));
				        	memset(caIdno,0,sizeof(caIdno));
				        	memset(caIdtype,0,sizeof(caIdtype));
				        	lDid=0;
				        	lUid=0;
				        	lStime=0;
				        	memset(caSip,0,sizeof(caSip));
				        	lService=0;
				        	memset(caMfrom,0,sizeof(caMfrom));
				        	memset(caMto,0,sizeof(caMto));
				        	memset(caContent,0,sizeof(caContent));
				        	lSid=0;
				        	lTid=0;
        	        iReturn = pasDbFetchInto(psCur,  
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_STRING,47,caMfrom,
                                     UT_TYPE_STRING,254,caContent,
                                     UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_LONG,4,&lTid);
            }
            pasDbCloseCursor(psCur);
            if(lLastid>0&&lSum_keylog>0){
            	ncsUtlSetLastInfo_v4("SearchidSearch",lLastid); 
            }
        	
        	for(i=0;i<lSum_keylog;i++){
        		iFind=0;
             strcpy(caSubject,pData[i].caSubject);
              if(strlen(pData[i].caContent)>0){
                  strcpy(caContent,pData[i].caContent);
               	  iFind = pasAcbmSearch(psAcbmTree_keyword,caContent,strlen(caContent),psMatch,1);
               	 strcpy(caContent,pasDbReplaceQuote1(pData[i].caContent));
              }
              else{
              	strcpy(caContent,"");
              }
              
               if(iFind>0){

               	    sprintf(caTemp,"select count(*) from ncskeywordlog where did=%d and uid=%d and subject ='%s' and content='%s' and vtime>%d and vtime<%d ",
               	        pData[i].did,pData[i].uid,caSubject,caContent,pData[i].vtime-1800,pData[i].vtime+1800);
          
               	        lCount=0;
               	        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount); 
               	        if(lCount==0){
               //	        	printf("insert into ncskeywordlog (did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,subject,content,url,type) \
               //	        	 values(%d,%d,'%s','%s','%s','%s','%s','%s',%d,%d,'%s','%s','%s','%s',2)\n", \
               	//        	 pData[i].did,pData[i].uid,pData[i].caDispname,pData[i].caIdcard,pData[i].caMac,pData[i].caIdtype,pData[i].caPosition,pData[i].caSip,pData[i].vtime,psAcbmPat_keyword[psMatch[0]].flags,psAcbmPat_keyword[psMatch[0]].data,caSubject,caContent,pData[i].caUrl);
               	        	 
               	        	pasDbExecSqlF("insert into ncskeywordlog (did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,subject,content,url,type,tid) \
               	        	 values(%d,%d,'%s','%s','%s','%s','%s','%s',%d,%d,'%s','%s','%s','%s',%d,%d)", \
               	        	 pData[i].did,pData[i].uid,pData[i].caDispname,pData[i].caIdcard,pData[i].caMac,pData[i].caIdtype,pData[i].caPosition,pData[i].caSip,pData[i].vtime,psAcbmPat_keyword[psMatch[0]].flags,psAcbmPat_keyword[psMatch[0]].data,caSubject,caContent,pData[i].caUrl,5,pData[i].tid);
               	        	 
               	        } 
               	    
               	 
               
               }

             
        		}
        		
        	}
//即时通讯搜索结束 







 

           	
        	 free(pData);

        }
        
   
     
  
  	sleep(3);
  }
}


//过滤敏感URL和虚拟身份

int ncsCheckUrlNetid(utShmHead *psShmHead)
{
	pasDbCursor *psCur;
	long iSumurl,iSumnetid,iSumuser;
	long iReturn,lTime;
	char caTemp[4096];
	char caTableday[20];
	 struct data_s{
	 	char caDispname[32];
	 	char caMac[20];
	 	char caPosition[32];
	 	char caIdcard[20];
	 	char caIdtype[4];
	 	long did;
	 	long uid;
	 	long tid;
	 	char caSip[16];
	 	long vtime;
	 	char caKeyword[64];
	 	char caTitle[128];
	 	char caHost[64];
	 	long urlid;
	 	char caUrl[128];
	 	char caNetid[64];
	 	long ntype;
	 	long funcode;
	};
	char caDip[16],caNetid[68];
	long lNtype,lFuncode;
  struct data_s *pData;
  char caHost[68],caTitle[132];
	long long lLastid;
	long lUrlid;
	long lCount;
	long lSum_urllog,lSum_netidlog,lSum_userlog;
	char caDispname[32],caMac[20],caPosition[32],caIdno[20],caIdtype[8];
	long lDid,lUid,lStime,lRep;
	char caSip[16],caUrl[128],caTemp_d[1000];
	long long lSid;
	long int psMatch[8];
  int iFind,i;
  long lService;
  long lTid;
  char caMfrom[48],caMto[48];
	iReturn = pasConnect(psShmHead);
	if(iReturn<0){
		sleep(60);
  	return 0;
  }
  iSumurl=ncLoadAcbm_url();
  iSumnetid=ncLoadAcbm_netid();
  iSumuser=ncLoadAcbm_user();
  printf("iSumuser=%d\n",iSumuser);
  while(1){
 // 	printf("iSumrul=%d\n",iSumurl);
  	if(iSumurl>0){    		                                                 //对URL进行过滤
  		  lTime=time(0);
			  strcpy(caTableday,utTimFormat("%Y%m",lTime));
			  pData=(struct data_s*)malloc((10002)*sizeof(struct data_s));
			  if(pData==NULL){
			  	free(psAcbmPat_url);
			  	sleep(60);
			  	return (-1);
			  }
			  
			  
			  ncsUtlGetLastInfo_v4("urlidSearch",&lLastid);
			  sprintf(caTemp,"select dispname,mac,position,idno,idtype,did,uid,stime,sip,uri,host,title,urlid,sid,tid from ncshttplog_if_%s where sid>%llu order by sid limit 0,10000 ", caTableday,lLastid);
//		  printf("caTemp=%s\n",caTemp);
			  

		
			  psCur = pasDbOpenSql(caTemp,0);
        if(psCur){
        	memset(caDispname,0,sizeof(caDispname));
        	memset(caMac,0,sizeof(caMac));
        	memset(caPosition,0,sizeof(caPosition));
        	memset(caIdno,0,sizeof(caIdno));
        	memset(caIdtype,0,sizeof(caIdtype));
        	lDid=0;
        	lUid=0;
        	lStime=0;
        	memset(caSip,0,sizeof(caSip));
        	memset(caUrl,0,sizeof(caUrl));
        	memset(caHost,0,sizeof(caHost));
        	memset(caTitle,0,sizeof(caTitle));
        	lUrlid=0;
        	lTid=0;

        	lSid=0;
        	lLastid=0;
        	iReturn = pasDbFetchInto(psCur,  
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_STRING,127,caUrl,
                                     UT_TYPE_STRING,64,caHost,
                                     UT_TYPE_STRING,127,caTitle,
                                     UT_TYPE_LONG,4,&lUrlid,
                                     UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_LONG,4,&lTid);
            lSum_urllog=0; 
            lRep=0;                     
            while((iReturn==0)||(iReturn==1405)){
            	          
            if(lSum_urllog==0){
            	lRep=1;
            }
            else{
            	if(lDid==pData[lSum_urllog-1].did&&lUid==pData[lSum_urllog-1].uid&&strcmp(caHost,pData[lSum_urllog-1].caHost)==0&&strcmp(caUrl,pData[lSum_urllog-1].caUrl)==0){
            		lRep=0;
            	}
            	else{
            		lRep=1;
            	}
            }
            	if(lRep==1){
            		strcpy(pData[lSum_urllog].caDispname,caDispname);
            		strcpy(pData[lSum_urllog].caMac,caMac);
            		strcpy(pData[lSum_urllog].caPosition,caPosition);
            		strcpy(pData[lSum_urllog].caIdcard,caIdno);
            		strcpy(pData[lSum_urllog].caIdtype,caIdtype);
            		pData[lSum_urllog].did=lDid;
            		pData[lSum_urllog].uid=lUid;
            		strcpy(pData[lSum_urllog].caSip,caSip);
            		pData[lSum_urllog].vtime=lStime;
            		strcpy(pData[lSum_urllog].caHost,caHost);
            		strcpy(pData[lSum_urllog].caTitle,caTitle);
            		strcpy(pData[lSum_urllog].caUrl,caUrl);
            		pData[lSum_urllog].urlid=lUrlid;
            		pData[lSum_urllog].tid=lTid;
            		lSum_urllog++;
            	}
            	
            	          lLastid=lSid;
            	        	memset(caDispname,0,sizeof(caDispname));
							        	memset(caMac,0,sizeof(caMac));
							        	memset(caPosition,0,sizeof(caPosition));
							        	memset(caIdno,0,sizeof(caIdno));
							        	memset(caIdtype,0,sizeof(caIdtype));
							        	lDid=0;
							        	lUid=0;
							        	lStime=0;
							        	memset(caSip,0,sizeof(caSip));
							        	memset(caUrl,0,sizeof(caUrl));
							        	memset(caHost,0,sizeof(caHost));
							        	memset(caTitle,0,sizeof(caTitle));
							        	lUrlid=0;
							        	lSid=0;
							        	lTid=0;
							        	iReturn = pasDbFetchInto(psCur,  
							                                     UT_TYPE_STRING,31,caDispname,
							                                     UT_TYPE_STRING,19,caMac,
							                                     UT_TYPE_STRING,31,caPosition,
							                                     UT_TYPE_STRING,18,caIdno,
							                                     UT_TYPE_STRING,3,caIdtype,
							                                     UT_TYPE_LONG,4,&lDid,
							                                     UT_TYPE_LONG,4,&lUid,
							                                     UT_TYPE_LONG,4,&lStime,
							                                     UT_TYPE_STRING,15,caSip,
							                                     UT_TYPE_STRING,127,caUrl,
							                                     UT_TYPE_STRING,64,caHost,
							                                     UT_TYPE_STRING,127,caTitle,
							                                     UT_TYPE_LONG,4,&lUrlid,
							                                     UT_TYPE_LONG8,8,&lSid,
							                                     UT_TYPE_LONG,4,&lTid);
            	
            }
            pasDbCloseCursor(psCur);
            if(lLastid>0&&lSum_urllog>0){
            	ncsUtlSetLastInfo_v4("urlidSearch",lLastid); 
            }
        	
        	for(i=0;i<lSum_urllog;i++){
        		iFind=0;
        		if(strlen(pData[i].caHost)>0){
        			sprintf(caTemp_d,"%s%s",pData[i].caHost,pData[i].caUrl);

    //	         strcpy(caSubject,pasDbReplaceQuote1(caTemp_d));
               iFind = pasAcbmSearch(psAcbmTree_url,caTemp_d,strlen(caTemp_d),psMatch,1);
              }
             
               if(iFind>0){
                    strcpy(caUrl,pasDbReplaceQuote1(pData[i].caUrl));
               	    sprintf(caTemp,"select count(*) from ncssenurllog where did=%d and uid=%d and host ='%s'  and vtime>%d and vtime<%d ",
               	        pData[i].did,pData[i].uid,pData[i].caHost,pData[i].vtime-1800,pData[i].vtime+1800);
        
               	        lCount=0;
               	        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount); 
               	        if(lCount==0){

               	        	 
               	        	pasDbExecSqlF("insert into ncssenurllog (did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,host,urlid,uri,title,tid) \
               	        	 values(%d,%d,'%s','%s','%s','%s','%s','%s',%d,%d,'%s','%s','%d','%s','%s',%d)", \
               	        	 pData[i].did,pData[i].uid,pData[i].caDispname,pData[i].caIdcard,pData[i].caMac,pData[i].caIdtype,pData[i].caPosition,pData[i].caSip,pData[i].vtime,psAcbmPat_url[psMatch[0]].flags,psAcbmPat_url[psMatch[0]].data,pData[i].caHost,pData[i].urlid,caUrl,pData[i].caTitle,pData[i].tid);
               	        	 
               	        } 
               	    
               	 
               
               }

             
        		}
        		
        	}
//URL搜索结束  


           	
        	 free(pData);
        }
        
        
 //虚拟身份搜索 
 //printf("iSumnetid=%d\n",iSumnetid);      
     	if(iSumnetid>0){    		                                                 //对虚拟身份进行过滤
  		  lTime=time(0);
			  strcpy(caTableday,utTimFormat("%Y%m",lTime));
			  pData=(struct data_s*)malloc((10002)*sizeof(struct data_s));
			  if(pData==NULL){
			  	free(psAcbmPat_netid);
			  	sleep(60);
			  	return (-1);
			  }
			  
			  
			  ncsUtlGetLastInfo_v4("netidSearch",&lLastid);
			  sprintf(caTemp,"select dispname,mac,position,idno,idtype,did,uid,stime,sip,dip,name,ntype,funcode,sid,tid from ncsnetlog_if_%s where sid>%llu order by sid limit 0,10000 ", caTableday,lLastid);
//	  printf("caTemp=%s\n",caTemp);
			  

		
			  psCur = pasDbOpenSql(caTemp,0);
        if(psCur){
        	memset(caDispname,0,sizeof(caDispname));
        	memset(caMac,0,sizeof(caMac));
        	memset(caPosition,0,sizeof(caPosition));
        	memset(caIdno,0,sizeof(caIdno));
        	memset(caIdtype,0,sizeof(caIdtype));
        	lDid=0;
        	lUid=0;
        	lStime=0;
        	memset(caSip,0,sizeof(caSip));
        	memset(caDip,0,sizeof(caDip));
        	memset(caNetid,0,sizeof(caNetid));
        	lNtype=0;
        	lFuncode=0;
        	lSid=0;
        	lLastid=0;
        	lTid=0;
        	iReturn = pasDbFetchInto(psCur,  
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_STRING,15,caDip,
                                     UT_TYPE_STRING,63,caNetid,
                                     UT_TYPE_LONG,4,&lNtype,
                                     UT_TYPE_LONG,4,&lFuncode,
                                     UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_LONG,4,&lTid);
            lSum_netidlog=0; 
            lRep=0;                     
            while((iReturn==0)||(iReturn==1405)){
            	          
            if(lSum_netidlog==0){
            	lRep=1;
            }
            else{
            	if(lDid==pData[lSum_netidlog-1].did&&lUid==pData[lSum_netidlog-1].uid&&strcmp(caNetid,pData[lSum_netidlog-1].caNetid)==0&&pData[lSum_netidlog-1].ntype==lNtype){
            		lRep=0;
            	}
            	else{
            		lRep=1;
            	}
            }
            	if(lRep==1){
            		strcpy(pData[lSum_netidlog].caDispname,caDispname);
            		strcpy(pData[lSum_netidlog].caMac,caMac);
            		strcpy(pData[lSum_netidlog].caPosition,caPosition);
            		strcpy(pData[lSum_netidlog].caIdcard,caIdno);
            		strcpy(pData[lSum_netidlog].caIdtype,caIdtype);
            		pData[lSum_netidlog].did=lDid;
            		pData[lSum_netidlog].uid=lUid;
            		strcpy(pData[lSum_netidlog].caSip,caSip);
            		pData[lSum_netidlog].vtime=lStime;
            		strcpy(pData[lSum_netidlog].caHost,caDip);
            		strcpy(pData[lSum_netidlog].caNetid,caNetid);
   //         		printf("netid=%s\n",pData[lSum_netidlog].caNetid);
            	  pData[lSum_netidlog].funcode=lFuncode;
            		pData[lSum_netidlog].ntype=lNtype;
            		pData[lSum_netidlog].tid=lTid;
            		lSum_netidlog++;
            	}
            	
            	          lLastid=lSid;
            	        	memset(caDispname,0,sizeof(caDispname));
							        	memset(caMac,0,sizeof(caMac));
							        	memset(caPosition,0,sizeof(caPosition));
							        	memset(caIdno,0,sizeof(caIdno));
							        	memset(caIdtype,0,sizeof(caIdtype));
							        	lDid=0;
							        	lUid=0;
							        	lStime=0;
							        	memset(caSip,0,sizeof(caSip));
							        	memset(caSip,0,sizeof(caSip));
							        	memset(caDip,0,sizeof(caDip));
							        	memset(caNetid,0,sizeof(caNetid));
							        	lNtype=0;
							        	lFuncode=0;
							        	lSid=0;
							          lTid=0;
							        	iReturn = pasDbFetchInto(psCur,  
							                                     UT_TYPE_STRING,31,caDispname,
							                                     UT_TYPE_STRING,19,caMac,
							                                     UT_TYPE_STRING,31,caPosition,
							                                     UT_TYPE_STRING,18,caIdno,
							                                     UT_TYPE_STRING,3,caIdtype,
							                                     UT_TYPE_LONG,4,&lDid,
							                                     UT_TYPE_LONG,4,&lUid,
							                                     UT_TYPE_LONG,4,&lStime,
							                                     UT_TYPE_STRING,15,caSip,
							                                     UT_TYPE_STRING,15,caDip,
							                                     UT_TYPE_STRING,63,caNetid,
							                                     UT_TYPE_LONG,4,&lNtype,
							                                     UT_TYPE_LONG,4,&lFuncode,
							                                     UT_TYPE_LONG8,8,&lSid,
							                                     UT_TYPE_LONG,4,&lTid);
            	
            }
            pasDbCloseCursor(psCur);
            if(lLastid>0&&lSum_netidlog>0){
            	ncsUtlSetLastInfo_v4("netidSearch",lLastid); 
            }
        	
        	for(i=0;i<lSum_netidlog;i++){
        		iFind=0;
        		if(strlen(pData[i].caNetid)>0){
        			sprintf(caTemp_d,"%d%s",pData[i].ntype,pData[i].caNetid);
//printf("caTemp_d=%s\n",caTemp_d);
    //	         strcpy(caSubject,pasDbReplaceQuote1(caTemp_d));
               iFind = pasAcbmSearch(psAcbmTree_netid,caTemp_d,strlen(caTemp_d),psMatch,1);
              }
             
               if(iFind>0){
                    strcpy(caNetid,pasDbReplaceQuote1(pData[i].caNetid));
               	    sprintf(caTemp,"select count(*) from ncssennetidlog where did=%d and uid=%d and netid ='%s' and ntype=%d and vtime>%d and vtime<%d ",
               	        pData[i].did,pData[i].uid,pData[i].caNetid,pData[i].ntype,pData[i].vtime-1800,pData[i].vtime+1800);

               	        lCount=0;
               	        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount); 
               	        if(lCount==0){
      	 
               	        	pasDbExecSqlF("insert into ncssennetidlog (did,uid,dispname,idcard,mac,idtype,position,sip,vtime,kid,keyword,ntype,netid,dip,fun,tid) \
               	        	 values(%d,%d,'%s','%s','%s','%s','%s','%s',%d,%d,'%s','%d','%s','%s',%d,%d)", \
               	        	 pData[i].did,pData[i].uid,pData[i].caDispname,pData[i].caIdcard,pData[i].caMac,pData[i].caIdtype,pData[i].caPosition,pData[i].caSip,pData[i].vtime,psAcbmPat_netid[psMatch[0]].flags,psAcbmPat_netid[psMatch[0]].data,pData[i].ntype,caNetid,pData[i].caHost,pData[i].funcode,pData[i].tid);
               	        	 
               	        } 
               	    
               	 
               
               }

             
        		}
        		
        	}
//URL搜索结束  


           	
        	 free(pData);
        }
   
    //上下线搜索 
 //printf("iSumnetid=%d\n",iSumnetid);      
     	if(iSumuser>0){    		                                                 //对虚拟身份进行过滤
  		  lTime=time(0);
			  strcpy(caTableday,utTimFormat("%Y%m",lTime));
			  pData=(struct data_s*)malloc((10002)*sizeof(struct data_s));
			  if(pData==NULL){
			  	free(psAcbmPat_user);
			  	sleep(60);
			  	return (-1);
			  }
			  
			  
			  ncsUtlGetLastInfo_v4("userlogSearch",&lLastid);
			  sprintf(caTemp,"select dispname,mac,position,idno,idtype,did,uid,stime,sip,flags,sid,tid from ncsuserlog_if_%s where sid>%llu order by sid limit 0,10000 ", caTableday,lLastid);
	//  printf("caTemp=%s\n",caTemp);
			  

		
			  psCur = pasDbOpenSql(caTemp,0);
        if(psCur){
        	memset(caDispname,0,sizeof(caDispname));
        	memset(caMac,0,sizeof(caMac));
        	memset(caPosition,0,sizeof(caPosition));
        	memset(caIdno,0,sizeof(caIdno));
        	memset(caIdtype,0,sizeof(caIdtype));
        	lDid=0;
        	lUid=0;
        	lStime=0;
        	memset(caSip,0,sizeof(caSip));
        	memset(caDip,0,sizeof(caDip));
        	memset(caNetid,0,sizeof(caNetid));
        	lNtype=0;
        	lFuncode=0;
        	lSid=0;
        	lLastid=0;
        	lTid=0;
        	iReturn = pasDbFetchInto(psCur,  
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_LONG,4,&lFuncode,
                                     UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_LONG,4,&lTid);
            lSum_userlog=0; 
            lRep=0;                     
            while((iReturn==0)||(iReturn==1405)){
            	          
            if(lSum_userlog==0){
            	lRep=1;
            }
            else{
            	if(lDid==pData[lSum_userlog-1].did&&lUid==pData[lSum_userlog-1].uid){
            		lRep=0;
            	}
            	else{
            		lRep=1;
            	}
            }
            	if(lRep==1){
            		strcpy(pData[lSum_userlog].caDispname,caDispname);
            		strcpy(pData[lSum_userlog].caMac,caMac);
            		strcpy(pData[lSum_userlog].caPosition,caPosition);
            		strcpy(pData[lSum_userlog].caIdcard,caIdno);
            		strcpy(pData[lSum_userlog].caIdtype,caIdtype);
            		pData[lSum_userlog].did=lDid;
            		pData[lSum_userlog].uid=lUid;
            		strcpy(pData[lSum_userlog].caSip,caSip);
            		pData[lSum_userlog].vtime=lStime;
            	
   //         		printf("netid=%s\n",pData[lSum_netidlog].caNetid);
            	  pData[lSum_userlog].funcode=lFuncode;
            	
            		pData[lSum_userlog].tid=lTid;
            		lSum_userlog++;
            	}
            	
            	          lLastid=lSid;
            	        	memset(caDispname,0,sizeof(caDispname));
							        	memset(caMac,0,sizeof(caMac));
							        	memset(caPosition,0,sizeof(caPosition));
							        	memset(caIdno,0,sizeof(caIdno));
							        	memset(caIdtype,0,sizeof(caIdtype));
							        	lDid=0;
							        	lUid=0;
							        	lStime=0;
							        	memset(caSip,0,sizeof(caSip));
							        	memset(caSip,0,sizeof(caSip));
							        	memset(caDip,0,sizeof(caDip));
							        	memset(caNetid,0,sizeof(caNetid));
							        	lNtype=0;
							        	lFuncode=0;
							        	lSid=0;
							          lTid=0;
							        	iReturn = pasDbFetchInto(psCur,  
							                                     UT_TYPE_STRING,31,caDispname,
							                                     UT_TYPE_STRING,19,caMac,
							                                     UT_TYPE_STRING,31,caPosition,
							                                     UT_TYPE_STRING,18,caIdno,
							                                     UT_TYPE_STRING,3,caIdtype,
							                                     UT_TYPE_LONG,4,&lDid,
							                                     UT_TYPE_LONG,4,&lUid,
							                                     UT_TYPE_LONG,4,&lStime,
							                                     UT_TYPE_STRING,15,caSip,
							                                     UT_TYPE_LONG,4,&lFuncode,
							                                     UT_TYPE_LONG8,8,&lSid,
							                                     UT_TYPE_LONG,4,&lTid);
            	
            }
            pasDbCloseCursor(psCur);
            if(lLastid>0&&lSum_userlog>0){
            	ncsUtlSetLastInfo_v4("userlogSearch",lLastid); 
            }
        	
        	for(i=0;i<lSum_userlog;i++){
        		iFind=0;
        		if(strlen(pData[i].caDispname)>0){
        			sprintf(caTemp_d,"%s%s%s",pData[i].caDispname,pData[i].caMac,pData[i].caIdcard);
//printf("caTemp_d=%s\n",caTemp_d);
    //	         strcpy(caSubject,pasDbReplaceQuote1(caTemp_d));
               iFind = pasAcbmSearch(psAcbmTree_user,caTemp_d,strlen(caTemp_d),psMatch,1);
              }
             
               if(iFind>0){
                    
               	    sprintf(caTemp,"select count(*) from ncssenuserlog where did=%d and uid=%d  and vtime>%d and vtime<%d ",
               	        pData[i].did,pData[i].uid,pData[i].vtime-1800,pData[i].vtime+1800);

               	        lCount=0;
               	        pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount); 
               	        if(lCount==0){
              	        	 
               	        	 
               	        	pasDbExecSqlF("insert into ncssenuserlog (did,uid,dispname,idcard,mac,idtype,position,sip,vtime,keyword,fun,tid,flag) \
               	        	 values(%d,%d,'%s','%s','%s','%s','%s','%s',%d,'%s','%d',%d,0)", \
               	        	 pData[i].did,pData[i].uid,pData[i].caDispname,pData[i].caIdcard,pData[i].caMac,pData[i].caIdtype,pData[i].caPosition,pData[i].caSip,pData[i].vtime,psAcbmPat_user[psMatch[0]].data,pData[i].funcode,pData[i].tid);
               	        	 
               	        } 
               	    
               	 
               
               }

             
        		}
        		
        	}
//上下线日志搜索结束  


           	
        	 free(pData);
        }
   
   
   
   
       
  
  	sleep(3);
  }
}
int ncLoadAcbm_keywordOld()
{
	pasDbCursor  *psCur;
	int iSumAcbmPat=0,i;
	int lSumRecord;
  char  caWord[128];
  char caTemp[1024];
  int iReturn,lTid;
  sprintf(caTemp,"select count(*) from ncscasepost a,ncscaseinfo b where a.cid=b.sid and b.flags=1 ");
  iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSumRecord);
  if(iReturn!=0||lSumRecord==0){
  	return 0;
  }
  psAcbmPat_keyword=(pattern_data*)malloc(lSumRecord*sizeof(pattern_data));
  if(psAcbmPat_keyword==NULL){
  	return (-1);
  }
  memset(psAcbmPat_keyword,0,lSumRecord*sizeof(pattern_data));
  psCur=pasDbOpenSqlF("select a.cont,a.cid from ncscasepost a,ncscaseinfo b where a.cid=b.sid and b.flags=1 ");
  if(psCur==NULL){
  	free(psAcbmPat_keyword);
  	return (-1);
  }
  i=0;
  memset(caWord,0,sizeof(caWord));
  lTid=0;
  iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,64,caWord,
                               UT_TYPE_LONG,4,&lTid);
  while(iReturn==0||iReturn==1405){
  	if(strlen(caWord)>0){
  	   strcpy(psAcbmPat_keyword[i].data,caWord);
  	   psAcbmPat_keyword[i].len=strlen(caWord);
  	   psAcbmPat_keyword[i].flags=lTid;
  	   i++;
  	   }
  	  memset(caWord,0,sizeof(caWord));
  	  
		  lTid=0;
		  iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,64,caWord,
		                               UT_TYPE_LONG,4,&lTid);
  }
     iSumAcbmPat = i;


  pasDbCloseCursor(psCur);
  psAcbmTree_keyword = (pattern_tree *)pasAcbmInit(psAcbmPat_keyword,iSumAcbmPat);

    return iSumAcbmPat;

  
  
}
//新版搜索关键字加到布控告警中老版本搜索关键字
int ncsCheckKeyword_old(utShmHead *psShmHead)
{
	pasDbCursor *psCur;
	long iSumkey;
	long iReturn,lTime;
	char caTemp[20000];
	char caTableday[20];
	 struct data_s{
	 	char caDispname[32];
	 	char caMac[20];
	 	char caPosition[32];
	 	char caIdcard[20];
	 	char caIdtype[4];
	 	unsigned long did;
	 	unsigned long uid;
	 	unsigned long tid;
	 	unsigned long lSip;
	 	long vtime;
	 	char caKeyword[64];
	 	char caSubject[256];
	 	long service;
	 	char caContent[10000];
	 	char caUrl[200];
	 	long long fileid;
	 	char caDesc[256];
	};
  struct data_s *pData;
	unsigned long lSip;
	long long lLastid;
	long lCount;
	long lSum_keylog;
	char caDispname[32],caMac[20],caPosition[32],caIdno[20],caIdtype[8];
	long lDid,lUid,lStime,lRep;
	char caSip[16],caUrl[128],caSubject[512],caContent[10000],caTemp_d[10000];
	long long lSid;
	long int psMatch[8];
  int iFind,i;
  long lService,lTid;
  long iFlags;
  char *p;
  int iSize;
  char caMfrom[48],caMto[48],caTempDir[128],caFile2[128],caFile1[128];
  char caMailfile[256],caTable[64];

    ncPostHead  *psHead;
    pasFile     *ps;
    ncPostCont *psCont;

  char caSdate[20];
  char *tmpgbk;
  long long lFileid,len;
  char caHost[72];
	iReturn = pasConnect(psShmHead);
	if(iReturn<0){
		sleep(60);
  	return 0;
  }
  iSumkey=ncLoadAcbm_keywordOld();
  
printf("iSumkey=%d\n",iSumkey);
 
  while(1){


  	if(iSumkey>0){    		                                                 //对关键字进行过滤
  		  lTime=time(0);
			  strcpy(caTableday,utTimFormat("%Y%m",lTime));
			  pData=(struct data_s*)malloc((1002)*sizeof(struct data_s));
			  if(pData==NULL){
			  	free(psAcbmPat_keyword);
			  	sleep(60);
			  	return (-1);
			  }
			  
  




 


//搜索搜索日志
        char caLast[20];
//新版管理中心
         ncsUtlGetLastInfo_v4("BkidSearch",&lLastid);
 //        lTime=time(0);
 			   strcpy(caTableday,utTimFormat("%Y%m",lTime));
 			 
   //     strcpy(caLast,utTimFormat("%Y/%m/%d %H:%M:%S",lLastid));
			  sprintf(caTemp,"select sid,dispname,mac,did,uid,stime,sip,sname,svalue,tid from ncssearchlog_if_%s where sid>%llu order by sid limit 0,1000 ", caTableday,lLastid);
		  printf("caTemp=%s\n",caTemp);
			  		
			  psCur = pasDbOpenSql(caTemp,0);
        if(psCur){
        	memset(caDispname,0,sizeof(caDispname));
        	memset(caMac,0,sizeof(caMac));
        	
        	lDid=0;
        	lUid=0;
        	lStime=0;
        	memset(caSip,0,sizeof(caSip));
        	lService=0;
        	memset(caMfrom,0,sizeof(caMfrom));
        	memset(caMto,0,sizeof(caMto));
        	memset(caContent,0,sizeof(caContent));
        	lSid=0;
        	lLastid=0;
        	lTid=0;
        	lSip=0;
        	memset(caSdate,0,sizeof(caSdate));
    
        	iReturn = pasDbFetchInto(psCur, 
        	                           UT_TYPE_LONG8,8,&lSid, 
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_ULONG,4,&lDid,
                                     UT_TYPE_ULONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_ULONG,4,&lSip,
                                     UT_TYPE_STRING,47,caMfrom,
                                     UT_TYPE_STRING,254,caContent,
                                     UT_TYPE_LONG,4,&lTid);
            lSum_keylog=0; 
            lRep=0;                     
            while((iReturn==0)||(iReturn==1405)){
            	          
            if(lSum_keylog==0){
            	lRep=1;
            }
            else{
            	if(lDid==pData[lSum_keylog-1].did&&lUid==pData[lSum_keylog-1].uid&&strcmp(caContent,pData[lSum_keylog-1].caContent)==0){
            		lRep=0;
            	}
            	else{
            		lRep=1;
            	}
            }
            
            	if(lRep==1){
            		strcpy(pData[lSum_keylog].caDispname,caDispname);
            		strcpy(pData[lSum_keylog].caMac,caMac);
            		
            		pData[lSum_keylog].did=lDid;
            		pData[lSum_keylog].uid=lUid;
            		pData[lSum_keylog].tid=lTid;
            		pData[lSum_keylog].lSip=lSip;
            		
            		pData[lSum_keylog].vtime=lStime;
            		sprintf(pData[lSum_keylog].caSubject,"搜索%s",caContent);
            		strcpy(pData[lSum_keylog].caContent,caContent);
            		sprintf(pData[lSum_keylog].caUrl,"");
            		lSum_keylog++;
            	}
// printf("lTime=%d\n",lStime);  
                  if(lSid>0){        	
            	      lLastid=lSid;
            	    }
				        	memset(caDispname,0,sizeof(caDispname));
				        	memset(caMac,0,sizeof(caMac));
				        	memset(caPosition,0,sizeof(caPosition));
				        	memset(caIdno,0,sizeof(caIdno));
				        	memset(caIdtype,0,sizeof(caIdtype));
				        	lDid=0;
				        	lUid=0;
				        	lStime=0;
				        	memset(caSip,0,sizeof(caSip));
				        	lService=0;
				        	memset(caMfrom,0,sizeof(caMfrom));
				        	memset(caMto,0,sizeof(caMto));
				        	memset(caContent,0,sizeof(caContent));
				        	lSid=0;
				        	lTid=0;
				        	lSip=0;
           	iReturn = pasDbFetchInto(psCur,  
           	                         UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_ULONG,4,&lDid,
                                     UT_TYPE_ULONG,4,&lUid,
                                     UT_TYPE_LONG,4,&lStime,
                                     UT_TYPE_ULONG,4,&lSip,
                                     UT_TYPE_STRING,47,caMfrom,
                                     UT_TYPE_STRING,254,caContent,
                                     UT_TYPE_LONG,4,&lTid);
            }
            pasDbCloseCursor(psCur);
            if(lLastid>0&&lSum_keylog>0){
            	printf("sitid=%llu",lLastid);
            	ncsUtlSetLastInfo_v4("BkidSearch",lLastid); 
            }
// printf("lSum_keylog=%d\n",lSum_keylog);       	
        	for(i=0;i<lSum_keylog;i++){
        		iFind=0;
             strcpy(caSubject,pData[i].caSubject);
              if(strlen(pData[i].caContent)>0){
                  strcpy(caContent,pData[i].caContent);
               	  iFind = pasAcbmSearch(psAcbmTree_keyword,caContent,strlen(caContent),psMatch,1);
               	 strcpy(caContent,pasDbReplaceQuote1(pData[i].caContent));
              }
              else{
              	strcpy(caContent,"");
              }
              
               if(iFind>0){
               sprintf(caSubject,"搜索[%s]",psAcbmPat_keyword[psMatch[0]].data);
                	        	  
               	        	pasDbExecSqlF("insert into ncscasemesg (did,uid,udisp,sip,position,stime,cid,message) \
               	        	 values(%d,%d,'%s',%lu,'',%d,%d,'%s')", \
               	        	 pData[i].did,pData[i].uid,pData[i].caDispname,pData[i].lSip,pData[i].vtime,psAcbmPat_keyword[psMatch[0]].flags,caSubject);
               	        	 
               	      
               	    
               	 
               
               }

             
        		}
        		
        	}
//即时通讯搜索结束 







 

           	
        	 free(pData);

        }
        
   
     
  
  	sleep(3);
  }
}
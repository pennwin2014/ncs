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

int getHYCodebyName(char *name){
	long iReturn=99;
	if(strcmp(name,"党政机关")==0){
		iReturn=24;
	}
	else if(strcmp(name,"企事业单位")==0){
		iReturn=25;
	}
	else if(strcmp(name,"宾馆酒店")==0){
		iReturn=21;
	}
	else if(strcmp(name,"大专院校")==0){
		iReturn=26;
	}
	else if(strcmp(name,"中小学")==0){
		iReturn=28;
	}
	else if(strcmp(name,"餐饮购物休闲")==0){
		iReturn=23;
	}
	return iReturn;
}

char * getHYNamebyCode(long lCode){
	static char name[32];
	strcpy(name,"其它");
	if(lCode==24){
		strcpy(name,"党政机关");
	}
	else if(lCode==25){
	strcpy(name,"企事业单位");
	}
	else if(lCode==21){
	strcpy(name,"宾馆酒店");
	}
	else if(lCode==26){
	strcpy(name,"大专院校");
	}
	else if(lCode==28){
	strcpy(name,"中小学");
	}
	else if(lCode==23){
	strcpy(name,"餐饮购物休闲");
	}
    return &name[0];

}

char *getServicecodeByid(utShmHead *psShmHead,unsigned long lDid){
	static char code[32];
	ncsClient * psClient;
	memset(code,0,sizeof(code));
	psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lDid);           
  if(psClient){
  	strcpy(code,psClient->username);
  }
  return &code[0];
}

char *getServiceNameByCode(utShmHead *psShmHead,char *pCode){
	static char dispname[128];
	ncsuserCode2Id *psData;
	ncsClient * psClient;
	unsigned  long lDid;
	memset(dispname,0,sizeof(dispname));
	psData=(ncsuserCode2Id *)utShmHashLook(psShmHead,NCS_LNK_CLINTCODETOID,pCode);
	if(psData){
		lDid=psData->id;
	psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lDid);           
  if(psClient){
  	strcpy(dispname,psClient->dispname);
  }
  }
  return &dispname[0];
}



char *getServicecodeByGroupid(utShmHead *psShmHead,char *caGroupid){
        static char caReturn[8024];
        char caServicecode[32];
        int iReturn,lId,iNum;
         pasDbCursor *psCur;
         memset(caReturn,0,sizeof(caReturn));
              
                      iNum=0;
        //              printf("select groupid from ncsgroup where pid in(%s)\n ",caIds);
                     psCur = pasDbOpenSqlF("select username from ncsuser a,ncsgroup b where a.groupid=b.groupid and (b.groupid in (%s) or b.pid in(%s)) limit 0,300 ",caGroupid,caGroupid);
                     if(psCur){
                        iReturn=0;
                        memset(caServicecode,0,sizeof(caServicecode));
                        iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,16,caServicecode);
                        while(iReturn==0){
                                if(iNum==0){
                                        sprintf(caReturn,"\'%s\'",caServicecode);
                                }
                                else{
                                        sprintf(caReturn+strlen(caReturn),",\'%s\'",caServicecode);
                                }
                                 memset(caServicecode,0,sizeof(caServicecode));
                                iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,16,caServicecode);

                                iNum++;

                        }
                     }
                           pasDbCloseCursor(psCur);

                             


                  return &caReturn[0];
}


char * getSwfsCodebyName(char *name){
	static char code[32];
	strcpy(code,"01");
	if(strcmp(name,"无线")==0){
		strcpy(code,"08");
	}
	else if(strcmp(name,"有线")==0){
		strcpy(code,"01");
	}
	else if(strcmp(name,"有线、无线")==0){
		strcpy(code,"09");
	}

	return &code[0];
}

char * getSwfsNamebyCode(char *code){
	static char name[32];
	strcpy(name,"有线");
	if(strcmp(code,"01")==0){
		strcpy(name,"有线");
    }
	else if(strcmp(code,"08")==0){
		strcpy(name,"无线");
	}
	else if(strcmp(code,"09")==0){
		strcpy(name,"有线、无线");
	}


    return &name[0];

}

int ncsSortOnId(char *p1,char *p2);
  typedef struct ncsKeywordStat_s {
            long id;
            char caKeyword[64];
            long sum;                //总记录数
            long csum;               //已处理记录数
   } ncsKeywordStat;
 //排序  按记录数排序
static int ncsUtlSortKeyword(char *p1,char *p2)
{
    int i;
    long  l;
   
    ncsKeywordStat psC1,psC2;
    memcpy(&psC1,p1,sizeof(ncsKeywordStat));
    memcpy(&psC2,p2,sizeof(ncsKeywordStat));    
    l=psC2.sum-psC1.sum;
    if(l>0) return 1;
    if(l<0) return -1;
    if(l==0) return 0;
  
} 


//显示虚拟身份布控信息
int ncsCaseDet_netid_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  long iNum;
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,14,
	                 "cid",            UT_TYPE_STRING, 10, caCid,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,4,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "nettype",UT_TYPE_STRING,10,caNtype,
			             "netid",UT_TYPE_STRING,30,caNetid,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel);  
	  if(strlen(caNetid)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caNetid,caTemp,63)); 
	  	strcpy(caNetid,caTemp_d);
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
	  		sprintf(caTemp,"select count(*) from ncscasenetid where cid=%lu and nettype=%lu and name='%s' ",atol(caCid),atol(caNtype),caNetid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("update ncscasenetid set flags=%lu,lasttime=%lu where cid=%lu and nettype=%lu and name='%s' ",atol(caStatus),time(0),atol(caCid),atol(caNtype),caNetid);
	  		}
	  		else{
	  			 pasDbExecSqlF("insert into ncscasenetid(cid,nettype,name,flags,lasttime) \
	       		values(%lu,%lu,'%s',%lu,%lu)",atol(caCid),atol(caNtype),caNetid,atol(caStatus),time(0));
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncscasenetid where cid=%lu and nettype=%lu and name='%s' and sid!=%s ",atol(caCid),atol(caNtype),caNetid,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("delete from ncscasenetid where cid=%lu and nettype=%lu and name='%s' ",atol(caCid),atol(caNtype),caNetid);
	  			pasDbExecSqlF("update ncscasenetid set flags=%lu,lasttime=%lu,nettype=%lu,name='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
	  		}
	  		else{
	  				pasDbExecSqlF("update ncscasenetid set flags=%lu,lasttime=%lu,nettype=%lu,name='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("update ncscasenetid set flags=8,lasttime=%lu where sid in(%s)",time(0),caSelsid);
	  }
	  
	  
	  
	  
	  sprintf(caTemp,"select count(*) from ncscasenetid where 1=1 ");
    if(strlen(caCid)>0)
    sprintf(caTemp+strlen(caTemp)," and cid=%s and flags!=8 ",caCid);
    
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and name like '%c%s%c' ",'%',caKeyword,'%');
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select a.sid,a.nettype,a.name,b.name,a.flags from ncscasenetid a left join ncsnettype b on (a.nettype=b.ntype) where 1=1 ");
    	if(strlen(caCid)>0)
    	sprintf(caTemp+strlen(caTemp)," and a.cid=%s and a.flags!=8 ",caCid);
    	
     if(strlen(caKeyword)>0){
    	 sprintf(caTemp+strlen(caTemp)," and a.name like '%c%s%c' ",'%',caKeyword,'%');
     }
    	sprintf(caTemp+strlen(caTemp)," order by a.sid desc limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
   	 sid=0;
   	 memset(caName,0,sizeof(caName));
   	 memset(caNetname,0,sizeof(caNetname));
   	 nettype=0;
   	 flags=0;
   	 
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&sid,
      	                               UT_TYPE_LONG,4,&nettype,
      	                               UT_TYPE_STRING,63,caName,
      	                               UT_TYPE_STRING,31,caNetname,
      	                               UT_TYPE_LONG,4,&flags);
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);
        utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",flags);
        utPltPutLoopVar(psDbHead,"nettype",iNum,caNetname);
        utPltPutLoopVarF(psDbHead,"typeid",iNum,"%lu",nettype);
        utPltPutLoopVar(psDbHead,"netid",iNum,caName);
				if(flags==0){
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"启用");
				}
				else{
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"停用");
				}
			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_netid_list.htm");
	  return 0;
}



//显示虚拟身份类别组合框选择
int ncsWebNettypeComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[32],caGroupid[16];
   	
   	char caName[32];
   	char caWhere[256];
   	char caSql[256];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
   	long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caQuery[256],caTemp[1024];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	long lId;
   	char *p,*pVar;
   	long lNtype;
   	char caType_flag[16];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    
    	iReturn = utMsgGetSomeNVar(psMsgHead,1,
					"type_flag",      UT_TYPE_STRING, 10, caType_flag);
					
					
	//				printf("caQuery=%s\n",pasCvtGBK(2,caQuery,caTemp,255));
    
    
    
    



    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",300);
    lStartRec = (lCurPg - 1) * lRowNum;  
    
//printf("select name,ntype from ncsnettype  order by ntype \n");
   sprintf(caTemp,"select name,ntype from ncsnettype where 1=1 ");
   if(atol(caType_flag)==1000){
   	sprintf(caTemp+strlen(caTemp)," and ntype>=1002 and ntype<=1999 "); 
   }
   else if(atol(caType_flag)==2000){
   	sprintf(caTemp+strlen(caTemp)," and ntype>=2000 and ntype<=2999 ");
  }
  else if(atol(caType_flag)==20000){
//  	sprintf(caTemp+strlen(caTemp)," and ntype>=20000 and ntype<=29999 ");
      sprintf(caTemp+strlen(caTemp)," and gid=20 ");
  }
  else if(atol(caType_flag)==21000){
  	  sprintf(caTemp+strlen(caTemp)," and gid=21 ");
  	}
  	else if(atol(caType_flag)==24000){
  		sprintf(caTemp+strlen(caTemp)," and gid=24 ");
  	}
  	else if(atol(caType_flag)==22000){
  		sprintf(caTemp+strlen(caTemp)," and gid=22 ");
  	}
  	else if(atol(caType_flag)==23000){
  		sprintf(caTemp+strlen(caTemp)," and gid=23 ");
  	}
  	else if(atol(caType_flag)==25000){
  		sprintf(caTemp+strlen(caTemp)," and gid=25 ");
  	}
  else if(atol(caType_flag)==13000){
    sprintf(caTemp+strlen(caTemp)," and ((ntype>=13000 and ntype<=13999) or gid=13) ");
  } 	
  sprintf(caTemp+strlen(caTemp)," order by ntype ");
   	
  	 psCur = pasDbOpenSql(caTemp,0);

    if(psCur == NULL) {
    	  
        return 0;
    }
    psDbHead = utPltInitDb();
    
     
    memset(caName,0,sizeof(caName));
    lNtype=0;
    
        utPltPutLoopVar(psDbHead,"classname", 1,"全部");
        utPltPutLoopVar(psDbHead,"classid",   1,"");
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
                                   UT_TYPE_LONG,4,&lNtype);
                                  
    iNum = 1;
    while(iReturn == 0) {
        
        if(iNum>0){
    				utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
    	   }
    	
        iNum++;
        
        utPltPutLoopVar(psDbHead,"classname", iNum,caName);
        utPltPutLoopVarF(psDbHead,"classid",   iNum,"%ld",lNtype);
      
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lNtype);
    }
    pasDbCloseCursor(psCur); 
    utPltPutVarF(psDbHead,"TotRec","%d",iNum);  
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_nettype_list.htm");
      
    return 0;
}



//显示网址类别组合框选择
int ncsWebSitetypeComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[32],caGroupid[16];
   	
   	char caName[32];
   	char caWhere[256];
   	char caSql[256];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
   	long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caQuery[256],caTemp[1024];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	long lId;
   	char *p,*pVar;
   	long lNtype;
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    
    	iReturn = utMsgGetSomeNVar(psMsgHead,1,
					"query",      UT_TYPE_STRING, 127, caQuery);
					
					
	//				printf("caQuery=%s\n",pasCvtGBK(2,caQuery,caTemp,255));
    
    
    
    


 
    
//printf("select name,ntype from ncsnettype  order by ntype \n");
   
  	 psCur = pasDbOpenSqlF("select name,id from ncwebclass  order by id ");

    if(psCur == NULL) {
    	  
        return 0;
    }
    psDbHead = utPltInitDb();
    
     
    memset(caName,0,sizeof(caName));
    lNtype=0;
    

    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
                                   UT_TYPE_LONG,4,&lNtype);
                                  
    iNum = 1;
    
        utPltPutLoopVar(psDbHead,"classname", 1,"不限");
        utPltPutLoopVarF(psDbHead,"classid",   1,"%ld",0);
    
    while(iReturn == 0) {
        
        if(iNum>0){
    				utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
    	   }
    	
        iNum++;
        
        utPltPutLoopVar(psDbHead,"classname", iNum,caName);
        utPltPutLoopVarF(psDbHead,"classid",   iNum,"%ld",lNtype);
      
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lNtype);
    }
    pasDbCloseCursor(psCur); 
    utPltPutVarF(psDbHead,"TotRec","%d",iNum);  
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_nettype_list.htm");
      
    return 0;
}

//显示关键字库类别
int ncsKeywordtypeComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[32],caGroupid[16];
   	
   	char caName[32];
   	char caWhere[256];
   	char caSql[256];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
   	long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caQuery[256],caTemp[1024];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	long lId;
   	char *p,*pVar;
   	long lNtype;
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    
    	iReturn = utMsgGetSomeNVar(psMsgHead,1,
					"query",      UT_TYPE_STRING, 127, caQuery);
					
					
	//				printf("caQuery=%s\n",pasCvtGBK(2,caQuery,caTemp,255));
    
    
    
    


 
    
//printf("select name,ntype from ncsnettype  order by ntype \n");
   
  	 psCur = pasDbOpenSqlF("select name,id from ncskeyindex where pid=400  order by id ");

    if(psCur == NULL) {
    	  
        return 0;
    }
    psDbHead = utPltInitDb();
    
     
    memset(caName,0,sizeof(caName));
    lNtype=0;
    

    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
                                   UT_TYPE_LONG,4,&lNtype);
                                  
    iNum = 1;
    
        utPltPutLoopVar(psDbHead,"classname", 1,"不限");
        utPltPutLoopVarF(psDbHead,"classid",   1,"%ld",0);
    
    while(iReturn == 0) {
        
        if(iNum>0){
    				utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
    	   }
    	
        iNum++;
        
        utPltPutLoopVar(psDbHead,"classname", iNum,caName);
        utPltPutLoopVarF(psDbHead,"classid",   iNum,"%ld",lNtype);
      
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lNtype);
    }
    pasDbCloseCursor(psCur); 
    utPltPutVarF(psDbHead,"TotRec","%d",iNum);  
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_nettype_list.htm");
      
    return 0;
}





//显示网址布控信息
int ncsCaseDet_website_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  long iNum;
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,14,
	                 "cid",            UT_TYPE_STRING, 10, caCid,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,4,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "nettype",UT_TYPE_STRING,10,caNtype,
			             "netid",UT_TYPE_STRING,30,caNetid,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel);  
	  if(strlen(caNetid)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caNetid,caTemp,63)); 
	  	strcpy(caNetid,caTemp_d);
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
	  		sprintf(caTemp,"select count(*) from ncscaseweb where cid=%lu and urlid=%lu and url='%s' ",atol(caCid),atol(caNtype),caNetid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("update ncscaseweb set flags=%lu,lasttime=%lu where cid=%lu and urlid=%lu and url='%s' ",atol(caStatus),time(0),atol(caCid),atol(caNtype),caNetid);
	  		}
	  		else{
	  			 pasDbExecSqlF("insert into ncscaseweb(cid,urlid,url,flags,lasttime) \
	       		values(%lu,%lu,'%s',%lu,%lu)",atol(caCid),atol(caNtype),caNetid,atol(caStatus),time(0));
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncscaseweb where cid=%lu and urlid=%lu and url='%s' and sid!=%s ",atol(caCid),atol(caNtype),caNetid,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("delete from ncscaseweb where cid=%lu and urlid=%lu and url='%s' ",atol(caCid),atol(caNtype),caNetid);
	  			pasDbExecSqlF("update ncscaseweb set flags=%lu,lasttime=%lu,urlid=%lu,url='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
	  		}
	  		else{
	  				pasDbExecSqlF("update ncscaseweb set flags=%lu,lasttime=%lu,urlid=%lu,url='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("update ncscaseweb set flags=8,lasttime=%lu where sid in(%s)",time(0),caSelsid);
	  }
	  
	  
	  
	  
	  sprintf(caTemp,"select count(*) from ncscaseweb where 1=1 ");
    if(strlen(caCid)>0)
    sprintf(caTemp+strlen(caTemp)," and cid=%s and flags!=8 ",caCid);
    
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and url like '%c%s%c' ",'%',caKeyword,'%');
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select a.sid,a.urlid,a.url,b.name,a.flags from ncscaseweb a left join ncwebclass b on (a.urlid=b.id) where 1=1 ");
    	if(strlen(caCid)>0)
    	sprintf(caTemp+strlen(caTemp)," and a.cid=%s and a.flags!=8 ",caCid);
    	
     if(strlen(caKeyword)>0){
    	 sprintf(caTemp+strlen(caTemp)," and a.url like '%c%s%c' ",'%',caKeyword,'%');
     }
    	sprintf(caTemp+strlen(caTemp)," order by a.sid desc limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
   	 sid=0;
   	 memset(caName,0,sizeof(caName));
   	 memset(caNetname,0,sizeof(caNetname));
   	 nettype=0;
   	 flags=0;
   	 
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&sid,
      	                               UT_TYPE_LONG,4,&nettype,
      	                               UT_TYPE_STRING,63,caName,
      	                               UT_TYPE_STRING,31,caNetname,
      	                               UT_TYPE_LONG,4,&flags);
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);
        utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",flags);
        utPltPutLoopVar(psDbHead,"nettype",iNum,caNetname);
        utPltPutLoopVarF(psDbHead,"typeid",iNum,"%lu",nettype);
        utPltPutLoopVar(psDbHead,"netid",iNum,caName);
				if(flags==0){
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"启用");
				}
				else{
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"停用");
				}
			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_netid_list.htm");
	  return 0;
}




//显示使用者特征布控信息
int ncsCaseDet_user_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  long iNum;
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,14,
	                 "cid",            UT_TYPE_STRING, 10, caCid,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,4,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "nettype",UT_TYPE_STRING,10,caNtype,
			             "netid",UT_TYPE_STRING,30,caNetid,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel);  
	  if(strlen(caNetid)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caNetid,caTemp,63)); 
	  	strcpy(caNetid,caTemp_d);
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
	  		sprintf(caTemp,"select count(*) from ncscaseuser where cid=%lu and ctype=%lu and cvalue='%s' ",atol(caCid),atol(caNtype),caNetid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("update ncscaseuser set flags=%lu,lasttime=%lu where cid=%lu and ctype=%lu and cvalue='%s' ",atol(caStatus),time(0),atol(caCid),atol(caNtype),caNetid);
	  		}
	  		else{
	  			 pasDbExecSqlF("insert into ncscaseuser(cid,ctype,cvalue,flags,lasttime) \
	       		values(%lu,%lu,'%s',%lu,%lu)",atol(caCid),atol(caNtype),caNetid,atol(caStatus),time(0));
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncscaseuser where cid=%lu and ctype=%lu and cvalue='%s' and sid!=%s ",atol(caCid),atol(caNtype),caNetid,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("delete from ncscaseuser where cid=%lu and ctype=%lu and cvalue='%s' ",atol(caCid),atol(caNtype),caNetid);
	  			pasDbExecSqlF("update ncscaseuser set flags=%lu,lasttime=%lu,ctype=%lu,cvalue='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
	  		}
	  		else{
	  				pasDbExecSqlF("update ncscaseuser set flags=%lu,lasttime=%lu,ctype=%lu,cvalue='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("update ncscaseuser set flags=8,lasttime=%lu where sid in(%s)",time(0),caSelsid);
	  }
	  
	  
	  
	  
	  sprintf(caTemp,"select count(*) from ncscaseuser where 1=1 ");
    if(strlen(caCid)>0)
    sprintf(caTemp+strlen(caTemp)," and cid=%s and flags!=8 ",caCid);
    
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and cvalue like '%c%s%c' ",'%',caKeyword,'%');
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select a.sid,a.ctype,a.cvalue,'',a.flags from ncscaseuser a  where 1=1 ");
    	if(strlen(caCid)>0)
    	sprintf(caTemp+strlen(caTemp)," and a.cid=%s and a.flags!=8 ",caCid);
    	
     if(strlen(caKeyword)>0){
    	 sprintf(caTemp+strlen(caTemp)," and a.cvalue like '%c%s%c' ",'%',caKeyword,'%');
     }
    	sprintf(caTemp+strlen(caTemp)," order by a.sid desc limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
   	 sid=0;
   	 memset(caName,0,sizeof(caName));
   	 memset(caNetname,0,sizeof(caNetname));
   	 nettype=0;
   	 flags=0;
   	 
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&sid,
      	                               UT_TYPE_LONG,4,&nettype,
      	                               UT_TYPE_STRING,63,caName,
      	                               UT_TYPE_STRING,31,caNetname,
      	                               UT_TYPE_LONG,4,&flags);
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);
        utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",flags);
        if(nettype==1){
        	strcpy(caNetname,"登录名");
        }
        else if(nettype==2){
        	strcpy(caNetname,"显示名");
        }
        else if(nettype==3){
        	strcpy(caNetname,"计算机名");
        }
        else if(nettype==4){
        	strcpy(caNetname,"MAC地址");
        }
        else if(nettype==5){
        	strcpy(caNetname,"证件号");
        }
        else{
        	strcpy(caNetname,"");
        }
       utPltPutLoopVar(psDbHead,"nettype",iNum,caNetname);
        utPltPutLoopVarF(psDbHead,"typeid",iNum,"%lu",nettype);
        utPltPutLoopVar(psDbHead,"netid",iNum,caName);
				if(flags==0){
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"启用");
				}
				else{
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"停用");
				}
			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_netid_list.htm");
	  return 0;
}



//显示关键字布控信息
int ncsCaseDet_keyword_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount,lCount_mail,lCount_form,lCount_im;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags,lStartRec_im,lStartRec_mail;
	  char caSubj[64];
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16],caLogid[16];
	  long iNum,sum,i,num;
	  char caObj[100][50],caTsid[16],caTlogid[16];
	  
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,15,
	                 "cid",            UT_TYPE_STRING, 10, caCid,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,4,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "nettype",UT_TYPE_STRING,10,caNtype,
			             "netid",UT_TYPE_STRING,30,caNetid,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel,
			             "log_id",UT_TYPE_STRING,10,caLogid);  
			             
		           
			             
			             
	  if(strlen(caNetid)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caNetid,caTemp,63)); 
	  	strcpy(caNetid,caTemp_d);
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
				  		//表单        
				  		sprintf(caTemp,"select count(*) from ncscasepost where cid=%lu and contid=%lu and cont='%s' ",atol(caCid),atol(caNtype),caNetid);
				  		lCount=0;
				  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
				  		if(lCount>0){
				  			pasDbExecSqlF("update ncscasepost set flags=%lu,lasttime=%lu where cid=%lu and contid=%lu and cont='%s' ",atol(caStatus),time(0),atol(caCid),atol(caNtype),caNetid);
				  		}
				  		else{
				  			 pasDbExecSqlF("insert into ncscasepost(cid,url,urlid,contid,cont,flags,lasttime) \
				       		values(%lu,'',0,%lu,'%s',%lu,%lu)",atol(caCid),atol(caNtype),caNetid,atol(caStatus),time(0));
				  		}
				  		
				  			//即时通讯        
					  		sprintf(caTemp,"select count(*) from ncscaseim where cid=%lu and contid=%lu and cont='%s' ",atol(caCid),atol(caNtype),caNetid);
					  		lCount=0;
					  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
					  		if(lCount>0){
					  			pasDbExecSqlF("update ncscaseim set flags=%lu,lasttime=%lu where cid=%lu and contid=%lu and cont='%s' ",atol(caStatus),time(0),atol(caCid),atol(caNtype),caNetid);
					  		}
					  		else{
					  			 pasDbExecSqlF("insert into ncscaseim(cid,nettype,contid,cont,flags,lasttime) \
					       		values(%lu,0,%lu,'%s',%lu,%lu)",atol(caCid),atol(caNtype),caNetid,atol(caStatus),time(0));
					  		}
				  			//邮件内容       
					  		sprintf(caTemp,"select count(*) from ncscasemail where cid=%lu and contid=%lu and cont='%s' ",atol(caCid),atol(caNtype),caNetid);
					  		lCount=0;
					  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
					  		if(lCount>0){
					  			pasDbExecSqlF("update ncscasemail set flags=%lu,lasttime=%lu where cid=%lu and contid=%lu and cont='%s' ",atol(caStatus),time(0),atol(caCid),atol(caNtype),caNetid);
					  		}
					  		else{
					  			 pasDbExecSqlF("insert into ncscasemail(cid,mfrom,mto,subj,ctype,contid,cont,flags,lasttime) \
					       		values(%lu,'%s','%s','%s',0,%lu,'%s',%lu,%lu)",atol(caCid),"","","",atol(caNtype),caNetid,atol(caStatus),time(0));
					  		}
				  		
				  			//邮件主题       
					  		sprintf(caTemp,"select count(*) from ncscasemail where cid=%lu and contid=%lu and subj='%s' ",atol(caCid),atol(caNtype),caNetid);
					  		lCount=0;
					  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
					  		if(lCount>0){
					  			pasDbExecSqlF("update ncscasemail set flags=%lu,lasttime=%lu where cid=%lu and contid=%lu and cont='%s' ",atol(caStatus),time(0),atol(caCid),atol(caNtype),caNetid);
					  		}
					  		else{
					  			 pasDbExecSqlF("insert into ncscasemail(cid,mfrom,mto,subj,ctype,contid,cont,flags,lasttime) \
					       		values(%lu,'%s','%s','%s',0,%lu,'%s',%lu,%lu)",atol(caCid),"","",caNetid,atol(caNtype),"",atol(caStatus),time(0));
					  		}
				  		
				  		
				  		
				  		
				  	}
				  	else{
				  		if(atol(caLogid)==1){     //处理表单
							  		sprintf(caTemp,"select count(*) from ncscasepost where cid=%lu and contid=%lu and cont='%s' and sid!=%s ",atol(caCid),atol(caNtype),caNetid,caSid);
							  		lCount=0;
							  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
							  		if(lCount>0){
							  			pasDbExecSqlF("delete from ncscasepost where cid=%lu and contid=%lu and cont='%s' ",atol(caCid),atol(caNtype),caNetid);
							  			pasDbExecSqlF("update ncscasepost set flags=%lu,lasttime=%lu,contid=%lu,cont='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
							  		}
							  		else{
							  				pasDbExecSqlF("update ncscasepost set flags=%lu,lasttime=%lu,contid=%lu,cont='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
							  		}
							  }
							  else if(atol(caLogid)==2){       //修改IM策略
							  	
							  		sprintf(caTemp,"select count(*) from ncscaseim where cid=%lu and contid=%lu and cont='%s' and sid!=%s ",atol(caCid),atol(caNtype),caNetid,caSid);
							  		lCount=0;
							  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
							  		if(lCount>0){
							  			pasDbExecSqlF("delete from ncscaseim where cid=%lu and contid=%lu and cont='%s' ",atol(caCid),atol(caNtype),caNetid);
							  			pasDbExecSqlF("update ncscaseim set flags=%lu,lasttime=%lu,contid=%lu,cont='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
							  		}
							  		else{
							  				pasDbExecSqlF("update ncscaseim set flags=%lu,lasttime=%lu,contid=%lu,cont='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
							  		}
							  	
							  }
							  else if(atol(caLogid)==3){         //修改邮件主题
							  		sprintf(caTemp,"select count(*) from ncscasemail where cid=%lu and contid=%lu and subj='%s' and sid!=%s ",atol(caCid),atol(caNtype),caNetid,caSid);
							  		lCount=0;
							  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
							  		if(lCount>0){
							  			pasDbExecSqlF("delete from ncscasemail where cid=%lu and contid=%lu and subj='%s' ",atol(caCid),atol(caNtype),caNetid);
							  			pasDbExecSqlF("update ncscasemail set flags=%lu,lasttime=%lu,contid=%lu,subj='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
							  		}
							  		else{
							  				pasDbExecSqlF("update ncscasemail set flags=%lu,lasttime=%lu,contid=%lu,subj='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
							  		}
							  	
							  }
							  else if(atol(caLogid)==4){        //修改邮件内容策略
							  		sprintf(caTemp,"select count(*) from ncscasemail where cid=%lu and contid=%lu and cont='%s' and sid!=%s ",atol(caCid),atol(caNtype),caNetid,caSid);
							  		lCount=0;
							  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
							  		if(lCount>0){
							  			pasDbExecSqlF("delete from ncscasemail where cid=%lu and contid=%lu and cont='%s' ",atol(caCid),atol(caNtype),caNetid);
							  			pasDbExecSqlF("update ncscasemail set flags=%lu,lasttime=%lu,contid=%lu,cont='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
							  		}
							  		else{
							  				pasDbExecSqlF("update ncscasemail set flags=%lu,lasttime=%lu,contid=%lu,cont='%s' where sid=%s ",atol(caStatus),time(0),atol(caNtype),caNetid,caSid);
							  		}
							  }
				  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	
	  		 num=ncs_SepChar0(caSelsid,',',caObj); 
	  		 for(i=0;i<num;i++){
	  		 	utStrGetSomeWordBy(caObj[i],"_",2,UT_TYPE_STRING,10,caTsid,UT_TYPE_STRING,10,caTlogid);
	  		 	if(atol(caTlogid)==1){
	  		 		pasDbExecSqlF("update ncscasepost set flags=8,lasttime=%lu where sid=%s",time(0),caTsid);
	  		 	}
	  		 	else if(atol(caTlogid)==2){
	  		 		pasDbExecSqlF("update ncscaseim set flags=8,lasttime=%lu where sid=%s",time(0),caTsid);
	  		 	}
	  		 	else if((atol(caTlogid)==3)||(atol(caTlogid)==4)){
	  		 	 pasDbExecSqlF("update ncscasemail set flags=8,lasttime=%lu where sid=%s",time(0),caTsid);
	  		 	}
	  		 	
	  		}
	  	
	  	
	  }
	  
	  
	  
	//表单  
	  sprintf(caTemp,"select count(*) from ncscasepost where (cont!='' or contid>0) ");
    if(strlen(caCid)>0)
    sprintf(caTemp+strlen(caTemp)," and cid=%s and flags!=8 ",caCid);
    
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and cont like '%c%s%c' ",'%',caKeyword,'%');
    }
    lCount_form=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount_form);
	
	//邮件
	 	  sprintf(caTemp,"select count(*) from ncscasemail where (cont!='' or contid>0 or subj!='') ");
	    if(strlen(caCid)>0)
	    sprintf(caTemp+strlen(caTemp)," and cid=%s and flags!=8 ",caCid);
	    
	    if(strlen(caKeyword)>0){
	    	sprintf(caTemp+strlen(caTemp)," and (cont like '%c%s%c' or subj like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');
	    }
	    lCount_mail=0;
	    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount_mail);
	
	  	//IM
	 	  sprintf(caTemp,"select count(*) from ncscaseim where (cont!='' or contid>0 ) ");
	    if(strlen(caCid)>0)
	    sprintf(caTemp+strlen(caTemp)," and cid=%s and flags!=8 ",caCid);
	    
	    if(strlen(caKeyword)>0){
	    	sprintf(caTemp+strlen(caTemp)," and (cont like '%c%s%c') ",'%',caKeyword,'%');
	    }
	    lCount_im=0;
	    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount_im);
	    lCount=lCount_form+lCount_mail+lCount_im;
	  

	  
//表单	  
	    sprintf(caTemp,"select a.sid,a.contid,a.cont,b.name,a.flags from ncscasepost a left join ncskeyindex  b on (a.contid=b.id) where (a.cont!='' or a.contid>0) ");
    	if(strlen(caCid)>0)
    	sprintf(caTemp+strlen(caTemp)," and a.cid=%s and a.flags!=8 ",caCid);
    	
     if(strlen(caKeyword)>0){
    	 sprintf(caTemp+strlen(caTemp)," and a.cont like '%c%s%c' ",'%',caKeyword,'%');
     }
    	sprintf(caTemp+strlen(caTemp)," order by a.sid desc limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
   	 sid=0;
   	 memset(caName,0,sizeof(caName));
   	 memset(caNetname,0,sizeof(caNetname));
   	 nettype=0;
   	 flags=0;
   	 
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&sid,
      	                               UT_TYPE_LONG,4,&nettype,
      	                               UT_TYPE_STRING,63,caName,
      	                               UT_TYPE_STRING,31,caNetname,
      	                               UT_TYPE_LONG,4,&flags);
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);
        utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",flags);
        utPltPutLoopVar(psDbHead,"nettype",iNum,caNetname);
        utPltPutLoopVarF(psDbHead,"typeid",iNum,"%lu",nettype);
        utPltPutLoopVar(psDbHead,"netid",iNum,caName);
				if(flags==0){
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"启用");
				}
				else{
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"停用");
				}
				utPltPutLoopVar(psDbHead,"log_id",iNum,"1");
				utPltPutLoopVar(psDbHead,"log_name",iNum,"表单内容");
			
    }
    pasDbCloseCursor(psCur);

//即时通讯	  
    if(lCount_form+lCount_im>lStartRec&&lCount_im>0&&iNum<lRowNum){
    	lStartRec_im=lStartRec-lCount_form;
    	if(lStartRec_im<0) lStartRec_im=0;
    	
    		    sprintf(caTemp,"select a.sid,a.contid,a.cont,b.name,a.flags from ncscaseim a left join ncskeyindex  b on (a.contid=b.id) where (a.cont!='' or a.contid>0) ");
			    	if(strlen(caCid)>0)
			    	sprintf(caTemp+strlen(caTemp)," and a.cid=%s and a.flags!=8 ",caCid);
			    	
			     if(strlen(caKeyword)>0){
			    	 sprintf(caTemp+strlen(caTemp)," and a.cont like '%c%s%c' ",'%',caKeyword,'%');
			     }
			    	sprintf(caTemp+strlen(caTemp)," order by a.sid desc limit %d,%d",lStartRec_im,lRowNum);
			    	
			  	printf("caTemp=%s\n",caTemp);
			    	
			
			        psCur = pasDbOpenSql(caTemp,0);
			   
			    if(psCur == NULL) {
			        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
			        return 0;
			    }
			   
			   	                              
			   	iReturn=0;
			   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
			   	
			   	 sid=0;
			   	 memset(caName,0,sizeof(caName));
			   	 memset(caNetname,0,sizeof(caNetname));
			   	 nettype=0;
			   	 flags=0;
			   	 
			   	 iReturn = pasDbFetchInto(psCur,
			      	                               UT_TYPE_LONG,4,&sid,
			      	                               UT_TYPE_LONG,4,&nettype,
			      	                               UT_TYPE_STRING,63,caName,
			      	                               UT_TYPE_STRING,31,caNetname,
			      	                               UT_TYPE_LONG,4,&flags);
			      	                              
			      	                                                             
			      	                              
			      	                              
			   	if((iReturn!=0)&&(iReturn!=1405)) break;	
			        iNum++;
			       if(iNum>1){
			       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
			       }
			        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
			        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);
			        utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",flags);
			        utPltPutLoopVar(psDbHead,"nettype",iNum,caNetname);
			        utPltPutLoopVarF(psDbHead,"typeid",iNum,"%lu",nettype);
			        utPltPutLoopVar(psDbHead,"netid",iNum,caName);
							if(flags==0){
								utPltPutLoopVar(psDbHead,"flag_desc",iNum,"启用");
							}
							else{
								utPltPutLoopVar(psDbHead,"flag_desc",iNum,"停用");
							}
						  utPltPutLoopVar(psDbHead,"log_id",iNum,"2");
				      utPltPutLoopVar(psDbHead,"log_name",iNum,"聊天内容");
						
			    }
			    pasDbCloseCursor(psCur);
    	
    	
    }

//邮件关键字策略	  
    if(lCount_form+lCount_im+lCount_mail>lStartRec&&lCount_mail>0&&iNum<lRowNum){
    	lStartRec_mail=lStartRec-lCount_form-lCount_im;
    	if(lStartRec_mail<0) lStartRec_mail=0;
    	
    		    sprintf(caTemp,"select a.sid,a.contid,a.cont,b.name,a.flags,a.subj from ncscasemail a left join ncskeyindex  b on (a.contid=b.id) where (a.cont!='' or a.contid>0 or a.subj!='') ");
			    	if(strlen(caCid)>0)
			    	sprintf(caTemp+strlen(caTemp)," and a.cid=%s and a.flags!=8 ",caCid);
			    	
			     if(strlen(caKeyword)>0){
			    	 sprintf(caTemp+strlen(caTemp)," and (a.cont like '%c%s%c' or a.subj like '%c%s%c' ",'%',caKeyword,'%','%',caKeyword,'%');
			     }
			    	sprintf(caTemp+strlen(caTemp)," order by a.sid desc limit %d,%d",lStartRec_mail,lRowNum);
			    	
			  	printf("caTemp=%s\n",caTemp);
			    	
			
			        psCur = pasDbOpenSql(caTemp,0);
			   
			    if(psCur == NULL) {
			        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
			        return 0;
			    }
			   
			   	                              
			   	iReturn=0;
			   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
			   	
			   	 sid=0;
			   	 memset(caName,0,sizeof(caName));
			   	 memset(caNetname,0,sizeof(caNetname));
			   	 
			   	 nettype=0;
			   	 flags=0;
			   	 memset(caSubj,0,sizeof(caSubj));
			   	 iReturn = pasDbFetchInto(psCur,
			      	                               UT_TYPE_LONG,4,&sid,
			      	                               UT_TYPE_LONG,4,&nettype,
			      	                               UT_TYPE_STRING,63,caName,
			      	                               UT_TYPE_STRING,31,caNetname,
			      	                               UT_TYPE_LONG,4,&flags,
			      	                               UT_TYPE_STRING,63,caSubj);
			      	                              
			      	                                                             
			      	                              
			      	                              
			   	if((iReturn!=0)&&(iReturn!=1405)) break;	
			        iNum++;
			       if(iNum>1){
			       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
			       }
			        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
			        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);
			        utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",flags);
			        utPltPutLoopVar(psDbHead,"nettype",iNum,caNetname);
			        utPltPutLoopVarF(psDbHead,"typeid",iNum,"%lu",nettype);
			        if(strlen(caName)==0){
			        	strcpy(caName,caSubj);
			        	utPltPutLoopVar(psDbHead,"log_id",iNum,"3");
				        utPltPutLoopVar(psDbHead,"log_name",iNum,"邮件主题");
			        }
			        else{
			        	utPltPutLoopVar(psDbHead,"log_id",iNum,"4");
				        utPltPutLoopVar(psDbHead,"log_name",iNum,"邮件正文");
			        }
			        utPltPutLoopVar(psDbHead,"netid",iNum,caName);
							if(flags==0){
								utPltPutLoopVar(psDbHead,"flag_desc",iNum,"启用");
							}
							else{
								utPltPutLoopVar(psDbHead,"flag_desc",iNum,"停用");
							}

						
			    }
			    pasDbCloseCursor(psCur);
    	
    	
    }







	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	// utPltShowDb(psDbHead);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_netid_list.htm");
	  return 0;
}



//显示敏感服务类别
int ncsSenserviceComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[32],caGroupid[16];
   	
   	char caName[32];
   	char caWhere[256];
   	char caSql[256];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
   	long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caQuery[256],caTemp[1024];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	long lId;
   	char *p,*pVar;
   	long lNtype;
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    
    	iReturn = utMsgGetSomeNVar(psMsgHead,1,
					"query",      UT_TYPE_STRING, 127, caQuery);
					
					
	//				printf("caQuery=%s\n",pasCvtGBK(2,caQuery,caTemp,255));
    
    
    
    


 
    
//printf("select name,ntype from ncsnettype  order by ntype \n");
   
  	 psCur = pasDbOpenSqlF("select name,ntype from ncssenservice  order by name ");

    if(psCur == NULL) {
    	  
        return 0;
    }
    psDbHead = utPltInitDb();
    
     
    memset(caName,0,sizeof(caName));
    lNtype=0;
    

    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
                                   UT_TYPE_LONG,4,&lNtype);
                                  
    iNum = 0;
    
       
    while(iReturn == 0) {
        
        if(iNum>0){
    				utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
    	   }
    	
        iNum++;
        
        utPltPutLoopVar(psDbHead,"classname", iNum,caName);
        utPltPutLoopVarF(psDbHead,"classid",   iNum,"%ld",lNtype);
      
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lNtype);
    }
    pasDbCloseCursor(psCur); 
    utPltPutVarF(psDbHead,"TotRec","%d",iNum);  
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_nettype_list.htm");
      
    return 0;
}



//显示敏感服务布控信息
int ncsCaseDet_service_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  long iNum;
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,14,
	                 "cid",            UT_TYPE_STRING, 10, caCid,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,4,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "nettype",UT_TYPE_STRING,10,caNtype,
			             "netid",UT_TYPE_STRING,30,caNetid,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel);  
	  if(strlen(caNetid)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caNetid,caTemp,63)); 
	  	strcpy(caNetid,caTemp_d);
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
	  		sprintf(caTemp,"select count(*) from ncscaseservice where cid=%lu and nettype=%lu  ",atol(caCid),atol(caNtype));
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("update ncscaseservice set flags=%lu,lasttime=%lu where cid=%lu and nettype=%lu  ",atol(caStatus),time(0),atol(caCid),atol(caNtype));
	  		}
	  		else{

	  			 pasDbExecSqlF("insert into ncscaseservice(cid,nettype,flags,lasttime) \
	       		values(%lu,%lu,%lu,%lu)",atol(caCid),atol(caNtype),atol(caStatus),time(0));
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncscaseservice where cid=%lu and nettype=%lu  and sid!=%s ",atol(caCid),atol(caNtype),caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("delete from ncscaseservice where cid=%lu and nettype=%lu  ",atol(caCid),atol(caNtype));
	  			pasDbExecSqlF("update ncscaseservice set flags=%lu,lasttime=%lu,nettype=%lu where sid=%s ",atol(caStatus),time(0),atol(caNtype),caSid);
	  		}
	  		else{
	  				pasDbExecSqlF("update ncscaseservice set flags=%lu,lasttime=%lu,nettype=%lu where sid=%s ",atol(caStatus),time(0),atol(caNtype),caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("update ncscaseservice set flags=8,lasttime=%lu where sid in(%s)",time(0),caSelsid);
	  }
	  
	  
	  
	  
	  sprintf(caTemp,"select count(*) from ncscaseservice where 1=1 ");
    if(strlen(caCid)>0)
    sprintf(caTemp+strlen(caTemp)," and cid=%s and flags!=8 ",caCid);
    
   
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select a.sid,a.nettype,'',b.name,a.flags from ncscaseservice a left join ncssenservice b on (a.nettype=b.ntype) where 1=1 ");
    	if(strlen(caCid)>0)
    	sprintf(caTemp+strlen(caTemp)," and a.cid=%s and a.flags!=8 ",caCid);
    	
   
    	sprintf(caTemp+strlen(caTemp)," order by a.sid desc limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
   	 sid=0;
   	 memset(caName,0,sizeof(caName));
   	 memset(caNetname,0,sizeof(caNetname));
   	 nettype=0;
   	 flags=0;
   	 
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&sid,
      	                               UT_TYPE_LONG,4,&nettype,
      	                               UT_TYPE_STRING,63,caName,
      	                               UT_TYPE_STRING,31,caNetname,
      	                               UT_TYPE_LONG,4,&flags);
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);
        utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",flags);
        utPltPutLoopVar(psDbHead,"nettype",iNum,caNetname);
        utPltPutLoopVarF(psDbHead,"typeid",iNum,"%lu",nettype);
        utPltPutLoopVar(psDbHead,"netid",iNum,caName);
				if(flags==0){
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"启用");
				}
				else{
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"停用");
				}
			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_netid_list.htm");
	  return 0;
}


//显示IP端口布控信息
int ncsCaseDet_ipport_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  long iNum;
	  char caStartip[16],caEndip[16],caStartport[12],caEndport[12];
	  long lDlport,lDuport;
	  char caDlip[16],caDuip[16];
	  unsigned long lDlip,lDuip;
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,16,
	                 "cid",            UT_TYPE_STRING, 10, caCid,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,4,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "startip",UT_TYPE_STRING,15,caStartip,
			             "endip",UT_TYPE_STRING,15,caEndip,
			             "startport",UT_TYPE_STRING,10,caStartport,
			             "endport",UT_TYPE_STRING,10,caEndport,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel);  
	  if(strlen(caNetid)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caNetid,caTemp,63)); 
	  	strcpy(caNetid,caTemp_d);
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
	  	lDlip=0;
	  	lDuip=0;
	  	lDlport=atol(caStartport);
	  	lDuport=atol(caEndport);
	  	if(strlen(caStartip)>0){
	  		lDlip=ntohl(utComHostAddress(caStartip));
	  	}
	  	if(strlen(caEndip)>0){
	  		lDuip=ntohl(utComHostAddress(caEndip));
	  	}
	  	if(strlen(caSid)==0){
	  		sprintf(caTemp,"select count(*) from ncscaseip where cid=%lu and dlip=%lu and duip=%lu and dlport=%d and duport=%d  ",atol(caCid),lDlip,lDuip,lDlport,lDuport);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("update ncscaseip set flags=%lu,lasttime=%lu where cid=%lu and dlip=%lu and duip=%lu and dlport=%d and duport=%d  ",atol(caCid),lDlip,lDuip,lDlport,lDuport);
	  		}
	  		else{

	  			 pasDbExecSqlF("insert into ncscaseip(cid,slip,suip,dlip,duip,prot,dlport,duport,service,flags,lasttime) \
	       		values(%lu,0,0,%lu,%lu,0,%lu,%lu,0,%lu,%lu)",atol(caCid),lDlip,lDuip,lDlport,lDuport,atol(caStatus),time(0));
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncscaseip where cid=%lu and dlip=%lu and duip=%lu and dlport=%d and duport=%d  and sid!=%s ",atol(caCid),lDlip,lDuip,lDlport,lDuport,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("delete from ncscaseip where cid=%lu and dlip=%lu and duip=%lu and dlport=%d and duport=%d  ",atol(caCid),lDlip,lDuip,lDlport,lDuport);
	  			pasDbExecSqlF("update ncscaseip set flags=%lu,lasttime=%lu,dlip=%lu,duip=%lu,dlport=%lu,duport=%lu where sid=%s ",atol(caStatus),time(0),lDlip,lDuip,lDlport,lDuport,caSid);
	  		}
	  		else{
	  			printf("update ncscaseip set flags=%lu,lasttime=%lu,dlip=%lu,duip=%lu,dlport=%lu,duport=%lu where sid=%s \n",atol(caStatus),time(0),lDlip,lDuip,lDlport,lDuport,caSid);
	  				pasDbExecSqlF("update ncscaseip set flags=%lu,lasttime=%lu,dlip=%lu,duip=%lu,dlport=%lu,duport=%lu where sid=%s ",atol(caStatus),time(0),lDlip,lDuip,lDlport,lDuport,caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("update ncscaseip set flags=8,lasttime=%lu where sid in(%s)",time(0),caSelsid);
	  }
	  
	  
	  
	  
	  sprintf(caTemp,"select count(*) from ncscaseip where 1=1 ");
    if(strlen(caCid)>0)
    sprintf(caTemp+strlen(caTemp)," and cid=%s and flags!=8 ",caCid);
    
   
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select a.sid,inet_ntoa(a.dlip),inet_ntoa(a.duip),a.dlport,a.duport,a.flags from ncscaseip a   where 1=1 ");
    	if(strlen(caCid)>0)
    	sprintf(caTemp+strlen(caTemp)," and a.cid=%s and a.flags!=8 ",caCid);
    	
   
    	sprintf(caTemp+strlen(caTemp)," order by a.sid desc limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
   	 sid=0;
     memset(caDlip,0,sizeof(caDlip));
     memset(caDuip,0,sizeof(caDuip));
     lDlport=0;
     lDuport=0;
   	 flags=0;
   	 
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&sid,
      	                               UT_TYPE_STRING,15,caDlip,
      	                               UT_TYPE_STRING,15,caDuip,
      	                               UT_TYPE_LONG,4,&lDlport,
      	                               UT_TYPE_LONG,4,&lDuport,
      	                               UT_TYPE_LONG,4,&flags);
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);
        utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",flags);
        utPltPutLoopVar(psDbHead,"startip",iNum,caDlip);
        utPltPutLoopVar(psDbHead,"endip",iNum,caDuip);
        utPltPutLoopVarF(psDbHead,"startport",iNum,"%d",lDlport);
        utPltPutLoopVarF(psDbHead,"endport",iNum,"%d",lDuport);

				if(flags==0){
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"启用");
				}
				else{
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"停用");
				}
			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_ipport_list.htm");
	  return 0;
}


//显示手机通知组合框选择
int ncsMobileComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[32],caGroupid[16];
   	
   	char caName[32];
   	char caWhere[256];
   	char caSql[256];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	char caWaddr[64];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
   	long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caQuery[256],caTemp[1024];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	long lId;
   	char *p,*pVar;
   	long lNtype;
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    
    	iReturn = utMsgGetSomeNVar(psMsgHead,1,
					"query",      UT_TYPE_STRING, 127, caQuery);
					
					
	//				printf("caQuery=%s\n",pasCvtGBK(2,caQuery,caTemp,255));
    

   
  	 psCur = pasDbOpenSqlF("select waddr,name from ncsmovdictel  order by name ");

    if(psCur == NULL) {
    	  
        return 0;
    }
    psDbHead = utPltInitDb();
    
     
    memset(caName,0,sizeof(caName));
    memset(caWaddr,0,sizeof(caWaddr));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,63,caWaddr,
                                   UT_TYPE_STRING,31,caName);
                                  
                                  
    iNum = 0;
    
       
    
    while(iReturn == 0) {
        
        if(iNum>0){
    				utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
    	   }
    	
        iNum++;
    sprintf(caTemp,"%s(%s)",caWaddr,caName);
        
        utPltPutLoopVar(psDbHead,"classname", iNum,caTemp);
    sprintf(caTemp,"%s|%s",caWaddr,caName);     
        utPltPutLoopVar(psDbHead,"classid",   iNum,caTemp);
      
		    memset(caName,0,sizeof(caName));
		    memset(caWaddr,0,sizeof(caWaddr));
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,63,caWaddr,
		                                   UT_TYPE_STRING,31,caName);
    }
    pasDbCloseCursor(psCur); 
    utPltPutVarF(psDbHead,"TotRec","%d",iNum);  
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_nettype_list.htm");
      
    return 0;
}



//显示告警通知信息
int ncsCaseDet_warnset_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  long iNum;
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,15,
	                 "cid",            UT_TYPE_STRING, 10, caCid,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,4,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "nettype",UT_TYPE_STRING,10,caNtype,
			             "netid",UT_TYPE_STRING,30,caNetid,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel,
			             "name",UT_TYPE_STRING,31,caName);  
	  if(strlen(caNetid)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caNetid,caTemp,63)); 
	  	strcpy(caNetid,caTemp_d);
	  }
	  if(strlen(caKeyword)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,63));
	  	strcpy(caKeyword,caTemp_d);
	  }          
			              
	  if(strlen(caName)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caName,caTemp,63));
	  	strcpy(caName,caTemp_d);
	  } 
	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caUpdate,"update")==0){
	  	if(strlen(caSid)==0){
	  		sprintf(caTemp,"select count(*) from ncscasewarn where cid=%lu and wtype=%lu and waddr='%s' ",atol(caCid),atol(caNtype),caNetid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
	  		
	  			 pasDbExecSqlF("insert into ncscasewarn(cid,wtype,waddr,name) \
	       		values(%lu,%lu,'%s','%s')",atol(caCid),atol(caNtype),caNetid,caName);
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncscasewarn where cid=%lu and wtype=%lu and waddr='%s' and sid!=%s ",atol(caCid),atol(caNtype),caNetid,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("delete from ncscasewarn where cid=%lu and wtype=%lu and waddr='%s' ",atol(caCid),atol(caNtype),caNetid);
	  			pasDbExecSqlF("update ncscasewarn set wtype=%lu,waddr='%s',name='%s' where sid=%s ",atol(caNtype),caNetid,caName,caSid);
	  		}
	  		else{
	  					  			pasDbExecSqlF("update ncscasewarn set wtype=%lu,waddr='%s',name='%s' where sid=%s ",atol(caNtype),caNetid,caName,caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("delete from ncscasewarn where sid in(%s)",caSelsid);
	  	
	  }
	  
	  
	  
	  
	  sprintf(caTemp,"select count(*) from ncscasewarn where 1=1 ");
    if(strlen(caCid)>0)
    sprintf(caTemp+strlen(caTemp)," and cid=%s  ",caCid);
    
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select a.sid,a.wtype,a.waddr,a.name,0 from ncscasewarn a where 1=1 ");
    	if(strlen(caCid)>0)
    	sprintf(caTemp+strlen(caTemp)," and a.cid=%s and a.flags!=8 ",caCid);
    	

    	sprintf(caTemp+strlen(caTemp)," order by a.sid desc limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
   	 sid=0;
   	 memset(caName,0,sizeof(caName));
   	 memset(caNetname,0,sizeof(caNetname));
   	 nettype=0;
   	 flags=0;
   	 
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&sid,
      	                               UT_TYPE_LONG,4,&nettype,
      	                               UT_TYPE_STRING,63,caName,
      	                               UT_TYPE_STRING,31,caNetname,
      	                               UT_TYPE_LONG,4,&flags);
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);
        utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",flags);
        utPltPutLoopVar(psDbHead,"nettype",iNum,caNetname);
        utPltPutLoopVarF(psDbHead,"typeid",iNum,"%lu",nettype);
        utPltPutLoopVar(psDbHead,"netid",iNum,caName);
				if(nettype==2){
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"短信通知");
				}
				else{
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"邮件通知");
				}
			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_netid_list.htm");
	  return 0;
}



//显示布控区域信息
int ncsCaseArea_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  long iNum;
	  struct date_s {
 			unsigned long id;
 			char name[64];
 		} *pData;
	  long id,ctype,did,i,mCount,num;
	  char caObj[100][50];
	  char caSeid[1024];
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,12,
	                 "cid",            UT_TYPE_STRING, 10, caCid,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "seid",UT_TYPE_STRING,1000,caSeid,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel);  
	
	
	
	  iReturn=0; 
   //单位组
 sprintf(caTemp,"select count(*) from ncsgroup ");
 pData=NULL;
 pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&mCount);
 
 	pData=malloc(sizeof(struct date_s)*(mCount+2));
	if(pData==NULL)
	{
		utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","分配内存出错","分配内存出错");
        	return 0;
	}
 
  sprintf(caTemp,"select groupid,groupname from ncsgroup where 1=1 ");


   psCur = pasDbOpenSql(caTemp,0);
   memset(caName,0,sizeof(caName));
    iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG,4,&id,
                                     UT_TYPE_STRING,63,caName);
    iNum = 0;                                  
    while((iReturn==0)||(iReturn==1405))
    {
        pData[iNum].id=id;
	  	  strcpy(pData[iNum].name,caName);
        iNum++;       
        memset(caName,0,sizeof(caName));
       iReturn = pasDbFetchInto(psCur,
                                       UT_TYPE_LONG,4,&id,
                                       UT_TYPE_STRING,63,caName);
    }
    pasDbCloseCursor(psCur); 
	
	 
			              

	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caUpdate,"update")==0){
	  	char caCtype[16],caDid[16];
	  	if(strlen(caSeid)>0){
	  		 num=ncs_SepChar0(caSeid,',',caObj); 
	  		 for(i=0;i<num;i++){
	  		 	utStrGetSomeWordBy(caObj[i],"_",2,UT_TYPE_STRING,10,caDid,UT_TYPE_STRING,10,caCtype);
	  		 	 sprintf(caTemp,"select count(*) from ncscaseobj where cid=%d and ctype=%d and did=%d ",atol(caCid),atol(caCtype),atol(caDid));
	  		 	 lCount=0;
	  		 	 pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		 	 if(lCount>0){
	  		 	 	pasDbExecSqlF("update ncscaseobj set flags=0,lasttime=%lu where cid=%d and ctype=%d and did=%d ",time(0),atol(caCid),atol(caCtype),atol(caDid));
	  		 	}
	  		 	else{
	  		 		pasDbExecSqlF("insert into ncscaseobj (cid,ctype,did,flags,lasttime) values(%d,%d,%d,%d,%d)",atol(caCid),atol(caCtype),atol(caDid),0,time(0));
	  		 	}
	  		}
	  	}
	  	
	  	
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("delete from ncscaseobj where sid in(%s)",caSelsid);
	  }
	  
	  
	  
	  
	  sprintf(caTemp,"select count(*) from ncscaseobj where 1=1 ");
    if(strlen(caCid)>0)
    sprintf(caTemp+strlen(caTemp)," and cid=%s and flags!=8 ",caCid);
    
   
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select sid,ctype,did,flags from ncscaseobj where 1=1");
    	if(strlen(caCid)>0)
    	sprintf(caTemp+strlen(caTemp)," and cid=%s  ",caCid);
    	
   
    	sprintf(caTemp+strlen(caTemp)," order by sid desc limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
   	 sid=0;
     ctype=0;
     did=0;
   	 flags=0;
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&sid,
      	                               UT_TYPE_LONG,4,&ctype,
      	                               UT_TYPE_LONG,4,&did,
      	                               UT_TYPE_LONG,4,&flags);
      	                              
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);
        utPltPutLoopVarF(psDbHead,"status",iNum,"%lu",flags);
        if(ctype==0){
        	utPltPutLoopVar(psDbHead,"type_desc",iNum,"单位");
        }
        else if(ctype==1){
        	utPltPutLoopVar(psDbHead,"type_desc",iNum,"单位类别");
        }
        else if(ctype==2){
        	utPltPutLoopVar(psDbHead,"type_desc",iNum,"单位组");
        }

        utPltPutLoopVarF(psDbHead,"ctype",iNum,"%lu",ctype);
        if(did==0){
        	utPltPutLoopVar(psDbHead,"cname",iNum,"全部");
        }
        else{
        	if(ctype==0){
        		utPltPutLoopVar(psDbHead,"cname",iNum,ncsUtlGetClientNameById(psShmHead,did,"未知"));
        	}
        if(ctype==1){
        	sprintf(caTemp,"%d",did);
        		utPltPutLoopVar(psDbHead,"cname",iNum,ncsGetUserTnameByType(caTemp));
        	}
        	else if(ctype==2){
        			for(i=0;i<mCount;i++)
			        {
			       	  if(pData[i].id==did)
			        	{
			        		utPltPutLoopVar(psDbHead,"cname",iNum,pData[i].name);
			        		break;
			        	}
			        }
        	}
        }
				if(flags==0){
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"启用");
				}
				else{
					utPltPutLoopVar(psDbHead,"flag_desc",iNum,"停用");
				}
			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  if(pData) free(pData);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_obj_list.htm");
	  return 0;
}

typedef struct ncs_corp_s {
 		unsigned long did;
 		unsigned long lCount;
 	} ncscorp;
//布控日志
int ncsCase_xflog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[2048];
    int i,iNum;   
    utPltDbHead *psDbHead;
   	int iReturn;
   	char caOpt[256];
   	long lId,js_flag;
    unsigned long stime,did,uid,sip;
    char caSdate[24],caEdate[24];
    long lCtime;
    char caTtime[32];
    char caPage[16],caKeyword[72],caLimit[16],caSort[16],caDir[16],caTime_flag[16];
    char udisp[36],position[64],message[256],casename[32],caDispname[64];
 	  pasDbCursor *psCur;
 	  long lCurPg,lRowNum,lStartRec,lTime,lstime,letime,lCount;
 	  char caStemptime[24];
 	  char caCid[16];
 	  char caTemp_d[72];
 	  char caStat[18];
 	  char caCname[64];
 	  long tot,ytot,cktot,flags;
    float srat;
    int lSumUser,lCtype,lDid,j,m;
    ncsClient *psClient;
    long m_flag,n;
 	 	typedef struct ncs_watchobj_s {
 		unsigned long ctype;
 		unsigned long did;
 	} ncswatchobj;
 	struct ncs_watchobj_s *pObj;
 	struct ncs_corp_s *pCorp;
 	  
 		sprintf(caTemp,"select count(*) from ncscaseobj where cid=%lu ",atol(caCid));
	printf("caTemp=%s\n",caTemp);
	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	pObj=malloc((lCount+2)*sizeof(struct ncs_watchobj_s));
	if(pObj==NULL){
		    utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),"分配内存出错");
        return 0;
	}
	sprintf(caTemp,"select ctype,did from ncscaseobj where cid=%lu  group by ctype,did ",atol(caCid));	
printf("caTemp=%s\n",caTemp);
	psCur = pasDbOpenSql(caTemp,0); 
  if(psCur == NULL) {
    	  free(pObj);
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
      	                              
   	iReturn=0;
   		iNum=0;
   	while(iReturn == 0 ||iReturn==1405 ) {
   	lCtype=0;
   	lDid=0;
   
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&lCtype,
      	                               UT_TYPE_LONG,4,&lDid);     	                                    	                                                    	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
 				pObj[iNum].ctype=lCtype;
 				pObj[iNum].did=lDid;
        iNum++;
     }			
    pasDbCloseCursor(psCur);
	
	 j=0;
	  psClient = (ncsClient *)utShmArray(psShmHead,NCS_LNK_CLIENT);
    if(psClient) {      
        lSumUser = utShmArrayRecord(psShmHead,NCS_LNK_CLIENT);

        	pCorp=malloc((lSumUser+2)*sizeof(struct ncs_corp_s));
					if(pCorp==NULL){
						    free(pObj);
						    utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),"分配内存出错");
				        return 0;
					}
				
       
       for(m=0;m<iNum;m++){
        	if(j<lSumUser){	
        		if(pObj[m].ctype==0){
        		 if(pObj[m].did==0){
        		 	j=0;
        		 	for(i=0;i<lSumUser;i++){
        				pCorp[j].did=psClient[i].userid;
        				j++;
        			}
        				break;
        		 }
        		else {
        			  m_flag=0;
        				for(i=0;i<j;i++){
        					if(pCorp[i].did==pObj[m].did){
        						m_flag=1;
        						break;
        					}
        				}
        				if(m_flag==0){
        					pCorp[j].did=pObj[m].did;
        					j++;
        				}
        			
        			}
        		}
        		else if(pObj[m].ctype==2){
        			if(pObj[m].did==0){
        				j=0;
        				for(i=0;i<lSumUser;i++){      					
        					pCorp[j].did=psClient[i].userid;
        					j++;
        			  }
        				break;
        			}
        			else{
        				for(i=0;i<lSumUser;i++){
        			     if(pObj[m].did==psClient[i].groupid){
        			     	m_flag=0;
        			     	  for(n=0;n<j;n++){
        			     	  	if(pCorp[n].did==psClient[i].userid){
        			     	  		m_flag=1;
        			     	  		break;
        			     	  	}
        			     	  }
        			     	 if(m_flag==0){
        				      pCorp[j].did=psClient[i].userid;
        				      j++;
        				    }
        				  }
        			  }
        		  }
        	}
        		else if(pObj[m].ctype==1){
        				if(pObj[m].did==0){
        					j=0;
        					for(i=0;i<lSumUser;i++){
        					pCorp[j].did=psClient[i].userid;
        				
        					j++;
        				   }
        					break;
        				}
        				else {
        					for(i=0;i<lSumUser;i++){
        					   if(pObj[m].did==psClient[i].usertype){
        					   	m_flag=0;
        					   	for(n=0;n<j;n++){
        					   		if(pCorp[n].did==psClient[i].userid){
        					   			m_flag=1;
        					   			break;
        					   		}
        					   		if(m_flag==0){
        					   			pCorp[j].did=psClient[i].userid;
        					   			j++;
        					   		}
        					
        				     }
                    }
                 }
               }
            }
			  }
	    }
	}
  qsort(pCorp,j,sizeof(struct ncs_corp_s),ncsSortOnId);  
 	 tot=j;
   ytot=0;
   srat=0; 
 	  
 	  
 	  
 	  
 	  
 	  memset(caOpt,0,sizeof(caOpt)); 
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);

    utMsgPrintMsg(psMsgHead); 

	iReturn = utMsgGetSomeNVar(psMsgHead,7,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "stat",    UT_TYPE_STRING,8,caStat,
   	  "cid",          UT_TYPE_STRING,10,caCid);
 	 
      if(strlen(caKeyword)>0){
      	 strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
    	   strcpy(caKeyword,caTemp_d);
      }

    lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

    if(strlen(caLimit)>0){
    	lRowNum=atol(caLimit);
    }
    else{
    	lRowNum=40;
    }
    lStartRec = (lCurPg - 1) * lRowNum; 
       
    lTime=time(0);
    


    
    
    
    
    
 
   psDbHead = utPltInitDb();
   

  	sprintf(caTemp,"select count(*) from ncscasectllog,ncsuser where did=userid ");

	
  if(strlen(caKeyword)>0){
    sprintf(caTemp+strlen(caTemp)," and (cname like '%c%s%c' or dispname like '%c%s%c' )",'%',caKeyword,'%','%',caKeyword,'%');
  }
   if(strlen(caStat)>0){
    sprintf(caTemp+strlen(caTemp)," and flags=%s ",caStat);
  }
  
  if(strlen(caCid)>0){
  	sprintf(caTemp+strlen(caTemp)," and a.cid=%s ",caCid);
  }
  lCount=0;
//  printf("caKeyword=%s\n",caKeyword);

  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
   
  utPltPutVarF(psDbHead,"TotRec", "%lu", lCount);
   

 sprintf(caTemp,"select did,stime,cname,flags,dispname from ncscasectllog,ncsuser where did=userid and cid=%lu ",atol(caCid));
  if(strlen(caKeyword)>0){
    sprintf(caTemp+strlen(caTemp)," and (cname like '%c%s%c' or dispname like '%c%s%c' )",'%',caKeyword,'%','%',caKeyword,'%');
  }
   if(strlen(caStat)>0){
    sprintf(caTemp+strlen(caTemp)," and flags=%s ",caStat);
  }
  
  
	sprintf(caTemp+strlen(caTemp),"  limit %d,%d ",lStartRec,lRowNum);

printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
    	
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	ytot=0;
   	cktot=0;
   	iNum=1;
   	while(iReturn == 0 ||iReturn==1405 ) {
   	
     stime=0;
     memset(caCname,0,sizeof(caCname));
     lDid=0;
     flags=0;
     memset(caDispname,0,sizeof(caDispname));
   	 
   	 iReturn = pasDbFetchInto(psCur,   UT_TYPE_LONG,4,&lDid,
      	                               UT_TYPE_LONG,4,&stime,
      	                               UT_TYPE_STRING,63,caCname,
      	                               UT_TYPE_LONG,4,&flags,
      	                               UT_TYPE_STRING,63,caDispname);
      	                               
                                                        	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;

        utPltPutLoopVar(psDbHead,"dh",iNum,",");
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);
        utPltPutLoopVar(psDbHead,"cname",iNum,caCname);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
        utPltPutLoopVar(psDbHead,"sdate",iNum,utTimFormat("%y/%m/%d %H:%M",stime));
        if(flags==1)
        utPltPutLoopVar(psDbHead,"flags_desc",iNum,"成功");
        else if(flags==0)
        utPltPutLoopVar(psDbHead,"flags_desc",iNum,"未布控");
        else if(flags==9)
        utPltPutLoopVar(psDbHead,"flags_desc",iNum,"撤控");
        
        
        if(findCorpIde(pCorp,tot,lDid)!=-1){
        	if(flags==1) ytot++;
        	if(flags==9) cktot++;
        }

     }				
    pasDbCloseCursor(psCur);
    
   if(ytot>0){
   		srat=100*((float)ytot/(float)tot);
   }
   	else 
   		srat=0; 
   
ytot=1;
tot=1;
srat=100; 
 utPltPutLoopVarF(psDbHead,"sdate",1,"<font color=red>已布控单位:%d</font>",ytot);
 utPltPutLoopVarF(psDbHead,"cname",1,"<font color=red>应布控单位：%d</font>",tot);
 utPltPutLoopVarF(psDbHead,"dispname",1,"<font color=red>已撤控单位:%d</font>",cktot);
 utPltPutLoopVarF(psDbHead,"flags_desc",1,"<font color=red>布控成功率:%0.2f</font>",srat);

    
    
if(pCorp) free(pCorp);
if(pObj) free(pObj);
//  utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_xfstore.htm");
    return 0;
}


//查询布控策略

	struct ncsallcaserule_s{
 		int sid;
 		int casetype;   //1-聊天内容布控 2-敏感服务布控 3-IP地址布控 4-邮件内容布控 5-虚拟身份布控 6- 表单布控 7-使用者特征 8-网址布控
 		int cid;     //0=案件id 
 		char cname[36]; //案件名称
 		char casecode[24];//案件编号
 		char content[128];  //内容
 	};
 static long lOrder,lBy;
 
static int ncsUtlSortCaseRule(struct ncsallcaserule_s *p1,struct ncsallcaserule_s *p2)
{
	long iRet;
        if(lOrder==1){
        	if(lBy==1){
        		iRet=strcmp(p1->casecode,p2->casecode);
        	}
        	else{
        		iRet=strcmp(p2->casecode,p1->casecode);
        	}
        }
        else if(lOrder==2){
        	if(lBy==1){
        		iRet=strcmp(p1->cname,p2->cname);
        	}
        	else{
        		iRet=strcmp(p2->cname,p1->cname);
        	}
        }
        else {
        	if(lBy==1){
        		iRet=p1->casetype-p2->casetype;
        	}
        	else{
        		iRet=p2->casetype-p1->casetype;
        	}
        }
        
        
       return iRet;

}
 
//查询布控信息
int ncsCaseDet_search_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  long iNum;
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	 	struct ncsallcaserule_s *pRule;
	  long iMax;
	  long lSum,lSid,lCtype,lCid,prot,dlport,duport,service,urlid,contid,i,m;
	  char caCname[36],caSlip[20],caDuip[20];
	  char caDispname[36],caSname[64];
	  char caUrl[64];
	  char caMfrom[256],caMto[256],caSubj[256];
	  char caCasecode[24];
    char caMesg[256],caSuip[20],caDlip[20],caCont[256];
    char caSort[16];
	  utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,7,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,			      
			             "start",UT_TYPE_STRING,10,caStart,
			             "sort", UT_TYPE_STRING,10,caSort,
			             "dir",UT_TYPE_STRING,10,caDir);
		if(strcmp(caSort,"casecode")==0){
			lOrder=1;
		}
		else if(strcmp(caSort,"cname")==0){
			lOrder=2;
		}
		else if(strcmp(caSort,"type_desc")==0){
			lOrder=3;
		}
		else {
			lOrder=0;
		}
		if(strcmp(caDir,"ASC")==0){
			lBy=1;
		}
		else{
			lBy=2;
		}       

	  if(strlen(caKeyword)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,63));
	  	strcpy(caKeyword,caTemp_d);
	  }          
			              

	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  
	  
   iMax=60000;
	  
	lSum=0;
 	
 	pRule=(struct ncsallcaserule_s*)malloc(iMax*sizeof(struct ncsallcaserule_s));
 	if(pRule==NULL){
 		 utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
 		 return 0;
 	}
 	
 	 	sprintf(caTemp,"select a.sid,a.ctype,a.cvalue,a.cid,b.casename,b.casecode from ncscaseuser a,ncscaseinfo b where a.cid=b.sid and a.flags!=8 ");
 
        	
  //  	printf("caTemp=%s\n",caTemp);
    	
   psCur = pasDbOpenSql(caTemp,0);
  
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	while((iReturn == 0 ||iReturn==1405 )&&lSum<iMax) {
   	
    lSid=0;
    lCtype=0;
    memset(caDispname,0,sizeof(caDispname));
    lCid=0;
    memset(caCname,0,sizeof(caCname));
    memset(caCasecode,0,sizeof(caCasecode));
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&lCtype,
      	                               UT_TYPE_STRING,32,caDispname,
      	                               UT_TYPE_LONG,4,&lCid,
      	                               UT_TYPE_STRING,32,caCname,
      	                               UT_TYPE_STRING,20,caCasecode);
      	                              
     if((iReturn!=0)&&(iReturn!=1405)) break;	 
       
         memset(caMesg,0,sizeof(caMesg));

				
				if(lCtype==1){
				sprintf(caMesg+strlen(caMesg),"登录名:[%s] ",caDispname);				
			  }
			  else if(lCtype==2){
				sprintf(caMesg+strlen(caMesg),"显示名:[%s] ",caDispname);				
			  }
			  else if(lCtype==3){
				sprintf(caMesg+strlen(caMesg),"计算机名:[%s] ",caDispname);				
			  }
			  else if(lCtype==4){
				sprintf(caMesg+strlen(caMesg),"MAC地址:[%s] ",caDispname);				
			  }
			  else if(lCtype==5){
				sprintf(caMesg+strlen(caMesg),"证件号:[%s] ",caDispname);				
			  }
			
			  pRule[lSum].sid=lSid;
        pRule[lSum].casetype=7;
        pRule[lSum].cid=lCid;
        strcpy(pRule[lSum].cname,caCname);
			  strcpy(pRule[lSum].content,caMesg);
			  strcpy(pRule[lSum].casecode,caCasecode);
			  lSum++;
			
			

				
    }
    pasDbCloseCursor(psCur);
 	
 	
 	 	//统计虚拟身份布控
 	  sprintf(caTemp,"select a.sid,a.nettype,a.name,b.name,a.cid,c.casename,c.casecode from (ncscasenetid a,ncscaseinfo c)  left join ncsnettype b on (a.nettype=b.ntype) where a.cid=c.sid and a.flags!=8  ");
    	
//  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	while((iReturn == 0 ||iReturn==1405 )&&lSum<iMax) {
   	
   	 lSid=0;
   	 memset(caName,0,sizeof(caName));
   	 memset(caNetname,0,sizeof(caNetname));
   	 nettype=0;
   	 lCid=0;
   	 memset(caCname,0,sizeof(caCname));
   	 memset(caCasecode,0,sizeof(caCasecode));
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&nettype,
      	                               UT_TYPE_STRING,63,caName,
      	                               UT_TYPE_STRING,32,caNetname,
      	                               UT_TYPE_LONG,4,&lCid,
      	                               UT_TYPE_STRING,32,caCname,
      	                               UT_TYPE_STRING,20,caCasecode);
      	                              
      	if((iReturn!=0)&&(iReturn!=1405)) break;	                                                             
      	                                    
        memset(caMesg,0,sizeof(caMesg));
             
				if(strlen(caNetname)>0){
				sprintf(caMesg+strlen(caMesg),"服务类别:[%s] ",caNetname);
			  }
				if(strlen(caName)>0){
					sprintf(caMesg+strlen(caMesg),"帐号:[%s] ",caName);				
			  }
		
		
				pRule[lSum].sid=lSid;
        pRule[lSum].casetype=5;
        pRule[lSum].cid=lCid;
        strcpy(pRule[lSum].cname,caCname);
			  strcpy(pRule[lSum].content,caMesg);
			  strcpy(pRule[lSum].casecode,caCasecode);
			  lSum++;
						
    }
    pasDbCloseCursor(psCur);
    
     //统计IP布控	
 	
 	 sprintf(caTemp,"select a.sid,inet_ntoa(a.slip),inet_ntoa(a.suip),inet_ntoa(a.dlip),inet_ntoa(a.duip),a.prot,a.dlport,a.duport,a.service,b.name,a.cid,c.casename,c.casecode from (ncscaseip a,ncscaseinfo c) left join ncipindex b on (a.service=b.id) where a.cid=c.sid ");
    
 	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	while((iReturn == 0 ||iReturn==1405 )&&lSum<iMax) {
   	
   	 lSid=0;
   	 memset(caSlip,0,sizeof(caSlip));
   	 memset(caSuip,0,sizeof(caSuip));
   	 memset(caDlip,0,sizeof(caDlip));
   	 memset(caDuip,0,sizeof(caDuip));
   	 prot=0;
   	 dlport=0;
   	 duport=0;
   	 service=0;
   	 lCid=0;
   	 memset(caCname,0,sizeof(caCname));
   	 memset(caName,0,sizeof(caName));
   	 memset(caCasecode,0,sizeof(caCasecode));
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_STRING,18,caSlip,
      	                               UT_TYPE_STRING,18,caSuip,
      	                               UT_TYPE_STRING,18,caDlip,
      	                               UT_TYPE_STRING,18,caDuip,
      	                               UT_TYPE_LONG,4,&prot,
      	                               UT_TYPE_LONG,4,&dlport,
      	                               UT_TYPE_LONG,4,&duport,
      	                               UT_TYPE_LONG,4,&service,
      	                               UT_TYPE_STRING,63,caName,
      	                               UT_TYPE_LONG,4,&lCid,
      	                               UT_TYPE_STRING,32,caCname,
      	                               UT_TYPE_STRING,20,caCasecode);
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
        memset(caMesg,0,sizeof(caMesg));
      
				
				if((strlen(caSlip)>0)||(strlen(caSuip)>0)){
				sprintf(caMesg+strlen(caMesg),"源IP:[%s--%s] ",caSlip,caSuip);
			  }
			  if((strlen(caDlip)>0)||(strlen(caDuip)>0)){
				sprintf(caMesg+strlen(caMesg),"目标IP:[%s--%s] ",caDlip,caDuip);
			  }
			  if(dlport>0||duport>0)
			  sprintf(caMesg+strlen(caMesg),"访问端口:[%d--%d] ",dlport,duport);
			  
				if(strlen(caName)>0){
					sprintf(caMesg+strlen(caMesg),"服务:[%s] ",caName);				
			  }
		    if(prot==6){
		    	sprintf(caMesg+strlen(caMesg),"协议:[%s] ","TCP");
		    }
		    else if(prot==17){
		    	sprintf(caMesg+strlen(caMesg),"协议:[%%s] ","UDP");
		    }
		    	
		 		pRule[lSum].sid=lSid;
        pRule[lSum].casetype=3;
        pRule[lSum].cid=lCid;
        strcpy(pRule[lSum].cname,caCname);
			  strcpy(pRule[lSum].content,caMesg);
			 	strcpy(pRule[lSum].casecode,caCasecode);
			  lSum++;
				
    }
    pasDbCloseCursor(psCur);
    
       //post表单	
 		sprintf(caTemp,"select a.sid,a.url,a.urlid,a.cont,a.contid,b.name,c.name,a.cid,d.casename,d.casecode from (ncscasepost a,ncscaseinfo d) left join ncwebclass b on (a.urlid=b.id) left join ncskeyindex c on (a.contid=c.id) where a.cid=d.sid and a.flags!=8");
 	
   
 	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	while((iReturn == 0 ||iReturn==1405 )&&lSum<iMax) {
   	
   	 lSid=0;
     memset(caUrl,0,sizeof(caUrl));
     urlid=0;
   	 lCid=0;
   	 memset(caCname,0,sizeof(caCname));
   	 memset(caName,0,sizeof(caName));
   	 memset(caSname,0,sizeof(caSname));
   	 contid=0;
   	 memset(caCont,0,sizeof(caCont));
   	 memset(caCasecode,0,sizeof(caCasecode));   	 
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_STRING,127,caUrl,
      	                               UT_TYPE_LONG,4,&urlid,
      	                               UT_TYPE_STRING,127,caCont,
      	                               UT_TYPE_LONG,4,&contid,
      	                               UT_TYPE_STRING,32,caName,
      	                               UT_TYPE_STRING,32,caSname,
      	                               UT_TYPE_LONG,4,&lCid,
      	                               UT_TYPE_STRING,32,caCname,
      	                               UT_TYPE_STRING,20,caCasecode);
      	                               
      	 if((iReturn!=0)&&(iReturn!=1405)) break;	                              
   
        iNum++;
        memset(caMesg,0,sizeof(caMesg));
      
				
				if(strlen(caUrl)>0){
				sprintf(caMesg+strlen(caMesg),"访问网址:[%s] ",caUrl);
			  }
				if(strlen(caName)>0){
					sprintf(caMesg+strlen(caMesg),"网址类别:[%s] ",caName);				
			  }
			 if(strlen(caCont)>0){
					sprintf(caMesg+strlen(caMesg),"表单内容:[%s] ",caCont);				
			  }
			  if(strlen(caSname)>0){
			  	sprintf(caMesg+strlen(caMesg),"敏感类别:[%s] ",caSname);
			  }
		    	
		 		pRule[lSum].sid=lSid;
        pRule[lSum].casetype=6;
        pRule[lSum].cid=lCid;
        strcpy(pRule[lSum].cname,caCname);
			  strcpy(pRule[lSum].content,caMesg);
			  strcpy(pRule[lSum].casecode,caCasecode);
			  lSum++;
				
    }
    pasDbCloseCursor(psCur);
    
      //Web布控	
 	
 	sprintf(caTemp,"select a.sid,a.url,a.urlid,b.name,a.cid,c.casename,c.casecode from (ncscaseweb a,ncscaseinfo c) left join ncwebclass b on (a.urlid=b.id) where a.cid=c.sid and a.flags!=8 ");
 	
 	
   
 	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	while((iReturn == 0 ||iReturn==1405 )&&lSum<iMax) {
   	
   	 lSid=0;
     memset(caUrl,0,sizeof(caUrl));
     urlid=0;
   	 lCid=0;
   	 memset(caCname,0,sizeof(caCname));
   	 memset(caName,0,sizeof(caName));
   	 memset(caCasecode,0,sizeof(caCasecode));
  	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_STRING,127,caUrl,
      	                               UT_TYPE_LONG,4,&urlid,
      	                               UT_TYPE_STRING,32,caName,
      	                               UT_TYPE_LONG,4,&lCid,
      	                               UT_TYPE_STRING,32,caCname,
      	                               UT_TYPE_STRING,20,caCasecode);
 if((iReturn!=0)&&(iReturn!=1405)) break;	
      
        memset(caMesg,0,sizeof(caMesg));
      
				
			  if(strlen(caUrl)>0){
				sprintf(caMesg+strlen(caMesg),"访问网址:[%s] ",caUrl);
			  }
				if(strlen(caName)>0){
					sprintf(caMesg+strlen(caMesg),"网址类别:[%s] ",caName);				
			  }
		    	
		 		pRule[lSum].sid=lSid;
        pRule[lSum].casetype=8;
        pRule[lSum].cid=lCid;
        strcpy(pRule[lSum].cname,caCname);
			  strcpy(pRule[lSum].content,caMesg);
			  strcpy(pRule[lSum].casecode,caCasecode);
			  lSum++;
				
    }
    pasDbCloseCursor(psCur);	
    
     	    //聊天内容 
  
  	  sprintf(caTemp,"select a.sid,a.nettype,a.cont,a.contid,c.name,a.cid,b.casename,b.casecode from (ncscaseim a,ncscaseinfo b) left join ncskeyindex c on (a.contid=c.id) where a.cid=b.sid and a.flags!=8 ");
    	
 // 	printf("caTemp=%s\n",caTemp);
    	
    psCur = pasDbOpenSql(caTemp,0);  
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	while((iReturn == 0 ||iReturn==1405 )&&lSum<iMax) { 	
   	 lSid=0;	
   	 memset(caSname,0,sizeof(caSname));
   	 memset(caCont,0,sizeof(caCont));
   	 nettype=0;
   	 contid=0;
     lCid=0;
     memset(caCname,0,sizeof(caCname));
   	 memset(caCasecode,0,sizeof(caCasecode));
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&nettype,
      	                               UT_TYPE_STRING,127,caCont,
      	                               UT_TYPE_LONG,4,&contid,
      	                               UT_TYPE_STRING,32,caSname,
      	                               UT_TYPE_LONG,4,&lCid,
      	                               UT_TYPE_STRING,32,caCname,
      	                               UT_TYPE_STRING,20,caCasecode);
      	                                  	                                                             	                              
      if((iReturn!=0)&&(iReturn!=1405)) break;		                              
   
        iNum++;
        memset(caMesg,0,sizeof(caMesg));
       
				
				if(nettype==7)
				  strcpy(caName,"雅虎通");
				 else if(nettype==10)
				  strcpy(caName,"MSN");
				 else if(nettype==11)
				  strcpy(caName,"ICQ");
				 else
				  strcpy(caName,"");
				  
				if(strlen(caName)>0){
				sprintf(caMesg+strlen(caMesg),"类别:[%s] ",caName);
			  }
				if(strlen(caCont)>0){
					sprintf(caMesg+strlen(caMesg),"内容:[%s] ",caCont);				
			  }		
			  if(strlen(caSname)>0){
			  	sprintf(caMesg+strlen(caMesg),"敏感类别:[%s] ",caSname);
			  }
			   
				pRule[lSum].sid=lSid;
        pRule[lSum].casetype=1;
        pRule[lSum].cid=lCid;
        strcpy(pRule[lSum].cname,caCname);
			  strcpy(pRule[lSum].content,caMesg);
			  strcpy(pRule[lSum].casecode,caCasecode);
			  lSum++;
		
			
    }
    pasDbCloseCursor(psCur);
    
      //邮件内容 
 
  	  sprintf(caTemp,"select a.sid,a.mfrom,a.mto,a.subj,a.cont,a.contid,a.ctype,c.name,a.cid,b.casename,b.casecode from (ncscasemail a,ncscaseinfo b) left join ncskeyindex c on (a.contid=c.id) where a.cid=b.sid and a.flags!=8 ");
    
//  	printf("caTemp=%s\n",caTemp);
    	
    psCur = pasDbOpenSql(caTemp,0);  
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	while((iReturn == 0 ||iReturn==1405 )&&lSum<iMax) { 	
   	
   	 lSid=0;	
   	 memset(caSname,0,sizeof(caSname));
   	 memset(caCont,0,sizeof(caCont));
   	 memset(caMfrom,0,sizeof(caMfrom));
   	 memset(caMto,0,sizeof(caMto));
   	 memset(caSubj,0,sizeof(caSubj));
   	 contid=0;
   	 lCid=0;
   	 memset(caCname,0,sizeof(caCname));
   	 memset(caCasecode,0,sizeof(caCasecode));
   	 lCtype=0;
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_STRING,63,caMfrom,
      	                               UT_TYPE_STRING,63,caMto,
      	                               UT_TYPE_STRING,127,caSubj,
      	                               UT_TYPE_STRING,127,caCont,
      	                               UT_TYPE_LONG,4,&contid,
      	                               UT_TYPE_LONG,4,&lCtype,
      	                               UT_TYPE_STRING,32,caSname,
      	                               UT_TYPE_LONG,4,&lCid,
      	                               UT_TYPE_STRING,32,caCname,
      	                               UT_TYPE_STRING,20,caCasecode);
      	                                  	                                                             	                              
      	if((iReturn!=0)&&(iReturn!=1405)) break;	                              
   
        iNum++;
        memset(caMesg,0,sizeof(caMesg));
       
				
				if(strlen(caMfrom)>0){
					sprintf(caMesg+strlen(caMesg),"发件人:[%s] ",caMfrom);
				}
				if(strlen(caMto)>0){
					sprintf(caMesg+strlen(caMesg),"收件人:[%s] ",caMto);
				}
				if(strlen(caSubj)>0){
					sprintf(caMesg+strlen(caMesg),"主题包含:[%s] ",caSubj);
				}
				
				if(strlen(caCont)>0){
					sprintf(caMesg+strlen(caMesg),"内容包含:[%s] ",caCont);				
			  }		
			  if(strlen(caSname)>0){
			  	sprintf(caMesg+strlen(caMesg),"敏感类别:[%s] ",caSname);
			  }
			  if(lCtype==1){
			  	sprintf(caMesg+strlen(caMesg),"收发类型:[发送] ");
			  }
		    if(lCtype==2){
			  	sprintf(caMesg+strlen(caMesg),"收发类型:[收件] ");	
			  }
				pRule[lSum].sid=lSid;
        pRule[lSum].casetype=4;
        pRule[lSum].cid=lCid;
        strcpy(pRule[lSum].cname,caCname);
			  strcpy(pRule[lSum].content,caMesg);
			  strcpy(pRule[lSum].casecode,caCasecode);
			  lSum++;
    }
    pasDbCloseCursor(psCur);
    
     //敏感服务布控
    sprintf(caTemp,"select a.sid,a.nettype,b.name,a.cid,c.casename,c.casecode from (ncscaseservice a,ncscaseinfo c) left join ncssenservice b on (a.nettype=b.ntype) where a.cid=c.sid and a.flags!=8 ");
    
//  	printf("caTemp=%s\n",caTemp);
    	
    psCur = pasDbOpenSql(caTemp,0);  
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
   	 lSid=0;	
   	 memset(caSname,0,sizeof(caSname));
   	 lCtype=0;
   	 lCid=0;
   	 memset(caCname,0,sizeof(caCname));
   	 memset(caCasecode,0,sizeof(caCasecode));
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&lCtype,
      	                               UT_TYPE_STRING,32,caSname,
      	                               UT_TYPE_LONG,4,&lCid,
      	                               UT_TYPE_STRING,32,caCname,
      	                               UT_TYPE_STRING,20,caCasecode);
      	                                  	                                                             	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
        memset(caMesg,0,sizeof(caMesg));
       
			
			
			  if(strlen(caSname)>0){
			  	sprintf(caMesg+strlen(caMesg),"敏感服务类别:[%s] ",caSname);
			  }
			 	pRule[lSum].sid=lSid;
        pRule[lSum].casetype=2;
        pRule[lSum].cid=lCid;
        strcpy(pRule[lSum].cname,caCname);
			  strcpy(pRule[lSum].content,caMesg);
			  strcpy(pRule[lSum].casecode,caCasecode);
			  lSum++;
				
    }
    pasDbCloseCursor(psCur);
    
    qsort(pRule, lSum,sizeof(struct ncsallcaserule_s),ncsUtlSortCaseRule);
    
    psDbHead = utPltInitDb(); 
    iNum=0;
    m=0;
    for(i=0;i<lSum;i++){
	
	if(strstr(pRule[i].content,caKeyword)||strstr(pRule[i].cname,caKeyword)||strlen(caKeyword)==0||strstr(pRule[i].casecode,caKeyword)){
		if(m>=lStartRec&&m<lStartRec+lRowNum)
		{
	    m++;
	    
	     if(iNum>0){
       	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
       }
	    
			utPltPutLoopVarF(psDbHead,"sid",iNum+1,"%d",pRule[i].sid);
			utPltPutLoopVarF(psDbHead,"casetype",iNum+1,"%d",pRule[i].casetype);

      if(pRule[i].casetype==1)
       utPltPutLoopVar(psDbHead,"type_desc",iNum+1,"聊天内容");
      else if(pRule[i].casetype==2)
       utPltPutLoopVar(psDbHead,"type_desc",iNum+1,"敏感服务");
      else if(pRule[i].casetype==3)
       utPltPutLoopVar(psDbHead,"type_desc",iNum+1,"IP地址");
      else if(pRule[i].casetype==4)
       utPltPutLoopVar(psDbHead,"type_desc",iNum+1,"邮件内容");
      else if(pRule[i].casetype==5)
       utPltPutLoopVar(psDbHead,"type_desc",iNum+1,"虚拟身份");
      else if(pRule[i].casetype==6)
       utPltPutLoopVar(psDbHead,"type_desc",iNum+1,"表单布控");
      else if(pRule[i].casetype==7)
       utPltPutLoopVar(psDbHead,"type_desc",iNum+1,"使用者特征");
      else if(pRule[i].casetype==8)
       utPltPutLoopVar(psDbHead,"type_desc",iNum+1,"网址布控");


			utPltPutLoopVar(psDbHead,"cname",iNum+1,pRule[i].cname);
			utPltPutLoopVar(psDbHead,"casecode",iNum+1,pRule[i].casecode);
			utPltPutLoopVar(psDbHead,"content",iNum+1,pRule[i].content);
			
			utPltPutLoopVarF(psDbHead,"num",iNum+1,"%d",iNum);
	    iNum++;
	    }
	    else
	      m++;
   }
}
lCount=m;
 	
 	
 	
	  
	  
	  if(pRule) free(pRule);
	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
//	  utPltShowDb(psDbHead);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_detall_list.htm");
	  return 0;
}



//关键字组树
int ncsTreeKeyword_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[1024];
   	char sqlbuf[1024];
   	long lFlag,lKid;
   	int  iNum;
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
    long lId,lSid;
    char caPlate[128];
    char caPid[16];
    char caPid1[16];
    long lPid;
    long lSumRec=0;
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,9,
			             "del",           UT_TYPE_STRING,8,caDel,
			             "keyname",       UT_TYPE_STRING,30,caKeyname,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "del",UT_TYPE_STRING,10,caDel,
			             "plate",UT_TYPE_STRING,100,caPlate,
			             "pid",  UT_TYPE_STRING,10,caPid,
			             "node",UT_TYPE_STRING,10,caPid1);
			             
    	pHash=(unsigned char *)pasLHashInit(1000,1000,sizeof(struct ncsKeywordStat_s),0,4); 
    	if(pHash==NULL){
    		return -1;
    	}
    if(strlen(caPlate)>0){

    	sprintf(sqlbuf,"select count(*),flag,kid from ncskeywordlog group by kid,flag ");

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
    else{
    	
        sprintf(sqlbuf,"select count(*),tid from ncskeycont,ncskeyindex where tid=id and pid=%d group by tid ",atol(caPid1));
 //   	sprintf(sqlbuf,"select pid from ncskeyindex limit 0,999 ");

    	psCur=pasDbOpenSql(sqlbuf,0);
    	if(psCur==NULL){
    		free(pHash);
    		return 0;
    	}

    	  lKid=0;
    	  lCount=0;
    	  iReturn=pasDbFetchInto(psCur,
    	                       UT_TYPE_LONG,4,&lCount,
    	                       UT_TYPE_LONG,4,&lKid);

                       
			     while(iReturn==0||iReturn==1405)
			     {
			
			     	  psData = (struct ncsKeywordStat_s *)pasLHashLookA(pHash,&lKid);  
			     	 	if(psData){
			     	 		psData->sum=psData->sum+lCount;
			     	 		lSumRec=lSumRec+lCount;
			     	 		}     	  
			    	  lKid=0;
			    	  lCount=0;
			    	  iReturn=pasDbFetchInto(psCur,
			    	                       UT_TYPE_LONG,4,&lCount,
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
	  		sprintf(caTemp,"select count(*) from ncskeyindex where name='%s' ",caKeyname);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
	  		   lSid = ncsUtlGenId(psShmHead);
	  			 pasDbExecSqlF("insert into ncskeyindex(id,name,lasttime,flags,pid) values (%lu,'%s',%lu,8,%d)",
                                             lSid,caKeyname,time(0),atol(caPid));
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncskeyindex where name='%s' and id!=%s ",caKeyname,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
	  					  			pasDbExecSqlF("update ncskeyindex set name='%s',lasttime=%d,pid=%d where id=%s ",caKeyname,time(0),atol(caPid),caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	   	       iReturn=pasDbExecSqlF("delete from ncskeyindex where id in (%s)",caSelsid);
   	             if(iReturn==0){
   	             	pasDbExecSqlF("update ncskeyindex set pid=0 where pid in (%s) ",caSelsid);
                iReturn = pasDbExecSqlF("delete from ncskeycont where tid  in(%s)",caSelsid);
              }

	  	
	  }
 
 
 
 
 
    iNum = 0;
    
     psDbHead = utPltInitDb();

    


		  	 psCur = pasDbOpenSqlF("select name,id,pid from ncskeyindex where pid=%d order by name limit 0,1000",atol(caPid1));
		  	 
 //    psCur = pasDbOpenSqlF("select name,id,pid from ncskeyindex  order by name limit 0,1000");
		  	 
		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
	     memset(caName,0,sizeof(caName));
	     lId=0;
	     lPid=0;
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_LONG,4,&lId,
		                                   UT_TYPE_LONG,4,&lPid);
		                                
		
		 
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  
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
		          	
		          		psData = (struct ncsKeywordStat_s *)pasLHashLook(pHash,&lId);  
					     	  if(psData){
					     	  	 utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s_(%d)",caName,psData->sum);
							    }
							    else{
							    	utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s_(%d)",caName,0);
							    }
		          	
		             
		          }
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"pid",iNum,"%ld",lPid);
		         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%ld",lId);
		         
		           psData = (struct stData *)pasLHashLook(pHash,&lId);       
		         
  //       	if(psData){
  //       		printf("id=%d,lcount=%d\n",psData->id,psData->count);
 //         	utPltPutLoopVar(psDbHead,"leaf", iNum,"false");
 //        		utPltPutLoopVar(psDbHead,"cls", iNum,"folder");
//         	}
 //         else{
          	utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
         		utPltPutLoopVar(psDbHead,"cls", iNum,"file");
          	

 //         }
		         	
	    		 memset(caName,0,sizeof(caName));
	     		 lId=0;
			     lPid=0;
				   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
				                                   UT_TYPE_LONG,4,&lId,
				                                   UT_TYPE_LONG,4,&lPid);
		    }
		    pasDbCloseCursor(psCur);
		
    
    	
    	free(pHash);
// utPltShowDb(psDbHead);
      if(strlen(caPlate)>0){
      	
      	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
      	
      }
      else{
      	utPltPutVarF(psDbHead,"sum","%d",lSumRec);
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_keyword_checktree.htm");
    	}
    

    return 0;
}


//有害关键字组树
int ncsTreeKeyword_yh_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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

    


		  	 psCur = pasDbOpenSqlF("select name,id,count(*) from ncskeywordlog,ncskeyindex where kid=id and flag=1 group by id order by name limit 0,1000");
		  	 

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




//显示关键字明细
int ncsCase_keywordDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
	   char caPid[16];
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  memset(caPre,0,sizeof(caPre));
	  iReturn = utMsgGetSomeNVar(psMsgHead,17,
	                 "kid",            UT_TYPE_STRING, 10, caKid,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,4,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "ktype",UT_TYPE_STRING,6,caKtype,
			             "tid",UT_TYPE_STRING,10,caTid,
			             "caseflags",UT_TYPE_STRING,6,caCaseflags,
			             "content",UT_TYPE_STRING,31,caContent,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel,
			             "pid",UT_TYPE_STRING,10,caPid);  
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
	  		sprintf(caTemp,"select count(*) from ncskeycont where tid=%lu  and content=Binary('%s') ",atol(caTid),caContent);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			
	  			pasDbExecSqlF("update ncskeycont set ktype=%lu,caseflags=%lu where tid=%lu and content='%s' ",atol(caKtype),atol(caCaseflags),atol(caTid),caContent);
	  		}
	  		else{
	  			 pasDbExecSqlF("insert into ncskeycont(tid,content,ktype,caseflags) \
	       		values(%lu,'%s',%lu,%lu)",atol(caTid),caContent,atol(caKtype),atol(caCaseflags));
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncskeycont where tid=%lu and content='%s' and sid!=%s ",atol(caTid),caContent,caSid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount==0){
 		
	  				pasDbExecSqlF("update ncskeycont set ktype=%lu,caseflags=%lu,content='%s' where sid=%s ",atol(caKtype),atol(caCaseflags),caContent,caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("delete from  ncskeycont  where sid in(%s)",caSelsid);
	  }
	  
	  
	  
	  
	  sprintf(caTemp,"select count(*) from ncskeycont where 1=1 ");
    if(strlen(caKid)>0)
      sprintf(caTemp+strlen(caTemp)," and (tid=%s ) ",caKid);
    
    if(strlen(caPid)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid in (select id from ncskeyindex where pid=400) ");
    }
    else{
    	sprintf(caTemp+strlen(caTemp)," and tid in (select id from ncskeyindex where pid!=400) ");
    }
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and content like '%c%s%c' ",'%',caKeyword,'%');
    }
    printf("caTemp=%s\n",caTemp);
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select a.sid,a.tid,a.content,a.ktype,a.caseflags,b.name from ncskeycont a left join ncskeyindex b on (a.tid=b.id) where 1=1");
    	if(strlen(caKid)>0)
    	sprintf(caTemp+strlen(caTemp)," and (a.tid=%s ) ",caKid);
    	
    if(strlen(caPid)>0){
    	sprintf(caTemp+strlen(caTemp)," and tid in (select id from ncskeyindex where pid=400) ");
    }
    else{
    	sprintf(caTemp+strlen(caTemp)," and tid in (select id from ncskeyindex where pid!=400) ");
    }
    	
     if(strlen(caKeyword)>0){
    	 sprintf(caTemp+strlen(caTemp)," and a.content like '%c%s%c' ",'%',caKeyword,'%');
     }
    	sprintf(caTemp+strlen(caTemp)," order by a.tid,a.content desc limit %d,%d",lStartRec,lRowNum);
    	
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
    lktype=0;
    lcaseflags=0;
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&lTid,
      	                               UT_TYPE_STRING,63,caContent,
      	                               UT_TYPE_LONG,4,&lktype,
      	                               UT_TYPE_LONG,4,&lcaseflags,
      	                               UT_TYPE_STRING,31,caName);
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"tid",iNum,"%lu",lTid);
        utPltPutLoopVar(psDbHead,"content",iNum,caContent);
        utPltPutLoopVarF(psDbHead,"ktype",iNum,"%lu",lktype);
        utPltPutLoopVarF(psDbHead,"caseflags",iNum,"%lu",lcaseflags);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
         if(strcmp(caPre,caName)!=0){
        	utPltPutLoopVar(psDbHead,"type_desc",iNum,caName);
        }
         strcpy(caPre,caName);

        if(lktype==1)
        	utPltPutLoopVar(psDbHead,"ktype_desc",iNum,"正则表达式");
        else if(lktype==2)
           utPltPutLoopVar(psDbHead,"ktype_desc",iNum,"扩展正则表达式");
        else
          utPltPutLoopVar(psDbHead,"ktype_desc",iNum,"文本匹配");
          
       if(lcaseflags==1)
       		utPltPutLoopVar(psDbHead,"caseflags_desc",iNum,"省级");
       else if(lcaseflags==2)
          utPltPutLoopVar(psDbHead,"caseflags_desc",iNum,"部级");
       else
       	  utPltPutLoopVar(psDbHead,"caseflags_desc",iNum,"市级");
			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_keword_list.htm");
	  return 0;
}

//根据权限,获取单位组ID组合
char *getDsGroupid(){
	static char caReturn[1024];
	char caIds[1024],caTempid[1024];
	char caOpt[256];
	int iReturn,lId,iNum;
	 pasDbCursor *psCur;
	 memset(caReturn,0,sizeof(caReturn));
   lId=0;

    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    printf("dsiReturn=%d\n",iReturn);
    if(iReturn!=0||lId<=0){
    	 return &caReturn[0];
    }

    	memset(caOpt,0,sizeof(caOpt));
 	    pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
 	    if(strstr(caOpt,"all")){
 	    	return &caReturn[0];
    }


	
	 	  	 sprintf(caReturn,"%s",caOpt);
	 	  	 sprintf(caIds,"%s",caOpt);
	 	  	 
	 	  while(strlen(caIds)>0){
	 	  	memset(caTempid,0,sizeof(caTempid));
	 	  	iNum=0; 
	 	  	printf("select groupid from ncsgroup where pid in(%s)\n ",caIds);
	 	     psCur = pasDbOpenSqlF("select groupid from ncsgroup where pid in(%s) ",caIds);
	 	     if(psCur){
	 	     	iReturn=0;
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId);
	 	     	while(iReturn==0){
	 	     		if(iNum==0){
	 	     			sprintf(caTempid,"%d",lId);
	 	     		}
	 	     		else{
	 	     			sprintf(caTempid+strlen(caTempid),",%d",lId);
	 	     		}
	 	     			
	 	     		iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId);
	 	      			
	 	     			iNum++;
	 	     		
	 	     	}
	 	     }
	 	  	   pasDbCloseCursor(psCur);               
	 	  	
		 	  	if(strlen(caTempid)>0){
		 	  		sprintf(caReturn+strlen(caReturn),",%s",caTempid);
		 	  	}
		 	  	strcpy(caIds,caTempid);
	 	  	}
	 	  	
	 	  	
	 	 
	 	  return &caReturn[0];
 }


//由部门ID获取部门组ID组合

char *getGroupid(long lGroupid){
	static char caReturn[1024];
	char caIds[1024],caTempid[1024];
	int iReturn,lId,iNum;
	 pasDbCursor *psCur;
	 memset(caReturn,0,sizeof(caReturn));

	
	 	  	 sprintf(caReturn,"%d",lGroupid);
	 	  	 sprintf(caIds,"%d",lGroupid);
	 	  	 
	 	  while(strlen(caIds)>0){
	 	  	memset(caTempid,0,sizeof(caTempid));
	 	  	iNum=0; 
	// 	  	printf("select groupid from ncsgroup where pid in(%s)\n ",caIds);
	 	     psCur = pasDbOpenSqlF("select groupid from ncsgroup where pid in(%s) ",caIds);
	 	     if(psCur){
	 	     	iReturn=0;
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId);
	 	     	while(iReturn==0){
	 	     		if(iNum==0){
	 	     			sprintf(caTempid,"%d",lId);
	 	     		}
	 	     		else{
	 	     			sprintf(caTempid+strlen(caTempid),",%d",lId);
	 	     		}
	 	     			
	 	     		iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId);
	 	      			
	 	     		iNum++;
	 	     			
	 	     	}
	 	     }
	 	  	   pasDbCloseCursor(psCur);               
	 	  	
		 	  	if(strlen(caTempid)>0){
		 	  		sprintf(caReturn+strlen(caReturn),",%s",caTempid);
		 	  	}
		 	  	strcpy(caIds,caTempid);
	 	  	}
	 	  	
	 	  	
	 	 
	 	  return &caReturn[0];
 }

//由部门ID获取部门组ID组合

char *getNetidByGid(long lGid){
	static char caReturn[1024];
	char caIds[1024],caTempid[1024];
	int iReturn,lId,iNum;
	 pasDbCursor *psCur;
	 memset(caReturn,0,sizeof(caReturn));

	
	 	  	iNum=0; 
	// 	  	printf("select groupid from ncsgroup where pid in(%s)\n ",caIds);
	 	     psCur = pasDbOpenSqlF("select ntype from ncsnettype where gid=%d limit 0,100",lGid);
	 	     if(psCur){
	 	     	iReturn=0;
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId);
	 	     	while(iReturn==0){
	 	     		if(iNum==0){
	 	     			sprintf(caTempid,"%d",lId);
	 	     		}
	 	     		else{
	 	     			sprintf(caTempid+strlen(caTempid),",%d",lId);
	 	     		}
	 	     			
	 	     		iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId);
	 	      			
	 	     		iNum++;
	 	     			
	 	     	}
	 	     }
	 	  	   pasDbCloseCursor(psCur);               
	 	  	
		 	  	if(strlen(caTempid)>0){
		 	  		sprintf(caReturn+strlen(caReturn),"%s",caTempid);
		 	  	}
		 	  
	 	  	
	 	 
	 	  return &caReturn[0];
 }


//根据厂商权限,获取单位组ID组合
char *getDsFacGroupid(){
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

//根据厂商权限,获取单位iD组合
char *getDsFacDid(){
	static char caReturn[1024];
	char caIds[1024],caTempid[1024];
	char caOpt[256];
	int iReturn,lId,iNum;
	 pasDbCursor *psCur;
	 char caObj[100][50];
	 char caFacs[1024];
	 long i;
   long num=0;
	 memset(caReturn,0,sizeof(caReturn));

  strcpy(caFacs,getDsFacGroupid());
  memset(caTempid,0,sizeof(caTempid));
   psCur = pasDbOpenSqlF("select userid from ncsuser where fcode in(%s) limit 0,100 ",caFacs);
	 	     if(psCur){
	 	     	iReturn=0;
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId);
	 	     	iNum=0;
	 	     	while(iReturn==0){
	 	     		if(iNum==0){
	 	     			sprintf(caTempid,"%d",lId);
	 	     		}
	 	     		else{
	 	     			sprintf(caTempid+strlen(caTempid),",%d",lId);
	 	     		}
	 	     			
	 	     		iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId);
	 	      			
	 	     			iNum++;
	 	     		
	 	     	}
	 	     }
	 	  	   pasDbCloseCursor(psCur);  
  
  
  
  
  
  
 
 	 
	 	 return &caTempid[0];
 }







//显示系统用户管理
int ncsSysUserlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	 long preid,lId;
	 char caGroupname[32];
	 long lTime,lLevel,i;
	 char opt_desc[1024],caOpt[256];
	 char caSort[20];
	 char caTelphone[64],caHandphone[32],caGroup[32],caEmail[64],caDispname[32],caGroupid[32];
	 struct groupdate{
 	    char id[16];
 	  	char name[32];
 	  };
 	  struct groupdate *pGdate,*pFdate;
 	  char caCode[16],caFac[256],fac_desc[256];
 	  long lFcount,lId_s;
 	  struct roledate{
 	    long id;
 	  	char name[512];
 	  };
 	  struct roledate *pRole;
 	  long lRolecount,lGroupcount;
	  long lUseflags;
	  char caGroups[1024];
    strcpy(caGroups,getDsGroupid());

	 	  
	  lId_s=0;
	  iReturn = dsCltGetMyInfo(1,"Userid",&lId_s);
	 
	 
	 
	  //用户角色        
  	pasDbOneRecord("select count(distinct id) from dsuserrole",0,UT_TYPE_LONG,4,&lCount);
 	  
 	  pRole=(struct roledate*)malloc(lCount*sizeof(struct roledate));
 	  if(pRole==NULL){
 	  	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错1 ");
        return 0;
 	  }
 	  
 	  sprintf(caTemp,"select b.id,a.dispname from dsuserrole b,dsrole a where a.id=b.roleid order by b.id");

 	  psCur = pasDbOpenSql(caTemp,0);

    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错2 ");
        return 0;
    }   
    lId=0;
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,31,caGroupname);
    lRolecount=0;
    preid=-1;
    
    while(iReturn==0||iReturn==1405){
    	if(preid!=lId){
    		pRole[lRolecount].id=lId;

    	strcpy(pRole[lRolecount].name,caGroupname);
    	lRolecount++;
    	preid=lId;
     }
     else{
      if(strlen(pRole[lRolecount-1].name)<490)
     	sprintf(pRole[lRolecount-1].name+strlen(pRole[lRolecount-1].name),",%s",caGroupname);
     	
    }
    	lId=0;
    	memset(caGroupname,0,sizeof(caGroupname));
    	iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,31,caGroupname);
    }
    pasDbCloseCursor(psCur);
	 
	     //管理区域
 	  pasDbOneRecord("select count(*) from ncsgroup",0,UT_TYPE_LONG,4,&lCount);
 	  pGdate=(struct groupdate*)malloc(lCount*sizeof(struct groupdate));
 	  if(pGdate==NULL){
 	  	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错3 ");
        return 0;
 	  }
 	  
 	  sprintf(caTemp,"select groupid,groupname from ncsgroup");
 	  

 	  psCur = pasDbOpenSql(caTemp,0);

    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错4 ");
        return 0;
    }   
    lId=0;
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,31,caGroupname);
    lGroupcount=0;
    while(iReturn==0||iReturn==1405){
    	sprintf(pGdate[lGroupcount].id,"%d",lId);
    	strcpy(pGdate[lGroupcount].name,caGroupname);
    	lGroupcount++;
    	lId=0;
    	memset(caGroupname,0,sizeof(caGroupname));
    	iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,31,caGroupname);
    }   
    
 	  pasDbCloseCursor(psCur);
	 
	 
	 	     //管理厂商
 	  pasDbOneRecord("select count(*) from ncsfactorycod",0,UT_TYPE_LONG,4,&lCount);
 	  pFdate=(struct groupdate*)malloc(lCount*sizeof(struct groupdate));
 	  if(pGdate==NULL){
 	  	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错3 ");
        return 0;
 	  }
 	  
 	  sprintf(caTemp,"select code,name from ncsfactorycod");
 	  

 	  psCur = pasDbOpenSql(caTemp,0);

    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错4 ");
        return 0;
    }   
    lId=0;
    memset(caCode,0,sizeof(caCode));
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
                                 UT_TYPE_STRING,31,caGroupname);
    lFcount=0;
    while(iReturn==0||iReturn==1405){
    	sprintf(pFdate[lFcount].id,"%s",caCode);
    	strcpy(pFdate[lFcount].name,caGroupname);
    	lFcount++;
    memset(caCode,0,sizeof(caCode));
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
                                 UT_TYPE_STRING,31,caGroupname);
    }   
    
 	  pasDbCloseCursor(psCur);
	 
	 
	 
	 
	 
	 
	 
	  utMsgPrintMsg(psMsgHead);
	  memset(caPre,0,sizeof(caPre));
	  iReturn = utMsgGetSomeNVar(psMsgHead,10,
	                 "groupid",UT_TYPE_STRING,12,caGroupid,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "sort",          UT_TYPE_STRING,15,caSort,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,			             
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel);  
      if(strcmp(caSort,"username")==0){
      	strcpy(caSort,"name");
      }
      else if(strcmp(caSort,"groupname")==0){
      	strcpy(caSort,"a.groupid");
      }
      if(strlen(caSort)==0){
      	strcpy(caSort,"a.id");
      }
      if(strlen(caDir)==0){
      	strcpy(caDir,"desc");
      }


	  if(strlen(caKeyword)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,63));
	  	strcpy(caKeyword,caTemp_d);
	  }          
			              

	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  

	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	 // 	printf("delete from  dsuser  where id in(%s)\n",caSelsid);
	  	pasDbExecSqlF("delete from  dsuser  where id in(%s)",caSelsid);
	  }
	  
	  
	  
	  
	  sprintf(caTemp,"select count(*) from dsuser where 1=1 ");
    if(strlen(caGroupid)>0)
    sprintf(caTemp+strlen(caTemp)," and a.groupid in(%s)  ",getGroupid(atol(caGroupid)));
    if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and a.groupid in (%s) ",caGroups);
    }
    
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and (a.name like '%c%s%c' or a.groupname like '%c%s%c' or a.dispname like '%c%s%c' or a.email like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    }
    

    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	  sprintf(caTemp,"select a.id,a.name,a.dispname,a.groupname,a.addtime,a.email,a.usrlevel,a.telphone,a.handphone,b.groupname,opt,fac,useflags from dsuser a left join ncsgroup b on a.groupid=b.groupid where 1=1 ");
	  
    if(strlen(caGroupid)>0)
    sprintf(caTemp+strlen(caTemp)," and a.groupid in(%s)  ",getGroupid(atol(caGroupid)));
    if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and a.groupid in (%s) ",caGroups);
    }
    
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and (a.name like '%c%s%c' or a.groupname like '%c%s%c' or a.dispname like '%c%s%c' or a.email like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    }
    sprintf(caTemp+strlen(caTemp)," order by %s %s limit %d,%d",caSort,caDir,lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
    memset(caGroupname,0,sizeof(caGroupname));
    memset(caHandphone,0,sizeof(caHandphone));
    memset(caTelphone,0,sizeof(caTelphone));
    memset(caEmail,0,sizeof(caEmail));
    memset(caDispname,0,sizeof(caDispname));
    memset(caOpt,0,sizeof(caOpt));
    memset(caFac,0,sizeof(caFac));
    lUseflags=0;
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                    UT_TYPE_STRING,31,caName,
                                    UT_TYPE_STRING,31,caDispname,
                                    UT_TYPE_STRING,31,caGroup,
                                    UT_TYPE_LONG,   4,&lTime,
                                    UT_TYPE_STRING,63,caEmail,
                                    UT_TYPE_LONG,4,&lLevel,
                                    UT_TYPE_STRING,63,caTelphone,
                                    UT_TYPE_STRING,31,caHandphone,
                                    UT_TYPE_STRING,31,caGroupname,
                                    UT_TYPE_STRING,255,caOpt,
                                    UT_TYPE_STRING,255,caFac,
                                    UT_TYPE_LONG,4,&lUseflags);
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
            utPltPutLoopVarF(psDbHead,"num", iNum,"%lu",iNum);
            utPltPutLoopVarF(psDbHead,"userid", iNum,"%lu",lId);
            utPltPutLoopVar(psDbHead,"username",iNum,caName);
            utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
            utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
            utPltPutLoopVar(psDbHead,"email",iNum,caEmail);
            utPltPutLoopVar(psDbHead,"addtime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lTime));
            utPltPutLoopVar(psDbHead,"level",iNum,ncsGetFirstNameById(lLevel));
            utPltPutLoopVarF(psDbHead,"level2",iNum,"%d",lLevel);
            utPltPutLoopVar(psDbHead,"telphone",iNum,caTelphone);
            utPltPutLoopVar(psDbHead,"handphone",iNum,caHandphone);
            utPltPutLoopVarF(psDbHead,"useflags",iNum,"%d",lUseflags);
            
            if(strstr(caOpt,"all")||(strlen(caOpt)==0)){
            	utPltPutLoopVar(psDbHead,"opt",iNum,"全部");
            }
            else{
            	memset(opt_desc,0,sizeof(opt_desc));
            	for(i=0;i<lGroupcount;i++){
          //  		if(strstr(caOpt,pGdate[i].id))
              if(isGroupID(caOpt,pGdate[i].id)==1)
            		sprintf(opt_desc+strlen(opt_desc),"%s ",pGdate[i].name);
            		
            	}
            	utPltPutLoopVar(psDbHead,"opt",iNum,opt_desc);
            }
            
            memset(fac_desc,0,sizeof(fac_desc));
            if(strstr(caFac,"all")||(strlen(caFac)==0)){
            	utPltPutLoopVar(psDbHead,"fac",iNum,"全部");
            }
            else{
            	for(i=0;i<lFcount;i++){
            	if(isGroupID(caFac,pFdate[i].id)==1){
            		sprintf(fac_desc+strlen(fac_desc),"%s ",pFdate[i].name);
            	}
            }
            utPltPutLoopVar(psDbHead,"fac",iNum,fac_desc);
            utPltPutLoopVar(psDbHead,"facid",iNum,caFac);
          }
            
            
            
            for(i=0;i<lRolecount;i++){
            	if(pRole[i].id==lId){
            		utPltPutLoopVar(psDbHead,"role",iNum,pRole[i].name);
            		break;
            	}
            }
			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	//  ncsWriteSysLog(psMsgHead,caUsername2,"06","成功","查看系统用户列表信息");

     free(pGdate); 
     free(pFdate);
     free(pRole); 
	  
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_sysuser_list.htm");
	  return 0;
}


//显示角色列表
int ncsRolelist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  char caContent[64],caKtype[8],caCaseflags[8];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[2048],caStatus[16];
	  char caRightcheck[2048],caRolename[64];
	  char caTid[16],caKid[16];
	  long lSid,lTid;
	  char caPre[36];
	  long iNum;
	   long lktype,lcaseflags,ltid;
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	 long preid,lId;
	 char caGroupname[32];
	 long lTime,lLevel,i;

 	  long lPasid;
	  pasLHashInfo sHashInfo;
	  char *pHash;
	  struct rData_s{
 	 	unsigned long id;  
 	  char caFun[2048];
 	  char caUser[2048];
 	};
	 struct rData_s *psData;
	 char caOut[200][256];
	 long lSum;
	 char caRoleid[16];
	 memset(caOut,0,200*36);
	 
	   

	 
	 
	 	  utMsgPrintMsg(psMsgHead);
    /* 根据Roleid和Pasid取相应的权限信息  */
    pasDbOneRecord("select id from dsappsrv",0,UT_TYPE_LONG,4,&lPasid);

	 

	  iReturn = utMsgGetSomeNVar(psMsgHead,11,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,			             
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "rolename",UT_TYPE_STRING,31,caRolename,
			             "rightcheck",UT_TYPE_STRING,2000,caRightcheck,
			             "roleid",UT_TYPE_STRING,10,caRoleid);  

	  if(strlen(caKeyword)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,63));
	  	strcpy(caKeyword,caTemp_d);
	  }          
			    
	 if(strlen(caRightcheck)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caRightcheck,caTemp,2000));
	  	strcpy(caRightcheck,caTemp_d);
	  }              
	  if(strlen(caRolename)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caRolename,caTemp,63));
	  	strcpy(caRolename,caTemp_d);
	  }


	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
//printf("caRightcheck=%s\n",caRightcheck);
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	 // 	printf("delete from  dsuser  where id in(%s)\n",caSelsid);
	    iReturn=pasDbExecSqlF("delete from dsuserrole where roleid  in(%s) and roleid!=101 ",caSelsid);
	  	iReturn=pasDbExecSqlF("delete from dsroleattr where roleid  in(%s) and roleid!=101 ",caSelsid);
	  	
	  	if(iReturn==0){
	  		pasDbExecSqlF("delete from dsrole where id  in(%s) and id!=101 ",caSelsid);
	  	}
	  }
	  
	  if((strcmp(caUpdate,"update")==0)&&strlen(caRolename)>0){
	  	lCount=sepcharbydh(caRightcheck,caOut);
	  	
	  	sprintf(caTemp,"select id from dsrole where name='%s' ",caRolename);
//	  	printf("caTemp=%s\n",caTemp);
	  	lId=0;
	  	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lId);
	  	if(lId==0){
	  		lId = pasGetSid("dsrole","id");
	  		iReturn = pasDbExecSqlF("insert into dsrole(id,name,dispname,descr,addtime,moditime) values (%lu,'%s','%s','%s',%lu,%lu)",
                                             lId,caRolename,caRolename,caRolename,time(0),time(0));
                                             
        if(iReturn==0){
        	pasDbExecSqlF("delete from dsroleattr where roleid=%d ",lId);
   //     	printf("delete from dsroleattr where roleid=%d \n",lId);
        	for(i=0;i<lCount;i++){
        		if(strlen(caOut[i])>0)
   //     		printf("caOut=%s\n",caOut[i]);
  //      		printf("insert into dsroleattr(roleid,pasid,flags,lright) values(%d,%d,0,'%s')\n",lId,lPasid,caOut[i]);
        		  pasDbExecSqlF("insert into dsroleattr(roleid,pasid,flags,lright) values(%d,%d,0,'%s')",lId,lPasid,caOut[i]);
        	}
            	
        }                                     
	  	}
	  	else {
	  			iReturn = pasDbExecSqlF("update dsrole set name='%s',dispname='%s',descr='%s',moditime=%lu where id=%d ",  			
                                             caRolename,caRolename,caRolename,time(0),lId);
           pasDbExecSqlF("delete from dsroleattr where roleid=%d ",lId);
        	for(i=0;i<lCount;i++){
        		if(strlen(caOut[i])>0)
        		  pasDbExecSqlF("insert into dsroleattr(roleid,pasid,flags,lright) values(%d,%d,0,'%s')",lId,lPasid,caOut[i]);
        	}                                  
                                             
	  	}
	  	
	  }
	//授权  
	  if((strcmp(caUpdate,"grant")==0)&&(strlen(caRoleid)>0)){
	  	lCount=sepcharbydh(caRightcheck,caOut);
	  	iReturn=pasDbExecSqlF("delete from dsuserrole where roleid=%d",atol(caRoleid));
	  	if(lCount>0){
	  	
	  		if(iReturn==0){
	  			for(i=0;i<lCount;i++){
	  				pasDbExecSqlF("insert into dsuserrole(id,roleid,addtime,addby) values(%d,%d,%d,'')",atol(caOut[i]),atol(caRoleid),time(0));
	  			}
	  		}
	  	}
	  	
	  }
	  
	  
	     //将权限功能列表放到HASH表中

   lCount=0;
   pasDbOneRecord("select count(*) from dsrole ",0,UT_TYPE_LONG,4,&lCount);
   if(lCount<100){
   	lSum=100;
  }
  else {
  	lSum=lCount*1.2;
  }
  printf("lSum=%d\n",lSum);
	 pHash = (unsigned char *)pasLHashInit(lSum,lSum,sizeof(struct rData_s),0,4); 
	 if(pHash==NULL){
	 	printf("init hash error\n");
	 	return 0;
	}
	
	  sprintf(caTemp,"select roleid,lright from dsroleattr where pasid=%d order by lright",lPasid);
	  
	  
	  
 	  psCur = pasDbOpenSql(caTemp,0);
//printf("caTemp=%s\n",caTemp);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错2 ");
        return 0;
    }   
    lId=0;
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,31,caGroupname);

    
    while(iReturn==0||iReturn==1405){
    	
    	psData = (struct rData_s *)pasLHashLookA(pHash,&lId); 
    	if(psData){
    		if(strlen(psData->caFun)==0){
    			strcpy(psData->caFun,caGroupname);
    		}
    		else{
    			sprintf(psData->caFun+strlen(psData->caFun),",%s",caGroupname);
    		}
    	}
    	
    	lId=0;
    	memset(caGroupname,0,sizeof(caGroupname));
    	iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,31,caGroupname);
    }
    	
    pasDbCloseCursor(psCur);
	  
//将角色对象放到hash中

	  sprintf(caTemp,"select a.roleid,b.dispname from dsuserrole a,dsuser b where a.id=b.id group by b.dispname,a.roleid;");
 	  psCur = pasDbOpenSql(caTemp,0);
printf("caTemp=%s\n",caTemp);
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错2 ");
        return 0;
    }   
    lId=0;
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,31,caGroupname);

    
    while(iReturn==0||iReturn==1405){
    	
    	psData = (struct rData_s *)pasLHashLookA(pHash,&lId); 
    	if(psData){
    		if(strlen(psData->caUser)==0){
    			strcpy(psData->caUser,caGroupname);
    		}
    		else{
    			sprintf(psData->caUser+strlen(psData->caUser),",%s",caGroupname);
    		}
    	}
    	
    	lId=0;
    	memset(caGroupname,0,sizeof(caGroupname));
    	iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                 UT_TYPE_STRING,31,caGroupname);
    }
    	
    pasDbCloseCursor(psCur);
	  
	  
	  
	  
	  
	  
	  long lCount2;
	  iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    if(iReturn!=0||lId<=0){
       lId=0;
    }
	   if(lId>0){
          sprintf(caTemp,"select count(*) from dsuserrole where roleid=101 and id=%d ",lId);
        	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount2);
    }
	  
	  
	  if(lCount2>0||lId==0){
	      sprintf(caTemp,"select count(*) from dsrole where 1=1 ");
     }
     else{
     	  sprintf(caTemp,"select count(distinct roleid) from dsrole,dsuserrole where dsrole.id=roleid and dsuserrole.id=%d ",lId);
    }
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and (name like '%c%s%c') ",'%',caKeyword,'%');
    }
    lCount=0;
    
    printf("caTemp=%s\n",caTemp);
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	   if(lCount2>0||lId==0){
	 		 sprintf(caTemp,"select id,name from dsrole where 1=1 ");
	 		}
	 		else{
	 			 sprintf(caTemp,"select distinct roleid,name from dsrole ,dsuserrole where dsrole.id=roleid and dsuserrole.id=%d ",lId);
	 		}
	  
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and (name like '%c%s%c') ",'%',caKeyword,'%');
    }
    	sprintf(caTemp+strlen(caTemp)," order by name limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

    psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
    lId=0;
    memset(caName,0,sizeof(caName));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lId,
                                    UT_TYPE_STRING,31,caName);
                                   
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
            utPltPutLoopVarF(psDbHead,"num", iNum,"%lu",iNum);
            utPltPutLoopVarF(psDbHead,"userid", iNum,"%lu",lId);
            utPltPutLoopVar(psDbHead,"username",iNum,caName);
          
            psData=pasLHashLook(pHash,&lId); 
            if(psData){
            	utPltPutLoopVar(psDbHead,"fun",iNum,psData->caFun);
            	utPltPutLoopVar(psDbHead,"dx",iNum,psData->caUser);
            }
          
            
			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	//  ncsWriteSysLog(psMsgHead,caUsername2,"06","成功","查看系统用户列表信息");

 free(pHash); 
    
	//  utPltShowDb(psDbHead);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_role_list.htm");
	  return 0;
}




//显示角色功能列表列表
int ncsDispRoleFunlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    int iReturn,iNum,iSum1,iSum2,k;
    long lStatus,lId,j;
    char caResult1[16000];
    char caResult2[16000];
    char caSmt[1256];
    long lPasid;
    char caPasid[16],caRoleid[16];
    char caName[32],caDispname[32],caName2[32];
    char *p,*p2;
    long lCount;
    char caTemp[1024];

    utMsgPrintMsg(psMsgHead);

//    iReturn = utMsgGetSomeNVar(psMsgHead,1,
  //                  "roleid", UT_TYPE_STRING,12,caRoleid);
    pasDbOneRecord("select id from dsappsrv",0,UT_TYPE_LONG,4,&lPasid);
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    if(iReturn!=0||lId<=0){
       lId=0;
    }
     lCount=0;
    if(lId>0){
          sprintf(caTemp,"select count(*) from dsuserrole where roleid=101 and id=%d ",lId);
        	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    }
    if(lCount>0||lId==0){
         sprintf(caSmt,"select name,dispname from dscltright where pasid=%d",lPasid);
    }
    else{
    	sprintf(caSmt,"select distinct lright,dscltright.dispname from dsroleattr,dsrole,dsuserrole,dscltright where dsrole.id=dsroleattr.roleid and dsuserrole.roleid=dsrole.id and dscltright.name=lright and dsuserrole.id=%d ",lId);
    	
    }
    printf("caSmt=%s\n",caSmt);
    /* 根据Roleid和Pasid取相应的权限信息  */

   /*
   sprintf(caSmt,"select name from dsrole where id=%d ",atol(caRoleid));
   pasDbOneRecord(caSmt,0,UT_TYPE_STRING,31,caName2);
   
    sprintf(caSmt,"select lright from dsroleattr where roleid=%s and pasid=%d",caRoleid,lPasid);

    iReturn = dsCltSqlRecordSet(caSmt,1,9999,&iSum1,caResult1);

    if(iReturn != 0 && iReturn != 1403 ) { 
        iSum1 = 0;
        strcpy(caResult1,"\0");
    }
  */  
  

    iReturn = dsCltSqlRecordSet(caSmt,1,9999,&iSum2,caResult2);

    psDbHead = utPltInitDb();
    p = caResult2;
    iNum = 0;
    j = 0;
           

    for(k=0;k<iSum2;k++) {
        p = utStrGetRecordValue(p,2,
                    UT_TYPE_STRING,31,caName,
                    UT_TYPE_STRING,31,caDispname);


            iNum++;
          if(iNum>1){
       	  	  utPltPutLoopVar(psDbHead,"dh",iNum,",");
       	  }
            utPltPutLoopVarF(psDbHead,"vara",iNum,"v%d",k);
            utPltPutLoopVar(psDbHead,"namea", iNum,caName);
            utPltPutLoopVar(psDbHead,"dispnamea", iNum,caDispname);
   //         if(iSum1>0 && utStrIsExistRecord(caResult1,1,1,caName)) {
    //            utPltPutLoopVar(psDbHead,"checka", iNum,"checked");
  //          }
  //          else{
  //          	utPltPutLoopVar(psDbHead,"checka", iNum,"checked");
  //          }
                   
    }
    
    	  utPltPutVarF(psDbHead,"TotRec","%d",iSum2);
   // 	  utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_rolefun_list.htm");
    return 0;
}


//权限配置文件
int ncsSysUserRoleConfig_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[1024];
   	int  iNum;
    char caFun[2048];
    char caTool[2048];
    long lLevel;
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64],caSy[64];
    long lId,lSid;
    
    dsCltGetSessionValue(1,"id",UT_TYPE_LONG,sizeof(long),&lId);
    	
    utMsgPrintMsg(psMsgHead);
    memset(caFun,0,sizeof(caFun));
    
    sprintf(caTemp,"select tname from dstoolsy_v4 where id=%d ",lId);
    memset(caSy,0,sizeof(caSy));
    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caSy);
   if(strlen(caSy)==0){
    sprintf(caTemp,"select usrlevel from dsuser where id=%d ",lId);
    pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lLevel);
  strcpy(caSy,ncsGetFirstNameById(lLevel));
    }
    
    sprintf(caTemp,"select lright from dsuser a,dsuserrole b,dsroleattr c,dscltright d where a.id=b.id and b.roleid=c.roleid and d.name=lright and a.id=%d group by lright",lId);  
    
    
		 psCur = pasDbOpenSqlF(caTemp);
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
	     memset(caName,0,sizeof(caName));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName);
		   iNum=0;	 
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum==0){
		         		strcpy(caFun,caName);
		         }
		         else{
		         	  sprintf(caFun+strlen(caFun),",%s",caName);
		         }
		         iNum++;
		        
				     memset(caName,0,sizeof(caName));
					   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName);
		    }
		    pasDbCloseCursor(psCur);
		
     psDbHead = utPltInitDb();
     utPltPutVar(psDbHead,"sy",caSy);
     utPltPutVar(psDbHead,"fun",caFun);
    	
    	memset(caTool,0,sizeof(caTool));
    	sprintf(caTemp,"select tname from dstool_v4 where id=%d ",lId);
    	psCur=pasDbOpenSqlF(caTemp);
    	memset(caTool,0,sizeof(caTool));
    	if(psCur){
    		memset(caName,0,sizeof(caName));
    		iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName);
    		while(iReturn==0||iReturn==1405){
    		
    		sprintf(caTool+strlen(caTool),",%s",caName);
    		memset(caName,0,sizeof(caName));
    		iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName);
    	}
    	pasDbCloseCursor(psCur);
    }
    utPltPutVar(psDbHead,"tool",caTool);
    	
    	
// utPltShowDb(psDbHead);

    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_right_config.htm");
    

    return 0;
}

//系统刷新
int ncsSysReflesh_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	utPltDbHead *psDbHead;
	int iReturn;
	
	    utMsgPrintMsg(psMsgHead);
	psDbHead = utPltInitDb();
	
	iReturn=0;
  if(iReturn==0){
  	 utPltPutVar(psDbHead,"mesg","true");
  }
  else{
  	utPltPutVar(psDbHead,"mesg","false");
  }
 
  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_msg.htm");
   iReturn=ncsResetSystem(psShmHead);
  return 0;
}



//有关接入厂商的树
int ncsTreeFac_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
    char caCode[16];
    char caFacs[1024];	
    strcpy(caFacs,getDsFacGroupid());
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

    
        sprintf(sqlbuf,"select code,name from ncsfactorycod where 1=1 ");
        if(strlen(caFacs)>0){
        	sprintf(sqlbuf+strlen(sqlbuf)," and code in (%s) ",caFacs);
        }
        

		  	 psCur = pasDbOpenSql(sqlbuf,0);
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
	     memset(caName,0,sizeof(caName));
       memset(caCode,0,sizeof(caCode));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
		                                  UT_TYPE_STRING,63,caName);
		                                  
		                                
		
		 
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  		        
					   utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s",caName);					    
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%s",caCode);
		         utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
		         utPltPutLoopVar(psDbHead,"cls", iNum,"file");
		         	
	     memset(caName,0,sizeof(caName));
       memset(caCode,0,sizeof(caCode));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
		                                  UT_TYPE_STRING,63,caName);
		    }
		    pasDbCloseCursor(psCur);
		
        if(strlen(caPlate)>0){
        	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
        }
        else{
    	  	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");
    	  }

    

    return 0;
}


//有关单位类别的树
int ncsTreeLb_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
    char caCode[16];
    char caFacs[1024];	
    strcpy(caFacs,getDsFacGroupid());
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

    
        sprintf(sqlbuf,"select code,name from ncsuser_lb where 1=1 ");
        if(strlen(caFacs)>0){
        	sprintf(sqlbuf+strlen(sqlbuf)," and code in (%s) ",caFacs);
        }
        

		  	 psCur = pasDbOpenSql(sqlbuf,0);
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
	     memset(caName,0,sizeof(caName));
       memset(caCode,0,sizeof(caCode));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
		                                  UT_TYPE_STRING,63,caName);
		                                  
		                                
		
		 
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  		        
					   utPltPutLoopVarF(psDbHead,"groupname",iNum,"%s",caName);					    
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%s",caCode);
		         utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
		         utPltPutLoopVar(psDbHead,"cls", iNum,"file");
		         	
	     memset(caName,0,sizeof(caName));
       memset(caCode,0,sizeof(caCode));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
		                                  UT_TYPE_STRING,63,caName);
		    }
		    pasDbCloseCursor(psCur);
		
        if(strlen(caPlate)>0){
        	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
        }
        else{
    	  	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");
    	  }

    

    return 0;
}




//关键字日志树-包含关键字组和关键字
int ncsTreeKeyword_log_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[4024];
   	char sqlbuf[1024];
   	long lFlag,lKid;
   	int  iNum;
  
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64];
    long lId,lSid,lSum;
    char caPlate[128];
    char caId[16];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
    char *pHash;
    char caFlag[16];
   ncsKeywordStat *psData,*pOutdata;
   typedef struct ncsKey_s{
   	  long id;
   	  char caKeyword[64];
   	} ncsKey;
   	ncsKey stKey;
   	pasLHashInfo sHashInfo;
    char caUsername2[36];
    char caGroups[2048];
    strcpy(caGroups,getDsGroupid());  
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,2, "id",           UT_TYPE_STRING,10,caId,
	                                          "flag",         UT_TYPE_STRING,10,caFlag);
	 memset(caUsername2,0,sizeof(caUsername2));         
   dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
 
    iNum = 0;
    lSum=0;
     pHash=(unsigned char *)pasLHashInit(10000,10000,sizeof(struct ncsKeywordStat_s),0,68); 
    	if(pHash==NULL){
    		return -1;
    	}
    pOutdata=(struct ncsKeywordStat_s*)malloc(10000*sizeof(struct ncsKeywordStat_s));
    if(pOutdata==NULL){
    	return -1;
    }
    
   
        iNum=0;
        if(strlen(caId)==0){
        	sprintf(caTemp,"select name,id,count(*),flag from ncskeywordlog,ncskeyindex where kid=id and flag!=9 ");
        	if(strlen(caFlag)>0){
        		sprintf(caTemp+strlen(caTemp)," and flag=%s ",caFlag);
        	}
        	if(strlen(caGroups)>0){
        		sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
        	}
        	sprintf(caTemp+strlen(caTemp)," group by id,flag limit 0,10000 ");
        
        }
        else{
        	sprintf(caTemp,"select keyword,kid,count(*),flag from ncskeywordlog,ncskeyindex where kid=id and flag!=9 and kid=%s ",caId);
          if(strlen(caFlag)>0){
          	sprintf(caTemp+strlen(caTemp)," and flag=%s ",caFlag);
          }
          if(strlen(caGroups)>0){
        		sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
        	}
          sprintf(caTemp+strlen(caTemp)," group by keyword,kid,flag limit 0,20000 ");
        }
        printf("caTemp=%s\n",caTemp);
        psCur=pasDbOpenSqlF(caTemp);
         if(psCur){
         	          memset(caName,0,sizeof(caName));
         	          lId=0;
         	          lCount=0;
         	          lFlag=0;
         	    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,63,caName,
         	    	                                 UT_TYPE_LONG,4,&lId,
         	    	                                 UT_TYPE_LONG,4,&lCount,
         	    	                                 UT_TYPE_LONG,4,&lFlag);
                         while(iReturn==0||iReturn==1405)
										     {
										        memset(&stKey,0,sizeof(ncsKey));
										        stKey.id=lId;
										        strcpy(stKey.caKeyword,caName);
										     	  psData = (struct ncsKeywordStat_s *)pasLHashLookA(pHash,&stKey);  
										     	  if(psData){
										     	  	if(lFlag>0){
										     	  		psData->csum=psData->csum+lCount;
										     	  	}
										     	  	psData->sum=psData->sum+lCount;
										     	  }
										     	  
										     	  
				         	          memset(caName,0,sizeof(caName));
				         	          lId=0;
				         	          lCount=0;
				         	          lFlag=0;
				         	    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,63,caName,
				         	    	                                 UT_TYPE_LONG,4,&lId,
				         	    	                                 UT_TYPE_LONG,4,&lCount,
				         	    	                                 UT_TYPE_LONG,4,&lFlag);
										     }
										     pasDbCloseCursor(psCur); 
     	
         	               psData=(struct ncsKeywordStat_s *)pasLHashFirst(pHash,&sHashInfo);
         	               iNum=0;
         	               while(psData){
         	               	    pOutdata[iNum].id=psData->id;
         	               	    strcpy(pOutdata[iNum].caKeyword,psData->caKeyword);
         	               	    pOutdata[iNum].sum=psData->sum;
         	               	    pOutdata[iNum].csum=psData->csum;
         	               	    
         	               	    
         	               	    iNum++;
    	                        psData=(struct ncsKeywordStat_s *)pasLHashNext(&sHashInfo);
         	               }
         	               lSum=iNum;
         	               printf("lSum=%d\n",lSum);
  //       	               排序
         	         qsort(pOutdata,lSum,sizeof(struct ncsKeywordStat_s),ncsUtlSortKeyword);  
           }
        psDbHead = utPltInitDb();
         //输出
        for(i=0;i<lSum;i++){
         	if(i>500) break;
         	
         	if(i>0){
			       utPltPutLoopVar(psDbHead,"dh",i+1,",");
			    }
         	utPltSetCvtHtml(1);
         	if(strlen(caFlag)>0){
         		utPltPutLoopVarF(psDbHead,"groupname",i+1,"%s_(%d)",pOutdata[i].caKeyword,pOutdata[i].sum);
         	}
         	else{
          	utPltPutLoopVarF(psDbHead,"groupname",i+1,"%s_(%d/%d)",pOutdata[i].caKeyword,pOutdata[i].sum-pOutdata[i].csum,pOutdata[i].sum);
          }
  //   printf("i=%d,sum=%d,id=%d\n",i,pOutdata[i].sum,pOutdata[i].id);
         	utPltSetCvtHtml(0);
         	if(strlen(caId)==0){
         				utPltPutLoopVarF(psDbHead,"groupid",   i+1,"%ld",pOutdata[i].id);
         				utPltPutLoopVar(psDbHead,"leaf", i+1,"false");
		         		utPltPutLoopVar(psDbHead,"cls", i+1,"folder");
         	}
         	else{
         		    utPltPutLoopVarF(psDbHead,"did",   i+1,"%s",pOutdata[i].caKeyword);
         				utPltPutLoopVar(psDbHead,"leaf", i+1,"true");
		         		utPltPutLoopVar(psDbHead,"cls", i+1,"file");
         	}
         	
        }
        
    
       free(pHash);
       free(pOutdata);

		

    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");

    

    return 0;
}



//敏感网址日志树-包含网址库和网址
int ncsTreeUrl_log_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[4024];
   	char sqlbuf[1024];
   	long lFlag,lKid;
   	int  iNum;
  
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64];
    long lId,lSid,lSum;
    char caPlate[128];
    char caId[16];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
    char *pHash;
    char caFlag[16];
   ncsKeywordStat *psData,*pOutdata;
   typedef struct ncsKey_s{
   	  long id;
   	  char caKeyword[64];
   	} ncsKey;
   	ncsKey stKey;
   	pasLHashInfo sHashInfo;
    char caUsername2[36];
    char caGroups[2048];
    strcpy(caGroups,getDsGroupid());
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,2, "id",           UT_TYPE_STRING,10,caId,
	                                          "flag",         UT_TYPE_STRING,10,caFlag);
	 memset(caUsername2,0,sizeof(caUsername2));         
   dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
 
    iNum = 0;
    lSum=0;
     pHash=(unsigned char *)pasLHashInit(10000,10000,sizeof(struct ncsKeywordStat_s),0,68); 
    	if(pHash==NULL){
    		return -1;
    	}
    pOutdata=(struct ncsKeywordStat_s*)malloc(10000*sizeof(struct ncsKeywordStat_s));
    if(pOutdata==NULL){
    	return -1;
    }
    
   
        iNum=0;
        if(strlen(caId)==0){
        	sprintf(caTemp,"select name,id,count(*),flag from ncssenurllog,ncwebclass where kid=id and flag!=9 ");
        	if(strlen(caFlag)>0){
        		sprintf(caTemp+strlen(caTemp)," and flag=%s ",caFlag);
        	}
        	if(strlen(caGroups)>0){
    				sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
          }
        	sprintf(caTemp+strlen(caTemp)," group by id,flag limit 0,10000 ");
        
        }
        else{
        	sprintf(caTemp,"select keyword,kid,count(*),flag from ncssenurllog,ncwebclass where kid=id and flag!=9 and kid=%s ",caId);
          if(strlen(caFlag)>0){
          	sprintf(caTemp+strlen(caTemp)," and flag=%s ",caFlag);
          }
          if(strlen(caGroups)>0){
    				sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
          }
          sprintf(caTemp+strlen(caTemp)," group by keyword,kid,flag limit 0,20000 ");
        }
        printf("caTemp=%s\n",caTemp);
        psCur=pasDbOpenSqlF(caTemp);
         if(psCur){
         	          memset(caName,0,sizeof(caName));
         	          lId=0;
         	          lCount=0;
         	          lFlag=0;
         	    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,63,caName,
         	    	                                 UT_TYPE_LONG,4,&lId,
         	    	                                 UT_TYPE_LONG,4,&lCount,
         	    	                                 UT_TYPE_LONG,4,&lFlag);
                         while(iReturn==0||iReturn==1405)
										     {
										        memset(&stKey,0,sizeof(ncsKey));
										        stKey.id=lId;
										        strcpy(stKey.caKeyword,caName);
										     	  psData = (struct ncsKeywordStat_s *)pasLHashLookA(pHash,&stKey);  
										     	  if(psData){
										     	  	if(lFlag>0){
										     	  		psData->csum=psData->csum+lCount;
										     	  	}
										     	  	psData->sum=psData->sum+lCount;
										     	  }
										     	  
										     	  
				         	          memset(caName,0,sizeof(caName));
				         	          lId=0;
				         	          lCount=0;
				         	          lFlag=0;
				         	    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,63,caName,
				         	    	                                 UT_TYPE_LONG,4,&lId,
				         	    	                                 UT_TYPE_LONG,4,&lCount,
				         	    	                                 UT_TYPE_LONG,4,&lFlag);
										     }
										     pasDbCloseCursor(psCur); 
     	
         	               psData=(struct ncsKeywordStat_s *)pasLHashFirst(pHash,&sHashInfo);
         	               iNum=0;
         	               while(psData){
         	               	    pOutdata[iNum].id=psData->id;
         	               	    strcpy(pOutdata[iNum].caKeyword,psData->caKeyword);
         	               	    pOutdata[iNum].sum=psData->sum;
         	               	    pOutdata[iNum].csum=psData->csum;
         	               	    
         	               	    
         	               	    iNum++;
    	                        psData=(struct ncsKeywordStat_s *)pasLHashNext(&sHashInfo);
         	               }
         	               lSum=iNum;
         	               printf("lSum=%d\n",lSum);
  //       	               排序
         	         qsort(pOutdata,lSum,sizeof(struct ncsKeywordStat_s),ncsUtlSortKeyword);  
           }
        psDbHead = utPltInitDb();
         //输出
        for(i=0;i<lSum;i++){
         	if(i>500) break;
         	
         	if(i>0){
			       utPltPutLoopVar(psDbHead,"dh",i+1,",");
			    }
         	utPltSetCvtHtml(1);
         	if(strlen(caFlag)>0){
         		utPltPutLoopVarF(psDbHead,"groupname",i+1,"%s_(%d)",pOutdata[i].caKeyword,pOutdata[i].sum);
         	}
         	else{
          	utPltPutLoopVarF(psDbHead,"groupname",i+1,"%s_(%d/%d)",pOutdata[i].caKeyword,pOutdata[i].sum-pOutdata[i].csum,pOutdata[i].sum);
          }
  //   printf("i=%d,sum=%d,id=%d\n",i,pOutdata[i].sum,pOutdata[i].id);
         	utPltSetCvtHtml(0);
         	if(strlen(caId)==0){
         				utPltPutLoopVarF(psDbHead,"groupid",   i+1,"%ld",pOutdata[i].id);
         				utPltPutLoopVar(psDbHead,"leaf", i+1,"false");
		         		utPltPutLoopVar(psDbHead,"cls", i+1,"folder");
         	}
         	else{
         		    utPltPutLoopVarF(psDbHead,"did",   i+1,"%s",pOutdata[i].caKeyword);
         				utPltPutLoopVar(psDbHead,"leaf", i+1,"true");
		         		utPltPutLoopVar(psDbHead,"cls", i+1,"file");
         	}
         	
        }
            
       free(pHash);
       free(pOutdata);

		

    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");

    

    return 0;
}



//敏感虚拟身份日志树-包含虚拟身份组和虚拟身份
int ncsTreeNetid_log_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[4024];
   	char sqlbuf[1024];
   	long lFlag,lKid;
   	int  iNum;
  
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64];
    long lId,lSid,lSum;
    char caPlate[128];
    char caId[16];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
    char *pHash;
    char caFlag[16];
   ncsKeywordStat *psData,*pOutdata;
   typedef struct ncsKey_s{
   	  long id;
   	  char caKeyword[64];
   	} ncsKey;
   	ncsKey stKey;
   	pasLHashInfo sHashInfo;
    char caUsername2[36];
    char caGroups[2048];
    strcpy(caGroups,getDsGroupid());
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,2, "id",           UT_TYPE_STRING,10,caId,
	                                          "flag",         UT_TYPE_STRING,10,caFlag);
	 memset(caUsername2,0,sizeof(caUsername2));         
   dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
 
    iNum = 0;
    lSum=0;
     pHash=(unsigned char *)pasLHashInit(10000,10000,sizeof(struct ncsKeywordStat_s),0,68); 
    	if(pHash==NULL){
    		return -1;
    	}
    pOutdata=(struct ncsKeywordStat_s*)malloc(10000*sizeof(struct ncsKeywordStat_s));
    if(pOutdata==NULL){
    	return -1;
    }
    
   
        iNum=0;
        if(strlen(caId)==0){
        	sprintf(caTemp,"select name,id,count(*),flag from ncssennetidlog,ncuseridclass where kid=id and flag!=9 ");
        	if(strlen(caFlag)>0){
        		sprintf(caTemp+strlen(caTemp)," and flag=%s ",caFlag);
        	}
        	if(strlen(caGroups)>0){
    				sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
          }
        	sprintf(caTemp+strlen(caTemp)," group by id,flag limit 0,10000 ");
        
        }
        else{
        	sprintf(caTemp,"select keyword,kid,count(*),flag from ncssennetidlog,ncuseridclass where kid=id and flag!=9 and kid=%s ",caId);
          if(strlen(caFlag)>0){
          	sprintf(caTemp+strlen(caTemp)," and flag=%s ",caFlag);
          }
          if(strlen(caGroups)>0){
    				sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
          }
          sprintf(caTemp+strlen(caTemp)," group by keyword,kid,flag limit 0,20000 ");
        }
        printf("caTemp=%s\n",caTemp);
        psCur=pasDbOpenSqlF(caTemp);
         if(psCur){
         	          memset(caName,0,sizeof(caName));
         	          lId=0;
         	          lCount=0;
         	          lFlag=0;
         	    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,63,caName,
         	    	                                 UT_TYPE_LONG,4,&lId,
         	    	                                 UT_TYPE_LONG,4,&lCount,
         	    	                                 UT_TYPE_LONG,4,&lFlag);
                         while(iReturn==0||iReturn==1405)
										     {
										        memset(&stKey,0,sizeof(ncsKey));
										        stKey.id=lId;
										        strcpy(stKey.caKeyword,caName);
										     	  psData = (struct ncsKeywordStat_s *)pasLHashLookA(pHash,&stKey);  
										     	  if(psData){
										     	  	if(lFlag>0){
										     	  		psData->csum=psData->csum+lCount;
										     	  	}
										     	  	psData->sum=psData->sum+lCount;
										     	  }
										     	  
										     	  
				         	          memset(caName,0,sizeof(caName));
				         	          lId=0;
				         	          lCount=0;
				         	          lFlag=0;
				         	    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,63,caName,
				         	    	                                 UT_TYPE_LONG,4,&lId,
				         	    	                                 UT_TYPE_LONG,4,&lCount,
				         	    	                                 UT_TYPE_LONG,4,&lFlag);
										     }
										     pasDbCloseCursor(psCur); 
     	
         	               psData=(struct ncsKeywordStat_s *)pasLHashFirst(pHash,&sHashInfo);
         	               iNum=0;
         	               while(psData){
         	               	    pOutdata[iNum].id=psData->id;
         	               	    strcpy(pOutdata[iNum].caKeyword,psData->caKeyword);
         	               	    pOutdata[iNum].sum=psData->sum;
         	               	    pOutdata[iNum].csum=psData->csum;
         	               	    
         	               	    
         	               	    iNum++;
    	                        psData=(struct ncsKeywordStat_s *)pasLHashNext(&sHashInfo);
         	               }
         	               lSum=iNum;
  //       	               printf("lSum=%d\n",lSum);
  //       	               排序
         	         qsort(pOutdata,lSum,sizeof(struct ncsKeywordStat_s),ncsUtlSortKeyword);  
           }
        psDbHead = utPltInitDb();
         //输出
        for(i=0;i<lSum;i++){
         	if(i>500) break;
         	
         	if(i>0){
			       utPltPutLoopVar(psDbHead,"dh",i+1,",");
			    }
         	utPltSetCvtHtml(1);
         	if(strlen(caFlag)>0){
         		utPltPutLoopVarF(psDbHead,"groupname",i+1,"%s_(%d)",pOutdata[i].caKeyword,pOutdata[i].sum);
         	}
         	else{
          	utPltPutLoopVarF(psDbHead,"groupname",i+1,"%s_(%d/%d)",pOutdata[i].caKeyword,pOutdata[i].sum-pOutdata[i].csum,pOutdata[i].sum);
          }
  //   printf("i=%d,sum=%d,id=%d\n",i,pOutdata[i].sum,pOutdata[i].id);
         	utPltSetCvtHtml(0);
         	if(strlen(caId)==0){
         				utPltPutLoopVarF(psDbHead,"groupid",   i+1,"%ld",pOutdata[i].id);
         				utPltPutLoopVar(psDbHead,"leaf", i+1,"false");
		         		utPltPutLoopVar(psDbHead,"cls", i+1,"folder");
         	}
         	else{
         		    utPltPutLoopVarF(psDbHead,"did",   i+1,"%s",pOutdata[i].caKeyword);
         				utPltPutLoopVar(psDbHead,"leaf", i+1,"true");
		         		utPltPutLoopVar(psDbHead,"cls", i+1,"file");
         	}
         	
        }
        
    
       free(pHash);
       free(pOutdata);

		

    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");

    

    return 0;
}



//重点用户日志树
int ncsTreeSenuser_log_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[4024];
   	char sqlbuf[1024];
   	long lFlag,lKid;
   	int  iNum;
  
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64];
    long lId,lSid,lSum;
    char caPlate[128];
    char caId[16];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
    char *pHash;
    char caFlag[16];
   ncsKeywordStat *psData,*pOutdata;
   typedef struct ncsKey_s{
   	  long id;
   	  char caKeyword[64];
   	} ncsKey;
   	ncsKey stKey;
   	pasLHashInfo sHashInfo;
    char caUsername2[36];
    char caGroups[2048];
    strcpy(caGroups,getDsGroupid());
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,2, "id",           UT_TYPE_STRING,10,caId,
	                                          "flag",         UT_TYPE_STRING,10,caFlag);
	 memset(caUsername2,0,sizeof(caUsername2));         
   dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
 
    iNum = 0;
    lSum=0;
     pHash=(unsigned char *)pasLHashInit(10000,10000,sizeof(struct ncsKeywordStat_s),0,68); 
    	if(pHash==NULL){
    		return -1;
    	}
    pOutdata=(struct ncsKeywordStat_s*)malloc(10000*sizeof(struct ncsKeywordStat_s));
    if(pOutdata==NULL){
    	return -1;
    }
    
   
        iNum=0;

        	sprintf(caTemp,"select keyword,0,count(*),flag from ncssenuserlog where  flag!=9  ");
          if(strlen(caFlag)>0){
          	sprintf(caTemp+strlen(caTemp)," and flag=%s ",caFlag);
          }
          if(strlen(caGroups)>0){
    				sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
          }
          
          sprintf(caTemp+strlen(caTemp)," group by keyword,flag limit 0,20000 ");
     
        printf("caTemp=%s\n",caTemp);
        psCur=pasDbOpenSqlF(caTemp);
         if(psCur){
         	          memset(caName,0,sizeof(caName));
         	          lId=0;
         	          lCount=0;
         	          lFlag=0;
         	    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,63,caName,
         	    	                                 UT_TYPE_LONG,4,&lId,
         	    	                                 UT_TYPE_LONG,4,&lCount,
         	    	                                 UT_TYPE_LONG,4,&lFlag);
                         while(iReturn==0||iReturn==1405)
										     {
										        memset(&stKey,0,sizeof(ncsKey));
										        stKey.id=lId;
										        strcpy(stKey.caKeyword,caName);
										     	  psData = (struct ncsKeywordStat_s *)pasLHashLookA(pHash,&stKey);  
										     	  if(psData){
										     	  	if(lFlag>0){
										     	  		psData->csum=psData->csum+lCount;
										     	  	}
										     	  	psData->sum=psData->sum+lCount;
										     	  }
										     	  
										     	  
				         	          memset(caName,0,sizeof(caName));
				         	          lId=0;
				         	          lCount=0;
				         	          lFlag=0;
				         	    	    iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,63,caName,
				         	    	                                 UT_TYPE_LONG,4,&lId,
				         	    	                                 UT_TYPE_LONG,4,&lCount,
				         	    	                                 UT_TYPE_LONG,4,&lFlag);
										     }
										     pasDbCloseCursor(psCur); 
     	
         	               psData=(struct ncsKeywordStat_s *)pasLHashFirst(pHash,&sHashInfo);
         	               iNum=0;
         	               while(psData){
         	               	    pOutdata[iNum].id=psData->id;
         	               	    strcpy(pOutdata[iNum].caKeyword,psData->caKeyword);
         	               	    pOutdata[iNum].sum=psData->sum;
         	               	    pOutdata[iNum].csum=psData->csum;
         	               	    
         	               	    
         	               	    iNum++;
    	                        psData=(struct ncsKeywordStat_s *)pasLHashNext(&sHashInfo);
         	               }
         	               lSum=iNum;
  //       	               printf("lSum=%d\n",lSum);
  //       	               排序
         	         qsort(pOutdata,lSum,sizeof(struct ncsKeywordStat_s),ncsUtlSortKeyword);  
           }
        psDbHead = utPltInitDb();
         //输出
        for(i=0;i<lSum;i++){
         	if(i>500) break;
         	
         	if(i>0){
			       utPltPutLoopVar(psDbHead,"dh",i+1,",");
			    }
         	utPltSetCvtHtml(1);
         	if(strlen(caFlag)>0){
         		utPltPutLoopVarF(psDbHead,"groupname",i+1,"%s_(%d)",pOutdata[i].caKeyword,pOutdata[i].sum);
         	}
         	else{
          	utPltPutLoopVarF(psDbHead,"groupname",i+1,"%s_(%d/%d)",pOutdata[i].caKeyword,pOutdata[i].sum-pOutdata[i].csum,pOutdata[i].sum);
          }
  //   printf("i=%d,sum=%d,id=%d\n",i,pOutdata[i].sum,pOutdata[i].id);
         	utPltSetCvtHtml(0);

         		    utPltPutLoopVarF(psDbHead,"did",   i+1,"%s",pOutdata[i].caKeyword);
         				utPltPutLoopVar(psDbHead,"leaf", i+1,"true");
		         		utPltPutLoopVar(psDbHead,"cls", i+1,"file");
         	
         	
        }
        
    
       free(pHash);
       free(pOutdata);

		

    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");

    

    return 0;
}

//显示我的工具条

int ncsDispTool_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[1024];
   	int  iNum;
    char caFun[2048];
    long lLevel;
   	int iReturn,i,m;   
    long lCount;
    char caName[64],caSy[64];
    long lId,lSid;
    utPltDbHead *psDbHead;
    char caTool[1024];
    char caRight_all[2048];
    char caTname[36],ckRight[36];
    dsCltGetSessionValue(1,"id",UT_TYPE_LONG,sizeof(long),&lId);
    	
    psDbHead = utPltInitDb();
    utPltPutVarF(psDbHead,"id","%d",lId);
    utMsgPrintMsg(psMsgHead);
    memset(caFun,0,sizeof(caFun));
    memset(caRight_all,0,sizeof(caRight_all));
sprintf(caTemp,"select tname from dstoolsy_v4 where id=%d ",lId);
memset(caSy,0,sizeof(caSy));
pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caSy);
if(strlen(caSy)==0){
    sprintf(caTemp,"select usrlevel from dsuser where id=%d ",lId);
    pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lLevel);
    strcpy(caSy,ncsGetFirstNameById(lLevel));
  }
    sprintf(caTemp,"select tname from dstool_v4 where id=%d ",lId);
    psCur=pasDbOpenSqlF(caTemp);
    memset(caTool,0,sizeof(caTool));
    if(psCur){
    	memset(caName,0,sizeof(caName));
    	iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName);
    	while(iReturn==0||iReturn==1405){
    		
    		sprintf(caTool+strlen(caTool),",%s",caName);
    		memset(caName,0,sizeof(caName));
    		iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName);
    	}
    	pasDbCloseCursor(psCur);
    }
    
    
    sprintf(caTemp,"select lright from dsuser a,dsuserrole b,dsroleattr c,dscltright d where a.id=b.id and b.roleid=c.roleid and d.name=lright and a.id=%d group by lright",lId);  
    
   
		 psCur = pasDbOpenSqlF(caTemp);
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
	     memset(caName,0,sizeof(caName));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName);
		   iNum=0;	
		   m=0; 
		   i=0;
		    while(iReturn == 0||iReturn==1405) {
		    	  sprintf(caRight_all+strlen(caRight_all),"%s,",caName);
		        utPltPutLoopVar(psDbHead,"right",iNum+1,caName);
		        utPltPutLoopVarF(psDbHead,"num",iNum+1,"%d",i);
		        if(strlen(caTool)==0){
		        	utPltPutLoopVar(psDbHead,"ckright",iNum+1,"checked");
		        }
		        else if(strstr(caTool,caName)){
		        	utPltPutLoopVar(psDbHead,"ckright",iNum+1,"checked");
		        }
             i++;
	/*	        if(strcmp(caName,"敏感规则")==0){
	
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\'>%s",i,"敏感规则","敏感规则");
		        	utPltPutLoopVar2(psDbHead,"content1",iNum+1,1,caTemp);
		        	
		        	i++;
		        }
		        else if(strcmp(caName,"敏感监控")==0){
		 
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\'>%s",i,"敏感监控","敏感监控");
		        	utPltPutLoopVar2(psDbHead,"content1",iNum+1,1,caTemp);
		        
		        	i++;
		        }
		*/
		        if(strcmp(caName,"身份库管理")==0){
		            if(strstr(caTool,"身份库管理")){
		            	strcpy(ckRight,"checked");
		            }
		            else{
		            	strcpy(ckRight,"");
		            }
		         	sprintf(caRight_all+strlen(caRight_all),"%s,","上网用户库");   
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\' %s>%s",i,"上网用户库",ckRight,"上网用户库");
		        	utPltPutLoopVar2(psDbHead,"content1",iNum+1,1,caTemp);
		        	i++;
		        	sprintf(caRight_all+strlen(caRight_all),"%s,","MAC地址库"); 
		        	 if(strstr(caTool,"MAC地址库")){
		            	strcpy(ckRight,"checked");
		            }
		            else{
		            	strcpy(ckRight,"");
		            }
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\' %s>%s",i,"MAC地址库",ckRight,"MAC地址库");
		        	utPltPutLoopVar2(psDbHead,"content2",iNum+1,1,caTemp);
		        	i++;
		        	sprintf(caRight_all+strlen(caRight_all),"%s,","虚拟身份库");
		        		if(strstr(caTool,"虚拟身份库")){
		            	strcpy(ckRight,"checked");
		            }
		            else{
		            	strcpy(ckRight,"");
		            }
		          sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\' %s>%s",i,"虚拟身份库",ckRight,"虚拟身份库");
		        	utPltPutLoopVar2(psDbHead,"content3",iNum+1,1,caTemp);
		        	i++;
		        	
		        	
		        	
		        	
		        }
		        else if(strcmp(caName,"权限管理")==0){
		        		if(strstr(caTool,"权限角色")){
		            	strcpy(ckRight,"checked");
		            }
		            else{
		            	strcpy(ckRight,"");
		            }
		       		sprintf(caRight_all+strlen(caRight_all),"%s,","权限角色"); 	
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\' %s>%s",i,"权限角色",ckRight,"权限角色");
		        	utPltPutLoopVar2(psDbHead,"content1",iNum+1,1,caTemp);
		        	i++;
		        	
		        		if(strstr(caTool,"系统用户")){
		            	strcpy(ckRight,"checked");
		            }
		            else{
		            	strcpy(ckRight,"");
		            }
		          sprintf(caRight_all+strlen(caRight_all),"%s,","系统用户"); 
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\' %s>%s",i,"系统用户",ckRight,"系统用户");
		        	utPltPutLoopVar2(psDbHead,"content2",iNum+1,1,caTemp);
		        	i++;
		        	
		        	 if(strstr(caTool,"系统操作日志")){
		            	strcpy(ckRight,"checked");
		            }
		            else{
		            	strcpy(ckRight,"");
		            }
		          sprintf(caRight_all+strlen(caRight_all),"%s,","系统操作日志");  
		            
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\' %s>%s",i,"系统操作日志",ckRight,"系统操作日志");
		        	utPltPutLoopVar2(psDbHead,"content3",iNum+1,1,caTemp);
		        	i++;
		        
		        	
		        }
		        	else if(strcmp(caName,"统计报表")==0){
		        		
		        		if(strstr(caTool,"统计报表")){
		            	strcpy(ckRight,"checked");
		            }
		            else{
		            	strcpy(ckRight,"");
		            }
		           sprintf(caRight_all+strlen(caRight_all),"%s,","当前在线率");  
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\' %s>%s",i,"当前在线率",ckRight,"当前在线率");
		        	utPltPutLoopVar2(psDbHead,"content1",iNum+1,1,caTemp);
		        	i++;
		        		if(strstr(caTool,"综合统计")){
		            	strcpy(ckRight,"checked");
		            }
		            else{
		            	strcpy(ckRight,"");
		            }
		          sprintf(caRight_all+strlen(caRight_all),"%s,","综合统计");  
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\' %s>%s",i,"综合统计",ckRight,"综合统计");
		        	utPltPutLoopVar2(psDbHead,"content2",iNum+1,1,caTemp);
		        	i++;
		        
		        	
		        }
	/*	        else if(strcmp(caName,"运行分析")==0){
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\'>%s",i,"运行分析","运行分析");
		        	utPltPutLoopVar2(psDbHead,"content1",iNum+1,1,caTemp);
		        	i++;
		

		        	
		        }
		        else if(strcmp(caName,"布控管理")==0){
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\'>%s",i,"布控管理","布控管理");
		        	utPltPutLoopVar2(psDbHead,"content1",iNum+1,1,caTemp);
		        	i++;
		
		        	
		        }
		*/
		        else if(strcmp(caName,"单位管理")==0){
		        		if(strstr(caTool,"单位管理")){
		            	strcpy(ckRight,"checked");
		            }
		            else{
		            	strcpy(ckRight,"");
		            }
		        	
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\' %s>%s",i,"单位管理",ckRight,"单位管理");
		        	utPltPutLoopVar2(psDbHead,"content1",iNum+1,1,caTemp);
		        	i++;
		        	
		        		if(strstr(caTool,"在线单位")){
		            	strcpy(ckRight,"checked");
		            }
		            else{
		            	strcpy(ckRight,"");
		            }
		          sprintf(caRight_all+strlen(caRight_all),"%s,","在线单位");  
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\' %s>%s",i,"在线单位",ckRight,"在线单位");
		        	utPltPutLoopVar2(psDbHead,"content2",iNum+1,1,caTemp);
		        	i++;
		        	 if(strstr(caTool,"单位组")){
		            	strcpy(ckRight,"checked");
		            }
		            else{
		            	strcpy(ckRight,"");
		            }
		          sprintf(caRight_all+strlen(caRight_all),"%s,","单位组");   
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\' %s>%s",i,"单位组",ckRight,"单位组");
		        	utPltPutLoopVar2(psDbHead,"content3",iNum+1,1,caTemp);
		        	i++;
		        	
		        	 if(strstr(caTool,"接入厂商代码维护")){
		            	strcpy(ckRight,"checked");
		            }
		            else{
		            	strcpy(ckRight,"");
		            }
		         	sprintf(caRight_all+strlen(caRight_all),"%s,","接入厂商代码维护");   
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\' %s>%s",i,"接入厂商代码维护",ckRight,"接入厂商代码维护");
		        	utPltPutLoopVar2(psDbHead,"content4",iNum+1,1,caTemp);
		        	i++;

		        	
		        }
	/*	        else if(strcmp(caName,"单位组管理")==0){
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\'>%s",i,"单位组管理","单位组管理");
		        	utPltPutLoopVar2(psDbHead,"content1",iNum+1,1,caTemp);
		        	i++;
		
		        
		      
		        	
		        }
		        else if(strcmp(caName,"导航树")==0){
		    		  sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\'>%s",i,"导航树","导航树");
		        	utPltPutLoopVar2(psDbHead,"content1",iNum+1,1,caTemp);
		        
		        
		        	
		        }
		        else if(strcmp(caName,"告警信息")==0){
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\'>%s",i,"告警信息","告警信息");
		        	utPltPutLoopVar2(psDbHead,"content1",iNum+1,1,caTemp);
		        	i++;
		
		        
		        
		        	
		        }
		        else if(strcmp(caName,"归档日志")==0){
		        	sprintf(caTemp,"<input type=\'checkbox\' name=\'ch%d\' value=\'%s\'>%s",i,"归档日志","归档日志");
		        	utPltPutLoopVar2(psDbHead,"content1",iNum+1,1,caTemp);
		        	i++;
		
		        
		     
		        	
		        }
		  */      
		        iNum++;
		         
		        
				     memset(caName,0,sizeof(caName));
					   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName);
		    }
		    pasDbCloseCursor(psCur);
		
 //   printf("caRight_all=%s\n",caRight_all);
    iNum=0;
    if(strstr(caRight_all,"单位管理")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"单位管理");
    	iNum++;
    }	
    if(strstr(caRight_all,"单位组")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"单位组");
    	iNum++;
    }		
    if(strstr(caRight_all,"归档日志")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"归档日志");
    	iNum++;
    }	
    if(strstr(caRight_all,"在线单位")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"在线单位");
    	iNum++;
    }	
    if(strstr(caRight_all,"在线率统计")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"在线率统计");
    	iNum++;
    }		
    if(strstr(caRight_all,"告警信息")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"告警信息");
    	iNum++;
    }		
    if(strstr(caRight_all,"布控管理")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"布控管理");
    	iNum++;
    }		
    if(strstr(caRight_all,"权限管理")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"权限管理");
    	iNum++;
    }	
    if(strstr(caRight_all,"综合统计")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"综合统计");
    	iNum++;
    }	
    if(strstr(caRight_all,"系统操作日志")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"系统操作日志");
    	iNum++;
    }	
    if(strstr(caRight_all,"敏感监控")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"敏感监控");
    	iNum++;
    }	
    if(strstr(caRight_all,"敏感规则")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"敏感规则");
    	iNum++;
    }	
    if(strstr(caRight_all,"虚拟身份库")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"虚拟身份库");
    	iNum++;
    }	
    if(strstr(caRight_all,"运行分析")){
    	utPltPutLoopVar(psDbHead,"osy",iNum+1,"运行分析");
    	iNum++;
    }	
// utPltShowDb(psDbHead);
    utPltPutVar(psDbHead,"sy",caSy);
  //  printf("caSy=%s\n",caSy);
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_mytool.htm");
    

    return 0;
}



int ncsDispToolSave_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	int i;
	char caVar[32];
	char caFun[32];
	char caTemp[1024];
	int iReturn,lCount;
	char caSy[36];
	long lId;
	dsCltGetSessionValue(1,"id",UT_TYPE_LONG,sizeof(long),&lId);
	if(lId>0){
		pasDbExecSqlF("delete from dstool_v4 where id=%d ",lId);
		utMsgGetSomeNVar(psMsgHead,1,
		   "sy",UT_TYPE_STRING,30,caSy);
		   sprintf(caTemp,"select count(*) from dstoolsy_v4 where id=%d ",lId);
		   
		   lCount=0;
		   pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
		   if(lCount>0){
		   	pasDbExecSqlF("update dstoolsy_v4 set tname='%s' where id=%d ",caSy,lId);
		  }
		  else{
		  	pasDbExecSqlF("insert into dstoolsy_v4(tname,id) values('%s',%d)",caSy,lId);
		  }
		
	for(i=0;i<50;i++){
		sprintf(caVar,"ch%d",i);
		iReturn=utMsgGetSomeNVar(psMsgHead,1,
		   caVar,UT_TYPE_STRING,30,caFun);
		   if(iReturn==1&&strlen(caFun)>2){
		   	sprintf(caTemp,"select count(*) from dstool_v4 where id=%d and tname='%s' ",lId,caFun);
		   	lCount=0;
		   	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
		   	if(lCount==0){
		   		pasDbExecSqlF("insert into dstool_v4(tname,id) values('%s',%d)",caFun,lId);
		   }
		   
	//	   printf("iReturn=%d,fun=%s\n",iReturn,caFun);
		  }
	 }
	}
	ncsDispTool_v4(psShmHead,iFd,psMsgHead);
}

//显示关键字下发策略
int ncsKeywordXf_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
	  char caName[64],caNetname[32],caTemp_d[128],caStatus[16];
	  long iNum;
	  struct fac_s {
    	char caCod[12];
    	char caName[64];
    };
 		char *pHash_lb;
 		struct fac_s *psFac;
 		char *pHash;
 		struct data_s{
 			long id;
 			char caName[64];
 		};
 		struct data_s *psData;
 		
	  long id,ctype,did,i,mCount,num;
	  char caObj[100][50];
	  char caSeid[1024];
	  char caDx[9000];
	  char caCode[16];
	  long kid,lValidtime,lStime,lId;
	  long caKid[1024],caSdate[16];
	  char caIds[4096],caOpname[16],caType[16],caId[16],caDispname[128];
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,12,
	                 "kid",            UT_TYPE_STRING, 1000, caKid,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "vtime",       UT_TYPE_STRING,12,caSdate,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "seid",UT_TYPE_STRING,1000,caSeid,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel); 
	memset(caDispname,0,sizeof(caDispname)); 
	iReturn=dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caDispname);
	
	
	    pHash_lb = (unsigned char *)pasLHashInit(30,30,sizeof(struct fac_s),0,12);    
    if(pHash_lb == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
       //将单位类别装入到内存   
     sprintf(caTemp,"select code,name from ncsuser_lb limit 0,29 "); 
    psCur = pasDbOpenSql(caTemp,0);

    if(psCur){
    memset(caCode,0,sizeof(caCode));
    memset(caName,0,sizeof(caName));
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_STRING,10,caCode,
      	                             UT_TYPE_STRING,63,caName);
      	                            
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
   		 psFac = (struct fac_s *)pasLHashLookA(pHash_lb,caCode);  
       if(psFac){
    		  strcpy(psFac->caName,caName);
       }	
		    memset(caCode,0,sizeof(caCode));
		    memset(caName,0,sizeof(caName));
		  	iReturn = pasDbFetchInto(psCur,
		      	                             UT_TYPE_STRING,10,caCode,
		      	                             UT_TYPE_STRING,63,caName);                              
    }    
    pasDbCloseCursor(psCur); 
   
  }
	     
	 pHash = (unsigned char *)pasLHashInit(1500,1500,sizeof(struct data_s),0,4);    
    if(pHash == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    } 
	     
	     
	     
	  //将关键字组装入到内存   
     sprintf(caTemp,"select id,name from ncskeyindex order by lasttime desc limit 0,500 "); 
    psCur = pasDbOpenSql(caTemp,0);

    if(psCur){
     lId=0;
     memset(caName,0,sizeof(caName));
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lId,
      	                             UT_TYPE_STRING,31,caName);
      	                            
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
   		 psData = (struct data_s *)pasLHashLookA(pHash,&lId);  
       if(psData){
    		  strcpy(psData->caName,caName);
       }	
		     lId=0;
		     memset(caName,0,sizeof(caName));
		  	iReturn = pasDbFetchInto(psCur,
		      	                             UT_TYPE_LONG,4,&lId,
		      	                             UT_TYPE_STRING,31,caName);                              
    }    
    pasDbCloseCursor(psCur); 
   
  }   
	     
	     
	            

	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caUpdate,"update")==0){
	  	char caCtype[16],caDid[16];
	  	if(strlen(caKid)>0){
printf("caKid=%s\n",caKid);
	  		 num=ncs_SepChar0(caKid,',',caObj); 
	  		 printf("num=%d\n",num);
	  		 for(i=0;i<num;i++){
		  		sprintf(caTemp,"select count(*) from ncskeyword_procy where kid=%d ",atol(caObj[i]));
		  		printf("caTemp=%s\n",caTemp);
		  		lCount=0;
		  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
		  		if(lCount>0){
		  			pasDbExecSqlF("update ncskeyword_procy set ids='%s',validtime=%d,stime=%d,opname='%s' where kid=%d",caSeid,utTimStrToLong("%Y/%m/%d", caSdate),time(0),caDispname,atol(caObj[i])); 
		  			printf("update ncskeyword_procy set ids='%s',validtime=%d,stime=%d,opname='%s' where kid=%d\n",caSeid,utTimStrToLong("%Y/%m/%d", caSdate),time(0),caDispname,atol(caObj[i]));  
		  		}
		  		else{
		  			printf("insert into ncskeyword_procy(kid,ktype,ids,validtime,stime,addtime,opname) values(%d,0,'%s',%d,%d,%d,'%s')\n",atol(caObj[i]),caSeid,utTimStrToLong("%Y/%m/%d", caSdate),time(0),time(0),caDispname);
		  			pasDbExecSqlF("insert into ncskeyword_procy(kid,ktype,ids,validtime,stime,addtime,opname) values(%d,0,'%s',%d,%d,%d,'%s')",atol(caObj[i]),caSeid,utTimStrToLong("%Y/%m/%d", caSdate),time(0),time(0),caDispname);
		  		}
	  		}
	  	}
	  	
	  	
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("delete from ncskeyword_procy where sid in(%s)",caSelsid);
	  }
	  
	  
	  sprintf(caTemp,"select count(*) from ncskeyword_procy where 1=1 ");
	 
   
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  sprintf(caTemp,"select sid,kid,ids,validtime,stime,opname from ncskeyword_procy where 1=1 ");
	  
	    	
   
    	sprintf(caTemp+strlen(caTemp)," order by sid desc limit %d,%d",lStartRec,lRowNum);
    	
  	printf("caTemp=%s\n",caTemp);
    	

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum=0;
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	
   	 sid=0;
     kid=0;
     memset(caIds,0,sizeof(caIds));
     lValidtime=0;
     lStime=0;
     memset(caOpname,0,sizeof(caOpname));
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&sid,
      	                               UT_TYPE_LONG,4,&kid,
      	                               UT_TYPE_STRING,4000,caIds,
      	                               UT_TYPE_LONG,4,&lValidtime,
      	                               UT_TYPE_LONG,4,&lStime,
      	                               UT_TYPE_STRING,31,caOpname);
      	                             
      	                              
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);

  		   num=ncs_SepChar0(caIds,',',caObj); 
	  		 for(i=0;i<num;i++){
	  		 	utStrGetSomeWordBy(caObj[i],"_",2,UT_TYPE_STRING,10,caType,UT_TYPE_STRING,10,caId);

          if(strcmp(caType,"0")==0){
          	strcpy(caDispname,"全部");
          }
          else if(strcmp(caType,"1")==0){                //单位类别
          	   memset(caCode,0,sizeof(caCode));
               strcpy(caCode,caId);
          	  psFac=(struct fax_s *)pasLHashLook(pHash_lb,caCode);
              if(psFac){
                strcpy(caDispname,psFac->caName);
              }
              else{
              	strcpy(caDispname,"");
              }
          }
          else if(strcmp(caType,"2")==0){                //单位
          	strcpy(caDispname,ncsUtlGetClientNameById(psShmHead,atol(caId),"未知"));
	        }
	        else{
	        	strcpy(caDispname,"");
	        }
	        if(strlen(caDispname)>0){
	        	if(i==0){
	        		strcpy(caDx,caDispname);
	        	}
	        	else{
	        		sprintf(caDx+strlen(caDx),",%s",caDispname);
	        	}
	        }
       }

        utPltPutLoopVar(psDbHead,"dx",iNum,caDx);
        utPltPutLoopVar(psDbHead,"stime",iNum,utTimFormat("%Y/%m/%d",lStime));
        utPltPutLoopVar(psDbHead,"vtime",iNum,utTimFormat("%Y/%m/%d",lValidtime));
        utPltPutLoopVar(psDbHead,"opname",iNum,caOpname);
        
               psData=(struct data_s *)pasLHashLook(pHash,&kid);
              if(psData){
                 utPltPutLoopVar(psDbHead,"keyword",iNum,psData->caName);
              }
           
       

			
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  free(pHash);
	   free(pHash_lb);
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_keywordxf_list.htm");
	  return 0;
}






//关键字树-身份串并
int ncsTreeUserKeyword_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
   	char caTemp[4024];
   	char sqlbuf[1024];
   	long lFlag,lKid;
   	int  iNum; 
    utPltDbHead *psDbHead;
   	int iReturn,i;   
    long lCount;
    char caName[64];
    long lId,lSid,lSum;
    char caPlate[128];
    char caId[16];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
    char *pHash;
    char caFlag[16];
    long lDid,lUid;
    char caDispname[32];
    char caUsername2[36];
    char caGroups[2048];
  
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,2, "id",           UT_TYPE_STRING,10,caId,
	                                          "flag",         UT_TYPE_STRING,10,caFlag);
	 memset(caUsername2,0,sizeof(caUsername2));         
   dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);

    psDbHead = utPltInitDb();
   
       sprintf(caTemp,"select count(*) aa,did,uid,dispname from ncskeywordlog group by did,uid,dispname order by aa desc limit 0,400 ");  
        iNum=0;
        printf("caTemp=%s\n",caTemp);
        psCur=pasDbOpenSqlF(caTemp);
         if(psCur){         	         
         	          lCount=0;
         	          lDid=0;
         	          lUid=0;
         	          memset(caDispname,0,sizeof(caDispname));
         	    	    iReturn=pasDbFetchInto(psCur,
         	    	                                 UT_TYPE_LONG,4,&lCount,
         	    	                                 UT_TYPE_LONG,4,&lDid,
         	    	                                 UT_TYPE_LONG,4,&lUid,
         	    	                                 UT_TYPE_STRING,30,caDispname);

                         while(iReturn==0||iReturn==1405)
										     {
										       if(iNum>0){
										       	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
										      }
										      utPltPutLoopVar(psDbHead,"leaf", iNum+1,"true");
		         		          utPltPutLoopVar(psDbHead,"cls", iNum+1,"file");
										      utPltPutLoopVarF(psDbHead,"count",iNum+1,"%d",lCount);
										      utPltPutLoopVarF(psDbHead,"did",iNum+1,"%d",lDid);
										      utPltPutLoopVarF(psDbHead,"uid",iNum+1,"%d",lUid);
										      utPltSetCvtHtml(1);
										     	 utPltPutLoopVar(psDbHead,"dispname",iNum+1,caDispname);  
										     	utPltSetCvtHtml(0);  
										     	iNum++;
				         	          lCount=0;
				         	          lDid=0;
				         	          lUid=0;
				         	          memset(caDispname,0,sizeof(caDispname));
         	    	    iReturn=pasDbFetchInto(psCur,
         	    	                                 UT_TYPE_LONG,4,&lCount,
         	    	                                 UT_TYPE_LONG,4,&lDid,
         	    	                                 UT_TYPE_LONG,4,&lUid,
         	    	                                 UT_TYPE_STRING,30,caDispname);
										     }
										     pasDbCloseCursor(psCur); 
     	
//       printf("aaaaaa\n");
      }
 
		

    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userkeyword_tree.htm");

    

    return 0;
}



//显示key维护信息
int ncskid_user_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 pasDbCursor *psCur;
	  utPltDbHead *psDbHead;
	  long iReturn,lCount;
	  char caStart[16],caTemp[2048],caCurPg[16];
	  long lCurPg,lRowNum,lStartRec,lSid,lFlag;
	  char caKid[32],caDispname[32],caDwname[120],caMobile[20],caDemo[200],caTemp_d[128],caStatus[16];
	  long iNum;
	  char caFob[16];
	 char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caKeyword[64],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
	  utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,17,
			             "limit",            UT_TYPE_STRING, 10, caLimit,
			             "page",          UT_TYPE_STRING, 10, caPage,  
			             "del",           UT_TYPE_STRING,8,caDel,
			             "dir",           UT_TYPE_STRING,8,caDir,
			             "keyword",       UT_TYPE_STRING,30,caKeyword,
			             "selsid",UT_TYPE_STRING,1000,caSelsid,
			             "status",UT_TYPE_STRING,4,caStatus,
			             "update",UT_TYPE_STRING,10,caUpdate,
			             "kid",UT_TYPE_STRING,30,caKid,
			             "dispname",UT_TYPE_STRING,30,caDispname,
			             "dwname",UT_TYPE_STRING,80,caDwname,
			             "mobile",UT_TYPE_STRING,15,caMobile,
			             "demo",UT_TYPE_STRING,128,caDemo,
			             "sid",UT_TYPE_STRING,10,caSid,
			             "start",UT_TYPE_STRING,10,caStart,
			             "del",UT_TYPE_STRING,10,caDel,
			             "fob",UT_TYPE_STRING,10,caFob);  
	  if(strlen(caDispname)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caDispname,caTemp,30)); 
	  	strcpy(caDispname,caTemp_d);
	  }
	  	  if(strlen(caKid)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caKid,caTemp,24)); 
	  	strcpy(caKid,caTemp_d);
	  }
	  
	  if(strlen(caKeyword)>0){
	  	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,63));
	  	strcpy(caKeyword,caTemp_d);
	  }          
		if(strlen(caDwname)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caDwname,caTemp,80)); 
	  	strcpy(caDwname,caTemp_d);
	  }	              
		if(strlen(caDemo)>0){
	  	strcpy(caTemp_d,  pasCvtGBK(2,caDemo,caTemp,120)); 
	  	strcpy(caDemo,caTemp_d);
	  }	 


	  lRowNum=atol(caLimit);
	  if(lCurPg<=0) lCurPg=1;
	  lStartRec = atol(caStart);
	  
	  psDbHead = utPltInitDb(); 
	  
	  if(strcmp(caUpdate,"update")==0){
	  	if(strlen(caSid)==0){
	  		sprintf(caTemp,"select count(*) from ncsusbkeyuser where kid='%s' ",caKid);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("update ncsusbkeyuser set dispname='%s',dwname='%s',mobile='%s',demo='%' where kid='%s' ",caDispname,caDwname,caMobile,caDemo,caKid);
	  		}
	  		else{
	  			 pasDbExecSqlF("insert into ncsusbkeyuser(kid,dispname,dwname,mobile,demo,flag) \
	       		values('%s','%s','%s','%s','%s',0)",caKid,caDispname,caDwname,caMobile,caDemo);
	  		}
	  	}
	  	else{
	  		sprintf(caTemp,"select count(*) from ncsusbkeyuser where kid='%s' and sid!=%s ",caKid,caSid);
	  		printf("caTemp=%s\n",caTemp);
	  		lCount=0;
	  		pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  		if(lCount>0){
	  			pasDbExecSqlF("delete from ncsusbkeyuser where kid='%s' and sid!=%s  ",caKid,caSid);
	  			pasDbExecSqlF("update ncsusbkeyuser set kid='%s',dispname='%s',dwname='%s',mobile='%s',demo='%' where sid=%s ",caKid,caDispname,caDwname,caMobile,caDemo,caSid);
	  		}
	  		else{
	  			printf("update ncsusbkeyuser set kid='%s',dispname='%s',dwname='%s',mobile='%s',demo='%s' where sid=%s \n",caKid,caDispname,caDwname,caMobile,caDemo,caSid);
	  				pasDbExecSqlF("update ncsusbkeyuser set kid='%s',dispname='%s',dwname='%s',mobile='%s',demo='%s' where sid=%s ",caKid,caDispname,caDwname,caMobile,caDemo,caSid);
	  		}
	  	}
	  	
	  }
	  if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("delete from ncsusbkeyuser where sid in (%s)",caSelsid);
	  }
	  if(strcmp(caFob,"fob")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("update ncsusbkeyuser set flag=1 where sid in (%s)",caSelsid);
	  }
	  
	  if(strcmp(caFob,"permit")==0&&strlen(caSelsid)>0){
	  	pasDbExecSqlF("update ncsusbkeyuser set flag=0 where sid in (%s)",caSelsid);
	  }
	  
	  sprintf(caTemp,"select count(*) from ncsusbkeyuser where 1=1 ");
   
    if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and kid like '%c%s%c' or dispname like '%c%s%c' or dwname like '%c%s%c' or mobile like '%c%s%c' or demo like '%c%s%c' ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    }
    lCount=0;
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	  
	  
	    sprintf(caTemp,"select sid,kid,dispname,dwname,mobile,demo,flag from ncsusbkeyuser  where 1=1 ");
     if(strlen(caKeyword)>0){
    	sprintf(caTemp+strlen(caTemp)," and kid like '%c%s%c' or dispname like '%c%s%c' or dwname like '%c%s%c' or mobile like '%c%s%c' or demo like '%c%s%c' ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
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
   	while((iReturn == 0 ||iReturn==1405 )&&iNum<lRowNum) {
   	 lSid=0;
   	 memset(caKid,0,sizeof(caKid));
   	 memset(caDispname,0,sizeof(caDispname));
   	 memset(caDwname,0,sizeof(caDwname));
   	 memset(caMobile,0,sizeof(caMobile));
   	 memset(caDemo,0,sizeof(caDemo));
   	 lFlag=0;
   	 
   	 iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_STRING,20,caKid,
      	                               UT_TYPE_STRING,30,caDispname,
      	                               UT_TYPE_STRING,98,caDwname,
      	                               UT_TYPE_STRING,15,caMobile,
      	                               UT_TYPE_STRING,120,caDemo,
      	                               UT_TYPE_LONG,4,&lFlag);
      	                            
      	                              
      	                                                             
      	                              
      	                              
   	if((iReturn!=0)&&(iReturn!=1405)) break;	
        iNum++;
       if(iNum>1){
       	utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);
        
        utPltPutLoopVar(psDbHead,"kid",iNum,caKid);
        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
        utPltPutLoopVar(psDbHead,"dwname",iNum,caDwname);
        utPltPutLoopVar(psDbHead,"mobile",iNum,caMobile);
        utPltPutLoopVar(psDbHead,"demo",iNum,caDemo);
        if(lFlag==1){
        	utPltPutLoopVar(psDbHead,"flag_desc",iNum,"<font color=red>禁用</font>");
        }
        else{
        	utPltPutLoopVar(psDbHead,"flag_desc",iNum,"<font color=green>启用</font>");
        }
        
      
    }
    pasDbCloseCursor(psCur);
	  

	  utPltPutVarF(psDbHead,"TotRec","%d",lCount);
	  
	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/ncs_case_netid_list.htm");
	  return 0;
}

// 显示usbkey维护日志
int ncsUsbkeyloglist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
  char caKid[36],caDispname[36],caServicecode[20],caDwname[128],caAddress[128],caEndnum[16],caOemname[36];
  char caOemtel[36],caIdno[32],caAqname[32],caAqtel[20],caYhxz[32],caHylb[32],caStar[32],caFws[32];
  char caSwfs[32],caIp[20],caSn[20],caNetflag[20];
  unsigned long lHttpcount,lIpcount,lFormcount,lMailcount,lStime;
  char caTemp_d[68];
  lCurPg=1;
 	strcpy(caWhere,"\0");
	
	iReturn = utMsgGetSomeNVar(psMsgHead,8,
	    "TotPg",      UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      UT_TYPE_STRING, 12, caCurPg,
			"keyword", UT_TYPE_STRING,60,caKeyword,
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

 
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;    	
   
   	
  
  
   	                
   
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
			     sprintf(caEtemptime,"%s 23:59",caEdate_d,caEtime);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	 
   

 
   utStrDelSpaces(caKeyword);           
  
   
	 memset(caWhere,0,sizeof(caWhere));
	 
	 sprintf(caWhere," and stime>=%lu and stime<=%lu ",lstime,letime);
	
	if(strlen(caKeyword)>0){
		sprintf(caWhere+strlen(caWhere)," and (kid like '%c%s%c' or dispname like '%c%s%c' or servicecode like '%c%s%c' or dwname like '%c%s%c' or address like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
	}
	
	sprintf(sqlbuf,"select count(*) from ncsusbkeylog where 1=1 ");
	sprintf(sqlbuf+strlen(sqlbuf)," %s ",caWhere);
	lCount=0;
	pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lCount);
  
	sprintf(sqlbuf,"select sid,kid,dispname,servicecode,dwname,address,endnum,oemname,oemtel,idno,aqname,aqtel,yhxz,hylb,star,fws,swfs,ip,sn,netflag,httpcount,ipcount,formcount,mailcount,stime from ncsusbkeylog where 1=1 ");
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
         memset(caKid,0,sizeof(caKid));
         memset(caDispname,0,sizeof(caDispname));
         memset(caServicecode,0,sizeof(caServicecode));
         memset(caDwname,0,sizeof(caDwname));
         memset(caAddress,0,sizeof(caAddress));
         memset(caEndnum,0,sizeof(caEndnum));
         memset(caOemname,0,sizeof(caOemname));
         memset(caOemtel,0,sizeof(caOemtel));
         memset(caIdno,0,sizeof(caIdno));
         memset(caAqname,0,sizeof(caAqname));
         memset(caAqtel,0,sizeof(caAqtel));
         memset(caYhxz,0,sizeof(caYhxz));
         memset(caHylb,0,sizeof(caHylb));
         memset(caStar,0,sizeof(caStar));
         memset(caFws,0,sizeof(caFws));
         memset(caSwfs,0,sizeof(caSwfs));
         memset(caIp,0,sizeof(caIp));
         memset(caSn,0,sizeof(caSn));
         memset(caNetflag,0,sizeof(caNetflag));
         lHttpcount=0;
         lIpcount=0;
         lFormcount=0;
         lMailcount=0;
         lStime=0;
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lSid,
			                                   UT_TYPE_STRING,30,caKid,
			                                   UT_TYPE_STRING,30,caDispname,
			                                   UT_TYPE_STRING,15,caServicecode,
			                                   UT_TYPE_STRING,99,caDwname,
			                                   UT_TYPE_STRING,127,caAddress,
			                                   UT_TYPE_STRING,9,caEndnum,
			                                   UT_TYPE_STRING,30,caOemname,
			                                   UT_TYPE_STRING,15,caOemtel,
			                                   UT_TYPE_STRING,19,caIdno,
			                                   UT_TYPE_STRING,30,caAqname,
			                                   UT_TYPE_STRING,15,caAqtel,
			                                   UT_TYPE_STRING,2,caYhxz,
			                                   UT_TYPE_STRING,4,caHylb,
			                                   UT_TYPE_STRING,4,caStar,
			                                   UT_TYPE_STRING,30,caFws,
			                                   UT_TYPE_STRING,10,caSwfs,
			                                   UT_TYPE_STRING,15,caIp,
			                                   UT_TYPE_STRING,15,caSn,
			                                   UT_TYPE_STRING,15,caNetflag,
			                                   UT_TYPE_LONG,4,&lHttpcount,
			                                   UT_TYPE_LONG,4,&lIpcount,
			                                   UT_TYPE_LONG,4,&lFormcount,
			                                   UT_TYPE_LONG,4,&lMailcount,
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
	        
	        utPltPutLoopVarF(psDbHead,"num", iNum,"%lu",iNum);
          utPltPutLoopVarF(psDbHead,"sid", iNum,"%lu",lSid);
          utPltPutLoopVar(psDbHead,"stime", iNum,utTimFormat("%Y/%m/%d %H:%M:%S",lStime));
	        utPltPutLoopVar(psDbHead,"kid",iNum,caKid);
	        utPltPutLoopVar(psDbHead,"dispname",iNum,caDispname);
	        utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
	        utPltPutLoopVar(psDbHead,"dwname",iNum,caDwname);
	        utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
	        utPltPutLoopVar(psDbHead,"endnum",iNum,caEndnum);
	        utPltPutLoopVar(psDbHead,"oemname",iNum,caOemname);
	        utPltPutLoopVar(psDbHead,"oemtel",iNum,caOemtel);
	        utPltPutLoopVar(psDbHead,"idno",iNum,caIdno);
	        utPltPutLoopVar(psDbHead,"aqname",iNum,caAqname);
	        utPltPutLoopVar(psDbHead,"aqtel",iNum,caAqtel);
	        utPltPutLoopVar(psDbHead,"yhxz",iNum,caYhxz);
	        utPltPutLoopVar(psDbHead,"hylb",iNum,getHYNamebyCode(atol(caHylb)));
	        utPltPutLoopVar(psDbHead,"star",iNum,caStar);
	        utPltPutLoopVar(psDbHead,"fws",iNum,caFws);
	        utPltPutLoopVar(psDbHead,"swfs",iNum,getSwfsNamebyCode(caSwfs));
	        utPltPutLoopVar(psDbHead,"ip",iNum,caIp);
	        utPltPutLoopVar(psDbHead,"sn",iNum,caSn);
	        if(strcmp(caNetflag,"2")==0){
	          utPltPutLoopVar(psDbHead,"netflag",iNum,"IP方式");
	        }
	        else if(strcmp(caNetflag,"3")==0){
	        	utPltPutLoopVar(psDbHead,"netflag",iNum,"IP方式");
	        }
	        else if(strcmp(caNetflag,"4")==0||strcmp(caNetflag,"5")==0||strcmp(caNetflag,"1")==0){
	        	utPltPutLoopVar(psDbHead,"netflag",iNum,"实名验证");
	        }
	        utPltPutLoopVarF(psDbHead,"httpcount",iNum,"%d",lHttpcount);
	        utPltPutLoopVarF(psDbHead,"ipcount",iNum,"%d",lIpcount);
	        utPltPutLoopVarF(psDbHead,"formcount",iNum,"%d",lFormcount);
	        utPltPutLoopVarF(psDbHead,"mailcount",iNum,"%d",lMailcount);
	   
         
      
     
			   lSid=0;
         memset(caKid,0,sizeof(caKid));
         memset(caDispname,0,sizeof(caDispname));
         memset(caServicecode,0,sizeof(caServicecode));
         memset(caDwname,0,sizeof(caDwname));
         memset(caAddress,0,sizeof(caAddress));
         memset(caEndnum,0,sizeof(caEndnum));
         memset(caOemname,0,sizeof(caOemname));
         memset(caOemtel,0,sizeof(caOemtel));
         memset(caIdno,0,sizeof(caIdno));
         memset(caAqname,0,sizeof(caAqname));
         memset(caAqtel,0,sizeof(caAqtel));
         memset(caYhxz,0,sizeof(caYhxz));
         memset(caHylb,0,sizeof(caHylb));
         memset(caStar,0,sizeof(caStar));
         memset(caFws,0,sizeof(caFws));
         memset(caSwfs,0,sizeof(caSwfs));
         memset(caIp,0,sizeof(caIp));
         memset(caSn,0,sizeof(caSn));
         memset(caNetflag,0,sizeof(caNetflag));
         lHttpcount=0;
         lIpcount=0;
         lFormcount=0;
         lMailcount=0;
         lStime=0;
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lSid,
			                                   UT_TYPE_STRING,30,caKid,
			                                   UT_TYPE_STRING,30,caDispname,
			                                   UT_TYPE_STRING,15,caServicecode,
			                                   UT_TYPE_STRING,99,caDwname,
			                                   UT_TYPE_STRING,127,caAddress,
			                                   UT_TYPE_STRING,9,caEndnum,
			                                   UT_TYPE_STRING,30,caOemname,
			                                   UT_TYPE_STRING,15,caOemtel,
			                                   UT_TYPE_STRING,19,caIdno,
			                                   UT_TYPE_STRING,30,caAqname,
			                                   UT_TYPE_STRING,15,caAqtel,
			                                   UT_TYPE_STRING,2,caYhxz,
			                                   UT_TYPE_STRING,4,caHylb,
			                                   UT_TYPE_STRING,4,caStar,
			                                   UT_TYPE_STRING,30,caFws,
			                                   UT_TYPE_STRING,10,caSwfs,
			                                   UT_TYPE_STRING,15,caIp,
			                                   UT_TYPE_STRING,15,caSn,
			                                   UT_TYPE_STRING,15,caNetflag,
			                                   UT_TYPE_LONG,4,&lHttpcount,
			                                   UT_TYPE_LONG,4,&lIpcount,
			                                   UT_TYPE_LONG,4,&lFormcount,
			                                   UT_TYPE_LONG,4,&lMailcount,
			                                   UT_TYPE_LONG,4,&lStime);
    }



    lTotPg = (lCount -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lCount);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        
//     utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_usbkeylog_list.htm");
    return 0;
}


// 显示MAC扫描日志
int ncsScanMaclist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{

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
   	long lCurPg,lRowNum,lStartRec,lSstarttime;
   	char sql[40960] = "";
   char caTotPg[16],caCurPg[16];
   char caPage[12],caLimit[16],caSort[16],caDir[16];
   char caKeyword[64],caTime_flag[32];
   	char sql2[256] = "";
   	char frmfile[256] = "";
   	char tmp[32];
   	char tmpstr[32];
   	char *tmpgbk = NULL;
	char **ptmp = NULL;	
	long outputlimit=0;
	
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
    char datanum[20];
    long datanums=0;
   long iReturn;
  unsigned long lIp;
    char caStime[16];
   	char caEtime[16];
   	char caTemp_d[64];
   	char caGroupid[36],caDid[36];
    char caSdate[24],caEdate[24];
    char caApmac[36],caMac[36],caApname[64],caPosition[64];
    char caCorpname[128];
	 strcpy(caFacs,getDsFacDid());
   strcpy(caGroups,getDsGroupid());  
   utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	printf("lTIme = %d\n",lTime);
	iReturn = utMsgGetSomeNVar(psMsgHead,21,
	    "TotPg",      UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      UT_TYPE_STRING, 12, caCurPg,
      "exp",     UT_TYPE_STRING, 10, caExport,
      "datanum", UT_TYPE_STRING, 16, datanum,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag,
   	  "did",          UT_TYPE_STRING,10,caDid,
   	  "groupid",      UT_TYPE_STRING,10,caGroupid,
   	  "sdate",        UT_TYPE_STRING,12,sdate,
   	  "stime",        UT_TYPE_STRING,10,caStime,
   	  "edate",        UT_TYPE_STRING,12,edate,
   	  "etime",        UT_TYPE_STRING,10,caEtime,
   	  "apmac",        UT_TYPE_STRING,20,caApmac,
   	  "mac",          UT_TYPE_STRING,20,caMac,
   	  "apname",       UT_TYPE_STRING,31,caApname,
   	  "position",     UT_TYPE_STRING,31,caPosition,
   	  "corpname",     UT_TYPE_STRING,60,caCorpname);
   	  printf("pid = %d\n",getpid());
   	  datanums = atol(datanum);
   	 
   	  
   	  if(!(strlen(caExport)>0)){
   	   if(strlen(caKeyword)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
      }
      
   	  if(strlen(caCorpname)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caCorpname,caTemp,60));
      	strcpy(caCorpname,caTemp_d);
      }
   	  
   	 if(strlen(caApname)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caApname,caTemp,60));
      	strcpy(caApname,caTemp_d);
      }
   	 if(strlen(caPosition)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caPosition,caTemp,60));
      	strcpy(caPosition,caTemp_d);
      }
    }
      
     
   	
   	  
   	 lCurPg = atol(caPage);
   	  // printf("lCurPg = %d\n", lCurPg);
    if(lCurPg <= 0) lCurPg = 1;

 
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    		//printf("lRowNum = %ld\n" , lRowNum);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum; 
      	//printf("lStartRec = %ld\n" , lStartRec);
   	  
   	iTime_flag = atoi(caTime_flag);
   		//printf("iTime_flag = %ld\n" , iTime_flag);
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
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncsmaclog_if_", &lStartTime, &lTime, &lCount);
		//printf("lStartTime=%d,sdate=%s\n",lStartTime,sdate);
	}
	else{
		ptmp = ncsUtlGetTable(lTime, days, "ncsmaclog_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}

  printf("tables %d\n", lCount);
	for(i = 0; i < lCount; i++)
	{
		snprintf(frmfile, sizeof(frmfile), "/usr/local/infobright/data/ncs/%s.frm", ptmp[i]);
//		if(access(frmfile, F_OK) == 0)
//		{
			lCount1 = 0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s where starttime >= %lu and starttime <= %lu  ",  ptmp[i], lStartTime, lTime);
			if(strlen(caKeyword)>0){
				 if(strlen(caKeyword)==17){
				 	snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac='%s' ",caKeyword);
				}
				else{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (mac like '%c%s%c' or servicename like '%c%s%c' or apmac like '%c%s%c' or servicecode like '%c%s%c')",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
			  }
		 }
		    if(strlen(caDid)>0){
//	    	sprintf(sql+strlen(sql)," and servicecode in (select username from ncsuser where userid=%s )",caDid);
          sprintf(sql+strlen(sql)," and servicecode='%s' ",getServicecodeByid(psShmHead,atol(caDid)));
	    }
	    if(strlen(caGroupid)>0){
	//    	sprintf(sql+strlen(sql)," and servicecode in (select username from ncsuser where groupid=%s )",caGroupid);
	        sprintf(sql+strlen(sql)," and servicecode in (%s)",getServicecodeByGroupid(psShmHead,caGroupid));
	    }
	    if(strlen(caGroups)>0){
//	    	sprintf(sql+strlen(sql)," and servicecode in (select username from ncsuser where groupid in (%s)) ",caGroups);
          sprintf(sql+strlen(sql)," and servicecode in (%s)",getServicecodeByGroupid(psShmHead,caGroups));
	    }


      
		   if(strlen(caApmac)>0){
	    	sprintf(sql+strlen(sql)," and apmac like '%c%s%c' ",'%',caApmac,'%');
	    }
	    if(strlen(caMac)>0){
	    	sprintf(sql+strlen(sql)," and mac like '%c%s%c' ",'%',caMac,'%');
	    }
	    if(strlen(caPosition)>0){
	    	sprintf(sql+strlen(sql)," and address like '%c%s%c' ",'%',caPosition,'%');
	    }
	    if(strlen(caCorpname)>0){
	    	sprintf(sql+strlen(sql)," and servicename like '%c%s%c' ",'%',caCorpname,'%');
	    }
	    if(strlen(caApname)>0){
	    	sprintf(sql+strlen(sql)," and apname like '%c%s%c' ",'%',caApname,'%');
	    }
	    
	    if(datanums > 0){
	    	sprintf(sql+strlen(sql), "order by starttime limit %lu, %lu", iStart, datanums);
	    }
	
			tmpgbk = convert("UTF-8", "GBK", sql);
			//if(tmpgbk != NULL)
			//{
				printf("sql = %s\n", sql);
				pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
					printf("lCount1 = %ld\n" , lCount1);
			//	free(tmpgbk);
			//}
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


	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	
	if(datanums>0 && datanums < lTotRec)lTotRec = datanums;
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
   //printf("lTotRec= %d\n" , lTotRec);
	

	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	}
	//
	
	iNum = 0;
	iStart = lStartRec;
	iLimit = lRowNum;
	if(datanums == 0)
		datanums = lTotRec;
	
    //printf("iStart = %ld, iLimit = %ld\n" , iStart, iLimit);
		//printf("num = %d\n",num);
	
	for(i = num-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		snprintf(sql, sizeof(sql), "select servicecode,servicename,address,mac,apname,apmac,ssid,starttime,endtime,flags from %s where starttime >= %lu and starttime <= %lu  ", tableinfo[i].tname, lStartTime, lTime);
		
			if(!utStrIsSpaces(caKeyword))
			{
				 if(strlen(caKeyword)==17){
				 	snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and mac='%s' ",caKeyword);
				}
				else{
				snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " and (mac like '%c%s%c' or servicename like '%c%s%c' or apmac like '%c%s%c' or servicecode like '%c%s%c')",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
			  }
			}
	    if(strlen(caDid)>0){
	    	    sprintf(sql+strlen(sql)," and servicecode='%s' ",getServicecodeByid(psShmHead,atol(caDid)));
	//    	sprintf(sql+strlen(sql)," and servicecode in (select username from ncsuser where userid=%s )",caDid);
	    }
	    if(strlen(caGroupid)>0){
	    	 sprintf(sql+strlen(sql)," and servicecode in (%s)",getServicecodeByGroupid(psShmHead,caGroupid));
	//    	sprintf(sql+strlen(sql)," and servicecode in (select username from ncsuser where groupid=%s )",caGroupid);
	    }
	    if(strlen(caGroups)>0){
	    	sprintf(sql+strlen(sql)," and servicecode in (%s)",getServicecodeByGroupid(psShmHead,caGroups));
  //	   sprintf(sql+strlen(sql)," and servicecode in (select username from ncsuser where groupid in (%s)) ",caGroups);
	    }
	
	    if(strlen(caApmac)>0){
	    	sprintf(sql+strlen(sql)," and apmac like '%c%s%c' ",'%',caApmac,'%');
	    }
	    if(strlen(caMac)>0){
	    	sprintf(sql+strlen(sql)," and mac like '%c%s%c' ",'%',caMac,'%');
	    }
	     if(strlen(caApname)>0){
	    	sprintf(sql+strlen(sql)," and apname like '%c%s%c' ",'%',caApname,'%');
	    }
	    if(strlen(caPosition)>0){
	    	sprintf(sql+strlen(sql)," and address like '%c%s%c' ",'%',caPosition,'%');
	    }
	    if(strlen(caCorpname)>0){
	    	sprintf(sql+strlen(sql)," and servicename like '%c%s%c' ",'%',caCorpname,'%');
	    }
	    if(datanums<iLimit && datanums>0) iLimit = datanums;
	    	if(strlen(caExport)>0){
	    		 snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by starttime desc limit %lu, %lu", iStart, datanums);
	    		}
	    		 	else
		   snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by starttime desc limit %lu, %lu", iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
		
		
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			_break = 1;
		}
	  
    outputlimit = iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount;
		printf("__line = %d, sql = %s\n", __LINE__, sql);
		   if(strlen(caExport)>0){
    
  
            sprintf(caFilename,"ncsmaclog%d.csv",utStrGetId());
            sprintf(caPath,"%s/html","/home/ncmysql/ncs");
            sprintf(caFile,"%s/%s",caPath,caFilename);
            fp = fopen(caFile,"w");
            if(fp == NULL) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
                return 0;
            }
            printf("写日志、\n");
            fprintf(fp,"MACLOG日志\n");
            ncUtlPrintCsv(fp,7,UT_TYPE_STRING,"MAC地址",
                               UT_TYPE_STRING,"AP名称",
                               UT_TYPE_STRING,"接入AP MAC",
                               UT_TYPE_STRING,"场所名称",
                               UT_TYPE_STRING,"安装位置",
                               UT_TYPE_STRING,"进入时间",
                               UT_TYPE_STRING,"离开时间");

  }
  
		psCur = pasDbOpenSql(sql, 0);
		if(psCur != NULL)
		{
         char caServicecode[16],caServicename[64],caAddress[64],caMac[20],caApname[32],caApmac[20],caSsid[64];
         long lStarttime,lEndtime,lFlags;
         memset(caServicecode,0,sizeof(caServicecode));
         memset(caServicename,0,sizeof(caServicename));
         memset(caAddress,0,sizeof(caAddress));
         memset(caMac,0,sizeof(caMac));
         memset(caApname,0,sizeof(caApname));
         memset(caApmac,0,sizeof(caApmac));
         memset(caSsid,0,sizeof(caSsid));
         
			iret = pasDbFetchInto(psCur,
			     UT_TYPE_STRING,14,caServicecode,
			     UT_TYPE_STRING,63,caServicename,
			     UT_TYPE_STRING,63,caAddress,
			     UT_TYPE_STRING,18,caMac,
			     UT_TYPE_STRING,31,caApname,
			     UT_TYPE_STRING,18,caApmac,
			     UT_TYPE_STRING,63,caSsid,
			     UT_TYPE_LONG,4,&lStarttime,
			     UT_TYPE_LONG,4,&lEndtime,
			     UT_TYPE_LONG,4,&lFlags);
			     
			    
					
			while((iret == 0) || (iret == 1405))
			{
				iNum++;
				if(outputlimit>0){
					outputlimit--;
				if(iNum > 1)
				{
					utPltPutLoopVar(psDbHead,"dh",iNum,",");
				}
          //printf("  %ld          %ld\n",lStarttime,lEndtime);
         utPltPutLoopVar(psDbHead,"starttime",iNum,utTimFormat("%y/%m/%d %H:%M",lStarttime));
         if(lFlags==1){
           utPltPutLoopVar(psDbHead,"endtime",iNum,utTimFormat("%y/%m/%d %H:%M",lEndtime));
         }
         utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
         utPltPutLoopVar(psDbHead,"apname",iNum,caApname);
         utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
         utPltPutLoopVar(psDbHead,"apmac",iNum,caApmac);
          utPltPutLoopVar(psDbHead,"servicename",iNum,caServicename);
        }
        
        
         
         if(strlen(caExport)>0 && datanums > 0){
         //printf("datanums = %ld\n", datanums);
         	strcpy(tmp,utTimFormat("%y/%m/%d %H:%M",lEndtime));
         	if(lFlags==1)
         ncUtlPrintCsv(fp,7,UT_TYPE_STRING, caMac,
                               UT_TYPE_STRING,caApname,
                               UT_TYPE_STRING,caApmac,
                               UT_TYPE_STRING,caServicename,                            
                               UT_TYPE_STRING,caAddress,
                               UT_TYPE_STRING,utTimFormat("%y/%m/%d %H:%M",lStarttime),
                               UT_TYPE_STRING,tmp);
                                else
          	{
          		 ncUtlPrintCsv(fp,6,UT_TYPE_STRING, caMac,
                               UT_TYPE_STRING,caApname,
                               UT_TYPE_STRING,caApmac,
                               UT_TYPE_STRING,caServicename,                            
                               UT_TYPE_STRING,caAddress,
                               UT_TYPE_STRING,utTimFormat("%y/%m/%d %H:%M",lStarttime));
          	}
                               datanums--;
                               if(datanums==0)
                               	break;
                               }
         
         memset(caServicecode,0,sizeof(caServicecode));
         memset(caServicename,0,sizeof(caServicename));
         memset(caAddress,0,sizeof(caAddress));
         memset(caMac,0,sizeof(caMac));
         memset(caApname,0,sizeof(caApname));
         memset(caApmac,0,sizeof(caApmac));
         memset(caSsid,0,sizeof(caSsid));
         
			iret = pasDbFetchInto(psCur,
			     UT_TYPE_STRING,14,caServicecode,
			     UT_TYPE_STRING,63,caServicename,
			     UT_TYPE_STRING,63,caAddress,
			     UT_TYPE_STRING,18,caMac,
			     UT_TYPE_STRING,31,caApname,
			     UT_TYPE_STRING,18,caApmac,
			     UT_TYPE_STRING,63,caSsid,
			     UT_TYPE_LONG,4,&lStarttime,
			     UT_TYPE_LONG,4,&lEndtime,
			     UT_TYPE_LONG,4,&lFlags);
			}			 
			pasDbCloseCursor(psCur);
		} 
     
		if(_break == 1)
			break; 
	}
  if(strlen(caExport)>0){
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        printf("output\n");
        return 0;
   }
   printf("hhasss\n");

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_scanmaclog_list.htm");
    return 0;
}

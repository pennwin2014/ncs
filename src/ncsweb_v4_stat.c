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
#include "dlfcn.h"
#include "time.h"

extern char *pHashNettype;

typedef struct data_s{
         char caDispname[32];
         unsigned long lKeyyh;
         unsigned long lKeyxs;
         unsigned long lKeydq;
         unsigned long lKeytot;
         unsigned long lUrlyh;
         unsigned long lUrlxs;
         unsigned long lUrldq;
         unsigned long lUrltot;
         unsigned long lVidyh;
         unsigned long lVidxs;
         unsigned long lViddq;
         unsigned long lVidtot;
         unsigned long lUseyh;
         unsigned long lUsexs;
         unsigned long lUsedq;
         unsigned long lUsetot;
         unsigned long lBkyh;
         unsigned long lBkxs;
         unsigned long lBkdq;
         unsigned long lBktot;
         unsigned long lTot;
    } ncsDate_s;
static long iSort,iBy;
static int ncUtlSortOnCount(char *p1,char *p2)
{
    int i;
    long  l;

    struct data_s psC1,psC2;
    memcpy(&psC1,p1,sizeof(struct data_s));
    memcpy(&psC2,p2,sizeof(struct data_s)); 
    if(iBy==1){
        if(iSort==1){
            l=psC2.lTot-psC1.lTot;
        }
        else if(iSort==2){
            l=psC2.lKeyyh-psC1.lKeyyh;
        }
        else if(iSort==3){
            l=psC2.lKeyxs-psC1.lKeyxs;
        }
        else if(iSort==4){
            l=psC2.lKeydq-psC1.lKeydq;
        }
        else if(iSort==5){
            l=psC2.lKeytot-psC1.lKeytot;
        }
        else if(iSort==6){
            l=psC2.lUrlyh-psC1.lUrlyh;
        }
        else if(iSort==7){
            l=psC2.lUrlxs-psC1.lUrlxs;
        }
        else if(iSort==8){
            l=psC2.lUrldq-psC1.lUrldq;
        }
        else if(iSort==9){
            l=psC2.lUrltot-psC1.lUrltot;
        }
      else if(iSort==10){
            l=psC2.lVidyh-psC1.lVidyh;
        }
        else if(iSort==11){
            l=psC2.lVidxs-psC1.lVidxs;
        }
        else if(iSort==12){
            l=psC2.lViddq-psC1.lViddq;
        }
        else if(iSort==13){
            l=psC2.lVidtot-psC1.lVidtot;
        }
        else if(iSort==14){
            l=psC2.lUseyh-psC1.lUseyh;
        }
        else if(iSort==15){
            l=psC2.lUsexs-psC1.lUsexs;
        }
        else if(iSort==16){
            l=psC2.lUsedq-psC1.lUsedq;
        }
        else if(iSort==17){
            l=psC2.lUsetot-psC1.lUsetot;
        }
        else if(iSort==18){
            l=psC2.lBkyh-psC1.lBkyh;
        }
        else if(iSort==19){
            l=psC2.lBkxs-psC1.lBkxs;
        }
        else if(iSort==20){
            l=psC2.lBkdq-psC1.lBkdq;
        }
        else if(iSort==21){
            l=psC2.lBktot-psC1.lBktot;
        }
        else{
            l=psC2.lTot-psC1.lTot;
        }
    }
    else{
        
        if(iSort==1){
            l=psC1.lTot-psC2.lTot;
        }
        else if(iSort==2){
            l=psC1.lKeyyh-psC2.lKeyyh;
        }
        else if(iSort==3){
            l=psC1.lKeyxs-psC2.lKeyxs;
        }
        else if(iSort==4){
            l=psC1.lKeydq-psC2.lKeydq;
        }
        else if(iSort==5){
            l=psC1.lKeytot-psC2.lKeytot;
        }
        else if(iSort==6){
            l=psC1.lUrlyh-psC2.lUrlyh;
        }
        else if(iSort==7){
            l=psC1.lUrlxs-psC2.lUrlxs;
        }
        else if(iSort==8){
            l=psC1.lUrldq-psC2.lUrldq;
        }
        else if(iSort==9){
            l=psC1.lUrltot-psC2.lUrltot;
        }
      else if(iSort==10){
            l=psC1.lVidyh-psC2.lVidyh;
        }
        else if(iSort==11){
            l=psC1.lVidxs-psC2.lVidxs;
        }
        else if(iSort==12){
            l=psC1.lViddq-psC2.lViddq;
        }
        else if(iSort==13){
            l=psC1.lVidtot-psC2.lVidtot;
        }
        else if(iSort==14){
            l=psC1.lUseyh-psC2.lUseyh;
        }
        else if(iSort==15){
            l=psC1.lUsexs-psC2.lUsexs;
        }
        else if(iSort==16){
            l=psC1.lUsedq-psC2.lUsedq;
        }
        else if(iSort==17){
            l=psC1.lUsetot-psC2.lUsetot;
        }
        else if(iSort==18){
            l=psC1.lBkyh-psC2.lBkyh;
        }
        else if(iSort==19){
            l=psC1.lBkxs-psC2.lBkxs;
        }
        else if(iSort==20){
            l=psC1.lBkdq-psC2.lBkdq;
        }
        else if(iSort==21){
            l=psC1.lBktot-psC2.lBktot;
        }
        else{
            l=psC1.lTot-psC2.lTot;
        }
        
        
    }
  
  
    if(l>0) return 1;
    if(l<0) return -1;
    if(l==0) return 0;
  
} 

//统计上下线记录数 按日统计
int ncsUserlogStatByday_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[2048];
    char caEdate[20];
    char caWireflag[16];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
       int iReturn;

  
       float lRat;
       long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;

    char sqlbuf[2048]="";
    unsigned long lStatrow,lSum;
   long lStime,lEtime;
     pasDbCursor *psCur;
 
     char caTable[36];

  char caSdate_d[20],caEdate_d[20],caSdate_t[20];
  struct data_s{
         char caDate[16];
         unsigned long lCount;
         unsigned long lCount_netid;
    };
    long index,lSumitem,lstime1;
  struct data_s stData[32];
  lCurPg=1;
     strcpy(caWhere,"\0");

     

     
    iReturn = utMsgGetSomeNVar(psMsgHead,3,
            "sdate",UT_TYPE_STRING,19,caSdate,
            "edate",UT_TYPE_STRING,19,caEdate,
            "wireflag",UT_TYPE_STRING,10,caWireflag);
            
    
    utMsgPrintMsg(psMsgHead); 
   
   lTime=time(0);
   ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
   if(strlen(caSdate)==0){
               strcpy(caSdate,caSdate_d);
    }
    if(strlen(caEdate)==0){
        strcpy(caEdate,caEdate_d);
    }
    
    
    sprintf(caStemptime,"%s 00:00",caSdate);
    lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
    sprintf(caStemptime,"%s 23:59",caEdate);
    letime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
    
    
    strcpy(caSdate,utTimFormat("%Y-%m-%d",lstime));
    strcpy(caEdate,utTimFormat("%Y-%m-%d",letime));
    
    lstime1=lstime;
    
    
    for(i=lstime;i<letime;i=i+86400){
        index=(i-lstime)/86400;

        strcpy(stData[index].caDate,utTimFormat("%m/%d",i));

        stData[index].lCount=0;
        stData[index].lCount_netid=0;
        if(index>30) break;
    }
    lSumitem=index+1;
    
    sprintf(caTemp,"select sdate,sum(lcount) from ncsuserlogstatday where sdate>='%s' and sdate<='%s' ",caSdate,caEdate);
    if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where (jointype='08' or jointype='09')) ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where jointype!='08') ");
      }
    
    }
    
    sprintf(caTemp+strlen(caTemp)," group by sdate order by sdate limit 0,31 ");
        

    
printf("caTemp=%s\n",caTemp);     
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
        
    iReturn = 0;
    memset(caSdate_t,0,sizeof(caSdate_t));
    lCount=0;
        iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caSdate_t,
                                               UT_TYPE_LONG,4,&lCount);
                                       
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
              
     sprintf(caStemptime,"%s 00:00",caSdate_t);
     lTime=utTimStrToLong("%Y-%m-%d %H:%M",caStemptime);
     index=(lTime-lstime)/86400;
     stData[index].lCount=lCount;
 //     memset(caSdate_t,0,sizeof(caSdate_t));
     lCount=0;
                iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caSdate_t,
                                               UT_TYPE_LONG,4,&lCount);
    }


    pasDbCloseCursor(psCur);

/*
    if(strcmp(caSdate_t,caEdate)<0&&index<31){ 
      if(strcmp(caSdate_t,caSdate)<0){
           strcpy(caSdate_t,caSdate);
      }
        lstime=utTimStrToLong("%Y-%m-%d",caSdate_t)+86400;                            //还需要在ncsuserlog中统计
        sprintf(caTable,"ncsuserlog_if_%s",utTimFormat("%Y%m",lTime));
//        sprintf(caTemp,"select from_unixtime(stime,'%%Y-%%m-%%d') aa,count(*) from %s where stime>=%d and stime<=%d group by aa order by stime ",caTable,lstime,letime);
     sprintf(caTemp,"select stime from %s where stime>=%d and stime<=%d  ",caTable,lstime,letime);
     
      if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where (jointype='08' or jointype='09')) ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where jointype!='08') ");
      }
    
    }
     
     
printf("caTemp=%s\n",caTemp);
        psCur=pasDbOpenSql(caTemp,0);
        if(psCur!= NULL) {
    //          memset(caSdate_t,0,sizeof(caSdate_t));
          lCount=0;
//                iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caSdate_t,
//                                               UT_TYPE_LONG,4,&lCount);
          iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lTime);                      
        while(iReturn == 0||iReturn==1405) {
            
            //         sprintf(caStemptime,"%s 00:00",caSdate_t);
    //                 lTime=utTimStrToLong("%Y-%m-%d %H:%M",caStemptime);
                     index=(lTime-lstime1)/86400;
                     stData[index].lCount++;
                     
                      if(index>30) break;
//                      memset(caSdate_t,0,sizeof(caSdate_t));
                     lCount=0;
                     iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lTime);  
    //                iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caSdate_t,
    //                                               UT_TYPE_LONG,4,&lCount);
         }
      pasDbCloseCursor(psCur);
            
            

        }
    }
  */  
//统计虚拟身份轨迹    
    sprintf(caTemp,"select sdate,sum(lcount) from ncsnetidstatday where sdate>='%s' and sdate<='%s' ",caSdate,caEdate);
    if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where (jointype='08' or jointype='09')) ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where jointype!='08') ");
      }
    
     }   
     sprintf(caTemp+strlen(caTemp)," group by sdate order by sdate limit 0,31 ");
        

    
printf("caTemp=%s\n",caTemp);     
    
       
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
        
    iReturn = 0;
    memset(caSdate_t,0,sizeof(caSdate_t));
    lCount=0;
                iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caSdate_t,
                                               UT_TYPE_LONG,4,&lCount);
                                            
                                   
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
              
     sprintf(caStemptime,"%s 00:00",caSdate_t);
     lTime=utTimStrToLong("%Y-%m-%d %H:%M",caStemptime);
     index=(lTime-lstime1)/86400;
 //    printf("index=%d\n",index);
     stData[index].lCount_netid=lCount;
   //   memset(caSdate_t,0,sizeof(caSdate_t));
     lCount=0;
                iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caSdate_t,
                                               UT_TYPE_LONG,4,&lCount);
    }


    pasDbCloseCursor(psCur);
 //   printf("ccccc\n");
       if(strcmp(caSdate_t,caSdate)<0){
           strcpy(caSdate_t,caSdate);
      }
 
 /*
     if(strcmp(caSdate_t,caEdate)<0){ 

        lstime=utTimStrToLong("%Y-%m-%d",caSdate_t)+86400;                            //还需要在ncsnetlog中统计
        sprintf(caTable,"ncsnetlog_if_%s",utTimFormat("%Y%m",lTime));
        sprintf(caTemp,"select from_unixtime(stime,'%%Y-%%m-%%d') aa,count(*) from %s where stime>=%d and stime<=%d ",caTable,lstime,letime);
    if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where (jointype='08' or jointype='09')) ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where jointype!='08') ");
      }
    
     } 
     sprintf(caTemp+strlen(caTemp)," group by aa order by stime ");

//printf("pid=%d\n",getpid());
//sleep(15);
printf("caTemp=%s\n",caTemp);
        psCur=pasDbOpenSql(caTemp,0);
        if(psCur!= NULL) {
    //          memset(caSdate_t,0,sizeof(caSdate_t));
          lCount=0;
                iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caSdate_t,
                                               UT_TYPE_LONG,4,&lCount);
                                
        while(iReturn == 0||iReturn==1405) {
            
                     sprintf(caStemptime,"%s 00:00",caSdate_t);
                     lTime=utTimStrToLong("%Y-%m-%d %H:%M",caStemptime);
                     index=(lTime-lstime1)/86400;
                     stData[index].lCount_netid=lCount;
                     
                      if(index>30) break;
//                      memset(caSdate,0,sizeof(caSdate));
                     lCount=0;
                    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caSdate_t,
                                                   UT_TYPE_LONG,4,&lCount);
         }
      pasDbCloseCursor(psCur);
            
            

        }
    }
    
  */  
    
    
    
    iNum=0;
        psDbHead = utPltInitDb(); 

    for(i=0;i<lSumitem;i++){        
        if(iNum>0){
               utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
         }
         else{
              utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
         }
            iNum++;

          utPltPutLoopVar(psDbHead,"sdate",iNum,stData[i].caDate);
          utPltPutLoopVarF(psDbHead,"lcount",iNum,"%d",stData[i].lCount);
          utPltPutLoopVarF(psDbHead,"lcount_netid",iNum,"%d",stData[i].lCount_netid);
   printf("lcount=%d\n",stData[i].lCount);
    }
    
    
    lSum=lSumitem;
    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    
  printf("cccc\n");
  //     utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_systat_list.htm");
    return 0;
} 


int isInGroup(char* caGroups,long lGroupid)
{
    int iReturn;
    char caTemp[10480],caGroupid[20];

    sprintf(caTemp,",%s,",caGroups);
    sprintf(caGroupid,",%d,",lGroupid);
    if(strstr(caTemp,caGroupid)){
        return 1;
    }
    else{
        return 0;
    }
}



//显示首页
int ncsShowZhtj_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[9048];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
       int iReturn;
        long lFlags;
        long lBk_w;
        long lBk_y;
   char caWireflag[12];
       float lRat;
       long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;

    char sqlbuf[9048]="";
    unsigned long lStatrow,lSum;
   long lStime,lEtime;
     pasDbCursor *psCur;
 
     char caTable[36];

  char caSdate_d[20],caEdate_d[20],caSdate_t[20];
 
   unsigned char *pHash_wh;
   struct corpwh_s{
           unsigned long lUserid;  //单位ID
          unsigned long flag;     //4-停业 5-维护
      };
   struct corpwh_s *psData_wh;
  unsigned long lGroupid,lUserid;
  char caGroupname[32];
  long wh_count=0;
  long ty_count=0;
  char caCode[16],caName[64];
   unsigned char *pHash_fac;
   unsigned char *pHash_factoid;
   struct corpfac_s{
           char caCode[12];  //厂商代码
           char caName[64];
           long lSum;       //单位总数
           long lWh;
           long lTy;
           long lZx;
      };
      struct corpfactoid_s{
           long lUserid;
           char caCode[12];
      };
   struct corpfac_s *psData_fac;
   struct corpfactoid_s *psData_factoid;
   pasLHashInfo sHashInfo;
   char caGroups[4024];
  char caFacs_did[4024];
  char caFacs[4024];
  long lMax,lTemp;
  printf("pid=%d\n",getpid());
//sleep(15);
  
  strcpy(caFacs,getDsFacGroupid());
     strcpy(caFacs_did,getDsFacDid());
   strcpy(caGroups,getDsGroupid());
   
   utMsgGetSomeNVar(psMsgHead,1, "wireflag",UT_TYPE_STRING, 8, caWireflag);
   
   
   pHash_factoid = (unsigned char *)pasLHashInit(10000,10000,sizeof(struct corpfactoid_s),0,4);   
    pHash_fac = (unsigned char *)pasLHashInit(100,100,sizeof(struct corpfac_s),0,12);    
    if(pHash_fac == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
    sprintf(caTemp,"select code,name,userid from ncsuser,ncsfactorycod where fcode=code  ");
    if(strlen(caFacs_did)>0){
        sprintf(caTemp+strlen(caTemp)," and userid in (%s) ",caFacs_did);
    }
    
     if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and (jointype='08' or jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and jointype!='08' ");
      }
    
    }
    if(strlen(caGroups)>0){
      sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caGroups);
    }
    
 //   printf("caTemp1111=%s\n",caTemp);
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur){
          memset(caCode,0,sizeof(caCode));
          memset(caName,0,sizeof(caName));
          lUserid=0;
          iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,11,caCode,
                                       UT_TYPE_STRING,63,caName,
                                       UT_TYPE_LONG,4,&lUserid);
          while(iReturn==0||iReturn==1405){
              psData_fac = (struct corpfac_s *)pasLHashLookA(pHash_fac,caCode);  
            if(psData_fac){
                strcpy(psData_fac->caName,caName);
                psData_fac->lSum++;
            }
              psData_factoid=(struct corpfactoid_s *)pasLHashLookA(pHash_factoid,&lUserid); 
              if(psData_factoid){
                  strcpy(psData_factoid->caCode,caCode);
              } 
              memset(caCode,0,sizeof(caCode));
              memset(caName,0,sizeof(caName));
              lUserid=0;
              iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,11,caCode,
                                           UT_TYPE_STRING,63,caName,
                                           UT_TYPE_LONG,4,&lUserid);
          }                               
        pasDbCloseCursor(psCur);  
    }
 
 
    utMsgPrintMsg(psMsgHead); 
   
    //将维护和停用单位放到Hash中
    pHash_wh = (unsigned char *)pasLHashInit(1000,1000,sizeof(struct corpwh_s),0,4);    
    if(pHash_wh == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
   sprintf(caTemp,"select userid,servicestate from ncsuser where servicestate>='2' and servicestate<='5' ");
       if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and (jointype='08' or jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and jointype!='08' ");
      }
    
    }
  sprintf(caTemp+strlen(caTemp)," limit 0,999 ");

  printf("caTemp=%s\n",caTemp);
  psCur = pasDbOpenSql(caTemp,0);
  if(psCur == NULL) {
        if(pHash_wh) free(pHash_wh);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
   }
  lGroupid=0;
  memset(caGroupname,0,sizeof(caGroupname));
  iReturn = pasDbFetchInto(psCur,                                     
                                       UT_TYPE_LONG,4,&lGroupid,
                                       UT_TYPE_STRING,1,caGroupname);
                                     
                                      
                                      
    while((iReturn==0)||(iReturn==1405))
    {
             psData_wh = (struct corpwh_s *)pasLHashLookA(pHash_wh,&lGroupid);  
            if(psData_wh){
                  if(strcmp(caGroupname,"5")==0){
                      psData_wh->flag=5;
                  }
                  else{
                      psData_wh->flag=4;
                  }
            
            }
             
              lGroupid=0;
              memset(caGroupname,0,sizeof(caGroupname));
              iReturn = pasDbFetchInto(psCur,                                     
                                                   UT_TYPE_LONG,4,&lGroupid,
                                                   UT_TYPE_STRING,1,caGroupname);                                   
    }    
    pasDbCloseCursor(psCur);  
   
   
   
   
   lTime=time(0);
    ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
      psDbHead = utPltInitDb(); 
    utPltPutVar(psDbHead,"sdate",caSdate_d);
    utPltPutVar(psDbHead,"edate",caEdate_d);
 
 // 统计布控告警
    sprintf(caTemp,"select count(*),a.flags from ncscasemesg a,ncscaseinfo b where a.cid=b.sid and b.flags=1 ");
     if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and a.did in (select userid from ncsuser where  (jointype='08' or jointype='09')) ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and a.did in (select userid from ncsuser where jointype!='08') ");
      }
    
    }
    
    sprintf(caTemp+strlen(caTemp)," group by a.flags ");
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
     lCount=0;
     lFlags=0;
     iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                  UT_TYPE_LONG,4,&lFlags);
     lBk_w=0;
     lBk_y=0;
                                               
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
        if(lFlags==0){
            lBk_w=lCount;
        }
        else{
            lBk_y=lBk_y+lCount;
        }
        
     lCount=0;
     lFlags=0;
     iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                  UT_TYPE_LONG,4,&lFlags);
    }


    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead,"bkgjw","%d",lBk_w);
    utPltPutVarF(psDbHead,"bkgjy","%d",lBk_y);
 
 
  // 统计关键字告警
    sprintf(caTemp,"select count(*),flag from ncskeywordlog where 1=1 ");
    if(strlen(caGroups)>0){
        sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
    }
    if(strlen(caFacs_did)>0){
        sprintf(caTemp+strlen(caTemp)," and did in (%s) ",caFacs_did);
    }
    
    if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where (jointype='08' or jointype='09')) ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where jointype!='08') ");
      }
    
    }
    
     sprintf(caTemp+strlen(caTemp)," group by flag ");
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
     lCount=0;
     lFlags=0;
     iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                  UT_TYPE_LONG,4,&lFlags);
     lBk_w=0;
     lBk_y=0;
                                               
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
        if(lFlags==0){
            lBk_w=lCount;
        }
        else{
            lBk_y=lBk_y+lCount;
        }
        
     lCount=0;
     lFlags=0;
     iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                  UT_TYPE_LONG,4,&lFlags);
    }


    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead,"keylogw","%d",lBk_w);
    utPltPutVarF(psDbHead,"keylogy","%d",lBk_y);
 
 //敏感网址归档
    sprintf(caTemp,"select count(*),flag from ncssenurllog  where 1=1 ");
    if(strlen(caGroups)>0){
        sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
    }
    if(strlen(caFacs_did)>0){
        sprintf(caTemp+strlen(caTemp)," and did in (%s) ",caFacs_did);
    }
    
     if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where (jointype='08' or jointype='09')) ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where jointype!='08') ");
      }
    
    }
    
    
     sprintf(caTemp+strlen(caTemp)," group by flag ");
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
     lCount=0;
     lFlags=0;
     iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                  UT_TYPE_LONG,4,&lFlags);
     lBk_w=0;
     lBk_y=0;
                                               
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
        if(lFlags==0){
            lBk_w=lCount;
        }
        else{
            lBk_y=lBk_y+lCount;
        }
        
     lCount=0;
     lFlags=0;
     iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                  UT_TYPE_LONG,4,&lFlags);
    }


    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead,"surllogw","%d",lBk_w);
    utPltPutVarF(psDbHead,"surllogy","%d",lBk_y);
 
 
  //敏感虚拟身份归档
    sprintf(caTemp,"select count(*),flag from ncssennetidlog  where 1=1 ");
    if(strlen(caGroups)>0){
        sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
    }
    if(strlen(caFacs_did)>0){
        sprintf(caTemp+strlen(caTemp)," and did in (%s) ",caFacs_did);
    }
    if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where (jointype='08' or jointype='09')) ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where jointype!='08') ");
      }
    
    }
    
    
    
     sprintf(caTemp+strlen(caTemp)," group by flag ");
    
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
     lCount=0;
     lFlags=0;
     iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                  UT_TYPE_LONG,4,&lFlags);
     lBk_w=0;
     lBk_y=0;
                                               
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
        if(lFlags==0){
            lBk_w=lCount;
        }
        else{
            lBk_y=lBk_y+lCount;
        }
        
     lCount=0;
     lFlags=0;
     iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                  UT_TYPE_LONG,4,&lFlags);
    }


    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead,"snetidlogw","%d",lBk_w);
    utPltPutVarF(psDbHead,"snetidlogy","%d",lBk_y);
 
   //重点用户归档
    sprintf(caTemp,"select count(*),flag from ncssenuserlog  where 1=1 ");
    if(strlen(caGroups)>0){
        sprintf(caTemp+strlen(caTemp)," and tid in (%s) ",caGroups);
    }
    if(strlen(caFacs_did)>0){
        sprintf(caTemp+strlen(caTemp)," and did in (%s) ",caFacs_did);
    }
    if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where (jointype='08' or jointype='09')) ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where jointype!='08') ");
      }
    
    }
    
    
     sprintf(caTemp+strlen(caTemp)," group by flag ");
     
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
     lCount=0;
     lFlags=0;
     iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                  UT_TYPE_LONG,4,&lFlags);
     lBk_w=0;
     lBk_y=0;
                                               
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
        if(lFlags==0){
            lBk_w=lCount;
        }
        else{
            lBk_y=lBk_y+lCount;
        }
        
     lCount=0;
     lFlags=0;
     iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                  UT_TYPE_LONG,4,&lFlags);
    }


    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead,"suserlogw","%d",lBk_w);
    utPltPutVarF(psDbHead,"suserlogy","%d",lBk_y);
 
    long iMaxCorp,iMaxOnline,iMaxCorp1;
    ncsClient *psUser;
    ncsOnline *psOnline;
    psUser = (ncsClient *)utShmArray(psShmHead,NCS_LNK_CLIENT);
    if(psUser == NULL) {
        
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","在线单位","memory Error ");
        return 0;
    }
   iMaxOnline=0;
   iMaxCorp1=0;
   iMaxCorp = utShmArrayRecord(psShmHead,NCS_LNK_CLIENT);
   
   //取单位信息
    sprintf(caTemp,"select userid,groupid from ncsuser where 1=1  ");
    
     if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and (jointype='08' or jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and jointype!='08' ");
      }
    
    }
   
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
     lUserid=0;
     lGroupid=0;

                                                  
         iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lUserid,
                                  UT_TYPE_LONG,4,&lGroupid);                              
 //printf("pid=%d\n",getpid());
 // sleep(15);  
        ncsClient *psClient;                                       
    iNum = 0;                                   
    while(iReturn == 0||iReturn==1405) {
            if(strlen(caGroups)>0&&isInGroup(caGroups,lGroupid)==0){
                   goto nexrec;
               }
          if(strlen(caFacs)>0&&isInGroup(caFacs_did,lUserid)==0){
              goto nexrec;
          }
  
                 iMaxCorp1++;
       psData_factoid=(struct corpfactoid_s *)pasLHashLook(pHash_factoid,&lUserid); 
       psOnline =  (ncsOnline *)ncsUtlGetOnlineById(psShmHead,lUserid);
       psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lUserid);  
       if(psOnline&&psClient){
       if(psClient->status==1||psClient->status==3){
           iMaxOnline++;
           if(psData_factoid){
               memset(caCode,0,sizeof(caCode));
               strcpy(caCode,psData_factoid->caCode);
               psData_fac=(struct corpfac*)pasLHashLook(pHash_fac,caCode);
               if(psData_fac){
                   psData_fac->lZx++;
           }
       }
  }
       else{
               psData_wh=(struct corpwh_s *)pasLHashLook(pHash_wh,&lUserid); 
                    if(psData_wh){
                           if(psData_factoid){
                                      memset(caCode,0,sizeof(caCode));
                                           strcpy(caCode,psData_factoid->caCode);
                                           psData_fac=(struct corpfac*)pasLHashLook(pHash_fac,caCode);
                                   if(psData_wh->flag==5){
                                       psData_fac->lWh++;
                                   }
                                   else{
                                       psData_fac->lTy++;
                                   }
                           }
                                    if(psData_wh->flag==5){
                                        wh_count++;
                                    }
                                    else{
                                        ty_count++;
                                    }
                  }
       }
  }
      else{
                 psData_wh=(struct corpwh_s *)pasLHashLook(pHash_wh,&lUserid); 
                    if(psData_wh){
                                if(psData_factoid){
                                              memset(caCode,0,sizeof(caCode));
                                                   strcpy(caCode,psData_factoid->caCode);
                                                   psData_fac=(struct corpfac*)pasLHashLook(pHash_fac,caCode);
                                           if(psData_wh->flag==5){
                                               psData_fac->lWh++;
                                           }
                                           else{
                                               psData_fac->lTy++;
                                           }
                                   }
                                    if(psData_wh->flag==5){
                                        wh_count++;
                                    }
                                    else{
                                        ty_count++;
                                    }
                  }
          
      }
          
          
          
          
          
          
          
          
          
          
          
          
          
          
        nexrec:
     lUserid=0;
     lGroupid=0;
         iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lUserid,
                                  UT_TYPE_LONG,4,&lGroupid);  
    }


    pasDbCloseCursor(psCur);
   
   
   
   
   
   

//   printf("iMaxOnline=%d\n",iMaxOnline);
//   iMaxOnline = utShmHashRecord(psShmHead,NCS_LNK_ONLINE);
    utPltPutVarF(psDbHead,"lxcorp","%d",iMaxCorp1-iMaxOnline-wh_count-ty_count);
    utPltPutVarF(psDbHead,"zxcorp","%d",iMaxOnline);
    utPltPutVarF(psDbHead,"whcorp","%d",wh_count);
    utPltPutVarF(psDbHead,"tycorp","%d",ty_count);
 //输出接入厂商场所信息
    psData_fac=(struct corpfac*)pasLHashFirst(pHash_fac,&sHashInfo);
    iNum=0;
    while(psData_fac){
        utPltPutLoopVar(psDbHead,"facname",iNum+1,psData_fac->caName);
        utPltPutLoopVarF(psDbHead,"faczx",iNum+1,"%d",psData_fac->lZx);
        utPltPutLoopVarF(psDbHead,"facwh",iNum+1,"%d",psData_fac->lWh);
        utPltPutLoopVarF(psDbHead,"facty",iNum+1,"%d",psData_fac->lTy);
        utPltPutLoopVarF(psDbHead,"faclx",iNum+1,"%d",psData_fac->lSum-psData_fac->lZx-psData_fac->lWh-psData_fac->lTy);
        utPltPutLoopVar(psDbHead,"fccode",iNum+1,psData_fac->caCode);
        iNum++;
        psData_fac=(struct areastatdata_s *)pasLHashNext(&sHashInfo);
    }
 
 
 
 
    free(pHash_wh);
    free(pHash_fac);
    free(pHash_factoid);
    
    
    
 //统计某天最大虚拟身份数和上网日志数
       iReturn = utMsgGetSomeNVar(psMsgHead,2,
            "sdate",UT_TYPE_STRING,19,caSdate,
            "edate",UT_TYPE_STRING,19,caEdate);
   
      lTime=time(0);
   ncUtlMonthDate(lTime,caSdate_d,caEdate_d);
   if(strlen(caSdate)==0){
               strcpy(caSdate,caSdate_d);
    }
    if(strlen(caEdate)==0){
        strcpy(caEdate,caEdate_d);
    }
       
    sprintf(caStemptime,"%s 00:00",caSdate);
    lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
    sprintf(caStemptime,"%s 23:59",caEdate);
    letime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);   
    strcpy(caSdate,utTimFormat("%Y-%m-%d",lstime));
    strcpy(caEdate,utTimFormat("%Y-%m-%d",letime));
    
    lMax=100;
     sprintf(caTemp,"select sdate,sum(lcount) aaa from ncsuserlogstatday where sdate>='%s' and sdate<='%s' ",caSdate,caEdate);
         if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where (jointype='08' or jointype='09')) ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where jointype!='08') ");
      }
    
    }
     
     sprintf(caTemp+strlen(caTemp)," group by sdate order by aaa desc limit 0,1 ");
//     printf("caTemp=%s\n",caTemp);
     pasDbOneRecord(caTemp,0,UT_TYPE_STRING,10,caSdate,
                             UT_TYPE_LONG,4,&lCount);
     if(lCount>lMax){
         lMax=lCount;
    }
     sprintf(caTemp,"select sdate,sum(lcount) aaa from ncsnetidstatday where sdate>='%s' and sdate<='%s' group by sdate order by aaa desc limit 0,1 ",caSdate,caEdate);    
//     printf("caTemp=%s\n",caTemp);     
     pasDbOneRecord(caTemp,0,UT_TYPE_STRING,10,caSdate,
                             UT_TYPE_LONG,4,&lCount);
     if(lCount>lMax){
           lMax=lCount;
     }
       lTemp=(lMax*1.3/100)*100;
       
       if(lTemp<=100) lTemp=1000;
       utPltPutVarF(psDbHead,"maxaix","%d",lTemp);
    
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_zhtj.html");
    return 0;
} 



/* 定时统计虚拟身份                                          */
int ncsStatNetid_v4(utShmHead *psShmHead)
{
    unsigned long lStopTime,lStartTime,lTime,lBackupTime;
    pasDbCursor *psCur;
    char caTemp[1024];
    char caTime[20];
    long lCount;
    int iReturn;
    int i;
    unsigned long temp_id,temp_ip;
    long long lLastid,lMaxsid;
    unsigned long lLastime;
   
    pasDbConn *psConn;
  
 struct data_s{
         char caServicecode[16];
         char caUsername[32];
         char caDispname[32];
         char caMac[20];
         char caPosition[32];
         char caIdno[20];
         char caIdtype[4];
         long did;
         long uid;
         long tid;
         char caSip[16];
         long ntype;
         char caName[64];
         long starttime;
         long  lasttime;
         long long sid;
    };
  struct data_s *pData;
  long long lSid;
  long lDid,lTid,lUid,lNtype,lMinstime,lMaxstime,iNum;
  char caServicecode[16],caUsername[32],caDispname[32],caMac[20],caPosition[32],caIdno[20];
  char caIdtype[8],caSip[16],caName[64],caTableday[20];
  lTime=time(0);
  strcpy(caTableday,utTimFormat("%Y%m",lTime));
  pData=(struct data_s*)malloc((50000)*sizeof(struct data_s));
  if(pData==NULL) return;
    lLastid=0;
   ncsUtlGetLastInfo_v4("NetidStat",&lLastid);

     sprintf(caTemp,"select sid,servicecode,username,dispname,mac,position,idno,idtype,did,tid,uid,sip,ntype,name,stime from ncsnetlog_if_%s where sid>%lld  order by sid limit 0,49999 ",caTableday,lLastid);
     
 //    printf("caTemp=%s\n",caTemp);

//printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
        printf("open table ncsnetlog error\n");
        free(pData);
        return 0;
    }
 
    lSid=0;
    memset(caServicecode,0,sizeof(caServicecode));
    memset(caUsername,0,sizeof(caUsername));
    memset(caDispname,0,sizeof(caDispname));
    memset(caMac,0,sizeof(caMac));
    memset(caPosition,0,sizeof(caPosition));
    memset(caIdno,0,sizeof(caIdno));
    memset(caIdtype,0,sizeof(caIdtype));
    lDid=0;
    lTid=0;
    lUid=0;
    memset(caSip,0,sizeof(caSip));
    lNtype=0;
    memset(caName,0,sizeof(caName));
    lMinstime=0;
    lMaxstime=0;
    iReturn = pasDbFetchInto(psCur,  UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_STRING,14,caServicecode,
                                     UT_TYPE_STRING,31,caUsername,
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lTid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_LONG,4,&lNtype,
                                     UT_TYPE_STRING,63,caName,
                                     UT_TYPE_LONG,4,&lMaxstime);
                                    
     iNum=0;                         
                                     
      lLastid=0;
      while((iReturn==0)||(iReturn==1405)){
      if(strlen(caName)<30&&lMaxstime<lTime+360000){
          
          pData[iNum].sid=lSid;
          strcpy(pData[iNum].caServicecode,caServicecode);
          strcpy(pData[iNum].caUsername,caUsername);
          strcpy(pData[iNum].caDispname,caDispname);
          strcpy(pData[iNum].caMac,caMac);
          strcpy(pData[iNum].caPosition,caPosition);
          strcpy(pData[iNum].caIdno,caIdno);
          strcpy(pData[iNum].caIdtype,caIdtype);
          pData[iNum].did=lDid;
          pData[iNum].uid=lUid;
          pData[iNum].tid=lTid;
          strcpy(pData[iNum].caSip,caSip);
          pData[iNum].ntype=lNtype;
          strcpy(pData[iNum].caName,caName);
          pData[iNum].starttime=lMinstime;
          pData[iNum].lasttime=lMaxstime;
          lLastid=lSid;
          iNum++;
          

         }

    lSid=0;
    memset(caServicecode,0,sizeof(caServicecode));
    memset(caUsername,0,sizeof(caUsername));
    memset(caDispname,0,sizeof(caDispname));
    memset(caMac,0,sizeof(caMac));
    memset(caPosition,0,sizeof(caPosition));
    memset(caIdno,0,sizeof(caIdno));
    memset(caIdtype,0,sizeof(caIdtype));
    lDid=0;
    lTid=0;
    lUid=0;
    memset(caSip,0,sizeof(caSip));
    lNtype=0;
    memset(caName,0,sizeof(caName));
    lMinstime=0;
    lMaxstime=0;
    iReturn = pasDbFetchInto(psCur,  UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_STRING,14,caServicecode,
                                     UT_TYPE_STRING,31,caUsername,
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lTid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_LONG,4,&lNtype,
                                     UT_TYPE_STRING,63,caName,
                                     UT_TYPE_LONG,4,&lMaxstime);
                                 
      }                                 
         pasDbCloseCursor(psCur);

        if(lLastid>0){
                 ncsUtlSetLastInfo_v4("NetidStat",lLastid); 
         }                          
unsigned long lSid_t;
       for(i=0;i<iNum;i++){
           //生成id
           sprintf(caTemp,"%s_%d_%d_%d",pData[i].caName,pData[i].ntype,pData[i].did,pData[i].uid);
           lSid_t=utMd5Code(caTemp,strlen(caTemp),"ffff");
            sprintf(caTemp,"select count(*) from ncsnetid_if where ndbsid=%lu ",lSid_t);
 //           printf("lSid=%lu,caTemp=%s\n",lSid_t,caTemp);
            lCount=0;
            iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount); 
            if(iReturn==1146){
//创建新表
          sprintf(caTemp,"create table ncsnetid_if(");
          strcat(caTemp,"servicecode      char(14),");
          strcat(caTemp,"username           char(32),");
          strcat(caTemp,"dispname           char(32),");
          strcat(caTemp,"mac                 char(18),");
          strcat(caTemp,"position           char(32),");
          strcat(caTemp,"idno                char(18),");
          strcat(caTemp,"idtype              char(3),");
          strcat(caTemp,"did                  int,");
          strcat(caTemp,"tid            int,");
          strcat(caTemp,"uid            int,");
          strcat(caTemp,"sip               char(15),");
          strcat(caTemp,"ntype          int  ,");
          strcat(caTemp,"name           char(64),");
          strcat(caTemp,"starttime         int,");
          strcat(caTemp,"lasttime             int,");
          strcat(caTemp,"ndbsid            bigint)engine=MyISAM;");
  //        printf("caTemp=%s\n",caTemp);
           pasDbExecSql(caTemp,0);
           pasDbExecSqlF("create index ncsnetid_if_ind1 on ncsnetid_if (ndbsid); ");
           pasDbExecSqlF("create index ncsnetid_if_ind2 on ncsnetid_if (ntype); ");             
           pasDbExecSqlF("create index ncsnetid_if_ind3 on ncsnetid_if (tid); "); 
           pasDbExecSqlF("create index ncsnetid_if_ind4 on ncsnetid_if (did); ");  
           pasDbExecSqlF("create index ncsnetid_if_ind5 on ncsnetid_if (lasttime); "); 
           pasDbExecSqlF("create index ncsnetid_if_ind6 on ncsnetid_if (name); "); 
            }
            if(lCount>0){
           
 //               sprintf(caTemp,"update ncsnetid_if_%s set servicecode='%s',username='%s',dispname='%s',mac='%s',position='%s',idno='%s',idtype='%s',did=%d,tid=%d,uid=%d,sip='%s',ntype=%d,name='%s',lasttime=%d where ndbsid=%lu \n", \
 //                    caTableday,pData[i].caServicecode,pData[i].caUsername,pData[i].caDispname,pData[i].caMac,pData[i].caPosition,pData[i].caIdno,pData[i].caIdtype,pData[i].did,pData[i].tid,pData[i].uid,pData[i].caSip,pData[i].ntype,pData[i].caName,pData[i].lasttime,lSid_t);
  //        printf("caTemp=%s\n",caTemp);              
                   pasDbExecSqlF("update ncsnetid_if set servicecode='%s',username='%s',dispname='%s',mac='%s',position='%s',idno='%s',idtype='%s',did=%d,tid=%d,uid=%d,sip='%s',ntype=%d,name='%s',lasttime=%d where ndbsid=%lu ",
                     pData[i].caServicecode,pData[i].caUsername,pData[i].caDispname,pData[i].caMac,pData[i].caPosition,pData[i].caIdno,pData[i].caIdtype,pData[i].did,pData[i].tid,pData[i].uid,pData[i].caSip,pData[i].ntype,pData[i].caName,pData[i].lasttime,lSid_t);
                     
               }
               else{
                    sprintf(caTemp,"insert into ncsnetid_if(servicecode,username,dispname,mac,position,idno,idtype,did,tid,uid,sip,ntype,name,starttime,lasttime,ndbsid) \
                                                              values('%s','%s','%s','%s','%s','%s','%s',%d,%d,%d,'%s',%d,'%s',%d,%d,%lu) ",
                                               pData[i].caServicecode,pData[i].caUsername,pData[i].caDispname,pData[i].caMac,pData[i].caPosition,pData[i].caIdno,pData[i].caIdtype,pData[i].did,pData[i].tid,pData[i].uid,pData[i].caSip,pData[i].ntype,pData[i].caName,pData[i].lasttime,pData[i].lasttime,lSid_t);
                                              
                   pasDbExecSql(caTemp,0);
               }
       }
    
    
    
    free(pData);
    return 0;
}


//显示运行状态直观图
int ncsWebDispXxztZgt_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    int iReturn;

    struct Areadata_s{
          unsigned long lGroupid;  //单位ID
         char caGroupname[32];
         char caGroups[2048];        //放置这个区域下的子区域
         long lSum_did;
         long lSum_on;
         long lSum_wh;
         long lSum_ty;
     };
  struct Areadata_s *pArea;
  char caServicestate[16];
   pasDbCursor *psCur;
   long lGroupid;
   char caGroupname[36];
   char caTemp[4096];
   long lSumArea,i,iNum_2,iNum_3,m,lUserid,n;
   char caDispname[36];
   char caUsername[36];
   ncsOnline  *psOnline;
   ncsClient * psClient;
   long lSum_g_zx,lSum_g_lx,lSum_g_wh,lSum_g_ty;
   long lSum_zx,lSum_lx,lSum_wh,lSum_ty;
   char caGroupid[16];
  char caGroups[1024];
  char caFacs[1024];
  char caLb[32],caFcode[32];
  long iNum_1;
  char caWireflag[16];
     strcpy(caFacs,getDsFacDid());
   strcpy(caGroups,getDsGroupid());
   
   lSum_zx=0;
   lSum_lx=0;
   lSum_wh=0;
   lSum_ty=0;
   psDbHead = utPltInitDb(); 

 utMsgPrintMsg(psMsgHead);
 utMsgGetSomeNVar(psMsgHead,4,
                    "groupid",      UT_TYPE_STRING, 10, caGroupid,
                    "lb",           UT_TYPE_STRING,10,caLb,
                    "fcode",        UT_TYPE_STRING,10,caFcode,
                    "wireflag",     UT_TYPE_STRING,10,caWireflag);
   pArea=(struct Areadata_s *)malloc(1000*sizeof(struct Areadata_s ));
   if(pArea==NULL){
   
       return -1;
  }

  sprintf(caTemp,"select groupid,groupname from ncsgroup where 1=1 ");

  if(strlen(caGroupid)>0){
      sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",getGroupid(atol(caGroupid)));
  }
  else{
      if(strlen(caGroups)==0){
        sprintf(caTemp+strlen(caTemp)," and pid=0 limit 0,999");
      }
      
  }
  if(strlen(caGroups)>0){
      sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caGroups);
  }
  printf("caTemp=%s\n",caTemp);
  psCur = pasDbOpenSql(caTemp,0);
  if(psCur == NULL) {
    
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
   }
  lGroupid=0;
  memset(caGroupname,0,sizeof(caGroupname));
  iReturn = pasDbFetchInto(psCur,                                     
                                       UT_TYPE_LONG,4,&lGroupid,
                                       UT_TYPE_STRING,31,caGroupname);
   lSumArea=0;

      while((iReturn==0)||(iReturn==1405))
    {
          pArea[lSumArea].lGroupid=lGroupid;
          strcpy(pArea[lSumArea].caGroupname,caGroupname);
               lSumArea++;
                  lGroupid=0;
                  memset(caGroupname,0,sizeof(caGroupname));
                  iReturn = pasDbFetchInto(psCur,                                     
                                                       UT_TYPE_LONG,4,&lGroupid,
                                                       UT_TYPE_STRING,31,caGroupname);                                  
    }    
    pasDbCloseCursor(psCur); 
   iNum_1=0;
   for(i=0;i<lSumArea;i++){
        if(strlen(caGroups)==0){
            strcpy(pArea[i].caGroups,getGroupid(pArea[i].lGroupid));
        }
        else{
         sprintf(pArea[i].caGroups,"%d",pArea[i].lGroupid);
        }
 //       strcpy(pArea[i].caGroups,getGroupid(pArea[i].lGroupid));

        lSum_g_zx=0;
        lSum_g_lx=0;
        lSum_g_wh=0;
        lSum_g_ty=0;
        if(strlen(pArea[i].caGroups)>0){
              iNum_2=0;
              iNum_3=0;
              m=0;
                sprintf(caTemp,"select userid,dispname,username,servicestate from ncsuser where groupid in (%s) ",pArea[i].caGroups);
                if(strlen(caGroups)>0){
                    sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caGroups);
                }
                if(strlen(caFacs)>0){
                    sprintf(caTemp+strlen(caTemp)," and userid in (%s) ",caFacs);
                }
        if(strlen(caLb)>0){
            sprintf(caTemp+strlen(caTemp)," and gtype=%s ",caLb);
        }
        if(strlen(caFcode)>0){
            sprintf(caTemp+strlen(caTemp)," and fcode='%s' ",caFcode);
        }
      if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and (jointype='08' or jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and jointype!='08' ");
      }
    
    }
        printf("caTemp=%s\n",caTemp);
                lUserid=0;
                memset(caDispname,0,sizeof(caDispname));
                memset(caUsername,0,sizeof(caUsername));
                memset(caServicestate,0,sizeof(caServicestate));
                 psCur = pasDbOpenSql(caTemp,0);
                iReturn = pasDbFetchInto(psCur,                                     
                                       UT_TYPE_LONG,4,&lUserid,
                                       UT_TYPE_STRING,31,caDispname,
                                       UT_TYPE_STRING,20,caUsername,
                                       UT_TYPE_STRING,2,caServicestate);

                while((iReturn==0)||(iReturn==1405))
                    {

                  iNum_2=m/7;
                  iNum_3=m%7;
             
                  utPltPutLoopVarF3(psDbHead,"did",iNum_1+1,iNum_2+1,iNum_3+1,"%d",lUserid);
                  utPltPutLoopVar3(psDbHead,"cname",iNum_1+1,iNum_2+1,iNum_3+1,caDispname);
                  utPltPutLoopVar3(psDbHead,"username",iNum_1+1,iNum_2+1,iNum_3+1,caUsername);
                                    
                                    psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lUserid);          
                  psOnline =  (ncsOnline *)ncsUtlGetOnlineById(psShmHead,lUserid);
                                    if(psClient) {
                     if(psClient->status==1||psClient->status==3){ 
                         if(psOnline){
 //在线  
                        utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/zxzc.gif>");
                     lSum_g_zx++; 
                     lSum_zx++; 
                         }
                         else{
//离线                   
                      if(strcmp(caServicestate,"2")==0||strcmp(caServicestate,"3")==0||strcmp(caServicestate,"4")==0){
                          utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/zdbj.gif>");
                          lSum_g_ty++;
                          lSum_ty++;
                      }    
                      else if(strcmp(caServicestate,"5")==0){
                          lSum_g_wh++;
                          lSum_wh++;
                        utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/ztyy.gif>");
                      }
                      else{
                          lSum_g_lx++;
                          lSum_lx++;
                          utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/jjbj.gif>");
                      }
                         }
                     } 
                     else{
                            if(strcmp(caServicestate,"2")==0||strcmp(caServicestate,"3")==0||strcmp(caServicestate,"4")==0){
                                lSum_g_ty++;
                                lSum_ty++;
                          utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/zdbj.gif>");
                      }    
                      else if(strcmp(caServicestate,"5")==0){
                          lSum_g_wh++;
                          lSum_wh++;
                        utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/ztyy.gif>");
                      }
                      else{
                          lSum_g_lx++;
                          lSum_lx++;
                          utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/jjbj.gif>");
                      }
                         
                     }
                                    }
                                    
                                    
                                    
                                    m++;
                                    lUserid=0;
                                    memset(caDispname,0,sizeof(caDispname));
                                    memset(caUsername,0,sizeof(caUsername));
                                    memset(caServicestate,0,sizeof(caServicestate));
                iReturn = pasDbFetchInto(psCur,                                     
                                       UT_TYPE_LONG,4,&lUserid,
                                       UT_TYPE_STRING,31,caDispname,
                                       UT_TYPE_STRING,20,caUsername,
                                       UT_TYPE_STRING,2,caServicestate);                            
                   }
                    pasDbCloseCursor(psCur);                              

                    if(m>0){
                     for(n=iNum_3;n<7;n++){                                     
               utPltPutLoopVar3(psDbHead,"cname",iNum_1+1,iNum_2+1,n+1,"");
                     }
                           utPltPutLoopVarF(psDbHead,"gzx",iNum_1+1,"%d",lSum_g_zx);
                           utPltPutLoopVarF(psDbHead,"glx",iNum_1+1,"%d",lSum_g_lx);
                           utPltPutLoopVarF(psDbHead,"gwh",iNum_1+1,"%d",lSum_g_wh);
                           utPltPutLoopVarF(psDbHead,"gty",iNum_1+1,"%d",lSum_g_ty);                           
                          utPltPutLoopVarF(psDbHead,"groupid",iNum_1+1,"%d",pArea[i].lGroupid);
                     utPltPutLoopVar(psDbHead,"groupname",iNum_1+1,pArea[i].caGroupname);
                     iNum_1++;
         }
                   
         }
        
        
   
   }


//统计其它单位
//i++;
  if(strlen(caGroupid)==0){

            lSum_g_zx=0;
            lSum_g_lx=0;
            lSum_g_wh=0;
            lSum_g_ty=0;


              iNum_2=0;
              iNum_3=0;
              m=0;
                sprintf(caTemp,"select userid,dispname,username,servicestate from ncsuser where groupid not in (select groupid from ncsgroup ) ");
                if(strlen(caGroups)>0){
                    sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caGroups);
                }
                if(strlen(caFacs)>0){
                    sprintf(caTemp+strlen(caTemp)," and userid in (%s) ",caFacs);
                }
        if(strlen(caLb)>0){
            sprintf(caTemp+strlen(caTemp)," and gtype=%s ",caLb);
        }
        if(strlen(caFcode)>0){
            sprintf(caTemp+strlen(caTemp)," and fcode='%s' ",caFcode);
        }
     if(strlen(caWireflag)>0){
        if(strcmp(caWireflag,"2")==0){
           sprintf(caTemp+strlen(caTemp)," and (jointype='08' or jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
          sprintf(caTemp+strlen(caTemp)," and jointype!='08' ");
      }
    
    }
        
                lUserid=0;
                memset(caDispname,0,sizeof(caDispname));
                memset(caUsername,0,sizeof(caUsername));
                memset(caServicestate,0,sizeof(caServicestate));
                 psCur = pasDbOpenSql(caTemp,0);
                iReturn = pasDbFetchInto(psCur,                                     
                                       UT_TYPE_LONG,4,&lUserid,
                                       UT_TYPE_STRING,31,caDispname,
                                       UT_TYPE_STRING,20,caUsername,
                                       UT_TYPE_STRING,2,caServicestate);

                while((iReturn==0)||(iReturn==1405))
                    {
                  iNum_2=m/7;
                  iNum_3=m%7;
             
                  utPltPutLoopVarF3(psDbHead,"did",iNum_1+1,iNum_2+1,iNum_3+1,"%d",lUserid);
                  utPltPutLoopVar3(psDbHead,"cname",iNum_1+1,iNum_2+1,iNum_3+1,caDispname);
                  utPltPutLoopVar3(psDbHead,"username",iNum_1+1,iNum_2+1,iNum_3+1,caUsername);
                                    
                                    psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lUserid);          
                  psOnline =  (ncsOnline *)ncsUtlGetOnlineById(psShmHead,lUserid);
                                    if(psClient) {
                     if(psClient->status==1||psClient->status==3){ 
                         if(psOnline){
 //在线  
                        utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/zxzc.gif>");
                     lSum_g_zx++; 
                     lSum_zx++; 
                         }
                         else{
//离线                   
                      if(strcmp(caServicestate,"2")==0||strcmp(caServicestate,"3")==0||strcmp(caServicestate,"4")==0){
                          utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/zdbj.gif>");
                          lSum_g_ty++;
                          lSum_ty++;
                      }    
                      else if(strcmp(caServicestate,"5")==0){
                          lSum_g_wh++;
                          lSum_wh++;
                        utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/ztyy.gif>");
                      }
                      else{
                          lSum_g_lx++;
                          lSum_lx++;
                          utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/jjbj.gif>");
                      }
                         }
                     } 
                     else{
                            if(strcmp(caServicestate,"2")==0||strcmp(caServicestate,"3")==0||strcmp(caServicestate,"4")==0){
                                lSum_g_ty++;
                                lSum_ty++;
                          utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/zdbj.gif>");
                      }    
                      else if(strcmp(caServicestate,"5")==0){
                          lSum_g_wh++;
                          lSum_wh++;
                        utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/ztyy.gif>");
                      }
                      else{
                          lSum_g_lx++;
                          lSum_lx++;
                          utPltPutLoopVar3(psDbHead,"img",iNum_1+1,iNum_2+1,iNum_3+1,"<img src=/images/info/jjbj.gif>");
                      }
                         
                     }
                                    }
                                    
                                    
                                    
                                    m++;
                                    lUserid=0;
                                    memset(caDispname,0,sizeof(caDispname));
                                    memset(caUsername,0,sizeof(caUsername));
                                    memset(caServicestate,0,sizeof(caServicestate));
                iReturn = pasDbFetchInto(psCur,                                     
                                       UT_TYPE_LONG,4,&lUserid,
                                       UT_TYPE_STRING,31,caDispname,
                                       UT_TYPE_STRING,20,caUsername,
                                       UT_TYPE_STRING,2,caServicestate);                            
                   }
                    pasDbCloseCursor(psCur);  
                if(lSum_g_zx>0||lSum_g_lx>0||lSum_g_wh>0||lSum_g_ty>0){                            
                    for(n=iNum_3;n<7;n++){                                     
               utPltPutLoopVar3(psDbHead,"cname",iNum_1+1,iNum_2+1,n+1,"");
                    }
                    
                   utPltPutLoopVarF(psDbHead,"groupid",iNum_1+1,"%d",99999999);
              utPltPutLoopVar(psDbHead,"groupname",iNum_1+1,"其它");
                   utPltPutLoopVarF(psDbHead,"gzx",iNum_1+1,"%d",lSum_g_zx);
                   utPltPutLoopVarF(psDbHead,"glx",iNum_1+1,"%d",lSum_g_lx);
                   utPltPutLoopVarF(psDbHead,"gwh",iNum_1+1,"%d",lSum_g_wh);
                   utPltPutLoopVarF(psDbHead,"gty",iNum_1+1,"%d",lSum_g_ty);

        }



      }









   free(pArea);
         utPltPutVarF(psDbHead,"zx","%d",lSum_zx);
         utPltPutVarF(psDbHead,"lx","%d",lSum_lx);
         utPltPutVarF(psDbHead,"wh","%d",lSum_wh);
         utPltPutVarF(psDbHead,"ty","%d",lSum_ty);
         utPltPutVarF(psDbHead,"tot","%d",lSum_zx+lSum_lx+lSum_wh+lSum_ty);
//   utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_yxzt_zgt.htm");
    return 0;
} 


/* 定时统计用户信息                                          */
int ncsStatEnduser_v4(utShmHead *psShmHead)
{
    unsigned long lStopTime,lStartTime,lTime,lBackupTime;
    pasDbCursor *psCur;
    char caTemp[1024];
    char caTime[20];
    long lCount;
    int iReturn;
    int i;
    unsigned long temp_id,temp_ip;
    long long lLastid,lMaxsid;
    unsigned long lLastime;
   
    pasDbConn *psConn;
  
 struct data_s{
         char caUsername[32];
         char caDispname[32];
         char caMac[20];
         char caPosition[32];
         char caIdno[20];
         char caIdtype[4];
         long did;
         long uid;
         long tid;
         char caSip[16];
         long  lasttime;
         long long sid;
    };
  struct data_s *pData;
  long long lSid;
  long lDid,lTid,lUid,lNtype,lMinstime,lMaxstime,iNum;
  char caServicecode[16],caUsername[32],caDispname[32],caMac[20],caPosition[32],caIdno[20];
  char caIdtype[8],caSip[16],caName[64],caTableday[20];
  lTime=time(0);
  strcpy(caTableday,utTimFormat("%Y%m",lTime));
  pData=(struct data_s*)malloc((50000)*sizeof(struct data_s));
  if(pData==NULL) return;
    lLastid=0;
   ncsUtlGetLastInfo_v4("UserlogStat",&lLastid);

     sprintf(caTemp,"select sid,username,dispname,mac,position,idno,idtype,did,tid,uid,sip,max(stime) from ncsuserlog_if_%s where sid>%lld group by did,uid order by sid limit 0,49999 ",caTableday,lLastid);
     
 //    printf("caTemp=%s\n",caTemp);

//printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
        printf("open table ncsnetlog error\n");
        free(pData);
        return 0;
    }
 
    lSid=0;
    memset(caServicecode,0,sizeof(caServicecode));
    memset(caUsername,0,sizeof(caUsername));
    memset(caDispname,0,sizeof(caDispname));
    memset(caMac,0,sizeof(caMac));
    memset(caPosition,0,sizeof(caPosition));
    memset(caIdno,0,sizeof(caIdno));
    memset(caIdtype,0,sizeof(caIdtype));
    lDid=0;
    lTid=0;
    lUid=0;
    memset(caSip,0,sizeof(caSip));
    lNtype=0;
    memset(caName,0,sizeof(caName));
    lMinstime=0;
    lMaxstime=0;
    iReturn = pasDbFetchInto(psCur,  UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_STRING,31,caUsername,
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lTid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_LONG,4,&lMaxstime);
                                    
     iNum=0;                         
                                     
lLastid=0;
      while((iReturn==0)||(iReturn==1405)){
      if((lMaxstime<lTime+360000)&&strlen(caUsername)>0&&strlen(caDispname)>0){
          
          pData[iNum].sid=lSid;
          strcpy(pData[iNum].caUsername,caUsername);
          strcpy(pData[iNum].caDispname,caDispname);
          strcpy(pData[iNum].caMac,caMac);
          strcpy(pData[iNum].caPosition,caPosition);
          strcpy(pData[iNum].caIdno,caIdno);
          strcpy(pData[iNum].caIdtype,caIdtype);
          pData[iNum].did=lDid;
          pData[iNum].uid=lUid;
          pData[iNum].tid=lTid;
          strcpy(pData[iNum].caSip,caSip);

          pData[iNum].lasttime=lMaxstime;
          lLastid=lSid;
          iNum++;
          

         }

    lSid=0;
    memset(caServicecode,0,sizeof(caServicecode));
    memset(caUsername,0,sizeof(caUsername));
    memset(caDispname,0,sizeof(caDispname));
    memset(caMac,0,sizeof(caMac));
    memset(caPosition,0,sizeof(caPosition));
    memset(caIdno,0,sizeof(caIdno));
    memset(caIdtype,0,sizeof(caIdtype));
    lDid=0;
    lTid=0;
    lUid=0;
    memset(caSip,0,sizeof(caSip));
    lNtype=0;
    memset(caName,0,sizeof(caName));
    lMinstime=0;
    lMaxstime=0;
    iReturn = pasDbFetchInto(psCur,  UT_TYPE_LONG8,8,&lSid,
                                     UT_TYPE_STRING,31,caUsername,
                                     UT_TYPE_STRING,31,caDispname,
                                     UT_TYPE_STRING,19,caMac,
                                     UT_TYPE_STRING,31,caPosition,
                                     UT_TYPE_STRING,18,caIdno,
                                     UT_TYPE_STRING,3,caIdtype,
                                     UT_TYPE_LONG,4,&lDid,
                                     UT_TYPE_LONG,4,&lTid,
                                     UT_TYPE_LONG,4,&lUid,
                                     UT_TYPE_STRING,15,caSip,
                                     UT_TYPE_LONG,4,&lMaxstime);
                                 
      }                                 
         pasDbCloseCursor(psCur);

        if(lLastid>0){
                 ncsUtlSetLastInfo_v4("UserlogStat",lLastid); 
         }                          

       for(i=0;i<iNum;i++){

            sprintf(caTemp,"select count(*) from ncsenduser where did=%lu and uid=%lu ",lDid,lUid);
 //           printf("lSid=%lu,caTemp=%s\n",lSid_t,caTemp);
            lCount=0;
            iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount); 

            if(lCount>0){
 //               sprintf(caTemp,"update ncsnetid_if_%s set servicecode='%s',username='%s',dispname='%s',mac='%s',position='%s',idno='%s',idtype='%s',did=%d,tid=%d,uid=%d,sip='%s',ntype=%d,name='%s',lasttime=%d where ndbsid=%lu \n", \
 //                    caTableday,pData[i].caServicecode,pData[i].caUsername,pData[i].caDispname,pData[i].caMac,pData[i].caPosition,pData[i].caIdno,pData[i].caIdtype,pData[i].did,pData[i].tid,pData[i].uid,pData[i].caSip,pData[i].ntype,pData[i].caName,pData[i].lasttime,lSid_t);
  //        printf("caTemp=%s\n",caTemp);              
                   pasDbExecSqlF("update ncsenduser set username='%s',dispname='%s',mac='%s',position='%s',idcard='%s',idfytype='%s',ip=%lu,lasttime=%d where did=%lu and uid=%lu ",
                    pData[i].caUsername,pData[i].caDispname,pData[i].caMac,pData[i].caPosition,pData[i].caIdno,pData[i].caIdtype,ntohl(utComHostAddress(pData[i].caSip)),pData[i].lasttime,pData[i].did,pData[i].uid);
                     
               }
               else{
                    sprintf(caTemp,"insert into ncsenduser(username,dispname,mac,position,idcard,idfytype,did,gid,uid,ip,syntime,lasttime) \
                                                              values('%s','%s','%s','%s','%s','%s',%d,0,%d,%lu,%lu,%lu) ",
                                               pData[i].caUsername,pData[i].caDispname,pData[i].caMac,pData[i].caPosition,pData[i].caIdno,pData[i].caIdtype,pData[i].did,pData[i].uid,ntohl(utComHostAddress(pData[i].caSip)),pData[i].lasttime,pData[i].lasttime);
          //           printf("caTemp=%s\n",caTemp);                         
                   pasDbExecSql(caTemp,0);
               }
       }
    
    
    
    free(pData);
    return 0;
}


//敏感信息处理统计
int ncsMgcl_stat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[2048];
    char caEdate[20];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum;
    utPltDbHead *psDbHead;
       int iReturn;
       char caOpname[32];
       long lCount,lFlag;
        char caExport[32];
      FILE *fp;
        char caFilename[32];
        char caFile[128];
        char caPath[128];
    char sqlbuf[2048]="";

   long lStime,lEtime;
   char caSort[16],caDir[16];
     pasDbCursor *psCur;
 
  char caSdate_d[20],caEdate_d[20],caSdate_t[20];
  long  lSum=0,lSum_keyyh=0,lSum_keyxs=0,lSum_keydq=0,lSum_keytot=0,lSum_urlyh=0,lSum_urlxs=0,lSum_urldq=0,lSum_urltot=0;
  long lSum_vidyh=0,lSum_vidxs=0,lSum_viddq=0,lSum_vidtot=0,lSum_useyh=0,lSum_usexs=0,lSum_usedq=0,lSum_usetot=0;
  long lSum_bkyh=0,lSum_bkxs=0,lSum_bkdq=0,lSum_bktot=0;
    unsigned char *pHash;
    long index,lSumitem,lstime1;
  struct data_s stData[200];
  struct data_s *psData;
  pasLHashInfo sHashInfo;
  iReturn = utMsgGetSomeNVar(psMsgHead,5,
                     "sdate",          UT_TYPE_STRING,10,caSdate_d,
                     "edate",          UT_TYPE_STRING,10,caEdate_d,
                     "exp",            UT_TYPE_STRING,10,caExport,
                     "sort",           UT_TYPE_STRING,10,caSort,
                     "dir",            UT_TYPE_STRING,10,caDir);
   
    if(strcmp(caDir,"ASC")==0){
        iBy=1;
    }
    else{
        iBy=0;
    }
    if(strcmp(caSort,"keyyh")==0){
        iSort=2;
    }
    else if(strcmp(caSort,"keyxs")==0){
        iSort=3;
    }
    else if(strcmp(caSort,"keydq")==0){
        iSort=4;
    }
    else if(strcmp(caSort,"keytot")==0){
        iSort=5;
    }
    else if(strcmp(caSort,"urlyh")==0){
        iSort=6;
    }
    else if(strcmp(caSort,"urlxs")==0){
        iSort=7;
    }
    else if(strcmp(caSort,"urltot")==0){
        iSort=9;
    }
    else if(strcmp(caSort,"urldq")==0){
        iSort=8;
    }
    else if(strcmp(caSort,"vidyh")==0){
        iSort=10;
    }
    else if(strcmp(caSort,"vidxs")==0){
        iSort=11;
    }
    else if(strcmp(caSort,"viddq")==0){
        iSort=12;
    }
    else if(strcmp(caSort,"vidtot")==0){
        iSort=13;
    }
    else if(strcmp(caSort,"useyh")==0){
        iSort=14;
    }
    else if(strcmp(caSort,"usexs")==0){
        iSort=15;
    }
    else if(strcmp(caSort,"usedq")==0){
        iSort=16;
    }
    else if(strcmp(caSort,"usetot")==0){
        iSort=17;
    }
    else if(strcmp(caSort,"bkyh")==0){
        iSort=18;
    }
    else if(strcmp(caSort,"bkxs")==0){
        iSort=19;
    }
    else if(strcmp(caSort,"bkdq")==0){
        iSort=20;
    }
    else if(strcmp(caSort,"bktot")==0){
        iSort=21;
    }
                   sprintf(caStemptime,"%s 00:00",caSdate_d);
                 lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
                 sprintf(caEtemptime,"%s 23:59",caEdate_d);
                 letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
            
    pHash = (unsigned char *)pasLHashInit(300,300,sizeof(struct data_s),0,32);    
    if(pHash == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
    
    //统计关键字
    sprintf(caTemp,"select count(*),opname,flag from ncskeywordlog where flag>0 and length(opname)>1 ");
    if(strlen(caSdate_d)>0){
        sprintf(caTemp+strlen(caTemp)," and optime>=%d ",lstime);
    }
    if(strlen(caEdate_d)>0){
        sprintf(caTemp+strlen(caTemp)," and optime<=%d ",letime);
    }
    sprintf(caTemp+strlen(caTemp)," group by opname,flag ");
    
    printf("caTemp=%s\n",caTemp);
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur){
           lCount=0;
           memset(caOpname,0,sizeof(caOpname));
           lFlag=0;
          iReturn = pasDbFetchInto(psCur,
                                       UT_TYPE_LONG,4,&lCount,
                                       UT_TYPE_STRING,30,caOpname,
                                       UT_TYPE_LONG,4,&lFlag);
            while(iReturn==0||iReturn==1405){
                  
                  psData = (struct data_s *)pasLHashLookA(pHash,caOpname); 
                  if(psData){
                      lSum=lSum+lCount;
                      lSum_keytot=lSum_keytot+lCount;
                      psData->lKeytot=psData->lKeytot+lCount;
                      psData->lTot=psData->lTot+lCount;
                      if(lFlag==1){
                          lSum_keyyh=lSum_keyyh+lCount;
                          psData->lKeyyh=psData->lKeyyh+lCount;
                      }
                      else if(lFlag==2){
                          lSum_keyxs=lSum_keyxs+lCount;
                          psData->lKeyxs=psData->lKeyxs+lCount;
                      }
                      else if(lFlag==9){
                          lSum_keydq=lSum_keydq+lCount;
                          psData->lKeydq=psData->lKeydq+lCount;
                      }
                
                   lCount=0;
                   memset(caOpname,0,sizeof(caOpname));
                   lFlag=0;
                  iReturn = pasDbFetchInto(psCur,
                                               UT_TYPE_LONG,4,&lCount,
                                               UT_TYPE_STRING,30,caOpname,
                                               UT_TYPE_LONG,4,&lFlag);
            }
            
        
    }
     pasDbCloseCursor(psCur);
  }
  
  
  
  
  
    //统计敏感URL
    sprintf(caTemp,"select count(*),opname,flag from ncssenurllog where flag>0 and length(opname)>1 ");
    if(strlen(caSdate_d)>0){
        sprintf(caTemp+strlen(caTemp)," and optime>=%d ",lstime);
    }
    if(strlen(caEdate_d)>0){
        sprintf(caTemp+strlen(caTemp)," and optime<=%d ",letime);
    }
    sprintf(caTemp+strlen(caTemp)," group by opname,flag ");
    
    printf("caTemp=%s\n",caTemp);
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur){
           lCount=0;
           memset(caOpname,0,sizeof(caOpname));
           lFlag=0;
          iReturn = pasDbFetchInto(psCur,
                                       UT_TYPE_LONG,4,&lCount,
                                       UT_TYPE_STRING,30,caOpname,
                                       UT_TYPE_LONG,4,&lFlag);
            while(iReturn==0||iReturn==1405){
                  
                  psData = (struct data_s *)pasLHashLookA(pHash,caOpname); 
                  if(psData){
                      lSum=lSum+lCount;
                      lSum_urltot=lSum_urltot+lCount;
                      psData->lUrltot=psData->lUrltot+lCount;
                      psData->lTot=psData->lTot+lCount;
                      if(lFlag==1){
                          lSum_urlyh=lSum_urlyh+lCount;
                          psData->lUrlyh=psData->lUrlyh+lCount;
                      }
                      else if(lFlag==2){
                          lSum_urlxs=lSum_urlxs+lCount;
                          psData->lUrlxs=psData->lUrlxs+lCount;
                      }
                      else if(lFlag==9){
                          lSum_urldq=lSum_urldq+lCount;
                          psData->lUrldq=psData->lUrldq+lCount;
                      }
                
                   lCount=0;
                   memset(caOpname,0,sizeof(caOpname));
                   lFlag=0;
                  iReturn = pasDbFetchInto(psCur,
                                               UT_TYPE_LONG,4,&lCount,
                                               UT_TYPE_STRING,30,caOpname,
                                               UT_TYPE_LONG,4,&lFlag);
            }
            
        
    }
     pasDbCloseCursor(psCur);
  }
  
  
      //统计敏感虚拟身份
    sprintf(caTemp,"select count(*),opname,flag from ncssennetidlog where flag>0 and length(opname)>1 ");
    if(strlen(caSdate_d)>0){
        sprintf(caTemp+strlen(caTemp)," and optime>=%d ",lstime);
    }
    if(strlen(caEdate_d)>0){
        sprintf(caTemp+strlen(caTemp)," and optime<=%d ",letime);
    }
    sprintf(caTemp+strlen(caTemp)," group by opname,flag ");
    
    printf("caTemp=%s\n",caTemp);
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur){
           lCount=0;
           memset(caOpname,0,sizeof(caOpname));
           lFlag=0;
          iReturn = pasDbFetchInto(psCur,
                                       UT_TYPE_LONG,4,&lCount,
                                       UT_TYPE_STRING,30,caOpname,
                                       UT_TYPE_LONG,4,&lFlag);
            while(iReturn==0||iReturn==1405){
                  
                  psData = (struct data_s *)pasLHashLookA(pHash,caOpname); 
                  if(psData){
                      lSum=lSum+lCount;
                      lSum_vidtot=lSum_vidtot+lCount;
                      psData->lVidtot+psData->lVidtot;
                      psData->lTot=psData->lTot+lCount;
                      if(lFlag==1){
                          lSum_vidyh=lSum_vidyh+lCount;
                          psData->lVidyh=psData->lVidyh+lCount;
                      }
                      else if(lFlag==2){
                          lSum_vidxs=lSum_vidxs+lCount;
                          psData->lVidxs=psData->lVidxs+lCount;
                      }
                      else if(lFlag==9){
                          lSum_viddq=lSum_viddq+lCount;
                          psData->lViddq=psData->lViddq+lCount;
                      }
                
                   lCount=0;
                   memset(caOpname,0,sizeof(caOpname));
                   lFlag=0;
                  iReturn = pasDbFetchInto(psCur,
                                               UT_TYPE_LONG,4,&lCount,
                                               UT_TYPE_STRING,30,caOpname,
                                               UT_TYPE_LONG,4,&lFlag);
            }
            
        
    }
     pasDbCloseCursor(psCur);
  }
  
  
    //统计敏感重点用户
    sprintf(caTemp,"select count(*),opname,flag from ncssenuserlog where flag>0 and length(opname)>1 ");
    if(strlen(caSdate_d)>0){
        sprintf(caTemp+strlen(caTemp)," and optime>=%d ",lstime);
    }
    if(strlen(caEdate_d)>0){
        sprintf(caTemp+strlen(caTemp)," and optime<=%d ",letime);
    }
    sprintf(caTemp+strlen(caTemp)," group by opname,flag ");
    
    printf("caTemp=%s\n",caTemp);
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur){
           lCount=0;
           memset(caOpname,0,sizeof(caOpname));
           lFlag=0;
          iReturn = pasDbFetchInto(psCur,
                                       UT_TYPE_LONG,4,&lCount,
                                       UT_TYPE_STRING,30,caOpname,
                                       UT_TYPE_LONG,4,&lFlag);
            while(iReturn==0||iReturn==1405){
                  
                  psData = (struct data_s *)pasLHashLookA(pHash,caOpname); 
                  if(psData){
                      lSum=lSum+lCount;
                      lSum_usetot=lSum_usetot+lCount;
                      psData->lUsetot=psData->lUsetot+lCount;
                      psData->lTot=psData->lTot+lCount;
                      if(lFlag==1){
                          lSum_useyh=lSum_useyh+lCount;
                          psData->lUseyh=psData->lUseyh+lCount;
                      }
                      else if(lFlag==2){
                          lSum_usexs=lSum_usexs+lCount;
                          psData->lUsexs=psData->lUsexs+lCount;
                      }
                      else if(lFlag==9){
                          lSum_usedq=lSum_usedq+lCount;
                          psData->lUsedq=psData->lUsedq+lCount;
                      }
                
                   lCount=0;
                   memset(caOpname,0,sizeof(caOpname));
                   lFlag=0;
                  iReturn = pasDbFetchInto(psCur,
                                               UT_TYPE_LONG,4,&lCount,
                                               UT_TYPE_STRING,30,caOpname,
                                               UT_TYPE_LONG,4,&lFlag);
            }
            
        
    }
     pasDbCloseCursor(psCur);
  }
  
   //统计布控告警
    sprintf(caTemp,"select count(*),opname,flags from ncscasemesg where flag>0 and length(opname)>1 ");
    if(strlen(caSdate_d)>0){
        sprintf(caTemp+strlen(caTemp)," and optime>=%d ",lstime);
    }
    if(strlen(caEdate_d)>0){
        sprintf(caTemp+strlen(caTemp)," and optime<=%d ",letime);
    }
    sprintf(caTemp+strlen(caTemp)," group by opname,flags ");
    
    printf("caTemp=%s\n",caTemp);
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur){
           lCount=0;
           memset(caOpname,0,sizeof(caOpname));
           lFlag=0;
          iReturn = pasDbFetchInto(psCur,
                                       UT_TYPE_LONG,4,&lCount,
                                       UT_TYPE_STRING,30,caOpname,
                                       UT_TYPE_LONG,4,&lFlag);
            while(iReturn==0||iReturn==1405){
                  
                  psData = (struct data_s *)pasLHashLookA(pHash,caOpname); 
                  if(psData){
                      lSum=lSum+lCount;
                      lSum_bktot=lSum_bktot+lCount;
                      psData->lTot=psData->lTot+lCount;
                      psData->lBktot=psData->lBktot+lCount;
                      if(lFlag==1){
                          lSum_bkyh=lSum_bkyh+lCount;
                          psData->lBkyh=psData->lBkyh+lCount;
                      }
                      else if(lFlag==2){
                          lSum_bkxs=lSum_bkxs+lCount;
                          psData->lBkxs=psData->lBkxs+lCount;
                      }
                      else if(lFlag==9){
                          lSum_bkdq=lSum_bkdq+lCount;
                          psData->lBkdq=psData->lBkdq+lCount;
                      }
                
                   lCount=0;
                   memset(caOpname,0,sizeof(caOpname));
                   lFlag=0;
                  iReturn = pasDbFetchInto(psCur,
                                               UT_TYPE_LONG,4,&lCount,
                                               UT_TYPE_STRING,30,caOpname,
                                               UT_TYPE_LONG,4,&lFlag);
            }
            
        
    }
     pasDbCloseCursor(psCur);
  }
  
    psData=(struct data_s*)pasLHashFirst(pHash,&sHashInfo);
    iNum=0;
    while(psData){
       strcpy(stData[iNum].caDispname,psData->caDispname);
       stData[iNum].lKeyyh=psData->lKeyyh;
       stData[iNum].lKeyxs=psData->lKeyxs;
       stData[iNum].lKeydq=psData->lKeydq;
       stData[iNum].lKeytot=psData->lKeytot;
       
       stData[iNum].lUrlyh=psData->lUrlyh;
       stData[iNum].lUrlxs=psData->lUrlxs;
       stData[iNum].lUrldq=psData->lUrldq;
       stData[iNum].lUrltot=psData->lUrltot;
       
       stData[iNum].lVidyh=psData->lVidyh;
       stData[iNum].lVidxs=psData->lVidxs;
       stData[iNum].lViddq=psData->lViddq;
       stData[iNum].lVidtot=psData->lVidtot;
       
       stData[iNum].lUseyh=psData->lUseyh;
       stData[iNum].lUsexs=psData->lUsexs;
       stData[iNum].lUsedq=psData->lUsedq;
       stData[iNum].lUsetot=psData->lUsetot;
       stData[iNum].lBkyh=psData->lBkyh;
       stData[iNum].lBkxs=psData->lBkxs;
       stData[iNum].lBkdq=psData->lBkdq;
       stData[iNum].lBktot=psData->lBktot;
       stData[iNum].lTot=psData->lTot;
       
       
       
        iNum++;
        psData=(struct data_s *)pasLHashNext(&sHashInfo);
    }
    free(pHash);
    

 
    qsort(stData, iNum,sizeof(struct data_s),ncUtlSortOnCount);
  
  
  
           if(strlen(caExport)>0){
        
                sprintf(caFilename,"ncsworkstat%d.csv",utStrGetId());
                sprintf(caPath,"%s/html","/home/ncmysql/ncs");
                sprintf(caFile,"%s/%s",caPath,caFilename);
                fp = fopen(caFile,"w");
                if(fp == NULL) {
                    pasDbCloseCursor(psCur);
                 
                    utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
                    return 0;
                }
                
                fprintf(fp,"关键字日志\n");
                ncUtlPrintCsv(fp,22,UT_TYPE_STRING,"操作员",
                                   UT_TYPE_STRING,"关键字归入有害",
                                   UT_TYPE_STRING,"关键字归入显示",
                                   UT_TYPE_STRING,"关键字丢弃日志",
                                   UT_TYPE_STRING,"合计处理关键字",
                                   UT_TYPE_STRING,"敏感网址归入有害",
                                   UT_TYPE_STRING,"敏感网址归入线索",
                                   UT_TYPE_STRING,"丢弃敏感网址",
                                   UT_TYPE_STRING,"合计处理敏感网址",
                                   UT_TYPE_STRING,"虚拟身份归入有害",
                                   UT_TYPE_STRING,"虚拟身份归入线索",
                                   UT_TYPE_STRING,"丢弃虚拟身份",
                                   UT_TYPE_STRING,"合计处理虚拟身份",
                                   UT_TYPE_STRING,"重点用户归入有害",
                                   UT_TYPE_STRING,"重点用户归入线索",
                                   UT_TYPE_STRING,"丢弃重点用户",
                                   UT_TYPE_STRING,"合计处理重点用户",
                                   UT_TYPE_STRING,"布控告警归入有害",
                                   UT_TYPE_STRING,"布控告警归入线索",
                                   UT_TYPE_STRING,"丢弃布控告警",
                                   UT_TYPE_STRING,"合计处理布控告警",
                                   UT_TYPE_STRING,"总计");
                                      
    }
  
  
    psDbHead = utPltInitDb(); 
  
      for(i=0;i<iNum;i++){
        if(i>0){
            utPltPutLoopVar(psDbHead,"dh",i+1,",");
        }
        else{
            utPltPutLoopVar(psDbHead,"dh",i+1,"");
        }
        utPltPutLoopVar(psDbHead,"dispname",i+1,stData[i].caDispname);
        utPltPutLoopVarF(psDbHead,"keyyh",i+1,"%d",stData[i].lKeyyh);
        utPltPutLoopVarF(psDbHead,"keyxs",i+1,"%d",stData[i].lKeyxs);
        utPltPutLoopVarF(psDbHead,"keydq",i+1,"%d",stData[i].lKeydq);
        utPltPutLoopVarF(psDbHead,"keytot",i+1,"%d",stData[i].lKeytot);
        utPltPutLoopVarF(psDbHead,"urlyh",i+1,"%d",stData[i].lUrlyh);
        utPltPutLoopVarF(psDbHead,"urlxs",i+1,"%d",stData[i].lUrlxs);
        utPltPutLoopVarF(psDbHead,"urldq",i+1,"%d",stData[i].lUrldq);
        utPltPutLoopVarF(psDbHead,"urltot",i+1,"%d",stData[i].lUrltot);
        utPltPutLoopVarF(psDbHead,"vidyh",i+1,"%d",stData[i].lVidyh);
        utPltPutLoopVarF(psDbHead,"vidxs",i+1,"%d",stData[i].lVidxs);
        utPltPutLoopVarF(psDbHead,"viddq",i+1,"%d",stData[i].lViddq);
        utPltPutLoopVarF(psDbHead,"vidtot",i+1,"%d",stData[i].lVidtot);
        utPltPutLoopVarF(psDbHead,"useyh",i+1,"%d",stData[i].lUseyh);
        utPltPutLoopVarF(psDbHead,"usexs",i+1,"%d",stData[i].lUsexs);
        utPltPutLoopVarF(psDbHead,"usedq",i+1,"%d",stData[i].lUsedq);
        utPltPutLoopVarF(psDbHead,"usetot",i+1,"%d",stData[i].lUsetot);
        utPltPutLoopVarF(psDbHead,"bkyh",i+1,"%d",stData[i].lBkyh);
        utPltPutLoopVarF(psDbHead,"bkxs",i+1,"%d",stData[i].lBkxs);
        utPltPutLoopVarF(psDbHead,"bkdq",i+1,"%d",stData[i].lBkdq);
        utPltPutLoopVarF(psDbHead,"bktot",i+1,"%d",stData[i].lBktot);
        utPltPutLoopVarF(psDbHead,"tot",i+1,"%d",stData[i].lTot);
        
         if(strlen(caExport)>0){
                     ncUtlPrintCsv(fp,22,
                                    UT_TYPE_STRING,stData[i].caDispname,
                                    UT_TYPE_LONG,stData[i].lKeyyh,
                                    UT_TYPE_LONG,stData[i].lKeyxs,                                    
                          UT_TYPE_LONG,stData[i].lKeydq,
                                    UT_TYPE_LONG,stData[i].lKeytot,
                            UT_TYPE_LONG,stData[i].lUrlyh,
                                    UT_TYPE_LONG,stData[i].lUrlxs,                                
                          UT_TYPE_LONG,stData[i].lUrldq,
                                    UT_TYPE_LONG,stData[i].lUrltot,
                                    UT_TYPE_LONG,stData[i].lVidyh,
                                    UT_TYPE_LONG,stData[i].lVidxs,
                            UT_TYPE_LONG,stData[i].lViddq,
                                    UT_TYPE_LONG,stData[i].lVidtot,                                
                          UT_TYPE_LONG,stData[i].lUseyh,
                                    UT_TYPE_LONG,stData[i].lUsexs,
                                    UT_TYPE_LONG,stData[i].lUsedq,
                                    UT_TYPE_LONG,stData[i].lUsetot,
                                    UT_TYPE_LONG,stData[i].lBkyh,
                            UT_TYPE_LONG,stData[i].lBkxs,
                                    UT_TYPE_LONG,stData[i].lBkdq,                                
                          UT_TYPE_LONG,stData[i].lBktot,
                                    UT_TYPE_LONG,stData[i].lTot);
                         
                                                  
                 }
        
        
    }
    if(i>=2){
        utPltPutLoopVar(psDbHead,"dh",i+1,",");
      utPltPutLoopVar(psDbHead,"dispname",i+1,"<font color=red>合计</font>");
        utPltPutLoopVarF(psDbHead,"keyyh",i+1,"<font color=red>%d</font>",lSum_keyyh);
        utPltPutLoopVarF(psDbHead,"keyxs",i+1,"<font color=red>%d</font>",lSum_keyxs);
        utPltPutLoopVarF(psDbHead,"keydq",i+1,"<font color=red>%d</font>",lSum_keydq);
        utPltPutLoopVarF(psDbHead,"keytot",i+1,"%d",lSum_keytot);
        utPltPutLoopVarF(psDbHead,"urlyh",i+1,"<font color=red>%d</font>",lSum_urlyh);
        utPltPutLoopVarF(psDbHead,"urlxs",i+1,"<font color=red>%d</font>",lSum_urlxs);
        utPltPutLoopVarF(psDbHead,"urldq",i+1,"<font color=red>%d</font>",lSum_urldq);
        utPltPutLoopVarF(psDbHead,"urltot",i+1,"%d",lSum_urltot);
        utPltPutLoopVarF(psDbHead,"vidyh",i+1,"<font color=red>%d</font>",lSum_vidyh);
        utPltPutLoopVarF(psDbHead,"vidxs",i+1,"<font color=red>%d</font>",lSum_vidxs);
        utPltPutLoopVarF(psDbHead,"viddq",i+1,"<font color=red>%d</font>",lSum_viddq);
        utPltPutLoopVarF(psDbHead,"vidtot",i+1,"%d",lSum_vidtot);
        utPltPutLoopVarF(psDbHead,"useyh",i+1,"<font color=red>%d</font>",lSum_useyh);
        utPltPutLoopVarF(psDbHead,"usexs",i+1,"<font color=red>%d</font>",lSum_usexs);
        utPltPutLoopVarF(psDbHead,"usedq",i+1,"<font color=red>%d</font>",lSum_usedq);
        utPltPutLoopVarF(psDbHead,"usetot",i+1,"%d",lSum_usetot);
        utPltPutLoopVarF(psDbHead,"bkyh",i+1,"<font color=red>%d</font>",lSum_bkyh);
        utPltPutLoopVarF(psDbHead,"bkxs",i+1,"<font color=red>%d</font>",lSum_bkxs);
        utPltPutLoopVarF(psDbHead,"bkdq",i+1,"<font color=red>%d</font>",lSum_bkdq);
        utPltPutLoopVarF(psDbHead,"bktot",i+1,"%d",lSum_bktot);
        utPltPutLoopVarF(psDbHead,"tot",i+1,"%d",lSum);
        
        
        if(strlen(caExport)>0){
                     ncUtlPrintCsv(fp,22,
                                    UT_TYPE_STRING,"合计",
                                    UT_TYPE_LONG,lSum_keyyh,
                                    UT_TYPE_LONG,lSum_keyxs,                                    
                          UT_TYPE_LONG,lSum_keydq,
                                    UT_TYPE_LONG,lSum_keytot,
                            UT_TYPE_LONG,lSum_urlyh,
                                    UT_TYPE_LONG,lSum_urlxs,                                
                          UT_TYPE_LONG,lSum_urldq,
                                    UT_TYPE_LONG,lSum_urltot,
                                    UT_TYPE_LONG,lSum_vidyh,
                                    UT_TYPE_LONG,lSum_vidxs,
                            UT_TYPE_LONG,lSum_viddq,
                                    UT_TYPE_LONG,lSum_vidtot,                                
                          UT_TYPE_LONG,lSum_useyh,
                                    UT_TYPE_LONG,lSum_usexs,
                                    UT_TYPE_LONG,lSum_usedq,
                                    UT_TYPE_LONG,lSum_usetot,
                                    UT_TYPE_LONG,lSum_bkyh,
                            UT_TYPE_LONG,lSum_bkxs,
                                    UT_TYPE_LONG,lSum_bkdq,                                
                          UT_TYPE_LONG,lSum_bktot,
                                    UT_TYPE_LONG,lSum);
                         
                                                  
                 }
        
        
        
    }
   utPltPutVarF(psDbHead,"TotRec", "%lu", iNum+1);
   
             if(strlen(caExport)>0){
             fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
         
         
     }
   
   
  //  printf("cccc\n");
 // utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_mgxxcl_list.htm");

    return 0;

}



//显示虚拟身份统计框架页面
int ncsVid_stat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{

    int i,iNum;
    utPltDbHead *psDbHead;
       int iReturn;
    char caServices[2048];
    char caSdate[16],caEdate[16],caGroupid[16],caDid[16],caTemp[2048];
    long lService,lCount;
    char caStemptime[20],caEtemptime[20];
    long lstime,letime;
    ncsNettype *psData;
     pasDbCursor *psCur;
  psDbHead = utPltInitDb();
   iReturn = utMsgGetSomeNVar(psMsgHead,4,
                     "sdate",          UT_TYPE_STRING,10,caSdate,
                     "edate",          UT_TYPE_STRING,10,caEdate,
                     "groupid",        UT_TYPE_STRING,10,caGroupid,
                     "did",            UT_TYPE_STRING,10,caDid);
                     
                     
        sprintf(caStemptime,"%s 00:00",caSdate);
            lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
          sprintf(caEtemptime,"%s 23:59",caEdate);
            letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);              
                     
                     
                     
                     
      iNum=0;
        sprintf(caTemp,"select service,sum(lCount) aa from ncsnetidstat group by service order by aa desc limit 0,16    ");
        psCur=pasDbOpenSql(caTemp,0);
         if(psCur){
               lService=0;
               lCount=0;
               iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lService,
                                            UT_TYPE_LONG,4,&lCount); 
               iNum=0;
               memset(caServices,0,sizeof(caServices));
               while(iReturn==0||iReturn==1405){
                    psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&lService); 
                  if(psData){
                      iNum++;
                      if(iNum==1){
                          sprintf(caServices,"%d",lService);
                      }
                      else{
                          sprintf(caServices+strlen(caServices),",%d",lService);
                      }
                      utPltPutLoopVar(psDbHead,"sname",iNum,psData->caName);
                      utPltPutLoopVarF(psDbHead,"sservice",iNum,"s%d",lService);
                  }
                     
                       lService=0;
                       lCount=0;
                       iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lService,
                                                    UT_TYPE_LONG,4,&lCount); 
               }      
             
             pasDbCloseCursor(psCur);
        }  
        if(iNum==0){
                    sprintf(caTemp,"select ntype,count(*) aa from ncsnetid_if group by ntype order by aa desc limit 0,16    ");
                    printf("caTemp=%s\n",caTemp);
                    psCur=pasDbOpenSql(caTemp,0);
                     if(psCur){
                           lService=0;
                           lCount=0;
                           iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lService,
                                                        UT_TYPE_LONG,4,&lCount); 
                           iNum=0;
                           memset(caServices,0,sizeof(caServices));
                           while(iReturn==0||iReturn==1405){
                                psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&lService); 
                              if(psData){
                                  iNum++;
                                  if(iNum==1){
                                      sprintf(caServices,"%d",lService);
                                  }
                                  else{
                                      sprintf(caServices+strlen(caServices),",%d",lService);
                                  }
                                  utPltPutLoopVar(psDbHead,"sname",iNum,psData->caName);
                                  utPltPutLoopVarF(psDbHead,"sservice",iNum,"s%d",lService);
                              }
                                 
                                   lService=0;
                                   lCount=0;
                                   iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lService,
                                                                UT_TYPE_LONG,4,&lCount); 
                           }      
            printf("iNum=%d\n",iNum);             
                         pasDbCloseCursor(psCur);
                    }  
                        
        }
        iNum++;
        utPltPutLoopVar(psDbHead,"sname",iNum,"其它");
        utPltPutLoopVar(psDbHead,"sservice",iNum,"s0");
        utPltPutLoopVar(psDbHead,"sname",iNum+1,"合计");
        utPltPutLoopVar(psDbHead,"sservice",iNum+1,"stot");      
                
    utPltPutVar(psDbHead,"services",caServices);
  //  printf("cccc\n");
//    utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_vid_stat.html");
    return 0;

}

static int getServiceNum(char *in,int lService){
    long iNum,i;
    long iRet=0;
    char caObj[100][50];
    iNum=ncs_SepChar0(in,',',caObj);
    for(i=0;i<iNum;i++){
        if(lService==atol(caObj[i])){
            return i;
        }
    }
    return 0;
}

int ncsVid_statdet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
    char caObj[100][50];
    unsigned long lCurPg,lTotPg;
    unsigned long lTime;
    int i,iNum,num,lCount_servcie;
    
    utPltDbHead *psDbHead;
   	int iReturn;
    char caSvalue[128],caSname[128];

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
  char caGroupid[16],caServices[512];
  long lCount_netid;
  long lHj[32];
  lCurPg=1;
  struct ServiceData {
  	long lService;
  	long lCount;
  };
  typedef struct sddata_s{
  	long lDid;
  	long lSum;
  	struct ServiceData psService[20];
  };
  char *pHash;
  struct sddata_s *psData;
  pasLHashInfo sHashInfo; 
  char caGroups[2048];
  long lSum_did,lDid;
  long lSum_oth;
  long m;
  
  	  FILE *fp;
		char caFilename[32];
		char caFile[128];
		char caPath[128];
		ncsNettype *psData2;
    char caNtype[32];
    long lNtype;
  lSum=0;
  lSum_oth=0;
 	strcpy(caWhere,"\0");
  for(i=0;i<32;i++){
  	lHj[i]=0;
  }
 	

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,10,
			"groupid",UT_TYPE_STRING,10,caGroupid,
			"did",    UT_TYPE_STRING,10,caDid,
			"sdate",  UT_TYPE_STRING,19,caSdate,
			"edate",  UT_TYPE_STRING,19,caEdate,
			"services",UT_TYPE_STRING,255,caServices,
   	  "exp",  UT_TYPE_STRING,10,caExport,
   	  "page",    UT_TYPE_STRING,10,caPage,
   	  "limit",   UT_TYPE_STRING,10,caLimit,
   	  "sort",    UT_TYPE_STRING, 15, caSort,
   	  "dir",     UT_TYPE_STRING, 8,  caDir);
   	 memset(caGroups,0,sizeof(caGroups));
    if(strlen(caGroupid)>0){
    	strcpy(caGroups,getGroupid(atol(caGroupid)));
    }
   //   printf("caGroups=%s\n",caGroups);
    utMsgPrintMsg(psMsgHead); 
           
      lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;
     lCount_servcie=ncs_SepChar0(caServices,',',caObj);

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
      	   	  
   	  	 sprintf(caStemptime,"%s 00:00",caSdate);
		     lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
		     sprintf(caEtemptime,"%s 23:59",caEdate);
		     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
  
     
    pHash = (unsigned char *)pasLHashInit(5000,5000,sizeof(struct sddata_s),0,4);    
    if(pHash == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
     
     
     
     
     sprintf(caTemp,"select count(*) from ncsnetidstat where 1=1 ");
     if(strlen(caSdate)>0){
        sprintf(caTemp+strlen(caTemp)," and sdate>='%s' ",caSdate);
      }
      if(strlen(caEdate)>0){
      	sprintf(caTemp+strlen(caTemp)," and edate<='%s' ",caEdate);
      }
      if(strlen(caGroups)>0){
      	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where groupid in (%s)) ",caGroups);
      }
      if(strlen(caDid)>0){
      	sprintf(caTemp+strlen(caTemp)," and did=%s ",caDid);
      }
      
      sprintf(caTemp+strlen(caTemp)," group by did ");
      lCount=0;
      printf("caTemp=%s\n",caTemp);
      pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
   	                
    if(lCount==0){
    	sprintf(caTemp,"select count(*) from ncsnetid_if where 1=1 ");
    	if(strlen(caSdate)>0){
    		sprintf(caTemp+strlen(caTemp)," and starttime>=%d ",lstime);
    	}
    	if(strlen(caEdate)>0){
    		sprintf(caTemp+strlen(caTemp)," and starttime<=%d ",letime);
     }
       if(strlen(caGroups)>0){
      	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where groupid in (%s)) ",caGroups);
      }
      if(strlen(caDid)>0){
      	sprintf(caTemp+strlen(caTemp)," and did=%s ",caDid);
      }
      lCount_netid=0;
      pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount_netid);
	  }
	  
	  if(lCount>0){
	  	sprintf(caTemp,"select sum(lcount),did,service from ncsnetidstat where 1=1 ");
	  	if(strlen(caServices)>0){
	  		sprintf(caTemp+strlen(caTemp)," and service in (%s) ",caServices);
	  	}
     if(strlen(caSdate)>0){
        sprintf(caTemp+strlen(caTemp)," and sdate>='%s' ",caSdate);
      }
      if(strlen(caEdate)>0){
      	sprintf(caTemp+strlen(caTemp)," and edate<='%s' ",caEdate);
      }
      if(strlen(caGroups)>0){
      	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where groupid in (%s)) ",caGroups);
      }
      if(strlen(caDid)>0){
      	sprintf(caTemp+strlen(caTemp)," and did=%s ",caDid);
      }
	  	sprintf(caTemp+strlen(caTemp)," group by did,service ");
	  	printf("caTemp=%s\n",caTemp);
	  	psCur=pasDbOpenSql(caTemp,0);
	  	if(psCur){
	  		lSum_did=0;
	  		lDid=0;
	  		lService=0;
	  		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_LONG,4,&lSum_did,
		                               UT_TYPE_LONG,4,&lDid,
		                               UT_TYPE_LONG,4,&lService);
		    while(iReturn==0||iReturn==1405){
		    	
		    	 psData = (struct data_s *)pasLHashLookA(pHash,&lDid); 
		    	  if(psData){
		    	  	num=getServiceNum(caServices,lService);
		    	  	psData->psService[num].lCount=lSum_did;
		    	  	psData->lSum=psData->lSum+lSum_did;
		//    	 	   psData->psService[psData->index].lService=lService;
	//	    	 	   psData->psService[psData->index].lCount=lSum_did;
	//	    	 	   psData->index++;
		    	  }
		    	
		    	
		    		lSum_did=0;
			  		lDid=0;
			  		lService=0;
			  		iReturn = pasDbFetchInto(psCur,
				                               UT_TYPE_LONG,4,&lSum_did,
				                               UT_TYPE_LONG,4,&lDid,
				                               UT_TYPE_LONG,4,&lService);
		    }
		    
	  		
	  		pasDbCloseCursor(psCur);
	  		
	  	}
	  		  	//统计其它服务虚拟身份
	     sprintf(caTemp,"select count(*),did from ncsnetidstat where 1=1 ");

	  		sprintf(caTemp+strlen(caTemp)," and service not in (%s) ",caServices);

      if(strlen(caSdate)>0){
        sprintf(caTemp+strlen(caTemp)," and sdate>='%s' ",caSdate);
      }
      if(strlen(caEdate)>0){
      	sprintf(caTemp+strlen(caTemp)," and edate<='%s' ",caEdate);
      }
      if(strlen(caGroups)>0){
      	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where groupid in (%s)) ",caGroups);
      }
      if(strlen(caDid)>0){
      	sprintf(caTemp+strlen(caTemp)," and did=%s ",caDid);
      }
      
	  	sprintf(caTemp+strlen(caTemp)," group by did ");
	  		  	psCur=pasDbOpenSql(caTemp,0);
				  	if(psCur){
				  		lSum_did=0;
				  		lDid=0;
				  		lService=0;
				  		iReturn = pasDbFetchInto(psCur,
					                               UT_TYPE_LONG,4,&lSum_did,
					                               UT_TYPE_LONG,4,&lDid);
					                              
					    while(iReturn==0||iReturn==1405){
					    	
					    	  psData = (struct data_s *)pasLHashLookA(pHash,&lDid); 
					    	  if(psData){
					   // 	  	num=getServiceNum(caServices,lService);
					    	  	psData->psService[lCount_servcie].lCount=lSum_did;
					    	  	psData->lSum=psData->lSum+lSum_did;
					    	  }
					    	
					    	
					    		lSum_did=0;
						  		lDid=0;
						  		lService=0;
						  		iReturn = pasDbFetchInto(psCur,
							                               UT_TYPE_LONG,4,&lSum_did,
							                               UT_TYPE_LONG,4,&lDid);
							                               
					    }
					    
				  		
				  		pasDbCloseCursor(psCur);
				  		
				  	}
	  	
	  	
	  	
	  	
	  	
	  	
	  }
	  else if(lCount_netid>0){
	  	
	  	sprintf(caTemp,"select count(*),did,ntype from ncsnetid_if where 1=1 ");
	  	if(strlen(caServices)>0){
	  		sprintf(caTemp+strlen(caTemp)," and ntype in (%s) ",caServices);
	  	}
    	if(strlen(caSdate)>0){
    		sprintf(caTemp+strlen(caTemp)," and starttime>=%d ",lstime);
    	}
    	if(strlen(caEdate)>0){
    		sprintf(caTemp+strlen(caTemp)," and starttime<=%d ",letime);
     }
      if(strlen(caGroups)>0){
      	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where groupid in (%s)) ",caGroups);
      }
      if(strlen(caDid)>0){
      	sprintf(caTemp+strlen(caTemp)," and did=%s ",caDid);
      }
	  	sprintf(caTemp+strlen(caTemp)," group by did,ntype ");
	  	printf("caTemp=%s\n",caTemp);
	  	psCur=pasDbOpenSql(caTemp,0);
	  	if(psCur){
	  		lSum_did=0;
	  		lDid=0;
	  		lService=0;
	  		iReturn = pasDbFetchInto(psCur,
		                               UT_TYPE_LONG,4,&lSum_did,
		                               UT_TYPE_LONG,4,&lDid,
		                               UT_TYPE_LONG,4,&lService);
		    while(iReturn==0||iReturn==1405){
		    	
		    	  psData = (struct data_s *)pasLHashLookA(pHash,&lDid); 
		    	  
		    	  if(psData){
		    	  	num=getServiceNum(caServices,lService);
		    	  	psData->psService[num].lCount=lSum_did;
		    	  	psData->lSum=psData->lSum+lSum_did;
		    	  	lSum=lSum+lSum_did;
		    	  }
		    	
		    	
		    		lSum_did=0;
			  		lDid=0;
			  		lService=0;
			  		iReturn = pasDbFetchInto(psCur,
				                               UT_TYPE_LONG,4,&lSum_did,
				                               UT_TYPE_LONG,4,&lDid,
				                               UT_TYPE_LONG,4,&lService);
		    }
		    
	  		
	  		pasDbCloseCursor(psCur);
	  		
	  	}
	  	//统计其它服务虚拟身份
	     sprintf(caTemp,"select count(*),did from ncsnetid_if where 1=1 ");

	  		sprintf(caTemp+strlen(caTemp)," and ntype not in (%s) ",caServices);

    	if(strlen(caSdate)>0){
    		sprintf(caTemp+strlen(caTemp)," and starttime>=%d ",lstime);
    	}
    	if(strlen(caEdate)>0){
    		sprintf(caTemp+strlen(caTemp)," and starttime<=%d ",letime);
     }
      if(strlen(caGroups)>0){
      	sprintf(caTemp+strlen(caTemp)," and did in (select userid from ncsuser where groupid in (%s)) ",caGroups);
      }
      if(strlen(caDid)>0){
      	sprintf(caTemp+strlen(caTemp)," and did=%s ",caDid);
      }
	  	sprintf(caTemp+strlen(caTemp)," group by did ");
	  		  	psCur=pasDbOpenSql(caTemp,0);
				  	if(psCur){
				  		lSum_did=0;
				  		lDid=0;
				  		lService=0;
				  		iReturn = pasDbFetchInto(psCur,
					                               UT_TYPE_LONG,4,&lSum_did,
					                               UT_TYPE_LONG,4,&lDid);
					                              
					    while(iReturn==0||iReturn==1405){
					    	
					    	  psData = (struct data_s *)pasLHashLookA(pHash,&lDid); 
					    	  if(psData){
					   // 	  	num=getServiceNum(caServices,lService);
					    	  	psData->psService[lCount_servcie].lCount=lSum_did;
					    	  	psData->lSum=psData->lSum+lSum_did;
					    	  	lSum_oth=lSum_oth+lSum_did;
					    	  	lSum=lSum+lSum_did;
					    	  }
					    	
					    	
					    		lSum_did=0;
						  		lDid=0;
						  		lService=0;
						  		iReturn = pasDbFetchInto(psCur,
							                               UT_TYPE_LONG,4,&lSum_did,
							                               UT_TYPE_LONG,4,&lDid);
							                               
					    }
					    
				  		
				  		pasDbCloseCursor(psCur);
				  		
				  	}
	  	
	  	
	  }
    else{
    	 psDbHead = utPltInitDb();
       utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
       utPltPutVarF(psDbHead,"TotPg", "%lu",0);
       utPltPutVarF(psDbHead,"CurPg", "%lu", 1);
       free(pHash);
       utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_vidstat_list.htm");
      return 0;
    }
    
    
      		 if(strlen(caExport)>0){
		
		        sprintf(caFilename,"ncsnetidstat%d.csv",utStrGetId());
		        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		        sprintf(caFile,"%s/%s",caPath,caFilename);
		    	fp = fopen(caFile,"w");
		        if(fp == NULL) {
		            pasDbCloseCursor(psCur);
		         
		            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
		            return 0;
		        }
		        
		        fprintf(fp,"虚拟身份\n");
		        fprintf(fp,"单位名称");
		        for(i=0;i<lCount_servcie;i++){
		        	 lNtype=atol(caObj[i]);
		        	  psData2 = (struct ncsNettype_s *)pasLHashLook(pHashNettype,&lNtype); 
					      if(psData2){
					         strcpy(caNtype,psData2->caName);
					      }
					      else{
					      	strcpy(caNtype,"");
					      }
		        	fprintf(fp,",%s",caNtype);
			
		        }
           fprintf(fp,",其它,合计\n");
		                          	
	     }
    
    	
    psDbHead = utPltInitDb();
    
    
// printf("caWhere=%s\n",caWhere);  
    
    	
    	
 
    lStartRec = (lCurPg - 1) *lRowNum;  
 
     psData=(struct sddata_s *)pasLHashFirst(pHash,&sHashInfo);
     iNum=0;
     m=0;
      while(psData){
  
      	if(m>=lStartRec&&iNum<lRowNum){
         if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }             	      
         	utPltPutLoopVar(psDbHead,"cname",iNum+1,ncsUtlGetClientNameById(psShmHead,psData->lDid,"未知"));
 //        	printf("count=%d\n",psData->index);
         	for(i=0;i<lCount_servcie;i++){
         		utPltPutLoopVarF2(psDbHead,"sname",iNum+1,i+1,"s%s",caObj[i]);
         		utPltPutLoopVarF2(psDbHead,"scount",iNum+1,i+1,"%d",psData->psService[i].lCount);
         		lHj[i]=lHj[i]+psData->psService[i].lCount;
         	}
         	utPltPutLoopVar2(psDbHead,"sname",iNum+1,lCount_servcie+1,"s0");
         	utPltPutLoopVarF2(psDbHead,"scount",iNum+1,lCount_servcie+1,"%d",psData->psService[lCount_servcie].lCount);
         	   
         	utPltPutLoopVar2(psDbHead,"sname",iNum+1,lCount_servcie+2,"stot");
         	utPltPutLoopVarF2(psDbHead,"scount",iNum+1,lCount_servcie+2,"<font color=red>%d</font>",psData->lSum);           	               	    
         iNum++;
         
        }
        else{
        	 for(i=0;i<lCount_servcie;i++){        		
         		lHj[i]=lHj[i]+psData->psService[i].lCount;
         	}
        }
        if(strlen(caExport)>0){
        	fprintf(fp,"%s",ncsUtlGetClientNameById(psShmHead,psData->lDid,"未知"));
        	for(i=0;i<lCount_servcie;i++){
        		fprintf(fp,",%d",psData->psService[i].lCount);
        	}
        	fprintf(fp,",%d",psData->psService[lCount_servcie].lCount);
        	fprintf(fp,",%d\n",psData->lSum);
        	
        }
        
        
        
        m++;
    	   psData=(struct sddata_s *)pasLHashNext(&sHashInfo);
      }
  	
	//输出合计
	if(iNum>1){
		utPltPutLoopVar(psDbHead,"cname",iNum+1,"<font color=red>总计</font>");
		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		for(i=0;i<lCount_servcie;i++){
			utPltPutLoopVarF2(psDbHead,"sname",iNum+1,i+1,"s%s",caObj[i]);
			utPltPutLoopVarF2(psDbHead,"scount",iNum+1,i+1,"<font color=red>%d</font>",lHj[i]);
		}
		
		      utPltPutLoopVar2(psDbHead,"sname",iNum+1,i+1,"s0");
         	utPltPutLoopVarF2(psDbHead,"scount",iNum+1,i+1,"<font color=red>%d</font>",lSum_oth);
         	   
         	utPltPutLoopVar2(psDbHead,"sname",iNum+1,i+2,"stot");
         	utPltPutLoopVarF2(psDbHead,"scount",iNum+1,i+2,"<font color=red>%d</font>",lSum);  
		
		if(strlen(caExport)>0){
			fprintf(fp,"%s","总计");
			for(i=0;i<lCount_servcie;i++){
				fprintf(fp,",%d",lHj[i]);
			}
			fprintf(fp,",%d,%d\n",lSum_oth,lSum);
		}
		
		
	}

  
               



  //  lTotPg = (m ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", m);

    if(strlen(caExport)>0){
	 	    fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
	 		 	
	 }

 // utPltShowDb(psDbHead);
    free(pHash);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_vidstat_list.htm");
    return 0;
}// int ncsVid_statdet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)


//报表任务定义树
int ncsTreeRpTask_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
    char caPid[16];
    char caPid1[16];
    long lPid;
    long lSumRec=0;
    long lTime;
    char caSdate[20],caEdate[20];
    char caSdate1[20],caEdate1[20];
    char caDel[16],caUpdate[16],caSelsid[1024],caKeyname[64],caSid[16],caTemp_d[64];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif      
    utMsgPrintMsg(psMsgHead);
      iReturn = utMsgGetSomeNVar(psMsgHead,10,
                         "del",           UT_TYPE_STRING,8,caDel,
                         "name",   UT_TYPE_STRING, 31, caName,      
                         "selsid",UT_TYPE_STRING,1000,caSelsid,
                         "update",UT_TYPE_STRING,10,caUpdate,
                         "sid",UT_TYPE_STRING,10,caSid,
                         "del",UT_TYPE_STRING,10,caDel,
                         "pid",  UT_TYPE_STRING,10,caPid,
                         "node",UT_TYPE_STRING,10,caPid1,
                         "sdate",   UT_TYPE_STRING, 10,caSdate1,
                    "edate",   UT_TYPE_STRING, 10, caEdate1);
        if(strlen(caName)>0){
          strcpy(caTemp_d,  pasCvtGBK(2,caName,caTemp,30)); 
          strcpy(caName,caTemp_d);
      }   
                         

       if(strcmp(caUpdate,"update")==0){
                 lTime = time(0);
             ncUtlWeekDate(lTime,caSdate,caEdate);
             
          if(strlen(caSid)==0){
              sprintf(caTemp,"select count(*) from reporttask where tname='%s' ",caName);
              lCount=0;
              pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
              if(lCount==0){
                 lSid = ncsUtlGenId(psShmHead);
                   pasDbExecSqlF("insert into reporttask(tname,sdate,edate,addtime,flags) values ('%s','%s','%s',%d,%d)",
                                             caName,caSdate,caEdate,time(0),0);
              }
          }
          else{
              sprintf(caTemp,"select count(*) from reporttask where tname='%s' and sid!=%s ",caName,caSid);
              lCount=0;
              pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
              if(lCount==0){
                          pasDbExecSqlF("update reporttask set tname='%s' where sid=%s ",caName,caSid);
              }
          }
          
      }
      
          if(strcmp(caUpdate,"modi")==0){
            pasDbExecSqlF("update reporttask set sdate='%s',edate='%s' where sid=%s ",caSdate1,caEdate1,caSid);
      }
      
      if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
                        iReturn=pasDbExecSqlF("delete from reporttask where sid in (%s)",caSelsid);
                    if(iReturn==0){
                    
                iReturn = pasDbExecSqlF("delete from taskdet where id  in(%s)",caSelsid);
              }

          
      }
 
 
 
 
 
    iNum = 0;
    
     psDbHead = utPltInitDb();

    


               psCur = pasDbOpenSqlF("select tname,sid,sdate,edate from reporttask where 1=1 order by tname limit 0,1000");
               
 //    psCur = pasDbOpenSqlF("select name,id,pid from ncskeyindex  order by name limit 0,1000");
               
            if(psCur == NULL) {
                  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
        //          free(pHash);
                return 0;
            }
           
         memset(caName,0,sizeof(caName));
         lId=0;
         memset(caSdate,0,sizeof(caSdate));
         memset(caEdate,0,sizeof(caEdate));
           iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
                                           UT_TYPE_LONG,4,&lId,
                                           UT_TYPE_STRING,10,caSdate,
                                           UT_TYPE_STRING,10,caEdate);
                                          
        
         
            while(iReturn == 0||iReturn==1405) {
                
                 if(iNum>0){
                     utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
                 }
                 iNum++;
                 utPltSetCvtHtml(1);  
                         utPltPutLoopVar(psDbHead,"tname",iNum,caName);
                 
                 utPltSetCvtHtml(0);  
             utPltPutLoopVar(psDbHead,"sdate",iNum,caSdate);
             utPltPutLoopVar(psDbHead,"edate",iNum,caEdate);
                 utPltPutLoopVarF(psDbHead,"sid",   iNum,"%ld",lId);
                 
             
              utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
                 utPltPutLoopVar(psDbHead,"cls", iNum,"file");
                                   
         memset(caName,0,sizeof(caName));
         lId=0;
         memset(caSdate,0,sizeof(caSdate));
         memset(caEdate,0,sizeof(caEdate));
           iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
                                           UT_TYPE_LONG,4,&lId,
                                           UT_TYPE_STRING,10,caSdate,
                                           UT_TYPE_STRING,10,caEdate);
            }
            pasDbCloseCursor(psCur);
        
          utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_retask_checktree.htm");
    
    return 0;
}




//显示烽火上传统计
int ncsFuUpload_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
     pasDbCursor *psCur;
      utPltDbHead *psDbHead;
      long iReturn,lCount;
      char caStart[16],caTemp[2048],caCurPg[16];
      long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
      long lSum,lstime,letime,iNum;
      char caStemptime[24],caEtemptime[24],caSdate[16],caEdate[16];
     char caCid[16],caLimit[16],caPage[16],caDel[16],caDir[16],caType_desc[24];
             char caExport[32];
      FILE *fp;
        char caFilename[32];
        char caFile[128];
        char caPath[128];
      char caServicecode[20];
      char caTemp_d[20];
      char caStime[20],caEtime[20];
      utMsgPrintMsg(psMsgHead);
      iReturn = utMsgGetSomeNVar(psMsgHead,9,
                         "limit",            UT_TYPE_STRING, 10, caLimit,
                         "page",          UT_TYPE_STRING, 10, caPage,  
                         "dir",           UT_TYPE_STRING,8,caDir,
                         "sdate",       UT_TYPE_STRING,15,caSdate,
                         "edate",         UT_TYPE_STRING,15,caEdate,
                         "stime",        UT_TYPE_STRING,4,caStime,
                         "etime",        UT_TYPE_STRING,4,caEtime,
                         "exp",            UT_TYPE_STRING,10,caExport,
                         "servicecode",    UT_TYPE_STRING,14,caServicecode);
        if(strlen(caStime)==0) strcpy(caStime,"00");
        if(strlen(caEtime)==0) strcpy(caEtime,"24");
        if(strlen(caServicecode)>0){
          strcpy(caTemp_d,  pasCvtGBK(2,caServicecode,caTemp,16)); 
          strcpy(caServicecode,caTemp_d);
      }    
        
        if(strlen(caSdate)==0&&strlen(caEdate)==0){
            strcpy(caSdate,utTimFormat("%Y/%m/%d",time(0)));
            strcpy(caEdate,caSdate);
        }
                 
        if(strlen(caSdate)>0){
           sprintf(caStemptime,"%s %s:00",caSdate,caStime);
             lstime=utTimStrToLong("%Y/%m/%d %H:%M",caStemptime);
        }
        if(strlen(caEdate)>0){
             sprintf(caEtemptime,"%s %s:00",caEdate,caEtime);
             letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);   
            }
                         
       
                          

      lRowNum=atol(caLimit);
      if(lCurPg<=0) lCurPg=1;
      lStartRec = atol(caStart);
      
      psDbHead = utPltInitDb(); 
      
 
      sprintf(caTemp,"select sum(lcount),type_desc from ncs_twlog where 1=1 ");
      if(strlen(caSdate)>0){
          sprintf(caTemp+strlen(caTemp)," and stime>=%lu ",lstime);
      }
      if(strlen(caEdate)>0){
          sprintf(caTemp+strlen(caTemp)," and stime<=%lu ",letime);
      }
      if(strlen(caServicecode)>0){
          sprintf(caTemp+strlen(caTemp)," and servicecode='%s' ",caServicecode);
      }
      sprintf(caTemp+strlen(caTemp)," group by type_desc ");
      
          
      printf("caTemp=%s\n",caTemp);
        

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
              if(strlen(caExport)>0){
        
                sprintf(caFilename,"ncsfhlog%d.csv",utStrGetId());
                sprintf(caPath,"%s/html","/home/ncmysql/ncs");
                sprintf(caFile,"%s/%s",caPath,caFilename);
                fp = fopen(caFile,"w");
                if(fp == NULL) {
                    pasDbCloseCursor(psCur);
                 
                    utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
                    return 0;
                }
    fprintf(fp,"烽火日志上传统计\n");
                   ncUtlPrintCsv(fp,2,UT_TYPE_STRING,"日志类型",
                                   UT_TYPE_STRING,"上传数量");
   
       }
                                     
       iReturn=0;
       iNum=0;
       while(iReturn == 0 ||iReturn==1405 ) {
       
       lSum=0;
       memset(caType_desc,0,sizeof(caType_desc));
        
        iReturn = pasDbFetchInto(psCur,
                                         UT_TYPE_LONG,4,&lSum,
                                         UT_TYPE_STRING,19,caType_desc);
                                         
                                                                       
                                        
                                        
       if((iReturn!=0)&&(iReturn!=1405)) break;    
        iNum++;
       if(iNum>1){
           utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }        
        utPltPutLoopVarF(psDbHead,"sum",iNum,"%lu",lSum);    
        utPltPutLoopVar(psDbHead,"type_desc",iNum,caType_desc);
        if(strlen(caExport)>0){
                     ncUtlPrintCsv(fp,2,
                                    UT_TYPE_STRING,caType_desc,
                                    UT_TYPE_LONG,lSum);
                                  }
            
    }
    pasDbCloseCursor(psCur);
      

      utPltPutVarF(psDbHead,"TotRec","%d",iNum);
      
      
                   if(strlen(caExport)>0){
             fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
         
         
     }
      
      
      utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_fhuplog_list.htm");
      return 0;
}




//显示报表任务明细
int ncsWebTaskDetlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
     pasDbCursor *psCur;
      utPltDbHead *psDbHead;
      long iReturn,lCount;
      char caStart[16],caTemp[2048],caCurPg[16];
      long lCurPg,lRowNum,lStartRec,sid,nettype,flags;
      char caName[64],caNetname[32],caTemp_d[128],caStatus[16];

      long lSid,lTid;

      long iNum;
    char caId[16];
    long i;
    long lId;
    char caGenrep[32];
    char caSdate1[16],caEdate1[16],caSdate[16],caEdate[16];
      char caLimit[16],caPage[16],caDel[16],caDir[16],caSelsid[1024],caUpdate[16],caNtype[16],caNetid[64],caSid[16];
      char caRpname[32],caWorktime[16],caSortby[16],caTid[16],caDid[16],caOpt[16],caGname[32],caMsg[1024];
       struct stData_s {
             unsigned long id;
             char name[64];
         } *pData;
    struct stData_s *psData;
       pasLHashInfo sHashInfo; 
    unsigned char *pHash;
     
     
      utMsgPrintMsg(psMsgHead);

      iReturn = utMsgGetSomeNVar(psMsgHead,12,
                     "sid",            UT_TYPE_STRING, 10, caSid,
                         "limit",            UT_TYPE_STRING, 10, caLimit,
                         "page",          UT_TYPE_STRING, 10, caPage,  
                         "del",           UT_TYPE_STRING,8,caDel,
                         "dir",           UT_TYPE_STRING,8,caDir,
                         "genrep",       UT_TYPE_STRING,10,caGenrep,
                         "selsid",UT_TYPE_STRING,1000,caSelsid,
                         "update",UT_TYPE_STRING,10,caUpdate,
                         "start",UT_TYPE_STRING,10,caStart,
                         "id",     UT_TYPE_STRING, 10,caId,
                    "sdate",   UT_TYPE_STRING, 10,caSdate1,
                    "edate",   UT_TYPE_STRING, 10, caEdate1);
                         
///      if(strlen(caContent)>0){
//          strcpy(caTemp_d,  pasCvtGBK(2,caContent,caTemp,63)); 
//          strcpy(caContent,caTemp_d);
//      }
                  

      lRowNum=atol(caLimit);
      if(lCurPg<=0) lCurPg=1;
      lStartRec = atol(caStart);
      lSid = atol(caSid);
      lId=atol(caId);
      psDbHead = utPltInitDb(); 
      
      if(strlen(caGenrep)>0){        
           utComSetVar_s(psShmHead,"reptaskid",caId);
 //          printf("caId=%s\n",caId);
//            utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","进入后台统计，统计需要一段时间，结果请在<历史报表查看>中查阅!","进入后台统计，统计需要一段时间，结果请在<历史报表查看>中查阅!");
//        return 0;        
    }
      
      
      

      if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
          pasDbExecSqlF("delete from taskdet where sid  in(%s)",caSelsid);
      }
      
      
      pHash = (unsigned char *)pasLHashInit(1500,1500,sizeof(struct stData_s),0,4);    
    if(pHash == NULL) {         
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),"内存分配错误",pasDbErrorMsg(NULL));
        return 0;
    }
  
      sprintf(caTemp,"select groupid,groupname from ncsgroup ");  
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
        if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
      
        lId=0;
        memset(caName,0,sizeof(caName));
      i=0;
      iReturn = pasDbFetchInto(psCur,
                                       UT_TYPE_LONG,4,&lId,
                                       UT_TYPE_STRING,31,caName);
                                                                         
                                      
    while((iReturn==0)||(iReturn==1405))
    {
         psData = (struct stData_s *)pasLHashLookA(pHash,&lId);  
       if(psData){
           strcpy(psData->name,caName);
       }
    
          lId=0;
            memset(caName,0,sizeof(caName));
          iReturn = pasDbFetchInto(psCur,
                                       UT_TYPE_LONG,4,&lId,
                                       UT_TYPE_STRING,31,caName);
                                                           
    }    
    pasDbCloseCursor(psCur);  
       
       
         sprintf(caTemp,"select count(*) from taskdet where 1=1");
       sprintf(caTemp+strlen(caTemp)," and id=%d ",atol(caId));
       printf("caTemp=%s\n",caTemp);
       iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0561数据统计出错"));
        return 0;
    }
       
       
       
       
      
    
      
        sprintf(caTemp,"select sid,rpname,id,worktime,sortby,tid,did,opt,gname  from taskdet  where 1=1");
         sprintf(caTemp+strlen(caTemp)," and id=%d ",atol(caId));
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
   memset(caRpname,0,sizeof(caRpname));
   lId=0;
   memset(caWorktime,0,sizeof(caWorktime));
   memset(caSortby,0,sizeof(caSortby));
   memset(caTid,0,sizeof(caTid));
   memset(caDid,0,sizeof(caDid));
   memset(caOpt,0,sizeof(caOpt));
   memset(caGname,0,sizeof(caGname));
   
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
                                       UT_TYPE_STRING,30,caRpname,
                                         UT_TYPE_LONG,4,&lId,
                                         UT_TYPE_STRING,10,caWorktime,
                                         UT_TYPE_STRING,10,caSortby,
                                         UT_TYPE_STRING,10,caTid,
                                         UT_TYPE_STRING,10,caDid,
                                         UT_TYPE_STRING,10,caOpt,
                                         UT_TYPE_STRING,30,caGname);
                                        
                                                                       
                                        
                                        
       if((iReturn!=0)&&(iReturn!=1405)) break;    
        iNum++;
       if(iNum>1){
           utPltPutLoopVar(psDbHead,"dh",iNum,",");
       }
           utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",lId);
        utPltPutLoopVar(psDbHead,"rpname",iNum,caRpname);
        memset(caMsg,0,sizeof(caMsg));
        
        if(atol(caTid)>0){
            lTid=atol(caTid);
         psData = (struct stData_s *)pasLHashLook(pHash,&lTid);  
           if(psData){
                 sprintf(caMsg+strlen(caMsg)," 单位组:%s",psData->name);
                   
           }
      
        }
        if(atol(caDid)>0){
            sprintf(caMsg+strlen(caMsg)," 单位:%s",ncsUtlGetClientNameById(psShmHead,atol(caDid),"未知"));
        }
        if(strlen(caGname)>0){
            sprintf(caMsg+strlen(caMsg)," 部门包含:%s",caGname);
        }
        if(atol(caWorktime)==1){
            sprintf(caMsg+strlen(caMsg)," 时间段:工作时间");
        }
        if(atol(caWorktime)==2){
            sprintf(caMsg+strlen(caMsg)," 时间段:非工作时间");
        }
        if(atol(caOpt)==1){
            sprintf(caMsg+strlen(caMsg)," 统计对象:单位");
        }
        else if(atol(caOpt)==2){
            sprintf(caMsg+strlen(caMsg)," 统计对象:部门");
        }
        else if(atol(caOpt)==3){
            sprintf(caMsg+strlen(caMsg)," 统计对象:个人");
        }
        if(atol(caSortby)==1){
            sprintf(caMsg+strlen(caMsg)," 排序:流量");
        }
        else if(atol(caSortby)==2){
            sprintf(caMsg+strlen(caMsg)," 排序:百分比");
        }
        else if(atol(caSortby)==3){
            sprintf(caMsg+strlen(caMsg)," 排序:单位");
        }
       utPltPutLoopVar(psDbHead,"msg",iNum,caMsg);
            
    }
    pasDbCloseCursor(psCur);
      

      utPltPutVarF(psDbHead,"TotRec","%d",lCount);
      if(pHash) free(pHash);
      utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_taskdet_list.htm");
      return 0;
}


//显示历史报表下载页面
int ncReportHis_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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

    char caAction[6];
    char caResult[16];
   
    char caDesc_temp[300];
    char caSdate_all[24],caEdate_all[24];
    long lStime,lEtime;

     pasDbCursor *psCur;
 
  char caSelsid[1024],caDel[16],caExp[16];
  char caKeyword[64],caTime_flag[16];
  char caPage[12],caLimit[16],caSort[16],caDir[16];

  char caName[136],caSdate_d[20],caEdate_d[20],caEtime[16];
  char caDescr[128];

  char caTemp_d[68];
  lCurPg=1;
     strcpy(caWhere,"\0");
    
    iReturn = utMsgGetSomeNVar(psMsgHead,12,
                               "TotPg",      UT_TYPE_STRING, 12, caTotPg,
                                "CurPg",      UT_TYPE_STRING, 12, caCurPg,
                                "sdate",UT_TYPE_STRING,19,caSdate,
                                "edate",UT_TYPE_STRING,19,caEdate,
                                "page",            UT_TYPE_STRING,10,caPage,
                                "limit",           UT_TYPE_STRING,10,caLimit,
                                "sort",            UT_TYPE_STRING, 15, caSort,
                                "dir",          UT_TYPE_STRING, 8,  caDir,
                                "selsid",UT_TYPE_STRING,1000,caSelsid,
                                "keyword",UT_TYPE_STRING,30,caKeyword,
                                "del",UT_TYPE_STRING,10,caDel,
                                "exp",UT_TYPE_STRING,10,caExp);
    
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

 
   utStrDelSpaces(caKeyword);           
  
          if(strcmp(caDel,"del")==0&&strlen(caSelsid)>0){
                        iReturn=pasDbExecSqlF("delete from ncsreportlog where sid in (%s)",caSelsid);
                    

          
      }
    
    
    
    sprintf(sqlbuf,"select count(*) from ncsreportlog where 1=1 ");
    if(strlen(caSdate)>0){
        sprintf(sqlbuf+strlen(sqlbuf)," and stime>=%d ",lstime);
    }
    if(strlen(caEdate)>0){
        sprintf(sqlbuf+strlen(sqlbuf)," and stime<=%d ",letime);
    }
    if(strlen(caKeyword)>0){
        sprintf(sqlbuf+strlen(sqlbuf)," and reportname like '%c%s%c' ",'%',caKeyword,'%' );
    }
    lCount=0;
    pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lCount);
  
    sprintf(sqlbuf,"select sid,reportname,stime,filename from ncsreportlog where 1=1 ");
        if(strlen(caSdate)>0){
        sprintf(sqlbuf+strlen(sqlbuf)," and stime>=%d ",lstime);
    }
    if(strlen(caEdate)>0){
        sprintf(sqlbuf+strlen(sqlbuf)," and stime<=%d ",letime);
    }
    if(strlen(caKeyword)>0){
        sprintf(sqlbuf+strlen(sqlbuf)," and reportname like '%c%s%c' ",'%',caKeyword,'%' );
    }
    
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
    memset(caName,0,sizeof(caName));
    lStime=0;
    memset(caDescr,0,sizeof(caDescr));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lSid,
                                   UT_TYPE_STRING,120,caName,
                                   UT_TYPE_LONG,4,&lStime,
                                   UT_TYPE_STRING,120,caDescr);
                                   
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
        utPltPutLoopVar(psDbHead,"stime", iNum,utTimFormat("%Y/%m/%d %H:%M:%S",lStime));
        utPltPutLoopVar(psDbHead,"reportname",  iNum,caName);
        utPltPutLoopVar(psDbHead,"file",iNum,caDescr);  
            utPltSetCvtHtml(0);
     
    lSid=0;
    memset(caName,0,sizeof(caName));
    lStime=0;
    memset(caDescr,0,sizeof(caDescr));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lSid,
                                   UT_TYPE_STRING,120,caName,
                                   UT_TYPE_LONG,4,&lStime,
                                   UT_TYPE_STRING,120,caDescr);
    }



    lTotPg = (lCount -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lCount);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        
//     utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_hisreport_list.htm");
    return 0;
}


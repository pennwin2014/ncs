/* 网络督察管理中心    
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <utoall.h>
#include "ncsdef.h"
#include "pasdb.h"
int ncsTcpInit(utShmHead *psShmHead);
int ncsInit(utShmHead *psShmHead);
int ncsLoadSmsProcy(utShmHead *psShmHead);
int ncsLoadId2Code(utShmHead *psShmHead);
int    iDebugLevel=0;
char *pHashEndgroup;
char *pHashNettype;
char *pHashWebclass;
char *pHashService;
int ncsStartSomeProcess_mac(utShmHead *psShmHead);
int main(int argc,char **argv)
{
	utShmHead *psShmHead;
	int iReturn,i;
	unsigned long lMax;
	char caConfFile[64];
	char caVar[32];
	char *pLog;
	
	
	
	
	strcpy(caConfFile,"pqconf_ncs.cnf");
    pasKillProcess(NULL,"proeim_server");
    printf("ProEim Server  %s  Release Date %s \n",NCS_VERSION,NCS_RELEASETIME);
    psShmHead = (utShmHead *)pasServerInit777(caConfFile);
    if(psShmHead == NULL) {
         printf(" Memory Init Error \n");
         exit(9);
    }
    pasSetLogFile(NCS_RUNLOG);
    pasSetLogPath("../log");
    lMax = utComGetVar_ld(psShmHead,"MaxLogSize",50000000L);
    pasSetMaxLogSize(lMax);    
	pLog = utComGetVar_s(psShmHead,"DebugInfo");
  	i = 1;
	while(pLog || i < 10) {
	    if(pLog) {
    	    printf("Set DebugInfo:[%s]\n",pLog);
        	pasSetDebugLogInfo(pLog);
        }
    	sprintf(caVar,"DebugInfo%d",i);
	    pLog = utComGetVar_s(psShmHead,caVar);
    	i++;
    }
  
  
//  ncUtlSms_JS(psShmHead,"13816246346","您本次上网验证码:11111，【上海】");
  
  
    
    iReturn = pasConnect(psShmHead); /* 连接数据库  */
    if(iReturn != 0) {
        printf(" DataBase Connect Error \n");
    }
    
    ncsInitEndgroup(psShmHead);
    ncsInitNettype(psShmHead);
    ncsInitWebclass(psShmHead);
    ncsInitServiceIndex(psShmHead);
    iReturn = ncsInit(psShmHead);
    iReturn = ncsInitMinId(psShmHead);   /* 设置最小ID  */
    iReturn = ncsInitProeim(psShmHead);
  
    if(iReturn != 0) {
        printf(" Proeim Init Error \n");
    }
    else {
        printf(" ProEIM Init OK! \n");
    }
    //加载短信发送策略
//    ncsLoadSmsProcy(psShmHead);
    //加载短信用户
//    iReturn=ncsLoadMobuser(psShmHead);
ncsLoadId2Code(psShmHead);

    printf("iReturn=%d\n",iReturn);
    ncsUpdateServerInit(psShmHead);
    utComSetMaxTcpBuf(1024000L);
    dsCltSetSessionType("email",UT_TYPE_STRING,63,1);
    dsCltSetSessionType("groupname",UT_TYPE_STRING,31,1);
    dsCltSetSessionType("groupid",  UT_TYPE_LONG,4,1);
    dsCltSetSessionType("id",  UT_TYPE_LONG,4,1);
    dsCltSetSessionType("dispname",UT_TYPE_STRING,32,1);
    dsCltSetSessionType("plate", UT_TYPE_STRING,63,0);
    pasPyLoad(NULL);
    pasDbClose(NULL);
   
    
 //   iReturn = ncsStartSomeProcess(psShmHead);
    
    
    //启动mac嗅探专用的进程
    ncsStartSomeProcess_mac(psShmHead);
    iReturn = dsCltInit(psShmHead);
    if(iReturn < 0) {
        printf(" Start Dsclt Error ");
        exit(9);
    }
    
 /*   
       if(strcasecmp(utComGetVar_sd(psShmHead,"smpfws","bizsms"),"smsmoden")==0){
    	setenv("LANG", "en-US.UTF-8", 1);
	
				if((iReturn = modem_init() < 0))
				{
					printf("init smsmoden err\n");
				}
				printf("ggggggg\n");
    }
 */
    iReturn = pasTcpServer(psShmHead,ncsTcpInit);    /* Tcp处理进程   */
    if(iReturn < 0) {
        exit(9);
    }
    pasOnTimeDo(psShmHead);
    utShmClose(psShmHead);

}


int ncsInitMinId(utShmHead *psShmHead)
{
    unsigned long lSid;
    
    lSid = utComGetVar_ld(psShmHead,"MinGroupId",1);
    pasSetSid("ncsgroup","groupid",lSid);
    lSid = utComGetVar_ld(psShmHead,"MinClientId",1);
    pasSetSid("ncsuser","userid",lSid);
    lSid = utComGetVar_ld(psShmHead,"MinCaseId",1);
    pasSetSid("ncscaseinfo","sid",lSid);
    
    return 0;
}

    
int ncsInitEndgroup(utShmHead *psShmHead)
{
    char *pPltPath;
    int iReturn,i,iSum;
    long lTime,iMax;
    long lDid,lGid;
    char sqlbuf[1024];
    char caGname[36];
    pasDbCursor *psCur;
    char *p;
    struct ncsGid_s {
            unsigned long did;
            unsigned long gid;
    };
    struct ncsGid_s  stGid; 
    struct ncsEndGroup_s *psData;
    iMax = utComGetVar_ld(psShmHead,"ncsEndgroup",50000);
  
    pHashEndgroup = (unsigned char *)pasLHashInit(iMax,iMax,sizeof(struct ncsEndGroup_s),0,8);    
    if(pHashEndgroup == NULL) {

         return (-1);
    }
    sprintf(sqlbuf,"select did,gid,dispname from ncsendgroup limit 0,49999 ");

    psCur=pasDbOpenSql(sqlbuf,0);
    if(psCur==NULL){
 
    	free(pHashEndgroup);
    	return 0;
    }
    lDid=0;
    lGid=0;
    memset(caGname,0,sizeof(caGname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lDid,
                                 UT_TYPE_LONG,4,&lGid,
                                 UT_TYPE_STRING,31,caGname);
                                 printf("iReturn=%d\n",iReturn);
     while(iReturn==0||iReturn==1405)
     {
     	  stGid.did=lDid;
     	  stGid.gid=lGid;
     	  psData = (struct ncsEndGroup_s *)pasLHashLookA(pHashEndgroup,(char *)&stGid);  
     	  if(psData){
     	  	strcpy(psData->name,caGname);
     	  	
     	  }
     	  
     	  
     	  lDid=0;
		    lGid=0;
		    memset(caGname,0,sizeof(caGname));
		    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lDid,
		                                 UT_TYPE_LONG,4,&lGid,
		                                 UT_TYPE_STRING,31,caGname);
     }
     pasDbCloseCursor(psCur); 
    
    return 0;

}    

//初始化虚拟身份类别   
int ncsInitNettype(utShmHead *psShmHead)
{
    char *pPltPath;
    int iReturn,i,iSum;
    long lTime,iMax;
    long lDid,lGid;
    char sqlbuf[1024];
    char caGname[36];
    pasDbCursor *psCur;
    char *p;
  
    struct ncsNettype_s *psData;
    iMax = utComGetVar_ld(psShmHead,"ncsNettype",3000);
  
    pHashNettype = (unsigned char *)pasLHashInit(iMax,iMax,sizeof(struct ncsNettype_s),0,4);    
    if(pHashNettype == NULL) {

         return (-1);
    }
    sprintf(sqlbuf,"select ntype,name from ncsnettype limit 0,2999 ");

    psCur=pasDbOpenSql(sqlbuf,0);
    if(psCur==NULL){
 
    	free(pHashNettype);
    	return 0;
    }
    lDid=0;
    lGid=0;
    memset(caGname,0,sizeof(caGname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lDid,
                                 UT_TYPE_STRING,31,caGname);
                               
     while(iReturn==0||iReturn==1405)
     {

     	  psData = (struct ncsNettype_s *)pasLHashLookA(pHashNettype,(char *)&lDid);  
     	  if(psData){    	  	
     	  	strcpy(psData->caName,caGname);
     	  
     	  }
     	  
    lDid=0;
    lGid=0;
    memset(caGname,0,sizeof(caGname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lDid,
                                 UT_TYPE_STRING,31,caGname);
     }
     pasDbCloseCursor(psCur); 
    
    return 0;

}   
 
//初始化网址类别   
int ncsInitWebclass(utShmHead *psShmHead)
{
    char *pPltPath;
    int iReturn,i,iSum;
    long lTime,iMax;
    long lDid,lGid;
    char sqlbuf[1024];
    char caGname[36];
    pasDbCursor *psCur;
    char *p;
  
    struct ncsNettype_s *psData;
    iMax = utComGetVar_ld(psShmHead,"ncsWebclass",300);
  
    pHashWebclass = (unsigned char *)pasLHashInit(iMax,iMax,sizeof(struct ncsNettype_s),0,4);    
    if(pHashWebclass == NULL) {

         return (-1);
    }
    sprintf(sqlbuf,"select id,name from ncwebclass limit 0,299 ");

    psCur=pasDbOpenSql(sqlbuf,0);
    if(psCur==NULL){
 
    	free(pHashWebclass);
    	return 0;
    }
    lDid=0;
    lGid=0;
    memset(caGname,0,sizeof(caGname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lDid,
                                 UT_TYPE_STRING,31,caGname);
                               
     while(iReturn==0||iReturn==1405)
     {

     	  psData = (struct ncsNettype_s *)pasLHashLookA(pHashWebclass,(char *)&lDid);  
     	  if(psData){    	  	
     	  	strcpy(psData->caName,caGname);
	  
     	  }
     	  
    lDid=0;
    lGid=0;
    memset(caGname,0,sizeof(caGname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lDid,
                                 UT_TYPE_STRING,31,caGname);
     }
     pasDbCloseCursor(psCur); 
    
    return 0;

} 
 
 //初始化网络服务类别   
int ncsInitServiceIndex(utShmHead *psShmHead)
{
    char *pPltPath;
    int iReturn,i,iSum;
    long lTime,iMax;
    long lDid,lGid;
    char sqlbuf[1024];
    char caGname[36];
    pasDbCursor *psCur;
    char *p;
  
    struct ncsNettype_s *psData;
    iMax = utComGetVar_ld(psShmHead,"ncsServiceIndex",5000);
  
    pHashService = (unsigned char *)pasLHashInit(iMax,iMax,sizeof(struct ncsNettype_s),0,4);    
    if(pHashService == NULL) {

         return (-1);
    }
    sprintf(sqlbuf,"select sid,name from ncipindex limit 0,4999 ");

    psCur=pasDbOpenSql(sqlbuf,0);
    if(psCur==NULL){
 
    	free(pHashService);
    	return 0;
    }
    lDid=0;
    lGid=0;
    memset(caGname,0,sizeof(caGname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lDid,
                                 UT_TYPE_STRING,31,caGname);
                               
     while(iReturn==0||iReturn==1405)
     {

     	  psData = (struct ncsNettype_s *)pasLHashLookA(pHashService,(char *)&lDid);  
     	  if(psData){    	  	
     	  	strcpy(psData->caName,caGname);
     	  
     	  }
     	  
    lDid=0;
    lGid=0;
    memset(caGname,0,sizeof(caGname));
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lDid,
                                 UT_TYPE_STRING,31,caGname);
     }
     pasDbCloseCursor(psCur); 
    
    return 0;

} 
    

int ncsInit(utShmHead *psShmHead)
{
    char *pPltPath;
    int iReturn,i,iSum;
    long lTime;
    char *p;
    pPltPath = utComGetVar_s(psShmHead,"PLATEPATH");
    if(pPltPath) {
        utPltSetPlatePath(pPltPath);
    }
    iDebugLevel = utComGetVar_ld(psShmHead,"DebugLevel",0);
    pasSetLogLevel(iDebugLevel);
    ncsInitWebFun(psShmHead);
    procapWeb001Fun(psShmHead);
    ncsInitncsFun_v4();
    ncsInitWebServiceFun(psShmHead);
    init_se();                   //对全文搜索的函数进行注册
    ncsInitNccFun(psShmHead);   /* Add By Liyunming at 1006/05/04 */
    ncsInitComFun(psShmHead);
    if(strcasecmp(utComGetVar_sd(psShmHead,"SubAdmMgr","No"),"Yes")==0) {
        ncsSrvInitFun(psShmHead);
    }   
    if(strcasecmp(utComGetVar_sd(psShmHead,"SynIndServer","No"),"Yes")==0) {
        ncsIndSrvInitFun(psShmHead);
    }   

    p = utComGetVar_sd(psShmHead,"SaveNetLog2File","No");
    if(strcasecmp(utComGetVar_sd(psShmHead,"StartLog2Load","No"),"Yes")==0 || strcasecmp(p,"Yes")==0) {
        ncsUtlSetLog2DbFlags(1,utComGetVar_sd(psShmHead,"Log2LoadLogPath","/data/fss/db"));
    }
    else {
        ncsUtlSetLog2DbFlags(0,NULL);
    }
    
    p = utComGetVar_sd(psShmHead,"SaveMacLogTo","Db"); // Db,LoadFile,DbandLoadFile
    if(strcasecmp(p,"LoadFile")==0) {
        char caMacLoadPath[128];
        strcpy(caMacLoadPath,utComGetVar_sd(psShmHead,"MacLogPath","/data/load"));
        pasUtlOut2LogSetLogPath(caMacLoadPath,102400000L);
        ncsUtlSetMacLogTo(1);
    }
    else if(strcasecmp(p,"DbLoadFile") ==0) {
        char caMacLoadPath[128];
        strcpy(caMacLoadPath,utComGetVar_sd(psShmHead,"MacLogPath","/data/load"));
        pasUtlOut2LogSetLogPath(caMacLoadPath,102400000L);
        ncsUtlSetMacLogTo(2);
    }
    else {
        ncsUtlSetMacLogTo(0);
    }

    pasLoadIpdb();                       //加载IP位置数据库
    return 0;

}


int ncsTcpInit(utShmHead *psShmHead)
{
    int iReturn;
    iReturn = pasConnect(psShmHead);
    if(iReturn != 0) {
        utSysLog("Connect Database Error Sqlcode is %d",iReturn);
        exit(9);
    }
    return 0;
}


int ncsLoadSmsProcy(utShmHead *psShmHead)
{
  	ncsSmsOnline *psData;
	  char caSn[16],caUpdate[16];
	  pasDbCursor *psCur;
	  long lFlag,lLimcount,lCount,iReturn;
	  char caTemp[1024];
    utShmFreeHash(psShmHead,NCS_LNK_SMSONLINE);
    utShmHashInit(psShmHead,NCS_LNK_SMSONLINE,10000,10000,sizeof(ncsSmsOnline),0,16);
    sprintf(caTemp,"select sn,flag,limcount,lcount,uptime from ncssmsonline ");
    psCur=pasDbOpenSql(caTemp,0);
    if(psCur){
    	memset(caSn,0,sizeof(caSn));
    	lFlag=0;
    	lLimcount=0;
    	lCount=0;
    	memset(caUpdate,0,sizeof(caUpdate));
    	iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,14,caSn,
                                 UT_TYPE_LONG,4,&lFlag,
                                 UT_TYPE_LONG,4,&lLimcount,
                                 UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_STRING,11,caUpdate);
     while(iReturn==0||iReturn==1405)
     { 
     	 
		     psData=(ncsSmsOnline *)utShmHashLookA(psShmHead,NCS_LNK_SMSONLINE,caSn);
			   if(psData){
			   	psData->lFlag=lFlag;
			   	psData->lLimCount=lLimcount;
			   	psData->lCount=lCount;
			   	strcpy(psData->caUptime,caUpdate);
			   }
     	    memset(caSn,0,sizeof(caSn));
		    	lFlag=0;
		    	lLimcount=0;
		    	lCount=0;
		    	memset(caUpdate,0,sizeof(caUpdate));
		    	iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,14,caSn,
		                                 UT_TYPE_LONG,4,&lFlag,
		                                 UT_TYPE_LONG,4,&lLimcount,
		                                 UT_TYPE_LONG,4,&lCount,
		                                 UT_TYPE_STRING,11,caUpdate);
     }                         
    	
    	
    	pasDbCloseCursor(psCur); 
    }
    
    return 0;
}


int ncsLoadMobuser(utShmHead *psShmHead)
{
  	ncsSmsNo *psData;
	  char caNo[24],caPass[24],caMac[24];
	  pasDbCursor *psCur;
	  long iReturn;
	  long lLasttime;
	  char caTemp[1024];
    utShmFreeHash(psShmHead,NCS_LNK_SMSNO);
    iReturn = utShmHashInit(psShmHead,NCS_LNK_SMSNO,500000,500000,sizeof(ncsSmsNo),0,20);
    
    sprintf(caTemp,"select username,password,mac,lasttime from ncmobuser where lasttime>%d order by lasttime desc limit 0,100000 ",time(0)-3600*24);

    psCur=pasDbOpenSql(caTemp,0);
    if(psCur){
      memset(caNo,0,sizeof(caNo));
      memset(caPass,0,sizeof(caPass));
      memset(caMac,0,sizeof(caMac));
      lLasttime=0;
    	iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,19,caNo,
    	                             UT_TYPE_STRING,19,caPass,
    	                             UT_TYPE_STRING,19,caMac,
    	                             UT_TYPE_LONG,4,&lLasttime);
            
     while(iReturn==0||iReturn==1405)
     { 
 //    	 printf("caNo=%s\n",caNo);
		     psData=(ncsSmsNo *)utShmHashLookA(psShmHead,NCS_LNK_SMSNO,caNo);
			   if(psData){
			    strcpy(psData->caPass,caPass);
			   	strcpy(psData->caMac,caMac);
			   	psData->flag=0;
			   	psData->lasttime=lLasttime;
		//	   	printf("caNo=%s\n",caNo);
			   }
		      memset(caNo,0,sizeof(caNo));
		      memset(caPass,0,sizeof(caPass));
		      memset(caMac,0,sizeof(caMac));
		      lLasttime=0;
		      lLasttime=0;
		    	iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,19,caNo,
		    	                             UT_TYPE_STRING,19,caPass,
		    	                             UT_TYPE_STRING,19,caMac,
		    	                             UT_TYPE_LONG,4,&lLasttime);
     }                         
    	
    	
    	pasDbCloseCursor(psCur); 
    }
    else{
  //  	printf("ffffffffffffffffffff\n");
    }
    
    return utShmHashRecord(psShmHead,NCS_LNK_SMSNO);
}




//加载场所ID和Code对照表
int ncsLoadId2Code(utShmHead *psShmHead)
{
  	ncsuserCode2Id *psData;
	  char caCode[36];
	  unsigned long lId;
	  pasDbCursor *psCur;
	  long iReturn;
	  long lLasttime;
	  char caTemp[1024];
    utShmFreeHash(psShmHead,NCS_LNK_CLINTCODETOID);
    iReturn = utShmHashInit(psShmHead,NCS_LNK_CLINTCODETOID,50000,50000,sizeof(ncsuserCode2Id),0,14);
    
    sprintf(caTemp,"select username,userid from ncsuser  limit 0,50000 ");

    psCur=pasDbOpenSql(caTemp,0);
    if(psCur){
     memset(caCode,0,sizeof(caCode));
     lId=0;
    	iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,15,caCode,
    	                             UT_TYPE_ULONG,4,&lId);
    	                           
            
     while(iReturn==0||iReturn==1405)
     { 
 //    	 printf("caNo=%s\n",caNo);
		     psData=(ncsuserCode2Id *)utShmHashLookA(psShmHead,NCS_LNK_CLINTCODETOID,caCode);
			   if(psData){			    
			   	psData->id=lId;			  
			   }
         memset(caCode,0,sizeof(caCode));
         lId=0;
    	   iReturn=pasDbFetchInto(psCur,UT_TYPE_STRING,15,caCode,
    	                             UT_TYPE_ULONG,4,&lId);
     }                         
    	
    	
    	pasDbCloseCursor(psCur); 
    }
    else{
  //  	printf("ffffffffffffffffffff\n");
    }
    
    return utShmHashRecord(psShmHead,NCS_LNK_CLINTCODETOID);
}
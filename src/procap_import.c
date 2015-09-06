/*
     Modify 2015/8/14   by zyn*/
#define PAS_SRCFILE      9888     
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
#include <time.h>

static char *getDsGroupids()
{
    static char caReturn[1024];
    char caIds[1024], caTempid[1024];
    char caOpt[256];
    int iReturn, lId, iNum;
    pasDbCursor *psCur;
    memset(caReturn, 0, sizeof(caReturn));
    lId = 0;

    iReturn = dsCltGetMyInfo(1, "Userid", &lId);
    //printf("dsiReturn=%d\n", iReturn);
    if(iReturn != 0 || lId <= 0)
    {
        return &caReturn[0];
    }

    memset(caOpt, 0, sizeof(caOpt));
    pasDbOneRecord("select opt from dsuser where id=:id", 1, "id", UT_TYPE_LONG, lId, UT_TYPE_STRING, 255, caOpt);
    if(strstr(caOpt, "all"))
    {
        return &caReturn[0];
    }



    sprintf(caReturn, "%s", caOpt);
    sprintf(caIds, "%s", caOpt);

    while(strlen(caIds) > 0)
    {
        memset(caTempid, 0, sizeof(caTempid));
        iNum = 0;
        //printf("select groupid from ncsgroup where pid in(%s)\n ", caIds);
        psCur = pasDbOpenSqlF("select groupid from ncsgroup where pid in(%s) ", caIds);
        if(psCur)
        {
            iReturn = 0;
            iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lId);
            while(iReturn == 0)
            {
                if(iNum == 0)
                {
                    sprintf(caTempid, "%d", lId);
                }
                else
                {
                    sprintf(caTempid + strlen(caTempid), ",%d", lId);
                }

                iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lId);

                iNum++;

            }
        }
        pasDbCloseCursor(psCur);

        if(strlen(caTempid) > 0)
        {
            sprintf(caReturn + strlen(caReturn), ",%s", caTempid);
        }
        strcpy(caIds, caTempid);
    }



    return &caReturn[0];
}

static char *getServicecodesByDids(utShmHead *psShmHead, char *caGroupids)
{
    static char caReturn[8024];
    char caAllGroupIds[8024];
    char sql[8024];
    char caServicecode[32];
    int iReturn, lId, iNum;
    char caGroupId[12] = {0};
    pasDbCursor *psCur;
    memset(caReturn, 0, sizeof(caReturn));
    memset(caAllGroupIds, 0, sizeof(caAllGroupIds));
    strcpy(caAllGroupIds, caGroupids);
    if(strlen(caAllGroupIds) == 0)
        strcpy(caAllGroupIds, "0");
    int isBreak = 0;

    for(iNum = 0; iNum < 3; iNum++)
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select groupid from ncsgroup where groupid in (%s) or pid in (%s)", caAllGroupIds, caAllGroupIds);
        //printf("iNum=%d,sql=[%s],groupids=[%s]\n", iNum, sql, caAllGroupIds);
        /*
            //放到sql以后就可以清空
            memset(caAllGroupIds, 0, sizeof(caAllGroupIds));
            */
        psCur = pasDbOpenSqlF(sql);
        if(psCur)
        {
            iReturn = 0;
            memset(caGroupId, 0, sizeof(caGroupId));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 11, caGroupId);
            while(iReturn == 0)
            {
                //不断壮大的groupid列表
                if(strlen(caAllGroupIds) == 0)
                {
                    sprintf(caAllGroupIds, "%s", caGroupId);
                }
                else
                {
                    if(!strstr(caAllGroupIds, caGroupId))
                        sprintf(caAllGroupIds + strlen(caAllGroupIds), ",%s", caGroupId);
                }
                memset(caGroupId, 0, sizeof(caGroupId));
                iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 11, caGroupId);
            }
        }
        pasDbCloseCursor(psCur);
    }

    iNum = 0;
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select username from ncsuser a,ncsgroup b where a.groupid=b.groupid and (b.groupid in (%s) or b.pid in(%s)) limit 0,300 ", caAllGroupIds, caAllGroupIds);
#ifdef SHOWBAB_DEBUG
    printf("get servicecode list sql=[%s]\n", sql);
#endif
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        iReturn = 0;
        memset(caServicecode, 0, sizeof(caServicecode));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 16, caServicecode);
        while(iReturn == 0)
        {
            //最终结果
            if(iNum == 0)
            {
                sprintf(caReturn, "\'%s\'", caServicecode);
            }
            else
            {
                sprintf(caReturn + strlen(caReturn), ",\'%s\'", caServicecode);
            }
            memset(caServicecode, 0, sizeof(caServicecode));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 16, caServicecode);
            iNum++;
        }
    }
    pasDbCloseCursor(psCur);
    return &caReturn[0];
}

/* 显示热点分组信息导入next界面  */
int ncsDrNext_mac(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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

		printf("-------51ncSenVidDrNext_v9 start-------");
		psDbHead = utPltInitDb();
		if(!utFileIsExist("/home/ncmysql/ncs/upload"))
		{
			if(mkdir("/home/ncmysql/ncs/upload",777)!=0)
			{
				utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0194导入"),ncsLang("0183打开文件出错"));
				return 0;

			}
		}
		system("chmod -Rf 777 /home/ncmysql/ncs/upload");
		//chmod("/home/ncmysql/ncs/upload",777);

		/*取分组描述*/ 
		 sprintf(sqlbuf,"select groupid,groupdesc  from  nctermmacgroup order by groupid");
		
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
	/*取终端分组名称*/ 
   sprintf(sqlbuf,"select groupid,groupname from nctermmacgroup order by groupname ");
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
    while(p && *p) 
	{
        p = utStrGetWord(p,caTemp,300,",\r\n");
        if(!utStrIsSpaces(caTemp))
		{
            iNum++;
            utPltPutLoopVar(psDbHead,"caTemp",iNum,caTemp);
            utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum-1);
            
            if(iNum==1)
            {
				utPltPutVar(psDbHead,"webclass",caTemp);
				utPltPutVarF(psDbHead,"webclass_num","%lu",iNum-1);
            }
        }
        if(*p != ',') 
		{
            break;
        }
        p++;
    }
    fclose(fp);
}
   utPltPutVar(psDbHead,"fname",temp_file);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/MAC_import_next.htm");
	 printf("-------411411411411ncs_imp_MAC end  temp_file=%d-------",temp_file);
    return 0;
}

//导入热点分组信息
int ncs_imp_MAC(utShmHead  *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	FILE *fp;
	int iReturn;
	char caFile[128]="";
	
	char *p;
	long sid,id;
	char caUrl[68],caName[68],caUrlClass[36];
  
    long groupid=0;
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
	char caNtype[64];
	long sum=0;
	utPltDbHead *psDbHead;
	//	psDbHead = utPltInitDb();
	//utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,5,
					"keyword",UT_TYPE_STRING,11,caIdNum,
					"defclass",UT_TYPE_STRING,10,caDefid,
					"sname_type",UT_TYPE_STRING,63,caNtype,
					"first",     UT_TYPE_STRING, 11,  caFirst,
					"fname",   UT_TYPE_STRING, 127, caFile);
		      
	lTime=time(0);
	printf("453ntype=%s\n",caNtype);
	if(utStrIsSpaces(caIdNum))
	{
        lId = 999;
    }
    else {
        lId = atol(caIdNum);
    }
    
    fp = fopen(caFile,"r");
    if(fp == NULL) 
	{
    	 utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0194导入"),ncsLang("0183打开文件出错"));
        return 0;
    }
    p = fgets(caBuf,10000,fp);
    if(!utStrIsSpaces(caFirst)) 
	{  /* 忽略第一行  */
        p = fgets(caBuf,10000,fp);
    }
    while(p) 
	{
        pmUtlGetCsvWord(caBuf,lId,caKeyword);
  
      	id=atol(caDefid);
      
        
		if(!utStrIsSpaces(caKeyword)) 
		{
			lcount=0;
			sprintf(sqlbuf,"select count(*) from nctermmacgroup,nctermmacgroupinfo where nctermmacgroup.groupid=nctermmacgroupinfo.groupid and groupname='%s' ",caNtype);
			//	printf("sqlbuf=%s\n",sqlbuf);		
			pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lcount);
			
			printf("486caNtype=%s\n",caNtype);
			
			sprintf(sqlbuf,"select groupid from nctermmacgroup where groupname=\'%s\' ",caNtype);
		    printf("490 sqlbuf=%s\n",sqlbuf);		
			pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&groupid);
			printf("490 groupid=%lu \n",groupid);	
				sprintf(sqlbuf,"insert into nctermmacgroupinfo(groupid,mac) \
					values(%lu,\'%s\') ",\
					groupid,caKeyword);
				printf("sqlbuf=%s\n",sqlbuf);																																									                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          					                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
				pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
				pasDbCommit(NULL);
			                             
        }
        p = fgets(caBuf,10000,fp);
    }
    fclose(fp);
    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入完成!","导入完成!");   	
	return 0;
}


	/* 显示导入热点mac界面  */
int ncS_macUserDr_Next(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
	//解析文件
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
            utPltPutVar(psDbHead,"Ispname_name",caTemp);
            utPltPutVarF(psDbHead,"Ispname_num","%lu",iNum-1);
            }
			 if(iNum==4)
            {
            utPltPutVar(psDbHead,"phone_name",caTemp);
            utPltPutVarF(psDbHead,"phone_num","%lu",iNum-1);
            }
			 if(iNum==5)
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
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/mac_userimport_next.htm");
    return 0;
}


//导入合规热点mac
int ncs_mac_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	FILE *fp;
	int iReturn;
	char caFile[128]="";
	char caPhoneNum[128]="";
	char caIspnameNum[128]="";
	char caPhone[128]="";
	char *p;
	long sid,id;
	char caUsername[128],caDispname[128],caCompname[128],caIspname[128],caIdtype[128],caIdcard[128]; 
  char caCid[16],caMacNum[128],caMACNum[128];
  long lIspname;
  unsigned long lNettype;
    long lcount=0;
    char caFirst[12]="";
    
    char caDispnameNum[12]="";
    char caIdno[36],caDescr[64];
    char caFlags[16];
    long lUsernameNum,lDispnameNum,lIdno,lDescr,lPhone,lmac;
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
	char caMAC[128]="";
	utPltDbHead *psDbHead;
//	psDbHead = utPltInitDb();
//utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,7,
					"idno",UT_TYPE_STRING,8,caIdno,
					"dispname",UT_TYPE_STRING,8,caDispnameNum,
					//接收
					"Ispname",UT_TYPE_STRING,8,caIspnameNum,
					"first",     UT_TYPE_STRING, 11,  caFirst,
					"fname",   UT_TYPE_STRING, 127, caFile,
					"phone",   UT_TYPE_STRING, 127, caPhoneNum,
                    "mac",   UT_TYPE_STRING, 127, caMACNum					
					);
	lTime=time(0);
	
	printf("strat====%s\n",caIdno);
	
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
    
    if(utStrIsSpaces(caIspnameNum)) {
        lIspname = 999;
    }
    else {
        lIspname = atol(caIspnameNum);
    }
	if(utStrIsSpaces(caPhoneNum)) {
        lPhone = 999;
    }
    else {
        lPhone= atol(caPhoneNum);
    }
	if(utStrIsSpaces(caMACNum)) {
        lmac = 999;
    }
    else {
        lmac= atol(caMACNum);
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
        pmUtlGetCsvWord(caBuf,lIspname,caIspname);
		pmUtlGetCsvWord(caBuf,lPhone,caPhone);
		pmUtlGetCsvWord(caBuf,lmac,caMAC);
    

     
			lcount=0;
			if(strlen(caDispname)>0){
			/*	
			sprintf(sqlbuf,"select count(*) from ncszduser where dispname='%s' and idcard='%s' ",caDispname,caIdno);
			//	printf("sqlbuf=%s\n",sqlbuf);		
			pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lcount);
                */
			if(lcount==0)
			{
				sprintf(sqlbuf,"insert into nchotspotinfo(authflag,ISPemail,ssid,ISPname,ISPphone,servicequlity,servicetype,security) \
					values(1,\'%s\',\'%s\',\'%s\',\'%s\',1,0,1) ",\
					caDispname,caIdno,caIspname,caPhone);
					printf("\n492sqlbuf=%s\n",sqlbuf);
				pasDbExecSql(sqlbuf,0);	
               sprintf(sqlbuf,"insert into nchotmacgroup(ssid,mac) values(\'%s\',\'%s\') ",caIdno,caMAC);
                 printf("\n495sqlbuf=%s\n",sqlbuf);			  
			  pasDbExecSql(sqlbuf,0);	
				pasDbCommit(NULL);  																																	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          					                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

			}   
		}
//			 printf("sqlbuf=%s\n",sqlbuf);		
                  
        
        p = fgets(caBuf,10000,fp);
    }
    fclose(fp);
		printf("720debug");
    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","Sucess!!!","Sucess!!!");   	
	return 0;

}

	/* 显示导入设备管理界面  */
int ncS_APsbDr_Next(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
   /*
   从消息数据包中取指定数量的变量。按指定的长度返回。
函数原型
#include <stdarg.h>
int utMsgGetSomeNVar(utMsgHead *psMsgHead,int iNumVar,...)
   */
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
	//解析文件
    while(p && *p) {
		/*
		从指定的字符串中取一个词。
		函数原型：
		char *utStrGetWord(char *p,char *pOut,
                   short nMaxLen,char *pTerm);
		*/
        p = utStrGetWord(p,caTemp,300,",\r\n");
//        if(!utStrIsSpaces(caTemp)) {
            iNum++;
            utPltPutLoopVar(psDbHead,"caTemp",iNum,caTemp);
            utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum-1);
                       
            if(iNum==1)
            {
            utPltPutVar(psDbHead,"aptype_name",caTemp);
            utPltPutVarF(psDbHead,"aptype_num","%lu",iNum-1);
            }
      
            if(iNum==2)
            {
            utPltPutVar(psDbHead,"apname_name",caTemp);
            utPltPutVarF(psDbHead,"apname_num","%lu",iNum-1);
            }
            if(iNum==3)
            {
            utPltPutVar(psDbHead,"dispname_name",caTemp);
            utPltPutVarF(psDbHead,"dispname_num","%lu",iNum-1);
            }
			 if(iNum==4)
            {
            utPltPutVar(psDbHead,"servicename_name",caTemp);
            utPltPutVarF(psDbHead,"servicename_num","%lu",iNum-1);
            }
			 if(iNum==5)
            {
            utPltPutVar(psDbHead,"code_name",caTemp);
            utPltPutVarF(psDbHead,"code_num","%lu",iNum-1);
            }
			//加入
			//接入厂家名
			 if(iNum==6)
            {
            utPltPutVar(psDbHead,"name_name",caTemp);
            utPltPutVarF(psDbHead,"name_num","%lu",iNum-1);
            } 
			if(iNum==7)
            {
            utPltPutVar(psDbHead,"mac_name",caTemp);
            utPltPutVarF(psDbHead,"mac_num","%lu",iNum-1);
            } 
			if(iNum==8)
            {
            utPltPutVar(psDbHead,"address_name",caTemp);
            utPltPutVarF(psDbHead,"address_num","%lu",iNum-1);
            } 
			if(iNum==9)
            {
            utPltPutVar(psDbHead,"longitude_name",caTemp);
            utPltPutVarF(psDbHead,"longitude_num","%lu",iNum-1);
            } 
			if(iNum==10)
            {
            utPltPutVar(psDbHead,"latitude_name",caTemp);
            utPltPutVarF(psDbHead,"latitude_num","%lu",iNum-1);
            }
			if(iNum==11)
            {
            utPltPutVar(psDbHead,"ssid_name",caTemp);
            utPltPutVarF(psDbHead,"ssid_num","%lu",iNum-1);
            } 
			if(iNum==12)
            {
            utPltPutVar(psDbHead,"line_name",caTemp);
            utPltPutVarF(psDbHead,"line_num","%lu",iNum-1);
            }
			if(iNum==13)
            {
            utPltPutVar(psDbHead,"plate_name",caTemp);
            utPltPutVarF(psDbHead,"plate_num","%lu",iNum-1);
            }
			if(iNum==14)
            {
            utPltPutVar(psDbHead,"flags_name",caTemp);
            utPltPutVarF(psDbHead,"flags_num","%lu",iNum-1);
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
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/case/mac_APsbimport_next.htm");
    return 0;
}


	//导入设备
int ncs_APsb_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	FILE *fp;
	int iReturn;
	char caFile[128]="";
	char caPhoneNum[128]="";
	char caIspnameNum[128]="";
	char caPhone[128]="";
	char *p;
	long sid,id;
	char caUsername[128],caCompname[128],caIspname[128],caIdtype[128],caIdcard[128]; 
  char caCid[16],caMACNum[128];
  long lIspname;
  unsigned long lNettype;
    long lcount=0;
    char caFirst[12]="";

    char caIdno[36],caDescr[64];
    long lUsernameNum,lDispnameNum,lIdno,lDescr,lPhone,lmac;
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
	char caMAC[128]="";
	utPltDbHead *psDbHead;
	char caAPtypeNum[36]="";
	char caAPnameNum[36]="";
	char caDispnameNum[36]="";
	char caServicenameNum[36]="";
	char caCodeNum[36]="";
	char caNameNum[36]="";
	char caMacNum[36]="";
	char caAddressNum[36]="";
	char caLongitudeNum[36]="";
	char caLatitudeNum[36]="";
	char caSsidNum[36]="";
	char caLineNum[36]="";
	char caPlateNum[36]="";
	char caFlagsNum[36]="";
	long lAPtype,lAPname,lServicename,lCode,lName,lMac,lAddress;
	long lLongitude,lLatitude,lSsid,lLine,lPlate,lFlags,lDispname;
	char caAPtype[36]="";
	char caAPname[36]="";
	char caServicename[36]="";
	char caCode[36]="";
	char caName[36]="";
	char caMac[36]="";
	char caAddress[36]="";
	char caLongitude[36]="";
	char caLatitude[36]="";
	char caSsid[36]="";
	char caLine[36]="";
	char caPlate[36]="";
	char caFlags[36]="";
	char caDispname[36]="";
//	psDbHead = utPltInitDb();
//utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,19,
					"idno",UT_TYPE_STRING,8,caIdno,
					//接收
					"Ispname",UT_TYPE_STRING,8,caIspnameNum,
					"first",     UT_TYPE_STRING, 11,  caFirst,
					"fname",   UT_TYPE_STRING, 127, caFile,
					"phone",   UT_TYPE_STRING, 127, caPhoneNum,
                  	
                    "aptype",UT_TYPE_STRING,32,caAPtypeNum,	
					"apname",UT_TYPE_STRING,32,caAPnameNum,
                    "dispname",UT_TYPE_STRING,32,caDispnameNum,					
					"servicename",UT_TYPE_STRING,32,caServicenameNum,	
					"code",UT_TYPE_STRING,32,caCodeNum,	
					"name",UT_TYPE_STRING,32,caNameNum,	
					"mac",UT_TYPE_STRING,32,caMacNum,	
					"address",UT_TYPE_STRING,32,caAddressNum,	
					"longitude",UT_TYPE_STRING,32,caLongitudeNum,	
					"latitude",UT_TYPE_STRING,32,caLatitudeNum,	
					"ssid",UT_TYPE_STRING,32,caSsidNum,	
					"line",UT_TYPE_STRING,32,caLineNum,	
					"plate",UT_TYPE_STRING,32,caPlateNum,	
					"flags",UT_TYPE_STRING,32,caFlagsNum	
									
					);
	lTime=time(0);
	
	printf("strat====%s\n",caIdno);
	// int utStrIsSpaces(char *pIn);判断字符串是否为空。
   
    if(utStrIsSpaces(caIdno)) {
        lIdno = 999;
    }
    else {
        lIdno = atol(caIdno);
    }
    
    if(utStrIsSpaces(caIspnameNum)) {
        lIspname = 999;
    }
    else {
        lIspname = atol(caIspnameNum);
    }
	if(utStrIsSpaces(caPhoneNum)) {
        lPhone = 999;
    }
    else {
        lPhone= atol(caPhoneNum);
    }
	//加入AP设备导入
	
	 if(utStrIsSpaces(caAPtypeNum)) {
        lAPtype = 999;
    }
    else {
        lAPtype= atol(caAPtypeNum);
    }
	
     if(utStrIsSpaces(caAPnameNum)) {
        lAPname= 999;
    }
    else {
        lAPname = atol(caAPnameNum);
    }
	if(utStrIsSpaces(caDispnameNum)) {
        lDispname= 999;
    }
    else {
        lDispname= atol(caDispnameNum);
    }
	
	if(utStrIsSpaces(caServicenameNum)) {
        lServicename= 999;
    }
    else {
        lServicename= atol(caServicenameNum);
    }
	
	if(utStrIsSpaces(caCodeNum)) {
        lCode= 999;
    }
    else {
        lCode= atol(caCodeNum);
    } 
	
	if(utStrIsSpaces(caNameNum)) {
        lName= 999;
    }
    else {
        lName= atol(caNameNum);
    } 
	
	if(utStrIsSpaces(caMacNum)) {
        lMac= 999;
    }
    else {
        lMac= atol(caMacNum);
    } 
	
	if(utStrIsSpaces(caAddressNum)) {
        lAddress= 999;
    }
    else {
        lAddress= atol(caAddressNum);
    }
	
	if(utStrIsSpaces(caLongitudeNum)) {
        lLongitude= 999;
    }
    else {
        lLongitude= atol(caLongitudeNum);
    }
	
	if(utStrIsSpaces(caLatitudeNum)) {
        lLatitude= 999;
    }
    else {
        lLatitude= atol(caLatitudeNum);
    }
	
	if(utStrIsSpaces(caSsidNum)) {
        lSsid= 999;
    }
    else {
        lSsid= atol(caSsidNum);
    } 
	
	if(utStrIsSpaces(caLineNum)) {
        lLine= 999;
    }
    else {
        lLine= atol(caLineNum);
    } 
	
	if(utStrIsSpaces(caPlateNum)) {
        lPlate= 999;
    }
    else {
        lPlate= atol(caPlateNum);
    } 
	
	if(utStrIsSpaces(caFlagsNum)) {
        lFlags= 999;
    }
    else {
        lFlags= atol(caFlagsNum);
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
       
       
        pmUtlGetCsvWord(caBuf,lIdno,caIdno);
        pmUtlGetCsvWord(caBuf,lIspname,caIspname);
		pmUtlGetCsvWord(caBuf,lPhone,caPhone);
	    
		pmUtlGetCsvWord(caBuf,lAPtype,caAPtype);
		pmUtlGetCsvWord(caBuf,lAPname,caAPname);
		pmUtlGetCsvWord(caBuf,lDispname,caDispname);
        pmUtlGetCsvWord(caBuf,lServicename,caServicename);
		pmUtlGetCsvWord(caBuf,lCode,caCode);
		pmUtlGetCsvWord(caBuf,lName,caName);
		pmUtlGetCsvWord(caBuf,lMac,caMac);
		pmUtlGetCsvWord(caBuf,lAddress,caAddress);
		pmUtlGetCsvWord(caBuf,lLongitude,caLongitude);
		pmUtlGetCsvWord(caBuf,lLatitude,caLatitude);
		pmUtlGetCsvWord(caBuf,lSsid,caSsid);
		pmUtlGetCsvWord(caBuf,lLine,caLine);
		pmUtlGetCsvWord(caBuf,lPlate,caPlate);
		pmUtlGetCsvWord(caBuf,lFlags,caFlags);
		

     
			lcount=0;
			if(strlen(caCode)>0){
				
			sprintf(sqlbuf,"select count(*) from ncsfactorycod  where code='%s'",caCode);
			printf("sqlbuf=%s\n",sqlbuf);		
			pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lcount);
			if(lcount==0)
			{
				sprintf(sqlbuf,"insert into ncaplist(aptype,apname,servicename,fcode,mac,address,longitude,latitude,ssid,line,plate,flags,dispname)\
				values(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\') ",\
				caAPtype,caAPname,caServicename,caCode,caMac,caAddress,caLongitude,caLatitude,caSsid,caLine,caPlate,caFlags,caDispname);
				printf("\n492sqlbuf=%s\n",sqlbuf);
				pasDbExecSql(sqlbuf,0);	
				sprintf(sqlbuf,"insert into ncsfactorycod(code,name) values(\'%s\',\'%s\')",caCode,caName);
                printf("\n495sqlbuf=%s\n",sqlbuf);			  
				pasDbExecSql(sqlbuf,0);	
			
			pasDbCommit(NULL);  																																	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          					                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

			}   
		}
//			 printf("sqlbuf=%s\n",sqlbuf);		
                  
        
        p = fgets(caBuf,10000,fp);
    }
    fclose(fp);
    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","apSucess!!!","apSucess!!!");   	
	return 0;

}
//系统运维告警
int  ncsSysystem_alarm(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead){
		
	   
	   char  caAlarmcode[16]="";
	   char  caAlarmlevel[32]="";
	   char  caDevtype[8]="";
	   long  lAlarmtime;
	   long  lUpdatetime;
	   char	 caDescription[128]="";
	   long  lStatus;
	   //用于判断游标状态
	int iReturn = 0;
	 //用于判断行数
	int iNum = 0; 
	long lCount=0;
	char sql[10240]="";
	char sql_user[1024]="";
	char sql_ap[1024]="";
	char caLimit[16]="";
	char caPage[16]="";	
	int ilimit=0;  
	int ipage=0;  
	char caAlarmtime[20]="";
	char caUpdatetime[20]="";
	char caStatus[16]="";
	int status=0;
	char caSid[1024]="";
	long lSid=0;
	int sid=0;
	char description[128]="";
	char caTime_flag[16]="";
	char caAlarm_reason[16]="";
	char caisAlarm[16]="";
	char strCondition[1024]="";
	pasDbCursor *psCur = NULL;
	pasDbCursor *psCur_1 = NULL;
	pasDbCursor *psCur_2= NULL;
	int time_flag=100;
	int Alarm_reason=100;
	int isAlarm=100;
	char caServicecodes[8024]="";
	char sql_count[10240]="";
    char cakeyword[64]="";
	 char caTemp_d[256]="";
	  char caTemp[256]="";
	  char caHandle[16]="";
	utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	 utMsgPrintMsg(psMsgHead);
	//该用户所管辖的区域列表
	 char caGroupSql[8024] = "";
	strcpy(caGroupSql, (char*)getDsGroupcodeSql("servicecode"));
	strcpy(caServicecodes, getServicecodesByDids(psShmHead, getDsGroupids()));
	printf("caServicecodes==[%s]\n",caServicecodes);
	 iReturn = utMsgGetSomeNVar(psMsgHead,9,
									"limit", UT_TYPE_STRING, 10, caLimit,
									"page", UT_TYPE_STRING, 10, caPage,
									"status",UT_TYPE_STRING,10,caStatus,
									"sid",UT_TYPE_STRING,1023,caSid,
									"time_flag",UT_TYPE_STRING,10,caTime_flag,
									"Alarm_reason",UT_TYPE_STRING,10,caAlarm_reason,
									"isAlarm",UT_TYPE_STRING,10,caisAlarm,
									"keyword",UT_TYPE_STRING,63,cakeyword,
									"handle",UT_TYPE_STRING,15,caHandle
									
           );
		   
		   if(strlen(caStatus)>0){
			 status=atoi(caStatus);
		   }
		 if(strlen(caTime_flag)>0)
			{
				time_flag=atoi(caTime_flag);
			}
		  if(strlen(caAlarm_reason)>0)
			{
				Alarm_reason=atoi(caAlarm_reason);
			}
		  if(strlen(caisAlarm)>0)
			{
				isAlarm=atoi(caisAlarm);
			}
		  
			if(strlen(cakeyword)>0)
			{
				strcpy(caTemp_d,pasCvtGBK(2,cakeyword,caTemp,128));
				strcpy(cakeyword,caTemp_d);
			}
		 if(strlen(cakeyword)>0)
			{
				sprintf(strCondition+strlen(strCondition)," and (alarmcode like '%c%s%c' or dispname like '%c%s%c' ) ",'%',cakeyword,'%','%',cakeyword,'%');
			   //printf("1188strCondition=%s",strCondition);
			}
		 
		 if(Alarm_reason==2)
		 {
			sprintf(strCondition+strlen(strCondition)," and (alarmcode  ='10007' or alarmcode='10008')  "); 
		 }else if(Alarm_reason==1)
		 {
			sprintf(strCondition+strlen(strCondition)," and (alarmcode  ='10011' or alarmcode='10012')  ");  
		 }
		 if(isAlarm==0)
		 {
			sprintf(strCondition+strlen(strCondition)," and status=0 "); 
		 }else if(isAlarm==1)
		 {
			sprintf(strCondition+strlen(strCondition)," and status<>0  ");  
		 }
		 
		 if(time_flag==1)
		 {
			sprintf(strCondition+strlen(strCondition)," and alarmtime >=  UNIX_TIMESTAMP(DATE(NOW()))  AND alarmtime <= (UNIX_TIMESTAMP(NOW()))  "); 
		 }else if(time_flag==2)
		 {
			sprintf(strCondition+strlen(strCondition)," and  alarmtime >=  UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY))  AND alarmtime <= (UNIX_TIMESTAMP(NOW())) ");  
		 }else if(time_flag==3)
		 {
			sprintf(strCondition+strlen(strCondition)," and alarmtime >=  UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY))  AND alarmtime <= (UNIX_TIMESTAMP(NOW())) ");  
		 }else if(time_flag==4)
		 {
			sprintf(strCondition+strlen(strCondition)," and  alarmtime >=  UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH))  AND alarmtime <= (UNIX_TIMESTAMP(NOW())) ");  
		 }else if(time_flag==5)
		 {
			 sprintf(strCondition+strlen(strCondition)," and  alarmtime >=  UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH))  AND alarmtime <= (UNIX_TIMESTAMP(NOW())) "); 
		 }else if(time_flag==6)
		 {
			 sprintf(strCondition+strlen(strCondition)," and alarmtime >=  UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH))  AND alarmtime <= (UNIX_TIMESTAMP(NOW()))  "); 
		 }
		 
		 
		 if(strlen(caStatus)>0)
		 {
			
				if(strlen(caHandle)>0)
				{
					sprintf(sql,"update nctermsysalarm set status=%d   where sid in (%s)",status,caSid); 
					printf("%d updatesql=%s",__LINE__,sql);			
					psCur = pasDbOpenSql(sql, 0);	
					memset(sql,0,sizeof(sql));
				}
				else
				{
					if(strlen(caSid)>0)
						{
							sid=atoi(caSid);
						}
					
					sprintf(sql,"update nctermsysalarm set status=%d   where sid=%d",status,sid); 
					psCur = pasDbOpenSql(sql,0); 
					memset(sql,0,sizeof(sql));
				}
		 }
		   ilimit=atoi(caLimit);
			printf("111 ilimit=[%d]\n",ilimit);
			
			ipage=atoi(caPage);
			printf("capage=[%d]\n",caPage);
			printf("%dhhas",__LINE__);
		
		   if(strlen(caGroupSql)==0){
			  sprintf(sql, "select sid,alarmcode,alarmlevel,devtype,from_unixtime(alarmtime),from_unixtime(updatetime),dispname,status from   nctermsysalarm,ncsuser WHERE nctermsysalarm.description=ncsuser.username and  devtype='02'");
              printf("%dsql%s\n",__LINE__,sql);			 
			 sprintf(sql_count,"select count(*) from  nctermsysalarm,ncsuser WHERE nctermsysalarm.description=ncsuser.username and  devtype='02' ");	 
		     printf("%dsql_count=%s\n",__LINE__,sql_count); 
		   }else{
			   sprintf(sql, "select sid,alarmcode,alarmlevel,devtype,from_unixtime(alarmtime),from_unixtime(updatetime),dispname,status from   nctermsysalarm,ncsuser WHERE nctermsysalarm.description=ncsuser.username and  devtype='02' and %s", getDsGroupcodeSql("description"));
			   sprintf(sql_count, "select count(*)  WHERE nctermsysalarm.description=ncsuser.username and  devtype='02' and %s", getDsGroupcodeSql("description"));
			   printf("%dsql%s\n",__LINE__,sql);
			   printf("%dsql_count=%s\n",__LINE__,sql_count);
			 //  sprintf(sql, "select sid,alarmcode,alarmlevel,devtype,from_unixtime(alarmtime),from_unixtime(updatetime),dispname,status from   nctermsysalarm,ncsuser WHERE nctermsysalarm.description=ncsuser.username and  devtype='02' and description in (%s)", caServicecodes);
			  // sprintf(sql_count, "select count(*)  from   nctermsysalarm,ncsuser WHERE nctermsysalarm.description=ncsuser.username and  devtype='02' and description in (%s)", caServicecodes);   
		   }
		   if(strlen(strCondition)>0)
			{
			sprintf(sql+ strlen(sql), " %s ", strCondition);
			sprintf(sql_count+ strlen(sql_count), " %s ", strCondition);
			printf("strCondition=%s\n",strCondition);
			}
			int istart;
			ilimit=40;
			istart = (ipage - 1) * ilimit;		
			
			sprintf(sql + strlen(sql), " order by  sid  desc limit %d,%d",istart,ilimit);

		  printf("%d my  sql=%s\n",__LINE__,sql); 
		  pasDbOneRecord(sql_count, 0, UT_TYPE_LONG, 4, &lCount);
          utPltPutVarF(psDbHead, "TotRec", "%lu", lCount);	
		  printf("%d lCount =%lu\n",__LINE__,lCount); 	
         		  
		  psCur = pasDbOpenSql(sql, 0);	
            		  
		   	   
		  iReturn = pasDbFetchInto( psCur,UT_TYPE_LONG, 4,&lSid,
									UT_TYPE_STRING, sizeof(caAlarmcode)-1,    caAlarmcode,
	                                UT_TYPE_STRING, sizeof(caAlarmlevel)-1,    caAlarmlevel,							
									UT_TYPE_STRING, sizeof(caDevtype)-1,    caDevtype,
									UT_TYPE_STRING, sizeof(caAlarmtime)-1,    caAlarmtime,
									UT_TYPE_STRING,sizeof(caUpdatetime)-1,    caUpdatetime,
									UT_TYPE_STRING, sizeof(caDescription)-1,caDescription,
									UT_TYPE_LONG, 4,    &lStatus
									
								);
								
						
			//			
	while(0 == iReturn || 1405 == iReturn)
	{
	    	  
		iNum++;
		
		if(iNum > 1)
		{
			 //当循环到第二行，增加一个逗号
			utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
		}
		utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
	    utPltPutLoopVar(psDbHead,"alarmcode",iNum,caAlarmcode);
		utPltPutLoopVar(psDbHead,"alarmlevel",iNum,caAlarmlevel);
		utPltPutLoopVar(psDbHead,"devtype",iNum,caDevtype);
		utPltPutLoopVar(psDbHead,"alarmtime", iNum,caAlarmtime);
		utPltPutLoopVar(psDbHead,"updatetime", iNum,caUpdatetime);
		utPltPutLoopVar(psDbHead,"description",iNum,caDescription);
		utPltPutLoopVarF(psDbHead,"status", iNum,"%lu",lStatus);
		 iReturn = pasDbFetchInto( psCur,UT_TYPE_LONG, 4,&lSid,
									UT_TYPE_STRING, sizeof(caAlarmcode)-1,    caAlarmcode,
	                                UT_TYPE_STRING, sizeof(caAlarmlevel)-1,    caAlarmlevel,
									UT_TYPE_STRING, sizeof(caDevtype)-1,    caDevtype,
									UT_TYPE_STRING, sizeof(caAlarmtime)-1,    caAlarmtime,
									UT_TYPE_STRING,sizeof(caUpdatetime)-1,    caUpdatetime,
									UT_TYPE_STRING, sizeof(caDescription)-1,    caDescription,
									UT_TYPE_LONG, 4,&lStatus
									
								);					
	}
	// utPltPutVarF(psDbHead, "TotRec", "%d", iNum);
	
	/*
	memset(sql,0,sizeof(sql));
	 sprintf(sql, "select sid,alarmcode,alarmlevel,devtype,from_unixtime(alarmtime),from_unixtime(updatetime),dispname,status from   nctermsysalarm,ncaplist WHERE nctermsysalarm.description=ncaplist.apname and  devtype='01'");
			if(strlen(strCondition)>0)
			{
			sprintf(sql+ strlen(sql), " %s ", strCondition);
			printf("strCondition%s\n",strCondition);
			}
			//int istart;
			//istart = (ipage - 1) * ilimit;		
			// ilimit = ilimit < (iCount - castart) ?  (capage * ilimit) : iCount;	
			sprintf(sql + strlen(sql), " order by  sid  desc limit %d,%d",istart,ilimit);

		  printf("46 my  sql=%s\n",sql); 
		  psCur = pasDbOpenSql(sql, 0);
		  iReturn = pasDbFetchInto( psCur,UT_TYPE_LONG, 4,&lSid,
									UT_TYPE_STRING, sizeof(caAlarmcode)-1,    caAlarmcode,
	                                UT_TYPE_STRING, sizeof(caAlarmlevel)-1,    caAlarmlevel,							
									UT_TYPE_STRING, sizeof(caDevtype)-1,    caDevtype,
									UT_TYPE_STRING, sizeof(caAlarmtime)-1,    caAlarmtime,
									UT_TYPE_STRING,sizeof(caUpdatetime)-1,    caUpdatetime,
									UT_TYPE_STRING, sizeof(caDescription)-1,caDescription,
									UT_TYPE_LONG, 4,    &lStatus
									
								);
								
						
			//			
	while(0 == iReturn || 1405 == iReturn)
	{
	    	  
		iNum++;
		
		if(iNum > 1)
		{
			 //当循环到第二行，增加一个逗号
			utPltPutLoopVar(psDbHead,"dh",iNum,","); // utPltPutLoopVar输出一个循环变量的值。
		}
		utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
	    utPltPutLoopVar(psDbHead,"alarmcode",iNum,caAlarmcode);
		utPltPutLoopVar(psDbHead,"alarmlevel",iNum,caAlarmlevel);
		utPltPutLoopVar(psDbHead,"devtype",iNum,caDevtype);
		utPltPutLoopVar(psDbHead,"alarmtime", iNum,caAlarmtime);
		utPltPutLoopVar(psDbHead,"updatetime", iNum,caUpdatetime);
		utPltPutLoopVar(psDbHead,"description",iNum,caDescription);
		utPltPutLoopVarF(psDbHead,"status", iNum,"%lu",lStatus);
		 iReturn = pasDbFetchInto( psCur,UT_TYPE_LONG, 4,&lSid,
									UT_TYPE_STRING, sizeof(caAlarmcode)-1,    caAlarmcode,
	                                UT_TYPE_STRING, sizeof(caAlarmlevel)-1,    caAlarmlevel,
									UT_TYPE_STRING, sizeof(caDevtype)-1,    caDevtype,
									UT_TYPE_STRING, sizeof(caAlarmtime)-1,    caAlarmtime,
									UT_TYPE_STRING,sizeof(caUpdatetime)-1,    caUpdatetime,
									UT_TYPE_STRING, sizeof(caDescription)-1,    caDescription,
									UT_TYPE_LONG, 4,&lStatus
									
								);					
	}
	*/
	  //关闭游标
	pasDbCloseCursor(psCur);
	
           	 
		   
	 //把数据以json
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"/normally/proauth_system_alarm.htm");				
	return 0;	
		   
}

//显示接入厂商代码维护
int ncsFacCode_v9(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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

int procapImportSetFun(utShmHead *psShmHead)
{   	
		pasSetTcpFunName("ncs_imp_MAC",ncs_imp_MAC,0); 
		pasSetTcpFunName("ncsDrNext_mac",ncsDrNext_mac,0);
		pasSetTcpFunName("ncs_mac_imp",ncs_mac_imp,0);
		pasSetTcpFunName("ncS_macUserDr_Next",ncS_macUserDr_Next,0);		
		pasSetTcpFunName("ncs_APsb_imp",ncs_APsb_imp,0);
		pasSetTcpFunName("ncS_APsbDr_Next",ncS_APsbDr_Next,0);	
		pasSetTcpFunName("ncsSysystem_alarm",ncsSysystem_alarm,0);	
        pasSetTcpFunName("ncsFacCode_v9",ncsFacCode_v9,0); 		
}
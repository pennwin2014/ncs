/* 监控软件---有关Web的应用(1)                                      */
/*                   2001/12/15   Created By Liyunming  Pronetway   */
/********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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
#include "ncsdef.h"
/* 测试用   */
int ncsTest2006(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    unsigned long lVersion;
#ifdef LDEBUG
utMsgPrintMsg(psMsgHead);    
#endif
    utMsgGetSomeNVar(psMsgHead,1,
                    "version",  UT_TYPE_LONG,  4,&lVersion);
    
    pasTcpResponse(iFd,psMsgHead,
                    NULL,                /* 密钥，暂时不用  */
                    0,
                    "status",    UT_TYPE_LONG,0);
    return 0;
}

/* 用户验证  */
int ncsWebAuth(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
   	char caUsername[33],caPassword[33],caIp[16];
   	char caKey[64],caRight[65];
   	char caModi[24];
   	char caPass0[32];
   	char caCount[8];
   	unsigned long lIp;
   	unsigned long lTime;
   	unsigned long lUserid;
   	long lLevel;
   	char caTemp[512];
   	char caMsg[256],caAuthcount[8];
   	char caScreenwidth[12];
    long lWidth,lRight,lLeft,lRight1;
   	int iCount,lAuthcount;
    utPltDbHead *psDbHead;
    char caWindows_flag[16];
   	int iReturn;
    strcpy(caModi,"\0");
    lTime = time(0);
    strcpy(caAuthcount,utComGetVar_sd(psShmHead,"AuthCount","5"));
    lAuthcount=atol(caAuthcount);
   	iReturn = utMsgGetSomeNVar(psMsgHead,6,
   	                "ModiPass",  UT_TYPE_STRING, 16, caModi,
					"username",  UT_TYPE_STRING, 31, caUsername,
					"password",  UT_TYPE_STRING, 31, caPassword,
					"clientip",  UT_TYPE_STRING, 15, caIp,
					"screenwidth",UT_TYPE_STRING,10,caScreenwidth,
					"windows_flag",UT_TYPE_STRING,5,caWindows_flag);
       lLeft=200;
     if(strlen(caScreenwidth)==0){
						lWidth=1024;
						lRight=1024-lLeft;
						lRight1=lRight+10;
					}
					else{
						lWidth=atol(caScreenwidth);
						lRight=lWidth-lLeft;
						lRight1=lRight+10;
					}
    
    lIp = utComHostAddress(caIp);
    if((iCount=ncsAdmCheckAdmIp(psShmHead,lIp))>lAuthcount) { /* 检查IP地址  */
        psDbHead = utPltInitDbHead();
        utPltPutVarF(psDbHead,"message", "您已连续%lu次输入出错，终端被锁定",lAuthcount);
        ncsWriteSysLog(psMsgHead,caUsername,"01","失败","连续三次登录不成功，终端被锁定");
 //       ncsWriteSysLog(caUsername,caIp,"连续三次登录不成功，终端被锁定");
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/login2.htm");
        return 0;
    }
    utStrDelSpaces(caUsername);
//    utStrToLower(caUsername);
		if(strlen(caUsername)>0){
			
			sprintf(caTemp,"select id ,usrlevel from dsuser where name='%s' ",caUsername);
			pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lUserid,
			                        UT_TYPE_LONG,4,&lLevel);
	  }

    utStrDelSpaces(caPassword);
    
    

    
    
    
    iReturn = dsCltUserLogin(caUsername,caPassword,caIp);
    if(iReturn != 0) {
        if(iReturn == (-1)) {
            psDbHead = utPltInitDbHead();
            utPltPutVarF(psDbHead,"message", "用户不存在");
            
      //      ncsAdmWriteLog(caUsername,caIp,"第%d次用户不存在",iCount);
            ncsWriteSysLog(psMsgHead,caUsername,"01","失败","连续三次登录不成功，终端被锁定");
            utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/login2.htm");
        }
        else if(iReturn == (-2)) {
            psDbHead = utPltInitDbHead();
            utPltPutVarF(psDbHead,"message", "口令不正确");
            sprintf(caMsg,"第%d次口令不正确",iCount);
            ncsWriteSysLog(psMsgHead,caUsername,"01","失败",caMsg);
 //           ncsAdmWriteLog(caUsername,caIp,"第%d次口令不正确",iCount);
            utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/login2.htm");
        }
        else {
            psDbHead = utPltInitDbHead();
//            ncsAdmWriteLog(caUsername,caIp,"第%d次验证不正确",iCount);
            sprintf(caMsg,"第%d次验证不正确",iCount);
            ncsWriteSysLog(psMsgHead,caUsername,"01","失败",caMsg);
            utPltPutVarF(psDbHead,"message", "验证不正确");
            utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/login2.htm");
        }
        return 0;
    }
    else {
    	char caMbserno[36],caSql[512];
    	long lLogflag,lDef,lFlag;
    	
        psDbHead = utPltInitDbHead();
        ncsAdmSetAdmIp(psShmHead,lIp);
        
        if(lWidth>1900) lWidth=1900;
        if(lWidth<1024) lWidth=1024;
        utPltPutVarF(psDbHead,"rwidth","%d",lRight);
        utPltPutVarF(psDbHead,"swidth","%d",lWidth);
        utPltPutVarF(psDbHead,"rwidth1","%d",lRight1);
        
        
        lDef = atol(utComGetVar_sd(psShmHead,"MbLoginDef","0"));
				memset(caMbserno,0,sizeof(caMbserno));
				sprintf(caSql,"select mbserno,logflag,flag from ncuserbindmb where userid=%d ",lUserid);
				pasDbOneRecord(caSql,0,UT_TYPE_STRING,16,caMbserno,
				                       UT_TYPE_LONG,4,&lLogflag,
				                       UT_TYPE_LONG,4,&lFlag);
				
				if(strlen(caMbserno)>0){
					if(lFlag==0){
						   dsCltSetSessionValue(1,"mbflag",UT_TYPE_LONG,4,10);
						  }
					if(lLogflag==1){
						 utPltPutVar(psDbHead,"fname","ncMbLogin");
						 utPltPutVar(psDbHead,"mbserno",caMbserno);
						 
				
						   
					   utPltOutToHtml(iFd,psMsgHead,psDbHead,"ds/ds_mbauth.htm");
					   return 0;
					  }
				}
				else{
					
					
					if(lDef==1){
						utPltPutVarF(psDbHead,"message", "系统要求动态验证，但该用户没有绑定移动密宝");
				    utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/login2.htm");
						return 0;
					}
				}
        

        
        
        
        
        
        ncsWriteSysLog(psMsgHead,caUsername,"01","成功","成功登录到系统");
        
       if(strcasecmp(utComGetVar_sd(psShmHead,"ycflag","No"),"Yes")==0){  
        	utPltPutVar(psDbHead,"username",caUsername);	
   	    	utPltOutToHtml(iFd,psMsgHead,psDbHead,"ycncs/main.htm"); 
   	    	return 0; 	
        } 
        
        if(strcasecmp(caWindows_flag,"2")==0){
        	utPltPutVar(psDbHead,"username",caUsername);	
   	    	utPltOutToHtml(iFd,psMsgHead,psDbHead,"windows/main_desk.htm"); 
        	return 0;
        }
        
        
        
        if(strcasecmp(utComGetVar_sd(psShmHead,"wzflag","No"),"Yes")==0){  
        	utPltPutVar(psDbHead,"username",caUsername);	
   	    	utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/main_wz.htm");  	
        } 
        else{
//        	if(lLevel==2){
//        		 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/main_stat.htm");
//        	}
//        	else if(lLevel==4){
//        		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ycncs/main_zd.htm");
//        	}
//        	else if(lLevel==6){
//        		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ycncs/main_base.htm");           //宁波象山给市局看的页面
//        	}
//        	else{
 						if(strcasecmp(utComGetVar_sd(psShmHead,"wireless_flag","No"),"Yes")==0){  
 							utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/main_wireless.htm");
						}
						else{



        	  		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/main.htm");
        	  }
//          }
        }
    }
    return 0;
}





int ncsWebDispModiPass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iReturn;
    long lId;
    char caUsername[32];
    char caTemp[1024];
#ifdef LDEBUG
    utMsgPrintMsg(psMsgHead);
#endif   
iReturn = dsCltGetMyInfo(1,"Userid",&lId);
// iReturn = dsCltGetMyInfo(1,"Userid",&lId);
              
   	printf("iReturn=%d,lId=%d\n",iReturn,lId);
    if(iReturn < 0) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","口令修改",
                               "用户没有登录 ");
        return 0;
    }                               
    else {  
    	     sprintf(caTemp,"select  name from dsuser where id=%d ",lId); 
    	     pasDbOneRecord(caTemp,0,UT_TYPE_STRING,30,caUsername);       	                           
        utPltDbHead *psDbHead;
        psDbHead = utPltInitDbHead();
        utPltPutVar(psDbHead,"username", caUsername);
        utPltPutVarF(psDbHead,"userid",  "%lu",lId);
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/nc_modipass.htm");
        return 0;
   }
}

/* 口令修改  */
int ncsWebModiPass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{

   	char caUsername[33],caPassword1[33],caPassword2[33];
   	char caPass0[33],caOldpass[33],caUserid[33];
    utPltDbHead *psDbHead;
    char caKey[32],caKey0[64];
  	long lId;
   	int iReturn;
   	iReturn = dsCltGetMyInfo(1,"Userid",&lId);
   	                          
   	if(iReturn < 0) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","用户登录","用户没有登录 ");
        return 0;
    }
    
   	iReturn = utMsgGetSomeNVar(psMsgHead,3,
   	      "rname",UT_TYPE_STRING,30,caUsername,
					"password11",     UT_TYPE_STRING, 31, caPassword1,
					"password21",    UT_TYPE_STRING, 31, caPassword2);
    utStrDelSpaces(caPassword2);
    utStrDelSpaces(caPassword1);
   utStrDelSpaces(caUsername);
   sprintf(caKey0,"%s%s",caUsername,caPassword1);

   utMd5Ascii22(caKey0,strlen(caKey0),NULL,caKey);
   
    iReturn = pasDbExecSqlF("update dsuser set lKey='%s' where id=%d ",caKey,lId);
   
    if(iReturn != 0) {
        psDbHead = utPltInitDbHead();
        utPltPutVarF(psDbHead,"message", "口令修改出错, %d",iReturn);
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/ncmsg_back.htm");
        return 0;
    }
    else {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","密码修改",
                               "密码修改成功 ");
    }
    return 0;
}


/* 用户验证  */
int ncsWebAuth2(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
   	char caUsername[33],caPassword[33],caIp[16];
   	char caKey[64],caRight[65];
   	char caModi[24];
   	char caPass0[32];
   	char caCount[8];
   	unsigned long lIp;
   	unsigned long lTime;
   	long lLevel;
   	char caMsg[256],caAuthcount[8];
   	int iCount,lAuthcount;
    utPltDbHead *psDbHead;
   	int iReturn;
    char caTemp[1024];
    lTime = time(0);
  
   	iReturn = utMsgGetSomeNVar(psMsgHead,2,
					"username",  UT_TYPE_STRING, 31, caUsername,
					"password2",  UT_TYPE_STRING, 31, caPassword);
	  	

    utStrDelSpaces(caUsername);
    utStrToLower(caUsername);
    utStrDelSpaces(caPassword);
    
    memset(caKey,0,sizeof(caKey));
    sprintf(caTemp,"select usrlevel,key2 from dsuser where name='%s' ",caUsername);

    pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lLevel,UT_TYPE_STRING,24,caKey);
     psDbHead = utPltInitDbHead();
    if(strcmp(caPassword,caKey)==0){
        	if(lLevel==2){
        		 utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/main_stat.htm");
        	}
        	else if(lLevel==4){
        		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ycncs/main_zd.htm");
        	}
        	else if(lLevel==6){
        		utPltOutToHtml(iFd,psMsgHead,psDbHead,"ycncs/main_base.htm");           //宁波象山给市局看的页面
        	}
        	else{
        	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/main.htm");
          } 	 
    }
    else{
    	utPltPutVar(psDbHead,"username",caUsername);
    	utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/main_wz.htm");
    }    
    return 0;
}





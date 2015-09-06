/* 监控软件---有关Web的应用
     */
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
#define NC_LIMDATE_DAY    1      /* 独立日  */
#define NC_LIMDATE_WEEK   2      /* 每周    */
#define NC_LIMDATE_MONTH  3      /* 每月    */
#define NC_LIMDATE_YEAR   4      /* 每年    */
#define NC_LIMIT_NOEXIST   9
#define NC_GTYPE_USER       1 
#define NC_GTYPE_GROUP      2
#define NC_GTYPE_COMPANY    3           /* 公司       */
#define NC_GTYPE_CENTER     8           /* 中心       */
#define NC_GTYPE_SYSTEM     9           /* 系统       */
#define NC_GTYPE_LOCAL      0           /*本地*/
static char *ncsLimGetChar(char *pDate,int iStart,int iLen);
static char *ncsLimDateCvt(int iType,int iDate);

/* 显示日期列表  */
int ncsWebLimDateList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caMessage[128];
    char caId[16],caSear[16],caModi[16],caDel[16],caSid[16],caAdd[16];
    char caUdate[32],caLdate[32],caName[64],caName1[64],caApp[16],caType[16];
    char *p,*pBuf,*pSelect;
    char *pType;
    int i,iNum,iReturn;
    utPltDbHead *psDbHead;
   	long lStartRec,lSumUser,lCount,lCurPg,lTotRec,lRowNum,lSid1,lId1;
   	long lSid,lUdate,lLdate,lType,lType1,lUdate1,lLdate1;

 	pasDbCursor *psCur;
 	
	iReturn = utMsgGetSomeNVar(psMsgHead,10,
	                "group",      UT_TYPE_STRING, 12, caId,
	                "add",        UT_TYPE_STRING, 12, caAdd,
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "app",        UT_TYPE_STRING, 12, caApp,
	                "del",        UT_TYPE_STRING, 12, caDel,
	                "sid",        UT_TYPE_STRING, 12, caSid,
	                "type",       UT_TYPE_STRING, 12, caType,
	                "ldate",      UT_TYPE_STRING, 12, caLdate,
	                "udate",      UT_TYPE_STRING, 12, caUdate,
	                "name",       UT_TYPE_STRING, 31, caName);
    if(!utStrIsSpaces(caAdd)) {  /* 增加记录  */
        if(utStrIsSpaces(caId)) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0462日期的描述"),ncsLang("0523时间类别不能为空"));
            return 0;
        }            
        lLdate = ncsLimCheckValidDate(caType,caLdate,caMessage);
        if(lLdate < 0) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0462日期的描述"),
                             caMessage);
            return 0;
        }            
        if(utStrIsSpaces(caUdate)) {
            lUdate = lLdate;
        }
        else {
            lUdate = ncsLimCheckValidDate(caType,caUdate,caMessage);
            if(lUdate < 0) {
                 utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0462日期的描述"),
                             caMessage);
                 return 0;
            }
        }      
        iReturn = pasDbExecSqlF("insert into nclimdate(sid,id,ltype,llower,lupper) values (0,%s,%s,%lu,%lu)",
                                             caId,caType,lLdate,lUdate);
        if(iReturn != 0) {
            sprintf(caMessage,"insert into nclimdate(sid,id,ltype,llower,lupper) values (0,%s,%s,%lu,%lu)",
                                             caId,caType,lLdate,lUdate);
            utSysLog("Sql:%s",caMessage);                                             
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0068Ip地址列表"),ncsLang("0314记录增加出错, 可能为记录重复"));
            pasDbRollback(NULL);
            return 0;
        }        
        else {
            pasDbCommit(NULL);
        }
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];
   	    for(i=0;i<50;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
                iReturn = pasDbExecSqlF("delete from nclimdate where sid = %s",p);
                if(iReturn == 0) {
                    pasDbCommit(NULL);
                }
   	        }
   	    }
    }
    else if(!utStrIsSpaces(caModi)) { /* 修改记录  */
        if(utStrIsSpaces(caId)) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0462日期的描述"),ncsLang("0523时间类别不能为空"));
            return 0;
        }            
        lLdate = ncsLimCheckValidDate(caType,caLdate,caMessage);
        if(lLdate < 0) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0462日期的描述"),
                             caMessage);
            return 0;
        }            
        if(utStrIsSpaces(caUdate)) {
            lUdate = lLdate;
        }
        else {
            lUdate = ncsLimCheckValidDate(caType,caUdate,caMessage);
            if(lUdate < 0) {
                 utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0462日期的描述"),
                             caMessage);
                 return 0;
            }
        }      
        iReturn = pasDbExecSqlF("update nclimdate set id=%s,ltype=%s,llower=%lu,lupper=%lu where sid=%s",
                               caId,caType,lLdate,lUdate,caSid);
        if(iReturn != 0) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0068Ip地址列表"),ncsLang("0314记录增加出错, 可能为记录重复"));
            pasDbRollback(NULL);
            return 0;
        }        
        else {
            pasDbCommit(NULL);
        }
    }
  
    if(utStrIsSpaces(caId)) {
        psCur = pasDbOpenSqlF("select a.sid,a.id,a.ltype,a.llower,a.lupper,b.name  from nclimdate a,nclimdatedes b where a.id = b.id order by a.id");
    }
    else {
        psCur = pasDbOpenSqlF("select a.sid,a.id,a.ltype,a.llower,a.lupper,b.name  from nclimdate a,nclimdatedes b where a.id = b.id and a.id = %s order by a.id",caId);
    }
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0746用户查询"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lSid1,
      	                           UT_TYPE_LONG,4,&lId1,
  	                               UT_TYPE_ULONG,4,&lType1,
  	                               UT_TYPE_ULONG,4,&lLdate1,
  	                               UT_TYPE_ULONG,4,&lUdate1,
  	                               UT_TYPE_STRING,31,caName1);
   	iNum = 0;
   	while(iReturn == 0) {
        iNum++;
        switch(lType1) {
            case NC_LIMDATE_DAY:
                utPltPutLoopVar(psDbHead,"stype",iNum,ncsLang("0395某天"));
                break;
            case NC_LIMDATE_WEEK:
                utPltPutLoopVar(psDbHead,"stype",iNum,ncsLang("0386每周"));
                break;
            case NC_LIMDATE_MONTH:
                utPltPutLoopVar(psDbHead,"stype",iNum,ncsLang("0385每月"));
                break;
            case NC_LIMDATE_YEAR:
                utPltPutLoopVar(psDbHead,"stype",iNum,ncsLang("0381每年"));
                break;
        }
        utPltPutLoopVar(psDbHead,"sdate",  iNum,ncsLimDateCvt(lType1,lLdate1));
        utPltPutLoopVar(psDbHead,"edate",  iNum,ncsLimDateCvt(lType1,lUdate1));
        utPltPutLoopVarF(psDbHead,"lsid", iNum,"%lu",lSid1);
        utPltPutLoopVarF(psDbHead,"ltype", iNum,"%lu",lType1);
        utPltPutLoopVarF(psDbHead,"num",   iNum,"%lu",iNum);
        utPltPutLoopVarF(psDbHead,"lid", iNum,"%lu",lId1);
        utPltPutLoopVar(psDbHead,"lname",iNum,caName1);
        iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lSid1,
      	                           UT_TYPE_LONG,4,&lId1,
  	                               UT_TYPE_ULONG,4,&lType1,
  	                               UT_TYPE_ULONG,4,&lLdate1,
  	                               UT_TYPE_ULONG,4,&lUdate1,
  	                               UT_TYPE_STRING,31,caName1);
    }
    pasDbCloseCursor(psCur);
    i = 0;
    while(iNum < 30 + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }

    pBuf = pasUtlCvtSqlSelect("select name,id from nclimdatedes order by name");
    if(pBuf) {
        pSelect= pasUtlCvtInputSelect("group",caId,pBuf);
        free(pBuf);
    }
    else {
        pSelect= pasUtlCvtInputSelect("group"," ",ncsLang("0651无, ;"));
    }
    utPltPutVar(psDbHead,"group", pSelect);
    utPltPutVar(psDbHead,"name", caName);
    free(pSelect);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_limdatelist.htm");
    return 0;
}


int ncsWebLimDateGroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
   	long lId;
   	char caName[64];
   	char caPlate[128];
 	pasDbCursor *psCur;
 	
	iReturn = utMsgGetSomeNVar(psMsgHead,1,
	                "plate",  UT_TYPE_STRING,127,caPlate);
    psCur = pasDbOpenSqlF("select id,name from nclimdatedes");
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0746用户查询"),ncsLang("0524时间信息出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lId,
  	                               UT_TYPE_STRING,63,caName);
   	iNum = 0;
   	while(iReturn == 0) {
        iNum++;
        utPltPutLoopVarF(psDbHead,"num",   iNum,"%lu",iNum);
        utPltPutLoopVarF(psDbHead,"id",   iNum,"%u",lId);
        utPltPutLoopVarF(psDbHead,"name", iNum,"%s",caName);
        iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lId,
  	                               UT_TYPE_STRING,63,caName);
    }
    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead,"dateid","%lu",utStrGetId());
    utPltPutVar(psDbHead,"plate",caPlate);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    return 0;
}

int ncsWebLimDateGroupEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
   	long lId,i,lSid;
   	char caVar[32];
   	char *p;
   	unsigned long lGid;
   	char caName[64];
   	char caPlate[128];
   	char caDid[16],caAdd[16],caDel[16];
   	char caNname[32],caNid[16],caModi[16];
 	pasDbCursor *psCur;
 	
	iReturn = utMsgGetSomeNVar(psMsgHead,8,
	                "gname",  UT_TYPE_STRING,63,caName,
	                "sid",    UT_TYPE_STRING,12,caDid,
	                "add",    UT_TYPE_STRING,12,caAdd,
	                "ddd",    UT_TYPE_STRING,12,caDel,
	                "plate",  UT_TYPE_STRING,127,caPlate,
	                "nid",    UT_TYPE_STRING,12,caNid,
	                "modi",   UT_TYPE_STRING,12,caModi,
	                "nname",  UT_TYPE_STRING,31,caNname
	                );
    if(!utStrIsSpaces(caAdd)) { /* 增加一个组  */
        if(!utStrIsSpaces(caName)) {
            lGid = ncsUtlGenId(psShmHead);
            iReturn = pasDbExecSqlF("insert into nclimdatedes(id,name) values (%lu,'%s')",
                                             lGid,caName);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0316记录增加出错,可能为ID重复"));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                pasDbCommit(NULL);
            }                
        }
        else {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0363类别名称为空或ID出错"));
            return 0;
       }            
            
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除一个组  */
          	for(i=0;i<999;i++) {
   	        sprintf(caVar,"ch%d",i+1);

   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
                iReturn = pasDbExecSqlF("delete from nclimdatedes where id = %lu",lSid);
                if(iReturn==0)
                iReturn = pasDbExecSqlF("delete from nclimdate where id = %lu",lSid);
   	        }
             	        
   	    }   	  
    }
    else if(!utStrIsSpaces(caModi) && !utStrIsSpaces(caName)) { /* 修改一个组  */
        iReturn = pasDbExecSqlF("update nclimdatedes set name='%s' where id = %s",
                         caName,caDid);
        pasDbCommit(NULL);
    }
    return ncsWebLimDateGroup(psShmHead,iFd,psMsgHead);
    return 0;
}


int ncsLimCheckValidDate(char *pType,char *pDate,char *pMessage)
{
    long lDate;
    int iType;
    iType = atol(pType);
    if(iType == NC_LIMDATE_DAY) { /* 独立日  */
        int iYear,iMonth,iDay;
        iYear = atol(ncsLimGetChar(pDate,0,4));
        iMonth = atol(ncsLimGetChar(pDate,5,2));
        iDay = atol(ncsLimGetChar(pDate,8,2));
        lDate = utTimIsValidDate(iYear,iMonth,iDay);
        if(lDate < 0) {
            strcpy(pMessage,ncsLang("0463日期格式不对，正确的格式为: YYYY/MM/DD"));
            return (-1);
        }
        else {
            return lDate;
        }
    }
    else if(iType == NC_LIMDATE_WEEK) { 
        lDate = atol(pDate);
        if(lDate >= 0 && lDate <= 6) {
            return lDate;
        }
        else {
            strcpy(pMessage,ncsLang("0691星期的格式不对，正确的范围为: 0-6 (0表示星期日)"));
            return (-1);
        }
    }
    else if(iType == NC_LIMDATE_MONTH) { 
        lDate = atol(pDate);
        if(lDate >= 1 && lDate <= 31) {
            return lDate;
        }
        else {
            strcpy(pMessage,ncsLang("0826月份的格式不对，正确的范围为: 1-12"));
            return (-1);
        }
    }
    else if(iType == NC_LIMDATE_YEAR) {
        int iMonth,iDay;
        iMonth = atol(ncsLimGetChar(pDate,0,2));
        iDay = atol(ncsLimGetChar(pDate,3,2));
        if(iMonth > 0 && iMonth <= 12 && iDay > 0 && iDay <= 31) {
            lDate = iMonth * 100 + iDay;
            return lDate;
        }
        else {
            strcpy(pMessage,ncsLang("0461日期的格式，正确的格式为: YY/MM"));
            return (-1);
        }
    }
    else {
        strcpy(pMessage,ncsLang("0460日期的格式，可能是类型不正确"));
        return (-1);
    }
}

/* 取字符串  */
static char *ncsLimGetChar(char *pDate,int iStart,int iLen)
{
    static char caTemp[32];
    memcpy(caTemp,pDate+iStart,iLen);
    caTemp[iLen]=0;
    return caTemp;
}

static char *ncsLimDateCvt(int iType,int iDate)
{
    int iYear,iMonth,iDay;
    static char caTemp[32];
    strcpy(caTemp,"\0");
    switch(iType) {
        case NC_LIMDATE_DAY:
            iYear = iDate / 10000;
            iMonth = (iDate - iYear * 10000) / 100;
            iDay = iDate % 100;
            sprintf(caTemp,"%04d/%02d/%02d",iYear,iMonth,iDay);
            break;
        case NC_LIMDATE_WEEK:
            sprintf(caTemp,"%0d",iDate);
            break;
        case NC_LIMDATE_MONTH:
            sprintf(caTemp,"%02d",iDate);
            break;
        case NC_LIMDATE_YEAR:
            iMonth = iDate  / 100;
            iDay = iDate % 100;
            sprintf(caTemp,"%02d/%02d",iMonth,iDay);
            break;
        
    }
    return caTemp;
}

/* 显示Ip地址列表  */
int ncsWebLimIpList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[255],caTemp[255];
    char caCond[64];
    char caTotPg[16],caCurPg[16],caTotRec[16];
    char caId[16],caSear[16],caModi[16],caDel[16],caSid[16],caAdd[16];
    char caUip[32],caLip[32],caName[64],caApp[16],caProt[16];
    char caUip1[32],caLip1[32],caName1[64],caId1[16],caNamet[64];
    char caUport[32],caLport[32];
    char *p,*pBuf,*pSelect;
    int i,iNum,iReturn;
    utPltDbHead *psDbHead;
   	long lStartRec,lSumUser,lCount,lCurPg,lTotRec,lRowNum,lSid1,lId1,lLport1,lUport1;
   	unsigned long lSid,lTotPg,lLip,lUip,lLip1,lUip1,lProt,lProt1;
   	unsigned short nLport,nUport;
 	pasDbCursor *psCur;
 	
	iReturn = utMsgGetSomeNVar(psMsgHead,17,
	                "app",        UT_TYPE_STRING,12,caApp,
	                "cond",       UT_TYPE_STRING,63,caCond, 
	                "group",      UT_TYPE_STRING, 12, caId,
	                "sear",       UT_TYPE_STRING, 12, caSear,
	                "add",        UT_TYPE_STRING, 12, caAdd,
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,
	                "sid",        UT_TYPE_STRING, 12, caSid,
	                "lip",        UT_TYPE_STRING, 31, caLip,
	                "uip",        UT_TYPE_STRING, 31, caUip,
	                "lport",      UT_TYPE_STRING, 15, caLport,
	                "uport",      UT_TYPE_STRING, 15, caUport,
	                "prot",       UT_TYPE_STRING, 15, caProt,
	                "name",       UT_TYPE_STRING, 63, caName,
					"CurPg",      UT_TYPE_STRING, 12, caCurPg,
					"TotRec",     UT_TYPE_STRING, 12, caTotRec,
					"where",      UT_TYPE_STRING, 63, caWhere);
printf("caId=%s\n",caId);
    lCurPg = atol(caCurPg);
    if(lCurPg <= 0) lCurPg = 1;
    if(!utStrIsSpaces(caAdd)) {  /* 增加记录  */
        if(utStrIsSpaces(caLip) ||  utStrIsSpaces(caId)) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0068Ip地址列表"),ncsLang("0200地址或组为空"));
            return 0;
        }
        else {
            if(!utStrIsValidIp(caLip)) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0068Ip地址列表"),ncsLang("0059IP地址格式不对"));
                return 0;
            }
            if(utStrIsSpaces(caUip)) {
                strcpy(caUip,caLip);
            }
            if(!utStrIsValidIp(caUip)) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0068Ip地址列表"),ncsLang("0059IP地址格式不对"));
                return 0;
            }
            lLip = ntohl(utComHostAddress(caLip));
            lUip = ntohl(utComHostAddress(caUip));
            lProt = atol(caProt);
            if(lProt != 6) lProt = 17;
            nLport = (unsigned short)atol(caLport);
            nUport = (unsigned short)atol(caUport);
            lSid = ncsUtlGenId(psShmHead);
            iReturn = pasDbExecSqlF("insert into nciplist(sid,id,lip,uip,lport,uport,name,prot,flags) values (%lu,%s,%lu,%lu,%lu,%lu,'%s',%lu,0)",
                                             lSid,caId,lLip,lUip,nLport,nUport,caName,lProt);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0068Ip地址列表"),ncsLang("0311记录增加出错,%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                strcpy(caTotPg,"\0");
                pasDbCommit(NULL);
            }
        }    
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];
   	    for(i=0;i<50;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
                iReturn = pasDbExecSqlF("delete from nciplist where sid = %s",p);
                if(iReturn == 0) {
                    strcpy(caTotPg,"\0");
                    pasDbCommit(NULL);
                }
   	        }
   	    }
    }
    else if(!utStrIsSpaces(caModi)) { /* 修改记录  */
        if(!utStrIsSpaces(caSid)) {
            if(!utStrIsValidIp(caLip)) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0068Ip地址列表"),ncsLang("0059IP地址格式不对"));
                return 0;
            }
            if(utStrIsSpaces(caUip)) {
                strcpy(caLip,caUip);
            }
            if(!utStrIsValidIp(caUip)) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0068Ip地址列表"),ncsLang("0059IP地址格式不对"));
                return 0;
            }
            lLip = ntohl(utComHostAddress(caLip));
            lUip = ntohl(utComHostAddress(caUip));
            nLport = (unsigned short)atol(caLport);
            nUport = (unsigned short)atol(caUport);
            lProt = atol(caProt);
            if(lProt != 6) lProt = 17;
            iReturn = pasDbExecSqlF("update nciplist set id=%s,lip=%lu,uip=%lu,name='%s',lport=%lu,uport=%lu,prot=%lu where sid = %s",
                    caId,lLip,lUip,caName,nLport,nUport,lProt,caSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0303记录修改出错"));
                pasDbRollback(NULL);
            }
        }
    }
//    else if(!utStrIsSpaces(caApp)) { /* 应用  */
//        ncWebLimApp(psShmHead,iFd,psMsgHead);
//        return 0;
//    }
    if(!utStrIsSpaces(caSear) || utStrIsSpaces(caTotRec)) {
        strcpy(caWhere,"\0");
        if(!utStrIsSpaces(caId)) {
            utStrAddF(caWhere,"a.id = %lu",atol(caId));
        }
        if(!utStrIsSpaces(caCond)) {
            if(!utStrIsSpaces(caWhere)) {
                utStrAddF(caWhere," and ");
            }
            if(utStrIsValidIp(caCond)) {
                lLip = ntohl(utComHostAddress(caCond));
                utStrAddF(caWhere,"(a.lip >= %lu  or a.name like '%c%s%c')",lLip,'%',caCond,'%');
            }
            else {
                utStrAddF(caWhere,"(a.name like '%c%s%c')",'%',caCond,'%');
            }                
        }
        if(!utStrIsSpaces(caWhere)) {
            sprintf(caTemp,"select count(*) from nciplist a where %s",caWhere);
        }
        else {
            sprintf(caTemp,"select count(*) from nciplist");
        }
        iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
        if(iReturn != 0) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0561数据统计出错"));
            return 0;
        }
        lTotRec = lCount;
    }
    else {
        lTotRec = atol(caTotRec);
    }

    
    lRowNum = 30;
    lStartRec = (lCurPg - 1) * lRowNum;   
    if(!utStrIsSpaces(caWhere)) {
        psCur = pasDbOpenSqlF("select a.sid,a.id,a.lip,a.uip,a.lport,a.uport,a.prot,a.name,b.name from nciplist a left join ncipindex as b on a.id=b.id where %s order by a.id,a.lip,a.uip limit %d,%d",
               caWhere,lStartRec,lRowNum);
    }
    else {
        psCur = pasDbOpenSqlF("select a.sid,a.id,a.lip,a.uip,a.lport,a.uport,a.prot,a.name,b.name from nciplist a left join ncipindex as b on a.id=b.id order by a.id,a.lip,a.uip limit %d,%d",
                lStartRec,lRowNum);
    }        
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0746用户查询"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lSid1,
      	                           UT_TYPE_LONG,4,&lId1,
  	                               UT_TYPE_ULONG,4,&lLip1,
  	                               UT_TYPE_ULONG,4,&lUip1,
  	                               UT_TYPE_ULONG,4,&lLport1,
  	                               UT_TYPE_ULONG,4,&lUport1,
  	                               UT_TYPE_ULONG,4,&lProt1,
  	                               UT_TYPE_STRING,63,caName1,
  	                               UT_TYPE_STRING,63,caNamet);
   	iNum = 0;
   	while(iReturn == 0 && iNum < lRowNum) {
        iNum++;
        utPltPutLoopVarF(psDbHead,"num",   iNum,"%u",iNum);
        utPltPutLoopVarF(psDbHead,"lsid",   iNum,"%lu",lSid1);
        utPltPutLoopVarF(psDbHead,"lid",    iNum,"%lu", lId1);
        utPltPutLoopVar(psDbHead,"llip",    iNum, utComHostIp(htonl(lLip1)));
        utPltPutLoopVar(psDbHead,"luip",    iNum, utComHostIp(htonl(lUip1)));
        utPltPutLoopVarF(psDbHead,"llport",  iNum, "%lu",lLport1);
        utPltPutLoopVarF(psDbHead,"luport",  iNum, "%lu",lUport1);
        utPltPutLoopVar(psDbHead,"lname",   iNum,caName1);
        utPltPutLoopVarF(psDbHead,"lprot",  iNum, "%lu",lProt1);
        utPltPutLoopVar(psDbHead,"namet",iNum,caNamet);
        if(lProt1 == 6) {
            utPltPutLoopVarF(psDbHead,"protn",  iNum, "TCP");
        }
        else {
            utPltPutLoopVarF(psDbHead,"protn",  iNum, "UDP");
        }            
       iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lSid1,
      	                           UT_TYPE_LONG,4,&lId1,
  	                               UT_TYPE_ULONG,4,&lLip1,
  	                               UT_TYPE_ULONG,4,&lUip1,
  	                               UT_TYPE_ULONG,4,&lLport1,
  	                               UT_TYPE_ULONG,4,&lUport1,
  	                               UT_TYPE_ULONG,4,&lProt1,
  	                               UT_TYPE_STRING,63,caName1,
  	                               UT_TYPE_STRING,63,caNamet);
    }
    pasDbCloseCursor(psCur);
    lTotPg = (lTotRec - 1) / lRowNum + 1;
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
    utPltPutVar(psDbHead,"where",caWhere);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    utPltPutVar(psDbHead,"cond", caCond);
    utPltPutVar(psDbHead,"lip", "0.0.0.0");
    utPltPutVar(psDbHead,"uip", "0.0.0.0");
    utPltPutVar(psDbHead,"lport", "0");
    utPltPutVar(psDbHead,"uport", "65535");
    sprintf(caProt,"%lu",lProt);
    pBuf = pasUtlCvtSqlSelect("select name,id from ncipindex order by name");
    if(pBuf) {
        pSelect= pasUtlCvtInputSelect("group",caId,pBuf);
        free(pBuf);
    }
    else {
        pSelect= pasUtlCvtInputSelect("group"," ",ncsLang("0651无, ;"));
    }
    utPltPutVar(psDbHead,"group", pSelect);

    free(pSelect);
    pSelect = pasUtlCvtInputSelect("prot",caProt,"TCP,6;UDP,17;");
    if(pSelect) {
        utPltPutVar(psDbHead,"prot", pSelect);
        free(pSelect);
    }
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_limiplist.htm");
    return 0;
}


/* 显示Web地址组的列表  */
int ncsWebLimIpGroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
   	long lId;
   	char caName[64];
   	char caPlate[128];
 	pasDbCursor *psCur;
 	
	iReturn = utMsgGetSomeNVar(psMsgHead,1,
	                "plate",  UT_TYPE_STRING,127,caPlate);
 	
    psCur = pasDbOpenSqlF("select id,name from ncipindex");
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0746用户查询"),ncsLang("0087Web组信息出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lId,
  	                               UT_TYPE_STRING,63,caName);
   	iNum = 0;
   	while(iReturn == 0) {
        iNum++;
        utPltPutLoopVarF(psDbHead,"id",   iNum,"%u",lId);
        utPltPutLoopVarF(psDbHead,"name", iNum,"%s",caName);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%u",iNum);
        iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lId,
  	                               UT_TYPE_STRING,63,caName);
    }
    pasDbCloseCursor(psCur);
    utPltPutVar(psDbHead,"plate",caPlate);
    utPltPutVarF(psDbHead,"ipid","%lu",utStrGetId());
    utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    return 0;
}

int ncsWebLimIpGroupEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
   	long lId,lSid,i;
   	char temp_id[20];
   	char caName[64];
   	char caPlate[128];
   	char caGid[16],caDid[16],caAdd[16],caDel[16];
	char caNname[32],caNid[16],caModi[16],caVar[32];
 	char *p;
 	pasDbCursor *psCur;
 	
	iReturn = utMsgGetSomeNVar(psMsgHead,9,
	                "gname",  UT_TYPE_STRING,63,caName,
	                "gid",    UT_TYPE_STRING,12,caGid,
	                "did",    UT_TYPE_STRING,12,caDid,
	                "add",    UT_TYPE_STRING,12,caAdd,
	                "ddd",    UT_TYPE_STRING,12,caDel,
	                "plate",  UT_TYPE_STRING,127,caPlate,
	                "nid",    UT_TYPE_STRING,12,caNid,
	                "modi",   UT_TYPE_STRING,12,caModi,
	                "nname",  UT_TYPE_STRING,31,caNname
	                );
    if(!utStrIsSpaces(caAdd)) { /* 增加一个组  */
        if(!utStrIsSpaces(caName) && utStrIsDigital(caGid)) {
//            lSid = ncsUtlGenId(psShmHead);
           lSid=0;
            iReturn = pasDbOneRecord("select max(id) from ncipindex;",0,UT_TYPE_LONG,4,&lSid);
           
            	if(lSid<20000000)
                lSid = 20000000;
                else
                lSid=lSid+1;
           
            
            iReturn = pasDbExecSqlF("insert into ncipindex(id,name,flags) values (%lu,'%s',0)",
                                             lSid,caName);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0316记录增加出错,可能为ID重复"));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                pasDbCommit(NULL);
   /*
                utMsgDelVar(psMsgHead,"add");
                utMsgDelVar(psMsgHead,"group");
                sprintf(temp_id,"%lu",lSid);
                utMsgAdd_s(&psMsgHead,"group",temp_id);
                ncsWebLimIpList(psShmHead,iFd,psMsgHead);
                return 0;
   */
            }                
        }
        else {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0363类别名称为空或ID出错"));
            return 0;
       }            
            
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除一个组  */
    	  for(i=0;i<999;i++) {
   	        sprintf(caVar,"ch%d",i+1);

   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
	        iReturn = pasDbExecSqlF("delete from ncipindex where id= %lu",lSid);
		        if(iReturn != 0) {
	            pasDbRollback(NULL);
	        }                
	        else {
	            iReturn = pasDbExecSqlF("delete from nciplist where id= %lu",lSid);
	            pasDbCommit(NULL);
	        }            
   	        }
             	        
   	    }           
    }
    else if(!utStrIsSpaces(caModi) && !utStrIsSpaces(caName)) { /* 修改一个组  */
        iReturn = pasDbExecSqlF("update ncipindex set name='%s' where id = %s",
                         caName,caGid);
        pasDbCommit(NULL);
    }
    
    return ncsWebLimIpGroup(psShmHead,iFd,psMsgHead);
    return 0;
}



struct ncWebPol_s {
     long tid;
     int  flags;
     int  warnid;
};
struct ncWebPol_s* ncUtlLookWebPol(struct ncWebPol_s *ps,int iSum,long lTid)
{
    int bottom,middle,top;
    struct ncWebPol_s *pStr;
    if(ps == NULL) {
        return NULL;
    }

    top = iSum - 1;
    bottom = 0;
    
    while(top >= bottom) {
        middle = (top + bottom) / 2;
        if(lTid == ps[middle].tid) {
           pStr=malloc(sizeof(struct ncWebPol_s));
           pStr->tid=ps[middle].tid;
           pStr->flags=ps[middle].flags;
           pStr->warnid=ps[middle].warnid;
            return pStr;
        }
        else if(lTid < ps[middle].tid) {
            top = middle -1;
        }
        else {
            bottom = middle + 1;
        }
    }
    return NULL;
}
/* 服务访问策略  */

/* 将设置的字符串转换为访问策略
    lPid ---- 策略类别
    pStr ---- 设置的字符串，格式为: 
                 ctlax2=x;ctlb1x1244=y;ctlb1x1580=y;
 */
struct ncWebPol_s *ncUtlLoadIpPol(long lPid,int *iSum); 
struct ncWebPol_s *ncUtlLoadWebPol_l(long lPid,int *iSum);
int ncUtlIpPolCvtSave(int lPid,char *pStr)
{
    char *p;
    char caTemp[20];
    int i,iCount=0,iFlags=0,iReturn;
    p = pStr;
    iReturn = pasDbExecSqlF("delete from ncippol where pid =%lu",lPid);
    while(*p) {
        while(*p != 'x' && *p != 0) p++;
        if(*p == 'x') {
            p++;
            for(i=0;i<12&&(*p)!='='&&(*p)!=0;i++,p++) caTemp[i]=(*p);
            caTemp[i]=0;
            if(*p == '=') {
                p++;
                if(*p == 'y') {
                    iFlags = 1;
                }
                else {
                    iFlags = 0;
                }
                iReturn = pasDbExecSqlF("insert into ncippol(pid,tid,vflags) values (%lu,%s,%lu)",
                                             lPid,caTemp,iFlags);
                iCount++;
                iFlags = 0;
            }
            p++;
        }
        else {
            pasDbCommit(NULL);
            return iCount;
        }
    }
    pasDbCommit(NULL);
    return iCount;
}



int ncsWebLim_SetIpPol(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[2048];
    char caMsg[2048];
    char caCurPg[16],caTotRec[16];
    char caSid[16],caModi[16],caDel[16],caAdd[16];
    char caName[32],caApp[16],caPid[16],caTid[16],caVflags[16],caWarnid[16];
    char caNameW[32],caNameT[32];
    long lLocalwanr,lAdmcenter,lMesgnote,lMailnote,lMobile;
    int i,iNum,iReturn;
    long lSid,lCount,id,lVflags,lWarnid,lTid,lVister;
    char *p;
    utPltDbHead *psDbHead;
    long lStartRec,lCurPg,lTotRec,lRowNum,lTotPg;
    long lPort,lUport,lProt,lVfalgs;
    char caLip[16],caUip[16];
    pasDbCursor *psCur;
        struct data_s {
    	unsigned long id;
    	char name[32];
    	long lVister;
    	long lWarnid;
    	char namew[32];
    } s_data[40];
#ifdef LDEBUG 	
    utMsgPrintMsg(psMsgHead);
#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,10, 
	                "sid",     UT_TYPE_STRING,12,caSid,
	                "pid",     UT_TYPE_STRING,12,caPid,   
	                "name",   UT_TYPE_STRING, 31, caName,
	                "tid",UT_TYPE_STRING,15,caTid,
	                "vflags",UT_TYPE_STRING,2,caVflags,
	                "warnid",UT_TYPE_STRING,15,caWarnid,	               
	                "add",        UT_TYPE_STRING, 12, caAdd,	               
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,	               
			"CurPg",      UT_TYPE_STRING, 12, caCurPg);
			
    
    lRowNum = 32;
  
    	    
    lCurPg = atol(caCurPg);
  
  if(strlen(caVflags)==0)
  strcpy(caVflags,"0");
  if(strlen(caWarnid)==0)
  strcpy(caWarnid,"0");
  
  if(strlen(caSid)==0)
  strcpy(caSid,"0");
  
  
    if(lCurPg <= 0) lCurPg = 1;
    if(!utStrIsSpaces(caAdd)) {  /* 增加记录  */
       
            
 //           lSid = ncsUtlGenId(psShmHead);
            sprintf(caTemp,"select count(*) from ncippol where tid=%s and pid=%s ",caTid,caPid);   
	    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	    if(lCount> 0) {
	        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0261该类型已存在"),ncsLang("0261该类型已存在"));
	        return 0;
	    }
            iReturn = pasDbExecSqlF("insert into ncippol(pid,tid,vflags,warnid) values \
                     (%s,%s,%s,%s)", caPid,caTid,caVflags,caWarnid);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0904监控单位"),ncsLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                strcpy(caTotRec,"\0");
                pasDbCommit(NULL);
            }
       
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];

   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
   	            sprintf(caTemp,"delete from ncippol where tid=%lu",lSid);
             printf("caTemp=%s\n",caTemp);
   	           iReturn=pasDbExecSqlF("delete from ncippol where tid=%lu",lSid);

   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    }
    else if(!utStrIsSpaces(caModi)) { /* 修改记录  */

        if(!utStrIsSpaces(caSid)) {
           if(atol(caTid)!=atol(caSid))
           {
            sprintf(caTemp,"select count(*) from ncippol where tid=%s and pid=%s ",caTid,caPid);   
	    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	    if(lCount> 0) {
	        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0261该类型已存在"),ncsLang("0261该类型已存在"));
	        return 0;
	    }
           }
            iReturn = pasDbExecSqlF("update ncippol set vflags=%s,warnid=%s,tid=%s where tid = %s",
                    caVflags,caWarnid,caTid,caSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0305记录修改出错"),ncsLang("0305记录修改出错, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
        }
        strcpy(caTotRec,"\0");
    }
  
       
    sprintf(caTemp,"select count(*) from ncippol where pid=%s",caPid); 
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0561数据统计出错"));
        return 0;
    }
    lTotRec = lCount;
    lStartRec = (lCurPg - 1) * lRowNum;  

//printf("caTemp=%s\n",caTemp);
        psCur = pasDbOpenSqlF("select tid,vflags,warnid,b.name,c.name from ncippol a left join nclimwarntype b on a.warnid=b.id left join ncipindex c on a.tid=c.id where pid=%s order by b.name limit %d,%d",
                          caPid,lStartRec,lRowNum);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
   
    memset(caNameW,0,sizeof(caNameW));
    memset(caNameT,0,sizeof(caNameT));
    lVflags=0;
    lWarnid=0;
    lTid=0;
    memset(caNameW,0,sizeof(caNameW));
    memset(caNameT,0,sizeof(caNameT));
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&lTid,
      	                               UT_TYPE_LONG,4,&lVflags,
      	                               UT_TYPE_LONG,4,&lWarnid,
      	                               UT_TYPE_STRING,31,caNameW,
      	                               UT_TYPE_STRING,31,caNameT);
      	                              
  	                              
   	iNum = 0;
   	while(iReturn == 0 ||iReturn==1405 ) {
   	strcpy(s_data[iNum].name,caNameT);
   	strcpy(s_data[iNum].namew,caNameW);
   	s_data[iNum].id=lTid;
   	s_data[iNum].lVister=lVflags;
   	s_data[iNum].lWarnid=lWarnid;
        iNum++;

        
    memset(caNameW,0,sizeof(caNameW));
    memset(caNameT,0,sizeof(caNameT));
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&lTid,
      	                               UT_TYPE_LONG,4,&lVflags,
      	                               UT_TYPE_LONG,4,&lWarnid,
      	                               UT_TYPE_STRING,31,caNameW,
      	                               UT_TYPE_STRING,31,caNameT);
    }
    pasDbCloseCursor(psCur);
    
    for(i=0;i<iNum;i++)
    {
    	memset(caMsg,0,sizeof(caMsg));
    	sprintf(caTemp,"select inet_ntoa(lip),inet_ntoa(uip),lport,uport,prot from nciplist where id=%lu ",s_data[i].id);
    	psCur = pasDbOpenSql(caTemp,0);
    	if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
         }
    	memset(caLip,0,sizeof(caLip));
    	memset(caUip,0,sizeof(caUip));
     iReturn = pasDbFetchInto(psCur,   UT_TYPE_STRING,15,caLip,
     				       UT_TYPE_STRING,15,caUip,
    	   			       UT_TYPE_LONG,4,&lPort,
    	   			       UT_TYPE_LONG,4,&lUport,
    	   			       UT_TYPE_LONG,4,&lProt);
   while((iReturn==0)||(iReturn==1405))
   {
      	 if(strlen(caMsg)>2000) break;
      	 
      	 sprintf(caMsg+strlen(caMsg),"IP地址%s=>%s ",caLip,caUip);
      	 sprintf(caMsg+strlen(caMsg),"端口%lu=>%lu ",lPort,lUport);
      	 if(lProt==6)
      	 sprintf(caMsg+strlen(caMsg),"TCP ");
      	 if(lPort==17)
      	 sprintf(caMsg+strlen(caMsg),"UDP ");
      	 sprintf(caMsg+strlen(caMsg),";");
      	 memset(caLip,0,sizeof(caLip));
    	 memset(caUip,0,sizeof(caUip));
        iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,15,caLip,
     				       UT_TYPE_STRING,15,caUip,
    	   			       UT_TYPE_LONG,4,&lPort,
    	   			       UT_TYPE_LONG,4,&lUport,
    	   			       UT_TYPE_LONG,4,&lProt);
      	 
      	}  
      	pasDbCloseCursor(psCur); 
      	
      	utPltPutLoopVar(psDbHead,"msg",i+1,caMsg);	
        utPltPutLoopVarF(psDbHead,"tid",i+1,"%lu",s_data[i].id);
        utPltPutLoopVarF(psDbHead,"vflags",i+1,"%lu",s_data[i].lVister);
        utPltPutLoopVarF(psDbHead,"warnid",i+1,"%lu",s_data[i].lWarnid);
        utPltPutLoopVar(psDbHead,"namet",i+1,s_data[i].name);
      
        if(s_data[i].lVister==1)
        utPltPutLoopVar(psDbHead,"msgv",i+1,"不控制");
        else if(s_data[i].lVister==0)
        utPltPutLoopVar(psDbHead,"msgv",i+1,"禁止");
        else
        utPltPutLoopVar(psDbHead,"msgv",i+1,"未设置");
        
        if(s_data[i].lWarnid==0)
        utPltPutLoopVar(psDbHead,"msgw",i+1,"不告警");
        else
        utPltPutLoopVar(psDbHead,"msgw",i+1,s_data[i].namew);
        
        utPltPutLoopVarF(psDbHead,"num",i+1,"%lu",i+1);
    }
                       
    lTotPg = (lTotRec - 1) / lRowNum + 1;
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    utPltPutVar(psDbHead, "pid", caPid);
    
    sprintf(caTemp,"select a.name,a.vister,b.name from ncsippoltype a left join nclimwarntype b on a.warnid=b.id where pid=%s ",caPid);
    memset(caNameW,0,sizeof(caNameW));
    pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caName,
    			    UT_TYPE_LONG,4,&lVister,
    			    UT_TYPE_STRING,31,caNameW);
    			    
    utPltPutVar(psDbHead,"clname",caName);
    if(strlen(caNameW)!=0)			   
    utPltPutVar(psDbHead,"warname",caNameW);
    else
    utPltPutVar(psDbHead,"warname","不告警");
    if(lVister==1)
    utPltPutVar(psDbHead,"NV","不控制");
    else if(lVister==0)
    utPltPutVar(psDbHead,"NV","禁止");
    else
    utPltPutVar(psDbHead,"NV","不控制");
    
   
    
       /*取告警类别*/  
   psCur = pasDbOpenSql("select id,name from nclimwarntype  order by name ",0);
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caName);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVarF(psDbHead,"idw",   iNum,"%lu",id);
        utPltPutLoopVar(psDbHead,"namew",iNum,caName);
     iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,31,caName);
    }
    pasDbCloseCursor(psCur); 
    
  /*取网络服务类别*/  
   psCur = pasDbOpenSql("select sid,name from ncipindex  order by name ",0);
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,31,caName);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVarF(psDbHead,"ids",   iNum,"%lu",id);
        utPltPutLoopVar(psDbHead,"names",iNum,caName);
     iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,31,caName);
    }
    pasDbCloseCursor(psCur);
    
    
        //输出网络服务大类
long lCgid;

     	psCur = pasDbOpenSqlF("select groupid,name  from ncservicecgroup   order by groupid ");
                    
	    if(psCur == NULL) {
	        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0746用户查询"),ncsLang("0171查询数据出错"));
	        return 0;
	    }
     
    lCgid=0;
    memset(caName,0,sizeof(caName));
    iNum=0;
	    iReturn = pasDbFetchInto(psCur,     	                           
	      	                           UT_TYPE_LONG,4,&lCgid, 	                           
	  	                               UT_TYPE_STRING,31,caName);
	  	                             
   	
   	while(iReturn == 0 ||iReturn==1405) {
   		
    utPltPutLoopVarF(psDbHead,"cgid",iNum+1,"%d",lCgid);
    utPltPutLoopVar(psDbHead,"cgname",iNum+1,caName);
   	
   	iNum++;
   		
    lCgid=0;
    memset(caName,0,sizeof(caName));
	    iReturn = pasDbFetchInto(psCur,     	                           
	      	                           UT_TYPE_LONG,4,&lCgid, 	                           
	  	                               UT_TYPE_STRING,31,caName);
        }  
    pasDbCloseCursor(psCur); 
    
    
    
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_lim_ippol.htm");
    return 0;
}


struct ncWebPol_s *ncUtlLoadIpPol(long lPid,int *iSum)
{
    pasDbCursor *psCur;
    struct ncWebPol_s *ps;
    long lCount,lTid,lFlags;
    int iReturn,iNum;
    *iSum = 0;
    iReturn = pasDbOneRecord("select count(*) from ncippol where pid = :id",1,
                "id",UT_TYPE_LONG,lPid,
                UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0 || lCount == 0) {
        return NULL;
    }
    ps = malloc(lCount*sizeof( struct ncWebPol_s));
    if(ps == NULL) {
        return NULL;
    }
    psCur = pasDbOpenSqlF("select tid,vflags from ncippol where pid = %lu order by tid ",lPid);
    if(psCur == NULL) {
        free(ps);
        return NULL;
    }
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG, 4,&lTid,
                                   UT_TYPE_LONG, 4,&lFlags);
    iNum = 0;
    while(iReturn == 0) {
        ps[iNum].tid = lTid;
        ps[iNum].flags = lFlags;
        iNum++;
        iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG, 4,&lTid,
                                   UT_TYPE_LONG, 4,&lFlags);
    }
    pasDbCloseCursor(psCur);
    *iSum = iNum;
    return ps; 
}


/* 显示Web策略列表  */
int ncsWebLimIpPolList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
   	long lId,lVister;
   	char caName[64];
   	char caPlate[128];
 	pasDbCursor *psCur;
 	
	iReturn = utMsgGetSomeNVar(psMsgHead,1,
	                "plate",  UT_TYPE_STRING,127,caPlate);
 	
    psCur = pasDbOpenSqlF("select pid,name,vister from ncsippoltype");
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0746用户查询"),ncsLang("0087Web组信息出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lId,
  	                               UT_TYPE_STRING,63,caName,
  	                               UT_TYPE_LONG,4,&lVister);
   	iNum = 0;
   	while(iReturn == 0) {
        iNum++;
        utPltPutLoopVarF(psDbHead,"id",   iNum,"%u",lId);
        utPltPutLoopVarF(psDbHead,"name", iNum,"%s",caName);
        utPltPutLoopVarF(psDbHead,"vister",iNum,"%u",lVister);
        iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lId,
  	                               UT_TYPE_STRING,63,caName,
  	                               UT_TYPE_LONG,4,&lVister);
    }
    pasDbCloseCursor(psCur);
    utPltPutVar(psDbHead,"plate",caPlate);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    return 0;
}

int ncsWebLimIpPolEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    long temp_count=0;
    char caTemp[2048];
    char caMsg[2048];
    char caCurPg[16],caTotRec[16];
    char caSid[16],caModi[16],caDel[16],caAdd[16];
    char caName[32],caApp[16],caNameW[32];
    char caVister[16],caFlags[16],caWarnid[16];
    long lVister,lWarnid,lVflags;
    int i,iNum,iReturn;
    long lSid,lCount,id,lId;
    char *p;
    char caPlate[128];
    utPltDbHead *psDbHead;
    long lStartRec,lCurPg,lTotRec,lRowNum,lTotPg;
    pasDbCursor *psCur;
      struct data_s {
    	unsigned long id;
    	char name[32];
    	long lVister;
    	long lWarnid;
    	char namew[32];
    } s_data[40];
#ifdef LDEBUG 	
    utMsgPrintMsg(psMsgHead);
#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,9,    
	                "sid",    UT_TYPE_STRING, 12, caSid,
	                "name",   UT_TYPE_STRING, 31, caName,
	                "vister",UT_TYPE_STRING,2,caVister,
	                "warnid",UT_TYPE_STRING,12,caWarnid,	               
	                "add",        UT_TYPE_STRING, 12, caAdd,	               
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,	               
			            "CurPg",      UT_TYPE_STRING, 12, caCurPg,
			            "plate",      UT_TYPE_STRING, 127,caPlate);
			
    
    lRowNum = 32;
  
    lSid = atol(caSid);		    
    lCurPg = atol(caCurPg);
  
  if(strlen(caVister)==0)
  strcpy(caVister,"1");
  if(strlen(caWarnid)==0)
  strcpy(caWarnid,"0");
  
    if(lCurPg <= 0) lCurPg = 1;
    if(!utStrIsSpaces(caAdd)) {  /* 增加记录  */
       
       sprintf(caTemp,"select max(pid) from ncsippoltype ");
       pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&temp_count);
       
            sprintf(caTemp,"select count(*) from ncsippoltype where name='%s' ",caName);   
	    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	    if(lCount> 0) {
	        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0913类别名称不能重复"),ncsLang("0913类别名称不能重复"));
	        return 0;
	    }
	    if(temp_count<20000000)
	    temp_count=20000000;
	    else
	    temp_count=0;
           // lSid = ncsUtlGenId(psShmHead);
            iReturn = pasDbExecSqlF("insert into ncsippoltype(pid,name,vister,warnid,flags) values \
                     (%lu,'%s',%s,%s,0)",temp_count,caName,caVister,caWarnid);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0904监控单位"),ncsLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                strcpy(caTotRec,"\0");
                pasDbCommit(NULL);
            }
       
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];

   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
   	           iReturn=pasDbExecSqlF("delete from ncsippoltype where pid=%lu",lSid);
   	            
                iReturn = pasDbExecSqlF("delete from ncippol where tid = %lu",lSid);
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    }
    else if(!utStrIsSpaces(caModi)) { /* 修改记录  */

        if(!utStrIsSpaces(caSid)) {
            iReturn = pasDbExecSqlF("update ncsippoltype set name='%s',vister=%s,warnid=%s where pid = %s",
                    caName,caVister,caWarnid,caSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0305记录修改出错"),ncsLang("0305记录修改出错, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
        }
        strcpy(caTotRec,"\0");
    }
  
    if(strlen(caPlate)>0)
    {
    	    psCur = pasDbOpenSqlF("select pid,name,vister,warnid from ncsippoltype");
			    if(psCur == NULL) {
			        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0746用户查询"),ncsLang("0087Web组信息出错"));
			        return 0;
			    }
			    psDbHead = utPltInitDb();
			    iReturn = pasDbFetchInto(psCur,
			      	                           UT_TYPE_LONG,4,&lId,
			  	                               UT_TYPE_STRING,63,caName,
			  	                               UT_TYPE_LONG,4,&lVister,
			  	                               UT_TYPE_LONG,4,&lWarnid);
			   	iNum = 0;
			   	while(iReturn == 0) {
			        iNum++;
			        utPltPutLoopVarF(psDbHead,"id",   iNum,"%u",lId);
			        utPltPutLoopVarF(psDbHead,"name", iNum,"%s",caName);
			        utPltPutLoopVarF(psDbHead,"vister",iNum,"%u",lVister);
			        utPltPutLoopVarF(psDbHead,"warnid",iNum,"%lu",lWarnid);
			        utPltPutLoopVarF(psDbHead,"num",iNum,"%u",iNum);
			    iReturn = pasDbFetchInto(psCur,
			      	                           UT_TYPE_LONG,4,&lId,
			  	                               UT_TYPE_STRING,63,caName,
			  	                               UT_TYPE_LONG,4,&lVister,
			  	                               UT_TYPE_LONG,4,&lWarnid);
			    }
			    pasDbCloseCursor(psCur);
    }
  else
  {
    
       
		    sprintf(caTemp,"select count(*) from ncsippoltype");
		 
		    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
		    if(iReturn != 0) {
		        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0561数据统计出错"));
		        return 0;
		    }
		    lTotRec = lCount;
		    lStartRec = (lCurPg - 1) * lRowNum;  
		   
		        psCur = pasDbOpenSqlF("select pid,a.name,vister,warnid,b.name from ncsippoltype a left join nclimwarntype as b on a.warnid=b.id order by a.name limit %d,%d",
		                          lStartRec,lRowNum);
		   
		    if(psCur == NULL) {
		        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
		        return 0;
		    }
		    psDbHead = utPltInitDb();
		   
		    memset(caName,0,sizeof(caName));
		    memset(caNameW,0,sizeof(caNameW));
		    lVister=0;
		    lWarnid=0;
		    memset(caName,0,sizeof(caName));
		    memset(caNameW,0,sizeof(caNameW));
		    iReturn = pasDbFetchInto(psCur,
		      	                               UT_TYPE_LONG,4,&id,
		      	                               UT_TYPE_STRING,31,caName,
		      	                               UT_TYPE_LONG,4,&lVister,
		      	                               UT_TYPE_LONG,4,&lWarnid,
		      	                               UT_TYPE_STRING,31,caNameW);
		  	                              
		   	iNum = 0;
		   	while(iReturn == 0 ||iReturn==1405 ) {
		   	strcpy(s_data[iNum].name,caName);
		   	strcpy(s_data[iNum].namew,caNameW);
		   	s_data[iNum].id=id;
		   	s_data[iNum].lVister=lVister;
		   	s_data[iNum].lWarnid=lWarnid;
		   	
		   	
		        iNum++;
		    memset(caName,0,sizeof(caName));
		    memset(caNameW,0,sizeof(caNameW)); 
		    iReturn = pasDbFetchInto(psCur,
		      	                               UT_TYPE_LONG,4,&id,
		      	                               UT_TYPE_STRING,31,caName,
		      	                               UT_TYPE_LONG,4,&lVister,
		      	                               UT_TYPE_LONG,4,&lWarnid,
		      	                               UT_TYPE_STRING,31,caNameW);
		    }
		    pasDbCloseCursor(psCur);
		    
		    
		    for(i=0;i<iNum;i++)
		    {
		    	memset(caMsg,0,sizeof(caMsg));
		    	sprintf(caTemp,"select vflags,warnid,a.name,b.name from ncippol,ncipindex a left join nclimwarntype b on warnid=b.id where tid=a.id and pid=%lu ",s_data[i].id);
		    	psCur = pasDbOpenSql(caTemp,0);
		    	 if(psCur == NULL) {
		        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
		        return 0;
		         }
		           memset(caName,0,sizeof(caName));
		           memset(caNameW,0,sizeof(caNameW));
		        
		    	   iReturn = pasDbFetchInto(psCur,
		    	   			       UT_TYPE_LONG,4,&lVflags,
		    	   			       UT_TYPE_LONG,4,&lWarnid,
		      	                               UT_TYPE_STRING,31,caName,
		      	                               UT_TYPE_STRING,31,caNameW);
		      	                              
			    
			                          
			    while((iReturn==0)||(iReturn==1405))
			    {
			  	if(strlen(caMsg)>2000) break;
			        sprintf(caMsg+strlen(caMsg)," %s(",caName);
			        if(lVflags==1)
			        sprintf(caMsg+strlen(caMsg),"不控制,");
			        else if(lVflags==0)
			        sprintf(caMsg+strlen(caMsg),"禁止,");
			       
			        if(lWarnid==0)
			        sprintf(caMsg+strlen(caMsg),"不告警)");
			        else
			        sprintf(caMsg+strlen(caMsg),"%s)",caNameW);
			        
			        memset(caName,0,sizeof(caName));
		                memset(caNameW,0,sizeof(caNameW));
		   	   iReturn = pasDbFetchInto(psCur,
		    	   			       UT_TYPE_LONG,4,&lVflags,
		    	   			       UT_TYPE_LONG,4,&lWarnid,
		      	                               UT_TYPE_STRING,31,caName,
		      	                               UT_TYPE_STRING,31,caNameW);
			    }
			    pasDbCloseCursor(psCur); 
		    	
		    	
		    	
		    	utPltPutLoopVarF(psDbHead,"id",i+1,"%lu",s_data[i].id);
		   	utPltPutLoopVar(psDbHead,"name",i+1,s_data[i].name);
		   	utPltPutLoopVarF(psDbHead,"vister",i+1,"%lu",s_data[i].lVister);
		   	utPltPutLoopVarF(psDbHead,"warnid",i+1,"%lu",s_data[i].lWarnid);
		   	utPltPutLoopVarF(psDbHead,"num",i+1,"%lu",i+1);
		   	
		   	if(s_data[i].lWarnid==0)
		   	utPltPutLoopVar(psDbHead,"msgw",i+1,"不告警");
		   	else if(strlen(s_data[i].namew)!=0)
		   	utPltPutLoopVar(psDbHead,"msgw",i+1,s_data[i].namew);
		   	
		   	if(s_data[i].lVister==0)
		   	utPltPutLoopVar(psDbHead,"msgv",i+1,"禁止");
		   	else if(s_data[i].lVister==1)
		   	utPltPutLoopVar(psDbHead,"msgv",i+1,"允许");
		   	else 
		   	utPltPutLoopVar(psDbHead,"msgv",i+1,"未设置");
		   	
		   	utPltPutLoopVar(psDbHead,"msg",i+1,caMsg);
		   	
		    }
		
		    lTotPg = (lTotRec - 1) / lRowNum + 1;
		    i = 0;
		    while(iNum < lRowNum + 1) {
		        i++;
		        iNum++;
		        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
		    }
		    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
		    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
		    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);


}



    psCur = pasDbOpenSql("select id,name from nclimwarntype  order by name limit 0,999",0);
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caName);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVarF(psDbHead,"idw",   iNum,"%lu",id);
        utPltPutLoopVar(psDbHead,"namew",iNum,caName);
     iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caName);
    }
    pasDbCloseCursor(psCur); 
       
      if(strlen(caPlate)>0)
      utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
      else
      utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_limippolgroup.htm");
 
    return 0;

}



/* 网址策略设置
       显示和处理一级网址类别
 */
int ncsWebLim_SetWebPol1(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iNum2,iReturn,iSum;
    utPltDbHead *psDbHead;
   	long lId,lId0,lId2,lSid,lId1,lPid,lFlags,lVister;
   	char caName[64];
   	char caPname[64],caVister[16];
   	char caGif[10],caPid[16],caSave[16];
   	struct ncWebPol_s *ps;
   	char *pSelect;
 	pasDbCursor *psCur;
    utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,4,
	                "save", UT_TYPE_STRING,15,caSave,
	                "pid",  UT_TYPE_STRING,15,caPid,
	                "name",UT_TYPE_STRING,63,caPname,
	                "vister",UT_TYPE_STRING,15,caVister);
    lPid = atol(caPid);
    
    if(!utStrIsSpaces(caSave)) {
        char *pStr;
        iReturn = pasDbExecSqlF("update ncwebpoltype set name='%s',vister=%lu where pid = %lu",
                         caPname,atol(caVister),lPid);
        pStr = utMsgGetVar_s(psMsgHead,"rtl");
        if(pStr) {
            ncUtlWebPolCvtSave_l(lPid,pStr);
        }
        if(iReturn == 0) {
            pasDbCommit(NULL);
        }                         
    }
    else {
        iReturn = pasDbOneRecord("select name,vister from ncwebpoltype where pid = :id",1,
                "id",UT_TYPE_LONG,lPid,
                UT_TYPE_STRING,63,caPname,
                UT_TYPE_LONG,4,&lVister);
        if(iReturn != 0) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0562数据统计出错%s"),pasDbErrorMsg(NULL));
            return 0;
        }
        sprintf(caVister,"%d",lVister);
    }
    ps = ncUtlLoadWebPol_l(lPid,&iSum);

    /* 读一级目录  */
    psCur = pasDbOpenSqlF("select sid,id1,name from ncwebtype where id2 = 0 order by id1");
    if(psCur == NULL) {
        if(ps) free(ps);
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0562数据统计出错%s"),pasDbErrorMsg(NULL));
        return 0;
    }
    psDbHead = utPltInitDb();
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG, 4,&lSid,
                                   UT_TYPE_LONG, 4,&lId1,
  	                               UT_TYPE_STRING,63,caName);
    iNum = 0;
    lId = 0;
   	while(iReturn == 0) {
        lFlags = ncUtlLookWebPol(ps,iSum,lSid);
        if(lFlags == NC_LIMIT_NOEXIST) {
            strcpy(caGif,"no");
        }
        else if(lFlags == 1) {
            strcpy(caGif,"y1");
        }
        else {
            strcpy(caGif,"x1");
        }
        iNum++;
        utPltPutLoopVarF(psDbHead,"numa",   iNum,"%u",iNum);
        utPltPutLoopVarF(psDbHead,"sida",   iNum,"%u",lSid);
        utPltPutLoopVarF(psDbHead,"pname",  iNum,"%s",caPname);
        utPltPutLoopVarF(psDbHead,"id",    iNum,"%u",lPid);
        utPltPutLoopVarF(psDbHead,"ida",    iNum,"%u",lId1);
        utPltPutLoopVarF(psDbHead,"namea",  iNum,"%s",caName);
        utPltPutLoopVarF(psDbHead,"gifa",   iNum,"%s",caGif);
        iReturn = pasDbFetchInto(psCur,
                                   UT_TYPE_LONG,4,&lSid,
      	                           UT_TYPE_LONG,4,&lId1,
  	                               UT_TYPE_STRING,63,caName);
    }
    pasDbCloseCursor(psCur);
    if(ps) free(ps);
    utPltPutVarF(psDbHead,"pid",  "%lu",lPid);
    pSelect = pasUtlCvtInputSelect("vister",caVister,ncsLang("0653无,9;允许,1;禁止,0;"));
    if(pSelect) {
        utPltPutVar(psDbHead,"vister", pSelect);
        free(pSelect);
    }
    utPltPutVar(psDbHead,"name", caPname);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_lim_webpol_l.htm");
    return 0;
}




/* 保存设置的信息  */
int ncUtlWebPolCvtSave_l(int lPid,char *pStr)
{
    char *p;
    char caTemp[20];
    char caTempw[20];
    int i,iCount=0,iFlags=0,iReturn;
    p = pStr;
    iReturn = pasDbExecSqlF("delete from ncwebpollist where pid =%lu",lPid);
    while(*p) {
    	while(*p != 'd' && *p != 0) p++;
    	if(*p=='d'){
    		p++;
    		for(i=0;i<12&&(*p)!='c'&&(*p)!=0;i++,p++) caTempw[i]=(*p);
    		caTempw[i]=0;
    	}
        while(*p != 'x' && *p != 0) p++;
        if(*p == 'x') {
            p++;
            for(i=0;i<12&&(*p)!='='&&(*p)!=0;i++,p++) caTemp[i]=(*p);
            caTemp[i]=0;
            if(*p == '=') {
                p++;
                if(*p == 'y') {
                    iFlags = 1;
                }
                else {
                    iFlags = 0;
                }
                iReturn = pasDbExecSqlF("insert into ncwebpollist(pid,tid,vflags,warnid) values (%lu,%s,%lu,%s)",
                                             lPid,caTemp,iFlags,caTempw);
                iCount++;
                iFlags = 0;
            }
            else {
                break;
            }
            p++;
        }
        else {
            pasDbCommit(NULL);
            return iCount;
        }
    }
    pasDbCommit(NULL);
    return iCount;
}

struct ncWebPol_s *ncUtlLoadWebPol_l(long lPid,int *iSum)
{
    pasDbCursor *psCur;
    struct ncWebPol_s *ps;
    long lCount,lTid,lFlags,lWarnid;
    int iReturn,iNum;
    *iSum = 0;
    iReturn = pasDbOneRecord("select count(*) from ncwebpollist where pid = :id ",1,
                "id",UT_TYPE_LONG,lPid,
                UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0 || lCount == 0) {
        return NULL;
    }
    ps = malloc(lCount*sizeof( struct ncWebPol_s));
    if(ps == NULL) {
        return NULL;
    }
    psCur = pasDbOpenSqlF("select tid,vflags,warnid from ncwebpollist where pid = %lu order by tid ",lPid);
    if(psCur == NULL) {
        free(ps);
        return NULL;
    }
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG, 4,&lTid,
                                   UT_TYPE_LONG, 4,&lFlags,
                                   UT_TYPE_LONG,4,&lWarnid);
    iNum = 0;
    while(iReturn == 0) {
        ps[iNum].tid = lTid;
        ps[iNum].flags = lFlags;
        ps[iNum].warnid=lWarnid;
        iNum++;
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG, 4,&lTid,
                                   UT_TYPE_LONG, 4,&lFlags,
                                   UT_TYPE_LONG,4,&lWarnid);
    }
    pasDbCloseCursor(psCur);
    *iSum = iNum;
    return ps; 
}


/* 显示Web地址库类别  */
int ncsWebLimWebClass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[255];
    char caCurPg[16],caTotRec[16];
    char caSid[16],caModi[16],caDel[16],caAdd[16],caFlags[16],caStatus[16];
    char caName[32],caApp[16],caName1[32];
    char caPlate[64];
    char caModlb[16],caRowNum[16];
    char *pSelect,*p;
    int i,iNum,iReturn,iDelcount;
    long lSid,lSid1,lFlags,lFlags1,lCount,lCount1,lStatus,lStatus1,lId;
    utPltDbHead *psDbHead;
   	long lStartRec,lCurPg,lTotRec,lRowNum,lTotPg;
 	pasDbCursor *psCur;
#ifdef LDEBUG 	
    utMsgPrintMsg(psMsgHead);
#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,12,
	                "plate",      UT_TYPE_STRING, 63, caPlate,
	                "flags",      UT_TYPE_STRING, 12, caFlags,
	                "sid",        UT_TYPE_STRING, 12, caSid,
	                "name",       UT_TYPE_STRING, 31, caName, 
	                "status",     UT_TYPE_STRING, 12, caStatus,
	                "add",        UT_TYPE_STRING, 12, caAdd,
	                "modlb",      UT_TYPE_STRING, 12, caModlb,
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,
	                "app",        UT_TYPE_STRING, 12, caApp, 
					"CurPg",      UT_TYPE_STRING, 12, caCurPg,
					"rownum",     UT_TYPE_STRING, 12, caRowNum);
    if(utStrIsSpaces(caRowNum)) {
        lRowNum = 24;
    }
    else {
        lRowNum = atol(caRowNum);
    }
    lSid = atol(caSid);		
    lFlags = atol(caFlags);
    lCurPg = atol(caCurPg);
    lStatus = atol(caStatus);
    if(lCurPg <= 0) lCurPg = 1;
    if(!utStrIsSpaces(caAdd)) {  /* 增加记录  */
        if(utStrIsSpaces(caName)) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0360类别不能为空"));
            return 0;
        }
        else {
//            lSid = ncsUtlGenId(psShmHead);
            lSid=0;
            iReturn = pasDbOneRecord("select max(id) from ncwebclass;",0,UT_TYPE_LONG,4,&lSid);
           
            	if(lSid<20000000)
                lSid = 20000000;
                else
                lSid=lSid+1;
            iReturn = pasDbExecSqlF("insert into ncwebclass(id,name,lcount,modtime,status,flags) values \
                     (%lu,'%s',%lu,%lu,%d,%d)", lSid,caName,0,time(0),lStatus,NC_GTYPE_CENTER);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                strcpy(caTotRec,"\0");
                pasDbCommit(NULL);
            }
        }
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];
        iDelcount=0;
   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
   	            iReturn = pasDbExecSqlF("delete from ncwebclass where id = %lu",lSid);
   	      //      iReturn = pasDbExecSqlF("update ncwebclass set flags=99,modtime=%lu  where id = %lu",time(0),lSid);
//                iReturn = pasDbExecSqlF("delete from ncwebclass where id = %lu",lSid);
                if(iReturn==0)
                  iReturn = pasDbExecSqlF("delete from ncweblist where id = %lu",lSid);
           //     iReturn = pasDbExecSqlF("update ncweblist set flags=99,modtime=%lu  where id = %lu",time(0),lSid);
//                iReturn = pasDbExecSqlF("delete from ncweblist where id = %lu",lSid);
                
                iDelcount++;
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    }
    else if(!utStrIsSpaces(caModlb)) {
        char caVar[16];
   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
     	        lSid = atol(p);
                iReturn = pasDbExecSqlF("update ncwebclass set status = %lu,modtime=%lu where id = %lu",
                                    lStatus,time(0),lSid);
   	        }
   	    }
        pasDbCommit(NULL);
    }
    else if(!utStrIsSpaces(caModi)) { /* 修改记录  */
       
        if(!utStrIsSpaces(caSid)) {
            iReturn = pasDbExecSqlF("update ncwebclass set name='%s',status=%d,modtime=%lu where id = %lu",
                    caName,lStatus,time(0),lSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0305记录修改出错, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
        }
        strcpy(caTotRec,"\0");
    }
    else if(!utStrIsSpaces(caApp)) { /* 应用  */
        unsigned long lIndexTime=0,lLstTime=0;
        iReturn = pasDbOneRecord("select max(modtime) from ncweblist",0,
                            UT_TYPE_ULONG,  4, &lLstTime);
        iReturn = pasDbOneRecord("select max(modtime) from ncwebclass",0,
                            UT_TYPE_ULONG,  4, &lIndexTime);
        if(iReturn == 0) {
            ncsUpdateSetUrlTime(psShmHead,lIndexTime,lLstTime);
        }
    }
    
   
        sprintf(caTemp,"select count(*) from ncwebclass where flags!=99 ");
        if(atol(caFlags)>0)
        sprintf(caTemp+strlen(caTemp), " and flags=%s  ",caFlags);
        
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0561数据统计出错"));
        return 0;
    }
    lTotRec = lCount;
    lStartRec = (lCurPg - 1) * lRowNum;  
    printf("select id,name,lcount,status,flags from ncwebclass where flags=%s order by flags,id limit %d,%d\n",
                          caFlags,lStartRec,lRowNum);
                          
                          
   if(atol(caFlags)>0)
        psCur = pasDbOpenSqlF("select id,name,lcount,status,flags from ncwebclass where flags=%s order by flags,id limit %d,%d",
                          caFlags,lStartRec,lRowNum);
   else
        psCur = pasDbOpenSqlF("select id,name,lcount,status,flags from ncwebclass where flags!=99 order by flags,id limit %d,%d",
                          lStartRec,lRowNum);
    
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0746用户查询"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lSid1,
  	                               UT_TYPE_STRING,31,caName1,
  	                               UT_TYPE_LONG,4,&lCount1,
  	                               UT_TYPE_LONG,4,&lStatus1,
  	                               UT_TYPE_LONG,4,&lFlags1);
   	iNum = 0;
   	while(iReturn == 0 && iNum < lRowNum) {
        iNum++;
        
        utPltPutLoopVarF(psDbHead,"num",   iNum,"%u",iNum);
        utPltPutLoopVarF(psDbHead,"lid",   iNum,"%lu",lSid1);
        utPltPutLoopVar(psDbHead, "lname",  iNum,caName1);
        utPltPutLoopVarF(psDbHead,"lcount",  iNum,"%lu",lCount1);
        utPltPutLoopVarF(psDbHead,"lstatus",  iNum,"%lu",lStatus1);
        utPltPutLoopVarF(psDbHead,"lflags",  iNum,"%lu",lFlags1);
        utPltPutLoopVar(psDbHead, "lsname",  iNum,ncsUtlGtypeName(lFlags1,"\0"));
        if(lStatus1 == 0) {
            utPltPutLoopVar(psDbHead,"lusename",  iNum,ncsLang("0568停用"));
        }
        else {
            utPltPutLoopVar(psDbHead,"lusename",  iNum,ncsLang("0437启用"));
        }
        iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lSid1,
  	                               UT_TYPE_STRING,31,caName1,
  	                               UT_TYPE_LONG,4,&lCount1,
  	                               UT_TYPE_LONG,4,&lStatus1,
  	                               UT_TYPE_LONG,4,&lFlags1);
    }
    pasDbCloseCursor(psCur);
    lTotPg = (lTotRec - 1) / lRowNum + 1;
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    utPltPutVar(psDbHead, "sid", caSid);
    utPltPutVar(psDbHead, "flags", caFlags);
    if(lFlags > 0) {
        utPltPutVar(psDbHead, "sname",  ncsUtlGtypeName(lFlags,"\0"));
    }
    pSelect= pasUtlCvtInputSelect("status",caStatus,ncsLang("0569停用,0;启用,1;"));
    if(pSelect) {
        utPltPutVar(psDbHead,"status", pSelect);
        free(pSelect);
    }
    if(utStrIsSpaces(caPlate)) {
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_limwebclass.htm");
    }
    else {
        utPltPutVar(psDbHead,"plate", caPlate);
        utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    }
    return 0;
}


/* 显示Web地址列表  */
int ncsWebLimWebList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[255],caTemp[255];
    char caCurPg[16],caTotRec[16];
    char caId[16],caSear[16],caModi[16],caDel[16],caSid[16],caAdd[16],caFlags[12];
    char caUrl[64],caName[64],caTname[64],caApp[16];
    char caUrl1[64],caName1[64],caTname1[64],caId1[16],caSid1[16],caTid[16];
    char caModlb[32],caPlate[64];
    char *p,*pBuf,*pSelect;
    int i,iNum,iReturn;
    int iDelcount;
    long lId,lId1,lFlags,lSid,lSid1,lFlags1,lCount,lTid;
    utPltDbHead *psDbHead;
   	long lStartRec,lCurPg,lTotRec,lRowNum,lTotPg;
 	pasDbCursor *psCur;
#ifdef LDEBUG 	
    utMsgPrintMsg(psMsgHead);
#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,15,
	                "plate",      UT_TYPE_STRING, 63, caPlate,
	                "flags",      UT_TYPE_STRING, 12, caFlags,
	                "id",         UT_TYPE_STRING, 12, caId, 
	                "sid",        UT_TYPE_STRING, 12, caSid,
	                "url",        UT_TYPE_STRING, 63, caUrl,
	                "name",       UT_TYPE_STRING, 63, caName,
	                "sear",       UT_TYPE_STRING, 12, caSear,
	                "add",        UT_TYPE_STRING, 12, caAdd,
	                "modlb",      UT_TYPE_STRING, 12, caModlb,
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,
	                "app",        UT_TYPE_STRING, 12, caApp, 
					"CurPg",      UT_TYPE_STRING, 12, caCurPg,
					"TotRec",     UT_TYPE_STRING, 12, caTotRec,
					"where",      UT_TYPE_STRING, 127,caWhere);
    lRowNum = 25;
    lId = atol(caId);		
    lFlags = atol(caFlags);
    lSid = atol(caSid);
    lCurPg = atol(caCurPg);
    lTotRec = atol(caTotRec);
    if(lCurPg <= 0) lCurPg = 1;

    if(!utStrIsSpaces(caAdd)) {  /* 增加记录  */
       
        if(utStrIsSpaces(caUrl) ||  utStrIsSpaces(caId) ) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0200地址或组为空"));
            return 0;
        }
        else {
            iReturn = pasDbExecSqlF("insert into ncweblist(sid,id,url,name,modtime,flags) values (0,%lu,'%s','%s',%lu,%d)",
                                          lId,caUrl,caName,time(0),8 );
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                strcpy(caTotRec,"\0");
                pasDbCommit(NULL);
            }
        }
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];
       iDelcount=0;
   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
  // 	             iReturn = pasDbExecSqlF("update ncweblist set flags=99,modtime=%lu where sid = %lu",time(0),lSid);
             iReturn = pasDbExecSqlF("delete from ncweblist where sid = %lu",lSid);
                  iDelcount++;
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    }
    else if(!utStrIsSpaces(caModlb)) {
        char caVar[16];
  
        if(utStrIsSpaces(caId)) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0360类别不能为空"));
            return 0;        
        }
        iDelcount=0;
   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
     	        lSid = atol(p);
                iReturn = pasDbExecSqlF("update ncweblist set id = %lu,modtime=%lu where sid = %lu",lId,time(0),lSid);
                 strcpy(caTotRec,"\0");
                 iDelcount++;
   	        }
   	    }
        pasDbCommit(NULL);
    }
    else if(!utStrIsSpaces(caModi)) { /* 修改记录  */
        if(!utStrIsSpaces(caSid)) {
            iReturn = pasDbExecSqlF("update ncweblist set id=%lu,url='%s',name='%s',modtime=%lu where sid = %lu",
                    lId,caUrl,caName,time(0),lSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0305记录修改出错, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
        }
        strcpy(caTotRec,"\0");
    }
    
    if(!utStrIsSpaces(caSear)) {
        strcpy(caWhere,"\0");
        strcpy(caTotRec,"\0");
        if(!utStrIsSpaces(caId)) {
            utStrAddF(caWhere,"and a.id = %lu ",lId);
        }
        
        if(!utStrIsSpaces(caUrl)) {
            utStrAddF(caWhere,"and a.url like '%c%s%c' ",
                                  '%',caUrl,'%');
        }
        if(!utStrIsSpaces(caName)) {
            utStrAddF(caWhere,"and a.name like '%c%s%c' ",
                                  '%',caName,'%');
        }
    }

        if(utStrIsSpaces(caWhere)) {
            sprintf(caTemp,"select count(*) from ncweblist a where a.id = %lu and a.flags=8",lId);
            sprintf(caWhere,"and a.id = %lu ",lId);
        }
        else {
            sprintf(caTemp,"select count(*) from ncweblist a where %s and a.flags!=99",caWhere+4);
        }
        iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
        if(iReturn != 0) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0561数据统计出错"));
            return 0;
        }
        lTotRec = lCount;

    lStartRec = (lCurPg - 1) * lRowNum;   
    if(!utStrIsSpaces(caWhere)) {
    	
//    	printf("select a.sid,a.id,a.url,a.name,b.name from ncweblist a,ncwebclass b where a.id = b.id and a.flags!=99 %s order by a.flags,a.url limit %d,%d\n",
//               caWhere,lStartRec,lRowNum);
        psCur = pasDbOpenSqlF("select a.sid,a.id,a.url,a.name,b.name from ncweblist a,ncwebclass b where a.id = b.id and a.flags=8 %s order by a.id,a.url limit %d,%d",
               caWhere,lStartRec,lRowNum);
    }
    else {
        sprintf(caWhere,"and a.id = %lu ",lId);
        psCur = pasDbOpenSqlF("select a.sid,a.id,a.url,a.name,b.name from ncweblist a,ncwebclass b where a.id = b.id and a.id = %lu and a.flags=8 order by a.id,a.url limit %d,%d",
                lId,lStartRec,lRowNum);
    }        
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0746用户查询"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lSid1,
      	                           UT_TYPE_LONG,4,&lId1,
  	                               UT_TYPE_STRING,63,caUrl1,
  	                               UT_TYPE_STRING,63,caName1,
  	                               UT_TYPE_STRING,63,caTname1);
   	iNum = 0;
   	while(iReturn == 0 && iNum < lRowNum) {
        iNum++;
        utPltPutLoopVarF(psDbHead,"num",   iNum,"%u",iNum);
        utPltPutLoopVarF(psDbHead,"lsid",   iNum,"%lu",lSid1);
        utPltPutLoopVarF(psDbHead,"lid",    iNum,"%lu", lId1);
        utPltPutLoopVarF(psDbHead,"lflags", iNum,"%lu", lFlags);
        utPltPutLoopVar(psDbHead, "lurl",    iNum,caUrl1);
        utPltPutLoopVar(psDbHead,"lname",   iNum,caName1);
        utPltPutLoopVar(psDbHead,"ltname",   iNum,caTname1);
        iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lSid1,
      	                           UT_TYPE_LONG,4,&lId1,
  	                               UT_TYPE_STRING,63,caUrl1,
  	                               UT_TYPE_STRING,63,caName1,
  	                               UT_TYPE_STRING,63,caTname1);
    }
    pasDbCloseCursor(psCur);
    lTotPg = (lTotRec - 1) / lRowNum + 1;
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
    utPltPutVar(psDbHead,"where",caWhere);
    utPltPutVarF(psDbHead,"lid", "%lu", lId);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    utPltPutVar(psDbHead,"flags", caFlags);
    utPltPutVar(psDbHead,"name", caName);
    
        sprintf(caTemp,"select name,id from ncwebclass order by id");
   
    pBuf = pasUtlCvtSqlSelectA(caTemp,ncsLang("0630未知, 0;"));
    if(pBuf) {
        pSelect= pasUtlCvtInputSelect("id",caId,pBuf);
        if(pSelect) {
            utPltPutVar(psDbHead,"id", pSelect);
            free(pSelect);
        }
        free(pBuf);
    }
    if(utStrIsSpaces(caPlate)) {
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_limweblist.htm");
    }
    else {
        utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    }    
    return 0;
}

/* 显示Web地址库策略类别  */
int ncsWebWebPolClass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[255];
    char caCurPg[16],caTotRec[16];
    char caSid[16],caModi[16],caDel[16],caAdd[16],caFlags[16],caStatus[16];
    char caName[32],caApp[16],caName1[32],caNamew[32];
    char caPlate[64];
    char caWarnid[16];
    long id,lWarnid;
    char caVister[16];
    char caModlb[16],caRowNum[16];
    char *pSelect,*p;
    int i,iNum,iReturn;
    long lSid,lSid1,lFlags,lFlags1,lCount,lCount1,lStatus,lStatus1,lId;
    utPltDbHead *psDbHead;
   	long lStartRec,lCurPg,lTotRec,lRowNum,lTotPg,lVister,lVister1;
 	pasDbCursor *psCur;
#ifdef LDEBUG 	
    utMsgPrintMsg(psMsgHead);
#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,12,
	                "plate",      UT_TYPE_STRING, 63, caPlate,
	                "flags",      UT_TYPE_STRING, 12, caFlags,
	                "sid",        UT_TYPE_STRING, 12, caSid,
	                "name",       UT_TYPE_STRING, 31, caName, 
	                "vister",     UT_TYPE_STRING, 12, caVister,
	                "warnid",     UT_TYPE_STRING,12,caWarnid,
	                "add",        UT_TYPE_STRING, 12, caAdd,
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,
	                "app",        UT_TYPE_STRING, 12, caApp, 
					"CurPg",      UT_TYPE_STRING, 12, caCurPg,
					"rownum",     UT_TYPE_STRING, 12, caRowNum);
    if(utStrIsSpaces(caRowNum)) {
        lRowNum = 24;
    }
    else {
        lRowNum = atol(caRowNum);
    }
    if(strlen(caWarnid)==0)
    strcpy(caWarnid,"0");
    
    lSid = atol(caSid);		
    lFlags = atol(caFlags);
    lCurPg = atol(caCurPg);
    lVister = atol(caVister);
    if(lCurPg <= 0) lCurPg = 1;
    if(!utStrIsSpaces(caAdd)) {  /* 增加记录  */
        if(utStrIsSpaces(caName)) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0360类别不能为空"));
            return 0;
        }
        else {
            sprintf(caTemp,"select max(pid) from ncwebpolclass ");
            pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lSid);
            if(lSid<20000000)
            lSid=20000000;
            else
            lSid=0;
            iReturn = pasDbExecSqlF("insert into ncwebpolclass(pid,name,vister,flags,warnid) values \
                     (%lu,'%s',%d,%d,%s)", lSid,caName,lVister,NC_GTYPE_CENTER,caWarnid);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                strcpy(caTotRec,"\0");
                pasDbCommit(NULL);
            }
        }
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];
   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
                iReturn = pasDbExecSqlF("delete from ncwebpolclass where pid = %lu",lSid);
                iReturn = pasDbExecSqlF("delete from ncwebpolistl  where pid = %lu",lSid);
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    }
    else if(!utStrIsSpaces(caModi)) { /* 修改记录  */
        if(!utStrIsSpaces(caSid)) {
            iReturn = pasDbExecSqlF("update ncwebpolclass set name='%s',vister=%d,warnid=%s  where pid = %lu",
                    caName,lVister,caWarnid,lSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0305记录修改出错, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
        }
        strcpy(caTotRec,"\0");
    }
//    else if(!utStrIsSpaces(caApp)) { /* 应用  */
        /* 重新修改网址库计数  */
/*	    if(ncUtlIsLock(psShmHead,NC_LOCK_SYSREFRESH,5)==0) {
            ncUtlReReadConFig(psShmHead);
            ncInitProeim(psShmHead,1);
            ncUtlSendProcessControl(psShmHead,"ALL",NC_RELOAD_ALL,1);
            ncUtlUnLock(psShmHead,NC_LOCK_SYSREFRESH);
        }
        else {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0672系统正忙，请稍候再试"));
            return 0;
        } 
    }
*/

        sprintf(caTemp,"select count(*) from ncwebpolclass");
  
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0561数据统计出错"));
        return 0;
    }
    lTotRec = lCount;
    lStartRec = (lCurPg - 1) * lRowNum;  
 
        psCur = pasDbOpenSqlF("select pid,a.name,vister,a.flags,warnid,b.name from ncwebpolclass a left join nclimwarntype b on a.warnid=b.id order by a.flags,pid limit %d,%d",
                          lStartRec,lRowNum);
 
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0746用户查询"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lSid1,
  	                               UT_TYPE_STRING,31,caName1,
  	                               UT_TYPE_LONG,4,&lVister1,
  	                               UT_TYPE_LONG,4,&lFlags1,
  	                               UT_TYPE_LONG,4,&lWarnid,
  	                               UT_TYPE_STRING,31,caNamew);
   	iNum = 0;
   	while((iReturn == 0||iReturn==1405) && iNum < lRowNum) {
        iNum++;
        utPltPutLoopVarF(psDbHead,"num",   iNum,"%u",iNum);
        utPltPutLoopVarF(psDbHead,"lid",   iNum,"%lu",lSid1);
        utPltPutLoopVar(psDbHead, "lname",  iNum,caName1);
        utPltPutLoopVarF(psDbHead,"lvister",  iNum,"%lu",lVister1);
        utPltPutLoopVarF(psDbHead,"lwarnid",iNum,"%lu",lWarnid);
        utPltPutLoopVarF(psDbHead,"lflags",  iNum,"%lu",lFlags1);
        utPltPutLoopVar(psDbHead, "lsname",  iNum,ncsUtlGtypeName(lFlags1,"\0"));
        if(lVister1 == 0) {
            utPltPutLoopVar(psDbHead,"lvname",  iNum,ncsLang("0332禁止"));
        }
        else if(lVister1 == 1) {
            utPltPutLoopVar(psDbHead,"lvname",  iNum,ncsLang("0827允许"));
        }
        else {
            utPltPutLoopVar(psDbHead,"lvname",  iNum,ncsLang("0627未设置"));
        }
        if(lWarnid==0)
        utPltPutLoopVar(psDbHead,"wname",iNum,"不告警");
        else
        utPltPutLoopVar(psDbHead,"wname",iNum,caNamew);
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lSid1,
  	                               UT_TYPE_STRING,31,caName1,
  	                               UT_TYPE_LONG,4,&lVister1,
  	                               UT_TYPE_LONG,4,&lFlags1,
  	                               UT_TYPE_LONG,4,&lWarnid,
  	                               UT_TYPE_STRING,31,caNamew);
    }
    pasDbCloseCursor(psCur);
    lTotPg = (lTotRec - 1) / lRowNum + 1;
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    utPltPutVar(psDbHead, "sid", caSid);
    utPltPutVar(psDbHead, "flags", caFlags);
    if(lFlags > 0) {
        utPltPutVar(psDbHead, "sname",  ncsUtlGtypeName(lFlags,"\0"));
    }
    pSelect= pasUtlCvtInputSelect("vister",caVister,ncsLang("0333禁止,0;允许,1;未设置,9;"));
    if(pSelect) {
        utPltPutVar(psDbHead,"vister", pSelect);
        free(pSelect);
    }
    
    psCur = pasDbOpenSql("select id,name from nclimwarntype  order by name limit 0,999",0);
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,31,caName);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVarF(psDbHead,"idw",   iNum,"%lu",id);
        utPltPutLoopVar(psDbHead,"namew",iNum,caName);
     iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caName);
    }
    pasDbCloseCursor(psCur);
    
    if(utStrIsSpaces(caPlate)) {
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_webpolclass.htm");
    }
    else {
        utPltPutVar(psDbHead,"plate", caPlate);
        utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    }
    return 0;
}

//显示网址库策略明细
int ncsWebLim_DispWebPo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
   	char caName[64],caPid[64],caVister[64];
   	int iReturn;
    utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,3,
	                "name", UT_TYPE_STRING,63,caName,
	                "pid",  UT_TYPE_STRING,15,caPid,
	                "vister",UT_TYPE_STRING,15,caVister);
    psDbHead = utPltInitDb();
    utPltPutVar(psDbHead,"pid",caPid);
    utPltPutVar(psDbHead,"name",caName);
    utPltPutVar(psDbHead,"vister",caVister);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_lim_setwebpol.htm");
    return 0;
}

int ncsWebWebPolList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iNum2,iReturn,iSum;
    utPltDbHead *psDbHead;
   	long lId,lId0,lId2,lSid,lId1,lPid,lFlags,lVister,lFlags1,lWarnid,lWarnid2;
   	
   	char caPname[64],caVister[16],caWarnid[16];
   	char caGif[10],caPid[16],caSave[16];
   	struct ncWebPol_s *pLook;
   	struct ncWebPol_s *ps;
   	char *pSelect;
    struct data_s {
    	unsigned long id;
    	char name[32];
    } s_data[1000];
    unsigned long id,m,lWum,lWarnid3;
    char caName[64];
 	pasDbCursor *psCur;
    utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,5,
	                "save", UT_TYPE_STRING,15,caSave,
	                "pid",  UT_TYPE_STRING,15,caPid,
	                "name",UT_TYPE_STRING,63,caPname,
	                "vister",UT_TYPE_STRING,15,caVister,
	                "warnid",UT_TYPE_STRING,15,caWarnid);
    lPid = atol(caPid);
   
    if(!utStrIsSpaces(caSave)) {
    	
        char *pStr;
        lWarnid=atol(caWarnid);
        iReturn = pasDbExecSqlF("update ncwebpolclass set name='%s',vister=%lu,warnid=%lu where pid = %lu",
                         caPname,atol(caVister),atol(caWarnid),lPid);
        pStr = utMsgGetVar_s(psMsgHead,"rtl");
        if(pStr) {
            ncUtlWebPolCvtSave_l(lPid,pStr);
            printf("gggggggg\n");
        }
    }
    else {
        iReturn = pasDbOneRecord("select name,vister,warnid from ncwebpolclass where pid = :id",1,
                "id",UT_TYPE_LONG,lPid,
                UT_TYPE_STRING,63,caPname,
                UT_TYPE_LONG,4,&lVister,
                UT_TYPE_LONG,4,&lWarnid);
        if(iReturn != 0) {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0562数据统计出错%s"),pasDbErrorMsg(NULL));
            return 0;
        }
        sprintf(caVister,"%d",lVister);
    }
#ifdef LDEBUG
printf(" After Save ...\n");
#endif
    ps = ncUtlLoadWebPol_l(lPid,&iSum);

 psDbHead = utPltInitDb();
 utPltPutVarF(psDbHead,"warnid","%lu",lWarnid);
   psCur = pasDbOpenSql("select id,name from nclimwarntype  order by name ",0);
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,31,caName);
    iNum = 0;
    lWum=0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
    	if(iNum>999) break;
  	s_data[iNum].id=id;
  	strcpy(s_data[iNum].name,caName);
        iNum++; 
        lWum++;      
        utPltPutLoopVarF(psDbHead,"idw",   iNum,"%lu",id);
        utPltPutLoopVar(psDbHead,"namew",iNum,caName);
     iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,31,caName);
    }
    pasDbCloseCursor(psCur); 

    /* 读一级目录  */
    psCur = pasDbOpenSqlF("select id,name,flags from ncwebclass where status = 1 order by flags,id");
    if(psCur == NULL) {
        if(ps) free(ps);
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0562数据统计出错%s"),pasDbErrorMsg(NULL));
        return 0;
    }
   
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG, 4,&lSid,
  	                               UT_TYPE_STRING,63,caName,
  	                               UT_TYPE_LONG,4,&lFlags1);
  	                              
    iNum = 0;
    lId = 0;
   	while(iReturn == 0) {
        pLook = ncUtlLookWebPol(ps,iSum,lSid);
        if(pLook==NULL)
        {
        strcpy(caGif,"no");
        utPltPutLoopVar(psDbHead,"tip",iNum,"未设置");
        }
        else
        {
	        if(pLook->flags == NC_LIMIT_NOEXIST) {	        	
	            utPltPutLoopVar(psDbHead,"tip",iNum+1,"未设置");
	            strcpy(caGif,"no");
	        }
	        else if(pLook->flags == 1) {
	            strcpy(caGif,"y1");
	            utPltPutLoopVar(psDbHead,"tip",iNum+1,"允许");
	        }
	        else {
	            strcpy(caGif,"x1");
	            utPltPutLoopVar(psDbHead,"tip",iNum+1,"禁止");
	        }
	  }
	 
        iNum++;
        
        for(m=0;m<lWum;m++)
        {
            utPltPutLoopVarF2(psDbHead,"idw",iNum,m+1,"%lu",s_data[m].id);
            utPltPutLoopVar2(psDbHead,"namew",iNum,m+1,s_data[m].name);
        }
        utPltPutLoopVarF(psDbHead,"num",    iNum,"%u",iNum);
        utPltPutLoopVarF(psDbHead,"sida",    iNum,"%u",lSid);
        utPltPutLoopVarF(psDbHead,"pname",  iNum,"%s",caPname);
        utPltPutLoopVarF(psDbHead,"ida",    iNum,"%u",lPid);
        utPltPutLoopVarF(psDbHead,"namea",  iNum,"%s",caName);
        utPltPutLoopVarF(psDbHead,"gifa",   iNum,"%s",caGif);
        utPltPutLoopVarF(psDbHead,"warnid",iNum,"%lu",lWarnid);
        

        if(pLook)
        {
        //	printf("sff=%d\n",pLook->warnid);
        utPltPutLoopVarF(psDbHead,"lwarnid",iNum,"%lu",pLook->warnid);
         }
         else
         utPltPutLoopVar(psDbHead,"lwarnid",iNum,"0");
         if(pLook) free(pLook);
 //       utPltPutLoopVarF(psDbHead,"lwarnid",iNum,"%lu",lWarnid3);
        if(lFlags1 > 0) {
            utPltPutVar(psDbHead, "sname",  ncsUtlGtypeName(lFlags1,"\0"));
        }        
       iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG, 4,&lSid,
  	                               UT_TYPE_STRING,63,caName,
  	                               UT_TYPE_LONG,4,&lFlags1);
  	                              
    }
    pasDbCloseCursor(psCur);
    if(ps) free(ps);
    utPltPutVarF(psDbHead,"pid",  "%lu",lPid);
    pSelect = pasUtlCvtInputSelect("vister",caVister,ncsLang("0653无,9;允许,1;禁止,0;"));
    if(pSelect) {
        utPltPutVar(psDbHead,"vister", pSelect);
        free(pSelect);
    }
    utPltPutVar(psDbHead,"name", caPname);

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_lim_webpol_1.htm");
    return 0;
}

/* 显示邮件地址类别列表  */
int mailclasslist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[2048];
    char caCurPg[16],caTotRec[16];
    char caSid[16],caModi[16],caDel[16],caAdd[16];
    char caName[32],caApp[16],caDesc[128];
    char caModtime[32],caStatus[16];
    unsigned long lModtime,lStatus,lFlags;
    char caPlate[128];
    char caMods[32];
    int i,iNum,iReturn;
    long lSid,lCount,id;
    char *p;
    utPltDbHead *psDbHead;
    long lStartRec,lCurPg,lTotRec,lRowNum,lTotPg;
    pasDbCursor *psCur;

#ifdef LDEBUG 	
    utMsgPrintMsg(psMsgHead);
#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,9,    
	                "sid",    UT_TYPE_STRING, 12, caSid,
	                "name",   UT_TYPE_STRING, 31, caName,
	                "status",UT_TYPE_STRING,2,caStatus,               
	                "add",        UT_TYPE_STRING, 12, caAdd,	               
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,	               
			            "CurPg",      UT_TYPE_STRING, 12, caCurPg,
			            "plate",      UT_TYPE_STRING,127,caPlate,
			            "mods",       UT_TYPE_STRING,12,caMods);
			
    
    lRowNum = 24;
  
    lSid = atol(caSid);		    
    lCurPg = atol(caCurPg);
  
  if(strlen(caStatus)==0)
  strcpy(caStatus,"0");

  
    if(lCurPg <= 0) lCurPg = 1;
    if(!utStrIsSpaces(caAdd)) {  /* 增加记录  */
       
            sprintf(caTemp,"select count(*) from ncmailclass where name='%s' ",caName);   
	    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	    if(lCount> 0) {
	        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0913类别名称不能重复"),ncsLang("0913类别名称不能重复"));
	        return 0;
	    }
         //   lSid = ncsUtlGenId(psShmHead);
             lSid=0;
            iReturn = pasDbOneRecord("select max(id) from ncmailclass;",0,UT_TYPE_LONG,4,&lSid);
           
            	if(lSid<20000000)
                lSid = 20000000;
                else
                lSid=lSid+1;
            lModtime=time(0);
            iReturn = pasDbExecSqlF("insert into ncmailclass(id,name,status,modtime,flags) values \
                     (%lu,'%s',%s,%lu,8)", lSid,caName,caStatus,lModtime);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0904监控单位"),ncsLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                strcpy(caTotRec,"\0");
                pasDbCommit(NULL);
            }
       
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];

   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
   	           iReturn=pasDbExecSqlF("delete from ncmailclass where id=%lu",lSid);
   	            
                iReturn = pasDbExecSqlF("delete from ncmailaddr where id = %lu",lSid);
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    }
     else if(!utStrIsSpaces(caMods)) { /* 修改状态  */
        char caVar[16];

   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
   	           iReturn=pasDbExecSqlF("update ncmailclass set status=%s where id=%lu",caStatus,lSid);
   	           
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    } 
    else if(!utStrIsSpaces(caModi)) { /* 修改记录  */

        if(!utStrIsSpaces(caSid)) {
           lModtime=time(0);

            iReturn = pasDbExecSqlF("update ncmailclass set name='%s',status=%s,modtime=%lu where id = %s",
                    caName,caStatus,lModtime,caSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0305记录修改出错"),ncsLang("0305记录修改出错, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
        }
        strcpy(caTotRec,"\0");
    }
  
       
       sprintf(caTemp,"select count(*) from ncmailclass");
 
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0561数据统计出错"));
        return 0;
    }
    lTotRec = lCount;
    lStartRec = (lCurPg - 1) * lRowNum;  
   
        psCur = pasDbOpenSqlF("select id,name,modtime,status,flags from ncmailclass order by id desc limit %d,%d",
                          lStartRec,lRowNum);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
   
    memset(caName,0,sizeof(caName));
    lModtime=0;
    lStatus=0;
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,31,caName,
      	                               UT_TYPE_LONG,4,&lModtime,
      	                               UT_TYPE_LONG,4,&lStatus,
      	                               UT_TYPE_LONG,4,&lFlags);
  	                              
   	iNum = 0;
   	while(iReturn == 0 ||iReturn==1405 ) {
   
        iNum++;
        utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",id);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        utPltPutLoopVar(psDbHead,"name",iNum,caName);
        utPltPutLoopVarF(psDbHead,"lstatus",iNum,"%lu",lStatus);
        utPltPutLoopVarF(psDbHead,"lflags",iNum,"%lu",lFlags);
        utPltPutLoopVar(psDbHead,"modtime",iNum,utTimFormat("%Y/%m/%d %H:%M",lModtime));
        if(lFlags>0)
        utPltPutLoopVar(psDbHead,"sname",iNum,ncsUtlGtypeName(lFlags,"\0"));
        
        if(lStatus==1)
        utPltPutLoopVar(psDbHead,"status",iNum,"启用");
        else
        utPltPutLoopVar(psDbHead,"status",iNum,"停用");
        
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,31,caName,
      	                               UT_TYPE_LONG,4,&lModtime,
      	                               UT_TYPE_LONG,4,&lStatus,
      	                               UT_TYPE_LONG,4,&lFlags);
    }
    pasDbCloseCursor(psCur);
    

    lTotPg = (lTotRec - 1) / lRowNum + 1;
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
      if(strlen(caPlate)>0)
      utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_mailclasslisttree.htm");
      else
      utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_mailclasslist.htm");
 
    return 0;
}

/* 显示邮件地址列表  */
int mailaddrlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[2048];
    char caCurPg[16],caTotRec[16];
    char caSid[16],caModi[16],caDel[16],caAdd[16];
    char caName[32],caApp[16],caDesc[128];
    char caModtime[32],caStatus[16],caId[16],caMail[64],caNamec[64],caSerch[16];
    unsigned long lModtime,lStatus,lFlags;
    int i,iNum,iReturn;
    long lSid,lCount,id;
    char *p;
    utPltDbHead *psDbHead;
    long lStartRec,lCurPg,lTotRec,lRowNum,lTotPg;
    pasDbCursor *psCur;

#ifdef LDEBUG 	
    utMsgPrintMsg(psMsgHead);
#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,10,    
	                "sid",    UT_TYPE_STRING, 12, caSid,
	                "id",     UT_TYPE_STRING, 12,caId,
	                "mail",   UT_TYPE_STRING, 63,caMail,
	                "name",   UT_TYPE_STRING, 31, caName,
	                "status",UT_TYPE_STRING,2,caStatus,               
	                "add",        UT_TYPE_STRING, 12, caAdd,	               
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,	               
			"CurPg",      UT_TYPE_STRING, 12, caCurPg,
			"serch",      UT_TYPE_STRING, 12,caSerch);
			
 
    lRowNum = 22;
  
    lSid = atol(caSid);		    
    lCurPg = atol(caCurPg);
  
  if(strlen(caStatus)==0)
  strcpy(caStatus,"0");
  
  
    if(lCurPg <= 0) lCurPg = 1;
    if(!utStrIsSpaces(caAdd)) {  /* 增加记录  */
       
            sprintf(caTemp,"select count(*) from ncmailaddr where id=%s and name='%s' ",caId,caName);   
	    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	    if(lCount> 0) {
	        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0913类别名称不能重复"),ncsLang("0913类别名称不能重复"));
	        return 0;
	    }
            
            lModtime=time(0);
            iReturn = pasDbExecSqlF("insert into ncmailaddr(sid,id,mail,name,modtime,flags) values \
                     (0,%s,'%s','%s',%lu,8)", caId,caMail,caName,lModtime);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0904监控单位"),ncsLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                strcpy(caTotRec,"\0");
                pasDbCommit(NULL);
            }
       
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];

   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
//   	    printf(" delete from ncmailaddr where sid = %lu",lSid);       
                iReturn = pasDbExecSqlF("delete from ncmailaddr where sid = %lu",lSid);
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    }
    else if(!utStrIsSpaces(caModi)) { /* 修改记录  */

        if(!utStrIsSpaces(caSid)) {
           lModtime=time(0);

            iReturn = pasDbExecSqlF("update ncmailaddr set id=%s,mail='%s',name='%s',modtime=%lu where sid = %s",
                    caId,caMail,caName,lModtime,caSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0305记录修改出错"),ncsLang("0305记录修改出错, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
        }
        strcpy(caTotRec,"\0");
    }
  
       
       sprintf(caTemp,"select count(*) from ncmailaddr where 1=1");
       if(strlen(caId)!=0)
       sprintf(caTemp+strlen(caTemp)," and id=%s ",caId);
  
       if(strlen(caSerch)!=0)
       {
       	if(strlen(caMail)!=0)
       	sprintf(caTemp+strlen(caTemp)," and mail like '%c%s%c' ",'%',caMail,'%');
       	
       	if(strlen(caName)!=0)
       	sprintf(caTemp+strlen(caTemp)," and name like '%c%s%c' ",'%',caName,'%');
       }
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0561数据统计出错"));
        return 0;
    }
    lTotRec = lCount;
    lStartRec = (lCurPg - 1) * lRowNum;  
        sprintf(caTemp,"select a.sid,a.id,a.mail,a.name,a.modtime,a.flags,b.name from ncmailaddr a left join ncmailclass b on a.id=b.id where 1=1 ");
        if(strlen(caId)!=0)
        sprintf(caTemp+strlen(caTemp)," and a.id=%s ",caId);
        
       if(strlen(caSerch)!=0)
       {
       	if(strlen(caMail)!=0)
       	sprintf(caTemp+strlen(caTemp)," and a.mail like '%c%s%c' ",'%',caMail,'%');
       	
       	if(strlen(caName)!=0)
       	sprintf(caTemp+strlen(caTemp)," and a.name like '%c%s%c' ",'%',caName,'%');
       }
        
        sprintf(caTemp+strlen(caTemp)," order by a.id,a.mail limit %d,%d ",lStartRec,lRowNum);
 //  printf("caTemp=%s\n",caTemp);
        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
   utPltPutVar(psDbHead,"id",caId);
    memset(caName,0,sizeof(caName));
    lModtime=0;
    lStatus=0;
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caMail,
      	                               UT_TYPE_STRING,31,caName,
      	                               UT_TYPE_LONG,4,&lModtime,
      	                               UT_TYPE_LONG,4,&lFlags,
      	                               UT_TYPE_STRING,32,caNamec);
  	                              
   	iNum = 0;
   	while(iReturn == 0 ||iReturn==1405 ) {
   
        iNum++;
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",id);
        utPltPutLoopVar(psDbHead,"mail",iNum,caMail);
        utPltPutLoopVar(psDbHead,"name",iNum,caName);
        utPltPutLoopVar(psDbHead,"namec1",iNum,caNamec);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
       
        utPltPutLoopVarF(psDbHead,"lflags",iNum,"%lu",lFlags);
        utPltPutLoopVar(psDbHead,"modtime",iNum,utTimFormat("%Y/%m/%d %H:%M",lModtime));
        if(lFlags>0)
        utPltPutLoopVar(psDbHead,"sname",iNum,ncsUtlGtypeName(lFlags,"\0"));
        
      
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caMail,
      	                               UT_TYPE_STRING,31,caName,
      	                               UT_TYPE_LONG,4,&lModtime,
      	                               UT_TYPE_LONG,4,&lFlags,
      	                               UT_TYPE_STRING,32,caNamec);
    }
    pasDbCloseCursor(psCur);
    

    lTotPg = (lTotRec - 1) / lRowNum + 1;
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    
    /*取类别*/  
   psCur = pasDbOpenSql("select id,name from ncmailclass  order by name ",0);
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caName);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVarF(psDbHead,"idc",   iNum,"%lu",id);
        utPltPutLoopVar(psDbHead,"namec",iNum,caName);
     iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caName);
    }
    pasDbCloseCursor(psCur); 
       
      utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_mailaddrlist.htm");
 
    return 0;
}

/* 显示用户id类别列表  */
int ncs_userclasslist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[2048];
    char caCurPg[16],caTotRec[16];
    char caSid[16],caModi[16],caDel[16],caAdd[16];
    char caName[32],caApp[16],caDesc[128];
    char caModtime[32],caStatus[16];
    char caSerch[16];
    char caPlate[128];
    char caMods[32];
    unsigned long lModtime,lStatus,lFlags;
    int i,iNum,iReturn;
    long lSid,lCount,id;
    char *p;
    utPltDbHead *psDbHead;
    long lStartRec,lCurPg,lTotRec,lRowNum,lTotPg;
    pasDbCursor *psCur;

#ifdef LDEBUG 	
    utMsgPrintMsg(psMsgHead);
#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,10,    
	                "sid",    UT_TYPE_STRING, 12, caSid,
	                "name",   UT_TYPE_STRING, 31, caName,
	                "status",UT_TYPE_STRING,2,caStatus,
	                "plate",UT_TYPE_STRING,127,caPlate,               
	                "add",        UT_TYPE_STRING, 12, caAdd,	               
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,
	                "mods",       UT_TYPE_STRING,12,caMods,
			"CurPg",      UT_TYPE_STRING, 12, caCurPg,
			"caSerch",    UT_TYPE_STRING, 12,caSerch);
			
    
    lRowNum = 24;
  
    lSid = atol(caSid);		    
    lCurPg = atol(caCurPg);
  
  if(strlen(caStatus)==0)
  strcpy(caStatus,"1");

  
    if(lCurPg <= 0) lCurPg = 1;
   if(!utStrIsSpaces(caAdd)) {  
       
            sprintf(caTemp,"select count(*) from ncuseridclass where name='%s' ",caName);   
	    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	    if(lCount> 0) {
	        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0913类别名称不能重复"),ncsLang("0913类别名称不能重复"));
	        return 0;
	    }
             lSid=0;
            iReturn = pasDbOneRecord("select max(id) from ncuseridclass;",0,UT_TYPE_LONG,4,&lSid);
           
            	if(lSid<20000000)
                lSid = 20000000;
                else
                lSid=lSid+1;
            lModtime=time(0);
            iReturn = pasDbExecSqlF("insert into ncuseridclass(id,name,status,modtime,flags) values \
                     (%lu,'%s',%s,%lu,8)", lSid,caName,caStatus,lModtime);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0904监控单位"),ncsLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                strcpy(caTotRec,"\0");
                pasDbCommit(NULL);
            }
       
    }
    else if(!utStrIsSpaces(caDel)) { 
        char caVar[16];

   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
   	           iReturn=pasDbExecSqlF("delete from ncuseridclass where id=%lu",lSid);
   	            
                iReturn = pasDbExecSqlF("delete from ncuseridlist where id = %lu",lSid);
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    }
     else if(!utStrIsSpaces(caMods)) { /* 修改状态  */
        char caVar[16];

   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
   	           iReturn=pasDbExecSqlF("update ncuseridclass set status=%s where id=%lu",caStatus,lSid);
   	           
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    } 
    else if(!utStrIsSpaces(caModi)) { 

        if(!utStrIsSpaces(caSid)) {
           lModtime=time(0);

            iReturn = pasDbExecSqlF("update ncuseridclass set name='%s',status=%s,modtime=%lu where id = %s",
                    caName,caStatus,lModtime,caSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0305记录修改出错"),ncsLang("0305记录修改出错, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
        }
        strcpy(caTotRec,"\0");
    }
  
      
       sprintf(caTemp,"select count(*) from ncuseridclass ");

    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0561数据统计出错"));
        return 0;
    }
    lTotRec = lCount;
    lStartRec = (lCurPg - 1) * lRowNum;  
  
        psCur = pasDbOpenSqlF("select id,name,modtime,status,flags from ncuseridclass order by id desc limit %d,%d ",lStartRec,lRowNum);
                     
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
   
    memset(caName,0,sizeof(caName));
    lModtime=0;
    lStatus=0;
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,31,caName,
      	                               UT_TYPE_LONG,4,&lModtime,
      	                               UT_TYPE_LONG,4,&lStatus,
      	                               UT_TYPE_LONG,4,&lFlags);
  	                              
   	iNum = 0;
   	while(iReturn == 0 ||iReturn==1405 ) {
   
        iNum++;
        utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",id);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        utPltPutLoopVar(psDbHead,"name",iNum,caName);
        utPltPutLoopVarF(psDbHead,"lstatus",iNum,"%lu",lStatus);
        utPltPutLoopVarF(psDbHead,"lflags",iNum,"%lu",lFlags);
        utPltPutLoopVar(psDbHead,"modtime",iNum,utTimFormat("%Y/%m/%d %H:%M",lModtime));
        if(lFlags>0)
        utPltPutLoopVar(psDbHead,"sname",iNum,ncsUtlGtypeName(lFlags,"\0"));
        
        if(lStatus==1)
        utPltPutLoopVar(psDbHead,"status",iNum,"启用");
        else
        utPltPutLoopVar(psDbHead,"status",iNum,"停用");
        
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,31,caName,
      	                               UT_TYPE_LONG,4,&lModtime,
      	                               UT_TYPE_LONG,4,&lStatus,
      	                               UT_TYPE_LONG,4,&lFlags);
    }
    pasDbCloseCursor(psCur);
    
    lTotPg = (lTotRec - 1) / lRowNum + 1;
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
   
      if(strlen(caPlate)!=0)
      	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
      else
      	utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_userclasslist.htm");
 
    return 0;
}


/* 显示用户id列表  */
int ncs_useridlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[2048];
    char caCurPg[16],caTotRec[16];
    char caSid[16],caModi[16],caDel[16],caAdd[16];
    char caName[32],caApp[16],caDescr[64];
    char caModtime[32],caStatus[16],caId[16],caUname[64],caNamec[64],caSerch[16];
    unsigned long lModtime,lStatus,lFlags;
    int i,iNum,iReturn;
    long lSid,lCount,id;
    char *p;
    utPltDbHead *psDbHead;
    long lStartRec,lCurPg,lTotRec,lRowNum,lTotPg;
    pasDbCursor *psCur;

 	struct stData_s {
 		unsigned long id;
 		char name[64];
 	} *pData;
 	struct stData_s *psData;
  pasLHashInfo sHashInfo; 
  unsigned char *pHash;
  char caNtype[16];
  long lId,lNtype;
   pHash = (unsigned char *)pasLHashInit(1500,1500,sizeof(struct stData_s),0,4);    
    if(pHash == NULL) {   	  
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),"内存分配错误",pasDbErrorMsg(NULL));
        return 0;
    }
   	              
 	
		sprintf(caTemp,"select ntype,name from ncsnettype order by ntype ");  
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




#ifdef LDEBUG 	
    utMsgPrintMsg(psMsgHead);
#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,11,    
	                "sid",    UT_TYPE_STRING, 12, caSid,
	                "id",     UT_TYPE_STRING, 12,caId,
	                "uname",   UT_TYPE_STRING, 63,caUname,
	                "descr",   UT_TYPE_STRING, 31, caDescr,
	                "status",UT_TYPE_STRING,2,caStatus, 
	                "serch",UT_TYPE_STRING,12,caSerch, 
	                "ntype",UT_TYPE_STRING,10,caNtype,             
	                "add",        UT_TYPE_STRING, 12, caAdd,	               
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,	               
			"CurPg",      UT_TYPE_STRING, 12, caCurPg);
			
 
    lRowNum = 22;
  
    lSid = atol(caSid);		    
    lCurPg = atol(caCurPg);
  
  if(strlen(caStatus)==0)
  strcpy(caStatus,"0");
  
  if(strlen(caId)!=0){
  	lStatus=0;
  	sprintf(caTemp,"select status from ncuseridclass where id=%s ",caId);
  	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lStatus);
  }
  
    if(lCurPg <= 0) lCurPg = 1;
    if(!utStrIsSpaces(caAdd)) {  /* 增加记录  */
       
      sprintf(caTemp,"select count(*) from ncuseridlist where id=%s and uname='%s' and ntype=%s ",caId,caUname,caNtype);   
	    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	    if(lCount> 0) {
	        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0913类别名称不能重复"),ncsLang("0913类别名称不能重复"));
	        return 0;
	    }
            
            lModtime=time(0);
            iReturn = pasDbExecSqlF("insert into ncuseridlist(sid,id,uname,descr,modtime,flags,ntype) values \
                     (0,%s,'%s','%s',%lu,8,%d)", caId,caUname,caDescr,lModtime,atol(caNtype));
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0904监控单位"),ncsLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                free(pHash);
                return 0;
            }        
            else {
                strcpy(caTotRec,"\0");
                pasDbCommit(NULL);
            }
       
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];

   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
//   	    printf(" delete from ncmailaddr where sid = %lu",lSid);       
                iReturn = pasDbExecSqlF("delete from ncuseridlist where sid = %lu",lSid);
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    }
    else if(!utStrIsSpaces(caModi)) { /* 修改记录  */

        if(!utStrIsSpaces(caSid)) {
           lModtime=time(0);

            iReturn = pasDbExecSqlF("update ncuseridlist set id=%s,uname='%s',descr='%s',modtime=%lu,ntype=%s where sid = %s",
                    caId,caUname,caDescr,lModtime,caNtype,caSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
            	free(pHash);
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0305记录修改出错"),ncsLang("0305记录修改出错, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
        }
        strcpy(caTotRec,"\0");
    }
  
       
       sprintf(caTemp,"select count(*) from ncuseridlist where 1=1");
       if(strlen(caId)!=0)
       sprintf(caTemp+strlen(caTemp)," and id=%s ",caId);
       
       if(strlen(caSerch)!=0)
       {
       	if(strlen(caUname)!=0)
       sprintf(caTemp+strlen(caTemp)," and uname like '%c%s%c' ",'%',caUname,'%'); 
       
       if(strlen(caDescr)!=0)
       sprintf(caTemp+strlen(caTemp)," and descr like '%c%s%c' ",'%',caDescr,'%');
       }
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0561数据统计出错"));
        return 0;
    }
    lTotRec = lCount;
    lStartRec = (lCurPg - 1) * lRowNum;  
        sprintf(caTemp,"select a.sid,a.id,a.uname,a.descr,a.modtime,a.flags,b.name,a.ntype from ncuseridlist a left join ncuseridclass b on a.id=b.id where 1=1 ");
        if(strlen(caId)!=0)
        sprintf(caTemp+strlen(caTemp)," and a.id=%s ",caId);
       if(strlen(caSerch)!=0)
       {
       	if(strlen(caUname)!=0)
       sprintf(caTemp+strlen(caTemp)," and a.uname like '%c%s%c' ",'%',caUname,'%'); 
       
       if(strlen(caDescr)!=0)
       sprintf(caTemp+strlen(caTemp)," and a.descr like '%c%s%c' ",'%',caDescr,'%');
       }
        sprintf(caTemp+strlen(caTemp)," order by a.id,a.sid desc limit %d,%d ",lStartRec,lRowNum);

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
    	free(pHash);
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
   utPltPutVar(psDbHead,"id",caId);
   utPltPutVarF(psDbHead,"status","%d",lStatus);
    memset(caName,0,sizeof(caName));
    lModtime=0;
    lStatus=0;
    lNtype=0;
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caUname,
      	                               UT_TYPE_STRING,31,caDescr,
      	                               UT_TYPE_LONG,4,&lModtime,
      	                               UT_TYPE_LONG,4,&lFlags,
      	                               UT_TYPE_STRING,32,caNamec,
      	                               UT_TYPE_LONG,4,&lNtype);
  	                              
   	iNum = 0;
   	while(iReturn == 0 ||iReturn==1405 ) {
   
        iNum++;
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",id);
        utPltPutLoopVar(psDbHead,"uname",iNum,caUname);
        utPltPutLoopVar(psDbHead,"descr",iNum,caDescr);
        utPltPutLoopVar(psDbHead,"namec1",iNum,caNamec);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
       
        utPltPutLoopVarF(psDbHead,"lflags",iNum,"%lu",lFlags);
        utPltPutLoopVar(psDbHead,"modtime",iNum,utTimFormat("%Y/%m/%d %H:%M",lModtime));
        if(lFlags>0)
        utPltPutLoopVar(psDbHead,"sname",iNum,ncsUtlGtypeName(lFlags,"\0"));
        
      
       psData = (struct stData_s *)pasLHashLook(pHash,&lNtype);
       if(psData){
       	utPltPutLoopVar(psDbHead,"ntype_name",iNum,psData->name);
      }
      utPltPutLoopVarF(psDbHead,"ntype",iNum,"%d",lNtype);  
      
      lNtype=0;
     iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caUname,
      	                               UT_TYPE_STRING,31,caDescr,
      	                               UT_TYPE_LONG,4,&lModtime,
      	                               UT_TYPE_LONG,4,&lFlags,
      	                               UT_TYPE_STRING,32,caNamec,
      	                               UT_TYPE_LONG,4,&lNtype);
    }
    pasDbCloseCursor(psCur);
    

    lTotPg = (lTotRec - 1) / lRowNum + 1;
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    
    /*取类别*/  
   psCur = pasDbOpenSql("select id,name from ncuseridclass  order by name ",0);
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caName);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVarF(psDbHead,"idc",   iNum,"%lu",id);
        utPltPutLoopVar(psDbHead,"namec",iNum,caName);
     iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caName);
    }
    pasDbCloseCursor(psCur); 
       
 //输出虚拟身份类别   
     iNum=0;   
         psData = (struct stData_s *)pasLHashFirst(pHash,&sHashInfo);
      	 while(psData){
      	 	utPltPutLoopVar(psDbHead,"nname",iNum+1,psData->name);
      	 	utPltPutLoopVarF(psDbHead,"nid",iNum+1,"%d",psData->id);
      	 	iNum++;
      	 	psData = (struct stData_s *)pasLHashNext(&sHashInfo);
      	}
       free(pHash);
       
      utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_useridlist.htm");
 
    return 0;
}



/* 显示敏感信息列表  */
int ncsWebSentiveList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[2048];
    char caCurPg[16],caTotRec[16];
    char caSid[16],caModi[16],caDel[16],caAdd[16];
    char caName[64],caApp[16],caDescr[64];
    char caContent[64],caKtype[8],caCaseflags[8];
    char caModtime[32],caStatus[16],caId[16],caUname[64],caNamec[64],caSerch[16];
    unsigned long lModtime,lStatus,lFlags;
    char caPre[36];
    int i,iNum,iReturn;
    long lSid,lCount,id;
    char *p;
    utPltDbHead *psDbHead;
    long lStartRec,lCurPg,lTotRec,lRowNum,lTotPg;
    long lktype,lcaseflags,ltid;
    pasDbCursor *psCur;
    memset(caPre,0,sizeof(caPre));
#ifdef LDEBUG 	
    utMsgPrintMsg(psMsgHead);
#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,10,    
	                "sid",    UT_TYPE_STRING, 12, caSid,
	                "tid",     UT_TYPE_STRING, 12,caId,
	                "content",   UT_TYPE_STRING, 63,caContent,
	                "ktype",   UT_TYPE_STRING, 6, caKtype,
	                "caseflags",UT_TYPE_STRING,6,caCaseflags, 
	                "serch",UT_TYPE_STRING,12,caSerch,              
	                "add",        UT_TYPE_STRING, 12, caAdd,	               
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,	               
			"CurPg",      UT_TYPE_STRING, 12, caCurPg);
			
 
    lRowNum = 22;
 
    lSid = atol(caSid);		    
    lCurPg = atol(caCurPg);
  
  if(strlen(caKtype)==0) strcpy(caKtype,"0");
  if(strlen(caCaseflags)==0) strcpy(caCaseflags,"0");
  
  
    if(lCurPg <= 0) lCurPg = 1;
    if(!utStrIsSpaces(caAdd)) {  /* 增加记录  */
       
            sprintf(caTemp,"select count(*) from ncskeycont where tid=%s and content=Binary('%s') ",caId,caContent);   
	    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	    if(lCount> 0) {
	        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0913类别名称不能重复"),ncsLang("0913类别名称不能重复"));
	        return 0;
	    }
            
            lModtime=time(0);
            iReturn = pasDbExecSqlF("insert into ncskeycont(sid,tid,content,ktype,caseflags) values \
                     (0,%s,'%s',%s,%s)", caId,caContent,caKtype,caCaseflags);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0904监控单位"),ncsLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                strcpy(caTotRec,"\0");
                pasDbCommit(NULL);
            }
       
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];

   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
//   	    printf(" delete from ncmailaddr where sid = %lu",lSid);       
                iReturn = pasDbExecSqlF("delete from ncskeycont where sid = %lu",lSid);
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    }
    else if(!utStrIsSpaces(caModi)) { /* 修改记录  */

        if(!utStrIsSpaces(caSid)) {
           lModtime=time(0);

            iReturn = pasDbExecSqlF("update ncskeycont set tid=%s,content='%s',ktype=%s,caseflags=%s where sid = %s",
                    caId,caContent,caKtype,caCaseflags,caSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0305记录修改出错"),ncsLang("0305记录修改出错, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
        }
        strcpy(caTotRec,"\0");
    }
  
       
       sprintf(caTemp,"select count(*) from ncskeycont where 1=1");
       if(strlen(caId)!=0)
       sprintf(caTemp+strlen(caTemp)," and tid=%s ",caId);
       
       if(strlen(caSerch)!=0)
       {
       	if(strlen(caContent)!=0)
       sprintf(caTemp+strlen(caTemp)," and content like '%c%s%c' ",'%',caContent,'%'); 
       
       }
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0561数据统计出错"));
        return 0;
    }
    lTotRec = lCount;
    lStartRec = (lCurPg - 1) * lRowNum;  
        sprintf(caTemp,"select a.sid,a.tid,a.content,a.ktype,a.caseflags,b.name from ncskeycont a left join ncskeyindex b on a.tid=b.id where 1=1 ");
        if(strlen(caId)!=0)
        sprintf(caTemp+strlen(caTemp)," and a.tid=%s ",caId);
       if(strlen(caSerch)!=0)
       {
        	if(strlen(caContent)!=0)
       		sprintf(caTemp+strlen(caTemp)," and content like '%c%s%c' ",'%',caContent,'%'); 

       }
        sprintf(caTemp+strlen(caTemp)," order by a.tid,a.content limit %d,%d ",lStartRec,lRowNum);

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
   utPltPutVar(psDbHead,"tid",caId);
    memset(caContent,0,sizeof(caContent));
    lktype=0;
    lcaseflags=0;
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&ltid,
      	                               UT_TYPE_STRING,63,caContent,
      	                               UT_TYPE_LONG,4,&lktype,
      	                               UT_TYPE_LONG,4,&lcaseflags,
      	                               UT_TYPE_STRING,31,caName);
      	                              
  	                              
   	iNum = 0;
   	while(iReturn == 0 ||iReturn==1405 ) {
   
        iNum++;
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"tid",iNum,"%lu",ltid);
        utPltPutLoopVar(psDbHead,"content",iNum,caContent);
        utPltPutLoopVarF(psDbHead,"ktype",iNum,"%lu",lktype);
        utPltPutLoopVarF(psDbHead,"caseflags",iNum,"%lu",lcaseflags);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);

        	utPltPutLoopVar(psDbHead,"type_desc",iNum,caName);
       
        if(lktype==1)
        	utPltPutLoopVar(psDbHead,"ktype_desc",iNum,"正则表达式");
        else if(lktype==2)
           utPltPutLoopVar(psDbHead,"ktype_desc",iNum,"扩展正则表达式");
        else
          utPltPutLoopVar(psDbHead,"ktype_desc",iNum,"文本匹配");
          
       if(lcaseflags==1)
       		utPltPutLoopVar(psDbHead,"caseflags_desc",iNum,"大小写敏感");
       else
       			utPltPutLoopVar(psDbHead,"caseflags_desc",iNum,"大小写不敏感");
       
       memset(caContent,0,sizeof(caContent));
    lktype=0;
    lcaseflags=0;
      iReturn = pasDbFetchInto(psCur,  UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&ltid,
      	                               UT_TYPE_STRING,63,caContent,
      	                               UT_TYPE_LONG,4,&lktype,
      	                               UT_TYPE_LONG,4,&lcaseflags,
      	                               UT_TYPE_STRING,31,caName);
    }
    pasDbCloseCursor(psCur);
    

    lTotPg = (lTotRec - 1) / lRowNum + 1;
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    
    /*取类别*/  
   psCur = pasDbOpenSql("select id,name from ncskeyindex  order by name ",0);
   memset(caName,0,sizeof(caName));
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caName);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVarF(psDbHead,"idc",   iNum,"%lu",id);
        utPltPutLoopVar(psDbHead,"namec",iNum,caName);
        memset(caName,0,sizeof(caName));
     iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caName);
    }
    pasDbCloseCursor(psCur); 

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/nc_sentivelist.htm");
    return 0;
}


int ncsWebSentiveGroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	
	    char caTemp[2048];
    char caCurPg[16],caTotRec[16];
    char caSid[16],caModi[16],caDel[16],caAdd[16];
    char caName[32],caApp[16],caDesc[128];
    char caModtime[32],caStatus[16];
    char caSerch[16];
    char caPlate[128];
    char caMods[32];
    unsigned long lModtime,lStatus,lFlags,lcurtime,lGid;
    int i,iNum,iReturn;
    long lSid,lCount,id;
    char *p;
    utPltDbHead *psDbHead;
    long lStartRec,lCurPg,lTotRec,lRowNum,lTotPg;
    pasDbCursor *psCur;

#ifdef LDEBUG 	
    utMsgPrintMsg(psMsgHead);
#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,8,    
	                "sid",    UT_TYPE_STRING, 12, caSid,
	                "name",   UT_TYPE_STRING, 31, caName,
	                "plate",UT_TYPE_STRING,127,caPlate,               
	                "add",        UT_TYPE_STRING, 12, caAdd,	               
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,
	                "mods",       UT_TYPE_STRING,12,caMods,
			            "CurPg",      UT_TYPE_STRING, 12, caCurPg);
			
		lcurtime=time(0);	
    
    lRowNum = 24;
  
    lSid = atol(caSid);		    
    lCurPg = atol(caCurPg);
  
  if(strlen(caStatus)==0)
  strcpy(caStatus,"0");

  
    if(lCurPg <= 0) lCurPg = 1;
   if(!utStrIsSpaces(caAdd)) {  
       
            sprintf(caTemp,"select count(*) from ncskeyindex where name='%s' ",caName);   
	    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	    if(lCount> 0) {
	        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0913类别名称不能重复"),ncsLang("0913类别名称不能重复"));
	        return 0;
	    }
      lGid = ncsUtlGenId(psShmHead);
            iReturn = pasDbExecSqlF("insert into ncskeyindex(id,name,lasttime,flags) values (%lu,'%s',%lu,8)",
                                             lGid,caName,lcurtime);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0316记录增加出错,可能为ID重复"));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                pasDbCommit(NULL);
            }                
           
       
    }
    else if(!utStrIsSpaces(caDel)) { 
        char caVar[16];

   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
   	           iReturn=pasDbExecSqlF("delete from ncskeyindex where id=%lu",lSid);
   	            
                iReturn = pasDbExecSqlF("delete from ncskeycont where tid = %lu",lSid);
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    }
    else if(!utStrIsSpaces(caModi)) { 

        if(!utStrIsSpaces(caSid)) {
           lModtime=time(0);

            iReturn = pasDbExecSqlF("update ncskeyindex set name='%s',lasttime=%lu where id = %s",
                    caName,lcurtime,caSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0305记录修改出错"),ncsLang("0305记录修改出错, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
        }
        strcpy(caTotRec,"\0");
    }
  
      
       sprintf(caTemp,"select count(*) from ncskeyindex ");

    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0561数据统计出错"));
        return 0;
    }
    lTotRec = lCount;
    lStartRec = (lCurPg - 1) * lRowNum;  
  
        psCur = pasDbOpenSqlF("select id,name,lasttime,flags from ncskeyindex order by id desc limit %d,%d ",lStartRec,lRowNum);
                     
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
   
    memset(caName,0,sizeof(caName));
    lModtime=0;
    lStatus=0;
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,31,caName,
      	                               UT_TYPE_LONG,4,&lModtime,
      	                               UT_TYPE_LONG,4,&lFlags);
  	                              
   	iNum = 0;
   	while(iReturn == 0 ||iReturn==1405 ) {
   
        iNum++;
        utPltPutLoopVarF(psDbHead,"id",iNum,"%lu",id);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        utPltPutLoopVar(psDbHead,"name",iNum,caName);
        utPltPutLoopVarF(psDbHead,"lflags",iNum,"%lu",lFlags);
        utPltPutLoopVar(psDbHead,"modtime",iNum,utTimFormat("%Y/%m/%d %H:%M",lModtime));
        if(lFlags>0)
        utPltPutLoopVar(psDbHead,"sname",iNum,ncsUtlGtypeName(lFlags,"\0"));
        
               
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,31,caName,
      	                               UT_TYPE_LONG,4,&lModtime,
      	                               UT_TYPE_LONG,4,&lFlags);
    }
    pasDbCloseCursor(psCur);
    
    lTotPg = (lTotRec - 1) / lRowNum + 1;
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
	
    utPltPutVar(psDbHead,"plate",caPlate);
    if(strlen(caPlate)>0)
    utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    else
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"tree/nc_sentivetree.htm");
    return 0;
}

int ncsWebSentiveEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
   	long lId;
   	unsigned long lGid;
   	char caName[64];
   	char caPlate[128];
   	char caDid[16],caAdd[16],caDel[16];
   	char caNname[32],caNid[16],caModi[16];
   	unsigned long lcurtime;
   	time(&lcurtime);
 	pasDbCursor *psCur;
 	
	iReturn = utMsgGetSomeNVar(psMsgHead,8,
	                "gname",  UT_TYPE_STRING,63,caName,
	                "did",    UT_TYPE_STRING,12,caDid,
	                "add",    UT_TYPE_STRING,12,caAdd,
	                "ddd",    UT_TYPE_STRING,12,caDel,
	                "plate",  UT_TYPE_STRING,127,caPlate,
	                "nid",    UT_TYPE_STRING,12,caNid,
	                "modi",   UT_TYPE_STRING,12,caModi,
	                "nname",  UT_TYPE_STRING,31,caNname
	                );
    if(!utStrIsSpaces(caAdd)) { /* 增加一个组  */
        if(!utStrIsSpaces(caName)) {
            lGid = ncsUtlGenId(psShmHead);
            iReturn = pasDbExecSqlF("insert into ncskeyindex(id,name,lasttime,flags) values (%lu,'%s',%lu,8)",
                                             lGid,caName,lcurtime);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0316记录增加出错,可能为ID重复"));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                pasDbCommit(NULL);
            }                
        }
        else {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0363类别名称为空或ID出错"));
            return 0;
       }            
            
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除一个组  */
    	   long i,lSid;
    	   char *p;
    	   char caVar[16];
   	    for(i=0;i<50;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
   	            iReturn = pasDbExecSqlF("delete from ncskeyindex where id = %lu",lSid);
                if(iReturn==0)
                  iReturn = pasDbExecSqlF("delete from ncskeycont where tid = %lu",lSid);
                            
   	        }
                 
   	    }
    	
        iReturn = pasDbExecSqlF("delete from ncskeyindex where id= %s",caDid);
        if(iReturn != 0) {
            pasDbRollback(NULL);
        }                
        else {
            iReturn = pasDbExecSqlF("delete from ncskeycont where tid= %s",caDid);
            pasDbCommit(NULL);
        }            
    }
    else if(!utStrIsSpaces(caModi) && !utStrIsSpaces(caNname)) { /* 修改一个组  */
        iReturn = pasDbExecSqlF("update ncskeyindex set name='%s' where id = %s",
                         caNname,caNid);
        pasDbCommit(NULL);
    }
    return ncsWebSentiveGroup(psShmHead,iFd,psMsgHead);
    return 0;
}


/* 显示网站社区列表  */
int ncsWebSiteBbsList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[2048];
    char caCurPg[16],caTotRec[16];
    char caSid[16],caModi[16],caDel[16],caAdd[16];
    char caName[64],caApp[16],caDescr[64];
    char caContent[64],caKtype[8],caCaseflags[8];
    char caModtime[32],caStatus[16],caId[16],caUname[64],caNamec[64],caSerch[16];
    unsigned long lModtime,lStatus,lFlags;
    char caHost[64],caUrl[64],caVarname[32];
    int i,iNum,iReturn;
    long lSid,lCount,id;
    char *p;
    utPltDbHead *psDbHead;
    long lStartRec,lCurPg,lTotRec,lRowNum,lTotPg;
    long lktype,lcaseflags,ltid;
    pasDbCursor *psCur;

#ifdef LDEBUG 	
    utMsgPrintMsg(psMsgHead);
#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,10,    
	                "sid",    UT_TYPE_STRING, 12, caSid,
	                "tid",     UT_TYPE_STRING, 12,caId,
	                "host",   UT_TYPE_STRING, 63,caHost,
	                "uri",   UT_TYPE_STRING, 63, caUrl,
	                "varname",UT_TYPE_STRING,31,caVarname, 
	                "serch",UT_TYPE_STRING,12,caSerch,              
	                "add",        UT_TYPE_STRING, 12, caAdd,	               
	                "modi",       UT_TYPE_STRING, 12, caModi,
	                "del",        UT_TYPE_STRING, 12, caDel,	               
			"CurPg",      UT_TYPE_STRING, 12, caCurPg);
			
 
    lRowNum = 22;
  
    lSid = atol(caSid);		    
    lCurPg = atol(caCurPg);
  
   
  
    if(lCurPg <= 0) lCurPg = 1;
    if(!utStrIsSpaces(caAdd)) {  /* 增加记录  */
       
    
            
            lModtime=time(0);
            iReturn = pasDbExecSqlF("insert into ncsbbsvar(sid,tid,host,url,varname) values \
                     (0,%s,'%s','%s','%s')", caId,caHost,caUrl,caVarname);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0904监控单位"),ncsLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                strcpy(caTotRec,"\0");
                pasDbCommit(NULL);
            }
       
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];

   	    for(i=0;i<lRowNum;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	            lSid = atol(p);
//   	    printf(" delete from ncmailaddr where sid = %lu",lSid);       
                iReturn = pasDbExecSqlF("delete from ncsbbsvar where sid = %lu",lSid);
   	        }
            strcpy(caTotRec,"\0");   	        
   	    }
    }
    else if(!utStrIsSpaces(caModi)) { /* 修改记录  */

        if(!utStrIsSpaces(caSid)) {
           lModtime=time(0);

            iReturn = pasDbExecSqlF("update ncsbbsvar set tid=%s,host='%s',url='%s',varname='%s' where sid = %s",
                    caId,caHost,caUrl,caVarname,caSid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
            else {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0305记录修改出错"),ncsLang("0305记录修改出错, %s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
            }
        }
        strcpy(caTotRec,"\0");
    }
  
       
       sprintf(caTemp,"select count(*) from ncsbbsvar where 1=1");
       if(strlen(caId)!=0)
       sprintf(caTemp+strlen(caTemp)," and tid=%s ",caId);
       
       if(strlen(caSerch)!=0)
       {
       	if(strlen(caHost)!=0)
       sprintf(caTemp+strlen(caTemp)," and host like '%c%s%c' ",'%',caHost,'%'); 
       if(strlen(caUrl)!=0)
       sprintf(caTemp+strlen(caTemp)," and url like '%c%s%c' ",'%',caUrl,'%');
       
       if(strlen(caVarname)!=0)
       sprintf(caTemp+strlen(caTemp)," and varname like '%c%s%c' ",'%',caVarname,'%');
       
       
       }
    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0561数据统计出错"));
        return 0;
    }
    lTotRec = lCount;
    lStartRec = (lCurPg - 1) * lRowNum;  
        sprintf(caTemp,"select a.sid,a.tid,a.host,a.url,a.varname,b.name from ncsbbsvar a left join ncsbbsindex b on a.tid=b.id where 1=1 ");
        if(strlen(caId)!=0)
        sprintf(caTemp+strlen(caTemp)," and a.tid=%s ",caId);
       if(strlen(caSerch)!=0)
       {
       	if(strlen(caHost)!=0)
       sprintf(caTemp+strlen(caTemp)," and host like '%c%s%c' ",'%',caHost,'%'); 
       if(strlen(caUrl)!=0)
       sprintf(caTemp+strlen(caTemp)," and url like '%c%s%c' ",'%',caUrl,'%');
       
       if(strlen(caVarname)!=0)
       sprintf(caTemp+strlen(caTemp)," and varname like '%c%s%c' ",'%',caVarname,'%');

       }
        sprintf(caTemp+strlen(caTemp)," order by a.tid,a.host limit %d,%d ",lStartRec,lRowNum);

        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
   utPltPutVar(psDbHead,"tid",caId);
    memset(caHost,0,sizeof(caHost));
		memset(caUrl,0,sizeof(caUrl));
		memset(caVarname,0,sizeof(caVarname));
		memset(caName,0,sizeof(caName));
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&ltid,
      	                               UT_TYPE_STRING,63,caHost,
      	                               UT_TYPE_STRING,63,caUrl,
      	                               UT_TYPE_STRING,31,caVarname,
      	                               UT_TYPE_STRING,31,caName);
      	                              
  	                              
   	iNum = 0;
   	while(iReturn == 0 ||iReturn==1405 ) {
   
        iNum++;
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"tid",iNum,"%lu",ltid);
        utPltPutLoopVar(psDbHead,"host",iNum,caHost);
        utPltPutLoopVar(psDbHead,"uri",iNum,caUrl);
        utPltPutLoopVar(psDbHead,"varname",iNum,caVarname);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        utPltPutLoopVar(psDbHead,"type_desc",iNum,caName);

    
    memset(caHost,0,sizeof(caHost));
		memset(caUrl,0,sizeof(caUrl));
		memset(caVarname,0,sizeof(caVarname));
		memset(caName,0,sizeof(caName));
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
      	                               UT_TYPE_LONG,4,&ltid,
      	                               UT_TYPE_STRING,63,caHost,
      	                               UT_TYPE_STRING,63,caUrl,
      	                               UT_TYPE_STRING,31,caVarname,
      	                               UT_TYPE_STRING,31,caName);
      	                              
    }
    pasDbCloseCursor(psCur);
    

    lTotPg = (lTotRec - 1) / lRowNum + 1;
    i = 0;
    while(iNum < lRowNum + 1) {
        i++;
        iNum++;
        utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    }
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    
    /*取类别*/  
   psCur = pasDbOpenSql("select id,name from ncsbbsindex  order by name ",0);
   memset(caName,0,sizeof(caName));
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caName);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVarF(psDbHead,"idc",   iNum,"%lu",id);
        utPltPutLoopVar(psDbHead,"namec",iNum,caName);
        memset(caName,0,sizeof(caName));
     iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&id,
      	                               UT_TYPE_STRING,63,caName);
    }
    pasDbCloseCursor(psCur); 
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"ncs/ncs_websitbbslist.htm");
    return 0;
}


int ncsWebSiteBbsGroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
   	long lId;
   	char caName[64];
   	char caPlate[128];
 	pasDbCursor *psCur;
 	
	iReturn = utMsgGetSomeNVar(psMsgHead,1,
	                "plate",  UT_TYPE_STRING,127,caPlate);
    psCur = pasDbOpenSqlF("select id,name from ncsbbsindex where ntype=0 order by lasttime desc ");
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0746用户查询"),ncsLang("0524时间信息出错"));
        return 0;
    }
    psDbHead = utPltInitDb();
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lId,
  	                               UT_TYPE_STRING,63,caName);
   	iNum = 0;
   	while(iReturn == 0) {
        iNum++;
        utPltPutLoopVarF(psDbHead,"id",   iNum,"%u",lId);
        utPltPutLoopVarF(psDbHead,"name", iNum,"%s",caName);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%u",iNum);
        iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,4,&lId,
  	                               UT_TYPE_STRING,63,caName);
    }
    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead,"dateid","%lu",utStrGetId());
    utPltPutVar(psDbHead,"plate",caPlate);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    return 0;
}

int ncsWebSiteBbsEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
   	long lId,i;
   	unsigned long lGid;
   	char caName[64];
   	char caPlate[128];
   	char caDid[16],caAdd[16],caDel[16];
   	char caNname[32],caNid[16],caModi[16];
   	unsigned long lcurtime;
   	time(&lcurtime);
 	pasDbCursor *psCur;
 	
	iReturn = utMsgGetSomeNVar(psMsgHead,8,
	                "gname",  UT_TYPE_STRING,63,caName,
	                "did",    UT_TYPE_STRING,12,caDid,
	                "add",    UT_TYPE_STRING,12,caAdd,
	                "ddd",    UT_TYPE_STRING,12,caDel,
	                "plate",  UT_TYPE_STRING,127,caPlate,
	                "nid",    UT_TYPE_STRING,12,caNid,
	                "modi",   UT_TYPE_STRING,12,caModi,
	                "nname",  UT_TYPE_STRING,31,caNname
	                );
    if(!utStrIsSpaces(caAdd)) { /* 增加一个组  */
        if(!utStrIsSpaces(caName)) {
            lGid = ncsUtlGenId(psShmHead);
            iReturn = pasDbExecSqlF("insert into ncsbbsindex(id,name,lasttime,flags) values (%lu,'%s',%lu,8)",
                                             lGid,caName,lcurtime);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0316记录增加出错,可能为ID重复"));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
                pasDbCommit(NULL);
            }                
        }
        else {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0363类别名称为空或ID出错"));
            return 0;
       }            
            
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除一个组  */
    	
    	  char caVar[16];
        char *p;
   	    for(i=0;i<100;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
                iReturn = pasDbExecSqlF("delete from ncsbbsindex where id=%s",p);
                if(iReturn == 0) {
                   pasDbExecSqlF("delete from ncsbbsvar where tid= %s",p);
                    pasDbCommit(NULL);
                }
   	        }
	      } 	
            
    }
    else if(!utStrIsSpaces(caModi) && !utStrIsSpaces(caName)) { /* 修改一个组  */
        iReturn = pasDbExecSqlF("update ncsbbsindex set name='%s' where id = %s",
                         caName,caNid);
        pasDbCommit(NULL);
    }
    return ncsWebSiteBbsGroup(psShmHead,iFd,psMsgHead);
    return 0;
}

int ncsNetidGroupEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
   	long lId;
   	unsigned long lSid;
   	char caName[64];
   	char caPlate[128];
   	char caDid[16],caAdd[16],caDel[16];
   	char caNname[32],caNid[16],caModi[16];
   	char caStatus[8],caStatus_m[8];
   	unsigned long lModtime;
    char caTemp[1024];
    unsigned long lCount;
 	pasDbCursor *psCur;
 	
	iReturn = utMsgGetSomeNVar(psMsgHead,10,
	                "gname",  UT_TYPE_STRING,63,caName,
	                "did",    UT_TYPE_STRING,12,caDid,
	                "add",    UT_TYPE_STRING,12,caAdd,
	                "ddd",    UT_TYPE_STRING,12,caDel,
	                "plate",  UT_TYPE_STRING,127,caPlate,
	                "nid",    UT_TYPE_STRING,12,caNid,
	                "modi",   UT_TYPE_STRING,12,caModi,
	                "nname",  UT_TYPE_STRING,31,caNname,
	                "status", UT_TYPE_STRING,4,caStatus,
	                "mstatus",UT_TYPE_STRING,4,caStatus_m
	                );
    if(!utStrIsSpaces(caAdd)) { /* 增加一个组  */
        if(!utStrIsSpaces(caName)) {
            sprintf(caTemp,"select count(*) from ncuseridclass where name='%s' ",caName);   
	    iReturn = pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
	    if(lCount> 0) {
	        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0913类别名称不能重复"),ncsLang("0913类别名称不能重复"));
	        return 0;
	    }
             lSid=0;
            iReturn = pasDbOneRecord("select max(id) from ncuseridclass;",0,UT_TYPE_LONG,4,&lSid);
           
            	if(lSid<20000000)
                lSid = 20000000;
                else
                lSid=lSid+1;
            lModtime=time(0);
            iReturn = pasDbExecSqlF("insert into ncuseridclass(id,name,status,modtime,flags) values \
                     (%lu,'%s',%s,%lu,8)", lSid,caName,caStatus,lModtime);
            if(iReturn != 0) {
                utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0904监控单位"),ncsLang("0311记录增加出错%s"),pasDbErrorMsg(NULL));
                pasDbRollback(NULL);
                return 0;
            }        
            else {
               
                pasDbCommit(NULL);
            }
        }
        else {
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0086Web地址列表"),ncsLang("0363类别名称为空或ID出错"));
            return 0;
       }            
            
    }
    else if(!utStrIsSpaces(caDel)) { /* 删除一个组  */
        iReturn = pasDbExecSqlF("delete from ncuseridclass where id= %s",caDid);
        if(iReturn != 0) {
            pasDbRollback(NULL);
        }                
        else {
            iReturn = pasDbExecSqlF("delete from ncuseridlist where id= %s",caDid);
            pasDbCommit(NULL);
        }            
    }
    else if(!utStrIsSpaces(caModi) && !utStrIsSpaces(caNname)) { /* 修改一个组  */
    	       lModtime=time(0);
            iReturn = pasDbExecSqlF("update ncuseridclass set name='%s',status=%s,modtime=%lu where id = %s",
                    caNname,caStatus_m,lModtime,caNid);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
       
    }
    return ncs_userclasslist(psShmHead,iFd,psMsgHead);
    
}


int ncs_netidclass_mod(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    int iNum,iReturn;
    utPltDbHead *psDbHead;
   	long lId;
   	unsigned long lSid;
    unsigned long lModtime;
   	char caStatus[8],caId[16];
   
    char caTemp[1024];
 	
	iReturn = utMsgGetSomeNVar(psMsgHead,2,
	                "id",  UT_TYPE_STRING,12,caId,
	                "status",UT_TYPE_STRING,4,caStatus);
	               
   
         
     if(!utStrIsSpaces(caId) && !utStrIsSpaces(caStatus)) { /* 修改一个组  */
    	       lModtime=time(0);
            iReturn = pasDbExecSqlF("update ncuseridclass set status=%s,modtime=%lu where id = %s",
                    caStatus,lModtime,caId);
            if(iReturn == 0) {
                pasDbCommit(NULL);
            }
       
    }
    return ncs_useridlist(psShmHead,iFd,psMsgHead);
    
}



int pmUtlGetCsvWord(char *caBuf,int iNum,char *caName)
{
    char *p;
    int i;
    char caTemp[256];
    p = caBuf;
    i=0;
    while(p && *p) {
        p = utStrGetWord(p,caTemp,255,",\r\n");
        if(i==iNum) {
            strcpy(caName,caTemp);
            return 0;
        }
        if(*p != ',') {
            strcpy(caName,"\0");
            return 0;
        }
        p++;
        i++;
    }
}



/* 显示网址库资料导入next界面  */
int ncWebDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
//chmod("/home/ncmysql/ncs/upload",777);
system("chmod -Rf 777 /home/ncmysql/ncs/upload");
	/*取网址类别*/ 
  sprintf(sqlbuf,"select id,name from ncwebclass where flags!=9 order by name ");
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
        if(!utStrIsSpaces(caTemp)) {
            iNum++;
            utPltPutLoopVar(psDbHead,"caTemp",iNum,caTemp);
            utPltPutLoopVarF(psDbHead,"iNum",iNum,"%lu",iNum-1);
            
            if(iNum==1)
            {
            utPltPutVar(psDbHead,"webclass",caTemp);
            utPltPutVarF(psDbHead,"webclass_num","%lu",iNum-1);
            }
            else if(iNum==2)
            {
            utPltPutVar(psDbHead,"urlname",caTemp);
            utPltPutVarF(psDbHead,"urlname_num","%lu",iNum-1);
            }
            else if(iNum==3)
            {
            utPltPutVar(psDbHead,"urldesc",caTemp);
            utPltPutVarF(psDbHead,"urldesc_num","%lu",iNum-1);
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
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/web_import_next.htm");
    return 0;
}

//导网址库资料资料
int webclass_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
	char *APName;

	long sum=0;
	utPltDbHead *psDbHead;
//	psDbHead = utPltInitDb();
//utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,6,
					"webclass",UT_TYPE_STRING,11,caIdNum,
					"urlname",UT_TYPE_STRING,11,caUrlNum,
					"urldesc",UT_TYPE_STRING,11,caNameNum,
					"defclass",UT_TYPE_STRING,10,caDefid,
		      "first",     UT_TYPE_STRING, 11,  caFirst,
		      "fname",   UT_TYPE_STRING, 127, caFile);
		      
	lTime=time(0);
	
	if(utStrIsSpaces(caIdNum)) {
        lId = 999;
    }
    else {
        lId = atol(caIdNum);
    }
    if(utStrIsSpaces(caUrlNum)) {
        lUrl = 999;
    }
    else {
        lUrl = atol(caUrlNum);
    }
   
    if(utStrIsSpaces(caNameNum)) {
        lName = 999;
    }
    else {
        lName = atol(caNameNum);
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
        pmUtlGetCsvWord(caBuf,lId,caUrlClass);
        pmUtlGetCsvWord(caBuf,lUrl,caUrl);
        pmUtlGetCsvWord(caBuf,lName,caName);
       

    if(strlen(caUrl)==0) strcpy(caUrl," ");
    if(strlen(caName)==0) strcpy(caName," ");  
      if(!utStrIsSpaces(caUrlClass)){
      	sprintf(sqlbuf,"select id from ncwebclass where name='%s' ",caUrlClass);
//printf("sqlbuf=%s\n",sqlbuf);
      	lIdn=0;
      	tReturn=pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lIdn);
      	if(tReturn==0){
      		id=lIdn;
      	}
      	else{
      		id=ncsUtlGenId(psShmHead);
      		sprintf(sqlbuf,"insert into ncwebclass (id,name,modtime,lcount,status,flags) values(%lu,'%s',%lu,1,1,1) ",\
      			id,caUrlClass,lTime);
      		pasDbExecSql(sqlbuf,0);
      		
     		printf("sqlbuf=%s\n",sqlbuf);
      	}
      }
      else{
      	id=atol(caDefid);
      }
        
      if(!utStrIsSpaces(caUrl)) {
			lcount=0;
			sprintf(sqlbuf,"select count(*) from ncweblist where url='%s' and id=%lu   ",caUrl,id);
//	printf("sqlbuf=%s\n",sqlbuf);		
			pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lcount);

			if(lcount==0)
			{
				
				sprintf(sqlbuf,"insert into ncweblist (id,url,name,modtime) \
					values(%lu,'%s','%s',%lu) ",\
					id,caUrl,caName,lTime);
printf("sqlbuf=%s\n",sqlbuf);																																									                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          					                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
				pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
				pasDbCommit(NULL);
			}                               
        }
        p = fgets(caBuf,10000,fp);
    }
    fclose(fp);
    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","Sucess!","Sucess!");   	
	return 0;

}

/* 显示Web地址库类别查询  */
int ncWebLimWebClassCx(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[1024];
    char caUrl[256],caDel[16],caMov[16];
    int iReturn;
    char caClassName[32];
    char caName[64];
    long lFlag,lStatus,iNum,lSid;
    char caClassname[32];
    char caClassid[16];
    long classid,i;
    utPltDbHead *psDbHead;
   	
 	pasDbCursor *psCur;

	iReturn = utMsgGetSomeNVar(psMsgHead,4,
	                "caurl",      UT_TYPE_STRING, 250, caUrl,
	                "del",       UT_TYPE_STRING,  12, caDel,
	                "mov",       UT_TYPE_STRING,  12,caMov,
	                "webclass",  UT_TYPE_STRING,  12,caClassid);
 psDbHead = utPltInitDb();	                
if(strlen(caUrl)!=0)
{
	utPltPutVar(psDbHead,"caurl",caUrl);
	 if(!utStrIsSpaces(caDel)) { /* 删除记录  */
        char caVar[16];
        char *p;
   	    for(i=0;i<100;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
   	        	iReturn=pasDbExecSqlF("update ncweblist set flags=99 where sid = %s",p);
//                iReturn = pasDbExecSqlF("delete from ncweblist where sid = %s",p);
                if(iReturn == 0) {
                  
                    pasDbCommit(NULL);
                }
   	        }
	}
      }
      
      	 if(!utStrIsSpaces(caMov)) { /* 移动  */
        char caVar[16];
        char *p;
   	    for(i=0;i<100;i++) {
   	        sprintf(caVar,"ch%d",i+1);
   	        p = utMsgGetVar_s(psMsgHead,caVar);
   	        if(p) {
                iReturn = pasDbExecSqlF("update ncweblist set id=%d  where sid = %s",atol(caClassid),p);
                if(iReturn == 0) {
                  
                    pasDbCommit(NULL);
                }
   	        }
	}
      }
      
    sprintf(caTemp,"select b.sid,a.name,b.url,b.name,a.flags,a.status from ncwebclass a,ncweblist b where a.id=b.id and b.url like '%c%s%c' ",'%',caUrl,'%');
    printf("caTemp=%s\n",caTemp);
    psCur=pasDbOpenSql(caTemp,0);

    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0746用户查询"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
    				       UT_TYPE_STRING,30,caClassName,
    				       UT_TYPE_STRING,60,caUrl,
    				       UT_TYPE_STRING,60,caName,
    				       UT_TYPE_LONG,4,&lFlag,
    				       UT_TYPE_LONG,4,&lStatus);
      	                               
   	iNum = 0;
   	while(iReturn == 0 ||iReturn==1405) {
        iNum++;
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum);
        utPltPutLoopVar(psDbHead,"classname",   iNum,caClassName);
        utPltPutLoopVar(psDbHead, "caurl",  iNum,caUrl);
        utPltPutLoopVar(psDbHead,"caName",iNum,caName);
        utPltPutLoopVar(psDbHead, "lsname",  iNum,ncsUtlGtypeName(lFlag,"\0"));
        if(lStatus == 0) {
            utPltPutLoopVar(psDbHead,"lusename",  iNum,ncsLang("0568停用"));
        }
        else {
            utPltPutLoopVar(psDbHead,"lusename",  iNum,ncsLang("0437启用"));
        }
    iReturn = pasDbFetchInto(psCur,    UT_TYPE_LONG,4,&lSid,
    				       UT_TYPE_STRING,30,caClassName,
    				       UT_TYPE_STRING,60,caUrl,
    				       UT_TYPE_STRING,60,caName,
    				       UT_TYPE_LONG,4,&lFlag,
    				       UT_TYPE_LONG,4,&lStatus);
    }
    pasDbCloseCursor(psCur);
   
 }
 
   
   sprintf(caTemp,"select id,name from ncwebclass ");
   psCur=pasDbOpenSql(caTemp,0);
   iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&classid,
                                UT_TYPE_STRING,30,caClassname);
   iNum=0;
   while(iReturn==0)
   { 
   	iNum++;
   	utPltPutLoopVarF(psDbHead,"classid",iNum,"%lu",classid);
   	utPltPutLoopVar(psDbHead,"cname",iNum,caClassname);                               
        iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&classid,
                                UT_TYPE_STRING,30,caClassname);
        }
        pasDbCloseCursor(psCur);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/nc_limwebclasscx.htm");
   
    return 0;
}



/* 显示关键字导入next界面  */
int ncKeywordDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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

	/*取网址类别*/ 
  sprintf(sqlbuf,"select id,name from ncskeyindex order by name ");
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
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"nc/keyword_import_next.htm");
    return 0;
}


//导入关键字
int ncskeyword_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
  char caDeflev[12];
  long lLev=0;
	long sum=0;
	utPltDbHead *psDbHead;
//	psDbHead = utPltInitDb();
//utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,5,
					"keyword",UT_TYPE_STRING,11,caIdNum,
					"defclass",UT_TYPE_STRING,10,caDefid,
		      "first",     UT_TYPE_STRING, 11,  caFirst,
		      "fname",   UT_TYPE_STRING, 127, caFile,
		      "deflev", UT_TYPE_STRING,10,caDeflev);
		      
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
			sprintf(sqlbuf,"select count(*),caseflags from ncskeycont where content=Binary('%s') and tid=%lu   ",caKeyword,id);
//	printf("sqlbuf=%s\n",sqlbuf);	
      lLev=0;	
			pasDbOneRecord(sqlbuf,0,UT_TYPE_LONG,4,&lcount,
			                        UT_TYPE_LONG,4,&lLev);

			if(lcount==0)
			{
				
				sprintf(sqlbuf,"insert into ncskeycont (sid,tid,content,caseflags) \
					values(0,%lu,'%s',%lu) ",\
					id,caKeyword,atol(caDeflev));
printf("sqlbuf=%s\n",sqlbuf);																																									                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          					                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
				pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
				pasDbCommit(NULL);
			}
			else {
				if(lLev<atol(caDeflev)){
					sprintf(sqlbuf,"update ncskeycont set caseflags=%d,tid=%d where content=Binary('%s')   ",atol(caDeflev),id,caKeyword);
					pasDbExecSql(sqlbuf,0);																                                                                                                                                                                                                                                                                                                                                                                                                          
				  pasDbCommit(NULL);
				}
			}                               
        }
        p = fgets(caBuf,10000,fp);
    }
    fclose(fp);
    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","导入完成!","导入完成!");   	
	return 0;

}
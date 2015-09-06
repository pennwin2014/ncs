/* 监控软件---有关Web的应用
     Modify 2002/9/9   by Liyunming*/
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

char * getRqlxId(char *pType){
	static char caReturn[100];
	memset(caReturn,0,sizeof(caReturn));
	if(strcmp(pType,"1")==0){
		strcpy(caReturn,"10,21,22,23,26,27,29");
	}
	else if(strcmp(pType,"2")==0){
		strcpy(caReturn,"10,21,22,23,29");
	}
	else if(strcmp(pType,"3")==0){
		strcpy(caReturn,"24,25,28");
	}
	else{
		strcpy(caReturn,"");
	}
	return &caReturn[0];
}



 struct areastatdata_s{
 	 	unsigned long pid;  //父节点ID
 		unsigned long id;
 		int  ondid_count;   //在线单位数
 		int  ycdid_count;   //异常单位数
 		int  wh_count;      //维护单位数
 		int  ty_count;      //停业单位数
 		int  did_count;     //单位数
 		int  onuid_count;   //在线用户数
 		int  endnum;      //终端用户数
 		int  ondid_count_z;   //儿孙节点在线单位数
 		int  ycdid_count_z;   //儿孙节点异常单位数
 		int  did_count_z;     //儿孙节点单位数
 		int  onuid_count_z;   //儿孙节点在线用户数
 		int  endnum_z;        //儿孙节点终端用户数
 		int  wh_count_z;
 		int  ty_count_z;
 		char groupname[32];
 		struct areastatdata_s * pArea;
 		long lSum;             //子节点个数
 	};

char* ncsGetJoinTypeByCode(char *pType){
 		static char caTname[32];
 		memset(caTname,0,sizeof(caTname));
 		if(strcmp(pType,"01")==0){
 			strcpy(caTname,"专网真实IP地址");
 		}
 		else if(strcmp(pType,"02")==0){
 			strcpy(caTname,"专线");
 		}
 		else if(strcmp(pType,"03")==0){
 			strcpy(caTname,"ADSL拨号");
 		}
 		else if(strcmp(pType,"04")==0){
 			strcpy(caTname,"ISDN");
 		}
 		else if(strcmp(pType,"05")==0){
 			strcpy(caTname,"普通拨号");
 		}
 		else if(strcmp(pType,"06")==0){
 			strcpy(caTname,"Cable Moderm 拨号");
 		}
 		else if(strcmp(pType,"07")==0){
 			strcpy(caTname,"电力线");
 		}
 		else if(strcmp(pType,"08")==0){
 			strcpy(caTname,"无线上网");
 		}
 		else if(strcmp(pType,"09")==0){
 			strcpy(caTname,"无线+有线");
 		}
 		else {
 			strcpy(caTname,"其它");
 		}
 		return &caTname[0];
 	}

char* ncsGetJoinCodeByName(char *pType){
 		static char caTname[32];
 		memset(caTname,0,sizeof(caTname));
 		if(strcmp(pType,"专网真实IP地址")==0||strcmp(pType,"01")==0){
 			strcpy(caTname,"01");
 		}
 		else if(strcmp(pType,"专线")==0||strcmp(pType,"02")==0){
 			strcpy(caTname,"02");
 		}
 		else if(strcmp(pType,"ADSL拨号")==0||strcmp(pType,"03")==0){
 			strcpy(caTname,"03");
 		}
 		else if(strcmp(pType,"ISDN")==0||strcmp(pType,"04")==0){
 			strcpy(caTname,"04");
 		}
 		else if(strcmp(pType,"普通拨号")==0||strcmp(pType,"05")==0){
 			strcpy(caTname,"05");
 		}
 		else if(strcmp(pType,"Cable Moderm 拨号")==0||strcmp(pType,"06")==0){
 			strcpy(caTname,"06");
 		}
 		else if(strcmp(pType,"电力线")==0||strcmp(pType,"07")==0){
 			strcpy(caTname,"07");
 		}
 		else if(strcmp(pType,"无线上网")==0||strcmp(pType,"08")==0){
 			strcpy(caTname,"08");
 		}
 		else if(strcmp(pType,"无线+有线")==0||strcmp(pType,"09")==0){
 			strcpy(caTname,"09");
 		}
 		else {
 			strcpy(caTname,"99");
 		}
 		return &caTname[0];
 	}

char* ncsGetIdnoNameByType(char *pType){
 		static char caTname[32];
 		memset(caTname,0,sizeof(caTname));
 		if(strcmp(pType,"11")==0||strcmp(pType,"111")==0){
 			strcpy(caTname,"身份证");
 		}
 		else if(strcmp(pType,"15")==0||strcmp(pType,"133")==0){
 			strcpy(caTname,"学生证");
 		}
 		else if(strcmp(pType,"90")==0||strcmp(pType,"114")==0){
 			strcpy(caTname,"军官证");
 		}
 		else if(strcmp(pType,"91")==0||strcmp(pType,"123")==0){
 			strcpy(caTname,"警官证");
 		}
 		else if(strcmp(pType,"92")==0){
 			strcpy(caTname,"士兵证");
 		}
 		else if(strcmp(pType,"93")==0){
 			strcpy(caTname,"户口本");
 		}
 		else if(strcmp(pType,"94")==0||strcmp(pType,"411")==0||strcmp(pType,"412")==0||strcmp(pType,"413")==0||strcmp(pType,"414")==0){
 			strcpy(caTname,"护照");
 		}
 		else if(strcmp(pType,"95")==0){
 			strcpy(caTname,"台胞证");
 		}
 		else if(strcmp(pType,"96")==0){
 			strcpy(caTname,"同乡证");
 		}
 		else {
 			strcpy(caTname,"其它");
 		}
 		return &caTname[0];
 	}

	 struct servicedata_s{
 		unsigned long userid;
 		char caServicecode[16];
 		int  onuid_count;   //在线用户数
 		int  endnum;      //终端用户数 		
 		char caCname[64];
 		long onstate;          //1 在线    2-离线 
 		char caIp[16];
 		char caDate[16];
 		char caEmail[64];
 		char caAddress[128];
 		char caHandphone[32];
 		char caTel[64];
 		char caContact[32];
 		char caVersion[16];
 		long lMonflag;
 		long lUseflags;	
 		char caGroupname[32];
 		char caDemo[256];
 		char caGtype[4];
 		unsigned long groupid;
 		long lLasttime;
 		char caAddtime[20];
 		char caFcode[16];
 		char servicestate[4];  //客户端状态 5-维护 2-勒令停业 3-暂停营业 4-拆机 5-维护 9-待审核    无线系统这个状态暂时拆分到别的地方
 		char caJointype[12];   //01 专网真实IP地址，02-专线 03-ADSL拨号 04-ISDN 05-普通拨号 06-Cable Moderm 拨号 07-电力线 -08-无线上网 09 无线+有线 99-其它
 		long lShflag;
 		long lWorkflag;
 		long lYyflag;
 		long lDataflag;
 	};

static iSort_corp;
static iSortby_corp;
 static int ncsUtlSortOnCorp(char *p1,char *p2)
{
    int i;
    long long l;   
    struct servicedata_s psC1,psC2;
    memcpy(&psC1,p1,sizeof(struct servicedata_s));
    memcpy(&psC2,p2,sizeof(struct servicedata_s));  
    if(iSort_corp==3){  
		if(iSortby_corp==1)
		    return psC2.onstate-psC1.onstate;
		 else
		    return psC1.onstate-psC2.onstate;    
		  }
		  else if(iSort_corp==1){
		  	if(iSortby_corp==1)
		  		return strcmp(psC2.caServicecode,psC1.caServicecode);
		  	else
		  	  return strcmp(psC1.caServicecode,psC2.caServicecode);
		  	}
		  	else if(iSort_corp==2){
		  	if(iSortby_corp==1)
		  		return strcmp(psC2.caCname,psC1.caCname);
		  	else
		  	  return strcmp(psC1.caCname,psC2.caCname);
		  	}
		  	else if(iSort_corp==4){
		  	if(iSortby_corp==1)
		  	  return (psC2.onuid_count-psC1.onuid_count);
		  	else
		  	  return (psC1.onuid_count-psC2.onuid_count);
		  	}
		  	else if(iSort_corp==6){
		  	if(iSortby_corp==1)
		  		return strcmp(psC2.caGroupname,psC1.caGroupname);
		  	else
		  	  return strcmp(psC1.caGroupname,psC2.caGroupname);
		  	}
		  	else if(iSort_corp==7){
		  	if(iSortby_corp==1)
		  		return strcmp(psC2.caVersion,psC1.caVersion);
		  	else
		  	  return strcmp(psC1.caVersion,psC2.caVersion);
		  	}
		  	else{

		  		return strcmp(psC2.caAddtime,psC1.caAddtime);
		  	
		  	}
		}

 
  int ncsWebReqSyscorp_v4(utShmHead *psShmHead,char *sql)
    {
    
            		if(strcasecmp(utComGetVar_sd(psShmHead,"syscorpinfo","No"),"Yes") == 0){
            			char caCenterip[24];
            			char caServerIp[24];
            			char *p;
            			strcpy(caCenterip,utComGetVar_sd(psShmHead,"CenterDataIp",""));
            			if(strlen(caCenterip)>0){
            				if(p=strstr(caCenterip,":")){
            					*p='\0';
            				}            			
            				utMsgHead *psMsgHead2;
            				pasLogs(PAS_SRCFILE,1000,"IP=%s,SQL=%s ",caCenterip,sql); 
            				
            				 unsigned long lIp = utComHostAddress(caCenterip);
                     int nPort=ntohs(7195);
            				psMsgHead2 = pasTcpRequest(lIp,nPort,
                         0,        /* CheckSum */
                         0,        /* Encrypt  */
                         "ncsNotifySyscorp",
                          0,
                          0,       /* Rid  */
                          30,      /* 超时  */
                          NULL,    /* key   */
                          1,
                          "passql",UT_TYPE_STRING,sql);
                     if(psMsgHead2){
                        utMsgFree(psMsgHead2);
                     }
            				
            			}
            		}

      return 0;
    }
 
//系统同步单位进程
  int ncsSyscorp_v4(utShmHead *psShmHead)
    {
    	  int iReturn;
    	  pasDbCursor *psCur;
    	  utMsgHead *psMsgHead2;
        char caTemp[2024];
        unsigned long lGroupid,lGtype,lModitime,lLastid;
        char caUsername[20],caDispname[64],caAddress[128],caTelphone[64],caContact[32],caHandphone[32];
        char caEmail[128],caCeoname[30],caPostcode[12],caServicestate[8],caJoincode[8],caEndnum[8];
        char caServernum[8],caJointype[8],caEmpnum[8],caArea[70],caAreaman[30],caAreatel[30],caDemo[256];
        char caIp[16],caVersion[16],caFcode[16];
                  char caCenterip[24];
            			char caServerIp[24];
            			char caCentport[12];
            			long lPort;
            			char *p;
            			strcpy(caCenterip,utComGetVar_sd(psShmHead,"CenterDataIp",""));
            			strcpy(caCentport,utComGetVar_sd(psShmHead,"CenterDataPort","7195"));
            			lPort=atol(caCentport);
            			if(strlen(caCenterip)>0){
            				if(p=strstr(caCenterip,":")){
            					*p='\0';
            				}  
            				 unsigned long lIp = utComHostAddress(caCenterip);
                     int nPort=ntohs(lPort);
        
        
		    iReturn = pasConnect(psShmHead);
		    if(iReturn < 0) {
		       printf("connect db error\n");
		        sleep(60);
		        return 0;
		    }
    
        while(1){
                

            				
            				 
            			 lLastid=0;
    	             lLastid=ncsUtlGetLastInfo("syscorptime");
            			 sprintf(caTemp,"select groupid,username,dispname,address,telphone,contact,handphone,email,ceoname,postcode,\
            			 servicestate,joincode,endnum,servernum,jointype,empnum,area,areaman,areatel,demo,ip,gtype,version,fcode,moditime from ncsuser where moditime>%d order by moditime ",lLastid);
            			 pasLogs(PAS_SRCFILE,1000,caTemp); 
            			  psCur = pasDbOpenSql(caTemp,0);
                   if(psCur == NULL) {
                   	  sleep(10);
                   	  return 0;
                   }
                    lLastid=0;
                    lGroupid=0;
                    memset(caUsername,0,sizeof(caUsername));
                    memset(caDispname,0,sizeof(caDispname));
                    memset(caAddress,0,sizeof(caAddress));
                    memset(caTelphone,0,sizeof(caTelphone));
                    memset(caContact,0,sizeof(caContact));
                    memset(caHandphone,0,sizeof(caHandphone));
                    memset(caEmail,0,sizeof(caEmail));
                    memset(caCeoname,0,sizeof(caCeoname));
                    memset(caPostcode,0,sizeof(caPostcode));
                    memset(caServicestate,0,sizeof(caServicestate));
                    memset(caJoincode,0,sizeof(caJoincode));
                    memset(caEndnum,0,sizeof(caEndnum));
                    memset(caArea,0,sizeof(caArea));
                    memset(caAreaman,0,sizeof(caAreaman));
                    memset(caAreatel,0,sizeof(caAreatel));
                    memset(caDemo,0,sizeof(caDemo));
                    memset(caIp,0,sizeof(caIp));
                    lGtype=0;
                    memset(caVersion,0,sizeof(caVersion));
                    memset(caFcode,0,sizeof(caFcode));
                    lModitime=0;
            				 iReturn = pasDbFetchInto(psCur,
            				                 UT_TYPE_ULONG,4,&lGroupid,
            				                 UT_TYPE_STRING,15,caUsername,
            				                 UT_TYPE_STRING,63,caDispname,
            				                 UT_TYPE_STRING,127,caAddress,
            				                 UT_TYPE_STRING,63,caTelphone,
            				                 UT_TYPE_STRING,31,caContact,
            				                 UT_TYPE_STRING,31,caHandphone,
            				                 UT_TYPE_STRING,127,caEmail,
            				                 UT_TYPE_STRING,29,caCeoname,
            				                 UT_TYPE_STRING,6,caPostcode,
            				                 UT_TYPE_STRING,1,caServicestate,
            				                 UT_TYPE_STRING,6,caJoincode,
            				                 UT_TYPE_STRING,6,caEndnum,
            				                 UT_TYPE_STRING,2,caServernum,
            				                 UT_TYPE_STRING,2,caJointype,
            				                 UT_TYPE_STRING,6,caEmpnum,
            				                 UT_TYPE_STRING,69,caArea,
            				                 UT_TYPE_STRING,29,caAreaman,
            				                 UT_TYPE_STRING,29,caAreatel,
            				                 UT_TYPE_STRING,254,caDemo,
            				                 UT_TYPE_STRING,15,caIp,
            				                 UT_TYPE_LONG,4,&lGtype,
            				                 UT_TYPE_STRING,15,caVersion,
            				                 UT_TYPE_STRING,9,caFcode,
            				                 UT_TYPE_LONG,4,&lModitime);
            				  while(iReturn==0||iReturn==1405){               
      	                    
      	                    
      	                psMsgHead2 = pasTcpRequest(lIp,nPort,
                         0,        /* CheckSum */
                         0,        /* Encrypt  */
                         "ncsSyscorp",
                          0,
                          0,       /* Rid  */
                          30,      /* 超时  */
                          NULL,    /* key   */
                          24,
                          "groupid", UT_TYPE_ULONG,lGroupid,
                          "username",UT_TYPE_STRING,caUsername,
                          "dispname",UT_TYPE_STRING,caDispname,
                          "address", UT_TYPE_STRING,caAddress,
                          "telphone",UT_TYPE_STRING,caTelphone,
                          "contact", UT_TYPE_STRING,caContact,
                          "handphone",UT_TYPE_STRING,caHandphone,
                          "email",    UT_TYPE_STRING,caEmail,
                          "ceoname",  UT_TYPE_STRING,caCeoname,
                          "postcode", UT_TYPE_STRING,caPostcode,
                          "servicestate",UT_TYPE_STRING,caServicestate,
                          "joincode",   UT_TYPE_STRING,caJoincode,
                          "endnum",     UT_TYPE_STRING,caEndnum,
                          "servernum",  UT_TYPE_STRING,caServernum,
                          "jointype",  UT_TYPE_STRING,caJointype,
                          "empnum",    UT_TYPE_STRING,caEmpnum,
                          "area",UT_TYPE_STRING,caArea,
                          "areaman",UT_TYPE_STRING,caAreaman,
                          "areatel",UT_TYPE_STRING,caAreatel,
                          "demo",  UT_TYPE_STRING,caDemo,
                          "ip",UT_TYPE_STRING,caIp,
                          "gtype",UT_TYPE_LONG,lGtype,
                          "version",UT_TYPE_STRING,caVersion,
                          "fcode",UT_TYPE_STRING,caFcode);
                          printf("caFcode=%s,gtype=%d\n",caFcode,lGtype);
                     if(psMsgHead2){
                        utMsgFree(psMsgHead2);
                     }  
      	                    
      	                    
      	                    
      	                    
      	                      
      	               if(lModitime>lLastid){
      	               	lLastid=lModitime;
      	               }    
      	                      
      	            lGroupid=0;
                    memset(caUsername,0,sizeof(caUsername));
                    memset(caDispname,0,sizeof(caDispname));
                    memset(caAddress,0,sizeof(caAddress));
                    memset(caTelphone,0,sizeof(caTelphone));
                    memset(caContact,0,sizeof(caContact));
                    memset(caHandphone,0,sizeof(caHandphone));
                    memset(caEmail,0,sizeof(caEmail));
                    memset(caCeoname,0,sizeof(caCeoname));
                    memset(caPostcode,0,sizeof(caPostcode));
                    memset(caServicestate,0,sizeof(caServicestate));
                    memset(caJoincode,0,sizeof(caJoincode));
                    memset(caEndnum,0,sizeof(caEndnum));
                    memset(caArea,0,sizeof(caArea));
                    memset(caAreaman,0,sizeof(caAreaman));
                    memset(caAreatel,0,sizeof(caAreatel));
                    memset(caDemo,0,sizeof(caDemo));
                    memset(caIp,0,sizeof(caIp));
                    lGtype=0;
                    memset(caVersion,0,sizeof(caVersion));
                    memset(caFcode,0,sizeof(caFcode));
                    lModitime=0;
            				 iReturn = pasDbFetchInto(psCur,
            				                 UT_TYPE_ULONG,4,&lGroupid,
            				                 UT_TYPE_STRING,15,caUsername,
            				                 UT_TYPE_STRING,63,caDispname,
            				                 UT_TYPE_STRING,127,caAddress,
            				                 UT_TYPE_STRING,63,caTelphone,
            				                 UT_TYPE_STRING,31,caContact,
            				                 UT_TYPE_STRING,31,caHandphone,
            				                 UT_TYPE_STRING,127,caEmail,
            				                 UT_TYPE_STRING,29,caCeoname,
            				                 UT_TYPE_STRING,6,caPostcode,
            				                 UT_TYPE_STRING,1,caServicestate,
            				                 UT_TYPE_STRING,6,caJoincode,
            				                 UT_TYPE_STRING,6,caEndnum,
            				                 UT_TYPE_STRING,2,caServernum,
            				                 UT_TYPE_STRING,2,caJointype,
            				                 UT_TYPE_STRING,6,caEmpnum,
            				                 UT_TYPE_STRING,69,caArea,
            				                 UT_TYPE_STRING,29,caAreaman,
            				                 UT_TYPE_STRING,29,caAreatel,
            				                 UT_TYPE_STRING,254,caDemo,
            				                 UT_TYPE_STRING,15,caIp,
            				                 UT_TYPE_LONG,4,&lGtype,
            				                 UT_TYPE_STRING,15,caVersion,
            				                 UT_TYPE_STRING,9,caFcode,
            				                 UT_TYPE_LONG,4,&lModitime);   
      	                           
            				   }
            				   pasDbCloseCursor(psCur);  
            				   
            				   if(lLastid>0){
            				   	 ncsUtlSetLastInfo("syscorptime",lLastid);  
            				   	 
            				   }
            				   sleep(60);  			
            				
            				
            				
            				
            			
            				
            			}
            }
      sleep(60);
      return 0;
    }





//显示单位信息
int ncsWebClientSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512],caCond[64],caCond0[64];
    char caTotPg[16],caCurPg[16],caSort[16],caSortby[8];
    char caUsername[32],caGroupname[32],caMac[32],caIp[32],caDate[32];
    char caGroupid[16],caGroupid2[16];
    char caDispname[64],caAddress[128],caTel[64],caHandphone[32],caContact[32];
    char caTemp[2024],name[64];
    char caVar[16];
    long lSid;
    char *p;
    long lCount_t,js_flag;
    unsigned long lSumuser;
    unsigned long lCount_u;
    char caAll[1024];
    long index;
    ncsOnline  *psOnline;
    ncsClient * psClient;
    char caEmail[128];
    char *pSelect,*pBuf;
    unsigned long lUserid,lGroupid,lCurPg,lTotPg;
    int lCount;
    int i,iNum,lId;
    char *pSear;
    struct servicedata_s *pAreastatdata;     
	  pasLHashInfo sHashInfo; 
	  unsigned char *pHash;
	  char caDir[20],caDid[16];
  
    unsigned char *pHash_fac;
    struct fac_s {
    	char caCod[12];
    	char caName[64];
    };
    struct fac_s *psFac;
    char caSmt[4048],caOrder[64];
    char caDel[16],caExport[16];
    utPltDbHead *psDbHead;
   	int iReturn;
   	long lGroupid0,lRowNum,lUseflags,lStartRec,lUseflags0,lTotRec;
   	char servicecode1[24],dispname1[68],address1[130],ceoname1[32],handphone1[34],contact1[34],telphone1[68],email1[130];
   	char ip1[20],servicestate1[4],jointype1[4],area1[72],areaman1[32],joincode1[8];
 	  pasDbCursor *psCur;
 	  char caUsername2[32];
 	  char caMsg[1024];
 	  char caOnlineflag[16];
 	  long lMonflag=0;
 	  unsigned long lCurtime,lTime,lRuntime;
   	unsigned long lCase;
   	char caYear[30];
   	char caPlate[128];
   	char caOpt[256];
   	char caVersion[20];
   	char caPage[16],caLimit[16];
   	char caDemo[256];
   	char caDemo1[256];
    long lGtype=0;
    char caCode[16],caName[64];
    char caFcod_c[16],caGtype[16],caTcode[16];
    long lSumcorp;
    char caKeyword[36];
    char caWireflag[16];
    char caUpdate[16],caUserid[16],caTelphone[24],caGroupid1[16];
    struct servicedata_s *pCorp;
    FILE *fp;
    char pFile[256],caBuf[128];
    char caLxstate[16];
    long lLxstate;
     char caTemp_d[256];
    char caSesid[1024];
    char caMove[16],caMid[16];
    char caOut[200][256];
    char caFcode[16],caFname[68];
    char caSdate[20],caAddtime[20];
    char caGroups[1024];
    char caFacs[1024];
    char caServicestate[16];
    char caLastln[20];
    char caClastime[24];
    char caRqlx[16];
 
    char caJointype[20],caJointype_t[20];
    	 char caFlag[16],caSet[16],caSid[16];
    	 char caJrcs[68],caDwlb[68];
    	 char *pHash_lb;
        //获取权限组ID和接入厂商
			struct stUserAtt
			{
			        unsigned long did;
			        long lSh_flag;
			        long lLasttime;
			};
			unsigned char *psUserAtt;
			
    strcpy(caFacs,getDsFacGroupid());
    strcpy(caGroups,getDsGroupid());

    memset(caAll,0,sizeof(caAll));
   
    
      psUserAtt = (unsigned char *)pasHashInit(2000,2000,sizeof(struct stUserAtt), 0, 4);
        if(psUserAtt == NULL)
        {
        	return 0;
        }
       struct stUserAtt *data = NULL;

        psCur = pasDbOpenSql("select userid, tw_shflag from ncsuser_att ", 0);
        if(psCur != NULL)
        {
                lUserid = 0;
                long lSh_flag=0;
                int iRet = pasDbFetchInto(psCur,                                                                                                               
                                                UT_TYPE_ULONG, 4, &lUserid,   
                                                UT_TYPE_LONG,4,&lSh_flag);                                                                                  
                                               
                while((iRet == 0) || (iRet == 1405))                                                                                                           
                {                                                                                                                                              
                                                                                                                                                               
                                                                                                                                              
                                data = (struct stUserAtt *)pasHashLookA(psUserAtt, &lUserid);                                                                    
                                if(data != NULL)                                                                                                     
                                {                                                                                                                              
                                    data->lSh_flag=lSh_flag;                                                                            
                                }                                                                                                                              
                                                                                                                                                            
                                                                                                                                                          
                                                                                                                                               
                 lUserid = 0;
                 lSh_flag=0;
                iRet = pasDbFetchInto(psCur,                                                                                                               
                                                UT_TYPE_ULONG, 4, &lUserid,   
                                                UT_TYPE_LONG,4,&lSh_flag);                                                                  
                }                                                                                                                                              
                pasDbCloseCursor(psCur);  
    }
    
    
    
 //   printf("caAll=%s\n",caAll);
   
    pHash_fac = (unsigned char *)pasLHashInit(30,30,sizeof(struct fac_s),0,12);    
    if(pHash_fac == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
       //将接入厂家装入到内存   
     sprintf(caTemp,"select code,name from ncsfactorycod limit 0,29 "); 
    psCur = pasDbOpenSql(caTemp,0);

    if(psCur){
    memset(caCode,0,sizeof(caCode));
    memset(caName,0,sizeof(caName));
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_STRING,10,caCode,
      	                             UT_TYPE_STRING,63,caName);
      	                            
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
   		 psFac = (struct fac_s *)pasLHashLookA(pHash_fac,caCode);  
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
   
   
   
    
    pHash = (unsigned char *)pasLHashInit(5000,5000,sizeof(struct servicedata_s),0,4);    
    if(pHash == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
    
    
    memset(caOpt,0,sizeof(caOpt)); 
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
   	
   	
 	  lSumuser=0;
 	  dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
	  
	  iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    js_flag=0;

    	
	  
	  
 utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,46,
	                 "TotPg",            UT_TYPE_STRING, 12, caTotPg,
			             "CurPg",            UT_TYPE_STRING, 12, caCurPg,
			             "groupid",          UT_TYPE_STRING, 12, caGroupid,  	              
   	                "sort",            UT_TYPE_STRING, 15, caSort,
   	                "dir",          UT_TYPE_STRING, 8,  caDir,   	              
   	                "del",             UT_TYPE_STRING, 10,caDel,
   	                "export2",          UT_TYPE_STRING,10,caExport,
   	                "name",            UT_TYPE_STRING,63,name,
   	                "servicecode1",    UT_TYPE_STRING,16,servicecode1,
   	                "dispname1",       UT_TYPE_STRING,64,dispname1,
   	                "address1",        UT_TYPE_STRING,128,address1,
   	                "ceoname1",        UT_TYPE_STRING,30,ceoname1,
   	                "handphone1",      UT_TYPE_STRING,32,handphone1,   	                
   	                "telphone1",       UT_TYPE_STRING,64,telphone1,
   	                "email1",          UT_TYPE_STRING,128,email1,
   	                "ip1",             UT_TYPE_STRING,16,ip1,
   	                "joincode1",       UT_TYPE_STRING,7,joincode1,
   	                "servicestate1",    UT_TYPE_STRING,2,servicestate1,
   	                "jointype1",       UT_TYPE_STRING,3,jointype1,
   	                "area1",           UT_TYPE_STRING,60,area1,
   	                "contact1",         UT_TYPE_STRING,30,contact1,
   	                "areaman1",        UT_TYPE_STRING,29,areaman1,
   	                "onlineflag",      UT_TYPE_STRING,2,caOnlineflag,
   	                "plate",           UT_TYPE_STRING,120,caPlate,
   	                "page",            UT_TYPE_STRING,10,caPage,
   	                "limit",           UT_TYPE_STRING,10,caLimit,
   	                "fcode",           UT_TYPE_STRING,10,caFcod_c,
   	                "lb",           UT_TYPE_STRING,10,caGtype,
   	                "lxstate",           UT_TYPE_STRING,10,caLxstate,
   	                "fcode",           UT_TYPE_STRING,10,caFcode,
   	                 "selsid",UT_TYPE_STRING,1000,caSesid,
   	                "did",   UT_TYPE_STRING,10,caDid,
   	                "keyword",UT_TYPE_STRING,30,caKeyword,
   	                "move",UT_TYPE_STRING,10,caMove,
   	                "mid",UT_TYPE_STRING,10,caMid,
   	                "update",UT_TYPE_STRING,10,caUpdate,			         
   	                "sid",UT_TYPE_STRING,10,caSid,
			            "set",UT_TYPE_STRING,10,caSet,
			            "flag",UT_TYPE_STRING,10,caFlag,
			            "lastln",UT_TYPE_STRING,10,caLastln,
			            "jrcs",  UT_TYPE_STRING,60,caJrcs,
			            "dwlb",  UT_TYPE_STRING,60,caDwlb,
			            "rqlx",  UT_TYPE_STRING,3,caRqlx,
			            "demo1", UT_TYPE_STRING,100,caDemo1,
			            "wireflag",UT_TYPE_STRING,8,caWireflag,
			            "jointype",UT_TYPE_STRING,19,caJointype_t);
	char caJointype_c[20];
	   memset(caJointype_c,0,sizeof(caJointype));            
	    if(strlen(caJointype_t)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caJointype_t,caTemp,64));
    	strcpy(caJointype_t,caTemp_d);
    	strcpy(caJointype_c,ncsGetJoinCodeByName(caJointype_t));
 printf("caJointype_c=%s\n",caJointype_c);   	
    }		            
			            
			            
	if(strcmp(caGtype,"undefined")==0) strcpy(caGtype,"");
	if(strcmp(caGroupid,"undefined")==0) strcpy(caGroupid,"");
	if(strcmp(caFcode,"undefined")==0) strcpy(caFcode,"");
		
   	if(strcmp(caSet,"set")==0&&strlen(caSid)>0){
	  	sprintf(caTemp,"update ncsuser set servicestate='%s' where userid=%s",caFlag,caSid);
	  	printf("caTemp=%s\n",caTemp);
	  	pasDbExecSql(caTemp,0);
	  }
   
      if(strlen(caJrcs)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caJrcs,caTemp,64));
    	strcpy(caJrcs,caTemp_d);
    	
    	sprintf(caTemp,"select code from ncsfactorycod where name='%s' ",caJrcs);
    	memset(caCode,0,sizeof(caCode));
    	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,10,caCode);
    	if(strlen(caCode)>0){
    		strcpy(caJrcs,caCode);
    	}
    }
     if(strlen(caDwlb)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caDwlb,caTemp,64));
    	strcpy(caDwlb,caTemp_d);
    	sprintf(caTemp,"select code from ncsuser_lb where name='%s' ",caDwlb);

    	memset(caCode,0,sizeof(caCode));
    	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,10,caCode);

    	if(strlen(caCode)>0){
    		strcpy(caDwlb,caCode);
    	}
    	
    	
    }
   
   	                
   if(strlen(dispname1)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,dispname1,caTemp,64));
    	strcpy(dispname1,caTemp_d);
    }
    if(strlen(area1)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,area1,caTemp,64));
    	strcpy(area1,caTemp_d);
    }
    if(strlen(address1)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,address1,caTemp,127));
    	strcpy(address1,caTemp_d);
    }                
   	if(strlen(caKeyword)>0){
   		 iReturn=pasCharIsUtf8(caKeyword, strlen(caKeyword));
   		 if(iReturn){
   		   strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,30));
   		   strcpy(caKeyword,caTemp_d);
   		  }
   		
   	}               
   	                
   if(strcmp(caDir,"ASC")==0){
   	iSortby_corp=1;
  }
  else{
  	iSortby_corp=0;
  }
  if(strcmp(caSort,"dispname")==0){
  		 iSort_corp=2;
  }
 else if(strcmp(caSort,"username")==0){
  			iSort_corp=1;
  }
  else if(strcmp(caSort,"status")==0){
  	iSort_corp=3;
  }
  else if(strcmp(caSort,"onuser")==0){
  	iSort_corp=4;
  }
  else if(strcmp(caSort,"groupname")==0){
  	iSort_corp=6;
  }
  else if(strcmp(caSort,"version")==0){
  	iSort_corp=7;
  }
  else{
  	iSort_corp=5;
  }
  
  //修改或增加
  if(strcmp(caUpdate,"update")==0){
  	   	iReturn = utMsgGetSomeNVar(psMsgHead,7,
   	      "userid",      UT_TYPE_STRING, 10, caUserid,
					"username",    UT_TYPE_STRING, 30, caUsername,
				  "groupid1",     UT_TYPE_STRING,10,caGroupid1,
					"dispname",    UT_TYPE_STRING, 63, caDispname,
					"address",     UT_TYPE_STRING, 127, caAddress,
					"telphone",    UT_TYPE_STRING, 15, caTelphone,
				  "contact",     UT_TYPE_STRING, 15, caContact);
				  		//编码转换
					if(strlen(caUsername)>0){
						 pasCvtGBK(2,caUsername,caTemp,30);
						 strcpy(caUsername,caTemp);
					}
					if(strlen(caDispname)>0){
						 pasCvtGBK(2,caDispname,caTemp,63);
						 strcpy(caDispname,caTemp);
					}
					if(strlen(caAddress)>0){
						 pasCvtGBK(2,caAddress,caTemp,63);
						 strcpy(caAddress,caTemp);
					}
					if(strlen(caTelphone)>0){
						 pasCvtGBK(2,caTelphone,caTemp,63);
						 strcpy(caTelphone,caTemp);
					}	
					if(strlen(caContact)>0){
						 pasCvtGBK(2,caContact,caTemp,63);
						 strcpy(caContact,caTemp);
					}	
					if(atol(caUserid)>0){
						lUserid = atol(caUserid);
						sprintf(caTemp,"update ncsuser set username='%s',groupid=%s,dispname='%s',address='%s',contact='%s',telphone='%s',moditime=%lu,fcode='%s',gtype='%s',jointype='%s' where userid=%s ",
						caUsername,caGroupid1,caDispname,caAddress,caContact,caTelphone,time(0),caJrcs,caDwlb,caJointype_c,caUserid);
						printf("caTemp=%s\n",caTemp);
						iReturn = pasDbExecSqlF(caTemp);
						            		//和数据中心同步单位
						    if(iReturn==0){
            		 ncsWebReqSyscorp_v4(psShmHead,caTemp); 
            	  }
						
						sprintf(caTemp,"修改上网场所信息，代码：%s,显示名：%s",caUsername,caDispname);    
		        ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caTemp);  
					}
					else{
						   lUserid=utMd5Code(caUsername,strlen(caUsername),"ffff");
				       if(lUserid>2000000000) lUserid=lUserid-2000000000;       
				       if(lUserid>2000000000) lUserid=lUserid-2000000000;
				        sprintf(caTemp,"select count(*) from ncsuser where userid=%lu ",lUserid);
				        lCount=0;
				       pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
				       if(lCount>0){
				       	lUserid=lUserid+10;
				      }
				       
						 strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M",time(0)));
						 
							 sprintf(caTemp,"insert into ncsuser (userid,username,groupid,dispname,address,telphone,contact,handphone,email,ip,addtime,lasttime,\
		               servicestate,joincode,endnum,servernum,empnum,area,areaman,areatel,demo,postcode,ceoname,moditime,monflag,password,fcode,gtype,jointype) values \
                                                 (%lu,'%s', %lu,'%s','%s','%s','%s','%s','%s','%s','%s','%s',\
                                                 '%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%lu,%lu,'','%s','%s','%s')",\
                                                         lUserid,caUsername,strtoul(caGroupid1,NULL,10),caDispname,caAddress,caTelphone,caContact,"","","",caSdate,caSdate,
														 "","","","","",
		 												 "","","","","","",time(0),0,caJrcs,caDwlb,caJointype_c);
	 					 printf("sqlbuf=%s\n",caTemp);	
	 					 							 
             iReturn = pasDbExecSqlF(caTemp);
           		 if(iReturn==0){
            		 ncsWebReqSyscorp_v4(psShmHead,caTemp); 
            	  }
             
             
             sprintf(caTemp,"添加上网场所 场所编码%s，名称：%s",caUsername,caDispname);    
             ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caTemp);
						
					}
					if(iReturn==0){
						  psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lUserid);
			        if(psClient) {
			            strcpy(psClient->username,caUsername);
			            strcpy(psClient->dispname,caDispname);
			            psClient->groupid=strtoul(caGroupid1,NULL,10);
			        }
					}
					
					
  }
  
  
  
  
   if(!utStrIsSpaces(caDel)) {  /* 删除选中的项目*/
    	if(strlen(caSesid)>0){
    		      iReturn = pasDbExecSqlF("delete from ncsuser where userid in(%s)",caSesid);
            	sprintf(caMsg,"delete from ncsuser where userid in(%s) ",caSesid);
            	if(iReturn==0){
            		//和数据中心同步单位
            		ncsWebReqSyscorp_v4(psShmHead,caMsg);           		
            		ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caMsg);
            		
            		
            	}
            	else{
            		ncsWriteSysLog(psMsgHead,caUsername2,"05","失败",caMsg);
            	}
    	}
		}
		if(strcmp(caMove,"move")==0){
			if(strlen(caSesid)>0&&strlen(caMid)>0){
				 iReturn = pasDbExecSqlF("update ncsuser set groupid=%s where userid in(%s)",caMid,caSesid);
				 lCount=sepcharbydh(caSesid,caOut);
				 for(i=0;i<lCount;i++){
			//	 	printf("caOut=%s\n",caOut[i]);
				 	       psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,atol(caOut[i]));
				        if(psClient) {
				            psClient->groupid=atol(caMid);
				           
				        }
				 }
			}
		}

    if(strlen(caPage)>0){
    	strcpy(caCurPg,caPage);
    }
    lCurPg = atol(caCurPg);//当前页面
    if(lCurPg <= 0) lCurPg = 1 ;//如若当前页面小于0，置1
    
     	if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
  
    lStartRec = (lCurPg - 1) * lRowNum;   //当前页面的开始第一行的行数	

       
   sprintf(caSmt,"select useflags,userid,dispname,username,ip,lasttime,email,address,telphone,handphone,contact,monflag,version,demo,b.groupname,gtype,a.groupid,a.addtime,a.fcode,a.servicestate,a.jointype from ncsuser a left join ncsgroup b on ( a.groupid=b.groupid) where 1=1 ");

      if(strlen(servicecode1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and username like '%c%s%c' ",'%',servicecode1,'%');
    }
    if(strlen(dispname1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and dispname like '%c%s%c' ",'%',dispname1,'%');
    }
    if(strlen(address1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and address like '%c%s%c' ",'%',address1,'%');
    }
    if(strlen(ceoname1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and ceoname like '%c%s%c' ",'%',ceoname1,'%');
    }
    if(strlen(handphone1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and handphone like '%c%s%c' ",'%',handphone1,'%');
    }
    if(strlen(contact1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and contact like '%c%s%c' ",'%',contact1,'%');
    }
    if(strlen(telphone1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and telphone like '%c%s%c' ",'%',telphone1,'%');
    }
    if(strlen(email1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and email like '%c%s%c' ",'%',email1,'%');
    }
    if(strlen(ip1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and ip like '%c%s%c' ",'%',ip1,'%');
    }
    
    if(strlen(joincode1)!=0){
     sprintf(caSmt+strlen(caSmt),"  and joincode like '%c%s%c' ",'%',joincode1,'%');
    }
    if(strlen(servicestate1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and servicestate= '%s' ",servicestate1);
    }
    if(strlen(jointype1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and jointype= '%s' ",jointype1);
    }
    if(strlen(area1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and area like '%c%s%c' ",'%',area1,'%');
    }
    if(strlen(areaman1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and areaman like '%c%s%c' ",'%',areaman1,'%');
    }
    if(atol(caGroupid)==99999999){
    	sprintf(caSmt+strlen(caSmt)," and a.groupid not in (select groupid from ncsgroup ) ");
    }
    else if(strlen(caGroupid)>0){
    	sprintf(caSmt+strlen(caSmt)," and a.groupid in (%s) ",getGroupid(atol(caGroupid)));
//    	sprintf(caSmt+strlen(caSmt)," and (a.groupid=%s or a.groupid in (select groupid from ncsgroup where pid=%s)) ",caGroupid,caGroupid);
    }
//权限
     if(strlen(caGroups)>0){
    	sprintf(caSmt+strlen(caSmt)," and a.groupid in (%s) ",caGroups);
    }
    if(strlen(caFacs)>0){
    	sprintf(caSmt+strlen(caSmt)," and a.fcode in (%s) ",caFacs);
    }
    if(strlen(caWireflag)>0){
    	if(strcmp(caWireflag,"2")==0){
    	   sprintf(caSmt+strlen(caSmt)," and (a.jointype='08' or a.jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
      	sprintf(caSmt+strlen(caSmt)," and a.jointype!='08' ");
      }
    
    }
    if(strlen(caRqlx)>0){
    	sprintf(caSmt+strlen(caSmt)," and gtype in (%s) ",getRqlxId(caRqlx));
    }
    
     if(strlen(caDemo1)>0){
    	sprintf(caSmt+strlen(caSmt)," and demo like '%c%s%c' ",'%',caDemo1,'%');
    }
    
    
    if(strlen(caGtype)>0){
    		if(atol(caGtype)!=9){
    			 if(atol(caGtype)==200){
    			 	sprintf(caSmt+strlen(caSmt)," and gtype in ('1','2','8','11','7','4')");
    			}
    			else if(atol(caGtype)==300){
    				sprintf(caSmt+strlen(caSmt)," and gtype in ('5','6','10','3')");
    			}
    			else{
    		   sprintf(caSmt+strlen(caSmt)," and gtype='%s' ",caGtype);
    		  }
    	}
    	else{
    		sprintf(caSmt+strlen(caSmt)," and gtype not in ('1','2','3','4','5','6','7','8','10','11') ");
    	}
//   		sprintf(caSmt+strlen(caSmt)," and gtype=%s ",caGtype);
  	}
 // 	if(strlen(caFcod_c)>0){
 // 		sprintf(caSmt+strlen(caSmt)," and substring(username,9,2)='%s' ",caFcod_c);
  //	}
    if(strlen(caDid)>0){
    	sprintf(caSmt+strlen(caSmt)," and userid=%s ",caDid);
    }
    
    printf("caKeyword=%s\n",caKeyword);
    if(strlen(caKeyword)>0){
    	sprintf(caSmt+strlen(caSmt)," and (dispname like '%c%s%c' or username like '%c%s%c' or demo like '%c%s%c')",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    }
    if(strlen(caFcode)>0){
    	sprintf(caSmt+strlen(caSmt)," and fcode='%s' ",caFcode);
    }
 /*   
    if(strlen(caSort)>0&&strcmp(caSort,"state")!=0){
    	sprintf(caSmt+strlen(caSmt),"  order by %s ",caSort);
    }
    else{
    sprintf(caSmt+strlen(caSmt),"  order by dispname ");
    }
    if(strcmp(caSortby,"desc")==0){
    	sprintf(caSmt+strlen(caSmt),"  desc ");
    }
 */
  printf("caSmt=%s\n",caSmt);
     lLxstate=atol(caLxstate);
    psCur = pasDbOpenSql(caSmt,0);
    if(psCur==NULL) {  	  
    	
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    	}
       memset(caDispname,0,sizeof(caDispname));
       memset(caUsername,0,sizeof(caUsername));
       memset(caIp,0,sizeof(caIp));
       memset(caDate,0,sizeof(caDate));
       memset(caEmail,0,sizeof(caEmail));
       memset(caAddress,0,sizeof(caAddress));
       memset(caTel,0,sizeof(caTel));
       memset(caHandphone,0,sizeof(caHandphone));
       memset(caContact,0,sizeof(caContact));
       memset(caGtype,0,sizeof(caGtype));
       lUseflags=0;
       lMonflag=0;
       memset(caVersion,0,sizeof(caVersion));
       memset(caDemo,0,sizeof(caDemo));
       memset(caGroupname,0,sizeof(caGroupname));
       lGtype=0;
       lGroupid=0;
       memset(caAddtime,0,sizeof(caAddtime));
       memset(caFcode,0,sizeof(caFcode));
       memset(caServicestate,0,sizeof(caServicestate));
       memset(caJointype,0,sizeof(caJointype));
   	   iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lUseflags,
  	                               UT_TYPE_ULONG,4,&lUserid,
  	                               UT_TYPE_STRING,63,caDispname,
  	                               UT_TYPE_STRING,15,caUsername,
  	                               UT_TYPE_STRING,15,caIp,
  	                               UT_TYPE_STRING,15,caDate,
  	                               UT_TYPE_STRING,63,caEmail,
  	                               UT_TYPE_STRING,127,caAddress,
  	                               UT_TYPE_STRING,63,caTel,
  	                               UT_TYPE_STRING,31,caHandphone,
  	                               UT_TYPE_STRING,31,caContact,
  	                               UT_TYPE_LONG,4,&lMonflag,
  	                               UT_TYPE_STRING,15,caVersion,
  	                               UT_TYPE_STRING,250,caDemo,
  	                               UT_TYPE_STRING,31,caGroupname,
  	                               UT_TYPE_STRING,4,caGtype,
  	                               UT_TYPE_ULONG,4,&lGroupid,
  	                               UT_TYPE_STRING,19,caAddtime,
  	                               UT_TYPE_STRING,10,caFcode,
  	                               UT_TYPE_STRING,2,caServicestate,
  	                               UT_TYPE_STRING,4,caJointype);
  	    lSumcorp=0;
        while((iReturn == 0)||(iReturn==1405)) { 

        pAreastatdata = (struct servicedata_s *)pasLHashLookA(pHash,&lUserid);  
       if(pAreastatdata){
       	lSumcorp++;

    			 strcpy(pAreastatdata->caServicecode,caUsername);
    			 strcpy(pAreastatdata->caCname,caDispname);
    			 strcpy(pAreastatdata->caIp,caIp);
    			 strcpy(pAreastatdata->caDate,caDate);
    			 strcpy(pAreastatdata->caEmail,caEmail);
    			 strcpy(pAreastatdata->caAddress,caAddress);
    			 strcpy(pAreastatdata->caHandphone,caHandphone);
    			 strcpy(pAreastatdata->caTel,caTel);
    			 strcpy(pAreastatdata->caContact,caContact);
    			 strcpy(pAreastatdata->caVersion,caVersion);
    			 strcpy(pAreastatdata->caDemo,caDemo);
    			 strcpy(pAreastatdata->caGroupname,caGroupname);
    			 strcpy(pAreastatdata->caJointype,caJointype);
    			 pAreastatdata->lMonflag=lMonflag;
    			 pAreastatdata->lUseflags=lUseflags;
    			 strcpy(pAreastatdata->caGtype,caGtype);
    			 pAreastatdata->groupid=lGroupid;
    			 strcpy(pAreastatdata->caAddtime,caAddtime);
    			 strcpy(pAreastatdata->caFcode,caFcode);
    			 strcpy(pAreastatdata->servicestate,caServicestate);
          
      psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lUserid);          
      psOnline =  (ncsOnline *)ncsUtlGetOnlineById(psShmHead,lUserid);
       if(psOnline){
       	 strcpy(pAreastatdata->caIp,utComHostIp(htonl(psOnline->lSip)));
       	pAreastatdata->onuid_count=psOnline->lUser;
       	pAreastatdata->lLasttime=psOnline->lLastTime;
       lSumuser=lSumuser+psOnline->lUser;
      }
      else{
      	pAreastatdata->lLasttime=0;
      }
      
      data = (struct stUserAtt *)pasHashLook(psUserAtt, &lUserid);                                                                    
	     if(data != NULL)                                                                                                     
	     {                                                                                                                              
	              if(data->lSh_flag==0){
	    //          	printf("lLxstate=%d\n",lLxstate);
	              	     if(lLxstate==0||lLxstate==9){
	              	        pAreastatdata->onstate=9;
	              	      }
	              }                                                                       
	     }    
 //printf("onstate=%d\n",pAreastatdata->onstate);     
     if(pAreastatdata->onstate!=9){
        if(psClient) {
        	if(psClient->status==1){       		
        		if(psOnline){
        			pAreastatdata->onstate=1;
        		}
        		else{
        			if(strcmp(pAreastatdata->servicestate,"5")==0){
        				pAreastatdata->onstate=5;
        			}
        			else if(strcmp(pAreastatdata->servicestate,"2")==0||strcmp(pAreastatdata->servicestate,"3")==0||strcmp(pAreastatdata->servicestate,"4")==0){
        				pAreastatdata->onstate=4;
        			}
        			else{
        			 pAreastatdata->onstate=2;
        			}
        		}
        	}
        	else if(psClient->status==3){
        		pAreastatdata->onstate=1;
        	}
        	else if(psClient->status==2){
        		  if(strcmp(pAreastatdata->servicestate,"5")==0){
        				pAreastatdata->onstate=5;
        			}
        			else if(strcmp(pAreastatdata->servicestate,"2")==0||strcmp(pAreastatdata->servicestate,"3")==0||strcmp(pAreastatdata->servicestate,"4")==0){
        				pAreastatdata->onstate=4;
        			}
        			else{
        			 pAreastatdata->onstate=2;
        			}
        	}
        
        }
       }
       
       
      }
        	
        	
        memset(caDispname,0,sizeof(caDispname));
       memset(caUsername,0,sizeof(caUsername));
       memset(caIp,0,sizeof(caIp));
       memset(caDate,0,sizeof(caDate));
       memset(caEmail,0,sizeof(caEmail));
       memset(caAddress,0,sizeof(caAddress));
       memset(caTel,0,sizeof(caTel));
       memset(caHandphone,0,sizeof(caHandphone));
       memset(caContact,0,sizeof(caContact));
       lUseflags=0;
       lMonflag=0;
       memset(caVersion,0,sizeof(caVersion));
       memset(caDemo,0,sizeof(caDemo));
       memset(caGroupname,0,sizeof(caGroupname));
       lGtype=0;
       lGroupid=0;
       memset(caAddtime,0,sizeof(caAddtime));
       memset(caServicestate,0,sizeof(caServicestate));
   	   iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lUseflags,
  	                               UT_TYPE_ULONG,4,&lUserid,
  	                               UT_TYPE_STRING,63,caDispname,
  	                               UT_TYPE_STRING,15,caUsername,
  	                               UT_TYPE_STRING,15,caIp,
  	                               UT_TYPE_STRING,15,caDate,
  	                               UT_TYPE_STRING,63,caEmail,
  	                               UT_TYPE_STRING,127,caAddress,
  	                               UT_TYPE_STRING,63,caTel,
  	                               UT_TYPE_STRING,31,caHandphone,
  	                               UT_TYPE_STRING,31,caContact,
  	                               UT_TYPE_LONG,4,&lMonflag,
  	                               UT_TYPE_STRING,15,caVersion,
  	                               UT_TYPE_STRING,250,caDemo,
  	                               UT_TYPE_STRING,31,caGroupname,
  	                               UT_TYPE_STRING,2,caGtype,
  	                               UT_TYPE_ULONG,4,&lGroupid,
  	                               UT_TYPE_STRING,19,caAddtime,
  	                               UT_TYPE_STRING,10,caFcode,
  	                               UT_TYPE_STRING,2,caServicestate);
            }
        
        
        pasDbCloseCursor(psCur);
    
        //各单位用户数   
    sprintf(caTemp,"select count(b.did ),a.userid from ncsuser a left join ncsenduser b on ( a.userid=b.did) where 1=1 ");
    if(!strstr(caOpt,"all")&&(strlen(caOpt)>0)){
			sprintf(caTemp+strlen(caTemp)," and a.groupid in (%s) ",caOpt);
   }
    sprintf(caTemp+strlen(caTemp)," group by a.userid order by a.userid ");  
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
    	if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
      
		  lCount_u=0;
		  lUserid=0;
  	  i=0;
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_u,
      	                             UT_TYPE_ULONG,4,&lUserid);
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
   		 pAreastatdata = (struct servicedata_s *)pasLHashLook(pHash,&lUserid);  
       if(pAreastatdata){
    			pAreastatdata->endnum=lCount_u;   	
//    			printf("endnu=%d,lUserid=%d\n",lCount_u,lUserid);
       }	
		  lCount_u=0;
		  lUserid=0;
		  i++;
  	  iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_u,
      	                             UT_TYPE_ULONG,4,&lUserid);	                              
    }    
    pasDbCloseCursor(psCur);  
    
    
    
    
    
       
 //  printf("lSumcorp=%d\n",lSumcorp);    
     pCorp= (struct servicedata_s*)malloc((lSumcorp+1)*sizeof(struct servicedata_s)); 
     if(pCorp==NULL){
     	  utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;
    }
    memset(pCorp,0,(lSumcorp+1)*sizeof(struct servicedata_s)); 
    
       
    pAreastatdata  = (struct servicedata_s *)pasLHashFirst(pHash,&sHashInfo);
    iNum = 0;
    lLxstate=atol(caLxstate);

    while(pAreastatdata) {
         if(strlen(caLxstate)==0||pAreastatdata->onstate==lLxstate){
         pCorp[iNum].userid=pAreastatdata->userid;
         strcpy(pCorp[iNum].caServicecode,pAreastatdata->caServicecode);
         strcpy(pCorp[iNum].caCname,pAreastatdata->caCname);
         strcpy(pCorp[iNum].caIp,pAreastatdata->caIp);
         strcpy(pCorp[iNum].caDate,pAreastatdata->caDate);
         strcpy(pCorp[iNum].caEmail,pAreastatdata->caEmail);
         strcpy(pCorp[iNum].caAddress,pAreastatdata->caAddress);
         strcpy(pCorp[iNum].caHandphone,pAreastatdata->caHandphone);
         strcpy(pCorp[iNum].caTel,pAreastatdata->caTel);
    
         strcpy(pCorp[iNum].caContact,pAreastatdata->caContact);
         strcpy(pCorp[iNum].caVersion,pAreastatdata->caVersion);
         strcpy(pCorp[iNum].caDemo,pAreastatdata->caDemo);
         strcpy(pCorp[iNum].caGroupname,pAreastatdata->caGroupname);
         strcpy(pCorp[iNum].caJointype,pAreastatdata->caJointype);
         pCorp[iNum].lMonflag=pAreastatdata->lMonflag;
         pCorp[iNum].lUseflags=pAreastatdata->lUseflags;
         pCorp[iNum].onuid_count=pAreastatdata->onuid_count;
         pCorp[iNum].onstate=pAreastatdata->onstate;
   //      printf("state=%d\n",pAreastatdata->onstate);
         strcpy(pCorp[iNum].caGtype,pAreastatdata->caGtype);
         pCorp[iNum].endnum=pAreastatdata->endnum;
         pCorp[iNum].groupid=pAreastatdata->groupid;
         strcpy(pCorp[iNum].caAddtime,pAreastatdata->caAddtime);
         strcpy(pCorp[iNum].caFcode,pAreastatdata->caFcode);
         pCorp[iNum].lLasttime=pAreastatdata->lLasttime;
         if(pCorp[iNum].endnum<pCorp[iNum].onuid_count){
         	pCorp[iNum].endnum=pCorp[iNum].onuid_count;
        }
         iNum++;
        }
         
       
       pAreastatdata = (struct servicedata_s *)pasLHashNext(&sHashInfo);
    }
    
    lSumcorp=iNum;

    qsort(pCorp,lSumcorp,sizeof(struct servicedata_s),ncsUtlSortOnCorp);

//  printf("lSumcopr=%d\n",lSumcorp);
    if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
    	char caState_desc[32];
      sprintf(caFilename,"ncwarn%d.csv",utStrGetId());
      sprintf(caPath,"%s/html","/home/ncmysql/ncs");
      sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0893监控单位"),ncsLang("0546数据导出出错"));
            return 0;
        }
    ncsWriteSysLog(psMsgHead,caUsername2,"06","成功","导出上网服务场所信息"); 
       
      fprintf(fp,(char *)ncsLang("0893监控单位\n"));
      ncUtlPrintCsv(fp,13,UT_TYPE_STRING,"场所编码",
                       UT_TYPE_STRING,ncsLang("0906单位名称"),
                       UT_TYPE_STRING,"单位类别",
                       UT_TYPE_STRING,ncsLang("0054IP地址"),
                       UT_TYPE_STRING,ncsLang("0907E-Mail"),
                       UT_TYPE_STRING,ncsLang("0908地址"),
                       UT_TYPE_STRING,ncsLang("0910电话"),
                       UT_TYPE_STRING,ncsLang("0909手机"),
                       UT_TYPE_STRING,ncsLang("0911联系人"),
                       UT_TYPE_STRING,ncsLang("0912最后上线时间"),
                       UT_TYPE_STRING,"单位组",
                       UT_TYPE_STRING,"状态",
                       UT_TYPE_STRING,"备注");
                      
       for(i=0;i<lSumcorp;i++){
       	       if(pCorp[i].onstate==1){
       	       	strcpy(caState_desc,"在线");
       	      }
       	      else if(pCorp[i].onstate==2){
       	      	strcpy(caState_desc,"离线");
       	      }
       	      else if(pCorp[i].onstate==4){
       	      	strcpy(caState_desc,"停业");
       	      }
       	      else if(pCorp[i].onstate==5){
       	      	strcpy(caState_desc,"维护");
       	      }
       	     else if(pCorp[i].onstate==9){
       	      	strcpy(caState_desc,"待审核");
       	      }
       	      else{
       	      	strcpy(caState_desc,"");
       	      }
       	       sprintf(caTemp,"\r%s",pCorp[i].caServicecode);
       	       
       memset(caCode,0,sizeof(caCode));
       sprintf(caCode,"%s",pCorp[i].caGtype);
       psFac=(struct fax_s *)pasLHashLook(pHash_lb,caCode);
       if(psFac){
 //      	printf("aaaaaaaa=%s\n",psFac->caName);
          strcpy(caDwlb,psFac->caName);
       	
       }
       else{
       	strcpy(caDwlb,"");
      }
	       
       	       
       	        ncUtlPrintCsv(fp,13,UT_TYPE_STRING,caTemp,
                       UT_TYPE_STRING,pCorp[i].caCname,
                       UT_TYPE_STRING,caDwlb,
                       UT_TYPE_STRING,pCorp[i].caIp,
                       UT_TYPE_STRING,pCorp[i].caEmail,
                       UT_TYPE_STRING,pCorp[i].caAddress,
                       UT_TYPE_STRING,pCorp[i].caTel,
                       UT_TYPE_STRING,pCorp[i].caHandphone,
                       UT_TYPE_STRING,pCorp[i].caContact,
                       UT_TYPE_STRING,pCorp[i].caDate,
                       UT_TYPE_STRING,pCorp[i].caGroupname,
                       UT_TYPE_STRING,caState_desc,
                       UT_TYPE_STRING,pCorp[i].caDemo);
       	
      }
        
    
   	 
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        free(pHash);
        free(pHash_fac);
        free(pCorp);
        return 0;
    }
         
    psDbHead = utPltInitDb();
   

   	iNum = 0;
  	
   	for(i=0;i<lSumcorp;i++){
   		if(i>=lStartRec&&iNum<lRowNum){
   			if(iNum>0){
        	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
        }
   			iNum++;
   			utPltPutLoopVarF(psDbHead,"useflags", iNum,"%d",pCorp[i].lUseflags);
        utPltPutLoopVarF(psDbHead,"userid",   iNum,"%lu",pCorp[i].userid);
        utPltPutLoopVar(psDbHead,"username", iNum,pCorp[i].caServicecode);
        utPltPutLoopVar(psDbHead,"dispname", iNum,pCorp[i].caCname);
        utPltPutLoopVar(psDbHead,"groupname",iNum,pCorp[i].caGroupname);
        utPltPutLoopVarF(psDbHead,"num", iNum,"%d",iNum-1);
        utPltPutLoopVar(psDbHead,"email", iNum,pCorp[i].caEmail);
        utPltPutLoopVar(psDbHead,"addr",iNum,pCorp[i].caAddress);
        utPltPutLoopVar(psDbHead,"contact",iNum,pCorp[i].caContact);
        utPltPutLoopVar(psDbHead,"ip", iNum,pCorp[i].caIp);
        utPltPutLoopVar(psDbHead,"version",iNum,pCorp[i].caVersion);
        utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",pCorp[i].groupid);
        utPltPutLoopVar(psDbHead,"telphone",iNum,pCorp[i].caTel);
        utPltPutLoopVar(psDbHead,"jointype",iNum,ncsGetJoinTypeByCode(pCorp[i].caJointype));
//   			if(strlen(pCorp[i].caServicecode)>10){
//       	memset(caCode,0,sizeof(caCode));
//       	char *p=&pCorp[i].caServicecode[0];
//       	memcpy(caCode,p+7,1);
  //     	printf("caCode=%s\n",caCode);
//       	  utPltPutLoopVar(psDbHead,"dwlx",iNum,ncsGetUserTnameByType(caCode));
 //       }
       	memset(caCode,0,sizeof(caCode));
       	strcpy(caCode,pCorp[i].caFcode);
       psFac=(struct fax_s *)pasLHashLook(pHash_fac,caCode);
       if(psFac){
       	utPltPutLoopVar(psDbHead,"jrcj",iNum,psFac->caName);
       }
       
       
       memset(caCode,0,sizeof(caCode));
       	sprintf(caCode,"%s",pCorp[i].caGtype);
  //     	printf("caCode=%s\n",caCode);
       psFac=(struct fax_s *)pasLHashLook(pHash_lb,caCode);
       if(psFac){
 //      	printf("aaaaaaaa=%s\n",psFac->caName);
       	utPltPutLoopVar(psDbHead,"dwlb",iNum,psFac->caName);
       }
       
       
       
   			utPltPutLoopVar(psDbHead,"xxdesc",iNum,"详细");
   			utPltPutLoopVarF(psDbHead,"endnum",iNum,"%d",pCorp[i].endnum);  			    
        utPltPutLoopVarF(psDbHead,"onuser",iNum,"%lu",pCorp[i].onuid_count);
    
       if(pCorp[i].onstate==1){
       	utPltPutLoopVar(psDbHead,"status", iNum,"<font color=green>在线</font>");
      }
      else if(pCorp[i].onstate==2){
 //     	if(
      	utPltPutLoopVar(psDbHead,"status",iNum,"<font color=red>离线</font>");
      }
      else if(pCorp[i].onstate==4){
      	utPltPutLoopVar(psDbHead,"status",iNum,"<font color=blue>停业</font>");
      }
      else if(pCorp[i].onstate==5){
      	utPltPutLoopVar(psDbHead,"status",iNum,"维护");
      }
      else if(pCorp[i].onstate==9){
      	utPltPutLoopVar(psDbHead,"status",iNum,"待审核");
      }
      utPltPutLoopVar(psDbHead,"xxdesc",iNum,"详细");
       
      if(pCorp[i].lLasttime>0){
      	utPltPutLoopVar(psDbHead,"lasttime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",pCorp[i].lLasttime)); 
      }

       	       
       	memset(caCode,0,sizeof(caCode));
       sprintf(caCode,"%s",pCorp[i].caGtype);
       psFac=(struct fax_s *)pasLHashLook(pHash_lb,caCode);
       if(psFac){
 //      	printf("aaaaaaaa=%s\n",psFac->caName);
          strcpy(caDwlb,psFac->caName);
       	
       }
       else{
       	strcpy(caDwlb,"");
      }

   
  //    utPltPutLoopVar(psDbHead,"gtype",iNum,ncsGetUserTnameByType(caTemp));
        utPltPutLoopVar(psDbHead,"demo",iNum,caDwlb);
        
        utPltPutLoopVarF(psDbHead,"CurPg",iNum,"%lu", lCurPg);
 			
   			if(atol(caLastln)==1||(pCorp[i].onstate!=1)){
//获取上次客户端连接时间
   				sprintf(caTemp,"select sdate from ncscltlog where userid=%lu order by sdate desc limit 0,1 ",pCorp[i].userid);
   				
			printf("caTemp=%s\n",caTemp);
   				memset(caClastime,0,sizeof(caClastime));
   				
   				pasDbOneRecord(caTemp,0,UT_TYPE_STRING,20,caClastime);
   				utPltPutLoopVar(psDbHead,"clastime",iNum,caClastime);
   			}
   			
   		}
   	}
   
     
    lCount=lSumcorp;
    lTotPg = (lCount - 1) / lRowNum + 1;
    lTotRec = lCount;
    i = 0;
//    while(iNum < lRowNum + 1) {
 //       i++;
       
 //       utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
 //        iNum++;
//    }
    utPltPutVar(psDbHead,"sort", caSort);
    utPltPutVar(psDbHead,"sortby", caSortby);

    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    utPltPutVar(psDbHead,"groupid",caGroupid);
    utPltPutVar(psDbHead,"did",caDid);
    utPltPutVar(psDbHead,"name",name);
    
    utPltPutVar(psDbHead,"servicecode1",servicecode1);
    utPltPutVar(psDbHead,"dispname1",dispname1);
    utPltPutVar(psDbHead,"ceoname1",ceoname1);
    utPltPutVar(psDbHead,"contact1",contact1);
    utPltPutVar(psDbHead,"ip1",ip1);
    utPltPutVar(psDbHead,"servicestate1",servicestate1);
    utPltPutVar(psDbHead,"gtype",caGtype);
    utPltPutVar(psDbHead,"fcode",caFcod_c);
    utPltPutVar(psDbHead,"lxstate",caLxstate);
    if(pHash) free(pHash);
    if(pCorp) free(pCorp);
    /*
     psFac  = (struct fax_s *)pasLHashFirst(pHash_fac,&sHashInfo);

    iNum = 0;
    while(psFac) {

           utPltPutLoopVar(psDbHead,"fcod",iNum+1,psFac->caCod);
           utPltPutLoopVar(psDbHead,"fname",iNum+1,psFac->caName);
           iNum++;
       psFac = (struct fax_s *)pasLHashNext(&sHashInfo);
    }
    
   */ 
    free(pHash_fac);
    free(pHash_lb);
//  utPltShowDb(psDbHead); 
   free(psUserAtt);
    
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_corp_list.htm");
     
    return 0;
}



/* 显示增加/修改用户的界面  */
int ncsWebDispClientAddForm_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caIp[34],caMac[64],caAddtime[16];
    char caUserid[32];
   	char caUsername[34],caHandphone[32];
   	char caEmail[127],caLasttime[16];
   	char caPassword[32];
   	char caCond[32],caGroupid[16];
   	char caContact[32],caTelphone[32],caAddress[128];
   	int iNum1;
   	long lUse;
   	unsigned long lUserid,lGroupid,lGroupid0;
    utPltDbHead *psDbHead;
   	int iReturn;
   	int iModFlags;
   	char caSmt[256];
   	char *pBuf,*pSelect;
   	char  caUse[16];
   	unsigned long lTime;
   	char caDispname[64],groupname[32];
   	ncsClient *psClient;
   	char caTemp[128];
   	int iNum=0;
   	unsigned int groupid=0;
   	int gtype=0;
    char sqlbuf[4048]="";
    char caGroupcode[32];
   	char handphone[36],servicecode[16],postcode[10],servicestate[12],ceoname[32],areaman[32];
   	char joincode[10],empnum[10],area[72],areatel[32],demo[256],ip[18],servernum[14],endnum[10],jointype[10];
   	int useflags,userid;
   	char caLongitude[16],caLatitude[16];
   	char servernum1[8],postcode1[16],caEmail1[127];
   	char disflag[8];
   	char caPlate[128];
   	char servicestate_choice[16]="selected";
   	char join_type_choice[15]="sel";
   	long lMonflag=0;
    char caAreacodeToUser[32];
   char caFcode[16],caName[68];
    char caGroups[1024];
    char caFacs[1024];   
   long lDateid;
   double fLongitude,fLatitude;
   char caGtype[32];
   long lJytype;
   char caCeoidtype[8],caCeocardid[32],caCeotel[64],caSdate[20],caEdate[20],caPort_start[20],caPort_end[20],caJytype[8],caCeltel[32];
    //获取权限组ID和接入厂商
    strcpy(caFacs,getDsFacGroupid());
    strcpy(caGroups,getDsGroupid());
   
   strcpy(caAreacodeToUser,utComGetVar_sd(psShmHead,"AreacodeToUser","No"));
   
    psDbHead = utPltInitDb();
    sprintf(sqlbuf,"select groupid,groupname from ncsgroup where 1=1 ");
    if(strlen(caGroups)>0){
    	sprintf(sqlbuf+strlen(sqlbuf)," and groupid in (%s) ",caGroups);
    }
    sprintf(sqlbuf+strlen(sqlbuf)," order by groupid desc");
    psCur = pasDbOpenSql(sqlbuf,0);
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_ULONG,4,&groupid,
      	                               UT_TYPE_STRING,31,groupname);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%lu",groupid);
        utPltPutLoopVar(psDbHead,"groupname",iNum,groupname);
        iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_ULONG,4,&groupid,
      	                               UT_TYPE_STRING,31,groupname);
    }
    pasDbCloseCursor(psCur);
    
    
    
  //接入厂商  
    sprintf(sqlbuf,"select code,name from ncsfactorycod where 1=1 ");
    if(strlen(caFacs)>0){
    	sprintf(sqlbuf+strlen(sqlbuf)," and code in (%s)",caFacs);
    }
    psCur = pasDbOpenSql(sqlbuf,0);
    memset(caFcode,0,sizeof(caFcode));
    memset(caName,0,sizeof(caName));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caFcode,
                                   UT_TYPE_STRING,60,caName);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVar(psDbHead,"fcod",   iNum,caFcode);
        utPltPutLoopVar(psDbHead,"fname",iNum,caName);
		    memset(caFcode,0,sizeof(caFcode));
		    memset(caName,0,sizeof(caName));
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caFcode,
		                                   UT_TYPE_STRING,60,caName);
    }
    pasDbCloseCursor(psCur);
    
    //营业时间
    psCur = pasDbOpenSql("select id,name from nclimdatedes ",0);
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&groupid,
      	                               UT_TYPE_STRING,31,groupname);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVarF(psDbHead,"dateid",   iNum,"%lu",groupid);
        utPltPutLoopVar(psDbHead,"datename",iNum,groupname);
        iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&groupid,
      	                               UT_TYPE_STRING,31,groupname);
    }
    pasDbCloseCursor(psCur);
    
    
    
    
    
    
    
    
   	iReturn = utMsgGetSomeNVar(psMsgHead,5,
   	      "groupid", UT_TYPE_STRING, 15, caGroupid,     /* 位置  */
					"userid",  UT_TYPE_STRING, 15, caUserid,
					"cond",    UT_TYPE_STRING, 31, caCond,
					"disflag", UT_TYPE_STRING,2,disflag,
					"plate",UT_TYPE_STRING,100,caPlate);

    iModFlags = 0;
 //   lUserid = atol(caUserid);

    lUserid=strtoul(caUserid,NULL,10);
     printf("caUserid=%s,userid=%lu\n",caUserid,lUserid);
    lTime = time(0);
    if(!utStrIsSpaces(caUserid)) { 
        iModFlags = 1;
    //    lUserid = atol(caUserid);
        
        
        
        
        userid=0;
        memset(caUsername,0,sizeof(caUsername));
        memset(caPassword,0,sizeof(caPassword));
        lGroupid=0;
        memset(caDispname,0,sizeof(caDispname));
        memset(caAddress,0,sizeof(caAddress));
        memset(caTelphone,0,sizeof(caTelphone));
        memset(caContact,0,sizeof(caContact));
        memset(handphone,0,sizeof(handphone));
        memset(caEmail,0,sizeof(caEmail));
        memset(ip,0,sizeof(ip));
        memset(caAddtime,0,sizeof(caAddtime));
        useflags=0;
        memset(caLasttime,0,sizeof(caLasttime));
        gtype=0;
        memset(servicestate,0,sizeof(servicestate));
        memset(joincode,0,sizeof(joincode));
        memset(endnum,0,sizeof(endnum));
        memset(servernum,0,sizeof(servernum));
        memset(jointype,0,sizeof(jointype));
        memset(empnum,0,sizeof(empnum));
        memset(area,0,sizeof(area));
        memset(areaman,0,sizeof(areaman));
        memset(areatel,0,sizeof(areatel));
        memset(demo,0,sizeof(demo));
        memset(servicecode,0,sizeof(servicecode));
        memset(postcode,0,sizeof(postcode));
        memset(ceoname,0,sizeof(ceoname));
       memset(caEmail1,0,sizeof(caEmail1)); 
       memset(servicestate,0,sizeof(servicestate));                    
       memset(endnum,0,sizeof(endnum));
       memset(servernum1,0,sizeof(servernum1));
       memset(ip,0,sizeof(ip));     
       memset(postcode1,0,sizeof(postcode1));
       memset(caLasttime,0,sizeof(caLasttime)); 
       memset(caFcode,0,sizeof(caFcode));
       lDateid=0;
       lMonflag=0;
       lJytype=0;
       memset(caCeoidtype,0,sizeof(caCeoidtype));
       memset(caCeocardid,0,sizeof(caCeocardid));
       memset(caSdate,0,sizeof(caSdate));
       memset(caEdate,0,sizeof(caEdate));
       memset(caPort_start,0,sizeof(caPort_start));
       memset(caPort_end,0,sizeof(caPort_end));
        sprintf(sqlbuf,"select userid,username,password,groupid,dispname,address,\
                               telphone,contact,handphone,addtime,useflags,gtype,joincode,\
                               jointype,empnum,area,areaman,areatel,demo,\
                               ceoname,endnum,servernum,postcode,email,servicestate,ip,lasttime,monflag,fcode,dateid,longitude,latitude,\
                               jytype,ceoidtype,ceocardid,ceotel,sdate,edate,port_start,port_end from ncsuser where userid=%lu",lUserid);                
       printf("sqlbuf=%s\n",sqlbuf);  
      memset(caLongitude,0,sizeof(caLongitude));
      memset(caLatitude,0,sizeof(caLatitude));       
        iReturn = pasDbOneRecord(sqlbuf,
                            0,
                            UT_TYPE_LONG,4,&userid,
                            UT_TYPE_STRING,31, caUsername,
                            UT_TYPE_STRING,31, caPassword,
                            UT_TYPE_LONG,4,&lGroupid,
                            UT_TYPE_STRING,63, caDispname,
                            UT_TYPE_STRING,127,caAddress,
                            UT_TYPE_STRING,63, caTelphone,
                            UT_TYPE_STRING,31, caContact,
                            UT_TYPE_STRING,31,handphone,
                            UT_TYPE_STRING,16,caAddtime,
                            UT_TYPE_LONG,4,&useflags, 
                            UT_TYPE_STRING,2,caGtype,
                            UT_TYPE_STRING,10,joincode,
                            UT_TYPE_STRING,10,jointype,
                            UT_TYPE_STRING,10,empnum,
                            UT_TYPE_STRING,70,area,
                            UT_TYPE_STRING,70,areaman,
                            UT_TYPE_STRING,30,areatel,
                            UT_TYPE_STRING,255,demo,
                            UT_TYPE_STRING,30,ceoname,
                            UT_TYPE_STRING,6,endnum,
                            UT_TYPE_STRING,2,servernum1,
                            UT_TYPE_STRING,6,postcode1,
                            UT_TYPE_STRING,127,caEmail1,
                            UT_TYPE_STRING,2,servicestate,
                            UT_TYPE_STRING,16,ip,
                            UT_TYPE_STRING,16,caLasttime,
                            UT_TYPE_LONG,4,&lMonflag,
                            UT_TYPE_STRING,10,caFcode,
                            UT_TYPE_LONG,4,&lDateid,
                            UT_TYPE_STRING,12,caLongitude,
                            UT_TYPE_STRING,12,caLatitude,
                            UT_TYPE_LONG,4,&lJytype,
                            UT_TYPE_STRING,sizeof(caCeoidtype)-1,caCeoidtype,
                            UT_TYPE_STRING,sizeof(caCeocardid)-1,caCeocardid,
                            UT_TYPE_STRING,sizeof(caCeltel)-1,caCeltel,
                            UT_TYPE_STRING,sizeof(caSdate)-1,caSdate,
                            UT_TYPE_STRING,sizeof(caEdate)-1,caEdate,
                            UT_TYPE_STRING,sizeof(caPort_start)-1,caPort_start,
                            UT_TYPE_STRING,sizeof(caPort_end)-1,caPort_end);
                              
                               
     if(iReturn != 0 && iReturn != PAS_DB_NULLVALUE) {
            utPltFreeDb(psDbHead);
            utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","修改用户","用户不存在 %d ",iReturn);
           return 0;
      }
    else{    
          utPltPutVar(psDbHead,"userid",    caUserid);
          utPltPutVar(psDbHead,"username",  caUsername);
          utPltPutVar(psDbHead,"dispname",  caDispname); //单位名称
          
          
//          printf("gtype=%d\n",gtype);
//          printf("lGroupid=%d\n",lGroupid);
          utPltPutVarF(psDbHead,"groupid","%lu",lGroupid);//所属组
        
        
        sprintf(sqlbuf,"select code,name from ncsuser_lb where 1=1 ");
        
		  	psCur = pasDbOpenSql(sqlbuf,0);
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   char caCode[16];
	     memset(caName,0,sizeof(caName));
       memset(caCode,0,sizeof(caCode));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
		                                  UT_TYPE_STRING,63,caName);
		                                  
		                                
		
		   iNum=0;
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  		        
					   utPltPutLoopVarF(psDbHead,"lbname",iNum,"%s",caName);					    
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"lbcode",   iNum,"%s",caCode);
		         if(strcmp(caGtype,caCode)==0){
		         	utPltPutLoopVar(psDbHead,"check",iNum,",checked:true");
		        }
		       
		         	
	     memset(caName,0,sizeof(caName));
       memset(caCode,0,sizeof(caCode));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
		                                  UT_TYPE_STRING,63,caName);
		    }
		    pasDbCloseCursor(psCur);
        
        
        
        
      /*  
          
          
          if(gtype==1){
          	utPltPutVar(psDbHead,"jdcheck",",checked:true");
          }
          else if(gtype==2){
          	utPltPutVar(psDbHead,"ylscheck",",checked:true");
          }
          else if(gtype==3){
          	utPltPutVar(psDbHead,"xxcheck",",checked:true");
          }
          else if(gtype==4){
          	utPltPutVar(psDbHead,"ylcheck",",checked:true");
          }
          else if(gtype==5){
          	utPltPutVar(psDbHead,"dzcheck",",checked:true");
          }
          else if(gtype==6){
          	utPltPutVar(psDbHead,"qycheck",",checked:true");
          }
          else if(gtype==7){
          	utPltPutVar(psDbHead,"idccheck",",checked:true");
          }
          else if(gtype==8){
          	utPltPutVar(psDbHead,"jtcheck",",checked:true");
          }
          else if(gtype==10){
          	utPltPutVar(psDbHead,"gxcheck",",checked:true");
          }
          else if(gtype==11){
          	utPltPutVar(psDbHead,"wbcheck",",checked:true");
          }
          else{
          	utPltPutVar(psDbHead,"qtcheck",",checked:true");
          }
          */
          utPltPutVarF(psDbHead,"gtype","%s",caGtype); //公司类型
          	 
                  
          utPltPutVar(psDbHead,"servicestate",servicestate);//服务状态
          utPltPutVar(psDbHead,"endnum",endnum);
          
        
         
          utPltPutVar(psDbHead,jointype,jointype);//接入类型
   
          utPltPutVar(psDbHead,"end_num",endnum);
          utPltPutVar(psDbHead,"servernum",servernum1);
          utPltPutVar(psDbHead,"postcode",postcode1);
          utPltPutVar(psDbHead,"email_addr",caEmail1);
          utPltPutVar(psDbHead,"joincode",joincode);
          utPltPutVar(psDbHead,"servicecode",servicecode);

          utPltPutVar(psDbHead,"address", caAddress); 
          utPltPutVar(psDbHead,"ceoname",ceoname);
          utPltPutVar(psDbHead,"handphone",handphone);
          utPltPutVar(psDbHead,"contact", caContact);  
          utPltPutVar(psDbHead,"telphone",caTelphone);
          utPltPutVar(psDbHead,"empnum",empnum);
          utPltPutVar(psDbHead,"area",area);
          utPltPutVar(psDbHead,"areaman",areaman);
          utPltPutVar(psDbHead,"areatel",areatel);
          utPltPutVar(psDbHead,"entrance_ip", ip);
          utPltPutVar(psDbHead,"jointype",jointype);
          utPltPutVar(psDbHead,"fcode",caFcode);
          utPltPutVarF(psDbHead,"dateid","%d",lDateid);
          utPltPutVarF(psDbHead,"longitude","%s",caLongitude);
          utPltPutVarF(psDbHead,"latitude","%s",caLatitude);
           psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,atol(caUserid));
           if(psClient){
	          if(psClient->status==1)
	          	utPltPutVar(psDbHead,"userflags","正常");
	          else if(psClient->status==3){
	          	sprintf(caTemp,"异常在线:%s",psClient->caMsg);
	            utPltPutVar(psDbHead,"userflags",caTemp);
	          }
	          else if(psClient->status==2){
	          	utPltPutVar(psDbHead,"userflags","离线");
	          }
          }
          utPltPutVar(psDbHead,"addtime", caAddtime);
          utPltPutVar(psDbHead,"lasttime", caLasttime);
          utPltPutVarF(psDbHead,"monflag","%lu",lMonflag);
          utPltPutVarF(psDbHead,"jytype","%d",lJytype);
          utPltPutVar(psDbHead,"ceoidtype",caCeoidtype);
          utPltPutVar(psDbHead,"ceocardid",caCeocardid);
          utPltPutVar(psDbHead,"sdate",caSdate);
          utPltPutVar(psDbHead,"edate",caEdate);
          utPltPutVar(psDbHead,"port_start",caPort_start);
          utPltPutVar(psDbHead,"port_end",caPort_end);
          utPltSetCvtHtml(1);
          utPltPutVar(psDbHead,"demo",demo);//备注
          utPltSetCvtHtml(0);
  //        printf("demo=%s\n",demo);
          if(strncmp(caPassword,"***",3)!=0 && strncmp(caPassword,"@@@",3)!=0) {
                utPltPutVar(psDbHead,"password", "********");
            }
        
    }
  }
    else {
    	  utPltPutVar(psDbHead,"sdate","08:00");
    	  utPltPutVar(psDbHead,"edate","18:00");
        strcpy(caAddtime,utTimFormat("%Y-%m-%d %H:%M:%S",lTime));
        utPltPutVar(psDbHead,"addtime", caAddtime);
        utPltPutVar(psDbHead,"lasttime",caAddtime);
        if(strlen(caGroupid)!=0)
        utPltPutVar(psDbHead,"groupid",caGroupid);
        else
        utPltPutVar(psDbHead,"groupid","0");
        
        utPltPutVar(psDbHead,"qtcheck",",checked:true");
     
        utPltPutVar(psDbHead,"servicestate","1");//服务状态
        utPltPutVar(psDbHead,"jointype","99");//接入类型
        utPltPutVar(psDbHead,"monflag","0");
        utPltPutVar(psDbHead,"end_num","0");
        utPltPutVar(psDbHead,"servernum","0");
    }
  //将行政区代码自动转到单位代码的前几位上
    if(strcasecmp(caAreacodeToUser,"Yes")==0){
    	sprintf(caTemp,"select groupcode from ncsgroup where groupid=%s ",caGroupid);
//    	printf("caTemp=%s\n",caTemp);
    	memset(caGroupcode,0,sizeof(caGroupcode));
    	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,10,caGroupcode);
    	utPltPutVar(psDbHead,"groupcode",caGroupcode);
    	utPltPutVar(psDbHead,"codeflag","1");
    }
    
    
    if(strlen(caPlate)>0){
    	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    }
    else{
    	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/nc_form_clientedit.htm");
    }
    return 0;
}



/* 增加用户  */
int ncsWebClientAdd_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
   	char caUsername[33],caGroupid[33],caIp[16],caMac[33],caMacstr[42],caUse[10];
   	char caDispname[64],caLimit[32];
   	char caContact[32],caTelphone[32],caAddress[128];
   	char caHandphone[32];
   	unsigned long lUserid,lGroupid;
    utPltDbHead *psDbHead;
   	int iReturn;
   	unsigned long lUse,lUserid0;
   	unsigned long lIp;
   	int iPass,iAddFlags;
   	char caPassword[32],caPassword1[32];
   	char caDate[32],caUserid[16],caUsername0[32];
   	char caGtype[16];
   	ncsClient *psClient;
    char caOk[16],caSave[16],caEmail[64],caWebip[16],caPost[16],caLimitid[16];

	char handphone[32],servicecode[16],postcode[16],servicestate[10],ceoname[30],areaman[30],gtype[32],addtime[30],lasttime[30];
  char joincode[16],empnum[16],area[70],areatel[30],demo[255],ip[16],servernum[12],endnum111[10],jointype[12];
  char servicestate_desc[32],pre_setdesc[32],caPreset[16];
	char sqlbuf[4048]="";
  char caTemp[1024];
  char caUsername2[32];
 	char caMsg[1024];

 	char caMonflag[12];
 	char caFcode[16],caDateid[16];
 	char caLongitude[16],caLatitude[16];
  char caCeoidtype[8],caCeocardid[32],caCeotel[64],caSdate[20],caEdate[20],caPort_start[20],caPort_end[20],caJytype[8];
 	memset(caMonflag,0,sizeof(caMonflag));
 	dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);

    strcpy(caDate,utTimFormat("%Y-%m-%d %H:%M:%S",time(0)));

    utMsgPrintMsg(psMsgHead);

   	iReturn = utMsgGetSomeNVar(psMsgHead,38,
   	      "userid",      UT_TYPE_STRING, 10, caUserid,
					"username",    UT_TYPE_STRING, 30, caUsername,
				  "groupid",     UT_TYPE_STRING,10,caGroupid,
					"dispanme",    UT_TYPE_STRING, 63, caDispname,
					"address",     UT_TYPE_STRING, 127, caAddress,
					"telphone",    UT_TYPE_STRING, 15, caTelphone,
				  "contact",     UT_TYPE_STRING, 15, caContact,
					"handphone",   UT_TYPE_STRING, 30,handphone,
				  "email_addr",  UT_TYPE_STRING, 63, caEmail,
					"entrance_ip", UT_TYPE_STRING, 16, caIp,
					"addtime",     UT_TYPE_STRING,24,addtime,
					"lasttime",    UT_TYPE_STRING,24,lasttime,
          "gtype",       UT_TYPE_STRING, 10,gtype,
					"servicestate",UT_TYPE_STRING,10,servicestate,
					"joincode",    UT_TYPE_STRING,6,joincode,				
					"servernum",    UT_TYPE_STRING,4,servernum,
					"end_num111",   UT_TYPE_STRING,8,endnum111,
					"jointype",    UT_TYPE_STRING,2,jointype,
					"empnum" ,      UT_TYPE_STRING,6,empnum,
					"area" ,        UT_TYPE_STRING,70,area,
					"areaman" ,     UT_TYPE_STRING,30,areaman,
					"areatel",      UT_TYPE_STRING,30,areatel,
          "demo" ,         UT_TYPE_STRING,255,demo,
					"servicecode", UT_TYPE_STRING,15,servicecode,
          "postcode",    UT_TYPE_STRING,6,postcode,
					"ceoname",     UT_TYPE_STRING,30,ceoname,
					"monflag",     UT_TYPE_STRING,10,caMonflag,
					"fcode",       UT_TYPE_STRING,10,caFcode,
					"dateid",      UT_TYPE_STRING,10,caDateid,
					"longitude",   UT_TYPE_STRING,12,caLongitude,
					"latitude",    UT_TYPE_STRING,12,caLatitude,
					"jytype",      UT_TYPE_STRING,sizeof(caJytype)-1,caJytype,
					"ceoidtype",   UT_TYPE_STRING,sizeof(caCeoidtype)-1,caCeoidtype,
					"ceocardid",   UT_TYPE_STRING,sizeof(caCeocardid)-1,caCeocardid,
					"sdate",       UT_TYPE_STRING,sizeof(caSdate)-1,caSdate,
					"edate",       UT_TYPE_STRING,sizeof(caEdate)-1,caEdate,
					"port_start",  UT_TYPE_STRING,sizeof(caPort_start)-1,caPort_start,
					"port_end",    UT_TYPE_STRING,sizeof(caPort_end)-1,caPort_end);
	 utStrDelSpaces(caUsername);
    utStrDelSpaces(caIp);			
			
	//编码转换
	if(strlen(caUsername)>0){
		 pasCvtGBK(2,caUsername,caTemp,30);
		 strcpy(caUsername,caTemp);
	}
	if(strlen(caDispname)>0){
		 pasCvtGBK(2,caDispname,caTemp,63);
		 strcpy(caDispname,caTemp);
	}
	if(strlen(caAddress)>0){
		 pasCvtGBK(2,caAddress,caTemp,63);
		 strcpy(caAddress,caTemp);
	}
	if(strlen(caTelphone)>0){
		 pasCvtGBK(2,caTelphone,caTemp,63);
		 strcpy(caTelphone,caTemp);
	}	
	if(strlen(caContact)>0){
		 pasCvtGBK(2,caContact,caTemp,63);
		 strcpy(caContact,caTemp);
	}	
	if(strlen(handphone)>0){
		 pasCvtGBK(2,handphone,caTemp,63);
		 strcpy(handphone,caTemp);
	}		
	if(strlen(area)>0){
		 pasCvtGBK(2,area,caTemp,63);
		 strcpy(area,caTemp);
	}	
	if(strlen(areaman)>0){
		 pasCvtGBK(2,areaman,caTemp,63);
		 strcpy(areaman,caTemp);
	}	
	if(strlen(demo)>0){
		 pasCvtGBK(2,demo,caTemp,63);
		 strcpy(demo,caTemp);
	}
	if(strlen(ceoname)>0){
		 pasCvtGBK(2,ceoname,caTemp,63);
		 strcpy(ceoname,caTemp);
	}
		if(strlen(joincode)>0){
		 pasCvtGBK(2,joincode,caTemp,6);
		 strcpy(joincode,caTemp);
	}
		
					
		                     
				

  
    
    if(utStrIsSpaces(caUserid)) {
    	lUserid=pasGetSid("ncsuser","userid");
 //       lUserid =  ncsUtlGenId(psShmHead);
        iAddFlags = 1;
           
    }
    else {
    	lUserid = atol(caUserid);
    	iAddFlags = 0;
    }
    if(iAddFlags) { /* 增加记录  */
        	

    		sprintf(sqlbuf,"添加上网场所%s,名称：%s",caUsername,caDispname);
    		ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",sqlbuf);
 	      strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M",time(0)));
		      sprintf(sqlbuf,"insert into ncsuser (userid,username,groupid,dispname,address,telphone,contact,handphone,email,ip,addtime,lasttime,\
		               gtype,servicestate,joincode,endnum,servernum,jointype,empnum,area,areaman,areatel,demo,postcode,ceoname,moditime,monflag,password,fcode,dateid,jytype,ceoidtype,ceocardid,sdate,edate,port_start,port_end) values \
                                                 (%lu,'%s', %lu,'%s','%s','%s','%s','%s','%s','%s','%s','%s',\
                                                 %d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%lu,%lu,'','%s',%d,%d,'%s','%s','%s','%s','%s','%s')",\
                                                         lUserid,caUsername,strtoul(caGroupid,NULL,10),caDispname,caAddress,caTelphone,caContact,handphone,caEmail,caIp,caSdate,caSdate,
														 atoi(gtype),servicestate,joincode,endnum111,servernum,jointype,empnum,
		 												 area,areaman,areatel,demo,postcode,ceoname,time(0),atol(caMonflag),caFcode,atol(caDateid),atol(caJytype),caCeoidtype,caCeocardid,caSdate,caEdate,caPort_start,caPort_end);
	 					printf("sqlbuf=%s\n",sqlbuf);								 
            iReturn = pasDbExecSqlF(sqlbuf);
            if(iReturn==0){
            	 ncsWebReqSyscorp_v4(psShmHead,sqlbuf); 
            }
  //          ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",sqlbuf);
        
//						printf("sqlbuf=%s\n",sqlbuf);					 
   //        iReturn =pasDbExecSqlF(sqlbuf);
           
           sprintf(sqlbuf,"添加上网场所 场所编码%s，名称：%s",caUsername,caDispname);    
           ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",sqlbuf);
            
        if(iReturn != 0) {
        	  strcpy(caMsg,"false");
            pasDbRollback(NULL);
        }
        else { 
            pasDbCommit(NULL);
            strcpy(caMsg,"true");
    //        sprintf(caUserid,"%d",lUserid);
    //        utMsgAdd_s(&psMsgHead,"userid",caUserid);

            if(strlen(caIp)>0){
		        	if(utStrIsValidIp(caIp)){
		        		lIp=ntohl(utComHostAddress(caIp));
		        		sprintf(caTemp,"insert into ncsuserip (userid,lip,uip,stime) values(%lu,%lu,%lu,%lu) ",lUserid,lIp,lIp,time(0));

		        		pasDbExecSql(caTemp,0);
        			}
        		}
            
        }
//    printf("iReturn=%d,msg=%s\n",iReturn,caMsg);    
        psDbHead = utPltInitDb();
        utPltPutVar(psDbHead,"mesg",caMsg);
        utPltPutVarF(psDbHead,"userid","%d",lUserid);
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_msg.htm");
        return 0;
        
    }        
    else { /* 修改记录  */
  
			sprintf(sqlbuf,"update ncsuser set username='%s',groupid=%lu,\
      	        gtype=%d,servicestate='%s',jointype='%s',joincode='%s',endnum='%s',\
				servernum='%s',dispname='%s',address='%s',ceoname='%s',handphone='%s',postcode='%s',\
				email='%s',contact='%s',telphone='%s',areaman='%s',areatel='%s',ip='%s',empnum='%s',area='%s',demo='%s',moditime=%lu,monflag=%lu,fcode='%s',dateid=%d,longitude='%s',latitude='%s',\
				jytype=%d,ceoidtype='%s',ceocardid='%s',sdate='%s',edate='%s',port_start='%s',port_end='%s' where userid=%lu",
                caUsername,strtoul(caGroupid,NULL,10),atoi(gtype),servicestate,
				jointype,joincode,endnum111,servernum,caDispname,caAddress,
                ceoname,handphone,postcode,caEmail,caContact,caTelphone,areaman,
				areatel,caIp,empnum,area,demo,time(0),atol(caMonflag),caFcode,atol(caDateid),caLongitude,caLatitude,\
				atol(caJytype),caCeoidtype,caCeocardid,caSdate,caEdate,caPort_start,caPort_end,atoi(caUserid));

 printf("sqlbuf=%s\n",sqlbuf);     	   
		  
       	    iReturn = pasDbExecSqlF(sqlbuf);
       	    
       	    if(iReturn==0){
            	 ncsWebReqSyscorp_v4(psShmHead,sqlbuf); 
            }
       	    
       			   sprintf(sqlbuf,"修改上网场所信息，代码：%s,显示名：%s",caUsername,caDispname);    
		       ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",sqlbuf);   
		        
        if(iReturn != 0) {
            strcpy(caMsg,"false");
            pasDbRollback(NULL);
        }
        else {
        	  strcpy(caMsg,"true");
            pasDbCommit(NULL);
        }
        lUserid = atol(caUserid);
        psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lUserid);
        if(psClient) {
            strcpy(psClient->username,caUsername);
            strcpy(psClient->dispname,caDispname);
        }

         psDbHead = utPltInitDb();
        utPltPutVar(psDbHead,"mesg",caMsg);
        utPltPutVar(psDbHead,"userid",caUserid);
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_msg.htm");
        return 0;
      }
}



//部门树AJAX通讯

int ncsTreeGroup_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[32],caGroupid[16];
   	
   	char caName[32];
   	char caWhere[256];
   	char caSql[2048];
   	char caTemp[4096];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
   	unsigned long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	long lId;
   	char *p,*pVar;
   	unsigned char *pHash;
    pasLHashInfo  sHashInfo;
    struct stData{
    	unsigned long id;
    	unsigned long count;
    };
    long lCount;
    struct stData *psData;
    
    char caGroups[1024];

    
    //获取权限组ID
   
    strcpy(caGroups,getDsGroupid());
    
    
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);

   	iReturn = utMsgGetSomeNVar(psMsgHead,4,
					"plate",      UT_TYPE_STRING, 127, caPlate,
					"node",        UT_TYPE_STRING,10,caPid,
					"fname",      UT_TYPE_STRING,63,caFname,
					"bfname",     UT_TYPE_STRING,63,caBfname);
 
    
//    iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere,"\0");




    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",500);
  
    pHash = (unsigned char *)pasLHashInit(lRowNum,lRowNum,sizeof(struct stData),0,4);
    
    if(pHash == NULL) {
             printf(" Memory HttpHash Error \n");
             return (-1);
    }
//  printf("select count(pid),pid from ncsgroup where pid in (select groupid from ncsgroup where pid=%d) group by pid limit 0,%d\n",atol(caPid),lRowNum);  
 
       if(strlen(caGroups)>0){
   
      		if(strlen(caPid)==0){
		     	 sprintf(caTemp,"select pid from ncsgroup where pid not in (%s) and groupid in (%s) ",caGroups,caGroups);
		     	 lPid=0;
		     	 pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lPid);
		     	 sprintf(caPid,"%d",lPid);
		     	}
        }
 
 
    psCur=pasDbOpenSqlF("select count(pid),pid from ncsgroup where pid in (select groupid from ncsgroup where pid=%d) group by pid limit 0,%d",atol(caPid),lRowNum);
    if(psCur==NULL){
    	free(pHash);
    	return 0;
    }
    lGroupid=0;
    lCount=0;
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                   UT_TYPE_ULONG,4,&lGroupid);
    while(iReturn == 0) { 
    	 psData = (struct stData *)pasLHashLookA(pHash,&lGroupid);    	
       if(psData){       
       	psData->count=lCount;
       }
    	lGroupid=0;
    	lCount=0;
    	iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                   UT_TYPE_ULONG,4,&lGroupid);
    }
    pasDbCloseCursor(psCur);
    
    
    lStartRec = (lCurPg - 1) * lRowNum;  
    

      if(strlen(caGroups)>0){
      	sprintf(caSql,"select groupname,groupid,pid from ncsgroup where 1=1 and groupid in (%s)",caGroups);

      	 sprintf(caSql+strlen(caSql)," and pid=%d ",atol(caPid));
      }
      else{
     	  sprintf(caSql,"select groupname,groupid,pid from ncsgroup where pid=%d ",atol(caPid));
     	}
     	sprintf(caSql+strlen(caSql)," order by groupname limit %lu,%lu",0,lRowNum);
    psCur = pasDbOpenSql(caSql,0);
   printf("caSql=%s\n",caSql);
    if(psCur == NULL) {
    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
    	  free(pHash);
        return 0;
    }
    psDbHead = utPltInitDb();
    
     
    
    
    utPltPutVar(psDbHead,"fname",caFname);
    utPltPutVar(psDbHead,"bfname",caBfname);
    memset(caGroupname,0,sizeof(caGroupname));
    lGroupid=0;
    lPid=0;
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caGroupname,
                                   UT_TYPE_ULONG,4,&lGroupid,
                                   UT_TYPE_ULONG,4,&lPid);

    iNum = 0;
    while(iReturn == 0) {
        
         if(iNum>0){
         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
         }
         iNum++;
         utPltPutLoopVar(psDbHead,"groupname", iNum,caGroupname);
         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%lu",lGroupid);
         psData = (struct stData *)pasLHashLook(pHash,&lGroupid);       
         	if(psData){
  //       		printf("id=%d,lcount=%d\n",psData->id,psData->count);
          	utPltPutLoopVar(psDbHead,"leaf", iNum,"false");
         		utPltPutLoopVar(psDbHead,"cls", iNum,"folder");
         	}
          else{
          	utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
         		utPltPutLoopVar(psDbHead,"cls", iNum,"file");
          	

          }
        

        utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);

        utPltPutLoopVarF(psDbHead,"pid",iNum,"%lu",lPid);
        utPltPutLoopVar(psDbHead,"plate",    iNum,caPlate);
        utPltPutLoopVar(psDbHead,"fname",iNum,caFname);
		    memset(caGroupname,0,sizeof(caGroupname));
		    lGroupid=0;
		    lLevel=0;
		    lPid=0;
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caGroupname,
		                                   UT_TYPE_ULONG,4,&lGroupid,
		                                   UT_TYPE_ULONG,4,&lPid);
    }
    pasDbCloseCursor(psCur);
     free(pHash);
    utPltPutVar(psDbHead,"rootname",utComGetVar_sd(psShmHead,"rootname","全部单位"));
    	
// utPltShowDb(psDbHead);
   if(strlen(caPlate)>0){
   	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
   }
   else{
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_group_tree.htm");
    	}
   

    return 0;
}


//显示全部部门 主要用户组合框选择
int ncsWebGroupComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
   	unsigned long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caQuery[256],caTemp[2024];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	long lId;
   	char *p,*pVar;
   	char caGroups[1024];
   	strcpy(caGroups,getDsGroupid());
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    
    	iReturn = utMsgGetSomeNVar(psMsgHead,1,
					"query",      UT_TYPE_STRING, 127, caQuery);
					
					
	//				printf("caQuery=%s\n",pasCvtGBK(2,caQuery,caTemp,255));
    
    
    
  //  iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere,"\0");
/* 
     memset(caOpt,0,sizeof(caOpt)); 
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    if(iReturn==0){
 	    pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
    } 

*/

    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",300);
    lStartRec = (lCurPg - 1) * lRowNum;  
    
    sprintf(caTemp,"select groupname,groupid,pid from ncsgroup where 1=1 ");
    if(strlen(caGroups)>0){
   		sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caGroups);
  	}
  	sprintf(caTemp+strlen(caTemp)," order by pid limit %lu,%lu",0,lRowNum);
  	psCur = pasDbOpenSql(caTemp,0);
printf("caTemp=%s\n",caTemp);  	
    if(psCur == NULL) {
    	  
        return 0;
    }
    psDbHead = utPltInitDb();
    
     
    
    
 
    memset(caGroupname,0,sizeof(caGroupname));
    lGroupid=0;
    lPid=0;
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caGroupname,
                                   UT_TYPE_ULONG,4,&lGroupid,
                                   UT_TYPE_ULONG,4,&lPid);
    if(!utStrIsSpaces(caPlate)) utPltPutVar(psDbHead,"plate",caPlate);
    if(!utStrIsSpaces(caFname)) utPltPutVar(psDbHead,"fname",caFname);
    iNum = 0;
    while(iReturn == 0) {

    		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
    	
        iNum++;
        
        utPltPutLoopVar(psDbHead,"groupname", iNum,caGroupname);
        utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%lu",lGroupid);
      
		    memset(caGroupname,0,sizeof(caGroupname));
		    lGroupid=0;
		    lLevel=0;
		    lPid=0;
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caGroupname,
		                                   UT_TYPE_ULONG,4,&lGroupid,
		                                   UT_TYPE_ULONG,4,&lPid);
    }
    pasDbCloseCursor(psCur);   
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_corpclass_list.htm");
      
    return 0;
}



//显示接入厂商
int ncsWebFacComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[32],caGroupid[16];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
    char caFacs[1024];
   	long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
    char caCode[16],caName[68];
    char caTemp[1024];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    
     strcpy(caFacs,getDsFacGroupid());
    


     	
    sprintf(caTemp,"select code,name from ncsfactorycod where 1 "); 
    if(strlen(caFacs)>0){
    	sprintf(caTemp+strlen(caTemp)," and code in (%s) ",caFacs);
    }
    psCur = pasDbOpenSql(caTemp,0);

    if(psCur){
    	
    	psDbHead = utPltInitDb();
    	
    memset(caCode,0,sizeof(caCode));
    memset(caName,0,sizeof(caName));
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_STRING,10,caCode,
      	                             UT_TYPE_STRING,63,caName);
    iNum=0;
    while((iReturn==0)||(iReturn==1405))
    {
    		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
    	
        iNum++;
        utPltPutLoopVar(psDbHead,"groupname", iNum,caName);
        utPltPutLoopVar(psDbHead,"groupid",   iNum,caCode);
		    memset(caCode,0,sizeof(caCode));
		    memset(caName,0,sizeof(caName));
		  	iReturn = pasDbFetchInto(psCur,
		      	                             UT_TYPE_STRING,10,caCode,
		      	                             UT_TYPE_STRING,63,caName);                              
    }    
    pasDbCloseCursor(psCur); 
   
  }
    
 
  
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_corpclass_list.htm");
      
    return 0;
}

//显示接入厂商
int ncsWebFacComp_mesg(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[32],caGroupid[16];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
    char caFacs[1024];
   	long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
    char caCode[16],caName[68];
    char caTemp[1024];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    
     strcpy(caFacs,getDsFacGroupid());
    
	psDbHead = utPltInitDb();

   if(strlen(caFacs)==0){
   	sprintf(caTemp,"select code,name from ncsfactorycod where 1 ");  
 
    psCur = pasDbOpenSql(caTemp,0);

    if(psCur){
    	
    
    	
    memset(caCode,0,sizeof(caCode));
    memset(caName,0,sizeof(caName));
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_STRING,10,caCode,
      	                             UT_TYPE_STRING,63,caName);
      	                             
    iNum=0;
      utPltPutLoopVar(psDbHead,"groupname",iNum+1,"全部");
      utPltPutLoopVar(psDbHead,"groupid",iNum+1,"");
      iNum++;
    while((iReturn==0)||(iReturn==1405))
    {
    	 if(iNum>0){
    		  utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
    	  }
    	
        iNum++;
        
        utPltPutLoopVar(psDbHead,"groupname", iNum,caName);
        utPltPutLoopVar(psDbHead,"groupid",   iNum,caCode);
		    memset(caCode,0,sizeof(caCode));
		    memset(caName,0,sizeof(caName));
		  	iReturn = pasDbFetchInto(psCur,
		      	                             UT_TYPE_STRING,10,caCode,
		      	                             UT_TYPE_STRING,63,caName);                              
    }    
    pasDbCloseCursor(psCur); 
   
  }
  }
  else{
  	    utPltPutLoopVar(psDbHead,"groupname", 1,"管理员");
        utPltPutLoopVar(psDbHead,"groupid",   1,"admin");
  }
 
  
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_facmesg_list.htm");
      
    return 0;
}

//显示单位类别
int ncsWebLbComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[32],caGroupid[16];
   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
   	long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
    char caCode[16],caName[68];
    char caTemp[1024];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    
    
    


     	
    sprintf(caTemp,"select code,name from ncsuser_lb limit 0,29 "); 
    psCur = pasDbOpenSql(caTemp,0);

    if(psCur){
    	
    	psDbHead = utPltInitDb();
    	
    memset(caCode,0,sizeof(caCode));
    memset(caName,0,sizeof(caName));
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_STRING,10,caCode,
      	                             UT_TYPE_STRING,63,caName);
    iNum=0;
    while((iReturn==0)||(iReturn==1405))
    {
    		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
    	
        iNum++;
        
        utPltPutLoopVar(psDbHead,"groupname", iNum,caName);
        utPltPutLoopVar(psDbHead,"groupid",   iNum,caCode);
		    memset(caCode,0,sizeof(caCode));
		    memset(caName,0,sizeof(caName));
		  	iReturn = pasDbFetchInto(psCur,
		      	                             UT_TYPE_STRING,10,caCode,
		      	                             UT_TYPE_STRING,63,caName);                              
    }    
    pasDbCloseCursor(psCur); 
   
  }
    
 
  
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_corpclass_list.htm");
      
    return 0;
}


//显示一级部门
int ncsWebGroupComp0(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
   	unsigned long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caQuery[256],caTemp[2024];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	long lId;
   	char *p,*pVar;
   	char caGroups[1024];
   	strcpy(caGroups,getDsGroupid());
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
   
   sprintf(caTemp,"select groupname,groupid from ncsgroup where pid=0 ");
   if(strlen(caGroups)>0){
   	sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caGroups);
  }
   
  	psCur = pasDbOpenSql(caTemp,0);
 
    if(psCur == NULL) {
    	  
        return 0;
    }
    psDbHead = utPltInitDb();
    
     
    
    
 
    memset(caGroupname,0,sizeof(caGroupname));
    lGroupid=0;
    lPid=0;
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caGroupname,
                                   UT_TYPE_ULONG,4,&lGroupid);
                   

    iNum = 0;
    while(iReturn == 0) {

    		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
    	
        iNum++;
        
        utPltPutLoopVar(psDbHead,"groupname", iNum,caGroupname);
        utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%lu",lGroupid);
      
		    memset(caGroupname,0,sizeof(caGroupname));
		    lGroupid=0;
		    lLevel=0;
		    lPid=0;
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caGroupname,
		                                   UT_TYPE_ULONG,4,&lGroupid);

    }
    pasDbCloseCursor(psCur);   
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_corpclass_list.htm");
      
    return 0;
}






//显示单位固定ip
int ncsCorpIp_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[2048];
    char caCurPg[16],caTotRec[16];
    char caSid[16],caModi[16],caDel[16],caAdd[16]; 
    char caName[64];
    unsigned long lSid,llTime,iReturn,i,iNum,lCount;
    utPltDbHead *psDbHead;
    unsigned long id,userid,lip,uip,lStime;
    char caUserid[16],caLip[16],caUip[16],caId[16];
    char caStart[12],caSort[12],caDir[12];
    char *p;
 	pasDbCursor *psCur;
//#ifdef LDEBUG 	
//    utMsgPrintMsg(psMsgHead);
//#endif    

	iReturn = utMsgGetSomeNVar(psMsgHead,4,	
			"userid",   UT_TYPE_STRING,12,caUserid,
			"start",  UT_TYPE_STRING,10,caStart,
			"sort",UT_TYPE_STRING,10,caSort,
			"dir",UT_TYPE_STRING,8,caDir);
			
	utMsgPrintMsg(psMsgHead);
	 lCount=0;
   sprintf(caTemp,"select count(*) from ncsuserip where userid=%lu ",atol(caUserid));
   pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
   
   
   
  sprintf(caTemp,"select id,inet_ntoa(lip),inet_ntoa(uip),stime from ncsuserip where userid=%lu order by %s %s limit %d,25",atol(caUserid),caSort,caDir,atol(caStart));
  printf("caTemp=%s\n",caTemp);
  psCur = pasDbOpenSql(caTemp,0);
   if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
   }
   psDbHead = utPltInitDb(); 
   utPltPutVar(psDbHead,"userid",caUserid);
   utPltPutVarF(psDbHead,"TotRec","%d",lCount);
   
   
   memset(caLip,0,sizeof(caLip));
   memset(caUip,0,sizeof(caUip));
   lStime=0;
   iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG,4,&id,
                                     UT_TYPE_STRING,15,caLip,
                                     UT_TYPE_STRING,15,caUip,
                                     UT_TYPE_LONG,4,&lStime);
                                  
    iNum = 0;                                  
    while((iReturn==0)||(iReturn==1405))
    {
        if(iNum>0){
        	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
        }
        iNum++; 
        utPltPutLoopVarF(psDbHead,"num",iNum,"%lu",iNum-1);      
        utPltPutLoopVarF(psDbHead,"id",   iNum,"%lu",id);
        if(strcmp(caLip,"255.255.255.255")!=0)
        utPltPutLoopVar(psDbHead,"lip",iNum,caLip);
        utPltPutLoopVar(psDbHead,"uip",iNum,caUip);
        utPltPutLoopVar(psDbHead,"stime",iNum,utTimFormat("%Y/%m/%d %H:%M:%S",lStime));
     memset(caLip,0,sizeof(caLip));
     memset(caUip,0,sizeof(caUip));
   
   lStime=0;
   iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG,4,&id,
                                     UT_TYPE_STRING,15,caLip,
                                     UT_TYPE_STRING,15,caUip,
                                     UT_TYPE_LONG,4,&lStime);
    }
    pasDbCloseCursor(psCur); 
  
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_corpip_list.htm");
    return 0;
}


//显示单位组列表
int ncsWebClientClassList_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512],caCond[64],caCond0[64];
    char caTotPg[16],caCurPg[16],caSort[16],caSortby[6];
    char caGroupname[32];
    char caGroupid[16];

    char caTemp[2024];
   
    char caAll[1024];
   
  
    unsigned long lGroupid,lPid,lCurPg,lTotPg;
    int lCount;
    int i,iNum,lId;
    char caGroupcode[16];

    char caSmt[4048],caOrder[64];
    char caDel[16],caExport[16];
    utPltDbHead *psDbHead;
   	int iReturn;
   	long lGroupid0,lRowNum,lUseflags,lStartRec,lUseflags0,lTotRec;
   	char servicecode1[24],dispname1[68],address1[130],ceoname1[32],handphone1[34],contact1[34],telphone1[68],email1[130];
   	char ip1[20],servicestate1[4],jointype1[4],area1[72],areaman1[32],joincode1[8];
 	  pasDbCursor *psCur;
 	  char caUsername2[32];
 	  char caMsg[1024];
 	  char caOnlineflag[16];
 	  long lMonflag=0;
 	  unsigned long lCurtime,lTime,lRuntime;
   	unsigned long lCase;
   	char caYear[30];
   	char caPlate[128];
   	char caOpt[256];
   	char caPage[16],caLimit[16];
   	char caDir[12];
    FILE *fp;
    char pFile[256],caBuf[128];
    unsigned char *pHash;
    struct stData{
    	unsigned long id;
    	char caGroupname[36];
    	long lCount;
    };
    long lCount_d;
    struct stData *psData;
   char caTemp_d[256];
   char caSesid[1024];
   long lSumHash,js_flag;
   long lGroupid3;
   char caUpdate[12],caPid[64];
   char caGroups[1024];
   char caGroupname2[36],caGroupcode2[20],caGroupid2[20];
    memset(caAll,0,sizeof(caAll));
   strcpy(caGroups,getDsGroupid());
       
    memset(caOpt,0,sizeof(caOpt)); 
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    if(iReturn==0){
 	    pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
    } 
   	
 	  dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);	  
	  iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    js_flag=0;
	
	  
	  
	 utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,13,
	                 "groupid",          UT_TYPE_STRING,10,caGroupid,
			             "groupid2",          UT_TYPE_STRING, 12, caGroupid2,  	              
   	                "sort",            UT_TYPE_STRING, 15, caSort,
   	                "dir",          UT_TYPE_STRING,8,  caDir,   	              
   	                "del",             UT_TYPE_STRING, 10,caDel,
   	                "export",          UT_TYPE_STRING,10,caExport,
   	                "groupname",       UT_TYPE_STRING,31,caGroupname2,
   	                "groupcode",       UT_TYPE_STRING,16,caGroupcode2,
   	                "page",            UT_TYPE_STRING,10,caPage,
   	                "limit",           UT_TYPE_STRING,10,caLimit,
   	                "selsid",UT_TYPE_STRING,1000,caSesid,
   	                "update",UT_TYPE_STRING,10,caUpdate,
   	                "pid",UT_TYPE_STRING,60,caPid);

    if(strlen(caGroupname2)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caGroupname2,caTemp,64));
    	strcpy(caGroupname,caTemp_d);
    }
    if(strlen(caGroupcode2)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caGroupcode2,caTemp,15));
    	strcpy(caGroupcode,caTemp_d);
    }

      if(strlen(caPid)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caPid,caTemp,64));
    	strcpy(caPid,caTemp_d);
    	
    	sprintf(caTemp,"select groupid from ncsgroup where groupname='%s' ",caPid);
    	printf("caTemp=%s\n",caTemp);
    	lGroupid3=0;
    	pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lGroupid3);
    	if(lGroupid3>0){
    		sprintf(caPid,"%d",lGroupid3);
    	}
    }



  if(strcmp(caUpdate,"update")==0){
  	lPid=atol(caPid);
  	lGroupid=atol(caGroupid2);
  	if(lGroupid>0){
	  		if((lGroupid!=lPid)&&strlen(caGroupname)>0){
	  			pasDbExecSqlF("update ncsgroup set groupname='%s',pid=%d,groupcode='%s' where groupid=%d ",caGroupname,lPid,caGroupcode,lGroupid);
	  		}
  	}
  	else{
  		 lGroupid=utMd5Code(caGroupname,strlen(caGroupname),"ffff");
       if(lGroupid>2000000000) lGroupid=lGroupid-2000000000;
       
       if(lGroupid>2000000000) lGroupid=lGroupid-2000000000;
     
      sprintf(caTemp,"select count(*) from ncsgroup where groupid=%d ",lGroupid);
      lCount=0;
  	  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
  	  if(lCount>0){
  	  	pasDbExecSqlF("update ncsgroup set groupname='%s',pid=%d,groupcode='%s' where groupid=%d ",caGroupname,lPid,caGroupcode,lGroupid);
  	  }
  	  else{
  	  	pasDbExecSqlF("insert into ncsgroup(groupid,groupname,descr,pid,groupcode) values(%d,'%s','%s',%d,'%s')",lGroupid,caGroupname,caGroupname,lPid,caGroupcode);
  	  }
  	}
  	strcpy(caGroupcode2,"");
  	strcpy(caGroupname2,"");
  	strcpy(caGroupid2,"");
  }

    if(!utStrIsSpaces(caDel)) {  /* 删除选中的项目*/
    	if(strlen(caSesid)>0){
    		printf("delete from ncsuser where groupid in(%s)\n",caSesid);
    		      iReturn = pasDbExecSqlF("delete from ncsuser where groupid in(%s)",caSesid);
    		      
    		       iReturn = pasDbExecSqlF("delete from ncsgroup where groupid in(%s)",caSesid);
    	printf("delete from ncsgroup where groupid in(%s)\n",caSesid);	      
            	sprintf(caMsg,"delete from ncsgroup where groupid in(%s) ",caSesid);
            	if(iReturn==0){
            		ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caMsg);
            	}
            	else{
            		ncsWriteSysLog(psMsgHead,caUsername2,"05","失败",caMsg);
            	}
    	}
		
    }

    lCurPg = atol(caPage);//当前页面
    if(lCurPg <= 0) lCurPg = 1 ;//如若当前页面小于0，置1
    

    	if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
  
    lStartRec = (lCurPg - 1) * lRowNum;   //当前页面的开始第一行的行数		
    
    	
    sprintf(caSmt,"select count(*) from ncsgroup where 1=1"); 
    
    if(strlen(caGroupname2)!=0){
    sprintf(caSmt+strlen(caSmt),"  and groupname like '%c%s%c' ",'%',caGroupname2,'%');
    }

    if(strlen(caGroupcode2)>0){
    	sprintf(caSmt+strlen(caSmt)," and groupcode=%s ",caGroupcode2);
    }
    if(strlen(caGroupid)>0){
   	  sprintf(caSmt+strlen(caSmt)," and groupid in (%s) ", getGroupid(atol(caGroupid)) );
    }
    if(strlen(caGroups)>0){
    	sprintf(caSmt+strlen(caSmt)," and groupid in (%s) ",caGroups);
    }

// printf("caSmt=%s\n",caSmt);   
    iReturn = pasDbOneRecord(caSmt,0,UT_TYPE_LONG,4,&lCount);
     
    
    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","用户查询","查询数据出错 ");
        return 0;
    } 
    
 //  lSumHash=lCount*1.3+1;
lSumHash=1000;
//printf("lSumHash=%d\n",lSumHash);   
     pHash = (unsigned char *)pasLHashInit(lSumHash,lSumHash,sizeof(struct stData),0,4);    
    if(pHash == NULL) {
         utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
         return (-1);
    }
   

     
    sprintf(caTemp,"select groupid,groupname from ncsgroup  ");
     psCur = pasDbOpenSql(caTemp,0);
     lGroupid=0;
     memset(caGroupname,0,sizeof(caGroupname));
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lGroupid,
      	                             UT_TYPE_STRING,32,caGroupname);
      	           
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {

    	 psData = (struct stData *)pasLHashLookA(pHash,&lGroupid);    	
       if(psData){       
       	strcpy(psData->caGroupname,caGroupname);	
      }
     	lGroupid=0;
     	memset(caGroupname,0,sizeof(caGroupname));
  		iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lGroupid,
      	                             UT_TYPE_STRING,32,caGroupname);	                              
    }    
    pasDbCloseCursor(psCur);  
    
    
    
    sprintf(caTemp,"select count(*),groupid from ncsuser group by groupid ");
    psCur = pasDbOpenSql(caTemp,0);
       lCount_d=0;
       lGroupid=0;
      	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_d,
      	                             UT_TYPE_LONG,4,&lGroupid);
      	                            
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {

    	 psData = (struct stData *)pasLHashLookA(pHash,&lGroupid);    	
       if(psData){       
       	psData->lCount=lCount_d;	
      }
       lCount_d=0;
       lGroupid=0;
      	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_d,
      	                             UT_TYPE_LONG,4,&lGroupid);                              
    }    
    pasDbCloseCursor(psCur);  
    
    
    
    
    
    
   sprintf(caSmt,"select groupid,groupname,pid,groupcode from ncsgroup where 1=1");
    if(strlen(caGroupid)>0){
   	  sprintf(caSmt+strlen(caSmt)," and groupid in (%s) ", getGroupid(atol(caGroupid)) );
    }
    if(strlen(caGroups)>0){
    	sprintf(caSmt+strlen(caSmt)," and groupid in (%s) ",caGroups);
    }
    if(strlen(caGroupname2)!=0){
    sprintf(caSmt+strlen(caSmt),"  and groupname like '%c%s%c' ",'%',caGroupname2,'%');
    }

    if(strlen(caGroupcode2)>0){
    	sprintf(caSmt+strlen(caSmt)," and groupcode=%s ",caGroupcode2);
    }
    
    if(strlen(caSort)>0){
    	sprintf(caSmt+strlen(caSmt),"  order by pid limit %lu,%lu",lStartRec,lRowNum);
    }
    else{
    		sprintf(caSmt+strlen(caSmt),"  order by pid limit %lu,%lu",lStartRec,lRowNum);
    }
  printf("caSmt=%s\n",caSmt);
 
    psCur = pasDbOpenSql(caSmt,0);
    if(psCur==NULL) {  	  
        free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    	}
    
   
    psDbHead = utPltInitDb();
    
    
      lGroupid=0;
      lPid=0;
      memset(caGroupcode,0,sizeof(caGroupcode));
       memset(caGroupname,0,sizeof(caGroupname));
   	   iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lGroupid,
   	                                  UT_TYPE_STRING,30,caGroupname,
   	                                  UT_TYPE_LONG,4,&lPid,
   	                                  UT_TYPE_STRING,10,caGroupcode);
   	iNum = 0;
   	
   	while(iReturn == 0||iReturn==1405 && iNum < lRowNum) {
        if(iNum>0){
        	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
        }
        iNum++;
        utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
        utPltPutLoopVarF(psDbHead,"groupid",iNum,"%d",lGroupid);
        utPltPutLoopVar(psDbHead,"groupcode",iNum,caGroupcode);
        utPltPutLoopVarF(psDbHead,"pid",iNum,"%d",lPid);
	
   	    psData = (struct stData *)pasLHashLook(pHash,&lPid);       
         	if(psData){
         		utPltPutLoopVar(psDbHead,"pname",iNum,psData->caGroupname);
      
         	}
         	
    	  else {
 
    	  	utPltPutLoopVar(psDbHead,"pname",iNum,"");
    	  	
    	  }
           psData = (struct stData *)pasLHashLook(pHash,&lGroupid);      
         	if(psData){
         		utPltPutLoopVarF(psDbHead,"count",iNum,"%d",psData->lCount);
         	}

      lGroupid=0;
      lPid=0;
      memset(caGroupcode,0,sizeof(caGroupcode));
       memset(caGroupname,0,sizeof(caGroupname));
   	   iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lGroupid,
   	                                  UT_TYPE_STRING,30,caGroupname,
   	                                  UT_TYPE_LONG,4,&lPid,
   	                                  UT_TYPE_STRING,10,caGroupcode);
    }
    pasDbCloseCursor(psCur);
    lTotPg = (lCount - 1) / lRowNum + 1;
    lTotRec = lCount;
    i = 0;
   
    utPltPutVar(psDbHead,"sort", caSort);
    utPltPutVar(psDbHead,"sortby", caSortby);

    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    utPltPutVar(psDbHead,"groupid",caGroupid);
  
    if(pHash) free(pHash);
   

    	ncsWriteSysLog(psMsgHead,caUsername2,"06","成功","查看上网服务场所列表信息"); 
    	
  
     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_dwfzgl_list.htm");
    

    return 0;
}

/* 显示部门修改界面页面  */
int ncsWebDispDeptForm_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
   	char caGroupname[34];
   	char caTemp1[1024];
   	int iNum1,i,lGid;
   	long lUserid,lGroupid;
   	char *pBuf,*pSelect;
    utPltDbHead *psDbHead;
    char caTemp[1024];
    pasDbCursor *psCur;
    long groupid,iNum;
    char groupname[32];
   	int iReturn;
    char caGid[16];
    char caPlate[128];
    long lGroupid0,group_flag;
    char caVar[256],caFname[256];
    long lPid;
    char caGroupid[16],caPid[16],caGroupcode[16];
 
    
   	iReturn = utMsgGetSomeNVar(psMsgHead,2,   	     
					"groupid",      UT_TYPE_STRING,10,caGroupid,
					"pid",        UT_TYPE_STRING,10,caPid);	
           psDbHead = utPltInitDb();
   
    psCur = pasDbOpenSql("select groupid,groupname from ncsgroup order by groupid desc ",0);
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&groupid,
      	                               UT_TYPE_STRING,31,groupname);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%lu",groupid);
        utPltPutLoopVar(psDbHead,"groupname",iNum,groupname);
        iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_LONG,4,&groupid,
      	                               UT_TYPE_STRING,31,groupname);
    }
    pasDbCloseCursor(psCur);
      
      
   if(strlen(caGroupid)>0){
    		sprintf(caTemp,"select groupname,pid,groupcode from ncsgroup where groupid=%s  ",caGroupid);
printf("caTemp=%s\n",caTemp);
        memset(caGroupcode,0,sizeof(caGroupcode));

    	  iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_STRING,31,caGroupname,
    	                        UT_TYPE_LONG,4,&lPid,
    	                        UT_TYPE_STRING,14,caGroupcode);
        utPltPutVar(psDbHead,"groupname",caGroupname);
        utPltPutVar(psDbHead,"groupcode",caGroupcode);
        utPltPutVarF(psDbHead,"pid","%d",lPid);
    	
    }
    else{
		     utPltPutVar(psDbHead,"pid",caPid);	    
    }
    

    	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/nc_form_group_v4.htm");
    return 0;
}


//单位树AJAX通讯
int ncsTreeUser_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[72],caGroupid[16];
   	char caTemp[4024];
   	char caName[32];
   	char caWhere[256];

   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
   	unsigned long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	char caGroups[1024];
   	char caFacs[1024];
   	long lId;
   	char *p,*pVar;
   	unsigned char *pHash;
   	char caCname[72];
    char caUsername[36];
    pasLHashInfo  sHashInfo;
    struct stData{
    	unsigned long id;
    	unsigned long count;
    };
    long lCount;
    struct stData *psData;
    char caCname1[72];
    char caSql[2048];
    char caWireflag[20];
    char caFaccods[1024];
  
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
      strcpy(caGroups,getDsGroupid());
      strcpy(caFacs,getDsFacGroupid());
      printf("caFacs=%s\n",caFacs);
//      strcpy(caFaccods,getDsFacs()));
    memset(caCname,0,sizeof(caCname));
   	iReturn = utMsgGetSomeNVar(psMsgHead,4,
					"node",        UT_TYPE_STRING,10,caPid,
					"cname",       UT_TYPE_STRING,30,caCname1,
					"plate",       UT_TYPE_STRING,100,caPlate,
					"wireflag",    UT_TYPE_STRING,10,caWireflag);
//部门				
pHash = (unsigned char *)pasLHashInit(1000,1000,sizeof(struct stData),0,4);  	
if(pHash==NULL) return 0;
	
	psCur=pasDbOpenSql("select pid from ncsgroup where pid!=0 ",0);
	if(psCur){
		iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lPid);
		while(iReturn==0){
			 psData = (struct stData *)pasLHashLookA(pHash,&lPid);
			 if(psData){
			 	psData->count++;
			} 
			 iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lPid);
		}	
	  pasDbCloseCursor(psCur);
	}		
	
	//单位

	psCur=pasDbOpenSql("select distinct groupid from ncsuser  ",0);
	if(psCur){
		iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lPid);
		while(iReturn==0){
			 psData = (struct stData *)pasLHashLookA(pHash,&lPid);
			 if(psData){
			 	psData->count++;
			} 
			 iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lPid);
		}	
	  pasDbCloseCursor(psCur);
	}					
				
    if(strlen(caCname1)>0){
    	strcpy(caCname,pasCvtGBK(2,caCname1,caTemp,30));
    }
//    iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere,"\0");

     memset(caOpt,0,sizeof(caOpt)); 
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    if(iReturn==0){
     pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
    } 

    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",300);
  
    
 
    iNum = 0;
    
     psDbHead = utPltInitDb();
    if(strlen(caCname)==0){
		    lStartRec = (lCurPg - 1) * lRowNum;  
		     sprintf(caTemp,"select groupname,groupid,pid from ncsgroup where 1=1 ");
		     if(strlen(caGroups)>0){
		     	  if(strlen(caPid)==0){
		     	 sprintf(caSql,"select pid from ncsgroup where pid not in (%s) and groupid in (%s) ",caGroups,caGroups);
		     	 lPid=0;
		     	 pasDbOneRecord(caSql,0,UT_TYPE_LONG,4,&lPid);
		     	 sprintf(caPid,"%d",lPid);
		     	}
		     	sprintf(caTemp+strlen(caTemp),"and groupid in (%s) ",caGroups);
		    }
	//	    if(strlen(caFacs)>0){
	//	    	sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caFacs);
	//	    }
		    sprintf(caTemp+strlen(caTemp)," and pid=%d ",atol(caPid));
		    sprintf(caTemp+strlen(caTemp)," order by groupname limit %lu,%lu ",0,lRowNum);
		    printf("caTemp=%s\n",caTemp);
		    psCur=pasDbOpenSql(caTemp,0);
		  
		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
		    
		     
		
		    memset(caGroupname,0,sizeof(caGroupname));
		    lGroupid=0;
		    lPid=0;
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
		                                   UT_TYPE_LONG,4,&lGroupid,
		                                   UT_TYPE_LONG,4,&lPid);
		
		 
		    while(iReturn == 0) {
		      psData = (struct stData *)pasLHashLook(pHash,&lGroupid);       
	        	if(psData){   
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  
		         utPltPutLoopVar(psDbHead,"groupname", iNum,utStrGetCharHan(caGroupname,48));
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%ld",lGroupid);
		       
	       
		          	utPltPutLoopVar(psDbHead,"leaf", iNum,"false");
		         		utPltPutLoopVar(psDbHead,"cls", iNum,"folder");
		         		
	        	
  //      else{
	//	        	utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
	//       		  utPltPutLoopVar(psDbHead,"cls", iNum,"folder");
		          		
//	         }
		        utPltPutLoopVar(psDbHead,"dtype",iNum,"2");
		
		        utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
		
		        utPltPutLoopVarF(psDbHead,"pid",iNum,"%d",lPid);
		       }
				    memset(caGroupname,0,sizeof(caGroupname));
				    lGroupid=0;
				    lLevel=0;
				    lPid=0;
				    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
				                                   UT_TYPE_LONG,4,&lGroupid,
				                                   UT_TYPE_LONG,4,&lPid);
		    }
		    pasDbCloseCursor(psCur);
		//     free(pHash);
		    utPltPutVar(psDbHead,"rootname",utComGetVar_sd(psShmHead,"rootname","全部单位"));
    	
     }
    	sprintf(caTemp,"select dispname,userid,groupid,username from ncsuser where 1=1 ");
    	if(strlen(caGroups)>0){
    		sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caGroups);
    	}
    	if(strlen(caFacs)>0){
    		sprintf(caTemp+strlen(caTemp)," and fcode in (%s) ",caFacs);
    	}
    	 if(strlen(caWireflag)>0){
    	if(strcmp(caWireflag,"2")==0){
    	   sprintf(caTemp+strlen(caTemp)," and (jointype='08' or jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
      	sprintf(caTemp+strlen(caTemp)," and jointype!='08' ");
      }
    
    }
    	
    	
    if(strlen(caCname)==0){
    	   
		    	if(strlen(caPid)==0){
		    		  sprintf(caTemp+strlen(caTemp)," and groupid not in (select groupid from ncsgroup ) ");
		    		  
		    	}
		    	else{
		    		  sprintf(caTemp+strlen(caTemp)," and groupid=%d ",atol(caPid));
		    		 		
		    	}
     }
     else{
     	   sprintf(caTemp+strlen(caTemp),"and (dispname like '%c%s%c' or username like '%c%s%c') ",'%',caCname,'%','%',caCname,'%');
     	  
     	 
     	
     }
      sprintf(caTemp+strlen(caTemp)," order by dispname limit %lu,%lu",0,lRowNum);
      printf("caTemp=%s\n",caTemp);
       psCur = pasDbOpenSql(caTemp,0);
    	 if(psCur!=NULL){
    	    memset(caGroupname,0,sizeof(caGroupname));
			    lGroupid=0;
			    lPid=0;
			    memset(caUsername,0,sizeof(caUsername));
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
			                                   UT_TYPE_LONG,4,&lGroupid,
			                                   UT_TYPE_LONG,4,&lPid,
			                                   UT_TYPE_STRING,15,caUsername);
			
			    while(iReturn == 0) {
			        
			         if(iNum>0){
			         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
			         }
			         iNum++;
			         utPltSetCvtHtml(1);  
			         utPltPutLoopVar(psDbHead,"groupname", iNum,utStrGetCharHan(caGroupname,48));
			         utPltSetCvtHtml(0);  
			         utPltPutLoopVarF(psDbHead,"did",   iNum,"%ld",lGroupid);

			          	utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
			         		utPltPutLoopVar(psDbHead,"cls", iNum,"file");
			          	
			            utPltPutLoopVar(psDbHead,"dtype",iNum,"0");
			            utPltPutLoopVar(psDbHead,"code",iNum,caUsername);
			        
			
			        utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			
			        utPltPutLoopVarF(psDbHead,"pid",iNum,"%d",lPid);
			
					    memset(caGroupname,0,sizeof(caGroupname));
					    lGroupid=0;
					    lLevel=0;
					    lPid=0;
			    memset(caUsername,0,sizeof(caUsername));
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
			                                   UT_TYPE_LONG,4,&lGroupid,
			                                   UT_TYPE_LONG,4,&lPid,
			                                   UT_TYPE_STRING,15,caUsername);
			    }
			    pasDbCloseCursor(psCur);
    	  }
    	
    	
// utPltShowDb(psDbHead);
free(pHash);
   if(strlen(caPlate)>0){
   	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
  }
  else{
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_user_tree.htm");
    	}
   

    return 0;
}


//inforbright  表ncsuserlog海量查询
int ncsUserlogSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	  char caTitlename[64];
	  int group_flag;
    char caWhere[512];
    char caStime0[6];
    char caEtime[6];
    char caStemptime[17];
    char caEtemptime[17];
    char caUserid[17];
    long lstime=0;
    long letime=0;
    char caTotPg[16],caCurPg[16];
    char caUsername[32],caSdate[32];
    char caTemp[1024],caGroupid[12],caMac[20];
    char caEdate[20];
    char caSip[16],caDip[16],caSport[10],caDport[10];
    unsigned long lUserid,lGroupid,lCurPg,lTotPg,lUserid0;
    unsigned long lTime,l,lTime1,lSip,lDip;
    int i,iNum;
    utPltDbHead *psDbHead;
   	int iReturn;
   	char caDispname[32],caProt[12],caService[12];
   	char caStime[32];
   	char caIdfyno[20];
    char caIp[20];
  
   	float lRat;
   	long lGroupid0,lRowNum,lflags,lStartRec,lCount,lStatus1;
	int     lSumUser;
	char sqlbuf[20480]="";
	unsigned long lStatrow,lMintime,lMaxtime,lSum,lStarttime,lEndtime,lMaxin,lMinin,lIn,lVtime;
   	unsigned long lConntime,lService,lPort,lType;
   	long long lUbytes,lDbytes;
   char caPosition[36];
   long lStime,lEtime;
 	pasDbCursor *psCur;
 
 	char caTable_hz[36];
 	char caSdate_d[16],caEdate_d[16];
 	char caTable[20],caUdisp[36];
 	long starttime;
  char caIdno[24],caCname[64],caKeyword[64],caTime_flag[16],caField_flag[16],caIdtype[8];
  char caDid[16],caPage[12],caLimit[16],caSort[16],caDir[16];
  long id,lDid,lUid,lFlag;
  char caName[36];
  char caUid[16],caServicecode[24],caTrantype[16],caPort[16],caCond[1024];
  char caExport[16];
  long mCount,lCmonth;

  long long lBytes;
  char caTemp_d[68];
  lCurPg=1;
 	strcpy(caWhere,"\0");

 	
 	lStatrow=atol(utComGetVar_sd(psShmHead,"statrow","10000"));
 	lSum=0;
 	lMintime=0;
 	lMaxtime=0;

  memset(caTitlename,0,sizeof(caTitlename));
 

 	
	iReturn = utMsgGetSomeNVar(psMsgHead,21,
	    "TotPg",      UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      UT_TYPE_STRING, 12, caCurPg,
			"did",UT_TYPE_STRING,12,caDid,
			"groupid",UT_TYPE_STRING,12,caGroupid,
			"sip",UT_TYPE_STRING,15,caSip,
			"idno",UT_TYPE_STRING,20,caIdno,
			"cname",UT_TYPE_STRING,60,caCname,
   	  "servicecode",UT_TYPE_STRING,14,caServicecode,
			"dispname",UT_TYPE_STRING,63,caDispname,
			"sdate",UT_TYPE_STRING,19,caSdate,
			"edate",UT_TYPE_STRING,19,caEdate,
			"position",UT_TYPE_STRING,30,caPosition,
			"keyword", UT_TYPE_STRING,60,caKeyword,
			"mac",UT_TYPE_STRING,19,caMac,
   	  "export1",     UT_TYPE_STRING,10,caExport,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag,
   	  "field_flag",   UT_TYPE_STRING,8,caField_flag);
    
    
      if(strlen(caCname)>0){
      	 strcpy(caTemp_d,pasCvtGBK(2,caCname,caTemp,64));
    	   strcpy(caCname,caTemp_d);
      }
      if(strlen(caServicecode)>0){
      	 strcpy(caTemp_d,pasCvtGBK(2,caServicecode,caTemp,14));
    	   strcpy(caServicecode,caTemp_d);
      }
      if(strlen(caDispname)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caDispname,caTemp,30));
      	strcpy(caDispname,caTemp_d);
      }
      if(strlen(caPosition)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caPosition,caTemp,30));
      	strcpy(caPosition,caTemp_d);
      }
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
			     sprintf(caEtemptime,"%s 23:59",caEdate,caEtime);
			     letime=utTimStrToLong("%Y/%m/%d %H:%M",caEtemptime);
        }
   	  }
   

 
   utStrDelSpaces(caCname);           
   utStrDelSpaces(caServicecode);  
   utStrDelSpaces(caDispname);           
   utStrDelSpaces(caKeyword);  
   utStrDelSpaces(caPosition);           

   
   
   
	 memset(caWhere,0,sizeof(caWhere));
	 
	    lCmonth=0;   
nn:
	 
	 
	 sprintf(caWhere,"where stime>=%d and stime<=%d ",lstime,letime); 
   if(!utStrIsSpaces(caDispname)) {
	     sprintf(caWhere+strlen(caWhere)," and (username like '%c%s%c' or dispname like '%c%s%c') ",'%',caDispname,'%','%',caDispname,'%');       
	  }
    if(!utStrIsSpaces(caMac)) {
    	 sprintf(caWhere+strlen(caWhere)," and mac='%s' ",caMac);
    	}
   if(atol(caDid)>0){
	  	  utStrAddF(caWhere," and did=%lu ",atol(caDid));
	  	}
    if(atol(caGroupid)>0){
    	utStrAddF(caWhere," and tid=%lu ",atol(caGroupid));
    }
    if(strlen(caIdno)!=0){
     	sprintf(caWhere+strlen(caWhere)," and idno like '%c%s%c' ",'%',caIdno,'%');
    }
    

    if(!utStrIsSpaces(caSip)) {

       utStrAddF(caWhere," and sip like '%c%s%c' ", '%',caSip,'%');
   
    }

    if(!utStrIsSpaces(caPosition)) {
        utStrAddF(caWhere," and position like '%c%s%c' ",'%',caPosition,'%');
      }
   
    if(strlen(caServicecode)>0){
    	
    	utStrAddF(caWhere," and servicecode like '%c%s%c' ",'%',caServicecode,'%');
    }	
    if(strlen(caCname)>0){
    	utStrAddF(caWhere," and did in (select userid from ncsuser where dispname like '%c%s%c' )",'%',caCname,'%');
    }
    if(strlen(caKeyword)>0){
    	 if(atol(caField_flag)==1){
    	 	 	 sprintf(caWhere+strlen(caWhere)," and (username like '%c%s%c' or dispname like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');   
    	 }
    	 else if(atol(caField_flag)==2){
    	 	  utStrAddF(caWhere," and sip like '%c%s%c' ", '%',caKeyword,'%');
    	 }
    	 else if(atol(caField_flag)==3){
    	 	 utStrAddF(caWhere," and mac like '%c%s%c' ",'%',caKeyword,'%');
    	 	}
    	 	else if(atol(caField_flag)==4){
    	 		sprintf(caWhere+strlen(caWhere)," and idno like '%c%s%c' ",'%',caKeyword,'%');
    	 	}
    	 	else if(atol(caField_flag)==5){
    	 		utStrAddF(caWhere," and servicecode like '%c%s%c' ",'%',caKeyword,'%');
    	 	}
    	 	else if(atol(caField_flag)==6){
    	 		utStrAddF(caWhere," and position like '%c%s%c' ",'%',caKeyword,'%');
    	 	}
    }
    	
    psDbHead = utPltInitDb();
  
  
    
    if(strlen(caDid)>0){
    	strcpy(caTitlename,ncsUtlGetClientNameById(psShmHead,atol(caDid),"未知"));
    }
    else if(strlen(caGroupid)>0){
    	sprintf(caTemp,"select groupname from ncsgroup where groupid=%s ",caGroupid);
    	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,32,caTitlename);
    }
    utPltPutVar(psDbHead,"title_name",caTitlename);
  		
    
    
    
  printf("caWhere=%s\n",caWhere);  
    
    	
    	
   lSum=0;
   iNum=0;
   for(starttime=lstime;starttime<letime;starttime+=2764800){
   	sprintf(caTable,"ncsuserlog_if_%s",utTimFormat("%Y%m",starttime));
    sprintf(caTemp,"select count(*) from %s ",caTable);
    strcat(caTemp,caWhere);
    lCount=0;
//    printf("caTemp=%s\n",caTemp);
    iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
 //  	printf("table=%s,iReturn=%d,lCount=%d\n",caTable,iReturn,lCount);
   	if(iReturn==1146||lCount==0){
 		continue;
   	}
   	lSum=lSum+lCount;
   	iNum++;
   	if(iNum==1){
   			sprintf(sqlbuf,"(select stime,did,uid,dispname,sip,position,sumflow,etime,mac,servicecode,idno,idtype,flags from %s ",caTable);
        strcat(sqlbuf,caWhere);
        sprintf(sqlbuf+strlen(sqlbuf)," order by stime %s limit %d,%d ",caDir,lStartRec,lRowNum);  
        strcat(sqlbuf," )");
    }
    else{
    	  sprintf(sqlbuf+strlen(sqlbuf)," union all (select stime,did,uid,dispname,sip,position,sumflow,etime,mac,servicecode,idno,idtype,flags from %s ",caTable);
    	   strcat(sqlbuf,caWhere);
    	    sprintf(sqlbuf+strlen(sqlbuf)," order by stime %s limit %d,%d ",caDir,lStartRec,lRowNum); 
        strcat(sqlbuf," )");
    }
  }
 
   
    lStartRec = (lCurPg - 1) *lRowNum;  
 
  if(iNum>0){
  	sprintf(sqlbuf+strlen(sqlbuf)," order by stime %s limit %d,%d ",caDir,lStartRec,lRowNum);  
   
  }    
  else{
  	 utPltPutVarF(psDbHead,"TotRec", "%lu", 0);
     utPltPutVarF(psDbHead,"TotPg", "%lu",0);
     utPltPutVarF(psDbHead,"CurPg", "%lu", 1);

     utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
     return 0;
  }	              

    
   	              
 
 //   strcpy(caDispname,ncUtlBufGetUsername(psShmHead,atol(caUserid)));  	              
                 	              

	
printf("sqlbuf=%s\n",sqlbuf);	 
    
       
    psCur=pasDbOpenSql(sqlbuf,0);
    if(psCur == NULL) {
       utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_space.htm",ncsLang("0746用户查询"),ncsLang("0172查询数据出错 %s"),pasDbErrorMsg(NULL));
        return 0;
    }
    
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncsuserlog%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
         
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
        
        fprintf(fp,"上下线日志\n");
      ncUtlPrintCsv(fp,8,UT_TYPE_STRING,ncsLang("0906单位名称"),
                       UT_TYPE_STRING,ncsLang("0692姓名"),
                       UT_TYPE_STRING,ncsLang("0926开始时间"),
                       UT_TYPE_STRING,"结束时间",
                       UT_TYPE_STRING,"位置或房间号",
                       UT_TYPE_STRING,"MAC地址",
                       UT_TYPE_STRING,"IP地址",
                       UT_TYPE_STRING,"流量");
                       
                                      
			    lStime=0;
			    lDid=0;
			    lUid=0;
			    memset(caDispname,0,sizeof(caDispname));
			    memset(caSip,0,sizeof(caSip));
			    memset(caPosition,0,sizeof(caPosition));
			    lBytes=0;
			    lEtime=0;
			    memset(caMac,0,sizeof(caMac));
			    memset(caServicecode,0,sizeof(caServicecode));
			    memset(caIdno,0,sizeof(caIdno));
			    memset(caIdtype,0,sizeof(caIdtype));
			    lFlag=0;
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lStime,
			    															 UT_TYPE_LONG,4,&lDid,
			    															 UT_TYPE_LONG,4,&lUid,
			    															 UT_TYPE_STRING,31,caDispname,
			    															 UT_TYPE_STRING,15,caSip,
			    															 UT_TYPE_STRING,31,caPosition,
			    															 UT_TYPE_LONG,8,&lBytes,
			    															 UT_TYPE_LONG,4,&lEtime,
			    															 UT_TYPE_STRING,18,caMac,
			    															 UT_TYPE_STRING,14,caServicecode,
			    															 UT_TYPE_STRING,20,caIdno,
			    															 UT_TYPE_STRING,3,caIdtype,
			    															 UT_TYPE_LONG,4,&lFlag);
			    															 
  	                              
        while((iReturn == 0)||(iReturn==1405)) { 
       
        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S", lStime));
       strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S", lEtime));
	      
        ncUtlPrintCsv(fp,8,UT_TYPE_STRING,ncsUtlGetClientNameById(psShmHead,lDid,"未知"),
                       UT_TYPE_STRING,caDispname,
                       UT_TYPE_STRING,caSdate,
                       UT_TYPE_STRING,caEdate,
                       UT_TYPE_STRING,caPosition,
                       UT_TYPE_STRING,caMac,
                       UT_TYPE_STRING,caSip,
                       UT_TYPE_LONG8,lBytes);
                      
                       
			    lStime=0;
			    lDid=0;
			    lUid=0;
			    memset(caDispname,0,sizeof(caDispname));
			    memset(caSip,0,sizeof(caSip));
			    memset(caPosition,0,sizeof(caPosition));

			    lEtime=0;
			    memset(caMac,0,sizeof(caMac));
			   
			    memset(caServicecode,0,sizeof(caServicecode));
			    memset(caIdno,0,sizeof(caIdno));
			    memset(caIdtype,0,sizeof(caIdtype));
			    lFlag=0;
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lStime,
			    															 UT_TYPE_LONG,4,&lDid,
			    															 UT_TYPE_LONG,4,&lUid,
			    															 UT_TYPE_STRING,31,caDispname,
			    															 UT_TYPE_STRING,15,caSip,
			    															 UT_TYPE_STRING,31,caPosition,
			    															 UT_TYPE_LONG,8,&lBytes,
			    															 UT_TYPE_LONG,4,&lEtime,
			    															 UT_TYPE_STRING,18,caMac,
			    															 UT_TYPE_STRING,14,caServicecode,
			    															 UT_TYPE_STRING,20,caIdno,
			    															 UT_TYPE_STRING,3,caIdtype,
			    															 UT_TYPE_LONG,4,&lFlag);
        }
        pasDbCloseCursor(psCur);
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);

        return 0;
    }
    
    

  

     
    iReturn = 0;
    lIn=0;
    
			    lStime=0;
			    lDid=0;
			    lUid=0;
			    memset(caDispname,0,sizeof(caDispname));
			    memset(caSip,0,sizeof(caSip));
			    memset(caPosition,0,sizeof(caPosition));
          lBytes=0;
			    lEtime=0;
			    memset(caMac,0,sizeof(caMac));
			   
			    memset(caServicecode,0,sizeof(caServicecode));
			    memset(caIdno,0,sizeof(caIdno));
			    memset(caIdtype,0,sizeof(caIdtype));
			    lFlag=0;
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lStime,
			    															 UT_TYPE_LONG,4,&lDid,
			    															 UT_TYPE_LONG,4,&lUid,
			    															 UT_TYPE_STRING,31,caDispname,
			    															 UT_TYPE_STRING,15,caSip,
			    															 UT_TYPE_STRING,31,caPosition,
			    															 UT_TYPE_LONG,8,&lBytes,
			    															 UT_TYPE_LONG,4,&lEtime,
			    															 UT_TYPE_STRING,18,caMac,
			    															 UT_TYPE_STRING,14,caServicecode,
			    															 UT_TYPE_STRING,20,caIdno,
			    															 UT_TYPE_STRING,3,caIdtype,
			    															 UT_TYPE_LONG,4,&lFlag);
    															
                                   
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
	        utPltPutLoopVar(psDbHead,"dispname", iNum, caDispname);
          utPltPutLoopVar(psDbHead,"position",iNum,caPosition);
	        utPltPutLoopVar(psDbHead,"cname",iNum,ncsUtlGetClientNameById(psShmHead,lDid,"未知"));
	        utPltPutLoopVar(psDbHead,"idtype",iNum,ncsGetIdnoNameByType(caIdtype));
	        if(lFlag==1){
	        	utPltPutLoopVar(psDbHead,"fun",iNum,"上线");
	        }
	        else if(lFlag==2){
	        	utPltPutLoopVar(psDbHead,"fun",iNum,"下线");
	        }
	            
	        utPltSetCvtHtml(0);
	      
         
        strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M:%S", lStime));
        strcpy(caEdate,utTimFormat("%Y-%m-%d %H:%M:%S", lEtime));
        utPltPutLoopVar(psDbHead,"stime", iNum,caSdate);
        utPltPutLoopVar(psDbHead,"etime",iNum,caEdate);
        utPltPutLoopVarF(psDbHead,"num",iNum,"%d",iNum-1);
        utPltPutLoopVar(psDbHead,"sip",iNum,caSip);
        utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
        utPltPutLoopVar(psDbHead,"idno",iNum,caIdno);
        utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
        
       
        utPltPutLoopVarF(psDbHead,"flow",iNum,"%lu",lBytes);
       
 
        utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",lDid);
        utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",lUid);
     
			    lStime=0;
			    lDid=0;
			    lUid=0;
			    memset(caDispname,0,sizeof(caDispname));
			    memset(caSip,0,sizeof(caSip));
			    memset(caPosition,0,sizeof(caPosition));
			    lBytes=0;
			    lEtime=0;
			    memset(caMac,0,sizeof(caMac));
			   
			    memset(caServicecode,0,sizeof(caServicecode));
			    memset(caIdno,0,sizeof(caIdno));
			    memset(caIdtype,0,sizeof(caIdtype));
			    lFlag=0;
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lStime,
			    															 UT_TYPE_LONG,4,&lDid,
			    															 UT_TYPE_LONG,4,&lUid,
			    															 UT_TYPE_STRING,31,caDispname,
			    															 UT_TYPE_STRING,15,caSip,
			    															 UT_TYPE_STRING,31,caPosition,
			    															 UT_TYPE_LONG,8,&lBytes,
			    															 UT_TYPE_LONG,4,&lEtime,
			    															 UT_TYPE_STRING,18,caMac,
			    															 UT_TYPE_STRING,14,caServicecode,
			    															 UT_TYPE_STRING,20,caIdno,
			    															 UT_TYPE_STRING,3,caIdtype,
			    															 UT_TYPE_LONG,4,&lFlag);
    }



    lTotPg = (lSum -1 ) / lRowNum + 1;
 
    utPltPutVarF(psDbHead,"TotRec", "%lu", lSum);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
       
    pasDbCloseCursor(psCur);
    
        

 
  
    
 //输出翻页标记   
    
//      strcpy(caTemp,ncsGetPagBuf(lCurPg,lTotPg,lCount));     
//      utPltPutVar(psDbHead,"page",caTemp);
//      utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlog_list.htm");
    return 0;
}



/*统计在线单位分布饼图*/
int ncs_onlinecorp_first_pie(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
//    pasHashInfo sHashInfo;
    ncsOnline *psOnline;
    ncsClient *psUser;
    char *pHash;
  	char caTemp[1024];
  	char caGroupname[32];
  	pasDbCursor *psCur;
  	int lCount_area,lCount_corp,lCount_zx,lCount_lx,lCount_yc,lCount_user,lCount_useron;
    struct areastatdata_s *pAreastatdata;
    utPltDbHead *psDbHead;
   	int iReturn,i,lStart,lGroupid,lCount_g,index,iNum,lCurPg,lTotPg;
    char caTotPg[16],caCurPg[16],caExport[12];
    int lSum_did,lSum_ondid,lSum_ycdid,lSum_lxdid,lSum_endnum,lSum_onuid;
    char caOpt[256];
    unsigned long lId,lPid;
    char caUsername2[32];
    char caGroupid[16];
    FILE *fp;
    char pFile[256],caBuf[128];
    char caAll[1024];
    char caNid[10240],caSid[16];
    struct areastatdata_s *psData,*psData2,*psData3;
    unsigned char *pHash_dep;
    pasLHashInfo sHashInfo,sHashInfo2;
    long lSum2,m,n,iCount=0,iCount1=0;
    long lUserid;
    memset(caAll,0,sizeof(caAll));
 
     utMsgPrintMsg(psMsgHead); 
 
   sprintf(caTemp,"select uid from ncsexcpgroup limit 0,300 ");
  
  psCur = pasDbOpenSql(caTemp,0);
  memset(caNid,0,sizeof(caNid));
  iNum=0;
  if(psCur != NULL) {
  	   lUserid=0;
    	 iReturn = pasDbFetchInto(psCur,    	                             
      	                             UT_TYPE_LONG,4,&lUserid);
      	while((iReturn==0)||(iReturn==1405))
        { 
        	if(iNum==0){
        		sprintf(caNid,"%d",lUserid);
        	}
        	else{
        		sprintf(caNid+strlen(caNid),",%d",lUserid);
        	}
        	iNum++;
        	     	 iReturn = pasDbFetchInto(psCur,    	                             
      	                             UT_TYPE_LONG,4,&lUserid);
        }                       
      	pasDbCloseCursor(psCur);                             
   }
                       
    	                              

 
 
    
    
    dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
    memset(caOpt,0,sizeof(caOpt)); 
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    if(iReturn==0){
 	    pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
    } 
   
   
   
    lSum_did=0;
    lSum_ondid=0;
    lSum_ycdid=0;
    lSum_lxdid=0;
    lSum_endnum=0;
    lSum_onuid=0;
    
    lCount_corp=0;
    lCount_zx=0;
    lCount_lx=0;
    lCount_yc=0;
    lCount_user=0;
    lCount_useron=0;
    
    
  
    
    	iReturn = utMsgGetSomeNVar(psMsgHead,2,
	    "groupid",      UT_TYPE_STRING, 10, caGroupid,
   	  "exp",     UT_TYPE_STRING,10,caExport);
 
    lCurPg = atol(caCurPg);
    if(lCurPg <= 0) lCurPg = 1;
    lStart = (lCurPg - 1) * 28;  
   
     	pHash_dep = (unsigned char *)pasLHashInit(1000,1000,sizeof(struct areastatdata_s),4,4);    
    if(pHash_dep == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
   
   
   

  
  sprintf(caTemp,"select b.groupid,b.groupname,b.pid from  ncsgroup b where  b.groupid!=0 ");
   if(!strstr(caOpt,"all")&&(strlen(caOpt)>0)){
			sprintf(caTemp+strlen(caTemp)," and (b.groupid in (%s) or b.groupid in (select groupid from ncsgroup where pid in (%s))) ",caOpt,caOpt);
   }
  if(strlen(caGroupid)>0){
  	sprintf(caTemp+strlen(caTemp)," and (b.groupid=%s or b.groupid in (select groupid from ncsgroup where pid=%s))",caGroupid,caGroupid);
  }
   
  sprintf(caTemp+strlen(caTemp)," group by b.pid,b.groupid,b.groupname order by b.groupid limit 0,999");
  printf("caTemp=%s\n",caTemp);
  psCur = pasDbOpenSql(caTemp,0);
  if(psCur == NULL) {
    	if(pAreastatdata) free(pAreastatdata);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
   }
  lGroupid=0;
  lCount_g=0;
  lPid=0;
  i=0;
  memset(caGroupname,0,sizeof(caGroupname));
  iReturn = pasDbFetchInto(psCur,    	                             
      	                             UT_TYPE_LONG,4,&lGroupid,
      	                             UT_TYPE_STRING,30,caGroupname,
      	                             UT_TYPE_LONG,4,&lPid);
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
    		 psData = (struct areastatdata_s *)pasLHashLookA(pHash_dep,&lGroupid);  
       	 if(psData){
       	 	psData->pid=lPid;
       	 	psData->did_count=0;
       	 	strcpy(psData->groupname,caGroupname);
          psData->ondid_count=0;
          psData->ycdid_count=0;
          psData->ondid_count=0;
          psData->onuid_count=0;
          psData->endnum=0;
       	 }
    	 	
		  lGroupid=0;
		  lCount_g=0;
  		memset(caGroupname,0,sizeof(caGroupname));
  		lPid=0;
      iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lGroupid,
      	                             UT_TYPE_STRING,30,caGroupname,
      	                             UT_TYPE_LONG,4,&lPid); 	                              
    }    
    pasDbCloseCursor(psCur);  
    
    //统计各地区单位数
    sprintf(caTemp,"select count(*),groupid from ncsuser where groupid!=0 ");
    if(strlen(caNid)>0){
    	sprintf(caTemp+strlen(caTemp)," and userid not in (%s) ",caNid);
    }
    sprintf(caTemp+strlen(caTemp)," group by groupid ");
    if(!strstr(caOpt,"all")&&(strlen(caOpt)>0)){
	//		sprintf(caTemp+strlen(caTemp)," and a.groupid in (%s) ",caOpt);
    }
 
      printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
    	if(pHash_dep) free(pHash_dep);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lGroupid=0;
  	lCount_g=0;
  	i=0;
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_g,
      	                             UT_TYPE_LONG,4,&lGroupid);
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
    	 psData = (struct areastatdata_s *)pasLHashLook(pHash_dep,&lGroupid);   
       if(psData){ 
       	psData->did_count=lCount_g;
       	}   
			  lGroupid=0;
			  lCount_g=0;
			  iReturn = pasDbFetchInto(psCur,
			      	                             UT_TYPE_LONG,4,&lCount_g,
			      	                             UT_TYPE_LONG,4,&lGroupid);  	                              
    }    
    pasDbCloseCursor(psCur); 
    
    
    
 
    
 //在线单位数
    psUser = (ncsClient *)utShmArray(psShmHead,NCS_LNK_CLIENT);
    if(psUser == NULL) {
    	if(pHash_dep) free(pHash_dep);
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","在线单位","memory Error ");
        return 0;
    }
    long iMaxRecord;
   iMaxRecord = utShmArrayRecord(psShmHead,NCS_LNK_CLIENT);
 //  printf("iMaxRec=%d\n",iMaxRecord);
   for(i=0;i<iMaxRecord;i++){
   	   psOnline =  (ncsOnline *)ncsUtlGetOnlineById(psShmHead,psUser[i].userid);
       if(psOnline){
       	  psData = (struct areastatdata_s *)pasLHashLook(pHash_dep,&psUser[i].groupid);   
		      if(psData){ 
		      	if(psUser[i].status==1){
		       		psData->ondid_count++;
		        }
		        else if(psUser[i].status==3){
		        	psData->ycdid_count++;
		        }
		        if(psOnline->lUser<1000000){
		        	psData->onuid_count=psData->onuid_count+psOnline->lUser;
		        }
		      } 
          
          }
      }
//统计儿孙节点单位数
       	 psData = (struct areastatdata_s *)pasLHashFirst(pHash_dep,&sHashInfo);
       	 iCount = 0;
      	 while(psData && iCount < 2000){
 //     	 	printf("pid=%d,did_count=%d,ondid=%d,ycdid=%d,onuid=%d,endnum=%d,groupname=%s,groupid=%d\n",psData->pid,psData->did_count,\
  //    	 	psData->ondid_count,psData->ycdid_count,psData->onuid_count,psData->endnum,psData->groupname,psData->id);
             psData2 = (struct areastatdata_s *)pasLHashLook(pHash_dep,&psData->pid);
             iCount1=0;
             while(psData2 && iCount1<1000){
             	 psData2->ondid_count_z=psData2->ondid_count_z+psData->ondid_count;
             	 psData2->ycdid_count_z=psData2->ycdid_count_z+psData->ycdid_count;
             	 psData2->did_count_z=psData2->did_count_z+psData->did_count;
             	 psData2->onuid_count_z=psData2->onuid_count_z+psData->onuid_count;
             	 psData2->endnum_z=psData2->endnum_z+psData->endnum;
             	 psData3 = (struct areastatdata_s *)pasLHashLook(pHash_dep,&psData2->pid);
             	 psData2=psData3;
             	 iCount1++;
             }
                
                iCount ++;
             
  		    psData = (struct areastatdata_s *)pasLHashNext(&sHashInfo);
         }
 
   long lSum=0;
 lSum_ondid=0;
 lSum_did=0;
    pAreastatdata=(struct areastatdata_s*)malloc((1000)*sizeof(struct areastatdata_s));
    if(pAreastatdata==NULL){
  	    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;
    }
    memset(pAreastatdata,0,(1000)*sizeof(struct areastatdata_s));
 
        psData = (struct areastatdata_s *)pasLHashFirst(pHash_dep,&sHashInfo);
        iCount = 0;
      	 while(psData && iCount < 2000){
      	 
      	 	  psData2 = (struct areastatdata_s *)pasLHashLook(pHash_dep,&psData->pid);
      	 	  //找不到，代表是根
            if(psData2==NULL){
            	pAreastatdata[lSum].pid=psData->pid;
            	pAreastatdata[lSum].id=psData->id;
            	pAreastatdata[lSum].ondid_count=psData->ondid_count+psData->ondid_count_z;
            	pAreastatdata[lSum].ycdid_count=psData->ycdid_count+psData->ycdid_count_z;
            	pAreastatdata[lSum].did_count=psData->did_count+psData->did_count_z;
            	pAreastatdata[lSum].onuid_count=psData->onuid_count+psData->onuid_count_z;
            	pAreastatdata[lSum].endnum=psData->endnum+psData->endnum_z;
            	strcpy(pAreastatdata[lSum].groupname,psData->groupname);
            
            	pAreastatdata[lSum].pArea=(struct areastatdata_s*)malloc((100)*sizeof(struct areastatdata_s));
            	memset(pAreastatdata[lSum].pArea,0,100*sizeof(struct areastatdata_s));
            	//获取下个节点的数据
            	 psData3 = (struct areastatdata_s *)pasLHashFirst(pHash_dep,&sHashInfo2);
            	 lSum2=0;
            	 iCount1 = 0;
      	       while(psData3 && iCount1 < 2000){
      	       	if(pAreastatdata[lSum].id==psData3->pid){
      	       		
//      	       		   printf("sum=%d,zpid=%d,did_count=%d,ondid=%d,ycdid=%d,onuid=%d,endnum=%d,groupname=%s,groupid=%d\n",lSum2,psData3->pid,psData3->did_count_z,psData3->ondid_count_z,psData3->ycdid_count_z,psData3->onuid_count_z,psData3->endnum_z,psData3->groupname,psData3->id);  
      	       		
      	       		pAreastatdata[lSum].pArea[lSum2].pid=psData3->pid;
      	       		pAreastatdata[lSum].pArea[lSum2].id=psData3->id;
      	       		   		
      	       		pAreastatdata[lSum].pArea[lSum2].did_count=pAreastatdata[lSum].pArea[lSum2].did_count+psData3->did_count+psData3->did_count_z;
      	       		pAreastatdata[lSum].pArea[lSum2].ondid_count=pAreastatdata[lSum].pArea[lSum2].ondid_count+psData3->ondid_count+psData3->ondid_count_z;
      	       		
   
    	       		
      	       		pAreastatdata[lSum].pArea[lSum2].ycdid_count=pAreastatdata[lSum].pArea[lSum2].ycdid_count+psData3->ycdid_count+psData3->ycdid_count_z;
      	       		
      
      	       		pAreastatdata[lSum].pArea[lSum2].onuid_count=pAreastatdata[lSum].pArea[lSum2].onuid_count+psData3->onuid_count+psData3->onuid_count_z;
      	       		pAreastatdata[lSum].pArea[lSum2].endnum=pAreastatdata[lSum].pArea[lSum2].endnum+psData3->endnum+psData3->endnum_z;
      	       		strcpy(pAreastatdata[lSum].pArea[lSum2].groupname,psData3->groupname);
      	       		pAreastatdata[lSum].lSum++;
      	       		lSum2++;
      	       		if(lSum2>99) break;
      	       	}
      	       	psData3 = (struct areastatdata_s *)pasLHashNext(&sHashInfo2);
      	       	iCount1 ++;
      	       }
            	lSum_ondid=lSum_ondid+pAreastatdata[lSum].ondid_count;
            	lSum_did=lSum_did=pAreastatdata[lSum].did_count;
            	lSum++;
            } 
             iCount ++;
  		    psData = (struct areastatdata_s *)pasLHashNext(&sHashInfo);
         }
 
   
    iNum=0;
   psDbHead = utPltInitDb();
//   printf("lSum=%d\n",lSum);
   for(i=0;i<lSum;i++){
   	  if(i>0){
   	  	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
   	  }
    	utPltPutLoopVar(psDbHead,"area",iNum+1,pAreastatdata[i].groupname);
    	utPltPutLoopVarF(psDbHead,"ondid_count",iNum+1,"%lu",pAreastatdata[i].ondid_count);
    	utPltPutLoopVarF(psDbHead,"did_count",iNum+1,"%lu",pAreastatdata[i].did_count);
    	if(lSum_ondid>0){
    		 utPltPutLoopVarF(psDbHead,"onlinerate",iNum+1,"%d%c",(int)(100*(float)(pAreastatdata[i].ondid_count+pAreastatdata[i].ycdid_count)/lSum_ondid+0.5),'%');
    	}
    	else{
    		 utPltPutLoopVarF(psDbHead,"onlinerate",iNum+1,"%d%c",0,'%');
    	}
    	utPltPutLoopVarF(psDbHead,"endnum",iNum+1,"%lu",pAreastatdata[i].endnum);
    	utPltPutLoopVarF(psDbHead,"ycdid_count",iNum+1,"%lu",pAreastatdata[i].ycdid_count);
    	if(pAreastatdata[i].did_count-pAreastatdata[i].ondid_count-pAreastatdata[i].ycdid_count>0){
    		utPltPutLoopVarF(psDbHead,"lxdid_count",iNum+1,"%lu",pAreastatdata[i].did_count-pAreastatdata[i].ondid_count-pAreastatdata[i].ycdid_count);
    	}
    	else{
    		utPltPutLoopVarF(psDbHead,"lxdid_count",iNum+1,"%lu",0);
    	}
    	utPltPutLoopVarF(psDbHead,"onuid_count",iNum+1,"%lu",pAreastatdata[i].onuid_count);
    	
    	if(pAreastatdata[i].did_count>0){
    		utPltPutLoopVarF(psDbHead,"rate",iNum+1,"%d%c",100*(pAreastatdata[i].ondid_count+pAreastatdata[i].ycdid_count)/pAreastatdata[i].did_count,'%');
    	}
    	 else
    	  utPltPutLoopVar(psDbHead,"rate",iNum+1,"0");
//输出第二层节点 

/*
    	 for(m=0;m<pAreastatdata[i].lSum;m++){
    	 	
    	 	
      	   
    	 	
    	 	utPltPutLoopVar2(psDbHead,"area",iNum+1,m+1,pAreastatdata[i].pArea[m].groupname);
    	 	utPltPutLoopVarF2(psDbHead,"ondid_count",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].ondid_count);
    	 	utPltPutLoopVarF2(psDbHead,"did_count",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].did_count);
    	 	utPltPutLoopVarF2(psDbHead,"endnum",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].endnum);
    	 	utPltPutLoopVarF2(psDbHead,"ycdid_count",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].ycdid_count);
    	 	if(pAreastatdata[i].pArea[m].did_count-pAreastatdata[i].pArea[m].ondid_count-pAreastatdata[i].pArea[m].ycdid_count>0){
    	 		utPltPutLoopVarF2(psDbHead,"lxdid_count",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].did_count-pAreastatdata[i].pArea[m].ondid_count-pAreastatdata[i].pArea[m].ycdid_count);
    		}
    		else{
    			utPltPutLoopVarF2(psDbHead,"lxdid_count",iNum+1,m+1,"%lu",0);
    		}
    	 	utPltPutLoopVarF2(psDbHead,"onuid_count",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].onuid_count);
    	 	if(pAreastatdata[i].pArea[m].did_count>0){
    	 		if(pAreastatdata[i].pArea[m].ondid_count+pAreastatdata[i].pArea[m].ycdid_count<pAreastatdata[i].pArea[m].did_count){
    				utPltPutLoopVarF2(psDbHead,"rate",iNum+1,m+1,"%d%c",100*(pAreastatdata[i].pArea[m].ondid_count+pAreastatdata[i].pArea[m].ycdid_count)/pAreastatdata[i].pArea[m].did_count,'%');
    		  }
    		}
    	 	else
    	    utPltPutLoopVar2(psDbHead,"rate",iNum+1,m+1,"0");
    	 	
    	 	  //输出第三层的节点
    	 	  n=0;
         psData = (struct areastatdata_s *)pasLHashFirst(pHash_dep,&sHashInfo);
         iCount = 0;
      	 while(psData && iCount < 2000){ 
      	 	 if(pAreastatdata[i].pArea[m].id==psData->pid){
      	 	 	utPltPutLoopVar3(psDbHead,"area",iNum+1,m+1,n+1,psData->groupname);
      	 	 	utPltPutLoopVarF3(psDbHead,"ondid_count",iNum+1,m+1,n+1,"%lu",psData->ondid_count+psData->ondid_count_z);
      	 	 	utPltPutLoopVarF3(psDbHead,"did_count",iNum+1,m+1,n+1,"%lu",psData->did_count+psData->did_count_z);
      	 	 	utPltPutLoopVarF3(psDbHead,"endnum",iNum+1,m+1,n+1,"%lu",psData->endnum+psData->endnum_z);
      	 	 	utPltPutLoopVarF3(psDbHead,"ycdid_count",iNum+1,m+1,n+1,"%lu",psData->ycdid_count+psData->ycdid_count_z);
      	 	 	if(psData->did_count+psData->did_count_z-psData->ondid_count-psData->ondid_count_z-psData->ycdid_count-psData->ycdid_count_z>=0){
      	 	 		utPltPutLoopVarF3(psDbHead,"lxdid_count",iNum+1,m+1,n+1,"%lu",psData->did_count+psData->did_count_z-psData->ondid_count-psData->ondid_count_z-psData->ycdid_count-psData->ycdid_count_z);
      	 	  }
      	 	  else{
      	 	  	utPltPutLoopVarF3(psDbHead,"lxdid_count",iNum+1,m+1,n+1,"%lu",0);
      	 	  }
      	 	 	utPltPutLoopVarF3(psDbHead,"onuid_count",iNum+1,m+1,n+1,"%lu",psData->onuid_count+psData->onuid_count_z);
      	 	 	if(psData->did_count>0){
      	 	 		utPltPutLoopVarF3(psDbHead,"rate",iNum+1,m+1,n+1,"%d%c",100*(psData->ondid_count+psData->ondid_count_z+psData->ycdid_count+psData->ycdid_count_z)/(psData->did_count+psData->did_count_z),'%');
      	 	 	}
      	 	 	else{
      	 	 		utPltPutLoopVar3(psDbHead,"rate",iNum+1,m+1,n+1,"0");
      	 	 	}
      	 	 	n++;
      	 	 }
      	 	
      	 	iCount ++;
      	 	psData = (struct areastatdata_s *)pasLHashNext(&sHashInfo);
      	}	 	  
    	 	  
    
    	 	  
    	 	
    	}
    	*/
    	
    	iNum++;

//    	lSum_did=lSum_did+pAreastatdata[i].did_count;
//    	lSum_ondid=lSum_ondid+pAreastatdata[i].ondid_count;
//    	lSum_ycdid=lSum_ycdid+pAreastatdata[i].ycdid_count;
 //   	if(lSum_lxdid+pAreastatdata[i].did_count-pAreastatdata[i].ondid_count-pAreastatdata[i].ycdid_count>0){
 //   		lSum_lxdid=lSum_lxdid+pAreastatdata[i].did_count-pAreastatdata[i].ondid_count-pAreastatdata[i].ycdid_count;
 //   	}
 //   	else{
//    		lSum_lxdid=0;
//    	}
  //  	lSum_endnum=lSum_endnum+pAreastatdata[i].endnum;
  //  	lSum_onuid=lSum_onuid+pAreastatdata[i].onuid_count;
 //   	if(iNum>28) break;
    }
    
 //   if(pAreastatdata) free(pAreastatdata);
    
//		utPltPutVarF(psDbHead,"sum_did","%d",lSum_did);
//		utPltPutVarF(psDbHead,"sum_ondid","%d",lSum_ondid);
//		utPltPutVarF(psDbHead,"sum_ycdid","%d",lSum_ycdid);
//		utPltPutVarF(psDbHead,"sum_lxdid","%d",lSum_lxdid);
//		utPltPutVarF(psDbHead,"sum_endnum","%d",lSum_endnum);
//		utPltPutVarF(psDbHead,"sum_onuid","%d",lSum_onuid);

 //     	 	 	if(lSum_did>0){
 //     	 	 		utPltPutVarF(psDbHead,"rate","%d%c",100*(lSum_ondid)/lSum_did,'%');
  //    	 	 	}
 //     	 	 	else{
 //     	 	 		utPltPutVar(psDbHead,"rate","0");
 //     	 	 	}

        if(pHash_dep) free(pHash_dep);
	        for(i=0;i<lSum;i++){ 
	        		free(pAreastatdata[lSum].pArea);
	        }
        free(pAreastatdata);
        
    
       utPltPutVarF(psDbHead,"TotRec","%d",lSum);
//utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_corp_firstbt.htm");
    return 0;
}




/*统计在线单位在线率*/
int ncs_onlinecorp_first_onlinerate(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    ncsOnline *psOnline;
    ncsClient *psUser;
    char *pHash;
  	char caTemp[9096];
  	char caGroupname[32];
  	pasDbCursor *psCur;
  	int lCount_area,lCount_corp,lCount_zx,lCount_lx,lCount_yc,lCount_user,lCount_useron;
    struct areastatdata_s *pAreastatdata;
    utPltDbHead *psDbHead;
   	int iReturn,i,lStart,lGroupid,lCount_g,index,iNum,lCurPg,lTotPg;
    char caTotPg[16],caCurPg[16],caExport[12];
    int lSum_did,lSum_ondid,lSum_ycdid,lSum_lxdid,lSum_endnum,lSum_onuid,lSum_tycount,lSum_whcount;
    char caOpt[256];
    unsigned long lId,lPid;
    char caUsername2[32];
    FILE *fp;
    char pFile[256],caBuf[128];
    char caAll[4024];
    char caSid[16];
    struct areastatdata_s *psData,*psData2,*psData3;
    unsigned char *pHash_dep;
    pasLHashInfo sHashInfo,sHashInfo2;
    long lSum2,m,n,iCount=0,iCount1=0;
    long lUserid;
    unsigned char *pHash_wh;
     struct corpwh_s{
 	 	unsigned long lUserid;  //单位ID
 		unsigned long flag;     //4-停业 5-维护
 	};
   struct corpwh_s *psData_wh;
    char caGroups[4024];
      char caFacs[4024];
   char caWireflag[20];
	 strcpy(caFacs,getDsFacDid());
    strcpy(caGroups,getDsGroupid());
    memset(caAll,0,sizeof(caAll));
   
       	iReturn = utMsgGetSomeNVar(psMsgHead,1,
	       "wireflag",      UT_TYPE_STRING, 10, caWireflag);
   
 //将单位放到Hash flag=5 维护  2-5 停业中
    pHash_wh = (unsigned char *)pasLHashInit(10000,10000,sizeof(struct corpwh_s),0,4);    
    if(pHash_wh == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
  sprintf(caTemp,"select userid,servicestate from ncsuser where 1=1 ");
     if(strlen(caWireflag)>0){
    	if(strcmp(caWireflag,"2")==0){
    	   sprintf(caTemp+strlen(caTemp)," and (jointype='08' or jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
      	sprintf(caTemp+strlen(caTemp)," and jointype!='08' ");
      }
    
    }
  
  sprintf(caTemp+strlen(caTemp)," limit 0,9999 ");
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
       	 	  else if(strcmp(caGroupname,"2")>0){
       	 	  	psData_wh->flag=4;
       	 	  }
       	 	  else{
       	 	  	psData_wh->flag=0;
       	 	  }
       	 
       	 }
    	 	
			  lGroupid=0;
			  memset(caGroupname,0,sizeof(caGroupname));
			  iReturn = pasDbFetchInto(psCur,    	                             
			      	                             UT_TYPE_LONG,4,&lGroupid,
			      	                             UT_TYPE_STRING,1,caGroupname); 	                              
    }    
    pasDbCloseCursor(psCur);  
    
    
    
    
    
    
 /*
   sprintf(caTemp,"select uid from ncsexcpgroup limit 0,300 ");
  
  psCur = pasDbOpenSql(caTemp,0);
  memset(caNid,0,sizeof(caNid));
  iNum=0;
  if(psCur != NULL) {
  	   lUserid=0;
    	 iReturn = pasDbFetchInto(psCur,    	                             
      	                             UT_TYPE_LONG,4,&lUserid);
      	while((iReturn==0)||(iReturn==1405))
        { 
        	if(iNum==0){
        		sprintf(caNid,"%d",lUserid);
        	}
        	else{
        		sprintf(caNid+strlen(caNid),",%d",lUserid);
        	}
        	iNum++;
        	     	 iReturn = pasDbFetchInto(psCur,    	                             
      	                             UT_TYPE_LONG,4,&lUserid);
        }                       
      	pasDbCloseCursor(psCur);                             
   }
                       
    	                              
*/
 
 
    
    
//    dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
 //   memset(caOpt,0,sizeof(caOpt)); 
 //   iReturn = dsCltGetMyInfo(1,"Userid",&lId);
  //  if(iReturn==0){
 //	    pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
 //   } 
   
   
   
    lSum_did=0;
    lSum_ondid=0;
    lSum_ycdid=0;
    lSum_lxdid=0;
    lSum_endnum=0;
    lSum_onuid=0;
    lSum_whcount=0;
    lSum_tycount=0;
    lCount_corp=0;
    lCount_zx=0;
    lCount_lx=0;
    lCount_yc=0;
    lCount_user=0;
    lCount_useron=0;
    
    
  
    
    	iReturn = utMsgGetSomeNVar(psMsgHead,3,
	    "TotPg",      UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      UT_TYPE_STRING, 12, caCurPg,
   	  "exp",     UT_TYPE_STRING,10,caExport);
 
    lCurPg = atol(caCurPg);
    if(lCurPg <= 0) lCurPg = 1;
    lStart = (lCurPg - 1) * 28;  
   
    pHash_dep = (unsigned char *)pasLHashInit(1000,1000,sizeof(struct areastatdata_s),4,4);    
    if(pHash_dep == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
   
   
   
 //   pAreastatdata=(struct areastatdata_s*)malloc((lCount_area+1)*sizeof(struct areastatdata_s));
//    if(pAreastatdata==NULL){
 // 	    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
//        return 0;
//  }
  
 sprintf(caTemp,"select b.groupid,b.groupname,b.pid from  ncsgroup b where  b.groupid!=0 ");
 //  if(!strstr(caOpt,"all")&&(strlen(caOpt)>0)){
	//		sprintf(caTemp+strlen(caTemp)," and (b.groupid in (%s) or b.groupid in (select groupid from ncsgroup where pid in (%s))) ",caOpt,caOpt);
//   }
  if(strlen(caGroups)>0){
  	sprintf(caTemp+strlen(caTemp)," and b.groupid in (%s) ",caGroups);
  }
   
  sprintf(caTemp+strlen(caTemp)," group by b.pid,b.groupid,b.groupname order by b.groupid limit 0,999");
  printf("caTemp=%s\n",caTemp);
  psCur = pasDbOpenSql(caTemp,0);
  if(psCur == NULL) {
   // 	if(pAreastatdata) free(pAreastatdata);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
   }
  lGroupid=0;
  lCount_g=0;
  lPid=0;
  i=0;
  memset(caGroupname,0,sizeof(caGroupname));
  iReturn = pasDbFetchInto(psCur,    	                             
      	                             UT_TYPE_LONG,4,&lGroupid,
      	                             UT_TYPE_STRING,30,caGroupname,
      	                             UT_TYPE_LONG,4,&lPid);
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
    		 psData = (struct areastatdata_s *)pasLHashLookA(pHash_dep,&lGroupid);  
       	 if(psData){
       	 	psData->pid=lPid;
       	 	psData->did_count=0;
       	 	strcpy(psData->groupname,caGroupname);
          psData->ondid_count=0;
          psData->ycdid_count=0;
          psData->ondid_count=0;
          psData->onuid_count=0;
          psData->endnum=0;
          psData->wh_count=0;
          psData->ty_count=0;
       	 }
    	 	
		  lGroupid=0;
		  lCount_g=0;
  		memset(caGroupname,0,sizeof(caGroupname));
  		lPid=0;
      iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lGroupid,
      	                             UT_TYPE_STRING,30,caGroupname,
      	                             UT_TYPE_LONG,4,&lPid); 	                              
    }    
    pasDbCloseCursor(psCur);  
    
    
    //添加未知区域
        lGroupid=99999999;
         psData = (struct areastatdata_s *)pasLHashLookA(pHash_dep,&lGroupid);  
       	 if(psData){
       	 	psData->pid=0;
       	 	psData->did_count=0;
       	 	strcpy(psData->groupname,"其它");
          psData->ondid_count=0;
          psData->ycdid_count=0;
          psData->ondid_count=0;
          psData->onuid_count=0;
          psData->endnum=0;
          psData->wh_count=0;
          psData->ty_count=0;
       	 }
    
    
    
    
    
    
    
    
    
    //统计各地区单位数
    sprintf(caTemp,"select count(*),groupid from ncsuser where 1=1 ");
  //  if(strlen(caNid)>0){
  //  	sprintf(caTemp+strlen(caTemp)," and userid not in (%s) ",caNid);
//    }


    if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caGroups);
    }
    if(strlen(caFacs)>0){
				sprintf(caTemp+strlen(caTemp)," and userid in (%s) ",caFacs);
		}
    
    if(strlen(caWireflag)>0){
    	if(strcmp(caWireflag,"2")==0){
    	   sprintf(caTemp+strlen(caTemp)," and (jointype='08' or jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
      	sprintf(caTemp+strlen(caTemp)," and jointype!='08' ");
      }
    
    }
    
    sprintf(caTemp+strlen(caTemp)," group by groupid ");
 //   if(!strstr(caOpt,"all")&&(strlen(caOpt)>0)){
	//		sprintf(caTemp+strlen(caTemp)," and a.groupid in (%s) ",caOpt);
 //   }
      printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
    	if(pHash_dep) free(pHash_dep);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lGroupid=0;
  	lCount_g=0;
  	i=0;
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_g,
      	                             UT_TYPE_LONG,4,&lGroupid);
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
    	 psData = (struct areastatdata_s *)pasLHashLook(pHash_dep,&lGroupid);   
       if(psData){ 
       	psData->did_count=lCount_g;
       	} 
       	else{
       		lGroupid=99999999;
       		 psData = (struct areastatdata_s *)pasLHashLook(pHash_dep,&lGroupid);   
		       if(psData){ 
		       	psData->did_count=psData->did_count+lCount_g;
		       	}
       	}  
			  lGroupid=0;
			  lCount_g=0;
			  iReturn = pasDbFetchInto(psCur,
			      	                             UT_TYPE_LONG,4,&lCount_g,
			      	                             UT_TYPE_LONG,4,&lGroupid);  	                              
    }    
    pasDbCloseCursor(psCur); 
    
    
    
    
 //各地区用户数   
   memset(caTemp,0,sizeof(caTemp));
       strcpy(caTemp,"select sum(lcount),a.groupid from ncsclient_enduserstat a where 1=1 ");
//    strcpy(caTemp,"select count(a.groupid),a.groupid from ncsuser a,ncsenduser b where a.userid=b.did and a.groupid!=0 ");
//    if(!strstr(caOpt,"all")&&(strlen(caOpt)>0)){
//			sprintf(caTemp+strlen(caTemp)," and a.groupid in (%s) ",caOpt);
//   }
//   if(strlen(caNid)>0){
 //   	sprintf(caTemp+strlen(caTemp)," and a.userid not in (%s) ",caNid);
//    }
   if(strlen(caGroups)>0){
 //   	printf("caGroups3=%s\n",caGroups);
     	sprintf(caTemp+strlen(caTemp)," and a.groupid in (%s) ",caGroups);
    	}
       if(strlen(caFacs)>0){
				sprintf(caTemp+strlen(caTemp)," and a.userid in (%s) ",caFacs);
		}
		
		 if(strlen(caWireflag)>0){
    	if(strcmp(caWireflag,"2")==0){
    	   sprintf(caTemp+strlen(caTemp)," and a.userid in (select userid from ncsuser where (jointype='08' or jointype='09')) ");
      }
      else if(strcmp(caWireflag,"1")==0){
      	sprintf(caTemp+strlen(caTemp)," and a.userid in (select userid from ncsuser where jointype!='08') ");
      }
    
    }
		
    sprintf(caTemp+strlen(caTemp)," group by a.groupid "); 
    
    
      printf("caTemp2=%s\n",caTemp); 
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
    	if(pHash_dep) free(pHash_dep);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
      
    lGroupid=0;
  	lCount_g=0;
  	i=0;
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_g,
      	                             UT_TYPE_LONG,4,&lGroupid);
  //   printf("pid=%d\n",getpid());
  //   sleep(15); 	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
    	 psData = (struct areastatdata_s *)pasLHashLook(pHash_dep,&lGroupid);   
       if(psData){ 
       	psData->endnum=lCount_g;
       	}  
       	else{
       		lGroupid=99999999;
       		 psData = (struct areastatdata_s *)pasLHashLook(pHash_dep,&lGroupid);   
		       if(psData){ 
		       	psData->endnum=psData->endnum+lCount_g;
		       	} 
       	} 
			  lGroupid=0;
			  lCount_g=0;
			  iReturn = pasDbFetchInto(psCur,
			      	                             UT_TYPE_LONG,4,&lCount_g,
			      	                             UT_TYPE_LONG,4,&lGroupid);  	                              
    }    
    pasDbCloseCursor(psCur); 
 
 //在线单位数
    psUser = (ncsClient *)utShmArray(psShmHead,NCS_LNK_CLIENT);
    if(psUser == NULL) {
    	if(pHash_dep) free(pHash_dep);
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","在线单位","memory Error ");
        return 0;
    }
    long iMaxRecord;
   iMaxRecord = utShmArrayRecord(psShmHead,NCS_LNK_CLIENT);

   for(i=0;i<iMaxRecord;i++){
   
		   	if(strlen(caGroups)>0&&isInGroup(caGroups,psUser[i].groupid)==0){
		   		continue;
		   	}
		   	if(strlen(caFacs)>0&&isInGroup(caFacs,psUser[i].userid)==0){
		   		continue;
		   	}
   		  psData_wh=(struct corpwh_s *)pasLHashLook(pHash_wh,&psUser[i].userid); 
		    if(!psData_wh){
		       continue;
		    }
   	
   	      psOnline =  (ncsOnline *)ncsUtlGetOnlineById(psShmHead,psUser[i].userid);
   	      psData = (struct areastatdata_s *)pasLHashLook(pHash_dep,&psUser[i].groupid); 
   	      if(psData==NULL){
   	      	    lGroupid=99999999;
          	    psData = (struct areastatdata_s *)pasLHashLook(pHash_dep,&lGroupid);  
   	      }  
		      if(psData){ 
		      	if(psUser[i].status==1||psUser[i].status==3){
		      		if(psOnline){
		       			psData->ondid_count++;
		       			if(psOnline->lUser<1000000){
				        	psData->onuid_count=psData->onuid_count+psOnline->lUser;
				        }
		       	  }
		       	  else{
		       	  	
		       	  			psData_wh=(struct corpwh_s *)pasLHashLook(pHash_wh,&psUser[i].userid); 
					        	if(psData_wh){
					        		
					        		if(psData_wh->flag==5){
					        			psData->wh_count++;
					        		}
					        		else if(psData_wh->flag>=2&&psData_wh->flag<=4){
					        			psData->ty_count++;
					        		}
					        }
		       	  	
		       	  }
		        }
		        else{
//查询一下这个单位是否处于停业或维护状态
		        	psData_wh=(struct corpwh_s *)pasLHashLook(pHash_wh,&psUser[i].userid); 
		        	if(psData_wh){
		        		
		        		if(psData_wh->flag==5){
		        			psData->wh_count++;
		        		}
		        		else if(psData_wh->flag>=2&&psData_wh->flag<=4){
		        			psData->ty_count++;
		        		}
		        }
		       }
        }
      }

//统计儿孙节点单位数
       	 psData = (struct areastatdata_s *)pasLHashFirst(pHash_dep,&sHashInfo);
       	 iCount = 0;
      	 while(psData && iCount < 2000){
  //   	 	printf("pid=%d,did_count=%d,ondid=%d,ycdid=%d,onuid=%d,endnum=%d,groupname=%s,groupid=%d\n",psData->pid,psData->did_count,\
 //     	 	psData->ondid_count,psData->ycdid_count,psData->onuid_count,psData->endnum,psData->groupname,psData->id);
             psData2 = (struct areastatdata_s *)pasLHashLook(pHash_dep,&psData->pid);
             iCount1=0;
             while(psData2 && iCount1<1000){
             	 psData2->ondid_count_z=psData2->ondid_count_z+psData->ondid_count;
             	 psData2->ycdid_count_z=psData2->ycdid_count_z+psData->ycdid_count;
             	 psData2->did_count_z=psData2->did_count_z+psData->did_count;
             	 psData2->onuid_count_z=psData2->onuid_count_z+psData->onuid_count;
             	 psData2->endnum_z=psData2->endnum_z+psData->endnum;
             	 psData2->wh_count_z=psData2->wh_count_z+psData->wh_count;
             	 psData2->ty_count_z=psData2->ty_count_z+psData->ty_count;
             	 psData3 = (struct areastatdata_s *)pasLHashLook(pHash_dep,&psData2->pid);
             	 psData2=psData3;
             	 iCount1++;
             }
                
                iCount ++;
             
  		    psData = (struct areastatdata_s *)pasLHashNext(&sHashInfo);
         }

   long lSum=0;
    pAreastatdata=(struct areastatdata_s*)malloc((1000)*sizeof(struct areastatdata_s));
    if(pAreastatdata==NULL){
  	    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;
    }
    memset(pAreastatdata,0,(1000)*sizeof(struct areastatdata_s));
 
        psData = (struct areastatdata_s *)pasLHashFirst(pHash_dep,&sHashInfo);
        iCount = 0;
      	 while(psData && iCount < 2000){
      	 
      	 	  psData2 = (struct areastatdata_s *)pasLHashLook(pHash_dep,&psData->pid);
      	 	  //找不到，代表是根
            if(psData2==NULL){
            	pAreastatdata[lSum].pid=psData->pid;
            	pAreastatdata[lSum].id=psData->id;
            	pAreastatdata[lSum].ondid_count=psData->ondid_count+psData->ondid_count_z;
            	pAreastatdata[lSum].ycdid_count=psData->ycdid_count+psData->ycdid_count_z;
            	pAreastatdata[lSum].did_count=psData->did_count+psData->did_count_z;
            	pAreastatdata[lSum].onuid_count=psData->onuid_count+psData->onuid_count_z;
            	pAreastatdata[lSum].endnum=psData->endnum+psData->endnum_z;
            	pAreastatdata[lSum].wh_count=psData->wh_count+psData->wh_count_z;
            	pAreastatdata[lSum].ty_count=psData->ty_count+psData->ty_count_z;
            	strcpy(pAreastatdata[lSum].groupname,psData->groupname);
            
            	pAreastatdata[lSum].pArea=(struct areastatdata_s*)malloc((100)*sizeof(struct areastatdata_s));
            	memset(pAreastatdata[lSum].pArea,0,100*sizeof(struct areastatdata_s));
            	//获取下个节点的数据
            	 psData3 = (struct areastatdata_s *)pasLHashFirst(pHash_dep,&sHashInfo2);
            	 lSum2=0;
            	 iCount1 = 0;
      	       while(psData3 && iCount1 < 2000){
      	       	if(pAreastatdata[lSum].id==psData3->pid){
      	       		
//      	       		   printf("sum=%d,zpid=%d,did_count=%d,ondid=%d,ycdid=%d,onuid=%d,endnum=%d,groupname=%s,groupid=%d\n",lSum2,psData3->pid,psData3->did_count_z,psData3->ondid_count_z,psData3->ycdid_count_z,psData3->onuid_count_z,psData3->endnum_z,psData3->groupname,psData3->id);  
      	       		
      	       		pAreastatdata[lSum].pArea[lSum2].pid=psData3->pid;
      	       		pAreastatdata[lSum].pArea[lSum2].id=psData3->id;
      	       		   		
      	       		pAreastatdata[lSum].pArea[lSum2].did_count=pAreastatdata[lSum].pArea[lSum2].did_count+psData3->did_count+psData3->did_count_z;
      	       		pAreastatdata[lSum].pArea[lSum2].ondid_count=pAreastatdata[lSum].pArea[lSum2].ondid_count+psData3->ondid_count+psData3->ondid_count_z;
      	       		pAreastatdata[lSum].pArea[lSum2].wh_count=pAreastatdata[lSum].pArea[lSum2].wh_count+psData3->wh_count+psData3->wh_count_z;
      	       		pAreastatdata[lSum].pArea[lSum2].ty_count=pAreastatdata[lSum].pArea[lSum2].ty_count+psData3->ty_count+psData3->ty_count_z;
   
    	       		
      	       		pAreastatdata[lSum].pArea[lSum2].ycdid_count=pAreastatdata[lSum].pArea[lSum2].ycdid_count+psData3->ycdid_count+psData3->ycdid_count_z;
      	       		
      
      	       		pAreastatdata[lSum].pArea[lSum2].onuid_count=pAreastatdata[lSum].pArea[lSum2].onuid_count+psData3->onuid_count+psData3->onuid_count_z;
      	       		pAreastatdata[lSum].pArea[lSum2].endnum=pAreastatdata[lSum].pArea[lSum2].endnum+psData3->endnum+psData3->endnum_z;
      	       		strcpy(pAreastatdata[lSum].pArea[lSum2].groupname,psData3->groupname);
      	       		pAreastatdata[lSum].lSum++;
      	       		lSum2++;
      	       		if(lSum2>99) break;
      	       	}
      	       	psData3 = (struct areastatdata_s *)pasLHashNext(&sHashInfo2);
      	       	iCount1 ++;
      	       }
            	
            	lSum++;
            } 
             iCount ++;
  		    psData = (struct areastatdata_s *)pasLHashNext(&sHashInfo);
         }
 
 
 
printf("44444444444444444444\n");

  
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
    	char caRate[16];
        sprintf(caFilename,"ncsstat%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
            if(pAreastatdata) free(pAreastatdata);
            if(pHash_dep) free(pHash_dep);
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
        
          fprintf(fp,"统计信息\n");
		      ncUtlPrintCsv(fp,10,UT_TYPE_STRING,"区域",
		                       UT_TYPE_STRING,"单位总数",
		                       UT_TYPE_STRING,"在线单位",
		                       UT_TYPE_STRING,"异常在线单位",
		                       UT_TYPE_STRING,"维护单位数",
		                       UT_TYPE_STRING,"停业单位数",
		                       UT_TYPE_STRING,"离线单位",
		                       UT_TYPE_STRING,"用户总数",
		                       UT_TYPE_STRING,"在线用户",
		                       UT_TYPE_STRING,"在线率");
			 for(i=0;i<lSum;i++){
			 	    	if(pAreastatdata[i].did_count>0){
			 	    		sprintf(caRate,"%d%c",100*(pAreastatdata[i].ondid_count+pAreastatdata[i].ycdid_count)/(pAreastatdata[i].did_count-pAreastatdata[i].wh_count-pAreastatdata[i].ty_count),'%');
			 	    	}
			 	    	else{
			 	    		strcpy(caRate,"0");
			 	    	}
    		
			 	
			 	
			 	
			 	 ncUtlPrintCsv(fp,10,UT_TYPE_STRING,pAreastatdata[i].groupname,
                       UT_TYPE_LONG,pAreastatdata[i].did_count,
                       UT_TYPE_LONG,pAreastatdata[i].ondid_count,
                       UT_TYPE_LONG,pAreastatdata[i].ycdid_count,
                       UT_TYPE_LONG,pAreastatdata[i].wh_count,
                       UT_TYPE_LONG,pAreastatdata[i].ty_count,
                       UT_TYPE_LONG,pAreastatdata[i].did_count-pAreastatdata[i].ondid_count-pAreastatdata[i].ycdid_count-pAreastatdata[i].wh_count-pAreastatdata[i].ty_count,
                       UT_TYPE_LONG,pAreastatdata[i].endnum,
                       UT_TYPE_LONG,pAreastatdata[i].onuid_count,
                       UT_TYPE_STRING,caRate);
                       for(m=0;m<pAreastatdata[i].lSum;m++){
                       	 memset(caRate,0,sizeof(caRate));
                       	 if(pAreastatdata[i].pArea[m].did_count>0){
								    	 		if(pAreastatdata[i].pArea[m].ondid_count+pAreastatdata[i].pArea[m].ycdid_count<=(pAreastatdata[i].pArea[m].did_count-pAreastatdata[i].pArea[m].wh_count-pAreastatdata[i].pArea[m].ty_count)){
								    	 			sprintf(caRate,"%d%c",100*(pAreastatdata[i].pArea[m].ondid_count+pAreastatdata[i].pArea[m].ycdid_count)/(pAreastatdata[i].pArea[m].did_count-pAreastatdata[i].pArea[m].wh_count-pAreastatdata[i].pArea[m].ty_count),'%');
								   				
								    		  }
    										}
    	 								else{
    	 									strcpy(caRate,"0");
    	 								}
    	    
                       	
                       	
                       	ncUtlPrintCsv(fp,10,UT_TYPE_STRING,pAreastatdata[i].pArea[m].groupname,
                       		UT_TYPE_LONG,pAreastatdata[i].pArea[m].did_count,
                       		UT_TYPE_LONG,pAreastatdata[i].pArea[m].ondid_count,
                       		UT_TYPE_LONG,pAreastatdata[i].pArea[m].ycdid_count,
                       		UT_TYPE_LONG,pAreastatdata[i].pArea[m].wh_count,
                       		UT_TYPE_LONG,pAreastatdata[i].pArea[m].ty_count,
                       		UT_TYPE_LONG,pAreastatdata[i].pArea[m].did_count-pAreastatdata[i].pArea[m].ondid_count-pAreastatdata[i].pArea[m].ycdid_count-pAreastatdata[i].pArea[m].wh_count-pAreastatdata[i].pArea[m].ty_count,
                       		UT_TYPE_LONG,pAreastatdata[i].pArea[m].endnum,
                       		UT_TYPE_LONG,pAreastatdata[i].pArea[m].onuid_count,
                       		UT_TYPE_STRING,caRate);
                     	 	  //输出第三层的节点    	 	 
								         psData = (struct areastatdata_s *)pasLHashFirst(pHash_dep,&sHashInfo);
								      	 while(psData){ 
								      	 	 if(pAreastatdata[i].pArea[m].id==psData->pid){
								      	 	 	 memset(caRate,0,sizeof(caRate));
								             if(psData->did_count>0){
								             	sprintf(caRate,"%d%c",100*(psData->ondid_count+psData->ondid_count_z+psData->ycdid_count+psData->ycdid_count_z)/(psData->did_count+psData->did_count_z-psData->wh_count-psData->wh_count_z-psData->ty_count-psData->ty_count_z),'%');
								            }
								            else{
								            	strcpy(caRate,"0");
								            }

								                  ncUtlPrintCsv(fp,10,UT_TYPE_STRING,psData->groupname,
								                       		UT_TYPE_LONG,psData->did_count+psData->did_count_z,
								                       		UT_TYPE_LONG,psData->ondid_count+psData->ondid_count_z,
								                       		UT_TYPE_LONG,psData->ycdid_count+psData->ycdid_count_z,
								                       		UT_TYPE_LONG,psData->wh_count+psData->wh_count_z,
								                       		UT_TYPE_LONG,psData->ty_count+psData->ty_count_z,
								                       		UT_TYPE_LONG,psData->did_count+psData->did_count_z-psData->ondid_count-psData->ondid_count_z-psData->ycdid_count-psData->ycdid_count_z-psData->wh_count-psData->wh_count_z-psData->ty_count-psData->ty_count_z,
								                       		UT_TYPE_LONG,psData->endnum+psData->endnum_z,
								                       		UT_TYPE_LONG,psData->onuid_count+psData->onuid_count_z,
								                       		UT_TYPE_STRING,caRate);      	 	 									      	 	          	 	
								      	 	   }      	
								     	 	
								      	 	psData = (struct areastatdata_s *)pasLHashNext(&sHashInfo);          	

								                       	
								                       	
								         }
            }             
                     
			  }
			  fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        if(pHash_dep) free(pHash_dep);
	        for(i=0;i<lSum;i++){ 
	        		free(pAreastatdata[lSum].pArea);
	        }
        free(pAreastatdata);
        free(pHash_wh);
        return 0;
			
		}
    
    iNum=0;
   psDbHead = utPltInitDb();
   for(i=0;i<lSum;i++){
   	if(strcmp(pAreastatdata[i].groupname,"其它")==0&&pAreastatdata[i].did_count==0){
   		continue;
   	}
  	
   	if(iNum>0){
   		utPltPutLoopVar(psDbHead,"dhf",iNum+1,",");
   	}
   	 utPltPutLoopVarF(psDbHead,"id",iNum+1,"%lu",pAreastatdata[i].id);
   	
    	utPltPutLoopVar(psDbHead,"area",iNum+1,pAreastatdata[i].groupname);
    	utPltPutLoopVarF(psDbHead,"ondid_count",iNum+1,"%lu",pAreastatdata[i].ondid_count);
    	utPltPutLoopVarF(psDbHead,"did_count",iNum+1,"%lu",pAreastatdata[i].did_count);
    	utPltPutLoopVarF(psDbHead,"endnum",iNum+1,"%lu",pAreastatdata[i].endnum);
    	utPltPutLoopVarF(psDbHead,"ycdid_count",iNum+1,"%lu",pAreastatdata[i].ycdid_count);
    	utPltPutLoopVarF(psDbHead,"wh_count",iNum+1,"%lu",pAreastatdata[i].wh_count);
    	utPltPutLoopVarF(psDbHead,"ty_count",iNum+1,"%lu",pAreastatdata[i].ty_count);
    	if(pAreastatdata[i].did_count-pAreastatdata[i].ondid_count-pAreastatdata[i].ycdid_count-pAreastatdata[i].wh_count-pAreastatdata[i].ty_count>0){
    		utPltPutLoopVarF(psDbHead,"lxdid_count",iNum+1,"%lu",pAreastatdata[i].did_count-pAreastatdata[i].ondid_count-pAreastatdata[i].ycdid_count-pAreastatdata[i].wh_count-pAreastatdata[i].ty_count);
    	}
    	else{
    		utPltPutLoopVarF(psDbHead,"lxdid_count",iNum+1,"%lu",0);
    	}
    	utPltPutLoopVarF(psDbHead,"onuid_count",iNum+1,"%lu",pAreastatdata[i].onuid_count);


    	
    	if(pAreastatdata[i].did_count-pAreastatdata[i].wh_count-pAreastatdata[i].ty_count>0){
    		utPltPutLoopVarF(psDbHead,"rate",iNum+1,"%d%c",100*(pAreastatdata[i].ondid_count+pAreastatdata[i].ycdid_count)/(pAreastatdata[i].did_count-pAreastatdata[i].wh_count-pAreastatdata[i].ty_count),'%');
    	}
    	 else
    	  utPltPutLoopVar(psDbHead,"rate",iNum+1,"100");
    	  
    	  printf("one\n");
//输出第二层节点  	
 printf("two\n");
    	 for(m=0;m<pAreastatdata[i].lSum;m++){
    	 	
    	 	 if(m>0){
    	 	 	 utPltPutLoopVar2(psDbHead,"dhs",iNum+1,m+1,",");
    	 	}
      	   
    	 	utPltPutLoopVarF2(psDbHead,"id",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].id);
    	 	utPltPutLoopVar2(psDbHead,"area",iNum+1,m+1,pAreastatdata[i].pArea[m].groupname);
    	 	utPltPutLoopVarF2(psDbHead,"ondid_count",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].ondid_count);
    	 	utPltPutLoopVarF2(psDbHead,"did_count",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].did_count);
    	 	utPltPutLoopVarF2(psDbHead,"endnum",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].endnum);
    	 	utPltPutLoopVarF2(psDbHead,"ycdid_count",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].ycdid_count);
    	 	utPltPutLoopVarF2(psDbHead,"wh_count",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].wh_count);
    	 	utPltPutLoopVarF2(psDbHead,"ty_count",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].ty_count);
    	 	if(pAreastatdata[i].pArea[m].did_count-pAreastatdata[i].pArea[m].ondid_count-pAreastatdata[i].pArea[m].ycdid_count-pAreastatdata[i].pArea[m].wh_count-pAreastatdata[i].pArea[m].ty_count>0){
    	 		utPltPutLoopVarF2(psDbHead,"lxdid_count",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].did_count-pAreastatdata[i].pArea[m].ondid_count-pAreastatdata[i].pArea[m].ycdid_count-pAreastatdata[i].pArea[m].wh_count-pAreastatdata[i].pArea[m].ty_count);
    		}
    		else{
    			utPltPutLoopVarF2(psDbHead,"lxdid_count",iNum+1,m+1,"%lu",0);
    		}
    		 		
    		
    	 	utPltPutLoopVarF2(psDbHead,"onuid_count",iNum+1,m+1,"%lu",pAreastatdata[i].pArea[m].onuid_count);
    	 	if(pAreastatdata[i].pArea[m].did_count-pAreastatdata[i].pArea[m].wh_count-pAreastatdata[i].pArea[m].ty_count>0){
    	 		if(pAreastatdata[i].pArea[m].ondid_count+pAreastatdata[i].pArea[m].ycdid_count<=(pAreastatdata[i].pArea[m].did_count-pAreastatdata[i].pArea[m].wh_count-pAreastatdata[i].pArea[m].ty_count)){
    				utPltPutLoopVarF2(psDbHead,"rate",iNum+1,m+1,"%d%c",100*(pAreastatdata[i].pArea[m].ondid_count+pAreastatdata[i].pArea[m].ycdid_count)/(pAreastatdata[i].pArea[m].did_count-pAreastatdata[i].pArea[m].wh_count-pAreastatdata[i].pArea[m].ty_count),'%');
    		  }
    		}
    	 	else
    	    utPltPutLoopVar2(psDbHead,"rate",iNum+1,m+1,"0");
    
    
     printf("three\n");
//    printf("pid=%d\n",getpid());	 
  //  sleep(15);	
    	 	  //输出第三层的节点
    	 	  n=0;
         psData = (struct areastatdata_s *)pasLHashFirst(pHash_dep,&sHashInfo);
         iCount = 0;
      	 while(psData && iCount < 2000){ 
      	 	 if(pAreastatdata[i].pArea[m].id==psData->pid){
      	 	 	if(n>0){
      	 	 		utPltPutLoopVar3(psDbHead,"dht",iNum+1,m+1,n+1,",");
      	 	 	}
      	 	  utPltPutLoopVarF3(psDbHead,"id",iNum+1,m+1,n+1,"%lu",psData->id);
      	 	 	utPltPutLoopVar3(psDbHead,"area",iNum+1,m+1,n+1,psData->groupname);
      	 	 	utPltPutLoopVarF3(psDbHead,"ondid_count",iNum+1,m+1,n+1,"%lu",psData->ondid_count+psData->ondid_count_z);
      	 	 	utPltPutLoopVarF3(psDbHead,"did_count",iNum+1,m+1,n+1,"%lu",psData->did_count+psData->did_count_z);
      	 	 	utPltPutLoopVarF3(psDbHead,"endnum",iNum+1,m+1,n+1,"%lu",psData->endnum+psData->endnum_z);
      	 	 	utPltPutLoopVarF3(psDbHead,"ycdid_count",iNum+1,m+1,n+1,"%lu",psData->ycdid_count+psData->ycdid_count_z);
      	 	 	utPltPutLoopVarF3(psDbHead,"wh_count",iNum+1,m+1,n+1,"%lu",psData->wh_count+psData->wh_count_z);
      	 	 	utPltPutLoopVarF3(psDbHead,"ty_count",iNum+1,m+1,n+1,"%lu",psData->ty_count+psData->ty_count_z);
      	 	 	if(psData->did_count+psData->did_count_z-psData->ondid_count-psData->ondid_count_z-psData->ycdid_count-psData->ycdid_count_z-psData->wh_count-psData->wh_count_z-psData->ty_count-psData->ty_count_z>=0){
      	 	 		utPltPutLoopVarF3(psDbHead,"lxdid_count",iNum+1,m+1,n+1,"%lu",psData->did_count+psData->did_count_z-psData->ondid_count-psData->ondid_count_z-psData->ycdid_count-psData->ycdid_count_z-psData->wh_count-psData->wh_count_z-psData->ty_count-psData->ty_count_z);
      	 	  }
      	 	  else{
      	 	  	utPltPutLoopVarF3(psDbHead,"lxdid_count",iNum+1,m+1,n+1,"%lu",0);
      	 	  }
      	 	 	utPltPutLoopVarF3(psDbHead,"onuid_count",iNum+1,m+1,n+1,"%lu",psData->onuid_count+psData->onuid_count_z);
      	 	 	if(psData->did_count+psData->did_count_z-psData->wh_count-psData->wh_count_z-psData->ty_count-psData->ty_count_z>0){
      	 	 		utPltPutLoopVarF3(psDbHead,"rate",iNum+1,m+1,n+1,"%d%c",100*(psData->ondid_count+psData->ondid_count_z+psData->ycdid_count+psData->ycdid_count_z)/(psData->did_count+psData->did_count_z-psData->wh_count-psData->wh_count_z-psData->ty_count-psData->ty_count_z),'%');
      	 	 	}
      	 	 	else{
      	 	 		utPltPutLoopVar3(psDbHead,"rate",iNum+1,m+1,n+1,"0");
      	 	 	}
      	 	 	n++;
      	 	 }
      	 	
      	 	iCount ++;
      	 	psData = (struct areastatdata_s *)pasLHashNext(&sHashInfo);
      	}	 	  
    	 	  
    	 	  
    	 	  
    	 	
    	}
    	
printf("four\n");    	
    	iNum++;

    	lSum_did=lSum_did+pAreastatdata[i].did_count;
    	lSum_ondid=lSum_ondid+pAreastatdata[i].ondid_count;
    	lSum_ycdid=lSum_ycdid+pAreastatdata[i].ycdid_count;
    	lSum_whcount=lSum_whcount+pAreastatdata[i].wh_count;
    	lSum_tycount=lSum_tycount+pAreastatdata[i].ty_count;
    	if(lSum_lxdid+pAreastatdata[i].did_count-pAreastatdata[i].ondid_count-pAreastatdata[i].ycdid_count-pAreastatdata[i].wh_count-pAreastatdata[i].ty_count>0){
    		lSum_lxdid=lSum_lxdid+pAreastatdata[i].did_count-pAreastatdata[i].ondid_count-pAreastatdata[i].ycdid_count-pAreastatdata[i].wh_count-pAreastatdata[i].ty_count;
    	}
    	else{
    		lSum_lxdid=0;
    	}
    	lSum_endnum=lSum_endnum+pAreastatdata[i].endnum;
    	lSum_onuid=lSum_onuid+pAreastatdata[i].onuid_count;
 //   	if(iNum>28) break;
    }

printf("vvaaa\n");    
 //   if(pAreastatdata) free(pAreastatdata);

		utPltPutVarF(psDbHead,"sum_did","%d",lSum_did);
		utPltPutVarF(psDbHead,"sum_ondid","%d",lSum_ondid);
		utPltPutVarF(psDbHead,"sum_ycdid","%d",lSum_ycdid);
		utPltPutVarF(psDbHead,"sum_lxdid","%d",lSum_lxdid);
		utPltPutVarF(psDbHead,"sum_endnum","%d",lSum_endnum);
		utPltPutVarF(psDbHead,"sum_onuid","%d",lSum_onuid);
		utPltPutVarF(psDbHead,"wh_count","%d",lSum_whcount);
		utPltPutVarF(psDbHead,"ty_count","%d",lSum_tycount);

      	 	 	if(lSum_did-lSum_whcount-lSum_tycount>0){
      	 	 		utPltPutVarF(psDbHead,"rate","%d%c",100*(lSum_ondid)/(lSum_did-lSum_whcount-lSum_tycount),'%');
      	 	 	}
      	 	 	else{
      	 	 		utPltPutVar(psDbHead,"rate","0");
      	 	 	}

        if(pHash_dep) free(pHash_dep);
	        for(i=0;i<lSum;i++){ 
	        		free(pAreastatdata[lSum].pArea);
	        }
	 
        free(pAreastatdata);
        free(pHash_wh);
printf("five\n");
//utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_first_onlinestat.htm");
    return 0;
}



//当前告警信息
int ncsWarnInfoOnline_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
 	  
 	  lCtime=time(0)+3600;
 	  
 	  char caKflag[16],caSid[1024];
	 long lFlags;
	 char caFlag[16],caSet[16];
 	 char caGroups[1024];
   char caFacs[1024];
   char caUsername2[32];
   long lSid;
   //获取权限组ID和接入厂商
    strcpy(caFacs,getDsFacGroupid());
    strcpy(caGroups,getDsGroupid());
   
 	  memset(caOpt,0,sizeof(caOpt)); 
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);

    utMsgPrintMsg(psMsgHead); 

	iReturn = utMsgGetSomeNVar(psMsgHead,11,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag,
   	  "cid",          UT_TYPE_STRING,10,caCid,
   	  "kflag",UT_TYPE_STRING,8,caKflag,
			"sid",UT_TYPE_STRING,1000,caSid,
			"set",UT_TYPE_STRING,10,caSet,
			"flag",UT_TYPE_STRING,10,caFlag);
			
 	 	 	memset(caUsername2,0,sizeof(caUsername2));         
      dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
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

    
    if(strcmp(caSet,"set")==0&&strlen(caSid)>0){
	  	sprintf(caTemp,"update ncscasemesg set flags=%d,opname='%s',optime=%d where sid in(%s)",atol(caFlag),caUsername2,time(0),caSid);
	//  	printf("caTemp=%s\n",caTemp);
	  	pasDbExecSql(caTemp,0);
	  }
    
    
 /*   
    js_flag=0;
    if(iReturn==0){
 	    pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);

    }
  */ 
   psDbHead = utPltInitDb();
   
  if(strlen(caFacs)>0||strlen(caGroups)>0){
  	sprintf(caTemp,"select count(*) from ncscasemesg a,ncscaseinfo b,ncsuser c where a.cid=b.sid and b.flags=1 and a.flags!=9 and a.did=c.userid  ");
  	if(strlen(caGroups)>0){
  		sprintf(caTemp+strlen(caTemp)," and c.groupid in (%s) ",caGroups);
  	}
  	if(strlen(caFacs)>0){
  		sprintf(caTemp+strlen(caTemp)," and c.fcode in (%s) ",caFacs);
  	}
  }
  else{
  	sprintf(caTemp,"select count(*) from ncscasemesg a,ncscaseinfo b where a.cid=b.sid and b.flags=1 and a.flags!=9 ");
  }
  if(strlen(caKflag)>0){
    	sprintf(caTemp+strlen(caTemp)," and a.flags=%s ",caKflag);
  }
  
  
		if(strlen(caTime_flag)>0){
		  sprintf(caTemp+strlen(caTemp)," and a.stime>=%d and a.stime<%d ",lstime,letime+3600);
    }
  if(strlen(caKeyword)>0){
  	 sprintf(caTemp+strlen(caTemp)," and (a.udisp like '%c%s%c' or inet_ntoa(a.sip)='%s' or a.message like '%c%s%c' or b.casename like '%c%s%c' or a.position='%s')  ",\
  	 '%',caKeyword,'%',caKeyword,'%',caKeyword,'%','%',caKeyword,'%',caKeyword);

  }
  if(strlen(caCid)>0){
  	sprintf(caTemp+strlen(caTemp)," and a.cid=%s ",caCid);
  }
  lCount=0;
//  printf("caKeyword=%s\n",caKeyword);

  pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
   
  utPltPutVarF(psDbHead,"TotRec", "%lu", lCount);
   

	 	if(strlen(caFacs)>0||strlen(caGroups)>0){
	  	sprintf(caTemp,"select a.stime,a.did,a.uid,a.udisp,a.sip,a.position,a.message,b.casename from ncscasemesg a,ncscaseinfo b,ncsuser c where a.cid=b.sid and b.flags=1 and a.flags!=9 and a.did=c.userid  ");
		  if(strlen(caGroups)>0){
	  		sprintf(caTemp+strlen(caTemp)," and c.groupid in (%s) ",caGroups);
	  	}
	  	if(strlen(caFacs)>0){
	  		sprintf(caTemp+strlen(caTemp)," and c.fcode in (%s) ",caFacs);
	  	}
	  	
	  	
		}
		else{
			  sprintf(caTemp,"select a.stime,a.did,a.uid,a.udisp,a.sip,a.position,a.message,b.casename,a.sid,a.flags from ncscasemesg a,ncscaseinfo b where a.cid=b.sid and b.flags=1 and a.flags!=9 ");		  	
		}
		if(strlen(caTime_flag)>0){
		  sprintf(caTemp+strlen(caTemp)," and a.stime>=%d and a.stime<%d ",lstime,letime+3600);
    }
// printf("aaaaaaaaaaa\n"); 
  if(strlen(caKeyword)>0){
  	 sprintf(caTemp+strlen(caTemp)," and (a.udisp like '%c%s%c' or inet_ntoa(a.sip)='%s' or a.message like '%c%s%c' or b.casename like '%c%s%c' or a.position='%s')  ",'%',caKeyword,'%',caKeyword,'%',caKeyword,'%','%',caKeyword,'%',caKeyword);
  }
  if(strlen(caCid)>0){
  	sprintf(caTemp+strlen(caTemp)," and a.cid=%s ",caCid);
  }
  if(strlen(caKflag)>0){
    	sprintf(caTemp+strlen(caTemp)," and a.flags=%s ",caKflag);
  }
  
	sprintf(caTemp+strlen(caTemp)," order by a.stime desc limit %d,%d ",lStartRec,lRowNum);

//printf("bbbbbbbbbbb\n");		
	printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

    if(psCur == NULL) {
  //      utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错 ");
        return 0;
    }   


         
   
    stime=0;
    did=0;
    uid=0;
    memset(udisp,0,sizeof(udisp));
    sip=0;
    memset(position,0,sizeof(position));
    memset(message,0,sizeof(message));
    memset(casename,0,sizeof(casename)); 
    lSid=0;
    lFlags=0;  
   	iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&stime,
   	                               UT_TYPE_LONG,4,&did,
   	                               UT_TYPE_LONG,4,&uid,
   	                               UT_TYPE_STRING,32,udisp,
   	                               UT_TYPE_ULONG,4,&sip,
   	                               UT_TYPE_STRING,63,position,
   	                               UT_TYPE_STRING,254,message,
   	                               UT_TYPE_STRING,31,casename,
   	                               UT_TYPE_LONG,4,&lSid,
   	                               UT_TYPE_LONG,4,&lFlags);
   				       
   	iNum = 0;
   	
   	while(iReturn == 0||iReturn==1405 ) {
   		
   		  if(iNum>0){
        		utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
          }
          else{
          	utPltPutLoopVar(psDbHead,"dh",iNum+1,"");
          }
   		
        iNum++;
        memset(caTemp,0,sizeof(caTemp));
        if(stime>0){
        strcpy(caTemp,utTimFormat("%Y/%m/%d %H:%M:%S",stime));
        utPltPutLoopVar(psDbHead,"sdate",iNum,caTemp);
      }
        

        if(strlen(casename)>0)
          sprintf(caTemp+strlen(caTemp),"案件:%s告警 ",casename);
          
         sprintf(caTemp+strlen(caTemp),"%s ",ncsUtlGetClientNameById(psShmHead,did,"未知"));
         if(strlen(udisp)>0)
         sprintf(caTemp+strlen(caTemp),"%s ",udisp);
         
         if(sip>0)
         sprintf(caTemp+strlen(caTemp)," 在%s",utComHostIp(htonl(sip)));
         
         if(strlen(udisp)>0)
         sprintf(caTemp+strlen(caTemp),"(%s)",position);
         
         if(strlen(message)>0)
         sprintf(caTemp+strlen(caTemp),"%s",message);
 utPltSetCvtHtml(1);     
        utPltPutLoopVar(psDbHead,"message",iNum,message);
        utPltPutLoopVar(psDbHead,"cname",iNum,ncsUtlGetClientNameById(psShmHead,did,"未知"));
        utPltPutLoopVar(psDbHead,"dispname",iNum,udisp);
        utPltPutLoopVar(psDbHead,"sip",iNum,utComHostIp(htonl(sip)));
        utPltPutLoopVar(psDbHead,"position",iNum,position);
 utPltSetCvtHtml(0);       
       
        utPltPutLoopVarF(psDbHead,"did",iNum,"%lu",did);
        utPltPutLoopVarF(psDbHead,"uid",iNum,"%lu",uid);
        utPltPutLoopVarF(psDbHead,"stime",iNum,"%lu",stime-7200);
        utPltPutLoopVarF(psDbHead,"etime",iNum,"%lu",stime+7200);
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",lSid);
         utPltPutLoopVarF(psDbHead,"flag",iNum,"%d",lFlags);
//printf("caTemp=%s\n",caTemp);
    stime=0;
    did=0;
    uid=0;
    memset(udisp,0,sizeof(udisp));
    sip=0;
    memset(position,0,sizeof(position));
    memset(message,0,sizeof(message));
    memset(casename,0,sizeof(casename));   
    lSid=0;
    lFlags=0;  
   	iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&stime,
   	                               UT_TYPE_LONG,4,&did,
   	                               UT_TYPE_LONG,4,&uid,
   	                               UT_TYPE_STRING,32,udisp,
   	                               UT_TYPE_ULONG,4,&sip,
   	                               UT_TYPE_STRING,63,position,
   	                               UT_TYPE_STRING,254,message,
   	                               UT_TYPE_STRING,31,casename,
   	                               UT_TYPE_LONG,4,&lSid,
   	                               UT_TYPE_LONG,4,&lFlags);
    }
    pasDbCloseCursor(psCur);

//  utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_first_warnonline.htm");
    return 0;
}



//显示案件列表
int ncsCaseList_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caTemp[4048];
    char caSelsid[1024];
    char caCond[1024],caSorta[12],caOrdera[12];
    char caCasecode[24],caCasename[36],caCasedept[64],caAddby[20],cmemo[1024],caGroupid[16];
    unsigned long sid,casetype,caseclass,groupid,casedate,lasttime,flags;
    char caTotPg[16],caCurPg[16];
    char caCasetype[16],caCaseclass[16],caSdate[12],caEdate[12],caFlags[16],caRycx[64];
    unsigned long lCurPg,lTotPg;
    int lCount;
    int i,iNum;
    unsigned long lId;
    char caDel[16],caExport[16],caApp[16],caCk[16];
    utPltDbHead *psDbHead;
   	int iReturn;
   	long lUsrlevel;
   	char caOpt[256];
   	long lRowNum,lStartRec,lTotRec;
   	unsigned long lExpiretime;
   	char caAddby2[16];
   	long lGroupid,lCasetype;
   	char caCmemo[1024],caGroupname[36];
   	char caRownum[16];
   	char caHf[16];
 	  pasDbCursor *psCur;

   	char caUsername2[32];
   	char caSid[16],caUpdate[16];
    char caMsg[1024];
    unsigned long lCurtime,lTamp,lSx,lExpire,lSid;
    char caKeyword[64],caTemp_d[64],caExpiretime[16],caGroupid1[16];
    unsigned char *pHash;
    struct caseData_s{
    	unsigned long id;
    	unsigned long count;       //策略数
    	unsigned long lWarncount;   //告警数
    	unsigned long lXfcount;     //下发单位数
    };
    struct caseData_s *pData;
    lCurtime=time(0);
    
    	utMsgPrintMsg(psMsgHead);
    
    memset(caUsername2,0,sizeof(caUsername2));
    dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
	iReturn = utMsgGetSomeNVar(psMsgHead,23,
	    "TotPg",      UT_TYPE_STRING, 12, caTotPg,
			"page",      UT_TYPE_STRING, 12, caCurPg,
			"limit",     UT_TYPE_STRING,10,caRownum,
			"groupid1",UT_TYPE_STRING,12,caGroupid,
			"groupid",UT_TYPE_STRING,10,caGroupid1,
			"casecode",UT_TYPE_STRING,20,caCasecode,
			"casename",UT_TYPE_STRING,32,caCasename,
			"casetype",UT_TYPE_STRING,12,caCasetype,
			"caseclass",UT_TYPE_STRING,12,caCaseclass,
			"casedept",UT_TYPE_STRING,60,caCasedept,
			"addby",UT_TYPE_STRING,15,caAddby,
			"flags",UT_TYPE_STRING,2,caFlags,
			"update",UT_TYPE_STRING,10,caUpdate,
			"sid",UT_TYPE_STRING,10,caSid,
			"keyword",UT_TYPE_STRING,31,caKeyword,
			"cmemo",UT_TYPE_STRING,250,caCmemo,
			"expiretime",UT_TYPE_STRING,10,caExpiretime,
			"del",UT_TYPE_STRING,8,caDel,
			"app",UT_TYPE_STRING,3,caApp,
      "exp",     UT_TYPE_STRING,10,caExport,
      "selsid",  UT_TYPE_STRING,1000,caSelsid,
      "ck",      UT_TYPE_STRING,10,caCk,
      "hf",      UT_TYPE_STRING,10,caHf);
   	  
      if(strlen(caKeyword)>0){
      	 strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
    	   strcpy(caKeyword,caTemp_d);
      }
      if(strlen(caCasecode)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caCasecode,caTemp,20));
      	strcpy(caCasecode,caTemp_d);
      }
      if(strlen(caCasename)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caCasename,caTemp,30));
      	strcpy(caCasename,caTemp_d);
      }
      if(strlen(caCasedept)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caCasedept,caTemp,30));
      	strcpy(caCasedept,caTemp_d);
      }
      if(strlen(caCmemo)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caCmemo,caTemp,250));
      	strcpy(caCmemo,caTemp_d);
      }
   
     lTamp=atol(caExpiretime);
     if(lTamp==1){
     	lExpire=lCurtime+3*3600*24;
     }
     else if(lTamp==2){
     	lExpire=lCurtime+7*3600*24;
    }
    else if(lTamp==3){
    	lExpire=lCurtime+30*3600*24;
    }
    else if(lTamp==4){
    	lExpire=lCurtime+90*3600*24;
    }
    else if(lTamp==5){
    	lExpire=lCurtime+180*3600*24;
    }
    else if(lTamp==6){
    	lExpire=lCurtime+365*3600*24;
    }
    else if(lTamp==7){
    	lExpire=lCurtime+730*3600*24;
    }
    else if(lTamp==8){
    	lExpire=lCurtime+1800*3600*24;
    }
    else{
    	lExpire=lCurtime;
    }
     if(strcmp(caUpdate,"update")==0){
        lSid=atol(caSid);

        if(lSid>0){
        	printf("update ncscaseinfo set casecode='%s',casename='%s',casedept='%s',addby='%s',casetype=%lu,caseclass=%lu,groupid=%lu,casedate=%lu,flags=1,lasttime=%lu,cmemo='%s',expiretime=%lu where sid=%d \n",
         caCasecode,caCasename,caCasedept,caUsername2,atol(caCasetype),atol(caCaseclass),atol(caGroupid1),lCurtime,lCurtime,caCmemo,lExpire,lSid);
        	 pasDbExecSqlF("update ncscaseinfo set casecode='%s',casename='%s',casedept='%s',addby='%s',casetype=%lu,caseclass=%lu,groupid=%lu,flags=1,lasttime=%lu,cmemo='%s',expiretime=%lu where sid=%d ",
         caCasecode,caCasename,caCasedept,caUsername2,atol(caCasetype),atol(caCaseclass),atol(caGroupid1),lCurtime,caCmemo,lExpire,lSid);
        }
        else{
        	sprintf(caTemp,"%s%s",caCasecode,caCasename);
        	lSid=utMd5Code(caTemp,strlen(caTemp),"ffff");
        	if(lSid>2000000000) lSid=lSid-2000000000;      
          if(lSid>2000000000) lSid=lSid-2000000000;
          sprintf(caTemp,"select count(*) from ncscaseinfo where sid=%d ",lSid);
          lCount=0;
          pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
          if(lCount>0){
          	 pasDbExecSqlF("update ncscaseinfo set casecode='%s',casename='%s',casedept='%s',addby='%s',casetype=%lu,caseclass=%lu,groupid=%lu,flags=1,lasttime=%lu,cmemo='%s',expiretime=%lu where sid=%d ",
              caCasecode,caCasename,caCasedept,caUsername2,atol(caCasetype),atol(caCaseclass),atol(caGroupid),lCurtime,caCmemo,lExpire,lSid);
          }
          else{
          	printf("insert into ncscaseinfo(sid,casecode,casename,casedept,addby,casetype,caseclass,groupid,casedate,flags,lasttime,cmemo,expiretime) \
          	  values(%lu,'%s','%s','%s','%s',%d,%d,%d,%d,1,%d,'%s',%d)\n",
          	  lSid,caCasecode,caCasename,caCasedept,caUsername2,atol(caCasetype),atol(caCaseclass),atol(caGroupid1),lCurtime,lCurtime,caCmemo,lExpire);
    
          	pasDbExecSqlF("insert into ncscaseinfo(sid,casecode,casename,casedept,addby,casetype,caseclass,groupid,casedate,flags,lasttime,cmemo,expiretime) \
          	  values(%lu,'%s','%s','%s','%s',%d,%d,%d,%d,1,%d,'%s',%d)",
          	  lSid,caCasecode,caCasename,caCasedept,caUsername2,atol(caCasetype),atol(caCaseclass),atol(caGroupid1),lCurtime,lCurtime,caCmemo,lExpire);
          }
        	
        	
        	
        }
    }
   
   
   
   
   
   
    memset(caOpt,0,sizeof(caOpt)); 
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    if(iReturn==0){
 	    sprintf(caTemp,"select usrlevel,opt from dsuser where id=%lu",lId);
 	    pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lUsrlevel,
 	                        UT_TYPE_STRING,255,caOpt);
    }  
 //   if(strlen(caOpt)==0) strcpy(caOpt,"0");
    if(!utStrIsSpaces(caDel)&&strlen(caSelsid)>0){
  	
  	    char sqlbuf[512];
  	    char casecode[16];

  	
  			    sprintf(sqlbuf,"select casecode from ncscaseinfo where sid in (%s)",caSelsid);
  			    memset(casecode,0,sizeof(casecode));
  			    pasDbOneRecord(sqlbuf,0,UT_TYPE_STRING,15,casecode);
  			    sprintf(sqlbuf,"删除案件 编号:%s",casecode);
  			    ncsWriteSysLog(psMsgHead,caUsername2,"04","成功",sqlbuf); 
  			    sprintf(caTemp,"<DSSQL>delete from ncscaseobj where cid in (%s)</DSSQL>\n",caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>delete from ncscaseuser where cid in (%s)</DSSQL>\n",caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>delete from ncscasenetid where cid in (%s)</DSSQL>\n",caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>delete from ncscaseip where cid in (%s)</DSSQL>\n",caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>delete from ncscaseweb where cid in (%s)</DSSQL>\n",caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>delete from ncscasepost where cid in (%s)</DSSQL>\n",caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>delete from ncscaseim where cid in (%s)</DSSQL>\n",caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>delete from ncscasemail where cid in (%s)</DSSQL>\n",caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>delete from ncscasewarn where cid in (%s)</DSSQL>\n",caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>delete from ncscasectllog where cid in (%s)</DSSQL>\n",caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscaseinfo set flags=8,lasttime=%lu where sid in(%s)</DSSQL>\n",time(0),caSelsid);
  			    iReturn = dsCltSqlExecSome(caTemp);
  	
  
  	}
     if(!utStrIsSpaces(caCk)&&strlen(caSelsid)>0){
  	
  	    char sqlbuf[512];
  	    char casecode[16];

  	
  			    sprintf(sqlbuf,"select casecode from ncscaseinfo where sid in (%s)",caSelsid);
  			    memset(casecode,0,sizeof(casecode));
  			    pasDbOneRecord(sqlbuf,0,UT_TYPE_STRING,15,casecode);
  			    sprintf(sqlbuf,"撤控编号:%s",casecode);
  			    ncsWriteSysLog(psMsgHead,caUsername2,"04","成功",sqlbuf); 
 			    sprintf(caTemp,"<DSSQL>update ncscaseobj set flags=1 where cid in (%s)</DSSQL>\n",caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscaseuser set flags=1,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscasenetid set flags=1,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscaseip set flags=1,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscaseweb set flags=1,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscasepost set flags=1,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscaseim set flags=1,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscasemail set flags=1,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscasewarn set flags=1 ,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscaseinfo set flags=9,lasttime=%lu where sid in (%s) </DSSQL>\n",time(0),caSelsid);
  			    iReturn = dsCltSqlExecSome(caTemp);
  	
  
  	}
 
 //恢复撤控
      if(!utStrIsSpaces(caHf)&&strlen(caSelsid)>0){
  	
  	    char sqlbuf[512];
  	    char casecode[16];

  	
  			    sprintf(sqlbuf,"select casecode from ncscaseinfo where sid in (%s)",caSelsid);
  			    memset(casecode,0,sizeof(casecode));
  			    pasDbOneRecord(sqlbuf,0,UT_TYPE_STRING,15,casecode);
  			    sprintf(sqlbuf,"恢复撤控编号:%s",casecode);
  			    ncsWriteSysLog(psMsgHead,caUsername2,"04","成功",sqlbuf); 
 			    sprintf(caTemp,"<DSSQL>update ncscaseobj set flags=0 where cid in (%s)</DSSQL>\n",caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscaseuser set flags=0,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscasenetid set flags=0,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscaseip set flags=0,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscaseweb set flags=0,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscasepost set flags=0,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscaseim set flags=0,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscasemail set flags=0,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscasewarn set flags=0 ,lasttime=%lu where cid in (%s)</DSSQL>\n",time(0),caSelsid);
  			    sprintf(caTemp+strlen(caTemp),"<DSSQL>update ncscaseinfo set flags=1,lasttime=%lu where sid in (%s) </DSSQL>\n",time(0),caSelsid);
  			    iReturn = dsCltSqlExecSome(caTemp);
  	
  
  	}
 
 	
  	

  	if(!utStrIsSpaces(caApp)) {
  	    ncsSystemInfo *psInfo;
  	    psInfo = (ncsSystemInfo *)utShmArray(psShmHead,NCS_LNK_SYSINFO);
        if(psInfo) {
        	   ncsResetSystem(psShmHead);
 //           ncsUtlLoadCaseCtl(psInfo);
  	    }
  	}
  	
  	pHash=(unsigned char *)pasLHashInit(30000,30000,sizeof(struct caseData_s),0,4); 
    if(pHash==NULL){
    	return 0;
    }
    //统计虚拟身份布控策略
    sprintf(caTemp,"select count(*),a.sid from ncscaseinfo a,ncscasenetid b where a.sid=b.cid and b.flags!=8 ");
    if(strlen(caFlags)!=0){
				sprintf(caTemp+strlen(caTemp)," and a.flags=%s ",caFlags);
    }
    sprintf(caTemp+strlen(caTemp)," group by a.sid ");
    printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur==NULL) { 
    		if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lCount=0;
    lSid=0;
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    while(iReturn==0||iReturn==1405){
    	  pData=(struct caseData_s *)pasLHashLookA(pHash,&lSid); 
    	  if(pData){
    	  	pData->count=pData->count+lCount;
    	  } 
    	 lCount=0;
    	 lSid=0;
       iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    }                             
  	pasDbCloseCursor(psCur);  
  	
  	//统计表单布控策略
    sprintf(caTemp,"select count(*),a.sid from ncscaseinfo a,ncscasepost b where a.sid=b.cid and b.flags!=8 ");
    if(strlen(caFlags)!=0){
				sprintf(caTemp+strlen(caTemp)," and a.flags=%s ",caFlags);
    }
    sprintf(caTemp+strlen(caTemp)," group by a.sid ");
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur==NULL) { 
    		if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lCount=0;
    lSid=0;
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    while(iReturn==0||iReturn==1405){
    	  pData=(struct caseData_s *)pasLHashLookA(pHash,&lSid); 
    	  if(pData){
    	  	pData->count=pData->count+lCount;
    	  } 
    	 lCount=0;
    	 lSid=0;
       iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    }                             
  	pasDbCloseCursor(psCur); 
  	
  //统计使用者特征布控策略
    sprintf(caTemp,"select count(*),a.sid from ncscaseinfo a,ncscaseuser b where a.sid=b.cid and b.flags!=8 ");
    if(strlen(caFlags)!=0){
				sprintf(caTemp+strlen(caTemp)," and a.flags=%s ",caFlags);
    }
    sprintf(caTemp+strlen(caTemp)," group by a.sid ");
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur==NULL) { 
    		if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lCount=0;
    lSid=0;
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    while(iReturn==0||iReturn==1405){
    	  pData=(struct caseData_s *)pasLHashLookA(pHash,&lSid); 
    	  if(pData){
    	  	pData->count=pData->count+lCount;
    	  } 
    	 lCount=0;
    	 lSid=0;
       iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    }                             
  	pasDbCloseCursor(psCur); 
  	
  	//统计网址库布控策略
    sprintf(caTemp,"select count(*),a.sid from ncscaseinfo a,ncscaseweb b where a.sid=b.cid and b.flags!=8 ");
    if(strlen(caFlags)!=0){
				sprintf(caTemp+strlen(caTemp)," and a.flags=%s ",caFlags);
    }
    sprintf(caTemp+strlen(caTemp)," group by a.sid ");
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur==NULL) { 
    		if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lCount=0;
    lSid=0;
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    while(iReturn==0||iReturn==1405){
    	  pData=(struct caseData_s *)pasLHashLookA(pHash,&lSid); 
    	  if(pData){
    	  	pData->count=pData->count+lCount;
    	  } 
    	 lCount=0;
    	 lSid=0;
       iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    }                             
  	pasDbCloseCursor(psCur); 
  	
  	//统计邮件布控策略
    sprintf(caTemp,"select count(*),a.sid from ncscaseinfo a,ncscasemail b where a.sid=b.cid and b.flags!=8 ");
    if(strlen(caFlags)!=0){
				sprintf(caTemp+strlen(caTemp)," and a.flags=%s ",caFlags);
    }
    sprintf(caTemp+strlen(caTemp)," group by a.sid ");
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur==NULL) { 
    		if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lCount=0;
    lSid=0;
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    while(iReturn==0||iReturn==1405){
    	  pData=(struct caseData_s *)pasLHashLookA(pHash,&lSid); 
    	  if(pData){
    	  	pData->count=pData->count+lCount;
    	  } 
    	 lCount=0;
    	 lSid=0;
       iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    }                             
  	pasDbCloseCursor(psCur); 
  	
  	//统计IP布控策略
    sprintf(caTemp,"select count(*),a.sid from ncscaseinfo a,ncscaseip b where a.sid=b.cid and b.flags!=8 ");
    if(strlen(caFlags)!=0){
				sprintf(caTemp+strlen(caTemp)," and a.flags=%s ",caFlags);
    }
    sprintf(caTemp+strlen(caTemp)," group by a.sid ");
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur==NULL) { 
    		if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lCount=0;
    lSid=0;
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    while(iReturn==0||iReturn==1405){
    	  pData=(struct caseData_s *)pasLHashLookA(pHash,&lSid); 
    	  if(pData){
    	  	pData->count=pData->count+lCount;
    	  } 
    	 lCount=0;
    	 lSid=0;
       iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    }                             
  	pasDbCloseCursor(psCur); 
  	
  	//统计聊天内容布控策略
    sprintf(caTemp,"select count(*),a.sid from ncscaseinfo a,ncscaseim b where a.sid=b.cid and b.flags!=8 ");
    if(strlen(caFlags)!=0){
				sprintf(caTemp+strlen(caTemp)," and a.flags=%s ",caFlags);
    }
    sprintf(caTemp+strlen(caTemp)," group by a.sid ");
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur==NULL) { 
    		if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lCount=0;
    lSid=0;
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    while(iReturn==0||iReturn==1405){
    	  pData=(struct caseData_s *)pasLHashLookA(pHash,&lSid); 
    	  if(pData){
    	  	pData->count=pData->count+lCount;
    	  } 
    	 lCount=0;
    	 lSid=0;
       iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    }                             
  	pasDbCloseCursor(psCur); 
  	
  	//统计敏感服务布控策略
    sprintf(caTemp,"select count(*),a.sid from ncscaseinfo a,ncscaseservice b where a.sid=b.cid and b.flags!=8 ");
    if(strlen(caFlags)!=0){
				sprintf(caTemp+strlen(caTemp)," and a.flags=%s ",caFlags);
    }
    sprintf(caTemp+strlen(caTemp)," group by a.sid ");
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur==NULL) { 
    		if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lCount=0;
    lSid=0;
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    while(iReturn==0||iReturn==1405){
    	  pData=(struct caseData_s *)pasLHashLookA(pHash,&lSid); 
    	  if(pData){
    	  	pData->count=pData->count+lCount;
    	  } 
    	 lCount=0;
    	 lSid=0;
       iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    }                             
  	pasDbCloseCursor(psCur); 
  	
 //统计告警数
  	  	
    sprintf(caTemp,"select count(*),a.cid from ncscasemesg a,ncscaseinfo b where b.sid=a.cid and b.flags!=8 ");
    if(strlen(caFlags)!=0){
				sprintf(caTemp+strlen(caTemp)," and b.flags=%s ",caFlags);
    }
    sprintf(caTemp+strlen(caTemp)," group by a.cid ");
    printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur==NULL) { 
    		if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lCount=0;
    lSid=0;
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    while(iReturn==0||iReturn==1405){
    	  pData=(struct caseData_s *)pasLHashLookA(pHash,&lSid); 
    	  if(pData){
    	  	pData->lWarncount=pData->lWarncount+lCount;
    	  } 
    	 lCount=0;
    	 lSid=0;
       iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    }                             
  	pasDbCloseCursor(psCur); 
  	
  	
  	 //统计下发单位数
  	  	
    sprintf(caTemp,"select count(*),a.cid from ncscasectllog a,ncscaseinfo b where b.sid=a.cid and b.flags!=8 and a.flags=1 ");
    if(strlen(caFlags)!=0){
				sprintf(caTemp+strlen(caTemp)," and b.flags=%s ",caFlags);
    }
    sprintf(caTemp+strlen(caTemp)," group by a.cid ");
    printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur==NULL) { 
    		if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lCount=0;
    lSid=0;
    iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    while(iReturn==0||iReturn==1405){
    	  pData=(struct caseData_s *)pasLHashLookA(pHash,&lSid); 
    	  if(pData){
    	  	pData->lXfcount=pData->lXfcount+lCount;
    	  } 
    	 lCount=0;
    	 lSid=0;
       iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lCount,
                                 UT_TYPE_LONG,4,&lSid);
    }                             
  	pasDbCloseCursor(psCur); 
  	
  	
  	
    memset(caCond,0,sizeof(caCond)); 
 if(!strstr(caOpt,"all")&&(strlen(caOpt)>0)){
    	sprintf(caCond+strlen(caCond)," and (a.groupid in(%s) or casetype=1 or a.groupid=0)",caOpt);
    }
   
   if(strlen(caKeyword)>0){
   	  sprintf(caCond+strlen(caCond)," and (casecode like '%c%s%c' or casename like '%c%s%c' or casedept like '%c%s%c')",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
   } 
  



	if(strlen(caFlags)!=0)
	sprintf(caCond+strlen(caCond)," and flags=%s ",caFlags);
	if(strlen(caGroupid)!=0)
	sprintf(caCond+strlen(caCond)," and a.groupid=%s ",caGroupid);
	
	

	    strcat(caCond," order by casedate ");
	   
	    strcat(caCond," desc ");
	
    lCurPg = atol(caCurPg);
    if(lCurPg <= 0) lCurPg = 1;

 

    if(!utStrIsSpaces(caExport)){
    lRowNum = 999999;
    }
    else
    lRowNum=atol(caRownum);
    
    lStartRec = (lCurPg - 1) * lRowNum;    


   
    sprintf(caTemp,"select count(*) from ncscaseinfo a where flags!=8 ");
    
 
    if(strlen(caCond)!=0)
    strcat(caTemp,caCond);
    
//  printf("caTemp=%s\n",caTemp);
    iReturn = pasDbOneRecord(caTemp,0,
                            UT_TYPE_LONG,4,&lCount);

    if(iReturn != 0) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错 ");
        return 0;
    }  
    
    sprintf(caTemp,"select sid,casecode,casename,caseclass,casedept,casedate,flags,expiretime,addby,a.groupid,cmemo,casetype,b.groupname from ncscaseinfo a left join ncsgroup b on (a.groupid=b.groupid) where flags!=8");
    
 
    if(strlen(caCond)!=0)
    strcat(caTemp,caCond);
   
if(strstr(caTemp,"order by")==NULL)
strcat(caTemp," order by sid desc");

    sprintf(caTemp+strlen(caTemp)," limit %lu,%lu ",lStartRec,lRowNum);


printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);

    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","查询数据出错 ");
        return 0;
    }   

    if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
        sprintf(caFilename,"ncscase%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
        
        fprintf(fp,ncsLang("案件列表\n"));
      ncUtlPrintCsv(fp,6,UT_TYPE_STRING,ncsLang("案件编号"),
                       UT_TYPE_STRING,"案件名称",
                       UT_TYPE_STRING,"案件类别",
                       UT_TYPE_STRING,"立案时间",
                       UT_TYPE_STRING,"立案单位",
                       UT_TYPE_STRING,"状态");
                       
                                      
       memset(caCasecode,0,sizeof(caCasecode));
       memset(caCasename,0,sizeof(caCasename));
       memset(caCasedept,0,sizeof(caCasedept));
       memset(caAddby2,0,sizeof(caAddby2));   
       sid=0;
       caseclass=0;
       casedate=0;  
       flags=0;
       lExpiretime=0;
       lGroupid=0;
       memset(caCmemo,0,sizeof(caCmemo));
       lCasetype=0;
       memset(caGroupname,0,sizeof(caGroupname));
   	iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&sid,
   							 UT_TYPE_STRING,20,caCasecode,
   							 UT_TYPE_STRING,32,caCasename,
   							 UT_TYPE_LONG,4,&caseclass,
   							 UT_TYPE_STRING,60,caCasedept,
   							 UT_TYPE_ULONG,4,&casedate,
   							 UT_TYPE_LONG,4,&flags,
   							 UT_TYPE_LONG,4,&lExpiretime,
   							 UT_TYPE_STRING,15,caAddby2,
   							 UT_TYPE_LONG,4,&lGroupid,
   							 UT_TYPE_STRING,255,caCmemo,
   							 UT_TYPE_LONG,4,&lCasetype,
   							 UT_TYPE_STRING,31,caGroupname);
   				      
  	                              
        while((iReturn == 0)||(iReturn==1405)) {
      
               
        ncUtlPrintCsv(fp,6,UT_TYPE_STRING,caCasecode,
        	             UT_TYPE_STRING,caCasename,
                       UT_TYPE_STRING,ncsGetCaseTypeById(caseclass),
                       UT_TYPE_STRING,utTimFormat("%Y/%m/%d",casedate),
                       UT_TYPE_STRING,caCasedept,
                       UT_TYPE_STRING,ncsGetCaseStateById(flags));
                      
                      
       memset(caCasecode,0,sizeof(caCasecode));
       memset(caCasename,0,sizeof(caCasename));
       memset(caCasedept,0,sizeof(caCasedept));
          
       sid=0;
       caseclass=0;
       casedate=0;  
       flags=0;
       lExpiretime=0;
       memset(caAddby2,0,sizeof(caAddby2));
       lGroupid=0;
       memset(caCmemo,0,sizeof(caCmemo));
       lCasetype=0;
       memset(caGroupname,0,sizeof(caGroupname));
   	iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&sid,
   							 UT_TYPE_STRING,20,caCasecode,
   							 UT_TYPE_STRING,32,caCasename,
   							 UT_TYPE_LONG,4,&caseclass,
   							 UT_TYPE_STRING,60,caCasedept,
   							 UT_TYPE_ULONG,4,&casedate,
   							 UT_TYPE_LONG,4,&flags,
   							 UT_TYPE_LONG,4,&lExpiretime,
   							 UT_TYPE_STRING,15,caAddby2,
   							 UT_TYPE_LONG,4,&lGroupid,
   							 UT_TYPE_STRING,255,caCmemo,
   							 UT_TYPE_LONG,4,&lCasetype,
   							 UT_TYPE_STRING,31,caGroupname);
        }
        
        
        pasDbCloseCursor(psCur);
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        if(pHash) free(pHash);
        return 0;
    }
         
    psDbHead = utPltInitDb();
  
    
       memset(caCasecode,0,sizeof(caCasecode));
       memset(caCasename,0,sizeof(caCasename));
       memset(caCasedept,0,sizeof(caCasedept));
          
       sid=0;
       caseclass=0;
       casedate=0;  
       flags=0;
       lExpiretime=0;
       memset(caAddby2,0,sizeof(caAddby2));
       lGroupid=0;
       memset(caCmemo,0,sizeof(caCmemo));
       lCasetype=0;
       memset(caGroupname,0,sizeof(caGroupname));
   	iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&sid,
   							 UT_TYPE_STRING,20,caCasecode,
   							 UT_TYPE_STRING,32,caCasename,
   							 UT_TYPE_LONG,4,&caseclass,
   							 UT_TYPE_STRING,60,caCasedept,
   							 UT_TYPE_ULONG,4,&casedate,
   							 UT_TYPE_LONG,4,&flags,
   							 UT_TYPE_LONG,4,&lExpiretime,
   							 UT_TYPE_STRING,15,caAddby2,
   							 UT_TYPE_LONG,4,&lGroupid,
   							 UT_TYPE_STRING,255,caCmemo,
   							 UT_TYPE_LONG,4,&lCasetype,
   							 UT_TYPE_STRING,31,caGroupname);
   	iNum = 0;
   	
   	while(iReturn == 0||iReturn==1405 && iNum < lRowNum) {
   		if(iNum>0){
   			utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
   		}
        iNum++;
       
        utPltPutLoopVarF(psDbHead,"num",iNum,"%d",iNum);
        utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",sid);
        utPltPutLoopVarF(psDbHead,"sjjbid",iNum,"%d",caseclass);
        utPltPutLoopVarF(psDbHead,"ajzlid",iNum,"%d",lCasetype);
        utPltPutLoopVar(psDbHead,"ajzl",iNum,ncsGetCaseCatById(lCasetype));
        utPltPutLoopVar(psDbHead,"casecode",iNum,caCasecode);
        utPltPutLoopVar(psDbHead,"casename",iNum,caCasename);
        utPltPutLoopVar(psDbHead,"addby",iNum,caAddby2);
        utPltPutLoopVar(psDbHead,"caseclass", iNum,ncsGetCaseTypeById(caseclass));
        utPltPutLoopVar(psDbHead,"casedate", iNum,utTimFormat("%Y/%m/%d",casedate));
        utPltPutLoopVar(psDbHead,"expiretime", iNum,utTimFormat("%Y/%m/%d",lExpiretime));
        utPltPutLoopVar(psDbHead,"casedept",iNum,caCasedept);
        utPltPutLoopVar(psDbHead,"flags",iNum,ncsGetCaseStateById(flags));
        utPltPutLoopVar(psDbHead,"groupname",iNum,caGroupname);
        
        //输出策略数
        pData=(struct caseData_s *)pasLHashLook(pHash,&sid); 
    	  if(pData){
    	  	utPltPutLoopVarF(psDbHead,"pcount",iNum,"%d",pData->count);  
    	  	utPltPutLoopVarF(psDbHead,"wcount",iNum,"%d",pData->lWarncount);
    	  	utPltPutLoopVarF(psDbHead,"xcount",iNum,"%d",pData->lXfcount); 	  
    	  }
    	  else{
    	  	utPltPutLoopVarF(psDbHead,"pcount",iNum,"%d",0);
    	  	utPltPutLoopVarF(psDbHead,"wcount",iNum,"%d",0);
    	  	utPltPutLoopVarF(psDbHead,"xcount",iNum,"%d",0); 
    	  }
        
   if(lCurtime<lExpiretime){
   	lTamp=(lExpiretime-lCurtime)/(3600*24);
   	if(lTamp>0&&lTamp<=3){
   		lSx=1;
   	}
   	else if(lTamp>3&&lTamp<=7){
   		lSx=2;
   	}
   	else if(lTamp>7&&lTamp<=30){
   		lSx=3;
   	}
   	else if(lTamp>30&&lTamp<=90){
   		lSx=4;
   	}
   	else if(lTamp>90&&lTamp<=180){
   		lSx=5;
   	}
    else if(lTamp>180&&lTamp<=365){
    	lSx=6;
    }
    else if(lTamp>365&&lTamp<=730){
    	lSx=7;
    }
    else if(lTamp>730){
    	lSx=8;
    }
    else{
    	lSx=0;
    }
    
  }    
  else{lSx=0;
  } 
  utPltPutLoopVarF(psDbHead,"sxid",iNum,"%d",lSx);
        utPltPutLoopVarF(psDbHead,"groupid",iNum,"%d",lGroupid);
        utPltSetCvtHtml(1);
        utPltPutLoopVar(psDbHead,"cmemo",iNum,caCmemo);
        utPltSetCvtHtml(0);
       memset(caCasecode,0,sizeof(caCasecode));
       memset(caCasename,0,sizeof(caCasename));
       memset(caCasedept,0,sizeof(caCasedept));
          
       sid=0;
       caseclass=0;
       casedate=0;  
       flags=0;
       lExpiretime=0;
       memset(caAddby2,0,sizeof(caAddby2));
       lGroupid=0;
       memset(caCmemo,0,sizeof(caCmemo));
       lCasetype=0;
       memset(caGroupname,0,sizeof(caGroupname));
   	iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&sid,
   							 UT_TYPE_STRING,20,caCasecode,
   							 UT_TYPE_STRING,32,caCasename,
   							 UT_TYPE_LONG,4,&caseclass,
   							 UT_TYPE_STRING,60,caCasedept,
   							 UT_TYPE_ULONG,4,&casedate,
   							 UT_TYPE_LONG,4,&flags,
   							 UT_TYPE_LONG,4,&lExpiretime,
   							 UT_TYPE_STRING,15,caAddby2,
   							 UT_TYPE_LONG,4,&lGroupid,
   							 UT_TYPE_STRING,255,caCmemo,
   							 UT_TYPE_LONG,4,&lCasetype,
   							 UT_TYPE_STRING,31,caGroupname);
    }
    pasDbCloseCursor(psCur);
    lTotPg = (lCount - 1) / lRowNum + 1;
    lTotRec = lCount;
  
    
    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);

  if(pHash) free(pHash);
    ncsWriteSysLog(psMsgHead,caUsername2,"06","成功","查看案件列表信息");  
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_case_list.htm");
    return 0;
}





//单位组树——供comp选择使用

int ncsTreeGroup_comp_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
   	unsigned long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	char caCode[16];
   	long lId;
   	char *p,*pVar;
   	unsigned char *pHash;
    pasLHashInfo  sHashInfo,sHashInfo2;
    struct stData{
    	unsigned long groupid;
    	unsigned long pid;
    	char caGroupname[32];
    	char caCode[12];
    	struct stData * pArea;  //子节点
 		  long lSum;             //子节点个数
    };
    char caTemp[2048];
    long lCount,lSum,lSum2,m,n;
    char caGroups[1024];
     //获取权限组ID
      strcpy(caGroups,getDsGroupid());
    struct stData *psData,*pAreastatdata,*psData2,*psData3;
    
    
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);

  
     
 //   iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere,"\0");
 //   if(iReturn == 0 && lGroupid0 > 0) {
 //       sprintf(caWhere,"and (groupid = %lu or pid=%lu) ",lGroupid0,lGroupid0);
 //   }
    
  //  sprintf(caWhere+strlen(caWhere)," and level=%d ",atol(caLevel));

//    if(strlen(caPid)>0)
//      sprintf(caWhere+strlen(caWhere)," and pid=%d and level>0",atol(caPid));
//    else{
//    	 sprintf(caWhere+strlen(caWhere)," and pid=0 and level=0",atol(caPid));
//    }
 //    memset(caOpt,0,sizeof(caOpt)); 
 //   iReturn = dsCltGetMyInfo(1,"Userid",&lId);
 //   if(iReturn==0){
// 	    pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
//    } 



    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",1000);
  
    pHash = (unsigned char *)pasLHashInit(lRowNum,lRowNum,sizeof(struct stData),0,4);
    
    if(pHash == NULL) {
             printf(" Memory HttpHash Error \n");
             return (-1);
    }
    sprintf(caTemp,"select pid,groupid,groupname,groupcode from ncsgroup where 1=1 ");
   
 //  if(!strstr(caOpt,"all")&&(strlen(caOpt)>0)){
//			sprintf(caTemp+strlen(caTemp)," and (groupid in (%s) or groupid in (select groupid from ncsgroup where pid in (%s))) ",caOpt,caOpt);
//   }
   if(strlen(caGroups)>0){
   	sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caGroups);
  }
   printf("caTemp=%s\n",caTemp);
    psCur=pasDbOpenSql(caTemp,0);
  
    if(psCur==NULL){
    	free(pHash);
    	return 0;
    }
    lGroupid=0;
    lPid=0;
    memset(caGroupname,0,sizeof(caGroupname));
    memset(caCode,0,sizeof(caCode));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lPid,
                                   UT_TYPE_LONG,4,&lGroupid,
                                   UT_TYPE_STRING,31,caGroupname,
                                   UT_TYPE_STRING,10,caCode);
    while(iReturn == 0||iReturn==1405) { 
    	 psData = (struct stData *)pasLHashLookA(pHash,&lGroupid);    	
       if(psData){       
       	psData->pid=lPid;
       	strcpy(psData->caGroupname,caGroupname);
       	strcpy(psData->caCode,caCode);
       }
    lGroupid=0;
    lPid=0;
    memset(caGroupname,0,sizeof(caGroupname));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lPid,
                                   UT_TYPE_LONG,4,&lGroupid,
                                   UT_TYPE_STRING,31,caGroupname);
    }
    pasDbCloseCursor(psCur);
    
    
    pAreastatdata=(struct stData*)malloc((1000)*sizeof(struct stData));
    if(pAreastatdata==NULL){
  	    utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;
    }
    memset(pAreastatdata,0,(1000)*sizeof(struct stData));
    lSum=0;

    psData = (struct stData *)pasLHashFirst(pHash,&sHashInfo);

     while(psData && lSum < 1000){
             psData2 = (struct stData *)pasLHashLook(pHash,&psData->pid);
      	 	  //找不到，代表是根
            if(psData2==NULL){
            	pAreastatdata[lSum].pid=psData->pid;
            	pAreastatdata[lSum].groupid=psData->groupid;
            	strcpy(pAreastatdata[lSum].caGroupname,psData->caGroupname);
            	strcpy(pAreastatdata[lSum].caCode,psData->caCode);
            	//分配子节点
            	pAreastatdata[lSum].pArea=(struct stData*)malloc((100)*sizeof(struct stData));
            	memset(pAreastatdata[lSum].pArea,0,100*sizeof(struct stData));
            	//获取下个节点的数据
            	 psData3 = (struct stData *)pasLHashFirst(pHash,&sHashInfo2);
            	 lSum2=0;
            	 while(psData3&&lSum2<100){
            	 	 if(pAreastatdata[lSum].groupid==psData3->pid){
            	 	 	 pAreastatdata[lSum].pArea[lSum2].pid=psData3->pid;
            	 	 	 pAreastatdata[lSum].pArea[lSum2].groupid=psData3->groupid;
            	 	 	 strcpy(pAreastatdata[lSum].pArea[lSum2].caCode,psData3->caCode);
            	 	 	 strcpy(pAreastatdata[lSum].pArea[lSum2].caGroupname,psData3->caGroupname);
            	 	 	 pAreastatdata[lSum].lSum++;
            	 	 	 lSum2++;
            	 	 }
            	 	 	psData3 = (struct stData *)pasLHashNext(&sHashInfo2);
            	 }
            	
            	lSum++;
            }
     
     	psData = (struct stData *)pasLHashNext(&sHashInfo);
     }
    iNum=0;
     psDbHead = utPltInitDb();
    
     for(i=0;i<lSum;i++){
     	if(iNum>0){
     		   		utPltPutLoopVar(psDbHead,"dhf",iNum+1,",");
   	  }
   	  utPltPutLoopVar(psDbHead,"area",iNum+1,pAreastatdata[i].caGroupname);
   	  utPltPutLoopVarF(psDbHead,"groupid",iNum+1,"%d",pAreastatdata[i].groupid);
   	  utPltPutLoopVar(psDbHead,"code",iNum+1,pAreastatdata[i].caCode);
   	  if(pAreastatdata[i].lSum>0){
   	  	utPltPutLoopVar(psDbHead,"iconCls",iNum+1,"task-folder");
   	  }
   	  else{
   	  	utPltPutLoopVar(psDbHead,"iconCls",iNum+1,"task");
   	  }
   	    //输出第二层节点
   	    for(m=0;m<pAreastatdata[i].lSum;m++){
   	     if(m>0){
    	 	 	 utPltPutLoopVar2(psDbHead,"dhs",iNum+1,m+1,",");
    	 	}      	   
    	 	  utPltPutLoopVarF2(psDbHead,"groupid",iNum+1,m+1,"%d",pAreastatdata[i].pArea[m].groupid);
    	 	  utPltPutLoopVar2(psDbHead,"area",iNum+1,m+1,pAreastatdata[i].pArea[m].caGroupname);
    	 	  utPltPutLoopVar2(psDbHead,"code",iNum+1,m+1,pAreastatdata[i].pArea[m].caCode);
    	 	  //输出第三层节点
    	 	  n=0;
    	 	   psData = (struct stData *)pasLHashFirst(pHash,&sHashInfo);
      
      	   while(psData && n < 100){ 
      	   	if(pAreastatdata[i].pArea[m].groupid==psData->pid){
      	   	if(n>0){
      	 	 		utPltPutLoopVar3(psDbHead,"dht",iNum+1,m+1,n+1,",");
      	 	 	}
      	 	 	utPltPutLoopVar3(psDbHead,"area",iNum+1,m+1,n+1,psData->caGroupname);
      	 	 	utPltPutLoopVarF3(psDbHead,"groupid",iNum+1,m+1,n+1,"%d",psData->groupid);
      	 	 	utPltPutLoopVar3(psDbHead,"code",iNum+1,m+1,n+1,psData->caCode);
      	   	n++;
      	   }
      	   	psData = (struct stData *)pasLHashNext(&sHashInfo);
      	   	
      	  }
    	 	  
    	 	  
   	    }
   	  
        iNum++;
      }

        if(pHash)  free(pHash);
	        for(i=0;i<lSum;i++){ 
	        		free(pAreastatdata[lSum].pArea);
	        }
          free(pAreastatdata);

    
    utPltPutVar(psDbHead,"rootname",utComGetVar_sd(psShmHead,"rootname","全部单位"));
    	
//utPltShowDb(psDbHead);
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_group_select_tree.htm");
   

    return 0;
}



//显示关键字组合框选择
int ncsWebKeywordComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
   	char caQuery[256],caTemp[2024];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	long lId;
   	char *p,*pVar;
   	char caGroups[1024];

   	strcpy(caGroups,getDsGroupid());
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
    
    	iReturn = utMsgGetSomeNVar(psMsgHead,1,
					"pid",      UT_TYPE_STRING, 10, caPid);
					
					
	//				printf("caQuery=%s\n",pasCvtGBK(2,caQuery,caTemp,255));
    
    
    
  //  iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere,"\0");
    
    sprintf(caTemp,"select name,id,pid from ncskeyindex where 1=1 ");
    if(strlen(caPid)>0){
    	sprintf(caTemp+strlen(caTemp)," and id=%s ",caPid);
    }
    else{
    	sprintf(caTemp+strlen(caTemp)," and id!=400 and pid!=400 ");
    }
  	sprintf(caTemp+strlen(caTemp)," order by pid limit %lu,%lu",0,1000);
  	psCur = pasDbOpenSql(caTemp,0);
printf("caTemp=%s\n",caTemp);  	
    if(psCur == NULL) {
    	  
        return 0;
    }
    psDbHead = utPltInitDb();
    
     
    
    
 
    memset(caGroupname,0,sizeof(caGroupname));
    lGroupid=0;
    lPid=0;
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caGroupname,
                                   UT_TYPE_LONG,4,&lGroupid,
                                   UT_TYPE_LONG,4,&lPid);

    iNum = 0;
    while(iReturn == 0) {
        if(iNum>0){
    		    utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
    	  }
    	
        iNum++;
        
        utPltPutLoopVar(psDbHead,"groupname", iNum,caGroupname);
        utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%ld",lGroupid);
      
		    memset(caGroupname,0,sizeof(caGroupname));
		    lGroupid=0;
		    lLevel=0;
		    lPid=0;
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caGroupname,
		                                   UT_TYPE_LONG,4,&lGroupid,
		                                   UT_TYPE_LONG,4,&lPid);
    }
    pasDbCloseCursor(psCur);   
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_keyword_class_list.htm");
      
    return 0;
}





//虚拟身份类别树——供comp选择使用

int ncsTreeNettype_comp_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caName[36];
    unsigned long lNtype;
    unsigned long lGid;
    char caTemp[1024];
    utPltDbHead *psDbHead;
   	int iReturn,i;
    long iNum_im,iNum_game,iNum_mail,iNum_lt,iNum_wb,iNum_zp,iNum_jy,iNum_ly,iNum_gw;
	utMsgPrintMsg(psMsgHead);
//utPltShowDb(psDbHead);
psDbHead = utPltInitDb();

        sprintf(caTemp,"select ntype,name,gid from ncsnettype  ");
        psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0561数据统计出错"),ncsLang("0171查询数据出错"));
        return 0;
    }
   
   	                              
   	iReturn=0;
   	iNum_im=0;
   	iNum_game=0;
   	iNum_mail=0;
   	iNum_lt=0;
   	iNum_wb=0;
   	iNum_zp=0;
   	iNum_jy=0;
   	iNum_ly=0;
   	iNum_gw=0;
   	lNtype=0;
   	lGid=0;
   	memset(caName,0,sizeof(caName));
   	iReturn = pasDbFetchInto(psCur,    
                                       UT_TYPE_ULONG,4,&lNtype,
                                       UT_TYPE_STRING,31,caName,
                                       UT_TYPE_LONG,4,&lGid);
               
   	while((iReturn == 0 ||iReturn==1405 )) {

   		if(((lNtype>=1001&&lNtype<=1999)||lGid==1)&&iNum_im<100){
   			if(iNum_im>0){
   			  utPltPutLoopVar(psDbHead,"dh_im",iNum_im+1,",");
   		  }
   			
   			utPltPutLoopVarF(psDbHead,"groupid_im",iNum_im+1,"%lu",lNtype);
   		  utPltPutLoopVar(psDbHead,"groupname_im",iNum_im+1,caName);
   		  iNum_im++;
   		}
      else if((lNtype>=2001&&lNtype<=2999)||lGid==2){      	
      	if(iNum_game>0){
   			  utPltPutLoopVar(psDbHead,"dh_game",iNum_game+1,",");
   		  }   			
   			utPltPutLoopVarF(psDbHead,"groupid_game",iNum_game+1,"%lu",lNtype);
   		  utPltPutLoopVar(psDbHead,"groupname_game",iNum_game+1,caName);
   		  iNum_game++;
      }
   		 else if((lNtype>=13000&&lNtype<=13999)||lGid==13){      	
      	if(iNum_mail>0){
   			  utPltPutLoopVar(psDbHead,"dh_mail",iNum_mail+1,",");
   		  }   			
   			utPltPutLoopVarF(psDbHead,"groupid_mail",iNum_mail+1,"%lu",lNtype);
   		  utPltPutLoopVar(psDbHead,"groupname_mail",iNum_mail+1,caName);
   		  iNum_mail++;
      }
   		 else if(((lNtype>=20000&&lNtype<=29999&&lGid==0)||lGid==20)&&iNum_lt<100){      	
      	if(iNum_lt>0){
   			  utPltPutLoopVar(psDbHead,"dh_lt",iNum_lt+1,",");
   		  }   			
   			utPltPutLoopVarF(psDbHead,"groupid_lt",iNum_lt+1,"%lu",lNtype);
   		  utPltPutLoopVar(psDbHead,"groupname_lt",iNum_lt+1,caName);
   		  iNum_lt++;
      }
   		 else if(lGid==21){      	
      	if(iNum_wb>0){
   			  utPltPutLoopVar(psDbHead,"dh_wb",iNum_wb+1,",");
   		  }   			
   			utPltPutLoopVarF(psDbHead,"groupid_wb",iNum_wb+1,"%lu",lNtype);
   		  utPltPutLoopVar(psDbHead,"groupname_wb",iNum_wb+1,caName);
   		  iNum_wb++;
      }
   		else if(lGid==22){      	
      	if(iNum_jy>0){
   			  utPltPutLoopVar(psDbHead,"dh_jy",iNum_jy+1,",");
   		  }   			
   			utPltPutLoopVarF(psDbHead,"groupid_jy",iNum_jy+1,"%lu",lNtype);
   		  utPltPutLoopVar(psDbHead,"groupname_jy",iNum_jy+1,caName);
   		  iNum_jy++;
      }
   		else if(lGid==23){      	
      	if(iNum_zp>0){
   			  utPltPutLoopVar(psDbHead,"dh_zp",iNum_zp+1,",");
   		  }   			
   			utPltPutLoopVarF(psDbHead,"groupid_zp",iNum_zp+1,"%lu",lNtype);
   		  utPltPutLoopVar(psDbHead,"groupname_zp",iNum_zp+1,caName);
   		  iNum_zp++;
      }
      else if(lGid==24){      	
      	if(iNum_gw>0){
   			  utPltPutLoopVar(psDbHead,"dh_gw",iNum_gw+1,",");
   		  }   			
   			utPltPutLoopVarF(psDbHead,"groupid_gw",iNum_gw+1,"%lu",lNtype);
   		  utPltPutLoopVar(psDbHead,"groupname_gw",iNum_gw+1,caName);
   		  iNum_gw++;
      }
      else if(lGid==25){      	
      	if(iNum_ly>0){
   			  utPltPutLoopVar(psDbHead,"dh_ly",iNum_ly+1,",");
   		  }   			
   			utPltPutLoopVarF(psDbHead,"groupid_ly",iNum_ly+1,"%lu",lNtype);
   		  utPltPutLoopVar(psDbHead,"groupname_ly",iNum_ly+1,caName);
   		  iNum_ly++;
      }
   		 lNtype=0;
		   	lGid=0;
		   	memset(caName,0,sizeof(caName));
		   	iReturn = pasDbFetchInto(psCur,    
		                                       UT_TYPE_ULONG,4,&lNtype,
		                                       UT_TYPE_STRING,31,caName,
		                                       UT_TYPE_LONG,4,&lGid);
   	}
     pasDbCloseCursor(psCur);    
        
        

    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_netid_select_tree.htm");
   

    return 0;
}




//单位类别树AJAX通讯
int ncsTreeUserLb_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
   	char caGroupname[72],caGroupid[16];
   	char caTemp[4024];
   	char caName[32];
   	char caWhere[256];

   	char caCurPg[16],caTotRec[16];
   	char caPid[16];
   	unsigned long lLevel,lPid;
   	int  iNum;
  	char caFilter[256];
  	char caNo[16];
   	unsigned long lGroupid,lGroupid0,lCurPg,lTotRec,lRowNum,lStartRec;
    utPltDbHead *psDbHead;
   	int iReturn,i;
   	char caVar[256],caFname[64],caBfname[64];
   	char caValue[256];
   	char caVar1[32];
   	char caOpt[256];
   	char caGroups[1024];
   	char caFacs[1024];
   	long lId;
   	char *p,*pVar;

   	char caCname[72];
    char caCode[16];
    long lCount;
    
    char caCname1[72];
    char caSql[2048];
#ifdef LDEBUG1 
    utMsgPrintMsg(psMsgHead);
#endif  	
    utMsgPrintMsg(psMsgHead);
      strcpy(caGroups,getDsGroupid());
      strcpy(caFacs,getDsFacGroupid());
    memset(caCname,0,sizeof(caCname));
   	iReturn = utMsgGetSomeNVar(psMsgHead,3,
					"node",        UT_TYPE_STRING,10,caPid,
					"cname",       UT_TYPE_STRING,30,caCname1,
					"plate",       UT_TYPE_STRING,100,caPlate);
				

    lRowNum = utComGetVar_ld(psShmHead,"GroupTreeRow",300);
  
    
 
    iNum = 0;
    
     psDbHead = utPltInitDb();
	//	    lStartRec = (lCurPg - 1) * lRowNum; 
	if(strlen(caPid)==0){
		     sprintf(caTemp,"select name,code from ncsuser_lb where 1=1 ");
		    sprintf(caTemp+strlen(caTemp)," order by code limit %lu,%lu ",0,lRowNum);
		    printf("caTemp=%s\n",caTemp);
		    psCur=pasDbOpenSql(caTemp,0);
		  
		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   
		    
		     
	      memset(caName,0,sizeof(caName));
	      memset(caCode,0,sizeof(caCode));
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
		                                   UT_TYPE_STRING,10,caCode);
		                                   
		
		 
		    while(iReturn == 0) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  
		         utPltPutLoopVar(psDbHead,"groupname", iNum,caName);
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVar(psDbHead,"groupid",   iNum,caCode);

		        utPltPutLoopVar(psDbHead,"leaf", iNum,"false");
		        utPltPutLoopVar(psDbHead,"cls", iNum,"folder");
		        utPltPutLoopVar(psDbHead,"dtype",iNum,"1");
		     	
		        utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
		
		        utPltPutLoopVarF(psDbHead,"pid",iNum,"%d",lPid);
		
			      memset(caName,0,sizeof(caName));
			      memset(caCode,0,sizeof(caCode));
				    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caName,
				                                   UT_TYPE_STRING,10,caCode);
		    }
		    pasDbCloseCursor(psCur);
	
		    utPltPutVar(psDbHead,"rootname",utComGetVar_sd(psShmHead,"rootname","全部单位"));
    	
    }
    else if(strlen(caPid)>0){
    	      sprintf(caTemp,"select dispname,userid,gtype from ncsuser where 1=1 ");
		    		sprintf(caTemp+strlen(caTemp)," and gtype=%d ",atol(caPid));  
            sprintf(caTemp+strlen(caTemp)," order by dispname limit %lu,%lu",0,lRowNum);
      printf("caTemp=%s\n",caTemp);
       			psCur = pasDbOpenSql(caTemp,0);
    	 if(psCur!=NULL){
    	    memset(caGroupname,0,sizeof(caGroupname));
			    lGroupid=0;
			    lPid=0;
			    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
			                                   UT_TYPE_LONG,4,&lGroupid,
			                                   UT_TYPE_LONG,4,&lPid);
			
			    while(iReturn == 0) {
			        
			         if(iNum>0){
			         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
			         }
			         iNum++;
			         utPltSetCvtHtml(1);  
			         utPltPutLoopVar(psDbHead,"groupname", iNum,utStrGetCharHan(caGroupname,48));
			         utPltSetCvtHtml(0);  
			         utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%ld",lGroupid);

			          	utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
			         		utPltPutLoopVar(psDbHead,"cls", iNum,"file");
			          	
			            utPltPutLoopVar(psDbHead,"dtype",iNum,"2");
			 
			        
			
			        utPltPutLoopVarF(psDbHead,"iNum",iNum,"%d",iNum-1);
			
			        utPltPutLoopVarF(psDbHead,"pid",iNum,"%d",lPid);
			
					    memset(caGroupname,0,sizeof(caGroupname));
					    lGroupid=0;
					    lLevel=0;
					    lPid=0;
					    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,68,caGroupname,
					                                   UT_TYPE_LONG,4,&lGroupid,
					                                   UT_TYPE_LONG,4,&lPid);
			    }
			    pasDbCloseCursor(psCur);
    	  }
    	}
    	
// utPltShowDb(psDbHead);

    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_userlb_tree.htm");
    
    return 0;
}




/*按类别统计在线单位在线率*/
int ncs_lbonlinerate(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{	
	 struct dwlxdata_s{
    char code[12];
    char  name[64];
 		int  ondid_count;   //在线单位数
 		int  ycdid_count;   //异常单位数
 		int  wh_count;      //维护单位数
 		int  ty_count;      //停业单位数
 		int  did_count;     //单位数
 		int  onuid_count;   //在线用户数
 		int  endnum;      //终端用户数
 	};
    ncsOnline *psOnline;
    ncsClient *psUser;
    char *pHash;
    struct dwlxdata_s *psData;
    unsigned char *pHash_wh;
     struct corpwh_s{
 	 	 unsigned long lUserid;  //单位ID
 		 unsigned long flag;     //4-停业 5-维护
 	 };
   struct corpwh_s *psData_wh;
    
    
  	char caTemp[4096];
  	char caGroupname[32];
  	pasDbCursor *psCur;
  	int lCount_area,lCount_corp,lCount_zx,lCount_lx,lCount_yc,lCount_user,lCount_useron;
    
    utPltDbHead *psDbHead;
   	int iReturn,i,lStart,lGroupid,lCount_g,index,iNum,lCurPg,lTotPg;
    char caTotPg[16],caCurPg[16],caExport[12];
    int lSum_did,lSum_ondid,lSum_ycdid,lSum_lxdid,lSum_endnum,lSum_onuid,lSum_tycount,lSum_whcount;
    char caOpt[256];
    unsigned long lId,lPid;
    char caUsername2[32];
    FILE *fp;
    char pFile[256],caBuf[128];
    char caAll[1024];
    char caSid[16];
  
    pasLHashInfo sHashInfo;
    long lSum2,m,n,iCount=0,iCount1=0;
    long lUserid;
    char caCode[16],caName[64];
    char caGroups[1024];
      char caFacs[1024];
      char caWireflag[16];
	 strcpy(caFacs,getDsFacDid());
    strcpy(caGroups,getDsGroupid());
    memset(caAll,0,sizeof(caAll));
   
 //将维护和停用单位放到Hash中
    pHash_wh = (unsigned char *)pasLHashInit(10000,10000,sizeof(struct corpwh_s),0,4);    
    if(pHash_wh == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
  sprintf(caTemp,"select userid,servicestate from ncsuser  where 1=1");
       if(strlen(caWireflag)>0){
    	if(strcmp(caWireflag,"2")==0){
    	   sprintf(caTemp+strlen(caTemp)," and (jointype='08' or jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
      	sprintf(caTemp+strlen(caTemp)," and jointype!='08' ");
      }
    
    }
  
  sprintf(caTemp+strlen(caTemp)," limit 0,9999 ");
  
  
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
       	 	  else if(strcmp(caGroupname,"2")>0){
       	 	  	psData_wh->flag=4;
       	 	  }
       	 	  else{
       	 	  	psData_wh->flag=0;
       	 	  }
       	 
       	 }
    	 	
			  lGroupid=0;
			  memset(caGroupname,0,sizeof(caGroupname));
			  iReturn = pasDbFetchInto(psCur,    	                             
			      	                             UT_TYPE_LONG,4,&lGroupid,
			      	                             UT_TYPE_STRING,1,caGroupname); 	                              
    }    
    pasDbCloseCursor(psCur);  
    
    
    
   
    lSum_did=0;
    lSum_ondid=0;
    lSum_ycdid=0;
    lSum_lxdid=0;
    lSum_endnum=0;
    lSum_onuid=0;
    lSum_whcount=0;
    lSum_tycount=0;
    lCount_corp=0;
    lCount_zx=0;
    lCount_lx=0;
    lCount_yc=0;
    lCount_user=0;
    lCount_useron=0;
    
    
  
    
    	iReturn = utMsgGetSomeNVar(psMsgHead,4,
	    "TotPg",      UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      UT_TYPE_STRING, 12, caCurPg,
   	  "exp",     UT_TYPE_STRING,10,caExport,
   	  "wireflag",UT_TYPE_STRING,10,caWireflag);
 
    lCurPg = atol(caCurPg);
    if(lCurPg <= 0) lCurPg = 1;
    lStart = (lCurPg - 1) * 28;  
   
    pHash = (unsigned char *)pasLHashInit(100,100,sizeof(struct dwlxdata_s),0,12);    
    if(pHash == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
   
   
   

  
  sprintf(caTemp,"select code,name from ncsuser_lb ");

  printf("caTemp=%s\n",caTemp);
  psCur = pasDbOpenSql(caTemp,0);
  if(psCur == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
   }
  memset(caCode,0,sizeof(caCode));
  memset(caName,0,sizeof(caName));
  iReturn = pasDbFetchInto(psCur,    	                             
      	                             UT_TYPE_STRING,11,caCode,
      	                             UT_TYPE_STRING,63,caName);
      	         
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
    		 psData = (struct dwlxdata_s *)pasLHashLookA(pHash,caCode);  
       	 if(psData){
       	 
       	 	psData->did_count=0;
       	 	strcpy(psData->name,caName);
          psData->ondid_count=0;
          psData->ycdid_count=0;
          psData->ondid_count=0;
          psData->onuid_count=0;
          psData->endnum=0;
          psData->wh_count=0;
          psData->ty_count=0;
       	 }
    	 	
			  memset(caCode,0,sizeof(caCode));
			  memset(caName,0,sizeof(caName));
			  iReturn = pasDbFetchInto(psCur,    	                             
			      	                             UT_TYPE_STRING,11,caCode,
			      	                             UT_TYPE_STRING,63,caName);	                              
    }    
    pasDbCloseCursor(psCur);  
    
    
    //添加未知类别
         memset(caCode,0,sizeof(caCode));
         strcpy(caCode,"未知类别");
         psData = (struct dwlxdata_s *)pasLHashLookA(pHash,caCode);  
       	 if(psData){  
       	 	strcpy(psData->name,"未知类别");
          psData->ondid_count=0;
          psData->ycdid_count=0;
          psData->ondid_count=0;
          psData->onuid_count=0;
          psData->endnum=0;
          psData->wh_count=0;
          psData->ty_count=0;
       	 }
    
    
    
    
    
    
    
    
    
    //统计各类别单位数
    sprintf(caTemp,"select count(*),gtype from ncsuser where 1=1 ");
 
    if(strlen(caGroups)>0){
    	sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caGroups);
    }
    if(strlen(caFacs)>0){
				sprintf(caTemp+strlen(caTemp)," and userid in (%s) ",caFacs);
		}
    
    if(strlen(caWireflag)>0){
    	if(strcmp(caWireflag,"2")==0){
    	   sprintf(caTemp+strlen(caTemp)," and (jointype='08' or jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
      	sprintf(caTemp+strlen(caTemp)," and jointype!='08' ");
      }
    
    }
    
    sprintf(caTemp+strlen(caTemp)," group by gtype ");

      printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
    	if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
    lGroupid=0;
  	lCount_g=0;
  	i=0;
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_g,
      	                             UT_TYPE_LONG,4,&lGroupid);
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
    	memset(caCode,0,sizeof(caCode));
    	sprintf(caCode,"%d",lGroupid);
   // 	printf("code=%s,count=%d\n",caCode,lCount_g);
    	 psData = (struct dwlxdata_s *)pasLHashLook(pHash,caCode);   
       if(psData){ 
       	psData->did_count=lCount_g;
       	} 
       	else{
         memset(caCode,0,sizeof(caCode));
         strcpy(caCode,"未知类别");
         psData = (struct dwlxdata_s *)pasLHashLook(pHash,caCode);  
		       if(psData){ 
		       	psData->did_count=psData->did_count+lCount_g;
		       	}
       	}  
			  lGroupid=0;
			  lCount_g=0;
			  iReturn = pasDbFetchInto(psCur,
			      	                             UT_TYPE_LONG,4,&lCount_g,
			      	                             UT_TYPE_LONG,4,&lGroupid);  	                              
    }    
    pasDbCloseCursor(psCur); 
    
    
    
    
 //各类别用户数   
   memset(caTemp,0,sizeof(caTemp));
       strcpy(caTemp,"select sum(lcount),b.gtype from ncsclient_enduserstat a,ncsuser b where a.userid=b.userid ");

   if(strlen(caGroups)>0){
 //   	printf("caGroups3=%s\n",caGroups);
     	sprintf(caTemp+strlen(caTemp)," and a.groupid in (%s) ",caGroups);
    	}
       if(strlen(caFacs)>0){
				sprintf(caTemp+strlen(caTemp)," and a.userid in (%s) ",caFacs);
		}
		 if(strlen(caWireflag)>0){
    	if(strcmp(caWireflag,"2")==0){
    	   sprintf(caTemp+strlen(caTemp)," and (a.jointype='08' or a.jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
      	sprintf(caTemp+strlen(caTemp)," and a.jointype!='08' ");
      }
    
    }
		
		
		
    sprintf(caTemp+strlen(caTemp)," group by b.gtype "); 
    
    
      printf("caTemp2=%s\n",caTemp); 
    psCur = pasDbOpenSql(caTemp,0);
   
    if(psCur){  
    lGroupid=0;
  	lCount_g=0;
  	i=0;
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_g,
      	                             UT_TYPE_LONG,4,&lGroupid);
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
    	 memset(caCode,0,sizeof(caCode));
    	 sprintf(caCode,"%d",lGroupid);
    	 psData = (struct dwlxdata_s *)pasLHashLook(pHash,caCode);   
       if(psData){ 
       	psData->endnum=lCount_g;
       	}  
       	else{
         memset(caCode,0,sizeof(caCode));
         strcpy(caCode,"未知类别");
         psData = (struct dwlxdata_s *)pasLHashLook(pHash,caCode);  
		       if(psData){ 
		       	psData->endnum=psData->endnum+lCount_g;
		       	} 
       	} 
			  lGroupid=0;
			  lCount_g=0;
			  iReturn = pasDbFetchInto(psCur,
			      	                             UT_TYPE_LONG,4,&lCount_g,
			      	                             UT_TYPE_LONG,4,&lGroupid);  	                              
    }    
    pasDbCloseCursor(psCur); 
   }
 //在线单位数
    psUser = (ncsClient *)utShmArray(psShmHead,NCS_LNK_CLIENT);
    if(psUser == NULL) {
    	if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","在线单位","memory Error ");
        return 0;
    }
    long iMaxRecord;
   iMaxRecord = utShmArrayRecord(psShmHead,NCS_LNK_CLIENT);

   for(i=0;i<iMaxRecord;i++){
   
		   	if(strlen(caGroups)>0&&isInGroup(caGroups,psUser[i].groupid)==0){
		   		continue;
		   	}
		   	if(strlen(caFacs)>0&&isInGroup(caFacs,psUser[i].userid)==0){
		   		continue;
		   	}
   	   	psData_wh=(struct corpwh_s *)pasLHashLook(pHash_wh,&psUser[i].userid); 
		    if(!psData_wh){
		       continue;
		    }
   	
   	      psOnline =  (ncsOnline *)ncsUtlGetOnlineById(psShmHead,psUser[i].userid);
   	      memset(caCode,0,sizeof(caCode));
   	      sprintf(caCode,"%d",psUser[i].usertype);
   	      psData = (struct dwlxdata_s *)pasLHashLook(pHash,caCode); 
   	      if(psData==NULL){
            memset(caCode,0,sizeof(caCode));
            strcpy(caCode,"未知类别");
            psData = (struct dwlxdata_s *)pasLHashLook(pHash,caCode);   
   	      }  
		      if(psData){ 
		      	if(psUser[i].status==1||psUser[i].status==3){
		      		if(psOnline){
		       			psData->ondid_count++;
		       			if(psOnline->lUser<1000000){
				        	psData->onuid_count=psData->onuid_count+psOnline->lUser;
				        }
		       	  }
		       	  else{
		       	  	
		       	  			psData_wh=(struct corpwh_s *)pasLHashLook(pHash_wh,&psUser[i].userid); 
					        	if(psData_wh){
					        		
					        		if(psData_wh->flag==5){
					        			psData->wh_count++;
					        		}
					        		else if(psData_wh->flag>=2&&psData_wh->flag<=4){
					        			psData->ty_count++;
					        		}
					        }
		       	  	
		       	  }
		        }
		        else{
//查询一下这个单位是否处于停业或维护状态
		        	psData_wh=(struct corpwh_s *)pasLHashLook(pHash_wh,&psUser[i].userid); 
		        	if(psData_wh){
		        		
		        		if(psData_wh->flag==5){
		        			psData->wh_count++;
		        		}
					      else if(psData_wh->flag>=2&&psData_wh->flag<=4){
					        			psData->ty_count++;
					      }
		        }
		       }
        }
      }

  
      if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
    	char caRate[16];
        sprintf(caFilename,"ncsstat%d.csv",utStrGetId());
        sprintf(caPath,"%s/html","/home/ncmysql/ncs");
        sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
   
            if(pHash) free(pHash);
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0546数据导出出错"),ncsLang("0546数据导出出错"));
            return 0;
        }
     
          fprintf(fp,"按类别统计在线率\n");
		      ncUtlPrintCsv(fp,9,UT_TYPE_STRING,"单位类别",
		                       UT_TYPE_STRING,"单位总数",
		                       UT_TYPE_STRING,"在线单位",
		                       UT_TYPE_STRING,"异常在线单位",
		                       UT_TYPE_STRING,"维护单位数",
		                       UT_TYPE_STRING,"停业单位数",
		                       UT_TYPE_STRING,"离线单位",
//		                       UT_TYPE_STRING,"用户总数",
		                       UT_TYPE_STRING,"在线用户",
		                       UT_TYPE_STRING,"在线率");
		   psData = (struct dwlxdata_s *)pasLHashFirst(pHash,&sHashInfo);
			while(psData){  
				 	if(psData->did_count-psData->wh_count-psData->ty_count>0){
			 	    		sprintf(caRate,"%d%c",100*(psData->ondid_count+psData->ycdid_count)/(psData->did_count-psData->wh_count-psData->ty_count),'%');
			 	    	}
			 	    	else{
			 	    		strcpy(caRate,"0");
			 	    	}        
				         
				      			 	 ncUtlPrintCsv(fp,9,UT_TYPE_STRING,psData->name,
                       UT_TYPE_LONG,psData->did_count,
                       UT_TYPE_LONG,psData->ondid_count,
                       UT_TYPE_LONG,psData->ycdid_count,
                       UT_TYPE_LONG,psData->wh_count,
                       UT_TYPE_LONG,psData->ty_count,
                       UT_TYPE_LONG,psData->did_count-psData->ondid_count-psData->ycdid_count-psData->wh_count-psData->ty_count,
                       UT_TYPE_LONG,psData->onuid_count,
                       UT_TYPE_STRING,caRate);   
				         
				         
				              
   			psData = (struct dwlxdata_s *)pasLHashNext(&sHashInfo);   
 			}        

			  fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        if(pHash) free(pHash);
	     
        free(pHash_wh);
        return 0;
			
		}
    
    iNum=0;
   psDbHead = utPltInitDb();
      psData = (struct dwlxdata_s *)pasLHashFirst(pHash,&sHashInfo);
			while(psData){ 
				if(iNum>0){
   		    utPltPutLoopVar(psDbHead,"dhf",iNum+1,",");
   	    }
//	printf("name=%s,didcount=%d,oncount=%d\n",	psData->name,psData->did_count,psData->ondid_count);		
			utPltPutLoopVar(psDbHead,"area",iNum+1,psData->name);
    	utPltPutLoopVarF(psDbHead,"ondid_count",iNum+1,"%lu",psData->ondid_count);
    	utPltPutLoopVarF(psDbHead,"did_count",iNum+1,"%lu",psData->did_count);
    	utPltPutLoopVarF(psDbHead,"endnum",iNum+1,"%lu",psData->endnum);
    	utPltPutLoopVarF(psDbHead,"ycdid_count",iNum+1,"%lu",psData->ycdid_count);
    	utPltPutLoopVarF(psDbHead,"wh_count",iNum+1,"%lu",psData->wh_count);
    	utPltPutLoopVarF(psDbHead,"ty_count",iNum+1,"%lu",psData->ty_count);
    	utPltPutLoopVar(psDbHead,"id",iNum+1,psData->code);
    	if(psData->did_count-psData->ondid_count-psData->ycdid_count-psData->wh_count-psData->ty_count>0){
    		utPltPutLoopVarF(psDbHead,"lxdid_count",iNum+1,"%lu",psData->did_count-psData->ondid_count-psData->ycdid_count-psData->wh_count-psData->ty_count);
    		lSum_lxdid=lSum_lxdid+psData->did_count-psData->ondid_count-psData->ycdid_count-psData->wh_count-psData->ty_count;
    	}
    	else{
    		utPltPutLoopVarF(psDbHead,"lxdid_count",iNum+1,"%lu",0);
    	}
    	utPltPutLoopVarF(psDbHead,"onuid_count",iNum+1,"%lu",psData->onuid_count);


    	
    	if(psData->did_count-psData->wh_count-psData->ty_count>0){
    		utPltPutLoopVarF(psDbHead,"rate",iNum+1,"%d%c",100*(psData->ondid_count+psData->ycdid_count)/(psData->did_count-psData->wh_count-psData->ty_count),'%');
    	}
    	 else
    	  utPltPutLoopVar(psDbHead,"rate",iNum+1,"100");
				
		lSum_did=lSum_did+psData->did_count;
		lSum_ondid=	lSum_ondid+psData->ondid_count;	
		lSum_ycdid=lSum_ycdid+psData->ycdid_count;
		lSum_endnum=lSum_endnum+psData->endnum;
		lSum_onuid=lSum_onuid+psData->onuid_count;
		lSum_whcount=lSum_whcount+psData->wh_count;
		lSum_tycount=lSum_tycount+psData->ty_count;
		
				iNum++;
				
   			psData = (struct dwlxdata_s *)pasLHashNext(&sHashInfo);   
 			}
   
   
   
  
		utPltPutVarF(psDbHead,"sum_did","%d",lSum_did);
		utPltPutVarF(psDbHead,"sum_ondid","%d",lSum_ondid);
		utPltPutVarF(psDbHead,"sum_ycdid","%d",lSum_ycdid);
		utPltPutVarF(psDbHead,"sum_lxdid","%d",lSum_lxdid);
		utPltPutVarF(psDbHead,"sum_endnum","%d",lSum_endnum);
		utPltPutVarF(psDbHead,"sum_onuid","%d",lSum_onuid);
		utPltPutVarF(psDbHead,"wh_count","%d",lSum_whcount);
		utPltPutVarF(psDbHead,"ty_count","%d",lSum_tycount);

      	 	 	if(lSum_did-lSum_whcount-lSum_tycount>0){
      	 	 		utPltPutVarF(psDbHead,"rate","%d%c",100*(lSum_ondid)/(lSum_did-lSum_whcount-lSum_tycount),'%');
      	 	 	}
      	 	 	else{
      	 	 		utPltPutVar(psDbHead,"rate","0");
      	 	 	}

        if(pHash) free(pHash);
	      
        free(pHash_wh);
//printf("five\n");
//utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_first_onlinestat.htm");
    return 0;
}


int ncsNotifySyscorp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	     char caSql[2048];
	     memset(caSql,0,sizeof(caSql));
	     
	     utMsgGetSomeNVar(psMsgHead,1, "passql",UT_TYPE_STRING,2000,caSql);
	     	pasLogs(PAS_SRCFILE,1000,"receive,SQL=%s ",caSql); 
       if(strlen(caSql)>0){
       	pasDbExecSql(caSql,0);                              


       
             pasTcpResponse(iFd,psMsgHead,
                    NULL,                /* 密钥，暂时不用  */
                    1,
                    "status",UT_TYPE_LONG,0);
       }
     return 0;
	
}


int ncsSyscorp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	     char caSql[2048];
	      char caTemp[2024];
	      unsigned long lCount,lUserid;
        unsigned long lGroupid,lGtype,lModitime,lLastid;
        char caUsername[20],caDispname[64],caAddress[128],caTelphone[64],caContact[32],caHandphone[32];
        char caEmail[128],caCeoname[30],caPostcode[12],caServicestate[8],caJoincode[8],caEndnum[8];
        char caServernum[8],caJointype[8],caEmpnum[8],caArea[70],caAreaman[30],caAreatel[30],caDemo[256];
        char caIp[16],caVersion[16],caFcode[16];
	     
	     memset(caSql,0,sizeof(caSql));
	   //   utMsgPrintMsg(psMsgHead);
	     utMsgGetSomeNVar(psMsgHead,24, "groupid",UT_TYPE_ULONG,4,&lGroupid,
	                                    "username",UT_TYPE_STRING,15,caUsername,
	                                    "dispname",UT_TYPE_STRING,63,caDispname,
	                                    "address",UT_TYPE_STRING,254,caAddress,
	                                    "telphone",UT_TYPE_STRING,18,caTelphone,
	                                    "contact",UT_TYPE_STRING,30,caContact,
	                                    "handphone",UT_TYPE_STRING,31,caHandphone,
	                                    "email",UT_TYPE_STRING,127,caEmail,
	                                    "ceoname",UT_TYPE_STRING,29,caCeoname,
	                                    "postcode",UT_TYPE_STRING,6,caPostcode,
	                                    "servicestate",UT_TYPE_STRING,1,caServicestate,
	                                    "joincode",UT_TYPE_STRING,6,caJoincode,
	                                    "endnum",UT_TYPE_STRING,6,caEndnum,
	                                    "servernum",UT_TYPE_STRING,2,caServernum,
	                                    "jointype",UT_TYPE_STRING,2,caJointype,
	                                    "empnum",UT_TYPE_STRING,6,caEmpnum,
	                                    "area",UT_TYPE_STRING,69,caArea,
	                                    "areaman",UT_TYPE_STRING,29,caAreaman,
	                                    "areatel",UT_TYPE_STRING,29,caAreatel,
	                                    "demo",UT_TYPE_STRING,254,caDemo,
	                                    "ip",UT_TYPE_STRING,15,caIp,
	                                    "gtype",UT_TYPE_LONG,4,&lGtype,
	                                    "version",UT_TYPE_STRING,15,caVersion,
	                                    "fcode",UT_TYPE_STRING,8,caFcode);
	     	printf("fcode=%s,lgtype=%lu\n",caFcode,lGtype);
       if(strlen(caUsername)>0&&strlen(caDispname)>0){
       	sprintf(caSql,"select count(*) from ncsuser where username='%s' ",caUsername);
       	lCount=0;
       	pasDbOneRecord(caSql,0,UT_TYPE_LONG,4,&lCount);
       	if(lCount>0){
       		sprintf(caSql,"update ncsuser set dispname='%s',groupid=%lu,dispname='%s',address='%s',telphone='%s',contact='%s',handphone='%s',email='%s',\
       		   ceoname='%s',postcode='%s',servicestate='%s',joincode='%s',endnum='%s',servernum='%s',jointype='%s',empnum='%s',area='%s',areaman='%s',areatel='%s',demo='%s',\
       		   ip='%s',gtype=%lu,version='%s',fcode='%s' where username='%s' ",\
       		   caDispname,lGroupid,caDispname,caAddress,caTelphone,caContact,caHandphone,caEmail,\
       		   caCeoname,caPostcode,caServicestate,caJoincode,caEndnum,caServernum,caJointype,caEmpnum,caArea,caAreaman,caAreatel,caDemo,\
       		   caIp,lGtype,caVersion,caFcode,caUsername);
       		   	pasLogs(PAS_SRCFILE,1000,caSql);
       	}
       	else{
       		  lUserid=utMd5Code(caUsername,strlen(caUsername),"ffff");
				       if(lUserid>2000000000) lUserid=lUserid-2000000000;       
				       if(lUserid>2000000000) lUserid=lUserid-2000000000;
				       	
				    sprintf(caSql,"insert into ncsuser(userid,username,groupid,dispname,address,telphone,contact,handphone,email,ceoname,postcode,servicestate,joincode,endnum,servernum,jointype,empnum,area,areaman,areatel,demo,ip,gtype,version,fcode,addtime,moditime) \
				     values(%lu,'%s',%lu,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%lu,'%s','%s',%d,%d) ",
				     lUserid,caUsername,lGroupid,caDispname,caAddress,caTelphone,caContact,caHandphone,caEmail,caCeoname,caPostcode,caServicestate,caJoincode,caEndnum,caServernum,caJointype,caEmpnum,caArea,caAreaman,caAreatel,caDemo,caIp,lGtype,caVersion,caFcode,time(0),time(0));
				     pasLogs(PAS_SRCFILE,1000,caSql);
       	}
       	pasDbExecSql(caSql,0);                              

      }
       
             pasTcpResponse(iFd,psMsgHead,
                    NULL,                /* 密钥，暂时不用  */
                    1,
                    "status",UT_TYPE_LONG,0);
      
     return 0;
	
}


//显示单位信息
int ncsWebClientSearch_wif(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    char caWhere[512],caCond[64],caCond0[64];
    char caTotPg[16],caCurPg[16],caSort[16],caSortby[8];
    char caUsername[32],caGroupname[32],caMac[32],caIp[32],caDate[32];
    char caGroupid[16],caGroupid2[16];
    char caDispname[64],caAddress[128],caTel[64],caHandphone[32],caContact[32];
    char caTemp[2024],name[64];
    char caVar[16];
    long lSid;
    char *p;
    long lCount_t,js_flag;
    unsigned long lSumuser;
    unsigned long lCount_u;
    char caAll[1024];
    long index;
    ncsOnline  *psOnline;
    ncsClient * psClient;
    char caEmail[128];
    char *pSelect,*pBuf;
    unsigned long lUserid,lGroupid,lCurPg,lTotPg;
    int lCount;
    int i,iNum,lId;
    char *pSear;
    struct servicedata_s *pAreastatdata;     
	  pasLHashInfo sHashInfo; 
	  unsigned char *pHash;
	  char caDir[20],caDid[16];
  
    unsigned char *pHash_fac;
    struct fac_s {
    	char caCod[12];
    	char caName[64];
    };
    struct fac_s *psFac;
    char caSmt[4048],caOrder[64];
    char caDel[16],caExport[16];
    utPltDbHead *psDbHead;
   	int iReturn;
   	long lGroupid0,lRowNum,lUseflags,lStartRec,lUseflags0,lTotRec;
   	char servicecode1[24],dispname1[68],address1[130],ceoname1[32],handphone1[34],contact1[34],telphone1[68],email1[130];
   	char ip1[20],servicestate1[4],jointype1[4],area1[72],areaman1[32],joincode1[8];
 	  pasDbCursor *psCur;
 	  char caUsername2[32];
 	  char caMsg[1024];
 	  char caOnlineflag[16];
 	  long lMonflag=0;
 	  unsigned long lCurtime,lTime,lRuntime;
   	unsigned long lCase;
   	char caYear[30];
   	char caPlate[128];
   	char caOpt[256];
   	char caVersion[20];
   	char caPage[16],caLimit[16];
   	char caDemo[256];
   	char caDemo1[256];
    long lGtype=0;
    char caCode[16],caName[64];
    char caFcod_c[16],caGtype[16],caTcode[16];
    long lSumcorp;
    char caKeyword[36];
    char caWireflag[16];
    char caUpdate[16],caUserid[16],caTelphone[24],caGroupid1[16];
    struct servicedata_s *pCorp;
    FILE *fp;
    char pFile[256],caBuf[128];
    char caLxstate[16];
    long lLxstate;
     char caTemp_d[256];
    char caSesid[1024];
    char caMove[16],caMid[16];
    char caOut[200][256];
    char caFcode[16],caFname[68];
    char caSdate[20],caAddtime[20];
    char caGroups[1024];
    char caFacs[1024];
    char caServicestate[16];
    char caLastln[20];
    char caClastime[24];
    char caRqlx[16];
    long lShflag,lWorkflag,lYyflag;
    char caJointype[20],caJointype_t[20];
    	 char caFlag[16],caSet[16],caSid[16];
    	 char caJrcs[68],caDwlb[68];
    	 char *pHash_lb;
    	 char caWorkflag[16],caShflag[16],caYyflag[16],caDataflag[16];
        //获取权限组ID和接入厂商
	
	//		unsigned char *psUserAtt;
			
    strcpy(caFacs,getDsFacGroupid());
    strcpy(caGroups,getDsGroupid());

    memset(caAll,0,sizeof(caAll));
   
    

    
 //   printf("caAll=%s\n",caAll);
   
    pHash_fac = (unsigned char *)pasLHashInit(30,30,sizeof(struct fac_s),0,12);    
    if(pHash_fac == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
       //将接入厂家装入到内存   
     sprintf(caTemp,"select code,name from ncsfactorycod limit 0,29 "); 
    psCur = pasDbOpenSql(caTemp,0);

    if(psCur){
    memset(caCode,0,sizeof(caCode));
    memset(caName,0,sizeof(caName));
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_STRING,10,caCode,
      	                             UT_TYPE_STRING,63,caName);
      	                            
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
   		 psFac = (struct fac_s *)pasLHashLookA(pHash_fac,caCode);  
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
   
   
   
    
    pHash = (unsigned char *)pasLHashInit(5000,5000,sizeof(struct servicedata_s),4,16);    
    if(pHash == NULL) {
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;        
    }
    
    
    memset(caOpt,0,sizeof(caOpt)); 
    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
   	
   	
 	  lSumuser=0;
 	  dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);
	  
	  iReturn = dsCltGetMyInfo(1,"Userid",&lId);
    js_flag=0;

    	
	  
	  char caSh[32];
 utMsgPrintMsg(psMsgHead);
	  iReturn = utMsgGetSomeNVar(psMsgHead,51,
	                 "TotPg",            UT_TYPE_STRING, 12, caTotPg,
			             "CurPg",            UT_TYPE_STRING, 12, caCurPg,
			             "groupid",          UT_TYPE_STRING, 12, caGroupid,  	              
   	                "sort",            UT_TYPE_STRING, 15, caSort,
   	                "dir",          UT_TYPE_STRING, 8,  caDir,   	              
   	                "del",             UT_TYPE_STRING, 10,caDel,
   	                "export2",          UT_TYPE_STRING,10,caExport,
   	                "name",            UT_TYPE_STRING,63,name,
   	                "servicecode1",    UT_TYPE_STRING,16,servicecode1,
   	                "dispname1",       UT_TYPE_STRING,64,dispname1,
   	                "address1",        UT_TYPE_STRING,128,address1,
   	                "ceoname1",        UT_TYPE_STRING,30,ceoname1,
   	                "handphone1",      UT_TYPE_STRING,32,handphone1,   	                
   	                "telphone1",       UT_TYPE_STRING,64,telphone1,
   	                "email1",          UT_TYPE_STRING,128,email1,
   	                "ip1",             UT_TYPE_STRING,16,ip1,
   	                "joincode1",       UT_TYPE_STRING,7,joincode1,
   	                "servicestate1",    UT_TYPE_STRING,2,servicestate1,
   	                "jointype1",       UT_TYPE_STRING,3,jointype1,
   	                "area1",           UT_TYPE_STRING,60,area1,
   	                "contact1",         UT_TYPE_STRING,30,contact1,
   	                "areaman1",        UT_TYPE_STRING,29,areaman1,
   	                "onlineflag",      UT_TYPE_STRING,2,caOnlineflag,
   	                "plate",           UT_TYPE_STRING,120,caPlate,
   	                "page",            UT_TYPE_STRING,10,caPage,
   	                "limit",           UT_TYPE_STRING,10,caLimit,
   	                "fcode",           UT_TYPE_STRING,10,caFcod_c,
   	                "lb",           UT_TYPE_STRING,10,caGtype,
   	                "lxstate",           UT_TYPE_STRING,10,caLxstate,
   	                "fcode",           UT_TYPE_STRING,10,caFcode,
   	                 "selsid",UT_TYPE_STRING,1000,caSesid,
   	                "did",   UT_TYPE_STRING,10,caDid,
   	                "keyword",UT_TYPE_STRING,30,caKeyword,
   	                "move",UT_TYPE_STRING,10,caMove,
   	                "mid",UT_TYPE_STRING,10,caMid,
   	                "update",UT_TYPE_STRING,10,caUpdate,			         
   	                "sid",UT_TYPE_STRING,10,caSid,
			            "set",UT_TYPE_STRING,10,caSet,
			            "flag",UT_TYPE_STRING,10,caFlag,
			            "lastln",UT_TYPE_STRING,10,caLastln,
			            "jrcs",  UT_TYPE_STRING,60,caJrcs,
			            "dwlb",  UT_TYPE_STRING,60,caDwlb,
			            "rqlx",  UT_TYPE_STRING,3,caRqlx,
			            "demo1", UT_TYPE_STRING,100,caDemo1,
			            "wireflag",UT_TYPE_STRING,8,caWireflag,
			            "jointype",UT_TYPE_STRING,19,caJointype_t,
			            "sh",   UT_TYPE_STRING,8,caSh,
			            "workflag",UT_TYPE_STRING,6,caWorkflag,
			            "shflag",  UT_TYPE_STRING,6,caShflag,
			            "yyflag",  UT_TYPE_STRING,6,caYyflag,
			            "dataflag", UT_TYPE_STRING,6,caDataflag);
	char caJointype_c[20];
	   memset(caJointype_c,0,sizeof(caJointype));            
	    if(strlen(caJointype_t)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caJointype_t,caTemp,64));
    	strcpy(caJointype_t,caTemp_d);
    	strcpy(caJointype_c,ncsGetJoinCodeByName(caJointype_t));
 printf("caJointype_c=%s\n",caJointype_c);   	
    }		            
			            
			            
	if(strcmp(caGtype,"undefined")==0) strcpy(caGtype,"");
	if(strcmp(caGroupid,"undefined")==0) strcpy(caGroupid,"");
	if(strcmp(caFcode,"undefined")==0) strcpy(caFcode,"");
		
   	if(strcmp(caSet,"set")==0&&strlen(caSid)>0){
	  	sprintf(caTemp,"update ncsuser set servicestate='%s' where userid=%s",caFlag,caSid);
	  	printf("caTemp=%s\n",caTemp);
	  	pasDbExecSql(caTemp,0);
	  }
   
      if(strlen(caJrcs)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caJrcs,caTemp,64));
    	strcpy(caJrcs,caTemp_d);
    	
    	sprintf(caTemp,"select code from ncsfactorycod where name='%s' ",caJrcs);
    	memset(caCode,0,sizeof(caCode));
    	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,10,caCode);
    	if(strlen(caCode)>0){
    		strcpy(caJrcs,caCode);
    	}
    }
     if(strlen(caDwlb)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,caDwlb,caTemp,64));
    	strcpy(caDwlb,caTemp_d);
    	sprintf(caTemp,"select code from ncsuser_lb where name='%s' ",caDwlb);

    	memset(caCode,0,sizeof(caCode));
    	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,10,caCode);

    	if(strlen(caCode)>0){
    		strcpy(caDwlb,caCode);
    	}
    	
    	
    }
   
   	                
   if(strlen(dispname1)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,dispname1,caTemp,64));
    	strcpy(dispname1,caTemp_d);
    }
    if(strlen(area1)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,area1,caTemp,64));
    	strcpy(area1,caTemp_d);
    }
    if(strlen(address1)>0){
    	strcpy(caTemp_d,pasCvtGBK(2,address1,caTemp,127));
    	strcpy(address1,caTemp_d);
    }                
   	if(strlen(caKeyword)>0){
   		 iReturn=pasCharIsUtf8(caKeyword, strlen(caKeyword));
   		 if(iReturn){
   		   strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,30));
   		   strcpy(caKeyword,caTemp_d);
   		  }
   		
   	}               
   	                
   if(strcmp(caDir,"ASC")==0){
   	iSortby_corp=1;
  }
  else{
  	iSortby_corp=0;
  }
  if(strcmp(caSort,"dispname")==0){
  		 iSort_corp=2;
  }
 else if(strcmp(caSort,"username")==0){
  			iSort_corp=1;
  }
  else if(strcmp(caSort,"status")==0){
  	iSort_corp=3;
  }
  else if(strcmp(caSort,"onuser")==0){
  	iSort_corp=4;
  }
  else if(strcmp(caSort,"groupname")==0){
  	iSort_corp=6;
  }
  else if(strcmp(caSort,"version")==0){
  	iSort_corp=7;
  }
  else{
  	iSort_corp=5;
  }
  
  //修改或增加
  if(strcmp(caUpdate,"update")==0){
  	   	iReturn = utMsgGetSomeNVar(psMsgHead,7,
   	      "userid",      UT_TYPE_STRING, 10, caUserid,
					"username",    UT_TYPE_STRING, 30, caUsername,
				  "groupid1",     UT_TYPE_STRING,10,caGroupid1,
					"dispname",    UT_TYPE_STRING, 63, caDispname,
					"address",     UT_TYPE_STRING, 127, caAddress,
					"telphone",    UT_TYPE_STRING, 15, caTelphone,
				  "contact",     UT_TYPE_STRING, 15, caContact);
				  		//编码转换
					if(strlen(caUsername)>0){
						 pasCvtGBK(2,caUsername,caTemp,30);
						 strcpy(caUsername,caTemp);
					}
					if(strlen(caDispname)>0){
						 pasCvtGBK(2,caDispname,caTemp,63);
						 strcpy(caDispname,caTemp);
					}
					if(strlen(caAddress)>0){
						 pasCvtGBK(2,caAddress,caTemp,63);
						 strcpy(caAddress,caTemp);
					}
					if(strlen(caTelphone)>0){
						 pasCvtGBK(2,caTelphone,caTemp,63);
						 strcpy(caTelphone,caTemp);
					}	
					if(strlen(caContact)>0){
						 pasCvtGBK(2,caContact,caTemp,63);
						 strcpy(caContact,caTemp);
					}	
					if(atol(caUserid)>0){
						lUserid = atol(caUserid);
						sprintf(caTemp,"update ncsuser set username='%s',groupid=%s,dispname='%s',address='%s',contact='%s',telphone='%s',moditime=%lu,fcode='%s',gtype=%lu,jointype='%s' where userid=%s ",
						caUsername,caGroupid1,caDispname,caAddress,caContact,caTelphone,time(0),caJrcs,atol(caDwlb),caJointype_c,caUserid);
						printf("caTemp=%s\n",caTemp);
						iReturn = pasDbExecSqlF(caTemp);
						            		
						
						sprintf(caTemp,"修改上网场所信息，代码：%s,显示名：%s",caUsername,caDispname);    
		        ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caTemp);  
					}
					else{
						   lUserid=utMd5Code(caUsername,strlen(caUsername),"ffff");
				       if(lUserid>2000000000) lUserid=lUserid-2000000000;       
				       if(lUserid>2000000000) lUserid=lUserid-2000000000;
				        sprintf(caTemp,"select count(*) from ncsuser where userid=%lu ",lUserid);
				        lCount=0;
				       pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
				       if(lCount>0){
				       	lUserid=lUserid+10;
				      }
				       
						 strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M",time(0)));
						 
							 sprintf(caTemp,"insert into ncsuser (userid,username,groupid,dispname,address,telphone,contact,handphone,email,ip,addtime,lasttime,\
		               servicestate,joincode,endnum,servernum,empnum,area,areaman,areatel,demo,postcode,ceoname,moditime,monflag,password,fcode,gtype,jointype) values \
                                                 (%lu,'%s', %lu,'%s','%s','%s','%s','%s','%s','%s','%s','%s',\
                                                 '%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%lu,%lu,'','%s',%lu,'%s')",\
                                                         lUserid,caUsername,strtoul(caGroupid1,NULL,10),caDispname,caAddress,caTelphone,caContact,"","","",caSdate,caSdate,
														 "","","","","",
		 												 "","","","","","",time(0),0,caJrcs,atol(caDwlb),caJointype_c);
	 					 printf("sqlbuf=%s\n",caTemp);	
	 					 							 
             iReturn = pasDbExecSqlF(caTemp);
           		 if(iReturn==0){
            		 ncsWebReqSyscorp_v4(psShmHead,caTemp); 
            	  }
             
             
             sprintf(caTemp,"添加上网场所 场所编码%s，名称：%s",caUsername,caDispname);    
             ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caTemp);
						
					}
					if(iReturn==0){
						  psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lUserid);
			        if(psClient) {
			            strcpy(psClient->username,caUsername);
			            strcpy(psClient->dispname,caDispname);
			            psClient->groupid=strtoul(caGroupid1,NULL,10);
			        }
					}
					
					
  }
  
  
  
  
   if(!utStrIsSpaces(caDel)) {  /* 删除选中的项目*/
    	if(strlen(caSesid)>0){
    		      iReturn = pasDbExecSqlF("delete from ncsuser where userid in(%s)",caSesid);
            	sprintf(caMsg,"delete from ncsuser where userid in(%s) ",caSesid);
            	if(iReturn==0){
            		  		
            		ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caMsg);
            		
            		
            	}
            	else{
            		ncsWriteSysLog(psMsgHead,caUsername2,"05","失败",caMsg);
            	}
    	}
		}

		 if(!utStrIsSpaces(caSh)) {  /* 审核*/
    	if(strlen(caSesid)>0){
    		 iReturn = pasDbExecSqlF("update ncsuser set shflag=1 where userid in(%s)",caSesid);
   // 		 printf("update ncsuser set shflag=1 where userid in(%s)\n",caSesid);
 //   printf("pid=%d\n",getpid());
 //   sleep(15);		
				 lCount=sepcharbydh(caSesid,caOut);
				 for(i=0;i<lCount;i++){
				 
            	sprintf(caMsg,"审核单位%s ",ncsUtlGetClientNameById(psShmHead,atol(caOut[i]),"未知"));
            	if(iReturn==0){
            		  		
            		ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caMsg);
            		
            		
            	}
            	else{
            		ncsWriteSysLog(psMsgHead,caUsername2,"05","失败",caMsg);
            	}
            }
    	}
		}
		
		
		
		if(strcmp(caMove,"move")==0){
			if(strlen(caSesid)>0&&strlen(caMid)>0){
				 iReturn = pasDbExecSqlF("update ncsuser set groupid=%s where userid in(%s)",caMid,caSesid);
				 lCount=sepcharbydh(caSesid,caOut);
				 for(i=0;i<lCount;i++){
			//	 	printf("caOut=%s\n",caOut[i]);
				 	       psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,atol(caOut[i]));
				        if(psClient) {
				            psClient->groupid=atol(caMid);
				           
				        }
				 }
			}
		}

    if(strlen(caPage)>0){
    	strcpy(caCurPg,caPage);
    }
    lCurPg = atol(caCurPg);//当前页面
    if(lCurPg <= 0) lCurPg = 1 ;//如若当前页面小于0，置1
    
     	if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
         lRowNum =40;//每一页面行数
      }
  
    lStartRec = (lCurPg - 1) * lRowNum;   //当前页面的开始第一行的行数	

       
   sprintf(caSmt,"select useflags,userid,dispname,username,ip,lasttime,email,address,telphone,handphone,contact,monflag,version,demo,b.groupname,gtype,a.groupid,a.addtime,a.fcode,a.servicestate,a.jointype,a.shflag,a.workflag,a.yyflag from ncsuser a left join ncsgroup b on ( a.groupid=b.groupid) where 1=1 ");

      if(strlen(servicecode1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and username like '%c%s%c' ",'%',servicecode1,'%');
    }
    if(strlen(dispname1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and dispname like '%c%s%c' ",'%',dispname1,'%');
    }
    if(strlen(address1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and address like '%c%s%c' ",'%',address1,'%');
    }
    if(strlen(ceoname1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and ceoname like '%c%s%c' ",'%',ceoname1,'%');
    }
    if(strlen(handphone1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and handphone like '%c%s%c' ",'%',handphone1,'%');
    }
    if(strlen(contact1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and contact like '%c%s%c' ",'%',contact1,'%');
    }
    if(strlen(telphone1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and telphone like '%c%s%c' ",'%',telphone1,'%');
    }
    if(strlen(email1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and email like '%c%s%c' ",'%',email1,'%');
    }
    if(strlen(ip1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and ip like '%c%s%c' ",'%',ip1,'%');
    }
    
    if(strlen(joincode1)!=0){
     sprintf(caSmt+strlen(caSmt),"  and joincode like '%c%s%c' ",'%',joincode1,'%');
    }
    if(strlen(servicestate1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and servicestate= '%s' ",servicestate1);
    }
    if(strlen(jointype1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and jointype= '%s' ",jointype1);
    }
    if(strlen(area1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and area like '%c%s%c' ",'%',area1,'%');
    }
    if(strlen(caWorkflag)>0){
    	sprintf(caSmt+strlen(caSmt)," and workflag=%s ",caWorkflag);
    }
    if(strlen(caShflag)>0){
    	sprintf(caSmt+strlen(caSmt)," and shflag=%s ",caShflag);
    }
    if(strlen(caYyflag)>0){
    	sprintf(caSmt+strlen(caSmt)," and yyflag=%s ",caYyflag);
    }
    
    
    if(strlen(areaman1)!=0){
    sprintf(caSmt+strlen(caSmt),"  and areaman like '%c%s%c' ",'%',areaman1,'%');
    }
    if(atol(caGroupid)==99999999){
    	sprintf(caSmt+strlen(caSmt)," and a.groupid not in (select groupid from ncsgroup ) ");
    }
    else if(strlen(caGroupid)>0){
    	sprintf(caSmt+strlen(caSmt)," and a.groupid in (%s) ",getGroupid(atol(caGroupid)));
//    	sprintf(caSmt+strlen(caSmt)," and (a.groupid=%s or a.groupid in (select groupid from ncsgroup where pid=%s)) ",caGroupid,caGroupid);
    }
//权限
     if(strlen(caGroups)>0){
    	sprintf(caSmt+strlen(caSmt)," and a.groupid in (%s) ",caGroups);
    }
    if(strlen(caFacs)>0){
    	sprintf(caSmt+strlen(caSmt)," and a.fcode in (%s) ",caFacs);
    }
    if(strlen(caWireflag)>0){
    	if(strcmp(caWireflag,"2")==0){
    	   sprintf(caSmt+strlen(caSmt)," and (a.jointype='08' or a.jointype='09') ");
      }
      else if(strcmp(caWireflag,"1")==0){
      	sprintf(caSmt+strlen(caSmt)," and a.jointype!='08' ");
      }
    
    }
    if(strlen(caRqlx)>0){
    	sprintf(caSmt+strlen(caSmt)," and gtype in (%s) ",getRqlxId(caRqlx));
    }
    
     if(strlen(caDemo1)>0){
    	sprintf(caSmt+strlen(caSmt)," and demo like '%c%s%c' ",'%',caDemo1,'%');
    }
    
    
    if(strlen(caGtype)>0){
    		if(atol(caGtype)!=9){
    			 if(atol(caGtype)==200){
    			 	sprintf(caSmt+strlen(caSmt)," and gtype in ('1','2','8','11','7','4')");
    			}
    			else if(atol(caGtype)==300){
    				sprintf(caSmt+strlen(caSmt)," and gtype in ('5','6','10','3')");
    			}
    			else{
    		   sprintf(caSmt+strlen(caSmt)," and gtype=%s ",caGtype);
    		  }
    	}
    	else{
    		sprintf(caSmt+strlen(caSmt)," and gtype not in ('1','2','3','4','5','6','7','8','10','11') ");
    	}
//   		sprintf(caSmt+strlen(caSmt)," and gtype=%s ",caGtype);
  	}
 // 	if(strlen(caFcod_c)>0){
 // 		sprintf(caSmt+strlen(caSmt)," and substring(username,9,2)='%s' ",caFcod_c);
  //	}
    if(strlen(caDid)>0){
    	sprintf(caSmt+strlen(caSmt)," and userid=%s ",caDid);
    }
    
    printf("caKeyword=%s\n",caKeyword);
    if(strlen(caKeyword)>0){
    	sprintf(caSmt+strlen(caSmt)," and (dispname like '%c%s%c' or username like '%c%s%c' or demo like '%c%s%c')",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');
    }
    if(strlen(caFcode)>0){
    	sprintf(caSmt+strlen(caSmt)," and fcode='%s' ",caFcode);
    }
 /*   
    if(strlen(caSort)>0&&strcmp(caSort,"state")!=0){
    	sprintf(caSmt+strlen(caSmt),"  order by %s ",caSort);
    }
    else{
    sprintf(caSmt+strlen(caSmt),"  order by dispname ");
    }
    if(strcmp(caSortby,"desc")==0){
    	sprintf(caSmt+strlen(caSmt),"  desc ");
    }
 */
  printf("caSmt=%s\n",caSmt);
     lLxstate=atol(caLxstate);
    psCur = pasDbOpenSql(caSmt,0);
    if(psCur==NULL) {  	  
    	
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    	}
       memset(caDispname,0,sizeof(caDispname));
       memset(caUsername,0,sizeof(caUsername));
       memset(caIp,0,sizeof(caIp));
       memset(caDate,0,sizeof(caDate));
       memset(caEmail,0,sizeof(caEmail));
       memset(caAddress,0,sizeof(caAddress));
       memset(caTel,0,sizeof(caTel));
       memset(caHandphone,0,sizeof(caHandphone));
       memset(caContact,0,sizeof(caContact));
       lUseflags=0;
       lMonflag=0;
       memset(caVersion,0,sizeof(caVersion));
       memset(caDemo,0,sizeof(caDemo));
       memset(caGroupname,0,sizeof(caGroupname));
       lGtype=0;
       lGroupid=0;
       memset(caAddtime,0,sizeof(caAddtime));
       memset(caFcode,0,sizeof(caFcode));
       memset(caServicestate,0,sizeof(caServicestate));
       memset(caJointype,0,sizeof(caJointype));
       lShflag=0;
       lWorkflag=0;
       lYyflag=0;
   	   iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lUseflags,
  	                               UT_TYPE_ULONG,4,&lUserid,
  	                               UT_TYPE_STRING,63,caDispname,
  	                               UT_TYPE_STRING,15,caUsername,
  	                               UT_TYPE_STRING,15,caIp,
  	                               UT_TYPE_STRING,15,caDate,
  	                               UT_TYPE_STRING,63,caEmail,
  	                               UT_TYPE_STRING,127,caAddress,
  	                               UT_TYPE_STRING,63,caTel,
  	                               UT_TYPE_STRING,31,caHandphone,
  	                               UT_TYPE_STRING,31,caContact,
  	                               UT_TYPE_LONG,4,&lMonflag,
  	                               UT_TYPE_STRING,15,caVersion,
  	                               UT_TYPE_STRING,250,caDemo,
  	                               UT_TYPE_STRING,31,caGroupname,
  	                               UT_TYPE_STRING,4,caGtype,
  	                               UT_TYPE_ULONG,4,&lGroupid,
  	                               UT_TYPE_STRING,19,caAddtime,
  	                               UT_TYPE_STRING,10,caFcode,
  	                               UT_TYPE_STRING,2,caServicestate,
  	                               UT_TYPE_STRING,4,caJointype,
  	                               UT_TYPE_LONG,4,&lShflag,
  	                               UT_TYPE_LONG,4,&lWorkflag,
  	                               UT_TYPE_LONG,4,&lYyflag);
  	    lSumcorp=0;
        while((iReturn == 0)||(iReturn==1405)) { 
 pAreastatdata = (struct servicedata_s *)pasLHashLookA(pHash,caUsername);  
 //       pAreastatdata = (struct servicedata_s *)pasLHashLookA(pHash,&lUserid);  
       if(pAreastatdata){
       	lSumcorp++;
        pAreastatdata->userid=lUserid;
 //       printf("caVersion=%s,caGtype=%s\n",caVersion,caGtype);
    //			 strcpy(pAreastatdata->caServicecode,caUsername);
    			 strcpy(pAreastatdata->caCname,caDispname);
    			 strcpy(pAreastatdata->caIp,caIp);
    			 strcpy(pAreastatdata->caDate,caDate);
    			 strcpy(pAreastatdata->caEmail,caEmail);
    			 strcpy(pAreastatdata->caAddress,caAddress);
    			 strcpy(pAreastatdata->caHandphone,caHandphone);
    			 strcpy(pAreastatdata->caTel,caTel);
    			 strcpy(pAreastatdata->caContact,caContact);
    			 strcpy(pAreastatdata->caVersion,caVersion);
    			 strcpy(pAreastatdata->caDemo,caDemo);
    			 strcpy(pAreastatdata->caGroupname,caGroupname);
    			 strcpy(pAreastatdata->caJointype,caJointype);
    			 pAreastatdata->lMonflag=lMonflag;
    			 pAreastatdata->lUseflags=lUseflags;
    			 strcpy(pAreastatdata->caGtype,caGtype);
    			 pAreastatdata->groupid=lGroupid;
    			 strcpy(pAreastatdata->caAddtime,caAddtime);
    			 strcpy(pAreastatdata->caFcode,caFcode);
    			 strcpy(pAreastatdata->servicestate,caServicestate);
           pAreastatdata->lShflag=lShflag;
           pAreastatdata->lWorkflag=lWorkflag;
           pAreastatdata->lYyflag=lYyflag;
      psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lUserid);          
      psOnline =  (ncsOnline *)ncsUtlGetOnlineById(psShmHead,lUserid);
       if(psOnline){
       	 strcpy(pAreastatdata->caIp,utComHostIp(htonl(psOnline->lSip)));
       	pAreastatdata->onuid_count=psOnline->lUser;
       	pAreastatdata->lLasttime=psOnline->lLastTime;
       lSumuser=lSumuser+psOnline->lUser;
      }
      else{
      	pAreastatdata->lLasttime=0;
      }
      
      
 //printf("onstate=%d\n",pAreastatdata->onstate);     
     if(pAreastatdata->onstate!=9){
        if(psClient) {
        	if(psClient->status==1||psClient->status==3){       		
        		if(psOnline){
        			pAreastatdata->onstate=1;
        		}
        		else{

        			 pAreastatdata->onstate=2;
        			
        		}
        	}
//        	else if(psClient->status==3){
//        		pAreastatdata->onstate=1;
//        	}
        	else if(psClient->status==2){
        	  pAreastatdata->onstate=2;
        	
        	}
        
        }
       }
       
       
      }
        	
        	
        memset(caDispname,0,sizeof(caDispname));
       memset(caUsername,0,sizeof(caUsername));
       memset(caIp,0,sizeof(caIp));
       memset(caDate,0,sizeof(caDate));
       memset(caEmail,0,sizeof(caEmail));
       memset(caAddress,0,sizeof(caAddress));
       memset(caTel,0,sizeof(caTel));
       memset(caHandphone,0,sizeof(caHandphone));
       memset(caContact,0,sizeof(caContact));
       lUseflags=0;
       lMonflag=0;
       memset(caVersion,0,sizeof(caVersion));
       memset(caDemo,0,sizeof(caDemo));
       memset(caGroupname,0,sizeof(caGroupname));
       lGtype=0;
       lGroupid=0;
       memset(caAddtime,0,sizeof(caAddtime));
       memset(caServicestate,0,sizeof(caServicestate));
       lShflag=0;
       lWorkflag=0;
       lYyflag=0;
   	   iReturn = pasDbFetchInto(psCur,UT_TYPE_LONG,4,&lUseflags,
  	                               UT_TYPE_ULONG,4,&lUserid,
  	                               UT_TYPE_STRING,63,caDispname,
  	                               UT_TYPE_STRING,15,caUsername,
  	                               UT_TYPE_STRING,15,caIp,
  	                               UT_TYPE_STRING,15,caDate,
  	                               UT_TYPE_STRING,63,caEmail,
  	                               UT_TYPE_STRING,127,caAddress,
  	                               UT_TYPE_STRING,63,caTel,
  	                               UT_TYPE_STRING,31,caHandphone,
  	                               UT_TYPE_STRING,31,caContact,
  	                               UT_TYPE_LONG,4,&lMonflag,
  	                               UT_TYPE_STRING,15,caVersion,
  	                               UT_TYPE_STRING,250,caDemo,
  	                               UT_TYPE_STRING,31,caGroupname,
  	                               UT_TYPE_STRING,4,caGtype,
  	                               UT_TYPE_ULONG,4,&lGroupid,
  	                               UT_TYPE_STRING,19,caAddtime,
  	                               UT_TYPE_STRING,10,caFcode,
  	                               UT_TYPE_STRING,2,caServicestate,
  	                               UT_TYPE_STRING,4,caJointype,
  	                               UT_TYPE_LONG,4,&lShflag,
  	                               UT_TYPE_LONG,4,&lWorkflag,
  	                               UT_TYPE_LONG,4,&lYyflag);
            }
        
        
        pasDbCloseCursor(psCur);
    
        //各单位用户数   
    sprintf(caTemp,"select count(b.did ),a.username from ncsuser a left join ncsenduser b on ( a.userid=b.did) where 1=1 ");
    if(!strstr(caOpt,"all")&&(strlen(caOpt)>0)){
			sprintf(caTemp+strlen(caTemp)," and a.groupid in (%s) ",caOpt);
   }
    sprintf(caTemp+strlen(caTemp)," group by a.username order by a.username ");  
    psCur = pasDbOpenSql(caTemp,0);
    if(psCur == NULL) {
    	if(pHash) free(pHash);
        utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm",ncsLang("0613网站访问查询"),ncsLang("0549数据库出错"));
        return 0;
    }
      
		  lCount_u=0;
		  lUserid=0;
  	  i=0;
  	  memset(caUsername,0,sizeof(caUsername));
  	iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_u,
      	                             UT_TYPE_STRING,15,caUsername);
      	                           
      	                            
    	                              
    while((iReturn==0)||(iReturn==1405))
    {
   		 pAreastatdata = (struct servicedata_s *)pasLHashLook(pHash,caUsername);  
       if(pAreastatdata){
    			pAreastatdata->endnum=lCount_u;   	
//    			printf("endnu=%d,lUserid=%d\n",lCount_u,lUserid);
       }	
		  lCount_u=0;
		  lUserid=0;
		  i++;
  	  memset(caUsername,0,sizeof(caUsername));
  	  iReturn = pasDbFetchInto(psCur,
      	                             UT_TYPE_LONG,4,&lCount_u,
      	                             UT_TYPE_STRING,15,caUsername);                              
    }    
    pasDbCloseCursor(psCur);  
    
//统计是否有数据
      char caDataOffTime[12];
      long lDataTime;
      char caToday[32],caTable[32];
      strcpy(caDataOffTime,utComGetVar_sd(psShmHead,"DataTimeOut","86400"));
      lDataTime=atol(caDataOffTime);
      lTime=time(0);
      strcpy(caToday,utTimFormat("%Y%m",lTime));
      sprintf(caTable,"nctermlog_if_%s",caToday);
      sprintf(caTemp,"select distinct servicecode from %s where stime>%d ",caTable,lTime-lDataTime);
      printf("caTemp=%s\n",caTemp);
       psCur = pasDbOpenSql(caTemp,0);
      if(psCur){
      	memset(caUsername,0,sizeof(caUsername));
      	iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,15,caUsername);
      	 while((iReturn==0)||(iReturn==1405))
         {
    
         	   		 pAreastatdata = (struct servicedata_s *)pasLHashLook(pHash,caUsername);  
                 if(pAreastatdata){
           
                 	pAreastatdata->lDataflag=1;
                 }
         	
         	    memset(caUsername,0,sizeof(caUsername));
      	      iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,15,caUsername);
      	      
         }
          pasDbCloseCursor(psCur);  
      }
    
  
 //判断MAC日志是否有   
      sprintf(caTable,"ncmactermatt_if_%s",caToday);
      sprintf(caTemp,"select distinct servicecode from %s where stime>%d ",caTable,lTime-lDataTime);
      printf("caTemp=%s\n",caTemp);
       psCur = pasDbOpenSql(caTemp,0);
      if(psCur){
      	memset(caUsername,0,sizeof(caUsername));
      	iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,15,caUsername);
      	 while((iReturn==0)||(iReturn==1405))
         {
    
         	   		 pAreastatdata = (struct servicedata_s *)pasLHashLook(pHash,caUsername);  
                 if(pAreastatdata){
           
                 	pAreastatdata->lDataflag=1;
                 }
         	
         	    memset(caUsername,0,sizeof(caUsername));
      	      iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,15,caUsername);
      	      
         }
          pasDbCloseCursor(psCur);  
      }
    
   
    
  
       
 //  printf("lSumcorp=%d\n",lSumcorp);    
     pCorp= (struct servicedata_s*)malloc((lSumcorp+1)*sizeof(struct servicedata_s)); 
     if(pCorp==NULL){
     	  utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","分配内存出错","分配内存出错");
        return 0;
    }
    memset(pCorp,0,(lSumcorp+1)*sizeof(struct servicedata_s)); 
    
       
    pAreastatdata  = (struct servicedata_s *)pasLHashFirst(pHash,&sHashInfo);
    iNum = 0;
    lLxstate=atol(caLxstate);
    long lDataflag=atol(caDataflag);
     sprintf(caTable,"ncmactermatt_if_%s",caToday);
    while(pAreastatdata) {

   		    	
    	

         if(strlen(caLxstate)==0||pAreastatdata->onstate==lLxstate){
         	if(strlen(caDataflag)==0||pAreastatdata->lDataflag==lDataflag){
         pCorp[iNum].userid=pAreastatdata->userid;
         strcpy(pCorp[iNum].caServicecode,pAreastatdata->caServicecode);
         strcpy(pCorp[iNum].caCname,pAreastatdata->caCname);
         strcpy(pCorp[iNum].caIp,pAreastatdata->caIp);
         strcpy(pCorp[iNum].caDate,pAreastatdata->caDate);
         strcpy(pCorp[iNum].caEmail,pAreastatdata->caEmail);
         strcpy(pCorp[iNum].caAddress,pAreastatdata->caAddress);
         strcpy(pCorp[iNum].caHandphone,pAreastatdata->caHandphone);
         strcpy(pCorp[iNum].caTel,pAreastatdata->caTel);
    
         strcpy(pCorp[iNum].caContact,pAreastatdata->caContact);
         strcpy(pCorp[iNum].caVersion,pAreastatdata->caVersion);
         strcpy(pCorp[iNum].caDemo,pAreastatdata->caDemo);
         strcpy(pCorp[iNum].caGroupname,pAreastatdata->caGroupname);
         strcpy(pCorp[iNum].caJointype,pAreastatdata->caJointype);
         pCorp[iNum].lMonflag=pAreastatdata->lMonflag;
         pCorp[iNum].lUseflags=pAreastatdata->lUseflags;
         pCorp[iNum].onuid_count=pAreastatdata->onuid_count;
         pCorp[iNum].onstate=pAreastatdata->onstate;
   //      printf("state=%d\n",pAreastatdata->onstate);
         strcpy(pCorp[iNum].caGtype,pAreastatdata->caGtype);
         pCorp[iNum].endnum=pAreastatdata->endnum;
         pCorp[iNum].groupid=pAreastatdata->groupid;
         pCorp[iNum].lShflag=pAreastatdata->lShflag;
         pCorp[iNum].lWorkflag=pAreastatdata->lWorkflag;
         pCorp[iNum].lYyflag=pAreastatdata->lYyflag;
         pCorp[iNum].lDataflag=pAreastatdata->lDataflag;
         strcpy(pCorp[iNum].caAddtime,pAreastatdata->caAddtime);
         strcpy(pCorp[iNum].caFcode,pAreastatdata->caFcode);
         pCorp[iNum].lLasttime=pAreastatdata->lLasttime;
         if(pCorp[iNum].endnum<pCorp[iNum].onuid_count){
         	pCorp[iNum].endnum=pCorp[iNum].onuid_count;
        }
         iNum++;
        }
       }
       
       pAreastatdata = (struct servicedata_s *)pasLHashNext(&sHashInfo);
    }
    
    lSumcorp=iNum;

    qsort(pCorp,lSumcorp,sizeof(struct servicedata_s),ncsUtlSortOnCorp);

//  printf("lSumcopr=%d\n",lSumcorp);
    if(!utStrIsSpaces(caExport)){
    	FILE *fp;
    	char caFilename[32];
    	char caFile[128];
    	char caPath[128];
    	char caState_desc[32];
      sprintf(caFilename,"ncwarn%d.csv",utStrGetId());
      sprintf(caPath,"%s/html","/home/ncmysql/ncs");
      sprintf(caFile,"%s/%s",caPath,caFilename);
    	fp = fopen(caFile,"w");
        if(fp == NULL) {
            pasDbCloseCursor(psCur);
            utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm",ncsLang("0893监控单位"),ncsLang("0546数据导出出错"));
            return 0;
        }
    ncsWriteSysLog(psMsgHead,caUsername2,"06","成功","导出上网服务场所信息"); 
       
      fprintf(fp,(char *)ncsLang("0893监控单位\n"));
      ncUtlPrintCsv(fp,12,UT_TYPE_STRING,"场所编码",
                       UT_TYPE_STRING,ncsLang("0906单位名称"),
                       UT_TYPE_STRING,ncsLang("0054IP地址"),
                       UT_TYPE_STRING,ncsLang("0907E-Mail"),
                       UT_TYPE_STRING,ncsLang("0908地址"),
                       UT_TYPE_STRING,ncsLang("0910电话"),
                       UT_TYPE_STRING,ncsLang("0909手机"),
                       UT_TYPE_STRING,ncsLang("0911联系人"),
                       UT_TYPE_STRING,ncsLang("0912最后上线时间"),
                       UT_TYPE_STRING,"单位组",
                       UT_TYPE_STRING,"状态",
                       UT_TYPE_STRING,"备注");
                      
       for(i=0;i<lSumcorp;i++){
       	       if(pCorp[i].onstate==1){
       	       	strcpy(caState_desc,"在线");
       	      }
       	      else if(pCorp[i].onstate==2){
       	      	strcpy(caState_desc,"离线");
       	      }
       	      else if(pCorp[i].onstate==4){
       	      	strcpy(caState_desc,"停业");
       	      }
       	      else if(pCorp[i].onstate==5){
       	      	strcpy(caState_desc,"维护");
       	      }
       	     else if(pCorp[i].onstate==9){
       	      	strcpy(caState_desc,"待审核");
       	      }
       	      else{
       	      	strcpy(caState_desc,"");
       	      }
       	       sprintf(caTemp,"\r%s",pCorp[i].caServicecode);
       	        ncUtlPrintCsv(fp,12,UT_TYPE_STRING,caTemp,
                       UT_TYPE_STRING,pCorp[i].caCname,
                       UT_TYPE_STRING,pCorp[i].caIp,
                       UT_TYPE_STRING,pCorp[i].caEmail,
                       UT_TYPE_STRING,pCorp[i].caAddress,
                       UT_TYPE_STRING,pCorp[i].caTel,
                       UT_TYPE_STRING,pCorp[i].caHandphone,
                       UT_TYPE_STRING,pCorp[i].caContact,
                       UT_TYPE_STRING,pCorp[i].caDate,
                       UT_TYPE_STRING,pCorp[i].caGroupname,
                       UT_TYPE_STRING,caState_desc,
                       UT_TYPE_STRING,pCorp[i].caDemo);
       	
      }
        
    
   	 
        fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
        free(pHash);
        free(pHash_fac);
        free(pCorp);
        return 0;
    }
         
    psDbHead = utPltInitDb();
   

   	iNum = 0;
  	
   	for(i=0;i<lSumcorp;i++){
   		

   		
   		
   		
   		
   		if(i>=lStartRec&&iNum<lRowNum){
   			if(iNum>0){
        	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
        }
   			iNum++;
   			utPltPutLoopVarF(psDbHead,"useflags", iNum,"%d",pCorp[i].lUseflags);
        utPltPutLoopVarF(psDbHead,"userid",   iNum,"%lu",pCorp[i].userid);
        utPltPutLoopVar(psDbHead,"username", iNum,pCorp[i].caServicecode);
        utPltPutLoopVar(psDbHead,"dispname", iNum,pCorp[i].caCname);
        utPltPutLoopVar(psDbHead,"groupname",iNum,pCorp[i].caGroupname);
        utPltPutLoopVarF(psDbHead,"num", iNum,"%d",iNum-1);
        utPltPutLoopVar(psDbHead,"email", iNum,pCorp[i].caEmail);
        utPltPutLoopVar(psDbHead,"addr",iNum,pCorp[i].caAddress);
        utPltPutLoopVar(psDbHead,"contact",iNum,pCorp[i].caContact);
        utPltPutLoopVar(psDbHead,"ip", iNum,pCorp[i].caIp);
        utPltPutLoopVar(psDbHead,"version",iNum,pCorp[i].caVersion);
        utPltPutLoopVarF(psDbHead,"groupid",iNum,"%lu",pCorp[i].groupid);
        utPltPutLoopVar(psDbHead,"telphone",iNum,pCorp[i].caTel);
        utPltPutLoopVar(psDbHead,"jointype",iNum,ncsGetJoinTypeByCode(pCorp[i].caJointype));
//   			if(strlen(pCorp[i].caServicecode)>10){
//       	memset(caCode,0,sizeof(caCode));
//       	char *p=&pCorp[i].caServicecode[0];
//       	memcpy(caCode,p+7,1);
  //     	printf("caCode=%s\n",caCode);
//       	  utPltPutLoopVar(psDbHead,"dwlx",iNum,ncsGetUserTnameByType(caCode));
 //       }
       	memset(caCode,0,sizeof(caCode));
       	strcpy(caCode,pCorp[i].caFcode);
       psFac=(struct fax_s *)pasLHashLook(pHash_fac,caCode);
       if(psFac){
       	utPltPutLoopVar(psDbHead,"jrcj",iNum,psFac->caName);
       }
       
       
       memset(caCode,0,sizeof(caCode));
       	sprintf(caCode,"%s",pCorp[i].caGtype);
  //     	printf("caCode=%s\n",caCode);
       psFac=(struct fax_s *)pasLHashLook(pHash_lb,caCode);
       if(psFac){
 //      	printf("aaaaaaaa=%s\n",psFac->caName);
       	utPltPutLoopVar(psDbHead,"dwlb",iNum,psFac->caName);
       }
       
       utPltPutLoopVarF(psDbHead,"shflag",iNum,"%d",pCorp[i].lShflag);
       utPltPutLoopVarF(psDbHead,"workflag",iNum,"%d",pCorp[i].lWorkflag);
       utPltPutLoopVarF(psDbHead,"yyflag",iNum,"%d",pCorp[i].lYyflag);
       utPltPutLoopVarF(psDbHead,"dataflag",iNum,"%d",pCorp[i].lDataflag); 
       
   			utPltPutLoopVar(psDbHead,"xxdesc",iNum,"详细");
   			utPltPutLoopVarF(psDbHead,"endnum",iNum,"%d",pCorp[i].endnum);  			    
        utPltPutLoopVarF(psDbHead,"onuser",iNum,"%lu",pCorp[i].onuid_count);
    
       if(pCorp[i].onstate==1){
       	if(pCorp[i].lDataflag!=1){
       		utPltPutLoopVar(psDbHead,"status", iNum,"<font color=blue>异常</font>");
       	}
       	else{
       	utPltPutLoopVar(psDbHead,"status", iNum,"<font color=green>在线</font>");
        }
      }
      else if(pCorp[i].onstate==2){
      	utPltPutLoopVar(psDbHead,"status",iNum,"<font color=red>离线</font>");
      }
      else if(pCorp[i].onstate==4){
      	utPltPutLoopVar(psDbHead,"status",iNum,"<font color=blue>停业</font>");
      }
      else if(pCorp[i].onstate==5){
      	utPltPutLoopVar(psDbHead,"status",iNum,"维护");
      }
      else if(pCorp[i].onstate==9){
      	utPltPutLoopVar(psDbHead,"status",iNum,"待审核");
      }
      utPltPutLoopVar(psDbHead,"xxdesc",iNum,"详细");
       
      if(pCorp[i].lLasttime>0){
      	utPltPutLoopVar(psDbHead,"lasttime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",pCorp[i].lLasttime)); 
      }

      sprintf(caTemp,"%s",pCorp[i].caGtype);
      utPltPutLoopVar(psDbHead,"gtype",iNum,ncsGetUserTnameByType(caTemp));
        utPltPutLoopVar(psDbHead,"demo",iNum,pCorp[i].caDemo);
        
        utPltPutLoopVarF(psDbHead,"CurPg",iNum,"%lu", lCurPg);
 			
   			if(atol(caLastln)==1||(pCorp[i].onstate!=1)){
//获取上次客户端连接时间
   				sprintf(caTemp,"select sdate from ncscltlog where userid=%lu order by sdate desc limit 0,1 ",pCorp[i].userid);
//printf("pid=%d\n",getpid());
//sleep(15);
 				
			printf("caTemp=%s\n",caTemp);
   				memset(caClastime,0,sizeof(caClastime));
   				
   				pasDbOneRecord(caTemp,0,UT_TYPE_STRING,20,caClastime);
   				utPltPutLoopVar(psDbHead,"clastime",iNum,caClastime);
   			}
   			
   		}
   	}
   
     
    lCount=lSumcorp;
    lTotPg = (lCount - 1) / lRowNum + 1;
    lTotRec = lCount;
    i = 0;
//    while(iNum < lRowNum + 1) {
 //       i++;
       
 //       utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
 //        iNum++;
//    }
    utPltPutVar(psDbHead,"sort", caSort);
    utPltPutVar(psDbHead,"sortby", caSortby);

    utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead,"TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead,"CurPg", "%lu", lCurPg);
    utPltPutVar(psDbHead,"groupid",caGroupid);
    utPltPutVar(psDbHead,"did",caDid);
    utPltPutVar(psDbHead,"name",name);
    
    utPltPutVar(psDbHead,"servicecode1",servicecode1);
    utPltPutVar(psDbHead,"dispname1",dispname1);
    utPltPutVar(psDbHead,"ceoname1",ceoname1);
    utPltPutVar(psDbHead,"contact1",contact1);
    utPltPutVar(psDbHead,"ip1",ip1);
    utPltPutVar(psDbHead,"servicestate1",servicestate1);
    utPltPutVar(psDbHead,"gtype",caGtype);
    utPltPutVar(psDbHead,"fcode",caFcod_c);
    utPltPutVar(psDbHead,"lxstate",caLxstate);
    if(pHash) free(pHash);
    if(pCorp) free(pCorp);
   
    free(pHash_fac);
    free(pHash_lb);
//  utPltShowDb(psDbHead); 
  
    	  utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_corp_list.htm");
     
    return 0;
}



/* 显示增加/修改用户的界面  */
int ncsWebDispClientAddForm_wif(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caIp[34],caMac[64],caAddtime[16];
    char caUserid[32];
   	char caUsername[34],caHandphone[32];
   	char caEmail[127],caLasttime[16];
   	char caPassword[32];
   	char caCond[32],caGroupid[16];
   	char caContact[32],caTelphone[32],caAddress[128];
   	int iNum1;
   	long lUse;
   	unsigned long lUserid,lGroupid,lGroupid0;
    utPltDbHead *psDbHead;
   	int iReturn;
   	int iModFlags;
   	char caSmt[256];
   	char *pBuf,*pSelect;
   	char  caUse[16];
   	unsigned long lTime;
   	char caDispname[64],groupname[32];
   	ncsClient *psClient;
   	char caTemp[128];
   	int iNum=0;
   	unsigned int groupid=0;
   	int gtype=0;
    char sqlbuf[4048]="";
    char caGroupcode[32];
   	char handphone[36],servicecode[16],postcode[10],servicestate[12],ceoname[32],areaman[32];
   	char joincode[10],empnum[10],area[72],areatel[32],demo[256],ip[18],servernum[14],endnum[10],jointype[10];
   	int useflags,userid;
   	char caLongitude[16],caLatitude[16];
   	char servernum1[8],postcode1[16],caEmail1[127];
   	char disflag[8];
   	char caPlate[128];
   	char servicestate_choice[16]="selected";
   	char join_type_choice[15]="sel";
   	long lMonflag=0;
    char caAreacodeToUser[32];
   char caFcode[16],caName[68];
    char caGroups[1024];
    char caFacs[1024];   
   long lDateid;
   double fLongitude,fLatitude;
   long lJytype;
   char caCeoidtype[8],caCeocardid[32],caCeotel[64],caSdate[20],caEdate[20],caPort_start[20],caPort_end[20],caJytype[8],caCeltel[32];
   long lWorkflag,lYyflag;
    //获取权限组ID和接入厂商
    strcpy(caFacs,getDsFacGroupid());
    strcpy(caGroups,getDsGroupid());
   
   strcpy(caAreacodeToUser,utComGetVar_sd(psShmHead,"AreacodeToUser","No"));
   
    psDbHead = utPltInitDb();
     sprintf(sqlbuf,"select groupid,groupname,groupcode from ncsgroup where 1=1 ");
    if(strlen(caGroups)>0){
    	sprintf(sqlbuf+strlen(sqlbuf)," and groupid in (%s) ",caGroups);
    }
    sprintf(sqlbuf+strlen(sqlbuf)," order by groupid desc");
    psCur = pasDbOpenSql(sqlbuf,0);
    iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_ULONG,4,&groupid,
      	                               UT_TYPE_STRING,31,groupname,
      	                               UT_TYPE_STRING,16,caGroupcode);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	
        iNum++;       
        utPltPutLoopVarF(psDbHead,"groupid",   iNum,"%lu",groupid);
        utPltPutLoopVar(psDbHead,"groupname",iNum,groupname);
        utPltPutLoopVar(psDbHead,"groupcode",iNum,caGroupcode);
        memset(caGroupcode,0,sizeof(caGroupcode));
        iReturn = pasDbFetchInto(psCur,
      	                               UT_TYPE_ULONG,4,&groupid,
      	                               UT_TYPE_STRING,31,groupname,
      	                               UT_TYPE_STRING,16,caGroupcode);
    }
    pasDbCloseCursor(psCur);
    
    
    
  //接入厂商  
    sprintf(sqlbuf,"select code,name from ncsfactorycod where 1=1 ");
    if(strlen(caFacs)>0){
    	sprintf(sqlbuf+strlen(sqlbuf)," and code in (%s)",caFacs);
    }
    psCur = pasDbOpenSql(sqlbuf,0);
    memset(caFcode,0,sizeof(caFcode));
    memset(caName,0,sizeof(caName));
    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caFcode,
                                   UT_TYPE_STRING,60,caName);
    iNum = 0; 	                               
    while((iReturn==0)||(iReturn==1405))
    {
  	    if(iNum==0){strcpy(caFacs,caFcode);}
        iNum++;       
        utPltPutLoopVar(psDbHead,"fcod",   iNum,caFcode);
        utPltPutLoopVar(psDbHead,"fname",iNum,caName);
		    memset(caFcode,0,sizeof(caFcode));
		    memset(caName,0,sizeof(caName));
		    iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caFcode,
		                                   UT_TYPE_STRING,60,caName);
    }
    pasDbCloseCursor(psCur);
    

    
    sprintf(sqlbuf,"select code,name from ncsuser_lb where 1=1 ");       
		psCur = pasDbOpenSql(sqlbuf,0);
		  	 

		    if(psCur == NULL) {
		    	  utWebDispMsg(iFd,psMsgHead,"ncs/ncmsg_back.htm","部门查询","数据库出错");
		//    	  free(pHash);
		        return 0;
		    }
		   char caCode[16];
	     memset(caName,0,sizeof(caName));
       memset(caCode,0,sizeof(caCode));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
		                                  UT_TYPE_STRING,63,caName);
		                                  
		                                
		
		   iNum=0;
		    while(iReturn == 0||iReturn==1405) {
		        
		         if(iNum>0){
		         	utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		         }
		         iNum++;
		         utPltSetCvtHtml(1);  		        
					   utPltPutLoopVarF(psDbHead,"lbname",iNum,"%s",caName);					    
		         utPltSetCvtHtml(0);  
		         utPltPutLoopVarF(psDbHead,"lbcode",   iNum,"%s",caCode);
		         if(gtype==atol(caCode)){
		         	utPltPutLoopVar(psDbHead,"check",iNum,",checked:true");
		        }
		       
		         	
	     memset(caName,0,sizeof(caName));
       memset(caCode,0,sizeof(caCode));
		   iReturn = pasDbFetchInto(psCur,UT_TYPE_STRING,10,caCode,
		                                  UT_TYPE_STRING,63,caName);
		    }
		    pasDbCloseCursor(psCur);
    
    
    
    
    
    
   	iReturn = utMsgGetSomeNVar(psMsgHead,5,
   	      "groupid", UT_TYPE_STRING, 15, caGroupid,     /* 位置  */
					"userid",  UT_TYPE_STRING, 15, caUserid,
					"cond",    UT_TYPE_STRING, 31, caCond,
					"disflag", UT_TYPE_STRING,2,disflag,
					"plate",UT_TYPE_STRING,100,caPlate);

    iModFlags = 0;
 //   lUserid = atol(caUserid);

    lUserid=strtoul(caUserid,NULL,10);
     printf("caUserid=%s,userid=%lu\n",caUserid,lUserid);
    lTime = time(0);
    if(!utStrIsSpaces(caUserid)) { 
        iModFlags = 1;
    //    lUserid = atol(caUserid);
        
        
        
        
        userid=0;
        memset(caUsername,0,sizeof(caUsername));
        memset(caPassword,0,sizeof(caPassword));
        lGroupid=0;
        memset(caDispname,0,sizeof(caDispname));
        memset(caAddress,0,sizeof(caAddress));
        memset(caTelphone,0,sizeof(caTelphone));
        memset(caContact,0,sizeof(caContact));
        memset(handphone,0,sizeof(handphone));
        memset(caEmail,0,sizeof(caEmail));
        memset(ip,0,sizeof(ip));
        memset(caAddtime,0,sizeof(caAddtime));
        useflags=0;
        memset(caLasttime,0,sizeof(caLasttime));
        gtype=0;
        memset(servicestate,0,sizeof(servicestate));
        memset(joincode,0,sizeof(joincode));
        memset(endnum,0,sizeof(endnum));
        memset(servernum,0,sizeof(servernum));
        memset(jointype,0,sizeof(jointype));
        memset(empnum,0,sizeof(empnum));
        memset(area,0,sizeof(area));
        memset(areaman,0,sizeof(areaman));
        memset(areatel,0,sizeof(areatel));
        memset(demo,0,sizeof(demo));
        memset(servicecode,0,sizeof(servicecode));
        memset(postcode,0,sizeof(postcode));
        memset(ceoname,0,sizeof(ceoname));
       memset(caEmail1,0,sizeof(caEmail1)); 
       memset(servicestate,0,sizeof(servicestate));                    
       memset(endnum,0,sizeof(endnum));
       memset(servernum1,0,sizeof(servernum1));
       memset(ip,0,sizeof(ip));     
       memset(postcode1,0,sizeof(postcode1));
       memset(caLasttime,0,sizeof(caLasttime)); 
       memset(caFcode,0,sizeof(caFcode));
       lDateid=0;
       lMonflag=0;
       lJytype=0;
       memset(caCeoidtype,0,sizeof(caCeoidtype));
       memset(caCeocardid,0,sizeof(caCeocardid));
       memset(caSdate,0,sizeof(caSdate));
       memset(caEdate,0,sizeof(caEdate));
       memset(caPort_start,0,sizeof(caPort_start));
       memset(caPort_end,0,sizeof(caPort_end));
       lWorkflag=0;
       lYyflag=0;
        sprintf(sqlbuf,"select userid,username,password,groupid,dispname,address,\
                               telphone,contact,handphone,addtime,useflags,gtype,joincode,\
                               jointype,empnum,area,areaman,areatel,demo,\
                               ceoname,endnum,servernum,postcode,email,servicestate,ip,lasttime,monflag,fcode,dateid,longitude,latitude,\
                               jytype,ceoidtype,ceocardid,ceotel,sdate,edate,port_start,port_end,workflag,yyflag from ncsuser where userid=%lu",lUserid);                
       printf("sqlbuf=%s\n",sqlbuf);  
      memset(caLongitude,0,sizeof(caLongitude));
      memset(caLatitude,0,sizeof(caLatitude));       
        iReturn = pasDbOneRecord(sqlbuf,
                            0,
                            UT_TYPE_LONG,4,&userid,
                            UT_TYPE_STRING,31, caUsername,
                            UT_TYPE_STRING,31, caPassword,
                            UT_TYPE_LONG,4,&lGroupid,
                            UT_TYPE_STRING,63, caDispname,
                            UT_TYPE_STRING,127,caAddress,
                            UT_TYPE_STRING,63, caTelphone,
                            UT_TYPE_STRING,31, caContact,
                            UT_TYPE_STRING,31,handphone,
                            UT_TYPE_STRING,16,caAddtime,
                            UT_TYPE_LONG,4,&useflags, 
                            UT_TYPE_LONG,4,&gtype,
                            UT_TYPE_STRING,10,joincode,
                            UT_TYPE_STRING,10,jointype,
                            UT_TYPE_STRING,10,empnum,
                            UT_TYPE_STRING,70,area,
                            UT_TYPE_STRING,70,areaman,
                            UT_TYPE_STRING,30,areatel,
                            UT_TYPE_STRING,255,demo,
                            UT_TYPE_STRING,30,ceoname,
                            UT_TYPE_STRING,6,endnum,
                            UT_TYPE_STRING,2,servernum1,
                            UT_TYPE_STRING,6,postcode1,
                            UT_TYPE_STRING,127,caEmail1,
                            UT_TYPE_STRING,2,servicestate,
                            UT_TYPE_STRING,16,ip,
                            UT_TYPE_STRING,16,caLasttime,
                            UT_TYPE_LONG,4,&lMonflag,
                            UT_TYPE_STRING,10,caFcode,
                            UT_TYPE_LONG,4,&lDateid,
                            UT_TYPE_STRING,12,caLongitude,
                            UT_TYPE_STRING,12,caLatitude,
                            UT_TYPE_LONG,4,&lJytype,
                            UT_TYPE_STRING,sizeof(caCeoidtype)-1,caCeoidtype,
                            UT_TYPE_STRING,sizeof(caCeocardid)-1,caCeocardid,
                            UT_TYPE_STRING,sizeof(caCeltel)-1,caCeltel,
                            UT_TYPE_STRING,sizeof(caSdate)-1,caSdate,
                            UT_TYPE_STRING,sizeof(caEdate)-1,caEdate,
                            UT_TYPE_STRING,sizeof(caPort_start)-1,caPort_start,
                            UT_TYPE_STRING,sizeof(caPort_end)-1,caPort_end,
                            UT_TYPE_LONG,4,&lWorkflag,
                            UT_TYPE_LONG,4,&lYyflag);
                              
                               
     if(iReturn != 0 && iReturn != PAS_DB_NULLVALUE) {
            utPltFreeDb(psDbHead);
            utWebDispMsg(iFd,psMsgHead,"nc/ncmsg_back.htm","修改用户","用户不存在 %d ",iReturn);
           return 0;
      }
    else{    
          utPltPutVar(psDbHead,"userid",    caUserid);
          utPltPutVar(psDbHead,"username",  caUsername);
          utPltPutVar(psDbHead,"dispname",  caDispname); //单位名称
          
          
//          printf("gtype=%d\n",gtype);
//          printf("lGroupid=%d\n",lGroupid);
          utPltPutVarF(psDbHead,"groupid","%lu",lGroupid);//所属组
        
        

        
        
        
       
          utPltPutVarF(psDbHead,"gtype","%lu",gtype); //公司类型
          	 
                  
          utPltPutVar(psDbHead,"servicestate",servicestate);//服务状态
          utPltPutVar(psDbHead,"endnum",endnum);
          
        
         
          utPltPutVar(psDbHead,jointype,jointype);//接入类型
   
          utPltPutVar(psDbHead,"end_num",endnum);
          utPltPutVar(psDbHead,"servernum",servernum1);
          utPltPutVar(psDbHead,"postcode",postcode1);
          utPltPutVar(psDbHead,"email_addr",caEmail1);
          utPltPutVar(psDbHead,"joincode",joincode);
          utPltPutVar(psDbHead,"servicecode",servicecode);

          utPltPutVar(psDbHead,"address", caAddress); 
          utPltPutVar(psDbHead,"ceoname",ceoname);
          utPltPutVar(psDbHead,"handphone",handphone);
          utPltPutVar(psDbHead,"contact", caContact);  
          utPltPutVar(psDbHead,"telphone",caTelphone);
          utPltPutVar(psDbHead,"empnum",empnum);
          utPltPutVar(psDbHead,"area",area);
          utPltPutVar(psDbHead,"areaman",areaman);
          utPltPutVar(psDbHead,"areatel",areatel);
          utPltPutVar(psDbHead,"entrance_ip", ip);
          utPltPutVar(psDbHead,"jointype",jointype);
          utPltPutVar(psDbHead,"fcode",caFcode);
          utPltPutVarF(psDbHead,"dateid","%d",lDateid);
          utPltPutVarF(psDbHead,"longitude","%s",caLongitude);
          utPltPutVarF(psDbHead,"latitude","%s",caLatitude);
           psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,atol(caUserid));
           if(psClient){
	          if(psClient->status==1)
	          	utPltPutVar(psDbHead,"userflags","正常");
	          else if(psClient->status==3){
	          	sprintf(caTemp,"异常在线:%s",psClient->caMsg);
	            utPltPutVar(psDbHead,"userflags",caTemp);
	          }
	          else if(psClient->status==2){
	          	utPltPutVar(psDbHead,"userflags","离线");
	          }
          }
          utPltPutVar(psDbHead,"addtime", caAddtime);
          utPltPutVar(psDbHead,"lasttime", caLasttime);
          utPltPutVarF(psDbHead,"monflag","%lu",lMonflag);
          utPltPutVarF(psDbHead,"jytype","%d",lJytype);
          utPltPutVar(psDbHead,"ceoidtype",caCeoidtype);
          utPltPutVar(psDbHead,"ceocardid",caCeocardid);
          utPltPutVar(psDbHead,"sdate",caSdate);
          utPltPutVar(psDbHead,"edate",caEdate);
          utPltPutVar(psDbHead,"port_start",caPort_start);
          utPltPutVar(psDbHead,"port_end",caPort_end);
          utPltPutVarF(psDbHead,"workflag","%d",lWorkflag);
          utPltPutVarF(psDbHead,"yyflag","%d",lYyflag);
          utPltSetCvtHtml(1);
          utPltPutVar(psDbHead,"demo",demo);//备注
          utPltSetCvtHtml(0);
  //        printf("demo=%s\n",demo);
          if(strncmp(caPassword,"***",3)!=0 && strncmp(caPassword,"@@@",3)!=0) {
                utPltPutVar(psDbHead,"password", "********");
            }
        
    }
  }
    else {
    	  utPltPutVar(psDbHead,"sdate","08:00");
    	  utPltPutVar(psDbHead,"edate","18:00");
        strcpy(caAddtime,utTimFormat("%Y-%m-%d %H:%M:%S",lTime));
        utPltPutVar(psDbHead,"addtime", caAddtime);
        utPltPutVar(psDbHead,"lasttime",caAddtime);
        if(strlen(caGroupid)!=0)
        utPltPutVar(psDbHead,"groupid",caGroupid);
        else
        utPltPutVar(psDbHead,"groupid","0");
        
        utPltPutVar(psDbHead,"qtcheck",",checked:true");
     
        utPltPutVar(psDbHead,"servicestate","1");//服务状态
        utPltPutVar(psDbHead,"jointype","99");//接入类型
        utPltPutVar(psDbHead,"monflag","0");
        utPltPutVar(psDbHead,"end_num","0");
        utPltPutVar(psDbHead,"servernum","0");
          utPltPutVarF(psDbHead,"workflag","%d",0);
          utPltPutVarF(psDbHead,"yyflag","%d",0);
          
          utPltPutVar(psDbHead,"fcode",caFacs);
    }
  
    
    
    if(strlen(caPlate)>0){
    	utPltOutToHtml(iFd,psMsgHead,psDbHead,caPlate);
    }
    else{
    	utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/nc_form_clientedit.htm");
    }
    return 0;
}



/* 增加用户  */
int ncsWebClientAdd_wif(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{

   	char caUsername[33],caGroupid[33],caIp[16],caMac[33],caMacstr[42],caUse[10];
   	char caDispname[64],caLimit[32];
   	char caContact[32],caTelphone[32],caAddress[128];
   	char caHandphone[32];
   	char caWorkflag[16],caYyflag[16];
   	unsigned long lUserid,lGroupid;
    utPltDbHead *psDbHead;
   	int iReturn;
   	unsigned long lUse,lUserid0;
   	unsigned long lIp;
   	int iPass,iAddFlags;
   	char caPassword[32],caPassword1[32];
   	char caDate[32],caUserid[16],caUsername0[32];
   	char caGtype[16];
   	ncsClient *psClient;
    char caOk[16],caSave[16],caEmail[64],caWebip[16],caPost[16],caLimitid[16];

	char handphone[32],servicecode[16],postcode[16],servicestate[10],ceoname[30],areaman[30],gtype[32],addtime[30],lasttime[30];
  char joincode[16],empnum[16],area[70],areatel[30],demo[255],ip[16],servernum[12],endnum111[10],jointype[12];
  char servicestate_desc[32],pre_setdesc[32],caPreset[16];
	char sqlbuf[4048]="";
  char caTemp[1024];
  char caUsername2[32];
 	char caMsg[1024];

 	char caMonflag[12];
 	char caFcode[16],caDateid[16];
 	char caLongitude[16],caLatitude[16];
  char caCeoidtype[8],caCeocardid[32],caCeotel[64],caSdate[20],caEdate[20],caPort_start[20],caPort_end[20],caJytype[8];
 	memset(caMonflag,0,sizeof(caMonflag));
 	dsCltGetSessionValue(1,"dispname",UT_TYPE_STRING,30,caUsername2);

    strcpy(caDate,utTimFormat("%Y-%m-%d %H:%M:%S",time(0)));

    utMsgPrintMsg(psMsgHead);

   	iReturn = utMsgGetSomeNVar(psMsgHead,40,
   	      "userid",      UT_TYPE_STRING, 10, caUserid,
					"username",    UT_TYPE_STRING, 30, caUsername,
				  "groupid",     UT_TYPE_STRING,10,caGroupid,
					"dispanme",    UT_TYPE_STRING, 63, caDispname,
					"address",     UT_TYPE_STRING, 127, caAddress,
					"telphone",    UT_TYPE_STRING, 15, caTelphone,
				  "contact",     UT_TYPE_STRING, 15, caContact,
					"handphone",   UT_TYPE_STRING, 30,handphone,
				  "email_addr",  UT_TYPE_STRING, 63, caEmail,
					"entrance_ip", UT_TYPE_STRING, 16, caIp,
					"addtime",     UT_TYPE_STRING,24,addtime,
					"lasttime",    UT_TYPE_STRING,24,lasttime,
          "gtype",       UT_TYPE_STRING, 10,gtype,
					"servicestate",UT_TYPE_STRING,10,servicestate,
					"joincode",    UT_TYPE_STRING,6,joincode,				
					"servernum",    UT_TYPE_STRING,4,servernum,
					"end_num111",   UT_TYPE_STRING,8,endnum111,
					"jointype",    UT_TYPE_STRING,2,jointype,
					"empnum" ,      UT_TYPE_STRING,6,empnum,
					"area" ,        UT_TYPE_STRING,70,area,
					"areaman" ,     UT_TYPE_STRING,30,areaman,
					"areatel",      UT_TYPE_STRING,30,areatel,
          "demo" ,         UT_TYPE_STRING,255,demo,
					"servicecode", UT_TYPE_STRING,15,servicecode,
          "postcode",    UT_TYPE_STRING,6,postcode,
					"ceoname",     UT_TYPE_STRING,30,ceoname,
					"monflag",     UT_TYPE_STRING,10,caMonflag,
					"fcode",       UT_TYPE_STRING,10,caFcode,
					"dateid",      UT_TYPE_STRING,10,caDateid,
					"longitude",   UT_TYPE_STRING,12,caLongitude,
					"latitude",    UT_TYPE_STRING,12,caLatitude,
					"jytype",      UT_TYPE_STRING,sizeof(caJytype)-1,caJytype,
					"ceoidtype",   UT_TYPE_STRING,sizeof(caCeoidtype)-1,caCeoidtype,
					"ceocardid",   UT_TYPE_STRING,sizeof(caCeocardid)-1,caCeocardid,
					"sdate",       UT_TYPE_STRING,sizeof(caSdate)-1,caSdate,
					"edate",       UT_TYPE_STRING,sizeof(caEdate)-1,caEdate,
					"port_start",  UT_TYPE_STRING,sizeof(caPort_start)-1,caPort_start,
					"port_end",    UT_TYPE_STRING,sizeof(caPort_end)-1,caPort_end,
					"workflag",    UT_TYPE_STRING,sizeof(caWorkflag)-1,caWorkflag,
					"yyflag",      UT_TYPE_STRING,sizeof(caYyflag)-1,caYyflag);
	 utStrDelSpaces(caUsername);
    utStrDelSpaces(caIp);			
			
	//编码转换
	if(strlen(caUsername)>0){
		 pasCvtGBK(2,caUsername,caTemp,30);
		 strcpy(caUsername,caTemp);
	}
	if(strlen(caDispname)>0){
		 pasCvtGBK(2,caDispname,caTemp,63);
		 strcpy(caDispname,caTemp);
	}
	if(strlen(caAddress)>0){
		 pasCvtGBK(2,caAddress,caTemp,63);
		 strcpy(caAddress,caTemp);
	}
	if(strlen(caTelphone)>0){
		 pasCvtGBK(2,caTelphone,caTemp,63);
		 strcpy(caTelphone,caTemp);
	}	
	if(strlen(caContact)>0){
		 pasCvtGBK(2,caContact,caTemp,63);
		 strcpy(caContact,caTemp);
	}	
	if(strlen(handphone)>0){
		 pasCvtGBK(2,handphone,caTemp,63);
		 strcpy(handphone,caTemp);
	}		
	if(strlen(area)>0){
		 pasCvtGBK(2,area,caTemp,63);
		 strcpy(area,caTemp);
	}	
	if(strlen(areaman)>0){
		 pasCvtGBK(2,areaman,caTemp,63);
		 strcpy(areaman,caTemp);
	}	
	if(strlen(demo)>0){
		 pasCvtGBK(2,demo,caTemp,63);
		 strcpy(demo,caTemp);
	}
	if(strlen(ceoname)>0){
		 pasCvtGBK(2,ceoname,caTemp,63);
		 strcpy(ceoname,caTemp);
	}
		if(strlen(joincode)>0){
		 pasCvtGBK(2,joincode,caTemp,6);
		 strcpy(joincode,caTemp);
	}
		
					
		                     
				

  
    
    if(utStrIsSpaces(caUserid)) {
    	lUserid=pasGetSid("ncsuser","userid");
 //       lUserid =  ncsUtlGenId(psShmHead);
        iAddFlags = 1;
           
    }
    else {
    	lUserid = atol(caUserid);
    	iAddFlags = 0;
    }
    if(iAddFlags) { /* 增加记录  */
        	

    		sprintf(sqlbuf,"添加上网场所%s,名称：%s",caUsername,caDispname);
    		ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",sqlbuf);
 	      strcpy(caSdate,utTimFormat("%Y-%m-%d %H:%M",time(0)));
		      sprintf(sqlbuf,"insert into ncsuser (userid,username,groupid,dispname,address,telphone,contact,handphone,email,ip,addtime,lasttime,\
		               gtype,servicestate,joincode,endnum,servernum,jointype,empnum,area,areaman,areatel,demo,postcode,ceoname,moditime,monflag,password,fcode,dateid,jytype,ceoidtype,ceocardid,sdate,edate,port_start,port_end,workflag,yyflag) values \
                                                 (%lu,'%s', %lu,'%s','%s','%s','%s','%s','%s','%s','%s','%s',\
                                                 %d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%lu,%lu,'','%s',%d,%d,'%s','%s','%s','%s','%s','%s',%d,%d)",\
                                                         lUserid,caUsername,strtoul(caGroupid,NULL,10),caDispname,caAddress,caTelphone,caContact,handphone,caEmail,caIp,caSdate,caSdate,
														 atoi(gtype),servicestate,joincode,endnum111,servernum,jointype,empnum,
		 												 area,areaman,areatel,demo,postcode,ceoname,time(0),atol(caMonflag),caFcode,atol(caDateid),atol(caJytype),caCeoidtype,caCeocardid,caSdate,caEdate,caPort_start,caPort_end,atol(caWorkflag),atol(caYyflag));
	 					printf("sqlbuf=%s\n",sqlbuf);								 
            iReturn = pasDbExecSqlF(sqlbuf);
            if(iReturn==0){
            	 ncsWebReqSyscorp_v4(psShmHead,sqlbuf); 
            }
  //          ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",sqlbuf);
        
//						printf("sqlbuf=%s\n",sqlbuf);					 
   //        iReturn =pasDbExecSqlF(sqlbuf);
           
           sprintf(sqlbuf,"添加上网场所 场所编码%s，名称：%s",caUsername,caDispname);    
           ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",sqlbuf);
            
        if(iReturn != 0) {
        	  strcpy(caMsg,"false");
            pasDbRollback(NULL);
        }
        else { 
            pasDbCommit(NULL);
            strcpy(caMsg,"true");
    //        sprintf(caUserid,"%d",lUserid);
    //        utMsgAdd_s(&psMsgHead,"userid",caUserid);

            if(strlen(caIp)>0){
		        	if(utStrIsValidIp(caIp)){
		        		lIp=ntohl(utComHostAddress(caIp));
		        		sprintf(caTemp,"insert into ncsuserip (userid,lip,uip,stime) values(%lu,%lu,%lu,%lu) ",lUserid,lIp,lIp,time(0));

		        		pasDbExecSql(caTemp,0);
        			}
        		}
            
        }
//    printf("iReturn=%d,msg=%s\n",iReturn,caMsg);    
        psDbHead = utPltInitDb();
        utPltPutVar(psDbHead,"mesg",caMsg);
        utPltPutVarF(psDbHead,"userid","%d",lUserid);
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_msg.htm");
        return 0;
        
    }        
    else { /* 修改记录  */
  
			sprintf(sqlbuf,"update ncsuser set username='%s',groupid=%lu,\
      	        gtype=%d,servicestate='%s',jointype='%s',joincode='%s',endnum='%s',\
				servernum='%s',dispname='%s',address='%s',ceoname='%s',handphone='%s',postcode='%s',\
				email='%s',contact='%s',telphone='%s',areaman='%s',areatel='%s',ip='%s',empnum='%s',area='%s',demo='%s',moditime=%lu,monflag=%lu,fcode='%s',dateid=%d,longitude='%s',latitude='%s',\
				jytype=%d,ceoidtype='%s',ceocardid='%s',sdate='%s',edate='%s',port_start='%s',port_end='%s',workflag=%d,yyflag=%d where userid=%lu",
                caUsername,strtoul(caGroupid,NULL,10),atoi(gtype),servicestate,
				jointype,joincode,endnum111,servernum,caDispname,caAddress,
                ceoname,handphone,postcode,caEmail,caContact,caTelphone,areaman,
				areatel,caIp,empnum,area,demo,time(0),atol(caMonflag),caFcode,atol(caDateid),caLongitude,caLatitude,\
				atol(caJytype),caCeoidtype,caCeocardid,caSdate,caEdate,caPort_start,caPort_end,atol(caWorkflag),atol(caYyflag),atoi(caUserid));

 printf("sqlbuf=%s\n",sqlbuf);     	   
		  
       	    iReturn = pasDbExecSqlF(sqlbuf);
       	    
       	    if(iReturn==0){
            	 ncsWebReqSyscorp_v4(psShmHead,sqlbuf); 
            }
       	    
       			   sprintf(sqlbuf,"修改上网场所信息，代码：%s,显示名：%s",caUsername,caDispname);    
		       ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",sqlbuf);   
		        
        if(iReturn != 0) {
            strcpy(caMsg,"false");
            pasDbRollback(NULL);
        }
        else {
        	  strcpy(caMsg,"true");
            pasDbCommit(NULL);
        }
        lUserid = atol(caUserid);
        psClient =  (ncsClient *)ncsUtlGetClientById(psShmHead,lUserid);
        if(psClient) {
            strcpy(psClient->username,caUsername);
            strcpy(psClient->dispname,caDispname);
        }

         psDbHead = utPltInitDb();
        utPltPutVar(psDbHead,"mesg",caMsg);
        utPltPutVar(psDbHead,"userid",caUserid);
        utPltOutToHtml(iFd,psMsgHead,psDbHead,"v4/ncs_msg.htm");
        return 0;
      }
}
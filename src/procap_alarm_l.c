/* 监控软件---有关Web的应用
     Modify 2002/9/9   by Liyunming*/
#define PAS_SRCFILE      9889
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
#include "ncapi.h"
extern char *pHashNettype;


#include "pas_acbm.h"

static  pattern_data  *psAcbmPat_mac = NULL;      // MAC告警模式内容
static  pattern_tree  *psAcbmTree_mac = NULL;     // Mac告警Acbm模式树

char *convmac(unsigned char *in)
{
    static char out[24];

    long j = 0;
    long i = 0;

    memset(out, 0, sizeof(out));

    if(strlen(in) == 12)
    {
        for(i = 0; i < strlen(in); i++)
        {
            if(i == 2 || i == 4 || i == 6 || i == 8 || i == 10)
            {
                out[j] = '-';
                j++;
            }
            out[j] = in[i];
            j++;
        }
    }
    else
    {
        strcpy(out, in);
    }
    utStrReplaceWith(out, ":", "-");
    utStrToUpper(out);
    return out;

}

//输入单位组ID,返回逗号分割的单位组Code ,"11111","222222","3333"
char *getGroupCodeByGroupid(utShmHead *psShmHead,char *caGroupid){
        static char caReturn[8024];
        char caServicecode[32];
        char caGroupcode[30];
        char caIds[1024];
        char caTempid[1024],caTempcode[1024];
        int iReturn,lId,iNum;
         pasDbCursor *psCur;
         memset(caReturn,0,sizeof(caReturn));
              
              iNum=0;
    
         psCur = pasDbOpenSqlF("select distinct groupcode from ncsgroup where   groupid in(%s) ",caGroupid);
	 	     if(psCur){
	 	     	iReturn=0;
	 	     	lId=0;
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	 iNum=0;
	 	     	while(iReturn==0||iReturn==1405){
	 	     		if(iNum==0){	 	     			
	 	     			sprintf(caReturn,"\"%s\"",caGroupcode);
	 	     		}
	 	     		else{ 	     		
	 	     			sprintf(caReturn+strlen(caReturn),",\"%s\"",caGroupcode);
	 	     		}
	 	     		iNum++;
	 	     		
	 	     			memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	   iReturn=pasDbFetchInto(psCur,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	}
	 	     	 pasDbCloseCursor(psCur);  
	 	     }

	
	 	//  	 sprintf(caReturn,"%s",caOpt);
	 	  	 sprintf(caIds,"%s",caGroupid);
	 iNum=0;
	   long index=0;
	   memset(caTempcode,0,sizeof(caTempcode));
	 	 while(strlen(caIds)>0){
	 	  	memset(caTempid,0,sizeof(caTempid));
	 	  	iNum=0; 

	 	     psCur = pasDbOpenSqlF("select groupid,groupcode from ncsgroup where   pid in(%s) ",caIds);
	 	     if(psCur){
	 	     	iReturn=0;
	 	     	lId=0;
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lId,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	while(iReturn==0||iReturn==1405){
	 	     		if(iNum==100) break;
	 	     		if(iNum==0){
	 	     			sprintf(caTempid,"%lu",lId);
	 	     		
	 	     		}
	 	     		else{
	 	     			sprintf(caTempid+strlen(caTempid),",%lu",lId);
	 	     			
	 	     		}
	 	     		if(index==0){
	 	     				sprintf(caTempcode,"\"%s\"",caGroupcode);
	 	     		
	 	     		}
	 	     		else{
	 	     				sprintf(caTempcode+strlen(caTempcode),",\"%s\"",caGroupcode);
	 	     		}
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lId,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	      			
	 	     			iNum++;
	 	     		  index++;
	 	     	}
	 	     }
	 	  	   pasDbCloseCursor(psCur);               
	  	
		
		 	  	strcpy(caIds,caTempid);
	 	  	}
	 	
	 	  	 if(strlen(caTempcode)>0){
		 	  		sprintf(caReturn+strlen(caReturn),",%s",caTempcode);
		 	   }
	 	 
	 	  return &caReturn[0];              


                
}
//根据权限,获取单位组Code组合
//返回逗号分割的单位组CODE ,"11111","222222","3333"
char *getDsGroupcode(){
	static char caReturn[1024];
	char caIds[1024],caTempid[1024];
	char caGroupcode[20];
	char caTempcode[1024];
	char caOpt[256];
	int iReturn,lId,iNum;
	 pasDbCursor *psCur;
	 memset(caReturn,0,sizeof(caReturn));
   lId=0;

    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
  //  printf("dsiReturn=%d\n",iReturn);
    if(iReturn!=0||lId<=0){
    	 return &caReturn[0];
    }


    	memset(caOpt,0,sizeof(caOpt));
 	    pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
 	    if(strstr(caOpt,"all")||strlen(caOpt)==0){
 	    	return &caReturn[0];
    }

         psCur = pasDbOpenSqlF("select groupcode from ncsgroup where   groupid in(%s) ",caOpt);
	 	     if(psCur){
	 	     	iReturn=0;
	 	     	lId=0;
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	 iNum=0;
	 	     	while(iReturn==0||iReturn==1405){
	 	     		if(iNum==0){	 	     			
	 	     			sprintf(caReturn,"\"%s\"",caGroupcode);
	 	     		}
	 	     		else{ 	     		
	 	     			sprintf(caReturn+strlen(caReturn),",\"%s\"",caGroupcode);
	 	     		}
	 	     		iNum++;
	 	     		
	 	     			memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	   iReturn=pasDbFetchInto(psCur,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	}
	 	     	 pasDbCloseCursor(psCur);  
	 	     }

	
	 	//  	 sprintf(caReturn,"%s",caOpt);
	 	  	 sprintf(caIds,"%s",caOpt);
	     iNum=0;
	   long index=0;
	   memset(caTempcode,0,sizeof(caTempcode));
	 	  while(strlen(caIds)>0){
	 	  	memset(caTempid,0,sizeof(caTempid));
	 	  	iNum=0; 
	
	 	     psCur = pasDbOpenSqlF("select groupid,groupcode from ncsgroup where   pid in(%s) ",caIds);
	 	     if(psCur){
	 	     	iReturn=0;
	 	     	lId=0;
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lId,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	while(iReturn==0||iReturn==1405){
	 	     		if(iNum==100) break;
	 	     		if(iNum==0){
	 	     			sprintf(caTempid,"%lu",lId);
	 	     		
	 	     		}
	 	     		else{
	 	     			sprintf(caTempid+strlen(caTempid),",%lu",lId);
	 	     		
	 	     		}
	 	     			
	 	      	if(index==0){
	 	     				sprintf(caTempcode,"\"%s\"",caGroupcode);
	 	     		
	 	     		}
	 	     		else{
	 	     				sprintf(caTempcode+strlen(caTempcode),",\"%s\"",caGroupcode);
	 	     		}
	 	     			
	 	     			
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lId,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	      			
	 	     			iNum++;
	 	     		  index++;
	 	     	}
	 	     }
	 	  	   pasDbCloseCursor(psCur);               
	  	
		
		 	  	strcpy(caIds,caTempid);
	 	  	}
	 	  	
	 	  	 if(strlen(caTempcode)>0){
		 	  		sprintf(caReturn+strlen(caReturn),",%s",caTempcode);
		 	   }
	 	 
	 	  return &caReturn[0];
 }


//根据权限,获取单位组Code组合
//返回逗号分割的单位组CODE ,"11111","222222","3333"
char *getDsGroupcodeSql(char *field){
	static char caReturn[1024];
	char caIds[1024],caTempid[1024];
	char caGroupcode[20];
	char caTempcode[1024];
	char caOpt[256];
	pasLHashInfo sHashInfo;
	int iReturn,lId,iNum;
	 pasDbCursor *psCur;
	 long lLen;
	  typedef struct ncsDsData_s
    {
        long len;                //字段长度
        char codes[1024];
        long count;                
    } ncsDsData;
    char *pHash;
    ncsDsData *psData;
  
	  memset(caReturn,0,sizeof(caReturn));
	
	 
	 
	 
	 
   lId=0;

    iReturn = dsCltGetMyInfo(1,"Userid",&lId);
  //  printf("dsiReturn=%d\n",iReturn);
    if(iReturn!=0||lId<=0){
    	 return &caReturn[0];
    }

    	memset(caOpt,0,sizeof(caOpt));
 	    pasDbOneRecord("select opt from dsuser where id=:id",1,"id",UT_TYPE_LONG,lId,UT_TYPE_STRING,255,caOpt);
 	    if(strstr(caOpt,"all")||strlen(caOpt)==0){
 	    	return &caReturn[0];
    }

    pHash = (unsigned char *)pasLHashInit(100, 100, sizeof(struct ncsDsData_s), 0, 4);
    if(pHash == NULL)
    {
        return &caReturn[0];
    }


         psCur = pasDbOpenSqlF("select groupcode from ncsgroup where   groupid in(%s) ",caOpt);
	 	     if(psCur){
	 	     	iReturn=0;
	 	     	lId=0;
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	 iNum=0;
	 	     	while(iReturn==0||iReturn==1405){
	 	     		lLen=strlen(caGroupcode);
	 	     		psData = (ncsDsData *)pasLHashLookA(pHash, &lLen);
            if(psData)
            {
            	if(psData->count==0){
            		sprintf(psData->codes,"\"%s\"",caGroupcode);
            	}
            	else{
            		sprintf(psData->codes+strlen(psData->codes),",\"%s\"",caGroupcode);
            	}
            	psData->count++;
            }
	 	     		
	 	     		
	 	     		
	 	     		
	 	     			memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	   iReturn=pasDbFetchInto(psCur,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	}
	 	     	 pasDbCloseCursor(psCur);  
	 	     }

	
	 	//  	 sprintf(caReturn,"%s",caOpt);
	 	  	 sprintf(caIds,"%s",caOpt);
	     iNum=0;
	 	  while(strlen(caIds)>0){
	 	  	memset(caTempid,0,sizeof(caTempid));
	 	  	iNum=0; 
	
	 	     psCur = pasDbOpenSqlF("select groupid,groupcode from ncsgroup where   pid in(%s) ",caIds);
	 	     if(psCur){
	 	     	iReturn=0;
	 	     	lId=0;
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lId,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	while(iReturn==0||iReturn==1405){
	 	     		if(iNum==100) break;
	 	     			
	 	     		lLen=strlen(caGroupcode);
	 	     		psData = (ncsDsData *)pasLHashLookA(pHash, &lLen);
            if(psData)
            {
            	if(psData->count==0){
            		sprintf(psData->codes,"\"%s\"",caGroupcode);
            	}
            	else{
            		sprintf(psData->codes+strlen(psData->codes),",\"%s\"",caGroupcode);
            	}
            	psData->count++;
            }
	 	     			
	 	     		if(iNum==0){
	 	     			sprintf(caTempid,"%lu",lId);
	 	     			
	 	     		}
	 	     		else{
	 	     			sprintf(caTempid+strlen(caTempid),",%lu",lId);
	 	     			
	 	     		}
	 	     			
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lId,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	      			
	 	     			iNum++;
	 	     		
	 	     	}
	 	     }
	 	  	   pasDbCloseCursor(psCur);               
	  	
		
		 	  	strcpy(caIds,caTempid);
	 	  	}
	 	  	
	 	  //拼装SQL语句
	 	  	                       psData=(ncsDsData *)pasLHashFirst(pHash,&sHashInfo);
                               iNum=0;
                               memset(caReturn,0,sizeof(caReturn));
                               while(psData){
                               	if(iNum==0){
                               	  sprintf(caReturn,"( left(%s,%d) in (%s) ",field,psData->len,psData->codes);
                               	}
                               	else{
                               		sprintf(caReturn+strlen(caReturn)," or left(%s,%d) in (%s) ",field,psData->len,psData->codes);
                               	}
                               	iNum++;
                                  
                                psData=(ncsDsData *)pasLHashNext(&sHashInfo);
                               }
	 	  	
	 	  	
	 	  	
	 	  	
	 	  	 if(iNum>0){
		 	  		sprintf(caReturn+strlen(caReturn),")");
		 	   }
	 	 free(pHash);
	 	  return &caReturn[0];
 }


char *getGroupcodeSql(char *field,char *caGroupid){
	static char caReturn[1024];
	char caIds[1024],caTempid[1024];
	char caGroupcode[20];
	char caTempcode[1024];
	char caOpt[256];
	pasLHashInfo sHashInfo;
	int iReturn,lId,iNum;
	 pasDbCursor *psCur;
	 long lLen;
	  typedef struct ncsDsData_s
    {
        long len;                //字段长度
        char codes[1024];
        long count;                
    } ncsDsData;
    char *pHash;
    ncsDsData *psData;
  
	  memset(caReturn,0,sizeof(caReturn));
	
	 
	 
	 
	 
   lId=0;

  

    pHash = (unsigned char *)pasLHashInit(100, 100, sizeof(struct ncsDsData_s), 0, 4);
    if(pHash == NULL)
    {
        return &caReturn[0];
    }


         psCur = pasDbOpenSqlF("select groupcode from ncsgroup where   groupid in(%s) ",caGroupid);
	 	     if(psCur){
	 	     	iReturn=0;
	 	     	lId=0;
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	 iNum=0;
	 	     	while(iReturn==0||iReturn==1405){
	 	     		lLen=strlen(caGroupcode);
	 	     		psData = (ncsDsData *)pasLHashLookA(pHash, &lLen);
            if(psData)
            {
            	if(psData->count==0){
            		sprintf(psData->codes,"\"%s\"",caGroupcode);
            	}
            	else{
            		sprintf(psData->codes+strlen(psData->codes),",\"%s\"",caGroupcode);
            	}
            	psData->count++;
            }
	 	     		
	 	     		
	 	     		
	 	     		
	 	     			memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	   iReturn=pasDbFetchInto(psCur,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	}
	 	     	 pasDbCloseCursor(psCur);  
	 	     }

	
	 	//  	 sprintf(caReturn,"%s",caOpt);
	 	  	 sprintf(caIds,"%s",caGroupid);
	     iNum=0;
	 	  while(strlen(caIds)>0){
	 	  	memset(caTempid,0,sizeof(caTempid));
	 	  	iNum=0; 
	
	 	     psCur = pasDbOpenSqlF("select groupid,groupcode from ncsgroup where   pid in(%s) ",caIds);
	 	     if(psCur){
	 	     	iReturn=0;
	 	     	lId=0;
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lId,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	while(iReturn==0||iReturn==1405){
	 	     		if(iNum==100) break;
	 	     			
	 	     		lLen=strlen(caGroupcode);
	 	     		psData = (ncsDsData *)pasLHashLookA(pHash, &lLen);
            if(psData)
            {
            	if(psData->count==0){
            		sprintf(psData->codes,"\"%s\"",caGroupcode);
            	}
            	else{
            		sprintf(psData->codes+strlen(psData->codes),",\"%s\"",caGroupcode);
            	}
            	psData->count++;
            }
	 	     			
	 	     		if(iNum==0){
	 	     			sprintf(caTempid,"%lu",lId);
	 	     			
	 	     		}
	 	     		else{
	 	     			sprintf(caTempid+strlen(caTempid),",%lu",lId);
	 	     			
	 	     		}
	 	     			
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lId,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	      			
	 	     			iNum++;
	 	     		
	 	     	}
	 	     }
	 	  	   pasDbCloseCursor(psCur);               
	  	
		
		 	  	strcpy(caIds,caTempid);
	 	  	}
	 	  	
	 	  //拼装SQL语句
	 	  	                       psData=(ncsDsData *)pasLHashFirst(pHash,&sHashInfo);
                               iNum=0;
                               memset(caReturn,0,sizeof(caReturn));
                               while(psData){
                               	if(iNum==0){
                               	  sprintf(caReturn,"( left(%s,%d) in (%s) ",field,psData->len,psData->codes);
                               	}
                               	else{
                               		sprintf(caReturn+strlen(caReturn)," or left(%s,%d) in (%s) ",field,psData->len,psData->codes);
                               	}
                               	iNum++;
                                  
                                psData=(ncsDsData *)pasLHashNext(&sHashInfo);
                               }
	 	  	
	 	  	
	 	  	
	 	  	
	 	  	 if(iNum>0){
		 	  		sprintf(caReturn+strlen(caReturn),")");
		 	   }
	 	 free(pHash);
	 	  return &caReturn[0];
 }



/*判断某场所代码是否在指定的区域代码内
输入场所编码，区域代码 区域代码可以由getDsGroupcode() 或getGroupCodeByGroupid 获得
返回值：0 不在区域 1 在区域
*/

long isInGroupCode(char *scode,char *gcodes){
	long len1;
	int iReturn,lId,iNum;
	char caCode[32];
	 long iRet=0;
	 long lLen=0;
	
   lId=0;
   utStrReplaceWith(gcodes,"\"","");
   strcat(gcodes,",");
   char *pos1,*pos2;
   pos1=gcodes;
   pos2=strstr(pos1,",");
   while(pos2!=NULL){
   	len1=pos2-pos1;
   	if(len1>20) break;
   		memset(caCode,0,sizeof(caCode));
   		memcpy(caCode,pos1,len1);
   		lLen=strlen(caCode);
   //		printf("caCode=%s,scode=%s,len=%d\n",caCode,scode,lLen);
   		if(strncmp(scode,caCode,lLen)==0){
   			iRet=1;
   			break;
   		}
   		pos1=pos2+1;
   		pos2=strstr(pos1,",");
   	}




	 	 
	 	  return iRet;
 }






//加载MAC告警策略库
int ncLoadAcbm_mac(utShmHead *psShmHead)
{
    pasDbCursor  *psCur;
    ncsMacProcyObj *psData;
    int iSumAcbmPat = 0, i;
    int lSumRecord;
    char  caWord[128];
    char caTemp[2024];
    int iReturn, lTid;
    if(psAcbmTree_mac)
    {
        pasAcbmClean(psAcbmTree_mac);
        psAcbmTree_mac = NULL;
    }
    sprintf(caTemp, "select count(*) from (select aa.cid,aa.mac from (select cid,mac from ncscasmac where length(mac)>0 and flags=0 union select cid,b.mac from ncscasmac,nctermmacgroupinfo b where macgroupid=b.groupid) as aa where cid in (select cid from ncscaseinfo where flags=1) group by aa.cid,aa.mac) as mm ");
    iReturn = pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lSumRecord);
    if(iReturn != 0 || lSumRecord == 0)
    {
        return 0;
    }
    psAcbmPat_mac = (pattern_data*)malloc(lSumRecord * sizeof(pattern_data));
    if(psAcbmPat_mac == NULL)
    {
        return (-1);
    }
    memset(psAcbmPat_mac, 0, lSumRecord * sizeof(pattern_data));
    psCur = pasDbOpenSqlF("select aa.cid,aa.mac from (select cid,mac from ncscasmac where length(mac)>0 and flags=0 union select cid,b.mac from ncscasmac,nctermmacgroupinfo b where macgroupid=b.groupid) as aa where cid in (select cid from ncscaseinfo where flags=1) group by aa.cid,aa.mac ");
    if(psCur == NULL)
    {
        free(psAcbmPat_mac);
        return (-1);
    }
    i = 0;
    memset(caWord, 0, sizeof(caWord));
    lTid = 0;
    iReturn = pasDbFetchInto(psCur,
                             UT_TYPE_LONG, 4, &lTid,
                             UT_TYPE_STRING, 64, caWord);
    while(iReturn == 0 || iReturn == 1405)
    {
        if(strlen(caWord) > 3)
        {
            strcpy(psAcbmPat_mac[i].data, convmac(caWord));
            psAcbmPat_mac[i].len = strlen(psAcbmPat_mac[i].data);
            psAcbmPat_mac[i].flags = lTid;
            i++;
        }
        memset(caWord, 0, sizeof(caWord));

        lTid = 0;
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG, 4, &lTid,
                                 UT_TYPE_STRING, 64, caWord);
    }
    iSumAcbmPat = i;


    pasDbCloseCursor(psCur);
    psAcbmTree_mac = (pattern_tree *)pasAcbmInit(psAcbmPat_mac, iSumAcbmPat);
    //加载布控区域
    utShmFreeHash(psShmHead, NCS_LNK_MACPROCYOBJ);
    utShmHashInit(psShmHead, NCS_LNK_MACPROCYOBJ, 2000, 2000, sizeof(ncsMacProcyObj), 0, 32);
    sprintf(caTemp, "select sid,ctype,did from (select sid,9 as ctype,9 as did from ncscaseinfo where flags=1 union select a.sid,ctype,did from ncscaseinfo a,ncscaseobj b where a.sid=b.cid and a.flags=1) as tb");

    unsigned long lSid, lCtype, lId;
    char caCode[36];
    psCur = pasDbOpenSql(caTemp, 0);
    if(psCur)
    {

        lSid = 0;
        lCtype = 0;
        lId = 0;
        iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &lSid,
                                 UT_TYPE_LONG, 4, &lCtype,
                                 UT_TYPE_ULONG, 4, &lId);


        while(iReturn == 0 || iReturn == 1405)
        {
            memset(caCode, 0, sizeof(caCode));
            sprintf(caCode, "%d_%d_%lu", lSid, lCtype, lId);
            psData = (ncsMacProcyObj *)utShmHashLookA(psShmHead, NCS_LNK_MACPROCYOBJ, caCode);
            if(psData)
            {
                psData->id = lId;

            }
            lSid = 0;
            lCtype = 0;
            lId = 0;
            iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &lSid,
                                     UT_TYPE_LONG, 4, &lCtype,
                                     UT_TYPE_ULONG, 4, &lId);
        }


        pasDbCloseCursor(psCur);
    }



    return iSumAcbmPat;
}
unsigned long getServiceIdByCode(utShmHead *psShmHead, char *pCode)
{
    static char dispname[128];
    ncsuserCode2Id *psData;
    ncsClient * psClient;
    unsigned  long lDid;
    lDid = 0;

    psData = (ncsuserCode2Id *)utShmHashLook(psShmHead, NCS_LNK_CLINTCODETOID, pCode);
    if(psData)
    {
        lDid = psData->id;
    }
    return lDid;
}


unsigned long getGroupIdByCode(utShmHead *psShmHead, char *pCode)
{
    static char dispname[128];
    ncsuserCode2Id *psData;
    ncsClient * psClient;
    unsigned  long lDid, lGroupid;
    lDid = 0;
    lGroupid = 0;
    memset(dispname, 0, sizeof(dispname));
    psData = (ncsuserCode2Id *)utShmHashLook(psShmHead, NCS_LNK_CLINTCODETOID, pCode);
    if(psData)
    {
        lDid = psData->id;
        psClient = (ncsClient *)ncsUtlGetClientById(psShmHead, lDid);
        if(psClient)
        {
            lGroupid = psClient->groupid;

        }


    }
    return lGroupid;
}


//启动告警布控进程
int ncsStatMacWarn(utShmHead *psShmHead)
{
    pasDbCursor *psCur;
    long iSummac;
    long iReturn, lTime;
    char caTemp[4096];
    char caTableday[20];
    struct data_s
    {
        char caDispname[64];
        char caMac[20];
        long lStime;
        char caServicecode[16];
        long lCid;
        long long lSid;
    };
    struct data_s *pData;
    char caHost[68], caTitle[132];
    long long lLastid;
    char caMac[24], caVname[68], caServicecode[20];
    long lStime;
    long long lSid;
    long int psMatch[8];
    int iFind, i;
    long iNum;
    char caMesg[256];
    char caReload[32];
    unsigned long lVaild, lCid, lDid, lGroupid;
    char caCode[36];
    ncsMacProcyObj *psData;
    iReturn = pasConnect(psShmHead);
    if(iReturn < 0)
    {
        sleep(60);
        return 0;
    }
    iSummac = ncLoadAcbm_mac(psShmHead);

    // printf("iSummac=%d\n",iSummac);
    while(1)
    {

        strcpy(caReload, utComGetVar_sd(psShmHead, "mpreload", ""));
        if(strcmp(caReload, "reload") == 0)
        {
            ncLoadAcbm_mac(psShmHead);
            printf("reload\n");
            utComSetVar_s(psShmHead, "mpreload", "");
        }

        if(iSummac > 0) //对MAC日志进行过滤
        {
            lTime = time(0);
            strcpy(caTableday, utTimFormat("%Y%m", lTime));
            pData = (struct data_s*)malloc((50000) * sizeof(struct data_s));
            if(pData == NULL)
            {
                sleep(60);
                return (-1);
            }


            ncsUtlGetLastInfo_v4("macWarnSearch", &lLastid);
            sprintf(caTemp, "select mac,servicecode,stime,vname,sid from ncmactermatt_if_%s where sid>%llu order by sid limit 0,50000 ", caTableday, lLastid);
            //        printf("caTemp=%s\n",caTemp);



            psCur = pasDbOpenSql(caTemp, 0);
            if(psCur)
            {

                memset(caMac, 0, sizeof(caMac));
                memset(caServicecode, 0, sizeof(caServicecode));
                lStime = 0;
                memset(caVname, 0, sizeof(caVname));
                lSid = 0;


                lLastid = 0;
                iNum = 0;
                iReturn = pasDbFetchInto(psCur,
                                         UT_TYPE_STRING, 19, caMac,
                                         UT_TYPE_STRING, 14, caServicecode,
                                         UT_TYPE_LONG, 4, &lStime,
                                         UT_TYPE_STRING, 63, caVname,
                                         UT_TYPE_LONG8, 8, &lSid);



                while((iReturn == 0) || (iReturn == 1405))
                {


                    strcpy(pData[iNum].caMac, caMac);
                    strcpy(pData[iNum].caServicecode, caServicecode);
                    pData[iNum].lStime = lStime;
                    strcpy(pData[iNum].caDispname, caVname);
                    pData[iNum].lSid = lSid;

                    iNum++;

                    lLastid = lSid;
                    memset(caMac, 0, sizeof(caMac));
                    memset(caServicecode, 0, sizeof(caServicecode));
                    lStime = 0;
                    memset(caVname, 0, sizeof(caVname));
                    lSid = 0;
                    iReturn = pasDbFetchInto(psCur,
                                             UT_TYPE_STRING, 19, caMac,
                                             UT_TYPE_STRING, 14, caServicecode,
                                             UT_TYPE_LONG, 4, &lStime,
                                             UT_TYPE_STRING, 63, caVname,
                                             UT_TYPE_LONG8, 8, &lSid);

                }
                pasDbCloseCursor(psCur);
                if(lLastid > 0 && iNum > 0)
                {
                    ncsUtlSetLastInfo_v4("macWarnSearch", lLastid);
                }

                for(i = 0; i < iNum; i++)
                {
                    iFind = 0;
                    if(strlen(pData[i].caMac) > 0)
                    {
                        iFind = pasAcbmSearch(psAcbmTree_mac, pData[i].caMac, strlen(pData[i].caMac), psMatch, 1);
                    }

                    if(iFind > 0)
                    {
                        //判断该场所编码是否在布控区域
                        lVaild = 0;
                        lCid = psAcbmPat_mac[psMatch[0]].flags;
                        memset(caServicecode, 0, sizeof(caServicecode));
                        strcpy(caServicecode, pData[i].caServicecode);
                        lDid = getServiceIdByCode(psShmHead, caServicecode);
                        lGroupid = getGroupIdByCode(psShmHead, caServicecode);
                        memset(caCode, 0, sizeof(caCode));
                        //如果这个cid没有添加布控区域，默认全部

                        sprintf(caCode, "%d_9_9", lCid);
                        psData = (ncsMacProcyObj *)utShmHashLook(psShmHead, NCS_LNK_MACPROCYOBJ, caCode);
                        if(!psData)
                        {
                            lVaild = 1;

                            //      printf("111111111111\n");
                        }
                        else
                        {
                            memset(caCode, 0, sizeof(caCode));
                            sprintf(caCode, "%d_0_0", lCid);
                            psData = (ncsMacProcyObj *)utShmHashLook(psShmHead, NCS_LNK_MACPROCYOBJ, caCode);
                            if(psData)
                            {
                                lVaild = 1;
                                //          printf("2222222222222222222222\n");
                            }
                            else
                            {
                                memset(caCode, 0, sizeof(caCode));
                                sprintf(caCode, "%d_2_%lu", lCid, lGroupid);
                                psData = (ncsMacProcyObj *)utShmHashLook(psShmHead, NCS_LNK_MACPROCYOBJ, caCode);
                                if(psData)
                                {
                                    lVaild = 1;
                                }
                                else
                                {
                                    memset(caCode, 0, sizeof(caCode));
                                    sprintf(caCode, "%d_0_%lu", lCid, lDid);
                                    psData = (ncsMacProcyObj *)utShmHashLook(psShmHead, NCS_LNK_MACPROCYOBJ, caCode);
                                    if(psData)
                                    {
                                        lVaild = 1;
                                        //                      printf("33333333333333333333333\n");
                                    }

                                }
                            }
                        }
                        //  printf("lVaild=%d\n",lVaild);
                        if(lVaild == 1)
                        {
                            sprintf(caMesg, "在场所[%s]捕获到MAC【%s】", getServiceNameByCode(psShmHead, pData[i].caServicecode), pData[i].caMac);
                            //                printf("insert into ncscasemacwarnlog(servicecode,udisp,mac,stime,ruleid,msid,cid,message) \
                            //                            values('%s','%s','%s',%d,0,%llu,%d,'%s')\n", \
                            //                            pData[i].caServicecode,pData[i].caDispname,pData[i].caMac,time(0),pData[i].lSid,psAcbmPat_mac[psMatch[0]].flags,caMesg);

                            pasDbExecSqlF("insert into ncscasemacwarnlog(servicecode,udisp,mac,stime,ruleid,msid,cid,message) \
               	        	 values('%s','%s','%s',%d,0,%llu,%d,'%s')", \
                                          pData[i].caServicecode, pData[i].caDispname, pData[i].caMac, time(0), pData[i].lSid, psAcbmPat_mac[psMatch[0]].flags, caMesg);
                        }
                    }



                }


            }
            free(pData);
        }
        sleep(2);
    }
}




//显示MAC类别
int ncsMacGroupComp(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
    char caGroupname[32], caGroupid[16];

    char caName[32];
    char caWhere[256];
    char caSql[256];
    char caCurPg[16], caTotRec[16];
    char caPid[16];
    unsigned long lLevel, lPid;
    int  iNum;
    char caFilter[256];
    char caNo[16];
    long lGroupid, lGroupid0, lCurPg, lTotRec, lRowNum, lStartRec;
    utPltDbHead *psDbHead;
    int iReturn, i;
    char caVar[256], caFname[64], caBfname[64];
    char caQuery[256], caTemp[1024];
    char caValue[256];
    char caVar1[32];
    char caOpt[256];
    long lId;
    char *p, *pVar;
    long lNtype;
#ifdef LDEBUG1
    utMsgPrintMsg(psMsgHead);
#endif


    psCur = pasDbOpenSqlF("select groupname,groupid from nctermmacgroup   order by groupname ");

    if(psCur == NULL)
    {

        return 0;
    }
    psDbHead = utPltInitDb();


    memset(caName, 0, sizeof(caName));
    lNtype = 0;


    iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 31, caName,
                             UT_TYPE_LONG, 4, &lNtype);

    iNum = 1;

    utPltPutLoopVar(psDbHead, "classname", 1, "不限");
    utPltPutLoopVarF(psDbHead, "classid",   1, "%ld", 0);

    while(iReturn == 0)
    {

        if(iNum > 0)
        {
            utPltPutLoopVar(psDbHead, "dh", iNum + 1, ",");
        }

        iNum++;

        utPltPutLoopVar(psDbHead, "classname", iNum, caName);
        utPltPutLoopVarF(psDbHead, "classid",   iNum, "%ld", lNtype);

        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 31, caName,
                                 UT_TYPE_LONG, 4, &lNtype);
    }
    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead, "TotRec", "%d", iNum);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/ncs_nettype_list.htm");
    return 0;
}


//布控MAC策略
int ncsSys_MacProcy(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int lCount = 0;
    char caUpdate[16] = "";
    char caDel[16];
    char caSesid[1024];
    char sql[2024] = "";

    char caPage[12], caLimit[16], caSort[16], caDir[16];
    long lCurPg, lRowNum, lStartRec;

    char   flags[128] = "";
    char caKeyword[64] = "";

    int ireturn = 0;
    int iNum = 0;
    char   caTemp_d[128] = "";
    char   caTemp[1024] = "";

    unsigned long lSid;


    char caSid[20];
    unsigned sids[100];
    long iReturn;
    long i;
    unsigned lGid;
    long lFlags;
    char caCid[20], caMac[24], caGname[64], caGmac[64], caFlags[12];
    utMsgPrintMsg(psMsgHead);
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板

    ireturn = utMsgGetSomeNVar(psMsgHead, 13,
                               "del", UT_TYPE_STRING, 10, caDel,
                               "selsid", UT_TYPE_STRING, 1000, caSesid,
                               "update", UT_TYPE_STRING, 10, caUpdate,
                               "keyword", UT_TYPE_STRING, 64, caKeyword,
                               "filed3", UT_TYPE_STRING, 4, caFlags,
                               "filed1", UT_TYPE_STRING, 10, caSid,
                               "cid", UT_TYPE_STRING, 10, caCid,
                               "filed2", UT_TYPE_STRING, 64, caGname,
                               "filed4", UT_TYPE_STRING, 19, caMac,
                               "page", UT_TYPE_STRING, 10, caPage,
                               "limit", UT_TYPE_STRING, 10, caLimit,
                               "sort", UT_TYPE_STRING, 15, caSort,
                               "dir", UT_TYPE_STRING, 8,  caDir);

    lRowNum = atol(caLimit);
    lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;
    lStartRec = (lCurPg - 1) * lRowNum;


    if(strlen(caGname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caGname, caTemp, 60));
        strcpy(caGname, caTemp_d);
        if(strlen(caSid) > 0)
        {
            sprintf(sql, "select groupid from nctermmacgroup where groupname='%s' ", caGname);
            lGid = 0;
            pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lGid);
            if(lGid > 0) sprintf(caGname, "%lu", lGid);
        }
    }





    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }



    //更新
    if(strlen(caUpdate) > 0)
    {
        if(strlen(caSid) > 0)
        {
            sprintf(sql, "update ncscasmac set macgroupid=%lu,mac='%s',lasttime=%d,flags=%d where sid=%s", atol(caGname), caMac, time(0), atol(caFlags), caSid);
            iReturn = pasDbExecSqlF(sql);
        }
        else
        {
            if(strlen(caMac) > 0 || atol(caGname) > 0)
            {
                sprintf(sql, "insert into ncscasmac (cid,macgroupid,mac,flags,lasttime) \
        	 values(%s,%lu,'%s',%d,%d) ",
                        caCid, atol(caGname), caMac, atol(caFlags), time(0));
                iReturn = pasDbExecSqlF(sql);
            }

        }

        printf("sql = %s\n", sql);

    }



    //删除
    if(strlen(caDel) > 0)
    {
        printf("del start");
        memset(sql, 0, sizeof(sql));
        iReturn = pasDbExecSqlF("delete from ncscasmac where sid in (%s) ", caSesid);


        printf("sql = %s", sql);
    }


    //统计布控记录数
    sprintf(sql, "select count(*) from ncscasmac where cid=%s ", caCid);
    if(strlen(caKeyword) > 0)
    {
        sprintf(sql + strlen(sql), " and (mac like '%c%s%c')", '%', caKeyword, '%');

    }
    pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);

    utPltPutVarF(psDbHead, "TotRec", "%d", lCount);

    sprintf(sql, "select  sid,macgroupid,mac,flags  from ncscasmac  where cid=%s ", caCid);
    if(strlen(caKeyword) > 0)
    {
        sprintf(sql + strlen(sql), " and (mac like '%c%s%c')", '%', caKeyword, '%');
        printf("chaxunsql%s\n", sql);
    }
    sprintf(sql + strlen(sql), " order by sid desc limit %d,%d", lStartRec, lRowNum);
    printf("chaxunsql%s\n", sql);
    //执行sql语句
    psCur = pasDbOpenSql(sql, 0);

    printf("sql%s\n", sql);
    if(psCur)
    {
        lFlags = 0;
        lSid = 0;
        lGid = 0;
        memset(caMac, 0, sizeof(caMac));
        ireturn = pasDbFetchInto(
                      psCur, UT_TYPE_ULONG,  4,      &lSid,
                      UT_TYPE_ULONG, 4, &lGid,
                      UT_TYPE_STRING, 19, caMac,
                      UT_TYPE_ULONG, 4, &lFlags);




        while(0 == ireturn || 1405 == ireturn)
        {
            sids[iNum] = lGid;
            iNum++;
            if(iNum > 1)
            {
                //当循环到第二行，增加一个逗号
                utPltPutLoopVar(psDbHead, "dh", iNum, ","); // utPltPutLoopVar输出一个循环变量的值。
            }
            utPltPutLoopVarF(psDbHead, "sid", iNum, "%lu", lSid);
            utPltPutLoopVar(psDbHead, "mac", iNum, caMac);
            utPltPutLoopVarF(psDbHead, "gid", iNum, "%lu", lGid);
            utPltPutLoopVarF(psDbHead, "flags", iNum, "%lu", lFlags);

            lFlags = 0;
            lSid = 0;
            lGid = 0;
            memset(caMac, 0, sizeof(caMac));
            ireturn = pasDbFetchInto(
                          psCur, UT_TYPE_ULONG,  4,      &lSid,
                          UT_TYPE_ULONG, 4, &lGid,
                          UT_TYPE_STRING, 19, caMac,
                          UT_TYPE_ULONG, 4, &lFlags);

        }
        //关闭游标
        pasDbCloseCursor(psCur);
    }
    for(i = 0; i < iNum; i++)
    {
        //Mac组名
        memset(caGmac, 0, sizeof(caGmac));
        if(sids[i] > 0)
        {
            sprintf(sql, "select groupname from nctermmacgroup where groupid=%lu ", sids[i]);
            //      printf("sql=%s\n",sql);
            pasDbOneRecord(sql, 0, UT_TYPE_STRING, 19, caGmac);
            utPltPutLoopVar(psDbHead, "gname", i + 1, caGmac);

        }
    }



    utPltOutToHtml(iFd, psMsgHead, psDbHead, "/normally/proauth_macprocy.htm");
    return 0;
}


//告警通知人
int ncsSys_WarnNoty(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int lCount = 0;
    char caUpdate[16] = "";
    char caDel[16];
    char caSesid[1024];
    char sql[2024] = "";

    char caPage[12], caLimit[16], caSort[16], caDir[16];
    long lCurPg, lRowNum, lStartRec;

    char   flags[128] = "";
    char caKeyword[64] = "";

    int ireturn = 0;
    int iNum = 0;
    char   caTemp_d[128] = "";
    char   caTemp[1024] = "";

    unsigned long lSid;


    char caSid[20];
    unsigned sids[100];
    long iReturn;
    long i;
    unsigned lGid;
    long lFlags;
    long lWtype;
    char caName[64], caWaddr[68], caWtype[16];
    char caCid[20], caMac[24], caFlags[12];
    utMsgPrintMsg(psMsgHead);
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板

    ireturn = utMsgGetSomeNVar(psMsgHead, 12,
                               "del", UT_TYPE_STRING, 10, caDel,
                               "selsid", UT_TYPE_STRING, 1000, caSesid,
                               "update", UT_TYPE_STRING, 10, caUpdate,
                               "filed3", UT_TYPE_STRING, 63, caWaddr,
                               "filed1", UT_TYPE_STRING, 10, caSid,
                               "cid", UT_TYPE_STRING, 10, caCid,
                               "filed2", UT_TYPE_STRING, 63, caName,
                               "filed4", UT_TYPE_STRING, 10, caWtype,
                               "page", UT_TYPE_STRING, 10, caPage,
                               "limit", UT_TYPE_STRING, 10, caLimit,
                               "sort", UT_TYPE_STRING, 15, caSort,
                               "dir", UT_TYPE_STRING, 8,  caDir);

    lRowNum = atol(caLimit);
    lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;
    lStartRec = (lCurPg - 1) * lRowNum;


    if(strlen(caWtype) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caWtype, caTemp, 10));
        strcpy(caWtype, caTemp_d);
        if(strcmp(caWtype, "邮件") == 0)
        {
            strcpy(caWtype, "1");
        }
        else if(strcmp(caWtype, "短信") == 0)
        {
            strcpy(caWtype, "2");
        }
    }

    if(strlen(caName) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caName, caTemp, 30));
        strcpy(caName, caTemp_d);
    }



    if(strlen(caWaddr) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caWaddr, caTemp, 60));
        strcpy(caWaddr, caTemp_d);
    }



    //更新
    if(strlen(caUpdate) > 0)
    {
        if(strlen(caSid) > 0)
        {
            sprintf(sql, "update ncscasewarn set wtype=%lu,waddr='%s',lasttime=%d,name='%s' where sid=%s", atol(caWtype), caWaddr, time(0), caName, caSid);
            iReturn = pasDbExecSqlF(sql);
        }
        else
        {
            if(strlen(caName) > 0 || atol(caWaddr) > 0)
            {
                sprintf(sql, "insert into ncscasewarn (cid,uid,name,wtype,waddr,lasttime) \
        	 values(%s,0,'%s',%lu,'%s',%d) ",
                        caCid, caName, atol(caWtype), caWaddr, time(0));
                iReturn = pasDbExecSqlF(sql);
            }

        }

        printf("sql = %s\n", sql);

    }



    //删除
    if(strlen(caDel) > 0)
    {

        memset(sql, 0, sizeof(sql));
        iReturn = pasDbExecSqlF("delete from ncscasewarn where sid in (%s) ", caSesid);


        printf("sql = %s", sql);
    }


    //统计布控记录数
    sprintf(sql, "select count(*) from ncscasewarn where cid=%s ", caCid);

    pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);

    utPltPutVarF(psDbHead, "TotRec", "%d", lCount);

    sprintf(sql, "select  sid,name,wtype,waddr  from ncscasewarn  where cid=%s ", caCid);

    printf("chaxunsql%s\n", sql);
    //执行sql语句
    psCur = pasDbOpenSql(sql, 0);

    printf("sql%s\n", sql);
    if(psCur)
    {

        lSid = 0;
        lWtype = 0;
        memset(caName, 0, sizeof(caName));
        memset(caWaddr, 0, sizeof(caWaddr));
        ireturn = pasDbFetchInto(
                      psCur, UT_TYPE_ULONG, 4, &lSid,
                      UT_TYPE_STRING, 31, caName,
                      UT_TYPE_ULONG, 4, &lWtype,
                      UT_TYPE_STRING, 63, caWaddr);





        while(0 == ireturn || 1405 == ireturn)
        {

            iNum++;
            if(iNum > 1)
            {
                //当循环到第二行，增加一个逗号
                utPltPutLoopVar(psDbHead, "dh", iNum, ","); // utPltPutLoopVar输出一个循环变量的值。
            }
            utPltPutLoopVarF(psDbHead, "sid", iNum, "%lu", lSid);
            utPltPutLoopVar(psDbHead, "name", iNum, caName);
            utPltPutLoopVarF(psDbHead, "wtype", iNum, "%lu", lWtype);
            utPltPutLoopVar(psDbHead, "waddr", iNum, caWaddr);

            lSid = 0;
            lWtype = 0;
            memset(caName, 0, sizeof(caName));
            memset(caWaddr, 0, sizeof(caWaddr));
            ireturn = pasDbFetchInto(
                          psCur, UT_TYPE_ULONG, 4, &lSid,
                          UT_TYPE_STRING, 31, caName,
                          UT_TYPE_ULONG, 4, &lWtype,
                          UT_TYPE_STRING, 63, caWaddr);

        }
        //关闭游标
        pasDbCloseCursor(psCur);
    }


    utPltOutToHtml(iFd, psMsgHead, psDbHead, "/normally/proauth_warnnotfy.htm");
    return 0;
}

//显示告警联系人
int ncsMacWarnmanComp(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
    char caGroupname[32], caGroupid[16];

    char caWhere[256];
    char caSql[256];
    char caCurPg[16], caTotRec[16];
    char caPid[16];
    unsigned long lLevel, lPid;
    int  iNum;
    char caFilter[256];
    char caNo[16];
    long lGroupid, lGroupid0, lCurPg, lTotRec, lRowNum, lStartRec;
    utPltDbHead *psDbHead;
    int iReturn, i;
    char caVar[256], caFname[64], caBfname[64];
    char caQuery[256], caTemp[1024];
    char caName[64], caPhone[24], caEmail[64];
#ifdef LDEBUG1
    utMsgPrintMsg(psMsgHead);
#endif

    printf("select name,phone,email from stafinfo   group by name,phone,email \n");
    psCur = pasDbOpenSqlF("select name,phone,email from stafinfo   group by name,phone,email ");
    iNum = 0;
    if(psCur == NULL)
    {

        return 0;
    }
    psDbHead = utPltInitDb();


    memset(caName, 0, sizeof(caName));
    memset(caPhone, 0, sizeof(caPhone));
    memset(caEmail, 0, sizeof(caEmail));


    iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 15, caName,
                             UT_TYPE_STRING, 63, caPhone,
                             UT_TYPE_STRING, 63, caEmail);


    iNum = 0;


    while(iReturn == 0 || iReturn == 1405)
    {

        if(iNum > 0)
        {
            utPltPutLoopVar(psDbHead, "dh", iNum + 1, ",");
        }

        iNum++;

        utPltPutLoopVar(psDbHead, "nname", iNum, caName);
        utPltPutLoopVar(psDbHead, "phone", iNum, caPhone);
        utPltPutLoopVar(psDbHead, "email", iNum, caEmail);

        memset(caName, 0, sizeof(caName));
        memset(caPhone, 0, sizeof(caPhone));
        memset(caEmail, 0, sizeof(caEmail));


        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 15, caName,
                                 UT_TYPE_STRING, 63, caPhone,
                                 UT_TYPE_STRING, 63, caEmail);
    }
    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead, "TotRec", "%d", iNum);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "normally/proauth_warnman.htm");

    return 0;
}


//显示终端阀值布控
int ncsSys_WarnMacLim(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int lCount = 0;
    char caUpdate[16] = "";
    char caDel[16];
    char caSesid[1024];
    char sql[2024] = "";

    char caPage[12], caLimit[16], caSort[16], caDir[16];
    long lCurPg, lRowNum, lStartRec;

    char   flags[128] = "";
    char caKeyword[64] = "";

    int ireturn = 0;
    int iNum = 0;
    char   caTemp_d[128] = "";
    char   caTemp[1024] = "";

    unsigned long lSid;


    char caSid[20];
    unsigned sids[100];
    long iReturn;
    long i;

    long lFlags;
    char caCid[20], caMac[24], caFlags[12];
    char caDateid[32], caLevel[32], caCountlimit[32];
    unsigned long lLevel, lDateid, lCountlimit;
    char caApname[32];
    utMsgPrintMsg(psMsgHead);
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板

    ireturn = utMsgGetSomeNVar(psMsgHead, 14,
                               "del", UT_TYPE_STRING, 10, caDel,
                               "selsid", UT_TYPE_STRING, 1000, caSesid,
                               "update", UT_TYPE_STRING, 10, caUpdate,
                               "filed3", UT_TYPE_STRING, 10, caDateid,
                               "filed1", UT_TYPE_STRING, 10, caSid,
                               "cid", UT_TYPE_STRING, 10, caCid,
                               "filed2", UT_TYPE_STRING, 10, caLevel,
                               "filed4", UT_TYPE_STRING, 10, caCountlimit,
                               "filed5", UT_TYPE_STRING, 30, caApname,
                               "flags", UT_TYPE_STRING, 10, caFlags,
                               "page", UT_TYPE_STRING, 10, caPage,
                               "limit", UT_TYPE_STRING, 10, caLimit,
                               "sort", UT_TYPE_STRING, 15, caSort,
                               "dir", UT_TYPE_STRING, 8,  caDir);

    lRowNum = atol(caLimit);
    lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;
    lStartRec = (lCurPg - 1) * lRowNum;


    if(strlen(caLevel) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caLevel, caTemp, 10));
        strcpy(caLevel, caTemp_d);
        if(strcmp(caLevel, "一般") == 0)
        {
            strcpy(caLevel, "1");
        }
        else if(strcmp(caLevel, "中度") == 0)
        {
            strcpy(caLevel, "2");
        }
        else if(strcmp(caLevel, "严重") == 0)
        {
            strcpy(caLevel, "3");
        }
    }


    if(strlen(caDateid) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caDateid, caTemp, 10));
        strcpy(caDateid, caTemp_d);
        if(strcmp(caDateid, "每分钟") == 0)
        {
            strcpy(caDateid, "1");
        }
        else if(strcmp(caDateid, "每小时") == 0)
        {
            strcpy(caDateid, "2");
        }
        else if(strcmp(caDateid, "每天") == 0)
        {
            strcpy(caDateid, "3");
        }
    }
    if(strlen(caFlags) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caFlags, caTemp, 10));
        strcpy(caFlags, caTemp_d);
        if(strcmp(caFlags, "启用") == 0)
        {
            strcpy(caFlags, "0");
        }
        else if(strcmp(caFlags, "停用") == 0)
        {
            strcpy(caFlags, "1");
        }

    }


    if(strlen(caApname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caApname, caTemp, 30));
        strcpy(caApname, caTemp_d);
    }






    //更新
    if(strlen(caUpdate) > 0)
    {
        if(strlen(caSid) > 0)
        {
            sprintf(sql, "update ncscasetermlim set level=%lu,dateid=%lu,countlimit=%lu,flags=%lu,apname='%s',lasttime=%d where sid=%s", \
                    atol(caLevel), atol(caDateid), atol(caCountlimit), atol(caFlags), caApname, time(0), caSid);
            iReturn = pasDbExecSqlF(sql);
        }
        else
        {
            if(strlen(caCountlimit) > 0)
            {
                sprintf(sql, "insert into ncscasetermlim (cid,level,dateid,countlimit,apname,flags,lasttime) \
        	 values(%s,%d,'%d',%lu,'%s',%d,%d) ",
                        caCid, atol(caLevel), atol(caDateid), atol(caCountlimit), caApname, atol(caFlags), time(0));
                iReturn = pasDbExecSqlF(sql);
            }

        }

        printf("sql = %s\n", sql);

    }



    //删除
    if(strlen(caDel) > 0)
    {

        memset(sql, 0, sizeof(sql));
        iReturn = pasDbExecSqlF("delete from ncscasetermlim where sid in (%s) ", caSesid);


        printf("sql = %s", sql);
    }


    //统计布控记录数
    sprintf(sql, "select count(*) from ncscasetermlim where cid=%s ", caCid);

    pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);

    utPltPutVarF(psDbHead, "TotRec", "%d", lCount);

    sprintf(sql, "select  sid,level,dateid,countlimit,apname,flags  from ncscasetermlim  where cid=%s ", caCid);

    printf("chaxunsql%s\n", sql);
    //执行sql语句
    psCur = pasDbOpenSql(sql, 0);

    printf("sql%s\n", sql);
    if(psCur)
    {

        lSid = 0;
        lLevel = 0;
        lDateid = 0;
        lCountlimit = 0;
        memset(caApname, 0, sizeof(caApname));
        lFlags = 0;
        ireturn = pasDbFetchInto(
                      psCur, UT_TYPE_ULONG, 4, &lSid,
                      UT_TYPE_ULONG, 4, &lLevel,
                      UT_TYPE_ULONG, 4, &lDateid,
                      UT_TYPE_ULONG, 4, &lCountlimit,
                      UT_TYPE_STRING, 24, caApname,
                      UT_TYPE_ULONG, 4, &lFlags);





        while(0 == ireturn || 1405 == ireturn)
        {

            iNum++;
            if(iNum > 1)
            {
                //当循环到第二行，增加一个逗号
                utPltPutLoopVar(psDbHead, "dh", iNum, ","); // utPltPutLoopVar输出一个循环变量的值。
            }
            utPltPutLoopVarF(psDbHead, "sid", iNum, "%lu", lSid);
            utPltPutLoopVarF(psDbHead, "level", iNum, "%lu", lLevel);
            utPltPutLoopVarF(psDbHead, "dateid", iNum, "%lu", lDateid);
            utPltPutLoopVarF(psDbHead, "countlimit", iNum, "%lu", lCountlimit);
            utPltPutLoopVar(psDbHead, "apname", iNum, caApname);
            utPltPutLoopVarF(psDbHead, "flags", iNum, "%d", lFlags);


            lSid = 0;
            lLevel = 0;
            lDateid = 0;
            lCountlimit = 0;
            memset(caApname, 0, sizeof(caApname));
            lFlags = 0;
            ireturn = pasDbFetchInto(
                          psCur, UT_TYPE_ULONG, 4, &lSid,
                          UT_TYPE_ULONG, 4, &lLevel,
                          UT_TYPE_ULONG, 4, &lDateid,
                          UT_TYPE_ULONG, 4, &lCountlimit,
                          UT_TYPE_STRING, 24, caApname,
                          UT_TYPE_ULONG, 4, &lFlags);

        }
        //关闭游标
        pasDbCloseCursor(psCur);
    }


    utPltOutToHtml(iFd, psMsgHead, psDbHead, "/normally/proauth_warnnotfy.htm");
    return 0;
}


// 显示虚拟身份信息日志
int ncsNetidloglist_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
    long lCurPg, lRowNum, lStartRec, lSstarttime;
    char sql[40960] = "";
    char caTotPg[16], caCurPg[16];
    char caPage[12], caLimit[16], caSort[16], caDir[16];
    char caKeyword[64], caTime_flag[32];
    char sql2[256] = "";
    char frmfile[256] = "";
    char *tmpgbk = NULL;
    char **ptmp = NULL;

    struct sttable
    {
        char tname[32];
        unsigned long lCount;
    };
    struct sttable tableinfo[120];//10年？
    pasDbCursor *psCur = NULL;
    char caGroups[1024];
    char caFacs[1024];
    char caArea[256], caName2[256];
    char caTemp[1024];
    long iReturn;
    unsigned long lIp;
    char caStime[16];
    char caEtime[16];
    char caTemp_d[64];
    char caGroupid[36], caDid[36];
    char caSdate[24], caEdate[24];
    char caApmac[36], caMac[36], caApname[64], caPosition[64];
    char caCorpname[128];
    char caCont[2048];
    char caServicecode[16], caSsid[36], caSearchtype[32];
    strcpy(caFacs, getDsFacDid());
    strcpy(caGroups, getDsGroupid());
    struct ncsNettype_s *psData;
    utPltDbHead *psDbHead = utPltInitDb();
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 21,
                               "TotPg",      UT_TYPE_STRING, 12, caTotPg,
                               "CurPg",      UT_TYPE_STRING, 12, caCurPg,
                               "keyword", UT_TYPE_STRING, 60, caKeyword,
                               "page",            UT_TYPE_STRING, 10, caPage,
                               "limit",           UT_TYPE_STRING, 10, caLimit,
                               "sort",            UT_TYPE_STRING, 15, caSort,
                               "dir",          UT_TYPE_STRING, 8,  caDir,
                               "time_flag",    UT_TYPE_STRING, 8, caTime_flag,
                               "did",          UT_TYPE_STRING, 10, caDid,
                               "groupid",      UT_TYPE_STRING, 10, caGroupid,
                               "sdate",        UT_TYPE_STRING, 12, sdate,
                               "stime",        UT_TYPE_STRING, 10, caStime,
                               "edate",        UT_TYPE_STRING, 12, edate,
                               "etime",        UT_TYPE_STRING, 10, caEtime,
                               "apmac",        UT_TYPE_STRING, 20, caApmac,
                               "mac",          UT_TYPE_STRING, 20, caMac,
                               "apname",       UT_TYPE_STRING, 31, caApname,
                               "servicecode",     UT_TYPE_STRING, 15, caServicecode,
                               "servicename",     UT_TYPE_STRING, 60, caCorpname,
                               "ssid",            UT_TYPE_STRING, 30, caSsid,
                               "searchtype",      UT_TYPE_STRING, 10, caSearchtype);

    if(strlen(caSearchtype) > 0)
    {
        strcpy(caKeyword, "");
    }
    else
    {
        strcpy(sdate, "");
        strcpy(edate, "");
    }
    memset(caCont, 0, sizeof(caCont));


    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }
    if(strlen(caCorpname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caCorpname, caTemp, 60));
        strcpy(caCorpname, caTemp_d);
    }

    if(strlen(caApname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caApname, caTemp, 60));
        strcpy(caApname, caTemp_d);
    }



    lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;


    if(strlen(caLimit) > 0)
    {
        lRowNum = atol(caLimit);
    }
    else
    {
        lRowNum = 40; //每一页面行数
    }
    lStartRec = (lCurPg - 1) * lRowNum;

    iTime_flag = atoi(caTime_flag);
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
        ptmp = ncsUtlGetTable3(sdate, edate, caStime, caEtime, "nctermnetlog_if_", &lStartTime, &lTime, &lCount);
        printf("lStartTime=%d,sdate=%s\n", lStartTime, sdate);
    }
    else
    {
        ptmp = ncsUtlGetTable(lTime, days, "nctermnetlog_if_",  &lStartTime, &lCount);
        lTime = lTime + 2 * 3600;
    }

    memset(caCont, 0, sizeof(caCont));
    if(strlen(caSearchtype) > 0)
    {
        if(strlen(caMac) > 0)
        {
            sprintf(caCont + strlen(caCont), " and mac like '%c%s%c' ", '%', caMac, '%');
        }

        if(strlen(caApname) > 0)
        {
            sprintf(caCont + strlen(caCont), " and apname like '%c%s%c' ", '%', caApname, '%');
        }
        if(strlen(caServicecode) > 0)
        {
            sprintf(caCont + strlen(caCont), " and servicecode like '%c%s%c' ", '%', caServicecode, '%');
        }

    }
    else
    {
        if(strlen(caKeyword) > 0)
        {
            sprintf(caCont, " and (mac like '%c%s%c' or vname like '%c%s%c' ) ", '%', caKeyword, '%', '%', caKeyword, '%');
        }
    }
    if(strlen(caDid) > 0)
    {

        sprintf(caCont + strlen(caCont), " and servicecode='%s' ", getServicecodeByid(psShmHead, atol(caDid)));
    }
    if(strlen(caGroupid) > 0)
    {
        sprintf(caCont + strlen(caCont), " and servicecode in (%s)", getServicecodeByGroupid(psShmHead, caGroupid));
    }
    if(strlen(caGroups) > 0)
    {
        sprintf(caCont + strlen(caCont), " and servicecode in (%s)", getServicecodeByGroupid(psShmHead, caGroups));
    }




    for(i = 0; i < lCount; i++)
    {

        lCount1 = 0;
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu  ",  ptmp[i], lStartTime, lTime);
        strcat(sql, caCont);

        printf("sql = %s\n", sql);
        pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
        if(lCount1 > 0)
        {
            strcpy(tableinfo[num].tname, ptmp[i]);
            tableinfo[num].lCount = lCount1;
            num++;
        }
        //  }
        free(ptmp[i]);
    }
    free(ptmp);
    for(i = 0; i < num; i++)
    {
        lTotRec += tableinfo[i].lCount;
    }
    utPltPutVarF(psDbHead, "TotRec", "%lu", lTotRec);

    //
    for(i = num - 1; i >= 0; i--)
    {
        printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
    }
    //

    iNum = 0;
    iStart = lStartRec;
    iLimit = lRowNum;

    for(i = num - 1; i >= 0; i--)
    {
        memset(sql, 0, sizeof(sql));
        if(tableinfo[i].lCount <= iStart)
        {
            iStart -= tableinfo[i].lCount;
            continue;
        }
        snprintf(sql, sizeof(sql), "select servicecode,username,mac,apname,stime,longitude,latitude,nettype,vname,inet_ntoa(sip),inet_ntoa(dip) from %s where stime >= %lu and stime <= %lu  ", tableinfo[i].tname, lStartTime, lTime);
        strcat(sql, caCont);

        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " order by stime desc limit %lu, %lu", iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);


        if(tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart + iLimit)
        {
            iLimit = iLimit - (tableinfo[i].lCount - iStart);
            iStart = 0;
        }
        else
        {
            _break = 1;
        }

        printf("__line = %d, sql = %s\n", __LINE__, sql);
        psCur = pasDbOpenSql(sql, 0);
        if(psCur != NULL)
        {
            char caServicecode[16], caServicename[64], caAddress[64], caMac[20], caApname[32], caApmac[20], caSsid[64];
            long lStime, lNettype, lEndtime, lFlags;
            char caSip[20], caDip[20], caLongitude[16], caLatitude[16], caType[20];
            char caTermtype[128], caVname[64], caUsername[64];
            long lVtype;
            memset(caServicecode, 0, sizeof(caServicecode));
            memset(caMac, 0, sizeof(caMac));
            memset(caApname, 0, sizeof(caApname));
            lStime = 0;
            memset(caLongitude, 0, sizeof(caLongitude));
            memset(caLatitude, 0, sizeof(caLatitude));
            lNettype = 0;
            memset(caVname, 0, sizeof(caVname));
            memset(caSip, 0, sizeof(caSip));
            memset(caDip, 0, sizeof(caDip));
            iret = pasDbFetchInto(psCur,
                                  UT_TYPE_STRING, 14, caServicecode,
                                  UT_TYPE_STRING, 63, caUsername,
                                  UT_TYPE_STRING, 18, caMac,
                                  UT_TYPE_STRING, 31, caApname,
                                  UT_TYPE_LONG, 4, &lStime,
                                  UT_TYPE_STRING, 11, caLongitude,
                                  UT_TYPE_STRING, 11, caLatitude,
                                  UT_TYPE_LONG, 4, &lNettype,
                                  UT_TYPE_STRING, 63, caVname,
                                  UT_TYPE_STRING, 15, caSip,
                                  UT_TYPE_STRING, 15, caDip);


            while((iret == 0) || (iret == 1405))
            {
                iNum++;
                if(iNum > 1)
                {
                    utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                }

                utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%y/%m/%d %H:%M", lStime));

                utPltPutLoopVar(psDbHead, "username", iNum, caUsername);
                utPltPutLoopVar(psDbHead, "apname", iNum, caApname);
                utPltPutLoopVar(psDbHead, "mac", iNum, caMac);
                utPltPutLoopVar(psDbHead, "apmac", iNum, caApmac);

                utPltPutLoopVar(psDbHead, "longitude", iNum, caLongitude);
                utPltPutLoopVar(psDbHead, "latitude", iNum, caLatitude);

                utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);

                utPltPutLoopVar(psDbHead, "vname", iNum, caVname);
                utPltPutLoopVar(psDbHead, "servicename", iNum, getServiceNameByCode(psShmHead, caServicecode));

                psData = (struct ncsNettype_s *)pasLHashLook(pHashNettype, &lNettype);
                if(psData)
                {
                    strcpy(caType, psData->caName);
                }
                else
                {
                    strcpy(caType, "");
                }
                utPltPutLoopVar(psDbHead, "vtype", iNum, caType);
                utPltPutLoopVar(psDbHead, "sip", iNum, caSip);
                utPltPutLoopVar(psDbHead, "dip", iNum, caDip);
                memset(caServicecode, 0, sizeof(caServicecode));
                memset(caMac, 0, sizeof(caMac));
                memset(caApname, 0, sizeof(caApname));
                lStime = 0;
                memset(caLongitude, 0, sizeof(caLongitude));
                memset(caLatitude, 0, sizeof(caLatitude));
                lNettype = 0;
                memset(caVname, 0, sizeof(caVname));
                memset(caSip, 0, sizeof(caSip));
                memset(caDip, 0, sizeof(caDip));
                iret = pasDbFetchInto(psCur,
                                      UT_TYPE_STRING, 14, caServicecode,
                                      UT_TYPE_STRING, 63, caUsername,
                                      UT_TYPE_STRING, 18, caMac,
                                      UT_TYPE_STRING, 31, caApname,
                                      UT_TYPE_LONG, 4, &lStime,
                                      UT_TYPE_STRING, 11, caLongitude,
                                      UT_TYPE_STRING, 11, caLatitude,
                                      UT_TYPE_LONG, 4, &lNettype,
                                      UT_TYPE_STRING, 63, caVname,
                                      UT_TYPE_STRING, 15, caSip,
                                      UT_TYPE_STRING, 15, caDip);
            }
            pasDbCloseCursor(psCur);
        }

        if(_break == 1)
            break;
    }



    utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_netidlog_list.htm");
    return 0;
}
//布控策略树
int ncsTreeBkProcy_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caTemp[4024];
    char sqlbuf[1024];
    long lFlag, lKid;
    int  iNum;
    utPltDbHead *psDbHead;
    int iReturn, i;
    long lCount;
    char caName[64];
    long lId, lSid, lSum;
    char caPlate[128];
    char caId[16];
    char caDel[16], caUpdate[16], caSelsid[1024], caKeyname[64], caSid[16], caTemp_d[64];
    char *pHash;
    char caFlag[16];
    long lDid, lUid;
    char caDispname[32];
    char caUsername2[36];
    char caGroups[2048];
    char caKeyword[64];
    unsigned long lCid;
    char caCasename[68];
#ifdef LDEBUG1
    utMsgPrintMsg(psMsgHead);
#endif
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 1, "cname",           UT_TYPE_STRING, 63, caKeyword);


    psDbHead = utPltInitDb();

    sprintf(caTemp, "select count(*) aa,a.cid,b.casename from ncscasemacwarnlog a,ncscaseinfo b where a.cid=b.sid ");
    if(strlen(caKeyword) > 0)
    {
        sprintf(caTemp + strlen(caTemp), " and b.casename like '%c%s%c' ", '%', caKeyword, '%');
    }
    sprintf(caTemp + strlen(caTemp), " group by a.cid,b.casename order by a.cid desc limit 0,400 ");
    iNum = 0;
    printf("caTemp=%s\n", caTemp);
    psCur = pasDbOpenSql(caTemp, 0);
    if(psCur)
    {
        lCount = 0;
        lCid = 0;
        memset(caCasename, 0, sizeof(caCasename));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG, 4, &lCount,
                                 UT_TYPE_LONG, 4, &lCid,
                                 UT_TYPE_STRING, 63, caCasename);


        while(iReturn == 0 || iReturn == 1405)
        {
            if(iNum > 0)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum + 1, ",");
            }
            utPltPutLoopVar(psDbHead, "leaf", iNum + 1, "true");
            utPltPutLoopVar(psDbHead, "cls", iNum + 1, "file");
            utPltPutLoopVarF(psDbHead, "count", iNum + 1, "%d", lCount);
            utPltPutLoopVarF(psDbHead, "uid", iNum + 1, "%d", lCid);

            utPltSetCvtHtml(1);
            utPltPutLoopVar(psDbHead, "dispname", iNum + 1, caCasename);
            utPltSetCvtHtml(0);
            iNum++;
            lCount = 0;
            lCid = 0;
            memset(caCasename, 0, sizeof(caCasename));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG, 4, &lCount,
                                     UT_TYPE_LONG, 4, &lCid,
                                     UT_TYPE_STRING, 63, caCasename);
        }
        pasDbCloseCursor(psCur);

        //       printf("aaaaaa\n");
    }



    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/ncs_userkeyword_tree.htm");



    return 0;
}


int ncsSys_controlmanage2(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int lCount = 0;
    char caUpdate[16] = "";
    char caDel[16];
    char caSesid[1024];
    char sql[2024] = "";
    char  sid[128] = "";
    char caPage[12], caLimit[16], caSort[16], caDir[16];
    long lCurPg, lRowNum, lStartRec;
    char casecode[128] = "";
    char casename[128] = "";
    char  casetype[128] = "";
    char caseclass[128] = "";
    char groupid[128] = "";
    char casedept[128] = "";
    char casedate[128] = "";
    char addby[128] = "";
    char cmemo[128] = "";
    char  lasttime[128] = "";
    char  expiretime[128] = "";
    char   flags[128] = "";
    char caKeyword[64] = "";
    char caObjids[1024];
    unsigned long lExpiretime, lLasttime, lFlags, lCasetype, lGroupid, lCaseclass, lCasedate;
    //用于判断游标状态
    int ireturn = 0;
    //用于判断行数
    int iNum = 0;
    char   caTemp_d[128] = "";
    char   caTemp[1024] = "";


    unsigned long lSid;

    char wcount[128] = "";
    char wdate[128] = "";
    char insert[128] = "";
    char caSid[20];
    unsigned sids[100];
    char caApp[16], caSetflag[16];
    long iReturn;
    long i;
    utMsgPrintMsg(psMsgHead);
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板

    ireturn = utMsgGetSomeNVar(psMsgHead, 22,
                               "del", UT_TYPE_STRING, 10, caDel,
                               "selsid", UT_TYPE_STRING, 1000, caSesid,
                               "update", UT_TYPE_STRING, 10, caUpdate,
                               "keyword", UT_TYPE_STRING, 64, caKeyword,
                               "sid", UT_TYPE_STRING, 64, sid,
                               "flags", UT_TYPE_STRING, 64, flags,
                               "filed1", UT_TYPE_STRING, 64, casename,
                               "casedate", UT_TYPE_STRING, 64, casedate,
                               "filed2", UT_TYPE_STRING, 64, expiretime,
                               "groupid", UT_TYPE_STRING, 64, groupid,
                               "filed4", UT_TYPE_STRING, 64, cmemo,
                               "filed5", UT_TYPE_STRING, 1000, caObjids,
                               "filed6", UT_TYPE_STRING, 10, caSid,
                               "wcount", UT_TYPE_STRING, 64, wcount,
                               "wdate", UT_TYPE_STRING, 64, wdate,
                               "insert", UT_TYPE_STRING, 64, insert,
                               "page", UT_TYPE_STRING, 10, caPage,
                               "limit", UT_TYPE_STRING, 10, caLimit,
                               "sort", UT_TYPE_STRING, 15, caSort,
                               "dir", UT_TYPE_STRING, 8,  caDir,
                               "app", UT_TYPE_STRING, 10, caApp,
                               "setflag", UT_TYPE_STRING, 10, caSetflag);

    lRowNum = atol(caLimit);
    lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;
    lStartRec = (lCurPg - 1) * lRowNum;

    if(strlen(sid) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, sid, caTemp, 60));
        strcpy(sid, caTemp_d);
    }
    if(strlen(flags) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, flags, caTemp, 60));
        strcpy(flags, caTemp_d);
    }
    if(strlen(casename) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, casename, caTemp, 60));
        strcpy(casename, caTemp_d);
    }
    if(strlen(casedate) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, casedate, caTemp, 60));
        strcpy(casedate, caTemp_d);
    }
    if(strlen(groupid) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, groupid, caTemp, 60));
        strcpy(groupid, caTemp_d);
    }
    if(strlen(cmemo) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, cmemo, caTemp, 60));
        strcpy(cmemo, caTemp_d);
    }
    if(strlen(wcount) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, wcount, caTemp, 60));
        strcpy(wcount, caTemp_d);
    }
    if(strlen(wdate) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, wdate, caTemp, 60));
        strcpy(wdate, caTemp_d);
    }
    if(strlen(expiretime) > 0)
    {
        sprintf(caTemp_d, "%s 23:59:59", expiretime);
        strcpy(expiretime, caTemp_d);
    }
    if(strlen(insert) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, insert, caTemp, 60));
        strcpy(insert, caTemp_d);
    }



    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }

    //应用 在共享内存中设置重新加载布控策略标志
    if(strlen(caApp) > 0)
    {
        utComSetVar_s(psShmHead, "mpreload", "reload");
    }
    if(strlen(caSetflag) > 0)
    {
        if(strcmp(caSetflag, "1") == 0)
        {
            pasDbExecSqlF("update ncscaseinfo set flags=1 where sid in (%s) ", caSesid);
        }
        else if(strcmp(caSetflag, "9") == 0)
        {
            pasDbExecSqlF("update ncscaseinfo set flags=9 where sid in (%s) ", caSesid);
        }
    }
    //更新
    if(strlen(caUpdate) > 0)
    {
        if(strlen(caSid) > 0)
        {
            sprintf(sql, "update ncscaseinfo set casename='%s',cmemo='%s',lasttime=%d,expiretime=%d where sid=%s", casename, cmemo, time(0), utTimStrToLong("%Y/%m/%d %H:%M:%S", expiretime), caSid);
            iReturn = pasDbExecSqlF(sql);
        }
        else
        {
            char caCasecode[32];
            strcpy(caCasecode, utTimFormat("%Y%m%d%H%M%S", time(0)));
            sprintf(sql, "insert into ncscaseinfo (casecode,casename,casetype,caseclass,groupid,casedept,casedate,addby,cmemo,lasttime,expiretime,flags) \
                 values('%s','%s',0,0,0,'',%d,'','%s',%d,%d,1) ",
                    caCasecode, casename, time(0), cmemo, time(0), utTimStrToLong("%Y/%m/%d %H:%M:%S", expiretime));
            iReturn = pasDbExecSqlF(sql);
            lSid = 0;
            pasDbOneRecord("select max(sid) from ncscaseinfo ", 0, UT_TYPE_ULONG, 4, &lSid);
            sprintf(caSid, "%lu", lSid);
        }

        printf("sql = %s\n", sql);
        if(iReturn == 0)
        {
            //添加布控区域
            char caOut[200][256];
            char caType[12], caId[20];
            pasDbExecSqlF("delete from ncscaseobj where cid=%s ", caSid);
            if(strstr(caObjids, "all"))
            {
                pasDbExecSqlF("insert into ncscaseobj(cid,ctype,did,lasttime,flags) values(%s,0,0,%d,0) ", caSid, time(0));
            }
            else
            {
                memset(caOut, 0, 200 * 256);
                lCount = sepcharbydh(caObjids, caOut);
                for(i = 0; i < lCount; i++)
                {
                    utStrGetSomeWordBy(caOut[i], "_", 2, UT_TYPE_STRING, 3, caType,
                                       UT_TYPE_STRING, 10, caId);
                    printf("caType=%s,caId=%s\n", caType, caId);
                    if(strlen(caId) > 0)
                    {
                        printf("insert into ncscaseobj(cid,ctype,did,lasttime,flags) values(%s,%d,%s,%d,0) \n",
                               caSid, atol(caType), caId, time(0));
                        pasDbExecSqlF("insert into ncscaseobj(cid,ctype,did,lasttime,flags) values(%s,%d,%s,%d,0) ",
                                      caSid, atol(caType), caId, time(0));
                    }
                }
            }
        }
    }



    //删除
    if(strlen(caDel) > 0)
    {
        printf("del start");
        memset(sql, 0, sizeof(sql));
        iReturn = pasDbExecSqlF("delete from ncscaseobj where cid in (%s) ", caSesid);
        ireturn = pasDbExecSqlF("delete from ncscasmac where cid in(%s)", caSesid);
        ireturn = pasDbExecSqlF("delete from ncscasetermlim where cid in(%s)", caSesid);
        ireturn = pasDbExecSqlF("delete from ncscasewarn where cid in(%s)", caSesid);
        //删除告警信息
        ireturn = pasDbExecSqlF("delete from ncscasemacwarnlog where cid in(%s)", caSesid);
        ireturn = pasDbExecSqlF("delete from ncscaseinfo where sid in(%s)", caSesid);

        ;
    }


    //统计布控记录数
    sprintf(sql, "select count(*) from ncscaseinfo where flags!=8 ");
    if(strlen(caKeyword) > 0)
    {
        sprintf(sql + strlen(sql), " and (casename like '%c%s%c')", '%', caKeyword, '%');

    }
    pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);

    utPltPutVarF(psDbHead, "TotRec", "%d", lCount);

    sprintf(sql, "select  sid,casecode,casename,casetype,caseclass,groupid,casedept,casedate,addby,cmemo,lasttime,expiretime,flags  from ncscaseinfo  where flags!=8");
    if(strlen(caKeyword) > 0)
    {
        sprintf(sql + strlen(sql), " and (casename like '%c%s%c')", '%', caKeyword, '%');
        printf("chaxunsql%s\n", sql);
    }
    sprintf(sql + strlen(sql), " order by %s %s limit %d,%d", caSort, caDir, lStartRec, lRowNum);
    printf("chaxunsql%s\n", sql);
    //执行sql语句
    psCur = pasDbOpenSql(sql, 0);

    printf("sql%s\n", sql);
    lExpiretime = 0;
    lLasttime = 0;
    lFlags = 0;
    lSid = 0;
    lCasetype = 0;
    lGroupid = 0;
    lCaseclass = 0;
    ireturn = pasDbFetchInto(
                  psCur, UT_TYPE_ULONG,  4,      &lSid,
                  UT_TYPE_STRING,  sizeof(casecode) - 1,      casecode,
                  UT_TYPE_STRING,  sizeof(casename) - 1,      casename,
                  UT_TYPE_ULONG,  4,     &lCasetype,
                  UT_TYPE_ULONG,  4,    &lCaseclass,
                  UT_TYPE_ULONG, 4,  &lGroupid,
                  UT_TYPE_STRING,  sizeof(casedept) - 1,   casedept,
                  UT_TYPE_ULONG,  4,  &lCasedate,
                  UT_TYPE_STRING,  sizeof(addby) - 1,  addby,
                  UT_TYPE_STRING,  sizeof(cmemo) - 1,  cmemo,
                  UT_TYPE_ULONG,  4, &lLasttime,
                  UT_TYPE_ULONG,  4, &lExpiretime,
                  UT_TYPE_ULONG, 4, &lFlags);




    while(0 == ireturn || 1405 == ireturn)
    {
        sids[iNum] = lSid;
        iNum++;
        if(iNum > 1)
        {
            //当循环到第二行，增加一个逗号
            utPltPutLoopVar(psDbHead, "dh", iNum, ","); // utPltPutLoopVar输出一个循环变量的值。
        }
        utPltPutLoopVarF(psDbHead, "sid", iNum, "%lu", lSid);
        utPltPutLoopVar(psDbHead, "casecode", iNum, casecode);
        utPltPutLoopVar(psDbHead, "casename", iNum, casename);
        //         utPltPutLoopVar(psDbHead,"casetype",iNum,casetype);
        //          utPltPutLoopVar(psDbHead,"caseclass",iNum,caseclass);
        //               utPltPutLoopVar(psDbHead,"groupid",iNum,groupid);
        //            utPltPutLoopVar(psDbHead,"casedept",iNum,casedept);
        //         utPltPutLoopVar(psDbHead,"casedate",iNum,casedate);
        utPltPutLoopVar(psDbHead, "addby", iNum, addby);
        utPltPutLoopVar(psDbHead, "cmemo", iNum, cmemo);
        if(lLasttime > 0)
        {
            utPltPutLoopVar(psDbHead, "lasttime", iNum, utTimFormat("%y/%m/%d %H:%M", lLasttime));
        }
        if(lExpiretime > 0)
        {
            utPltPutLoopVar(psDbHead, "expiretime", iNum, utTimFormat("%Y/%m/%d", lExpiretime));
        }
        utPltPutLoopVarF(psDbHead, "flags", iNum, "%lu", lFlags);

        lExpiretime = 0;
        lLasttime = 0;
        lFlags = 0;
        lSid = 0;
        lCasetype = 0;
        lGroupid = 0;
        lCaseclass = 0;
        ireturn = pasDbFetchInto(
                      psCur, UT_TYPE_ULONG,  4,      &lSid,
                      UT_TYPE_STRING,  sizeof(casecode) - 1,      casecode,
                      UT_TYPE_STRING,  sizeof(casename) - 1,      casename,
                      UT_TYPE_ULONG,  4,     &lCasetype,
                      UT_TYPE_ULONG,  4,    &lCaseclass,
                      UT_TYPE_ULONG, 4,  &lGroupid,
                      UT_TYPE_STRING,  sizeof(casedept) - 1,   casedept,
                      UT_TYPE_ULONG,  4,  &lCasedate,
                      UT_TYPE_STRING,  sizeof(addby) - 1,  addby,
                      UT_TYPE_STRING,  sizeof(cmemo) - 1,  cmemo,
                      UT_TYPE_ULONG,  4, &lLasttime,
                      UT_TYPE_ULONG,  4, &lExpiretime,
                      UT_TYPE_ULONG, 4, &lFlags);

    }
    //关闭游标
    pasDbCloseCursor(psCur);
    char objids[1024];
    char objname[2048];
    unsigned long lCtype = 0;
    unsigned lDid = 0;
    long iNum2;
    for(i = 0; i < iNum; i++)
    {
        //布控区域
        memset(objids, 0, sizeof(objids));
        memset(objname, 0, sizeof(objname));
        sprintf(sql, "select ctype,did from ncscaseobj where cid=%lu group by ctype,did order by ctype limit 0,100", sids[i]);
        //      printf("sql=%s\n",sql);
        psCur = pasDbOpenSql(sql, 0);
        if(psCur)
        {
            lCtype = 0;
            lDid = 0;
            iNum2 = 0;
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_ULONG,  4, &lCtype,
                                     UT_TYPE_ULONG, 4, &lDid);
            while(0 == iReturn || 1405 == iReturn)
            {
                if(lDid == 0)
                {
                    strcpy(objname, "全部");
                    strcpy(objids, "0_0");
                    break;
                }
                if(iNum2 == 0)
                {
                    sprintf(objids, "%lu_%lu", lCtype, lDid);
                    if(lCtype == 0)
                    {
                        sprintf(objname, "%s", ncsUtlGetClientNameById(psShmHead, lDid, "\0"));
                    }
                    else if(lCtype == 2)
                    {
                        sprintf(objname, "%s", ncsUtlGetGroupNameById(psShmHead, lDid, "\0"));
                    }
                }
                else
                {
                    sprintf(objids + strlen(objids), ",%lu_%lu", lCtype, lDid);
                    if(lCtype == 0)
                    {
                        sprintf(objname + strlen(objname), ",%s", ncsUtlGetClientNameById(psShmHead, lDid, "\0"));
                    }
                    else if(lCtype == 2)
                    {
                        sprintf(objname + strlen(objname), ",%s", ncsUtlGetGroupNameById(psShmHead, lDid, "\0"));
                    }
                }
                iNum2++;
                lCtype = 0;
                lDid = 0;
                iReturn = pasDbFetchInto(psCur,
                                         UT_TYPE_ULONG,  4, &lCtype,
                                         UT_TYPE_ULONG, 4, &lDid);
            }
            pasDbCloseCursor(psCur);

            utPltPutLoopVar(psDbHead, "cobjid", i + 1, objids);
            utPltPutLoopVar(psDbHead, "cobj", i + 1, objname);

        }
        //统计策略
        sprintf(sql, "select sum(lcount) from (select count(*) as lcount from ncscasmac where cid=%lu union select count(*) as lcount from ncscasetermlim where cid=%lu) as ttb ", sids[i], sids[i]);
        printf("sql=%s\n", sql);
        lCount = 0;
        pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);
        utPltPutLoopVarF(psDbHead, "pcount", i + 1, "%d", lCount);

        //统计告警日志
        sprintf(sql, "select count(*),max(stime) from ncscasemacwarnlog where cid=%lu ", sids[i]);
        lCount = 0;
        lLasttime = 0;
        pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount,
                       UT_TYPE_ULONG, 4, &lLasttime);

        if(lLasttime > 0)
        {
            utPltPutLoopVar(psDbHead, "warnlasttime", i + 1, utTimFormat("%y/%m/%d %H:%M", lLasttime));
        }
        utPltPutLoopVarF(psDbHead, "wcount", i + 1, "%d", lCount);
    }



    utPltOutToHtml(iFd, psMsgHead, psDbHead, "/normally/proauth_controlmanage.htm");
    return 0;
}

//统计移动AP信息

int ncsMobileApStat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
        char *tmpgbk = NULL;
        char **ptmp = NULL;
        ncApSrvOnline *psAp;
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
   long iReturn;
  unsigned long lIp;
    char caStime[16];
        char caEtime[16];
        char caTemp_d[64];
        char caGroupid[36],caDid[36];
    char caSdate[24],caEdate[24];
    char caApmac[36],caMac[36],caApname[64],caPosition[64];
    char caCorpname[128];
    char caCont[2048];
    char caAps[4096];
    char caDispname[68];
    char caServicecode[16],caSsid[36],caSearchtype[32];
    long lLogcount;
    char caServicename[256];
    long iAp=0;
    unsigned char *pHash;
    long index;
      struct stApIndex
			{
			       char apname[24];
			       long index;
			};
			struct stApIndex *psData;
	 
	/* 
	 pasHashInfo     sHashInfo;
 char *pHash2 =  (unsigned char *)utShmHashHead(psShmHead,NC_LNK_APSRVONLINE);
        if(pHash2) {
     
                psAp = (ncApSrvOnline *)pasHashFirst(pHash2,&sHashInfo);
                while(psAp){
                	printf("apname=%s\n",psAp->apname);
                	printf("dispname=%s\n",psAp->dispname);
                	 psAp = (ncApSrvOnline *)pasHashNext(&sHashInfo);
                	}
                }
	 
	 
	 */
	 
	 
	 
	 
			pHash = (unsigned char *)pasLHashInit(100,100,sizeof(struct stApIndex), 0, 24);
       if(pHash==NULL){
    
       	return 0;
      }
  printf("ds=%s\n",getDsGroupcode());    
 //   printf("code=%s\n",getGroupCodeByGroupid(psShmHead,"1887814587"));
 //   printf("dssql=%s\n",getDsGroupcodeSql("servicecode"));
 //   printf("groupsql=%s\n",getGroupcodeSql("servicecode","1887814587"));
 strcpy(caGroups,getDsGroupcode());    
 printf("iRet=%d\n",isInGroupCode("41011521020019",caGroups));
    utPltDbHead *psDbHead = utPltInitDb();
        utMsgPrintMsg(psMsgHead);
        iReturn = utMsgGetSomeNVar(psMsgHead,13,
          "TotPg",      UT_TYPE_STRING, 12, caTotPg,
          "CurPg",      UT_TYPE_STRING, 12, caCurPg,
          "keyword", UT_TYPE_STRING,60,caKeyword,
          "page",            UT_TYPE_STRING,10,caPage,
          "limit",           UT_TYPE_STRING,10,caLimit,
          "sort",            UT_TYPE_STRING, 15, caSort,
          "dir",          UT_TYPE_STRING, 8,  caDir,
          "time_flag",    UT_TYPE_STRING,8,caTime_flag,
          "sdate",        UT_TYPE_STRING,12,sdate,
          "stime",        UT_TYPE_STRING,10,caStime,
          "edate",        UT_TYPE_STRING,12,edate,
          "etime",        UT_TYPE_STRING,10,caEtime,
          "export",       UT_TYPE_STRING,10,caExport);
   

          memset(caCont,0,sizeof(caCont));
     memset(caAps,0,sizeof(caAps));

     if(strlen(caKeyword)>0){
        strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
        strcpy(caKeyword,caTemp_d);
      }
      memset(caAps,0,sizeof(caAps));
     if(strlen(caKeyword)>0){
     	sprintf(caTemp,"select apname from ncaplist where dispname like '%c%s%c' limit 0,10",'%',caKeyword,'%');
     	          psCur = pasDbOpenSql(caTemp, 0);               
                if(psCur != NULL)
                {
                         memset(caApname,0,sizeof(caApname));                      
                        iret = pasDbFetchInto(psCur,UT_TYPE_STRING,31,caApname);
                        while((iret == 0) || (iret == 1405))
                        {
                                iNum++;
                                if(iNum > 1)
                                {                                
                                   sprintf(caAps+strlen(caAps),",\'%s\'",caApname);                                   
                                }
                                else{
                                	sprintf(caAps,"\'%s\'",caApname);
                                }                             
				                        iret = pasDbFetchInto(psCur,                           
				                             UT_TYPE_STRING,31,caApname);				                           
                        }
                        pasDbCloseCursor(psCur);
                }
     	
     } 

      lCurPg = atol(caPage);
      if(lCurPg <= 0) lCurPg = 1;


      if(strlen(caLimit)>0){
         lRowNum=atol(caLimit);
      }
      else{
         lRowNum =40;//每一页面行数
      }
      lStartRec = (lCurPg - 1) * lRowNum;

      if(strlen(caExport)>0){
      	lRowNum=100000;
      	lStartRec=0;
      	
      	 sprintf(caFilename,"ncsmobileap%d.csv",utStrGetId());
		     sprintf(caPath,"%s/html","/home/ncmysql/ncs");
		     sprintf(caFile,"%s/%s",caPath,caFilename);
      		fp = fopen(caFile,"w");
		        if(fp == NULL) {
		        	return 0;
		        }
		        
		       fprintf(fp,"移动采集设备信息\n");
		        ncUtlPrintCsv(fp,5,UT_TYPE_STRING,"移动设备名称",
		                           UT_TYPE_STRING,"移动设备编号",
		                           UT_TYPE_STRING,"场所编码",
		                           UT_TYPE_STRING,"场所名称",
		                           UT_TYPE_STRING,"轨迹数");
		                           
		        
		        
      }  
      
      

        iTime_flag = atoi(caTime_flag);
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
                ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncmacmobileap_if_", &lStartTime, &lTime, &lCount);
                printf("lStartTime=%d,sdate=%s\n",lStartTime,sdate);
        }
        else{
                ptmp = ncsUtlGetTable(lTime, days, "ncmacmobileap_if_",  &lStartTime, &lCount);
                lTime=lTime+2*3600;
        }

     memset(caCont,0,sizeof(caCont));
     if(strlen(caKeyword)>0){
     	if(strlen(caAps)>0){
     		 sprintf(caCont," and (apname like '%c%s%c' or servicecode like '%c%s%c' or apname in(%s) ) ",'%',caKeyword,'%','%',caKeyword,'%',caAps);  
     	}
     	else{
        sprintf(caCont," and (apname like '%c%s%c' or servicecode like '%c%s%c' ) ",'%',caKeyword,'%','%',caKeyword,'%');  
      }  
     }
  

 char caSdate_cx[20],caEdate_cx[20],caStime_cx[12],caEtime_cx[12];
 strcpy(caSdate_cx,utTimFormat("%Y/%m/%d", lStartTime));
 strcpy(caStime_cx,utTimFormat("%H:%M:%S", lStartTime));
 
 strcpy(caEdate_cx,utTimFormat("%Y/%m/%d", lTime));
 strcpy(caEtime_cx,utTimFormat("%H:%M:%S", lTime));

                

        for(i = 0; i < lCount; i++)
        {

                        lCount1 = 0;
                        memset(sql, 0, sizeof(sql));
                        snprintf(sql, sizeof(sql), "select count(distinct apname) from %s where stime >= %lu and stime <= %lu  ",  ptmp[i], lStartTime, lTime);
                         strcat(sql,caCont);

                                printf("sql = %s\n", sql);
                                pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
                        if(lCount1 > 0)
                        {
                                strcpy(tableinfo[num].tname, ptmp[i]);
                                tableinfo[num].lCount = lCount1;
                                num++;
                        }
        //      }
                free(ptmp[i]);
        }
        free(ptmp);
        for(i = 0; i < num; i++)
        {
                lTotRec += tableinfo[i].lCount;
        }
        utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);

        //
        for(i = num-1; i >= 0; i--)
        {
                printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
        }
        //

        iNum = 0;
        iStart = lStartRec;
        iLimit = lRowNum;
        memset(caAps,0,sizeof(caAps));
        for(i = num-1; i >= 0; i--)
        {
                memset(sql, 0, sizeof(sql));
                if(tableinfo[i].lCount <= iStart)
                {
                        iStart-=tableinfo[i].lCount;
                        continue;
                }
                snprintf(sql, sizeof(sql), "select apname,servicecode,servicename,count(*) from %s where stime >= %lu and stime <= %lu  ", tableinfo[i].tname, lStartTime, lTime);
                 strcat(sql,caCont);

                snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " group by apname,servicecode,servicename  limit %lu, %lu", iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);


                if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
                {
                        iLimit = iLimit -  (tableinfo[i].lCount-iStart);
                        iStart = 0;
                }
                else
                {
                        _break = 1;
                }

                printf("__line = %d, sql = %s\n", __LINE__, sql);
                psCur = pasDbOpenSql(sql, 0);
                
                if(psCur != NULL)
                {
                         memset(caApname,0,sizeof(caApname));
                         memset(caServicecode,0,sizeof(caServicecode));
                         memset(caServicename,0,sizeof(caServicename));
                         lLogcount=0;
                        iret = pasDbFetchInto(psCur,                           
                             UT_TYPE_STRING,31,caApname,
                             UT_TYPE_STRING,14,caServicecode,
                             UT_TYPE_STRING,250,caServicename,
                             UT_TYPE_LONG,4,&lLogcount);
                            



                        while((iret == 0) || (iret == 1405))
                        {
                                iNum++;
                                if(iNum > 1&&iNum<41)
                                {
                                   utPltPutLoopVar(psDbHead,"dh",iNum,",");
                                  
                                   sprintf(caAps+strlen(caAps),",\'%s\'",caApname);
                                   
                                }
                                else{
                                	sprintf(caAps,"\'%s\'",caApname);
                                }

                               psData = (struct stData *)pasLHashLookA(pHash,caApname);  
                               if(psData){
                               	psData->index=iNum;
                               } 
                               if(iNum<41){ 	
												         utPltPutLoopVar(psDbHead,"apname",iNum,caApname);
												         utPltPutLoopVar(psDbHead,"servicename",iNum,caServicename);        
												         utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
                                 utPltPutLoopVarF(psDbHead,"logcount",iNum,"%d",lLogcount);
                                 utPltPutLoopVar(psDbHead,"sdate_cx",iNum,caSdate_cx);
                                 utPltPutLoopVar(psDbHead,"stime_cx",iNum,caStime_cx);
                                 utPltPutLoopVar(psDbHead,"edate_cx",iNum,caEdate_cx);
                                 utPltPutLoopVar(psDbHead,"etime_cx",iNum,caEtime_cx);
                                 
                               }
                                 
                              if(strlen(caApname)==21){
                                 	iAp=9;
                                }
                                else{
                                	iAp=0;
                                }
                        //     printf("apname=%s\n", caApname+iAp);
                             psAp = (ncApSrvOnline *)utShmHashLook(psShmHead,NC_LNK_APSRVONLINE,caApname+iAp);
                             if(psAp){
                             
                             	strcpy(caDispname,psAp->dispname);
                       //      	printf("dispname=%s\n",psAp->dispname);
                       if(iNum<41){
                       	  utPltPutLoopVar(psDbHead,"dispname",iNum,psAp->dispname);
                        }
                             } 
                             else{
                             	strcpy(caDispname,"");
                             } 
                             
                        if(strlen(caExport)>0){
                        
					                ncUtlPrintCsv(fp,5,UT_TYPE_STRING,caDispname,
                          UT_TYPE_STRING,caApname,
                          UT_TYPE_STRING,caServicecode,
                          UT_TYPE_STRING,caServicename,
                          UT_TYPE_LONG,lLogcount);
                        }
                                 
													         memset(caServicecode,0,sizeof(caServicecode));
													         memset(caServicename,0,sizeof(caServicename));
													         memset(caApname,0,sizeof(caApname));
													         lLogcount=0;
				                        iret = pasDbFetchInto(psCur,                           
				                             UT_TYPE_STRING,31,caApname,
				                             UT_TYPE_STRING,14,caServicecode,
				                             UT_TYPE_STRING,250,caServicename,
				                             UT_TYPE_LONG,4,&lLogcount);
                        }
                        pasDbCloseCursor(psCur);
                }

                if(_break == 1)
                        break;
        }

//统计AP 收件的MAC
    sprintf(caTemp,"select count(*),apname from nctermmacs_ap where apname in (%s) and moditime>=%lu and moditime<=%lu group by apname ",caAps,lStartTime, lTime);
    printf("caTemp=%s\n",caTemp);
    psCur = pasDbOpenSql(caTemp, 0);
    if(psCur != NULL)
    {
    	               lCount=0;
    	               memset(caApname,0,sizeof(caApname));
    	               iret = pasDbFetchInto(psCur,   
    	                       UT_TYPE_LONG,4,&lCount,                        
                             UT_TYPE_STRING,31,caApname);
                      while((iret == 0) || (iret == 1405))
                        {
                        	 psData = (struct stData *)pasLHashLook(pHash,caApname);  
                 //       	 printf("caApname=%s\n",caApname);
                               if(psData){
                               	index=psData->index;
                //               	printf("index=%d\n",index);
                               	utPltPutLoopVarF(psDbHead,"maccount",index,"%d",lCount);
                               } 
                        	 lCount=0;
    	                     memset(caApname,0,sizeof(caApname));
                        	 iret = pasDbFetchInto(psCur,   
    	                       UT_TYPE_LONG,4,&lCount,                        
                             UT_TYPE_STRING,31,caApname);
                        }         
          pasDbCloseCursor(psCur);                  
    }
    
    
     free(pHash);                         


if(strlen(caExport)>0){

    //    utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
    //    remove(caFile);
    //    utPltFreeDb(psDbHead);
     utPltPutVar(psDbHead,"path",caPath);
     utPltPutVar(psDbHead,"filename",caFilename);
 //     utPltOutToHtml(iFd,psMsgHead,psDbHead,"mac/infoquery/mobileap/mobileap_downmsg.htm");
//        return 0;
	 	
	 	
	 }






    utPltOutToHtml(iFd,psMsgHead,psDbHead,"mac/infoquery/mobileap/mobileap_stat_list.htm");
    return 0;
}

int ncsMobileApGj_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
        char *tmpgbk = NULL;
        char **ptmp = NULL;

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
   long iReturn;
  unsigned long lIp;
    char caStime[16];
        char caEtime[16];
        char caTemp_d[64];
        char caGroupid[36],caDid[36];
    char caSdate[24],caEdate[24];
    char caApmac[36],caMac[36],caApname[64],caPosition[64];
    char caCorpname[128];
    char caCont[2048];
    char caServicecode[16],caSsid[36],caSearchtype[32];
    long lLogcount;
    char caServicename[256];    
    char caLongitude[24],caLatitude[24];
    long lStime;
    utPltDbHead *psDbHead = utPltInitDb();
        utMsgPrintMsg(psMsgHead);
        iReturn = utMsgGetSomeNVar(psMsgHead,10,
          "page",            UT_TYPE_STRING,10,caPage,
          "limit",           UT_TYPE_STRING,10,caLimit,
          "apname",          UT_TYPE_STRING,32,caApname,
          "sort",            UT_TYPE_STRING, 15, caSort,
          "dir",          UT_TYPE_STRING, 8,  caDir,
          "time_flag",    UT_TYPE_STRING,8,caTime_flag,
          "sdate",        UT_TYPE_STRING,12,sdate,
          "stime",        UT_TYPE_STRING,10,caStime,
          "edate",        UT_TYPE_STRING,12,edate,
          "etime",        UT_TYPE_STRING,10,caEtime);
          

        memset(caCont,0,sizeof(caCont));

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
                ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncmacmobileap_if_", &lStartTime, &lTime, &lCount);
                printf("lStartTime=%d,sdate=%s\n",lStartTime,sdate);
        }
        else{
                ptmp = ncsUtlGetTable(lTime, days, "ncmacmobileap_if_",  &lStartTime, &lCount);
                lTime=lTime+2*3600;
        }

     memset(caCont,0,sizeof(caCont));
     if(strlen(caApname)>0){
        sprintf(caCont," and apname='%s' ",caApname);    
     }

 
        iNum=0;
        for(i = 0; i < lCount; i++)
        {
           sprintf(sql,"select distinct longitude,latitude,stime from %s where stime >= %lu and stime <= %lu and  length(longitude)>1 and length(latitude)>1 ",  ptmp[i], lStartTime, lTime);                       
           strcat(sql,caCont);
           
    //       printf("sql = %s\n", sql);     
          free(ptmp[i]);
          psCur = pasDbOpenSql(sql, 0);
          if(psCur != NULL)
          {
          	iret = pasDbFetchInto(psCur,                           
                             UT_TYPE_STRING,11,caLongitude,
                             UT_TYPE_STRING,11,caLatitude,
                             UT_TYPE_LONG,4,&lStime);

                        while((iret == 0) || (iret == 1405))
                        {
                        	iNum++;
                        	                            
                                if(iNum > 1)
                                {
                                  utPltPutLoopVar(psDbHead,"dh",iNum,",");
                                }
                        	      utPltPutLoopVar(psDbHead,"longitude",iNum,caLongitude);
                        	      utPltPutLoopVar(psDbHead,"latitude",iNum,caLatitude);
                        	      utPltPutLoopVar(psDbHead,"stime",iNum,utTimFormat("%Y/%m/%d %H:%M:%S",lStime));
                        	 iret = pasDbFetchInto(psCur,                           
                             UT_TYPE_STRING,11,caLongitude,
                             UT_TYPE_STRING,11,caLatitude,
                             UT_TYPE_LONG,4,&lStime);
                        }
                       pasDbCloseCursor(psCur);
                }  
          	
         }
          
    
        free(ptmp);



	



       utPltPutVarF(psDbHead,"TotRec", "%lu", iNum);

      
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"mac/infoquery/mobileap/mobileap_gj_list.htm");
    return 0;
}




int ncsMobileApMac_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
   char caSdate_all[32],caEdate_all[32];     
   unsigned long lStarttime,lModitime;
        pasDbCursor *psCur = NULL;
  char caGroups[1024];
  char caFacs[1024];
   char caArea[256],caName2[256];
     char caTemp[1024];
   long iReturn;
  unsigned long lIp;
    char caStime[16];
        char caEtime[16];
        char caTemp_d[64];
        char caGroupid[36],caDid[36];
    char caSdate[24],caEdate[24];
    char caApmac[36],caMac[36],caApname[64],caPosition[64];
    char caCorpname[128];
    char caCont[2048];
    char caServicecode[16],caSsid[36],caSearchtype[32];
    long lLogcount;
    char caServicename[256];    
    char caLongitude[24],caLatitude[24];
    long lStime;
    utPltDbHead *psDbHead = utPltInitDb();
        utMsgPrintMsg(psMsgHead);
        iReturn = utMsgGetSomeNVar(psMsgHead,9,
          "page",            UT_TYPE_STRING,10,caPage,
          "limit",           UT_TYPE_STRING,10,caLimit,
          "apname",          UT_TYPE_STRING,32,caApname,
          "sort",            UT_TYPE_STRING, 15, caSort,
          "dir",          UT_TYPE_STRING, 8,  caDir,
          "sdate",        UT_TYPE_STRING,12,sdate,
          "stime",        UT_TYPE_STRING,10,caStime,
          "edate",        UT_TYPE_STRING,12,edate,
          "etime",        UT_TYPE_STRING,10,caEtime);
 
      lRowNum = atol(caLimit);
      lCurPg = atol(caPage);
      if(lCurPg <= 0) lCurPg = 1;
      lStartRec = (lCurPg - 1) * lRowNum;
     
     
       sprintf(caSdate_all,"%s %s",sdate,caStime);
       sprintf(caEdate_all,"%s %s",edate,caEtime);
       lStartTime=utTimStrToLong("%Y/%m/%d %H:%M:%S",caSdate_all);
       lTime=utTimStrToLong("%Y/%m/%d %H:%M:%S",caEdate_all);
        

   
    
     sprintf(sql,"select count(*) from nctermmacs_ap where 1=1 ");
     if(strlen(caApname)>0){
        sprintf(sql+strlen(sql)," and apname='%s' ",caApname);    
     }
     if(strlen(sdate)>0){
     	  sprintf(sql+strlen(sql)," and moditime>=%lu ",lStartTime);
     }
     if(strlen(edate)>0){
    	  sprintf(sql+strlen(sql)," and moditime<=%lu ",lTime);
     }
     printf("sql=%s\n",sql);
     pasDbOneRecord(sql,0,UT_TYPE_LONG,4,&lCount);
 
        iNum=0;
       
           sprintf(sql,"select mac,starttime,moditime from nctermmacs_ap where 1=1 ");
           if(strlen(caApname)>0){
		        sprintf(sql+strlen(sql)," and apname='%s' ",caApname);    
		     }
		     if(strlen(sdate)>0){
		     	  sprintf(sql+strlen(sql)," and moditime>=%lu ",lStartTime);
		     }
		     if(strlen(edate)>0){
		    	  sprintf(sql+strlen(sql)," and moditime<=%lu ",lTime);
		     }
          sprintf(sql+strlen(sql)," order by moditime desc limit %d,%d", lStartRec, lRowNum);
   printf("sql=%s\n",sql);       
          psCur = pasDbOpenSql(sql, 0);
          if(psCur != NULL)
          {
          	memset(caMac,0,sizeof(caMac));
          	lStarttime=0;
          	lModitime=0;
          	iret = pasDbFetchInto(psCur,                           
                             UT_TYPE_STRING,19,caMac,
                             UT_TYPE_LONG,4,&lStarttime,
                             UT_TYPE_LONG,4,&lModitime);
                             

                        while((iret == 0) || (iret == 1405))
                        {
                        	iNum++;
                        	                            
                                if(iNum > 1)
                                {
                                  utPltPutLoopVar(psDbHead,"dh",iNum,",");
                                }
                        	      utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
                        	      utPltPutLoopVar(psDbHead,"moditime",iNum,utTimFormat("%Y/%m/%d %H:%M:%S",lModitime));
                        	      utPltPutLoopVar(psDbHead,"starttime",iNum,utTimFormat("%Y/%m/%d %H:%M:%S",lStarttime));
          									memset(caMac,0,sizeof(caMac));
          									lStarttime=0;
          									lModitime=0;
							          	iret = pasDbFetchInto(psCur,                           
							                             UT_TYPE_STRING,19,caMac,
							                             UT_TYPE_LONG,4,&lStarttime,
							                             UT_TYPE_LONG,4,&lModitime);
                        }
                       pasDbCloseCursor(psCur);
          }  
          	
        
      

        utPltPutVarF(psDbHead, "TotRec", "%d", lCount);

      
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"mac/infoquery/mobileap/mobileap_mac_list.htm");
    return 0;
}
//移动AP轨迹日志
int ncsMobileApLog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
        char *tmpgbk = NULL;
        char **ptmp = NULL;

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
  long iReturn;
  unsigned long lIp;
    char caStime[16];
        char caEtime[16];
        char caTemp_d[64];
        char caGroupid[36],caDid[36];
    char caSdate[24],caEdate[24];
    char caApmac[36],caMac[36],caApname[64],caPosition[64];
    char caCorpname[128];
    char caCont[2048];
    char caServicecode[16],caSsid[36],caSearchtype[32];
    long lLogcount;
    char caServicename[256];    
    char caLongitude[24],caLatitude[24];
    long lStime;
    utPltDbHead *psDbHead = utPltInitDb();
        utMsgPrintMsg(psMsgHead);
        iReturn = utMsgGetSomeNVar(psMsgHead,10,
          "page",            UT_TYPE_STRING,10,caPage,
          "limit",           UT_TYPE_STRING,10,caLimit,
          "apname",          UT_TYPE_STRING,32,caApname,
          "sort",            UT_TYPE_STRING, 15, caSort,
          "dir",          UT_TYPE_STRING, 8,  caDir,
          "time_flag",    UT_TYPE_STRING,8,caTime_flag,
          "sdate",        UT_TYPE_STRING,12,sdate,
          "stime",        UT_TYPE_STRING,10,caStime,
          "edate",        UT_TYPE_STRING,12,edate,
          "etime",        UT_TYPE_STRING,10,caEtime);
       lRowNum = atol(caLimit);
      lCurPg = atol(caPage);
      if(lCurPg <= 0) lCurPg = 1;
      lStartRec = (lCurPg - 1) * lRowNum;    

        memset(caCont,0,sizeof(caCont));

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
                ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncmacmobileap_if_", &lStartTime, &lTime, &lCount);
                printf("lStartTime=%d,sdate=%s\n",lStartTime,sdate);
        }
        else{
                ptmp = ncsUtlGetTable(lTime, days, "ncmacmobileap_if_",  &lStartTime, &lCount);
                lTime=lTime+2*3600;
        }

     memset(caCont,0,sizeof(caCont));
     if(strlen(caApname)>0){
        sprintf(caCont," and apname='%s' ",caApname);    
     }

 
        iNum=0;
        for(i = 0; i < lCount; i++)
        {
           sprintf(sql,"select apname,servicecode,servicename,longitude,latitude,stime from %s where stime >= %lu and stime <= %lu and  length(longitude)>1 and length(latitude)>1  ",  ptmp[i], lStartTime, lTime);                       
           strcat(sql,caCont);
           sprintf(sql+strlen(sql)," order by sid desc limit %d,%d", lStartRec, lRowNum);
    //       printf("sql = %s\n", sql);     
          free(ptmp[i]);
          psCur = pasDbOpenSql(sql, 0);
          if(psCur != NULL)
          {
          	memset(caApname,0,sizeof(caApname));
          	memset(caServicecode,0,sizeof(caServicecode));
          	memset(caServicename,0,sizeof(caServicename));
          	iret = pasDbFetchInto(psCur,  
          	                 UT_TYPE_STRING,30,caApname,  
          	                 UT_TYPE_STRING,20,caServicecode,
          	                 UT_TYPE_STRING,63,caServicename,                       
                             UT_TYPE_STRING,11,caLongitude,
                             UT_TYPE_STRING,11,caLatitude,
                             UT_TYPE_LONG,4,&lStime);

                        while((iret == 0) || (iret == 1405))
                        {
                        	iNum++;
                        	                            
                                if(iNum > 1)
                                {
                                  utPltPutLoopVar(psDbHead,"dh",iNum,",");
                                }
                                utPltPutLoopVar(psDbHead,"apname",iNum,caApname);
                                
                                utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
                                utPltPutLoopVar(psDbHead,"servicename",iNum,caServicename);
                        	      utPltPutLoopVar(psDbHead,"longitude",iNum,caLongitude);
                        	      utPltPutLoopVar(psDbHead,"latitude",iNum,caLatitude);
                        	      utPltPutLoopVar(psDbHead,"stime",iNum,utTimFormat("%Y/%m/%d %H:%M:%S",lStime));
          	memset(caApname,0,sizeof(caApname));
          	memset(caServicecode,0,sizeof(caServicecode));
          	memset(caServicename,0,sizeof(caServicename));
          	iret = pasDbFetchInto(psCur,  
          	                 UT_TYPE_STRING,30,caApname,  
          	                 UT_TYPE_STRING,20,caServicecode,
          	                 UT_TYPE_STRING,63,caServicename,                       
                             UT_TYPE_STRING,11,caLongitude,
                             UT_TYPE_STRING,11,caLatitude,
                             UT_TYPE_LONG,4,&lStime);
                        }
                       pasDbCloseCursor(psCur);
                }  
          	
         }
          
    
        free(ptmp);

       utPltPutVarF(psDbHead,"TotRec", "%lu", iNum);

      
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"mac/infoquery/mobileap/mobileap_log_list.htm");
    return 0;
}


int ncsDownloadFile(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
 
        char caFilename[128];
        char caFile[128];
        char caPath[128];
        long iReturn;
        utMsgPrintMsg(psMsgHead);
        iReturn = utMsgGetSomeNVar(psMsgHead,2,
          "path",            UT_TYPE_STRING,127,caPath,
          "filename",        UT_TYPE_STRING,127,caFilename);
          
     sprintf(caFile,"%s/%s",caPath,caFilename);
       
      utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
       
   if(strlen(caPath)>0&&strlen(caFile)>0){
     	printf("caFile=%s\n",caFile);
    	remove(caFile);
    }    
      
   
    return 0;
}

//显示某一个AP信息
int ncsShowPerApInfo(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead)
{   
  
    int iReturn;
    char caFname[80];
    utPltDbHead *psDbHead = utPltInitDb();
    char caApname[32],caServicecode[16],caDispname[128],caVersion[20],caMac[20],caSsid[32];
    char caAddress[128],caPlate[128],caMapid[64],caLongitude[16],caLatitude[16],caAptype[8];
    char caFaccode[16],caRadius[16],caLine[128],caServicename[128];;
    char caTemp[1024];
    long iAp=0;
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead,1,"apname",UT_TYPE_STRING,30,caApname);
    if(strlen(caApname)==21){
         iAp=9;
    }
    else{
         iAp=0;
    }
     sprintf(caTemp,"select apname,servicecode,dispname,servicename,version,mac,ssid,address,plate,line,mapid,longitude,latitude,aptype,fcode,radius from ncaplist "); 
     sprintf(caTemp+strlen(caTemp)," where apname='%s' ",caApname+iAp);
     memset(caApname,0,sizeof(caApname));
     memset(caServicecode,0,sizeof(caServicecode));
     memset(caDispname,0,sizeof(caDispname));
     memset(caVersion,0,sizeof(caVersion));
     memset(caMac,0,sizeof(caMac));
     memset(caSsid,0,sizeof(caSsid));
     memset(caAddress,0,sizeof(caAddress));
     memset(caPlate,0,sizeof(caPlate));
     memset(caLine,0,sizeof(caLine));
     memset(caMapid,0,sizeof(caMapid));
     memset(caLongitude,0,sizeof(caLongitude));
     memset(caLatitude,0,sizeof(caLatitude));
     memset(caAptype,0,sizeof(caAptype));
     memset(caFaccode,0,sizeof(caFaccode));
     memset(caRadius,0,sizeof(caRadius));
     printf("caTemp=%s\n",caTemp);
     pasDbOneRecord(caTemp,0,UT_TYPE_STRING,21,caApname,
                             UT_TYPE_STRING,15,caServicecode,
                             UT_TYPE_STRING,127,caDispname,
                             UT_TYPE_STRING,127,caServicename,
                             UT_TYPE_STRING,15,caVersion,
                             UT_TYPE_STRING,19,caMac,
                             UT_TYPE_STRING,31,caSsid,
                             UT_TYPE_STRING,127,caAddress,
                             UT_TYPE_STRING,127,caPlate,
                             UT_TYPE_STRING,127,caLine,
                             UT_TYPE_STRING,63,caMapid,
                             UT_TYPE_STRING,12,caLongitude,
                             UT_TYPE_STRING,12,caLatitude,
                             UT_TYPE_STRING,1,caAptype,
                             UT_TYPE_STRING,15,caFaccode,
                             UT_TYPE_STRING,10,caRadius);
     
      utPltPutVar(psDbHead,"apname",caApname);
      utPltPutVar(psDbHead,"servicecode",caServicecode);
      utPltPutVar(psDbHead,"dispname",caDispname);
      utPltPutVar(psDbHead,"servicename",caServicename);
      utPltPutVar(psDbHead,"version",caVersion);
      utPltPutVar(psDbHead,"mac",caMac);
      utPltPutVar(psDbHead,"ssid",caSsid);
      utPltPutVar(psDbHead,"address",caAddress);
      utPltPutVar(psDbHead,"plate",caPlate);
      utPltPutVar(psDbHead,"line",caLine);
      utPltPutVar(psDbHead,"mapid",caMapid);
      utPltPutVar(psDbHead,"longitude",caLongitude);
      utPltPutVar(psDbHead,"latitude",caLatitude);
      if(strcmp(caAptype,"1")==0){
      	utPltPutVar(psDbHead,"aptype","固定");
      }
      else if(strcmp(caAptype,"2")==0){
      	utPltPutVar(psDbHead,"aptype","移动车载");
      }
      else if(strcmp(caAptype,"3")==0){
      	utPltPutVar(psDbHead,"aptype","单兵采集设备");
      }
      memset(caFname,0,sizeof(caFname));
      if(strlen(caFaccode)>0){
      	sprintf(caTemp,"select name from ncsfactorycod where code='%s' ",caFaccode);
      	pasDbOneRecord(caTemp,0,UT_TYPE_STRING,69,caFname);
      	printf("caTemp=%s\n",caTemp);
      }
      if(strlen(caFname)>0){
      	utPltPutVar(psDbHead,"faccode",caFname);
      }
      else{
      utPltPutVar(psDbHead,"faccode",caFaccode);
    }
      utPltPutVar(psDbHead,"radius",caRadius);
//    utPltShowDb(psDbHead);
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"mac/infoquery/mobileap/mobileap_one.htm");
    return 0;
}

//统计移动AP采集到MAC数量，图表显示
int ncsMobileApMacCount_chart(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
  char caExport[32];
  long lCount_d;
  char caDay[20];
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
        char sql[4096] = "";
   char caTotPg[16],caCurPg[16];
   char caPage[12],caLimit[16],caSort[16],caDir[16];
   char caKeyword[64],caTime_flag[32];
        char sql2[256] = "";
   char caSdate_all[32],caEdate_all[32];     
   unsigned long lStarttime,lModitime;
        pasDbCursor *psCur = NULL;
  char caGroups[1024];
  char caFacs[1024];
   char caArea[256],caName2[256];
     char caTemp[1024];
   long iReturn;
  unsigned long lIp;
    char caStime[16];
        char caEtime[16];
        char caTemp_d[64];
        char caGroupid[36],caDid[36];
    char caSdate[24],caEdate[24];
    char caApmac[36],caMac[36],caApname[64],caPosition[64];
    char caCorpname[128];
    char caCont[2048];
    char caServicecode[16],caSsid[36],caSearchtype[32];
    long lLogcount;
    char caServicename[256];    
    char caLongitude[24],caLatitude[24];
    long lStime;
    long lStype;
    utPltDbHead *psDbHead = utPltInitDb();
        utMsgPrintMsg(psMsgHead);
        iReturn = utMsgGetSomeNVar(psMsgHead,9,
          "page",            UT_TYPE_STRING,10,caPage,
          "limit",           UT_TYPE_STRING,10,caLimit,
          "apname",          UT_TYPE_STRING,32,caApname,
          "sort",            UT_TYPE_STRING, 15, caSort,
          "dir",          UT_TYPE_STRING, 8,  caDir,
          "sdate",        UT_TYPE_STRING,12,sdate,
          "stime",        UT_TYPE_STRING,10,caStime,
          "edate",        UT_TYPE_STRING,12,edate,
          "etime",        UT_TYPE_STRING,10,caEtime);
 
      lRowNum = atol(caLimit);
      lCurPg = atol(caPage);
      if(lCurPg <= 0) lCurPg = 1;
      lStartRec = (lCurPg - 1) * lRowNum;
     
     
       sprintf(caSdate_all,"%s %s",sdate,caStime);
       sprintf(caEdate_all,"%s %s",edate,caEtime);
       lStartTime=utTimStrToLong("%Y/%m/%d %H:%M:%S",caSdate_all);
       lTime=utTimStrToLong("%Y/%m/%d %H:%M:%S",caEdate_all);
        

      if((lTime-lStartTime)<3600*24+200){
      	lStype=1;              //按小时统计
      }
      else if((lTime-lStartTime)<3600*24*32){
      	lStype=2;             //按日统计
      }
      else if((lTime-lStartTime)>3600*24*32){
      	lStype=3;             //按月统计
      }
      else{
      	lStype=1;
      }
    
   printf("lStype=%d\n",lStype);
 
        iNum=0;
       if(lStype==1){
           sprintf(sql,"select count(distinct mac),from_unixtime(moditime,'%cH') aa from nctermmacs_ap where 1=1 ",'%');
       }
       else if(lStype==2){
       	   sprintf(sql,"select count(distinct mac),from_unixtime(moditime,'%cY%cm%cd') aa from nctermmacs_ap where 1=1 ",'%','%','%');
       }
        else if(lStype==3){
        	sprintf(sql,"select count(distinct mac),from_unixtime(moditime,'%cY%cm') aa from nctermmacs_ap where 1=1 ",'%','%');
       }
       else{
       	 sprintf(sql,"select count(distinct mac),from_unixtime(moditime,'%cH') aa from nctermmacs_ap where 1=1 ",'%');
      }
           if(strlen(caApname)>0){
		        sprintf(sql+strlen(sql)," and apname='%s' ",caApname);    
		     }
		     if(strlen(sdate)>0){
		     	  sprintf(sql+strlen(sql)," and moditime>=%lu ",lStartTime);
		     }
		     if(strlen(edate)>0){
		    	  sprintf(sql+strlen(sql)," and moditime<=%lu ",lTime);
		     }
          sprintf(sql+strlen(sql)," group by aa order by aa  ");
   printf("sql=%s\n",sql); 
   printf("\n");      
          psCur = pasDbOpenSql(sql, 0);
          if(psCur != NULL)
          {
          	lCount_d=0;
          	memset(caDay,0,sizeof(caDay));
          	iret = pasDbFetchInto(psCur,
          	                 UT_TYPE_LONG,4,&lCount_d,
          	                 UT_TYPE_STRING,10,caDay);                           
                            
                             

                        while((iret == 0) || (iret == 1405))
                        {
                        	iNum++;
                        	                            
                                if(iNum > 1)
                                {
                                  utPltPutLoopVar(psDbHead,"dh",iNum,",");
                                }
                        	      utPltPutLoopVar(psDbHead,"day",iNum,caDay);
                        	      utPltPutLoopVarF(psDbHead,"countd",iNum,"%d",lCount_d);
                        	    
							          	lCount_d=0;
							          	memset(caDay,0,sizeof(caDay));
							          	iret = pasDbFetchInto(psCur,
							          	                 UT_TYPE_LONG,4,&lCount_d,
							          	                 UT_TYPE_STRING,10,caDay); 
                        }
                       pasDbCloseCursor(psCur);
          }  
          	
        
      

        utPltPutVarF(psDbHead, "TotRec", "%d", iNum);

      
    utPltOutToHtml(iFd,psMsgHead,psDbHead,"mac/infoquery/mobileap/mobileap_maccount.htm");
    return 0;
}







int procapAlarmSetFun_L(utShmHead *psShmHead)
{
    procapAlarmSetFun_2(psShmHead);
    pasSetTcpFunName("ncsMacGroupComp", ncsMacGroupComp, 0);
    pasSetTcpFunName("ncsSys_MacProcy", ncsSys_MacProcy, 0);
    pasSetTcpFunName("ncsSys_WarnNoty", ncsSys_WarnNoty, 0);
    pasSetTcpFunName("ncsMacWarnmanComp", ncsMacWarnmanComp, 0);
    pasSetTcpFunName("ncsSys_WarnMacLim", ncsSys_WarnMacLim, 0);
    pasSetTcpFunName("ncsNetidloglist_v4", ncsNetidloglist_v4, 0);
    pasSetTcpFunName("ncsTreeBkProcy_v4", ncsTreeBkProcy_v4, 0);
    pasSetTcpFunName("ncsSys_controlmanage2", ncsSys_controlmanage2, 0);
    pasSetTcpFunName("ncsMobileApStat_v4",ncsMobileApStat_v4, 0);
    pasSetTcpFunName("ncsMobileApGj_v4",ncsMobileApGj_v4, 0);
    pasSetTcpFunName("ncsMobileApMac_v4",ncsMobileApMac_v4, 0);
    pasSetTcpFunName("ncsMobileApLog_v4",ncsMobileApLog_v4, 0);
    pasSetTcpFunName("ncsDownloadFile",ncsDownloadFile, 0);
    pasSetTcpFunName("ncsShowPerApInfo",ncsShowPerApInfo, 0);
    pasSetTcpFunName("ncsMobileApMacCount_chart",ncsMobileApMacCount_chart, 0);
}
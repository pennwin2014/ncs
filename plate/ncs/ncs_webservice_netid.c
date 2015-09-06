
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include "ncsdef.h"

//查询虚拟身份基本信息

int wbs_selectData_NetIDBasicList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	 char caTemp[4096];
	 int iReturn,iNum,i;
	 int lResult,lCount,lMsgCode;
	 utMsgHead *psMsgHead1;
	 pasDbCursor *psCur;
	 long itemCount;
	  char caAlarmlog[8];
	 char v_DataItem[16],v_Matching[16],v_Content[16];
   char caDataItem[20],caMatching[20],caContent[68];
   char caMatching_t[16],caCond[1024];	 
   char caContent_t[68],Ip_out[20];
   char caMsg3[260],caType[32],caMesg2[260];
   unsigned long lTime,lIp,ntype;
   char caMesg[260];
   unsigned long sid,rule_id,level,lSid_lim,lSid;
   char sdate[24],mesg[100],service_code[16],sip[16],dip[16],endname[20],mac[24],dispname[32],idfytype[8],idcard[24],idfyunit[32];
   char country[4];
	 char v_service_code[24],v_matching_id[24],v_matching_level[24],v_matching_time[24],v_net_ending_ip[24];
	 char v_net_ending_name[24],v_net_ending_mac[24],v_destination_ip[24],v_service_type[24];
	 char v_keyword1[24],v_keyword2[24],v_keyword3[24],v_customer_name[24],v_certificate_type[24];
	 char v_rule_id[24],v_certificate_code[24],v_org_name[24],v_country[24];
	 char mac_temp[20],sdate_temp[20],matching_id[24],matching_level[24],s_rule_id[24];
   char e_rule_id[24],f_rule_id[24],g_rule_id[24];
   char caAllowAsyn[8];
   int e_num,f_num,g_num,e_service,g_service;
   char f_url[64],g_uid[64];
   unsigned long lStime;
   char caFilename[128],caPathfile[256];
	 FILE *fp=NULL; 
   char caData[128];
	 char *pDataDir;
	 int lSum;
	 char caBuf[800000],caBuf1[800000];
	 int filecount;
	 char filelist[100][128];
	 char keywork[256],keyword2[256],caService[24],caRule_id[24],caSip[24],caDip[24];
	#ifdef LDEBUG
	  utMsgPrintMsg(psMsgHead);
	#endif
	
	
	 sprintf(caTemp,"select alarmlog from upstatusproperty ");
    	iReturn=pasDbOneRecord(caTemp,0,UT_TYPE_STRING,1,caAlarmlog);
    	if(iReturn!=0){
    		strcpy(caAlarmlog,"N");
    	}
	
	   
    memset(caCond,0,sizeof(caCond));

   	 iReturn = utMsgGetSomeNVar(psMsgHead,2,"itemCount",UT_TYPE_LONG, 4, &itemCount,
   	                                        "AllowAsyn",UT_TYPE_STRING,1, caAllowAsyn);

		   for(i=0;i<itemCount;i++){
		   	sprintf(v_DataItem,"DataItem_%lu",i);
		   	sprintf(v_Matching,"Matching_%lu",i);
		   	sprintf(v_Content,"Content_%lu",i);
		   	utMsgGetSomeNVar(psMsgHead,3,v_DataItem,UT_TYPE_STRING,16,caDataItem,
		   	                             v_Matching,UT_TYPE_STRING,16,caMatching,
		   	                             v_Content,UT_TYPE_STRING,64,caContent);
		    trim(caDataItem);
		    trim(caMatching);
		    trim(caContent);

		   if((strlen(caDataItem)>0)&&(strlen(caMatching)>0)&&(strlen(caContent)>0)){

		    	memset(caMatching_t,0,sizeof(caMatching_t));
		    	ncsStrReplace(caMatching,caMatching_t,"&gt;",">");
		    	memset(caMatching,0,sizeof(caMatching));
		    	ncsStrReplace(caMatching_t,caMatching,"&lt;","<");
	
		    	if(strcasecmp(caDataItem,"Service_code") == 0){
		    		if(strcasecmp(caMatching,"like")==0){
		    			sprintf(caCond+strlen(caCond)," and c.username like '%c%s%c' ",'%',caContent,'%');
		    		}
		    		else
	           sprintf(caCond+strlen(caCond)," and c.username='%s' ",caContent);

		    		}
		        else if(strcasecmp(caDataItem,"Service_name")==0){	
		        	 if(strcasecmp(caMatching,"like")==0){
		    				sprintf(caCond+strlen(caCond)," and c.dispname like '%c%s%c' ",'%',caContent,'%');
		    		
		    			}
		    			else{
		    				sprintf(caCond+strlen(caCond)," and c.dispname %s '%s' ",caMatching,caContent);
		    				
		    			 }
		    			
		    		}			
		    		else if(strcasecmp(caDataItem,"Matching_time")==0){	
		    			 lTime=utTimStrToLong("%Y%m%d%H%M%S",caContent);	    		
		    			 sprintf(caCond+strlen(caCond)," and a.stime %s %lu ",caMatching,lTime);

		    		}			    		
		    		else if(strcasecmp(caDataItem,"Destination_ip")==0){	
		    			if(strcasecmp(caMatching,"like")==0){
		    				memset(Ip_out,0,sizeof(Ip_out));
		    				sprintf(caCond+strlen(caCond)," and inet_ntoa(a.sip) like '%c%s%c' ",'%',wbsStrIpDel0(caContent,Ip_out),'%');
		    			}
		    			else
		    				sprintf(caCond+strlen(caCond)," and inet_ntoa(a.sip)= '%s' ",wbsStrIpDel0(caContent,Ip_out));	
		    					
    			 }
    			 
    			 else if(strcasecmp(caDataItem,"Customer_name")==0){
    			 	if(strcasecmp(caMatching,"like")==0){
    			 		sprintf(caCond+strlen(caCond)," and a.udisp like '%c%s%c' ",'%',caContent,'%');
    			 	}
    			 	else
		    				sprintf(caCond+strlen(caCond)," and a.udisp= '%s' ",caContent);		    			
		    				
    			 }
    			 else if(strcasecmp(caDataItem,"Certificate_type")==0){
		    				sprintf(caCond+strlen(caCond)," and d.idfytype= '%s' ",caContent);
		    			  			
		    		
    			 }
    			 else if(strcasecmp(caDataItem,"Certificate_code")==0){
    			 		if(strcasecmp(caMatching,"like")==0){
    			 			sprintf(caCond+strlen(caCond)," and d.idcard like '%c%s%c' ",'%',caContent,'%');	
    			 		}
    			 		else
		    				sprintf(caCond+strlen(caCond)," and d.idcard= '%s' ",caContent);		    			
		    				
    			 }
    			 else if(strcasecmp(caDataItem,"Rule_id")==0){
    			 	 if(strcasecmp(caMatching,"like")==0){
    			 	 	 sprintf(caCond+strlen(caCond)," and (b.casecode like '%c%s%c')",'%',caContent,'%'); 
    			 	}
    			 	else{
    			 		sprintf(caCond+strlen(caCond)," and (b.casecode='%s' )",caContent); 
    			 	}
    			}
           else if(strcasecmp(caDataItem,"Rule_name")==0){
           	   if(strcasecmp(caMatching,"like")==0){
    			 	 	 sprintf(caCond+strlen(caCond)," and (b.casename like '%c%s%c')",'%',caContent,'%'); 
    			 	   }
    			 	  else{
    			 		  sprintf(caCond+strlen(caCond)," and (b.casename='%s')",caContent); 
    			 	  }
           	
          } 	  			
          else if(strcasecmp(caDataItem,"Matching_level")==0){   			
    			 		sprintf(caCond+strlen(caCond)," and b.caseclass=%s ",caContent);
           }
     }
   }
   
	  lSum=0;
    filecount=0;
   	iNum=0;  	
	 memset(caBuf,0,sizeof(caBuf));
	 sprintf(caTemp,"select a.sid,b.casecode,b.caseclass,a.stime,inet_ntoa(a.sip),c.username,d.mac,a.udisp,d.position,d.idfytype,d.idcard,d.idfyunit,d.country,message from ncscasemesg a,ncscaseinfo b,ncsuser c,ncsenduser d where a.cid=b.sid and a.did=d.did and a.uid=d.uid and a.did=c.userid %s limit 0,5000 ",caCond);
	 
	 

	 printf("caTemp=%s\n",caTemp);
	 psCur=pasDbOpenSql(caTemp,0);
	 if(psCur==NULL){
	 	printf("psCur is NULL\n");
	 	if(fp) fclose(fp);
	 	remove(caPathfile);
	 	return -1;
	}
	 iNum=0;
	 
	 
	 
	 sid=0;
	rule_id=0;
	 memset(sdate,0,sizeof(sdate));
	
	 memset(mesg,0,sizeof(mesg));
	 level=0;
	 
	memset(service_code,0,sizeof(service_code));
	memset(dispname,0,sizeof(dispname));
	memset(idfytype,0,sizeof(idfytype));
	memset(idcard,0,sizeof(idcard));
	memset(idfyunit,0,sizeof(idfyunit));
	memset(country,0,sizeof(country));
	 memset(mac,0,sizeof(mac));
   memset(dip,0,sizeof(dip));
	 memset(sdate,0,sizeof(sdate));
	 memset(endname,0,sizeof(endname));
	memset(e_rule_id,0,sizeof(e_rule_id));
	memset(g_rule_id,0,sizeof(g_rule_id));
	memset(f_rule_id,0,sizeof(f_rule_id));
  e_num=0;
  f_num=0;
  g_num=0;
  e_service=0;
  g_service=0;
  memset(f_url,0,sizeof(f_url));
  memset(g_uid,0,sizeof(g_uid));
  memset(caMesg,0,sizeof(caMesg));
	 iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&sid,
	                              UT_TYPE_STRING,20,e_rule_id,
	 															UT_TYPE_LONG,4,&e_num,
	                              UT_TYPE_LONG,4,&lStime,	                            
	                              UT_TYPE_STRING,15,sip,
	                              UT_TYPE_STRING,14,service_code,
	                              UT_TYPE_STRING,20,mac,
	                              UT_TYPE_STRING,31,dispname,	                             
	                              UT_TYPE_STRING,16,endname,                             
	                              UT_TYPE_STRING,3,idfytype,
	                              UT_TYPE_STRING,20,idcard,
	                              UT_TYPE_STRING,30,idfyunit,
	                              UT_TYPE_STRING,3,country,
	                              UT_TYPE_STRING,250,caMesg);
	                             
	                                                     
	 
	 while((iReturn == 0)||(iReturn==1405)) {
	 	if(strlen(idfytype)==0){
	 		strcpy(idfytype,"990");
	 	}
	 	
	 	ncsStrDel(mac,mac_temp,":");
	 
	  sprintf(matching_id,"%015d",sid);	

	 	memset(Ip_out,0,sizeof(Ip_out));
	  strcpy(caDip,"000.000.000.000");
	  memset(Ip_out,0,sizeof(Ip_out));
    strcpy(caSip,wbsStrReplaceIp(sip,Ip_out));
	 
	 strcpy(caRule_id,e_rule_id);
  
    sprintf(matching_level,"%lu",e_num);  
    strcpy(keywork,"");   
    strcpy(caService,"0000");
    strcpy(sdate_temp,utTimFormat("%Y%m%d%H%M%S",lStime));
	 	memset(keyword2,0,sizeof(keyword2));
	 	if(strlen(caMesg)>0){
	 		memset(caMsg3,0,sizeof(caMsg3));
	 		memset(caType,0,sizeof(caType));	 		
	 		strcpy(caMesg2,caMesg);
	 		ncUtlGetWordBetween(caMesg2,":[","]",caMsg3,60);

	 		utStrGetSomeWordBy(caMsg3,":",2,
                         UT_TYPE_STRING,31,caType,
                         UT_TYPE_STRING,31,keyword2);

      if(strcmp(caType,"QQ")==0){
      	strcpy(caService,"1002");
      }
      else if(strcmp(caType,"MSN")==0){
      	strcpy(caService,"1001");
	 	 }
  }
	sprintf(caBuf+strlen(caBuf),"    <Item Matching_id=\"%015d\" Service_code=\"%s\" Rule_id=\"%s\" Matching_level=\"%s\" Matching_time=\"%s\" Net_ending_ip=\"%s\" Net_ending_name=\"%s\" Net_ending_mac=\"%s\" Destination_ip=\"%s\" Service_type=\"%s\" KeyWord1=\"%s\" KeyWord2=\"%s\" KeyWord3=\"\" KeyWord4=\"%s\" File_name=\"\" Customer_name=\"%s\" Certificate_type=\"%s\" Certificate_code=\"%s\" Org_name=\"%s\" Country=\"%s\"/>\n",
	 	 sid,service_code,caRule_id,matching_level,sdate_temp,caSip,endname,mac_temp,caDip,caService,keywork,keyword2,caMesg,dispname,idfytype,idcard,idfyunit,country);
	 	
	 lSum++;
	 iNum++;	 
	 	 
	 	 
	 if(iNum>=1000){
			  	memset(caBuf1,0,sizeof(caBuf1));
			  	sprintf(caBuf1,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
			  	sprintf(caBuf1+strlen(caBuf1),"<Body>\n");
			  	sprintf(caBuf1+strlen(caBuf1),"  <Table Name=\"AlertList\" Description=\"过滤结果表\" ItemSum=\"%d\">\n",iNum);
			  	sprintf(caBuf1+strlen(caBuf1),"%s",caBuf);
			  	sprintf(caBuf1+strlen(caBuf1),"   </Table>\n");
			  	sprintf(caBuf1+strlen(caBuf1),"</Body>\n");
			  	
			  	iNum=0;
			  	memset(caBuf,0,sizeof(caBuf));
			  	if(lSum<=1000){
			  		lMsgCode = psMsgHead->lCode / 1000;
    				lMsgCode = lMsgCode * 1000 + 102;
	  				psMsgHead1 = utMsgInit(lMsgCode,psMsgHead->lFunCode,psMsgHead->lMsgId,800000);
    				if(psMsgHead1 == (utMsgHead *)NULL)
        				return (-1);
     
    				psMsgHead1->lReserve[0] = psMsgHead->lReserve[1];
    				psMsgHead1->lReserve[1] = psMsgHead->lReserve[0];
    				psMsgHead1->lFunCode  = psMsgHead->lFunCode;
			  		
			  		
			  	 utMsgAdd_s(&psMsgHead1,"AlertList",caBuf1);
			  	 
			  	 utComTcpMsgSend(iFd,psMsgHead1);
           utMsgFree(psMsgHead1);
          }
			  	else if((strcasecmp(caAllowAsyn,"Y")==0)&&(strcasecmp(caAlarmlog,"Y")==0)&&(filecount<MAXQUERY_NUM)){
			  		pDataDir = utComGetVar_sd(psShmHead,"DataPath","/home/ncmysql/ncs/data");
					  sprintf(caData,"%s/xml",pDataDir);    
					  if(utFileIsExist(caData)!=1) {
					        if(mkdir(caData,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
					          printf("mkdir error\n");
					            return (-1);
					        }
					   }
 
				    sprintf(caFilename,"AlertList%s_%d.xml",utTimFormat("%Y%m%d%H%M%S",time(0)),filecount);
				    sprintf(caPathfile,"%s/%s",caData,caFilename);
				    
				    fp = fopen(caPathfile,"w");
				    if(fp == NULL) {
				        printf(" File %s Open Error \n",caPathfile);
				        return (-1);
				    }
			  		fprintf(fp,"%s",caBuf1);
			  		fclose(fp);
			  		strcpy(filelist[filecount],caPathfile);			  		
			  		filecount++;
		  		
			  	}
			  	else{
			  		break;
			  	}
			  }
	 	 
	 	 
	 	 
	 sid=0;
	rule_id=0;
	 memset(sdate,0,sizeof(sdate));
	 memset(mesg,0,sizeof(mesg));
	 level=0;
	 
	memset(service_code,0,sizeof(service_code));
	memset(dispname,0,sizeof(dispname));
	memset(idfytype,0,sizeof(idfytype));
	memset(idcard,0,sizeof(idcard));
	memset(idfyunit,0,sizeof(idfyunit));
	memset(country,0,sizeof(country));
	 memset(mac,0,sizeof(mac));
   memset(dip,0,sizeof(dip));
	 memset(sdate,0,sizeof(sdate));
	 memset(endname,0,sizeof(endname));
	

	memset(e_rule_id,0,sizeof(e_rule_id));
	memset(g_rule_id,0,sizeof(g_rule_id));
	memset(f_rule_id,0,sizeof(f_rule_id));

  e_num=0;
  f_num=0;
  g_num=0;
  memset(f_url,0,sizeof(f_url));
  memset(g_uid,0,sizeof(g_uid));
  memset(f_url,0,sizeof(f_url));
  memset(g_uid,0,sizeof(g_uid));
  memset(caMesg,0,sizeof(caMesg));
	 iReturn=pasDbFetchInto(psCur,UT_TYPE_LONG,4,&sid,
	                              UT_TYPE_STRING,20,e_rule_id,
	 															UT_TYPE_LONG,4,&e_num,
	                              UT_TYPE_LONG,4,&lStime,	                            
	                              UT_TYPE_STRING,15,sip,
	                              UT_TYPE_STRING,14,service_code,
	                              UT_TYPE_STRING,20,mac,
	                              UT_TYPE_STRING,31,dispname,	                             
	                              UT_TYPE_STRING,16,endname,                             
	                              UT_TYPE_STRING,3,idfytype,
	                              UT_TYPE_STRING,20,idcard,
	                              UT_TYPE_STRING,30,idfyunit,
	                              UT_TYPE_STRING,3,country,
	                              UT_TYPE_STRING,250,caMesg);
	  }
	 	pasDbCloseCursor(psCur);
		
		
		

  	if((strlen(caBuf)>0)&&((strcasecmp(caAllowAsyn,"Y")==0)&&(strcasecmp(caAlarmlog,"Y")==0)&&(filecount<MAXQUERY_NUM))||(lSum<1000)){
		
			  	memset(caBuf1,0,sizeof(caBuf1));
			  	sprintf(caBuf1,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
			  	sprintf(caBuf1+strlen(caBuf1),"<Body Code=\"0\" Description=\"执行成功,没有错误\">\n");
			  	sprintf(caBuf1+strlen(caBuf1),"  <Table Name=\"AlertList\" Description=\"过滤结果表\" ItemSum=\"%d\">\n",iNum);
			  	sprintf(caBuf1+strlen(caBuf1),"%s",caBuf);
			  	sprintf(caBuf1+strlen(caBuf1),"   </Table>\n");
			  	sprintf(caBuf1+strlen(caBuf1),"</Body>\n");
			  	
			  	iNum=0;
			  	memset(caBuf,0,sizeof(caBuf));
			  	if(lSum<=1000){
			  		lMsgCode = psMsgHead->lCode / 1000;
    				lMsgCode = lMsgCode * 1000 + 102;
	  				psMsgHead1 = utMsgInit(lMsgCode,psMsgHead->lFunCode,psMsgHead->lMsgId,800000);
    				if(psMsgHead1 == (utMsgHead *)NULL)
        				return (-1);
     
    				psMsgHead1->lReserve[0] = psMsgHead->lReserve[1];
    				psMsgHead1->lReserve[1] = psMsgHead->lReserve[0];
    				psMsgHead1->lFunCode  = psMsgHead->lFunCode;
			  		
			  		
			  	 utMsgAdd_s(&psMsgHead1,"AlertList",caBuf1);		  	 
			  	 utComTcpMsgSend(iFd,psMsgHead1);
           utMsgFree(psMsgHead1);
          }
			  	else if((strcasecmp(caAllowAsyn,"Y")==0)&&(strcasecmp(caAlarmlog,"Y")==0)&&(filecount<MAXQUERY_NUM)){
			  		pDataDir = utComGetVar_sd(psShmHead,"DataPath","/home/ncmysql/ncs/data");
					  sprintf(caData,"%s/xml",pDataDir);    
					  if(utFileIsExist(caData)!=1) {
					        if(mkdir(caData,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
					          printf("mkdir error\n");
					            return (-1);
					        }
					   }
 
				    sprintf(caFilename,"AlertList%s_%d.xml",utTimFormat("%Y%m%d%H%M%S",time(0)),filecount);
				    sprintf(caPathfile,"%s/%s",caData,caFilename);
				    
				    fp = fopen(caPathfile,"w");
				    if(fp == NULL) {
				        printf(" File %s Open Error \n",caPathfile);
				        return (-1);
				    }
			  		fprintf(fp,"%s",caBuf1);
			  		fclose(fp);
			  		strcpy(filelist[filecount],caPathfile);			  		
			  		filecount++;
		  		
			  	}
			  	
			  	
			}
    
    
    for(i=0;i<filecount;i++)
    {  
	    sprintf(caTemp,"insert into ncsxmlfile(filename,stime) values('%s',%lu) ",filelist[i],time(0));
	    iReturn=pasDbExecSql(caTemp,0);
	    if(iReturn!=0) remove(caPathfile);
	  }

    return iReturn;
}
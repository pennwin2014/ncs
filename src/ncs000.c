#include <stdio.h>
#include "utoall.h"


int ncsNcGetTime(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcLogin(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcLogin_m(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcHeatBeat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcHeatBeat_m(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcGetSynSumm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcReqSynData(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcSendRealData(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcSendBaseData(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcSendFile(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDownLoadFile(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcSynCaseInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcSynCaseOk(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcDownCaseIndex(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcSynMessage(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcSendMessage(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsDebugReset(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugClientOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugClientInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugSynIndex(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugSynMonSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugSynRealIp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugSynRealWeb(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugSynRealErr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugSynRealWarn(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugSynRealMail(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugSynRealForm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugSynRealIm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugSynRealUserlog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugSynRealNetId(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDebugSynRealTelnet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsGetOnlineInfo(utShmHead *psShmHead,unsigned long lId,char *pKey);

int ncsReqAuthCenter(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcGetCom(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

// 转发
int ncsNcTopDownCaseIndex(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcTopSynCaseInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcTopSynCaseOk(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcTopDownLoadFile(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsNcsTopCaseIndex(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcsTopSynCaseInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcSynKeyWord(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsReqAuthWmd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncYqLoadHotelToShm(utShmHead *psShmHead,char *filepath);
//int ncShow_hotel_map(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//int ncSearch_hotel_map(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncyq_addmo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCustDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebReplaceco(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int companyinfo_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsShow_hotel_map(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//ncsClient *ncsGetClientInfoByGis(utShmHead *psShmHead,int *iSum,double fx1,double fy1,double fx2,double fy2);
int ncsBdMapSet_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsBaiduSave_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsBdMapInit_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncs_dept_subcorp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_dept_subcorp_am(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcSynDeptId(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);       // 下载单位和编码对照表
int ncsNcDeptHeat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);        // 心跳信息
int ncsNcSynDeptIdV2(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);


int ncsNcSynUserMacDown(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcSynUserMacUp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/* Wifi  */
int ncsWifiSynMacLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWifiSynMacLogv2(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/* 初试化通信函数  */
int ncsInitComFun(utShmHead *psShmHead)
{
    int iReturn;
    iReturn = pasSetTcpFunNameS("ncsWifiSynMacLog",    ncsWifiSynMacLog, NULL,            0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsWifiSynMacLogv2",    ncsWifiSynMacLogv2, NULL,            0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcGetTime",        ncsNcGetTime, NULL,            0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcLogin",          ncsNcLogin,   NULL,            0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcHeatBeat",       ncsNcHeatBeat,ncsGetOnlineInfo,0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcLogin_m",        ncsNcLogin_m,   NULL,            0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcHeatBeat_m",     ncsNcHeatBeat_m,ncsGetOnlineInfo,0,PAS_CRYPT_TEA);

    iReturn = pasSetTcpFunNameS("ncsNcGetSynSumm",     ncsNcGetSynSumm,ncsGetOnlineInfo,0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcReqSynData",     ncsNcReqSynData, ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcSendRealData",   ncsNcSendRealData,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcSendBaseData",   ncsNcSendBaseData,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcSendFile",       ncsNcSendFile,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsDownLoadFile",     ncsDownLoadFile,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcSynCaseInfo",    ncsNcSynCaseInfo,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcSynCaseOk",      ncsNcSynCaseOk,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcDownCaseIndex",  ncsNcDownCaseIndex,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcSynMessage",     ncsNcSynMessage,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcSendMessage",    ncsNcSendMessage,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcGetCom",          ncsNcGetCom,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);


    iReturn = pasSetTcpFunNameS("ncsNcTopDownCaseIndex",    ncsNcTopDownCaseIndex,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcTopSynCaseInfo",    ncsNcTopSynCaseInfo,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcTopSynCaseOk",    ncsNcTopSynCaseOk,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcTopDownLoadFile",    ncsNcTopDownLoadFile,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);

    iReturn = pasSetTcpFunNameS("ncsNcsTopCaseIndex",    ncsNcsTopCaseIndex,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcsTopSynCaseInfo",    ncsNcsTopSynCaseInfo,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsNcSynKeyWord",    ncsNcSynKeyWord,ncsGetOnlineInfo, 0,PAS_CRYPT_TEA);

    iReturn = pasSetTcpFunName("ncsDebugReset",  ncsDebugReset,  0);
    iReturn = pasSetTcpFunName("ncsDebugClientOnline",  ncsDebugClientOnline,  0);
    iReturn = pasSetTcpFunName("ncsDebugClientInfo",  ncsDebugClientInfo,  0);
    iReturn = pasSetTcpFunName("ncsDebugSynIndex",  ncsDebugSynIndex,  0);
    iReturn = pasSetTcpFunName("ncsDebugSynMonSet",  ncsDebugSynMonSet,  0);
    iReturn = pasSetTcpFunName("ncsDebugSynRealIp",  ncsDebugSynRealIp, 0);
    iReturn = pasSetTcpFunName("ncsDebugSynRealWeb",  ncsDebugSynRealWeb,  0);
    iReturn = pasSetTcpFunName("ncsDebugSynRealErr",  ncsDebugSynRealErr,  0);
    iReturn = pasSetTcpFunName("ncsDebugSynRealWarn",  ncsDebugSynRealWarn,0);
    iReturn = pasSetTcpFunName("ncsDebugSynRealMail",  ncsDebugSynRealMail,0);
    iReturn = pasSetTcpFunName("ncsDebugSynRealForm",  ncsDebugSynRealForm,0);
    iReturn = pasSetTcpFunName("ncsDebugSynRealIm",  ncsDebugSynRealIm,0);
    iReturn = pasSetTcpFunName("ncsDebugSynRealUserlog",  ncsDebugSynRealUserlog,0);
    iReturn = pasSetTcpFunName("ncsDebugSynRealNetId",  ncsDebugSynRealNetId,0);
    iReturn = pasSetTcpFunName("ncsDebugSynRealTelnet",  ncsDebugSynRealTelnet,0);
    iReturn=pasSetTcpFunName("ncyq_addmo",ncyq_addmo,0);
		iReturn=pasSetTcpFunName("ncsCustDrNext",ncsCustDrNext,0);
		iReturn=pasSetTcpFunName("ncWebReplaceco",ncWebReplaceco,0);
		iReturn=pasSetTcpFunName("companyinfo_imp",companyinfo_imp,0);
		iReturn=pasSetTcpFunName("ncsShow_hotel_map",ncsShow_hotel_map,0);
		iReturn=pasSetTcpFunName("ncsBdMapSet_par",ncsBdMapSet_par,0);
		iReturn=pasSetTcpFunName("ncsBaiduSave_par",ncsBaiduSave_par,0);
		iReturn=pasSetTcpFunName("ncsBdMapInit_par",ncsBdMapInit_par,0);


    iReturn = pasSetTcpFunNameS("ncsReqAuthCenter",        ncsReqAuthCenter, NULL,            0,PAS_CRYPT_TEA);
    iReturn = pasSetTcpFunNameS("ncsReqAuthWmd",        ncsReqAuthWmd, NULL,            0,PAS_CRYPT_TEA);

    iReturn=pasSetTcpFunName("ncs_dept_subcorp",ncs_dept_subcorp,0);   //子单位管理
	iReturn=pasSetTcpFunName("ncs_dept_subcorp_am",ncs_dept_subcorp_am,0);   //子单位管理
	
	iReturn=pasSetTcpFunNameS("ncsNcSynDeptId",ncsNcSynDeptId,ncsGetOnlineInfo,0,PAS_CRYPT_TEA);   
	iReturn=pasSetTcpFunNameS("ncsNcSynDeptIdV2",ncsNcSynDeptIdV2,ncsGetOnlineInfo,0,PAS_CRYPT_TEA);   
	iReturn=pasSetTcpFunNameS("ncsNcDeptHeat",ncsNcDeptHeat,ncsGetOnlineInfo,0,PAS_CRYPT_TEA);   
	iReturn=pasSetTcpFunNameS("ncsNcSynUserMacDown",ncsNcSynUserMacDown,ncsGetOnlineInfo,0,PAS_CRYPT_TEA);   
	iReturn=pasSetTcpFunNameS("ncsNcSynUserMacUp",ncsNcSynUserMacUp,ncsGetOnlineInfo,0,PAS_CRYPT_TEA);   



    return 0;
}



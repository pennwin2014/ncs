#include <stdio.h>
#include "utoall.h"
#include "ncsrtl.h"

int ncsWebBMZD(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebZKlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebHYSserverSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_smpptset(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_smpptsetsave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_smppt_test(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebShowServiceDet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebOnlineuserlog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsQQPasslist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int qqShowpassword(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int vpnServerPar(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int vpnServerSave_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int vpnClientPar(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int vpnClientSave_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int vpnDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int vpnGateToGateConnect(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int vpnAddGateConnect(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int vpnGate2GateSave_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int vpnClientCnf(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int vpnGenClientCnf(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int vpnuserlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int vpnAddUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int vpnUserSave(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int vpnuseractive(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int vpnclientnet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int vpnClientNetAdd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int vpnClientNetSave(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);



int ncsWebQQSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispMtoQQ(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispQQCont(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispQQLoginExit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebMacSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispMacUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispMacLoginExit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebOthIMSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebBBSVidSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebShowNetidBt(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebBBSform(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncsImDet_Con(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsImStat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebGameVidSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebFTPVidSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebMailVidSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebUplogStat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispCorpUplog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSearchLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebAccountMail(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSesIplog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSesHttplog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSesMaillog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSesBBSform(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncsSesIm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSesSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLimWebList2(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncReportHis(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncXzReportHis(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebStatVidAll(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsYjcx_Con(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebNetIdYjcx(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebMacYjcx(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int NcsBBSMon_syj(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int NcsImLog_syj(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int NcsMailMon_syj(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncsWebStatBBSAll(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebStatImAll(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebStatMailAll(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsWebruleSet_zd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsVidruleSet_zd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsMailruleSet_zd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsjstxruleSet_zd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsBbsruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsdefaultruleSet_zd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsAllruleSet_zd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebTasklist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebTaskDetlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTaskdet_Add(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTaskdet_Edit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebStatVidAllByUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispUserSearchlog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_Areaonline_Stat_xm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebExcepDept(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebTreeDispGroupCorp_nolimit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_Areaonline_AvStat_xm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsReqSendMobilePasswd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebTreeDispCorp_user(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebTreeDispCorp_zd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//重点用户上网日志
int ncsWebNetLog_zd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebBBSform(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncsMovTelDicSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebVidSearch_Exp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsMacAddStat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSendMobilePasswd_cy(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsReqSendJGUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsReqSendWmdUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsReqAuthWmd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsProcerror_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/*IDC相关函数*/
int ncs_idc_net_show(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_idc_net_locus(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_idc_webpage_search(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_idc_alarm_search(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsIdcSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsIdcSetKey(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsIdcRule_maintain(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsIdcRlue_am(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsIdcRulesSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebTreeIdcrule_nolimit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsIDC_RulekeyDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_Idc_key_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebReplaceco_key(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);


int ncsIdcRulekey_maintain(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsIdc_watchArea(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int suportReqTestLink(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsWebFactoryCode(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebTreeDispFactory(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_Factory_Stat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//海量日志查询
int ncsIpSearch_ib(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsHttpSearch_ib(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserlogSearch_ib(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsFormonSearch_ib(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsMailSearch_ib(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsImSearch_ib(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSearchLog_ib(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsVidSearch_ib(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncHttpLogin(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncHttpLogout(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncsGetHttpUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsReqCenterHotelUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//同步旅馆用户信息到网络督察
int ncsReqSysHotelUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncs_Type_Stat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncKeywordDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncskeyword_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSiteRegLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSiteLoginLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int WebShowpassword(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//管理中心转发短信
int ncsReqCenterSms(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsWebModiSomeClientBm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);


//新版管理中心
int ncsWebDispClientAddForm_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebClientSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebClientAdd_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeGroup_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsShowServiceCorpDet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebGroupComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCorpIp_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebClientClassList_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDispDeptForm_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeUser_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserlogSearch_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_onlinecorp_first_pie(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebGroupComp0(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_onlinecorp_first_onlinerate(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWarnInfoOnline_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseList_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeGroup_comp_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseDet_netid_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebNettypeComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSitetypeComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseDet_website_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseDet_user_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseDet_keyword_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsKeywordtypeComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncCaseKeywodDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int case_keyword_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSenserviceComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseDet_service_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseDet_ipport_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsMobileComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseDet_warnset_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseArea_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCase_xflog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseDet_search_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeKeyword_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCase_keywordDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSysUserlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsRolelist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispRoleFunlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsSysUserRoleConfig_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSysReflesh_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSysloglist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsEndUserlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeSnetid_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSentive_NetidDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncSenVidDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSenVid_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsTreeUrl_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSen_UrlDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSen_CCICDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSen_UserDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncSentiveUserDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_sentiveuser_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsKewordlog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSenUrllog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSenNetidlog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSenUserlog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsVid_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsMacVid_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebShowUserBh(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsUserHttpStat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserHttpDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserSearchStat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserSearchDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserBbsStat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserBbsDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserImStat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsImdetcontlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserMailDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserNetidStat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserNetidDet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeKeyword_yh_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeUrl_yh_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeNetid_yh_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserlogStatByday_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsShowZhtj_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsFacCode_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebFacComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeFac_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTestRight_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeCase_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeCase_yh_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsLogistlog_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsShowOneMail_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebMailCtlOutHtm_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebPostRead_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsMailCtlDownload_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDispXxztZgt_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeKeyword_log_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeUrl_log_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeNetid_log_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeSenuser_log_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispTool_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispToolSave_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsMgcl_stat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsVid_stat_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsVid_statdet_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebKeywordComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncRecvSmbInfo_new(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsTreeNettype_comp_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsLbCode_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeLb_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLbComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeUserLb_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSendMobileMesg_cy(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsKeywordXf_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeUserKeyword_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsTreeRpTask_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncReportHis_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSendMMesg_sms(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSendPasswd_sms(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//烽火日志上传统计
int ncsFuUpload_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebTaskDetlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSendAd_Stat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_lbonlinerate(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsReqSendJGUser_S(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsReqMobileNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncHttpReg(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncsWebAuditList_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsGroupCorpComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDateComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUrlidComp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsAuditSave_v8(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsGetGatawayIp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncMailSendSet_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncMailSendSave_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int nc_opentest_mail(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int nc_setest_mail(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//搜索引擎
int ncsbbsmon_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncsimlog_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncsshowfield_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncsmailmon_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncsformmon_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncsfulltext_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncgetattfile_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

int ncsbbsFulltext_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncsMailFulltext_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

int ncWebBBSLog_utf8(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int ncsScanMaclist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNotifySyscorp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSyscorp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCheckstate(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncHttpUserReg(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncHttpApReg(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncHttpDetect(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);


//快速上网
int ncsQuickSearch_sxx(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsQuickSearch_netidlog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsQuickSearch_httplog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsQuickSearch_searchlog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsQuickSearch_bbslog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsQuickSearch_maillog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

// 统计
int ncs_user_log(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_user_log_wif(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebClientSearch_wif(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDispClientAddForm_wif(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebClientAdd_wif(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int proauth_statusparset(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int proauth_statusparsave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_org_on_line_log(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int procapWeb001Fun(utShmHead *psShmHead);
/* 初试化Web操作函数  */
int ncsInitWebFun(utShmHead *psShmHead)
{
    int iReturn;
    proauthInitWeb001Fun(psShmHead);
    procapInitWeb001Fun(psShmHead);
    iReturn = pasSetTcpFunName("ncsTest2006",            ncsTest2006,   0);
    iReturn = pasSetTcpFunName("ncsWebAuth",            ncsWebAuth,   0);
    iReturn = pasSetTcpFunName("ncsWebDispModiPass",    ncsWebDispModiPass,   0);
    iReturn = pasSetTcpFunName("ncsWebModiPass",        ncsWebModiPass,   0);

    iReturn = pasSetTcpFunName("ncsWebClientSearch",    ncsWebClientSearch,   0);

/* ncs004.c */    
    iReturn = pasSetTcpFunName("ncsWebDispClientAddForm",ncsWebDispClientAddForm,0);
    iReturn = pasSetTcpFunName("ncsWebClientAdd",        ncsWebClientAdd,   0);
    iReturn = pasSetTcpFunName("ncWebDelSomeClient",        ncWebDelSomeClient,   0);
    iReturn = pasSetTcpFunName("ncsWebClientOnline",        ncsWebClientOnline,   0);
    iReturn = pasSetTcpFunName("ncsWebClientGoNc",        ncsWebClientGoNc,   0);
    iReturn=pasSetTcpFunName("ncsWebAdminUserList",ncsWebAdminUserList,0);
    iReturn=pasSetTcpFunName("ncsWebAdminDispUserForm",ncsWebAdminDispUserForm,0);
    iReturn=pasSetTcpFunName("ncsAdminUserSave",ncsAdminUserSave,0);
    iReturn=pasSetTcpFunName("ncsWebAdminRoleList",ncsWebAdminRoleList,0);
    iReturn=pasSetTcpFunName("ncsAdminRoleModi",ncsAdminRoleModi,0);
    iReturn=pasSetTcpFunName("ncsWebAdminRole",ncsWebAdminRole,0);

    iReturn=pasSetTcpFunName("ncsWebShowProcess",ncsWebShowProcess,0);
    iReturn=pasSetTcpFunName("ncsWebShowProcess",ncsWebShowOnTime,0);
/* ncs005.c */

//    ncInitWebFun(psShmHead);
/*ncsweb000.c*/
   iReturn=pasSetTcpFunName("ncWebTreeDispCorp",ncWebTreeDispCorp,0);
   iReturn=pasSetTcpFunName("ncWebTreeDispClass",ncWebTreeDispClass,0);
   iReturn=pasSetTcpFunName("ncCorpClassAddForm",ncCorpClassAddForm,0);
   iReturn=pasSetTcpFunName("ncsWebReplace",ncsWebReplace,0);
   iReturn=pasSetTcpFunName("ncsIpruleSet",ncsIpruleSet,0);
   iReturn=pasSetTcpFunName("ncsWebruleSet",ncsWebruleSet,0);
   iReturn=pasSetTcpFunName("ncsAllruleSet",ncsAllruleSet,0);
   iReturn=pasSetTcpFunName("ncsMailruleSet",ncsMailruleSet,0);
   iReturn=pasSetTcpFunName("ncsjstxruleSet",ncsjstxruleSet,0);
   iReturn=pasSetTcpFunName("ncsformruleSet",ncsformruleSet,0);
   iReturn=pasSetTcpFunName("ncsftpruleSet",ncsftpruleSet,0);
   iReturn=pasSetTcpFunName("ncsdefaultruleSet",ncsdefaultruleSet,0);
   
 /*ncsweb004.c*/

   iReturn=pasSetTcpFunName("ncswarnWebruleSet",ncswarnWebruleSet,0);
   iReturn=pasSetTcpFunName("ncsAllwarnruleSet",ncsAllwarnruleSet,0);
   iReturn=pasSetTcpFunName("ncsMailwarnruleSet",ncsMailwarnruleSet,0);
   iReturn=pasSetTcpFunName("ncsjstxwarnruleSet",ncsjstxwarnruleSet,0);
   iReturn=pasSetTcpFunName("ncsformwarnruleSet",ncsformwarnruleSet,0);
   iReturn=pasSetTcpFunName("ncsvistNetRulSet",ncsvistNetRulSet,0);
   iReturn=pasSetTcpFunName("ncsWarnClass",ncsWarnClass,0);
   iReturn=pasSetTcpFunName("ncWarnClassAddForm",ncWarnClassAddForm,0);
   iReturn=pasSetTcpFunName("ncWarnNotfyAddForm",ncWarnNotfyAddForm,0);
   iReturn=pasSetTcpFunName("ncsWebLimDateList",ncsWebLimDateList,0);
   iReturn=pasSetTcpFunName("ncsWebLimDateGroup",ncsWebLimDateGroup,0);
   iReturn=pasSetTcpFunName("ncsWebLimDateGroupEdit",ncsWebLimDateGroupEdit,0);
   iReturn=pasSetTcpFunName("ncsWebLimIpList",ncsWebLimIpList,0);
   iReturn=pasSetTcpFunName("ncsWebLimIpGroup",ncsWebLimIpGroup,0);
   iReturn=pasSetTcpFunName("ncsWebLimIpGroupEdit",ncsWebLimIpGroupEdit,0);
   iReturn=pasSetTcpFunName("ncsWebLimIpPolList",ncsWebLimIpPolList,0);
   iReturn=pasSetTcpFunName("ncsWebLim_SetIpPol",ncsWebLim_SetIpPol,0);
   iReturn=pasSetTcpFunName("ncsWebLimIpPolEdit",ncsWebLimIpPolEdit,0);
   iReturn=pasSetTcpFunName("ncsWebLimWebClass",ncsWebLimWebClass,0);
   iReturn=pasSetTcpFunName("ncsWebLimWebList",ncsWebLimWebList,0);
   iReturn=pasSetTcpFunName("ncsWebWebPolClass",ncsWebWebPolClass,0);
   iReturn=pasSetTcpFunName("ncsWebLim_DispWebPo",ncsWebLim_DispWebPo,0);
   iReturn=pasSetTcpFunName("ncsWebWebPolList",ncsWebWebPolList,0);
   iReturn=pasSetTcpFunName("mailclasslist",mailclasslist,0);
   iReturn=pasSetTcpFunName("mailaddrlist",mailaddrlist,0);
   iReturn=pasSetTcpFunName("ncs_userclasslist",ncs_userclasslist,0);
   iReturn=pasSetTcpFunName("ncs_useridlist",ncs_useridlist,0);
   iReturn=pasSetTcpFunName("ncs_netidclass_mod",ncs_netidclass_mod,0);
   iReturn=pasSetTcpFunName("ncsWebSentiveGroup",ncsWebSentiveGroup,0);
   iReturn=pasSetTcpFunName("ncsWebSentiveList",ncsWebSentiveList,0);
   iReturn=pasSetTcpFunName("ncsWebSentiveEdit",ncsWebSentiveEdit,0);
   
   iReturn=pasSetTcpFunName("ncsWebSiteBbsEdit",ncsWebSiteBbsEdit,0);
   iReturn=pasSetTcpFunName("ncsWebSiteBbsGroup",ncsWebSiteBbsGroup,0);
   iReturn=pasSetTcpFunName("ncsWebSiteBbsList",ncsWebSiteBbsList,0);
//ncsweb002.c
   iReturn=pasSetTcpFunName("ncsWebCorpDep",ncsWebCorpDep,0);
   iReturn=pasSetTcpFunName("ncWebCorpDepSearch",ncWebCorpDepSearch,0);
   iReturn=pasSetTcpFunName("ncsWebCorpUser",ncsWebCorpUser,0);
   iReturn=pasSetTcpFunName("ncsWebNetId",ncsWebNetId,0);
   iReturn=pasSetTcpFunName("ncsWebNetLog",ncsWebNetLog,0);
   iReturn=pasSetTcpFunName("ncsWebShowUserinfo",ncsWebShowUserinfo,0);
   iReturn=pasSetTcpFunName("ncsWebNetIdLog",ncsWebNetIdLog,0);
   iReturn=pasSetTcpFunName("ncsWebIpLog",ncsWebIpLog,0);
   iReturn=pasSetTcpFunName("ncsWebWebLog",ncsWebWebLog,0);
   iReturn=pasSetTcpFunName("ncWebSelectZd",ncWebSelectZd,0);
   
//ncsweb002-1.c
   iReturn=pasSetTcpFunName("ncsWebMailLog",ncsWebMailLog,0);
   iReturn=pasSetTcpFunName("ncsWebImLog",ncsWebImLog,0);
   iReturn=pasSetTcpFunName("ncsWebFormLog",ncsWebFormLog,0);
   iReturn=pasSetTcpFunName("ncsWebFobLog",ncsWebFobLog,0);
   iReturn=pasSetTcpFunName("ncsWebWarnLog",ncsWebWarnLog,0);
   iReturn=pasSetTcpFunName("ncsWebLoginLog",ncsWebLoginLog,0);
   iReturn=pasSetTcpFunName("ncsWebMailMonRead",ncsWebMailMonRead,0);
   iReturn=pasSetTcpFunName("ncsWebMailCtlOutHtm",ncsWebMailCtlOutHtm,0);
   iReturn=pasSetTcpFunName("ncsWebMailCtlDownload",ncsWebMailCtlDownload,0);
   iReturn=pasSetTcpFunName("ncsUtlMailReadSelf",ncsUtlMailReadSelf,0);
   iReturn=pasSetTcpFunName("ncsWebPostRead",ncsWebPostRead,0);
   iReturn=pasSetTcpFunName("ncsWebMailMonCvtCode",ncsWebMailMonCvtCode,0);
   
   iReturn=pasSetTcpFunName("ncsNetidGroupEdit",ncsNetidGroupEdit,0);

 //ncsweb001.c
   iReturn=pasSetTcpFunName("ncsWebMailOnline",ncsWebMailOnline,0);
   iReturn=pasSetTcpFunName("ncsWebImOnline",ncsWebImOnline,0);
   iReturn=pasSetTcpFunName("ncsWebFtpOnline",ncsWebFtpOnline,0);
   iReturn=pasSetTcpFunName("ncsWebWebVistOnline",ncsWebWebVistOnline,0);
   iReturn=pasSetTcpFunName("ncsWebNetVistOnline",ncsWebNetVistOnline,0);
   iReturn=pasSetTcpFunName("ncsWebWarnOnline",ncsWebWarnOnline,0);
 //ncsweb001-1.c
   iReturn=pasSetTcpFunName("ncsWebLoginOnline",ncsWebLoginOnline,0);
   iReturn=pasSetTcpFunName("ncsWebErrOnline",ncsWebErrOnline,0);
   iReturn=pasSetTcpFunName("ncsWebFormOnline",ncsWebFormOnline,0);
   iReturn=pasSetTcpFunName("ncsWebNetServiceOnline",ncsWebNetServiceOnline,0);
   iReturn=pasSetTcpFunName("ncsIPvistOnline",ncsIPvistOnline,0);
   
 //ncsweb005.c
   iReturn=pasSetTcpFunName("ncsWebShowSystemInfo",ncsWebShowSystemInfo,0);
   iReturn=pasSetTcpFunName("ncsWebAdmQyeSysLog",ncsWebAdmQyeSysLog,0);
   iReturn=pasSetTcpFunName("ncsWebShowDataBack",ncsWebShowDataBack,0);
   iReturn=pasSetTcpFunName("ncsWebBackFileDownload",ncsWebBackFileDownload,0);
   iReturn=pasSetTcpFunName("ncsWebDelMailBack",ncsWebDelMailBack,0);
   iReturn=pasSetTcpFunName("ncsWebBackFileDownload",ncsWebBackFileDownload,0);
   iReturn=pasSetTcpFunName("ncsWebShutdown",ncsWebShutdown,0);
 //ncsweb000-1.c
   iReturn=pasSetTcpFunName("ncsWebSystemSet_back",ncsWebSystemSet_back,0);
   iReturn=pasSetTcpFunName("ncsWebSystemSave_back",ncsWebSystemSave_back,0);
   iReturn=pasSetTcpFunName("ncsWebSystemSet_clean",ncsWebSystemSet_clean,0);
   iReturn=pasSetTcpFunName("ncsWebSystemSave_clean",ncsWebSystemSave_clean,0);
   iReturn=pasSetTcpFunName("ncsWebSystemSet_par",ncsWebSystemSet_par,0);
   iReturn=pasSetTcpFunName("ncsWebSystemSave_par",ncsWebSystemSave_par,0);
   
//ncsweb003.c
   iReturn=pasSetTcpFunName("ncsDispWarnStaCon",ncsDispWarnStaCon,0);
   iReturn=pasSetTcpFunName("ncs_warnbycorp_stat",ncs_warnbycorp_stat,0);
   iReturn=pasSetTcpFunName("ncs_warnbyclassname_stat",ncs_warnbyclassname_stat,0);
   iReturn=pasSetTcpFunName("ncs_warnbydate_stat",ncs_warnbydate_stat,0);
   iReturn=pasSetTcpFunName("ncs_errbycorp_stat",ncs_errbycorp_stat,0);
   iReturn=pasSetTcpFunName("ncs_errbydip_stat",ncs_errbydip_stat,0);
   iReturn=pasSetTcpFunName("ncsWebRefresh",ncsWebRefresh,0);
   iReturn=pasSetTcpFunName("ncsNetSesLog",ncsNetSesLog,0);
   iReturn=pasSetTcpFunName("ncs_web_stat",ncs_web_stat,0);
   iReturn=pasSetTcpFunName("ncs_ip_stat",ncs_ip_stat,0);
   
//ncsweb006.c
	 iReturn=pasSetTcpFunName("ncsWebTreeDispDep",ncsWebTreeDispDep,0);
	 iReturn=pasSetTcpFunName("ncsWebTreeDispUser",ncsWebTreeDispUser,0);
	 iReturn=pasSetTcpFunName("ncsDispNetAccount",ncsDispNetAccount,0);
	 iReturn=pasSetTcpFunName("ncsNetAccount",ncsNetAccount,0);
	 iReturn=pasSetTcpFunName("ncsWebTreeDispCompt",ncsWebTreeDispCompt,0);
	 iReturn=pasSetTcpFunName("ncsWebOnlineSearch",ncsWebOnlineSearch,0);
	 iReturn=pasSetTcpFunName("ncsWebDispOnlineCorp",ncsWebDispOnlineCorp,0);
	 
	 iReturn=pasSetTcpFunName("ncWebDrNext",ncWebDrNext,0);
	 iReturn=pasSetTcpFunName("webclass_imp",webclass_imp,0);
	 iReturn=pasSetTcpFunName("ncWebLimWebClassCx",ncWebLimWebClassCx,0);
	  
	 iReturn=pasSetTcpFunName("ncWebSelectClass",ncWebSelectClass,0);
	 iReturn=pasSetTcpFunName("ncsWebDispNetIdAdd",ncsWebDispNetIdAdd,0);
	 iReturn=pasSetTcpFunName("ncsNetidAddSave",ncsNetidAddSave,0);
	 iReturn=pasSetTcpFunName("ncsDispNetidIframe",ncsDispNetidIframe,0);
	 
	 iReturn=pasSetTcpFunName("ncsCaseList",ncsCaseList,0);
	 iReturn=pasSetTcpFunName("ncsCaseBaseInfo",ncsCaseBaseInfo,0);
	 iReturn=pasSetTcpFunName("ncsCaseAddSave",ncsCaseAddSave,0);
	 iReturn=pasSetTcpFunName("ncsCaseWatch",ncsCaseWatch,0);
	 iReturn=pasSetTcpFunName("ncsCaseWatchArea",ncsCaseWatchArea,0);
	 iReturn=pasSetTcpFunName("ncsCaseWarnSet",ncsCaseWarnSet,0);
	 iReturn=pasSetTcpFunName("ncsCaseWatchStat",ncsCaseWatchStat,0);
	 iReturn=pasSetTcpFunName("ncWebSelectWarnNotfy",ncWebSelectWarnNotfy,0);
	 iReturn=pasSetTcpFunName("ncsCaseWarnNotySave",ncsCaseWarnNotySave,0);
	 iReturn=pasSetTcpFunName("ncWebCaseSearch",ncWebCaseSearch,0);
	 iReturn=pasSetTcpFunName("ncsCaseCtlStat",ncsCaseCtlStat,0);
	 iReturn=pasSetTcpFunName("ncCaseImDrNext",ncCaseImDrNext,0);
	 iReturn=pasSetTcpFunName("case_im_imp",case_im_imp,0);
	 iReturn=pasSetTcpFunName("ncCaseUserDrNext",ncCaseUserDrNext,0);
	 iReturn=pasSetTcpFunName("case_user_imp",case_user_imp,0);
	 iReturn=pasSetTcpFunName("ncsWebTracUser",ncsWebTracUser,0);
	 
	 iReturn=pasSetTcpFunName("ncsWebShowFirstRight",ncsWebShowFirstRight,0);
	 iReturn=pasSetTcpFunName("ncsWebShowFirstLeft",ncsWebShowFirstLeft,0);
	 iReturn=pasSetTcpFunName("ncsShowMainCorpOnline",ncsShowMainCorpOnline,0);
	 iReturn=pasSetTcpFunName("ncsWarnInfoOnline",ncsWarnInfoOnline,0);
	 iReturn=pasSetTcpFunName("ncsCaseWarnlist",ncsCaseWarnlist,0);
	 iReturn=pasSetTcpFunName("ncsCorpIp",ncsCorpIp,0);
	 iReturn=pasSetTcpFunName("ncWebWarnSearch",ncWebWarnSearch,0);
	 iReturn=pasSetTcpFunName("ncsWebZdjkUser",ncsWebZdjkUser,0);
	 iReturn=pasSetTcpFunName("ncsWebZdjkSet_rule",ncsWebZdjkSet_rule,0);
	 iReturn=pasSetTcpFunName("ncsWebShowGjNext",ncsWebShowGjNext,0);
	 iReturn=pasSetTcpFunName("ncsGjUserlist",ncsGjUserlist,0);
	 iReturn=pasSetTcpFunName("ncsWebRuleQuery",ncsWebRuleQuery,0);
	 iReturn=pasSetTcpFunName("ncsNoAutoBackup",ncsNoAutoBackup,0);
	 iReturn=pasSetTcpFunName("ncsWebBackRestore",ncsWebBackRestore,0);
	 iReturn=pasSetTcpFunName("ncWebDispSyslogSearch",ncWebDispSyslogSearch,0);
	 iReturn=pasSetTcpFunName("ncsvidwarnruleSet",ncsvidwarnruleSet,0);
	 iReturn=pasSetTcpFunName("ncsClassbase_stat",ncsClassbase_stat,0);
	 iReturn=pasSetTcpFunName("ncs_Areaonline_Stat",ncs_Areaonline_Stat,0);
	 
	 iReturn=pasSetTcpFunName("ncsRealUserStat",ncsRealUserStat,0);
	 iReturn=pasSetTcpFunName("ncsWebRealUserSearch",ncsWebRealUserSearch,0);
   iReturn=pasSetTcpFunName("ncsDispNetidDet",ncsDispNetidDet,0);
   iReturn=pasSetTcpFunName("ncsWebDispCorp_new",ncsWebDispCorp_new,0);
   iReturn=pasSetTcpFunName("ncsNetidStat",ncsNetidStat,0);
   iReturn=pasSetTcpFunName("ncsWebVidSearch",ncsWebVidSearch,0);
   iReturn=pasSetTcpFunName("ncsUserNetidStat",ncsUserNetidStat,0);
   iReturn=pasSetTcpFunName("ncsUserNetidDet",ncsUserNetidDet,0);
   iReturn=pasSetTcpFunName("ncsVistNetTrac",ncsVistNetTrac,0);
   iReturn=pasSetTcpFunName("ncsWebNetIdTrac",ncsWebNetIdTrac,0);
   
   iReturn=pasSetTcpFunName("ncsWebAuditList",ncsWebAuditList,0);
   iReturn=pasSetTcpFunName("ncsDispAuditAdd",ncsDispAuditAdd,0);
   iReturn=pasSetTcpFunName("ncsAuditSave",ncsAuditSave,0);
   iReturn=pasSetTcpFunName("ncsWebAdultLog",ncsWebAdultLog,0);
   
   iReturn=pasSetTcpFunName("ncs_CCIC",ncs_CCIC,0);
   
   iReturn=pasSetTcpFunName("ncsDispRolelist",ncsDispRolelist,0);
   iReturn=pasSetTcpFunName("ncsDispRoleFunlist",ncsDispRoleFunlist,0);
   iReturn=pasSetTcpFunName("ncsWebRoleEdit",ncsWebRoleEdit,0);
   iReturn=pasSetTcpFunName("ncsRoleRightSave",ncsRoleRightSave,0);
   
   iReturn=pasSetTcpFunName("ncsMovTelSet",ncsMovTelSet,0);
   iReturn=pasSetTcpFunName("ncCaseCCICDrNext",ncCaseCCICDrNext,0);
   iReturn=pasSetTcpFunName("ccic_user_imp",ccic_user_imp,0);
   iReturn=pasSetTcpFunName("ncsDispNetidMain",ncsDispNetidMain,0);
   iReturn=pasSetTcpFunName("ncsDispNetidOth",ncsDispNetidOth,0);
   iReturn=pasSetTcpFunName("ncsWebAuth2",ncsWebAuth2,0);
   
   iReturn=pasSetTcpFunName("ncsWebDeptModRootname_new",ncsWebDeptModRootname_new,0);
   iReturn=pasSetTcpFunName("ncsWebTreeDispGroup_nolimit",ncsWebTreeDispGroup_nolimit,0);
   
   iReturn=pasSetTcpFunName("ncsWebDispDeptForm_new",ncsWebDispDeptForm_new,0);
   iReturn=pasSetTcpFunName("wbs_selectData_ISBSList_hx",wbs_selectData_ISBSList_hx,0);
   iReturn=pasSetTcpFunName("wbs_selectData_ServiceUserList",wbs_selectData_ServiceUserList,0);
   iReturn=pasSetTcpFunName("wbs_ServiceStatus",wbs_ServiceStatus,0);
   
   iReturn=pasSetTcpFunName("ncsWebDeptAdd_new",ncsWebDeptAdd_new,0);
   iReturn=pasSetTcpFunName("ncsWebDeptDel_new",ncsWebDeptDel_new,0);
   iReturn=pasSetTcpFunName("ncsWebDeptModi_new",ncsWebDeptModi_new,0);
   iReturn=pasSetTcpFunName("ncs_Areaonline_Stat_new",ncs_Areaonline_Stat_new,0);
   
   iReturn=pasSetTcpFunName("ncsWebSubservice",ncsWebSubservice,0);
   
   ////ncsSearchMany.c
   iReturn = pasSetTcpFunName("ncWebSelectClass2", ncWebSelectClass2, 0);
   iReturn = pasSetTcpFunName("ncsWebShowSearchAccessListPlate", ncsWebShowSearchAccessListPlate, 0);
   iReturn = pasSetTcpFunName("ncsWebSaveCondition", ncsWebSaveCondition, 0);
   iReturn = pasSetTcpFunName("ncsShowSearchResult", ncsShowSearchResult, 0);
   iReturn = pasSetTcpFunName("ncsUpdateResult", ncsUpdateResult, 0);
   iReturn = pasSetTcpFunName("ncsWebShowSearchTask", ncsWebShowSearchTask, 0); 
   iReturn = pasSetTcpFunName("ncsMailCommand", ncsMailCommand, 0);
   iReturn = pasSetTcpFunName("ncsShowMailResult", ncsShowMailResult, 0);
   iReturn = pasSetTcpFunName("ncsShowSearchPreResult", ncsShowSearchPreResult, 0);
   iReturn = pasSetTcpFunName("ncsWebPostRead2", ncsWebPostRead2, 0);
   
   
   //移动密宝
   iReturn = pasSetTcpFunName("ncMbDrNext", ncMbDrNext, 0);
   iReturn = pasSetTcpFunName("ncMbLogin", ncMbLogin, 0);
   iReturn = pasSetTcpFunName("ncWebMbSave", ncWebMbSave, 0);
   iReturn = pasSetTcpFunName("ncUserBindMb", ncUserBindMb, 0);
   iReturn = pasSetTcpFunName("ncWebMbManagerSave", ncWebMbManagerSave, 0);
   iReturn = pasSetTcpFunName("ncWebDisplayMbManager", ncWebDisplayMbManager, 0);
   
   iReturn = pasSetTcpFunName("ncsPassProcyList", ncsPassProcyList, 0);
   iReturn = pasSetTcpFunName("ncsAccountPassList", ncsAccountPassList, 0);
   
   iReturn = pasSetTcpFunName("ncsCaseRuleSearch",ncsCaseRuleSearch, 0);
   
   iReturn = pasSetTcpFunName("ncs_offline_warnset",ncs_offline_warnset, 0);
   iReturn=pasSetTcpFunName("ncs_offline_warnsetsave",ncs_offline_warnsetsave,0);
   iReturn=pasSetTcpFunName("ncs_offline_warntest",ncs_offline_warntest,0);
   iReturn=pasSetTcpFunName("ncsShowOfflinelog",ncsShowOfflinelog,0);
   iReturn=pasSetTcpFunName("ncWebBBSLog",ncWebBBSLog,0);
   
   iReturn=pasSetTcpFunName("ncsSendPatfile",ncsSendPatfile,0);
   iReturn=pasSetTcpFunName("ncsReqPatVersion",ncsReqPatVersion,0);
   iReturn=pasSetTcpFunName("ncRecvSmbInfo",ncRecvSmbInfo,0);
   iReturn=pasSetTcpFunName("ncsReqAuthMobile",ncsReqAuthMobile,0);   
   iReturn=pasSetTcpFunName("ncsReqModMobilePasswd",ncsReqModMobilePasswd,0);
   iReturn=pasSetTcpFunName("ncsBackMobileInfo",ncsBackMobileInfo,0);
   iReturn=pasSetTcpFunName("ncsRecvMobileFile",ncsRecvMobileFile,0);
   iReturn=pasSetTcpFunName("ncsMobileFileOver",ncsMobileFileOver,0);
   
   iReturn=pasSetTcpFunName("ncWebBBSShow",ncWebBBSShow,0);
   
   iReturn=pasSetTcpFunName("ncsWebBMZD",ncsWebBMZD,0);
   iReturn=pasSetTcpFunName("ncsWebZKlist",ncsWebZKlist,0);
   iReturn=pasSetTcpFunName("ncsWebHYSserverSet",ncsWebHYSserverSet,0);
   iReturn=pasSetTcpFunName("ncs_smpptset",ncs_smpptset,0);
   
   iReturn=pasSetTcpFunName("ncs_smpptsetsave",ncs_smpptsetsave,0);
   iReturn=pasSetTcpFunName("ncs_smppt_test",ncs_smppt_test,0);
    iReturn=pasSetTcpFunName("ncWebShowServiceDet",ncWebShowServiceDet,0);
      
    iReturn=pasSetTcpFunName("ncsWebOnlineuserlog",ncsWebOnlineuserlog,0);
    
    iReturn=pasSetTcpFunName("ncsWebGJSXX",ncsWebGJSXX,0);
    
    iReturn=pasSetTcpFunName("ncsWebDeptCode",ncsWebDeptCode,0);
    iReturn=pasSetTcpFunName("ncsWebNetIdOneUser",ncsWebNetIdOneUser,0);
    iReturn=pasSetTcpFunName("ncsWebNetLogOneUser",ncsWebNetLogOneUser,0);
    
    iReturn=pasSetTcpFunName("ncCaseWebSiteDrNext",ncCaseWebSiteDrNext,0);
    
    iReturn=pasSetTcpFunName("case_website_imp",case_website_imp,0);
    
    iReturn=pasSetTcpFunName("ncsQQPasslist",ncsQQPasslist,0);
    iReturn=pasSetTcpFunName("qqShowpassword",qqShowpassword,0);
    
    
    		iReturn=pasSetTcpFunName("vpnServerPar",vpnServerPar,0); 	
		    iReturn=pasSetTcpFunName("vpnServerSave_par",vpnServerSave_par,0); 
		    
		    iReturn=pasSetTcpFunName("vpnClientPar",vpnClientPar,0); 
		    iReturn=pasSetTcpFunName("vpnClientSave_par",vpnClientSave_par,0); 
		    
		      
		    iReturn=pasSetTcpFunName("vpnDrNext",vpnDrNext,0); 
		    iReturn=pasSetTcpFunName("vpnGateToGateConnect",vpnGateToGateConnect,0); 
		    iReturn=pasSetTcpFunName("vpnAddGateConnect",vpnAddGateConnect,0); 
		    iReturn=pasSetTcpFunName("vpnGate2GateSave_par",vpnGate2GateSave_par,0); 
		    iReturn=pasSetTcpFunName("vpnClientCnf",vpnClientCnf,0); 
		    
		    iReturn=pasSetTcpFunName("vpnGenClientCnf",vpnGenClientCnf,0);
		    iReturn=pasSetTcpFunName("vpnuserlist",vpnuserlist,0);
		    iReturn=pasSetTcpFunName("vpnAddUser",vpnAddUser,0);
		    iReturn=pasSetTcpFunName("vpnUserSave",vpnUserSave,0);
		    iReturn=pasSetTcpFunName("vpnuseractive",vpnuseractive,0);
		    
		    iReturn=pasSetTcpFunName("vpnclientnet",vpnclientnet,0);
		    iReturn=pasSetTcpFunName("vpnClientNetAdd",vpnClientNetAdd,0);
		    iReturn=pasSetTcpFunName("vpnClientNetSave",vpnClientNetSave,0);
    		
    		iReturn=pasSetTcpFunName("ncsWebQQSearch",ncsWebQQSearch,0);
    		iReturn=pasSetTcpFunName("ncsDispMtoQQ",ncsDispMtoQQ,0);
    		iReturn=pasSetTcpFunName("ncsDispQQCont",ncsDispQQCont,0);
    		
    		iReturn=pasSetTcpFunName("ncsDispQQLoginExit",ncsDispQQLoginExit,0);
    		iReturn=pasSetTcpFunName("ncsWebMacSearch",ncsWebMacSearch,0);
    		iReturn=pasSetTcpFunName("ncsDispMacUser",ncsDispMacUser,0);
    		iReturn=pasSetTcpFunName("ncsDispMacLoginExit",ncsDispMacLoginExit,0);
    		iReturn=pasSetTcpFunName("ncsWebOthIMSearch",ncsWebOthIMSearch,0);
    		iReturn=pasSetTcpFunName("ncsWebBBSVidSearch",ncsWebBBSVidSearch,0);
    		iReturn=pasSetTcpFunName("ncsWebShowNetidBt",ncsWebShowNetidBt,0);
    		iReturn=pasSetTcpFunName("ncsWebBBSform",ncsWebBBSform,0);
    		iReturn=pasSetTcpFunName("ncsImDet_Con",ncsImDet_Con,0);
    		iReturn=pasSetTcpFunName("ncsImStat",ncsImStat,0);
    		iReturn=pasSetTcpFunName("ncsWebGameVidSearch",ncsWebGameVidSearch,0);
    		iReturn=pasSetTcpFunName("ncsWebFTPVidSearch",ncsWebFTPVidSearch,0);
    		iReturn=pasSetTcpFunName("ncsWebMailVidSearch",ncsWebMailVidSearch,0);
    		iReturn=pasSetTcpFunName("ncsWebUplogStat",ncsWebUplogStat,0);
    		iReturn=pasSetTcpFunName("ncsDispCorpUplog",ncsDispCorpUplog,0);
    		iReturn=pasSetTcpFunName("ncsSearchLog",ncsSearchLog,0);
    		iReturn=pasSetTcpFunName("ncsWebAccountMail",ncsWebAccountMail,0);
    		
    		iReturn=pasSetTcpFunName("ncsSesIplog",ncsSesIplog,0);
    		iReturn=pasSetTcpFunName("ncsSesHttplog",ncsSesHttplog,0);
    		
    		iReturn=pasSetTcpFunName("ncsSesMaillog",ncsSesMaillog,0);
    		iReturn=pasSetTcpFunName("ncsSesBBSform",ncsSesBBSform,0);
    		
    		iReturn=pasSetTcpFunName("ncsSesIm",ncsSesIm,0);
    		iReturn=pasSetTcpFunName("ncsSesSearch",ncsSesSearch,0);
    		
    		iReturn=pasSetTcpFunName("ncsWebLimWebList2",ncsWebLimWebList2,0);
    		iReturn=pasSetTcpFunName("ncReportHis",ncReportHis,0);
    		iReturn=pasSetTcpFunName("ncXzReportHis",ncXzReportHis,0);
    		
    		iReturn=pasSetTcpFunName("ncsWebStatVidAll",ncsWebStatVidAll,0);
    		iReturn=pasSetTcpFunName("ncsYjcx_Con",ncsYjcx_Con,0);
    		iReturn=pasSetTcpFunName("ncsWebNetIdYjcx",ncsWebNetIdYjcx,0);
    		
    		iReturn=pasSetTcpFunName("ncsWebMacYjcx",ncsWebMacYjcx,0);
    		iReturn=pasSetTcpFunName("NcsBBSMon_syj",NcsBBSMon_syj,0);
    		iReturn=pasSetTcpFunName("NcsImLog_syj",NcsImLog_syj,0);
    		iReturn=pasSetTcpFunName("NcsMailMon_syj",NcsMailMon_syj,0);
    		
    		iReturn=pasSetTcpFunName("ncsWebStatBBSAll",ncsWebStatBBSAll,0);
    		iReturn=pasSetTcpFunName("ncsWebStatImAll",ncsWebStatImAll,0);
    		iReturn=pasSetTcpFunName("ncsWebStatMailAll",ncsWebStatMailAll,0);
    		
    		iReturn=pasSetTcpFunName("ncsWebruleSet_zd",ncsWebruleSet_zd,0);
    		iReturn=pasSetTcpFunName("ncsVidruleSet_zd",ncsVidruleSet_zd,0);
    		iReturn=pasSetTcpFunName("ncsMailruleSet_zd",ncsMailruleSet_zd,0);
    		iReturn=pasSetTcpFunName("ncsjstxruleSet_zd",ncsjstxruleSet_zd,0);
    		
    		iReturn=pasSetTcpFunName("ncsBbsruleSet",ncsBbsruleSet,0);
    		
    		iReturn=pasSetTcpFunName("ncsdefaultruleSet_zd",ncsdefaultruleSet_zd,0);
    		iReturn=pasSetTcpFunName("ncsAllruleSet_zd",ncsAllruleSet_zd,0);
    		iReturn=pasSetTcpFunName("ncsWebTasklist",ncsWebTasklist,0);
    		iReturn=pasSetTcpFunName("ncsWebTaskDetlist",ncsWebTaskDetlist,0);
    		 
    		iReturn=pasSetTcpFunName("ncsTaskdet_Add",ncsTaskdet_Add,0);
    		iReturn=pasSetTcpFunName("ncsTaskdet_Edit",ncsTaskdet_Edit,0);
    		
    		iReturn=pasSetTcpFunName("ncsWebStatVidAllByUser",ncsWebStatVidAllByUser,0);
    		iReturn=pasSetTcpFunName("ncsDispUserSearchlog",ncsDispUserSearchlog,0);
    		
    		iReturn=pasSetTcpFunName("ncs_Areaonline_Stat_xm",ncs_Areaonline_Stat_xm,0);
    		iReturn=pasSetTcpFunName("ncsWebExcepDept",ncsWebExcepDept,0);
    		iReturn=pasSetTcpFunName("ncsWebTreeDispGroupCorp_nolimit",ncsWebTreeDispGroupCorp_nolimit,0);
    		
    		iReturn=pasSetTcpFunName("ncs_Areaonline_AvStat_xm",ncs_Areaonline_AvStat_xm,0);
    		iReturn=pasSetTcpFunName("ncsReqSendMobilePasswd",ncsReqSendMobilePasswd,0);
    		iReturn=pasSetTcpFunName("ncsWebTreeDispCorp_user",ncsWebTreeDispCorp_user,0);
    		
    		iReturn=pasSetTcpFunName("ncsWebTreeDispCorp_zd",ncsWebTreeDispCorp_zd,0);
    		iReturn=pasSetTcpFunName("ncsWebNetLog_zd",ncsWebNetLog_zd,0);
    		
    		iReturn=pasSetTcpFunName("ncWebBBSform",ncWebBBSform,0);
    		
    		iReturn=pasSetTcpFunName("ncsMovTelDicSet",ncsMovTelDicSet,0);
    		
    		iReturn=pasSetTcpFunName("ncsWebVidSearch_Exp",ncsWebVidSearch_Exp,0);
    		
    		iReturn=pasSetTcpFunName("ncsMacAddStat",ncsMacAddStat,0);
    		iReturn=pasSetTcpFunName("ncsSendMobilePasswd_cy",ncsSendMobilePasswd_cy,0);
    		
    		iReturn=pasSetTcpFunName("ncsReqSendJGUser",ncsReqSendJGUser,0);
    		
		iReturn=pasSetTcpFunName("ncs_idc_net_show",ncs_idc_net_show,0);
		iReturn=pasSetTcpFunName("ncs_idc_net_locus",ncs_idc_net_locus,0);
		iReturn=pasSetTcpFunName("ncs_idc_webpage_search",ncs_idc_webpage_search,0);
		iReturn=pasSetTcpFunName("ncs_idc_alarm_search",ncs_idc_alarm_search,0);
		iReturn=pasSetTcpFunName("ncsIdcSearch",ncsIdcSearch,0);
		iReturn=pasSetTcpFunName("ncsIdcSetKey",ncsIdcSetKey,0);
		iReturn=pasSetTcpFunName("ncsIdcRule_maintain",ncsIdcRule_maintain,0);
		iReturn=pasSetTcpFunName("ncsIdcRlue_am",ncsIdcRlue_am,0);
		iReturn=pasSetTcpFunName("ncsIdcRulesSave",ncsIdcRulesSave,0);
		iReturn=pasSetTcpFunName("ncsWebTreeIdcrule_nolimit",ncsWebTreeIdcrule_nolimit,0);
		iReturn=pasSetTcpFunName("ncsIDC_RulekeyDrNext",ncsIDC_RulekeyDrNext,0);
		iReturn=pasSetTcpFunName("ncs_Idc_key_imp",ncs_Idc_key_imp,0);
		iReturn=pasSetTcpFunName("ncWebReplaceco_key",ncWebReplaceco_key,0);
		iReturn=pasSetTcpFunName("ncsIdcRulekey_maintain",ncsIdcRulekey_maintain,0);
		iReturn=pasSetTcpFunName("ncsIdc_watchArea",ncsIdc_watchArea,0);
    		
    iReturn=pasSetTcpFunName("suportReqTestLink",suportReqTestLink,0);		
    iReturn=pasSetTcpFunName("ncsReqSendWmdUser",ncsReqSendWmdUser,0);
    
    iReturn=pasSetTcpFunName("ncsIpSearch_ib",ncsIpSearch_ib,0);
    iReturn=pasSetTcpFunName("ncsHttpSearch_ib",ncsHttpSearch_ib,0);
   
    iReturn=pasSetTcpFunName("ncsReqSysHotelUser",ncsReqSysHotelUser,0);
    
    iReturn=pasSetTcpFunName("ncsWebFactoryCode",ncsWebFactoryCode,0);
    
    iReturn=pasSetTcpFunName("ncsWebTreeDispFactory",ncsWebTreeDispFactory,0);
    
    iReturn=pasSetTcpFunName("ncs_Factory_Stat",ncs_Factory_Stat,0);
    iReturn=pasSetTcpFunName("ncs_Type_Stat",ncs_Type_Stat,0);
    
    iReturn=pasSetTcpFunName("ncKeywordDrNext",ncKeywordDrNext,0);
    iReturn=pasSetTcpFunName("ncskeyword_imp",ncskeyword_imp,0);
    
    iReturn=pasSetTcpFunName("ncsUserlogSearch_ib",ncsUserlogSearch_ib,0);
    iReturn=pasSetTcpFunName("ncsFormonSearch_ib",ncsFormonSearch_ib,0);    
    iReturn=pasSetTcpFunName("ncsMailSearch_ib",ncsMailSearch_ib,0);
    
    iReturn=pasSetTcpFunName("ncsImSearch_ib",ncsImSearch_ib,0);
    iReturn=pasSetTcpFunName("ncsSearchLog_ib",ncsSearchLog_ib,0);
    iReturn=pasSetTcpFunName("ncsVidSearch_ib",ncsVidSearch_ib,0);
    
    iReturn=pasSetTcpFunName("ncsReqCenterSms",ncsReqCenterSms,0);
    
    iReturn=pasSetTcpFunName("ncsWebModiSomeClientBm",ncsWebModiSomeClientBm,0);
    iReturn=pasSetTcpFunName("ncsWebSiteRegLog",ncsWebSiteRegLog,0);
    iReturn=pasSetTcpFunName("ncsWebSiteLoginLog",ncsWebSiteLoginLog,0);
    iReturn=pasSetTcpFunName("WebShowpassword",WebShowpassword,0);
    
 //新版本注册函数   
    		iReturn=pasSetTcpFunName("ncsWebClientSearch_v4",ncsWebClientSearch_v4,0);
    		iReturn=pasSetTcpFunName("ncsWebDispClientAddForm_v4",ncsWebDispClientAddForm_v4,0);
    		iReturn=pasSetTcpFunName("ncsWebClientAdd_v4",ncsWebClientAdd_v4,0);
    		iReturn=pasSetTcpFunName("ncsTreeGroup_v4",ncsTreeGroup_v4,0);    		
    		iReturn=pasSetTcpFunName("ncsShowServiceCorpDet",ncsShowServiceCorpDet,0);
    		iReturn=pasSetTcpFunName("ncsWebGroupComp",ncsWebGroupComp,0);    		
    		iReturn=pasSetTcpFunName("ncsCorpIp_v4",ncsCorpIp_v4,0);
    		iReturn=pasSetTcpFunName("ncsWebClientClassList_v4",ncsWebClientClassList_v4,0);    		
    		iReturn=pasSetTcpFunName("ncsWebDispDeptForm_v4",ncsWebDispDeptForm_v4,0);
    		
    		iReturn=pasSetTcpFunName("ncsTreeUser_v4",ncsTreeUser_v4,0);
    		iReturn=pasSetTcpFunName("ncsUserlogSearch_v4",ncsUserlogSearch_v4,0);
    		iReturn=pasSetTcpFunName("ncs_onlinecorp_first_pie",ncs_onlinecorp_first_pie,0);
    		iReturn=pasSetTcpFunName("ncsWebGroupComp0",ncsWebGroupComp0,0);
    		iReturn=pasSetTcpFunName("ncs_onlinecorp_first_onlinerate",ncs_onlinecorp_first_onlinerate,0);
    		iReturn=pasSetTcpFunName("ncsWarnInfoOnline_v4",ncsWarnInfoOnline_v4,0);
    		iReturn=pasSetTcpFunName("ncsCaseList_v4",ncsCaseList_v4,0);
    	  iReturn=pasSetTcpFunName("ncsTreeGroup_comp_v4",ncsTreeGroup_comp_v4,0);
    		
        iReturn=pasSetTcpFunName("ncsCaseDet_netid_v4",ncsCaseDet_netid_v4,0);
        iReturn=pasSetTcpFunName("ncsWebNettypeComp",ncsWebNettypeComp,0);
        
        iReturn=pasSetTcpFunName("ncsWebSitetypeComp",ncsWebSitetypeComp,0);
        iReturn=pasSetTcpFunName("ncsCaseDet_website_v4",ncsCaseDet_website_v4,0);
        iReturn=pasSetTcpFunName("ncsCaseDet_user_v4",ncsCaseDet_user_v4,0);
        iReturn=pasSetTcpFunName("ncsCaseDet_keyword_v4",ncsCaseDet_keyword_v4,0);
        iReturn=pasSetTcpFunName("ncsKeywordtypeComp",ncsKeywordtypeComp,0);
        iReturn=pasSetTcpFunName("ncCaseKeywodDrNext",ncCaseKeywodDrNext,0);
        iReturn=pasSetTcpFunName("case_keyword_imp",case_keyword_imp,0);
        iReturn=pasSetTcpFunName("ncsSenserviceComp",ncsSenserviceComp,0);
        iReturn=pasSetTcpFunName("ncsCaseDet_service_v4",ncsCaseDet_service_v4,0);
        iReturn=pasSetTcpFunName("ncsCaseDet_ipport_v4",ncsCaseDet_ipport_v4,0);
        
        iReturn=pasSetTcpFunName("ncsMobileComp",ncsMobileComp,0);
        iReturn=pasSetTcpFunName("ncsCaseDet_warnset_v4",ncsCaseDet_warnset_v4,0);
        iReturn=pasSetTcpFunName("ncsCase_xflog_v4",ncsCase_xflog_v4,0);
        iReturn=pasSetTcpFunName("ncsCaseArea_v4",ncsCaseArea_v4,0);
        
        iReturn=pasSetTcpFunName("ncsCaseDet_search_v4",ncsCaseDet_search_v4,0);
        iReturn=pasSetTcpFunName("ncsTreeKeyword_v4",ncsTreeKeyword_v4,0);
        iReturn=pasSetTcpFunName("ncsCase_keywordDet_v4",ncsCase_keywordDet_v4,0);
        iReturn=pasSetTcpFunName("ncsSysUserlist_v4",ncsSysUserlist_v4,0);
        iReturn=pasSetTcpFunName("ncsRolelist_v4",ncsRolelist_v4,0);
        iReturn=pasSetTcpFunName("ncsDispRoleFunlist_v4",ncsDispRoleFunlist_v4,0);
        iReturn=pasSetTcpFunName("ncsSysUserRoleConfig_v4",ncsSysUserRoleConfig_v4,0);
        
        iReturn=pasSetTcpFunName("ncsSysReflesh_v4",ncsSysReflesh_v4,0);
        
        iReturn=pasSetTcpFunName("ncsSysloglist_v4",ncsSysloglist_v4,0);
        iReturn=pasSetTcpFunName("ncsEndUserlist_v4",ncsEndUserlist_v4,0);
       
        iReturn=pasSetTcpFunName("ncsTreeSnetid_v4",ncsTreeSnetid_v4,0);
        iReturn=pasSetTcpFunName("ncsSentive_NetidDet_v4",ncsSentive_NetidDet_v4,0);
        
        iReturn=pasSetTcpFunName("ncSenVidDrNext",ncSenVidDrNext,0);
        iReturn=pasSetTcpFunName("ncsSenVid_imp",ncsSenVid_imp,0);
        
        iReturn=pasSetTcpFunName("ncsTreeUrl_v4",ncsTreeUrl_v4,0);
        iReturn=pasSetTcpFunName("ncsSen_UrlDet_v4",ncsSen_UrlDet_v4,0);
        iReturn=pasSetTcpFunName("ncsSen_CCICDet_v4",ncsSen_CCICDet_v4,0);
        iReturn=pasSetTcpFunName("ncsSen_UserDet_v4",ncsSen_UserDet_v4,0);
        iReturn=pasSetTcpFunName("ncSentiveUserDrNext",ncSentiveUserDrNext,0);
        iReturn=pasSetTcpFunName("ncs_sentiveuser_imp",ncs_sentiveuser_imp,0);
        
        iReturn=pasSetTcpFunName("ncsKewordlog_v4",ncsKewordlog_v4,0);
        iReturn=pasSetTcpFunName("ncsSenUrllog_v4",ncsSenUrllog_v4,0);
        
        iReturn=pasSetTcpFunName("ncsSenNetidlog_v4",ncsSenNetidlog_v4,0);
        iReturn=pasSetTcpFunName("ncsSenUserlog_v4",ncsSenUserlog_v4,0);
        
        iReturn=pasSetTcpFunName("ncsVid_v4",ncsVid_v4,0);
        iReturn=pasSetTcpFunName("ncsMacVid_v4",ncsMacVid_v4,0);
        iReturn=pasSetTcpFunName("ncsWebShowUserBh",ncsWebShowUserBh,0);
        
        iReturn=pasSetTcpFunName("ncsUserHttpStat_v4",ncsUserHttpStat_v4,0);
        iReturn=pasSetTcpFunName("ncsUserHttpDet_v4",ncsUserHttpDet_v4,0);
        
        iReturn=pasSetTcpFunName("ncsUserSearchStat_v4",ncsUserSearchStat_v4,0);
        iReturn=pasSetTcpFunName("ncsUserSearchDet_v4",ncsUserSearchDet_v4,0);
        
        iReturn=pasSetTcpFunName("ncsUserBbsStat_v4",ncsUserBbsStat_v4,0);
        iReturn=pasSetTcpFunName("ncsUserBbsDet_v4",ncsUserBbsDet_v4,0);
        iReturn=pasSetTcpFunName("ncsUserImStat_v4",ncsUserImStat_v4,0);
        iReturn=pasSetTcpFunName("ncsImdetcontlist",ncsImdetcontlist,0);
        iReturn=pasSetTcpFunName("ncsUserMailDet_v4",ncsUserMailDet_v4,0);
        
        iReturn=pasSetTcpFunName("ncsUserNetidStat_v4",ncsUserNetidStat_v4,0);
        iReturn=pasSetTcpFunName("ncsUserNetidDet_v4",ncsUserNetidDet_v4,0);
        
        iReturn=pasSetTcpFunName("ncsTreeKeyword_yh_v4",ncsTreeKeyword_yh_v4,0);
        iReturn=pasSetTcpFunName("ncsTreeUrl_yh_v4",ncsTreeUrl_yh_v4,0);
        iReturn=pasSetTcpFunName("ncsTreeNetid_yh_v4",ncsTreeNetid_yh_v4,0);
        
        iReturn=pasSetTcpFunName("ncsUserlogStatByday_v4",ncsUserlogStatByday_v4,0);
        iReturn=pasSetTcpFunName("ncsShowZhtj_v4",ncsShowZhtj_v4,0);
        
        iReturn=pasSetTcpFunName("ncsFacCode_v4",ncsFacCode_v4,0);
        
        iReturn=pasSetTcpFunName("ncsWebFacComp",ncsWebFacComp,0);
        iReturn=pasSetTcpFunName("ncsTreeFac_v4",ncsTreeFac_v4,0);
        iReturn=pasSetTcpFunName("ncsTestRight_v4",ncsTestRight_v4,0);
        
        iReturn=pasSetTcpFunName("ncsTreeCase_v4",ncsTreeCase_v4,0);
        iReturn=pasSetTcpFunName("ncsTreeCase_yh_v4",ncsTreeCase_yh_v4,0);
        iReturn=pasSetTcpFunName("ncsLogistlog_v4",ncsLogistlog_v4,0);
        iReturn=pasSetTcpFunName("ncsShowOneMail_v4",ncsShowOneMail_v4,0);
        iReturn=pasSetTcpFunName("ncsWebMailCtlOutHtm_v4",ncsWebMailCtlOutHtm_v4,0);
        
        iReturn=pasSetTcpFunName("ncsWebPostRead_v4",ncsWebPostRead_v4,0);
        iReturn=pasSetTcpFunName("ncsMailCtlDownload_v4",ncsMailCtlDownload_v4,0);
        iReturn=pasSetTcpFunName("ncsWebDispXxztZgt_v4",ncsWebDispXxztZgt_v4,0);
        iReturn=pasSetTcpFunName("ncsTreeKeyword_log_v4",ncsTreeKeyword_log_v4,0);
        
        iReturn=pasSetTcpFunName("ncsTreeUrl_log_v4",ncsTreeUrl_log_v4,0);
        iReturn=pasSetTcpFunName("ncsTreeNetid_log_v4",ncsTreeNetid_log_v4,0);
        
        iReturn=pasSetTcpFunName("ncsTreeSenuser_log_v4",ncsTreeSenuser_log_v4,0);
        
        iReturn=pasSetTcpFunName("ncsDispTool_v4",ncsDispTool_v4,0);
        iReturn=pasSetTcpFunName("ncsDispToolSave_v4",ncsDispToolSave_v4,0);
        iReturn=pasSetTcpFunName("ncsMgcl_stat_v4",ncsMgcl_stat_v4,0);
        
        iReturn=pasSetTcpFunName("ncsVid_stat_v4",ncsVid_stat_v4,0);
        iReturn=pasSetTcpFunName("ncsVid_statdet_v4",ncsVid_statdet_v4,0);
        iReturn=pasSetTcpFunName("ncsWebKeywordComp",ncsWebKeywordComp,0);
        iReturn=pasSetTcpFunName("ncRecvSmbInfo_new",ncRecvSmbInfo_new,0);
        iReturn=pasSetTcpFunName("ncsTreeNettype_comp_v4",ncsTreeNettype_comp_v4,0);
        iReturn=pasSetTcpFunName("ncsLbCode_v4",ncsLbCode_v4,0);
        
        iReturn=pasSetTcpFunName("ncsTreeLb_v4",ncsTreeLb_v4,0);
        iReturn=pasSetTcpFunName("ncsWebLbComp",ncsWebLbComp,0);
        iReturn=pasSetTcpFunName("ncsTreeUserLb_v4",ncsTreeUserLb_v4,0);
        
        iReturn=pasSetTcpFunName("ncsKeywordXf_v4",ncsKeywordXf_v4,0);
        iReturn=pasSetTcpFunName("ncsTreeUserKeyword_v4",ncsTreeUserKeyword_v4,0);
        iReturn=pasSetTcpFunName("ncsTreeRpTask_v4",ncsTreeRpTask_v4,0);
        
        iReturn=pasSetTcpFunName("ncsSendMobileMesg_cy",ncsSendMobileMesg_cy,0);
        iReturn=pasSetTcpFunName("ncsFuUpload_v4",ncsFuUpload_v4,0);
        
         iReturn=pasSetTcpFunName("ncsWebTaskDetlist_v4",ncsWebTaskDetlist_v4,0);
         
         iReturn=pasSetTcpFunName("ncReportHis_v4",ncReportHis_v4,0);
         iReturn=pasSetTcpFunName("ncsSendMMesg_sms",ncsSendMMesg_sms,0);
         iReturn=pasSetTcpFunName("ncsSendPasswd_sms",ncsSendPasswd_sms,0);
         
         iReturn=pasSetTcpFunName("ncsSendAd_Stat",ncsSendAd_Stat,0);
         
         iReturn=pasSetTcpFunName("ncHttpLogin",ncHttpLogin,0);
         iReturn=pasSetTcpFunName("ncHttpLogout",ncHttpLogout,0);
         
         iReturn=pasSetTcpFunName("ncsGetHttpUser",ncsGetHttpUser,0);
         
         iReturn=pasSetTcpFunName("ncsReqCenterHotelUser",ncsReqCenterHotelUser,0);
         iReturn=pasSetTcpFunName("ncs_lbonlinerate",ncs_lbonlinerate,0);
    
         iReturn=pasSetTcpFunName("ncsReqSendJGUser_S",ncsReqSendJGUser_S,0);
         iReturn=pasSetTcpFunName("ncsReqMobileNext",ncsReqMobileNext,0);
         iReturn=pasSetTcpFunName("ncHttpReg",ncHttpReg,0);
         iReturn=pasSetTcpFunName("ncsWebAuditList_v4",ncsWebAuditList_v4,0);
         iReturn=pasSetTcpFunName("ncsGroupCorpComp",ncsGroupCorpComp,0);
          iReturn=pasSetTcpFunName("ncsDateComp",ncsDateComp,0);
          iReturn=pasSetTcpFunName("ncsUrlidComp",ncsUrlidComp,0);
          iReturn=pasSetTcpFunName("ncsAuditSave_v8",ncsAuditSave_v8,0);
          iReturn=pasSetTcpFunName("ncsGetGatawayIp",ncsGetGatawayIp,0);
          iReturn=pasSetTcpFunName("ncsProcerror_v4",ncsProcerror_v4,0);
          
          iReturn=pasSetTcpFunName("ncMailSendSet_par",ncMailSendSet_par,0);
          iReturn=pasSetTcpFunName("ncMailSendSave_par",ncMailSendSave_par,0);
          iReturn=pasSetTcpFunName("nc_opentest_mail",nc_opentest_mail,0);
          iReturn=pasSetTcpFunName("nc_setest_mail",nc_setest_mail,0);
          
           iReturn=pasSetTcpFunName("ncWebBBSLog_utf8",ncWebBBSLog_utf8,0);
           iReturn=pasSetTcpFunName("ncskid_user_v4",ncskid_user_v4,0);
           iReturn=pasSetTcpFunName("ncsUsbkeyloglist_v4",ncsUsbkeyloglist_v4,0);
           iReturn=pasSetTcpFunName("ncsGetCorpInfo",ncsGetCorpInfo,0);
           iReturn=pasSetTcpFunName("ncsCommitCorpInfo",ncsCommitCorpInfo,0);
           iReturn=pasSetTcpFunName("ncsScanMaclist_v4",ncsScanMaclist_v4,0);
           iReturn=pasSetTcpFunName("ncsNotifySyscorp",ncsNotifySyscorp,0);
           iReturn=pasSetTcpFunName("ncsSyscorp",ncsSyscorp,0);
           iReturn=pasSetTcpFunName("ncsCheckstate",ncsCheckstate,0);
           iReturn=pasSetTcpFunName("ncHttpUserReg",ncHttpUserReg,0);
           iReturn=pasSetTcpFunName("ncHttpApReg",ncHttpApReg,0);
           iReturn=pasSetTcpFunName("ncHttpDetect",ncHttpDetect,0);
          //搜索引擎
/*           iReturn=pasSetTcpFunName("ncsbbsmon_v4",ncsbbsmon_v4,0);
           iReturn=pasSetTcpFunName("ncsimlog_v4",ncsimlog_v4,0);
           
           iReturn=pasSetTcpFunName("ncsshowfield_v4",ncsshowfield_v4,0);
           iReturn=pasSetTcpFunName("ncsmailmon_v4",ncsmailmon_v4,0);
           
           iReturn=pasSetTcpFunName("ncsformmon_v4",ncsformmon_v4,0);
           iReturn=pasSetTcpFunName("ncsfulltext_v4",ncsfulltext_v4,0);
          
           iReturn=pasSetTcpFunName("ncgetattfile_v4",ncgetattfile_v4,0);
           
           iReturn=pasSetTcpFunName("ncsbbsFulltext_v4",ncsbbsFulltext_v4,0);
           iReturn=pasSetTcpFunName("ncsMailFulltext_v4",ncsMailFulltext_v4,0);
*/
  //快速查询
           iReturn=pasSetTcpFunName("ncsQuickSearch_sxx",ncsQuickSearch_sxx,0);
           iReturn=pasSetTcpFunName("ncsQuickSearch_netidlog",ncsQuickSearch_netidlog,0);
           iReturn=pasSetTcpFunName("ncsQuickSearch_httplog",ncsQuickSearch_httplog,0);
           iReturn=pasSetTcpFunName("ncsQuickSearch_searchlog",ncsQuickSearch_searchlog,0);
           iReturn=pasSetTcpFunName("ncsQuickSearch_bbslog",ncsQuickSearch_bbslog,0);
           iReturn=pasSetTcpFunName("ncsQuickSearch_maillog",ncsQuickSearch_maillog,0);
   //统计 
           iReturn=pasSetTcpFunName("ncs_user_log", ncs_user_log, 0);
           iReturn=pasSetTcpFunName("ncs_user_log_wif", ncs_user_log_wif, 0);
           
           iReturn=pasSetTcpFunName("ncsWebClientSearch_wif", ncsWebClientSearch_wif, 0);
           iReturn=pasSetTcpFunName("ncsWebDispClientAddForm_wif", ncsWebDispClientAddForm_wif, 0);
           iReturn=pasSetTcpFunName("ncsWebClientAdd_wif", ncsWebClientAdd_wif, 0);
           iReturn=pasSetTcpFunName("proauth_statusparset", proauth_statusparset, 0);
           iReturn=pasSetTcpFunName("proauth_statusparsave", proauth_statusparsave, 0);
           iReturn=pasSetTcpFunName("ncs_org_on_line_log", ncs_org_on_line_log, 0);
           
    return 0;
}


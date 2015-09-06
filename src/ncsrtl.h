#ifndef __NCSRTLDEF__
#define __NCSRTLDEF__
/* ncs003.c */
int ncsTest2006(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebAuth(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDispModiPass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebModiPass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/* ncs004.c */
int ncsWebClientSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDispClientAddForm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebClientAdd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebDelSomeClient(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebClientOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebClientGoNc(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsWebAdminUserList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebAdminDispUserForm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsAdminUserSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebAdminRoleList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsAdminRoleModi(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebAdminRole(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/* ncs005.c */
int ncsNcLogin(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNcActive(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSyncImFilter(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsSyncImWarning(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
/* ncs006.c */
int ncsWebErrHsearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebErrDispSform(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebErrNewLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebImFilterSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/* ncs009.c */
int ncsWebShowProcess(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebShowOnTime(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/*ncsweb000.c*/
int ncWebTreeDispClass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebTreeDispCorp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncCorpClassAddForm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebReplace(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsIpruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsAllruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsMailruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsjstxruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsformruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsftpruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsdefaultruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
/*ncsweb004*/

int ncswarnWebruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsAllwarnruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsMailwarnruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsjstxwarnruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsformwarnruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsvistNetRulSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWarnClass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWarnClassAddForm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWarnNotfyAddForm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLimDateList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//ncsweb004-1.c
int ncsWebSentiveList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSentiveGroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSentiveEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSiteBbsEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSiteBbsGroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSiteBbsList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsWebLimDateGroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLimDateGroupEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLimIpList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLimIpGroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLimIpGroupEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLimIpPolList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLim_SetIpPol(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLimIpPolEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsWebLimWebClass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLimWebList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebWebPolClass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLim_DispWebPo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebWebPolList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int mailclasslist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int mailaddrlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_userclasslist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_useridlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//ncsweb002.c
int ncsWebCorpDep(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebCorpDepSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebCorpUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebNetId(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebNetLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebShowUserinfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebNetIdLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebIpLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebWebLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebSelectZd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//ncsweb002-1.c
int ncsWebMailLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebFormLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebImLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebFobLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebWarnLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebLoginLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebMailMonRead(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebMailCtlOutHtm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebMailCtlDownload(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUtlMailReadSelf(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncsWebPostRead(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebMailMonCvtCode(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNetidGroupEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_netidclass_mod(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
//ncsweb001.c
int ncsWebMailOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebImOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebFtpOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebWebVistOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebNetVistOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebWarnOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//ncsweb001-1.c
int ncsWebLoginOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebErrOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebFormOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebNetServiceOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsIPvistOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//ncsweb005.c
int ncsWebShowSystemInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebAdmQyeSysLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebShowDataBack(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebBackFileDownload(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDelMailBack(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebBackFileDownload(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebShutdown(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//ncsweb000-1.c
int ncsWebSystemSet_back(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSystemSave_back(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSystemSet_clean(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncsWebSystemSave_clean(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSystemSet_par(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncsWebSystemSave_par(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//ncsweb003.c
int ncsDispWarnStaCon(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_warnbycorp_stat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_warnbyclassname_stat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_warnbydate_stat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_errbycorp_stat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_errbydip_stat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebRefresh(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNetSesLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_web_stat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_ip_stat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//ncsweb006.c
int ncsWebTreeDispDep(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebTreeDispUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispNetAccount(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNetAccount(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebTreeDispCompt(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebOnlineSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDispOnlineCorp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncWebDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int webclass_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebLimWebClassCx(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebSelectClass(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDispNetIdAdd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNetidAddSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispNetidIframe(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseBaseInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseAddSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseWatch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseWatchArea(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseWarnSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseWatchStat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebSelectWarnNotfy(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseWarnNotySave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebCaseSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseCtlStat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncCaseImDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int case_im_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncCaseUserDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int case_user_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebTracUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebShowFirstLeft(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebShowFirstRight(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsShowMainCorpOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWarnInfoOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCaseWarnlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebWarnSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCorpIp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsWebZdjkUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebZdjkSet_rule(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncsWebShowGjNext(utShmHead *psShmHead,int iFd,utMsgHead *psMsgHead);
int ncsGjUserlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsWebRuleQuery(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNoAutoBackup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebBackRestore(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebDispSyslogSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsvidwarnruleSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsClassbase_stat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_Areaonline_Stat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsRealUserStat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebRealUserSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsDispNetidDet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDispCorp_new(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsNetidStat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebVidSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserNetidStat(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserNetidDet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUserNetidDet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsVistNetTrac(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebNetIdTrac(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebAuditList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispAuditAdd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsAuditSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebAdultLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncs_CCIC(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsDispRolelist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispRoleFunlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebRoleEdit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsRoleRightSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);


int ncsMovTelSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncCaseCCICDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ccic_user_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispNetidMain(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsDispNetidOth(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebAuth2(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsWebDeptModRootname_new(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebTreeDispGroup_nolimit(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDispDeptForm_new(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int wbs_selectData_ISBSList_hx(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int wbs_selectData_ServiceUserList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int wbs_ServiceStatus(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsWebDeptAdd_new(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDeptDel_new(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDeptModi_new(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncs_Areaonline_Stat_new(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSubservice(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

//ncsSearchMany.c
int ncWebSelectClass2(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebShowSearchAccessListPlate(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebSaveCondition(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsShowSearchResult(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUpdateResult(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsShowSearchResult2(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUpdateData(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebShowSearchTask(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); 
int ncsMailCommand(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsShowMailResult(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsShowSearchPreResult(utShmHead * psShmHead, int iFd, utMsgHead * psMsgHead);
int ncsWebPostRead2(utShmHead * psShmHead, int iFd, utMsgHead * psMsgHead);

//“∆∂Ø√‹±¶
int ncMbDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncMbLogin(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebMbSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncUserBindMb(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebMbManagerSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebDisplayMbManager(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsPassProcyList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsAccountPassList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncTcServiceType(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncTcServiceTypeSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebServiceTypeDet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsCaseRuleSearch(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncs_offline_warnset(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncs_offline_warnsetsave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncs_offline_warntest(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsShowOfflinelog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncWebBBSLog(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

int ncsSendPatfile(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsReqPatVersion(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncRecvSmbInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsReqAuthMobile(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncsReqModMobilePasswd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsBackMobileInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsRecvMobileFile(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsMobileFileOver(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncWebBBSShow(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

int ncsWebGJSXX(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebDeptCode(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebNetIdOneUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsWebNetLogOneUser(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

int ncCaseWebSiteDrNext(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int case_website_imp(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncskid_user_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsUsbkeyloglist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsGetCorpInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int ncsCommitCorpInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
#endif

/* include file for Oracle */
#ifndef __PQMAINECHEAD__
#define __PQMAINECHEAD__
#define PQ_SQL_NULLVALUE         -1405
#define PQ_SQL_NOTFOUND          1403
#define PQ_SQL_DUPLICATEVALUE     -1
EXEC SQL begin declare section;
    struct pqTabPquser {
        long        picqnum;
        char        username[17];
        char        password[25];
        char        email[65];
        char        realname[21];
        char        micqnum[13];
        long        curstatus;
        long        applytime;
        long        musertime;
        long        mcommtime;
        long        logintime;
        long        logincount;
        long        power;
        long        userkind;
        char        dpassword[25];
    };
     
    struct pqTabPquser_bus {
        long        picqnum;
        char        country[13];
        char        province[13];
        char        city[13];
        char        company[41];
        char        depart[21];
        char        occu[13];
        char        zipcode[7];
        char        address[65];
        char        tele[21];
        char        fax[21];
        char        bpcall[21];
        char        memo[65];
        char        homeurl[65];
        long        secrecy;
    };
    
    struct pqTabPquser_pri {
        long        picqnum;
        char        birthday[11];
        long        gender;
        char        htele[21];
        char        mtele[21];
        char        hzipcode[7];
        char        hcountry[13];
        char        hprovince[13];
        char        hcity[13];
        char        haddress[65];
        char        phomeurl[65];
        char        educate[9];
        char        school[65];
        char        gradudate[11];
        char        special[33];
        char        class[17];
        long        hsecrecy;
    };

    struct pqUserInfo {
        long        picqnum;
        char      username[17]; 
        char      email[65];
        char      realname[21];
        char      micqnum[13];
        long        power;
        long        userkind;
        char      country[13];
        char      province[13];
        char      city[13];
        char      company[41];
        char      depart[21];
        char      occu[13];
        char      zipcode[7];
        char      address[65];
        char      tele[21];
        char      fax[21];
        char      bpcall[21];
        char      memo[65];
        char      homeurl[65];
        long        secrecy;
        char      birthday[11];
        long        gender;
        char      htele[21];
        char      mtele[21];
        char      hzipcode[7];
        char      hcountry[13];
        char      hprovince[13];
        char      hcity[13];
        char      haddress[65];
        char      phomeurl[65];
        char      educate[9];
        char      school[65];
        char      gradudate[11];
        char      special[33];
        char      class[17];
        long        hsecrecy;
        long        secrecy0;
        long        opflags;    /* ²Ù×÷      */
    };
    
EXEC SQL end declare section;
typedef struct {
    int len;
    char buf[8192];
} long_varraw;
EXEC SQL type long_varraw is long varraw (8192);

#endif


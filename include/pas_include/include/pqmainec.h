#ifndef __PQMAINECHEAD__
#define __PQMAINECHEAD__

#define PQ_SQL_DUPLICATEVALUE     -268
EXEC SQL begin declare section;
    struct pqTabPquser {
        long        picqnum;
        string      username[17];
        string      password[25];
        string      email[65];
        string      realname[21];
        string      micqnum[13];
        long        curstatus;
        long        applytime;
        long        musertime;
        long        mcommtime;
        long        logintime;
        long        logincount;
        long        power;
        long        userkind;
        string      dpassword[25];
    };
     
    struct pqTabPquser_bus {
        long        picqnum;
        string      country[13];
        string      province[13];
        string      city[13];
        string      company[41];
        string      depart[21];
        string      occu[13];
        string      zipcode[7];
        string      address[65];
        string      tele[21];
        string      fax[21];
        string      bpcall[21];
        string      memo[65];
        string      homeurl[65];
        long        secrecy;
    };
    
    struct pqTabPquser_pri {
        long        picqnum;
        string      birthday[11];
        long        gender;
        string      htele[21];
        string      mtele[21];
        string      hzipcode[7];
        string      hcountry[13];
        string      hprovince[13];
        string      hcity[13];
        string      haddress[65];
        string      phomeurl[65];
        string      educate[9];
        string      school[65];
        string      gradudate[11];
        string      special[33];
        string      class[17];
        long        hsecrecy;
    };

    struct pqUserInfo {
        long        picqnum;
        string      username[17]; 
        string      email[65];
        string      realname[21];
        string      micqnum[13];
        long        power;
        long        userkind;
        string      country[13];
        string      province[13];
        string      city[13];
        string      company[41];
        string      depart[21];
        string      occu[13];
        string      zipcode[7];
        string      address[65];
        string      tele[21];
        string      fax[21];
        string      bpcall[21];
        string      memo[65];
        string      homeurl[65];
        long        secrecy;
        string      birthday[11];
        long        gender;
        string      htele[21];
        string      mtele[21];
        string      hzipcode[7];
        string      hcountry[13];
        string      hprovince[13];
        string      hcity[13];
        string      haddress[65];
        string      phomeurl[65];
        string      educate[9];
        string      school[65];
        string      gradudate[11];
        string      special[33];
        string      class[17];
        long        hsecrecy;
        long        secrecy0;
        long        opflags;    /* ²Ù×÷      */
    };
    
EXEC SQL end declare section;

#endif


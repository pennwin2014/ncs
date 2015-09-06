
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iconv.h>
#include <time.h>
#include <ctype.h>
#include <sys/time.h>
#include <curl/curl.h>
#include "sphinxclient.h"
#include "cjson.h"
 
#include "utoall.h"
#include "pasdb.h"
#include "utoplt01.h"
#include "pasutl.h"

#define SEPCHARS " .,?\"\n"

//char *iconv_convert(const char*, const char*, const char*);
void str_replace(char *, const char, const char *);
int  getCharset(char *, int);
//char *utTimFormat(char *, unsigned long);
int utMsgGetSomeNVar(utMsgHead*, int, ...);
int utPltOutToHtml(int, utMsgHead*, utPltDbHead*, char*);
int utPltPutLoopVarF(utPltDbHead*, char*, int, char *, ...);
int utPltPutLoopVar(utPltDbHead*, char*, int, char *);
int utPltPutLoopVar2(utPltDbHead* , char*, int, int, char*);
int utPltPutVarF(utPltDbHead* , char*, char*, ...);
int utPltPutLoopVarF2(utPltDbHead*, char*, int, int, char*, ...);
int utPltSetCvtHtml(int);
int utWebDispMsg(int, utMsgHead*, char*, char*, char*, ...);                      
int utPltFileDownload(int, char*, char*, char*, char*); 
char *get_ntype(int);


static char *attdir = "/usr/src/elasticsearch-rtf/elasticsearch/resources";

struct curl_data
{
	char *buf;
	int shift;
};


///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct ncHtmSkipTag_s {
    char            caTag[16];
    int             iLen;
    char            caTag1[16];
} ncHtmSkipTag;

static ncHtmSkipTag  psTagSkip[]={{"<script",7,"</script>"},
                                  {"<!",     2,">"      },
                                  {"<style", 6,"</style>" },
                                  {"\0",    0,"\0"}};
static ncHtmSkipTag  psTagSpecial[]={ 
                                  {"&nbsp;",  6," "},
                                  {"&lt;",    4,"<"},
                                  {"&gt;",    4,">"},
                                  {"&amp;",   5,"&"},
                                  {"&quot;",  6,"\""},
                                  {"\0",0,"\0"}};

static int ncHttpIsSpaces(unsigned char p)
{
        if(p == ' ' || p == '\t' || p=='\r' || p== '\n')
        {
                return 1;
        }
        else 
        {
                return 0;
        }
}

static char *iconv_convert(const char *codefrom, const char *codeto, const char *src)
{
        char    to[32] = "";
        char    *out = NULL;
        char    *dst = NULL;

        iconv_t c;
        size_t  n_in  =  strlen(src);
        size_t  n_out =  2*n_in;

        snprintf(to, sizeof(to), "%s//IGNORE", codeto);
        if( (out=(char *)calloc(n_out+1, sizeof(char))) == NULL)
        {
                printf("allocate memory for %d bytes failed\n", n_out);
                return NULL;
        }
        dst=out;

        if((c = iconv_open(to, codefrom))==(iconv_t)(-1))
        {
                printf("codefrom = %s, codeto = %s, failed\n", codefrom, codeto);
                return NULL;
        }
        iconv(c, (char **)&src, &n_in, &dst, &n_out);
        iconv_close(c);
        return out;
}


static char *pasStrCaseStr(char *pCont,char *pSub)
{
    int i,j,k;
    i=0;
    j=0;
    k=0;
    while(1) {
        if(tolower(pCont[i]) == tolower(pSub[j])) {
            i++;
            j++;
            if(pSub[j]==0) {
                return &pCont[k];
            }
            else if(pCont[i]==0) {
                return NULL;
            }
        }
        else if(pCont[i]==0) {
            return NULL;
        }
        else {
            j=0;
            k++;
            i = k;
        }
    }
    return NULL;
}

static int ncHtmlIsSkipTag(char *pIn)
{
    int i;
    for(i=0;psTagSkip[i].iLen!=0;i++) {
        if(strncasecmp(pIn,psTagSkip[i].caTag,psTagSkip[i].iLen)==0) {
            return i;
        }
    }
    return (-1);
}

static char *ncHtmlDoSkipTag(char *pStart,int iSkip)
{
    char caVar[32];
    char *p;
    sprintf(caVar,"</%s>",psTagSkip[iSkip].caTag);
    p = (char *)pasStrCaseStr(pStart,psTagSkip[iSkip].caTag1);
    if(p) {
        return p + strlen(psTagSkip[iSkip].caTag1);
    }
    else {
        return NULL;
    }
}

static char *ncHtmlDoSkipSpecial(char *pIn,char *c)
{
    int i;
    for(i=0;psTagSpecial[i].iLen>0;i++) {
        if(strncasecmp(pIn,psTagSpecial[i].caTag,psTagSpecial[i].iLen)==0) {
            (*c) =  psTagSpecial[i].caTag1[0];
            return pIn + psTagSpecial[i].iLen;
        }
    }
    // &#124;
    if(pIn[1] == '#' && pIn[5] == ';') {
        char caTemp[8];
        int l;
        memcpy(caTemp,&pIn+2,3);
        caTemp[3]=0;
        l = atol(caTemp);
        (*c) = l;
        return pIn + 6;
    }
    else {
        *c = 0;
        return pIn;
    }
}

static int ncHttpCvtHtml2Text(char *pHtml,char *pText,int iMax)
{
    char cWord;
    int iCount,iSkip;
    char *o,*p,*p1;
    o = pText;
    iCount = 0;
    p = utStrSkipSpaces(pHtml);
    while(p && (*p) && iCount < iMax) {    
        switch(*p) {
            case '<':
                if( (iSkip = ncHtmlIsSkipTag(p)) >= 0) {
                    p = ncHtmlDoSkipTag(p+1,iSkip);
                }
                else {
                    p1 = strchr(p+1,'>');
                    if(p1) {
                        p = p1 + 1;
                    }
                    else {
                        if(!ncHttpIsSpaces(*p)) {
                            o[iCount] = (*p);
                            iCount++;
                        }
                        p++;
                    }
                }
                break;
            case '&':
                p = ncHtmlDoSkipSpecial(p,&cWord);
                if(cWord > 0) {
                    if(!ncHttpIsSpaces(cWord)) {
                        o[iCount] = cWord;
                        iCount++;
                    }
                }
                else {
                    if(!ncHttpIsSpaces(*p)) {
                        o[iCount] = (*p);
                        iCount++;
                    }
                    p++;
                }
                break;
            default:
                if(!ncHttpIsSpaces(*p)) {
                    o[iCount] = (*p);
                    iCount++;
                }
                p++;
                break;
        }
    }
    o[iCount] = 0;
    return iCount;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

size_t write_data(void *ptr, size_t size, size_t nmemb, void *userp)
{
	if(nmemb == 0) return 0;
	int res_size;
	struct curl_data *data = (struct curl_data *)userp;
    	res_size = size * nmemb;
    	data->buf = realloc(data->buf, data->shift+res_size + 1);
    	memcpy(data->buf + data->shift, ptr, res_size);
    	data->buf[data->shift+res_size] = '\0';
    	data->shift += res_size;
    	return size * nmemb;
}

/*
static size_t read_data(void *ptr, size_t size, size_t nmemb, void *userp)
{
  	int res_size;
  	int len;
	struct curl_data *data = (struct curl_data *)userp;
    	res_size = size * nmemb;
    	len = strlen(data->buf + data->shift);
    	res_size = res_size > len ? len : res_size;
    	if(res_size > 0)
    	{
    		memcpy(ptr, data->buf + data->shift, res_size);
    		data->shift += res_size;
    	}
    	return res_size;
}
*/


void _utPltPutLoopVar(utPltDbHead *psDbHead, char *name, int iNum, char *value)
{
	char *tmpgbk = iconv_convert("UTF-8", "GBK", value);
	utPltPutLoopVar(psDbHead, name, iNum, tmpgbk ? tmpgbk : "");
	free(tmpgbk);
}





void _uutPltPutLoopVar(utPltDbHead *psDbHead, char *name, int iNum, char *value)
{
	char caTemp[10240];
	char *tmpgbk = iconv_convert("UTF-8", "GBK", value);
 
	if(tmpgbk){
		if(strlen(tmpgbk)>10000) return;
			utStrReplaceWith(tmpgbk,"\\","");
			utStrReplaceWith(tmpgbk," ","");
			utStrReplaceWith(tmpgbk,"@","");
			utStrReplaceWith(tmpgbk,":","");
			utStrReplaceWith(tmpgbk,"=","");
			utStrReplaceWith(tmpgbk,":","");
		pasStrCvtUcode(tmpgbk,caTemp);
printf("tmpgbk=%s\n",tmpgbk);
printf("caTemp=%s\n",caTemp);
//		printf("p=%s\n",ncsStrCvtUcode(tmpgbk,p));
	  utPltPutLoopVar(psDbHead, name, iNum,caTemp);

	free(tmpgbk);
 }
}

void _utPltPutLoopVar2(utPltDbHead *psDbHead, char *name, int m, int n, char *value)
{
	char *tmpgbk = iconv_convert("UTF-8", "GBK", value);
	utPltPutLoopVar2(psDbHead, name, m, n, tmpgbk ? tmpgbk : "");
	free(tmpgbk);
}


typedef unsigned chartype;
char* strcasestr (const char* phaystack, const char* pneedle)
{
    const unsigned char *haystack, *needle;
    chartype bl, bu, cl, cu;

    haystack = (const unsigned char *) phaystack;
    needle = (const unsigned char *) pneedle;

    bl = tolower (*needle);
    if (bl != '\0')
    {
        bu = toupper (bl);
        haystack--;/* possible ANSI violation */
        do
        {
            cl = *++haystack;
            if (cl == '\0')
                goto ret0;
        }
        while ((cl != bl) && (cl != bu));

        cl = tolower (*++needle);
        if (cl == '\0')
            goto foundneedle;
        cu = toupper (cl);
        ++needle;
        goto jin;

        for (;;)
        {                                                                                                                                                      
            chartype a;                                                                                                                                        
            const unsigned char *rhaystack, *rneedle;                                                                                                          
                                                                                                                                                               
            do                                                                                                                                                 
            {                                                                                                                                                  
                a = *++haystack;                                                                                                                               
                if (a == '\0')                                                                                                                                 
                    goto ret0;                                                                                                                                 
                if ((a == bl) || (a == bu))                                                                                                                    
                    break;                                                                                                                                     
                a = *++haystack;                                                                                                                               
                if (a == '\0')                                                                                                                                 
                    goto ret0;                                                                                                                                 
              shloop:                                                                                                                                          
                ;                                                                                                                                              
            }                                                                                                                                                  
            while ((a != bl) && (a != bu));                                                                                                                    
                                                                                                                                                               
          jin:  a = *++haystack;                                                                                                                               
            if (a == '\0')                                                                                                                                     
                goto ret0;                                                                                                                                     
                                                                                                                                                               
            if ((a != cl) && (a != cu))                                                                                                                        
                goto shloop;                                                                                                                                   
                                                                                                                                                               
            rhaystack = haystack-- + 1;                                                                                                                        
            rneedle = needle;                                                                                                                                  
            a = tolower (*rneedle);                                                                                                                            
                                                                                                                                                               
            if (tolower (*rhaystack) == (int) a)                                                                                                               
                do                                                                                                                                             
                {                                                                                                                                              
                    if (a == '\0')                                                                                                                             
                        goto foundneedle;                                                                                                                      
                    ++rhaystack;                                                                                                                               
                    a = tolower (*++needle);                                                                                                                   
                    if (tolower (*rhaystack) != (int) a)                                                                                                       
                        break;                                                                                                                                 
                    if (a == '\0')                                                                                                                             
                        goto foundneedle;                                                                                                                      
                    ++rhaystack;                                                                                                                               
                    a = tolower (*++needle);                                                                                                                   
                }                                                                                                                                              
                while (tolower (*rhaystack) == (int) a);                                                                                                       
                                                                                                                                                               
            needle = rneedle;/* took the register-poor approach */                                                                                             
                                                                                                                                                               
            if (a == '\0')                                                                                                                                     
                break;                                                                                                                                         
        }                                                                                                                                                      
    }                                                                                                                                                          
  foundneedle:                                                                                                                                                 
    return (char*) haystack;                                                                                                                                   
  ret0:                                                                                                                                                        
    return 0;                                                                                                                                                  
}  

char *strrstr(const char *haystack, const char *needle)
{
        char *r = NULL;

        if (!needle[0])
                return (char*)haystack + strlen(haystack);
        while (1) {
                char *p = strstr(haystack, needle);
                if (!p)
                        return r;
                r = p;
                haystack = p + 1;
        }
}

void get_stbuf(const char *buf, const char *prefix, const char c, char *strbuf, int len)
{
        char *p = strcasestr(buf, prefix);
        char *p1 = NULL;
        int len2 = strlen(prefix);
        if(p != NULL)
        {
                p1 = strchr(p+len2, c);
                if(p1 != NULL && p1-p-len2 <= len)
                {
                        strncpy(strbuf, p+len2, p1-p-len2);
                        strbuf[p1-p-len2] = '\0';
                }
        }
}

static char *str_delstr(char *p1,const char *p2)
{
    char *p;
    int i,len;
    len=strlen(p2);
    while((p=strstr(p1,p2))!=NULL)
    {
       for(i=p-p1+len;p1[i];i++)
       {
         *(p1+i-len)=*(p1+i);
       }
       *(p1+i-len)='\0';

   }
   return p1;
}

void printvar(sphinx_client *client, utPltDbHead *psDbHead, cJSON *pItem, char *psDbHeadName, char *pItemName, char *pQuery, struct curl_data data, int iNum)
{
	if(data.buf != NULL)
        {
        	_utPltPutLoopVar(psDbHead, psDbHeadName, iNum, data.buf ? data.buf : "");	
        }
        /*
        else if(utStrIsSpaces(pQuery))
        {
        	_utPltPutLoopVar(psDbHead, psDbHeadName, iNum, cJSON_GetObjectItem(pItem, pItemName)->valuestring);
        }
        */
        else
        {
		if(client == NULL)
		{
			client = sphinx_create ( SPH_TRUE, 5331);
		}
		if(client != NULL)
		{
			char *doc = cJSON_GetObjectItem(pItem, pItemName)->valuestring;
			char *docs[1];
			char **res;
			char *ptxt = NULL;
			if(strcmp(pItemName, "mailbody") == 0)
			{
				ptxt = calloc(strlen(doc)+1, sizeof(char));
				if(ptxt != NULL)
				{
					ncHttpCvtHtml2Text(doc, ptxt, strlen(doc));
					docs[0] = ptxt;
				}
			}
			else 
			{
				docs[0] = doc;
			}
			if(docs[0] == NULL) docs[0] = "";
			res = sphinx_build_excerpts ( client, 1, (const char **)docs, "dc", !utStrIsSpaces(pQuery) ? pQuery : "", NULL);
			if(res)
			{
				_utPltPutLoopVar(psDbHead, psDbHeadName, iNum, res[0]);
				//if(strcmp(pItemName, "mailbody") == 0)
				//	printf("res[0] = %s, pQuery = %s\n", res[0], pQuery);
				free(res[0]);
				free(res);
			}
			free(ptxt);
			
		}
        }
}

//flags = 1 指定按时间降序排列
char *createMatchAllJsonString(int start, int limit, int flags)
{
	cJSON *fmt;
	cJSON *fmt1;
	cJSON *root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "from", start);
	cJSON_AddNumberToObject(root, "size", limit);
	if(flags == 1)
	{
		fmt1=cJSON_CreateObject();
		cJSON_AddItemToObject(fmt1, "stime", fmt=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt, "order", "desc");
		cJSON_AddItemToObject(root, "sort", fmt=cJSON_CreateArray());
		cJSON_AddItemToArray(fmt, fmt1);
	}
	cJSON_AddItemToObject(root, "query", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt,"match_all",cJSON_CreateObject());
	
	const char *strings[]={"all*","tid*","num*"};//
	cJSON_AddItemToObject(root, "partial_fields", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "partial1", fmt1=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt1, "exclude", cJSON_CreateStringArray(strings,3));
	
	char *out=cJSON_Print(root);	
	cJSON_Delete(root);	
	printf("send = %s\n", out);
	return out;
}

char *createMatchAllTermJsonString(char *field, int start, int limit, int flags, char *query, unsigned long start1, unsigned long end, char *caQ1, char *caQ2, char *caQ3, char *caQ4, char *qeystr, unsigned long service)
{
	char *tmpgbk = NULL;
	cJSON *fmt;
	cJSON *fmt1;
	cJSON *fmt2;
	cJSON *fmt3;
	cJSON *fmt4;
	cJSON *fmt5;
	cJSON *fmt6;
	cJSON *root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "from", start);
	cJSON_AddNumberToObject(root, "size", limit);
	if(flags == 1)//按时间排序
	{
		fmt1=cJSON_CreateObject();
		cJSON_AddItemToObject(fmt1, "stime", fmt=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt, "order", "desc");
		cJSON_AddItemToObject(root, "sort", fmt=cJSON_CreateArray());
		cJSON_AddItemToArray(fmt, fmt1);
	}
	cJSON_AddItemToObject(root, "query", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "bool", fmt1=cJSON_CreateObject());
	
	if(!utStrIsSpaces(qeystr))//Bool Query
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", qeystr);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		
		const char *fields[]={field, NULL};//
		cJSON_AddItemToObject(fmt4, "query_string", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, "fields", cJSON_CreateStringArray(fields, 1));
		cJSON_AddStringToObject(fmt5, "query", tmpgbk ? tmpgbk : "");
		//cJSON_AddStringToObject(fmt5, "auto_generate_phrase_queries", "true");
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(service != 0)//指定了服务类型查询
	{
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, "service", fmt6=cJSON_CreateObject());
		cJSON_AddNumberToObject(fmt6, "from",service);
		cJSON_AddNumberToObject(fmt6, "to",service);
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
	}
	if(!utStrIsSpaces(query))//指定了关键字查询
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", query);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ1))//包含以下全部的关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ1);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ2))//包含以下的完整关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ2);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		
		cJSON_AddItemToObject(fmt4, "match", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "type", "phrase");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ3))//包含以下任意一个关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ3);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "or");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ4))//不包括以下关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ4);
		cJSON_AddItemToObject(fmt1, "must_not", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "or");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	const char *strings[]={"all*","tid*","num*"};//
	cJSON_AddItemToObject(root, "partial_fields", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "partial1", fmt1=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt1, "exclude", cJSON_CreateStringArray(strings,3));
	fmt1=cJSON_CreateObject();
	cJSON_AddItemToObject(fmt1, "fields", fmt=cJSON_CreateObject());
	
	cJSON_AddItemToObject(fmt, "mesg", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mfrom", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mto", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "dispname", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "sip", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "dip", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "bbsname", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "url", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "subject", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "content", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "formbody", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailsub", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailfrom", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailto", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailcc", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailbody", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "attachment_name", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "attachment_content", cJSON_CreateObject());
	
	cJSON_AddItemToObject(root, "highlight", fmt1);
	char *out=cJSON_Print(root);	
	cJSON_Delete(root);	
	printf("send = %s\n", out);
	return out;
}


char *createMatchIMTermJsonString(char *field, int start, int limit, int flags, char *query, unsigned long start1, unsigned long end, char *caQ1, char *caQ2, char *caQ3, char *caQ4, char *qeystr, unsigned long service)
{
	char *tmpgbk = NULL;
	cJSON *fmt;
	cJSON *fmt1;
	cJSON *fmt2;
	cJSON *fmt3;
	cJSON *fmt4;
	cJSON *fmt5;
	cJSON *fmt6;
	cJSON *root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "from", start);
	cJSON_AddNumberToObject(root, "size", limit);
	if(flags == 1)//按时间排序
	{
		fmt1=cJSON_CreateObject();
		cJSON_AddItemToObject(fmt1, "stime", fmt=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt, "order", "desc");
		cJSON_AddItemToObject(root, "sort", fmt=cJSON_CreateArray());
		cJSON_AddItemToArray(fmt, fmt1);
	}
	cJSON_AddItemToObject(root, "query", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "bool", fmt1=cJSON_CreateObject());
	
	if(!utStrIsSpaces(qeystr))//Bool Query
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", qeystr);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		
		const char *fields[]={field, NULL};//
		cJSON_AddItemToObject(fmt4, "query_string", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, "fields", cJSON_CreateStringArray(fields, 1));
		cJSON_AddStringToObject(fmt5, "query", tmpgbk ? tmpgbk : "");
		//cJSON_AddStringToObject(fmt5, "auto_generate_phrase_queries", "true");
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(service != 0)//指定了服务类型查询
	{
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, "service", fmt6=cJSON_CreateObject());
		cJSON_AddNumberToObject(fmt6, "from",service);
		cJSON_AddNumberToObject(fmt6, "to",service);
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
	}
	if(!utStrIsSpaces(query))//指定了关键字查询
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", query);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ1))//包含以下全部的关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ1);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ2))//包含以下的完整关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ2);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		
		cJSON_AddItemToObject(fmt4, "match", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "type", "phrase");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ3))//包含以下任意一个关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ3);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "or");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ4))//不包括以下关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ4);
		cJSON_AddItemToObject(fmt1, "must_not", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "or");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	const char *strings[]={"all*","tid*","num*"};//
	cJSON_AddItemToObject(root, "partial_fields", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "partial1", fmt1=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt1, "exclude", cJSON_CreateStringArray(strings,3));
	fmt1=cJSON_CreateObject();
	cJSON_AddItemToObject(fmt1, "fields", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mesg", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mfrom", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mto", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "dispname", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "sip", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "dip", cJSON_CreateObject());
	cJSON_AddItemToObject(root, "highlight", fmt1);
	char *out=cJSON_Print(root);	
	cJSON_Delete(root);	
	printf("send = %s\n", out);
	return out;
}

char *createMatchBBSTermJsonString(char *field, int start, int limit, int flags, char *query, unsigned long start1, unsigned long end, char *caQ1, char *caQ2, char *caQ3, char *caQ4, char *qeystr)
{
	char *tmpgbk = NULL;
	cJSON *fmt;
	cJSON *fmt1;
	cJSON *fmt2;
	cJSON *fmt3;
	cJSON *fmt4;
	cJSON *fmt5;
	cJSON *fmt6;
	cJSON *root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "from", start);
	cJSON_AddNumberToObject(root, "size", limit);
	if(flags == 1)//按时间排序
	{
		fmt1=cJSON_CreateObject();
		cJSON_AddItemToObject(fmt1, "stime", fmt=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt, "order", "desc");
		cJSON_AddItemToObject(root, "sort", fmt=cJSON_CreateArray());
		cJSON_AddItemToArray(fmt, fmt1);
	}
	cJSON_AddItemToObject(root, "query", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "bool", fmt1=cJSON_CreateObject());
	
	if(!utStrIsSpaces(qeystr))//Bool Query
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", qeystr);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		
		const char *fields[]={field, NULL};//
		cJSON_AddItemToObject(fmt4, "query_string", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, "fields", cJSON_CreateStringArray(fields, 1));
		cJSON_AddStringToObject(fmt5, "query", tmpgbk ? tmpgbk : "");
		//cJSON_AddStringToObject(fmt5, "auto_generate_phrase_queries", "true");
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(query))//指定了关键字查询
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", query);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ1))//包含以下全部的关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ1);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ2))//包含以下的完整关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ2);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		
		cJSON_AddItemToObject(fmt4, "match", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "type", "phrase");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ3))//包含以下任意一个关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ3);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "or");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ4))//不包括以下关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ4);
		cJSON_AddItemToObject(fmt1, "must_not", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "or");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	const char *strings[]={"all*","tid*","num*"};//
	cJSON_AddItemToObject(root, "partial_fields", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "partial1", fmt1=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt1, "exclude", cJSON_CreateStringArray(strings,3));
	fmt1=cJSON_CreateObject();
	cJSON_AddItemToObject(fmt1, "fields", fmt=cJSON_CreateObject());
	
	cJSON_AddItemToObject(fmt, "dispname", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "bbsname", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "url", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "subject", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "content", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "sip", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "dip", cJSON_CreateObject());
	cJSON_AddItemToObject(root, "highlight", fmt1);
	char *out=cJSON_Print(root);	
	cJSON_Delete(root);	
	printf("send = %s\n", out);
	return out;
}



char *createMatchBBSTermJsonString_v4(char *field, int start, int limit, int flags, char *query, unsigned long start1, unsigned long end, char *caQ1, char *caQ2, char *caQ3, char *caQ4, char *qeystr)
{
	char *tmpgbk = NULL;
	cJSON *fmt;
	cJSON *fmt1;
	cJSON *fmt2;
	cJSON *fmt3;
	cJSON *fmt4;
	cJSON *fmt5;
	cJSON *fmt6;
	cJSON *root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "from", start);
	cJSON_AddNumberToObject(root, "size", limit);
	if(flags == 1)//按时间排序
	{
		fmt1=cJSON_CreateObject();
		cJSON_AddItemToObject(fmt1, "stime", fmt=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt, "order", "desc");
		cJSON_AddItemToObject(root, "sort", fmt=cJSON_CreateArray());
		cJSON_AddItemToArray(fmt, fmt1);
	}
	
	
	
	
/*	
	if(!utStrIsSpaces(qeystr))//Bool Query
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", qeystr);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		
		const char *fields[]={field, NULL};//
		cJSON_AddItemToObject(fmt4, "query_string", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, "fields", cJSON_CreateStringArray(fields, 1));
		cJSON_AddStringToObject(fmt5, "query", tmpgbk ? tmpgbk : "");
		//cJSON_AddStringToObject(fmt5, "auto_generate_phrase_queries", "true");
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	*/
	if(!utStrIsSpaces(query))//指定了关键字查询
	{
//		tmpgbk = iconv_convert("GBK", "UTF-8", query);
  cJSON_AddItemToObject(root, "query", fmt=cJSON_CreateObject());
	
    cJSON_AddItemToObject(fmt, "bool", fmt1=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", query);
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
	//	free(tmpgbk);
	}
	else{
	//	 cJSON_AddItemToObject(fmt,"match_all",cJSON_CreateObject());

	//			cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
//		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
//		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
//		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
	//	cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
	//	cJSON_AddStringToObject(fmt6, "query", " ");
//		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(root, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		
		
	}
/*
	if(!utStrIsSpaces(caQ1))//包含以下全部的关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ1);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ2))//包含以下的完整关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ2);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		
		cJSON_AddItemToObject(fmt4, "match", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "type", "phrase");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ3))//包含以下任意一个关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ3);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "or");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ4))//不包括以下关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ4);
		cJSON_AddItemToObject(fmt1, "must_not", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "or");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	*/
	const char *strings[]={"all*","tid*","num*"};//
	cJSON_AddItemToObject(root, "partial_fields", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "partial1", fmt1=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt1, "exclude", cJSON_CreateStringArray(strings,3));
	fmt1=cJSON_CreateObject();
	cJSON_AddItemToObject(fmt1, "fields", fmt=cJSON_CreateObject());
	
	cJSON_AddItemToObject(fmt, "dispname", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "bbsname", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "url", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "subject", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "content", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "sip", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "dip", cJSON_CreateObject());
	cJSON_AddItemToObject(root, "highlight", fmt1);
	char *out=cJSON_Print(root);	
	cJSON_Delete(root);	
	printf("send = %s\n", out);
	return out;
}


//创建mail查询joson字符串
char *createMatchMailTermJsonString_v4(char *field, int start, int limit, int flags, char *query, unsigned long start1, unsigned long end, char *caQ1, char *caQ2, char *caQ3, char *caQ4, char *qeystr)
{
	char *tmpgbk = NULL;
	cJSON *fmt;
	cJSON *fmt1;
	cJSON *fmt2;
	cJSON *fmt3;
	cJSON *fmt4;
	cJSON *fmt5;
	cJSON *fmt6;
	cJSON *root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "from", start);
	cJSON_AddNumberToObject(root, "size", limit);
	if(flags == 1)//按时间排序
	{
		fmt1=cJSON_CreateObject();
		cJSON_AddItemToObject(fmt1, "stime", fmt=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt, "order", "desc");
		cJSON_AddItemToObject(root, "sort", fmt=cJSON_CreateArray());
		cJSON_AddItemToArray(fmt, fmt1);
	}
	
	
	
	

	if(!utStrIsSpaces(query))//指定了关键字查询
	{
//		tmpgbk = iconv_convert("GBK", "UTF-8", query);
    cJSON_AddItemToObject(root, "query", fmt=cJSON_CreateObject());	
    cJSON_AddItemToObject(fmt, "bool", fmt1=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", query);
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
	//	free(tmpgbk);
	}
	else{

		if(start1 != 0)
		{
			cJSON_AddItemToObject(root, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		
		
	}

	const char *strings[]={"all*","tid*","num*"};//
	cJSON_AddItemToObject(root, "partial_fields", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "partial1", fmt1=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt1, "exclude", cJSON_CreateStringArray(strings,3));
	fmt1=cJSON_CreateObject();
	cJSON_AddItemToObject(fmt1, "fields", fmt=cJSON_CreateObject());
	
	cJSON_AddItemToObject(fmt, "dispname", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailfrom", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailto", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailsub", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailbody", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "sip", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "dip", cJSON_CreateObject());
	cJSON_AddItemToObject(root, "highlight", fmt1);
	char *out=cJSON_Print(root);	
	cJSON_Delete(root);	
	printf("send = %s\n", out);
	return out;
}







char *createMatchMAILTermJsonString(char *field, int start, int limit, int flags, char *query, unsigned long start1, unsigned long end, char *caQ1, char *caQ2, char *caQ3, char *caQ4, char *qeystr)
{
	char *tmpgbk = NULL;
	cJSON *fmt;
	cJSON *fmt1;
	cJSON *fmt2;
	cJSON *fmt3;
	cJSON *fmt4;
	cJSON *fmt5;
	cJSON *fmt6;
	cJSON *root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "from", start);
	cJSON_AddNumberToObject(root, "size", limit);
	if(flags == 1)//按时间排序
	{
		fmt1=cJSON_CreateObject();
		cJSON_AddItemToObject(fmt1, "stime", fmt=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt, "order", "desc");
		cJSON_AddItemToObject(root, "sort", fmt=cJSON_CreateArray());
		cJSON_AddItemToArray(fmt, fmt1);
	}
	cJSON_AddItemToObject(root, "query", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "bool", fmt1=cJSON_CreateObject());
	
	if(!utStrIsSpaces(qeystr))//Bool Query
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", qeystr);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		
		const char *fields[]={field, NULL};//
		cJSON_AddItemToObject(fmt4, "query_string", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, "fields", cJSON_CreateStringArray(fields, 1));
		cJSON_AddStringToObject(fmt5, "query", tmpgbk ? tmpgbk : "");
		//cJSON_AddStringToObject(fmt5, "auto_generate_phrase_queries", "true");
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(query))//指定了关键字查询
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", query);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ1))//包含以下全部的关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ1);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ2))//包含以下的完整关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ2);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		
		cJSON_AddItemToObject(fmt4, "match", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "type", "phrase");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ3))//包含以下任意一个关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ3);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "or");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ4))//不包括以下关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ4);
		cJSON_AddItemToObject(fmt1, "must_not", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "or");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	const char *strings[]={"all*","tid*","num*"};//
	cJSON_AddItemToObject(root, "partial_fields", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "partial1", fmt1=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt1, "exclude", cJSON_CreateStringArray(strings,3));
	fmt1=cJSON_CreateObject();
	cJSON_AddItemToObject(fmt1, "fields", fmt=cJSON_CreateObject());
	
	cJSON_AddItemToObject(fmt, "dispname", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "sip", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "dip", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailsub", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailfrom", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailto", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailcc", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailbody", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "attachment_name", cJSON_CreateObject());
	//cJSON_AddNumberToObject(fmt6, "number_of_fragments", 0);
	cJSON_AddItemToObject(fmt, "attachment_content", cJSON_CreateObject());
	
	cJSON_AddItemToObject(root, "highlight", fmt1);
	char *out=cJSON_Print(root);	
	cJSON_Delete(root);	
	printf("send = %s\n", out);
	return out;
}

char *createMatchFORMTermJsonString(char *field, int start, int limit, int flags, char *query, unsigned long start1, unsigned long end, char *caQ1, char *caQ2, char *caQ3, char *caQ4, char *qeystr)
{
	char *tmpgbk = NULL;
	cJSON *fmt;
	cJSON *fmt1;
	cJSON *fmt2;
	cJSON *fmt3;
	cJSON *fmt4;
	cJSON *fmt5;
	cJSON *fmt6;
	cJSON *root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "from", start);
	cJSON_AddNumberToObject(root, "size", limit);
	if(flags == 1)//按时间排序
	{
		fmt1=cJSON_CreateObject();
		cJSON_AddItemToObject(fmt1, "stime", fmt=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt, "order", "desc");
		cJSON_AddItemToObject(root, "sort", fmt=cJSON_CreateArray());
		cJSON_AddItemToArray(fmt, fmt1);
	}
	cJSON_AddItemToObject(root, "query", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "bool", fmt1=cJSON_CreateObject());
	
	if(!utStrIsSpaces(qeystr))//Bool Query
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", qeystr);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		
		const char *fields[]={field, NULL};//
		cJSON_AddItemToObject(fmt4, "query_string", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, "fields", cJSON_CreateStringArray(fields, 1));
		cJSON_AddStringToObject(fmt5, "query", tmpgbk ? tmpgbk : "");
		//cJSON_AddStringToObject(fmt5, "auto_generate_phrase_queries", "true");
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(query))//指定了关键字查询
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", query);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ1))//包含以下全部的关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ1);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "and");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ2))//包含以下的完整关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ2);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		
		cJSON_AddItemToObject(fmt4, "match", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "type", "phrase");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ3))//包含以下任意一个关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ3);
		cJSON_AddItemToObject(fmt1, "must", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "or");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	if(!utStrIsSpaces(caQ4))//不包括以下关键词
	{
		tmpgbk = iconv_convert("GBK", "UTF-8", caQ4);
		cJSON_AddItemToObject(fmt1, "must_not", fmt2=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt2, "filtered", fmt3=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt3, "query", fmt4=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt4, "text", fmt5=cJSON_CreateObject());
		cJSON_AddItemToObject(fmt5, field, fmt6=cJSON_CreateObject());
		cJSON_AddStringToObject(fmt6, "query", tmpgbk ? tmpgbk : "");
		cJSON_AddStringToObject(fmt6, "operator", "or");	
		if(start1 != 0)
		{
			cJSON_AddItemToObject(fmt3, "filter", fmt4=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt4, "range", fmt5=cJSON_CreateObject());
			cJSON_AddItemToObject(fmt5, "stime", fmt6=cJSON_CreateObject());
			cJSON_AddNumberToObject(fmt6, "from", start1);	
			cJSON_AddNumberToObject(fmt6, "to", end);	
		}
		free(tmpgbk);
	}
	const char *strings[]={"all*","tid*","num*"};//
	cJSON_AddItemToObject(root, "partial_fields", fmt=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "partial1", fmt1=cJSON_CreateObject());
	cJSON_AddItemToObject(fmt1, "exclude", cJSON_CreateStringArray(strings,3));
	fmt1=cJSON_CreateObject();
	cJSON_AddItemToObject(fmt1, "fields", fmt=cJSON_CreateObject());
	
	cJSON_AddItemToObject(fmt, "dispname", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "sip", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "dip", cJSON_CreateObject());
	/*
	cJSON_AddItemToObject(fmt, "mailsub", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailfrom", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailto", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "mailcc", cJSON_CreateObject());
	*/
	cJSON_AddItemToObject(fmt, "formbody", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "url", cJSON_CreateObject());
	cJSON_AddItemToObject(fmt, "attachment_name", cJSON_CreateObject());
	//cJSON_AddNumberToObject(fmt6, "number_of_fragments", 0);
	cJSON_AddItemToObject(fmt, "attachment_content", cJSON_CreateObject());
	
	cJSON_AddItemToObject(root, "highlight", fmt1);
	char *out=cJSON_Print(root);	
	cJSON_Delete(root);	
	printf("send = %s\n", out);
	return out;
}

void parse_im_jsonstring(utPltDbHead *psDbHead, int iSum, char *jsonstring, int *took, int *count, char *query)
{
	char *tmputf8 = iconv_convert("GBK", "UTF-8", query);
	if(tmputf8 == NULL) return;
	int *v_did = (int *)calloc(iSum, sizeof(int));
	if(v_did == NULL)
	{
		free(tmputf8);
		return;
	}
		
	int *v_uid = (int *)calloc(iSum, sizeof(int));
	if(v_uid == NULL)
	{	free(v_did);
		free(tmputf8);
		return;
	}
	sphinx_client *client = NULL;	
	cJSON* pRoot = cJSON_Parse ( jsonstring );
	cJSON *pArrayItem = NULL;
	cJSON *pItem = NULL;
	cJSON *pItem1 = NULL;
	cJSON *pItem2 = NULL;
	int i;
	int cnt;
	int iNum = 0;
	int j = 0;
	int cnt2 = 0;
	
	
	//char *out=cJSON_Print(pRoot);
	//printf("recv = %s\n",out);
	//free(out);
	
	
	*took = cJSON_GetObjectItem( pRoot, "took") ? cJSON_GetObjectItem( pRoot, "took")->valueint : 0;
	if(*took == 0)
	{
		*count = 0;
		cJSON_Delete(pRoot);
		return;
	}
	
	if((pItem = cJSON_GetObjectItem ( pRoot, "hits")) && (*count=cJSON_GetObjectItem( pItem, "total")->valueint,pItem = cJSON_GetObjectItem ( pItem, "hits")))
	{
		cnt = cJSON_GetArraySize( pItem);
		
		struct curl_data data_dispname;
		struct curl_data data_sip;
		struct curl_data data_dip;
		struct curl_data data_mfrom;
		struct curl_data data_mto;
		struct curl_data data_mesg;
		char *highlightstr = NULL;
		for( i= 0; i < cnt; i++)
                {
                	memset(&data_dispname, 0, sizeof(struct curl_data));
                	memset(&data_sip, 0, sizeof(struct curl_data));
                	memset(&data_dip, 0, sizeof(struct curl_data));
                	memset(&data_mfrom, 0, sizeof(struct curl_data));
                	memset(&data_mto, 0, sizeof(struct curl_data));
                	memset(&data_mesg, 0, sizeof(struct curl_data));
                	pArrayItem = cJSON_GetArrayItem(pItem, i);
                	if((pItem1 = cJSON_GetObjectItem ( pArrayItem, "highlight")))
                	{
                		pItem2 = cJSON_GetObjectItem ( pItem1, "dispname");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_dispname);
                			}	
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "sip");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_sip);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "dip");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_dip);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mfrom");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mfrom);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mto");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mto);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mesg");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mesg);
                			}
                		}
                	}
                	if((pItem1 = cJSON_GetObjectItem ( pArrayItem, "fields")) && (pItem1 = cJSON_GetObjectItem ( pItem1, "partial1")))
                	{
                		pItem2 = NULL;
                		pItem1 = pItem2 ? pItem2 : pItem1;
                		v_did[iNum] = cJSON_GetObjectItem(pItem1, "did")->valueint;
  				v_uid[iNum] = cJSON_GetObjectItem(pItem1, "uid")->valueint;
                		iNum++;
  				if(iNum%2)
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  " ");
  				}	
  				else
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  "class=\"tbl_alt\"");
  				}
  				utPltPutLoopVarF(psDbHead,"iNum", iNum, "%lu", iNum);
  				utPltPutLoopVar(psDbHead, "service", iNum, get_ntype(cJSON_GetObjectItem(pItem1, "service")->valueint));
  				utPltPutLoopVarF(psDbHead, "userid", iNum, "%d", cJSON_GetObjectItem(pItem1, "uid")->valueint);
  				utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S",cJSON_GetObjectItem(pItem1, "stime")->valueint));
  				
  				printvar(client, psDbHead, pItem1, "udisp", "dispname", tmputf8, data_dispname, iNum);
  				printvar(client, psDbHead, pItem1, "sip", "sip", tmputf8, data_sip, iNum);
  				printvar(client, psDbHead, pItem1, "dip", "dip", tmputf8, data_dip, iNum);
  				printvar(client, psDbHead, pItem1, "mfrom", "mfrom", tmputf8, data_mfrom, iNum);
  				printvar(client, psDbHead, pItem1, "mto", "mto", tmputf8, data_mto, iNum);
  				printvar(client, psDbHead, pItem1, "mesg", "mesg", tmputf8, data_mesg, iNum);
                		
                		//cleanup
                		if(data_dispname.buf) free(data_dispname.buf);
                		if(data_sip.buf) free(data_sip.buf);
                		if(data_dip.buf) free(data_dip.buf);
                		if(data_mfrom.buf) free(data_mfrom.buf);
                		if(data_mto.buf) free(data_mto.buf);
                		if(data_mesg.buf) free(data_mesg.buf);
                		data_dispname.shift = 0;
                		data_sip.shift = 0;
                		data_dip.shift = 0;
                		data_mfrom.shift = 0;
                		data_mto.shift = 0;
                		data_mesg.shift = 0;
                	}
                }
	}
	cJSON_Delete(pRoot);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	int num = 0;
 	char username[17] = "";
  	char dispname[65] = "";
  	char ca_sql[256] = "";
  	for(num = 0; num < iNum; num++){
    		sprintf(ca_sql, "select username, dispname from ncsuser where userid = %d", v_did[num]);
    		
    		memset(username, 0, sizeof(username));
    		memset(dispname, 0, sizeof(dispname));
    		
    		pasDbOneRecord(ca_sql, 0, 
    				UT_TYPE_STRING, 16, username, 
    				UT_TYPE_STRING, 64, dispname);
    				
    		utPltPutLoopVar(psDbHead, "servicecode", num+1, username);
    		utPltPutLoopVar(psDbHead, "servicename", num+1, dispname);
    		              
   	}	
 	
 	char caMac[19] = "";
  	char caPosition[33] = "";
  	for(num = 0; num < iNum; num++){
       		sprintf(ca_sql, "select mac, position from ncsenduser where did = %d and uid = %d", v_did[num], v_uid[num]);
       		
       		memset(caMac,      0, sizeof(caMac));
       		memset(caPosition, 0, sizeof(caPosition));
       		
       		pasDbOneRecord(ca_sql, 0, 
       				UT_TYPE_STRING, 18, caMac, 
       				UT_TYPE_STRING, 32, caPosition);
       		utPltPutLoopVar(psDbHead, "mac", num+1, caMac);
    		utPltPutLoopVar(psDbHead, "position", num+1, caPosition);
  	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	free(v_did);
	free(v_uid);
	free(tmputf8);
	if(client != NULL)
		sphinx_destroy(client);
}

void parse_bbs_jsonstring(utPltDbHead *psDbHead, int iSum, char *jsonstring, int *took, int *count, char *query)
{
	
	char *tmputf8 = iconv_convert("GBK", "UTF-8", query);
	if(tmputf8 == NULL) return;
	int *v_did = (int *)calloc(iSum, sizeof(int));
	if(v_did == NULL)
	{
		free(tmputf8);
		return;
	}
		
	int *v_uid = (int *)calloc(iSum, sizeof(int));
	if(v_uid == NULL)
	{	free(v_did);
		free(tmputf8);
		return;
	}
	
	sphinx_client *client = NULL;
	cJSON* pRoot = cJSON_Parse ( jsonstring );
	cJSON *pArrayItem = NULL;
	cJSON *pItem = NULL;
	cJSON *pItem1 = NULL;
	cJSON *pItem2 = NULL;
	int i;
	int cnt;
	int iNum = 0;
	int j = 0;
	int cnt2 = 0;
	
	
	//char *out=cJSON_Print(pRoot);
	//printf("recv = %s\n",out);
	//free(out);
	
	*took = cJSON_GetObjectItem( pRoot, "took") ? cJSON_GetObjectItem( pRoot, "took")->valueint : 0;
	if(*took == 0)
	{
		*count = 0;
		cJSON_Delete(pRoot);
		return;
	}
	
	if((pItem = cJSON_GetObjectItem ( pRoot, "hits")) && (*count=cJSON_GetObjectItem( pItem, "total")->valueint,pItem = cJSON_GetObjectItem ( pItem, "hits")))
	{
		cnt = cJSON_GetArraySize( pItem);
		
		struct curl_data data_dispname;
		struct curl_data data_sip;
		struct curl_data data_dip;
		struct curl_data data_bbsname;
		struct curl_data data_url;
		struct curl_data data_subject;
		struct curl_data data_content;
		char *highlightstr = NULL;
		for( i= 0; i < cnt; i++)
                {
                	memset(&data_dispname, 0, sizeof(struct curl_data));
                	memset(&data_sip, 0, sizeof(struct curl_data));
                	memset(&data_dip, 0, sizeof(struct curl_data));
                	memset(&data_bbsname, 0, sizeof(struct curl_data));
                	memset(&data_url, 0, sizeof(struct curl_data));
                	memset(&data_subject, 0, sizeof(struct curl_data));
                	memset(&data_content, 0, sizeof(struct curl_data));
                	pArrayItem = cJSON_GetArrayItem(pItem, i);
                	if((pItem1 = cJSON_GetObjectItem ( pArrayItem, "highlight")))
                	{
                		pItem2 = cJSON_GetObjectItem ( pItem1, "dispname");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_dispname);
                			}	
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "sip");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_sip);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "dip");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_dip);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "bbsname");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_bbsname);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "url");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_url);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "subject");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_subject);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "content");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_content);
                			}
                		}
                	}
                	if((pItem1 = cJSON_GetObjectItem ( pArrayItem, "fields")) && (pItem1 = cJSON_GetObjectItem ( pItem1, "partial1")))
                	{
                		pItem2 = NULL;
                		pItem1 = pItem2 ? pItem2 : pItem1;
                		v_did[iNum] = cJSON_GetObjectItem(pItem1, "did")->valueint;
  				v_uid[iNum] = cJSON_GetObjectItem(pItem1, "uid")->valueint;
                		iNum++;
  				if(iNum%2)
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  " ");
  				}	
  				else
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  "class=\"tbl_alt\"");
  				}
  				utPltPutLoopVarF(psDbHead,"iNum", iNum, "%lu", iNum);
  				utPltPutLoopVar(psDbHead,"id", iNum, cJSON_GetObjectItem ( pArrayItem, "_id")->valuestring);
  				utPltPutLoopVarF(psDbHead, "userid", iNum, "%d", cJSON_GetObjectItem(pItem1, "uid")->valueint);
  				utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S",cJSON_GetObjectItem(pItem1, "stime")->valueint));
  				_utPltPutLoopVar(psDbHead, "url_no_highlighter", iNum, cJSON_GetObjectItem(pItem1, "url")->valuestring);
  				
                		printvar(client, psDbHead, pItem1, "udisp", "dispname", tmputf8, data_dispname, iNum);
                		printvar(client, psDbHead, pItem1, "sip", "sip", tmputf8, data_sip, iNum);
  				printvar(client, psDbHead, pItem1, "dip", "dip", tmputf8, data_dip, iNum);
  				printvar(client, psDbHead, pItem1, "bbsname", "bbsname", tmputf8, data_bbsname, iNum);
  				printvar(client, psDbHead, pItem1, "url", "url", tmputf8, data_url, iNum);
  				printvar(client, psDbHead, pItem1, "subject", "subject", tmputf8, data_subject, iNum);
  				printvar(client, psDbHead, pItem1, "content", "content", tmputf8, data_content, iNum);
  				
                		
                		//cleanup
                		if(data_dispname.buf) free(data_dispname.buf);
                		if(data_sip.buf) free(data_sip.buf);
                		if(data_dip.buf) free(data_dip.buf);
                		if(data_bbsname.buf) free(data_bbsname.buf);
                		if(data_url.buf) free(data_url.buf);
                		if(data_subject.buf) free(data_subject.buf);
                		if(data_content.buf) free(data_content.buf);
                		data_dispname.shift = 0;
                		data_sip.shift = 0;
                		data_dip.shift = 0;
                		data_bbsname.shift = 0;
                		data_url.shift = 0;
                		data_subject.shift = 0;
                		data_content.shift = 0;
                	}
                }
	}
	cJSON_Delete(pRoot);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	int num = 0;
 	char username[17] = "";
  	char dispname[65] = "";
  	char ca_sql[256] = "";
  	for(num = 0; num < iNum; num++){
    		sprintf(ca_sql, "select username, dispname from ncsuser where userid = %d", v_did[num]);
    		
    		memset(username, 0, sizeof(username));
    		memset(dispname, 0, sizeof(dispname));
    		
    		pasDbOneRecord(ca_sql, 0, 
    				UT_TYPE_STRING, 16, username, 
    				UT_TYPE_STRING, 64, dispname);
    				
    		utPltPutLoopVar(psDbHead, "servicecode", num+1, username);
    		utPltPutLoopVar(psDbHead, "servicename", num+1, dispname);
    		              
   	}	
 	
 	char caMac[19] = "";
  	char caPosition[33] = "";
  	for(num = 0; num < iNum; num++){
       		sprintf(ca_sql, "select mac, position from ncsenduser where did = %d and uid = %d", v_did[num], v_uid[num]);
       		
       		memset(caMac,      0, sizeof(caMac));
       		memset(caPosition, 0, sizeof(caPosition));
       		
       		pasDbOneRecord(ca_sql, 0, 
       				UT_TYPE_STRING, 18, caMac, 
       				UT_TYPE_STRING, 32, caPosition);
       		utPltPutLoopVar(psDbHead, "mac", num+1, caMac);
    		utPltPutLoopVar(psDbHead, "position", num+1, caPosition);
  	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	free(v_did);
	free(v_uid);
	if(client != NULL)
                sphinx_destroy ( client );
        free(tmputf8);
}

void parse_mail_jsonstring(utPltDbHead *psDbHead, int iSum, char *jsonstring, int *took, int *count, char *query)
{
	char *tmputf8 = iconv_convert("GBK", "UTF-8", query);
	if(tmputf8 == NULL) return;
	int *v_did = (int *)calloc(iSum, sizeof(int));
	if(v_did == NULL)
	{
		free(tmputf8);
		return;
	}
		
	int *v_uid = (int *)calloc(iSum, sizeof(int));
	if(v_uid == NULL)
	{	free(v_did);
		free(tmputf8);
		return;
	}
	sphinx_client *client = NULL;	
	char **res = NULL;
	char *docs[1];
	cJSON* pRoot = cJSON_Parse ( jsonstring );
	cJSON *pArrayItem = NULL;
	cJSON *pItem = NULL;
	cJSON *pItem1 = NULL;
	cJSON *pItem2 = NULL;
	int i;
	int cnt;
	int iNum = 0;
	int j = 0;
	int cnt2 = 0;
	char *p = NULL;
	char *p1 = NULL;
	
	//char *out=cJSON_Print(pRoot);
	//printf("recv = %s\n",out);
	//free(out);
	
	*took = cJSON_GetObjectItem( pRoot, "took") ? cJSON_GetObjectItem( pRoot, "took")->valueint : 0;
	if(*took == 0)
	{
		*count = 0;
		cJSON_Delete(pRoot);
		return;
	}
	
	if((pItem = cJSON_GetObjectItem ( pRoot, "hits")) && (*count=cJSON_GetObjectItem( pItem, "total")->valueint,pItem = cJSON_GetObjectItem ( pItem, "hits")))
	{
		cnt = cJSON_GetArraySize( pItem);
		
		struct curl_data data_dispname;
		struct curl_data data_sip;
		struct curl_data data_dip;
		struct curl_data data_mailsub;
		struct curl_data data_mailfrom;
		struct curl_data data_mailto;
		struct curl_data data_mailcc;
		struct curl_data data_mailbody;
		struct curl_data data_attachment_name;
		struct curl_data data_attachment_content;
		struct curl_data data_attachment_content_orig;
		char *highlightstr = NULL;
		char *orig_str = NULL;
		for( i= 0; i < cnt; i++)
                {
                	memset(&data_dispname, 0, sizeof(struct curl_data));
                	memset(&data_sip, 0, sizeof(struct curl_data));
                	memset(&data_dip, 0, sizeof(struct curl_data));
                	memset(&data_mailsub, 0, sizeof(struct curl_data));
                	memset(&data_mailfrom, 0, sizeof(struct curl_data));
                	memset(&data_mailto, 0, sizeof(struct curl_data));
                	memset(&data_mailcc, 0, sizeof(struct curl_data));
                	memset(&data_mailbody, 0, sizeof(struct curl_data));
                	memset(&data_attachment_name, 0, sizeof(struct curl_data));
                	memset(&data_attachment_content, 0, sizeof(struct curl_data));
                	
                	pArrayItem = cJSON_GetArrayItem(pItem, i);
                	if((pItem1 = cJSON_GetObjectItem ( pArrayItem, "highlight")))
                	{
                		pItem2 = cJSON_GetObjectItem ( pItem1, "dispname");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_dispname);
                			}	
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "sip");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_sip);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "dip");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_dip);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mailsub");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mailsub);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mailfrom");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mailfrom);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mailto");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mailto);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mailcc");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mailcc);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mailbody");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mailbody);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "attachment_name");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_attachment_name);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "attachment_content");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_attachment_content);
                			}
                		}
                	}
                	if((pItem1 = cJSON_GetObjectItem ( pArrayItem, "fields")) && (pItem1 = cJSON_GetObjectItem ( pItem1, "partial1")))
                	{
                		pItem2 = NULL;
                		pItem1 = pItem2 ? pItem2 : pItem1;
                		v_did[iNum] = cJSON_GetObjectItem(pItem1, "did")->valueint;
  				v_uid[iNum] = cJSON_GetObjectItem(pItem1, "uid")->valueint;
                		iNum++;
  				if(iNum%2)
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  " ");
  				}	
  				else
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  "class=\"tbl_alt\"");
  				}
  				utPltPutLoopVarF(psDbHead,"iNum", iNum, "%lu", iNum);
  				utPltPutLoopVar(psDbHead,"id", iNum, cJSON_GetObjectItem ( pArrayItem, "_id")->valuestring);
  				utPltPutLoopVarF(psDbHead,"style", iNum, "%d", cJSON_GetObjectItem ( pItem1, "style") ? cJSON_GetObjectItem ( pItem1, "style")->valueint : 1);
  				utPltPutLoopVarF(psDbHead, "userid", iNum, "%d", cJSON_GetObjectItem(pItem1, "uid")->valueint);
  				utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S",cJSON_GetObjectItem(pItem1, "stime")->valueint));
  				
  				printvar(client, psDbHead, pItem1, "udisp", "dispname", tmputf8, data_dispname, iNum);
                		printvar(client, psDbHead, pItem1, "sip", "sip", tmputf8, data_sip, iNum);
  				printvar(client, psDbHead, pItem1, "dip", "dip", tmputf8, data_dip, iNum);
  				printvar(client, psDbHead, pItem1, "sub", "mailsub", tmputf8, data_mailsub, iNum);
                		printvar(client, psDbHead, pItem1, "mfrom", "mailfrom", tmputf8, data_mailfrom, iNum);
  				printvar(client, psDbHead, pItem1, "mto", "mailto", tmputf8, data_mailto, iNum);
  				//printvar(client, psDbHead, pItem1, "mcc", "mailcc", tmputf8, data_mailcc, iNum);
  				printvar(client, psDbHead, pItem1, "body", "mailbody", tmputf8, data_mailbody, iNum);
				
				//邮件附件正文
                		if(data_attachment_content.buf != NULL)
                		{
                			_utPltPutLoopVar(psDbHead, "att_body", iNum, data_attachment_content.buf ? data_attachment_content.buf : "");	
                		}
                		else 
                		{
                			memset(&data_attachment_content_orig, 0, sizeof(struct curl_data));
  					cnt2 = cJSON_GetArraySize(cJSON_GetObjectItem(pItem1, "attachment_content"));
                			for( j = 0; j < cnt2; j++)
                			{
                				orig_str = cJSON_GetArrayItem(cJSON_GetObjectItem(pItem1, "attachment_content"), j)->valuestring;
                				write_data(orig_str, sizeof(char), strlen(orig_str), &data_attachment_content_orig);
                			}
					if(client == NULL)
					{
						client = sphinx_create ( SPH_TRUE, 5331);
					}
					if(client != NULL)
					{
						docs[0] = data_attachment_content_orig.buf;
						res = sphinx_build_excerpts ( client, 1, (const char **)docs, "dc", !utStrIsSpaces(tmputf8) ? tmputf8 : "", NULL);
						if(res)
						{
							_utPltPutLoopVar(psDbHead, "att_body", iNum, res[0]);
							free(res[0]);
							free(res);
						}
					}
                			free(data_attachment_content_orig.buf);
					data_attachment_content_orig.shift = 0;	
                		}
				
				//邮件附件名称
				cnt2 = cJSON_GetArraySize(cJSON_GetObjectItem(pItem1, "attachment_name"));
                		for( j = 0; j < cnt2; j++)
                		{
					if(client == NULL)
					{
							client = sphinx_create ( SPH_TRUE, 5331);
					}
					if(client != NULL)
					{
						docs[0] = cJSON_GetArrayItem(cJSON_GetObjectItem(pItem1, "attachment_name"), j)->valuestring;
						_utPltPutLoopVar2(psDbHead, "fname", iNum, j+1, docs[0]);
						
						utPltPutLoopVarF2(psDbHead, "idx", iNum, j+1, "%lu", j+1);
						utPltPutLoopVar2(psDbHead, "id", iNum, j+1, cJSON_GetObjectItem ( pArrayItem, "_id")->valuestring);
						
						
						if( (p = strstr(docs[0], "_")) && (p1 = strstr(p+1, "_")) &&(p = strstr(p1+1, "_")) )
						if(p == NULL)
							continue;
						docs[0] = p+1;
						res = sphinx_build_excerpts ( client, 1, (const char **)docs, "dc", !utStrIsSpaces(tmputf8) ? tmputf8 : "", NULL);
						if(res)
						{
							_utPltPutLoopVar2(psDbHead, "att_name", iNum, j+1, res[0]);
							free(res[0]);
							free(res);
						}
					}
                		}
                		
                		//cleanup
                		if(data_dispname.buf) free(data_dispname.buf);
                		if(data_sip.buf) free(data_sip.buf);
                		if(data_dip.buf) free(data_dip.buf);
                		if(data_mailsub.buf) free(data_mailsub.buf);
                		if(data_mailfrom.buf) free(data_mailfrom.buf);
                		if(data_mailto.buf) free(data_mailto.buf);	
                		if(data_mailcc.buf) free(data_mailcc.buf);
                		if(data_mailbody.buf) free(data_mailbody.buf);	
                		if(data_attachment_name.buf) free(data_attachment_name.buf);
                		if(data_attachment_content.buf) free(data_attachment_content.buf);
                		data_dispname.shift = 0;
                		data_sip.shift = 0;
                		data_dip.shift = 0;
                		data_mailsub.shift = 0;
                		data_mailfrom.shift = 0;
                		data_mailto.shift = 0;
                		data_mailcc.shift = 0;
                		data_mailbody.shift = 0;
                		data_attachment_name.shift = 0;
                		data_attachment_content.shift = 0;
                	}
                }
	}
	cJSON_Delete(pRoot);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	int num = 0;
 	char username[17] = "";
  	char dispname[65] = "";
  	char ca_sql[256] = "";
  	for(num = 0; num < iNum; num++){
    		sprintf(ca_sql, "select username, dispname from ncsuser where userid = %d", v_did[num]);
    		
    		memset(username, 0, sizeof(username));
    		memset(dispname, 0, sizeof(dispname));
    		
    		pasDbOneRecord(ca_sql, 0, 
    				UT_TYPE_STRING, 16, username, 
    				UT_TYPE_STRING, 64, dispname);
    				
    		utPltPutLoopVar(psDbHead, "servicecode", num+1, username);
    		utPltPutLoopVar(psDbHead, "servicename", num+1, dispname);
    		              
   	}	
 	
 	char caMac[19] = "";
  	char caPosition[33] = "";
  	for(num = 0; num < iNum; num++){
       		sprintf(ca_sql, "select mac, position from ncsenduser where did = %d and uid = %d", v_did[num], v_uid[num]);
       		
       		memset(caMac,      0, sizeof(caMac));
       		memset(caPosition, 0, sizeof(caPosition));
       		
       		pasDbOneRecord(ca_sql, 0, 
       				UT_TYPE_STRING, 18, caMac, 
       				UT_TYPE_STRING, 32, caPosition);
       		utPltPutLoopVar(psDbHead, "mac", num+1, caMac);
    		utPltPutLoopVar(psDbHead, "position", num+1, caPosition);
  	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	free(v_did);
	free(v_uid);
	if(client != NULL)
		sphinx_destroy ( client );
	free(tmputf8);
}

void parse_form_jsonstring(utPltDbHead *psDbHead, int iSum, char *jsonstring, int *took, int *count, char *query)
{
	char *tmputf8 = iconv_convert("GBK", "UTF-8", query);
	if(tmputf8 == NULL) return;
	int *v_did = (int *)calloc(iSum, sizeof(int));
	if(v_did == NULL)
	{
		free(tmputf8);
		return;
	}
		
	int *v_uid = (int *)calloc(iSum, sizeof(int));
	if(v_uid == NULL)
	{	free(v_did);
		free(tmputf8);
		return;
	}
	sphinx_client *client = NULL;	
	char **res = NULL;
	char *docs[1];
	cJSON* pRoot = cJSON_Parse ( jsonstring );
	cJSON *pArrayItem = NULL;
	cJSON *pItem = NULL;
	cJSON *pItem1 = NULL;
	cJSON *pItem2 = NULL;
	int i;
	int cnt;
	int iNum = 0;
	int j = 0;
	int cnt2 = 0;
	char *p = NULL;
	char *p1 = NULL;
	
	//char *out=cJSON_Print(pRoot);
	//printf("recv = %s\n",out);
	//free(out);
	
	
	*took = cJSON_GetObjectItem( pRoot, "took") ? cJSON_GetObjectItem( pRoot, "took")->valueint : 0;
	if(*took == 0)
	{
		*count = 0;
		cJSON_Delete(pRoot);
		return;
	}
	
	if((pItem = cJSON_GetObjectItem ( pRoot, "hits")) && (*count=cJSON_GetObjectItem( pItem, "total")->valueint,pItem = cJSON_GetObjectItem ( pItem, "hits")))
	{
		cnt = cJSON_GetArraySize( pItem);
		
		struct curl_data data_dispname;
		struct curl_data data_sip;
		struct curl_data data_dip;
		struct curl_data data_url;
		struct curl_data data_formbody;
		struct curl_data data_attachment_name;
		struct curl_data data_attachment_content;
		struct curl_data data_attachment_content_orig;
		char *highlightstr = NULL;
		char *orig_str = NULL;
		for( i= 0; i < cnt; i++)
                {
                	memset(&data_dispname, 0, sizeof(struct curl_data));
                	memset(&data_sip, 0, sizeof(struct curl_data));
                	memset(&data_dip, 0, sizeof(struct curl_data));
                	memset(&data_url, 0, sizeof(struct curl_data));
                	memset(&data_formbody, 0, sizeof(struct curl_data));
                	memset(&data_attachment_name, 0, sizeof(struct curl_data));
                	memset(&data_attachment_content, 0, sizeof(struct curl_data));
                	
                	pArrayItem = cJSON_GetArrayItem(pItem, i);
                	if((pItem1 = cJSON_GetObjectItem ( pArrayItem, "highlight")))
                	{
                		pItem2 = cJSON_GetObjectItem ( pItem1, "dispname");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_dispname);
                			}	
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "url");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_url);
                			}	
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "sip");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_sip);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "dip");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_dip);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "formbody");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_formbody);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "attachment_name");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_attachment_name);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "attachment_content");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_attachment_content);
                			}
                		}
                	}
                	if((pItem1 = cJSON_GetObjectItem ( pArrayItem, "fields")) && (pItem1 = cJSON_GetObjectItem ( pItem1, "partial1")))
                	{
                		pItem2 = NULL;
                		pItem1 = pItem2 ? pItem2 : pItem1;
                		v_did[iNum] = cJSON_GetObjectItem(pItem1, "did")->valueint;
  				v_uid[iNum] = cJSON_GetObjectItem(pItem1, "uid")->valueint;
                		iNum++;
  				if(iNum%2)
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  " ");
  				}	
  				else
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  "class=\"tbl_alt\"");
  				}
  				utPltPutLoopVarF(psDbHead,"iNum", iNum, "%lu", iNum);
  				utPltPutLoopVarF(psDbHead, "userid", iNum, "%d", cJSON_GetObjectItem(pItem1, "uid")->valueint);
  				utPltPutLoopVar(psDbHead,"id", iNum, cJSON_GetObjectItem ( pArrayItem, "_id")->valuestring);
  				_utPltPutLoopVar(psDbHead, "url_no_highlighter", iNum, cJSON_GetObjectItem(pItem1, "url")->valuestring);
  				utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S",cJSON_GetObjectItem(pItem1, "stime")->valueint));
  				
  				printvar(client, psDbHead, pItem1, "udisp", "dispname", tmputf8, data_dispname, iNum);
                		printvar(client, psDbHead, pItem1, "sip", "sip", tmputf8, data_sip, iNum);
  				printvar(client, psDbHead, pItem1, "dip", "dip", tmputf8, data_dip, iNum);
  				printvar(client, psDbHead, pItem1, "url", "url", tmputf8, data_url, iNum);
  				printvar(client, psDbHead, pItem1, "body", "formbody", tmputf8, data_formbody, iNum);
				
				//表单附件正文
                		if(data_attachment_content.buf != NULL)
                		{
                			_utPltPutLoopVar(psDbHead, "att_body", iNum, data_attachment_content.buf ? data_attachment_content.buf : "");	
                		}
                		else 
                		{
                			memset(&data_attachment_content_orig, 0, sizeof(struct curl_data));
  					          cnt2 = cJSON_GetArraySize(cJSON_GetObjectItem(pItem1, "attachment_content"));
                			for( j = 0; j < cnt2; j++)
                			{
                				orig_str = cJSON_GetArrayItem(cJSON_GetObjectItem(pItem1, "attachment_content"), j)->valuestring;
                				write_data(orig_str, sizeof(char), strlen(orig_str), &data_attachment_content_orig);
                			}
					if(client == NULL)
					{
						client = sphinx_create ( SPH_TRUE, 5331);
					}
					if(client != NULL)
					{
						docs[0] = data_attachment_content_orig.buf;
						res = sphinx_build_excerpts ( client, 1, (const char **)docs, "dc", !utStrIsSpaces(tmputf8) ? tmputf8 : "", NULL);
						if(res)
						{
							_utPltPutLoopVar(psDbHead, "att_body", iNum, res[0]);
							free(res[0]);
							free(res);
						}
					}
                			free(data_attachment_content_orig.buf);
					data_attachment_content_orig.shift = 0;	
                		}
				
				//表单附件名称
				cnt2 = cJSON_GetArraySize(cJSON_GetObjectItem(pItem1, "attachment_name"));
                		for( j = 0; j < cnt2; j++)
                		{
					if(client == NULL)
					{
							client = sphinx_create ( SPH_TRUE, 5331);
					}
					if(client != NULL)
					{
						docs[0] = cJSON_GetArrayItem(cJSON_GetObjectItem(pItem1, "attachment_name"), j)->valuestring;
						_utPltPutLoopVar2(psDbHead, "fname", iNum, j+1, docs[0]);
						
						utPltPutLoopVarF2(psDbHead, "idx", iNum, j+1, "%lu", j+1);
						utPltPutLoopVar2(psDbHead, "id", iNum, j+1, cJSON_GetObjectItem ( pArrayItem, "_id")->valuestring);
						
						
						if( (p = strstr(docs[0], "_")) && (p1 = strstr(p+1, "_")) &&(p = strstr(p1+1, "_")) )
						if(p == NULL)
							continue;
						docs[0] = p+1;
						res = sphinx_build_excerpts ( client, 1, (const char **)docs, "dc", !utStrIsSpaces(tmputf8) ? tmputf8 : "", NULL);
						if(res)
						{
							_utPltPutLoopVar2(psDbHead, "att_name", iNum, j+1, res[0]);
							free(res[0]);
							free(res);
						}
					}
                		}
                		
                		//cleanup
                		if(data_dispname.buf) free(data_dispname.buf);
                		if(data_sip.buf) free(data_sip.buf);
                		if(data_dip.buf) free(data_dip.buf);
                		/*
                		if(data_mailsub.buf) free(data_mailsub.buf);
                		if(data_mailfrom.buf) free(data_mailfrom.buf);
                		if(data_mailto.buf) free(data_mailto.buf);	
                		if(data_mailcc.buf) free(data_mailcc.buf);
                		*/
                		if(data_url.buf) free(data_url.buf);	
                		if(data_formbody.buf) free(data_formbody.buf);	
                		if(data_attachment_name.buf) free(data_attachment_name.buf);
                		if(data_attachment_content.buf) free(data_attachment_content.buf);
                		data_dispname.shift = 0;
                		data_sip.shift = 0;
                		data_dip.shift = 0;
                		/*
                		data_mailsub.shift = 0;
                		data_mailfrom.shift = 0;
                		data_mailto.shift = 0;
                		data_mailcc.shift = 0;
                		*/
                		data_url.shift = 0;
                		data_formbody.shift = 0;
                		data_attachment_name.shift = 0;
                		data_attachment_content.shift = 0;
                	}
                }
	}
	cJSON_Delete(pRoot);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	int num = 0;
 	char username[17] = "";
  	char dispname[65] = "";
  	char ca_sql[256] = "";
  	for(num = 0; num < iNum; num++){
    		sprintf(ca_sql, "select username, dispname from ncsuser where userid = %d", v_did[num]);
    		
    		memset(username, 0, sizeof(username));
    		memset(dispname, 0, sizeof(dispname));
    		
    		pasDbOneRecord(ca_sql, 0, 
    				UT_TYPE_STRING, 16, username, 
    				UT_TYPE_STRING, 64, dispname);
    				
    		utPltPutLoopVar(psDbHead, "servicecode", num+1, username);
    		utPltPutLoopVar(psDbHead, "servicename", num+1, dispname);
    		              
   	}	
 	
 	char caMac[19] = "";
  	char caPosition[33] = "";
  	for(num = 0; num < iNum; num++){
       		sprintf(ca_sql, "select mac, position from ncsenduser where did = %d and uid = %d", v_did[num], v_uid[num]);
       		
       		memset(caMac,      0, sizeof(caMac));
       		memset(caPosition, 0, sizeof(caPosition));
       		
       		pasDbOneRecord(ca_sql, 0, 
       				UT_TYPE_STRING, 18, caMac, 
       				UT_TYPE_STRING, 32, caPosition);
       		utPltPutLoopVar(psDbHead, "mac", num+1, caMac);
    		utPltPutLoopVar(psDbHead, "position", num+1, caPosition);
  	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	free(v_did);
	free(v_uid);
	if(client != NULL)
		sphinx_destroy ( client );
	free(tmputf8);
}

char *getdoc(char *url, char *field, int idx)
{
	CURL *curl;
  	CURLcode res;
  	char *out = NULL;
  	struct curl_data wdata;
  	curl_global_init(CURL_GLOBAL_ALL);
  	curl = curl_easy_init();
  	if(!curl)
  		return NULL;
  	curl_easy_setopt(curl, CURLOPT_URL, url);
  	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE,  1L);
  	memset(&wdata, 0, sizeof(struct curl_data));
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA,  &wdata);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    	res = curl_easy_perform(curl);
    	
    	if(res == CURLE_OK)
    	{
    		wdata.buf[wdata.shift] = '\0';
    		cJSON* pRoot = cJSON_Parse ( wdata.buf );
    		cJSON* pItem1 = cJSON_GetObjectItem(pRoot, "fields");
    		if(pItem1 != NULL)
    		{
    			cJSON* pItem2 = cJSON_GetObjectItem(pItem1, field);
    			if(pItem2 != NULL)
    			{
    				if(idx == 0)
    					out = strdup(pItem2->valuestring);
    				else 
    				{
    					out = strdup(cJSON_GetArrayItem(pItem2, idx-1)->valuestring);
    				}
    			}
    		}
    		cJSON_Delete(pRoot);
    		free(wdata.buf);
    	}
    	curl_global_cleanup();
    	return out;
}

int ncsshowfield_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	utMsgPrintMsg(psMsgHead);
	char _table[17] = "";
	char id[65] = "";
	char url[128] = "";
	char field[17] = "";
	char idx[17];
	char style[2] = "";
	char *doc = NULL;
	char *tmpgbk = NULL;
	utPltDbHead *psDbHead = utPltInitDb();
	
	utMsgGetSomeNVar(psMsgHead, 5, "table", UT_TYPE_STRING, sizeof(_table)-1, _table, 
				       "id", UT_TYPE_STRING, sizeof(id)-1, id,
				       "field", UT_TYPE_STRING, sizeof(field)-1, field,
				       "idx",   UT_TYPE_STRING, sizeof(idx)-1, idx,
				       "style", UT_TYPE_STRING, sizeof(style)-1, style);
	if(strcmp(_table, "ncsbbsmon") == 0)
	{
		snprintf(url, sizeof(url), "http://127.0.0.1:9200/dc/bbs/%s?fields=content", id);
		doc = getdoc(url, "content", 0);
		if(doc != NULL)
		{
			tmpgbk = iconv_convert("UTF-8", "GBK", doc);
			utPltSetCvtHtml(1);
			utPltPutVar(psDbHead, "body", tmpgbk ? tmpgbk : "");
			utPltSetCvtHtml(0);	
			free(tmpgbk);
			free(doc);
		}
	}
	else if(strcmp(_table, "ncsmailmon") == 0)
	{
		if(strcmp(field, "body") == 0)
		{
			snprintf(url, sizeof(url), "http://127.0.0.1:9200/dc/mail/%s?fields=mailbody", id);
			doc = getdoc(url, "mailbody", 0);
			if(doc != NULL)
			{
				tmpgbk = iconv_convert("UTF-8", "GBK", doc);
				if(strcmp(style, "1") == 0)
					utPltSetCvtHtml(1);
				utPltPutVar(psDbHead, "body", tmpgbk ? tmpgbk : "");
				if(strcmp(style, "1") == 0)
					utPltSetCvtHtml(0);
				free(tmpgbk);
				free(doc);
			}
		}
		if(strcmp(field, "att_body") == 0)
		{
			snprintf(url, sizeof(url), "http://127.0.0.1:9200/dc/mail/%s?fields=attachment_content", id);
			doc = getdoc(url, "attachment_content", atoi(idx));
			if(doc != NULL)
			{
				tmpgbk = iconv_convert("UTF-8", "GBK", doc);
				utPltSetCvtHtml(1);
				utPltPutVar(psDbHead, "body", tmpgbk ? tmpgbk : "");
				utPltSetCvtHtml(0);
				free(tmpgbk);
				free(doc);
			}
		}
		
	}
	else if(strcmp(_table, "ncsformmon") == 0)
	{
		if(strcmp(field, "body") == 0)
		{
			snprintf(url, sizeof(url), "http://127.0.0.1:9200/dc/form/%s?fields=formbody", id);
			doc = getdoc(url, "formbody", 0);
			if(doc != NULL)
			{
				tmpgbk = iconv_convert("UTF-8", "GBK", doc);
				utPltSetCvtHtml(1);
				utPltPutVar(psDbHead, "body", tmpgbk ? tmpgbk : "");
				utPltSetCvtHtml(0);
				free(tmpgbk);
				free(doc);
			}
		}
		if(strcmp(field, "att_body") == 0)
		{
			snprintf(url, sizeof(url), "http://127.0.0.1:9200/dc/form/%s?fields=attachment_content", id);
			doc = getdoc(url, "attachment_content", atoi(idx));
			if(doc != NULL)
			{
				tmpgbk = iconv_convert("UTF-8", "GBK", doc);
				utPltSetCvtHtml(1);
				utPltPutVar(psDbHead, "body", tmpgbk ? tmpgbk : "");
				utPltSetCvtHtml(0);
				free(tmpgbk);
				free(doc);
			}
		}
		
	}
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/show_bbscontent.html");             
	return 0;
}

int ncsimlog_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	char caRe[4] = "";
	char caRn[16] = "";
	char caLm[16] = "";
	char caQuery[256] = "";
	char caTotal[16] = "";
	char caSort_flag[3] = "";
	char caQ1[256] = "";
	char caQ2[256] = "";
	char caQ3[256] = "";
	char caQ4[256] = "";
	char caQ5[16] = "";
	char caQ6[16] = "";
	
	char ca_query_text[1024] = ""; 
	char wd[1024]= "";
	char query[1024] = "";
	char qeystr[1024] = "";
	
	char ca_cur_pg[9] = "";
	int iSortFlag = 0;
	int iSum = 0;
	int took = 0;
	struct curl_data wdata;
	CURL *curl;
  	CURLcode res;
	char *jsonstr = NULL;
	char *field = "all";
	unsigned long lCp = 0;
	unsigned long iStart = 0;
	unsigned long lCount = 0;
	unsigned long start = 0;
	unsigned long end = time(NULL);
	utMsgPrintMsg(psMsgHead);
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgGetSomeNVar(psMsgHead, 14, 
				       "f", 	UT_TYPE_STRING, 3, 	caRe,
				       "rn",    UT_TYPE_STRING, 15, 	caRn,
	                               "lm",    UT_TYPE_STRING, 15, 	caLm, 
	                               "q",     UT_TYPE_STRING, 255, 	caQuery,           	
	                               "t",     UT_TYPE_STRING, 15,  	caTotal,          	
	                               "sf", 	UT_TYPE_STRING, 2, 	caSort_flag, 
	                               "q1",    UT_TYPE_STRING, 255,	caQ1,
	                               "q2",    UT_TYPE_STRING, 255,	caQ2,
	                               "q3",    UT_TYPE_STRING, 255,	caQ3,
	                               "q4",    UT_TYPE_STRING, 255,	caQ4,
	                               "q5",    UT_TYPE_STRING, 15, 	caQ5,//查询字段
	                               "q6",    UT_TYPE_STRING, 15, 	caQ6,
	                               "wd",    UT_TYPE_STRING, 255, 	wd, 
	                               "cp",    UT_TYPE_STRING, 8,  	ca_cur_pg); 
	utPltPutVar(psDbHead, "q1", caQ1);
	utPltPutVar(psDbHead, "q2", caQ2);
	utPltPutVar(psDbHead, "q3", caQ3);
	utPltPutVar(psDbHead, "q4", caQ4);
	utPltPutVar(psDbHead, "rn", caRn);
	utPltPutVar(psDbHead, "lm", caLm);
	if(strlen(caRn) > 0)/* 指定了搜索结果显示的条数 */ 
	{
	  	iSum = atoi(caRn);
                if(iSum == 10 || iSum == 20 || iSum == 50 || iSum == 100)
                {
                }
                else
                {
                        iSum = 10;
                }
	}
	else /* 默认10条 */
	{
		iSum = 10;
	}
	lCp = atol(ca_cur_pg);
	if (lCp == 0) 
		lCp = 1;
        iStart = (lCp - 1) * iSum;
	lCount = 0;
	utPltPutVarF(psDbHead, "cp", "%lu", lCp);
	utPltPutVarF(psDbHead, "start", "%lu", iStart+1);
	
	
	/////////////////////////////////////////////////////////////////////////////////
	if(strlen(caQ1) > 0 && !utStrIsSpaces(caQ1))
	{
	  	snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "%s ", caQ1);
	}
	if(strlen(caQ3) > 0 && !utStrIsSpaces(caQ3))
	{
		char *word = strtok(caQ3, SEPCHARS);
		if(word != NULL)
			if(strlen(ca_query_text) > 0)
				snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "AND ");
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "(%s", word);	
		word = strtok(NULL, SEPCHARS);
		while(word != NULL)
		{
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " OR %s", word);
			word = strtok(NULL, SEPCHARS);
		}
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text),  ")");
	}  
	if(strlen(caQ4) > 0 && !utStrIsSpaces(caQ4))
	{
		if(strlen(ca_query_text) > 0)
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " AND (");
		char *word = strtok(caQ4, SEPCHARS);
		while(word != NULL)
		{
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "NOT %s", word);
			word = strtok(NULL, SEPCHARS);
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "%s", word ? " " : "");
		}
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text),  ")");
	} 
	if(strlen(caQ2) > 0 && !utStrIsSpaces(caQ2))
	{
		if(strlen(ca_query_text) > 0)
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " ");
	  	snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "\"%s\"", caQ2);
	}
	if(strlen(wd) == 0 || utStrIsSpaces(wd))
	{
		memset(wd, 0, sizeof(wd));
		strncpy(wd, ca_query_text, sizeof(wd)-1);
	}
	
	strncpy(query, wd, sizeof(query)-1);
	str_replace(query, '\"',  "&quot;");
	utPltPutVar(psDbHead, "query", query);
	/////////////////////////////////////////////////////////////////////////////////
	
	
	if(strcmp(caRe, "re") == 0)
	{
	  	utPltPutVar(psDbHead, "query_text", caQuery);
      		utPltPutVar(psDbHead, "total",      caTotal);
      		return utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/re_im_search.html");
  	} 
  	if(strlen(caSort_flag) == 0 || strcmp(caSort_flag, "0") == 0)/* 按时间排序 */
	{
		iSortFlag = 1;
	}
	
	unsigned long lLm = atol(caLm);
	if(lLm == 1 || lLm == 7 || lLm == 30 || lLm == 365)
		start = end - lLm * 86400;
	else start = 0;
		
	switch(atol(caQ5))
	{
		case 1:
			field = "dispname";
			break;
		case 2:
			field = "sip";
			break;
		case 3:
			field = "dip";
			break;
		case 4:
			field = "mfrom";
			break;	
		case 5:
			field = "mto";
			break;	
		case 6:
			field = "mesg";
			break;	
		default:
			break;
	}
	
	int service = atol(caQ6);
	utMsgGetSomeNVar(psMsgHead, 4, 
	                               "q1",    UT_TYPE_STRING, 255,	caQ1,
	                               "q2",    UT_TYPE_STRING, 255,	caQ2,
	                               "q3",    UT_TYPE_STRING, 255,	caQ3,
	                               "q4",    UT_TYPE_STRING, 255,	caQ4); 
	if(!utStrIsSpaces(caQ1) || !utStrIsSpaces(caQ2) || !utStrIsSpaces(caQ3)) //指定了高级查询
	{
		jsonstr = createMatchIMTermJsonString(field, iStart, iSum, iSortFlag, "", start, end, caQ1, caQ2, caQ3, caQ4, "", service);
	}
	else if(!utStrIsSpaces(wd))
	{
		get_stbuf(wd, "\"", '\"', caQ2, sizeof(caQ2)-1);
		if(strcasestr(wd, "AND") || strcasestr(wd, "OR") || strcasestr(wd, "NOT") )
		{
			strcpy(qeystr, wd);
			str_delstr(qeystr, "\"");
			if(!utStrIsSpaces(caQ2))
			{
				str_delstr(qeystr, caQ2);
			}
			if(qeystr[strlen(qeystr)-1] == ' ')
			{
				qeystr[strlen(qeystr)-1] = '\0';
			}
			printf("qeystr = %s, caQ2 = %s\n", qeystr, caQ2);
			jsonstr = createMatchIMTermJsonString(field, iStart, iSum, iSortFlag, "", start, end, "", caQ2, "", "", qeystr, service);
		}
		else 
		{
			if(strlen(caQ2) > 0 ) 
				wd[0] = '\0';
			jsonstr = createMatchIMTermJsonString(field, iStart, iSum, iSortFlag, wd, start, end, "", caQ2, "", "", "", service);
		}
	}
	else 
	{
		jsonstr = createMatchAllJsonString(iStart, iSum, iSortFlag);
	}
  	curl_global_init(CURL_GLOBAL_ALL);
  	curl = curl_easy_init();
  	if(!curl)
  		return 0;
  	curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:9200/dc/im/_search?pretty=true");
  	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE,  1L);
  	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  	memset(&wdata, 0, sizeof(struct curl_data));
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA,  &wdata);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr);//for XGET
	res = curl_easy_perform(curl);
    	if(res == CURLE_OK)
    	{
    		wdata.buf[wdata.shift] = '\0';
    		char word[1536] = "";
    		if(!utStrIsSpaces(wd))
    			snprintf(word, sizeof(word), "%s", wd);
    		if(!utStrIsSpaces(caQ1))
    			snprintf(word, sizeof(word), " %s", caQ1);
    		if(!utStrIsSpaces(caQ2))
    			snprintf(word, sizeof(word), " %s", caQ2);
    		if(!utStrIsSpaces(caQ3))
    			snprintf(word, sizeof(word), " %s", caQ3);
    		if(!utStrIsSpaces(caQ4))
    			snprintf(word, sizeof(word), " %s", caQ4);
    		parse_im_jsonstring(psDbHead, iSum, wdata.buf, &took, (int *)&lCount, word);
    		
    		free(wdata.buf);
    	}
    	else
    	{
    		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    	}
    	curl_global_cleanup();
    	free(jsonstr);
    	if(lCount == 0 || iStart > lCount){
		utPltPutVar(psDbHead, "display2", "none");
		utPltPutVar(psDbHead, "display1", " ");
		return utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/search_im_result.html");
   	}
   	else 
	{    	
	        utPltPutVar(psDbHead, "display1", "none");
	}        
	unsigned long ltotpg = (lCount % iSum) ? (lCount / iSum + 1) : lCount / iSum;	//总页数 
	utPltPutVarF(psDbHead, "total", "%lu", lCount);
	utPltPutVarF(psDbHead, "total_times", "%d.%03d", took/1000, took%1000);
	utPltPutVarF(psDbHead, "end", "%lu", lCp == ltotpg ? lCount : iStart+iSum);
		
	strncpy(wd, query, sizeof(wd)-1);
	str_replace(caQ1, '\"',  "&quot;");
	str_replace(caQ2, '\"',  "&quot;");
	str_replace(caQ3, '\"',  "&quot;");
	str_replace(caQ4, '\"',  "&quot;");
    	if(lCp != 1 ){
		utPltPutVar(psDbHead, "prev", "上一页");
		utPltPutVarF(psDbHead, "CurPg_prev", "%d", lCp-1);
		utPltPutVar(psDbHead, "wd_prev",wd);
		utPltPutVar(psDbHead, "rn_prev", caRn);
		utPltPutVar(psDbHead, "lm_prev", caLm);
		utPltPutVar(psDbHead, "sf_prev", caSort_flag);
		
		utPltPutVar(psDbHead, "q1_prev", caQ1);
		utPltPutVar(psDbHead, "q2_prev", caQ2);
		utPltPutVar(psDbHead, "q3_prev", caQ3);
		utPltPutVar(psDbHead, "q4_prev", caQ4);
		utPltPutVar(psDbHead, "q5_prev", caQ5);
		utPltPutVar(psDbHead, "q6_prev", caQ6);
	}
	if(lCp != ltotpg && ltotpg > 0){
	
		utPltPutVar(psDbHead, "next", "下一页");
		utPltPutVarF(psDbHead, "CurPg_next", "%d", lCp+1);
		utPltPutVar(psDbHead, "wd_next",wd);
		utPltPutVar(psDbHead, "rn_next", caRn);
		utPltPutVar(psDbHead, "lm_next", caLm);
		utPltPutVar(psDbHead, "sf_next", caSort_flag);
		
		utPltPutVar(psDbHead, "q1_next", caQ1);
		utPltPutVar(psDbHead, "q2_next", caQ2);
		utPltPutVar(psDbHead, "q3_next", caQ3);
		utPltPutVar(psDbHead, "q4_next", caQ4);
		utPltPutVar(psDbHead, "q5_next", caQ5);
		utPltPutVar(psDbHead, "q6_next", caQ6);
   	}
	int i = 0;
   	if(lCp > 10){
	 	for(i = lCp - 10; i < lCp ; i++){ 
			utPltPutLoopVarF(psDbHead, "page1", i-(lCp - 11), "[%d]", i);
			utPltPutLoopVarF(psDbHead, "CurPg_page1", i-(lCp - 11), "%d", i);
			utPltPutLoopVar(psDbHead, "wd_page1",i-(lCp - 11),wd);
			utPltPutLoopVar(psDbHead, "rn_page1", i-(lCp - 11), caRn);
			utPltPutLoopVar(psDbHead, "lm_page1", i-(lCp - 11), caLm);
			utPltPutLoopVar(psDbHead, "sf_page1", i-(lCp - 11), caSort_flag);
			
			utPltPutLoopVar(psDbHead, "q1_page1", i-(lCp - 11), caQ1);
			utPltPutLoopVar(psDbHead, "q2_page1", i-(lCp - 11), caQ2);
			utPltPutLoopVar(psDbHead, "q3_page1", i-(lCp - 11), caQ3);
			utPltPutLoopVar(psDbHead, "q4_page1", i-(lCp - 11), caQ4);
			utPltPutLoopVar(psDbHead, "q5_page1", i-(lCp - 11), caQ5);
			utPltPutLoopVar(psDbHead, "q6_page1", i-(lCp - 11), caQ6);
		}
 	}
	
 	if(lCp <= 10){
	 	for(i = 1; i < lCp; i++){
			utPltPutLoopVarF(psDbHead, "page1", i, "[%d]", i);
			utPltPutLoopVarF(psDbHead, "CurPg_page1", i, "%d", i);
			utPltPutLoopVar(psDbHead, "wd_page1",i,wd);
			utPltPutLoopVar(psDbHead, "rn_page1", i, caRn);
			utPltPutLoopVar(psDbHead, "lm_page1", i, caLm);
			utPltPutLoopVar(psDbHead, "sf_page1", i, caSort_flag);
			
			utPltPutLoopVar(psDbHead, "q1_page1", i, caQ1);
			utPltPutLoopVar(psDbHead, "q2_page1", i, caQ2);
			utPltPutLoopVar(psDbHead, "q3_page1", i, caQ3);
			utPltPutLoopVar(psDbHead, "q4_page1", i, caQ4);
			utPltPutLoopVar(psDbHead, "q5_page1", i, caQ5);
			utPltPutLoopVar(psDbHead, "q6_page1", i, caQ6);
	 	}
 	}
 	if(ltotpg > 0)
	 	utPltPutVarF(psDbHead, "page",  "%d", lCp);
 
 	for(i = lCp+1; i < lCp+10 && i < ltotpg; i++){
	 	utPltPutLoopVarF(psDbHead, "page2", i-lCp, "[%d]", i);
	 	utPltPutLoopVarF(psDbHead, "CurPg_page2", i-lCp, "%d", i);
	 	utPltPutLoopVar(psDbHead, "wd_page2",i-lCp,wd);
	 	utPltPutLoopVar(psDbHead, "rn_page2", i-lCp, caRn);
     		utPltPutLoopVar(psDbHead, "lm_page2", i-lCp, caLm);
     		utPltPutLoopVar(psDbHead, "sf_page2",  i-lCp, caSort_flag);
     		
     		utPltPutLoopVar(psDbHead, "q1_page2",  i-lCp, caQ1);
     		utPltPutLoopVar(psDbHead, "q2_page2",  i-lCp, caQ2);
     		utPltPutLoopVar(psDbHead, "q3_page2",  i-lCp, caQ3);
     		utPltPutLoopVar(psDbHead, "q4_page2",  i-lCp, caQ4);
     		utPltPutLoopVar(psDbHead, "q5_page2",  i-lCp, caQ5);
     		utPltPutLoopVar(psDbHead, "q6_page2",  i-lCp, caQ6);
 	}
 	if(strcmp(caSort_flag, "1") == 0){	
 		utPltPutVarF(psDbHead, "sort1", "%s", "<b>已按相关性排序</b>");
 		utPltPutVarF(psDbHead, "sort2", "<a href=\"/pronline/Msg?FunName@ncsimlog_v4&wd@%s&q1@%s&q2@%s&q3@%s&q4@%s&cp@%d&rn@%s&lm@%s&sf@0\">按日期排序</a>",  wd, caQ1, caQ2, caQ3, caQ4, lCp, caRn, caLm);
 	}
 	else{
 		utPltPutVarF(psDbHead, "sort1", "<a href=\"/pronline/Msg?FunName@ncsimlog_v4&wd@%s&q1@%s&q2@%s&q3@%s&q4@%s&cp@%d&rn@%s&lm@%s&sf@1\">按相关性排序</a>",  wd, caQ1, caQ2, caQ3, caQ4, lCp, caRn, caLm);
 		utPltPutVarF(psDbHead, "sort2", "%s", "<b>已按日期排序</b>");
 	}
    	utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/search_im_result.html");
	return 0;
}

int ncsbbsmon_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	char caRe[4] = "";
	char caRn[16] = "";
	char caLm[16] = "";
	char caQuery[256] = "";
	char caTotal[16] = "";
	char caSort_flag[3] = "";
	char caQ1[256] = "";
	char caQ2[256] = "";
	char caQ3[256] = "";
	char caQ4[256] = "";
	char caQ5[16] = "";
	char caQ6[16] = "";
	
	char ca_query_text[1024] = ""; 
	char wd[1024]= "";
	char query[1024] = "";
	char qeystr[1024] = "";
	
	char ca_cur_pg[9] = "";
	int iSortFlag = 0;
	int iSum = 0;
	int took = 0;
	struct curl_data wdata;
	CURL *curl;
  	CURLcode res;
	char *jsonstr = NULL;
	char *field = "all";
	unsigned long lCp = 0;
	unsigned long iStart = 0;
	unsigned long lCount = 0;
	unsigned long start = 0;
	unsigned long end = time(NULL);
	utMsgPrintMsg(psMsgHead);
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgGetSomeNVar(psMsgHead, 14, 
				       "f", 	UT_TYPE_STRING, 3, 	caRe,
				       "rn",    UT_TYPE_STRING, 15, 	caRn,
	                               "lm",    UT_TYPE_STRING, 15, 	caLm, 
	                               "q",     UT_TYPE_STRING, 255, 	caQuery,           	
	                               "t",     UT_TYPE_STRING, 15,  	caTotal,          	
	                               "sf", 	UT_TYPE_STRING, 2, 	caSort_flag, 
	                               "q1",    UT_TYPE_STRING, 255,	caQ1,
	                               "q2",    UT_TYPE_STRING, 255,	caQ2,
	                               "q3",    UT_TYPE_STRING, 255,	caQ3,
	                               "q4",    UT_TYPE_STRING, 255,	caQ4,
	                               "q5",    UT_TYPE_STRING, 15, 	caQ5,//查询字段
	                               "q6",    UT_TYPE_STRING, 15, 	caQ6,
	                               "wd",    UT_TYPE_STRING, 255, 	wd, 
	                               "cp",    UT_TYPE_STRING, 8,  	ca_cur_pg); 
	utPltPutVar(psDbHead, "q1", caQ1);
	utPltPutVar(psDbHead, "q2", caQ2);
	utPltPutVar(psDbHead, "q3", caQ3);
	utPltPutVar(psDbHead, "q4", caQ4);
	utPltPutVar(psDbHead, "rn", caRn);
	utPltPutVar(psDbHead, "lm", caLm);
	if(strlen(caRn) > 0)/* 指定了搜索结果显示的条数 */ 
	{
	  	iSum = atoi(caRn);
                if(iSum == 10 || iSum == 20 || iSum == 50 || iSum == 100)
                {
                }
                else
                {
                        iSum = 10;
                }
	}
	else /* 默认10条 */
	{
		iSum = 10;
	}
	lCp = atol(ca_cur_pg);
	if (lCp == 0) 
		lCp = 1;
        iStart = (lCp - 1) * iSum;
	lCount = 0;
	utPltPutVarF(psDbHead, "cp", "%lu", lCp);
	utPltPutVarF(psDbHead, "start", "%lu", iStart+1);
	
	
	/////////////////////////////////////////////////////////////////////////////////
	if(strlen(caQ1) > 0 && !utStrIsSpaces(caQ1))
	{
	  	snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "%s ", caQ1);
	}
	if(strlen(caQ3) > 0 && !utStrIsSpaces(caQ3))
	{
		char *word = strtok(caQ3, SEPCHARS);
		if(word != NULL)
			if(strlen(ca_query_text) > 0)
				snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "AND ");
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "(%s", word);	
		word = strtok(NULL, SEPCHARS);
		while(word != NULL)
		{
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " OR %s", word);
			word = strtok(NULL, SEPCHARS);
		}
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text),  ")");
	}  
	if(strlen(caQ4) > 0 && !utStrIsSpaces(caQ4))
	{
		if(strlen(ca_query_text) > 0)
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " AND (");
		char *word = strtok(caQ4, SEPCHARS);
		while(word != NULL)
		{
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "NOT %s", word);
			word = strtok(NULL, SEPCHARS);
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "%s", word ? " " : "");
		}
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text),  ")");
	} 
	if(strlen(caQ2) > 0 && !utStrIsSpaces(caQ2))
	{
	  	if(strlen(ca_query_text) > 0)
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " ");
	  	snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "\"%s\"", caQ2);
	}
	if(strlen(wd) == 0 || utStrIsSpaces(wd))
	{
		memset(wd, 0, sizeof(wd));
		strncpy(wd, ca_query_text, sizeof(wd)-1);
	}
	
	strncpy(query, wd, sizeof(query)-1);
	str_replace(query, '\"',  "&quot;");
	utPltPutVar(psDbHead, "query", query);
	/////////////////////////////////////////////////////////////////////////////////
	
	
	if(strcmp(caRe, "re") == 0)
	{
	  	utPltPutVar(psDbHead, "query_text", caQuery);
      		utPltPutVar(psDbHead, "total",      caTotal);
      		return utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/re_bbs_search.html");
  	} 
  	if(strlen(caSort_flag) == 0 || strcmp(caSort_flag, "0") == 0)/* 按时间排序 */
	{
		iSortFlag = 1;
	}
	
	unsigned long lLm = atol(caLm);
	if(lLm == 1 || lLm == 7 || lLm == 30 || lLm == 365)
		start = end - lLm * 86400;
	else start = 0;
		
	switch(atol(caQ5))
	{
		case 1:
			field = "dispname";
			break;
		case 2:
			field = "bbsname";
			break;
		case 3:
			field = "url";
			break;
		case 6:
			field = "subject";
			break;	
		case 7:
			field = "content";
			break;	
		case 8:
			field = "sip";
			break;	
		case 9:
			field = "dip";
			break;	
		default:
			break;
	}
	
	utMsgGetSomeNVar(psMsgHead, 4, 
	                               "q1",    UT_TYPE_STRING, 255,	caQ1,
	                               "q2",    UT_TYPE_STRING, 255,	caQ2,
	                               "q3",    UT_TYPE_STRING, 255,	caQ3,
	                               "q4",    UT_TYPE_STRING, 255,	caQ4); 
	if(!utStrIsSpaces(caQ1) || !utStrIsSpaces(caQ2) || !utStrIsSpaces(caQ3)) //指定了高级查询
	{
		jsonstr = createMatchBBSTermJsonString(field, iStart, iSum, iSortFlag, "", start, end, caQ1, caQ2, caQ3, caQ4, "");
	}
	else if(!utStrIsSpaces(wd))
	{
		get_stbuf(wd, "\"", '\"', caQ2, sizeof(caQ2)-1);
		if(strcasestr(wd, "AND") || strcasestr(wd, "OR") || strcasestr(wd, "NOT") )
		{
			strcpy(qeystr, wd);
			str_delstr(qeystr, "\"");
			if(!utStrIsSpaces(caQ2))
			{
				str_delstr(qeystr, caQ2);
			}
			if(qeystr[strlen(qeystr)-1] == ' ')
			{
				qeystr[strlen(qeystr)-1] = '\0';
			}
			printf("qeystr = %s, caQ2 = %s\n", qeystr, caQ2);
			jsonstr = createMatchBBSTermJsonString(field, iStart, iSum, iSortFlag, "", start, end, "", caQ2, "", "", qeystr);
		}
		else 
		{
			if(strlen(caQ2) > 0 ) 
				wd[0] = '\0';
			jsonstr = createMatchBBSTermJsonString(field, iStart, iSum, iSortFlag, wd, start, end, "", caQ2, "", "", "");
		}
	}
	else 
	{
		jsonstr = createMatchAllJsonString(iStart, iSum, iSortFlag);
	}
  	curl_global_init(CURL_GLOBAL_ALL);
  	curl = curl_easy_init();
  	if(!curl)
  		return 0;
  	curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:9200/dc/bbs/_search?pretty=true");
  	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE,  1L);
  	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  	memset(&wdata, 0, sizeof(struct curl_data));
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA,  &wdata);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr);//for XGET
	res = curl_easy_perform(curl);
    	if(res == CURLE_OK)
    	{
    		wdata.buf[wdata.shift] = '\0';
    		char word[1536] = "";
    		if(!utStrIsSpaces(wd))
    			snprintf(word, sizeof(word), "%s", wd);
    		if(!utStrIsSpaces(caQ1))
    			snprintf(word, sizeof(word), " %s", caQ1);
    		if(!utStrIsSpaces(caQ2))
    			snprintf(word, sizeof(word), " %s", caQ2);
    		if(!utStrIsSpaces(caQ3))
    			snprintf(word, sizeof(word), " %s", caQ3);
    		if(!utStrIsSpaces(caQ4))
    			snprintf(word, sizeof(word), " %s", caQ4);
    		parse_bbs_jsonstring(psDbHead, iSum, wdata.buf, &took, (int *)&lCount, word);
    		
    		free(wdata.buf);
    	}
    	else
    	{
    		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    	}
    	curl_global_cleanup();
    	free(jsonstr);
    	if(lCount == 0 || iStart > lCount){
		utPltPutVar(psDbHead, "display2", "none");
		utPltPutVar(psDbHead, "display1", " ");
		return utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/search_bbs_result.html");
   	}
   	else 
	{    	
	        utPltPutVar(psDbHead, "display1", "none");
	}        
	unsigned long ltotpg = (lCount % iSum) ? (lCount / iSum + 1) : lCount / iSum;	//总页数 
	utPltPutVarF(psDbHead, "total", "%lu", lCount);
	utPltPutVarF(psDbHead, "total_times", "%d.%03d", took/1000, took%1000);
	utPltPutVarF(psDbHead, "end", "%lu", lCp == ltotpg ? lCount : iStart+iSum);
		
	strncpy(wd, query, sizeof(wd)-1);
	str_replace(caQ1, '\"',  "&quot;");
	str_replace(caQ2, '\"',  "&quot;");
	str_replace(caQ3, '\"',  "&quot;");
	str_replace(caQ4, '\"',  "&quot;");
    	if(lCp != 1 ){
		utPltPutVar(psDbHead, "prev", "上一页");
		utPltPutVarF(psDbHead, "CurPg_prev", "%d", lCp-1);
		utPltPutVar(psDbHead, "wd_prev",wd);
		utPltPutVar(psDbHead, "rn_prev", caRn);
		utPltPutVar(psDbHead, "lm_prev", caLm);
		utPltPutVar(psDbHead, "sf_prev", caSort_flag);
		
		utPltPutVar(psDbHead, "q1_prev", caQ1);
		utPltPutVar(psDbHead, "q2_prev", caQ2);
		utPltPutVar(psDbHead, "q3_prev", caQ3);
		utPltPutVar(psDbHead, "q4_prev", caQ4);
		utPltPutVar(psDbHead, "q5_prev", caQ5);
		utPltPutVar(psDbHead, "q6_prev", caQ6);
	}
	if(lCp != ltotpg && ltotpg > 0){
	
		utPltPutVar(psDbHead, "next", "下一页");
		utPltPutVarF(psDbHead, "CurPg_next", "%d", lCp+1);
		utPltPutVar(psDbHead, "wd_next",wd);
		utPltPutVar(psDbHead, "rn_next", caRn);
		utPltPutVar(psDbHead, "lm_next", caLm);
		utPltPutVar(psDbHead, "sf_next", caSort_flag);
		
		utPltPutVar(psDbHead, "q1_next", caQ1);
		utPltPutVar(psDbHead, "q2_next", caQ2);
		utPltPutVar(psDbHead, "q3_next", caQ3);
		utPltPutVar(psDbHead, "q4_next", caQ4);
		utPltPutVar(psDbHead, "q5_next", caQ5);
		utPltPutVar(psDbHead, "q6_next", caQ6);
   	}
	int i = 0;
   	if(lCp > 10){
	 	for(i = lCp - 10; i < lCp ; i++){ 
			utPltPutLoopVarF(psDbHead, "page1", i-(lCp - 11), "[%d]", i);
			utPltPutLoopVarF(psDbHead, "CurPg_page1", i-(lCp - 11), "%d", i);
			utPltPutLoopVar(psDbHead, "wd_page1",i-(lCp - 11),wd);
			utPltPutLoopVar(psDbHead, "rn_page1", i-(lCp - 11), caRn);
			utPltPutLoopVar(psDbHead, "lm_page1", i-(lCp - 11), caLm);
			utPltPutLoopVar(psDbHead, "sf_page1", i-(lCp - 11), caSort_flag);
			
			utPltPutLoopVar(psDbHead, "q1_page1", i-(lCp - 11), caQ1);
			utPltPutLoopVar(psDbHead, "q2_page1", i-(lCp - 11), caQ2);
			utPltPutLoopVar(psDbHead, "q3_page1", i-(lCp - 11), caQ3);
			utPltPutLoopVar(psDbHead, "q4_page1", i-(lCp - 11), caQ4);
			utPltPutLoopVar(psDbHead, "q5_page1", i-(lCp - 11), caQ5);
			utPltPutLoopVar(psDbHead, "q6_page1", i-(lCp - 11), caQ6);
		}
 	}
	
 	if(lCp <= 10){
	 	for(i = 1; i < lCp; i++){
			utPltPutLoopVarF(psDbHead, "page1", i, "[%d]", i);
			utPltPutLoopVarF(psDbHead, "CurPg_page1", i, "%d", i);
			utPltPutLoopVar(psDbHead, "wd_page1",i,wd);
			utPltPutLoopVar(psDbHead, "rn_page1", i, caRn);
			utPltPutLoopVar(psDbHead, "lm_page1", i, caLm);
			utPltPutLoopVar(psDbHead, "sf_page1", i, caSort_flag);
			
			utPltPutLoopVar(psDbHead, "q1_page1", i, caQ1);
			utPltPutLoopVar(psDbHead, "q2_page1", i, caQ2);
			utPltPutLoopVar(psDbHead, "q3_page1", i, caQ3);
			utPltPutLoopVar(psDbHead, "q4_page1", i, caQ4);
			utPltPutLoopVar(psDbHead, "q5_page1", i, caQ5);
			utPltPutLoopVar(psDbHead, "q6_page1", i, caQ6);
	 	}
 	}
 	if(ltotpg > 0)
	 	utPltPutVarF(psDbHead, "page",  "%d", lCp);
 
 	for(i = lCp+1; i < lCp+10 && i < ltotpg; i++){
	 	utPltPutLoopVarF(psDbHead, "page2", i-lCp, "[%d]", i);
	 	utPltPutLoopVarF(psDbHead, "CurPg_page2", i-lCp, "%d", i);
	 	utPltPutLoopVar(psDbHead, "wd_page2",i-lCp,wd);
	 	utPltPutLoopVar(psDbHead, "rn_page2", i-lCp, caRn);
     		utPltPutLoopVar(psDbHead, "lm_page2", i-lCp, caLm);
     		utPltPutLoopVar(psDbHead, "sf_page2",  i-lCp, caSort_flag);
     		
     		utPltPutLoopVar(psDbHead, "q1_page2",  i-lCp, caQ1);
     		utPltPutLoopVar(psDbHead, "q2_page2",  i-lCp, caQ2);
     		utPltPutLoopVar(psDbHead, "q3_page2",  i-lCp, caQ3);
     		utPltPutLoopVar(psDbHead, "q4_page2",  i-lCp, caQ4);
     		utPltPutLoopVar(psDbHead, "q5_page2",  i-lCp, caQ5);
     		utPltPutLoopVar(psDbHead, "q6_page2",  i-lCp, caQ6);
 	}
 	if(strcmp(caSort_flag, "1") == 0){	
 		utPltPutVarF(psDbHead, "sort1", "%s", "<b>已按相关性排序</b>");
 		utPltPutVarF(psDbHead, "sort2", "<a href=\"/pronline/Msg?FunName@ncsbbsmon_v4&wd@%s&q1@%s&q2@%s&q3@%s&q4@%s&cp@%d&rn@%s&lm@%s&sf@0\">按日期排序</a>",  wd, caQ1, caQ2, caQ3, caQ4, lCp, caRn, caLm);
 	}
 	else{
 		utPltPutVarF(psDbHead, "sort1", "<a href=\"/pronline/Msg?FunName@ncsbbsmon_v4&wd@%s&q1@%s&q2@%s&q3@%s&q4@%s&cp@%d&rn@%s&lm@%s&sf@1\">按相关性排序</a>",  wd, caQ1, caQ2, caQ3, caQ4, lCp, caRn, caLm);
 		utPltPutVarF(psDbHead, "sort2", "%s", "<b>已按日期排序</b>");
 	}
    	utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/search_bbs_result.html");
	return 0;
}
//http://192.168.20.51:9200/dc/_analyze?analyzer=ik&text=%E6%B5%8B%E8%AF%95elasticsearch%E5%88%86%E8%AF%8D%E5%99%A8
int ncsmailmon_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	char caRe[4] = "";
	char caRn[16] = "";
	char caLm[16] = "";
	char caQuery[256] = "";
	char caTotal[16] = "";
	char caSort_flag[3] = "";
	char caQ1[256] = "";
	char caQ2[256] = "";
	char caQ3[256] = "";
	char caQ4[256] = "";
	char caQ5[16] = "";
	char caQ6[16] = "";
	
	char ca_query_text[1024] = ""; 
	char wd[1024]= "";
	char query[1024] = "";
	char qeystr[1024] = "";
	
	char ca_cur_pg[9] = "";
	int iSortFlag = 0;
	int iSum = 0;
	int took = 0;
	struct curl_data wdata;
	CURL *curl;
  	CURLcode res;
	char *jsonstr = NULL;
	char *field = "all";
	unsigned long lCp = 0;
	unsigned long iStart = 0;
	unsigned long lCount = 0;
	unsigned long start = 0;
	unsigned long end = time(NULL);
	utMsgPrintMsg(psMsgHead);
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgGetSomeNVar(psMsgHead, 14, 
				       "f", 	UT_TYPE_STRING, 3, 	caRe,
				       "rn",    UT_TYPE_STRING, 15, 	caRn,
	                               "lm",    UT_TYPE_STRING, 15, 	caLm, 
	                               "q",     UT_TYPE_STRING, 255, 	caQuery,           	
	                               "t",     UT_TYPE_STRING, 15,  	caTotal,          	
	                               "sf", 	UT_TYPE_STRING, 2, 	caSort_flag, 
	                               "q1",    UT_TYPE_STRING, 255,	caQ1,
	                               "q2",    UT_TYPE_STRING, 255,	caQ2,
	                               "q3",    UT_TYPE_STRING, 255,	caQ3,
	                               "q4",    UT_TYPE_STRING, 255,	caQ4,
	                               "q5",    UT_TYPE_STRING, 15, 	caQ5,//查询字段
	                               "q6",    UT_TYPE_STRING, 15, 	caQ6,
	                               "wd",    UT_TYPE_STRING, 255, 	wd, 
	                               "cp",    UT_TYPE_STRING, 8,  	ca_cur_pg); 
	utPltPutVar(psDbHead, "q1", caQ1);
	utPltPutVar(psDbHead, "q2", caQ2);
	utPltPutVar(psDbHead, "q3", caQ3);
	utPltPutVar(psDbHead, "q4", caQ4);
	utPltPutVar(psDbHead, "rn", caRn);
	utPltPutVar(psDbHead, "lm", caLm);
	if(strlen(caRn) > 0)/* 指定了搜索结果显示的条数 */ 
	{
	  	iSum = atoi(caRn);
                if(iSum == 10 || iSum == 20 || iSum == 50 || iSum == 100)
                {
                }
                else
                {
                        iSum = 10;
                }
	}
	else /* 默认10条 */
	{
		iSum = 10;
	}
	lCp = atol(ca_cur_pg);
	if (lCp == 0) 
		lCp = 1;
        iStart = (lCp - 1) * iSum;
	lCount = 0;
	utPltPutVarF(psDbHead, "cp", "%lu", lCp);
	utPltPutVarF(psDbHead, "start", "%lu", iStart+1);
	
	
	/////////////////////////////////////////////////////////////////////////////////
	if(strlen(caQ1) > 0 && !utStrIsSpaces(caQ1))
	{
	  	snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "%s ", caQ1);
	}
	if(strlen(caQ3) > 0 && !utStrIsSpaces(caQ3))
	{
		char *word = strtok(caQ3, SEPCHARS);
		if(word != NULL)
			if(strlen(ca_query_text) > 0)
				snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "AND ");
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "(%s", word);		
		word = strtok(NULL, SEPCHARS);
		while(word != NULL)
		{
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " OR %s", word);
			word = strtok(NULL, SEPCHARS);
		}
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text),  ")");
	}  
	if(strlen(caQ4) > 0 && !utStrIsSpaces(caQ4))
	{
		if(strlen(ca_query_text) > 0)
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " AND (");
		char *word = strtok(caQ4, SEPCHARS);
		while(word != NULL)
		{
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "NOT %s", word);
			word = strtok(NULL, SEPCHARS);
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "%s", word ? " " : "");
		}
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text),  ")");
	} 
	if(strlen(caQ2) > 0 && !utStrIsSpaces(caQ2))
	{
	  	if(strlen(ca_query_text) > 0)
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " ");
	  	snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "\"%s\"", caQ2);
	}
	if(strlen(wd) == 0 || utStrIsSpaces(wd))
	{
		memset(wd, 0, sizeof(wd));
		strncpy(wd, ca_query_text, sizeof(wd)-1);
	}
	
	strncpy(query, wd, sizeof(query)-1);
	str_replace(query, '\"',  "&quot;");
	utPltPutVar(psDbHead, "query", query);
	/////////////////////////////////////////////////////////////////////////////////
	
	
	if(strcmp(caRe, "re") == 0)
	{
	  	utPltPutVar(psDbHead, "query_text", caQuery);
      		utPltPutVar(psDbHead, "total",      caTotal);
      		return utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/re_mail_search.html");
  	} 
  	if(strlen(caSort_flag) == 0 || strcmp(caSort_flag, "0") == 0)/* 按时间排序 */
	{
		iSortFlag = 1;
	}
	
	unsigned long lLm = atol(caLm);
	if(lLm == 1 || lLm == 7 || lLm == 30 || lLm == 365)
		start = end - lLm * 86400;
	else start = 0;
		
	switch(atol(caQ5))
	{
		case 1:
			field = "mailfrom";
			break;
		case 2:
			field = "mailto";
			break;
		case 3:
			field = "mailsub";
			break;
		case 4:
			field = "mailbody";
			break;	
		case 5:
			field = "attachment_name";
			break;	
		case 6:
			field = "attachment_content";
			break;	
		case 7:
			field = "dispname";
			break;	
		case 8:
			field = "sip";
			break;	
		case 9:
			field = "dip";
			break;	
		case 10:
			field = "mailcc";
			break;	
		default:
			break;
	}
	
	utMsgGetSomeNVar(psMsgHead, 4, 
	                               "q1",    UT_TYPE_STRING, 255,	caQ1,
	                               "q2",    UT_TYPE_STRING, 255,	caQ2,
	                               "q3",    UT_TYPE_STRING, 255,	caQ3,
	                               "q4",    UT_TYPE_STRING, 255,	caQ4); 
	if(!utStrIsSpaces(caQ1) || !utStrIsSpaces(caQ2) || !utStrIsSpaces(caQ3)) //指定了高级查询
	{
		jsonstr = createMatchMAILTermJsonString(field, iStart, iSum, iSortFlag, "", start, end, caQ1, caQ2, caQ3, caQ4, "");
	}
	else if(!utStrIsSpaces(wd))
	{
		get_stbuf(wd, "\"", '\"', caQ2, sizeof(caQ2)-1);
		if(strcasestr(wd, "AND") || strcasestr(wd, "OR") || strcasestr(wd, "NOT") )
		{
			strcpy(qeystr, wd);
			str_delstr(qeystr, "\"");
			if(!utStrIsSpaces(caQ2))
			{
				str_delstr(qeystr, caQ2);
			}
			if(qeystr[strlen(qeystr)-1] == ' ')
			{
				qeystr[strlen(qeystr)-1] = '\0';
			}
			printf("qeystr = %s, caQ2 = %s\n", qeystr, caQ2);
			jsonstr = createMatchMAILTermJsonString(field, iStart, iSum, iSortFlag, "", start, end, "", caQ2, "", "", qeystr);
		}
		else 
		{
			if(strlen(caQ2) > 0 ) 
				wd[0] = '\0';
			jsonstr = createMatchMAILTermJsonString(field, iStart, iSum, iSortFlag, wd, start, end, "", caQ2, "", "", "");
		}
	}
	else 
	{
		jsonstr = createMatchAllJsonString(iStart, iSum, iSortFlag);
	}
  	curl_global_init(CURL_GLOBAL_ALL);
  	curl = curl_easy_init();
  	if(!curl)
  		return 0;
  	curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:9200/dc/mail/_search?pretty=true");
  	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE,  1L);
  	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  	memset(&wdata, 0, sizeof(struct curl_data));
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA,  &wdata);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr);//for XGET
	res = curl_easy_perform(curl);
    	if(res == CURLE_OK)
    	{
    		wdata.buf[wdata.shift] = '\0';
    		char word[1536] = "";
    		if(!utStrIsSpaces(wd))
    			snprintf(word, sizeof(word), "%s", wd);
    		if(!utStrIsSpaces(caQ1))
    			snprintf(word, sizeof(word), " %s", caQ1);
    		if(!utStrIsSpaces(caQ2))
    			snprintf(word, sizeof(word), " %s", caQ2);
    		if(!utStrIsSpaces(caQ3))
    			snprintf(word, sizeof(word), " %s", caQ3);
    		if(!utStrIsSpaces(caQ4))
    			snprintf(word, sizeof(word), " %s", caQ4);
    		parse_mail_jsonstring(psDbHead, iSum, wdata.buf, &took, (int *)&lCount, word);
    		free(wdata.buf);
    	}
    	else
    	{
    		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    	}
    	curl_global_cleanup();
    	free(jsonstr);
    	if(lCount == 0 || iStart > lCount){
		utPltPutVar(psDbHead, "display2", "none");
		utPltPutVar(psDbHead, "display1", " ");
		return utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/search_mail_result.html");
   	}
   	else 
	{    	
	        utPltPutVar(psDbHead, "display1", "none");
	}        
	unsigned long ltotpg = (lCount % iSum) ? (lCount / iSum + 1) : lCount / iSum;	//总页数 
	utPltPutVarF(psDbHead, "total", "%lu", lCount);
	utPltPutVarF(psDbHead, "total_times", "%d.%03d", took/1000, took%1000);
	utPltPutVarF(psDbHead, "end", "%lu", lCp == ltotpg ? lCount : iStart+iSum);
		
	strncpy(wd, query, sizeof(wd)-1);
	str_replace(caQ1, '\"',  "&quot;");
	str_replace(caQ2, '\"',  "&quot;");
	str_replace(caQ3, '\"',  "&quot;");
	str_replace(caQ4, '\"',  "&quot;");
    	if(lCp != 1 ){
		utPltPutVar(psDbHead, "prev", "上一页");
		utPltPutVarF(psDbHead, "CurPg_prev", "%d", lCp-1);
		utPltPutVar(psDbHead, "wd_prev",wd);
		utPltPutVar(psDbHead, "rn_prev", caRn);
		utPltPutVar(psDbHead, "lm_prev", caLm);
		utPltPutVar(psDbHead, "sf_prev", caSort_flag);
		
		utPltPutVar(psDbHead, "q1_prev", caQ1);
		utPltPutVar(psDbHead, "q2_prev", caQ2);
		utPltPutVar(psDbHead, "q3_prev", caQ3);
		utPltPutVar(psDbHead, "q4_prev", caQ4);
		utPltPutVar(psDbHead, "q5_prev", caQ5);
		utPltPutVar(psDbHead, "q6_prev", caQ6);
	}
	if(lCp != ltotpg && ltotpg > 0){
	
		utPltPutVar(psDbHead, "next", "下一页");
		utPltPutVarF(psDbHead, "CurPg_next", "%d", lCp+1);
		utPltPutVar(psDbHead, "wd_next",wd);
		utPltPutVar(psDbHead, "rn_next", caRn);
		utPltPutVar(psDbHead, "lm_next", caLm);
		utPltPutVar(psDbHead, "sf_next", caSort_flag);
		
		utPltPutVar(psDbHead, "q1_next", caQ1);
		utPltPutVar(psDbHead, "q2_next", caQ2);
		utPltPutVar(psDbHead, "q3_next", caQ3);
		utPltPutVar(psDbHead, "q4_next", caQ4);
		utPltPutVar(psDbHead, "q5_next", caQ5);
		utPltPutVar(psDbHead, "q6_next", caQ6);
   	}
	int i = 0;
   	if(lCp > 10){
	 	for(i = lCp - 10; i < lCp ; i++){ 
			utPltPutLoopVarF(psDbHead, "page1", i-(lCp - 11), "[%d]", i);
			utPltPutLoopVarF(psDbHead, "CurPg_page1", i-(lCp - 11), "%d", i);
			utPltPutLoopVar(psDbHead, "wd_page1",i-(lCp - 11),wd);
			utPltPutLoopVar(psDbHead, "rn_page1", i-(lCp - 11), caRn);
			utPltPutLoopVar(psDbHead, "lm_page1", i-(lCp - 11), caLm);
			utPltPutLoopVar(psDbHead, "sf_page1", i-(lCp - 11), caSort_flag);
			
			utPltPutLoopVar(psDbHead, "q1_page1", i-(lCp - 11), caQ1);
			utPltPutLoopVar(psDbHead, "q2_page1", i-(lCp - 11), caQ2);
			utPltPutLoopVar(psDbHead, "q3_page1", i-(lCp - 11), caQ3);
			utPltPutLoopVar(psDbHead, "q4_page1", i-(lCp - 11), caQ4);
			utPltPutLoopVar(psDbHead, "q5_page1", i-(lCp - 11), caQ5);
			utPltPutLoopVar(psDbHead, "q6_page1", i-(lCp - 11), caQ6);
		}
 	}
	
 	if(lCp <= 10){
	 	for(i = 1; i < lCp; i++){
			utPltPutLoopVarF(psDbHead, "page1", i, "[%d]", i);
			utPltPutLoopVarF(psDbHead, "CurPg_page1", i, "%d", i);
			utPltPutLoopVar(psDbHead, "wd_page1",i,wd);
			utPltPutLoopVar(psDbHead, "rn_page1", i, caRn);
			utPltPutLoopVar(psDbHead, "lm_page1", i, caLm);
			utPltPutLoopVar(psDbHead, "sf_page1", i, caSort_flag);
			
			utPltPutLoopVar(psDbHead, "q1_page1", i, caQ1);
			utPltPutLoopVar(psDbHead, "q2_page1", i, caQ2);
			utPltPutLoopVar(psDbHead, "q3_page1", i, caQ3);
			utPltPutLoopVar(psDbHead, "q4_page1", i, caQ4);
			utPltPutLoopVar(psDbHead, "q5_page1", i, caQ5);
			utPltPutLoopVar(psDbHead, "q6_page1", i, caQ6);
	 	}
 	}
 	if(ltotpg > 0)
	 	utPltPutVarF(psDbHead, "page",  "%d", lCp);
 
 	for(i = lCp+1; i < lCp+10 && i < ltotpg; i++){
	 	utPltPutLoopVarF(psDbHead, "page2", i-lCp, "[%d]", i);
	 	utPltPutLoopVarF(psDbHead, "CurPg_page2", i-lCp, "%d", i);
	 	utPltPutLoopVar(psDbHead, "wd_page2",i-lCp,wd);
	 	utPltPutLoopVar(psDbHead, "rn_page2", i-lCp, caRn);
     		utPltPutLoopVar(psDbHead, "lm_page2", i-lCp, caLm);
     		utPltPutLoopVar(psDbHead, "sf_page2",  i-lCp, caSort_flag);
     		
     		utPltPutLoopVar(psDbHead, "q1_page2",  i-lCp, caQ1);
     		utPltPutLoopVar(psDbHead, "q2_page2",  i-lCp, caQ2);
     		utPltPutLoopVar(psDbHead, "q3_page2",  i-lCp, caQ3);
     		utPltPutLoopVar(psDbHead, "q4_page2",  i-lCp, caQ4);
     		utPltPutLoopVar(psDbHead, "q5_page2",  i-lCp, caQ5);
     		utPltPutLoopVar(psDbHead, "q6_page2",  i-lCp, caQ6);
 	}
 	if(strcmp(caSort_flag, "1") == 0){	
 		utPltPutVarF(psDbHead, "sort1", "%s", "<b>已按相关性排序</b>");
 		utPltPutVarF(psDbHead, "sort2", "<a href=\"/pronline/Msg?FunName@ncsmailmon_v4&wd@%s&q1@%s&q2@%s&q3@%s&q4@%s&cp@%d&rn@%s&lm@%s&sf@0\">按日期排序</a>",  wd, caQ1, caQ2, caQ3, caQ4, lCp, caRn, caLm);
 	}
 	else{
 		utPltPutVarF(psDbHead, "sort1", "<a href=\"/pronline/Msg?FunName@ncsmailmon_v4&wd@%s&q1@%s&q2@%s&q3@%s&q4@%s&cp@%d&rn@%s&lm@%s&sf@1\">按相关性排序</a>",  wd, caQ1, caQ2, caQ3, caQ4, lCp, caRn, caLm);
 		utPltPutVarF(psDbHead, "sort2", "%s", "<b>已按日期排序</b>");
 	}
    	utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/search_mail_result.html");
	return 0;
}


void parse_fulltext_jsonstring(utPltDbHead *psDbHead, int iSum, char *jsonstring, int *took, int *count, char *query)
{
	char *tmputf8 = iconv_convert("GBK", "UTF-8", query);
	if(tmputf8 == NULL) return;
	int *v_did = (int *)calloc(iSum, sizeof(int));
	if(v_did == NULL)
	{
		free(tmputf8);
		return;
	}
		
	int *v_uid = (int *)calloc(iSum, sizeof(int));
	if(v_uid == NULL)
	{	free(v_did);
		free(tmputf8);
		return;
	}
	sphinx_client *client = NULL;	
	char **res = NULL;
	char *docs[1];
	cJSON* pRoot = cJSON_Parse ( jsonstring );
	cJSON *pArrayItem = NULL;
	cJSON *pItem = NULL;
	cJSON *pItem1 = NULL;
	cJSON *pItem2 = NULL;
	int i;
	int cnt;
	int iNum = 0;
	int j = 0;
	int cnt2 = 0;
	char *p = NULL;
	char *p1 = NULL;
	
	//char *out=cJSON_Print(pRoot);
	//printf("recv = %s\n",out);
	//free(out);
	
	//printf("pid = %d\n", getpid());
	//sleep(20);
	
	*took = cJSON_GetObjectItem( pRoot, "took") ? cJSON_GetObjectItem( pRoot, "took")->valueint : 0;
	if(*took == 0)
	{
		*count = 0;
		cJSON_Delete(pRoot);
		return;
	}
	
	if((pItem = cJSON_GetObjectItem ( pRoot, "hits")) && (*count=cJSON_GetObjectItem( pItem, "total")->valueint,pItem = cJSON_GetObjectItem ( pItem, "hits")))
	{
		cnt = cJSON_GetArraySize( pItem);
		
		struct curl_data data_dispname;
		struct curl_data data_sip;
		struct curl_data data_dip;
		struct curl_data data_url;
		struct curl_data data_mailsub;
		struct curl_data data_mailfrom;
		struct curl_data data_mailto;
		
		struct curl_data data_mailcc;
		struct curl_data data_mailbody;
		struct curl_data data_formbody;
		
		struct curl_data data_attachment_name;
		struct curl_data data_attachment_content;
		struct curl_data data_attachment_content_orig;
		
		struct curl_data data_mfrom;
		struct curl_data data_mto;
		struct curl_data data_mesg;
		
		struct curl_data data_bbsname;
		struct curl_data data_subject;
		struct curl_data data_content;
		
		char *highlightstr = NULL;
		char *orig_str = NULL;
		for( i= 0; i < cnt; i++)
                {
                	memset(&data_dispname, 0, sizeof(struct curl_data));
                	memset(&data_sip, 0, sizeof(struct curl_data));
                	memset(&data_dip, 0, sizeof(struct curl_data));
                	
                	memset(&data_mailsub, 0, sizeof(struct curl_data));
                	memset(&data_mailfrom, 0, sizeof(struct curl_data));
                	memset(&data_mailto, 0, sizeof(struct curl_data));
                	
                	memset(&data_mailcc, 0, sizeof(struct curl_data));
                	memset(&data_formbody, 0, sizeof(struct curl_data));
                	memset(&data_mailbody, 0, sizeof(struct curl_data));
                	
                	memset(&data_mfrom,    0, sizeof(struct curl_data));
                	memset(&data_mto,    0, sizeof(struct curl_data));
                	memset(&data_mesg,    0, sizeof(struct curl_data));
                	memset(&data_url, 0, sizeof(struct curl_data));
                	memset(&data_bbsname, 0, sizeof(struct curl_data));
                	memset(&data_subject, 0, sizeof(struct curl_data));
                	memset(&data_content, 0, sizeof(struct curl_data));
                	
                	memset(&data_attachment_name, 0, sizeof(struct curl_data));
                	memset(&data_attachment_content, 0, sizeof(struct curl_data));
                	
                	pArrayItem = cJSON_GetArrayItem(pItem, i);
                	if((pItem1 = cJSON_GetObjectItem ( pArrayItem, "highlight")))
                	{
                		
                		pItem2 = cJSON_GetObjectItem ( pItem1, "bbsname");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_bbsname);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "url");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_url);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "subject");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_subject);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "content");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_content);
                			}
                		}
                		
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mfrom");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mfrom);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mto");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mto);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mesg");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mesg);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "dispname");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_dispname);
                			}	
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "formbody");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_formbody);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "sip");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_sip);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "dip");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_dip);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mailsub");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mailsub);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mailfrom");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mailfrom);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mailto");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mailto);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mailcc");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mailcc);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "mailbody");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_mailbody);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "attachment_name");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_attachment_name);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "attachment_content");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_attachment_content);
                			}
                		}
                	}
                	if( strcmp(cJSON_GetObjectItem ( pArrayItem, "_type")->valuestring, "mail") == 0 && (pItem1 = cJSON_GetObjectItem ( pArrayItem, "fields")) && (pItem1 = cJSON_GetObjectItem ( pItem1, "partial1")))
                	{
                		pItem2 = NULL;
                		pItem1 = pItem2 ? pItem2 : pItem1;
                		v_did[iNum] = cJSON_GetObjectItem(pItem1, "did")->valueint;
  				v_uid[iNum] = cJSON_GetObjectItem(pItem1, "uid")->valueint;
                		iNum++;
  				if(iNum%2)
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  " ");
  				}	
  				else
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  "class=\"tbl_alt\"");
  				}
  				utPltPutLoopVarF(psDbHead,"iNum", iNum, "%lu", iNum);
  				
  				
  				utPltPutLoopVar(psDbHead, "mail", iNum, "");
  				utPltPutLoopVar(psDbHead, "im", iNum, "none");
  				utPltPutLoopVar(psDbHead, "bbs", iNum, "none");
  				utPltPutLoopVar(psDbHead, "form", iNum, "none");
  				utPltPutLoopVar(psDbHead, "type", iNum, "邮件");
  				//
  				
  				
  				utPltPutLoopVar(psDbHead,"id", iNum, cJSON_GetObjectItem ( pArrayItem, "_id")->valuestring);
  				utPltPutLoopVarF(psDbHead,"style", iNum, "%d", cJSON_GetObjectItem ( pItem1, "style") ? cJSON_GetObjectItem ( pItem1, "style")->valueint : 1);
  				utPltPutLoopVarF(psDbHead, "userid", iNum, "%d", cJSON_GetObjectItem(pItem1, "uid")->valueint);
  				utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S",cJSON_GetObjectItem(pItem1, "stime")->valueint));
  				
  				printvar(client, psDbHead, pItem1, "udisp", "dispname", tmputf8, data_dispname, iNum);
                		printvar(client, psDbHead, pItem1, "sip", "sip", tmputf8, data_sip, iNum);
  				printvar(client, psDbHead, pItem1, "dip", "dip", tmputf8, data_dip, iNum);
  				printvar(client, psDbHead, pItem1, "sub", "mailsub", tmputf8, data_mailsub, iNum);
                		printvar(client, psDbHead, pItem1, "mfrom", "mailfrom", tmputf8, data_mailfrom, iNum);
  				printvar(client, psDbHead, pItem1, "mto", "mailto", tmputf8, data_mailto, iNum);
  				//printvar(client, psDbHead, pItem1, "mcc", "mailcc", tmputf8, data_mailcc, iNum);
  				printvar(client, psDbHead, pItem1, "body", "mailbody", tmputf8, data_mailbody, iNum);
				
				//邮件附件正文
                		if(data_attachment_content.buf != NULL)
                		{
                			_utPltPutLoopVar(psDbHead, "att_body", iNum, data_attachment_content.buf ? data_attachment_content.buf : "");	
                		}
                		else 
                		{
                			memset(&data_attachment_content_orig, 0, sizeof(struct curl_data));
  					cnt2 = cJSON_GetArraySize(cJSON_GetObjectItem(pItem1, "attachment_content"));
                			for( j = 0; j < cnt2; j++)
                			{
                				orig_str = cJSON_GetArrayItem(cJSON_GetObjectItem(pItem1, "attachment_content"), j)->valuestring;
                				write_data(orig_str, sizeof(char), strlen(orig_str), &data_attachment_content_orig);
                			}
					if(client == NULL)
					{
						client = sphinx_create ( SPH_TRUE, 5331);
					}
					if(client != NULL)
					{
						docs[0] = data_attachment_content_orig.buf;
						res = sphinx_build_excerpts ( client, 1, (const char **)docs, "dc", !utStrIsSpaces(tmputf8) ? tmputf8 : "", NULL);
						if(res)
						{
							_utPltPutLoopVar(psDbHead, "att_body", iNum, res[0]);
							free(res[0]);
							free(res);
						}
					}
                			free(data_attachment_content_orig.buf);
					data_attachment_content_orig.shift = 0;	
                		}
				
				//邮件附件名称
				cnt2 = cJSON_GetArraySize(cJSON_GetObjectItem(pItem1, "attachment_name"));
                		for( j = 0; j < cnt2; j++)
                		{
					if(client == NULL)
					{
							client = sphinx_create ( SPH_TRUE, 5331);
					}
					if(client != NULL)
					{
						docs[0] = cJSON_GetArrayItem(cJSON_GetObjectItem(pItem1, "attachment_name"), j)->valuestring;
						_utPltPutLoopVar2(psDbHead, "fname", iNum, j+1, docs[0]);
						
						utPltPutLoopVarF2(psDbHead, "idx", iNum, j+1, "%lu", j+1);
						utPltPutLoopVar2(psDbHead, "id", iNum, j+1, cJSON_GetObjectItem ( pArrayItem, "_id")->valuestring);
						
						
						if( (p = strstr(docs[0], "_")) && (p1 = strstr(p+1, "_")) &&(p = strstr(p1+1, "_")) )
						if(p == NULL)
							continue;
						docs[0] = p+1;
						res = sphinx_build_excerpts ( client, 1, (const char **)docs, "dc", !utStrIsSpaces(tmputf8) ? tmputf8 : "", NULL);
						if(res)
						{
							_utPltPutLoopVar2(psDbHead, "att_name", iNum, j+1, res[0]);
							free(res[0]);
							free(res);
						}
					}
                		}
                		
                		//cleanup
                		if(data_dispname.buf) free(data_dispname.buf);
                		if(data_sip.buf) free(data_sip.buf);
                		if(data_dip.buf) free(data_dip.buf);
                		if(data_mailsub.buf) free(data_mailsub.buf);
                		if(data_mailfrom.buf) free(data_mailfrom.buf);
                		if(data_mailto.buf) free(data_mailto.buf);	
                		if(data_mailcc.buf) free(data_mailcc.buf);
                		if(data_mailbody.buf) free(data_mailbody.buf);	
                		if(data_attachment_name.buf) free(data_attachment_name.buf);
                		if(data_attachment_content.buf) free(data_attachment_content.buf);
                		data_dispname.shift = 0;
                		data_sip.shift = 0;
                		data_dip.shift = 0;
                		data_mailsub.shift = 0;
                		data_mailfrom.shift = 0;
                		data_mailto.shift = 0;
                		data_mailcc.shift = 0;
                		data_mailbody.shift = 0;
                		data_attachment_name.shift = 0;
                		data_attachment_content.shift = 0;
                	}
                	else if(strcmp(cJSON_GetObjectItem ( pArrayItem, "_type")->valuestring, "im") == 0 && (pItem1 = cJSON_GetObjectItem ( pArrayItem, "fields")) && (pItem1 = cJSON_GetObjectItem ( pItem1, "partial1")))
                	{
                		pItem2 = NULL;
                		pItem1 = pItem2 ? pItem2 : pItem1;
                		v_did[iNum] = cJSON_GetObjectItem(pItem1, "did")->valueint;
  				v_uid[iNum] = cJSON_GetObjectItem(pItem1, "uid")->valueint;
                		iNum++;
  				if(iNum%2)
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  " ");
  				}	
  				else
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  "class=\"tbl_alt\"");
  				}
  				utPltPutLoopVarF(psDbHead,"iNum", iNum, "%lu", iNum);
  				
  				
  				utPltPutLoopVar(psDbHead, "mail", iNum, "none");
  				utPltPutLoopVar(psDbHead, "im", iNum, "");
  				utPltPutLoopVar(psDbHead, "bbs", iNum, "none");
  				utPltPutLoopVar(psDbHead, "form", iNum, "none");
  				utPltPutLoopVar(psDbHead, "type", iNum, "即时通讯");
  				
  				utPltPutLoopVar(psDbHead, "service", iNum, get_ntype(cJSON_GetObjectItem(pItem1, "service")->valueint));
  				utPltPutLoopVarF(psDbHead, "userid", iNum, "%d", cJSON_GetObjectItem(pItem1, "uid")->valueint);
  				utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S",cJSON_GetObjectItem(pItem1, "stime")->valueint));
  				
  				printvar(client, psDbHead, pItem1, "udisp", "dispname", tmputf8, data_dispname, iNum);
  				printvar(client, psDbHead, pItem1, "sip", "sip", tmputf8, data_sip, iNum);
  				printvar(client, psDbHead, pItem1, "dip", "dip", tmputf8, data_dip, iNum);
  				printvar(client, psDbHead, pItem1, "mfrom", "mfrom", tmputf8, data_mfrom, iNum);
  				printvar(client, psDbHead, pItem1, "mto", "mto", tmputf8, data_mto, iNum);
  				printvar(client, psDbHead, pItem1, "mesg", "mesg", tmputf8, data_mesg, iNum);
                		
                		//cleanup
                		if(data_dispname.buf) free(data_dispname.buf);
                		if(data_sip.buf) free(data_sip.buf);
                		if(data_dip.buf) free(data_dip.buf);
                		if(data_mfrom.buf) free(data_mfrom.buf);
                		if(data_mto.buf) free(data_mto.buf);
                		if(data_mesg.buf) free(data_mesg.buf);
                		data_dispname.shift = 0;
                		data_sip.shift = 0;
                		data_dip.shift = 0;
                		data_mfrom.shift = 0;
                		data_mto.shift = 0;
                		data_mesg.shift = 0;
                	}
                	else if(strcmp(cJSON_GetObjectItem ( pArrayItem, "_type")->valuestring, "bbs") == 0 && (pItem1 = cJSON_GetObjectItem ( pArrayItem, "fields")) && (pItem1 = cJSON_GetObjectItem ( pItem1, "partial1")))
                	{
                		pItem2 = NULL;
                		pItem1 = pItem2 ? pItem2 : pItem1;
                		v_did[iNum] = cJSON_GetObjectItem(pItem1, "did")->valueint;
  				v_uid[iNum] = cJSON_GetObjectItem(pItem1, "uid")->valueint;
                		iNum++;
  				if(iNum%2)
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  " ");
  				}	
  				else
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  "class=\"tbl_alt\"");
  				}
  				utPltPutLoopVarF(psDbHead,"iNum", iNum, "%lu", iNum);
  				
  				utPltPutLoopVar(psDbHead, "mail", iNum, "none");
  				utPltPutLoopVar(psDbHead, "im", iNum, "none");
  				utPltPutLoopVar(psDbHead, "bbs", iNum, "");
  				utPltPutLoopVar(psDbHead, "form", iNum, "none");
  				utPltPutLoopVar(psDbHead, "type", iNum, "BBS");
  				
  				utPltPutLoopVar(psDbHead,"id", iNum, cJSON_GetObjectItem ( pArrayItem, "_id")->valuestring);
  				utPltPutLoopVarF(psDbHead, "userid", iNum, "%d", cJSON_GetObjectItem(pItem1, "uid")->valueint);
  				utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S",cJSON_GetObjectItem(pItem1, "stime")->valueint));
  				_utPltPutLoopVar(psDbHead, "url_no_highlighter", iNum, cJSON_GetObjectItem(pItem1, "url")->valuestring);
  				
                		printvar(client, psDbHead, pItem1, "udisp", "dispname", tmputf8, data_dispname, iNum);
                		printvar(client, psDbHead, pItem1, "sip", "sip", tmputf8, data_sip, iNum);
  				printvar(client, psDbHead, pItem1, "dip", "dip", tmputf8, data_dip, iNum);
  				printvar(client, psDbHead, pItem1, "bbsname", "bbsname", tmputf8, data_bbsname, iNum);
  				printvar(client, psDbHead, pItem1, "url", "url", tmputf8, data_url, iNum);
  				printvar(client, psDbHead, pItem1, "subject", "subject", tmputf8, data_subject, iNum);
  				printvar(client, psDbHead, pItem1, "content", "content", tmputf8, data_content, iNum);
  				
                		
                		//cleanup
                		if(data_dispname.buf) free(data_dispname.buf);
                		if(data_sip.buf) free(data_sip.buf);
                		if(data_dip.buf) free(data_dip.buf);
                		if(data_bbsname.buf) free(data_bbsname.buf);
                		if(data_url.buf) free(data_url.buf);
                		if(data_subject.buf) free(data_subject.buf);
                		if(data_content.buf) free(data_content.buf);
                		data_dispname.shift = 0;
                		data_sip.shift = 0;
                		data_dip.shift = 0;
                		data_bbsname.shift = 0;
                		data_url.shift = 0;
                		data_subject.shift = 0;
                		data_content.shift = 0;
                	}
                	else if(strcmp(cJSON_GetObjectItem ( pArrayItem, "_type")->valuestring, "form") == 0 && (pItem1 = cJSON_GetObjectItem ( pArrayItem, "fields")) && (pItem1 = cJSON_GetObjectItem ( pItem1, "partial1")))
                	{
                		pItem2 = NULL;
                		pItem1 = pItem2 ? pItem2 : pItem1;
                		v_did[iNum] = cJSON_GetObjectItem(pItem1, "did")->valueint;
  				v_uid[iNum] = cJSON_GetObjectItem(pItem1, "uid")->valueint;
                		iNum++;
  				if(iNum%2)
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  " ");
  				}	
  				else
  				{        
  					utPltPutLoopVar(psDbHead, "class", iNum,  "class=\"tbl_alt\"");
  				}
  				utPltPutLoopVarF(psDbHead,"iNum", iNum, "%lu", iNum);
  				
  				utPltPutLoopVar(psDbHead, "mail", iNum, "none");
  				utPltPutLoopVar(psDbHead, "im", iNum, "none");
  				utPltPutLoopVar(psDbHead, "bbs", iNum, "none");
  				utPltPutLoopVar(psDbHead, "form", iNum, "");
  				utPltPutLoopVar(psDbHead, "type", iNum, "表单");
  				
  				utPltPutLoopVarF(psDbHead, "userid", iNum, "%d", cJSON_GetObjectItem(pItem1, "uid")->valueint);
  				utPltPutLoopVar(psDbHead,"id", iNum, cJSON_GetObjectItem ( pArrayItem, "_id")->valuestring);
  				_utPltPutLoopVar(psDbHead, "url_no_highlighter", iNum, cJSON_GetObjectItem(pItem1, "url")->valuestring);
  				utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S",cJSON_GetObjectItem(pItem1, "stime")->valueint));
  				
  				printvar(client, psDbHead, pItem1, "udisp", "dispname", tmputf8, data_dispname, iNum);
                		printvar(client, psDbHead, pItem1, "sip", "sip", tmputf8, data_sip, iNum);
  				printvar(client, psDbHead, pItem1, "dip", "dip", tmputf8, data_dip, iNum);
  				printvar(client, psDbHead, pItem1, "url", "url", tmputf8, data_url, iNum);
  				printvar(client, psDbHead, pItem1, "body", "formbody", tmputf8, data_formbody, iNum);
				
				//表单附件正文
                		if(data_attachment_content.buf != NULL)
                		{
                			_utPltPutLoopVar(psDbHead, "att_body", iNum, data_attachment_content.buf ? data_attachment_content.buf : "");	
                		}
                		else 
                		{
                			memset(&data_attachment_content_orig, 0, sizeof(struct curl_data));
  					cnt2 = cJSON_GetArraySize(cJSON_GetObjectItem(pItem1, "attachment_content"));
                			for( j = 0; j < cnt2; j++)
                			{
                				orig_str = cJSON_GetArrayItem(cJSON_GetObjectItem(pItem1, "attachment_content"), j)->valuestring;
                				write_data(orig_str, sizeof(char), strlen(orig_str), &data_attachment_content_orig);
                			}
					if(client == NULL)
					{
						client = sphinx_create ( SPH_TRUE, 5331);
					}
					if(client != NULL)
					{
						docs[0] = data_attachment_content_orig.buf;
						res = sphinx_build_excerpts ( client, 1, (const char **)docs, "dc", !utStrIsSpaces(tmputf8) ? tmputf8 : "", NULL);
						if(res)
						{
							_utPltPutLoopVar(psDbHead, "att_body", iNum, res[0]);
							free(res[0]);
							free(res);
						}
					}
                			free(data_attachment_content_orig.buf);
					data_attachment_content_orig.shift = 0;	
                		}
				
				//表单附件名称
				cnt2 = cJSON_GetArraySize(cJSON_GetObjectItem(pItem1, "attachment_name"));
                		for( j = 0; j < cnt2; j++)
                		{
					if(client == NULL)
					{
							client = sphinx_create ( SPH_TRUE, 5331);
					}
					if(client != NULL)
					{
						docs[0] = cJSON_GetArrayItem(cJSON_GetObjectItem(pItem1, "attachment_name"), j)->valuestring;
						_utPltPutLoopVar2(psDbHead, "fname", iNum, j+1, docs[0]);
						
						utPltPutLoopVarF2(psDbHead, "idx", iNum, j+1, "%lu", j+1);
						utPltPutLoopVar2(psDbHead, "id", iNum, j+1, cJSON_GetObjectItem ( pArrayItem, "_id")->valuestring);
						
						
						if( (p = strstr(docs[0], "_")) && (p1 = strstr(p+1, "_")) &&(p = strstr(p1+1, "_")) )
						if(p == NULL)
							continue;
						docs[0] = p+1;
						res = sphinx_build_excerpts ( client, 1, (const char **)docs, "dc", !utStrIsSpaces(tmputf8) ? tmputf8 : "", NULL);
						if(res)
						{
							_utPltPutLoopVar2(psDbHead, "att_name", iNum, j+1, res[0]);
							free(res[0]);
							free(res);
						}
					}
                		}
                		
                		//cleanup
                		if(data_dispname.buf) free(data_dispname.buf);
                		if(data_sip.buf) free(data_sip.buf);
                		if(data_dip.buf) free(data_dip.buf);
                		/*
                		if(data_mailsub.buf) free(data_mailsub.buf);
                		if(data_mailfrom.buf) free(data_mailfrom.buf);
                		if(data_mailto.buf) free(data_mailto.buf);	
                		if(data_mailcc.buf) free(data_mailcc.buf);
                		*/
                		if(data_url.buf) free(data_url.buf);	
                		if(data_formbody.buf) free(data_formbody.buf);	
                		if(data_attachment_name.buf) free(data_attachment_name.buf);
                		if(data_attachment_content.buf) free(data_attachment_content.buf);
                		data_dispname.shift = 0;
                		data_sip.shift = 0;
                		data_dip.shift = 0;
                		/*
                		data_mailsub.shift = 0;
                		data_mailfrom.shift = 0;
                		data_mailto.shift = 0;
                		data_mailcc.shift = 0;
                		*/
                		data_url.shift = 0;
                		data_formbody.shift = 0;
                		data_attachment_name.shift = 0;
                		data_attachment_content.shift = 0;
                	}
                	else 
                	{
                	}
                }
	}
	cJSON_Delete(pRoot);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	int num = 0;
 	char username[17] = "";
  	char dispname[65] = "";
  	char ca_sql[256] = "";
  	for(num = 0; num < iNum; num++){
    		sprintf(ca_sql, "select username, dispname from ncsuser where userid = %d", v_did[num]);
    		
    		memset(username, 0, sizeof(username));
    		memset(dispname, 0, sizeof(dispname));
    		
    		pasDbOneRecord(ca_sql, 0, 
    				UT_TYPE_STRING, 16, username, 
    				UT_TYPE_STRING, 64, dispname);
    				
    		utPltPutLoopVar(psDbHead, "servicecode", num+1, username);
    		utPltPutLoopVar(psDbHead, "servicename", num+1, dispname);
    		              
   	}	
 	
 	char caMac[19] = "";
  	char caPosition[33] = "";
  	for(num = 0; num < iNum; num++){
       		sprintf(ca_sql, "select mac, position from ncsenduser where did = %d and uid = %d", v_did[num], v_uid[num]);
       		
       		memset(caMac,      0, sizeof(caMac));
       		memset(caPosition, 0, sizeof(caPosition));
       		
       		pasDbOneRecord(ca_sql, 0, 
       				UT_TYPE_STRING, 18, caMac, 
       				UT_TYPE_STRING, 32, caPosition);
       		utPltPutLoopVar(psDbHead, "mac", num+1, caMac);
    		utPltPutLoopVar(psDbHead, "position", num+1, caPosition);
  	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	free(v_did);
	free(v_uid);
	if(client != NULL)
		sphinx_destroy ( client );
	free(tmputf8);
}

int ncsformmon_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	char caRe[4] = "";
	char caRn[16] = "";
	char caLm[16] = "";
	char caQuery[256] = "";
	char caTotal[16] = "";
	char caSort_flag[3] = "";
	char caQ1[256] = "";
	char caQ2[256] = "";
	char caQ3[256] = "";
	char caQ4[256] = "";
	char caQ5[16] = "";
	char caQ6[16] = "";
	
	char ca_query_text[1024] = ""; 
	char wd[1024]= "";
	char query[1024] = "";
	char qeystr[1024] = "";
	
	char ca_cur_pg[9] = "";
	int iSortFlag = 0;
	int iSum = 0;
	int took = 0;
	struct curl_data wdata;
	CURL *curl;
  	CURLcode res;
	char *jsonstr = NULL;
	char *field = "all";
	unsigned long lCp = 0;
	unsigned long iStart = 0;
	unsigned long lCount = 0;
	unsigned long start = 0;
	unsigned long end = time(NULL);
	utMsgPrintMsg(psMsgHead);
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgGetSomeNVar(psMsgHead, 14, 
				       "f", 	UT_TYPE_STRING, 3, 	caRe,
				       "rn",    UT_TYPE_STRING, 15, 	caRn,
	                               "lm",    UT_TYPE_STRING, 15, 	caLm, 
	                               "q",     UT_TYPE_STRING, 255, 	caQuery,           	
	                               "t",     UT_TYPE_STRING, 15,  	caTotal,          	
	                               "sf", 	UT_TYPE_STRING, 2, 	caSort_flag, 
	                               "q1",    UT_TYPE_STRING, 255,	caQ1,
	                               "q2",    UT_TYPE_STRING, 255,	caQ2,
	                               "q3",    UT_TYPE_STRING, 255,	caQ3,
	                               "q4",    UT_TYPE_STRING, 255,	caQ4,
	                               "q5",    UT_TYPE_STRING, 15, 	caQ5,//查询字段
	                               "q6",    UT_TYPE_STRING, 15, 	caQ6,
	                               "wd",    UT_TYPE_STRING, 255, 	wd, 
	                               "cp",    UT_TYPE_STRING, 8,  	ca_cur_pg); 
	utPltPutVar(psDbHead, "q1", caQ1);
	utPltPutVar(psDbHead, "q2", caQ2);
	utPltPutVar(psDbHead, "q3", caQ3);
	utPltPutVar(psDbHead, "q4", caQ4);
	utPltPutVar(psDbHead, "rn", caRn);
	utPltPutVar(psDbHead, "lm", caLm);
	if(strlen(caRn) > 0)/* 指定了搜索结果显示的条数 */ 
	{
	  	iSum = atoi(caRn);
                if(iSum == 10 || iSum == 20 || iSum == 50 || iSum == 100)
                {
                }
                else
                {
                        iSum = 10;
                }
	}
	else /* 默认10条 */
	{
		iSum = 10;
	}
	lCp = atol(ca_cur_pg);
	if (lCp == 0) 
		lCp = 1;
        iStart = (lCp - 1) * iSum;
	lCount = 0;
	utPltPutVarF(psDbHead, "cp", "%lu", lCp);
	utPltPutVarF(psDbHead, "start", "%lu", iStart+1);
	
	
	/////////////////////////////////////////////////////////////////////////////////
	if(strlen(caQ1) > 0 && !utStrIsSpaces(caQ1))
	{
	  	snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "%s ", caQ1);
	}
	if(strlen(caQ3) > 0 && !utStrIsSpaces(caQ3))
	{
		char *word = strtok(caQ3, SEPCHARS);
		if(word != NULL)
			if(strlen(ca_query_text) > 0)
				snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "AND ");
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "(%s", word);	
		word = strtok(NULL, SEPCHARS);
		while(word != NULL)
		{
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " OR %s", word);
			word = strtok(NULL, SEPCHARS);
		}
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text),  ")");
	}  
	if(strlen(caQ4) > 0 && !utStrIsSpaces(caQ4))
	{
		if(strlen(ca_query_text) > 0)
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " AND (");
		char *word = strtok(caQ4, SEPCHARS);
		while(word != NULL)
		{
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "NOT %s", word);
			word = strtok(NULL, SEPCHARS);
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "%s", word ? " " : "");
		}
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text),  ")");
	} 
	if(strlen(caQ2) > 0 && !utStrIsSpaces(caQ2))
	{
	  	if(strlen(ca_query_text) > 0)
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " ");
	  	snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "\"%s\"", caQ2);
	}
	if(strlen(wd) == 0 || utStrIsSpaces(wd))
	{
		memset(wd, 0, sizeof(wd));
		strncpy(wd, ca_query_text, sizeof(wd)-1);
	}
	
	strncpy(query, wd, sizeof(query)-1);
	str_replace(query, '\"',  "&quot;");
	utPltPutVar(psDbHead, "query", query);
	/////////////////////////////////////////////////////////////////////////////////
	
	
	if(strcmp(caRe, "re") == 0)
	{
	  	utPltPutVar(psDbHead, "query_text", caQuery);
      		utPltPutVar(psDbHead, "total",      caTotal);
      		return utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/re_form_search.html");
  	} 
  	if(strlen(caSort_flag) == 0 || strcmp(caSort_flag, "0") == 0)/* 按时间排序 */
	{
		iSortFlag = 1;
	}
	
	unsigned long lLm = atol(caLm);
	if(lLm == 1 || lLm == 7 || lLm == 30 || lLm == 365)
		start = end - lLm * 86400;
	else start = 0;
		
	switch(atol(caQ5))
	{
		case 1:
			field = "url";
			break;
		case 2:
			field = "formbody";
			break;
		case 3:
			field = "attachment_name";
			break;
		case 4:
			field = "attachment_content";
			break;	
		case 5:
			field = "dispname";
			break;	
		case 6:
			field = "sip";
			break;	
		case 7:
			field = "dip";
			break;	
		default:
			break;
	}
	
	utMsgGetSomeNVar(psMsgHead, 4, 
	                               "q1",    UT_TYPE_STRING, 255,	caQ1,
	                               "q2",    UT_TYPE_STRING, 255,	caQ2,
	                               "q3",    UT_TYPE_STRING, 255,	caQ3,
	                               "q4",    UT_TYPE_STRING, 255,	caQ4); 
	if(!utStrIsSpaces(caQ1) || !utStrIsSpaces(caQ2) || !utStrIsSpaces(caQ3)) //指定了高级查询
	{
		jsonstr = createMatchFORMTermJsonString(field, iStart, iSum, iSortFlag, "", start, end, caQ1, caQ2, caQ3, caQ4, "");
	}
	else if(!utStrIsSpaces(wd))
	{
		get_stbuf(wd, "\"", '\"', caQ2, sizeof(caQ2)-1);
		if(strcasestr(wd, "AND") || strcasestr(wd, "OR") || strcasestr(wd, "NOT") )
		{
			strcpy(qeystr, wd);
			str_delstr(qeystr, "\"");
			if(!utStrIsSpaces(caQ2))
			{
				str_delstr(qeystr, caQ2);
			}
			if(qeystr[strlen(qeystr)-1] == ' ')
			{
				qeystr[strlen(qeystr)-1] = '\0';
			}
			printf("qeystr = %s, caQ2 = %s\n", qeystr, caQ2);
			jsonstr = createMatchFORMTermJsonString(field, iStart, iSum, iSortFlag, "", start, end, "", caQ2, "", "", qeystr);
		}
		else 
		{
			if(strlen(caQ2) > 0 ) 
				wd[0] = '\0';
			jsonstr = createMatchFORMTermJsonString(field, iStart, iSum, iSortFlag, wd, start, end, "", caQ2, "", "", "");
		}
	}
	else 
	{
		jsonstr = createMatchAllJsonString(iStart, iSum, iSortFlag);
	}
	
	printf("jsonstr=%s\n",jsonstr);
  	curl_global_init(CURL_GLOBAL_ALL);
  	curl = curl_easy_init();
  	if(!curl)
  		return 0;
  	curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:9200/dc/form/_search?pretty=true");
  	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE,  1L);
  	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  	memset(&wdata, 0, sizeof(struct curl_data));
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,  &wdata);
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr);//for XGET
	  res = curl_easy_perform(curl);
    	if(res == CURLE_OK)
    	{
 printf("buf=%s\n",wdata.buf);
    		wdata.buf[wdata.shift] = '\0';
    		char word[1536] = "";
    		if(!utStrIsSpaces(wd))
    			snprintf(word, sizeof(word), "%s", wd);
    		if(!utStrIsSpaces(caQ1))
    			snprintf(word, sizeof(word), " %s", caQ1);
    		if(!utStrIsSpaces(caQ2))
    			snprintf(word, sizeof(word), " %s", caQ2);
    		if(!utStrIsSpaces(caQ3))
    			snprintf(word, sizeof(word), " %s", caQ3);
    		if(!utStrIsSpaces(caQ4))
    			snprintf(word, sizeof(word), " %s", caQ4);
    		parse_form_jsonstring(psDbHead, iSum, wdata.buf, &took, (int *)&lCount, word);
    		free(wdata.buf);
    	}
    	else
    	{
    		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    	}
    	curl_global_cleanup();
    	free(jsonstr);
    	if(lCount == 0 || iStart > lCount){
		utPltPutVar(psDbHead, "display2", "none");
		utPltPutVar(psDbHead, "display1", " ");
		return utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/search_form_result.html");
   	}
   	else 
	{    	
	        utPltPutVar(psDbHead, "display1", "none");
	}        
	unsigned long ltotpg = (lCount % iSum) ? (lCount / iSum + 1) : lCount / iSum;	//总页数 
	utPltPutVarF(psDbHead, "total", "%lu", lCount);
	utPltPutVarF(psDbHead, "total_times", "%d.%03d", took/1000, took%1000);
	utPltPutVarF(psDbHead, "end", "%lu", lCp == ltotpg ? lCount : iStart+iSum);
		
	strncpy(wd, query, sizeof(wd)-1);
	str_replace(caQ1, '\"',  "&quot;");
	str_replace(caQ2, '\"',  "&quot;");
	str_replace(caQ3, '\"',  "&quot;");
	str_replace(caQ4, '\"',  "&quot;");
    	if(lCp != 1 ){
		utPltPutVar(psDbHead, "prev", "上一页");
		utPltPutVarF(psDbHead, "CurPg_prev", "%d", lCp-1);
		utPltPutVar(psDbHead, "wd_prev",wd);
		utPltPutVar(psDbHead, "rn_prev", caRn);
		utPltPutVar(psDbHead, "lm_prev", caLm);
		utPltPutVar(psDbHead, "sf_prev", caSort_flag);
		
		utPltPutVar(psDbHead, "q1_prev", caQ1);
		utPltPutVar(psDbHead, "q2_prev", caQ2);
		utPltPutVar(psDbHead, "q3_prev", caQ3);
		utPltPutVar(psDbHead, "q4_prev", caQ4);
		utPltPutVar(psDbHead, "q5_prev", caQ5);
		utPltPutVar(psDbHead, "q6_prev", caQ6);
	}
	if(lCp != ltotpg && ltotpg > 0){
	
		utPltPutVar(psDbHead, "next", "下一页");
		utPltPutVarF(psDbHead, "CurPg_next", "%d", lCp+1);
		utPltPutVar(psDbHead, "wd_next",wd);
		utPltPutVar(psDbHead, "rn_next", caRn);
		utPltPutVar(psDbHead, "lm_next", caLm);
		utPltPutVar(psDbHead, "sf_next", caSort_flag);
		
		utPltPutVar(psDbHead, "q1_next", caQ1);
		utPltPutVar(psDbHead, "q2_next", caQ2);
		utPltPutVar(psDbHead, "q3_next", caQ3);
		utPltPutVar(psDbHead, "q4_next", caQ4);
		utPltPutVar(psDbHead, "q5_next", caQ5);
		utPltPutVar(psDbHead, "q6_next", caQ6);
   	}
	int i = 0;
   	if(lCp > 10){
	 	for(i = lCp - 10; i < lCp ; i++){ 
			utPltPutLoopVarF(psDbHead, "page1", i-(lCp - 11), "[%d]", i);
			utPltPutLoopVarF(psDbHead, "CurPg_page1", i-(lCp - 11), "%d", i);
			utPltPutLoopVar(psDbHead, "wd_page1",i-(lCp - 11),wd);
			utPltPutLoopVar(psDbHead, "rn_page1", i-(lCp - 11), caRn);
			utPltPutLoopVar(psDbHead, "lm_page1", i-(lCp - 11), caLm);
			utPltPutLoopVar(psDbHead, "sf_page1", i-(lCp - 11), caSort_flag);
			
			utPltPutLoopVar(psDbHead, "q1_page1", i-(lCp - 11), caQ1);
			utPltPutLoopVar(psDbHead, "q2_page1", i-(lCp - 11), caQ2);
			utPltPutLoopVar(psDbHead, "q3_page1", i-(lCp - 11), caQ3);
			utPltPutLoopVar(psDbHead, "q4_page1", i-(lCp - 11), caQ4);
			utPltPutLoopVar(psDbHead, "q5_page1", i-(lCp - 11), caQ5);
			utPltPutLoopVar(psDbHead, "q6_page1", i-(lCp - 11), caQ6);
		}
 	}
	
 	if(lCp <= 10){
	 	for(i = 1; i < lCp; i++){
			utPltPutLoopVarF(psDbHead, "page1", i, "[%d]", i);
			utPltPutLoopVarF(psDbHead, "CurPg_page1", i, "%d", i);
			utPltPutLoopVar(psDbHead, "wd_page1",i,wd);
			utPltPutLoopVar(psDbHead, "rn_page1", i, caRn);
			utPltPutLoopVar(psDbHead, "lm_page1", i, caLm);
			utPltPutLoopVar(psDbHead, "sf_page1", i, caSort_flag);
			
			utPltPutLoopVar(psDbHead, "q1_page1", i, caQ1);
			utPltPutLoopVar(psDbHead, "q2_page1", i, caQ2);
			utPltPutLoopVar(psDbHead, "q3_page1", i, caQ3);
			utPltPutLoopVar(psDbHead, "q4_page1", i, caQ4);
			utPltPutLoopVar(psDbHead, "q5_page1", i, caQ5);
			utPltPutLoopVar(psDbHead, "q6_page1", i, caQ6);
	 	}
 	}
 	if(ltotpg > 0)
	 	utPltPutVarF(psDbHead, "page",  "%d", lCp);
 
 	for(i = lCp+1; i < lCp+10 && i < ltotpg; i++){
	 	utPltPutLoopVarF(psDbHead, "page2", i-lCp, "[%d]", i);
	 	utPltPutLoopVarF(psDbHead, "CurPg_page2", i-lCp, "%d", i);
	 	utPltPutLoopVar(psDbHead, "wd_page2",i-lCp,wd);
	 	utPltPutLoopVar(psDbHead, "rn_page2", i-lCp, caRn);
     		utPltPutLoopVar(psDbHead, "lm_page2", i-lCp, caLm);
     		utPltPutLoopVar(psDbHead, "sf_page2",  i-lCp, caSort_flag);
     		
     		utPltPutLoopVar(psDbHead, "q1_page2",  i-lCp, caQ1);
     		utPltPutLoopVar(psDbHead, "q2_page2",  i-lCp, caQ2);
     		utPltPutLoopVar(psDbHead, "q3_page2",  i-lCp, caQ3);
     		utPltPutLoopVar(psDbHead, "q4_page2",  i-lCp, caQ4);
     		utPltPutLoopVar(psDbHead, "q5_page2",  i-lCp, caQ5);
     		utPltPutLoopVar(psDbHead, "q6_page2",  i-lCp, caQ6);
 	}
 	if(strcmp(caSort_flag, "1") == 0){	
 		utPltPutVarF(psDbHead, "sort1", "%s", "<b>已按相关性排序</b>");
 		utPltPutVarF(psDbHead, "sort2", "<a href=\"/pronline/Msg?FunName@ncsformmon_v4&wd@%s&q1@%s&q2@%s&q3@%s&q4@%s&cp@%d&rn@%s&lm@%s&sf@0\">按日期排序</a>",  wd, caQ1, caQ2, caQ3, caQ4, lCp, caRn, caLm);
 	}
 	else{
 		utPltPutVarF(psDbHead, "sort1", "<a href=\"/pronline/Msg?FunName@ncsformmon_v4&wd@%s&q1@%s&q2@%s&q3@%s&q4@%s&cp@%d&rn@%s&lm@%s&sf@1\">按相关性排序</a>",  wd, caQ1, caQ2, caQ3, caQ4, lCp, caRn, caLm);
 		utPltPutVarF(psDbHead, "sort2", "%s", "<b>已按日期排序</b>");
 	}
    	utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/search_form_result.html");
	return 0;
}

int ncsfulltext_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	char caRe[4] = "";
	char caRn[16] = "";
	char caLm[16] = "";
	char caQuery[256] = "";
	char caTotal[16] = "";
	char caSort_flag[3] = "";
	char caQ1[256] = "";
	char caQ2[256] = "";
	char caQ3[256] = "";
	char caQ4[256] = "";
	char caQ5[16] = "";
	char caQ6[16] = "";
	
	char ca_query_text[1024] = ""; 
	char wd[1024]= "";
	char query[1024] = "";
	char qeystr[1024] = "";
	
	char ca_cur_pg[9] = "";
	int iSortFlag = 0;
	int iSum = 0;
	int took = 0;
	struct curl_data wdata;
	CURL *curl;
  	CURLcode res;
	char *jsonstr = NULL;
	char *field = "all";
	unsigned long lCp = 0;
	unsigned long iStart = 0;
	unsigned long lCount = 0;
	unsigned long start = 0;
	unsigned long end = time(NULL);
	utMsgPrintMsg(psMsgHead);
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgGetSomeNVar(psMsgHead, 14, 
				       "f", 	UT_TYPE_STRING, 3, 	caRe,
				       "rn",    UT_TYPE_STRING, 15, 	caRn,
	                               "lm",    UT_TYPE_STRING, 15, 	caLm, 
	                               "q",     UT_TYPE_STRING, 255, 	caQuery,           	
	                               "t",     UT_TYPE_STRING, 15,  	caTotal,          	
	                               "sf", 	UT_TYPE_STRING, 2, 	caSort_flag, 
	                               "q1",    UT_TYPE_STRING, 255,	caQ1,
	                               "q2",    UT_TYPE_STRING, 255,	caQ2,
	                               "q3",    UT_TYPE_STRING, 255,	caQ3,
	                               "q4",    UT_TYPE_STRING, 255,	caQ4,
	                               "q5",    UT_TYPE_STRING, 15, 	caQ5,//查询字段
	                               "q6",    UT_TYPE_STRING, 15, 	caQ6,
	                               "wd",    UT_TYPE_STRING, 255, 	wd, 
	                               "cp",    UT_TYPE_STRING, 8,  	ca_cur_pg); 
	utPltPutVar(psDbHead, "q1", caQ1);
	utPltPutVar(psDbHead, "q2", caQ2);
	utPltPutVar(psDbHead, "q3", caQ3);
	utPltPutVar(psDbHead, "q4", caQ4);
	utPltPutVar(psDbHead, "rn", caRn);
	utPltPutVar(psDbHead, "lm", caLm);
	if(strlen(caRn) > 0)/* 指定了搜索结果显示的条数 */ 
	{
	  	iSum = atoi(caRn);
                if(iSum == 10 || iSum == 20 || iSum == 50 || iSum == 100)
                {
                }
                else
                {
                        iSum = 10;
                }
	}
	else /* 默认10条 */
	{
		iSum = 10;
	}
	lCp = atol(ca_cur_pg);
	if (lCp == 0) 
		lCp = 1;
        iStart = (lCp - 1) * iSum;
	lCount = 0;
	utPltPutVarF(psDbHead, "cp", "%lu", lCp);
	utPltPutVarF(psDbHead, "start", "%lu", iStart+1);
	
	
	/////////////////////////////////////////////////////////////////////////////////
	if(strlen(caQ1) > 0 && !utStrIsSpaces(caQ1))
	{
	  	snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "%s ", caQ1);
	}
	if(strlen(caQ3) > 0 && !utStrIsSpaces(caQ3))
	{
		char *word = strtok(caQ3, SEPCHARS);
		if(word != NULL)
			if(strlen(ca_query_text) > 0)
				snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "AND ");
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "(%s", word);	
		word = strtok(NULL, SEPCHARS);
		while(word != NULL)
		{
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " OR %s", word);
			word = strtok(NULL, SEPCHARS);
		}
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text),  ")");
	}  
	if(strlen(caQ4) > 0 && !utStrIsSpaces(caQ4))
	{
		if(strlen(ca_query_text) > 0)
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " AND (");
		char *word = strtok(caQ4, SEPCHARS);
		while(word != NULL)
		{
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "NOT %s", word);
			word = strtok(NULL, SEPCHARS);
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "%s", word ? " " : "");
		}
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text),  ")");
	} 
	if(strlen(caQ2) > 0 && !utStrIsSpaces(caQ2))
	{
	  	if(strlen(ca_query_text) > 0)
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " ");
	  	snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "\"%s\"", caQ2);
	}
	if(strlen(wd) == 0 || utStrIsSpaces(wd))
	{
		memset(wd, 0, sizeof(wd));
		strncpy(wd, ca_query_text, sizeof(wd)-1);
	}
	
	strncpy(query, wd, sizeof(query)-1);
	str_replace(query, '\"',  "&quot;");
	utPltPutVar(psDbHead, "query", query);
	/////////////////////////////////////////////////////////////////////////////////
	
	
	if(strcmp(caRe, "re") == 0)
	{
	  	utPltPutVar(psDbHead, "query_text", caQuery);
      		utPltPutVar(psDbHead, "total",      caTotal);
      		return utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/re_fulltext_search.html");
  	} 
  	if(strlen(caSort_flag) == 0 || strcmp(caSort_flag, "0") == 0)/* 按时间排序 */
	{
		iSortFlag = 1;
	}
	
	unsigned long lLm = atol(caLm);
	if(lLm == 1 || lLm == 7 || lLm == 30 || lLm == 365)
		start = end - lLm * 86400;
	else start = 0;
		
	switch(atol(caQ5))
	{
		case 1:
			field = "url";
			break;
		case 2:
			field = "formbody";
			break;
		case 3:
			field = "attachment_name";
			break;
		case 4:
			field = "attachment_content";
			break;	
		case 5:
			field = "dispname";
			break;	
		case 6:
			field = "sip";
			break;	
		case 7:
			field = "dip";
			break;	
		default:
			break;
	}
	int service = atol(caQ6);
	utMsgGetSomeNVar(psMsgHead, 4, 
	                               "q1",    UT_TYPE_STRING, 255,	caQ1,
	                               "q2",    UT_TYPE_STRING, 255,	caQ2,
	                               "q3",    UT_TYPE_STRING, 255,	caQ3,
	                               "q4",    UT_TYPE_STRING, 255,	caQ4); 
	if(!utStrIsSpaces(caQ1) || !utStrIsSpaces(caQ2) || !utStrIsSpaces(caQ3)) //指定了高级查询
	{
		jsonstr = createMatchAllTermJsonString(field, iStart, iSum, iSortFlag, "", start, end, caQ1, caQ2, caQ3, caQ4, "", service);
	}
	else if(!utStrIsSpaces(wd))
	{
		get_stbuf(wd, "\"", '\"', caQ2, sizeof(caQ2)-1);
		if(strcasestr(wd, "AND") || strcasestr(wd, "OR") || strcasestr(wd, "NOT") )
		{
			strcpy(qeystr, wd);
			str_delstr(qeystr, "\"");
			if(!utStrIsSpaces(caQ2))
			{
				str_delstr(qeystr, caQ2);
			}
			if(qeystr[strlen(qeystr)-1] == ' ')
			{
				qeystr[strlen(qeystr)-1] = '\0';
			}
			printf("qeystr = %s, caQ2 = %s\n", qeystr, caQ2);
			jsonstr = createMatchAllTermJsonString(field, iStart, iSum, iSortFlag, "", start, end, "", caQ2, "", "", qeystr, service);
		}
		else 
		{
			if(strlen(caQ2) > 0 ) 
				wd[0] = '\0';
			jsonstr = createMatchAllTermJsonString(field, iStart, iSum, iSortFlag, wd, start, end, "", caQ2, "", "", "", service);
		}
	}
	else 
	{
		jsonstr = createMatchAllJsonString(iStart, iSum, iSortFlag);
	}
  	curl_global_init(CURL_GLOBAL_ALL);
  	curl = curl_easy_init();
  	if(!curl)
  		return 0;
  	curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:9200/dc/_search?pretty=true");
  	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE,  1L);
  	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  	memset(&wdata, 0, sizeof(struct curl_data));
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA,  &wdata);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr);//for XGET
	res = curl_easy_perform(curl);
    	if(res == CURLE_OK)
    	{
    		wdata.buf[wdata.shift] = '\0';
    		char word[1536] = "";
    		if(!utStrIsSpaces(wd))
    			snprintf(word, sizeof(word), "%s", wd);
    		if(!utStrIsSpaces(caQ1))
    			snprintf(word, sizeof(word), " %s", caQ1);
    		if(!utStrIsSpaces(caQ2))
    			snprintf(word, sizeof(word), " %s", caQ2);
    		if(!utStrIsSpaces(caQ3))
    			snprintf(word, sizeof(word), " %s", caQ3);
    		if(!utStrIsSpaces(caQ4))
    			snprintf(word, sizeof(word), " %s", caQ4);
    		parse_fulltext_jsonstring(psDbHead, iSum, wdata.buf, &took, (int *)&lCount, word);
    		free(wdata.buf);
    	}
    	else
    	{
    		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    	}
    	curl_global_cleanup();
    	free(jsonstr);
    	if(lCount == 0 || iStart > lCount){
		utPltPutVar(psDbHead, "display2", "none");
		utPltPutVar(psDbHead, "display1", " ");
		return utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/search_fulltext_result.html");
   	}
   	else 
	{    	
	        utPltPutVar(psDbHead, "display1", "none");
	}        
	unsigned long ltotpg = (lCount % iSum) ? (lCount / iSum + 1) : lCount / iSum;	//总页数 
	utPltPutVarF(psDbHead, "total", "%lu", lCount);
	utPltPutVarF(psDbHead, "total_times", "%d.%03d", took/1000, took%1000);
	utPltPutVarF(psDbHead, "end", "%lu", lCp == ltotpg ? lCount : iStart+iSum);
		
	strncpy(wd, query, sizeof(wd)-1);
	str_replace(caQ1, '\"',  "&quot;");
	str_replace(caQ2, '\"',  "&quot;");
	str_replace(caQ3, '\"',  "&quot;");
	str_replace(caQ4, '\"',  "&quot;");
    	if(lCp != 1 ){
		utPltPutVar(psDbHead, "prev", "上一页");
		utPltPutVarF(psDbHead, "CurPg_prev", "%d", lCp-1);
		utPltPutVar(psDbHead, "wd_prev",wd);
		utPltPutVar(psDbHead, "rn_prev", caRn);
		utPltPutVar(psDbHead, "lm_prev", caLm);
		utPltPutVar(psDbHead, "sf_prev", caSort_flag);
		
		utPltPutVar(psDbHead, "q1_prev", caQ1);
		utPltPutVar(psDbHead, "q2_prev", caQ2);
		utPltPutVar(psDbHead, "q3_prev", caQ3);
		utPltPutVar(psDbHead, "q4_prev", caQ4);
		utPltPutVar(psDbHead, "q5_prev", caQ5);
		utPltPutVar(psDbHead, "q6_prev", caQ6);
	}
	if(lCp != ltotpg && ltotpg > 0){
	
		utPltPutVar(psDbHead, "next", "下一页");
		utPltPutVarF(psDbHead, "CurPg_next", "%d", lCp+1);
		utPltPutVar(psDbHead, "wd_next",wd);
		utPltPutVar(psDbHead, "rn_next", caRn);
		utPltPutVar(psDbHead, "lm_next", caLm);
		utPltPutVar(psDbHead, "sf_next", caSort_flag);
		
		utPltPutVar(psDbHead, "q1_next", caQ1);
		utPltPutVar(psDbHead, "q2_next", caQ2);
		utPltPutVar(psDbHead, "q3_next", caQ3);
		utPltPutVar(psDbHead, "q4_next", caQ4);
		utPltPutVar(psDbHead, "q5_next", caQ5);
		utPltPutVar(psDbHead, "q6_next", caQ6);
   	}
	int i = 0;
   	if(lCp > 10){
	 	for(i = lCp - 10; i < lCp ; i++){ 
			utPltPutLoopVarF(psDbHead, "page1", i-(lCp - 11), "[%d]", i);
			utPltPutLoopVarF(psDbHead, "CurPg_page1", i-(lCp - 11), "%d", i);
			utPltPutLoopVar(psDbHead, "wd_page1",i-(lCp - 11),wd);
			utPltPutLoopVar(psDbHead, "rn_page1", i-(lCp - 11), caRn);
			utPltPutLoopVar(psDbHead, "lm_page1", i-(lCp - 11), caLm);
			utPltPutLoopVar(psDbHead, "sf_page1", i-(lCp - 11), caSort_flag);
			
			utPltPutLoopVar(psDbHead, "q1_page1", i-(lCp - 11), caQ1);
			utPltPutLoopVar(psDbHead, "q2_page1", i-(lCp - 11), caQ2);
			utPltPutLoopVar(psDbHead, "q3_page1", i-(lCp - 11), caQ3);
			utPltPutLoopVar(psDbHead, "q4_page1", i-(lCp - 11), caQ4);
			utPltPutLoopVar(psDbHead, "q5_page1", i-(lCp - 11), caQ5);
			utPltPutLoopVar(psDbHead, "q6_page1", i-(lCp - 11), caQ6);
		}
 	}
	
 	if(lCp <= 10){
	 	for(i = 1; i < lCp; i++){
			utPltPutLoopVarF(psDbHead, "page1", i, "[%d]", i);
			utPltPutLoopVarF(psDbHead, "CurPg_page1", i, "%d", i);
			utPltPutLoopVar(psDbHead, "wd_page1",i,wd);
			utPltPutLoopVar(psDbHead, "rn_page1", i, caRn);
			utPltPutLoopVar(psDbHead, "lm_page1", i, caLm);
			utPltPutLoopVar(psDbHead, "sf_page1", i, caSort_flag);
			
			utPltPutLoopVar(psDbHead, "q1_page1", i, caQ1);
			utPltPutLoopVar(psDbHead, "q2_page1", i, caQ2);
			utPltPutLoopVar(psDbHead, "q3_page1", i, caQ3);
			utPltPutLoopVar(psDbHead, "q4_page1", i, caQ4);
			utPltPutLoopVar(psDbHead, "q5_page1", i, caQ5);
			utPltPutLoopVar(psDbHead, "q6_page1", i, caQ6);
	 	}
 	}
 	if(ltotpg > 0)
	 	utPltPutVarF(psDbHead, "page",  "%d", lCp);
 
 	for(i = lCp+1; i < lCp+10 && i < ltotpg; i++){
	 	utPltPutLoopVarF(psDbHead, "page2", i-lCp, "[%d]", i);
	 	utPltPutLoopVarF(psDbHead, "CurPg_page2", i-lCp, "%d", i);
	 	utPltPutLoopVar(psDbHead, "wd_page2",i-lCp,wd);
	 	utPltPutLoopVar(psDbHead, "rn_page2", i-lCp, caRn);
     		utPltPutLoopVar(psDbHead, "lm_page2", i-lCp, caLm);
     		utPltPutLoopVar(psDbHead, "sf_page2",  i-lCp, caSort_flag);
     		
     		utPltPutLoopVar(psDbHead, "q1_page2",  i-lCp, caQ1);
     		utPltPutLoopVar(psDbHead, "q2_page2",  i-lCp, caQ2);
     		utPltPutLoopVar(psDbHead, "q3_page2",  i-lCp, caQ3);
     		utPltPutLoopVar(psDbHead, "q4_page2",  i-lCp, caQ4);
     		utPltPutLoopVar(psDbHead, "q5_page2",  i-lCp, caQ5);
     		utPltPutLoopVar(psDbHead, "q6_page2",  i-lCp, caQ6);
 	}
 	if(strcmp(caSort_flag, "1") == 0){	
 		utPltPutVarF(psDbHead, "sort1", "%s", "<b>已按相关性排序</b>");
 		utPltPutVarF(psDbHead, "sort2", "<a href=\"/pronline/Msg?FunName@ncsfulltext_v4&wd@%s&q1@%s&q2@%s&q3@%s&q4@%s&cp@%d&rn@%s&lm@%s&sf@0\">按日期排序</a>",  wd, caQ1, caQ2, caQ3, caQ4, lCp, caRn, caLm);
 	}
 	else{
 		utPltPutVarF(psDbHead, "sort1", "<a href=\"/pronline/Msg?FunName@ncsfulltext_v4&wd@%s&q1@%s&q2@%s&q3@%s&q4@%s&cp@%d&rn@%s&lm@%s&sf@1\">按相关性排序</a>",  wd, caQ1, caQ2, caQ3, caQ4, lCp, caRn, caLm);
 		utPltPutVarF(psDbHead, "sort2", "%s", "<b>已按日期排序</b>");
 	}
    	utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/search_fulltext_result.html");
	return 0;
}

int ncgetattfile_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	char fname[512] = "";
	char file[512] = "";
	char path[512] = "";
	utMsgPrintMsg(psMsgHead);
	utMsgGetSomeNVar(psMsgHead, 1, "fname", UT_TYPE_STRING, sizeof(fname)-1, fname);
	
	char *basename = NULL;
	char *basename_utf8 = NULL;
	
	char *p = NULL;
	char *p1 = NULL;
	if( (p = strstr(fname, "/")) && (p1 = strstr(p+1, "/")) &&(p = strstr(p1+1, "/")) )
	{
		basename = strdup(p+1);
		basename_utf8 = iconv_convert("GBK", "UTF-8", basename);
		
		*p = '\0';
		
		strcat(path, attdir);
		strcat(path, "/");
		strcat(path,fname);
		
		strcat(file, path);
		strcat(file, "/");
		strcat(file,basename_utf8);
		
		printf("file = %s\n", file);
		
		if(access(file, R_OK)) 
		{
                        printf("access %s failed\n", file);
                        utWebDispMsg(iFd,psMsgHead,"ncs/nc_msg_back.html","File No Exits","File No Exits");
                        return 0;
                }
                utPltFileDownload(iFd,"application/text",path,basename_utf8,basename);
		free(basename_utf8);
		free(basename);
	}
	return 0;
}



void parse_bbs_jsonstring_v4(utPltDbHead *psDbHead, int iSum, char *jsonstring, int *took, int *count, char *query)
{
	unsigned long lIp,iReturn;
	char caArea[256],caName2[256],caTemp[1024];
  cJSON *pItemL = NULL;

	int *v_did = (int *)calloc(iSum, sizeof(int));
	if(v_did == NULL)
	{
		
		return;
	}
		
	int *v_uid = (int *)calloc(iSum, sizeof(int));
	if(v_uid == NULL)
	{	free(v_did);
	
		return;
	}
//printf("pid=%d\n",getpid());
//sleep(15);
	cJSON* pRoot = cJSON_Parse ( jsonstring );
	cJSON *pArrayItem = NULL;
	cJSON *pItem = NULL;
	cJSON *pItem1 = NULL;
	cJSON *pItem2 = NULL;
	int i;
	int cnt;
	int iNum = 0;
	int j = 0;
	int cnt2 = 0;
	
	
	//char *out=cJSON_Print(pRoot);
	//printf("recv = %s\n",out);
	//free(out);
	
	*took = cJSON_GetObjectItem( pRoot, "took") ? cJSON_GetObjectItem( pRoot, "took")->valueint : 0;
	if(*took == 0)
	{
		*count = 0;
		cJSON_Delete(pRoot);
		free(v_did);
		free(v_uid);
		return;
	}
	
	if((pItem = cJSON_GetObjectItem ( pRoot, "hits")) && (*count=cJSON_GetObjectItem( pItem, "total")->valueint,pItem = cJSON_GetObjectItem ( pItem, "hits")))
	{
		cnt = cJSON_GetArraySize( pItem);
		
		struct curl_data data_dispname;
		struct curl_data data_sip;
		struct curl_data data_dip;
		struct curl_data data_bbsname;
		struct curl_data data_url;
		struct curl_data data_subject;
		struct curl_data data_content;
		char *highlightstr = NULL;
		iNum=0;
		for( i= 0; i < cnt; i++)
                {
                	memset(&data_dispname, 0, sizeof(struct curl_data));
                	memset(&data_sip, 0, sizeof(struct curl_data));
                	memset(&data_dip, 0, sizeof(struct curl_data));
                	memset(&data_bbsname, 0, sizeof(struct curl_data));
                	memset(&data_url, 0, sizeof(struct curl_data));
                	memset(&data_subject, 0, sizeof(struct curl_data));
                	memset(&data_content, 0, sizeof(struct curl_data));
                	pArrayItem = cJSON_GetArrayItem(pItem, i);
                	pItemL = cJSON_GetObjectItem ( pArrayItem, "highlight");
 /*
                	if((pItem1 = cJSON_GetObjectItem ( pArrayItem, "highlight")))
                	{
                		pItem2 = cJSON_GetObjectItem ( pItem1, "dispname");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_dispname);
                			}	
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "sip");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_sip);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "dip");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_dip);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "bbsname");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_bbsname);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "url");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_url);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "subject");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_subject);
                			}
                		}
                		pItem2 = cJSON_GetObjectItem ( pItem1, "content");
                		if(pItem2)
                		{
                			cnt2 = cJSON_GetArraySize(pItem2);
                			for( j = 0; j < cnt2; j++)
                			{
                				highlightstr = cJSON_GetArrayItem(pItem2, j)->valuestring;
                				write_data(highlightstr, sizeof(char), strlen(highlightstr), &data_content);
                			}
                		}
                	}
*/
                	if((pItem1 = cJSON_GetObjectItem ( pArrayItem, "fields")) && (pItem1 = cJSON_GetObjectItem ( pItem1, "partial1")))
                	{
                		pItem2 = NULL;
                		pItem1 = pItem2 ? pItem2 : pItem1;
                		v_did[iNum] = cJSON_GetObjectItem(pItem1, "did")->valueint;
  				v_uid[iNum] = cJSON_GetObjectItem(pItem1, "uid")->valueint;
                		iNum++;
  //				if(iNum%2)
 // 				{        
//  					utPltPutLoopVar(psDbHead, "class", iNum,  " ");
 // 				}	
 // 				else
 // 				{        
//  					utPltPutLoopVar(psDbHead, "class", iNum,  "class=\"tbl_alt\"");
//  				}
if(iNum>1){
	utPltPutLoopVar(psDbHead,"dh",iNum,",");
}
  				utPltPutLoopVarF(psDbHead,"iNum", iNum, "%lu", iNum);
  				utPltPutLoopVar(psDbHead,"id", iNum, cJSON_GetObjectItem ( pArrayItem, "_id")->valuestring);
  				utPltPutLoopVarF(psDbHead, "uid", iNum, "%d", cJSON_GetObjectItem(pItem1, "uid")->valueint);
  				utPltPutLoopVarF(psDbHead,"did",iNum,"%d",cJSON_GetObjectItem(pItem1, "did")->valueint);
  				utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S",cJSON_GetObjectItem(pItem1, "stime")->valueint));
  		//		urlencode(cJSON_GetObjectItem(pItem1, "url")->valuestring,caTemp, 1000);
  				
  				_utPltPutLoopVar(psDbHead, "url", iNum, cJSON_GetObjectItem(pItem1, "url")->valuestring);
  			  
  			  utPltPutLoopVar(psDbHead, "sip", iNum, cJSON_GetObjectItem(pItem1, "sip")->valuestring);
  			  utPltPutLoopVar(psDbHead, "dip", iNum, cJSON_GetObjectItem(pItem1, "dip")->valuestring);
			    utPltSetCvtHtml(1);
			  _utPltPutLoopVar(psDbHead, "dispname", iNum,cJSON_GetObjectItem(pItem1, "dispname")->valuestring);
			        if(pItemL&&(pItem2=cJSON_GetObjectItem(pItemL, "subject"))){
                 highlightstr = cJSON_GetArrayItem(pItem2, 0)->valuestring;
                  _utPltPutLoopVar(psDbHead, "subject", iNum,highlightstr);
              }
              else{
              	 _utPltPutLoopVar(psDbHead, "subject", iNum,cJSON_GetObjectItem(pItem1, "subject")->valuestring);
              }	
			  			if(pItemL&&(pItem2=cJSON_GetObjectItem(pItemL, "content"))){
                 highlightstr = cJSON_GetArrayItem(pItem2, 0)->valuestring;
                  _utPltPutLoopVar(psDbHead, "content", iNum,highlightstr);
              }
              else{
              	 _utPltPutLoopVar(psDbHead, "content", iNum,cJSON_GetObjectItem(pItem1, "content")->valuestring);
              }	
			  
			  			if(pItemL&&(pItem2=cJSON_GetObjectItem(pItemL, "bbsname"))){
                 highlightstr = cJSON_GetArrayItem(pItem2, 0)->valuestring;
                
                  _utPltPutLoopVar(psDbHead, "bname", iNum,highlightstr ? highlightstr:"");
              }
              else{
              
             	 _utPltPutLoopVar(psDbHead, "bname", iNum,cJSON_GetObjectItem(pItem1, "bbsname")->valuestring ?cJSON_GetObjectItem(pItem1, "bbsname")->valuestring:"" );
             	 
              }	
			  
			  
			  
  //			 _utPltPutLoopVar(psDbHead, "subject", iNum,cJSON_GetObjectItem(pItem1, "subject")->valuestring);
  	//		 _utPltPutLoopVar(psDbHead, "content", iNum, cJSON_GetObjectItem(pItem1, "content")->valuestring);
 			 utPltSetCvtHtml(0);
  	//		 _utPltPutLoopVar(psDbHead, "bname", iNum, cJSON_GetObjectItem(pItem1, "bbsname")->valuestring);
  			 _utPltPutLoopVar(psDbHead, "author", iNum, cJSON_GetObjectItem(pItem1, "author")->valuestring);
  			 
         lIp=ntohl(utComHostAddress(cJSON_GetObjectItem(pItem1, "dip")->valuestring));
         iReturn = pasUtlGetIpLocation(lIp,caArea,caName2);
      //    printf("iReturn=%d,lIp=%lu,caArea=%s\n",iReturn,lIp,caArea);
          if(iReturn == 1) {
            sprintf(caTemp,"%s,%s",caArea,caName2);
    //        printf("caTemp=%s\n",caTemp);
          }
          else{
                strcpy(caTemp,"");
          }
        utPltPutLoopVar(psDbHead,"iplocal",iNum,caTemp);


//  				printvar(client, psDbHead, pItem1, "dip", "dip", tmputf8, data_dip, iNum);
 // 				printvar(client, psDbHead, pItem1, "bbsname", "bbsname", tmputf8, data_bbsname, iNum);
 // 				printvar(client, psDbHead, pItem1, "url", "url", tmputf8, data_url, iNum);
 // 				printvar(client, psDbHead, pItem1, "subject", "subject", tmputf8, data_subject, iNum);
 // 				printvar(client, psDbHead, pItem1, "content", "content", tmputf8, data_content, iNum);
  				
                		
                		//cleanup
                		if(data_dispname.buf) free(data_dispname.buf);
                		if(data_sip.buf) free(data_sip.buf);
                		if(data_dip.buf) free(data_dip.buf);
                		if(data_bbsname.buf) free(data_bbsname.buf);
                		if(data_url.buf) free(data_url.buf);
                		if(data_subject.buf) free(data_subject.buf);
                		if(data_content.buf) free(data_content.buf);
                		data_dispname.shift = 0;
                		data_sip.shift = 0;
                		data_dip.shift = 0;
                		data_bbsname.shift = 0;
                		data_url.shift = 0;
                		data_subject.shift = 0;
                		data_content.shift = 0;
                	}
                }
	}
	cJSON_Delete(pRoot);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	int num = 0;
 	char username[17] = "";
  	char dispname[65] = "";
  	char ca_sql[256] = "";
  	for(num = 0; num < iNum; num++){
    		sprintf(ca_sql, "select username, dispname from ncsuser where userid = %d", v_did[num]);
    		
    		memset(username, 0, sizeof(username));
    		memset(dispname, 0, sizeof(dispname));
    		
    		pasDbOneRecord(ca_sql, 0, 
    				UT_TYPE_STRING, 16, username, 
    				UT_TYPE_STRING, 64, dispname);
    				
    		utPltPutLoopVar(psDbHead, "servicecode", num+1, username);
    		utPltPutLoopVar(psDbHead, "corpname", num+1, dispname);
    		              
   	}	
 	
 	char caMac[19] = "";
  	char caPosition[33] = "";
  	for(num = 0; num < iNum; num++){
       		sprintf(ca_sql, "select mac, position from ncsenduser where did = %d and uid = %d", v_did[num], v_uid[num]);
       		
       		memset(caMac,      0, sizeof(caMac));
       		memset(caPosition, 0, sizeof(caPosition));
       		
       		pasDbOneRecord(ca_sql, 0, 
       				UT_TYPE_STRING, 18, caMac, 
       				UT_TYPE_STRING, 32, caPosition);
       		utPltPutLoopVar(psDbHead, "mac", num+1, caMac);
    		utPltPutLoopVar(psDbHead, "position", num+1, caPosition);
  	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	free(v_did);
	free(v_uid);

}

//解析mail
void parse_mail_jsonstring_v4(utPltDbHead *psDbHead, int iSum, char *jsonstring, int *took, int *count, char *query)
{
	unsigned long lIp,iReturn,mType;
	char caArea[256],caName2[256],caTemp[1024];


	int *v_did = (int *)calloc(iSum, sizeof(int));
	if(v_did == NULL)
	{
		
		return;
	}
		
	int *v_uid = (int *)calloc(iSum, sizeof(int));
	if(v_uid == NULL)
	{	free(v_did);
	
		return;
	}
//printf("jsonstring=%s\n",jsonstring);
	cJSON* pRoot = cJSON_Parse ( jsonstring );
	cJSON *pArrayItem = NULL;
	cJSON *pItem = NULL;
	cJSON *pItem1 = NULL;
	cJSON *pItem2 = NULL;
	cJSON *pItemL = NULL;
	int i;
	int cnt;
	int iNum = 0;
	int j = 0;
	int cnt2 = 0;
	
	
	//char *out=cJSON_Print(pRoot);
	//printf("recv = %s\n",out);
	//free(out);
	
	*took = cJSON_GetObjectItem( pRoot, "took") ? cJSON_GetObjectItem( pRoot, "took")->valueint : 0;
	if(*took == 0)
	{
		*count = 0;
		cJSON_Delete(pRoot);
		free(v_did);
		free(v_uid);
		return;
	}
//	printf("pid=%d\n",getpid());
//	sleep(15);
	if((pItem = cJSON_GetObjectItem ( pRoot, "hits")) && (*count=cJSON_GetObjectItem( pItem, "total")->valueint,pItem = cJSON_GetObjectItem ( pItem, "hits")))
	{
		cnt = cJSON_GetArraySize( pItem);
	printf("cnt=%d\n",cnt);	
	printf("pid=%d\n",getpid());
	sleep(15);
		char *highlightstr = NULL;
		iNum=0;
		for( i= 0; i < cnt; i++)
                {
   
                	pArrayItem = cJSON_GetArrayItem(pItem, i);
                  pItemL = cJSON_GetObjectItem ( pArrayItem, "highlight");
             pItem1 = cJSON_GetObjectItem ( pArrayItem, "fields");
             if(pItem1){
             	cnt2 = cJSON_GetArraySize( pItem1);
             	printf("cnt2=%d\n",cnt2);
             	if(cnt2<=0) continue;
             		
             		
             	pItem1 = cJSON_GetArrayItem ( pItem1, 0);
  //           	pItem1 = cJSON_GetObjectItem ( pItem1, "partial1");
            }
            if(pItem1)
   //             	if((pItem1 = cJSON_GetObjectItem ( pArrayItem, "fields")) && (pItem1 = cJSON_GetObjectItem ( pItem1, "partial1")))
                	{
                		pItem2 = NULL;
                		pItem1 = pItem2 ? pItem2 : pItem1;
                		v_did[iNum] = cJSON_GetObjectItem(pItem1, "did")->valueint;
  				          v_uid[iNum] = cJSON_GetObjectItem(pItem1, "uid")->valueint;
                		iNum++;

if(iNum>1){
	utPltPutLoopVar(psDbHead,"dh",iNum,",");
}
  				utPltPutLoopVarF(psDbHead,"iNum", iNum, "%lu", iNum);
  				if(cJSON_GetObjectItem ( pArrayItem, "_id"))
  				  utPltPutLoopVar(psDbHead,"id", iNum, cJSON_GetObjectItem ( pArrayItem, "_id")->valuestring);
  				if(cJSON_GetObjectItem(pItem1, "uid"))
  				   utPltPutLoopVarF(psDbHead, "uid", iNum, "%d", cJSON_GetObjectItem(pItem1, "uid")->valueint);
  				   
  				if(cJSON_GetObjectItem(pItem1, "did"))
  				   utPltPutLoopVarF(psDbHead,"did",iNum,"%d",cJSON_GetObjectItem(pItem1, "did")->valueint);
  				
  				if(cJSON_GetObjectItem(pItem1, "stime"))
  				  utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S",cJSON_GetObjectItem(pItem1, "stime")->valueint));
  		//		urlencode(cJSON_GetObjectItem(pItem1, "url")->valuestring,caTemp, 1000);
  				
  			
  	//		  _uutPltPutLoopVar(psDbHead, "dispname", iNum,cJSON_GetObjectItem(pItem1, "dispname")->valuestring);
  	//		  utPltPutLoopVar(psDbHead, "sip", iNum, cJSON_GetObjectItem(pItem1, "sip")->valuestring);
  	//		  utPltPutLoopVar(psDbHead, "dip", iNum, cJSON_GetObjectItem(pItem1, "dip")->valuestring);
			  utPltSetCvtHtml(1);
  			
  			         
              if(pItemL&&(pItem2=cJSON_GetObjectItem(pItemL, "mailsub"))){
                 highlightstr = cJSON_GetArrayItem(pItem2, 0)->valuestring;
                  _utPltPutLoopVar(psDbHead, "mailsub", iNum,highlightstr);
              }
              else{
              	if(cJSON_GetObjectItem(pItem1, "mailsub"))
              	 _utPltPutLoopVar(psDbHead, "mailsub", iNum,cJSON_GetObjectItem(pItem1, "mailsub")->valuestring);
              }	
  			 if(pItemL&&(pItem2=cJSON_GetObjectItem(pItemL, "mailbody"))){
                 highlightstr = cJSON_GetArrayItem(pItem2, 0)->valuestring;
                  _utPltPutLoopVar(psDbHead, "mailbody", iNum,highlightstr);
               }
          else{
          	if(cJSON_GetObjectItem(pItem1, "mailbody")){
          	 highlightstr = cJSON_GetObjectItem(pItem1, "mailbody")->valuestring;
          	
          	 _utPltPutLoopVar(psDbHead, "mailbody", iNum,utStrGetCharHan(highlightstr,2000));
          }
  		//	     _utPltPutLoopVar(psDbHead, "mailbody", iNum, cJSON_GetObjectItem(pItem1, "mailbody")->valuestring);
         }
         
              if(pItemL&&(pItem2=cJSON_GetObjectItem(pItemL, "mailfrom"))){
                 highlightstr = cJSON_GetArrayItem(pItem2, 0)->valuestring;
                  _utPltPutLoopVar(psDbHead, "mailfrom", iNum,highlightstr);
              }
              else{
              	if(cJSON_GetObjectItem(pItem1, "mailfrom"))
              	 _utPltPutLoopVar(psDbHead, "mailfrom", iNum,cJSON_GetObjectItem(pItem1, "mailfrom")->valuestring);
              }	
              if(pItemL&&(pItem2=cJSON_GetObjectItem(pItemL, "mailto"))){
                 highlightstr = cJSON_GetArrayItem(pItem2, 0)->valuestring;
                  _utPltPutLoopVar(psDbHead, "mailto", iNum,highlightstr);
              }
              else{
              	if(cJSON_GetObjectItem(pItem1, "mailto"))
              	 _utPltPutLoopVar(psDbHead, "mailto", iNum,cJSON_GetObjectItem(pItem1, "mailto")->valuestring);
              }	
              if(pItemL&&(pItem2=cJSON_GetObjectItem(pItemL, "dispname"))){
                 highlightstr = cJSON_GetArrayItem(pItem2, 0)->valuestring;
                  _utPltPutLoopVar(psDbHead, "dispname", iNum,highlightstr);
              }
              else{
              	if(cJSON_GetObjectItem(pItem1, "dispname"))
              	 _utPltPutLoopVar(psDbHead, "dispname", iNum,cJSON_GetObjectItem(pItem1, "dispname")->valuestring);
              }	
               if(pItemL&&(pItem2=cJSON_GetObjectItem(pItemL, "sip"))){
                 highlightstr = cJSON_GetArrayItem(pItem2, 0)->valuestring;
                  _utPltPutLoopVar(psDbHead, "sip", iNum,highlightstr);
              }
              else{
              	 if(cJSON_GetObjectItem(pItem1, "dispname"))
              	 _utPltPutLoopVar(psDbHead, "sip", iNum,cJSON_GetObjectItem(pItem1, "dispname")->valuestring);
              }
     	        if(pItemL&&(pItem2=cJSON_GetObjectItem(pItemL, "dip"))){
                 highlightstr = cJSON_GetArrayItem(pItem2, 0)->valuestring;
                  _utPltPutLoopVar(psDbHead, "dip", iNum,highlightstr);
              }
              else{
              	if(cJSON_GetObjectItem(pItem1, "dip"))
              	  _utPltPutLoopVar(psDbHead, "dip", iNum,cJSON_GetObjectItem(pItem1, "dip")->valuestring);
              }
     	   
         
 			 utPltSetCvtHtml(0);
  			 if(cJSON_GetObjectItem ( pItem1, "style")){
  			 mType=cJSON_GetObjectItem ( pItem1, "style")->valueint;
  			 if(mType==1){
  			 	utPltPutLoopVar(psDbHead,"mtype",iNum,"SMTP");
  			}
  			else if(mType==2){
  				utPltPutLoopVar(psDbHead,"mtype",iNum,"POP3");
  			}
  			else if(mType==3){
  				utPltPutLoopVar(psDbHead,"mtype",iNum,"WebMail");
  			}
  		}
  		if(cJSON_GetObjectItem(pItem1, "dip")){
         lIp=ntohl(utComHostAddress(cJSON_GetObjectItem(pItem1, "dip")->valuestring));
         iReturn = pasUtlGetIpLocation(lIp,caArea,caName2);
      //    printf("iReturn=%d,lIp=%lu,caArea=%s\n",iReturn,lIp,caArea);
          if(iReturn == 1) {
            sprintf(caTemp,"%s,%s",caArea,caName2);
    //        printf("caTemp=%s\n",caTemp);
          }
          else{
                strcpy(caTemp,"");
          }
        utPltPutLoopVar(psDbHead,"iplocal",iNum,caTemp);
       }

                		
    
                	}
                }
	}
	cJSON_Delete(pRoot);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	int num = 0;
 	char username[17] = "";
  	char dispname[65] = "";
  	char ca_sql[256] = "";
  	for(num = 0; num < iNum; num++){
    		sprintf(ca_sql, "select username, dispname from ncsuser where userid = %d", v_did[num]);
    		
    		memset(username, 0, sizeof(username));
    		memset(dispname, 0, sizeof(dispname));
    		
    		pasDbOneRecord(ca_sql, 0, 
    				UT_TYPE_STRING, 16, username, 
    				UT_TYPE_STRING, 64, dispname);
    				
    		utPltPutLoopVar(psDbHead, "servicecode", num+1, username);
    		utPltPutLoopVar(psDbHead, "corpname", num+1, dispname);
    		              
   	}	
 	
 	char caMac[19] = "";
  	char caPosition[33] = "";
  	for(num = 0; num < iNum; num++){
       		sprintf(ca_sql, "select mac, position from ncsenduser where did = %d and uid = %d", v_did[num], v_uid[num]);
       		
       		memset(caMac,      0, sizeof(caMac));
       		memset(caPosition, 0, sizeof(caPosition));
       		
       		pasDbOneRecord(ca_sql, 0, 
       				UT_TYPE_STRING, 18, caMac, 
       				UT_TYPE_STRING, 32, caPosition);
       		utPltPutLoopVar(psDbHead, "mac", num+1, caMac);
    		utPltPutLoopVar(psDbHead, "position", num+1, caPosition);
  	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	free(v_did);
	free(v_uid);

}


//bbs全文检索

int ncsbbsFulltext_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	char caRe[4] = "";
	char caRn[16] = "";
	char caLm[16] = "";
	char caQuery[256] = "";
	char caTotal[16] = "";
	char caSort_flag[3] = "";
	char caQ1[256] = "";
	char caQ2[256] = "";
	char caQ3[256] = "";
	char caQ4[256] = "";
	char caQ5[16] = "";
	char caQ6[16] = "";
	
	char ca_query_text[1024] = ""; 
	char wd[1024]= "";
	char query[1024] = "";
	char qeystr[1024] = "";
	
	char ca_cur_pg[9] = "";
	int iSortFlag = 0;
	int iSum = 0;
	int took = 0;
	struct curl_data wdata;
	CURL *curl;
  	CURLcode res;
	char *jsonstr = NULL;
	char *field = "all";
	unsigned long lCp = 0;
	unsigned long iStart = 0;
	unsigned long lCount = 0;
	unsigned long start = 0;
	unsigned long end = time(NULL);
	utMsgPrintMsg(psMsgHead);
	utPltDbHead *psDbHead = utPltInitDb();
	char caStart[20],limit[20],sort[16],dir[16],time_flag[16],_keyword[128];
	unsigned long iTime_flag,days;
	   	utMsgGetSomeNVar(psMsgHead, 6,
   				     "start",    UT_TYPE_STRING,  sizeof(caStart)-1,     caStart,
   				     "limit",    UT_TYPE_STRING,  sizeof(limit)-1,     limit,
   				     "sort",     UT_TYPE_STRING,  sizeof(sort)-1,      sort,
   				     "dir",      UT_TYPE_STRING,  sizeof(dir)-1,       dir,
   				     "time_flag",UT_TYPE_STRING,  sizeof(time_flag)-1, time_flag,
   				     "keyword",  UT_TYPE_STRING,  sizeof(_keyword)-1, _keyword);
	iStart=atol(caStart);
  iSum=atol(limit);
  

  
  
/*	
	utMsgGetSomeNVar(psMsgHead, 14, 
				       "f", 	UT_TYPE_STRING, 3, 	caRe,
				       "rn",    UT_TYPE_STRING, 15, 	caRn,
	                               "lm",    UT_TYPE_STRING, 15, 	caLm, 
	                               "q",     UT_TYPE_STRING, 255, 	caQuery,           	
	                               "t",     UT_TYPE_STRING, 15,  	caTotal,          	
	                               "sf", 	UT_TYPE_STRING, 2, 	caSort_flag, 
	                               "q1",    UT_TYPE_STRING, 255,	caQ1,
	                               "q2",    UT_TYPE_STRING, 255,	caQ2,
	                               "q3",    UT_TYPE_STRING, 255,	caQ3,
	                               "q4",    UT_TYPE_STRING, 255,	caQ4,
	                               "q5",    UT_TYPE_STRING, 15, 	caQ5,//查询字段
	                               "q6",    UT_TYPE_STRING, 15, 	caQ6,
	                               "wd",    UT_TYPE_STRING, 255, 	wd, 
	                               "cp",    UT_TYPE_STRING, 8,  	ca_cur_pg); 
	utPltPutVar(psDbHead, "q1", caQ1);
	utPltPutVar(psDbHead, "q2", caQ2);
	utPltPutVar(psDbHead, "q3", caQ3);
	utPltPutVar(psDbHead, "q4", caQ4);
	utPltPutVar(psDbHead, "rn", caRn);
	utPltPutVar(psDbHead, "lm", caLm);
	if(strlen(caRn) > 0)
	{
	  	iSum = atoi(caRn);
                if(iSum == 10 || iSum == 20 || iSum == 50 || iSum == 100)
                {
                }
                else
                {
                        iSum = 10;
                }
	}
	else 
	{
		iSum = 10;
	}
	lCp = atol(ca_cur_pg);
	if (lCp == 0) 
		lCp = 1;
        iStart = (lCp - 1) * iSum;
	lCount = 0;
	utPltPutVarF(psDbHead, "cp", "%lu", lCp);
	utPltPutVarF(psDbHead, "start", "%lu", iStart+1);
	
	
	/////////////////////////////////////////////////////////////////////////////////
	if(strlen(caQ1) > 0 && !utStrIsSpaces(caQ1))
	{
	  	snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "%s ", caQ1);
	}
	if(strlen(caQ3) > 0 && !utStrIsSpaces(caQ3))
	{
		char *word = strtok(caQ3, SEPCHARS);
		if(word != NULL)
			if(strlen(ca_query_text) > 0)
				snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "AND ");
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "(%s", word);	
		word = strtok(NULL, SEPCHARS);
		while(word != NULL)
		{
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " OR %s", word);
			word = strtok(NULL, SEPCHARS);
		}
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text),  ")");
	}  
	if(strlen(caQ4) > 0 && !utStrIsSpaces(caQ4))
	{
		if(strlen(ca_query_text) > 0)
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " AND (");
		char *word = strtok(caQ4, SEPCHARS);
		while(word != NULL)
		{
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "NOT %s", word);
			word = strtok(NULL, SEPCHARS);
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "%s", word ? " " : "");
		}
		snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text),  ")");
	} 
	if(strlen(caQ2) > 0 && !utStrIsSpaces(caQ2))
	{
	  	if(strlen(ca_query_text) > 0)
			snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), " ");
	  	snprintf(ca_query_text+strlen(ca_query_text), sizeof(ca_query_text)-strlen(ca_query_text), "\"%s\"", caQ2);
	}
	if(strlen(wd) == 0 || utStrIsSpaces(wd))
	{
		memset(wd, 0, sizeof(wd));
		strncpy(wd, ca_query_text, sizeof(wd)-1);
	}
	*/
	
	strncpy(query, _keyword, sizeof(_keyword)-1);
	str_replace(query, '\"',  "&quot;");
//	utPltPutVar(psDbHead, "query", query);
	/////////////////////////////////////////////////////////////////////////////////
	
	/*
	if(strcmp(caRe, "re") == 0)
	{
	  	utPltPutVar(psDbHead, "query_text", caQuery);
      		utPltPutVar(psDbHead, "total",      caTotal);
      		return utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/re_bbs_search.html");
  	}
 */ 
  	if(strlen(sort) == 0 || strcmp(sort, "0") == 0)/* 按时间排序 */
	{
		iSortFlag = 1;
	}
	
	//unsigned long lLm = atol(time_flag);
	
	  iTime_flag = atoi(time_flag);
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
		start = end - days * 86400;
	
	/*	
	switch(atol(caQ5))
	{
		case 1:
			field = "dispname";
			break;
		case 2:
			field = "bbsname";
			break;
		case 3:
			field = "url";
			break;
		case 6:
			field = "subject";
			break;	
		case 7:
			field = "content";
			break;	
		case 8:
			field = "sip";
			break;	
		case 9:
			field = "dip";
			break;	
		default:
			break;
	}
	
	utMsgGetSomeNVar(psMsgHead, 4, 
	                               "q1",    UT_TYPE_STRING, 255,	caQ1,
	                               "q2",    UT_TYPE_STRING, 255,	caQ2,
	                               "q3",    UT_TYPE_STRING, 255,	caQ3,
	                               "q4",    UT_TYPE_STRING, 255,	caQ4); 
*/
//	if(!utStrIsSpaces(caQ1) || !utStrIsSpaces(caQ2) || !utStrIsSpaces(caQ3)) //指定了高级查询
//	{
//		jsonstr = createMatchBBSTermJsonString(field, iStart, iSum, iSortFlag, "", start, end, caQ1, caQ2, caQ3, caQ4, "");
//	}
/*	else if(!utStrIsSpaces(wd))
	{
		get_stbuf(wd, "\"", '\"', caQ2, sizeof(caQ2)-1);
		if(strcasestr(wd, "AND") || strcasestr(wd, "OR") || strcasestr(wd, "NOT") )
		{
			strcpy(qeystr, wd);
			str_delstr(qeystr, "\"");
			if(!utStrIsSpaces(caQ2))
			{
				str_delstr(qeystr, caQ2);
			}
			if(qeystr[strlen(qeystr)-1] == ' ')
			{
				qeystr[strlen(qeystr)-1] = '\0';
			}
			printf("qeystr = %s, caQ2 = %s\n", qeystr, caQ2);
			jsonstr = createMatchBBSTermJsonString(field, iStart, iSum, iSortFlag, "", start, end, "", caQ2, "", "", qeystr);
		}
		else 
		{
			if(strlen(caQ2) > 0 ) 
				wd[0] = '\0';
			jsonstr = createMatchBBSTermJsonString(field, iStart, iSum, iSortFlag, wd, start, end, "", caQ2, "", "", "");
		}
	}
	else 
	{
		jsonstr = createMatchAllJsonString(iStart, iSum, iSortFlag);
	}
	*/
	
	if(!utStrIsSpaces(_keyword)){
//		strcpy(qeystr, _keyword);
		
  }
		jsonstr = createMatchBBSTermJsonString_v4(field, iStart, iSum, iSortFlag, _keyword, start, end, "", "", "", "", "");
	
  	curl_global_init(CURL_GLOBAL_ALL);
  	curl = curl_easy_init();
  	if(!curl)
  		return 0;
  	curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:9200/dc/bbs/_search?pretty=true");
  	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE,  1L);
  	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  	memset(&wdata, 0, sizeof(struct curl_data));
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA,  &wdata);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr);//for XGET
	res = curl_easy_perform(curl);
    	if(res == CURLE_OK)
    	{
    		wdata.buf[wdata.shift] = '\0';
    		char word[1536] = "";
    		if(!utStrIsSpaces(wd))
    			snprintf(word, sizeof(word), "%s", wd);
    	
    			printf("buf=%s\n",wdata.buf);
    		parse_bbs_jsonstring_v4(psDbHead, iSum, wdata.buf, &took, (int *)&lCount, word);
    		
    		free(wdata.buf);
    	}
    	else
    	{
    		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    	}
    	curl_global_cleanup();
    	free(jsonstr);
    	if(lCount == 0 || iStart > lCount){
		    utPltPutVar(psDbHead, "TotRec", "0");
		
		    return utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/fulltext/ncs_bbs_list.html");
   	  }
   	     
//	unsigned long ltotpg = (lCount % iSum) ? (lCount / iSum + 1) : lCount / iSum;	//总页数 
	utPltPutVarF(psDbHead, "TotRec", "%lu", lCount);
	//utPltPutVarF(psDbHead, "total_times", "%d.%03d", took/1000, took%1000);
//	utPltPutVarF(psDbHead, "end", "%lu", lCp == ltotpg ? lCount : iStart+iSum);
		

//    	utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/search_bbs_result.html");
utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/fulltext/ncs_bbs_list.html");
	return 0;
}


//mail全文检索

int ncsMailFulltext_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	char caRe[4] = "";
	char caRn[16] = "";
	char caLm[16] = "";
	char caQuery[256] = "";
	char caTotal[16] = "";
	char caSort_flag[3] = "";
	char caQ1[256] = "";
	char caQ2[256] = "";
	char caQ3[256] = "";
	char caQ4[256] = "";
	char caQ5[16] = "";
	char caQ6[16] = "";
	
	char ca_query_text[1024] = ""; 
	char wd[1024]= "";
	char query[1024] = "";
	char qeystr[1024] = "";
	
	char ca_cur_pg[9] = "";
	int iSortFlag = 0;
	int iSum = 0;
	int took = 0;
	struct curl_data wdata;
	CURL *curl;
  	CURLcode res;
	char *jsonstr = NULL;
	char *field = "all";
	unsigned long lCp = 0;
	unsigned long iStart = 0;
	unsigned long lCount = 0;
	unsigned long start = 0;
	unsigned long end = time(NULL);
	utMsgPrintMsg(psMsgHead);
	utPltDbHead *psDbHead = utPltInitDb();
	char caStart[20],limit[20],sort[16],dir[16],time_flag[16],_keyword[128];
	unsigned long iTime_flag,days;
	   	utMsgGetSomeNVar(psMsgHead, 6,
   				     "start",    UT_TYPE_STRING,  sizeof(caStart)-1,     caStart,
   				     "limit",    UT_TYPE_STRING,  sizeof(limit)-1,     limit,
   				     "sort",     UT_TYPE_STRING,  sizeof(sort)-1,      sort,
   				     "dir",      UT_TYPE_STRING,  sizeof(dir)-1,       dir,
   				     "time_flag",UT_TYPE_STRING,  sizeof(time_flag)-1, time_flag,
   				     "keyword",  UT_TYPE_STRING,  sizeof(_keyword)-1, _keyword);
	iStart=atol(caStart);
  iSum=atol(limit);
  

 
	
	strncpy(query, _keyword, sizeof(_keyword)-1);
	str_replace(query, '\"',  "&quot;");
//	utPltPutVar(psDbHead, "query", query);
	/////////////////////////////////////////////////////////////////////////////////
	

  if(strlen(sort) == 0 || strcmp(sort, "0") == 0)/* 按时间排序 */
	{
		iSortFlag = 1;
	}
	
	//unsigned long lLm = atol(time_flag);
	
	  iTime_flag = atoi(time_flag);
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
		start = end - days * 86400;
	
	

		jsonstr = createMatchMailTermJsonString_v4(field, iStart, iSum, iSortFlag, _keyword, start, end, "", "", "", "", "");
	
  	curl_global_init(CURL_GLOBAL_ALL);
  	curl = curl_easy_init();
  	if(!curl)
  		return 0;
  	curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:9200/dc/mail/_search?pretty=true");
  	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE,  1L);
  	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  	memset(&wdata, 0, sizeof(struct curl_data));
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA,  &wdata);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr);//for XGET
	res = curl_easy_perform(curl);
    	if(res == CURLE_OK)
    	{
    		wdata.buf[wdata.shift] = '\0';
    		char word[1536] = "";
    		if(!utStrIsSpaces(wd))
    			snprintf(word, sizeof(word), "%s", wd);
    	
 //   			printf("buf=%s\n",wdata.buf);
    	  parse_mail_jsonstring_v4(psDbHead, iSum, wdata.buf, &took, (int *)&lCount, word);
    		
    		free(wdata.buf);
    	}
    	else
    	{
    		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    	}
    	curl_global_cleanup();
    	free(jsonstr);
    	if(lCount == 0 || iStart > lCount){
		    utPltPutVar(psDbHead, "TotRec", "0");
		
		    return utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/fulltext/ncs_mail_list.html");
   	  }
   	     
	utPltPutVarF(psDbHead, "TotRec", "%lu", lCount);

//    	utPltOutToHtml(iFd, psMsgHead, psDbHead, "ncs/search_bbs_result.html");
  utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/fulltext/ncs_mail_list.html");
	return 0;
}
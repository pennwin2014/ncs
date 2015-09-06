#ifndef _MAKE_MIME_H_
#define _MAKE_MIME_H_

#define SIZEDTEXT struct mail_sizedtext	

struct mime_type
{
        char filetype[24];
        char contype[128];
};

static struct mime_type type_table[83]={
        {"txt", "text/plain"},
        {"html","text/html"},
        {"htm", "text/html"},
        {"gif", "image/gif"},
        {"jpg", "image/jpeg"},
        {"jpe", "image/jpeg"},
        {"jpeg", "image/jpeg"},
        {"java", "text/plain"},
        {"body", "text/html"},
        {"rtx", "text/richtext"},
        {"tsv", "text/tab-separated-values"},
        {"etx", "text/x-setext"},
        {"ps", "application/x-postscript"},
        {"class", "application/java"},
        {"csh", "application/x-csh"},
        {"sh", "application/x-sh"},
        {"tcl", "application/x-tcl"},
        {"tex", "application/x-tex"},
        {"texinfo", "application/x-texinfo"},
        {"texi", "application/x-texinfo"},
        {"t", "application/x-troff"},
        {"tr", "application/x-troff"},
        {"roff", "application/x-troff"},
        {"man", "application/x-troff-man"},
        {"me", "application/x-troff-me"},
        {"ms", "application/x-wais-source"},
        {"src", "application/x-wais-source"},
        {"zip", "application/zip"},
        {"bcpio", "application/x-bcpio"},
        {"cpio", "application/x-cpio"},
        {"gtar", "application/x-gtar"},
        {"shar", "application/x-shar"},
        {"sv4cpio", "application/x-sv4cpio"},
        {"sv4crc", "application/x-sv4crc"},
        {"tar", "application/x-tar"},
        {"ustar", "application/x-ustar"},
        {"dvi", "application/x-dvi"},
        {"hdf", "application/x-hdf"},
        {"latex", "application/x-latex"},
        {"bin", "application/octet-stream"},
        {"oda", "application/oda"},
        {"pdf", "application/pdf"},
        {"ps", "application/postscript"},
        {"eps", "application/postscript"},
        {"ai", "application/postscript"},
        {"rtf", "application/rtf"},
        {"nc", "application/x-netcdf"},
        {"cdf", "application/x-netcdf"},
        {"cer", "application/x-x509-ca-cert"},
        {"exe", "application/octet-stream"},
        {"gz", "application/x-gzip"},
        {"Z", "application/x-compress"},
        {"z", "application/x-compress"},
        {"hqx", "application/mac-binhex40"},
        {"mif", "application/x-mif"},
        {"ief", "image/ief"},
        {"tiff", "image/tiff"},
        {"tif", "image/tiff"},
        {"ras", "image/x-cmu-raster"},
        {"pnm", "image/x-portable-anymap"},
        {"pbm", "image/x-portable-bitmap"},
        {"pgm", "image/x-portable-graymap"},
        {"ppm", "image/x-portable-pixmap"},
        {"rgb", "image/x-rgb"},
        {"xbm", "image/x-xbitmap"},
        {"xpm", "image/x-xpixmap"},
        {"xwd", "image/x-xwindowdump"},
        {"au", "audio/basic"},
        {"snd", "audio/basic"},
        {"aif", "audio/x-aiff"},
        {"aiff", "audio/x-aiff"},
        {"aifc", "audio/x-aiff"},
        {"wav", "audio/x-wav"},
        {"mpeg", "video/mpeg"},
        {"mpg", "video/mpeg"},
        {"mpe", "video/mpeg"},
        {"qt", "video/quicktime"},
        {"mov", "video/quicktime"},
        {"avi", "video/x-msvideo"},
        {"movie", "video/x-sgi-movie"},
        {"avx", "video/x-rad-screenplay"},
        {"wrl", "x-world/x-vrml"},
        {"mpv2", "video/mpeg2"}
        };
/*----------------------------------------------------------------------------*/
/* Content */
/* attach list */
struct post_attach {
	int type;			/*0,附件为url,1为内容*/
	char filename[128];		/* 保存的附件名*/
	char fileurl[512];
	char *outCon;	
	unsigned long filesize;		/* 附件大小*/		
	char content_type[128];		/* 附件类型*/
	struct post_attach *next;	/* 下一个附件 							*/
};

struct mimeMail{
    char from[128];
	char to[512];
	char subject[512];
	char cc[512];
	char bcc[512];
	char *content;
	int  conType;    /*0 文本 1 html*/
	char *signature;
	int flag;	//0无附件，1有附件
	struct post_attach *attach;
};

SIZEDTEXT {
	unsigned char *data;		/* text 								*/
	unsigned long size;		/* size of text in octets 				*/
};

int getFileConType(char *filename,char *fileConType);
struct mimeMail* newMail(char *strFrom,char *strTo,char *strCc,char *strSubject,char *strCon, int conType,char *signature );
int addAttachMent(struct mimeMail* xMail,char *strCon,int conLen,char *displayname,char *contentType);
int addAttachMentFromFile(struct mimeMail* xMail,char *fileurl,char *displayname,char *contentType);
int sendMimeMail(struct  mimeMail* xMail,char *mailfrom);
int freeMail(struct  mimeMail* xMail);
int sendMailConFromFile(char *msgurl, char *from);
int sendMailCon(char *msg, char *from, unsigned long len);

#endif




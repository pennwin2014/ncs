// #include "conf.h"
#if !defined(NDBM)
#define	NDBM		/* all supported platforms use NDBM now */
#endif

#if defined(__alpha)
typedef unsigned int	UINT4;
#else
typedef unsigned long	UINT4;
#endif

#if defined(unixware) || defined(sys5) || defined(M_UNIX)
#include        <string.h>
#else   /* unixware */
#include        <strings.h>
#endif  /* unixware */

#if defined(bsdi)
#include        <machine/inline.h>
#endif	/* bsdi */

#if defined(bsdi) || defined(__FreeBSD__)
#include        <machine/endian.h>
#else /* bsdi */
#include        <malloc.h>
#ifndef __USE_BSD
extern char *sys_errlist[];
#endif /* _USE_BSD */
#endif /* bsdi */

#include	<stdlib.h>
#include	<stdio.h>

#if defined(aix)
#include	<sys/select.h>
#endif	/* aix 	*/

#if !defined(NDBM)
#define dbm_fetch(db,x) fetch(x)
#define dbm_close(db)   dbm_close()
#endif

#if defined(SECURID) || defined(ACTIVCARD)
#define SMARTCARD
#endif

#define VPORTS		/* Enable virtual port feature */

/* #define PASSCHANGE *//* uncomment this to enable radpass -- not recommended */


#define AUTH_VECTOR_LEN		16
#define AUTH_PASS_LEN		16
#define AUTH_MAXPASS_LEN	48
#define AUTH_STRING_LEN		253	/* maximum of 253 */
#define MAX_LINE_SIZE		256
#define MAX_HOST_SIZE		128

typedef struct pw_auth_hdr {
	u_char		code;
	u_char		id;
	u_short		length;
	char		vector[AUTH_VECTOR_LEN];
	u_char		data[2];
} AUTH_HDR;

#define AUTH_HDR_LEN			20
#define CHAP_VALUE_LENGTH		16

#define PW_AUTH_UDP_PORT		1645
#define PW_ACCT_UDP_PORT		1646
#define PW_PROXY_UDP_PORT		1815
#define PW_PROXYACCT_UDP_PORT		1816

#define PW_TYPE_STRING			0
#define PW_TYPE_INTEGER			1
#define PW_TYPE_IPADDR			2
#define PW_TYPE_DATE			3


#define	PW_AUTHENTICATION_REQUEST	1
#define	PW_AUTHENTICATION_ACK		2
#define	PW_AUTHENTICATION_REJECT	3
#define	PW_ACCOUNTING_REQUEST		4
#define	PW_ACCOUNTING_RESPONSE		5
#define PW_ACCESS_CHALLENGE		11

#define	PW_USER_NAME			1
#define	PW_PASSWORD			2
#define	PW_CHAP_PASSWORD		3
#define	PW_CLIENT_ID			4
#define	PW_CLIENT_PORT_ID		5
#define	PW_USER_SERVICE_TYPE		6
#define	PW_FRAMED_PROTOCOL		7
#define	PW_FRAMED_ADDRESS		8
#define	PW_FRAMED_NETMASK		9
#define	PW_FRAMED_ROUTING		10
#define	PW_FRAMED_FILTER_ID		11
#define	PW_FRAMED_MTU			12
#define	PW_FRAMED_COMPRESSION		13
#define	PW_LOGIN_HOST			14
#define	PW_LOGIN_SERVICE		15
#define	PW_LOGIN_TCP_PORT		16
#define PW_PORT_MESSAGE			18
#define PW_DIALBACK_NO			19
#define PW_DIALBACK_NAME		20
#define PW_FRAMED_ROUTE			22
#define PW_FRAMED_IPXNET		23
#define PW_STATE			24
#define PW_CLASS			25
#define PW_VENDOR			26
#define PW_SESSION_TIMEOUT		27
#define PW_TERMINATION			29
#define PW_CALLED			30
#define PW_CALLING			31
#define PW_PROXY			33

#define PW_ACCT_STATUS_TYPE		40
#define PW_ACCT_DELAY_TIME		41
#define PW_ACCT_INPUT_OCTETS		42
#define PW_ACCT_OUTPUT_OCTETS		43
#define PW_ACCT_SESSION_ID		44
#define PW_ACCT_AUTHENTIC		45
#define PW_ACCT_INPUT_PACKETS		47
#define PW_ACCT_OUTPUT_PACKETS		48

#define PW_ACCT_TERMINATE_CAUSE		49
#
#define PW_CHAP_CHALLENGE		60
#define PW_NAS_PORT_TYPE		61
#define PW_CONNECT_INFO			77
/*
 * Non-Protocol Attributes
 */
#define PW_EXPIRATION			  21
#define PW_AUTHTYPE			1000
#define PW_MENU				1001
#define PW_TERMINATION_MENU		1002
#define PW_PREFIX			1003
#define PW_SUFFIX			1004
#define PW_GROUP			1005
#define PW_CRYPT_PASSWORD		1006
#define PW_CONNECT_RATE			1007

/*
 *	INTEGER TRANSLATIONS
 */

/*	USER TYPES	*/

#define	PW_LOGIN_USER			1
#define	PW_FRAMED_USER			2
#define	PW_DIALBACK_LOGIN_USER		3
#define	PW_DIALBACK_FRAMED_USER		4
#define	PW_OUTBOUND_USER		5
#define	PW_ADMIN_USER			6
#define	PW_PROMPT_USER			7
#define PW_CALL_CHECK_USER      	10
#define PW_OLD_CALL_CHECK_USER      	129

/*	FRAMED PROTOCOLS	*/

#define	PW_PPP				1
#define	PW_SLIP				2

/*	FRAMED ROUTING VALUES	*/

#define	PW_NONE				0
#define	PW_BROADCAST			1
#define	PW_LISTEN			2
#define	PW_BROADCAST_LISTEN		3

/*	FRAMED COMPRESSION TYPES	*/

#define	PW_VAN_JACOBSON_TCP_IP		1

/*	LOGIN SERVICES	*/

#define	PW_TELNET			0
#define	PW_RLOGIN			1
#define	PW_TCP_CLEAR			2
#define	PW_PORTMASTER			3

/*	AUTHENTICATION LEVEL	*/

#define PW_AUTH_NONE			0
#define PW_AUTH_RADIUS			1
#define PW_AUTH_LOCAL			2

/*	STATUS TYPES	*/

#define PW_STATUS_START			1
#define PW_STATUS_STOP			2

/*	TERMINATION OPTIONS	*/

#define PW_TERM_DEFAULT			0
#define PW_TERM_RADIUS_REQUEST		1

/*	Internal Authentication Types	*/

#define PW_AUTHTYPE_NONE		-1
#define PW_AUTHTYPE_LOCAL		0
#define PW_AUTHTYPE_UNIX		1
#define PW_AUTHTYPE_SECURID		2
#define PW_AUTHTYPE_CRYPT		3
#define PW_AUTHTYPE_REJECT		4
#define PW_AUTHTYPE_ACTIVCARD		5

/*	Proxy Flags			*/

#define PEER_NOPROXY			1  /* do not include Proxy-State */
#define PEER_OLDACCT			2  /* do not check sig on acct-response */
#define PEER_ADMINOK			4  /* allow admin logins by proxy */
#define PEER_IPASS			8  /* forward using ipass protocol */

/*	AUTH_REQ Flags			*/

#define REQ_PROXY			1  /* packet was forwarded */
#define REQ_DUP				2  /* packet is a duplicate */
#define REQ_ERR				4  /* packet is in error */
#define REQ_FREE			8  /* packet can be freed */

/*	build_packet Flags		*/

/*	Report Indices			*/

#define FW_REPLY			0  /* reply to client */
#define FW_SERVER			1  /* forward to server */
#define FW_CLIENT			2  /* forward reply to client */


#define	RR_PORT1			1 /* packets received on RADIUS port */
#define	RR_PORT2			2 /* packets received on RADIUS accounting port */
#define	RR_PORT3			3 /* packets received on RADIUS proxy port */
#define	RR_PORT4			4 /* packets received on RADIUS acct proxy port */
#define	RR_ACCEPT			5 /* access-accepts sent */
#define	RR_REJECT			6 /* access-rejects sent */
#define	RR_CHALLENGE			7 /* access-challenges sent */
#define	RR_ACCOUNT			8 /* accounting-responses sent */

#define RR_MAX				20

/* Default Database File Names */

#define RADIUS_DIR		"."
#define RADACCT_DIR		"../data"

#define RADIUS_DICTIONARY	"dictionary"
#define RADIUS_CLIENTS		"clients"
#define RADIUS_CLIENT_CACHE	"clcache"
#define RADIUS_PROXY		"proxy"
#define RADIUS_USERS		"users"

/* Server data structures */

typedef struct dict_attr {
	char			name[32];
	int			value;
	int			type;
	int			vendor;	/* non-zero for Vendor-Specific */
	int			vsvalue;
	struct dict_attr	*next;
} DICT_ATTR;

typedef struct dict_value {
	char			attrname[32];
	char			name[32];
	int			value;
	struct dict_value	*next;
} DICT_VALUE;

typedef struct value_pair {
	char		name[32];
	int			attribute;
	int			type;
	int			vendor;
	int			vsattribute;
	UINT4			lvalue;	/* length of strvalue if present */
	char			strvalue[AUTH_STRING_LEN];
	struct value_pair	*next;
} VALUE_PAIR;

typedef struct auth_req {
	UINT4			ipaddr;
	u_short			udp_port;
	u_char			id;
	u_char			code;
	char			vector[AUTH_VECTOR_LEN];
	char			secret[20];	/* max length supported is 16 */
	VALUE_PAIR		*request;
	int			    child_pid;	/* Process ID of child */
	UINT4			timestamp;
	struct auth_req		*next;		/* Next active request */
	UINT4			forw_addr;
	u_short			forw_port;
	u_char			forw_id;
	u_char			flags;
	char			forw_vector[AUTH_VECTOR_LEN];
	char			forw_secret[20];
	char			name[64];
	char			realm[64];
	char			*packet;
} AUTH_REQ;

typedef struct peer {
	UINT4			ipaddr;
	struct peer		*next;
	u_short			radport;
	u_short			acctport;
	char			realm[64];
	char			secret[20];
	u_char			id;
	u_char			flags;
	int			ip_kind_id;
} PEER;


#define DEBUG	if(debug_flag)log_debug

#define SECONDS_PER_DAY		86400
#define CLEANUP_DELAY		5
#define MAX_REQUESTS		100	/* -q */
#define MAX_REQUEST_TIME	30	/* -t */
#define MAX_PROXY_TIME		30	/* -w */

#define RADIUS_MSG_KEY(pid)	(('r' << 24) + ((pid) & 0x00ffffff))

extern int debug_flag;
extern void debug_pair (VALUE_PAIR * pair);

void	log_debug(), log_err(), pairfree(), reqfree();

/******Add by Woodrow******/
#define	PW_BILLING_RESPONSE	255

/* define users status */
#define ON			1
#define OFF			0

#define TRUE			1	
#define FALSE			0	

/* dial user type */
#define REG_USER		0
#define CARD_USER		1
#define CALLING_USER		2
#define VIP_USER		3

#define IP_SEGMENT_NUMBER 	10

typedef struct billing_center_str {
	char			server_name[128];
	char			ipaddr[64];
	char			region_name[128];
	char			port[8];
} BILLING_CENTER_STR;

typedef  struct user_kind {
	int			id;
	char			ip_kind[128];
	struct	user_kind	*next;
} USER_KIND;

typedef  struct nas_device {
	char 		name[32];
	UINT4		ipaddr;
	int		port_number;
	int		order;
	struct  nas_device  	*next;
} NAS_DEVICE;

typedef struct ip_struct {
	char		nas_name[32];
	int		userkind;
	UINT4		start_ip;
	UINT4		netmask;
	int		ip_number;
	char		filter_id[64];
	struct ip_struct	*next;
} IP_STRUCT;

typedef struct nas_struct {
	char		user_name[32];
	int		user_kind;
	char		calling_number[20];
	long		port;
	int		ip_seg;
	UINT4		ipaddr;
	int		flag;
} NAS_STRUCT;

typedef  struct file_head {
	UINT4		file_id; /* file identifier */
	UINT4		version;		
	UINT4		size;  /*  file size */
	UINT4		reserve[5]; 
	int		number_nas; /* nas number */
	UINT4		number_userkind;  /* userkind  number */
	int		lock_flag; /* 0 is free 1 is locked */
	UINT4		lock_time;
} FILE_HEAD;

typedef struct  ip_queue {
	int		ip_seg;
	long		port;
	int		flag;
} IP_QUEUE;

typedef struct nas_index {
	char	   nas_name[32];
	UINT4	   nas_ip_addr;
	int	   ip_seg[10];
	int	   ip_number[10][IP_SEGMENT_NUMBER]; /* according user kind */
	int	   port_total;
	int	   port_start_position;
	UINT4	   start_address[10][IP_SEGMENT_NUMBER]; /* according user kind */
	int	   ip_start_position[10][IP_SEGMENT_NUMBER];
} NAS_INDEX; 

typedef	struct nas_inode {
	NAS_DEVICE		*nas;
	int			port_total;
	int			nas_total;
} NAS_INODE;

typedef struct pw_billing_table {
        UINT4                   ipaddr;
        u_short                 udp_port;
	char    		acct_buffer[4096];
	int			length;
	u_char			vector[16];
	char			key[20];
	char			acct_session_id[20];
	long			curtime;
	struct pw_billing_table	*next;
} BILLING_TABLE;

typedef struct local_auth_user {
	char			name[16];
	struct local_auth_user	*next;
} LOCAL_AUTH_USER;

typedef struct called_limited {
	int			user_type;
	char			called_num[64];
	struct called_limited	*next;
} CALLED_LIMITED;

/******Add by Woodrow******/





/********** Begin Add by fred Zhang 2001-12-5************/



typedef  struct telList{

        char telno[20];
        struct telList * next;

}TELLIST;


/*********** End Add by Fred Zhang 2001-12-5*************/


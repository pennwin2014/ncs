#ifndef   _ARPUB_H_
#define   _ARPUB_H_

#ifndef   _STDIO_H_
#include  <stdio.h>
#define   _STDIO_H_
#endif

#ifndef   _SYS_IPC_H_
#include  <sys/ipc.h>
#define   _SYS_IPC_H_
#endif

#ifndef   _SYS_MSG_H_
#include  <sys/msg.h>
#define   _SYS_MSG_H_
#endif
#ifndef   _SYS_SHM_H_
#include  <sys/shm.h>
#define   _SYS_SHM_H_
#endif

#ifndef   _SIGNAL_H_
#include  <signal.h>
#define   _SIGNAL_H_
#endif

#ifndef   _SYS_TIMES_H
#include  <sys/times.h>
// #define   _SYS_TIMES_H
#endif
#ifndef   _SYS_TIME_H
#include  <sys/time.h>
//#define   _SYS_TIME_H
#endif

#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <arpa/inet.h>
#include  <netdb.h>

#ifndef   _SYS_ERRNO_H_
#include  <sys/errno.h>
#define   _SYS_ERRNO_H_
#endif

#ifndef   _UNISTD_H_
#include  <unistd.h>
#define   _UNISTD_H_
#endif

/**
#ifndef	u_char
typedef unsigned char 	u_char;
#endif
#ifndef	uchar
typedef unsigned char 	uchar;
#endif

#ifndef	u_long
typedef unsigned long	u_long;
#endif
#ifndef	ulong
typedef unsigned long	ulong;
#endif

#ifndef	u_short
typedef unsigned short	u_short;
#endif
#ifndef	ushort
typedef unsigned short	ushort;
#endif
***/

typedef enum {
	Log_3,		/* all User Data and Protocol Data */
	Log_2,		/* User Data and main Protocol Data */
	Log_1,		/* only make log of User Data */
	Log_0,		/* Mini User Data */
	Log_x,		/* Detail Log Level */
} Log_Level;

#define IPC_SEM_P		        -1
#define IPC_SEM_V			1
#define IPC_SEM_SIGNALDISABLE		1
#define IPC_SEMWAIT      		1
#define IPC_SEMTHROUGHT  		0
#define MAX_SEM_SEMNUM  		10


#ifdef RS6000
#define TRUE  0
#define FALSE  -1
#endif

#define OK 0

extern int iErrno;
extern int iSysLogLvl;

#endif

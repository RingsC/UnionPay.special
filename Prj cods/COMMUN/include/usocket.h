/*************************************
 *
 *
 *
 *
 *
 *
 *
 * *********************************/

#ifndef __USOCKET_H__
#define __USOCKET_H__

#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/epoll.h>
#include <fcntl.h>
//#include <stdarg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include "globalvars.h"
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/select.h>
	
	typedef struct {
		char szListenIP  [20] ;	//just likes decimal dot format : xxxx.xxxx.xxxx.xxxx
		int  iListenPort  ;	//Listening port .
		int  iTimeOutValue ;	//Timeout Value , UNIT:miliseconds
	} LISTEN_SKT_INFO ;

#endif 

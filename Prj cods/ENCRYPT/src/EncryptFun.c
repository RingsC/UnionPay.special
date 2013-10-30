/***********************************************************************************
 *	Copyright : 
 *			All rights reserved by lihao and Palm info.
 *	Date:
 *			Nov 8 2008 
 *	Author:	
 *			Lihao 
 *	Purpose:
 *			To send a string to encryption machine, then get the answer from 
 *			encryption mechine via TCP/IP 
 *	change List:
 *			NONE.
 *
 *
 **********************************************************************************/

#include "EncryptFun.h"

static int Encrypt_Socket_Open() ;
static int Encrypt_Socket_Close() ;
static int isActive ; //The flag of connection 
static int CharToInt (char) ;	//Convert a char to integer.
static int sfd ;


int Encrypt_PIN_str (const char* szStr, char* szOutput) 
{ //To caculate the PIN value
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff)) ;
	if (!strlen(szStr)){
		sprintf (szLogBuff,"[%s-line:%d] You have passed an invalid PIN,pls chck your ssytem...",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
		return  -1; 	
	}
	
	char szOriginalStr [MAXCONTLEN] ; //To hold the original string.
	bzero (szOriginalStr,sizeof(szOriginalStr));
		
	char szTmp [MAXCONTLEN] ;
	bzero (szTmp,sizeof(szTmp));
	read_config_file ("./sysconfig.cfg","PIN_VALUE",szTmp);
	if (!strlen(szTmp)){
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] Can not read PIN value from configuration file properlly...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		WriteLogFileInHex_Ex (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1;
	}
	
	sprintf (szOriginalStr,"601001%s0%d%s",szTmp,strlen(szStr),szStr) ;
	bzero (szTmp,sizeof(szTmp));
	StringOfChar('F',14-strlen(szStr),szTmp);	
	strcat (szOriginalStr,szTmp) ;
	
	bzero(szTmp,sizeof(szTmp));
	bzero (szOutput,sizeof(szOutput));
	if (!Encrypt_str_Encrypt (szOriginalStr,szTmp)){
		strncpy (szOutput,szTmp,strlen(szTmp)) ;
		return 0;		
	} else 
		return -1; 	
	

}
int Encrypt_MAC_Str (const char* szStr ,char* szOutput ) 
{//To caculate the MAC Value
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff));
	
	if (strlen(szStr)||szStr==NULL){ //To handle the exception.
		sprintf (szLogBuff,"[%s-line:%d] You have passed an invalid String to Encrypt,pls chck your sytem...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;	
	}
	char szMAC [48+1];
	bzero (szMAC,sizeof(szMAC)) ;
	read_config_file ("./sysconfig.cfg","MAC_VALUE",szMAC) ;	
	if (!strlen(szMAC)){
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d]Can not read MAC from configuration file properly...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		WriteLogFileInHex_Ex (szMAC,__SYS_TYPE_LOGFILE_RAW__);
		return -1 ;
	}
	
	char szOrigStr [MAXCONTLEN] ;
	bzero (szOrigStr,sizeof(szOrigStr)) ;	//To hold the original string,which will be sent to encrypt
	sprintf (szOrigStr,"80001%s%03d%s%s",szMAC,strlen(szStr),szStr,szStr); //Just like 80001005ASDFGASDFG
	
	bzero (szOutput,sizeof(szOutput)) ;
	return	Encrypt_str_Encrypt (szOrigStr,szOutput) ;
}
static int CharToInt (char cChar)
{
	char ss[2] ;
	bzero (ss,2) ;
	sprintf (ss,"%c",cChar) ;
	return atoi (ss) ;
}
int Encrypt_str_Encrypt (const char* szMacStr,char* szOutput)
{
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff)) ;
	
	int fd = Encrypt_Socket_Open() ;
	if (fd==-1|| fd<0){
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] Open the socket failed...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		Encrypt_Socket_Close(fd);
		return -1 ;
	}
	char szTmp [30] ;
	bzero (szTmp,sizeof(szTmp));	
	read_config_file ("./sysconfig.cfg","TIMEOUT_VAL",szTmp); 
	if (!strlen(szTmp))	
		sprintf (szTmp,"%d" ,3)  ;
	struct timeval tv;
	bzero (&tv ,sizeof(struct timeval)) ;
	tv.tv_sec = atoi (szTmp);	
	tv.tv_usec = atoi (szTmp);
	
	fd_set rset ,wset  ;
	FD_ZERO (&rset) ;
	FD_ZERO (&wset) ;
	
	//To set the readable and writable set .
	FD_SET (fd,&rset) ;
	FD_SET (fd,&wset) ;
	
	int iRet  = select (1024,NULL,&wset,NULL,&tv);
	if (iRet){
		//Here ,we will assemble the mac string,then send it.
		int ilen = strlen (szMacStr) ;	
		char sllen  [5] ;
		bzero (sllen,sizeof(sllen)) ;
		sprintf (sllen,"%d",ilen) ; //same as fj
		
		int ymod  = ilen % 256 ;
		ilen = ilen/ 256  ;
		char szSentMsg [MAXCONTLEN] ;
		bzero (szSentMsg,sizeof(szSentMsg)) ;
		char szReadMsg [MAXCONTLEN] ;	//To hold the response message .
		bzero (szReadMsg,sizeof(szReadMsg));
		
		sprintf (szSentMsg,"%x%x%s",ilen,ymod,szMacStr) ;		
		iRet  = write (fd,szSentMsg,strlen(szSentMsg)) ;
		if (iRet== strlen(szSentMsg)) {
			bzero (szLogBuff,sizeof(szLogBuff));
			sprintf (szLogBuff,"Send the request msg to Encrypt Successfully...") ;
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
			WriteLogFileInHex_Ex (&szSentMsg,__SYS_TYPE_LOGFILE_RAW__) ;	
		} else{	//Can not send data totally.
			bzero (szLogBuff,sizeof(szLogBuff)) ;
			sprintf (szLogBuff,"[%s-line:%d] Send the request msg to Encrypt Failed,pls chck your system...",__FILE__,__LINE__) ;
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFileInHex_Ex (szSentMsg,__SYS_TYPE_LOGFILE_RAW__) ;
			Encrypt_Socket_Close(fd) ;
			strncpy (szOutput,"-1",2);
			return  -1 ;	
		}
		//Here , we will read the response msg from encryption mechine.
		iRet  = select (1024,&rset,NULL,NULL,&tv) ;
		if (iRet){
			if (FD_ISSET (fd,&rset)){
				iRet = read (fd,szReadMsg,2) ; //First 2 character are length of following
				if (iRet!=2){ //Can not read properlly.
				 	bzero (szLogBuff,sizeof(szLogBuff)) ;
					sprintf (szLogBuff,"[%s-line:%d] can not read length,pls chck your sys...",__FILE__,__LINE__) ;
					WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
					WriteLogFileInHex_Ex (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
					strncpy (szOutput,"-1",sizeof("-1")) ; //The Value of executation.
					Encrypt_Socket_Close(fd) ;
					return -1; 
				}
				ilen = CharToInt(szReadMsg[0])*256+CharToInt(szReadMsg[1]) ;
				bzero (szReadMsg,sizeof(szReadMsg)) ;
				iRet = read (fd,szReadMsg,ilen);	
				if (iRet!= ilen) {
					bzero (szLogBuff,sizeof(szLogBuff)) ;
					sprintf (szLogBuff,"[%s-line:%d] Read The following data error,pls check your system...",__FILE__,__LINE__) ;
					
					WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
					WriteLogFileInHex_Ex(szReadMsg,__SYS_TYPE_LOGFILE_RAW__) ;
					Encrypt_Socket_Close(fd) ;
					strncpy (szOutput,"-1",sizeof("-1")) ;
					return -1; 	
				}
				char sResCode  [3] ;
				bzero (sResCode,sizeof(sResCode)) ;//________________________
				strncpy (sResCode,szReadMsg+2,2) ;//|_0_|_1_|_2_|_3_|_4_|_5_|
				if (strcmp ("00",sResCode)==0){
					bzero (szLogBuff,sizeof(szLogBuff)) ;
					if (strlen(szReadMsg)==4) {
						sprintf (szLogBuff,"Send the reponse to server...") ;
						strncpy (szOutput,"0",1) ;
						WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
					}else {
						sprintf (szLogBuff,"send the Response message to server...") ;
						strncpy (szOutput,szReadMsg+4,strlen(szReadMsg)-4) ;
						WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
						WriteLogFile (szReadMsg+4,__SYS_TYPE_LOGFILE_RAW__) ;
					}
				}else{
					strncpy (szOutput,"-1",sizeof("-1"));
					Encrypt_Socket_Close(fd) ;
					return  -1 ;
				}	
			}
		}else if (iRet==-1){
			bzero (szLogBuff,sizeof(szLogBuff)) ;
			sprintf (szLogBuff,"[%s-line:%d] select() Exceptions occur,pls check your system...",__FILE__,__LINE__) ;
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
			Encrypt_Socket_Close(fd);
			strncpy (szOutput,"-1",sizeof("-1"));
			return  -1; 
		}else{
			bzero (szLogBuff,sizeof(szLogBuff)) ;
			sprintf (szLogBuff,"[%s-line:%d] Timeout Exceptions occur,pls check your system...",__FILE__,__LINE__) ;
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
			Encrypt_Socket_Close(fd);
			strncpy (szOutput,"-1",sizeof("-1")) ;
			return -1 ;
		}
	} else if (iRet==-1){
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] Select() exceptions occur...",__FILE__,__LINE__);	
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
		Encrypt_Socket_Close(fd)  ;
		strncpy (szOutput,"-1",sizeof("-1")) ;
		return -1 ;	
	} else {
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] Select() timeout occur...",__FILE__,__LINE__);	
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
		Encrypt_Socket_Close(fd)  ;
		strncpy (szOutput,"-1",sizeof("-1")) ;
		return -1 ;	
	}	
	
	return 0 ;
}
static int Encrypt_Socket_Open () 
{
	
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff));	
	char szTmp [30] ;
	bzero (szTmp,sizeof(szTmp));
	isActive  = 0 ;	
	
	read_config_file ("./sysconfig.cfg","ENCRYPT_IP",szTmp) ;
	if (!strlen(szTmp)){
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] can not read the addr of Encryp from cfg file...",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
		return -1  ;
	}
	sfd  = socket (AF_INET,SOCK_STREAM,0) ;
	if (sfd ==-1) {
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] Can not create a socket handle, pls check your system...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;
	}
	
	struct sockaddr_in sin ;
	bzero (&sin,sizeof(struct sockaddr_in)) ;
	sin.sin_addr.s_addr = inet_addr (szTmp) ;
	sin.sin_family = AF_INET ;
	bzero (szTmp,sizeof(szTmp)) ;
	read_config_file ("./sysconfig.cfg","ENCRYPT_PORT",szTmp) ;
	if (!strlen(szTmp))	
		sprintf (szTmp,"%s","8888") ;	
	sin.sin_port = htons (atoi(szTmp)) ;
	
	int ret  = connect (sfd,(struct sockaddr*) &sin,sizeof(struct sockaddr_in));		
	if (ret==-1 ){
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] can not connect to encryption,pls check your system...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;
	}
	isActive  = 1 ; //To set flag of connection.
	return sfd ; 
}
static int Encrypt_Socket_Close (int fd)
{
	char szLogBuff [MAXCONTLEN] ;
	
	//if (isActive){
		shutdown(sfd,2) ;
		close (sfd); 
		sfd  =-1  ;
		isActive =0 ;
		bzero (szLogBuff,sizeof(szLogBuff));
		sprintf (szLogBuff,"[%s-line:%d] ",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
	//}
	return 0 ;
}

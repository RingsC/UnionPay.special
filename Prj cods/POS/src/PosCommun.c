/*****************************************************************************************
 *
 *	Copyright Information:
 *		All rights reversed by lihao and pamlm.
 *	Author:
 *		lihao
 *	Purpose:
 *		To implement the communication interface of POS
 *	Date:
 *		NOv 5 2008
 *	Change Lists:
 *
 *		NONE
 *	Version:
 *		1.0 ver
 *
 ****************************************************************************************/

#include "PosCommun.h"

static int p_fd = -1 ;
int Pos_Socket_Send_Ex (char* szMsgBuffer, int iSendLen)
{//Sent the message by socket .
	int iFd  ;
	char szLogBuffer [MAXCONTLEN] ;

	if ((iFd = Pos_Socket_Init ())!=0){ //Error 
		bzero (szLogBuffer,sizeof(szLogBuffer)) ;
		sprintf(szLogBuffer,"Can not Initit the socket handle successfully,pls chck your sys...") ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__);
		return -1 ;
	}
	int iRet  ;
	if ((iRet= Pos_Socket_Conn(iFd))!=0){ //can not connect to serer in timeout_value seconds.i
		Pos_Socket_Close(iFd) ; // To close the openned socket.
		return  -1; 
	}
	
	if((iRet = Pos_Socket_Send(iFd,szMsgBuffer,iSendLen)) !=0){
		bzero (szLogBuffer,sizeof(szLogBuffer)) ;
		sprintf (szLogBuffer,"Can not send the message to peer sucessfully...") ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		WriteLogFileInHex_Ex (szMsgBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		Pos_Socket_Close(iFd) ;
		return  -1; 
	} else{
		bzero (szLogBuffer,sizeof(szLogBuffer)) ;
		sprintf (szLogBuffer,"Sending the Message to peer successfully...") ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		WriteLogFileInHex_Ex (szMsgBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		Pos_Socket_Close (iFd);
	}
	
	return  0;
}
int Pos_Socket_Read_Ex (char* szMsgBuffer, int iReadLen) 
{
	int iFd,iRet,iFlag;
	char szLogBuffer [MAXCONTLEN] ;
	
	bzero (szLogBuffer,sizeof(szLogBuffer));
	if((iFd = Pos_Socket_Init())!= 0) {
		sprintf (szLogBuffer,"Can not initialize the socket successfully,pls check your sys...") ;
		iFlag =-1 ;
	} else{
		if ((iRet=Pos_Socket_Conn (iFd))!=0 ){ //Connection timeout .
			Pos_Socket_Close (iFd) ;
			sprintf (szLogBuffer,"Can not connect to peer,pls check your system...") ;
			iFlag = -1 ;
		} else{
			if ((iRet=Pos_Socket_Read (iFd,szMsgBuffer,iReadLen))!=0){
				sprintf (szLogBuffer,"Can not send the msg to peer,pls check your sys...") ;
				iFlag = -1;
			} else{
				sprintf (szLogBuffer,"Send the msg to peer successfully,a good job :-)...") ;
				iFlag =0 ;	
			}
			Pos_Socket_Close(iFd) ;
		}
	}

	WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
	WriteLogFileInHex_Ex (szMsgBuffer,__SYS_TYPE_LOGFILE_RAW__) ;	
	return iFlag  ;
}
int Pos_Socket_Init () 
{
	//int fd ; //Which is used to represent socket handle.
	
	char szTmp [MAXCONTLEN] ;
	memset (szTmp,'\0',sizeof(szTmp)) ;
	
	char szBuff [20] ;
	memset (szBuff,'\0',sizeof(szTmp)) ; 		
	p_fd =-1 ;	

	p_fd  = socket (PF_INET,SOCK_STREAM,0) ;
	if (p_fd==-1){	
		sprintf (szTmp,"[%s-line:%d] Can not Create socket handle,pls check your system",__FILE__,__LINE__) ;
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_ERR__) ;
		return  -1 ; 
	}else{
		sprintf (szTmp,"[%s-line:%d] Create Pos Socket handle successfully...") ;
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_ERR__);
	}		
	
	/* Here ,we use the socket as a server deamon [Don't delete these codes]
	int iport =  8090 ; 
	char szIp [30] ;
	memset  (szIp,'\0',sizeof(szIp)) ;

		
	struct sockaddr_in sin ;
	memset  (&sin,'\0',sizeof(struct sockaddr_in*)) ;
	sin.sin_family  = AF_INET ;
	read_config_file ("./sysconfig.cfg","REMOTE_IP",szIp) ;
	sin.sin_addr.s_addr = inet_addr (szIp) ;

	memset (szIp,'\0',sizeof(szIp));	
	read_config_file ("./sysconfig.cfg","REMOTE_PORT",szIp) ;
	iport = atoi (szIp) ;
	sin.sin_port = htons (iport) ;	
	int ret  = bind (p_fd,(struct sockaddr_in*)&sin,sizeof(sin)) ;
	if (ret==-1) {
		memset (szTmp,'\0',sizeof(szTmp)) ;
		sprintf (szTmp,"[%s-line:%d] Can not bind to socket,pls check your system...",__FILE__,__LINE__) ;
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_ERR__) ;
		return  -1 ;
	}
	*/			
	return p_fd ;
}
int Pos_Socket_Conn (int fd)
{
	char szTmp [MAXCONTLEN] ;
	memset (szTmp,'\0',sizeof(szTmp)) ;
	
	if (fd<0){
		sprintf (szTmp,"[%s-line:%d] You have passed an invalid handle,pls check your system...\n") ;
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_ERR__);
		return  -1; 
	}
	
	if (fd!=p_fd)
		p_fd = fd ;
	
	int iConnected  = 0 ;
	struct sockaddr_in sin;
	memset (&sin,'\0',sizeof(sin)) ;		
	char szbuff [30] ;
	memset (szbuff,'\0',sizeof(szbuff));
	sin.sin_family = AF_INET ;
	read_config_file ("./sysconfig.cfg","REMOTE_IP",szbuff) ;

	sin.sin_addr.s_addr = inet_addr (szbuff);	
	memset (szbuff,'\0',sizeof(szbuff)) ;	
	read_config_file ("./sysconfig.cfg","REMOTE_PORT",szbuff) ;
	sin.sin_port = htons  (atoi (szbuff)) ;
	int ret = connect (fd,(struct sockaddr*)&sin,sizeof(sin));
	if (ret==-1){
		memset (szTmp,'\0',sizeof(szTmp)) ;
		sprintf (szTmp,"[%s-line:%d] Connecting to Pos server some errors occur,pls check your system....",__FILE__,__LINE__) ;
		WriteLogFile (szTmp ,__SYS_TYPE_LOGFILE_ERR__) ;
		iConnected  =0 ;	
	}else{
		memset (szTmp,'\0',sizeof(szTmp)) ;
		sprintf ("[%s-line:%d] Connecting to Pos server successfully....",__FILE__,__LINE__) ;
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
		iConnected =1  ;
	}				

	return iConnected  ; 	
}

int Pos_Socket_Send (int fd, const char* szMsg, int iLen)
{
	struct timeval tv ;
	memset (&tv,'\0',sizeof(struct timeval));
	char szbuff [30] ;
	char szTmp [MAXCONTLEN] ;
		
	memset (szbuff,'\0',sizeof(szbuff)) ;
	memset (szTmp,'\0',sizeof(szTmp));
	
	if (fd<0) {//Invalid the socket handle .
		bzero (szTmp,sizeof(szTmp));
		sprintf (szTmp,"[%s-line:%d] You have passed an invalid socket handle,pls check your system...",__FILE__,__LINE__) ;
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;		
	}	
	
	if (!strlen(szMsg)){
		memset (szTmp,'\0',sizeof(szTmp));
		sprintf (szTmp,"[%s-line:%d] you have passed a null message,pls check your sytem...",__FILE__,__LINE__);
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_ERR__) ;
		return  -1; 
	}
	
	read_config_file ("./sysconfig.cfg","TIMEOUT_VAL",szbuff);
	tv.tv_sec=  atoi (szbuff);	
	tv.tv_usec = atoi (szbuff) ;
	fd_set wset ;
	
	FD_ZERO (&wset);
	FD_SET (fd,&wset) ;	
	
	int ret  = select (1024,NULL,&wset,NULL,&tv) ;
	if (ret){
		if (FD_ISSET(fd,&wset)){ //here, we will send the data to Remote Server . 
			int iSize  = write (fd,szMsg,iLen) ;
			if (iSize== iLen) {
				bzero (szTmp,sizeof(szTmp)) ;
				sprintf (szTmp,"Write the Data to remote host successfully...") ;
				WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
				WriteLogFileInHex_Ex (szMsg,__SYS_TYPE_LOGFILE_RAW__) ;
			}else { //an error occurs. The data can not be deliveried successfully...
				bzero (szTmp,sizeof(szTmp)) ;
				sprintf (szTmp,"Write the data to remote host errors occurs,pls check your system...");
				WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
				WriteLogFileInHex_Ex (szMsg,__SYS_TYPE_LOGFILE_RAW__) ;
				return  -1 ; 
			}	
		}
	}else if (ret==-1){ // Exceptions occur
		bzero (szTmp,sizeof(szTmp)) ;
		sprintf (szTmp,"[%s-line:%d] Select() exceptions occur,pls check your system...",__FILE__,__LINE__);	
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;	
	}else{ //Here ,TImeout 
		bzero (szTmp,sizeof(szTmp)) ;
		sprintf (szTmp,"[%s-line:%d] Writing the data time-out occurs,pls chck your system...") ;
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1; 	
	}
	return 0 ;
}
int Pos_Socket_Read (int fd, char* szMsg, const int iLen) 
{
	char szLogBuff [MAXCONTLEN];	
	bzero (szLogBuff,sizeof(szLogBuff)) ;
	char szTmp [30] ;
	bzero (szTmp,sizeof(szTmp)) ;
	
	if (fd<0) {//check the validity of handle of socket.
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d]You just have passed an invalid socket handle,pls chck your system...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;
	} 
	if (szMsg== NULL) { //Invalid memory address.MUST BE CHCK,otherwise will corrupt the system.
		
		return  -1 ;
	}
	
	read_config_file ("./sysconfig.cfg","TIMEOUT_VAL",szTmp) ;
	if (!strlen(szTmp))
		sprintf (szTmp,"%d",3);
	struct timeval tv;
	bzero (&tv,sizeof(struct timeval));
	tv.tv_sec = atoi (szTmp);	
	tv.tv_usec = atoi (szTmp);
	
	fd_set rset ;
	FD_ZERO (&rset);
	FD_SET (fd,&rset);
	
	int iRet  ;
	iRet= select (1024,&rset,NULL,NULL,&tv);
	if (iRet){
		if (FD_ISSET (fd,&rset) ){
			iRet  = read (fd,szMsg,iLen) ;
			if (iRet==iLen){ //Read the msg successfully from remote host.
				bzero (szLogBuff,sizeof(szLogBuff)) ;
				sprintf (szLogBuff,"Read the data from remote server successfully...");
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
				WriteLogFile (szMsg,__SYS_TYPE_LOGFILE_RAW__) ;		
			}else{ //some errors occur...
				bzero (szLogBuff,sizeof(szLogBuff)) ;
				sprintf (szLogBuff,"[%s-line:%d] Some errors occur while reading data from remote svr...",__FILE__,__LINE__);
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
				WriteLogFile (szMsg,__SYS_TYPE_LOGFILE_RAW__) ;		
				return -1 ;
			}
		}	
	}else if (iRet==-1){
		bzero (szLogBuff,sizeof(szLogBuff));
		sprintf (szLogBuff,"[%s-line:%d] Select() an exception occurs...",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
	}else{
		
	}	
	return 0 ;
}

int Pos_Socket_Close(int fd) 
{
	char buffer [MAXCONTLEN] ;
	memset (buffer,'\0',sizeof(buffer)) ;
	
	if (fd<0) { //Don't care the socket handle ,if it's negative vale.
		
		close (fd);
		return  0 ;
	}
	
	if ((shutdown(fd,2)==-1)|| (close (fd)==-1)){
		sprintf (buffer,"[%s-line:%d] Closing the pos socket errours occur....",__FILE__,__LINE__) ;
		WriteLogFile (buffer,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;
	}else {
		sprintf (buffer,"[%s-line:%d] Closing the pos socket successfully...",__FILE__,__LINE__);
		WriteLogFile  (buffer,__SYS_TYPE_LOGFILE_ERR__) ;		

		return 0 ;
	}
}


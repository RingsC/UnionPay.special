/*******************************
 *	Copyright:
 *		All rights resvered by lihao and palm info company .
 *	Purpose :
 *		Create a socket ,start listening socket .All things about communication 
 *		is implemented in this file. 
 *	Author :
 *		lihao
 *	Date:
 *		Oct 8 ,2008
 *		
 *	Change List:
 *		
 *		NONE
 *	Comment:
 *		In the next version, we will revise the system architecture. To fork sub-process 
 *		to monitor socket rather than the current structure.
 *
 *******************************/

#include "../include/usocket.h"
	
static void*  StartSocket_Listen_Thread (void*) ; //Listening thread
static void*  StartBuinessThread (void*) ;	  //Process user request data thread .

static int Process_ReplyKey (int , char *,char *,char *,char *);	 //Apply kery 
static int Process_MerchantSignIn(int,char* ,char* ,char* ,char* ); 	//Merchant Sign in 
static int Process_BalanceQuery (int,char *,char* ,char*,char* ); 	//Amount Balance Query.
static int Process_TransDetail (int ,char*,char* ,char* ,char* ) ;	//TransDetails information 
static int Process_Payment (int ,char*,char*,char*,char*);		//Do payment Actions.
static int Process_ChangePwd (int,char *,char*,char*,char*);		//Change Business Passowrd
static int Process_NetManage (int , char* ,char* ,char* ,char*);
static int Process_Rollback (int,char*,char*,char*,char*);		//Roball record .
int StartSocket_Listen (char* szListenIP , int iListenPort, int iSocket_Type ,int* hSocket)
{

	//Here ,we will not use these codes again.We get the settings from config file
	//Because of the future using ,I do not remove these juke codes from this function.		
	char szTmp [MAXCONTLEN] ;
	
	LISTEN_SKT_INFO  skt_info ;
	memset (&skt_info , 0x0 ,sizeof(skt_info)) ;
	strncpy (skt_info.szListenIP ,szListenIP ,strlen (skt_info.szListenIP)); 	
	skt_info.iListenPort = iListenPort ;
	skt_info.iTimeOutValue = 0 ;

	int ret ;
	ret  = pthread_create (&gb_ListenThread , NULL ,&StartSocket_Listen_Thread,(void*) &skt_info);
	if (ret!=0)  {
		memset (szTmp,'\0',sizeof(szTmp)) ;
		sprintf  (szTmp,"[%s-line:%d]Create Listening Thread Faild,Pls Check your system...",__FILE__,__LINE__);
		return  -1 ; 
	}
	//printf ("It will create listening thread...\n") ;
	//sleep (1) ;
	
	return  0 ;
} 

static void* StartSocket_Listen_Thread (void* strThreadParam)
{
	char szTmp [MAXCONTLEN];

	memset (szTmp,'\0',sizeof(szTmp)) ;
	sprintf (szTmp,"Begin Listening thread...");
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
		
	LISTEN_SKT_INFO skt_info ;
	char szListenIP [20] ;
	int  iListenPort =-1 ;
	int  iSocket_Type  =-1;	
	int  iTimeOut  = 0 ;
	
	char szTmpBuff [20] ;
	memset (&skt_info ,'\0', sizeof(LISTEN_SKT_INFO));
	memset (szListenIP ,'\0',sizeof(szListenIP));	
	memset (szTmpBuff,'\0',sizeof(szTmpBuff));	
	
	
	/* Here ,we read the settings from configuration file.don't delete these comments *** Important ***
	if (strThreadParam== NULL || 0== sizeof((struct LISTEN_SKT_INFO*)strThreadParam)) {
		//Set the default Socket params .		
	} else{
		skt_info  = *((LISTEN_SKT_INFO*) strThreadParam) ;	
		//memcpy (&skt_info ,strThreadParam,sizeof(LISTEN_SKT_INFO));
		strncpy (szListenIP ,skt_info.szListenIP ,strlen(skt_info.szListenIP));
		iListenPort  = skt_info.iListenPort ;
		iTimeOut = skt_info.iTimeOutValue ;
	}
				
	printf ("The params of socket.[IP:%s][Port:%d][TimeOutValue:%d]\n",szListenIP,iListenPort,iTimeOut);

	if (!strlen(szListenIP)||NULL == szListenIP) { //verifying the valildity of IP address & port  .
		return ;
	}
	*/
	read_config_file ("./sysconfig.cfg","LISTEN_IP",szTmpBuff);
	if (strlen(szTmpBuff))	
		strcpy (szListenIP ,szTmpBuff);
	else
		strcpy (szListenIP,"127.0.0.1");	//Default Listening IP 
	
	memset  (szTmpBuff,0x0,sizeof(szTmpBuff));
	read_config_file ("./sysconfig.cfg","LISTEN_PORT",szTmpBuff);
	if (strlen(szTmpBuff))	
		iListenPort =atoi(szTmpBuff);
	else
		iListenPort = 8080 ;			//Default Listen Port
	
	memset (szTmpBuff,0x0 ,sizeof(szTmpBuff));	
	read_config_file ("./sysconfig.cfg","TIMEOUT_VAL",szTmpBuff);
	if (strlen(szTmpBuff))
		iTimeOut = atoi (szTmpBuff);
	else
		iTimeOut = 3 ;			//Default Timeout Value.

	memset (szTmp,'\0',sizeof(szTmp));	
	sprintf (szTmp,"The params of socket.[IP:%s][Port:%d][TimeOutValue:%d]",szListenIP,iListenPort,iTimeOut);
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__);
	int h_socket= socket (PF_INET, SOCK_STREAM , 0);
	if (-1 == h_socket) {
		
		 memset (szTmp,'\0',sizeof(szTmp));
		 sprintf (szTmp,"[%s-line:%d] can not create socket handle",__FILE__,__LINE__) ;
		 WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
		 return  ;
	}
	
	 memset (szTmp,'\0',sizeof(szTmp));
	 sprintf  (szTmp,"Create the socket successfully...") ;	
	 WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
	
	struct sockaddr_in  serveraddr;
	struct sockaddr_in  csin_addr ;
	
	memset (&serveraddr ,0x0,sizeof(struct sockaddr_in*));
	memset (&csin_addr , 0x0 ,sizeof(struct sockaddr_in* ));
	
	serveraddr.sin_family = AF_INET ;
	serveraddr.sin_addr.s_addr = inet_addr (szListenIP) ;
	serveraddr.sin_port  = htons ((short)iListenPort);
	
	if(-1 == bind(h_socket,(struct sockaddr*)&serveraddr ,sizeof(serveraddr))){
		memset (szTmp,'\0',sizeof(szTmp)) ;
		sprintf (szTmp,"[%s-line:%d] can not bind to socket,pls check system",__FILE__,__LINE__);
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
		return  ;
	}
		
	memset (szTmp,'\0',sizeof(szTmp)) ;
	sprintf  (szTmp,"Bind the socket successfully,then it will start listening..."); 	
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
	
	if  (listen (h_socket,1024) <0 ){ //here ,we can accept the maximum handle : 1024 	
		memset (szTmp,'\0',sizeof(szTmp)) ;
		sprintf (szTmp,"[%s-line:%d] listen port error,pls check your system",__FILE__,__LINE__);
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
		close (h_socket);
		return  ;
	}
	
	memset (szTmp,'\0',sizeof(szTmp)) ;
	sprintf  (szTmp,"Start Listeining successfully...") ;	
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;	
	//hSocket = &h_socket ;	
	
	int iRecvedSkt_hd = -1 ;
	socklen_t len = sizeof(struct sockaddr);		
	memset (szTmp,'\0',sizeof(szTmp)) ;
	sprintf  (szTmp,"Waitting for the client connections....");	
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__);	

	pthread_t accptedThread ;
	int ret ;
	struct hostnet* hostptr = NULL  ;
	
	char hostname [20] ;
	memset (hostname,0x0,sizeof(hostname));
		
	struct timeval tm;
	memset (&tm,0x0,sizeof(struct timeval));
	tm.tv_sec = 0; 
	tm.tv_usec = 0;	
	
	fd_set readset ;
	FD_ZERO (&readset) ;	
	FD_SET (h_socket,&readset); //h_socket is listening socket.
	
	while  (gb_ListeningFlag){
		
		//here ,we use select to determine ,which socket is readable ,then accept the readable socket 
		//handle .
		int iRet = -1; 
		//iRet  = select(2,&readset,NULL,NULL ,&tm);
		iRet  = select(1024,&readset,NULL,NULL ,NULL);
		if (iRet){
			if (!FD_ISSET(h_socket,&readset)) continue ;
			memset (szTmp,'\0',sizeof(szTmp));
			sprintf (szTmp,"New connection is arrival...");
			WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
			
			iRecvedSkt_hd = accept  (h_socket,(struct sockaddr*)&csin_addr,&len ) ;
                	if (iRecvedSkt_hd ==-1 ){
				memset (szTmp,'\0',sizeof(szTmp)) ;
                        	sprintf (szTmp,"[%s-line:%d] Can not accept the remote connection....",__FILE__,__LINE__);
				WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
                       		continue ;
               		}
			
		        hostptr = (struct hostnet*)gethostbyaddr ((const char*)&(csin_addr.sin_addr.s_addr),(socklen_t)4,AF_INET) ;
                	if (hostptr== NULL ){
				memset (szTmp,'\0',sizeof(szTmp)) ;
                       		sprintf (szTmp,"[%s-line:%d] Unknown host",__FILE__,__LINE__);
				WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__);
				
               		 }else{
				memset (szTmp,'\0',sizeof(szTmp)) ;
                       		strcpy (hostname ,inet_ntoa(csin_addr.sin_addr));
				sprintf (szTmp,"Recv Connection from [%s]",hostname);
				memset (hostname,'\0',sizeof(hostname)) ;
				WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
                       	}
		        ret  = pthread_create (&accptedThread, NULL ,StartBuinessThread , (void*)&iRecvedSkt_hd);
                	memset (szTmp,'\0',sizeof(szTmp));
			if (ret!=0) {
                       		sprintf (szTmp,"Create buiness processing thread failed,pls check your system...");
				WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;                          
                       		continue ;
                	}else {
                		sprintf (szTmp,"[%s-line:%d] accept remote socket connetion [handle:%d]",__FILE__,__LINE__,iRecvedSkt_hd);
				WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
			}
		}else if (iRet==-1){
			memset (szTmp,'\0',sizeof(szTmp)) ;
			sprintf (szTmp,"[%s-line:%d] Select() Eorror....",__FILE__,__LINE__);
			WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
			return ;
		}else{
			//printf ("There is no connection from remote host\n") ;
		}
	}
	
	memset (szTmp,'\0',sizeof(szTmp)) ;	
	sprintf (szTmp,"User has terminated the listening socket...") ;
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
		
	shutdown (h_socket,2) ; // Don't send the data which are stored in sending-buffer of socket...
	close (h_socket);
	pthread_exit(NULL) ;
	return  ;
}
	
static void* StartBuinessThread (void* strThreadParams)
{// Processing the user request data.
	char szTmp [MAXCONTLEN] ;
	
	if  (strThreadParams==NULL){
		memset (szTmp,'\0',sizeof(szTmp)) ;
		sprintf (szTmp,"[%s-line:%d] You have pass a invalid pointer to thread ,pls check your system...",__FILE__,__LINE__);
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__);	
		return ;
	}
	
	int sfd ;		
	memcpy (&sfd,strThreadParams,sizeof(int));	
	memset (szTmp,'\0',sizeof(szTmp)) ;
	sprintf (szTmp,"Begin the buiness processing thread [handle:%d]...",sfd) ;
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
		
	//Here ,we will cope with the request data
	char szTmpBuff [30] ;
	memset (szTmpBuff,'\0',sizeof(szTmpBuff));
	read_config_file ("./sysconfig.cfg","TIMEOUT_VAL",szTmpBuff);
	if (!strlen(szTmpBuff))		
		sprintf (szTmpBuff,"%d",30000);
	struct timeval tv ;
	memset (&tv,0x0,sizeof(struct timeval));
	tv.tv_sec = atoi (szTmpBuff) ;
	tv.tv_usec = atoi (szTmpBuff);	
	fd_set rset ,wset ;
	
	FD_ZERO (&rset);
	FD_ZERO (&wset);	
	FD_SET (sfd,&rset);	//To monitor sfd ,which is used to read data .
	

	CMD_MSG_HEAD msg_head ;
	memset (&msg_head,'\0',sizeof(CMD_MSG_HEAD));
	
	int ret =-1;
	ret = select (1024,&rset,NULL,NULL,&tv);
	if (ret){ 
		if (FD_ISSET(sfd,&rset)){
			memset (szTmp,'\0',sizeof(szTmp)) ;
			sprintf (szTmp,"Readable handle is ready...");
			WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__);
		}
		ret = read (sfd,&msg_head,sizeof (CMD_MSG_HEAD));
		WriteLogFileInHex_Ex (&msg_head,__SYS_TYPE_LOGFILE_RAW__) ; //Write the raw data into logfile.
	
		if (ret!=sizeof(CMD_MSG_HEAD)){
			memset (szTmp,'\0',sizeof(szTmp)) ;	
			sprintf (szTmp,"[%s-line:%d]Read data errors occur,pls check your system",__FILE__,__LINE__);
			WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
			close(sfd);
			return  ; 
		}else{
			memset (szTmp,'\0',sizeof(szTmp)) ;
			sprintf (szTmp,"Recvd data from remote host:[serial]%s;[Cmd]%s;[Tm]%s;[FollowLen]%s",
					msg_head.szSerialID,msg_head.szCommandID,msg_head.szTimeStamp,msg_head.szFollowLen);
			WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
			switch (atoi(msg_head.szCommandID)){
				case 820 :{ //_PUB_CMD_REAPPLYKEY_REQ_
					Process_ReplyKey (sfd,msg_head.szSerialID,msg_head.szCommandID,msg_head.szTimeStamp,msg_head.szFollowLen) ;
				
				} break;
				case 830 :{
					Process_MerchantSignIn(sfd,msg_head.szSerialID,msg_head.szCommandID,msg_head.szTimeStamp,msg_head.szFollowLen);
				} break; 
				case 200 :{
					Process_BalanceQuery (sfd,msg_head.szSerialID,msg_head.szCommandID,msg_head.szTimeStamp,msg_head.szFollowLen);
		
				} break;
				case 300 :
				{
					Process_TransDetail (sfd,msg_head.szSerialID,msg_head.szCommandID,msg_head.szTimeStamp,msg_head.szFollowLen) ;

				} break;
				case 400 :{
					Process_Payment(sfd,msg_head.szSerialID,msg_head.szCommandID,msg_head.szTimeStamp,msg_head.szFollowLen);
				} break; 
				case 500 :{
					Process_ChangePwd (sfd,msg_head.szSerialID,msg_head.szCommandID,msg_head.szTimeStamp,msg_head.szFollowLen);	
				
				} break; 
				
				case 600 :{
					Process_NetManage (sfd,msg_head.szSerialID,msg_head.szCommandID,msg_head.szTimeStamp,msg_head.szFollowLen);
				
				} break;
				
				case 700 :{
					Process_Rollback (sfd,msg_head.szSerialID,msg_head.szCommandID,msg_head.szTimeStamp,msg_head.szFollowLen);
				} break;
				
				default:break ;
			}
					
		}				

	}else if(ret==-1){
		memset (szTmp,'\0',sizeof(szTmp));
		sprintf (szTmp,"[%s-line:%d] Exe Select() function error occurs...",__FILE__,__LINE__);
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__);
		return  ;
	}
	else{//Timeout
		bzero (szTmp,sizeof(szTmp)); 
		sprintf (szTmp,"[%s-line:%d] Can not read data from remote host for Waiting data timeout",__FILE__,__LINE__);
		WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
	}
	shutdown (sfd,SHUT_RDWR) ;	
	close (sfd);	//After , we receive the data ,close the socket handle .
	//printf ("Recv data from remote host;[Data]:%s\n",szBuffer);
	pthread_exit (NULL);
	return ;
}
static int Process_ReplyKey (int fd ,char *szSerialID,char *szCommandID,char *szTimeStap,char *szFollowLen) 
{
	if (fd<0) return  -1 ; 
	char szBuffer [30] ;
	memset (szBuffer,0x0,sizeof(szBuffer));
	
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff)) ;	

	read_config_file("./sysconfig.cfg","TIMEOUT_VAL",szBuffer) ;
	struct timeval tv ;
	memset (&tv,0x0,sizeof(struct timeval));
	tv.tv_sec = atoi(szBuffer) ;
	tv.tv_usec = atoi (szBuffer);
	
	fd_set rset  ;		//Set file descriptor monitor
	FD_ZERO (&rset);
	FD_SET (fd ,&rset);

	
	CMD_REAPPLYKEY_REQ aply_key_req ;
	memset (&aply_key_req,0x0,sizeof(CMD_REAPPLYKEY_REQ));
	
	int ret =-1 ;
	ret = select ( 1024 , &rset ,NULL , NULL , &tv);	
	if (ret){
		if (read (fd ,&aply_key_req,sizeof(CMD_REAPPLYKEY_REQ))!=sizeof(CMD_REAPPLYKEY_REQ)){
			bzero (szLogBuff,sizeof(szLogBuff)) ;
			sprintf (szLogBuff,"[%s-line:%d] Recving Msg Body some errors occur,pls check your system",__FILE__,__LINE__);
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFileInHex_Ex (&aply_key_req,__SYS_TYPE_LOGFILE_RAW__);
			//Sending the error response message.
			return -1 ;
		}
		WriteLogFileInHex_Ex (&aply_key_req,__SYS_TYPE_LOGFILE_RAW__) ;
		bzero (szLogBuff,sizeof(szLogBuff));
		sprintf (szLogBuff,"Recvd Data from remote host...");
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
		//Send the request to POS mainframe .
		//
		//	NOt FInished  .
		//
		//
		//
		//Recv the reposne msg from pos ,then send response to remote host.		
		
		CMD_MSG_HEAD res_head ;	
		memset (&res_head,0x0,sizeof(CMD_MSG_HEAD));
		strncpy (res_head.szSerialID ,szSerialID ,strlen(res_head.szSerialID));
		strncpy (res_head.szCommandID ,_PUB_CMD_REAPPLYKEY_RES_, strlen(res_head.szCommandID));
		char szTmp [20] ;
		memset (szTmp,0x0,sizeof(szTmp));
		GetTimeStamp (szTmp);
		strncpy (res_head.szTimeStamp ,szTmp,sizeof(res_head.szTimeStamp));
		memset (szTmp,0x0,sizeof(szTmp));
		
		sprintf (szTmp,"%d",sizeof(CMD_REAPPLYKEY_RES));
		strncpy (res_head.szFollowLen,szTmp,sizeof(res_head.szFollowLen));	
		
		CMD_REAPPLYKEY_RES aply_key_res;
		memset (&aply_key_res, 0x0 ,sizeof(CMD_REAPPLYKEY_RES));
		
		//Here ,we will use the true answer from POS 
		strncpy (aply_key_res.szTransDateTime,"20080909",sizeof(aply_key_res.szTransDateTime));
		strncpy (aply_key_res.szTraceNo ,"123456",sizeof(aply_key_res.szTraceNo));
		strncpy (aply_key_res.szSenderID ,"1234567890",sizeof(aply_key_res.szSenderID)) ;
		strncpy (aply_key_res.szResponseCode,"00",sizeof(aply_key_res.szResponseCode));
		
		int sentLen = sizeof(CMD_MSG_HEAD)+ sizeof(CMD_REAPPLYKEY_RES);
		if ((ret=write (fd,&res_head,sizeof(CMD_MSG_HEAD)))!=sizeof(CMD_MSG_HEAD)){
			bzero (szLogBuff,sizeof(szLogBuff));
			sprintf (szLogBuff,"[%s-line:%d] Sending the reponse msg head errors occur....",__FILE__,__LINE__);
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
			WriteLogFileInHex_Ex (&res_head,__SYS_TYPE_LOGFILE_RAW__) ;	
			return  -1 ; 
		}else{
			if ((ret=write (fd,&aply_key_res,sizeof(CMD_REAPPLYKEY_RES)))!=sizeof(CMD_REAPPLYKEY_RES)){
				bzero (szLogBuff,sizeof(szLogBuff));
				sprintf (szLogBuff,"[%s-line:%d] Sending the reponse msg body errors occur...",__FILE__,__LINE__);
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
				WriteLogFileInHex_Ex (&aply_key_res,__SYS_TYPE_LOGFILE_RAW__) ;
				return  -1 ;
			}else{
				bzero (szLogBuff,sizeof(szLogBuff));
				sprintf (szLogBuff,"Sending the response to remote host successfully...");
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
				WriteLogFileInHex_Ex (&aply_key_res,__SYS_TYPE_LOGFILE_RAW__);
			}
		}		
		
	}else if (ret==-1){
		bzero (szLogBuff,sizeof(szLogBuff));
		sprintf (szLogBuff,"[%s-line:%d] Select() error...",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
		return  -1 ;
	}else{
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] Recv following data timeout,pls check your system",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
	}
	return  0;
}

static int Process_MerchantSignIn(int fd,char *szSerialID,char *szCommandID,char* szTimeStamp,char* szFollowLen)
{
	if (fd<0) return -1 ;
	char szBuff [30] ;
	memset (szBuff,0x0,sizeof(szBuff)) ;
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff));
	
	
	read_config_file ("./sysconfig.cfg","TIMEOUT_VAL",szBuff) ;
	if (!strlen(szBuff))
		sprintf (szBuff,"%d",3);	
	struct timeval tv ;
	memset (&tv,0x0,sizeof(struct timeval));
	tv.tv_sec = atoi (szBuff);
	tv.tv_usec = atoi (szBuff) ;
	
	fd_set rset ;
	FD_ZERO (&rset)	;
	FD_SET (fd,&rset) ;
		
	CMD_MERCHANTSIGNIN_REQ merchant_req ;
	memset (&merchant_req,'\0',sizeof(CMD_MERCHANTSIGNIN_REQ)) ;
	int ret  =-1 ;
	ret  = select (1024 , &rset ,NULL, NULL , &tv) ;
	if ( ret ) {
		if ((ret =read (fd,&merchant_req,sizeof(CMD_MERCHANTSIGNIN_REQ)))!= sizeof(CMD_MERCHANTSIGNIN_REQ)){
			bzero (szLogBuff,sizeof(szLogBuff));
			sprintf (szLogBuff,"Read Merchant signin request body errors occur...");
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFileInHex_Ex (&merchant_req,__SYS_TYPE_LOGFILE_RAW__);
			return  -1 ;
		}else{ //Do somethings.
				
			CMD_MSG_HEAD res_head ;
			CMD_MERCHANTSIGNIN_RES merchant_res ;
			memset (&res_head,0x0,sizeof(CMD_MSG_HEAD)) ;
			memset (&merchant_res , 0x0,sizeof(CMD_MERCHANTSIGNIN_RES));
			
			if (Pos_SendMsg_CheckIn_ToBank (&merchant_req ,&merchant_res, NULL,NULL, NULL ) != 0 ){ //sendto 
				bzero (szLogBuff,sizeof(szLogBuff)); 
				sprintf (szLogBuff,"[%s-line:%d] Can not get the correct answer from pos, pls check your sysm... ",__FILE__,__LINE__ );
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
				WriteLogFileInHex_Ex (merchant_res,__SYS_TYPE_LOGFILE_RAW__) ;	
				return -1 ;
				
			} 
			//Assemble the response's head.
			strncpy (res_head.szSerialID ,szSerialID,sizeof(res_head.szSerialID));	//SerialID of Response
			strncpy (res_head.szCommandID , _PUB_CMD_MERCHANTSIGNIN_RES_,sizeof(res_head.szCommandID)) ; //CommandID Of Response
			char szTmp [30] ;
			memset (szTmp ,0x0,sizeof(szTmp)) ;
			GetTimeStamp (szTmp) ;
			strncpy (res_head.szTimeStamp ,szTmp, sizeof(res_head.szTimeStamp)); //TimeStamp of Response	
			memset (szTmp ,0x0,sizeof(szTmp)) ;
			sprintf(szTmp,"%d",sizeof(CMD_MERCHANTSIGNIN_RES)) ; //Following Len of Response
			strncpy (res_head.szFollowLen,szTmp,sizeof(res_head.szFollowLen)) ;
				
			
			if ((ret= write (fd,&res_head,sizeof(CMD_MSG_HEAD)))==sizeof(CMD_MSG_HEAD)){
				bzero (szLogBuff,sizeof(szLogBuff));
				sprintf (szLogBuff,"Sending the head of Response SUccessfully, it will send Body...") ;
				WriteLogFile(szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
				WriteLogFileInHex_Ex (&res_head,__SYS_TYPE_LOGFILE_RAW__) ;
				
				if ((ret= write (fd,&merchant_res,sizeof(CMD_MERCHANTSIGNIN_RES)))==sizeof(CMD_MERCHANTSIGNIN_RES)){
					bzero (szLogBuff,sizeof(szLogBuff));
					sprintf (szLogBuff,"Sending the body of response successfully...") ;
					WriteLogFile (szLogBuff, __SYS_TYPE_LOGFILE_RAW__) ;
					WriteLogFileInHex_Ex (&merchant_res,__SYS_TYPE_LOGFILE_RAW__); 	
					
				}else{
					bzero (szLogBuff, sizeof(szLogBuff)) ;
					sprintf (szLogBuff,"[%s-line:%d] Sending the body of response errors ouccr...",__FILE__,__LINE__) ;
					WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
					WriteLogFileInHex_Ex (&merchant_res,__SYS_TYPE_LOGFILE_RAW__);
					return  -1 ;
				}	
			}else{
				bzero (szLogBuff,sizeof(szLogBuff));
				sprintf (szLogBuff,"[%s-line:%d]Sending the Head of Response errors occur...",__FILE__,__LINE__);
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
				WriteLogFileInHex_Ex (&res_head,__SYS_TYPE_LOGFILE_RAW__);	
				return -1 ;
			}
		}
	}else if (ret==-1) {
		bzero (szLogBuff,sizeof(szLogBuff));
		sprintf (szLogBuff,"[%s-line:%d] Select() Error...",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return  -1; 
	}else{
		bzero (szLogBuff,sizeof(szLogBuff));
		sprintf (szLogBuff,"Recving Request Msg Body Timeout occurs...") ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ; 
	}	
	return 0 ;		
}


static int Process_BalanceQuery (int fd,char *szSerialID,char *szCommandID,char* szTimeStamp,char *szFollowLen){
	if (fd<0) return  -1 ; 
	char szTmp [30] ;
	memset (szTmp ,0x0,sizeof(szTmp));
		
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff));
	
	read_config_file ("./sysconfig.cfg","TIMEOUT_VAL",szTmp);
	if (!strlen(szTmp))	
		sprintf (szTmp,"%d",30000) ;
		
	struct timeval tv;
	memset (&tv,0x0,sizeof(struct timeval));
	tv.tv_sec =  atoi (szTmp) ;
	tv.tv_usec = atoi (szTmp) ;	
	
	int ret = -1 ;
	fd_set rset ;
	FD_ZERO (&rset);
	FD_SET (fd,&rset) ;
	
	ret = select ( 1024 , &rset,NULL,NULL,&tv);
	if (ret){
		CMD_BALANCEAMOUNTQURY_REQ balance_req; 
		memset (&balance_req,0x0 ,sizeof(CMD_BALANCEAMOUNTQURY_REQ));
		if ((ret = read  (fd,&balance_req,sizeof(CMD_BALANCEAMOUNTQURY_REQ)))==sizeof(CMD_BALANCEAMOUNTQURY_REQ)){
			bzero (szLogBuff,sizeof(szLogBuff));
			sprintf (szLogBuff,"Reading head of request successfully...");
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__); //Write the logfile into file
			WriteLogFileInHex_Ex (&balance_req,__SYS_TYPE_LOGFILE_RAW__);
			//Here ,assemble request data to pos
			
			//Assemble the head of response .
			CMD_MSG_HEAD msg_head;
			memset(&msg_head,0x0,sizeof(CMD_MSG_HEAD)) ;
			strncpy (msg_head.szSerialID,szSerialID,sizeof(msg_head.szSerialID)) ;
			strcpy (msg_head.szCommandID,_PUB_CMD_BALANCEAMOUNTQURY_RES_);
			memset (szTmp ,0x0,sizeof(szTmp));
			GetTimeStamp (szTmp);
			strncpy (msg_head.szTimeStamp,szTmp,sizeof(msg_head.szTimeStamp));
			memset(szTmp,0x0 ,sizeof(szTmp));
			sprintf (szTmp,"%d",sizeof(CMD_BALANCEAMOUNTQURY_RES));  	
			strncpy (msg_head.szFollowLen,szTmp,sizeof(msg_head.szFollowLen));
			
			//Assemble the body of response.	
			CMD_BALANCEAMOUNTQURY_RES balance_res; 
			memset (&balance_res,0x0 ,sizeof(CMD_BALANCEAMOUNTQURY_RES));
			
			
			if ( Pos_SendMsg_QueryAmount_ToBank (&balance_req ,&balance_res, NULL,NULL, NULL ) != 0 ){ //sendto 			
				bzero (szLogBuff,sizeof(szLogBuff)) ;
				sprintf (szLogBuff,"can not parse the receved string correctlly,pls check your sytem...") ;	
				WriteLogFile (szLogBuff, __SYS_TYPE_LOGFILE_RAW__ ) ;
				return -1 ;
			}			
			if((ret = write (fd,&msg_head,sizeof(CMD_MSG_HEAD)))==sizeof(CMD_MSG_HEAD)){
				bzero (szLogBuff,sizeof(szLogBuff));
				sprintf (szLogBuff,"Sending the head of Reponse successfully...");
				WriteLogFile (szLogBuff , __SYS_TYPE_LOGFILE_RAW__) ;
				WriteLogFileInHex_Ex (&msg_head , __SYS_TYPE_LOGFILE_RAW__) ;
					
				if((ret  = write (fd,&balance_res,sizeof(CMD_BALANCEAMOUNTQURY_RES)))==sizeof(CMD_BALANCEAMOUNTQURY_RES)){
					bzero (szLogBuff,sizeof(szLogBuff));
					sprintf (szLogBuff,"Sending the body of Response successfully...");
					WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;	
					WriteLogFileInHex_Ex (&balance_res,__SYS_TYPE_LOGFILE_RAW__) ;
				}		
						
			}else{
				bzero (szLogBuff,sizeof(szLogBuff)) ;
				sprintf (szLogBuff,"[%s-line:%d] Sending the head of reponse errors occur...",__FILE__,__LINE__);
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
				WriteLogFileInHex_Ex (&msg_head,__SYS_TYPE_LOGFILE_RAW__);	
				return -1 ;
			}	
			
		}else{ 
			bzero (szLogBuff,sizeof(szLogBuff)) ;
			sprintf (szLogBuff,"[%s-line:%d] Can not read head of request from remote host...",__FILE__,__LINE__);
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFileInHex_Ex (&balance_req,__SYS_TYPE_LOGFILE_RAW__) ;
			return  -1 ;
		}
	}else if (ret==-1){
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] select() Error ...",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ; 
	}else{
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf ("[%s-line:%d] Waiting from data from remote host timeout...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1;	
	}	
	return 0; 
}

		

static int Process_TransDetail (int fd,char *szSerialID,char *szCommandID,char* szTimeStamp,char*szFollowLen)
{
	if (fd<0) return -1 ;
	
	
	char szBuff [30] ;
	memset  (szBuff,0x0 ,sizeof(szBuff));
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff));
		
	read_config_file ("./sysconfig.cfg","TIMEOUT_VAL",szBuff) ;
	if (!strlen(szBuff))
		sprintf (szBuff,"%d",3) ;
	
	struct timeval tv ;
	memset (&tv,0x0,sizeof(struct timeval)) ;
	tv.tv_sec = atoi (szBuff) ;
	tv.tv_usec = atoi (szBuff) ;
	
	fd_set rset ;
	FD_ZERO (&rset) ;			
	FD_SET (fd,&rset);
	
	int ret = -1 ;
	ret  = select (1024 , &rset,NULL ,NULL ,&tv) ;
	if (ret){
		CMD_TRANSDETAILSQURY_REQ trans_detail_req ;
		memset (&trans_detail_req,0x0,sizeof(CMD_TRANSDETAILSQURY_REQ)) ;
		if((ret = read (fd,&trans_detail_req,sizeof(CMD_TRANSDETAILSQURY_REQ)))==sizeof(CMD_TRANSDETAILSQURY_REQ)){
			bzero (szLogBuff,sizeof(szLogBuff)) ;
			sprintf (szLogBuff,"Reading body of request successfully...");
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
	
			WriteLogFileInHex_Ex (&trans_detail_req,__SYS_TYPE_LOGFILE_RAW__) ;
			//Do somethings, Send the request to pos ,then get response.
			//
			//
			//
			////////////////////////////////////////////////////////
				
			CMD_MSG_HEAD res_head;
			memset (&res_head,0x0 ,sizeof(CMD_MSG_HEAD)) ;
			TRANSDETAILITEM trans_item ;
			memset(&trans_item ,0x0 ,sizeof(TRANSDETAILITEM)) ;
			CMD_TRANSDETAILSQURY_RES trans_detail_res ;
			memset (&trans_detail_res,0x0,sizeof(CMD_TRANSDETAILSQURY_RES)) ;
			
			//Assemble head message.
			
					
			
		} else {
			bzero (szLogBuff,sizeof(szLogBuff)) ;
			sprintf (szLogBuff,"[%s-line:%d] Reading body of request errors occur...",__FILE__,__LINE__) ;
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFileInHex_Ex (&trans_detail_req,__SYS_TYPE_LOGFILE_RAW__) ;
			return  -1 ;
		}		
	}else if(ret==-1){
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] Select() Error...",__FILE__,__LINE__); 
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return  -1 ;
	} else{
		bzero (szLogBuff,sizeof(szLogBuff));
		sprintf (szLogBuff,"[%s-line:%d] Waiting for data from remote data timeout ocurrs...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;
	}		
		
	return 0 ;
}

static int  Process_Payment(int fd, char *szSerialID,char *szCommandID,char *szTimeStamp,char *szFollowlen)
{
	if (fd<0) return  -1 ;
	char buffer[30];
	memset  (buffer,0x0,sizeof(buffer));

	char szLogBuff [MAXCONTLEN];	
	bzero (szLogBuff,sizeof(szLogBuff));
	
	read_config_file ("./sysconfig.cfg","TIMEOUT_VAL",buffer) ;
	if (!strlen(buffer))
		sprintf (buffer ,"%d",3);
	
	struct timeval tv ;
	memset (&tv,0x0,sizeof(struct timeval)) ;
	tv.tv_sec = atoi (buffer);
	tv.tv_usec = atoi (buffer);
	
	
	fd_set rset ;
	FD_ZERO (&rset);
	FD_SET (fd,&rset) ;
	
	int ret  =-1 ;
	ret  = select (1024,&rset , NULL,NULL ,&tv);
	if (ret){
		
		CMD_PAYMENT_REQ pay_req ;
		memset (&pay_req,0x0,sizeof(CMD_PAYMENT_REQ));
		
		if((ret  = read (fd , &pay_req , sizeof(CMD_PAYMENT_REQ))) == sizeof(CMD_PAYMENT_REQ)){
			bzero (szLogBuff,sizeof(szLogBuff));
			sprintf (szLogBuff,"Recv the pay_req from remote host sucessfully...") ;
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
			WriteLogFileInHex_Ex (&pay_req,__SYS_TYPE_LOGFILE_RAW__) ;
			////////////////do somethings for the request then send reponse message to remote 
			//
			//
			//
			//
			//
			//
			//
			//////////////////////////////////////////////////////////////////////////////////
			CMD_MSG_HEAD cmd_res_head; 
			memset (&cmd_res_head,0x0,sizeof(CMD_MSG_HEAD));
			CMD_PAYMENT_RES pay_res ;
			memset (&pay_res,0x0,sizeof(CMD_PAYMENT_RES));
		
			//Here, I use thread-safe strncpy to replace strcpy 
			strncpy (cmd_res_head.szSerialID,szSerialID,strlen(szSerialID));
			strncpy (cmd_res_head.szCommandID,_PUB_CMD_PAYMENT_RES_,strlen(cmd_res_head.szCommandID));
			memset (buffer,0x0,sizeof(buffer)); 
			GetTimeStamp (buffer);
			strncpy (cmd_res_head.szTimeStamp,buffer,strlen(cmd_res_head.szTimeStamp));
			memset (buffer,0x0,sizeof(buffer));
			sprintf (buffer,"%d",sizeof(CMD_PAYMENT_RES)) ;
			strncpy (cmd_res_head.szFollowLen,buffer,strlen(buffer));
			
			
			strncpy (pay_res.szPaymentAmount,"payment_amout",sizeof(pay_res.szPaymentAmount));			       strncpy (pay_res.szSysTraceNo,"trace_no",sizeof(pay_res.szSysTraceNo));	
			strncpy (pay_res.szLocalTransTime,"localTime",sizeof(pay_res.szLocalTransTime));			       strncpy (pay_res.szLocalTransDate,"localDate",sizeof(pay_res.szLocalTransDate));				      strncpy (pay_res.szSettlementDate,"settle",sizeof(pay_res.szSettlementDate));
			strncpy (pay_res.szMerchantType,"type",sizeof(pay_res.szMerchantType));
			strncpy (pay_res.szAcquireInstID,"instId",sizeof(pay_res.szAcquireInstID));
			strncpy (pay_res.szSenderInstID,"instID_S",sizeof(pay_res.szSenderInstID));
			strncpy (pay_res.szResponseCode,"00",sizeof(pay_res.szResponseCode));
			strncpy (pay_res.szTerminalID,"term",sizeof(pay_res.szTerminalID));
			strncpy (pay_res.szMerchantID,"merchant",sizeof(pay_res.szMerchantID));
			
			if ((ret = write (fd,&cmd_res_head,sizeof(CMD_MSG_HEAD)))==sizeof(CMD_MSG_HEAD)){
				bzero (szLogBuff,sizeof(szLogBuff)) ;
				sprintf (szLogBuff,"Send the response Msg Head successfully...");
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
				WriteLogFileInHex_Ex (&cmd_res_head,__SYS_TYPE_LOGFILE_RAW__) ;
				
				if ((ret= write (fd,&pay_res,sizeof(CMD_PAYMENT_RES)))==sizeof(CMD_PAYMENT_RES)){
						bzero (szLogBuff,sizeof(szLogBuff)) ;
						sprintf (szLogBuff,"Send the Reponse Msg Body Successfully...n");
						WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
						WriteLogFileInHex_Ex (&pay_res,__SYS_TYPE_LOGFILE_RAW__) ;
						
						return 0 ; 
				}else{
					bzero (szLogBuff,sizeof(szLogBuff)) ;	
					sprintf ("[%s-line:%d] Send the response Msg Body error...",__FILE__,__LINE__);
					WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
					WriteLogFileInHex_Ex (&pay_res,__SYS_TYPE_LOGFILE_RAW__) ;
					return  -1 ;
				}
			}else{
				bzero (szLogBuff,sizeof(szLogBuff)) ;
				printf ("[%s-line:%d] Send the Response Msg Head error...",__FILE__,__LINE__);
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
				WriteLogFile (&cmd_res_head,__SYS_TYPE_LOGFILE_RAW__) ;
				return -1 ;
			} 
 				
		}else{
			bzero (szLogBuff,sizeof(szLogBuff));
			sprintf (szLogBuff,"[%s-line:%d] Read request body of payment from remote host error,pls check your sys...",__FILE__,__LINE__);
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFile (&pay_req,__SYS_TYPE_LOGFILE_RAW__) ;
			return  -1 ;
		}
		
	}else if (ret==-1){
		bzero (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		sprintf (szLogBuff,"[%s-line:%d] Select () error ...",__FILE__,__LINE__); 
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
		return  -1 ;
	}else {
		bzero (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		sprintf (szLogBuff,"[%s-line:%d] Wating for data from remote host time out, pls check your system ...",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
		return  -1 ;
	}
		
}

static int Process_ChangePwd (int fd ,char *szSerialID ,char* szCommandID,char*szTimeStamp,char*szFollowLen)
{
	if (fd<0) return -1 ;
	char buffer [30] ;	
	memset (buffer,0x0,sizeof(buffer));
	
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff));
	
	read_config_file  ("./sysconfig.cfg","TIMEOUT_VAL",buffer);
	if (!strlen(buffer))
		sprintf (buffer,"%d",3);
	
	struct timeval tv ;
	memset (&tv,0x0,sizeof(struct timeval)) ;
	tv.tv_sec = atoi (buffer) ;
	tv.tv_usec = atoi (buffer); 
	
	int ret  = -1 ;
	fd_set rset ;
	FD_ZERO (&rset);
	FD_SET (fd,&rset);
	
	ret = select  (1024 , &rset ,NULL ,NULL , &tv ); 
	if (ret){//seuccessfully...
		CMD_CHANGEBUSINESSPWD_REQ chng_pwd_req ;
		memset (&chng_pwd_req,0x0,sizeof(CMD_CHANGEBUSINESSPWD_REQ));	
			
		if ((ret = read (fd,&chng_pwd_req,sizeof(CMD_CHANGEBUSINESSPWD_REQ)))==sizeof(CMD_CHANGEBUSINESSPWD_REQ)){
			bzero (szLogBuff,sizeof(szLogBuff)) ;
			sprintf (szLogBuff," Read the request Msg head successfully...") ;
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFileInHex_Ex (&chng_pwd_req,__SYS_TYPE_LOGFILE_RAW__) ;
			
			///////Do somethings ,suck as send to pos mainframe ....
			//
			//
			//
			//
			//
			//
			//
			///////////////////////////////////////////////////////////
			
			CMD_MSG_HEAD res_head; 
			memset (&res_head,0x0,sizeof(CMD_MSG_HEAD));
			CMD_CHANGEBUSINESSPWD_RES chng_pwd_res ;
			memset (&chng_pwd_res,0x0,sizeof(CMD_CHANGEBUSINESSPWD_RES));
			
			strncpy (res_head.szSerialID ,szSerialID,sizeof(res_head.szSerialID));
			strncpy (res_head.szCommandID,_PUB_CMD_CHANGEBUSINESSPWD_RES_,sizeof(res_head.szCommandID));
			memset (buffer,0x0,sizeof(buffer));
			GetTimeStamp (buffer);
			strncpy (res_head.szTimeStamp,buffer,sizeof(res_head.szTimeStamp)); 	 
			
			memset (buffer,0x0,sizeof(buffer));
			sprintf (buffer,"%d",sizeof(CMD_CHANGEBUSINESSPWD_RES));
			strncpy (res_head.szFollowLen,buffer,sizeof(res_head.szFollowLen));
				
			
			strncpy (chng_pwd_res.szTransferDateTime,"TransDateTime",sizeof(chng_pwd_res.szTransferDateTime));		
			strncpy (chng_pwd_res.szSysTraceNo,"",sizeof(chng_pwd_res.szSysTraceNo));
			strncpy (chng_pwd_res.szTransTime,"TransTime",sizeof(chng_pwd_res.szTransTime));
			strncpy (chng_pwd_res.szTransDate,"TransDate",sizeof(chng_pwd_res.szTransDate));
			strncpy (chng_pwd_res.szMerchantType,"",sizeof(chng_pwd_res.szMerchantType));
			strncpy (chng_pwd_res.szAcquireInstID,"",sizeof(chng_pwd_res.szAcquireInstID));
			strncpy (chng_pwd_res.szSenderInstID,"",sizeof(chng_pwd_res.szSenderInstID));
			strncpy (chng_pwd_res.szSysIndexOfRefNo,"",sizeof(chng_pwd_res.szSysIndexOfRefNo));
			strncpy (chng_pwd_res.szResponseCode,"",sizeof(chng_pwd_res.szResponseCode));
			strncpy (chng_pwd_res.szTerminalID,"",sizeof(chng_pwd_res.szTerminalID));
			strncpy (chng_pwd_res.szNewPassword,"",sizeof(chng_pwd_res.szNewPassword));
			
			ret  = -1 ;
			if ((ret= write (fd,&res_head,sizeof(CMD_MSG_HEAD)))==sizeof(CMD_MSG_HEAD)){
				bzero (szLogBuff,sizeof(szLogBuff));
				sprintf (szLogBuff,"Sending the response head to remote host successfully...");
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
				WriteLogFileInHex_Ex (&res_head,__SYS_TYPE_LOGFILE_RAW__) ;
				
				if((ret= write (fd,&chng_pwd_res,sizeof(CMD_CHANGEBUSINESSPWD_RES)))==sizeof(CMD_CHANGEBUSINESSPWD_RES)){
					bzero (szLogBuff,sizeof(szLogBuff)) ;
					sprintf (szLogBuff,"Sending the response body to remote host successfully...");
					WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
					WriteLogFileInHex_Ex (&chng_pwd_res,__SYS_TYPE_LOGFILE_RAW__) ;
					
				
				}else{
					bzero (szLogBuff,sizeof(szLogBuff)) ;
					sprintf (szLogBuff,"[%s-line:%d] Sending the response body to remote host failed...",__FILE__,__LINE__);
					WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
					WriteLogFileInHex_Ex (&chng_pwd_res,__SYS_TYPE_LOGFILE_RAW__) ;
					return  -1 ;
				}
			}else{
				bzero (szLogBuff,sizeof(szLogBuff));
				sprintf (szLogBuff,"[%s-line:%d] Sending the response head to remote host failed...",__FILE__,__LINE__);	
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
				WriteLogFileInHex_Ex (&res_head,__SYS_TYPE_LOGFILE_RAW__) ;
				
				return  -1 ;
			}
			return 0 ;
		}else{
			bzero (szLogBuff,sizeof(szLogBuff)) ;
			sprintf (szLogBuff,"Read the request Msg head error,pls check your system...");
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFileInHex_Ex (&chng_pwd_req,__SYS_TYPE_LOGFILE_RAW__);
			return -1; 
		}
		
	}else if ( ret==-1 ){//error
		bzero(szLogBuff,sizeof(szLogBuff)) ;	
		sprintf (szLogBuff,"[%s-line:%d] select () error,pls check your system...",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
		return  -1 ;
	}else {//timeout
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] waitting for data from remote host error...",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1;  
	}
	return 0;	
}		

static int Process_NetManage (int fd,char *szSerialID, char *szCommandID,char *szTimeStamp, char *szFollowLen)
{

	if (fd<0) {
		printf ("You have inputed an invalid socket handle,pls check your system...\n");
		return -1 ;
	} 	
	
	char buffer[30] ;
	memset (buffer,'\0',sizeof(buffer));
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff)) ;
	
	read_config_file ("./sysconfig.cfg","TIMEOUT_VAL",buffer);
	
	if (!strlen(buffer))
		sprintf (buffer,"%d",3);
	
	struct timeval tv ;
	memset (&tv,0x0,sizeof(struct timeval));
	tv.tv_sec = atoi (buffer) ;
	tv.tv_usec = atoi (buffer);
		
	fd_set rset ;
	FD_ZERO (&rset);
	FD_SET (fd,&rset) ;

			
	int ret = -1 ;
	ret  = select (1024 , &rset, NULL ,NULL, &tv) ;
	if (ret) {
		//CMD_MSG_HEAD msg_req;
		//memset (&msg_req,0x0,sizeof(CMD_MSG_HEAD));
		CMD_NETMANAGEMENT_REQ  net_manage_req; 
		memset (&net_manage_req,0x0,sizeof(CMD_NETMANAGEMENT_REQ)) ;
		if ((ret  = read (fd,&net_manage_req,sizeof(CMD_NETMANAGEMENT_REQ)))==sizeof(CMD_NETMANAGEMENT_REQ)){
			bzero (szLogBuff,sizeof(szLogBuff)) ;
			sprintf (szLogBuff,"[%s-line:%d] Reading the request head from remote host successfully...",__FILE__,__LINE__) ;
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFileInHex_Ex (&net_manage_req,__SYS_TYPE_LOGFILE_RAW__) ;
			/////////////////////Do somethings///////////////////////////////////
			//
			//
			//
			//
			//	Send the reqeust to pos mainframe .
			//
			//
			//
			///////////////////////////////////////////////////////////////////////

			CMD_MSG_HEAD msg_res ;
			CMD_NETMANAGEMENT_RES net_manage_res ;
			
			memset (&msg_res,0x0,sizeof(CMD_MSG_HEAD)) ;
			memset (&net_manage_res,0x0,sizeof(CMD_NETMANAGEMENT_RES));	
				
			strncpy (msg_res.szSerialID,szSerialID,sizeof(msg_res.szSerialID));	
			strncpy (msg_res.szCommandID,_PUB_CMD_NETMANGEMENT_REQ_,sizeof(msg_res.szCommandID));
			memset (buffer,0x0,sizeof(buffer));
			
			GetTimeStamp (buffer);
			strncpy (msg_res.szTimeStamp,buffer,sizeof(msg_res.szTimeStamp));
			
			memset (buffer,0x0,sizeof(buffer));
			sprintf (buffer,"%d",sizeof(CMD_NETMANAGEMENT_RES)) ;
			strncpy (msg_res.szFollowLen,buffer,sizeof(msg_res.szFollowLen));
			

			strncpy (net_manage_res.szTransferDateTime,"TransDateTime",sizeof(net_manage_res.szTransferDateTime));
			strncpy (net_manage_res.szSysTraceNo,"TraceNo",sizeof(net_manage_res.szSysTraceNo));
			strncpy (net_manage_res.szSenderInstID,"",sizeof(net_manage_res.szSenderInstID));
			strncpy (net_manage_res.szResponseCode,"00",sizeof(net_manage_res.szResponseCode));
			//It'll send the reponse messages
		
			if ((ret= write (fd,&msg_res,sizeof(CMD_MSG_HEAD)) )==sizeof(CMD_MSG_HEAD)){
				bzero (szLogBuff,sizeof(szLogBuff)) ;
				sprintf (szLogBuff,"Sending the response head successfully...");
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
				WriteLogFileInHex_Ex (&msg_res,__SYS_TYPE_LOGFILE_RAW__) ;
				
				if ((ret  = write (fd,&net_manage_res,sizeof(CMD_NETMANAGEMENT_RES))) == sizeof(CMD_NETMANAGEMENT_RES)){
					bzero (szLogBuff,sizeof(szLogBuff));
					sprintf (szLogBuff,"Sending the response body to remote host successfully...");
					WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
					WriteLogFileInHex_Ex (&net_manage_res,__SYS_TYPE_LOGFILE_RAW__) ;
						
				}else{
					bzero (szLogBuff,sizeof(szLogBuff));
					sprintf (szLogBuff,"[%s-line:%d] Sending the response body error,pls chck your system ...",__FILE__,__LINE__);
					WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
					WriteLogFileInHex_Ex (&net_manage_res,__SYS_TYPE_LOGFILE_RAW__) ;
					return -1; 	
				}		
			}else{
				bzero (szLogBuff,sizeof(szLogBuff));
				sprintf (szLogBuff,"[%s-line:%d] Sending the head of response error,pls chck your system...",__FILE__,__LINE__);
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
				WriteLogFileInHex_Ex (&msg_res,__SYS_TYPE_LOGFILE_RAW__) ;
				return -1 ;		
			}					
		}else{
			bzero (szLogBuff,sizeof(szLogBuff)) ;
			sprintf (szLogBuff,"[%s-line:%d] Reading request head from remote host error...",__FILE__,__LINE__) ;
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFileInHex_Ex (&net_manage_req,__SYS_TYPE_LOGFILE_RAW__) ;
			return -1 ;
		}		
			
	}else if (ret == -1){
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		
		sprintf (szLogBuff,"[%s-line:%d] Select() error...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;		
	}else {
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] Select exe timeout...",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;
	}	
	
	return 0; //all successfully done. 
}
				

static int Process_Rollback (int fd,char* szSerialID,char* szCommandID,char*szTimeStamp,char*szFollowLen)
{
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff)) ;
	
		
	if (fd<0){
		bzero (szLogBuff,sizeof(szLogBuff));
		sprintf (szLogBuff,"[%s-line:%d]You have passed an invalid handle,pls chck your system...",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
		return  -1 ;
	}	
	
	char tmp [30] ;
	memset (tmp,'\0',sizeof(tmp));
	read_config_file ("./sysconfig.cfg","TIMEOUT_VAL",tmp);
	if (strlen(tmp))
		sprintf (tmp,"%d",3);
	
	
	struct timeval tv ;
	memset (&tv,0x0,sizeof(struct timeval));
	
	tv.tv_sec= atoi (tmp);
	tv.tv_usec = atoi (tmp);
	
	fd_set rset  ;
	FD_ZERO  (&rset) ;
	FD_SET (fd,&rset) ;
	
	int ret =-1 ;
	
	ret  = select (1024 , &rset,NULL,NULL,&tv) ;
	if (ret) {
		CMD_TRANSROLLBACK_REQ rollback_req ;
		memset (&rollback_req ,0x0,sizeof(CMD_TRANSROLLBACK_REQ)); 
	
		if((ret  = read (fd,&rollback_req,sizeof(CMD_TRANSROLLBACK_REQ)))== sizeof(CMD_TRANSROLLBACK_REQ)){
			bzero (szLogBuff,sizeof(szLogBuff));
			sprintf (szLogBuff,"Read the head of request from remote host successfully...");
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__);
			///////////do somethings, such as send the request to pos mainframe.
			//
			//
			//
			//
			//
			//
			//
			//
			//
			////////////////////////////////////////////////////////////////////
				
			
			CMD_MSG_HEAD res_head; 	
			CMD_TRANSROLLBACK_RES rollback_res ;
			
			memset (&res_head,0x0,sizeof(res_head)) ;
			memset (&res_head,0x0,sizeof(rollback_res)) ;
			
			strncpy (res_head.szSerialID,szSerialID,sizeof(res_head.szSerialID));
		        strncpy(res_head.szCommandID,_PUB_CMD_TRANSROLLBACK_RES_,sizeof(res_head.szCommandID));
			
			memset (tmp,0x0,sizeof(tmp));
			GetTimeStamp (tmp); 
			strncpy (res_head.szTimeStamp,tmp,sizeof(res_head.szTimeStamp));		
		 
			sprintf (tmp,"%d",sizeof(CMD_TRANSROLLBACK_RES));
			strncpy (res_head.szFollowLen,tmp,sizeof(res_head.szFollowLen));
			
			strncpy (rollback_res.szPaymentAmount,rollback_req.szPaymentAmount,sizeof(rollback_res.szPaymentAmount));
			strncpy (rollback_res.szSysTraceNo,rollback_req.szSysTraceNo,sizeof(rollback_res.szSysTraceNo));
			strncpy (rollback_res.szLocalTransTime,"",sizeof(rollback_res.szLocalTransTime)) ;
			strncpy (rollback_res.szLocalTransDate,"",sizeof(rollback_res.szLocalTransDate));
			strncpy (rollback_res.szSettlementDate,"",sizeof(rollback_res.szSettlementDate));
			strncpy (rollback_res.szMerchantType,"",sizeof(rollback_res.szMerchantType));
						
			strncpy (rollback_res.szAcquireInstID,"",sizeof(rollback_res.szAcquireInstID));
			strncpy (rollback_res.szSenderInstID,"",sizeof(rollback_res.szSenderInstID));
			strncpy (rollback_res.szResponseCode,"",sizeof(rollback_res.szResponseCode));	
			strncpy (rollback_res.szTerminalID,"",sizeof(rollback_res.szTerminalID));
			strncpy (rollback_res.szMerchantID,"",sizeof(rollback_res.szMerchantID));
			strncpy (rollback_res.szOriginalTransID,"",sizeof(rollback_res.szOriginalTransID));
			
			
			if ((ret=(write(fd,&res_head,sizeof(CMD_MSG_HEAD))))==sizeof(CMD_MSG_HEAD)){	
				bzero (szLogBuff,sizeof(szLogBuff));
				sprintf (szLogBuff,"Write the response head to remote host successfully...");
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
				WriteLogFileInHex_Ex (&res_head,__SYS_TYPE_LOGFILE_RAW__) ;
				if ((ret=write(fd,&rollback_res,sizeof(CMD_TRANSROLLBACK_RES)))==sizeof(CMD_TRANSROLLBACK_RES)){
					bzero (szLogBuff,sizeof(szLogBuff)) ;
					sprintf (szLogBuff,"Write the reponse body to remote host successfully...") ;
					WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
					WriteLogFileInHex_Ex (&rollback_res,__SYS_TYPE_LOGFILE_RAW__) ;
					
				}else{
					bzero (szLogBuff,sizeof(szLogBuff)) ;
					sprintf (szLogBuff,"[%s-line:%d]Write the response body to remote host error...",__FILE__,__LINE__);
					WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
					WriteLogFileInHex_Ex (&rollback_res,__SYS_TYPE_LOGFILE_RAW__) ;
					return -1 ;
				}		
			}else{
				bzero (szLogBuff,sizeof(szLogBuff)) ;
				sprintf (szLogBuff,"[%s-line:%d] Write the reponse head to remote host error...",__FILE__,__LINE__);
				WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
				WriteLogFileInHex_Ex (&res_head,__SYS_TYPE_LOGFILE_RAW__) ;
				return -1 ;
			}	
			
		}else{
			bzero (szLogBuff,sizeof(szLogBuff)) ;
			sprintf (szLogBuff,"[%s-line:%d] Read the head of request from remote host error...",__FILE__,__LINE__);
			
			WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFileInHex_Ex (&rollback_req,__SYS_TYPE_LOGFILE_RAW__) ;
			
			return -1 ;
		}
		
	}else if (ret==-1) {
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] Select() error...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;
	}else {
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] Waiting for data from remote host error...",__FILE__,__LINE__);
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1;
		
	}		
		
	
	return  0 ; 
}

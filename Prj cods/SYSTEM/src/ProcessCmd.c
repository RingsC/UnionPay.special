/************************************************************************
 *
 *	Copyright :
 *		All rigths reseverd by lihao and palm info company. 
 *	
 *	Purpose :
 *		Process user's command .
 *			
 *	Author: lihao 
 *	
 *	Date : Oct 8 , 2008 
 *
 *	Change List:
 *		
 *		NONE ;
 *
 *
 *
 *
 ***********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../PUBLIC/include/globalvars.h"
#include <signal.h> //to install signal handle.

static void 
handler(int sig, siginfo_t *si, void *unused)
{
           printf("Got SIGSEGV at address: 0x%lx\n",
                   (long) si->si_addr);
	
           exit(EXIT_FAILURE);
}

int ProcessSignal ()
{	
	char szLogBuffer [MAXCONTLEN] ;
	bzero (szLogBuffer,sizeof(szLogBuffer)) ;
		
	struct sigaction sa ;
	bzero (&sa,sizeof(struct sigaction)) ;
        sa.sa_flags = SA_SIGINFO;
        sigemptyset(&sa.sa_mask);
        sa.sa_sigaction = handler;
        if (sigaction(SIGSEGV, &sa, NULL) == -1){
		sprintf (szLogBuffer,"[%s-line:%d] Set the signal error...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;	
		return -1; 
	}		
	return  0; 
}
static void InstallSig_Event (int sig)
{
	char szLogBuffer [MAXCONTLEN] ;
	bzero (szLogBuffer,sizeof(szLogBuffer)) ;
	
	if (sig == SIGTERM) {
		sprintf (szLogBuffer,"You want to terminate system in abnormal way,your command is discarded...\n") ;	
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__);
		return  ;
	}
	return  ;
}
void InitGVariables () 
{
	g_ListenSktHandle = -1 ; 
	g_SeedID  =0 ;
	gb_ListeningFlag = 0;
}
int ProcessSysSignal () 
{
	signal(SIGHUP, SIG_IGN);    

	signal(SIGINT, SIG_IGN);    

	signal(SIGQUIT, SIG_IGN);   

	signal(SIGTSTP, SIG_IGN);		

	signal(SIGKILL, SIG_IGN); 
	
	signal (SIGILL,SIG_DFL) ; //to dump core file .
	
	signal (SIGABRT,SIG_DFL) ; //to dump core file .
	
	signal (SIGFPE,SIG_DFL) ;// to dump core file .		
		
	signal (SIGTERM, InstallSig_Event);
}
static int PrintHelp ()
{
	char szTmp [MAXCONTLEN] ;
	memset (szTmp,'\0',sizeof(szTmp)) ;
	
	sprintf (szTmp," list ------list all parameters which is using by system") ;
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
	memset (szTmp,'\0',sizeof(szTmp));	

	sprintf (szTmp," exe -------execute a shell command with current shell");
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
	memset (szTmp,'\0',sizeof(szTmp));	
	
	sprintf (szTmp," startup ---start up the socket listening system");
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
	memset (szTmp,'\0',sizeof(szTmp));	
	
	sprintf (szTmp," shutdown---shut down the socket listening system");
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
	memset (szTmp,'\0',sizeof(szTmp));	
	
	sprintf (szTmp," help ------list all help message and usage of system commands") ;
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
	memset (szTmp,'\0',sizeof(szTmp));	
	
	sprintf (szTmp," quit ------quit system");
	WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
	memset (szTmp,'\0',sizeof(szTmp));	
}

static int ProcessListCmd (char* szCommadPara)
{
	char buffer [30];
	memset (buffer,0x0,sizeof(buffer));	
	
	read_config_file ("./sysconfig.cfg","LISTEN_IP",buffer);
	printf ("[Sytstem parameters as following]\n");
	printf ("\n");
	printf ("***********************************************\n");
	printf ("Listen IP:%s\t",buffer) ;
		
	memset (buffer,0x0,sizeof(buffer));
	read_config_file ("./sysconfig.cfg","LISTEN_PORT",buffer);
	printf ("Listen Port:%s\t\n",buffer);

		
	memset (buffer,0x0,sizeof(buffer));
	read_config_file("./sysconfig.cfg","REMOTE_IP",buffer);
	printf ("Remote IP:%s\t",buffer);
	

	memset (buffer,0x0,sizeof(buffer));
	read_config_file("./sysconfig.cfg","REMOTE_PORT",buffer);
	printf ("Remote PORT:%s\n",buffer);
	
	memset (buffer,0x0,sizeof(buffer));
	read_config_file("./sysconfig.cfg","TIMEOUT_VAL",buffer);
	printf ("TimeOut Value:%s(:sec)\t\n",buffer);
	

	printf ("\n***********************************************\n");
	return  0;
}
static int ProcessHelpCmd (char* szHelpCmdPara)
{
	PrintHelp() ;
	return  0;
}	
static int ProcessQuitCmd (char* szQuitCmdPara)
{
	char szLogBuffer [MAXCONTLEN] ;
	bzero (szLogBuffer,sizeof(szLogBuffer)) ;
	
	if (gb_ListeningFlag) {
		sprintf (szLogBuffer,"The socket is working,you can not quit system. You should run \"shutdown\" first to shutdown socket listeing...") ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		return 1;
	}
	
	shutdown (g_ListenSktHandle,2);
	close (g_ListenSktHandle) ;
	gb_ListeningFlag  = 0 ; // Before we quit system , we should terminate the listening thread fistly.
	
	//pthread_exit (&gb_ListenThread) ;
	return  0;
}
static int ProcessExeCmd (char* szExeCmdPara)
{
	if (!strlen(szExeCmdPara)) return -1 ;
	return system (szExeCmdPara);
}
static int ProcessStartSocketListenCmd (char* szExeCmdPara)
{
	int iListenPort = 8080 ;
	if (strlen(szExeCmdPara)==0) {
		//printf ("You do not set listening port ,using the default port\n");	
	}else
		iListenPort  = atoi (szExeCmdPara);
	
	//printf  ("System will listen on %d... \n",iListenPort) ;
	g_ListenSktHandle = -1 ;
	gb_ListeningFlag  = 1 ; //Start Listening Thread.
	StartSocket_Listen ("127.0.0.1" ,iListenPort ,0,&g_ListenSktHandle) ;
}

static int ProcessShutDownCmd (char *szCmdPara) 
{
	//First of all, we close the listening socket handle.
	gb_ListeningFlag  = 0 ;	//Set the Listening Flag to false;
	shutdown (g_ListenSktHandle,2) ;
	close (g_ListenSktHandle) ;
}
int ProcessCommand ()
{
	char szCommand [MAXCONTLEN], szCmdKey[120] ;
	memset (szCommand,'\0',sizeof(szCommand));
	memset (szCmdKey,'\0' ,sizeof(szCmdKey));
	
	int bRunFlag =1   ;
	char* szCmdMsg[150] ;
	
	char szTmp [MAXCONTLEN] ;	//Reflush the buffer.
	memset (szTmp,'\0',sizeof(szTmp)) ;	
	
	while (bRunFlag){//To process your input command strings. 
		printf ("OnlinePaySys]") ;
		memset (szCommand,'\0',sizeof(szCommand));
		if (fgets (szCommand,MAXCONTLEN,stdin)){
			TrimString(szCommand);
			if (!strlen (szCommand)) continue ;
		        if (*szCommand=='\n') continue ;
			int i ;		
			
			int sNum = splitString (szCommand,szCmdMsg ,' ');
			if (sNum<2)
				strncpy (szCmdKey,szCmdMsg[0],strlen(szCmdMsg[0])-1);	
			else
				strncpy (szCmdKey,szCmdMsg[0],strlen(szCmdMsg[0])) ;
		
			char szCmdParas[MAXCONTLEN] ;
			memset (szCmdParas,'\0',sizeof(szCmdParas)) ;
				
			for ( i =1  ;i < sNum ;i++) {
				strcat (szCmdParas,szCmdMsg[i]);
				strcat (szCmdParas," ");
			}
			
			if (strcmp(szCmdKey,"list")==0){
				ProcessListCmd (szCmdParas) ;
			}else if (strcmp(szCmdKey,"help")==0){
				ProcessHelpCmd (szCmdParas) ;
			} else if (strcmp(szCmdKey,"quit")==0){
				bRunFlag  = ProcessQuitCmd(szCmdParas) ;
			}else if (strcmp(szCmdKey,"exe")==0){
				ProcessExeCmd (szCmdParas);	 
			}else if (strcmp(szCmdKey,"startup")==0){
				ProcessStartSocketListenCmd (szCmdParas) ;
			}else if (strcmp(szCmdKey,"shutdown")==0){
				ProcessShutDownCmd (szCmdParas) ;
			}
			else {
				memset (szTmp,'\0',sizeof(szTmp)) ;
				sprintf (szTmp,"Invalid Command, please re-input your command again or \
use 'help' command to list all command usages ");
				WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
			}
			
			
			for  (i= 0 ;i < sNum ; i++) {
				memset (szCmdMsg [i] ,'\0',strlen (szCmdMsg[i]));
			}

			memset (szCmdKey ,'\0',strlen (szCmdKey));
		}else
			continue ;

	}
	return 0;
}

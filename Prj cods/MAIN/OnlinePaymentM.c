#include <stdio.h>
#include <string.h>
#include "globalvars.h"
#include <stdlib.h>
#include <unistd.h> 


int main (int argc ,char* argv[])
{
	if  (argc<1) return -1; 
	
	int iParent = fork () ;
	if (iParent ==-1  ) {
		printf ("Can not fork a process,pls check your system....\n") ;
		return -1; 
	} else if (iParent==0 ) { //here ,i will generate a daemon process to monitor the running states of sy
		/*
		int iSessionIDOfCall  =  setsid () ;
		
		int iPid = getpid () ;
		
		umask (0) ;	
		chdir ("/") ;	
		printf ("The new sessionID of calling :%d\n",iSessionIDOfCall) ; 
		*/
        } else{//IN parent process .
		char szTmp [MAXCONTLEN] ;
		memset (szTmp,'\0',sizeof(szTmp)) ;
	
		int iret  =-1  ; 
		if (InitializeGlboalVars () != 0){ //Here ,we will write the log to file and show on screen .
			memset (szTmp ,'\0',sizeof(szTmp)) ;
			sprintf (szTmp,"[%s-line:%d]Initializing the global varibales errors occur",__FILE__,__LINE__);
			WriteLogFile (szTmp , __SYS_TYPE_LOGFILE_ERR__) ;
		}
		else {
			memset (szTmp,'\0',sizeof(szTmp)) ;
			sprintf (szTmp,"[%s-line:%d]Initializating the global varibalse successfully",__FILE__,__LINE__);
			WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__) ;
		}
		InitGVariables  () ;
		if (!ProcessSysSignal ()) { 
			memset (szTmp,'\0',sizeof(szTmp)) ;
			sprintf  (szTmp,"[%s-line:%d]Processing system singnales successfully",__FILE__,__LINE__);
			WriteLogFile (szTmp,__SYS_TYPE_LOGFILE_RAW__);
		}
	
		#ifndef __DEBUG__
			ProcessCommand (); 
		#endif
	} //In Parent Process .
	return 0 ;
	
}

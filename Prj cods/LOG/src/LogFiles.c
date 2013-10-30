/**************************************************************************	
 *	Copyrights: All copyrights reserved by lihao & plam co,.ltd
 *	Purpose: To write log messages into files.
 *	Date: Sep 13, 2008
 *	Author: lihao 
 *	Change History:
 *		Sep 13, 2008 Created
 *	
 *
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "../../PUBLIC/include/globalvars.h"

pthread_mutex_t pLogFileWrite_Mutex = PTHREAD_MUTEX_INITIALIZER ;

static int WriteLogFileInHex  (const char* ,const char* );//Local Function 
static int WriteLogFile_Ex (const char*,const char* ) ;	  //..
int WriteLogFile (const char*,int) ;			//Writing the logMessage into file [global]
int WriteLogFileInHex_Ex (const char* ,int ) ;		// ...

int WriteLogFile (const char* szStrMsg , int iType)
{ // According to iType to write log file independly...
	if (!strlen(szStrMsg))
		return  -1  ;
	time_t t ;
	struct tm* tm = NULL ;
	t = time (NULL)	 ;
	tm = localtime (&t) ;
	char szBuffer [MAXCONTLEN] ;
	memset (szBuffer, '\0',sizeof(szBuffer)) ;
	
	int iRet =-1 ;
	switch (iType){
		case __SYS_TYPE_LOGFILE_RAW__ :{
			
			if (tm){
				strftime (szBuffer, sizeof(szBuffer),"%Y%m%d.raw",tm);
			}	
		} break;
		case __SYS_TYPE_LOGFILE_ERR__ :{
			if (tm){
				strftime (szBuffer, sizeof(szBuffer),"%Y%m%d.err",tm);
			}
		} break;
		case __SYS_TYPE_LOGFILE_SQL__ :{
			if (tm){
				strftime (szBuffer, sizeof(szBuffer),"%Y%m%d.sql",tm);
			}
		} break; 
		default : break ; 
	}
		
	if (strlen(szBuffer))			
		return	 WriteLogFile_Ex ((const char*)szBuffer,szStrMsg) ;	
	else
		return  iRet ; 
}
int WriteLogFileInHex_Ex (const char* szStrMsg,int iType) 
{
	if (!strlen(szStrMsg)) //NNULL is passed into.
		return  -1 ; 
	//Here ,i will generate the file name for per day files, the rules is dataetime of Today.
	time_t t ;
	struct tm* tm  = NULL  ;
	t = time (NULL) ;
	tm =  localtime (&t);
	char tmp [MAXCONTLEN] ;
	memset (tmp , '\0',sizeof(tmp));
	switch (iType){
		case __SYS_TYPE_LOGFILE_RAW__ : { 
			if (tm)
				strftime (tmp,sizeof(tmp),"%Y%m%d.raw",tm) ;
		} break; 
		case __SYS_TYPE_LOGFILE_ERR__ :{
			if (tm)
				strftime (tmp,sizeof(tmp),"%Y%m%d.err",tm); 
		} break ;
		case __SYS_TYPE_LOGFILE_SQL__ : {
			if (tm)
				strftime (tmp,sizeof(tmp),"%Y%m%d.sql",tm) ;
		} break ;
	}		
	
	return  WriteLogFileInHex (szStrMsg, (const char*)tmp)  ;
 
}

static int WriteLogFileInHex  (const char* szLogMsg,const char* szFileName){
	pthread_mutex_lock (&pLogFileWrite_Mutex);
	
	if (!strlen (szFileName)||!strlen (szLogMsg)) {
		printf ("You have passed an invalid msg,pls check your system...\n") ;
		return -1 ;
	}

	char szBuffer[1024] ;
	memset (szBuffer,'\0',sizeof(szBuffer)) ;
		
	char szTmp [10] ;
	memset (szTmp,'\0',sizeof(szTmp)) ;	
	
	int iShow = 0;	
	int iWrite  = 0  ;	

	read_config_file ("./sysconfig.cfg","MSGSWITCH",szTmp) ;
	if (!strlen(szTmp))		
		iShow =0 ;	
	else
		iShow  = atoi (szTmp) ;
	memset (szTmp,'\0',sizeof(szTmp));	
	read_config_file("./sysconfig.cfg","LOGWRITE",szTmp);
	if (!strlen(szTmp))
		iWrite  = 0 ;
	else
		iWrite = atoi (szTmp);
	
	time_t t ; 
	struct tm* tm  = NULL ;
	t = time (NULL);
	tm= localtime(&t) ;
	char tmptmp  [MAXCONTLEN] ;
	memset (tmptmp,'\0',sizeof(tmptmp));
	if (tm)
		strftime (tmptmp,sizeof(tmptmp),"\n[%Y-%m-%d %H:%M:%s]\n",tm);
	else{
		pthread_mutex_unlock (&pLogFileWrite_Mutex) ; 
		return  - 1 ;
	}
	
	FILE* fd = NULL ;
	
	if  ((fd= fopen (szFileName,"a+"))==NULL){
		fclose(fd) ;
		pthread_mutex_unlock (&pLogFileWrite_Mutex);
		return  -1 ;
	}
	if (iWrite) {
		fseek (fd,0L,SEEK_END);
		fwrite (tmptmp,sizeof(char), strlen(tmptmp) ,fd) ;
		fseek (fd,0L,SEEK_END) ;	
	}
	
	int iLen = strlen (szLogMsg) ;
	int x =0 ;
	int y = 0 ;
	char tmp [16] ;
	memset (tmp,'\0',sizeof(tmp)) ;	
	//printf ("\n");	
	int ipos =0 ;
	for (x= 0 ; x < iLen ; ++x) {
		tmp [x%16] = szLogMsg[x] ;
		
		if ((x+1)%16==0) {
			for (y=0 ; y< 16 ; ++y ){
				sprintf (szBuffer+ipos,"0x%02X ",tmp[y]&0xFF);
				//printf ("0x%02X ",tmp[y]&0xFF);
				ipos +=5 ;
			}
			for (y=0 ; y < 16; ++y ){
				sprintf (szBuffer+ipos,"%c",isprint (tmp[y])?tmp[y]:'.');
				//printf ("%c",isprint (tmp[y])?tmp[y]:'.');
				ipos ++ ;
			}
			//printf ("\n");
			strcat(szBuffer,"\n");
			ipos +=1 ;
		}
	}
	if (iShow)	
		printf ("%s\n" ,szBuffer) ;
	if (iWrite){
		fwrite (szBuffer,sizeof(char), strlen(szBuffer) ,fd) ;
		fseek (fd,0L,SEEK_END);
	}	

	fclose(fd);
	pthread_mutex_unlock (&pLogFileWrite_Mutex) ;
	return  0 ;	//Successfully...
}
	
static int WriteLogFile_Ex (const char* szFileName,const char* szLogMsg )
{

	pthread_mutex_lock (&pLogFileWrite_Mutex) ;
	
	if (!strlen (szFileName)||!strlen(szLogMsg)){
		pthread_mutex_unlock(&pLogFileWrite_Mutex) ;	
		return  -1 ;
	}
	
	//Retrieve local time 
	time_t t ;
	struct tm* tm = NULL;
	
	t  = time (NULL);
	tm = localtime(&t);	
	char tmp [MAXCONTLEN] ;
	memset (tmp,0x00,sizeof(tmp));

	if  (tm)
		strftime (tmp,sizeof(tmp),"[%Y-%m-%d %H:%M:%S] ",tm);
	else {
		pthread_mutex_unlock(&pLogFileWrite_Mutex) ;
		return  -1 ;
	}
	
	int iShow  =0 ;
	int iWrite  = 0 ;
	
	char szBuff [10] ;	//To hold the flag of Whether shows the log message on screen .
	memset (szBuff,'\0',sizeof(szBuff)) ;	
	read_config_file ("./sysconfig.cfg","MSGSWITCH",szBuff) ;
	if (!strlen (szBuff))
		iShow  =0 ;
	else 
		iShow = atoi (szBuff) ;
		
	memset (szBuff,'\0',sizeof(szBuff)) ;
	read_config_file ("./sysconfig.cfg","LOGWRITE",szBuff) ;
	if (!strlen(szBuff))
		iWrite  =  0 ;
	else
		iWrite  = atoi (szBuff);
		
	FILE* fd = NULL ;
	if  ((fd= fopen (szFileName,"a+"))==NULL){
		fclose(fd) ;
		pthread_mutex_unlock (&pLogFileWrite_Mutex);
		return  -1 ;
	}
	if (iShow)	
		printf ("%s",tmp);
	if (iWrite) {
		fseek (fd,0L,SEEK_END);
		fwrite (tmp,sizeof(char), strlen(tmp)+0 ,fd) ;
	}
	if (iShow)
		printf ("%s\n",szLogMsg);
	if (iWrite){
		fseek (fd,0L,SEEK_END) ;	
		fwrite (szLogMsg,sizeof(char), strlen(szLogMsg)+0 ,fd);	
		fseek (fd,0L,SEEK_END) ;	
		fwrite ("\n\r",sizeof(char),2,fd) ;
	}

	fclose(fd);
	pthread_mutex_unlock (&pLogFileWrite_Mutex) ;
	return  0 ;
}

/********************************************************************
 *
 *
 *
 * Copyrights: All copyright reserved by lihao & palm co,.ltd
 *	Purpose:  The public functions ,which maybe used in main function or other fuctions
 *	Author:	lihao
 *	Date:	Sep 13 ,2008
 *	Change History:
 *		Sep 13 , 2008 Created 
 *		
 *		Dec 7 2008 :
 *			In function of GetTerminalID, we use terminalID at random order.
 *			We generate a numic between 0 and ArrLen . Just simulating the hash
 *			table Index .
 *
 *		
 *
 *	commnents:
 *		
 *
 ********************************************************************/

#include <stdio.h>
#include <string.h>
#include "../include/globalvars.h"
#include <time.h>
#include <pthread.h>

static pthread_mutex_t getSerLock_t = PTHREAD_MUTEX_INITIALIZER ;

int InitializeGlboalVars ()
{ // To initialize the global varibales .
	
	g_SeedID = -1 ; 
	g_ListenSktHandle = -1 ; 
	gb_ListeningFlag  = -1  ; 
	
	return  0 ;
}
int GetTimeStamp (char* szBuffer)
{

        char tmp[MAXCONTLEN] ;
        memset (tmp,0x00,sizeof(tmp)) ;

        time_t t ;
        struct tm* t_tm ;
        t = time (NULL);
        t_tm = localtime(&t);
        sprintf  (tmp,"%4d-%02d-%2d %2d:%2d:%2d",
                                       t_tm->tm_year+1900 ,t_tm->tm_mon,t_tm->tm_mday,
                                            t_tm->tm_hour,t_tm->tm_min,t_tm->tm_sec);
         strcpy (szBuffer,tmp);
         return 0; 
}
int GetLocalTime (char* szBuffer)
{
	char tmp [7] ;
	memset (tmp,'\0',sizeof(tmp)) ;	
	
	time_t t ;
	struct tm* t_tm ;
	t = time  (NULL);
	t_tm = localtime (&t) ;
	sprintf (tmp,"%2d%2d%2d",t_tm->tm_hour,t_tm->tm_min,t_tm->tm_sec) ;
	strncpy (szBuffer,tmp,strlen(tmp)) ;
	return  0; 
}	


int GetLocalDate (char* szBuffer) 
{
	char tmp [5] ;
	memset (tmp,'\0',sizeof(tmp)) ;	
	
	time_t t ;
	struct tm* t_tm ;
	t = time  (NULL);
	t_tm = localtime (&t) ;
	sprintf (tmp,"%2d%2d",t_tm->tm_mon,t_tm->tm_mday) ;
	strncpy (szBuffer,tmp,strlen(tmp)) ;
	return  0; 

}
int GetPublicSerial (char* pszBuffer)
{

	if (!pszBuffer) return -1 ;
	
   	pthread_mutex_lock(&getSerLock_t) ;		
	
	char tmp[MAXCONTLEN] ;
	memset (tmp,0x00,sizeof(tmp)) ;
	
	time_t t ;	
	struct tm* t_tm ;
	t = time (NULL);	
	t_tm = localtime(&t);
	
	if (t_tm==NULL) { 
		pthread_mutex_unlock(&getSerLock_t);
		return -1 ;
	}
	//Here ,we can use strftime to format the time.for more info use 'man strftime'.		
	sprintf  (tmp,"%4d%02d%2d%2d%2d%2d",
			t_tm->tm_year+1900 ,t_tm->tm_mon,t_tm->tm_mday,
				t_tm->tm_hour,t_tm->tm_min,t_tm->tm_sec
		) ;
	char tmptmp [10] ;
	g_SeedID ++ ;
	sprintf(tmptmp,"%d",g_SeedID);
	strncat (tmp,tmptmp,strlen(tmptmp));
	strcpy (pszBuffer,tmp);
        pthread_mutex_unlock(&getSerLock_t) ;	
	return  0 ;
}

int TestBitByIndex (int testee ,int index)
{
	if (index<0|| index>64) return  -1 ;
	int tmp  = -1  ;	
	tmp = 0x01 <<  index ;
	
	return ( testee & tmp  )?1:0 ;
}

int ConvertTo8LenBinStr (char value , char* szOutput) 
{
	char sztmp [9] ;
	memset (sztmp,'0',sizeof(sztmp));
	sztmp [8] ='\0' ;
	int i= 7;
	int ival = value ;
	while (ival/2){
		sztmp [i] = (ival % 2)?'1':'0' ;
		ival = ival/ 2 ;
		i -- ; 
	}
	sztmp [i] = (ival)?'1':'0' ;
	memcpy (szOutput,sztmp,9) ;
	return 0;	
}
int ConvertIntToStrOnBase (unsigned long  testee,int iBase , char* szBuffer)
{//Convert a long var to a 128 bits length binary string ,just like 1 to 0000...0000001

	
	if (testee<0|| iBase<=0 ) return -1 ;
	char szTmp [128+1] ;	
	memset (szTmp,'0',sizeof(szTmp));
	
	int i =127 ;
	int iVal = testee ;
	while  (iVal/iBase){
		switch ( iVal % iBase) {
			case 0: szTmp [i] ='0'; break ;
			case 1: szTmp [i] ='1' ; break; 
			case 2: szTmp [i] ='2' ; break; 
			case 3: szTmp [i] ='3' ; break; 
			case 4: szTmp [i] = '4' ;break; 
			case 5: szTmp [i] = '5' ;break;
			case 6: szTmp [i] = '6' ;break;
			case 7: szTmp [i] = '7' ;break;
			case 8: szTmp [i] = '8' ;break;
			case 9: szTmp [i] = '9' ;break;
			case 10: szTmp [i] = 'A' ;break;
			case 11: szTmp [i] = 'B' ;break;
			case 12: szTmp [i] = 'C' ;break;
			case 13: szTmp [i] = 'D' ;break;
			case 14: szTmp [i] = 'E' ;break;
			case 15: szTmp [i] = 'F' ;break;
			default: break; 	
		}
		iVal = iVal / iBase ;
		i -- ;
	}
	char slast ;
	switch (iVal){
		case 0 : slast ='0' ; break;
		case 1 : slast ='1' ; break; 
		case 2 : slast ='2' ; break; 
		case 3 : slast ='3' ; break;
		case 4 : slast ='4' ; break; 
		case 5 : slast ='5' ; break;
		case 6 : slast ='6' ; break;
		case 7 : slast ='7' ; break; 
		case 8 : slast ='8' ; break; 
		case 9 : slast ='9' ; break;
		case 10: slast ='A' ; break;
		case 11: slast ='B' ; break;
		case 12: slast ='C' ; break;
		case 13: slast ='D' ; break;
		case 14: slast ='E' ; break; 
		case 15: slast ='F' ; break;
		default : break; 
	}	
	szTmp[i] = slast ;
	szTmp [128] ='\0' ;
	strncpy (szBuffer,szTmp,strlen(szTmp));
	return 0;
}	

int StringOfChar (char cChar, int iLen ,char* szOutput) 
{	
	if (iLen>128) { //The maximum size : 128
		return -1; 
	}
	char szTmp [128+1];
	memset (szTmp,cChar,sizeof(szTmp));	
	szTmp[128] ='\0' ;
	strncpy (szOutput,szTmp,iLen);
	return 0;
}
static int Get_VarStr (const char* szStr ,int iCd,char* szOutput) 
{
	bzero(szOutput,sizeof(szOutput)) ;
	int iTmLen ;
	int iLen = strlen (szStr);
	iTmLen = iLen ;

	char szTmp [MAXCONTLEN/3];
	bzero (szTmp,sizeof(szTmp));
	
	sprintf (szTmp,"%d",iLen) ;
	iLen = strlen (szTmp) ;			
	
	bzero (szTmp,sizeof(szTmp)) ;	
	StringOfChar ('0',iCd-iLen,szTmp);
	strncat (szOutput,szTmp,strlen(szTmp));
	bzero (szTmp,sizeof(szTmp));
	
	sprintf (szTmp,"%d",iTmLen) ;
	strncat (szOutput,szTmp,strlen(szTmp)) ;
	strncat (szOutput,szStr,strlen(szStr));
	
	return   0;
}
int Fill_With_Char (const int iWichSide ,const char* szOriginalStr ,char* szFilledStr ,const char cFillChr,const int iLen)
{ //Desc: iWichSide indicates which side we will to . iLen ,filled string lenght
	if (!strlen (szOriginalStr)) return -1 ;
	if (iLen<strlen (szOriginalStr)) return  0 ;	//DO nothing ;
	
 	char szTmp [MAXCONTLEN] ;
	char b[MAXCONTLEN] ;
	
	memset(szTmp ,'\0',sizeof(szTmp));	
	memset (b ,cFillChr ,sizeof(b));
	
	if (AG_LEFT==iWichSide){
		strncpy (szTmp , b ,iLen-strlen (szOriginalStr));
		strcat(szTmp,szOriginalStr);		
		
	}else if (AG_RIGHT==iWichSide){
		strncpy (szTmp ,szOriginalStr ,strlen(szOriginalStr));
		strncpy (szTmp+strlen(szOriginalStr) ,b,iLen -strlen(szOriginalStr));
	}else return -1 ;
	
	strncpy (szFilledStr ,szTmp ,strlen (szTmp));

	return 0 ;
}
int LString (const char* szSource, const int iLength, char* szOutputStr)
{//We will copy the iLength characters from LEFT side of szSource.
	if (iLength<0 || szSource == NULL) 
		return -1;	
	if (iLength>= strlen (szSource)) {
		strncpy  (szOutputStr,szSource,strlen(szSource)) ;
		return  0 ;
	}
	
	memset (szOutputStr,'\0',sizeof(szOutputStr)) ;
	strncpy (szOutputStr,szSource,iLength) ;	
	return 0;
}
int RString (const char* szSource ,const int iLength ,char* szOutputStr)
{//Copy the iLength characters from RIGHT side of szSource.

	if (iLength >= strlen (szSource)){ //If the parameters you passed is greater then length of source,it iwll copy whole string to szOutputStr from szSource.same bas above.
		strncpy (szOutputStr,szSource,strlen(szSource)) ;
		return  0;
	}
	if (szSource == NULL || iLength <0) { //You have passed an invalid parameters.
		return -1; 
	}
	
	int iPos  = strlen(szSource)- iLength ; //The start pos of copying.
	strncpy (szOutputStr,szSource+ iPos,iLength) ;	
	return  0; 
}
int UpperCase (char* szString)
{ //Cast all characters of string into upper case , then restored in original string 
	if (!strlen(szString)|| szString == NULL)		
		return -1;
	

	char* sp = szString ;
	while (*sp!='\0'){
		if (*sp>=65&&*sp<=122)
			*sp= *sp - 32 ;
		sp ++ ;
	}
	return 0;	
}
int LowerCase (char* szString)
{//cast all characters of stirng into lower case , then restored in original string
	if (!strlen(szString)||szString== NULL )	
		return  -1;  	
	char* sp = szString ;
	while (*sp!='\0'){
		if (*sp>=65&&*sp<=122)
			*sp = *sp +32 ;
		sp ++ ;
	}
	return 0;
}
int Pos (const char cChar ,const char* szSource) 
{ //Return the first pos of occurrence of char .
	if (szSource == NULL || !strlen(szSource))
		return -1;
	char sb [10] ;
	bzero (sb,sizeof(sb));
	
	sprintf (sb,"%c",cChar) ;
	
	char* sp  = (char*)szSource ;	
	char* se = strstr (szSource,sb) ;
	if (se== NULL|| se==sp) return  0;
	return se-sp +1 ;
}

pthread_mutex_t mtx_getTerminalID  ;

int GetTerminalID (char* szOutput) 
{
	
	char szLogBuffer [1024] ;	
	bzero (szLogBuffer,sizeof(szLogBuffer)) ; //reflush the logbuffer .
	
	int iRet  = pthread_mutex_init (&mtx_getTerminalID,NULL) ;
	if (iRet) {
		sprintf (szLogBuffer,"[%s-line:%d]Can not initialize the mutext successfully[Err:%d],pls check your system...",__FILE__,__LINE__,iRet)  ;
			
		WriteLogFile (&szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;	
		pthread_mutex_destroy (&mtx_getTerminalID) ;
		return  -1 ;
	}; 
		
	
	char szBuffer  [1024*2] ;			
	bzero (&szBuffer,sizeof(szBuffer)) ; //reflush the buffer .	
	pthread_mutex_lock (&mtx_getTerminalID) ; //to hold the terminalid read from config
	
	read_config_file ("./sysconfig.cfg","TERMINAL",szBuffer) ;
	if (!strlen(szBuffer)) {
		
		bzero (szLogBuffer,sizeof(szLogBuffer)) ;
		sprintf (szLogBuffer,"[%s-line:%d]Can not read the terminal from config file, pls check your system...",__FILE__,__LINE__) ;	
		
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		pthread_mutex_unlock (&mtx_getTerminalID) ;	//Unlock the mutex .
		pthread_mutex_destroy (&mtx_getTerminalID) ;    //clean up 
		
		return -1 ;
		
	}
	char* szTerminalArr [200] ;
	int iTermArrLen = splitString  (szBuffer,szTerminalArr,';') ;
	
	if (iTermArrLen <= 0){
		pthread_mutex_unlock (&mtx_getTerminalID) ;
		return -1 ;
	}
	
	int j = -1 ; //To generate a random number between 0 and iTermArrLen ;
	j = 0 + (int) (iTermArrLen*1.0 * (rand() / (RAND_MAX + 0.0)));
	if (j>= iTermArrLen) 
		j  =  0 ; //IF the random number is greater than iTermArrLen, set j  to 0 ;
	
	sprintf (szOutput,"%s",szTerminalArr [j]); 	
	pthread_mutex_unlock (&mtx_getTerminalID) ; //Unlock 
	pthread_mutex_destroy (&mtx_getTerminalID) ; //clean up the resource .
	return  0 ;	
}













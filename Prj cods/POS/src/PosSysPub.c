/*************************************************************************************************
 *	Copyright : 
 *			All Reserved by lihao and palm co.
 *	Purpose:
 *			To encrypt the mac string and pin string,and All the function with bank mainframe.
 *	Version:
 *			Version 1 
 *	Change List:
 *			NONE.
 *
 *	Date:
 *			Nov 8 2008 
 *
 *
 *
 *************************************************************************************************/
#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Interface.h"
#include "../../PUBLIC/include/globalvars.h"

static int ParseBankStrings (const char* szRecvedBankStr , const char* szRetrieveStr , char** szOutputStr) ;
static int IsNeedField (int , int,char** )  ;


int Verify_Encrypt_MAC (const char* szStr,const char* szValue,char* szOutPutVal) 		 
{
	char szLogBuff [MAXCONTLEN] ;
	
	if (!strlen(szStr)||!strlen(szValue)){
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] Your have passed an invalid parameters [%s][%s]" ,__FILE__,__LINE__,szStr,szValue) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;
	}
	//Used for local variable .
	char sloutput [10] ;
	bzero  (sloutput,sizeof(sloutput));	
	sprintf (sloutput,"%s","-100") ; //Initialize with -100 ;
	
	char szMacStr [MAXCONTLEN] ;	//To stored the mac string from encrypt-machine.
	bzero (szMacStr,sizeof(szMacStr)) ;
	//Encrypt_str_Encrypt (szStr,strlen(szStr),szMacStr);
	//if (strcmp (szMacStr,"-1")){
			
	//}			
}
static int ConvertHexStrToStr (const char* szHexStr, char* szOutputStr) 
{
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff));
	
	
	if (!strlen(szHexStr)){
		bzero (szLogBuff,sizeof(szLogBuff));
		sprintf (szLogBuff,"You have passed an invalid HexStr [NUL]",__FILE__,__LINE__) ;	
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return  -1 ;
	}
	
	return 0 ;
}
static int ConvertStrToHexStr (const char* szBinStr,char* szOutputStr)
{
	char szLogBuff [MAXCONTLEN] ;
	bzero (szLogBuff,sizeof(szLogBuff)) ;
	
	if (!strlen(szBinStr)){
		bzero (szLogBuff,sizeof(szLogBuff)) ;
		sprintf (szLogBuff,"[%s-line:%d] You have passed an invalid BinStr [NULL]",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ;
	}

	return 0;
}

static int GetFieldValueByIndex (int iIndex , char* szOutput)
{ //To Retrieve the field length value from configuration file .
	char szFieldsTmp [512] ;
	bzero (szFieldsTmp,sizeof(szFieldsTmp)) ;
	bzero (szOutput,sizeof(szOutput)) ;
	
	read_config_file ("./sysconfig.cfg","FIELDS_VAL",szFieldsTmp) ;
	if (!strlen(szFieldsTmp))	
		return -1 ;
	char* sp = szFieldsTmp ;
	int iFieldCount =0 ;

	char* pstart ;
	char* pend ;
	pstart = szFieldsTmp ;
	pend = szFieldsTmp ;
	while (*sp!='\0'){
		if (*sp==';') {	
			iFieldCount ++ ;
		
			if (iFieldCount== (iIndex-1)) 
				pstart = sp ;
			if (iFieldCount == iIndex ) {
				pend  = sp ;
				if (iIndex==1)
					memcpy (szOutput,pstart,pend-pstart) ;
				else
					memcpy (szOutput,pstart+1,pend-pstart-1) ;
				break ;
			}
		}		
		sp++ ; //Fetch to the next bits
	}
	return 0;			
}
int CalBitMapValue (const char* szBitMapStr ,char* szBitMapVal)
{
		
	return 0;
}

int Parse_Getting_YL (const char* szYlstr , char* str1,char* str2 , char* macStr )
{//To parse the getting string from YL. So...
	
	char szLogBuff [MAXCONTLEN] ;
	memset  (szLogBuff,'\0',sizeof(szLogBuff)) ;
			
	if (!strlen(szYlstr)) {	
		bzero (szLogBuff,sizeof(szLogBuff));
		sprintf (szLogBuff,"[%s-line:%d] you have passed an invalid string into system..." , __FILE__,__LINE__) ;
		WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		WriteLogFileInHex_Ex (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;
		return  -1 ; 
	}
	
	bzero (szLogBuff,sizeof(szLogBuff)) ;
	sprintf (szLogBuff,"You have Get YL String....") ;
	WriteLogFile (szLogBuff,__SYS_TYPE_LOGFILE_RAW__) ;	
	WriteLogFileInHex_Ex (szYlstr,__SYS_TYPE_LOGFILE_RAW__) ;//Write the raw string of your had inputed	

	char szValueStr1 [MAXCONTLEN] ;
	bzero (szValueStr1,sizeof(szValueStr1)) ;	
	strncpy (szValueStr1,str1,sizeof(szValueStr1)) ;

	char szMacStr1 [MAXCONTLEN] ;
	bzero (szMacStr1,sizeof(szMacStr1)) ;
	char szMacValue [MAXCONTLEN] ;
	bzero (szMacValue,sizeof(szMacValue)) ;	
	
		
	bzero (str1,sizeof(str1)) ;
	bzero (str2,sizeof(str2));	
	int YL_Lens = strlen(szYlstr) ;
	
	memcpy (str2, szYlstr+0, 4);
	char szfjstr [MAXCONTLEN/2] ;
	memset (szfjstr,'\0',sizeof(szfjstr));	
	memcpy (szfjstr,szYlstr+5,8) ;
	int lens = 13; 
	unsigned int yhvalue ;
	memcpy (&yhvalue,szfjstr,1) ;
	int yh = 1 << 7 ; 	

	char szbitmap [MAXCONTLEN] ;
	memset (szbitmap,'\0',sizeof(szbitmap)) ;
	if ((yhvalue&yh) ==yh) {
		memcpy (szbitmap,szYlstr+5,16) ; //To retrieve the bitmap value.
		lens = 21 ;
	}
	int bit_len = strlen (szbitmap) ;	
	
	char szbinStr [129] ;
	char szBIN [9] ;
	
	bzero(szbinStr,sizeof(szbinStr)) ;	//To stored the bitmap value
	bzero(szBIN,sizeof(szBIN)) ;		//The first 8 bits of bitmap value .
			
	int i;
	int pvalue ;
	char szTmptmp [9] ;			//For temporay using .
	bzero (szTmptmp,sizeof(szTmptmp)) ;

	char szFieldStr_Len [MAXCONTLEN] ;
	memset (szFieldStr_Len,'\0',sizeof(szFieldStr_Len)) ;
	
		
	for (i  = 1 ;i<bit_len;i++){
		memcpy (&yhvalue,szfjstr+i-1,1) ;
		if (yhvalue==0) continue ;
		
		bzero (szbinStr,sizeof(szbinStr)) ;
		if (ConvertIntToStrOnBase(yhvalue,2,szbinStr)==-1)
			return -1 ;	
		pvalue =0 ;
		int j ;
		if (i==1) {
			bzero (szTmptmp,sizeof(szTmptmp)) ;
			RString (szBIN,7,szTmptmp) ;
			j = Pos ('1',szTmptmp) ;	
			pvalue = 1 ;
		}else
			j = Pos ('1',szBIN) ;		
		int vlens ;
		char szFieldValue [5] ;
		
		while (j!=0){
			j =+ pvalue ;
			vlens = (i -1) * 8 +j ; //Get pos of fields.
			
			bzero (szFieldValue,sizeof(szFieldValue));
			GetFieldValueByIndex (vlens-2,szFieldValue) ;
			if (!strlen(szFieldValue))
				return -1;
			int bc_len ;
			if (Pos('.',szFieldValue)==0){ //it's fixed fields.
				bc_len = atoi (szFieldValue) ;
				if (lens>YL_Lens) 
					return -1;
				bzero(szFieldStr_Len,sizeof(szFieldStr_Len)) ;
				memcpy (szFieldStr_Len,szYlstr+lens,bc_len) ; //Get context.
				if (!strlen(szMacStr1)){
					if (vlens==bit_len*8)
						strcpy (szMacValue,szFieldValue) ; //Get Mac value
					//sfdsfsif Pos(',')
				}
				

			}else{ //it's variable fields.
			}
		}
	}
	/*
	char szbinstr [128] ;
	memset(szbinstr,'\0',sizeof(szbinstr)) ;
	char szBIN [9] ;
	memset (szBIN ,'\0',sizeof(szBIN)) ;
	
	if (CovertIntToStrOnBase (yhvalue,2,szbinstr)==-1)
		return -1 ;
	Rstring (szbinstr ,8,szBIN) ;	
	int pvalue =0 ;
	*/			
	return 0;
}

static int PublicInfo_Set_BankReqBody (char* szBuffer, int iType)
{ //To settt the public information of the request body of bank peer.
	char szLogBuffer [MAXCONTLEN] ;
	
	if (szBuffer== NULL ) {
		bzero (szLogBuffer,sizeof(szLogBuffer)) ;
		sprintf (szLogBuffer,"[%s-line:%d] You have passed an invalid parameters,pls check your system ",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		WriteLogFileInHex_Ex (szBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		return  -1; 
	}
	
	if (iType<=0) return -1 ;
		
	char szTmp [20] ;
	bzero (szLogBuffer,sizeof(szLogBuffer));

	switch (iType) {
		case 1: { //To query amount of balace of bank card.
			memset (szTmp,'\0',sizeof(szTmp)) ;
			read_config_file ("./syscofig.cfg","BALANCE_AMOUNT_MSGTYPE",szTmp) ;	
			if (!strlen(szTmp)) 
				return -1 ;
				
			strncpy (((QUERY_BALANCE_REQ_BODY*) szBuffer)->szMessageTypeIdentifier,szTmp,strlen(szTmp));  //To set the message type id.
			memset (szTmp,'\0',sizeof(szTmp));
			read_config_file ("./sysconfig.cfg","BALANCE_AMOUNT_BITMAP",szTmp) ;
			if (!strlen(szTmp))
				return -1 ;
			strncpy (((QUERY_BALANCE_REQ_BODY*) szBuffer)->szBitMap,szTmp,strlen(szTmp)) ;//To sett the bit map value
			memset (szTmp,'\0',sizeof(szTmp));
			read_config_file ("./sysconfig.cfg","BALANCE_AMOUNT_PROCESSCODE",szTmp);
			if (!strlen(szTmp)) return -1; 
			strncpy (((QUERY_BALANCE_REQ_BODY*) szBuffer)->szProcessCode,szTmp,strlen(szTmp)) ;	//To set the process code from configfile .
			memset (szTmp,'\0',sizeof(szTmp));
			read_config_file ("./sysconfig.cfg","BALANCE_AMOUNT_SVRTYPE",szTmp); //To set serice type 012 .
			if (!strlen(szTmp)) return -1 ;
			strncpy (((QUERY_BALANCE_REQ_BODY*) szBuffer)->szServiceType,szTmp,strlen(szTmp));
			
			memset (szTmp,'\0',sizeof(szTmp));
			read_config_file ("./sysconfig.cfg","BALANCE_AMOUNT_SVRCONDID",szTmp) ;
			if (!strlen(szTmp)) return -1 ;
			strncpy (((QUERY_BALANCE_REQ_BODY*) szBuffer)->szConditionCodeOfService,szTmp,strlen(szTmp)) ;	
			memset (szTmp,'\0',sizeof(szTmp)) ;
			read_config_file ("./sysconfig.cfg","CURRENCY_TYPE",szTmp);	
			if (!strlen(szTmp)) return -1 ; //To set currency type of request body.
			strncpy (((QUERY_BALANCE_REQ_BODY*) szBuffer)->szCurrencyType,szTmp,strlen(szTmp)) ;			       
			//memset (szTmp,'\0',sizeof(szTmp)) ;		
			//read_config_file ("./sysconfig.cfg","MAC_VALUE",szTmp) ;		
			//if (!strlen(szTmp)) return -1 ;	
			memset (((QUERY_BALANCE_REQ_BODY*) szBuffer)->field_48,' ',sizeof(FIELD_48_QUERYBALANCE)) ;	
		        sprintf  (((QUERY_BALANCE_REQ_BODY*) szBuffer)->field_48[0].szLength,"%03d",sizeof(FIELD_48_QUERYBALANCE)-3); //to set the field_48's length
			
			
			
			bzero (szLogBuffer,sizeof(szLogBuffer)) ;
			sprintf (szLogBuffer,"To assemble the Request body of BALANCE_AMOUNT_REQ_BODY sucessfully...");
			WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
			
		} break ;
		case 2 :{ //payment: request body of payment.
			memset (szTmp,'\0',sizeof(szTmp));	
			read_config_file ("./sysconfig.cfg","PAYMENT_MSGTYPE",szTmp);	
			if (!strlen(szTmp)) return -1 ;	//To set payment message type .
			strncpy (((PAYMENT_REQ_BODY*)szBuffer)->szMessageTypeIdentifier,szTmp,strlen(szTmp)) ;
			
			memset (szTmp,'\0',sizeof(szTmp));	
			read_config_file ("./sysconfig.cfg","PAYMENT_BITMAP",szTmp);	
			if (!strlen(szTmp)) return -1 ;	//To set payment bitmap .
			strncpy (((PAYMENT_REQ_BODY*)szBuffer)->szBitMap,szTmp,strlen(szTmp)) ;
			
			memset (szTmp,'\0',sizeof(szTmp));	
			read_config_file ("./sysconfig.cfg","PAYMENT_PROCESSCODE",szTmp);	
			if (!strlen(szTmp)) return -1 ;	//To set process code .
			strncpy (((PAYMENT_REQ_BODY*)szBuffer)->szProcessCode,szTmp,strlen(szTmp)) ;
			
			memset (szTmp,'\0',sizeof(szTmp));	
			read_config_file ("./sysconfig.cfg","PAYMENT_SVRTYPE",szTmp);	
			if (!strlen(szTmp)) return -1 ;	//To set service input type .
			strncpy (((PAYMENT_REQ_BODY*)szBuffer)->szServiceInputType,szTmp,strlen(szTmp)) ;
				
			memset (szTmp,'\0',sizeof(szTmp));	
			read_config_file ("./sysconfig.cfg","PAYMENT_SVRCONDID",szTmp);	
			if (!strlen(szTmp)) return -1 ;	//To set service input condition code.
			strncpy (((PAYMENT_REQ_BODY*)szBuffer)->szConditionCodeOfService,szTmp,strlen(szTmp)) ;
				
			memset (szTmp,'\0',sizeof(szTmp));	
			read_config_file ("./sysconfig.cfg","CURRENCY_TYPE",szTmp);	
			if (!strlen(szTmp)) return -1 ;	//To set Currency type .
			strncpy (((PAYMENT_REQ_BODY*)szBuffer)->szCurrencyType,szTmp,strlen(szTmp)) ;
			
			memset (((PAYMENT_REQ_BODY*)szBuffer)->field_48,' ',sizeof(FIELD_48_PAYMENT)) ;
			sprintf (((PAYMENT_REQ_BODY*)szBuffer)->field_48[0].szLength,"%03d",sizeof(FIELD_48_PAYMENT)-3) ;
				
			bzero (szLogBuffer,sizeof(szLogBuffer)) ;
			sprintf (szLogBuffer,"To assemble  the body of  payment sucessfully....");	
			WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
					
		} break;
		case 3: { //To assemble the rollback message info.
			memset (szTmp,'\0',sizeof(szTmp)) ;	
			read_config_file ("./sysconfig.cfg","ROLLBACK_MSGTYPE",szTmp) ;
			if (!strlen(szTmp)) return -1 ;
			//To set message type id 
			strncpy (((ROLLBACK_REQ_BODY*)szBuffer)->szMessageTypeIdentifier,szTmp,strlen(szTmp)) ;
			
			memset (szTmp,'\0',sizeof(szTmp)) ;	
			read_config_file ("./sysconfig.cfg","ROLLBACK_BITMAP",szTmp) ;
			if (!strlen(szTmp)) return -1;
			//To set message BItmap
			strncpy (((ROLLBACK_REQ_BODY*)szBuffer)->szBitMap,szTmp,strlen(szTmp)) ;
				
			memset (szTmp,'\0',sizeof(szTmp)) ;	
			read_config_file ("./sysconfig.cfg","CURRENCY_TYPE",szTmp) ;
			if (!strlen(szTmp)) return -1;
			//To set message type id 
			strncpy (((ROLLBACK_REQ_BODY*)szBuffer)->szCurrencyType,szTmp,strlen(szTmp)) ;
			
			bzero (szLogBuffer,sizeof(szLogBuffer)) ;
			sprintf (szLogBuffer,"To assemble the rollback public information sucessfully...") ;
			WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;	
		}break; 
		case 4:{//To assemble check_in message .
			memset (szTmp,'\0',sizeof(szTmp));	
			read_config_file ("./sysconfig.cfg","CHECKIN_MSGTYPE",szTmp) ;
			if (!strlen(szTmp)) return -1;  //To assemble the message type.
			strncpy (((CHECKIN_REQ_BODY*)szBuffer)->szMessageTypeIdentifier,szTmp,strlen(szTmp)) ;
			
			memset (szTmp,'\0',sizeof(szTmp));	
			read_config_file ("./sysconfig.cfg","CHECKIN_BITMAP",szTmp) ;
			if (!strlen(szTmp)) return -1; 
			strncpy (((CHECKIN_REQ_BODY*)szBuffer)->szBitMap,szTmp,strlen(szTmp)) ;
				
			memset (szTmp,'\0',sizeof(szTmp));	
			read_config_file ("./sysconfig.cfg","CHECKIN_NETMNGCODE",szTmp) ;
			if (!strlen(szTmp)) return -1; 
			strncpy (((CHECKIN_REQ_BODY*)szBuffer)->szNetManageCode,szTmp,strlen(szTmp)) ;
			
			bzero (szLogBuffer,sizeof(szLogBuffer));
			sprintf (szLogBuffer,"To assemble the Check_in package successfully...");
			WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;	
		} break;
		case 5:{ //To apply reset the password .
			memset (szTmp,'\0',sizeof(szTmp)) ;
			read_config_file ("./sysconfig.cfg","APLRESETPWD_MSGTYPE",szTmp);
			if (!strlen(szTmp)) return -1 ; 
			//To set the message id 
			strncpy (((RESETPWD_REQ_BODY*)szBuffer)->szMessageTypeIdentifier,szTmp,strlen(szTmp)); 
			
			
			memset (szTmp,'\0',sizeof(szTmp)) ; //To set bitmap	
			read_config_file ("./sysconfig.cfg","APLYRESETPWD_BITMAP",szTmp) ;
			if (!strlen(szTmp)) return -1 ;
			strncpy (((RESETPWD_REQ_BODY*)szBuffer)->szBitMap,szTmp,strlen(szTmp)); 
			
			memset (szTmp,'\0',sizeof(szTmp)) ; //To set network manage code.	
			read_config_file ("./sysconfig","APLYRESETPWD_NETMNGCODE",szTmp) ;
			if (!strlen (szTmp)) return -1 ; 
			strncpy (((RESETPWD_REQ_BODY*)szBuffer)->szNetManageInfo,szTmp,strlen(szTmp)); 
			bzero (szLogBuffer,sizeof(szLogBuffer)) ;
			sprintf (szLogBuffer,"To assemble the RESET-PASSWORD package successfully...") ;
			WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;	
		} break;	
		case 6: { //To set the working password. 
			
		} break; 
		
		default :break; 
	}
	
	bzero (szLogBuffer,sizeof(szLogBuffer)) ; 
	sprintf (szLogBuffer,"[%s-line:%d You have set the public information of package successfull...]",__FILE__,__LINE__) ;	
	WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ; //To write the logfile of message.
	WriteLogFileInHex_Ex (szBuffer,__SYS_TYPE_LOGFILE_RAW__); //To write the raw message of packages.	
	return 0; 
}   

int Pos_SendMsg_QueryAmount_ToBank (char* szReqMsg,char* szResMsg,char* szMacStr,char* szPinStr,char* szOutput)
{
	char szLogBuffer [MAXCONTLEN] ; 
	bzero (szLogBuffer,sizeof(szLogBuffer)) ;
	if (szReqMsg == NULL || szResMsg == NULL || szMacStr == NULL || szPinStr == NULL ){
		bzero (szLogBuffer,sizeof(szLogBuffer));
		sprintf (szLogBuffer,"[%s-line:%d] You have just passed an invalid parameters, pls check your system...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__);
		return -1 ;
	}		
	CMD_BALANCEAMOUNTQURY_REQ query_amount_req ;
	memset (&query_amount_req,'\0',sizeof(query_amount_req));
	memcpy (&query_amount_req,szReqMsg,sizeof(query_amount_req)) ;

	QUERY_BALANCE_REQ_BODY query_req_body ;
	memset (&query_req_body,' ',sizeof(query_req_body)) ; //to reflush the buffer.		
	
	///To set the public information. 
	PublicInfo_Set_BankReqBody ((char*)&query_req_body,1) ;	
	
	//Here , we must to process bitmap value from config files .	
	strncpy (query_req_body.szSysTraceNumber,query_amount_req.szSysTraceNo,sizeof(query_req_body.szSysTraceNumber)) ;
	strncpy (query_req_body.szTransTimeOfLocal,query_amount_req.szLocalTransTime,sizeof(query_req_body.szTransTimeOfLocal)) ;
	strncpy (query_req_body.szTransDateOfLocal,query_amount_req.szLocalTransDate,sizeof(query_req_body.szTransDateOfLocal)) ;
	strncpy (query_req_body.szMerchantType,query_amount_req.szMerchantType,sizeof(query_req_body.szMerchantType));
	strncpy (query_req_body.szCardAcceptorInstitID,query_amount_req.szAcceptorInstitueID,sizeof(query_req_body.szCardAcceptorInstitID)) ;
	
	char szTmp [MAXCONTLEN] ;
	memset (szTmp,'\0',sizeof(szTmp));
	read_config_file ("./sysconfig.cfg","SENDER_INSTITID",szTmp);
	if (!strlen(szTmp)) return -1 ;
	strncpy (query_req_body.szSenderInstitID,szTmp,sizeof(query_req_body.szSenderInstitID)) ;	
	
	memset (szTmp,'\0',sizeof(szTmp));
	if (GetTerminalID(szTmp)||!strlen(szTmp))  {
		
		return -1 ;
	}
		
	strncpy (query_req_body.szTerminalID,szTmp,strlen(szTmp)) ;
	strncpy (query_req_body.szMerchantID,query_amount_req.szMerchantID,sizeof(query_req_body.szMerchantID)) ;
	memset (szTmp,'\0',sizeof(szTmp)) ;
	read_config_file ("./sysconfig.cfg","MAC_VALUE",szTmp) ;
	strncpy (query_req_body.szMACValue,szTmp,sizeof(query_req_body.szMACValue)) ; //To copy the mac value	
	char szSenderBuffer [MAXCONTLEN] ;
	memset (szSenderBuffer,'\0',sizeof(szSenderBuffer));	
	//Here ,we will cpy all message into senderbuffer ,then send it .
	
	int iFD,iRet ;
	iFD  = Pos_Socket_Init () ;
	if (iFD <=0 )  {
		bzero (szLogBuffer,sizeof(szLogBuffer)) ;
		sprintf (szLogBuffer,"[%s-line:%d] Can not create the socket handle successfully,pls check your system..." ,__FILE__,__LINE__) ;	
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__)  ;
		return -1 ;
	}		
	if ((iRet=Pos_Socket_Conn (iFD)) == 0) {
	
	} else{
		bzero (szLogBuffer,sizeof(szLogBuffer)) ;
		sprintf (szLogBuffer,"[%s-line:%d] Can not connect to socket server,pls check your system.",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		Pos_Socket_Close (iFD) ;	
		return -1 ;
	}
	
	//To copy the send buffer.
	MESSAGE_HEAD pos_head ;
	char szSendBuffer [MAXCONTLEN] ;
	memset (szSenderBuffer,'\0',sizeof(szSendBuffer)) ;
	
	int iStart  ; 
	iStart = 0 ;	
	
	memcpy ( szSenderBuffer ,query_req_body.szMessageTypeIdentifier ,sizeof(query_req_body.szMessageTypeIdentifier));
	iStart += sizeof(query_req_body.szMessageTypeIdentifier) ;

	char szBitMap [16] ;
	memset (szBitMap,'\0',sizeof(szBitMap)) ;
	read_config_file ("./sysconfig.cfg","BALANCE_AMOUNT_BITMAP",szBitMap) ;
	if (!strlen(szBitMap)) return -1 ;
	
	//To copy the bitmap to sender buffer .	
	memcpy (szSenderBuffer+iStart,szBitMap,16) ;
	iStart += 16 ; 	
	memcpy (szSenderBuffer+iStart,query_req_body.szProcessCode,41);
	iStart += 41 ;  // it includes, processcode until to szConditionCodeOfService
			
	sprintf (szBitMap,"%02d",strlen(query_req_body.szCardAcceptorInstitID)) ;	
	memcpy (szSenderBuffer+iStart, szBitMap,2) ; //the length
	iStart += 2 ;
	memcpy (szSenderBuffer+iStart , query_req_body.szCardAcceptorInstitID,strlen(query_req_body.szCardAcceptorInstitID)) ;
	iStart += strlen(query_req_body.szCardAcceptorInstitID) ;
	
	memset (szBitMap ,'\0',sizeof(szBitMap)) ;		
	sprintf (szBitMap,"%02d",strlen(query_req_body.szSenderInstitID)) ;
	memcpy (szSenderBuffer+ iStart , szBitMap,2) ; 
	iStart += 2 ; 		
	memcpy (szSendBuffer+ iStart ,query_req_body.szSenderInstitID,strlen(query_req_body.szSenderInstitID)) ;
	iStart  += strlen(query_req_body.szSenderInstitID) ;
	memcpy (szSenderBuffer+ iStart ,query_req_body.szTerminalID,23) ; //to cp 23 characters
	iStart += 23 ;  //include ,terminalid + merchant_id (23 characters )	
		
	memset (szBitMap,'\0',sizeof(szBitMap)) ;
	sprintf (szBitMap,"%03d",sizeof(FIELD_48_QUERYBALANCE));	
	memcpy (szSenderBuffer+ iStart ,szBitMap,3) ;	
	iStart +=3  ;
	
	memcpy (szSenderBuffer +iStart ,query_req_body.field_48,sizeof(query_req_body.field_48)) ;	
	iStart  += sizeof(FIELD_48_QUERYBALANCE) ;		
		
	memcpy (szSenderBuffer+ iStart ,query_req_body.szCurrencyType,11) ;
	iStart += 11 ;
							
	memset (&pos_head,'\0',sizeof(pos_head));
	sprintf (pos_head.BodyLength,"%04d",strlen(szSenderBuffer)); //to hold the length of following lens
	
	if ((iRet = Pos_Socket_Send (iFD , &pos_head,sizeof(pos_head))) == 0){
	//Send the message head successfully.	
		if ((iRet = Pos_Socket_Send (iFD,szSenderBuffer,strlen(szSenderBuffer))) == 0 ) {
			bzero (szLogBuffer,sizeof(szLogBuffer)) ;
			sprintf (szLogBuffer,"Send the head&body of Request of Query_AMOUNT successfully...") ;
			WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;	
			WriteLogFileInHex_Ex (&pos_head,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFileInHex_Ex (szSenderBuffer,__SYS_TYPE_LOGFILE_RAW__) ; //write the logfile .
			
			memset (&pos_head,'\0',sizeof(pos_head)) ;
			memset (szSenderBuffer,'\0',sizeof(szSenderBuffer)) ;
			
			bzero (szLogBuffer,sizeof(szLogBuffer)) ;	
			if ((iRet = Pos_Socket_Read(iFD,&pos_head,sizeof(pos_head))) == 0) {//read the response head
				int followlen = atoi (pos_head.BodyLength) ;
				if ((iRet = Pos_Socket_Read (iFD ,&szSenderBuffer,followlen)) == 0 ){
					//We use the szsenderBuffer to hold the response message as the sender message does.
					bzero (szLogBuffer,sizeof(szLogBuffer)) ;
					sprintf (szLogBuffer,"Read the head&body of reaponse successfully...") ;
					WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;	
					WriteLogFileInHex_Ex (&pos_head,__SYS_TYPE_LOGFILE_RAW__) ;
					WriteLogFileInHex_Ex (szSenderBuffer,__SYS_TYPE_LOGFILE_RAW__) ;	
				}else{ //can not read the respond body correctlly...
					sprintf (szLogBuffer,"can not read the respond body correctlly,pls check your system...") ;
					WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
					WriteLogFileInHex_Ex (szSenderBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
					Pos_Socket_Close (iFD) ;	
					return -1  ;
				}
			} else{ //can not read the respond head correctlly.
				sprintf (szLogBuffer,"can not read the respond head correctly,pls check your system...") ;
				WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
				WriteLogFileInHex_Ex  (&pos_head,__SYS_TYPE_LOGFILE_RAW__) ;
				Pos_Socket_Close (iFD) ;	
				return -1 ;
			}
			
		} else{ //can not send the request of body to server successfully...
			bzero (szLogBuffer,sizeof(szLogBuffer)) ;
			sprintf (szLogBuffer,"[%-line:%d] can not send the body of request to server successfully...",__FILE__,__LINE__); 
			WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ; 
			Pos_Socket_Close (iFD) ;	
			return -1 ;
		}
	}else { //ca not send the head successfully.
		bzero (szLogBuffer,sizeof(szLogBuffer)) ;
		sprintf (szLogBuffer,"[%s-line:%d] Can not send the head of request of query_amount to server sucessfully,pls check your system...",__FILE__,__LINE__)  ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		Pos_Socket_Close (iFD) ;	
		return -1 ;	
	} 	
		
	char szReturnFields  [128] ;
	bzero (szReturnFields,sizeof(szReturnFields)) ;
	read_config_file ("./sysconfig.cfg","BALANCE_RETURNFIELDS",szReturnFields);
	
	if (!strlen(szReturnFields)) {
		// can not read the return fields from files
		bzero (szLogBuffer,sizeof(szLogBuffer));
		sprintf (szLogBuffer,"[%s-line:%d] Can not read the return fields from configuration file,pls check your system...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		Pos_Socket_Close (iFD) ;	
		return -1;
	}
	char szOutFields [128][MAXCONTLEN] ;
	memset (szOutFields,'\0',sizeof(szOutFields)) ;			
	if (ParseBankStrings (szSenderBuffer, szReturnFields ,(char**) szOutFields) == 0 ){
		CMD_BALANCEAMOUNTQURY_RES  query_amount_res ;	
		memset (&query_amount_res,'\0',sizeof(query_amount_res)) ;
		
		//strncpy  (query_amount_res.szTransferDateTime,szOutFields[7]) ;			
		strncpy (query_amount_res.szSysTraceNo,szOutFields[11],strlen(szOutFields[11])) ;
		strncpy (query_amount_res.szLocalTransTime,szOutFields[12],strlen(szOutFields[12])) ;	
		strncpy (query_amount_res.szLocalTransDate,szOutFields[13],strlen(szOutFields[13])) ;	
		strncpy (query_amount_res.szSettlementDate,szOutFields[15],strlen(szOutFields[15])) ;
		strncpy (query_amount_res.szMerchantType,szOutFields [18],strlen(szOutFields[18])) ;	
		strncpy (query_amount_res.szAcquireInstID,szOutFields[32] ,strlen(szOutFields[32])) ;
		strncpy (query_amount_res.szSenderInstID,szOutFields [33],strlen(szOutFields[33])) ;
		strncpy (query_amount_res.szResponseCode,szOutFields[39] ,strlen(szOutFields[39])) ;
		strncpy (query_amount_res.szTerminalID,szOutFields [41],strlen(szOutFields [41])) ;
		strncpy (query_amount_res.szMerchantID,szOutFields[42],strlen(szOutFields[42])) ;
		strncpy (query_amount_res.szBalanceAmount,szOutFields[54],strlen(szOutFields[54])) ;				
		memcpy (szResMsg,&query_amount_res,sizeof(query_amount_res)) ;	 //to copy the response
	} else {
		Pos_Socket_Close (iFD) ;	
		return -1 ;
	}
				
	Pos_Socket_Close (iFD) ;	
	return 0 ;
}

int 
Pos_SendMsg_ReaplyKey_ToBank(char* szReqMsg, char* szResMsg ,char* szMacStr, char* szPinStr, char* szOutput) 
{ //To Send the request message of Re-apply the KEy to Bank, the get the response,and parse it.

	char* szLogBuffer = new (MAXCONTLEN) ;	
	if ( szLogBuffer== NULL ) 
		return  -1 ;
	bzero (szLogBuffer,sizeof(szLogBuffer)) ;	
	
	
	if (szLogBuffer)	
		delete (szLogBuffer) ;
	
	return  0; 
}

int 
Pos_SendMsg_Rollback_ToBank (char* szReqMsg ,char* szResMsg ,char* szMacStr, char *szPinStr, char* szOutput) 
{ //To send the rollback Request message to pos ,then receving the respond ,parsing it .
	
	char* szLogBuffer = NULL  ;
	szLogBuffer  = new (MAXCONTLEN) ;
	bzero (szLogBuffer,sizeof(szLogBuffer) );
	ROLLBACK_REQ_BODY rollback_req ;
	
	CMD_TRANSROLLBACK_REQ cmd_rollback_req ;
	bzero (&cmd_rollback_req,sizeof(cmd_rollback_req)) ;	
	
	memcpy (&cmd_rollback_req,szReqMsg,sizeof(CMD_TRANSROLLBACK_REQ)) ; //get the request message.
	memset (&rollback_req,' ',sizeof(ROLLBACK_REQ_BODY)) ; //to refulsh the request body of pos .
	
	PublicInfo_Set_BankReqBody ( (void*) &rollback_req , 3 );	
	
	strncpy (rollback_req.szPaymentMoney,cmd_rollback_req.szPaymentAmount,sizeof(rollback_req.szPaymentMoney)) ;	
	strncpy (rollback_req.szSysTraceNum,cmd_rollback_req.szSysTraceNo,sizeof(rollback_req.szSysTraceNum)) ;	
	strncpy (rollback_req.szTransTimeOfLocal,cmd_rollback_req.szLocalTransTime,sizeof(rollback_req.szTransTimeOfLocal)) ;
	
	strncpy (rollback_req.szTransDateOfLocal,cmd_rollback_req.szLocalTransDate,sizeof(rollback_req.szTransDateOfLocal)) ;
	
	strncpy (rollback_req.szCardAccepterInstitID,cmd_rollback_req.szAcquireInstID,sizeof(rollback_req.szCardAccepterInstitID)) ;
	
	strncpy (rollback_req.szSenderInstitID,cmd_rollback_req.szSenderInstID,sizeof(rollback_req.szSenderInstitID)) ;
	
	//strncpy (rollback_req.szTerminalID,cmd_rollback_req.szTerminalID,sizeof(rollback_req.szTerminalID)) ;
	char szTermID  [20]  ; //Get the terminal ID .
	bzero (szTermID,sizeof(szTermID)) ;
	GetTerminalID (szTermID) ;
	strncpy (rollback_req.szTerminalID,szTermID,strlen(szTermID)) ;
	
	sprintf (szLogBuffer,"Assembling the request body of rollback,then send it to pos...",__FILE__,__LINE__) ;
	WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
	WriteLogFileInHex_Ex (rollback_req,__SYS_TYPE_LOGFILE_RAW__) ;
	
	int iFD,iRet  ; 
	if ((iFD  = Pos_Socket_Init ()) != 0) // can not initialize the socket 
		return -1 ;
	
	if ((iRet  = Pos_Socket_Conn (iFD) ) != 0 ) {  //can not open the socket .
		Pos_Socket_Close (iFD) ;
		return -1 ; 
	}
	
	MESSAGE_HEAD pos_head ;  //to initialize the message head.
	bzero (&pos_head,sizeof(pos_head)) ;
	sprintf (pos_head.BodyLength,"%d",sizeof(rollback_req)) ;
	
	bzero (szLogBuffer,sizeof(szLogBuffer)) ;	
	sprintf (szLogBuffer,"Assembling the message head of pos successfully...") ;
	WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
	WriteLogFileInHex_Ex (&pos_head,__SYS_TYPE_LOGFILE_RAW__)  ;
	
	if (iRet = Pos_Socket_Send (iFD,&pos_head,sizeof(pos_head))!= sizeof(MESSAGE_HEAD)) {
		bzero (szLogBuffer,sizeof(szLogBuffer)) ;
		
		sprintf (szLogBuffer,"Can not send the message head to pos successfully...") ;	
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		WriteLogFileInHex_Ex (&pos_head,__SYS_TYPE_LOGFILE_RAW__) ;
		
		Pos_Socket_Close (iFD) ;// send the request body failed.
		return -1; 
	} else {
	
		if (iRet = Pos_Socket_Send (iFD,&rollback_req,sizeof(rollback_req))== sizeof(ROLLBACK_REQ_BODY)){
			bzero (szLogBuffer,sizeof(szLogBuffer)) ;
			sprintf (szLogBuffer,"Send the request message body to pos successfully..."); 	
			WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__);
			
			MESSAGE_HEAD res_head; 
			bzero (&res_head,sizeof(res_head));
			
			if ((iRet  = Pos_Socket_Read (iFD,&res_head,sizeof(res_head))) == sizeof(res_head)){
				int iFollowLen  = atoi (res_head.BodyLength) ;
				char szRecevedMsg [MAXCONTLEN] ;
				memset (szRecevedMsg,'\0',sizeof(szRecevedMsg)) ;
				
				if ((iRet = Pos_Socket_Read (iFD,szRecevedMsg,iFollowLen)) == iFollowLen){
					char szReturnFields  [128] ;
				        bzero (szReturnFields,sizeof(szReturnFields)) ;
				        read_config_file ("./sysconfig.cfg","ROLLBACK_RETURNFIELDS",szReturnFields);
				        if (!strlen(szReturnFields)) {
                                		 bzero (szLogBuffer,sizeof(szLogBuffer));
                                                 sprintf (szLogBuffer,"[%s-line:%d] Can not read the return fields from configuration file,pls check your system...",__FILE__,__LINE__) ;
                                                 WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
						 Pos_Socket_Close (iFD) ;
                                                 return -1;
                                        }
						
                                       char szOutFields [128][MAXCONTLEN] ;
                                       memset (szOutFields,'\0',sizeof(szOutFields)) ;
                                       if (ParseBankStrings (szRecevedMsg, szReturnFields ,(char**) szOutFields) == 0 ) {
                    				CMD_TRANSROLLBACK_RES cmd_rollback_res ; 
						bzero (&cmd_rollback_res,sizeof(cmd_rollback_res)) ;
						strncpy (cmd_rollback_res.szPaymentAmount,szOutFields[4],sizeof(cmd_rollback_res.szPaymentAmount)) ;
						strncpy (cmd_rollback_res.szSysTraceNo,szOutFields[11],sizeof(cmd_rollback_res.szSysTraceNo)) ;
						strncpy (cmd_rollback_res.szLocalTransTime,szOutFields[12],sizeof(cmd_rollback_res.szLocalTransTime)) ;
						strncpy (cmd_rollback_res.szLocalTransDate,szOutFields[13],sizeof(cmd_rollback_res.szLocalTransDate)) ;
						strncpy (cmd_rollback_res.szMerchantType,szOutFields[18],sizeof(cmd_rollback_res.szMerchantType)) ;
						strncpy (cmd_rollback_res.szAcquireInstID,szOutFields[32],sizeof(cmd_rollback_res.szAcquireInstID)) ;
						strncpy (cmd_rollback_res.szSenderInstID,szOutFields[33],sizeof(cmd_rollback_res.szSenderInstID)) ;
						strncpy (cmd_rollback_res.szResponseCode,szOutFields[39],sizeof(cmd_rollback_res.szResponseCode)) ;
						strncpy (cmd_rollback_res.szTerminalID,szOutFields [41],sizeof(cmd_rollback_res.szTerminalID)) ;
						strncpy (cmd_rollback_res.szMerchantID,szOutFields [42],sizeof(cmd_rollback_res.szMerchantID)) ;
						

						memcpy (szResMsg,&cmd_rollback_res,sizeof(CMD_TRANSROLLBACK_RES));
								
				        }else{
                                            	Pos_Socket_Close (iFD) ;
						return -1 ;
                                        }
                                } else {
					bzero (szLogBuffer,sizeof(szLogBuffer)) ;
					sprintf (szLogBuffer,"Read the response body from pos failed,pls check you system...") ;
					WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__);
					Pos_Socket_Close (iFD) ;
					return -1 ;
			      }
			}else{
				bzero (szLogBuffer,sizeof(szLogBuffer));
				sprintf (szLogBuffer,"Read the response message head from pos failed,pls check your system..."); 
				WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
				Pos_Socket_Close (iFD) ;
				return -1 ;
			}
				
		}else{ //can not send the message sucessfully....
			bzero (szLogBuffer,sizeof(szLogBuffer)) ;
			sprintf (szLogBuffer,"can not send the Request body to pos successfully,pls check your system...") ;
			WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
			WriteLogFileInHex_Ex (&rollback_req,__SYS_TYPE_LOGFILE_RAW__) ;
			Pos_Socket_Close (iFD) ;
			return -1 ;
		
		}
	}
	
	Pos_Socket_Close (iFD) ;	
	bzero (szLogBuffer,sizeof(szLogBuffer)) ;
	sprintf (szLogBuffer,"Send the the rquest messge to pos successfully, then close the socket...") ;
	WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		
	if (szLogBuffer)
		delete (szLogBuffer) ;
	return  0 ;
}
int Pos_SendMsg_CheckIn_ToBank (char* szReqMsg, char* szResMsg, char* szMacStr,char* szPinStr,char* szOutput)
{//Send the rquest message to bank server , then get the response message. 
 //No Mac Message is needed.
 //////////////////////////////////////////////////////////////////////////////
 
	
	char szLogBuffer [MAXCONTLEN] ;
	bzero (szLogBuffer,sizeof(szLogBuffer)) ;
	/* Don't remove these comments [Reserved]
	if (szReqMsg == NULL || szResMsg== NULL || szMacStr == NULL || szPinStr == NULL ) 
	{
		bzero (szLogBuffer,sizeof(szLogBuffer)) ;
		sprintf (szLogBuffer,"[%s-line:%d] You have just passed an invalid parameters,pls chck your system...",__FILE__,__LINE__) ; 
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1;  
	}	
	*/
	
	CHECKIN_REQ_BODY  chck_in_req ; //To pos
	CMD_MERCHANTSIGNIN_REQ merchant_sigin ; //To external interface.
	
	memset (&merchant_sigin,'\0',sizeof(merchant_sigin)) ; //To get originall request body.
	strncpy ((char*)&merchant_sigin,szReqMsg,sizeof(merchant_sigin)) ;
	memset (&chck_in_req,' ',sizeof(chck_in_req)) ;
	
	PublicInfo_Set_BankReqBody ((char*)&chck_in_req, 4 ) ; //To set the public information.
	
	strncpy (chck_in_req.szSystemTraceAuditNum,merchant_sigin.szSysTraceNo,strlen(chck_in_req.szSystemTraceAuditNum));
	
	strncpy (chck_in_req.szTransTimeOfLocal,merchant_sigin.szLocalTransTime,strlen(chck_in_req.szTransTimeOfLocal)) ; 
	strncpy (chck_in_req.szTransDateOfLocal,merchant_sigin.szLocalTransDate,strlen(chck_in_req.szTransDateOfLocal)) ;
	
	char szTmp [20] ;
	bzero (szTmp,sizeof(szTmp)) ;
	read_config_file ("./sysconfig.cfg","SENDER_INSTITID",szTmp) ;
	if (!strlen(szTmp)) return -1 ;  //Here ,we will get the sender institute id .
	sprintf (chck_in_req.szSenderInstituteID,"%02d%s",strlen(szTmp),szTmp) ; //to assemble the the senderid
	bzero (szTmp,sizeof(szTmp));
	if (GetTerminalID (szTmp)||!strlen(szTmp)){ //can not get available terminal ID .
		return -1; 
	} 		
			
	sprintf (szLogBuffer,"[%s-line;%d] It's setted the public information,it will send to svr...",__FILE__,__LINE__);
	WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
	WriteLogFile (&chck_in_req,__SYS_TYPE_LOGFILE_RAW__) ;	 //It's will write the raw files.
	
	int iFD ,iRet;
	if ((iFD = Pos_Socket_Init ()) != 0) { //can not create the pos socket handle successfully...
		return -1 ;
	}	
	
	if ((iRet = Pos_Socket_Conn (iFD)) != 0) { //Can not connect to server.it will exit with -1
		Pos_Socket_Close(iFD) ;
		return -1; 
	}
	
	MESSAGE_HEAD  msg_head ;
	memset (&msg_head,'\0',sizeof(msg_head));
	sprintf (msg_head.BodyLength,"%04d",sizeof(chck_in_req));	

	bzero (szLogBuffer,sizeof(szLogBuffer)) ;
	//Perhaps ,there are some improperly sending...
	if ((iRet = Pos_Socket_Send (iFD,&msg_head,sizeof(msg_head))) ==0 ){
		if ((iRet = Pos_Socket_Send (iFD,&chck_in_req,sizeof(chck_in_req)))== 0){
			sprintf (szLogBuffer,"Send the request head&body to server successfully...");
			WriteLogFile(szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ; 
		} else {
			sprintf (szLogBuffer,"Can not send the request body to server,pls check your sys...") ;
			WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		}
	} else{
		sprintf (szLogBuffer,"Can not send the rquest head to server successfully,pls check your sys...") ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
	}
		
	WriteLogFileInHex_Ex (&msg_head,__SYS_TYPE_LOGFILE_RAW__) ;//Write the raw log file.
	WriteLogFileInHex_Ex (&chck_in_req,__SYS_TYPE_LOGFILE_RAW__) ;	//Write the raw logfile .
	
	MESSAGE_HEAD rmsg_head;	//hold the response message from server .
	bzero (&rmsg_head,sizeof(rmsg_head));
	
	//CHECKIN_RES_BODY chck_res_body ;
	//bzero (&chck_res_body,sizeof(chck_res_body));
	
		
	int iFollowLen ;
	char szReadBuffer [MAXCONTLEN] ;
	memset (szReadBuffer,'\0',sizeof(szReadBuffer)) ;
	bzero (szLogBuffer,sizeof(szLogBuffer)) ;	
	if ((iRet  = Pos_Socket_Read (iFD,&rmsg_head,sizeof(rmsg_head))) == 0 ) {
		
		iFollowLen = atoi (rmsg_head.BodyLength) ;
		if ((iRet = Pos_Socket_Read (iFD ,&szReadBuffer,iFollowLen))== 0){ //Read the res from svr
			sprintf (szLogBuffer,"Read the response body from svr succesfully....") ; 	
			WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
						
		}else{ //To the response body error.
			sprintf (szLogBuffer,"Read the response body from svr errors occur,pls chck your sys....") ; 			
			WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;			
			WriteLogFileInHex_Ex (&rmsg_head,__SYS_TYPE_LOGFILE_RAW__);
			WriteLogFileInHex_Ex (&szLogBuffer,__SYS_TYPE_LOGFILE_RAW__)  ;
			Pos_Socket_Close(iFD) ;
			return -1 ;
		}
	} else { //Read the response head error .
		sprintf (szLogBuffer,"Read the response head errors occur,pls check yours sys....") ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;		
		WriteLogFileInHex_Ex (&rmsg_head,__SYS_TYPE_LOGFILE_RAW__);
		WriteLogFileInHex_Ex (&szLogBuffer,__SYS_TYPE_LOGFILE_RAW__)  ;
		return  -1 ; 
	}
	//Here ,we will analysis the the recieved messages.		
	
	char szReturnFields  [128] ;
	bzero (szReturnFields,sizeof(szReturnFields)) ;
	read_config_file ("./sysconfig.cfg","CHECKIN_RETURNFIELDS",szReturnFields);
	if (!strlen(szReturnFields)) {
		// can not read the return fields from files
		bzero (szLogBuffer,sizeof(szLogBuffer));
		sprintf (szLogBuffer,"[%s-line:%d] Can not read the return fields from configuration file,pls check your system...",__FILE__,__LINE__) ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		Pos_Socket_Close(iFD) ;
		return -1;
	}
	char szOutFields [128][MAXCONTLEN] ;
	memset (szOutFields,'\0',sizeof(szOutFields)) ;			
	if (ParseBankStrings (szReadBuffer, szReturnFields ,(char**) szOutFields) == 0 ){
		CMD_MERCHANTSIGNIN_RES merchant_sigin_res ;
		memset (&merchant_sigin_res , '\0' ,sizeof(merchant_sigin_res)) ; 
		strncpy (merchant_sigin_res.szTransDateTime,szOutFields[7],strlen(szOutFields[7])) ; //TransDT
		strncpy (merchant_sigin_res.szSysTraceNo,szOutFields[11],strlen(szOutFields[11]));   //SysTrac
		strncpy (merchant_sigin_res.szLocalTransTime,szOutFields[12],strlen(szOutFields[12]));			       strncpy (merchant_sigin_res.szLocalTransDate,szOutFields[13],strlen(szOutFields[13])) ;
		strncpy (merchant_sigin_res.szSenderID,szOutFields[33] ,strlen(szOutFields[33])) ;	
		strncpy (merchant_sigin_res.szCardAcceptorTermID,szOutFields[41],strlen(szOutFields[41])) ;
		strncpy (merchant_sigin_res.szResponseCode,szOutFields[39],strlen(szOutFields[39])) ;

		memcpy (szResMsg ,&merchant_sigin_res, sizeof(merchant_sigin_res)); 
	}else{
		Pos_Socket_Close(iFD) ;
		return -1 ;
	}
		
	Pos_Socket_Close (iFD) ;	
	return 0;
}
static int ParseBankStrings (const char* szRecvedBankStr , const char* szRetrieveStr , char** szOutputStr) 
{ // To parse the recieved string by bitmap values .	
	if (!strlen(szRecvedBankStr)|| !strlen(szRetrieveStr)) //to determine the whether is null string .
		return -1 ;
	
	char szFieldStr  [MAXCONTLEN] ;
	bzero (szFieldStr,sizeof(szFieldStr));
	
	char szLogBuffer [MAXCONTLEN] ;	
	
	read_config_file  ("./sysconfig.cfg","FIELDS_VAL",szFieldStr) ;	
	if (!strlen(szLogBuffer)) //can not get fields properly...
		return -1 ;
	
	char* szFieldLen [128] ;
	int iFieldNum = splitString (szFieldStr,szFieldLen ,';') ;
	if (iFieldNum !=128) {
		bzero (szLogBuffer,sizeof(szLogBuffer)); 	
		sprintf (szLogBuffer,"[%s-line:%d] The fields lenghts of pos perhaps is damaged..." ,__FILE__,__LINE__) ;
		WriteLogFile (szLogBuffer,__SYS_TYPE_LOGFILE_RAW__) ;
		return -1 ; 
	}
	
	char* szRetrieveField [128] ; //the field we need to retrieve from the banks.
	int iRetrievNum = splitString  (szRetrieveStr, szRetrieveField,';') ;	
	if (iRetrievNum <=0) return  -1 ;
	
	char bitmap [16] ;
	bzero (bitmap,sizeof(bitmap)) ;
	memcpy (bitmap,szRecvedBankStr,16) ; //To copy the bitmap value.
	
	char szBinStr [129] ; //here ,i will parse the bitmap value. [128th to hold '\0']
	memset (szBinStr,'\0',sizeof(szBinStr)) ; 	
	char szTmptmp [9] ;
	memset (szTmptmp,'\0',sizeof(szTmptmp)) ;
	int i ;	
	
	for (i =0 ;i <16 ; i++ ){
		ConvertTo8LenBinStr (bitmap[i],szTmptmp);
		strcat(szBinStr,szTmptmp);
	} //To get all binary string of bitmap value .
	
	
	int isVar ,iVarLens;	//isvar indicates whethere is variable fields, iVallens indicates the length ofvariables fields .
	char szOutputMsg [128][MAXCONTLEN] ;  //To hold the output message is need by us. 
	memset (szOutputMsg,'\0',sizeof(szOutputMsg)) ;
	
	int iStart = 20  ; //the startpoint of start, the first 1th field is message type (4bytes lens),then 2nd is bitmap (16 bytes). 

	char szvt [4] ; 
	
	for (i = 1 ; i < 128 ; i ++) { //here ,we will parse the strings.
		if (szBinStr[i]=='0') continue ;
			
		isVar =0  ;
		if (strstr (szFieldLen[i],".") == NULL ) { //this field is varible length.
			isVar = 1 ;
			iVarLens = atoi ((szFieldLen[i]+1)) ; //to get the lengths of variable fields.
		} else { //Fixed value .
			isVar =0 ;
			iVarLens = atoi (szFieldLen[i]) ;
		}
		/* we wil settle down this after executation.
		if (!IsNeedField (i,iRetrievNum,szRetrieveField )){ //this field is need to return.
				
		} 
		*/ 
		int iilen ;
		if (isVar){
			memcpy (szvt,szRecvedBankStr+iStart,iVarLens) ;
			iilen = atoi (szvt) ;  //var fields lengths.
			iStart += iVarLens ;  //the var fields lenght indications.
			memcpy (szOutputMsg [i], szRecvedBankStr + iStart ,iilen);
			iStart += iilen ;
			if (iStart > strlen(szRecvedBankStr)) { //error
				return -1; 
			}
		}else{ //if this fields is fixed lengths 
			memcpy (szOutputMsg [i], szRecvedBankStr+ iStart ,iVarLens);
			iStart += iVarLens;
		}
	}
	for (i =0 ;i < 128 ; i ++ ){
		strncpy (szOutputStr[i],szOutputMsg[i],strlen(szOutputMsg[i])) ;
	}		
	return 0;		
}

static int IsNeedField (int iIndex, int iNumOfArray ,char** szFiledArray) 
{
	int i ;
	int iFlag ;
	iFlag =0 ;
	
	iIndex -= 1 ; //We count field from 1 
	
	for (i = 0 ; i< iNumOfArray ; i++) {
		if (iIndex == atoi (szFiledArray[i])) {
			iFlag  = 1  ; 
			break ;
		}
	}
	return  iFlag ;	
}

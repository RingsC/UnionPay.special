/*****************************************************************************************
 *		Copyright:
 *				All rights reserved by lihao and Palm Co.
 *		Date:
 *				Nov 8 2008
 *
 *		Purpose:
 *				To include all head files need by source file .
 *		Change List:
 *				None.
 *
 *		Version:
 *				1.0
 *
 ******************************************************************************************/

#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__


	#include "../../PUBLIC/include/globalvars.h"
	#include <stdlib.h>
	#include <stdio.h>
	#include <strings.h>
	#include <string.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/select.h>

/*
static int sfd ;
static int Encrypt_Socket_Open () ; //To Open Encryption -machine.
static int Encrypt_Socket_Close (int); //To close opened socket handle .        
int Encrypt_Str_Encrypt (const char* ,int ,char*); //Send the message to encryption mechine.
static int CharToInt (char) ;   //Convert a char to integer.
static int isActive ; //The flag of connection 
*/

// The definition of Encryption used by Encryption-mechine.
//
//
// ///////////////////////////////////////////////////////////

	#define BANK_INDEX "001"
	#define JMJ_FILLCHR '0'

	typedef struct {
		char szML [2] ;			 //
		char szBank_Index [3] ;		//Index of bank
		char szBank_My [16];		//MY of bank
		char szMac_Length [3] ;		//Length of MAC
	} TMACCreate_In ;



	typedef struct {
		char szLength [2] ;	
		char szML [2] ;	
		char szError_Num [2] ;	
		char szMY [16] ;	
	} TMACCreate_Out ;
	
	typedef struct {
		char szML [2] ;
		char szPIN_Lx [1] ;	
		char szBank_Index [3] ;
		char sPIK_My [16] ;
		char szPIN_mw [16] ;	
	} TPINCreate_In ;

	typedef struct {
		char szML [2] ;
		char szError_Num [2] ;
		char szMY [16] ;
	} TPINCreate_Out ;

	typedef struct {
		char szML [2] ;
		char szBank_Index [3] ;
		char szMAK_Password [16] ;
		char szMAC [8] ;
		char szMAC_Length [3] ;	
	} TMACYZ_In ;

	typedef struct {
		char szLen [2] ;
		char szResponse [2] ;	
		char szError_Num [2] ;
	} TJMJ_OUT_2 ;


#endif

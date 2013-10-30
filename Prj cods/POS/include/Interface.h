/*****************************
* Copyright: All rights reserved by lihao and Palm Gaming info Co.,Ltd
* Author: Lihao 
* Purpose:
*	Define the interace protocols with Unionpay main-frame server 
*	by standard of Union Public Onlinepayment System.
* Date: Sep 16 ,2008 
* Change History:
*
*	Sep 16 ,2008 Created 
*	
*	Nov 12 2008  In the most of Package, the primary account is not need anymore.
*		
* B.T.W:	
* 	For the Chinese description ,Pls see the doucment of POS in Chinese.
* 	
******************************/

#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#define __WITHOUT_MERCHANT_NAME__

	typedef struct {
		char BodyLength [4] ; //length of following diagram.
	} MESSAGE_HEAD ;

	typedef struct {
		char szMessageTypeIdentifier [4] ;	//ID of Message Type .8020
		char szBitMap [16] ;			//BitMap value ;
		char szSystemTraceAuditNum [6] ;	//System Audit trace Number.
		char szTransTimeOfLocal [6] ;		//Local time of transaction. 
		char szTransDateOfLocal [4] ;		//Local Date of transaction.
		char szSenderInstituteID [13] ;		//ID of Sender institute.
		char szCardAcceptorTermID [8] ;		//ID of Terminal of card acceptor.
		char szNetManageCode [3] ;		//Net Management code //fixed: 161.
	} CHECKIN_REQ_BODY ;	

	typedef struct {
		char szLength [3] ;			//length  fixed: 048
		char szEncryptPINKey [8];		//Key of PIN ;
		char szEncryptPINVerifyKey[4];		//PIN_CHECK_VALUE ;
		char szEncryptMACkey[8] ;		//key of MAC ;
		char szEncryptMACVerifyKey [4] ;	//MAC_CHECK_VALUE 
	} FIELD_48_CHECKIN ;
	
	typedef struct {
		char szMessageTypeIdentifier [4] ;	//ID of Message Type.8030
		char szBitMap [16] ;			//BitMap 
		char szSystemTraceAduitNum [6] ;	//System Aduit trace Number 
		char szTransTimeOfLocal [6] ;		//Local Time of Transaction
		char szTransDateOfLocal [4] ;		//Local Date of Transaction.
		char szSenderInstituteID [11] ;		//ID of Sender Institute.
		char szResponseCode  [2] ;		//Response code 
		char szCardAccepterTermID [8] ;		//ID of Terminal of card Acceptor
		FIELD_48_CHECKIN  field_48[1] ;			//48 field .
		char szNetManageCode [3] ;		//Net Manage code //fixed value:161 	
	} CHECKIN_RES_BODY ;	

	

	typedef struct {
		char szLength [3] ;			//The length of Message Type.
		char szUsageType [2] ;			//Type of usage. Fixed value: CQ 
		char szTypeOfBill [2] ;			//Type of Bill 
		char szNumberOfBill  [20];		//Number of Bill
		char szTypeOfPayment [2] ;		//Type of Payment
		char szNumberOfTypeOfPayment [20]; 	//Number of Type Of Payment.
		char szCardNum [6] ;			//Number of Card or the last 6 num of Account
		char szBankIssueInstitID [11] ;		//Bank Issuing Instit ID .
		char szChannelID [15] ;			//Channel's ID .		 	
		char szChannelName [40] ;		//Channel's name or address of channel agent.
	} FIELD_48_QUERYBALANCE ;
		
	typedef struct {
		char szMessageTypeIdentifier [4] ;	//Type of Message :0200
		char szBitMap [16] ;			//Bit map value ; 
		char szProcessCode [6];			//Fixed value: 300000
		char szTransTransferDateTime [10] ;	//Transfer datetime of Transaction.
		char szSysTraceNumber  [6] ;		//System Trace Number .
		char szTransTimeOfLocal [6] ;		//Local Time of Transaction  .Format: hhmmss
		char szTransDateOfLocal [4] ;		//Local Date of Transaction  .Format: MMDD 
		char szMerchantType [4] ;		//Type of merchant 
		char szServiceType  [3]	;		//Type of service .012 No track2_3, No Pwd
							// 011 no Track2_3 , Have password .
		char szConditionCodeOfService[2] ;	//Condition code Of Service. Fixed value: 00
		char szCardAcceptorInstitID [11];	//Acceptor ID of transaction .
		char szSenderInstitID [11] ;		//Sender Institute ID .
		char szTerminalID [8] ;			//Terminal ID .
		char szMerchantID [15] ;		//Merchant ID .
		FIELD_48_QUERYBALANCE field_48[1];	//Field_48_VALUE
		char szCurrencyType [3];		//Currency Type ID: [156 RMB] .
		char szMACValue [8] ;			//MAC value from POS mainframe .
	} QUERY_BALANCE_REQ_BODY ;
	
	typedef struct {
		char szMessageTypeIdentifier [4] ;	//Type of Message . 0210
		char szBitMap  [16] ;			//Bit Map value 
		char szProcessCode  [6]	;		//Process Code Value: 300000;
		char szTransTransferDateTime [10] ;	//Transfer Datetime of Transaction.
		char szSysTraceNumber  [6];		//System Trace Number .
		char szTransTimeOfLocal [6] ;		//Local Time of Transaction . Format: hhmmss
		char szTransDateOfLocal [4] ;		//Local Date Of Transaction . Fromat : MMDD ;
		char szSettlementDate [4] ;		//Settlement data of transaction.
		char szMerchantType [4] ;		//Merchant type.
		char szConditionCodeOfService [2] ;	//Condition Code of Service : Fixed value : 00 ;
		char szCardAcceptorInstitID  [11] ;	//Card Acceptor Insstitue ID 	
		char szSenderInstitID [11] ;		//Send Institute ID 
		char szResponseCode [2] ;		//The Response Code . 00 .
		char szTerminalID  [8] ;		//The Terminal ID 
		char szMerchantID [15] ;		//Merchant ID ;
		FIELD_48_QUERYBALANCE field_48 [1] ;	//field 48 
		char szCurrencyType [3] ;		//Currency Code
		char szBalanceAmount [40] ;		//Amount of Balance	 
		char szMACValue  [8] ;			//MAC Value . if the respose code is 00 , (NOT NULL)
	} QUERY_BALANCE_RES_BODY ;
	
	
	typedef struct {
		char szLength [3] ;			//The length of field48.
		char szUageType  [2] ;			//Usage Type . CP consume. PA for signle pay. AG:Agent
		char szTypeOfBill [2] ;			//Type of BILL .
		char szNumberOfBill  [20] ;		//Number of BILL 
		char szTypeOfPayment [2] ;		//Type of PAYment .
		char szNumberOfPayment [20] ;		//Number of Payment.
		char szIndexOfPayment [10] ;		//Index of Payment .
		char szChannelID [15] ;			//Channel ID ;
		char szChannelName [40] ;		//Channel Name .
		char szChannelBusinessID [4] ;		//Business ID of channel.	
	} FIELD_48_PAYMENT ;	

	
	typedef struct {
		char szMessageTypeIdentifier [4] ;	//Message Type Indetifier :0200 
		char szBitMap [16] ;			//BitMap Value ;
		char szProcessCode [6] ;		//Proces code 
		char szPaymentMoney [12] ;		//Moeny Of Payment
		char szTransferDateTimeOfTrans [10] ;	//Transfer Datetime of Transaction.
		char szSysTraceNumber [6] ;		//System Trace Number .
		char szTransTimeOfLocal [6] ;		//Local Time of Transaction Format: hhmmss
		char szTransDateOfLocal [4] ;		//Local Date of Transaction Format: MMDD
		char szMerchantType [4] ;		//Merchant Type 
		char szServiceInputType [3] ;		//Input type fo service : 012 without track_23, No pwd
							//	011 without track_23, have pwd
		char szConditionCodeOfService [2] ;	//Condition code of servce  .fixed value 81
		char szCardAcceptorInstitID [11] ;	//Card Acceptor Instit ID 
		char szSenderInstitID  [11] ;		//Sender Instititue ID 
		char szTerminalID  [8] ;		//Terminal ID
		char szMerchantID  [15] ;		//Merchant ID .
		FIELD_48_PAYMENT field_48 [1] ;		//field 48
		char szCurrencyType  [3] ;		//Type of Currency 156 RMB
		char szMACValue  [8] ;			//MAC Value  .
	} PAYMENT_REQ_BODY ;	

		
	typedef struct {	
		char szMessageTypeIdentifier [4] ;	//Message Type Identifier .
		char szBitMap [16] ;			//Bit map value .
		char szProcessCode  [6];		//Process Code: fixed value: 190000
		char szpaymentMoney [12] ;		//Money of Payment.
		char szTransferDateTimeOfTrans [10] ;	//Transfer Datetime of Transaction.
		char szSysTraceNumber [6] ;		//System Trance Number .
		char szTransDateOfLocal [6] ;		//Local Date of Transaction.
		char szTransTimeOfLocal [4] ;		//Local Time of Transaction.
		char szSettlementDate [4] ;		//Settlement Datetime.
		char szConditionCodeOfService [2] ;	//Condition Code of Service : fixed value:81 ;
		char szCardAcceptorInstitID [11] ;	//Card accpetor Instit ID.
		char szSenderInstitID [11] ;		//Sender Institute ID ;
		char szResponseCode [2] ;		//response code .
		char szTerminalID [8] ;			//Terminal Code .
		char szMerchantID [15] ;		//Merchant Type ID.
		FIELD_48_PAYMENT field_48 [1] ;		//field 48
		char szCurrencyType [3] ;		//Currency type fixed value: 156 RMB.
		char szMACValue [8] ;			//MAC Value ;
	} PAYMENT_RES_BODY ;
			
	typedef struct { //same as original package.
		char szLength [3] ;			//The length of field48.
		char szUageType  [2] ;			//Usage Type . CP consume. PA for signle pay. AG:Agent
		char szTypeOfBill [2] ;			//Type of BILL .
		char szNumberOfBill  [20] ;		//Number of BILL 
		char szTypeOfPayment [2] ;		//Type of PAYment .
		char szNumberOfPayment [20] ;		//Number of Payment.
		char szIndexOfPayment [10] ;		//Index of Payment .
		char szChannelID [15] ;			//Channel ID ;
		char szChannelName [40] ;		//Channel Name .
		char szChannelBusinessID [4] ;		//Business ID of channel.	
	} FIELD_48_ROLLBACK ;
	
	typedef struct {
		char szMessageTypeIdentifier [4] ;      //Mesage Type Identifier.
		char szBitMap  [16] ;			//Bit Map Value;
		char szProcessCode [6] ;		//Originail Process Code of transaction.
		char szPaymentMoney [12] ;		//Payment Money .
		char szSysTraceNum [6] ;		//Ssystem trace Number .
		char szTransTimeOfLocal [6] ;		//Trans Time of Local 
		char szTransDateOfLocal [4] ;		//Trans Date of Local 
		char szMerchantType [4] ;		//Merchant Type ;
		char szConditionCodeOfService [2] ;	//Condition Code of Service .same as original trans.
		char szCardAccepterInstitID  [11] ;	//Card Acceptor Instit ID.
		char szSenderInstitID  [11] ;		//Sender Instit ID .
		char szTerminalID [8] ;			//Terminail ID .
		char szMerchantID [15] ;		//Merchant TYpe .
		FIELD_48_ROLLBACK field_48[1] ;		//feild 48
		char szCurrencyType  [3] ;		//currency type same as original transaction.
		char szMACValue [8] ;			//MAC value .	
	} ROLLBACK_REQ_BODY ;		
	

	typedef struct {
		char szMessageTypeIdentifier [4] ;	//Message Type identifier.
		char szBitMap [16] ;			//BitMap value.
		char szProcessCode [6] ;		//ProcessCode .
		char szPaymentMoney [12] ;		//Payment money.
		char szSysTraceNumber [6] ;		//System trace Number. 
		char szTransDateOfLocal [6] ;		//Trans Date of Local .
		char szTransTimeOfLocal [4] ;		//Trans Time of Local .
		char szSettlementDate [4] ;		//Date of Settlement of transaction,
		char szMerchantType  [4] ;		//Type of merchant .
		char szConditionCodeOfService[2] ;	//Condition Code of Service .	
		char szCardAcceptorInstitID [11] ;	//Card acceptor Instit ID .
		char szSenderInstitID [11] ;		//Sender Instit ID.
		char szResponseCode [2] ;		//Response code .
		char szTerminalID [8] ;			//Terminal ID .
		char szMerchantID [15] ;		//MerchantTYpe .
		FIELD_48_ROLLBACK field_48[1] ;		//field 48
		char szCurrencyType [3] ;		//Currency type : 156 RMB
		char szMACValue [8] ;			//MAC value .
	} ROLLBACK_RES_BODY ;
		
	
	typedef struct {
		char szMessageTypeIdentifier [4] ;	//Message Type identifier 
		char szBitMap [16] ;			//Bit Map value .
		char szDateTimeOfTrans [10] ;		//Date Time of Trans.
		char szSysTranceNumber [6] ;		//System trace number .
		char szSenderInstitID [11] ;		//Sender Instit ID.
		char szSecurityInfo [16] ;		//Net security Control Message. 
		char szNetManageInfo [3] ;		//Net Manage Infor. 
	} RESETPWD_REQ_BODY ;
	
	typedef struct {
		char szMessageTypeIdentifier [4] ;	//Message Type Identifier.
		char szBitMap [16] ;			//Bit map
		char szDateTimeOfTrans [10] ;		//Date Time of Trans.
		char szSysTraceNumber [6] ;		//System Trace Number. 
		char szSenderInstitID [11] ;		//
	} RESETPWD_RES_BODY ;
	
#endif	

/*Copyright by :All rights reserved by lihao and Shandong palm Info co.,ltd.
 *Purpose: The global variables definitions& to define the external intefaces protocol .
 *Author: lihao (mixtrue@hotmail.com)
 *Version:0.1
 *Change History: 
 *		Sep 8 ,2008 Created 
 *
 *
 */

#ifndef __DEF_GLOBALVARS_H__

#define __DEF_GLOBALVARS_H__
#define __DEF_USEMSGAUHTOR__OFF_
#define _FILENAME_ __FILE__
#define _LINENUM_  __LINE__


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXCONTLEN	256
#define AG_LEFT 	1 
#define AG_RIGHT 	2
///////////////The Command Words///////////////
#define _PUB_CMD_REAPPLYKEY_REQ_	"0820"
#define _PUB_CMD_REAPPLYKEY_RES_	"0821"

#define _PUB_CMD_MERCHANTSIGNIN_REQ_	"0830"
#define _PUB_CMD_MERCHANTSIGNIN_RES_	"0831"

#define _PUB_CMD_BALANCEAMOUNTQURY_REQ_	"0200"
#define _PUB_CMD_BALANCEAMOUNTQURY_RES_	"0201"

#define _PUB_CMD_TRANSDETAILSQURY_REQ_	"0300"
#define _PUB_CMD_TRANSDETAILSQURY_RES_	"0301"

#define _PUB_CMD_PAYMENT_REQ_		"0400"
#define _PUB_CMD_PAYMENT_RES_		"0401"

#define _PUB_CMD_CHANGEBUSINESSPWD_REQ_	"0500"
#define _PUB_CMD_CHANGEBUSINESSPWD_RES_ "0501"

#define _PUB_CMD_NETMANGEMENT_REQ_	"0600" 
#define _PUB_CMD_NETMANGEMENT_RES_	"0601" 

#define	_PUB_CMD_TRANSROLLBACK_REQ_	"0700"
#define	_PUB_CMD_TRANSROLLBACK_RES_	"0701"
///////////////The Command Words///////////////

///////////////////////FILE LOG TYPE ///////////
#define __SYS_TYPE_LOGFILE_RAW__	1024	//Raw LOG
#define __SYS_TYPE_LOGFILE_ERR__	1025	//ERR FILE LOG
#define __SYS_TYPE_LOGFILE_SQL__	1026	//SQL FILE LOG 


#define new (void*)malloc  
#define delete free 

//#define  gMaxFdCounts = 1000 ;		//The maxmium size of SOCKETs

typedef struct {
		char szSerialID  [32] ; //流水编号
		char szCommandID [8];   //命令字
		char szTimeStamp [20] ;	//时间戳
		char szFollowLen [5]  ; //后续报文长度
		char szReserved  [8] ; 	//预留字段	
	} CMD_MSG_HEAD;

typedef struct{
		char szTransDateTime [11] ; //传输时间
		char szSysTraceNo [7] ;	    //系统跟踪号
		char szSenderID [12] ;	    //发送方编号
		char szPassword [7] ;	    //原始密码
		#ifdef __DEF_USEMSGAUTHOR__
			char szMsgAuthor[64] ;	//消息完整性验证码	
		#endif 				
	}CMD_REAPPLYKEY_REQ;

typedef struct {
		char szTransDateTime  [11] ;	//系统传输时间
		char szTraceNo	[7];		//系统跟踪号
		char szSenderID [12] ;		//发送方编号
		char szResponseCode [3]	;	//响应码
		#ifdef __DEF_USERMSGAUTHOR__	
			char szMsgAuthor[64];	//消息完整性验证码
		#endif 
	} CMD_REAPPLYKEY_RES;

typedef struct {
		char szTransDateTime  [11] ;	//系统传输时间
		char szSysTraceNo [7]; 		//系统跟踪编号
		char szLocalTransTime [7] ;	//系统本地交易时间
		char szLocalTransDate [5] ;	//本地交易时间
		char szSenderID [12];		//发送机构编号 //not be used,we get it from config file on svr.
		char szCardAccptTermID [9] ;	//收卡机构终端编号
		#ifdef __DEF_USERMSGAUTHOR__	
			char szMsgAuthor [64] ;	//消息完整性验证
		#endif 	
	} CMD_MERCHANTSIGNIN_REQ; 	

typedef struct {
		char szTransDateTime [11] ;	//系统传输时间
		char szSysTraceNo [7] ;		//系统跟踪编号
		char szLocalTransTime [7] ;	//本地交易时间
		char szLocalTransDate [5] ;	//本地交易日期
		char szSenderID	[12] ;		//发送方编号
		char szCardAcceptorTermID [9];  //受卡机构终端编号
		char szResponseCode [3] ;	//响应编码
		#ifdef __DEF_USEMSGAUTHOR__
			char szMsgAuthor[64] ;	//消息完整性验证码
		#endif
	} CMD_MERCHANTSIGNIN_RES ;
	
typedef struct {
		char szTransferDateTime [11] ;	//系统传输时间
		char szSysTraceNo [7] ;		//系统跟踪编号
		char szLocalTransTime [7] ;	//本地交易时间
		char szLocalTransDate [5] ;	//本地交易日期
		char szMerchantType [5];	//商户类型
		char szAcceptorInstitueID [12] ;	//受理机构代码
		char szSenderInstitueID [12] ;		//发送机构代码
		char szTerminalID [9] ;		//终端号码
		char szMerchantID [9] ;		//商户编号
		char szPassword [7] ;		//用户密码
		char szIDCard [19] ;		//身份证号码
		char szTelNum [13] ;		//用户联系电话
		#ifdef __DEF_USEMSGAUTHOR__	
			char szMsgAuthor [64] ;	
		#endif 	
	} CMD_BALANCEAMOUNTQURY_REQ ;

typedef struct {
		char szTransferDateTime [11] ;	//传输时间
		char szSysTraceNo [7] ;		//系统跟踪编号
		char szLocalTransTime [7] ;	//本地交易时间
		char szLocalTransDate [5] ;	//本地交易日期
		char szSettlementDate [5] ;	//清算日期
		char szMerchantType [5]	 ;	//商户类型
		char szAcquireInstID [12] ;	//受理机构编号
		char szSenderInstID  [12] ;	//发送机构编号
		char szResponseCode [3] ;	//响应码
		char szTerminalID [9] ;		//终端编号
		char szMerchantID [16]; 	//商户编号
		char szBalanceAmount [41] ;	//账户余额
		#ifdef __DEF_USEMSGAUTHOR__
			char szMsgAuthor [64] ;	//消息完整性验证码
		#endif 
	} CMD_BALANCEAMOUNTQURY_RES ;
typedef struct {
		char szTransferDateTime [11] ;	//传输时间
		char szSysTraceNo [7] ;		//系统跟踪编号
		char szLocalTransTime [7] ;	//系统本地交易时间
		char szLocalTransDate [5] ;	//系统本地交易日期
		char szMerchantType  [5] ;	//系统商户类型
		char szMerchantID  [16] ;	//商户编号
		char szAcquireInstID [12] ; 	//受理机构编号
		char szSenderInstID  [12] ;	//发送机构代码
		char szPassword [7] ;		//密码
		char szTerminalID  [9] ;	//终端编号
		char szIDCard [19] ;		//身份证编号
		char szTelNum [13] ;		//联系电话
		#ifdef __DEF_USEMSGAUTHOR__	
			char szMsgAuthor [64] ;	//消息完整性验证编号
		#endif 	
	} CMD_TRANSDETAILSQURY_REQ;

typedef struct {
		char szDetails [200] ;		//单笔明细
	}  TRANSDETAILITEM ;

typedef struct {
		char szTransferDateTime [11] ;	//传输时间
		char szSysTraceNo [7] ;		//系统跟踪编号
		char szLocalTransDate [7] ;	//本地交易日期
		char szLocalTransTime [5] ;	//本能交易时间
		char szSettlementDate [5] ;	//清算日期
		char szMerchantType [5] ;	//商户类型
		char szAcquireInstID [12] ;	//受理机构代码
		char szSenderInstID  [12] ;	//发送机构代码
		char szResponseCode  [3] ;	//响应代码
		char szTerminalID  [9] ;	//终端代码
		char szMerchantID [16] ;	//商户代码
		char szBalanceAmount [41] ;	//余额
		char szAcceptorInstID [12] ;	//受理机构编号
		char szDetailsCount [5] ;	//交易明细数据个数
		TRANSDETAILITEM  strTransDetails[3] ;//交易明细
		#ifdef __DEF_USEMSGAUTHOR__
			char szMsgAuthor [64] ;	//消息完整性验证码
		#endif 
	} CMD_TRANSDETAILSQURY_RES ;
typedef struct {
		char szIDCard [19] ;		//身份证编号
		char szTelNum [13] ;		//用户手机号码 
		char szSysTraceNo [7] ;		//系统跟踪编号
		char szPaymentAmount [13] ;	//交易金额
		char szLocalTransTime [7] ;	//本地交易时间
		char szLocalTransDate [5] ;	//本地交易日期 
		char szMerchantType [5] ;	//商户类型
		char szAcquireInstID [12] ;	//受理机构编号
		char szSenderInstID  [12] ;	//发送机构编号
		char szTerminalID [9] ;		//终端编号
		char szMerchantID [16] ;	//商户编号
		char szPassword [7] ;		//交易密码
		#ifdef __DEF_USEMSGAUTHOR__ 
			char szMsgAuthor [64];	//消息完整性验证码
		#endif 
				
	} CMD_PAYMENT_REQ;
typedef struct  {
		char szPaymentAmount  [13] ;	//交易金额
		char szSysTraceNo [7] ;		//系统跟踪编号
		char szLocalTransTime [7] ;	//本地交易时间
		char szLocalTransDate [5] ;	//本地交易日期
		char szSettlementDate [5] ;	//清算日期
		char szMerchantType [5]	;	//商户类型
		char szAcquireInstID [12] ;	//受理机构编号
		char szSenderInstID  [12] ;	//发送机构编码
		char szResponseCode [3]	;	//返回码
		char szTerminalID [9] ;		//终端编号
		char szMerchantID [16] ;	//商户编号
		#ifdef __DEF_USEMSGAUTHOR__
			char szMsgAuthor [64] ;	//消息完整性验证
		#endif 
	} CMD_PAYMENT_RES;		
 	

typedef struct {
		char szTransferDateTime [11] ;	//传输时间
		char szSysTraceNo [7] ;		//系统跟综编号
		char szTransTime  [7] ;		//交易时间
		char szTransDate [5] ;		//交易日期
		char szMerchantType [5];	//商户类型
		char szAcquireInstID [12] ;	//受单行编号
		char szSenderInstID [12] ;	//发送方编号
		char szTerminalID [9] ;		//终端编号
		char szMerchantID [16] ;	//商户编号
		char szPassword [14] ;		//用户密码(格式:旧密码＋新密码)
		#ifdef __DEF_USEMSGAUTHOR__
			char szMsgAuthor [64] ;	//消息完整性验证码
		#endif 
	}CMD_CHANGEBUSINESSPWD_REQ ;

typedef struct {
		char szTransferDateTime [11] ;	//传输时间
		char szSysTraceNo  [7] ;	//系统跟踪编号
		char szTransTime [7] ;		//交易时间
		char szTransDate [5] ;		//交易日期
		char szMerchantType [5] ;	//商户类型
		char szAcquireInstID [12] ;	//收单行机构编号
		char szSenderInstID [12] ;	//发送机构编号
		char szSysIndexOfRefNo [13] ;	//系统检索参考号
		char szResponseCode [3] ;	//响应码
		char szTerminalID [9] ;		//终端编号
		char szNewPassword [7] ;	//新密码
		#ifdef __DEF_USEMSGAUTHOR__	
			char szMsgAuthor [64] ;	//消息完整性验证码
		#endif 
	} CMD_CHANGEBUSINESSPWD_RES ;

typedef struct {
		char szTransferDateTime [11] ;	//传输时间
		char szSysTraceNo [7] ;		//系统跟综编号
		char szSenderInstID [12] ;	//发送机构编号
		#ifdef __DEF_USERMSGAUTHOR_
			char szMsgAuthor [64] ;	//系统消息完整性验证码_
		#endif 
	} CMD_NETMANAGEMENT_REQ ;

typedef struct {
		char szTransferDateTime[11] ;	//系统传输时间
		char szSysTraceNo [7] ;		//系统跟综编号
		char szSenderInstID [12] ;	//发送机构编号
		char szResponseCode [3] ;	//响应编码
		#ifdef __DEF_USEMSGAUTHOR__
			char szMsgAuthor [64] ;	
		#endif 
	} CMD_NETMANAGEMENT_RES ;

typedef struct {
		char szPaymentAmount [13] ;	//交易金额
		char szSysTraceNo [7] ;		//系统跟踪编号
		char szLocalTransTime [7] ;	//系统本地交易时间
		char szLocalTransDate [5] ;	//本地交易日期
		char szMerchantType [5] ;	//商户类型
		char szAcquireInstID [12] ;	//受单行机构编码
		char szSenderInstID [12] ;	//发送机构编码
		char szTerminalID [9] ;		//终端编号
		char szMerchantID [16] ;	//商户编号
		char szOriginalTransID [32] ;	//原始交易流水编号	
		#ifdef __DEF_USEMSGAUTHOR__
			char szMsgAuthor [64] ;	//消息完整性验证码
		#endif 
	} CMD_TRANSROLLBACK_REQ;

typedef struct {
		char szPaymentAmount [13] ;	//交易金额
		char szSysTraceNo [7] ;		//系统跟踪编号
		char szLocalTransTime [7] ;	//本地交易时间
		char szLocalTransDate [5] ;	//本地交易日期
		char szSettlementDate [5] ;	//清算日期
		char szMerchantType [5] ;	//商户类型
		char szAcquireInstID [12] ;	//受理机构代码
		char szSenderInstID [12] ;	//发送机构代码
		char szResponseCode [3] ;	//响应码
		char szTerminalID [9] ;		//终端编号
		char szMerchantID [16] ;	//商户编号
		char szOriginalTransID [32] ;	//原始交易流水编号
		#ifdef __DEF_USEMSGAUTHOR__
			char szMsgAuthor [64] ;	//消息完整性验证码
		#endif 
	} CMD_TRANSROLLBACK_RES ;


///////////////////////////////////////////////////////
	int g_SeedID  ;			//Global seed ;
	int g_ListenSktHandle ; 	//Listening socket handle 
	int gb_ListeningFlag ; 		//Flag of Listening socket 
	pthread_t gb_ListenThread ;	//To holde the listening socket handle .
		
#endif


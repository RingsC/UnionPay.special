**************************************************************************************************************
*													     *
*													     *
*													     *
*	IMPORTANT INFORMATION ABOUT THIS PROJECT:							     *
*		THIS PROJECT GOALS ARE TO PROVIDE SERVICES FOR OUR CURSTOMER WHO WANT SHOPPING ON-LINE.	     *
*													     *
*													     *
**************************************************************************************************************


Main functions of each folder:
	COMMUN
		: to store the head files and source files,which is used to communication with remote server,mainly, with external server. such as ,third parties company. In these folder, i provide some interface protocols,which can be used by third party company.
		[In parent folder, there are 'include' and 'src' two subfolders.]
		
	ENCRYPT  
		: which is used for communicating with encrypt-machine. The functions of these files to get MAC,PIN value . It's a hardware encryption.
		[In parent folder, there are 'include' and 'src two'  subfolders.]
	LOG 
		: The one of important factors we should consider carefully is providing a way to track all transactions we have done. So, the transaction logs will play a very import role in this issue. In this system, I provide three types logfile. As following descripted: 
			
		[The First One]: 
				RAW LOGFILE . Which is to store the raw messages which are sent or read from remote host,and all system information shown on screen if the shown-flag is switched on.
				The extend name of this type is 'raw', Everyday, we generate a file is named after the format of 'date.raw'. Such as, "20080808.raw"
		[The Second ONE]: 
				ERROR LOGFILE. Which is to store the exception messages raised by system. The naming method same as  description above.But, the extend formation is not same as above,it's named with 'err'.For an instance, "20080808.err"
		
		[The Third ONE]:
				SQL LOGFILE. which is stored the sql-statements, and all about datebase operations.This type logfile is with 'sql' extend file name.For an instance, "200808.sql"
				
	MAIN  
			
	MAKE  
	
	POS  
	
	PUBLIC 
	
	README  
	
	SYSTEM  
	
	TEST


[ABOUT THE CONFIGURATION FILE] :
	File Name:sysconfig.cfg
	Path:same with execution file.
	commentation character:#
DESCRIPTION OF SUB-ITEMS:
	[LISTEN_IP]	This server address is often used by external client.		 
	[LISTEN_PORT]   The port which is server provided.
	[REMOTE_IP] 	The POS mainframe server's address.
	[REMOTE_PORT] 	The Pos mainframe server's port.
	[TIMEOUT_VAL]	The Time out vale for connection.
	[ENCYPT_IP] 	The Encrypt-machine's address
	[ENCYPT_PORT]	The Encypte-machine's port 
	[MAC_VALUE]	The MAC VALUE.
	[PIN_VALUE]	The PIN VALUE
	[FIELDS_VAL] 	The field descriptions of protocols of POS
	[MSGSWITCH]     1 indicates, the log message will be shown on screen ,otherwise,0 indicates turn off thelogmessage show.	
	[LOGWRITE]  	1 indicates, the all log message shown on screen will be write down as logfile,otherwise, 0 indicates don't do log-writing.

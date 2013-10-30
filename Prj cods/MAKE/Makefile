##################################################################
#Copyright: all reversed by lihao and palm co.,ltd		 #
#Purpose: To compile objects files  				 #
#Date: Sep 8 ,2008						 #
#Change History:						 #
#		Sep 8 ,2008  Created 				 #
#								 #
#Make files to compile all files 				 #
#								 #
##################################################################

#all list below here are directories of source files.
GCC = gcc
PUBLICDIR = ../PUBLIC
SYSTEMDIR = ../SYSTEM
MAKEDIR = ../MAKE
MAINDIR = ../MAIN
LOGDIR =../LOG
COMMUNDIR =../COMMUN
POSDIR  = ../POS
ENCRYDIR = ../ENCRYPT
SQLDIR  = ../SQL


INCLUDE = -I $(PUBLICDIR)/include -I $(SYSTEMDIR)/include -I $(LOGDIR)/include -I $(COMMUNDIR)/include -I $(POSDIR)/include -I$(ENCRYDIR)/include  -I $(SQLDIR)/include  -I /usr/include/mysql 

LIBRARIES =  -lpthread  -lnsl  

PUBLICOBJ1 = SettingFiles.o PublicFuncs.o  
PUBLICOBJS= $(PUBLICOBJ1)   

LOGOBJS1 = LogFiles.o 
LOGOBJS = $(LOGOBJS1) 

SYSOBJS1 = ProcessCmd.o 
SYSOBJS = $(SYSOBJS1) 


MAINOBJS1= OnlinePaymentM.o 
MAINOBJS = $(MAINOBJS1)

COMMUNOBJ1 = usocket.o
COMMUNOBJS = $(COMMUNOBJ1) 

POSOBJ1  = PosCommun.o PosSysPub.o
POSOBJ = $(POSOBJ1)

ENCRYPTOBJ1 = EncryptFun.o 
ENCRYPTOBJ = $(ENCRYPTOBJ1)

SQLOBJ1 = SqlInterface.o
SQLOBJ  = $(SQLOBJ1) 

#all objected file for system.
ALLOBJS = $(SYSTEMOBJS) $(PUBLICOBJS) $(LOGOBJS) $(SYSOBJS) $(MAINOBJS) $(COMMUNOBJS) $(POSOBJ) $(ENCRYPTOBJ) $(SQLOBJ)

OnlinePayment: $(ALLOBJS) 
	$(GCC) -g3 -Wall -o $@  $(ALLOBJS) $(INCLUDE) $(LIBRARIES) 


.PHONY:cleanup
cleanup:
	rm *.o -rf


#----------PUBIC-------------------------------
SettingFiles.o:$(PUBLICDIR)/src/SettingFiles.c
	$(GCC)   $(INCLUDE) -c $< -g $(LIBRARIES)
PublicFuncs.o:$(PUBLICDIR)/src/PublicFuncs.c
	$(GCC)   $(INCLUDE) -c $< -g $(LIBRARIES)

#----------------------------------------------

#-----------LOGFILE-----------------------------
LogFiles.o: $(LOGDIR)/src/LogFiles.c
	$(GCC)  $(INCLUDE) -c $< -g $(LIBRARIES)

#----------------------------------------------

#----------SYSTEM-------------------------------
ProcessCmd.o: $(SYSTEMDIR)/src/ProcessCmd.c
	$(GCC) $(INCLUDE) -c $^ -g $(LIBRARIES)

#----------------------------------------------


#-----------MAIN-------------------------------
OnlinePaymentM.o:$(MAINDIR)/OnlinePaymentM.c
	$(GCC)  $(INCLUDE) -c  $< -g $(LIBRARIES)

#----------------------------------------------



#-----------COMMNUN------------------------------
usocket.o:$(COMMUNDIR)/src/usocket.c $(COMMUNDIR)/include/usocket.h
	$(GCC) $(INCLUDE) -c $< -g $(LIBRARIES) 
#----------------------------------------------

#----------POS----------------------------------
PosCommun.o:$(POSDIR)/src/PosCommun.c $(POSDIR)/include/PosCommun.h
	$(GCC) $(INCLUDE) -c $< -g $(LIBRARIES)
PosSysPub.o:$(POSDIR)/src/PosSysPub.c $(POSDIR)/include/PosCommun.h $(POSDIR)/include/Interface.h
	$(GCC) $(INCLUDE) -c $< -g $(LIBRARIES)
#-----------------------------------------------

#---------ENCRYPT------------------------------
EncryptFun.o:$(ENCRYDIR)/src/EncryptFun.c $(ENCRYDIR)/include/EncryptFun.h
	$(GCC) $(INCLUDE) -c $< -g $(LIBRARIES)
#---------------------------------------------

#-----------------SQL-------------------------
SqlInterface.o:$(SQLDIR)/src/SqlInterface.c $(SQLDIR)/include/SqlInterface.h
	$(GCC) $(INCLUDE) -c $< -g $(LIBRARIES)
#----------------------------------------------

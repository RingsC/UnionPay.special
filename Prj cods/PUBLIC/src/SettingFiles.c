/*Copyrights : all rights reserved by lihao and palm co.,ltd.
 * Author:lihao
 * Purpose : Reading Settings from configuration file ,which stored the main configurates
 * Date: Sep 8, 2008 
 * change history:
 * 		Sep 8 ,2008 Created 
 *		Oct 10, 2008 Added .
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/globalvars.h"

int  LTrimString (char* pszString )
{
	if (NULL == pszString ) return -1 ;
	char* pszTmp = pszString  ;
	while (*pszTmp!='\0'){
		
		if (*pszTmp==' ')
			pszTmp ++ ;
		else
			break; 
	}
	while (*pszTmp!='\0'){
		*pszString = *pszTmp;
		 pszString++ ;
		 pszTmp ++ ;
	}
		
	*pszString = '\0' ;
	return 0  ;
}
int RTrimString (char* pszString)
{
	if  (NULL== pszString) return  -1 ;
	char* szTmp = pszString + strlen(pszString) -1;
	
	while  (szTmp != pszString ){
		if (*szTmp==' ')
			*szTmp ='\0' ;
		else
			break; 	
		szTmp -- ;
			
	}
	
	return 0; 
}

int TrimString  (char* pszTrimedStr )
{
	LTrimString (pszTrimedStr );
	RTrimString (pszTrimedStr );
}

int splitString(const char *cp_srcstr,char *ca_desstr[], char c_chr)
{
        int i, icount, istrlen, ipos;
        char ctemp[MAXCONTLEN];
        if(cp_srcstr==NULL){
                return (0);
        }

        ipos = 0;
        icount = 0;
        memset(ctemp, 0x00, sizeof(ctemp));
        istrlen = sizeof(char)*strlen(cp_srcstr);
        for(i=0; i<istrlen; i++){
                if(cp_srcstr[i]==c_chr){
                        if(ipos>0){
                                ca_desstr[icount] = strdup(ctemp);
                                icount ++;
                                memset(ctemp, 0x00, sizeof(ctemp));
                                ipos = 0;
                        }
                }
                else{
                        ctemp[ipos]=cp_srcstr[i];
                        ipos++;
                }
        }

        if(ipos>0 && strlen(ctemp)>0){
                ca_desstr[icount] = strdup(ctemp);
                icount++;
        }

        return icount;
}


int  ReadStringsFromFile  (const char* pszFileName ,const char* pszSectionName,char* pszSectionValue)
{	
	
	if  (NULL==pszFileName || NULL == pszSectionName )
		return -1 ;
	if  (0==strlen(pszFileName)||0 == strlen(pszSectionName))
		return -1 ;
	
	FILE* fhd = NULL ;
	fhd = fopen (pszFileName,"r");
	if  (!fhd){
		fseek (fhd,0,SEEK_SET);
					
		
	}
	
	fclose (fhd);
	return 0 ;
}


//Here ,I will give my thanks to the guy, who write this prototype of this function in kernel.
int  read_config_file(char *filename,char* szSectionName ,char* szValue) 
{
        FILE *file;
        char *line = NULL;
        size_t dummy;
        file = fopen(filename, "r");
        if (!file)
                return;
        while (!feof(file)) {
                char *c;
                line = NULL;
                if (getline(&line, &dummy, file) <= 0) {
                        free(line);
                        break;
                }
                if (line[0] == '#') {
                        free(line);
                        continue;
                }
		
                c = strstr(line, szSectionName);
                if (c) {
                        c = strstr(c, "=");
                        if (c)
				strcpy  (szValue, c+1 );
                }
                free(line);
        }
        fclose(file);
	szValue [strlen(szValue)-1]='\0' ;
	TrimString (szValue) ;
	return 0;
}


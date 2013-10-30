#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int SplitString (const char* buffer, const char SplitChar , char**  array)
{
	if (NULL == buffer) return  -1; 
	int i =-1 ;
	
	i = strlen (*array);	
	printf ("i=%d",i) ;
	return 0;
}

/*
int main (int argc ,char* argv[])
{

	char csp = '=';
	char saArray [][20] ={"Hello world","Test Message","OK and Old"};
	char* buffer ="This is a test=343554";
	SplitString (buffer,csp,saArray);
	
	return  0;
}
*/

int main ()
{
	int a =4 ,b =0 ;
	_asm
	{
		bsf eax , a 
		mov b ,eax 
	}
	printf  ("b value is: %d\n" ,b) ;
	return  0;	

}

#include <stdio.h>
#include <sybfront.h>
#include <sybdb.h>
#include <string.h>
#include <stdlib.h>
#include "sybdbex.h"

char *date_to_str(DBDATETIME date)
{
	/* #include <malloc.h> needed
	
	   DBDATETIME is an integer
	   converts that number to the actual date as a string in the format day. name_of_month year

	   returns
			success: a pointer to the converted date string (string has to be freed after use)
			error:   NULL
	 */
	char y[4+1];
	char m[19+1];
	char d[2+1];
	char *pDateStr;

	if (dbdatename(dbproc,y,DBDATE_YY,&date) == -1)
		return NULL;
	if (dbdatename(dbproc,m,DBDATE_MM,&date) == -1)
		return NULL;
	if (dbdatename(dbproc,d,DBDATE_DD,&date) == -1)
		return NULL;

	pDateStr = (char *)malloc(100+1);
	if(pDateStr != NULL)
	{
		pDateStr[0]='\0';
		strcat(pDateStr,d);
		strcat(pDateStr,". ");
		strcat(pDateStr,m);
		strcat(pDateStr," ");
		strcat(pDateStr,y);

		return pDateStr;
	}
	else
		return NULL;
}

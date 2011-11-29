#include <stdio.h>
#include <sybfront.h>
#include <sybdb.h>
#include <string.h>
#include <stdlib.h>
#include "sybdbex.h"

/*globale Variablen*/
DBPROCESS     *dbproc;     
LOGINREC      *login;       
RETCODE        result;


int err_handler(DBPROCESS* dbproc, int severity, int dberr, int oserr, char* dberrstr, char* oserrstr)
{
	if ((dbproc == NULL) || (DBDEAD(dbproc)))
		return(INT_EXIT);
	else 
	{
		fprintf (ERR_CH, "DB-Library error:\n\t%s\n", dberrstr);

		if (oserr != DBNOERR)
			fprintf (ERR_CH, "Operating-system error:\n\t%s\n", oserrstr);

		return(INT_CANCEL);
	}
}


int msg_handler(DBPROCESS* dbproc, DBINT msgno, int msgstate, int severity, char* msgtext, char* srvname, char* procname, DBUSMALLINT line)
{
	fprintf (ERR_CH, "Msg %ld, Level %d, State %d\n", 
	        msgno, severity, msgstate);

	if (strlen(srvname) > 0)
		fprintf (ERR_CH, "Server '%s', ", srvname);
	if (strlen(procname) > 0)
		fprintf (ERR_CH, "Procedure '%s', ", procname);
	if (line > 0)
		fprintf (ERR_CH, "Line %d", line);

	fprintf (ERR_CH, "\n\t%s\n", msgtext);

	return(0);
}

int main(void)
{
	int i=0;
	char menge[5+1];
	DBCHAR title_id[7];
	DBCHAR stor_id[5];
	DBCHAR ord_num[21];
	DBFLT8 discount;
	DBINT qty;
	
	printf("\n\nAnzeige von Verkaeufen aller Shops\n");

	/* initialize db library */
	if (dbinit() == FAIL)
		return 0;
	
	/* set message and error handler routines */
	dberrhandle((EHANDLEFUNC)err_handler);
	dbmsghandle((MHANDLEFUNC)msg_handler);

	/* create login, user and pw are stored in  sybdbex.h */
	login = dblogin();
	DBSETLUSER(login, USER);
	DBSETLPWD(login, PASSWORD);
	DBSETLAPP(login, "store_beispiel");

	/* login to server */
	dbproc = dbopen(login, "syb150");
	dbuse(dbproc, "pubs2"); 

	printf("Geben Sie die Menge ein: ");
	scanf("%s",menge);

	dbfcmd(dbproc,"SELECT * FROM salesdetail WHERE qty> %s", menge);
		
	dbsqlexec(dbproc);

	while (dbresults(dbproc)!=NO_MORE_RESULTS)

	{	
	dbbind(dbproc,1,NTBSTRINGBIND,0,stor_id);
	dbbind(dbproc,2,NTBSTRINGBIND,0,ord_num);
	dbbind(dbproc,3,NTBSTRINGBIND,0,title_id);
	dbbind(dbproc,4,INTBIND,0,(BYTE *)&qty);
	dbbind(dbproc,5,FLT8BIND,0, (BYTE *)&discount);
	
	
	while (dbnextrow(dbproc)!=NO_MORE_ROWS)
		
		{
		printf("Store:      %s\n",stor_id);
		printf("Bestellnr.: %s\n",ord_num);
		printf("Discount:   %f\n",discount);
		printf("Anzahl:     %i\n",qty);
		printf("Titel-ID:   %s\n\n",title_id);
		printf("______________________________________\n");
		}
	}


/* Close and deallocate all DBPROCESS structures, clean up */

	/* dbexit(STDEXIT); */
	dbexit();
	exit(STDEXIT);
}

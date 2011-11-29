#include <stdio.h>
#include <sybfront.h>
#include <sybdb.h>
#include "sybdbex.h"	// Username und PW einfügen

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

void main()
{
	RETCODE results;
	DBPROCESS *dbproc;
	LOGINREC *login;
	DBCHAR title_id[7];
	DBCHAR store_id[5];
	DBCHAR ord_num[21];
	DBCHAR payterm[13];
	DBCHAR dates[31];
	DBINT qty;	
	
	dberrhandle(err_handler);
	dbmsghandle(msg_handler);
	login = dblogin();
	DBSETLUSER(login, USER);
	DBSETLPWD(login, PASSWORD);
	DBSETLAPP(login, "store");
	
	dbproc=dbopen(login, NULL);
	dbuse(dbproc, "pubs2");
	dbcmd(dbproc, "SELECT * FROM sales");  // erste Spalte aufgerufen
	dbsqlexec(dbproc);		// alternative an dieser Stelle möglich
							/*
								if(dbsqlexec(dbproc)== FAIL)
								{
									printf("dbsqlexec fail!");
									dbexit(STDEXIT); // STDEXIT = symbolische konstante aus der include
									exit();
								}		
							*/
	
	while(dbresults(dbproc)!= NOW_MORE_RESULTS)
	{
		dbbind(dbproc, 1, NTBSTRINGBIND, 0, store_id);		//NTB = Null terminated byte
		dbbind(dbproc, 2, NTBSTRINGBIND, 0, ord_num);
		dbbind(dbproc, 3, NTBSTRINGBIND, 0, dates);
		dbbind(dbproc, 4, INTBIND, 0, &qty);
		dbbind(dbproc, 5, NTBSTRINGBIND, 0, payterm);
		dbbind(dbproc, 6, NTBSTRINGBIND, 0, title_id);
		while(dbnextrow(dbproc)!=NO_MORE_ROWS)
		{
			printf();
		}
	}
	
	dbexit(STDEXIT); // STDEXIT = symbolische konstante aus der include
	exit();
}


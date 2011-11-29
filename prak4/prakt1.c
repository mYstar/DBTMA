#include <stdio.h>
#include <sybfront.h>
#include <sybdb.h>
#include <string.h>
#include <stdlib.h>
#include "sybdbex.h"
#define BUFSIZE 1024
/*globale Variablen*/
DBPROCESS     *dbproc;
DBPROCESS     *dbproc_insert;
LOGINREC      *login;
RETCODE        result;

int i=0;
char beruf_input[16];
char mitnr_input[16];
DBCHAR Beruf[16];
DBCHAR Mitnr[16];
DBCHAR Name[16];
DBCHAR Mitarbeiter[16];
DBCHAR Ort[16];
DBCHAR Vorname[16];
DBCHAR Proname[16];
DBDATETIME Gebdat;
DBFLT8 Istvzae;
DBFLT8 Planvzae;
DBCHAR Telnr[16];
DBCHAR abstract_var[512];
char answer[20];
char  avg_old[20];
char buf[BUFSIZE + 1];
BYTE * ret;

char *date_to_str(DBDATETIME date){
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

int err_handler(DBPROCESS* dbproc, int severity, int dberr, int oserr, char* dberrstr, char* oserrstr){
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

int msg_handler(DBPROCESS* dbproc, DBINT msgno, int msgstate, int severity, char* msgtext, char* srvname, char* procname, DBUSMALLINT line){
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

int init_db(void){
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
	DBSETLAPP(login, "berufe");

	/* login to server */
	dbproc = dbopen(login, "syb150");
	dbproc_insert = dbopen(login, "syb150");
	dbuse(dbproc, "ma11s67778");
	dbuse(dbproc_insert, "ma11s67778");

}
void print_ort(void){
	/*anzeige aller berufe*/
	dbcmd(dbproc,"SELECT Ort FROM Mitarbeiter group by Ort");
	dbsqlexec(dbproc);

	while (dbresults(dbproc)!=NO_MORE_RESULTS)
	{
		dbbind(dbproc,1,NTBSTRINGBIND,0,Ort);
		while (dbnextrow(dbproc)!=NO_MORE_ROWS)
		{
			printf(":      %s\n",Mitarbeiter);
		 }
	}

}
void insert_lebenslauf(void){
	/*anzeige aller berufe*/
	dbcmd(dbproc,"SELECT Lebenslauf FROM Mitarbeiter");
	dbsqlexec(dbproc);

	while (dbresults(dbproc)!=NO_MORE_RESULTS)
	{
		dbbind(dbproc,1,STRINGBIND,0,abstract_var);
		while (dbnextrow(dbproc)!=NO_MORE_ROWS)
		{
      strcpy(abstract_var, "Hauptschule");
      dbwritetext(dbproc_insert, "Mitarbeiter.Lebenslauf",
          dbtxptr(dbproc, 1), DBTXPLEN, dbtxtimestamp(dbproc,1),TRUE,
          (DBINT)strlen(abstract_var),abstract_var);
		}
	}

}
void read_lebenslauf(void){
  long bytes;
	/*anzeige aller berufe*/
	dbcmd(dbproc,"SELECT Lebenslauf FROM Mitarbeiter");
	dbsqlexec(dbproc);

	while (dbresults(dbproc)!=NO_MORE_RESULTS)
	{
		while ((bytes= dbreadtext(dbproc,(void *)buf,BUFSIZE)) != NO_MORE_ROWS)
		{
      if ( bytes == 0){
        printf("end of row");
      }
      else{
      buf[bytes] = '\0';
      printf("%s\n",buf);

      }
		}
	}

}
int main(void)
{
	// initialisiere die DB
	init_db();
  read_lebenslauf();
/*
		print_ort();
		gets(answer);
    doAverage(answer);

		[> Close and deallocate all DBPROCESS structures, clean up <]

		[> dbexit(STDEXIT); <]<]
*/
	dbexit();
	exit(STDEXIT);
}


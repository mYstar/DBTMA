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
int i;

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
  dbuse(dbproc, "ma11s61406");

  //createStoredProc();

  /* dbexit(STDEXIT); */
  dbexit();
  exit(STDEXIT);
}


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
DBCHAR beruf[16];
DBCHAR mitnr[6];
char answer[16];
DBCHAR name[11];
DBCHAR vorname[11];
DBCHAR ort[21];
DBDATETIME gebdat;
DBCHAR telnr[13];
DBCHAR proname[16];
DBFLT8 istvzae;
DBFLT8 planvzae;

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

void get_berufe()
{
  dbcmd(dbproc,"SELECT Beruf FROM Mitarbeiter GROUP BY Beruf");

  dbsqlexec(dbproc);

  while (dbresults(dbproc)!=NO_MORE_RESULTS)
  {	
    i=0;
    dbbind(dbproc,1,NTBSTRINGBIND,15,beruf);

    while (dbnextrow(dbproc)!=NO_MORE_ROWS)
    {
      printf("%d:      %s\n",i, beruf);
      i++;
    }
  }
}

void get_mitarbeiter(char *beruf_in)
{
  dbfcmd(dbproc,"SELECT Mitnr, Name, Vorname FROM Mitarbeiter WHERE Beruf = '%s'", beruf_in);

  dbsqlexec(dbproc);

  while (dbresults(dbproc)!=NO_MORE_RESULTS)
  {	
    i=0;
    dbbind(dbproc,1,NTBSTRINGBIND,5,mitnr);  
    dbbind(dbproc,2,NTBSTRINGBIND,10,name);
    dbbind(dbproc,3,NTBSTRINGBIND,10,vorname);

    while (dbnextrow(dbproc)!=NO_MORE_ROWS)
    {
      printf("%d:\t%s,\t%s,\t%s\n",i, mitnr, name, vorname);
      i++;
    }
  }
}

void get_all(char *mitnr_in)
{
  dbfcmd(dbproc,"SELECT m.Name, m.Vorname, m.Ort, m.Gebdat, m.Beruf, m.Telnr, mp.Istvzae, mp.Planvzae, p.Proname FROM Mitarbeiter m, MiPro mp, Projekt p WHERE m.Mitnr = mp.Mitnr AND mp.Pronr = p.Pronr AND m.Mitnr = '%s'", mitnr_in);

  dbsqlexec(dbproc);

  while (dbresults(dbproc)!=NO_MORE_RESULTS)
  {	
    i=0;
    dbbind(dbproc,1,NTBSTRINGBIND,0,name);
    dbbind(dbproc,2,NTBSTRINGBIND,0,vorname);
    dbbind(dbproc,3,NTBSTRINGBIND,0,ort);
    dbbind(dbproc,4,DATETIMEBIND,0,(BYTE *)&gebdat);
    dbbind(dbproc,5,NTBSTRINGBIND,0,beruf);
    dbbind(dbproc,6,NTBSTRINGBIND,0,telnr);
    dbbind(dbproc,7,FLT8BIND,0,(BYTE *)&istvzae);
    dbbind(dbproc,8,FLT8BIND,0,(BYTE *)&planvzae);
    dbbind(dbproc,9,NTBSTRINGBIND,0,proname);

    while (dbnextrow(dbproc)!=NO_MORE_ROWS)
    {
      printf("%d:\t%s,\t%s,\t%s,\t%s,\t%s,\t%s,\t%f,\t%f,\t%s,\n",i, name, vorname, ort, date_to_str(gebdat), beruf, telnr, istvzae, planvzae, proname);
      i++;
    }
  }

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

  while(1)
  {
    get_berufe();

    printf("beruf?\n");
    gets(answer);
    
    if(strcmp(answer, "")==0)
      continue;
  while(1)
  {
    get_mitarbeiter(answer);

    printf("mitnr?\n");
    gets(answer);

    if(strcmp(answer, "")==0)
      break;
    get_all(answer);
    break;
  }
  }

    /* dbexit(STDEXIT); */
    dbexit();
    exit(STDEXIT);
}

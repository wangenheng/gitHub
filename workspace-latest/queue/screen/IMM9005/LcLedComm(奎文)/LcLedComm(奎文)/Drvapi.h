/********************************
*���ţ�                         *
*CIM��Ʒ��������ƽ̨2.0�з�С�� *
*����          ����         ����*
*2002/02/28     ��           �� *
*���ߣ�                         *
*���׹㡢��С淡���ΰ           *
********************************/
#ifndef    H_DRVAPI
#define    H_DRVAPI

#include   <direct.h>
#include   <stdarg.h>
#include   <stdio.h>
#include   <string.h>
#include   <time.h>

/****����־����****/
extern   int  Writelog(char *filename,const char *fmt,...);
/****�����쳣��־��¼****/
extern   int  SevErrWritelog(int flag,const char *fmt,...);
/****�����ڡ�ʱ�亯��****/
extern   char *GetDateTime(char *str);
/****���ص�ǰĿ¼��ĳһ�ļ��ľ���·��****/
extern   char *SetFilePath(char *filename,char *filenameurl);
/****������ش��������Ϣ****/
extern   char *GetOutputMsg(char *lpAppName,char *lpKeyName,char *outmsg);
/***�����й�ic�����������Ϣ**/
extern   char *GetICOutMsg(char *lpAppName,unsigned char *lpKeyName,char *outmsg);
/****���ַ���0λ��ʼȡ��ָ���ַ����ַ���****/
extern int GetStrtodot(char *resource,char *dest,char dot);
/****�ж�һ���ַ����ж��ٸ��ַ�dot****/
extern int GetNdot(char *str,char dot);
extern void WriteDebuglog(char *Inbuff);
extern int str2hex(unsigned char *read, unsigned char *write, int len);
extern int asc2hex(unsigned char dat, unsigned char *newdat);
extern int hex2asc(unsigned char dat, unsigned char *newdat);
extern int random(int number) ;

#endif
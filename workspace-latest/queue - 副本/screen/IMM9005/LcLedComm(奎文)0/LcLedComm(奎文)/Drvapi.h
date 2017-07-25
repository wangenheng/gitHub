/********************************
*部门：                         *
*CIM产品部，驱动平台2.0研发小组 *
*日期          星期         天气*
*2002/02/28     四           晴 *
*作者：                         *
*刘兆广、徐小娣、王伟           *
********************************/
#ifndef    H_DRVAPI
#define    H_DRVAPI

#include   <direct.h>
#include   <stdarg.h>
#include   <stdio.h>
#include   <string.h>
#include   <time.h>

/****记日志函数****/
extern   int  Writelog(char *filename,const char *fmt,...);
/****服务、异常日志记录****/
extern   int  SevErrWritelog(int flag,const char *fmt,...);
/****返日期、时间函数****/
extern   char *GetDateTime(char *str);
/****返回当前目录下某一文件的绝对路径****/
extern   char *SetFilePath(char *filename,char *filenameurl);
/****返回相关错误码的信息****/
extern   char *GetOutputMsg(char *lpAppName,char *lpKeyName,char *outmsg);
/***返回有关ic卡错误码的信息**/
extern   char *GetICOutMsg(char *lpAppName,unsigned char *lpKeyName,char *outmsg);
/****从字符串0位开始取到指定字符的字符串****/
extern int GetStrtodot(char *resource,char *dest,char dot);
/****判断一个字符串有多少个字符dot****/
extern int GetNdot(char *str,char dot);
extern void WriteDebuglog(char *Inbuff);
extern int str2hex(unsigned char *read, unsigned char *write, int len);
extern int asc2hex(unsigned char dat, unsigned char *newdat);
extern int hex2asc(unsigned char dat, unsigned char *newdat);
extern int random(int number) ;

#endif
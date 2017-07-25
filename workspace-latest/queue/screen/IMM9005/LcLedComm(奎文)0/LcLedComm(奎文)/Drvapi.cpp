#include "StdAfx.h"
#include "Drvapi.h"


/****************************
���ܣ�
	��¼��־
������
	filename,��־�ļ���
	fmt,��ʽ�ַ���
	...����ʽ��Ӧ��ֵ
����ֵ��
	-1�����ļ�ʧ��
	0���ɹ�
*****************************/

int  Writelog(char *filename,const char *fmt,...)
{
	FILE *fp;
	char buff[1000];
	va_list ap;

	if((fp = fopen(filename,"a+")) == NULL)
	{
		return -1;
	}

	memset(buff,0x00,sizeof(buff));
	va_start(ap,fmt);
	vsprintf(buff,fmt,ap);
	va_end(ap);
	
	fprintf(fp,"%s\n",buff);
	fclose(fp);
	return 0;
}

void WriteDebuglog(char *Inbuff)
{
	CTime t;
	t = CTime::GetCurrentTime();
	char dtstr[64];
	memset(dtstr,0,sizeof(dtstr));
	_snprintf(dtstr,sizeof(dtstr),"%d%02d",t.GetYear(),t.GetMonth());
	
	char filename[1024];
	memset(filename,0,sizeof(filename));
	::GetCurrentDirectory(sizeof(filename),filename);
	strcat(filename,"\\DrvWiz.ini");
	int DebugFlag=::GetPrivateProfileInt("General","DEBUG",0,filename);
	if(DebugFlag==1)
	{
		FILE *fp;
		memset(filename,0,sizeof(filename));
		::GetCurrentDirectory(sizeof(filename),filename);
		sprintf(&filename[strlen(filename)],"\\log\\Debug%s.log",&dtstr[2]);
		if((fp = fopen(filename,"a+"))==NULL)
		{
			return; 
		}
		fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d  %s\n",
				t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),Inbuff);
		fclose(fp);
	}
	return;

}


/****************************
���ܣ�
	�����쳣��־��¼
������
	flag:0,������־��¼��1���쳣��־��¼
	fmt,��ʽ�ַ���
	...,��ʽ��Ӧ��ֵ
����ֵ��
	-1��ʧ��
	0���ɹ�
	SevErrWritelog(0,"%s|%s|%s","ocx","read","sdjl");
*****************************/
int  SevErrWritelog(int flag,const char *fmt,...)
{
	FILE *fp;
	char path[256];
	char filepath[256];
	char dtstr[64];
	char buff[1000];

	CTime t;
	va_list ap;

	memset(path,0x00,sizeof(path));
	memset(filepath,0x00,sizeof(filepath));
	memset(dtstr,0x00,sizeof(dtstr));
	memset(buff,0x00,sizeof(buff));
	
	t = CTime::GetCurrentTime();
	_snprintf(dtstr,sizeof(dtstr),"%d%02d",t.GetYear(),t.GetMonth());
	
	switch(flag)
	{
		case 0:
			_snprintf(filepath,sizeof(filepath),"log\\LcLedComm%s.log",&dtstr[2]);
			break;
		case 1:
			_snprintf(filepath,sizeof(filepath),"log\\LcLedCommErrMsg%s.log",&dtstr[2]);
			break;

		default:
			return -1;
	}

	SetFilePath(filepath,path);
	
	if((fp = fopen(path,"a+")) == NULL)
	{
		return -1;
	}
	
	va_start(ap,fmt);
	vsprintf(buff,fmt,ap);
	va_end(ap);
	
	memset(dtstr,0x00,sizeof(dtstr));
	fprintf(fp,"%s|%s\n",GetDateTime(dtstr),buff);
	fclose(fp);
	
	return 0;
}

/****************************
���ܣ�
	�����ڡ�ʱ�亯��
������
	str,������ڡ�ʱ����ַ���
����ֵ��
	����,2002.02.28|13:53:30
*****************************/
char *GetDateTime(char *str)
{
	time_t t;
    struct tm *mytm;
	
    t = time(NULL);
    mytm = localtime(&t);
    sprintf(str,"%04d.%02d.%02d|%02d:%02d:%02d",mytm->tm_year+1900,mytm->tm_mon+1,mytm->tm_mday,mytm->tm_hour,mytm->tm_min,mytm->tm_sec);

	return str;
}

/****************************
���ܣ�
	���ص�ǰĿ¼��ĳһ�ļ�(��
	Ŀ¼��ĳһ�ļ�)�ľ���·��
������
	filename,����װ����·����
	�ļ�
����ֵ��
	��װ����·�����ַ���
*****************************/
char *SetFilePath(char *filename,char *filenameurl)
{
	char path[256];
	
	memset(path,0x00,sizeof(path));
	::GetCurrentDirectory(sizeof(path),path);
//	_getcwd(path,sizeof(path));
	sprintf(filenameurl,"%s\\%s",path,filename);

	return filenameurl;
}
/****************************
���ܣ�
	������ش������Ϣ
������
	lpAppName,��Ϣ�ļ�С����
	lpKeyName,��Ϣ�ļ��Ĺؼ���
	outmsg,�����ȡ����Ϣ
����ֵ��
	���عؼ��ֶ�Ӧ����Ϣ����
	����Ҳ�������"�������Ϣ"
*****************************/
char *GetOutputMsg(char *lpAppName,char *lpKeyName,char *outmsg)
{
	char  path[256];
	memset(path,0x00,sizeof(path));
	SetFilePath("OutPutMsg.ini",path);
	GetPrivateProfileString(lpAppName,lpKeyName,"�������Ϣ",outmsg,128,path);
	
	return outmsg;
}
char *GetICOutMsg(char *lpAppName,unsigned char *lpKeyName,char *outmsg)
{
	char buff[512];
	memset(buff,0,sizeof(buff));
	int SW1=lpKeyName[0];
	int SW2=lpKeyName[1];

	if(SW2<0)
		SW2=256+SW2;
	
	char keyname[5];
	memset(keyname,0,sizeof(keyname));
	sprintf(keyname,"%2.2x%2.2x",SW1,SW2);
	char key[5];
	memset(key,0,sizeof(key));

	char  path[256];
	memset(path,0x00,sizeof(path));
	SetFilePath("OutPutMsg.ini",path);
	GetPrivateProfileString(lpAppName,keyname,"000",key,4,path);

	if(strcmp(key,"000")==0)
		sprintf(buff,"%s�������Ϣ",keyname);
	else
		GetOutputMsg("ShebeiMsg",key,buff);
	sprintf(outmsg,"%s|%s",key,buff);
	return outmsg;
	
}
/****************************
����:
	���ַ���0λ��ʼȡ��ָ���ַ����ַ���
������
	resource:Դ�ַ���
	dest:Ŀ���ַ���
	dot:ָ���ַ�
����ֵ:
        -1:Դ�ַ���Ϊ��
        -2:resource�ѵ��ַ�����λ��
        >0:�ӵ�ǰλ�õ�ָ���ַ�λ�õĳ���
*****************************/
int GetStrtodot(char *resource,char *dest,char dot)
{
        int pos;

        if(resource == NULL)
                return -1;

        pos = 0;
        while(resource[pos] != 0)
        {
                if(resource[pos] != dot)
                {
                        dest[pos] = resource[pos];
                        pos++;
                }
                else
                        break;
        }

        if(resource[pos] == 0)
                return -2;

        dest[pos] = 0;
        return pos+1;
}

/****************************
���ܣ�
	�ж�һ���ַ����ж��ٸ��ַ�dot
������
	str,�ַ�Դ
	dot,�ַ�
����ֵ��
	dot��str�еĸ���
*****************************/
int GetNdot(char *str,char dot)
{
	int num;
	int pos;

	num = 0;
	pos = 0;
	while(str[pos] != 0)
	{
		if(str[pos] == dot)
			num+=1;
		pos++;
	}
	return num;
}

int str2hex(unsigned char *read, unsigned char *write, int len)
{
	unsigned char dat;
	
	while (len--) {
		if (asc2hex(*read++, &dat))
			return -1;
		*write = *write << 4 | dat;
		if (!(len & 1)) {
			++write;
		}
	}
	return 0;
}

int asc2hex(unsigned char dat, unsigned char *newdat)
{
	if ((dat >= '0') && (dat <= '9'))
		*newdat = dat - '0';
	else if ((dat >= 'A') && (dat <= 'F'))
		*newdat = dat - 'A' + 10; 
	else if ((dat >= 'a') && (dat <= 'f'))
		*newdat = dat - 'a' + 10;
	else
		return -1;
	return 0;
}


int hex2asc(unsigned char dat, unsigned char *newdat)
{
	if ((dat >= 0) && (dat <= 9))
		*newdat = dat +'0';
	else 
		return -1;
	return 0;
}

 int random(int number) 
 { 
     return (int)(number/(float)RAND_MAX * rand()); 
 } 

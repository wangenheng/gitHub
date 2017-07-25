#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
//#include "StdAfx.h"
#include "cfgfile.h"

//-------------------------�ײ������ļ���д�ӿ�------------------------------

int CFG_ReadStr(char *FileName,
				char *ConfigName,
				char *ConfigValue)
{
    FILE *fp;
    char buff[4096],tar_value[256],ConfigNameEx[256];

    int i,counter,cur;	//cur����Ŀ��ֵ��λ��
    char *pos;		//pos��Ҫ���ҵ���Ŀ���ļ���λ��
    int rv,len;

    memset(buff,0,sizeof(buff));
    memset(tar_value,0,sizeof(tar_value));
    memset(ConfigNameEx,0,sizeof(ConfigNameEx));

    fp = fopen(FileName,"r");		//�������ļ�
    if(fp == NULL)
	return CFG_FILEOPEN_ERROR;
	
    rv = fseek(fp,0,SEEK_END);		//���ļ�ָ����λ���ļ���β
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    len = ftell(fp);			//����ļ�����
    if(len -1 > 4095 || len == -1)	//�ļ����ȷǷ�
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    
    rv = fseek(fp,0,SEEK_SET);		//���ļ�ָ������ָ���ļ�ͷ
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;    	
    }
    
    fread(buff,4096,sizeof(char),fp);
    fclose(fp);

    memcpy(ConfigNameEx,ConfigName,strlen(ConfigName) + 1);
    ConfigNameEx[strlen(ConfigName)] = ' ';

    pos = strstr(buff,ConfigNameEx);	//���Ҫд�����Ŀ���ļ��еĿ�ʼλ��
    if(pos == NULL)  
	return CFG_FINDITEM_ERROR;

    //Ҫ�������ļ���ÿ����Ŀ�ĸ�ʽ��  ��Ŀ��+�ո�+�Ⱥ�+�ո�+��Ŀ����+��

    //posָ����Ŀֵ�ĵ�һ���ֽ�,3��" = "�ĳ���
    pos = pos + strlen(ConfigName) + 3;

    cur = pos - buff;			//�����Ŀֵ��buff�����е�λ��

    counter=0;

    for(i = cur; i < 4096; i++)		//�ݴ���,���˵��Ƿ��ַ�
	if(buff[i]!=';')
/*	    if( (buff[i] >='a' && buff[i] <='z') || 
	        (buff[i] >='A' && buff[i] <='Z') ||
		(buff[i] >='0' && buff[i] <='9') ||
		 buff[i] == '.' || buff[i] == '_' || 
		 buff[i] == 92  || buff[i] == ':' ||
		 buff[i] == 47  || buff[i] == '-')*/
		{	
		    tar_value[counter]=buff[i];
		    counter++;
		} 
/*	    else
		continue;*/
	else
	    break;
		
    strcpy(ConfigValue,tar_value);	//���������ַ���ֵ������

    return OP_SUCCESS;
}

int CFG_ReadInt(	char *FileName,
					char *ConfigName,
					int  *ConfigValue)
{
    FILE *fp;
    char buff[4096],tar_value[256],ConfigNameEx[256];
	
    int i,counter,cur;			//cur����Ŀ��ֵ��λ��
    char *pos;				//pos��Ҫ���ҵ���Ŀ���ļ���λ��
    int rv,len;

    memset(buff,0,sizeof(buff));
    memset(tar_value,0,sizeof(tar_value));
    memset(ConfigNameEx,0,sizeof(ConfigNameEx));

    fp = fopen(FileName,"r");		//�������ļ�
    if(fp == NULL)
	return CFG_FILEOPEN_ERROR;
	
    rv = fseek(fp,0,SEEK_END);		//���ļ�ָ����λ���ļ���β
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    len = ftell(fp);			//����ļ�����
    if(len -1 > 4095 || len == -1)	//�ļ����ȷǷ�
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    
    rv = fseek(fp,0,SEEK_SET);		//���ļ�ָ������ָ���ļ�ͷ
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;    	
    }
	
    fread(buff,4096,sizeof(char),fp);
    fclose(fp);
    
    memcpy(ConfigNameEx,ConfigName,strlen(ConfigName) + 1);
    ConfigNameEx[strlen(ConfigName)] = ' ';

    pos = strstr(buff,ConfigNameEx);	//���Ҫд�����Ŀ���ļ��еĿ�ʼλ��
    if(pos == NULL)  
	return CFG_FINDITEM_ERROR;

    //Ҫ�������ļ���ÿ����Ŀ�ĸ�ʽ��  ��Ŀ��+�ո�+�Ⱥ�+�ո�+��Ŀ����+��

    //posָ����Ŀֵ�ĵ�һ���ֽ�
    pos = pos + strlen(ConfigName) + 3;

    cur = pos - buff;			//�����Ŀֵ��buff�����е�λ��

    counter=0;

    for(i = cur; i < 4096; i++)		//�ݴ���,���˵��Ƿ��ַ�
	if(buff[i]!=';')
	    if((buff[i] >='0' && buff[i] <='9'))
		{	
		    tar_value[counter]=buff[i];
		    counter++;
		} 
		else
		    continue;
	else
	    break;
		
    sscanf(tar_value, "%d", ConfigValue);   //����ȡ��ֵת�������Ͳ���ֵ������

    return OP_SUCCESS;
}

int CFG_WriteStr(	char *FileName,
					char *ConfigName,
					char *ConfigValue)
{
    FILE *fp;
    char buff[4096],temp[4096],tar_value[256],decbuf[256],ConfigNameEx[256];
	
    int i,counter,cur,filelen,newlen;	//cur����Ŀ��ֵ��λ��
					//counter���ַ����ݳ��ȼ�����
					//newlen���޸ĺ���ļ�����
    char *pos;				//pos��Ҫ���ҵ���Ŀ���ļ���λ��
    int rv,len;

    memset(buff,0,sizeof(buff));
    memset(temp,0,sizeof(temp));
    memset(decbuf,0,sizeof(decbuf));
    memset(tar_value,0,sizeof(tar_value));
    memset(ConfigNameEx,0,sizeof(ConfigNameEx));

    memcpy(decbuf,ConfigValue,strlen(ConfigValue));	
/*
    for(i = 0; i < (int)strlen(ConfigValue); i++)   //�ݴ���,���ַǷ��ַ�����
	if((decbuf[i] >='a' && decbuf[i] <='z') || 
	    (decbuf[i] >='A' && decbuf[i] <='Z') ||
	    (decbuf[i] >='0' && decbuf[i] <='9') ||
	     decbuf[i] == '.' || decbuf[i] == '_' || 
	     decbuf[i] == 92  || decbuf[i] == ':' ||
	     decbuf[i] == 47  || decbuf[i] == '-'||decbuf[i] == '|'||decbuf[i] == ',')
		 
	{    
	    continue;
	} 
	else
	{
	    return CFG_PARAMETER_ERROR;
	}
*/
		
    fp = fopen(FileName,"rb");		//�������ļ�
    if(fp == NULL)
    {
	return CFG_FILEOPEN_ERROR;
    }
	
    rv = fseek(fp,0,SEEK_END);		//���ļ�ָ����λ���ļ���β
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    len = ftell(fp);			//����ļ�����
    if(len -1 > 4095 || len == -1)	//�ļ����ȷǷ�
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    
    rv = fseek(fp,0,SEEK_SET);		//���ļ�ָ������ָ���ļ�ͷ
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;    	
    }
    
    fread(buff,4096,sizeof(char),fp);
    fclose(fp);
    
    memcpy(ConfigNameEx,ConfigName,strlen(ConfigName) + 1);
    ConfigNameEx[strlen(ConfigName)] = ' ';

    filelen = strlen(buff);		//����ļ�����

    pos = strstr(buff,ConfigNameEx);	//���Ҫд�����Ŀ���ļ��еĿ�ʼλ��
    if(pos == NULL)
    {
	return CFG_FINDITEM_ERROR;
    }

    //Ҫ�������ļ���ÿ����Ŀ�ĸ�ʽ��  ��Ŀ��+�ո�+�Ⱥ�+�ո�+��Ŀ����+��

    //posָ����Ŀֵ�ĵ�һ���ֽ�
    pos = pos + strlen(ConfigName) + 3;

    cur = pos - buff;			//�����Ŀֵ��buff�����е�λ��

    counter=0;				//����Ŀ���ݼ�������0

    for(i=cur;i<4096;i++)		//�����Ҫ�޸ĵ���Ŀ���ݵĳ���
	if(buff[i]!=';')
	    counter++;
	else
	    break;

    for(i = 0; i < filelen - (cur+counter); i++)
	temp[i] = buff[cur+counter+i];		//�����޸���Ŀ���������

    memcpy(&buff[cur],ConfigValue,strlen(ConfigValue));	//��Ҫ�޸ĵ���Ŀ����д��

    //�Ѳ���Ҫ�޸ĵĲ���д��
    memcpy(&buff[cur + strlen(ConfigValue)],temp,filelen-(cur+counter));

    newlen = filelen + (strlen(ConfigValue)-counter);	//����µ��ļ�����

    fp = fopen(FileName,"wb");				//������д���ļ�
    if(fp == NULL)
    {
	return CFG_FILEOPEN_ERROR;
    }

    fwrite(buff,1,newlen,fp);
    fclose(fp);

    return OP_SUCCESS;
}

int CFG_WriteInt(	char *FileName,
					char *ConfigName,
					int  DataValue)
{
    FILE *fp;
    char buff[4096],temp[4096],tar_value[256],ConfigValue[256],ConfigNameEx[256];
	
    int i,counter,cur,filelen,newlen;	//cur����Ŀ��ֵ��λ��
					//counter���ַ����ݳ��ȼ�����
					//newlen���޸ĺ���ļ�����
    char *pos;				//pos��Ҫ���ҵ���Ŀ���ļ���λ��
    int rv,len;

    memset(buff,0,sizeof(buff));
    memset(temp,0,sizeof(temp));
    memset(tar_value,0,sizeof(tar_value));
    memset(ConfigValue,0,sizeof(ConfigValue));
    memset(ConfigNameEx,0,sizeof(ConfigNameEx));
	
    sprintf(ConfigValue,"%d",DataValue);	//�������������ֵת��Ϊ�ַ���

    fp = fopen(FileName,"rb");
    if(fp == NULL)
	return CFG_FILEOPEN_ERROR;

    rv = fseek(fp,0,SEEK_END);		//���ļ���λ���ļ���β
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    len = ftell(fp);			//����ļ�����
    if(len -1 > 4095 || len == -1)	//�ļ����ȷǷ�
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    
    rv = fseek(fp,0,SEEK_SET);		//���ļ�ָ������ָ���ļ�ͷ
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;    	
    }
    
    fread(buff,4096,sizeof(char),fp);	//���ļ�����д������
    fclose(fp);
    
    memcpy(ConfigNameEx,ConfigName,strlen(ConfigName) + 1);
    ConfigNameEx[strlen(ConfigName)] = ' ';

    filelen = strlen(buff);		//����ļ�����

    pos = strstr(buff,ConfigNameEx);
    if(pos == NULL)  
	return CFG_FINDITEM_ERROR;

    //Ҫ�������ļ���ÿ����Ŀ�ĸ�ʽ��  ��Ŀ��+�ո�+�Ⱥ�+�ո�+��Ŀ����+��

    pos = pos + strlen(ConfigName) + 3;	//posָ����Ŀֵ�ĵ�һ���ֽ�

    cur = pos - buff;			//�����Ŀֵ��buff�����е�λ��

    counter=0;				//����Ŀ���ݼ�������0

    for(i=cur;i<4096;i++)		//�����Ҫ�޸ĵ���Ŀ���ݵĳ���
	if(buff[i]!=';')
	    counter++;
	else
	    break;

    for(i = 0; i < filelen - (cur+counter); i++)
	temp[i] = buff[cur+counter+i];	//�����޸���Ŀ���������

    memcpy(&buff[cur],ConfigValue,strlen(ConfigValue));	//��Ҫ�޸ĵ���Ŀ����д��

    //�Ѳ���Ҫ�޸ĵĲ���д��
    memcpy(&buff[cur + strlen(ConfigValue)],temp,filelen-(cur+counter));

    newlen = filelen + (strlen(ConfigValue)-counter);	//����µ��ļ�����

    fp = fopen(FileName,"wb");				//�������ļ�
    if(fp == NULL)
	return CFG_FILEOPEN_ERROR;

    fwrite(buff,1,newlen,fp);
    fclose(fp);

    return OP_SUCCESS;
}

void CFG_GetCFGPath(unsigned char *CFGName)
{
	unsigned char cfgname[256];
	memset(cfgname, 0, sizeof(cfgname));

#if defined _OS_WIN32_
	memcpy(CFGName, CFGName, strlen((const char *)CFGName));
#endif

#if defined _OS_LINUX_
	strcpy(cfgname, getenv("HOME"));
	strcat(cfgname, "/etc/");
	strcat(cfgname, CFGName);
	memcpy(CFGName, cfgname, strlen(cfgname));
#endif
}
int CFG_Create(char *FileName,char *ConfigName)
{
	char ch[100];

	int i=0;
	memset(ch,0,100);
	i=CFG_ReadStr(FileName,ConfigName,ch);
	if(i==CFG_FINDITEM_ERROR)
	{
		FILE * file;
		if(NULL==(file=fopen(FileName,"a+")))
		{
			return  CFG_FILEOPEN_ERROR;
		}
		else
		{
			memset(ch,0,100);
			ch[0]=0x0D;
			//fwrite(&ch[0],1,strlen(ch),file);
			strcat(ch,ConfigName);
			strcat(ch," = ;");
			fwrite(ch,1,strlen(ch),file);
			fclose(file);
		}
	}
	return i;
}


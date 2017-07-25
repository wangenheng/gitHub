#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
//#include "StdAfx.h"
#include "cfgfile.h"

//-------------------------底层配置文件读写接口------------------------------

int CFG_ReadStr(char *FileName,
				char *ConfigName,
				char *ConfigValue)
{
    FILE *fp;
    char buff[4096],tar_value[256],ConfigNameEx[256];

    int i,counter,cur;	//cur是条目的值的位置
    char *pos;		//pos是要查找的条目在文件中位置
    int rv,len;

    memset(buff,0,sizeof(buff));
    memset(tar_value,0,sizeof(tar_value));
    memset(ConfigNameEx,0,sizeof(ConfigNameEx));

    fp = fopen(FileName,"r");		//打开配置文件
    if(fp == NULL)
	return CFG_FILEOPEN_ERROR;
	
    rv = fseek(fp,0,SEEK_END);		//将文件指定定位到文件结尾
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    len = ftell(fp);			//获得文件长度
    if(len -1 > 4095 || len == -1)	//文件长度非法
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    
    rv = fseek(fp,0,SEEK_SET);		//将文件指针重新指回文件头
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;    	
    }
    
    fread(buff,4096,sizeof(char),fp);
    fclose(fp);

    memcpy(ConfigNameEx,ConfigName,strlen(ConfigName) + 1);
    ConfigNameEx[strlen(ConfigName)] = ' ';

    pos = strstr(buff,ConfigNameEx);	//获得要写入的条目在文件中的开始位置
    if(pos == NULL)  
	return CFG_FINDITEM_ERROR;

    //要求配置文件的每个条目的格式是  条目名+空格+等号+空格+条目内容+；

    //pos指向条目值的第一个字节,3是" = "的长度
    pos = pos + strlen(ConfigName) + 3;

    cur = pos - buff;			//获得条目值在buff数组中的位置

    counter=0;

    for(i = cur; i < 4096; i++)		//容错处理,过滤掉非法字符
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
		
    strcpy(ConfigValue,tar_value);	//将读出的字符赋值给出参

    return OP_SUCCESS;
}

int CFG_ReadInt(	char *FileName,
					char *ConfigName,
					int  *ConfigValue)
{
    FILE *fp;
    char buff[4096],tar_value[256],ConfigNameEx[256];
	
    int i,counter,cur;			//cur是条目的值的位置
    char *pos;				//pos是要查找的条目在文件中位置
    int rv,len;

    memset(buff,0,sizeof(buff));
    memset(tar_value,0,sizeof(tar_value));
    memset(ConfigNameEx,0,sizeof(ConfigNameEx));

    fp = fopen(FileName,"r");		//打开配置文件
    if(fp == NULL)
	return CFG_FILEOPEN_ERROR;
	
    rv = fseek(fp,0,SEEK_END);		//将文件指定定位到文件结尾
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    len = ftell(fp);			//获得文件长度
    if(len -1 > 4095 || len == -1)	//文件长度非法
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    
    rv = fseek(fp,0,SEEK_SET);		//将文件指针重新指回文件头
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;    	
    }
	
    fread(buff,4096,sizeof(char),fp);
    fclose(fp);
    
    memcpy(ConfigNameEx,ConfigName,strlen(ConfigName) + 1);
    ConfigNameEx[strlen(ConfigName)] = ' ';

    pos = strstr(buff,ConfigNameEx);	//获得要写入的条目在文件中的开始位置
    if(pos == NULL)  
	return CFG_FINDITEM_ERROR;

    //要求配置文件的每个条目的格式是  条目名+空格+等号+空格+条目内容+；

    //pos指向条目值的第一个字节
    pos = pos + strlen(ConfigName) + 3;

    cur = pos - buff;			//获得条目值在buff数组中的位置

    counter=0;

    for(i = cur; i < 4096; i++)		//容错处理,过滤掉非法字符
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
		
    sscanf(tar_value, "%d", ConfigValue);   //将读取的值转换成整型并赋值给出参

    return OP_SUCCESS;
}

int CFG_WriteStr(	char *FileName,
					char *ConfigName,
					char *ConfigValue)
{
    FILE *fp;
    char buff[4096],temp[4096],tar_value[256],decbuf[256],ConfigNameEx[256];
	
    int i,counter,cur,filelen,newlen;	//cur是条目的值的位置
					//counter是字符内容长度计数器
					//newlen是修改后的文件长度
    char *pos;				//pos是要查找的条目在文件中位置
    int rv,len;

    memset(buff,0,sizeof(buff));
    memset(temp,0,sizeof(temp));
    memset(decbuf,0,sizeof(decbuf));
    memset(tar_value,0,sizeof(tar_value));
    memset(ConfigNameEx,0,sizeof(ConfigNameEx));

    memcpy(decbuf,ConfigValue,strlen(ConfigValue));	
/*
    for(i = 0; i < (int)strlen(ConfigValue); i++)   //容错处理,发现非法字符返错
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
		
    fp = fopen(FileName,"rb");		//打开配置文件
    if(fp == NULL)
    {
	return CFG_FILEOPEN_ERROR;
    }
	
    rv = fseek(fp,0,SEEK_END);		//将文件指定定位到文件结尾
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    len = ftell(fp);			//获得文件长度
    if(len -1 > 4095 || len == -1)	//文件长度非法
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    
    rv = fseek(fp,0,SEEK_SET);		//将文件指针重新指回文件头
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;    	
    }
    
    fread(buff,4096,sizeof(char),fp);
    fclose(fp);
    
    memcpy(ConfigNameEx,ConfigName,strlen(ConfigName) + 1);
    ConfigNameEx[strlen(ConfigName)] = ' ';

    filelen = strlen(buff);		//获得文件长度

    pos = strstr(buff,ConfigNameEx);	//获得要写入的条目在文件中的开始位置
    if(pos == NULL)
    {
	return CFG_FINDITEM_ERROR;
    }

    //要求配置文件的每个条目的格式是  条目名+空格+等号+空格+条目内容+；

    //pos指向条目值的第一个字节
    pos = pos + strlen(ConfigName) + 3;

    cur = pos - buff;			//获得条目值在buff数组中的位置

    counter=0;				//给条目内容计数器清0

    for(i=cur;i<4096;i++)		//获得需要修改的条目内容的长度
	if(buff[i]!=';')
	    counter++;
	else
	    break;

    for(i = 0; i < filelen - (cur+counter); i++)
	temp[i] = buff[cur+counter+i];		//保存修改条目后面的内容

    memcpy(&buff[cur],ConfigValue,strlen(ConfigValue));	//将要修改的条目内容写入

    //把不需要修改的部分写回
    memcpy(&buff[cur + strlen(ConfigValue)],temp,filelen-(cur+counter));

    newlen = filelen + (strlen(ConfigValue)-counter);	//获得新的文件长度

    fp = fopen(FileName,"wb");				//将数组写回文件
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
	
    int i,counter,cur,filelen,newlen;	//cur是条目的值的位置
					//counter是字符内容长度计数器
					//newlen是修改后的文件长度
    char *pos;				//pos是要查找的条目在文件中位置
    int rv,len;

    memset(buff,0,sizeof(buff));
    memset(temp,0,sizeof(temp));
    memset(tar_value,0,sizeof(tar_value));
    memset(ConfigValue,0,sizeof(ConfigValue));
    memset(ConfigNameEx,0,sizeof(ConfigNameEx));
	
    sprintf(ConfigValue,"%d",DataValue);	//将传入的整型数值转化为字符型

    fp = fopen(FileName,"rb");
    if(fp == NULL)
	return CFG_FILEOPEN_ERROR;

    rv = fseek(fp,0,SEEK_END);		//将文件定位到文件结尾
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    len = ftell(fp);			//获得文件长度
    if(len -1 > 4095 || len == -1)	//文件长度非法
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;
    }
    
    rv = fseek(fp,0,SEEK_SET);		//将文件指针重新指回文件头
    if(rv)
    {
    	fclose(fp);
	return CFG_FILELEN_ERROR;    	
    }
    
    fread(buff,4096,sizeof(char),fp);	//将文件内容写入数组
    fclose(fp);
    
    memcpy(ConfigNameEx,ConfigName,strlen(ConfigName) + 1);
    ConfigNameEx[strlen(ConfigName)] = ' ';

    filelen = strlen(buff);		//获得文件长度

    pos = strstr(buff,ConfigNameEx);
    if(pos == NULL)  
	return CFG_FINDITEM_ERROR;

    //要求配置文件的每个条目的格式是  条目名+空格+等号+空格+条目内容+；

    pos = pos + strlen(ConfigName) + 3;	//pos指向条目值的第一个字节

    cur = pos - buff;			//获得条目值在buff数组中的位置

    counter=0;				//给条目内容计数器清0

    for(i=cur;i<4096;i++)		//获得需要修改的条目内容的长度
	if(buff[i]!=';')
	    counter++;
	else
	    break;

    for(i = 0; i < filelen - (cur+counter); i++)
	temp[i] = buff[cur+counter+i];	//保存修改条目后面的内容

    memcpy(&buff[cur],ConfigValue,strlen(ConfigValue));	//将要修改的条目内容写入

    //把不需要修改的部分写回
    memcpy(&buff[cur + strlen(ConfigValue)],temp,filelen-(cur+counter));

    newlen = filelen + (strlen(ConfigValue)-counter);	//获得新的文件长度

    fp = fopen(FileName,"wb");				//打开配置文件
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


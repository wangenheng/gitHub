
#ifndef _CFG_FILE_H_
#define _CFG_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

//#define _OS_LINUX_
#define _OS_WIN32_



/**************************************************************************
******************				                    	 ******************
******************					错误码               ******************
******************										 ******************
***************************************************************************/
#define OP_SUCCESS				0
#define	OP_ERRORBASE			0X10010000
#define	CFG_PARAMETER_ERROR		OP_ERRORBASE+0x0001	//输入参数失败
#define	CFG_FILEOPEN_ERROR		OP_ERRORBASE+0x0002	//文件打开失败
#define	CFG_FINDITEM_ERROR		OP_ERRORBASE+0x0003	//找不到指定的条目
#define	CFG_FILELEN_ERROR		OP_ERRORBASE+0x0004	//文件长度非法
#define	CFG_WK_ERROR			OP_ERRORBASE+0x0005	//配置文件工作密钥数量不正确
#define	CFG_MKFILEOPEN_ERROR	OP_ERRORBASE+0x0006	//主密钥文件打开失败
#define	CFG_WKFILEOPEN_ERROR	OP_ERRORBASE+0x0007	//工作密钥文件打开失败
#define	CFG_WRITEINT_ERROR		OP_ERRORBASE+0x0008	//写整型条目失败
#define	CFG_WRITESTR_ERROR		OP_ERRORBASE+0x0009	//写字符型条目失败
#define	CFG_READINT_ERROR		OP_ERRORBASE+0x0010	//读整型条目失败
#define	CFG_READSTR_ERROR		OP_ERRORBASE+0x0011	//读字符型条目失败
#define	CFG_NAME		"C:\\cfg\\cfg.ini"

//-----------------------------底层配置文件读写接口------------------------------

/********************************************************************************************/
/*函 数 名：    CFG_ReadStr                                                                 */
/*功能描述：    读取配置文件中指定参数的整形  值                                            */
/********************************************************************************************/
int CFG_ReadStr(char *FileName,
				char *ConfigName,
				char *ConfigValue);

/********************************************************************************************/
/*函 数 名：    CFG_ReadInt                                                                 */
/*功能描述：    读取配置文件中指定参数的字符型值                                            */
/********************************************************************************************/
int CFG_ReadInt(char *FileName,
				char *ConfigName,
				int  *ConfigValue);

/********************************************************************************************/
/*函 数 名：    CFG_WriteStr                                                                */
/*功能描述：    将字符型值写入配置文件                                                      */
/********************************************************************************************/
int CFG_WriteStr(char *FileName,
				 char *ConfigName,
				 char *ConfigValue);

/********************************************************************************************/
/*函 数 名：    CFG_WriteInt                                                                */
/*功能描述：    将整型值写入配置文件                                                        */
/********************************************************************************************/
int CFG_WriteInt(char *FileName,
				 char *ConfigName,
				 int  DataValue);

/********************************************************************************************/
/*函 数 名：    GetCFGPath                                                                  */
/*功能描述：    获得配置文件路径和名称                                                      */
/********************************************************************************************/
void CFG_GetCFGPath(unsigned char *CFGName);
/********************************************************************************************/
/*函 数 名：    CFG_Create                                                                 */
/*功能描述：    创建配置项                                                      */
/********************************************************************************************/
int CFG_Create(char *FileName,
				 char *ConfigName);
#ifdef __cplusplus
}
#endif

#endif /* _CFG_FILE_H_ */







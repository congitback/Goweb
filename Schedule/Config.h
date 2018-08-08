/******************************************************************************/
/**
*@file    Config.h        
*@brief   配置文件                 
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/
#ifndef __CONFIG_H_
#define __CONFIG_H_
#define  __NET_CARD__
#define  SOFT_VERSIONH   0x01	/*软件版本*/
#define  SOFT_VERSIONM   0x00
#define  SOFT_VERSIONL   0x02

#define  VERSION1  1
#define  VERSION2  2
/* 板间协议模块设备配置 */
#define  MAJOR_PROTO_DEV                 3
#define  PROTO_DEV_CHAN1                 1
#define  PROTO_DEV_CHAN2                 2
#define  PROTO_DEV_CHAN3                 3
#define  PROTO_DEV_CHAN4                 4

/* 外部接口模块配置 */
#define   MAJOR_EXPORT_DEV               3
#define   MINOR1_EXPORT_DEV              5
#define   MINOR2_EXPORT_DEV              6
#define   MINOR3_EXPORT_DEV              7
#define   MINOR4_EXPORT_DEV              8
/*外部接口数据类型*/
#define   DATATYPE_EXPORT_PORT1          1
#define   DATATYPE_EXPORT_PORT2          1
#endif /*CONFIG_H_*/

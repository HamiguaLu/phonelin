
#ifndef __PHONE_LIN_HEADER__
#define __PHONE_LIN_HEADER__




#ifdef _MSC_VER 
typedef __int8  int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t; 
typedef unsigned __int32 uint32_t; 
typedef __int64 int64_t; 
typedef unsigned __int64 uint64_t; 
#else 
#include <stdint.h> 
#endif 



#ifdef Q_OS_WIN32
#include<WinSock2.h>
#else
typedef int8_t   HANDLE;
#endif


typedef uint64_t  ID_TYPE;

#define MAX_DEVICE_ID_LENGTH		(100)
#define MAX_FILE_NAME_LEN			(1024)


#define PL_RET_OK			(0)
#define PL_RET_FAIL			(-1)

#define EA_RET_OK = 0;
#define EA_RET_FAILED = 1;
#define EA_RET_UNKONW_HOST = 2;
#define EA_RET_CONNECT_EXCEPTION = 3;
#define EA_RET_OPEN_FILE_FAILED = 4;
#define EA_RET_END_OF_FILE = 7;
#define EA_RET_UNKONW_REQ = 8;

#define PL_SERVER_PORT                  (12345)
#define PL_BROADCAST_PORT               (15674)


#define  PL_REQ_REPORT_ID               (1000)
#define  PL_REQ_GET_SYS_INFO          	(1001)
#define  PL_REQ_GET_PIM_DATA 			(1010)
#define  PL_REQ_GET_PIM_PHOTO 			(1011)
#define  PL_REQ_SEND_SMS     			(1012)

#define  PL_REQ_GET_APP_LIST			(1020)
#define  PL_REQ_GET_APP_ICON 			(1021)
#define  PL_REQ_DEL_APP 				(1022)
#define  PL_REQ_INSTALL_APP 			(1023)

#define  PL_REQ_GET_FILE_LIST 			(1030)
#define  PL_REQ_DOWNLOAD_FILE 			(1031)
#define  PL_REQ_RENAME_FILE 			(1032)
#define  PL_REQ_DEL_FILE 				(1033)
#define  PL_REQ_NEW_FILE 				(1034)
#define  PL_REQ_UPLOAD_FILE 			(1035)
#define  PL_REQ_GET_SD_CARD_LIST 		(1036)
#define  PL_REQ_COPY_FILE 				(1037)
#define  PL_REQ_CUT_FILE 				(1038)


	
#define	 PL_REQ_GET_MEDIA_LIST 			(1040)



#define  PIM_ACTION_GET_SMS_LIST			(100)
#define  PIM_ACTION_DEL_THREAD				(101)
#define  PIM_ACTION_SEND_SMS				 (102)
#define  PIM_ACTION_GET_CALL_LIST			 (103)
#define  PIM_ACTION_GET_CONTACT_LIST		 (104)
#define  PIM_ACTION_GET_SIM_CONTACT_LIST	 (105)
#define  PIM_ACTION_GET_CONTACT_DATA_LIST	 (106)
#define  PIM_ACTION_GET_CONTACT_EMAIL_LIST	 (107)
#define  PIM_ACTION_GET_CONTACT_PHONE_LIST	 (108)
#define  PIM_ACTION_GET_CONTACT_ADDR_LIST	 (109)


#define  MEDIA_ACTION_GET_AUDIO_LIST		 (130)
#define  MEDIA_ACTION_GET_VIDEO_LIST		 (131)
#define  MEDIA_ACTION_GET_IMAGE_LIST		 (132)


#define  PL_REQ_POST_EVENT 					(1900)
#define  PL_REQ_HEART_BEATEN 				(1999)



#define READ_STATE_WAIT_PACKAGE_HEADER   			0
#define READ_STATE_WAIT_PACKAGE_DATA   				1
#define READ_STATE_WAIT_FILE_JSON_HDR_LEN      			2
#define READ_STATE_WAIT_FILE_JSON_HDR       			3
#define READ_STATE_WAIT_FILE_DATA					4


#define PL_MAIN_BTN_FONT_SIZE                           13

#define PL_DB_NAME         				"PhoneLin.db"


#define	FILE_PATH_SEPERATOR					"/"
QString util_make_path(QString folder,QString file);


enum SubViewIndex {
    HOME_VIEW_IDX = 1,
    SMS_VIEW_IDX = 2,
    CALL_VIEW_IDX = 3,
    CONTACT_VIEW_IDX = 4,
    APP_VIEW_IDX 	= 5,
    FILE_VIEW_IDX = 6,
};


enum FILE_OP_MODE {
    FILE_OP_COPY = 1,
    FILE_OP_CUT = 2,
    FILE_OP_PASTE = 3,
    FILE_OP_DEL = 4,
    FILE_OP_RENAME = 5,
    FILE_OP_NEW = 6
};



#define PE_CONNNETION_STATE_UNKONW  0
#define PE_CONNNETION_STATE_OFFLINE 1
#define PE_CONNNETION_STATE_ONLINE  2


#include <QtCore/QDebug>

#define DEBUG_INFO	qDebug() <<  "[" << __FILE__ << ":" << __LINE__ << ":"  << __FUNCTION__ <<"] "


#endif



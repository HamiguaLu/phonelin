package  mobitnt.util;

import java.io.File;

public class EADefine {
    public static final String TAG = "PHONELIN";
	
	public static final String EA_PREF_NAME = "phoneLinPref";
	public static final String PE_APP_VER = "23";

    public static final int PL_SERVER_PORT = 12345;
    public static final int PL_BROADCAST_PORT = 15674;

    public static final int MAX_FILE_NAME_LEN = 1024;

	//////////////////////////////////////////////////////////////////
	// Error Code list
	public static final int EA_RET_OK = 0;
	public static final int EA_RET_FAILED = 1;
	public static final int EA_RET_UNKONW_HOST = 2;
	public static final int EA_RET_CONNECT_EXCEPTION = 3;
	public static final int EA_RET_OPEN_FILE_FAILED = 4;
	public static final int EA_RET_CHAT_EXCEPTION = 6;
	public static final int EA_RET_ONLY_FILE = 7;
	public static final int EA_RET_UNKONW_REQ = 8;
	

	//action tag
	public static final int PL_REQ_REPORT_ID = 1000;
	public static final int PL_REQ_GET_SYS_INFO = 1001;
	
	public static final int PL_REQ_GET_PIM_DATA = 1010;
	public static final int PL_REQ_GET_PIM_PHOTO = 1011;
	public static final int PL_REQ_SEND_SMS = 1012;


	public static final int PL_REQ_GET_APP_LIST = 1020;
	public static final int PL_REQ_GET_APP_ICON = 1021;
	public static final int PL_REQ_DEL_APP = 1022;
	public static final int PL_REQ_INSTALL_APP = 1023;

	public static final int PL_REQ_GET_FILE_LIST = 1030;
	public static final int PL_REQ_DOWNLOAD_FILE = 1031;
	public static final int PL_REQ_RENAME_FILE = 1032;
	public static final int PL_REQ_DEL_FILE = 1033;
	public static final int PL_REQ_NEW_FILE = 1034;
	public static final int PL_REQ_UPLOAD_FILE = 1035;
	public static final int PL_REQ_GET_SD_CARD_LIST = 1036;
	public static final int PL_REQ_COPY_FILE = 1037;
	public static final int PL_REQ_CUT_FILE  = 1038;

	public static final int PIM_ACTION_GET_SMS_LIST = 100;
	public static final int PIM_ACTION_DEL_THREAD = 101;
	public static final int PIM_ACTION_SEND_SMS = 102;
	public static final int PIM_ACTION_GET_CALL_LIST = 103;
	public static final int PIM_ACTION_GET_CONTACT_LIST = 104;
	public static final int PIM_ACTION_GET_SIM_CONTACT_LIST = 105;
	public static final int PIM_ACTION_GET_CONTACT_DATA_LIST = 106;
	public static final int PIM_ACTION_GET_CONTACT_EMAIL_LIST =	107;
	public static final int PIM_ACTION_GET_CONTACT_PHONE_LIST =	108;
	public static final int PIM_ACTION_GET_CONTACT_ADDR_LIST	= 109;

	public static final int MEDIA_ACTION_GET_AUDIO_LIST = 130;
	public static final int MEDIA_ACTION_GET_VIDEO_LIST = 131;
	public static final int MEDIA_ACTION_GET_IMAGE_LIST = 132;

	public static final int PL_REQ_GET_MEDIA_LIST = 1040;

	public static final int PL_REQ_POST_EVENT = 1900;

	public static final int PL_REQ_HEART_BEATEN = 1999;

	public static final int PL_MAX_JSON_ITEM_COUNT = 500;

    public static final String EA_ACT_SYS_PRODUCT_MODEL_TAG = "ProductModel";
    public static final String EA_ACT_SYS_EXT_AVAILABLE_SPACE = "SDCardAvailableSpace";
    public static final String EA_ACT_SYS_EXT_TOTAL_SPACE = "SDCardTotalSpace";
    public static final String EA_ACT_SYS_AVAIL_RAM = "AvailRAM";
    public static final String EA_ACT_SYS_TOTAL_RAM = "TotalRAM";
    public static final String EA_ACT_SYS_BATTERY_TAG = "BatteryLevel";
    public static final String EA_ACT_SYS_CPU_FREQ_TAG = "CpuFreq";
    public static final String EA_ACT_SYS_CPU_MODEL_TAG = "CpuModel";
    public static final String EA_ACT_SYS_LOG_STATE_TAG = "LogState";

	//sys evt
	public final static int SYS_EVT_NONE = 100;
	public final static int SYS_EVT_SMS_CHANGED = 101;
	public final static int SYS_EVT_CALL_LOG_CHANGED = 102;
	public final static int SYS_EVT_CONTACT_CHANGED = 103;
	public final static int SYS_EVT_CALENDAR_CHANGED = 104;
	public final static int SYS_EVT_SMS_SENT_STATUS = 105;
	public final static int SYS_EVT_SMS_DELIVER_STATUS = 106;
	public final static int SYS_EVT_BATTERY_LEVEL_CHANGED = 107;
	
	//
	public final static String INTENT_ACTION_PHONE_STATE = "android.intent.action.PHONE_STATE";
	public final static String EA_SYS_EXT_AVAILABLE_SPACE = "SDCardAvailableSpace";
	public final static String EA_SYS_EXT_TOTAL_SPACE = "SDCardTotalSpace";


	public final static int UI_MSG_CONNECT_TO_PC = 1;
	
	public final static int UI_MSG_STOP_SERVICE  = 100;
	public final static int UI_MSG_QUERY_SERVICE  = 101;


	public final static int SERVICE_MSG_PC_FOUND = 1000;
	public final static int SERVICE_MSG_SERVICE_ALIVE  = 1001;
	public final static int SERVICE_MSG_SERVICE_STOPPED  = 1002;



	public final static int SERVICE_MSG_HOST_CLOSED = 1100;
	public final static int SERVICE_MSG_CONNECTTION_ERROR  = 1101;
	public final static int SERVICE_MSG_CONNECTTION_UP  = 1102;
	public final static int SERVICE_MSG_FILE_RECV  = 1103;

	
	
	
	
}



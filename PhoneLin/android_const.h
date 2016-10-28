

#ifndef __ANDROID_CONSTANT__
#define __ANDROID_CONSTANT__


#define BASE_TYPE_DATA 							"data1"
#define BASE_TYPE_LABEL 						"data3"
#define BASE_TYPE_TYPE 							"data2"




#define IDENTITY_CONTENT_ITEM_TYPE              "vnd.android.cursor.item/identity"
#define IDENTITY                                "data1"
#define NAMESPACE                               "data2"




#define SIP_ADDR_CONTENT_ITEM_TYPE              "vnd.android.cursor.item/sip_address"
#define SIP_ADDRESS                             "data1"
#define SIP_ADDR_TYPE_HOME                      1
#define SIP_ADDR_TYPE_OTHER                     3
#define SIP_ADDR_TYPE_WORK                      2

#define WEBSITE_CONTENT_ITEM_TYPE               "vnd.android.cursor.item/website"
#define WEBSITE_TYPE_BLOG                       2
#define WEBSITE_TYPE_FTP                        6
#define WEBSITE_TYPE_HOME                       4
#define WEBSITE_TYPE_HOMEPAGE                   1
#define WEBSITE_TYPE_OTHER                      7
#define WEBSITE_TYPE_PROFILE                    3
#define WEBSITE_TYPE_WORK                       5
#define WEBSITE_URL                             "data1"



#define GROUP_MEMBERSHIP_CONTENT_ITEM_TYPE              "vnd.android.cursor.item/group_membership"
#define GROUP_ROW_ID                                    "data1"
#define GROUP_SOURCE_ID                                 "group_sourceid"



#define NOTE_CONTENT_ITEM_TYPE                          "vnd.android.cursor.item/note"
#define NOTE                                            "data1"



#define PHOTO_CONTENT_ITEM_TYPE                         "vnd.android.cursor.item/photo"
#define PHOTO                                           "data15"
#define PHOTO_FILE_ID                                   "data14"



#define EVENT_CONTENT_ITEM_TYPE                         "vnd.android.cursor.item/contact_event"
#define EVENT_START_DATE                                "data1"
#define EVENT_TYPE_ANNIVERSARY                          1
#define EVENT_TYPE_BIRTHDAY                             3
#define EVENT_TYPE_OTHER                                2



#define RELATION_CONTENT_ITEM_TYPE                      "vnd.android.cursor.item/relation"
#define RELATION_NAME                                   "data1"
#define RELATION_TYPE_ASSISTANT                         1
#define RELATION_TYPE_BROTHER                           2
#define RELATION_TYPE_CHILD                             3
#define RELATION_TYPE_DOMESTIC_PARTNER                  4
#define RELATION_TYPE_FATHER                            5
#define RELATION_TYPE_FRIEND                            6
#define RELATION_TYPE_MANAGER                           7
#define RELATION_TYPE_MOTHER                            8
#define RELATION_TYPE_PARENT                            9
#define RELATION_TYPE_PARTNER                           10
#define RELATION_TYPE_REFERRED_BY                       11
#define RELATION_TYPE_RELATIVE                          12
#define RELATION_TYPE_SISTER                            13
#define RELATION_TYPE_SPOUSE                            14


#define ORG_CONTENT_ITEM_TYPE                           "vnd.android.cursor.item/organization"
#define ORG_COMPANY                                     "data1"
#define ORG_DEPARTMENT                                  "data5"
#define ORG_JOB_DESCRIPTION                             "data6"
#define ORG_OFFICE_LOCATION                             "data9"
#define ORG_PHONETIC_NAME                               "data8"
#define ORG_SYMBOL                                      "data7"
#define ORG_TITLE                                       "data4"
#define ORG_TYPE_OTHER                                  2
#define ORG_TYPE_WORK                                   1



#define IM_CONTENT_ITEM_TYPE                            "vnd.android.cursor.item/im"
#define IM_CUSTOM_PROTOCOL                              "data6"
#define IM_PROTOCOL                                     "data5"
#define IM_PROTOCOL_AIM                                 0
#define IM_PROTOCOL_CUSTOM                              -1
#define IM_PROTOCOL_GOOGLE_TALK                         5
#define IM_PROTOCOL_ICQ                                 6
#define IM_PROTOCOL_JABBER                              7
#define IM_PROTOCOL_MSN                                 1
#define IM_PROTOCOL_NETMEETING                          8
#define IM_PROTOCOL_QQ                                  4
#define IM_PROTOCOL_SKYPE                               3
#define IM_PROTOCOL_YAHOO                               2
#define IM_TYPE_HOME                                    1
#define IM_TYPE_OTHER                                   3
#define IM_TYPE_WORK                                    2


#define POSTAL_ADDR_CONTENT_ITEM_TYPE                   "vnd.android.cursor.item/postal-address_v2"
#define POSTAL_ADDR_CITY                                "data7"
#define POSTAL_ADDR_COUNTRY                             "data10"
#define POSTAL_ADDR_FORMATTED_ADDRESS                   "data1"
#define POSTAL_ADDR_NEIGHBORHOOD                        "data6"
#define POSTAL_ADDR_POBOX                               "data5"
#define POSTAL_ADDR_POSTCODE                            "data9"
#define POSTAL_ADDR_REGION                              "data8"
#define POSTAL_ADDR_STREET                              "data4"
#define POSTAL_ADDR_TYPE_HOME                           1
#define POSTAL_ADDR_TYPE_OTHER                          3
#define POSTAL_ADDR_TYPE_WORK                           2


#define EMAIL_CONTENT_ITEM_TYPE                         "vnd.android.cursor.item/email_v2"
#define EMAIL_ADDRESS                                   "data1"
#define EMAIL_DISPLAY_NAME                              "data4"
#define EMAIL_TYPE_HOME                                 1
#define EMAIL_TYPE_MOBILE                               4
#define EMAIL_TYPE_OTHER                                3
#define EMAIL_TYPE_WORK                                 2


#define PHONE_CONTENT_ITEM_TYPE                         "vnd.android.cursor.item/phone_v2"
#define PHONE_NORMALIZED_NUMBER                         "data4"
#define PHONE_NUMBER                                    "data1"
#define PHONE_TYPE_HOME                                 1
#define PHONE_TYPE_MOBILE                               2
#define PHONE_TYPE_WORK                                 3
#define PHONE_TYPE_FAX_WORK                             4
#define PHONE_TYPE_FAX_HOME                             5
#define PHONE_TYPE_PAGER                                6
#define PHONE_TYPE_OTHER                                7
#define PHONE_TYPE_CALLBACK                             8
#define PHONE_TYPE_CAR                                  9
#define PHONE_TYPE_COMPANY_MAIN                         10
#define PHONE_TYPE_ISDN                                 11
#define PHONE_TYPE_MAIN                                 12
#define PHONE_TYPE_OTHER_FAX                            13
#define PHONE_TYPE_RADIO                                14
#define PHONE_TYPE_TELEX                                15
#define PHONE_TYPE_TTY_TDD                              16
#define PHONE_TYPE_WORK_MOBILE                          17
#define PHONE_TYPE_WORK_PAGER                           18
#define PHONE_TYPE_ASSISTANT                            19
#define PHONE_TYPE_MMS                                  20



#define NICKNAME_CONTENT_ITEM_TYPE                      "vnd.android.cursor.item/nickname"
#define NICKNAME_NAME                                   "data1"
#define NICKNAME_TYPE_DEFAULT                           1
#define NICKNAME_TYPE_INITIALS                          5
#define NICKNAME_TYPE_MAIDEN_NAME                       3
#define NICKNAME_TYPE_OTHER_NAME                        2
#define NICKNAME_TYPE_SHORT_NAME                        4



#define NAME_CONTENT_ITEM_TYPE                          "vnd.android.cursor.item/name"
#define NAME_DISPLAY_NAME                               "data1"
#define NAME_FAMILY_NAME                                "data3"
#define NAME_FULL_NAME_STYLE                            "data10"
#define NAME_GIVEN_NAME                                 "data2"
#define NAME_MIDDLE_NAME                                "data5"
#define NAME_PHONETIC_FAMILY_NAME                       "data9"
#define NAME_PHONETIC_GIVEN_NAME                        "data7"
#define NAME_PHONETIC_MIDDLE_NAME                       "data8"
#define NAME_PREFIX                                     "data4"
#define NAME_SUFFIX                                     "data6"


#define MAX_CONTACT_DATA_COUNT							15

/*
String DATA1           "data1"
String DATA10           "data10"
String DATA11           "data11"
String DATA12           "data12"
String DATA13           "data13"
String DATA14           "data14"
String DATA15           "data15"
String DATA2           "data2"
String DATA3           "data3"
String DATA4           "data4"
String DATA5           "data5"
String DATA6           "data6"
String DATA7           "data7"
String DATA8           "data8"
String DATA9           "data9"
//String DATA_VERSION           "data_version"
String MIMETYPE           "mimetype"
*/



enum DisplayNameSources {
    EMAIL = 10,
    NICKNAME = 35,
    ORGANIZATION = 30,
    PHONE = 20,
    STRUCTURED_NAME = 40,
    UNKNOW = 0,
};


enum NameStyle {
    CHINESE = 3,
    CJK = 2,
    JAPANESE = 4,
    KOREAN = 5,
    UNDEFINED = 0,
    WESTERN = 1,
};


enum _CALL_TYPE
{
	CALL_TYPE_INCOMING_CALL	= 1,
	CALL_TYPE_OUTGOING_CALL,
	CALL_TYPE_MISSED_CALL,
	CALL_TYPE_VOICE_MAIL,
	CALL_TYPE_REJECTED,
	CALL_TYPE_REFUSED,
};



#endif


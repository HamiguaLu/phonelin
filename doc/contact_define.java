//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

package android.provider;

import android.accounts.Account;
import android.content.ContentProviderClient;
import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.content.Context;
import android.content.EntityIterator;
import android.content.res.Resources;
import android.database.Cursor;
import android.graphics.Rect;
import android.net.Uri;
import android.os.RemoteException;
import android.provider.BaseColumns;
import android.provider.SyncStateContract.Columns;
import android.util.Pair;
import android.view.View;
import java.io.InputStream;

public final class ContactsContract {
    public static final String AUTHORITY = "com.android.contacts";
    public static final Uri AUTHORITY_URI = null;
    public static final String CALLER_IS_SYNCADAPTER = "caller_is_syncadapter";
    public static final String DEFERRED_SNIPPETING = "deferred_snippeting";
    public static final String DEFERRED_SNIPPETING_QUERY = "deferred_snippeting_query";
    public static final String DIRECTORY_PARAM_KEY = "directory";
    public static final String LIMIT_PARAM_KEY = "limit";
    public static final String PRIMARY_ACCOUNT_NAME = "name_for_primary_account";
    public static final String PRIMARY_ACCOUNT_TYPE = "type_for_primary_account";
    public static final String REMOVE_DUPLICATE_ENTRIES = "remove_duplicate_entries";
    public static final String STREQUENT_PHONE_ONLY = "strequent_phone_only";

    public ContactsContract() {
        throw new RuntimeException("Stub!");
    }

    public static boolean isProfileId(long id) {
        throw new RuntimeException("Stub!");
    }

    public static final class Intents {
        public static final String ATTACH_IMAGE = "com.android.contacts.action.ATTACH_IMAGE";
        public static final String CONTACTS_DATABASE_CREATED = "android.provider.Contacts.DATABASE_CREATED";
        public static final String EXTRA_CREATE_DESCRIPTION = "com.android.contacts.action.CREATE_DESCRIPTION";
        public static final String EXTRA_FORCE_CREATE = "com.android.contacts.action.FORCE_CREATE";
        public static final String INVITE_CONTACT = "com.android.contacts.action.INVITE_CONTACT";
        public static final String SEARCH_SUGGESTION_CLICKED = "android.provider.Contacts.SEARCH_SUGGESTION_CLICKED";
        public static final String SEARCH_SUGGESTION_CREATE_CONTACT_CLICKED = "android.provider.Contacts.SEARCH_SUGGESTION_CREATE_CONTACT_CLICKED";
        public static final String SEARCH_SUGGESTION_DIAL_NUMBER_CLICKED = "android.provider.Contacts.SEARCH_SUGGESTION_DIAL_NUMBER_CLICKED";
        public static final String SHOW_OR_CREATE_CONTACT = "com.android.contacts.action.SHOW_OR_CREATE_CONTACT";

        public Intents() {
            throw new RuntimeException("Stub!");
        }

        public static final class Insert {
            public static final String ACTION = "android.intent.action.INSERT";
            public static final String COMPANY = "company";
            public static final String DATA = "data";
            public static final String EMAIL = "email";
            public static final String EMAIL_ISPRIMARY = "email_isprimary";
            public static final String EMAIL_TYPE = "email_type";
            public static final String FULL_MODE = "full_mode";
            public static final String IM_HANDLE = "im_handle";
            public static final String IM_ISPRIMARY = "im_isprimary";
            public static final String IM_PROTOCOL = "im_protocol";
            public static final String JOB_TITLE = "job_title";
            public static final String NAME = "name";
            public static final String NOTES = "notes";
            public static final String PHONE = "phone";
            public static final String PHONETIC_NAME = "phonetic_name";
            public static final String PHONE_ISPRIMARY = "phone_isprimary";
            public static final String PHONE_TYPE = "phone_type";
            public static final String POSTAL = "postal";
            public static final String POSTAL_ISPRIMARY = "postal_isprimary";
            public static final String POSTAL_TYPE = "postal_type";
            public static final String SECONDARY_EMAIL = "secondary_email";
            public static final String SECONDARY_EMAIL_TYPE = "secondary_email_type";
            public static final String SECONDARY_PHONE = "secondary_phone";
            public static final String SECONDARY_PHONE_TYPE = "secondary_phone_type";
            public static final String TERTIARY_EMAIL = "tertiary_email";
            public static final String TERTIARY_EMAIL_TYPE = "tertiary_email_type";
            public static final String TERTIARY_PHONE = "tertiary_phone";
            public static final String TERTIARY_PHONE_TYPE = "tertiary_phone_type";

            public Insert() {
                throw new RuntimeException("Stub!");
            }
        }
    }

    public static final class DisplayPhoto {
        public static final Uri CONTENT_MAX_DIMENSIONS_URI = null;
        public static final Uri CONTENT_URI = null;
        public static final String DISPLAY_MAX_DIM = "display_max_dim";
        public static final String THUMBNAIL_MAX_DIM = "thumbnail_max_dim";

        DisplayPhoto() {
            throw new RuntimeException("Stub!");
        }
    }

    public static final class QuickContact {
        public static final String ACTION_QUICK_CONTACT = "android.provider.action.QUICK_CONTACT";
        public static final String EXTRA_EXCLUDE_MIMES = "android.provider.extra.EXCLUDE_MIMES";
        public static final int MODE_LARGE = 3;
        public static final int MODE_MEDIUM = 2;
        public static final int MODE_SMALL = 1;

        public QuickContact() {
            throw new RuntimeException("Stub!");
        }

        public static void showQuickContact(Context context, View target, Uri lookupUri, int mode, String[] excludeMimes) {
            throw new RuntimeException("Stub!");
        }

        public static void showQuickContact(Context context, Rect target, Uri lookupUri, int mode, String[] excludeMimes) {
            throw new RuntimeException("Stub!");
        }
    }

    public static final class PinnedPositions {
        public static final int DEMOTED = -1;
        public static final int UNPINNED = 0;

        public PinnedPositions() {
            throw new RuntimeException("Stub!");
        }

        public static void undemote(ContentResolver contentResolver, long contactId) {
            throw new RuntimeException("Stub!");
        }

        public static void pin(ContentResolver contentResolver, long contactId, int pinnedPosition) {
            throw new RuntimeException("Stub!");
        }
    }

    public static final class DataUsageFeedback {
        public static final Uri DELETE_USAGE_URI = null;
        public static final Uri FEEDBACK_URI = null;
        public static final String USAGE_TYPE = "type";
        public static final String USAGE_TYPE_CALL = "call";
        public static final String USAGE_TYPE_LONG_TEXT = "long_text";
        public static final String USAGE_TYPE_SHORT_TEXT = "short_text";

        public DataUsageFeedback() {
            throw new RuntimeException("Stub!");
        }
    }

    public static final class Settings implements ContactsContract.SettingsColumns {
        public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/setting";
        public static final String CONTENT_TYPE = "vnd.android.cursor.dir/setting";
        public static final Uri CONTENT_URI = null;

        Settings() {
            throw new RuntimeException("Stub!");
        }
    }

    protected interface SettingsColumns {
        String ACCOUNT_NAME = "account_name";
        String ACCOUNT_TYPE = "account_type";
        String ANY_UNSYNCED = "any_unsynced";
        String DATA_SET = "data_set";
        String SHOULD_SYNC = "should_sync";
        String UNGROUPED_COUNT = "summ_count";
        String UNGROUPED_VISIBLE = "ungrouped_visible";
        String UNGROUPED_WITH_PHONES = "summ_phones";
    }

    public static final class AggregationExceptions implements BaseColumns {
        public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/aggregation_exception";
        public static final String CONTENT_TYPE = "vnd.android.cursor.dir/aggregation_exception";
        public static final Uri CONTENT_URI = null;
        public static final String RAW_CONTACT_ID1 = "raw_contact_id1";
        public static final String RAW_CONTACT_ID2 = "raw_contact_id2";
        public static final String TYPE = "type";
        public static final int TYPE_AUTOMATIC = 0;
        public static final int TYPE_KEEP_SEPARATE = 2;
        public static final int TYPE_KEEP_TOGETHER = 1;

        AggregationExceptions() {
            throw new RuntimeException("Stub!");
        }
    }

    public static final class Groups implements BaseColumns, ContactsContract.GroupsColumns, ContactsContract.SyncColumns {
        public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/group";
        public static final Uri CONTENT_SUMMARY_URI = null;
        public static final String CONTENT_TYPE = "vnd.android.cursor.dir/group";
        public static final Uri CONTENT_URI = null;

        Groups() {
            throw new RuntimeException("Stub!");
        }

        public static EntityIterator newEntityIterator(Cursor cursor) {
            throw new RuntimeException("Stub!");
        }
    }

    protected interface GroupsColumns {
        String AUTO_ADD = "auto_add";
        String DATA_SET = "data_set";
        String DELETED = "deleted";
        String FAVORITES = "favorites";
        String GROUP_IS_READ_ONLY = "group_is_read_only";
        String GROUP_VISIBLE = "group_visible";
        String NOTES = "notes";
        String RES_PACKAGE = "res_package";
        String SHOULD_SYNC = "should_sync";
        String SUMMARY_COUNT = "summ_count";
        String SUMMARY_WITH_PHONES = "summ_phones";
        String SYSTEM_ID = "system_id";
        String TITLE = "title";
        String TITLE_RES = "title_res";
    }

    public static final class CommonDataKinds {
        CommonDataKinds() {
            throw new RuntimeException("Stub!");
        }

        public static final class Contactables implements ContactsContract.DataColumnsWithJoins, ContactsContract.CommonDataKinds.CommonColumns {
            public static final Uri CONTENT_FILTER_URI = null;
            public static final Uri CONTENT_URI = null;
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String VISIBLE_CONTACTS_ONLY = "visible_contacts_only";

            public Contactables() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Callable implements ContactsContract.DataColumnsWithJoins, ContactsContract.CommonDataKinds.CommonColumns {
            public static final Uri CONTENT_FILTER_URI = null;
            public static final Uri CONTENT_URI = null;
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";

            public Callable() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Identity implements ContactsContract.DataColumnsWithJoins {
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/identity";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String IDENTITY = "data1";
            public static final String NAMESPACE = "data2";

            Identity() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class SipAddress implements ContactsContract.DataColumnsWithJoins, ContactsContract.CommonDataKinds.CommonColumns {
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/sip_address";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String SIP_ADDRESS = "data1";
            public static final int TYPE_HOME = 1;
            public static final int TYPE_OTHER = 3;
            public static final int TYPE_WORK = 2;

            SipAddress() {
                throw new RuntimeException("Stub!");
            }

            public static final int getTypeLabelResource(int type) {
                throw new RuntimeException("Stub!");
            }

            public static final CharSequence getTypeLabel(Resources res, int type, CharSequence label) {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Website implements ContactsContract.DataColumnsWithJoins, ContactsContract.CommonDataKinds.CommonColumns {
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/website";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final int TYPE_BLOG = 2;
            public static final int TYPE_FTP = 6;
            public static final int TYPE_HOME = 4;
            public static final int TYPE_HOMEPAGE = 1;
            public static final int TYPE_OTHER = 7;
            public static final int TYPE_PROFILE = 3;
            public static final int TYPE_WORK = 5;
            public static final String URL = "data1";

            Website() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class GroupMembership implements ContactsContract.DataColumnsWithJoins {
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/group_membership";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String GROUP_ROW_ID = "data1";
            public static final String GROUP_SOURCE_ID = "group_sourceid";

            GroupMembership() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Note implements ContactsContract.DataColumnsWithJoins {
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/note";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String NOTE = "data1";

            Note() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Photo implements ContactsContract.DataColumnsWithJoins {
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/photo";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String PHOTO = "data15";
            public static final String PHOTO_FILE_ID = "data14";

            Photo() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Event implements ContactsContract.DataColumnsWithJoins, ContactsContract.CommonDataKinds.CommonColumns {
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/contact_event";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String START_DATE = "data1";
            public static final int TYPE_ANNIVERSARY = 1;
            public static final int TYPE_BIRTHDAY = 3;
            public static final int TYPE_OTHER = 2;

            Event() {
                throw new RuntimeException("Stub!");
            }

            public static int getTypeResource(Integer type) {
                throw new RuntimeException("Stub!");
            }

            public static final CharSequence getTypeLabel(Resources res, int type, CharSequence label) {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Relation implements ContactsContract.DataColumnsWithJoins, ContactsContract.CommonDataKinds.CommonColumns {
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/relation";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String NAME = "data1";
            public static final int TYPE_ASSISTANT = 1;
            public static final int TYPE_BROTHER = 2;
            public static final int TYPE_CHILD = 3;
            public static final int TYPE_DOMESTIC_PARTNER = 4;
            public static final int TYPE_FATHER = 5;
            public static final int TYPE_FRIEND = 6;
            public static final int TYPE_MANAGER = 7;
            public static final int TYPE_MOTHER = 8;
            public static final int TYPE_PARENT = 9;
            public static final int TYPE_PARTNER = 10;
            public static final int TYPE_REFERRED_BY = 11;
            public static final int TYPE_RELATIVE = 12;
            public static final int TYPE_SISTER = 13;
            public static final int TYPE_SPOUSE = 14;

            Relation() {
                throw new RuntimeException("Stub!");
            }

            public static final int getTypeLabelResource(int type) {
                throw new RuntimeException("Stub!");
            }

            public static final CharSequence getTypeLabel(Resources res, int type, CharSequence label) {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Organization implements ContactsContract.DataColumnsWithJoins, ContactsContract.CommonDataKinds.CommonColumns {
            public static final String COMPANY = "data1";
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/organization";
            public static final String DEPARTMENT = "data5";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String JOB_DESCRIPTION = "data6";
            public static final String OFFICE_LOCATION = "data9";
            public static final String PHONETIC_NAME = "data8";
            public static final String SYMBOL = "data7";
            public static final String TITLE = "data4";
            public static final int TYPE_OTHER = 2;
            public static final int TYPE_WORK = 1;

            Organization() {
                throw new RuntimeException("Stub!");
            }

            public static final int getTypeLabelResource(int type) {
                throw new RuntimeException("Stub!");
            }

            public static final CharSequence getTypeLabel(Resources res, int type, CharSequence label) {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Im implements ContactsContract.DataColumnsWithJoins, ContactsContract.CommonDataKinds.CommonColumns {
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/im";
            public static final String CUSTOM_PROTOCOL = "data6";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String PROTOCOL = "data5";
            public static final int PROTOCOL_AIM = 0;
            public static final int PROTOCOL_CUSTOM = -1;
            public static final int PROTOCOL_GOOGLE_TALK = 5;
            public static final int PROTOCOL_ICQ = 6;
            public static final int PROTOCOL_JABBER = 7;
            public static final int PROTOCOL_MSN = 1;
            public static final int PROTOCOL_NETMEETING = 8;
            public static final int PROTOCOL_QQ = 4;
            public static final int PROTOCOL_SKYPE = 3;
            public static final int PROTOCOL_YAHOO = 2;
            public static final int TYPE_HOME = 1;
            public static final int TYPE_OTHER = 3;
            public static final int TYPE_WORK = 2;

            Im() {
                throw new RuntimeException("Stub!");
            }

            public static final int getTypeLabelResource(int type) {
                throw new RuntimeException("Stub!");
            }

            public static final CharSequence getTypeLabel(Resources res, int type, CharSequence label) {
                throw new RuntimeException("Stub!");
            }

            public static final int getProtocolLabelResource(int type) {
                throw new RuntimeException("Stub!");
            }

            public static final CharSequence getProtocolLabel(Resources res, int type, CharSequence label) {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class StructuredPostal implements ContactsContract.DataColumnsWithJoins, ContactsContract.CommonDataKinds.CommonColumns {
            public static final String CITY = "data7";
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/postal-address_v2";
            public static final String CONTENT_TYPE = "vnd.android.cursor.dir/postal-address_v2";
            public static final Uri CONTENT_URI = null;
            public static final String COUNTRY = "data10";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String FORMATTED_ADDRESS = "data1";
            public static final String NEIGHBORHOOD = "data6";
            public static final String POBOX = "data5";
            public static final String POSTCODE = "data9";
            public static final String REGION = "data8";
            public static final String STREET = "data4";
            public static final int TYPE_HOME = 1;
            public static final int TYPE_OTHER = 3;
            public static final int TYPE_WORK = 2;

            StructuredPostal() {
                throw new RuntimeException("Stub!");
            }

            public static final int getTypeLabelResource(int type) {
                throw new RuntimeException("Stub!");
            }

            public static final CharSequence getTypeLabel(Resources res, int type, CharSequence label) {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Email implements ContactsContract.DataColumnsWithJoins, ContactsContract.CommonDataKinds.CommonColumns {
            public static final String ADDRESS = "data1";
            public static final Uri CONTENT_FILTER_URI = null;
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/email_v2";
            public static final Uri CONTENT_LOOKUP_URI = null;
            public static final String CONTENT_TYPE = "vnd.android.cursor.dir/email_v2";
            public static final Uri CONTENT_URI = null;
            public static final String DISPLAY_NAME = "data4";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final int TYPE_HOME = 1;
            public static final int TYPE_MOBILE = 4;
            public static final int TYPE_OTHER = 3;
            public static final int TYPE_WORK = 2;

            Email() {
                throw new RuntimeException("Stub!");
            }

            public static final int getTypeLabelResource(int type) {
                throw new RuntimeException("Stub!");
            }

            public static final CharSequence getTypeLabel(Resources res, int type, CharSequence label) {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Phone implements ContactsContract.DataColumnsWithJoins, ContactsContract.CommonDataKinds.CommonColumns {
            public static final Uri CONTENT_FILTER_URI = null;
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/phone_v2";
            public static final String CONTENT_TYPE = "vnd.android.cursor.dir/phone_v2";
            public static final Uri CONTENT_URI = null;
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String NORMALIZED_NUMBER = "data4";
            public static final String NUMBER = "data1";
            public static final String SEARCH_DISPLAY_NAME_KEY = "search_display_name";
            public static final String SEARCH_PHONE_NUMBER_KEY = "search_phone_number";
            public static final int TYPE_ASSISTANT = 19;
            public static final int TYPE_CALLBACK = 8;
            public static final int TYPE_CAR = 9;
            public static final int TYPE_COMPANY_MAIN = 10;
            public static final int TYPE_FAX_HOME = 5;
            public static final int TYPE_FAX_WORK = 4;
            public static final int TYPE_HOME = 1;
            public static final int TYPE_ISDN = 11;
            public static final int TYPE_MAIN = 12;
            public static final int TYPE_MMS = 20;
            public static final int TYPE_MOBILE = 2;
            public static final int TYPE_OTHER = 7;
            public static final int TYPE_OTHER_FAX = 13;
            public static final int TYPE_PAGER = 6;
            public static final int TYPE_RADIO = 14;
            public static final int TYPE_TELEX = 15;
            public static final int TYPE_TTY_TDD = 16;
            public static final int TYPE_WORK = 3;
            public static final int TYPE_WORK_MOBILE = 17;
            public static final int TYPE_WORK_PAGER = 18;

            Phone() {
                throw new RuntimeException("Stub!");
            }

            public static final int getTypeLabelResource(int type) {
                throw new RuntimeException("Stub!");
            }

            public static final CharSequence getTypeLabel(Resources res, int type, CharSequence label) {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Nickname implements ContactsContract.DataColumnsWithJoins, ContactsContract.CommonDataKinds.CommonColumns {
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/nickname";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String NAME = "data1";
            public static final int TYPE_DEFAULT = 1;
            public static final int TYPE_INITIALS = 5;
            public static final int TYPE_MAIDEN_NAME = 3;
            /** @deprecated */
            @Deprecated
            public static final int TYPE_MAINDEN_NAME = 3;
            public static final int TYPE_OTHER_NAME = 2;
            public static final int TYPE_SHORT_NAME = 4;

            Nickname() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class StructuredName implements ContactsContract.DataColumnsWithJoins {
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/name";
            public static final String DISPLAY_NAME = "data1";
            public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
            public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
            public static final String FAMILY_NAME = "data3";
            public static final String FULL_NAME_STYLE = "data10";
            public static final String GIVEN_NAME = "data2";
            public static final String MIDDLE_NAME = "data5";
            public static final String PHONETIC_FAMILY_NAME = "data9";
            public static final String PHONETIC_GIVEN_NAME = "data7";
            public static final String PHONETIC_MIDDLE_NAME = "data8";
            public static final String PREFIX = "data4";
            public static final String SUFFIX = "data6";

            StructuredName() {
                throw new RuntimeException("Stub!");
            }
        }

        protected interface CommonColumns extends ContactsContract.CommonDataKinds.BaseTypes {
            String DATA = "data1";
            String LABEL = "data3";
            String TYPE = "data2";
        }

        public interface BaseTypes {
            int TYPE_CUSTOM = 0;
        }
    }

    public static class SearchSnippets {
        public static final String DEFERRED_SNIPPETING_KEY = "deferred_snippeting";
        public static final String SNIPPET = "snippet";

        public SearchSnippets() {
            throw new RuntimeException("Stub!");
        }
    }

    /** @deprecated */
    @Deprecated
    public static final class Presence extends ContactsContract.StatusUpdates {
        public Presence() {
            throw new RuntimeException("Stub!");
        }
    }

    public static class StatusUpdates implements ContactsContract.StatusColumns, ContactsContract.PresenceColumns {
        public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/status-update";
        public static final String CONTENT_TYPE = "vnd.android.cursor.dir/status-update";
        public static final Uri CONTENT_URI = null;
        public static final Uri PROFILE_CONTENT_URI = null;

        StatusUpdates() {
            throw new RuntimeException("Stub!");
        }

        public static final int getPresenceIconResourceId(int status) {
            throw new RuntimeException("Stub!");
        }

        public static final int getPresencePrecedence(int status) {
            throw new RuntimeException("Stub!");
        }
    }

    protected interface PresenceColumns {
        String CUSTOM_PROTOCOL = "custom_protocol";
        String DATA_ID = "presence_data_id";
        String IM_ACCOUNT = "im_account";
        String IM_HANDLE = "im_handle";
        String PROTOCOL = "protocol";
    }

    public static final class PhoneLookup implements BaseColumns, ContactsContract.PhoneLookupColumns, ContactsContract.ContactsColumns, ContactsContract.ContactOptionsColumns {
        public static final Uri CONTENT_FILTER_URI = null;
        public static final Uri ENTERPRISE_CONTENT_FILTER_URI = null;
        public static final String QUERY_PARAMETER_SIP_ADDRESS = "sip";

        PhoneLookup() {
            throw new RuntimeException("Stub!");
        }
    }

    protected interface PhoneLookupColumns {
        String LABEL = "label";
        String NORMALIZED_NUMBER = "normalized_number";
        String NUMBER = "number";
        String TYPE = "type";
    }

    public static final class RawContactsEntity implements BaseColumns, ContactsContract.DataColumns, ContactsContract.RawContactsColumns {
        public static final String CONTENT_TYPE = "vnd.android.cursor.dir/raw_contact_entity";
        public static final Uri CONTENT_URI = null;
        public static final String DATA_ID = "data_id";
        public static final Uri PROFILE_CONTENT_URI = null;

        RawContactsEntity() {
            throw new RuntimeException("Stub!");
        }
    }

    public static final class Data implements ContactsContract.DataColumnsWithJoins {
        public static final String CONTENT_TYPE = "vnd.android.cursor.dir/data";
        public static final Uri CONTENT_URI = null;
        public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
        public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
        public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";
        public static final String VISIBLE_CONTACTS_ONLY = "visible_contacts_only";

        Data() {
            throw new RuntimeException("Stub!");
        }

        public static Uri getContactLookupUri(ContentResolver resolver, Uri dataUri) {
            throw new RuntimeException("Stub!");
        }
    }

    protected interface DataColumnsWithJoins extends BaseColumns, ContactsContract.DataColumns, ContactsContract.StatusColumns, ContactsContract.RawContactsColumns, ContactsContract.ContactsColumns, ContactsContract.ContactNameColumns, ContactsContract.ContactOptionsColumns, ContactsContract.ContactStatusColumns, ContactsContract.DataUsageStatColumns {
    }

    protected interface DataUsageStatColumns {
        String LAST_TIME_USED = "last_time_used";
        String TIMES_USED = "times_used";
    }

    protected interface DataColumns {
        String DATA1 = "data1";
        String DATA10 = "data10";
        String DATA11 = "data11";
        String DATA12 = "data12";
        String DATA13 = "data13";
        String DATA14 = "data14";
        String DATA15 = "data15";
        String DATA2 = "data2";
        String DATA3 = "data3";
        String DATA4 = "data4";
        String DATA5 = "data5";
        String DATA6 = "data6";
        String DATA7 = "data7";
        String DATA8 = "data8";
        String DATA9 = "data9";
        String DATA_VERSION = "data_version";
        String IS_PRIMARY = "is_primary";
        String IS_READ_ONLY = "is_read_only";
        String IS_SUPER_PRIMARY = "is_super_primary";
        String MIMETYPE = "mimetype";
        String RAW_CONTACT_ID = "raw_contact_id";
        String RES_PACKAGE = "res_package";
        String SYNC1 = "data_sync1";
        String SYNC2 = "data_sync2";
        String SYNC3 = "data_sync3";
        String SYNC4 = "data_sync4";
    }

    /** @deprecated */
    @Deprecated
    protected interface StreamItemPhotosColumns {
        /** @deprecated */
        @Deprecated
        String PHOTO_FILE_ID = "photo_file_id";
        /** @deprecated */
        @Deprecated
        String PHOTO_URI = "photo_uri";
        /** @deprecated */
        @Deprecated
        String SORT_INDEX = "sort_index";
        /** @deprecated */
        @Deprecated
        String STREAM_ITEM_ID = "stream_item_id";
        /** @deprecated */
        @Deprecated
        String SYNC1 = "stream_item_photo_sync1";
        /** @deprecated */
        @Deprecated
        String SYNC2 = "stream_item_photo_sync2";
        /** @deprecated */
        @Deprecated
        String SYNC3 = "stream_item_photo_sync3";
        /** @deprecated */
        @Deprecated
        String SYNC4 = "stream_item_photo_sync4";
    }

    /** @deprecated */
    @Deprecated
    public static final class StreamItemPhotos implements BaseColumns, ContactsContract.StreamItemPhotosColumns {
        /** @deprecated */
        @Deprecated
        public static final String PHOTO = "photo";

        StreamItemPhotos() {
            throw new RuntimeException("Stub!");
        }
    }

    /** @deprecated */
    @Deprecated
    protected interface StreamItemsColumns {
        /** @deprecated */
        @Deprecated
        String ACCOUNT_NAME = "account_name";
        /** @deprecated */
        @Deprecated
        String ACCOUNT_TYPE = "account_type";
        /** @deprecated */
        @Deprecated
        String COMMENTS = "comments";
        /** @deprecated */
        @Deprecated
        String CONTACT_ID = "contact_id";
        /** @deprecated */
        @Deprecated
        String CONTACT_LOOKUP_KEY = "contact_lookup";
        /** @deprecated */
        @Deprecated
        String DATA_SET = "data_set";
        /** @deprecated */
        @Deprecated
        String RAW_CONTACT_ID = "raw_contact_id";
        /** @deprecated */
        @Deprecated
        String RAW_CONTACT_SOURCE_ID = "raw_contact_source_id";
        /** @deprecated */
        @Deprecated
        String RES_ICON = "icon";
        /** @deprecated */
        @Deprecated
        String RES_LABEL = "label";
        /** @deprecated */
        @Deprecated
        String RES_PACKAGE = "res_package";
        /** @deprecated */
        @Deprecated
        String SYNC1 = "stream_item_sync1";
        /** @deprecated */
        @Deprecated
        String SYNC2 = "stream_item_sync2";
        /** @deprecated */
        @Deprecated
        String SYNC3 = "stream_item_sync3";
        /** @deprecated */
        @Deprecated
        String SYNC4 = "stream_item_sync4";
        /** @deprecated */
        @Deprecated
        String TEXT = "text";
        /** @deprecated */
        @Deprecated
        String TIMESTAMP = "timestamp";
    }

    /** @deprecated */
    @Deprecated
    public static final class StreamItems implements BaseColumns, ContactsContract.StreamItemsColumns {
        /** @deprecated */
        @Deprecated
        public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/stream_item";
        /** @deprecated */
        @Deprecated
        public static final Uri CONTENT_LIMIT_URI = null;
        /** @deprecated */
        @Deprecated
        public static final Uri CONTENT_PHOTO_URI = null;
        /** @deprecated */
        @Deprecated
        public static final String CONTENT_TYPE = "vnd.android.cursor.dir/stream_item";
        /** @deprecated */
        @Deprecated
        public static final Uri CONTENT_URI = null;
        /** @deprecated */
        @Deprecated
        public static final String MAX_ITEMS = "max_items";

        StreamItems() {
            throw new RuntimeException("Stub!");
        }

        /** @deprecated */
        @Deprecated
        public static final class StreamItemPhotos implements BaseColumns, ContactsContract.StreamItemPhotosColumns {
            /** @deprecated */
            @Deprecated
            public static final String CONTENT_DIRECTORY = "photo";
            /** @deprecated */
            @Deprecated
            public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/stream_item_photo";
            /** @deprecated */
            @Deprecated
            public static final String CONTENT_TYPE = "vnd.android.cursor.dir/stream_item_photo";

            StreamItemPhotos() {
                throw new RuntimeException("Stub!");
            }
        }
    }

    protected interface StatusColumns {
        int AVAILABLE = 5;
        int AWAY = 2;
        int CAPABILITY_HAS_CAMERA = 4;
        int CAPABILITY_HAS_VIDEO = 2;
        int CAPABILITY_HAS_VOICE = 1;
        String CHAT_CAPABILITY = "chat_capability";
        int DO_NOT_DISTURB = 4;
        int IDLE = 3;
        int INVISIBLE = 1;
        int OFFLINE = 0;
        String PRESENCE = "mode";
        /** @deprecated */
        @Deprecated
        String PRESENCE_CUSTOM_STATUS = "status";
        /** @deprecated */
        @Deprecated
        String PRESENCE_STATUS = "mode";
        String STATUS = "status";
        String STATUS_ICON = "status_icon";
        String STATUS_LABEL = "status_label";
        String STATUS_RES_PACKAGE = "status_res_package";
        String STATUS_TIMESTAMP = "status_ts";
    }

    public static final class RawContacts implements BaseColumns, ContactsContract.RawContactsColumns, ContactsContract.ContactOptionsColumns, ContactsContract.ContactNameColumns, ContactsContract.SyncColumns {
        public static final int AGGREGATION_MODE_DEFAULT = 0;
        public static final int AGGREGATION_MODE_DISABLED = 3;
        /** @deprecated */
        @Deprecated
        public static final int AGGREGATION_MODE_IMMEDIATE = 1;
        public static final int AGGREGATION_MODE_SUSPENDED = 2;
        public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/raw_contact";
        public static final String CONTENT_TYPE = "vnd.android.cursor.dir/raw_contact";
        public static final Uri CONTENT_URI = null;

        RawContacts() {
            throw new RuntimeException("Stub!");
        }

        public static Uri getContactLookupUri(ContentResolver resolver, Uri rawContactUri) {
            throw new RuntimeException("Stub!");
        }

        public static EntityIterator newEntityIterator(Cursor cursor) {
            throw new RuntimeException("Stub!");
        }

        public static final class DisplayPhoto {
            public static final String CONTENT_DIRECTORY = "display_photo";

            DisplayPhoto() {
                throw new RuntimeException("Stub!");
            }
        }

        /** @deprecated */
        @Deprecated
        public static final class StreamItems implements BaseColumns, ContactsContract.StreamItemsColumns {
            /** @deprecated */
            @Deprecated
            public static final String CONTENT_DIRECTORY = "stream_items";

            StreamItems() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Entity implements BaseColumns, ContactsContract.DataColumns {
            public static final String CONTENT_DIRECTORY = "entity";
            public static final String DATA_ID = "data_id";

            Entity() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Data implements BaseColumns, ContactsContract.DataColumns {
            public static final String CONTENT_DIRECTORY = "data";

            Data() {
                throw new RuntimeException("Stub!");
            }
        }
    }

    protected interface RawContactsColumns {
        String ACCOUNT_TYPE_AND_DATA_SET = "account_type_and_data_set";
        String AGGREGATION_MODE = "aggregation_mode";
        String CONTACT_ID = "contact_id";
        String DATA_SET = "data_set";
        String DELETED = "deleted";
        String RAW_CONTACT_IS_READ_ONLY = "raw_contact_is_read_only";
        String RAW_CONTACT_IS_USER_PROFILE = "raw_contact_is_user_profile";
    }

    public static final class DeletedContacts implements ContactsContract.DeletedContactsColumns {
        public static final Uri CONTENT_URI = null;
        public static final long DAYS_KEPT_MILLISECONDS = 2592000000L;

        DeletedContacts() {
            throw new RuntimeException("Stub!");
        }
    }

    protected interface DeletedContactsColumns {
        String CONTACT_DELETED_TIMESTAMP = "contact_deleted_timestamp";
        String CONTACT_ID = "contact_id";
    }

    public static final class Profile implements BaseColumns, ContactsContract.ContactsColumns, ContactsContract.ContactOptionsColumns, ContactsContract.ContactNameColumns, ContactsContract.ContactStatusColumns {
        public static final Uri CONTENT_RAW_CONTACTS_URI = null;
        public static final Uri CONTENT_URI = null;
        public static final Uri CONTENT_VCARD_URI = null;
        public static final long MIN_ID = 9223372034707292160L;

        Profile() {
            throw new RuntimeException("Stub!");
        }
    }

    public static class Contacts implements BaseColumns, ContactsContract.ContactsColumns, ContactsContract.ContactOptionsColumns, ContactsContract.ContactNameColumns, ContactsContract.ContactStatusColumns {
        public static final Uri CONTENT_FILTER_URI = null;
        public static final Uri CONTENT_FREQUENT_URI = null;
        public static final Uri CONTENT_GROUP_URI = null;
        public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/contact";
        public static final Uri CONTENT_LOOKUP_URI = null;
        public static final Uri CONTENT_MULTI_VCARD_URI = null;
        public static final Uri CONTENT_STREQUENT_FILTER_URI = null;
        public static final Uri CONTENT_STREQUENT_URI = null;
        public static final String CONTENT_TYPE = "vnd.android.cursor.dir/contact";
        public static final Uri CONTENT_URI = null;
        public static final String CONTENT_VCARD_TYPE = "text/x-vcard";
        public static final Uri CONTENT_VCARD_URI = null;
        public static final String EXTRA_ADDRESS_BOOK_INDEX = "android.provider.extra.ADDRESS_BOOK_INDEX";
        public static final String EXTRA_ADDRESS_BOOK_INDEX_COUNTS = "android.provider.extra.ADDRESS_BOOK_INDEX_COUNTS";
        public static final String EXTRA_ADDRESS_BOOK_INDEX_TITLES = "android.provider.extra.ADDRESS_BOOK_INDEX_TITLES";

        Contacts() {
            throw new RuntimeException("Stub!");
        }

        public static Uri getLookupUri(ContentResolver resolver, Uri contactUri) {
            throw new RuntimeException("Stub!");
        }

        public static Uri getLookupUri(long contactId, String lookupKey) {
            throw new RuntimeException("Stub!");
        }

        public static Uri lookupContact(ContentResolver resolver, Uri lookupUri) {
            throw new RuntimeException("Stub!");
        }

        /** @deprecated */
        @Deprecated
        public static void markAsContacted(ContentResolver resolver, long contactId) {
            throw new RuntimeException("Stub!");
        }

        public static boolean isEnterpriseContactId(long contactId) {
            throw new RuntimeException("Stub!");
        }

        public static InputStream openContactPhotoInputStream(ContentResolver cr, Uri contactUri, boolean preferHighres) {
            throw new RuntimeException("Stub!");
        }

        public static InputStream openContactPhotoInputStream(ContentResolver cr, Uri contactUri) {
            throw new RuntimeException("Stub!");
        }

        public static final class Photo implements BaseColumns, ContactsContract.DataColumnsWithJoins {
            public static final String CONTENT_DIRECTORY = "photo";
            public static final String DISPLAY_PHOTO = "display_photo";
            public static final String PHOTO = "data15";
            public static final String PHOTO_FILE_ID = "data14";

            Photo() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class AggregationSuggestions implements BaseColumns, ContactsContract.ContactsColumns, ContactsContract.ContactOptionsColumns, ContactsContract.ContactStatusColumns {
            public static final String CONTENT_DIRECTORY = "suggestions";

            AggregationSuggestions() {
                throw new RuntimeException("Stub!");
            }
        }

        /** @deprecated */
        @Deprecated
        public static final class StreamItems implements ContactsContract.StreamItemsColumns {
            /** @deprecated */
            @Deprecated
            public static final String CONTENT_DIRECTORY = "stream_items";

            StreamItems() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Entity implements BaseColumns, ContactsContract.ContactsColumns, ContactsContract.ContactNameColumns, ContactsContract.RawContactsColumns, ContactsContract.BaseSyncColumns, ContactsContract.SyncColumns, ContactsContract.DataColumns, ContactsContract.StatusColumns, ContactsContract.ContactOptionsColumns, ContactsContract.ContactStatusColumns, ContactsContract.DataUsageStatColumns {
            public static final String CONTENT_DIRECTORY = "entities";
            public static final String DATA_ID = "data_id";
            public static final String RAW_CONTACT_ID = "raw_contact_id";

            Entity() {
                throw new RuntimeException("Stub!");
            }
        }

        public static final class Data implements BaseColumns, ContactsContract.DataColumns {
            public static final String CONTENT_DIRECTORY = "data";

            Data() {
                throw new RuntimeException("Stub!");
            }
        }
    }

    protected interface ContactNameColumns {
        String DISPLAY_NAME_ALTERNATIVE = "display_name_alt";
        String DISPLAY_NAME_PRIMARY = "display_name";
        String DISPLAY_NAME_SOURCE = "display_name_source";
        String PHONETIC_NAME = "phonetic_name";
        String PHONETIC_NAME_STYLE = "phonetic_name_style";
        String SORT_KEY_ALTERNATIVE = "sort_key_alt";
        String SORT_KEY_PRIMARY = "sort_key";
    }

    public interface DisplayNameSources {
        int EMAIL = 10;
        int NICKNAME = 35;
        int ORGANIZATION = 30;
        int PHONE = 20;
        int STRUCTURED_NAME = 40;
        int UNDEFINED = 0;
    }

    public interface PhoneticNameStyle {
        int JAPANESE = 4;
        int KOREAN = 5;
        int PINYIN = 3;
        int UNDEFINED = 0;
    }

    public interface FullNameStyle {
        int CHINESE = 3;
        int CJK = 2;
        int JAPANESE = 4;
        int KOREAN = 5;
        int UNDEFINED = 0;
        int WESTERN = 1;
    }

    protected interface ContactStatusColumns {
        String CONTACT_CHAT_CAPABILITY = "contact_chat_capability";
        String CONTACT_PRESENCE = "contact_presence";
        String CONTACT_STATUS = "contact_status";
        String CONTACT_STATUS_ICON = "contact_status_icon";
        String CONTACT_STATUS_LABEL = "contact_status_label";
        String CONTACT_STATUS_RES_PACKAGE = "contact_status_res_package";
        String CONTACT_STATUS_TIMESTAMP = "contact_status_ts";
    }

    protected interface ContactsColumns {
        String CONTACT_LAST_UPDATED_TIMESTAMP = "contact_last_updated_timestamp";
        String DISPLAY_NAME = "display_name";
        String HAS_PHONE_NUMBER = "has_phone_number";
        String IN_DEFAULT_DIRECTORY = "in_default_directory";
        String IN_VISIBLE_GROUP = "in_visible_group";
        String IS_USER_PROFILE = "is_user_profile";
        String LOOKUP_KEY = "lookup";
        String NAME_RAW_CONTACT_ID = "name_raw_contact_id";
        String PHOTO_FILE_ID = "photo_file_id";
        String PHOTO_ID = "photo_id";
        String PHOTO_THUMBNAIL_URI = "photo_thumb_uri";
        String PHOTO_URI = "photo_uri";
    }

    protected interface ContactOptionsColumns {
        String CUSTOM_RINGTONE = "custom_ringtone";
        String LAST_TIME_CONTACTED = "last_time_contacted";
        String PINNED = "pinned";
        String SEND_TO_VOICEMAIL = "send_to_voicemail";
        String STARRED = "starred";
        String TIMES_CONTACTED = "times_contacted";
    }

    protected interface SyncColumns extends ContactsContract.BaseSyncColumns {
        String ACCOUNT_NAME = "account_name";
        String ACCOUNT_TYPE = "account_type";
        String DIRTY = "dirty";
        String SOURCE_ID = "sourceid";
        String VERSION = "version";
    }

    protected interface BaseSyncColumns {
        String SYNC1 = "sync1";
        String SYNC2 = "sync2";
        String SYNC3 = "sync3";
        String SYNC4 = "sync4";
    }

    public static final class ProfileSyncState implements Columns {
        public static final String CONTENT_DIRECTORY = "syncstate";
        public static final Uri CONTENT_URI = null;

        ProfileSyncState() {
            throw new RuntimeException("Stub!");
        }

        public static byte[] get(ContentProviderClient provider, Account account) throws RemoteException {
            throw new RuntimeException("Stub!");
        }

        public static Pair<Uri, byte[]> getWithUri(ContentProviderClient provider, Account account) throws RemoteException {
            throw new RuntimeException("Stub!");
        }

        public static void set(ContentProviderClient provider, Account account, byte[] data) throws RemoteException {
            throw new RuntimeException("Stub!");
        }

        public static ContentProviderOperation newSetOperation(Account account, byte[] data) {
            throw new RuntimeException("Stub!");
        }
    }

    public static final class SyncState implements Columns {
        public static final String CONTENT_DIRECTORY = "syncstate";
        public static final Uri CONTENT_URI = null;

        SyncState() {
            throw new RuntimeException("Stub!");
        }

        public static byte[] get(ContentProviderClient provider, Account account) throws RemoteException {
            throw new RuntimeException("Stub!");
        }

        public static Pair<Uri, byte[]> getWithUri(ContentProviderClient provider, Account account) throws RemoteException {
            throw new RuntimeException("Stub!");
        }

        public static void set(ContentProviderClient provider, Account account, byte[] data) throws RemoteException {
            throw new RuntimeException("Stub!");
        }

        public static ContentProviderOperation newSetOperation(Account account, byte[] data) {
            throw new RuntimeException("Stub!");
        }
    }

    public static final class Directory implements BaseColumns {
        public static final String ACCOUNT_NAME = "accountName";
        public static final String ACCOUNT_TYPE = "accountType";
        public static final String CONTENT_ITEM_TYPE = "vnd.android.cursor.item/contact_directory";
        public static final String CONTENT_TYPE = "vnd.android.cursor.dir/contact_directories";
        public static final Uri CONTENT_URI = null;
        public static final long DEFAULT = 0L;
        public static final String DIRECTORY_AUTHORITY = "authority";
        public static final String DISPLAY_NAME = "displayName";
        public static final String EXPORT_SUPPORT = "exportSupport";
        public static final int EXPORT_SUPPORT_ANY_ACCOUNT = 2;
        public static final int EXPORT_SUPPORT_NONE = 0;
        public static final int EXPORT_SUPPORT_SAME_ACCOUNT_ONLY = 1;
        public static final long LOCAL_INVISIBLE = 1L;
        public static final String PACKAGE_NAME = "packageName";
        public static final String PHOTO_SUPPORT = "photoSupport";
        public static final int PHOTO_SUPPORT_FULL = 3;
        public static final int PHOTO_SUPPORT_FULL_SIZE_ONLY = 2;
        public static final int PHOTO_SUPPORT_NONE = 0;
        public static final int PHOTO_SUPPORT_THUMBNAIL_ONLY = 1;
        public static final String SHORTCUT_SUPPORT = "shortcutSupport";
        public static final int SHORTCUT_SUPPORT_DATA_ITEMS_ONLY = 1;
        public static final int SHORTCUT_SUPPORT_FULL = 2;
        public static final int SHORTCUT_SUPPORT_NONE = 0;
        public static final String TYPE_RESOURCE_ID = "typeResourceId";

        Directory() {
            throw new RuntimeException("Stub!");
        }

        public static void notifyDirectoryChange(ContentResolver resolver) {
            throw new RuntimeException("Stub!");
        }
    }
}


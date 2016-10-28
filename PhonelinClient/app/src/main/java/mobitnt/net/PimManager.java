
package mobitnt.net;


import org.json.JSONArray;
import org.json.JSONObject;

import android.accounts.Account;
import android.accounts.AccountManager;
import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.provider.CallLog;
import android.provider.ContactsContract;
import android.telephony.SmsManager;
import android.util.Log;

import mobitnt.phonelin.PhoneLinService;
import mobitnt.util.EADefine;
import mobitnt.util.EAUtil;


import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;


public class PimManager extends ReqHandler {

    public String GetPhoneNo(String sPhoneEntry) {
        if (sPhoneEntry == null || sPhoneEntry.length() < 1) {
            return "";
        }

        String sNo = "";
        for (int i = 0; i < sPhoneEntry.length(); ++i) {
            char c = sPhoneEntry.charAt(i);
            if (Character.isDigit(c)) {
                sNo += c;
            }
        }

        return sNo;
    }

    int sendSMS(String sPhone, String message, String sTimeStamp) {

        String sVal = sPhone + sTimeStamp;
        sPhone = GetPhoneNo(sPhone);

		/*
         * Pattern pattern = Pattern.compile("[0-9]*"); Matcher isNum =
		 * pattern.matcher(sPhone); if (!isNum.matches()) { String phoneNo =
		 * sVal; phoneNo += "=" +
		 * String.valueOf(EADefine.EA_RET_INVALID_PHONE_NO);
		 * SysApi.PushSysEvt(SysApi.SYS_EVT_SMS_SENT_STATUS, phoneNo); return
		 * EADefine.EA_RET_INVALID_PHONE_NO; }
		 */

        String SENT = "SMS_SENT" + System.currentTimeMillis();
        String DELIVERED = "SMS_DELIVERED" + System.currentTimeMillis();

        Intent sentIntent = new Intent(SENT);
        sentIntent.putExtra("timestamp", sVal);
        sentIntent.putExtra("smscontent", message);
        sentIntent.putExtra("phoneNo", sPhone);
        PendingIntent sentPI = PendingIntent.getBroadcast(
                EAUtil.GetEAContext(), (int) System.currentTimeMillis(),
                sentIntent, 0);

        Intent deliveredIntent = new Intent(DELIVERED);
        deliveredIntent.putExtra("timestamp", sVal);
        deliveredIntent.putExtra("smscontent", message);
        deliveredIntent.putExtra("phoneNo", sPhone);

        PendingIntent deliveredPI = PendingIntent.getBroadcast(
                EAUtil.GetEAContext(), (int) System.currentTimeMillis(),
                deliveredIntent, 0);

        // ---when the SMS has been sent---
        EAUtil.GetEAContext().registerReceiver(new BroadcastReceiver() {
            @Override
            public void onReceive(Context arg0, Intent arg1) {
                String timestamp = arg1.getExtras().getString("timestamp");
                String phoneNo = arg1.getExtras().getString("phoneNo");
                String sContent = arg1.getExtras().getString("smscontent");
                String sState = timestamp;
                switch (getResultCode()) {
                    case Activity.RESULT_OK:
                        sState += ":" + String.valueOf(EADefine.EA_RET_OK) + ":"
                                + phoneNo;
                        long lDate = new java.util.Date().getTime();
                        //insertsms(phoneNo, sContent, "0", "2", lDate);
                        break;
                    case SmsManager.RESULT_ERROR_GENERIC_FAILURE:
                    case SmsManager.RESULT_ERROR_NO_SERVICE:
                    case SmsManager.RESULT_ERROR_NULL_PDU:
                    case SmsManager.RESULT_ERROR_RADIO_OFF:
                    default:
                        sState += ":" + String.valueOf(EADefine.EA_RET_FAILED)
                                + ":" + phoneNo;
                        break;
                }
                PhoneLinService.sendEvt2Host("SYS_EVT_SMS_SENT_STATUS:" + sState);
                //SysApi.PushSysEvt(EADefine.SYS_EVT_SMS_SENT_STATUS, sState);
            }
        }, new IntentFilter(SENT));

        // ---when the SMS has been delivered---
        EAUtil.GetEAContext().registerReceiver(new BroadcastReceiver() {

            @Override
            public void onReceive(Context arg0, Intent arg1) {
                String timestamp = arg1.getExtras().getString("timestamp");
                String phoneNo = arg1.getExtras().getString("phoneNo");
                // String sContent = arg1.getExtras().getString("smscontent");
                String sState = timestamp;
                switch (getResultCode()) {
                    case Activity.RESULT_OK:
                        sState += ":" + String.valueOf(EADefine.EA_RET_OK) + ":"
                                + phoneNo;
                        break;
                    case Activity.RESULT_CANCELED:
                    default:
                        sState += ":" + String.valueOf(EADefine.EA_RET_FAILED)
                                + ":" + phoneNo;
                        break;
                }
                PhoneLinService.sendEvt2Host("SYS_EVT_SMS_DELIVER_STATUS:" + sState);
                //SysApi.PushSysEvt(EADefine.SYS_EVT_SMS_DELIVER_STATUS, sState);
            }
        }, new IntentFilter(DELIVERED));

        try {
            SmsManager smsMgr = SmsManager.getDefault();
            ArrayList<String> msgArray = smsMgr.divideMessage(message);
            int iSize = msgArray.size();
            if (iSize > 1) {

                smsMgr.sendMultipartTextMessage(sPhone, null, msgArray, null, null);
                String sState = sVal;
                sState += ":" + String.valueOf(EADefine.EA_RET_OK) + ":" + sPhone;
                long lDate = new java.util.Date().getTime();
                //insertsms(sPhone,message, "0", "2", lDate);
                //SysApi.PushSysEvt(EADefine.SYS_EVT_SMS_SENT_STATUS, sState);
                PhoneLinService.sendEvt2Host("SYS_EVT_SMS_SENT_STATUS:" + sState);
            } else {
                smsMgr.sendTextMessage(sPhone, null, message, sentPI, deliveredPI);
            }
        } catch (Exception e) {
            String sState = ":" + String.valueOf(EADefine.EA_RET_FAILED) + ":"
                    + sPhone;
            //SysApi.PushSysEvt(EADefine.SYS_EVT_SMS_SENT_STATUS, sState);
            PhoneLinService.sendEvt2Host("SYS_EVT_SMS_SENT_STATUS:" + sState);
            return EADefine.EA_RET_FAILED;
        }

        return EADefine.EA_RET_OK;
    }


     public JSONArray getData(String uri, String[] projection,
                                    String selection, String[] selectionArgs, String sortOrder) {

        ContentResolver cr = EAUtil.GetContentResolver();
        if (cr == null) {
            return null;
        }

      /*  Account[] accountList = AccountManager.get(EAUtil.GetEAContext()).getAccounts();
        String accountSelection = "";
        for(int i = 0 ; i < accountList.length ; i++) {
            if(accountSelection.length() != 0)
                accountSelection = accountSelection + " AND ";
            accountSelection = accountSelection + ContactsContract.Groups.ACCOUNT_TYPE + " != '" +  accountList[i].type + "'";
        }*/

        Cursor cur = cr.query(Uri.parse(uri), projection, selection, selectionArgs, sortOrder);
        if (cur == null) {
            return null;
        }

        if (!cur.moveToFirst()) {
            cur.close();
            return null;
        }

        JSONArray ret = new JSONArray();

        int iMaxCount = EADefine.PL_MAX_JSON_ITEM_COUNT;
        do {
            JSONObject o = EAUtil.Cursor2Json(cur);
            ret.put(o);
        } while (cur.moveToNext() && --iMaxCount > 0);

        cur.close();
        return ret;
    }

    public byte[] openPhoto(long contactId) {
        Uri contactUri = ContentUris.withAppendedId(ContactsContract.Contacts.CONTENT_URI, contactId);
        Uri photoUri = Uri.withAppendedPath(contactUri, ContactsContract.Contacts.Photo.CONTENT_DIRECTORY);
        Cursor cursor = EAUtil.GetContentResolver().query(photoUri,
                new String[]{ContactsContract.Contacts.Photo.PHOTO}, null, null, null);
        if (cursor == null) {
            return null;
        }
        try {
            if (cursor.moveToFirst()) {
                byte[] data = cursor.getBlob(0);
                if (data == null) {
                    return null;
                }

                Bitmap bm = BitmapFactory.decodeStream(new ByteArrayInputStream(data));
                ByteArrayOutputStream iconStream = new ByteArrayOutputStream();
                bm.compress(Bitmap.CompressFormat.PNG, 100, iconStream);

                cursor.close();

                if (iconStream == null) {
                    return null;
                }
                return iconStream.toByteArray();
            }
        } catch (Exception e) {
            Log.e(EADefine.TAG, "PIM:" + e.toString());
        }
        return null;

    }

    public byte[] processRequest(int action, JSONObject req) {

        if (action == EADefine.PL_REQ_GET_PIM_PHOTO) {
            try {
                long contact_id = req.getLong("contact_id");
                String sCID = String.valueOf(contact_id);
                byte[] iconData = openPhoto(contact_id);
                int dataLen = iconData != null ? iconData.length : 0;
                if (sCID.getBytes().length > 254) {
                    return null;
                }

                byte[] ret = new byte[255 + dataLen];
                Arrays.fill(ret, 0, 254, (byte) 0);
                System.arraycopy(sCID.getBytes(), 0, ret, 0, sCID.getBytes().length);
                if (dataLen > 0) {
                    System.arraycopy(iconData, 0, ret, 255, dataLen);
                }

                return ret;
            } catch (Exception e) {
                Log.e(EADefine.TAG, "PIM:" + e.toString());
            }

            return null;
        }
        try {
            int pim_action = req.getInt("pim_action");

            if (action == EADefine.PL_REQ_SEND_SMS) {
                String recipients = req.getString("recipients");
                String content = req.getString("content");
                int ret = sendSMS(recipients, content, "");
                JSONObject retObj = new JSONObject();
                retObj.put("pim_action", pim_action);
                retObj.put("recipients",recipients);
                retObj.put("retCode",ret);
                return json2Byte(retObj);
            }

            if (action != EADefine.PL_REQ_GET_PIM_DATA) {
                return null;
            }
            String uri = "";
          /*  switch (pim_action){
                case EADefine.PIM_ACTION_GET_SMS_LIST:{
                    uri = "content://sms/";
                    break;
                }
                case EADefine.PIM_ACTION_GET_CALL_LIST:{
                    uri = CallLog.Calls.CONTENT_URI.toString();
                    break;
                }
                case EADefine.PIM_ACTION_GET_CONTACT_LIST:{
                   uri =  ContactsContract.Contacts.CONTENT_URI.toString();
                    break;
                }
                case EADefine.PIM_ACTION_GET_CONTACT_DATA_LIST:{
                    uri = ContactsContract.Data.CONTENT_URI.toString();
                    break;
                }
                case EADefine.PIM_ACTION_GET_CONTACT_EMAIL_LIST:{
                    uri = ContactsContract.CommonDataKinds.Email.CONTENT_URI.toString();
                    break;
                }
                case EADefine.PIM_ACTION_GET_CONTACT_PHONE_LIST:{
                    uri = ContactsContract.CommonDataKinds.Phone.CONTENT_URI.toString();
                    break;
                }
                case EADefine.PIM_ACTION_GET_CONTACT_ADDR_LIST:{
                    uri = ContactsContract.CommonDataKinds.Phone.CONTACT_ID.toString();
                    break;
                }
            }*/

            if (req.has("uri")) {
                uri = req.getString("uri");
            }

            int projCount = 0;
            if (req.has("projCount")) {
                projCount = req.getInt("projCount");
            }

            JSONObject proj = null;
            if (req.has("projection")) {
                proj = req.getJSONObject("projection");
            }

            String sel = req.getString("selection");
            if (req.has("selection")) {
                sel = req.getString("selection");
            }

            JSONObject arg = null;
            if (req.has("argument")) {
                arg = req.getJSONObject("argument");
            }

            int argCount = 0;
            if (req.has("argCount")) {
                argCount = req.getInt("argCount");
            }

            String sortOrder = "";
            if (req.has("sort_order")) {
                sortOrder = req.getString("sort_order");
            }

            String[] projection = null;
            if (projCount > 0) {
                projection = new String[projCount];
                for (int i = 0; i < projCount; ++i) {
                    projection[i] = proj.getString("proj" + i);
                }
            }

            if (sel != null) {
                sel = sel.trim();
                if (sel.length() == 0) {
                    sel = null;
                }
            }

            String[] selectionArgs = null;
            if (argCount > 0) {
                selectionArgs = new String[argCount];
                for (int i = 0; i < argCount; ++i) {
                    selectionArgs[i] = arg.getString("arg" + i);
                }
            }

            JSONObject ret = new JSONObject();
            ret.put("pim_action", pim_action);

            JSONArray retdata = getData(uri, projection, sel, selectionArgs, sortOrder);
            if (retdata != null) {
                ret.put("data", retdata);
            }

            return json2Byte(ret);
        } catch (Exception e) {
            Log.e(EADefine.TAG, "PIM:" + e.toString());
        }

        return null;
    }

}

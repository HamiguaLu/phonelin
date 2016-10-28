package  mobitnt.util;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.InetAddress;
import java.net.URLEncoder;
import java.net.UnknownHostException;
import java.util.Random;


import android.content.ContentResolver;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.database.Cursor;
import android.telephony.TelephonyManager;
import android.util.Log;

import org.json.JSONObject;

import mobitnt.net.FileManager;

public class EAUtil {
	static Context eaContext;
	static InetAddress m_ClientIP = null;
	
	public static final int SRV_STATE_WAIT = 0;
	public static final int SRV_STATE_BACKUP_SMS = 1;
	public static final int SRV_STATE_BACKUP_CALL = 2;
	public static final int SRV_STATE_RESOTORE_SMS = 3;
	public static final int SRV_STATE_RESOTORE_CALL = 4;
	static int m_iSrvState = SRV_STATE_WAIT;
	
	public static Context GetEAContext(){
		return eaContext;
	}
	
	public static String GetResString(int id){
		if (eaContext == null){
			return null;
		}
		
		return eaContext.getString(id);
	}
	
	public static void SetEAContext(Context c){
		eaContext = c;
	}

	public static String CHECK_STRING(String s,String DefValue){
		if (s == null || s.length() < 1){
			return DefValue;
		}
		
		return s;
	}

	static public String GetPEFolderOnSDCard() {
		String sPath = FileManager.getExternalStoragePath();
		if (sPath == null || sPath.length() < 2) {
			return "";
		}

		sPath += "/PhoneLin";
		FileManager.createFile(sPath,true);

		return sPath;
	}

	static public ContentResolver GetContentResolver() {
		if (eaContext != null) {
			return eaContext.getContentResolver();
		}
		return null;
	}

	static String GenRandomNumber(int iLen) {
		String s = "";
		for (int i = 0; i < iLen; ++i) {
			int r = new Random().nextInt(10);
			s += String.valueOf(r);
		}

		return s;
	}

	static public String getLastHostIP() {
		SharedPreferences sp = EAUtil.GetEAContext().getSharedPreferences(
				EADefine.EA_PREF_NAME, 0);
		if (sp == null) {
			//MobiTNTLog.write("GetPhoneID: can not open pref");
			return "";
		}

		return sp.getString("LastHostIP", "");
	}

	static public void setLastHostIP(String ip) {
		SharedPreferences sp = EAUtil.GetEAContext().getSharedPreferences(
				EADefine.EA_PREF_NAME, 0);
		if (sp == null) {
			//MobiTNTLog.write("GetPhoneID: can not open pref");
			return;
		}

		sp.edit().putString("LastHostIP",ip).apply();

		return;
	}


	static public String GetPhoneID() {
		TelephonyManager telephonyManager = (TelephonyManager)EAUtil.GetEAContext().getSystemService(Context.TELEPHONY_SERVICE);
		return telephonyManager.getDeviceId();
	}


	static public String EncodeItem(String s) {
		String sVal = EAUtil.CHECK_STRING(s, " ");

		try {
			sVal = URLEncoder.encode(sVal, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return sVal;
	}
	
	static public JSONObject Cursor2Json(Cursor cur){
		if (cur == null) {
			return null;
		}

		JSONObject json = new JSONObject();
        String[] colNames = cur.getColumnNames();
		for (int i = 0; i < colNames.length; ++i) {
			String sCol = colNames[i];
			sCol = sCol.trim();
			if (sCol.length() < 1){
				continue;
			}
			
			/*if (sCol.replaceAll("[a-z]*[A-Z]*\\d*-*_*\\s*", "")
					.length() != 0) {
				continue;
			}
			
			if (sCol.contains("sync_") || sCol.contains("_sync")) {
				continue;
			}*/
			
			String sVal = "";
			try{
				if (cur.getType(i) == Cursor.FIELD_TYPE_BLOB){
					sVal = cur.getBlob(i).toString();
				}else{
					sVal = cur.getString(i);
				}
			}catch (Exception e){
				sVal = "";
                Log.e(EADefine.TAG, e.toString());
			}
			
			if (sVal == null || sVal.length() < 1) {
				continue;
			}

			sVal = sVal.trim();
			if (sVal.length() > 0) {
                try{
				    json.put(sCol, sVal);
                }catch (Exception e){
                    Log.e(EADefine.TAG, e.toString());
                }
			}
		}

		return json;
	}

	static public String Cursor2Xml(Cursor cur) {
		if (cur == null) {
			return "";
		}

		StringBuilder sXml = new StringBuilder("");
		String[] colNames = cur.getColumnNames();
		for (int i = 0; i < colNames.length; ++i) {
			String sCol = colNames[i];
			sCol = sCol.trim();
			if (sCol.length() < 1){
				continue;
			}
			
			if (sCol.replaceAll("[a-z]*[A-Z]*\\d*-*_*\\s*", "")
					.length() != 0) {
				continue;
			}
			
			if (sCol.contains("sync_") || sCol.contains("_sync")) {
				continue;
			}
			
			String sVal = "";
			try{
				sVal = cur.getString(i);
			}catch (Exception e){
				sVal = "";
			}
			
			if (sVal == null || sVal.length() < 1) {
				continue;
			}

			sVal = sVal.trim();
			if (sVal.length() > 0) {
				sXml.append("<" + sCol + ">");
				sXml.append(EAUtil.EncodeItem(cur.getString(i)));
				sXml.append("</" + sCol + ">");
			}
		}

		return sXml.toString();
	}
	

	public static boolean IsInternetReachable() {
		try {
			InetAddress.getByName("www.google.com").isReachable(3);
			return true;
		} catch (UnknownHostException e) {
			return false;
		} catch (IOException e) {
			return false;
		}
	}

}

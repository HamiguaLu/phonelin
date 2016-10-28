package mobitnt.net;

import android.app.ActivityManager;
import android.content.Context;
import android.os.Environment;
import android.os.StatFs;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import org.json.JSONException;
import org.json.JSONObject;


import mobitnt.util.*;


public class SysManager extends ReqHandler {
    
    public static int m_iBatteryLevel;

    public List<String> loadSysInfo() {
        String[] sMemInfo = getMemoryInfo();
        String[] sStorageInfo = getStorageInfo();
        String[] sModelInfo = getModelInfo();
        String[] sCpuInfo = getCpuInfo();

        List<String> sysInfo = new ArrayList<String>();

        sysInfo.add("BatteryLevel:" + String.valueOf(m_iBatteryLevel));

        if (sMemInfo != null) {
            for (int i = 0; i < sMemInfo.length; ++i) {
                sysInfo.add(sMemInfo[i]);
            }
        }

        if (sStorageInfo != null) {
            for (int i = 0; i < sStorageInfo.length; ++i) {
                sysInfo.add(sStorageInfo[i]);
            }
        }

        if (sModelInfo != null) {
            for (int i = 0; i < sModelInfo.length; ++i) {
                sysInfo.add(sModelInfo[i]);
            }
        }

        if (sCpuInfo != null) {
            for (int i = 0; i < sCpuInfo.length; ++i) {
                sysInfo.add(sCpuInfo[i]);
            }
        }

        return sysInfo;
    }


    public String getImei() {
        return EAUtil.GetPhoneID();
    }

    String[] getModelInfo() {
        String[] modelInfo = {EADefine.EA_ACT_SYS_PRODUCT_MODEL_TAG + ":"
                + android.os.Build.MODEL/*
                                         * , String.format("SDK version:%s",
										 * android.os.Build.VERSION.SDK),
										 * String.format("Release Version:%s",
										 * android.os.Build.VERSION.RELEASE),
										 */
        };
        // String.format("SDCard total space:%s", sTotalSize) };
        return modelInfo;
    }

    String[] getStorageInfo() {
        String sdcard = Environment.getExternalStorageDirectory().getPath();
        File file = new File(sdcard);
        StatFs statFs = new StatFs(file.getPath());

        long bs = statFs.getBlockSize();
        long ab = statFs.getAvailableBlocks();
        long bc = statFs.getBlockCount();

        long lAvailableSpace = bs * (ab - 4);
        long lTotalSpace = bc * bs;

        // String sASize = formatSize(lAvailableSpace);
        // String sTotalSize = formatSize(lTotalSpace);

        String[] storageInfo = {
                EADefine.EA_SYS_EXT_AVAILABLE_SPACE + ":"
                        + String.valueOf(lAvailableSpace),
                EADefine.EA_SYS_EXT_TOTAL_SPACE + ":"
                        + String.valueOf(lTotalSpace)};

        return storageInfo;
    }

    String[] getMemoryInfo() {
        ActivityManager activityManager = (ActivityManager) EAUtil.GetEAContext()
                .getSystemService(Context.ACTIVITY_SERVICE);
        ActivityManager.MemoryInfo outInfo = new ActivityManager.MemoryInfo();
        activityManager.getMemoryInfo(outInfo);

        String sAvailableMem = String.valueOf(outInfo.availMem / 1024);//formatSize(outInfo.availMem);
        String sTotalRAM = getTotalRAM();

        sTotalRAM = sTotalRAM.toUpperCase();
        sTotalRAM = sTotalRAM.replace("KB", "");

        String[] sysInfo = {EADefine.EA_ACT_SYS_AVAIL_RAM + ":" + sAvailableMem,
                EADefine.EA_ACT_SYS_TOTAL_RAM + ":" + sTotalRAM
        };
        return sysInfo;
    }

    String getTotalRAM() {
        String sPath = "/proc/meminfo";
        String sItem = "";
        try {
            FileReader fr = new FileReader(sPath);
            BufferedReader localBufferedReader = new BufferedReader(fr, 8192);
            while ((sItem = localBufferedReader.readLine()) != null) {
                if (sItem.contains("MemTotal")) {
                    sItem = sItem.replace("MemTotal:", "");
                    localBufferedReader.close();
                    return sItem.trim();
                }
            }
            localBufferedReader.close();
        } catch (IOException e) {
        }

        return "UNKONW";
    }

    String[] getCpuInfo() {
        String sPath = "/proc/cpuinfo";
        String sItem = "";
        String[] cpuInfo = {"", ""};
        String[] arrayOfString;
        try {
            FileReader fr = new FileReader(sPath);
            BufferedReader localBufferedReader = new BufferedReader(fr, 8192);
            sItem = localBufferedReader.readLine();
            arrayOfString = sItem.split("\\s+");
            for (int i = 2; i < arrayOfString.length; i++) {
                cpuInfo[0] = cpuInfo[0] + arrayOfString[i] + " ";
            }
            sItem = localBufferedReader.readLine();
            arrayOfString = sItem.split("\\s+");
            cpuInfo[1] += arrayOfString[2];
            localBufferedReader.close();
        } catch (IOException e) {
        }

        String[] info = {EADefine.EA_ACT_SYS_CPU_MODEL_TAG + ":" + cpuInfo[0], EADefine.EA_ACT_SYS_CPU_FREQ_TAG + ":" + cpuInfo[1]};
        return info;
    }


    JSONObject getSysInfo() throws JSONException {
        JSONObject sysInfo = new JSONObject();

        List<String> sSysInfo = loadSysInfo();
        if (sSysInfo == null || sSysInfo.size() < 1) {
            return null;
        }

        for (int i = 0; i < sSysInfo.size(); ++i) {
            String sItmes[] = sSysInfo.get(i).split(":");

            if (sItmes.length == 2) {
                sysInfo.put(sItmes[0], sItmes[1]);
            }
        }

        return sysInfo;
    }

    public byte[] processRequest(int action, JSONObject req) throws Exception {

        try {
            JSONObject sys = getSysInfo();
            return json2Byte(sys);

        } catch (JSONException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        //String sLangType = parms.getProperty(EADefine.EA_ACT_LANG_TYPE_TAG);
		
		
	/*	String sAction = parms.getProperty(EADefine.EA_ACT_ACTION_TAG,EADefine.EA_ACT_GET_SYS_INFO_TAG);
		if (sAction != null && sAction.length() > 0) {
			return "var g_sIMEI=" + SysApi.getImei();
		}
			
		String sImei = parms.getProperty(EADefine.EA_ACT_CHECK_IMEI_TAG);
		if (sImei != null && sImei.length() > 0) {
			if (sImei.equals(SysApi.getImei())){
				return genRetCode(EADefine.EA_RET_OK);
			}
			
			return genRetCode(EADefine.EA_RET_FAILED);
		}*/


        return null;
    }
}

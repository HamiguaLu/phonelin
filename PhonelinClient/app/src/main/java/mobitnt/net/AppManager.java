package mobitnt.net;

import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Properties;

import org.json.JSONArray;
import org.json.JSONObject;

import mobitnt.util.*;


public class AppManager extends ReqHandler {

    public byte[] GetAppIcon(String sPackageName) {
        List<PackageInfo> packs = EAUtil.GetEAContext().getPackageManager()
                .getInstalledPackages(PackageManager.GET_ACTIVITIES);

        int iCount = packs.size();
        if (iCount == 0) {
            return null;
        }

        PackageManager pm = EAUtil.GetEAContext().getPackageManager();

        try {

            for (int i = 0; i < iCount; i++) {
                PackageInfo p = packs.get(i);
                if ((p.applicationInfo.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) == 0
                        && (p.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) != 0) {
                    continue;
                }

                if (!p.packageName.equals(sPackageName)) {
                    continue;
                }

                Drawable icon = p.applicationInfo.loadIcon(EAUtil
                        .GetEAContext().getPackageManager());

                BitmapDrawable bd = (BitmapDrawable) icon;
                Bitmap bm = bd.getBitmap();

                ByteArrayOutputStream iconStream = new ByteArrayOutputStream();
                bm.compress(Bitmap.CompressFormat.PNG, 100, iconStream);
                if (iconStream == null){
                    return null;
                }
                return iconStream.toByteArray();
            }
        } catch (Exception e) {
            // TODO: handle exception
            e.printStackTrace();
        }

        return null;

    }


    JSONArray getInstalledApps() {
        List<PackageInfo> packs = EAUtil.GetEAContext().getPackageManager()
                .getInstalledPackages(PackageManager.GET_ACTIVITIES);

        int iCount = packs.size();
        if (iCount == 0) {
            return null;
        }

        JSONArray ret = new JSONArray();

        try {
            for (int i = 0; i < iCount; i++) {
                PackageInfo p = packs.get(i);
                if ((p.applicationInfo.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) == 0
                        && (p.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) != 0) {
                    continue;
                }

                JSONObject appInfo = new JSONObject();

                appInfo.put("appname", p.applicationInfo.loadLabel(
                        EAUtil.GetEAContext().getPackageManager()).toString());
                appInfo.put("packageName", p.packageName);
                appInfo.put("versionName", p.versionName);
                appInfo.put("versionCode", p.versionCode);

                ret.put(appInfo);
            }

            return ret;

        } catch (Exception e) {
            // TODO: handle exception
            e.printStackTrace();

        }

        return null;
    }

    public String GetApkStorePath() {
        return EAUtil.GetEAContext().getExternalFilesDir(null).getAbsolutePath();
    }

    public void InstallApp(String sApkPath) {
        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.setDataAndType(Uri.parse("file://" + sApkPath), "application/vnd.android.package-archive");
        EAUtil.GetEAContext().startActivity(intent);
    }

    public void RemoveApp(String sPackageName) {
        Uri packageURI = Uri.fromParts("package", sPackageName, null);
        Intent uninstallIntent = new Intent(Intent.ACTION_DELETE, packageURI);
        uninstallIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        EAUtil.GetEAContext().startActivity(uninstallIntent);
        return;
    }


    public byte[] processRequest(int action, JSONObject req) throws Exception{
		if (action == EADefine.PL_REQ_GET_APP_ICON) {
			String apkPath = req.getString("packageName");
			byte[] iconData = GetAppIcon(apkPath);
            int dataLen = iconData != null ? iconData.length : 0;
            if (apkPath.getBytes().length > 254){
                return null;
            }

            byte[] ret = new byte[255 + dataLen];
            Arrays.fill(ret,0,254,(byte)0);
            System.arraycopy(apkPath.getBytes(),0,ret,0,apkPath.getBytes().length);
            if (dataLen > 0){
                System.arraycopy(iconData,0,ret,255,dataLen);
            }

            return ret;
		}
		
		if (action == EADefine.PL_REQ_DEL_APP) {
			String apkPath = req.getString("packageName");
			RemoveApp(apkPath);
			return genRetCode(EADefine.EA_RET_OK);
		}
		
		if (action == EADefine.PL_REQ_INSTALL_APP ){
			String apkPath = req.getString("fileName");
			InstallApp(apkPath);
			return genRetCode(EADefine.EA_RET_OK);
		}
		
	
        if (action == EADefine.PL_REQ_GET_APP_LIST) {

			int from = req.getInt("from");

            try {
                JSONArray apps = getInstalledApps();
                JSONObject ret = new JSONObject();

                if (apps != null) {
                    ret.put("data", apps);
                }
                return json2Byte(ret);
            } catch (Exception e) {
                Log.e(EADefine.TAG, "APP:" + e.toString());
            }
        }

        return genRetCode(EADefine.EA_RET_UNKONW_REQ);

    }


}



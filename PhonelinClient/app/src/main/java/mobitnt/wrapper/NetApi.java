package mobitnt.wrapper;

import java.lang.reflect.Method;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;


import mobitnt.util.EADefine;
import mobitnt.util.EAUtil;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;

public class NetApi {

	static public String getLocalMacAddress() {
		if (EAUtil.GetEAContext() == null) {
			return "";
		}

		WifiManager wifi = (WifiManager) EAUtil.GetEAContext()
				.getSystemService(Context.WIFI_SERVICE);
		WifiInfo info = wifi.getConnectionInfo();

		return info.getMacAddress();
	}

	public static boolean isWifiApEnabled() {
		WifiManager wifi = (WifiManager) EAUtil.GetEAContext()
				.getSystemService(Context.WIFI_SERVICE);
		Method[] wmMethods = wifi.getClass().getDeclaredMethods();
		boolean bEnabled = false;
		for (Method method : wmMethods) {
			if (method.getName().equals("isWifiApEnabled")) {

				try {
					bEnabled = (Boolean) method.invoke(wifi);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		
		return bEnabled;
	}

	public static boolean isWifiEnabled() {
		if (EAUtil.GetEAContext() == null) {
			return false;
		}

		ConnectivityManager cm = (ConnectivityManager) EAUtil.GetEAContext()
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		// cm.getActiveNetworkInfo();
		NetworkInfo[] netInfo = cm.getAllNetworkInfo();
		if (netInfo == null || netInfo.length < 1) {
			return false;
		}

		for (NetworkInfo ni : netInfo) {
			if (ni.getTypeName().equalsIgnoreCase("WIFI"))
				if (ni.isConnected()) {
					return true;
				}
		}
		
		return false;
	}
	
	public static String getWifiApIpAddress() {
	    try {
	    	Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces();
	    	if (en == null){
	    		return null;
	    	}
	    	
	        for (; en.hasMoreElements();) {
	            NetworkInterface intf = en.nextElement();
	            String sInetName = intf.getName();
	            if (sInetName.contains("wlan") || sInetName.contains("wl") || sInetName.contains("ap") ) {
	                for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr
	                        .hasMoreElements();) {
	                    InetAddress inetAddress = enumIpAddr.nextElement();
	                    if (!inetAddress.isLoopbackAddress()
	                            && (inetAddress.getAddress().length == 4)) {
	                        return inetAddress.getHostAddress();
	                    }
	                }
	            }
	        }
	    } catch (SocketException ex) {

	    }
	    return null;
	}

	public static String getLocalIpAddress() {
		if (EAUtil.GetEAContext() == null) {
			return "127.0.0.1";
		}

		if (!isWifiEnabled()) {
			String sIP = getWifiApIpAddress();
			if (sIP == null){
				// if no wifi available,usb USB instead
				return "127.0.0.1";
			}
			return sIP;
		}

		WifiManager wifi_service = (WifiManager) EAUtil.GetEAContext()
				.getSystemService(Context.WIFI_SERVICE);
		WifiInfo wifiinfo = wifi_service.getConnectionInfo();
		int ipAddress = wifiinfo.getIpAddress();

		return String.format(Locale.ENGLISH, "%d.%d.%d.%d", (ipAddress & 0xff),
				(ipAddress >> 8 & 0xff), (ipAddress >> 16 & 0xff),
				(ipAddress >> 24 & 0xff));

	}



	static public void ReportIP() {
		if (!isWifiEnabled()) {


			return;
		}


		//SrvSock.acquireWakeLock();

		String sWifi = getLocalIpAddress();
		

		//MobiTNTLog.write("ReportIP success:" + sResp);

		StopReportTimer();
	}

	static public void StopReportTimer() {

	}

	static public void StartReportTimer() {
		StopReportTimer();


	}

}

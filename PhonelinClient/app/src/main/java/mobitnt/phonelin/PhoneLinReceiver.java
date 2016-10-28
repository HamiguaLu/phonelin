package mobitnt.phonelin;

import mobitnt.net.SysManager;
import mobitnt.wrapper.NetApi;
import mobitnt.util.EADefine;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;

public class PhoneLinReceiver extends BroadcastReceiver {
    //static final String ACTION = "android.intent.action.BOOT_COMPLETED";

    Context my_context;


    @Override
    public void onReceive(Context context, Intent intent) {
        my_context = context;

        String action = intent.getAction();

        if (action.equals(PhoneLinService.UI_ACTION)) {
            int msg = intent.getIntExtra("msg_type", 0);
            if (msg == EADefine.UI_MSG_CONNECT_TO_PC) {
                String clientIp = intent.getStringExtra("data");
                PhoneLinService.startClientThread(clientIp);
                return;
            }

            if (msg == EADefine.UI_MSG_STOP_SERVICE) {
                PhoneLinService.onStopSelf();
                return;
            }
        }

        if (action.equals(Intent.ACTION_BATTERY_CHANGED)) {
            int level = intent.getIntExtra("level", 0);
            SysManager.m_iBatteryLevel = level;
			
            new Thread() {
                public void run() {
                    PhoneLinService.sendEvt2Host("SYS_EVT_BATTERY_LEVEL_CHANGED");
                }
            }.start();

            return;
        }

        if (action.equals(EADefine.INTENT_ACTION_PHONE_STATE)) {
            new Thread() {
                public void run() {
                    PhoneLinService.sendEvt2Host("SYS_EVT_CALL_LOG_CHANGED");
                }
            }.start();
            return;
        }


        if (action.equals(PhoneLinService.smsAction)) {
            new Thread() {
                public void run() {
                    PhoneLinService.sendEvt2Host("SYS_EVT_SMS_RECEIVED");
                }
            }.start();
            return;
        }

        /*if (action.equals(WifiManager.SUPPLICANT_CONNECTION_CHANGE_ACTION)) {
            boolean bConnected = intent.getBooleanExtra(
                    WifiManager.EXTRA_SUPPLICANT_CONNECTED, false);
            if (!bConnected) {
                return;
            }


            NetApi.StartReportTimer();
            return;
        }

        if (action.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) {
            int wifiState = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE,
                    WifiManager.WIFI_STATE_UNKNOWN);

            if (wifiState != WifiManager.WIFI_STATE_ENABLED) {
                return;
            }

            NetApi.StartReportTimer();
            return;
        }

        if (action.equals(WifiManager.NETWORK_STATE_CHANGED_ACTION)) {
            NetworkInfo info = (NetworkInfo) intent.getParcelableExtra(WifiManager.EXTRA_NETWORK_INFO);
            if (info == null) {
                return;
            }

            if (info.getState().equals(NetworkInfo.State.CONNECTED)) {
                NetApi.StartReportTimer();
            }

            return;
        }

		if (action.equals("android.intent.action.BOOT_COMPLETED")) {
            final Intent srvintent = new Intent();
			srvintent.setAction("mobitnt.net.YASYNCSERVICE");
			try {
				ComponentName srvName = context.startService(srvintent);
				if (srvName == null) {
					//
					MobiTNTLog.write("Boot:Start service failed");
				} else {
					String sName = srvName.getClassName();
					MobiTNTLog.write("Boot:service started  " + sName);
				}
			} catch (Exception ex) {
				ex.printStackTrace();
				MobiTNTLog.write("Boot:Start service failed");
			}
			
			return;
		}*/

        return;
    }
}

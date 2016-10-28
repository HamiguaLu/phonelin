package mobitnt.phonelin;

import java.io.IOException;

import mobitnt.net.PLClient;
import mobitnt.net.UdpHelper;
import mobitnt.wrapper.*;
import mobitnt.util.*;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.WifiManager;
import android.os.IBinder;
import android.os.PowerManager;
import android.util.Log;

import org.json.JSONObject;

public class PhoneLinService extends Service {

    static PhoneLinService myService = null;

    public PhoneLinService() {

    }

    UdpHelper m_broadcastReceiver = new UdpHelper();

    @Override
    public boolean onUnbind(Intent intent) {
        super.onUnbind(intent);
        return false;
    }

    private static BroadcastReceiver mBroadcastReceiver = new PhoneLinReceiver();

    public static final String smsAction = "android.provider.Telephony.SMS_RECEIVED";
    public static final String callAction = "android.intent.action.PHONE_STATE";
    public static final String UI_ACTION = "PhoneLine.ACTION.UI";

    void InstallReceiver() {
        IntentFilter filter = new IntentFilter();

        filter.addAction(smsAction);
        filter.addAction(callAction);
        filter.addAction(UI_ACTION);

        filter.addAction(Intent.ACTION_UMS_CONNECTED);
        filter.addAction(Intent.ACTION_BATTERY_CHANGED);
        filter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
        filter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);

        registerReceiver(mBroadcastReceiver, filter);
    }

    Thread m_clientThread = null;
    String m_clientIP = "10.0.2.2";
    PLClient m_client = null;

    static synchronized public void  startClientThread(String clientIP) {
        if (myService == null) {
            return;
        }

        myService.m_clientIP = clientIP;
        if (myService.m_clientThread != null && myService.m_clientThread.isAlive()) {
            Log.d("PhoneLinService", "client thread already exists");
            return;
        }

        myService.m_clientThread = new Thread() {
            public void run() {
                try {
                    myService.m_client = new PLClient();
                    do {
                        int ret = myService.m_client.connect(myService.m_clientIP);
                        if (ret == EADefine.EA_RET_OK) {
                            break;
                        }

                        myService.sendMsgToUI(EADefine.SERVICE_MSG_CONNECTTION_ERROR, "");
                    } while (true);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        };
        myService.m_clientThread.start();
    }

    @Override
    public void onCreate() {
        super.onCreate();

        myService = this;
        EAUtil.SetEAContext(this.getBaseContext());

        Log.i("Service", "onCreate PhoneLin service");

        InstallReceiver();

        acquireWakeLock();

        try {

        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        m_broadcastReceiver.HearFromServer();

        sendMsgToUI(EADefine.SERVICE_MSG_SERVICE_ALIVE, "");

    }

    private static PowerManager.WakeLock sWakeLock = null;
    private static WifiManager.WifiLock sWifiLock = null;

     static public void acquireWakeLock() {
        if (sWakeLock == null) {
            PowerManager pMgr = (PowerManager) EAUtil.GetEAContext()
                    .getSystemService(Context.POWER_SERVICE);

            sWakeLock = pMgr.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK,
                    "PhoneLinService.sync() wakelock.");

            WifiManager wMgr = (WifiManager) EAUtil.GetEAContext()
                    .getSystemService(Context.WIFI_SERVICE);
            sWifiLock = wMgr.createWifiLock("PhoneLin network");
        }

        if (!sWakeLock.isHeld()) {
            sWakeLock.acquire(1000 * 45);
        }

        if (!sWifiLock.isHeld()) {
            sWifiLock.acquire();
        }
    }
    @Override
    public void onDestroy() {
        stopThread();

        sendMsgToUI(EADefine.SERVICE_MSG_SERVICE_STOPPED, "");

        super.onDestroy();
        Log.i("Service", "onDestroy");
    }

    public int onStartCommand(Intent intent, int flags, int startId) {
        return START_STICKY;
    }


    @Override
    public IBinder onBind(Intent intent) {
        // TODO Auto-generated method stub
        return null;
    }

    static public void sendMsgToUI(int msg, String data) {
        if (myService == null) {
            return;
        }

        Intent intent = new Intent(phoneLinActivity.SERVICE_ACTION);
        intent.putExtra("msg_type", msg);
        intent.putExtra("data", data);
        myService.sendBroadcast(intent);

    }

    static public void sendEvt2Host(String evt) {
        try {
            JSONObject obj = new JSONObject();
            obj.put("evt", evt);
            if (myService.m_client != null) {
                myService.m_client.postEvent(obj.toString().getBytes("utf-8"));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    void stopThread() {
        m_broadcastReceiver.Stop();

        if (m_clientThread != null && m_clientThread.isAlive()) {
            Log.d("PhoneLinService", "client thread already exists");
            m_clientThread.interrupt();
        }
    }

    static public void onStopSelf() {
        if (myService == null) {
            return;
        }

        myService.stopThread();

        myService.stopSelf();
    }
}

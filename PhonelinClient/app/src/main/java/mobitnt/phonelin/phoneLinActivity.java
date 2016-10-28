package mobitnt.phonelin;


import mobitnt.util.EADefine;
import mobitnt.util.EAUtil;

import android.app.Activity;


import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;

public class phoneLinActivity extends Activity implements
        android.view.View.OnClickListener {
    static public phoneLinActivity m_mainApp = null;
    public static final int HELP = Menu.FIRST + 2;
    public static final int RENEW_SECURITY_CODE = Menu.FIRST;
    public static final int ENABLE_SECURITY_CODE = Menu.FIRST + 1;

    public static final String SERVICE_ACTION = "PhoneLin.Service.Action";

    Intent serviceintent = new Intent("mobitnt.phonelin.PhoneLinSERVICE");

    public static String humanReadableByteCount(long bytes, boolean si) {
        int unit = si ? 1000 : 1024;
        if (bytes < unit) return bytes + " B";
        int exp = (int) (Math.log(bytes) / Math.log(unit));
        String pre = (si ? "kMGTPE" : "KMGTPE").charAt(exp-1) + (si ? "" : "i");
        return String.format("%.1f %sB", bytes / Math.pow(unit, exp), pre);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        /*menu.add(0, HELP, 0, "Help");
        if (EAUtil.IsSecurityEnabled()) {
			menu.add(0, ENABLE_SECURITY_CODE, 0, "Disable Security Code");
			menu.add(0, RENEW_SECURITY_CODE, 0, "Refresh Security Code");
		} else {
			menu.add(0, ENABLE_SECURITY_CODE, 0, "Enable Security Code");
		}*/
        return super.onCreateOptionsMenu(menu);

    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        //TextView t1 = (TextView) findViewById(R.id.txtCode);
        switch (item.getItemId()) {
            case HELP:
                Intent intent = new Intent();
                intent.setAction(Intent.ACTION_VIEW);
                intent.setData(android.net.Uri
                        .parse("http://www.mobitnt.com/sync-outlook-with-android-guide.htm"));
                startActivity(intent);
                break;

        }

        return super.onOptionsItemSelected(item);
    }

    public void onClick(View v) {
 
    }

    boolean m_bStartService = false;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.main);

        EAUtil.SetEAContext(this.getBaseContext());

        TextView tTitle = (TextView) findViewById(R.id.txtTitle);
        TextView tips = (TextView) findViewById(R.id.txtTips);

        View view = this.getWindow().getDecorView();
        //view.setBackgroundColor(0xff3FA9F5);
        view.setBackgroundColor(0xffB7D396);

        View v = findViewById(R.id.MainScreen);
        v.setOnClickListener(this);

        tTitle.setTextColor(0xff253B4B);
        //tTitle.setTextSize();
        tips.setTextColor(0xff2FBFF4);

        m_mainApp = this;

        IntentFilter filter = new IntentFilter();
        filter.addAction(SERVICE_ACTION);
        registerReceiver(m_broadcastReceiver, filter);

        /*Button btnStop = (Button) findViewById(R.id.btnStopService);
        btnStop.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                //sendMsg2Service(EADefine.UI_MSG_STOP_SERVICE, "");
                if (!m_bStartService) {
                    stopService(serviceintent);
                }else {
                    startService(serviceintent);
                }
            }
        });*/

        Button btnScan = (Button) findViewById(R.id.btnScan);
        btnScan.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                //new IntentIntegrator(m_mainApp).initiateScan();
                IntentIntegrator integrator = new IntentIntegrator(m_mainApp);
                //integrator.setCaptureActivity(AnyOrientationCaptureActivity.class);
                integrator.setDesiredBarcodeFormats(IntentIntegrator.QR_CODE_TYPES);
                integrator.setPrompt("Scan PC ip address");
                integrator.setOrientationLocked(false);
                //integrator.setBeepEnabled(false);
                integrator.initiateScan();
            }
        });

        m_txtLog = (TextView) findViewById(R.id.txtTips);

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        IntentResult result = IntentIntegrator.parseActivityResult(requestCode, resultCode, data);
        if(result != null) {
            if(result.getContents() != null) {
                //Toast.makeText(this, "Scanned: " + result.getContents(), Toast.LENGTH_LONG).show();
                String ip = result.getContents();
                boolean isIPv4;
                try {
                    InetAddress inet = InetAddress.getByName(ip);
                    isIPv4 = inet instanceof Inet4Address;
                } catch (final UnknownHostException e) {
                    isIPv4 = false;
                }
                if (isIPv4) {
                    m_mainApp.showConnectDlg(result.getContents());
                }else{
                    Toast.makeText(this, "Scanned: " + ip + " is not valid", Toast.LENGTH_LONG).show();
                }
            }
        } else {
            super.onActivityResult(requestCode, resultCode, data);
        }
    }


    TextView m_txtLog;


    public void onStart() {
        super.onStart();
        startService(serviceintent);
		//printLog("Starting service...");

        sendMsg2Service(EADefine.UI_MSG_QUERY_SERVICE, "");
    }


    private BroadcastReceiver m_broadcastReceiver = new BroadcastReceiver() {

        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            if (action.equals(SERVICE_ACTION)) {
                Message msg = new Message();
                msg.what = intent.getExtras().getInt("msg_type");
                msg.obj = intent.getExtras().getString("data");

                mMsgHandler.sendMessage(msg);
                return;
            }
        }
    };

    
	String m_logInfo = "";
    void addLog(String log){
        if (m_logInfo.length() > 1024) {
            m_logInfo = m_logInfo.substring(512);
        }

        m_logInfo += log;
        m_logInfo += "\n";

        m_txtLog.setText(m_logInfo);

        if (m_txtLog.getLayout() == null){
            return;
        }

        int scrollAmount = m_txtLog.getLayout().getLineTop(m_txtLog.getLineCount()) - m_txtLog.getHeight();
        // if there is no need to scroll, scrollAmount will be <=0
        if (scrollAmount > 0)
            m_txtLog.scrollTo(0, scrollAmount);
        else
            m_txtLog.scrollTo(0, 0);

    }

    static void printLog(String log) {
        if (m_mainApp == null) {
            return;
        }

        m_mainApp.addLog(log);
    }

    public static Handler mMsgHandler = new Handler() {
        boolean bShowConnError = true;
        @Override
        public void handleMessage(Message msg) {
            if (m_mainApp == null) {
                return;
            }

            switch (msg.what) {
                /*case EADefine.SERVICE_MSG_PC_FOUND: {
                    String clientIp = msg.obj.toString();
                    if ( m_mainApp.m_sHostIP.equals(clientIp)){
                        return;
                    }

                    printLog("PC " + clientIp + " found");

                    m_mainApp.showConnectDlg(clientIp);
                    break;
                }*/
                case EADefine.SERVICE_MSG_CONNECTTION_UP:{
                    String clientIp = msg.obj.toString();
                    m_mainApp.m_sHostIP = clientIp;
                    printLog("Connected to PC " + clientIp);
                    bShowConnError = true;

                    break;
                }
                case EADefine.SERVICE_MSG_FILE_RECV:{
                    m_mainApp.m_logInfo = "";
                    String datalen = msg.obj.toString();
                    datalen = humanReadableByteCount(Long.parseLong(datalen),true);
                    printLog("Receiving file:" + datalen + " left");
                    break;
                }
                case EADefine.SERVICE_MSG_SERVICE_ALIVE: {
                    /*Button button = (Button) m_mainApp.findViewById(R.id.btnStopService);
                    button.setText("Stop Service");*/
                    m_mainApp.m_bStartService = false;
                    printLog("Service started,waiting PC now");
                    String lastHost = EAUtil.getLastHostIP();
                    if (lastHost.length() > 4) {
                       m_mainApp.sendMsg2Service(EADefine.UI_MSG_CONNECT_TO_PC, lastHost);
                    }
                    break;
                }
                case EADefine.SERVICE_MSG_CONNECTTION_ERROR: {
                    if (bShowConnError) {
                        printLog("Connecttion is down, will try again");
                        bShowConnError = false;
                    }
                    break;
                }
                case EADefine.SERVICE_MSG_SERVICE_STOPPED: {
                    printLog("Service stopped");
                    /*Button button = (Button) m_mainApp.findViewById(R.id.btnStopService);
                    button.setText("Start Service");*/
                    m_mainApp.m_bStartService = true;
                    break;
                }
                default:
                    break;
            }
        }
    };

    String m_sHostIP = "";
	//List<String> m_HostList = new ArrayList<String>();

    private void showConnectDlg(String sIP) {
		if ( m_mainApp.m_sHostIP.equals(sIP)){
			return;
		}

        String lastHost = EAUtil.getLastHostIP();
        if (lastHost.length() > 4) {
            if (lastHost.equals(sIP)) {
                m_mainApp.sendMsg2Service(EADefine.UI_MSG_CONNECT_TO_PC, lastHost);
                return;
            }
        }

		m_mainApp.m_sHostIP = sIP;

        final AlertDialog dlg = new AlertDialog.Builder(this).create();
        dlg.show();
        Window window = dlg.getWindow();

        window.setContentView(R.layout.pc_found_dlg);

		TextView txtHostIP = (TextView) window.findViewById(R.id.txtHostIP);
		txtHostIP.setText("Connect to " + m_sHostIP + "?");

        Button ok = (Button) window.findViewById(R.id.btn_connect_2_pc);
        ok.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                EAUtil.setLastHostIP(m_sHostIP);
                sendMsg2Service(EADefine.UI_MSG_CONNECT_TO_PC, m_sHostIP);
				dlg.cancel();
            }
        });

        Button cancel = (Button) window.findViewById(R.id.btn_connect_cancel);
        cancel.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                dlg.cancel();
            }
        });
    }

    void sendMsg2Service(int msg, String data) {
        Intent intent = new Intent(PhoneLinService.UI_ACTION);
        intent.putExtra("msg_type", msg);
        intent.putExtra("data", data);
        sendBroadcast(intent);
    }

}

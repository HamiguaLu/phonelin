package mobitnt.net;


import android.util.Log;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.security.KeyStore;


import org.apache.http.conn.ssl.SSLSocketFactory;

import org.json.JSONObject;

import mobitnt.phonelin.PhoneLinService;
import mobitnt.phonelin.R;
import mobitnt.util.*;

public class PLClient {

    public class NetPackage {
        public int action;
        public long datalen;
        public byte[] data;
	};

    public PLClient() {

    }

    private SSLSocketFactory newSslSocketFactory() {
        try {
            KeyStore trusted = KeyStore.getInstance("BKS");
            InputStream in = EAUtil.GetEAContext().getResources().openRawResource(R.raw.trustkeystore);
            try {
                trusted.load(in, "!m@o#b%".toCharArray());
            } finally {
                in.close();
            }

            SSLSocketFactory mySslFact = new SSLSocketFactory(trusted);
            //mySslFact.setHostNameVerifier(new MyHstNameVerifier());
            return mySslFact;
        } catch (Exception e) {
            e.printStackTrace();
        }

        return null;
    }

    public void postEvent(byte[] event) {
        sendCommand(EADefine.PL_REQ_POST_EVENT, event);
    }

    synchronized boolean sendFile(int Action, byte[] path) throws Exception {
        if (path == null) {
            return false;
        }

        JSONObject req = new JSONObject(new String(path, "UTF-8"));
        String filepath = req.getString("path");
        //String dstPath = req.getString("dstPath");
        FileInputStream fis = FileManager.getFile(filepath);
        int retcode = EADefine.EA_RET_OK;
        int fileLen = 0;
        if (fis == null) {
            retcode = EADefine.EA_RET_OPEN_FILE_FAILED;
        } else {
            fileLen = fis.available();
        }

        /*
        Data layout:
        ACTION  ,dataLen  jsonLen jsonHdr  FileData
         */

        m_outDataStream.writeInt(Action);
        req.put("retCode", retcode);

        byte[] tmp = req.toString().getBytes("utf-8");
        int datalen = tmp.length + fileLen;
        m_outDataStream.writeLong(datalen);
        m_outDataStream.writeLong(tmp.length);
        m_outDataStream.write(tmp, 0, tmp.length);

        if (fileLen == 0) {
            fis.close();
            return false;
        }

        byte[] buff = new byte[20480];
        while (true) {
            int read = fis.read(buff, 0, 20480);
            if (read <= 0) {
                break;
            }

            m_outDataStream.write(buff, 0, read);
        }

        fis.close();

        return true;
    }


    synchronized boolean sendCommand(int Action, byte[] data) {
        try {
            m_outDataStream.writeInt(Action);
            if (data == null) {
                m_outDataStream.writeInt(0);
                return true;
            }

            m_outDataStream.writeLong(data.length);
            m_outDataStream.write(data);

            return true;
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        return false;
    }

    String m_dstFolder;
    int saveData2SdCard(DataInputStream dis, long dataLen, String folder) throws Exception {
        int iFileNameLen = dis.readInt();
        byte[] nameBuf = new byte[iFileNameLen];
        if (iFileNameLen != dis.read(nameBuf, 0, iFileNameLen)) {
            return EADefine.EA_RET_FAILED;
        }

        JSONObject req = new JSONObject(new String(nameBuf, "UTF-8"));
        String fileName = req.getString("dstPath");
        dataLen -= iFileNameLen;
        folder = folder + "/" + fileName;
        m_dstFolder = folder;

        new File(folder).deleteOnExit();

        OutputStream os = new FileOutputStream(new File(folder));
        if (os == null) {
            return EADefine.EA_RET_OPEN_FILE_FAILED;
        }

        int read = 0;
        final int bufLen = 10240;
        byte[] bytes = new byte[bufLen];

        do {
            read = bufLen;
            if (dataLen < bufLen) {
                read = (int)dataLen;
            }

            read = dis.read(bytes, 0, read);
            if (read < 0) {
                return EADefine.EA_RET_FAILED;
            }

            os.write(bytes,0,read);

            dataLen -= read;

            PhoneLinService.sendMsgToUI(EADefine.SERVICE_MSG_FILE_RECV,String.valueOf(dataLen));
        } while (dataLen > 0);

        os.flush();
        os.close();

        return EADefine.EA_RET_OK;
    }

    NetPackage recvCommand(InputStream is) {
        try {
            DataInputStream dis = new DataInputStream(is);

            NetPackage hdr = new NetPackage();

            hdr.action = dis.readInt();
            hdr.datalen = dis.readLong();

            if (hdr.action == EADefine.PL_REQ_UPLOAD_FILE
                    || hdr.action == EADefine.PL_REQ_INSTALL_APP) {

                String folder = "";
                if (hdr.action == EADefine.PL_REQ_INSTALL_APP) {
                    folder = EAUtil.GetEAContext().getExternalCacheDir().getAbsolutePath();
                }

                int ret = saveData2SdCard(dis, hdr.datalen, folder);

                JSONObject f = new JSONObject();
                f.put("fileName", m_dstFolder);
                f.put("retCode",ret);
                hdr.data = f.toString().getBytes("utf-8");
                hdr.datalen = hdr.data.length;

                return hdr;
            }

            if (hdr.datalen > 1024 * 1024 * 1024) {
                //prevent attack
                return null;
            }

            if (hdr.datalen == 0) {
                return hdr;
            }

            hdr.data = new byte[(int)hdr.datalen];
            if (hdr.datalen != dis.read(hdr.data, 0, (int)hdr.datalen)) {
                return null;
            }

            return hdr;

        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        return null;
    }

    byte[] parseCommand(NetPackage netPkg) {
        if (netPkg == null) {
            return null;
        }

        try {
            JSONObject req = null;

            if (netPkg.data != null && netPkg.datalen > 0) {
                req = new JSONObject(new String(netPkg.data, "UTF-8"));
            }

            ReqHandler reqHandler = null;
            switch (netPkg.action) {
                case EADefine.PL_REQ_GET_SYS_INFO: {
                    reqHandler = new SysManager();
                    break;
                }
                case EADefine.PL_REQ_SEND_SMS:
                case EADefine.PL_REQ_GET_PIM_PHOTO:
                case EADefine.PL_REQ_GET_PIM_DATA: {
                    reqHandler = new PimManager();
                    break;
                }
                case EADefine.PL_REQ_GET_APP_ICON:
                case EADefine.PL_REQ_DEL_APP:
                case EADefine.PL_REQ_INSTALL_APP:
                case EADefine.PL_REQ_GET_APP_LIST: {
                    reqHandler = new AppManager();
                    break;
                }
                case EADefine.PL_REQ_GET_SD_CARD_LIST:
                case EADefine.PL_REQ_NEW_FILE:
                case EADefine.PL_REQ_GET_FILE_LIST:
                case EADefine.PL_REQ_RENAME_FILE:
                case EADefine.PL_REQ_UPLOAD_FILE:
                case EADefine.PL_REQ_DEL_FILE:
                case EADefine.PL_REQ_COPY_FILE:
                case EADefine.PL_REQ_CUT_FILE:{
                    reqHandler = new FileManager();
                    break;
                }
                case EADefine.PL_REQ_GET_MEDIA_LIST: {
                    reqHandler = new MediaManager();
                    break;
                }
            }

            if (reqHandler == null) {
                return null;
            }

            return reqHandler.processRequest(netPkg.action, req);

        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        return null;
    }

    DataOutputStream m_outDataStream = null;

    public int connect(String ip) {
        SSLSocketFactory ssf = newSslSocketFactory();

        try {
            Socket sslsock = ssf.createSocket();
            InetSocketAddress remoteAddress = new InetSocketAddress(ip, EADefine.PL_SERVER_PORT);
            Log.i(EADefine.TAG,"Start connect");
            sslsock.connect(remoteAddress, 3000);
            PhoneLinService.sendMsgToUI(EADefine.SERVICE_MSG_CONNECTTION_UP,ip);
            //sslsock.setSoTimeout(soTimeout);
            Log.i(EADefine.TAG,"connected");
            m_outDataStream = new DataOutputStream(sslsock.getOutputStream());
            InputStream is = sslsock.getInputStream();
            String phoneID = EAUtil.GetPhoneID();
            if (!sendCommand(EADefine.PL_REQ_REPORT_ID, phoneID.getBytes())) {
                return EADefine.EA_RET_CONNECT_EXCEPTION;
            }

            NetPackage netPkg = null;
            while ((netPkg = recvCommand(is)) != null) {
                if (netPkg.action == EADefine.PL_REQ_DOWNLOAD_FILE) {
                    sendFile(netPkg.action, netPkg.data);
                }  /*else if (netPkg.action == EADefine.PL_REQ_UPLOAD_FILE
                        || netPkg.action == EADefine.PL_REQ_INSTALL_APP) {

                }*/ else {
                    byte[] retData = parseCommand(netPkg);
                    sendCommand(netPkg.action, retData);
                }
            }

            return EADefine.EA_RET_CHAT_EXCEPTION;

        } catch (UnknownHostException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return EADefine.EA_RET_UNKONW_HOST;
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return EADefine.EA_RET_CONNECT_EXCEPTION;
        }

    }

}



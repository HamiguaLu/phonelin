package mobitnt.net;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import android.content.Context;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.os.StrictMode;
import android.util.Log;


import mobitnt.phonelin.PhoneLinService;
import mobitnt.util.EADefine;
import mobitnt.util.EAUtil;

public class UdpHelper {

	boolean m_workingFlag = true;

	Thread notifyThread = null;

	public void NotifyOtherDevice() {
		notifyThread = new Thread(new Runnable() {
			public void run() {
				while (m_workingFlag) {
					try {
						sendUdpBroadcast("CLIENT");
						Thread.sleep(1000 * 5);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
		});
		notifyThread.start();
	}

	public void Stop() {
		m_workingFlag = false;

		try {
			if (recv_socket != null) {
				recv_socket.close();
			}

			if (hearThread != null) {
				hearThread.join(1000);
			}

			if (notifyThread != null) {
				notifyThread.join(1000);
			}
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	Thread hearThread = null;

	public void HearFromServer() {
		hearThread = new Thread(new Runnable() {
			public void run() {
				try {
					recv_socket = new DatagramSocket(EADefine.PL_BROADCAST_PORT);
					recv_socket.setBroadcast(true);
					recv_socket.setSoTimeout(1000 * 10);
				} catch (SocketException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (Exception e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				while (m_workingFlag) {
					try {
						ReceiveUdpBroadcasting();
					} catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
		});
		hearThread.start();
	}

	DatagramSocket recv_socket = null;

	void ReceiveUdpBroadcasting() {
		try {
			// Keep a socket open to listen to all the UDP trafic that is
			// destined for this port
			
			//Log.i(EADefine.EA_LOG_TAG, "Ready to receive broadcast packets!");

			// Receive a packet
			byte[] recvBuf = new byte[150];
			DatagramPacket packet = new DatagramPacket(recvBuf, recvBuf.length);

			recv_socket.receive(packet);

			String data = new String(packet.getData()).trim();
			OnUdpDataReceived(data, packet.getAddress());

			// Packet received
			Log.i(EADefine.TAG, "Packet received from: "
					+ packet.getAddress().getHostAddress());
			Log.i(EADefine.TAG, "Packet received; data: " + data);

		} catch (Exception ex) {
			Log.i(EADefine.TAG, "ReceiveUdpBroadcasting Exception:" + ex);
		}
	}

	void sendUdpBroadcast(String messageStr) {
		// Hack Prevent crash (sending should be done using an async task)
		StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder()
				.permitAll().build();
		StrictMode.setThreadPolicy(policy);

		try {
			// Open a random port to send the package
			DatagramSocket socket = new DatagramSocket();
			socket.setBroadcast(true);
			byte[] sendData = messageStr.getBytes();
			DatagramPacket sendPacket = new DatagramPacket(sendData,
					sendData.length, getBroadcastAddress(),
					EADefine.PL_BROADCAST_PORT);
			socket.send(sendPacket);
			Log.i(EADefine.TAG, "send broadcast now");
		} catch (IOException e) {
			Log.e(EADefine.TAG, "IOException: " + e.getMessage());
		}
	}

	InetAddress getBroadcastAddress() throws IOException {
		WifiManager wifi = (WifiManager) EAUtil.GetEAContext()
				.getSystemService(Context.WIFI_SERVICE);
		DhcpInfo dhcp = wifi.getDhcpInfo();
		// handle null somehow

		int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
		byte[] quads = new byte[4];
		for (int k = 0; k < 4; k++)
			quads[k] = (byte) ((broadcast >> k * 8) & 0xFF);
		return InetAddress.getByAddress(quads);
	}




	void OnUdpDataReceived(String sData, InetAddress fromAddr) {
		if (!sData.startsWith("PhoneLinServer")) {
			return;
		}

		String sMAC = sData.replace("PhoneLinServer","");

		String sIP = fromAddr.getHostAddress();

		//PhoneLinService.sendMsgToUI(EADefine.SERVICE_MSG_PC_FOUND,sIP);
		PhoneLinService.startClientThread(sIP);

	}

}

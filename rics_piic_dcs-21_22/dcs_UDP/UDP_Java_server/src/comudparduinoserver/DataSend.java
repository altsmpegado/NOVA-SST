package comudparduinoserver;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

/**
 *
 * @author alts
 */

public class DataSend {
    ServerSocket server_socket;
    DatagramSocket data_socket;
    InetAddress IPAddress;
    byte[] sendData = new byte[1024];  
    DatagramPacket sendPacket;

    public DataSend(ServerSocket _ss) throws Exception{
        this.server_socket = _ss;
        data_socket = server_socket.returnServerSocket();
    }

    public void packetSend(String IP, String message) throws Exception{
        IPAddress = InetAddress.getByName(IP);
        message = message.toUpperCase();
        System.out.println("SENT: " + message);
        sendData = message.getBytes();
        sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 8888);
        data_socket.send(sendPacket);
    }    
}

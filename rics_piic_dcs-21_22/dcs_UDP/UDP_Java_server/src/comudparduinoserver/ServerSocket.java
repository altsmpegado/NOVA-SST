package comudparduinoserver;

import java.net.DatagramSocket;

/**
 *
 * @author alts
 */
public class ServerSocket {

    DatagramSocket serverSocket;
    
    public ServerSocket(int port) throws Exception{
        serverSocket = new DatagramSocket(port);
    }
    
    DatagramSocket returnServerSocket(){
        return serverSocket;
    }
}

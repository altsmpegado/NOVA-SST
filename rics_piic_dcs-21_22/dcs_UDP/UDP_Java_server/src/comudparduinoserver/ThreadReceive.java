package comudparduinoserver;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author alts
 */

public class ThreadReceive extends Thread{

    byte[] receiveData = new byte[1024];  
    DatagramPacket receivePacket;
    ServerSocket server_socket;
    DatagramSocket data_socket;
    
    public static ArrayBlockingQueue rcv_queue = new ArrayBlockingQueue(10);
    
    public ThreadReceive(ServerSocket _ss) throws Exception{
        this.server_socket = _ss;
        data_socket = server_socket.returnServerSocket();
        receivePacket = new DatagramPacket(receiveData, receiveData.length);
    }
    
    public String packetReceive(){
        try{
            data_socket.receive(receivePacket);
        }
        catch (IOException ex){
            Logger.getLogger(ThreadReceive.class.getName()).log(Level.SEVERE, null, ex);
        }
        String msg = new String(receivePacket.getData());
        return msg;
    }
    
    @Override
    public void run(){
        while(true){
            rcv_queue.add(packetReceive());
            System.out.println("RCV_QUEUE SIZE: " + rcv_queue.size());
        }      
    }
}

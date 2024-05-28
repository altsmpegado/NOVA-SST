package tcp;

import java.util.concurrent.ArrayBlockingQueue;

/**
 *
 * @author alts
 */

public class MainSystem {

    public static void main(String[] args) throws Exception {
        /* clients */
        Client clientA = new Client("192.168.200.4", 8888);
        Client clientB = new Client("192.168.200.5", 8888);
        Client clientPi = new Client("192.168.200.242", 8888);
        
        /* starting comms */
        clientA.startConnection();
        clientB.startConnection();
        clientPi.startConnection();

        /* plcs */
        Arduino aa = new Arduino("A", clientA);
        Arduino ab = new Arduino("B", clientB);
        Raspberry pi = new Raspberry(clientPi);
                
        ArrayBlockingQueue rcv_queue = new ArrayBlockingQueue(10);
        SystemGUI gui = new SystemGUI(aa, ab, pi);
        
        /* threads */
        ThreadReceive receiveA = new ThreadReceive(clientA, rcv_queue);
        ThreadReceive receiveB = new ThreadReceive(clientB, rcv_queue);
        ThreadReceive receivePi = new ThreadReceive(clientPi, rcv_queue);
        ThreadExecutionManager manager = new ThreadExecutionManager(aa, ab, pi, gui, rcv_queue);
        
        /* start threads */
        receiveA.start(); 
        receiveB.start();
        receivePi.start();
        manager.start(); 
        gui.setVisible(true);
    }
}
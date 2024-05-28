package tcp;

import java.io.IOException;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author alts
 */

public class ThreadReceive extends Thread{
    
    Client c;
    String msg;
    
    ArrayBlockingQueue rcv_queue;
    
    public ThreadReceive(Client _c, ArrayBlockingQueue queue) throws Exception{
        this.c = _c;
        this.rcv_queue = queue;
    }
    
    public String packetReceive() throws InterruptedException{
        try{
            msg = c.receiveMessage();
            this.c.stopConnection();
            ThreadReceive.sleep(10);
            this.c.startConnection();
            ThreadReceive.sleep(10);
        }
        catch (IOException ex){
            Logger.getLogger(ThreadReceive.class.getName()).log(Level.SEVERE, null, ex);
        }
        return msg;
    }
    
    @Override
    public void run(){
        while(true)
            try {
                rcv_queue.add(packetReceive());
            } catch (InterruptedException ex) {
                Logger.getLogger(ThreadReceive.class.getName()).log(Level.SEVERE, null, ex);
            }   
    }
}

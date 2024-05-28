package tcp;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author alts
 */

public class ThreadExecutionManager extends Thread {
 
    String msg;
    Arduino aa;
    Arduino ab;
    Raspberry pi;
    SystemGUI gui;
    ArrayBlockingQueue rcv_queue;
    
    public ThreadExecutionManager(Arduino _aa, Arduino _ab, Raspberry _pi, SystemGUI _gui, ArrayBlockingQueue queue){
        this.aa = _aa;
        this.ab = _ab;
        this.pi = _pi;
        this.gui = _gui;
        this.rcv_queue = queue;
    }
    
    public void messageContentManager(String msg) throws Exception{
        
        System.out.println("RECEIVED: " + msg);
        
        switch(msg){
            case "A_END_EXE":
                if(gui.complex){
                    System.out.println("Transition AC");
                    aa.transition_send();   
                    pi.transitionReceiveCL();
                } else{
                    System.out.println("End of processing");
                    gui.executionA.setEnabled(true);
                    gui.executionB.setEnabled(true);
                    gui.complexA.setEnabled(true);
                    gui.complexB.setEnabled(true);
                    gui.gen_complex.setEnabled(true);
                }
                break;
            case "CR_END_EXE":
                System.out.println("Transition CD");
                pi.transitionSendCR();
                pi.transitionReceiveDL();
                break;
            case "DR_END_EXE":
                System.out.println("Transition DB");
                pi.transitionSendDR();
                ab.transition_rcv();
                break;
            case "B_END_EXE":
                if(gui.complex){   
                    System.out.println("Transition BD");
                    ab.transition_send();
                    pi.transitionReceiveDR();
                } else {
                    System.out.println("End of processing");
                    gui.executionA.setEnabled(true);
                    gui.executionB.setEnabled(true);
                    gui.complexA.setEnabled(true);
                    gui.complexB.setEnabled(true);
                    gui.gen_complex.setEnabled(true);
                }
                break;
            case "DL_END_EXE":
                System.out.println("Transition DC");
                pi.transitionSendDL();
                pi.transitionReceiveCR();
                break;
            case "CL_END_EXE":
                System.out.println("Transition CA");
                pi.transitionSendCL();
                aa.transition_rcv();
                break;
            case "A_RCV":
                System.out.println("A received art");
                pi.stopC();
                aa.execution();
                gui.complex=false;
                break;
            case "B_RCV":
                System.out.println("B received part");
                pi.stopD();
                ab.execution();
                gui.complex=false;  
                break;
            case "CL_RCV":
                System.out.println("C Left received part");
                aa.stop();
                pi.moveCRight();
                break;
            case "DL_RCV":
                System.out.println("D Left received part");
                pi.stopC();
                pi.moveDRight();
                break;
            case "CR_RCV":
                System.out.println("C Right received part");
                pi.stopD();
                pi.moveCLeft();
                break;
            case "DR_RCV":
                System.out.println("D Right received part");
                ab.stop();
                pi.moveDLeft();
                break;
            case "A_ACK_ON":
                aa.execution();
                break;
            case "B_ACK_ON":
                ab.execution();
                break;
            default:
                break;     
       }   
    }

    @Override
    public void run(){
        while(true){
            try {
                messageContentManager((String)rcv_queue.take());
            } catch (InterruptedException ex) {
                Logger.getLogger(ThreadExecutionManager.class.getName()).log(Level.SEVERE, null, ex);
            } catch (Exception ex) {
                Logger.getLogger(ThreadExecutionManager.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}
package comudparduinoserver;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author alts
 */

public class ThreadExecutionManager extends Thread {
 
    String msg;
    ArduinoKitA ka;
    ArduinoKitB kb;
    RaspberryKitPi kpi;
    SystemGUI gui;
    
    public ThreadExecutionManager(ArduinoKitA _ka, ArduinoKitB _kb, RaspberryKitPi _kpi, SystemGUI _gui){
        this.ka = _ka;
        this.kb = _kb;
        this.kpi = _kpi;
        this.gui = _gui;
    }
    
    public void messageContentManager(String msg){
        
        System.out.println("RECEIVED: " + msg);
            
        if(msg.contains("A_END_EXE") && gui.complex){
            System.out.println("Transition AC");
            ka.transition_send();   
            kpi.transition_CLrcv();
        }
        else if(msg.contains("CR_END_EXE")){
            System.out.println("Transition CD");
            kpi.transition_CRsend();
            kpi.transition_DLrcv();
        }
        else if(msg.contains("DR_END_EXE")){
            System.out.println("Transition DB");
            kpi.transition_DRsend();
            kb.transition_rcv();
        }
        else if(msg.contains("B_END_EXE") && gui.complex){
            System.out.println("Transition BD");
            kb.transition_send();
            kpi.transition_DRrcv();
        }
        else if(msg.contains("DL_END_EXE")){
            System.out.println("Transition DC");
            kpi.transition_DLsend();
            kpi.transition_CRrcv();
        }
        else if(msg.contains("CL_END_EXE")){
            System.out.println("Transition CA");
            kpi.transition_CLsend();
            ka.transition_rcv();
        }
        else if((msg.contains("A_END_EXE") || msg.contains("B_END_EXE")) && !gui.complex){
            System.out.println("End of processing");
            gui.executionA.setEnabled(true);
            gui.executionB.setEnabled(true);
            gui.complexA.setEnabled(true);
            gui.complexB.setEnabled(true);
            gui.gen_complex.setEnabled(true);
        }
        else if(msg.contains("A_RCV")){
            System.out.println("A received art");
            kpi.stopC();
            ka.execution();
            gui.complex=false;
        }
        else if(msg.contains("B_RCV")){
            System.out.println("B received part");
            kpi.stopD();
            kb.execution();
            gui.complex=false;
        }
        else if(msg.contains("CL_RCV")){
            System.out.println("C Left received part");
            ka.stop();
            kpi.movCright();
        }
        else if(msg.contains("DL_RCV")){
            System.out.println("D Left received part");
            kpi.stopC();
            kpi.movDright();
        }
        else if(msg.contains("CR_RCV")){
            System.out.println("C Right received part");
            kpi.stopD();
            kpi.movCleft();
        }
        else if(msg.contains("DR_RCV")){
            System.out.println("D Right received part");
            kb.stop();
            kpi.movDleft();
        }
        
        else if(msg.contains("A_ACK_ON")){
            ka.execution();
        }
        else if(msg.contains("B_ACK_ON")){
            kb.execution();
        }
    }

    @Override
    public void run(){
        while(true){
            try {
                messageContentManager((String)ThreadReceive.rcv_queue.take());
            } catch (InterruptedException ex) {
                Logger.getLogger(ThreadExecutionManager.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}
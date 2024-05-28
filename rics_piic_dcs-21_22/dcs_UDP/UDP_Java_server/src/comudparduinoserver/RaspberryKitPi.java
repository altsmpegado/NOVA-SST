package comudparduinoserver;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author alts
 */

public class RaspberryKitPi {
    String ipPi = "192.168.200.242";
    String leftC = "1";
    String rightC = "2";
    String leftD = "3";
    String rightD = "4";
    String tsendCL = "L5";
    String tsendCR = "R5";
    String trcvCL = "L6";
    String trcvCR = "R6";
    String tsendDL = "L7";
    String tsendDR = "R7";
    String trcvDL = "L8";
    String trcvDR = "R8";
    String stopC = "9";
    String stopD = "10";
    DataSend ds;

    public RaspberryKitPi(DataSend _ds) throws Exception{
        this.ds = _ds;
    }
    
    public void movCleft(){
        try {
            System.out.println("EVENT: C_execution_left");
            ds.packetSend(ipPi, leftC);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void movCright(){
        try {
            System.out.println("EVENT: C_execution_right");
            ds.packetSend(ipPi, rightC);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void movDleft(){
        try {
            System.out.println("EVENT: D_execution_left");
            ds.packetSend(ipPi, leftD);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void movDright(){
        try {
            System.out.println("EVENT: D_execution_right");
            ds.packetSend(ipPi, rightD);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void transition_CLsend(){
        try {
            System.out.println("EVENT: CL_transition_send");
            ds.packetSend(ipPi, tsendCL);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void transition_CRsend(){
        try {
            System.out.println("EVENT: CR_transition_send");
            ds.packetSend(ipPi, tsendCR);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void transition_CLrcv(){
        try {
            System.out.println("EVENT: CL_transition_receive");
            ds.packetSend(ipPi, trcvCL);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void transition_CRrcv(){
        try {
            System.out.println("EVENT: CR_transition_receive");
            ds.packetSend(ipPi, trcvCR);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void transition_DLsend(){
        try {
            System.out.println("EVENT: DL_transition_send");
            ds.packetSend(ipPi, tsendDL);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void transition_DRsend(){
        try {
            System.out.println("EVENT: DR_transition_send");
            ds.packetSend(ipPi, tsendDR);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void transition_DLrcv(){
        try {
            System.out.println("EVENT: DL_transition_receive");
            ds.packetSend(ipPi, trcvDL);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void transition_DRrcv(){
        try {
            System.out.println("EVENT: DR_transition_receive");
            ds.packetSend(ipPi, trcvDR);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void stopC(){
        try {
            System.out.println("EVENT: C_stop");
            ds.packetSend(ipPi, stopC);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void stopD(){
        try {
            System.out.println("EVENT: D_stop");
            ds.packetSend(ipPi, stopD);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
}

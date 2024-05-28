package comudparduinoserver;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author alts
 */

public class ArduinoKitB {
    String ipB = "192.168.200.217";
    String runB = "1";
    String stopB = "2";
    String emgstopB = "3";
    String resumeB = "4";
    String tsendB = "5";
    String trcvB = "6";
    String sensorB = "7";
    DataSend ds;
    
    public ArduinoKitB(DataSend _ds) throws Exception{
        this.ds = _ds;
    }
    
    public void execution(){
        try {
            System.out.println("EVENT: B_execution");
            ds.packetSend(ipB, runB);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void emergency_stop(){
        try {
            System.out.println("EVENT: B_stop");
            ds.packetSend(ipB, emgstopB);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void resume(){
        try {
            System.out.println("EVENT: B_resume");
            ds.packetSend(ipB, resumeB);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void transition_send(){
        try {
            System.out.println("EVENT: B_send");
            ds.packetSend(ipB, tsendB);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
   
    public void transition_rcv(){
        try {
            System.out.println("EVENT: B_receive");
            ds.packetSend(ipB, trcvB);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void stop(){
        try {
            System.out.println("EVENT: B_stop");
            ds.packetSend(ipB, stopB);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void sensor(){
        try {
            System.out.println("EVENT: B_get_sensor_state");
            ds.packetSend(ipB, sensorB);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}

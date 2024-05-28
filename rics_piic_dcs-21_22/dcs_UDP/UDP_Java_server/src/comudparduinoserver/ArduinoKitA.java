package comudparduinoserver;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author alts
 */

public class ArduinoKitA {
    String ipA = "192.168.200.234";
    String runA = "1";
    String stopA = "2";
    String emgstopA = "3";
    String resumeA = "4";
    String tsendA = "5";
    String trcvA = "6";
    String sensorA = "7";
    DataSend ds;
    
    public ArduinoKitA(DataSend _ds) throws Exception{
        this.ds = _ds;
    }
    
    public void execution(){
        try {
            System.out.println("EVENT: A_execution");
            ds.packetSend(ipA, runA);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void emergency_stop(){
        try {
            System.out.println("EVENT: A_stop");
            ds.packetSend(ipA, emgstopA);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void resume(){
        try {
            System.out.println("EVENT: A_resume");
            ds.packetSend(ipA, resumeA);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void transition_send(){
        try {
            System.out.println("EVENT: A_send");
            ds.packetSend(ipA, tsendA);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
   
    public void transition_rcv(){
        try {
            System.out.println("EVENT: A_receive");
            ds.packetSend(ipA, trcvA);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void stop(){
        try {
            //dataSend ds = new dataSend(ss);
            System.out.println("EVENT: A_stop");
            ds.packetSend(ipA, stopA);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void sensor(){
        try {
            System.out.println("EVENT: A_get_sensor_state");
            ds.packetSend(ipA, sensorA);
        } catch (Exception ex) {
            Logger.getLogger(SystemGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}

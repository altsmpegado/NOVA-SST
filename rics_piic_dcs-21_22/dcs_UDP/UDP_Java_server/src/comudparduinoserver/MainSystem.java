package comudparduinoserver;

/**
 *
 * @author alts
 */

public class MainSystem {

    public static void main(String[] args) throws Exception {
        ServerSocket ss = new ServerSocket(8888);
        DataSend ds = new DataSend(ss);
        
        ArduinoKitA ka = new ArduinoKitA(ds);
        ArduinoKitB kb = new ArduinoKitB(ds);
        RaspberryKitPi kpi = new RaspberryKitPi(ds);
        
        SystemGUI gui = new SystemGUI(ka, kb, kpi);
        
        gui.setVisible(true);
        
        ThreadReceive treceive = new ThreadReceive(ss);
        ThreadExecutionManager texe_manager = new ThreadExecutionManager(ka, kb, kpi, gui);
        
        treceive.start();  
        texe_manager.start();  
    }
}
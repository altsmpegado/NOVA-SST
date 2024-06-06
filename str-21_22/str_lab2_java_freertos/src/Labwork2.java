
public class Labwork2 {
    
    public static void main (String[] args){
        
        SplitterConveyor sc = new SplitterConveyor();
        sc.initializeHardwarePorts();
        
        Cylinder cs = new CylinderStart(sc);
        Cylinder c1 = new Cylinder1(sc);
        Cylinder c2 = new Cylinder2(sc);  
        Mechanism m = new Mechanism(sc, c1, c2, cs);
        SystemGUI sg = new SystemGUI(m);
        
        ThreadLeds threadLeds = new ThreadLeds(m);
        ThreadDockSwitches threadDockSwitches = new ThreadDockSwitches(m, sc, sg);
        ThreadStopResume threadStopResume = new ThreadStopResume(cs, c1, c2, m, sg);
        ThreadGetPart threadGetPart = new ThreadGetPart(m, sg, cs, c1, c2);
        ThreadDistribution2 threadDistribution2 = new ThreadDistribution2(sg, m, c2);
        
        //Calibration
        //m.cylinderCalibration();
        cs.calibration();
        c1.calibration();
        c2.calibration();
        
        //Threads Start - Inicialization
        threadLeds.start();
        threadDockSwitches.start();
        threadGetPart.start();
        threadDistribution2.start();
        threadStopResume.start();
        
        sg.setVisible(true);      
        sg.writeInGUI("Calibration was done successfully!");
        sg.writeInGUI("LEDs thread just started!");
        sg.writeInGUI("Dock blocking switches thread just started!");
        sg.writeInGUI("Identifying thread just started!");
        sg.writeInGUI("Sending package thread just started!");
        sg.writeInGUI("Distribution thread just started!");
        sg.writeInGUI("Emergency stop thread just started!");
        sg.writeInGUI("\nPlease configure the docks before proceeding...");
    }   
}

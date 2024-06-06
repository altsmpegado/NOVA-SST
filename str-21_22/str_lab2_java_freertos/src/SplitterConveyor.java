
import java.util.concurrent.Semaphore;

public class SplitterConveyor {
    static{
        System.load("C:\\str\\SplitterConveyor\\x64\\Debug\\SplitterConveyor.dll");
    }
    
    static synchronized native public void initializeHardwarePorts();
    static synchronized native public void cylinder1MoveForward();
    static synchronized native public void cylinder1MoveBackward();
    static synchronized native public void cylinder1Stop();
    static synchronized native public void cylinder2MoveForward();
    static synchronized native public void cylinder2MoveBackward();
    static synchronized native public void cylinder2Stop();
    static synchronized native public void cylinderStartMoveForward();
    static synchronized native public void cylinderStartMoveBackward();
    static synchronized native public void cylinderStartStop();
    static synchronized native public void conveyorMove();
    static synchronized native public void conveyorStop();
   
    //these methods just read sensors
    static synchronized native public int getCylinder1Position();
    static synchronized native public int getCylinder2Position();
    static synchronized native public int getCylinderStartPosition();
    static synchronized native public int getIdentificationSensors();
    
    static synchronized native public boolean isCylinder1MovingForward();
    static synchronized native public boolean isCylinder1MovingBackward();
    static synchronized native public boolean isCylinder2MovingForward();
    static synchronized native public boolean isCylinder2MovingBackward();
    static synchronized native public boolean isCylinderStartMovingForward();
    static synchronized native public boolean isCylinderStartMovingBackward();
    static synchronized native public boolean isPartAtDock1();
    static synchronized native public boolean isPartAtDock2();
    static synchronized native public boolean isDock1Active();
    static synchronized native public boolean isDock2Active();
    static synchronized native public boolean isDockEndActive();
    static synchronized native public boolean isLEDActive(); 
    
    static synchronized native public int readPort(int port);
    static synchronized native public void writePort(int port, int value);
    static synchronized native public void setBitValue(int var, int n_bit, int new_value_bit);
    static synchronized native public void flashLED(int value);    
    
    private static final Semaphore sem_critical = new Semaphore(1);

    static public void   enterCriticalArea() {
        try {sem_critical.acquire();} 
        catch (InterruptedException ex){}                
    } 

    static public void   exitCriticalArea()  {
        sem_critical.release();  
    } 
     
}
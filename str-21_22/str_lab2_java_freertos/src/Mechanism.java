
import java.util.concurrent.ArrayBlockingQueue;


public class Mechanism {
    
    SplitterConveyor sc;
    private final Cylinder c1, c2, cs;
    public boolean start;
    public boolean dock1_full;
    public boolean dock2_full;
    public boolean dockend_full;
    public boolean blocked_state;
    public boolean stop_state;
    public boolean interrupted;
    
    public boolean moving_cyl_state;
    
    public static final ArrayBlockingQueue d1queue = new ArrayBlockingQueue(5);
    public static final ArrayBlockingQueue d2queue = new ArrayBlockingQueue(5);
    
    public Mechanism(SplitterConveyor _sc, Cylinder _c1, Cylinder _c2, Cylinder _cs) {
        this.sc = _sc;
        this.c1 = _c1;
        this.c2 = _c2;
        this.cs = _cs;
        this.start = false;
        this.dock1_full = false;
        this.dock2_full = false;
        this.dockend_full = false;
        this.blocked_state = false;
        this.stop_state = false;
        this.interrupted = false;
        this.moving_cyl_state = false;
    }
    
    public static Integer consume_d1() {
        try {
            return (Integer) d1queue.take();
        } catch (InterruptedException ex) {}
        return null;
    }
    
    public static Integer consume_d2() {
        try {
            return (Integer) d2queue.take();
        } catch (InterruptedException ex) {}
        return null;
    }
    
    public void moveConveyor(){
        SplitterConveyor.conveyorMove();
    }
    
    public void stopConveyor(){
        SplitterConveyor.conveyorStop();
    }
    
    public void switchLed(int time){
        sc.flashLED(1);
        Sleep(time);
        sc.flashLED(0);
    }
    
    public void Sleep(int time) {
        try {Thread.sleep(time);} 
        catch (InterruptedException ex) {}
    }
    
    void stop() {
        interrupted = true;
    }
    
    synchronized void resume(){
        interrupted = false;
    }
    
    public void cylinderCalibration(){
        cs.gotoPosition(0);
        c1.gotoPosition(0);
        c2.gotoPosition(0);
    }
    
    public int identifyPart() {
        boolean sens1 = false;
        boolean sens2 = false;
        int id = 1;
        while (!sc.isPartAtDock1()) { //until reaches the cylinder 1
               if(sc.getIdentificationSensors() == 1)
                   sens1 = true;
               if(sc.getIdentificationSensors() == 2)
                   sens2 = true;
        }
        if(sens1) id++;
        if(sens2) id++;    
        return id;
    } 
}

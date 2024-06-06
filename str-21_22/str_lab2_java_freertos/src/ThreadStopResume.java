
import java.util.concurrent.Semaphore;

public class ThreadStopResume extends Thread {
    private Mechanism m;
    private SystemGUI sg;
    Cylinder c1;
    Cylinder c2;
    Cylinder cs;
    
    private static final Semaphore sem_stop = new Semaphore(0);
    private static final Semaphore sem_resume = new Semaphore(0);

    public ThreadStopResume(Cylinder _cs, Cylinder _c1, Cylinder _c2, Mechanism _m, SystemGUI _sg) {
        this.cs = _cs;
        this.c1 = _c1;
        this.c2 = _c2;
        this.m = _m;
        this.sg = _sg;
    }
    
    public static void Stop() {
        sem_stop.release();
    }
    
    public static void Resume() {
        sem_resume.release();
    }
    
    public void run() {
        int goto_cs;
        int goto_c1;
        int goto_c2;
        while(true){
            goto_cs = -1;
            goto_c1 = -1;
            goto_c2 = -1;
            
            try {sem_stop.acquire();} 
            catch (InterruptedException ex) {}
            
            if(cs.isMovingForward() && cs.getPosition()!=0)
                goto_cs = 1;
            else if(cs.isMovingBackward()&& cs.getPosition()!=0)
                goto_cs = 0;

            if(c1.isMovingForward()&& c1.getPosition()!=0)
                goto_c1 = 1;
            else if(c1.isMovingBackward()&& c1.getPosition()!=0)
                goto_c1 = 0;

            if(c2.isMovingForward()&& c2.getPosition()!=0)
                goto_c2 = 1;
            else if(c2.isMovingBackward()&& c2.getPosition()!=0)
                goto_c2 = 0;
            
            m.stopConveyor();cs.stop();c1.stop();c2.stop();
            
            ThreadDockSwitches.ledqueue.add("emergency");

            try {sem_resume.acquire();} 
            catch (InterruptedException ex) {}   
            
            if(m.moving_cyl_state==false)
                m.moveConveyor();
            
            
            ThreadDockSwitches.ledqueue.add("null"); // sai do while do emergency led porque !mb,isEmpty, vai ao switch, vai default e depois break
            
            if(goto_cs == 1)
                cs.moveForward();
                
            if(goto_cs == 0)
                cs.moveBackward();

            if(goto_c1 == 1)
                c1.moveForward();

            if(goto_c1 == 0)
                c1.moveBackward();

            if(goto_c2 == 1)
                c2.moveForward();

            if(goto_c2 == 0)
                c2.moveBackward();
                
            while((cs.isMoving() || c1.isMoving() || c2.isMoving()) && !m.stop_state){}
            if(m.stop_state) continue; //is new stop is received before ending current resume
            cs.stop();c1.stop();c2.stop();
        }
    }
}

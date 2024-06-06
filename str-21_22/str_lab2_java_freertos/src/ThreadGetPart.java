
import java.util.concurrent.Semaphore;
import java.util.logging.Level;
import java.util.logging.Logger;

public class ThreadGetPart extends Thread{
    Cylinder cs;
    Cylinder c1;
    Cylinder c2;
    private SystemGUI sg;
    private Mechanism m;
    
    public ThreadGetPart(Mechanism mechanism, SystemGUI _sg, Cylinder _cs, Cylinder _c1, Cylinder _c2) {
        this.m = mechanism;
        this.sg = _sg;
        this.cs = _cs;
        this.c1 = _c1;
        this.c2 = _c2;
    }
    
    private static final Semaphore semaphore = new Semaphore(0);
    
    public static void startGetPart() {
        semaphore.release();
    }
    
    @Override
    public void run(){
        while(true){
            
            try{ semaphore.acquire(); } catch(InterruptedException ex) {continue; }
            if(!m.blocked_state && !m.interrupted){
                ThreadIdentify threadIdentify = new ThreadIdentify(m, sg, c1, c2);
                threadIdentify.start();
                CylinderThread tcylinder = new CylinderThread(cs);
                tcylinder.start();
                try {
                    tcylinder.join();
                } catch (InterruptedException ex) {
                    Logger.getLogger(ThreadGetPart.class.getName()).log(Level.SEVERE, null, ex);
                }
                try {
                    threadIdentify.join();
                } catch (InterruptedException ex) {
                    Logger.getLogger(ThreadGetPart.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }
}

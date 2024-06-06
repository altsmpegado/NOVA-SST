import java.util.concurrent.Semaphore;
import java.util.logging.Level;
import java.util.logging.Logger;

//this is a permanent thread
public class ThreadIdentify extends Thread{
    
    private Mechanism m;
    private SystemGUI sg;
    Cylinder c1;
    Cylinder c2;
    
    private static final Semaphore semaphore = new Semaphore(0);
    
    public ThreadIdentify(Mechanism mechanism, SystemGUI _sg,  Cylinder _c1, Cylinder _c2) {
        this.m = mechanism;
        this.sg = _sg;
        this.c1 = _c1;
        this.c2 = _c2;
    }
    
    public static void startIdentification() {
        semaphore.release();
    }

    @Override
    public void run() {
        int part = m.identifyPart();
        sg.writeInGUI(java.time.LocalTime.now() + " - Box is of type " + part);
        sg.updatePackageCount(part);
        m.d1queue.add(part);
        sg.writeInGUI("Added part of Type " + part + " to D1");
        sg.writeInGUI("Distribution1 queue: "+ m.d1queue);
        ThreadDistribution1 threadDistribution1 = new ThreadDistribution1(sg, m, c1, c2);
        threadDistribution1.start();
        try {
            threadDistribution1.join();
        } catch (InterruptedException ex) {
            Logger.getLogger(ThreadIdentify.class.getName()).log(Level.SEVERE, null, ex);
        }
        while(c1.packageDetected())m.Sleep(1);
    }
}


import java.util.concurrent.Semaphore;
import java.util.logging.Level;
import java.util.logging.Logger;


public class ThreadDistribution2 extends Thread{
    private SplitterConveyor sc;
    public SystemGUI sg;
    private Mechanism m;
    Cylinder c2;
    
    public ThreadDistribution2(SystemGUI _sg, Mechanism _m, Cylinder _c2){
        this.sg = _sg;
        this.m = _m;
        this.c2 = _c2;
    }
    private static final Semaphore semaphore = new Semaphore(0);
    
    public static void startDistribution2() {
        semaphore.release();
    }
    
    @Override
    public void run(){
        while(true){
            try{semaphore.acquire();}
            catch (InterruptedException ex) {}
            
            while(!c2.packageDetected())m.Sleep(1);
            int partType2 = m.consume_d2();
            if((sg.dock2 == partType2 && !m.dock2_full) || (partType2 == 1 && !m.dock2_full && m.dock1_full)||(partType2 == 3 && !m.dock2_full && m.dockend_full)){
                sg.writeInGUI("Cylinder 2 handled part of Type " + partType2);
                sg.writeInGUI("Distribution2 queue: " + m.d2queue);
                m.moving_cyl_state=true;
                m.stopConveyor();
                CylinderThread tcylinder = new CylinderThread(c2);
                tcylinder.start();
                try {
                tcylinder.join();
                } catch (InterruptedException ex) {
                Logger.getLogger(ThreadGetPart.class.getName()).log(Level.SEVERE, null, ex);
                }
                m.moving_cyl_state=false;
                m.moveConveyor();
                sg.updateDockCount(2, partType2);

            } else {
                sg.writeInGUI("DockEnd received part of Type " + partType2);
                sg.writeInGUI("Distribution2 queue: "+ m.d2queue);
                sg.updateDockCount(3, partType2);
            }   
            //wait for current package to leave sensor, only then a new package is handled
            while(c2.packageDetected())m.Sleep(1);
        }
    }
}

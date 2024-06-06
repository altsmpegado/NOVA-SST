
import java.util.concurrent.Semaphore;
import java.util.logging.Level;
import java.util.logging.Logger;

public class ThreadDistribution1 extends Thread {
    
    private SplitterConveyor sc;
    public SystemGUI sg;
    private Mechanism m;
    Cylinder c1;
    Cylinder c2;
    
    public ThreadDistribution1(SystemGUI _sg, Mechanism _m, Cylinder _c1, Cylinder _c2){
        
        this.sg = _sg;
        this.m = _m;
        this.c1 = _c1;
        this.c2 = _c2;
    }
        
    @Override
    public void run(){
            int partType = m.consume_d1();
            
            if((c1.packageDetected()&& sg.dock1 == partType && !m.dock1_full)||(partType == 2 && !m.dock1_full && m.dock2_full)||(partType== 3 && !m.dock1_full && m.dockend_full)){
                sg.writeInGUI("Cylinder 1 handled part " + partType);
                sg.writeInGUI("Distribution1 queue: "+ m.d1queue);
                m.moving_cyl_state=true;
                m.stopConveyor();
                CylinderThread tcylinder = new CylinderThread(c1);
                tcylinder.start();
                try {
                    tcylinder.join();
                } catch (InterruptedException ex) {
                    Logger.getLogger(ThreadGetPart.class.getName()).log(Level.SEVERE, null, ex);
                }
                m.moving_cyl_state=false;
                m.moveConveyor();
                sg.updateDockCount(1, partType);
            }
            
            else{
                sg.writeInGUI("Part of Type " + partType + " transfered to D2");
                sg.writeInGUI("Distribution1 queue: "+ m.d1queue);
                m.d2queue.add(partType);
                sg.writeInGUI("Added part of Type " + partType + " to D2");
                sg.writeInGUI("Distribution2 queue: "+ m.d2queue);
                ThreadDistribution2.startDistribution2();
            }
    }
}

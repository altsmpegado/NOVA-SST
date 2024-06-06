
import java.util.concurrent.ArrayBlockingQueue;

public class ThreadDockSwitches extends Thread {
    
    private final Mechanism m;
    SplitterConveyor sc;
    public boolean k = false;
    private final SystemGUI sg;
    public static ArrayBlockingQueue ledqueue = new ArrayBlockingQueue(10);
    
    public ThreadDockSwitches(Mechanism _m, SplitterConveyor _sc, SystemGUI _sg){
        this.m = _m;
        this.sc = _sc;
        this.sg = _sg;
    }
    
    @Override
    public void run(){
        
        while(true){
            if(sc.isDock1Active() && m.dock1_full){
                m.dock1_full = false;
                sg.writeInGUI("Dock 1 available!");
                m.Sleep(500); //a execução destas açoes sao tao rapidas que um simples toque no botao resulta em varias execuçoes das funçoes, isto impede tal
            } else if(sc.isDock1Active()){
                m.dock1_full = true;
                ledqueue.add("blink");
                sg.writeInGUI("Dock 1 is now blocked!");
                m.Sleep(500); 
            }
                
            if(sc.isDock2Active() && m.dock2_full){
                m.dock2_full = false;
                sg.writeInGUI("Dock 2 available!");
                m.Sleep(500);
            } else if(sc.isDock2Active()){
                m.dock2_full = true;
                ledqueue.add("blink");
                sg.writeInGUI("Dock 2 is now blocked!");
                m.Sleep(500);
            }
                
            if(sc.isDockEndActive() && m.dockend_full){
                 m.dockend_full = false;
                 sg.writeInGUI("Dock End available!");
                 m.Sleep(500);
            } else if(sc.isDockEndActive()){
                m.dockend_full = true;
                ledqueue.add("blink longer");
                sg.writeInGUI("Dock End is now blocked!");
                m.Sleep(500);
            }
                
            if(m.dock1_full && m.dock2_full && m.dockend_full && !k){
                sg.writeInGUI("The system is now blocked!");
                m.blocked_state = true;
                ledqueue.add("blocked");
                k = true;
            } else if((k && !m.dock1_full) || (k && !m.dock2_full) || (k && !m.dockend_full)){
                sg.writeInGUI("The system is now unblocked!");
                m.blocked_state = false;
                ledqueue.add("turnoff");
                k = false;
            }
        }
    }
}


public class ThreadLeds extends Thread {
    
    String ledstate;
    private Mechanism m;
    
    public ThreadLeds(Mechanism _m){
        this.m = _m;
    }
    
    private void blinkLed(){
        m.switchLed(1000);
        m.Sleep(100);
    }
    
    private void blinkLongerLed(){
        m.switchLed(3000);
        m.Sleep(100);
    }
    
    private void emergencyLed(){
        while(true){
            m.switchLed(400);
            m.Sleep(400);
            if(!ThreadDockSwitches.ledqueue.isEmpty())
                break;
        }
    }
    
    private void blockedLed(){
        while(true){
            m.switchLed(500);
            m.Sleep(1000);
            if(!ThreadDockSwitches.ledqueue.isEmpty())
                break;
        }
    }
        
    @Override
    public void run(){
        while(true){
            try{ ledstate = (String) ThreadDockSwitches.ledqueue.take();
            } catch(InterruptedException ex) { continue; }
            switch(ledstate){
                case "blink": blinkLed(); break;
                case "blink longer": blinkLongerLed(); break;
                case "blocked": blockedLed(); break;
                case "emergency": emergencyLed(); break;
                default: break;
            }
        }
    }
}

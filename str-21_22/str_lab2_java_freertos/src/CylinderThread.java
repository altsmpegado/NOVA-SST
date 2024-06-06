
public class CylinderThread extends Thread{
    private Cylinder cylinder;
    
    public CylinderThread(Cylinder cylinder){
        this.cylinder = cylinder;
    }
    
    public void initializeCylinder(){
        this.cylinder.gotoPosition(1);
        this.cylinder.gotoPosition(0);
    }
    
    @Override
    public void run(){
        this.initializeCylinder();
    }
}

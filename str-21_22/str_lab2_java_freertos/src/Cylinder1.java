
public class Cylinder1 implements Cylinder { 
    
    SplitterConveyor sc;
    
    public Cylinder1(SplitterConveyor _sc) {
        this.sc = _sc;
    }
    
    @Override
    public void moveForward(){
        SplitterConveyor.cylinder1MoveForward();
    }
    
    @Override
    public void moveBackward(){
        SplitterConveyor.cylinder1MoveBackward();
    }
    
    @Override
    public void stop(){
        SplitterConveyor.cylinder1Stop();
    }
    
    @Override
    public int getPosition(){
        return SplitterConveyor.getCylinder1Position();
    }
    
    @Override
    public void gotoPosition(int position){
        if (getPosition() < position) {
            moveForward();
        } 
        else if (getPosition() > position) {
            moveBackward();
        }
        //   while position not reached
        while (getPosition() != position) {
            try {Thread.sleep(1);} 
            catch (InterruptedException ex) {}
        }
        stop();
    }

    @Override
    public boolean isMoving() {
        if(getPosition()!=0)
            return true;
        else 
            return false;
    }

    @Override
    public boolean isMovingForward() {
        return SplitterConveyor.isCylinder1MovingForward();
    }

    @Override
    public boolean isMovingBackward() {
        return SplitterConveyor.isCylinder1MovingBackward();
    }

    @Override
    public boolean isStopped() {
        if(getPosition()== 0)
            return true;
        else 
            return false;
    }

    @Override
    public boolean packageDetected() {
        return SplitterConveyor.isPartAtDock1();
    }

    @Override
    public void calibration() {
        ThreadCalibration threadCalibration = new ThreadCalibration(this, "Cylinder 1");
        threadCalibration.startCalibration();
    }
}

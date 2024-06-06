
public class CylinderStart implements Cylinder {
    
    SplitterConveyor sc;
    
    public CylinderStart(SplitterConveyor __sc) {
        this.sc = __sc;
    }
    
    @Override
    public void moveForward(){
        SplitterConveyor.cylinderStartMoveForward();
    }
    
    @Override
    public void moveBackward(){
        SplitterConveyor.cylinderStartMoveBackward();
    }
    
    @Override
    public void stop(){
        SplitterConveyor.cylinderStartStop();
    }
    
    @Override
    public int getPosition(){
        return SplitterConveyor.getCylinderStartPosition();
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
        return SplitterConveyor.isCylinderStartMovingForward();
    }

    @Override
    public boolean isMovingBackward() {
        return SplitterConveyor.isCylinderStartMovingBackward();
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
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public void calibration() {
        ThreadCalibration threadCalibration = new ThreadCalibration(this, "Cylinder start");
        threadCalibration.startCalibration();
    }
}

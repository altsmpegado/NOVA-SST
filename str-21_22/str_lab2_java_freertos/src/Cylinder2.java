
import java.util.Scanner;

public class Cylinder2 implements Cylinder {
    SplitterConveyor sc;
    
    public Cylinder2(SplitterConveyor __sc) {
        this.sc = __sc;
    }
    
    @Override
    public void moveForward(){
        SplitterConveyor.cylinder2MoveForward();
    }
    
    @Override
    public void moveBackward(){
        SplitterConveyor.cylinder2MoveBackward();
    }
    
    @Override
    public void stop(){
        SplitterConveyor.cylinder2Stop();
    }
    
    @Override
    public int getPosition(){
        return SplitterConveyor.getCylinder2Position();
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
        return SplitterConveyor.isCylinder2MovingForward();
    }

    @Override
    public boolean isMovingBackward() {
        return SplitterConveyor.isCylinder2MovingBackward();
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
        return SplitterConveyor.isPartAtDock2();
    }

    @Override
    public void calibration() {
        ThreadCalibration threadCalibration = new ThreadCalibration(this, "Cylinder 2");
        threadCalibration.startCalibration();
    }
}   

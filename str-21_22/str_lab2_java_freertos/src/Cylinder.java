
import java.util.concurrent.Semaphore;


public interface Cylinder {
    static final Semaphore sem_calibrated = new Semaphore(0);
    public void moveForward();
    public void moveBackward();
    public void stop();
    public void gotoPosition(int position);
    public int getPosition(); // return 0,1 or -1 (between)
    public boolean isMoving();
    public boolean isMovingForward();
    public boolean isMovingBackward();
    public boolean isStopped();
    public boolean packageDetected();
    public void calibration();
}


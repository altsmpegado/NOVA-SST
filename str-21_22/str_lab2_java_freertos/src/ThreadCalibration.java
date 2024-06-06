import java.util.Scanner;

public class ThreadCalibration{

    public Cylinder c;
    public String name;

    public ThreadCalibration(Cylinder _c, String _name){
        this.c = _c;
        this.name = _name;
    }

    private boolean calibrate(Cylinder c){

        while(c.isMoving() && c.getPosition()!=-1){}
        while(c.isMoving() && c.getPosition()==-1){}
        c.stop();
        return c.getPosition()==0;
    }

    public void startCalibration() {
        Scanner scan = new Scanner(System.in);
        int key;
        System.out.println(name + " calibration! 1 - In, 2 - Out");
        while(c.getPosition()!=0){
            key = scan.nextInt();
            switch(key){
                case 1: c.moveForward(); break;
                case 2: c.moveBackward(); break;
                default: break;
            }
            if(!calibrate(c))
                System.out.println("Not calibrated at sensor 0...");
            else{
                System.out.println("Cylinder calibrated at sensor 0!");
                break; //breaks the loop and the thread dies
            }
        }
    }
}


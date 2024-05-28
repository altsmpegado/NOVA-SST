package tcp;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author alts
 */

public class Raspberry {
    
    /* variables */
    private final Client client;
    /* primary actions strings */
    private final String leftC = "1";
    private final String rightC = "2";
    private final String leftD = "3";
    private final String rightD = "4";
    private final String stopC = "9";
    private final String stopD = "10";
    /* transitions strings */
    private final String tSendCL = "L5";
    private final String tSendCR = "R5";
    private final String tRcvCL = "L6";
    private final String tRcvCR = "R6";
    private final String tSendDL = "L7";
    private final String tSendDR = "R7";
    private final String tRcvDL = "L8";
    private final String tRcvDR = "R8";
    
    /* constructor */
    public Raspberry(Client _client) throws Exception{
        this.client = _client;
    }
    
    /* functions */
    public void moveCLeft() throws IOException{
        System.out.println("EVENT: C execution left");
        client.sendMessage(leftC);
    }
    
    public void moveCRight() throws IOException{
        System.out.println("EVENT: C execution right");
        client.sendMessage(rightC);
    }
    
    public void moveDLeft() throws IOException{
        System.out.println("EVENT: D execution left");
        client.sendMessage(leftD);
    }
    
    public void moveDRight() throws IOException{
        System.out.println("EVENT: D execution right");
        client.sendMessage(rightD);
    }
    
    public void transitionSendCL() throws IOException{
        System.out.println("EVENT: CL sending box");
        client.sendMessage(tSendCL);
    }
    
    public void transitionSendCR() throws IOException{
        System.out.println("EVENT: CR sending box");
        client.sendMessage(tSendCR);
    }
    
    public void transitionReceiveCL() throws IOException{
        System.out.println("EVENT: CL receiving box");
        client.sendMessage(tRcvCL);
    }
    
    public void transitionReceiveCR() throws IOException{
        System.out.println("EVENT: CR receiving box");
        client.sendMessage(tRcvCR);
    }
    
    public void transitionSendDL() throws IOException{
        System.out.println("EVENT: DL sending box");
        client.sendMessage(tSendDL);
    }
    
    public void transitionSendDR() throws IOException{
        System.out.println("EVENT: DR sending box");
        client.sendMessage(tSendDR);
    }
    
    public void transitionReceiveDL() throws IOException{
        System.out.println("EVENT: DL receiving box");
        client.sendMessage(tRcvDL);
    }
    
    public void transitionReceiveDR() throws IOException{
        System.out.println("EVENT: DR receiving box");
        client.sendMessage(tRcvDR);
    }
    
    public void stopC() throws IOException{
        System.out.println("EVENT: C stop");
        client.sendMessage(stopC);
    }
    
    public void stopD() throws IOException{
        System.out.println("EVENT: D stop");
        client.sendMessage(stopD);
    }
    
}

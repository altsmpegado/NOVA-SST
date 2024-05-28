package tcp;

/**
 *
 * @author João Amorim
 */
public class Arduino {
   
    /* Variables */
    private final String kit;
    private final Client client;
    
    /* Actions strings */
    final String run = "1";
    final String stop = "2";
    final String emergency = "3";
    final String resume = "4";
    final String tSend = "5";
    final String tRcv = "6";
    final String sensor = "7";

    /* Constructor */
    public Arduino(String _kit, Client _client) throws Exception{
        this.kit = _kit;
        this.client = _client;  
    }
    
    /* functions */
    public void execution() throws Exception{
        System.out.println("EVENT: " + kit + " execution.");
        client.sendMessage(run);
    }
    
    public void stop() throws Exception{
        System.out.println("EVENT: " + kit + " stop.");
        client.sendMessage(stop);
    }
    
    public void resume() throws Exception{
        System.out.println("EVENT: " + kit + " resume.");
        client.sendMessage(resume);
    }
    
    public void transition_send() throws Exception{
        System.out.println("EVENT: " + kit + " sending box.");
        client.sendMessage(tSend);
    }
    
    public void transition_rcv() throws Exception{
        System.out.println("EVENT: " + kit + " receiving box.");
        client.sendMessage(tRcv);
    }
    
    public void emergency_stop() throws Exception{
        System.out.println("EVENT: " + kit + " emergency stop.");
        client.sendMessage(emergency);
    }
    
     public void sensor() throws Exception{
         System.out.println("EVENT: " + kit + " getting sensor state.");
         client.sendMessage(sensor);
     }
    
}

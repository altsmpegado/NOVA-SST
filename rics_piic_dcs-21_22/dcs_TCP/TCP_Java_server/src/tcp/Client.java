package tcp;
import java.io.*;
import java.net.*;

public class Client {
    
    /* variables */
    private Socket clientSocket;
    private PrintWriter out;
    private BufferedReader in;
    private final String ip;
    private final int port;

    /* constructor */
    public Client(String _ip, int _port){
        this.ip = _ip;
        this.port = _port;
    }
    
    /* functions */
    public void startConnection() throws IOException {
        clientSocket = new Socket(ip, port);
        out = new PrintWriter(clientSocket.getOutputStream(), true);
        in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));  
    }

    public void sendMessage(String msg) throws IOException {
        System.out.println("Message to be sent: " + msg);
        out.println(msg);
    }
    
    public String receiveMessage() throws IOException {
        String resp = in.readLine();
        return resp;
    }

    public void stopConnection() throws IOException {
        in.close();
        out.close();
        clientSocket.close();
    }
}

// import UDP library
import hypermedia.net.*;

UDP udp;

final String serverIP = "239.13.10.13"; 
final int serverPort = 51013;
final int clientPort = 51014;

int backgroundColor = 0;

void setup() {
  size(400, 400);
  udp = new UDP(this, clientPort);
  udp.listen(true);
}

void draw()
{
    background(backgroundColor);
}

void keyPressed() {
    
    if (key == 'c')
    {
        println("Connecting");
        String message = "HELLOPHOTON";
        udp.send(message, serverIP, serverPort);
    }
    
    if (key == 'd')
    {
        println("Disconnecting");
        String message = "GOODBYEPHOTON";
        udp.send(message, serverIP, serverPort);
    }
}

void receive(byte[] data, String ip, int port)
{
  data = subset(data, 0, data.length);
  String message = new String(data);
  println( "[MSG]: " + message + " [FROM]: " + ip + ":" + port);
  
  if (message.compareTo("Ok") != 0) {
      backgroundColor = Integer.parseInt(message);
  }
}
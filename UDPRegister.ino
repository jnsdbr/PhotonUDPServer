#define SERVER_PORT 51013
#define BUFFER_SIZE 100
#define CLIENT_PORT 51014
#define INTERVAL 1000

// UDP
UDP udp;

// Multicast IP
IPAddress multicastIP(239, 13, 10, 13);

// UDP message buffer
char buffer[BUFFER_SIZE];

// Keyword
String registerKeyword = "HELLOPHOTON";
String unregisterKeyword = "GOODBYEPHOTON";

// Clients
IPAddress clients[100];
int clientCount = 0;

// Inverval
unsigned int lastMessage;

void setup()
{
    Serial.begin(9600);
    
    // Setup UDP Connection
    udp.begin(SERVER_PORT);
    udp.joinMulticast(multicastIP);
    
    // Timer
    lastMessage = millis() + INTERVAL;
}

void loop()
{
    listenForClient();
    
    if (lastMessage <= millis()) {
        // Send random number to all clients
        sendMessageToClients(String(random(255)));
        lastMessage = millis() + INTERVAL;
    }
    
}

void sendMessageToClients(String message) {
    for (int i = 0; i < clientCount; i++) {
        udp.beginPacket(clients[i], CLIENT_PORT);
        udp.write(message);
        udp.endPacket();
    }
}

void addClient(IPAddress clientIP) {
    clients[clientCount] = clientIP;
    clientCount++;
}

void removeClient(IPAddress clientIP) {
    int result = searchForClient(clientIP);
    if (result >= 0) {
        for(int i = result; i < clientCount - 1; i++) {
            clients[i] = clients[i + 1];
        }
        clientCount--;
    }
}

int searchForClient(IPAddress clientIP) {
    for (int i = 0; i < clientCount; i++) {
        if (String(clients[i]) == String(clientIP)) {
            return i; 
        }
    }
    
    return -1;
}

void listenForClient() {
    int packetSize = udp.parsePacket();

    // Check if data has been received
    if (packetSize > 0)
    {
        buffer[packetSize] = '\0'; // End string
        udp.read(buffer, BUFFER_SIZE);
        String input = buffer;
        
        udp.flush();
        
        // Simple auth
        if (registerKeyword.equals(input.trim()))
        {
            // Store sender ip and port
            IPAddress ipAddress = udp.remoteIP();
            
            // Debugging
            Serial.print("[MSG]: Ok [TO]: ");
            Serial.print(ipAddress);
            Serial.print(":");
            Serial.println(CLIENT_PORT);
            
            addClient(ipAddress);
            
            // Echo back data to client
            udp.beginPacket(ipAddress, CLIENT_PORT);
            udp.write("Ok");
            udp.endPacket();            
        }
        else if (unregisterKeyword.equals(input.trim()))
        {
            // Store sender ip and port
            IPAddress ipAddress = udp.remoteIP();
            
            // Debugging
            Serial.print("[MSG]: Ok [TO]: ");
            Serial.print(ipAddress);
            Serial.print(":");
            Serial.println(CLIENT_PORT);            
            
            // Echo back data to client
            udp.beginPacket(ipAddress, CLIENT_PORT);
            udp.write("Ok");
            udp.endPacket();           
            
            removeClient(ipAddress);
        }
    }
}

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB};
IPAddress ip(192,168,1,220);
IPAddress server(192,168,1,1);

int failPin = 4;
int okPin = 5;

EthernetClient client;

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 300000;  // delay between updates, in milliseconds

String buffer;

void setup() {
  // start serial port:
  Serial.begin(9600);
  pinMode(failPin, OUTPUT);
  pinMode(okPin, OUTPUT);
  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection using a fixed IP address and DNS server:
  Ethernet.begin(mac, ip);
  // print the Ethernet board/shield's IP address:
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());
  lastConnectionTime = millis() - postingInterval;
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    buffer += c;
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    Serial.print("Errors: ");
    if (buffer[buffer.length() - 1] == '0') {
      digitalWrite(failPin, LOW);
      digitalWrite(okPin, HIGH);
      Serial.println("None");
    } else {
      digitalWrite(failPin, HIGH);
      digitalWrite(okPin, LOW);
      Serial.println("Yes");
    }
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    buffer = "";
    httpRequest();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /bb-arduino.php HTTP/1.1");
    client.println("Host: localhost");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}


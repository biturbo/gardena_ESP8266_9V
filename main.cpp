#include <SeeedOLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <FS.h>
#include "defines.h"

WiFiServer server(SERVER_PORT);

const char* ssid = "xxxx";
const char* password = "xxxx";

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output1State = "off";
String output2State = "off";
char buffer[20];

String get_ip(IPAddress _ip) {
  String ip = "";
  ip = _ip[0];
  ip += ".";
  ip += _ip[1];
  ip += ".";
  ip += _ip[2];
  ip += ".";
  ip += _ip[3];
  return ip;
}


void wifi_init() {
  SeeedOled.clearDisplay();
  SeeedOled.setTextXY(2, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("Waiting for");
  SeeedOled.setTextXY(4, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("WiFi connection");

  for (int i = 5; i > -1; i--) { // wait 10sec with fan on at start up to indicate sucessful reset on powerup
    Serial.print(i);
    Serial.print(' ');
    SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putNumber(i); //Print the Number
    SeeedOled.putString(" ");
    delay(1000);
  }
  Serial.println("");
  WiFi.persistent(false); // turn off persistent, fixing flash crashing issue

  WiFi.begin(ssid, password);
  Serial.println("WiFi.begin called waiting for connected");

  //  IPAddress ip(192, 168, 3, 24);
  //  IPAddress gateway(ip[0], ip[1], ip[2], 1); // set gatway to ... 1
  //  IPAddress subnet(255, 255, 255, 0);
  //  IPAddress dns(8, 8, 8, 8);

  Serial.print(F("Setting ip to DHCP: "));
  Serial.println("");
  Serial.print(F("HTTP Port: "));
  Serial.println(SERVER_PORT);

  //  if (!WiFi.config(ip, gateway, subnet, dns)) {
  //    Serial.println("WiFi.config() failed");
  //  }
  //  Serial.println("WiFi.config() succeeded");

  // Wait for connection for 15 sec and then just continue
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    SeeedOled.setTextXY(6, 0);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("finishing..."); //Print the String
  }

  Serial.println("");
  Serial.printf("Connect to %s\n", ssid);
  Serial.print("Module IP: ");
  Serial.println(WiFi.localIP());
  Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());

  if (!SPIFFS.begin()) {
    DEBUG_PRINTLN(F("failed to mount file system!"));
  }

}
void oled_init() {
  SeeedOled.init();  //initialze SEEED OLED display
  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
}

void disp_status(String output1State, String output2State) {

  if (output1State == "off" and output2State == "off")   {
    SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
    SeeedOled.setNormalDisplay();  //Set display to normal mode
    SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("IP: "); //Print the String
    IPAddress ip = WiFi.localIP(); // Convert IP Here
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    ipStr.toCharArray(buffer, 20);
    SeeedOled.putString((buffer));
    SeeedOled.setTextXY(1, 0);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("HTTP: "); //Print the String
    SeeedOled.putNumber(SERVER_PORT); //Print the Number
    SeeedOled.setTextXY(3, 0);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("SSID: "); //Print the String
    SeeedOled.putString(ssid); //Print the String
    SeeedOled.setTextXY(4, 0);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("RSSI: "); //Print the String
    SeeedOled.putNumber(WiFi.RSSI());
    SeeedOled.putString(" dBm "); //Print the String
    SeeedOled.setTextXY(6, 0);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("GARDENA V2.2"); //Print the String
    SeeedOled.setTextXY(7, 0);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("BOTH VALVE OFF"); //Print the String
  } else if (output1State == "on" and output2State == "off") {
    SeeedOled.setInverseDisplay();
    SeeedOled.clearDisplay();
    SeeedOled.setTextXY(2, 1);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("TERRASSE OPEN"); //Print the String
  } else if (output1State == "off" and output2State == "on") {
    SeeedOled.setInverseDisplay();
    SeeedOled.clearDisplay();
    SeeedOled.setTextXY(5, 3);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("RASEN OPEN"); //Print the String
  } else if (output1State == "on" and output2State == "on") {
    SeeedOled.setInverseDisplay();
    SeeedOled.clearDisplay();
    SeeedOled.setTextXY(2, 1);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("TERRASSE OPEN"); //Print the String
    SeeedOled.setTextXY(5, 3);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("RASEN OPEN"); //Print the String
  }
}

void do_setup() {

  DEBUG_BEGIN(115200);
  Serial.begin(115200);
  Serial.println("");

  digitalWrite (BRIDGE1_PIN, LOW);
  digitalWrite (BRIDGE2_PIN, LOW);
  digitalWrite (BRIDGE3_PIN, LOW);
  digitalWrite (BRIDGE4_PIN, LOW);
  pinMode (BRIDGE1_PIN, OUTPUT);
  pinMode (BRIDGE2_PIN, OUTPUT);
  pinMode (BRIDGE3_PIN, OUTPUT);
  pinMode (BRIDGE4_PIN, OUTPUT);
  digitalWrite (BRIDGE1_PIN, LOW);
  digitalWrite (BRIDGE2_PIN, LOW);
  digitalWrite (BRIDGE3_PIN, LOW);
  digitalWrite (BRIDGE4_PIN, LOW);

  Serial.println("");
  server.begin();
  Serial.println("HTTP server started");

  Wire.begin();
  oled_init();
  wifi_init();
  disp_status("off", "off");
}

void do_loop(void) {
  //  server.handleClient();

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the VALVE on and off
            if (header.indexOf("GET /1/open") >= 0) {
              Serial.println("VALVE 1 open");
              output1State = "on";
              digitalWrite (BRIDGE1_PIN, HIGH);
              digitalWrite (BRIDGE2_PIN, LOW);
              delay (250);
              digitalWrite (BRIDGE1_PIN, LOW);
            } else if (header.indexOf("GET /1/close") >= 0) {
              Serial.println("VALVE 1 closed");
              output1State = "off";
              SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
              SeeedOled.clearDisplay();
              SeeedOled.setTextXY(4, 1);         //Set the cursor to Xth Page, Yth Column
              SeeedOled.putString("TERRASSE CLOSED"); //Print the String
              digitalWrite (BRIDGE1_PIN, LOW);
              digitalWrite (BRIDGE2_PIN, HIGH);
              delay (62);
              digitalWrite (BRIDGE2_PIN, LOW);
            } else if (header.indexOf("GET /2/open") >= 0) {
              Serial.println("VALVE 2 open");
              output2State = "on";
              digitalWrite (BRIDGE3_PIN, HIGH);
              digitalWrite (BRIDGE4_PIN, LOW);
              delay (250);
              digitalWrite (BRIDGE3_PIN, LOW);
            } else if (header.indexOf("GET /2/close") >= 0) {
              Serial.println("VALVE 2 close");
              output2State = "off";
              digitalWrite (BRIDGE3_PIN, LOW);
              digitalWrite (BRIDGE4_PIN, HIGH);
              delay (62);
              digitalWrite (BRIDGE4_PIN, LOW);
            }

            disp_status(output1State, output2State);


            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>Gardena Web Server</h1>");

            // Display current state, and ON/OFF buttons for VALVE 1
            client.println("<p>Valve Terrasse - State " + output1State + "</p>");
            // If the output1State is off, it displays the ON button
            if (output1State == "off") {
              client.println("<p><a href=\"/1/open\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/close\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for VALVE 2
            client.println("<p>Valve Rasen - State " + output2State + "</p>");
            // If the output2State is off, it displays the ON button
            if (output2State == "off") {
              client.println("<p><a href=\"/2/open\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/close\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
  }
}



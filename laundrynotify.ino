/*********
  code started with:
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
  Arduino IDE example: Examples > Arduino OTA > BasicOTA.ino
*********/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Replace with your network credentials
const char* ssid = "";
const char* password = "";
const char* host = "";                  // hostname for esp board

const int VIB_SENSOR_PINS[] = {4,5};    // to which pins is a vibration sensor attached?
const int VIB_SENSOR_COUNT = 2;         // how many pins (must match the element count of VIB_SENSOR_PINS[]
const int POLL_MS = 10;                 // how frequently are the sensors polled

int sensor_count[] = {};                // used to tally sensor HIGH values at each POLL
String sensor_info = "";
int loop_counter = 0;

// setup web server
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // start web server
  server.begin();

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");
  ArduinoOTA.setHostname(host);

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  // setup serial
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // initialize sensor count array
  for (int i=0;i<VIB_SENSOR_COUNT;i++) {
    sensor_count[i] = 0;
  }
}

// prepare a web page to be send to a client (web browser)

void loop() {
  ArduinoOTA.handle();

  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          String response = "<!DOCTYPE HTML>\r\n<html>" +
            String (sensor_info) +
            "</html>\r\n";
          client.printf("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %u\r\n\r\n%s",
    response.length(), response.c_str());
          client.flush();
          break;
        }
      }
    }
    delay(1); // give the web browser time to receive the data

    // close the connection:
    client.stop();
    Serial.println("[Client disonnected]");
  }
  
  // increment loop coutner
  loop_counter = loop_counter + POLL_MS;
  
  // check sensor values
  for (int i=0;i<VIB_SENSOR_COUNT;i++) {
    if (digitalRead(VIB_SENSOR_PINS[i]) == HIGH) sensor_count[i]++;
  }
  
  // check for one second elapsed time
  if (loop_counter % 1000 == 0) {
    sensor_info = "";
    for (int j=0;j<VIB_SENSOR_COUNT;j++) {
      sensor_info = sensor_info +
        String(VIB_SENSOR_PINS[j]) +
        ": " +
        String(sensor_count[j]) +
        " ";
      sensor_count[j] = 0;
    }
    loop_counter = 0;
    Serial.println(sensor_info);
  }

  // wait
  delay(POLL_MS - 1);
}

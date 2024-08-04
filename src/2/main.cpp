#include <Arduino.h>

#include <WiFi.h>
const char *ssid = "Name of your WiFi netsork";
const char *password = "Password of your WiFi network. Usually a long digits";

WiFiServer server(80);
//---------------
// WiFi
//---------------
/*
#define WiFiMode_t wifi_mode_t
#define WIFI_OFF WIFI_MODE_NULL
#define WIFI_STA WIFI_MODE_STA
#define WIFI_AP WIFI_MODE_AP
#define WIFI_AP_STA WIFI_MODE_APSTA
*/
//---------------

#define LIGHT_SENSOR_PIN 36 // GIOP36 (ADC1_0)
// #define BATTERY_METER_PIN 39 // GIOP36 (ADC1_3)
#define SLEEP_TIME 12 // in seconds
// put function declarations here:
// int myFunction(int, int);

void setup()
{
  // put your setup code here, to run once:
  //  int result = myFunction(2, 3);
  //  Serial.begin(9600);
  Serial.begin(115200);
  unsigned long starttime = millis();
  while (!Serial)
    ;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Wifi Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  int analogValue = analogRead(LIGHT_SENSOR_PIN);
  //  Serial.print("Analog Value = ");
  //  Serial.print(analogValue);
  // put your main code here, to run repeatedly:

  WiFiClient client = server.available();
  if (client)
  {
    while (client.connected())
    {
      int analogValue = analogRead(LIGHT_SENSOR_PIN);
      client.println("HTTP/1.1 200 OK");
      //      client.println("Content-Type: text/html; charset=utf-8;");
      client.println();

      client.println("<!DOCTYPE html>");
      //      client.println("<style> * { font-family: sans-serif; } </stile>");
      //      client.println("<head><meta charset='UTF-8'></head>");
      //      client.println("<title>ESP32 with LDR </title>");
      //      client.println("</head><body><h1>Measure brightness</h1>");
      //      client.println(" <p> here</p> </body></html>");
      client.println("<html><body style='font-family: sans-serif'><p>");
      client.println(analogValue);
      client.println("</body></html>");
      Serial.println(analogValue);
      //      delay(2000);
    }
    client.stop();
  }

  //  Serial.println("");
  //      delay(1000);
  uint64_t sleeptime = SLEEP_TIME * 1000000 - (millis() - starttime) * 1000;
  esp_deep_sleep(sleeptime);
}

void loop()
{
}

// put function definitions here:
/*
int myFunction(int x, int y)
{
  return x + y;
}
*/

#include <Arduino.h>

#include <WiFi.h>
const char *ssid = "XXXXX";   // name of your WiFi network
const char *password = "00000000000000000000" // password of your WiFi network
WiFiServer server(80);

//---------------

#define LIGHT_SENSOR_PIN 36 // GIOP36 (ADC0)

// put function declarations here:
// int myFunction(int, int);

void setup()
{
  // put your setup code here, to run once:
  //  int result = myFunction(2, 3);
  Serial.begin(9600);
  while (!Serial)
    ;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Wifi Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{

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
      client.println();

      client.println("<!DOCTYPE html>");
      client.println("<html><body style='font-family: sans-serif'><p>");
      client.println(analogValue);
      client.println("</body></html>");
      //      Serial.println("client connected");
      delay(2000);
    }
    client.stop();
  }

  //  Serial.println("");
  delay(1000);
}

// put function definitions here:
/*
int myFunction(int x, int y)
{
  return x + y;
}
*/

// A quick tutorial to post data to Sensus using NodeMCU
// This program sends data every 10 seconds
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;
HTTPClient http;

void setup() {  
    // Connect serial port
    Serial.begin(115200);    
    Serial.print("Serial port OK!\n");

    // Connect to wifi
    Serial.print("Connecting to wifi... \n");
    WiFiMulti.addAP("[WIFI SSID]", "[WIFI PASSWORD]");    
}

String gettoken()
{
    http.begin("http://sensus.periapsys.net/api/v3/device/init/[DEVICE ID]");

    // Specify content-type header
    // I am using Postman (Google Chrome extension) to generate hashed username + password
    // Open Postman once installed
    // From Authorization, choose Basic Auth. Enter your username + password
    // Click Update Request
    // Click Code. You may see a line that contains Basic xxxxxxxxxxxxxx
    // That's the your hashed username + password = [HASHED VALUE]
    http.addHeader("authorization", "Basic [HASHED VALUE]");

    // Send the request
    int httpCode = http.GET();

    // Get the response payload
    String token = http.getString();     

    // Print HTTP return code
    Serial.println(httpCode);

    // Print request response payload
    Serial.println(token);

    // Close connection
    http.end();                   

    return token;
}

String token = "";

void loop() {  
    // Wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
      Serial.print("Wifi is connected! \n");

      if (token == "")
      {
        Serial.print("Getting token... \n");
        token = gettoken();
      }

      Serial.print("Beginning POST... \n");
      http.begin("http://sensus.periapsys.net/api/v3/device/post");
      http.addHeader("authorization", token);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Sample post
      int httpCode = http.POST("Amount=1000");

      // Print HTTP return code
      Serial.println(httpCode);

      // If token is expire which causes 401
      if (httpCode == 401)
      {
           Serial.print("Unauthorized request!");
        
          // Get new token
          token = gettoken();
      }
    }    

    delay(10000);
}



#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>    // Nokia lcd graphics library
#include <Adafruit_PCD8544.h>// Main library
#include <ArduinoJson.h>

char ssid[] = "Vodafone-049B";       // your network SSID (name)
char password[] = "ERIC_Maurin29";  // your network key                                                     
long int cases,todayCases,deaths,todaydeaths,recovered;

Adafruit_PCD8544 display = Adafruit_PCD8544(2, 0, 4, 5, 16);// Defining pins of Nodemcu (D4,D3,D2,D1,D0)

void setup() 
 {
   Serial.begin(115200);
   WiFi.begin(ssid, password);
   display.begin();
   display.setContrast(55);
   display.setTextColor(BLACK);
   display.clearDisplay();
   display.setCursor(0,0);
   display.print("Covid19 server");
   display.setCursor(13,8);
   display.print("Connecting");
   display.display();
   while (WiFi.status() != WL_CONNECTED) 
    {
     for(int i=0;i < 75;i++)
     {
     display.setCursor(i,16);
     display.print(".");
     delay(10);
     display.display();
    }
    }
     display.clearDisplay();
     delay(1000);
     display.setCursor(0,0);
     display.print("Connected");
     display.setCursor(0,8);
     display.print("IP Address"); 
     display.setCursor(0,16);
     display.print(WiFi.localIP());     
     Serial.println(WiFi.localIP());
     display.display();
     delay(2000);
     display.clearDisplay();
    //}
 }
//  Main loop will run continuously  /////////////////
void loop()
 {
  if(WiFi.status() == WL_CONNECTED)
   {
    Serial.println("Connected");
      IncomingData("http://coronavirus-19-api.herokuapp.com/countries/World");  // Url for getting day of covid-19 on Earth
    Display("World",cases,todayCases,deaths,todaydeaths,recovered);  // Calling display function and passing world as a string
      IncomingData("http://coronavirus-19-api.herokuapp.com/countries/USA");  // Urls as per countries, check the Name formate through the main (all) url
    Display("USA",cases,todayCases,deaths,todaydeaths,recovered);
      IncomingData("http://coronavirus-19-api.herokuapp.com/countries/India");
    Display("India",cases,todayCases,deaths,todaydeaths,recovered);
      IncomingData("http://coronavirus-19-api.herokuapp.com/countries/Germany");
    Display("Germany",cases,todayCases,deaths,todaydeaths,recovered);
      IncomingData("http://coronavirus-19-api.herokuapp.com/countries/Spain");
    Display("Spain",cases,todayCases,deaths,todaydeaths,recovered);
   }
   else  // If nodemcu will get disconnect to the Internet provider,It will start making connection request
   {
      while (WiFi.status() != WL_CONNECTED) 
    {
     display.clearDisplay();
     display.setCursor(29,0);
     display.print("No");
     display.setCursor(20,0);
     display.print("No");
     display.setCursor(20,16);
     display.print("INTERNET");
     display.setCursor(20,32);
     display.print("Connection");
     delay(1000);
    } 
   }
 }
 
int long IncomingData(String url)
 {
  HTTPClient http;
 // Serial.print("[HTTP] begin...\n");
  http.begin(url); //HTTP
 // Serial.print("[HTTP] GET...\n");
  int httpCode = http.GET();
  if (httpCode > 0) 
  {
 //  Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) 
     {
       String payload = http.getString();
 //      Serial.println(payload);
       const size_t buffersize = JSON_OBJECT_SIZE(12)+317;  // total size of JSON string
       DynamicJsonBuffer jsonBuffer(buffersize);
       JsonObject& root = jsonBuffer.parseObject(payload); // Parsing 

       cases = root["cases"];
       todayCases =root["todayCases"];
       deaths = root["deaths"];
       todaydeaths = root["todayDeaths"];
       recovered = root["recovered"];
      }      
    else
      {
  //     Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
   //    Serial.println("Error.");
      }
      return cases,todayCases,deaths,todaydeaths,recovered;
    http.end();   //Close connection
  }
  delay(100);
}

void Display(String Country, long int a,long int b, long int c, long int d,long int e)
{
/* a =cases
 * b =todaycases
 * c = deaths
 * d= todaydeaths
 * e=recovered
 */
 display.clearDisplay();
 display.setCursor(29,0);
 display.print(Country);
//Serial.print(Country);
 display.setCursor(0,8);
 display.print("Total:");
 display.setCursor(40,8);
 display.print(a);
  display.setCursor(0,16);
 display.print("Today:");
 display.setCursor(40,16);
 display.print(b);
  display.setCursor(0,24);
 display.print("Death:");
 display.setCursor(40,24);
 display.print(c);
  display.setCursor(0,32);
 display.print("T Death:");
 display.setCursor(50,32);
 display.print(d);
  display.setCursor(0,40);
 display.print("Safed:");
 display.setCursor(40,40);
 display.print(e);
display.display();
delay(7000);
}

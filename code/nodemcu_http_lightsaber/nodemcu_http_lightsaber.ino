#include <Wire.h> 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

const char* ssid = "ssid";
const char* password = "password";

// ******** ADAFRUIT NEOPIXEL ********
#define LED_PIN          12
#define NUMPIXELS        1
#define KYBER_CRYSTAL    0
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
// ************* CONST ***************
#define RED_32B          0xFF0000
#define ORANGE_32B       0xFF8000
#define GREEN_32B        0x00FF00
// ***********************************

int red    = 0;
int green  = 255;
int blue   = 0;

int brightnessMax   = 100;
int brightnessMin   = 20;
int brigthnessSpeed = 15;




ESP8266WebServer server(80);



void handleRoot(){
 server.send(200, "text/html", 
  "<script>"
    "var colorWell;"
    "var defaultColor = '#0000ff';"
    "window.addEventListener('load', startup, false);"
    
    " function startup() {"
      "colorWell = document.querySelector('#colorWell');"
      "colorWell.value = defaultColor;"
      "colorWell.addEventListener('change', updateAll, false);"
      "colorWell.addEventListener('input', updateAll, false);"
      "colorWell.select();"
    "}"
    
   " function updateAll(event) {"
	"var hexColor = event.target.value;"
	"var r = hexToRgb(hexColor).r;"
	"var g = hexToRgb(hexColor).g;"
	"var b = hexToRgb(hexColor).b;"
  
        "const Http = new XMLHttpRequest();"
	"var  url = 'http://192.168.1.13';"
	"url +='/changeColor' +'?r='+ r +'&g='+ g +'&b='+ b;"
  
	"console.log(url);"
        "Http.open('GET', url);"
        "Http.send();"
    "}"
    
    " function hexToRgb(hex) {"
	 "console.log('hex is ' + hex);"
         "var result = \/^#?([a-f\\d]{2})([a-f\\d]{2})([a-f\\d]{2})$\/i.exec(hex);"
	 " return result ? {"
		"r: parseInt(result[1], 16), g: parseInt(result[2], 16), b: parseInt(result[3], 16)"
	  "} : null;"
    "}"
      
    "</script>"
    "<label for='colorWell'>Lightsaber's color :</label>"
    "<input type='color' value='#ff0000' id='colorWell'>"
    
    );
}

void changeColor(){
    red = server.arg(0).toInt();
    green = server.arg(1).toInt();
    blue = server.arg(2).toInt();

    Serial.print("Red : ");
    Serial.println(red);

    Serial.print("Green : ");
    Serial.println(green);

    Serial.print("Blue : ");
    Serial.println(blue);

    pixels.setPixelColor(KYBER_CRYSTAL, pixels.Color(red,green,blue)); 
    pixels.show(); 

    handleRoot();
}

void changeBlink(){
    brightnessMin   = server.arg(0).toInt();
    brightnessMax   = server.arg(1).toInt();
    brigthnessSpeed = server.arg(2).toInt();
    handleRoot();
}

void setup(void) {
  Serial.begin(115200);
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.println("Connecting to Wifi...");
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    pixels.setPixelColor(KYBER_CRYSTAL, ORANGE_32B);
    pixels.show();  
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  pixels.setPixelColor(KYBER_CRYSTAL, GREEN_32B);
  pixels.show();  

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  // Route
  server.on("/", handleRoot);
  server.on("/changeColor", changeColor);
  server.on("/changeBlink", changeBlink);
  
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Starting Loop");
}

void blinkColor(){
  for (float i = brightnessMax; i >= brightnessMin; i--) {
    applyBrigthness(i);
  }
  for (float i = brightnessMin ; i <= brightnessMax; i++) {
    applyBrigthness(i);
  }
}

void applyBrigthness(float i){
    pixels.setPixelColor(KYBER_CRYSTAL, pixels.Color(red*(i/100), green*(i/100), blue*(i/100))); 
    pixels.show();
    delay(brigthnessSpeed);
    server.handleClient();
}

void loop() {
  server.handleClient();
  blinkColor();
}

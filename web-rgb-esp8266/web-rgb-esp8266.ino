/* RGB web server with ESP8266 & Arduino
* analogWrite with values received from web page
* NodeMCU ESP8266 part
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

SoftwareSerial NodeMCU(5, 4); //RX, TX

const char *ssid = "Wifi";
const char *password = "";
String red = "", green = "", blue = "";
ESP8266WebServer webServer(80);

String webpage = ""
                 "<!DOCTYPE html><html><head><title>RGB control</title><meta name='mobile-web-app-capable' content='yes' />"
                 "<meta name='viewport' content='width=device-width' /></head><body style='margin: 0px; padding: 0px;'>"
                 "<canvas id='colorspace'></canvas></body>"
                 "<script type='text/javascript'>"
                 "(function () {"
                 " var canvas = document.getElementById('colorspace');"
                 " var ctx = canvas.getContext('2d');"
                 " function drawCanvas() {"
                 " var colours = ctx.createLinearGradient(0, 0, window.innerWidth, 0);"
                 " for(var i=0; i <= 360; i+=10) {"
                 " colours.addColorStop(i/360, 'hsl(' + i + ', 100%, 50%)');"
                 " }"
                 " ctx.fillStyle = colours;"
                 " ctx.fillRect(0, 0, window.innerWidth, window.innerHeight);"
                 " var luminance = ctx.createLinearGradient(0, 0, 0, ctx.canvas.height);"
                 " luminance.addColorStop(0, '#000000');"
                 " luminance.addColorStop(0.05, '#000000');"
                 " luminance.addColorStop(0.5, 'rgba(0,0,0,0)');"
                 " luminance.addColorStop(0.95, '#ffffff');"
                 " luminance.addColorStop(1, '#ffffff');"
                 " ctx.fillStyle = luminance;"
                 " ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);"
                 " }"
                 " var eventLocked = false;"
                 " function handleEvent(clientX, clientY) {"
                 " if(eventLocked) {"
                 " return;"
                 " }"
                 " function colourCorrect(v) {"
                 " return Math.round(1023-(v*v)/64);"
                 " }"
                 " var data = ctx.getImageData(clientX, clientY, 1, 1).data;"
                 " var params = ["
                 " 'r=' + colourCorrect(data[0]),"
                 " 'g=' + colourCorrect(data[1]),"
                 " 'b=' + colourCorrect(data[2])"
                 " ].join('&');"
                 " var req = new XMLHttpRequest();"
                 " req.open('POST', '?' + params, true);"
                 " req.send();"
                 " eventLocked = true;"
                 " req.onreadystatechange = function() {"
                 " if(req.readyState == 4) {"
                 " eventLocked = false;"
                 " }"
                 " }"
                 " }"
                 " canvas.addEventListener('click', function(event) {"
                 " handleEvent(event.clientX, event.clientY, true);"
                 " }, false);"
                 " canvas.addEventListener('touchmove', function(event){"
                 " handleEvent(event.touches[0].clientX, event.touches[0].clientY);"
                 "}, false);"
                 " function resizeCanvas() {"
                 " canvas.width = window.innerWidth;"
                 " canvas.height = window.innerHeight;"
                 " drawCanvas();"
                 " }"
                 " window.addEventListener('resize', resizeCanvas, false);"
                 " resizeCanvas();"
                 " drawCanvas();"
                 " document.ontouchmove = function(e) {e.preventDefault()};"
                 " })();"
                 "</script></html>";

void handleRoot() {
  // Serial.println("handle root..");
  red = webServer.arg(0); // read RGB arguments
  green = webServer.arg(1);
  blue = webServer.arg(2);

  if((red != "") && (green != "") && (blue != "")) {
    NodeMCU.print(red.toInt() / 4);     NodeMCU.print("R");
    NodeMCU.print(green.toInt() / 4);   NodeMCU.print("G");
    NodeMCU.print(blue.toInt() / 4);    NodeMCU.print("B");
    NodeMCU.println("\n");
  }
  
  Serial.print("Red: ");
  Serial.println(red.toInt() / 4); 
  Serial.print("Green: ");
  Serial.println(green.toInt() / 4); 
  Serial.print("Blue: ");
  Serial.println(blue.toInt() / 4); 
  Serial.println();
  
  webServer.send(200, "text/html", webpage);
}

void setup() {
  // Serial setup
  delay(1000);
  Serial.begin(9600);
  Serial.println();

  // Communication to Arduino setup
  NodeMCU.begin(4800);

  // Wifi connection setup
  Serial.print("Connecting to the network");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  Serial.print("IP Address of network: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://"); // Will print IP address in URL format
  Serial.print(WiFi.localIP());
  Serial.println("/");

  webServer.on("/", handleRoot);

  webServer.begin();
  Serial.println("Server started");
}

void loop() {
  webServer.handleClient();
}
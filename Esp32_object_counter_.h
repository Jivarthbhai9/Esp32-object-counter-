#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

const int sensorPin = 15; // GPIO pin connected to the OUT pin of the IR sensor
int counter = 0;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);

  // Configure ESP32 as an Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Serve the main webpage
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><head><meta http-equiv='refresh' content='2'></head><body>";
    html += "<h1>Object Count: " + String(counter) + "</h1>";
    html += "<form action=\"/reset\" method=\"POST\"><button type=\"submit\">Reset Count</button></form>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  // Handle reset counter
  server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request){
    counter = 0;
    request->send(200, "text/html", "<html><body><h1>Counter reset!</h1><a href='/'>Back</a></body></html>");
  });

  server.begin();
}

void loop() {
  static int lastState = HIGH;
  int currentState = digitalRead(sensorPin);

  // Check for a falling edge (object detected)
  if (lastState == HIGH && currentState == LOW) {
    counter++;
    Serial.print("Object Count: ");
    Serial.println(counter);
  }

  lastState = currentState;
  delay(50); // Debounce delay
}

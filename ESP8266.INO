#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Motor pins
#define IN1 D1  // Left motor forward
#define IN2 D2  // Left motor backward
#define IN3 D3  // Right motor forward
#define IN4 D4  // Right motor backward
#define ENA D5  // Left motor speed
#define ENB D6  // Right motor speed

ESP8266WebServer server(80);

// Motor speed (0-1023 for ESP8266 PWM)
int motorSpeed = 800; // default ~78% power

void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopMotors();

  // Connect Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Web routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/forward", HTTP_GET, moveForward);
  server.on("/backward", HTTP_GET, moveBackward);
  server.on("/left", HTTP_GET, turnLeft);
  server.on("/right", HTTP_GET, turnRight);
  server.on("/stop", HTTP_GET, stopMotors);
  server.on("/speed", HTTP_GET, setSpeed);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

// Web interface with speed slider
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>ESP8266 RC Car</title></head><body>";
  html += "<h1>RC Car Control</h1>";
  html += "<button onclick=\"location.href='/forward'\">Forward</button><br><br>";
  html += "<button onclick=\"location.href='/left'\">Left</button>";
  html += "<button onclick=\"location.href='/stop'\">Stop</button>";
  html += "<button onclick=\"location.href='/right'\">Right</button><br><br>";
  html += "<button onclick=\"location.href='/backward'\">Backward</button><br><br>";

  html += "<form action='/speed' method='GET'>Speed (0-1023): ";
  html += "<input type='number' name='value' min='0' max='1023' value='" + String(motorSpeed) + "'>";
  html += "<input type='submit' value='Set Speed'></form>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Motor movement functions
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  server.sendHeader("Location", "/");
  server.send(303);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  server.sendHeader("Location", "/");
  server.send(303);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  server.sendHeader("Location", "/");
  server.send(303);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  server.sendHeader("Location", "/");
  server.send(303);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  server.sendHeader("Location", "/");
  server.send(303);
}

// Change motor speed
void setSpeed() {
  if (server.hasArg("value")) {
    int val = server.arg("value").toInt();
    motorSpeed = constrain(val, 0, 1023);
    Serial.print("Motor speed set to: ");
    Serial.println(motorSpeed);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}
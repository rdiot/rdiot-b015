/* Arduino Yun (ARDUINO-YUN) [B015] : http://rdiot.tistory.com/27 [RDIoT Demo] */

/* sample 
http://180.69.91.171//arduino/digital/13 : calls digitalRead(13);
http://180.69.91.171//arduino/digital/13/1 : calls digitalWrite(13,1);
http://180.69.91.171//arduino/analog/9/123 : analogWrite(9,123);
http://180.69.91.171//arduino/analog/2 : analogRead(2);
http://180.69.91.171//arduino/mode/13/input : pinMode(13, INPUT);
http://180.69.91.171//arduino/mode/13/output : pinMode(13, OUTPUT);
*/

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
//https://www.arduino.cc/en/Tutorial/Bridge
 
YunServer server;
 
void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  
  server.listenOnLocalhost();
  server.begin();
}
 
void loop() {
  YunClient client = server.accept();
 
  if (client) {
    process(client);
    client.stop();
  }
 
  delay(50); 
}
 
void process(YunClient client) {
  String command = client.readStringUntil('/');
 
  if (command == "digital") {
    digitalCommand(client);
  }
  if (command == "analog") {
    analogCommand(client);
  }
  if (command == "mode") {
    modeCommand(client);
  }
}
 
void digitalCommand(YunClient client) {
  int pin, value;
pin = client.parseInt();
 
  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
  } 
  else {
    value = digitalRead(pin);
  }
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.println(value);
 
  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}
 
void analogCommand(YunClient client) {
  int pin, value;
 
  pin = client.parseInt();
 
  if (client.read() == '/') {
    value = client.parseInt();
    analogWrite(pin, value);
 
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);
 
    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  }
  else {
    value = analogRead(pin);
 
    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" reads analog "));
    client.println(value);
 
    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}
 
void modeCommand(YunClient client) {
  int pin;
  pin = client.parseInt();
if (client.read() != '/') {
    client.println(F("error"));
    return;
  }
 
String mode = client.readStringUntil('\r');
 
  if (mode == "input") {
    pinMode(pin, INPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as INPUT!"));
    return;
  }
 
  if (mode == "output") {
    pinMode(pin, OUTPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as OUTPUT!"));
    return;
  }
 
  client.print(F("error: invalid mode "));
  client.print(mode);
}

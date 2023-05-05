//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

#include <FS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>


AsyncWebServer server(80);

// const char* ssid = "i-Keeper";
// const char* password = "iKeeperCERT509D2";

// const char* ssid = "Cherity-703";
// const char* password = "dasom4064*";

// const char* ssid = "AndroidHotspotef_bc_4e";
// const char* password = "00000000";

const char* ssid = "PASCUCCI2";
const char* password = "0000006767A";

// const char* ssid = "twosome2F_2G";
// const char* password = "twosome_1234";

// const char* ssid = "d2-502";
// const char* password = "d2502515";

const char* PARAM_MESSAGE = "commend";

Servo servo;
int pos = 0;
int servo_pin = 15;

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

// void action(AsyncWebServerRequest *request) {
//   Serial.println("ACTION!");

//   int params = request->params();
//   for (int i = 0; i < params; i++) {
//     AsyncWebParameter* p = request->getParam(i);
//     Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
//   }
//   request->send_P(200, "text/plain", "OK");
// }

// void action(AsyncWebServerRequest *request) {
//   Serial.println("ACTION!");
//   AsyncWebParameter* p = request->getParam("message", true);
//   Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
//   request->send_P(200, "text/plain", "OK");
// }

void servo_activate() {
  if (pos == 90) {
    Serial.println("Already Activate");
    return;
  } else {
    pos = 180;
    while (pos != 90) {
      servo.write(pos);
      delay(15);
      pos--;
    }
  }
}

void servo_deactivate() {
  if (pos == 180) {
    Serial.println("Already Deactivate");
    return;
  } else {
    pos = 90;
    while (pos != 180) {
      servo.write(pos);
      delay(15);
      pos++;
    }
  }
}


void setup() {

  Serial.begin(115200);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo.setPeriodHertz(50);
  servo.attach(servo_pin, 500, 2400);

  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    WiFi.begin(ssid, password);
    delay(1000);
  }

  Serial.println("Success WiFi connect!!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "");
  });

  // // Send a GET request to <IP>/get?message=<message>
  // server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   String message;
  //   if (request->hasParam(PARAM_MESSAGE)) {
  //     message = request->getParam(PARAM_MESSAGE)->value();
  //   } else {
  //     message = "No message sent";
  //   }
  //   request->send(200, "text/plain", "Hello, GET: " + message);
  // });

  //GET 요청은 됨, POST 요청은 안됨

  // Send a POST request to <IP>/post with a form field message set to <message>
  // server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
  //     AsyncWebParameter* message;
  //     Serial.println(request->hasParam(PARAM_MESSAGE, true));
  //     if (request->hasParam(PARAM_MESSAGE, true)) {
  //         message = request->getParam(PARAM_MESSAGE, true);
  //     } else {
  //         // message = "No message sent";
  //         Serial.println("No message sent");
  //     }
  //     // request->send(200, "text/plain", "Hello, POST: " + message);
  //     request->send(200, "text/plain", "Hello, POST: ");
  // });

  server.on("/post", HTTP_POST, [](AsyncWebServerRequest* request) {
    AsyncWebParameter* message = request->getParam(PARAM_MESSAGE, true);
    String key = message->name().c_str();
    String value = message->value().c_str();
    Serial.printf("POST[%s]: %s\n", key, value);
    request->send(200, "text/plain", "Request OK");
    // add head json타입으로 받을 수 있게 헤더를 설정해줘야함
    if (value == "activate") {
      Serial.println("Servo Moter Activate");
      servo_activate();
    } else if (value == "deactivate") {
      Serial.println("Servo Moter Deactivate");
      servo_deactivate();
    }
  });

  // server.on("/post", HTTP_POST, action);
  server.onNotFound(notFound);

  server.begin();
}

void loop() {
}
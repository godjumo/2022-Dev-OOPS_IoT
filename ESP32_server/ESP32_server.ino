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
// #include <ESP32Servo.h>
#include <AccelStepper.h>

AsyncWebServer server(80);

// const char* ssid = "***"
// const char* password = "***"

const char* PARAM_MESSAGE = "commend";
bool activate_state = NULL;  // 차수벽 가동 상태 True : 차수벽이 올라가있는 상태, False : 차수벽이 내려가있는 상태
bool wall_activate_flag = NULL;
bool wall_deactivate_flag = NULL;

// Servo servo;
// int pos = 0;
// int servo_pin = 15;

#define DIR 13
#define STEP 12
// #define ENABLE 14

AccelStepper stepper = AccelStepper(1, STEP, DIR);
int speed = 1000;

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

// void wall_activate() {
//   if (pos == 90) {
//     Serial.println("Already Activate");
//     return;
//   } else {
//     pos = 180;
//     while (pos != 90) {
//       servo.write(pos);
//       delay(15);
//       pos--;
//     }
//   }
// }

void wall_activate() {
  if (activate_state == true) {
    return;
  }
  else {
    Serial.println("Wall Activate");

    stepper.setSpeed(50);
    while(stepper.currentPosition() != 400) {
      stepper.runSpeed();
    }
    stepper.setSpeed(0);

    activate_state = true;
    wall_activate_flag = false;
  }
}

// void wall_activate() {
  
//   if (activate_state == true){
//     return;
//   }
//   else{
//     activate_state = true;

//     digitalWrite(DIR, HIGH);
//     for(int i = 0 ; i < 800 ; i++){
//       digitalWrite(STEP, HIGH);
//       delay(1000);
//       digitalWrite(STEP, LOW);
//       delay(1000);
//     }
//   }
// }


// void wall_deactivate() {
//   if (pos == 180) {
//     Serial.println("Already Deactivate");
//     return;
//   } else {
//     pos = 90;
//     while (pos != 180) {
//       servo.write(pos);
//       delay(15);
//       pos++;
//     }
//   }
// }

void wall_deactivate() {
  if(activate_state == false) {
    return;
  }
  else {
    Serial.println("Wall Deactivate");

    stepper.setSpeed(-50);
    while(stepper.currentPosition() != -400){
      stepper.runSpeed();
    }
    stepper.setSpeed(0);

    activate_state = false;
    wall_deactivate_flag = false;
  }
}

// void wall_deactivate() {
  
//   if (activate_state == false){
//     return;
//   }
//   else{
//     activate_state = false;

//     digitalWrite(DIR, LOW);
//     for(int i = 0 ; i < 800 ; i++){
//       digitalWrite(STEP, HIGH);
//       delayMicroseconds(500);
//       digitalWrite(STEP, LOW);
//       delayMicroseconds(500);
//     }
//   }
// }

void setup() {

  Serial.begin(115200);

  // ESP32PWM::allocateTimer(0);
  // ESP32PWM::allocateTimer(1);
  // ESP32PWM::allocateTimer(2);
  // ESP32PWM::allocateTimer(3);
  // servo.setPeriodHertz(50);
  // servo.attach(servo_pin, 500, 2400);
  
  // pinMode(DIR,OUTPUT);
  // pinMode(STEP,OUTPUT);
  // pinMode(ENABLE, OUTPUT);

  // digitalWrite(ENABLE, LOW);

  stepper.setMaxSpeed(200);
  stepper.setCurrentPosition(0);

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
      wall_activate_flag = true;
    } else if (value == "deactivate") {
      wall_deactivate_flag = true;
    }
  });

  // server.on("/post", HTTP_POST, action); 
  server.onNotFound(notFound);

  server.begin();
}

void loop() {
  if(wall_activate_flag){
    wall_activate();
  }
  if(wall_deactivate_flag){
    wall_deactivate();
  }
  
}

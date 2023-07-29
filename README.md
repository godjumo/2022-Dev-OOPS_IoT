![254303205-2c708d0a-7f16-4501-8fc8-0b3f5333c2f4](https://github.com/HyeokHam/2022-Dev-OOPS_IoT/assets/90409694/571e8304-9cd6-4ffe-9f66-a7351bc67254)

# IoT

차수벽 관리 및 가동에 필요한 각종 서버, 차수벽 작동 프로세스를 구성하고 작동하는 역할을 합니다.

## 기술 스택 및 환경

* Environment : Python 3.10, Arduino, C++
* Framework : Flask 2.3.2
* Hardware : ESP32, ESP32-CAM

## 작동 과정

ESP32는 차수벽 제어 서버를 통해 **중앙 서버**에서 차수벽 가동 명령을 수신 받고, 차수벽을 가동시킵니다.

또한 ESP32-CAM으로 구현한 CCTV는 항상 가동하며, 중앙 서버에서 요청을 받아 CV 서버로 CCTV 스트리밍 영상을 송신합니다.

또한ESP32-CAM은 RTSP 통신을 이용해 CCTV 스트리밍 영상을 실시간으로 전송합니다.

[아키텍쳐]

## 구성

### 중앙 서버

중앙 서버는 Raspberry PI4에서 동작합니다.

중앙 서버는 Flask로 구축했으며, HTTP 통신을 통해 다른 서버와 통신합니다.

서버의 구성요소는 서버를 들어온 데이터를 받는 부분과 차수벽 제어 서버로 명령을 전송하는 부분으로 구성되었습니다.



### 차수벽 제어 서버

차수벽 제어 서버는 ESP32에서 동작합니다.

차수벽 제어 서버는 Arduino로 구축하였으며, 중앙 서버에서 들어온 차수벽 가동 명령을 수행합니다.



### CCTV 서버

CCTV 서버는 ESP32-CAM에서 동작합니다

해당 서버는 PlatformIO를 통한 C++로 구축되어 있으며, rzeldent의 [esp32cam-rtsp](https://github.com/rzeldent/esp32cam-rtsp) 를 사용했습니다.



## 라이센스

rzeldent의 esp32cam-rtsp는 PlatformIO와 Bootstrap5를 사용했으므로

Apache 2.0 라이선스, MIT 라이선스를 적용합니다.


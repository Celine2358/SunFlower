#include <TM1637Display.h>

// TM1637 디스플레이의 CLK와 DIO 핀 설정 (토양 습도)
#define CLK 3
#define DIO 4
#define SoilHumSensor A0 // 토양 습도 센서 A0 핀
#define WaterLevSensor A1 // 수위 센서 A1 

// LED
int RED = 5;
int BLUE = 6;

// 워터 펌프
int WaterPump = 10;

// 토양 습도 값 설정
const int wetValue = 725; // 완전히 습할 때의 값 (보정 필요)
const int dryValue = 0; // 완전히 건조할 때의 값 (보정 필요)
int SunFlowerHum = 30; // 해바라기 토양 습도

TM1637Display display(CLK, DIO); // TM1637 디스플레이 객체 생성

void setup() {
  Serial.begin(9600); // 시리얼 통신 시작
  pinMode(SoilHumSensor, INPUT);
  pinMode(WaterLevSensor, INPUT);
  pinMode(WaterPump, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);

  display.setBrightness(0x0d); // 디스플레이 밝기 설정 (0x00~0x0f)
}

void loop() {
  delay(1000); // 1초 대기

  int SoilHum = analogRead(SoilHumSensor); // 토양 습도 값 읽기
  int SoilPercent = map(SoilHum, dryValue, wetValue, 0, 100); // 토양 습도 센서의 아날로그 값을 0% ~ 100%로 환산
  SoilPercent = constrain(SoilPercent, 0, 100); // 변환된 토양 습도 값을 0% ~ 100% 사이로 제한

  int WaterLev = analogRead(WaterLevSensor);

  /*
  Serial.print("토양 습도 : ");
  Serial.print(SoilPercent);
  Serial.println("%");
  */
  Serial.print("Water Level : ");
  Serial.println(WaterLev);

  // 물이 감지됨에 따라 LED 색 바뀜
  if (WaterLev > 0) {
    digitalWrite(BLUE, HIGH);
    digitalWrite(RED, LOW);
  } else {
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, LOW);
  }

  // 토양 습도에 따라 물 공급
  if (SoilPercent < SunFlowerHum) {
    digitalWrite(WaterPump, HIGH);
  } else if (SoilPercent > SunFlowerHum) {
    digitalWrite(WaterPump, LOW);
  }

  display.showNumberDec(SoilPercent); // TM1637 디스플레이에 토양 습도 값 출력
}

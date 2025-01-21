#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>

// LCD 설정 (I2C 주소는 보통 0x27 또는 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16x2 LCD 크기

// RFID 핀 설정
#define SS_PIN 10 // RFID 리더의 SS 핀
#define RST_PIN 9 // RFID 리더의 RST 핀
MFRC522 mfrc522(SS_PIN, RST_PIN); // MFRC522 객체 생성

// 메시지 배열 정의
const char* messages[] = {
  "Wake up",
  "This is real!",
  "Where are you?",
  "Don't look back",
  "I see you",
  "Who are you?"
};

const int numMessages = sizeof(messages) / sizeof(messages[0]);

// 마지막 카드 인식 시간 기록
unsigned long lastCardTime = 0;
const unsigned long debounceDelay = 2000; // 카드 인식 후 2초 동안 재인식 방지

void setup() {
  Serial.begin(9600);  // 시리얼 통신 시작
  SPI.begin();         // SPI 버스 시작
  mfrc522.PCD_Init();  // RFID 리더 초기화

  lcd.begin(16, 2);         // LCD 초기화 (16x2 크기 지정)
  lcd.backlight();         // LCD 백라이트 켜기
  lcd.setCursor(0, 0);     // 첫 번째 줄 첫 번째 칸
  lcd.print("Dream Message:"); // 초기 메시지
  delay(2000);             // 2초 대기
  lcd.clear();
}

void loop() {
  // RFID 카드가 감지되면
  if (mfrc522.PICC_IsNewCardPresent()) {
    // 카드 번호를 읽기
    if (mfrc522.PICC_ReadCardSerial()) {
      // 현재 시간과 마지막 카드 인식 시간 비교
      if (millis() - lastCardTime > debounceDelay) {
        int randomIndex = random(0, numMessages); // 랜덤 메시지 선택

        // 이전 메시지를 지우기 위해 공백으로 덮어쓰기
        lcd.setCursor(0, 0); 
        lcd.print("                "); // 첫 번째 줄 공백으로 초기화
        lcd.setCursor(0, 1); 
        lcd.print("                "); // 두 번째 줄 공백으로 초기화

        // 새 메시지 출력
        lcd.setCursor(0, 0); // 첫 번째 줄
        lcd.print("Dream Message: ");
        lcd.setCursor(0, 1); // 두 번째 줄
        lcd.print(messages[randomIndex]); // 랜덤 메시지 출력

        Serial.println("Card detected!");
        lastCardTime = millis(); // 마지막 카드 인식 시간 업데이트
      }
    }
  }

  delay(100); // 카드 감지 대기
}

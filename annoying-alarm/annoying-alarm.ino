#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <WiFi.h>
#include <time.h>
#include <TimeLib.h>

#define TFT_SCLK 9
#define TFT_MOSI 10
#define TFT_RST 8
#define TFT_DC 4
#define TFT_CS 5
#define TFT_BL 6

#define KEYONE 3
#define KEYTWO 2
#define KEYTHREE 1
#define KEYFOUR 0
#define BUZZER 7

const char* ssid     = "Sarvaani"; // Replace with your own Wifi name
const char* password = "Master9494194535"; // Replace with your own Wifi password (Shhh don't look at mine)

const char* insults[] = {
  "Wake up, ya lazy lug!",
  "Kid, a rock is more awake than you rn.",
  "If this is your prime, you're cooked.",
  "You're dumber than you look, and that's saying something.",
  "Your alarm is judging you.",
  "Rise and whine, Avatar the Last Chairbender",
  "Get up and do your homework like a man!",
  "Dude, your RSM teacher's gonna kill you.",
  "I give up. You're just an idiot. I'm sorry.",
  "Practice your flute, your alarm sounds better than your flute does!",
};
const int totalInsults = 6;

class MyST7789 : public Adafruit_ST7789 {
public:
  MyST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst)
    : Adafruit_ST7789(cs, dc, mosi, sclk, rst) {}
  void setOffsets(uint8_t col, uint8_t row) {
    _colstart = _colstart2 = col;
    _rowstart = _rowstart2 = row;
  }
};

MyST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

bool alarmTriggered = false;

String formatTwoDigits(int number) {
  if (number < 10) return "0" + String(number);
  return String(number);
}

void playSiren() {
  for (int hz = 400; hz < 1200; hz += 15) {
    tone(BUZZER, hz);
    delay(4);
  }
  for (int hz = 1200; hz > 400; hz -= 15) {
    tone(BUZZER, hz);
    delay(4);
  }
}

void displayInsult() {
  int index = random(0, totalInsults);
  tft.fillRect(0, 65, 284, 11, ST77XX_BLACK);
  tft.setCursor(10, 65);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print(insults[index]);
  Serial.println(insults[index]);
}

void setup() {
  Serial.begin(115200);
  pinMode(TFT_BL, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(KEYONE, INPUT_PULLUP);
  pinMode(KEYTWO, INPUT_PULLUP);
  
  digitalWrite(TFT_BL, LOW); 
  noTone(BUZZER);
  
  tft.init(76, 284);
  tft.setOffsets(82, 18);
  tft.invertDisplay(false);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  
  tft.setCursor(10, 20);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Connecting WiFi...");
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 20);
  tft.print("Syncing Time...");
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
  }
  
  randomSeed(analogRead(A0) + millis());
  
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(20, 10);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("ALARM SET: 06:00:00");
}

void loop() {
  if (digitalRead(KEYTWO) == LOW) {
    displayInsult();
    delay(200);
  }

  String timeString = formatTwoDigits(hour()) + ":" + 
                      formatTwoDigits(minute()) + ":" + 
                      formatTwoDigits(second());
  
  tft.setCursor(20, 25);
  tft.setTextSize(4);
  
  if (alarmTriggered) {
    tft.setTextColor(ST77XX_BLACK, ST77XX_RED);
  } else {
    tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
  }
  tft.print(timeString);

  if (timeString == "06:00:00") {
    alarmTriggered = true;
    displayInsult();
  }

  if (alarmTriggered) {
    unsigned long startTime = millis();
    while (millis() - startTime < 5000) {
      if (digitalRead(KEYTWO) == LOW) {
        displayInsult();
        delay(200);
      }
      playSiren();
      if (digitalRead(KEYONE) == LOW) {
        alarmTriggered = false;
        noTone(BUZZER);
        tft.fillRect(0, 65, 284, 11, ST77XX_BLACK);
        break;
      }
    }
    noTone(BUZZER);
    if (alarmTriggered) {
      delay(1000);
    }
  } else {
    delay(100);
  }
}



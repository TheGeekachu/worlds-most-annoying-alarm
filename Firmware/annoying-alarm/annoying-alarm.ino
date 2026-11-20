#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <WiFi.h>
#include <time.h>

#define TFT_SCLK D9
#define TFT_MOSI D10
#define TFT_RST  D8
#define TFT_DC   D4
#define TFT_CS   D5
#define TFT_BL   D6

#define KEYONE   D3
#define KEYTWO   D2
#define KEYTHREE D1
#define BUZZER   D7 

const char* ssid     = "Sarvaani";
const char* password = "Master9494194535";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -28800;
const int   daylightOffset_sec = 3600;

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
  "Practice your flute, your alarm sounds better than your flute does!"
};
const int totalInsults = 10;

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
int lastAlarmDay = -1;

void triggerBuzzer(bool state) {
  digitalWrite(BUZZER, state ? HIGH : LOW);
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
  delay(500);

  pinMode(TFT_BL, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(KEYONE, INPUT_PULLUP);
  pinMode(KEYTWO, INPUT_PULLUP);
  pinMode(KEYTHREE, INPUT_PULLUP);
  
  digitalWrite(TFT_BL, LOW);
  triggerBuzzer(false);

  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  tft.init(76, 284);
  tft.setOffsets(82, 18);
  tft.setRotation(1);
  tft.invertDisplay(false);
  tft.fillScreen(ST77XX_BLACK);

  // Header Title
  tft.setCursor(5, 5);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print("World's Most Annoying Alarm Clock");

  tft.setCursor(5, 20);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Connecting WiFi...");

  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 15) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  tft.fillRect(5, 20, 250, 15, ST77XX_BLACK);
  tft.setCursor(5, 20);
  tft.print("Syncing NTP Time...");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  int retry = 0;
  while (!getLocalTime(&timeinfo) && retry < 10) {
    delay(500);
    retry++;
  }

  randomSeed(analogRead(A0) + millis());

  tft.fillRect(0, 20, 284, 40, ST77XX_BLACK);
}

void loop() {
  if (digitalRead(KEYTWO) == LOW) {
    displayInsult();
    delay(250);
  }
  if (digitalRead(KEYTHREE) == LOW) {
    Serial.println("Key pressed...");
    triggerBuzzer(true);
    delay(150);
    triggerBuzzer(false);
    delay(150);
  }

  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    char timeBuffer[9];
    sprintf(timeBuffer, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    
    tft.setCursor(20, 25);
    tft.setTextSize(3);
    
    if (alarmTriggered) {
      tft.setTextColor(ST77XX_BLACK, ST77XX_RED);
    } else {
      tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
    }
    tft.print(timeBuffer);

    if (timeinfo.tm_hour == 6 && timeinfo.tm_min == 0 && timeinfo.tm_sec == 0 && lastAlarmDay != timeinfo.tm_mday) {
      alarmTriggered = true;
      lastAlarmDay = timeinfo.tm_mday;
      displayInsult();
    }

    if (alarmTriggered) {
      unsigned long startTime = millis();
      while (millis() - startTime < 5000) {
        if (digitalRead(KEYTWO) == LOW) {
          displayInsult();
          delay(250);
        }
        
        triggerBuzzer(true);
        delay(150);
        triggerBuzzer(false);
        delay(150);
        
        if (digitalRead(KEYONE) == LOW) {
          alarmTriggered = false;
          triggerBuzzer(false);
          tft.fillRect(0, 65, 284, 11, ST77XX_BLACK);
          break;
        }
      }
      triggerBuzzer(false);
      if (alarmTriggered) {
        delay(1000);
      }
    }
  }
  
  delay(100);
}
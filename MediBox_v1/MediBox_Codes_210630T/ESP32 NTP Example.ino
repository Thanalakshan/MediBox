#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <DHTesp.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET_DST 0
#define BUZZER_PIN 15
#define LED_1 2
#define PB_CL 0
#define PB_OK 27
#define PB_UP 12
#define PB_DW 14
#define DHTPIN 13
#define HIGH_Temp 26
#define LOW_Temp 32
#define HIGH_Humi 33
#define LOW_Humi 25

// Credits to the creator
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

int tempo = 120; // Speedup and Slowdown the melody
int melody[] = {
  NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, //1
  NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16,
  NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16,
  NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16,
  NOTE_G4,-4, NOTE_C4,-4,//5

  NOTE_DS4,16, NOTE_F4,16, NOTE_G4,4, NOTE_C4,4, NOTE_DS4,16, NOTE_F4,16, //6
  NOTE_D4,-1, //7 and 8
  NOTE_F4,-4, NOTE_AS3,-4,
  NOTE_DS4,16, NOTE_D4,16, NOTE_F4,4, NOTE_AS3,-4,
  NOTE_DS4,16, NOTE_D4,16, NOTE_C4,-1, //11 and 12

  //repeats from 5
  NOTE_G4,-4, NOTE_C4,-4,//5

  NOTE_DS4,16, NOTE_F4,16, NOTE_G4,4, NOTE_C4,4, NOTE_DS4,16, NOTE_F4,16, //6
  NOTE_D4,-1, //7 and 8
  NOTE_F4,-4, NOTE_AS3,-4,
  NOTE_DS4,16, NOTE_D4,16, NOTE_F4,4, NOTE_AS3,-4,
  NOTE_DS4,16, NOTE_D4,16, NOTE_C4,-1, //11 and 12
  NOTE_G4,-4, NOTE_C4,-4,
  NOTE_DS4,16, NOTE_F4,16, NOTE_G4,4,  NOTE_C4,4, NOTE_DS4,16, NOTE_F4,16,

  NOTE_D4,-2,//15
  NOTE_F4,-4, NOTE_AS3,-4,
  NOTE_D4,-8, NOTE_DS4,-8, NOTE_D4,-8, NOTE_AS3,-8,
  NOTE_C4,-1,
  NOTE_C5,-2,
  NOTE_AS4,-2,
  NOTE_C4,-2,
  NOTE_G4,-2,
  NOTE_DS4,-2,
  NOTE_DS4,-4, NOTE_F4,-4, 
  NOTE_G4,-1,
  
  NOTE_C5,-2,//28
  NOTE_AS4,-2,
  NOTE_C4,-2,
  NOTE_G4,-2, 
  NOTE_DS4,-2,
  NOTE_DS4,-4, NOTE_D4,-4,
  NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16, NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16,
  NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16, NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16,
  
  REST,4, NOTE_GS5,16, NOTE_AS5,16, NOTE_C6,8, NOTE_G5,8, NOTE_GS5,16, NOTE_AS5,16,
  NOTE_C6,8, NOTE_G5,16, NOTE_GS5,16, NOTE_AS5,16, NOTE_C6,8, NOTE_G5,8, NOTE_GS5,16, NOTE_AS5,16,  
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;

int alarm_hours[] = {24,24,24};
int alarm_mins[] = {59,59,59};
int time_now[]={0,0};
int temp_time[]={0,0};
int set_temp[] = {5,30}; // To make sure +5.30 UTC as default Setting time for Sri Lanka
bool alarm_trigerred[] = {false,false,false};
bool alarm_enabled[] = {false,false,false};
bool breaked = true;
bool pressed = false;
bool ignored = false;
int menuIndex = 0;
int UTC_OFFSET = 0;
String menuList[] = {"Set Time","Set Alarm : 1","Set Alarm : 2","Set Alarm : 3","Disable   All Alarms"};

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    display.setCursor(0, 1);
    return;
  }

  display.clearDisplay();
  time_now[0] = timeinfo.tm_hour;
  time_now[1] = timeinfo.tm_min;
  temp_time[0] = timeinfo.tm_hour;
  temp_time[1] = timeinfo.tm_min;
  display.setCursor(16, 0);
  display.println(&timeinfo, "%H:%M:%S"); // Print time
  display.setCursor(6, 25);
  display.println(&timeinfo, "%d/%m/%Y"); // Print date
  display.setCursor(45, 50);
  display.println("IST");                 // Print timezone
  display.display(); // Display buffer
}

void buzzer(int ringing) {
  display.clearDisplay();
  display.setCursor(22, 0);
  display.println("Time to");
  display.setCursor(40, 25);
  display.println("take");
  display.setCursor(16, 50);
  display.println("MEDICINE");
  display.display();

  digitalWrite(LED_1, HIGH);    // LED warning

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }
    tone(BUZZER_PIN, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZER_PIN);

    if (digitalRead(PB_CL) == LOW){
      alarm_trigerred[ringing] = true;
      breaked = true;
      alarm_enabled[ringing] = false; // Alarm will not repeat for next day
      break;
    }
  }
  digitalWrite(LED_1, LOW);
  delay(1000);
}

void checkAlarm(){
  for (int i=0; i<3; i++){
    if (!alarm_trigerred[i] && (alarm_hours[i]*60 + alarm_mins[i] <= time_now[0]*60 + time_now[1]) && alarm_enabled[i]){
      breaked = false;
      buzzer(i);
    }
  }
}

void menuPrint(int menuIndex){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(menuList[menuIndex]);
  display.display();
}

void setTime(){
  while (true){
    delay(150);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("OFFSET hr : "+String(set_temp[0]));
    display.display();
    if (digitalRead(PB_CL) == LOW){
      break;
    }
    if (digitalRead(PB_UP) == LOW){
      set_temp[0] += 1;
      if (set_temp[0] == 12){
        set_temp[0] = -11;
      }
    }
    if (digitalRead(PB_DW) == LOW){
      set_temp[0] -= 1;
      if (set_temp[0] == -12){
        set_temp[0] = 11;
      }
    }
    if (digitalRead(PB_OK) == LOW){
      while (true){
        delay(150);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("OFFSET min : "+String(set_temp[1]));
        display.display();
        if (digitalRead(PB_CL) == LOW){
          break;
        }
        if (digitalRead(PB_UP) == LOW){
          set_temp[1] += 1;
          if (set_temp[1] == 60){
            set_temp[1] = 0;
          }
        }
        if (digitalRead(PB_DW) == LOW){
          set_temp[1] -= 1;
          if (set_temp[1] == -1){
            set_temp[1] = 59;
          }
        }
        if (digitalRead(PB_OK) == LOW){
          UTC_OFFSET = 3600*set_temp[0]+60*set_temp[1];
          display.clearDisplay();
          display.setCursor(0, 0);
          display.println("Restarting");
          display.display();
          delay(500);
          setup();
          break;
        }
      }
      break;
    }
  }
}

void setAlarm(int menuIndex){
  while (true){
    delay(150);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Alarm hr  : "+String(temp_time[0]));
    display.display();
    if (digitalRead(PB_CL) == LOW){
      break;
    }
    if (digitalRead(PB_UP) == LOW){
      temp_time[0] += 1;
      if (temp_time[0] == 24){
        temp_time[0] = 0;
      }
    }
    if (digitalRead(PB_DW) == LOW){
      temp_time[0] -= 1;
      if (temp_time[0] == -1){
        temp_time[0] = 23;
      }
    }
    if (digitalRead(PB_OK) == LOW){
      while (true){
        delay(150);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Alarm min : "+String(temp_time[1]));
        display.display();
        if (digitalRead(PB_CL) == LOW){
          break;
        }
        if (digitalRead(PB_UP) == LOW){
          temp_time[1] += 1;
          if (temp_time[1] == 60){
            temp_time[1] = 0;
          }
        }
        if (digitalRead(PB_DW) == LOW){
          temp_time[1] -= 1;
          if (temp_time[1] == -1){
            temp_time[1] = 59;
          }
        }
        if (digitalRead(PB_OK) == LOW){
          alarm_hours[menuIndex-1] = temp_time[0];
          alarm_mins[menuIndex-1] = temp_time[1];
          display.clearDisplay();
          display.setCursor(0, 0);
          display.println("Alarm set");
          display.display();
          alarm_trigerred[menuIndex-1] = false;
          if (alarm_hours[menuIndex-1]*60 + alarm_mins[menuIndex-1] <= time_now[0]*60 + time_now[1]){
            alarm_enabled[menuIndex-1] = false;
          }
          else if (alarm_hours[menuIndex-1] != 24){
              alarm_enabled[menuIndex-1] = true;
          }
          delay(500);
          break;
        }
      }
      break;
    }
  }
}

void menu(){
  delay(20);
  if (digitalRead(PB_OK) == LOW){
    menuPrint(menuIndex);
    while (true){
      delay(125);
      if (digitalRead(PB_UP) == LOW){
        menuIndex += 1;
        if (menuIndex==5){menuIndex=0;}
        menuPrint(menuIndex);
      }
      if (digitalRead(PB_DW) == LOW){
        menuIndex -= 1;
        if (menuIndex==-1){menuIndex=4;}
        menuPrint(menuIndex);
      }
      if (digitalRead(PB_CL) == LOW){
        menuIndex = 0;
        break;
      }
      if (digitalRead(PB_OK) == LOW){
        if (menuIndex == 0){
          setTime();
          break;
          }
        if (menuIndex == 1 || menuIndex == 2 ||menuIndex == 3){
          setAlarm(menuIndex);
          break;
        }
        if (menuIndex == 4){
          alarm_trigerred[0] = true;
          alarm_trigerred[1] = true;
          alarm_trigerred[2] = true;
          display.clearDisplay();
          display.setCursor(0, 0);
          display.println("No More   Alarms");
          display.display();
          delay(500);
          break;
        }
      }
    }
  }
}

void alarmSchedule(){
  // This function will schedule the alarm for next day
  if (time_now[0]==0 && time_now[1]<2){
    for (int i=0; i<3; i++){
      alarm_enabled[i] = true;
    }
  }
}

void DHTCheck(){
  while(true){
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    breaked = true;
    if (data.temperature > 32){
      digitalWrite(HIGH_Temp,HIGH);
    }
    else {
      digitalWrite(HIGH_Temp,LOW);
    }
    if (data.temperature < 26){
      digitalWrite(LOW_Temp, HIGH);
    }
    else {
      digitalWrite(LOW_Temp,LOW);
    }
    if (data.humidity > 80){
      digitalWrite(HIGH_Humi, HIGH);
    }
    else {
      digitalWrite(HIGH_Humi,LOW);
    }
    if (data.humidity < 60){
      digitalWrite(LOW_Humi, HIGH);
    }
    else {
      digitalWrite(LOW_Humi,LOW);
    }
    if (!ignored) {
      if (data.temperature > 32 || data.temperature < 26 || data.humidity > 80 || data.humidity < 60){
        display.clearDisplay();
        display.setCursor(0, 0);
        breaked = false;
        if (data.temperature > 32){
          display.println("Temp :    High");
        }
        if (data.temperature < 26){
          display.println("Temp :    Low");
        }
        if (data.humidity > 80){
          display.println("Humidity :High");
        }
        if (data.humidity < 60){
          display.println("Humidity :Low");
        }
      }
    }
    display.display();
    if (digitalRead(PB_CL) == LOW){
      breaked = true;
      ignored = true;
    }
    if (breaked){
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);

  // OLED Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(25, 0);
  display.println("WELCOME");
  display.setCursor(55, 25);
  display.println("TO");
  display.setCursor(25, 50);
  display.println("MEDIBOX");
  display.display();

  // WiFi setup
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  // LED
  pinMode(LED_1, OUTPUT);
  // Push Buttons
  pinMode(PB_CL, INPUT);
  pinMode(PB_OK, INPUT);
  pinMode(PB_UP, INPUT);
  pinMode(PB_DW, INPUT);
  pinMode(HIGH_Temp, OUTPUT);
  pinMode(LOW_Temp, OUTPUT);
  pinMode(HIGH_Humi, OUTPUT);
  pinMode(LOW_Humi, OUTPUT);
  dhtSensor.setup(DHTPIN,DHTesp::DHT22);
}

void loop() {
// If you wanna ignore the temperature & humidity warnings, 'PRESS CANCEL'
// Otherwice you cant't use MediBox
  DHTCheck();
  delay(100); 
  if (breaked){   // Disconnect time updates during alarm ringing
    printLocalTime();
  }
  checkAlarm();
  if (breaked){   // Disallow menu access during alarm ringing
    menu();
    alarmSchedule();
  }
}
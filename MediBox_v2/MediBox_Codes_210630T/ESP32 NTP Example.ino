#include <WiFi.h>
#include <DHTesp.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP32Servo.h>

DHTesp dhtSensor;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
Servo servo;

#define BUZZER_PIN 15
#define DHTPIN 13
#define LDR1 35
#define LDR2 34
#define SERVO_PIN 2

bool alarmState;
int scheduleTime;
int timeNow;
double LDRIn;
double D_val = 30;
double r_val = 0.75;
int theta_offset;
bool A_selected = false;
bool B_selected = false;
bool C_selected = false;
bool D_selected = false;

void getTime() {
  timeClient.update();
  timeNow = timeClient.getEpochTime();
}

void buzzer(bool buzzerState) {
  if (buzzerState){
    tone(BUZZER_PIN, 1000);
  } else {
    noTone(BUZZER_PIN);
  }
}

void DHTCheck(){
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  if (data.temperature > 32 && data.humidity > 80){
    mqttClient.publish("DHT_State_210630T","High Temperature and High Humidity");
  } else if (data.temperature > 32 && data.humidity < 60){
    mqttClient.publish("DHT_State_210630T","High Temperature and Low Humidity");
  } else if (data.temperature < 26 && data.humidity < 60){
    mqttClient.publish("DHT_State_210630T","Low Temperature and Low Humidity");
  } else if (data.temperature < 26 && data.humidity > 80){
    mqttClient.publish("DHT_State_210630T","Low Temperature and High Humidity");
  } else if (data.temperature > 32){
    mqttClient.publish("DHT_State_210630T","High Temperature");
  } else if (data.temperature < 26){
    mqttClient.publish("DHT_State_210630T","Low Temperature");
  } else if (data.humidity < 60){
    mqttClient.publish("DHT_State_210630T","Low Humidity");
  } else if (data.humidity > 80){
    mqttClient.publish("DHT_State_210630T","High Humidity");
  } else {
    mqttClient.publish("DHT_State_210630T","Perfect Temperature and Humidity");
  } 
}

void setupWiFi(){
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }
  Serial.println("WiFi Connected");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
}

void setupMQTT(){
  mqttClient.setServer("test.mosquitto.org",1883);
  mqttClient.setCallback(receiveCallback);
  if (!mqttClient.connected()){
    while (!mqttClient.connected()){
      Serial.println("Attempting MQTT connection...");
      if (mqttClient.connect("ESP32-123456789")){
        break;
      }else{
        Serial.print("MQTT connection failed");
        Serial.println(mqttClient.state());
        delay(5000);
      }
    }
  }
  Serial.println("MQTT Client connected successfully.");
  mqttClient.subscribe("Buzzer_State_210630T");
  mqttClient.subscribe("Schedule_210630T");
  mqttClient.subscribe("Minimum_Angle_210630T");
  mqttClient.subscribe("Controlling_Factor_210630T");
}

void LDRInput(){
  LDRIn = ((4096-min(analogRead(LDR1),analogRead(LDR2)))/40.96)/100;
  if (analogRead(LDR1) >= analogRead(LDR2)) {
    D_val = 0.5;
    mqttClient.publish("LDR_Side_210630T","Highest Light Intensity from Right LDR");
  } else {
    D_val = 1.5;
    mqttClient.publish("LDR_Side_210630T","Highest Light Intensity from Left LDR");
  }

  String str = String(LDRIn);
  char buffer[10];
  str.toCharArray(buffer, sizeof(buffer));
  const char* LDR_IN = buffer;
  mqttClient.publish("LDR_Value_210630T",LDR_IN);
}

void scheduleAlarm(int scheduleTime){
  if (alarmState && (timeNow >= scheduleTime)){
    buzzer(true);
  } else {
    buzzer(false);
  }
}

void receiveCallback(char* topic, byte* payload, unsigned int length) {
  char payloadCharAr[length];
  for (int i = 0; i < length; i++) {
    payloadCharAr[i] = (char)payload [i];
  }
  if (strcmp(topic, "Buzzer_State_210630T") == 0) {
    if (((char)payload[0] == '1')){
      alarmState = false;
      buzzer(false);
    }
  } else if (strcmp(topic, "Schedule_210630T") == 0) {
    if (payloadCharAr[0] == 'N') {
      alarmState = false;
    } else {
      alarmState = true;
      scheduleTime = atoi(payloadCharAr);
    }
  } else if (strcmp(topic, "Minimum_Angle_210630T") == 0) {
    if (payloadCharAr[1] == 0 || payloadCharAr[1] == 5){
      theta_offset = payloadCharAr[0];
    } else {
      theta_offset = atoi(payloadCharAr);
    }
  } else if (strcmp(topic, "Controlling_Factor_210630T") == 0) {
    r_val = atof(payloadCharAr);
  }
}

void setup() {
  Serial.begin(115200);

  // WiFi setup
  setupWiFi();
  // MQTT setup
  setupMQTT();
  // Time CLient setup
  timeClient.begin();
  timeClient.setTimeOffset(5.5*3600);
  // DHT Sensor setup
  dhtSensor.setup(DHTPIN,DHTesp::DHT22);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LDR1, INPUT);
  pinMode(LDR2, INPUT);
  servo.attach(SERVO_PIN);
}

void loop() {
  DHTCheck();
  getTime();
  mqttClient.loop();
  LDRInput();
  scheduleAlarm(scheduleTime);
  servo.write(min(180.0, theta_offset*D_val + (180-theta_offset)*LDRIn*r_val));
  delay(100);
}
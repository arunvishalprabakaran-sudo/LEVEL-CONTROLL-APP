#include <WiFi.h> 
#include <Firebase_ESP_Client.h> 
#include <ThingSpeak.h> 
 
/* -------- WiFi -------- */ 
const char* ssid = "MyESP32"; 
const char* password = "12345678"; 
 
/* -------- ThingSpeak -------- */ 
WiFiClient client; 
unsigned long myChannelNumber = 3150868; 
const char* myWriteAPIKey = "S72I63Y8XIPYKUN5"; 
 
10 
 
/* -------- Firebase -------- */ 
#define API_KEY       "AIzaSyCq1J11i1Y033_c0u6llqct7N5oXSXdjV8" 
#define DATABASE_URL  "https://iot-level-control-default
rtdb.firebaseio.com/" 
 
FirebaseData fbdo; 
FirebaseAuth auth; 
FirebaseConfig config; 
 
String userEmail    = "arunvishalprabakaran@gmail.com"; 
String userPassword = "12345678"; 
 
/* -------- Firebase paths -------- */ 
#define PATH_LOW_TH        "/tank/lowThreshold" 
#define PATH_HIGH_TH       "/tank/highThreshold" 
#define PATH_MANUAL_CMD    "/tank/manualPumpCmd" 
#define PATH_LEVEL         "/tank/level" 
#define PATH_PUMP_STATUS   "/tank/pumpStatus" 
 
/* -------- Hardware pins -------- */ 
#define TRIG 4 
#define ECHO 2 
#define PUMP_PIN 5 
 
/* -------- Variables -------- */ 
int level = 0; 
int lowTh = 5; 
int highTh = 95; 
11 
 
int manualPumpCmd = 0; 
 
bool pump = 0; 
 
unsigned long lastUpdate = 0; 
 
/* ---------------------------------------------------- 
   ULTRASONIC READ → LEVEL % ---------------------------------------------------- */ 
int readLevel() { 
 
  long duration; 
  float distanceCM; 
 
  digitalWrite(TRIG, LOW); 
  delayMicroseconds(2); 
  digitalWrite(TRIG, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(TRIG, LOW); 
 
  duration = pulseIn(ECHO, HIGH, 30000); 
 
  if (duration == 0) return level;  // keep previous valid reading 
 
  distanceCM = duration * 0.0343 / 2.0; 
 
  float tankHeight = 50.0; 
12 
 
  float waterCM = tankHeight - distanceCM; 
 
  if (waterCM < 0) waterCM = 0; 
  if (waterCM > tankHeight) waterCM = tankHeight; 
 
  return (waterCM / tankHeight) * 100.0; 
} 
 
/* ---------------------------------------------------- 
   APPLY CONTROL LOGIC (OPTION A) ---------------------------------------------------- */ 
void applyLogic() { 
 
  // HIGH THRESHOLD → Pump OFF 
  if (level >= highTh) { 
    pump = 0; 
    Serial.println("AUTO: HIGH LEVEL → PUMP OFF"); 
  } 
 
  // LOW THRESHOLD → Pump ON 
  else if (level <= lowTh) { 
    pump = 1; 
    Serial.println("AUTO: LOW LEVEL → PUMP ON"); 
  } 
 
  // Middle Range → Follow manualPumpCmd 
  else { 
13 
 
    pump = manualPumpCmd; 
    Serial.println("MID RANGE → Following manualPumpCmd"); 
  } 
 
  digitalWrite(PUMP_PIN, pump); 
 
  Firebase.RTDB.setBool(&fbdo, PATH_PUMP_STATUS, pump); 
} 
 
/* ---------------------------------------------------- 
   SEND LEVEL TO THINGSPEAK ---------------------------------------------------- */ 
void pushToThingSpeak() { 
  ThingSpeak.setField(1, level); 
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); 
} 
 
/* ---------------------------------------------------- 
   SETUP ---------------------------------------------------- */ 
void setup() { 
 
  Serial.begin(115200); 
 
  pinMode(TRIG, OUTPUT); 
  pinMode(ECHO, INPUT); 
  pinMode(PUMP_PIN, OUTPUT); 
14 
 
 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print("."); 
    delay(250); 
  } 
  Serial.println("\nWiFi Connected!"); 
 
  ThingSpeak.begin(client); 
 
  config.api_key = API_KEY; 
  config.database_url = DATABASE_URL; 
  auth.user.email = userEmail; 
  auth.user.password = userPassword; 
 
  Firebase.begin(&config, &auth); 
  Firebase.reconnectWiFi(true); 
 
  Serial.println("Firebase Ready!"); 
} 
 
/* ---------------------------------------------------- 
   LOOP ---------------------------------------------------- */ 
void loop() { 
 
  if (millis() - lastUpdate > 4000) { 
15 
 
    lastUpdate = millis(); 
 
    // 1) READ Firebase thresholds + manual command 
    if (Firebase.RTDB.getInt(&fbdo, PATH_LOW_TH)) 
      lowTh = fbdo.intData(); 
 
    if (Firebase.RTDB.getInt(&fbdo, PATH_HIGH_TH)) 
      highTh = fbdo.intData(); 
 
    if (Firebase.RTDB.getInt(&fbdo, PATH_MANUAL_CMD)) 
      manualPumpCmd = fbdo.intData(); 
 
    // 2) READ LEVEL 
    level = readLevel(); 
    Serial.printf("LEVEL = %d%%\n", level); 
 
    // 3) APPLY LOGIC 
    applyLogic(); 
 
    // 4) UPDATE Firebase level 
    Firebase.RTDB.setInt(&fbdo, PATH_LEVEL, level); 
 
    // 5) SEND TO THINGSPEAK 
    pushToThingSpeak(); 
  } 
} 

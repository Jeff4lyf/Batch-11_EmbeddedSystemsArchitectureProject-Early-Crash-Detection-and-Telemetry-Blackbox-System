#include <DHT.h>

#define ECHO_PIN     2
#define TRIG_PIN     3
#define BUZZER_PIN   11
#define LED_PIN      12
#define DHT_PIN      8
#define FLAME_PIN    A1
#define IR_1         A2
#define IR_2         A3
#define IR_3         A4
#define IR_4         A5

#define CRASH_DISTANCE_CM 15
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);

unsigned long previousTelemetryMillis = 0;
unsigned long previousDHTMillis       = 0;

const unsigned long telemetryInterval = 1000;
const unsigned long dhtInterval       = 2500;

float currentTemp     = 0.0;
float currentHumidity = 0.0;

void setup() {
  Serial.begin(115200); 
  Serial.println("\n[SYSTEM] Boot Sequence Initiated...");
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  pinMode(ECHO_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(IR_3, INPUT);
  pinMode(IR_4, INPUT);

  dht.begin();
  
  delay(1500); 
  Serial.println("[SYSTEM] Boot Complete. Entering Live Telemetry Loop.");
}

void loop() {
  unsigned long currentMillis = millis();
  
  long distance     = getUltrasonicDistance();
  bool ir1          = (digitalRead(IR_1) == LOW); 
  bool ir2          = (digitalRead(IR_2) == LOW);
  bool ir3          = (digitalRead(IR_3) == LOW);
  bool ir4          = (digitalRead(IR_4) == LOW);
  bool fireDetected = (digitalRead(FLAME_PIN) == LOW); 
  
  bool isCrashProne    = false;
  String warningSource = "SAFE";

  if (distance > 0 && distance <= CRASH_DISTANCE_CM) {
    isCrashProne  = true;
    warningSource = "REAR DIST";
  } 
  else if (ir1 || ir2 || ir3 || ir4) {
    isCrashProne  = true;
    warningSource = "PROXIMITY IR";
  }

  if (fireDetected || isCrashProne) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  if (currentMillis - previousDHTMillis >= dhtInterval) {
    previousDHTMillis = currentMillis;
    
    float tempH = dht.readHumidity();
    float tempT = dht.readTemperature();
    
    if (!isnan(tempH) && !isnan(tempT)) {
      currentHumidity = tempH;
      currentTemp     = tempT;
    }
  }

  if (currentMillis - previousTelemetryMillis >= telemetryInterval) {
    previousTelemetryMillis = currentMillis;
    
    Serial.print("[DATA] ");
    
    if (fireDetected) {
      Serial.print("[!!! FIRE !!!] ");
    } else if (isCrashProne) {
      Serial.print("[ALARM: "); Serial.print(warningSource); Serial.print("] ");
    } else {
      Serial.print("[ALL CLEAR] ");
    }

    Serial.print("Dist: ");
    if (distance == 999) Serial.print("MAX ");
    else { Serial.print(distance); Serial.print("cm "); }
    
    Serial.print("| IR: ");
    Serial.print(ir1

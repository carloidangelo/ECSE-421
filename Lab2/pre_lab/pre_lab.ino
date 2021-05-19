/*
 * Prelab Activity
 */
 
unsigned long currentMillis;        // will store the number of milliseconds passed since the start of program
unsigned long previousMillis_min = 0;        // will store last time minutes was updated
unsigned long previousMillis_sec = 0;        // will store last time second was updated
unsigned long count_min = 0;
unsigned long count_sec = 0;
const long INTERVAL_MIN = 60000;           // interval at which to update the minutes (milliseconds)
const long INTERVAL_SEC = 1000;           // interval at which to update the seconds (milliseconds)
const long MAX_MIN_AND_SEC = 60;           // interval at which to update the minutes (milliseconds)
String value_min, value_sec;

void setup() {
  Serial.begin(9600);
}
void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis_sec >= INTERVAL_SEC){
    previousMillis_sec = currentMillis;
    count_sec++;
    if (currentMillis - previousMillis_min >= INTERVAL_MIN){
      previousMillis_min = currentMillis;
      count_min++;
    }
  }
  
  value_min = String(count_min % MAX_MIN_AND_SEC);
  value_sec = String(count_sec % MAX_MIN_AND_SEC);
  
  if (value_min.toInt() < 10){
    Serial.print('0' + value_min + ':');
  }else{
    Serial.print(value_min + ':');
  }
  if (value_sec.toInt() < 10){
    Serial.println('0' + value_sec);
  }else{
    Serial.println(value_sec);  
  }
}

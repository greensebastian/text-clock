#include <DS3231.h>

#define five 0
#define ten 1
#define quarter 2
#define twenty 3
#define half 4
#define to 5
#define past 6

int pinM[] = {A0, A1, A2, A3, A6, A7, 13}; // 5 10 15 20 30 i över
int pinH[] = {0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12}; // 12 1 2 3 4 5 6 7 8 9 10 11

DS3231 rtc(SDA, SCL);

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  for(int i = 0; i < sizeof(pinH)/sizeof(int); i++){
    pinMode(pinH[i], OUTPUT);
    if(i < sizeof(pinM)/sizeof(int)){
      pinMode(pinM[i], OUTPUT);
    }
  }
  
  rtc.begin();
  //rtc.setDOW(WEDNESDAY);
  //rtc.setTime(18, 40, 0);
  //rtc.setDate(18, 4, 2018);
}

void loop() {
  // put your main code here, to run repeatedly:
   // Send Day-of-Week
   //Serial.println("Test, nåt händer");
   //Serial.print(rtc.getDOWStr());

  Time t = rtc.getTime();
  int m = t.min;
  int s = t.sec + 60*m;
  int h = t.hour;
  //digitalWrite(A0, s%2);
  //digitalWrite(A1, (s/2)%2);

  if(s >= 22.5*60){
    h = (h+1)%12;
  }
  int interval = (s + 150)/12;
  int arr[];
  switch(interval){
    case 0:
      arr = {h, five, past} // 5 över
      break;
    case 1:
      switchTo({h, ten, past}); // 10 över
      break;
    case 2:
      switchTo({h, quarter, past}); // kvart över
      break;
    case 3:
      switchTo({h, twenty, past}); // tjugo över
      break;
    case 4:
      switchTo({h, five, to, half}); // 5 i halv
      break;
    case 5:
      switchTo({h, half}); // halv
      break;
    case 6:
      switchTo({h, five, past, half}); // 5 över halv
      break;
    case 7:
      switchTo({h, twenty, to}); // tjugo i
      break;
    case 8:
      switchTo({h, quarter, to}); // kvart i
      break;
    case 9:
      switchTo({h, ten, to}); // 10 i
      break;
    case 10:
      switchTo({h, five, to}); // 5 i
      break;
    case 11:
      switchTo({h}); // 0
      break;
    default:
      Serial.println("h: " + h + " interval: " + interval);
      break;
  }
  switchTo(arr);
  /*Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  // Send time
  Serial.println(rtc.getTimeStr());
  */
  // Wait before repeating
  delay (100);
}

void switchTo(int on[]){
  
}


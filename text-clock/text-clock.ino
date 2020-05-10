#include <DS3231.h> // https://github.com/NorthernWidget/DS3231
#include <FastLED.h> // https://github.com/FastLED/FastLED
#include <Wire.h>

struct Interval{
  int hour;
  int interval;
};

#define LED_TYPE WS2812
#define LED_PIN 4
#define COLOR_ORDER RGB
#define NUM_LEDS 80
#define BRIGHTNESS 64

#define five 0
#define ten 1
#define quarter 2
#define twenty 3
#define half 4
#define to 5
#define past 6

// Splits total led strip length into word segments
int lengths[] = {7, 1, 2, 5, 1, 3, 1, 3, 5, 4, 1, 4, 1, 3, 1, 3, 3, 3, 3, 1, 4, 3, 4, 3, 4, 4, 3};

// Map minute and hour indices into lenghts index
int minuteIndices[] = {6, 5, 3, 8, 9, 12, 11}; // 5 10 15 20 30 i över
int hourIndices[] = {24, 13, 15, 16, 20, 18, 17, 21, 22, 23, 26, 25}; // 12 1 2 3 4 5 6 7 8 9 10 11

RTClib rtc;
CRGB ledColors[80];

void setup() {
  Wire.begin();
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  // Set up FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(ledColors, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
}

void loop() {
  // Get current interval
  Interval interval = getInterval();

  // Set leds to show that interval
  setLightsToInterval(interval);
  
  // Wait before repeating
  delay (100);
}

Interval getInterval(){
  DateTime t = rtc.now();
  int m = t.minute();
  int s = t.second() + 60*m;
  int h = t.hour();

  // Increase hour indicator if time is more than "20 past"
  // Swedish time reads "to <next hour>" it time is more than 20 minutes past the hour
  if(s >= 22.5*60){
    h = (h+1)%12;
  }

  // Offset time by 2.5 minutes (150s) to round properly to whole 5 minutes
  int interval = (s + 150)/12;

  Interval result;
  result.hour = h;
  result.interval = interval;
  return result;
}

void setLightsToInterval(Interval interval){
  int m = interval.interval;
  int h = interval.hour;
  switch(m){
    case 0:
      setStrip(h, new int[2]{five, past}); // 5 över
      break;
    case 1:
      setStrip(h, new int[2]{ten, past}); // 10 över
      break;
    case 2:
      setStrip(h, new int[2]{quarter, past}); // kvart över
      break;
    case 3:
      setStrip(h, new int[2]{twenty, past}); // tjugo över
      break;
    case 4:
      setStrip(h, new int[3]{five, to, half}); // 5 i halv
      break;
    case 5:
      setStrip(h, new int[1]{half}); // halv
      break;
    case 6:
      setStrip(h, new int[3]{five, past, half}); // 5 över halv
      break;
    case 7:
      setStrip(h, new int[2]{twenty, to}); // tjugo i
      break;
    case 8:
      setStrip(h, new int[2]{quarter, to}); // kvart i
      break;
    case 9:
      setStrip(h, new int[2]{ten, to}); // 10 i
      break;
    case 10:
      setStrip(h, new int[2]{five, to}); // 5 i
      break;
    case 11:
      setStrip(h, new int[0]); // 0
      break;
    default:
      break;
  }
}

void setStrip(int hour, int minutes[]){
  for (int i=0; i<sizeof(ledColors)/sizeof(ledColors[0]); i++){
    if (partOfHour(i, hour) || partOfMinutes(i, minutes)){
      ledColors[i] = CRGB::White;
    }
    else {
      ledColors[i] = CRGB::Black;
    }
  }
  FastLED.show();
}

// Checks whether the led at the specified index is part of the specified hour
bool partOfHour(int index, int hour){
  int wordIndex = hourIndices[hour];
  int wordLength = lengths[wordIndex];

  int startIndex = 0;
  for (int i=0; i<wordIndex; i++){
    startIndex += lengths[i];
  }
  return index >= startIndex && index < startIndex + wordLength;
}

// Checks whether the led at the specified index is part of one of the specified minute markers
bool partOfMinutes(int index, int minutes[]){
  for (int i=0; i<sizeof(minutes)/sizeof(minutes[0]); i++){
    int minute = minutes[i];
    int wordIndex = minuteIndices[minute];
    int wordLength = lengths[wordIndex];
  
    int startIndex = 0;
    for (int j=0; j<wordIndex; j++){
      startIndex += lengths[j];
    }
    if (index >= startIndex && index < startIndex + wordLength){
      return true;
    }
  }
  return false;
}

void setTime(int year, int month, int day, int hour, int minute, int second){
  //TODO
}


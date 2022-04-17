#include <FastLED.h>
#define DATA_PIN    4
#define NUM_LEDS    24
CRGB leds[NUM_LEDS];

int hall_pin = 5;        // Hall sensor pin
int wheel_diameter = 50; // In millimeters
int rotation_incs = 10;  // Sensor 'blips' per rotation
int led_inc = 7;         // Distance (mm) between each LED in your strip


uint32_t tot_rots;
int mark_interval;
int led_len = NUM_LEDS * led_inc;
int mark_list[256];
bool is_mark_list[256];
uint8_t last_mk_p = 0;
uint32_t last_mk;
int wheel_inc = int(PI * wheel_diameter) / rotation_incs;


bool on_state = false;

void setup() {
  Serial.begin(115200);
  int pot_value = analogRead(A0);
  mark_interval = map(pot_value, 0, 1024, 0, 100);
  Serial.println(mark_interval);
  Serial.println(wheel_inc);

  pinMode(hall_pin, INPUT);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(10 );

  FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Red); FastLED.show();
  delay(250);
  fill_solid(leds, NUM_LEDS, CRGB::Blue); FastLED.show();
  delay(250);
  fill_solid(leds, NUM_LEDS, CRGB::Green); FastLED.show();
  delay(250);
  fill_solid(leds, NUM_LEDS, CRGB::Black); FastLED.show();

}

void loop() {
  int pot_value = analogRead(A0);
  int fixed_val = map(pot_value, 0, 1024, 0, 100);
  if (fixed_val != mark_interval) {
    mark_interval = fixed_val;
  }

  if (digitalRead(hall_pin) == 0) {
    if (on_state == false) {
      on_state = true;
      tot_rots += wheel_inc;
      Serial.print("tot rots...");
      Serial.println(tot_rots);
      Serial.print("last mark...");
      Serial.println(last_mk);
      Serial.print("interval...");
      Serial.println(mark_interval);
      incrementMarks();
      if (tot_rots > last_mk + mark_interval) {
        addMark();
        last_mk = tot_rots;
      }
    }
  } else {
    on_state = false;
  }
}

void addMark() {
  is_mark_list[last_mk_p] = true;
  mark_list[last_mk_p] = 0;
  leds[0] = CHSV(35*last_mk_p, 255, 255);
    FastLED.show();
  last_mk_p++;

}
void incrementMarks() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (uint8_t i = 0; i < 255; i++) {
    if (is_mark_list[i]) {
      mark_list[i] += wheel_inc;
      if (mark_list[i] > led_len) {
        is_mark_list[i] = false;
      } else {
        leds[mark_list[i] / led_inc] = CHSV(35*i, 255, 255);
        //leds[mark_list[i] / led_inc] = CRGB::Blue;
        Serial.println(i);

      }
    }
  }
  FastLED.show();
}
//void updateMarks() {
//  for (int i = 0; i < 255; i++) {
//    if (mark_list[i])
//      int the_
//    }
//
//}

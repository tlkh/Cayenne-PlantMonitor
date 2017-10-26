#include <Adafruit_NeoPixel.h>

#include <CayenneMQTTESP8266.h>

int soil_moist_val;

// WiFi network info.
char ssid[] = "SUTD_ILP2";
char wifiPassword[] = "sutd-acce55";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "f24c33e0-b8d3-11e7-a9b2-a5d7f5484bce";
char password[] = "126b25b2788d2f1d5b48d878fce531978650f016";
char clientID[] = "ff854b80-b8d5-11e7-bd7e-3193fab997a8";

unsigned long lastMillis = 0;

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int PIN = 15;
int led_count = 0;
int remap = 0;

int sensorValue = 0;        // value read from the sensor

Adafruit_NeoPixel strip = Adafruit_NeoPixel(18, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin) - 300;

  led_count = map(sensorValue, 800, 0, 0, 18);
  remap = map(sensorValue, 800, 0, 0, 200);

  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print(" led_count = ");
  Serial.print(led_count);
  Serial.print(" remapped = ");
  Serial.println(remap);
  
  for (int i = 0; i < led_count; i++) {
    if (remap < 50) {
      strip.setPixelColor(i, strip.Color(75,0,0));
    } else {
      if (remap > 160) {
      strip.setPixelColor(i, strip.Color(75,0,0));
    } else {
      strip.setPixelColor(i, strip.Color((200-remap)/4,remap/6,0));
    }
    }
    
  }

  for (int i = led_count; i < 18; i++) {
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
  
  strip.show();
  
  Cayenne.loop();

  if (millis() - lastMillis > 50) {
    soil_moist_val = map(sensorValue, 800, 0, 0, 100);
    lastMillis = millis();
    Cayenne.virtualWrite(1, soil_moist_val, "soil_moist", "p");
  }

}

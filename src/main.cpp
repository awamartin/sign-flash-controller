#include <Arduino.h>

#include <ESP32Encoder.h>

ESP32Encoder enc1;
ESP32Encoder enc2;

void setup()
{
  ESP32Encoder::useInternalWeakPullResistors = UP;
  enc1.attachHalfQuad(17, 5);
  enc2.attachHalfQuad(18, 19);

  pinMode(4, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  // pinMode(32, OUTPUT);
  // pinMode(35, OUTPUT);

  ledcSetup(0, 25000, 8);
  ledcAttachPin(32, 0);

  Serial.begin(115200);
}

long pos1 = 0;
long pos2 = 0;

float brightness = 100;
long flash = 0;

unsigned long lastMillis = 0;

void loop()
{

  long newPos1 = enc1.getCount();
  long newPos2 = enc2.getCount();
  long delta1 = pos1 - newPos1;
  long delta2 = pos2 - newPos2;
  /*

    if (newPos1 != pos1)
      Serial.println("pos1" + String((int32_t)enc1.getCount()));
    if (newPos2 != pos2)
      Serial.println("pos2" + String((int32_t)enc2.getCount()));

  */
  pos1 = newPos1;
  pos2 = newPos2;

  brightness += delta1 * 2;
  if (brightness < 0)
  {
    brightness = 0;
  }
  else if (brightness > 254)
  {
    brightness = 254;
  }

  flash -= delta2 * 100;
  if (flash < 0)
  {
    flash = 0;
  }
  else if (flash > 2000)
  {
    flash = 2000;
  }

  Serial.println("brightness" + String((int32_t)brightness));
  Serial.println("flash" + String((int32_t)flash));

  if ((millis() - lastMillis) < flash)
  {
    ledcWrite(0, brightness);
  }
  else if ((millis() - lastMillis) < flash * 2)
  {
    ledcWrite(0, 0);
  }
  else
  {
    lastMillis = millis();
  }

  if (flash == 0)
  {
    ledcWrite(0, brightness);
  }

      delay(10);
}
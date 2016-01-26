#include <Adafruit_NeoPixel.h>
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
#include <avr/power.h>
#endif

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 24;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 2;
//int clockPin = 3;

const bool IS_ROBOT_FOR_COMPETITION = false;

//Number of LEDs, data pin, 800KHz with GRB wiring
Adafruit_NeoPixel strip = Adafruit_NeoPixel(nLEDs, dataPin, NEO_RGB + NEO_KHZ800);


void setup()
{
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();

  pinMode(3, INPUT); //Alliance Color Data
  pinMode(4, INPUT); //Color Switch 1 - Alliance color or green
  pinMode(5, INPUT); //Color Switch 2 - Red or blue
  pinMode(6, INPUT); //Mode Switch 1 - Chase effects or others
  pinMode(7, INPUT); //Mode Switch 2 - Rainbow effects or misc
  pinMode(8, INPUT); //Method Switch 1 - 1st in group or others
  pinMode(9, INPUT); //Method Switch 2 - 2nd in group or 3rd
}

void loop()
{
  if(IS_ROBOT_FOR_COMPETITION)
  {
    if(digitalRead(3) == HIGH) //Colors
    {
      if(digitalRead(4) == HIGH)
      {
        //Alliance color
        if(digitalRead(5) == HIGH)
        {
          Solid(255, 0, 0);
        }
        else
        {
          Solid(0, 0, 255);
        }
      }
      else
      {
        Solid(0, 255, 0);
      }
    }
    if (digitalRead(6) == HIGH) //Chase effects
    {
      if (digitalRead(8) == HIGH)
      {
        NightRider();
      }
      else if (digitalRead(9) == HIGH)
      {
        ColorChase();
      }
      else
      {
        FadeChase();
      }
    }
    else if (digitalRead(7) == HIGH) //Rainbow effects
    {
      if (digitalRead(8) == HIGH)
      {
        RainbowChase();
      }
      else if (digitalRead(9) == HIGH)
      {
        RainbowFade();
      }
      else
      {
        TasteTheRainbow();
      }
    }
    else //Misc effects
    {
      if (digitalRead(8) == HIGH)
      {
        TheaterCrawl();
      }
      else if (digitalRead(9) == HIGH)
      {
        ColorWipe();
      }
      else
      {
        GreenBluePurple();
      }
    }
  }
  else
  {
    //not competition. choose your most favorite effect(s)
    Solid(0, 255, 0);
  }
}

void Solid(byte r, byte g, byte b)
{
  for(int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, strip.Color(g, r, b));
  }
  strip.show();
}

void NightRider()
{
  byte* offColor = getOffColor();
  byte* color = getOnColor();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, getColor(offColor));
  }
  strip.show();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, getColor(color)); // Set new pixel 'on'
    strip.show();
    delay(25);
    for (int j = 0; j <= 2; j++)
    {
      strip.setPixelColor(i - j, colorLerp(color, offColor, 1.0f / 3.0f * (j + 1)));
    }
    strip.show();
  }
  for (int i = strip.numPixels() - 1; i >= 0; i--)
  {
    strip.setPixelColor(i, getColor(color)); // Set new pixel 'on'
    strip.show();
    delay(25);
    for (int j = 0; j <= 2; j++)
    {
      strip.setPixelColor(i + j, colorLerp(color, offColor, 1.0f / 3.0f * (j + 1)));
    }
    strip.show();
  }
}

void ColorChase()
{
  byte* offColor = getOffColor();
  byte* color = getOnColor();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, getColor(offColor));
  }
  strip.show();
  for (int i = 0; i < strip.numPixels() + 2; i++)
  {
    strip.setPixelColor(i, getColor(color)); // Set new pixel 'on'
    strip.show();
    delay(25);
    for (int j = 0; j <= 2; j++)
    {
      strip.setPixelColor(i - j, colorLerp(color, offColor, 1.0f / 3.0f * (j + 1)));
    }
    strip.show();
  }
}

void FadeChase()
{
  byte* offColor = getOffColor();
  byte* color = getOnColor();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, getColor(offColor));
  }
  strip.show();
  for (int j = -strip.numPixels(); j < strip.numPixels(); j++)
  {
    for (int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i + j, colorLerp(color, offColor, (double)i / strip.numPixels() / 2));
    }
    strip.show();
    delay(50 / strip.numPixels());
  }
  for (int j = -strip.numPixels(); j < strip.numPixels(); j++)
  {
    for (int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i + j, colorLerp(offColor, color, (double)i / strip.numPixels() / 2));
    }
    strip.show();
    delay(50 / strip.numPixels());
  }
}

void RainbowChase()
{
  byte* offColor = getOffColor();
  byte* color = getOnColor();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, getColor(offColor));
  }
  strip.show();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, Wheel(i * (384 / strip.numPixels()))); // Set new pixel 'on'
    strip.show();
    delay(25);
    for (int j = 0; j <= 2; j++)
    {
      strip.setPixelColor(i - j, colorLerp(WheelBytes((i - j) * (384 / strip.numPixels())), offColor, 1.0f / 3.0f * (j + 1)));
    }
    strip.show();
  }
}

void RainbowFade()
{
  for (int i = 0; i < 384; i++)
  {
    for (int j = 0; j < strip.numPixels(); j++)
    {
      strip.setPixelColor(j, Wheel(i));
    }
    strip.show();
    delay(1000 / 128.0f);
  }
}

void TasteTheRainbow()
{
  byte* offColor = getOffColor();
  byte* color = getOnColor();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, getColor(offColor));
  }
  strip.show();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, Wheel(i * (384 / strip.numPixels())));
    strip.show();
    delay(100 / (strip.numPixels() / 5));
  }
  for (int j = 0; j < 10; j++)
  {
    for (int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, colorLerp(WheelBytes(i * (384 / strip.numPixels())), offColor, 1.0f / 10.0f * (j + 1)));
    }
    strip.show();
    delay(10 / (strip.numPixels() / 5));
  }
  for (int i = strip.numPixels() - 1; i >= 0; i--)
  {
    strip.setPixelColor(i, Wheel(i * (384 / strip.numPixels())));
    strip.show();
    delay(100 / (strip.numPixels() / 5));
  }
  for (int j = 0; j < 10; j++)
  {
    for (int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, colorLerp(WheelBytes(i * (384 / strip.numPixels())), offColor, 1.0f / 10.0f * (j + 1)));
    }
    strip.show();
    delay(10 / (strip.numPixels() / 5));
  }
}

void TheaterCrawl()
{
  uint32_t offColor = getColor(getOffColor());
  uint32_t color = getColor(getOnColor());
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, offColor);
  }
  strip.show();
  for (int q = 0; q < 3; q++)
  {
    for (int i = 0; i < strip.numPixels(); i += 3)
    {
      strip.setPixelColor(i + q, color);    //turn every third pixel on
    }
    strip.show();
    delay(250);
    for (int i = 0; i < strip.numPixels(); i += 3)
    {
      strip.setPixelColor(i + q, offColor);        //turn every third pixel off
    }
  }
}

void ColorWipe()
{
  uint32_t offColor = getColor(getOffColor());
  uint32_t color = getColor(getOnColor());
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, offColor);
  }
  strip.show();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, color);
    strip.show();
    delay(25);
  }
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, offColor);
    strip.show();
    delay(25);
  }
}

void GreenBluePurple()
{
  byte green[] = {0, 255, 0};
  byte blue[] = {0, 0, 255};
  byte purple[] = {128, 0, 128};
  ColorFade(green, blue);
  ColorFade(blue, purple);
  ColorFade(purple, green);
}

uint32_t getColor(byte* color)
{
  return strip.Color(color[0], color[1], color[2]);
}

byte* getOnColor()
{
  if (IS_ROBOT_FOR_COMPETITION)
  {
    if (digitalRead(5) == HIGH) //red alliance
    {
      byte bytes[] = {255, 0, 0};
      return bytes;
    }
    else
    {
      byte bytes[] = {0, 0, 255};
      return bytes;
    }
  }
  else
  {
    byte bytes[] = {220, 20, 60};
    return bytes;
  }
}

byte* getOffColor()
{
  if (IS_ROBOT_FOR_COMPETITION)
  {
    byte bytes[] = {32, 32, 32};
    return bytes;
  }
  else
  {
    byte bytes[] = {192, 192, 192};
    return bytes;
  }
}

uint32_t colorLerp(byte* start, byte* finish, float t)
{
  byte r = lerp(start[0], finish[0], t);
  byte g = lerp(start[1], finish[1], t);
  byte b = lerp(start[2], finish[2], t);
  return strip.Color(r, g, b);
}

byte lerp(byte start, byte finish, float t)
{
  return (byte)((1 - t) * start + finish * t);
}

void ColorFade(byte* off, byte* on)
{
  for (int i = 0; i < strip.numPixels(); i++) strip.setPixelColor(i, getColor(off));
  for (double q = 0; q <= 1; q += 1.0f / 30.0f)
  {
    for (int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, colorLerp(off, on, q));
    }
    strip.show();
    delay(1000 / 30.0f);
  }
}

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch (WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break;
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break;
    case 2:
      b = 127 - WheelPos % 128;  //blue down
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break;
  }
  return (strip.Color(r, g, b));
}

byte* WheelBytes(uint16_t WheelPos)
{
  byte r, g, b;
  switch (WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break;
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break;
    case 2:
      b = 127 - WheelPos % 128;  //blue down
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break;
  }
  byte bytes[] = {r, g, b};
  return bytes;
}

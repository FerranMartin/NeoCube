#ifndef NeoCube_h
#define NeoCube_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

enum  pattern { NONE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE };
enum  direction { FORWARD, BACKWARD };

class NeoCube : public Adafruit_NeoPixel
{

public:
  NeoCube(uint16_t x, uint16_t y, uint16_t z, uint8_t pin, uint8_t type, void (*callback)());

  // Update the pattern
  void update();

  //-- Patterns
  void rainbowCycle(uint8_t interval, direction dir = FORWARD);
  void theaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD);
  void colorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD);
  void scanner(uint32_t color1, uint8_t interval);
  void fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD);

  //-- Public Helpers --

  // Reverse pattern direction
  void reverse();

  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
  uint32_t wheel(byte WheelPos);

private:
  uint16_t _x, _y, _z; //possar a uint8_t per optimitzar memoria

  pattern  _activePattern;
  direction _currentDirection;

  unsigned long _millisecondsPerInterval;   // milliseconds between updates
  unsigned long _lastUpdate; // last update of position

  uint32_t Color1, Color2;  // What colors are in use

  uint16_t _totalSteps;  // total number of steps in the pattern
  uint16_t _currentStep;  // current step within the pattern

  void (*OnComplete)();  // Callback on completion of pattern


  // Increment the _currentStep and reset at the end
  void increment();
  void incrementForward();
  void incrementBackward();

  void selectPatternUpdate();

  void rainbowCycleUpdate();
  void theaterChaseUpdate();
  void colorWipeUpdate();
  void scannerUpdate();
  void fadeUpdate();

  //-- Private Helpers --

  // Set all pixels to a color (synchronously)
  void colorSet(uint32_t color);

  // Calculate 50% dimmed version of a color (used by ScannerUpdate)
  uint32_t DimColor(uint32_t color);

  // Returns the Red component of a 32-bit color
  uint8_t Red(uint32_t color);

  // Returns the Green component of a 32-bit color
  uint8_t Green(uint32_t color);

  // Returns the Blue component of a 32-bit color
  uint8_t Blue(uint32_t color);
};

#endif

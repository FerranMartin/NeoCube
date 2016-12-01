//------------------------------------------------------------
//------------------------------------------------------------
//------      NeoCube Header      ----------------------------
//------------------------------------------------------------
//------------------------------------------------------------


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



//------------------------------------------------------------
//------------------------------------------------------------
//------      NeoCube Implementation      --------------------
//------------------------------------------------------------
//------------------------------------------------------------

// Constructor - calls base-class constructor to initialize strip
NeoCube::NeoCube(uint16_t x, uint16_t y, uint16_t z, uint8_t pin, uint8_t type, void (*callback)())
:Adafruit_NeoPixel(x*y*z, pin, type),
_x(x), _y(y), _z(z)
{
  OnComplete = callback;
}

// Update the pattern
void NeoCube::update()
{
  boolean needsToUpdate = (millis() - _lastUpdate) > _millisecondsPerInterval;
  if ( needsToUpdate ) {
    _lastUpdate = millis();
    selectPatternUpdate();
    show();
    increment();
  }
}

void NeoCube::selectPatternUpdate()
{
  switch(_activePattern) {
    case RAINBOW_CYCLE:
    rainbowCycleUpdate();
    break;

    case THEATER_CHASE:
    theaterChaseUpdate();
    break;

    case COLOR_WIPE:
    colorWipeUpdate();
    break;

    case SCANNER:
    scannerUpdate();
    break;

    case FADE:
    fadeUpdate();
    break;

    default:
    break;
  }
}

// Increment the _currentStep and reset at the end
void NeoCube::increment()
{
  if (_currentDirection == FORWARD) {
    incrementForward();
  }
  else {
    incrementBackward();
  }
}

void NeoCube::incrementForward()
{
  _currentStep++;
  if (_currentStep >= _totalSteps) {
    _currentStep = 0;
    if (OnComplete != NULL) {
      OnComplete();
    }
  }
}

void NeoCube::incrementBackward()
{
  --_currentStep;
  if (_currentStep <= 0) {
    _currentStep = _totalSteps - 1;
    if (OnComplete != NULL) {
      OnComplete();
    }
  }
}

// Reverse pattern direction
void NeoCube::reverse()
{
  if (_currentDirection == FORWARD) {
    _currentDirection = BACKWARD;
    _currentStep = _totalSteps - 1;
  }
  else {
    _currentDirection = FORWARD;
    _currentStep = 0;
  }
}

// Initialize for a RainbowCycle
void NeoCube::rainbowCycle(uint8_t interval, direction dir)
{
  _activePattern = RAINBOW_CYCLE;
  _millisecondsPerInterval = interval;
  _totalSteps = 255;
  _currentStep = 0;
  _currentDirection = dir;
}

// Update the Rainbow Cycle Pattern
void NeoCube::rainbowCycleUpdate()
{
  for(uint16_t i=0; i< numPixels(); i++) {
    setPixelColor(i, wheel(((i * 256 / numPixels()) + _currentStep) & 255));
  }
}

// Initialize for a Theater Chase
void NeoCube::theaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir)
{
  _activePattern = THEATER_CHASE;
  _millisecondsPerInterval = interval;
  _totalSteps = numPixels();
  Color1 = color1;
  Color2 = color2;
  _currentStep = 0;
  _currentDirection = dir;
}

// Update the Theater Chase Pattern
void NeoCube::theaterChaseUpdate()
{
  for(uint16_t i=0; i< numPixels(); i++) {
    if ((i + _currentStep) % 3 == 0) {
      setPixelColor(i, Color1);
    }
    else {
      setPixelColor(i, Color2);
    }
  }
}

// Initialize for a ColorWipe
void NeoCube::colorWipe(uint32_t color, uint8_t interval, direction dir)
{
  _activePattern = COLOR_WIPE;
  _millisecondsPerInterval = interval;
  _totalSteps = numPixels();
  Color1 = color;
  _currentStep = 0;
  _currentDirection = dir;
}

// Update the Color Wipe Pattern
void NeoCube::colorWipeUpdate()
{
  setPixelColor(_currentStep, Color1);
}

// Initialize for a SCANNNER
void NeoCube::scanner(uint32_t color1, uint8_t interval)
{
  _activePattern = SCANNER;
  _millisecondsPerInterval = interval;
  _totalSteps = (numPixels() - 1) * 2;
  Color1 = color1;
  _currentStep = 0;
}

// Update the Scanner Pattern
void NeoCube::scannerUpdate()
{
  for (uint16_t i = 0; i < numPixels(); i++) {
    if (i == _currentStep) { // Scan Pixel to the right
      setPixelColor(i, Color1);
    }
    else if (i == _totalSteps - _currentStep) { // Scan Pixel to the left
      setPixelColor(i, Color1);
    }
    else { // Fading tail
      setPixelColor(i, DimColor(getPixelColor(i)));
    }
  }
}

// Initialize for a Fade
void NeoCube::fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir)
{
  _activePattern = FADE;
  _millisecondsPerInterval = interval;
  _totalSteps = steps;
  Color1 = color1;
  Color2 = color2;
  _currentStep = 0;
  _currentDirection = dir;
}

// Update the Fade Pattern
void NeoCube::fadeUpdate()
{
  // Calculate linear interpolation between Color1 and Color2
  // Optimise order of operations to minimize truncation error
  uint8_t red = ((Red(Color1) * (_totalSteps - _currentStep)) + (Red(Color2) * _currentStep)) / _totalSteps;
  uint8_t green = ((Green(Color1) * (_totalSteps - _currentStep)) + (Green(Color2) * _currentStep)) / _totalSteps;
  uint8_t blue = ((Blue(Color1) * (_totalSteps - _currentStep)) + (Blue(Color2) * _currentStep)) / _totalSteps;

  colorSet(Color(red, green, blue));
}

// Set all pixels to a color (synchronously)
void NeoCube::colorSet(uint32_t color)
{
  for (uint16_t i = 0; i < numPixels(); i++) {
    setPixelColor(i, color);
  }
  show();
}

// Calculate 50% dimmed version of a color (used by ScannerUpdate)
uint32_t NeoCube::DimColor(uint32_t color)
{
  // Shift R, G and B components one bit to the right
  uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
  return dimColor;
}

// Returns the Red component of a 32-bit color
uint8_t NeoCube::Red(uint32_t color)
{
  return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t NeoCube::Green(uint32_t color)
{
  return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t NeoCube::Blue(uint32_t color)
{
  return color & 0xFF;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t NeoCube::wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}






//------------------------------------------------------------
//------------------------------------------------------------
//------      Arduino Init      ------------------------------
//------------------------------------------------------------
//------------------------------------------------------------

void cubeComplete();
NeoCube cube(3, 1, 5, 6, NEO_GRB + NEO_KHZ800, &cubeComplete);

// Initialize everything and prepare to start
void setup()
{
  Serial.begin(9600);

  Serial.print("total pixels: ");
  Serial.println(cube.numPixels());

    cube.begin();
    cube.rainbowCycle(25);
}

// Main loop
void loop()
{
    cube.update();
}

//------------------------------------------------------------
//Completion Routines - get called on completion of a pattern
//------------------------------------------------------------

// Stick Completion Callback
void cubeComplete()
{

}

#include <NeoCube.h>
#include <Color.h>

namespace NeoCube
{

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
      setPixelColor(i, Color::wheel(((i * 256 / numPixels()) + _currentStep) & 255));
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
        setPixelColor(i, Color::dimColor(getPixelColor(i)));
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
    uint8_t redCompoment1 = Color::redComponent(Color1);
    uint8_t greenCompoment1 = Color::greenComponent(Color1);
    uint8_t blueCompoment1 = Color::blueComponent(Color1);

    uint8_t redCompoment2 = Color::redComponent(Color2);
    uint8_t greenCompoment2 = Color::greenComponent(Color2);
    uint8_t blueCompoment2 = Color::blueComponent(Color2);

    uint16_t remainingSteps = _totalSteps - _currentStep;

    uint8_t red = ((redCompoment1 * remainingSteps) + (redCompoment2 * _currentStep)) / _totalSteps;
    uint8_t green = ((greenCompoment1 * remainingSteps) + (greenCompoment2 * _currentStep)) / _totalSteps;
    uint8_t blue = ((blueCompoment1 * remainingSteps) + (blueCompoment2 * _currentStep)) / _totalSteps;

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

}

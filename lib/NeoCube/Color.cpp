#include <Color.h>

namespace NeoCube
{
  // Convert separate R,G,B into packed 32-bit RGB color.
  uint32_t Color::color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  }

  // Convert separate R,G,B,W into packed 32-bit WRGB color.
  uint32_t Color::color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  }

  // Calculate 50% dimmed version of a color (used by ScannerUpdate)
  uint32_t Color::dimColor(uint32_t color) {
    // Shift R, G and B components one bit to the right
    uint8_t red = Color::redComponent(color);
    uint8_t green = Color::greenComponent(color);
    uint8_t blue = Color::blueComponent(color);

    uint32_t dimColor = Color::color(red >> 1, green >> 1, blue >> 1);
    return dimColor;
  }

  // Returns the Red component of a 32-bit color
  uint8_t Color::redComponent(uint32_t color) {
    return (color >> 16) & 0xFF;
  }

  // Returns the Green component of a 32-bit color
  uint8_t Color::greenComponent(uint32_t color) {
    return (color >> 8) & 0xFF;
  }

  // Returns the Blue component of a 32-bit color
  uint8_t Color::blueComponent(uint32_t color) {
    return color & 0xFF;
  }

  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
  uint32_t Color::wheel(uint8_t wheelPosition) {
    wheelPosition = 255 - wheelPosition;
    if(wheelPosition < 85) {
      return Color::color(255 - wheelPosition * 3, 0, wheelPosition * 3);
    }
    else if(wheelPosition < 170) {
      wheelPosition -= 85;
      return Color::color(0, wheelPosition * 3, 255 - wheelPosition * 3);
    }
    else {
      wheelPosition -= 170;
      return Color::color(wheelPosition * 3, 255 - wheelPosition * 3, 0);
    }
  }
}

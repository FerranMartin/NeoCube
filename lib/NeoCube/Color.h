#include <stdint.h>

namespace NeoCube
{
  class Color
  {
  public:
    // Convert separate R,G,B (or R,G,B,W) into packed 32-bit RGB(or WRGB) color.
    static uint32_t
      color(uint8_t r, uint8_t g, uint8_t b),
      color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    static uint32_t dimColor(uint32_t color);

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    static uint32_t wheel(uint8_t wheelPosition);

    // Returns the Red/Green/Blue component of a 32-bit color
    static uint8_t
      redComponent(uint32_t color),
      greenComponent(uint32_t color),
      blueComponent(uint32_t color);
  };
}

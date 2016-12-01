#include <Arduino.h>
#include <NeoCube.h>

void cubeComplete();
NeoCube::NeoCube cube(3, 1, 5, 6, NEO_GRB + NEO_KHZ800, &cubeComplete);

void setup()
{
  Serial.begin(9600);

  Serial.print("total pixels: ");
  Serial.println(cube.numPixels());

  cube.begin();
  cube.scanner(cube.Color(255,0,0), 25);
}

void loop()
{
  cube.update();
}

void cubeComplete()
{

}

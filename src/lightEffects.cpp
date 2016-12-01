#include <Arduino.h>
#include <NeoCube.h>

void cubeComplete();
NeoCube cube(3, 1, 5, 6, NEO_GRB + NEO_KHZ800, &cubeComplete);

// Initialize everything and prepare to start
void setup()
{
  Serial.begin(9600);

  Serial.print("total pixels: ");
  Serial.println(cube.numPixels());

    cube.begin();
    cube.scanner(cube.Color(255,0,0), 25);
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

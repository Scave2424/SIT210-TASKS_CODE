# Task 1.1P - Switching ON Lights

## System description

This system is built for Linda, who gets home late in the evening during winter and needs light to find her keys and get inside safely. Pressing a push button switches on both the porch light and the hallway light at the same time. The porch light stays on for 30 seconds and the hallway light stays on for 60 seconds. Both lights turn off automatically once their time is up no second button press needed.

The circuit uses an Arduino Nano 33 IoT with a push button as input and two LEDs (porch and hallway) as outputs wired through current-limiting resistors on a breadboard.

## Code overview

I broke the code into separate functions for the button and each light instead of writing it all as one big block, this follows the modular programming approach from the task.

- **checkButtonPress()** - Reads the button pin and detects the moment it's freshly pressed (not held). When that happens it calls the two "turn on" functions.
- **turnOnPorchLight() / turnOnHallwayLight()** - Switch their respective LED on and record the time it happened using `millis()`.
- **updatePorchLight() / updateHallwayLight()** - Run every loop cycle and check how much time has passed since their light turned on. Once the porch light's 30 seconds or the hallway light's 60 seconds are up the function switches that light off.
- **loop()** - Just calls the three functions above in sequence every cycle. It doesn't contain any of the actual logic itself.

I used `millis()` for the timing instead of `delay()`. `delay(30000)` pauses the entire program for that exact duration the Arduino doesn't have multitasking built into `loop()` everything runs one line at a time top to bottom. So if I'd used `delay(30000)` the board would just sit there doing nothing else for 30 seconds straight no other line of code would run until that delay finished. That means `digitalRead(buttonPin)` wouldn't get called during those 30 seconds either so if Linda pressed the button again or if the hallway light's own timer needed checking at the same time none of that could happen, the board would be stuck waiting on that one `delay()` call before it could move on.

With `millis()` the board keeps checking the button and running both light timers at the same time nothing gets blocked.

## Hardware setup

- Push button on D2 (with external 10kΩ pull-down resistor to GND)
- Porch LED on D3 (through a 220-330Ω resistor)
- Hallway LED on D4 (through a 220-330Ω resistor)
- Nano 3V3 to breadboard power rail, Nano GND to breadboard ground rail

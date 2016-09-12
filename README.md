# DISTANCE SENSOR

[Distance Sensor] with **Arduino**, perfect for cars. With LCD screen, LED, buzzer and pause button. Check a real time emulation in [circuits.io].
![alt tag][img/circuits_io.jpg]

 

--------------------------------------------------------------------------------
## OUTPUTS & INPUTS
This distance sensor allows 3 different outputs and 1 input:
- **OUTPUTS**:
    - **LCD Screen**: Shows "Distance: XXXcm" and "Range: X" (or "PAUSED"), with both contrast and backlight regulators (by two potenciometers)
    - **LED**: Flashes, faster as the distance is smaller
    - **Buzzer**: Beeps, faster as the distances is smaller
- **INPUTS**:
    - **Button**: Pause/Resume Button

 

 

--------------------------------------------------------------------------------
## CONFIGURATION
### Distance Ranges and Beep periods (distanceSensor.ino):
```c
const int rangesSize = 4;
const short int ranges[rangesSize]    = {10, 30, 100, 200};  // cm
const short int periods[rangesSize-1] = {200, 600, 1500};    // ms
```
- `cm < ranges[0]`: LED and the buzzer with continuous light and sound.
- `cm > ranges[rangesSize-1]`: LED and the buzzer with no light nor sound.
- `ranges[i] < cm < ranges[i+1]`: LED and buzzer with flash and beep every periods[i] milliseconds.

### Sensing Range and Frequency (Ultrasonic.cpp):
```c
Time_out=12000;  // 6000 µs = 1m // 30000 µs = 5 m 
```
`Time_out` determines the maximum sensing range, proportional to the relation 6000µs->1m (max is 24000µs->4m). It also affects the sensing frequency (the Time_out is the minimum time for each distance measure).

 

 

--------------------------------------------------------------------------------
## ABOUT SENSOR (HC-SR04) & EMULATION ([circuits.io])

This setup has been developed for the Ultrasonic Sensor HC-SR04, which is not available in circuits.io (another one is shown just for cosmetic reasons). Then, note that:
- To test the program from [circuits.io] emulator, go to [my project] and modify the line in the code (just in case you want to test it with different distances):
```c
// manually set cm=15, cause circuits.io does not handle the HC-SR04 Ultrasonic Sensor
cm=15;
```
- The HC-SR04 would be connected as shown in the Breadboard mini "Ultrasonic_Interface":
    - VCC: yellow
    - Trig: orange
    - Echo: red
    - GND: brown
- Note that it might run slowly, due to the emulation capabilities.

 

 

--------------------------------------------------------------------------------
## OTHER NOTES
The ultrasonic distance sensor range is quite wide (around 20º) and it fails when the surface is not perpendicular or homogeneus. Maybe, a setup with many laser distance sensors would solve those issues.

 

 

--------------------------------------------------------------------------------
## LICENSE
Under development.


[Distance Sensor]: <https://github.com/JaimeMartinSoler/distanceSensor>
[my project]: <https://circuits.io/circuits/2625122-distancesensor>
[circuits.io]: <https://circuits.io/circuits/2625122-distancesensor>
[img/circuits_io.jpg]: <https://raw.githubusercontent.com/JaimeMartinSoler/distanceSensor/master/img/circuits_io.jpg>


[//]: # (.md editor: <http://dillinger.io/>)
[//]: # (.md cheatsheet: <https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet>)
[//]: # (Invisible character for extra line breaking " ": <http://stackoverflow.com/questions/17978720/invisible-characters-ascii>)

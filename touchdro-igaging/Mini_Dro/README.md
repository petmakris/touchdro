# Dro-Mini
A dro display for lathe or milling machine with a STM32F103C8T6 microcontroler for 3 quadrature encoder. 
Use a Oled SSD1306 128 x 64 screen with I2C for the display and 4 push buttons for interaction with the User. 
Use the Arduino ide.

Installation :
1 - Install ARDUINO IDE V1.8.8
2 - Download a zip file containing the Arduino STM32 files : https://github.com/rogerclarkmelbourne/Arduino_STM32/archive/master.zip
3 - Unzip the content of the zip file, and place the Arduino_STM32 folder to [Arduino sketches folder]/[hardware]/[Arduino_STM32]. 
Create the 'hardware' folder if it does not exist yet.
Example how the path should look like: C:\Users\<user>\Documents\Arduino\hardware\Arduino_STM32  
4 - Restart ARDUINO IDE
5 - Choose : Tools > Board:"Generic ST32F103C series"
6 - Choose : Tools > Variant:"STMF32C103CB (20k RAM, 128k Flash)"
7 - Choose : Tools > Upload Method:"STLink"
8 - Choose : Tools > CPU Speed Mhz:"72Mhz (Normal)"
9 - Add additionnal board manager URL : File > Preferences > Additional Boards Manager URLs : "http://dan.drown.org/stm32duino/package_STM32duino_index.json"
10 - Install Arduino SAM Boards : Tools > Boards Manager > Install Arduino SAM boards ( 32bits cortex M3)
11 - Compile ...
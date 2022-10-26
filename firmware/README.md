# Firmware

Here are the instructions for getting the software work on the Arduino chip.

## Programming the Arduino

For a lot of pedal builders, working with the Arduino is going to be the biggest challenge, simply because you have most likely not done it before. It is really quite easy. The steps basically go as follows:

1. Download and install the Arduino IDE (integrated development environment)
2. Open Arduino IDE
3. Connect Arduino to computer using USB cable
4. Configure Arduino IDE for the board (Arduino nano), bootloader (for me, I had to use the Atmega328 old bootloader) and COM port (if only one Arduino is connected, it will only show one availabe port)
5. Open the Arduino `MLS_5.ino` file 
6. Upload sketch to Arduino
7. DONE! It should tell you that it uploaded successfully.

One thing to note is that if you get a generic Ardiuno, such as from Tayda or Aliexpress, it will likely have a different USB chip on it and may require you to download and install a new driver. I have included [the driver I used for the CH340 USB chip](../misc), since there was some garbage and several questionable websites that I found while searching.

If you should need help, the Ardiuno community is really amazing and responsize. I recommend going there for any Arduino specific problems.

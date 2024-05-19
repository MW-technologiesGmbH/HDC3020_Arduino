[![](./images/mw-logo.png)](https://mw-technologies.at)

# HDC3020 I2C with Arduino


![HDC3020](./images/HDC3020.png) 



[![button1](./images/data-sheet.png)](https://www.ti.com/lit/ds/symlink/hdc3020.pdf) 



## QUICK START GUIDE  

### Components 
- HDC3020
- Arduino
- Breadboard 
- 2 x (1 kΩ - 10 kΩ) pull-ups
- Wire jumper cable <br>

| Step |                                                                                                                                                             |
|------|-------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 1    |Refer to the provided image (or consult the HDC3020 datasheet for specific pinout details) to connect the sensor module to your Arduino board using jumper wires:<br>__Please note: For this connection scheme the I2C address is 0x44.__ <br> [<img src="images/HDC3020_breakoutboard_arduino.png" width="35%"/>](images/HDC3020_breakoutboard_arduino.png)|
| 2    | Download the latest version of the Arduino IDE from the official website:<br> https://www.arduino.cc/en/software<br>Follow the on-screen instructions to install the IDE on your computer. Version >1.8.7 recommended.                                                            |
| 3    | Download the ZIP File of this project.|
| 4    | Launch the Arduino IDE application on your computer.|
| 5    | Go to: <br>[<img src="images/add_library.png" width="550"/>](images/add_library.png) |
| 6    | Search for the downloaded ZIP File and open it.|
| 7    | Go to:<br>[<img src="images/open_file.png" width="500"/>](images/open_file.png)|
| 8    | Connect the Arduino to your PC via the USB cable. Select Board “Arduino Uno” and the appropriate COM-Port from Tools menu and upload the sketch |
| 9    | When the upload is finished, open the the "Serial Monitor" with the key combination (Control + Shift + M) or via the menu bar: <br> [<img src="images/serial_Monitor.png" width="400"/>](images/serial_Monitor.png) |

### I2C address
The sensor‘s I2C base address is 0x44 (without R/W bit). Pins ADDR and ADDR1 overrule the I2C base address.(for more information check the data sheet) <br>
<br> 


<br>

## License 
See [LICENSE](LICENSE).

# Uploading the Firmware to NMRduino

This guide will walk you through the process of uploading a firmware to
NMRduino.

## Prerequisites

Before you start, ensure you have the following:

-    An NMRduino, equipped with a Teensy 4.1 microcontroller (see
[https://www.pjrc.com/store/teensy41.html](https://www.pjrc.com/store/teensy41.html))
-    A computer running Windows, macOS, or Linux
-    A USB to micro-USB cable with power and data lines
-    An installed version of the Arduino Integrated Development
Environment (IDE, see
[https://www.arduino.cc/en/software](https://www.arduino.cc/en/software))


## Install Teensyduino

Teensy microcontrollers require additional software to be installed,
known as "Teensyduino", before being programmed with the Arduino IDE. To
install Teensyduino, follow these steps:

1.   Download the latest Teensyduino installer from the PJRC website.
[https://www.pjrc.com/teensy/td_download.html](https://www.pjrc.com/teensy/td_download.html)

1.   Run the installer and follow the on-screen instructions to complete
the installation.

1.   Restart the Arduino IDE.


## Configure Arduino IDE

After installing Teensyduino, one needs to select the Teensy board, CPU
speed, USB port, and other parameters from a list within the Arduino IDE.

Connect the NMRduino to your computer using a compatible USB cable,
usually USB to micro-USB. Then,

1.    Within the Arduino IDE, open the "Tools" menu and under the
"Boards" submenu, select the Teensy board model: "Board -> Teensy ->
Teensy 4.1".

1.    Also choose "USB Type -> Serial", "CPU Speed -> 600 MHz",
"Optimize -> Faster".

1.    Set the USB port to where your Teensy is connected, again from the
"Tools" menu and by selecting "Port -> ... (Teensy 4.1)".  If the board
is not listed, check that the Teensy is receiving power over USB.
Alternatively, check the computer's device manager for connected ports.


## Download the NMRduino/Teensy firmware from GitHub

The latest NMRduino firmware will always be located on this GitHub
repository.

1.    Download files from [../Software/MCU/latest](../Software/MCU/latest).  This is the latest version of NMRduino.

1.    Click on the green "Code" button, and then click "Download ZIP" to
download the firmware as a ZIP file.

1.    Extract the contents of the ZIP file to a location on your computer.


## Upload the Program to Teensy

To upload the firmware from the downloaded repository to your Teensy
board, follow these steps:

1.    Open the Arduino IDE.

1.    Go to "File" > "Open" and navigate to the folder where you
extracted the GitHub repository.

1.    Select the main program file (".ino" extension).

1.    Click the right-arrow icon in the top-left of the IDE window (or
go to "Sketch" > "Upload") to upload the code to the Teensy board.

1.    The Arduino IDE will compile your code and then upload it to the
Teensy. You will see the upload progress in the status bar at the bottom
of the IDE.

1.    Once the upload is successful, you should see a "Done uploading"
message in the status bar.  The Arduino IDE can now be closed.


## Troubleshooting

If you encounter any issues during the upload process, consider the
following troubleshooting steps:

-    Check your connections and ensure the computer and Teensy are
properly connected via USB.
-    Verify that the correct Teensy board and port has been selected in
the Arduino IDE.
-    Restart the Arduino IDE if necessary.
-    Consult the [Teensy
documentation](https://www.pjrc.com/teensy/tutorial.html) and [PJRC
forum](https://forum.pjrc.com/) for more specific troubleshooting.

Congratulations! You have successfully uploaded the firmware to your
NMRduino.

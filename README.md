# Photon NES Controller Decoder

Interfaces with an NES controller to read the register data for button presses and test functionality. A pinout diagram for the controller is found here: http://wiki.nesdev.com/w/index.php/Controller_port_pinout.

These three pins cover the bi-directional communication with the controller:

- D0 - Clock (CLK or Pulse)
- D1 - Latch (OUT)
- D2 - Data (D0)

These pins are used to activate certain flags in the code:

- A5 - DEBUG
- A0 - Manual test restart

*!!WARNING!! - The Photon cannot handle the 5V signal on the data line coming from the controller. A circuit such as this one below is necessary to avoid potential damage to your Photon. The data line would connect where the 5V is, and the D2 pin on the Photon should connect to the 3.3V line in the diagram.*

![image](https://user-images.githubusercontent.com/8311639/116807664-9e068600-aaf1-11eb-971e-6cef20309c4a.png)

Source: https://randomnerdtutorials.com/how-to-level-shift-5v-to-3-3v/

Further reading: https://tresi.github.io/nes/

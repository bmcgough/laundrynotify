# laundrynotify
Vibration sensor notifier based on esp8266. Don't sacrifice wet clothes to the gods of mildew!

## Initial state
The code is using the very useful ArduinoOTA functionality from the [esp8266/arduino folks](https://arduino-esp8266.readthedocs.io/en/latest/).

## Materials
I am using these parts:

 * HiLetgo ESP8266 NodeMCU (available on [Amazon](https://smile.amazon.com/gp/product/B081CSJV2V))
 * HiLetgo SW-420 Vibration Sensor (also availbe on [Amazon](https://smile.amazon.com/gp/product/B00HJ6ACY2))
 
 
## Overview
By attaching an esp8266 with one or more vibration sesnsors to your laundry machine(s) and configuring notifications, you can do your best to avoid leaving wet laundry in the washer.

The board enters "laundry running" state upon detecting continuous vibration over a period of time. Once in that state, when it no longer detects vibration for a period of time, it triggers a notification.

This project is simple, but you should be familiar with Arduino before deciding to implement.

This vibration sensor seems to work well in my initial testing. It seems to be a simple spring switch (you can hear it rattling around) that closes a circuit setting the output pin HIGH. Any vibration sensor that provides similar output should work. You may have to adjust sensitivity based on the vigor of your laundry machine.

I attach two sensors - one for the washer and one for the dryer. My dryer sometimes doesn't completely dry the clothes.

## Procedure

Wire up the sensors. They will want three pins - 3V, GND, OUTPUT. On my board I am using pin D4 and the GND/3V pins next to it for one sensor, and D2 along with the other two GND/3V pins on the same side of the board for the second sensor. I have simple header cables going between the devices.

The esp8266 is put into a magnetic cup and stuck to one side the washer. The sensors are stuck to the washer and dryer with 3M Command strips.

Flash the esp8266 with the stock ArduinoOTA code (Examples->ArduinoOTA->BasicOTA) over USB/serial. Then ensure you can flash it remotely. I just use the Arduino IDE to do this as it is easy and seems to work well.

Edit the laundrynotifyOTA.ino code in this repo with the particulars of your network, sensors, and notifications. Flash it onto the board.

## Future plans
Well, notification isn't here yet. Right now the esp8266 runs a web server on port 80 that displays the number of polling periods each second that the sensor read HIGH. This is roughly "vibes per second" and a value from 0 to 100 with the default polling period of 10ms. I think I will use IFTTT at first, with hopes of a more generic implementation later.

I am also toying with continued notifications and a physical button on the device so that I will keep getting notified until I actually go to the laundry room and hit the button (and presumably do something with the wet clothes).

Other uses? What else vibrates? Perhaps a dishwasher?

Other methods? I had thought about trying to read the motor signatures of the appliances on my electrical lines. This was easier to implement.

## This simple test uses an STM32F407VGT6 microcontroller to control an externally connected LED based on an ADC value reading from a photoresistor.

The circuit diagram isn't the best, but you should get the idea.

The test runs as follows:

microcontroller reads the ADC value and based on an if statement sets the LED either HIGH or LOW

HIGH corresponds to a dark room

LOW corresponds to a somewhat well lit room

a message is printed via a UART cable notifying the user of the state change of the LED

This code is extremely simple, but we can make it match the inital test by replacing the components.

To do this we can...

replace the external LED with the heater

replace the photoresistor with a thermistor

take into account cycle count and the number of cycles

add values for heatcount and a user controlled initial temperature setting as well as run time

Upload our saved data to an SD card for further analysis in your pogramming language of choice

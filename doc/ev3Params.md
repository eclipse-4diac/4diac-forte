Parameters for the ev3Dev interface
===================================

1.    IX: Boolean Input

	* Sensor connected to an input port: 
		> sensor.in[X]
	
		where X is the number of the input in the EV3 (1-4)

	* One of the 6 buttons of the EV3: 
		> button.[up | down | left | right | enter | backspace] 
	
		Only one of the inputs can be selected for each IX. All parameters must be in lower case.

2.    QX: Boolean Output

	* Led of the EV3:
		> led.[right | left].[red | green]
	
		The orange led does not exist, but is the sum of green + red.
	
	* Motor ON/OFF
		> motor.out[X].enable
	
		Where X is the output letter of the EV3 (A-D). A true value turns the motor on, and a false value turns the motor off. 
	
3. 	 IW: Word Input (16 bits)

	* Ultrasonic Sensor from Lego
		> sensorw.in[X]

		where X is the number of the input in the EV3 (1-4)

	* Current duty cycle from a motor (pwm)
		> pwm.out[X]

		Where X is the output letter of the EV3 (A-D). The value is between -100 and 100. It might seem confusing reading an input from an output but even though the signal is from an output, it is an input to the system.

	* Current speed of the motor 
		> speed.out[X]

		Where X is the output letter of the EV3 (A-D). It reads the velocity in degrees per second (360 degrees each turn)

4.   QW: word output (16 bits)

	* Desired duty cycle for a motor (pwm)
		> pwm.out[X]

		Where X is the output letter of the EV3 (A-D). The value range is between -100 and 100.

5.   ID: Double word input (32 bits)

	* Current position of the motor
		> motor.out[X].position


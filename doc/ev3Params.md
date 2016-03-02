# Parameters for the ev3Dev interface

## Standard access

The standard access refer to the access through well defined blocks, that allow transparency in the access of I/O in different platforms. Each block shows the supported parameters according to the type of connection.

1.    IX: Boolean Input

	* Sensor connected to an input port: 
		> sensor.in[X].[Y]
	
		where X is the number of the input in the EV3 (1-4) and Y is the value number where to read from according to the sensor. Read the specification of the sensor to be sure. Start trying from 0 if no information is available.

	* One of the 6 buttons of the EV3: 
		> button.[up | down | left | right | enter | backspace] 
	
		Only one of the inputs can be selected for each IX. All parameters must be in lower case.

2.    QX: Boolean Output

	* Led of the EV3:
		> led.[right | left].[red | green]
	
		The orange led does not exist, but is the sum of green + red.
	
	* Motor ON/OFF
		> motor.out[X].enable
	
		Where X is the output capital letter of the EV3 (A-D). A true value turns the motor on, and a false value turns the motor off. 
	
3. 	 IW: Word Input (16 bits) [CHECK FORMAT]

	* Ultrasonic Sensor from Lego
		> sensorw.in[X].[Y]

		where X is the number of the input in the EV3 (1-4) and Y is the value number where to read from according to the sensor. Read the specification of the sensor to be sure. Start trying from 0 if no information is available.

	* Current duty cycle from a motor (pwm)
		> pwm.out[X]

		Where X is the output letter of the EV3 (A-D). The value is between -100 and 100. It might seem confusing reading an input from an output but even though the signal is from an output, it is an input to the system.

	* Current speed of the motor 
		> speed.out[X]

		Where X is the output letter of the EV3 (A-D). It reads the velocity in degrees per second (360 degrees each turn)

4.   QW: word output (16 bits) [CHECK FORMAT]

	* Desired duty cycle for a motor (pwm)
		> pwm.out[X]

		Where X is the output letter of the EV3 (A-D). The value range is between -100 and 100.

5.   ID: Double word input (32 bits) [TO BE IMPLEMENTED]

	* Current position of the motor
		> motor.out[X].position

## Custom access

The custom access allows the user to access part of the system that are not implemented in the standard access. Care must be taken, since critical part of the system might be accessed. For each block, the specification of the new inputs and outputs are given.

1.   fileWriter: Block to write to any file in the system
	
	* FILE_NAME [input]: absolut path of the file. 
		> Example: /sys/class/tacho-motor/motor1/command
	
	* S1 [input]: String to write to the file
		> Example: run-direct
	
2.  fileReader: Block to read from any file in the system [TO BE IMPLEMENTED]

	* FILE_NAME [input]: absolut path of the file. 
		> Example: /sys/class/tacho-motor/motor1/duty_cycle
	
	* S1 [output]: String read from the file
		> Example: 58
 	


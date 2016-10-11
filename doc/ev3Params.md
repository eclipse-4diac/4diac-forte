# Parameters for the ev3Dev interface

## Standard access

### Transparent access

The standard access refer to the access through well defined IO blocks, that allow transparency in the access of I/O in different platforms. Each block supports different parameters. The blocks used to control the I/O are name two capital letters. 

1. The first letter indicates the direction:
	
	* Q: output
	* I: input
	
2. The second letter indicates the size of it:

	* X: boolean
	* W: Word (16 bits)
	* D: Double (32 bits)

So, for example an boolean input is read using the functional block IX. The output word, for example the PWM value of a motor is controlled with a QW block. Not all sizes are implemented in all platforms.

### Parameters for the EV3

The parameters for the EV3 were desgined for general use, and extension of them is easy to achieve. The format of the parameters follow 2 important rules:

1.	The only capital letters allowed are the A,B,C,D when used to define the output ports (outA, outB, outC, outD).

2.	The parameters are given in three parts separated by points:
			
	>   port.typeOfIO.specific

	1.port

	The first part of the parameter specifies the port of the Input or Output. For the EV3, the possible values  are [in1 | in2 | in3 | in4 | outA | outB | outC | outD | emb]. The first eight are directly related to the actual ports of the EV3. The 'emb' port refers to anything that can be managed in the EV3 but not in the I/O ports, until now the leds and buttons, but in the future the screen and speaker would also be reference with the 'emb' port.
	
	2. typeOfIO

	The second part, specifies what is actually connected to the specified port. The possible values are [sensor | sensorw | button | led | motor]. The difference between 'sensor' and 'sensorw' is that 'sensor' is used for boolean sensors, and 'sensorw' for sensors which values are words (16 bits). The 'led' and 'button' are used to control the embedded leds and buttons of the EV3. 'motor' is used for connected motors in any of the outputs.

	It must be clear that not all of the types can be used with any port. For example a led is only an output, so if used with a input, for example IX it won't intialize. Also, if used to control as a word QW, it won't initialize either.

	3. specific

	As its name says, this third parameter has no general form, and can also be formed of other several parameters (as in case of the led). Depending of the second parameter, the allowed values for this one are different, and it will be seen in the follow section.

#### Specific parameters

1. led:

	* right.red		(output only)
	* right.green	(output only)
	* left.red		(output only)
	* left.green	(output only)

	Each specific is used to control each one of the four leds available. The orange led doesn't exist, but is the sum of red + green, so in order to turn on an orange light of the left for example, both the green and red of the left must be turned on.

	Limitation: 'led' must be used with QX and with the 'emb' port.

	Example: 
	
	> emb.led.right.red

2. button

	* up			(input only)
	* down 			(input only)
	* left			(input only)
	* right			(input only)
	* enter			(input only)
	* backspace		(input only)

	Reads the buttons of the EV3. 

	Limitation: 'button' must be used with IX and with the 'emb' port.

	Example:

	> emb.button.enter

3. sensorw 

	* [0-N] 	(input only)
	* mode 		(input or output)

	The sensorw is used with sensors that read a word value isntead of a boolean and normally are more complex than regular touch sensors (boolean). Each sensor specifies how it retrieves the data, how many values and the format of it. Specifications of the supported sensors by the ev3dev are listed [here](http://www.ev3dev.org/docs/sensors/).

	Each one of the sensors' values is stored in a different file in the EV3. The specific [0-N] determines which file it is. For example the GYRO sensor can show the angle and the rotation speed. The values are store in value0 and value1 files, so to access just the rotation speed, the '1' specific must be used. The actual value must be checked in the link above.

	The 'mode' specific is used to change the mode of the sensor according to its specification. As an input, mode will retrieve the mode in which the sensor is working, and as output, the mode will be changed. The mode are  mirrored using a 0 index according to the specification of the sensor.

	Limitation: sensorw cannot be used with boolean blocks (QX or IX). It must be used with an in[1-4] port, but is not restricted to input or output blocks.
	
	Limitation: [0-N] can be used only as input, and the number shouldn't be greater than the allowed by the sensor in the mode that's working at the moment of initialization. 
	
	Examples: Let's see an example of using the GYRO sensor specified [here](http://www.ev3dev.org/docs/sensors/lego-ev3-gyro-sensor/). The GYRO works in GYRO-ANG by default, showing only one value, then the only possible parameter is:
	
	> in1.sensorw.0  (IW)
	
	If a 1 instead of a 0 were used, the initialization would have failed. If first the sensor's mode is changed using a QW with parameter 
	
	> in1.sensorw.mode (QW)
	
	and writing a value 3 (0 index mode, see link of the sensor above), the sensor's mode will change to GYRO-G&A wich shows 2 values, angle and rotation speed. After changing the mode, two IW blocks with parameters
	
	> in1.sensorw.0 (IW)
	> in1.sensorw.1 (IW)
	
	will retrieve the both values. Remember, to get the rotation speed, the second block must be initialized AFTER the mode has been changed. Using the 'mode' specific with a IW will retrieve the mode as 0 index.

4. sensor

	*[0-N]	(input only)

	 'sensor' specific is a simpler specific of the 'sensorw'. It doesn't allow mode, and even though the value is always stored in '0', the '0' specific must be used for two mainly reasons: 1. To have a standarized way of showing the parameters, and 2. to avoid problems in the future where a new sensor is stored in '1' instead of '0'.

	Limitations: 'sensor' must be used with QX and with an in[1-4] port.
	
	Example:
	
	> in2.sensor.0

5. Motor

	* enable		(output boolean only)
	* reset			(output boolean only)
	* stop			(output word only)
	* pwm			(input word or output word only)
	* position		(input word only)
	* speed			(input word only)
	* rot			(input word only)

	The 'enable' turns ON the motor with TRUE value and turn OFF with a FALSE value. 
	
	The 'reset' zeroes the position, pwm and speed values, and turn off the motor with a true value. A false value has no effect. 
	
	The 'stop' specific must not be confused with stoping the motor, because it specifies the way the motor behaves when PWM is zero according to the value written: 

		* 0: [coast]: Removes power from the motor. The motor will freely coast to a stop.
		
		* 1: [brake]: Removes power from the motor and creates a passive electrical load. This is usually done by shorting the motor terminals together. This load will absorb the energy from the rotation of the motors and cause the motor to stop more quickly than coasting.
	
		* 3: [hold]: Causes the motor to actively try to hold the current position. If an external force tries to turn the motor, the motor will “push back” to maintain its position.
	
	The 'pwm' reads the current pwm of the motor or writes the desired pwm for the motor.
	
	The 'position' reads the current position of the motor. In how many steps a rotation is divided, can be read from using the 'rot' specific.
	
	The 'speed' reads the current speed in positions by second.
	
	The 'rot' reads the motor specication of how many steps are in one turn of the motor. This can be read from the specifications of the motor normally and it's not necessary to add the block in an application just to read this information.
	
	Examples:
	
	> outB.motor.enable (QX)
	
	> outB.motor.pwm (QW writes the desired pwm, IW reads the actual pwm)
	
	> outB.motor.position (IW reads actual position) 
	
## Blocks cheatsheet

1.    IX: Boolean Input

	* Sensor connected to an input port: 
		> in[X].sensor.[Y]
	
		where X is the number of the input in the EV3 (1-4) and Y is the value number where to read from according to the sensor. Read the specification of the sensor to be sure. Start trying from 0 if no information is available.

	* One of the 6 buttons of the EV3: 
		> emb.button.[up | down | left | right | enter | backspace] 
	
		Only one of the inputs can be selected for each IX. All parameters must be in lower case.

2.    QX: Boolean Output

	* Led of the EV3:
		> emb.led.[right | left].[red | green]
	
		The orange led does not exist, but is the sum of green + red.
	
	* Motor ON/OFF
		> out[X].motor.enable
	
		Where X is the output capital letter of the EV3 (A-D, in uppercase). A true value turns the motor on, and a false value turns the motor off. 

	* Motor Reset
		> out[X].motor.reset

		Where X is the output capital letter of the EV3 (A-D, in uppercase). A true value resets all of the motor parameter attributes to their default values. This will also have the effect of stopping the motor.
	
3. 	 IW: Word Input (16 bits)

	* Sensor value
		> in[X].sensorw.[Y]

		where X is the number of the input in the EV3 (1-4) and Y is the value number where to read from according to the sensor. Read the specification of the sensor to be sure. Start trying from 0 if no information is available.

	* Sensor mode
		> in[X].sensorw.mode
		
		where X is the number of the input in the EV3 (1-4). The read value is an index number according to the list of modes of the sensor.

	* Current duty cycle from a motor (pwm)
		> out[X].motor.pwm

		Where X is the output letter of the EV3 (A-D, in uppercase). The value is between -100 and 100. It might seem confusing reading an input from an output but even though the signal is from an output, it is an input to the system.

	* Current speed of the motor 
		> out[X].motor.speed

		Where X is the output letter of the EV3 (A-D, in uppercase). It reads the velocity in steps per second

	* Number of steps in each turn
		> out[X].motor.rot

		Where X is the output letter of the EV3 (A-D, in uppercase).

	* Current position of the motor
		> out[X].motor.position

		Where X is the output letter of the EV3 (A-D, in uppercase).

4.   QW: word output (16 bits) [CHECK FORMAT]

	* Sensor mode
		> in[X].sensorw.mode
		
		where X is the number of the input in the EV3 (1-4). The value to be written should be the index number according to the list of modes of the sensor.

	* Desired duty cycle for a motor (pwm)
		> out[X].motor.pwm

		Where X is the output letter of the EV3 (A-D, in uppercase). The value range is between -100 and 100.

	* Desired behaviour when stop
		> out[X].motor.stop

		Where X is the output letter of the EV3 (A-D, in uppercase). This parameter is used to specify the behaviour of the motor when stoping. Possible values are 0, 1 and 2:
		
	    * 0: [coast]: Removes power from the motor. The motor will freely coast to a stop.
	    * 1: [brake]: Removes power from the motor and creates a passive electrical load. This is usually done by shorting the motor terminals together. This load will absorb the energy from the rotation of the motors and cause the motor to stop more quickly than coasting.
	    * 3: [hold]: Causes the motor to actively try to hold the current position. If an external force tries to turn the motor, the motor will “push back” to maintain its position.
 
5.   ID: Double word input (32 bits) [TO BE IMPLEMENTED]

	

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
 	


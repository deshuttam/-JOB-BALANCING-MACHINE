# A CONTROL UNIT FOR INDUSTRIAL JOB  BALANCING MACHINE 

By Uttam Deshpande et. al.,


## Introduction
A balancing machine is a measuring tool used for balancing rotating machine parts such as rotors for electric motors, fans, turbines, disc brakes, disc drives, propellers and pumps. The machine usually consists of two rigid pedestals, with suspension and bearings on top supporting a mounting platform. The unit under test is bolted to the platform and is rotated either with a belt-, air-, or end drive. As the part is rotated, the vibration in the suspension is detected with sensors and that information is used to determine the amount of unbalance in the part. Along with phase information, the machine can determine how much and where to add or remove weights to balance the part. The machine consists of a motor which is responsible for rotating the unit under test. Unit under test has to be rotated with constant speed to get the required vibrations for the sensors. The cases have been reported where in the rpm of the machine goes out of control and this may cause a severe damage to the machine parts and the UUT. Hence it becomes necessary to control the rpm of the motor and to make it shut down if its rpm crosses beyond certain limits. Further heat generated during this process has to be under control. 

![image](https://user-images.githubusercontent.com/107185323/199002112-84bc5e52-9569-4e91-8441-319b4f4ae78d.png)

### IMPLEMENTATION
The implementation details are as follows:  For measurement of rotations per minute of the motor the induction based proximity sensor is used. For the measurement of temperature sensor LM35 temperature sensor is used. This detailed explanation of how the RPM and Temperature are sensed and their circuit diagrams are given below.

![image](https://user-images.githubusercontent.com/107185323/199002014-6c23d3f4-9e7a-4e4b-b4f1-b758b0357144.png)

The schematic shown in figure 4.2 gives a visual of how the LM35 temperature sensor is interfaced with the LPC2148 microcontroller. 

![image](https://user-images.githubusercontent.com/107185323/199002056-b985b4ca-1cef-430e-b7e9-b5c3e4001147.png)


### USER INTERFACE
The diagram shown below is a pictorial representation of the final user interface with which the operator interacts.

![image](https://user-images.githubusercontent.com/107185323/199002226-19760b2a-8086-4111-a3b3-2e3db842b773.png)

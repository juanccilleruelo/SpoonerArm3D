# SpoonerArm3D

SpoonerArm3D is a 6 axis robotic arm with sensors based on arduino. 

SpoonerArm3D will include all the designs for be printed on 3D.
Includes also the electronic designs, assembly of components and software creation for the microcontrollers.

## Principles
   The result must to be educative but professional
   Also must allow updates to achieve the needs of the final use.
   Of course, all the documentation is going to be part of the project.
   
   Any collaborator must accept this principles. Branches without a complete documentation will not be merged. 
   
![Not real images while we don't have designed parts](https://github.com/juanccilleruelo/SpoonerArm3D/blob/main/docs/2-Figure1-1)


Robot arms are usually referred to by their degrees of freedom (DOF). This term indicates the number of rotational joints or axes on a particular arm; for instance, a 4-DOF arm can rotate at four separate joints. The more DOF, the more range of movement possible.

The axes of the arm will move simultaneously in soft and precisely movements.

It will accept instructions like AXE1.Move(60) or ARM3.BlockAt(90).
You can send to the arm instructions to move from a position to a new position, but you can specify that the inclination of part of the arm should stay in an inclination determined. 
You can send to the arm instructions in steps or in degreess. 

The Arm will be designed to hold different tools. This requires to define an interface mechanical, electrical and of software, to allow send instructions to the different tools.

The arm can receive the instructions, in different means:
   - In a file of text saved in memory (can be SD memory or other types)
   - Through LoRa. This requires define and create the interface to send and receive the instructions.
   - Through a joystick and buttons. These will be a different project that connect with this one.
   - Included in the code of the sketch. But with the same syntax that the rest of form.


The arm needs some type of calibration and it is to be programmed in the software base.

The length of each part of the Arm, will be designed to easily allow changes.

The first tool to be attached is a gripper. Of course the first version will be simple, but next version we want implement an pressure sensor and implement the commands that limit the press to do when hold something. 
Some ideas are:
    CNC, Pneumatic for suck or paint, Laser, Drill, 3D printing, 


The project is divided into the next stages: 
   1. Design the support of the base with the support for the first Servo. 
   2. Install the rotation base and complete the first join of the robot arm.

## Well Know limitations
   1. Servo are 180º. Take into account this all the time. 

![Not real images while we don't have designed parts](https://github.com/juanccilleruelo/SpoonerArm3D/blob/main/docs/industrialrobots-article-2.jpg)



# KittenArm3D
KittenArm3D is a robotic arm with sensors based on arduino. 

KittenArm3D will include all the designs for be printed on 3D.

The axes of the arm will move simultaneously in soft and preciselly movements.

It will accept instructions like AXE1.Move(60) or ARM3.BlockAt(90).
You can send to the arm instructions to move from a position to a new position, but you can specify that the inclination of part of the arm should stay in an inclination determined. 
You can send to the arm instructions in steps or in degress. 

The Arm will be designed to hold different tools. This requires to define an interface mechanical, electrical and of software, to allow send instructions to the different tools.

The arm can receibe the instructions, in different means:
   - In a file of text saved in memory (can be SD memory or other types)
   - Through LoRa. This requires define and create the interface to send and receibe the instructions.
   - Through a joystick and buttons. These will be a different project that connect with this one.
   - Included in the code of the sketch. But with the same sintax that the rest of form.


The arm needs some type of calibration and it is to be programmed in the software base.

The first tool to be attached is a gripper. Of course the first version will be simple, but next version we want implement an pressure sensor and implment the commands that limit the press to do when hold something. 



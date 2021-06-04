# stm32-serial-pc
This ROS node is used to read data sent from MCU to PC via serial port. It can read two kinds of data frame, the attitude and angular velocity. 

1. Toolchain installation

1.1 Ubuntu 18.04:
https://releases.ubuntu.com/18.04/

1.2 Ubuntu install of ROS Melodic:
http://wiki.ros.org/melodic/Installation/Ubuntu

1.3 ros-serial package:
sudo apt-get install ros-melodic-serial

2. ROS node usage

2.1 Create a ROS workspace:http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment

2.2 Run this node in terminal: rosrun parse_pkg parse_pkg_node

2.3 Record data: rosbag record -a

3. Communication protocol
![image](https://user-images.githubusercontent.com/30153639/119234062-6de55c80-bb5e-11eb-8723-11e260e9fdf8.png)

start: 2 bytes, always be 'A'(0x41),'T'(0x54).

tar-ID: always be 'A'(0x41). 

src-ID: source node id, such as '1'(0x31).

msg-ID: message id, '00'(0x30, 0x30) or '01'(0x30, 0x31). 
'00' represents the angular velocity frame, '01' represents the attitude frame. 

data-length: always be '2'(0x32),'0'(0x30).

data-section: the length of this section is 20 bytes indicated by data-length. As every four bytes represent a float number, the data segment can express 5 float type numbers.  
For angular velocity frame, the first three float numbers are roll, pitch, and yaw rate in sequence. The remaining bytes should be set to 0. 
For attitude frame, the first 4 float numbers are w, x, y, z in sequence. The remaining bytes should be set to 0. Note w, x, y, z are the components in quaternion. 

Checksum byte: the lower byte of the sum of all bytes, including Checksum byte itself, is 0.

end: always be  '!'(0x21).

4. Data visualization tools

4.1 rqt_bag:http://wiki.ros.org/rqt_bag

4.2 plotjuggler:https://plotjuggler.io/

5.Example package

5.1 attitude frame:if you send :"0x41 0x54 0x41 0x30 0x30 0x31 0x31 0x34 0x00 0x00 0x80 0x3f 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x3f 0x46 0x21"

means:

w = 1;

x = 0;

y = 0;

z = 0;

thrust = 0.5;

5.2 angular velocity frame:if you send :"0x41 0x54 0x41 0x30 0x30 0x30 0x31 0x34 0x00 0x00 0x00 0x00 0x00 0x00 0x80 0x3f 0x00 0x00 0x00 0x40 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x45 0x21"

means:

roll_rate = 0;//roll  0rad/s

pitch_rate = 1;//pitch 1rad/s

yaw_rate = 2;//yaw   2rad/s

idle = 0;

idle = 0;

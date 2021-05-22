# stm32-serial-pc
This ROS node is used to read data sent from MCU to PC via serial port. It can read two kinds of data frame, the attitude and angular velocity. 

1.Toolchain installation

1.1 ubuntu 18.04:
https://releases.ubuntu.com/18.04/

1.2 ubuntu install of ROS Melodic:
http://wiki.ros.org/melodic/Installation/Ubuntu

1.3 ros-serial package:
sudo apt-get install ros-melodic-serial

2.ROS node usage

2.1 create a ROS workspace:http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment

2.2 run this node in terminal: rosrun parse_pkg parse_pkg_node

2.3 record data: rosbag record -a

3.communication protocol
![image](https://user-images.githubusercontent.com/30153639/119234062-6de55c80-bb5e-11eb-8723-11e260e9fdf8.png)

start: 2 bytes, always be 'A'(0x41),'T'(0x54).

tar-ID: always be 'A'(0x41). 

src-ID: source node id, such as '1'(0x31).

msg-ID: message id, '0'(0x30) or '1'(0x31). 
'0' represents the angular velocity frame, '1' represents the attitude frame. 

data-length: always be '2'(0x32),'0'(0x30).

data-section: The length of this section is 20 bytes indicated by data-length. As every four bytes represent a float number, the data segment can express 5 float type numbers.  
For angular velocity frame, the first three float numbers are roll, pitch, and yaw in sequence. The remaining bytes should be set to 0. 
For attitude frame, the first 4 float number are w, x, y, z in sequence. The remaining bytes should be set to 0. Note w, x, y, z are the components in quaternion. 

Checksum byte: The lower byte of the sum of all bytes, including Checksum byte itself, is 0.

end: always be  '!'(0x21).

5.Data visualization tools

5.1 rqt_bag:http://wiki.ros.org/rqt_bag

5.2 plotjuggler:https://plotjuggler.io/

 

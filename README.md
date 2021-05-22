# stm32-serial-pc
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

2.3 record data : rosbag record -a

3.communication protocol
![image](https://user-images.githubusercontent.com/30153639/119227161-2f8c7500-bb3f-11eb-8058-e63c0601ac9a.png)

start: set 'AT' as the start of a package.

tar-ID：target node id，if set to 'A' means that the package is sent to PC.

src-ID：source node id, such as '1'.

msg-ID：message id. If set to 0, that is, the angular velocity data packet, the first three data segments are roll, pitch, and yaw in sequence. You need to use '0' to occupy the remaining two data segments; if set to 1, it is a attitude data packet, and the atttitude is represented by a quaternion, followed by w, x, y, z, and the remaining data segment is the thrust, you can set this bit any data between 0 and 1.


data-length：In this protocol, the length of the data segment is 20 bytes.

data-section：Every four bytes represents a float type number, so the data segment can express 5 float type numbers. For the meaning of each float number, refer to the description of msg-ID.

parity bit：The check byte and the last eight bits of all the previous bytes are added to 0.

end：set ‘！’ as the end of a package.

5. Data visualization tools

5.1 rqt_bag:http://wiki.ros.org/rqt_bag

5.2 plotjuggler:https://plotjuggler.io/

 

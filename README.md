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

start: set 'AT' as the start of a package
tar-ID：target node id，if set to 'A' means that the package is sent to PC.

src-ID：source node id, you can set this bit to a number, such as '1'

msg-ID：message id, 设置为0，即角速度数据包，前三个数据段依次为滚转，俯仰，横滚。剩余的两个数据段你需要用‘0’占位。设置为1，即为姿态数据包，姿态用四元数表示，依次为w,x,y,z，剩余的一个数据段为拉力值大小，你可以设置为0~1之间的任何数据。

data-length：在这个协议中，数据段长度都是20个字节

data-section：每四个字节表示一个浮点数，故而数据段能表达5个浮点数，每个浮点数的意义，参考msg-ID的描述。

校验：校验字节与之前所有字节的末八位相加为0

end：set ‘！’ as the end of a package

 

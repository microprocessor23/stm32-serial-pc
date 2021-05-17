/**
 * date: 2021.1.9
 * author: kaidi wang
 * mail:   1055080765@qq.com
 * describe: swarmlink parse function
**/
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Char.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Quaternion.h>
#include <swarmlink_parse/child_arming_parse.h>
#include <swarmlink_parse/child_attitude_parse.h>
#include <serial/serial.h>
#include "std_msgs/UInt8MultiArray.h"
#include "std_msgs/UInt8.h"


std::string  float_to_char(float fvalue);//float to char function
float get_float_from_string_yaw(const std::string &x);//string to float
void serial_receive_function();//serial parse function

std::string redundancy_string_last;
//define a serial port
serial::Serial ser1;
std_msgs::String result;//define a string of parse data
std_msgs::String result_last;


std_msgs::Float64 thrust;
geometry_msgs::Quaternion q;
//define a message basic parse class 
msg_basic_info msg_info;
msg_basic_info msg_info_last;


std_msgs::String test_str;// I need to known when the CAN bus is broken. 
std::string data_yaw;

//define list of swarmlink structure
child_arming_parse arm_parse;
child_attitude_parse attitude_parse;
int rec_len=0;


std::string float_to_char(float  fvalue )
{
	std::size_t size = sizeof(fvalue);
	std::string _return(size,0);
	char* start  = reinterpret_cast<char *>(&fvalue);
	for(std::size_t i =0; i<size; i++)
	{
		_return[i]=*(start+i);
	}
	return _return;
}

void parse_data_fun(std_msgs::String msg)
{
 	
	std::string rece_str;
	rece_str.clear();
	rece_str = redundancy_string_last+msg.data;

	msg_info = msg_info.parse_get_basic_info(rece_str);
	redundancy_string_last = "";
	redundancy_string_last = msg_info.redundancy_string;

	if(msg_info.target_id=='A'||msg_info.target_id==MY_ID)
	{
		//ROS_INFO_STREAM("good for my command...");
		switch (msg_info.msg_id)
		{
		case 0:// msg id is 0
			//ROS_INFO_STREAM("start to parse arm command");
			arm_parse.parse(msg_info.data_section);
			break;
		case 1:
			attitude_parse.parse_new(msg_info.data_section);
			q = attitude_parse.q;
			thrust = attitude_parse.thrust;
			ROS_INFO_STREAM("thrust: "<<thrust.data<<" q.w:"<<q.w<<" q.x:"<<q.x<<" q.y:"<<q.y<<" q.z:"<<q.z);
			break;
		default:
			break;
		}
	}
	else
	{
//		ROS_ERROR_STREAM("this command is not send to me");
	}
//	ROS_ERROR_STREAM("------------------------------------------");
}


float get_float_from_string_yaw(const std::string & x)
{
	char *p = const_cast<char*>(x.c_str());
	float *_return = 0;
	_return = reinterpret_cast<float*>(p);
	return *_return;
}


void serial_receive_function()//,std_msgs::Bool emg
{
	rec_len = ser1.available();
	if(ser1.waitReadable())
	{
		ser1.waitByteTimes(30);
		std_msgs::String serial_data;
		serial_data.data="";
		ser1.read(serial_data.data,30);
		parse_data_fun(serial_data);
	}

}



int main(int argc,char** argv)
{
	ros::init(argc,argv,"swarmlink_parse_node");
	ros::NodeHandle nh;

	ros::Publisher attitude_quat_pub = nh.advertise<geometry_msgs::Quaternion>("Attitude_q",100);
	ros::Publisher attitude_thrust_pub = nh.advertise<std_msgs::Float64>("Attitude_thrust",10);

	try
        {
		//serial init
		ser1.setPort("/dev/ttyUSB0");
		ser1.setBaudrate(460800);//115200, 460800
		serial::Timeout to = serial::Timeout::simpleTimeout(33.33);
		ser1.setTimeout(to);
		ser1.open();
		if(ser1.isOpen())
		{
	    	ROS_INFO_STREAM("the serial port is opened succeed...");
		}
        }
        catch(serial::IOException& e)
        {
		ROS_ERROR_STREAM("Unable to open port");
		return -1;
        }

	while (ros::ok())
	{
		serial_receive_function();
		if(q.w!=0 && q.x!=0 && q.y!=0 && q.z!=0 && thrust.data!=0)
		{
			if(thrust.data>0.000001 && thrust.data < 100)
			{
				attitude_quat_pub.publish(q);
				attitude_thrust_pub.publish(thrust);
			}
			
		}
;
		ros::spinOnce();
	}
	
	return 0;
}


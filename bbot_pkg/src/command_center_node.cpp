//
//  command_center_node.cpp
//  
//
//  Created by Austin Burch on 11/1/16.
//
//

#include "command_center_node.h"

using namespace std;

double command_center_time = 0;
int sensing_count = 0;
int driving_count = 0;

double Hm = 1;  //table height
double q1 = 15;
double q2 = 30;
double qf = 45;





// joint vars structure
struct Joint {
    float position = 0, speed = 0, accel = 0;
    float K_p=0, K_i=0, K_d=0;
    float e=0, P_val=0, I_val=0, D_val=0, PID_val=0;
    float prev_e=0, prev_I=0;
    int period=PERIOD_INIT,dutycycle=0,direction;
};
Joint joint1, joint2;

struct EndEff {
	float xPos;
	float yPos;
	float zPos;
	float vel;
    float vel_e, prev_vel_e;
    
    
};
EndEff endeff;

struct Hoop {
    double theta, xPos_G, yPos_G, xPos_R, yPos_R;
    
};
Hoop hoop;

double ball_vx0 = 0;
double ball_vy0 = 0;

// Callback Function for link1_node
void link1_msgCallback(const bbot_pkg::link1_msg::ConstPtr& link1_msg)
{
    link1_count = link1_msg->link1_count;
    
	joint1.position = link1_msg->link1_position;
	joint1.speed = link1_msg->link1_speed;
	joint1.accel = link1_msg->link1_accel;
    
    joint1.period = link1_msg->link1_period;
    joint1.dutycycle = link1_msg->link1_dutycycle;
    
    ROS_INFO("i: %d  j1_pos: %f  j1_speed: %f  j1_accel: %f  j1_dutycycle: %i ",i,joint1.position,joint1.speed,joint1.accel,joint1.dutycycle);
    
}




// Callback Function for link2_node
void link2_msgCallback(const bbot_pkg::link2_msg::ConstPtr& link2_msg)
{
    link2_count = link2_msg->link2_count;
    
	joint2.position = link2_msg->link2_position;
	joint2.speed = link2_msg->link2_speed;
	joint2.accel = link2_msg->link2_accel;
    
    joint2.period = link2_msg->link2_period;
    joint2.dutycycle = link2_msg->link2_dutycycle;
    
    ROS_INFO("i: %d  j1_pos: %f  j1_speed: %f  j1_accel: %f  j1_dutycycle: %i ",i,joint2.position,joint2.speed,joint2.accel,joint2.dutycycle);
    
}


// Main Routine
int main(int argc,char **argv){
    
    // setup comm
    ros::init(argc,argv,"command_center_node");
 	ros::NodeHandle link1_nh;
    ros::NodeHandle link2_nh;
    ros::NodeHandle command_center_nh;
    
	ros::Publisher command_center_pub=command_center_nh.advertise<bbot_pkg::command_center_msg>("command_center_msg",BUFF_SIZE);

    ros::Subscriber link1_sub=link1_nh.subscribe("link1_msg_node",BUFF_SIZE,link1_msgCallback);
    ros::Subscriber link2_sub=link2_nh.subscribe("link2_msg_node",BUFF_SIZE,link2_msgCallback);
    
    ros::Rate loop_rate(SPS); // maybe dont need this? just check to see it new data has arrived?
    
	int command_center_count=0;
    
    // user inputs
    hoop.theta = 0 ; // [deg]
    hoop.xPos = 2; // [m]
    hoop.yPos = .5; // [m];

    // trajectory calculations
    hoop.xPos_G=hoop.xPos-(((3*2.54)/100)*cos(hoop.theta));  //Adjust distance for hoop (m)
    hoop.yPos_G=hoop.yPos+(3*2.54)/100*sin(hoop.theta);
    
    endeff.xPos_G=l1*sin(q1)+l2*sin(q1+q2);
    endeff.yPos_G=l1*cos(q1)+l2*cos(q1+q2);
    
    endeff.xPos_R=hoop.xPos_G-endeff.xPos_G;
    endeff.yPos_R=hoop.yPos_G-(Hm-endeff.yPos_G);
    
    //Calculation of Required Initial Velocity -  math ref - > pow (7.0, 3.0)
    ball_vy0 = pow((GRAVITY_CONST/((endeff.xPos_R*tan(qf)-endeff.yPos_R)*2)),0.5)*(tan(qf)*endeff.xPos_R);
    ball_vx0 = ball_vy0/tan(qf);
    
    //Calculation of q1_dot & q2_dot
    //v=[-vy;vx;0;0;0;1]; // v matrix
    //J=[-l1*sin(q1)-l2*sin(q1+q2),-l2*sin(q1+q2);l1*cos(q1)+l2*cos(q1+q2),l2*cos(q1+q2);... // jacobian
       0,0;0,0;0,0;1,1];

    // q=v\J    %Calculation of Angular Velocity Matrix
    //q_dot = inv(J([1 2], [1 2])) * v([1 2], [1]);
    
    //q_ddot1 = (q_dot(1)^2)/(2*q1);
    //q_ddot2 = (q_dot(2)^2)/(2*q2);
    //q_ddotn = [q_ddot1;q_ddot2];
    
    
    
    int release = false ;
    
	while(ros::ok())
	{
        // update driving and sensing vars
        bbot_pkg::command_center_msg command_center_msg;
        ros::spin();
        
        // release ball
        if (joint1.position + joint2.position >= 45) {
            release = true;
        }
        
        joint1.dutycycle = joint1.K_p*(q1-joint1.position) + joint1.K_d*(q1-joint1.position)
        joint2.dutycycle = joint2.K_p*(q2-joint2.position) + joint2.K_d*(q2-joint2.position)

        
        //update command center topic
		command_center_msg.command_center_time=command_center_time;
        command_center_msg.command_center_j1_dutycycle=joint1.dutycycle;
        command_center_msg.command_center_j1_direction=joint1.direction;
        command_center_msg.command_center_j2_dutycycle=joint2.dutycycle;
        command_center_msg.command_center_j2_direction=joint2.direction;
        command_center_msg.command_center_ = release

		ROS_INFO("time: %d  j1_dutycycle: %f j2_dutycycle: %f, j1_direction: %d,j2_direction: %d",time,joint1.dutycycle,joint2.dutycycle,j,joint2.direction);
		command_center_count++;
	}
	return 0;
}






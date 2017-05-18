//
//  sensing_node.cpp
//  
//
//  Created by Austin Burch on 11/1/16.
//
//

#include "link1_node.h"
//#include "BBB_lib.h"

using namespace std;

eQEP eqep;
PWM pwm;


double command_center_time =0;
int link1_count = 0;
// joint vars structure
struct Joint {
    float position = 0, speed = 0, accel = 0;
    float K_p=0, K_i=0, K_d=0;
    float e=0, P_val=0, I_val=0, D_val=0, PID_val=0;
    float prev_e=0, prev_I=0;
    int period=PERIOD_INIT,dutycycle=0,direction;
};
Joint joint1;

struct EndEff {
	float xPos;
	float yPos;
	float zPos;
	float vel;
    float vel_e, prev_vel_e;
    
};
EndEff gripper;


void command_center_msgCallback(const bbot_pkg::command_center_msg::ConstPtr& command_center_msg)
{
    command_center_time = command_center_msg->command_center_time;
    joint1.dutycycle = command_center_msg->command_center_j1_dutycycle;
    joint1.direction = command_center_msg->command_center_j1_direction;
    release = command_center_msg->command_center_release
}




int main(int argc,char **argv){
	ros::init(argc,argv,"link1_node");
	ros::NodeHandle link1_nh;
    //int SPS = 100;
	ros::Publisher link1_pub=link1_nh.advertise<bbot_pkg::link1_msg>("link1_msg_node",BUFF_SIZE);
    ros::Subscriber command_center_sub=command_center_nh.subscribe("command_center_msg_node",BUFF_SIZE,command_center_msgCallback);
	ros::Rate loop_rate(SPS);
    
    double j1_position_curr=0;
    double j1_position_prev=0;
    double j1_speed_curr=0;
    double j1_speed_prev=0;
    double j1_accel = 0;
    
    int eqep1_prev = 0;
    int eqep1_curr = 0;
       
    eqep1_prev = eqep.getValue(0);
    //eqep1_prev = 10;
    //eqep2_prev = 20;
    
	while(ros::ok())
	{
        i++;
        bbot_pkg::link1_msg link1_msg;
        link1_msg.link1_count=i;
        
        //sense
        eqep1_curr = eqep.getValue(0);
        
        //pos
        j1_position_curr = j1_position_prev + (eqep1_curr-eqep1_prev) * GEAR1_RATIO; // map to degrees
        //speed
        j1_speed_curr = (j1_position_curr - j1_position_prev) * SPS;
        //accel
        j1_accel = (j1_speed_curr - j1_speed_prev) * SPS;
        //reset vars
        eqep1_prev = eqep1_curr;

        j1_position_prev = j1_position_curr;
        
        j1_speed_prev = j1_speed_curr;
        
        // receive messages
        ros::spin();
        
        // pwm output mapping/limits
        pwm.reconfig(JOINT1_PWM_CHANNEL, joint1.period, joint1.dutycycle);
        //
        
        // release ball
        if (release == true) {
            while(joint1.position >= 0) {
                pwm.reconfig(JOINT1_PWM_CHANNEL, joint1.period, 1000000*0.1);
        
            }
        }
    
        //publish
        link1_msg.link1_position = j1_position_curr;
        link1_msg.link1_speed = j1_speed_curr;
        link1_msg.link1_accel = j1_accel;
        link1_msg.link1_dutycycle = j1_dutycycle;
        link1_msg.link1_period = j1_period;
        
		ROS_INFO("i: %d  j1_pos: %f  j1_speed: %f  j1_accel: %f  j1_dutycycle: %i ",i,j1_position_curr,j1_speed_curr,j1_accel_curr,j1_dutycycle);
		sensing_pub.publish(link1_msg);
		loop_rate.sleep();

	}
	return 0;

}







/////////////////////////////eQEP CLASS METHODS/////////////////////////



eQEP::eQEP() {
    cout << "eQEP Class Initiated" << endl;
    
    eQEP0Export = "/sys/devices/ocp/48304000.epwmss";
    eQEP0Unexport = "/sys/devices/ocp/48304000.epwmss";
    
    eQEP1Export = "/sys/devices/platform/ocp/48302000.epwmss";
    eQEP1Unexport = "/sys/devices/platform/ocp/48302000.epwmss";
    
    eQEP0Handle = NULL;
    eQEP0Position = "/sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position";
    
    eQEP1Handle = NULL;
    eQEP1Position = "/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/position";
    
    system("echo bone_eqep2b > /sys/devices/platform/bone_capemgr/slots");
    //system("echo bone_eqep2 > /sys/devices/platform/bone_capemgr/slots");
    
}


void eQEP::setup(int channel) {
    cout << "eQEP SETUP" << endl;
    
    switch (channel) {
        case 0:
        {
            
            if((eQEP0Handle=fopen(eQEP0Export,"ab"))!=NULL){
                fwrite("0",sizeof(char),1,eQEP0Handle);
                fclose(eQEP0Handle);
            }
            
            break;
        }
        case 1:
        {
            
            if((eQEP1Handle=fopen(eQEP1Export,"ab"))!=NULL){
                fwrite("1",sizeof(char),1,eQEP1Handle);
                fclose(eQEP1Handle);
            }
            break;
        }
            
        default:
            cout << "Channel Select outside of Range (0-1)" << endl;
    }
    
    
    
}



int eQEP::getValue(int channel) {
    
    cout << "eQEP READ JOINT " << channel+1 << " : ";
    
    eQEP0Value =0;
    eQEP1Value =0;
    
    char eQEP0Read[10]={0} ;
    char eQEP1Read[10]={0} ;
    
    switch (channel) {
        case 0:
        {
            if ((eQEP0Handle=fopen(eQEP0Position, "r+"))!=NULL){
                //cout <<" READING FILE...";
                fread(eQEP0Read,sizeof(char),10,eQEP0Handle);
                fclose(eQEP0Handle);
            }
            else {
                cout << "error reading file " ;
            }
            for(int i=0 ; i < 10 ; i ++ ){
                cout << eQEP0Read[i];
            }
            //	    cout << endl;
            stringstream ss(eQEP0Read);
            ss >> eQEP0Value;
            return eQEP0Value;
            break;
        }
        case 1:
        {
            if ((eQEP0Handle=fopen(eQEP1Position, "r+"))!=NULL){
                fread(eQEP1Read,sizeof(char),10,eQEP0Handle);
                fclose(eQEP0Handle);
            }
            for(int i=0 ; i < 10 ; i ++ ){
                cout << eQEP1Read[i];
            }
            //          cout << endl;
            stringstream ss(eQEP1Read);
            ss >> eQEP1Value;
            return eQEP1Value;
            break;
        }
            
        default:
            
            cout << "Channel Select outside of Range (0-1)" << endl;
            return 1;
    }
}




void eQEP::close(int channel)  {
    cout << "eQEP CLOSE" << endl;
    
    
    
    switch (channel) {
        case 0:
        {
            if((eQEP0Handle=fopen(eQEP0Unexport,"ab"))!=NULL){
                fwrite("0",sizeof(char),1,eQEP0Handle);
                fclose(eQEP0Handle);
            }
            
            break;
        }
        case 1:
        {
            if((eQEP1Handle=fopen(eQEP1Unexport,"ab"))!=NULL){
                fwrite("1",sizeof(char),1,eQEP1Handle);
                fclose(eQEP1Handle);
            }
            
            break;
        }
            
        default:
            
            cout << "Channel Select outside of Range (0-1)" << endl;
    }
    
    
}




/////////////////////////////PWM CLASS METHODS/////////////////////////


PWM::PWM() {
    PWMCape = "/sys/devices/platform/bone_capemgr/slots";
    
    PWMExport = "/sys/class/pwm/pwmchip0/export";
    
    PWM0Handle = NULL;
    PWM0Period = "/sys/class/pwm/pwmchip0/pwm0/period";
    PWM0Dutycycle = "/sys/class/pwm/pwmchip0/pwm0/duty_cycle";
    PWM0Enable = "/sys/class/pwm/pwmchip0/pwm0/enable";
    
    PWM1Handle = NULL;
    PWM1Period = "/sys/class/pwm/pwmchip0/pwm1/period";
    PWM1Dutycycle = "/sys/class/pwm/pwmchip0/pwm1/duty_cycle";
    PWM1Enable = "/sys/class/pwm/pwmchip0/pwm1/enable";
    
    PWM2Handle = NULL;
    PWM2Period = "/sys/class/pwm/pwmchip0/pwm2/period";
    PWM2Dutycycle = "/sys/class/pwm/pwmchip0/pwm2/duty_cycle";
    PWM2Enable = "/sys/class/pwm/pwmchip0/pwm2/enable";
    
    PWM3Handle = NULL;
    PWM3Period = "/sys/class/pwm/pwmchip0/pwm3/period";
    PWM3Dutycycle = "/sys/class/pwm/pwmchip0/pwm3/duty_cycle";
    PWM3Enable = "/sys/class/pwm/pwmchip0/pwm3/enable";
    
    PWMUnexport = "/sys/class/pwm/pwmchip0/unexport";
    
    system("echo BB-PWM2 > /sys/devices/platform/bone_capemgr/slots");
    printf("PWMCape ON\n");
    
}



int PWM::setup(int channel, int period, int dutycycle) {
    cout << "PWM SETUP" << channel << endl;
    
    switch (channel) {
        case 0:
        {
            
            if((PWM0Handle=fopen(PWMExport,"ab"))!=NULL){
                fwrite("0",sizeof(char),1,PWM0Handle);
                fclose(PWM0Handle);
            }
            if ((PWM0Handle=fopen(PWM0Period, "r+"))!=NULL){
                fwrite("1000000",sizeof(char),7,PWM0Handle);
                fclose(PWM0Handle);
            }
            
            if ((PWM0Handle=fopen(PWM0Dutycycle, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,PWM0Handle);
                fclose(PWM0Handle);
            }
            return 0;
            break;
        }
        case 1:
        {
            
            if((PWM1Handle=fopen(PWMExport,"ab"))!=NULL){
                fwrite("1",sizeof(char),1,PWM1Handle);
                fclose(PWM1Handle);
            }
            if ((PWM1Handle=fopen(PWM1Period, "r+"))!=NULL){
                fwrite("1000000",sizeof(char),7,PWM1Handle);
                fclose(PWM1Handle);
            }
            
            if ((PWM1Handle=fopen(PWM1Dutycycle, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,PWM1Handle);
                fclose(PWM1Handle);
            }
            return 0;
            
            break;
        }
            
        default:
            return 1;
            cout << "Channel Select outside of Range (0-1)" << endl;
    }
    
}



int PWM::reconfig(int channel, int period, int dutycycle) {
    
    cout << "PWM RECONFIG" << endl;
    
    char periodStr[20] = {0};
    char dutycycleStr[20] = {0};
    sprintf(periodStr,"%d",period);
    sprintf(dutycycleStr,"%d",dutycycle);
    
    switch (channel) {
        case 0:
        {
            if ((PWM0Handle=fopen(PWM0Period, "r+"))!=NULL){
                fwrite(periodStr,sizeof(char),strlen(periodStr),PWM0Handle);
                fclose(PWM0Handle);
            }
            if ((PWM0Handle=fopen(PWM0Dutycycle, "r+"))!=NULL){
                fwrite("0",sizeof(char),strlen(dutycycleStr),PWM0Handle);
                fclose(PWM0Handle);
            }
            return 0;
            break;
        }
        case 1:
        {
            if ((PWM1Handle=fopen(PWM1Period, "r+"))!=NULL){
                fwrite(periodStr,sizeof(char),strlen(periodStr),PWM1Handle);
                fclose(PWM1Handle);
            }
            if ((PWM1Handle=fopen(PWM1Dutycycle, "r+"))!=NULL){
                fwrite("0",sizeof(char),strlen(dutycycleStr),PWM1Handle);
                fclose(PWM1Handle);
            }
            return 0;
            break;
        }
            
        default:
            return 1;
            cout << "Channel Select outside of Range (0-1)" << endl;
    }
}



int PWM::start(int channel)  {
    cout << "PWM START" << endl;
    
    switch (channel) {
        case 0:
        {
            if ((PWM0Handle=fopen(PWM0Enable, "r+"))!=NULL){
                fwrite("1",sizeof(char),1,PWM0Handle);
                fclose(PWM0Handle);
            }
            return 0;
            break;
        }
        case 1:
        {
            if ((PWM1Handle=fopen(PWM1Enable, "r+"))!=NULL){
                fwrite("1",sizeof(char),1,PWM1Handle);
                fclose(PWM1Handle);
            }
            return 0;
            
            break;
        }
            
        default:
            cout << "Channel Select outside of Range (0-1)" << endl;
            return 1;
    }
    
}

void PWM::stop(int channel)  {
    cout << "PWM STOP" << endl;
    
    switch (channel) {
        case 0:
        {
            if ((PWM0Handle=fopen(PWM0Enable, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,PWM0Handle);
                fclose(PWM0Handle);
            }
            
            break;
        }
        case 1:
        {
            if ((PWM1Handle=fopen(PWM1Enable, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,PWM1Handle);
                fclose(PWM1Handle);
            }
            
            
            break;
        }
            
        default:
            
            cout << "Channel Select outside of Range (0-1)" << endl;
    }
    
    
}



void PWM::close(int channel)  {
    cout << "PWM CLOSE" << endl;
    
    switch (channel) {
        case 0:
        {
            if ((PWM0Handle=fopen(PWM0Enable, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,PWM0Handle);
                fclose(PWM0Handle);
            }
            if((PWM0Handle=fopen(PWMUnexport,"ab"))!=NULL){
                fwrite("0",sizeof(char),1,PWM0Handle);
                fclose(PWM0Handle);
            }
            
            break;
        }
        case 1:
        {
            if ((PWM1Handle=fopen(PWM1Enable, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,PWM1Handle);
                fclose(PWM1Handle);
            }
            if((PWM1Handle=fopen(PWMUnexport,"ab"))!=NULL){
                fwrite("1",sizeof(char),1,PWM1Handle);
                fclose(PWM1Handle);
            }
            
            
            break;
        }
            
        default:
            
            cout << "Channel Select outside of Range (0-1)" << endl;
    }
    
    
}





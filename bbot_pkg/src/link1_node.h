//
//  sensing_node.h
//  
//
//  Created by Austin Burch on 11/1/16.
//
//

#ifndef ____link1_node__
#define ____link1_node__

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sstream>

#include "ros/ros.h"
#include "bbot_pkg/link1_msg.h"
#include "bbot_pkg/link2_msg.h"
#include "bbot_pkg/command_center_msg.h"
//#include "/home/ubuntu/BBB_lib/BBB_lib.h"
//#include "BBB_lib.h"

#define SPS 100
#define BUFF_SIZE 100
#define GEAR1_RATIO 74.83
#define GEAR2_RATIO 74.83



class eQEP {
    
    const char *eQEP0Export;
    const char *eQEP0Unexport;
    const char *eQEP1Export;
    const char *eQEP1Unexport;
    
    FILE *eQEP0Handle;
    const char *eQEP0Enable;
    const char *eQEP0Position;
//    char *eQEP0Read;
    
    FILE *eQEP1Handle;
    const char *eQEP1Enable;
    const char *eQEP1Position;
//    char *eQEP1Read;
    
    
    int eQEP0Value;
    int eQEP1Value;
    
public:
    eQEP();
    void setup(int channel);
    int getValue(int channel);
    void close(int channel);
};




#define PERIOD_INIT 1000000
#define MAX 48
#define SPS 100
#define BUFF_SIZE 100
#define JOINT1_PWM_CHANNEL 0
#define JOINT2_PWM_CHANNEL 1

class PWM{
    
    const char *PWMCape;
    
    const char *PWMExport;
    const char *PWMUnexport;
    
    FILE *PWM0Handle;
    const char *PWM0Period;
    const char *PWM0Dutycycle;
    const char *PWM0Enable;
    
    FILE *PWM1Handle;
    const char *PWM1Period;
    const char *PWM1Dutycycle;
    const char *PWM1Enable;
    
    FILE *PWM2Handle;
    const char *PWM2Period;
    const char *PWM2Dutycycle;
    const char *PWM2Enable;
    
    FILE *PWM3Handle;
    const char *PWM3Period;
    const char *PWM3Dutycycle;
    const char *PWM3Enable;
    
public:
    PWM();
    int setup(int channel, int period, int dutycycle);
    int reconfig(int channel, int period, int dutycycle);
    int start(int channel);
    void stop(int channel);
    void close(int channel);
    
};



#endif /* defined(____link1_node__) */



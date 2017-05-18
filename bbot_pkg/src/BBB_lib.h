//
//  BBB_lib.h
//
//
//  Created by Austin Burch on 10/18/16.
//
//

#ifndef ____BBB_lib__
#define ____BBB_lib__

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sstream>


// include message files //

using namespace std;

//*****************************************************************/
// ADC Class
//*****************************************************************/

class ADC {
    FILE *ADChandle;
    const char *ADC1Voltage;
    const char *ADC2Voltage;
    const char *ADC3Voltage;
    const char *ADC4Voltage;
    const char *ADC5Voltage;
    const char *ADC6Voltage;
    char ADCVoltageRead;
    int ADCvalue;
    float Voltage;
public:
    ADC();
    int getValue(int channel);
    
};


//*******************************************************************/
//LED Class
//*******************************************************************/

class LED {
    FILE *LEDhandle;
    const char *LED0Brightness;
    const char *LED1Brightness;
    const char *LED2Brightness;
    const char *LED3Brightness;
public:
    LED();
    void setValue(char LEDbyte);
    void clean();
    
};

//****************M*************************************************/
// GPIO Class
//*****************************************************************/

#define MAX 24

class GPIO{
    FILE *GPIOhandle;
public:
    bool setValue(int pin, bool value);
    
};

//*******************************************************************/
//PWM Class
//*******************************************************************/

#define JOINT1_PWM_PIN  11
#define JOINT2_PWM_PIN  12

class PWM{
    
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

//*******************************************************************/
//eQEP ENCODER Class
//*******************************************************************/

#define JOINT1_eQEP_PIN  13
#define JOINT2_eQEP_PIN  14


class eQEP {
    
    const char *eQEPExport;
    const char *eQEPUnexport;
    
    FILE *eQEP0Handle;
    const char *eQEP0Enable;
    const char *eQEP0Position;
    char *eQEP0Read;
    
    FILE *eQEP1Handle;
    const char *eQEP1Enable;
    const char *eQEP1Position;
    char *eQEP1Read;
    
    
    int eQEP0Value;
    int eQEP1Value;
    
public:
    eQEP();
    void setup(int channel);
    int getValue(int channel);
    void close(int channel);
};



#endif /* defined(____BBB_lib__) */


//
//  BBB_lib.cpp
//
//
//  Created by Austin Burch on 10/18/16.
//
//

#include "BBB_lib.h"

//*******************************************************************
//ADC Methods
//*******************************************************************


ADC::ADC(void){ // Constructor
    cout << "ADC Setup Initiated" << endl;
    ADChandle=NULL;
    
    ADC1Voltage = "/sys/bus/iio/devices/iio:device0/in_voltage1_raw";
    ADC2Voltage = "/sys/bus/iio/devices/iio:device0/in_voltage2_raw";
    ADC3Voltage = "/sys/bus/iio/devices/iio:device0/in_voltage3_raw";
    ADC4Voltage = "/sys/bus/iio/devices/iio:device0/in_voltage4_raw";
    ADC5Voltage = "/sys/bus/iio/devices/iio:device0/in_voltage5_raw";
    ADC6Voltage = "/sys/bus/iio/devices/iio:device0/in_voltage6_raw";
    
    int ADCvalue = 0;
    int Voltage = 0;
    
}

int ADC::getValue(int channel){
    
    cout << "Reading ADC ... " ;
    char ADCVoltageRead[5] = {0};
    switch(channel){
		case 1:
            if ((ADChandle=fopen(ADC1Voltage, "r"))==NULL){
                printf("Error opening '/sys/bus/iio/devices/iio:device0/in_voltage1_raw'");
                return 0;
                
            }
            fread(ADCVoltageRead,sizeof(char),sizeof(ADCVoltageRead-1),ADChandle);
            break;
        case 2:
            if ((ADChandle=fopen(ADC2Voltage, "r"))==NULL){
                printf("Error opening '/sys/bus/iio/devices/iio:device0/in_voltage2_raw'");
                return 0;
                
            }
            break;
        case 3:
            if ((ADChandle=fopen(ADC3Voltage, "r"))==NULL){
                printf("Error opening '/sys/bus/iio/devices/iio:device0/in_voltage3_raw'");
                return 0;
                
            }
            break;
        case 4:
            if ((ADChandle=fopen(ADC4Voltage, "r"))==NULL){
                printf("Error opening '/sys/bus/iio/devices/iio:device0/in_voltage4_raw'");
                return 0;
                
            }
            break;
        case 5:
            if ((ADChandle=fopen(ADC5Voltage, "r"))==NULL){
                printf("Error opening '/sys/bus/iio/devices/iio:device0/in_voltage5_raw'");
                return 0;
                
            }
            break;
        case 6:
            if ((ADChandle=fopen(ADC6Voltage, "r"))==NULL){
                printf("Error opening '/sys/bus/iio/devices/iio:device0/in_voltage6_raw'");
                return 0;
                
            }
            break;
        default:
            break;
    }
    
    fread(ADCVoltageRead,sizeof(char),sizeof(ADCVoltageRead-1),ADChandle);
    fclose(ADChandle);
    stringstream ss(ADCVoltageRead);
    
    ss >> Voltage;
    cout << Voltage << endl;
    //printf("%s",ADCVoltageRead);
    //usleep(50000);
    //convert ADC value or voltage, change to float value?
    //Voltage= (float)ADCvalue*1.8/4096;
    //Voltage = ADCvalue;
    //printf("%f (V) \n",  Voltage);
    return Voltage;
}
//*******************************************************************
//LED Methods
//*******************************************************************



LED::LED() { //Constructor ;
    cout << "LED Setup Initiated" << endl;
    LEDhandle = NULL;
    
    LED0Brightness = "/sys/class/leds/beaglebone:green/usr0/brightness";
    LED1Brightness = "/sys/class/leds/beaglebone:green/usr1/brightness";
    LED2Brightness = "/sys/class/leds/beaglebone:green/usr2/brightness";
    LED3Brightness = "/sys/class/leds/beaglebone:green/usr3/brightness";
    
}

void LED::setValue(char LEDbyte) {
    cout << "Setting LED Values..." << endl;
    
    
    if (LEDbyte && 0x01 > 0x00) {
        if ((LEDhandle=fopen(LED0Brightness, "r+"))!=NULL){
            fwrite("1",sizeof(char),1,LEDhandle);
            fclose(LEDhandle);
        }
        else {
            if ((LEDhandle=fopen(LED0Brightness, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,LEDhandle);
                fclose(LEDhandle);
            }
        }
        
    }
    
    if (LEDbyte && 0x02 > 0x00) {
        if ((LEDhandle=fopen(LED1Brightness, "r+"))!=NULL){
            fwrite("1",sizeof(char),1,LEDhandle);
            fclose(LEDhandle);
        }
        else {
            if ((LEDhandle=fopen(LED1Brightness, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,LEDhandle);
                fclose(LEDhandle);
            }
        }
        
    }
    
    if (LEDbyte && 0x04 > 0x00) {
        if ((LEDhandle=fopen(LED2Brightness, "r+"))!=NULL){
            fwrite("1",sizeof(char),1,LEDhandle);
            fclose(LEDhandle);
        }
        else {
            if ((LEDhandle=fopen(LED2Brightness, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,LEDhandle);
                fclose(LEDhandle);
            }
        }
        
    }
    
    if (LEDbyte && 0x08 > 0x00) {
        if ((LEDhandle=fopen(LED3Brightness, "r+"))!=NULL){
            fwrite("1",sizeof(char),1,LEDhandle);
            fclose(LEDhandle);
        }
        else {
            if ((LEDhandle=fopen(LED3Brightness, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,LEDhandle);
                fclose(LEDhandle);
            }
        }
        
    }
}

void LED::clean() {
    
    cout << "Cleaning up LEDs..." << endl;
    
    if ((LEDhandle=fopen(LED0Brightness, "r+"))!=NULL){
        fwrite("0",sizeof(char),1,LEDhandle);
        fclose(LEDhandle);
    }
    
    if ((LEDhandle=fopen(LED0Brightness, "r+"))!=NULL){
        fwrite("0",sizeof(char),1,LEDhandle);
        fclose(LEDhandle);
    }
    if ((LEDhandle=fopen(LED0Brightness, "r+"))!=NULL){
        fwrite("0",sizeof(char),1,LEDhandle);
        fclose(LEDhandle);
    }
}

//*******************************************************************
//GPIO Methods
//*******************************************************************


bool GPIO::setValue(int pin, bool value) {
    
    //init
    cout << "GPIO PIN setup" << endl;
    GPIOhandle=NULL;
    char setValue[4], GPIOString[4], GPIOValue[MAX], GPIODirection[MAX];
    sprintf(GPIOString, "%d", pin);
    sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value",pin);
    sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction",pin);
    
    //export 60
    if ((GPIOhandle=fopen("/sys/class/gpio/export", "ab"))==NULL){
        printf("Cannot export the GPIO pin \n");
        return 1;
    }
    strcpy(setValue,GPIOString);
    fwrite(&setValue, sizeof(char),2,GPIOhandle);
    fclose(GPIOhandle);
    
    //direction
    if ((GPIOhandle=fopen(GPIODirection, "rb+"))==NULL){
        printf("Cannot open direction handle \n");
        return 1;
    }
    strcpy(setValue,"out");
    fwrite(&setValue, sizeof(char),3,GPIOhandle);
    fclose(GPIOhandle);
    
    //setvalue
    if ((GPIOhandle=fopen(GPIOValue, "rb+"))==NULL){
        printf("Cannot open value handle \n");
        return 1;
    }
    cout << &GPIOhandle << endl;
    if (value == 0)
        strcpy(setValue,"0");
    else
        strcpy(setValue,"1");
    fwrite(&setValue,sizeof(char),1,GPIOhandle);
    //fprintf(LEDHandle, "%d",1);
    fclose(GPIOhandle);
    
    //unexport
    if ((GPIOhandle=fopen("/sys/class/gpio/unexport", "ab"))==NULL){
        printf("Cannot unexport the GPIO pin \n");
        return 1;
    }
    strcpy(setValue,GPIOString);
    fwrite(&setValue,sizeof(char),2,GPIOhandle);
    fclose(GPIOhandle);
    
    cout << "GPIO LED Flash Pin:" << pin << "end" << endl;
    return 0;
    
}

//*******************************************************************
//PWM Methods
//*******************************************************************

PWM::PWM() {
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
}





int PWM::setup(int channel, int period, int dutycycle) {
    cout << "PWM SETUP" << endl;
    
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
        case 2:
        {
            
            if((PWM2Handle=fopen(PWMExport,"ab"))!=NULL){
                fwrite("2",sizeof(char),1,PWM2Handle);
                fclose(PWM2Handle);
            }
            if ((PWM2Handle=fopen(PWM2Period, "r+"))!=NULL){
                fwrite("1000000",sizeof(char),7,PWM2Handle);
                fclose(PWM2Handle);
            }
            
            if ((PWM2Handle=fopen(PWM2Dutycycle, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,PWM2Handle);
                fclose(PWM2Handle);
            }
            return 0;
            
            break;
        }
        case 3:
        {
            
            if((PWM3Handle=fopen(PWMExport,"ab"))!=NULL){
                fwrite("3",sizeof(char),1,PWM3Handle);
                fclose(PWM3Handle);
            }
            if ((PWM3Handle=fopen(PWM3Period, "r+"))!=NULL){
                fwrite("1000000",sizeof(char),7,PWM3Handle);
                fclose(PWM0Handle);
            }
            
            if ((PWM3Handle=fopen(PWM3Dutycycle, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,PWM3Handle);
                fclose(PWM3Handle);
            }
            return 0;
            
            break;
        }
            
        default:
            return 1;
            cout << "Channel Select outside of Range (0-3)" << endl;
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
        case 2:
        {
            if ((PWM2Handle=fopen(PWM2Period, "r+"))!=NULL){
                fwrite(periodStr,sizeof(char),strlen(periodStr),PWM2Handle);
                fclose(PWM2Handle);
            }
            if ((PWM2Handle=fopen(PWM2Dutycycle, "r+"))!=NULL){
                fwrite("0",sizeof(char),strlen(dutycycleStr),PWM2Handle);
                fclose(PWM2Handle);
            }
            return 0;
            break;
        }
        case 3:
        {
            if ((PWM3Handle=fopen(PWM3Period, "r+"))!=NULL){
                fwrite(periodStr,sizeof(char),strlen(periodStr),PWM3Handle);
                fclose(PWM0Handle);
            }
            if ((PWM3Handle=fopen(PWM3Dutycycle, "r+"))!=NULL){
                fwrite("0",sizeof(char),strlen(dutycycleStr),PWM3Handle);
                fclose(PWM3Handle);
            }
            return 0;
            break;
        }
            
        default:
            return 1;
            cout << "Channel Select outside of Range (0-3)" << endl;
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
        case 2:
        {
            if ((PWM2Handle=fopen(PWM2Enable, "r+"))!=NULL){
                fwrite("1",sizeof(char),1,PWM2Handle);
                fclose(PWM2Handle);
            }
            return 0;
            
            break;
        }
        case 3:
        {
            if ((PWM3Handle=fopen(PWM3Enable, "r+"))!=NULL){
                fwrite("1",sizeof(char),1,PWM3Handle);
                fclose(PWM3Handle);
            }
            return 0;
            break;
        }
            
        default:
            cout << "Channel Select outside of Range (0-3)" << endl;
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
        case 2:
        {
            if ((PWM2Handle=fopen(PWM2Enable, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,PWM2Handle);
                fclose(PWM2Handle);
            }
            
            
            break;
        }
        case 3:
        {
            if ((PWM3Handle=fopen(PWM3Enable, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,PWM3Handle);
                fclose(PWM3Handle);
            }
            
            
            break;
        }
            
        default:
            
            cout << "Channel Select outside of Range (0-3)" << endl;
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
        case 2:
        {
            if ((PWM2Handle=fopen(PWM2Enable, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,PWM2Handle);
                fclose(PWM2Handle);
            }
            if((PWM2Handle=fopen(PWMUnexport,"ab"))!=NULL){
                fwrite("2",sizeof(char),1,PWM2Handle);
                fclose(PWM2Handle);
            }
            
            
            break;
        }
        case 3:
        {
            if ((PWM3Handle=fopen(PWM3Enable, "r+"))!=NULL){
                fwrite("0",sizeof(char),1,PWM3Handle);
                fclose(PWM3Handle);
            }
            if((PWM3Handle=fopen(PWMUnexport,"ab"))!=NULL){
                fwrite("3",sizeof(char),1,PWM3Handle);
                fclose(PWM3Handle);
            }
            
            
            break;
        }
            
        default:
            
            cout << "Channel Select outside of Range (0-3)" << endl;
    }
    
    
}


eQEP::eQEP() {
    cout << "eQEP Class Initiated" << endl;
    
    eQEPExport = "/sys/devices/plarform/ocp/48304000.epwmss";
    eQEPUnexport = "/sys/devices/plarform/ocp/48304000.epwmss";
    
    eQEP0Handle = NULL;
    eQEP0Position = "/sys/devices/plarform/ocp/48304000.epwmss/48304180.eqep/position";
    
    eQEP1Handle = NULL;
    eQEP1Position = "/sys/devices/plarform/ocp/48304000.epwmss/48304180.eqep/position";
    
}


void eQEP::setup(int channel) {
    cout << "eQEP SETUP" << endl;
    
    switch (channel) {
        case 0:
        {
            
            if((eQEP0Handle=fopen(eQEPExport,"ab"))!=NULL){
                fwrite("0",sizeof(char),1,eQEP0Handle);
                fclose(eQEP0Handle);
            }
            
            break;
        }
        case 1:
        {
            
            if((eQEP1Handle=fopen(eQEPExport,"ab"))!=NULL){
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
    
    cout << "eQEP READ" << endl;
    
    eQEP0Value =0;
    eQEP1Value =0;
    
    eQEP0Read[10] ;
    eQEP1Read[10] ;
    
    switch (channel) {
        case 0:
        {
            if ((eQEP0Handle=fopen(eQEP0Position, "r+"))!=NULL){
                fread(eQEP0Read,sizeof(char),10,eQEP0Handle);
                fclose(eQEP0Handle);
            }
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
            if((eQEP0Handle=fopen(eQEPUnexport,"ab"))!=NULL){
                fwrite("0",sizeof(char),1,eQEP0Handle);
                fclose(eQEP0Handle);
            }
            
            break;
        }
        case 1:
        {
            if((eQEP1Handle=fopen(eQEPUnexport,"ab"))!=NULL){
                fwrite("1",sizeof(char),1,eQEP1Handle);
                fclose(eQEP1Handle);
            }
            
            break;
        }
            
        default:
            
            cout << "Channel Select outside of Range (0-1)" << endl;
    }
    
    
}




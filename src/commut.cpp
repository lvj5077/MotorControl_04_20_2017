#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> /* File control definitions */
#include <errno.h>
#include <termios.h> /* POSIX terminal control definitions */


#include "SerialPortControl_stream.hpp"
/*********************
500 000 tick per sec by experiment...>>
*************************/



int main()
{
	std::cout<<"Hello ~~~~~~~~~~~~~~~~"<<std::endl;	
	SerialPortControl myMotor(9600,"/dev/ttyUSB0");
	std::cout<<"set up: checked"<<std::endl;	

	myMotor.writeEnable();

	std::cout<<"enable: checked"<<std::endl;	

	int resPrd = myMotor.readEncoderResolution();

	std::cout<<resPrd<<" per 3.60 degree"<<std::endl;	

	resPrd = 2048;
	myMotor.writeHome();
	sleep(1);
	std::cout<<"home: checked"<<std::endl;	


	int targetPos;
	int curPos;
	curPos = myMotor.readEncoder();
	std::cout<<"0 current position is "<<curPos<<" counts"<<std::endl;		
	std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<std::endl;
	std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<std::endl;


	targetPos = 100*resPrd;
	myMotor.writePoseAbsolute(targetPos);
	sleep(3);

	curPos = myMotor.readEncoder();
	std::cout<<"1 current position is "<<curPos<<" counts"<<std::endl;		
	std::cout<<"erro = " <<curPos-targetPos <<std::endl;
	std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<std::endl;


	targetPos = -300*resPrd;
	myMotor.writePoseAbsolute(targetPos);
	//sleep(3);
	curPos = myMotor.readEncoder();
	std::cout<<"2 current position is "<<curPos<<" counts"<<std::endl;		
	std::cout<<"erro = " <<curPos-targetPos <<std::endl;
	std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<std::endl;

	targetPos = 12000*resPrd;
	myMotor.writePoseAbsolute(targetPos);
	//sleep(3);
	curPos = myMotor.readEncoder();
	std::cout<<"3 current position is "<<curPos<<" counts"<<std::endl;		
	std::cout<<"erro = " <<curPos-targetPos <<std::endl;
	std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<std::endl;
	//sleep(10);


	targetPos = 00*resPrd;
	myMotor.writePoseAbsolute(targetPos);
	//sleep(3);
	curPos = myMotor.readEncoder();
	std::cout<<"4 current position is "<<curPos<<" counts"<<std::endl;		
	std::cout<<"erro = " <<curPos-targetPos <<std::endl;
	std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<std::endl;

	targetPos = 100*resPrd;
	myMotor.writePoseRelative(targetPos);
	curPos = myMotor.readEncoder();
	std::cout<<"5 current position is "<<curPos<<" counts"<<std::endl;		
	std::cout<<"erro = " <<curPos-targetPos <<std::endl;
	std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<std::endl;
	
	sleep(2);
	targetPos = -100*resPrd;
	myMotor.writePoseRelative(targetPos);
	curPos = myMotor.readEncoder();
	std::cout<<"6 current position is "<<curPos<<" counts"<<std::endl;		
	std::cout<<"erro = " <<curPos <<std::endl;
	std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<std::endl;

}

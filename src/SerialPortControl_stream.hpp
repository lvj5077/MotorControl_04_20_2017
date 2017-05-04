#ifndef _SERIALPORTCONTROL_HPP
#define _SERIALPORTCONTROL_HPP
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <deque>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <SerialStream.h>

#include <boost/lexical_cast.hpp>

class SerialPortControl{
	protected :
	//ATTRIBUTS 

	double _nTick; // ??? still have no idea of what this parameter is

	LibSerial::SerialStream _motor;

	int maxspeed = 100; // check the faulhaber document!!!

	int _enco;
	float _targetSpeed; 
	float _measuredSpeed; 

	bool _verbose;
	bool _readcorrectly;

	std::string PORT = "/dev/ttyUSB0";
	double Baud = 9600;	

	public:
	SerialPortControl(double , std::string){

		std::cout << "The motor is=> Baud "<< Baud<<" Port "<< PORT<<std::endl;
		_motor.Open(PORT);		

		 if ( ! _motor.good() ){
			std::cerr << "Error: Could not open serial port "<<PORT<< std::endl ;
			exit(1) ;
		}
		
		if(Baud==115200){
			_motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_115200);
			std::cout<<"Going for value of 115200"<<std::endl;
		}
		else if(Baud==1200){
			_motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_1200);
			std::cout<<"Going for value of 1200"<<std::endl;
		}
		else if(Baud==2400){
			_motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_2400);
			std::cout<<"Going for value of 2400"<<std::endl;
		}
		else if(Baud==19200){
			_motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_19200);
			std::cout<<"Going for value of 19200"<<std::endl;
		}
		else if(Baud==38400){
			_motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_38400);
			std::cout<<"Going for value of 38400"<<std::endl;
		}
		else if(Baud==57600){
			_motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_57600);
			std::cout<<"Going for value of 57600"<<std::endl;
		}
		else if(Baud==9600){
			_motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_9600);
			std::cout<<"Going for default value of 9600"<<std::endl;
		}
		else{
			std::cout<<"Value incorrect. Going for default value of 9600"<<std::endl;
			_motor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_9600);
		}
			  
		if ( ! _motor.good() ){
			std::cerr << "Error: Could not set the baud rate." <<std::endl;
			exit(1);
		}		
		
		
		_motor.SetCharSize(LibSerial::SerialStreamBuf::CHAR_SIZE_8);
		if ( ! _motor.good() ){
			std::cerr << "Error: Could not set the char size." <<std::endl;
			exit(1);
		}
		_motor.SetParity( LibSerial::SerialStreamBuf::PARITY_NONE ) ;
		if ( ! _motor.good() ){
			std::cerr << "Error: Could not set the parity." <<std::endl;
			exit(1);
		}
		_motor.SetFlowControl(LibSerial::SerialStreamBuf::FLOW_CONTROL_NONE ) ;
		if ( ! _motor.good() ){
			std::cerr << "Error: Could not set the control." <<std::endl;
			exit(1);
		}
		_motor.SetNumOfStopBits(1) ;
		if ( ! _motor.good() ){
			std::cerr << "Error: Could not set the stopbit." <<std::endl;
			exit(1);
		}
		
		//setMax Min speed
		writeDisable(); //Stop the motor in case the robot was still moving
		writeEnable();
		writeHome(); //Set the initial position to zero ;)

		writeTargetSpeed(50);
		//writeMaxSpeed(5);
		//writeMinSpeed(-maxspeed);

	}
	
	~SerialPortControl(){
		_motor.Close();
	}
	
	/**************************************************
	Accessors
	**************************************************/
	int getEncoder(){return _enco;}
	float getTargetSpeed(){return _targetSpeed;}
	float getMeasuredSpeed(){return _measuredSpeed;}
	LibSerial::SerialStream& getMotor(){return _motor;}
	double getTickNumber(){return _nTick;}
	bool getReadState(){return _readcorrectly;}
	/**************************************************
	Read values in the controller
	**************************************************/
	int readEncoder(); 
	int readTargetSpeed();
	int readRealSpeed();
	int readEncoderResolution();

	/**************************************************
	Set values Manually
	**************************************************/	
	void setTargetSpeed(float s){_targetSpeed=s;}
	void setVerbose(){_verbose=true;}
	void setReadState(){_readcorrectly=true;}
	
	/**************************************************
	Write values in the controller
	**************************************************/
	void writeEnable();
	void writeDisable();
	void writeGoEncoderIndex();
	void writeHome();
	void writeAcc(int acc);
	void writeDec(int dec);

	void writeMaxSpeed(int ms){
		try{
			std::string MaxSpeed=boost::lexical_cast<std::string>(ms);
			MaxSpeed="SP"+MaxSpeed+"\n";
			writePort(MaxSpeed);
		}
		catch(boost::bad_lexical_cast& blc){
		  std::cout << "Exception in Max Speed" << blc.what() << std::endl;
		  _readcorrectly=false;
	//	  scanf("%d",&testin);
		}
	}
	void writeMinSpeed(int ms){
		try{
			std::string MaxSpeed=boost::lexical_cast<std::string>(ms);
			MaxSpeed="MV"+MaxSpeed+"\n";
			writePort(MaxSpeed);
		}
		catch(boost::bad_lexical_cast& blc){
		  std::cout << "Exception in Min Speed" << blc.what() << std::endl;
	//	  scanf("%d",&testin);
		  _readcorrectly=false;
		}
	}

	void writeTargetSpeed(int ms){
		try{
			std::string MaxSpeed=boost::lexical_cast<std::string>(ms);
			MaxSpeed="V"+MaxSpeed+"\n";
			writePort(MaxSpeed);
		}
		catch(boost::bad_lexical_cast& blc){
		  std::cout << "Exception in Right wheel Speed" << blc.what() << std::endl;
	//	  scanf("%d",&testin);
		  _readcorrectly=false;
		}
	}

	void writePoseRelative(int ms){
		try{
			std::string pos=boost::lexical_cast<std::string>(ms);
			pos="LR"+pos+"\n";
			writePort(pos);
			int targetPos = ms+readEncoder();

			motorMove(targetPos);
		}
		catch(boost::bad_lexical_cast& blc){
		  std::cout << "Exception in Relative Right position" << blc.what() << std::endl;
		 // scanf("%d",&testin);
		  _readcorrectly=false;
		}
	}

	void writePoseAbsolute(int ms){
		try{
			std::string pos=boost::lexical_cast<std::string>(ms);
			pos="LA"+pos+"\n";

			writePort(pos);
			// std::cout << "CMD = "<< pos<<std::endl;
			// pos="V2\n";
			// writePort(pos);
			motorMove(ms);
		}
		catch(boost::bad_lexical_cast& blc){
		  std::cout << "Exception in Absolute Right Position" << blc.what() << std::endl;
		  //scanf("%d",&testin);
		  _readcorrectly=false;
		}

	}


	void motorMove(int targetPos){
		std::string pos="M\n";
		writePort(pos);
		//std::cout << "CMD = "<< pos<<std::endl;
		//sleep(4);
		int curPos = readEncoder();
		//std::cout << "curPos = "<< curPos<<std::endl;
		//sstd::cout << "targetPos = "<< targetPos<<std::endl;
		int stopCode = 0;
		int lastPos = 0;
		while((curPos-targetPos)>10 || (curPos-targetPos)<-10 ){
			usleep(50);
			lastPos = curPos;
			curPos = readEncoder();
			//std::cout << "curPos = "<< curPos<<std::endl;
			//std::cout << "lastPos = "<< lastPos<<std::endl;

			if((curPos-lastPos)<2 && (curPos-lastPos)>-2 ){
				stopCode ++;
			}
			else{
				stopCode = 0;
			}

			if(stopCode > 5000){
				pos="V0\n";
				writePort(pos);
				std::cout << "Stopped!!!"<< std::endl;
				if ((curPos-targetPos)>10 || (curPos-targetPos)<-10){
					std::cout << "curPos = "<< curPos<<std::endl;
					std::cout << "targetPos = "<< lastPos<<std::endl;
					std::cout << "Stuck!!!"<< std::endl;
					for (int i=0 ; i<5; i++){
						std::cout << 5-i << std::endl;
						sleep(1);
					}
					pos="EN0\n";
					writePort(pos);

						
				}
				
				break;
			}
		}
	}


	void writeMove(){
		std::string s="M\n";
	}

	/**************************************************
	Basic serial operations
	**************************************************/
	void writePort(std::string& str);
	int readPort();
};

/*********************************************************************************************************
FUNCTIONS
*********************************************************************************************************/

/********************************Read values in the controller**************************************/


inline int SerialPortControl::readEncoder(){
	std::string yo("POS\n");
	writePort(yo);
	return readPort();
}

inline int SerialPortControl::readEncoderResolution(){
	std::string yo("GENCRES\n");
	writePort(yo);
	std::string i;
	_motor>> i;
	while(_motor.rdbuf()->in_avail() >0){
		_motor>> i;
		//std::cout <<i<<std::endl;
		try{
			return boost::lexical_cast<int>(i);
		}
		catch(boost::bad_lexical_cast& blc){
		  continue;
		}
	}

}


inline int SerialPortControl::readTargetSpeed(){
	std::string tSpeed("GV\n");
	writePort(tSpeed);
	return readPort();
}


inline int SerialPortControl::readRealSpeed(){	
	std::string rSpeed("GN\n");
	writePort(rSpeed);
	return readPort();
}

/*****************************CFULHABER CONTROLER CONFIGURATION*******************************/

inline void SerialPortControl::writeEnable(){
	std::string enable("en\n");
	SerialPortControl::writePort(enable);
	std::cout << "motor enabled " << std::endl;
}

inline void SerialPortControl::writeDisable(){
	std::string enable("di\n");
	SerialPortControl::writePort(enable);
	std::cout << "motor disabled " << std::endl;
}

inline void SerialPortControl::writeGoEncoderIndex(){
	std::string enable("GOIX\n");
	SerialPortControl::writePort(enable);
}
inline void SerialPortControl::writeHome(){
	std::string enable("HO\n");
	SerialPortControl::writePort(enable);
}
inline void SerialPortControl::writeAcc(int acc){
	std::string accc=boost::lexical_cast<std::string>(acc);
	std::string enable("AC"+accc+"\n");
	SerialPortControl::writePort(enable);
}
inline void SerialPortControl::writeDec(int dec){
	std::string decc=boost::lexical_cast<std::string>(dec);
	std::string enable("DEC"+decc+"\n");
	SerialPortControl::writePort(enable);
}

/****************************Basic functon*******************************************/

inline void SerialPortControl::writePort(std::string& str){
	_motor << str;//lpBufferToWrite;
	// if(_verbose==true){
	// 	std::cout << "wrote the demand: "<<str.c_str()<< std::endl;
	// }
}

inline int SerialPortControl::readPort(){
	std::string i;
	while(_motor.rdbuf()->in_avail() >0){
		_motor>> i;
		//std::cout <<i<<std::endl;
		try{
			return boost::lexical_cast<int>(i);
		}
		catch(boost::bad_lexical_cast& blc){
		  continue;
		}
	}	
}

#endif

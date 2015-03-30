/*
 *  threadedSerial.h
 *  oscSerialSender
 *
 *  Created by jasch on 9/11/11.
 *  Copyright 2011 j45ch. All rights reserved.
 *
 */


#ifndef _THREADED_OBJECT
#define _THREADED_OBJECT

#include "ofMain.h"

#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "sys/time.h"

//#define RAD_TO_DEG 180.0 / PI  // 57.295792

#define SENSOR_PATTERNLENGTH_1 60

class threadedSerial : public ofThread
{
	
public:
	
	threadedSerial();
	~threadedSerial();
	
	void start();
	void stop();
	void threadedFunction();
    void readSerial();
	void serialparse(unsigned char *c);
    void parsePacket();
    
	void sendOSC();
	void draw();
    
    void eulerToAngleAxis();
    void quatToAngleAxis();
    void quatToEuler();
    void drawCube();

	ofSerial	serial;
		
	ofTrueTypeFont TTF;
	ofxOscSender sender;
	
	vector <ofSerialDeviceInfo> deviceList;

	string		serialport;
	int			baudrate;
	string		sendIP;
	int			sendport;		
	
	char		bytesRead[3];				// data from serial, we will be trying to read 3
	char		bytesReadString[4];			// a string needs a null terminator, so we need 3 + 1 bytes
	int			nBytesRead;					// how much did we read?
	int			nTimesRead;					// how many times did we read?
	float		readTime;                   // when did we last read?
	
    
	unsigned char serialStream[SENSOR_PATTERNLENGTH_1]; // the size of the largest expected packet
	int         streamSize;
    
	unsigned char input[SENSOR_PATTERNLENGTH_1];    // working buffer for each packet type
	bool		haveInput;				// flags to signal a full MPU packet was parsed successfully
		
    int         numImuAddr;
    int         numThumbsAddr;
    int         numSysAddr;
		
	bool		status;
	int			displaySpeed;
	int			display;
	bool		windowChanged;
    bool        drawValues;
	
	// OSC sender stuff
	string		mpuaddresses[24];
    string      thumbsaddresses[24];
	string		systemaddresses[24];
    ofxOscMessage m[24];
	
	long		systime;
	long		oldSystime;
	long		deltaTime;
    long		timestamp;
	
	bool		sendFlat;
    
    long        quatRaw[4];
    double      quaternion[4];
    double      angleAxis[4];
    double      euler[3];
    double      ypr[3];
    
    long        accelRaw[3];
    double      accel[3];
    long        gyroRaw[3];
    double      gyro[3];
    double      summedIMU[3];
    
    int         joystickRaw[3];
    double      joystick[3];
    int         oldJoySw;
    bool        joySwChanged;
    int         trackballRaw[3];
    double      trackball[3];
    int         oldTbSw;
    bool        tbSwChanged;

};

#endif

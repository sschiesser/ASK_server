#pragma once

#include "ofMain.h"

#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#include "threadedSerial.h"

//--------------------------------------------------------
class testApp : public ofSimpleApp
{
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void startSerial();
	void stopSerial();
	void getSerialDeviceList();
	void startNetsender();
	
	void keyReleased(int key);	

	void writePrefs();
	void dumpPrefs();
	bool readPrefs();
	void readMidicodes();
	void readAxistable();
	
	void resetCalibrate();

	void writeScaling();

	void receiveOSC();
	
	void mouseMoved( int x, int y);
	void mouseDragged( int x, int y, int button);
	void mousePressed( int x, int y, int button);
	void mouseReleased();
	
	// class vars
	threadedSerial	* serialThreadObject;
	ofxOscReceiver	receiver;	
	
	ofSerial serial;
	
	ofxXmlSettings XML;

	
	ofTrueTypeFont TTF;
	ofTrueTypeFont TTFsmall;
	ofTexture texScreen;
	
	int		framerate;
	bool	status;
	int		display;
	bool	windowChanged;
	
	int		receiveport;
	
	bool	prefStatus;
	string	status1;
	string	status2;
	string	status3;
	string	tempStr[1024];

	bool	senderStatus;
	bool	drawValues;

	float	timeOut;
	float	lastTime;
	
	bool	runOnce;
	float	runOnceDelay;
	float	runOnceStart;
	
	bool	redrawFlag;
	float	redrawInterval;
	float	lastRedraw;
	float   redrawValues[6];
	bool	firstflag;
	bool	drawTex;
	
	bool	menuState;
	int		numMenuItems;
	ofPoint menuClickPos;
	
		
};
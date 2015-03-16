#include "testApp.h"

void testApp::setup()
{
	ofSetEscapeQuitsApp(false);
	
	serialThreadObject = new(threadedSerial);
	
	ofTrueTypeFont::setGlobalDpi(72);

	TTF.loadFont("inconsolata.ttf", 12, true);
	TTF.setLineHeight(16);
	TTF.setLetterSpacing(0.98);
	TTF.setSpaceSize(0.5);


	TTFsmall.loadFont("inconsolata.ttf", 12, true);
	TTFsmall.setLineHeight(16.0f);
//	TTFsmall.setLetterSpacing(1.03);	
	
	
	serialThreadObject->TTF.loadFont("inconsolata.ttf", 12, true);
	serialThreadObject->TTF.setLineHeight(16.0f);
//	serialThreadObject->TTF.setLetterSpacing(1.03);
	
	texScreen.allocate(440, 681, GL_RGB);
	
	windowChanged = 1;
	drawValues = 0;
	menuState = 0;
	
	serialThreadObject->serialport = "/dev/tty.usbserial-A7005Ghs";
	serialThreadObject->baudrate = 115200;
	serialThreadObject->sendIP = "127.0.0.1";
	serialThreadObject->sendport = 40000;
	
	receiveport = 40001;
//	serialThreadObject->debounceTimeOut = 0;
	serialThreadObject->sendFlat = 0;


	status = readPrefs();
	if(status) {
		status3 = "Success reading settings in \"ASKServer.xml\"";
	}else{
		status3 = "Failed reading opening file \"ASKServer.xml\" ";
	}
	dumpPrefs();
//	readAxistable();
	lastTime = ofGetElapsedTimef();
	
	// open an outgoing connection to sendIP:PORT	
	receiver.setup( receiveport );
	
	framerate = 25;
	ofSetFrameRate( framerate ); // cap the glut callback rate
	ofSetVerticalSync( true );
	//ofEnableAlphaBlending();
	ofBackground( 250, 250, 250);	
	//ofSetBackgroundAuto(false);
	redrawFlag = 1;
	// redrawInterval = redrawValues[display]; // in seconds
	firstflag = 1;
    
    serialThreadObject->switchValue = 0;
    serialThreadObject->oldSwitchValue = 0;
    serialThreadObject->switchValueChanged = false;
	
	runOnce = 1;
	runOnceDelay = 1.0f;
	lastRedraw = runOnceStart = ofGetElapsedTimef();
 	getSerialDeviceList();	
	ofSetWindowPosition(0,44);
}

void testApp::update()
{
	float now = ofGetElapsedTimef();
	
	if(runOnce == 1) {
		if(now > (runOnceStart + runOnceDelay) ) {
//			startSerial();
			startNetsender();
			runOnce = 0;
		}
	}
}

void testApp::draw()
{
	int i;
	int anchorx = 15;
	int anchory = 74;	
	int stepsize = 16;
	int columnwidth = 200;
	int rightColumn = 295;
	int leftColumn = 105;
	int width;
	int height;
	double yy;
	
	if(windowChanged == 1) {
		if(drawValues == 0) {
			width = 430;
			height = 60;
			ofSetWindowShape(width, height);
			windowChanged = 0;
		} else if(drawValues == 1) {
			width = 430;
			height = 180;
			timeOut = 5.0;
			ofSetWindowShape(width, height);
			windowChanged = 0;
		}
	}
	
	if(redrawFlag == 1) // drawn once after first update
	{
		ofSetColor(250, 250, 250, 255);
		ofRect(0, 0, width, height);
		ofSetColor(0, 0, 0, 255);

		TTF.drawString(status1, anchorx, 26);
		TTF.drawString(status2, anchorx,  48);
		
		ofSetColor(250, 250, 250, 255);
		
		// Menu
		ofFill();
		ofSetColor(232, 232, 232);
		ofRect(leftColumn, 13, 188, 18);
		ofNoFill();
		ofSetColor(127, 127, 127);
		ofRect(leftColumn, 13, 188, 18);
		ofFill();
		ofTriangle(leftColumn+177,17,leftColumn+185, 17, leftColumn+181, 23);
		ofSetColor(0, 0, 0);
		TTFsmall.drawString(serialThreadObject->serialport,leftColumn+5, 26);
		

		// start/stop button
		if(serialThreadObject->status) {
			ofFill();
			ofSetColor(240, 240, 240);
			ofRect(rightColumn, 13, 124, 18);
			ofNoFill();
			ofSetColor(127, 127, 127);
			ofRect(rightColumn, 13, 124, 18);
			ofSetColor(0, 0, 0);
			TTFsmall.drawString("Stop", rightColumn+48, 26);
		} else {
			ofFill();
			ofSetColor(232, 232, 232);
			ofRect(rightColumn, 13, 124, 18);
			ofNoFill();
			ofSetColor(127, 127, 127);
			ofRect(rightColumn, 13, 124, 18);
			ofSetColor(0, 0, 0);
			TTFsmall.drawString("Start", rightColumn+48, 26);
		}
		
		// show values button
		ofFill();
		ofSetColor(232, 232, 232);
		ofRect(rightColumn, 33, 124, 20);
		ofNoFill();
		ofSetColor(127, 127, 127);
		ofRect(rightColumn, 33, 124, 20);
		ofSetColor(0, 0, 0);		
		TTFsmall.drawString("Show Values", rightColumn+28, 48);
		
		texScreen.loadScreenData(0,0, 440, 440);
		drawTex = true;
	} else {
		if(drawTex) {
			texScreen.draw(0, 0, 440, 440);
			drawTex = 0;
		}
	}
	
	if(drawValues) {
		serialThreadObject->draw();
	}
	
	if(menuState) {
		numMenuItems = (int)serialThreadObject->deviceList.size() / 2;

		ofSetColor(240, 240, 240);
		ofFill();
		ofRect(leftColumn, 31, 188, numMenuItems*18);

		vector<ofSerialDeviceInfo>::iterator it;
		int i;
		for ( it = serialThreadObject->deviceList.begin(), i = 0 ; it < serialThreadObject->deviceList.end(); it++) {
			if(!strncmp(it->getDeviceName().c_str(), "tty", 3)){
				ofNoFill();
				ofSetColor(191, 191, 191);
				ofRect(leftColumn, 31+(i*18), 188, 18);
				ofSetColor(0, 0, 0);
				TTFsmall.drawString(it->getDeviceName(), leftColumn+4, 45+i*18 );
				i++;
			}
		}
		ofNoFill();
		ofSetColor(127, 127, 127);
		ofRect(leftColumn, 31, 188, numMenuItems*18);
	}
}

void testApp::exit()
{
	free(serialThreadObject);
	
	XML.pushTag("ASKServer", 0);
	XML.removeTag("display", 0);
	XML.setValue("display", drawValues, 0);
	XML.popTag();
	XML.saveFile("ASKServer.xml");
	return;
	// printf("exit\n");
}

void testApp::startSerial()
{
	if(serialThreadObject->status) {
		serialThreadObject->serial.close();
		serialThreadObject->stop(); // the thread
	}
	
	serialThreadObject->status = serialThreadObject->serial.setup(serialThreadObject->serialport, serialThreadObject->baudrate);
	if(serialThreadObject->status) {
		status1 = "Device open ";//+serialThreadObject->serialport+" "+ofToString(serialThreadObject->baudrate);
		ofSetWindowTitle(serialThreadObject->serialport);
		serialThreadObject->start(); // the serial thread
	} else {
		status1 = "Unable to open ";//+serialThreadObject->serialport;
		serialThreadObject->stop(); // the thread
		ofSystemAlertDialog("ASK Server \n"+status1+serialThreadObject->serialport);
	}
	redrawFlag = 1;
}

void testApp::stopSerial()
{
	if(serialThreadObject->status) {
		serialThreadObject->serial.close();
		serialThreadObject->stop(); // the serial thread
		serialThreadObject->status = false;
	}
	redrawFlag = 1;
}

void testApp::startNetsender()
{
	status = serialThreadObject->sender.setup( serialThreadObject->sendIP.c_str(), serialThreadObject->sendport);
	if(status) {
		status2 = "Sending OSC to "+serialThreadObject->sendIP+" on port "+ofToString(serialThreadObject->sendport);
	} else {
		status2 = "Unable to open Network "+serialThreadObject->sendIP+" on port "+ofToString(serialThreadObject->sendport);
	}
	redrawFlag = 1;
}


void testApp::getSerialDeviceList()
{
	serialThreadObject->deviceList = serialThreadObject->serial.getDeviceList();
	vector<ofSerialDeviceInfo>::iterator it;
	for ( it = serialThreadObject->deviceList.begin() ; it < serialThreadObject->deviceList.end(); it++) {
		string devPath = it->getDevicePath();
		string devName = it->getDeviceName();
		if(!strncmp(devName.c_str(), "tty.", 4)){
			// printf("device %s %s\n", devPath.c_str(), devName.c_str());
		}
	}
}

void testApp::receiveOSC()
{
	string temp;
	
	while( receiver.hasWaitingMessages()) {
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		temp = m.getAddress();
		
		if ( !strcmp( temp.c_str(),"/askserver/framerate" )) {
			framerate = m.getArgAsInt32( 0 );
			status2 = "sending OSC to "+serialThreadObject->sendIP+" on port "+ofToString(serialThreadObject->sendport);
		}else if ( strcmp( temp.c_str(),"/askserver/display" ) == 0 ) {
			display = m.getArgAsInt32( 0 );
			windowChanged = 1;
		}else if ( strcmp( temp.c_str(),"/askserver/reset" ) == 0 ) {
		}else if ( strcmp( temp.c_str(),"/askserver/writePrefs" ) == 0 ) {
			testApp::writePrefs();
		}else if ( strcmp( temp.c_str(),"/askserver/readPrefs" ) == 0 ) {
			testApp::readPrefs();
		}else if ( strcmp( temp.c_str(),"/askserver/network/receiver/port" ) == 0 ) {
			receiveport = m.getArgAsInt32( 0 );
			receiver.setup( receiveport );
		}else if ( strcmp( temp.c_str(),"/askserver/network/sender/port" ) == 0 ) {
			serialThreadObject->sendport = m.getArgAsInt32( 0 );
			serialThreadObject->sender.setup( serialThreadObject->sendIP, serialThreadObject->sendport );
			status2 = "sending OSC to "+serialThreadObject->sendIP+" on port "+ofToString(serialThreadObject->sendport);
		}else if ( strcmp( temp.c_str(),"/askserver/network/sender/IP" ) == 0 ) {
			serialThreadObject->sendIP = m.getArgAsString(0);
			serialThreadObject->sender.setup( m.getArgAsString(0), serialThreadObject->sendport );
			status2 = "sending OSC to "+serialThreadObject->sendIP+" on port "+ofToString(serialThreadObject->sendport);
		}else if ( strcmp( temp.c_str(),"/askserver/exit" ) == 0 ) {
			testApp().exit();
		}else if ( strcmp( temp.c_str(),"/askserver/calibrate" ) == 0 ) {
			/*
			if(serialThreadObject->calibrate == 1) { // before we swith it off
				writeScaling(); // we write the values into the prefs
			}
			serialThreadObject->calibrate = !serialThreadObject->calibrate;
			printf("calibrate is %d\n", serialThreadObject->calibrate);
			*/
		}
	}
}

bool testApp::readPrefs()
{	
	int numTags;
	int numPtTags;
	int totalToRead;
	int i;
	int ID;
	int bitwidth;
	int value;
    
    string str1;
	string str2;
	string str3;
	string str4;
	string str5;
	string str6;
	string str7;
	string str8;
	
	bool result = XML.loadFile("ASKServer.xml");
	if(result) {
		serialThreadObject->serialport = XML.getValue("ASKServer:serialport", "/dev/tty.usbserial-A6007WWR");
		
		serialThreadObject->sendIP = XML.getValue("ASKServer:network:sender:IP", "127.0.0.1");
		serialThreadObject->sendport = XML.getValue("ASKServer:network:sender:port", 40000);
		receiveport = XML.getValue("ASKServer:network:receiver:port", 40001);

		serialThreadObject->baudrate = XML.getValue("ASKServer:baudrate", 57600);
		framerate = XML.getValue("ASKServer:framerate", 200);
		drawValues = XML.getValue("ASKServer:display", 0);
		drawValues = CLAMP(drawValues, 0, 1);
        
        // read mpu tags
		numTags = XML.getNumTags("ASKServer:mpu");
		if(numTags > 0) {
            printf("mpu tags found\n");
			XML.pushTag("ASKServer", numTags-1);
			
			serialThreadObject->numImuAddr = numPtTags = XML.getNumTags("mpu");
			if(numPtTags > 0) {
				totalToRead = MIN(numPtTags, 12);
				for(i = 0; i < totalToRead; i++) {
					ID = XML.getAttribute("mpu", "id", 0, i);
                    printf("imu ID's value is %d \n", ID);
                    if(ID >= 0 && ID < 24) {
                        serialThreadObject->mpuaddresses[ID] = XML.getValue("mpu:oscaddress", "/icst/mpu/0", i);
                        printf("attribute string is %s \n", serialThreadObject->mpuaddresses[ID+1].c_str() );

                    }
				}
			}
			
//            serialThreadObject->systemaddress = XML.getValue("system:oscaddress", "/icst/systime", 0);
//            printf("system string is %s \n", serialThreadObject->systemaddress.c_str() );
			
//			serialThreadObject->sendFlat = XML.getValue("ASKServer:sendFlat", 0, 0);
//			serialThreadObject->sendFlat = CLAMP(serialThreadObject->sendFlat, 0, 1);
			
			XML.popTag();
		}
        
        // read thumbs tags
        numTags = XML.getNumTags("ASKServer:thumbs");
        if(numTags > 0) {
            printf("thumbs tags found\n");
            XML.pushTag("ASKServer", numTags-1);
            
            serialThreadObject->numThumbsAddr = numPtTags = XML.getNumTags("thumbs");
            if(numPtTags > 0) {
                totalToRead = MIN(numPtTags, 12);
                for(i = 0; i < totalToRead; i++) {
                    ID = XML.getAttribute("thumbs", "id", 0, i);
                    printf("thumbs ID's value is %d \n", ID);
                    if(ID >= 0 && ID < 24) {
                        serialThreadObject->thumbsaddresses[ID] = XML.getValue("thumbs:oscaddress", "/icst/thumbs/0", i);
                        printf("attribute string is %s \n", serialThreadObject->thumbsaddresses[ID+1].c_str() );
                    }
                }
            }
            
            XML.popTag();
        }
        
        // read system tags
        numTags = XML.getNumTags("ASKServer:system");
        if(numTags > 0) {
            printf("system tags found\n");
            XML.pushTag("ASKServer", numTags-1);
            
            serialThreadObject->numSysAddr = numPtTags = XML.getNumTags("system");
            if(numPtTags > 0) {
                totalToRead = MIN(numPtTags, 12);
                for(i = 0; i < totalToRead; i++) {
                    ID = XML.getAttribute("system", "id", 0, i);
                    printf("system ID's value is %d \n", ID);
                    if(ID >= 0 && ID < 24) {
                        serialThreadObject->systemaddresses[ID] = XML.getValue("system:oscaddress", "/icst/system/0", i);
                        printf("attribute string is %s \n", serialThreadObject->systemaddresses[ID+1].c_str());
                    }
                }
            }
            
            XML.popTag();
        }
	}
	return result;
}

void testApp::dumpPrefs()
{
	int i;
	
	printf("serialport %s\n", serialThreadObject->serialport.c_str());
	printf("sender IP %s\n", serialThreadObject->sendIP.c_str());
	printf("sender port %d\n", serialThreadObject->sendport);
	printf("receive port %d\n", receiveport);
	printf("baudrate %d\n", serialThreadObject->baudrate);
	printf("send-raw %d\n", serialThreadObject->sendFlat);
	printf("framerate %d\n", framerate);
	printf("display %d\n", display);
	printf("imu\n");
	for(i = 0; i < serialThreadObject->numImuAddr; i++){
		printf("    oscaddress %d %s\n",i, serialThreadObject->mpuaddresses[i].c_str() );
	}
    printf("thumbs\n");
    for(i = 0; i < serialThreadObject->numThumbsAddr; i++) {
        printf("    oscaddress %d %s\n",i, serialThreadObject->thumbsaddresses[i].c_str() );
    }
    printf("system\n");
    for(i = 0; i < serialThreadObject->numSysAddr; i++) {
        printf("    oscaddress %d %s\n", i, serialThreadObject->systemaddresses[i].c_str());
    }

}


void testApp::writePrefs()
{
	XML.setValue("ASKServer:framerate",					framerate);

	XML.setValue("ASKServer:display",					display);
	XML.setValue("ASKServer:network:receiver:port",		receiveport);
	XML.setValue("ASKServer:network:sender:port",		serialThreadObject->sendport);
	XML.setValue("ASKServer:network:sender:IP",			serialThreadObject->sendIP);
	XML.setValue("ASKServer:serialport",					serialThreadObject->serialport);
	XML.setValue("ASKServer:baudrate",					serialThreadObject->baudrate);
	XML.getValue("ASKServer:send-raw",					serialThreadObject->sendFlat);
	XML.saveFile("ASKServer.xml");
	return;
}

void testApp::readMidicodes()
{	

}

void testApp::writeScaling()
{	
//	XML.pushTag("braceletServer", 0);
//	for(int i = 0; i < serialThreadObject->numKeyAddr; i++) {
//		XML.pushTag("analog", i);
//		
//		XML.removeTag("minimum", 0);
//		XML.removeTag("maximum", 0);
//		XML.setValue("minimum", (int)serialThreadObject->analog[i].minimum, i);
//		XML.setValue("maximum", (int)serialThreadObject->analog[i].maximum, i);
//		// printf("key %d min %d max %d\n", i, (int)serialThreadObject->analog[i].minimum, (int)serialThreadObject->analog[i].maximum);
//		XML.popTag();
//	}
//	XML.popTag();
//	XML.saveFile("braceletServer.xml");
//	return;
}

void testApp::resetCalibrate()
{

}

void testApp::keyReleased(int key)
{
	switch(key){
		case 'f': // f-key: switch winow size
			drawValues = !drawValues;
			windowChanged = 1;
			redrawFlag = 1;
			break;

		case 'F':
			drawValues = !drawValues;
			windowChanged = 1;
			redrawFlag = 1;
			break;			

	}
}

void testApp::mouseMoved( int x, int y)
{
}

void testApp::mouseDragged( int x, int y, int button)
{
}

void testApp::mouseReleased()
{
}

void testApp::mousePressed(int x, int y, int button)
{
	
	printf("mousepressed at %d %d\n", x, y);
	ofRect(295, 36, 295+124, 36+20);
	
	// click in start/stope values
	if(x > 295 && x < 419 && y > 14 && y < 35) {
		// printf("start/stop clicked with status %d\n", serialThreadObject->status);
		if(serialThreadObject->status) {
			stopSerial();
			drawValues = 0;
		} else {
			startSerial();
			drawValues = 1;
		}
		
		windowChanged = 1;
		redrawFlag = 1;
	}
    
	// click in show/hide values
	if(x > 295 && x < 419 && y > 36 && y < 58) {
		if(drawValues != 0) {
			drawValues = 0;
            serialThreadObject->drawValues = false;
		} else {
			drawValues = 1;
            serialThreadObject->drawValues = true;
		}
		windowChanged = 1;
		redrawFlag = 1;
	}
	// click in menu-original-textbox
	if(x > 105 && x < 293 && y > 13 && y < 31) {
		if(menuState != 0) {
			menuState = 0;
		} else {
			menuState = 1;
			getSerialDeviceList();
			stopSerial();
			drawValues = 1;
            serialThreadObject->drawValues = true;

		}
		windowChanged = 1;
		redrawFlag = 1;
	}
	
	// click in menu?
	if(menuState) {
		if(x > 105 && x < 293 && y > 31 && y < 31+numMenuItems*18) {
			int clickedItem = (y - 31) / 18;
			
			menuState = 0;

			vector<ofSerialDeviceInfo>::iterator it;
			int i = 0;
			for ( it = serialThreadObject->deviceList.begin(); it < serialThreadObject->deviceList.end(); it++) {
				if(!strncmp(it->getDeviceName().c_str(), "tty", 3)){
					if(i == clickedItem) {
						serialThreadObject->serialport = it->getDevicePath();
						startSerial();
					
						XML.pushTag("ASKServer", 0);
						XML.removeTag("serialport", 0);
						XML.setValue("serialport", serialThreadObject->serialport, 0);
						XML.popTag();
						XML.saveFile("ASKServer.xml");
						
						break;
					}
					i++;
				}
			}
			redrawFlag = 1;
		}
	}
}


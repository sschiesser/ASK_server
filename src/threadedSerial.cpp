/*
 *  threadedSerial.cpp
 *  oscSerialSender
 *
 *  Created by jasch on 9/11/11.
 *  Copyright 2011 j45ch. All rights reserved.
 *
 */

#include "threadedSerial.h"

threadedSerial::threadedSerial()
{
    numImuAddr = 24;
	TTF.loadFont("inconsolata.ttf", 11, 1, 1, 0);
    streamSize = SENSOR_PATTERNLENGTH_1;
}

threadedSerial::~threadedSerial()
{
	if( isThreadRunning() ) { 
		stopThread();
	}
    if(status){
        serial.close();
    }
}

union float2bytes {
    float f;
    uint8_t b[4];
};
float2bytes f2b;


void threadedSerial::start()
{
	startThread(true);   // blocking, verbose
    oldSystime = ofGetElapsedTimeMillis();
}

void threadedSerial::stop()
{
	stopThread();
}

//--------------------------
void threadedSerial::threadedFunction() 
{
	while( isThreadRunning() != 0 ){
		if( lock() ){
			readSerial(); // threaded serial polling call
            usleep(500);  // mac sleeps in microseconds = 1/2 millisecond interval for serialThread
			unlock();
		}
	}
}

void threadedSerial::readSerial()
{
	// reading serial port
	long timesRead = 0;
	long timesWritten = 0;
	long nRead  = 0;
	
	unsigned char bytesReturned[1];
	memset(bytesReadString, 0, 3);
	
	while( (nRead = serial.readBytes(bytesReturned, 1)) > 0){
		nTimesRead++;	
		nBytesRead = nRead;
//        printf("Incoming: %d\n", *bytesReturned);
		serialparse(bytesReturned);
	}
}

void threadedSerial::serialparse(unsigned char *c)
{
	int i, j;
	long sum;
	
    // push bytes forward in the circular buffer
    for (i = 0; i < streamSize-1; i++) {
        serialStream[i] = serialStream[i+1];
//        printf("SerialStream[%d] = %d\n", i, serialStream[i]);
    }
    serialStream[streamSize-1] = c[0]; // append new byte to buffer
//    printf("New byte = %d\n", serialStream[streamSize-1]);
    
	// pattern matching... MPU frame
	if (serialStream[0] == 60) { // packet start marker 'A'
        if(serialStream[1] == 191) {	// MPU sensor packet
            if(serialStream[35] == 90) { // packet end marker 'Z'
                for(i = 0; i < 34; i++) { // collect n-2 bytes into buffer
                    input[i] = serialStream[i+2];
                    printf("input[%d] = %d\n", i, input[i]);
                }
                haveInput = true;
                parsePacket();
                sendOSC();
            }
        }
    }
}

void threadedSerial::parsePacket()
{
    struct timeval tval_old, tval_new, tval_delta;
    
    if(haveInput) {
        gettimeofday(&tval_new, NULL);
        printf("Packet received... %f\n", (float)(0.001*(float)tval_new.tv_usec));
        
        f2b.b[0] = input[0];
        f2b.b[1] = input[1];
        f2b.b[2] = input[2];
        f2b.b[3] = input[3];
        quaternion[0] = f2b.f;
//        printf("Quat 1: in0 0x%x, in1 0x%x, in2 0x%x, in3 0x%x, float %f\n", input[0], input[1], input[2], input[3], quaternion[0]);

        f2b.b[0] = input[4];
        f2b.b[1] = input[5];
        f2b.b[2] = input[6];
        f2b.b[3] = input[7];
        quaternion[1] = f2b.f;

        f2b.b[0] = input[8];
        f2b.b[1] = input[9];
        f2b.b[2] = input[10];
        f2b.b[3] = input[11];
        quaternion[2] = f2b.f;

        f2b.b[0] = input[12];
        f2b.b[1] = input[13];
        f2b.b[2] = input[14];
        f2b.b[3] = input[15];
        quaternion[3] = f2b.f;
//        printf("Quat: w %f, x %f, y %f, z %f\n", quaternion[0], quaternion[1], quaternion[2], quaternion[3]);

        f2b.b[0] = input[16];
        f2b.b[1] = input[17];
        f2b.b[2] = input[18];
        f2b.b[3] = input[19];
        accel[0] = f2b.f;
        
        f2b.b[0] = input[20];
        f2b.b[1] = input[21];
        f2b.b[2] = input[22];
        f2b.b[3] = input[23];
        accel[1] = f2b.f;

        f2b.b[0] = input[24];
        f2b.b[1] = input[25];
        f2b.b[2] = input[26];
        f2b.b[3] = input[27];
        accel[2] = f2b.f;
//        printf("Accel: x %f, y %f, z %f\n", accel[0], accel[1], accel[2]);

        f2b.b[0] = input[28];
        f2b.b[1] = input[29];
        f2b.b[2] = input[30];
        f2b.b[3] = input[31];
        gyro[0] = f2b.f;
        
        f2b.b[0] = input[32];
        f2b.b[1] = input[33];
        f2b.b[2] = input[34];
        f2b.b[3] = input[35];
        gyro[1] = f2b.f;
        
        f2b.b[0] = input[36];
        f2b.b[1] = input[37];
        f2b.b[2] = input[38];
        f2b.b[3] = input[39];
        gyro[2] = f2b.f;
//        printf("Gyro: x %f, y %f, z %f\n", gyro[0], gyro[1], gyro[2]);

        
        joystickRaw[0] = (input[40] << 8) | input[41]; // X (horizontal)
        joystickRaw[1] = (input[42] << 8) | input[43]; // Y (vertical)
        joystickRaw[2] = input[44]; // button (binary)
        trackballRaw[0] = input[45]; // X (horizontal)
        trackballRaw[1] = input[47]; // Y (vertical)
        trackballRaw[2] = input[49]; // button (binary)
//        printf("joy raw: %d, %d, %d\ntb raw: %d, %d, %d\n", joystickRaw[0], joystickRaw[1], joystickRaw[2], trackballRaw[0], trackballRaw[1], trackballRaw[2]);

        summedIMU[0] = (fabs(accel[0] - 0.5) + fabs(accel[1] - 0.5) + fabs(accel[2] - 0.5) ) * 0.66666666666666666666;
        summedIMU[1] = (fabs(gyro[0] - 0.5) + fabs(gyro[1] - 0.5) + fabs(gyro[2] - 0.5) ) * 0.66666666666666666666;
    
        quatToEuler();
        quatToAngleAxis();
//        eulerToAngleAxis();
        
        joystick[0] = (joystickRaw[0]) / 1024.0f;
        joystick[1] = (joystickRaw[1]) / 1024.0f;
        joystick[2] = joystickRaw[2];
        if(joystick[2] != oldJoySw) {
            oldJoySw = joystick[2];
            joySwChanged = true;
        }
        trackball[0] = (trackballRaw[0]) / 240.0f;
        trackball[1] = (trackballRaw[1]) / 240.0f;
        trackball[2] = trackballRaw[2];
        if(trackball[2] != oldTbSw) {
            oldTbSw = trackball[2];
            tbSwChanged = true;
        }
//        printf("Slider: %f\nThumb Joy: %f - %f\nTrackball: %f - %f - %d\n", slider, joystick[0], joystick[1], trackball[0], trackball[1], (int)trackball[2]);
    }
}

void threadedSerial::sendOSC()
{
	if(haveInput) {
        systime = ofGetElapsedTimeMillis();
        timestamp = systime - oldSystime;
        oldSystime = systime;
        deltaTime = 0;
//        printf("timestamp: %ld\n", timestamp);
        
        if(sendFlat) {
            
            m[0].clear();
            m[0].setAddress( mpuaddresses[0] ); // flat array            m[0].addFloatArg( quaternion[0] );
            m[0].addFloatArg( quaternion[0] );
            m[0].addFloatArg( quaternion[1] );
            m[0].addFloatArg( quaternion[2] );
            m[0].addFloatArg( quaternion[3] );
            m[0].addFloatArg( accel[0] );
            m[0].addFloatArg( accel[1] );
            m[0].addFloatArg( accel[2] );
            m[0].addFloatArg( gyro[0] );
            m[0].addFloatArg( gyro[1] );
            m[0].addFloatArg( gyro[2] );
            sender.sendMessage( m[0] );
            
        } else {
       
            m[1].clear();
            m[1].setAddress( mpuaddresses[1] ); // quaternion
            m[1].addFloatArg( quaternion[0] );
            m[1].addFloatArg( quaternion[1] );
            m[1].addFloatArg( quaternion[2] );
            m[1].addFloatArg( quaternion[3] );
            sender.sendMessage( m[1] );
            
            m[2].clear();
            m[2].setAddress( mpuaddresses[2] ); // accel scaled
            m[2].addFloatArg( accel[0] );
            m[2].addFloatArg( accel[1] );
            m[2].addFloatArg( accel[2] );
            sender.sendMessage( m[2] );
            
            m[3].clear();
            m[3].setAddress( mpuaddresses[3] ); // gyro scaled
            m[3].addFloatArg( gyro[0] );
            m[3].addFloatArg( gyro[1] );
            m[3].addFloatArg( gyro[2] );
            sender.sendMessage( m[3] );
            
        
            m[4].clear();
            m[4].setAddress( mpuaddresses[4] ); // accel summed
            m[4].addFloatArg( summedIMU[0] );
            sender.sendMessage( m[4] );
            
            m[5].clear();
            m[5].setAddress( mpuaddresses[5] ); // gyro summed
            m[5].addFloatArg( summedIMU[1] );
            sender.sendMessage( m[5] );
            
            m[6].clear();
            m[6].setAddress( mpuaddresses[6] ); // euler
            m[6].addFloatArg( euler[0] );
            m[6].addFloatArg( euler[1] );
            m[6].addFloatArg( euler[2] );
            sender.sendMessage( m[6] );
            
            m[7].clear();
            m[7].setAddress( mpuaddresses[7] ); // yaw pitch roll
            m[7].addFloatArg( ypr[0] );
            m[7].addFloatArg( ypr[1] );
            m[7].addFloatArg( ypr[2] );
            sender.sendMessage( m[7] );
            
            m[8].clear();
            m[8].setAddress( thumbsaddresses[1] ); // thumb joystick
            m[8].addFloatArg( joystick[0]);
            m[8].addFloatArg( joystick[1]);
            m[8].addIntArg(joystick[2]);
            sender.sendMessage( m[8] );
            
            m[9].clear();
            m[9].setAddress( thumbsaddresses[2] ); // trackball
            m[9].addFloatArg( trackball[0] );
            m[9].addFloatArg( trackball[1] );
            if(tbSwChanged) {
                m[9].addFloatArg( trackball[2] );
                tbSwChanged = false;
            }
            sender.sendMessage( m[10] );
           
            m[11].clear();
            m[11].setAddress( systemaddresses[1] ); // timestamp
            m[11].addIntArg( timestamp );
            m[11].addIntArg( deltaTime );
            sender.sendMessage( m[11] );
        }		
		haveInput = false;
	}
}

void threadedSerial::draw()
{
	double x1 = 10; // left border for left sliders
    double x2 = x1 + 110; // left border for right sliders
	double yy = 74; // top border for text display
    int textLineHeight = 16;
    double y1 = 16; // top border for top sliders (x)
    double y2 = y1 + textLineHeight; // top border for middle sliders (y)
    double y3 = y2 + textLineHeight; // top border for bottom sliders (z)
    double accelScale = 0.25;
    double gyroScale = 0.02;
	
	if( lock() ) {
		if (status == 1) {

            // text display
            ofSetColor(0, 0, 0, 255);
            TTF.drawString( "quaternion " + ofToString(quaternion[0], 6) + " " + ofToString(quaternion[1], 6) + " " +  ofToString(quaternion[2], 6) + " " +  ofToString(quaternion[3], 6), x1, yy);
            TTF.drawString( "accel " + ofToString(accel[0], 5) + " " + ofToString(accel[1], 5) + " " + ofToString( accel[2], 5), x1, yy + textLineHeight);
            TTF.drawString(  "gyro   " +  ofToString(gyro[0], 5) + " " + ofToString(gyro[1], 5) + " " + ofToString( gyro[2], 5), x1, yy + 2*textLineHeight);
            TTF.drawString( "joystick " + ofToString(joystick[0]) + " " + ofToString(joystick[1]) + " " + ofToString(joystick[2]), x1, yy + 3*textLineHeight);
            TTF.drawString( "euler " + ofToString(euler[0] * RAD_TO_DEG) + " " + ofToString(euler[1] * RAD_TO_DEG)+ " " + ofToString(euler[2] * RAD_TO_DEG), x1 + 200, yy + textLineHeight);
            TTF.drawString( "ypr " + ofToString(ypr[0] * RAD_TO_DEG) + " " + ofToString(ypr[1] * RAD_TO_DEG) + " " + ofToString(ypr[2] * RAD_TO_DEG), x1 + 200, yy + 2*textLineHeight);
            TTF.drawString( "trackball " + ofToString(trackball[0]) + " " + ofToString(trackball[1]) + " " + ofToString(trackball[2]), x1 + 200, yy + 3*textLineHeight);
           
            ofPushMatrix();
            ofTranslate(0, yy + (3*textLineHeight));
            
            // sliders for accel & gyro display
            ofNoFill(); // rectangles
            ofSetColor(91, 91, 91, 255);
            ofRect(x1, y1, 104, 12); // accel x
            ofRect(x1, y2, 104, 12); // accel y
            ofRect(x1, y3, 104, 12); // accel z
            ofRect(x2, y1, 104, 12); // gyro x
            ofRect(x2, y2, 104, 12); // gyro y
            ofRect(x2, y3, 104, 12); // gyro z
            
            ofFill(); // cursors
            ofSetColor(0, 0, 0, 255);
            ofRect( x1 + (52 + (52 * accelScale * accel[0])), y1, 2, 12);
            ofRect( x1 + (52 + (52 * accelScale * accel[1])), y2, 2, 12);
            ofRect( x1 + (52 + (52 * accelScale * accel[2])), y3, 2, 12);
            ofRect( x2 + (52 + (52 * gyroScale * gyro[0])), y1, 2, 12);
            ofRect( x2 + (52 + (52 * gyroScale * gyro[1])), y2, 2, 12);
            ofRect( x2 + (52 + (52 * gyroScale * gyro[2])), y3, 2, 12);
            
            ofNoFill(); // middle lines
            ofSetColor(91, 91, 91, 255);
            ofLine(x1 + 52, y1, x1 + 52, y1 + 11);
            ofLine(x1 + 52, y2, x1 + 52, y2 + 11);
            ofLine(x1 + 52, y3, x1 + 52, y3 + 11);
            ofLine(x2 + 52, y1, x2 + 52, y1 + 11);
            ofLine(x2 + 52, y2, x2 + 52, y2 + 11);
            ofLine(x2 + 52, y3, x2 + 52, y3 + 11);
            
            ofPopMatrix();
            
            ofPushMatrix();
            ofTranslate(320., (yy + 3*textLineHeight + 40), 0); // set drawing center point for cube
            
            // cube
            ofRotate( angleAxis[0] * RAD_TO_DEG, angleAxis[1], -angleAxis[3], angleAxis[2]);
            ofScale(25., 5., 50.);
            drawCube();
            ofSetColor(255, 127, 0);
            ofLine(0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
            
            ofPopMatrix();
        }
		unlock();
	}
}

void threadedSerial::eulerToAngleAxis()
{
    double c1 = cos(ypr[2] / 2);
    double c2 = cos(ypr[0] / 2);
    double c3 = cos(ypr[1] / 2);
    double s1 = sin(ypr[2] / 2);
    double s2 = sin(ypr[0] / 2);
    double s3 = sin(ypr[1] / 2);
    double x = (s1 * s2 * c3) + (c1 * c2 * s3);
    double y = (s1 * c2 * c3) + (c1 * s2 * s3);
    double z = (c1 * s2 * c3) - (s1 * c2 * s3);
    double f = (double)sqrt(x * x + y * y + z * z);
    if (f < 1.1920929E-7) {
        f = 1.0;
    } else {
        f = 1.0 / f;
    }
    angleAxis[0] = 2 * (float)acos((c1 * c2 * c3) - (s1 * s2 * s3));
    angleAxis[1] = x * f;
    angleAxis[2] = y * f;
    angleAxis[3] = z * f;
}

void threadedSerial::quatToAngleAxis()
{
    double f = (double)sqrt( 1.0 - quaternion[0] * quaternion[0]);
    if (f < 1.1920929E-7) {
        f = 1.0;
    } else {
        f = 1.0 / f;
    }
    angleAxis[0] = ((float)acos(quaternion[0]) * 2.0);
    angleAxis[1] = (quaternion[1] * f);
    angleAxis[2] = (quaternion[2] * f);
    angleAxis[3] = (quaternion[3] * f);
}

void threadedSerial::quatToEuler()
{
    double gravity[3];
    double test = quaternion[1] * quaternion[2] + quaternion[3] * quaternion[0];
    
    // calculate gravity vector
    gravity[0] = 2 * (quaternion[1] * quaternion[3] - quaternion[0] * quaternion[2]);
    gravity[1] = 2 * (quaternion[0] * quaternion[1] + quaternion[2] * quaternion[3]);
    gravity[2] = quaternion[0] * quaternion[0] - quaternion[1] * quaternion[1] - quaternion[2] * quaternion[2] + quaternion[3] * quaternion[3];

    // calculate Euler angles
    if(test > 0.499) {
        euler[0] = 2 * atan2(quaternion[1], quaternion[0]);
        euler[1] = PI * 0.5;
        euler[2] = 0;
    }
    else if(test < -0.499) {
        euler[0] = -2 * atan2(quaternion[1], quaternion[0]);
        euler[1] = -PI * 0.5;
        euler[2] = 0;
    }
    else {
        euler[0] = atan2(2 * quaternion[2] * quaternion[0] - 2 * quaternion[1] * quaternion[3], 1 - 2 * quaternion[2] * quaternion[2] - 2 * quaternion[3] * quaternion[3]); // heading
        euler[1] = asin(2 * quaternion[1] * quaternion[2] + 2 * quaternion[3] * quaternion[0]); // attitude
        euler[2] = atan2(2 * quaternion[1] * quaternion[0] - 2 * quaternion[2] * quaternion[3], 1 - 2 * quaternion[1] * quaternion[1] - 2 * quaternion[3] * quaternion[3]); // bank
    }
 
    // calculate yaw/pitch/roll angles
    ypr[0] = atan2(2 * quaternion[1] * quaternion[2] - 2 * quaternion[0] * quaternion[3], 2 * quaternion[0] * quaternion[0] + 2 * quaternion[1] * quaternion[1] - 1); // yaw
    ypr[1] = atan(gravity[0] / sqrt(gravity[1] * gravity[1] + gravity[2] * gravity[2])); // pitch
    ypr[2] = atan(gravity[1] / sqrt(gravity[0] * gravity[0] + gravity[2] * gravity[2])); // roll
}



void threadedSerial::drawCube()
{
    GLfloat box[] =
    {
        // FRONT
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        // BACK
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        // LEFT
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        // RIGHT
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        // TOP
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        // BOTTOM
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
    };
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glVertexPointer(3, GL_FLOAT, 0, box);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    float a = 0.8f;	// alpha
    
    // FRONT AND BACK
    glColor4f(1.0f, 0.0f, 0.0f, a );
    glNormal3f(0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glColor4f(1.0f, 0.5f, 0.0f, a );
    glNormal3f(0.0f, 0.0f, -1.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    
    // LEFT AND RIGHT
    glColor4f(0.0f, 0.0f, 1.0f, a );
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glColor4f(0.0f, 0.5f, 1.0f, a );
    glNormal3f(1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    
    // TOP AND BOTTOM
    glColor4f(0.0f, 0.85f, 0.0f, a );
    glNormal3f(0.0f, 1.0f, 0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    glColor4f(1.0f, 0.0f, 1.0f, a );
    glNormal3f(0.0f, -1.0f, 0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
}

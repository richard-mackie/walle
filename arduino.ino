// Example 4 - Receive a number as text and convert it to an int
// https://forum.arduino.cc/t/serial-input-basics-updated/382007/2

#include <Servo.h>

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
char tempChars[numChars];        // temporary array for use when parsing

char messageFromPC[numChars] = {0};
int tilt_val = 0;
int pan_val = 0;

boolean newData = false;

Servo pan;
Servo tilt;


void setup() {
    pan.attach(9);
    tilt.attach(10);
    Serial.begin(115200);
    Serial.println("Data expected in format <pan, tilt>");
    Serial.println("<Arduino is ready>");
}


void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index
    Serial.println(strtokIndx);
 
    strtokIndx = strtok(tempChars, ","); // this continues where the previous call left off
    pan_val = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    tilt_val = atoi(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
    Serial.print("Pan Value ");
    Serial.println(pan_val);
    pan.write(pan_val);
    Serial.print("Tilt Value ");
    Serial.println(tilt_val);
    tilt.write(tilt_val);
}

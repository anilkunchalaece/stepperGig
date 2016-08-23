#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11 and 2 to 5
Stepper myStepper1(stepsPerRevolution, 8, 9, 10, 11);
Stepper myStepper2(stepsPerRevolution, 2, 3, 4, 5);

//=============

const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;

char messageFromPC[buffSize] = {0};
int newStepperPosition = 0;

void setup() {
  
  Serial.begin(9600);
   // set the speed at 60 rpm:
  myStepper1.setSpeed(60);
  
    // tell the PC we are ready
  Serial.println("<Arduino is ready>");
}

//=============

void loop() {

  getDataFromPC();
  if(newDataFromPC){
  updateStepper();
    replyToPC();
  }

}

//=============

void getDataFromPC() {

    // receive data from PC and save it into inputBuffer
    
  if(Serial.available() > 0) {

    char x = Serial.read();

      // the order of these IF clauses is significant
      
    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      parseData();
    }
    
    if(readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) { 
      bytesRecvd = 0; 
      readInProgress = true;
    }
  }
}

//=============
 
void parseData() {

    // split the data into its parts
    
  char * strtokIndx; // this is used by strtok() as an index
  
  strtokIndx = strtok(inputBuffer,",");      // get the first part - the string
  strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
  
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  newStepperPosition = atoi(strtokIndx);     // convert this part to an integer
  
}

//=============

void replyToPC() {

  if (newDataFromPC) {
    newDataFromPC = false;
    Serial.print("<Msg ");
    Serial.print(messageFromPC);
    Serial.print(" Stepper Position ");
    Serial.print(newStepperPosition);
   Serial.println(">");
  }
}

//============

void updateStepper() {
myStepper1.step(newStepperPosition);
//delay(1000);//wait for Stepper 1 to complete the revolution
myStepper2.step(stepsPerRevolution);//Rotate the stepper one full revolution i.e 360 degrees
//delay(1000);//wait for stepper 2 to complete the Revolution
//Please tinker with Delay to Get optimum Rotation
}



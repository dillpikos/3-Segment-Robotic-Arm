#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define CHANNEL 1 //Channel for communcation between transmitter and reciever device

//Create Objects for servos

Servo servoBottom,
      servoMiddle,
      servoTop,
      clawAngleServo,
      servoBase;

// Determine the Core FREERTOS will use //

#if CONFIG_FREERTOS_UNICORE //single core device
  static const BaseType_t app_cpu = 0;

#else //dual core device
  static const BaseType_t app_cpu = 1;

#endif

//Variables that control pin connections along with angles of servos

int servoOnePin = 23,
    servoTwoPin = 22, 
    servoClawPin = 21,
    servoClawOpenPin = 19,
    servoBasePin = 18; 
  
int midAngle = 45,
    bottomAngle = 90,
    topAngle = 20,
    baseAngle = 90;


//Struct that holds the information we're sending, a struct is necessary when //
//              sending multiple data when using ESP-NOW                      //
typedef struct joyStickData{

  int xPos; 
  int yPos;
  int xPos2; 
  int yPos2;
  int buttPin;


}joyStickData;

joyStickData incomingX;

uint8_t newData; //initializes unsigned integer newData


// function that task will execute // 
void processMovement(void *placeHolder){

  while(true){

    // Startup Case // 

    servoMiddle.write(midAngle); 
    servoBottom.write(bottomAngle);  
    clawAngleServo.write(topAngle); 
    servoTop.write(0);
    servoBase.write(baseAngle);

      
    //                         *Claw Grab Controls*                            //
    //     servoTop => push button (down = close claw, idel = open claw)       //

    if (incomingX.buttPin == HIGH){ //close claw with press of button
      servoTop.write(0);
    }
    
    else {                        //when button is not pressed leave claw open
      servoTop.write(180);
    }


    //        *Middle Segment Controls*         //
    //       yPos => left joystick y axis       //

    if( incomingX.yPos > 120){
      midAngle+= 5;
      if(midAngle > 180){
        midAngle = 180;
      }
    }

    else if ( incomingX.yPos < 50){
      midAngle -= 5;
      if(midAngle < 0){
        midAngle = 0;
      }
    }

    //        *Bottom Segment Controls*         //
    //       yPos2 => right joystick y axis     //

    if( incomingX.yPos2 > 120){
      bottomAngle += 5;
      if(bottomAngle > 180){
        bottomAngle = 180;
      }
    }

    else if ( incomingX.yPos2 < 50){
      bottomAngle -= 5;
      if(bottomAngle < 0){
        bottomAngle = 0;
      }
    }


    //       *Claw Angle Controls*      //
    //    xPos => left joystick x axis  //

      if( incomingX.xPos > 100){ //if x position of left js is pushed right move angle of claw up
          topAngle += 3;

          if (topAngle > 100){
          
            topAngle = 70; //max claw angle (top)

          }
          
      }

      else if ( incomingX.xPos < 35){ //if x position of left js is pushed left move angle of claw down
        topAngle -= 3;
        if(topAngle < 0){

          topAngle = 0; //max claw angle (low)

        }

      }

    //       *Arm Turning Controls*       //
    //   xPos2 => right joystick x axis   //
    
    if (incomingX.xPos2 < 60){
        baseAngle -= 4;

        if(baseAngle < 45){ 
          baseAngle = 45;   //max turn radius to right

        }
        
      }

    
    else if (incomingX.xPos2 > 110){

      baseAngle += 4;

      if(baseAngle > 180){
        baseAngle = 180; //max turn radius to left
      }

    }

    vTaskDelay(30 / portTICK_PERIOD_MS); //pause this task and run it every 30 mili-seconds
    }

  }

void setup() {  

  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP); 
  WiFi.softAP("RX_1", "RX_1_Password", CHANNEL, 0); // Give the network a name for searching
  
  esp_now_init(); // initializes esp_now
  esp_now_register_recv_cb(OnDataRecv);  // callback function to data received

  // attach servos to corresponding pins //
  servoBottom.attach(servoOnePin);          //GPIO 23
  servoMiddle.attach(servoTwoPin);          //GPIO 22
  clawAngleServo.attach(servoClawPin);      //GPIO 21
  servoTop.attach(servoClawOpenPin);        //GPIO 19
  servoBase.attach(servoBasePin);           //GPIO 18

  // Task created to handle processing of sent over data and converting it into arm movement //

  xTaskCreatePinnedToCore(
    processMovement,           //Function that's called
    "ProcessArmMovement",     //Name of Task
    4096,                    //(4096 good for tasks that require WiFi communication)
    NULL,                   //Parameter for function
    24,                    //Task Priority 0-24 (0 lowest priority & 24 highest priority)
    NULL,                 //Task handler
    app_cpu);            //run task on core selected at the top

  
}

void loop() {

  // loop empty due to while true function used with RTOS

}

// Function that acts as a call back, triggered whenever data is recieved from transmitter to ensure it's working as intended //
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int data_len) { 
  
  memcpy(&incomingX, data, sizeof(incomingX));


  /*

  Serial.print("JOYSTICK 1 Values");
  Serial.println("-------------------------------------");

  Serial.print("x-left: ");
  Serial.println(incomingX.xPos);

  Serial.print("y-left: ");
  Serial.println(incomingX.yPos);

  Serial.print("JOYSTICK 2 Values");
  Serial.println("--------------------------------------");

  Serial.print("x-right: ");
  Serial.println(incomingX.xPos2);

  Serial.print("y-right: ");
  Serial.println(incomingX.yPos2);

  Serial.print("Button Status: ");
  Serial.println(incomingX.buttPin);

  */

  Serial.print("Angle Values:");
  Serial.println("---------------------------------------------------------");

  Serial.print("Mid angle value: ");
  Serial.println(midAngle);

  Serial.print("Bottom Anlg value:");
  Serial.println(bottomAngle);

  Serial.print("Top Angle value:");
  Serial.println(topAngle);

  Serial.print("Base Angle value:");
  Serial.println(baseAngle);




}


#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1 //Channel for communcation between transmitter and reciever device
esp_now_peer_info_t slave; //store mac address of reciever

//Necessary Pin De
int xPin = 34, // x pin for controller 1
    yPin = 35, // y pin got controller 1
    xPin2 = 33,  // x pin for controller 2
    yPin2 = 32, 
    buttPin = 23,
    buttStat, //y pin for contoller 2
    xPosition,
    yPosition,
    xPosition2, //position x controller 2
    yPosition2; //position y controller 2

//Data thats sent//
uint8_t data = 3;

typedef struct joyStickData{

  int xPos;
  int yPos;
  int xPos2;
  int yPos2;
  int buttPin;


}joyStickData;

joyStickData xData;


void setup() {


  Serial.begin(115200);
  WiFi.mode(WIFI_STA); //puts esp in station mode
  esp_now_init();      //initilizes ESP now
  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);    //Call back function that runs everytime data is send
  ScanForSlave(); // WiFi.macAddress()
  esp_now_add_peer(&slave); 

  pinMode(buttPin, INPUT_PULLDOWN);

  //pinMode(xPin,INPUT); //read joystick xPosition
  //pinMode(yPin,INPUT); //read joystick ypostion
  //pinMode(xPin2,INPUT);
  //pinMode(yPin2,INPUT);

}

void loop() {

  //Read joystick values
  xPosition = analogRead(xPin) /3;
  yPosition = analogRead(yPin) /3;

  xPosition2 = analogRead(xPin2) /3;
  yPosition2 = analogRead(yPin2) /3;

  buttStat = digitalRead(buttPin);


  //Map x-Postition of joystick to control angle of servo and store that into struct
  xData.xPos = map(xPosition,0,1365, 0, 100); //tip angle of servo originally 0-160

  xData.yPos = map(yPosition, 0, 1365, 160, 0 );
  
  xData.xPos2 = map(xPosition2, 0, 1365 , 180 , 0);

  xData.yPos2 = map(yPosition2, 0 , 1365 , 160,  0);

  xData.buttPin = buttStat;


  esp_now_send(slave.peer_addr,(uint8_t *) &xData, sizeof(xData)); //function that sends struct values to reciever

  
  delay(100);
}

// Function that scans for target reciever

void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();

  for (int i = 0; i < scanResults; ++i) {
    String SSID = WiFi.SSID(i);
    String BSSIDstr = WiFi.BSSIDstr(i);

    if (SSID.indexOf("RX") == 0) {

      int mac[6];
      if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
        for (int ii = 0; ii < 6; ++ii ) {
          slave.peer_addr[ii] = (uint8_t) mac[ii];
        }
      }

      slave.channel = CHANNEL; // pick a channel
      slave.encrypt = 0; // no encryption
      break;
    }
  }
}

// function that runs everytime data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("I sent my data -> ");
  Serial.println(xData.xPos);
  Serial.print("I also sent ->");
  Serial.println(xData.yPos);
  Serial.print("I also sent y2: ->");
  Serial.println(xData.yPos2);
   Serial.print("I also sent ->");
  Serial.println(xData.xPos2);
  Serial.println(xData.buttPin);
}

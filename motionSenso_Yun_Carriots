/***********************************************************
* Author: PauMB                                            *
* Program: if PIR Sensor detects moves in the room,        *
*          the A.Yun send you an email and a sms           *
*          with Carriot's plataform                        *
* Created: 08/08/2014                                      *
* Company: HedaSoft                                        *
***********************************************************/
//importing librery
#include <Process.h>

#define APIKEY  "YOUR APIKEY";  // Replace with your Carriots apikey
#define DEVICE  "YOUR DEVICE";	// TO BE REPLACED with your Device's ID developer

String dataString = "";         //Constant to store the payload that will be sent

//Digital pins
int ledGreen = 2;
int ledRed = 13;
int sensorPIR = 12;

//varibales
int lastStatus = 0;                      //to know the last stat, detected or not
int detectados = -1;                      //number of detected

void setup() {
  // put your setup code here, to run once:
  Bridge.begin();
  Console.begin();
  while(!Console);                                        //wait console
  pinMode(ledGreen,OUTPUT);
  pinMode(ledRed,OUTPUT);
  pinMode(sensorPIR,INPUT);
  digitalWrite(ledGreen,HIGH);
  Console.println("PIR sensor ACTIVE!!!");
  Console.println("El sensor empezara a funcionar en:");
  int i;
  for(i=5; i>0; i--){                //the sensor start in 5"
    Console.print(i);
    Console.println("s");
    delay(1000);
  }
     
}

void loop() {
  // put your main code here, to run repeatedly:
  if(detectados == -1){    //show once.... this print can go in "setup()", it's stupid print... you can delete
    Console.println("Goooo!!!!");
    detectados += 1;
  }
  if(digitalRead(sensorPIR) == HIGH){    //detected moving
    
    digitalWrite(ledGreen,LOW);
    digitalWrite(ledRed,HIGH);
    
    if(lastStatus == 0){
      Console.println("There are something");
      
      if(detectados == 0 || detectados == 4){  //Only one mail if the sensor detected a moving in the firts time
        Console.println(F("Send Stream"));    // and send a sms if detected 5 moves
        updateData();
        sendData();
      }
      detectados += 1;                               //to know how many times it has detected motion
      lastStatus = 1;                               //only 1 time for the message "There are something"
    }
  }else{
    digitalWrite(ledRed,LOW);
    digitalWrite(ledGreen,HIGH);
    
    if(lastStatus == 1){
      Console.println("There aren't nothing");
      lastStatus = 0;                            //only 1 time for the message "There are something"
    }
  }

}

void updateData() {
  String txt = "";          // Text to send
  if(detectados == 0){    //firts time send an email
    txt = "ON";
  }
  if(detectados == 4){    //if the sensor detected 5 moving send a sms
    txt = "URGENTE";
  }
  
 
  Console.println(txt);      // For debugging purpose only
  
  // convert the readings to a String to send it:
  dataString = "{\"protocol\":\"v1\",\"checksum\":\"\",\"device\":\"";
  dataString += DEVICE;
  dataString += "\",\"at\":\"now\",\"data\":{\"sensorPIR\":";
  dataString += "\""+txt+"\"";
  dataString += "}}";
  
}

void sendData() {
  // form the string for the APIKEY header parameter:
  String apiString = "carriots.apikey: ";
  apiString += APIKEY;

  // Send the HTTP POST request
  Process carriots;
  Console.print("nnSending data... ");
  carriots.begin("curl");
  carriots.addParameter("-k");
  carriots.addParameter("--request");
  carriots.addParameter("POST");
  carriots.addParameter("--data");
  carriots.addParameter(dataString);
  carriots.addParameter("--header");
  carriots.addParameter(apiString);
  carriots.addParameter("https://api.carriots.com/streams/");
  carriots.run();
  Console.println("done!");

  // If there's incoming data from the net connection,
  // send it out the Serial:
  while (carriots.available() > 0) {
    char c = carriots.read();
    Console.write(c);
  }
  Console.println();

}

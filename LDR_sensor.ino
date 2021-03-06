/***********************************************************
* Author: PauMB                                            *
* Program: turn on || turn of a led,                       *
*          and connect with carriots' plataform            *
* Created: 08/05/2014                                      *
* Company: HedaSoft                                        *
***********************************************************/

#include <Process.h>

#define APIKEY  "Your api key"; // Replace with your Carriots apikey
#define DEVICE  "Your divice"; // Replace with the id_developer of your device

String dataString = ""; //Constant to store the payload that will be sent

int LDR = 1; // read the analog pin A1
int LED = 13; // turn on || turn of the led in the pin 11 (pwm)
int valLDR = 0; //the value of the LDR
int lightLED; //intensity of the led

const int ON = 1;    // Constant to indicate that lights are on
const int OFF = 2;  // Constant to indicate that lights are off


int lights = OFF;       // Current status

void setup() {
  
  // initialize serial communication:
  Bridge.begin();
  Serial.begin(9600);
  pinMode(LED, OUTPUT); //inicialize the led output
  
  //while (!Serial);   // wait for Network Serial to open
  Console.println("Carriots client");

}

void loop() {
  
  //read the value of LDR
  valLDR = analogRead(LDR);
  Console.println(valLDR); //print LDR's value
  
  if(valLDR > 100)
  {
    
    //regulate the led in funtion of LDR's value
    //lightLED = (valLDR/2)-150;
    digitalWrite(LED, LOW); //turn of the led
    lights = OFF;
    updateData();
    sendData();
  }
  else{
    digitalWrite(LED,/*lightLED*/HIGH); // turn on the led
    lights = ON;
    updateData();
    sendData();
  }
  delay(500);
  
}

void updateData() {
  Console.println("dentro updateData");
 
  String txt = "";          // Text to send
  if ( lights == OFF ) {   // Alarm OFF
     txt = "OFF";
  } else {                  // Alarm ON
     txt = "ON";
  }
  Console.println(txt);      // For debugging purpose only
  
  // convert the readings to a String to send it:
  dataString = "{\"protocol\":\"v1\",\"checksum\":\"\",\"device\":\"";
  dataString += DEVICE;
  dataString += "\",\"at\":\"now\",\"data\":{\"light\":";
  dataString += "\""+txt+"\"";
  dataString += "}}";
  Console.println("Fin updateData");
}

void sendData() {
  Console.println("Dentro del sendData");
 
  // form the string for the APIKEY header parameter:
  String apiString = "carriots.apikey: ";
  apiString += APIKEY;

  // Send the HTTP POST request
  Process carriots;
  Console.println("nnSending data... ");
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

}

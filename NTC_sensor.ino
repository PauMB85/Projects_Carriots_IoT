/***********************************************************
* Author: PauMB                                            *
* Program: turn on || turn of a led,                       *
*          in fuction of NTC sensor & Carriots             *
* Created: 08/05/2014                                      *
* Company: HedaSoft                                        *
***********************************************************/
//libreries
#include <Process.h>

#define APIKEY        "You apikey" 	// TO BE REPLACED with your Carriots APIKEY
#define DEVICE        "Your device" 

String dataString = "";  //Constant to store the payload that will be sent

const int ON = 1;      // Constant to indicate that lights are on
const int OFF = 2;     // Constant to indicate that lights are off

int ventilador = OFF;  //Current status
int newVentilador = OFF; //new status

//Digital Pin
int led = 7; 

//Analog Pin
int rNTC = 3;

//Values for the NTC
double vin = 5.0; //v. input
double r = 1000.0; //r who combine with ntc
double r25 = 2800.0; //r of NTC in 25gree
double beta = 3900.0; //constant beta in 25
double t0 = 293.15; //temperatue in kelvin (25)

//Values how calculete the RNTC
double vout = 0.0;
double R_NTC = 0.0;

//Temperature
double tK = 0.0; //temperature in Kelvin
double tC = 0.0; //temperature in C



void setup() {
  // put your setup code here, to run once:
  Bridge.begin();
  Console.begin();
  pinMode(led,OUTPUT);
  pinMode(rNTC,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Calculate the vout & R_NTC
  vout = vin/1024* analogRead(rNTC); //1024  A/D
  R_NTC = vout*r/(vin-vout);
  
  //Calculete the temperature
  tK= beta/(log(R_NTC/r25)+(beta/t0));
  tC = tK-273.15;
  
  //show in terminal
  Console.print("La temperatura en K:");
  Console.println(tK);
  Console.print("La temperatura en C:");
  Console.println(tC);

  //digitalWrite(led,HIGH);
  
  if(tC>23.0){
    Console.println("Turn on the Ventilador");
    digitalWrite(led,HIGH); //turn on the led
    newVentilador = ON;
  }
  else if(tC < 21.0){
    Console.println("Turn off the Ventilador");
    digitalWrite(led,LOW); // turn off the led
    newVentilador = OFF;
  }
  
  if(ventilador != newVentilador){
    Console.println("Send Stream");
    ventilador = newVentilador;
    updateData();
    sendData();
  }
  
  delay(2000);

}

void updateData() {
  String txt = "";          // Text to send
  if ( ventilador == OFF ) {   // Alarm OFF
     txt = "OFF";
  } else {                  // Alarm ON
     txt = "ON";
  }
  Console.println(txt);      // For debugging purpose only
  
  // convert the readings to a String to send it:
  dataString = "{\"protocol\":\"v1\",\"checksum\":\"\",\"device\":\"";
  dataString += DEVICE;
  dataString += "\",\"at\":\"now\",\"data\":{\"ventilador\":";
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

}

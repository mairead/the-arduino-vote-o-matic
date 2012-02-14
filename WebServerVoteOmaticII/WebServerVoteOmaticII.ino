/*
  Web Server Vote-o-Ramm
 
 A simple web server counting machine that tracks a set treshhold value of 3 analog light sensors and passes the highest analog input pin count to a webpage.
 Uses an Arduino Wiznet Ethernet shield. Use whatever means you like to trip the light sensor. We're using marbles and a child's marble run toy.
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0, A1, A3
 * Digital output pins 5, 6, 7
 
 created 18 Dec 2009
 by David A. Mellis
 modified 4 Sep 2010
 by Tom Igoe
 Further modified 15th Dec 2011
 by Mairead and Rob //http://www.headlondon.com //
 
 */

#include <SPI.h>
#include <Ethernet.h>

const int ledPin1 =  5;// the number of the LED pin as a tester
const int ledPin2 =  6;
const int ledPin3 =  7;

int photocellPin1 = A0;     // the cell and 10K pulldown are connected to a0
int photocellPin2 = A1;     // the cell and 10K pulldown are connected to a1
int photocellPin3 = A2;     // the cell and 10K pulldown are connected to a2
int photocellReading;     // the analog reading from the analog resistor divider

int total = 0;

int votes[] = {0,0,0};

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x89, 0x7C };
IPAddress ip(192,168,0,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup()
{
  // start the Ethernet connection and the server:
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  
  pinMode(ledPin1, OUTPUT);
  pinMode(photocellPin1, INPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(photocellPin2, INPUT); 
  pinMode(ledPin3, OUTPUT);
  pinMode(photocellPin3, INPUT); 
}

// A function to calculate which sensor has been tripped the most. A tie situation seems to register the last tying output tripped as the winner.
int calculateWinner()
{
  
  Serial.print("called function");
	int winner = 0;


	//determine which candidate has the most votes
	if(votes[1] > votes[0]){
		if(votes[1] > votes[2]){
		  winner = 2;
		}else{
		  winner = 3;
		}
	}else{
		if(votes[0] > votes[2]){
		  winner = 1;
		}else{
		  winner = 3;
		}
	}
  return winner;     //winner is the analog position with the most trips
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          
           Serial.print("write header");
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Content-Length: 44");
          client.println();
          
          // output the value of each analog input pin (to monitor values light values) plus assigned winning analog pin number
for (int analogChannel = 0; analogChannel < 1; analogChannel++) {
//            client.print("analog input ");
//            client.print(analogChannel);
//            client.print(" is ");
//            client.print(analogRead(analogChannel));
//            client.println("<br />");
//            client.print("<H1>And the winner is = ");
 Serial.print("output winner");
client.println(calculateWinner());
           
//            client.println("</H1>"); 
         }
          break;    // leave the loop
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
     Serial.print("page served");
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }

//LIGHTS
// VOTE OPTION 1

  photocellReading = analogRead(photocellPin1);
  
  Serial.print("Analog reading = ");
  Serial.print(photocellReading);   // the raw analog reading

  // We'll have a few threshholds, qualitatively determined
  if (photocellReading < 30) {
    Serial.println(" - DIM (VOTE 1)");
    votes[0] += 1;                  //if the sensor value dips below 30 add a vote to the pin counter
    digitalWrite(ledPin1, HIGH);
    delay(500);
  } else {
    Serial.println(" - LIGHT");
    
    digitalWrite(ledPin1, LOW);
    
  }
  
 // VOTE OPTION 2

  photocellReading = analogRead(photocellPin2);
  
  Serial.print("Analog reading = ");
  Serial.print(photocellReading);     // the raw analog reading
  
  // We'll have a few threshholds, qualitatively determined
  if (photocellReading < 30) {
    Serial.println(" - DIM (VOTE 2)");
    votes[1] += 1;                  //if the sensor value dips below 30 add a vote to the analog pin counter
    digitalWrite(ledPin2, HIGH);
    delay(500);
  } else {
    Serial.println(" - LIGHT");
    
    digitalWrite(ledPin2, LOW);
  }

 // VOTE OPTION 3

  photocellReading = analogRead(photocellPin3);
  
  Serial.print("Analog reading = ");
  Serial.print(photocellReading);     // the raw analog reading
  
  // We'll have a few threshholds, qualitatively determined
  if (photocellReading < 30) {
    Serial.println(" - DIM (VOTE 3)");
    votes[2] += 1;                 //if the sensor value dips below 30 add a vote to the pin counter
    digitalWrite(ledPin3, HIGH);
    delay(500);
  } else {
    Serial.println(" - LIGHT");
    
    digitalWrite(ledPin3, LOW);
  }
  Serial.println(votes[0]);
  Serial.println(", ");
  Serial.println(votes[1]);
  Serial.println(", ");
  Serial.println(votes[2]);
    delay(1);
}


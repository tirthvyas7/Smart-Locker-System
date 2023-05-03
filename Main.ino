/*  
    SDA=10, SCk=13, MOSI=11, MISO=12,
    RST=9, GND=GND, TX(Gsm)=6, RX(Gsm)=7  Servo=2 and power 3.3V  Piezo analog 4    
*/
  #include <SoftwareSerial.h>
  #include <Servo.h>
  #include <SPI.h>
  #include <MFRC522.h>
  #define SS_PIN 10
  #define RST_PIN 9
  //#define LED 8
  Servo myservo; 
  SoftwareSerial mySerial(6, 7);
  byte readCard[4];
  String tag_UID = "90D047A4";  // Replace this with the UID of your tag!!!
  String tagID = "";
  MFRC522 mfrc522(SS_PIN, RST_PIN);
  String sms=""; // Create MFRC522 instance
  char msg=0;
  String message;
  int pos=0;
  int sensoroutput = 4;
  int THRESHOLD = 900;

 boolean readID()
  {
    //Check if a new tag is detected or not. If not return.
    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
      return false;
    }
    //Check if a new tag is readable or not. If not return.
    if ( ! mfrc522.PICC_ReadCardSerial())
    {
      return false;
    }
    tagID = "";
    // Read the 4 byte UID
    for ( uint8_t i = 0; i < 4; i++)
    {
      //readCard[i] = mfrc522.uid.uidByte[i];
      tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Convert the UID to a single String
    }
    tagID.toUpperCase();
    //mfrc522.PICC_HaltA(); // Stop reading
    return true;
  }
  void SendMessage(String sms)
    {
      Serial.print("Entered send message function\n");
      Serial.print(" ");
      mySerial.println("AT+CMGF=1\r");    //Sets the GSM Module in Text Mode
      delay(1000);  // Delay of 1000 milli seconds or 1 second
      mySerial.println("AT+CMGS=\"+918160262053\"\r"); // Replace x with mobile number
      Serial.print(" ");
      delay(1000);
      mySerial.println(sms);// The SMS text you want to send
      delay(100);
      mySerial.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
      //It sends back the sent message to be transmitted
      while(mySerial.available()){
        mySerial.read();
  }
      Serial.println("Received message that needs to be sent");
      Serial.print(" ");
      //Wait till it sends confirmation that it has sent the message
      while(!mySerial.available()){}
      delay(1000);
      //Clear the confirmation message
      mySerial.flush();
      while(mySerial.available()){
        mySerial.read();
      }
      Serial.println("Sent the message. Exitting send_msg function");
      Serial.print(" ");
  }

  void ReceiveMessage()
  {
        Serial.print("Entered receive msg function\n");
      Serial.print(" ");
      mySerial.println("AT+CMGF=1");
      delay(1000);
      mySerial.println("AT+CNMI=2,2,0,0,0\r"); // AT Command to recieve a live SMS

      //Wait till it goes into receive message mode
      while(!mySerial.available()){
      }
      delay(1000);

      Serial.println("Entered receive message mode");
      Serial.print(" ");
      //Clear confirmation message
      mySerial.flush();
      while(mySerial.available()){
        mySerial.read();
      }

      //Now wait for message to arrive  
      Serial.println("Waiting for message to arrive");  
      Serial.print(" ");
      while(!mySerial.available()){
      }
      //Print Received message
      Serial.print("Got a Message. Size = ");
      Serial.print(" ");  
      delay(1000);
      Serial.println(mySerial.available());
      //Print message that has been transmitted
      message = "";
      while(mySerial.available()){
        Serial.print(mySerial.available());
        Serial.print(" ");
        msg = mySerial.read();
        message += msg;   
      }
      Serial.println(message);
      // message.remove(2,58);
      // message.remove(62,10);
      message =message.substring(51,54);//Truncate the message string to get the password and remove unwanted junk from the string; the length works for a SIM in which owners number is not saved
      Serial.println(message);
      Serial.println("Exitting recieve_msg funtion");
  }
  void MakeCall()
  {
        mySerial.println("ATD+918160262053;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
        Serial.println("Calling......."); // print response over serial port
        delay(1000);
  }

  void setup()
  {
      myservo.attach(2);
      mySerial.begin(9600);   // Setting the baud rate of GSM Module  
      //Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
      Serial.begin(115200);        // Initialize serial communications with the PC
      SPI.begin(); // SPI bus
      mfrc522.PCD_Init(); // Initialise MFRC522
      myservo.write(pos); 
      Serial.println("Initialized GSM");
      
}
  void(* resetFunc) (void) = 0;

  void loop()
  {
 
      int value = analogRead(sensoroutput);
      //Serial.println (value); // function to read analog voltage from sensor
    if (value >= THRESHOLD)                    // function to check voltage level from sensor
    {
        SendMessage("Locker under threat");
        MakeCall();
        delay(10000);
        //resetFunc(); 
    }
      //Wait until new tag is available

  while (readID())
  {

    if(tagID!=tag_UID)
       {
         SendMessage("Enter valid card");
         resetFunc();     
        
       }
      if(tagID==tag_UID)
      {
          //digitalWrite(LED, !digitalRead(LED));  // Turn on or off the onboard led
        SendMessage("Enter password");
        ReceiveMessage();
        Serial.println("Came back");
        Serial.print(message);
        while(message !="sip")
        {
          SendMessage("Invalid Password");
          ReceiveMessage();
        }
      
        if(message == "sip")
        {
          //Serial.println("Servo function done");
        
        for (pos = 0; pos <= 90; pos += 1)
        { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15 ms for the servo to reach the position
        } 
          Serial.println("Opened");
          SendMessage("Enter password to lock again");
          ReceiveMessage();

           while(message !="sip")
        {
          SendMessage("Invalid Password");
          ReceiveMessage();
        }
          if(message == "sip")
          {
              for (pos = 90; pos >= 0; pos -= 1)
                { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
                  myservo.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(15);                       // waits 15 ms for the servo to reach the position
                }
            }
          }
        }
      }
    }

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX for GSM module

void setup() {
  // Open the serial communications to the computer
  Serial.begin(9600);
  // Set the data rate for the SoftwareSerial port to your GSM module
  mySerial.begin(9600);
  // Give time for the GSM module to initialize
  delay(2000);
  // Initialize the GSM module
  sendATCommand("AT");
  delay(1000);
  sendATCommand("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  sendATCommand("AT+CLIP=1"); // Show caller ID
  delay(1000);
  sendATCommand("ATA"); // Answer calls automatically
  delay(1000);
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    switch (command) {
      case 'C': // Make a call
        sendATCommand("ATD+918150049151;"); // Replace with the number you want to call
        delay(10000); // Call duration (adjust as needed)
        sendATCommand("ATH"); // Hang up
        break;
      case 'S': // Send SMS
        sendATCommand("AT+CMGS=\"+918150049151\""); // Replace with the recipient's number
        delay(1000);
        mySerial.print("Hello, this is an SMS from Arduino!"); // Replace with your message
        mySerial.write(26); // Send Ctrl+Z to indicate the end of the message
        break;
      case 'R': // Receive call (manual answer)
        Serial.println("Waiting for an incoming call...");
        break;
      case 'G': // Get signal strength
        getSignalStrength();
        break;
      case 'B': // Get battery level
        getBatteryLevel();
        break;
     default:
        Serial.println("Invalid command. Available commands: C (Call), S (Send SMS), R (Receive call), G (Get signal strength), B (Get battery level)");
    }
  }

  // Check for incoming call
  if (mySerial.available()) {
    String response = mySerial.readString();
    if (response.indexOf("RING") != -1) {
      Serial.println("Incoming Call!");
      // Add code here to manually answer the call
      sendATCommand("ATA"); // Answer the call
    }
  }
}

void sendATCommand(String command) {
  mySerial.println(command);
  delay(1000); // Wait for a response
  while (mySerial.available()) {
    char c = mySerial.read();
    Serial.print(c);
  }
  Serial.println();
}

void getSignalStrength() {
  sendATCommand("AT+CSQ"); // Request signal quality
}

void getBatteryLevel() {
  sendATCommand("AT+CBC"); // Request battery status
}

void processIncomingSMS() {
  sendATCommand("AT+CMGL=\"ALL\"");
}

void readSMS(int index) {
  sendATCommand("AT+CMGR=" + String(index));
}

void showSMS(String sms) {
  Serial.println("Received SMS: " + sms);
}
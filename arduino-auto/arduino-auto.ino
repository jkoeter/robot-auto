#include <AFMotor.h>
#include <Servo.h>
#include <NewPing.h>

#define TRIG_PIN A4
#define ECHO_PIN A5
#define MAX_DISTANCE 400

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 
Servo sonarServo;  // create servo object to control a servo 
AF_DCMotor motorR(3, MOTOR12_1KHZ); // create motor #1, 1kHz PWM
AF_DCMotor motorL(4, MOTOR12_1KHZ); // create motor #2, 1kHz PWM

int pinLED = 13;                         // option LED comms indicator

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    
    pinMode(pinLED, OUTPUT);              // set LED pin to output mode

    sonarServo.attach(10);  // attaches the servo on pin 10 to the servo object 
    sonarServo.write(90); // face servo forward

    motorL.run(FORWARD);
    motorR.run(FORWARD);

    digitalWrite(pinLED, HIGH);           // flash the LED indicator twice
    delay(500);
    digitalWrite(pinLED, LOW);
    delay(500);
    digitalWrite(pinLED, HIGH);
    delay(500);
    digitalWrite(pinLED, LOW);
}

void HandleCommand(const String &command)
{
    String reply;
    
    if (command.startsWith("BRAKE"))
    {
        motorL.run(RELEASE);
        motorR.run(RELEASE);

        reply = String("BRAKE:OK");
    }
    else if (command.startsWith("MOVE:"))
    {
        // Move the two wheels at the same speed
        int speed = command.substring(5).toInt();

        if (speed < 0)
        {
            motorL.run(BACKWARD);
            motorR.run(BACKWARD);
            speed = 0 - speed;
        }
        else
        {
            motorL.run(FORWARD);
            motorR.run(FORWARD);
        }
        
        motorL.setSpeed(speed);
        motorR.setSpeed(speed);

        reply = String("MOVE:OK");
    }
    else if (command.startsWith("RIGHT:"))
    {
        int speed = command.substring(6).toInt();

        if (speed < 0)
        {
            motorR.run(BACKWARD);
            speed = 0 - speed;
        }
        else
        {
            motorR.run(FORWARD);
        }
        
        motorR.setSpeed(speed);
        reply = String("RIGHT:OK");
    }
    else if (command.startsWith("LEFT:"))
    {
        int speed = command.substring(5).toInt();

        if (speed < 0)
        {
            motorL.run(BACKWARD);
            speed = 0 - speed;
        }
        else
        {
            motorL.run(FORWARD);
        }
        
        motorL.setSpeed(speed);
        reply = String("LEFT:OK");
    }
    else if (command.startsWith("SERVO:"))
    {
        int angle = command.substring(6).toInt();
        sonarServo.write(angle);
        reply = String("SERVO:OK");
    }
    else if (command.startsWith("DIST"))
    {
        int cm = sonar.ping_cm();
        reply = String("DIST:") + String(cm);
    }

    Serial.println(reply);
}

void loop()
{
    static String hc06Received;
    char charReceived = 0;

    // copy all data to the other serial port
    while (Serial.available())
    {
        digitalWrite(pinLED, HIGH);
        charReceived = (char)Serial.read();
        if ((charReceived != 0x0d) && (charReceived != 0x0a))
        {
            hc06Received += charReceived;
        }
        else
        {
            break;
        }
        digitalWrite(pinLED, LOW);
    }
    digitalWrite(pinLED, LOW);

    // process the received data; remove leading CR and LF characters
    if (((charReceived==0x0a) || (charReceived==0x0d)) && hc06Received.length())
    {
        // complete message received; process
        HandleCommand(hc06Received);

        // for now clear the string
        hc06Received.remove(0);
    }
}


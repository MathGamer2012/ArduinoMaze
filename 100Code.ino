//Name: Ishaan Mittal & Erin Sobers
//Date: 25th June 2024
//Assignment: Culm
//Purpose: To make a car that avoids obstacles 

//Sources: https://www.youtube.com/watch?v=Xz4iVpdMd-w
//Sources: https://www.youtube.com/watch?v=n-gJ00GTsNg

//Left Motor (Slower)
byte motor1Clockwise = 6;  //pin for motor direction
byte motor1Counterclockwise = 7;
byte motor1Speed = 5;  //pin for power of motor 1

//Right Motor (Faster)
byte motor2Clockwise = 8;  //pin for motor 2 direction
byte motor2Counterclockwise = 9;
byte motor2Speed = 10;  //pin for power of motor 2

byte speed1 = 160; //sets the speed of the specific motors 
byte speed2 = 150; //is lower bc this motor was faster; 

byte slow = 50; //this is the speed value it is reduced by for the gradual stop 

byte buttonPin = 2;  // Pin for the button

byte LED = 3; //Pin for LED (used for testing purposes)

byte trigPin = 12; //pins for the sensors 
byte echoPin = 11;


bool isMoving = false;  //used to track the movement status of the vechile
float duration, distance; //used to get distance values from the sensor (there are multiple bc you have to generate it multiple times for the values to be updated -> works similar to Math.random())
float duration2, distance2;


void setup() {

  Serial.begin(115200); //used to make a serial to view output from the sensor 
  //setting pinMode

  pinMode(LED, OUTPUT);
  pinMode(motor1Clockwise, OUTPUT);
  pinMode(motor1Counterclockwise, OUTPUT);
  pinMode(motor1Speed, OUTPUT);

  pinMode(motor2Clockwise, OUTPUT);
  pinMode(motor2Counterclockwise, OUTPUT);
  pinMode(motor2Speed, OUTPUT);

  // Set the button pin as input with pull_up so we don't have to hold onto the button to make it move
  pinMode(buttonPin, INPUT_PULLUP);


  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void gradualStop(int slowdown) { //method used to slowdown the speed before stoping (takes in an integer value of slowdown which represents how much the speed is reduced by)
  digitalWrite(motor1Speed, speed1 - slowdown); //reducing the speed 
  digitalWrite(motor2Speed, speed2 - slowdown);
}

void Stop() { //method used to stop the car
  digitalWrite(motor1Speed, 0); //makes speed to 0 as that makes the motors stop
  digitalWrite(motor2Speed, 0);
}

void RecursionStop() { //used for the final stop as there was an error occuring with the other logic as most of the times the sensor didn't automatically read 

  float duration3, distance3; //declaring varibales for the distance calc

      digitalWrite(trigPin, LOW); //doing digitalWrite to make the sensor run (found from video)
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration3 = pulseIn(echoPin, HIGH);
      distance3 = (duration3 * 0.034) / 2; //gets the distance value based on calculations (value in cm)

      if (distance3 < 20 && distance3 != 0) { //used to check if the distance is within range of the stop, the reason it also says 0 is becuase for some reason everytime it detecting a distance it also outputted 0, so we made it is it is not included as otherwise this code never runs
         
        Stop(); //runs the stop method to make the car stop
        delay(200); //delay is put here as the code processing is faster than the car and therfore it didn't run the previous stop command thus why it is put here 
        isMoving = false;  //sets the boolean to false so the car doesn't continue movement as this is the final stop

      } 

      else {
          RecursionStop(); //does recusion basically meaning that it continues repeating the method until the car is within the stopping range, otherwise it continues going straight so it gets closer and thus gets into the range
      }

    }

void normalSpeed() { //used to get the straight running in the start 
  digitalWrite(motor1Clockwise, LOW);
  digitalWrite(motor1Counterclockwise, HIGH);  //makes motor move counterclockwise (as for clockwise it was going in circles)
  analogWrite(motor1Speed, speed1);            //Changed the speed to lower than max becuase it was causing an error with the straigtness of the car

  //right
  digitalWrite(motor2Clockwise, LOW);
  digitalWrite(motor2Counterclockwise, HIGH);  //makes motor move counterclockwise (as for clockwise it was going in circles)
  analogWrite(motor2Speed, speed2); //Changed the speed to lower than max becuase it was causing an error with the straigtness of the car
}

void goStraight() { //used to make the car move straight again after it stops mutliple times 
  analogWrite(motor1Speed, speed1); //the reason I didn't use the previous method again is becuase there was an error which is why we changed the method up a bit
  analogWrite(motor2Speed, speed2);
}

void turnRight() { //used to turn the car right 
  analogWrite(motor1Speed, speed1); //puts all the speed in the left wheel as during right turns, the right wheel/leg is the one that is pivoting thus not moving 
  analogWrite(motor2Speed, 0);
}

void turnLeft() { //used to turn the car left
  analogWrite(motor1Speed, 0); //same logic as aboce method but oppsite 
  analogWrite(motor2Speed, speed2);
}
void loop() {

  //generates the sensor values 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  distance = (duration * 0.034) / 2;

  //counter used to keep track of how many times the sesnor detects 
  byte counter = 0;

  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    delay(50);
    if (digitalRead(buttonPin) == LOW) {
      //Changes the movement state (since it is not it becomes true as the button was pressed)
      isMoving = !isMoving;

      // Waits until the button is released
      while (digitalRead(buttonPin) == LOW) {
      }
      delay(50);
    }

    else {                          //used if the button was not clicked
      analogWrite(motor2Speed, 0);  //Sets speed of motor 2 to 0 so it doesn't move
      analogWrite(motor1Speed, 0);  //Sets speed of motor 1 to 0 so it doesn't move
    }
  }

  if (isMoving) {  //checks if the boolean varibale is true (meaning the button is pressed and therfore the car should move)

    if (distance < 30 && distance != 0) { //sets the distance a bit higher to get the gradual stop 
      gradualStop(slow); //runs the gradul Stop method to slow speed
    }
    if (distance < 25 && distance != 0) {  //sets the distance lower to stop 
      counter++; //incrments coutner as this is the first time it detecets

      if (counter == 1) { //checks if counter == 1 (is when it first detectes)
        Stop();  //runs the stop method 
        delay(1000); //puts delay as without it doesnt work
        counter++; //increments counter to do the next part (right turn)
      }
    }

    if (counter == 2) { //checks if counter == 2
      //QUICK NOTES: 
      //MORE SPEED = LESS ANGLE 
      //LEss ANGLE = LESS DELAY
      
      //first right
      turnRight(); //runs right method
      delay(800); //puts delay as without it the code would not even run this as it prcoess it fast 
      Stop(); //stops the car 
      goStraight(); //runs the gostaight method as after turning it should move straight 

      //first left
      delay(400); //delay is put here for reasons stated above 
      turnLeft(); //same logic but for left turn
      Stop();
      goStraight();

      //second left
      delay(400); //same logic 
      turnLeft();
      delay(700);  //change this value to adjust the magnitude of the turn
      goStraight();

      //third left
      delay(100); //same logic 
      turnLeft();
      delay(650);  //change this value to adjust the magnitude of the turn
      goStraight();

      //second right
      delay(750);  //same logic    
      turnRight();
      delay(900);  //change this value to adjust the magnitude of the turn
      goStraight();
      
      //third right
      delay(100); //same logic
      turnRight();

      delay(1000);  //change this value to adjust the magnitude of the turn
      goStraight();

      counter++; //increments counter to perform the final stop
    }
    
    if (counter == 3) {
      digitalWrite(LED, LOW);  //used for testing purposes  
      goStraight(); //continues doing straight, until something is detected as highlted in the recusion stop method
      delay(750);
  
      RecursionStop(); //runs the recursion stop method and does the logic indetifed in the comments for the method 
      
      
      }
      else {
        //testing purpose
        digitalWrite(LED, HIGH);

        // Drive the motors forward
        normalSpeed();
      }
    }
  }
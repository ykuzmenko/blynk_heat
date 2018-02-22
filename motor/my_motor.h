
class MyMotor
{
int open_sensor_pin;
int closed_sensor_pin;
int enable_pin;
int input1_pin;
int input2_pin;

public:

  MyMotor(int open_sensor_pin, int closed_sensor_pin, int enable_pin, int input1_pin, int input2_pin ) {
    open_sensor_pin = open_sensor_pin;
    closed_sensor_pin = closed_sensor_pin;
    enable_pin = enable_pin;
    input1_pin = input1_pin;
    input2_pin = input2_pin;
  }

  void setup() {
      pinMode(input1_pin, OUTPUT);
      pinMode(input2_pin,OUTPUT);
      pinMode(enable_pin, OUTPUT);
      pinMode(closed_sensor_pin, INPUT);
      pinMode(open_sensor_pin,INPUT);
  };

  enum moveDirections {
    closing,
    opening,
    stopped
  };

  enum positionStates {
    opened,
    closed,
    state_undefined
  };

  enum moveDirections moveDir = stopped;
  enum positionStates posState;

  int sensorTriggered(int sensorPin) {
    return digitalRead(sensorPin) == HIGH;
  };

  void motorEnable(){
    digitalWrite(enable_pin, HIGH);
  };

  void motorDisable() {
    digitalWrite(enable_pin, LOW);
    moveDir = stopped;
  };

  void moveClose() {
    motorEnable();
    moveDir = closing;
    digitalWrite(input2_pin, HIGH);
    digitalWrite(input1_pin, LOW); 
  };

  void moveOpen() {
    motorEnable();
    moveDir = opening;
    digitalWrite(input1_pin, HIGH); 
    digitalWrite(input2_pin, LOW);  

  };

  void open() {
    moveOpen();
  }

  void close() {
    moveClose();
  }

  void processMotion() {
      switch (moveDir) {
        case closing:
          if (sensorTriggered(closed_sensor_pin)) {
            motorDisable();
          }
          break;

        case opening:
          if (sensorTriggered(open_sensor_pin)) {
            motorDisable();          
          }
          break;
       
        case stopped:
          break;
      }  
  };

  positionStates getPosition() {
    // Sensors can trigger only when motor is on

    if (moveDir == opening || moveDir == closing) {
      return state_undefined; 
    }

    if (moveDir == stopped) {
      moveClose();
      if (sensorTriggered(closed_sensor_pin)) {
        return closed;
      }
      if (sensorTriggered(open_sensor_pin)) {
        return opened;
      }
      
      moveOpen();
      if (sensorTriggered(open_sensor_pin)) {
        return opened;
      }
      if (sensorTriggered(closed_sensor_pin)) {
        return closed;
      }
      return state_undefined;
    }
    return  state_undefined;
  };

};


class MyMotor
{
int open_sensor_pin;
int closed_sensor_pin;
int enable_pin;
int input1_pin;
int input2_pin;

public:
  MyMotor(int open_sensor_pin, int closed_sensor_pin, int enable_pin, int input1_pin, int input2_pin ) {
    this->open_sensor_pin = open_sensor_pin;
    this->closed_sensor_pin = closed_sensor_pin;
    this->enable_pin = enable_pin;
    this->input1_pin = input1_pin;
    this->input2_pin = input2_pin;
  }

  void setup() {
      pinMode(this->input1_pin, OUTPUT);
      pinMode(this->input2_pin,OUTPUT);
      pinMode(this->enable_pin, OUTPUT);
      pinMode(this->closed_sensor_pin, INPUT);
      pinMode(this->open_sensor_pin,INPUT);
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
    return digitalRead(sensorPin) == LOW;
  };

  void motorEnable(){
    digitalWrite(this->enable_pin, HIGH);
  };

  void motorDisable() {
    digitalWrite(this->enable_pin, LOW);
    this->moveDir = this->stopped;
  };

  void moveClose() {
    this->motorEnable();
    this->moveDir = this->closing;
    digitalWrite(this->input2_pin, HIGH);
    digitalWrite(this->input1_pin, LOW); 
  };

  void moveOpen() {
    this->motorEnable();
    this->moveDir = this->opening;
    digitalWrite(this->input1_pin, HIGH); 
    digitalWrite(this->input2_pin, LOW);  

  };

  void open() {
    this->moveOpen();
  }

  void close() {
    this->moveClose();
  }

  void processMotion() {
      switch (this->moveDir) {
        case this->closing:
          if (this->sensorTriggered(this->closed_sensor_pin)) {
            this->motorDisable();
          }
          break;

        case this->opening:
          if (this->sensorTriggered(this->open_sensor_pin)) {
            this->motorDisable();          
          }
          break;
       
        case this->stopped:
          break;
      }  
  };

  positionStates getPosition() {
    // Sensors can trigger only when motor is on

    if (this->moveDir == opening || this->moveDir == closing) {
      return state_undefined; 
    }

    if (this->moveDir == this->stopped) {
      this->moveClose();
      if (sensorTriggered(this->closed_sensor_pin)) {
        return this->closed;
      }
      if (sensorTriggered(this->open_sensor_pin)) {
        return this->opened;
      }
      
      this->moveOpen();
      if (sensorTriggered(this->open_sensor_pin)) {
        return this->opened;
      }
      if (sensorTriggered(this->closed_sensor_pin)) {
        return this->closed;
      }
      return this->state_undefined;
    }
    return this->state_undefined;
  };
};

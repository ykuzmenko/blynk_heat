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
}

void motorEnable(){
  digitalWrite(ENABLE_PIN, HIGH);
}

void motorDisable() {
  digitalWrite(ENABLE_PIN, LOW);
  moveDir = stopped;
}

void moveClose() {
  motorEnable();
  moveDir = closing;
  digitalWrite(INPUT2_PIN, HIGH);
  digitalWrite(INPUT1_PIN, LOW); 
}

void moveOpen() {
  motorEnable();
  moveDir = opening;
  digitalWrite(INPUT1_PIN, HIGH); 
  digitalWrite(INPUT2_PIN, LOW);  

}

void processMotion() {
    switch (moveDir) {
      case closing:
        if (sensorTriggered(CLOSED_SENSOR_PIN)) {
          motorDisable();
        }
        break;

      case opening:
        if (sensorTriggered(OPEN_SENSOR_PIN)) {
          motorDisable();          
        }
        break;
     
      case stopped:
        break;
    }  
}

positionStates getPosition() {
  // Sensors can trigger only when motor is on

  if (moveDir == opening || moveDir == closing) {
    return state_undefined; 
  }

  if (moveDir == stopped) {
    moveClose();
    if (sensorTriggered(CLOSED_SENSOR_PIN)) {
      return closed;
    }
    if (sensorTriggered(OPEN_SENSOR_PIN)) {
      return opened;
    }
    
    moveOpen();
    if (sensorTriggered(OPEN_SENSOR_PIN)) {
      return opened;
    }
    if (sensorTriggered(CLOSED_SENSOR_PIN)) {
      return closed;
    }
    return state_undefined;
  }
  return  state_undefined;
}



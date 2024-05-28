int isActuatorUp(){
  return digitalRead(I1_9);
}

int isActuatorDown(){
  return digitalRead(I1_8);
}

int isPartAtActuator(){
  return !digitalRead(I1_10);
}

int isPartAtSensor(){
  return !digitalRead(I1_11);
}

void moveConveyorAct(){
  digitalWrite(Q1_0, HIGH);
  while(!isPartAtActuator());
  digitalWrite(Q1_0, LOW);
}

void moveConveyorSens(){
  digitalWrite(Q1_1, HIGH);
  while(!isPartAtSensor());
  digitalWrite(Q1_1, LOW);
  server.println("A_END_EXE");
  xSemaphoreGive(semExe);
}

void transition_send(){
  digitalWrite(Q1_1, HIGH);
}

void transition_rcv(){
  digitalWrite(Q1_0, HIGH);
  while(!isPartAtSensor());
  server.println("A_RCV");
  digitalWrite(Q1_0, LOW);
}

void conveyorStop(){
  digitalWrite(Q1_0, LOW);
  digitalWrite(Q1_1, LOW);
}

void actuatorStop(){
  digitalWrite(Q1_2, LOW);
  digitalWrite(Q1_3, LOW);
}

void partProcessing(){
  digitalWrite(Q1_3, HIGH);
  while(!isActuatorDown());
  digitalWrite(Q1_3, LOW);
  delay(5000);
  digitalWrite(Q1_2, HIGH);
  while(!isActuatorUp());
  digitalWrite(Q1_2, LOW);
}

void execution (){
  while(!isPartAtSensor());
  moveConveyorAct();
  partProcessing();
  moveConveyorSens();
}

void calibration(){
  if(!digitalRead(I1_9)){
    digitalWrite(Q1_2, HIGH);
    while(!isActuatorUp());
    digitalWrite(Q1_2, LOW);
  }
}
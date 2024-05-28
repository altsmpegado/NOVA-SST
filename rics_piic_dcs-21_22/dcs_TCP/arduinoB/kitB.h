int isActuatorUp(){
  return digitalRead(I1_4);
}

int isActuatorDown(){
  return digitalRead(I1_5);
}

int isPartAtActuator(){
  return !digitalRead(I1_3);
}

int isPartAtSensor(){
  return !digitalRead(I1_2);
}

void moveConveyorAct(){
  digitalWrite(Q0_0, HIGH);
  while(!isPartAtActuator());
  digitalWrite(Q0_0, LOW);
}

void moveConveyorSens(){
  digitalWrite(Q0_1, HIGH);
  while(!isPartAtSensor());
  digitalWrite(Q0_1, LOW);
  server.println("B_END_EXE");
  xSemaphoreGive(semExe);
}

void transition_send(){
  digitalWrite(Q0_1, HIGH);
}

void transition_rcv(){
  digitalWrite(Q0_0, HIGH);
  while(!isPartAtSensor());
  server.println("B_RCV");
  digitalWrite(Q0_0, LOW);
}

void conveyorStop(){
  digitalWrite(Q0_0, LOW);
  digitalWrite(Q0_1, LOW);
}

void actuatorStop(){
  digitalWrite(Q1_0, LOW);
  digitalWrite(Q1_1, LOW);
}

void partProcessing(){
  digitalWrite(Q1_1, HIGH);
  while(!isActuatorDown());
  digitalWrite(Q1_1, LOW);
  delay(5000);
  digitalWrite(Q1_0, HIGH);
  while(!isActuatorUp());
  digitalWrite(Q1_0, LOW);
}

void execution (){
  while(!isPartAtSensor());
  moveConveyorAct();
  partProcessing();
  moveConveyorSens();
}

void calibration(){
  if(!digitalRead(I1_4)){
    digitalWrite(Q1_0, HIGH);
    while(!isActuatorUp());
    digitalWrite(Q1_0, LOW);
  }
}
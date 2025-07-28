const int step1 = 9;
const int step2 = 10;
const int step3 = 11;
const int step4 = 12;
int step_number = 0;
int maxDist = 100;
int cycleTotal = 10;
int speed = 5;

void setup() {
Serial.begin(9600);
Serial.println("Starting Test \n");
pinMode(step1, OUTPUT);
pinMode(step2, OUTPUT);
pinMode(step3, OUTPUT);
pinMode(step4, OUTPUT);

int cycle = 0;
  for(int count = 0; count < cycleTotal; count++)
  {
    for(int i = 0; i < maxDist; i++){
    OneStep(false);
    delay(speed);
    }
    for(int i = 0; i < maxDist; i++){
    OneStep(true);
    delay(speed);
    }
    cycle++;
    Serial.print("cycle: ");
    Serial.print(cycle);
    Serial.println();
  }
}

void loop() {
}

void OneStep(bool dir) 
{
  if(dir){
    switch(step_number){
  case 0:
  digitalWrite(step1, HIGH);
  digitalWrite(step2, LOW);
  digitalWrite(step3, LOW);
  digitalWrite(step4, LOW);
  break;
  case 1:
  digitalWrite(step1, LOW);
  digitalWrite(step2, HIGH);
  digitalWrite(step3, LOW);
  digitalWrite(step4, LOW);
  break;
  case 2:
  digitalWrite(step1, LOW);
  digitalWrite(step2, LOW);
  digitalWrite(step3, HIGH);
  digitalWrite(step4, LOW);
  break;
  case 3:
  digitalWrite(step1, LOW);
  digitalWrite(step2, LOW);
  digitalWrite(step3, LOW);
  digitalWrite(step4, HIGH);
  break;
  }
  }else{
    switch(step_number){
        case 0:
  digitalWrite(step1, LOW);
  digitalWrite(step2, LOW);
  digitalWrite(step3, LOW);
  digitalWrite(step4, HIGH);
  break;
  case 1:
  digitalWrite(step1, LOW);
  digitalWrite(step2, LOW);
  digitalWrite(step3, HIGH);
  digitalWrite(step4, LOW);
  break;
  case 2:
  digitalWrite(step1, LOW);
  digitalWrite(step2, HIGH);
  digitalWrite(step3, LOW);
  digitalWrite(step4, LOW);
  break;
  case 3:
  digitalWrite(step1, HIGH);
  digitalWrite(step2, LOW);
  digitalWrite(step3, LOW);
  digitalWrite(step4, LOW);
    }
  }
      step_number++;
  if(step_number > 3){
    step_number = 0;
    }
}
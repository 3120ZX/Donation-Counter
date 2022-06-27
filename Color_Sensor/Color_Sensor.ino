#define S0 12
#define S1 13
#define S2 26
#define S3 27
#define OUT 5

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
int r, g, b;

//Frekuensi PWM TCS3200 untuk uang : [1.000,2.000,5.000,10.000,20.000,50.000,10.000]
const int rows = 2;
const int columns = 4;
int moneyFreq[rows][columns] = {
  {1,1,1,1000},
  {2,2,2,2000},
  {14,21,20,5000},
  {3,3,3,10000},
  {4,4,4,20000},
  {5,5,5,50000},
  {6,6,6,100000}
  };


void setup() {
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(OUT, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  // Begins serial communication
  Serial.begin(9600);
}

void loop(){
      r = process_red_value();
      delay(200);
      g = process_green_value();
      delay(200);
      b = process_blue_value();
      delay(200);
      Serial.print("r = ");
      Serial.print(r);
      Serial.print(" ");
      Serial.print("g = ");
      Serial.print(g);
      Serial.print(" ");
      Serial.print("b = ");
      Serial.print(b);
      Serial.print(" ");
      Serial.println();
      delay(1000);
      moneyChecker();
      delay(1000);
    }
int process(){
  }
  
int process_red_value(){
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  int pulse_length = pulseIn(OUT, LOW);
  return pulse_length;
  }

int process_green_value(){
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  int pulse_length = pulseIn(OUT, LOW);
  return pulse_length;
  }
  
int process_blue_value(){
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  int pulse_length = pulseIn(OUT, LOW);
  return pulse_length;
  }

int moneyChecker(){
  int mark = 0;
  for ( int i = 0; i < rows; ++i ) {
    if ((moneyFreq[i][0]-2)< r && r <(moneyFreq[i][0]+2) &&
        (moneyFreq[i][1]-2)< g && g <(moneyFreq[i][1]+2) &&
        (moneyFreq[i][2]-2)< b && b <(moneyFreq[i][2]+2)){
      Serial.println(moneyFreq[i][3]);
      break;
      }   
    else{
      continue;
      }
    }
    
  Serial.println("Done Checking");
  }

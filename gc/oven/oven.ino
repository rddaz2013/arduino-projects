//pins used
int heaterPin = 9;
int therm = 0;
int therm2 = 1;
int therm3 = 2;
int switchPin = 3;
int lightPin = 4;
int pumpPin = 5;
//temperature setpoint
// 550 is good for testing (around 25degC)
// 850 is around 40degC
int defaultSetpoint = 800; //temperature controlled to setpoint
int setpoint = defaultSetpoint;
//switch
int switchState = 0; //switch resets timer
//timer
unsigned long startMillis = 0; //timer
//proportional controller
int kp = 10;
int ki = 1;
int kd = 1;//constant, multiplied by tempDiff to give output
int output = 0; //varies current, 0 = off, 255 = full power

int serialOutput=0; // By default serial output of data is off, unti
                      // 'start' command is issued by computer.
                      
int preErr = 0;
int integral = 0;
int dt = 1;  //loop period (sec)

String readString;

int parseCmd(String cmdLine, String *key,String *value) {
  int equalsPos;
  equalsPos = cmdLine.indexOf('=');
  if (equalsPos==-1) {
    *key=cmdLine;
    *value="";
  }
  else {
    *key=cmdLine.substring(0,equalsPos);
    *value=cmdLine.substring(equalsPos+1);
  }
  return(equalsPos);
}





///////////////////////////////////////////////////////

void setup() {                

  Serial.begin(9600);
  startMillis = millis();
  pinMode(heaterPin, OUTPUT);
  pinMode(therm, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(lightPin, OUTPUT);
  digitalWrite(switchPin, HIGH);  
  preErr = 0;
  integral = 0;
}
///////////////////////////////////////////////////////

void loop() {
  int output;
  String k,v;
  ///////////////////////////////////////////////////////
  // reset switch
  ///////////////////////////////////////////////////////
  switchState = digitalRead(switchPin);
  unsigned long time = millis();
  if (switchState == LOW) {
    startMillis = time;
  }
  ////////////////////////////////////////////////
  // temperature controller.
  ////////////////////////////////////////////////
  int val = analogRead( therm );
  int val2 = analogRead( therm2 );
  int val3 = analogRead( therm3 );

    int tempDiff = setpoint - val;
    integral = integral + tempDiff * dt;
    int derivative = (tempDiff - preErr) / dt;
    output = tempDiff * kp + integral * ki + derivative * kd;
    preErr = tempDiff;
    
    if (output < 0) {
      output = 0;
    }   
    if (output > 255) {
      output = 255;
    }

  analogWrite (heaterPin,output);
  ////////////////////////////////////////////////
  // respond to commands from serial.
  ////////////////////////////////////////////////
  while (Serial.available()) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      readString += c;
    }

  }

    //Serial.println (readString);
    if (readString.length()>0) {
      parseCmd(readString, &k,&v);
      Serial.print("parseCmd k=");
      Serial.println(k);
      Serial.print("parseCmd v=");
      Serial.println(v);
    
    if (v=="") {
      if (k=="setpoint") {
        Serial.print("setpoint=");
        Serial.println(setpoint);
      }
      if (k=="kp") { 
        Serial.print("kp=");
        Serial.println(kp); 
      }    
      if (k=="ki") { 
        Serial.print("ki=");
        Serial.println(ki); 
      }
      if (k=="kd") { 
        Serial.print("kd=");
        Serial.println(kd); 
      }
      if (k=="start") {
        serialOutput=1;
      }
      if (k=="stop") {
        serialOutput=0;
      }
      if (k=="settings") {
        Serial.print("Set,");
        Serial.print(setpoint);
        Serial.print(",");
        Serial.print(kp);
        Serial.print(",");
        Serial.print(ki);
        Serial.print(",");
        Serial.print(kd);
        Serial.print(",");
        Serial.print(tempDiff);
        Serial.print(",");
        Serial.println(integral);
      }

    }
    
    else {
      if (k=="setpoint") {    //change setpoint
        setpoint = v.toInt();
      }
      if (k=="kp") {        //change gain
        kp = v.toInt();
      }
      if (k=="ki") {        //change gain
        ki = v.toInt();
      }
      if (k=="kd") {        //change gain
        kd = v.toInt();
      }
      if (k=="pump") {
        if (v=="on") {
        digitalWrite(pumpPin, HIGH);
        Serial.println("Pump is switched on");
        }
        if (v=="off") {
        digitalWrite(pumpPin, LOW);
        Serial.println("Pump is switched off");
       }
      }

    }
    
    readString = "";

    }
  
  //Serial.print(time-startMillis);

if (serialOutput==1){
  Serial.print("data,");
  Serial.print(time-startMillis);
  Serial.print(",");
  Serial.print(val);
  Serial.print(",");
  Serial.print(val2);
  Serial.print(",");
  Serial.print(val3);
  Serial.print(",");
  Serial.print(val3-val2);
  Serial.print(",");
  Serial.print(output);
  Serial.print(",");
  Serial.print(integral);
  Serial.println();
}
  
  delay(dt * 1000);

}






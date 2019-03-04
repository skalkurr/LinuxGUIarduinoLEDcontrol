
char ch;//to store serially received data

void setup() {
  //LED pin direction set as output
  pinMode(LED_BUILTIN,OUTPUT);
  //Serial initialization with 9600 baud rate
  Serial.begin(9600);
  //LED init status
  digitalWrite(LED_BUILTIN,LOW);
}

void loop() {
  while(Serial.available()>0)//Check for any serial data
  {
    ch=Serial.read();//read
      if(ch=='N'){
        digitalWrite(LED_BUILTIN,HIGH);//LED on
      }else digitalWrite(LED_BUILTIN,LOW);//LED off
  }
}

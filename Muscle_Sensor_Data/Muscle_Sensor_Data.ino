void setup() {
  // put your setup code here, to run once:
  pinMode(3, INPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.println(digitalRead(3));
  Serial.println(analogRead(A0));
}

const int LED = 13;

void setup() {
  // 2番ピンをアウトプットに設定
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

byte inputData;
void loop() {
  // シリアルデータを受信したら処理を実行する
  if (Serial.available() > 0) {
    inputData = Serial.read();
    switch(inputData){
      case '0':
        digitalWrite(LED, LOW);
        Serial.println("ランプ消灯");
        break;
      case '1':
        digitalWrite(LED, HIGH);
        tone(12,930,400);
        Serial.println("ランプ点灯");
        break;
      default:
        break;
    }
  }
}

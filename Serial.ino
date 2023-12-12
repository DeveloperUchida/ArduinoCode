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
      case 'c':tone(12,262,200); break;
      case 'v':tone(12,294,200); break;
      case 'b':tone(12,330,200); break;
      case 'n':tone(12,349,200); break;
      case 'm':tone(12,392,200); break;
      case ',':tone(12,440,200); break;
      case '.':tone(12,494,200); break;
      case '/':tone(12,523,200); break;
      case '0':
        digitalWrite(LED, LOW);
        Serial.println("turn off");
        break;
      case '1':
        digitalWrite(LED, HIGH);
        tone(12,440,200);
        Serial.println("turn on");
        break;
      default:
        break;
    }
  }
}

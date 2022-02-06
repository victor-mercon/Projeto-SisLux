//led_wifi_blink: busca controla o led para indentificar comunicação via wifi
void led_wifi_blink(){
  int state=0;
  for(int i=0;i<10;i++){
    digitalWrite(LedWiFi, (state) ? HIGH : LOW);
    state != state;
    delay(100);
  }
}

//led_bluetooth_blink: busca controla o led para indentificar comunicação via bluetooth
void led_bluetooth_blink(){
  int state=0;
  for(int i=0;i<10;i++){
    digitalWrite(LedBluetooth, (state) ? HIGH : LOW);
    state != state;
    delay(100);
  }
}

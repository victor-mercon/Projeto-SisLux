
//scanCorrente: faz a leitura da corrente e busca o ultimo consumo salvo no banco de dados e atualiza o consumo atual
void scanCorrente() {
  Irms = emon1.calcIrms(1996);    //Calcula IRMS
  Serial.println(Irms);
  array_Potencias[aux_potencias] = Irms * VOLTAGE_AC;
  aux_potencias++;
  for (int i = 0; i < 300; i++) {
    Serial.print(array_Potencias[i]);
    Serial.print(",");
  }
  Serial.println(" ");
  currentMillis3 = millis();
  if (currentMillis3 - startMillis3 >= period3) {
    kwh = 0;
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?apiCommand=getCellValue&celladress=" + mes_aux + SeriesNumber;
      Serial.println("Recuperando potencia do banco de dados");
      Serial.println("Making a request");
      led_wifi_blink();
      http.begin(url.c_str()); //Specify the URL and certificate
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      int httpCode = http.GET();
      String payload;
      if (httpCode > 0) { //checa pelo codigo de retorno
        payload = http.getString();
        kwh_atual = payload.toInt();
      } else {
        //Serial.println("Error on HTTP request");
      }
      http.end();
    }
    for (int i = 0; i < 300; i++) {
      array_Potencias[i] = array_Potencias[i]/1000;
    }
    for (int i = 0; i < 300; i++) {
      kwh = kwh +array_Potencias[i];
    }
    kwh_atual = kwh_atual + kwh;
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      Serial.println(kwh);
      String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?apiCommand=setCellValue&celladress=" + mes_aux + SeriesNumber + "&newvalue=" + kwh_atual;
      Serial.println("Atualizando potencia do banco de dados");
      Serial.println("Making a request");
      led_wifi_blink();
      http.begin(url.c_str()); //Specify the URL and certificate
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      int httpCode = http.GET();
      String payload;
      if (httpCode > 0) { //Checa pelo codigo de retorno
        payload = http.getString();
        kwh_atual = payload.toInt();
      } else {
        //Serial.println("Error on HTTP request");
      }
      http.end();
    }
    for (int i = 0; i < 300; i++) {
      array_Potencias[i] = 0;
    }
    aux_potencias = 0;
    startMillis3 = currentMillis3;
  }
}

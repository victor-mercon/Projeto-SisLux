//ScanPost: busca informações no banco de dados para atualizar o estados dos grupos durante a função setup
void ScanPost() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?apiCommand=scanCellValue&celladress1=A" + SeriesNumber + "&celladress2=B" + SeriesNumber + "&celladress3=C" + SeriesNumber + "&celladress4=D" + SeriesNumber + "&celladress5=E" + SeriesNumber + "&celladress6=F" + SeriesNumber + "&celladress7=G" + SeriesNumber + "&celladress8=H" + SeriesNumber;
    //   Serial.print(url);
    Serial.println("Fazendo scaneamento do banco de dados");
    Serial.println("Making a request");
    led_wifi_blink();
    http.begin(url.c_str()); //especifica a url e o certificado
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    String payload;
    Serial.println(httpCode);
    if (httpCode > 0) { //Checa pelo codigo de retorno
      payload = http.getString();

      Serial.println(httpCode);
      Serial.println(payload);
      Serial.println(payload.charAt(0));
      Serial.println(payload.charAt(2));
      Serial.println(payload.charAt(4));
      Serial.println(payload.charAt(6));
      Serial.println(payload.charAt(8));
      Serial.println(payload.charAt(10));
      Serial.println(payload.charAt(12));
      Serial.println(payload.charAt(14));

      state1 = payload.charAt(0) - '0';
      digitalWrite(G1, (state1) ? HIGH : LOW);

      state2 = payload.charAt(2) - '0';
      digitalWrite(G2, (state2) ? HIGH : LOW);

      state3 = payload.charAt(4) - '0';
      digitalWrite(G3, (state3) ? HIGH : LOW);

      state4 = payload.charAt(6) - '0';
      digitalWrite(G4, (state4) ? HIGH : LOW);

      state5 = payload.charAt(8) - '0';
      digitalWrite(G5, (state5) ? HIGH : LOW);

      state6 = payload.charAt(10) - '0';
      digitalWrite(G6, (state6) ? HIGH : LOW);

      state7 = payload.charAt(12) - '0';
      digitalWrite(G7, (state7) ? HIGH : LOW);

      state8 = payload.charAt(14) - '0';
      digitalWrite(G8, (state8) ? HIGH : LOW);

    } else {
      //Serial.println("Error on HTTP request");
    }
    http.end();
  }
}

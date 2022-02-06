/////////////////////////////////////////////////////////////////

//post: atualiza no banco de dados o estados dos grupos e envia o comando correspondente a partir do codigo de retorno para o rele correspondente
void post(String letra, String num, String Value) {
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?apiCommand=setCellValue&celladress=" + letra + num + "&newvalue=" + Value;
    //   Serial.print(url);
    Serial.println("Atualizando o estado dos grupos no banco de dados");
    Serial.println("Making a request");
    led_wifi_blink();
    http.begin(url.c_str()); //especifica o url e o certificado
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    Serial.println(httpCode);
    String payload;
    if (httpCode > 0) { //Checa pelo codigo de retorno
      payload = http.getString();

      Serial.println(httpCode);
      Serial.println(payload);

      if (letra == "A") {

        state1 = payload.toInt();
        digitalWrite(G1, (state1) ? HIGH : LOW);
        
      } else if (letra == "B") {

        state2 = payload.toInt();
        digitalWrite(G2, (state2) ? HIGH : LOW);
        
      } else if (letra == "C") {

        state3 = payload.toInt();
        digitalWrite(G3, (state3) ? HIGH : LOW);
        
      } else if (letra == "D") {

        state4 = payload.toInt();
        digitalWrite(G4, (state4) ? HIGH : LOW);
        
      } else if (letra == "E") {

        state5 = payload.toInt();
        digitalWrite(G5, (state5) ? HIGH : LOW);
        
      } else if (letra == "F") {

        state6 = payload.toInt();
        digitalWrite(G6, (state6) ? HIGH : LOW);
        
      } else if (letra == "G") {

        state7 = payload.toInt();
        digitalWrite(G7, (state7) ? HIGH : LOW);
        
      } else if (letra == "H") {

        state8 = payload.toInt();
        digitalWrite(G8, (state8) ? HIGH : LOW);
        
      }
    }
    else {
      //Serial.println("Error on HTTP request");
    }


    http.end();
  }
}

/////////////////////////////////////////////////////////////////

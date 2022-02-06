//btRead: le as informações passadas por bluetooth e salva as informações na memoria flash
void btRead() {
  if (SerialBT.available()>0) {

    ssid_pass = "";

    ssid_pass = SerialBT.readStringUntil('/');
    Serial.println(ssid_pass);

    for (int i = 0; i < ssid_pass.length(); i++)
    {
      if (ssid_pass.substring(i, i + 1) == ",")
      {
        ssid = ssid_pass.substring(0, i);
        pass = ssid_pass.substring(i + 1);
        Serial.print("SSID = "); Serial.println(ssid);
        Serial.print("Password = "); Serial.println(pass);

        break;
      }
    }
      preferences.putString("ssid", ssid);
      preferences.putString("password", pass);
      Serial.println("*********");
      Serial.print("ssid_pass = ");
      Serial.println(ssid_pass); // Presenta ssid_pass.
      Serial.println("*********");
      Serial.print("ssid = ");
      Serial.println(preferences.getString("ssid", "")); // Presenta ssid_pass.
      Serial.println("*********");
      Serial.print("pass = ");
      Serial.println(preferences.getString("password", "")); // Presenta ssid_pass.
      ESP.restart();
  }
}

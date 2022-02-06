/////////////////////////////////////////////////////////////////

//click: indentifica qual botão foi pressionado e chama a função para alterar seu estado no banco de dados
void click(Button2& btn) {
  if (btn == button1) {
    Serial.println("Grupo 1 clicked");
    if(state1==0){
      post("A",SeriesNumber,"1");
    }else{
      post("A",SeriesNumber,"0");
    }
  } else if (btn == button2) {
    Serial.println("Grupo 2 clicked");
    if(state2==0){
      post("B",SeriesNumber,"1");
    }else{
      post("B",SeriesNumber,"0");
    }
  }else if (btn == button3) {
    Serial.println("Grupo 3 clicked");
    if(state3==0){
      post("C",SeriesNumber,"1");
    }else{
      post("C",SeriesNumber,"0");
    }
  }else if (btn == button4) {
    Serial.println("Grupo 4 clicked");
    if(state4==0){
      post("D",SeriesNumber,"1");
    }else{
      post("D",SeriesNumber,"0");
    }
  }else if (btn == button5) {
    Serial.println("Grupo 5 clicked");
    if(state5==0){
      post("E",SeriesNumber,"1");
    }else{
      post("E",SeriesNumber,"0");
    }
  } else if (btn == button6) {
    Serial.println("Grupo 6 clicked");
    if(state6==0){
      post("F",SeriesNumber,"1");
    }else{
      post("F",SeriesNumber,"0");
    }
  }else if (btn == button7) {
    Serial.println("Grupo 7 clicked");
    if(state7==0){
      post("G",SeriesNumber,"1");
    }else{
      post("G",SeriesNumber,"0");
    }
  }else if (btn == button8) {
    Serial.println("Grupo 8 clicked");
    if(state8==0){
      post("H",SeriesNumber,"1");
    }else{
      post("H",SeriesNumber,"0");
    }
  }
}

/////////////////////////////////////////////////////////////////

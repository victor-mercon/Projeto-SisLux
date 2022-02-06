/////////////////////////////////////////////////////////////////

#include <Button2.h>          //biblioteca para controlar os botoes
#include <WiFi.h>             //biblioteca para habilitar wi-fi
#include <HTTPClient.h>       //biblioteca para usar protocolo HTTP
#include "EmonLib.h"          //biblioteca para fazer a leitura do sensor de corrente
#include "time.h"             //biblioteca para definir data com informações da rede
#include "BluetoothSerial.h"  //biblioteca para habilitar o bluetooth
#include <Preferences.h>      //biblioteca para gravação de dados na memoria flash

/////////////////////////////////////////////////////////////////

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

/////////////////////////////////////////////////////////////////

#define VOLTAGE_AC    127.00          // 127 Volts
#define ACS_MPY       15.41           // ganho/calibracao da corrente
#define BUTTON_1_PIN  13              // pino ligado ao botão que controla o grupo 1
#define BUTTON_2_PIN  12              // pino ligado ao botão que controla o grupo 2
#define BUTTON_3_PIN  14              // pino ligado ao botão que controla o grupo 3
#define BUTTON_4_PIN  27              // pino ligado ao botão que controla o grupo 4
#define BUTTON_5_PIN  26              // pino ligado ao botão que controla o grupo 5
#define BUTTON_6_PIN  25              // pino ligado ao botão que controla o grupo 6
#define BUTTON_7_PIN  33              // pino ligado ao botão que controla o grupo 7
#define BUTTON_8_PIN  32              // pino ligado ao botão que controla o grupo 8
#define SENSOR_PIN    34              // pino ligado ao sensor de corrente
#define G1            15              // pino ligado ao rele que ativa/desativa o grupo 1
#define G2            2               // pino ligado ao rele que ativa/desativa o grupo 2
#define G3            4               // pino ligado ao rele que ativa/desativa o grupo 3
#define G4            16              // pino ligado ao rele que ativa/desativa o grupo 4
#define G5            17              // pino ligado ao rele que ativa/desativa o grupo 5
#define G6            5               // pino ligado ao rele que ativa/desativa o grupo 6
#define G7            18              // pino ligado ao rele que ativa/desativa o grupo 7
#define G8            19              // pino ligado ao rele que ativa/desativa o grupo 8
#define LedWiFi       22              // pino ligado ao led que indica comunicação via wifi
#define LedBluetooth  23              // pino ligado ao led que indica comunicação bluetooth
#define SeriesNumber  "1"             // numero sequencial que indentifica o dispositivo

/////////////////////////////////////////////////////////////////

BluetoothSerial SerialBT;              //cria objeto bluetooth         

Preferences preferences;               //cria instancia de gravação na memoria flash

/////////////////////////////////////////////////////////////////

const char* ntpServer = "pool.ntp.org";               //server ntp para recuperação da data
const long  gmtOffset_sec = 3600;                     
const int   daylightOffset_sec = 3600;

/////////////////////////////////////////////////////////////////

EnergyMonitor emon1;               //criação de objeto para leitura do sensor de corrente

/////////////////////////////////////////////////////////////////

//variaveisd utilizadas para o calculo de potencia

int array_Potencias[300];
int aux_potencias = 0;
double Irms = 0;
int mes;
String mes_aux;
double kwh;
double kwh_atual;

/////////////////////////////////////////////////////////////////

//variaveis para conecção wi-fi

String ssid_pass;
String ssid;
String pass;
int aux_wifi = 0;
String GOOGLE_SCRIPT_ID = "AKfycbxm2TTT_XWXUSXdSRqAqcXUs5P1geKrwCT1wcv_hOnKXhsn-rZA6YFgjXfkAvylvDFX";

/////////////////////////////////////////////////////////////////

//declaração dos objetos para controlar os botoes

Button2 button1, button2, button3, button4, button5, button6, button7, button8;

/////////////////////////////////////////////////////////////////

//declaração do objeto wi-fi

WiFiClientSecure client;

/////////////////////////////////////////////////////////////////

//declaração dos estados dos grupos

int state1 = 0;
int state2 = 0;
int state3 = 0;
int state4 = 0;
int state5 = 0;
int state6 = 0;
int state7 = 0;
int state8 = 0;

/////////////////////////////////////////////////////////////////

//variaveis para o controle de tempo do acesso das funções

unsigned long startMillis1;
unsigned long currentMillis1;
const unsigned long period1 = 1000;  //o valor e um numero em milisegundos
unsigned long startMillis2;
unsigned long currentMillis2;
const unsigned long period2 = 1000;  //o valor e um numero em milisegundos
unsigned long startMillis3;
unsigned long currentMillis3;
const unsigned long period3 = 300000;  //o valor e um numero em milisegundos

/////////////////////////////////////////////////////////////////

//definição das tasks que serão rodadas

TaskHandle_t Task1;
TaskHandle_t Task2;

/////////////////////////////////////////////////////////////////

void setup() {
  startMillis1 = millis();  //tempo inicial 1
  startMillis2 = millis();  //tempo inicial 2
  startMillis3 = millis();  //tempo inicial 3

  Serial.begin(115200);
  delay(50);
  SerialBT.begin("SisLux1"); //nome do dispositvo Bluetooth

  preferences.begin("credentials", false);//inicializa o namespace onde as variaveis vão ser salvas na flash

  ssid = preferences.getString("ssid", "");//carrega o valor da variavel ssid
  pass = preferences.getString("password", "");//carrega o valor da variavel password

  //defio o modo dos pinos usados 
  pinMode(G1, OUTPUT);
  pinMode(G2, OUTPUT);
  pinMode(G3, OUTPUT);
  pinMode(G4, OUTPUT);
  pinMode(G5, OUTPUT);
  pinMode(G6, OUTPUT);
  pinMode(G7, OUTPUT);
  pinMode(G8, OUTPUT);
  pinMode(LedWiFi, OUTPUT);
  pinMode(LedBluetooth, OUTPUT);

  button1.begin(BUTTON_1_PIN);
  button1.setClickHandler(click);

  button2.begin(BUTTON_2_PIN);
  button2.setClickHandler(click);

  button3.begin(BUTTON_3_PIN);
  button3.setClickHandler(click);

  button4.begin(BUTTON_4_PIN);
  button4.setClickHandler(click);

  button5.begin(BUTTON_5_PIN);
  button5.setClickHandler(click);

  button6.begin(BUTTON_6_PIN);
  button6.setClickHandler(click);

  button7.begin(BUTTON_7_PIN);
  button7.setClickHandler(click);

  button8.begin(BUTTON_8_PIN);
  button8.setClickHandler(click);

  //inicializa o objeto que irá ler o sensor de corrente
  emon1.current(SENSOR_PIN, ACS_MPY);             // Corrente: pino analógico, calibracao.

  if (ssid == "" || pass == "") {
    Serial.println("sem valores salvos para ssid ou password");
  }
  else {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(LedWiFi, LOW);
      delay(250);
      Serial.print(".");
      digitalWrite(LedWiFi, HIGH);
      delay(250);
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    //inicia e captura o tempo
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    ScanPost();

    //cria a task que será exaecutada na função Task1code(), com prioridade 1 e execução co core 0
    xTaskCreatePinnedToCore(
      Task1code,   /* função task */
      "Task1",     /* nome da task. */
      10000,       /* tamanho de Stack da task */
      NULL,        /* parametro da task */
      10,           /* prioridade da task */
      &Task1,      /* Task handle para monitorar a task criada */
      0);          /* vincula task ao core 0 */
    delay(500);

    //cria a task que será exaecutada na função Task2code(), com prioridade 1 e execução co core 1
    xTaskCreatePinnedToCore(
      Task2code,   /* função task. */
      "Task2",     /* nome da task. */
      10000,       /* tamanho de Stack da task */
      NULL,        /* parametro da task */
      10,           /* prioridade da task */
      &Task2,      /* Task handle para monitorar a task criada */
      1);          /* vincula task ao core 0 */
    delay(500);
    if (aux_wifi == 30) {
      Serial.println("SSID ou Senha errados");
    }
    Serial.println(WiFi.localIP());
  }

}

/////////////////////////////////////////////////////////////////

//Task1code: chama a função para fazer leitura dos botões, define o mes atual e chama a função para fazer a leitura do sensor de corrente.
void Task1code( void * pvParameters ) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    button1.loop();
    button2.loop();
    button3.loop();
    button4.loop();
    button5.loop();
    button6.loop();
    button7.loop();
    button8.loop();

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    mes = timeinfo.tm_mon;
    switch (mes) {
      case 0:
        mes_aux = "Q";
        break;
      case 1:
        mes_aux = "R";
        break;
      case 2:
        mes_aux = "S";
        break;
      case 3:
        mes_aux = "T";
        break;
      case 4:
        mes_aux = "U";
        break;
      case 5:
        mes_aux = "V";
        break;
      case 6:
        mes_aux = "W";
        break;
      case 7:
        mes_aux = "X";
        break;
      case 8:
        mes_aux = "Y";
        break;
      case 9:
        mes_aux = "Z";
        break;
      case 10:
        mes_aux = "AA";
        break;
      case 11:
        mes_aux = "AB";
        break;
    }

    currentMillis2 = millis();
    if (currentMillis2 - startMillis2 >= period2)
    {
      startMillis2 = currentMillis2;
      scanCorrente();
    }
  }
}

/////////////////////////////////////////////////////////////////

//Task2code: busca informações no banco de dados para atualizar o estados dos grupos
void Task2code( void * pvParameters ) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    currentMillis1 = millis();
    if (currentMillis1 - startMillis1 >= period1)
    {
      ScanPost();
      startMillis1 = currentMillis1;  
    }
  }
}

/////////////////////////////////////////////////////////////////

//loop: chama a função para ler a comunicação bluetooth se tiver alguma mensagem a ser lida
void loop() {
  if (SerialBT.available()) {
    btRead();
  }
  disableCore0WDT();

}

/////////////////////////////////////////////////////////////////

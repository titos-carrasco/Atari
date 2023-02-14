#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient net;
PubSubClient mqtt(net);

#define WIFI_SSID     "Nombre de la red"
#define WIFI_PASS     "Clave de la red"
#define MQTT_SERVER   "test.mosquitto.org"
#define MQTT_PORT     1883
#define MQTT_CLIENTID ("Node_" + String(ESP.getChipId()))
#define MQTT_TOPIC    "rcr/ajoy"

#define button_up       D2
#define button_down     D3
#define button_left     D4
#define button_right    D5
#define button_trigger  D6

void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);  
  Serial.println();
  Serial.println();

  pinMode(button_up, OUTPUT);
  pinMode(button_down, OUTPUT);
  pinMode(button_left, OUTPUT);
  pinMode(button_right, OUTPUT);
  pinMode(button_trigger, OUTPUT);

  digitalWrite(button_up, LOW);
  digitalWrite(button_down, LOW);
  digitalWrite(button_left, LOW);
  digitalWrite(button_right, LOW);
  digitalWrite(button_trigger, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop(){
  yield();
  if(!wifiConnect()) return;
  if(!mqttConnect()) return;
  mqtt.loop();
}

bool wifiConnect(){
  if( WiFi.status() == WL_CONNECTED )
    return true;

  Serial.print( "Conectando a la WiFi: ." );
  for( int i=0; i<10; i++ ){
    if( WiFi.status() == WL_CONNECTED ){
      Serial.print( " --- " );
      Serial.print( WiFi.localIP() );
      Serial.println( " Ok" );
      return true;
    }

    Serial.print( "." );
    delay( 500 );
    yield();
  }
  Serial.println( "Timeout" );
  return false;
}


bool mqttConnect (){
  if( mqtt.connected() )
    return true;

  Serial.print( "Conectando a MQTT: ." );
  for( int i=0; i<10; i++ ){
    if( mqtt.connect( MQTT_CLIENTID.c_str() ) ){
      Serial.print( " --- " );
      Serial.print(  MQTT_SERVER );
      Serial.print(  ":" );
      Serial.print(  MQTT_PORT );
      Serial.println( " Ok" );

      mqtt.subscribe( MQTT_TOPIC, 0 );
      mqtt.setCallback( doReceiveMessage );
      
      return true;
    }

    Serial.print( "." );
    delay( 500 );
    yield();
  }
  Serial.println( "Timeout" );
  return false;
}

void doReceiveMessage(const char* topic, byte* payload, unsigned int len) { 
  char cmd[len+1];
  memcpy(cmd, payload, len);
  cmd[len] = 0;
  Serial.println(cmd);

  if(strcmp(cmd, "derecha") == 0 ){
    digitalWrite(button_right, HIGH); 
    pausa(500);
    digitalWrite(button_right, LOW); 
  }
  else if(strcmp(cmd, "izquierda") == 0 ){
    digitalWrite(button_left, HIGH); 
    pausa(500);
    digitalWrite(button_left, LOW); 
  }
  else if(strcmp(cmd, "arriba") == 0 ){
    digitalWrite(button_up, HIGH); 
    pausa(500);
    digitalWrite(button_up, LOW); 
  }
  else if(strcmp(cmd, "abajo") == 0 ){
    digitalWrite(button_down, HIGH); 
    pausa(500);
    digitalWrite(button_down, LOW); 
  }
  else if(strcmp(cmd, "disparo") == 0 ){
    digitalWrite(button_trigger, HIGH); 
    pausa(500);
    digitalWrite(button_trigger, LOW); 
  }
 
}

void pausa(unsigned long ms){
  unsigned t = millis() + ms;
  while(millis() < t) yield();  
}

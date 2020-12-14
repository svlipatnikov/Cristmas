byte mqttButtons = 0;

// подключение к MQTT 
const char *mqtt_server = "mqtt.dioty.co";          // Имя сервера MQTT
const int   mqtt_port = 1883;                       // Порт для подключения к серверу MQTT
const char *mqtt_user = "sv.lipatnikov@gmail.com";  // Логин от сервера
const char *mqtt_pass = "83eb5858";                 // Пароль от сервера

//топики 
const char topic_buttons[] = "/sv.lipatnikov@gmail.com/cristmas/buttons";
const char topic_buttons_state[] = "/sv.lipatnikov@gmail.com/cristmas/buttons_state";
const char topic_mode[] = "/sv.lipatnikov@gmail.com/cristmas/mode";
const char topic_mode_state[] = "/sv.lipatnikov@gmail.com/cristmas/mode_state";

//===================================================================================================

void Connect_mqtt(const char* client_name) {
  if (WiFi.status() == WL_CONNECTED) {
    client.setServer(mqtt_server, mqtt_port);            
    if (client.connect(client_name, mqtt_user, mqtt_pass)) 
      client.setCallback(mqtt_get);
  }
}


// Функция отправки int на сревер mqtt
void MQTT_publish_int(const char* topic , int data){
  char msg[5];
  sprintf (msg, "%u", data);    
  client.publish(topic, msg, true);
}


// Функция отправки float на сревер mqtt
void MQTT_publish_float(const char* topic , float data){
  char msg[5];
  dtostrf(data, 4, 1, msg);
  //sprintf (msg, "%2.1f", data);    
  client.publish(topic, msg, true);
}

// получение данных от сервера
void mqtt_get(char* topic, byte* payload, unsigned int length) {
 
  char localPayload[length + 1]; // создаем копию полученных данных
  for (int i = 0; i < length; i++) { 
    localPayload[i] = (char)payload[i]; 
  }
  localPayload[length] = 0;
  

  if (strcmp(topic, topic_mode) == 0) {
    int ivalue = 0; sscanf(localPayload, "%d", &ivalue);
    thisMode = (int)ivalue; 
    MQTT_publish_int(topic_mode_state, thisMode);  
    
    AUTOPLAY = false;  
    loadingFlag = true;
    gamemodeFlag = false;
    FastLED.clear();
    FastLED.show();
  }
  else if (strcmp(topic, topic_buttons) == 0) {
    int ivalue = 0; sscanf(localPayload, "%d", &ivalue);
    mqttButtons = (int)ivalue; 
    MQTT_publish_int(topic_buttons_state, mqttButtons); 
    controlFlag = true;   
  }
}

void MQTT_subscribe() {
  client.subscribe(topic_buttons);
  client.subscribe(topic_mode);    
}

// ************************ МАТРИЦА *************************

#define BRIGHTNESS 150        // стандартная маскимальная яркость (0-255)
#define CURRENT_LIMIT 1500    // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH 10              // ширина матрицы
#define HEIGHT 10             // высота матрицы
#define SEGMENTS 1            // диодов в одном "пикселе" (для создания матрицы из кусков ленты)

#define COLOR_ORDER GRB       // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE 0         // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE 0    // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION 1     // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
// при неправильной настрйоке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
// шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/
/*
#define MCU_TYPE 1            // микроконтроллер: 
//                            0 - AVR (Arduino NANO/MEGA/UNO)
//                            1 - ESP8266 (NodeMCU, Wemos D1)
//                            2 - STM32 (Blue Pill)
*/
// ******************** ЭФФЕКТЫ И РЕЖИМЫ ********************
#define D_TEXT_SPEED 150      // скорость бегущего текста по умолчанию (мс)
#define D_EFFECT_SPEED 80     // скорость эффектов по умолчанию (мс)
#define D_GAME_SPEED 250      // скорость игр по умолчанию (мс)
#define D_GIF_SPEED 80        // скорость гифок (мс)
#define DEMO_GAME_SPEED 120   // скорость игр в демо режиме (мс)

boolean AUTOPLAY = 1;         // 0 выкл / 1 вкл автоматическую смену режимов (откл. можно со смартфона)
int AUTOPLAY_PERIOD = 10;     // время между авто сменой режимов (секунды)
#define IDLE_TIME 60          // время бездействия кнопок или Bluetooth (в секундах) после которого запускается автосмена режимов и демо в играх

// о поддерживаемых цветах читай тут https://alexgyver.ru/gyvermatrixos-guide/
#define GLOBAL_COLOR_1 CRGB::Green    // основной цвет №1 для игр
#define GLOBAL_COLOR_2 CRGB::Orange   // основной цвет №2 для игр

#define SCORE_SIZE 0          // размер символов счёта в игре. 0 - маленький для 8х8 (шрифт 3х5), 1 - большой (шрифт 5х7)
#define FONT_TYPE 1           // (0 / 1) два вида маленького шрифта в выводе игрового счёта

// ************** ОТКЛЮЧЕНИЕ КОМПОНЕНТОВ СИСТЕМЫ (для экономии памяти) *************
// внимание! отключение модуля НЕ УБИРАЕТ его эффекты из списка воспроизведения!
// Это нужно сделать вручную во вкладке custom, удалив ненужные функции

#define USE_BUTTONS 0         // использовать физические кнопки управления играми (0 нет, 1 да)
#define BT_MODE 0             // использовать блютус (0 нет, 1 да)
#define USE_NOISE_EFFECTS 1   // крутые полноэкранные эффекты (0 нет, 1 да) СИЛЬНО ЖРУТ ПАМЯТЬ!!!11
#define USE_FONTS 1           // использовать буквы (бегущая строка) (0 нет, 1 да)
#define USE_CLOCK 0           // использовать часы (0 нет, 1 да)

// игры
#define USE_SNAKE 1           // игра змейка (0 нет, 1 да)
#define USE_TETRIS 1          // игра тетрис (0 нет, 1 да)
#define USE_MAZE 0            // игра лабиринт (0 нет, 1 да)
#define USE_RUNNER 0          // игра бегалка-прыгалка (0 нет, 1 да)
#define USE_FLAPPY 0          // игра flappy bird
#define USE_ARKAN 1           // игра арканоид

// ****************** ПИНЫ ПОДКЛЮЧЕНИЯ *******************
#define LED_PIN 0           // пин ленты

// ******************************** ДЛЯ РАЗРАБОТЧИКОВ ********************************
#define DEBUG 0
#define NUM_LEDS WIDTH * HEIGHT * SEGMENTS

#define RUNNING_STRING 0
#define CLOCK_MODE 1
#define GAME_MODE 2
#define MADNESS_NOISE 3
#define CLOUD_NOISE 4
#define LAVA_NOISE 5
#define PLASMA_NOISE 6
#define RAINBOW_NOISE 7
#define RAINBOWSTRIPE_NOISE 8
#define ZEBRA_NOISE 9
#define FOREST_NOISE 10
#define OCEAN_NOISE 11
#define SNOW_ROUTINE 12
#define SPARKLES_ROUTINE 13
#define MATRIX_ROUTINE 14
#define STARFALL_ROUTINE 15
#define BALL_ROUTINE 16
#define BALLS_ROUTINE 17
#define RAINBOW_ROUTINE 18
#define RAINBOWDIAGONAL_ROUTINE 19
#define FIRE_ROUTINE 20
#define IMAGE_MODE 21

#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0

#include "FastLED.h"
CRGB leds[NUM_LEDS];

static const byte maxDim = max(WIDTH, HEIGHT);
byte buttons = 4;   // 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата
int globalBrightness = BRIGHTNESS; 
byte globalSpeed = 200;
uint32_t globalColor = 0x00ff00;   // цвет при запуске зелёный
byte breathBrightness;
boolean loadingFlag = true;
byte frameNum;
int gameSpeed = DEMO_GAME_SPEED;
boolean gameDemo = true;
boolean idleState = true;  // флаг холостого режима работы
boolean BTcontrol = false;  // флаг контроля с блютус. Если false - управление с кнопок
int8_t thisMode = 0;
boolean controlFlag = false;
boolean gamemodeFlag = false;
boolean mazeMode = false;
int effects_speed = D_EFFECT_SPEED;
int8_t hrs = 10, mins = 25, secs;
boolean dotFlag;
byte modeCode;    // 0 бегущая, 1 часы, 2 игры, 3 нойс маднесс и далее, 21 гифка или картинка,
boolean fullTextFlag = false;
boolean clockSet = false;

#if (USE_FONTS == 1)
#include "fonts.h"
#endif

uint32_t autoplayTime = ((long)AUTOPLAY_PERIOD * 1000);
uint32_t autoplayTimer;

#include "timerMinim.h"
timerMinim effectTimer(D_EFFECT_SPEED);
timerMinim gameTimer(DEMO_GAME_SPEED);
timerMinim scrollTimer(D_TEXT_SPEED);
timerMinim idleTimer((long)IDLE_TIME * 1000);
timerMinim changeTimer(70);
timerMinim halfsecTimer(500);

#include <ESP8266WiFi.h>
char *ssid = "welcome's wi-fi";
char *pass = "27101988";
const bool NEED_STATIC_IP = true;
IPAddress gateway (192, 168, 1, 1);
IPAddress mask (255, 255, 255, 255);
IPAddress IP_Cristmas(192, 168, 1, 83);

#include <PubSubClient.h>
WiFiClient ESP_climate;
PubSubClient client(ESP_climate);
const char *mqtt_client_name = "ESP8266_cristmas";   // Имя клиента

#include <ESP8266HTTPUpdateServer.h>
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

// константы
const int RESTART_PERIOD = 10 * 60 * 1000;    // минимально время до ребута, если не удается подключиться к wi-fi
const int     CHECK_PERIOD = 5 * 60 * 1000;   // периодичность проверки на подключение к сервисам

// переменные времени
unsigned long Last_online_time;               // время когда модуль был онлайн
unsigned long Last_check_time;                // время крайней проверки подключения к сервисам

//===================================================================================================

void setup() {
  // подключение к Wi-Fi
  Connect_WiFi(IP_Cristmas, NEED_STATIC_IP);

  // подключение к MQTT
  Connect_mqtt(mqtt_client_name);
  MQTT_subscribe();

  // настройки ленты
  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.clear();
  FastLED.show();
  randomSeed(analogRead(0) + analogRead(1));    // пинаем генератор случайных чисел
}

//===================================================================================================

void loop() {
  // основные функции
  customRoutine();

  // сетевые функции
  httpServer.handleClient(); // для обновления по воздуху
  client.loop();             // для функций MQTT
  
  // проверка подключения к wifi
  if ((long)millis() - Last_check_time > CHECK_PERIOD) {
    Last_check_time = millis();

    if (WiFi.status() != WL_CONNECTED) { // wi-fi
      Connect_WiFi(IP_Cristmas, NEED_STATIC_IP);
      Restart(Last_online_time, RESTART_PERIOD);
    }
    else
      Last_online_time = millis();

    if (!client.connected()) { // mqtt
      Connect_mqtt(mqtt_client_name);
      MQTT_subscribe();
    }
  }
}

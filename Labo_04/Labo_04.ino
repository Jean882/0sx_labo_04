#include <AccelStepper.h>
#include <LCD_I2C.h>
#include <HCSR04.h>


// HC-SR04 Ultrasonic
#define TRIGGER_PIN 2
#define ECHO_PIN 3

HCSR04 hc(TRIGGER_PIN, ECHO_PIN);

// LCD
LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according

// Définir le type de moteur, les broches IN1-IN3-IN2-IN4
#define MOTOR_INTERFACE_TYPE 4

#define IN_1 8
#define IN_2 9
#define IN_3 10
#define IN_4 11

// Crée une instance
// Attention : L’ordre des broches (IN1, IN3, IN2, IN4) est spécifique au 28BYJ-48.
AccelStepper myStepper(MOTOR_INTERFACE_TYPE, IN_1, IN_3, IN_2, IN_4);

enum AppState {STOP, STATE_A};

AppState appState = STOP;

unsigned long currentTime = 0;
unsigned long previousTime = 0;
int interval = 100;
const int timePassed = 5000;
unsigned long start;

// Les pragma region permettent de créer des zones de code fermable
#pragma region Modèles

void xState(unsigned long cT) {
  static unsigned long lastTime = 0;
  const int rate = 500;

  static bool firstTime = true;

  if (firstTime) { // drapeau ouvert
    // Code d'initialisation de l'état
    // Reset tes trucs
    // Exemples : Angle de référence, initialiser le lastTime;

    firstTime = false; // ferme drapeau
    return;
  }

  if (cT - lastTime < rate) return;

  lastTime = cT;

  // Code de job à faire

  // Il s'agit de la transition qui permet de sortir de l'état
  // Qu'est-ce qui fait que l'on sort de l'état?
  bool transition = false;

  // Il est possible d'avoir plusieurs transitions

  if (transition) {
    // Code pour terminer la tâche

    firstTime = true;
  }
} // end of xState

// Modèle de tâche qui ne retourne aucune valeur.
void xTask(unsigned long ct) {
  static unsigned long lastTime = 0;
  unsigned long rate = 500;
  
  if (ct - lastTime < rate) {
    return;
  }
  
  lastTime = ct;
  
  // Faire le code de la tâche ici
  
} // end of xTask

// Modèle de tâche avec retour de valeur.
// Exemple d'utilisation :
//   - Mesure de distance, température, etc.
int xTaskWithReturn(unsigned long ct) {
  static unsigned long lastTime = 0;
  unsigned long rate = 500;
  static int lastResult = 0;
  int result = 0;
  
  if (ct - lastTime < rate) {
    return result;
  }
  
  lastTime = ct;
  
  // Faire le code de la tâche ici
  result = 1;
  
  lastResult = result;
  
  return result;  
} // end of xTaskWithReturn

#pragma endregion

void stateManager(unsigned long ct) {
  // Adapter selon votre situation!
  switch (appState) {
    case STOP:
      
      break;
  } // end of switch
} // end of stateManager

#pragma region setup-loop
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myStepper.setMaxSpeed(500);  // Vitesse max en pas/seconde
  myStepper.setAcceleration(100); // Accélération en pas/seconde²
	myStepper.setSpeed(200); // Vitesse constante en pas/seconde
	myStepper.moveTo(2038); // Position cible

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  lcd.begin();
  lcd.backlight();

  while (millis() <= 2000) {

    lcd.setCursor(0, 0);
    lcd.print("2206160      ");
    lcd.setCursor(0, 1);
    lcd.print("Labo 4a      ");
    

  } // end of while
} // end of setup

void loop() {
  currentTime = millis();

  float distance = hc.dist();
  Serial.print("Distance:");
  Serial.println(distance);
  
  if (myStepper.distanceToGo() == 0) 
		myStepper.moveTo(-myStepper.currentPosition());

	// Faire tourner le moteur d'un pas
  // Il faut appeler cette fonction dans le loop sinon le moteur ne tourne pas
	myStepper.run();

  stateManager(currentTime);

} // end of loop
#pragma endregion

// 12 ms incrémenter chaque 1 degree
// if () angle++
// if (transition) 
// suggestion lcd : variable globale message, fonction mettre a jour message

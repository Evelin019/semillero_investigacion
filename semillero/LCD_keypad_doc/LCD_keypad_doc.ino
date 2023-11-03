/**
 * Proyecto: DISEÑO DE UN SISTEMA DE MONITOREO PARA EL PROCESO DE GERMINACIÓN Y DESARROLLO DE PLÁNTULAS DE RÁBANO EN HUERTAS URBANAS
 * Descripción: Este código controla la temperatura y humedad de un sistema, activando o desactivando un sistema
 * Autor: Diego Romero
 * Repositorio: https://github.com/Evelin019/semillero_investigacion/tree/main
 * Versión: 1.0
 * Fecha de publicacion: 03/11/2023
 */

#include "LiquidCrystal.h"
#include <Keypad.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define RELAY1  8  // Salida para control de humedad
#define RELAY2  9  // Salida para control de temperatura

const int pinDatosDQ = 10;

const int AirValue = 854;   // Debes reemplazar este valor con el valor de referencia para suelo seco (Value_1)
const int WaterValue = 730;  // Debes reemplazar este valor con el valor de referencia para suelo húmedo (Value_2)
int soilMoistureValue = 0;
int soilmoisturepercent = 0;

// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

const byte ROWS = 4; // Cuatro filas
const byte COLS = 4; // Cuatro columnas

// Define los caracteres en los botones del teclado
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Conexiones de pines de filas y columnas del teclado
byte rowPins[ROWS] = {36, 34, 32, 30};
byte colPins[COLS] = {28, 26, 24, 22};

// Definiciones de caracteres personalizados para la pantalla LCD
byte arbolari[8]   = {B00000, B00001, B00011, B00101, B01111, B11011, B11111, B11111}; // Árbol
byte arbolard[8]   = {B00000, B10000, B11000, B11100, B11110, B10111, B11101, B11111};
byte arbolabi[8]   = {B01111, B00111, B00001, B00001, B00001, B00001, B00011, B11111};
byte arbolabd[8]   = {B11110, B11100, B10000, B10000, B10000, B10000, B11000, B11110};

byte arbollogoari[8]   = {B00000, B11000, B11001, B00101, B00011, B11001, B11001, B00101}; // Árbol Logo
byte arbollogoard[8]   = {B00110, B00110, B11000, B10000, B10011, B10011, B10100, B11000};
byte arbollogoabi[8]   = {B11011, B11001, B00101, B00011, B00001, B00111, B01111, B11111};
byte arbollogoabd[8]   = {B10000, B10110, B10110, B11000, B10000, B11100, B11110, B11111};

byte volver[8]   = {B00001, B00001, B00001, B00101, B01111, B11111, B01111, B00100};
byte termometro[8]   = {B00110, B00100, B00110, B00100, B01010, B11111, B11111, B01110}; // Imagen de termómetro para activación de calefacción
byte llave[8]   = {B11100, B01000, B11111, B00000, B01000, B11100, B11100, B01000}; // Imagen de llave para activación de riego
byte up[8]   = {B00100, B01110, B11111, B01110, B01110, B01110, B01110, B0000};
byte down[8]   = {B01110, B01110, B01110, B01110, B11111, B01110, B00100, B00000};

int temperatura = 0;
int humedad = 0;
String temp;
String hum;
String sum;

// Inicializa la biblioteca LCD y el teclado
LiquidCrystal lcd(2,3,4,5,6,7);
Keypad cKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

/**
 * Configuración inicial del sistema.
 */
void setup() {
  // Crea caracteres personalizados en la pantalla LCD
  // lcd.createChar(0, arbolari);
  // lcd.createChar(1, arbolard);
  // ...
  lcd.createChar(4, up);
  lcd.createChar(5, down);
  lcd.createChar(6, termometro);
  lcd.createChar(7, llave);
  
  lcd.createChar(0, arbollogoari);
  lcd.createChar(1, arbollogoard);
  // ...
  
  pinMode(RELAY1, OUTPUT);   
  pinMode(RELAY2, OUTPUT);   
  digitalWrite(RELAY1, 1);
  digitalWrite(RELAY2, 1);

  Serial.begin(9600);
  sensorDS18B20.begin();
  lcd.begin(16, 2);
  lcdMenuInit();
}

/**
 * Bucle principal del programa.
 */
void loop() {
  // Leer el valor de la tecla presionada
  char tecla = cKeypad.getKey();  

  if (tecla == 'D') {  
    lcdMenu();
  }
}

/**
 * Función que gestiona el menú principal de la pantalla LCD.
 */
void lcdMenu() {
  int posDisp = 0;
  int opcMenu = 0;
  char tecla;
  lcd.clear();

  do {
    tecla = cKeypad.getKey();  
    if (tecla == 'A') {
      lcd.clear();
      dispChange(&posDisp);
    }

    if (posDisp == 0) {
      lcd.setCursor(0, 0);
      lcd.print("1.Temperatura");
      lcd.setCursor(0, 1);
      lcd.print("2.Humedad");
      lcd.setCursor(14, 1);
      lcd.print("A");
      lcd.write(byte(5));

      switch (tecla) {
        case '1':
          lcdMenu1();
          break;
        case '2':
          lcdMenu2();
          break;
      }
    } else if (posDisp == 1) {
      lcd.setCursor(0, 0);
      lcd.print("3.Iniciar");
      lcd.setCursor(0, 1);
      lcd.print("4.Volver");
      lcd.setCursor(14, 1);
      lcd.print("A");
      lcd.write(byte(4));
      switch (tecla) {
        case '3':
          lcdIniciar();
          break;
        case '4':
          opcMenu = 1;
          break;
      }
    }
  } while (opcMenu != 1);
  lcdMenuInit();
}


//**
 * Función que maneja el menú de temperatura en la pantalla LCD.
 */
void lcdMenu1(){
    int posDisp = 0;
    int opcMenu = 0;
    char tecla;
    lcd.clear();

    do{
        tecla = cKeypad.getKey();
        if(tecla == 'A'){
            lcd.clear();
            dispChange(&posDisp);
        }

        if(posDisp == 0){
            lcd.setCursor(0, 0);
            lcd.print("1.Ajuste Temp.");
            lcd.setCursor(0, 1);
            lcd.print("2.Tem. Actual");
            lcd.setCursor(14, 1);
            lcd.print("A");
            lcd.write(byte(5));

            switch(tecla){
                case '1':
                    lcdMenuTemp();
                    break;
                case '2':
                    lcdleerTemp();
                    break;
            }
        }
        else if(posDisp == 1){
            lcd.setCursor(0, 0);
            lcd.print("3.Volver");
            lcd.setCursor(14, 1);
            lcd.print("A");
            lcd.write(byte(4));
            switch(tecla){
                case '3':
                    opcMenu = 1;
                    break;
                case '4':
                    break;
            }
        }
    }
    while(opcMenu != 1);
}

/**
 * Función que maneja el menú de humedad en la pantalla LCD.
 */
void lcdMenu2(){
    int posDisp = 0;
    int opcMenu = 0;
    char tecla;
    lcd.clear();

    do{
        tecla = cKeypad.getKey();
        if(tecla == 'A'){
            lcd.clear();
            dispChange(&posDisp);
        }

        if(posDisp == 0){
            lcd.setCursor(0, 0);
            lcd.print("1.Ajuste Hum.");
            lcd.setCursor(0, 1);
            lcd.print("2.Hum. Actual");
            lcd.setCursor(14, 1);
            lcd.print("A");
            lcd.write(byte(5));

            switch(tecla){
                case '1':
                    lcdMenuHum();
                    break;
                case '2':
                    lcdleerHum();
                    break;
            }
        }
        else if(posDisp == 1){
            lcd.setCursor(0, 0);
            lcd.print("3.Volver");
            lcd.setCursor(14, 1);
            lcd.print("A");
            lcd.write(byte(4));
            switch(tecla){
                case '3':
                    opcMenu = 1;
                    break;
                case '4':
                    break;
            }
        }
    }
    while(opcMenu != 1);
}

/**
 * Función para mostrar la temperatura actual en la pantalla LCD.
 */
void lcdleerTemp(){
    bool flag = true;
    char tecla;
    lcd.clear();
    float tempReal = 0;
    tempReal = leerTemp();

    do{
        tecla = cKeypad.getKey();
        lcd.setCursor(0, 0);
        lcd.print("T.Real");
        lcd.setCursor(7, 0);
        lcd.print("Obj.");
        lcd.setCursor(0, 1);
        lcd.print((int)tempReal);
        lcd.print("C");
        lcd.setCursor(7, 1);
        lcd.print(temperatura);
        lcd.print("C");
        lcd.setCursor(14, 0);
        lcd.print("D");
        lcd.print("<");
        if(tecla == 'D') {
            flag = false;
        }
    }
    while(flag);
    lcd.clear();
}

/**
 * Función para mostrar la humedad actual en la pantalla LCD.
 */
void lcdleerHum(){
    bool flag = true;
    char tecla;
    lcd.clear();
    float humedadReal = 0;
    humedadReal = leerHum();

    do{
        tecla = cKeypad.getKey();
        lcd.setCursor(0, 0);
        lcd.print("H.Real");
        lcd.setCursor(7, 0);
        lcd.print("Obj.");
        lcd.setCursor(0, 1);
        lcd.print((int)humedadReal);
        lcd.print("%");
        lcd.setCursor(7, 1);
        lcd.print(humedad);
        lcd.print("%");
        lcd.setCursor(14, 0);
        lcd.print("D");
        lcd.print("<");
        if(tecla == 'D') {
            flag = false;
        }
    }
    while(flag);
    lcd.clear();
}

/**
 * Función para mostrar el menú de ajuste de humedad en la pantalla LCD.
 */
void lcdMenuHum(){
    int humInt;
    int digito = 0;
    int opcMenu = 0;
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("D para confirmar ");
    lcd.setCursor(8, 0);
    lcd.print("%");
    lcd.setCursor(0, 0);
    lcd.print("Hum.= ");
    
    do{
        char tecla = cKeypad.getKey();
        lcd.cursor();
        if(tecla != 0){
            switch(digito){
                case 0:
                    lcd.print(tecla);
                    humedad = ((int)tecla - 48) * 10;
                    digito += 1;
                    break;
                case 1:
                    lcd.print(tecla);
                    humedad = humedad + ((int)tecla - 48);
                    digito += 1;
                    break;
                case 2:
                    if(tecla == 'D'){
                        opcMenu = 1;
                    }
                    break;
            }
            lcd.noCursor();
        }
    }
    while(opcMenu == 0);
    lcd.clear();
}



/**
 * Función para manejar el menú de ajuste de temperatura en la pantalla LCD.
 */
void lcdMenuTemp(){
    int digito = 0;
    int opcMenu = 0;
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("D para confirmar ");
    lcd.setCursor(0, 0);
    lcd.print("Temp.= ");
    
    do{
        char tecla = cKeypad.getKey();  
        lcd.cursor();
        if(tecla != 0){
            switch(digito){
                case 0:
                    lcd.print(tecla);
                    temperatura = ((int)tecla - 48) * 10;
                    digito += 1;
                    break;
                case 1:
                    lcd.print(tecla);
                    temperatura = temperatura + ((int)tecla - 48);
                    digito += 1;
                    break;
                case 2:
                    if(tecla == 'D'){
                        opcMenu = 1;
                    }
                    break;
            }
            lcd.noCursor();
        }
    }
    while(opcMenu == 0);
    lcd.clear();
}

/**
 * Función para manejar el menú de inicio en la pantalla LCD.
 */
void lcdMenuInit(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.print(" HomeCrop");
    lcd.setCursor(0, 1);
    lcd.write(byte(2));
    lcd.write(byte(3));
    lcd.print(" D para seguir");
}

/**
 * Función para cambiar la pantalla actual.
 */
void dispChange(int *pos){
    if(*pos == 0)
        *pos = 1;
    else if(*pos == 1)
        *pos = 0;
}

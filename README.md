# Diseño de un Sistema de Monitoreo para el Proceso de Germinación y Desarrollo de Plántulas de Rabano en Huertas Urbanas

Este proyecto se centra en la creación de un sistema de monitoreo de ambiente para cultivos, también conocido como agricultura inteligente. El objetivo principal es proporcionar a los agricultores una solución que les permita supervisar de manera eficiente las condiciones ambientales, como temperatura y humedad, en sus campos o invernaderos. Esto se logra mediante la integración de sensores y un microcontrolador (Arduino) para recopilar datos en tiempo real y un servidor Python para visualizar y registrar estos datos.

## Requisitos del Hardware
- Arduino (modelo específico)
- Sensores de temperatura y humedad
- Sensor de humedad del suelo
- Pantalla LCD
- Teclado matriz
- Relés para control de calefacción y riego
- Otros componentes específicos (mencionarlos)

## Librerías Externas
Este proyecto utiliza varias librerías de Arduino para el funcionamiento correcto:
- LiquidCrystal
- Keypad
- OneWire
- DallasTemperature

Asegúrate de instalar estas librerías en tu entorno de desarrollo.

## Diagrama de Conexión
Diagrama detallado de cómo se conectan los componentes del hardware (Arduino, sensores, pantalla, relés, etc.).

## Configuración
- Define las constantes relacionadas con los pines utilizados en el proyecto.
- Reemplaza los valores de `AirValue` y `WaterValue` con los valores reales para tu sensor de humedad del suelo.

## Funcionalidades
El proyecto incluye las siguientes funcionalidades:

### Menú de Inicio
- El sistema muestra una pantalla de inicio que muestra el nombre del proyecto y una invitación para presionar "D" para continuar.

### Menú de Temperatura
- El usuario puede ajustar la temperatura deseada.
- Puede verificar la temperatura actual.

### Menú de Humedad
- El usuario puede ajustar la humedad deseada.
- Puede verificar la humedad actual.

### Control de Riego y Calefacción
- El sistema controla el riego y la calefacción según los valores configurados.
- Se activa o desactiva el riego y la calefacción según los niveles de humedad y temperatura.

### Lectura de Sensores
- El proyecto realiza lecturas de temperatura y humedad del entorno.

### Calibración del Sensor de Humedad
- El sensor de humedad del suelo se calibra para proporcionar valores precisos.

# Servidor de Recepción de Datos Arduino

## Descripción del Servidor
Este servidor de Python se encarga de recibir los datos enviados por el Arduino, procesarlos y mostrar la temperatura y la humedad en la consola. También registra la marca de tiempo de cuándo se recibieron los datos.

## Requisitos
- Python 3.x instalado
- Conexión entre el servidor y el Arduino a través de la red local o una conexión directa.

## Configuración
1. Configura la dirección IP y el puerto en el que el Arduino enviará los datos.
   - `host`: Debes reemplazarlo con la dirección IP del Arduino.
   - `port`: El puerto en el que el Arduino enviará los datos (asegúrate de que coincida con la configuración del Arduino).

## Funcionamiento
El servidor Python realiza las siguientes operaciones:

1. Crea un socket TCP y lo enlaza a la dirección IP y el puerto especificados.
2. Escucha conexiones entrantes (una a la vez).
3. Cuando se establece una conexión desde el Arduino, recibe los datos.
4. Decodifica los datos para extraer la temperatura y la humedad.
5. Imprime los datos en la consola junto con la marca de tiempo.

## Uso
- Asegúrate de que el Arduino esté configurado para enviar los datos al servidor.
- Ejecuta este script Python en una computadora que esté en la misma red local que el Arduino.

## Ejecución
Para ejecutar el servidor, utiliza el siguiente comando en la terminal:

```bash
python server.py

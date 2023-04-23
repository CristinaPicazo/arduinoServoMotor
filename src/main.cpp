#include <Arduino.h>
#include <Servo.h>

#define PIN_PULSADOR A5

#define EJEMPLO2
#define EJEMPLO_LED

#if defined(EJEMPLO1)
#define TIEMPO_MINIMO 800
#define TIEMPO_MAXIMO 2200
#define PIN_SERVO DD3

void (*resetFunc)(void) = 0;
void esperandoPulsacion();

int posicion;
Servo motor1;
#endif

#if defined(EJEMPLO2)
#define PIN_LECTURA A0
#define PIN_PWM DD5
#define RESOLUCION_PWM 255
#define RESOLUCION_LECTURA 1023
#define PAUSA 2000

unsigned long ultimaLectura;
int valorLectura;
int valorEscritura;
bool incrementando = true;

#endif

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
#if defined(EJEMPLO1)
  motor1.attach(PIN_SERVO);
  // motor1.attach(PIN_SERVO,TIEMPO_MINIMO,TIEMPO_MAXIMO);

  posicion = motor1.read();
  Serial.print("Motor en la posición ");
  Serial.println(posicion);
  esperandoPulsacion();

  // Escribimos la posición
  motor1.write(posicion);
  Serial.print("Escribimos la posición ");
  Serial.println(posicion);
  esperandoPulsacion();

  // Nos movemos a la posición 0
  motor1.write(0);
  Serial.println("Nos movemos a la posición la posición 0");
  esperandoPulsacion();

  // Apagamos la señal.
  Serial.println("Apagamos señal ");
  motor1.detach();
  esperandoPulsacion();

  // Encendemos de nuevo la señal.
  motor1.attach(PIN_SERVO);
  // motor1.attach(PIN_SERVO,TIEMPO_MINIMO,TIEMPO_MAXIMO);
  posicion = motor1.read();
  Serial.print("Motor en la posición ");
  Serial.println(posicion);

  esperandoPulsacion();

  // Transición a la posición 90
  Serial.println("Iniciando transición a 90.");
  for (int i = 0; i < 90; i++)
  {
    motor1.write(i);
    delay(40);
  }
  Serial.println("fin transición.");
  esperandoPulsacion();

  Serial.println("Iniciando transición a 180.");
  for (int i = 90; i <= 180; i++)
  {
    motor1.write(i);
    delay(40);
  }
  Serial.println("fin transición.");
  esperandoPulsacion();
  resetFunc();
#endif

#if defined(EJEMPLO2)
  pinMode(PIN_PWM, OUTPUT);
  pinMode(PIN_LECTURA, INPUT);
  pinMode(DD3, OUTPUT);
  digitalWrite(DD3, 0);
#endif
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (millis() - ultimaLectura >= PAUSA)
  {
    ultimaLectura = millis();
    if (incrementando)
    {
#if defined(EJEMPLO_LED)
      valorLectura *= 2;
      valorLectura++;
#else
      valorLectura +=16;
      if(valorLectura < 127){valorLectura = 127;}
#endif
      if (valorLectura >= 255)
      {
        incrementando = false;
      }

    }
    else
    {
#if defined(EJEMPLO_LED)
      valorLectura /= 2;
#else
      valorLectura -=16;
      if(valorLectura < 127){valorLectura = 0;}
#endif
      if (valorLectura == 0)
      {
        incrementando = true;
      }
    }
    analogWrite(PIN_PWM, valorLectura);
    Serial.println(valorLectura);
  }
}

#if defined(EJEMPLO1)

void esperandoPulsacion()
{
  // Esperamos a que pulsen.
  while (digitalRead(PIN_PULSADOR))
  {
    delay(50);
  }
  // Esperamos a que suelten el pulsador.
  while (!digitalRead(PIN_PULSADOR))
  {
    delay(50);
  }
}

#endif

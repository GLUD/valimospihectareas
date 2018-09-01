char unsigned tablero[77] = {
  1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0 // esta fila son lo valores de desborde los cualesmseran evaluados para contar la vida a restar
};              //inicio la matriz del tablero como un vector de 75 posiciones
int  vida = 10; //vida del descontaminador
int columnas[] = {25, 26, 24, 28, 27, 29, 31};  //vector para definir los pines de las columnas
int filas[] = {42, 43, 44, 45, 46, 47, 48, 49, 50, 51}; //vector para definir los pines de las filas
int vidav[] = {32, 33, 34, 35, 36, 37, 38, 39, 40, 41};  //vector para definir los pines de las columnas
char unsigned tableroM[7][10];      //matriz que nace del vector
int time = 0;    //variable de tiempo
int d = 8;       //guarda la posicion del contaminador para añadir un 1 al desplazar
int e = 8;       //guarda la posicion del descontaminador para quitar la basura
int pos_con = 3; //posicion inicial del contaminador
int pos_des = 3; //Posicion inicial del descontaminador
String data = "";        // String para guardar los datos que vienen de la app
Servo servoMotor;        //servomotor contaminador
Servo servoMotor1;       //servomotor descontaminador
void setup() {
  Serial.begin(9600);   //Inicio el monitor serie a 9600 baudios
  Serial1.begin(9600);  //Inicio el monitor serie a 9600 baudios
  //se declaran los pines de los leds como salidas
  for (int i = 0; i <= 6; i++) {
    pinMode(columnas[i], OUTPUT);
  }
  for (int i = 0; i <= 9; i++) {
    pinMode(filas[i], OUTPUT);
  }
  for (int i = 0; i < 10; i++) {
    pinMode(vidav[i], OUTPUT);
    digitalWrite(vidav[i], HIGH);
  }
  data.reserve(2);//guarda los datos que llegan del bluetooth
  // Inicia la interrupcion por tiempo
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 34286;            // preload timer 65536-16MHz/1024/0.5Hz
  TCCR1B |= (1 << CS12);
  TCCR1B |= (1 << CS10);    // 1024 prescaler
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();   // enable all interrupts
  servoMotor.attach(9);
  servoMotor = pos_des * 45;
  servoMotor1.attach(10);
  servoMotor1 = pos_con * 45;
}

ISR(TIMER1_OVF_vect)       //interrupcion por tiempo cada 2 segundos
{
  noInterrupts();           //Deshabilita las interrupciones
  TCNT1 = 34286;            // preload timer
  VectoraMatriz();        //Convierte el vector a matriz
  desplazar();
  restarvida();
  interrupts();           //Habilita las interrupciones
}
void loop() {
  visu();
}

void visu() {  //rutina de visualizacion dinamica
  for (int k = 0; k < 10; k++) {
    for (int i = 0; i < 7; i++) {
      evaluarmatriz(i, k);
    }
  }
}

void evaluarmatriz(int i, int k) {
  if (tableroM[i][k] == 1) {
    off();
    digitalWrite(columnas[i], LOW);
    digitalWrite(filas[k], HIGH);
  }
}

void off() {
  for (int i = 0; i < 7 ; i++) {
    digitalWrite(columnas[i], HIGH);
  }
  for (int i = 0; i < 10 ; i++) {
    digitalWrite(filas[i], LOW);
  }
}

void VectoraMatriz() { // codigo para generar un vector atravez de la matriz
  for (int f = 0; f < 10; f++) {
    for (int k = 0; k < 7; k++) {
      tableroM[k][f] = tablero[(f * 7) + k];
    }
  }
}

void agregarbasura() {
  for (int j = 0; j <= 7; j++) { // Algoritmo para desplazar las columnas
    if (j == d) { //revisa si hay basura nueva en esa posicion y la agrega
      tablero[j] = 1;
      d = 8;
    }
  }
  VectoraMatriz();
}

void desplazar() {//desplaza todas las filas hacia abajo
  for (int j = 76; j >= 0; j--) { // Algoritmo para desplazar las columnas
    if (j < 7) {
      tablero[j] = 0;
    }
    else {
      tablero[j] = tablero[j - 7];
    }
  }
}

void restarvida() {
  digitalWrite(vidav[vida], LOW);
  for (int i = 70; i < 77; i++) {
    if (tablero[i] == 1 && pos_des != (i - 70)) { //verifica que el descontaminador recoja la basura
      vida--;
    }
  }
  //revisa si to
  if (vida == 0) {
    prendertodo();
  }
}

void  prendertodo() {
  for (int f = 0; f < 10; f++) {
    for (int k = 0; k < 7; k++) {
      digitalWrite(columnas[k], LOW);
      digitalWrite(filas[f], HIGH);
    }
  }
}
void quitarbasura() {
  for (int i = 63; i < 70; i++) {
    if (e == (i - 63)) { //verifica que el descontaminador recoja la basura
      tablero[i] = 0; //quita la basura
    }
  }
  VectoraMatriz();
}

//Interrupcion por Serial Bluetooth
void serialEvent1() {
  while (Serial1.available()) {
    // Consigue el nuevo byte
    char inChar = (char)Serial1.read();
    // lo añade a la cadena de data
    data += inChar;
    // Si ya tiene 2 datos, data esta lleno
    if (data.length() == 2) {
      if (data == "c0") {
        if (pos_con > 1) { //Limite Izquierdo
          pos_con += -1; //corre el contminador a la izquierda
        }
      }
      else if (data == "c1") {
        if (pos_con < 5) { //limite derecho
          pos_con += 1; //corre el contaminador a la derecha
          servoMotor1 = pos_con * 45;
        }
      }
      else if (data == "c2") {
        d = pos_con;
        agregarbasura(); //agrega una basura en la posicion d
        servoMotor1 = pos_con * 45;
      }
      else if (data == "d0") {
        if (pos_des > 1) { //Limite Izquierdo
          pos_des += -1; //corre el descontaminador a la izquierda
          servoMotor = pos_des * 45;
        }
      }
      else if (data == "d1") {
        if (pos_des < 5) { //limite derecho
          pos_des += 1; //corre el descontaminador a la derecha
          servoMotor = pos_des * 45;
        }
      }
      else if (data == "c2") {
        e = pos_des;
        quitarbasura();
      }
      Serial.print("Posicion contaminador: ");
      Serial.print(pos_con);
      Serial.println();

      Serial.print("Posicion descontaminador: ");
      Serial.print(pos_des);
      Serial.println();
      data = "";
    }
  }
}

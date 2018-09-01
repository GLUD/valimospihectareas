char unsigned tablero[77] = {
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0 /// esta fila son lo valores de desborde los cualesmseran evaluados para contar la vida a restar
}; //inicio la matriz del tablero como un vector de 75 posiciones
int vida = 10;
int time = 0; //variable de tiempo
int d = 8;    //guarda la posicion del contaminador para añadir un 1 al desplazar
int e = 8;    //guarda la posicion del descontaminador para quitar la basura
int pos_con = 3; //posicion inicial del contaminador
int pos_des =3; //Posicion inicial del descontaminador
String data = "";        // String para guardar los datos que vienen de la app
void setup() {
  Serial.begin(9600); //Inicio el monitor serie a 9600 baudios
  Serial1.begin(9600);//Inicia la transmision por tx1 y rx1 para el modulo bluetooth
  data.reserve(2);
  // Inicia la interrupcion por tiempo
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 34286;            // preload timer 65536-16MHz/1024/0.5Hz
  TCCR1B |= (1 << CS12);
  TCCR1B |= (1 << CS10);    // 1024 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts

}

ISR(TIMER1_OVF_vect)       //interrupcion por tiempo cada 2 segundos 
{
  noInterrupts();           //Deshabilita las interrupciones
  TCNT1 = 34286;            // preload timer
  printTablero();
  desplazar();
  restarvida();
  interrupts();           //Habilita las interrupciones
}
void loop() {
 /* desplazar();
  restarvida();
  printTablero();
  delay(3000);*/
}
void agregarbasura(){
  for (int j = 0; j <= 7; j++) { // Algoritmo para desplazar las columnas
      if (j == d) { //revisa si hay basura nueva en esa posicion y la agrega
        tablero[j] = 1;
        d = 8;
      }
    }
    printTablero();
}
void desplazar() {//desplaza todas las filas hacia abajo
  for (int j = 76; j >= 0; j--) { // Algoritmo para desplazar las columnas
    if (j <= 7) {
        tablero[j] = 0;
      }
    else {
      tablero[j] = tablero[j - 7];
    }
  }
}
void restarvida() {
  for (int i = 70; i < 77; i++) {
    if (tablero[i] == 1 && pos_des!=(i-70)) {//verifica que el descontaminador recoja la basura
      vida--;
    }
  }
}
void quitarbasura(){
  for (int i = 63; i < 70; i++) {
    if (e==(i-63)) {//verifica que el descontaminador recoja la basura
      tablero[i]=0; //quita la basura
    }
  }
  printTablero();
}
void printVida() {
  Serial.print("Vida: ");
  Serial.print(vida);
}

void printTablero() { //funcion para imprimir en el monitor serial el vector en forma de matriz
  Serial.println("Los valores de la matriz son:");
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 7; j++) {
      Serial.print(tablero[(i * 7) + j]);
    }
    Serial.print("\n");
  }
    printVida();
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
        if(pos_con>0){ //Limite Izquierdo
        pos_con += -1; //corre el contminador a la izquierda
        }
      }
      else if (data == "c1") {
        if(pos_con<6){ //limite derecho
        pos_con += 1; //corre el contaminador a la derecha
        }
      }
      else if (data == "c2") {
        d = pos_con;
        agregarbasura(); //agrega una basura en la posicion d
      }
      else if (data == "d0") {
        if(pos_des>0){ //Limite Izquierdo
        pos_des += -1; //corre el descontaminador a la izquierda
        }
      }
      else if (data == "d1") {
        if(pos_des<6){ //limite derecho
        pos_des += 1; //corre el descontaminador a la derecha
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

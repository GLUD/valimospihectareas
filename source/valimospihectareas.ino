int tablero[77]={
  0,0,0,0,0,0,0,
  1,0,0,0,0,0,0,
  0,1,0,0,0,0,0,
  1,0,1,0,0,0,0,
  0,1,0,1,0,0,0,
  0,0,1,0,1,0,0,
  0,0,0,1,0,0,0,
  0,0,0,0,1,0,0,
  0,0,0,0,0,1,0,
  0,0,0,0,0,0,1,
  0,0,0,0,0,0,0   /// esta fila son lo valores de desborde los cualesmseran evaluados para contar la vida a restar
  }; //inicio la matriz del tablero como un vector de 75 posiciones


void setup() {
  Serial.begin(9600); //Inicio el monitor serie a 9600 baudios
}

void loop() {
  printTablero();
  delay(3000);
  for(int j=74; j>=0; j--){
    if(j<=7){
      tablero[j]=0;
      }
    tablero[j]=tablero[j-7];
    
    }
   
  }

void printTablero(){
  Serial.println("Los valores de la matriz son:");
  for(int i=0; i<7 ; i++){
    Serial.print(tablero[i]);
    }
  Serial.print("\n");
  
  for(int i=7; i<14 ; i++){
    Serial.print(tablero[i]);
    }
  Serial.print("\n");
  
  for(int i=14; i<21 ; i++){
    Serial.print(tablero[i]);
    }
  Serial.print("\n");
  
  for(int i=21; i<28 ; i++){
    Serial.print(tablero[i]);
    }
  Serial.print("\n");
 
  for(int i=28; i<35 ; i++){
    Serial.print(tablero[i]);
    }
  Serial.print("\n");
  
  for(int i=35; i<42 ; i++){
    Serial.print(tablero[i]);
    }
  Serial.print("\n");
  
  for(int i=42; i<49 ; i++){
    Serial.print(tablero[i]);
    }
  Serial.print("\n");

 for(int i=49; i<56 ; i++){
    Serial.print(tablero[i]);
    }
  Serial.print("\n");

  for(int i=56; i<63 ; i++){
    Serial.print(tablero[i]);
    }
  Serial.print("\n");

  for(int i=63; i<70 ; i++){
    Serial.print(tablero[i]);
    }
  Serial.print("\n");
  }

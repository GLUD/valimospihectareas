char unsigned tablero[77]={
  0,0,1,0,0,0,0,
  0,0,0,1,0,1,0,
  1,0,0,0,0,0,0,
  0,1,0,0,0,0,1,
  0,0,0,0,0,1,1,
  0,0,1,0,0,0,0,
  1,0,0,0,0,0,1,
  0,1,0,1,0,0,0,
  0,0,1,0,1,1,1,
  0,0,0,0,0,0,0,
  0,0,0,0,0,0,0   /// esta fila son lo valores de desborde los cualesmseran evaluados para contar la vida a restar
  }; //inicio la matriz del tablero como un vector de 75 posiciones
 char unsigned  vida = 10; 
 int columnas[]={22, 23, 24, 25, 26, 27, 28};
 int filas[]={29, 30, 31, 32, 33, 34, 35, 36, 37, 38}; 
 char unsigned tableroM[7][10];


void setup() {
  Serial.begin(9600); //Inicio el monitor serie a 9600 baudios
  for(int i=0; i<=6; i++){
    pinMode(columnas[i], OUTPUT);
    }
  for(int i=0; i<=9; i++){
    pinMode(filas[i], OUTPUT);
    }
  
  

}

void loop() {
  
  }  



void visu(){   //rutina de visualizacion dinamica
  int i=0;
  int f=0;
  int k=0;
  for(f; f<70; f++){
    evaluarmatriz(i,k);
    i++;
    if(i==7){
      i=0;
      k++;
      }
    }
  }
    

void evaluarmatriz(int i, int k){
    if(tableroM[i][k]==1){
      offC();
      offF();
      digitalWrite(columnas[i], LOW);
      digitalWrite(filas[k], HIGH);
      delay(10);
      } 
    else{
      offC();
      offF();
      delay(10);
      }  
  }   



void offC(){
  for(int i =0; i<7 ; i++){
    digitalWrite(columnas[i], HIGH);
    }
  }



void offF(){
  for(int i =0; i<10 ; i++){
    digitalWrite(columnas[i], LOW);
    }
  }      

void VectoraMatriz(){ // codigo para generar un vector atravez de la matriz
  int i=0;
  int f=0;
  int k=0;
  for(i=0; i<70; i++){
    tableroM[k][f]=tablero[i];
    k++;
    if(k==7){
      k=0;
      f++;
      }
      
    }
  
}

void pM(){  // codigo para visualizar la matriz creada por la funcion VectoraMatriz()
  for (int j=0;j<10;j++){
    for(int k=0;k<7;k++){
      Serial.print(tableroM[k][j]);
    }
      Serial.print("\n");
   }
  }





  

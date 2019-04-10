#include <SparkFun_LED_8x7.h>
#include <Chaplex.h>

// Global variables
byte led_pins[] = {2, 3, 4, 5, 6, 7, 8, 9}; // Pins for LEDs
                        
int i = 0; // index global car utilisé par le main et la fonction loading array

byte prev_STR = 0; //Permet d'etre instancier pour la premiere loop du main

byte bitmap_1[] =       { 0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0 };
                          
byte bitmap_P3[][56] =   {{0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0}, 
                         
                         {0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,1,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0},

                         {0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,1,0,1,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0},
                          
                         {0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,1,0,0,0,1,0,
                          0,0,1,0,0,0,1,0,
                          0,0,1,0,0,0,1,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0},                          
                          
                         {0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,1,1,0,0,0,1,0,
                          0,0,1,0,0,0,1,0,
                          0,1,1,0,0,1,1,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0},
                                                    
                         {0,0,0,0,0,0,0,0,
                          1,1,1,0,0,0,1,0,
                          1,0,0,0,0,0,1,0,
                          1,1,1,0,1,1,1,0,
                          1,0,0,0,1,0,1,0,
                          1,1,1,0,1,1,1,0,
                          0,0,0,0,0,0,0,0}};
                          
byte bitmap_coeur[][56]={{0,0,0,1,1,0,0,0,
                          0,0,1,0,0,1,0,0,
                          0,1,0,0,0,0,1,0,
                          1,0,0,0,0,0,0,1,
                          1,0,0,0,0,0,0,1,
                          1,0,0,1,1,0,0,1,
                          0,1,1,0,0,1,1,0},
                         
                        
                        { 1,1,1,0,0,1,1,1,
                          1,0,1,0,0,1,0,1,
                          1,1,1,0,0,1,0,1,
                          0,0,1,0,0,1,0,0,
                          1,1,1,0,0,1,0,0,
                          1,0,1,0,0,1,0,0,
                          1,1,1,0,1,1,1,0 }};                          
                                              
void setup() {
  
  // Initialize and clear display
  Plex.init(led_pins);
  Plex.clear();
  Plex.display();

  //Initialize digital pin
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  pinMode(11, INPUT);
  pinMode(10, INPUT);
  
}



void loop() {

  byte STR = digitalRead(A5);  //Bit permentant de savoir quand modifier la matrice
  
  //Condition s'assurant d'avoir un changement d'état avant la modification de la matrice
  if (STR != prev_STR && STR == 1){
    //Loading des valeurs
    byte L[7] = {digitalRead(10),digitalRead(11),digitalRead(A0),digitalRead(A1),digitalRead(A2),digitalRead(A3),digitalRead(A4)};

    if(L[6] == 1 && L[5] == 0 && L[4] == 0 && L[3] == 0 && L[2] == 0 && L[1] == 0 && L[0] == 0){
      i=0;
    }
    
    else if(L[6] == 0 && L[5] == 1 && L[4] == 0 && L[3] == 0 && L[2] == 0 && L[1] == 0 && L[0] == 0){
      animationP3();
    }

    else if(L[6] == 0 && L[5] == 0 && L[4] == 1 && L[3] == 0 && L[2] == 0 && L[1] == 0 && L[0] == 0){
      animationcoeur();
    }
    
    else{
      //Modification de la matrice
      bitmap_1[0+i]   = L[0];
      bitmap_1[8+i]   = L[1];
      bitmap_1[16+i]  = L[2];
      bitmap_1[24+i]  = L[3];
      bitmap_1[32+i]  = L[4];
      bitmap_1[40+i]  = L[5];
      bitmap_1[48+i]  = L[6];
  
      i++;
    }
  
    if(i == 8){
      Plex.drawBitmap(bitmap_1);  //Load la matrice dans Plex
      Plex.display();             //Affiche la matrice
      delay(100);
      i = 0; 
    }
  }
  prev_STR = STR;
}

void animationP3(){
  for( i = 0; i < 6; i++){
    Plex.drawBitmap(bitmap_P3[i]);  //Load la matrice dans Plex
    Plex.display();             //Affiche la matrice
    delay(100);
  }
  delay(1000);
}

void animationcoeur(){
    for(i =0;i <2; i++){
      Plex.drawBitmap(bitmap_coeur[i]);  //Load la matrice dans Plex    
      Plex.display();
      delay(1000);
    }
}

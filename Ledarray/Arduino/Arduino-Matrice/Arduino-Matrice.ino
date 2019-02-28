#include <SparkFun_LED_8x7.h>
#include <Chaplex.h>

// Global variables
byte led_pins[] = {2, 3, 4, 5, 6, 7, 8, 9}; // Pins for LEDs
                        
int i = 0; // index global car utilisé par le main et la fonction loading array

byte prev_nxt = 0; //Permet d'etre instancier pour la premiere loop du main

byte bitmap_1[] =       { 0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0 };
                    
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
  //pinMode(13, OUTPUT);
  pinMode(11, INPUT);
  pinMode(10, INPUT);
  
}

void loop() {
  
  byte nxt = digitalRead(A5);  //Bit permentant de savoir quand modifier la matrice
  
  //Condition s'assurant d'avoir un changement d'état avant la modification de la matrice
  if (nxt != prev_nxt && nxt == 1){
    loadingarray();
  
    if(i == 8){
      Plex.drawBitmap(bitmap_1);  //Load la matrice dans Plex
      Plex.display();             //Affiche la matrice
      i = 0; 
    }
  }
  prev_nxt = nxt;

}


void loadingarray(){

  //Loading des valeurs
  byte L0 = digitalRead(10);
  byte L1 = digitalRead(11);
  byte L2 = digitalRead(A0);
  byte L3 = digitalRead(A1);
  byte L4 = digitalRead(A2);
  byte L5 = digitalRead(A3);
  byte L6 = digitalRead(A4);

  if(L6 == 1 && L5 == 0 && L4 == 0 && L3 == 0 && L2 == 0 && L1 == 0 && L0 == 0){
    i = 0;
  }
  else{
    //Modification de la matrice
    bitmap_1[0+i]   = L0;
    bitmap_1[8+i]   = L1;
    bitmap_1[16+i]  = L2;
    bitmap_1[24+i]  = L3;
    bitmap_1[32+i]  = L4;
    bitmap_1[40+i]  = L5;
    bitmap_1[48+i]  = L6;

    i++;
  }
}

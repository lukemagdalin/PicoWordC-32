#include <Arduino.h>

uint32_t read_register(void);

void setup() {
  // put your setup code here, to run once:
}



void loop() {
uint32_t x;
int y[10];
read_register();
for(int i = 0; i < 10; i++){
y[i] = x % 10;
x = x >> 1;
}

int z[10][8];
for(int j = 0; j < 10; j++){
switch(y[j]){
case 0:
z[j][0] = 0;
z[j][1] = 0;
z[j][2] = 1;
z[j][3] = 1;
z[j][4] = 1;
z[j][5] = 1;
z[j][6] = 1;
z[j][7] = 1;
  break;
case 1:
z[j][0] = 0;
z[j][1] = 0;
z[j][2] = 0;
z[j][3] = 0;
z[j][4] = 0;
z[j][5] = 1;
z[j][6] = 1;
z[j][7] = 0;
  break;
case 2:
z[j][0] = 0;
z[j][1] = 1;
z[j][2] = 0;
z[j][3] = 1;
z[j][4] = 1;
z[j][5] = 0;
z[j][6] = 1;
z[j][7] = 1;
  break;
case 3:
z[j][0] = 0;
z[j][1] = 1;
z[j][2] = 0;
z[j][3] = 0;
z[j][4] = 1;
z[j][5] = 1;
z[j][6] = 1;
z[j][7] = 1;
  break;
case 4:
z[j][0] = 0;
z[j][1] = 1;
z[j][2] = 1;
z[j][3] = 0;
z[j][4] = 0;
z[j][5] = 1;
z[j][6] = 1;
z[j][7] = 0;
  break;
case 5:
z[j][0] = 0;
z[j][1] = 1;
z[j][2] = 1;
z[j][3] = 0;
z[j][4] = 1;
z[j][5] = 1;
z[j][6] = 0;
z[j][7] = 1;
  break;
case 6:
z[j][0] = 0;
z[j][1] = 1;
z[j][2] = 1;
z[j][3] = 1;
z[j][4] = 1;
z[j][5] = 1;
z[j][6] = 0;
z[j][7] = 1;
  break;
case 7:
z[j][0] = 0;
z[j][1] = 0;
z[j][2] = 0;
z[j][3] = 0;
z[j][4] = 0;
z[j][5] = 1;
z[j][6] = 1;
z[j][7] = 1;
  break;
case 8:
z[j][0] = 0;
z[j][1] = 1;
z[j][2] = 1;
z[j][3] = 1;
z[j][4] = 1;
z[j][5] = 1;
z[j][6] = 1;
z[j][7] = 1;
  break;
case 9:
z[j][0] = 0;
z[j][1] = 1;
z[j][2] = 1;
z[j][3] = 0;
z[j][4] = 0;
z[j][5] = 1;
z[j][6] = 1;
z[j][7] = 1;
  break;
}
}

}


uint32_t read_register(void){
uint32_t x = 0;
for(int i = 0; i<31; i++){
bitRead(x, i);
}
return x; 
}
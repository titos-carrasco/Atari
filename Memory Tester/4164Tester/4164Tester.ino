/*

#define M_D     A5
#define M_WE    A4
#define M_RAS   A3
#define M_A0    A2
#define M_A2    A1
#define M_A1    A0
#define M_A7    8
#define M_A5    7
#define M_A4    6
#define M_A3    5
#define M_A6    4
#define M_Q     3
#define M_CAS   2
 */
 
#define M_D     A1
#define M_Q     8
#define M_CAS   9
#define M_RAS   A3
#define M_WE    A2

#define M_A0    A4
#define M_A1    2
#define M_A2    A5
#define M_A3    6
#define M_A4    5
#define M_A5    4
#define M_A6    7
#define M_A7    3


char msg[40];

void setup() {
  Serial.begin(9600);

  pinMode(M_D   , OUTPUT);
  pinMode(M_WE  , OUTPUT);
  pinMode(M_RAS , OUTPUT);
  pinMode(M_A0  , OUTPUT);
  pinMode(M_A1  , OUTPUT);
  pinMode(M_A2  , OUTPUT);
  pinMode(M_A7  , OUTPUT);
  pinMode(M_A5  , OUTPUT);
  pinMode(M_A4  , OUTPUT);
  pinMode(M_A3  , OUTPUT);
  pinMode(M_A6  , OUTPUT);
  pinMode(M_Q   , INPUT);
  pinMode(M_CAS , OUTPUT);

  digitalWrite(M_RAS, HIGH);
  digitalWrite(M_CAS, HIGH);
  digitalWrite(M_WE , HIGH);
  pause(100);

  while(!Serial);
}

void loop(){
  Serial.println("Iniciando verificacion...\n");
  Serial.flush();
  
  test_patterns();
  
  Serial.println("Fin de verificacion...\n");
  Serial.flush();
  
  while(1);
}


void test_patterns() {
  Serial.println("Pattern 00000000");
  Serial.flush();
  test_pattern(0b00000000);
  
  Serial.println("Pattern 11111111");
  Serial.flush();
  test_pattern(0b11111111);
  
  Serial.println("Pattern 10101010");
  Serial.flush();
  test_pattern(0b10101010);
  
  Serial.println("Pattern 01010101");
  Serial.flush();
  test_pattern(0b01010101);

  Serial.println("Pattern 11001100");
  Serial.flush();
  test_pattern(0b11001100);

  Serial.println("Pattern 11110000");
  Serial.flush();
  test_pattern(0b11110000);

}

void test_pattern( uint8_t pattern ){
  uint8_t patt;

  patt = pattern;
  for( unsigned int col=0; col<=0xFF; col++){
    for( unsigned int row=0; row<=0xFF; row++){
      uint8_t bit = patt & 0x01;
      patt = (patt >> 1) | (bit << 7);
      write(col, row, bit);
    }
  }

  patt = pattern;
  for( unsigned int col=0; col<=0xFF; col++){
    for( unsigned int row=0; row<=0xFF; row++){
      uint8_t bit = patt & 0x01;
      patt = (patt >> 1) | (bit << 7);

      uint8_t q = read(col, row);
      if(bit != q){
        sprintf(msg, "Pattern: error en %02X%02X: %d", col, row, bit);
        Serial.println(msg);
        Serial.flush();    
      }
    }
  }
}

void write(uint8_t col, uint8_t row, uint8_t bit){
    set_address(row);
    digitalWrite(M_RAS, LOW);
    digitalWrite(M_WE, LOW);
    digitalWrite(M_D, bit);
    set_address(col);
    digitalWrite(M_CAS, LOW);
    digitalWrite(M_WE, HIGH);
    digitalWrite(M_CAS, HIGH);
    digitalWrite(M_RAS, HIGH);
}

uint8_t read(uint8_t col, uint8_t row){
    set_address(row);
    digitalWrite(M_RAS, LOW);
    set_address(col);
    digitalWrite(M_CAS, LOW);
    uint8_t q = digitalRead(M_Q);
    digitalWrite(M_CAS, HIGH);
    digitalWrite(M_RAS, HIGH);

    return q;
}

void set_address(uint8_t a){
  digitalWrite(M_A0, a & 0b00000001);
  digitalWrite(M_A1, a & 0b00000010);
  digitalWrite(M_A2, a & 0b00000100);
  digitalWrite(M_A3, a & 0b00001000);
  digitalWrite(M_A4, a & 0b00010000);
  digitalWrite(M_A5, a & 0b00100000);
  digitalWrite(M_A6, a & 0b01000000);
  digitalWrite(M_A7, a & 0b10000000);
}

void pause(){
  pause(1);
}

void pause(unsigned int n){
  delayMicroseconds(n);
}

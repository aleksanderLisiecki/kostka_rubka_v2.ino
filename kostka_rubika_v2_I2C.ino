//=========================================
//= Aleksander Lisiecki 12.2016 - 04.2017 =
//=========================================


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <PortExpander_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


//expander niższy (0x20)
int DFRU = 4;
int ENF = 2;
int STF = 3;
int ENR = 7;
int STR = 6;
int ENU = 0;
int STU = 1;

//expander wyższy (0x21)
int DLBD = 5;
int ENL = 7;
int STL = 6;
int ENB = 2;
int STB = 3;
int END = 0;
int STD = 1;

PortExpander_I2C exFRU(0x20);
PortExpander_I2C exLBD(0x21);

int przerwa_krok = 1750;
int przerwa_miedzy_ruchami = 10;
int przerwa_po_wlaczeniu_silnikow = 20;

//1=r , 2=y , 3=o , 4=w , 5=b , 6=g
//___sciana czerwona___
byte r1, r2, r3, r4, r5, r6, r7, r8, r9;
//___sciana zolta___
byte y1, y2, y3, y4, y5, y6, y7, y8, y9;
//___sciana pomaranczowa___
byte o1, o2, o3, o4, o5, o6, o7, o8, o9;
//___sciana biala___
byte w1, w2, w3, w4, w5, w6, w7, w8, w9;
//___sciana niebieska___
byte b1, b2, b3, b4, b5, b6, b7, b8, b9;
//___sciana zielona___
byte g1, g2, g3, g4, g5, g6, g7, g8, g9;

//_______________________________do naroznikow__________________________________________
int d_1 = 100 * b3 + 10 * o1 + y3; int d_x = 100 * o1 + 10 * y3 + b3; int d_k = 100 * y3 + 10 * b3 + o1; //k-kwadrat(^2)       URB
int d_2 = 100 * b9 + 10 * y1 + r3; int d_r = 100 * y1 + 10 * r3 + b9; int d_y = 100 * r3 + 10 * b9 + y1; //r-rownasie(=)       URF
int d_3 = 100 * b7 + 10 * r1 + w3; int d_z = 100 * r1 + 10 * w3 + b7; int d_8 = 100 * w3 + 10 * b7 + r1; //                    ULF
int d_4 = 100 * g7 + 10 * o9 + w7; int d_a = 100 * o9 + 10 * w7 + g7; int d_p = 100 * w7 + 10 * g7 + o9; //p-plus(+)           DLB
int d_5 = 100 * g9 + 10 * y9 + o7; int d_m = 100 * y9 + 10 * o7 + g9; int d_b = 100 * o7 + 10 * g9 + y9; //m-minus(-)          DRB
int d_6 = 100 * g3 + 10 * r9 + y7; int d_c = 100 * r9 + 10 * y7 + g3; int d_n = 100 * y7 + 10 * g3 + r9; //n-razy, mnozenie(*) DRF
int d_7 = 100 * g1 + 10 * w9 + r7; int d_9 = 100 * w9 + 10 * r7 + g1; int d_d = 100 * r7 + 10 * g1 + w9; //                    DLF
int d_g = 100 * b1 + 10 * w1 + o3; //bufor    ULB

byte URB, URF, ULF, ULB, DRB, DRF, DLF, DLB;

int bufor_n;

//h-parity

//_______________________________do krawedzi_____________________________________________
byte d_A = 10 * b2 + o2;  byte d_B = 10 * o2 + b2;
byte d_C = 10 * b4 + w2;  byte d_D = 10 * w2 + b4;
byte d_E = 10 * b8 + r2;  byte d_F = 10 * r2 + b8;
byte d_G = 10 * b6 + y2;  byte d_H = 10 * y2 + b6;
byte d_I = 10 * g8 + o8;  byte d_J = 10 * o8 + g8;
byte d_K = 10 * g4 + w8;  byte d_L = 10 * w8 + g4;
byte d_M = 10 * g2 + r8;  byte d_N = 10 * r8 + g2;
byte d_O = 10 * g6 + y8;  byte d_P = 10 * y8 + g6;
byte d_R = 10 * r4 + w6;  byte d_S = 10 * w6 + r4;
byte d_T = 10 * w4 + o6;  byte d_U = 10 * o6 + w4;
byte d_W = 10 * o4 + y6;  byte d_X = 10 * y6 + o4;
byte d_Y = 10 * y4 + r6;  byte d_Z = 10 * r6 + y4;

byte UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BR, BL;

byte bufor_k;


char alg[100];
byte i = 0;
byte ii = 0;
char odczyt_alg[2];

unsigned long r;
unsigned long y;
unsigned long o;
unsigned long w;
unsigned long b;
unsigned long g;

byte faza = 0;
byte tryb = 0;

int przerwa = 10;

#define butt_next 5
int rzad1 = A1;
int rzad2 = A2;
int potpin = A0;
int v1, v2;

char kolory[15];
byte wcisniety_klawisz = 0; //licznik
byte kolor = 0;
//___do bt___
char inSerial[15];
byte rx = 0;
byte sciana_R = 0; byte sciana_Y = 0; byte sciana_O = 0; byte  sciana_W = 0; byte sciana_B = 0; byte sciana_G = 0; byte licznik_poprawnosci = 0; byte licznik_scian = 0; byte sciana_zrobiona = 0;
byte bt = 0;
byte ibt = 0;

byte text = 0;

int licz_next = 0;

int licznik_ruchow = 0;

int ilosc_ruchow = 0;

byte strzalka[8] = {0b00000, 0b00100, 0b00010, 0b11111, 0b00010, 0b00100, 0b00000, 0b00000};
byte cc[8] = {0b00010, 0b00100, 0b01110, 0b10000, 0b10000, 0b10001, 0b01110, 0b00000};
byte ee[8] = {0b00000, 0b00000, 0b01110, 0b10001, 0b11111, 0b10000, 0b01110, 0b00001};
byte ss[8] = {0b00010, 0b00100, 0b01110, 0b10000, 0b01110, 0b00001, 0b11110, 0b00000};
byte ll[8] = {0b01100, 0b00101, 0b00110, 0b00100, 0b01100, 0b00100, 0b01110, 0b00000};
byte oo[8] = {0b00010, 0b00100, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000};
byte aa[8] = {0b00000, 0b00000, 0b01110, 0b00001, 0b01111, 0b10001, 0b01111, 0b00001};
byte zz1[8] = {0b00010, 0b00100, 0b11111, 0b00010, 0b00100, 0b01000, 0b11111, 0b00000};
byte zz2[8] = {0b00100, 0b00000, 0b11111, 0b00010, 0b00100, 0b01000, 0b11111, 0b00000};
byte nn[8] = {0b00010, 0b00100, 0b10110, 0b11001, 0b10001, 0b10001, 0b10001, 0b00000};

void migajaca_strzalka() {
  if (licz_next < 250) {
    lcd.setCursor(19, 3);
    lcd.print(char(1));
    delay(2);
    licz_next++;
  }
  if (licz_next >= 250) {
    lcd.setCursor(19, 3);
    lcd.print(" ");
    delay(2);
    licz_next++;
  }
  if (licz_next >= 500) {
    licz_next = 0;
  }
}

byte x, z;
byte licz_x = 0;
void kolory_lcd() {
  switch (wcisniety_klawisz) {
    case 0: {
        x = 14;
        z = 1;
        lcd.setCursor(14, 1);
      } break;
    case 1: {
        x = 16;
        z = 1;
        lcd.setCursor(14, 1);
      } break;
    case 2: {
        x = 18;
        z = 1;
        lcd.setCursor(16, 1);
      } break;
    case 3: {
        x = 14;
        z = 2;
        lcd.setCursor(18, 1);
      } break;
    case 4: {
        x = 16;
        z = 2;
        lcd.setCursor(14, 2);
      } break;
    case 5: {
        x = 18;
        z = 2;
        lcd.setCursor(16, 2);
      } break;
    case 6: {
        x = 14;
        z = 3;
        lcd.setCursor(18, 2);
      } break;
    case 7: {
        x = 16;
        z = 3;
        lcd.setCursor(14, 3);
      } break;
    case 8: {
        x = 18;
        z = 3;
        lcd.setCursor(16, 3);
      } break;
    case 9: {
        x = 14;
        z = 3;
        lcd.setCursor(18, 3);
      } break;
  }
}
void migajacy_kursor() {
  if (licz_x < 5) {
    lcd.setCursor(x, z);
    lcd.print("_");
    licz_x++;
  }
  if (licz_x >= 5) {
    lcd.setCursor(x, z);
    lcd.print(" ");
    licz_x++;
  }
  if (licz_x >= 10) {
    licz_x = 0;
  }
}
void obroc_kostke_w_prawo () {
  lcd.setCursor(0, 1); lcd.print("Obroc kostke w prawo"); lcd.setCursor(3, 1); lcd.print(char(5)); lcd.setCursor(4, 1); lcd.print(char(0)); lcd.setCursor(11, 1); lcd.print(char(2));
  if (text == 1) {
    lcd.setCursor(1, 2);
    lcd.print("(na prawa sciane).");
    lcd.createChar(6, aa);
    lcd.setCursor(9, 2);
    lcd.print(char(6));
    lcd.setCursor(11, 2);
    lcd.print(char(3));
  }
  for (int kk = 0; kk < 20; kk++) {
    lcd.setCursor(kk, 3); lcd.print(".");
  }
  for (int kkk = 0; kkk < 19; kkk++) {
    delay(150); lcd.setCursor(kkk, 3); lcd.print(" ");
    if (digitalRead(butt_next) == HIGH) {
      break;
    }
  }
}

void setup() { //---------------------------------------------------------------------------------------------------------------------------
  Serial.begin(9600);
  //       /*
  lcd.begin(20, 4);

  pinMode(butt_next, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A0, INPUT);

  exFRU.init();

  exFRU.pinMode(0, OUTPUT);
  exFRU.pinMode(1, OUTPUT);
  exFRU.pinMode(2, OUTPUT);
  exFRU.pinMode(3, OUTPUT);
  exFRU.pinMode(4, OUTPUT);
  exFRU.pinMode(6, OUTPUT);
  exFRU.pinMode(7, OUTPUT);

  exLBD.init();

  exLBD.pinMode(0, OUTPUT);
  exLBD.pinMode(1, OUTPUT);
  exLBD.pinMode(2, OUTPUT);
  exLBD.pinMode(3, OUTPUT);
  exLBD.pinMode(5, OUTPUT);
  exLBD.pinMode(6, OUTPUT);
  exLBD.pinMode(7, OUTPUT);

  lcd.createChar(1, strzalka);
  lcd.createChar(0, cc);
  lcd.createChar(2, ee);
  lcd.createChar(3, ss);
  lcd.createChar(4, ll);
  lcd.createChar(5, oo);

  exFRU.digitalWrite(ENF, HIGH);
  exFRU.digitalWrite(ENR, HIGH);
  exFRU.digitalWrite(ENU, HIGH);
  exLBD.digitalWrite(ENL, HIGH);
  exLBD.digitalWrite(ENB, HIGH);
  exLBD.digitalWrite(END, HIGH);

  //_________________________________________Ekran wyboru trybu___________________________________
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Wybor trybu:"); lcd.setCursor(3, 0); lcd.print(char(5));
  lcd.setCursor(0, 1); lcd.print("Reczny:*"); lcd.setCursor(1, 1); lcd.print(char(2));
  lcd.setCursor(12, 1); lcd.print("czerwony");
  lcd.setCursor(0, 2); lcd.print("Automat.:");
  lcd.setCursor(18, 2); lcd.print("ty"); lcd.createChar(0, zz2); lcd.setCursor(15, 2); lcd.print(char(0)); lcd.setCursor(16, 2); lcd.print(char(5)); lcd.setCursor(17, 2); lcd.print(char(4));
  lcd.setCursor(0, 3); lcd.print("Bluetooth:");
  lcd.setCursor(11, 3); lcd.print("niebieski");
  byte c = 0;
  while (c == 0) {
    delay(10);
    v1 = analogRead(rzad1);
    v2 = analogRead(rzad2);
    if (v1 > 520 && v1 < 855) { //___zolty___/___automat___
      tryb = 2;
      c = 1;
    }
    if (v1 > 225 && v1 < 520) { //___niebieski___
      tryb = 3; //___bluetooth___
      c = 1;
      bt = 1;
      lcd.clear();
      lcd.setCursor(0, 1); lcd.print("Oczekuje na dane"); lcd.setCursor(7, 1); lcd.print(char(2));
    }

    if (v1 > 855) { //___czerwony___//___reczny__
      lcd.clear();
      byte nieskonczona_petla_trybu_recznego = 0;
      int t = 0;
      int przerwa_miedzy_obrotami = 200;
      lcd.setCursor(0, 0); lcd.print("Niestety tryb");
      lcd.setCursor(0, 1); lcd.print("reczny jest jeszcze");
      lcd.setCursor(0, 2); lcd.print("nie obslugiwany.");

      while (nieskonczona_petla_trybu_recznego = 0) {

      }
      /*        v1 = analogRead(rzad1);
              v2 = analogRead(rzad2);

              if (t == 0) {
                exFRU.digitalWrite(ENF, LOW);
                exFRU.digitalWrite(ENR, LOW);
                exFRU.digitalWrite(ENU, LOW);
                //    exLBD.digitalWrite(ENL, LOW);
                //    exLBD.digitalWrite(ENB, LOW);
                //    exLBD.digitalWrite(END, LOW);
                delay(przerwa_po_wlaczeniu_silnikow);
              }
              if (t == 100) {
                exFRU.digitalWrite(ENF, HIGH);
                exFRU.digitalWrite(ENR, HIGH);
                exFRU.digitalWrite(ENU, HIGH);
                exLBD.digitalWrite(ENL, HIGH);
                exLBD.digitalWrite(ENB, HIGH);
                exLBD.digitalWrite(END, HIGH);
              }

              if (digitalRead(butt_next) == HIGH) {
                t = 0;
                exFRU.digitalWrite(DFRU, HIGH);
                exLBD.digitalWrite(DLBD, HIGH);
              }
              else {
                exFRU.digitalWrite(DFRU, LOW);
                exLBD.digitalWrite(DLBD, LOW);
              }

              if (v1 > 225 && v1 < 520) {
                Serial.println("R");
                t = 0;
                krokFRU(STF); //F__(czerwony)
                delay(przerwa_miedzy_obrotami);
              }
              if (v1 > 520 && v1 < 855) {
                Serial.println("Y");
                t = 0;
                krokFRU(STR); //R__(zolty)
                delay(przerwa_miedzy_obrotami);
              }
              if (v2 > 520 && v2 < 855) {
                Serial.println("B");
                t = 0;
                krokFRU(STU); //U__(niebieski)
                delay(przerwa_miedzy_obrotami);
              }
              if (v2 > 225 && v2 < 520) {
                Serial.println("W");
                t = 0;
                krokLBD(STL); //L__(bialy)
                delay(przerwa_miedzy_obrotami);
              }
              if (v1 > 855) {
                Serial.println("O");
                t = 0;
                krokLBD(STB); //B__(pomaranczowy)
                delay(przerwa_miedzy_obrotami);
              }
              if (v2 > 855) {
                Serial.println("G");
                t = 0;
                krokLBD(STD); //D__(zielony);
                delay(przerwa_miedzy_obrotami);
              }
              t++;
              delay(10);
            }*/
    }



  }
  switch (tryb) {
    case 1: { //___tryb reczny___

      } break; //___konec trybu recznego___

    case 3: { //___tryb bluetooth

        pinMode(rx, INPUT);
        while (bt == 1) {
          ibt = 0;
          delay(500);
          if (Serial.available() > 0) {
            while (Serial.available() > 0) {
              inSerial[ibt] = Serial.read();
              ibt++;
            }
            inSerial[ibt] = '\0';
            Serial.println(inSerial);
            odebrane_bt(inSerial);
            if (sciana_zrobiona == 1) {
              sciana_zrobiona = 0;
              goto pomin_wprowadzanie;
            }
            goto bajpas_od_Bluetooth;
pomin_wprowadzanie:
            Serial.println(licznik_poprawnosci);
            if (licznik_poprawnosci == 9) {
              if (kolor == 1) {
                sciana_R = 1;
              }
              else if (kolor == 2) {
                sciana_Y = 1;
              }
              else if (kolor == 3) {
                sciana_O = 1;
              }
              else if (kolor == 4) {
                sciana_W = 1;
              }
              else if (kolor == 5) {
                sciana_B = 1;
              }
              else if (kolor == 6) {
                sciana_G = 1;
              }

            } licznik_poprawnosci = 0;
            if (licznik_scian == 6) {
              if ((sciana_R + sciana_Y + sciana_O + sciana_W + sciana_B + sciana_G) == 6) {
                Serial.print(r1); Serial.print(r2); Serial.print(r3); Serial.print(r4); Serial.print(r5); Serial.print(r6); Serial.print(r7); Serial.print(r8); Serial.println(r9);
                Serial.print(y1); Serial.print(y2); Serial.print(y3); Serial.print(y4); Serial.print(y5); Serial.print(y6); Serial.print(y7); Serial.print(y8); Serial.println(y9);
                Serial.print(o1); Serial.print(o2); Serial.print(o3); Serial.print(o4); Serial.print(o5); Serial.print(o6); Serial.print(o7); Serial.print(o8); Serial.println(o9);
                Serial.print(w1); Serial.print(w2); Serial.print(w3); Serial.print(w4); Serial.print(w5); Serial.print(w6); Serial.print(w7); Serial.print(w8); Serial.println(w9);
                Serial.print(b1); Serial.print(b2); Serial.print(b3); Serial.print(b4); Serial.print(b5); Serial.print(b6); Serial.print(b7); Serial.print(b8); Serial.println(b9);
                Serial.print(g1); Serial.print(g2); Serial.print(g3); Serial.print(g4); Serial.print(g5); Serial.print(g6); Serial.print(g7); Serial.print(g8); Serial.println(g9);
                bt = 2;
              }
              else {
                lcd.clear();
                lcd.setCursor(1, 0); lcd.print("Wystapil blad przy");
                lcd.setCursor(0, 1); lcd.print("wczytywaniu kolorow");
                lcd.setCursor(0, 2); lcd.print("przeslij je ponownie");
              }
            }
          }
          for (byte m = 0; m < 11; m++) {
            kolory[m] = 0;
          }
        }
      } break;

    case 2: { //___tryb automatyczny___
        lcd.clear();



        lcd.setCursor(0, 0); lcd.print("Chwyc kostke tak,aby"); lcd.createChar(0, cc); lcd.setCursor(4, 0); lcd.print(char(0)); lcd.createChar(2, ee); lcd.setCursor(11, 0); lcd.print(char(2));
        lcd.setCursor(2, 1); lcd.print("srodek przedniej"); lcd.createChar(3, ss); lcd.setCursor(2, 1); lcd.print(char(3));
        lcd.setCursor(0, 2); lcd.print("sciany byl czerwony,"); lcd.setCursor(0, 2); lcd.print(char(3)); lcd.setCursor(9, 2); lcd.print(char(4));
        lcd.setCursor(0, 3); lcd.print("a gornej niebieski."); lcd.setCursor(3, 3); lcd.print(char(5));
        while (digitalRead(butt_next) == LOW) {
          migajaca_strzalka();
        }
 /*       lcd.clear();
        while (digitalRead(butt_next) == HIGH) {
          lcd.setCursor(0, 0); lcd.print("Wprowad"); lcd.setCursor(13, 0); lcd.print("_____"); lcd.createChar(6, zz1); lcd.setCursor(7, 0); lcd.print(char(6));
          lcd.setCursor(0, 1); lcd.print("kolory"); lcd.setCursor(12, 1); lcd.print("|1 2 3|");
          lcd.setCursor(0, 2); lcd.print("wedlog"); lcd.setCursor(12, 2); lcd.print("|4 5 6|"); lcd.setCursor(3, 2); lcd.print(char(4)); lcd.setCursor(4, 2); lcd.print(char(5));
          lcd.setCursor(0, 3); lcd.print("kolejnosci:"); lcd.setCursor(12, 3); lcd.print("|7 8 9|"); lcd.setCursor(7, 3); lcd.print(char(3));
          delay(200);
        }
        while (digitalRead(butt_next) == LOW) {
          migajaca_strzalka();
        }
 */
        lcd.clear();
        while (digitalRead(butt_next) == HIGH) {
          lcd.setCursor(0, 0); lcd.print("Oznaczenia kolorow: "); lcd.setCursor(16, 0); lcd.print(char(5));
          lcd.setCursor(1, 1); lcd.print("R-czerw."); lcd.setCursor(10, 1); lcd.print("G-ziel.");
          lcd.setCursor(1, 2); lcd.print("B-nieb."); lcd.setCursor(10, 2); lcd.print("W-bialy"); lcd.setCursor(15, 2); lcd.print(char(4));
          lcd.setCursor(1, 3); lcd.print("O-pomar."); lcd.setCursor(10, 3); lcd.print("Y-zolty"); lcd.createChar(6, zz2); lcd.setCursor(12, 3); lcd.print(char(6)); lcd.setCursor(13, 3); lcd.print(char(5)); lcd.setCursor(14, 3); lcd.print(char(4));
          delay(200);
        }
        while (digitalRead(butt_next) == LOW) {
          migajaca_strzalka();
        }
        lcd.clear();
        while (digitalRead(butt_next) == HIGH) {
          lcd.setCursor(0, 0); lcd.print("W razie pomylki"); lcd.setCursor(12, 0); lcd.print(char(4));
          lcd.setCursor(0, 1); lcd.print("nacisnac  ."); lcd.createChar(6, aa); lcd.setCursor(6, 1); lcd.print(char(6)); lcd.setCursor(7, 1); lcd.print(char(0)); lcd.setCursor(9, 1); lcd.print(char(1));
//          lcd.setCursor(0, 3); lcd.print("Zacznij wprowadzac"); lcd.setCursor(17, 3); lcd.print(char(0));
          delay(200);
        }
        while (digitalRead(butt_next) == LOW) {
          migajaca_strzalka();
        }
        lcd.clear();


        while ((r1 + r2 + r3 + r4 + r5 + r6 + r7 + r8 + r9 + y1 + y2 + y3 + y4 + y5 + y6 + y7 + y8 + y9 + o1 + o2 + o3 + o4 + o5 + o6 + o7 + o8 + o9 + w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 + w9 + b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + g1 + g2 + g3 + g4 + g5 + g6 + g7 + g8 + g9) != 189) {
          delay(100);
          if (text == 0) {
            lcd_skrot();
            lcd.setCursor(0, 2); lcd.print("czerwonym"); lcd.setCursor(13, 2); lcd.print("|  R"); lcd.setCursor(19, 2); lcd.print("|");
            kolory_lcd();
            text++;
          }
          if (kolor == 1) {
            if (text == 1) {
              lcd.clear();
              obroc_kostke_w_prawo ();
              while (digitalRead(butt_next) == LOW) {
                migajaca_strzalka();
              }
              lcd.clear();
              lcd_skrot();
              lcd.setCursor(0, 2); lcd.print("zoltym"); lcd.setCursor(13, 2); lcd.print("|  Y"); lcd.setCursor(19, 2); lcd.print("|"); lcd.createChar(4, zz2); lcd.setCursor(0, 2); lcd.print(char(6)); lcd.setCursor(1, 2); lcd.print(char(5)); lcd.setCursor(2, 2); lcd.print(char(4));
              kolory_lcd();
              text++;
            }
          }
          if (kolor == 2) {
            if (text == 2) {
              lcd.clear();
              obroc_kostke_w_prawo ();
              while (digitalRead(butt_next) == LOW) {
                migajaca_strzalka();
              }
              lcd.clear();
              lcd_skrot();
              lcd.setCursor(0, 2); lcd.print("pomaranczowym"); lcd.setCursor(13, 2); lcd.print("|  O"); lcd.setCursor(19, 2); lcd.print("|"); lcd.createChar(6, nn); lcd.setCursor(6, 2); lcd.print(char(6));
              kolory_lcd();
              text++;
            }
          }
          if (kolor == 3) {
            if (text == 3) {
              lcd.clear();
              obroc_kostke_w_prawo ();
              while (digitalRead(butt_next) == LOW) {
                migajaca_strzalka();
              }
              lcd.clear();
              lcd_skrot();
              lcd.setCursor(0, 2); lcd.print("bialym"); lcd.setCursor(13, 2); lcd.print("|  W"); lcd.setCursor(19, 2); lcd.print("|"); lcd.setCursor(3, 2); lcd.print(char(4));
              kolory_lcd();
              text++;
            }
          }
          if (kolor == 4) {
            if (text == 4) {
              lcd.clear();
              lcd.setCursor(0, 0); lcd.print("Obroc kostke w prawo"); lcd.setCursor(3, 1); lcd.print(char(5)); lcd.setCursor(4, 1); lcd.print(char(0)); lcd.setCursor(11, 1); lcd.print(char(2));
              lcd.setCursor(0, 1); lcd.print("nastepnie na gorna"); lcd.setCursor(4, 1); lcd.print(char(2)); lcd.setCursor(14, 1); lcd.print(char(5));
              lcd.setCursor(0, 2); lcd.print("sciane (niebieska)"); lcd.setCursor(0, 2); lcd.print(char(3)); lcd.setCursor(5, 2); lcd.print(char(2));
              for (int kk = 0; kk < 20; kk++) {
                lcd.setCursor(kk, 3); lcd.print(".");
              }
              for (int kkk = 0; kkk < 19; kkk++) {
                delay(150); lcd.setCursor(kkk, 3); lcd.print(" ");
              }
              while (digitalRead(butt_next) == LOW) {
                migajaca_strzalka();
              }
              lcd.clear();
              lcd_skrot();
              lcd.setCursor(0, 2); lcd.print("niebieskim"); lcd.setCursor(13, 2); lcd.print("|  B"); lcd.setCursor(19, 2); lcd.print("|");
              kolory_lcd();
              text++;
            }
          }
          if (kolor == 5) {
            if (text == 5) {
              lcd.clear();
              lcd.setCursor(0, 1); lcd.print("Obroc kostke 2 razy"); lcd.setCursor(3, 1); lcd.print(char(5)); lcd.setCursor(4, 1); lcd.print(char(0)); lcd.setCursor(11, 1); lcd.print(char(2));
              lcd.setCursor(2, 2); lcd.print("w dol (zielona)"); lcd.setCursor(5, 2); lcd.print(char(5));
              for (int kk = 0; kk < 20; kk++) {
                lcd.setCursor(kk, 3); lcd.print(".");
              }
              for (int kkk = 0; kkk < 19; kkk++) {
                delay(150); lcd.setCursor(kkk, 3); lcd.print(" ");
              }
              while (digitalRead(butt_next) == LOW) {
                migajaca_strzalka();
              }
              lcd.clear();
              lcd_skrot();
              lcd.setCursor(0, 2); lcd.print("zielonym"); lcd.setCursor(13, 2); lcd.print("|  G"); lcd.setCursor(19, 2); lcd.print("|");
              kolory_lcd();
              text++;
            }
          }
          if (kolor == 6 && ((r1 + r2 + r3 + r4 + r5 + r6 + r7 + r8 + r9 + y1 + y2 + y3 + y4 + y5 + y6 + y7 + y8 + y9 + o1 + o2 + o3 + o4 + o5 + o6 + o7 + o8 + o9 + w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 + w9 + b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + g1 + g2 + g3 + g4 + g5 + g6 + g7 + g8 + g9) != 189)) {
            lcd.clear();
            lcd.setCursor(0, 0); lcd.print("Nie postarales sie,");
            lcd.setCursor(3, 1); lcd.print("kolory zostaly");
            lcd.setCursor(0, 2); lcd.print("wprowadzone blednie.");
            lcd.setCursor(1, 3); lcd.print("RESET i od nowa =)");
            delay(20000);
            //                lcd.clear();
            //                lcd.setCursor(2,0); lcd.print("Za czym czekasz?");
            //                lcd.setCursor(1,1); lcd.print("Zresetuj sterownik");
            //                lcd.setCursor(1,2); lcd.print("i wprowadz kolory");
            //                lcd.setCursor(0,3); lcd.print("od nowa baranie! xD");
            //                delay(25000);

          }

          migajacy_kursor();

          while (digitalRead(butt_next) == HIGH) {
            if (wcisniety_klawisz > 0) {
              lcd.setCursor(x, z); lcd.print(" ");
              //           Serial.println(wcisniety_klawisz);
              wcisniety_klawisz--;
              kolory_lcd();
              lcd.setCursor(x, z); lcd.print("_");
            }
            while (digitalRead(butt_next) == HIGH) {
              delay(50);
            }
          }

          v1 = analogRead(rzad1);
          v2 = analogRead(rzad2);
          if (v1 > 855) {
            //          Serial.println("R");
            kolory[wcisniety_klawisz] = 'R'; wcisniety_klawisz++;
            kolory_lcd();
            lcd.print("R");
            while (analogRead(rzad1) > 225) {}
          }
          if (v1 > 520 && v1 < 855) {
            //          Serial.println("Y");
            kolory[wcisniety_klawisz] = 'Y'; wcisniety_klawisz++;
            kolory_lcd();
            lcd.print("Y");
            while (analogRead(rzad1) > 225) {}
          }
          if (v2 > 855) {
            //         Serial.println("O");
            kolory[wcisniety_klawisz] = 'O'; wcisniety_klawisz++;
            kolory_lcd();
            lcd.print("O");
            while (analogRead(rzad2) > 225) {}
          }
          if (v2 > 520 && v2 < 855) {
            //              Serial.println("W");
            kolory[wcisniety_klawisz] = 'W'; wcisniety_klawisz++;
            kolory_lcd();
            lcd.print("W");
            while (analogRead(rzad2) > 225) {}
          }
          if (v1 > 225 && v1 < 520) {
            //             Serial.println("B");
            kolory[wcisniety_klawisz] = 'B'; wcisniety_klawisz++;
            kolory_lcd();
            lcd.print("B");
            while (analogRead(rzad1) > 225) {}
          }
          if (v2 > 225 && v2 < 520) {
            //              Serial.println("G");
            kolory[wcisniety_klawisz] = 'G'; wcisniety_klawisz++;
            kolory_lcd();
            lcd.print("G");
            while (analogRead(rzad2) > 225) {}
          }


          if (wcisniety_klawisz == 9) {
            //              Serial.println(kolory);
            wcisniety_klawisz = 0;
bajpas_od_Bluetooth:
            byte iii = 0;
            byte x;
            kolor++;
            while (iii < 9) {
              char kol[1];
              kol[0] = kolory[iii];
              if (kol[0] == 'R' ) {
                licznik_poprawnosci++;
                x = 1;
              }
              else if (kol[0] == 'Y' ) {
                licznik_poprawnosci++;
                x = 2;
              }
              else if (kol[0] == 'O' ) {
                licznik_poprawnosci++;
                x = 3;
              }
              else if (kol[0] == 'W' ) {
                licznik_poprawnosci++;
                x = 4;
              }
              else if (kol[0] == 'B' ) {
                licznik_poprawnosci++;
                x = 5;
              }
              else if (kol[0] == 'G' ) {
                licznik_poprawnosci++;
                x = 6;
              }
              switch (kolor) {
                case 1: {
                    switch (iii) {
                      case 0 : {
                          r1 = x;
                      } break; case 1: {
                          r2 = x;
                      } break; case 2: {
                          r3 = x;
                      } break; case 3 : {
                          r4 = x;
                      } break; case 4 : {
                          r5 = x;
                      } break; case 5 : {
                          r6 = x;
                      } break; case 6 : {
                          r7 = x;
                      } break; case 7 : {
                          r8 = x;
                      } break; case 8 : {
                          r9 = x;
                        } break;
                    }
                  } break;
                case 2: {
                    switch (iii) {
                      case 0 : {
                          y1 = x;
                      } break; case 1: {
                          y2 = x;
                      } break; case 2: {
                          y3 = x;
                      } break; case 3 : {
                          y4 = x;
                      } break; case 4 : {
                          y5 = x;
                      } break; case 5 : {
                          y6 = x;
                      } break; case 6 : {
                          y7 = x;
                      } break; case 7 : {
                          y8 = x;
                      } break; case 8 : {
                          y9 = x;
                        } break;
                    }
                  } break;
                case 3: {
                    switch (iii) {
                      case 0 : {
                          o1 = x;
                      } break; case 1: {
                          o2 = x;
                      } break; case 2: {
                          o3 = x;
                      } break; case 3 : {
                          o4 = x;
                      } break; case 4 : {
                          o5 = x;
                      } break; case 5 : {
                          o6 = x;
                      } break; case 6 : {
                          o7 = x;
                      } break; case 7 : {
                          o8 = x;
                      } break; case 8 : {
                          o9 = x;
                        } break;
                    }
                  } break;
                case 4: {
                    switch (iii) {
                      case 0 : {
                          w1 = x;
                      } break; case 1: {
                          w2 = x;
                      } break; case 2: {
                          w3 = x;
                      } break; case 3 : {
                          w4 = x;
                      } break; case 4 : {
                          w5 = x;
                      } break; case 5 : {
                          w6 = x;
                      } break; case 6 : {
                          w7 = x;
                      } break; case 7 : {
                          w8 = x;
                      } break; case 8 : {
                          w9 = x;
                        } break;
                    }
                  } break;
                case 5: {
                    switch (iii) {
                      case 0 : {
                          b1 = x;
                      } break; case 1: {
                          b2 = x;
                      } break; case 2: {
                          b3 = x;
                      } break; case 3 : {
                          b4 = x;
                      } break; case 4 : {
                          b5 = x;
                      } break; case 5 : {
                          b6 = x;
                      } break; case 6 : {
                          b7 = x;
                      } break; case 7 : {
                          b8 = x;
                      } break; case 8 : {
                          b9 = x;
                        } break;
                    }
                  } break;
                case 6: {
                    switch (iii) {
                      case 0 : {
                          g1 = x;
                      } break; case 1: {
                          g2 = x;
                      } break; case 2: {
                          g3 = x;
                      } break; case 3 : {
                          g4 = x;
                      } break; case 4 : {
                          g5 = x;
                      } break; case 5 : {
                          g6 = x;
                      } break; case 6 : {
                          g7 = x;
                      } break; case 7 : {
                          g8 = x;
                      } break; case 8 : {
                          g9 = x;
                        } break;
                    }
                  } break;
              } iii++;
            }


            if (bt == 1) {
              goto pomin_wprowadzanie;
            }
            //              Serial.print(r1); Serial.print(r2); Serial.print(r3); Serial.print(r4); Serial.print(r5); Serial.print(r6); Serial.print(r7); Serial.print(r8); Serial.println(r9);
            //              Serial.print(y1); Serial.print(y2); Serial.print(y3); Serial.print(y4); Serial.print(y5); Serial.print(y6); Serial.print(y7); Serial.print(y8); Serial.println(y9);
            //              Serial.print(o1); Serial.print(o2); Serial.print(o3); Serial.print(o4); Serial.print(o5); Serial.print(o6); Serial.print(o7); Serial.print(o8); Serial.println(o9);
            //              Serial.print(w1); Serial.print(w2); Serial.print(w3); Serial.print(w4); Serial.print(w5); Serial.print(w6); Serial.print(w7); Serial.print(w8); Serial.println(w9);
            //              Serial.print(b1); Serial.print(b2); Serial.print(b3); Serial.print(b4); Serial.print(b5); Serial.print(b6); Serial.print(b7); Serial.print(b8); Serial.println(b9);
            //              Serial.print(g1); Serial.print(g2); Serial.print(g3); Serial.print(g4); Serial.print(g5); Serial.print(g6); Serial.print(g7); Serial.print(g8); Serial.println(g9);
          }

          //        }


        } break; //_____konec trybu automatycznego_____
      }
  }

  //*/

  /*
    exFRU.init();

    exFRU.pinMode(0, OUTPUT);
    exFRU.pinMode(1, OUTPUT);
    exFRU.pinMode(2, OUTPUT);
    exFRU.pinMode(3, OUTPUT);
    exFRU.pinMode(4, OUTPUT);
    exFRU.pinMode(6, OUTPUT);
    exFRU.pinMode(7, OUTPUT);

    exLBD.init();

    exLBD.pinMode(0, OUTPUT);
    exLBD.pinMode(1, OUTPUT);
    exLBD.pinMode(2, OUTPUT);
    exLBD.pinMode(3, OUTPUT);
    exLBD.pinMode(5, OUTPUT);
    exLBD.pinMode(6, OUTPUT);
    exLBD.pinMode(7, OUTPUT);
  */




  // r= 465514263;
  // y= 443126653;
  // o= 664233222;
  // w= 623441131;
  // b= 144155511;
  // g= 632562535;
  //
  //
  //r1=r/100000000;r2=(r-((r/100000000)*100000000))/10000000;r3=(r-((r/10000000)*10000000))/1000000;r4=(r-((r/1000000)*1000000))/100000;r5=(r-((r/100000)*100000))/10000;r6=(r-((r/10000)*10000))/1000;r7=(r-((r/1000)*1000))/100;r8=(r-((r/100)*100))/10;r9=(r-((r/10)*10));
  //y1=y/100000000;y2=(y-((y/100000000)*100000000))/10000000;y3=(y-((y/10000000)*10000000))/1000000;y4=(y-((y/1000000)*1000000))/100000;y5=(y-((y/100000)*100000))/10000;y6=(y-((y/10000)*10000))/1000;y7=(y-((y/1000)*1000))/100;y8=(y-((y/100)*100))/10;y9=(y-((y/10)*10));
  //o1=o/100000000;o2=(o-((o/100000000)*100000000))/10000000;o3=(o-((o/10000000)*10000000))/1000000;o4=(o-((o/1000000)*1000000))/100000;o5=(o-((o/100000)*100000))/10000;o6=(o-((o/10000)*10000))/1000;o7=(o-((o/1000)*1000))/100;o8=(o-((o/100)*100))/10;o9=(o-((o/10)*10));
  //w1=w/100000000;w2=(w-((w/100000000)*100000000))/10000000;w3=(w-((w/10000000)*10000000))/1000000;w4=(w-((w/1000000)*1000000))/100000;w5=(w-((w/100000)*100000))/10000;w6=(w-((w/10000)*10000))/1000;w7=(w-((w/1000)*1000))/100;w8=(w-((w/100)*100))/10;w9=(w-((w/10)*10));
  //b1=b/100000000;b2=(b-((b/100000000)*100000000))/10000000;b3=(b-((b/10000000)*10000000))/1000000;b4=(b-((b/1000000)*1000000))/100000;b5=(b-((b/100000)*100000))/10000;b6=(b-((b/10000)*10000))/1000;b7=(b-((b/1000)*1000))/100;b8=(b-((b/100)*100))/10;b9=(b-((b/10)*10));
  //g1=g/100000000;g2=(g-((g/100000000)*100000000))/10000000;g3=(g-((g/10000000)*10000000))/1000000;g4=(g-((g/1000000)*1000000))/100000;g5=(g-((g/100000)*100000))/10000;g6=(g-((g/10000)*10000))/1000;g7=(g-((g/1000)*1000))/100;g8=(g-((g/100)*100))/10;g9=(g-((g/10)*10));

  if ((r1 + r2 + r3 + r4 + r5 + r6 + r7 + r8 + r9 + y1 + y2 + y3 + y4 + y5 + y6 + y7 + y8 + y9 + o1 + o2 + o3 + o4 + o5 + o6 + o7 + o8 + o9 + w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8 + w9 + b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + g1 + g2 + g3 + g4 + g5 + g6 + g7 + g8 + g9) != 189) {
    //   Serial.println("suma =/=");
    ; delay(6000);
  }


  d_1 = 100 * b3 + 10 * o1 + y3; d_x = 100 * o1 + 10 * y3 + b3; d_k = 100 * y3 + 10 * b3 + o1;
  d_2 = 100 * b9 + 10 * y1 + r3; d_r = 100 * y1 + 10 * r3 + b9; d_y = 100 * r3 + 10 * b9 + y1;
  d_3 = 100 * b7 + 10 * r1 + w3; d_z = 100 * r1 + 10 * w3 + b7; d_8 = 100 * w3 + 10 * b7 + r1;
  d_4 = 100 * g7 + 10 * o9 + w7; d_a = 100 * o9 + 10 * w7 + g7; d_p = 100 * w7 + 10 * g7 + o9;
  d_5 = 100 * g9 + 10 * y9 + o7; d_m = 100 * y9 + 10 * o7 + g9; d_b = 100 * o7 + 10 * g9 + y9;
  d_6 = 100 * g3 + 10 * r9 + y7; d_c = 100 * r9 + 10 * y7 + g3; d_n = 100 * y7 + 10 * g3 + r9;
  d_7 = 100 * g1 + 10 * w9 + r7; d_9 = 100 * w9 + 10 * r7 + g1; d_d = 100 * r7 + 10 * g1 + w9;
  d_g = 100 * b1 + 10 * w1 + o3;

  bufor_n = d_g;

  d_A = 10 * b2 + o2;   d_B = 10 * o2 + b2;
  d_C = 10 * b4 + w2;   d_D = 10 * w2 + b4;
  d_E = 10 * b8 + r2;   d_F = 10 * r2 + b8;
  d_G = 10 * b6 + y2;   d_H = 10 * y2 + b6;
  d_I = 10 * g8 + o8;   d_J = 10 * o8 + g8;
  d_K = 10 * g4 + w8;   d_L = 10 * w8 + g4;
  d_M = 10 * g2 + r8;   d_N = 10 * r8 + g2;
  d_O = 10 * g6 + y8;   d_P = 10 * y8 + g6;
  d_R = 10 * r4 + w6;   d_S = 10 * w6 + r4;
  d_T = 10 * w4 + o6;   d_U = 10 * o6 + w4;
  d_W = 10 * o4 + y6;   d_X = 10 * y6 + o4;
  d_Y = 10 * y4 + r6;   d_Z = 10 * r6 + y4;

  bufor_k = d_G;



}

void loop() { //----------------------------------------------------------------------------------------------------------------------------

  switch (faza) {
    case 0: { //_____faza 1: sprawdzanie ulozonych czesci___________________________________________________________________________________
        //________________________sprawdzanie ulozonych narożnikow___________________________________
        if (d_1 == 532) {
          URB++;
        }
        if (d_2 == 521) {
          URF++;
        }
        if (d_3 == 514) {
          ULF++;
        }
        if (d_4 == 634) {
          DLB++;
        }
        if (d_5 == 623) {
          DRB++;
        }
        if (d_6 == 612) {
          DRF++;
        }
        if (d_7 == 641) {
          DLF++;
        }
        //___________________________sprawdzanie ulozonych krawedzi__________________________________
        if (d_A == 53) {
          UB++;
        }
        if (d_C == 54) {
          UL++;
        }
        if (d_E == 51) {
          UF++;
        }
        if (d_I == 63) {
          DB++;
        }
        if (d_K == 64) {
          DL++;
        }
        if (d_M == 61) {
          DF++;
        }
        if (d_O == 62) {
          DR++;
        }
        if (d_R == 14) {
          FL++;
        }
        if (d_T == 43) {
          BL++;
        }
        if (d_W == 32) {
          BR++;
        }
        if (d_Y == 21) {
          FR++;
        }
        faza++;
      } break;

    case 1: { //_____faza 2: ukladanie setupu krawedzi_______________________________________________________________________________________

        //___________________________trafienie na bufor_k_________________________________________
        if (bufor_k == 52 || bufor_k == 25) {
          //      Serial.println("trafiono na bufor_k");
powtorzenie_krawedzi:
          if (UB == 0) {
            bufor_k = d_A;
            alg[i] = 'A';
            i++;
            UB++;
            licznik_ruchow = licznik_ruchow + 13;
          }
          else if (UL == 0) {
            bufor_k = d_C;
            alg[i] = 'C';
            i++;
            UL++;
            licznik_ruchow = licznik_ruchow + 15;
          }
          else if (UF == 0) {
            bufor_k = d_E;
            alg[i] = 'E';
            i++;
            UF++;
            licznik_ruchow = licznik_ruchow + 13;
          }
          else if (DB == 0) {
            bufor_k = d_J;
            alg[i] = 'J';
            i++;
            DB++;
            licznik_ruchow = licznik_ruchow + 13;
          }
          else if (DL == 0) {
            bufor_k = d_K;
            alg[i] = 'K';
            i++;
            DL++;
            licznik_ruchow = licznik_ruchow + 19;
          }
          else if (DF == 0) {
            bufor_k = d_N;
            alg[i] = 'N';
            i++;
            DF++;
            licznik_ruchow = licznik_ruchow + 13;
          }
          else if (DR == 0) {
            bufor_k = d_P;
            alg[i] = 'P';
            i++;
            DR++;
            licznik_ruchow = licznik_ruchow + 15;
          }
          else if (FL == 0) {
            bufor_k = d_R;
            alg[i] = 'R';
            i++;
            FL++;
            licznik_ruchow = licznik_ruchow + 17;
          }
          else if (BL == 0) {
            bufor_k = d_U;
            alg[i] = 'U';
            i++;
            BL++;
            licznik_ruchow = licznik_ruchow + 17;
          }
          else if (BR == 0) {
            bufor_k = d_X;
            alg[i] = 'X';
            i++;
            BR++;
            licznik_ruchow = licznik_ruchow + 19;
          }
          else if (FR == 0) {
            bufor_k = d_Y;
            alg[i] = 'Y';
            i++;
            FR++;
            licznik_ruchow = licznik_ruchow + 19;
          }
          else {
            //             Serial.print("KONIEC->KRAWEDZIE"); Serial.println(i); delay(2000);
            faza++;
            if (i % 2 != 0) {
              alg[i] = 'h';
              i++;
              licznik_ruchow = licznik_ruchow + 19;
            }
            break;
          }
        }

        //__________________2 na ktoryms liczniku == "powtorzenie" sie krawedzi________________
        if (UB == 2) {
          UB = 1;
          goto powtorzenie_krawedzi;
        }
        if (UL == 2) {
          UL = 1;
          goto powtorzenie_krawedzi;
        }
        if (UF == 2) {
          UF = 1;
          goto powtorzenie_krawedzi;
        }
        if (DB == 2) {
          DB = 1;
          goto powtorzenie_krawedzi;
        }
        if (DL == 2) {
          DL = 1;
          goto powtorzenie_krawedzi;
        }
        if (DF == 2) {
          DF = 1;
          goto powtorzenie_krawedzi;
        }
        if (DR == 2) {
          DR = 1;
          goto powtorzenie_krawedzi;
        }
        if (FL == 2) {
          FL = 1;
          goto powtorzenie_krawedzi;
        }
        if (BL == 2) {
          BL = 1;
          goto powtorzenie_krawedzi;
        }
        if (BR == 2) {
          BR = 1;
          goto powtorzenie_krawedzi;
        }
        if (FR == 2) {
          FR = 1;
          goto powtorzenie_krawedzi;
        }

        //__________________________________ukladanie setupu____________________________________
        switch (bufor_k) {
          case 53: {
              alg[i] = 'A';
              i++;
              bufor_k = d_A;
              UB++;
              licznik_ruchow = licznik_ruchow + 13;
            } break;
          case 35: {
              alg[i] = 'B';
              i++;
              bufor_k = d_B;
              UB++;
              licznik_ruchow = licznik_ruchow + 15;
            } break;
          case 54: {
              alg[i] = 'C';
              i++;
              bufor_k = d_C;
              UL++;
              licznik_ruchow = licznik_ruchow + 15;
            } break;
          case 45: {
              alg[i] = 'D';
              i++;
              bufor_k = d_D;
              UL++;
              licznik_ruchow = licznik_ruchow + 21;
            } break;
          case 51: {
              alg[i] = 'E';
              i++;
              bufor_k = d_E;
              UF++;
              licznik_ruchow = licznik_ruchow + 13;
            } break;
          case 15: {
              alg[i] = 'F';
              i++;
              bufor_k = d_F;
              UF++;
              licznik_ruchow = licznik_ruchow + 15;
            } break;
          case 63: {
              alg[i] = 'I';
              i++;
              bufor_k = d_I;
              DB++;
              licznik_ruchow = licznik_ruchow + 15;
            } break;
          case 36: {
              alg[i] = 'J';
              i++;
              bufor_k = d_J;
              DB++;
              licznik_ruchow = licznik_ruchow + 13;
            } break;
          case 64: {
              alg[i] = 'K';
              i++;
              bufor_k = d_K;
              DL++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 46: {
              alg[i] = 'L';
              i++;
              bufor_k = d_L;
              DL++;
              licznik_ruchow = licznik_ruchow + 15;
            } break;
          case 61: {
              alg[i] = 'M';
              i++;
              bufor_k = d_M;
              DF++;
              licznik_ruchow = licznik_ruchow + 15;
            } break;
          case 16: {
              alg[i] = 'N';
              i++;
              bufor_k = d_N;
              DF++;
              licznik_ruchow = licznik_ruchow + 13;
            } break;
          case 62: {
              alg[i] = 'O';
              i++;
              bufor_k = d_O;
              DR++;
              licznik_ruchow = licznik_ruchow + 17;
            } break;
          case 26: {
              alg[i] = 'P';
              i++;
              bufor_k = d_P;
              DR++;
              licznik_ruchow = licznik_ruchow + 15;
            } break;
          case 14: {
              alg[i] = 'R';
              i++;
              bufor_k = d_R;
              FL++;
              licznik_ruchow = licznik_ruchow + 17;
            } break;
          case 41: {
              alg[i] = 'S';
              i++;
              bufor_k = d_S;
              FL++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 43: {
              alg[i] = 'T';
              i++;
              bufor_k = d_T;
              BL++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 34: {
              alg[i] = 'U';
              i++;
              bufor_k = d_U;
              BL++;
              licznik_ruchow = licznik_ruchow + 17;
            } break;
          case 32: {
              alg[i] = 'W';
              i++;
              bufor_k = d_W;
              BR++;
              licznik_ruchow = licznik_ruchow + 21;
            } break;
          case 23: {
              alg[i] = 'X';
              i++;
              bufor_k = d_X;
              BR++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 21: {
              alg[i] = 'Y';
              i++;
              bufor_k = d_Y;
              FR++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 12: {
              alg[i] = 'Z';
              i++;
              bufor_k = d_Z;
              FR++;
              licznik_ruchow = licznik_ruchow + 21;
            } break;
        }
        //          Serial.print(UB); Serial.print(UL); Serial.print(UF); Serial.print(DB); Serial.print(DL); Serial.print(DF); Serial.print(DR); Serial.print(FL); Serial.print(BL); Serial.print(BR); Serial.println(FR);
        //          Serial.println(alg);
        //          Serial.println(bufor_k);
        //    delay(przerwa);
      }  break;

    case 2: { //_____faza 3: ukladanie setupu rogow______________________________________________________________________________________________________________________

        //___________________________trafienie na bufor_n_________________________________________
        if (bufor_n == 543 || bufor_n == 435 || bufor_n == 354) {
powtorzenie_naroznika:
          if (URB == 0) {
            bufor_n = d_k;
            alg[i] = 'k';
            i++;
            URB++;
            licznik_ruchow = licznik_ruchow + 15;
          }
          else if (URF == 0) {
            bufor_n = d_r;
            alg[i] = 'r';
            i++;
            URF++;
            licznik_ruchow = licznik_ruchow + 17;
          }
          else if (ULF == 0) {
            bufor_n = d_3;
            alg[i] = '3';
            i++;
            ULF++;
            licznik_ruchow = licznik_ruchow + 13;
          }
          else if (DLB == 0) {
            bufor_n = d_p;
            alg[i] = 'p';
            i++;
            DLB++;
            licznik_ruchow = licznik_ruchow + 19;
          }
          else if (DRB == 0) {
            bufor_n = d_m;
            alg[i] = 'm';
            i++;
            DRB++;
            licznik_ruchow = licznik_ruchow + 15;
          }
          else if (DRF == 0) {
            bufor_n = d_n;
            alg[i] = 'n';
            i++;
            DRF++;
            licznik_ruchow = licznik_ruchow + 15;
          }
          else if (DLF == 0) {
            bufor_n = d_9;
            alg[i] = '9';
            i++;
            DLF++;
            licznik_ruchow = licznik_ruchow + 15;
          }
          else {
            //             Serial.println("KONIEC->NAROZNIKI"); Serial.println(i); delay(2000);
            faza++;
            ilosc_ruchow = licznik_ruchow;
            lcd.clear();
            lcd.setCursor(0, 0); lcd.print("Liczba ruchow: "); lcd.print(ilosc_ruchow);
            lcd.setCursor(5, 1); lcd.print("Nacisnij "); lcd.setCursor(14, 1); lcd.print(char(1));
            lcd.setCursor(3, 2); lcd.print("aby rozpoczac"); lcd.createChar(0, cc); lcd.setCursor(15, 2); lcd.print(char(0)); lcd.createChar(3, aa); lcd.setCursor(14, 2); lcd.print(char(3));
            while (digitalRead(butt_next) == LOW) {}
            lcd.clear();
            lcd.print(alg);
            break;
          }
        }

        //__________________2 na ktoryms liczniku == "powtorzenie" sie naroznika________________
        if (URB == 2) {
          URB = 1;
          goto powtorzenie_naroznika;
        }
        if (URF == 2) {
          URF = 1;
          goto powtorzenie_naroznika;
        }
        if (ULF == 2) {
          ULF = 1;
          goto powtorzenie_naroznika;
        }
        if (DLB == 2) {
          DLB = 1;
          goto powtorzenie_naroznika;
        }
        if (DRB == 2) {
          DRB = 1;
          goto powtorzenie_naroznika;
        }
        if (DRF == 2) {
          DRF = 1;
          goto powtorzenie_naroznika;
        }
        if (DLF == 2) {
          DLF = 1;
          goto powtorzenie_naroznika;
        }


        //__________________________________ukladanie setupu____________________________________
        switch (bufor_n) {
          case 532: {
              alg[i] = '1';
              i++;
              bufor_n = d_1;
              URB++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 521: {
              alg[i] = '2';
              i++;
              bufor_n = d_2;
              URF++;
              licznik_ruchow = licznik_ruchow + 17;
            } break;
          case 514: {
              alg[i] = '3';
              i++;
              bufor_n = d_3;
              ULF++;
              licznik_ruchow = licznik_ruchow + 13;
            } break;
          case 634: {
              alg[i] = '4';
              i++;
              bufor_n = d_4;
              DLB++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 623: {
              alg[i] = '5';
              i++;
              bufor_n = d_5;
              DRB++;
              licznik_ruchow = licznik_ruchow + 21;
            } break;
          case 612: {
              alg[i] = '6';
              i++;
              bufor_n = d_6;
              DRF++;
              licznik_ruchow = licznik_ruchow + 17;
            } break;
          case 641: {
              alg[i] = '7';
              i++;
              bufor_n = d_7;
              DLF++;
              licznik_ruchow = licznik_ruchow + 17;
            } break;
          case 325: {
              alg[i] = 'x';
              i++;
              bufor_n = d_x;
              URB++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 152: {
              alg[i] = 'y';
              i++;
              bufor_n = d_y;
              URF++;
              licznik_ruchow = licznik_ruchow + 21;
            } break;
          case 145: {
              alg[i] = 'z';
              i++;
              bufor_n = d_z;
              ULF++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 346: {
              alg[i] = 'a';
              i++;
              bufor_n = d_a;
              DLB++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 362: {
              alg[i] = 'b';
              i++;
              bufor_n = d_b;
              DRB++;
              licznik_ruchow = licznik_ruchow + 17;
            } break;
          case 126: {
              alg[i] = 'c';
              i++;
              bufor_n = d_c;
              DRF++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 164: {
              alg[i] = 'd';
              i++;
              bufor_n = d_d;
              DLF++;
              licznik_ruchow = licznik_ruchow + 17;
            } break;
          case 253: {
              alg[i] = 'k';
              i++;
              bufor_n = d_k;
              URB++;
              licznik_ruchow = licznik_ruchow + 15;
            } break;
          case 215: {
              alg[i] = 'r';
              i++;
              bufor_n = d_r;
              URF++;
              licznik_ruchow = licznik_ruchow + 17;
            } break;
          case 261: {
              alg[i] = 'n';
              i++;
              bufor_n = d_n;
              DRF++;
              licznik_ruchow = licznik_ruchow + 15;
            } break;
          case 236: {
              alg[i] = 'm';
              i++;
              bufor_n = d_m;
              DRB++;
              licznik_ruchow = licznik_ruchow + 15;
            } break;
          case 463: {
              alg[i] = 'p';
              i++;
              bufor_n = d_p;
              DLB++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 451: {
              alg[i] = '8';
              i++;
              bufor_n = d_8;
              ULF++;
              licznik_ruchow = licznik_ruchow + 19;
            } break;
          case 416: {
              alg[i] = '9';
              i++;
              bufor_n = d_9;
              DLF++;
              licznik_ruchow = licznik_ruchow + 15;
            } break;
        }
        //          Serial.print(URB); Serial.print(URF); Serial.print(ULF); Serial.print(DLB); Serial.print(DRB); Serial.print(DRF); Serial.println(DLF);
        //       lcd.clear();
        //        lcd.print(alg);
        //          Serial.println(alg);
        //          Serial.println(bufor_n);




      } break;


    case 3: { //_____faza 4: układanie kostki ____________________________________________________________________________________________________________________
        //        int pot = analogRead(A0);
        //        przerwa_miedzy_ruchami = map(pot, 0, 1023, 1600, 4000);




        //       while (digitalRead(butt_next) == LOW) {}
        if (ii < i) {
          odczyt_alg[0] = alg[ii];  ii++;
          Serial.println(odczyt_alg);
          if (odczyt_alg[0] == 'A' ) { //L,U',R',U,L',U2,R,U',R',U2,R   J
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            exLBD.digitalWrite(ENL, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'L', 'u', 'r', 'U', 'l', 'U', 'U', 'R', 'u', 'r', 'U', 'U', 'R'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
          }  //L,U',R',U,L',U2,R,U',R',U2,R

          else if (odczyt_alg[0] == 'B' ) { //R,L',B,R,B',L,B2,R',B,R,U2,2R    J2  *
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exLBD.digitalWrite(ENB, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'R', 'l', 'B', 'R', 'b', 'L', 'B', 'B', 'r', 'B', 'R', 'B', 'B', 'r', 'r'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exLBD.digitalWrite(ENB, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//R,L',B,R,B',L,B2,R',B,R,U2,2R

          else if (odczyt_alg[0] == 'C' ) { //R,U,R',U',R',F,2R,U',R',U',R,U,R',F'  T
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            exFRU.digitalWrite(ENF, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'R', 'U', 'r', 'u', 'r', 'F', 'R', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
          }//R,U,R',U',R',F,2R,U',R',U',R,U,R',F'  T

          else if (odczyt_alg[0] == 'D' ) { //L',U,B',F,U,F',U',F',L,F2,U',F',U',F,U,F',L',B,U',L  T *
            exFRU.digitalWrite(ENU, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exLBD.digitalWrite(ENB, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'l', 'U', 'b', 'F', 'U', 'f', 'u', 'f', 'L', 'f', 'f', 'u', 'f', 'u', 'F', 'U', 'f', 'l', 'B', 'u', 'L'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENU, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exLBD.digitalWrite(ENB, HIGH);
          }//L',U,B',F,U,F',U',F',L,F2,U',F',U',F,U,F',L',B,U',L  T

          else if (odczyt_alg[0] == 'E' ) { //L',U,R,U',L,U2,R',U,R,U2,R'    J2
            exFRU.digitalWrite(ENU, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exFRU.digitalWrite(ENR, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'l', 'U', 'R', 'u', 'L', 'u', 'u', 'r', 'U', 'R', 'U', 'U', 'r'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENU, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
          }//L',U,R,U',L,U2,R',U,R,U2,R'    J2

          else if (odczyt_alg[0] == 'F' ) { //R',L,F',R',U,L',F2,R,F',R',U2,2R    J  *
            exFRU.digitalWrite(ENF, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exFRU.digitalWrite(ENU, LOW);
            exFRU.digitalWrite(ENR, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'r', 'L', 'f', 'r', 'F', 'l', 'F', 'F', 'R', 'f', 'r', 'f', 'f', 'r', 'r'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
          }//R',L,F',R',U,L',F2,R,F',R',U2,2R    J

          else if (odczyt_alg[0] == 'I' ) { //2R,L',D,R,D',L,D2,R',D,R,D2,R   J2
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exLBD.digitalWrite(END, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'R', 'R', 'l', 'D', 'R', 'd', 'L', 'D', 'D', 'r', 'D', 'R', 'D', 'D', 'R'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exLBD.digitalWrite(END, HIGH);
          }//2R,L',D,R,D',L,D2,R',D,R,D2,R   J2

          else if (odczyt_alg[0] == 'J' ) { //R,L,B',R',B,L',B2,R,B',R',B2    J
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exLBD.digitalWrite(ENB, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'R', 'L', 'b', 'r', 'B', 'l', 'b', 'b', 'R', 'b', 'r', 'b', 'b'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exLBD.digitalWrite(ENL, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(ENB, HIGH);
          }//R,L,B',R',B,L',B2,R,B',R',B2    J

          else if (odczyt_alg[0] == 'K' ) { //L2,R,U,R',U',R',F,2R,U',R',U',R,U,R',F',L2    T
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exLBD.digitalWrite(ENL, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'L', 'L', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'L', 'L'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
          }//L2,R,U,R',U',R',F,2R,U',R',U',R,U,R',F',L2    T

          else if (odczyt_alg[0] == 'L' ) { //D,R',L',F,R,F',L,F2,R',F,R,F2,R',R,D'   J2  *
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exLBD.digitalWrite(END, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'D', 'r', 'l', 'F', 'R', 'f', 'L', 'f', 'f', 'r', 'F', 'R', 'f', 'f', 'd'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exLBD.digitalWrite(ENL, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(END, HIGH);
          }//D,R',L',F,R,F',L,F2,R',F,R,F2,R',R,D'   J2

          else if (odczyt_alg[0] == 'M' ) { //2R,L,D',R',D,L',D2,R,D',R',D2,R'    J
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exLBD.digitalWrite(END, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'r', 'r', 'L', 'd', 'r', 'D', 'l', 'd', 'd', 'R', 'd', 'r', 'd', 'd', 'r'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exLBD.digitalWrite(ENL, HIGH);
            exLBD.digitalWrite(END, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
          }//2R,L,D',R',D,L',D2,R,D',R',D2,R'    J

          else if (odczyt_alg[0] == 'N' ) { //R',L',F,R,F',L,F2,R',F,R,F2   J2
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exFRU.digitalWrite(ENF, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'r', 'l', 'F', 'R', 'f', 'L', 'f', 'f', 'r', 'F', 'R', 'F', 'F'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exLBD.digitalWrite(ENL, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
          }//R',L',F,R,F',L,F2,R',F,R,F2   J2

          else if (odczyt_alg[0] == 'O' ) { //D,2R,L',D,R,D',L,D2,R',D,R,D2,R,D'    J2
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exLBD.digitalWrite(END, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'D', 'R', 'R', 'l', 'D', 'R', 'd', 'L', 'D', 'D', 'r', 'D', 'R', 'd', 'd', 'R', 'd'};
            ukladanie(algorytm);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exLBD.digitalWrite(END, HIGH);
          }//D,2R,L',D,R,D',L,D2,R',D,R,D2,R,D'    J2

          else if (odczyt_alg[0] == 'P' ) { //D',R',L',F,R,F',L,F2,R',F,R,F2,D    J2
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exLBD.digitalWrite(END, LOW);
            exFRU.digitalWrite(ENF, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'d', 'r', 'l', 'F', 'R', 'f', 'L', 'f', 'f', 'r', 'F', 'R', 'F', 'F', 'D'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exLBD.digitalWrite(END, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
          }//D',R',L',F,R,F',L,F2,R',F,R,F2,D    J2

          else if (odczyt_alg[0] == 'R' ) { //L',R,U,R',U',R',F,2R,U',R',U',R,U,R',F',L    T *
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'l', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'L'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//L',R,U,R',U',R',F,2R,U',R',U',R,U,R',F',L    T

          else if (odczyt_alg[0] == 'S' ) { //U',F,B,U,B',R,B2,U',B',U',B,U,B',R',F',U    T
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            exLBD.digitalWrite(ENB, LOW);
            exFRU.digitalWrite(ENF, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'u', 'F', 'B', 'U', 'b', 'u', 'b', 'R', 'b', 'b', 'u', 'b', 'u', 'B', 'U', 'b', 'r', 'f', 'U'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
            exLBD.digitalWrite(ENB, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
          }//U',F,B,U,B',R,B2,U',B',U',B,U,B',R',F',U    T

          else if (odczyt_alg[0] == 'T' ) { //U,B'F,U,F',U',F',L,F2,U',F',U',F,U,F',L'B,U'    T
            exFRU.digitalWrite(ENU, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exLBD.digitalWrite(ENB, LOW);
            exFRU.digitalWrite(ENF, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'U', 'b', 'F', 'U', 'f', 'u', 'f', 'L', 'f', 'f', 'u', 'f', 'u', 'F', 'U', 'f', 'l', 'B', 'u'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENU, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exLBD.digitalWrite(ENB, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
          }//U,B'F,U,F',U',F',L,F2,U',F',U',F,U,F',L'B,U'    T

          else if (odczyt_alg[0] == 'U' ) { //L,R,U,R',U',R',F,2R,U',R',U',R,U,R',F',L'   T
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'L', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'l'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//L,R,U,R',U',R',F,2R,U',R',U',R,U,R',F',L'   T

          else if (odczyt_alg[0] == 'W' ) { //U2,R',L,U,L',U',L',B,L2,U',L',U',L,U,L',F',R,U2   T
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exLBD.digitalWrite(ENB, LOW);
            exFRU.digitalWrite(ENF, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'u', 'u', 'r', 'L', 'U', 'l', 'u', 'l', 'B', 'L', 'L', 'u', 'l', 'u', 'L', 'U', 'l', 'b', 'R', 'U', 'U'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
            exLBD.digitalWrite(ENB, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exFRU.digitalWrite(ENF, HIGH);

          }//U2,R',L,U,L',U',L',B,L2,U',L',U',L,U,L',F',R,U2   T

          else if (odczyt_alg[0] == 'X' ) { //U,B,F,U,F',U',L,F2,U',F',U',F,U,F',L',B',U'   T
            exFRU.digitalWrite(ENU, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exLBD.digitalWrite(ENB, LOW);
            exFRU.digitalWrite(ENF, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'U', 'B', 'F', 'U', 'f', 'u', 'f', 'L', 'f', 'f', 'u', 'f', 'u', 'F', 'U', 'f', 'l', 'b', 'u'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENU, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exLBD.digitalWrite(ENB, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
          }//U,B,F,U,F',U',L,F2,U',F',U',F,U,F',L',B',U'   T

          else if (odczyt_alg[0] == 'Y' ) { //U',F',B,U,B',R,B2,U',B',U',B,U,B',R',F,U    T
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(ENB, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'u', 'f', 'B', 'U', 'b', 'u', 'b', 'R', 'b', 'b', 'u', 'b', 'u', 'B', 'U', 'b', 'r', 'F', 'U'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(ENB, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//U',F',B,U,B',R,B2,U',B',U',B,U,B',R',F,U    T

          else if (odczyt_alg[0] == 'Z' ) { //U2,R,L,U,L',U',L',B,L2,U',L',U',L,U,L',F',R',U2    T
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(ENL, LOW);
            exLBD.digitalWrite(ENB, LOW);
            exFRU.digitalWrite(ENU, LOW);
            exFRU.digitalWrite(ENF, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'U', 'U', 'R', 'L', 'U', 'l', 'u', 'l', 'B', 'L', 'L', 'u', 'l', 'u', 'L', 'U', 'l', 'b', 'r', 'u', 'u'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
            exLBD.digitalWrite(ENB, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
          }//U2,R,L,U,L',U',L',B,L2,U',L',U',L,U,L',F',R',U2    T




          else if (odczyt_alg[0] == 'h' ) { //U',L,U2,L',U2,L,F',L',U',L,U,L,F,L2,U2
            Serial.println("parity");
            exFRU.digitalWrite(ENU, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exLBD.digitalWrite(ENL, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'u', 'L', 'u', 'u', 'l', 'u', 'u', 'L', 'f', 'l', 'u', 'L', 'U', 'L', 'F', 'L', 'L', 'U', 'U'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENU, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exLBD.digitalWrite(ENL, HIGH);
          }//U',L,U2,L',U2,L,F',L',U',L,U,L,F,L2,U2






          else if (odczyt_alg[0] == '1' ) { //R,D',y,D,R'
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(END, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'R', 'd', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'D', 'r'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(END, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);

          }//R,D',y,D,R'

          else if (odczyt_alg[0] == '2' ) { //F,y,F'
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'F', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'f'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);

          }//F,y,F'

          else if (odczyt_alg[0] == '3' ) { /// j
            exLBD.digitalWrite(ENL, LOW);
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'r', 'U', 'L', 'u', 'R', 'U', 'U', 'l', 'U', 'L', 'U', 'U', 'l'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exLBD.digitalWrite(ENL, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }/// j

          else if (odczyt_alg[0] == '4' ) { //D,F',y,F,D'
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(END, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'D', 'f', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'F', 'd'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(END, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);

          }//D,F'

          else if (odczyt_alg[0] == '5' ) { //2R,F
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'R', 'R', 'F', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'f', 'r', 'r'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//2R,F

          else if (odczyt_alg[0] == '6' ) { //2F j
            exLBD.digitalWrite(ENL, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);

            char algorytm[22] = {'f', 'f', 'r', 'U', 'L', 'u', 'R', 'U', 'U', 'l', 'U', 'L', 'U', 'U', 'l', 'F', 'F'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exLBD.digitalWrite(ENL, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//2F j

          else if (odczyt_alg[0] == '7' ) { //F'
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'f', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'F'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//F'

          else if (odczyt_alg[0] == 'x' ) { //R',F,y,F',R
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'r', 'F', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'f', 'R'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//R',F,y,F',R

          else if (odczyt_alg[0] == 'r' ) {
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'r', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'R'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//R'

          else if (odczyt_alg[0] == 'z' ) { //F',D,Y,D',F
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(END, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'f', 'D', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'd', 'F'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(END, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//F',D

          else if (odczyt_alg[0] == 'a' ) { //D',R,y,R',D
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(END, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'d', 'R', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'r', 'D'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(END, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//D',R

          else if (odczyt_alg[0] == 'm' ) { //R
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'R', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//R

          else if (odczyt_alg[0] == 'c' ) { //D,R
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(END, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'D', 'R', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'r', 'd'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(END, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//D,R

          else if (odczyt_alg[0] == '9' ) { //F j
            exLBD.digitalWrite(ENL, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'F', 'r', 'U', 'L', 'u', 'R', 'U', 'U', 'l', 'U', 'L', 'U', 'U', 'l', 'f'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exLBD.digitalWrite(ENL, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//F j

          else if (odczyt_alg[0] == 'k' ) { //2R
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'r', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'r'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//2R

          else if (odczyt_alg[0] == 'y' ) { //2F,D
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(END, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'F', 'F', 'D', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'd', 'F', 'F'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(END, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//2F,D

          else if (odczyt_alg[0] == '8' ) { //2F
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'F', 'F', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'F', 'F'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//2F

          else if (odczyt_alg[0] == 'p' ) { //2D
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(END, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'D', 'D', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'D', 'D'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(END, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//2D

          else if (odczyt_alg[0] == 'b' ) { //D'
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(END, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'d', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'D'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(END, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//D'

          else if (odczyt_alg[0] == 'n' ) {
            exFRU.digitalWrite(ENR, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }///

          else if (odczyt_alg[0] == 'd' ) { //D
            exFRU.digitalWrite(ENR, LOW);
            exLBD.digitalWrite(END, LOW);
            exFRU.digitalWrite(ENF, LOW);
            exFRU.digitalWrite(ENU, LOW);
            delay(przerwa_po_wlaczeniu_silnikow);
            char algorytm[22] = {'D', 'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R', 'd'};
            ukladanie(algorytm);
            delay(przerwa_po_wlaczeniu_silnikow);
            exFRU.digitalWrite(ENR, HIGH);
            exLBD.digitalWrite(END, HIGH);
            exFRU.digitalWrite(ENF, HIGH);
            exFRU.digitalWrite(ENU, HIGH);
          }//D

          delay(przerwa);
        }
        else {
          Serial.println("KONIEC");
          
          delay(10000);
          break;
        }
      }
  }

}
/*
void alg_Y() {    //____________ R U' R' U' R U R' F' R U R' U' R' F R ___________
  //  exFRU.digitalWrite(ENF, LOW);
  //  exFRU.digitalWrite(ENR, LOW);
  //  exFRU.digitalWrite(ENU, LOW);
  //  delay(przerwa_po_wlaczeniu_silnikow);
  delay(przerwa_miedzy_ruchami);
  char algorytm[22] = {'R', 'u', 'r', 'u', 'R', 'U', 'r', 'f', 'R', 'U', 'r', 'u', 'r', 'F', 'R'};
  ukladanie(algorytm);

  //           delay(przerwa_po_wlaczeniu_silnikow);


  exFRU.digitalWrite(ENF, HIGH);
  exFRU.digitalWrite(ENR, HIGH);
  //  exFRU.digitalWrite(ENU, HIGH);


  
    exFRU.digitalWrite(DFRU, LOW);  //HIGH = ()'
    delay(przerwa_miedzy_ruchami);
    krokFRU(STR); //R
    exFRU.digitalWrite(DFRU, HIGH);
    delay(przerwa_miedzy_ruchami);
    krokFRU(STU); //U'
    delay(przerwa_miedzy_ruchami);
    krokFRU(STR); //R'
    delay(przerwa_miedzy_ruchami);
    krokFRU(STU); //U'
    exFRU.digitalWrite(DFRU, LOW);
    delay(przerwa_miedzy_ruchami);
    krokFRU(STR); //R
    delay(przerwa_miedzy_ruchami);
    krokFRU(STU); //U
    exFRU.digitalWrite(DFRU, HIGH);
    delay(przerwa_miedzy_ruchami);
    krokFRU(STR); //R'
    delay(przerwa_miedzy_ruchami);
    krokFRU(STF); //F'
    exFRU.digitalWrite(DFRU, LOW);
    delay(przerwa_miedzy_ruchami);
    krokFRU(STR); //R
    delay(przerwa_miedzy_ruchami);
    krokFRU(STU); //U
    exFRU.digitalWrite(DFRU, HIGH);
    delay(przerwa_miedzy_ruchami);
    krokFRU(STR); //R'
    delay(przerwa_miedzy_ruchami);
    krokFRU(STU); //U'
    delay(przerwa_miedzy_ruchami);
    krokFRU(STR); //R'
    exFRU.digitalWrite(DFRU, LOW);  //_() _____
    delay(przerwa_miedzy_ruchami);
    krokFRU(STF); //F
    delay(przerwa_miedzy_ruchami);
    krokFRU(STR); //R
  


}

void alg_J() {    //____________ R' U L U' R U2 L' U L U2 L' ___________

  //  exFRU.digitalWrite(ENR, LOW);
  //  exFRU.digitalWrite(ENU, LOW);
  //  exLBD.digitalWrite(ENL, LOW);
  //  delay(przerwa_po_wlaczeniu_silnikow);
  delay(przerwa_miedzy_ruchami);
  char algorytm[22] = {'r', 'U', 'L', 'u', 'R', 'U', 'U', 'l', 'U', 'L', 'U', 'U', 'l'};
  ukladanie(algorytm);
  //  delay(przerwa_po_wlaczeniu_silnikow);
  //  exFRU.digitalWrite(ENR, HIGH);
  //  exFRU.digitalWrite(ENU, HIGH);
  exLBD.digitalWrite(ENL, HIGH);


  
    exFRU.digitalWrite(DFRU, HIGH);
    delay(przerwa_miedzy_ruchami);
    krokFRU(STR); //R'
    exFRU.digitalWrite(DFRU, LOW);
    delay(przerwa_miedzy_ruchami);
    krokFRU(STU); //U
    exLBD.digitalWrite(DLBD, LOW);
    delay(przerwa_miedzy_ruchami);
    krokLBD(STL); //L
    exFRU.digitalWrite(DFRU, HIGH);
    delay(przerwa_miedzy_ruchami);
    krokFRU(STU); //U'
    exFRU.digitalWrite(DFRU, LOW);
    delay(przerwa_miedzy_ruchami);
    krokFRU(STR); //R
    delay(przerwa_miedzy_ruchami);
    krokFRU(STU); //U
    krokFRU(STU); //U
    exLBD.digitalWrite(DLBD, HIGH);
    delay(przerwa_miedzy_ruchami);
    krokLBD(STL); //L'
    delay(przerwa_miedzy_ruchami);
    krokFRU(STU); //U
    exLBD.digitalWrite(DLBD, LOW);
    delay(przerwa_miedzy_ruchami);
    krokLBD(STL); //L
    delay(przerwa_miedzy_ruchami);
    krokFRU(STU); //U
    krokFRU(STU); //U
    exLBD.digitalWrite(DLBD, HIGH);
    delay(przerwa_miedzy_ruchami);
    krokLBD(STL); //L'
  
}*/

void ukladanie (char inAlg[]) {


  int a = 0;
  for (int m = 0; m < 23; m++) {
    if (inAlg[a] == 'F') {
      if (exFRU.digitalRead(DFRU) == HIGH) {
        exFRU.digitalWrite(DFRU, LOW);
      }
      if (inAlg[a - 1] != 'F') {
        delay(przerwa_miedzy_ruchami);
      }
      krokFRU(STF); //F
    }
    if (inAlg[a] == 'f') {
      if (exFRU.digitalRead(DFRU) == LOW) {
        exFRU.digitalWrite(DFRU, HIGH);
      }
      if (inAlg[a - 1] != 'f') {
        delay(przerwa_miedzy_ruchami);
      }
      krokFRU(STF); //F'
    }
    if (inAlg[a] == 'R') {
      if (exFRU.digitalRead(DFRU) == HIGH) {
        exFRU.digitalWrite(DFRU, LOW);
      }
      if (inAlg[a - 1] != 'R') {
        delay(przerwa_miedzy_ruchami);
      }
      krokFRU(STR); //R
    }
    if (inAlg[a] == 'r') {
      if (exFRU.digitalRead(DFRU) == LOW) {
        exFRU.digitalWrite(DFRU, HIGH);
      }
      if (inAlg[a - 1] != 'r') {
        delay(przerwa_miedzy_ruchami);
      }
      krokFRU(STR); //R'
    }
    if (inAlg[a] == 'U') {
      if (exFRU.digitalRead(DFRU) == HIGH) {
        exFRU.digitalWrite(DFRU, LOW);
      }
      if (inAlg[a - 1] != 'U') {
        delay(przerwa_miedzy_ruchami);
      }
      krokFRU(STU); //U
    }
    if (inAlg[a] == 'u') {
      if (exFRU.digitalRead(DFRU) == LOW) {
        exFRU.digitalWrite(DFRU, HIGH);
      }
      if (inAlg[a - 1] != 'u') {
        delay(przerwa_miedzy_ruchami);
      }
      krokFRU(STU); //U'
    }
    if (inAlg[a] == 'L') {
      if (exLBD.digitalRead(DLBD) == HIGH) {
        exLBD.digitalWrite(DLBD, LOW);
      }
      if (inAlg[a - 1] != 'L') {

        delay(przerwa_miedzy_ruchami);
      }
      krokLBD(STL); //L
    }
    if (inAlg[a] == 'l') {
      if (exLBD.digitalRead(DLBD) == LOW) {
        exLBD.digitalWrite(DLBD, HIGH);
      }
      if (inAlg[a - 1] != 'l') {
        delay(przerwa_miedzy_ruchami);
      }
      krokLBD(STL); //L'
    }
    if (inAlg[a] == 'B') {
      if (exLBD.digitalRead(DLBD) == HIGH) {
        exLBD.digitalWrite(DLBD, LOW);
      }
      if (inAlg[a - 1] != 'B') {
        delay(przerwa_miedzy_ruchami);
      }
      krokLBD(STB); //L
    }
    if (inAlg[a] == 'b') {
      if (exLBD.digitalRead(DLBD) == LOW) {
        exLBD.digitalWrite(DLBD, HIGH);
      }
      if (inAlg[a - 1] != 'b') {
        delay(przerwa_miedzy_ruchami);
      }
      krokLBD(STB); //L'
    }
    if (inAlg[a] == 'D') {
      if (exLBD.digitalRead(DLBD) == HIGH) {
        exLBD.digitalWrite(DLBD, LOW);
      }
      if (inAlg[a - 1] != 'D') {
        delay(przerwa_miedzy_ruchami);
      }
      krokLBD(STD); //D
    }
    if (inAlg[a] == 'd') {
      if (exLBD.digitalRead(DLBD) == LOW) {
        exLBD.digitalWrite(DLBD, HIGH);
      }
      if (inAlg[a - 1] != 'd') {
        delay(przerwa_miedzy_ruchami);
      }
      krokLBD(STD); //D'
    }
    a++;
  }

}

void krokFRU(int STEP) {
  przerwa_krok = map(analogRead(potpin), 0, 1023, 1700, 6000);
  for (int i = 0; i < 50; i++) {
    exFRU.digitalWrite(STEP, HIGH);
    exFRU.digitalWrite(STEP, LOW);
    delayMicroseconds(przerwa_krok);
  }
  lcd.clear();
  lcd.home(); lcd.print(licznik_ruchow); lcd.print("/"); lcd.print(ilosc_ruchow);
  licznik_ruchow--;
}

void krokLBD(int STEP) {
  przerwa_krok = map(analogRead(potpin), 0, 1023, 1700, 6000);
  for (int i = 0; i < 50; i++) {
    exLBD.digitalWrite(STEP, HIGH);
    exLBD.digitalWrite(STEP, LOW);
    delayMicroseconds(przerwa_krok);
  }
  lcd.clear();
  lcd.home(); lcd.print(licznik_ruchow); lcd.print("/"); lcd.print(ilosc_ruchow);
  licznik_ruchow--;
}

void lcd_skrot() {
  lcd.setCursor(0, 0); lcd.print("Kolory ze"); lcd.setCursor(14, 0); lcd.print("_____");
  lcd.setCursor(0, 1); lcd.print("sciany z");  lcd.setCursor(13, 1); lcd.print("|_"); lcd.setCursor(19, 1); lcd.print("|"); lcd.setCursor(0, 1); lcd.print(char(3));
  lcd.setCursor(0, 3); lcd.print("srodkiem:"); lcd.setCursor(13, 3); lcd.print("|"); lcd.setCursor(19, 3); lcd.print("|"); lcd.setCursor(0, 3); lcd.print(char(3));
}



void odebrane_bt(char dane[]) {
  int m = 0;
  int mm = 0;
  if (dane[0] == 'R') {
    if (sciana_R == 1) {
      sciana_zrobiona = 1;
    }
    kolor = 0;
    licznik_scian++;
  }
  else if (dane[0] == 'Y') {
    if (sciana_Y == 1) {
      sciana_zrobiona = 1;
    }
    kolor = 1;
    licznik_scian++;
  }
  else if (dane[0] == 'O') {
    if (sciana_O == 1) {
      sciana_zrobiona = 1;
    }
    kolor = 2;
    licznik_scian++;
  }
  else if (dane[0] == 'W') {
    if (sciana_W == 1) {
      sciana_zrobiona = 1;
    }
    kolor = 3;
    licznik_scian++;
  }
  else if (dane[0] == 'B') {
    if (sciana_B == 1) {
      sciana_zrobiona = 1;
    }
    kolor = 4;
    licznik_scian++;
  }
  else if (dane[0] == 'G') {
    if (sciana_G == 1) {
      sciana_zrobiona = 1;
    }
    kolor = 5;
    licznik_scian++;
  }
  for (m = 0; m < 9; m++) {
    kolory[m] = dane[m + 1];
  }
  for (mm = 0; mm < 11; mm++) {
    dane[mm] = 0;
  }

}

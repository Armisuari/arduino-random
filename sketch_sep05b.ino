int setCycles; 
int setDuty;

void set(int cycles, float duty) 
{ 
  char oldSREG;
  int dutyCycle = cycles * duty;
  if (dutyCycle < 6) {
    dutyCycle = 6; 
  }
  if (dutyCycle > 1015) { 
    dutyCycle = 1015;
  }
  if (cycles < 50) { 
    cycles = 50;
  }
  oldSREG = SREG; 
  cli();
  ICR1 = cycles; 
  OCR1A = dutyCycle; 
  SREG = oldSREG;
}


//ins sensor tegangan
int SensorTegangan = A1;
int potensiom = A2;
float Vmodul = 0.0; 
float hasil = 0.0;
float R1 = 30000.0; //30k
float R2 = 7500.0; //7500 ohm resistor, 
int value = 0;
float setPoin;

//ins fuzzy
float error;
float NB,NS,NK,NOL,PK,PS,PB;
float derror;
float DNB,DNS,DNK,DNOL,DPK,DPS,DPB;
float dduty;
float cNB,cNS,cNK,cNOL,cPK,cPS,cPB;
float tg;
float dtc;
long previousMillis = 0;
long currentMillis =0;
float u;
float v;
float out;
float last_eror;
float A,B, du;
float duty = 0;
float lsduty;

//ins rule
float rul1,rul2,rul3,rul4,rul5,rul6,rul7,rul8,rul9,rul10,rul11,rul12,rul13,rul14,rul15,rul16,rul17,rul18,rul19,rul20,rul21,rul22,rul23,rul24,rul25;
float rul26,rul27,rul28,rul29,rul30,rul31,rul32,rul33,rul34,rul35,rul36,rul37,rul38,rul39,rul40,rul41,rul42,rul43,rul44,rul45,rul46,rul47,rul48,rul49;
//ins Z
float z1,z2,z3,z4,z5a,z5b,z6a,z6b,z7a,z7b;                              //1-7
float z8,z9,z10,z11a,z11b,z12a,z12b,z13a,z13b,z14a,z14b;                //8-14
float z15,z16,z17a,z17b,z18a,z18b,z19a,z19b,z20a,z20b,z21a,z21b;         //15-21
float z22,z23a,z23b,z24a,z24b,z25a,z25b,z26a,z26b,z27a,z27b,z28;        //21-28
float z29a,z29b,z30a,z30b,z31a,z31b,z32a,z32b,z33a,z33b,z34,z35;         //29-35
float z36a,z36b,z37a,z37b,z38a,z38b,z39a,z39b,z40,z41,z42;          //36-42
float z43a,z43b,z44a,z44b,z45a,z45b,z46,z47,z48,z49;                    //37-49


void setup(){
   Serial.begin(9600);
  TCCR1A = 0;
  TCCR1B = _BV(WGM13);
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12)); 
  TCCR1B |= _BV(CS10);
  DDRB |= _BV(PORTB1); 
  TCCR1A |= _BV(COM1A1);
   previousMillis = millis();
}


//input1
unsigned char aNB(){
  if (error <= -0.75){NB =1;}
  else if (error >-0.75 && error <=0.5){NB=(0.5-error)/0.25;}
  else if (error > -0.5){NB =0;}
  return NB;
}
unsigned char aNS(){
  if (error <= -0.75){NS =0;}
  else if (error> -0.25){NS =0;}
  else if (error >=-0.75 && error <=-0.5){NS=(0.5-error)/0.25;}
  else if (error >-0.5 && error <=-0.25){NS=(-0.25-error)/0.25;}
  return NS;
}
unsigned char aNK(){
  if (error <= -0.5){NK =0;}
  else if (error > 0){NK =0;}
  else if (error > -0.5 && error <=-0.25){NK=(error+0.5)/0.25;}
  else if (error > -0.25 && error <= -0){NK=(0-error)/0.25;}
  return NK;
}
unsigned char aNOL(){
  if (error <= -0.25){NOL =0;}
  else if (error > 0.25){NOL = 0;}
  else if (error > 0.25 && error <= 0){NOL= (error+0.25)/0.25;}
  else if (error > 0 && error <= 0.25){NOL=(0.25-error)/0.25;}
  return NOL;
}
unsigned char aPK(){
  if (error <= 0){PK =0;}
  else if (error > 0.5){PK = 0;}
  else if (error > 0 && error <= 0.25){PK= (error-0)/0.25;}
  else if (error > 0.25 && error <= 0.5){PK=(0.5-error)/0.25;}
  return PK;
}
unsigned char aPS(){
  if (error <= 0.25){PS =0;}
  else if (error > 0.75){PS = 0;}
  else if (error > 0.25 && error <= 0.5){PS= (error-0.25)/0.25;}
  else if (error > 0.5 && error <= 0.75){PS=(0.75-error)/0.25;}
  return PS;
}
unsigned char aPB(){
  if (error <= 0.75){PB =1;}
  else if (error > 0.5 && error <= 0.75){PB= (error-0.5)/0.25;}
  else if (error > 0.5){PB=0;}
  return PB;
}

//input2
unsigned char aDNB(){
  if (derror <= -0.75){DNB =1;}
  else if (derror >-0.75 && derror <=0.5){DNB=(0.5-derror)/0.25;}
  else if (derror > -0.5){DNB =0;}
  return DNB;
}
unsigned char aDNS(){
  if (derror <= -0.75){DNS =0;}
  else if (derror> -0.25){DNS =0;}
  else if (derror >=-0.75 && derror <=-0.5){DNS=(0.5-derror)/0.25;}
  else if (derror >-0.5 && derror <=-0.25){DNS=(-0.25-derror)/0.25;}
  return DNS;
}
unsigned char aDNK(){
  if (derror <= -0.5){DNK =0;}
  else if (derror > 0){DNK =0;}
  else if (derror > -0.5 && derror <=-0.25){DNK=(derror+0.5)/0.25;}
  else if (derror > -0.25 && derror <= -0){DNK=(0-derror)/0.25;}
  return DNK;
}
unsigned char aDNOL(){
  if (derror <= -0.25){DNOL =0;}
  else if (derror > 0.25){DNOL = 0;}
  else if (derror > 0.25 && derror <= 0){DNOL= (derror+0.25)/0.25;}
  else if (derror > 0 && derror <= 0.25){DNOL=(0.25-derror)/0.25;}
  return DNOL;
}
unsigned char aDPK(){
  if (derror <= 0){DPK =0;}
  else if (derror > 0.5){DPK = 0;}
  else if (derror > 0 && derror <= 0.25){DPK= (derror-0)/0.25;}
  else if (derror > 0.25 && derror <= 0.5){DPK=(0.5-derror)/0.25;}
  return DPK;
}
unsigned char aDPS(){
  if (derror <= 0.25){DPS =0;}
  else if (derror > 0.75){DPS = 0;}
  else if (derror > 0.25 && derror <= 0.5){DPS= (derror-0.25)/0.25;}
  else if (derror > 0.5 && derror <= 0.75){DPS=(0.75-derror)/0.25;}
  return DPS;
}
unsigned char aDPB(){
  if (derror <= 0.75){DPB =1;}
  else if (derror > 0.5 && derror <= 0.75){DPB= (derror-0.5)/0.25;}
  else if (derror > 0.5){DPB=0;}
  return DPB;
}


//output1 
unsigned char oNB(){
  if (dduty <= -0.15){cNB =1;}
  else if (dduty >-0.15 && dduty <=-0.02){cNB=(-0.02-dduty)/0.13;}
  else if (dduty > -0.02){cNB =0;}
  return cNB;
}
unsigned char oNS(){
  if (dduty <= -0.15){cNS =0;}
  else if (dduty > -0.002){cNS =0;}
  else if (dduty >-0.15 && dduty <=-0.02){cNS=(dduty+0.15)/0.13;}
  else if (dduty >-0.02 && dduty <=-0.002){cNS=(-0.002-dduty)/0.018;}
  return cNS;
}
unsigned char oNK(){
  if (dduty <= -0.002){cNK =0;}
  else if (dduty > 0){cNK =0;}
  else if (dduty >-0.02 && dduty <=-0.005){cNK=(dduty+0.02)/0.018;}
  else if (dduty >-0.002 && dduty <=-0){cNK=(0-dduty)/0.002;}
  return cNK;
}
unsigned char oNOL(){
  if (dduty <= -0.002){cNOL =0;}
  else if (dduty > 0.002){cNOL =0;}
  else if (dduty >-0.002 && dduty <=0){cNOL=(dduty+0.02)/0.002;}
  else if (dduty >0 && dduty <= 0.002){cNOL=(0.002-dduty)/0.002;}
  return cNOL;
}
unsigned char oPK(){
  if (dduty <= 0){cPK =0;}
  else if (dduty > 0.02){cPK =0;}
  else if (dduty >0 && dduty <=0.002){cPK=(dduty-0)/0.02;}
  else if (dduty >0.002 && dduty <= 0.02){cPK=(0.02-dduty)/0.18;}
  return cPK;
}
unsigned char oPS(){
  if (dduty <= 0.002){cPS =0;}
  else if (dduty > 0.15){cPS =0;}
  else if (dduty > 0.002 && dduty <= 0.02){cPS=(dduty-0.002)/0.13;}
  else if (dduty >0.02 && dduty <= 0.15){cPS=(0.15-dduty)/0.13;}
  return cPS;
}
unsigned char oPB(){
  if (dduty <= 0.02){cPB =0;}
 else if (dduty > 0.02 && dduty <= 0.15){cPB=(dduty-0.02)/0.13;}
 else if (dduty >0.15){cPB=1;}
  return cPS;
}

void fuzzifikasi() {
 aNB();
 aNS();
 aNK();
 aNOL();
 aPK();
 aPS();
 aPB();

 aDNB();
 aDNS();
 aDNK();
 aDNOL();
 aDPK();
 aDPS();
 aDPB();

 oPB();
 oNS();
 oNK();
 oNOL();
 oPK();
 oPS();
 oPB();
}

void mamdani () {
  if (u<=v) {out=u;}
  if (u>v) {out=v;}
}

void fuzy_rule() {
    setPoin=522;
    error=(setPoin-value)/setPoin;
    derror=last_eror-error;
    last_eror=error;
    fuzzifikasi();

     //jika eror negatif besar
        //deror neg besar maka du neg besar
        u=NB;
        v=DNB;
        mamdani();
        rul1=out;
        z1=(-0.02)-(0.13*rul1);
        //deror neg sedang maka du neg besar
        u=NB;
        v=DNS;
        mamdani();
        rul2=out;
        z2=(-0.02)-(0.13*rul2);
        //deror neg kecil maka du neg besar
        u=NB;
        v=DNK;
        mamdani();
        rul3=out;
        z3=(-0.02)-(0.13*rul3);
        //deror nol maka du neg besar
        u=NB;
        v=DNOL;
        mamdani();
        rul4=out;
        z4=(-0.02)-(0.13*rul4);
        //deror pos kecil maka du neg sedang
        u=NB;
        v=DPK;
        mamdani();
        rul5=out;
        z5a=(-0.15)+(0.13*rul5);
        z5b=(-0.002)-(0.018*rul5);
        //deror pos sedang maka du neg kecil
        u=NB;
        v=DPS;
        mamdani();
        rul6=out;
        z6a=(-0.02)+(0.018*rul6);
        z6b=0-(0.002*rul6);
        //deror pos besar maka du nol
        u=NB;
        v=DPB;
        mamdani();
        rul7=out;
        z7a=(-0.002)+(0.002*rul7);
        z7b=(0.002)-(0.002*rul7);

     //jika eror negatif sedang
        //deror neg besar maka du neg besar
        u=NS;
        v=DNB;
        mamdani();
        rul8=out;
        z8=(-0.02)-(0.13*rul8);
        //deror neg sedang maka du neg besar
        u=NS;
        v=DNS;
        mamdani();
        rul9=out;
        z9=(-0.02)-(0.13*rul9);
        //deror neg kecil maka du neg besar
        u=NS;
        v=DNK;
        mamdani();
        rul10=out;
        z10=(-0.02)-(0.13*rul10);
        //deror nol maka du neg sedang
        u=NS;
        v=DNOL;
        mamdani();
        rul11=out;
        z11a=(-0.15)+(0.13*rul11);
        z11b=(-0.002)-(0.018*rul11);
        //deror pos kecil maka du neg kecil
        u=NS;
        v=DPK;
        mamdani();
        rul12=out;
        z12a=(-0.02)+(0.018*rul12);
        z12b=0-(0.002*rul12);
        //deror pos sedang maka du nol
        u=NS;
        v=DPS;
        mamdani();
        rul13=out;
        z13a=(-0.002)-(0.002*rul13);
        z13b=0.002+(0.002*rul13);
        //deror pos besar maka du pos kecil
        u=NS;
        v=DPB;
        mamdani();
        rul14=out;
        z14a=0+(0.002*rul14);
        z14b=0.02-(0.018*rul14);

//jika eror negatif kecil
        //deror neg besar maka du neg besar
        u=NK;
        v=DNB;
        mamdani();
        rul15=out;
        z15=(-0.02)-(0.13*rul15);
        //deror neg sedang maka du neg besar
        u=NK;
        v=DNS;
        mamdani();
        rul16=out;
        z16=(-0.02)-(0.13*rul16);
        //deror neg kecil maka du neg sedang
        u=NK;
        v=DNK;
        mamdani();
        rul17=out;
        z17a=(-0.15)+(0.13*rul17);
        z17b=(-0.002)-(0.018*rul17);
        //deror nol maka du neg kecil
        u=NK;
        v=DNOL;
        mamdani();
        rul18=out;
        z18a=(-0.02)+(0.018*rul18);
        z18b=0-(0.002*rul18);
        //deror pos kecil maka du nol
        u=NK;
        v=DPK;
        mamdani();
        rul19=out;
        z19a=(-0.002)+(0.002*rul19);
        z19b=0.002-(0.002*rul19);
        //deror pos sedang maka du pos kecil
        u=NK;
        v=DPS;
        mamdani();
        rul20=out;
        z20a=0+(0.002*rul20);
        z20b=0.02-(0.018*rul20);
        //deror pos besar maka du pos sedang
        u=NK;
        v=DPB;
        mamdani();
        rul21=out;
        z21a=(0.002)+(0.018*rul21);
        z21b=0.15-(0.13*rul21);

//jika eror nol
        //deror neg besar maka du neg besar
        u=NOL;
        v=DNB;
        mamdani();
        rul22=out;
        z22=(-0.02)-(0.13*rul22);
        //deror neg sedang maka du neg sedang
        u=NOL;
        v=DNS;
        mamdani();
        rul23=out;
        z23a=(-0.15)+(0.13*rul23);
        z23b=(-0.002)-(0.018*rul23);
        //deror neg kecil maka du neg kecil
        u=NOL;
        v=DNK;
        mamdani();
        rul24=out;
        z24a=(-0.02)+(0.018*rul24);
        z24b=0-(0.002*rul24);
        //deror nol maka du nol
        u=NOL;
        v=DNOL;
        mamdani();
        rul25=out;
        z25a=(-0.002)+(0.002*rul25);
        z25b=0.002-(0.002*rul25);
        //deror pos kecil maka du pos kecil
        u=NOL;
        v=DPK;
        mamdani();
        rul26=out;
        z26a=0+(0.002*rul26);
        z26b=0.02-(0.018*rul26);
        //deror pos sedang maka du pos sedang
        u=NOL;
        v=DPS;
        mamdani();
        rul27=out;
        z27a=0.002+(0.018*rul27);
        z27b=0.15-(0.13*rul27);
        //deror pos besar maka du pos besar
        u=NOL;
        v=DPB;
        mamdani();
        rul28=out;
        z28=0.02+(0.13*rul28);

//jika eror pos kecil
        //deror neg besar maka du neg sedang
        u=PK;
        v=DNB;
        mamdani();
        rul29=out;
        z29a=(-0.15)+(0.13*rul29); 
        z29b=(-0.002)-(0.018*rul29);
        //deror neg sedang maka du neg kecil
        u=PK;
        v=DNS;
        mamdani();
        rul30=out;
        z30a=(-0.02)+(0.018*rul30);
        z30b=0-(0.002*rul30);
        //deror neg kecil maka du nol
        u=PK;
        v=DNK;
        mamdani();
        rul31=out;
        z31a=(-0.002)+(0.002*rul31);
        z31b=(0.002)-(0.002*rul31);
        //deror nol maka du pos kecil
        u=PK;
        v=DNOL;
        mamdani();
        rul32=out;
        z32a=0+(0.002*rul32);
        z32b=0.02-(0.018*rul32);
        //deror pos kecil maka du pos sedang
        u=PK;
        v=DPK;
        mamdani();
        rul33=out;
        z33a=0.002+(0.018*rul33);
        z33b=0.15-(0.13*rul33);
        //deror pos sedang maka du pos besar
        u=PK;
        v=DPS;
        mamdani();
        rul34=out;
        z34=0.02+(0.13*rul34);
         //deror pos besar maka du pos besar
         u=PK;
         v=DPS;
         mamdani();
         rul35=out;
         z35=0.02+(0.13*rul35);

   //jika eror pos sedang
        //deror neg besar maka du neg kecil
        u=PS;
        v=DNB;
        mamdani();
        rul36=out;
        z36a=(-0.02)+(0.018*rul36);
        z36b=0-(0.002*rul36);
        //deror neg sedang maka du nol
        u=PS;
        v=DNS;
        mamdani();
        rul37=out;
        z37a=(-0.002)+(0.002*rul37);
        z37b=0.002-(0.002*rul37);
        //deror neg kecil maka du pos kecil
        u=PS;
        v=DNK;
        mamdani();
        rul38=out;
        z38a=0+(0.002*rul38);
        z38b=0.02-(0.018*rul38);
        //deror nol maka du pos sedang
        u=PS;
        v=DNOL;
        mamdani();
        rul39=out;
        z39a=0.002+(0.018*rul39);
        z39b=0.15-(0.13*rul39);
        //deror pos kecil maka du pos besar
        u=PS;
        v=DPK;
        mamdani();
        rul40=out;
        z40=0.02+(0.13*rul40);
        //deror pos sedang maka du pos besar
        u=PS;
        v=DPS;
        mamdani();
        rul41=out;
        z41=0.02+(0.13*rul41);
        //deror pos besar maka du pos besar
        u=PS;
        v=DPB;
        mamdani();
        rul42=out;
        z42=(-0.02)+(0.13*rul42);

    //jika eror pos besar
        //deror neg besar maka du nol
        u=PB;
        v=DNB;
        mamdani();
        rul43=out;
        z43a=(-0.002)+(0.002*rul43);
        z43b=0.002-(0.002*rul43);
        //deror neg sedang maka du pos kecil
        u=PB;
        v=DNS;
        mamdani();
        rul44=out;
        z44a=0+(0.002*rul44);
        z44b=0.002-(0.018*rul44);
        //deror neg kecil maka du pos sedang
        u=PB;
        v=DNK;
        mamdani();
        rul45=out;
        z45a=0.002+(0.018*rul45);
        z45b=0.15-(0.13*rul45);
        //deror nol maka du pos besar
        u=PB;
        v=DNOL;
        mamdani();
        rul46=out;
        z46=0.02+(0.13*rul46);       
        //deror pos kecil maka du pos besar
        u=PB;
        v=DPK;
        mamdani();
        rul47=out;
        z47=0.02+(0.13*rul47);
        //deror pos sedang maka du pos besar
        u=PB;
        v=DPS;
        mamdani();
        rul48=out;
        z48=0.02+(0.13*rul48);
        //deror pos besar maka du pos besar
        u=PB;
        v=DPB;
        mamdani();
        rul49=out;
        z49=0.02+(0.13*rul49);

    //defuzifikasi metode COA
    A=(rul1*z1)+(rul2*z2)+(rul3*z3)+(rul4*z4)+(rul5*z5a)+(rul5*z5b)+(rul6*z6a)+(rul6*z6b)+(rul7*z7a)+(rul7*z7b)+(rul8*z8)+(rul9*z9)+(rul10*z10)+(rul11*z11a)+(rul11*z11b)+(rul12*z12a)+(rul12*z12b)+(rul13*z13a)+(rul13*z13b)+(rul14*z14a)+(rul14*z14b)+(rul15*z15)+(rul16*z16)+(rul17*z17a)+(rul17*z17b)+(rul18*z18a)+(rul18*z18b)+(rul19*z19a)+(rul19*z19b)+(rul20*z20a)+(rul20*z20b)+(rul21*z21a)+(rul21*z21b)+(rul22*z22)+(rul23*z23a)+(rul23*z23b)+(rul24*z24a)+(rul24*z24b)+(rul25*z25a)+(rul25*z25b)+(rul26*z26a)+(rul26*z26b)+(rul27*z27a)+(rul27*z27b)+(rul28*z28)+(rul29*z29a)+(rul29*z29b)+(rul30*z30a)+(rul30*z30b)+(rul31*z31a)+(rul31*z31b)+(rul32*z32a)+(rul32*z32b)+(rul33*z33a)+(rul33*z33b)+(rul34*z34)+(rul35*z35)+(rul36*z36a)+(rul36*z36b)+(rul37*z37a)+(rul37*z37b)+(rul38*z38a)+(rul38*z38b)+(rul39*z39a)+(rul39*z39b)+(rul40*z40)+(rul41*z41)+(rul42*z42)+(rul43*z43a)+(rul43*z43b)+(rul44*z44a)+(rul44*z44b)+(rul45*z45a)+(rul45*z45b)+(rul46*z46)+(rul47*z47)+(rul48*z48)+(rul49*z49);
    
    B=(rul1+rul2+rul3+rul4+rul5+rul6+rul7+rul8+rul9+rul10+rul11+rul12+rul13+rul14+rul15+rul16+rul17+rul19+rul20+rul21+rul22+rul23+rul24+rul25+rul26+rul27+rul28+rul29+rul30+rul31+rul32+rul33+rul34+rul35+rul36+rul37+rul38+rul39+rul40+rul41+rul42+rul43+rul44+rul45+rul46+rul47+rul48+rul49);
    
    du=A/B;
}


void loop() {
//  Sensortegangan
  value=analogRead(SensorTegangan);
  Vmodul = (value * 5.0) / 1024.0;
  hasil = Vmodul / (R2/(R1+R2));
//  Serial.print ("Tegangan: ");
//  Serial.println (hasil);
   
fuzy_rule ();
duty=lsduty+du;
lsduty=duty;
    if(duty>=0.50){duty=0.50;}
    if(duty<=0.05){duty=0.05;}
 Serial.println(lsduty);
//  byte dt=duty*225;

//boost
int temp_cycles =170; //frekuensi
int temp_duty =duty; //dutycycle
 if (temp_cycles != setCycles || temp_duty != setDuty){ 
  setCycles = temp_cycles;
  setDuty = temp_duty; 
  set(setCycles, setDuty / 1023.0);
  // put your main code here, to run repeatedly:
}
//  Serial.print("dutyCycle : ");
//  Serial.println(temp_duty / 1024.0);
}


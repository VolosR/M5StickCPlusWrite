#include "M5StickCPlus.h"
#include "Wire.h"
#include "Tone32.h"
#define JOY_ADDR 0x38
int8_t xj,yj,bj;

#define color1 0x026A
#define color2 0xBAC0

#define BUZZER_PIN 2
#define BUZZER_CHANNEL 0

void setup()
{
  pinMode(37,INPUT_PULLUP);
  pinMode(2,OUTPUT);
  M5.begin();
  M5.Axp.ScreenBreath(10);
  Wire.begin(0, 26, 100000);
  M5.Lcd.setRotation(2);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setSwapBytes(true);
  M5.Lcd.setTextWrap(true);
  initDraw();
}

int n=4;
int m=3;
int posX[12];
int posY[12];

int v=0;
int h=0;

int v1=0;
int h1=0;

int fromTop=96;
int fromLeft=5;
int boxW=39;
int boxH=32;
int space=3;
char buttons[4][3]={{'1','2','3'},{'4','5','6'},{'7','8','9'},{'*','0','#'}};
String letters[4][3]={{" ","abc","def"},{"ghi","jkl","mno"},{"pqrs","tuv","wxyz"},{"","",""}};
String temText="";
String text="";

int pres=0;
int pres2=0;
int presed=0;
bool started=0;

long curentTime=0;

void loop()
{

  checkButtons();
  drawText();

  if(started==1)
  if(millis()>curentTime+5000){
  started=0;
  }
}

void initDraw()
{
  M5.Lcd.setTextColor(WHITE);
  for(int i=0;i<n;i++){
  posY[i]=fromTop+(boxH*i)+(space*i);
  for(int j=0;j<m;j++)
    {
      posX[j]=fromLeft+(boxW*j)+(space*j);
      M5.Lcd.fillRoundRect(posX[j],posY[i],boxW,boxH,3,color1);
      M5.Lcd.drawString(String(buttons[i][j]),posX[j]+4,posY[i]+2,2);
       M5.Lcd.drawString(letters[i][j],posX[j]+4,posY[i]+20,1);
    }}
    M5.Lcd.fillRoundRect(posX[0],posY[0],boxW,boxH,3,color2);
    M5.Lcd.drawString(String(buttons[0][0]),posX[0]+4,posY[0]+2,2);
}

void checkButtons()
{
    M5.Lcd.setTextColor(WHITE);
    check();

    if(xj<-50){h1++; if(h1==3) h1=0;}
    if(xj>50){h1--; if(h1==-1) h1=2;}

     if(yj>50){v1++; if(v1==4) v1=0;}
    if(yj<-50){v1--; if(v1==-1) v1=3;}
   
    
   
    if(v!=v1  || h!=h1)
    {
      presed=0;
      started=0;
      M5.Lcd.fillRoundRect(posX[h],posY[v],boxW,boxH,3,color1);
      M5.Lcd.drawString(String(buttons[v][h]),posX[h]+4,posY[v]+2,2);
      M5.Lcd.drawString(letters[v][h],posX[h]+4,posY[v]+20,1);
       v=v1;
       h=h1;
      M5.Lcd.fillRoundRect(posX[h],posY[v],boxW,boxH,3,color2);
      M5.Lcd.drawString(String(buttons[v][h]),posX[h]+4,posY[v]+2,2);
      M5.Lcd.drawString(letters[v][h],posX[h]+4,posY[v]+20,1);
       tone(BUZZER_PIN, NOTE_C4, 80, BUZZER_CHANNEL);
      noTone(BUZZER_PIN, BUZZER_CHANNEL);
      delay(100);
    }

  
  if(digitalRead(37)==0)
  {
    if(pres==0)
    temText="";
    M5.Lcd.fillRect(0,0,128,50,WHITE);
  }else{pres=0;}



  if(bj==0)
    {
      
      if(pres2==0){
        pres2=1;
        M5.Lcd.fillRect(0,0,135,40,WHITE);
      if(started==1)
        {
         
         presed++;
         if(presed==letters[v][h].length())
         presed=0;
         if(v==0 && h==0)
         temText.setCharAt(temText.length()-1,' ');
         else
         temText.setCharAt(temText.length()-1,letters[v][h].charAt(presed));
        }
      
      if(started==0)
      {
        curentTime=millis();
        started=1;
        temText=temText+String(letters[v][h].charAt(presed));
      }
      }  tone(BUZZER_PIN, NOTE_C6, 80, BUZZER_CHANNEL);
      noTone(BUZZER_PIN, BUZZER_CHANNEL);}else{pres2=0;}
  }

  void drawText()
  {
    M5.Lcd.setTextColor(BLACK,WHITE);
    if(temText!=text){
    M5.Lcd.setCursor(5,5,4);
    M5.Lcd.print(temText);
    text=temText;
  }
    
  }

  void check()
{
  Wire.beginTransmission(JOY_ADDR);
  Wire.write(0x02); 
  Wire.endTransmission();
  Wire.requestFrom(JOY_ADDR, 3);
  if (Wire.available()) {
    xj = Wire.read();
    yj = Wire.read();
    bj= Wire.read();
}  }

#include <LiquidCrystal.h>
#include <DHT.h>
#include <DHT_U.h>

#define IRRIGATION_TIME 5000
#define DHTPIN A0
#define DHTTYPE DHT11 // DHT 11
#define PIN_CHUVA A4
#define PIN_RELE 8
#define PIN_UMIDADE1 A3
#define PIN_UMIDADE2 A2
#define PIN_UMIDADE3 A1

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
DHT dht(DHTPIN, DHTTYPE);

float h;
float t;
float chuva;
float umidade1;
float umidade2;
float umidade3;

void setup() {
  Serial.begin(9600);
  
  lcd.begin(20, 4);
  lcd.clear();

  dht.begin();

  pinMode(PIN_CHUVA, INPUT);
  pinMode(PIN_UMIDADE1, INPUT);
  pinMode(PIN_UMIDADE2, INPUT);
  pinMode(PIN_UMIDADE3, INPUT);
  pinMode(PIN_RELE, OUTPUT);

  digitalWrite(PIN_RELE, HIGH);
}

void loop() {
  readdht();
  readchuva();
  readumidade();
  //irrigate();

  disp();

  delay(600000);
}


void readdht()
{
  float th = dht.readHumidity();
  float tt = dht.readTemperature();

  if (isnan(tt))
  {
    Serial.println("Erro ao ler a temperatura");
  }
  else
  {
    t = tt;
  }

  if (isnan(th))
  {
    Serial.println("Erro ao ler a umidade");
  }
  else
  {
    h = th;
  }
}

void readchuva()
{
  int sensorchuva = analogRead(PIN_CHUVA);
  int delta = 1023 - sensorchuva;
  chuva = (float)delta / 800;
}

void readumidade()
{
  int t = analogRead(PIN_UMIDADE1);
  umidade1 = percUmidade(t);

  t = analogRead(PIN_UMIDADE2);
  umidade2 = percUmidade(t);

  t = analogRead(PIN_UMIDADE3);
  umidade3 = percUmidade(t);
}

float percUmidade(int aUmidade) {
  int delta = 1023 - aUmidade; 
  float dUmidade = (float)delta / 808;
  if(dUmidade > 1)
    dUmidade = (float)1;
  dUmidade = dUmidade * (float)100;

  return dUmidade;
}

void disp()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(t);


  lcd.setCursor(0,1);
  lcd.print("Umid: ");
  lcd.print(h);


  lcd.setCursor(0,2);
  lcd.print("Chuva: ");
  if (chuva < (float)0.1)
    lcd.print("NAO");
  else if (chuva < (float)0.25)
    lcd.print("LEVE");
  else if (chuva < (float)0.75)
    lcd.print("MODERADA");
  else
    lcd.print("FORTE");

  lcd.setCursor(0,3);
  lcd.print("Solo: ");
  lcd.print(umidade1);
  lcd.print("/");
  lcd.print(umidade2);
  lcd.print("/");
  lcd.print(umidade3);
  lcd.print("/");
}

void irrigate()
{
  digitalWrite(PIN_RELE, LOW);
  delay(IRRIGATION_TIME);
  digitalWrite(PIN_RELE, HIGH);

}


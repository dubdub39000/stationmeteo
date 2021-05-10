#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ImageReader.h>
#include "SparkFunBME280.h"
#include <SoftwareSerial.h>

// Declare pins for the display:
#define TFT_CS     10 //CS du TFT
#define TFT_RST    9
#define TFT_DC     8
#define SD_CS   6   //CS de la SD
#define SD_MISO 12
#define SD_MOSI 11
#define SD_SCK  13
SoftwareSerial softSerial(2, 3); // RX, TX
// les autres pins sont prédéfinies (SCK = 13 and SDA = 11)   
///////////////////paramètre réseau//////////////
String host = "192.168.104.183";
//String host = "192.168.104.160"; // IP second groupe pour test
String SSID = "weshalors";  // SSID du réseau Wi-Fi
//String SSID = "palu";
String PASS = "CfaiSN2oue"; // Mot de passe Wi-Fi
//String PASS = "+menfouu";
////////////////////////////////////////////////
// Creation de l'écran
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
//SdFat SD;         // Gestion de la SD CARD
//Adafruit_ImageReader reader(SD); //lecteur de fichier
BME280 mySensor;
String url_Humidity;
String url_pressure;
String url_temp;

void setup()
{
    
  softSerial.begin(115200);
    delay(100);
    Serial.begin(9600);
        //////////////////////init capteur////////////////////////////
  Wire.begin();
  mySensor.setI2CAddress(0x76); 
   if (mySensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while (1); //Freeze
  }
  else {
    Serial.println("connexion capteur OK");
  }

    //softSerial.begin(9600);
    //initSDCard();
    connexion(); //se connecte à la base de donnée
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(2);
    //Serial.println("Initializing SD card...");
  //if (!SD.begin(SD_CS)) {
    //Serial.println("failed!");
    //return;
  //}
    //ImageReturnCode stat; 
    //stat = reader.drawBMP("cat24.bmp", tft, 0, 0); 
    //delay(5000);
    


}  // End of setup

void initSDCard(){
}


void affichagevaleur(int nbr) {
  tft.setTextSize(2);
  switch (nbr) {
    case 1:
      tft.setTextColor(ST77XX_RED);
      tft.setCursor(0, 0);
      tft.print("Temp");
      ////////////////////////////////:
      tft.setCursor(50, 20); //50 ,50 représente le milieu de l'écran
      url_temp = String(mySensor.readTempC());
      tft.print(url_temp);
      break;
    case 2:
      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(0, 50);
      tft.print("Pressure");
  /////////////////////////////////////:
      tft.setCursor(30, 70); //50 ,50 représente le milieu de l'écran
      url_pressure = String(mySensor.readFloatPressure()/100);
      tft.print(url_pressure);
      break;
    case 3:
      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(0, 100);
      tft.print("Humidity");
      ///////////////////////////////////
      tft.setCursor(50, 120); //50 ,50 représente le milieu de l'écran
      url_Humidity = String(mySensor.readFloatHumidity());
      tft.print(url_Humidity);
      break;
  }
}

void loop()
{
tft.fillScreen(ST77XX_BLACK);
  affichagevaleur(1);
  affichagevaleur(2);
  affichagevaleur(3);
  String url = "?temp="+url_temp+"&pressure="+url_pressure+"&humidity="+url_Humidity;
  //String url = "?temp="+url_temp+"&press="+url_pressure+"&alt="+url_Humidity;
  sendValueUrl(url);
  delay(15000);
} 

void sendValueUrl(String value) { 
    // mode "connexions multiples"
  softSerial.println("AT+CIPMUX=1");
  delay(3000);
  // on se connecte à notre serveur en TCP sur le port 80
  softSerial.println("AT+CIPSTART=4,\"TCP\",\""+host+"\",80");
  delay(1000);
  String request = "GET /meteo/exe.php"+value+" HTTP/1.0\r\n";
  //String request = "GET /meteo/insertion.php"+value+" HTTP/1.0\r\n";
  // chemin de et nom du fichier qui recevra les données
  request       += "Host:"+host+"\r\n";
  // on donne la taille de la requête qu'on va envoyer, en ajoutant 2 car
  // println ajouter 2 caractères à la fin "\r" et "\n"
  softSerial.println("AT+CIPSEND=4,"+String(request.length()+2));
  delay(500);
  // on envoie la requete
  softSerial.println(request);
  delay(3000);
  softSerial.println("AT+CIPCLOSE=4");// je ferme toutes connections eventuelles
}

void connexion() {
 // on va se connecter à un réseau existant, donc on passe en mode station
softSerial.println("AT+CWMODE=1");
receiveValue(1000);
  delay(1000);
  // on se connecte au réseau
 softSerial.println("AT+CWJAP=\""+SSID+"\",\""+PASS+"\"");
 receiveValue(1000);
 delay(10000);
}

void receiveValue(const int timeout)
{
  String reponse= "";
  long int time = millis();
  while((time+timeout)>millis())
  {
    while(softSerial.available())
    {
      char c = softSerial.read();
      reponse+=c;
    }
  }
  Serial.print(reponse);
}
    
  

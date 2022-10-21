#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>

#include <RTClib.h>
#include <SD.h>
#include <SoftwareSerial.h>
RTC_DS1307 rtc;

String dossier;
String fichier;
String path;

String datas = "coucou les copains";

int chipSelect = 4;
int nombre_fichier = 1;
int nombre_dossier = 1;

bool recherche_fichier = true;

void setup()
{
    Serial.begin(9600);
    // Attente de la connection serie avec l'Arduino

    while (!Serial)
        ;
    // Lance le communication I2C avec le module RTC et
    // attend que la connection soit operationelle
    while (!rtc.begin())
    {
        Serial.println("Attente du module RTC...");
        delay(1000);
    }
    // Mise à jour de l'horloge du module RTC avec la date et
    // l'heure courante au moment de la compilation de ce croquis
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    Serial.println("Horloge du module RTC mise a jour");

    while (!Serial)
        ;
    // Lance le communication I2C avec le module RTC et
    // attend que la connection soit operationelle
    if (!SD.begin(chipSelect))
    {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        while (1)
            ;
    }
    path_depart();
}

String renvoieDate()
{

    DateTime now = rtc.now();
    char heure[10];

    String year = String(now.year());
    year = year.substring(2);

    String month = String(now.month());

    String day = String(now.second());

    String date = year + month + day;

    return date;
}

void loop()
{
    // put your main code here, to run repeatedly:
    enregistrement(datas);
    delay(1000);
}

void enregistrement(String data)
{

    changement_dossier();

    path = dossier + "/" + "221910_0.log";

    Serial.print("Path : ");
    Serial.println(path);

    // Serial.println(fichier);
    File dataFile = SD.open(path, FILE_WRITE);

    if (dataFile)
    {

        Serial.println(dataFile.size());
        if (dataFile.size() >= 40)
        {

            archiver();

            nv_fichier();

            Serial.print("Nouveau fichier créé : ");
            Serial.println(path);
        }

        dataFile.println(data);
        // Serial.println("Ecrire dans la carte SD");
        dataFile.close();
    }
}

void nv_fichier()
{

    // Serial.println("Nouveau fichier");
    int year = 22;
    int month = 10;
    int day = 18;
    String strYear = String(year);
    String strMonth = String(month);
    String StrDay = String(day);

    String date = strYear + strMonth + StrDay;
    fichier = date + "_0.log";
    nombre_fichier += 1;
    if (nombre_fichier == 10)
    {

        nombre_dossier += 1;
        archiver();
        dossier.replace("_" + String(nombre_dossier - 1), "_" + String(nombre_dossier));

        nombre_fichier = 1;
    }
}
void archiver()
{

    String fichier_archive = fichier;
    fichier_archive.replace("_0", "_" + String(nombre_fichier));

    String path_archive = dossier + "/" + fichier_archive;

    Serial.print("Nouvelle archive créée : ");
    Serial.println(path_archive);
    File dataFile1 = SD.open(path_archive, FILE_WRITE);
    File dataFile2 = SD.open(path, FILE_READ);
    while (dataFile2.available())
    {
        dataFile1.write(dataFile2.read());
    }
    dataFile1.close();
    dataFile2.close();
    SD.remove(path);
}

void changement_dossier()
{
    if (!SD.exists(dossier))
    {
        Serial.println("Création dossier");
        SD.mkdir(dossier);
    }
}

void path_depart()
{
    String doss_suivant = "doss_1";
    doss_suivant.replace("_" + String(nombre_dossier), "_" + String(nombre_dossier + 1));
    while (SD.exists(doss_suivant))
    {
        nombre_dossier += 1;
        doss_suivant.replace("_" + String(nombre_dossier), "_" + String(nombre_dossier + 1));
    }
    dossier = "doss_" + String(nombre_dossier);

    File dataFile = SD.open(dossier + "/");
    File testFile = dataFile.openNextFile();
    while (testFile)
    {
        nombre_fichier++;
        testFile = dataFile.openNextFile();
    }

    fichier = "221910_" + String(nombre_fichier) + ".log";

    Serial.print("dossier : ");
    Serial.println(nombre_dossier);
    Serial.print("fichier : ");
    Serial.println(nombre_fichier);
    dataFile.close();
}
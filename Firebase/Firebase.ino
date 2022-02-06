#include "Firebase_Arduino_WiFiNINA.h"

#define FIREBASE_HOST "hackviolet-2022-38530-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "kE0MkoA3j8K7ocesW4SH9rRJ3hLcYEojU1pmiCnu"
#define WIFI_SSID "Test"
#define WIFI_PASSWORD "12345678"

//Define Firebase data object
FirebaseData firebaseData;

void setup()
{

  Serial.begin(115200);
 
  //Conexin WIFI en modo cliente
  //------------------------------

  Serial.print("Conectando al Wi-Fi");
  
  int status = WL_IDLE_STATUS;
  
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("conectado con esta IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println("------------------------------------------------------------------------------------");
  
  //--------------------------------------------------------------------------

  //Conexin a Firebase
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true); //Le decimos que si se cae la conexin trate de reconectarse manera automtica

  String path = "/Test_Burgo"; //Path en la Base de Datos. Podemos colgar de la raz
  String jsonStr;
  
  //--------------------------  INSERTAMOS Y LEEMOS UN NUMERO ENTERO -----------------------------

  Serial.println("Insertamos un campo con un valor de un entero");
  
   if (Firebase.setInt(firebaseData, path + "/Int/Data1",123))
    {
      Serial.println("Insertado");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "int") // se hace para asegurar que si el tipo almacenado es entero cogemso el valor correctamente y no basura
        Serial.println(firebaseData.intData()); //Realmente estamos leyendo con esto el valor introducido
    }
    else
    {
      Serial.println("ERROR : " + firebaseData.errorReason());
      Serial.println();
    }
  
  
   Serial.println("Leemos el valor insertado");
  
   if (Firebase.getInt(firebaseData, path + "/Int/Data1"))
    {
      Serial.println("Resultado de la lectura");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "int")
        Serial.println(firebaseData.intData()); //Aqu tenemos el valor ledo
    }
    else
    {
      Serial.println("ERROR: " + firebaseData.errorReason());
      Serial.println();
    }
    Serial.println("------------------------------------------------------------------------------------");
  
  //--------------------------  INSERTAMOS Y LEEMOS UN STRING -----------------------------
  

 Serial.println("Insertamos un campo con un valor de un string");
  
   if (Firebase.setString(firebaseData, path + "/String/texto1","Hola FireBase"))
    {
      Serial.println("Insertado");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "string") // se hace para asegurar que si el tipo almacenado es string cogemos el valor correctamente y no basura
        Serial.println(firebaseData.stringData()); //Realmente estamos leyendo con esto el valor introducido. El payload tiene el valor
    }
    else
    {
      Serial.println("ERROR : " + firebaseData.errorReason());
      Serial.println();
    }
    
 Serial.println("Leemos el valor insertado");
  
   if (Firebase.getString(firebaseData, path + "/String/texto1"))
    {
      Serial.println("Resultado de la lectura");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "string")
        Serial.println(firebaseData.stringData()); //Aqu tenemos el valor ledo
    }
    else
    {
      Serial.println("ERROR: " + firebaseData.errorReason());
      Serial.println();
    }

    Serial.println("------------------------------------------------------------------------------------");
//--------------------------  INSERTAMOS Y LEEMOS UN JSON -----------------------------


Serial.println("Insertamos un json con un valor de un string, y un float en otra estructura ");

//De un web que me lo escapa y lo pone OK
jsonStr="{\n  \"employees\": {\n    \"employee\": [\n      {\n        \"id\": \"1\",\n        \"firstName\": \"Oscar\",\n        \"lastName\": \"Per\",\n        \"photo\": \"https://pbs.twimg.com/profile_images/735509975649378305/B81JwLT7.jpg\"\n      },\n      {\n        \"id\": \"2\",\n        \"firstName\": \"Maria\",\n        \"lastName\": \"Sharapova\",\n        \"photo\": \"https://pbs.twimg.com/profile_images/3424509849/bfa1b9121afc39d1dcdb53cfc423bf12.jpeg\"\n      },\n      {\n        \"id\": \"3\",\n        \"firstName\": \"Pedro\",\n        \"lastName\": \"Bond\",\n        \"photo\": \"https://pbs.twimg.com/profile_images/664886718559076352/M00cOLrh.jpg\"\n      }\n    ]\n  }\n}";
   if (Firebase.setJSON(firebaseData, path + "/JSON",jsonStr))
    {
      Serial.println("Insertado");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "json") // se hace para asegurar que si el tipo almacenado es string cogemos el valor correctamente y no basura
        Serial.println(firebaseData.jsonData()); //Realmente estamos leyendo con esto el valor introducido. El payload tiene el valor
    }
    else
    {
      Serial.println("ERROR : " + firebaseData.errorReason());
      Serial.println();
    }
    
 Serial.println("Leemos el valor JSON");
  
   if (Firebase.getJSON(firebaseData, path + "/JSON/employees/employee/1"))
    {
      Serial.println("Resultado de la lectura");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "json")
        Serial.println(firebaseData.jsonData()); //Aqu tenemos el valor ledo
    }
    else
    {
      Serial.println("ERROR: " + firebaseData.errorReason());
      Serial.println();
    }

  Serial.println("------------------------------------------------------------------------------------");


//--------------------------- REALIZAMOS UN PUSH DE UN INT --------------------------------------

  Serial.println("Realizamos un push a la BBDD como entero.Esto implica insertar un elemento entero pero la BBDD rellena el nombre.Util para logs");
  
    if (Firebase.pushInt(firebaseData, path + "/Push/Int", 6754))
    {
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.print("PUSH NAME: ");
      Serial.println(firebaseData.pushName());
    }
    else
    {
      Serial.println("ERROR: " + firebaseData.errorReason());
    }
  
  Serial.println("------------------------------------------------------------------------------------");
  
   
  //--------------------------- REALIZAMOS UN PUSH DE un JSON  --------------------------------------
  
  
  Serial.println("Realizamos un push a la BBDD desde un JSON. Mismo que antes pero ahora metes un json que puede tener muchos campos");
  
  
  for (uint8_t i = 0; i < 3; i++)
  {

    jsonStr = "{\"Data" + String(i + 1) + "\":" + String(i + 1) + "}";
    

    if (Firebase.pushJSON(firebaseData, path + "/Push/Json", jsonStr))
    {
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.print("PUSH NAME: ");
      Serial.println(firebaseData.pushName());
    }
    else
    {
      Serial.println("ERROR: " + firebaseData.errorReason());
    }
  }
    Serial.println("------------------------------------------------------------------------------------");

  //--------------------------- REALIZAMOS UN UPDATE --------------------------------------

  for (uint8_t i = 3; i < 6; i++)
  {

    jsonStr = "{\"Data" + String(i + 1) + "\":" + String(i + 5.5) + "}";

    if (Firebase.updateNode(firebaseData, path + "/Int", jsonStr))
    {
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      if (firebaseData.dataType() == "json")
        Serial.println(firebaseData.jsonData());
     
    }
    else
    { 
      Serial.println("ERROR: " + firebaseData.errorReason());
    }
  }

 //--------------------------- REALIZAMOS UN DELETE--------------------------------------

delay(15000);
Firebase.deleteNode(firebaseData, path);
 
}

void loop()
{
}

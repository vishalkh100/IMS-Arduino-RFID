/*
 * Initial Author: ryand1011 (https://github.com/ryand1011)
 *
 * Reads data written by a program such as "rfid_write_personal_data.ino"
 *
 * See: https://github.com/miguelbalboa/rfid/tree/master/examples/rfid_write_personal_data
 *
 * Uses MIFARE RFID card using RFID-RC522 reader
 * Uses MFRC522 - Library
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/

#include <SPI.h>
#include <MFRC522.h>

//#include <SoftwareSerial.h>
//SoftwareSerial BTSerial(1, 0);


constexpr uint8_t RST_PIN = 4;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//*****************************************************************************************//

void setup()
{
  Serial.begin(115200);
//  BTSerial.begin(38400);
  SPI.begin();
  mfrc522.PCD_Init();  
}

//*****************************************************************************************//

void loop() 
{

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  byte buffer2[18];
  block = 1;

  //byte size_  = sizeof(buffer2);

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) 
  {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) 
  {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

//  for(i=0; i<3; i++)
//  {
//    Serial.write(buffer2[i]);
//  }

    //PRINT DATA TO SERIAL
  
    if(buffer2[0]=='0')
    {
      Serial.write("core");
      Serial.write(",");
    }
    else if(buffer2[0]=='1')
    {
      Serial.write("summer");
      Serial.write(",");
    }
    else if(buffer2[0]=='2')
    {
      Serial.write("winter");
      Serial.write(",");
    }


    if(buffer2[1]=='1' && buffer2[2]=='5')
    {
      Serial.write("2015");
      Serial.write(";");
    }
    else if(buffer2[1]=='1' && buffer2[2]=='6')
    {
      Serial.write("2016");
      Serial.write(";");
    }
    else if(buffer2[1]=='1' && buffer2[2]=='7')
    {
      Serial.write("2017");
      Serial.write(";");
    }
    else if(buffer2[1]=='1' && buffer2[2]=='8')
    {
      Serial.write("2018");
      Serial.write(";");
    }

  delay(1000); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
}
//*****************************************************************************************//

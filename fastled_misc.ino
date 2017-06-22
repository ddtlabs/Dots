void loadSettings()
{
  brightness = EEPROM.read(EE_brightness);
  power      = EEPROM.read(EE_power);

  currentPatternIndex = EEPROM.read(EE_currentPatternIndex);
  if (currentPatternIndex < 0)
    currentPatternIndex = 0;
  else if (currentPatternIndex >= patternCount)
    currentPatternIndex = patternCount - 1;

  byte r = EEPROM.read(EE_r);
  byte g = EEPROM.read(EE_g);
  byte b = EEPROM.read(EE_b);

  if (r == 0 && g == 0 && b == 0)
  {
  }
  else
  {
    solidColor = CRGB(r, g, b);
  }

  // Fire2012
  byte t_COOLING  = EEPROM.read(EE_COOLING);
  byte t_SPARKING = EEPROM.read(EE_SPARKING);
  if (t_COOLING != 255) COOLING = t_COOLING;
  if (t_SPARKING != 255) SPARKING = t_SPARKING;

  if (debug) {
    Serial.print( F("Fire sparking: ") ); Serial.println(SPARKING);
    Serial.print( F("Fire cooling: ") );  Serial.println(COOLING);
  }
}


void Show_Environment()
{
  Serial.println();
  Serial.print( F("Heap: ") ); Serial.println(system_get_free_heap_size());
  Serial.print( F("Boot Vers: ") ); Serial.println(system_get_boot_version());
  Serial.print( F("CPU: ") ); Serial.println(system_get_cpu_freq());
  Serial.print( F("SDK: ") ); Serial.println(system_get_sdk_version());
  Serial.print( F("Chip ID: ") ); Serial.println(system_get_chip_id());
  Serial.print( F("Flash ID: ") ); Serial.println(spi_flash_get_id());
  Serial.print( F("Flash Size: ") ); Serial.println(ESP.getFlashChipRealSize());
  Serial.print( F("Vcc: ") ); Serial.println(ESP.getVcc());
  Serial.println();

  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), String(fileSize).c_str());
    }
    Serial.printf("\n");
  }
}


String parseString(String& string, byte indexFind)
{
  String tmpString = string;
  tmpString += ",";
  tmpString.replace(" ", ",");
  String locateString = "";
  byte count = 0;
  int index = tmpString.indexOf(',');
  while (index > 0)
  {
    count++;
    locateString = tmpString.substring(0, index);
    tmpString = tmpString.substring(index + 1);
    index = tmpString.indexOf(',');
    if (count == indexFind)
    {
      locateString.toLowerCase();
      return locateString;
    }
  }
  return "";
}




// ---------------------------------------------------------------------------------
// ------------------------------ rgbStr2Num ---------------------------------------
// ---------------------------------------------------------------------------------
int32_t rgbStr2Num(String rgbStr)
{
  int32_t rgbDec = (int) strtol( &rgbStr[0], NULL, 16);
  return rgbDec;
}


/********************************************************************************************\
  Convert a char string to integer
\********************************************************************************************/
unsigned long str2int(char *string)
{
  unsigned long temp = atof(string);
  return temp;
}



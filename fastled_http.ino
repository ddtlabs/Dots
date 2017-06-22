void handle_control()
{
  String webrequest = server.arg("cmd");
  String cmd     = parseString(webrequest, 1);
  String subcmd  = parseString(webrequest, 2);
  String p1  = parseString(webrequest, 3);
  String p2  = parseString(webrequest, 4);
  String p3  = parseString(webrequest, 5);

  if (debug) {
    Serial.print("Web control: command:"); Serial.print(cmd);
    Serial.print(" subcmd:"); Serial.print(subcmd); Serial.print(" p1:"); Serial.print(p1);
    Serial.print(" p2:");     Serial.print(p2);     Serial.print(" p3:"); Serial.println(p3);
  }

  if (cmd == F("dots") ) {

    if (subcmd == F("power") ) {
      if (p1 == F("on") || p1.toInt() == 1) setPower(1);
      else setPower(0);
      sendPower();
    }
    else if (subcmd == F("on") ) {
      setPower(1);
      sendPower();
    }
    else if (subcmd == F("off") ) {
      setPower(0);
      sendPower();
    }
    else if (subcmd == F("toggle") ) {
      if (power) setPower(0);
      else       setPower(1);
      sendPower();
    }

    else if (subcmd == F("solidcolor") || subcmd == F("rgb") ) {
      if (p1 != "") {
        int32_t rgbDec = rgbStr2Num(p1);
        int r = (rgbDec >> 16);
        int g = (rgbDec >> 8 & 0xFF);
        int b = (rgbDec & 0xFF);
        setSolidColor(r, g, b);
        sendSolidColor();
      }
    }

    else if (subcmd == F("pattern") || subcmd == F("effect") ) {
      if (p1 != "") {
        setPattern(p1.toInt());
        sendPattern();
      }
    }

    else if (subcmd == F("firecooling") ) {
      if (p1 != "") {
        COOLING = p1.toInt();
        if (COOLING > 254) COOLING = 254;
        EEPROM.write(5, COOLING);
        EEPROM.commit();
      }
    }

    else if (subcmd == F("firesparking") ) {
      if (p1 != "") {
        SPARKING = p1.toInt();
        if (SPARKING > 254) SPARKING = 254;
        EEPROM.write(6, SPARKING);
        EEPROM.commit();
      }
    }

    else if (subcmd == F("patternup") || subcmd == F("next") ) {
      adjustPattern(true);
      sendPattern();
    }

    else if (subcmd == F("patterndown") || subcmd == F("prev")) {
      adjustPattern(false);
      sendPattern();
    }

    else if (subcmd == F("brightness") || subcmd == F("bri") ) {
      setBrightness(p1.toInt());
      sendBrightness();
    }

    else if (subcmd == F("pct")) {
      int pct = p1.toInt();
      int bri = map(pct, 0, 100, 0, 255);
      setBrightness(bri);
      sendBrightness();
    }

    else if (subcmd == F("brightnessup") || subcmd == F("dimup") ) {
      adjustBrightness(true);
      sendBrightness();
    }

    else if (subcmd == F("brightnessdown") || subcmd == F("dimdown")) {
      adjustBrightness(false);
      sendBrightness();
    }

    else {
      Serial.print("Web control: unknown command!");
    }

  } // dots

  server.send(200, "text/html", "OK");
}


void Webserver_Start()
{
  //server.serveStatic("/", SPIFFS, "/index.htm"); // ,"max-age=86400"

  server.on("/control", HTTP_GET, []() {
    handle_control();
  });

  server.on("/all", HTTP_GET, []() {
    sendAll();
  });

  server.on("/power", HTTP_GET, []() {
    String value = server.arg("value");
    if (value != "") setPower(value.toInt());
    HTTPsend();
    sendPower();
  });

  server.on("/power", HTTP_POST, []() {
    String value = server.arg("value");
    setPower(value.toInt());
    HTTPsend();
    sendPower();
  });

  server.on("/solidColor", HTTP_GET, []() {
    sendSolidColor();
  });

  server.on("/solidColor", HTTP_POST, []() {
    String r = server.arg("r");
    String g = server.arg("g");
    String b = server.arg("b");
    setSolidColor(r.toInt(), g.toInt(), b.toInt());
    sendSolidColor();
  });

  server.on("/pattern", HTTP_GET, []() {
    sendPattern();
  });

  server.on("/pattern", HTTP_POST, []() {
    String value = server.arg("value");
    setPattern(value.toInt());
    sendPattern();
  });

  server.on("/patternUp", HTTP_POST, []() {
    adjustPattern(true);
    sendPattern();
  });

  server.on("/patternDown", HTTP_POST, []() {
    adjustPattern(false);
    sendPattern();
  });

  server.on("/brightness", HTTP_GET, []() {
    sendBrightness();
  });

  server.on("/brightness", HTTP_POST, []() {
    String value = server.arg("value");
    setBrightness(value.toInt());
    sendBrightness();
  });

  server.on("/brightnessUp", HTTP_POST, []() {
    adjustBrightness(true);
    sendBrightness();
  });

  server.on("/brightnessDown", HTTP_POST, []() {
    adjustBrightness(false);
    sendBrightness();
  });

  server.serveStatic("/index.htm", SPIFFS, "/index.htm");
  server.serveStatic("/fonts", SPIFFS, "/fonts", "max-age=86400");
  server.serveStatic("/js", SPIFFS, "/js");
  server.serveStatic("/css", SPIFFS, "/css", "max-age=86400");
  server.serveStatic("/images", SPIFFS, "/images", "max-age=86400");
  server.serveStatic("/", SPIFFS, "/index.htm");

  server.begin();

  Serial.println("HTTP server started");
}


void sendAll()
{
  String json = "{";

  json += "\"power\":" + String(power) + ",";
  json += "\"brightness\":" + String(brightness) + ",";

  json += "\"currentPattern\":{";
  json += "\"index\":" + String(currentPatternIndex);
  json += ",\"name\":\"" + patterns[currentPatternIndex].name + "\"}";

  json += ",\"solidColor\":{";
  json += "\"r\":" + String(solidColor.r);
  json += ",\"g\":" + String(solidColor.g);
  json += ",\"b\":" + String(solidColor.b);
  json += "}";

  json += ",\"patterns\":[";
  for (uint8_t i = 0; i < patternCount; i++)
  {
    json += "\"" + patterns[i].name + "\"";
    if (i < patternCount - 1)
      json += ",";
  }
  json += "]";

  json += "}";

  server.send(200, "text/json", json);
  json = String();
}

void sendPower()
{
  String json = String(power);
  server.send(200, "text/json", json);
  json = String();
}

void sendPattern()
{
  String json = "{";
  json += "\"index\":" + String(currentPatternIndex);
  json += ",\"name\":\"" + patterns[currentPatternIndex].name + "\"";
  json += "}";
  server.send(200, "text/json", json);
  json = String();
}

void sendBrightness()
{
  String json = String(brightness);
  server.send(200, "text/json", json);
  json = String();
}

void sendSolidColor()
{
  String json = "{";
  json += "\"r\":" + String(solidColor.r);
  json += ",\"g\":" + String(solidColor.g);
  json += ",\"b\":" + String(solidColor.b);
  json += "}";
  server.send(200, "text/json", json);
  json = String();
}


boolean HTTPsend()
{

  // We now create a URI for the request
  String url = F("/");

  // embed IP, important if there is NAT/PAT
  IPAddress ip = WiFi.localIP();
  char ipStrDotted[20];
  sprintf(ipStrDotted, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  char ipStrDashed[20];
  sprintf(ipStrDashed, "%u-%u-%u-%u", ip[0], ip[1], ip[2], ip[3]);

  //String x = patterns[currentPatternIndex].name;

  /*
    for (int PIndex = 0; PIndex < patternCount; PIndex++) {
      Serial.println(patterns);

    }
  */

  //int x = leds[1].red;


  // Create json root object
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["module"] = "ESPEasy";
  root["version"] = "1.02";

  // Create nested objects
  JsonObject& data = root.createNestedObject("data");
  JsonObject& ESP = data.createNestedObject("ESP");
  ESP["name"] = ipStrDashed;
  ESP["unit"] = system_get_chip_id();
  ESP["version"] = 150;
  ESP["build"] = 150;
  ESP["sleep"] = 0;
  ESP["ip"] = ipStrDotted;

  // Create nested SENSOR json object
  JsonObject& SENSOR = data.createNestedObject("SENSOR");

  JsonObject& val0 = SENSOR.createNestedObject("0");
  val0["valueName"]  = "onOff";
  val0["value"]      = power ? "on" : "off";
  val0["type"]       = 999;

  JsonObject& val1 = SENSOR.createNestedObject("1");
  val1["valueName"]  = "pct";
  byte pct = map(brightness, 0, 255, 0 , 100);
  val1["value"]      = pct;
  val1["type"]       = 999;

  JsonObject& val2 = SENSOR.createNestedObject("2");
  val2["valueName"]  = "effectIndex";
  val2["value"]      = currentPatternIndex;
  val2["type"]       = 999;

  JsonObject& val3 = SENSOR.createNestedObject("3");
  val3["valueName"]  = "effectName";
  val3["value"]      = patterns[currentPatternIndex].name;
  val3["type"]       = 999;

  JsonObject& val4 = SENSOR.createNestedObject("4");
  val4["valueName"]  = "effect-fire-cooling";
  val4["value"]      = COOLING;
  val4["type"]       = 999;

  JsonObject& val5 = SENSOR.createNestedObject("5");
  val5["valueName"]  = "effect-fire-sparking";
  val5["value"]      = SPARKING;
  val5["type"]       = 999;

//if (currentPatternIndex == 11) {
  JsonObject& val6 = SENSOR.createNestedObject("6");
  val6["valueName"]  = "rgb";
  val6["value"]      = fhem.rgb;
  val6["type"]       = 999;
//}

  // Create json buffer
  char buffer[root.measureLength() + 1];
  root.printTo(buffer, sizeof(buffer));
  // Push data to server
  FHEMHTTPsend(url, buffer);
}


boolean FHEMHTTPsend(String url, char* buffer)
{
  boolean success = false;

  String authHeader = "";

  char          ControllerUser[26];
  String cuser = "ich";
  strncpy(ControllerUser, cuser.c_str(), sizeof(ControllerUser));

  char          ControllerPassword[64];
  String cpass = "1234";
  strncpy(ControllerPassword, cpass.c_str(), sizeof(ControllerPassword));


  if ((ControllerUser[0] != 0) && (ControllerPassword[0] != 0)) {
    base64 encoder;
    String auth = ControllerUser;
    auth += ":";
    auth += ControllerPassword;
    authHeader = "Authorization: Basic " + encoder.encode(auth) + " \r\n";
  }


  char log[80];
  //  url.toCharArray(log, 80);
  //  addLog(LOG_LEVEL_DEBUG_MORE, log);

  // todo: read config from WifiManager
  byte Controller_IP[4];
  Controller_IP[0] = 192;
  Controller_IP[1] = 168;
  Controller_IP[2] = 30;
  Controller_IP[3] = 53;
  unsigned int  ControllerPort = 8383;
  int connectionFailures = 0;

  char host[20];
  sprintf_P(host, PSTR("%u.%u.%u.%u"), Controller_IP[0], Controller_IP[1], Controller_IP[2], Controller_IP[3]);

  sprintf_P(log, PSTR("%s%s using port %u"), "HTTP : connecting to ", host, ControllerPort);
  //  addLog(LOG_LEVEL_DEBUG, log);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, ControllerPort)) {
    connectionFailures++;
    strcpy_P(log, PSTR("HTTP : connection failed"));
    //    addLog(LOG_LEVEL_ERROR, log);
    return false;
  }

  //  statusLED(true);
  if (connectionFailures)
    connectionFailures--;

  // This will send the request to the server
  int len = strlen(buffer);
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Content-Length: " + len + "\r\n" +
               "Host: " + host + "\r\n" + authHeader +
               "Connection: close\r\n\r\n"
               + buffer);

  unsigned long timer = millis() + 200;
  while (!client.available() && millis() < timer)
    delay(1);

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\n');
    String helper = line;
    line.toCharArray(log, 80);
    //    addLog(LOG_LEVEL_DEBUG_MORE, log);
    if (line.substring(0, 15) == "HTTP/1.1 200 OK") {
      strcpy_P(log, PSTR("HTTP : Success"));
      success = true;
    }
    else if (line.substring(0, 24) == "HTTP/1.1 400 Bad Request") {
      strcpy_P(log, PSTR("HTTP : Unauthorized"));
    }
    else if (line.substring(0, 25) == "HTTP/1.1 401 Unauthorized") {
      strcpy_P(log, PSTR("HTTP : Unauthorized"));
    }
    //    addLog(LOG_LEVEL_DEBUG, log);
    delay(1);
  }
  strcpy_P(log, PSTR("HTTP : closing connection"));
  //  addLog(LOG_LEVEL_DEBUG, log);
  client.flush();
  client.stop();
}

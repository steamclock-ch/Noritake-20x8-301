
IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);


void clearWiFIifConnected()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Disconnect WiFi");
    WiFi.disconnect(true);
    delay(1000);
    Serial.println("Disconnected");
  }
}


void setAP()
{
  //params.setWiFiCredentialsValidity(false);
  Serial.println("\nNo Wifi, moving to AP");

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("SteamClock-611") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  
  u8x8.drawString(0,0,"SteamClock-611");
  u8x8.drawString(0,1,WiFi.softAPIP().toString().c_str());
  
  dnsServer.setTTL(10);
  dnsServer.start(53, "*", WiFi.softAPIP());

}

//*********************************************************
boolean setupWiFi()
{
  int wifiLoop = 0;
  bool actAsPortal = false;

  if (params.WiFiCredsValidity)
  {
    Serial.print("Valid credentials, Starting Wifi on ");
    Serial.println(params.WiFiSSID);
    WiFi.mode(WIFI_STA);

    WiFi.begin(params.WiFiSSID, params.WiFiPSK);
    u8x8.drawString(0,0,WiFi.SSID().c_str());

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      Serial.print(".");
      wifiLoop++;
      Serial.print(wifiLoop);
      if (wifiLoop > 30) {
        break;
      }
    }

    if (wifiLoop < 30)
    {
      Serial.println();
      Serial.print("\nConnected, IP address: ");
      Serial.print("SSID: ");
      Serial.println(WiFi.SSID());
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.println("");
      u8x8.drawString(0,1,WiFi.localIP().toString().c_str());
    }
    else
    {
      setAP();
      actAsPortal = true;
    }
  }
  else
  {
    setAP();
    actAsPortal = true;
  }

  WiFiNetworkCount = WiFi.scanNetworks();
  return actAsPortal;
}


String padListing(String FileName, int expLength){
 
 while(FileName.length()<expLength){
  FileName = FileName +" "; 
 }
 return FileName;
}

void SPIFFSListing(){
  Serial.print("\n\nSPIFFS Listing\n");
  Serial.print("╔════════════════════╗\n");
  Serial.print("║   SPIFFS LISTING                 ║\n");
  Serial.print("╠═════════════ ══════╣\n");
  
  String str = "";
  
  Dir dir = SPIFFS.openDir("");
  while (dir.next()) {
    Serial.print("║");
    Serial.print(padListing(dir.fileName(),20));
    Serial.print(" ╬ ");
    Serial.print(padListing(String(dir.fileSize()),10));
    Serial.println("║");
  }
  Serial.print("╚═══════════════════╝\n");
}

void setupSPIFFS(){

  //SPIFFSConfig cfg;
  //cfg.setAutoFormat(true);
  //SPIFFS.setConfig(cfg);
  
  if (!SPIFFS.begin()) {
    Serial.println("Error mounting the file system");
    Serial.print("Formatting SPIFFS ...");
    Serial.print(SPIFFS.format());
    Serial.println(".. done");
  } else {
    SPIFFSListing();
  }


}

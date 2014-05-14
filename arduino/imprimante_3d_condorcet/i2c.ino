// déclencheur à la réception de données du Raspberry par l'i2c
void receptionDonnees(int byteCount){
  while(Wire.available()) {
    mode = Wire.read();
    if (mode == 0x0A){    
    Serial.println("Configuration de l'Arduino");
    Serial.println(Wire.read());
    Serial.println(Wire.read());
    Serial.println(Wire.read());
    Serial.println(Wire.read());
    Serial.println(Wire.read());
    Serial.println(Wire.read());
    }
 }
}

// callback pour le renvoi de données au raspberry
void envoiMode(){
  Wire.write(mode);
}



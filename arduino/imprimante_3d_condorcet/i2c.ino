// déclencheur à la réception de données du Raspberry par l'i2c
void receptionDonnees(int byteCount){
  while(Wire.available()) {
    mode = Wire.read();
    if (mode == 0x0A){    
    Serial.println("Configuration de l'Arduino:");
    epaisseurCouche = Wire.read();
    Serial.println(epaisseurCouche);
    Serial.println(Wire.read());
    intervalleEntreExp = Wire.read();
    Serial.println(intervalleEntreExp);
    profPlongee = Wire.read();
    Serial.println(profPlongee);
    tempsArretAvantRemontee = Wire.read();
    Serial.println(tempsArretAvantRemontee);
    tempsPauseAvantExpo = Wire.read();
    Serial.println(tempsPauseAvantExpo);
    modifConfig = true;
    }
 }
}

// callback pour le renvoi de données au raspberry
void envoiMode(){
  Wire.write(mode);
}



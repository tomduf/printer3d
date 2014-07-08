// déclencheur à la réception de données du Raspberry par l'i2c
void receptionDonnees(int byteCount){
  while(Wire.available()) {
    mode = Wire.read();
    if (mode == 0x0A){
      epaisseurCouche = Wire.read();
      intervalleEntreExp = Wire.read();
      profPlongee = Wire.read();
      tempsArretAvantRemontee = Wire.read();
      tempsPauseAvantExpo = Wire.read();
      modifConfig = true;
    }
 }
}

// callback pour le renvoi de données au raspberry
void envoiMode(){
  Wire.write(mode);
}



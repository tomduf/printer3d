void balayageModes(){  
  // Balaye le mode (modifié par les boutons ou par le Raspberry)
  switch (mode){
  case modeStop :                // stop
    if (pas != 0){
      vitesse = 100;
      if (texteInfo != "Stop"){
        texteInfo = "Stop";
        changerTexte = true;
      }
      pas = 0;
      etatDiapo=0;
    }
    break;
  case modeHaut :                // up
    if (pas != 1){
      vitesse = 100;
      if (texteInfo != "Haut"){
        texteInfo = "Haut";
        changerTexte = true;
      }
      pas = 1;
      etatDiapo = 0;
    }
    break;
  case modeBas :                 // down
    if (pas != -1){
      vitesse = 100;
      if (texteInfo != "Bas"){
        texteInfo = "Bas";
        changerTexte = true;
      }
      pas = -1;
      etatDiapo = 0;
    }
    break;
  case modePrint :                // impression
    vitesse = 100;
    if (texteInfo != "Print"){
      texteInfo = "Print";
      changerTexte = true;
    }
    pas = 0;
    if (etatDiapo == 3 )
      etatDiapo = 0;
    break;
  case modeEtatDiapo :             // affichage
    vitesse = 100;
    texteInfo = "Print";
    changerTexte = true;
    pas = 0;
    etatDiapo++;
    break;
  case modePlateau :             // remontée du plateau
    if (pas != -1){
      vitesse = 100;
      if (texteInfo != "Montee"){
        texteInfo = "Montee";
        changerTexte = true;
      }
      pas = 1;
      etatDiapo = 0;
    }
    break;
  case modeCycle :               // cycle moteur (bas -> haut - épaisseur couche)
    if (pas != -1){
      vitesse = 50;
      if (texteInfo != "Cycle"){
        texteInfo = "Cycle";
        changerTexte = true;
      }
      pas = -1;
      etatDiapo=0;
    }
    break;
  case modeConfig :
    affichageTexte("config","ok",0,"");
    changerTexte = false;
  }
  if (changerTexte)
    affichageTexte(texteInfo, "", numCouche, "");
  changerTexte = false; 

  if (mode == modeHaut || mode == modeBas){
    myStepper.step(pas);
    envoiMode();
  }
  else if (mode == modeCycle)    // cycle moteur (bas -> haut - épaisseur couche)
    cycle();

  else if (mode == modePlateau)  // cycle de remontée du plateau
      remonteePlateau();
}


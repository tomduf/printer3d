Imprimante 3D
Lycée Condorcet - Montreuil

1. Origine du projet
Suite à la découverte de l'expérience "manuelle" d'imprimante 3D proposée par l'université de l'Illinois,
l'idée est venue de proposer la réalisation d'un imprimante 3D dans le cadre des projets pluridisciplinaires de Terminale SI (Sciences de l'Ingénieur).
Une équipe de quatre élèves a choisi ce projet en septembre 2012 :
- Ludovic Araujo
- Thomas Bande
- Max Dumesny
- Clément Soriot

2. Première version
À l'issue du projet  (juin 2013), un prototype fonctionnel mais non finalisé a permis l'impression d'une petite pièce très détaillée. La résine a été
réalisée par un professeur du laboratoire de physique-chimie (Nicolas Dubuisson) selon les recommandations du site de l'Université de l'Illinois.
L'affichage s'effectuait par vidéoprojecteur DPL et un PC via un programme Processing.
La commande du moteur était réalisée par une carte contrôleur Arduino Uno.
Le PC et l'Arduino Uno communiquaient par port série.

3. Version 2 en cours de développement (2014)
Le projet a été repris avec un objectif de portabilité totale :
- plus de PC externe
- une alimentation unique
- un accès wifi / Ethernet
- un clavier / souris intégré
- transportabilité manuelle

La solution retenue est la suivante :
- Raspberry Pi pour le logiciel d'affichage DLP et gestion du serveur d'images
- Affichage par navigateur web
- Serveur node.js
- Affichage client jquery
- Arduino Uno pour la commande moteur et l'affichage sur petit écran Oled
- Clavier / Souris articulé
- Poignée de transport
- Miroir basculant permettant d'afficher les instructions au mur via le vidéoprojecteur

Thomas DUFOUR
Professeur de Sciences de l'Ingénieur
Lycée Condorcet
31 rue Désiré Chevalier
93100 MONTREUIL

thomas.dufour@condorcet93.fr
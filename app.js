
/**
 * Module dependencies.
 */

var express = require('express');
var fs = require('fs');
var thumb = require('node-thumbnail').thumb;
var routes = require('./routes');
var user = require('./routes/user');
var http = require('http');
var path = require('path');
var i2c = require('i2c');

// Adresse de l'Arduino
var arduino = 0x05;
var accel = 0x44;

// Connexion avec l'Arduino
var wireArduino = new i2c(arduino, {device: '/dev/i2c-1'});
// Connexion avec l'accéléromètre
var wireAccel = new i2c(accel, {device: '/dev/i2c-1'});

var app = express();
var dossier_diapos = path.join(__dirname, 'public/slices');
var dossier_miniatures = path.join(__dirname, 'public/thumb-slices');

// all environments
app.set('port', process.env.PORT || 3000);
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.json());
app.use(express.urlencoded());
app.use(express.methodOverride());
app.use(express.cookieParser('your secret here'));
app.use(express.session());
app.use(app.router);
app.use(express.static(path.join(__dirname, 'public')));

// development only
if ('development' == app.get('env')) {
  app.use(express.errorHandler());
}

app.get('/users', user.list);

// Message de bienvenue
console.log('Serveur imprimante 3D - Lycée Condorcet');

// Réception de données i2c Arduino
wireArduino.on('data', function(data) {
  // result for continuous stream contains data buffer, address, length, timestamp
  for (var i = 0; i < data.length; i++){
	console.log(data[i]);
   }
});


// Démarrage
main();

// Récupération des vignettes des diapos par lecture du dossier
function main() {
	wireArduino.scan(function(err, tab_periphs_i2c) {
	  // Scan des périphériques i2c
	  if (!err){
		  console.log("> " + tab_periphs_i2c.length + " périphériques i2c détectés :");
		  tab_periphs_i2c.forEach(function(periph_i2c){
			console.log("0x" + periph_i2c.toString(16)) ;
		 });
	  }
	});
	
	// Lecture d'un octet
	wireArduino.readByte(function(err,res){
		console.log("Etat du mode de l'Arduino: " + res.toString(16));
	});
	
	// Lecture du dossier des miniatures	
    fs.readdir(dossier_miniatures, function(err1, fichiersThumb) {
        if (err1) {
            console.log("> les miniatures n'existent pas, création automatique en cours...");
            // Création du dossier thumb-slices
            fs.mkdirSync(dossier_miniatures);
            // Création des vignettes si elle n'existent pas
            thumb({
                source : dossier_diapos,
                destination : dossier_miniatures,
                concurrency : 4,
                width : 40
            }, function() {
                console.log('> ...miniatures créées !');
                main(); // on rappelle la fonction principale pour rescanner le dossier des diapos
            });
        } else {
            fs.readdir(dossier_diapos, function(err2, fichiersDiapos) {

                // Template de la page HTML principale (avec le module ejs)
                app.get('/', function(req, res) {
                    res.render('index', {
                        title : 'Imprimante 3D - Lycée Condorcet / Montreuil',
                        thumbFichiers : fichiersThumb, // on transmets le tableau des vignettes en paramètres afin de les afficher
                        diaposFichiers : fichiersDiapos // ainsi que le tableau des diapos pour les afficher une à une
                    });
                });

                // Appel d'une diapo et cycle moteur
                app.get('/diapo/:num_diapo', function(req, res) {
					if (req.params.num_diapo > 0){
						// Lancement d'un cycle moteur
						wireArduino.writeByte(0x09, function(err){});
						process.stdout.write("Cycle moteur en cours...");
						var count = 0;
                        // Tant que l'Arduino ne répond pas 8 (fin du cycle moteur)
						while (wireArduino.readByte(function(err,res){
							if (res != 8) return true;								
						}))
						{
							if (count == 250){ // affichage temporisé des points
								process.stdout.write(".");
								count = 0;
							}
							count++;
						}
						console.log("");
						console.log("Cycle moteur terminé");
						console.log("Plateau remonté pour la couche suivante");								
					}
					console.log("Affichage de la diapo " + (parseInt(req.params.num_diapo) + 1) + "/" + fichiersDiapos.length);
					res.send(fichiersDiapos[req.params.num_diapo]);
                    
                });
                // Appel de la fin d'impression
                app.get('/fin', function(req, res) {
                    // Lancement d'un cycle moteur
                    wireArduino.writeByte(0x07, function(err){});
                    process.stdout.write("Impression terminée, remontée du  plateau en cours...");
                    var count = 0;
                    // Tant que l'Arduino ne répond pas 6 (fin de la remontée du plateau)
                    while (wireArduino.readByte(function(err,res){
                        if (res != 6) return true;
                    }))
                    {
                        if (count == 250){ // affichage temporisé des points
                            process.stdout.write(".");
                            count = 0;
                        }
                        count++;
                    }
                    console.log("");
                    console.log("> Plateau remonté. Impression terminée");
                    console.log("> Interposer le miroir");
                    console.log("> Retirer le bain de résine");
                    console.log("> Insérer le bac de rinçage à température ambiante");
                });

                http.createServer(app).listen(app.get('port'), function(){
                    console.log('> ' + fichiersThumb.length + " vignettes lues");
                    console.log('> Lancer http://localhost:' + app.get('port') + ' en mode plein écran ');
                });
            });
        }
    });
}


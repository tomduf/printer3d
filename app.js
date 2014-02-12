
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

// Démarrage
main();

// Récupération des vignettes des diapos par lecture du dossier
function main() {
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

                // Template de la page principale (avec le module ejs)
                app.get('/', function(req, res) {
                    res.render('index', {
                        title : 'Imprimante 3D - Lycée Condorcet / Montreuil',
                        thumbFichiers : fichiersThumb, // on transmets le tableau des vignettes en paramètres afin de les afficher
                        diaposFichiers : fichiersDiapos // ainsi que le tableau des diapos pour les afficher une à une
                    });
                });

                // Appel d'une diapo par passage de paramètre
                app.get('/diapo/:num_diapo', function(req, res) {
                    res.send(fichiersDiapos[req.params.num_diapo]);
                });

                http.createServer(app).listen(app.get('port'), function(){
                    console.log('> ' + fichiersThumb.length + " vignettes lues");
                    console.log('> Lancer http://localhost:' + app.get('port') + ' en mode plein écran ');
                });
            });
        }
    });
}


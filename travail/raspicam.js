/**
 * Created by tomduf on 07/03/2014.
 */
var RaspiCam = require("raspicam");

var camera = new RaspiCam({
    mode : 'photo'});

//to take a snapshot, start a timelapse or video recording
camera.start( );

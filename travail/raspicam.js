/**
 * Created by tomduf on 07/03/2014.
 */
var RaspiCam = require("raspicam");

var camera = new RaspiCam({
    mode : "photo",
    output : "./images/photo.jpg"
});

//to take a snapshot, start a timelapse or video recording
camera.start( );

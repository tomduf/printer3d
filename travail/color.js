var clc = require('cli-color');
var error = clc.red.bold;
var warn = clc.yellow;
var notice = clc.blue;

console.log(clc.red('Text in red'));
console.log(clc.red.bgWhite.underline('Underlined red text on white background.'));
console.log(clc.red('red') + ' plain ' + clc.blue('blue'));

console.log(error('Error!'));
console.log(warn('Warning'));
console.log(notice('Notice'));
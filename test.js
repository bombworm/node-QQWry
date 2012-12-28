var util = require('util');
var QQWry = require('./build/Release/QQWry.node').QQWry;
var q;
try {
	q = new QQWry(process.cwd() + '\\' + 'qqwry.dat');
} catch (e) {
	console.log(e.toString());
}

var http = require('http');
http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/plain'});
  res.write(q.QueryIP('8.8.8.8') + '\r\n');
  res.write(q.QueryIP('2.2.2.2') + '\r\n');
  res.end('\n');
}).listen(2012, '127.0.0.1');


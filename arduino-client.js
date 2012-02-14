var connect = require('connect'),
 	http = require('http'),
	url = require('url')
	
function routes(app) {
  app.get('/winner', function (req, res, next) {

	var winner = ''; 
	var html = '';
	
	http.request(url.parse('http://192.168.0.177:80'), function (response) {
	console.log(response)  
	console.log("made request")
	response.setEncoding('ascii')
	  response.on('data', function (chunk) {
	  	    winner += chunk.toString();
			console.log(chunk)
	
	  	  }).on('end', function () {
	
			 
			//if statement testing value of winning candidate
			if(winner === "1\r\n"){
				html += "<div id='candidate1' class='face winner'></div><div id='candidate2' class='face loser'></div><div id='candidate3' class='face loser'></div><div class='bulb1'></div>"	
			}else if(winner === "2\r\n"){
				html += "<div id='candidate1' class='face loser'></div><div id='candidate2' class='face winner'></div><div id='candidate3' class='face loser'></div><div class='bulb2'></div>"
			}else if(winner === "3\r\n"){
				html += "<div id='candidate1' class='face loser'></div><div id='candidate2' class='face loser'></div><div id='candidate3' class='face winner'></div><div class='bulb3'></div>"
			}
		
			html += "</div></div></body></html>"
	
			  
			res.writeHead(200, { 'Content-Type': 'text/html' });
			res.end(html);
		});
	
	}).end();

  });
}

var connect = require('connect')

var server = connect.createServer(
	connect.favicon(),
	connect.logger(),
	connect.directory(__dirname),
	connect.static(__dirname + '/public'),
	connect.router(routes)
	)

server.listen(3000)

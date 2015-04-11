var koa = require('koa')
var route = require('koa-route')
var serve = require('koa-static')
var ws = require('koa-websocket')
var app = ws(koa())

/*app.use(function *(){
  this.body = 'hello world'
})*/

// static file serve
app.use(serve(__dirname + '/public'))

app.listen(3000)


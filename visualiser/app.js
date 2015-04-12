var koa = require('koa')
var router = require('koa-router')
var serve = require('koa-static')
var app = koa()

// koa
app.use(serve(__dirname + '/public'))
app.use(router(app))

app.get('/aa', function *(next){
  this.body = 'hello world'
})

// socket.io
var server = require('http').createServer(app.callback())
var io = require('socket.io')(server)

io.on('connection', function(socket){
  console.log('connection')
})

server.listen(3000)

// serial communication
var serialport = require('serialport')
var SerialPort = serialport.SerialPort
var sp = new SerialPort('/dev/tty.usbserial-DJ004Q2V', {
  baudrate: 115200,
  parser: serialport.parsers.readline('\n')
})

sp.on('data', function (data){
  console.log(data)
  io.emit('gps', JSON.parse(data))
})

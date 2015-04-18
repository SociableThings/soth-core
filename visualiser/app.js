var fs = require('fs')

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

    socket
      .on('getUsbDeviceNames', function(msg){
        console.log('fd')
        fs.readdir('/dev/', function(err, files){
          if (err) throw err
          var fileList = []
          files.filter(function(file){
            return /tty\.usb.*$/.test(file)
          }).forEach(function (file) {
            fileList.push('/dev/' + file);
          })
          console.log(fileList)
          socket.emit('responseUsbDeviceNames', fileList)
        })
      })
      .on('connectUsbDevice', function(msg){
        initializeSerialPort(msg.deviceName, msg.baudrate)
      })
  })

server.listen(3000)

// serial communication
var serialport = require('serialport')
var SerialPort = serialport.SerialPort

function initializeSerialPort(device, baudrate){
  var sp = new SerialPort(device, {
    baudrate: baudrate,
    parser: serialport.parsers.readline('\n')
  })

  sp.on('data', function (data){
    console.log(data)
    io.emit('gps', JSON.parse(data))
  })
}

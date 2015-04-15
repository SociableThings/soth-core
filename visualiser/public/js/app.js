var map;
var centerMarker;

$(function(){
  var latlng = new google.maps.LatLng(35.69020, 139.69001)
  map = new google.maps.Map(document.getElementById('map_canvas'), {
    center: latlng,
    zoom: 17,
    mapTypeId: google.maps.MapTypeId.ROADMAP,
    mapTypeControl: true
  })

  centerMarker = new google.maps.Marker({
    position: latlng,
    map: map,
    animation: false,
    draggable: false
  })
})

socket.on('gps', function(msg){
  console.log(msg)
  if(msg.validity){
    var latlng = new google.maps.LatLng(msg.latitude, msg.longitude)
    if(map) map.setCenter(latlng)
    if(centerMarker) centerMarker.setPosition(latlng)
  }
  
  $('#gps_info_validity').html(msg.validity ? '<span style="color:green">Valid</span>' : '<span style="color:red">Invalid</span>')
  $('#gps_info_latitude').html(msg.latitude)
  $('#gps_info_longitude').html(msg.longitude)
  $('#gps_info_speed').html(msg.speed + ' km/h')
  $('#gps_info_heading').html(msg.heading + ' deg.')
  $('#gps_info_date').html(
    msg.year + '/' + ('0'+msg.month).slice(-2) + '/' + ('0'+msg.day).slice(-2) + ' ' +
    ('0'+msg.hour).slice(-2) + ':' + ('0'+msg.minute).slice(-2) + ':' + ('0'+msg.second).slice(-2)
  )
})


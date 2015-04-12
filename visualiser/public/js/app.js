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
  
  $('#gps_info').html(
    'Validity: ' + msg.validity + 
    ', Latitude: ' + msg.latitude +
    ', Longitude: ' + msg.longitude +
    ', Speed: ' + msg.speed + 'km/h' +
    ', Heading: ' + msg.heading + 'deg.' +
    ', Date: ' + msg.year + '/' + msg.month + '/' + msg.day + ' ' +
    msg.hour + ':' + msg.minute + ':' + msg.second
  )
})


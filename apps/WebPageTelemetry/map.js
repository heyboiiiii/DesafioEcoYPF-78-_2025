//var map = L.map('map',{zoomControl:false,attributionControl:false,},).setView([0, 0], 0);

 // 1. Define map bounds (lat, lng)
const imageBounds = [[-32.451208, -58.318268], [-32.459931, -58.310758]];

// top-right autodromo  =  -32.451208, -58.318268
// bottom-left autodromo =  -32.459931, -58.310758



// 2. Create map
const map = L.map('map', {
  //crs: L.CRS.Simple,  // optional for pixel-based image, not needed if using lat/lng bounds
  minZoom: 15.5,
  zoomControl: false
});

// 3. Add the image
const imageUrl = 'autodromo.png'; // your static PNG file
L.imageOverlay(imageUrl, imageBounds).addTo(map);
//L.tilelayer(imageUrl);
map.fitBounds(imageBounds);



var circle = L.circle([-32.451368, -58.317751], {
    color: 'red',
    fillColor: '#f03',
    fillOpacity: 1,
    radius: 10
}).addTo(map);
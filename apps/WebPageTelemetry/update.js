async function updateData() {
  try {
    const res = await fetch('/data');
    if (!res.ok) throw new Error("Bad response");
    const data = await res.json();
    
    document.querySelector('.voltage_value').textContent = data.voltage.toFixed(2) + ' V';
    document.querySelector('.current_value').textContent = data.current.toFixed(2) + ' A';
    document.querySelector('.pot_value').textContent = data.power.toFixed(2) + ' W';
    document.querySelector('.speed_value').textContent = data.speed.toFixed(1) + ' km/h';
    document.querySelector('.charge_value').textContent = data.charge.toFixed(0) + ' %';
    document.querySelector('.lora_value').textContent = data.lora ? 'Conectado' : 'Desconectado';
    document.querySelector('.gps_value').textContent = data.gps ? 'Activo' : 'Inactivo';
  } catch (err) {
    console.error('Error fetching data:', err);
  }
}

// tasa de actualizacion; 
setInterval(updateData, 100);
window.onload = updateData;
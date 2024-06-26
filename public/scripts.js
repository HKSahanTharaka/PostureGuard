const socket = io();

// Handle data from Board 1
socket.on('sensorData1', (data) => {
  console.log('Received data from Board 1:', data);
  document.getElementById('heartRateData').innerHTML = `
    <p class="sensor-data"><span class="sensor-label">Heart Rate:</span> ${data.heartRate} bpm</p>
  `;
  document.getElementById('flexSensorData').innerHTML = `
    <p class="sensor-data"><span class="sensor-label">Flex Sensor 1:</span> ${data.flexSensor1}</p>
    <p class="sensor-data"><span class="sensor-label">Flex Sensor 2:</span> ${data.flexSensor2}</p>
  `;
});

// Handle data from Board 2
socket.on('sensorData2', (data) => {
  console.log('Received data from Board 2:', data);

  document.getElementById('leftMatData').innerHTML = `
    <p class="sensor-data"><span class="sensor-label">Left Mat Weight:</span> ${data.leftMatWeight} kg</p>
    <p class="sensor-data"><span class="sensor-label">Load Cell 1:</span> ${data.loadCellPercentages[0]}%</p>
    <p class="sensor-data"><span class="sensor-label">Load Cell 2:</span> ${data.loadCellPercentages[1]}%</p>
    <p class="sensor-data"><span class="sensor-label">Load Cell 3:</span> ${data.loadCellPercentages[2]}%</p>
    <p class="sensor-data"><span class="sensor-label">Load Cell 4:</span> ${data.loadCellPercentages[3]}%</p>
  `;

  document.getElementById('rightMatData').innerHTML = `
    <p class="sensor-data"><span class="sensor-label">Right Mat Weight:</span> ${data.rightMatWeight} kg</p>
    <p class="sensor-data"><span class="sensor-label">Load Cell 5:</span> ${data.loadCellPercentages[4]}%</p>
    <p class="sensor-data"><span class="sensor-label">Load Cell 6:</span> ${data.loadCellPercentages[5]}%</p>
    <p class="sensor-data"><span class="sensor-label">Load Cell 7:</span> ${data.loadCellPercentages[6]}%</p>
    <p class="sensor-data"><span class="sensor-label">Load Cell 8:</span> ${data.loadCellPercentages[7]}%</p>
  `;

  // Calculate the position of the red line
  const totalWeight = data.leftMatWeight + data.rightMatWeight;
  const leftPercentage = (data.leftMatWeight / totalWeight) * 100;
  const rightPercentage = (data.rightMatWeight / totalWeight) * 100;

  // Set the position of the red line
  const redLine = document.getElementById('redLine');
  if (totalWeight !== 0) {
    redLine.style.left = `calc(${rightPercentage}% - 1px)`; // Corrected: move to left if left mat has more weight
  } else {
    redLine.style.left = '50%'; // Center it if there is no weight
  }
});

// Fetch initial configuration for Board 1
fetch('/config1')
  .then(response => response.json())
  .then(config => {
    document.getElementById('flexRange1Min').value = config.flexRange1Min;
    document.getElementById('flexRange1Max').value = config.flexRange1Max;
    document.getElementById('flexRange2Min').value = config.flexRange2Min;
    document.getElementById('flexRange2Max').value = config.flexRange2Max;
  });

// Fetch initial configuration for Board 2
fetch('/config2')
  .then(response => response.json())
  .then(config => {
    document.getElementById('weightDifferenceThreshold').value = config.weightDifferenceThreshold;
  });

// Function to update configuration for Board 1
document.getElementById('updateBoard1ConfigBtn').addEventListener('click', () => {
  const config = {
    flexRange1Min: parseInt(document.getElementById('flexRange1Min').value),
    flexRange1Max: parseInt(document.getElementById('flexRange1Max').value),
    flexRange2Min: parseInt(document.getElementById('flexRange2Min').value),
    flexRange2Max: parseInt(document.getElementById('flexRange2Max').value)
  };

  fetch('/config1', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(config)
  }).then(() => {
    alert('Board 1 Configuration Updated Successfully');
  }).catch((error) => {
    console.error('Error updating Board 1 configuration:', error);
  });
});

// Function to update configuration for Board 2
document.getElementById('updateBoard2ConfigBtn').addEventListener('click', () => {
  const config = {
    weightDifferenceThreshold: parseFloat(document.getElementById('weightDifferenceThreshold').value)
  };

  fetch('/config2', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(config)
  }).then(() => {
    alert('Board 2 Configuration Updated Successfully');
  }).catch((error) => {
    console.error('Error updating Board 2 configuration:', error);
  });
});


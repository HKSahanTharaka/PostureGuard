const express = require('express');
const http = require('http');
const socketIo = require('socket.io');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);
const port = 3000;

app.use(express.json());

// Serve static files from the 'public' directory
app.use(express.static('public'));

let sensorData = [];

app.post('/data', (req, res) => {
  const data = req.body;
  sensorData.push(data);
  console.log('Received sensor data:', data);
  io.emit('sensorData', data); // Send data to all connected clients
  res.send('Data received');
});

app.get('/data', (req, res) => {
  res.json(sensorData);
});

io.on('connection', (socket) => {
  console.log('New client connected');
  socket.emit('initialData', sensorData);
  socket.on('disconnect', () => {
    console.log('Client disconnected');
  });
});

server.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});

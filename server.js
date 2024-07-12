const express = require('express');
const bodyParser = require('body-parser');
const http = require('http');
const socketIo = require('socket.io');
const path = require('path');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

app.use(bodyParser.json());
app.use(express.static(path.join(__dirname, 'public')));

let board1Config = {
  flexRange1Min: 260000,
  flexRange1Max: 270000,
  flexRange2Min: 260000,
  flexRange2Max: 270000
};

let board2Config = {
  weightDifferenceThreshold: 6.0
};

// Route for serving the frontend
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Route to handle data from ESP32 board 1
app.post('/data1', (req, res) => {
  const data = req.body;
  console.log('Data from Board 1:', data);
  io.emit('sensorData1', data);
  res.sendStatus(200);
});

// Route to handle data from ESP32 board 2
app.post('/data2', (req, res) => {
  const data = req.body;
  console.log('Data from Board 2:', data);
  io.emit('sensorData2', data);
  res.sendStatus(200);
});

// Route to get config for ESP32 board 1
app.get('/config1', (req, res) => {
  res.json(board1Config);
});

// Route to update config for ESP32 board 1
app.post('/config1', (req, res) => {
  board1Config = req.body;
  res.sendStatus(200);
});

// Route to get config for ESP32 board 2
app.get('/config2', (req, res) => {
  res.json(board2Config);
});

// Route to update config for ESP32 board 2
app.post('/config2', (req, res) => {
  board2Config = req.body;
  res.sendStatus(200);
});

// Socket.io connection handling
io.on('connection', (socket) => {
  console.log('Client connected');
  socket.on('disconnect', () => {
    console.log('Client disconnected');
  });
});

// Start the server
const port = 3000;
server.listen(port, () => {
  console.log(`Server is running on http://localhost:${port}`);
});

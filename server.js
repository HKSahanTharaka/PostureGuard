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

// Route for serving the frontend
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Route to handle data from ESP32 board 1
app.post('/data1', (req, res) => {
  const data = req.body;
  console.log('Data from Board 1:', data); // Debug: print received data
  io.emit('sensorData1', data);
  res.sendStatus(200);
});

// Route to handle data from ESP32 board 2
app.post('/data2', (req, res) => {
  const data = req.body;
  console.log('Data from Board 2:', data); // Debug: print received data
  io.emit('sensorData2', data);
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

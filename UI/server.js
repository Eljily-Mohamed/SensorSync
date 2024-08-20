import express from 'express';
import { createServer } from 'http';
import { Server } from 'socket.io';
import { SerialPort } from 'serialport';
import { ReadlineParser } from '@serialport/parser-readline';
import apiRouter from './routes.js'; 
import cors from 'cors';

const app = express();
const server = createServer(app);
const io = new Server(server);
app.use(cors());
app.use(express.json()); // Middleware to parse JSON bodies

// Use the API router
app.use('/api', apiRouter);

let port;
let parser;

// Function to set up the serial port based on the communication method
const setupSerialPort = (path) => {
  if (port) {
    port.close(); // Close the existing port if open
  }
  port = new SerialPort({ path, baudRate: 115200 });
  parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

  parser.on('data', (data) => {
    console.log('Data from serial port:', data);
    io.emit('serialData', data); // Emit the data to the connected clients
  });
};

// Initial serial port setup
setupSerialPort('/dev/ttyACM0'); // Default path

// Endpoint to set the communication method
app.post('/api/set-communication', (req, res) => {
  const { method } = req.body;
  if (method === 'ST-LINK') {
    setupSerialPort('/dev/ttyACM0');
  } else if (method === 'ZEGBEE') {
    setupSerialPort('/dev/ttyUSB0');
  } else {
    return res.status(400).json({ error: 'Invalid communication method' });
  }
  // Respond with JSON indicating success
  res.json({ message: `${method} has been set up successfully` });
});


// Socket.IO connection handling
io.on('connection', (socket) => {
  console.log('Client connected');

  // Listen for messages from clients
  socket.on('message', (data) => {
    console.log('Message from client:', data);
    // Write data to the serial port
    port.write(data);
  });

  // Listen for disconnection
  socket.on('disconnect', () => {
    console.log('Client disconnected');
  });
});

// Start the HTTP server
const PORT = 5000;
server.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});

import React, { useState, useEffect } from "react";
import socket from "../services/socketService.js";
import { ColorConverter } from '../utils/ColorConverter.js';
import BulbLight from '../components/BulbLight.js'; // Import the BulbLight component

const ColorComponent = () => {
  const [colorCode, setColorCode] = useState("#FFFFF"); // Initial color
  const [turnOn, setTurnOn] = useState(true);


  useEffect(() => {
    // Send message 'c' to the socket to request color update
    socket.emit("message", "c");
  
    // Listen for color updates from the socket
    socket.on("serialData", (sensorData) => {
      // Parse sensor data string to extract x and y values as floats
      const [xStr, yStr, briStr] = sensorData.split(',');
      const x = parseFloat(xStr);
      const y = parseFloat(yStr);
      const bri = parseInt(briStr);
      // Call xyBriToRgb function and log the result
      const rgb = ColorConverter.xyBriToRgb(x, y, bri);
  
      // Update the color with the new RGB value
      setColorCode(`#${rgb.r.toString(16).padStart(2, '0').toUpperCase()}${rgb.g.toString(16).padStart(2, '0').toUpperCase()}${rgb.b.toString(16).padStart(2, '0').toUpperCase()}`);
    });
  
    // Clean up socket listener
    return () => {
      socket.off("serialData");
    };
  }, []);
  
  return (
    <div>
      <BulbLight colorCode={colorCode} turnOn={turnOn} />
      <div style={{ backgroundColor: colorCode, width: "100px", height: "100px" }}></div>
      <h2>Color</h2>
      <p>Hexadecimal color code: {colorCode}</p>
    </div>
  );
};

export default ColorComponent;

import React, { useState, useEffect } from "react";
import socket from "../services/socketService.js";
import { fetchLatestTemperatureData, fetchLatestHumidityData, fetchMaxTemperature, fetchMinTemperature, fetchMaxHumidity, fetchMinHumidity } from "../services/apiService.js";
import LineChart from "../components/LineChart.js";
import BarChart from "../components/BarChart.js";

const Dashboard = () => {
  const [highestTemperature, setHighestTemperature] = useState(null);
  const [lowestTemperature, setLowestTemperature] = useState(null);
  const [highestHumidity, setHighestHumidity] = useState(null);
  const [lowestHumidity, setLowestHumidity] = useState(null);
  const [latestTemperatureData, setLatestTemperatureData] = useState([]);
  const [latestHumidityData, setLatestHumidityData] = useState([]);
  const [latestTemperatureDate, setLatestTemperatureDate] = useState(null);
  const [latestHumidityDate, setLatestHumidityDate] = useState(null);


  useEffect(() => {
    const fetchData = async () => {
      try {
        const maxTemp = await fetchMaxTemperature();
        setHighestTemperature(maxTemp['maxTemperature']);
    
        const minTemp = await fetchMinTemperature();
        setLowestTemperature(minTemp['minTemperature']);
    
        const maxHumid = await fetchMaxHumidity();
        setHighestHumidity(maxHumid['maxHumidity']);
    
        const minHumid = await fetchMinHumidity();
        setLowestHumidity(minHumid['minHumidity']);

        const latestTempData = await fetchLatestTemperatureData();
        const latestTempValues = latestTempData.map(item => item.value);
        const latestTempDates = latestTempData.map(item => item.dateRecorded);
        setLatestTemperatureData(latestTempValues);
        setLatestTemperatureDate(latestTempDates[0]);

        const latestHumData = await fetchLatestHumidityData();
        const latestHumValues = latestHumData.map(item => item.value);
        const latestHumDates = latestHumData.map(item => item.dateRecorded);
        setLatestHumidityData(latestHumValues);
        setLatestHumidityDate(latestHumDates[0]);
    
      } catch (error) {
        console.error("Error fetching data:", error);
      }
    };
  
    // Check for socket and realTimeData length within the effect
    if (socket) {
      socket.emit("message", "d"); // Assuming this triggers real-time data fetching
    }
  
    fetchData();
  
  }, []);
  

  return (
    // i can't use  max-w-screen-lg here as class 
    <div className="mx-auto max-w-screen-lg mt-8"> 
      {/* Welcome message */}
      <h1 className="text-2xl font-bold mb-4">Welcome back!</h1>
      {/* Cards */}
      <div className="grid grid-cols-4 gap-4">
        {/* Temperature Cards */}
        <div className="bg-blue-200 p-4 rounded-lg">
          <h2 className="text-lg font-semibold mb-2 text-black">Highest Temperature</h2>
          <p className="text-center">
            {highestTemperature !== null ? 
              <span className="text-white font-bold bg-blue-500 rounded-md p-1">{highestTemperature} °C</span> 
              : "Loading..."
            }
          </p>
        </div>
        <div className="bg-green-200 p-4 rounded-lg">
          <h2 className="text-lg font-semibold mb-2 text-black">Lowest Temperature</h2>
          <p className="text-center">
            {lowestTemperature !== null ? 
              <span className="text-white font-bold bg-green-500 rounded-md p-1">{lowestTemperature} °C</span> 
              : "Loading..."
            }
          </p>
        </div>
        {/* Humidity Cards */}
        <div className="bg-yellow-200 p-4 rounded-lg">
          <h2 className="text-lg font-semibold mb-2 text-black">Highest Humidity</h2>
          <p className="text-center">
            {highestHumidity !== null ? 
              <span className="text-white font-bold bg-yellow-500 rounded-md p-1">{highestHumidity}%</span> 
              : "Loading..."
            }
          </p>
        </div>
        <div className="bg-red-200 p-4 rounded-lg">
          <h2 className="text-lg font-semibold mb-2 text-black">Lowest Humidity</h2>
          <p className="text-center">
            {lowestHumidity !== null ? 
              <span className="text-white font-bold bg-red-500 rounded-md p-1">{lowestHumidity}%</span> 
              : "Loading..."
            }
          </p>
        </div>
      </div>
  
      {/* Charts */}
      <div className="grid grid-cols-2 gap-4 mt-8">
        {/* Temperature Chart */}
        <div className="bg-gray-200 p-4 rounded-lg">
        <h2 className="text-lg font-semibold mb-2 mt-8 text-black">Latest recorded date: {latestTemperatureDate}</h2>
          <h2 className="text-lg font-semibold mb-2 text-black">Temperature Chart</h2>
          <LineChart data={latestTemperatureData} unit="celsius" />
        </div>
        {/* Humidity Chart */}
        <div className="bg-gray-200 p-4 rounded-lg">
        <h2 className="text-lg font-semibold mb-2 mt-8 text-black">Latest recorded date: {latestHumidityDate}</h2>
          <h2 className="text-lg font-semibold mb-2 text-black">Humidity Chart</h2>
          <BarChart data={latestHumidityData} />
        </div>
      </div>
    </div>
  );
  
};

export default Dashboard;

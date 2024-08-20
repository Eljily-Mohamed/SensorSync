// TemperatureComponent.js
import React, { useEffect, useState } from "react";
import LineChart from "../components/LineChart.js";
import Loading from "../components/Loading.js";
import socket from "../services/socketService.js";
import { insertTemperature, fetchTemperatureData } from "../services/apiService.js";
import { convertToFahrenheit } from "../utils/temperatureUtils.js";

const TemperatureComponent = () => {
  const [realTimeData, setRealTimeData] = useState([]);
  const [databaseData, setDatabaseData] = useState([]);
  const [hasConnection, setHasConnection] = useState(false);
  const [temperatureUnit, setTemperatureUnit] = useState("celsius");
  const [selectedDate, setSelectedDate] = useState({ day: "", month: "", year: "" });
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    const fetchData = (data) => {
      // Check if data format matches the sensor output
      if (!data.startsWith("temperature: ")) {
        return; // Ignore data if not in expected format
      }
      // Extract the numerical part (ignoring "temperature: " and " C")
      const valueString = data.substring(13, data.length - 2);
      // Convert the extracted string to a float
      const value = parseFloat(valueString);
      // Use the extracted value for further processing
      console.log("Extracted temperature:", value);
      setRealTimeData((prevData) => [...prevData, value]);
      if (!hasConnection) {
        setHasConnection(true);
      }
      insertTemperature(value);
    };

    if (socket) {
      if (realTimeData.length === 0) {
        socket.emit("message", "t");
      }

      socket.on("serialData", fetchData);

      socket.on("disconnect", () => {
        setHasConnection(false);
      });

      return () => {
        socket.off("serialData", fetchData);
        socket.off("connect");
        socket.off("disconnect");
      };
    }
  }, [realTimeData]);

  const handleUnitChange = (unit) => {
    setTemperatureUnit(unit);
  };

  const handleDateUpdate = async () => {
    const { day, month, year } = selectedDate;
    const date = `${day}-${month}-${year}`;

    try {
      setLoading(true);
      const temperatureData = await fetchTemperatureData(date);
      let temperatures = temperatureData.map((item) => item.value);
      //Conversion des températures en fonction de l'unité sélectionnée
      if (temperatureUnit === "fahrenheit") {
        temperatures = temperatures.map(convertToFahrenheit);
        console.log(temperatures);
      } 
      setDatabaseData(temperatures);
    } catch (error) {
      console.error("Error updating temperature data:", error);
    } finally {
      setLoading(false);
    }
  };

  const handleInputChange = (event) => {
    const { name, value } = event.target;
    setSelectedDate((prevDate) => ({
      ...prevDate,
      [name]: value,
    }));
  };
  
  
  return (
    <div className={`grid ${hasConnection ? "grid-rows-2" : "rows-cols-1"} gap-6 h-screen ml-[180px]`}>
      {hasConnection && (
        <div className="p-4 rounded-lg">
          <p className="text-lg font-semibold mb-2">Real Time Temperature</p>
          <LineChart data={realTimeData} unit='celsius' />
        </div>
      )}
      <div className="p-4 rounded-lg">
        <p className="text-lg font-semibold mb-2">Temperature from Database</p>
        <div className="flex mb-4">
          <input
            type="text"
            value={selectedDate.day}
            onChange={handleInputChange}
            className="border p-2 rounded mr-2"
            placeholder="Day"
            name="day"
          />
          <input
            type="text"
            value={selectedDate.month}
            onChange={handleInputChange}
            className="border p-2 rounded mr-2"
            placeholder="Month"
            name="month"
          />
          <input
            type="text"
            value={selectedDate.year}
            onChange={handleInputChange}
            className="border p-2 rounded mr-2"
            placeholder="Year"
            name="year"
          />
          <button onClick={handleDateUpdate} className="bg-blue-500 text-white px-4 py-2 rounded">
            Update Data
          </button>
        </div>
        <div className="flex items-center mb-4">
          <input
            type="radio"
            id="celsius"
            name="temperatureUnit"
            value="celsius"
            checked={temperatureUnit === "celsius"}
            onChange={() => handleUnitChange("celsius")}
            className="mr-2"
          />
          <label htmlFor="celsius" className="mr-4">
            Celsius
          </label>
          <input
            type="radio"
            id="fahrenheit"
            name="temperatureUnit"
            value="fahrenheit"
            checked={temperatureUnit === "fahrenheit"}
            onChange={() => handleUnitChange("fahrenheit")}
            className="mr-2"
          />
          <label htmlFor="fahrenheit">Fahrenheit</label>
        </div>
        {loading ? <Loading /> : <LineChart data={databaseData} unit={temperatureUnit} />}
      </div>
    </div>
  );
};

export default TemperatureComponent;

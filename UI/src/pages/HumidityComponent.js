import React, { useEffect, useState } from "react";
import BarChart from "../components/BarChart.js";
import Loading from "../components/Loading.js";
import socket from "../services/socketService.js";
import { insertHumidity, fetchHumidityData } from "../services/apiService.js";

const HumidityComponent = () => {
  const [realTimeData, setRealTimeData] = useState([]);
  const [databaseData, setDatabaseData] = useState([]);
  const [hasConnection, setHasConnection] = useState(false);
  const [selectedDate, setSelectedDate] = useState({ day: "", month: "", year: "" });
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    const fetchData = (data) => {
      // Check if data format matches the sensor output
      if (!data.startsWith("humidity: ")) {
        return; // Ignore data if not in expected format
      }
      // Extract the numerical part (ignoring "humidity: ")
      const valueString = data.split(":")[1]; // Split on whitespace and get second element
      // Convert the extracted string to a float
      const value = parseFloat(valueString);
      // Use the extracted value for further processing
      console.log("Extracted temperature:", value);
      setRealTimeData((prevData) => [...prevData, value]);
      if (!hasConnection) {
        setHasConnection(true);
      }
      insertHumidity(value);
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

  const handleDateUpdate = async () => {
    const { day, month, year } = selectedDate;
    const date = `${day}-${month}-${year}`;

    try {
      setLoading(true);
      const humidityData = await fetchHumidityData(date);
      let humidity = humidityData.map((item) => item.value);
      setDatabaseData(humidity);
    } catch (error) {
      console.error("Error updating humidity data:", error);
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
          <p className="text-lg font-semibold mb-2">Real Time Humidity</p>
          <BarChart data={realTimeData} />
        </div>
      )}
      <div className="p-4 rounded-lg">
        <p className="text-lg font-semibold mb-2">Humidity from Database</p>
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
        {loading ? <Loading /> : <BarChart data={databaseData} />}
      </div>
    </div>
  );
};

export default HumidityComponent;

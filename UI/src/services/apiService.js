const insertTemperature = async (data) => {
  try {
    const response = await fetch("http://localhost:5000/api/insert-temperature", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ value: data }),
    });
    if (!response.ok) {
      throw new Error("Failed to insert temperature data");
    }
  } catch (error) {
    console.error("Error inserting temperature data:", error);
  }
};

const fetchTemperatureData = async (date) => {
  try {
    const response = await fetch(`http://localhost:5000/api/temperature-data/${date}`);
    if (!response.ok) {
      throw new Error("Failed to fetch temperature data");
    }
    const temperatureData = await response.json();
    return temperatureData;
  } catch (error) {
    console.error("Error fetching temperature data:", error);
    return [];
  }
};

const insertHumidity = async (data) => {
  try {
    const response = await fetch("http://localhost:5000/api/insert-humidity", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ value: data }),
    });
    if (!response.ok) {
      throw new Error("Failed to insert humidity data");
    }
  } catch (error) {
    console.error("Error inserting humidity data:", error);
  }
};

const fetchHumidityData = async (date) => {
  try {
    const response = await fetch(`http://localhost:5000/api/humidity-data/${date}`);
    if (!response.ok) {
      throw new Error("Failed to fetch humidity data");
    }
    const humidityData = await response.json();
    return humidityData;
  } catch (error) {
    console.error("Error fetching humidity data:", error);
    return [];
  }
};

const fetchLatestTemperatureData = async () => {
  try {
    const response = await fetch("http://localhost:5000/api/latest-temperature");
    if (!response.ok) {
      throw new Error("Failed to fetch latest temperature data");
    }
    const latestTemperatureData = await response.json();
    return latestTemperatureData;
  } catch (error) {
    console.error("Error fetching latest temperature data:", error);
    return null;
  }
};

const fetchLatestHumidityData = async () => {
  try {
    const response = await fetch("http://localhost:5000/api/latest-humidity");
    if (!response.ok) {
      throw new Error("Failed to fetch latest humidity data");
    }
    const latestHumidityData = await response.json();
    return latestHumidityData;
  } catch (error) {
    console.error("Error fetching latest humidity data:", error);
    return null;
  }
};

// Function to fetch the maximum temperature value
const fetchMaxTemperature = async () => {
  try {
    const response = await fetch("http://localhost:5000/api/max-temperature");
    if (!response.ok) {
      throw new Error("Failed to fetch maximum temperature data");
    }
    const maxTemperatureData = await response.json();
    return maxTemperatureData;
  } catch (error) {
    console.error("Error fetching maximum temperature data:", error);
    return null;
  }
};

// Function to fetch the minimum temperature value
const fetchMinTemperature = async () => {
  try {
    const response = await fetch("http://localhost:5000/api/min-temperature");
    if (!response.ok) {
      throw new Error("Failed to fetch minimum temperature data");
    }
    const minTemperatureData = await response.json();
    return minTemperatureData;
  } catch (error) {
    console.error("Error fetching minimum temperature data:", error);
    return null;
  }
};

// Function to fetch the maximum humidity value
const fetchMaxHumidity = async () => {
  try {
    const response = await fetch("http://localhost:5000/api/max-humidity");
    if (!response.ok) {
      throw new Error("Failed to fetch maximum humidity data");
    }
    const maxHumidityData = await response.json();
    return maxHumidityData;
  } catch (error) {
    console.error("Error fetching maximum humidity data:", error);
    return null;
  }
};

// Function to fetch the minimum humidity value
const fetchMinHumidity = async () => {
  try {
    const response = await fetch("http://localhost:5000/api/min-humidity");
    if (!response.ok) {
      throw new Error("Failed to fetch minimum humidity data");
    }
    const minHumidityData = await response.json();
    return minHumidityData;
  } catch (error) {
    console.error("Error fetching minimum humidity data:", error);
    return null;
  }
};

export { insertTemperature, fetchTemperatureData, insertHumidity, fetchHumidityData, fetchLatestTemperatureData, fetchLatestHumidityData, fetchMaxTemperature, fetchMinTemperature, fetchMaxHumidity, fetchMinHumidity };

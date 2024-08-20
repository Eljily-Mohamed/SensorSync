import express from 'express';
import { insertTemperatureData, getTemperatureDataByDate, insertHumidityData, getHumidityDataByDate, getLatestTemperatureData , getLatestHumidityData, getMaxTemperature, getMinTemperature, getMaxHumidity, getMinHumidity } from './database.js';

const router = express.Router();

// Endpoint to insert temperature data
router.post('/insert-temperature', (req, res) => {
  const { value } = req.body;

  if (!value) {
    return res.status(400).json({ error: 'Temperature value is required' });
  }

  insertTemperatureData(value);

  return res.status(200).json({ message: 'Temperature data inserted successfully' });
});

// Endpoint to retrieve temperature data by date
router.get('/temperature-data/:date', async (req, res) => {
  const { date } = req.params;

  try {
    const temperatureData = await getTemperatureDataByDate(date);
    return res.status(200).json(temperatureData);
  } catch (error) {
    console.error('Error fetching temperature data by date:', error);
    return res.status(500).json({ error: 'Internal server error' });
  }
});

// Endpoint to insert humidity data
router.post('/insert-humidity', (req, res) => {
  const { value } = req.body;

  if (!value) {
    return res.status(400).json({ error: 'Humidity value is required' });
  }

  insertHumidityData(value);

  return res.status(200).json({ message: 'Humidity data inserted successfully' });
});

// Endpoint to retrieve humidity data by date
router.get('/humidity-data/:date', async (req, res) => {
  const { date } = req.params;

  try {
    const humidityData = await getHumidityDataByDate(date);
    return res.status(200).json(humidityData);
  } catch (error) {
    console.error('Error fetching humidity data by date:', error);
    return res.status(500).json({ error: 'Internal server error' });
  }
});

// Endpoint to retrieve the most recent temperature data
router.get('/latest-temperature', async (req, res) => {
  try {
    const latestTemperatureData = await getLatestTemperatureData();
    if (latestTemperatureData) {
      return res.status(200).json(latestTemperatureData);
    } else {
      return res.status(404).json({ error: 'No temperature data available' });
    }
  } catch (error) {
    console.error('Error fetching latest temperature data:', error);
    return res.status(500).json({ error: 'Internal server error' });
  }
});

// Endpoint to retrieve the most recent humidity data
router.get('/latest-humidity', async (req, res) => {
  try {
    const latestHumidityData = await getLatestHumidityData();
    if (latestHumidityData) {
      return res.status(200).json(latestHumidityData);
    } else {
      return res.status(404).json({ error: 'No humidity data available' });
    }
  } catch (error) {
    console.error('Error fetching latest humidity data:', error);
    return res.status(500).json({ error: 'Internal server error' });
  }
});

// Endpoint to retrieve the maximum temperature value
router.get('/max-temperature', async (req, res) => {
  try {
    const maxTemperature = await getMaxTemperature();
    return res.status(200).json({ maxTemperature });
  } catch (error) {
    console.error('Error fetching maximum temperature:', error);
    return res.status(500).json({ error: 'Internal server error' });
  }
});

// Endpoint to retrieve the minimum temperature value
router.get('/min-temperature', async (req, res) => {
  try {
    const minTemperature = await getMinTemperature();
    return res.status(200).json({ minTemperature });
  } catch (error) {
    console.error('Error fetching minimum temperature:', error);
    return res.status(500).json({ error: 'Internal server error' });
  }
});

// Endpoint to retrieve the maximum humidity value
router.get('/max-humidity', async (req, res) => {
  try {
    const maxHumidity = await getMaxHumidity();
    return res.status(200).json({ maxHumidity });
  } catch (error) {
    console.error('Error fetching maximum humidity:', error);
    return res.status(500).json({ error: 'Internal server error' });
  }
});

// Endpoint to retrieve the minimum humidity value
router.get('/min-humidity', async (req, res) => {
  try {
    const minHumidity = await getMinHumidity();
    return res.status(200).json({ minHumidity });
  } catch (error) {
    console.error('Error fetching minimum humidity:', error);
    return res.status(500).json({ error: 'Internal server error' });
  }
});

export default router;

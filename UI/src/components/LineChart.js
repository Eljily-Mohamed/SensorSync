import React, { useEffect, useRef } from "react";
import Chart from "chart.js/auto";

const LineChart = ({ data, unit }) => {
  const chartRef = useRef(null);
  const chartInstance = useRef(null);

  useEffect(() => {
    if (chartRef.current && data.length > 0) {
      if (chartInstance.current) {
        chartInstance.current.data.labels = data.map((_, index) => index);
        chartInstance.current.data.datasets[0].data = data;
        chartInstance.current.options.scales.y.title.text = `Temperature (${unit})`;
        chartInstance.current.options.scales.y.min = unit === "celsius" ? -5 : 0;
        chartInstance.current.options.scales.y.max = unit === "celsius" ? 40 : 100;
        chartInstance.current.update();
      } else {
        const ctx = chartRef.current.getContext("2d");
        chartInstance.current = new Chart(ctx, {
          type: "line",
          data: {
            labels: data.map((_, index) => index),
            datasets: [
              {
                label: "Temperature",
                data: data,
                borderColor: "rgba(255, 99, 132, 1)",
                backgroundColor: "rgba(255, 99, 132, 0.2)",
                borderWidth: 1,
              },
            ],
          },
          options: {
            scales: {
              y: {
                min: unit === "celsius" ? -5 : 0,
                max: unit === "celsius" ? 40 : 100,
                display: true,
                title: {
                  display: true,
                  text: `Temperature (${unit})`,
                },
              },
            },
          },
        });
      }
    }
  }, [data, unit]);

  return <canvas ref={chartRef}></canvas>;
};

export default LineChart;

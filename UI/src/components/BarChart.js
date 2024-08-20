import React, { useEffect, useRef } from "react";
import Chart from "chart.js/auto";

const BarChart = ({ data }) => {
  const chartRef = useRef(null);
  const chartInstance = useRef(null);

  useEffect(() => {
    if (chartRef.current && data.length > 0) {
      if (chartInstance.current) {
        chartInstance.current.data.labels = data.map((_, index) => index);
        chartInstance.current.data.datasets[0].data = data;
        chartInstance.current.update();
      } else {
        const ctx = chartRef.current.getContext("2d");
        chartInstance.current = new Chart(ctx, {
          type: "bar",
          data: {
            labels: data.map((_, index) => index),
            datasets: [
              {
                label: "Value",
                data: data,
                backgroundColor: "rgba(54, 162, 235, 0.5)",
                borderColor: "rgba(54, 162, 235, 1)",
                borderWidth: 1,
              },
            ],
          },
          options: {
            scales: {
              y: {
                beginAtZero: true,
                title: {
                  display: true,
                  text: "Percentage (%)",
                },
              },
            },
          },
        });
      }
    }
  }, [data]);

  return <canvas ref={chartRef}></canvas>;
};

export default BarChart;

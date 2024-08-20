import React, { useState, useEffect } from "react";
import { LayoutDashboard, Sun, SnowflakeIcon, Palette, Settings, LifeBuoy } from "lucide-react";
import { ToastContainer, toast } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';
import Sidebar, { SidebarItem } from "./components/Sidebar.js";
import Dashboard from "./pages//Dashboard.js";
import TemperatureComponent from "./pages/TemperatureComponent.js";
import HumidityComponent from "./pages/HumidityComponent.js";
import ColorComponent from "./pages/ColorComponent.js";
import socket from "./services/socketService.js";


function App() {
  const [activeComponent, setActiveComponent] = useState("Dashboard");
  const [isDropdownOpen, setIsDropdownOpen] = useState(false);
  const [selectedConnection, setSelectedConnection] = useState('');


  useEffect(() => {
    if (selectedConnection) {
      const firstChar = selectedConnection.charAt(0);
      socket.emit("message",`${firstChar}`);
    }
  }, [selectedConnection]);

  const renderComponent = () => {
    switch (activeComponent) {
      case "Temperature":
        return <TemperatureComponent />;
      case "Humidity":
        return <HumidityComponent />;
      case "Color":
        return <ColorComponent />;
      case "Dashboard":
      default:
        return <Dashboard />;
    }
  };

  const ChangeActive = (text) => {
    console.log(text);
    setActiveComponent(text);
  }

  const toggleDropdown = () => {
    setIsDropdownOpen(!isDropdownOpen);
  };

  const handleConnectionChange = (event) => {
    const connection = event.target.value;
    setSelectedConnection(connection);
    setIsDropdownOpen(false);
    toast.dismiss();
  
    fetch('http://localhost:5000/api/set-communication', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ method: connection }),
    })
      .then(response => {
        if (!response.ok) {
          throw new Error('Network response was not ok');
        }
        return response.json(); // Parse the JSON response
      })
      .then(data => {
        // Ensure data.message exists and is a string
        if (typeof data.message === 'string') {
          // Notify the user using toast from react-toastify
          toast.success(data.message, {
            autoClose: 1000 // Close after 1 second
          });
        } else {
          // Handle unexpected response format gracefully
          throw new Error('Unexpected response format');
        }
      })
      .catch(error => {
        console.error('Error:', error);
        toast.error(`Failed to set up ${connection}`, {
          autoClose: 1000 // Close after 1 second
        });
      });
  };
  

  return (
    <>
      <div className="flex">
        <Sidebar>
          <SidebarItem icon={<LayoutDashboard size={20} />} text="Dashboard" active={activeComponent === "Dashboard"} onClick={ChangeActive}/>
          <SidebarItem icon={<Sun size={20} />} text="Temperature" active={activeComponent === "Temperature"} onClick={ChangeActive} />
          <SidebarItem icon={<SnowflakeIcon size={20} />} text="Humidity" active={activeComponent === "Humidity"} onClick={ChangeActive}/>
          <SidebarItem icon={<Palette size={20} />} text="Color" active={activeComponent === "Color"} onClick={ChangeActive} />
          <hr className="my-3" />
          <div className="relative">
      <SidebarItem 
        icon={<Settings size={20} />} 
        text="Settings" 
        onClick={toggleDropdown} 
        className="cursor-pointer flex items-center space-x-2 p-2 hover:bg-gray-100 rounded-md transition duration-150 ease-in-out"
      />
      {isDropdownOpen && (
        <div className="absolute right-0 mt-2 w-48 bg-white rounded-md shadow-lg ring-1 ring-black ring-opacity-5 z-20">
          <div className="py-2 text-center text-sm font-semibold text-gray-800 border-b">Communication Choice</div>
          <div className="py-1 px-2 flex flex-col items-center">
            <label className="inline-flex items-center mb-2">
              <input
                type="radio"
                value="ST-LINK"
                checked={selectedConnection === "ST-LINK"}
                onChange={handleConnectionChange}
                className="form-radio h-4 w-4 text-blue-600 focus:ring-blue-500 border-gray-300"
              />
              <span className="ml-2 text-gray-700">ST-LINK</span>
            </label>
            <label className="inline-flex items-center">
              <input
                type="radio"
                value="ZEGBEE"
                checked={selectedConnection === "ZEGBEE"}
                onChange={handleConnectionChange}
                className="form-radio h-4 w-4 text-blue-600 focus:ring-blue-500 border-gray-300"
              />
              <span className="ml-2 text-gray-700">ZEGBEE</span>
            </label>
          </div>
        </div>
      )}
      <ToastContainer />
    </div>
        </Sidebar>
        <div className="w-5/6 p-5">
          {console.log(activeComponent)}
          {renderComponent()}
        </div>
      </div>
    </>
  );
}

export default App;

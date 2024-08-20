import React from "react";
import './css/bulbLight.css'; // Assuming you have styles for the bulb light component

const BulbLight = ({ colorCode, turnOn }) => {


  const getInvertedColor = (color) => {
    // Assuming colorCode is in the format "#RRGGBB"
    const invertedColor = '#' + colorCode.slice(1).split('').map(c => (15 - parseInt(c, 16)).toString(16)).join('');
    return invertedColor;
  };
  
  return (
    // <div className={turnOn ? "block" : "block night"}>
    <div className="block night">
        <div className="container">
          <div className="bulb-light">
            <div id="light" style={{
            backgroundColor: colorCode,
            boxShadow: `0px 0px 500px rgba(${colorCode}, 1),
              inset 0px 0px 500px 90px rgba(${colorCode}, 0.42)`
           }} />

            <div id="bulb">
              <div className="bulb-top" style={{ background: colorCode }}>
                <div className="reflection" />
              </div>
              <div className="bulb-middle-1" style={{ borderTop: `55px solid ${colorCode}` }} />
              <div className="bulb-middle-2" style={{ borderTop: `50px solid ${colorCode}` }} />
              <div className="bulb-middle-3" style={{ borderTop: `30px solid ${colorCode}` }} />
              <div className="bulb-bottom" style={{ background: colorCode }} />
            </div>

            <div id="base">
              <div className="screw-top" style={{ background: colorCode }} />
              <div className="screw-a"/>
              <div className="screw-b"/>
              <div className="screw-a"/>
              <div className="screw-b"/>
              <div className="screw-a"/>
              <div className="screw-b"/>
              <div className="screw-c"/>
              <div className="screw-d"/>
            </div>
          </div>
        </div>
    </div>
  );
};

export default BulbLight;

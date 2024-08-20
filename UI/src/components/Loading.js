import React from "react";

const Loading = () => {
  return (
    <div className="flex items-center justify-center h-full">
      <svg className="animate-spin h-8 w-8 text-[#ff305d]" xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24">
        <circle className="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" strokeWidth="4"></circle>
        <path className="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.373A8 8 0 0012 20v4c-6.627 0-12-5.373-12-12h4zm5.373-2A8 8 0 0020 12h4c0 6.627-5.373 12-12 12v-4zm-2-5.373c1.334 0 2.598.259 3.767.725l-2.318 3.662A7.954 7.954 0 0112 19.373V16h-4v3.373c-.715.12-1.456.192-2.209.192-3.314 0-6.317-1.37-8.465-3.564l3.662-2.318c.465 1.169.725 2.433.725 3.767zM7.378 4.894l2.318 3.662A7.954 7.954 0 015.373 12H2c0-3.314 1.37-6.317 3.564-8.465z"></path>
      </svg>
    </div>
  );
};

export default Loading;

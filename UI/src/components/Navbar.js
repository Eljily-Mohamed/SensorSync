import React from 'react';

const Navbar = () => {
  return (
    <nav className="bg-gray-100 shadow-lg h-16 flex items-center justify-between px-6">
      <i className='bx bx-menu text-gray-800 cursor-pointer'></i>
      <form className="max-w-xs w-full ml-6 mr-auto">
        <div className="relative">
          <input
            type="search"
            placeholder="Search..."
            className="block w-full py-2 pl-10 pr-3 border border-gray-300 rounded-full focus:outline-none focus:border-blue-500 focus:ring focus:ring-blue-200"
          />
          <button type="submit" className="absolute top-0 right-0 mt-2 mr-3">
            <i className='bx bx-search text-white text-xl'></i>
          </button>
        </div>
      </form>
      <input type="checkbox" id="switch-mode" className="hidden" />
      <label htmlFor="switch-mode" className="switch-mode"></label>
    </nav>
  );
};

export default Navbar;

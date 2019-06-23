module Plain = {
  [@react.component]
  let make = (~children, ~onClick) => {
    let className = "bg-transparent hover:bg-gray-200 focus:outline-none hover:outline-none border border-gray-300 text-xs text-gray-800 font-bold outline-none pl-2 pr-4 rounded-sm inline-flex items-center";
    <button onClick className>
      <svg
        className="fill-current text-gray-600 w-6 h-6 mr-1"
        viewBox="0 0 20 20">
        <polygon id="Rectangle-161" points="4 4 16 10 4 16" />
      </svg>
      <span> children </span>
    </button>;
  };
};
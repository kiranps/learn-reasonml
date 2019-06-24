[@react.component]
let make = (~children, ~icon, ~onClick) => {
  let className = "bg-transparent hover:bg-gray-200 focus:outline-none hover:outline-none  border border-gray-300 text-xs text-gray-800 font-bold outline-none pl-2 pr-4 rounded-sm inline-flex items-center mx-1";
  <button onClick className>
    <Icon name=icon />
    <span> children </span>
  </button>;
};
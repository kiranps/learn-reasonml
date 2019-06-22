[@react.component]
let make = (~children) => {
  <div
    className="fixed flex w-full h-12 bg-gray-100 border-b border-gray-200 p-3 justify-between z-10">
    children
  </div>;
};
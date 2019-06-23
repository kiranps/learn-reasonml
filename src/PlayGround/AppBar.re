[@react.component]
let make = (~children) => {
  <div
    className="fixed flex w-full h-12 bg-white border-b border-gray-200 p-2 justify-start z-10">
    children
  </div>;
};
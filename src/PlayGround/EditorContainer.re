[@react.component]
let make = (~children) =>
  <div
    className="fixed inline-block pt-12 w-1/2 border-r border-gray-300 overflow-hidden top-0 bottom-0 left-0">
    children
  </div>;
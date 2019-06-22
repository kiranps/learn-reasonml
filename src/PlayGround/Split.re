[@react.component]
let make = (~className, ~children) =>
  <div className> {children[0]} {children[1]} </div>;
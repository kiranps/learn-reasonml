open Css;

let app_style =
  style([
    position(absolute),
    top(px(0)),
    bottom(px(0)),
    height(px(35)),
    width(pct(100.0)),
    boxSizing(borderBox),
    borderBottom(px(1), `solid, hex("bfbfbf")),
    backgroundColor(hex("ededed")),
  ]);

[@react.component]
let make = _ => {
  <div className=app_style> {"" |> React.string} </div>;
};
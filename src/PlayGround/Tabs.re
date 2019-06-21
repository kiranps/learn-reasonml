open Css;

let tab = active =>
  style([
    display(inlineBlock),
    width(px(100)),
    boxSizing(borderBox),
    backgroundColor(hex("f7f7f7")),
    textAlign(center),
    height(pct(100.)),
    padding(px(6)),
    fontSize(px(13)),
    userSelect(none),
    borderStyle(none),
    borderBottom(px(2), solid, active ? blue : transparent),
    marginRight(px(2)),
    cursor(`pointer),
  ]);

let header =
  style([
    width(pct(100.)),
    height(px(27)),
    boxSizing(borderBox),
    backgroundColor(hex("f7f7f7")),
    borderBottom(px(1), solid, rgba(158, 158, 158, 0.41)),
  ]);

[@react.component]
let make = (~children) => {
  let (activeTab, setTab) = React.useState(() => 0);
  let handleClick = value => setTab(_ => value);

  <>
    <div className=header>
      {children
       |> Array.mapi((i, (x, _)) =>
            <div
              key={string_of_int(i)}
              className={tab(i === activeTab)}
              onClick={_ => handleClick(i)}>
              {React.string(x)}
            </div>
          )
       |> React.array}
    </div>
    {children
     |> Array.mapi((i, (_, x)) =>
          i === activeTab ? <div key={string_of_int(i)}> x </div> : React.null
        )
     |> React.array}
  </>;
};

module Pane = {
  [@react.component]
  let make = (~name) => <div> {React.string(name)} </div>;
};
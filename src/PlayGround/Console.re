open Css;

let preview_style =
  style([
    position(absolute),
    display(inlineBlock),
    top(px(0)),
    bottom(px(0)),
    right(px(0)),
    width(pct(50.0)),
    padding(px(15)),
    boxSizing(borderBox),
  ]);

let interseptor: (string => unit) => unit = [%bs.raw
  {|
    ((cb) => {
        var log = console.log;
        console.log = (text) => {
            log(text);
            cb(text);
        }
    })
  |}
];

[@react.component]
let make = _ => {
  let (logData, setLogData) = React.useState(() => "");

  React.useEffect0(() => {
    let logger = data => setLogData(_ => data);
    let _ = interseptor(logger);
    Some(() => ());
  });

  <div className=preview_style> {logData |> React.string} </div>;
};
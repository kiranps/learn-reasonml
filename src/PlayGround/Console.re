open Css;

let preview_style =
  style([
    position(absolute),
    display(inlineBlock),
    top(px(36)),
    bottom(px(0)),
    right(px(0)),
    width(pct(50.0)),
    padding(px(15)),
    boxSizing(borderBox),
    backgroundColor(hex("f7f7f7")),
    overflow(auto),
  ]);

let log_info_style = style([paddingBottom(px(5))]);

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
  let (log, updateLog) = React.useState(() => "");

  React.useEffect0(() => {
    let logger = data => updateLog(log => log ++ data ++ "\n");
    let _ = interseptor(logger);
    Some(() => ());
  });

  <div className=preview_style>
    {log
     |> Js.String.split("\n")
     |> Array.mapi((i, x) =>
          <div key={string_of_int(i)} className=log_info_style>
            {React.string(x)}
          </div>
        )
     |> React.array}
  </div>;
};
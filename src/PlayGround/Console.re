open Css;

type t;
[@bs.new] [@bs.module "ansi_up"] external ansiUp: unit => t = "default";
[@bs.send] external ansi_to_html: (t, string) => string = "";

let ansiInstance = ansiUp();

type html = {. [@bs.set] "__html": string};

let toHtmlMarkup = text => {
  let foo = Js.Obj.empty();
  foo##__html #= text;
  foo;
};

let toAnsiHtml = text => text |> ansi_to_html(ansiInstance) |> toHtmlMarkup;

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

module ConsoleInfo = {
  let log_info_style = style([paddingBottom(px(5))]);

  [@react.component]
  let make = (~text) =>
    <div
      className=log_info_style
      dangerouslySetInnerHTML={toAnsiHtml(text)}
    />;
};

[@react.component]
let make = _ => {
  let (log, registerLogger, _, _, _, _) = Logger.useLogger();

  React.useEffect0(() => {
    let _ = registerLogger();
    Some(() => ());
  });

  <div className=preview_style>
    {log
     |> Js.String.split("\n")
     |> Array.mapi((i, log) =>
          <ConsoleInfo key={string_of_int(i)} text=log />
        )
     |> React.array}
  </div>;
};
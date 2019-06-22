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
    top(px(35)),
    bottom(px(0)),
    right(px(0)),
    width(pct(50.0)),
    boxSizing(borderBox),
    backgroundColor(white),
    overflow(auto),
  ]);

let message_style = type_ =>
  style([
    fontSize(px(13)),
    paddingTop(px(3)),
    paddingBottom(px(3)),
    paddingLeft(px(8)),
    borderBottom(px(1), solid, hex("f4f4f4")),
    (
      switch (type_) {
      | "log" => "757575"
      | "error" => "ca0101"
      | "warn" => "#ffb400"
      | "problem" => "#ffb400"
      | _ => "7575757"
      }
    )
    |> hex
    |> color,
  ]);

module Message = {
  let log_info_style = style([paddingBottom(px(5))]);

  [@react.component]
  let make = (~text, ~type_) =>
    <div
      className={message_style(type_)}
      dangerouslySetInnerHTML={toAnsiHtml(text)}
    />;
};

[@react.component]
let make = _ => {
  let (logs, _, _) = Logger.useLogger();

  React.useEffect0(() => Some(() => ()));

  let log = type_ =>
    logs
    |> List.filter((x: Logger.t) => x.message !== "")
    |> List.filter((x: Logger.t) =>
         type_ === "all" ? x.type_ !== "problems" : x.type_ === "problems"
       )
    |> List.mapi((i, x: Logger.t) =>
         <Message key={string_of_int(i)} type_={x.type_} text={x.message} />
       )
    |> List.rev
    |> Array.of_list
    |> React.array;

  <div className=preview_style>
    <Tabs>
      {[|
         ("Console", <Tabs.Pane> {log("all")} </Tabs.Pane>),
         ("Problems", <Tabs.Pane> {log("problems")} </Tabs.Pane>),
       |]}
    </Tabs>
  </div>;
};
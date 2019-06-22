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

let message_style = type_ =>
  "text-sm pt-1 pb-2 pl-4 border-b border-gray-300 "
  ++ (
    switch (type_) {
    | "log" => "text-gray-900"
    | "error" => "text-red-600"
    | "warn" => "text-orange-600"
    | "problem" => "text-orange-600"
    | _ => "text-gray-900"
    }
  );

module Message = {
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

  <div
    className="fixed inline-block pt-12 w-1/2 overflow-hidden top-0 bottom-0 right-0">
    <Tabs>
      {[|
         ("Console", <Tabs.Pane> {log("all")} </Tabs.Pane>),
         ("Problems", <Tabs.Pane> {log("problems")} </Tabs.Pane>),
       |]}
    </Tabs>
  </div>;
};
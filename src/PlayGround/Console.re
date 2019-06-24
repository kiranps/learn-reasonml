let message_style = type_ =>
  "text-xs pt-1 pb-1 pl-4 border-b border-gray-300 "
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
    <div className={message_style(type_)}> {React.string(text)} </div>;
};

[@react.component]
let make = _ => {
  let (logs, _) = Logger.useLogger();

  React.useEffect0(() => Some(() => ()));

  let log = type_ =>
    logs
    |> List.filter((x: Logger.t) => x.message !== "")
    |> List.filter((x: Logger.t) =>
         type_ === "all" ? x.type_ !== "problem" : x.type_ === "problem"
       )
    |> List.mapi((i, x: Logger.t) =>
         <Message key={string_of_int(i)} type_={x.type_} text={x.message} />
       )
    |> List.rev
    |> Array.of_list
    |> React.array;

  <div
    className="fixed inline-block w-1/2 overflow-hidden pt-12 top-0 bottom-0 right-0">
    <Tabs>
      [|
        ("Console", <Tabs.Pane> {log("all")} </Tabs.Pane>),
        ("Problems", <Tabs.Pane> {log("problem")} </Tabs.Pane>),
      |]
    </Tabs>
  </div>;
};
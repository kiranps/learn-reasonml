type t;
[@bs.module] external monaco: 'a = "monaco-editor/esm/vs/editor/editor.api";
[@bs.get] external editor: 'a => 'b = "";
[@bs.send] external create: ('b, Dom.element, 'c) => t = "";
[@bs.send] external onDidChangeModelContent: (t, 'evt => unit) => unit = "";
[@bs.send] external getValue: t => string = "";
[@bs.send] external setValue: (t, string) => unit = "";

type monacoOptions = {
  .
  lineNumbersMinChars: int,
  value: string,
  language: string,
  fontSize: int,
  theme: string,
  minimap: {. enabled: bool},
};

[@react.component]
let make = (~value, ~onChange) => {
  let divRef = React.useRef(Js.Nullable.null);
  let mRef = React.useRef(Js.Nullable.null);

  React.useEffect0(() => {
    switch (divRef |> React.Ref.current |> Js.Nullable.toOption) {
    | Some(ele) =>
      let options = {
        "lineNumbersMinChars": 3,
        "value": value,
        "fontSize": 13,
        "language": "rust",
        "theme": "vs",
        "minimap": {
          "enabled": false,
        },
      };

      let monacoRef = create(editor(monaco), ele, options);
      React.Ref.setCurrent(mRef, Js.Nullable.return(monacoRef));
      onDidChangeModelContent(monacoRef, _ => onChange(getValue(monacoRef)));
    | None => ()
    };

    Some(() => ());
  });

  React.useEffect1(
    () => {
      let monacoRef = mRef |> React.Ref.current |> Js.Nullable.toOption;
      switch (monacoRef) {
      | Some(e) =>
        if (getValue(e) !== value) {
          setValue(e, value);
        }
      | None => ()
      };
      Some(() => ());
    },
    [|value|],
  );

  <div className="w-full h-full" ref={ReactDOMRe.Ref.domRef(divRef)} />;
};
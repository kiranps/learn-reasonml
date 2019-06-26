Utils.require("codemirror/keymap/vim");
Utils.require("codemirror/lib/codemirror.css");
Utils.require("codemirror/addon/dialog/dialog.css");
Utils.require("codemirror/mode/rust/rust");

module CM = {
  [@bs.deriving abstract]
  type cmprops = {
    lineNumbers: bool,
    value: string,
    mode: string,
  };

  type cm;
  type evt;
  type cmds = {. [@bs.set] "save": evt => unit};

  [@bs.module]
  external init: (option(Dom.element), cmprops) => cm = "codemirror";
  [@bs.module "codemirror"] external commands: cmds = "";
  [@bs.send] external getValue: evt => string = "";
  [@bs.send] external setValue: (cm, string) => unit = "";
  [@bs.send] external on: (cm, string, evt => unit) => unit = "";
};

[@react.component]
let make = (~value, ~onChange=?, ~onSave=?, ~className="") => {
  let divRef = React.useRef(Js.Nullable.null);
  let cmRef = React.useRef(Js.Nullable.null);

  React.useEffect0(() => {
    let options = CM.cmprops(~lineNumbers=true, ~mode="rust", ~value);

    let cm =
      CM.init(Js.Nullable.toOption(React.Ref.(current(divRef))), options);

    React.Ref.(cm |> Js.Nullable.return |> setCurrent(cmRef));

    switch (onSave) {
    | None => ()
    | Some(handleSave) =>
      CM.commands##save #= (e => handleSave(CM.getValue(e)))
    };

    switch (onChange) {
    | None => ()
    | Some(handleChange) =>
      CM.on(
        cm,
        "change",
        e => {
          let value_ = CM.getValue(e);
          if (value_ !== value) {
            handleChange(value_);
          };
        },
      )
    };

    Some(() => ());
  });

  React.useEffect1(
    () => {
      let cm = Js.Nullable.toOption(React.Ref.current(cmRef));
      switch (cm) {
      | Some(ele) => CM.setValue(ele, value)
      | None => ()
      };
      Some(() => ());
    },
    [|value|],
  );

  <EditorContainer>
    <div className ref={ReactDOMRe.Ref.domRef(divRef)} />
  </EditorContainer>;
};
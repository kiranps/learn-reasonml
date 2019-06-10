Utils.require("codemirror/keymap/vim");
Utils.require("codemirror/lib/codemirror.css");
Utils.require("codemirror/addon/dialog/dialog.css");
Utils.require("codemirror/mode/javascript/javascript");

module CM = {
  [@bs.deriving abstract]
  type cmprops = {
    lineNumbers: bool,
    value: string,
    mode: string,
    keyMap: string,
  };

  type cm;
  type evt;
  type cmds = {. [@bs.set] "save": evt => unit};

  [@bs.module]
  external init: (option(Dom.element), cmprops) => cm = "codemirror";
  [@bs.module "codemirror"] external commands: cmds = "";
  [@bs.send] external getValue: evt => string = "";
  [@bs.send] external on: (cm, string, evt => unit) => unit = "";
};

[@react.component]
let make =
  React.memo((~value, ~onChange=?, ~onSave=?, ~className="") => {
    let divRef = React.useRef(Js.Nullable.null);

    React.useEffect1(
      () => {
        let options =
          CM.cmprops(
            ~lineNumbers=true,
            ~keyMap="vim",
            ~mode="javascript",
            ~value,
          );

        let cm =
          CM.init(
            Js.Nullable.toOption(React.Ref.(current(divRef))),
            options,
          );

        switch (onSave) {
        | None => ()
        | Some(handleSave) =>
          CM.commands##save #= (e => handleSave(CM.getValue(e)))
        };

        switch (onChange) {
        | None => ()
        | Some(handleChange) =>
          CM.on(cm, "change", e => handleChange(CM.getValue(e)))
        };

        Some(() => ());
      },
      [||],
    );

    <EditorContainer>
      <div className ref={ReactDOMRe.Ref.domRef(divRef)} />
    </EditorContainer>;
  });
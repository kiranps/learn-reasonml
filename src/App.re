[@react.component]
let make = _ => {
  let (code, setCode) = React.useState(() => "");

  React.useEffect1(() => Some(() => ()), [||]);

  let handleSave =
    React.useCallback1(
      value => {
        let jsCode = value |> Bs.compile;
        Js.log(jsCode);
        Js.log(value);
      },
      [||],
    );

  <CodeMirror className=Styles.editor value=code onSave=handleSave />;
};
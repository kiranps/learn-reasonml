open Css;

let editor_style =
  style([
    height(pct(100.0)),
    width(pct(100.0)),
    selector("& > div", [height(pct(100.0))]),
  ]);

[@react.component]
let make = _ => {
  let (code, setCode) = React.useState(() => "");

  let handleSave =
    React.useCallback1(
      value => {
        let (type_, result) = value |> Bs.compile;
        switch (type_) {
        | Fail => Js.log(result)
        | Success =>
          Utils.eval(result);
          setCode(_ => result);
        };
      },
      [||],
    );

  <>
    <CodeMirror className=editor_style value=code onSave=handleSave />
    <Preview />
  </>;
};
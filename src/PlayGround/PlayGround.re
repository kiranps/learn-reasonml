open Css;

let sample_code = {|let add = (x, y) => x + y;
let addTen = 10 |> add;
let _ = 12 |> addTen |> Js.log;|};

let editor_style =
  style([
    height(pct(100.0)),
    width(pct(100.0)),
    selector("& > div", [height(pct(100.0))]),
  ]);

[@react.component]
let make = _ => {
  let (code, setCode) = React.useState(() => sample_code);

  let handleSave =
    React.useCallback0(value => {
      let (type_, result) = value |> Bs.compile;
      switch (type_) {
      | Fail => Js.log(result)
      | Success =>
        Utils.eval(result);
        setCode(_ => result);
      };
    });

  <>
    <AppBar />
    <CodeMirror className=editor_style value=code onSave=handleSave />
    <Console />
  </>;
};
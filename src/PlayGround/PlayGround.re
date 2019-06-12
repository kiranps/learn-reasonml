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

  React.useEffect0(() => {
    Js.Promise.(
      Http.getText("/chapters/02-basic-types")
      |> then_(text => text |> (text => setCode(_ => text) |> resolve))
    );
    Some(() => ());
  });

  let handleSave =
    React.useCallback0(value => {
      let (type_, result) = value |> Bs.compile;
      switch (type_) {
      | Fail => Js.log(result)
      | Success =>
        let _ = Utils.eval(result);
        let _ = setCode(_ => result);
        ();
      };
    });

  <>
    <AppBar />
    {code !== ""
       ? <CodeMirror className=editor_style value=code onSave=handleSave />
       : <div> {React.string("Loading")} </div>}
    <Console />
  </>;
};
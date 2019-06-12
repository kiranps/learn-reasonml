open Css;

let editor_style =
  style([
    height(pct(100.0)),
    width(pct(100.0)),
    selector("& > div", [height(pct(100.0))]),
  ]);

[@react.component]
let make = (~exercise_name: string) => {
  let (code, setCode) = React.useState(() => None);

  React.useEffect0(() => {
    Js.Promise.(
      Http.getText("/exercises/" ++ exercise_name)
      |> then_(text => text |> (text => setCode(_ => Some(text)) |> resolve))
    );
    Some(() => ());
  });

  let handleSave =
    React.useCallback0(value => {
      let (type_, result) = value |> Compiler.compile;
      switch (type_) {
      | Fail => Js.log(result)
      | Success =>
        let _ = Utils.eval(result);
        let _ = setCode(_ => Some(result));
        ();
      };
    });

  <>
    <AppBar />
    {switch (code) {
     | Some(value) =>
       <CodeMirror className=editor_style value onSave=handleSave />
     | None => <div> {React.string("Loading")} </div>
     }}
    <Console />
  </>;
};
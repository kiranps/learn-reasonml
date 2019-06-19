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
  let (_, _, _, switchErrorToWarn, revertErrorToWarn, _) = Logger.useLogger();

  React.useEffect0(() => {
    Js.Promise.(
      Http.getText("/exercises/" ++ exercise_name)
      |> then_(text => text |> (text => setCode(_ => Some(text)) |> resolve))
    );
    Some(() => ());
  });

  let handleChange = React.useCallback0(value => setCode(_ => Some(value)));

  let handleRun = _ => {
    switch (code) {
    | Some(string) =>
      /* let _ = switchErrorToWarn(); */
      let result = string |> Compiler.compile;
      /* let _ = revertErrorToWarn(); */

      switch (result) {
      | Fail(error) => Js.log(error)
      | Success(message) => Utils.eval(message)
      };
    | None => Js.log("error")
    };
  };

  let handleSave = value => {
    let result = value |> Compiler.compile;

    switch (result) {
    | Fail(message) => Js.log("error")
    | Success(message) => Utils.eval(message)
    };
  };

  <>
    <AppBar>
      <button onClick=handleRun> {React.string("run")} </button>
    </AppBar>
    {switch (code) {
     | Some(value) =>
       <CodeMirror
         className=editor_style
         value
         onChange=handleChange
         onSave=handleSave
       />
     | None => <div> {React.string("Loading")} </div>
     }}
    <Console />
  </>;
};
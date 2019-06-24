let wrapInExports = code => "(function(exports) {" ++ code ++ "})({})";

[@react.component]
let make = (~exercise_name: string) => {
  let (code, setCode) = React.useState(() => None);
  let (_, clearConsole) = Logger.useLogger();

  React.useEffect0(() => {
    let _ = clearConsole();
    Js.Promise.(
      Http.getText("/exercises/" ++ exercise_name)
      |> then_(text => text |> (text => setCode(_ => Some(text)) |> resolve))
    );
    Some(() => ());
  });

  let handleChange = React.useCallback0(value => setCode(_ => Some(value)));

  let handleCompile = reasonCode => {
    let result = reasonCode |> Compiler.compile;

    switch (result) {
    | Compiled({code, warnings}) =>
      warnings |> Array.iter(Js.Console.warn);
      code |> wrapInExports |> Utils.eval;
    | CompileError(error) => error |> Compiler.formatCompileError |> Js.log
    | ReasonParseError(error) => Js.log(error)
    };
  };

  let handleRun = _ =>
    switch (code) {
    | Some(reasonCode) => handleCompile(reasonCode)
    | None => Js.log("error")
    };

  let handleSave = handleCompile;

  <>
    <AppBar>
      <Button.Plain onClick=handleRun> {React.string("Run")} </Button.Plain>
    </AppBar>
    {
      switch (code) {
      | Some(value) =>
        <CodeMirror
          className="h-full w-full"
          value
          onChange=handleChange
          onSave=handleSave
        />
      | None => <div> {React.string("Loading")} </div>
      }
    }
    <Console />
  </>;
};
let wrapInExports = code => "(function(exports) {" ++ code ++ "})({})";

[@react.component]
let make = (~exercise_name=?) => {
  let (code, setCode) = React.useState(() => None);
  let (_, clearConsole) = Logger.useLogger();
  let cmRef = React.useRef("");

  React.useEffect0(() => {
    let _ = clearConsole();
    switch (exercise_name) {
    | Some(name) =>
      Js.Promise.(
        Http.getText("/exercises/" ++ name)
        |> then_(text =>
             text
             |> (
               text =>
                 {
                   setCode(_ => Some(text));
                   React.Ref.setCurrent(cmRef, text);
                 }
                 |> resolve
             )
           )
      );
      ();
    | None => setCode(_ => Some(""))
    };

    Some(() => ());
  });

  let handleChange =
    React.useCallback0(value => value |> React.Ref.setCurrent(cmRef));

  let handleCompile =
    React.useCallback0(reasonCode => {
      let result = reasonCode |> Compiler.compile;

      switch (result) {
      | Compiled({code, warnings}) =>
        warnings |> Array.iter(Js.Console.warn);
        code |> wrapInExports |> Utils.eval;
      | CompileError(error) => error |> Compiler.formatCompileError |> Js.log
      | ReasonParseError(error) => Js.log(error)
      };
    });

  let handleRun = _ => cmRef |> React.Ref.current |> handleCompile;

  let handleReformat = _ => {
    let _ =
      cmRef
      |> React.Ref.current
      |> BsBox.Reason.format
      |> (value => setCode(_ => Some(value)));
    ();
  };

  let handleSave = React.useCallback0(handleCompile);

  <>
    <AppBar>
      <Button icon="play" onClick=handleRun> {React.string("Run")} </Button>
      <Button icon="format" onClick=handleReformat>
        {React.string("Pretty print")}
      </Button>
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
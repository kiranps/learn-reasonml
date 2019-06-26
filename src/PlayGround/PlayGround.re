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
      let _ =
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

  /* let handleChange =
     React.useCallback0(value => value |> React.Ref.setCurrent(cmRef)); */
  /* let handleChange =
     React.useCallback0(value => setCode(_ => value)); */

  let handleChange = value =>
    switch (code) {
    | Some(code) =>
      if (code !== value) {
        setCode(_ => Some(value));
      }
    | None => ()
    };

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

  /* let handleRun = _ => cmRef |> React.Ref.current |> handleCompile; */
  let handleRun = _ =>
    switch (code) {
    | Some(code) => code |> handleCompile
    | None => ()
    };

  /* let handlePrettyPrint = _ =>
     cmRef
     |> React.Ref.current
     |> BsBox.Reason.format
     |> (value => setCode(_ => Some(value))); */

  let handlePrettyPrint = _ =>
    switch (code) {
    | Some(code) =>
      code |> BsBox.Reason.format |> (value => setCode(_ => Some(value)))
    | None => ()
    };

  /* let handleSave = React.useCallback0(handleCompile); */

  let handleSave = handleCompile;

  <>
    <AppBar>
      <Button icon="play" onClick=handleRun> {React.string("Run")} </Button>
      <Button icon="format" onClick=handlePrettyPrint>
        {React.string("Pretty print")}
      </Button>
    </AppBar>
    <div
      className="fixed inline-block pt-12 w-1/2 border-r border-gray-300 overflow-hidden top-0 bottom-0 left-0">
      {
        switch (code) {
        | Some(value) => <Monaco value onChange=handleChange />
        | None => <div> {React.string("Loading")} </div>
        }
      }
    </div>
    <Console />
  </>;
};
/* <CodeMirror
     className="h-full w-full"
     value
     onChange=handleChange
     onSave=handleSave
   /> */
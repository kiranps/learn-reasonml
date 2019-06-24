type t =
  | Compiled(BsBox.success)
  | CompileError(BsBox.error)
  | ReasonParseError(BsBox.Reason.error);

let addTestModule = code => ReModules.test ++ code;

let compile = reasonCode =>
  reasonCode
  |> addTestModule
  |> BsBox.Reason.toOcaml
  |> (
    fun
    | Ocaml(code) =>
      code
      |> BsBox.compile
      |> BsBox.(
           (
             fun
             | Ok(output) => Compiled(output)
             | Error(`BsCompileError(err)) => CompileError(err)
           )
         )
    | ReasonParseError(message) => ReasonParseError(message)
  );

let formatCompileError = (error: BsBox.error) =>
  string_of_int(error.from.line)
  ++ ":"
  ++ string_of_int(error.from.column)
  ++ "-"
  ++ string_of_int(error.until.line)
  ++ ":"
  ++ string_of_int(error.until.column)
  ++ "  "
  ++ error.message;
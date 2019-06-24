type location = {
  line: int,
  column: int,
};

type success = {
  code: string,
  warnings: array(string),
};

type error = {
  message: string,
  from: location,
  until: location,
  console: array(string),
};

module Reason = {
  [@bs.deriving abstract]
  type parse_location = {
    endLine: int,
    endLineEndChar: int,
    startLine: int,
    startLineStartChar: int,
  };

  [@bs.deriving abstract]
  type parse_error = {
    location: parse_location,
    message: string,
  };

  type error = {
    message: string,
    from: location,
    until: location,
  };

  type reason_parse_out =
    | Ocaml(string)
    | ReasonParseError(error);

  type p;

  let printRE: p => string = [%bs.raw {| window.printRE |}];
  let printML: p => string = [%bs.raw {| window.printML |}];
  let parseRE: string => p = [%bs.raw {| window.parseRE |}];

  external unsafeAsError: Js.Exn.t => parse_error = "%identity";

  let toOcaml: string => reason_parse_out =
    reasonCode =>
      try (reasonCode |> parseRE |> printML |> (code => Ocaml(code))) {
      | Js.Exn.Error(error) =>
        let errObj = error |> unsafeAsError;
        let location = errObj->location;
        let message = errObj->message;

        let error = {
          message,
          from: {
            line: location->startLine,
            column: location->startLineStartChar,
          },
          until: {
            line: location->endLine,
            column: location->endLineEndChar,
          },
        };

        ReasonParseError(error);
      };

  let format = reasonCode => reasonCode |> parseRE |> printRE;
};

module InternalResult = {
  type t = {. "_type": Js.nullable(string)};

  type internalSuccess = {. "js_code": string};

  type internalError = {
    .
    "js_error_msg": string,
    "row": int,
    "column": int,
    "endRow": int,
    "endColumn": int,
    "text": string,
  };

  external unsafeFromJson: Js.Json.t => t = "%identity";
  external unsafeAsSuccess: t => internalSuccess = "%identity";
  external unsafeAsError: t => internalError = "%identity";

  let toResult = jsObj =>
    switch (Js.Nullable.toOption(jsObj##_type)) {
    | Some("error") =>
      let error = jsObj |> unsafeAsError;
      Belt.Result.Error({
        message: error##text,
        from: {
          line: error##row,
          column: error##column,
        },
        until: {
          line: error##endRow,
          column: error##endColumn,
        },
        console: [||],
      });
    | _ =>
      Belt.Result.Ok({
        code: (jsObj |> unsafeAsSuccess)##js_code,
        warnings: [||],
      })
    };
};

%raw
{|
    function _captureConsoleOutput(f) {
      const capture = (...args) => args.forEach(argument => errors.push(argument + `\n`));

      let errors = [];
      let res;

      if ((typeof process !== "undefined") && process.stdout && process.stdout.write) {
        const _stdoutWrite = process.stdout.write; // errors are written to stdout
        const _stderrWrite = process.stderr.write; // warnings are written to stderr ...
        process.stdout.write = capture;
        process.stderr.write = capture;

        res = f();

        process.stdout.write = _stdoutWrite;
        process.stderr.write = _stderrWrite;
      } else {
        const _consoleLog = console.log;     // errors are written to console.log
        const _consoleError = console.error; // warnings are written to console.error (at least it's consistently incnsistent)
        console.log = capture;
        console.error = capture;

        res = f();

        console.log = _consoleLog;
        console.error = _consoleError;
      }

      return [res, errors ? errors : ""];
    }
  |};

[@bs.val]
external _captureConsoleOutput: (unit => 'a) => ('a, array(string)) = "";

[@bs.val] [@bs.scope "ocaml"] external compile: string => 'b = "";
let compile = code => {
  let (json, consoleOutput) = _captureConsoleOutput(() => code |> compile);

  json
  |> InternalResult.unsafeFromJson
  |> InternalResult.toResult
  |> Belt.Result.(
       fun
       | Ok({code}) => Ok({code, warnings: consoleOutput})
       | Error(error) =>
         Error(`BsCompileError({...error, console: consoleOutput}))
     );
};

[@bs.val] [@bs.scope "ocaml"]
external loadModule: (string, string, string, string) => unit = "load_module";
let loadModule = (~name, ~cmi, ~cmj) =>
  loadModule({j|/static/cmis/$name.cmi|j}, cmi, {j|$name.cmj|j}, cmj);
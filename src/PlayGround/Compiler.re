[@bs.deriving abstract]
type t = {
  [@bs.optional]
  js_code: string,
  [@bs.optional]
  column: int,
  [@bs.optional]
  endColumn: int,
  [@bs.optional]
  endRow: int,
  [@bs.optional]
  js_error_msg: string,
  [@bs.optional]
  row: int,
  [@bs.optional]
  text: string,
  [@bs.optional] [@bs.as "type"]
  type_: string,
};

[@bs.deriving abstract]
type t_location = {
  endLine: int,
  endLineEndChar: int,
  startLine: int,
  startLineStartChar: int,
};

[@bs.deriving abstract]
type t_parse_error = {
  location: t_location,
  message: string,
};

type p;
type out_type =
  | Fail(string)
  | Success(string);

type t_reason_parsed =
  | AST(p)
  | ParseFailed(string);

type t_compile_output =
  | Compiled(t)
  | ParseFailed(string);

let compile_super_errors_ppx_v2: string => t = [%bs.raw
  {| window.ocaml.compile_super_errors_ppx_v2 |}
];

let printML: p => string = [%bs.raw {| window.printML |}];

let parseRE: string => p = [%bs.raw {| window.parseRE |}];

let addTestModule = code => ReModules.test ++ code;

let wrapInExports = code => "(function(exports) {" ++ code ++ "})({})";

let parseRE_: string => t_reason_parsed =
  reasonCode =>
    try (reasonCode |> parseRE |> (value => AST(value))) {
    | Js.Exn.Error(e) =>
      Js.log(e);
      switch (Js.Exn.message(e)) {
      | Some(message) => ParseFailed(message)
      | None => ParseFailed("parse error")
      };
    };

let compile = reasonCode =>
  reasonCode
  |> addTestModule
  |> parseRE_
  |> (
    parsedCode =>
      switch (parsedCode) {
      | AST(value) =>
        value
        |> printML
        |> compile_super_errors_ppx_v2
        |> (result => Compiled(result))
      | ParseFailed(message) => ParseFailed(message)
      }
  )
  |> (
    output =>
      switch (output) {
      | Compiled(out) =>
        switch (js_codeGet(out)) {
        | Some(value) => Success(wrapInExports(value))
        | None => Fail("compile error")
        }
      | ParseFailed(message) => Fail(message)
      }
  );
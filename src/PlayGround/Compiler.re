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

type out_type =
  | Fail(string)
  | Success(string);

let compile_super_errors_ppx_v2: string => t = [%bs.raw
  {| window.ocaml.compile_super_errors_ppx_v2 |}
];

let printML: string => string = [%bs.raw {| window.printML |}];

let parseRE: string => 'a = [%bs.raw {| window.parseRE |}];

let prependTestModule = code => ReModules.test ++ code;

let wrapInExports = code => "(function(exports) {" ++ code ++ "})({})";

let compile = reasonCode => {
  let output =
    reasonCode
    |> prependTestModule
    |> parseRE
    |> printML
    |> compile_super_errors_ppx_v2;

  switch (js_codeGet(output)) {
  | None => Fail("error")
  | Some(value) => Success(wrapInExports(value))
  };
};
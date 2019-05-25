[@bs.deriving abstract]
type js_out = {
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
  [@bs.as "type"]
  type_: string,
};

let compile_super_errors_ppx_v2: string => js_out = [%bs.raw
  {| window.ocaml.compile_super_errors_ppx_v2 |}
];

let compile = code => {
  let c = compile_super_errors_ppx_v2(code);
  Js.log(js_codeGet(c));
  c;
};

let wrapInExports = code => "(function(exports) {" ++ code ++ "})({})";
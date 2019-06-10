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
  [@bs.optional] [@bs.as "type"]
  type_: string,
};

type out_type =
  | Fail
  | Success;

let compile_super_errors_ppx_v2: string => js_out = [%bs.raw
  {| window.ocaml.compile_super_errors_ppx_v2 |}
];

let compile_to_ocaml: string => string = [%bs.raw
  {|
    function(reasonCode) {
      return reasonCode;
    }
  |}
];

let jsCalculate: (array(int), int) => int = [%bs.raw
  {|
 function (numbers, scaleFactor) {
   var result = 0;
   numbers.forEach(number => {
     result += number;
   });
   return result * scaleFactor;
 }
|}
];

let wrapInExports = code => "(function(exports) {" ++ code ++ "})({})";

let compile = code => {
  let output = code |> compile_super_errors_ppx_v2;
  Js.log(code);
  let _ = code => code |> compile_to_ocaml |> Js.log;

  switch (js_codeGet(output)) {
  | None =>
    switch (type_Get(output)) {
    | None => (Fail, "error")
    | Some(type_) => (Fail, "error")
    }
  | Some(value) => (Success, value |> wrapInExports)
  };
};
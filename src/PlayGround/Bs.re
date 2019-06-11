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

let testModule = {|
module Test {
  type t = {
    expr: bool,
    desc: string,
  };

  let create = ((expr, desc)) => {expr, desc};

  let test = t => t.expr;

  let lightRed = x => {j|\u001b[91m$x\u001b[39m|j};

  let lightGreen = x => {j|\u001b[92m$x\u001b[39m|j};

  let runAll = xs => {
    let toString = t =>
      "Testing "
      ++ t.desc
      ++ "... "
      ++ (
        switch (t.expr) {
        | false => lightRed("failed")
        | true => lightGreen("passed")
        }
      );
    let count = List.length(xs);
    let tests = List.map(create, xs);
    let failures =
      tests |> List.map(test) |> List.filter(y => ! y) |> List.length;
    let descriptions = List.map(toString, tests);
    Js.log("");
    List.iteri(
      (i, x) => Js.log(string_of_int(i + 1) ++ ". " ++ x),
      descriptions,
    );
    Js.log("");
    if (failures == 0) {
      Js.log(lightGreen("All tests passed!"));
    } else {
      Js.log(
        lightRed(
          string_of_int(failures)
          ++ " out of "
          ++ string_of_int(count)
          ++ " tests failed.",
        ),
      );
    };
    Js.log("");
  };
}


|};

let mathModule = {|
module Math {
  let add = (x, y) => x + y;
}
|};

type out_type =
  | Fail
  | Success;

let compile_super_errors_ppx_v2: string => js_out = [%bs.raw
  {| window.ocaml.compile_super_errors_ppx_v2 |}
];

let printML: string => string = [%bs.raw {| window.printML |}];

let parseRE: string => 'a = [%bs.raw {| window.parseRE |}];

let compile_to_ocaml: string => string = [%bs.raw
  {|
    function(reasonCode) {
    window.printML(window.parseRE(reasonCode))
    }
  |}
];

let wrapTestModule = code => testModule ++ code;

let wrapMathModule = code => mathModule ++ code;

let wrapInExports = code => "(function(exports) {" ++ code ++ "})({})";

let compile = code => {
  /* let output =
     code |> wrapMathModule |> parseRE |> printML |> compile_super_errors_ppx_v2; */
  let output =
    code |> wrapTestModule |> parseRE |> printML |> compile_super_errors_ppx_v2;

  switch (js_codeGet(output)) {
  | None =>
    switch (type_Get(output)) {
    | None => (Fail, "error")
    | Some(type_) => (Fail, "error")
    }
  | Some(value) => (Success, value |> wrapInExports)
  };
};
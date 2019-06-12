let test = {|
module Test {
  type t = {
    expr: bool,
    desc: string,
  };

  let create = ((expr, desc)) => {expr, desc};

  let test = t => t.expr;

  let runAll = xs => {
    let toString = t =>
      "Testing "
      ++ t.desc
      ++ "... "
      ++ (
        switch (t.expr) {
        | false => "failed"
        | true => "passed"
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
      Js.log("All tests passed!");
    } else {
      Js.log(
        string_of_int(failures)
        ++ " out of "
        ++ string_of_int(count)
        ++ " tests failed.",
    );
    };
    Js.log("");
  };
}

|};
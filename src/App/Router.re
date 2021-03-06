[@react.component]
let make = () => {
  let hash = ReasonReactRouter.useUrl().hash;
  let path = Js.String.split("/", hash);

  switch (path) {
  | [|""|] => <Exercises />
  | [|"", "exercises"|] => <Exercises />
  | [|"", "exercise", exercise_name|] => <PlayGround exercise_name />
  | [|"", "playground"|] => <PlayGround />
  | _ => <NotFoundPage />
  };
};
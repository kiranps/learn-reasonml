type t = {
  type_: string,
  message: string,
};
type state = {
  all: list(t),
  isBsLogging: bool,
};

type action =
  | Log(string)
  | Warn(string)
  | BsWarn(string)
  | ToggeBsWarn
  | Error(string)
  | UnCaughtError(string)
  | Clear;

[@bs.deriving abstract]
type error_u = {message: string};

/* console bindings */
[@bs.val] external window: 'w = "";
[@bs.val] external console: 'b = "";
[@bs.set] external setLog: ('b, 'a => unit) => unit = "log";
[@bs.set] external setWarn: ('b, 'a => unit) => unit = "warn";
[@bs.set] external setError: ('b, 'a => unit) => unit = "error";
[@bs.set] external setOnError: ('w, 'a => unit) => unit = "onerror";
let consoleLog: 'a => unit = [%bs.raw {| console.log |}];
let consoleWarn: 'a => unit = [%bs.raw {| console.warn |}];
let consoleError: 'a => unit = [%bs.raw {| console.error |}];
/* console bindings */

let intersept = cb => {
  setLog(
    console,
    (text: string) => {
      consoleLog(text);
      cb(Log(text ++ "\n"));
    },
  );

  setWarn(
    console,
    (text: string) => {
      consoleWarn(text);
      cb(Warn(text ++ "\n"));
    },
  );

  setError(
    console,
    (text: string) => {
      consoleError(text);
      cb(Error(text ++ "\n"));
    },
  );
};

module Context = {
  type t = {
    state,
    dispatch: action => unit,
  };

  include ReactContext.Make({
    type context = t;
    let defaultValue = {
      state: {
        isBsLogging: false,
        all: [],
      },
      dispatch: _ => (),
    };
  });
};

module Provider = {
  [@react.component]
  let make = (~children) => {
    let (state, dispatch) =
      React.useReducer(
        (state, action) =>
          switch (action) {
          | ToggeBsWarn => {...state, isBsLogging: !state.isBsLogging}
          | Log(value) => {
              ...state,
              all: [{type_: "log", message: value}, ...state.all],
            }
          | Error(value) => {
              ...state,
              all:
                state.isBsLogging ?
                  [{type_: "problems", message: value}, ...state.all] :
                  [{type_: "error", message: value}, ...state.all],
            }
          | Warn(value) => {
              ...state,
              all: [{type_: "warn", message: value}, ...state.all],
            }
          | Clear => {...state, all: []}
          },
        {all: [], isBsLogging: false},
      );

    React.useEffect0(() => {
      intersept(dispatch);
      Some(() => ());
    });

    <Context.Provider value={state, dispatch}> children </Context.Provider>;
  };
};

let useLogger = () => {
  let ctx = React.useContext(Context.ctx);

  (ctx.state.all, () => ctx.dispatch(Clear));
};
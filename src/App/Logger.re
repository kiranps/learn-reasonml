type state = {
  log: string,
  warn: string,
  error: string,
};

type action =
  | Log(string)
  | Warn(string)
  | Error(string)
  | Clear;

module Context = {
  type t = {
    state,
    dispatch: action => unit,
  };

  include ReactContext.Make({
    type context = t;
    let defaultValue = {
      state: {
        log: "",
        error: "",
        warn: "",
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
          | Log(value) => {...state, log: state.log ++ value}
          | Error(value) => {...state, error: state.error ++ value}
          | Warn(value) => {...state, warn: state.warn ++ value}
          | Clear => {...state, log: "", warn: "", error: ""}
          },
        {log: "", error: "", warn: ""},
      );
    <Context.Provider value={state, dispatch}> children </Context.Provider>;
  };
};

/* console bindings */
[@bs.scope "console"] [@bs.val] external log: 'a => unit = "";
[@bs.scope "console"] [@bs.val] external warn: 'a => unit = "";
[@bs.scope "console"] [@bs.val] external error: 'a => unit = "";
[@bs.val] external console: 'b = "";
[@bs.set] external setLog: ('b, 'a => unit) => unit = "log";
[@bs.set] external setWarn: ('b, 'a => unit) => unit = "warn";
[@bs.set] external setError: ('b, 'a => unit) => unit = "error";
let consoleLog: 'a => unit = [%bs.raw {| console.log |}];
let consoleWarn: 'a => unit = [%bs.raw {| console.warn |}];
let consoleError: 'a => unit = [%bs.raw {| console.error |}];
/* console bindings */

let intersept_ = cb => {
  setLog(
    console,
    (text: string) => {
      consoleLog(text);
      cb(Log(text ++ "\n"));
      ();
    },
  );
  setWarn(
    console,
    (text: string) => {
      consoleWarn(text);
      cb(Warn(text ++ "\n"));
      ();
    },
  );
  setError(
    console,
    (text: string) => {
      consoleError(text);
      cb(Log(text ++ "\n"));
      ();
    },
  );
};

let switchInterseptErrortoWarn_ = cb => {
  setLog(
    console,
    (text: string) => {
      consoleLog(text);
      cb(Log(text ++ "\n"));
      ();
    },
  );

  setWarn(
    console,
    (text: string) => {
      consoleWarn(text);
      cb(Warn(text ++ "\n"));
      ();
    },
  );

  setError(
    console,
    (text: string) => {
      consoleWarn(text);
      cb(Warn(text ++ "\n"));
      ();
    },
  );
};

let deregisterLogger_ = () => {
  setLog(console, consoleLog);
  setWarn(console, consoleWarn);
  setError(console, consoleError);
};

let useLogger = () => {
  let ctx = React.useContext(Context.ctx);

  let registerLogger = () => {
    let _ = intersept_(ctx.dispatch);
    ();
  };

  let deregisterLogger = () => {
    let _ = deregisterLogger_();
    ();
  };

  let switchInterseptErrortoWarn = () => {
    let _ = switchInterseptErrortoWarn_(ctx.dispatch);
    ();
  };

  let revertInterseptErrortoWarn = registerLogger;

  let clearConsole = () => ctx.dispatch(Clear);

  (
    ctx.state.log,
    registerLogger,
    deregisterLogger,
    switchInterseptErrortoWarn,
    revertInterseptErrortoWarn,
    clearConsole,
  );
};
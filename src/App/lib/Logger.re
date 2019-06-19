type state = {
  log: string,
  warn: string,
  error: string,
  uncaught: string,
};

type action =
  | Log(string)
  | Warn(string)
  | Error(string)
  | UnCaughtError(string)
  | Clear;

[@bs.deriving abstract]
type error_u = {message: string};

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
        uncaught: "",
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
          | Error(value) => {...state, log: state.log ++ value}
          | Warn(value) => {...state, log: state.log ++ value}
          | UnCaughtError(value) => {...state, log: state.log ++ value}
          | Clear => {...state, log: "", warn: "", error: "", uncaught: ""}
          },
        {log: "", error: "", warn: "", uncaught: ""},
      );
    <Context.Provider value={state, dispatch}> children </Context.Provider>;
  };
};

/* console bindings */
[@bs.scope "console"] [@bs.val] external log: 'a => unit = "";
[@bs.scope "console"] [@bs.val] external warn: 'a => unit = "";
[@bs.scope "console"] [@bs.val] external error: 'a => unit = "";
[@bs.scope "window"] [@bs.val] external onerror: 'a => unit = "";
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
      cb(Error(text ++ "\n"));
      ();
    },
  );
};

let switchInterseptErrortoWarn_ = cb => {
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

  React.useEffect0(() => {
    /* setOnError(window, (err: string) => ctx.dispatch(UnCaughtError(err))); */
    setOnError(window, (err: error_u) =>
      ctx.dispatch(UnCaughtError(messageGet(err)))
    );
    Some(() => ());
  });

  let registerLogger = () => {
    intersept_(ctx.dispatch);
  };

  let deregisterLogger = () => {
    deregisterLogger_();
  };

  let switchInterseptErrortoWarn = () => {
    switchInterseptErrortoWarn_(ctx.dispatch);
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
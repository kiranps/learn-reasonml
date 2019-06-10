Utils.require("./index.css");
ReactDOMRe.renderToElementWithId(<App />, "root");

/* write hook for console intersept */

/* type t;

   [@bs.send] external log: (t, string) => unit = "log";
   let console: t = [%bs.raw {| console |}];

   log(console, "hello"); */
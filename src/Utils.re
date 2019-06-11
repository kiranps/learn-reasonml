[@bs.val] external require: string => unit = "require";
[@bs.val] external eval: string => unit = "eval";

let receiveMessage: (string => unit) => unit = [%bs.raw
  {|
   (handleMessage) => window.addEventListener("message", e => handleMessage(e.data), false);
|}
];

let postMessage: string => unit = [%bs.raw {| window.postMessage |}];
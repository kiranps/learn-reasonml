[@react.component]
let make = (~children) => {
  let (activeTab, setTab) = React.useState(() => 0);
  let handleClick = value => setTab(_ => value);
  let tab = "w-24 h-full pt-2 text-center text-xs select-none cursor-pointer hover:bg-gray-200";
  let active_tab = tab ++ " border-b-2 border-blue-500";

  <>
    <div
      className="flex flex-start w-full h-8 bg-ghost white border-b border-gray-200">
      {
        children
        |> Array.mapi((i, (x, _)) =>
             <div
               key={string_of_int(i)}
               className={i === activeTab ? active_tab : tab}
               onClick={_ => handleClick(i)}>
               {React.string(x)}
             </div>
           )
        |> React.array
      }
    </div>
    {
      children
      |> Array.mapi((i, (_, x)) =>
           i === activeTab ?
             <div key={string_of_int(i)}> x </div> : React.null
         )
      |> React.array
    }
  </>;
};

module Pane = {
  [@react.component]
  let make = (~children) => <div> children </div>;
};
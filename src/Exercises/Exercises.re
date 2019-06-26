[@react.component]
let make = _ => {
  let (chapters, setChapters) = React.useState(() => [||]);

  React.useEffect0(() => {
    Js.Promise.(
      Http.get("/exercises/exercises.json")
      |> then_(json =>
           json
           |> Json.Decode.(array(string))
           |> (json => setChapters(_ => json) |> resolve)
         )
    );
    Some(() => ());
  });

  <div>
    <nav
      className="fixed flex w-full h-12 bg-white border-b border-gray-200 p-2 justify-start z-10"
    />
    <div
      className="flex content-start flex-wrap justify-around items-center w-full p-12">
      {
        chapters
        |> Array.mapi((i, text) =>
             <a key={string_of_int(i)} href={"#/exercise/" ++ text}>
               <div
                 className="max-w-sm rounded overflow-hidden mt-4 border border-gray-400  bg-white p-2">
                 <div className="px-6 py-4">
                   <div className="font-bold text-xl mb-2">
                     {React.string(text)}
                   </div>
                   <p className="text-gray-700 text-base">
                     {
                       React.string(
                         "Lorem ipsum dolor sit amet, consectetur adipisicing elit. Voluptatibus quia, nulla! Maiores et perferendis eaque, exercitationem praesentium nihil.
",
                       )
                     }
                   </p>
                 </div>
               </div>
             </a>
           )
        |> React.array
      }
    </div>
  </div>;
};
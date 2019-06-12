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
    {chapters
     |> Array.mapi((i, text) =>
          <div key={string_of_int(i)}>
            <a href={"#/exercise/" ++ text}> {React.string(text)} </a>
          </div>
        )
     |> React.array}
  </div>;
};
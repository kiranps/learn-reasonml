let get = url => Js.Promise.(Fetch.fetch(url) |> then_(Fetch.Response.json));

let getText = url =>
  Js.Promise.(Fetch.fetch(url) |> then_(Fetch.Response.text));

let post = (url, data) => {
  let contentType = {"Content-Type": "application/json"};
  let method_ = Fetch.Post;
  let headers = contentType |> Fetch.HeadersInit.make;
  let body = data |> Js.Json.stringify |> Fetch.BodyInit.make;

  Js.Promise.(
    Fetch.fetchWithInit(
      url,
      Fetch.RequestInit.make(~method_, ~body, ~headers, ()),
    )
    |> then_(Fetch.Response.json)
  );
};
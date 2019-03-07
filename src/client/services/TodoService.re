let getAll = (~onSuccess, ~onError=?, ()) => {
  Js.Promise.(
    Axios.get("/api/todos")
    |> then_(res =>
         onSuccess(res##data |> Json.Decode.list(Todo.decode)) |> resolve
       )
    |> catch(err =>
         onError
         |> Belt.Option.mapWithDefault(_, Js.Promise.resolve(), errorFn =>
              errorFn(err)
            )
       )
  );
};

let add = (~payload, ~onSuccess, ~onError=?, ()) => {
  Js.Promise.(
    Axios.postData("/api/todos", payload)
    |> then_(res => onSuccess(res##data |> Todo.decode) |> resolve)
    |> catch(err =>
         onError
         |> Belt.Option.mapWithDefault(_, Js.Promise.resolve(), errorFn =>
              errorFn(err)
            )
       )
  );
};

let delete = (~id, ~onSuccess, ~onError=?, ()) => {
  Js.Promise.(
    Axios.delete("/api/todos/" ++ id)
    |> then_(_ => onSuccess() |> resolve)
    |> catch(err =>
         onError
         |> Belt.Option.mapWithDefault(_, Js.Promise.resolve(), errorFn =>
              errorFn(err)
            )
       )
  );
};

let edit = (newTodo: Todo.t, ~onSuccess, ~onError=?, ()) => {
  Js.Promise.(
    Axios.patchData("/api/todos/" ++ newTodo.id, Todo.tToJs(newTodo))
    |> then_(_ => onSuccess() |> resolve)
    |> catch(err =>
         onError
         |> Belt.Option.mapWithDefault(_, Js.Promise.resolve(), errorFn =>
              errorFn(err)
            )
       )
  );
};
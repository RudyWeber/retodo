open Express;
open Router;
open Utils;
open Operators;

let router = router(~caseSensitive=true, ~strict=true, ());
use(router, Middleware.json(~limit=1.0 |> ByteLimit.mb, ()));

let todolist =
  ref(
    [
      {id: generateId(), description: "Buy some waffles", priority: 0},
      {id: generateId(), description: "Learn some ReasonML", priority: 1},
    ]: list(Todo.t),
  );

router->get(
  ~path="/",
  Middleware.from((_next, _req) =>
    Response.sendJson(Json.Encode.(todolist^ |> list(Todo.encode)))
  ),
);

router->post(
  ~path="/",
  Middleware.from((_next, req) => {
    let todo =
      Request.bodyJSON(req)
      >>= Todo.Add.decodePayload
      >>= Todo.Add.makeFromPayload;

    switch (todo) {
    | Some(todo) =>
      todolist := [todo, ...todolist^];
      Response.sendJson(Todo.encode(todo));
    | None => Response.sendStatus(Response.StatusCode.BadRequest)
    };
  }),
);

router->delete(
  ~path="/:todoId",
  Middleware.from((_next, req) =>
    req
    |> Request.params
    |> Js.Dict.get(_, "todoId")
    |> badRequestIfNone(jsonTodoId => {
         let todoId = Json.Decode.string(jsonTodoId);
         let newList =
           todolist^ |> List.filter((todo: Todo.t) => todo.id !== todoId);

         if (List.length(newList) === List.length(todolist^)) {
           Response.StatusCode.NotFound;
         } else {
           todolist := newList;
           Response.StatusCode.NoContent;
         };
       })
    |> Response.sendStatus
  ),
);

let handlePatch = (req, todoId) =>
  Request.bodyJSON(req)
  >>= Todo.Edit.decodePayload
  |> badRequestIfNone(decodedPayload => {
       todolist :=
         todolist^
         |> List.map((todo: Todo.t) =>
              switch (todo.id) {
              | id when id === todoId =>
                Todo.Edit.editFromPaylod(
                  ~payload=decodedPayload,
                  ~original=todo,
                )
              | _ => todo
              }
            );

       Response.StatusCode.Ok;
     });

router->patch(
  ~path="/:todoId",
  Middleware.from((_next, req) =>
    req
    |> Request.params
    |> Js.Dict.get(_, "todoId")
    |> badRequestIfNone(jsonTodoId => {
         let todoId = Json.Decode.string(jsonTodoId);
         let existsInList = List.exists((todo: Todo.t) => todo.id === todoId);

         existsInList(todolist^) ?
           handlePatch(req, todoId) : Response.StatusCode.NotFound;
       })
    |> Response.sendStatus
  ),
);
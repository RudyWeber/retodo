open Operators;

[@bs.deriving jsConverter]
type t = {
  id: string,
  description: string,
  priority: int,
};

let encode = todo =>
  Json.Encode.(
    object_([
      ("id", todo.id |> string),
      ("description", todo.description |> string),
      ("priority", todo.priority |> int),
    ])
  );

let decode = jsonTodo =>
  Json.Decode.{
    id: jsonTodo |> field("id", string),
    description: jsonTodo |> field("description", string),
    priority: jsonTodo |> field("priority", int),
  };

module Add = {
  [@bs.deriving jsConverter]
  type payload = {
    description: string,
    priority: option(int),
  };

  let decodePayload = todoJsonPayload =>
    try (
      Some({
        description:
          todoJsonPayload
          |> Json.Decode.field("description", Json.Decode.string),
        priority:
          todoJsonPayload |?> Json.Decode.field("priority", Json.Decode.int),
      })
    ) {
    | _ => None
    };

  let makeFromPayload = payload =>
    try (
      Some({
        id: Utils.generateId(),
        description: payload.description,
        priority: Belt.Option.getWithDefault(payload.priority, 0),
      })
    ) {
    | _ => None
    };
};

module Edit = {
  [@bs.deriving jsConverter]
  type payload = {
    description: option(string),
    priority: option(int),
  };

  let decodePayload = todoJsonPayload =>
    try (
      Some(
        Json.Decode.{
          description: todoJsonPayload |?> field("description", string),
          priority: todoJsonPayload |?> field("priority", int),
        },
      )
    ) {
    | _ => None
    };

  let editFromPaylod = (~payload, ~original: t) => {
    ...original,
    description:
      Belt.Option.getWithDefault(payload.description, original.description),
    priority: Belt.Option.getWithDefault(payload.priority, original.priority),
  };
};
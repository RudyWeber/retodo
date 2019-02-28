let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  didMount: _self => {
    Js.Promise.(
      Axios.get("/api") |> then_(res => Js.log(res##data) |> resolve)
    )
    |> ignore;
  },
  render: _self => ReasonReact.string("It all starts here..."),
};
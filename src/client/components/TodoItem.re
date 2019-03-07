type state = {
  description: string,
  isEditing: bool,
};

type action =
  | SET_DESCRIPTION(string)
  | SET_EDITING(bool);

let reducer = (action, state) =>
  switch (action) {
  | SET_DESCRIPTION(description) =>
    ReasonReact.Update({...state, description})
  | SET_EDITING(isEditing) => ReasonReact.Update({...state, isEditing})
  };

let component = ReasonReact.reducerComponent("TodoItem");
let make = (~description, ~delete, ~edit, _children) => {
  ...component,
  initialState: () => {isEditing: false, description},
  reducer,
  render: ({send, state}) =>
    <li>
      {state.isEditing ?
         <input
           value={state.description}
           onChange={changeEvent =>
             send(
               SET_DESCRIPTION(ReactEvent.Form.target(changeEvent)##value),
             )
           }
           onBlur={blurEvent => {
             ReactEvent.Synthetic.preventDefault(blurEvent);
             ignore(
               edit(ReactEvent.Focus.target(blurEvent)##value)
               |> Js.Promise.(
                    then_(() => send(SET_EDITING(false)) |> resolve)
                  ),
             );
           }}
         /> :
         <span onClick={_ => send(SET_EDITING(true))}>
           {ReasonReact.string(description)}
         </span>}
      <button onClick=delete> {ReasonReact.string("X")} </button>
    </li>,
};
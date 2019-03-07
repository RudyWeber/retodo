type state = {model: Todo.Add.payload};

type action =
  | RESET
  | SET_DESCRIPTION(string)
  | SET_PRIORITY(option(int));

let initialState = () => {
  model: {
    description: "",
    priority: None,
  },
};

let reducer = (action, state) =>
  switch (action) {
  | RESET => ReasonReact.Update(initialState())
  | SET_DESCRIPTION(description) =>
    ReasonReact.Update({
      model: {
        ...state.model,
        description,
      },
    })
  | SET_PRIORITY(priority) =>
    ReasonReact.Update({
      model: {
        ...state.model,
        priority,
      },
    })
  };

let component = ReasonReact.reducerComponent("TodoForm");
let make = (~add, _children) => {
  ...component,
  initialState,
  reducer,
  render: ({send, state}) => {
    <form>
      <fieldset>
        <legend> {ReasonReact.string("Add a task")} </legend>
        <input
          name="description"
          placeholder="description..."
          value={state.model.description}
          onChange={changeEvent =>
            send(
              SET_DESCRIPTION(ReactEvent.Form.target(changeEvent)##value),
            )
          }
        />
        <br />
        <input
          name="priority"
          type_="number"
          min=0
          placeholder="priority"
          value={Belt.Option.mapWithDefault(state.model.priority, "", p =>
            string_of_int(p)
          )}
          onChange={changeEvent =>
            send(
              SET_PRIORITY(
                try (
                  Some(
                    ReactEvent.Form.target(changeEvent)##value
                    |> int_of_string,
                  )
                ) {
                | _ => None
                },
              ),
            )
          }
        />
        <br />
        <button
          onClick={clickEvent => {
            ReactEvent.Synthetic.preventDefault(clickEvent);
            ignore(
              Js.Promise.(
                add(~payload=state.model)
                |> then_(_ => send(RESET) |> resolve)
              ),
            );
          }}>
          {ReasonReact.string("Add")}
        </button>
      </fieldset>
    </form>;
  },
};
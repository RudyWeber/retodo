type state = {todos: list(Todo.t)};

type action =
  | SET_LIST(list(Todo.t))
  | ADD_TODO(Todo.t)
  | DELETE_TODO(string)
  | EDIT_TODO(Todo.t);

let initialState = () => {todos: []};

let reducer = (action, {todos}) =>
  switch (action) {
  | SET_LIST(todos) => ReasonReact.Update({todos: todos})
  | ADD_TODO(todo) => ReasonReact.Update({todos: [todo, ...todos]})
  | DELETE_TODO(id) =>
    ReasonReact.Update({
      todos: todos |> List.filter((todo: Todo.t) => todo.id !== id),
    })
  | EDIT_TODO(todo) =>
    ReasonReact.Update({
      todos:
        todos
        |> List.map((currentTodo: Todo.t) =>
             todo.id === currentTodo.id ? todo : currentTodo
           ),
    })
  };

let didMount =
  ReasonReact.(
    ({send}) => {
      ignore(
        TodoService.getAll(~onSuccess=todos => send(SET_LIST(todos)), ()),
      );
    }
  );

let addTodo = (send, ~payload) => {
  TodoService.add(
    ~payload=Todo.Add.payloadToJs(payload),
    ~onSuccess=todo => send(ADD_TODO(todo)),
    (),
  );
};

let deleteTodo = (send, ~id, _event) => {
  ignore(
    TodoService.delete(~id, ~onSuccess=() => send(DELETE_TODO(id)), ()),
  );
};

let editTodo = (send, ~editedTodo, description) => {
  let newTodo: Todo.t = {...editedTodo, description};
  TodoService.edit(newTodo, ~onSuccess=() => send(EDIT_TODO(newTodo)), ());
};

let component = ReasonReact.reducerComponent("App");
let make = _children => {
  ...component,
  initialState,
  didMount,
  reducer,
  render: ({send, state}) => {
    <>
      <ul>
        {state.todos
         |> List.map(({id: key, description} as todo: Todo.t) =>
              <TodoItem
                key
                description
                delete={deleteTodo(send, ~id=key)}
                edit={editTodo(send, ~editedTodo=todo)}
              />
            )
         |> Array.of_list
         |> ReasonReact.array}
      </ul>
      <TodoForm add={addTodo(send)} />
    </>;
  },
};
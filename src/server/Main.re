open Express;
open Express.App;

let app = express();

app->useRouterOnPath(~path="/todos", TodosRouter.router);

app->get(
  ~path="/",
  Middleware.from((_next, _req) =>
    Response.sendString("This comes from the API...")
  ),
);

app->listen(
  ~port=8080,
  ~onListen=_ => Js.log("Listening at http://127.0.0.1:8080"),
  (),
);
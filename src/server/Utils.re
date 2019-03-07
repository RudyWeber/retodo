Random.self_init();

let generateRandomString = length => {
  let genRandomCharAsInt = () =>
    switch (Random.int(26 + 26 + 10)) {
    | n when n < 26 => int_of_char('a') + n
    | n when n < 26 + 26 => int_of_char('A') + n - 26
    | n => int_of_char('0') + n - 26 - 26
    };
  let gen = _ => String.make(1, genRandomCharAsInt() |> char_of_int);
  Array.init(length, gen) |> Array.to_list |> String.concat("");
};

let generateId = () => generateRandomString(6);

let badRequestIfNone = (f, x) =>
  Belt.Option.mapWithDefault(x, Express.Response.StatusCode.BadRequest, f);
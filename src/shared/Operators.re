let (>>=) = (value, f) =>
  switch (value) {
  | Some(x) => f(x)
  | None => None
  };

let (|?>) = (x, f) =>
  try (Some(f(x))) {
  | _ => None
  };
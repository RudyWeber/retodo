const proxy = require("http-proxy-middleware");
const Bundler = require("parcel-bundler");
const express = require("express");

let bundler = new Bundler("./src/client/index.html");
let app = express();

app.use(
  "/api",
  proxy({
    target: "http://localhost:8080",
    pathRewrite: {
      "/api": ""
    }
  })
);

app.use(bundler.middleware());

app.listen(Number(process.env.PORT || 1234));

# retodo

A todolist to learn some ReasonML

## Prerequisite

Install `bs-platform` : `yarn global add bs-platform`

## Compile the code

Run `yarn start` to compile Reason code in watch mode.

## API

It runs on port `8080` and the Front can access it on `/api` without the need to specify any port (see `apiProxy.js` if you want).<br />
You can run:

- `yarn server:dev` to start hacking and use the livereload.
- `yarn server:run` to just run the server.

## Front

It runs on port `1234` and auto-refreshes as you make changes to the code.<br />
To run it, use `yarn client:dev`.

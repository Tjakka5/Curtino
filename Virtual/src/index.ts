import express from "express";
import socketio from "socket.io";
import path from "path";
import http from "http";
import axios from "axios";

import config from "./serverConfig.json";

const app = express();
const port = 80;
app.set("port", port);

let _http = new http.Server(app);
let io = socketio(_http);

for (let i = 0; i < 4; i++) {
	axios.post(`${config.serverURL}registerDevice`, {
		deviceID: `VirtualCurtino_${i}`,
		deviceIP: `${config.ownIP}`,
	});
}


app.use(express.json());

app.get("/", (_req, res) => {
  res.sendFile(path.resolve("./html/index.html"));
});

app.post(`/post`, (req, res) => {
  let command = req.body["command"];
  if (command != undefined) {
		console.log(command);
		io.emit("sendCommand", req.body);
		res.send(JSON.stringify({ success: true }));
  } else {
		res.send(JSON.stringify({ success: false }));
	}
});

io.on("connection", function (socket) {
	console.log("a user connected");
	
  socket.on("message", function (message) {
		console.log(message);
  });
});

_http.listen(port, function () {
  console.log(`listening on *:${port}`);
});

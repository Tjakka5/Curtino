import { Client } from "discord.js";
import request = require("request");

const token = "NzE4NDA3ODcyMzg3NDgxNjUw.XtplGg.VwlVYd96gVGS6YVIEpQ5d7e3vfE";

let client = new Client();

client
  .login(token)
  .then(() => {
    console.log("Logged in");

    client.on("message", (msg) => {
      if (msg.author.bot) {
        return;
      }

      if (!msg.content.startsWith("!curtino")) {
        return;
      }

      if (msg.content.indexOf("ledOn") != -1) {
        console.log("Yes daddy");
        request.post(
          "http://192.168.2.15/post/",
          {
            json: {
              command: "setLed",
              state: true,
            },
          },
          (error, res, body) => {
            if (error) {
              msg.reply(`Something went wrong. StatusCode: ${res.statusCode}`);
              return;
            }

            msg.reply("Led turned on")
          }
        );
      }

      if (msg.content.indexOf("ledOff") != -1) {
        request.post(
          "http://192.168.2.15/post/",
          {
            json: {
              command: "setLed",
              state: false,
            },
          },
          (error, res, body) => {
            if (error) {
              msg.reply(`Something went wrong. StatusCode: ${res.statusCode}`);
              return;
            }

            msg.reply("Led turned off");
          }
        );
      }
    });
  })
  .catch((reason) => {
    console.log(reason);
  });

import { Client } from "discord.js";
import axios from "axios";
import config = require("./config.json");

let client = new Client();

client
  .login(config.client.token)
  .then(() => {
    console.log(`Logged in as ${client.user?.id}`);

    client.on("message", (msg) => {
      if (msg.author.bot) {
        return;
      }

      if (!msg.content.startsWith(config.client.prefix)) {
        return;
      }

      if (msg.content.indexOf("openCurtain") != -1) {
        axios.post(`${config.serverURL}sendCommand/`, {
          deviceID: "BigPeePeeESP",
          command: "openCurtain"
        })
        .then((response) => {
          msg.reply("Succes");
        })
        .catch((error) => {
          msg.reply("Nope");
        })
      }
    });
  })
  .catch((reason) => {
    console.log(reason);
  });

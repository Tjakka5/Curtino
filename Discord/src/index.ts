import { Client, Message, Presence, PresenceData } from "discord.js";
import axios from "axios";
import config = require("./config.json");
import clientConfig = require("./clientConfig.json");
import { isNumber } from "util";

import openCurtain from "./commands/openCurtain";
import closeCurtain from "./commands/closeCurtain";
import getCurtainStatus from "./commands/getCurtainStatus";
import configureTime from "./commands/configureTime";
import configureLight from "./commands/configureLight";
import { GetCurtainStatusResponse } from "./responses";

type Command = {
  name: string,
  description: string,

  execute: (msg: Message, args: string[]) => void;
}

let commands: Map<string, Command> = new Map([
  [openCurtain.name, openCurtain],
  [closeCurtain.name, closeCurtain],
  [getCurtainStatus.name, getCurtainStatus],
  [configureTime.name, configureTime],
  [configureLight.name, configureLight]
]);


let client = new Client();

function parseArguments(text: string): string[] {
  // Split text at spaces, but ignore spaces between single quotes
  let args = text.slice(config.prefix.length).match(/(?:[^\s']+|'[^']*')+/g);

  // Default args to empty array
  if (args == null) {
    args = [];
  }

  // Remove quotes
  for (var i = 0; i < args.length; i++) {
    args[i] = args[i].split("'").join("");
  }

  return args;
}

client
  .login(clientConfig.token)
  .then(() => {
    console.log(`Logged in as ${client.user?.id}`);

    setInterval(() => {
      axios.post(`${clientConfig.serverURL}sendCommand/`, {
        deviceID: "Curtino",
        command: "getCurtainStatus"
      })
        .then((response: GetCurtainStatusResponse) => {
          if (response.data.success) {
            client.user?.setActivity(`Curtain is: ${response.data.status}`, {
              type: "CUSTOM_STATUS",
            })
          } 
        });
      return true;
    }, 1000);

    client.on("message", (msg) => {
      if (msg.author.bot) {
        return;
      }

      if (!msg.content.startsWith(config.prefix)) {
        return;
      }

      let args = parseArguments(msg.content);
      let cmd = args.shift();

      if (cmd == undefined) {
        msg.reply(config.responses.noCommand);
      } else {
        if (cmd.toLowerCase() == "help") {
          let out = "All commandos zijn: \n"

          commands.forEach(command => {
            out += command.name + " - " + command.description + "\n";
          })

          msg.reply(out);
        } else if (!commands.has(cmd.toLowerCase())) {
          msg.reply(config.responses.commandNotFound);
        } else {
          let command = commands.get(cmd.toLowerCase())!;

          command.execute(msg, args);
        }
      }
    });
  })
  .catch((reason) => {
    console.log(reason);
  });

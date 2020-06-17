import { Client, Message } from "discord.js";
import axios from "axios";
import config = require("./config.json");
import clientConfig = require("./clientConfig.json");
import { isNumber } from "util";


interface DefaultData {
  success: boolean
}

interface DefaultResponse {
  data: DefaultData
}

interface GetCurtainStatusResponse {
  data: DefaultData & {
    isOpen: boolean
  }
}

let client = new Client();

function between(value: number, min: number, max: number) : boolean {
  return (value >= min && value <= max);
}

function handleRequestError(error: any, msg: Message) {
  msg.reply(config.responses.requestError);
}

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
      } else if (cmd == "openCurtain") {
        axios.post(`${clientConfig.serverURL}sendCommand/`, {
          deviceID: "BigPeePeeESP",
          command: "openCurtain"
        })
          .then((response) => {
            msg.reply(config.responses.success);
          })
          .catch((error) => {
            handleRequestError(error, msg);
          })
      } else if (cmd == "closeCurtain") {
        axios.post(`${clientConfig.serverURL}sendCommand/`, {
          deviceID: "BigPeePeeESP",
          command: "closeCurtain"
        })
          .then((response) => {
            msg.reply(config.responses.success);
          })
          .catch((error) => {
            handleRequestError(error, msg);
          })
      } else if (cmd == "getCurtainStatus") {
        axios.post(`${clientConfig.serverURL}sendCommand/`, {
          deviceID: "BigPeePeeESP",
          command: "getCurtainStatus"
        })
          .then((response: GetCurtainStatusResponse) => {
            if (!response.data.success) {
              msg.reply(config.responses.fail);

            } else {
              if (response.data.isOpen == true) {
                msg.reply(config.responses.curtainState.curtainOpen);
              } else {
                msg.reply(config.responses.curtainState.curtainClosed)
              }
            }
          })
          .catch((error) => {
            handleRequestError(error, msg);
          })
      } else if (cmd == "configureTime") {
        let openTime = Number(args[0]);
        let closeTime = Number(args[1]);

        if (isNaN(openTime) || isNaN(closeTime)) {
          msg.reply(config.responses.curtainTime.noTime);
        } else {
          axios.post(`${clientConfig.serverURL}sendCommand/`, {
            deviceID: "BigPeePeeESP",
            command: "configureTime",
            openTime: openTime,
            closeTime: closeTime,
          })
            .then((response: DefaultResponse) => {
              if (response.data.success == true) {
                msg.reply(config.responses.success);
              }
              else {
                msg.reply(config.responses.fail);
              }
            })
            .catch((error) => {
              handleRequestError(error, msg);
            });
        }
      } else if (cmd == "configureLight") {
        let lightRequiredToOpen = Number(args[0]);
        let lightRequiredToClose = Number(args[1]);

        if (isNaN(lightRequiredToOpen) || isNaN(lightRequiredToClose)) {
          msg.reply(config.responses.curtainLight.noLight);
        } else if (lightRequiredToClose < lightRequiredToOpen)
        {
          msg.reply(config.responses.curtainLight.incorrectOrder);
        } else if (!between(lightRequiredToOpen, 0, 100) || !between(lightRequiredToClose, 0, 100))
        {
          msg.reply(config.responses.curtainLight.invalidRange);
        }
        else {
          axios.post(`${clientConfig.serverURL}sendCommand/`, {
            deviceID: "BigPeePeeESP",
            command: "configureLight",
            lightRequiredToOpen: lightRequiredToOpen,
            lightRequiredToClose: lightRequiredToClose,
          })
            .then((response: DefaultResponse) => {
              if (response.data.success == true) {
                msg.reply(config.responses.success);
              }
              else {
                msg.reply(config.responses.fail);
              }
            })
            .catch((error) => {
              handleRequestError(error, msg);
            });
        }
      } else if (cmd == "help") {
        msg.reply(`Alle commando's \n -------------------- \n De prefix is ${config.prefix} \n help: laat deze lijst zien. \n openCurtain: doet gordijn open \n closeCurtain: doet gordijn dicht \n getCurtainStatus: laat weten of gordijn open of dicht is`)
      } else {
        msg.reply(config.responses.commandNotFound);
      }
    });
  })
  .catch((reason) => {
    console.log(reason);
  });

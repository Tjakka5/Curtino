import { Message } from "discord.js";
import axios from "axios";

import { handleRequestError, between } from "../helpers";

import { DefaultResponse } from "../responses";

import clientConfig = require("../clientConfig.json");
import config = require("../config.json");

export default {

    name: "configurelight",
    description: "Set a light value to open the curtain.",

    execute: function (msg: Message, args: string[]) {
        let lightRequiredToOpen = Number(args[0]);
        let lightRequiredToClose = Number(args[1]);

        if (isNaN(lightRequiredToOpen) || isNaN(lightRequiredToClose)) {
            msg.reply(config.responses.curtainLight.noLight);
        } else if (lightRequiredToClose < lightRequiredToOpen) {
            msg.reply(config.responses.curtainLight.incorrectOrder);
        } else if (!between(lightRequiredToOpen, 0, 100) || !between(lightRequiredToClose, 0, 100)) {
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
    }
}
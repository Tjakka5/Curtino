import { Message } from "discord.js";
import axios from "axios";

import { handleRequestError } from "../helpers";

import clientConfig = require("../clientConfig.json");
import config = require("../config.json");

export default {
    name: "closecurtain",
    description: "Closes the curtain",

    execute: function (msg: Message, args: string[]) {
        axios.post(`${clientConfig.serverURL}sendCommand/`, {
            deviceID: "BigPeePeeESP",
            command: "closeCurtain"
        })
            .then((response) => {
                msg.reply(config.responses.success);
            })
            .catch((error) => {
                handleRequestError(error, msg);
            });
    },
}
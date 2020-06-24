import { Message } from "discord.js";
import axios from "axios";

import { handleRequestError } from "../helpers";
import { GetCurtainStatusResponse } from "../responses";

import clientConfig = require("../clientConfig.json");
import config = require("../config.json");

export default {
    name: "getcurtainstatus",
    description: "Reports if curtain is open or closed.",

    execute: function (msg: Message, args: string[]) {
        axios.post(`${clientConfig.serverURL}sendCommand/`, {
            deviceID: "Curtino",
            command: "getCurtainStatus"
        })
            .then((response: GetCurtainStatusResponse) => {
                if (!response.data.success) {
                    msg.reply(config.responses.fail);
                } else {
                    msg.reply(`${config.responses.curtainState} ${response.data.status}`);
                }
            })
            .catch((error) => {
                handleRequestError(error, msg);
            })
    },
}

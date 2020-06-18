import { Message } from "discord.js";
import axios from "axios";

import { handleRequestError, between } from "../helpers";

import { DefaultResponse } from "../responses";

import clientConfig = require("../clientConfig.json");
import config = require("../config.json");

export default {
    name: "configuretime",
    description: "Set a time to open and close the curtain.",

    execute: function (msg: Message, args: string[]) {

        let rawOpenTime = args[0];
        let rawCloseTime = args[1];

        let openTimeHours = Number(rawOpenTime.split(':')[0]);
        let openTimeMinutes = Number(rawOpenTime.split(':')[1]);

        let closeTimeHours = Number(rawCloseTime.split(':')[0]);
        let closeTimeMinutes = Number(rawCloseTime.split(':')[1]);

        if (isNaN(openTimeHours) || isNaN(openTimeMinutes) || isNaN(closeTimeHours) || isNaN(closeTimeMinutes)) {
            msg.reply(config.responses.curtainTime.noTime);
        } else if (!between(openTimeHours, 0 , 23) || !between(openTimeMinutes, 0, 60) || !between(closeTimeHours, 0, 23) || !between(closeTimeMinutes, 0, 60)) {
            msg.reply(config.responses.curtainTime.noTime)
        } else {
            let openTime = (openTimeHours * 3600) + (openTimeMinutes * 60);
            let closeTime = (closeTimeHours * 3600) + (closeTimeMinutes * 60);

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
    }
}
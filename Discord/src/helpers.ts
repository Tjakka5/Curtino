import { Message } from "discord.js";
import config = require("./config.json");

export function handleRequestError(error: any, msg: Message) {
    msg.reply(config.responses.requestError);
}

export function between(value: number, min: number, max: number): boolean {
    return (value >= min && value <= max);
}
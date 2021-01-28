import Module from "../modules";
import DiscordCommand from "./discord";
import TwitterCommand from "./twitter";

export default class SocialsModule implements Module {
    moduleName = "socials";

    includedCommands = [
        TwitterCommand,
        DiscordCommand,
    ];
}
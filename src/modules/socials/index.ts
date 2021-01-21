import Module from "../modules";
import TwitterCommand from "./twitter";

export default class SocialsModule implements Module {
    moduleName = "socials";

    includedCommands = [
        TwitterCommand,
        
    ];
}
import Module from "../modules";
import { ListCommands } from "./commands";
import { GreetCommand } from "./greet";
import { PingCommand } from "./ping";

export default class Default implements Module {
    public readonly moduleName: string;
    public readonly includedCommands: any[];

    constructor () {
        this.moduleName = "default";
        this.includedCommands = [
            GreetCommand,
            PingCommand
        ];
    }
}
import Module from "../modules";
import { ListCommands } from "./commands";

export default class Default implements Module {
    public readonly moduleName: string;
    public readonly includedCommands: any[];

    constructor () {
        this.moduleName = "default";
        this.includedCommands = [
            //ListCommands
        ];
    }
}
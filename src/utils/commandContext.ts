/** A user-given command extracted from a message. */
export default class CommandContext {
    readonly parsedCommandName: string;
    readonly args: string[];
    readonly originalMessage: string;
    readonly commandPrefix: string;

    constructor (message: string, prefix: string) {
        this.commandPrefix = prefix;
        
        const splitMessage = message
            .slice(prefix.length)
            .trim()
            .split(/ +/g);

        this.parsedCommandName = splitMessage.shift()!.toLowerCase();
        this.args = splitMessage;
        this.originalMessage = message;
    }
}
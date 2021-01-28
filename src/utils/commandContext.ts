/** A user-given command extracted from a message. */
export default class CommandContext {
    public readonly parsedCommandName: string;
    public readonly args: string[];
    public readonly originalMessage: string;

    constructor(message: string, prefix: string) {
        const splitMessage = message
            .slice(prefix.length)
            .trim()
            .split(/ +/g);
        this.parsedCommandName = splitMessage.shift()!.toLowerCase();
        this.args = splitMessage;
        this.originalMessage = message;
    }
}
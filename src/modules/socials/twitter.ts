import Command from "../../utils/command";
import CommandContext from "../../utils/commandContext";
import { client } from '../../bot';

export default class TwitterCommand implements Command {
    commandNames = ["twitter"];

    async run(parsedUserCommand: CommandContext): Promise<void> {
        await client.say(client.channels[0], `Follow me on twitter for the latest updates and shit and giggles. https://twitter.com/SenpaiR6`);
    }

    hasPermissionToRun(parsedUserCommand: CommandContext): boolean {
        return true;
    }
}
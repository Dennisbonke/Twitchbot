import CommandContext from '../../utils/commandContext';
import Command from '../../utils/command';
import { client } from "../../bot";

export class PingCommand implements Command {
    commandNames = ['ping'];
    
    async run (parsedUserCommand: CommandContext): Promise<void> {
        await client.say(client.channels[0],'pong! The current ping to the servers for this API is `' + Math.round(Math.random()) + 'ms`');
    }

    hasPermissionToRun (parsedUserCommand: CommandContext): boolean {
        return true;
    }
}
import CommandContext from '../../utils/commandContext';
import Command from '../../utils/command';
import { client } from '../../bot';

export class GreetCommand implements Command {
    commandNames = ['greet', 'hello'];

    async run (parsedUserCommand: CommandContext): Promise<void> {
        await client.say(client.channels[0], `Welcome to this Server, ${parsedUserCommand.args[0]}`);
    }

    hasPermissionToRun (parsedUserCommand: CommandContext): boolean {
        return true;
    }
}
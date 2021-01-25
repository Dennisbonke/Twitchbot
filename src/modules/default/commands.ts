import CommandContext from '../../utils/commandContext';
import Command from '../../utils/command';
import { client } from '../../bot';
import { GreetCommand } from './greet';

export class ListCommands implements Command {
    readonly commandNames = ['commands', 'help'];

    private commands: Command[];

    constructor (commands: Command[]) {
        this.commands = commands;     
    }

    async run (commandContext: CommandContext): Promise<void> {
        const allowedCommands = this.commands.filter((command) =>      
            command.hasPermissionToRun(commandContext),      
        );      
        
        if (commandContext.args.length === 0) {
            // No command specified, give the user a list of all commands they can use.
            const commandNames = allowedCommands.map(
                (command) => command.commandNames[0],
            );
            await client.say(client.channels[0],
                `here is a list of commands you can run: ${commandNames.join(
                    ', ',
                )}. Try !commands ${commandNames[0]} to learn more about one of them.` +
                '\nVersion: 0.1 https://github.com/Westlanderz/twitchbot',
            );
            return;
        }

        const matchedCommand = this.commands.find((command) =>
            command.commandNames.includes(commandContext.args[0]),
        );
        
        if (!matchedCommand) {
            await client.say(client.channels[0],
                "I don't know about that command :(. Try !commands to find all commands you can use.",
            );
            throw Error('Unrecognized command');
        }
    }

    hasPermissionToRun (commandContext: CommandContext): boolean {
        return true;
    }
}


import CommandContext from '../../utils/commandContext';
import Command from '../../utils/command';
import { client } from '../../bot';

export class ListCommands implements Command {
    readonly commandNames = ['help', 'halp', 'hlep'];

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
                )}. Try !help ${commandNames[0]} to learn more about one of them.` +
                '\nVersion: 0.1 https://github.com/Westlanderz/DiscordBot',
            );
            return;
        }

        const matchedCommand = this.commands.find((command) =>
            command.commandNames.includes(commandContext.args[0]),
        );
        if (!matchedCommand) {
            await client.say(client.channels[0],
                "I don't know about that command :(. Try !help to find all commands you can use.",
            );
            throw Error('Unrecognized command');
        }
        if (allowedCommands.includes(matchedCommand)) {
            await client.say(client.channels[0],
                this.buildHelpMessageForCommand(matchedCommand, commandContext),
            );
        }
    }

    private buildHelpMessageForCommand (command: Command, context: CommandContext): string {
        return `${command.getHelpMessage(
            context.commandPrefix,
        )}\nCommand aliases: ${command.commandNames.join(', ')}`;
    }

    hasPermissionToRun (commandContext: CommandContext): boolean {
        return true;
    }

    getHelpMessage (commandPrefix: string) {
        return 'I think you already know how to use this command...';
    }
}


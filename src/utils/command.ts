import CommandContext from './commandContext';

export default interface Command {
    readonly commandNames: string[];
    getHelpMessage (commandPrefix: string): string;
    run (parsedUserCommand: CommandContext): Promise<void>;
    hasPermissionToRun (parsedUserCommand: CommandContext): boolean;
}
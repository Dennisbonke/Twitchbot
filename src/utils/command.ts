import CommandContext from './commandContext';

export default interface Command {
    readonly commandNames: string[];
    run (parsedUserCommand: CommandContext): Promise<void>;
    hasPermissionToRun (parsedUserCommand: CommandContext): boolean;
}
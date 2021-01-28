import CommandContext from './commandContext';

export default interface Command {
    readonly commandNames: string[];
    run(command: CommandContext): Promise<void>;
    hasPermissionToRun(command: CommandContext): boolean;
}
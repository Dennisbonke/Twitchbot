import Command from '../../utils/command';
import { client } from '../../bot';

export class GreetCommand implements Command {
    public readonly commandNames = ['greet', 'hello'];

    public async run(): Promise<void> {
        await client.say(client.channels[0], `Welcome to the stream`);
    }

    public hasPermissionToRun(): boolean {
        return true;
    }
}
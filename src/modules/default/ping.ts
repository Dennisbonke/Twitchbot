import Command from '../../utils/command';
import { client } from "../../bot";

export class PingCommand implements Command {
    public readonly commandNames = ['ping'];

    public async run(): Promise<void> {
        await client.say(client.channels[0], 'pong! The current ping to the servers for this API is `' + Math.round(Math.random()) + 'ms`');
    }

    public hasPermissionToRun(): boolean {
        return true;
    }
}
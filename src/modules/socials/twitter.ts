import Command from "../../utils/command";
import { client } from '../../bot';

export default class TwitterCommand implements Command {
    public readonly commandNames = ["twitter"];

    public async run(): Promise<void> {
        await client.say(client.channels[0], `Follow me on twitter for the latest updates and shit and giggles. https://twitter.com/SenpaiR6`);
    }

    public hasPermissionToRun(): boolean {
        return true;
    }
}
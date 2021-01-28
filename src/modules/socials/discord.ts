import { client } from "../../bot";
import Command from "../../utils/command";

export default class DiscordCommand implements Command {
    public readonly commandNames = ["discord"];

    public async run(): Promise<void> {
        await client.say(client.channels[0], `Join my general discord server by using this link: https://discord.gg/8FjTkfy. 
            If you want to learn dev or help me on projects join this server: https://discord.gg/VQ6E9JJyCC.`);
    }

    public hasPermissionToRun(): boolean {
        return true;
    }
}
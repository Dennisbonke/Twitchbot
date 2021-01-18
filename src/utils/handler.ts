import { client } from "../bot";

export default class Handler {
    public onConnectedHandler(addr: any, port: any) {
        console.log(`* Connected to ${addr}:${port}`);
    }

    public async onRaidHandler(channel: string, username: string, viewers: number): Promise<void> {
        const message = `Big thank you to ${username} for the raid with ${viewers} people!!!!`;
        client.say(channel, message);
    }
}
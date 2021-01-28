import { client } from "../bot";

export default class Handler {
    public onConnectedHandler(addr: any, port: any): void {
        console.log(`* Connected to ${addr}:${port}`);
    }

    public async onRaidHandler(channel: string, username: string, viewers: number): Promise<void> {
        const message = `Big thank you to ${username} for the raid with ${viewers} people!!!!`;
        await client.say(channel, message);
    }

    public on_sub_handler(): void {

    }

    public on_host_handler(): void {

    }

    public on_resub_handler(): void {

    }
}
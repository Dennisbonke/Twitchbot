import * as tmi from 'tmi.js';
import Config from './config/config';
import Handler from './utils/handler';
import MessageHandler from './utils/messageHandler';

class Client {
    public client: tmi.Client;
    private config: tmi.Options;
    public readonly channels: string[];
    public readonly prefix: string;

    constructor () {
        this.channels = [
            'Westlanderz',
        ];
        this.config = Config.fromEnv(process.env, this.channels).convertToTMI();
        this.client = tmi.client(this.config);
        this.prefix = '!';
        this.client.connect();
        this.client.on('connected', (addr: any, port: any) => {
            handler.onConnectedHandler(addr, port)
        });
    }

    public async say(channel: string, message: string) {
        this.client.say(channel, message);
    }
}

const handler = new Handler();
export const client = new Client();
const messageHandler = new MessageHandler(client.prefix);

client.client.on('message', (channel: string, userstate: tmi.ChatUserstate, message: string, self: boolean) => {
    messageHandler.handleMessage(channel, userstate, message, self)
});
// client.handler('subscription');
// client.handler('resub');
client.client.on('raided', handler.onRaidHandler);
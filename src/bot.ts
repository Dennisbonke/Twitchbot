import * as tmi from 'tmi.js';
import Config from './config/config';
import Handler from './utils/handler';
import MessageHandler from './utils/messageHandler';

// List of constants/MACROS
const DEBUG: boolean = true;

enum Errors { 
    NO_ERROR,
    DEFAULT_ERROR,
    API_ERROR,
}

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
    }

    public async say(channel: string, message: string) {
        this.client.say(channel, message);
    }
}

const handler = new Handler();
export const client = new Client();
const messageHandler = new MessageHandler(client.prefix);


client.client.on('connected', () => {
    handler.onConnectedHandler
});
client.client.on('message', (channel: string, userstate: tmi.ChatUserstate, message: string, self: boolean) => {
    messageHandler.handleMessage(channel, userstate, message, self)
});
// client.handler('subscription');
// client.handler('resub');
client.client.on('raided', handler.onRaidHandler);

// TODO: Remake to class system and module system.
// const sns: ChatCommand = {
//     trigeger: 'sens',
//     alias: 'sens',
//     everyone: true,

//     execute: () => {
//         consoleDebug(sens.trigger, DEBUG);
//         client.say(channels[0], `6/6 58 75 83 87 89 91 - 400 DPI`);
//         return { isSuccesfull: true }
//     }
// }

// const twitter: ChatCommand = {
//     trigger: 'twitter',
//     alias: 'twitter',
//     everyone: true,

//     execute: () => {
//         consoleDebug(twitter.trigger, DEBUG);
//         client.say(channels[0], `Follow me on twitter for the latest updates and shit and giggles. https://twitter.com/SenpaiR6`);
//         return { isSuccesfull: true }
//     }
// }

// const availableCommands: ChatCommand[] = [sens, twitter,];

// function onMessageHandler(target: string, senderData: tmi.Userstate, message: string, botSelf: boolean): void {
//     if (botSelf)
//         return;

//     var commandName: string = "";
//     var error: number = NO_ERROR;
//     var commandExecuted: boolean = false;
//     var params: string | string[] | null = null;

//     if (message.indexOf('!') === 0) {
//         commandName = getCommand(message);
//         const matchingAvailableCommand = availableCommands.find(x =>
//             (!Array.isArray(x.trigger) && commandName.indexOf(x.trigger.toLowerCase()) === 0) ||
//             (!Array.isArray(x.alias) && commandName.indexOf(x.alias.toLowerCase()) === 0) ||
//             (Array.isArray(x.alias) && x.alias.some(trigger => commandName.indexOf(trigger.toLowerCase()) === 0))
//         );
//         if (checkPermission(commandName)) {
//             matchingAvailableCommand.execute(params);
//             commandExecuted = true;
//         }
//     } 


//     // Check if response is invalid and give a response in chat
//     if (message.indexOf('!') === 0 && commandExecuted == false) {
//         error = DEFAULT_ERROR;
//         client.say(target, `This is an unknown command, for a full list of the available commands please check here.`);
//         consoleDebug(commandName, DEBUG, error);
//     }
// }

// function getCommand(msg: string): string {
//     const startIndex: number = 1;
//     const whiteIndex: number = (msg.indexOf(' ') > 0) ? msg.indexOf(' ') - 1 : msg.length - 1;
//     return msg.substr(startIndex, whiteIndex).trim().toLowerCase();
// }

// function getParam(msg: string, n: number): string[] {
//     const args = msg.slice(msg.indexOf(' ') + 1).split(' ');
//     var args2 = [];
//     for (var i = 0; i < n; i++) {
//         args2 = [...args2, args[i]];
//     }
//     return args2;
// }

// function consoleDebug(command: string, debug: boolean, err: number = NO_ERROR): void {
//     if (debug) {
//         switch (err) {
//             case NO_ERROR:
//                 console.log(`* Executed ${command} command`);
//                 break;

//             case DEFAULT_ERROR:
//                 console.log(`* Unknown command ${command}`);
//                 break;

//             case API_ERROR:
//                 console.log(command);
//                 break;
//         }
//     }
// }

// function checkPermission(command: string): boolean {
//     return true;
// }
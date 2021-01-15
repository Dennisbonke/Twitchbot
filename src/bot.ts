import * as tmi from 'tmi.js';
import Config from './config/config';

// List of constants/MACROS
const DEBUG: boolean = true;
const NO_ERROR: number = 0;
const DEFAULT_ERROR: number = 1;
const API_ERROR: number = 2;

const channels = ['Westlanderz'];
const config: Config = Config.fromEnv(process.env, channels)
const client: tmi.Client = tmi.client(config.convertToTMI());

client.connect();

client.on('message', onMessageHandler);
client.on('connected', onConnectedHandler);
client.on('subscription', onConnectedHandler);
client.on('resub', onConnectedHandler);
client.on('raided', onConnectedHandler);

interface ChatCommand {
    readonly trigger: string;
    readonly alias: string | string[];
    readonly modsOnly?: boolean;
    readonly subOnly?: boolean;
    readonly everyone?: boolean;

    execute: (params: string | string[] | null, sender?: string) => CommandResult
}

interface CommandResult {
    readonly isSuccesfull: boolean;
    readonly error?: string;
}

const sens: ChatCommand = {
    trigger: 'sens',
    alias: 'sens',
    everyone: true,

    execute: () => {
        consoleDebug(sens.trigger, DEBUG);
        client.say(channels[0], `6/6 58 75 83 87 89 91 - 400 DPI`);
        return { isSuccesfull: true }
    }
}

const twitter: ChatCommand = {
    trigger: 'twitter',
    alias: 'twitter',
    everyone: true,

    execute: () => {
        consoleDebug(twitter.trigger, DEBUG);
        client.say(channels[0], `Follow me on twitter for the latest updates and shit and giggles. https://twitter.com/SenpaiR6`);
        return { isSuccesfull: true }
    }
}

const availableCommands: ChatCommand[] = [sens, twitter,];

function onConnectedHandler(addr: any, port: any) {
    console.log(`* Connected to ${addr}:${port}`);
}

function onMessageHandler(target: string, senderData: tmi.Userstate, message: string, botSelf: boolean): void {
    if (botSelf)
        return;

    var commandName: string = "";
    var error: number = NO_ERROR;
    var commandExecuted: boolean = false;
    var params: string | string[] | null = null;

    if (message.indexOf('!') === 0) {
        commandName = getCommand(message);
        const matchingAvailableCommand = availableCommands.find(x =>
            (!Array.isArray(x.trigger) && commandName.indexOf(x.trigger.toLowerCase()) === 0) ||
            (!Array.isArray(x.alias) && commandName.indexOf(x.alias.toLowerCase()) === 0) ||
            (Array.isArray(x.alias) && x.alias.some(trigger => commandName.indexOf(trigger.toLowerCase()) === 0))
        );
        if (checkPermission(commandName)) {
            matchingAvailableCommand.execute(params);
            commandExecuted = true;
        }
    } 


    // Check if response is invalid and give a response in chat
    if (message.indexOf('!') === 0 && commandExecuted == false) {
        error = DEFAULT_ERROR;
        client.say(target, `This is an unknown command, for a full list of the available commands please check here.`);
        consoleDebug(commandName, DEBUG, error);
    }
}

function getCommand(msg: string): string {
    const startIndex: number = 1;
    const whiteIndex: number = (msg.indexOf(' ') > 0) ? msg.indexOf(' ') - 1 : msg.length - 1;
    return msg.substr(startIndex, whiteIndex).trim().toLowerCase();
}

function getParam(msg: string, n: number): string[] {
    const args = msg.slice(msg.indexOf(' ') + 1).split(' ');
    var args2 = [];
    for (var i = 0; i < n; i++) {
        args2 = [...args2, args[i]];
    }
    return args2;
}

function consoleDebug(command: string, debug: boolean, err: number = NO_ERROR): void {
    if (debug) {
        switch (err) {
            case NO_ERROR:
                console.log(`* Executed ${command} command`);
                break;

            case DEFAULT_ERROR:
                console.log(`* Unknown command ${command}`);
                break;

            case API_ERROR:
                console.log(command);
                break;
        }
    }
}

function checkPermission(command: string): boolean {
    return true;
}
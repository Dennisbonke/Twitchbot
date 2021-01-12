import * as tmi from 'tmi.js';
import { config } from './config.model.js';
import * as axios from 'axios';

// List of constants/MACROS
const DEBUG: boolean = true;
const NO_ERROR: number = 0;
const DEFAULT_ERROR: number = 1;
const API_ERROR: number = 2;
const PERM_ERROR: number = 3;

const client: tmi.Client = tmi.client(config);

client.connect();

client.on('message', onMessageHandler);
client.on('connected', onConnectedHandler);
client.on('subscription', onConnectedHandler);
client.on('resub', onConnectedHandler);
client.on('raided', onConnectedHandler);

interface ChatCommand {
    readonly trigger: string;
    readonly alias: string | string[];
    readonly needsParams: boolean;
    readonly amountParams?: number;
    readonly modsOnly?: boolean;
    readonly subOnly?: boolean;
    readonly everyone?: boolean;

    execute: (params?: string[], sender?: string) => CommandResult
}

interface CommandResult {
    readonly isSuccesfull: boolean;
    readonly error?: string;
}

const sens: ChatCommand = {
    trigger: 'sens',
    alias: 'sens',
    needsParams: false,
    everyone: true,

    execute: () => {
        consoleDebug(sens.trigger, DEBUG);
        client.say(config.channels[0], `10/10 83 - 400 DPI`);
        return { isSuccesfull: true }
    }
}

const twitter: ChatCommand = {
    trigger: 'twitter',
    alias: 'twitter',
    needsParams: false,
    everyone: true,

    execute: () => {
        consoleDebug(twitter.trigger, DEBUG);
        client.say(config.channels[0], `Follow me on twitter for the latest updates and shit and giggles. https://twitter.com/SenpaiR6`);
        return { isSuccesfull: true }
    }
}

const shoutout: ChatCommand = {
    trigger: 'shoutout',
    alias: ['so', 'shout'],
    needsParams: true,
    amountParams: 1,
    modsOnly: true,

    execute: (params) => {
        consoleDebug(shoutout.trigger, DEBUG);
        client.say(config.channels[0], `Please go and check out the channel of ${params[0].substr(1)}, by going to the link: https://www.twitch.tv/${params[0].substr(1)} 
        Be sure to give them a follow!!!!`);
        return { isSuccesfull: true }
    }
}

const uptime: ChatCommand = {
    trigger: 'uptime',
    alias: 'live',
    needsParams: false,
    everyone: true,

    execute: () => {
        var time: any = 0;
        return { isSuccesfull: true }
    }
}

const availableCommands: ChatCommand[] = [sens, twitter, shoutout, uptime];

function onConnectedHandler(addr: any, port: any) {
    console.log(`* Connected to ${addr}:${port}`);
}

function onMessageHandler(target: string, senderData: tmi.Userstate, message: string, botSelf: boolean): void {
    if (botSelf)
        return;

    var commandName: string = "";
    var error: number = NO_ERROR;
    var commandExecuted: boolean = false;
    var params: string[];

    if (message.indexOf('!') === 0) {
        commandName = getCommand(message);
        const matchingAvailableCommand = availableCommands.find(x =>
            (!Array.isArray(x.trigger) && commandName.indexOf(x.trigger.toLowerCase()) === 0) ||
            (!Array.isArray(x.alias) && commandName.indexOf(x.alias.toLowerCase()) === 0) ||
            (Array.isArray(x.alias) && x.alias.some(trigger => commandName.indexOf(trigger.toLowerCase()) === 0))
        );
        if (matchingAvailableCommand == undefined) {
            error = DEFAULT_ERROR;
            client.say(target, `This is an unknown command, for a full list of the available commands please check here.`);
            consoleDebug(commandName, DEBUG, error);
        }
        if (checkPermission(matchingAvailableCommand, senderData) && matchingAvailableCommand != undefined) {
            if (matchingAvailableCommand.needsParams) {
                params = getParam(message, matchingAvailableCommand.amountParams);
                matchingAvailableCommand.execute(params);
            } else {
                matchingAvailableCommand.execute();
            }
            commandExecuted = true;
        }
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

            case PERM_ERROR:
                console.log(`* User tried to execute ${command}, but he doesnt have permission`);
        }
    }
}

function checkPermission(command: ChatCommand, sender: tmi.Userstate): boolean {
    if (command.everyone || (command.subOnly && sender.subscriber || sender.mod) || (command.modsOnly && sender.mod) || sender.username === config.channels[0].substr(1)) {
        return true;
    } else {
        client.say(config.channels[0], `Sorry but you dont have permissions to execute the command ${command.trigger}, @${sender.username}`);
        consoleDebug(command.trigger, DEBUG, PERM_ERROR);
        return false;
    }
}
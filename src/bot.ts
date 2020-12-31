import { AnyNaptrRecord } from 'dns';
import * as tmi from 'tmi.js';
import { config } from './config.model.js';

const client = tmi.client(config);

client.connect();

client.on('message', onConnectedHandler);
client.on('connected', onConnectedHandler);
client.on('subscription', onConnectedHandler);
client.on('resub', onConnectedHandler);
client.on('raided', onConnectedHandler);

function onConnectedHandler(addr: any, port: any) {
    console.log(`* Connected to ${addr}:${port}`);
}

function onMessageHandler(target: string, senderData: tmi.Userstate, message: string, botSelf: boolean) {

}
import { Options } from "tmi.js";

type UsernameT = string;
type PasswordT = string;
type ChannelT = string;
type OptionsT = {
    debug: boolean;
};

export default class Config {
    private username: UsernameT;
    private password: PasswordT;
    private channels: ChannelT[];
    private options: OptionsT;
    public readonly prefix: string;

    static fromEnv(envVars: NodeJS.ProcessEnv, channels: ChannelT[]): Config {
        const username = envVars.TWITCH_USERNAME || 'Westlanderz';
        const password = envVars.TWITCH_PASSWORD || 'oauth:01uzdji64d8tjte06bf5ugai3ux5q1';   

        if (process.env.NODE_ENV !== 'production') //typeof username !== 'string' || typeof password !== 'string')
            require('dotenv').config();
            //throw new Error("Missing username or password");
        
        return new this(username, password, channels, {
            debug: envVars.APP_DEBUG === 'true'
        });
    }

    constructor(
        username: UsernameT,
        password: PasswordT,
        channels: ChannelT[],
        options: Partial<OptionsT>
    ) {
        this.username = username;
        this.password = password;
        this.channels = channels;
        this.prefix = '!';
        this.options = {
            debug: options?.debug || false,
        };
    }

    public getUsername(): UsernameT {
        return this.username;
    }

    private getPassword(): PasswordT {
        return this.password;
    }

    private getChannles(): ChannelT[] {
        return this.channels;
    }

    private isDebug(): boolean {
        return !!this.options.debug;
    }

    public convertToTMI(): Options {
        return {
            options: { debug: this.isDebug() },
            connection: {
                reconnect: true,
                secure: true
            },
            identity: {
                username: this.getUsername(),
                password: this.getPassword()
            },
            channels: this.getChannles()
        }
    }
}
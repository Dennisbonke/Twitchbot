import { ChatUserstate, Userstate } from "tmi.js";
import { client } from "../bot";
import Default from "../modules/default";
import { ListCommands } from "../modules/default/commands";
import Module from "../modules/modules";
import SocialsModule from "../modules/socials";
import Command from "./command";
import CommandContext from "./commandContext";

export default class MessageHandler {
    private commands: Command[];
    private modules: Module[];
    private readonly prefix: string;
    private commandClasses: any[];
    private moduleClasses: any[];
    private loadedModules: any[];

    constructor (prefix: string) {
        this.prefix = prefix;
        this.commandClasses = [];
        this.moduleClasses = [
            SocialsModule,
        ];
        this.loadedModules = [
            ,
        ];
        this.commands = this.commandClasses.map((CommandClass) => new CommandClass());
        this.commands.push(new ListCommands(this.commands));
        this.modules = this.moduleClasses.map((ModuleClass) => new ModuleClass());
        this.init_default();  
    }

    public async handleMessage (channel: string, userstate: ChatUserstate, message: string, self: boolean): Promise<void> {
        if (self || !this.isCommand(message))
            return;
            
        const commandContext = new CommandContext(message, this.prefix); 

        if (commandContext.parsedCommandName === 'load' && this.has_perms(userstate, client.channels[0])) {
            if (commandContext.args.length === 0) {
                const modulesUnloaded = [];
                for (let i = 0; i < this.modules.length; i++) {
                    for (let j = 0; j < this.loadedModules.length; j++) {
                        if (this.modules[i] != this.loadedModules[j])
                            modulesUnloaded.push(this.modules[i]);
                    }
                }
                const moduleNames = modulesUnloaded.map(
                    (module) => module.moduleName,
                );           
                await client.say(channel, 'You must provide a module to load, here is a list of modules you can load: ' + moduleNames.join(
                    ', ',
                ));
            } else {
                const module = this.modules.find((module) =>
                    module.moduleName.includes(commandContext.args[0]),
                );
                this.load_module(module, client.channels[0]);
            }
            return;
        } else if (commandContext.parsedCommandName === 'load' && this.has_perms(userstate, client.channels[0])) {
            await client.say(channel, `You have no permission to run this command`);
            return;
        } else if (commandContext.parsedCommandName === 'unload' && this.has_perms(userstate, client.channels[0])) {
            if (commandContext.args.length === 0) {
                const moduleNames = this.loadedModules.map(
                    (module) => module.moduleName,
                );
                
                await client.say(channel, 'You must provide a module to unload, here is a list of modules you can unload: ' + moduleNames.join(
                    ', ',
                ));
            } else {
                const module = this.modules.find((module) =>
                    module.moduleName.includes(commandContext.args[0]),
                );
                this.unload_module(module, client.channels[0]);
            }
            return;
        } else if (commandContext.parsedCommandName === 'unload' && this.has_perms(userstate, client.channels[0])) {
            await client.say(channel, `You have no permission to run this command`);
            return;
        }

        const allowedCommands = this.commands.filter((command) =>
            command.hasPermissionToRun(commandContext),
        );

        const matchedCommand = this.commands.find((command) =>
            command.commandNames.includes(commandContext.parsedCommandName),
        );      
        
        if (!matchedCommand) {
            await client.say(channel, "I don't recognize that command. Try !commands.");
        } else if (!allowedCommands.includes(matchedCommand)) {
            await client.say(channel, "You aren't allowed to use that command.");
        } else {
            await matchedCommand
                .run(commandContext)
                .then(() => {
                    console.log(`* ${matchedCommand.commandNames[0]} executed succesfully`);
                })
                .catch((reason) => {
                    console.log(`Something went wrong with executing ${matchedCommand.commandNames[0]}, ${reason}`);
                });
        }
    }

    private init_default (): void {
        const module = new Default();
        this.commandClasses.push(...module.includedCommands);
        this.update_commands();
        console.log(`* initialised default commands.`);
    }

    private load_module (module: any, channel: string): void {
        this.commandClasses.push(...module.includedCommands);
        this.loadedModules.push(module);
        this.update_commands();
        client.say(channel, `Loaded the module ${module.moduleName}`);
        console.log(`* Loaded the module ${module.moduleName}`);
    }

    private unload_module (module: any, channel: string): void {
        if (module == Default)
            return;
        for (let i = 0; i < this.commandClasses.length; i++) {
            for (let j = 0; j < module.includedCommands.length; j++) {
                if (this.commandClasses[i] == module.includedCommands[j])
                    this.commandClasses.splice(i, 1);
            }
        }
        for (let i = 0; i < this.loadedModules.length; i++) {
            if (module == this.loadedModules[i])
                this.loadedModules.splice(i, 1);  
        }
        this.update_commands();
        client.say(channel, `Unloaded the module ${module.moduleName}`);
        console.log(`* Unloaded the module ${module.moduleName}`);
    }

    private update_commands (): void {
        this.commands = this.commandClasses.map((CommandClass) => new CommandClass());
        this.commands.push(new ListCommands(this.commands));
    }

    private isCommand (message: string): boolean {
        return message.startsWith(this.prefix);
    }

    private has_perms (user: Userstate, channel): boolean {
        if (user["user-type"] === "mod" || user.username === channel.replace("#", "")) 
            return true;
        return false;
    }

    private console_debug(): void {
        
    }
}
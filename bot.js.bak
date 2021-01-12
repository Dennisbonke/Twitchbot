// List of required or included files
const tmi = require('tmi.js');
const axios = require('axios');
const config = require('./config.json');

// List of constants/MACROS
const DEBUG = true;
const NO_ERROR = 0;
const DEFAULT_ERROR = 1;
const API_ERROR = 2;

// Define configuration options.
const opts = {
  identity: {
    username: config.username,
    password: config.password,
  },
  channels: config.channels,
};

// Add the authorization token to every Axios request.
axios.defaults.headers.common['Authorization'] = `Bearer ${config.token}`;

// Create a client with our options
const client = new tmi.client(opts);

// Connect to Twitch
client.connect();


// Eventhandlers
// Register our event handlers (defined below)
client.on('message', onMessageHandler);
client.on('connected', onConnectedHandler);
client.on('subscription', onSubHandler);
client.on('resub', onResubHandler);
client.on('raided', onRaidHandler);

// Called every time a message comes in
function onMessageHandler(target, context, msg, self) {
  // List of constants
  const channel = 'Westlanderz';

  // List of variables
  var commandExecuted = false;
  var error = NO_ERROR;
  var user, check, commandName = "";

  // Ignore messages from the bot
  if (self) return;

  // Set the commandname
  if (msg.indexOf('!') === 0)
    commandName = getCommand(msg);

  // Check what the command is
  switch (commandName) {
    // General Commands
    // Gives back the URL of the list of commands
    case 'commands':
    case 'command':
      client.say(target, `Please check out the full list of commands here: {URL}`);
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;

    // Rolls a dice and puts the number you rolled back in chat
    case 'dice':
    case 'random':
      const num = rollDice();
      client.say(target, `You rolled a ${num}`);
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;

    // Responds with "General Kenobi"
    case 'hello':
    case 'henlo':
      client.say(target, `General Kenobi`);
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;


    // Management commands
    // Command will timeout a certain user for a set time, command needs parameters [user] [time]
    case 'timeout':
      user = getParam(msg, 9);
      check = checkParam(user);
      if (check === 0 && checkMod(context, target)) client.say(target, `/timeout ${user}`);
      else error = setError(target, commandName);
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;

    // Command will ban users for a certain reason, command needs parameters [user] [reason]
    case 'ban':
      user = getParam(msg, 5);
      check = checkParam(user);
      if (check === 0 && checkMod(context, target)) client.say(target, `/ban ${user}`);
      else error = setError(target, commandName);
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;

    // This command will unban a certain user
    case 'unban':
      user = getParam(msg, 5);
      check = checkParam(user);
      if (check === 0 && checkMod(context, target)) client.say(target, `/unban ${user}`);
      else error = setError(target, commandName);
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;


    // Streaminfo commands
    // Gives chat the current uptime
    case 'uptime':
    case 'live':
      var time = 0;
      axios.get(`https://beta.decapi.me/twitch/uptime/${channel}`)
        .then(response => {
          time = response.data;
          if (time == 'undefined') error = setError(target, commandName);
          else if (time == `${channel} is offline`) client.say(target, `${channel} is currently not streaming.`);
          else client.say(target, `${channel} has been live for ${time}.`);
        })
        .catch(err => {
          error = API_ERROR;
        })
        ;
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;

    // Gives the users current followage back in chat
    case 'followage':
      user = context.username;
      var age = 0;
      axios.get(`https://decapi.me/twitch/followage/${channel}/${user}?precision=7`)
        .then(response => {
          age = response.data;
          if (age == 'undefined')
            error = setError(target, commandName);
          else if (age == 'Follow not found')
            client.say(target, `${user} is not following ${channel}.`);
          else if (age == 'A user cannot follow themselfs.')
            client.say(target, `A user cannot follow themselfs.`);
          else
            client.say(target, `${user} has been following ${channel} for ${age}.`);
        })
        .catch(err => {
          error = API_ERROR;
        })
        ;
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;

    // Gives the current amount of subs back in chat
    case 'subs':
    case 'subscribers':
    case 'subcount':
    case 'supersimpais':
      var subs = 0;
      axios.get(`https://decapi.me/twitch/subcount/${channel}`)
        .then(response => {
          subs = response.data;
          if (subs == 'undefined')
            error = setError(target, commandName);
          else
            client.say(target, `${channel} has a total of ${subs} Super Simpais.`);
        })
        .catch(err => {
          error = API_ERROR;
        })
        ;
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;

    // Gives the current amount of followers back in chat.
    case 'follows':
    case 'followers':
    case 'followercount':
    case 'simpais':
      var followers = 0;
      axios.get(`https://decapi.me/twitch/followcount/${channel}`)
        .then(response => {
          followers = response.data;
          if (subs == 'undefined') error = setError(target, commandName);
          else client.say(target, `${channel} has a total of ${followers} Simpais.`);
        })
        .catch(err => {
          error = API_ERROR;
        })
        ;
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;

    // Checks a users watchtime -> works but gives empty result api probs broken
    case 'watchtime':
      user = context.username;
      axios.get(`https://api.streamelements.com/kappa/v2/points/${channel}/watchtime`)
        .then(response => {
          console.log(response.data);
        })
        .catch(err => {
          console.log(err);
        })
        ;
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;


    // R6 commands
    // Returns my current siege sens
    case 'sens':
      client.say(target, `6/6 83 - 400 DPI`);
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;

    // Returns in chat the link to a clip of me showing my current in-game settings
    case 'settings':
      //client.say(target, `Here are my current siege settings: {URL}`);
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = false;
      break;


    // Socials commands
    // Responds with my twitter page
    case 'twitter':
      client.say(target, `Follow me on twitter for the latest updates and shit and giggles. https://twitter.com/SenpaiR6`);
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;

    // Responds with a link to my discord server
    case 'discord':
      client.say(target, `You can join my discord by clicking the panel down below or by using this link: https://discord.gg/8FjTkfy`);
      consoleDebug(commandName, DEBUG, error);
      commandExecuted = true;
      break;
  }

  // Check if response is invalid and give a response in chat
  if (msg.indexOf('!') === 0 && commandExecuted == false) {
    error = DEFAULT_ERROR;
    client.say(target, `This is an unknown command, for a full list of the available commands please check here.`);
    consoleDebug(commandName, DEBUG, error);
  }
}

// Called every time the bot connects to Twitch chat
function onConnectedHandler(addr, port) {
  console.log(`* Connected to ${addr}:${port}`);
}

// Called every time someone subscribes for the first time
function onSubHandler(target, user, method, msg, context) {
  if (method.prime == true)
    client.say(target, `Many thanks ${user} for the prime subscription. And welcome to the Super Simpais.`);
  else {
    switch (method.plan) {
      // Checks for tier 1 sub
      case "1000":
        client.say(target, `Many thanks ${user} for the tier 1 subscription. And welcome to the Super Simpais.`);
        break;

      // Checks for tier 2 sub
      case "2000":
        client.say(target, `Many thanks ${user} for the tier 2 subscription. And welcome to the Super Simpais.`);
        break;

      //Checks for tier 3 sub
      case "3000":
        client.say(target, `Many thanks ${user} for the tier 3 subscription. And welcome to the Super Simpais.`);
        break;
    }
  }
}

// Called every time someone resubs
function onResubHandler(target, user, time, msg, context, method) {
  if (method.prime == true)
    client.say(target, `Many thanks ${user} for the prime subscription for a big ${time} and ${context["msg-param-streak-months"]} in a row.`);
  else {
    switch (method.plan) {
      // Checks for tier 1 sub
      case "1000":
        client.say(target, `Many thanks ${user} for the tier 1 subscription for a big ${time} and ${context["msg-param-streak-months"]} in a row.`);
        break;

      // Checks for tier 2 sub
      case "2000":
        client.say(target, `Many thanks ${user} for the tier 2 subscription for a big ${time} and ${context["msg-param-streak-months"]} in a row.`);
        break;

      //Checks for tier 3 sub
      case "3000":
        client.say(target, `Many thanks ${user} for the tier 3 subscription for a big ${time} and ${context["msg-param-streak-months"]} in a row.`);
        break;
    }
  }
}

// Called every time someone raided the channel
function onRaidHandler(target, streamer, views) {
  client.say(target, `Look at this madlad just raiding this shitter. Many thanks ${streamer} for the raid with ${views} viewers.`);
}


// Checking management functions
// Checks what user perms are
function checkMod(user, channel) {
  if (user["user-type"] === "mod" || user.username === channel.replace("#", "")) return true;
  return false;
}

// This function will log in console if debug mode is enabled
function consoleDebug(command, debug, err) {
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

// This function will extract the parameters of a given command
function getParam(msg) {

}

// This function checks the params for a given command
function checkParam(param) {
  if (param != '') return 0;
  return -1;
}

// Gets he first word of the message send in chat
function getCommand(msg) {
  const startIndex = 1;
  const whiteIndex = (msg.indexOf(' ') > 0) ? msg.indexOf(' ') - 1 : msg.length - 1;
  return msg.substr(startIndex, whiteIndex).trim();
}

// This function displays an error message in chat
function setError(channel, command) {
  client.say(channel, `An error occurred during the execution of the following command: ${command}, please try again later. 
  If this error keeps occurring please let the bot owner know.`);
  return DEFAULT_ERROR;
}


// General functions
// This function outputs a number between 1 and 6, just like a dice
function rollDice() {
  const sides = 6;
  return Math.floor(Math.random() * sides) + 1;
}

function duelPlayer(user, channel) {

}
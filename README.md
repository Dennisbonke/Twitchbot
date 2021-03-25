# Twitchbot
Making a general purpose twitch chat bot.

## Dependencies
This bot is dependent on having [libsockpp](https://github.com/fpagliughi/sockpp), [libasync](https://github.com/managarm/libasync) and [frigg](https://github.com/managarm/frigg) installed. You can install libsockpp from your package manager if available, or alternatively compile it from source. For frigg and libasync one can copy the folders with the headers to `/usr/include`. To build the bot, `meson` and `ninja` are required as well, install these from your package manager.

## Compilation
To compile this project, run the following commands
```
- mkdir build
- cd build
- meson ..
- ninja
```

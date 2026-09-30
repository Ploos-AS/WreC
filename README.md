# WreC

WreC is a C-based IRC bot with embedded Wren scripting.

## M0 goals

- Small, portable C IRC core
- Embed the Wren VM through its C API
- Event-driven scripting for IRC commands and messages
- PBMP integration boundary defined from the start
- Hooks for BotWeb and BotAI
- Standalone-first operation
- Deterministic tests for parser, dispatch and Wren bindings

## Initial architecture

```text
IRC network
    |
    v
C IRC core
    |
    +-- parser/state
    +-- command/event dispatcher
    +-- PBMP adapter
    +-- BotWeb/BotAI hooks
    |
    `-- Wren VM
         +-- commands
         +-- events
         +-- timers
         `-- modules
```

## M0 scripting surface

The first Wren API should expose a compact IRC-facing module with operations for:

- say / notice
- join / part
- nickname handling
- event registration
- command registration
- timers
- read-only bot/channel/user state

## Design principles

1. C first: the bot core remains understandable and portable.
2. Wren is a first-class extension language, not a configuration gimmick.
3. Script failures are isolated from the IRC core.
4. PBMP integration is optional at runtime, but first-class in the architecture.
5. BotWeb and BotAI remain optional components.
6. No malware or offensive payloads are stored in the repository.

## License

Software: MIT.

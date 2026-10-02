# WreC

WreC is a C-based IRC bot with embedded Wren scripting.

## M1 status

The standalone runtime now has a qualified language-neutral contract for IRC commands/events and deterministic timers. See docs/BOT_RUNTIME_CONTRACT.md.

- IRC command/event dispatch: qualified
- Timer registry and named handlers: qualified
- Wren timer bindings: qualified
- AFTER / EVERY / CANCEL semantics: qualified
- CI regression coverage: enabled

## M0 goals

- Small, portable C IRC core
- Embed the Wren VM through its C API
- Event-driven scripting for IRC commands and messages
- PBMP integration boundary defined from the start
- Hooks for BotWeb and BotAI
- Standalone-first operation
- Standalone qualification with PBMP, BotWeb, BotAI and BotLogic disabled
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
5. BotWeb, BotAI and BotLogic remain optional components.
6. No malware or offensive payloads are stored in the repository.


Standalone qualification requires the complete bot, including its Wren runtime, to build and pass its test suite with PBMP, BotWeb, BotAI and BotLogic disabled. The language/runtime is a required part of WreC, not an optional integration. PBMP, BotWeb, BotAI and BotLogic must remain optional build/runtime dependencies.

## License

Software: MIT.

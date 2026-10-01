# Bot Runtime Contract

WreC implements the common standalone bot-runtime contract using Wren-native bindings.

## M1 qualification

| Capability | Contract | WreC |
|---|---|---|
| IRC commands | C dispatcher -> language callback | PASS |
| IRC events | C event dispatcher -> language callback | PASS |
| One-shot timer | Timer.after | PASS |
| Repeating timer | Timer.every | PASS |
| Timer cancellation | Timer.cancel | PASS |
| Named callback | Wren method name | PASS |
| Deterministic polling | explicit runtime clock | PASS |
| CI qualification | make test | PASS |

## Wren timer API

    class Bot {
      static tick() {
        IRC.say("#ploos", "tick")
      }
    }
    var timerId = Timer.every(60000, "tick")
    Timer.cancel(timerId)

Timer IDs are returned as Wren numbers. Timer callback contexts are owned by the runtime and released on cancellation.

The C timer registry is language-neutral; Wren only supplies the callback binding.

## Contract rule

Future language features should add a deterministic test at the language-binding layer and retain the same runtime semantics.


## Command and event context

The common contract exposes the same IRC context to every language binding:

- command: command name, sender nick, target, and original message text
- event: event name, sender nick, target, and event text/payload

The exact call syntax remains language-native. Bindings must not invent different semantics for these fields. A handler may ignore fields it does not need.

The runtime keeps the parsed `irc_event` as the canonical source of truth; language adapters translate that context without changing its meaning.

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

class Hello {
  static onHello(nick, channel) {
    IRC.say(channel, "Hello %(nick) from WreC!")
  }
}

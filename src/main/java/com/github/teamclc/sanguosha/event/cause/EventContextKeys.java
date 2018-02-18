package com.github.teamclc.sanguosha.event.cause;

import com.github.teamclc.sanguosha.card.Card;
import com.github.teamclc.sanguosha.player.Player;

public interface EventContextKeys {
    EventContextKey<Player> PLAYER = new EventContextKey<>("player", Player.class);
    EventContextKey<Card> CARD = new EventContextKey<>("card", Card.class);
}

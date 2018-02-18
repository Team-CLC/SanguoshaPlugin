package com.github.teamclc.sanguosha.event.card;

import com.github.teamclc.sanguosha.card.Card;
import com.github.teamclc.sanguosha.event.Event;

public class CardSendEvent extends Event {
    private Card card;

    public CardSendEvent(Card card) {
        super("card_send");
        this.card = card;
    }

    public Card getCard() {
        return card;
    }

    public void setCard(Card card) {
        this.card = card;
    }
}

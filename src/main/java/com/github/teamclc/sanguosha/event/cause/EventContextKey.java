package com.github.teamclc.sanguosha.event.cause;

public class EventContextKey<T> {
    private final String id;
    private Class<T> type;

    public EventContextKey(String id, Class<T> type) {
        this.id = id;
        this.type = type;
    }

    public String getId() {
        return id;
    }

    public Class<T> getType() {
        return type;
    }

    public void setType(Class<T> type) {
        this.type = type;
    }
}

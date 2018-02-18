package com.github.teamclc.sanguosha.event;

import cc.huajistudio.aeb.Cancelable;
import com.github.teamclc.sanguosha.event.cause.Cause;

public class Event implements Cancelable {
    private boolean cancelled = false;
    private final String registryName;
    private Cause cause;

    public Event(Cause cause, String registryName) {
        this.cause = cause;
        this.registryName = registryName;
    }

    public String getRegistryName() {
        return registryName;
    }

    public Cause getCause() {
        return cause;
    }

    public void setCause(Cause cause) {
        this.cause = cause;
    }

    @Override
    public boolean isCancelled() {
        return cancelled;
    }

    public void cancel() {
        cancelled = true;
    }
}

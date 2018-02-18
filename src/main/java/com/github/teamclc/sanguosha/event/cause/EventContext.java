package com.github.teamclc.sanguosha.event.cause;

import java.util.*;

@SuppressWarnings("unchecked")
public class EventContext {
    private Map<EventContextKey<?>, Object> objectMap;

    public EventContext(Map<EventContextKey<?>, Object> objectMap) {
        this.objectMap = objectMap;
    }

    public <T> Optional<T> get(EventContextKey<T> key) {
        return Optional.ofNullable((T) objectMap.get(key));
    }

    public <T> T require(EventContextKey<T> key) {
        final Optional<T> optional = get(key);
        if (optional.isPresent()) {
            return optional.get();
        }
        throw new NoSuchElementException(String.format("Could not retrieve value for key '%s'", key.getId()));
    }

    /**
     * Gets whether the provided {@link EventContextKey} is included in this
     * context.
     *
     * @param key The context key to check
     * @return True if the key is used and there is an entry for it
     */
    public boolean containsKey(EventContextKey<?> key) {
        return this.objectMap.containsKey(key);
    }

    /**
     * Gets all {@link EventContextKey}s present in this context.
     *
     * @return All present keys
     */
    public Set<EventContextKey<?>> keySet() {
        return this.objectMap.keySet();
    }

    /**
     * Gets this event context as a {@link Map} of EventContextKeys to Objects.
     *
     * @return A map view of this context
     */
    public Map<EventContextKey<?>, Object> asMap() {
        return this.objectMap;
    }

    @Override
    public boolean equals(Object object) {
        if (object == this) {
            return true;
        }
        if (!(object instanceof EventContext)) {
            return false;
        }
        EventContext ctx = (EventContext) object;
        for (Map.Entry<EventContextKey<?>, Object> entry : this.objectMap.entrySet()) {
            Object other = ctx.objectMap.get(entry.getKey());
            if (other == null) {
                return false;
            }
            if (!entry.getValue().equals(other)) {
                return false;
            }
        }
        return true;
    }

    @Override
    public int hashCode() {
        return this.objectMap.hashCode();
    }

    @Override
    public String toString() {
        StringJoiner joiner = new StringJoiner(", ");
        for (Map.Entry<EventContextKey<?>, Object> entry : this.objectMap.entrySet()) {
            joiner.add("\"" + entry.getKey().getId() + "\"=" + entry.getValue().toString());
        }
        return "Context[" + joiner.toString() + "]";
    }

    public static Builder builder() {
        return new Builder();
    }

    public static class Builder {
        private Map<EventContextKey<?>, Object> objectMap = new HashMap<>();

        public <T> Builder add(EventContextKey<T> key, T value) {
            objectMap.put(key, value);
            return this;
        }

        public Builder reset() {
            objectMap.clear();
            return this;
        }

        public EventContext build() {
            return new EventContext(objectMap);
        }
    }
}

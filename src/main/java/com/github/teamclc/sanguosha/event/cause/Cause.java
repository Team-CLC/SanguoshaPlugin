package com.github.teamclc.sanguosha.event.cause;

import com.google.common.collect.ImmutableList;

import java.util.*;

import static com.google.common.base.Preconditions.checkArgument;
import static com.google.common.base.Preconditions.checkNotNull;
import static com.google.common.base.Preconditions.checkState;

@SuppressWarnings("unchecked")
public class Cause implements Iterable<Object> {
    private final Object[] cause;
    private final EventContext context;
    private ImmutableList<Object> immutableCauses;

    public Cause(Object[] cause, EventContext context) {
        this.cause = cause;
        this.context = context;
    }

    public Object root() {
        return this.cause[0];
    }

    /**
     * Gets the first <code>T</code> object of this {@link Cause}, if available.
     *
     * @param target The class of the target type
     * @param <T> The type of object being queried for
     * @return The first element of the type, if available
     */
    public <T> Optional<T> first(Class<T> target) {
        for (Object aCause : this.cause) {
            if (target.isInstance(aCause)) {
                return Optional.of((T) aCause);
            }
        }
        return Optional.empty();
    }

    /**
     * Gets the last object instance of the {@link Class} of type
     * <code>T</code>.
     *
     * @param target The class of the target type
     * @param <T> The type of object being queried for
     * @return The last element of the type, if available
     */
    public <T> Optional<T> last(Class<T> target) {
        for (int i = this.cause.length - 1; i >= 0; i--) {
            if (target.isInstance(this.cause[i])) {
                return Optional.of((T) this.cause[i]);
            }
        }
        return Optional.empty();
    }

    /**
     * Gets the object immediately before the object that is an instance of the
     * {@link Class} passed in.
     *
     * @param clazz The class of the object
     * @return The object
     */
    public Optional<?> before(Class<?> clazz) {
        checkArgument(clazz != null, "The provided class cannot be null!");
        if (this.cause.length == 1) {
            return Optional.empty();
        }
        for (int i = 0; i < this.cause.length; i++) {
            if (clazz.isInstance(this.cause[i]) && i > 0) {
                return Optional.of(this.cause[i - 1]);
            }
        }
        return Optional.empty();
    }

    /**
     * Gets the object immediately after the object that is an instance of the
     * {@link Class} passed in.
     *
     * @param clazz The class to type check
     * @return The object after, if available
     */
    public Optional<?> after(Class<?> clazz) {
        checkArgument(clazz != null, "The provided class cannot be null!");
        if (this.cause.length == 1) {
            return Optional.empty();
        }
        for (int i = 0; i < this.cause.length; i++) {
            if (clazz.isInstance(this.cause[i]) && i + 1 < this.cause.length) {
                return Optional.of(this.cause[i + 1]);
            }
        }
        return Optional.empty();
    }

    /**
     * Returns whether the target class matches any object of this {@link Cause}
     * .
     *
     * @param target The class of the target type
     * @return True if found, false otherwise
     */
    public boolean containsType(Class<?> target) {
        checkArgument(target != null, "The provided class cannot be null!");
        for (Object aCause : this.cause) {
            if (target.isInstance(aCause)) {
                return true;
            }
        }

        return false;
    }

    /**
     * Checks if this cause contains of any of the provided {@link Object}. This
     * is the equivalent to checking based on {@link #equals(Object)} for each
     * object in this cause.
     *
     * @param object The object to check if it is contained
     * @return True if the object is contained within this cause
     */
    public boolean contains(Object object) {
        for (Object aCause : this.cause) {
            if (aCause.equals(object)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Gets an {@link ImmutableList} of all objects that are instances of the
     * given {@link Class} type <code>T</code>.
     *
     * @param <T> The type of objects to query for
     * @param target The class of the target type
     * @return An immutable list of the objects queried
     */
    public <T> List<T> allOf(Class<T> target) {
        ImmutableList.Builder<T> builder = ImmutableList.builder();
        for (Object aCause : this.cause) {
            if (target.isInstance(aCause)) {
                builder.add((T) aCause);
            }
        }
        return builder.build();
    }

    /**
     * Gets an immutable {@link List} with all object causes that are not
     * instances of the provided {@link Class}.
     *
     * @param ignoredClass The class of object types to ignore
     * @return The list of objects not an instance of the provided class
     */
    public List<Object> noneOf(Class<?> ignoredClass) {
        ImmutableList.Builder<Object> builder = ImmutableList.builder();
        for (Object cause : this.cause) {
            if (!ignoredClass.isInstance(cause)) {
                builder.add(cause);
            }
        }
        return builder.build();
    }

    /**
     * Gets an {@link List} of all causes within this {@link Cause}.
     *
     * @return An immutable list of all the causes
     */
    public List<Object> all() {
        if (this.immutableCauses == null) {
            this.immutableCauses = ImmutableList.copyOf(this.cause);
        }
        return this.immutableCauses;
    }

    /**
     * Creates a new {@link Cause} where the objects are added at the end of the
     * cause array of objects.
     *
     * @param additional The additional object to add
     * @return The new cause
     */
    public Cause with(Object additional) {
        checkNotNull(additional, "No null arguments allowed!");
        List<Object> list = new ArrayList<>();
        list.add(additional);
        return with(list);
    }

    /**
     * Creates a new {@link Cause} where the objects are added at the end of the
     * cause array of objects.
     *
     * @param additional The additional object to add
     * @param additionals The remaining objects to add
     * @return The new cause
     */
    public Cause with(Object additional, Object... additionals) {
        List<Object> list = new ArrayList<>();
        list.add(additional);
        for (Object object : additionals) {
            checkNotNull(object, "Cannot add null objects!");
            list.add(object);
        }
        return with(list);
    }

    /**
     * Creates a new {@link Cause} where the objects are added at the end of the
     * cause array of objects.
     *
     * @param iterable The additional objects
     * @return The new cause
     */
    public Cause with(Iterable<Object> iterable) {
        Cause.Builder builder = new Builder().from(this);
        for (Object o : iterable) {
            checkNotNull(o, "Cannot add null causes");
            builder.append(o);
        }
        return builder.build(this.context);
    }

    /**
     * Merges this cause with the other cause.
     *
     * @param cause The cause to merge with this
     * @return The new merged cause
     */
    public Cause with(Cause cause) {
        Cause.Builder builder = builder().from(this);
        for (int i = 0; i < cause.cause.length; i++) {
            builder.append(cause.cause[i]);
        }
        return builder.build(this.context);
    }

    public static Builder builder() {
        return new Builder();
    }

    @Override
    public Iterator<Object> iterator() {
        return new Itr();
    }

    @Override
    public boolean equals(Object object) {
        if (object instanceof Cause) {
            Cause cause = ((Cause) object);
            return Arrays.equals(this.cause, cause.cause);
        }
        return false;
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(this.cause);
    }

    @Override
    public String toString() {
        String causeString = "Cause[Context=" + this.context.toString() + ", Stack={";
        StringJoiner joiner = new StringJoiner(", ");
        for (int i = 0; i < this.cause.length; i++) {
            joiner.add(this.cause[i].toString());
        }
        return causeString + joiner.toString() + "}]";
    }

    private class Itr implements Iterator<Object> {

        private int index = 0;

        Itr() { }

        @Override
        public Object next() {
            if (this.index >= Cause.this.cause.length) {
                throw new NoSuchElementException();
            }
            return Cause.this.cause[this.index++];
        }

        @Override
        public boolean hasNext() {
            return this.index != Cause.this.cause.length;
        }
    }

    public static final class Builder {
        final List<Object> causes = new ArrayList<>();

        Builder() {
        }

        /**
         * Appends the specified object to the cause.
         *
         * @param cause The object to append to the cause.
         * @return The modified builder, for chaining
         */
        public Builder append(Object cause) {
            checkNotNull(cause, "Cause cannot be null!");
            if (!this.causes.isEmpty() && this.causes.get(this.causes.size() - 1) == cause) {
                return this;
            }
            this.causes.add(cause);
            return this;
        }

        /**
         * Inserts the specified object into the cause.
         *
         * @param position The position to insert into
         * @param cause The object to insert into the cause
         * @return The modified builder, for chaining
         */
        public Builder insert(int position, Object cause) {
            checkNotNull(cause, "Cause cannot be null!");
            this.causes.add(position, cause);
            return this;
        }

        /**
         * Appends all specified objects onto the cause.
         *
         * @param causes The objects to add onto the cause
         * @return The modified builder, for chaining
         */
        public Builder appendAll(Collection<Object> causes) {
            checkNotNull(causes, "Causes cannot be null!");
            causes.forEach(this::append);
            return this;
        }

        /**
         * Constructs a new {@link Cause} with information added to the builder.
         *
         * @param ctx The context to build the cause with
         * @return The built cause
         */
        public Cause build(EventContext ctx) {
            checkState(!this.causes.isEmpty(), "Cannot create an empty Cause!");
            return new Cause(this.causes.toArray(), ctx);
        }

        public Builder from(Cause value) {
            this.causes.addAll(Arrays.asList(value.cause));
            return this;
        }

        public Builder reset() {
            this.causes.clear();
            return this;
        }
    }
}

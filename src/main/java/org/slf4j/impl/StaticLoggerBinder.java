package org.slf4j.impl;

import com.github.teamclc.sanguosha.log.CoolQLoggerFactory;
import org.slf4j.ILoggerFactory;

public class StaticLoggerBinder {
    private static final StaticLoggerBinder SINGLETON = new StaticLoggerBinder();
    private static final String loggerFactoryClassStr = CoolQLoggerFactory.class.getName();
    public static String REQUESTED_API_VERSION = "1.7.13";
    private final CoolQLoggerFactory factory;

    public static StaticLoggerBinder getSingleton() {
        return SINGLETON;
    }

    public StaticLoggerBinder() {
        this.factory = new CoolQLoggerFactory();
    }

    public ILoggerFactory getLoggerFactory() {
        return factory;
    }

    public static String getLoggerFactoryClassStr() {
        return loggerFactoryClassStr;
    }
}

package com.github.teamclc.sanguosha.log

import com.github.teamclc.sanguosha.NativeBridge
import org.slf4j.ILoggerFactory
import org.slf4j.Logger
import org.slf4j.helpers.MarkerIgnoringBase
import org.slf4j.helpers.MessageFormatter
import java.io.ByteArrayOutputStream
import java.io.PrintStream

private val loggers = HashMap<String, CoolQLogger>()
class CoolQLoggerFactory: ILoggerFactory {
    override fun getLogger(name: String): Logger {
        if (!loggers.containsKey(name)) loggers[name] = CoolQLogger(name)
        return loggers[name]!!
    }
}

private const val WARN_PRIORITY = 20
private const val ERROR_PRIORITY = 20
private const val DEBUG_PRIORITY = 20
private const val INFO_PRIORITY = 20
class CoolQLogger(category: String): MarkerIgnoringBase() {
    init {
        name = category
    }

    override fun warn(msg: String) = NativeBridge.log(msg, WARN_PRIORITY, name)
    override fun warn(format: String, arg: Any?) = warn(MessageFormatter.format(format, arg).message)
    override fun warn(format: String, arg1: Any?, arg2: Any?) = warn(MessageFormatter.format(format, arg1, arg2).message)
    override fun warn(format: String, vararg arguments: Any) = warn(MessageFormatter.arrayFormat(format, arguments).message)
    override fun warn(msg: String, t: Throwable) =
            warn(msg + ": " + ByteArrayOutputStream().also { it.let(::PrintStream).let(t::printStackTrace) }.toString("UTF-8"))

    override fun info(msg: String) = NativeBridge.log(msg, INFO_PRIORITY, name)
    override fun info(format: String, arg: Any?) = info(MessageFormatter.format(format, arg).message)
    override fun info(format: String, arg1: Any?, arg2: Any?) = info(MessageFormatter.format(format, arg1, arg2).message)
    override fun info(format: String, vararg arguments: Any) = info(MessageFormatter.arrayFormat(format, arguments).message)
    override fun info(msg: String, t: Throwable) =
            info(msg + ": " + ByteArrayOutputStream().also { it.let(::PrintStream).let(t::printStackTrace) }.toString("UTF-8"))

    override fun error(msg: String) = NativeBridge.log(msg, ERROR_PRIORITY, name)
    override fun error(format: String, arg: Any?) = error(MessageFormatter.format(format, arg).message)
    override fun error(format: String, arg1: Any?, arg2: Any?) = error(MessageFormatter.format(format, arg1, arg2).message)
    override fun error(format: String, vararg arguments: Any) = error(MessageFormatter.arrayFormat(format, arguments).message)
    override fun error(msg: String, t: Throwable) =
            error(msg + ": " + ByteArrayOutputStream().also { it.let(::PrintStream).let(t::printStackTrace) }.toString("UTF-8"))

    override fun debug(msg: String) = NativeBridge.log(msg, DEBUG_PRIORITY, name)
    override fun debug(format: String, arg: Any?) = debug(MessageFormatter.format(format, arg).message)
    override fun debug(format: String, arg1: Any?, arg2: Any?) = debug(MessageFormatter.format(format, arg1, arg2).message)
    override fun debug(format: String, vararg arguments: Any) = debug(MessageFormatter.arrayFormat(format, arguments).message)
    override fun debug(msg: String, t: Throwable) =
            debug(msg + ": " + ByteArrayOutputStream().also { it.let(::PrintStream).let(t::printStackTrace) }.toString("UTF-8"))

    override fun trace(msg: String) = NativeBridge.log(msg, DEBUG_PRIORITY, name)
    override fun trace(format: String, arg: Any?) = trace(MessageFormatter.format(format, arg).message)
    override fun trace(format: String, arg1: Any?, arg2: Any?) = trace(MessageFormatter.format(format, arg1, arg2).message)
    override fun trace(format: String, vararg arguments: Any) = trace(MessageFormatter.arrayFormat(format, arguments).message)
    override fun trace(msg: String, t: Throwable) =
            trace(msg + ": " + ByteArrayOutputStream().also { it.let(::PrintStream).let(t::printStackTrace) }.toString("UTF-8"))

    override fun isErrorEnabled(): Boolean = true
    override fun isWarnEnabled(): Boolean = true
    override fun isDebugEnabled(): Boolean = true
    override fun isInfoEnabled(): Boolean = true
    override fun isTraceEnabled(): Boolean = true
}
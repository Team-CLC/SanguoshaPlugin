@file:JvmName("MainLifeCycle")
package com.github.teamclc.sanguosha

import com.github.teamclc.sanguosha.event.globalEventBus
import com.github.teamclc.sanguosha.parser.GroupMessageReceivedListener

fun globalInit() {
    // TODO Initialize everything here, like registering listeners

    globalEventBus.register(GroupMessageReceivedListener())
}

fun init() {
    // TODO Initialize everything here
}

fun destroy() {
    // TODO Clean everything up here
}

fun openSettings() {
    // TODO Open Settings window here
}
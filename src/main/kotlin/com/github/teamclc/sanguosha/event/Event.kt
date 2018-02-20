@file:JvmName("GlobalEventBus")
package com.github.teamclc.sanguosha.event

import cc.huajistudio.aeb.Cancelable
import cc.huajistudio.aeb.EventBus

@JvmField
val globalEventBus = EventBus()

abstract class CancelableEvent: Cancelable {
    private var cancelled = false
    override fun isCancelled(): Boolean = cancelled

    fun cancel() {
        cancelled = true
    }
}
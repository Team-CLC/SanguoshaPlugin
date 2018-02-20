package com.github.teamclc.sanguosha.parser

import cc.huajistudio.aeb.quickstart.EventSubscriber
import com.github.teamclc.sanguosha.event.CancelableEvent

data class GroupMessageReceivedEvent(
        val message: String,
        val fromQQ: Long,
        val fromGroup: Long
): CancelableEvent()

class GroupMessageReceivedListener {
    @EventSubscriber
    fun onReceived(event: GroupMessageReceivedEvent) {

    }
}
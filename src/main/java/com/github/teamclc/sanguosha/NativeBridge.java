package com.github.teamclc.sanguosha;

import com.github.teamclc.sanguosha.event.GlobalEventBus;
import com.github.teamclc.sanguosha.parser.GroupMessageReceivedEvent;
import org.slf4j.LoggerFactory;

/**
 * Created by Cyl18 on 2/17/2018.
 */

public class NativeBridge {
    public static native void log(String content, int priority, String category);
    public static native int sendGroupMessage(String message, long toGroup);
    public static native int sendPrivateMessage(String message, long toQQ);

    public static void globalInit() {
        MainLifeCycle.globalInit();
    }

    public static void init() {
        LoggerFactory.getLogger("全局").info("已启动。");
        MainLifeCycle.init();
    }

    public static void destroy() {
        LoggerFactory.getLogger("全局").info("已关闭。");
        MainLifeCycle.destroy();
    }

    public static void openSetting() {
        LoggerFactory.getLogger("全局").info("打开设置界面。");
        MainLifeCycle.openSettings();
    }

    public static boolean groupMessageReceived(String message, int subtype, int msgid, long fromQQ, long fromGroup) {
        GlobalEventBus.globalEventBus.post(new GroupMessageReceivedEvent(message, fromQQ, fromGroup));
        return false;
    }
}

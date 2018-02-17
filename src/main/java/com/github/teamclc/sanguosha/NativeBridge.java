package com.github.teamclc.sanguosha;

/**
 * Created by Cyl18 on 2/17/2018.
 */

public class NativeBridge {
    public static native void log(String content, int priority, String category);
    public static native int sendGroupMessage(String message, long toGroup);
    public static native int sendPrivateMesaage(String message, long toQQ);

    public static void init() {
        log("已启动。", 10, "测试");
    }

    public static void destroy() {
        log("已关闭。", 10, "测试");
    }

    public static void openSetting() {
        log("尝试打开设置。", 10, "测试");
    }

    public static boolean groupMessageReceived(String message, int subtype, int msgid, long fromQQ, long fromGroup) {
        if (message.contains("sgstest")) {
            sendGroupMessage("测试Sgs。", fromGroup);
            sendPrivateMesaage("测试Sgs。", fromQQ);
            return true;
        }
        return false;
    }
}

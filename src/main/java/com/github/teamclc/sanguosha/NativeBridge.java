package com.github.teamclc.sanguosha;

/**
 * Created by Cyl18 on 2/17/2018.
 */

public class NativeBridge {
    public static native void log(String content, int priority, String category);
    public static native int sendGroupMessage(String message, long toGroup);
    public static native int sendPrivateMesaage(String message, long toQQ);

    public static void init() {

    }

    public static void destroy() {

    }

    public static void openSetting() {

    }

    public static void groupMessageReceived(String message, int subtype, int msgid, long fromQQ, long fromGroup) {

    }


}

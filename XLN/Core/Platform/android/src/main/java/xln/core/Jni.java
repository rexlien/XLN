package xln.core;

import android.app.Activity;
import android.app.Application;

/**
 * Created by rexlien on 3/14/17.
 */

public final class Jni {

    public static native void AppMain(Activity activity, Application applicaiton, String factoryServiceName);
    public static native void PostExecutorService(String srcID, String message);

}

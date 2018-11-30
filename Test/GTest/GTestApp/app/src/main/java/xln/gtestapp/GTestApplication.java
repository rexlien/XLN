package xln.gtestapp;

import android.app.Application;

import xln.core.XLNActivityLifeCycleCB;

/**
 * Created by rexlien on 23/11/2017.
 */

public class GTestApplication extends Application
{
    static
    {
        System.loadLibrary("XLNGTest");
    }
    @Override
    public void onCreate() {
        super.onCreate();
        this.registerActivityLifecycleCallbacks(new XLNActivityLifeCycleCB("GTestSystemFactoryService"));

    }
}

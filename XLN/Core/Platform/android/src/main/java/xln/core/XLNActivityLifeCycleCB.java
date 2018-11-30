package xln.core;

import android.app.Activity;
import android.app.Application;
import android.os.Bundle;

/**
 * Created by rexlien on 3/16/17.
 */

public class XLNActivityLifeCycleCB implements Application.ActivityLifecycleCallbacks
{

    private boolean mInitialized = false;
    private String mSysFactoryName;


    public XLNActivityLifeCycleCB(String systemFactoryName)
    {
        mSysFactoryName = systemFactoryName;
    }

    @Override
    public void onActivityCreated(Activity activity, Bundle savedInstanceState) {

        if(!mInitialized)
        {
            XLNApplicaiton.mAppContext = activity.getApplicationContext();
            Jni.AppMain(activity, activity.getApplication(), mSysFactoryName);
            mInitialized = true;
        }
    }

    @Override
    public void onActivityStarted(Activity activity) {

    }

    @Override
    public void onActivityResumed(Activity activity) {

    }

    @Override
    public void onActivityPaused(Activity activity) {

    }

    @Override
    public void onActivityStopped(Activity activity) {

    }

    @Override
    public void onActivitySaveInstanceState(Activity activity, Bundle outState) {

    }

    @Override
    public void onActivityDestroyed(Activity activity) {

    }
}

package xln.core;

import android.app.Application;
import android.content.Context;

/**
 * Created by rexlien on 3/16/17.
 */

public class XLNApplicaiton extends Application
{

    public static Context mAppContext = null;

    public static Context getAppContext()
    {
        return mAppContext;
    }

    @Override
    public void onCreate() {

        //LGEngineApplicaiton.getAppContext();
        super.onCreate();
    }
}

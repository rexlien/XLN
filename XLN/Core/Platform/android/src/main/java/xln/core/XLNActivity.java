package xln.core;

import android.app.Activity;
import android.os.Bundle;

/**
 * Created by rexlien on 2/15/17.
 */

public class XLNActivity extends Activity {

    static {
        System.loadLibrary("Core");
    }

    private Activity mNextActivity;

    public XLNActivity(Activity nextActivity)
    {

        super();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        //Jni.MainActivityInitalized(this, this.getApplication());
        //this.startActivity(mNextActivity);
        super.onCreate(savedInstanceState);
    }
}

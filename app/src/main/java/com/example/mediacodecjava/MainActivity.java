package com.example.mediacodecjava;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.Manifest;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import com.wonderful.permissionlibrary.annotation.PermissionCheck;
import com.wonderful.permissionlibrary.annotation.PermissionResult;


public class MainActivity extends AppCompatActivity{

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private static final String TAG = "MainActivity";

    private PlayerThread playerThread;
    private SurfaceView surfaceView;
    private Button play;


    private SurfaceView surfaceViewJni;
    private Button playJni;

    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/demo.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/LoveStory.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/4kTest4.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/video-h265.mkv";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/4k360.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/h265_4k.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/h265_8k.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/video2.m4s";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/h265.qlv";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/4k_h265_video.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/vps_sps_pps.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/36slice_error.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/25slice.mp4";
    private String path = Environment.getExternalStorageDirectory() + "/wonderful/streamSlice25.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/Immersive_high32.mp4";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        permission();

        mediaCodecInfo();

        surfaceView = findViewById(R.id.surface);
        play = findViewById(R.id.play);
        surfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(@NonNull SurfaceHolder holder) {

            }

            @Override
            public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
                if (playerThread == null){
                    playerThread = new PlayerThread();
                }
            }

            @Override
            public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
                if (playerThread != null){
                    playerThread.interrupt();
                }
            }
        });
        play.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                playerThread.start();
            }
        });



        surfaceViewJni = findViewById(R.id.surfaceJni);
        playJni = findViewById(R.id.playJni);
        playJni.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                playVideoThread(surfaceViewJni.getHolder().getSurface(),path);
            }
        });
    }

    private void mediaCodecInfo(){
        Log.d(TAG, "size: " +  MediaCodecList.getCodecCount());
        Log.d(TAG,"Decode");
        for (int i=0; i<MediaCodecList.getCodecCount(); i++){
            MediaCodecInfo codecInfo = MediaCodecList.getCodecInfoAt(i);
            if (codecInfo.isEncoder()) {
                continue;
            }
            Log.d(TAG,codecInfo.getName());
        }

        Log.d(TAG,"Encode");
        for (int i=0; i<MediaCodecList.getCodecCount(); i++){
            MediaCodecInfo codecInfo = MediaCodecList.getCodecInfoAt(i);
            if (codecInfo.isEncoder()) {
                Log.d(TAG,codecInfo.getName());
            }
        }
    }


    class PlayerThread extends Thread{

        @Override
        public void run() {
            playVideo(surfaceView.getHolder().getSurface(),path);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        stop();
    }

    @PermissionResult
    private void start(){
        Log.d(TAG, "PermissionResult,init surface ");
    }

    /**
     * 写一个方法,并使用PermissionCheck注解声明需要申请的权限,然后在需要申请权限的地方调用此方法
     */
    @PermissionCheck(
            permissions = {
                    Manifest.permission.READ_EXTERNAL_STORAGE
            }
    )
    private void permission(){
    }


    public native void playVideo(Surface surface,String path);
    public native void playVideoThread(Surface surface,String path);
    public native void stop();
}
package com.example.mediacodecjava;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.Manifest;
import android.media.MediaCodec;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import android.opengl.GLSurfaceView;
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

import java.io.EOFException;
import java.io.IOException;
import java.nio.ByteBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

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
    private Surface surfaceJni;

    private String path = Environment.getExternalStorageDirectory() + "/wonderful/demo.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/LoveStory.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/4kTest4.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/video-h265.mkv";
    //private String path = Environment.getExternalStorageDirectory() + "/wonderful/4k360.mp4";
    //private String path = Environment.getExternalStorageDirectory() + "/cybervr/res/4kTest4.mp4";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        permission();

        surfaceView = findViewById(R.id.surface);
        play = findViewById(R.id.play);
        surfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(@NonNull SurfaceHolder holder) {

            }

            @Override
            public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
                if (playerThread == null){
                    playerThread = new PlayerThread(holder.getSurface());
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
        surfaceViewJni.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(@NonNull SurfaceHolder holder) {

            }

            @Override
            public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
                surfaceJni = holder.getSurface();
            }

            @Override
            public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

            }
        });
        playJni.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                playVideoThread(surfaceJni,path);
            }
        });
    }


    class PlayerThread extends Thread{

        private Surface surface;

        public PlayerThread(Surface surface){
            this.surface = surface;
        }

        @Override
        public void run() {
            playVideo(surface,path);
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
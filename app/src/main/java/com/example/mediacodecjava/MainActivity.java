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

    private PlayerThreadJni playerThreadJni;
    private SurfaceView surfaceViewJni;
    private Button playJni;

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
                init();
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
                if (playerThreadJni == null){
                    playerThreadJni = new PlayerThreadJni(holder.getSurface());
                }
            }

            @Override
            public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

            }
        });
        playJni.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                playerThreadJni.start();
            }
        });
    }


    class PlayerThreadJni extends Thread{

        private Surface surface;
        private String path = Environment.getExternalStorageDirectory() + "/wonderful/demo.mp4";
        //private String path = Environment.getExternalStorageDirectory() + "/wonderful/LoveStory.mp4";
        //private String path = Environment.getExternalStorageDirectory() + "/wonderful/4kTest4.mp4";
        //private String path = Environment.getExternalStorageDirectory() + "/wonderful/video-h265.mkv";
        //private String path = Environment.getExternalStorageDirectory() + "/wonderful/4k360.mp4";
        //private String path = Environment.getExternalStorageDirectory() + "/cybervr/res/4kTest4.mp4";

        public PlayerThreadJni(Surface surface){
            this.surface = surface;
        }

        @Override
        public void run() {
            playVideo(surface,path);
        }
    }


    class PlayerThread extends Thread{

        private MediaExtractor mediaExtractor;
        private MediaCodec codec;
        private Surface surface;
        private String path = Environment.getExternalStorageDirectory() + "/wonderful/demo.mp4";
        //private String path = Environment.getExternalStorageDirectory() + "/wonderful/LoveStory.mp4";
        //private String path = Environment.getExternalStorageDirectory() + "/wonderful/4kTest4.mp4";
        //private String path = Environment.getExternalStorageDirectory() + "/wonderful/video-h265.mkv";
        //private String path = Environment.getExternalStorageDirectory() + "/wonderful/4k360.mp4";
        //private String path = Environment.getExternalStorageDirectory() + "/cybervr/res/4kTest4.mp4";

        public PlayerThread(Surface surface){
            this.surface = surface;
        }

        @Override
        public void run() {
            try {
                mediaExtractor = new MediaExtractor();
                mediaExtractor.setDataSource(path);
                int count = mediaExtractor.getTrackCount();
                Log.d(TAG, "tracks: " + count);
                for (int i = 0; i < count; i++) {
                    MediaFormat format = mediaExtractor.getTrackFormat(i);
                    String mine = format.getString(MediaFormat.KEY_MIME);
                    Log.d(TAG, "mediaFormat: " + mine);
                    if (mine.startsWith("video/")){
                        mediaExtractor.selectTrack(i);
                        createCodec(mine);
                        config(format,surface);
                        break;
                    }
                }

                //if (codec == null)return;
                startDecode();
                ByteBuffer[] inputBuffers = (ByteBuffer[]) getInputBuffers();
                if (inputBuffers != null){
                    Log.d(TAG, "inputBuffers size: " + inputBuffers.length);
                }else {
                    Log.d(TAG, "inputBuffers is null");
                }

                ByteBuffer[] outputBuffers = (ByteBuffer[]) getOutputBuffers();
                MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();

                boolean isEOS = false;

                //long startTime = System.currentTimeMillis();
                long timeDif = -1;

                Log.d(TAG, "loop...");
                while (!isInterrupted()){
                    loopBegin();

                    if (!isEOS){
                        Log.d(TAG, "dequeueInputBuffer...");
                        int index = dequeueInputBuffer(1000);
                        if (index >=0){
                            Log.d(TAG, "index " + index);
                            dequeueInputBufferOk();
                            ByteBuffer inputBuffer = inputBuffers[index];
                            int sampleSize = mediaExtractor.readSampleData(inputBuffer, 0);
                            Log.d(TAG, "sampleSize " + sampleSize);
                            if (sampleSize <0){
                                isEOS = true;
                                queueInputBuffer(index,0,0,0,MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                            }else{
                                queueInputBuffer(index,0,sampleSize,mediaExtractor.getSampleTime(),0);
                                mediaExtractor.advance();
                                Log.d(TAG, "queueInputBuffer: ");
                            }
                        }
                    }

                    int index = dequeueOutputBuffer(info, 1000);

                    if (timeDif == -1){
                        timeDif = System.currentTimeMillis() - info.presentationTimeUs;
                    }

                    Log.d(TAG, "dequeueOutputBuffer: ");
                    switch (index){
                        case MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED:
                            outputBuffers = (ByteBuffer[]) getOutputBuffers();
                            break;
                        case MediaCodec.INFO_OUTPUT_FORMAT_CHANGED:
                            break;
                        case MediaCodec.INFO_TRY_AGAIN_LATER:
                            break;
                        default:
//                            ByteBuffer outputBuffer = outputBuffers[index];
//                            while (info.presentationTimeUs / 1000 > System.currentTimeMillis() - startTime){
//                                try {
//                                    sleep(10);
//                                } catch (InterruptedException e) {
//                                    e.printStackTrace();
//                                    break;
//                                }
//                            }

//                            long delay = timeDif + info.presentationTimeUs / 1000 - System.currentTimeMillis();
//                            if (delay >0){
//                                Log.d(TAG, "sleep: %d" + delay);
//                                sleep(delay);
//                            }
//                            Log.d(TAG, "releaseOutputBuffer: ");
//                            sleep(50);
                            releaseOutputBuffer(index,true);
                            break;
                    }

                    if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0){
                        break;
                    }

                    loopEnd();
                }

                codec.stop();
                codec.release();
                mediaExtractor.release();

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
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

    public native void init();
    public native long createCodec(String mine);
    public native void config(MediaFormat format,Surface surface);
    public native void startDecode();
    public native int dequeueInputBuffer(long time);
    public native void dequeueInputBufferOk();
    public native void queueInputBuffer(int index,int offset, int size, long presentationTimeUs, int flags);
    public native int dequeueOutputBuffer(MediaCodec.BufferInfo info, long timeoutUs);
    public native int releaseOutputBuffer(int index,boolean render);
    public native Object getInputBuffers();
    public native Object getOutputBuffers();
    public native void loopBegin();
    public native void loopEnd();


    public native void playVideo(Surface surface,String path);
}
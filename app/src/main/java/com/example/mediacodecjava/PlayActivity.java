package com.example.mediacodecjava;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.Window;
import android.widget.TextView;

public class PlayActivity extends AppCompatActivity {

    private TextView textView;

    private Handler handler = new Handler(new Handler.Callback(){
        @Override
        public boolean handleMessage(@NonNull Message msg) {
            textView.setText((String) msg.obj);
            return true;
        }
    });

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_play);
        SurfaceView surfaceView = findViewById(R.id.surface);
        textView = findViewById(R.id.status);

        Intent intent = getIntent();
        String path = intent.getStringExtra("path");
        int mode = intent.getIntExtra("mode",1);

        surfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(@NonNull SurfaceHolder holder) {

            }

            @Override
            public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
                if(mode == 0){
                    new Thread(){
                        @Override
                        public void run() {
                            playFull(holder.getSurface(),getResources().getAssets(),path,mode);
                        }
                    }.start();
                }else {
                    playFull(holder.getSurface(),getResources().getAssets(),path,mode);
                }
            }

            @Override
            public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

            }
        });
    }

    public void updateStatus(String info){
        Message message = handler.obtainMessage();
        message.obj = info;
        message.sendToTarget();
    }

    public native void playFull(Surface surface, AssetManager assetManager, String path,int mode);
}
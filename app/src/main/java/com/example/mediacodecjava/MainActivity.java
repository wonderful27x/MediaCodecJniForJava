package com.example.mediacodecjava;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.Manifest;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;

import com.example.mediacodecjava.custom_view.ReSpinner;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.wonderful.permissionlibrary.annotation.PermissionCheck;
import com.wonderful.permissionlibrary.annotation.PermissionResult;

import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.List;


public class MainActivity extends AppCompatActivity{

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private static final String TAG = "MainActivity";

    private SurfaceView surfaceView;
    private Button play;
    private Button playFull;

    private SurfaceView surfaceViewJni;
    private Button playJni;
    private Button playJniFull;

    private EditText customAddress;
    private ReSpinner history;
    private ArrayAdapter<String> spinnerAdapter;
    private List<String> historyList;
    private Gson gson = new Gson();
    private static final String HISTORY_KEY = "historyAddress";
    private static final int MAX_HISTORY_NUM = 10;

    private String dir = Environment.getExternalStorageDirectory() + "/wonderful/";
    private String[] pathArray = {
            dir + "4k_h265_video.mp4",
            dir + "h265_2304_1600.mp4",
            dir + "Immersive_high30_1.mp4",
            dir + "test_4K_360_2D_30fps_sea.mp4",
            dir + "Immersive_high45.mp4",
            dir + "VR_2880x1600.mp4",
            dir + "VR_2880x1600Main10.mp4",
            dir + "Immersive_high_temp.mp4",
            "video/demo.mp4",
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        permission();

        mediaCodecInfo();

        customAddress = findViewById(R.id.customAddress);
        history = findViewById(R.id.history);

        historyList = new ArrayList<>();
        historyGet(historyList);
        historySave(false,pathArray);
        spinnerAdapter = new ArrayAdapter<>(this,android.R.layout.simple_spinner_item,historyList);
        history.setAdapter(spinnerAdapter);

        surfaceView = findViewById(R.id.surface);
        play = findViewById(R.id.play);
        playFull = findViewById(R.id.playFull);
        history.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                customAddress.setText(historyList.get(i));
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        play.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                historySave(true,customAddress.getText().toString());
                new Thread(){
                    @Override
                    public void run() {
                        playVideo(surfaceView.getHolder().getSurface(),getResources().getAssets(),customAddress.getText().toString());
                    }
                }.start();
            }
        });
        playFull.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                jumpToFullPlayActivity(customAddress.getText().toString(),0);
            }
        });

        surfaceViewJni = findViewById(R.id.surfaceJni);
        playJni = findViewById(R.id.playJni);
        playJniFull = findViewById(R.id.playJniFull);
        playJni.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                historySave(true,customAddress.getText().toString());
                playVideoThread(surfaceViewJni.getHolder().getSurface(),getResources().getAssets(),customAddress.getText().toString());
            }
        });
        playJniFull.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                jumpToFullPlayActivity(customAddress.getText().toString(),1);
            }
        });

        play.requestFocus();

    }


    private void jumpToFullPlayActivity(String path,int mode){
        Intent intent = new Intent(this,PlayActivity.class);
        intent.putExtra("path", path);
        intent.putExtra("mode", mode);
        startActivity(intent);
    }


    //獲取輸入的歷史地址
    private void historyGet(List<String> historyList){
        SharedPreferences preferences = getSharedPreferences(HISTORY_KEY, MODE_PRIVATE);
        String history = preferences.getString(HISTORY_KEY,null);
        if (history != null){
            Type type = new TypeToken<List<String>>(){}.getType();
            List<String> list = gson.fromJson(history,type);
            if (list != null){
                historyList.clear();
                historyList.addAll(list);
                if (spinnerAdapter != null){
                    spinnerAdapter.notifyDataSetChanged();
                }
            }
        }
    }

    //保存輸入的歷史地址
    private void historySave(boolean reorder,String... addressArray){
        for (String address:addressArray){
            if(historyList.contains(address) && !reorder)continue;
            historyList.remove(address);
            if(historyList.size() < MAX_HISTORY_NUM){
                historyList.add(0,address);
            }else{
                break;
            }
        }
        if (spinnerAdapter != null){
            spinnerAdapter.notifyDataSetChanged();
        }
        String json = gson.toJson(historyList);
        SharedPreferences preferences = getSharedPreferences(HISTORY_KEY, MODE_PRIVATE);
        preferences.edit().putString(HISTORY_KEY,json).apply();
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
//            for(String type:codecInfo.getSupportedTypes()){
//                MediaCodecInfo.CodecCapabilities codecCapabilities = codecInfo.getCapabilitiesForType(type);
//                Log.d(TAG,"max SupportInstance: " + codecCapabilities.getMaxSupportedInstances());
//            }
        }

        Log.d(TAG,"Encode");
        for (int i=0; i<MediaCodecList.getCodecCount(); i++){
            MediaCodecInfo codecInfo = MediaCodecList.getCodecInfoAt(i);
            if (codecInfo.isEncoder()) {
                Log.d(TAG,codecInfo.getName());
            }
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

    public native void playVideo(Surface surface, AssetManager assetManager,String path);
    public native void playVideoThread(Surface surface, AssetManager assetManager,String path);
    public native void stop();
}
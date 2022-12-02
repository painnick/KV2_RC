package com.painnick.kv2camera;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.widget.ImageView;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class MainActivity extends Activity implements View.OnClickListener {

    private static final String TAG = "MainActivity::";

    private HandlerThread stream_thread;
    private Handler stream_handler;
    private ImageView monitor;

    private final int ID_CONNECT = 200;

    private final String STREAM_IP = "192.168.5.18";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        monitor = findViewById(R.id.monitor);

        stream_thread = new HandlerThread("http");
        stream_thread.start();

        stream_handler = new HttpHandler(stream_thread.getLooper());
        stream_handler.sendEmptyMessage(ID_CONNECT);
    }

    @Override
    public void onClick(View v) {
        // Do Nothing
    }

    private class HttpHandler extends Handler {
        public HttpHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case ID_CONNECT:
                    VideoStream();
                    break;
                default:
                    break;
            }
        }
    }

    private void VideoStream() {
        String stream_url = "http://" + STREAM_IP + ":81/stream";

        BufferedInputStream bis = null;
        try {
            URL url = new URL(stream_url);

            try {
                HttpURLConnection huc = (HttpURLConnection) url.openConnection();
                huc.setRequestMethod("GET");
                huc.setConnectTimeout(1000 * 5);
                huc.setReadTimeout(1000 * 5);
                huc.setDoInput(true);
                huc.connect();

                if (huc.getResponseCode() == 200) {
                    InputStream in = huc.getInputStream();
                    InputStreamReader isr = new InputStreamReader(in);
                    BufferedReader br = new BufferedReader(isr);

                    String data;
                    int len;
                    byte[] buffer;

                    while ((data = br.readLine()) != null) {
                        if (data.contains("Content-Type:")) {
                            data = br.readLine();
                            len = Integer.parseInt(data.split(":")[1].trim());

                            bis = new BufferedInputStream(in);
                            buffer = new byte[len];

                            int t = 0;
                            while (t < len) {
                                t += bis.read(buffer, t, len - t);
                            }

                            Bytes2ImageFile(buffer, getExternalFilesDir(Environment.DIRECTORY_PICTURES) + "/0A.jpg");
                            final Bitmap bitmap = BitmapFactory.decodeFile(getExternalFilesDir(Environment.DIRECTORY_PICTURES) + "/0A.jpg");

                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    monitor.setImageBitmap(bitmap);
                                }
                            });
                        }
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } finally {
            try {
                if (bis != null) {
                    bis.close();
                }
                stream_handler.sendEmptyMessageDelayed(ID_CONNECT, 3000);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void Bytes2ImageFile(byte[] bytes, String fileName) {
        try {
            File file = new File(fileName);
            FileOutputStream fos = new FileOutputStream(file);
            fos.write(bytes, 0, bytes.length);
            fos.flush();
            fos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
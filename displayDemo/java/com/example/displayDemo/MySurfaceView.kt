package com.example.displayDemo

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.view.SurfaceHolder
import android.view.SurfaceView

class MySurfaceView(context: Context):
        SurfaceView(context),
        SurfaceHolder.Callback {
    private var drawThread: DrawThread? = null

    init {
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        drawThread = DrawThread(holder)
        drawThread?.running = true
        drawThread?.start()
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        drawThread?.running = false
        drawThread?.join()
    }


    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {

    }

    private class DrawThread(
        private val holder: SurfaceHolder
    ) : Thread() {
        var running = false

        private val paint = Paint().apply {
            color = Color.RED
            isAntiAlias = true
        }

        private var x = 100f
        private var y = 300f
        private var dx = 10f

        override fun run() {
            while (running) {
                var canvas: Canvas? = null

                try {
                    canvas = holder.lockCanvas();

                    if (canvas != null) {
                        canvas.drawColor(Color.WHITE)
                        canvas.drawCircle(x,y,100f, paint)
                        x += dx
                        if (x > canvas.width - 80 || x < 80) {
                            dx = -dx
                        }
                    }
                } finally {
                    if (canvas != null) {
                        holder.unlockCanvasAndPost(canvas)
                    }
                }
                sleep(5)
            }
        }
    }
}




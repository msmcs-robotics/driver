package net.lberrymage.msmcsrobot.controller

import android.os.Bundle
import android.widget.SeekBar
import android.widget.SeekBar.OnSeekBarChangeListener
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {
    private var currentLeftSpeed = 0
    private var currentRightSpeed = 0
    private lateinit var messageService: UdpMessageService

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        messageService = UdpMessageService()

        findViewById<SeekBar>(R.id.left_axis).setOnSeekBarChangeListener(object :
            OnSeekBarChangeListener {
            override fun onProgressChanged(seekbar: SeekBar?, progress: Int, fromUser: Boolean) {
                currentLeftSpeed = progress

                lifecycleScope.launch {
                    messageService.sendSpeed(currentLeftSpeed, currentRightSpeed)
                }
            }

            override fun onStartTrackingTouch(seekbar: SeekBar?) {}

            override fun onStopTrackingTouch(seekbar: SeekBar?) {
                currentLeftSpeed = 0
                seekbar?.setProgress(0, true)
            }
        })
        findViewById<SeekBar>(R.id.right_axis).setOnSeekBarChangeListener(object :
            OnSeekBarChangeListener {
            override fun onProgressChanged(seekbar: SeekBar?, progress: Int, fromUser: Boolean) {
                currentRightSpeed = progress

                lifecycleScope.launch {
                    messageService.sendSpeed(currentLeftSpeed, currentRightSpeed)
                }
            }

            override fun onStartTrackingTouch(seekbar: SeekBar?) {}

            override fun onStopTrackingTouch(seekbar: SeekBar?) {
                currentRightSpeed = 0
                seekbar?.setProgress(0, true)
            }
        })
    }

    override fun onDestroy() {
        super.onDestroy()
        messageService.close()
    }
}

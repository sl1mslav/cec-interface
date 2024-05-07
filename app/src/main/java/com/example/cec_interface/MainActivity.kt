package com.example.cec_interface

import android.os.Build
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import com.example.cec_interface.databinding.ActivityMainBinding
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import java.io.BufferedReader
import java.io.File
import java.io.FileOutputStream
import java.io.InputStreamReader
import kotlin.time.Duration.Companion.seconds


class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        //val libCEC = LibCEC.initialize(true)
        //launchCecClient()
        //binding.sampleText.text = libCEC.isTVOn.toString()
        lifecycleScope.launch {
            delay(5.seconds)
            Runtime.getRuntime().exec(arrayOf("su", "echo 0x40 0x36 > /sys/class/cec/cmd"))
        }
        //runtime.exec("adb root")
        //runtime.exec("echo 0x40 0x36 > /sys/class/cec/cmd")
        binding.turnTvOn.setOnClickListener {
            executeCommandAndLogOutput(command =  "adb shell \"su echo 0x40 0x04 > /sys/class/cec/cmd\"")
        }
        binding.turnTvOff.setOnClickListener {
            executeCommandAndLogOutput(command = "adb shell su \"echo 0xF0 0x36 > /sys/class/cec/cmd\"")
            executeCommandAndLogOutput(command = "adb shell su \"echo 0x40 0x36 > /sys/class/cec/cmd\"")
        }
    }

    private fun launchCecClient() {
        val cecName = if (Build.SUPPORTED_ABIS.contains("x86_64")) "cec_client.sh" else "cec_client_arm.sh"
        val pathToScript = getDir("cec_scripts", 0).absolutePath + File.separator + cecName
        val `in` = resources.openRawResource(R.raw.cec_client)
        var out: FileOutputStream? = null
        try {
            out = FileOutputStream(pathToScript)
            val buff = ByteArray(1024)
            var read: Int
            while ((`in`.read(buff).also { read = it }) > 0) {
                out.write(buff, 0, read)
            }
        } catch (e: Exception) {
            e.printStackTrace()
        } finally {
            try {
                `in`.close()
                out!!.close()
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
        executeCommandAndLogOutput(command = "chmod 777 $pathToScript")
        executeCommandAndLogOutput(command = "echo $pathToScript")
    }

    private fun executeCommandAndLogOutput(
        runtime: Runtime = Runtime.getRuntime(),
        command: String
    ) {
        var reader: BufferedReader? = null
        var result = ""
        try {
            val p = runtime.exec(command)

            /*Timer().schedule(object : TimerTask() {
                override fun run() {
                    reader = BufferedReader(InputStreamReader(p.inputStream))
                    var line: String?

                    while ((reader!!.readLine().also { line = it }) != null) {
                        result += line + "\n"
                        Log.i("Test ", result)
                    }
                }

            },1000,1000)*/

            reader = BufferedReader(InputStreamReader(p.inputStream))
            var line: String?

            while ((reader.readLine().also { line = it }) != null) {
                result += line + "\n"
            }

            p.waitFor()
        } catch (e: java.lang.Exception) {
            e.printStackTrace()
        } finally {
            if (reader != null) try {
                reader.close()
            } catch (e: java.lang.Exception) {
                e.printStackTrace()
            }
        }
        Log.d("TAG", "executeCommandAndLogOutput: $result")
    }
}
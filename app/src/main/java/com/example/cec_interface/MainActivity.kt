package com.example.cec_interface

import android.os.Build
import android.os.Bundle
import android.util.Log
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import com.example.cec_interface.databinding.ActivityMainBinding
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.BufferedReader
import java.io.BufferedWriter
import java.io.DataOutputStream
import java.io.File
import java.io.FileOutputStream
import java.io.InputStreamReader
import java.io.OutputStreamWriter
import kotlin.time.Duration.Companion.seconds


class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    @RequiresApi(Build.VERSION_CODES.O)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        launchCecClient()
        binding.turnTvOn.setOnClickListener {
            turnTvOn()
        }
        binding.turnTvOff.setOnClickListener {
            turnTvOff()
        }
    }

    private fun turnTvOn() {
        exec(listOf("echo 0x40 0x36 > /sys/class/cec/cmd"))
    }

    private fun turnTvOff() {
        exec(listOf("echo 0xF0 0x36 > /sys/class/cec/cmd"))
    }

    private fun exec (cmds: List<String>): ArrayList<String?> {
        // do su process
        val proc = Runtime.getRuntime().exec("su") ?: return arrayListOf(null, null)
        val os = DataOutputStream(proc.outputStream)
        for (cmd in cmds) {
            os.writeBytes(cmd + "\n")
        }
        os.writeBytes("exit\n")
        os.flush()
        os.close()
        proc.waitFor()

        // return resp
        return extractOutput(proc)
    }

    fun exec2 (cmd: String): ArrayList<String?> {
        val proc = Runtime.getRuntime().exec(arrayOf("su", "-c", cmd))
        proc.waitFor()
        return extractOutput(proc)
    }

    private fun extractOutput (proc: Process): ArrayList<String?> {
        // gather resp
        val stdInput = BufferedReader(InputStreamReader(proc.inputStream))
        val stdError = BufferedReader(InputStreamReader(proc.errorStream))
        var s: String? // null included
        var o: String? = ""
        var e: String? = ""
        while (stdInput.readLine().also { s = it } != null) { o += s }
        while (stdError.readLine().also { s = it } != null) { e += s }

        // if blank, cast to null
        if (o.isNullOrBlank()) o = null
        if (e.isNullOrBlank()) e = null

        // debug
        Log.d("TAG", "[success] $o")
        Log.d("TAG", "[error  ] $e")

        // return output, error
        return arrayListOf(o, e)
    }

    private fun launchCecClient() {
        val cecName = if (Build.SUPPORTED_ABIS.contains("x86_64")) "cec_client" else "cec_client_arm"
        val pathToScript = getDir("cec_scripts", 0).absolutePath + File.separator + cecName
        val `in` = resources.openRawResource(R.raw.cec_client_arm)
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
        exec(listOf("file $pathToScript"))
        exec(listOf(pathToScript))
    }
}
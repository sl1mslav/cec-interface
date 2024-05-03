package com.example.cec_interface

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.service.controls.DeviceTypes
import android.widget.TextView
import androidx.lifecycle.lifecycleScope
import com.example.cec_interface.databinding.ActivityMainBinding
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        val libCEC = LibCEC.initialize(true)
        println("yeah!")
        lifecycleScope.launch {
            while (true) {
                binding.sampleText.text = "Powering off TV..."
                libCEC.powerOffTV()
                delay(5000L)
                binding.sampleText.text = "Powering on TV..."
                delay(5000L)
            }
        }
        libCEC.powerOnTV()
    }
}
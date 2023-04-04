package com.easy.jni_sample

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.util.Printer
import com.easy.jni_sample.databinding.ActivityMainBinding
import kotlin.concurrent.thread

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private var name: String = "Dougie"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val s: Printer
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.btnUpdate.setOnClickListener {
            updateName()
            binding.sampleText.text = name
        }

        // Example of a call to a native method
        binding.sampleText.text = getStringFromJNI()
    }

    /**
     * A native method that is implemented by the 'jni_sample' native library,
     * which is packaged with this application.
     */
    external fun getStringFromJNI(): String

    external fun updateName()

    companion object {
        // Used to load the 'jni_sample' library on application startup.
        init {
            System.loadLibrary("jni_sample")
        }
    }

    private fun testAnr() {

        val lock1 = Object()
        val lock2 = Object()

        //子线程持有锁1，想要竞争锁2
        thread {
            synchronized(lock1) {
                Thread.sleep(100)
                synchronized(lock2) {
                    Log.d("ANR", "testAnr: getLock2")
                }
            }
        }

        //主线程持有锁2，想要竞争锁1
        synchronized(lock2) {
            Thread.sleep(100)

            synchronized(lock1) {
                Log.d("ANR", "testAnr: getLock1")
            }
        }
    }
}
package com.example.hollwann.control

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_descontaminador.*
import org.jetbrains.anko.doAsync
import java.io.IOException
import java.util.*

class descontaminador : AppCompatActivity() {
    var address: String? = null
    var tipo_jugador: String? = null
    private var myBluetooth: BluetoothAdapter? = null
    private var btSocket: BluetoothSocket? = null
    private var isBtConnected = false
    //SPP UUID
    private val myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_descontaminador)

        val newint = intent
        address = newint.getStringExtra("device_address") //Se recibe la direccion del bluetooth
        tipo_jugador = newint.getStringExtra("tipo_jugador")
        doAsync {
            //Se conecta el control al bluetooth
            ConnectBT()
        }

        btn_izq.setOnClickListener { env_accion("0") }
        btn_der.setOnClickListener { env_accion("1") }
        btn_accion.setOnClickListener { env_accion("2") }
    }


    private fun env_accion(accion: String) {
        if (btSocket != null) {
            try {
                btSocket!!.outputStream.write((tipo_jugador+accion).toByteArray())
            } catch (e: IOException) {
                msg("Error")
            }
        }
    }

    private fun ConnectBT() {
        var ConnectSuccess = true //if it's here, it's almost connected

        //val progress = progressDialog(message = "Por favor espere un momento.", title="conectando a bluetooth")  //todo

        try {
            if (btSocket == null || !isBtConnected) {
                myBluetooth = BluetoothAdapter.getDefaultAdapter()//Consigue el bluetooth del movil
                val dispositivo = myBluetooth!!.getRemoteDevice(address) as BluetoothDevice//Se conecta a la direccion bluetooth y revisa si es posible conectarse
                btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID)//crea una conexion SPP
                BluetoothAdapter.getDefaultAdapter().cancelDiscovery()
                btSocket!!.connect()//Empieza la conexion
            }
        } catch (e: IOException) {
            ConnectSuccess = false//Se mira la excepcion para saber cual fue el error
            println(e)
        }

        if (!ConnectSuccess) {
            msg("La conexion fallo, intenta de nuevo.")
            finish()
        } else {
            msg("Conectado.")
            isBtConnected = true
        }

    }


    // Forma rapida de llamar toast
    private fun msg(s: String) {
        runOnUiThread {
            Toast.makeText(this@descontaminador, s, Toast.LENGTH_LONG).show()
        }
    }
}
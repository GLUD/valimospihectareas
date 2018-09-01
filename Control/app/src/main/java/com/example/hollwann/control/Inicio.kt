package com.example.hollwann.control

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.Intent
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.TextView
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_inicio.*

class Inicio : AppCompatActivity() {

    //Bluetooth
    private var myBluetooth: BluetoothAdapter? = null
    private var pairedDevices: Set<BluetoothDevice>? = null
    var EXTRA_ADDRESS = "device_address"
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_inicio)

        //se guarda el adaptadaor de bluetooth del dispositivo en una variable
        myBluetooth = BluetoothAdapter.getDefaultAdapter()

        if (myBluetooth == null) {
            //Si el celular no tiene bluetooth se muestra un mensaje.
            Toast.makeText(applicationContext, "Bluetooth no disponible.", Toast.LENGTH_LONG).show()

            //Cierra la aplicacion
            finish()
        } else if (!myBluetooth!!.isEnabled) {
            //Le pide al usuario que encienda el bluetooth
            val turnBTon = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(turnBTon, 1)
        }

        btnPaired.setOnClickListener { pairedDevicesList() }

    }

    private fun pairedDevicesList() {
        pairedDevices = myBluetooth!!.bondedDevices
        val list = arrayListOf<String>()
        //guarda los dispositivos en una lista para mostrarla en un textview
        if ((pairedDevices as MutableSet<BluetoothDevice>?)!!.size > 0) {
            for (bt in (pairedDevices as MutableSet<BluetoothDevice>?)!!) {
                list.add(bt.name + "\n" + bt.getAddress()) //Consigue la direccion y el nombre del dispositivo
            }
        } else {
            //no se encontrarons dispositivos emparejados
            Toast.makeText(applicationContext, "Ningun dispotivio bluetooth emparejado.", Toast.LENGTH_LONG).show()
        }

        val adapter = ArrayAdapter(this, android.R.layout.simple_list_item_1, list)
        devicelist.adapter = adapter
        //listener de clic para lista, cuando se selcciona un disposito lo guarda y pasa al siguiente layout
        devicelist.onItemClickListener = myListClickListener

    }

    private val myListClickListener = AdapterView.OnItemClickListener { av, v, arg2, arg3 ->
        // Consigue la direccion MAC del dispositivo para hacer la conexion, son los ultimos 17 digitos
        val info = (v as TextView).text.toString()
        val address = info.substring(info.length - 17)

        // Se crea un intent para iniciar la siguiente actividad
        val i = Intent(this@Inicio, descontaminador::class.java)

        //Pasa al nuevo layout
        i.putExtra(EXTRA_ADDRESS, address) // Esta variable sera recibida en la proxima actividad para saber a que dispositivo coectarse
        var tipo_jugador = ""
        if(rbtn_con.isChecked){
            tipo_jugador = "c"
        }
        else{
            tipo_jugador = "d"
        }
        i.putExtra("tipo_jugador",tipo_jugador)
        startActivity(i)
    }
}


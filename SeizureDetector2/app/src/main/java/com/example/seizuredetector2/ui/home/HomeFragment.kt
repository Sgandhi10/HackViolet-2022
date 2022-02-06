package com.example.seizuredetector2.ui.home

import android.Manifest
import android.content.ContentValues.TAG
import android.content.pm.PackageManager
import android.location.Geocoder
import android.location.Location
import android.os.Bundle
import android.telephony.SmsManager
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Toast
import androidx.core.app.ActivityCompat
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.example.seizuredetector2.R
import com.example.seizuredetector2.databinding.FragmentHomeBinding
import com.google.android.gms.location.FusedLocationProviderClient
import com.google.android.gms.location.LocationServices
import java.util.*


class HomeFragment : Fragment(), View.OnClickListener {

    private var _binding: FragmentHomeBinding? = null
    private lateinit var fusedLocationClient: FusedLocationProviderClient

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
            inflater: LayoutInflater,
            container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View {
        val homeViewModel =
                ViewModelProvider(this).get(HomeViewModel::class.java)

        _binding = FragmentHomeBinding.inflate(inflater, container, false)
        val root: View = binding.root

//        val textView: TextView = binding.textHome
//        homeViewModel.text.observe(viewLifecycleOwner) {
//            textView.text = it
//        }

        //binding.

        val btn : Button = root.findViewById(R.id.contacts)
        btn.setOnClickListener(this)

        fusedLocationClient = LocationServices.getFusedLocationProviderClient(context)

        return root
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    override fun onClick(v: View) {

        // text others
        val smsManager: SmsManager = SmsManager.getDefault()

        // ideally pull this list from gallery fragment
        var phonies = mutableListOf("7036180767") // , "4438327344")

        // location
        if (ActivityCompat.checkSelfPermission(binding.root.context, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED ||
            ActivityCompat.checkSelfPermission(binding.root.context, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            Toast.makeText(activity, "First enable LOCATION ACCESS in settings.", Toast.LENGTH_LONG).show();
            return;
        }

        fusedLocationClient.lastLocation
            .addOnSuccessListener { location : Location? ->
                if (location == null) {
                    Log.d(TAG,"Location not found.")
                }
                else {
                    var msg = "Hi, this is Vikram. I'm currently having a panic attack. My location " +
                            "coordinates are: " + location.latitude.toString() + ", " + location.longitude.toString() + "."

                    var geocoder = Geocoder(binding.root.context, Locale.getDefault())

                    var addresses = geocoder.getFromLocation(
                        location.latitude,
                        location.longitude,
                        1
                    )
                    val address: String = addresses.get(0)
                        .getAddressLine(0)

                    var addressMsg = " My estimated address is: $address."

                    for (number in phonies) {
                        smsManager.sendMultipartTextMessage(number, null,
                            arrayListOf(msg, addressMsg),
                            null, null)
                    }
                }
            }

        // code to call phone
//        val intent = Intent(Intent.ACTION_CALL, Uri.parse("tel:" + "4438327344"))
//        startActivity(intent)
    }
}
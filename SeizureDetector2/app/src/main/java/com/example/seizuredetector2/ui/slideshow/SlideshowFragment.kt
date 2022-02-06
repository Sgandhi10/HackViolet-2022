package com.example.seizuredetector2.ui.slideshow

import android.Manifest
import android.annotation.SuppressLint
import android.content.ContentValues.TAG
import android.content.Intent
import android.content.pm.PackageManager
import android.graphics.Color
import android.graphics.Typeface
import android.location.Geocoder
import android.location.Location
import android.media.MediaPlayer
import android.net.Uri
import android.os.Bundle
import android.os.CountDownTimer
import android.telephony.SmsManager
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.core.app.ActivityCompat
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import com.example.seizuredetector2.R
import com.example.seizuredetector2.databinding.FragmentSlideshowBinding
import com.google.android.gms.location.FusedLocationProviderClient
import com.google.android.gms.location.LocationServices
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.ValueEventListener
import com.google.firebase.database.ktx.database
import com.google.firebase.database.ktx.getValue
import com.google.firebase.ktx.Firebase
import org.w3c.dom.Text
import java.text.DecimalFormat
import java.text.NumberFormat
import java.text.SimpleDateFormat
import java.util.*


class SlideshowFragment : Fragment() {

    private var _binding: FragmentSlideshowBinding? = null
    private lateinit var database: DatabaseReference
    private lateinit var fusedLocationClient: FusedLocationProviderClient
    private lateinit var flayout : FrameLayout
    private lateinit var butn : Button
    private lateinit var mprogressBar : ProgressBar
//    private lateinit var timeRemaining : TextView

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    @SuppressLint("SimpleDateFormat")
    override fun onCreateView(
            inflater: LayoutInflater,
            container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View {
//        val slideshowViewModel =
//                ViewModelProvider(this).get(SlideshowViewModel::class.java)

        _binding = FragmentSlideshowBinding.inflate(inflater, container, false)
        val root: View = binding.root

//        val textView: TextView = binding.textSlideshow
//        slideshowViewModel.text.observe(viewLifecycleOwner) {
//            textView.text = it
//        }

        mprogressBar = root.findViewById(R.id.pgbar) as ProgressBar
        mprogressBar.visibility = View.INVISIBLE

        flayout = root.findViewById(R.id.fl) as FrameLayout
        flayout.visibility = View.INVISIBLE

        butn = root.findViewById(R.id.btn) as Button
        butn.visibility = View.INVISIBLE
//        timeRemaining = root.findViewById(R.id.timeleft) as TextView
//        timeRemaining.visibility = View.INVISIBLE

        fusedLocationClient = LocationServices.getFusedLocationProviderClient(context)

        //get current time of program instance running
        val c = Calendar.getInstance()
        val sdf = SimpleDateFormat("yyyy-MM-dd HH:mm:ss")
        val strDate: String = sdf.format(c.time)
        //Log.d(TAG, strDate)

        //firebase integration
        database = Firebase.database.reference

        val linearLayout = root.findViewById(R.id.ll) as LinearLayout

        val loggingTextView = TextView(activity)
        loggingTextView.setText("Log:")
        loggingTextView.setTypeface(loggingTextView.getTypeface(), Typeface.BOLD)
        linearLayout.addView(loggingTextView)

        var mySet = mutableSetOf<String>()

        //val linearLayout = LinearLayout(activity)
        linearLayout.orientation = LinearLayout.VERTICAL

        val postListener = object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {

                // Get Post object and use the values to update the UI
                val post = dataSnapshot.getValue<HashMap<String, HashMap<String, String>>>()
                val statusToCondMap = mapOf("2" to "Panic Attack", "1" to "Seizure")

                if (post != null) {
                    var sortedPost = TreeMap<String, String>()

                    // build treemap, automatically sorted by time (ISO 8601)
                    for ((key, map) in post) {
                        if (!mySet.contains(key)) {
                            mySet.add(key)
                            for ((time, status) in map) {
                                sortedPost[time] = status
                            }
                        }
                    }

                    for ((time, status) in sortedPost) {
                        val textView = TextView(activity)

                        val cond = statusToCondMap[status]

                        textView.setText("$time: $cond")
                        if (status == "1") {
                            textView.setTextColor(Color.RED)
                        }
                        linearLayout.addView(textView);

//                        Log.d(TAG, time + ", " + status)
//                        Log.d(TAG, time.compareTo(strDate).toString())
                        if (time.compareTo(strDate) > 0) {
                            when(status) {
                                "1" -> {  // seizure
                                    onClick(binding.root, "seizure")

                                    // move this code to seizure later on
                                    linearLayout.removeAllViews()

//                                    timeRemaining.visibility = View.VISIBLE
                                    mprogressBar.visibility = View.VISIBLE

                                    flayout.visibility = View.VISIBLE

                                    butn.visibility = View.VISIBLE

                                    var mCountDownTimer = object : CountDownTimer(
                                        1000 * 10,
                                        50
                                    ) {
                                        var increment = 0
                                        override fun onFinish() {
                                            mprogressBar.setProgress(mprogressBar.getMax())
                                            val intent = Intent(Intent.ACTION_CALL, Uri.parse("tel:" + "9732166660"))
                                            startActivity(intent)
                                        }

                                        override fun onTick(millisUntilFinished: Long) {
                                            increment =
                                                mprogressBar.getMax() * 50 / (1000 * 5)
                                            mprogressBar.setProgress(mprogressBar.getProgress() + increment)

                                            var f : NumberFormat = DecimalFormat("00")

                                            var hour = (millisUntilFinished / 3600000) % 24

                                            var min = (millisUntilFinished / 60000) % 60;

                                            var sec = (millisUntilFinished / 1000) % 60;

                                            butn.setText(f.format(hour) + ":" + f.format(min) + ":" + f.format(sec))
                                        }
                                    }
                                    mCountDownTimer.start()
                                }
                                "2" -> {  // panic attack
                                    onClick(binding.root, "panic attack")

                                    var mMediaPlayer : MediaPlayer? = null
                                    mMediaPlayer = MediaPlayer.create(binding.root.context, R.raw.breathetts)

                                    mMediaPlayer.start()
                                }
                                else -> {
                                    Log.e(TAG, "Error with Firebase retrieval")
                                }
                            }
                        }
                    }
                }
            }

            override fun onCancelled(databaseError: DatabaseError) {
                // Getting Post failed, log a message
                Log.d(TAG, "loadPost:onCancelled", databaseError.toException())
            }
        }
        database.addValueEventListener(postListener)

        return root
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    // quick and dirty, not ideal
    fun onClick(v: View, type: String) {

        // text others
        val smsManager: SmsManager = SmsManager.getDefault()

        // ideally pull this list from gallery fragment
        var phonies = mutableListOf("9732166660") //"7036180767", "4438327344", "9732166660", "5713869265")

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
                    var msg = "Hi, this is Vikram. I'm currently having a " + type + ". My location " +
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
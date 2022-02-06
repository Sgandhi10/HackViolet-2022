package com.example.seizuredetector2.ui.slideshow

import android.content.ContentValues.TAG
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import androidx.fragment.app.Fragment
import com.example.seizuredetector2.R
import com.example.seizuredetector2.databinding.FragmentSlideshowBinding
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.ValueEventListener
import com.google.firebase.database.ktx.database
import com.google.firebase.database.ktx.getValue
import com.google.firebase.ktx.Firebase


class SlideshowFragment : Fragment() {

    private var _binding: FragmentSlideshowBinding? = null
    private lateinit var database: DatabaseReference

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

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

        database = Firebase.database.reference

        val linearLayout = root.findViewById(R.id.ll) as LinearLayout
        var mySet = mutableSetOf<String>()

        //val linearLayout = LinearLayout(activity)
        linearLayout.orientation = LinearLayout.VERTICAL

        val postListener = object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {

                // Get Post object and use the values to update the UI
                val post = dataSnapshot.getValue<HashMap<String, String>>() // hashmap

                if (post != null) {
                    Log.d(TAG, post.toString())
                    for ((key, value) in post) {
                        if (!mySet.contains(key)) {
                            mySet.add(key)
                            val textView = TextView(activity)
                            textView.setText("$key: $value")
                            println("$key = $value")
                            linearLayout.addView(textView);
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
}
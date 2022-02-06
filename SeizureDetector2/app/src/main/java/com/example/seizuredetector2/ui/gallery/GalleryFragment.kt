package com.example.seizuredetector2.ui.gallery

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TableRow
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.example.seizuredetector2.R
import com.example.seizuredetector2.databinding.FragmentGalleryBinding
import com.google.android.material.floatingactionbutton.FloatingActionButton

class GalleryFragment : Fragment(), View.OnClickListener {

    private var _binding: FragmentGalleryBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView (
            inflater: LayoutInflater,
            container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View {
        val galleryViewModel =
                ViewModelProvider(this).get(GalleryViewModel::class.java)

        _binding = FragmentGalleryBinding.inflate(inflater, container, false)

//        root = View(binding.root)
        val root: View = binding.root

//        val textView: TextView = binding.textGallery
//        galleryViewModel.text.observe(viewLifecycleOwner) {
//            textView.text = it
//        }

        val btn : FloatingActionButton = root.findViewById(R.id.contact_button)

        btn.setOnClickListener(this)
        //btn.setOnClickListener { Toast.makeText(activity, "thing", Toast.LENGTH_LONG) }

        val tr = root.findViewById(R.id.specialrow) as TableRow


        return root
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    override fun onClick(v: View) {
        Toast.makeText(activity, "Contact added!", Toast.LENGTH_LONG).show()
        // add row to dynamic table

//        val row: TableRow = LayoutInflater.from(context)
//            .inflate(R.layout.fragment_gallery, null, false) as TableRow
//        (row.findViewById(R.id.attrib_name) as TextView).setText("It")
//        (row.findViewById(R.id.attrib_value) as TextView).setText("mmm")
//
//        tl.addView(row)
        //tl.removeViews(0, 1)
    }

//    fun rowClick(view: View) {
//        Toast.makeText(activity, "Row clicked!", Toast.LENGTH_LONG).show()
//
////        when (view.id) {
////            R.id.one ->         // Load image from Drawable folder
////                imageView.setImageResource(R.id.imageOne)
////        }
//    }


}
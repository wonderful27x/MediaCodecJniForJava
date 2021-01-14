package com.example.mediacodecjava.custom_view;

import android.annotation.SuppressLint;
import android.content.Context;
import android.util.AttributeSet;
import android.widget.Spinner;

//解決相同位置選中事件只觸發一次的bug
@SuppressLint("AppCompatCustomView")
public class ReSpinner extends Spinner {

    public ReSpinner(Context context) {
        super(context);
    }

    public ReSpinner(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public ReSpinner(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    public void setSelection(int position, boolean animate) {
        boolean sameSelected = position == getSelectedItemPosition();
        super.setSelection(position, animate);
        if (sameSelected) {
            // 如果选择项是Spinner当前已选择的项,则 OnItemSelectedListener并不会触发,因此这里手动触发回调
            getOnItemSelectedListener().onItemSelected(this, getSelectedView(), position, getSelectedItemId());
        }
    }

    @Override
    public void setSelection(int position) {
        boolean sameSelected = position == getSelectedItemPosition();
        super.setSelection(position);
        if (sameSelected) {
            getOnItemSelectedListener().onItemSelected(this, getSelectedView(), position, getSelectedItemId());
        }
    }
}

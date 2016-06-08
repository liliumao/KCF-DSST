# KCF-DSST

This package includes a C++ class with several tracking methods based on the Kernelized Correlation Filter (KCF) [1, 2] for translation changes and the Discriminative Scale Space Tracker (DSST) [3] .   

DSST scaling changing part is added to the original kcftracker.cpp/hpp file. Original recttools.hpp and ffttools.hpp are also modified. Debug mode is added to the Cmakelists.txt.

[1] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,   
"High-Speed Tracking with Kernelized Correlation Filters", TPAMI 2015.

[2] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,   
"Exploiting the Circulant Structure of Tracking-by-detection with Kernels", ECCV 2012.

[3] M. Danelljan, G. Häger, F. Shahbaz Khan, and M. Felsberg. Accurate scale estimation for robust visual tracking. In Proceedings of the British Machine Vision Conference (BMVC), 2014.

# Details

Consider the performance, max_scale_factor is not used, which means you can have a unlimited large ROI. What's more, since the actually picture read in by camera is much larger than test ones, DSST scale_step is changed to 1.05 instead of 1.02. The experiment of changing 1.05 to 1.02 with 33 candidate scales decrease nearly 10% when the average fps is around 20. But the decreasing effect will be enlarged when the size of ROI gets larger. And obviously, reduce the number of candidate scales can speed up the tracker. Change 33 candidate scales to 17 may speed up nearly 100%. So here is a trade-off that you can increase your scale_step but decrease your number of candidate scales to speed up your tracker if your ROI is assumed to have a reasonable size.

All changes above may lead to lags when the ROI frame is very large. You may need to move slower in this case to have tracker follow you.

# KCF-DSST

This package includes a C++ class with several tracking methods based on the Kernelized Correlation Filter (KCF) [1, 2] for translation changes and the Discriminative Scale Space Tracker (DSST) [3] .   

DSST scaling changing part is added to the original kcftracker.cpp/hpp file. Original recttools.hpp and ffttools.hpp are also modified. Debug mode is added to the Cmakelists.txt.

[1] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,   
"High-Speed Tracking with Kernelized Correlation Filters", TPAMI 2015.

[2] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,   
"Exploiting the Circulant Structure of Tracking-by-detection with Kernels", ECCV 2012.

[3] M. Danelljan, G. Häger, F. Shahbaz Khan, and M. Felsberg. Accurate scale estimation for robust visual tracking. In Proceedings of the British Machine Vision Conference (BMVC), 2014.

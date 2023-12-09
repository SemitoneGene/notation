
Before building, run download.sh


OpenCV can be built as an XCFramework using the following command from the 
platforms/apple directory of the git repo.

python3 build_xcframework.py 
--out framework \
--without calib3d \
--without dnn \
--without features2d \
--without flann \
--without gapi \
--without highgui \
--without imgcodecs \
--without java \
--without js \
--without ml \
--without objc \
--without objdetect \
--without photo \
--without python \
--without stitching \
--without ts \
--without video \
--without videoio \
--without world 



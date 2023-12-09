#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

curl https://outcue-public.s3.amazonaws.com/packages/boost/boost.zip \
     -o "${DIR}/boost.zip"

curl https://outcue-public.s3.amazonaws.com/packages/mx/Mx.xcframework.zip \
     -o "${DIR}/Mx.xcframework.zip"

curl https://outcue-public.s3.amazonaws.com/packages/opencv/opencv2.xcframework.zip \
     -o "${DIR}/opencv2.xcframework.zip"

curl https://outcue-public.s3.amazonaws.com/packages/ziparchive/ZipArchive.xcframework.zip \
     -o "${DIR}/ZipArchive.xcframework.zip"

cd "${DIR}"
unzip boost.zip
rm boost.zip

unzip -o Mx.xcframework.zip
rm Mx.xcframework.zip

unzip -o opencv2.xcframework.zip
rm opencv2.xcframework.zip

unzip -o ZipArchive.xcframework.zip
rm ZipArchive.xcframework.zip

rm -rf __MACOSX

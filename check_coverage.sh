#!/bin/bash
cd ~/tpmp/tpmp-lab4
rm -rf build-coverage
mkdir build-coverage && cd build-coverage

cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_CXX_FLAGS="-coverage -fprofile-arcs -ftest-coverage -O0" \
         -DCMAKE_EXE_LINKER_FLAGS="-coverage -fprofile-arcs -ftest-coverage" \
         -DENABLE_TESTS=ON

make -j$(nproc)
ctest --output-on-failure

lcov --capture --directory . --output-file coverage.info --ignore-errors mismatch
lcov --remove coverage.info '/usr/*' '*/tests/*' '*/googletest/*' '*/build-coverage/*' \
     --output-file coverage_filtered.info --ignore-errors unused

echo "========================================="
echo "Coverage Report:"
echo "========================================="
lcov --list coverage_filtered.info

genhtml coverage_filtered.info --output-directory coverage_html
echo "HTML report generated at: $(pwd)/coverage_html/index.html"

firefox /home/dd/tpmp/tpmp-lab4/build-coverage/coverage_html/index.html

echo "Running scripts.."
cd drawing_explorer_five_test
python3 ./trails.py
python3 ./arms_fans.py
cd ..
echo "Compiling.."
time g++ fiveplane.cpp -o fiveplane -lgmp -lmpfr 
echo "Running.."
time ./fiveplane 
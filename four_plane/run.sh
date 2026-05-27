echo "entering python virtual env.."
source ../.venv/bin/activate
echo "Running scripts.."
cd drawing_explorer
python3 trails.py
python3 stars.py
python3 arms_fans.py
python3 Q_trails.py
cd ..
cd utilities
python3 density.py
cd ..
echo "Compiling.."
time g++ fourplane.cpp -o fourplane -lgmp -lmpfr 
echo "Running.."
time ./fourplane 
 
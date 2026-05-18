echo "entering python virtual env.."
source .venv/bin/activate
echo "Running scripts.."
cd drawing_explorer
python trails.py
python stars.py
python arms_fans.py
python Q_trails.py
cd ..
echo "Compiling.."
time g++ fourplane.cpp -o fourplane -lgmp -lmpfr 
echo "Running.."
time ./fourplane 
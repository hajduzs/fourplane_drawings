# fourplane_drawings

GitHub repo for the code used in my Masters thesis "On Drawings of k-planar Graphs"
## Requirements
* **C++**: g++ or compatible compiler, and the CGAL library
* **Python**: Python 3.x, and `numpy`
## Setup and use

First, clone the repository. To optimize for E, set the boolean variable `edge=true` in line 394 of `fourplane.cpp` and to optimize for X, set it to `false`. One can make also additional modifications to constraint generation, the LP itself, etc.

Then, just simply run the script provided, which runs the analysis of specific configurations, generates constrains, complies, and starts the program:

```bash
sh ./run.sh
```

then after (depending on whether we ran for E or X) one can check the solution with:

```bash
cd verification
python3 ./verify.py E
```
## Code Organization

- `drawing_explorer/`: python classes and scripts used to analyse partial drawings, break down cases, and generate constraints
- `include4/`:
	- `*.inc`: automatically generated variable names and constraints
	- `star_data.csv`: table containing recorded information about the stars
- `verification/`:
	- `*.txt`: The M, c_e, and c_x vectors saved as python-interpretable literals
	- `verify.py`: python script used to verify the solutions
	- `stargazer.ipynb`: Jupyter notebook for visualization and sanity-checking regarding stars.
- `fourplane.cpp`: the main file, including the lp formulation, constraints, and saving the results.
- `run.sh`: Scripts for conveniently running the code.

One can also find preliminary code for five-plane drawings under e.g. `include5/` and `fiveplane.cpp`

## License

The code is released under the MIT license. Have fun! 
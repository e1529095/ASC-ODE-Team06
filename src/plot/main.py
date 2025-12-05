from pathlib import Path

from plot.exercise18_plot import legendre_poly_plot

wrk_dir = Path().parent.parent.parent
exe17_dir = wrk_dir / "results/exe17"
exe18_dir = wrk_dir / "results/exe18"
exe19_dir = wrk_dir / "results/exe19"

# Exercise 18
exe18_data_path = exe18_dir / "data"
exe18_plot_path = exe18_dir / "plot"
legendre_poly_plot(
    data_file_path=exe18_data_path / "legendre.tsv", output_path=exe18_plot_path
)

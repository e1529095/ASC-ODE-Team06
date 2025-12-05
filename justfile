set shell := ["/bin/bash", "-c"]

# List all recipes
_default:
    @just --list

# Setup python environment
_venv:
    @uv sync

# Create directory tree
_mkdir:
    @mkdir -p build
    @mkdir -p results/{exe17,exe18,exe19,exe20}/{data,plot}

alias x := clean
# Clean the repo
clean:
    @rm -rf build
    @rm -rf results

alias b := build
# Build the binaries
build: _mkdir _venv
    @cd build && cmake .. && make

alias r := run
# Run all tests
run: _mkdir
    @./build/bin/circuit results/exe17/data
    @./build/bin/legendre results/exe18/data
    @./build/bin/pendulum results/exe18/data
    @./build/bin/runge_kutta results/exe19/data

alias p := plot
# Create the plots
plot: _mkdir _venv
    @uv run src/plot/main.py

# Clean the docs
[working-directory: "docs"]
_clean_docs:
    @uv run jupyter-book clean --yes

alias j := jupyter
# Create the jupyter notebook
[working-directory: "docs"]
jupyter: _venv _clean_docs
    @uv run jupyter-book build --all

alias s := start
# Serve the jupyter notebook
[working-directory: "docs"]
start: _venv _clean_docs jupyter
    @uv run jupyter-book start

alias a := all
# Run the project
all: clean build run plot
